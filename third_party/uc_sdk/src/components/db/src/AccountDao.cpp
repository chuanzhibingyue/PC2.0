///////////////////////////////////////////////////////////
//  AccountDao.cpp
//  Implementation of the Class AccountDao
//  Created on:      22-五月-2015 10:35:14
//  Original author: Administrator
///////////////////////////////////////////////////////////

#include "AccountDao.h"
#include "LogManager.h"
#include "utils/CrypWrapper.h"

#include <stdlib.h>

namespace uc {

AccountDao::AccountDao() {
	_db = SQLiteManager::GetInstance().GetCommonDB();
}

AccountDao::~AccountDao() {
	SQLiteManager::GetInstance().ReleaseCommonDB(_db);
}

int32_t AccountDao::SaveAccount(const AccountInfo& account) {
	int ret = SaveAccountInfo(account);
	return ret;
}

int32_t AccountDao::SaveAccountInfo(const AccountInfo& account) {
	CppSQLite3Statement stmt = _db->compileStatement(
		"INSERT OR REPLACE INTO TBL_HISTORY_ACCOUNT"
		"(USER_ACCOUNT, DISPLAY_NAME, LOCAL_AVATAR, LOGIN_TIME, PASSWORD, "
		"LAST_MANUAL_LOGIN_TIME, RECORD_PWD, AUTO_LOGIN, ACCOUNT_TYPE) "
		"VALUES(?, ?, ?, ?, ?, ?, ?, ?,?);");
	int i = 1;
	string accout = CrypWrapper::Encrypt(account.GetUserAccount());
	stmt.bind(i++, accout.c_str());
	stmt.bind(i++, account.GetDisplayName().c_str());
	stmt.bind(i++, account.GetLocalAvatar().c_str());
	stmt.bind(i++, account.GetLoginTime());
	string passwd = CrypWrapper::Encrypt(account.GetPassword());
	stmt.bind(i++, passwd.c_str());
	stmt.bind(i++, account.GetLastManualLoginTime());
	stmt.bind(i++, account.IsSavePassword());
	stmt.bind(i++, account.IsAutoLogin());
	stmt.bind(i++, account.GetAccountType());
	int ret = ExecDML(stmt); 
	stmt.reset();
	if (ret != 0) {
		UC_LOG(ERROR) << "SaveAccountInfo return error:" << ret
			<< "; user_account:" << account.GetUserAccount()
			<< ", display_name:" << account.GetDisplayName()
			<< ", local_avatar:" << account.GetLocalAvatar()
			<< ", login_time:" << account.GetLoginTime()
			<< ", password:" << account.GetPassword()
			<< ", last_manual_login_time:" << account.GetLastManualLoginTime()
			<< ", record_pwd:" << account.IsSavePassword()
			<< ", auto_login:" << account.IsAutoLogin()
			<< ", account_type:" << account.GetAccountType();

		return DBError::DB_INSERT_ERROR;
	}
	return DBError::DB_SUCCESS;
}

int32_t AccountDao::SaveAccount(const list<AccountInfo>& accountList) {
	int ret = DBError::DB_SUCCESS;
	ExecDML("BEGIN TRANSACTION;", _db);
	for (list<AccountInfo>::const_iterator itor = accountList.begin();
		itor != accountList.end(); ++itor) {
		ret = SaveAccountInfo(*itor);
		if (ret != 0) {
			break;
		}
	}
	ExecDML("COMMIT TRANSACTION;", _db);
	return ret;
}

int32_t AccountDao::GetAccount(const string& userAccount, AccountInfo& accountInfo) {
	CppSQLite3Statement stmt = _db->compileStatement(
		"SELECT * FROM TBL_HISTORY_ACCOUNT WHERE USER_ACCOUNT = ?;");
	stmt.bind(1, userAccount.c_str());
	CppSQLite3Query query;
	int ret = Query(stmt, query);
	if (ret != 0) {
		UC_LOG(ERROR) << "GetAccount return error:" << ret << "; user_account:" << userAccount;
		return DBError::DB_QUERY_ERROR;
	}

	if (!query.eof()) {
		SetAccount(query, accountInfo);
	} else {
		UC_LOG(ERROR) << "GetAccount return db empty error!" << "user_account:" << userAccount;
		return DBError::DB_QUERY_EMPTY;
	}
	return DBError::DB_SUCCESS;
}

int32_t AccountDao::GetAccountList(list<AccountInfo>& accountList, bool isDecode) {
	CppSQLite3Statement stmt = _db->compileStatement(
		"SELECT * FROM TBL_HISTORY_ACCOUNT ORDER BY LOGIN_TIME DESC;");
	CppSQLite3Query query;
	int ret = Query(stmt, query);
	if (ret != 0) {
		UC_LOG(ERROR) << "GetAccountList return error:" << ret;
		return DBError::DB_QUERY_ERROR;
	}

	if (query.eof()) {
		UC_LOG(ERROR) << "GetAccountList return db empty error!";
		return DBError::DB_QUERY_EMPTY;
	}

	while (!query.eof()) {
		AccountInfo data;
		SetAccount(query, data, isDecode);
		accountList.push_back(data);
		query.nextRow();
	}
	return DBError::DB_SUCCESS;
}

void AccountDao::SetAccount(CppSQLite3Query& query, AccountInfo& data, bool isDecode) {
	int i = 0;
	std::string account = isDecode ? CrypWrapper::Decrypt(query.fieldValue(i++)) : query.fieldValue(i++);
	data.SetUserAccount(account);
	data.SetUserName(query.fieldValue(i++));
	data.SetLocalAvatar(query.fieldValue(i++));
	data.SetLoginTime(StringConvert::Convert2Int64(query.fieldValue(i++)));
	std::string passwd = isDecode ? CrypWrapper::Decrypt(query.fieldValue(i++)) : query.fieldValue(i++);
	data.SetPassword(passwd);
	data.SetLastManualLoginTime(StringConvert::Convert2Int64(query.fieldValue(i++)));
	data.SetSavePassword(atoi(query.fieldValue(i++)));
	data.SetAutoLogin(atoi(query.fieldValue(i++)));
	data.SetAccountType(atoi(query.fieldValue(i++)));
}

int32_t AccountDao::DeleteAccount(const string& userAccount){
	CppSQLite3Statement stmt = _db->compileStatement(
		"DELETE FROM TBL_HISTORY_ACCOUNT WHERE USER_ACCOUNT = ?;");
	stmt.bind(1, userAccount.c_str());
	int ret = ExecDML(stmt);
	if (ret != 0) {
		UC_LOG(ERROR) << "DeleteAccount return error:" << ret 
			<< ", userAccount:" << userAccount;
		return DBError::DB_DELETE_ERROR;
	}

	return DBError::DB_SUCCESS;
}

int32_t AccountDao::SetAutoLogin(const std::string& userAccount, bool autoLogin){
	CppSQLite3Statement stmt = _db->compileStatement(
		"UPDATE TBL_HISTORY_ACCOUNT SET AUTO_LOGIN = ? WHERE USER_ACCOUNT = ?;");
	stmt.bind(1, autoLogin);
	stmt.bind(2, userAccount.c_str());
	int ret = ExecDML(stmt);
	if (ret != 0) {
		UC_LOG(ERROR) << "SetAutoLogin return error:" << ret 
			<< ", userAccount:" << userAccount;
		return DBError::DB_DELETE_ERROR;
	}
	return DBError::DB_SUCCESS;
}

int32_t AccountDao::RememberPassword(const string& userAccount, bool remember){
	CppSQLite3Statement stmt = _db->compileStatement(
		"UPDATE TBL_HISTORY_ACCOUNT SET RECORD_PWD = ? WHERE USER_ACCOUNT = ?;");
	stmt.bind(1, remember);
	stmt.bind(2, userAccount.c_str());
	int ret = ExecDML(stmt);
	if (ret != 0) {
		UC_LOG(ERROR) << "RememberPassword return error:" << ret 
			<< ", userAccount:" << userAccount;
		return DBError::DB_DELETE_ERROR;
	}
	return DBError::DB_SUCCESS;
}

int32_t AccountDao::UpdateAvatar(const std::string &userAccount, const std::string &localAvatar) {
	CppSQLite3Statement stmt = _db->compileStatement("UPDATE TBL_HISTORY_ACCOUNT SET LOCAL_AVATAR = ? WHERE USER_ACCOUNT = ?;");
	stmt.bind(1, localAvatar.c_str());
	stmt.bind(2, userAccount.c_str());
	int ret = ExecDML(stmt);
	if (ret != 0) {
		UC_LOG(ERROR) << "UpdateAvatar return error:" << ret
			<< ", userAccount:" << userAccount;
		return DBError::DB_DELETE_ERROR;
	}
	return DBError::DB_SUCCESS;
}

int32_t AccountDao::Clear() {
	CppSQLite3Statement stmt = _db->compileStatement(
		"DELETE FROM TBL_HISTORY_ACCOUNT;");
	int ret = ExecDML(stmt);
	if (ret != 0) {
		UC_LOG(ERROR) << "Clear return error:" << ret;
		return DBError::DB_DELETE_ERROR;
	}

	return DBError::DB_SUCCESS;
}

int32_t AccountDao::AddAccountTypeColumn(){

	// 先检测是否已经存在这列，因为全新安装时，虽然创建了表，但是会进入这个函数
	// http://mantis.gnetis.com/view.php?id=5708 
	CppSQLite3Statement stmt = _db->compileStatement(
		"SELECT * FROM TBL_HISTORY_ACCOUNT LIMIT 0;");
	CppSQLite3Query query;
	int ret = Query(stmt, query);
	if (ret != 0) {
		UC_LOG(ERROR) << "GetAccountList return error:" << ret;
		return DBError::DB_QUERY_ERROR;
	}

	int n = query.numFields();
	
	if (query.numFields() > 8)
	{
		std::string fieldName = query.fieldName(8);
		if(fieldName.compare("ACCOUNT_TYPE") == 0)
			return DBError::DB_SUCCESS;
	}


	stmt = _db->compileStatement(
		"ALTER TABLE TBL_HISTORY_ACCOUNT ADD ACCOUNT_TYPE INT DEFAULT 1;");
	ret = ExecDML(stmt);
	if (ret != 0){
		UC_LOG(ERROR) << "AddAccountTypeColumn error:" << ret;
		return DBError::DB_UPDATE_ERROR;
	}

	return DBError::DB_SUCCESS;
}

} //namespace uc
