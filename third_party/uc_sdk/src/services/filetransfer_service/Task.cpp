/*
* SynchronizeMap.cpp
*
*  Created on: Sept 7, 2015
*      Author: hualong.zhang
*/

#include "Task.h"
#include "UCErrorCode.h"
#include "LogManager.h"
#include "common/StringConvert.h"
#include <thread>

namespace uc {

using namespace std;

Task::Task(int32_t taskId, const std::string &filePath, const std::string &downloadURL/* = ""*/, int32_t mode/* = 0*/, int32_t cvtType/* = 0*/)
	: _ucFile(filePath, downloadURL)
{
	_taskId = taskId;
	_downloadURL = downloadURL;
	_mode = mode;
	_cvtType = cvtType;

	_progress = 0;
	_isCanceled = false;
	_isFirstSend = true;
	_fid = "";
	_cvtFid = "";
	_cvtDownloadURL = "";
	_bodyHeader = "";
	_offset = 0;
	_completedSize = 0;
	_isFailed = false;
}

int32_t Task::Init() {
	int32_t result = UC_SUCCEEDED;
	try{
		if (_mode == 0) {
			//上传任务
			result = _ucFile.InitForUpload();
		}
		else {
			//下载任务
			auto tp = _ucFile.InitForDownload();
			result = std::get<0>(tp);
			_offset = std::get<1>(tp);
		}
	}
	catch (fs::filesystem_error e) {
		UC_LOG(ERROR) << "encounter an exception when file operator , exception: error code: " << e.code().value()
			<< ", error message:" << e.what();
		result = FT_FAIL;
	}

	return result;
}

const std::string Task::RetrieveDescription() {
	std::stringstream stream;
	stream << "task description, ";
	stream << "taskId : " << _taskId;
	stream << "path : " << _ucFile._path;
	stream << "cachePath : " << _ucFile._cachePath;
	stream << "size : " << _ucFile._size;
	stream << ", mode : " << _mode;
	stream << ", cvtType : " << _cvtType;
	stream << ", progress : " << _progress;
	stream << ", isCanceled : " << _isCanceled;
	stream << ", ifFirstSend : " << _isFirstSend;
	stream << ", offset : " << _offset;
	stream << ", completedSize : " << _completedSize;
	stream << ", bodyHeader : " << _bodyHeader;
	stream << ", fid : " << _fid;
	stream << ", cvtFid : " << _cvtFid;
	return stream.str();
}

std::tuple<bool, int32_t> Task::HandleOnDataReceived(void *data, int32_t length) {
	_ucFile.Write(data, length);
	_completedSize += length;
	_offset += length;
	int32_t persent = _RetrievePersent();
	bool needTrigger = false;
	if (_progress < persent) {
		_progress = persent;
		needTrigger = true;
	}
	return make_tuple(needTrigger, _progress);
}

int64_t Task::RetrieveRemainLen() {
	return _ucFile._size - _offset;
}

std::tuple<bool, int32_t, int32_t> Task::HandleOnDataSended(char *buffer, int32_t size) {
	int32_t sizeRealSend = 0;
	if (_isFirstSend) {
		//先发送bodyheadr
		sizeRealSend = _HandleFirstSend(buffer, size);
	}
	else {
		//开始发送文件数据
		sizeRealSend = _HandleRealData(buffer, size);
	}

	//文件已经上传进度
	bool needTrigger = false;
	int32_t persent = _RetrievePersent();
	if (_progress < persent) {
		_progress = persent;
		needTrigger = true;
	}

	return make_tuple(needTrigger, _progress, sizeRealSend);
}

int32_t Task::MakeTaskId() {
	static boost::atomic_int taskId(1);
	return ((int32_t)time(NULL) + taskId++);
}

int32_t Task::_RetrievePersent() {
	float persent = _ucFile._size > 0 ? ((float)_offset / (float)_ucFile._size) * 100 : 0;
	persent = persent - 1;
	if (persent < 0) {
		persent = 0;
	}
	return (int32_t)persent;
}

int32_t Task::_HandleFirstSend(char *buffer, int32_t size) {
	static int completedHeaderSize = 0;
	int32_t actualSize = (int32_t)(_bodyHeader.length() - completedHeaderSize);
	char* bodyHeader = (char*)_bodyHeader.c_str();
	bodyHeader += completedHeaderSize;
	if (actualSize <= size) {
		memcpy(buffer, bodyHeader, actualSize);
		_isFirstSend = false;
		completedHeaderSize = 0;
		return actualSize;
	}
	else {
		memcpy(buffer, bodyHeader, size);
		completedHeaderSize += size;
		return size;
	}
}

int32_t Task::_HandleRealData(char *buffer, int32_t size) {
	int32_t readSize = (int32_t)(RetrieveRemainLen() > size ? size : RetrieveRemainLen());
	_ucFile.Read(_offset, buffer, readSize);
	_completedSize += readSize;
	_offset += readSize;
	return readSize;
}

Tasks::Tasks() {

}

Tasks::~Tasks() {
	Clear();
}

void Tasks::Clear() {
	boost::unique_lock<boost::recursive_mutex> lock(_mtx);
	_tasks.clear();

	lock.unlock();
}
bool Tasks::IsFileUploading(shared_ptr<Task> task) {
	boost::unique_lock<boost::recursive_mutex> lock(_mtx);
	if (DoesTaskExist(task->_taskId)) {
		//排除情况：之前失败的任务，现在重新来上传
		return false;
	}
	for (auto iter = _tasks.begin(); iter != _tasks.end(); iter++) {
		shared_ptr<Task> taskFromMap = iter->second;
		if (taskFromMap == NULL
			|| taskFromMap->_mode == 1) {
			continue;
		}

		//文件名与md5都相同
		if (taskFromMap->_ucFile._name == task->_ucFile._name
			&& taskFromMap->_ucFile._md5 == task->_ucFile._md5) {
			return true;
		}
	}
	return false;
}

bool Tasks::IsFileDownloading(const std::string &noUserDownloadUrl, int32_t taskId) {
	if (!DoesTaskExist(noUserDownloadUrl)) {
		return false;
	}

	if (taskId == 0) {
		//连续多次下载
		return true;
	} else {
		//resume task
		if (!DoesTaskExist(taskId)) {
			return true;
		} else {
			return false;
		}
	}
}

shared_ptr<Task> Tasks::GetTask(int32_t taskId) {
	boost::unique_lock<boost::recursive_mutex> lock(_mtx);
	auto iter = _tasks.find(taskId);
	if (iter == _tasks.end())
	{
		UC_LOG(ERROR) << "GetTask: taskId: " << taskId << "does not exist";
		return NULL;
	}

	return iter->second;
}

bool Tasks::DoesTaskExist(int32_t taskId)
{
	boost::unique_lock<boost::recursive_mutex> lock(_mtx);
	auto iter = _tasks.find(taskId);
	if (iter == _tasks.end())
	{
		return false;
	}
	return true;
}

bool Tasks::DoesTaskExist(const std::string &noUserDownloadUrl) {
	boost::unique_lock<boost::recursive_mutex> lock(_mtx);
	for (auto iter : _tasks) {
		if (iter.second->_downloadURL == noUserDownloadUrl) {
			return true;
		}
	}
	return false;
}

void Tasks::AddTask(shared_ptr<Task> task) {
	if (task == NULL) {
		return;
	}
	boost::unique_lock<boost::recursive_mutex> lock(_mtx);
	_tasks.insert(make_pair(task->_taskId, task));
}

void Tasks::DeleteTask(int32_t taskId, bool removeCacheFile) {
	boost::unique_lock<boost::recursive_mutex> lock(_mtx);
	auto iter = _tasks.find(taskId);
	if (iter == _tasks.end())
	{
		//当文件在本地已经存在或是从本地的其他地方拷贝过来时，会走到这
		return;
	}

	shared_ptr<Task> task = iter->second;
	task->_ucFile.Clean(removeCacheFile);
	_tasks.erase(iter);
}

int32_t Tasks::RetrieveTaskIdFromDownloadUrl(const std::string &noUserDownloadUrl) {
	boost::unique_lock<boost::recursive_mutex> lock(_mtx);
	for (auto iter : _tasks) {
		if (iter.second->_downloadURL == noUserDownloadUrl) {
			return iter.first;
		}
	}
	return 0;
}

}