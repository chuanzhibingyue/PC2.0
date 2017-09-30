/*
 * ServerInfo.h
 *
 *  Created on: 2015年5月7日
 *      Author: jianxue
 */

#ifndef SERVERINFO_H_
#define SERVERINFO_H_

#include <string>

#include "common/platform.h"

namespace uc {

using namespace std;

enum ServerType {
	UCAS_HTTP_SERVER = 1,
	UCAS_HTTPS_SERVER,
	FS_SERVER,
	RELAY_SERVER,
	UPGRADE_SERVER,
	PACKAGEINFO_SERVER,
	PACKAGE_DOWNLOAD_SERVER
};

class ServerInfo {
public:
	ServerInfo(int16_t type, const string &domain, const string &cname, const string &token);
	virtual ~ServerInfo();

public:
	int16_t GetType();
	string& GetDomain();
	string& GetCName();
	string& GetToken();
	void Update(const string &domain, const string &cname, const string &token);
	void SetDomain(const string &domain);
	void SetCName(const string &cname);
	void SetToken(const string &token);

private:
	ServerInfo(){}

protected:
	int16_t _type;
	string _domain;
	string _cname;
	string _token;
};

} /* namespace uc */

#endif /* SERVERINFO_H_ */
