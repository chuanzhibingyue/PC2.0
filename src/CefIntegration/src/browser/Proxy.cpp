#include <Windows.h>
#include <winhttp.h>
#include "browser/Proxy.h"
#include "LogManager.h"

namespace cefIntegration
{
    using namespace uc;

    Proxy::Proxy()
    {
        _proxyType = Proxy_Invalid;
        _pac = L"";
        _server = L"";
        _serverPass = L"";
        _user = L"";
        _pwd = L"";
    }

    Proxy::~Proxy()
    {

    }

    int Proxy::GetProxyType()
    {
        if (Proxy_Invalid != _proxyType)
        {
            return _proxyType;
        }

        WINHTTP_CURRENT_USER_IE_PROXY_CONFIG MyProxyConfig;
        if (!WinHttpGetIEProxyConfigForCurrentUser(&MyProxyConfig))
        {
            DWORD Err = GetLastError();
            UC_LOG(ERROR) << "get ie proxy config fail. error:" << Err;

            return Proxy_Invalid;
        }//end else

        _proxyType = Proxy_No; // 默认是不需要代理
        if (NULL != MyProxyConfig.lpszAutoConfigUrl)
        {
            UC_LOG(INFO) << "proxy pac url:" << MyProxyConfig.lpszAutoConfigUrl;
            _proxyType = Proxy_Pac;
            _pac = std::wstring(MyProxyConfig.lpszAutoConfigUrl);
            GlobalFree(MyProxyConfig.lpszAutoConfigUrl);
        }

        if (NULL != MyProxyConfig.lpszProxy)
        {
            UC_LOG(INFO) << "proxy server:" << MyProxyConfig.lpszProxy;
            _proxyType = Proxy_Server;
            _server = std::wstring(MyProxyConfig.lpszProxy);
            GlobalFree(MyProxyConfig.lpszProxy);
        }

        if (NULL != MyProxyConfig.lpszProxyBypass)
        {
            UC_LOG(INFO) << "proxy server pass:" << MyProxyConfig.lpszProxyBypass;
            _serverPass = std::wstring(MyProxyConfig.lpszProxyBypass);
            GlobalFree(MyProxyConfig.lpszProxyBypass);
        }

        return _proxyType;
    }

    int Proxy::GetPacUrlProxy(std::wstring &pac)
    {
        pac = _pac;
        return 0;
    }

    int Proxy::GetServerProxy(std::wstring &server)
    {
        server = _server;
        return 0;
    }

    int Proxy::GetServerPass(std::wstring &pass)
    {
        pass = _serverPass;
        return 0;
    }

    int Proxy::GetProxyUserAndPassword(std::wstring &user, std::wstring &pwd)
    {
        return 0;
    }
}