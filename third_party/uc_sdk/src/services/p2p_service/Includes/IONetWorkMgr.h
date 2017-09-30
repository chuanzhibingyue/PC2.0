#pragma once

#include "IOMgr.h"

namespace uc {

	void *createInstanceNW(const ISink *sink);
	void destroyInstanceNW(void *lp);

	class INetWorkSink : public ISink
	{
	public:
		virtual int onConn(int32_t session, int ret, int32_t fileID, int32_t peerID) = 0;
		virtual int OnAccept(int32_t session, int ret, int32_t fileID, int32_t peerID) = 0;
		virtual int onRecv(int32_t session, int len, char *buf) = 0;
		virtual int onError(int32_t session, int ret) = 0;
	};

	class INetWorkMgr : public IMgr
	{
	public:
		virtual int32_t createSession() = 0;
		virtual int connect(int32_t session, int32_t fileID, int32_t peerID) = 0;
		virtual int send(int32_t session, int16_t cmd, int32_t len, const char *buf) = 0;
#ifdef _TEST
		virtual int accept(int32_t session, int32_t fileID, int32_t mFromUserID) = 0;
#endif // _TEST
		// 	int 连接();
		// 	void 断开();
		// 	int 发送();
		// 	int 接收();
		// 	void on异常();
	};

}