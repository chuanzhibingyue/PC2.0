/*
 * FileTransferDao.cpp
 *
 *  Created on: 2015年8月14日
 *      Author: xuejian
 */

#include "FileTransferDao.h"
#include "LogManager.h"
#include <boost/filesystem.hpp>

namespace uc {

FileTransferDao::FileTransferDao() {
	_db = SQLiteManager::GetInstance().GetUserDB();
}

FileTransferDao::~FileTransferDao() {
	SQLiteManager::GetInstance().ReleaseUserDB(_db);
}

int32_t FileTransferDao::GetFSLocalPath(const std::list<string> &fileURLs, std::map<string, string> &filePaths, bool excludeIfNotExist) {
	if (fileURLs.empty()) {
		UC_LOG(ERROR) << "GetFSLocalPath input userIDList is empty!";
		return DBError::DB_INPUT_ERROR;
	}
	stringstream urlStream;
	list<string>::const_iterator itor = fileURLs.begin();
	urlStream << "\"" << *itor << "\"";

	for (++itor;itor != fileURLs.end(); ++itor) {
		urlStream << ","  << "\"" << *itor << "\"";
	}

	CppSQLite3Buffer sql;
	sql.format(
		"SELECT * FROM TBL_FILE_TRANSFER WHERE FILEURL IN (%s);", urlStream.str().c_str());

	CppSQLite3Query query;
	int ret = Query((const char*)sql, _db, query);
	if (ret != 0) {
		UC_LOG(ERROR) << "GetFSLocalPath query return error:" << ret
			<< ", sql:" << (const char*)sql;
		return DBError::DB_QUERY_ERROR;
	}

	while (!query.eof()) {
		int i = 0;
		std::string fileURL  = query.fieldValue(i++);
		std::string filePath = query.fieldValue(i++);

		if ((!excludeIfNotExist) ||
			(excludeIfNotExist && boost::filesystem::exists(boost::filesystem::path(StringConvert::FromUTF8Str(filePath))))) {
			filePaths[fileURL] = filePath;
		}

		query.nextRow();
	}

	if (filePaths.empty()) {
		return DBError::DB_QUERY_EMPTY;
	}
	return DBError::DB_SUCCESS;
}

int32_t FileTransferDao::GetFSLocalPath(const string &fileURL, string &filePath, bool excludeIfNotExist) {

	CppSQLite3Statement stmt = _db->compileStatement("SELECT LOCALPATH FROM TBL_FILE_TRANSFER WHERE FILEURL = ?;");

	int i = 1;
	stmt.bind(i++, fileURL.c_str());

	CppSQLite3Query query;
	int ret = Query(stmt, query);
	if (ret != 0) {
		UC_LOG(ERROR) << "UpdateFSLocalPath return error:" << ret
				<< "; FILEURL:" << fileURL;
		return DBError::DB_QUERY_ERROR;
	}

	if (!query.eof()) {
		int i = 0;
		filePath = query.fieldValue(i++);
		if (excludeIfNotExist && !boost::filesystem::exists(boost::filesystem::path(StringConvert::FromUTF8Str(filePath)))) {
			filePath = "";
			return DBError::DB_QUERY_EMPTY;
		}
	}

	return DBError::DB_SUCCESS;
}

int32_t FileTransferDao::UpdateFSLocalPath(const string &fileURL, const string &filePath) {
	CppSQLite3Statement stmt = _db->compileStatement(
			"INSERT OR REPLACE INTO TBL_FILE_TRANSFER (FILEURL, LOCALPATH) "
			"VALUES (?, ?);");
	int i = 1;
	stmt.bind(i++, fileURL.c_str());
	stmt.bind(i++, filePath.c_str());

	int ret = ExecDML(stmt);
	stmt.reset();
	if (ret != 0) {
		UC_LOG(ERROR) << "UpdateFSLocalPath return error:" << ret
				<< "; FILEURL:" << fileURL
				<< ", LOCALPATH:" << filePath;
		return DBError::DB_INSERT_ERROR;
	}
	return DBError::DB_SUCCESS;
}

int32_t FileTransferDao::GetFSLocalPathAndSize(const std::string &fileURL, std::string &filePath, int64_t &fileSize) {
	CppSQLite3Statement stmt = _db->compileStatement("SELECT LOCALPATH, FILE_SIZE FROM TBL_FILE_TRANSFER1 WHERE FILEURL = ?;");

	int i = 1;
	stmt.bind(i++, fileURL.c_str());

	CppSQLite3Query query;
	int ret = Query(stmt, query);
	if (ret != 0) {
		UC_LOG(ERROR) << "GetFSLocalPathAndSize return error:" << ret
			<< "; FILEURL:" << fileURL;
		return DBError::DB_QUERY_ERROR;
	}

	if (!query.eof()) {
		int i = 0;
		filePath = query.fieldValue(i++);
		fileSize = StringConvert::Convert2Int64(query.fieldValue(i++));
	}

	return DBError::DB_SUCCESS;
}

int32_t FileTransferDao::UpdateFSLocalPathAndSize(const std::string &fileURL, const std::string &filePath, int64_t fileSize) {
	CppSQLite3Statement stmt = _db->compileStatement(
		"INSERT OR REPLACE INTO TBL_FILE_TRANSFER1 (FILEURL, LOCALPATH, FILE_SIZE) "
		"VALUES (?, ?, ?);");

	int i = 1;
	stmt.bind(i++, fileURL.c_str());
	stmt.bind(i++, filePath.c_str());
	stmt.bind(i++, fileSize);

	int ret = ExecDML(stmt);
	stmt.reset();
	if (ret != 0) {
		UC_LOG(ERROR) << "UpdateFSLocalPath return error:" << ret
			<< "; FILEURL:" << fileURL
			<< ", LOCALPATH:" << filePath
			<< ", FILE_SIZE:" << fileSize;
		return DBError::DB_INSERT_ERROR;
	}
	return DBError::DB_SUCCESS;
}

} /* namespace uc */
