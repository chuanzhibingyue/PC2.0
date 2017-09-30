/*
* SynchronizeMap.cpp
*
*  Created on: Sept 7, 2015
*      Author: hualong.zhang
*/

#include "UCFile.h"
#include "UCErrorCode.h"
#include "LogManager.h"
#include "common/StringConvert.h"
#include "md5/md5.h"
#include "FSDef.h"
#include "FileHelp.hpp"

namespace uc {

using namespace std;

UCFile::UCFile(const std::string &filePath, const std::string &downloadUrl/* = L""*/)
	: _postfixName(".ucfsdown_")
	, _maxPathLength(260)
	, _maxPostfixName(32) {
	_size = 0;

	_path = filePath;
	_name = filehelp::filename(filePath);
	if (!downloadUrl.empty()) {
		_cachePath = _MakeCachePath(filePath, downloadUrl);
	}
}
	
UCFile::~UCFile() {
	if (_fstream.is_open()) {
		Close();
	}
}

int32_t UCFile::InitForUpload() {
	_size = filehelp::file_size(_path);
	if (0 != GetFileMD5()) {
		return FT_ERROR_FILE_MD5;
	}
	return OpenForRead();
}

std::tuple<int32_t, int64_t> UCFile::InitForDownload() {
	int64_t offset = 0;
	if (!filehelp::exists(_cachePath)) {
		if (!filehelp::create_directories(_cachePath)) {
			UC_LOG(ERROR) << "cachePath=" << _cachePath.c_str() << ", path" << _path.c_str();
			return make_tuple(FT_ERROR_CREATE_DIR, 0);
		}
	} else {
		offset = filehelp::file_size(_cachePath);
	}
	
	_CreateEmptyFile();

	int32_t result = OpenForWrite();
	return make_tuple(UC_SUCCEEDED, offset);
}

int32_t UCFile::_OpenFile(const std::string &filePath, std::ios_base::openmode mode) {
	const wstring unicodeFilePath = StringConvert::FromUTF8Str(filePath);
	_fstream.open(unicodeFilePath, mode);
	if (!_fstream.is_open()) {
		return FT_ERROR_FILE_OPEN;
	}
	return UC_SUCCEEDED;
}

std::string UCFile::_MakeCachePath(const std::string& filePath, const std::string& downloadURL) {

	if (filePath.empty() || downloadURL.empty()) {
		UC_LOG(ERROR) << "filePath=" << filePath.c_str() << ", downloadURL" << downloadURL.c_str();
		return "";
	}

	int curLen = (int32_t)(filePath.length() + _postfixName.length());
	if (curLen >= _maxPathLength)
	{
		UC_LOG(ERROR) << "filePath length=" << curLen;
		//当前传递进来的文件路径加上最短的下载后缀已经超出文件路径长度限制
		return "";
	}

	//为支持断点下载，缓存文件的名= 原文件名+ 部分fid;
	std::stringstream stream;
	stream << filePath << _RetrieveCacheFilePostfix(filePath.length(), downloadURL);
	return stream.str();
}

int32_t UCFile::GetFileMD5() {
	path p(StringConvert::FromUTF8Str(_path));
	fs::ifstream ifs(p, std::ios::in | std::ios::binary);
	if (!ifs.is_open()) {
		return FT_ERROR_PARAMETER;
	}
	ifs.seekg(0, std::ios::beg);
	MD5 m;
	char *buff = new char[1024 * 16];
	while (!ifs.eof()) {
		ifs.read(buff, 1024);

		MD5::size_type nCount = (MD5::size_type)ifs.gcount();
		if (nCount > 0)
			m.update(buff, nCount);
	}
	delete[]buff;

	_md5 = m.finalize().hexdigest();
	ifs.close();

	return UC_SUCCEEDED;
}

std::string UCFile::URLEncodeFileName() const {
	return StringConvert::URLEncode(_name);
}

int32_t UCFile::OpenForWrite() {
	return _OpenFile(_cachePath, ios::out | ios::app | std::ios::binary);
}

int32_t UCFile::OpenForRead() {
	return _OpenFile(_path, ios::in | std::ios::binary);
}

void UCFile::Write(void *data, int32_t length) {
	_fstream.seekg(0, std::ios::end);
	_fstream.write((char*)data, length);
	_fstream.flush();
}

void UCFile::Read(int64_t offset, char *buffer, int32_t readSize) {
	_fstream.seekg(offset);
	_fstream.read(buffer, readSize);
}

void UCFile::Close() {
	if (_fstream.is_open()) {
		_fstream.close();
	}
}

void UCFile::GenerateFileFromCacheFile() {
	Close();
	filehelp::rename(_cachePath, _path);
}

void UCFile::Clean(bool removeCacheFile) {
	Close();
	if (removeCacheFile 
		&& filehelp::exists(_cachePath)) {
		try {
			filehelp::remove(_cachePath);
		}
		catch (boost::filesystem::filesystem_error e) {
			UC_LOG(ERROR) << "FileTransferService::RemoveCacheFile error, file remove exception:" << e.what();
		}
	}
}

std::string UCFile::_RetrievePartFId(const std::string &downloadURL) {
	int offset = downloadURL.find(FS_URL_FID);
	if (offset == std::string::npos)
	{
		return "";
	}
	int fidStart = offset + strlen(FS_URL_FID);
	return downloadURL.substr(fidStart, _maxPostfixName);
}

std::string UCFile::_FilterIllegalFileNameChars(std::string &cacheFileId) {
	for (auto iter = cacheFileId.begin(); iter != cacheFileId.end();)
	{
		switch (*iter)
		{
		case '\\':
		case '/':
		case ':':
		case '?':
		case '\"':
		case '<':
		case '>':
		case '|':
			iter = cacheFileId.erase(iter);
			break;
		default:
			iter++;
			break;
		}
	}
	return cacheFileId;
}

std::string UCFile::_RetrieveCacheFilePostfix(int32_t sourcePathLength, const std::string &downloadURL) {
	std::string fid = _RetrievePartFId(downloadURL);
	fid = _FilterIllegalFileNameChars(fid);
	if ((int32_t)(sourcePathLength + _postfixName.length() + fid.length()) >= _maxPathLength)
	{
		//加上后缀的全路径超过文件长度限制
		return _postfixName;
	}

	std::stringstream stream;
	stream << _postfixName << fid;
	return stream.str();
}

bool UCFile::_CreateEmptyFile() {
	if (filehelp::exists(_path)) {
		UC_LOG(INFO) << "_CreateEmptyFile path is exists, path=" << _path.c_str();
		return true;
	}

// 	fstream hfs;
// 	const wstring unicodeFilePath = StringConvert::FromUTF8Str(_path);
// 	hfs.open(unicodeFilePath, ios_base::out | std::ios::binary);
// 	if (!hfs.is_open()) {
// 		return false;
// 	}
// 	hfs.close();

	return true;
}

}