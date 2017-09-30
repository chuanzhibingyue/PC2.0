#pragma once

#include "IOMgr.h"
#include "UCUtil.h"

namespace uc {

	void *createInstancePC(const ISink *sink);
	void destroyInstancePC(void *lp);

	class IPeerConnSink : public ISink
	{
	public:
		virtual void onConnPeer(File_Key &fk) = 0;
		virtual void onRead(File_Key &fk, int32_t blockID, int32_t blockOffset, int32_t len) = 0;
		virtual void onWrite(File_Key &fk, int32_t blockID, int32_t blockOffset, int32_t len, char *buf) = 0;
		virtual void onErrorCode(File_Key &fk, int32_t blockID, int32_t ret) = 0;

		virtual void onAccept(int32_t peerID, int32_t &fromUserID, int32_t &fileID, int32_t &fileSize) = 0;

	};

	class IPeerConnMgr : public IMgr
	{
	public:
		virtual int connPeer(File_Key &fk, int32_t remoteIP, int16_t port) = 0;
		virtual int sendReadResponse(File_Key &fk, int32_t blockID, int32_t blockOffset, int32_t len, char *buf) = 0;
		virtual int sendWriteRequest(File_Key &fk, int32_t blockID, int32_t blockOffset, int32_t len) = 0;
		virtual int sendErrorCode(File_Key &fk, int32_t blockID, int32_t ret) = 0;
		virtual bool isExistSession(int32_t peerID) = 0;
#ifdef _TEST
		virtual int accept(File_Key &fk, int32_t mFromUserID) = 0;
#endif // _TEST
	};
}