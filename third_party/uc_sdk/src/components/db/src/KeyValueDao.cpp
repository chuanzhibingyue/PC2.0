

#include "KeyValueDao.h"
#include "LogManager.h"

namespace uc {

KeyValueDao::KeyValueDao(){
	_db = SQLiteManager::GetInstance().GetUserDB();
}

KeyValueDao::~KeyValueDao(){
	SQLiteManager::GetInstance().ReleaseUserDB(_db);
}

int KeyValueDao::SaveValueByKey(const string& application, const string& key, const string& value) {
	CppSQLite3Statement stmt = _db->compileStatement(
		"INSERT OR REPLACE INTO TBL_KEY_VALUE(APPLICATION, CFG_NAME, CFG_VALUE) VALUES(?, ?, ?);");
	int i = 1;
	stmt.bind(i++, application.c_str());
	stmt.bind(i++, key.c_str());
	stmt.bind(i++, value.c_str());
	int ret = ExecDML(stmt);
	stmt.reset();
	if (ret != 0) {
		UC_LOG(ERROR) << "SaveValueByKey return error:" << ret
			<< "; application:" << application
			<< ", cfg_name:" << key
			<< ", cfg_value:" << value;
		return DBError::DB_INSERT_ERROR;
	}
	return DBError::DB_SUCCESS;
}


int KeyValueDao::GetValueByKey(const string& application, const string& key, string& value) {
	CppSQLite3Statement stmt = _db->compileStatement(
		"SELECT CFG_VALUE FROM TBL_KEY_VALUE WHERE APPLICATION = ? AND CFG_NAME = ?;");
	int i = 1;
	stmt.bind(i++, application.c_str());
	stmt.bind(i++, key.c_str());
	CppSQLite3Query query;
	int ret = Query(stmt, query);
	if (ret != 0) {
		UC_LOG(ERROR) << "GetValueByKey return error:" << ret
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

} //namespace uc
