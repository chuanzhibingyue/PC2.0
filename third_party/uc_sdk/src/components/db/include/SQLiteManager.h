///////////////////////////////////////////////////////////
//  SQLiteManager.h
//  Implementation of the Class SQLiteManager
//  Created on:      22-五月-2015 10:35:01
//  Original author: Administrator
///////////////////////////////////////////////////////////

#if !defined(SQLITEMANAGER_H_)
#define SQLITEMANAGER_H_

#include "UCSDK.h"
#include "CppSQLite3.h"
#include "DBManagerDef.h"

#include "boost/thread/mutex.hpp"
#include "boost/thread.hpp"

#include <string>
#include <map>
#include <list>

namespace uc {

using namespace std;

//数据连接池大小
#define COMMON_DB_CONNECTION_SIZE 2
#define USER_DB_CONNECTION_SIZE 6
#define EXTERNAL_DB_CONNECTION_SIZE 2

#ifdef PBX_SERVICE
#define PBX_DB_CONNECTION_SIZE 2
#endif // PBX_SERVICE

class SQLiteManager {
public:
	virtual ~SQLiteManager();

	static SQLiteManager& GetInstance();
	int InitCommonDB(const string& dbPath);
	int InitUserDB(const string& dbPath);
	int InitExternalDB(const string& dbPath);

	CppSQLite3DB* GetCommonDB();
	CppSQLite3DB* GetUserDB();
	CppSQLite3DB* GetExternalDB();

	void ReleaseCommonDB(CppSQLite3DB* db);
	void ReleaseUserDB(CppSQLite3DB* db);
	void ReleaseExternalDB(CppSQLite3DB* db);

#ifdef PBX_SERVICE
	int InitPbxDB(const std::string& dbPath, const std::string &strVer);
	CppSQLite3DB* GetPbxDB();
	void ReleasePbxDB(CppSQLite3DB* db);
#endif // PBX_SERVICE

private:
	SQLiteManager();
	int CheckCommonDB(const string& dbPath, int& version);
	int InitCommonDBConnection(const string& dbPath);
	int CheckUserDB(const string& dbPath);
	int InitUserDBConnection(const string& dbPath);
	int CheckExternalDB(const string& dbPath);
	int InitExternalDBConnection(const string& dbPath);
	int SaveDefaultConfig(const std::string& tabName, const string& app, const string& key, int value, CppSQLite3DB* db);
	void UnInitCommonDBConnection();
	void UnInitUserDBConnection();
	void UnInitExternalDBConnection();
	void ReleaseConnPool(list<CppSQLite3DB*>& connPool);
	void CheckDBChange(const string& dbPath);
	int GetDBVersion(CppSQLite3DB* userDb, const std::string& tabName);
	int EncryptAccount();
	int AddAccountTypeColumn();

#ifdef PBX_SERVICE
	int CheckPbxDB(const std::string& dbPath, const std::string &strVer);
	int InitPbxDBConnection(const std::string& dbPath);
	void UnInitPbxDBConnection();
	bool CheckPbxVer(const std::string& dbPath, const std::string &strVer);
#endif // PBX_SERVICE

private:
	//公共数据库连接池
	boost::recursive_mutex _commonMtx;
	boost::condition_variable_any _commonCond;
	list<CppSQLite3DB*> _commonDbConnectionPool;

	//用户数据库连接池
	boost::recursive_mutex _userMtx;
	boost::condition_variable_any _userCond;
	list<CppSQLite3DB*> _userDbConnectionPool;

	//外部数据库连接池;
	boost::recursive_mutex _externalMtx;
	boost::condition_variable_any _externalCond;
	list<CppSQLite3DB*> _externalDbConnectionPool;
#ifdef PBX_SERVICE
	//PBX数据库连接池;
	boost::recursive_mutex _pbxMtx;
	boost::condition_variable_any _pbxCond;
	list<CppSQLite3DB*> _pbxDbConnectionPool;
#endif // PBX_SERVICE
};

}

#endif // !defined(SQLITEMANAGER_H_)
