#pragma once

#include "UCUtil.h"
#include "IOCIMSConnMgr.h"
#include "IONetWorkMgr.h"

namespace uc {

	class UCCIMSConn : public ICIMSConnMgr, public INetWorkSink
	{
	public:
		UCCIMSConn();
		virtual ~UCCIMSConn();

	public:
		virtual int init();
		virtual void destroy();

		virtual int getWanIP();
		// INetWorkSink
		virtual void onNotify(int nType) const;
		virtual int onConn(int32_t session, int ret, int32_t fileID, int32_t peerID);
		virtual int OnAccept(int32_t session, int ret, int32_t fileID, int32_t peerID);
		virtual int onRecv(int32_t session, int len, char *buf);
		virtual int onError(int32_t session, int ret);

	protected:
		ICIMSConnSink *getSink() const { return (ICIMSConnSink *)mlpSink; }

	private:
		INetWorkMgr *m_lpNWMgr;
	};
}
