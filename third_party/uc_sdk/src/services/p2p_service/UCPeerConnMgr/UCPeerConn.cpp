#include "includes.h"
#include "UCPeerConn.h"

#include <boost\bind.hpp>

namespace uc {

	enum {
		pdu_base = 100,

		pdu_read_response,

		pdu_write_request,

		pdu_error_code,
	};

	UCPeerConn *glpPC = NULL;
	void *createInstancePC(const ISink *sink){

		if (NULL == glpPC)
		{
			glpPC = new UCPeerConn();
			glpPC->setMgrSink(sink);
		}

		return (void *)glpPC;
	}

	void destroyInstancePC(void *lp){

		if (NULL != glpPC)
		{
			delete (UCPeerConn *)glpPC;
			glpPC = NULL;
		}
	}

	UCPeerConn::UCPeerConn()
	{
	}

	UCPeerConn::~UCPeerConn()
	{
	}

	int UCPeerConn::init()
	{
		getSink()->onNotify(ST_PEERCONN);

		m_lpNWMgr = (INetWorkMgr *)createInstanceNW((INetWorkSink *)this);
		m_lpNWMgr->init();

		return P2P_ERROR_SUCCESSED;
	}

	void UCPeerConn::destroy()
	{

	}

	int UCPeerConn::connPeer(File_Key &fk, int32_t remoteIP, int16_t port)
	{
		MAPSINFO::iterator it = m_mapSInfo.find(fk);
		if (m_mapSInfo.end() != it)
		{
			return P2P_ERROR_3;
		}

		int32_t session = m_lpNWMgr->createSession();
		addSInfo(fk, session);

		int ret = m_lpNWMgr->connect(session, fk.getFileID(), fk.getPeerID());

		return ret;
	}

	int UCPeerConn::sendReadResponse(File_Key &fk, int32_t blockID, int32_t blockOffset, int32_t len, char *buf)
	{
		MAPSINFO::iterator it = m_mapSInfo.find(fk);
		if (m_mapSInfo.end() == it)
		{
			return P2P_ERROR_3;
		}

		Session_info *si = (Session_info *)it->second;

		int dataLen = sizeof(int32_t) *3 + len;
		char *lpT = new char[dataLen];
		((int32_t *)lpT)[0] = blockID;
		((int32_t *)lpT)[1] = blockOffset;
		((int32_t *)lpT)[2] = len;
		if (0 < len && NULL != buf)
		{
			memcpy(lpT + (dataLen - len), buf, len);
		}

		int ret = m_lpNWMgr->send(si->_session, pdu_read_response, dataLen, lpT);

		delete []lpT;

		return (ret > 0) ? P2P_ERROR_SUCCESSED : P2P_ERROR_5;
	}

	int UCPeerConn::sendWriteRequest(File_Key &fk, int32_t blockID, int32_t blockOffset, int32_t len)
	{
		MAPSINFO::iterator it = m_mapSInfo.find(fk);
		if (m_mapSInfo.end() == it)
		{
			return P2P_ERROR_3;
		}

		Session_info *si = it->second;

		int dataLen = sizeof(int32_t) *3;
		char *lpT = new char[dataLen];
		((int32_t *)lpT)[0] = blockID;
		((int32_t *)lpT)[1] = blockOffset;
		((int32_t *)lpT)[2] = len;

		int ret = m_lpNWMgr->send(si->_session, pdu_write_request, dataLen, lpT);

		delete []lpT;

		return (ret > 0) ? P2P_ERROR_SUCCESSED : P2P_ERROR_5;
	}

	int UCPeerConn::sendErrorCode(File_Key &fk, int32_t blockID, int32_t ret)
	{
		MAPSINFO::iterator it = m_mapSInfo.find(fk);
		if (m_mapSInfo.end() == it)
		{
			return P2P_ERROR_3;
		}

		Session_info *si = it->second;

		int dataLen = sizeof(int32_t) *2;
		char *lpT = new char[dataLen];
		((int32_t *)lpT)[0] = blockID;
		((int32_t *)lpT)[1] = ret;

		ret = m_lpNWMgr->send(si->_session, pdu_error_code, dataLen, lpT);

		delete []lpT;

		return (ret > 0) ? P2P_ERROR_SUCCESSED : P2P_ERROR_5;
	}

#ifdef _TEST
	int UCPeerConn::accept(File_Key &fk, int32_t mFromUserID)
	{
		MAPSINFO::iterator it = m_mapSInfo.find(fk);
		if (m_mapSInfo.end() == it)
		{
			return P2P_ERROR_3;
		}

		Session_info *si = it->second;

		m_lpNWMgr->accept(si->_session, fk.getFileID(), mFromUserID);
		return P2P_ERROR_4;
	}
#endif // _TEST

	void UCPeerConn::onNotify(int nType) const
	{

	}

	int UCPeerConn::onConn(int32_t session, int ret, int32_t fileID, int32_t peerID)
	{
		File_Key fk;
		getSInfo(session, fk);

		getSink()->onConnPeer(fk);

		return 1;
	}

	int UCPeerConn::OnAccept(int32_t session, int ret, int32_t fileID, int32_t peerID)
	{
		int32_t fromUserID = 0;
		int32_t localFileID = 0;
		int32_t fileSize = 0;
		getSink()->onAccept(peerID, fromUserID, localFileID, fileSize);

		File_Key fk(fileID, fromUserID);
		MAPSINFO::iterator it = m_mapSInfo.find(fk);
		if (m_mapSInfo.end() == it)
		{
			return P2P_ERROR_3;
		}

		Session_info *si = it->second;
		LOGI("主动方 ID %d FileID %d 通过 Session %d 连接 被动方", peerID, fileID, si->_session);
		LOGI("被动方 ID %d FileID %d, 通过 Session %d 连接 主动方", fromUserID, localFileID, session);

		File_Key fkT(localFileID, peerID);
		addSInfo(fkT, session);

		addRInfo(fkT, session);
		return 1;
	}

	int UCPeerConn::onRecv(int32_t session, int32_t len, char *buf)
	{
		int16_t cmd = ((int16_t *)buf)[0];
		char *lpData = buf + sizeof(int16_t);
		switch (cmd)
		{
		case pdu_read_response:
			{
				File_Key fk;
				getSInfo(session, fk);

				int32_t blockID = ((int32_t *)lpData)[0];
				int32_t blockOffset = ((int32_t *)lpData)[1];
				int32_t len = ((int32_t *)lpData)[2];
				char *data = NULL;

				getSink()->onWrite(fk, blockID, blockOffset, len, data);
			} break;
		case pdu_write_request:
			{
				File_Key fk;
				getSInfo(session, fk);

				int32_t blockID = ((int32_t *)lpData)[0];
				int32_t blockOffset = ((int32_t *)lpData)[1];
				int32_t len = ((int32_t *)lpData)[2];

				getSink()->onRead(fk, blockID, blockOffset, len);
			} break;
		case pdu_error_code:
			{
				File_Key fk;
				getSInfo(session, fk);

				int32_t blockID = ((int32_t *)lpData)[0];
				int32_t ret = ((int32_t *)lpData)[1];

				getSink()->onErrorCode(fk, blockID, ret);
			} break;
		}
		return 1;
	}

	int UCPeerConn::onError(int32_t session, int ret)
	{
		return 1;
	}

	bool UCPeerConn::addSInfo(File_Key &fk, int32_t session)
	{
		Session_info *lpT = new Session_info();
		lpT->_session = session;
		m_mapSInfo.insert(MAPSINFO::value_type(fk, lpT));

		LOGI("[PeerID %d, FileID %d] <==> session %d", fk.getPeerID(), fk.getFileID(), session);

		return true;
	}

	bool UCPeerConn::getSInfo(int32_t session, File_Key &fk)
	{
		MAPSINFO::iterator it;
		for(it = m_mapSInfo.begin(); it != m_mapSInfo.end(); ++it)
		{
			if (((Session_info *)(it->second))->_session == session)
			{
				fk = it->first;
				Session_info *lpT = (Session_info *)(it->second);
// 				LOGI("getSInfo session: %d == > [PeerID: %d, FileID %d]", 
// 					lpT->_session, fk.getPeerID(), fk.getFileID());
				return true;
			}
		}

		return false;
	}

	bool UCPeerConn::addRInfo(File_Key &fk, int32_t session)
	{
		// File_Key *lpT = new File_Key(fileID, peerID);
		// m_mapRInfo.insert(MAPSINFO::value_type(session, lpT));

		return true;
	}

	bool UCPeerConn::getRInfo(File_Key &fk, int32_t session)
	{
		return true;
	}

}

