#include "HTTPProxy.h"
#include "LogManager.h"

namespace uc {

HTTPProxy* HTTPProxy::_instance = NULL;

HTTPProxy::HTTPProxy()
{
	_query_proxy = false;
	_proxies = NULL;
	_pf = px_proxy_factory_new();
}

HTTPProxy::~HTTPProxy()
{
	_query_proxy = false;

	if (_proxies)
	{
		free(_proxies);
		_proxies = NULL;
	}

	if (_pf)
	{
		px_proxy_factory_free(_pf);
		_pf = NULL;
	}
}

HTTPProxy* HTTPProxy::GetInstance()
{
	if (NULL == _instance)
	{
		_instance = new HTTPProxy();
	}
	return _instance;
}

std::string HTTPProxy::QueryProxyInfo(const std::string& domain)
{
	try
	{
		if (!_query_proxy)
		{
			_query_proxy = true;
			if (_pf)
			{
				if (NULL == _proxies)
				{
					_proxies = px_proxy_factory_get_proxies(_pf, domain.c_str());
					UC_LOG(INFO) << "starting retrieve proxy info from brower....";
				}

				//assert(_proxies);
				for (int i = 0; _proxies[i]; i++)
				{
					//assert(_proxies[i]);
					if (strncmp("direct", _proxies[i], 6) == 0)
					{
						UC_LOG(INFO) << "no proxy configied in IE";
						continue;
					}

					if (strncmp("http", _proxies[i], 4) == 0)
					{
						_proxy_url = _proxies[i];
						UC_LOG(INFO) << "get proxy url: " << _proxies[i];
						break;
					}
				}
			}
		}
	}
	catch (...)
	{
		UC_LOG(ERROR) << "retrieve proxy info error...";
	}
	//_lock.unlock();

	return _proxy_url;

}

std::string HTTPProxy::GetCurProxyUrl() const
{
	return _proxy_url;
}

bool HTTPProxy::IsExistProxyUrl()
{
	if (_proxy_url.empty())
	{
		return false;
	}

	std::string user_account;
	std::string user_passwd;
	int auth_type = 0;


	return true;
}

void HTTPProxy::Reset()
{
	_query_proxy = false;
	_proxy_url = "";

	if (_proxies)
	{
		free(_proxies);
		_proxies = NULL;
	}
}

}