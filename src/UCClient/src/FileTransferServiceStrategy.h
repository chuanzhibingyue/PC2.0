#ifndef FILE_TRANSFER_SERVICE_STRATEGY_H_
#define FILE_TRANSFER_SERVICE_STRATEGY_H_

#include "IFileTransferService.h"
#include "CefClient.h"
#include "assist/FileTransferServiceJSEventAssist.h"

namespace ucclient {

using namespace uc;

class FileTransferServiceStrategy : public IFileTransferServiceEvent {
public:
	FileTransferServiceStrategy(IUCEngine *engine, cefIntegration::CefClient *cefClient);
	virtual ~FileTransferServiceStrategy();
public:
	int32_t UploadFile(const std::wstring &filePath, int32_t &taskId, int32_t convertType = 0, int32_t convertWidth = 200, int32_t convertHeight = 200);
	int32_t IsFileUploading(int32_t taskId, int32_t &isUploading);
	int32_t CancelUploadFile(int32_t taskId);
	int32_t DownloadFile(const std::wstring &downloadURL, const std::wstring &savePath, int32_t &taskId);
	int32_t IsFileDownloading(int32_t taskId, int32_t isDownloading);
	int32_t CancelDownloadFile(int32_t taskId);
	int32_t GetDownloadUrlFromFid(const std::wstring &fid, std::wstring &downloadUrl);
	int32_t DeleteFileFromFS(const std::string &fid);
	int32_t ResumeTask(int32_t taskId);
	int32_t GetLocalFilePath(const std::wstring &fileURL, std::wstring &filePath);
	int32_t UpdateLocalFilePath(const std::wstring &fileURL, const std::wstring &filePath);

	virtual void OnFileUploading(int32_t errorCode, int32_t taskId, int32_t percent);
	virtual void OnFileUploaded(int32_t errorCode, int32_t taskId, const std::string &fileName, 
		int64_t fileSize, int32_t operatorId, int64_t operatorTime, const std::string& fid,
		const std::string& downloadURL, const std::string& convertFid = "", const std::string& convertDownloadURL = "");

	virtual void OnFileDownloading(int32_t errorCode, int32_t taskId, int32_t percent);
	virtual void OnFileDownloaded(int32_t errorCode, int32_t taskId, const std::string &savePath);
	virtual void OnFileDeleted(int32_t errorCode, const std::string& fid);

private:
	cefIntegration::CefClient *_cefClient;
	IFileTransferService *_fsService;
	IUCEngine *_engine;
	FileTransferServiceJSEventAssist _jsEventAssist;
};

}

#endif /* FILE_TRANSFER_SERVICE_STRATEGY_H_ */
