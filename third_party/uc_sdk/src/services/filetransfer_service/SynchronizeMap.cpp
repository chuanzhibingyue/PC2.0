/*
* SynchronizeMap.cpp
*
*  Created on: Sept 7, 2015
*      Author: hualong.zhang
*/

#include "SynchronizeMap.h"
#include "UCErrorCode.h"
#include "LogManager.h"
#include "md5/md5.h"
#include "base/StringConvert.h"

namespace uc {
	using namespace std;


	Task::Task(const std::wstring &filePath, int32_t mode/* = 0*/, const std::string &downloadURL/* = ""*/, int32_t cvtType/* = 0*/)
		: _fileInfo(filePath, downloadURL)
	{
		_mode = mode;
		_cvtType = cvtType;

		_taskId = _MakeTaskId();
		_progress = 0;
		_isCanceled = false;
		_isFirstSend = true;
		_cvtDownloadURL = "";
		_bodyHeader = "";
		_offset = 0;
		_completedSize = 0;
	}

	int32_t Task::Init() {
		path p(_fileInfo._path);
		try{
			if (_mode == 0) {
				//上传任务
				_fileInfo._size = fs::file_size(p);
				if (0 != _GetFileMD5(_fileInfo._path, _fileInfo._md5)) {
					return FT_ERROR_FILE_MD5;
				}
				_fileInfo._fstream.open(p, ios::in | std::ios::binary);
			}
			else {
				//下载任务
				if (fs::exists(_fileInfo._cachPath)) {
					_offset = fs::file_size(_fileInfo._cachPath);
				}
				else {
					fs::create_directories(p.parent_path());
					if (!fs::is_directory(p.parent_path())) {
						return FT_ERROR_CREATE_DIR;
					}
				}
				_fileInfo._fstream.open(_fileInfo._cachPath, ios::out | ios::app | std::ios::binary);
			}

			if (!_fileInfo._fstream.is_open()) {
				return FT_ERROR_FILE_OPEN;
			}
		}
		catch (fs::filesystem_error e) {
			boost::system::error_code code = e.code();
			UC_LOG(ERROR) << "encounter an exception when file operator , exception: error code: " << code.value() << ", error message:" << e.what();
			return FT_FAIL;
		}

		return UC_SUCCEEDED;
	}

	int32_t Task::_MakeTaskId() {
		static boost::atomic_int taskId(1);
		return ((int32_t)time(NULL) + taskId++);
	}

	int32_t Task::_GetFileMD5(const std::wstring& filePath, std::string& md5) {
		path p(filePath);
		fs::ifstream ifs(p, std::ios::in | std::ios::binary);
		if (!ifs.is_open()) {
			return FT_ERROR_PARAMETER;
		}
		ifs.seekg(0, std::ios::beg);
		MD5 m;
		char *buff = new char[1024 * 16];
		while (!ifs.eof()) {
			ifs.read(buff, 1024);

			int nCount = ifs.gcount();
			if (nCount > 0)
				m.update(buff, nCount);
		}
		delete[]buff;

		md5 = m.finalize().hexdigest();
		ifs.close();

		return UC_SUCCEEDED;
	}

	FileInfo::FileInfo(std::wstring filePath, std::string downloadURL/* = L""*/)  {
		_size = 0;

		StringConvert::Replace(filePath, L"\\", L"/");
		_path = filePath;

		path p(filePath);
		_name = p.filename().wstring();

		if (!downloadURL.empty()) {
			FileTransferService::MakeCacheFileName(filePath, downloadURL, _cachPath);
		}
	}

	bool FileInfo::MakeCacheFileName(const std::wstring& filePath, const std::string& downloadURL, std::wstring& cachfilePath) {

		if (filePath.empty() || downloadURL.empty()) {
			return false;
		}

		cachfilePath = filePath;

		//MAX_PATH -10(".ucfsdown_")
		if (filePath.length() >= 250) {
			return true;
		}

		std::string strCacheFileId = "";
		int offset = downloadURL.find("fid=");
		int offsetend = downloadURL.find("$&u=");
		if (offset != std::string::npos && offset != std::string::npos && offset + 32 < offsetend) {
			strCacheFileId = downloadURL.substr(offset + 4, offsetend - offset - 4);
			std::string::iterator it = strCacheFileId.begin();
			while (it != strCacheFileId.end())
			{
				if (*it == '\\' || *it == '\/' || *it == ':' || *it == '?' || *it == '\"' || *it == '<' || *it == '>' || *it == '|') {
					it = strCacheFileId.erase(it);
				}
				else
					++it;
			}

			//为支持断点下载，缓存文件的名= 原文件名+ 部分fid;
			int fid_length = strCacheFileId.length();
			int fid_CanAppendlength = 260 - filePath.length() - 10;//保证缓存文件全路径长度需要小于MAX_PATH
			if (fid_length > 0 && fid_CanAppendlength > 0) {
				fid_CanAppendlength = fid_CanAppendlength > fid_length ? fid_length : fid_CanAppendlength;		//追加的fid不能超过总的fid
				fid_CanAppendlength = fid_CanAppendlength > 50 ? 50 : fid_CanAppendlength;			//追加的fid不能超过50个字符（防错）
				strCacheFileId = strCacheFileId.substr(0, fid_CanAppendlength);
			}
			else {
				strCacheFileId = "";
			}
		}

		cachfilePath = filePath + L".ucfsdown_";
		wstring cachFileId = StringConvert::FromUTF8Str(strCacheFileId);
		cachfilePath += cachFileId;
		return true;
	}

	FileInfo::~FileInfo() {
		if (_fstream.is_open()) {
			_fstream.close();
		}
	}

	bool Tasks::Add(const Task &task) {
		return true;
	}

	bool Tasks::Remove(const Task &task) {

	}

	bool IsTaskExist(int32_t taskID) {
	}
	
	Task GetTask(int32_t taskID) {

	}
}