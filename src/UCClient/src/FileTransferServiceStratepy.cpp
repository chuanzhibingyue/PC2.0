#include "FileTransferServiceStrategy.h"
#include "UCDefs.h"

namespace ucclient {

FileTransferServiceStrategy::FileTransferServiceStrategy(IUCEngine *engine, cefIntegration::CefClient *cefClient) 
	: _engine(engine)
	, _cefClient(cefClient)
	, _jsEventAssist(cefClient) {
	_fsService = (IFileTransferService*)_engine->QueryService(UCFileTransferService);
}

FileTransferServiceStrategy::~FileTransferServiceStrategy() {
}

int32_t FileTransferServiceStrategy::UploadFile(const std::wstring &filePath, int32_t &taskId, int32_t convertType /*= 0*/, int32_t convertWidth /*= 200*/, int32_t convertHeight /*= 200*/) {
	std::string utf8FilePath = StringConvert::ToUTF8Str(filePath);
	StringConvert::Replace(utf8FilePath, "\\", "/");
	return _fsService->UploadFile(utf8FilePath, taskId, convertType, convertWidth, convertHeight);
}

int32_t FileTransferServiceStrategy::IsFileUploading(int32_t taskId, int32_t &isUploading) {
	return _fsService->IsFileUploading(taskId, isUploading);
}

int32_t FileTransferServiceStrategy::CancelUploadFile(int32_t taskId) {
	return _fsService->CancelUploadFile(taskId);
}

int32_t FileTransferServiceStrategy::DownloadFile(const std::wstring &downloadURL, const std::wstring &savePath, int32_t &taskId) {
	std::string utf8DownloadURL = StringConvert::ToUTF8Str(downloadURL);
	std::string utf8SavePath = StringConvert::ToUTF8Str(savePath);
	StringConvert::Replace(utf8SavePath, "\\", "/");
	return _fsService->DownloadFile(utf8DownloadURL, utf8SavePath, taskId);
}

int32_t FileTransferServiceStrategy::IsFileDownloading(int32_t taskId, int32_t isDownloading) {
	return _fsService->IsFileDownloading(taskId, isDownloading);
}

int32_t FileTransferServiceStrategy::CancelDownloadFile(int32_t taskId) {
	return _fsService->CancelDownloadFile(taskId);
}

int32_t FileTransferServiceStrategy::GetDownloadUrlFromFid(const std::wstring &fid, std::wstring &downloadUrl) {
	std::string utf8downloadUrl = "";
	int result = _fsService->GetDownloadUrlFromFid(StringConvert::ToUTF8Str(fid), utf8downloadUrl);
	downloadUrl = StringConvert::FromUTF8Str(utf8downloadUrl);
	return result;
}

int32_t FileTransferServiceStrategy::DeleteFileFromFS(const std::string &fid) {
	return _fsService->DeleteFileFromFS(fid);
}

int32_t FileTransferServiceStrategy::ResumeTask(int32_t taskId) {
	return _fsService->ResumeTask(taskId);
}

int32_t FileTransferServiceStrategy::GetLocalFilePath(const std::wstring &fileURL, std::wstring &filePath) {
	std::string utf8FileURL = StringConvert::ToUTF8Str(fileURL);
	std::string utf8FilePath = "";

	int result = _fsService->GetLocalFilePath(utf8FileURL, utf8FilePath);
	filePath = StringConvert::FromUTF8Str(utf8FilePath);

	return result;
}

int32_t FileTransferServiceStrategy::UpdateLocalFilePath(const std::wstring &fileURL, const std::wstring &filePath) {
	std::string utf8FileURL = StringConvert::ToUTF8Str(fileURL);
	std::string utf8FilePath = StringConvert::ToUTF8Str(filePath);

	return _fsService->UpdateLocalFilePath(utf8FileURL, utf8FilePath);
}

void FileTransferServiceStrategy::OnFileUploading(int32_t errorCode, int32_t taskId, int32_t percent) {
	std::string parameters = _jsEventAssist.MakeParametersForOnFileTransmission(errorCode, taskId, percent);
	_jsEventAssist.MakeJSEventAndExecute(FILETRANSFER_SERVICE_ONFILEUPLOADING_EVENT, parameters);
}

void FileTransferServiceStrategy::OnFileUploaded(int32_t errorCode, int32_t taskId,
	const std::string &fileName, int64_t fileSize, int32_t operatorId, int64_t operatorTime, const std::string& fid, 
	const std::string& downloadURL, const std::string& convertFid, const std::string& convertDownloadURL) {

	std::string parameters = _jsEventAssist.MakeParametersForOnFileUploaded(errorCode, taskId, fileName, fileSize,
		operatorId, operatorTime, fid, downloadURL, convertFid, convertDownloadURL);
	_jsEventAssist.MakeJSEventAndExecute(FILETRANSFER_SERVICE_ONFILEUPLOADED_EVENT, parameters);
}

void FileTransferServiceStrategy::OnFileDownloading(int32_t errorCode, int32_t taskId, int32_t percent) {
	std::string parameters = _jsEventAssist.MakeParametersForOnFileTransmission(errorCode, taskId, percent);
	_jsEventAssist.MakeJSEventAndExecute(FILETRANSFER_SERVICE_ONFILEDOWNLOADING_EVENT, parameters);
}

void FileTransferServiceStrategy::OnFileDownloaded(int32_t errorCode, int32_t taskId, const std::string &savePath) {
	std::string parameters = _jsEventAssist.MakeParametersForOnFileDownloaded(errorCode, taskId, savePath);
	_jsEventAssist.MakeJSEventAndExecute(FILETRANSFER_SERVICE_ONFILEDOWNLOADED_EVENT, parameters);
}

void FileTransferServiceStrategy::OnFileDeleted(int32_t errorCode, const std::string& fid) {
	std::string parameters = _jsEventAssist.MakeParametersForOnFileDeleted(errorCode, fid);
	_jsEventAssist.MakeJSEventAndExecute(FILETRANSFER_SERVICE_ONFILEDELETED_EVENT, parameters);
}

}