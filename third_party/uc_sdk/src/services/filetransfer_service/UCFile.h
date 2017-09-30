/*
 * UCFile.h
 *
 *  Created on: 2015-9-7
 *      Author: hualong.zhang
 */

#ifndef UCFILE_H_
#define UCFILE_H_

#include <map>
#include "common/platform.h"
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

namespace fs = boost::filesystem;

namespace uc {

using namespace std;

class UCFile {
public:
	UCFile(const std::string &filePath, const std::string &downloadURL = "");
	int32_t InitForUpload();
	std::tuple<int32_t, int64_t> InitForDownload();
	int32_t GetFileMD5();
	std::string URLEncodeFileName() const;
	int32_t OpenForWrite();
	int32_t OpenForRead();
	void Write(void *data, int32_t length);
	void Read(int64_t _offset, char *buffer, int32_t readSize);
	void Close();
	void GenerateFileFromCacheFile();
	void Clean(bool removeCacheFile);
	~UCFile();

	fs::fstream _fstream;			//文件流
	std::string _path;				//文件路径
	std::string _cachePath;		    //缓存文件路径
	std::string _name;				//文件名称 
	std::string _md5;				//文件MD5值 
	int64_t _size;					//文件大小
private:
	int32_t _OpenFile(const std::string &filePath, std::ios_base::openmode mode);
	std::string _MakeCachePath(const std::string& filePath, const std::string& downloadURL);
	std::string _RetrievePartFId(const std::string &downloadURL);
	std::string _FilterIllegalFileNameChars(std::string &cacheFileId);
	std::string _RetrieveCacheFilePostfix(int32_t sourcePathLength, const std::string &downloadURL);
	bool _CreateEmptyFile();

	const std::string _postfixName;
	const int32_t _maxPostfixName;
	const int32_t _maxPathLength;
};

} /* namespace uc */
#endif /* UCFILE_H_ */
