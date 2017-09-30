/*
 * LoginInfo.cpp
 *
 *  Created on: 2015年5月7日
 *      Author: jianxue
 */

#include "LoginInfo.h"
#include "CommonDef.h"
#include "UCSDK.h"

namespace uc {

LoginInfo::LoginInfo() {
	// TODO Auto-generated constructor stub
	_account = "";
	_password = "";
	_clientInfo = "";
	_clientType = UCCLIENT_WIN;
	_uccServerDomain = UCCSERVER_URL;
	_lastKickTime = 0;
	_sessionID = "";
	_customerCode = "";
	_pwdExpiredDays = 90;
	_forceChangePwd = false;
	_AccountType = 0;

	_pbxKey = "";
	_pbxToken = "";
}

LoginInfo::~LoginInfo() {
	// TODO Auto-generated destructor stub
	ClearServerMap();
}

/**
 *
 * @param type
 * @param domain
 * @param cname
 * @param token
 * @return
 */
bool LoginInfo::AddServerInfo(ServerType type, const string &domain, const string &cname, const string &token) {
	ServerMap::iterator itor;

	itor = _servers.find(type);
	if (itor != _servers.end()) {
		ServerInfo &serverInfo = itor->second;
		serverInfo.Update(domain, cname, token);
	} else {
		_servers.insert(pair<int16_t, ServerInfo>(type, ServerInfo(type, domain, cname, token)));
	}

	return true;
}

bool LoginInfo::AddServerInfo(ServerType type, const string &domain) {
	string cname = "";
	string token = "";

	return AddServerInfo(type, domain, cname, token);
}

/**
 * 根据server类型获取serverinfo对象
 */
ServerInfo LoginInfo::GetServerInfo(ServerType type) const {
	ServerMap::const_iterator itor;
	itor = _servers.find(type);
	if (itor != _servers.end()) {
		return itor->second;
	}
	return ServerInfo(0, "", "", "");
}

/**
 * 清空servermap
 */
void LoginInfo::ClearServerMap() {
	_servers.clear();
}

} /* namespace uc */
