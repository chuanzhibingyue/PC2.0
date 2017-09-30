#pragma once
#include "IOPeerConnMgr.h"
#include "IONetWorkMgr.h"

namespace uc {

	struct Session_info{
		int32_t _session;
	};

	typedef LckMap<File_Key, Session_info *> MAPSINFO;
	typedef LckMap<int32_t, File_Key *> MAPREMOTEINFO; // key session

	class UCPeerConn : public IPeerConnMgr, INetWorkSink
	{
	public:
		UCPeerConn();
		~UCPeerConn();

	public:
		virtual int init();
		virtual void destroy();

		// IPeerConnMgr
		virtual int connPeer(File_Key &fk, int32_t remoteIP, int16_t port);
		virtual int sendReadResponse(File_Key &fk, int32_t blockID, int32_t blockOffset, int32_t len, char *buf);
		virtual int sendWriteRequest(File_Key &fk, int32_t blockID, int32_t blockOffset, int32_t len);
		virtual int sendErrorCode(File_Key &fk, int32_t blockID, int32_t ret);
		virtual bool isExistSession(int32_t peerID) { return false; }
#ifdef _TEST
		virtual int accept(File_Key &fk, int32_t mFromUserID);
#endif // _TEST
		
		// INetWorkSink
		virtual void onNotify(int nType) const;
		virtual int onConn(int32_t session, int ret, int32_t fileID, int32_t peerID);
		virtual int OnAccept(int32_t session, int ret, int32_t fileID, int32_t peerID);
		virtual int onRecv(int32_t session, int32_t len, char *buf);
		virtual int onError(int32_t session, int ret);

	protected:
		bool addSInfo(File_Key &fk, int32_t session);
		bool getSInfo(int32_t session, File_Key &fk);

		bool addRInfo(File_Key &fk, int32_t session);
		bool getRInfo(File_Key &fk, int32_t session);

	protected:
		IPeerConnSink *getSink() const { return (IPeerConnSink *)mlpSink; }
		MAPSINFO m_mapSInfo;
		MAPREMOTEINFO m_mapRInfo; // remoteInfo

		INetWorkMgr *m_lpNWMgr;
	};
}

