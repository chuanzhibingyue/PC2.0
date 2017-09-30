///////////////////////////////////////////////////////////
//  AccountDao.h
//  Implementation of the Class AccountDao
//  Created on:      22-五月-2015 10:35:14
//  Original author: Administrator
///////////////////////////////////////////////////////////

#if !defined(ACCOUNTDAO_H_)
#define ACCOUNTDAO_H_

#include "AppDao.h"
#include "AccountInfo.h"

#include <list>

namespace uc {

using namespace std;

class AccountDao : public AppDao {
public:
	AccountDao();
	virtual ~AccountDao();

	int32_t SaveAccount(const AccountInfo& account);
	int32_t SaveAccount(const list<AccountInfo>& accountList);
	int32_t GetAccount(const string& userAccount, AccountInfo& accountInfo);
	int32_t GetAccountList(list<AccountInfo>& accountList, bool isDecode = true);
	int32_t DeleteAccount(const string& userAccount);
	int32_t SetAutoLogin(const std::string& userAccount, bool autoLogin);
	int32_t RememberPassword(const string& userAccount, bool remember);
	int32_t UpdateAvatar(const std::string &userAccount, const std::string &localAvatar);
	int32_t Clear();
	int32_t AddAccountTypeColumn();

private:
	void SetAccount(CppSQLite3Query& query, AccountInfo& data, bool isDecode = true);
	int32_t SaveAccountInfo(const AccountInfo& account);

private:
	CppSQLite3DB* _db;

};

}
#endif // !defined(ACCOUNTDAO_H_)
