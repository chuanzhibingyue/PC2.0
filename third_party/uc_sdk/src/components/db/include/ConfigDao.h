///////////////////////////////////////////////////////////
//  ConfigDao.h
//  Implementation of the Class ConfigDao
//  Created on:      22-五月-2015 10:36:10
//  Original author: Administrator
///////////////////////////////////////////////////////////

#if !defined(CONFIGDAO_H_)
#define CONFIGDAO_H_

#include "AppDao.h"

namespace uc {

class ConfigDao : public AppDao {
public:
	ConfigDao();
	virtual ~ConfigDao();

	int SaveConfigValue(const string& application, const string& key, const string& value);
	int SaveConfigValue(const string& application, const map<string, string>& value);
	int GetConfigValue(const string& application, const string& key, string& value);
	int GetConfigValue(const string& application, map<string, string>& valueMap);
	int UpdateConfigValue(const string& application, const string& key, const string& value);
	int UpdateConfigValue(const string& application, const map<string, string>& value);

private:
	CppSQLite3DB* _db;

};

}
#endif // !defined(CONFIGDAO_H_)
