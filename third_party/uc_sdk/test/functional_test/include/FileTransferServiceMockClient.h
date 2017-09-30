#ifndef FILETRANSFER_SERVICE_MOCK_CLIENT_H_
#define FILETRANSFER_SERVICE_MOCK_CLIENT_H_

#include "IFileTransferService.h"
#include "UCMockBase.h"
using namespace uc;

class FileTransferServiceMockClient : public IFileTransferServiceEvent, public UCMockBase
{
public:
	void Init(IUCEngine *ucEngine);
	void UnInit();

	//interface
	void UploadFile(const std::string& filePath,int32_t& taskId,int32_t convertType = 0,int32_t convertWidth = 200,int32_t convertHeight = 200);
	void DownloadFile( const std::string& url, const std::string& savePath,int32_t& taskId,int32_t safeDown = 0,int32_t convertWidth = 200,int32_t convertHeight = 200 );
	//event

	virtual void OnFileUploading(int32_t errorCode, int32_t taskId, int32_t percent = 0);

	virtual void OnFileUploaded(
		int32_t errorCode, 
		int32_t taskId,
		const std::string& fileName,
		int64_t fileSize,
		int32_t operatorId,
		int64_t opreatorTime,
		const std::string& downloadURL, 
		const std::string& convertDownloadURL = ""
		);

	virtual void OnFileDownloading(int32_t errorCode, int32_t taskId, int32_t percent);

	virtual void OnFileDownloaded(int32_t errorCode, int32_t taskId, const std::string& savePath);

private:
	IFileTransferService* _fileTransferService;
};

#endif