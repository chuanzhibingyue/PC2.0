/*
 * SynchronizeMap.h
 *
 *  Created on: 2015-9-7
 *      Author: hualong.zhang
 */

#ifndef SYNCHRONIZE_MAP_H_
#define SYNCHRONIZE_MAP_H_

#include <map>
#include "common/platform.h"
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/thread/recursive_mutex.hpp>
#include <boost/atomic/atomic.hpp>

namespace fs = boost::filesystem;
using boost::filesystem::path;

namespace uc {

using namespace std;

class FileInfo {
public:
	FileInfo(std::wstring filePath, std::string downloadURL = "");
	bool MakeCacheFileName(const std::wstring& filePath, const std::string& downloadURL, std::wstring& cachfilePath);
	~FileInfo();

	fs::fstream _fstream;			//文件流
	std::wstring _path;				//文件路径
	std::wstring _cachPath;			//缓存文件路径
	std::wstring _name;				//文件名称 
	std::string _md5;				//文件MD5值 
	int64_t _size;					//文件大小
};

//文件上传或下载任务
class Task {
public:
	Task(const std::wstring &filePath, int32_t mode = 0, const std::string &downloadURL = "", int32_t cvtType = 0);
	int32_t Init();
	int32_t _taskId;				//任务ID
	int32_t _mode;					//任务类型 0--上传， 1--下载
	int32_t _cvtType;				//上传时转换类型：0--不转换，1--缩略图，2--mp4，默认不转换
	FileInfo _fileInfo;				//文件信息
	int32_t _progress;				//进度
	bool _isCanceled;				//是否被取消
	bool _isFirstSend;				//是否第一次上传数据
	int64_t _offset;				//历史已上传或下载的数据
	int64_t _completedSize;			//本次任务已经上传（或下载）完成的数据大小
	std::string _bodyHeader;		//上传时body头
	std::string _downloadURL;		//下载地址
	std::string _cvtDownloadURL;	//转换后文件的下载地址
private:
	int32_t _MakeTaskId();
	int32_t _GetFileMD5(const std::wstring& filePath, std::string& md5);
};

class Tasks {
private:
	typedef std::map<int32_t, Task> KeyValue;
	typedef std::map<int32_t, Task>::const_iterator ConstKeyValueIter;
	typedef std::map<int32_t, Task>::iterator KeyValueIter;
	KeyValue _tasks;

public:
	Tasks(){}
	virtual ~Tasks(){}

	bool Add(const Task &task);
	bool Remove(const Task &task);
	bool IsTaskExist(int32_t taskID);
	Task GetTask(int32_t taskID);
};

} /* namespace uc */
#endif /* SYNCHRONIZE_MAP_H_ */
