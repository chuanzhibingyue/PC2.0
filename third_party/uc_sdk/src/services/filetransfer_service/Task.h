/*
 * Task.h
 *
 *  Created on: 2015-9-7
 *      Author: hualong.zhang
 */

#ifndef TASK_H_
#define TASK_H_

#include <map>
#include "common/platform.h"
#include <boost/thread/recursive_mutex.hpp>
#include <boost/atomic/atomic.hpp>
#include "UCFile.h"

namespace uc {

using namespace std;

//文件上传或下载任务
class Task {
public:
	Task(int32_t taskId, const std::string &filePath, const std::string &downloadURL = "", int32_t mode = 0, int32_t cvtType = 0);
	int32_t Init();
	const std::string RetrieveDescription();
	std::tuple<bool, int32_t> HandleOnDataReceived(void *data, int32_t length);
	std::tuple<bool, int32_t, int32_t> HandleOnDataSended(char *data, int32_t size);
	int64_t RetrieveRemainLen();
	int32_t _HandleFirstSend(char *buffer, int32_t size);
	int32_t _HandleRealData(char *buffer, int32_t size);
	static int32_t MakeTaskId();
	int32_t _taskId;				//任务ID
	int32_t _mode;					//任务类型 0--上传， 1--下载
	int32_t _cvtType;				//上传时转换类型：0--不转换，1--缩略图，2--mp4，默认不转换
	UCFile _ucFile;					//文件信息
	int32_t _progress;				//进度
	bool _isCanceled;				//是否被取消
	bool _isFirstSend;				//是否第一次上传数据
	bool _isFailed;					//是否已经失败
	int64_t _offset;				//历史已上传或下载的数据
	int64_t _completedSize;			//本次任务已经上传（或下载）完成的数据大小
	std::string _bodyHeader;		//上传时body头
	std::string _downloadURL;		//下载地址
	std::string _cvtDownloadURL;	//转换后文件的下载地址
	std::string _fid;				//文件ID
	std::string _cvtFid;			//转换后的文件ID
private:
	int32_t _RetrievePersent();
};

class Tasks{
public:
	Tasks();
	virtual ~Tasks();
	void Clear();
	bool IsFileUploading(shared_ptr<Task> newTask);
	bool IsFileDownloading(const std::string &noUserDownloadUrl, int32_t taskId);
	shared_ptr<Task> GetTask(int32_t taskId);
	bool DoesTaskExist(int32_t taskId);
	bool DoesTaskExist(const std::string &noUserDownloadUrl);
	void AddTask(shared_ptr<Task> task);
	void DeleteTask(int32_t taskId, bool removeCacheFile = false);
	int32_t RetrieveTaskIdFromDownloadUrl(const std::string &noUserDownloadUrl);

private:
	std::map<int32_t, shared_ptr<Task>> _tasks;
	boost::recursive_mutex _mtx;
};

} /* namespace uc */
#endif /* TASK_H_ */
