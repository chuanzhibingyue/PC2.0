/*
 * FileTransferService.cpp
 *
 *  Created on: May 6, 2015
 *      Author: ethan
 */

#include "FileTransferService.h"
#include "UCErrorCode.h"
#include "client_service/ClientService.h"
#include "CommonDef.h"
#include "UCEngine.h"
#include "network/NetworkManager.h"
#include "Macro.h"
#include "UCTime.h"
#include "md5/md5.h"
#include "DBManager.h"
#include "FSDef.h"
#include <thread>
#include "boost/regex.hpp"
#include "FileHelp.hpp"

namespace uc {

FileTransferService::FileTransferService(IUCEngine *engine) 
	: _FSNetwork(FS_REQUEST_THREADS, FS_REQUEST_PERFORM_NUMBER)
	, _residentThread(this, &FileTransferService::_DownloadFile) {
	_engine = engine;
	_engine->registerListener(this);
}

FileTransferService::~FileTransferService() {
	_tasks.Clear();
	downloadingTaks.clear();
	_engine->removeListener(this);
}

int32_t FileTransferService::UploadFile( const std::string& filePath, OUT int32_t& taskId,
	int32_t convertType /*= 0*/, int32_t convertWidth /*= 200*/, int32_t convertHeight /*= 200 */ ) {
	UC_LOG(INFO) << "FileTransferService::UploadFile, filePath: " << filePath << ", convertType: " << convertType
		<< ", convertWidth: " << convertWidth << ", convertHeight: " << convertHeight;
	if (filePath.empty()) {
		UC_LOG(ERROR) << "FileTransferService::UploadFile error. input parameter error. file path is empty.";
		return FT_ERROR_PARAMETER;
	}
	
	{
		int32_t result = _CheckFile(filePath);
		if (result != UC_SUCCEEDED) {
			return result;
		}
	}

	if (taskId != 0) {
		//重启失败的任务
		shared_ptr<Task> task = _tasks.GetTask(taskId);
		if (task == NULL) {
			return FT_ERROR_TASK_NOT_EXIST;
		}
		task->_isFailed = false;
		_UploadFile(task);
	} else {
		//新建上传任务
		shared_ptr<Task> task = make_shared<Task>(Task::MakeTaskId(), filePath, "", 0, convertType);
		taskId = task->_taskId;
		std::thread td(&FileTransferService::_UploadFileInThread, this, task);
		td.detach();
	}

	return UC_SUCCEEDED;
}

//hdchk请求回调处理。
int32_t FileTransferService::OnFileChecked( int32_t errorCode, HTTPResponse* response, int32_t taskId ) {
	UC_LOG(INFO) << "FileTransferService::OnFileChecked, errorCode: " << errorCode << ", taskId: " << taskId;

	shared_ptr<Task> task = _tasks.GetTask(taskId);
	if (task == NULL) {
		UC_LOG(ERROR) << "FileTransferService::OnFileChecked error, task is null.";
		_TriggerUploadFailedEvent(FT_ERROR_TASK_NOT_EXIST, taskId);
		return 0;
	}

	if (task->_isCanceled) {
		UC_LOG(INFO) << "FileTransferService::OnFileChecked, task is canceled, path is : " << task->_ucFile._path;
		_TriggerUploadFailedEvent(FT_ERROR_TASK_CANCELED, task->_taskId);
		return 0;
	}

	int32_t result = _GetFSResponseAssist().GuardStatementForCallBackCode(errorCode, response->GetStatusCode());
	if (UC_SUCCEEDED != result)
	{
		_TriggerUploadFailedEvent(result, task->_taskId);
		return 0;
	}

	const HeaderMap header = response->GetHeaderInfo();
	result = _GetFSResponseAssist().GuardStatementForHeader(header);
	if (UC_SUCCEEDED != result)
	{
		_TriggerUploadFailedEvent(result, task->_taskId);
		return 0;
	}

	std::string fid = _GetFSResponseAssist().RetrieveValueFromKey(header, TANG_FILE_ID);
	if (fid.empty())
	{
		//需要上传
		UC_LOG(INFO) << "FileTransferService::OnFileChecked file need upload. file is : " << task->_ucFile._path;
		int64_t offset = _GetFSResponseAssist().RetrieveOffset(header);
		if (offset < 0 || offset > task->_ucFile._size) {
			UC_LOG(ERROR) << "FileTransferService::OnFileChecked error, response X-Tang-File-Offset = " << offset;
			_TriggerUploadFailedEvent(FT_ERROR_RESPONSE_XTANGFILEOFFSET, task->_taskId);
			return 0;
		}

		task->_offset = offset;
		task->_bodyHeader = _GetFSRequestAssist().MakeBodyHeader(task->_ucFile, task->_offset);
		const std::string cookie = _GetFSResponseAssist().RetrieveValueFromKey(header, TANG_COOKIE);
		int64_t needSendSize = _GetFSRequestAssist().CalculateNeedSendSize(task->_ucFile._size, task->_offset, task->_bodyHeader.length());
		HTTPRequest request = _GetFSRequestAssist().MakeRequestOfUploadFile(cookie, needSendSize);
		_FSNetwork.SendAsyncRequest(task->_taskId, request, this, HTTP_TIMEOUT, HTTP_RESPONSE_INFINITY);
	}
	else {
		//文件已经上传过了
		UC_LOG(INFO) << "FileTransferService::OnFileChecked file already uploaded. X-Tang-File-Id = " << fid.c_str();
		_ChangeDownloadInfo(fid, header, task);
		_TriggerUploadSuccessfulEvent(task);
		return 0;
	}

	return 0;
}

int32_t FileTransferService::OnFileDeleted(int32_t errorCode, HTTPResponse* response, const std::string fid) {
	UC_LOG(INFO) << "FileTransferService::OnFileDeleted, errorCode is : " << errorCode << ", fid is : " << fid;
	int32_t result = _GetFSResponseAssist().GuardStatementForCallBackCode(errorCode, response->GetStatusCode());
	if (UC_SUCCEEDED != result
		&& UC_SERVER_ERROR != result)
	{
		triggerEvent(IFileTransferServiceEvent, OnFileDeleted(result, fid));
		return -1;
	}

	result = _GetFSResponseAssist().GuardStatementForHeader(response->GetHeaderInfo());
	triggerEvent(IFileTransferServiceEvent, OnFileDeleted(result, fid));
	return 0;
}

int32_t FileTransferService::DownloadFile( const std::string& downloadURL, const std::string& savePath,
 OUT int32_t& taskId, int32_t safeDown /*= 0*/, int32_t cvtWidth /*= 0*/, int32_t cvtHeight /*= 0 */ ){
	if (downloadURL.empty() || savePath.empty()) {
		UC_LOG(ERROR) << "FileTransferService::DownloadFile failed, input parameter error!";
		return FT_ERROR_PARAMETER;
	}
	UC_LOG(INFO) << "FileTransferService::DownloadFile downloadURL: [" << downloadURL << "], savePath:[" << savePath << "].";
	DownloadTask task;
	task.taskIdFromApp = taskId;
	if (taskId == 0)
	{
		taskId = Task::MakeTaskId();
	}
	task.taskId = taskId;
	task.downloadURL = downloadURL;
	task.savePath = savePath;
	task.safeDown = safeDown;
	task.cvtWidth = cvtWidth;
	task.cvtHeight = cvtHeight;
	_residentThread.PushTask(task);

	return UC_SUCCEEDED;
}


/***************************************************************
* 实现 HTTPRequestEvent 虚函数
***************************************************************/
void FileTransferService::OnHeaderReceived(int32_t requestId, HTTPRequest *request, int32_t statusCode, int32_t authType, const HeaderMap &header) {
	UC_LOG(INFO) << "FileTransferService::OnHeaderReceived enter: taskId = " << requestId << ", statusCode = " << statusCode;

	if (statusCode == 407) {
		return;
	}

	shared_ptr<Task> task = _tasks.GetTask(requestId);
	if (task == NULL) {
		UC_LOG(ERROR) << "FileTransferService::OnHeaderReceived error, task is not exist.";
		_TriggerDownloadFailedEvent(FT_ERROR_TASK_NOT_EXIST, requestId);
		return;
	}

	if (statusCode != 200) {
		UC_LOG(ERROR) << "FileTransferService::OnHeaderReceived error. task : " << requestId << ", status : " << statusCode;
		RequestEnded(task, statusCode);
		return;
	}

	if (task->_isCanceled) {
		UC_LOG(ERROR) << "FileTransferService::OnHeaderReceived error , the task is canceled. taskId : " << requestId;
		//取消当前http链接
		RequestEnded(task, FT_ERROR_TASK_CANCELED);
		return;
	}

	if (UC_SUCCEEDED != _HandleHeaderOnHeaderReceived(header, task))
	{
		return;
	}

	if (task->_mode == 0) {
		_HandleUploadOnHeaderReceived(header, task);
	} else {
		_HandleDownloadOnHeaderReceived(header, task);
	}

}

//文件下载数据接收
void FileTransferService::OnDataReceived(int32_t requestId, HTTPRequest *request, void *data, int32_t length) {
	UC_LOG(INFO) << "FileTransferService::OnDataReceived: " << requestId << " length: " << length;
	shared_ptr<Task> task = _tasks.GetTask(requestId);
	if (task == NULL) {
		UC_LOG(ERROR) << "FileTransferService::OnDataReceived error, task is not exist. taskId : " << requestId;
		_TriggerDownloadFailedEvent(FT_ERROR_TASK_NOT_EXIST, requestId);
		return;
	}

	if (task->_isFailed) {
		UC_LOG(ERROR) << "FileTransferService::OnDataReceived error, task failed. task : " << task->RetrieveDescription();
		return;
	}

	if (task->_mode == 0) {
		UC_LOG(ERROR) << "FileTransferService::OnDataReceived error, task mode is upload. task : " << task->RetrieveDescription();
		return;
	}

	if (length <= 0 || data == NULL) {
		// 表示调用失败,response不是有效的数据
		UC_LOG(ERROR) << "FileTransferService::OnDataReceived error, response data is null. task : " << task->RetrieveDescription();
		_TriggerDownloadFailedEvent(FT_ERROR_RESPONSE_DATA, task->_taskId);
		return;
	}

	if (task->_isCanceled) {
		UC_LOG(ERROR) << "FileTransferService::OnDataReceived error , the task is canceled. taskId : " << task->RetrieveDescription();
		_TriggerDownloadFailedEvent(FT_ERROR_TASK_CANCELED, task->_taskId);
		return;
	}

	auto tp = task->HandleOnDataReceived(data, length);
	if (std::get<0>(tp))
	{
		triggerEvent(IFileTransferServiceEvent, OnFileDownloading(0, requestId, std::get<1>(tp)));
	}
}

void FileTransferService::OnRequestFinished(int32_t requestId, HTTPRequest *request, int32_t errorCode) {
	UC_LOG(INFO) << "FileTransferService::OnRequestFinished: " << requestId << "; errorCode:" << errorCode;
	shared_ptr<Task> task = _tasks.GetTask(requestId);
	if (task == NULL) {
		//todo:在OnHeaderReceived返回失败后，又会从这个地方返回成功
		UC_LOG(ERROR) << "FileTransferService::OnRequestFinished error, task is not exist.";
		return;
	}
	if (task->_isFailed) {
		UC_LOG(ERROR) << "FileTransferService::OnRequestFinished error, task is failed, so ignore.";
		return;
	}

	if (task->_isCanceled){ // 修正bug:0002738
		UC_LOG(ERROR) << "FileTransferService::OnRequestFinished error, the task is canceled. taskId : " << requestId;
		errorCode = FT_ERROR_TASK_CANCELED;
	}

	RequestEnded(task, errorCode);
}

void FileTransferService::OnDataSend( int32_t requestId, HTTPRequest *request, char *buffer, int32_t &size ) {
	UC_LOG(INFO) << "FileTransferService::OnDataSend, requestId : " << requestId << ", size : " << size;
	const int32_t sizeIn = size;
	size = 0;
	UC_LOG(INFO) << "FileTransferService::OnDataSend: " << requestId;
	shared_ptr<Task> task = _tasks.GetTask(requestId);
	if (task == NULL) {
		UC_LOG(ERROR) << "FileTransferService::OnDataSend error, task is not exist. taskId : " << requestId;
		_TriggerUploadFailedEvent(FT_ERROR_TASK_NOT_EXIST, requestId);
		return;
	}

	if (buffer == NULL
		|| sizeIn == 0
		|| task->RetrieveRemainLen() < 0) {
		UC_LOG(ERROR) << "FileTransferService::OnDataSend error, send info is not right, task : " << task->RetrieveDescription()
			<< ", size : " << sizeIn << ", RemainLen : " << task->RetrieveRemainLen();
		_TriggerUploadFailedEvent(FT_ERROR_DATA_SEND, requestId);
		return;
	}

	//任务被取消
	if (task->_isCanceled) {
		UC_LOG(ERROR) << "FileTransferService::OnDataSend error , The task has been canceled. taskId : "
			<< task->RetrieveDescription();
		_TriggerUploadFailedEvent(FT_ERROR_TASK_CANCELED, requestId);
		return;
	}

	auto tp = task->HandleOnDataSended(buffer, sizeIn);
	if (std::get<0>(tp))
	{
		triggerEvent(IFileTransferServiceEvent, OnFileUploading(0, requestId, std::get<1>(tp)));
	}
	size = std::get<2>(tp);

	UC_LOG(INFO) << "FileTransferService::OnDataSend. task : " << task->RetrieveDescription() << ", size: " << size;
}

int32_t FileTransferService::IsFileUploading( int32_t taskId, OUT int32_t& isUploading ) {
	UC_LOG(INFO) << "FileTransferService::IsFileUploading, taskId : " << taskId;
	isUploading = (bool)_tasks.DoesTaskExist(taskId);
	return UC_SUCCEEDED;
}

int32_t FileTransferService::IsFileDownloading( int32_t taskId, OUT int32_t& isDownloading ) {
	UC_LOG(INFO) << "FileTransferService::IsFileDownloading, taskId : " << taskId;
	isDownloading = (bool)_tasks.DoesTaskExist(taskId);
	return UC_SUCCEEDED;
}

int32_t FileTransferService::CancelUploadFile( int32_t taskId ) {
	UC_LOG(INFO) << "FileTransferService::CancelUploadFile, taskId : " << taskId;
	shared_ptr<Task> task = _tasks.GetTask(taskId);
	if (task == NULL) {
		UC_LOG(ERROR) << "FileTransferService::CancelUploadFile error, task is not exist. taskId : " << taskId;
		return FT_ERROR_TASK_NOT_EXIST;
	}

	_CancelTask(task);
	return UC_SUCCEEDED;
}

int32_t FileTransferService::CancelDownloadFile( int32_t taskId ) {
	UC_LOG(INFO) << "FileTransferService::CancelDownloadFile. taskId : " << taskId;
	shared_ptr<Task> task = _tasks.GetTask(taskId);
	if (task == NULL) {
		UC_LOG(ERROR) << "FileTransferService::CancelDownloadFile error, task is not exist. taskId : " << taskId;
		return FT_ERROR_TASK_NOT_EXIST;
	}

	_CancelTask(task);
	return UC_SUCCEEDED;
}

int32_t FileTransferService::GetDownloadUrlFromFid(const std::string &fid, std::string &downloadUrl) {
	downloadUrl = _GetFSResponseAssist().RetrieveDownloadURL(fid);
	UC_LOG(INFO) << "FileTransferService::GetDownloadUrlFromFid. fid : " << fid << ", downloadUrl : " << downloadUrl;
	return UC_SUCCEEDED;
}

int32_t FileTransferService::DeleteFileFromFS(const std::string &fid) {
	UC_LOG(INFO) << "FileTransferService::DeleteFile, fid = : " << fid;
	if (fid.empty())
	{
		UC_LOG(ERROR) << "FileTransferService::DeleteFile error, fid is empty.";
		return FT_ERROR_PARAMETER;
	}

	std::string realFid = fid;
	if (_CheckFSUrl(fid)) {
		realFid = _GetFSRequestAssist().RetrieveFidFromDownloadUrl(fid);
	}

	HTTPRequest request = _GetFSRequestAssist().MakeRequestOfDeleteFile(realFid);
	_FSNetwork.SendAsyncRequest(0, request, MakeRequestCallback(this, &FileTransferService::OnFileDeleted, fid), HTTP_TIMEOUT, HTTP_RESPONSE_INFINITY);
	return UC_SUCCEEDED;

}

int32_t FileTransferService::ResumeTask(int32_t taskId) {
	shared_ptr<Task> task = _tasks.GetTask(taskId);
	if (task == NULL) {
		return FT_ERROR_FILE_NOT_EXIST;
	}

	if (task->_mode == 0) {
		return UploadFile(task->_ucFile._path, taskId);
	} else {
		return DownloadFile(task->_downloadURL, task->_ucFile._path, taskId);
	}
}

//-----------------------------------util----------------------------------------------//

std::string FileTransferService::_RetrieveServerDomain() {
	ClientService* clientService = (ClientService*)_engine->QueryService(UCClientService);
	ServerInfo fsServer = clientService->GetLoginInfo().GetServerInfo(FS_SERVER);
	return fsServer.GetDomain();
}

int FileTransferService::_RetrieveUserID() {
	ClientService* clientService = (ClientService*)_engine->QueryService(UCClientService);
	return clientService->GetLoginInfo().GetUserId();
}

std::string FileTransferService::_RetrieveSessionID() {
	ClientService* clientService = (ClientService*)_engine->QueryService(UCClientService);
	return clientService->GetLoginInfo().GetSessionId();
}

FSRequestAssist &FileTransferService::_GetFSRequestAssist() {
	static FSRequestAssist _fsRequestAssist(_RetrieveServerDomain(), _RetrieveUserID(), _RetrieveSessionID());
	return _fsRequestAssist;
}

FSResponseAssist &FileTransferService::_GetFSResponseAssist() {
	static FSResponseAssist _fsResponseAssist(_RetrieveServerDomain(), _RetrieveUserID());
	return _fsResponseAssist;
}

void FileTransferService::_CancelTask(shared_ptr<Task> task) {
	task->_isCanceled = true;
	task->_ucFile.Close();
	int ret = _FSNetwork.CancelAysncRequest(task->_taskId);

	if (UC_SUCCEEDED == ret) {
		_tasks.DeleteTask(task->_taskId);
	}
}

void FileTransferService::_HandleDownloadUrlChanged(const HeaderMap &header, shared_ptr<Task> task) {
	UC_LOG(INFO) << "OnHeaderReceived error, use new download url, " << "taskId = " << task->_taskId;
	std::string newURL = _GetFSResponseAssist().RetrieveValueFromKey(header, TANG_NEW_URL);
	if (newURL.empty()) {
		UC_LOG(ERROR) << "OnHeaderReceived error.get X-Tang-New-URL error. " << "taskId= " << task->_taskId;
		RequestEnded(task, FT_ERROR_RESPONSE_GETXTANGNEWURL);
		return;
	}

	task->_downloadURL = newURL;
	
	//发起新的http请求
	HTTPRequest request = _GetFSRequestAssist().MakeRequestOfDownloadFile(newURL);
	_FSNetwork.SendAsyncRequest(task->_taskId, request, this, HTTP_TIMEOUT, HTTP_RESPONSE_INFINITY);
}

int32_t FileTransferService::_HandleHeaderOnHeaderReceived(const HeaderMap &header, shared_ptr<Task> task) {
	int32_t result = _GetFSResponseAssist().GuardStatementForHeader(header);
	if (UC_SUCCEEDED != result)
	{
		if (FT_ERROR_DOWNLOAD_URL_CHANGED == result) {
			_HandleDownloadUrlChanged(header, task);
		}
		else {
			RequestEnded(task, result);
		}
	}
	return result;
}

void FileTransferService::_HandleUploadOnHeaderReceived(const HeaderMap &header, shared_ptr<Task> task) {
	UC_LOG(INFO) << "FileTransferService::_HandleUploadOnHeaderReceived  upload task, task : " << task->RetrieveDescription();
	std::string fid = _GetFSResponseAssist().RetrieveValueFromKey(header, TANG_FILE_ID);
	if (fid.empty())
	{
		UC_LOG(ERROR) << "FileTransferService::_HandleUploadOnHeaderReceived fid is not exist";
		RequestEnded(task, FT_ERROR_RESPONSE_GETFILEID);
	} else {
		_ChangeDownloadInfo(fid, header, task);
	}
}

void FileTransferService::_HandleDownloadOnHeaderReceived(const HeaderMap &header, shared_ptr<Task> task) {
	int64_t len = 0;
	int32_t result = _GetFSResponseAssist().RetrieveFileLength(header, len);
	if (result != UC_SUCCEEDED) {
		RequestEnded(task, result);
		return;
	}
	task->_ucFile._size = len;

	result = _GetFSResponseAssist().RetrieveContentLength(header, task->_ucFile._size, task->_offset);
	if (result != UC_SUCCEEDED) {
		RequestEnded(task, result);
	}
}

bool FileTransferService::_CheckFSUrl(const std::string &downloadUrl) {
	//http://oncloud.quanshi.com/ucfserver/hddown?fid=MTIyODYzNC84L25naW546YWN572u5LiO5LyY5YyW5oyH5Y2XLnBkZl5eXnRhbmdoZGZzXl5eNjIyMWQ1Njg4NzAzOWQwMzE5NWE0ZDM2ZThmMWRkYmJeXl50YW5naGRmc15eXjE1OTg5MDA2$&u=1228634&off=0
	//匹配*hddown*fid=*或*hdsafedown*fid=*
	std::string key = "(.+)hd(safe|)down(.+)fid=(.+)";
	boost::regex reg(key);
	return boost::regex_match(downloadUrl, reg);
}

void FileTransferService::_ChangeDownloadInfo(const std::string &fid, const HeaderMap &header, shared_ptr<Task> task) {
	task->_fid = fid;
	task->_downloadURL = _GetFSResponseAssist().RetrieveDownloadURL(fid);
	task->_cvtFid = _GetFSResponseAssist().RetrieveConvertedFid(header, task->_cvtType);
	task->_cvtDownloadURL = _GetFSResponseAssist().RetrieveDownloadURL(task->_cvtFid);
}

void FileTransferService::_HandleErrorTask(int32_t errorCode, int32_t taskId) {
	if (_NeedDeleteTask(errorCode)) {
		bool removeCacheFile = false;
		if (errorCode == UC_URL_NOT_EXIST
			|| errorCode == FT_ERROR_FILE_DELETED) {
			removeCacheFile = true;
		}
		_tasks.DeleteTask(taskId, removeCacheFile);
	}
	else {
		shared_ptr<Task> task = _tasks.GetTask(taskId);
		if (task != NULL) {
			task->_isFailed = true;
		}
	}
}

int32_t FileTransferService::ResumeTaskInternal(int32_t taskId) {
	shared_ptr<Task> task = _tasks.GetTask(taskId);
	if (task == NULL) {
		return FT_ERROR_FILE_NOT_EXIST;
	}
	task->_isFailed = false;
	return UC_SUCCEEDED;
}

void FileTransferService::_DownloadReal(const std::string& downloadUrl, int32_t cvtWidth, int32_t cvtHeight, shared_ptr<Task> task) {
	const std::string url = _GetFSRequestAssist().MakeURLOfDownloadFile(downloadUrl, cvtWidth, cvtHeight, task->_offset);
	HTTPRequest request = _GetFSRequestAssist().MakeRequestOfDownloadFile(url);
	_FSNetwork.SendAsyncRequest(task->_taskId, request, this, HTTP_TIMEOUT, HTTP_RESPONSE_INFINITY);
}

int32_t FileTransferService::_CheckFile(const std::string &filePath) {
	try
	{
		if (!filehelp::exists(filePath)) {
			UC_LOG(ERROR) << "FileTransferService::_CheckFile error. file : '" << filePath << "' is not exist.";
			return FT_ERROR_FILE_NOT_EXIST;
		}

		int64_t size = filehelp::file_size(filePath);
		if (size > _size2G) {
			return FT_ERROR_FILE_TOO_BIG;
		}
	}
	catch (fs::filesystem_error e) {
		boost::system::error_code code = e.code();
		UC_LOG(ERROR) << "FileTransferService::_CheckFile， encounter an exception when file operator , exception: error code: "
			<< code.value() << ", error message:" << e.what();
		return FT_ERROR_FILE_CAN_NOT_OPERATE;
	}

	return UC_SUCCEEDED;
}

int32_t FileTransferService::_CheckFile(const std::string &filePath, int64_t fileSize) {
	if (fileSize == 0) {
		return FT_ERROR_FILE_SIZE;
	}

	try
	{
		if (!filehelp::exists(filePath)) {
			UC_LOG(INFO) << "FileTransferService::_CheckFile error. file : '" << filePath << "' is not exist.";
			return FT_ERROR_FILE_NOT_EXIST;
		}

		int64_t size = filehelp::file_size(filePath);
		if (size != fileSize) {
			return FT_ERROR_FILE_SIZE;
		}
	}
	catch (fs::filesystem_error e) {
		boost::system::error_code code = e.code();
		UC_LOG(ERROR) << "FileTransferService::_CheckFile， encounter an exception when file operator , exception: error code: "
			<< code.value() << ", error message:" << e.what();
		return FT_ERROR_FILE_CAN_NOT_OPERATE;
	}

	return UC_SUCCEEDED;
}

bool FileTransferService::_NeedDeleteTask(int32_t errorCode) {
	if (errorCode == UC_SUCCEEDED
		|| errorCode == FT_ERROR_TASK_CANCELED
		|| errorCode == UC_URL_NOT_EXIST
		|| errorCode == FT_ERROR_FILE_DELETED
		|| errorCode == FT_ERROR_FILE_RENAME) {
		return true;
	} else {
		return false;
	}
}

void FileTransferService::_DownloadFile(DownloadTask downloadTask) {
	if (!_CheckFSUrl(downloadTask.downloadURL))
	{
		UC_LOG(ERROR) << "FileTransferService::DownloadFile download Url is not from FS, url : " << downloadTask.downloadURL;
		_TriggerDownloadFailedEvent(FT_ERROR_DOWNLOAD_URL, downloadTask.taskId);
		return ;
	}

	const std::string noUserDownloadUrl = _GetFSRequestAssist().RetrieveNoUserDownloadURL(downloadTask.downloadURL);
	if (_tasks.IsFileDownloading(noUserDownloadUrl, downloadTask.taskId)) {
		UC_LOG(ERROR) << "FileTransferService::DownloadFile download Url is downloading, url : " << noUserDownloadUrl
			<< "path : " << downloadTask.savePath;
		if (downloadTask.taskId == 0) {
			downloadTask.taskId = Task::MakeTaskId();
		}
		int32_t oldTask = _tasks.RetrieveTaskIdFromDownloadUrl(noUserDownloadUrl);

		downloadingTaks[oldTask].push_back(downloadTask.taskId);
		return ;
	}

	if (downloadTask.taskIdFromApp != 0) {
		//重启失败的任务
		//重启失败的任务
		shared_ptr<Task> task = _tasks.GetTask(downloadTask.taskIdFromApp);
		if (task == NULL) {
			_TriggerDownloadFailedEvent(FT_ERROR_FILE_NOT_EXIST, downloadTask.taskIdFromApp);
			return ;
		}
		task->_isFailed = false;
		_DownloadReal(noUserDownloadUrl, downloadTask.cvtWidth, downloadTask.cvtHeight, task);
	}
	else {
		//检测是否已经下载过
		std::string pathFromDB{ "" };
		int64_t fileSize{ 0 };
		//生成一个任务Id返回给应用
		if (UC_SUCCEEDED == GetLocalFilePathAndSize(noUserDownloadUrl, pathFromDB, fileSize)
			&& UC_SUCCEEDED == _CheckFile(pathFromDB, fileSize)) {
			std::thread td(&FileTransferService::_CopyFileInThread, this,
				downloadTask.taskId, pathFromDB, downloadTask.savePath, fileSize, noUserDownloadUrl);
			td.detach();
		}
		else {
			shared_ptr<Task> task = make_shared<Task>(downloadTask.taskId, downloadTask.savePath, noUserDownloadUrl, 1);
			downloadTask.taskId = task->_taskId;
			int32_t result = task->Init();
			if (UC_SUCCEEDED != result) {
				UC_LOG(ERROR) << "FileTransferService::DownloadFile error. Init task failed. error code = " << result;
				_TriggerDownloadFailedEvent(result, downloadTask.taskId);
				return ;
			}
			_tasks.AddTask(task);
			_DownloadReal(noUserDownloadUrl, downloadTask.cvtWidth, downloadTask.cvtHeight, task);
		}

	}
}

void FileTransferService::_TriggerUploadFailedEvent(int32_t errorCode, int32_t taskId) {
	UC_LOG(ERROR) << "FileTransferService::_TriggerUploadFailedEvent, errorCode : " << errorCode << ", taskId : " << taskId;
	_TriggerUploadEvent(errorCode, taskId, "", 0, 0, 0, "", "", "", "");
}

void FileTransferService::_TriggerUploadSuccessfulEvent(shared_ptr<Task> task) {
	UC_LOG(INFO) << "FileTransferService::_TriggerUploadSuccessfulEvent, task : " << task->RetrieveDescription();
	_TriggerUploadEvent(UC_SUCCEEDED,
		task->_taskId, task->_ucFile._name,
		task->_ucFile._size, _RetrieveUserID(), GetTime(),
		task->_fid, task->_downloadURL, task->_cvtFid, task->_cvtDownloadURL);
}

void FileTransferService::_TriggerUploadEvent(int32_t errorCode, int32_t taskId, const std::string& fileName,
	int64_t fileSize, int32_t operatorId, int64_t opreatorTime, const std::string& fid, const std::string& downloadURL,
	const std::string& cvtFid, const std::string& convertDownloadURL) {
	_HandleErrorTask(errorCode, taskId);
	triggerEvent(IFileTransferServiceEvent, OnFileUploaded(errorCode,
		taskId, fileName, fileSize, operatorId, opreatorTime,
		fid, downloadURL, cvtFid, convertDownloadURL));
}

void FileTransferService::_TriggerDownloadFailedEvent(int32_t errorCode, int32_t taskId) {
	UC_LOG(ERROR) << "FileTransferService::_TriggerDownloadFailedEvent, errorCode : " << errorCode << ", taskId : " << taskId;
	_TriggerDownloadEvent(errorCode, taskId, "");
}

void FileTransferService::_TriggerDownloadSuccessfulEvent(int32_t taskId, const std::string &path) {
	_TriggerDownloadEvent(UC_SUCCEEDED, taskId, path);
}

void FileTransferService::_TriggerDownloadEvent(int32_t errorCode, int32_t taskId, const std::string &path) {
	triggerEvent(IFileTransferServiceEvent, OnFileDownloaded(errorCode, taskId, path));
	if (_NeedDeleteTask(errorCode)) {
		auto iter = downloadingTaks.find(taskId);
		if (iter != downloadingTaks.end()) {
            auto vec = iter->second;
            for (auto &var : vec) {
                //同一下载url的其他任务
                triggerEvent(IFileTransferServiceEvent, OnFileDownloaded(errorCode, var, path));
            }
            downloadingTaks.erase(taskId);
		}
	}
	_HandleErrorTask(errorCode, taskId);
}

int64_t FileTransferService::GetTime() {
	return uc::Time::GetSysTime();
}

int32_t FileTransferService::GetLocalFilePath(const std::string &fileURL, std::string &filePath) {
	return DBManager::GetInstance().GetFSLocalPath(fileURL, filePath);
}

int32_t FileTransferService::UpdateLocalFilePath(const std::string &fileURL, const std::string &filePath) {
	return DBManager::GetInstance().UpdateFSLocalPath(fileURL, filePath);
}

int32_t FileTransferService::GetLocalFilePathAndSize(const std::string &fileURL, std::string &filePath, int64_t &fileSize) {
	return DBManager::GetInstance().GetFSLocalPathAndSize(fileURL, filePath, fileSize);
}

int32_t FileTransferService::UpdateLocalFilePathAndSize(const std::string &fileURL, const std::string &filePath, int64_t fileSize) {
	return DBManager::GetInstance().UpdateFSLocalPathAndSize(fileURL, filePath, fileSize);
}

void FileTransferService::OnInit() {
	if (!_FSNetwork.Init(_engine) || !_FSNetwork.Start()) {
		UC_LOG(ERROR) << "fs network start error";
	}
	_residentThread.Start();
}

void FileTransferService::OnUnInit() {
	_FSNetwork.Stop();
	_residentThread.Stop();
}

void FileTransferService::RequestEnded(shared_ptr<Task> task, int32_t errorCode) {
	if (task->_mode == 0) {
		//上传完成
		if (errorCode != UC_SUCCEEDED) {
			_TriggerUploadFailedEvent(errorCode, task->_taskId);
		}
		else if ((task->_downloadURL.empty()) || (task->_fid.empty())) {
			_TriggerUploadFailedEvent(FT_ERROR_RESPONSE_DATA, task->_taskId);
		} 
		else {
			// 需求：文件上传时，需将URL和本地路径、大小存入数据库
			const std::string noUserDownloadUrl = _GetFSRequestAssist().RetrieveNoUserDownloadURL(task->_downloadURL);
			UpdateLocalFilePathAndSize(noUserDownloadUrl, task->_ucFile._path, task->_ucFile._size);
			// end
			_TriggerUploadSuccessfulEvent(task);
		}
	}
	else {
		//下载完成
		int32_t errorCodeUsed = errorCode;
		if (errorCodeUsed == UC_SUCCEEDED)
		{
			if (task->_ucFile._size == 0
				|| task->_ucFile._size != task->_offset) {
				errorCodeUsed = FT_ERROR_FILE_SIZE;
			}
		}

		if (UC_SUCCEEDED != errorCodeUsed) {
			_TriggerDownloadFailedEvent(errorCode, task->_taskId);
			return;
		}

		try {
			task->_ucFile.GenerateFileFromCacheFile();
			UpdateLocalFilePathAndSize(task->_downloadURL, task->_ucFile._path, task->_ucFile._size);
			_TriggerDownloadSuccessfulEvent(task->_taskId, task->_ucFile._path);
		}
		catch (boost::filesystem::filesystem_error e) {
			UC_LOG(ERROR) << "FileTransferService::RequestEnded error, file rename exception:" << e.what();
			UC_LOG(INFO) << "path=" << task->_ucFile._path.c_str() 
				<< ", cachePath=" << task->_ucFile._cachePath.c_str();
			_TriggerDownloadFailedEvent(FT_ERROR_FILE_RENAME, task->_taskId);
		}
	}
}

void FileTransferService::_UploadFileInThread(shared_ptr<Task> task) {
	//先插入队列是为了解决在获取MD5时，用户取消了任务
	_tasks.AddTask(task);
	int32_t result = task->Init();
	if (UC_SUCCEEDED != result) {
		UC_LOG(ERROR) << "FileTransferService::_UploadFile error. Init task failed. taskId : "
			<< task->_taskId << ", filePath : " << task->_ucFile._path;
		_TriggerUploadFailedEvent(result, 0);
		return;
	}

	if (task->_isCanceled 
		|| _UploadFile(task) != UC_SUCCEEDED) {
		_tasks.DeleteTask(task->_taskId);
	}
}

void FileTransferService::_CopyFileInThread(int32_t taskId, const std::string &from, 
	const std::string &to, int64_t size, const std::string &downloadUrl) {
	if (from == to
		|| UC_SUCCEEDED == _CheckFile(to, size)) {
		//新下载的路径跟数据库记录的路径一致或者新下载的路径中已经存在该文件
		_TriggerDownloadSuccessfulEvent(taskId, to);
		return;
	}

	try
	{
		filehelp::copy_file(from, to);
		UpdateLocalFilePathAndSize(downloadUrl, to, size);
		_TriggerDownloadSuccessfulEvent(taskId, to);
	}
	catch (fs::filesystem_error e) {
		boost::system::error_code code = e.code();
		UC_LOG(ERROR) << "FileTransferService::_CopyFileInThread， encounter an exception when file operator , exception: error code: "
			<< code.value() << ", error message:" << e.what();
		_TriggerDownloadFailedEvent(FT_ERROR_FILE_SIZE, taskId);
	}
}

int32_t FileTransferService::_UploadFile(shared_ptr<Task> task) {
	//文件是否正在上传
	if (_tasks.IsFileUploading(task)) {
		UC_LOG(ERROR) << "UploadFile error. file is uploading.";
		_TriggerUploadFailedEvent(FT_ERROR_FILE_UPLOADING, task->_taskId);
		return FT_ERROR_FILE_UPLOADING;
	}

	HTTPRequest request = _GetFSRequestAssist().MakeRequestOfCheckFile(task->_ucFile);
	_FSNetwork.SendAsyncRequest(0, request, MakeRequestCallback(this, &FileTransferService::OnFileChecked, task->_taskId), HTTP_TIMEOUT, HTTP_RESPONSE_INFINITY);
	return UC_SUCCEEDED;
}

} /* namespace uc */
