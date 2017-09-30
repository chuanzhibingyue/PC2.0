#pragma once

#include "IOMgr.h"

namespace uc {

	void *createInstanceP2P(const ISink *sink);
	void destroyInstanceP2P(void *lp);

	// 文件传输控制接口
	class IP2PSink : public ISink
	{
	public:
		virtual void onUploadFile() = 0;
		virtual void onDownloadFile() = 0;
	};

	class IP2PMgr : public IMgr
	{
	public:
		virtual int32_t uploadFile(bool bToFS, std::wstring path, int32_t currSize, int32_t peerID, int32_t remoteIP, int16_t port) = 0;

		virtual int32_t downloadFile(bool bToFS, std::wstring path, int32_t fileSize, int32_t peerID, int32_t remoteIP, int16_t port) = 0;
	};

}