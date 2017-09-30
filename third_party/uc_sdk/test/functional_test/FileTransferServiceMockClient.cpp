#include "FileTransferServiceMockClient.h"

void FileTransferServiceMockClient::Init( IUCEngine *ucEngine ) {
	_fileTransferService = (IFileTransferService*)ucEngine->QueryService(UCFileTransferService);
	_fileTransferService->registerListener(this);
}

void FileTransferServiceMockClient::UnInit() {
	_fileTransferService->removeListener(this);
}

void FileTransferServiceMockClient::UploadFile( const std::string& filePath,int32_t& taskId,int32_t convertType /*= 0*/,int32_t convertWidth /*= 200*/,int32_t convertHeight /*= 200*/ ) {
	_fileTransferService->UploadFile(filePath, taskId, convertType, convertWidth, convertHeight);

}


void FileTransferServiceMockClient::DownloadFile( const std::string& url, const std::string& savePath,int32_t& taskId,int32_t safeDown /*= 0*/,int32_t convertWidth /*= 200*/,int32_t convertHeight /*= 200*/ ) {
	_fileTransferService->DownloadFile(url, savePath, taskId, safeDown,convertWidth, convertHeight);

}

void FileTransferServiceMockClient::OnFileUploading( int32_t errorCode, int32_t taskId, int32_t percent /*= 0*/ ){
	NotifyResult(errorCode);
}

void FileTransferServiceMockClient::OnFileUploaded( int32_t errorCode, int32_t taskId, const std::string& fileName, int64_t fileSize, int32_t operatorId, int64_t opreatorTime, const std::string& downloadURL, const std::string& convertDownloadURL /*= "" */ ) {
	NotifyResult(errorCode);
}

void FileTransferServiceMockClient::OnFileDownloading( int32_t errorCode, int32_t taskId, int32_t percent ) {

}

void FileTransferServiceMockClient::OnFileDownloaded( int32_t errorCode, int32_t taskId, const std::string& savePath ) {

}
