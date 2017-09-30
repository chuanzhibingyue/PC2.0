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
#include "stdafx.h"
#include "Cookie.h"
#include <WinInet.h>
#pragma comment(lib, "Wininet.lib")
#include <shlobj.h>
#include <Debug.h>

// 设置Cookie
#define COOKIE_TIMEVALID			_T("expires = Sat, 01-Jan-2050 00:00:00 GMT")
// 失效Cookie
#define COOKIE_TIMEOUT				_T("expires = Sat, 01-Jan-2000 00:00:00 GMT")

LPSTR lpszName = NULL;
LPSTR lpszValue = NULL;

CCookie::CCookie()
{
	lpszName = NULL;
	lpszValue = NULL;
	m_type = 0;
	m_process = 0;
	m_strSiteURL = COOKIE_TANG_CLIENT_URL;
}

void CCookie::SetType(TangInstallType type)
{
	m_type = type;
}
BOOL CCookie::Write(BOOL bEnableCookie)
{
	CString strCookieContext;
	if(bEnableCookie)
	{
		// 写Cookie
		strCookieContext.Format(_T("TangInstallType = %d, Process = %d; expires = Sat, 01-Jan-2050 00:00:00 GMT"),
			m_type, m_process);
	}
	else
	{
		// 设置Cookie失效
		strCookieContext.Format(_T("TangInstallType = 0, Process = 0; expires = Sat, 01-Jan-1900 00:00:00 GMT"));
	}
	return InternetSetCookie(m_strSiteURL, NULL, strCookieContext);
}

HRESULT CCookie::GetFirefoxCookieName(CString& strCookieName)
{
	HRESULT hr = S_OK;
	CRegKey reg;
	CString strFirefoxPath;
	CString strFirefoxProfiles;
	CString strValue = _T("");
	LONG nMaxChar = MAX_PATH;
	LONG nRet = ERROR_SUCCESS;
	CString strAppName;
	CString strKeyName;
	CString strProfiles = _T("Profiles");

	strCookieName = strValue;

	SHGetSpecialFolderPath(NULL, strFirefoxPath.GetBuffer(MAX_PATH), CSIDL_APPDATA, NULL);
	strFirefoxPath.ReleaseBuffer();

	strFirefoxPath = strFirefoxPath + _T("\\Mozilla\\Firefox"); 
	strFirefoxProfiles = strFirefoxPath + _T("\\profiles.ini");
	BOOL_CHECK(m_folder.IsFileExists(strFirefoxProfiles));

	strAppName = _T("Profile0");
	strKeyName = _T("Path");
	strValue = _T("");
	GetPrivateProfileString(strAppName, strKeyName, _T(""),
		strValue.GetBuffer(MAX_PATH), MAX_PATH, strFirefoxProfiles);
	strValue.ReleaseBuffer();
	BOOL_CHECK(!strValue.IsEmpty());

	strFirefoxPath += _T("\\");
	strFirefoxPath += strProfiles;
	strFirefoxPath += _T("\\");
	strFirefoxPath += strValue.Right(strValue.GetLength() - strProfiles.GetLength() - 1);
	strValue = strFirefoxPath + _T("\\cookies.sqlite");
	BOOL_CHECK(m_folder.IsFileExists(strValue));

	strCookieName = strValue;
exit:

	return hr;
}

// 获得FireFox安装路径
HRESULT CCookie::GetFirefoxPath(CString& strPath)
{
	HRESULT hr = S_OK;
	CRegKey reg;
	CString strFirefoxPath;
	CString strValue = _T("");
	ULONG nMaxChar = MAX_PATH;
	LONG nRet = ERROR_SUCCESS;

	strPath = strValue;

	strFirefoxPath = _T("SOFTWARE\\Mozilla\\Mozilla Firefox"); 
	nRet = reg.Open(HKEY_LOCAL_MACHINE,	strFirefoxPath, KEY_READ);
	BOOL_CHECK((ERROR_SUCCESS == nRet));

	// 解决234错误码
	nMaxChar = MAX_PATH;
	nRet = reg.QueryStringValue(_T("CurrentVersion"), strValue.GetBuffer(MAX_PATH), &nMaxChar);
	strValue.ReleaseBuffer();
	BOOL_CHECK((ERROR_SUCCESS == nRet));
	reg.Close();

	strPath.Format(_T("%s\\%s\\Main"), strFirefoxPath, strValue);
	nRet = reg.Open(HKEY_LOCAL_MACHINE,	strPath, KEY_READ);
	BOOL_CHECK((ERROR_SUCCESS == nRet));

	nMaxChar = MAX_PATH;
	nRet = reg.QueryStringValue(_T("Install Directory"), strValue.GetBuffer(MAX_PATH), &nMaxChar);
	strValue.ReleaseBuffer();
	BOOL_CHECK((ERROR_SUCCESS == nRet));

	strPath = strValue;
exit:
	reg.Close();

	return hr;
}

HRESULT CCookie::GetFirefoxCookieHandle(HMODULE& hMozcrt19, HMODULE& hSqlite3)
{
	HRESULT hr = S_OK;
	CString strMozcrt19;
	CString strSqlite3;

	// 获得Firefox安装路径
	CString strFirefoxPath;
	GetFirefoxPath(strFirefoxPath);
	BOOL_CHECK((!strFirefoxPath.IsEmpty()));

	// 如果安装了Firefox，则需要写Cookie
	// 取得Firefox目录下mozcrt19.dll、sqlite3.dll的指针
	strMozcrt19 = strFirefoxPath + _T("\\mozcrt19.dll");
	BOOL_CHECK(m_folder.IsFileExists(strMozcrt19));

	// Firfox 4.0 beta 7 为 mozsqlite3.dll
	// Firfox 3.5 sqlite3.dll
	strSqlite3 = strFirefoxPath + _T("\\sqlite3.dll");
	BOOL_CHECK(m_folder.IsFileExists(strSqlite3));

	hMozcrt19 = LoadLibrary(strMozcrt19);
	CHECK_IF_NULL(hMozcrt19);

	hSqlite3 = LoadLibrary(strSqlite3);
	CHECK_IF_NULL(hSqlite3);

exit:
	return hr;
}

int CCookie::firefoxCallback(void *NotUsed, int argc, char **argv, char **azColName)
{
	if (!strcmp(azColName[0], "name"))
	{
		size_t nSize = strlen(argv[0]);
		if (nSize)
		{
			lpszName = new CHAR[nSize + 1];
			strcpy(lpszName, argv[0]);
		}
	}
	
	if (!strcmp(azColName[1], "value"))
	{
		size_t nSize = strlen(argv[1]);
		if (nSize)
		{
			lpszValue = new CHAR[nSize + 1];
			strcpy(lpszValue, argv[1]);
		}
	}

	return 0;
}

HRESULT CCookie::firefoxSetCookie( __in LPCWSTR lpszUrl, 
								  __in_opt LPCWSTR lpszCookieName, 
								  __in LPCWSTR lpszCookieData )
{
	HRESULT hr = S_OK;
	HMODULE hMozcrt19 = NULL; 
	HMODULE hSqlite3 = NULL;
	const int FILE_BLOCK_SIZE = 1024;
	CString strCookieName;
	int nLen = 0;
	CString strSql;
	LPSTR pCookieBuffer = NULL;
	LPSTR pBuffer = NULL;

	// 为了保证数据的唯一性，在插入之前，先删除原有的历史记录
	firefoxRemoveCookie(lpszUrl, lpszCookieName, lpszCookieData);

	GetFirefoxCookieName(strCookieName);
	////LOG_INFO(L"CCookie::firefoxRemoveCookie strCookieName is %s", strCookieName);

	if (strCookieName.IsEmpty())
	{
		hr = E_FAIL;
		goto exit;
	}

	GetFirefoxCookieHandle(hMozcrt19, hSqlite3);
	if (NULL == hSqlite3)
	{
		hr = E_FAIL;
		goto exit;
	}

	// 打开Cookie.sqlite
	pCookieBuffer = new CHAR[FILE_BLOCK_SIZE];
	nLen = ::WideCharToMultiByte(CP_UTF8, 0, strCookieName, strCookieName.GetLength(), 
		(LPSTR)pCookieBuffer, FILE_BLOCK_SIZE, NULL, NULL);
	BOOL_CHECK((nLen > 0));
	pCookieBuffer[nLen] = '\0';

	typedef LRESULT (_cdecl *SQLITE3_OPEN)(const char *, void **);
	SQLITE3_OPEN sqlite3_open = NULL;
	sqlite3_open = (SQLITE3_OPEN)GetProcAddress((HMODULE)hSqlite3, ("sqlite3_open"));
	CHECK_IF_NULL(sqlite3_open);

	typedef int (*SQLITE3_EXEC)(void*, const char*, void*, void**, char**);
	SQLITE3_EXEC sqlite3_exec = NULL;
	sqlite3_exec = (SQLITE3_EXEC)GetProcAddress((HMODULE)hSqlite3, ("sqlite3_exec"));
	CHECK_IF_NULL(sqlite3_open);

	typedef int (*SQLITE3_CLOSE)(void*);
	SQLITE3_CLOSE sqlite3_close = NULL;
	sqlite3_close = (SQLITE3_CLOSE)GetProcAddress((HMODULE)hSqlite3, ("sqlite3_close"));
	CHECK_IF_NULL(sqlite3_open);

	typedef int (*SQLITE3_FREE)(void*);
	SQLITE3_FREE sqlite3_free = NULL;
	sqlite3_free = (SQLITE3_CLOSE)GetProcAddress((HMODULE)hSqlite3, ("sqlite3_free"));
	CHECK_IF_NULL(sqlite3_free);

	typedef int (*SQLITE3_CALLBACK)(void*,int,char**,char**);

	// 写入Cookie内容

/*
	// Firefox 4.0 beta 7 cookie结构
	strSql.Format(_T("INSERT INTO moz_cookies (baseDomain, name, value, host, path, expiry, lastAccessed, createionTime, isSecure, isHttpOnly)	\
				VALUES('%s', '%s', '%s', '%s', '/', \
				2236926980, 1290155780046000, 1290155780046000, 0, 0)"),
				lpszUrl, lpszCookieName, lpszCookieData, lpszUrl);
*/

	// Firefox 3.5 cookie结构
	strSql.Format(_T("INSERT INTO moz_cookies (name, value, host, path, expiry, lastAccessed, isSecure, isHttpOnly)	\
					 VALUES('%s', '%s', '%s', '/', \
					 2236926980, 1290155780046000, 0, 0)"),
					 lpszCookieName, lpszCookieData, lpszUrl, lpszUrl);
	////LOG_INFO(L"CCookie::firefoxRemoveCookie strSql is %s", strSql);

	// 由于Sqlite3全部使用UTF-8编码
	pBuffer = new CHAR[FILE_BLOCK_SIZE];
	nLen = ::WideCharToMultiByte(CP_UTF8, 0, strSql, strSql.GetLength(), 
		(LPSTR)pBuffer, FILE_BLOCK_SIZE, NULL, NULL);
	BOOL_CHECK((nLen > 0));
	pBuffer[nLen] = '\0';

	int nRet = 0;
	LPVOID pSqliteDb = NULL;
	nRet = sqlite3_open(pCookieBuffer, &pSqliteDb);
	CHECK_IF_NULL(pSqliteDb);
	if (nRet)
	{
		//LOG_INFO(L"CCookie::firefoxRemoveCookie sqlite3_open error is %d", nRet);
		hr = E_FAIL;
		sqlite3_close(pSqliteDb);
		HR_CHECK(hr);
	}

	char* zErrMsg = NULL;
	nRet = sqlite3_exec(pSqliteDb, pBuffer, NULL, 0, &zErrMsg);
	if (nRet)
	{
		hr = E_FAIL;
		sqlite3_free(zErrMsg);
	}

	sqlite3_close(pSqliteDb);

exit:
	SAFE_DELETE_ARRAY(pCookieBuffer);
	SAFE_DELETE_ARRAY(pBuffer);

	if (NULL != hSqlite3)
	{
		FreeLibrary(hSqlite3);
		hSqlite3 = NULL;
	}

	if (NULL != hMozcrt19)
	{
		FreeLibrary(hMozcrt19);
		hMozcrt19 = NULL;
	}
	return hr;
}

HRESULT CCookie::firefoxReadCookie( __in LPCWSTR lpszUrl, 
								  __inout LPWSTR& lpszCookieName, 
								  __inout LPWSTR& lpszCookieData )
{
	HRESULT hr = S_OK;
	HMODULE hMozcrt19 = NULL; 
	HMODULE hSqlite3 = NULL;
	const int FILE_BLOCK_SIZE = 1024;
	CString strCookieName;
	int nLen = 0;
	CString strSql;
	LPSTR pCookieBuffer = NULL;
	LPSTR pBuffer = NULL;

	lpszCookieName = NULL;
	lpszCookieData = NULL;

	GetFirefoxCookieName(strCookieName);
	if (strCookieName.IsEmpty())
	{
		hr = E_FAIL;
		goto exit;
	}

	GetFirefoxCookieHandle(hMozcrt19, hSqlite3);
	if (NULL == hSqlite3)
	{
		hr = E_FAIL;
		goto exit;
	}

	// 打开Cookie.sqlite
	pCookieBuffer = new CHAR[FILE_BLOCK_SIZE];
	nLen = ::WideCharToMultiByte(CP_UTF8, 0, strCookieName, strCookieName.GetLength(), 
		(LPSTR)pCookieBuffer, FILE_BLOCK_SIZE, NULL, NULL);
	BOOL_CHECK((nLen > 0));
	pCookieBuffer[nLen] = '\0';

	typedef LRESULT (_cdecl *SQLITE3_OPEN)(const char *, void **);
	SQLITE3_OPEN sqlite3_open = NULL;
	sqlite3_open = (SQLITE3_OPEN)GetProcAddress((HMODULE)hSqlite3, ("sqlite3_open"));
	CHECK_IF_NULL(sqlite3_open);

	typedef int (*SQLITE3_EXEC)(void*, const char*, void*, void**, char**);
	SQLITE3_EXEC sqlite3_exec = NULL;
	sqlite3_exec = (SQLITE3_EXEC)GetProcAddress((HMODULE)hSqlite3, ("sqlite3_exec"));
	CHECK_IF_NULL(sqlite3_open);

	typedef int (*SQLITE3_CLOSE)(void*);
	SQLITE3_CLOSE sqlite3_close = NULL;
	sqlite3_close = (SQLITE3_CLOSE)GetProcAddress((HMODULE)hSqlite3, ("sqlite3_close"));
	CHECK_IF_NULL(sqlite3_open);

	typedef int (*SQLITE3_FREE)(void*);
	SQLITE3_FREE sqlite3_free = NULL;
	sqlite3_free = (SQLITE3_CLOSE)GetProcAddress((HMODULE)hSqlite3, ("sqlite3_free"));
	CHECK_IF_NULL(sqlite3_free);

	typedef int (*SQLITE3_CALLBACK)(void*,int,char**,char**);

	// 读取Cookie内容
	strSql.Format(_T("SELECT name, value FROM moz_cookies WHERE host = '%s'"),
		lpszUrl);

	// 由于Sqlite3全部使用UTF-8编码
	pBuffer = new CHAR[FILE_BLOCK_SIZE];
	nLen = ::WideCharToMultiByte(CP_UTF8, 0, strSql, strSql.GetLength(), 
		(LPSTR)pBuffer, FILE_BLOCK_SIZE, NULL, NULL);
	BOOL_CHECK((nLen > 0));
	pBuffer[nLen] = '\0';

	int nRet = 0;
	LPVOID pSqliteDb = NULL;
	nRet = sqlite3_open(pCookieBuffer, &pSqliteDb);
	CHECK_IF_NULL(pSqliteDb);
	if (nRet)
	{
		//LOG_INFO(L"CCookie::firefoxRemoveCookie sqlite3_open error is %d", nRet);
		hr = E_FAIL;
		sqlite3_close(pSqliteDb);
		HR_CHECK(hr);
	}

	char* zErrMsg = NULL;
	nRet = sqlite3_exec(pSqliteDb, pBuffer, firefoxCallback, 0, &zErrMsg);
	if (nRet)
	{
		hr = E_FAIL;
		sqlite3_free(zErrMsg);
	}

	sqlite3_close(pSqliteDb);

	if ((NULL != lpszName) && strlen(lpszName))
	{
		// char* 转换为 wchar_t*
		lpszCookieName = new WCHAR[FILE_BLOCK_SIZE];
		nLen = ::MultiByteToWideChar(CP_ACP, 0, lpszName, (int)strlen(lpszName), 
			lpszCookieName, FILE_BLOCK_SIZE);
		BOOL_CHECK((nLen > 0));
		lpszCookieName[nLen] = '\0';
	}

	if ((NULL != lpszValue) && strlen(lpszValue))
	{
		lpszCookieData = new WCHAR[FILE_BLOCK_SIZE];
		nLen = ::MultiByteToWideChar(CP_ACP, 0, lpszValue, (int)strlen(lpszValue), 
			lpszCookieData, FILE_BLOCK_SIZE);
		BOOL_CHECK((nLen > 0));
		lpszCookieData[nLen] = '\0';
	}

exit:
	SAFE_DELETE_ARRAY(pCookieBuffer);
	SAFE_DELETE_ARRAY(pBuffer);

	SAFE_DELETE_ARRAY(lpszName);
	SAFE_DELETE_ARRAY(lpszValue);

	if (NULL != hSqlite3)
	{
		FreeLibrary(hSqlite3);
		hSqlite3 = NULL;
	}

	if (NULL != hMozcrt19)
	{
		FreeLibrary(hMozcrt19);
		hMozcrt19 = NULL;
	}
	return hr;
}

HRESULT CCookie::firefoxRemoveCookie( __in LPCWSTR lpszUrl, 
									 __in_opt LPCWSTR lpszCookieName, 
									 __in LPCWSTR lpszCookieData )
{
	HRESULT hr = S_OK;
	HMODULE hMozcrt19 = NULL; 
	HMODULE hSqlite3 = NULL;
	const int FILE_BLOCK_SIZE = 1024;
	CString strCookieName;
	int nLen = 0;
	CString strSql;
	LPSTR pCookieBuffer = NULL;
	LPSTR pBuffer = NULL;

	GetFirefoxCookieName(strCookieName);
	//LOG_INFO(L"CCookie::firefoxRemoveCookie strCookieName is %s", strCookieName);

	if (strCookieName.IsEmpty())
	{
		hr = E_FAIL;
		goto exit;
	}

	GetFirefoxCookieHandle(hMozcrt19, hSqlite3);
	if (NULL == hSqlite3)
	{
		hr = E_FAIL;
		goto exit;
	}

	// 打开Cookie.sqlite
	pCookieBuffer = new CHAR[FILE_BLOCK_SIZE];
	nLen = ::WideCharToMultiByte(CP_UTF8, 0, strCookieName, strCookieName.GetLength(), 
		(LPSTR)pCookieBuffer, FILE_BLOCK_SIZE, NULL, NULL);
	BOOL_CHECK((nLen > 0));
	pCookieBuffer[nLen] = '\0';

	typedef LRESULT (_cdecl *SQLITE3_OPEN)(const char *, void **);
	SQLITE3_OPEN sqlite3_open = NULL;
	sqlite3_open = (SQLITE3_OPEN)GetProcAddress((HMODULE)hSqlite3, ("sqlite3_open"));
	CHECK_IF_NULL(sqlite3_open);

	typedef int (*SQLITE3_EXEC)(void*, const char*, void*, void**, char**);
	SQLITE3_EXEC sqlite3_exec = NULL;
	sqlite3_exec = (SQLITE3_EXEC)GetProcAddress((HMODULE)hSqlite3, ("sqlite3_exec"));
	CHECK_IF_NULL(sqlite3_open);

	typedef int (*SQLITE3_CLOSE)(void*);
	SQLITE3_CLOSE sqlite3_close = NULL;
	sqlite3_close = (SQLITE3_CLOSE)GetProcAddress((HMODULE)hSqlite3, ("sqlite3_close"));
	CHECK_IF_NULL(sqlite3_open);

	typedef int (*SQLITE3_FREE)(void*);
	SQLITE3_FREE sqlite3_free = NULL;
	sqlite3_free = (SQLITE3_CLOSE)GetProcAddress((HMODULE)hSqlite3, ("sqlite3_free"));
	CHECK_IF_NULL(sqlite3_free);

	typedef int (*SQLITE3_CALLBACK)(void*,int,char**,char**);

	// 写入Cookie内容
	strSql.Format(_T("DELETE FROM moz_cookies WHERE host = '%s'"), lpszUrl);
	//LOG_INFO(L"CCookie::firefoxRemoveCookie strSql is %s", strSql);

	// 由于Sqlite3全部使用UTF-8编码
	pBuffer = new CHAR[FILE_BLOCK_SIZE];
	nLen = ::WideCharToMultiByte(CP_UTF8, 0, strSql, strSql.GetLength(), 
		(LPSTR)pBuffer, FILE_BLOCK_SIZE, NULL, NULL);
	BOOL_CHECK((nLen > 0));
	pBuffer[nLen] = '\0';

	int nRet = 0;
	LPVOID pSqliteDb = NULL;
	nRet = sqlite3_open(pCookieBuffer, &pSqliteDb);
	CHECK_IF_NULL(pSqliteDb);
	if (nRet)
	{
		hr = E_FAIL;
		sqlite3_close(pSqliteDb);
		HR_CHECK(hr);
	}

	char* zErrMsg = NULL;
	nRet = sqlite3_exec(pSqliteDb, pBuffer, NULL, 0, &zErrMsg);
	if (nRet)
	{
		hr = E_FAIL;
		sqlite3_free(zErrMsg);
	}

	sqlite3_close(pSqliteDb);

exit:
	SAFE_DELETE_ARRAY(pCookieBuffer);
	SAFE_DELETE_ARRAY(pBuffer);

	if (NULL != hSqlite3)
	{
		FreeLibrary(hSqlite3);
		hSqlite3 = NULL;
	}

	if (NULL != hMozcrt19)
	{
		FreeLibrary(hMozcrt19);
		hMozcrt19 = NULL;
	}
	return hr;
}

HRESULT CCookie::GetSafariCookieName(CString& strCookieName)
{
	HRESULT hr = S_OK;
	CString strSafariPath;
	CString strValue = _T("");

	strCookieName = strValue;

	SHGetSpecialFolderPath(NULL, strSafariPath.GetBuffer(MAX_PATH), CSIDL_APPDATA, NULL);
	strSafariPath.ReleaseBuffer();

	strValue = strSafariPath + _T("\\Apple Computer\\Safari\\Cookies\\Cookies.plist"); 
	BOOL_CHECK(m_folder.IsFileExists(strValue));

	strCookieName = strValue;
exit:

	return hr;
}
HRESULT CCookie::safariSetCookie( __in LPCWSTR lpszUrl, 
								 __in_opt LPCWSTR lpszCookieName, 
								 __in LPCWSTR lpszCookieData )
{
	// 使用XML结构来保存Cookie
	// ANSI结构
	// C:\Documents and Settings\wei.chen.IBM-001.000\
	// Application Data\Apple Computer\Safari\Cookies\Cookies.plist
/*
	<dict>
		<key>Created</key>
		<real>311677313.8125</real>
		<key>Domain</key>
		<string>192.168.11.20</string>
		<key>Expires</key>
		<date>2010-12-17T09:01:02Z</date>
		<key>Name</key>
		<string>loginform:username</string>
		<key>Path</key>
		<string>/ehour</string>
		<key>Value</key>
		<string>wei.chen</string>
	</dict>
*/

	HRESULT hr = S_OK;
	const int FILE_BLOCK_SIZE = 1024;
	CString strCookieName;
	int nLen = 0;
	LPSTR pCookieBuffer = NULL;
	LPSTR pBuffer = NULL;

	// 使用MSXML来解析Cookie列表
	VARIANT variant;
	IXMLDOMDocument * pRecorderXML = NULL;
	VARIANT_BOOL bSuccess = VARIANT_TRUE;
	IXMLDOMNode* pNode = NULL;
	IXMLDOMNodeList* pNodeList = NULL;
	CComPtr<IXMLDOMNamedNodeMap> pDOMNamedNodeMap = NULL;
	CComPtr<IXMLDOMNode> pNamedItemNode = NULL;
	CComVariant varAttributeValue;
	CComVariant varNodeValue;
	CComPtr<IXMLDOMNodeList> pChildLst = NULL;
	CComPtr<IXMLDOMNode> pNextNode = NULL;
	CComPtr<IXMLDOMNodeList> pKeyChildLst = NULL;
	CComPtr<IXMLDOMNode> pKeyNode = NULL;
	CComPtr<IXMLDOMElement> pDictNode = NULL;
	CComPtr<IXMLDOMElement> pElementNode = NULL;
	CComPtr<IXMLDOMText>	pKeyText = NULL;
	BSTR nodeValue;
	BSTR nodeName;
	CString strNodeName;
	CString strDomainName = _T("Domain");
	CString strDomainValue;
	BOOL bFind = FALSE;
	BOOL bNameModify = FALSE;
	BOOL bValueModify = FALSE;

	GetSafariCookieName(strCookieName);
	if (strCookieName.IsEmpty())
	{
		//LOG_INFO(L"CCookie::safariSetCookie the safari cookie %s is corrupt!", strCookieName);
		goto exit;
	}

	// 读取Cookies.plist，以dict形式，查找是否有指定的字段
	
	// 如果有，则修改它

	// 如果没有查找到，则新建它

	::CoInitialize(NULL);
	hr =::CoCreateInstance(
		CLSID_DOMDocument, 
		NULL, 
		CLSCTX_INPROC_SERVER,
		IID_IXMLDOMDocument,
		(LPVOID*)(&pRecorderXML) );
	HR_CHECK(hr);
	ARG_CHECK(pRecorderXML);

	pRecorderXML->put_async(VARIANT_FALSE);

	variant.vt = VT_BSTR;
	variant.bstrVal = SysAllocString(strCookieName);
	hr = pRecorderXML->load(variant, &bSuccess);
	SysFreeString(variant.bstrVal);
	if (FAILED(hr))
	{
		//LOG_ERR(L"%s wase opened.", strCookieName);
	}
	HR_CHECK(hr);
	DWORD dw = GetLastError();
	BOOL_CHECK((bSuccess == VARIANT_TRUE));

	// 取dict列表
	hr = pRecorderXML->getElementsByTagName(_T("dict"), &pChildLst);
	if (SUCCEEDED(hr) && (NULL != pChildLst))
	{

		long len = 0;
		hr = pChildLst->get_length(&len);
		HR_CHECK(hr);

		// 在dict里面，寻找Domain
		for (long i = 0;i<len;i++)
		{
			pNextNode = NULL;
			hr = pChildLst->nextNode(&pNextNode);
			HR_CHECK(hr);
			CHECK_IF_NULL(pChildLst);

			hr = pNextNode->get_nodeName(&nodeName);
			HR_CHECK(hr);

			// 取子节点列表
			pKeyChildLst = NULL;
			hr = pNextNode->get_childNodes(&pKeyChildLst);
			HR_CHECK(hr);
			CHECK_IF_NULL(pKeyChildLst);

			long lenKey = 0;
			hr = pKeyChildLst->get_length(&lenKey);
			HR_CHECK(hr);

			// 循环dict子节点
			for (long j = 0; j < lenKey;)
			{
				pKeyNode = NULL;
				hr = pKeyChildLst->nextNode(&pKeyNode);
				HR_CHECK(hr);
				CHECK_IF_NULL(pKeyNode);
				j++;

				hr = pKeyNode->get_nodeName(&nodeName);
				HR_CHECK(hr);

				hr = pKeyNode->get_text(&nodeValue);
				HR_CHECK(hr);
				
				strNodeName = nodeValue;

				// 寻找的domain
				if (strNodeName != strDomainName)
				{
					continue;
				}

				// 读取Domain所对应的String
				pKeyNode = NULL;
				hr = pKeyChildLst->nextNode(&pKeyNode);
				HR_CHECK(hr);
				CHECK_IF_NULL(pKeyNode);
				j++;

				hr = pKeyNode->get_text(&nodeValue);
				HR_CHECK(hr);

				strDomainValue = nodeValue;

				if (strDomainValue != lpszUrl)
				{
					// 不是需要的Domain，寻找下一个dict
					break;
				}

				bFind = FALSE;
				// 读取剩下的节点
				for (; j < lenKey; )
				{
					pKeyNode = NULL;
					hr = pKeyChildLst->nextNode(&pKeyNode);
					if (FAILED(hr) || NULL == pKeyNode)
					{
						break;
					}
					j++;

					hr = pKeyNode->get_text(&nodeValue);
					HR_CHECK(hr);

					strDomainValue = nodeValue;

					if (strDomainValue == _T("Name"))
					{
						// 找到Value节点，将新值替换旧值
						pKeyNode = NULL;
						hr = pKeyChildLst->nextNode(&pKeyNode);
						HR_CHECK(hr);
						CHECK_IF_NULL(pKeyNode);
						j++;

						hr = pKeyNode->get_text(&nodeValue);
						HR_CHECK(hr);

						strNodeName = nodeValue;

						if (strNodeName == lpszCookieName)
						{
							// 已经找到，则修改value节点
							bFind = TRUE;
						}
					}

					if (bFind && (strDomainValue == _T("Value")))
					{
						// 找到Value节点，将新值替换旧值
						pKeyNode = NULL;
						hr = pKeyChildLst->nextNode(&pKeyNode);
						HR_CHECK(hr);
						CHECK_IF_NULL(pKeyNode);
						j++;

						nodeValue = ::SysAllocString(lpszCookieData);
						hr = pKeyNode->put_text(nodeValue);
						::SysFreeString(nodeValue);

						bValueModify = TRUE;
					}

					if (bNameModify && bValueModify)
					{
						break;
					}
				}

				if (bFind)
				{
					break;
				}
			}
		}
	}


	if (!bFind)
	{
		// 如果没有找到指定节点，则新建节点
		hr = pRecorderXML->selectSingleNode(_T("//plist/array"), &pNode);
		HR_CHECK(hr);
		CHECK_IF_NULL(pNode);
		
		pDictNode = NULL;
		hr = pRecorderXML->createElement(_T("dict"), &pDictNode);
		HR_CHECK(hr);
		CHECK_IF_NULL(pDictNode);

		pNode->appendChild(pDictNode, NULL);

		// <key>Created</key>
		pElementNode = NULL;
		hr = pRecorderXML->createElement(_T("key"), &pElementNode);
		HR_CHECK(hr);
		CHECK_IF_NULL(pElementNode);

		pElementNode->put_text(_T("Created"));

		pDictNode->appendChild(pElementNode, NULL);

		// <real>312294136.953125</real>
		pElementNode = NULL;
		hr = pRecorderXML->createElement(_T("real"), &pElementNode);
		HR_CHECK(hr);
		CHECK_IF_NULL(pElementNode);

		pElementNode->put_text(_T("312294136.953125"));

		pDictNode->appendChild(pElementNode, NULL);

		// <key>Domain</key>
		pElementNode = NULL;
		hr = pRecorderXML->createElement(_T("key"), &pElementNode);
		HR_CHECK(hr);
		CHECK_IF_NULL(pElementNode);

		pElementNode->put_text(_T("Domain"));

		pDictNode->appendChild(pElementNode, NULL);

		// <string>.baidu.com</string>
		pElementNode = NULL;
		hr = pRecorderXML->createElement(_T("string"), &pElementNode);
		HR_CHECK(hr);
		CHECK_IF_NULL(pElementNode);

		nodeValue = ::SysAllocString(lpszUrl);
		pElementNode->put_text(nodeValue);
		::SysFreeString(nodeValue);

		pDictNode->appendChild(pElementNode, NULL);

		// <key>Expires</key>
		pElementNode = NULL;
		hr = pRecorderXML->createElement(_T("key"), &pElementNode);
		HR_CHECK(hr);
		CHECK_IF_NULL(pElementNode);

		pElementNode->put_text(_T("Expires"));

		pDictNode->appendChild(pElementNode, NULL);

		// <date>2040-11-24T12:22:18Z</date>
		pElementNode = NULL;
		hr = pRecorderXML->createElement(_T("date"), &pElementNode);
		HR_CHECK(hr);
		CHECK_IF_NULL(pElementNode);

		pElementNode->put_text(_T("2040-11-24T12:22:18Z"));

		pDictNode->appendChild(pElementNode, NULL);

		// <key>Name</key>
		pElementNode = NULL;
		hr = pRecorderXML->createElement(_T("key"), &pElementNode);
		HR_CHECK(hr);
		CHECK_IF_NULL(pElementNode);

		pElementNode->put_text(_T("Name"));

		pDictNode->appendChild(pElementNode, NULL);

		// <string>BAIDUID</string>
		pElementNode = NULL;
		hr = pRecorderXML->createElement(_T("string"), &pElementNode);
		HR_CHECK(hr);
		CHECK_IF_NULL(pElementNode);

		pKeyText = NULL;
		nodeValue = ::SysAllocString(lpszCookieName);
		pElementNode->put_text(nodeValue);
		::SysFreeString(nodeValue);

		pDictNode->appendChild(pElementNode, NULL);

		// <key>Path</key>
		pElementNode = NULL;
		hr = pRecorderXML->createElement(_T("key"), &pElementNode);
		HR_CHECK(hr);
		CHECK_IF_NULL(pElementNode);

		pElementNode->put_text(_T("Path"));

		pDictNode->appendChild(pElementNode, NULL);

		// <string>/</string>
		pElementNode = NULL;
		hr = pRecorderXML->createElement(_T("string"), &pElementNode);
		HR_CHECK(hr);
		CHECK_IF_NULL(pElementNode);

		pElementNode->put_text(_T("/"));

		pDictNode->appendChild(pElementNode, NULL);

		// <key>Value</key>
		pElementNode = NULL;
		hr = pRecorderXML->createElement(_T("key"), &pElementNode);
		HR_CHECK(hr);
		CHECK_IF_NULL(pElementNode);

		pElementNode->put_text(_T("Value"));

		pDictNode->appendChild(pElementNode, NULL);

		// <string>F4ED511AFD87597A0312E7339FC5F620:FG=1;chen_chen_test</string>
		pElementNode = NULL;
		hr = pRecorderXML->createElement(_T("string"), &pElementNode);
		HR_CHECK(hr);
		CHECK_IF_NULL(pElementNode);

		pKeyText = NULL;
		nodeValue = ::SysAllocString(lpszCookieData);
		pElementNode->put_text(nodeValue);
		::SysFreeString(nodeValue);

		pDictNode->appendChild(pElementNode, NULL);
	}
	hr = pRecorderXML->save(variant);
	HR_CHECK(hr);

exit:
	if (NULL != pRecorderXML)
	{
		pRecorderXML->Release();
	}
	return hr;
}

HRESULT CCookie::safariReadCookie( __in LPCWSTR lpszUrl, 
								  __inout LPWSTR& lpszCookieName, 
								  __inout LPWSTR& lpszCookieData )
{
	HRESULT hr = S_OK;

	// 上层不处理Safari Cookie内容
	HR_CHECK(hr);
exit:
	return hr;
}

HRESULT CCookie::safariRemoveCookie( __in LPCWSTR lpszUrl, 
									__in_opt LPCWSTR lpszCookieName, 
									__in LPCWSTR lpszCookieData )
{
	HRESULT hr = S_OK;
	const int FILE_BLOCK_SIZE = 1024;
	CString strCookieName;
	int nLen = 0;
	LPSTR pCookieBuffer = NULL;
	LPSTR pBuffer = NULL;

	// 使用MSXML来解析Cookie列表
	VARIANT variant;
	IXMLDOMDocument * pRecorderXML = NULL;
	VARIANT_BOOL bSuccess = VARIANT_TRUE;
	IXMLDOMNode* pNode = NULL;
	IXMLDOMNodeList* pNodeList = NULL;
	CComPtr<IXMLDOMNamedNodeMap> pDOMNamedNodeMap = NULL;
	CComPtr<IXMLDOMNode> pNamedItemNode = NULL;
	CComVariant varAttributeValue;
	CComVariant varNodeValue;
	CComPtr<IXMLDOMNodeList> pChildLst = NULL;
	CComPtr<IXMLDOMNode> pNextNode = NULL;
	CComPtr<IXMLDOMNodeList> pKeyChildLst = NULL;
	CComPtr<IXMLDOMNode> pKeyNode = NULL;
	CComPtr<IXMLDOMElement> pDictNode = NULL;
	CComPtr<IXMLDOMElement> pElementNode = NULL;
	CComPtr<IXMLDOMText>	pKeyText = NULL;
	BSTR nodeValue;
	BSTR nodeName;
	CString strNodeName;
	CString strDomainName = _T("Domain");
	CString strDomainValue;
	BOOL bFind = FALSE;
	BOOL bNameModify = FALSE;
	BOOL bValueModify = FALSE;

	GetSafariCookieName(strCookieName);
	BOOL_CHECK((!strCookieName.IsEmpty()));

	// 读取Cookies.plist，以dict形式，查找是否有指定的字段

	// 如果有，则修改它

	// 如果没有查找到，则新建它

	::CoInitialize(NULL);
	hr =::CoCreateInstance(
		CLSID_DOMDocument, 
		NULL, 
		CLSCTX_INPROC_SERVER,
		IID_IXMLDOMDocument,
		(LPVOID*)(&pRecorderXML) );
	HR_CHECK(hr);
	ARG_CHECK(pRecorderXML);

	pRecorderXML->put_async(VARIANT_FALSE);

	variant.vt = VT_BSTR;
	variant.bstrVal = SysAllocString(strCookieName);
	hr = pRecorderXML->load(variant, &bSuccess);
	SysFreeString(variant.bstrVal);
	if (FAILED(hr))
	{
		//LOG_ERR(L"%s wase opened.", strCookieName);
	}
	HR_CHECK(hr);
	DWORD dw = GetLastError();
	BOOL_CHECK((bSuccess == VARIANT_TRUE));

	// 取dict列表
	hr = pRecorderXML->getElementsByTagName(_T("dict"), &pChildLst);
	if (SUCCEEDED(hr) && (NULL != pChildLst))
	{

		long len = 0;
		hr = pChildLst->get_length(&len);
		HR_CHECK(hr);

		// 在dict里面，寻找Domain
		for (long i = 0;i<len;i++)
		{
			pNextNode = NULL;
			hr = pChildLst->nextNode(&pNextNode);
			HR_CHECK(hr);
			CHECK_IF_NULL(pChildLst);

			hr = pNextNode->get_nodeName(&nodeName);
			HR_CHECK(hr);

			// 取子节点列表
			pKeyChildLst = NULL;
			hr = pNextNode->get_childNodes(&pKeyChildLst);
			HR_CHECK(hr);
			CHECK_IF_NULL(pKeyChildLst);

			long lenKey = 0;
			hr = pKeyChildLst->get_length(&lenKey);
			HR_CHECK(hr);

			// 循环dict子节点
			for (long j = 0; j < lenKey;)
			{
				pKeyNode = NULL;
				hr = pKeyChildLst->nextNode(&pKeyNode);
				HR_CHECK(hr);
				CHECK_IF_NULL(pKeyNode);
				j++;

				hr = pKeyNode->get_nodeName(&nodeName);
				HR_CHECK(hr);

				hr = pKeyNode->get_text(&nodeValue);
				HR_CHECK(hr);

				strNodeName = nodeValue;

				if (strNodeName != strDomainName)
				{
					continue;
				}

				// 读取Domain所对应的String
				pKeyNode = NULL;
				hr = pKeyChildLst->nextNode(&pKeyNode);
				HR_CHECK(hr);
				CHECK_IF_NULL(pKeyNode);
				j++;

				hr = pKeyNode->get_text(&nodeValue);
				HR_CHECK(hr);

				strDomainValue = nodeValue;

				if (strDomainValue != lpszUrl)
				{
					// 不是需要的Domain，寻找下一个dict
					break;
				}

				bFind = FALSE;
				// 读取剩下的节点
				for (; j < lenKey; )
				{
					pKeyNode = NULL;
					hr = pKeyChildLst->nextNode(&pKeyNode);
					if (FAILED(hr) || NULL == pKeyNode)
					{
						break;
					}
					j++;

					hr = pKeyNode->get_text(&nodeValue);
					HR_CHECK(hr);

					strDomainValue = nodeValue;

					if (strDomainValue == _T("Name"))
					{
						// 找到了Name节点
						pKeyNode = NULL;
						hr = pKeyChildLst->nextNode(&pKeyNode);
						HR_CHECK(hr);
						CHECK_IF_NULL(pKeyNode);
						j++;

						hr = pKeyNode->get_text(&nodeValue);
						HR_CHECK(hr);

						strNodeName = nodeValue;

						if (strNodeName == lpszCookieName)
						{
							// 已经找到
							bFind = TRUE;
						}
						break;
					}
				}

				if (bFind)
				{
					break;
				}
			}
			if (bFind)
			{
				break;
			}
		}

		if (bFind)
		{
			// 删除此节点
			pNode = NULL;
			hr = pRecorderXML->selectSingleNode(_T("//plist/array"), &pNode);
			HR_CHECK(hr);
			CHECK_IF_NULL(pNode);

			pKeyNode = NULL;
			hr = pNode->removeChild(pNextNode, &pKeyNode);
			HR_CHECK(hr);

			hr = pRecorderXML->save(variant);
			HR_CHECK(hr);
		}
	}
	HR_CHECK(hr);
exit:
	return hr;
}

HRESULT CCookie::GetChromeCookieName(CString& strCookieName)
{
	HRESULT hr = S_OK;
	CString strChromePath;
	CString strValue = _T("");

	strCookieName = strValue;

	// Local Settings\Application Data\Google\Chrome\User Data\Default
	SHGetSpecialFolderPath(NULL, strChromePath.GetBuffer(MAX_PATH), CSIDL_LOCAL_APPDATA, NULL);
	strChromePath.ReleaseBuffer();

	strValue = strChromePath + _T("\\Google\\Chrome\\User Data\\Default\\Cookies"); 
	BOOL_CHECK(m_folder.IsFileExists(strValue));

	strCookieName = strValue;
exit:

	return hr;
}

// 由于Chrome不带sqlite3.dll，因此，需要附加带上，放在Update模块的同一级目录
HRESULT CCookie::GetChromeCookieHandle(HMODULE& hSqlite3)
{
	HRESULT hr = S_OK;
	CString strFileName;
	CString strSqlite3;

	// 获得Chrome安装路径
	GetModuleFileName(NULL, strFileName.GetBuffer(MAX_PATH), _MAX_PATH);
	strFileName.ReleaseBuffer();
	strFileName = strFileName.Left(strFileName.ReverseFind('\\'));

	// 如果安装了Chrome，则需要写Cookie
	// 取得Chrome目录下sqlite3.dll的指针
	strSqlite3 = strFileName + _T("\\sqlite3.dll");
	BOOL_CHECK(m_folder.IsFileExists(strSqlite3));

	hSqlite3 = LoadLibrary(strSqlite3);
	CHECK_IF_NULL(hSqlite3);

exit:
	return hr;
}

int CCookie::chromeCallback(void *NotUsed, int argc, char **argv, char **azColName)
{
	if (!strcmp(azColName[0], "name"))
	{
		size_t nSize = strlen(argv[0]);
		if (nSize)
		{
			lpszName = new CHAR[nSize + 1];
			strcpy(lpszName, argv[0]);
		}
	}

	if (!strcmp(azColName[1], "value"))
	{
		size_t nSize = strlen(argv[1]);
		if (nSize)
		{
			lpszValue = new CHAR[nSize + 1];
			strcpy(lpszValue, argv[1]);
		}
	}

	return 0;
}

HRESULT CCookie::chromeSetCookie( __in LPCWSTR lpszUrl, 
								 __in_opt LPCWSTR lpszCookieName, 
								 __in LPCWSTR lpszCookieData )
{
	HRESULT hr = S_OK;

	CString strSql;
	
	// Chrome 浏览器，直接在Chrome.exe中封装了Sqlite3数据库的读写，使用Sqlite3.dll来读写
	HMODULE hSqlite3 = NULL;
	const int FILE_BLOCK_SIZE = 1024;
	CString strCookieName;
	int nLen = 0;
	LPSTR pCookieBuffer = NULL;
	LPSTR pBuffer = NULL;

	// 为了保证数据的唯一性，在插入之前，先删除原有的历史记录
	chromeRemoveCookie(lpszUrl, lpszCookieName, lpszCookieData);

	GetChromeCookieName(strCookieName);
	if (strCookieName.IsEmpty())
	{
		hr = E_FAIL;
		goto exit;
	}

	GetChromeCookieHandle(hSqlite3);
	if (NULL == hSqlite3)
	{
		hr = E_FAIL;
		goto exit;
	}

	// 打开Cookie
	pCookieBuffer = new CHAR[FILE_BLOCK_SIZE];
	nLen = ::WideCharToMultiByte(CP_UTF8, 0, strCookieName, strCookieName.GetLength(), 
		(LPSTR)pCookieBuffer, FILE_BLOCK_SIZE, NULL, NULL);
	BOOL_CHECK((nLen > 0));

	typedef LRESULT (_cdecl *SQLITE3_OPEN)(const char *, void **);
	SQLITE3_OPEN sqlite3_open = NULL;
	sqlite3_open = (SQLITE3_OPEN)GetProcAddress((HMODULE)hSqlite3, ("sqlite3_open"));
	CHECK_IF_NULL(sqlite3_open);

	typedef int (*SQLITE3_EXEC)(void*, const char*, void*, void**, char**);
	SQLITE3_EXEC sqlite3_exec = NULL;
	sqlite3_exec = (SQLITE3_EXEC)GetProcAddress((HMODULE)hSqlite3, ("sqlite3_exec"));
	CHECK_IF_NULL(sqlite3_open);

	typedef int (*SQLITE3_CLOSE)(void*);
	SQLITE3_CLOSE sqlite3_close = NULL;
	sqlite3_close = (SQLITE3_CLOSE)GetProcAddress((HMODULE)hSqlite3, ("sqlite3_close"));
	CHECK_IF_NULL(sqlite3_open);

	typedef int (*SQLITE3_FREE)(void*);
	SQLITE3_FREE sqlite3_free = NULL;
	sqlite3_free = (SQLITE3_CLOSE)GetProcAddress((HMODULE)hSqlite3, ("sqlite3_free"));
	CHECK_IF_NULL(sqlite3_free);

	typedef int (*SQLITE3_CALLBACK)(void*,int,char**,char**);

	// 写入Cookie内容
	strSql.Format(_T("INSERT INTO cookies(host_key, name, value, path, expires_utc, secure, httponly, last_access_utc)	 \
		VALUES('%s', '%s', \
		'%s', '/', 12965996825000000, 0, 0, 12934457368468125)"),
					 lpszUrl, lpszCookieName, lpszCookieData);

	// 由于Sqlite3全部使用UTF-8编码
	pBuffer = new CHAR[FILE_BLOCK_SIZE];
	nLen = ::WideCharToMultiByte(CP_UTF8, 0, strSql, strSql.GetLength(), 
		(LPSTR)pBuffer, FILE_BLOCK_SIZE, NULL, NULL);
	BOOL_CHECK((nLen > 0));
	pBuffer[nLen] = '\0';

	int nRet = 0;
	LPVOID pSqliteDb = NULL;
	nRet = sqlite3_open(pCookieBuffer, &pSqliteDb);
	CHECK_IF_NULL(pSqliteDb);
	if (nRet)
	{
		hr = E_FAIL;
		sqlite3_close(pSqliteDb);
		HR_CHECK(hr);
	}

	char* zErrMsg = NULL;
	nRet = sqlite3_exec(pSqliteDb, pBuffer, NULL, 0, &zErrMsg);
	if (nRet)
	{
		hr = E_FAIL;
		sqlite3_free(zErrMsg);
	}

	sqlite3_close(pSqliteDb);

exit:
	SAFE_DELETE_ARRAY(pCookieBuffer);
	SAFE_DELETE_ARRAY(pBuffer);

	if (NULL != hSqlite3)
	{
		FreeLibrary(hSqlite3);
		hSqlite3 = NULL;
	}
	return hr;
}

HRESULT CCookie::chromeReadCookie( __in LPCWSTR lpszUrl, 
								  __inout LPWSTR& lpszCookieName, 
								  __inout LPWSTR& lpszCookieData )
{
	HRESULT hr = S_OK;

	CString strSql;

	// Chrome 浏览器，直接在Chrome.exe中封装了Sqlite3数据库的读写，使用Sqlite3.dll来读写
	HMODULE hSqlite3 = NULL;
	const int FILE_BLOCK_SIZE = 1024;
	CString strCookieName;
	int nLen = 0;
	LPSTR pCookieBuffer = NULL;
	LPSTR pBuffer = NULL;

	GetChromeCookieName(strCookieName);
	if (strCookieName.IsEmpty())
	{
		hr = E_FAIL;
		goto exit;
	}

	GetChromeCookieHandle(hSqlite3);
	if (NULL == hSqlite3)
	{
		hr = E_FAIL;
		goto exit;
	}

	// 打开Cookie
	pCookieBuffer = new CHAR[FILE_BLOCK_SIZE];
	nLen = ::WideCharToMultiByte(CP_UTF8, 0, strCookieName, strCookieName.GetLength(), 
		(LPSTR)pCookieBuffer, FILE_BLOCK_SIZE, NULL, NULL);
	BOOL_CHECK((nLen > 0));

	typedef LRESULT (_cdecl *SQLITE3_OPEN)(const char *, void **);
	SQLITE3_OPEN sqlite3_open = NULL;
	sqlite3_open = (SQLITE3_OPEN)GetProcAddress((HMODULE)hSqlite3, ("sqlite3_open"));
	CHECK_IF_NULL(sqlite3_open);

	typedef int (*SQLITE3_EXEC)(void*, const char*, void*, void**, char**);
	SQLITE3_EXEC sqlite3_exec = NULL;
	sqlite3_exec = (SQLITE3_EXEC)GetProcAddress((HMODULE)hSqlite3, ("sqlite3_exec"));
	CHECK_IF_NULL(sqlite3_open);

	typedef int (*SQLITE3_CLOSE)(void*);
	SQLITE3_CLOSE sqlite3_close = NULL;
	sqlite3_close = (SQLITE3_CLOSE)GetProcAddress((HMODULE)hSqlite3, ("sqlite3_close"));
	CHECK_IF_NULL(sqlite3_open);

	typedef int (*SQLITE3_FREE)(void*);
	SQLITE3_FREE sqlite3_free = NULL;
	sqlite3_free = (SQLITE3_CLOSE)GetProcAddress((HMODULE)hSqlite3, ("sqlite3_free"));
	CHECK_IF_NULL(sqlite3_free);

	typedef int (*SQLITE3_CALLBACK)(void*,int,char**,char**);

	// 读取Cookie内容
	strSql.Format(_T("SELECT name, value FROM cookies WHERE host_key = '%s'"),
		lpszUrl);

	// 由于Sqlite3全部使用UTF-8编码
	pBuffer = new CHAR[FILE_BLOCK_SIZE];
	nLen = ::WideCharToMultiByte(CP_UTF8, 0, strSql, strSql.GetLength(), 
		(LPSTR)pBuffer, FILE_BLOCK_SIZE, NULL, NULL);
	BOOL_CHECK((nLen > 0));
	pBuffer[nLen] = '\0';

	int nRet = 0;
	LPVOID pSqliteDb = NULL;
	nRet = sqlite3_open(pCookieBuffer, &pSqliteDb);
	CHECK_IF_NULL(pSqliteDb);
	if (nRet)
	{
		hr = E_FAIL;
		sqlite3_close(pSqliteDb);
		HR_CHECK(hr);
	}

	char* zErrMsg = NULL;
	nRet = sqlite3_exec(pSqliteDb, pBuffer, chromeCallback, 0, &zErrMsg);
	if (nRet)
	{
		hr = E_FAIL;
		sqlite3_free(zErrMsg);
	}

	sqlite3_close(pSqliteDb);

	if ((NULL != lpszName) && strlen(lpszName))
	{
		// char* 转换为 wchar_t*
		lpszCookieName = new WCHAR[FILE_BLOCK_SIZE];
		nLen = ::MultiByteToWideChar(CP_ACP, 0, lpszName, (int)strlen(lpszName), 
			lpszCookieName, FILE_BLOCK_SIZE);
		BOOL_CHECK((nLen > 0));
		lpszCookieName[nLen] = '\0';
	}

	if ((NULL != lpszValue) && strlen(lpszValue))
	{
		lpszCookieData = new WCHAR[FILE_BLOCK_SIZE];
		nLen = ::MultiByteToWideChar(CP_ACP, 0, lpszValue, (int)strlen(lpszValue), 
			lpszCookieData, FILE_BLOCK_SIZE);
		BOOL_CHECK((nLen > 0));
		lpszCookieData[nLen] = '\0';
	}

exit:
	SAFE_DELETE_ARRAY(pCookieBuffer);
	SAFE_DELETE_ARRAY(pBuffer);

	SAFE_DELETE_ARRAY(lpszName);
	SAFE_DELETE_ARRAY(lpszValue);

	if (NULL != hSqlite3)
	{
		FreeLibrary(hSqlite3);
		hSqlite3 = NULL;
	}
	return hr;
}

HRESULT CCookie::chromeRemoveCookie( __in LPCWSTR lpszUrl, 
								 __in_opt LPCWSTR lpszCookieName, 
								 __in LPCWSTR lpszCookieData )
{
	HRESULT hr = S_OK;

	CString strSql;

	// Chrome 浏览器，直接在Chrome.exe中封装了Sqlite3数据库的读写，使用Sqlite3.dll来读写
	HMODULE hSqlite3 = NULL;
	const int FILE_BLOCK_SIZE = 1024;
	CString strCookieName;
	int nLen = 0;
	LPSTR pCookieBuffer = NULL;
	LPSTR pBuffer = NULL;

	GetChromeCookieName(strCookieName);
	if (strCookieName.IsEmpty())
	{
		hr = E_FAIL;
		goto exit;
	}

	GetChromeCookieHandle(hSqlite3);
	if (NULL == hSqlite3)
	{
		hr = E_FAIL;
		goto exit;
	}

	// 打开Cookie
	pCookieBuffer = new CHAR[FILE_BLOCK_SIZE];
	nLen = ::WideCharToMultiByte(CP_UTF8, 0, strCookieName, strCookieName.GetLength(), 
		(LPSTR)pCookieBuffer, FILE_BLOCK_SIZE, NULL, NULL);
	BOOL_CHECK((nLen > 0));

	typedef LRESULT (_cdecl *SQLITE3_OPEN)(const char *, void **);
	SQLITE3_OPEN sqlite3_open = NULL;
	sqlite3_open = (SQLITE3_OPEN)GetProcAddress((HMODULE)hSqlite3, ("sqlite3_open"));
	CHECK_IF_NULL(sqlite3_open);

	typedef int (*SQLITE3_EXEC)(void*, const char*, void*, void**, char**);
	SQLITE3_EXEC sqlite3_exec = NULL;
	sqlite3_exec = (SQLITE3_EXEC)GetProcAddress((HMODULE)hSqlite3, ("sqlite3_exec"));
	CHECK_IF_NULL(sqlite3_open);

	typedef int (*SQLITE3_CLOSE)(void*);
	SQLITE3_CLOSE sqlite3_close = NULL;
	sqlite3_close = (SQLITE3_CLOSE)GetProcAddress((HMODULE)hSqlite3, ("sqlite3_close"));
	CHECK_IF_NULL(sqlite3_open);

	typedef int (*SQLITE3_FREE)(void*);
	SQLITE3_FREE sqlite3_free = NULL;
	sqlite3_free = (SQLITE3_CLOSE)GetProcAddress((HMODULE)hSqlite3, ("sqlite3_free"));
	CHECK_IF_NULL(sqlite3_free);

	typedef int (*SQLITE3_CALLBACK)(void*,int,char**,char**);

	// 写入Cookie内容
	strSql.Format(_T("DELETE FROM cookies WHERE host_key = '%s'"), lpszUrl);

	// 由于Sqlite3全部使用UTF-8编码
	pBuffer = new CHAR[FILE_BLOCK_SIZE];
	nLen = ::WideCharToMultiByte(CP_UTF8, 0, strSql, strSql.GetLength(), 
		(LPSTR)pBuffer, FILE_BLOCK_SIZE, NULL, NULL);
	BOOL_CHECK((nLen > 0));
	pBuffer[nLen] = '\0';

	int nRet = 0;
	LPVOID pSqliteDb = NULL;
	nRet = sqlite3_open(pCookieBuffer, &pSqliteDb);
	CHECK_IF_NULL(pSqliteDb);
	if (nRet)
	{
		hr = E_FAIL;
		sqlite3_close(pSqliteDb);
		HR_CHECK(hr);
	}

	char* zErrMsg = NULL;
	nRet = sqlite3_exec(pSqliteDb, pBuffer, NULL, 0, &zErrMsg);
	if (nRet)
	{
		hr = E_FAIL;
		sqlite3_free(zErrMsg);
	}

	sqlite3_close(pSqliteDb);

exit:
	SAFE_DELETE_ARRAY(pCookieBuffer);
	SAFE_DELETE_ARRAY(pBuffer);

	if (NULL != hSqlite3)
	{
		FreeLibrary(hSqlite3);
		hSqlite3 = NULL;
	}
	return hr;
}

HRESULT CCookie::Get360ChromeCookieName(CString& strCookieName)
{
	HRESULT hr = S_OK;
	CString strChromePath;
	CString strValue = _T("");

	strCookieName = strValue;

	// Local Settings\Application Data\Google\Chrome\User Data\Default
	SHGetSpecialFolderPath(NULL, strChromePath.GetBuffer(MAX_PATH), CSIDL_LOCAL_APPDATA, NULL);
	strChromePath.ReleaseBuffer();

	strValue = strChromePath + _T("\\360Chrome\\Chrome\\User Data\\Default\\Cookies"); 
	BOOL_CHECK(m_folder.IsFileExists(strValue));

	strCookieName = strValue;
exit:

	return hr;
}

// 由于Chrome不带sqlite3.dll，因此，需要附加带上，放在Update模块的同一级目录
HRESULT CCookie::Get360ChromeCookieHandle(HMODULE& hSqlite3)
{
	HRESULT hr = S_OK;
	CString strFileName;
	CString strSqlite3;

	// 获得Chrome安装路径
	GetModuleFileName(NULL, strFileName.GetBuffer(MAX_PATH), _MAX_PATH);
	strFileName.ReleaseBuffer();
	strFileName = strFileName.Left(strFileName.ReverseFind('\\'));

	// 如果安装了Chrome，则需要写Cookie
	// 取得Chrome目录下sqlite3.dll的指针
	strSqlite3 = strFileName + _T("\\sqlite3.dll");
	BOOL_CHECK(m_folder.IsFileExists(strSqlite3));

	hSqlite3 = LoadLibrary(strSqlite3);
	CHECK_IF_NULL(hSqlite3);

exit:
	return hr;
}

int CCookie::chrome360Callback(void *NotUsed, int argc, char **argv, char **azColName)
{
	if (!strcmp(azColName[0], "name"))
	{
		size_t nSize = strlen(argv[0]);
		if (nSize)
		{
			lpszName = new CHAR[nSize + 1];
			strcpy(lpszName, argv[0]);
		}
	}

	if (!strcmp(azColName[1], "value"))
	{
		size_t nSize = strlen(argv[1]);
		if (nSize)
		{
			lpszValue = new CHAR[nSize + 1];
			strcpy(lpszValue, argv[1]);
		}
	}

	return 0;
}

HRESULT CCookie::chrome360SetCookie( __in LPCWSTR lpszUrl, 
								 __in_opt LPCWSTR lpszCookieName, 
								 __in LPCWSTR lpszCookieData )
{
	HRESULT hr = S_OK;

	CString strSql;

	// Chrome 浏览器，直接在Chrome.exe中封装了Sqlite3数据库的读写，使用Sqlite3.dll来读写
	HMODULE hSqlite3 = NULL;
	const int FILE_BLOCK_SIZE = 1024;
	CString strCookieName;
	int nLen = 0;
	LPSTR pCookieBuffer = NULL;
	LPSTR pBuffer = NULL;

	// 为了保证数据的唯一性，在插入之前，先删除原有的历史记录
	chrome360RemoveCookie(lpszUrl, lpszCookieName, lpszCookieData);

	Get360ChromeCookieName(strCookieName);
	if (strCookieName.IsEmpty())
	{
		hr = E_FAIL;
		goto exit;
	}

	Get360ChromeCookieHandle(hSqlite3);
	if (NULL == hSqlite3)
	{
		hr = E_FAIL;
		goto exit;
	}

	// 打开Cookie
	pCookieBuffer = new CHAR[FILE_BLOCK_SIZE];
	nLen = ::WideCharToMultiByte(CP_UTF8, 0, strCookieName, strCookieName.GetLength(), 
		(LPSTR)pCookieBuffer, FILE_BLOCK_SIZE, NULL, NULL);
	BOOL_CHECK((nLen > 0));

	typedef LRESULT (_cdecl *SQLITE3_OPEN)(const char *, void **);
	SQLITE3_OPEN sqlite3_open = NULL;
	sqlite3_open = (SQLITE3_OPEN)GetProcAddress((HMODULE)hSqlite3, ("sqlite3_open"));
	CHECK_IF_NULL(sqlite3_open);

	typedef int (*SQLITE3_EXEC)(void*, const char*, void*, void**, char**);
	SQLITE3_EXEC sqlite3_exec = NULL;
	sqlite3_exec = (SQLITE3_EXEC)GetProcAddress((HMODULE)hSqlite3, ("sqlite3_exec"));
	CHECK_IF_NULL(sqlite3_open);

	typedef int (*SQLITE3_CLOSE)(void*);
	SQLITE3_CLOSE sqlite3_close = NULL;
	sqlite3_close = (SQLITE3_CLOSE)GetProcAddress((HMODULE)hSqlite3, ("sqlite3_close"));
	CHECK_IF_NULL(sqlite3_open);

	typedef int (*SQLITE3_FREE)(void*);
	SQLITE3_FREE sqlite3_free = NULL;
	sqlite3_free = (SQLITE3_CLOSE)GetProcAddress((HMODULE)hSqlite3, ("sqlite3_free"));
	CHECK_IF_NULL(sqlite3_free);

	typedef int (*SQLITE3_CALLBACK)(void*,int,char**,char**);

	// 写入Cookie内容
	strSql.Format(_T("INSERT INTO cookies(host_key, name, value, path, expires_utc, secure, httponly, last_access_utc)	 \
					 VALUES('%s', '%s', \
					 '%s', '/', 12965996825000000, 0, 0, 12934457368468125)"),
					 lpszUrl, lpszCookieName, lpszCookieData);

	// 由于Sqlite3全部使用UTF-8编码
	pBuffer = new CHAR[FILE_BLOCK_SIZE];
	nLen = ::WideCharToMultiByte(CP_UTF8, 0, strSql, strSql.GetLength(), 
		(LPSTR)pBuffer, FILE_BLOCK_SIZE, NULL, NULL);
	BOOL_CHECK((nLen > 0));
	pBuffer[nLen] = '\0';

	int nRet = 0;
	LPVOID pSqliteDb = NULL;
	nRet = sqlite3_open(pCookieBuffer, &pSqliteDb);
	CHECK_IF_NULL(pSqliteDb);
	if (nRet)
	{
		hr = E_FAIL;
		sqlite3_close(pSqliteDb);
		HR_CHECK(hr);
	}

	char* zErrMsg = NULL;
	nRet = sqlite3_exec(pSqliteDb, pBuffer, NULL, 0, &zErrMsg);
	if (nRet)
	{
		hr = E_FAIL;
		sqlite3_free(zErrMsg);
	}

	sqlite3_close(pSqliteDb);

exit:
	SAFE_DELETE_ARRAY(pCookieBuffer);
	SAFE_DELETE_ARRAY(pBuffer);

	if (NULL != hSqlite3)
	{
		FreeLibrary(hSqlite3);
		hSqlite3 = NULL;
	}
	return hr;
}

HRESULT CCookie::chrome360ReadCookie( __in LPCWSTR lpszUrl, 
								  __inout LPWSTR& lpszCookieName, 
								  __inout LPWSTR& lpszCookieData )
{
	HRESULT hr = S_OK;

	CString strSql;

	// Chrome 浏览器，直接在Chrome.exe中封装了Sqlite3数据库的读写，使用Sqlite3.dll来读写
	HMODULE hSqlite3 = NULL;
	const int FILE_BLOCK_SIZE = 1024;
	CString strCookieName;
	int nLen = 0;
	LPSTR pCookieBuffer = NULL;
	LPSTR pBuffer = NULL;

	Get360ChromeCookieName(strCookieName);
	if (strCookieName.IsEmpty())
	{
		hr = E_FAIL;
		goto exit;
	}

	Get360ChromeCookieHandle(hSqlite3);
	if (NULL == hSqlite3)
	{
		hr = E_FAIL;
		goto exit;
	}

	// 打开Cookie
	pCookieBuffer = new CHAR[FILE_BLOCK_SIZE];
	nLen = ::WideCharToMultiByte(CP_UTF8, 0, strCookieName, strCookieName.GetLength(), 
		(LPSTR)pCookieBuffer, FILE_BLOCK_SIZE, NULL, NULL);
	BOOL_CHECK((nLen > 0));

	typedef LRESULT (_cdecl *SQLITE3_OPEN)(const char *, void **);
	SQLITE3_OPEN sqlite3_open = NULL;
	sqlite3_open = (SQLITE3_OPEN)GetProcAddress((HMODULE)hSqlite3, ("sqlite3_open"));
	CHECK_IF_NULL(sqlite3_open);

	typedef int (*SQLITE3_EXEC)(void*, const char*, void*, void**, char**);
	SQLITE3_EXEC sqlite3_exec = NULL;
	sqlite3_exec = (SQLITE3_EXEC)GetProcAddress((HMODULE)hSqlite3, ("sqlite3_exec"));
	CHECK_IF_NULL(sqlite3_open);

	typedef int (*SQLITE3_CLOSE)(void*);
	SQLITE3_CLOSE sqlite3_close = NULL;
	sqlite3_close = (SQLITE3_CLOSE)GetProcAddress((HMODULE)hSqlite3, ("sqlite3_close"));
	CHECK_IF_NULL(sqlite3_open);

	typedef int (*SQLITE3_FREE)(void*);
	SQLITE3_FREE sqlite3_free = NULL;
	sqlite3_free = (SQLITE3_CLOSE)GetProcAddress((HMODULE)hSqlite3, ("sqlite3_free"));
	CHECK_IF_NULL(sqlite3_free);

	typedef int (*SQLITE3_CALLBACK)(void*,int,char**,char**);

	// 读取Cookie内容
	strSql.Format(_T("SELECT name, value FROM cookies WHERE host_key = '%s'"),
		lpszUrl);

	// 由于Sqlite3全部使用UTF-8编码
	pBuffer = new CHAR[FILE_BLOCK_SIZE];
	nLen = ::WideCharToMultiByte(CP_UTF8, 0, strSql, strSql.GetLength(), 
		(LPSTR)pBuffer, FILE_BLOCK_SIZE, NULL, NULL);
	BOOL_CHECK((nLen > 0));
	pBuffer[nLen] = '\0';

	int nRet = 0;
	LPVOID pSqliteDb = NULL;
	nRet = sqlite3_open(pCookieBuffer, &pSqliteDb);
	CHECK_IF_NULL(pSqliteDb);
	if (nRet)
	{
		hr = E_FAIL;
		sqlite3_close(pSqliteDb);
		HR_CHECK(hr);
	}

	char* zErrMsg = NULL;
	nRet = sqlite3_exec(pSqliteDb, pBuffer, chrome360Callback, 0, &zErrMsg);
	if (nRet)
	{
		hr = E_FAIL;
		sqlite3_free(zErrMsg);
	}

	sqlite3_close(pSqliteDb);

	if ((NULL != lpszName) && strlen(lpszName))
	{
		// char* 转换为 wchar_t*
		lpszCookieName = new WCHAR[FILE_BLOCK_SIZE];
		nLen = ::MultiByteToWideChar(CP_ACP, 0, lpszName, (int)strlen(lpszName), 
			lpszCookieName, FILE_BLOCK_SIZE);
		BOOL_CHECK((nLen > 0));
		lpszCookieName[nLen] = '\0';
	}

	if ((NULL != lpszValue) && strlen(lpszValue))
	{
		lpszCookieData = new WCHAR[FILE_BLOCK_SIZE];
		nLen = ::MultiByteToWideChar(CP_ACP, 0, lpszValue, (int)strlen(lpszValue), 
			lpszCookieData, FILE_BLOCK_SIZE);
		BOOL_CHECK((nLen > 0));
		lpszCookieData[nLen] = '\0';
	}

exit:
	SAFE_DELETE_ARRAY(pCookieBuffer);
	SAFE_DELETE_ARRAY(pBuffer);

	SAFE_DELETE_ARRAY(lpszName);
	SAFE_DELETE_ARRAY(lpszValue);

	if (NULL != hSqlite3)
	{
		FreeLibrary(hSqlite3);
		hSqlite3 = NULL;
	}
	return hr;
}

HRESULT CCookie::chrome360RemoveCookie( __in LPCWSTR lpszUrl, 
									__in_opt LPCWSTR lpszCookieName, 
									__in LPCWSTR lpszCookieData )
{
	HRESULT hr = S_OK;

	CString strSql;

	// Chrome 浏览器，直接在Chrome.exe中封装了Sqlite3数据库的读写，使用Sqlite3.dll来读写
	HMODULE hSqlite3 = NULL;
	const int FILE_BLOCK_SIZE = 1024;
	CString strCookieName;
	int nLen = 0;
	LPSTR pCookieBuffer = NULL;
	LPSTR pBuffer = NULL;

	Get360ChromeCookieName(strCookieName);
	if (strCookieName.IsEmpty())
	{
		hr = E_FAIL;
		goto exit;
	}

	Get360ChromeCookieHandle(hSqlite3);
	if (NULL == hSqlite3)
	{
		hr = E_FAIL;
		goto exit;
	}

	// 打开Cookie
	pCookieBuffer = new CHAR[FILE_BLOCK_SIZE];
	nLen = ::WideCharToMultiByte(CP_UTF8, 0, strCookieName, strCookieName.GetLength(), 
		(LPSTR)pCookieBuffer, FILE_BLOCK_SIZE, NULL, NULL);
	BOOL_CHECK((nLen > 0));

	typedef LRESULT (_cdecl *SQLITE3_OPEN)(const char *, void **);
	SQLITE3_OPEN sqlite3_open = NULL;
	sqlite3_open = (SQLITE3_OPEN)GetProcAddress((HMODULE)hSqlite3, ("sqlite3_open"));
	CHECK_IF_NULL(sqlite3_open);

	typedef int (*SQLITE3_EXEC)(void*, const char*, void*, void**, char**);
	SQLITE3_EXEC sqlite3_exec = NULL;
	sqlite3_exec = (SQLITE3_EXEC)GetProcAddress((HMODULE)hSqlite3, ("sqlite3_exec"));
	CHECK_IF_NULL(sqlite3_open);

	typedef int (*SQLITE3_CLOSE)(void*);
	SQLITE3_CLOSE sqlite3_close = NULL;
	sqlite3_close = (SQLITE3_CLOSE)GetProcAddress((HMODULE)hSqlite3, ("sqlite3_close"));
	CHECK_IF_NULL(sqlite3_open);

	typedef int (*SQLITE3_FREE)(void*);
	SQLITE3_FREE sqlite3_free = NULL;
	sqlite3_free = (SQLITE3_CLOSE)GetProcAddress((HMODULE)hSqlite3, ("sqlite3_free"));
	CHECK_IF_NULL(sqlite3_free);

	typedef int (*SQLITE3_CALLBACK)(void*,int,char**,char**);

	// 写入Cookie内容
	strSql.Format(_T("DELETE FROM cookies WHERE host_key = '%s'"), lpszUrl);

	// 由于Sqlite3全部使用UTF-8编码
	pBuffer = new CHAR[FILE_BLOCK_SIZE];
	nLen = ::WideCharToMultiByte(CP_UTF8, 0, strSql, strSql.GetLength(), 
		(LPSTR)pBuffer, FILE_BLOCK_SIZE, NULL, NULL);
	BOOL_CHECK((nLen > 0));
	pBuffer[nLen] = '\0';

	int nRet = 0;
	LPVOID pSqliteDb = NULL;
	nRet = sqlite3_open(pCookieBuffer, &pSqliteDb);
	CHECK_IF_NULL(pSqliteDb);
	if (nRet)
	{
		hr = E_FAIL;
		sqlite3_close(pSqliteDb);
		HR_CHECK(hr);
	}

	char* zErrMsg = NULL;
	nRet = sqlite3_exec(pSqliteDb, pBuffer, NULL, 0, &zErrMsg);
	if (nRet)
	{
		hr = E_FAIL;
		sqlite3_free(zErrMsg);
	}

	sqlite3_close(pSqliteDb);

exit:
	SAFE_DELETE_ARRAY(pCookieBuffer);
	SAFE_DELETE_ARRAY(pBuffer);

	if (NULL != hSqlite3)
	{
		FreeLibrary(hSqlite3);
		hSqlite3 = NULL;
	}
	return hr;
}

HRESULT CCookie::SetCookie( __in LPCWSTR lpszUrl, __in_opt LPCWSTR lpszCookieName, __in LPCWSTR lpszCookieData )
{
	//LOG_INFO(L"CCookie::SetCookie Url = [%s] , Name = [%s] , Data = [%s]", lpszUrl, lpszCookieName, lpszCookieData);
	HRESULT hr = S_OK;
	BOOL	bUpdate = FALSE;
	CString strUrlName;
	int nPos = -1;
	CString strHttp;
	DWORD dw = 0;
	CString strContext;
	strContext = lpszCookieData;
	strContext += _T("; ");
	strContext += COOKIE_TIMEVALID;

	// 写入IE Cookie，客户端不需要考虑跨域问题
	
	// 由上层提供完整的URL链接，例如 http://www.sme.com
//	strHttp = _T("http://");
	strHttp += lpszUrl;
	//LOG_INFO(L"CCookie::SetCookie [InternetSetCookie] Url = [%s] , Name = [%s] , Data = [%s]", strHttp.GetBuffer(), lpszCookieName, strContext.GetBuffer());
	strHttp.ReleaseBuffer();
	strContext.ReleaseBuffer();
	BOOL bWrite = InternetSetCookie(strHttp, lpszCookieName, (LPCTSTR)strContext);
	if (!bWrite)
	{
		dw = GetLastError();
		//LOG_INFO(L"CCookie::SetCookie IE strHttp is '%s', GetLastError is %d", strHttp, dw);
	}

	// 写入其他浏览器的Cookie，Experies由各自浏览器决定
	strContext = lpszCookieData;

	// Chrome为sme.com
	strUrlName = lpszUrl;
	strHttp = _T("www.");
	nPos = strUrlName.Find(strHttp);
	if (-1 != nPos)
	{
		strUrlName = strUrlName.Mid(nPos + strHttp.GetLength(), 
			strUrlName.GetLength() - nPos - strHttp.GetLength());
	}
	//LOG_INFO(L"CCookie::SetCookie chromeSetCookie...");
	// 写入Chrome Cookie
	chromeSetCookie((LPCTSTR)strUrlName, lpszCookieName, (LPCTSTR)strContext);

	//LOG_INFO(L"CCookie::SetCookie chrome360SetCookie...");
	// 写入360Chrome Cookie
	chrome360SetCookie((LPCTSTR)strUrlName, lpszCookieName, (LPCTSTR)strContext);

	// Firefox不带http://www，为.sme.com
	strUrlName = lpszUrl;
	strHttp = _T("www");
	nPos = strUrlName.Find(strHttp);
	if (-1 != nPos)
	{
		strUrlName = strUrlName.Mid(nPos + strHttp.GetLength(), 
			strUrlName.GetLength() - nPos - strHttp.GetLength());
	}

	//LOG_INFO(L"CCookie::SetCookie firefoxSetCookie...");
	// 写入FireFox Cookie
	firefoxSetCookie((LPCTSTR)strUrlName, lpszCookieName, (LPCTSTR)strContext);

	// Safari和Firefox一样，不带http://www.
	// 写入Safari Cookie
//	safariSetCookie((LPCTSTR)strUrlName, lpszCookieName, (LPCTSTR)strContext);

	HR_CHECK(hr);

exit:
	if (FAILED(hr))
	{
		//LOG_ERR(L"UpdateManager::UpdateCookie exit with error 0x%08X", hr);
	}
	return hr;
}

HRESULT CCookie::RemoveCookie( __in LPCWSTR lpszUrl, __in_opt LPCWSTR lpszCookieName)
{
	//LOG_INFO(L"CCookie::RemoveCookie Url = [%s] , Name = [%s]", lpszUrl, lpszCookieName);
	HRESULT hr = S_OK;
	BOOL	bUpdate = FALSE;
	CString strUrlName;
	int nPos = -1;
	CString strHttp;
	DWORD dw = 0;
	CString strContext;
	strContext = _T("");
	strContext += _T("; ");
	strContext += _T("path=/; ");
	strContext += COOKIE_TIMEOUT;

	// 直接删除Cookie内容
	//strUrlName = _T("www.");
	strHttp = lpszUrl;
	//nPos = strHttp.Find(strUrlName);
	//strHttp = strHttp.Right(strHttp.GetLength() - strUrlName.GetLength());

	//strHttp = _T("http://") + strHttp;
	//LOG_INFO(L"CCookie::RemoveCookie [InternetSetCookie] Url = [%s] , Name = [%s] , Data = [%s]", strHttp.GetBuffer(), lpszCookieName, strContext.GetBuffer());
	strHttp.ReleaseBuffer();
	strContext.ReleaseBuffer();
	BOOL bWrite = InternetSetCookie(strHttp, lpszCookieName, (LPCTSTR)strContext);
	if (!bWrite)
	{
		dw = GetLastError();
	}

	// 写入其他浏览器的Cookie，Experies由各自浏览器决定
	strContext = _T("");

	// Firefox不带http://www，为.sme.com
	strUrlName = lpszUrl;
	strHttp = _T("www");
	nPos = strUrlName.Find(strHttp);
	if (-1 != nPos)
	{
		strUrlName = strUrlName.Mid(nPos + strHttp.GetLength(), 
			strUrlName.GetLength() - nPos - strHttp.GetLength());
	}
	//LOG_INFO(L"CCookie::RemoveCookie firefoxSetCookie...");
	// 删除FireFox Cookie
	firefoxRemoveCookie(strUrlName, lpszCookieName, strContext);

	// Chrome为sme.com
	strUrlName = lpszUrl;
	strHttp = _T("www.");
	nPos = strUrlName.Find(strHttp);
	if (-1 != nPos)
	{
		strUrlName = strUrlName.Mid(nPos + strHttp.GetLength(), 
			strUrlName.GetLength() - nPos - strHttp.GetLength());
	}
	//LOG_INFO(L"CCookie::SetCookie chromeSetCookie...");

	//LOG_INFO(L"CCookie::RemoveCookie chromeRemoveCookie...");
	// 删除Chrome Cookie
	chromeRemoveCookie((LPCTSTR)strUrlName, lpszCookieName, strContext);

	//LOG_INFO(L"CCookie::SetCookie chrome360RemoveCookie...");
	// 删除360Chrome Cookie
	chrome360RemoveCookie((LPCTSTR)strUrlName, lpszCookieName, (LPCTSTR)strContext);

	// 删除Safari Cookie
	safariRemoveCookie(lpszUrl, lpszCookieName, strContext);

	HR_CHECK(hr);
exit:
	if (FAILED(hr))
	{
		//LOG_ERR(L"UpdateManager::RemoveCookie exit with error 0x%08X", hr);
	}
	return hr;

}

/**
 * 解析输入的URL，并补充为完整的URL
 * @param [in] strInUrl: the input url
		  [in][out] strOutUrl: parse and return url
 * @return HRESULT: return S_OK if successful.
 * @rule
	完整的URL形式: 协议名://主机:端口号/路径?参数名=参数值&参数名=参数值
	端口号为80时可不填
	如：http://www.tang.com:8080/chk?a=1&s=2&sk=3
	当前函数功能，只是补充“协议名://主机”，从路径起始，后面都是由程序自动补充完成
 * @pre 
 * @post 
 * @par 
 * @code

 * @endcode
 * @since 
 * @see 
 */
// HRESULT CCookie::ParseUrl(const CString strInUrl, CString& strOutUrl)
// {
// 	HRESULT hr = S_OK;
// 	CAtlRegExp<> reUrl;
// 	CAtlREMatchContext<> mcUrl;
// 	UINT nGroupIndex = 0;
// 	const CAtlREMatchContext<>::RECHAR* szStart = 0;
// 	const CAtlREMatchContext<>::RECHAR* szEnd = 0;
// 	ptrdiff_t nLength = 0;
// 	CString str;
// 
// 	strOutUrl = _T("");
// 
// 	// 输入 http://search.microsoft.com/
// 	// 以()为分界标志，共分成5组
// 	// 第1组：{[^:/?#]+}:，^非，遇到:/?#则停止。输入http://时，取出http
// 	REParseError status = 
// 		reUrl.Parse(_T("({[^:/?#]+}://)?({[^:/?#]*})?"));
// 	if (REPARSE_ERROR_OK != status)
// 	{
// 		hr = E_INVALIDARG;
// 	}
// 	HR_CHECK(hr);
// 
// 	if (!reUrl.Match( strInUrl,	&mcUrl))
// 	{
// 		hr = E_INVALIDARG;
// 	}
// 	HR_CHECK(hr);
// 
// 	// domain
// 	if (mcUrl.m_uNumGroups >= 2)
// 	{
// 		nGroupIndex = 1;
// 		mcUrl.GetMatch(nGroupIndex, &szStart, &szEnd);
// 		nLength = szEnd - szStart;
// 
// 		if (nLength > 0)
// 		{
// 			str = szStart;
// 			str = str.Left((int)nLength);
// 
// 			// 保证为search.microsoft.com
// 			strOutUrl += str;
// 		}
// 	}
// 
// 	//LOG_INFO(L"UploadManager::ParseUrl strInUrl = '%s', strOutUrl = '%s'.",
// 		strInUrl, strOutUrl);
// 
// exit:
// 	if (FAILED(hr))
// 	{
// 		//LOG_ERR(L"CCookie::ParseUrl exit with error 0x%08X", hr);
// 	}
// 	return hr;
//}
