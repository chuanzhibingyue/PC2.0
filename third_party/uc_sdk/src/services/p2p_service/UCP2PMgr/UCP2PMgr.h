#pragma once

#include "IOP2PMgr.h"
#include "IOFileMgr.h"
#include "IOPeerConnMgr.h"
#include "IOCIMSConnMgr.h"
#include "UCTimer.h"

namespace uc {

	class UCP2PMgr : 
		public IP2PMgr, 
		public IFileSink, 
		public IPeerConnSink,
		public ICIMSConnSink, 
		public ITimerSink
	{
	public:
		// IFileTransferMgr
		virtual int init();
		virtual void destroy();

		virtual int32_t uploadFile(bool bToFS, std::wstring path, int32_t currSize, int32_t peerID, int32_t remoteIP, int16_t port);

		virtual int32_t downloadFile(bool bToFS, std::wstring path, int32_t fileSize, int32_t peerID, int32_t remoteIP, int16_t port);

	protected:
		// ISink
		virtual void onNotify(int nType) const;
		// IFileSink
		virtual void onProgress(bool isUp, int progress); // 进度通知
		// IPeerConnSink
		virtual void onConnPeer(File_Key &fk);
		virtual void onRead(File_Key &fk, int32_t blockID, int32_t blockOffset, int32_t len);
		virtual void onWrite(File_Key &fk, int32_t blockID, int32_t blockOffset, int32_t len, char *buf);
		virtual void onErrorCode(File_Key &fk, int32_t blockID, int32_t ret);
		virtual void onAccept(int32_t peerID, int32_t &fromUserID, int32_t &fileID, int32_t &fileSize);

		// ICIMSConnSink
		virtual void onGetWanIP(int32_t wanIP);

		virtual void onTimerProc();

	private:
		FT_OperateTask * addOTask(bool isUp, File_Key &fk);
		FT_OperateTask * getOTask(int32_t fileID);
		int getNextBalckInfo(int32_t fileID, BlockInfo &bi);

	public:
		UCP2PMgr();
		virtual ~UCP2PMgr();

	protected:
		IP2PSink *getSink() const { return (IP2PSink *)mlpSink; }

	private:
		int32_t m_WanIP;
		int32_t m_LanIP;
		int16_t m_port;

		UCTimer m_Timer;

		IFileMgr *mlpFileMgr;
		IPeerConnMgr *mlpPConnMgr;
		ICIMSConnMgr *mlpCIMSConnMgr;

		LSTOTASK m_lstOT;

#ifdef _TEST
		int32_t mDownPeerID; // 从哪个用户下载
		int32_t mDownFileID; // 下载方文件ID

		int32_t mFromUserID; // 谁要下载
#endif // _TEST
	};
}