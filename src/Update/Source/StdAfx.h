
#if !defined(AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_)
#define AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_

#pragma once

#define WIN32_LEAN_AND_MEAN	
#define _CRT_SECURE_NO_DEPRECATE

#include <windows.h>
#include <objbase.h>
#include <zmouse.h>

#include "UIlib.h"

using namespace DuiLib;

struct ServerCfg {
	int _port;
	int _appId;
	int _siteId;
	int _skinId;
	int _timeOut;
	std::wstring _upgradeServer;
	std::wstring _packageInfo;
	std::wstring _packageDownload;
	std::string _logPath;
	std::wstring _installPath;

	ServerCfg& operator=(const ServerCfg &cfg) {
		this->_port = cfg._port;
		this->_appId = cfg._appId;
		this->_siteId = cfg._siteId;
		this->_skinId = cfg._skinId;
		this->_timeOut = cfg._timeOut;
		this->_upgradeServer = cfg._upgradeServer;
		this->_packageInfo = cfg._packageInfo;
		this->_packageDownload = cfg._packageDownload;
		this->_logPath = cfg._logPath;
		this->_installPath = cfg._installPath;
		return *this;
	}
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_)
