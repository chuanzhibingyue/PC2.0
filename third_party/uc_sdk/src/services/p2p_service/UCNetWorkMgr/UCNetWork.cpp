#include "includes.h"
#include "UCNetWork.h"
#include "UCUtil.h"

namespace uc {

	UCNetWork *glpNW = NULL;
	void *createInstanceNW(const ISink *sink){

		if (NULL == glpNW) {
			glpNW = new UCNetWork();
			glpNW->setMgrSink(sink);
		}

		return (void *)glpNW;
	}

	void destroyInstanceNW(void *lp){

		if (NULL != glpNW)
		{
			delete (UCNetWork *)glpNW;
			glpNW = NULL;
		}
	}

	UCNetWork::UCNetWork()
	{
	}


	UCNetWork::~UCNetWork()
	{
	}

	int UCNetWork::init()
	{
		getSink()->onNotify(ST_NETWORK);
		m_WMsg.setSink(this);
		m_WMsg.openWndMsg();
		return 1;
	}

	void UCNetWork::destroy()
	{

	}

	int32_t UCNetWork::createSession() { 

		static int32_t session = 1000;
		return session++;
	}

	int UCNetWork::connect(int32_t session, int32_t fileID, int32_t peerID) {

		int dataLen = sizeof(int32_t) *3;
		char *lpT = new char[dataLen];

		((int32_t *)lpT)[0] = session;
		((int32_t *)lpT)[1] = fileID;
		((int32_t *)lpT)[2] = peerID;

		m_WMsg.postMsg(WM_USER_CONN, dataLen, (LPARAM)lpT);

		return P2P_ERROR_SUCCESSED;
	}

	int UCNetWork::send(int32_t session, int16_t cmd, int32_t len, const char *buf) { 

		int dataLen = sizeof(int32_t) *2 + sizeof(int16_t) + len;

		char *lpT = new char[dataLen];

		((int32_t *)lpT)[0] = session;
		((int32_t *)lpT)[1] = len;
		((int16_t *)(lpT + (sizeof(int32_t) *2)))[0] = cmd;

		if (len > 0 && NULL != buf)
		{
			memcpy(lpT + (dataLen - len), buf, len);
		}

		m_WMsg.postMsg(WM_USER_SEND, dataLen, (LPARAM)lpT);

		return 1;
	}

#ifdef _TEST
	int UCNetWork::accept(int32_t session, int32_t fileID, int32_t mFromUserID)
	{
		int dataLen = sizeof(int32_t) *3;
		char *lpT = new char[dataLen];

		((int32_t *)lpT)[0] = session;
		((int32_t *)lpT)[1] = fileID;
		((int32_t *)lpT)[2] = mFromUserID;

		m_WMsg.postMsg(WM_USER_ACCEPT, dataLen, (LPARAM)lpT);

		return 1;
	}
#endif // _TEST

	void UCNetWork::onWndMsgProc(UINT message, WPARAM wParam, LPARAM lParam, bool &ret)
	{
		if (WM_USER_CONN == message)
		{
			ret = true;

			char *lpT = (char *)lParam;
			int32_t session = ((int32_t *)lpT)[0];
			int32_t fileID = ((int32_t *)lpT)[1];
			int32_t peerID = ((int32_t *)lpT)[2];

			getSink()->onConn(session, 0, fileID, peerID);

			delete []lpT;
			lpT = NULL;
		}
		else if (WM_USER_ACCEPT == message)
		{
			ret = true;
			mPairSession.second = createSession();

			char *lpT = (char *)lParam;
			mPairSession.first = ((int32_t *)lpT)[0];
			int32_t fileID = ((int32_t *)lpT)[1];
			int32_t peerID = ((int32_t *)lpT)[2];

			getSink()->OnAccept(mPairSession.second, 0, fileID, peerID);

			delete []lpT;
			lpT = NULL;
		}
		else if (WM_USER_SEND == message)
		{
			ret = true;

			int32_t dataLen = wParam;

			char *lpT = (char *)lParam;
			int32_t session = (((int32_t *)lpT)[0] != mPairSession.first) ? mPairSession.first : mPairSession.second;
			int32_t len = ((int32_t *)lpT)[1];
			// cmd data
			getSink()->onRecv(session, len, lpT + (sizeof(int32_t) *2));
			delete []lpT;
			lpT = NULL;
		}
	}

}