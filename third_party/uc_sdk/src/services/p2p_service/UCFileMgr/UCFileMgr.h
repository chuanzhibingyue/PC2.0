#pragma once

#include "IOFileMgr.h"
#include "P2P_FileTask.h"

namespace uc {

	class UCFileMgr : public IFileMgr
	{
	public:
		UCFileMgr();
		~UCFileMgr();

	public:
		// IFileMgr
		virtual int init();
		virtual void destroy();
		virtual int initFile(bool isUp, std::wstring path, int32_t fileSize, File_Key &fk); // 初始化文件
		virtual int getTorrentInfo(bool isUp, File_Key &fk, int32_t &blockSize); // 上传者获取种子信息
		virtual bool isUpload(File_Key &fk);
		virtual int read(File_Key &fk, int offset, int &len, char* &buf);
		virtual int write(File_Key &fk, int offset, int len, char *buf);
		virtual int currentSize(File_Key &fk, int &size); // int offset, int originPos

		enum SeekPosition { begin = 0x0, current = 0x1, end = 0x2 };

	protected:
		IFileSink *getSink() const { return (IFileSink *)mlpSink; }

	private:
		int32_t makeFileID();

	private:
		LSTFTASK lstReadTask;
		LSTFTASK lstWriteTask;
	};
}
