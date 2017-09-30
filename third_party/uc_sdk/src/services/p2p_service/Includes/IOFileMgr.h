#pragma once

#include "IOMgr.h"
#include "UCUtil.h"

namespace uc {

	void *createInstanceFM(const ISink *sink);
	void destroyInstanceFM(void *lp);

	class IFileSink : public ISink
	{
	public:
		virtual void onProgress(bool isUp, int progress) = 0; // 进度通知
	};

	class IFileMgr : public IMgr
	{
	public:
		virtual int initFile(bool isUp, std::wstring path, int32_t fileSize, File_Key &fk) = 0; // 初始化文件
		virtual int getTorrentInfo(bool isUp, File_Key &fk, int32_t &blockSize) = 0; // 上传者获取种子信息
		virtual bool isUpload(File_Key &fk) = 0;

		virtual int read(File_Key &fk, int32_t offset, int32_t &len, char* &buf) = 0; // 文件读
		virtual int write(File_Key &fk, int32_t offset, int32_t len, char *buf) = 0; // 文件写
		virtual int currentSize(File_Key &fk, int32_t &size) = 0;
	};

}