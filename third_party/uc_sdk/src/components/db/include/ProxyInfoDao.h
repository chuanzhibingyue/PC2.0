#ifndef __PROXYINFODAO_H__
#define __PROXYINFODAO_H__

#include "AppDao.h"
#include "client_service/ProxyInfo.h"

namespace uc {

class ProxyInfoDao : public AppDao
{
public:
	ProxyInfoDao();
	virtual ~ProxyInfoDao();

	int SaveProxyAuthInfo(const ProxyInfo& proxyInfo);

	int GetAuthInfo(ProxyInfo& proxyInfo);

	int DeleteAuthInfo(const std::string& proxy_url);

private:
	CppSQLite3DB* _db;
};

}

#endif