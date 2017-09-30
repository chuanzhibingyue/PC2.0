#ifndef __HTTPPROXY_H__
#define __HTTPPROXY_H__

#include "proxy.h"
#include <string>
//#include <boost/thread.hpp>

namespace uc {

class HTTPProxy
{
public:
	~HTTPProxy();
	static HTTPProxy* GetInstance();
	std::string QueryProxyInfo(const std::string& domain);
	std::string GetCurProxyUrl() const;
	bool IsExistProxyUrl();
	void Reset();

protected:
	bool _query_proxy;
	char** _proxies;
	pxProxyFactory* _pf;
	std::string _proxy_url;

private:
	HTTPProxy();
	static HTTPProxy* _instance;
};

}

#endif