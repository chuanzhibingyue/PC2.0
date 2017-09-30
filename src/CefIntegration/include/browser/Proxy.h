/*
* Proxy.h
*
*  Created on: 2015.11.10
*      Author: he zhuo
*/

#ifndef PROXY_H_
#define PROXY_H_

#include <vector>
#include <string>

#include <objbase.h>
#include <zmouse.h>

namespace cefIntegration
{
    enum Proxy_Type
    {
        Proxy_Invalid = -1,
        Proxy_No,
        Proxy_AutoDetect,
        Proxy_Pac,
        Proxy_Server,
    };

class Proxy
{
public:
    Proxy();
    ~Proxy();

    int GetProxyType();
    int GetPacUrlProxy(std::wstring &pac);
    int GetServerProxy(std::wstring &server);
    int GetServerPass(std::wstring &pass);
    int GetProxyUserAndPassword(std::wstring &user, std::wstring &pwd);
private:
    int                 _proxyType;
    std::wstring        _pac;
    std::wstring        _server;
    std::wstring        _serverPass;
    std::wstring        _user;
    std::wstring        _pwd;

};

}

#endif //PROXY_H_