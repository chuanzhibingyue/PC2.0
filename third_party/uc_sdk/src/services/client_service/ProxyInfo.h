/*
 * ProxyInfo.h
 *
 *  Created on: 2015年11月18日
 *      Author: yongshuai.cheng
 */

#ifndef PROXYINFO_H_
#define PROXYINFO_H_

#include <string>
#include "UCSDK.h"

namespace uc {

using namespace std;

class ProxyInfo {
public:
	ProxyInfo():_proxyUrl(""), _authType(0), _proxyAccount(""),	_proxyPassword(""){}
	virtual ~ProxyInfo(){}

public:
	const string& GetProxyUrl() const {
		return _proxyUrl;
	}

	void SetProxyUrl(const string& proxyUrl) {
		_proxyUrl = proxyUrl;
	}

	int GetAuthType() const {
		return _authType;
	}

	void SetAuthType(int authType) {
		_authType = authType;
	}

	const string& GetProxyAccount() const {
		return _proxyAccount;
	}

	void SetProxyAccount(const string& proxyAccount) {
		_proxyAccount = proxyAccount;
	}

	const string& GetProxyPassword() const {
		return _proxyPassword;
	}

	void SetProxyPassword(const string& proxyPassword) {
		_proxyPassword = proxyPassword;
	}


private:
	string  _proxyUrl;
	int _authType;
	string  _proxyAccount;
	string  _proxyPassword;
};

} /* namespace uc */

#endif /* PROXYINFO_H_ */
