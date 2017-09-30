#include "includes.h"
#include "UCP2PMgr.h"

namespace uc {

	UCP2PMgr *glpP2P = NULL;
	void *createInstanceP2P(const ISink *sink){

		if (NULL == glpP2P) {
			glpP2P = new UCP2PMgr();
			glpP2P->setMgrSink(sink);
		}

		return (void *)glpP2P;
	}

	void destroyInstanceP2P(void *lp){

		if (NULL != glpP2P) {
			delete (UCP2PMgr *)glpP2P;
			glpP2P = NULL;
		}
	}


	UCP2PMgr::UCP2PMgr() {

		m_WanIP = 0;
		m_LanIP = 0;
		m_port = 0;

		mlpFileMgr = NULL;
		mlpPConnMgr = NULL;
		mlpCIMSConnMgr = NULL;
	}

	UCP2PMgr::~UCP2PMgr() { }

	int UCP2PMgr::init() 
	{

		LOGI("UCP2PMgr::init");
		int ret = P2P_ERROR_1;
		do 
		{
			mlpFileMgr = (IFileMgr *)createInstanceFM((IFileSink *)this);
			ret = mlpFileMgr->init();
			if (P2P_ERROR_SUCCESSED != ret)
			{
				break;
			}

			mlpPConnMgr = (IPeerConnMgr *)createInstancePC((IPeerConnSink *)this);
			ret = mlpPConnMgr->init();
			if (P2P_ERROR_SUCCESSED != ret)
			{
				break;
			}
		} while (false);

		m_Timer.setSink(this);
		m_Timer.startTimer();

		return ret;
	}

	void UCP2PMgr::destroy() {
	}

	void UCP2PMgr::onNotify(int nType) const 
	{
		switch(nType)
		{
		case ST_CIMSCONN:{ LOGI("ST_CIMSCONN"); } break;
		case ST_FILEMGR:{ LOGI("ST_FILEMGR"); } break;
		case ST_FSCONN:{ LOGI("ST_FSCONN"); } break;
		case ST_NETWORK:{ LOGI("ST_NETWORK"); } break;
		case ST_PEERCONN:{ LOGI("ST_PEERCONN"); } break;
		default:{ LOGI("default %d", nType); } break;
		}
	}

	void UCP2PMgr::onProgress(bool isUp, int progress)
	{
		LOGI("%s onProgress %d ", isUp ? "Up" : "Down", progress);
		// getSink()->onUploadFile();
	}

	int32_t UCP2PMgr:: uploadFile(bool bToFS, std::wstring path, 
		int32_t currSize, int32_t peerID, int32_t remoteIP, int16_t port)
	{
		LOGI("处理文件上传");
		if (m_WanIP != remoteIP)
		{
			return P2P_ERROR_2;
		}

		File_Key fk(0, peerID);
		int ret = mlpFileMgr->initFile(true, path, currSize, fk);
		if (P2P_ERROR_SUCCESSED != ret)
		{
			return ret;
		}

		FT_OperateTask *lpT = addOTask(true, fk);
		// 是否同一 LAN
		ret = mlpPConnMgr->connPeer(fk, remoteIP, port);

		return ret;
	}

	int32_t UCP2PMgr::downloadFile(bool bToFS, std::wstring path, 
		int32_t fileSize, int32_t peerID, int32_t remoteIP, int16_t port)
	{
		LOGI("处理文件下载");
		if (remoteIP != m_WanIP)
		{
			return P2P_ERROR_1;
		}

		File_Key fk(0, peerID);
		int ret = mlpFileMgr->initFile(false, path, fileSize, fk);
		if (P2P_ERROR_SUCCESSED != ret)
		{
			return ret;
		}

#ifdef _TEST
		mDownPeerID = peerID;
		mFromUserID = 7772;
		mDownFileID = fk.getFileID();
#endif // _TEST

		FT_OperateTask *lpT = addOTask(false, fk);

		ret = mlpPConnMgr->connPeer(fk, remoteIP, port);

		return ret;
	}

	//////////////////////////////////////////////////////////////////////////
	void UCP2PMgr::onConnPeer(File_Key &fk)
	{
		if (mlpFileMgr->isUpload(fk))
		{ // 通知UI可以进行P2P
			getSink()->onNotify(ST_FILETRANSFER);
			return ;
		}

#ifdef _TEST
		mlpPConnMgr->accept(fk, mFromUserID);
#endif // _TEST

		// 和上传方握手.

		// 握手后数据操作
		BlockInfo bi;
		int ret = getNextBalckInfo(fk.getFileID(), bi);
		if (P2P_ERROR_SUCCESSED == ret)
		{
			ret = mlpPConnMgr->sendWriteRequest(fk, bi._id, bi._offset, 120);
		}

		// 连通结果
		getSink()->onNotify(ST_FILETRANSFER);
	}

	void UCP2PMgr::onRead(File_Key &fk, int32_t blockID, int32_t blockOffset, int32_t len)
	{ // 对方希望读取的数据
		int ret = P2P_ERROR_2;
		FT_OperateTask *lpOT = getOTask(fk.getFileID());
		if (NULL != lpOT)
		{
			int32_t fileOffset = lpOT->getFileOffset(blockID) + blockOffset;

			char *buf = new char[len];
			ret = mlpFileMgr->read(fk, fileOffset, len, buf);
			if (P2P_ERROR_SUCCESSED == ret)
			{
				ret = mlpPConnMgr->sendReadResponse(fk, blockID, blockOffset, len, buf);
			}
			delete []buf;
		}

		if (P2P_ERROR_SUCCESSED != ret)
		{ // 通知对方异常
			mlpPConnMgr->sendErrorCode(fk, blockID, ret);
		}
	}

	void UCP2PMgr::onWrite(File_Key &fk, int32_t blockID, int32_t blockOffset, int32_t len, char *buf)
	{ // 保存获取到的数据
		//LOGI("onWrite");
		int ret = P2P_ERROR_2;
		do 
		{
			FT_OperateTask *lpOT = getOTask(fk.getFileID());
			if (NULL == lpOT)
			{
				break;
			}

			int32_t fileOffset = lpOT->getFileOffset(blockID) + blockOffset;
			ret = mlpFileMgr->write(fk, fileOffset, len, buf);
			if (P2P_ERROR_SUCCESSED != ret)
			{
				break;
			}

			ret = lpOT->updateBlockInfo(blockID, blockOffset, len);
			if (P2P_ERROR_SUCCESSED == ret)
			{ // 当前块未完成
				blockOffset += len;
			}
			else if (P2P_ERROR_4 == ret)
			{
				// 通知 CIMS bi._id 完成
				LOGI("块 %d 完成", blockID);
				// 计算下一个需要获取的数据
				BlockInfo bi;
				bi._id = blockID;
				bi._id++;
				ret = getNextBalckInfo(fk.getFileID(), bi);
				if (P2P_ERROR_SUCCESSED == ret)
				{
					blockID = bi._id;
					blockOffset = bi._offset;
				}
			}
			else
			{
				break;
			}
		} while (false);

		if (P2P_ERROR_SUCCESSED == ret)
		{
			ret = mlpPConnMgr->sendWriteRequest(fk, blockID, blockOffset, len);
		}
		else
		{ // 通知对方异常
			ret = mlpPConnMgr->sendErrorCode(fk, blockID, ret);
		}
	}

	void UCP2PMgr::onErrorCode(File_Key &fk, int32_t blockID, int32_t ret)
	{
		LOGI("ErrorCode PeerID %d, FileID %d, blockID %d, ret %d", 
			fk.getPeerID(), fk.getFileID(), blockID, ret);
	}

	void UCP2PMgr::onAccept(int32_t peerID, int32_t &fromUserID, int32_t &fileID, int32_t &fileSize)
	{
#ifdef _TEST
		assert(peerID == mFromUserID);

		fromUserID = mDownPeerID;
#endif // _TEST

		std::wstring path =  L"D:\\VS2013\\exportSDK\\src\\UC_SDK.cpp";
		LOGI("被动处理 文件 上传");

		File_Key fk(0, peerID);
		mlpFileMgr->initFile(true, path, 0, fk);
		fileID = fk.getFileID();

		addOTask(true, fk);
	}

	void UCP2PMgr::onGetWanIP(int32_t wanIP)
	{
		m_WanIP = wanIP;
	}

	void UCP2PMgr::onTimerProc()
	{
		//LOGI("timerProc");
	}

	FT_OperateTask * UCP2PMgr::addOTask(bool isUp, File_Key &fk)
	{
		int32_t blockSize = 0; 
		mlpFileMgr->getTorrentInfo(isUp, fk, blockSize);

		FT_OperateTask *lpT = new FT_OperateTask(fk.getFileID(), 1, blockSize, 0);

		m_lstOT.push_back(lpT);

		return lpT;
	}

	FT_OperateTask* UCP2PMgr::getOTask(int32_t fileID)
	{
		LSTOTASK::iterator it;  
		for( it = m_lstOT.begin(); it != m_lstOT.end(); ++it)  
		{  
			FT_OperateTask *lpT = *it;
			if (lpT->isOwn(fileID))
			{
				return lpT;
			}
		}

		return (FT_OperateTask*)NULL;
	}

	int UCP2PMgr::getNextBalckInfo(int32_t fileID, BlockInfo &bi)
	{
		FT_OperateTask *lpOT = getOTask(fileID);

		// 获取发布的文件快信息
		// 1.	优选完整快少的先下载
		// 排序
		//for (;;)
		{
			// 2.	相同片段优选连接数少的下载
			// 获取当前块持用用户列表
			// 排序
			//for (;;)
			{
				// 判断和当前用户是否存在连接
				// 3.	相同条件 随机选择
			}
		}

		lpOT->addBlockInfo(bi);

		return P2P_ERROR_SUCCESSED;
	}

} // namespace