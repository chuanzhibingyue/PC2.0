#include "includes.h"
#include "UCFileMgr.h"
#include "P2P_FileTask.h"

namespace uc {

	UCFileMgr *glpF = NULL;
	void *createInstanceFM(const ISink *sink){

		if (NULL == glpF)
		{
			glpF = new UCFileMgr();
			glpF->setMgrSink(sink);
		}

		return (void *)glpF;
	}

	void destroyInstanceFM(void *lp){

		if (NULL != glpF)
		{
			delete (UCFileMgr *)glpF;
			glpF = NULL;
		}
	}

	UCFileMgr::UCFileMgr()
	{
	}

	UCFileMgr::~UCFileMgr()
	{
	}

	int UCFileMgr::init()
	{
		LOGI("UCFileMgr::init()");
		getSink()->onNotify(ST_FILEMGR);
		return P2P_ERROR_SUCCESSED;
	}

	void UCFileMgr::destroy()
	{
		{
			AUTO_LOCK(al, lstReadTask.getLck());

			LSTFTASK::iterator it;  
			for( it = lstReadTask.begin(); it != lstReadTask.end(); ++it)  
			{  
				P2P_FileTask *Temp = *it;
				delete Temp;
				Temp = NULL;
			}
			lstReadTask.clear();
		}
		{
			AUTO_LOCK(al, lstWriteTask.getLck());

			LSTFTASK::iterator it;  
			for( it = lstWriteTask.begin(); it != lstWriteTask.end(); ++it)  
			{  
				P2P_FileTask *Temp = *it;
				delete Temp;
				Temp = NULL;
			}

			lstWriteTask.clear();
		}
	}

	int UCFileMgr::initFile(bool isUp, std::wstring path, int32_t fileSize, File_Key &fk)
	{
		// find filename;
		fk.setFileID(makeFileID());
		LOGI("PeerID %d <==> FileID %d", fk.getPeerID(), fk.getFileID());

		P2P_FileTask *lpFTask = new P2P_FileTask(fk);
		if (NULL == lpFTask)
		{
			return P2P_ERROR_2;
		}

		int ret = P2P_ERROR_1;
		if (isUp)
		{
			ret = lpFTask->initUploadTask(path, fileSize);
		}
		else
		{
			ret = lpFTask->initDownLoadTask(path, fileSize);
		}

		if (P2P_ERROR_SUCCESSED != ret)
		{
			delete lpFTask;
			return ret;
		}

		LSTFTASK *lstT = isUp ? &lstReadTask : &lstWriteTask;
		{
			AUTO_LOCK(al, lstWriteTask.getLck());
			lstT->push_back(lpFTask);
		}

		return ret;
	}

	int UCFileMgr::getTorrentInfo(bool isUp, File_Key &fk, int32_t &blockSize)
	{
		LSTFTASK *lstT = isUp ? &lstReadTask : &lstWriteTask;

		AUTO_LOCK(al, lstT->getLck());

		LSTFTASK::iterator it;  
		for( it = lstT->begin(); it != lstT->end(); ++it)  
		{  
			P2P_FileTask *Temp = *it;
			if (NULL != Temp && (Temp->getFKey()) == fk)
			{
				blockSize = Temp->getSize();
				return P2P_ERROR_SUCCESSED;
			}
		}

		return P2P_ERROR_NOT_EXIST;
	}

	bool UCFileMgr::isUpload(File_Key &fk)
	{
		AUTO_LOCK(al, lstReadTask.getLck());

		LSTFTASK::iterator it;  
		for( it = lstReadTask.begin(); it != lstReadTask.end(); ++it)  
		{  
			P2P_FileTask *Temp = *it;
			if (NULL != Temp && (Temp->getFKey()) == fk)
			{
				return true;
			}
		}

		return false;
	}

	int UCFileMgr::read(File_Key &fk, int offset, int &len, char* &buf)
	{
		AUTO_LOCK(al, lstReadTask.getLck());

		LSTFTASK::iterator it;  
		for( it = lstReadTask.begin(); it != lstReadTask.end(); ++it)  
		{  
			P2P_FileTask *Temp = *it;
			if (NULL != Temp && (Temp->getFKey()) == fk)
			{
				int readLen = Temp->read(offset, len, buf);
				if (readLen < 0)
				{
					len = 0;
					return P2P_ERROR_1;
				}

				LOGI("len %d, readLen %d", len, readLen);
				len = readLen;
				int progress = Temp->makeProgress();
				if (progress >= 0)
				{
					getSink()->onProgress(true, progress);
				}

				return P2P_ERROR_SUCCESSED;
			}
		}

		len = 0;
		return P2P_ERROR_2;
	}

	int UCFileMgr::write(File_Key &fk, int offset, int len, char *buf)
	{
		AUTO_LOCK(al, lstWriteTask.getLck());

		LSTFTASK::iterator it;  
		for (it = lstWriteTask.begin(); it != lstWriteTask.end(); ++it)  
		{  
			P2P_FileTask *Temp = *it;
			if (NULL != Temp && (Temp->getFKey()) == fk)
			{
				int writeLen = Temp->write(offset, len, (char *)buf);
				if (writeLen < 0)
				{
					return P2P_ERROR_1;
				}

				int progress = Temp->makeProgress();
				if (progress >= 0)
				{
					getSink()->onProgress(false, progress);
				}

				return P2P_ERROR_SUCCESSED;
			}
		}

		return P2P_ERROR_NOT_EXIST;
	}

	int UCFileMgr::currentSize(File_Key &fk, int &size)
	{
		return 0;
	}

	int32_t UCFileMgr::makeFileID()
	{
		static int32_t fileID = 5000;
		return fileID++;
	}

}