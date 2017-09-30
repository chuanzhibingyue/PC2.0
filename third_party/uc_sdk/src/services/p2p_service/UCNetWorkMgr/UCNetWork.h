#pragma once

#include "IONetWorkMgr.h"
#include "UCWndMsg.h"

namespace uc {

class UCNetWork : public INetWorkMgr, public IWndMsgSink
{
public:
	UCNetWork();
	~UCNetWork();

public:
	virtual int init();
	virtual void destroy();

	virtual int32_t createSession();
	virtual int connect(int32_t session, int32_t fileID, int32_t peerID);
	virtual int send(int32_t session, int16_t cmd, int32_t len, const char *buf);
#ifdef _TEST
	virtual int accept(int32_t session, int32_t fileID, int32_t mFromUserID);
#endif // _TEST

protected:
	virtual void onWndMsgProc(UINT message, WPARAM wParam, LPARAM lParam, bool &ret);

	INetWorkSink *getSink() const { return (INetWorkSink *)mlpSink; }

private:
	UCWndMsg m_WMsg;
	std::pair<int32_t, int32_t> mPairSession;
};

}