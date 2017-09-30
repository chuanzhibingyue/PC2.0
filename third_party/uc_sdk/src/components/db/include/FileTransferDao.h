/*
 * FileTransferDao.h
 *
 *  Created on: 2015年8月14日
 *      Author: xuejian
 */

#ifndef FILETRANSFERDAO_H_
#define FILETRANSFERDAO_H_

#include "AppDao.h"

namespace uc {

class FileTransferDao: public AppDao {
public:
	FileTransferDao();
	virtual ~FileTransferDao();
public:
	int32_t GetFSLocalPath(const std::string &fileURL, std::string &filePath, bool excludeIfNotExist = false);
	int32_t GetFSLocalPath(const std::list<string> &fileURLs, std::map<string, string> &filePaths, bool excludeIfNotExist = false);
	int32_t UpdateFSLocalPath(const std::string &fileURL, const std::string &filePath);
	int32_t GetFSLocalPathAndSize(const std::string &fileURL, std::string &filePath, int64_t &fileSize);
	int32_t UpdateFSLocalPathAndSize(const std::string &fileURL, const std::string &filePath, int64_t fileSize);

private:
	CppSQLite3DB* _db;
};

} /* namespace uc */

#endif /* FILETRANSFERDAO_H_ */
