///////////////////////////////////////////////////////////
//  AppDao.h
//  Implementation of the Class AppDao
//  Created on:      22-五月-2015 10:34:51
//  Original author: Administrator
///////////////////////////////////////////////////////////

#if !defined(APPDAO_H_)
#define APPDAO_H_

#include "SQLiteManager.h"
#include "DBManagerDef.h"
#include "common/StringConvert.h"
#include "UCSDK.h"

#include <string>
#include <sstream>

namespace uc {

using namespace std;

class AppDao {
public:
	AppDao();
	virtual ~AppDao();

public:
	int Open(const string& dbPath, CppSQLite3DB* db);
	bool TableExists(const string& table, CppSQLite3DB* db);
	int Close(CppSQLite3DB* db);
	int ExecDML(const string& sql,CppSQLite3DB* db);
	int ExecDML(CppSQLite3Statement& stmt);
	int ExecDMLEx(CppSQLite3Statement& stmt);
	int Query(const string& sql, CppSQLite3DB* db, CppSQLite3Query& query);
	int Query(CppSQLite3Statement& stmt, CppSQLite3Query& query);

};

}
#endif // !defined(APPDAO_H_)
