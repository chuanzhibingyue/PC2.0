/*
 * FileTransferService.h
 *
 *  Created on: May 6, 2015
 *      Author: ethan
 */

#ifndef FILETRANSFERSERVICE_H_
#define FILETRANSFERSERVICE_H_

#include "IFileTransferService.h"
#include "Task.h"
#include "network/NetworkManager.h"
#include "FSRequestAssist.h"
#include "FSResponseAssist.h"
#include "ResidentThread.hpp"

namespace fs = boost::filesystem;
using boost::filesystem::path;

namespace uc {

class FileTransferService : public IFileTransferService, IUCEngineEvent, HTTPRequestEvent {
public:
	FileTransferService(IUCEngine *engine);
	virtual ~FileTransferService();
	
	virtual int32_t UploadFile(
		const std::string& filePath,
		OUT int32_t& taskId,
		int32_t convertType = 0,
		int32_t convertWidth = 200,
		int32_t convertHeight = 200
		);

	virtual int32_t IsFileUploading(int32_t taskId, OUT int32_t& isUploading);

	virtual int32_t CancelUploadFile(int32_t taskId);

	virtual int32_t DownloadFile(
		const std::string& downloadURL,
		const std::string& savePath,
		OUT int32_t& taskId,
		int32_t safeDown = 0,
		int32_t cvtWidth = 0,
		int32_t cvtHeight = 0
		);

	virtual int32_t IsFileDownloading(int32_t taskId, OUT int32_t& isDownloading);

	virtual int32_t CancelDownloadFile(int32_t taskId);

	virtual int32_t GetDownloadUrlFromFid(const std::string &fid, std::string &downloadUrl);

	virtual int32_t DeleteFileFromFS(const std::string &fid);

	virtual int32_t ResumeTask(int32_t taskId);

	virtual int32_t GetLocalFilePath(const std::string &fileURL, std::string &filePath);

	virtual int32_t UpdateLocalFilePath(const std::string &fileURL, const std::string &filePath);

	int32_t GetLocalFilePathAndSize(const std::string &fileURL, std::string &filePath, int64_t &fileSize);
	int32_t UpdateLocalFilePathAndSize(const std::string &fileURL, const std::string &filePath, int64_t fileSize);

	void OnHeaderReceived(int32_t requestId, HTTPRequest *request, int32_t statusCode, int32_t authType, const HeaderMap &header);

	/**
	 * 发送body数据时触发
	 * @param requestId
	 * @param request
	 * @param buffer 需要赋值的字符指针
	 * @param size 允许赋值的字符长度
	 * return 最终赋值的字符长度
	 */
	void OnDataSend(int32_t requestId, HTTPRequest *request, char *buffer, int32_t &size);

	/**
	 * 接收到body数据时触发
	 * @param request
	 * @param data
	 * @param length
	 */
	void OnDataReceived(int32_t requestId, HTTPRequest *request, void *data, int32_t length);

	/**
	 * http请求结束时触发
	 * @param request
	 */
	virtual void OnRequestFinished(int32_t requestId, HTTPRequest *request, int32_t errorCode);
	

	// IUCEngineEvent
	virtual void OnInit();
	virtual void OnUnInit();

protected:
	int32_t OnFileChecked(int32_t errorCode, HTTPResponse* response, int32_t taskId);
	int32_t OnFileDeleted(int32_t errorCode, HTTPResponse* response, const std::string fid);

	void RequestEnded(shared_ptr<Task> task, int32_t errorCode);

public:
	int64_t GetTime();
private:
	struct DownloadTask {
		std::string downloadURL;
		std::string savePath;
		int32_t taskIdFromApp;
		int32_t taskId;
		int32_t safeDown;
		int32_t cvtWidth;
		int32_t cvtHeight;
	};

	std::string _RetrieveServerDomain();
	int _RetrieveUserID();
	std::string _RetrieveSessionID();
	FSRequestAssist &_GetFSRequestAssist();
	FSResponseAssist &_GetFSResponseAssist();
	void _UploadFileInThread(shared_ptr<Task> task);
	void _CopyFileInThread(int32_t taskId, const std::string &from, 
		const std::string &to, int64_t size, const std::string &downloadUrl);
	int32_t _UploadFile(shared_ptr<Task> task);
	void _CancelTask(shared_ptr<Task> task);
	void _HandleDownloadUrlChanged(const HeaderMap &header, shared_ptr<Task> task);
	int32_t _HandleHeaderOnHeaderReceived(const HeaderMap &header, shared_ptr<Task> task);
	void _HandleUploadOnHeaderReceived(const HeaderMap &header, shared_ptr<Task> task);
	void _HandleDownloadOnHeaderReceived(const HeaderMap &header, shared_ptr<Task> task);
	bool _CheckFSUrl(const std::string &downloadUrl);	
	void _ChangeDownloadInfo(const std::string &fid, const HeaderMap &header, shared_ptr<Task> task);
	void _TriggerUploadFailedEvent(int32_t errorCode, int32_t taskId);
	void _TriggerUploadSuccessfulEvent(shared_ptr<Task> task);
	void _TriggerUploadEvent(int32_t errorCode, int32_t taskId, const std::string& fileName, int64_t fileSize,
		int32_t operatorId, int64_t opreatorTime, const std::string& fid, const std::string& downloadURL,
		const std::string& cvtFid, const std::string& convertDownloadURL);
	void _TriggerDownloadFailedEvent(int32_t errorCode, int32_t taskId);
	void _TriggerDownloadSuccessfulEvent(int32_t taskId, const std::string &path);
	void _TriggerDownloadEvent(int32_t errorCode, int32_t taskId, const std::string &path);
	void _HandleErrorTask(int32_t errorCode, int32_t taskId);
	int32_t ResumeTaskInternal(int32_t taskId);
	void _DownloadReal(const std::string& downloadUrl, int32_t cvtWidth, int32_t cvtHeight, shared_ptr<Task> task);
	int32_t _CheckFile(const std::string &filePath);
	int32_t _CheckFile(const std::string &filePath, int64_t fileSize);
	bool _NeedDeleteTask(int32_t errorCode);
	void _DownloadFile(DownloadTask downloadTask);

	Tasks _tasks;
	std::map<int32_t, std::list<int32_t>> downloadingTaks;
	NetworkManager _FSNetwork;
	const int64_t _size2G = (int64_t)(1024 * 1024 * 1024) * 2;
	ResidentThread<FileTransferService, DownloadTask> _residentThread;
};

} /* namespace uc */

#endif /* FILETRANSFERSERVICE_H_ */
