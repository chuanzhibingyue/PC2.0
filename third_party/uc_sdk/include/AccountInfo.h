#ifndef ACCOUNTINFO_H_
#define ACCOUNTINFO_H_

#include <string>
#include "UCSDK.h"

namespace uc {

using namespace std;

class UC_API AccountInfo
{
public:
	AccountInfo(void) {
	}

	virtual ~AccountInfo(void) {
	}

	bool IsAutoLogin() const {
		return _autoLogin;
	}

	void SetAutoLogin(bool autoLogin) {
		_autoLogin = autoLogin;
	}

	const string& GetLocalAvatar() const {
		return _localAvatar;
	}

	void SetLocalAvatar(const string& localAvatar) {
		_localAvatar = localAvatar;
	}

	int64_t GetLastManualLoginTime() const {
		return _lastManualLoginTime;
	}

	void SetLastManualLoginTime(int64_t lastManualLoginTime) {
		_lastManualLoginTime = lastManualLoginTime;
	}

	int64_t GetLoginTime() const {
		return _loginTime;
	}

	void SetLoginTime(int64_t loginTime) {
		_loginTime = loginTime;
	}

	const string& GetPassword() const {
		return _password;
	}

	void SetPassword(const string& password) {
		_password = password;
	}

	bool IsSavePassword() const {
		return _savePassword;
	}

	void SetSavePassword(bool savePassword) {
		_savePassword = savePassword;
	}

	const string& GetUserAccount() const {
		return _userAccount;
	}

	void SetUserAccount(const string& userAccount) {
		_userAccount = userAccount;
	}

	const string& GetDisplayName() const {
		return _userName;
	}

	void SetUserName(const string& userName) {
		_userName = userName;
	}

	void SetAccountType(const int32_t & accountType){
		_accountType = accountType;
	}

	int32_t GetAccountType() const {
		return _accountType;
	}

private:
	string _userAccount;
	string _userName;
	string _localAvatar;
	int64_t _loginTime;
	string _password;
	int64_t _lastManualLoginTime;
	bool _savePassword;
	bool _autoLogin;
	int32_t _accountType; // 用户类型， 1：普通用户，2：企业用户
};

} /* namespace uc */

#endif /* ACCOUNTINFO_H_ */
