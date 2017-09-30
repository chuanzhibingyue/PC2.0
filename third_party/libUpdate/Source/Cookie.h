/******************************************************************************
* Copyright (c) 2009~2010 All Rights Reserved by
*  G-Net Integrated Service co. Ltd. 

******************************************************************************/
/**
* @file Cookie.h 
* @brief 对Cookie进行操作
* 
* @author Wei Chen (wei.chen@gnetis.com)
*
* @date Jun 13 2010
*
* @version 1.0.0
* 
* @warning null
*
* Revision History
* @if  CR/PR ID    Author          Date            Major Change @endif
* @bug 

******************************************************************************/

#pragma once
#include <atlstr.h>
#include "FolderOperator.h"
// TangClient Cookie URL
#define COOKIE_TANG_CLIENT_URL		_T("http://www.sme.com")

enum TangInstallType
{
	TANG_UNINSTALL		= 0,
	TANG_INSTALL		= 1,
	TANG_QUERY_VERSION	= 2,
	TANG_DOWNLOAD		= 3,
	TANG_UNPACK			= 4,
	TANG_CLIENT_INSTALL	= 3,
	TANG_INSTALL_FINISH	= 4,
};

class ISqlite3Callback
{
public:
	virtual int callback(void *NotUsed, int argc, char **argv, char **azColName) = 0;
};

class CCookie //: public ISqlite3Callback
{
private:
	CString m_strCookieContext;
	CString m_strSiteURL;

	UINT	m_type;
	UINT	m_process;
public:

	CCookie();

	void SetType(TangInstallType type);

	BOOL Write(BOOL bEnableCookie = TRUE);

	HRESULT SetCookie(		
		__in LPCWSTR lpszUrl,
		__in_opt LPCWSTR lpszCookieName,
		__in LPCWSTR lpszCookieData
		);

	HRESULT RemoveCookie(		
		__in LPCWSTR lpszUrl,
		__in_opt LPCWSTR lpszCookieName
		);


	HRESULT firefoxSetCookie(    
		__in LPCWSTR lpszUrl,
		__in_opt LPCWSTR lpszCookieName,
		__in LPCWSTR lpszCookieData
	);
	HRESULT firefoxReadCookie( __in LPCWSTR lpszUrl, 
		__inout LPWSTR& lpszCookieName, 
		__inout LPWSTR& lpszCookieData );

	HRESULT firefoxRemoveCookie(    
		__in LPCWSTR lpszUrl,
		__in_opt LPCWSTR lpszCookieName,
		__in LPCWSTR lpszCookieData
		);

	HRESULT chromeSetCookie(    
		__in LPCWSTR lpszUrl,
		__in_opt LPCWSTR lpszCookieName,
		__in LPCWSTR lpszCookieData
		);
	HRESULT chromeReadCookie( __in LPCWSTR lpszUrl, 
		__inout LPWSTR& lpszCookieName, 
		__inout LPWSTR& lpszCookieData );

	HRESULT chromeRemoveCookie(    
		__in LPCWSTR lpszUrl,
		__in_opt LPCWSTR lpszCookieName,
		__in LPCWSTR lpszCookieData
		);


	HRESULT chrome360SetCookie(    
		__in LPCWSTR lpszUrl,
		__in_opt LPCWSTR lpszCookieName,
		__in LPCWSTR lpszCookieData
		);
	HRESULT chrome360ReadCookie( __in LPCWSTR lpszUrl, 
		__inout LPWSTR& lpszCookieName, 
		__inout LPWSTR& lpszCookieData );

	HRESULT chrome360RemoveCookie(    
		__in LPCWSTR lpszUrl,
		__in_opt LPCWSTR lpszCookieName,
		__in LPCWSTR lpszCookieData
		);

	HRESULT safariSetCookie(    
		__in LPCWSTR lpszUrl,
		__in_opt LPCWSTR lpszCookieName,
		__in LPCWSTR lpszCookieData
		);
	HRESULT safariReadCookie( __in LPCWSTR lpszUrl, 
		__inout LPWSTR& lpszCookieName, 
		__inout LPWSTR& lpszCookieData );

	HRESULT safariRemoveCookie(    
		__in LPCWSTR lpszUrl,
		__in_opt LPCWSTR lpszCookieName,
		__in LPCWSTR lpszCookieData
		);

	HRESULT GetFirefoxPath(CString& strPath);
	HRESULT GetFirefoxCookieHandle(HMODULE& hMozcrt19, HMODULE& hSqlite3);
	HRESULT GetFirefoxCookieName(CString& strCookieName);
	static int firefoxCallback(void *NotUsed, int argc, char **argv, char **azColName);

	HRESULT GetChromePath(CString& strPath);
	HRESULT GetChromeCookieHandle(HMODULE& hSqlite3);
	HRESULT GetChromeCookieName(CString& strCookieName);
	static int chromeCallback(void *NotUsed, int argc, char **argv, char **azColName);

	HRESULT Get360ChromePath(CString& strPath);
	HRESULT Get360ChromeCookieHandle(HMODULE& hSqlite3);
	HRESULT Get360ChromeCookieName(CString& strCookieName);
	static int chrome360Callback(void *NotUsed, int argc, char **argv, char **azColName);

	HRESULT GetSafariCookieName(CString& strCookieName);
	//HRESULT ParseUrl(const CString strInUrl, CString& strOutUrl);
private:
	CFolderOperator m_folder;
};