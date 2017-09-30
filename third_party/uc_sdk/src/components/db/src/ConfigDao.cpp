///////////////////////////////////////////////////////////
//  ConfigDao.cpp
//  Implementation of the Class ConfigDao
//  Created on:      22-五月-2015 10:36:10
//  Original author: Administrator
///////////////////////////////////////////////////////////

#include "ConfigDao.h"
#include "LogManager.h"

namespace uc {

ConfigDao::ConfigDao(){
	_db = SQLiteManager::GetInstance().GetUserDB();
}

ConfigDao::~ConfigDao(){
	SQLiteManager::GetInstance().ReleaseUserDB(_db);
}

int ConfigDao::SaveConfigValue(const string& application, const string& key, const string& value) {
	CppSQLite3Statement stmt = _db->compileStatement(
		"INSERT OR REPLACE INTO TBL_CLIENT_CONFIG(APPLICATION, CFG_NAME, CFG_VALUE) VALUES(?, ?, ?);");
	int i = 1;
	stmt.bind(i++, application.c_str());
	stmt.bind(i++, key.c_str());
	stmt.bind(i++, value.c_str());
	int ret = ExecDML(stmt);
	stmt.reset();
	if (ret != 0) {
		UC_LOG(ERROR) << "SaveConfigValue return error:" << ret
			<< "; application:" << application
			<< ", cfg_name:" << key
			<< ", cfg_value:" << value;
		return DBError::DB_INSERT_ERROR;
	}
	return DBError::DB_SUCCESS;
}

int ConfigDao::SaveConfigValue(const string& application, const map<string, string>& value) {
	int ret = DBError::DB_SUCCESS;
	ExecDML("BEGIN TRANSACTION;", _db);
	for (map<string, string>::const_iterator itor = value.begin();
		itor != value.end(); ++itor) {
		ret = SaveConfigValue(application, itor->first, itor->second);
		if (ret != 0) {
			break;
		}
	}
	ExecDML("COMMIT TRANSACTION;", _db);
	return ret;
}

int ConfigDao::GetConfigValue(const string& application, const string& key, string& value) {
	CppSQLite3Statement stmt = _db->compileStatement(
		"SELECT CFG_VALUE FROM TBL_CLIENT_CONFIG WHERE APPLICATION = ? AND CFG_NAME = ?;");
	int i = 1;
	stmt.bind(i++, application.c_str());
	stmt.bind(i++, key.c_str());
	CppSQLite3Query query;
	int ret = Query(stmt, query);
	if (ret != 0) {
		UC_LOG(ERROR) << "GetConfigValue return error:" << ret
			<< "; application:" << application
			<< ", AND cfg_name:" << key;
		return DBError::DB_QUERY_ERROR;
	}

	if (!query.eof()) {
		value = query.fieldValue(0);
	} else {
		UC_LOG(ERROR) << "GetConfigValue return db empty error!"
			<< "; application:" << application
			<< ", AND cfg_name:" << key;
		return DBError::DB_QUERY_EMPTY;
	}
	return DBError::DB_SUCCESS;
}

int ConfigDao::GetConfigValue(const string& application, map<string, string>& valueMap) {
	CppSQLite3Statement stmt = _db->compileStatement(
		"SELECT CFG_NAME, CFG_VALUE FROM TBL_CLIENT_CONFIG WHERE APPLICATION = ?;");
	stmt.bind(1, application.c_str());
	CppSQLite3Query query;
	int ret = Query(stmt, query);
	if (ret != 0) {
		UC_LOG(ERROR) << "GetConfigValue return error:" << ret
			<< "; application:" << application;
		return DBError::DB_QUERY_ERROR;
	}

	if (query.eof()) {
		UC_LOG(ERROR) << "GetConfigValue return db empty error! application:" << application;
		return DBError::DB_QUERY_EMPTY; 
	}

	string key;
	string value;
	while (!query.eof()) {
		int i = 0;
		key = query.fieldValue(i++);
		value = query.fieldValue(i++);
		valueMap[key] = value;
		query.nextRow();
	}
	return DBError::DB_SUCCESS;
}

int ConfigDao::UpdateConfigValue(const string& application, const string& key, const string& value) {
	CppSQLite3Statement stmt = _db->compileStatement(
		"UPDATE TBL_CLIENT_CONFIG SET CFG_VALUE = ? WHERE APPLICATION = ? AND CFG_NAME = ?;");
	int i = 1;
	stmt.bind(i++, value.c_str());
	stmt.bind(i++, application.c_str());
	stmt.bind(i++, key.c_str());
	int ret = ExecDML(stmt);
	if (ret != 0) {
		UC_LOG(ERROR) << "UpdateConfigValue return error:" << ret
			<< "; cfg_value:" << value
			<< ", application:" << application
			<< ", cfg_name:" << key;
		ret = DBError::DB_UPDATE_ERROR;
	}
	return ret;
}

int ConfigDao::UpdateConfigValue(const string& application, const map<string, string>& value) {
	int ret = DBError::DB_SUCCESS;
	ExecDML("BEGIN TRANSACTION;", _db);
	for (map<string, string>::const_iterator itor = value.begin();
		itor != value.end(); ++itor) {
		ret = UpdateConfigValue(application, itor->first, itor->second);
		if (ret != 0) {
			break;
		}
	}
	ExecDML("COMMIT TRANSACTION;", _db);
	return ret;
}

} //namespace uc
