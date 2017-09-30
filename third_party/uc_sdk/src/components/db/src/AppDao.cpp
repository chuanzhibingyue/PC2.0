///////////////////////////////////////////////////////////
//  AppDao.cpp
//  Implementation of the Class AppDao
//  Created on:      22-五月-2015 10:34:51
//  Original author: Administrator
///////////////////////////////////////////////////////////

#include "AppDao.h"
#include "LogManager.h"

#include <sstream>

namespace uc {

#if defined(WIN32) || defined(WIN64)
    #define Sleep(MSEC) Sleep(MSEC)
#else
    #define Sleep(MSEC) usleep(MSEC*1000)
#endif


AppDao::AppDao() {
}

AppDao::~AppDao() {
}

int AppDao::Open(const string& dbPath, CppSQLite3DB* db) {
	try {
		db->open(dbPath.c_str());
	} catch (CppSQLite3Exception* e) {
		UC_LOG(ERROR) << "DB open exception:" << e->errorMessage() << ", dbPath:" << dbPath;
		return -1;
	} catch (...) {
		UC_LOG(ERROR) << "DB open exception!dbPath:" << dbPath << ", error msg:" << db->getErrMsg();
		return -2;
	}
	return 0;
}

bool AppDao::TableExists(const string& table, CppSQLite3DB* db){
	bool ret = false;
	try {
		ret = db->tableExists(table.c_str());
	} catch (CppSQLite3Exception* e) {
		UC_LOG(ERROR) << "DB exists exception:" << e->errorMessage() << ", table:" << table;
		return false;
	} catch (...) {
		UC_LOG(ERROR) << "DB exists exception! table:" << table << ", error msg:" << db->getErrMsg();
		return false;
	}
	return ret;
}

int AppDao::Close(CppSQLite3DB* db) {
	try {
		db->close();
	} catch (CppSQLite3Exception* e) {
		UC_LOG(ERROR) << "DB close exception:" << e->errorMessage();
		return -1;
	} catch (...) {
		UC_LOG(ERROR) << "DB close exception!" << ", error msg:" << db->getErrMsg();
		return -2;
	}
	return 0;
}

int AppDao::ExecDML(const string& sql,CppSQLite3DB* db) {
	int tryCnt = 3;
	while (true) {
		try {
			db->execDML(sql.c_str());
		}
		catch (CppSQLite3Exception* e) {
			UC_LOG(ERROR) << "ExecDML exception:" << e->errorMessage() << ", sql:" << sql;
			return -1;
		}
		catch (...) {
			if (strstr(db->getErrMsg(), "database is locked")) {
				if (--tryCnt < 0) return -3;
				Sleep(50);
				UC_LOG(WARNING) << "try exec, current try count:" << tryCnt;
				continue;
			}
			UC_LOG(ERROR) << "ExecDML exception, sql:" << sql << ", error msg:" << db->getErrMsg() << ", sql:" << sql;
			return -2;
		}
		break;
	}
	return 0;
}

int AppDao::ExecDML(CppSQLite3Statement& stmt) {
	int tryCnt = 3;
	while (true) {
		try {
			stmt.execDML();
		}
		catch (CppSQLite3Exception* e) {
			UC_LOG(ERROR) << "ExecDML exception:" << e->errorMessage();
			return -1;
		}
		catch (...) {
			if (strstr(stmt.getErrMsg(), "database is locked")) {
				if (--tryCnt < 0) return -3;
				Sleep(50);
				UC_LOG(WARNING) << "try exec, current try count:" << tryCnt;
				continue;
			}
			UC_LOG(ERROR) << "ExecDML exception!" << ", error msg:" << stmt.getErrMsg();
			return -2;
		}
		break;
	}
	return 0;
}

int AppDao::ExecDMLEx(CppSQLite3Statement& stmt) {
	int tryCnt = 3;
	while (true) {
		try {
			return stmt.execDML();
		}
		catch (CppSQLite3Exception* e) {
			UC_LOG(ERROR) << "ExecDML exception:" << e->errorMessage();
			return -1;
		}
		catch (...) {
			if (strstr(stmt.getErrMsg(), "database is locked")) {
				if (--tryCnt < 0) return -3;
				Sleep(50);
				UC_LOG(WARNING) << "try exec, current try count:" << tryCnt;
				continue;
			}
			UC_LOG(ERROR) << "ExecDML exception!" << ", error msg:" << stmt.getErrMsg();
			return -2;
		}
		break;
	}
	return 0;
}

int AppDao::Query(const string& sql, CppSQLite3DB* db, CppSQLite3Query& query) {
	int tryCnt = 3;
	while (true) {
		try {
			query = db->execQuery(sql.c_str());
		}
		catch (CppSQLite3Exception* e) {
			UC_LOG(ERROR) << "Query exception:" << e->errorMessage() << ", sql:" << sql;
			return -1;
		}
		catch (...) {
			if (strstr(db->getErrMsg(), "database is locked")) {
				if (--tryCnt < 0) return -3;
				Sleep(50);
				UC_LOG(WARNING) << "try Query, current try count:" << tryCnt;
				continue;
			}
			UC_LOG(ERROR) << "Query exception!" << ", error msg:" << db->getErrMsg() << ", sql:" << sql;
			return -2;
		}
		break;
	}
	return 0;
}

int AppDao::Query(CppSQLite3Statement& stmt, CppSQLite3Query& query) {
	int tryCnt = 3;
	while (true) {
		try {
			query = stmt.execQuery();
		}
		catch (CppSQLite3Exception* e) {
			UC_LOG(ERROR) << "Query exception:" << e->errorMessage();
			return -1;
		}
		catch (...) {
			if (strstr(stmt.getErrMsg(), "database is locked")) {
				if (--tryCnt < 0) return -3;
				Sleep(50);
				UC_LOG(WARNING) << "try Query, current try count:" << tryCnt;
				continue;
			}
			UC_LOG(ERROR) << "Query exception!" << ", error msg:" << stmt.getErrMsg();
			return -2;
		}
		break;
	}
	return 0;
}

} //namespace uc