///////////////////////////////////////////////////////////
//  SQLiteManager.cpp
//  Implementation of the Class SQLiteManager
//  Created on:      22-五月-2015 10:35:02
//  Original author: Administrator
///////////////////////////////////////////////////////////

#include "SQLConfig.h"
#include "SQLiteManager.h"
#include "AppDao.h"
#include "common/StringConvert.h"
#include "LogManager.h"
#include "AccountDao.h"

namespace uc {

SQLiteManager::SQLiteManager() {
	sqlite3_config(SQLITE_CONFIG_MULTITHREAD);
}

SQLiteManager::~SQLiteManager() {
	UnInitCommonDBConnection();
	UnInitUserDBConnection();
	UnInitExternalDBConnection();
#ifdef PBX_SERVICE
	UnInitPbxDBConnection();
#endif // PBX_SERVICE
}

void SQLiteManager::UnInitCommonDBConnection() {
	boost::unique_lock<boost::recursive_mutex> lock(_commonMtx);
	ReleaseConnPool(_commonDbConnectionPool);
}

void SQLiteManager::UnInitUserDBConnection() {
	boost::unique_lock<boost::recursive_mutex> lock(_userMtx);
	ReleaseConnPool(_userDbConnectionPool);
}

void SQLiteManager::UnInitExternalDBConnection() {
	boost::unique_lock<boost::recursive_mutex> lock(_externalMtx);
	ReleaseConnPool(_externalDbConnectionPool);
}


void SQLiteManager::ReleaseConnPool(list<CppSQLite3DB*>& connPool) {
	for (list<CppSQLite3DB*>::iterator itor = connPool.begin();
		itor != connPool.end(); ++itor) {
			delete *itor;
			*itor = NULL;
	}
	connPool.clear();
}

SQLiteManager& SQLiteManager::GetInstance() {
	static SQLiteManager instance;
	return instance;
}

int SQLiteManager::InitCommonDB(const string& dbPath) {
	UnInitCommonDBConnection();
	int version = 0;
	int ret = CheckCommonDB(dbPath, version);
	if (ret != 0) {
		UC_LOG(ERROR) << "CheckCommonDB return error:" << ret;
		return ret;
	}

	ret = InitCommonDBConnection(dbPath);
	if (ret != 0) {
		UC_LOG(ERROR) << "InitCommonDBConnection return error:" << ret;
	}

	if (version < DB_ACCOUNT_TYPE_VERSION)
	{
		// ALTER TABLE
		AddAccountTypeColumn();
	}

	if (version < DB_ENCRYPTION_VERSION) {
		EncryptAccount();
	}
	return ret;
}

int SQLiteManager::CheckCommonDB(const string& dbPath, int& version) {
	AppDao dao;
	CppSQLite3DB db;
	int ret = dao.Open(dbPath, &db);
	if (ret != 0) {
		UC_LOG(ERROR) << "Open common db return error:" << ret << ", dbPath:" << dbPath;
		return DBError::DB_OPEN_ERROR;
	}

	if (!dao.TableExists(HISTORY_ACCOUNT, &db)) {
		ret = dao.ExecDML(CREATE_HISTORY_ACCOUNT, &db);
		if (ret != 0) {
			UC_LOG(ERROR) << "Create history account table return error:" << ret << ", sql:" << CREATE_HISTORY_ACCOUNT;
			return DBError::DB_CREATE_TABLE_ERROR;
		}
	}

	if (!dao.TableExists(PROXY_AUTH_INFO, &db)) {
		ret = dao.ExecDML(CREATE_PROXY_AUTH_INFO, &db);
		if (ret != 0) {
			UC_LOG(ERROR) << "Create proxy auth info table return error:" << ret << ", sql:" << CREATE_PROXY_AUTH_INFO;
			return DBError::DB_CREATE_TABLE_ERROR;
		}
	}

	if (!dao.TableExists(COMMON_CONFIG, &db)) {
		ret = dao.ExecDML(CREATE_COMMON_CONFIG, &db);
		if (ret != 0) {
			UC_LOG(ERROR) << "Create common configure table return error:" << ret << ", sql:" << CREATE_COMMON_CONFIG;
			return DBError::DB_CREATE_TABLE_ERROR;
		}
	}
	else {
		version = GetDBVersion(&db, COMMON_CONFIG);
	}
	SaveDefaultConfig(COMMON_CONFIG, "DBManager", "DBVersion", DB_ACCOUNT_TYPE_VERSION, &db);

	return ret;
}

int SQLiteManager::InitCommonDBConnection(const string& dbPath) {
	boost::unique_lock<boost::recursive_mutex> lock(_commonMtx);
	AppDao dao;
	for (int i = 0; i < COMMON_DB_CONNECTION_SIZE; i++) {
		CppSQLite3DB* db = new CppSQLite3DB();
		if (dao.Open(dbPath, db) != 0) {
			return DBError::DB_OPEN_ERROR;
		}
		_commonDbConnectionPool.push_back(db);
	}
	return DBError::DB_SUCCESS;
}

int SQLiteManager::EncryptAccount() {
	AccountDao dao;
	list<AccountInfo> accountList;
	int ret = dao.GetAccountList(accountList, false);
	if (ret != 0) {
		UC_LOG(ERROR) << "GetAccountList return error:" << ret;
		return ret;
	}

	ret = dao.Clear();
	if (ret != 0) {
		UC_LOG(ERROR) << "Clear return error:" << ret;
		return ret;
	}

	ret = dao.SaveAccount(accountList);
	if (ret != 0) {
		UC_LOG(ERROR) << "SaveAccount return error:" << ret;
	}
	return ret;
}

int SQLiteManager::AddAccountTypeColumn()
{
	AccountDao dao;
	int ret = dao.AddAccountTypeColumn();
	if (ret != 0)
	{
		UC_LOG(ERROR) << "AddAccountTypeColumn return error:" << ret;
		return ret;
	}

	return ret;
}

int SQLiteManager::InitUserDB(const string& dbPath) {
	UnInitUserDBConnection();
	int ret = CheckUserDB(dbPath);
	if (ret != 0) {
		UC_LOG(ERROR) << "CheckUserDB return error:" << ret;
		return ret;
	}

	ret = InitUserDBConnection(dbPath);
	if (ret != 0) {
		UC_LOG(ERROR) << "InitUserDBConnection return error:" << ret;
	}
	return ret;
}

int SQLiteManager::CheckUserDB(const string& dbPath) {
	CheckDBChange(dbPath);
	AppDao dao;
	CppSQLite3DB db;
	int ret = dao.Open(dbPath, &db);
	if (ret != 0) {
		UC_LOG(ERROR) << "Open user db return error:" << ret << ", dbPath:" << dbPath;
		return DBError::DB_OPEN_ERROR;
	}

	//config
	if (!dao.TableExists(CONFIG, &db)) {
		ret = dao.ExecDML(CREATE_CONFIG, &db);
		if (ret != 0) {
			UC_LOG(ERROR) << "Create config table return error:" << ret << ", sql:" << CREATE_CONFIG;
			return DBError::DB_CREATE_TABLE_ERROR;
		}
	}
	SaveDefaultConfig(CONFIG, "DBManager", "DBVersion", DB_NEW_VERSION, &db);

	//contact
	if (!dao.TableExists(CONTACT, &db)) {
		ret = dao.ExecDML(CREATE_CONTACT, &db);
		if (ret != 0) {
			UC_LOG(ERROR) << "Create contact table return error:" << ret << ", sql:" << CREATE_CONTACT;
			return DBError::DB_CREATE_TABLE_ERROR;
		}
		//set last_contact_sync_time to 0
		SaveDefaultConfig(CONFIG, "ContactService", "last_contact_sync_time", 0, &db);
	}

	if (!dao.TableExists(GROUP, &db)) {
		ret = dao.ExecDML(CREATE_GROUP, &db);
		if (ret != 0) {
			UC_LOG(ERROR) << "Create group table return error:" << ret << ", sql:" << CREATE_GROUP;
			return DBError::DB_CREATE_TABLE_ERROR;
		}
	}

	if (!dao.TableExists(GROUP_MEMBER, &db)) {
		ret = dao.ExecDML(CREATE_GROUP_MEMBER, &db);
		if (ret != 0) {
			UC_LOG(ERROR) << "Create group member table return error:" << ret << ", sql:" << CREATE_GROUP_MEMBER;
			return DBError::DB_CREATE_TABLE_ERROR;
		}
	}

	if (!dao.TableExists(MESSAGE, &db)) {
		ret = dao.ExecDML(CREATE_MESSAGE, &db);
		if (ret != 0) {
			UC_LOG(ERROR) << "Create message table return error:" << ret << ", sql:" << CREATE_MESSAGE;
			return DBError::DB_CREATE_TABLE_ERROR;
		}
	}

	if (!dao.TableExists(CONVERSATION, &db)) {
		ret = dao.ExecDML(CREATE_CONVERSATION, &db);
		if (ret != 0) {
			UC_LOG(ERROR) << "Create message list table return error:" << ret << ", sql:" << CREATE_CONVERSATION;
			return DBError::DB_CREATE_TABLE_ERROR;
		}
	}

	if (!dao.TableExists(DELCONVERSATION, &db)){
		ret = dao.ExecDML(CREATE_DELCONVERSATION, &db);
		if (ret != 0){
			UC_LOG(ERROR) << "Create del_conversation table return error:" << ret << ", sql:" << CREATE_DELCONVERSATION;
			return DBError::DB_CREATE_TABLE_ERROR;
		}
	}

	if (!dao.TableExists(CONFERENCE, &db)) {
		ret = dao.ExecDML(CREATE_CONFERENCE, &db);
		if (ret != 0) {
			UC_LOG(ERROR) << "Create conference table return error:" << ret << ", sql:" << CREATE_CONFERENCE;
			return DBError::DB_CREATE_TABLE_ERROR;
		}
	}
	if (!dao.TableExists(CONF_MEMBER, &db)) {
		ret = dao.ExecDML(CREATE_CONF_MEMBER, &db);
		if (ret != 0) {
			UC_LOG(ERROR) << "Create conference member table return error:" << ret << ", sql:" << CREATE_CONF_MEMBER;
			return DBError::DB_CREATE_TABLE_ERROR;
		}
	}
	if (!dao.TableExists(SHARE, &db)) {
		ret = dao.ExecDML(CREATE_SHARE, &db);
		if (ret != 0) {
			UC_LOG(ERROR) << "Create conference share table return error:" << ret << ", sql:" << CREATE_SHARE;
			return DBError::DB_CREATE_TABLE_ERROR;
		}
	}
	if (!dao.TableExists(FILETRANSFER, &db)) {
		ret = dao.ExecDML(CREATE_FILETRANSFER, &db);
		if (ret != 0) {
			UC_LOG(ERROR) << "Create FileTransfer table return error:" << ret << ", sql:" << CREATE_FILETRANSFER;
			return DBError::DB_CREATE_TABLE_ERROR;
		}
	}
	if (!dao.TableExists(FILETRANSFER1, &db)) {
		ret = dao.ExecDML(CREATE_FILETRANSFER1, &db);
		if (ret != 0) {
			UC_LOG(ERROR) << "Create FileTransfer1 table return error:" << ret << ", sql:" << CREATE_FILETRANSFER;
			return DBError::DB_CREATE_TABLE_ERROR;
		}
	}

	if (!dao.TableExists(KEYVALUE, &db)) {
		ret = dao.ExecDML(CREATE_KEYVALUE, &db);
		if (ret != 0) {
			UC_LOG(ERROR) << "Create keyvalue table return error:" << ret << ", sql:" << CREATE_KEYVALUE;
			return DBError::DB_CREATE_TABLE_ERROR;
		}
	}

	if (!dao.TableExists(CALL_PHONENUM, &db)) {
		ret = dao.ExecDML(CREATE_CALL_PHONENUM, &db);
		if (ret != 0) {
			UC_LOG(ERROR) << "Create TBL_CALL_PHONENUM table return error:" << ret << ", sql:" << CREATE_CALL_PHONENUM;
			return DBError::DB_CREATE_TABLE_ERROR;
		}
	}

	return DBError::DB_SUCCESS;
}

int SQLiteManager::InitUserDBConnection(const string& dbPath) {
	boost::unique_lock<boost::recursive_mutex> lock(_userMtx);
	AppDao dao;
	for (int i = 0; i < USER_DB_CONNECTION_SIZE; i++) {
		CppSQLite3DB* db = new CppSQLite3DB();
		if (dao.Open(dbPath, db) != 0) {
			return DBError::DB_OPEN_ERROR;
		}
		_userDbConnectionPool.push_back(db);
	}
	return DBError::DB_SUCCESS;
}

int SQLiteManager::InitExternalDB(const string& dbPath) {
	UnInitExternalDBConnection();
	int ret = CheckExternalDB(dbPath);
	if (ret != 0) {
		UC_LOG(ERROR) << "CheckExternalDB return error:" << ret;
		return ret;
	}

	ret = InitExternalDBConnection(dbPath);
	if (ret != 0) {
		UC_LOG(ERROR) << "InitExternalDBConnection return error:" << ret;
	}
	return ret;
}

int SQLiteManager::CheckExternalDB(const string& dbPath) {
	AppDao dao;
	CppSQLite3DB db;
	int ret = dao.Open(dbPath, &db);
	if (ret != 0) {
		UC_LOG(ERROR) << "Open external db return error:" << ret << ", dbPath:" << dbPath;
		return DBError::DB_OPEN_ERROR;
	}

	if (!dao.TableExists(EXTERNAL_MESSAGE, &db)) {
		UC_LOG(ERROR) << "check external message table return error:" << ret;
		return DBError::DB_OPEN_ERROR;
	}

	return ret;
}

int SQLiteManager::InitExternalDBConnection(const string& dbPath) {
	boost::unique_lock<boost::recursive_mutex> lock(_externalMtx);
	AppDao dao;
	for (int i = 0; i < EXTERNAL_DB_CONNECTION_SIZE; i++) {
		CppSQLite3DB* db = new CppSQLite3DB();
		if (dao.Open(dbPath, db) != 0) {
			return DBError::DB_OPEN_ERROR;
		}
		_externalDbConnectionPool.push_back(db);
	}
	return DBError::DB_SUCCESS;
}

CppSQLite3DB* SQLiteManager::GetCommonDB() {
	boost::unique_lock<boost::recursive_mutex> lock(_commonMtx);
	while (_commonDbConnectionPool.empty()) {
		_commonCond.wait(lock);
	}

	CppSQLite3DB* connection = _commonDbConnectionPool.front();
	_commonDbConnectionPool.pop_front();
	return connection;
}

CppSQLite3DB* SQLiteManager::GetUserDB() {
	boost::unique_lock<boost::recursive_mutex> lock(_userMtx);
	while (_userDbConnectionPool.empty()) {
		_userCond.wait(lock);
	}

	CppSQLite3DB* connection = _userDbConnectionPool.front();
	_userDbConnectionPool.pop_front();
	return connection;
}

CppSQLite3DB* SQLiteManager::GetExternalDB() {
	boost::unique_lock<boost::recursive_mutex> lock(_externalMtx);
	while (_externalDbConnectionPool.empty()) {
		_externalCond.wait(lock);
	}

	CppSQLite3DB* connection = _externalDbConnectionPool.front();
	_externalDbConnectionPool.pop_front();
	return connection;
}

void SQLiteManager::ReleaseCommonDB(CppSQLite3DB* db) {
	boost::unique_lock<boost::recursive_mutex> lock(_commonMtx);
	_commonDbConnectionPool.push_back(db);
	_commonCond.notify_one();
}

void SQLiteManager::ReleaseUserDB(CppSQLite3DB* db) {
	boost::unique_lock<boost::recursive_mutex> lock(_userMtx);
	_userDbConnectionPool.push_back(db);
	_userCond.notify_one();
}

void SQLiteManager::ReleaseExternalDB(CppSQLite3DB* db) {
	boost::unique_lock<boost::recursive_mutex> lock(_externalMtx);
	_externalDbConnectionPool.push_back(db);
	_externalCond.notify_one();
}

#ifdef PBX_SERVICE
int SQLiteManager::InitPbxDB(const std::string& dbPath, const std::string &strVer) {

	UnInitPbxDBConnection();
	int ret = CheckPbxDB(dbPath, strVer);
	if (ret != 0) {
		UC_LOG(ERROR) << "CheckPbxDB return error:" << ret;
		return ret;
	}

	ret = InitPbxDBConnection(dbPath);
	if (ret != 0) {
		UC_LOG(ERROR) << "InitPbxDBConnection return error:" << ret;
	}

	return ret;
}

CppSQLite3DB* SQLiteManager::GetPbxDB() {
	boost::unique_lock<boost::recursive_mutex> lock(_pbxMtx);
	while (_pbxDbConnectionPool.empty()) {
		_pbxCond.wait(lock);
	}

	CppSQLite3DB* connection = _pbxDbConnectionPool.front();
	_pbxDbConnectionPool.pop_front();
	return connection;
}

void SQLiteManager::ReleasePbxDB(CppSQLite3DB* db) {
	boost::unique_lock<boost::recursive_mutex> lock(_pbxMtx);
	_pbxDbConnectionPool.push_back(db);
	_pbxCond.notify_one();
}

int SQLiteManager::CheckPbxDB(const std::string& dbPath, const std::string &strVer) {
	AppDao dao;
	CppSQLite3DB db;
	int ret = dao.Open(dbPath, &db);
	if (ret != 0) {
		UC_LOG(ERROR) << "Open Pbx db return error:" << ret << ", dbPath:" << dbPath;
		return DBError::DB_OPEN_ERROR;
	}

	bool isDrop = CheckPbxVer(dbPath, strVer);
	if (isDrop){
		std::string strDrop = "DROP TABLE ";
        
        CppSQLite3Query query;
		ret = dao.Query(strDrop + TABNAME_PBXCONFIG + ";", &db, query);
		ret = dao.Query(strDrop + TABNAME_PBXCALLRECORD + ";", &db, query);
	}

	if (!dao.TableExists(TABNAME_PBXCONFIG, &db)) {
		ret = dao.ExecDML(CREATE_PBXCONFIG, &db);
		if (ret != 0) {
			UC_LOG(ERROR) << "Create history account table return error:" << ret << ", sql:" << CREATE_PBXCONFIG;
			return DBError::DB_CREATE_TABLE_ERROR;
		}
	}

	if (isDrop) {
		std::string strSql = "INSERT OR REPLACE INTO ";
		strSql += TABNAME_PBXCONFIG;
		strSql += " (application, app_name, app_value)VALUES(?, ?, ?);";
		CppSQLite3Statement stmt = db.compileStatement(strSql.c_str());

		int index = 1;
		stmt.bind(index++, "DBManager");
		stmt.bind(index++, "DBVersion");
		stmt.bind(index++, strVer.c_str());

		ret = dao.ExecDML(stmt);
		stmt.reset();
		if (ret != 0) {
			return DBError::DB_INSERT_ERROR;
		}
	}

	if (!dao.TableExists(TABNAME_PBXCALLRECORD, &db)) {
		ret = dao.ExecDML(CREATE_PBXCALLRECORD, &db);
		if (ret != 0) {
			UC_LOG(ERROR) << "Create history account table return error:" << ret << ", sql:" << CREATE_PBXCALLRECORD;
			return DBError::DB_CREATE_TABLE_ERROR;
		}
	}

	dao.Close(&db);

	return ret;
}

int SQLiteManager::InitPbxDBConnection(const std::string& dbPath) {
	boost::unique_lock<boost::recursive_mutex> lock(_pbxMtx);
	AppDao dao;
	for (int i = 0; i < PBX_DB_CONNECTION_SIZE; i++) {
		CppSQLite3DB* db = new CppSQLite3DB();
		if (dao.Open(dbPath, db) != 0) {
			return DBError::DB_OPEN_ERROR;
		}
		_pbxDbConnectionPool.push_back(db);
	}
	return DBError::DB_SUCCESS;
}

void SQLiteManager::UnInitPbxDBConnection() {
	boost::unique_lock<boost::recursive_mutex> lock(_pbxMtx);
	ReleaseConnPool(_pbxDbConnectionPool);
}

bool SQLiteManager::CheckPbxVer(const std::string& dbPath, const std::string &strVer){
	AppDao dao;
	CppSQLite3DB db;
	int ret = dao.Open(dbPath, &db);
	if (ret != 0) {
		UC_LOG(ERROR) << "Open Pbx db return error:" << ret << ", dbPath:" << dbPath;
		return DBError::DB_OPEN_ERROR;
	}

	std::string strSql;
	strSql = "SELECT app_value FROM ";
	strSql += TABNAME_PBXCONFIG;
	strSql += " WHERE application='DBManager' AND app_name='DBVersion';";

	bool isDrop = true;
	CppSQLite3Query query;
	ret = dao.Query(strSql, &db, query);
	if (ret == 0 && !query.eof()) {
		std::string Temp = query.fieldValue(0);
		isDrop = (0 != strVer.compare(Temp));
	}

	dao.Close(&db);

	return isDrop;
}
#endif // PBX_SERVICE

int SQLiteManager::SaveDefaultConfig(const std::string& tabName, const string& app, const string& key, int value, CppSQLite3DB* db) {
	CppSQLite3Buffer sql;
	sql.format("INSERT OR REPLACE INTO %s(APPLICATION, CFG_NAME, CFG_VALUE) "
		"VALUES('%s', '%s', '%d');", tabName.c_str(), app.c_str(), key.c_str(), value);
	int ret = AppDao().ExecDML((const char*)sql, db);
	if (ret != 0) {
		UC_LOG(ERROR) << "SaveDefaultConfig return error:" << ret << ", sql:" << (const char*)sql;
		return DBError::DB_INSERT_ERROR;
	}
	return DBError::DB_SUCCESS;
}

void SQLiteManager::CheckDBChange(const string& dbPath) {
	AppDao dao;
	CppSQLite3DB db;
	int ret = dao.Open(dbPath, &db);
	if (ret != 0) {
		UC_LOG(ERROR) << "Open user db return error:" << ret << ", dbPath:" << dbPath;
		return;
	}

	//config
	if (!dao.TableExists(CONFIG, &db)) {
		return;
	}
	int oldVersion = GetDBVersion(&db, CONFIG);
	if (oldVersion != DB_NEW_VERSION) {
		int ret = AppDao().ExecDML(DROP_CHANGED_TABLES, &db);
		UC_LOG(INFO) << "drop changed tables:" << DROP_CHANGED_TABLES << ", return:" << ret;
	}
}

int SQLiteManager::GetDBVersion(CppSQLite3DB* userDb, const std::string& tabName) {
	CppSQLite3Buffer sql;
	sql.format("SELECT CFG_VALUE FROM %s WHERE APPLICATION = 'DBManager' AND CFG_NAME = 'DBVersion';", tabName.c_str());
	CppSQLite3Query query;
	int ret = AppDao().Query((const char*)sql, userDb, query);
	if (ret != 0) {
		UC_LOG(ERROR) << "GetDBVersion return error:" << ret << ", sql:" << (const char*)sql;
		return 0;
	}
	if (query.eof()) {
		UC_LOG(WARNING) << "GetDBVersion return db empty! sql:" << (const char*)sql;
		return 0;
	}

	int version = atoi(query.fieldValue(0));
	UC_LOG(INFO) << "DBVersion:" << version;
	return version;
}

}
