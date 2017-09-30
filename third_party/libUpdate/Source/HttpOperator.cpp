#include "stdafx.h"
#include "HttpOperator.h"
#include <Debug.h>
#include <shlobj.h>
#include <MsXml2.h>
//#include <atlrx.h>
#include "commdefines.h"
#include "LogManager.h"
#include "common\StringConvert.h"

#include "pugixml.hpp"

#undef MODULE_NAME
#undef FILE_NAME

#ifdef _DEBUG
#define MODULE_NAME     (L"Update_Debug")
#define FILE_NAME       (L"HttpOperator.cpp")
#else
#define MODULE_NAME     (L"Update")
#define FILE_NAME       (L"HttpOperator.cpp")
#endif // _DEBUG

#define MAXBLOCKSIZE	(1024)

// 升级文件URL
#define XTangFilePath		L"X-Tang-File-Path"
#define XTangFileSize		L"X-Tang-File-Size"

// Apache定义的错误码
#define TANG_APACHE_BAND_WIDTH		510

#define CONFIG_XML_STRING	"<?xml version=\"1.0\" encoding=\"UTF-8\"?>	\
	<release version=\"4.3\" timestamp=\"1288852911000\"  \
		appid=\"1\" siteid=\"2\" skinid=\"3\" hold=\"1\" language=\"2052\">	\
	<file path= version= checksum= size=/>"

#define XMLHEADER				L"<?xml version=\"1.0\" encoding=\"utf-8\"?>"

#define NODE_NAME_UPDATE_RELEASE			L"release"
#define NODE_NAME_UPDATE_VERSION			L"version"
#define NODE_NAME_UPDATE_APPID				L"appid"
#define NODE_NAME_UPDATE_SITEID				L"siteid"
#define NODE_NAME_UPDATE_SKINID				L"skinid"
#define NODE_NAME_UPDATE_HOLD				L"hold"
#define NODE_NAME_UPDATE_LANGUAGE			L"language"
#define NODE_NAME_UPDATE_VERSION			L"version"
#define NODE_NAME_UPDATE_RELEASEID			L"releaseId"
#define NODE_NAME_UPDATE_FILE				L"file"
#define NODE_NAME_UPDATE_PATH				L"path"
#define NODE_NAME_UPDATE_CHECKSUM			L"checksum"
#define NODE_NAME_UPDATE_SIZE				L"size"

#define NEED_UPDATE_STATUS                  L"needupdate=false"	//16

using namespace uc;
using namespace pugi;

BOOL HttpOperator::m_bServerTellNeedUpdate = FALSE;

// 向指定URL发起请求，获得状态码，返回状态码
// 在状态码为200时，将HTTP Body数据，写入到指定的FileName文件中
HRESULT 
HttpOperator::Request(CString strUrl, CString strFileName, DWORD& dwResult)
{
	HRESULT             hr = S_OK;
	HINTERNET           hSession = NULL;
	HINTERNET           hRequest = NULL;
	DWORD				dwSize;        //size of the data available   
	LPTSTR				lpszData = NULL;        //buffer for the data   
	BOOL				bRet = FALSE;
	WIN32_FIND_DATA     wfd = {0} ;
	HANDLE              hFile = INVALID_HANDLE_VALUE;
	LPBYTE				pBuffer = NULL;
	DWORD				dwFileSize = 0;
	DWORD               readNumber = 1;
	DWORD               len = 0;
	CString		strDirectory;

	dwResult = ERROR_PARAMETER;

	UC_LOG(INFO) << "HttpOperator::Request strUrl = " << strUrl << ", strFileName = " << strFileName;

	BOOL_CHECK((strUrl.GetLength() > 0));
	BOOL_CHECK((strFileName.GetLength() > 0));

	dwResult = ERROR_SERVER_EXCEPTION;

	hSession = ::InternetOpen(L"MSIE/1.0", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if (NULL == hSession)
	{
		UC_LOG(ERROR) << "HttpOperator::Request InternetOpen failed." << ",GetLastError =" << ::GetLastError();
	}
	HANDLE_CHECK(hSession);

	hRequest = ::InternetOpenUrl(hSession, strUrl, NULL, 0, INTERNET_FLAG_DONT_CACHE, 0);
	if (NULL == hRequest)
	{
        DWORD dw = GetLastError();
		UC_LOG(ERROR) << "HttpOperator::Request InternetOpenUrl failed, strUrl = " << strUrl << ",GetLastError =" << dw;
	}
	HANDLE_CHECK(hRequest);

	// 服务器返回上传文件信息
	dwSize = sizeof(int);
	dwResult = 0;
	bRet = HttpQueryInfo(hRequest, HTTP_QUERY_STATUS_CODE | HTTP_QUERY_FLAG_NUMBER, &dwResult, &dwSize, NULL);
	if (!bRet)
	{
		DWORD dw = GetLastError();
		UC_LOG(ERROR) << "HttpOperator::Request HttpQueryInfo failed, HTTP_QUERY_STATUS_CODE = " 
			<< dwResult << ", GetLastError = " << dw;
		dwResult = ERROR_INTERNET_INCORRECT_HANDLE_TYPE;
	}
	BOOL_CHECK(bRet);

	UC_LOG(INFO) << "HttpOperator::Request HttpQueryInfo successed, dwResult = " << dwResult;

	if (HTTP_STATUS_OK == dwResult)
	{
		// 200，请求成功，将HTTP Body数据写入到本地指定文件中

		// 长度
		dwSize = sizeof(int);
		dwFileSize = 0;
		bRet = HttpQueryInfo(hRequest, HTTP_QUERY_CONTENT_LENGTH | HTTP_QUERY_FLAG_NUMBER, &dwFileSize, &dwSize, NULL);
		if (!bRet)
		{
			UC_LOG(ERROR) << "HttpOperator::Request HttpQueryInfo failed, HTTP_QUERY_CONTENT_LENGTH = " << dwResult << ",GetLastError =" << ::GetLastError();
		}
		BOOL_CHECK(bRet);

        //////////////////////////////////////////////////////////////////////////
        //判断sever返回值是NEED_UPDATE_STATUS, 这里判断长度为该值，则不需要写文件，函数直接返回通知应用不需要更新
        CString strNeedUpdateStatus = NEED_UPDATE_STATUS;
        UC_LOG(INFO) << "HttpQueryInfo return contents length = " << dwFileSize;
        if (dwFileSize == strNeedUpdateStatus.GetLength() || dwFileSize <= 0)
        {
            UC_LOG(INFO) << "ServerTellNeedUpdate = FALSE";
            m_bServerTellNeedUpdate = FALSE;
            dwResult = HTTP_STATUS_OK;
            hr = S_OK;
            goto exit;
        }
        else
        {
			UC_LOG(INFO) << "ServerTellNeedUpdate = TRUE";
			m_bServerTellNeedUpdate = TRUE;
        }

		// 读取Body的Data内容，写入到本地
		strDirectory = strFileName.Left(strFileName.ReverseFind('\\'));
		hr = m_folder.CycleCreateDirectory(strDirectory);
		UC_LOG(INFO) << "HttpOperator::Request CreateDirectoryEx = " << strDirectory << ", hr =" << hr;
		HR_CHECK(hr);

		hFile = ::CreateFile(strFileName, 
			GENERIC_READ | GENERIC_WRITE, 
			FILE_SHARE_READ | FILE_SHARE_WRITE, 
			NULL, CREATE_ALWAYS, NULL, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			dwResult = GetLastError();
			UC_LOG(ERROR) << "HttpOperator::Request CreateFile failed, "
				"strFileName = " << strFileName << ", GetLastError = " << dwResult;
			hr = E_FAIL;
			goto exit;
		}

		pBuffer = new BYTE[dwFileSize];
		BOOL bSuccess = ::InternetReadFile(hRequest, pBuffer, dwFileSize, &readNumber);
		if (!bSuccess)
		{
			UC_LOG(ERROR) << "HttpOperator::Request InternetReadFile failed, readNumber = " << readNumber << ",GetLastError =" << ::GetLastError();
		}
		BOOL_CHECK(bSuccess);

		bSuccess = ::WriteFile(hFile, pBuffer, readNumber, &len, NULL);
		if (!bSuccess)
		{
			UC_LOG(ERROR) << "HttpOperator::Request WriteFile failed, len = " << len << ",GetLastError =" << ::GetLastError();
		}
		BOOL_CHECK(bSuccess);

		dwResult = HTTP_STATUS_OK;
		hr = S_OK;
	} else if ( HTTP_STATUS_NOT_FOUND == dwResult )
	{
		// 404：当前版本是最新版本或客户端请求的版本不存在
		UC_LOG(ERROR) << "***Current version does not need to update or remote file does not exist.";
		hr = S_OK;
	} else if ( HTTP_STATUS_SERVER_ERROR == dwResult )
	{
		// 500，服务器错误
		UC_LOG(ERROR) << "***update server response " << dwResult << " code.";
		hr = E_FAIL;
	}
	else
	{
		UC_LOG(ERROR) << "***update server response " << dwResult << " code.";
		hr = E_FAIL;
	}

exit:
	if (INVALID_HANDLE_VALUE != hFile)
	{
		::CloseHandle(hFile);
	}
	if ( NULL != lpszData)
	{
		delete[] lpszData;
		lpszData = NULL;
	}
	if ( NULL != pBuffer )
	{
		delete [] pBuffer;
		pBuffer = NULL;
	}
	if (FAILED(hr))
	{
		UC_LOG(ERROR) << "HttpOperator::Request exit with error:" << hr;
	}
	if (hRequest != NULL)
	{
		::InternetCloseHandle(hRequest);
		hRequest = NULL;
	}
	if (hSession != NULL)
	{
		::InternetCloseHandle(hSession);
		hSession = NULL;
	}
	return (DWORD)hr;
}

HRESULT 
HttpOperator::RequestDownload(CString strUrl, CString strHttpPort, 
							  CString strFileName, UpdateConfig& fl, DWORD& dwResult, 	
							  IDownloadCallback* pMsgProvider, CString strUrlDown, 
							  CString downLoadReleaseId)
{
	HRESULT             hr = S_OK;
	HINTERNET           hSession = NULL;
	HINTERNET           hRequest = NULL;
	HINTERNET			hConnection = NULL;
	DWORD				dwSize;        //size of the data available   
	LPTSTR				lpszData = NULL;        //buffer for the data   
	BOOL				bRet = FALSE;
	WIN32_FIND_DATA     wfd = {0} ;
	HANDLE              hFile = INVALID_HANDLE_VALUE;
	LPBYTE              Temp = NULL;
	LPBYTE				pBuffer = NULL;
	LPTSTR				lpszBuffer = NULL;
	UINT64				dwFileSize = 0;
	DWORD               readNumber = 1;
	CString				strDirectory;
	CString				Url = L"";
	CString				strHeader = L"";
	CString				strMimeHeader;
	CString				strContext;
	DWORD				nLen = 0;
	const int			MAX_BUF = 25;
	TCHAR				str[MAX_BUF];   
	DWORD				dwBytesWritten;			
	CString				fileName;
	std::wstring        strProtocol;
	std::wstring		strFormatUrl;
	std::wstring		strFormatPath;
	std::wstring		strFormatParams;
	INTERNET_PORT		PointPort;
	UINT64				nOffset = 0;
	CString				sHost = L"";
	CString				sApi = L"";

	BOOL_CHECK((strUrl.GetLength() > 0));
	BOOL_CHECK((fl.m_components.size() > 0));

#ifdef _GET_
	// /download?f=/abc.exe:1.0.0,/123.dll:1.0.1
	Url = _T("http://");
	Url.append(strUrl);
	if (_T("") != strHttpPort && _T("80") != strHttpPort)
	{
		Url.append(_T(":"));
		Url.append(strHttpPort);
	}
	Url.append(_T("/download?f="));

	strHeader = L"";
	for (std::vector<UpdateFile>::iterator iter = fl.m_components.begin();
		iter != fl.m_components.end(); ++iter)
	{
		// /abc.exe:1.0.0,
		strHeader.append(iter->m_strFileFullName);
		strHeader.append(_T(":"));
		strHeader.append(iter->m_strVersion);
		strHeader.append(_T(","));
	}

	dwResult = ERROR_PARAMETER;

	//LOG_INFO(L"HttpOperator::Request(GET) strUrl = %s, strFileName = %s", strHeader.c_str(), strFileName.c_str());

	BOOL_CHECK((strHeader.size() > 0));
	BOOL_CHECK((strFileName.size() > 0));

	dwResult = ERROR_SERVER_EXCEPTION;

	Url.append(strHeader);

	// 由于中文出现乱码，使用UTF-8
	ULONG FILE_BLOCK_SIZE = Url.size() * 2;
	pBuffer = new BYTE[FILE_BLOCK_SIZE];
	nLen = ::WideCharToMultiByte(CP_UTF8, 0, Url.c_str(), Url.size(), 
		(LPSTR)pBuffer, FILE_BLOCK_SIZE, NULL, NULL);
	BOOL_CHECK((nLen > 0));
	pBuffer[nLen] = '\0';

	hSession = ::InternetOpen(L"MSIE/1.0", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if (NULL == hSession)
	{
		//LOG_ERR(L"HttpOperator::RequestDownload InternetOpen failed.");
	}
	HANDLE_CHECK(hSession);
/*
	// 测试
	hRequest = ::InternetOpenUrl(hSession, _T("http://www.baidu.com"), NULL, 0, INTERNET_FLAG_DONT_CACHE, 0);
	HANDLE_CHECK(hRequest);
*/
	_ASSERT(0);
	hRequest = ::InternetOpenUrl(hSession, (LPTSTR)pBuffer, NULL, 0, INTERNET_FLAG_DONT_CACHE, 0);
	SAFE_DELETE(pBuffer);

	if (NULL == hRequest)
	{
		DWORD dw = GetLastError();
		//LOG_ERR(L"HttpOperator::RequestDownload InternetOpenUrl failed, GetLastError = %d, strUrl = %s.", 
			dw, strUrl.c_str());

	}
	HANDLE_CHECK(hRequest);

#else
	// Post方式不需要加http协议头
//	Url = _T("http://");
// 	hr = ParseUrl(strUrl.GetString(), PointPort, strProtocol, strFormatUrl, strFormatPath, strFormatParams);
// 	HR_CHECK(hr);
// 
// 	Url = strFormatUrl.c_str();

	Url = strUrl;
	//去年协议头
	int npos = strUrl.Find(L"://");
	if (npos>0)
	{
		Url = strUrl.Right(strUrl.GetLength() - npos - 3);
	}
	
	//取host和api
	npos = Url.Find(L'/');
	if (npos > 0 )
	{
		sApi = Url.Right(Url.GetLength() - npos-1);
		sHost = Url.Left(npos);
		npos = Url.Find(L":");
		if (npos>0)
		{
			sHost = Url.Left(npos);
		}
	}

	UC_LOG(WARNING) << "HttpOperator::RequestDownload(POST) Url = " << Url
		<< ", UrlDwon=" << strUrlDown
		<< ", strHttpPort =" << strHttpPort
		<< ", strFormatPath = " << strFormatPath
		<< ", strFormatParams = " << strFormatParams
		<< ", sHost = " << sHost
		<< ", sApi = " << sApi;


	// /download?f=/abc.exe:1.0.0,/123.dll:1.0.1
	strContext = L"r=";
	strContext.Append(downLoadReleaseId);
	strContext.Append(_T("&f="));
	for (std::vector<UpdateFile>::iterator iter = fl.m_components.begin();
		iter != fl.m_components.end(); ++iter)
	{
		// /abc.exe:1.0.0,
		strContext.Append(iter->m_strFileFullName);
		strContext.Append(_T(":"));
		strContext.Append(iter->m_strVersion);
		strContext.Append(_T(","));
	}
	// 去掉尾部的,号
	strContext = strContext.Left(strContext.GetLength() - 1);

	dwResult = ERROR_PARAMETER;

	BOOL_CHECK((strContext.GetLength() > 0));
	BOOL_CHECK((strFileName.GetLength() > 0));

	if (pMsgProvider)
	{
		BOOL_CHECK(!pMsgProvider->IsCancel());
	}
	dwResult = ERROR_SERVER_EXCEPTION;

	// 由于中文出现乱码，使用UTF-8
	int FILE_BLOCK_SIZE = strContext.GetLength() * 2;
	pBuffer = new BYTE[FILE_BLOCK_SIZE];
	nLen = ::WideCharToMultiByte(CP_UTF8, 0, strContext, strContext.GetLength(), 
		(LPSTR)pBuffer, FILE_BLOCK_SIZE, NULL, NULL);
	BOOL_CHECK((nLen > 0));
	pBuffer[nLen] = '\0';

	UC_LOG(WARNING) << "HttpOperator::RequestDownload start connect url, strContext:" << strContext;

	hSession = ::InternetOpen(L"MSIE/1.0", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if (NULL == hSession)
	{
		UC_LOG(ERROR) << "HttpOperator::RequestDownload InternetOpen failed. error:" << ::GetLastError();
	}
	HANDLE_CHECK(hSession);

	if (_T("") != strHttpPort && _T("80") != strHttpPort)
	{
		hConnection = ::InternetConnect(hSession, sHost, _ttoi(strHttpPort), 
			NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
	} else {
		hConnection = ::InternetConnect(hSession, sHost, INTERNET_DEFAULT_HTTP_PORT, 
			NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
	}
	HANDLE_CHECK(hConnection);

	strHeader = _T("");
	strHeader.Append(_T("Content-Length: "));
	_stprintf_s(str, MAX_BUF, _T("%d"),   nLen );   
	strHeader.Append(str);
	strHeader.Append(_T("\r\nConnection: close\r\t"));

	if (strFormatPath.size() > 0)
	{
		strMimeHeader = strFormatPath.c_str();
		strMimeHeader += _T("download");
	} else {
		strMimeHeader   =   _T("/download");
	}
	LPCTSTR accept[] = {
         _T("*/*"),
         NULL
     };
	hRequest = ::HttpOpenRequest(hConnection, _T("POST"), sApi, 
		NULL, NULL, accept, 0, INTERNET_FLAG_EXISTING_CONNECT|INTERNET_FLAG_NO_AUTO_REDIRECT|INTERNET_FLAG_HYPERLINK);
	if (FAILED(hr))
	{
		UC_LOG(ERROR) << "HttpOperator::RequestDownload HttpOpenRequest failed, server exception! error:" << ::GetLastError();
	}
	HANDLE_CHECK(hRequest);

	if (pMsgProvider)
	{
		BOOL_CHECK(!pMsgProvider->IsCancel());
	}

	INTERNET_BUFFERS ib;
	memset(&ib, 0, sizeof(INTERNET_BUFFERS));
	ib.dwStructSize = sizeof(INTERNET_BUFFERS);
	ib.dwBufferTotal = strContext.GetLength();    
	ib.dwBufferLength = strContext.GetLength();    

	dwResult = ERROR_SERVER_EXCEPTION;
	HttpAddRequestHeaders(   hRequest,   strHeader,   (DWORD)strHeader.GetLength(),   HTTP_ADDREQ_FLAG_ADD); 
	if(!HttpSendRequestEx(hRequest, &ib,NULL,0, 0))
	{
		DWORD dw = ::GetLastError();
		hr = HRESULT_FROM_WIN32(dw);
		if (FAILED(hr))
		{
			UC_LOG(ERROR) << "HttpOperator::RequestDownload HttpSendRequestEx failed, server exception, hr = " << hr << ", last error:" << dw;
		}
		HR_CHECK(hr);
	}

	InternetWriteFile(hRequest, pBuffer, nLen, &dwBytesWritten);
	BOOL_CHECK((dwBytesWritten == nLen));

	if (!HttpEndRequest(hRequest,NULL, NULL, NULL))
	{
		DWORD dw = ::GetLastError();
		hr = HRESULT_FROM_WIN32(dw);
		UC_LOG(ERROR) << "HttpOperator::RequestDownload HttpEndRequest failed, server exception, GetLastError = " << dw;
		HR_CHECK(hr);
	}
#endif

	if (pMsgProvider)
	{
		BOOL_CHECK(!pMsgProvider->IsCancel());
	}

	// 服务器返回上传文件信息
	dwSize = sizeof(int);
	dwResult = 0;
	bRet = HttpQueryInfo(hRequest, HTTP_QUERY_STATUS_CODE | HTTP_QUERY_FLAG_NUMBER, &dwResult, &dwSize, NULL);
	if (!bRet)
	{
		DWORD dw = GetLastError();
		UC_LOG(ERROR) << "HttpOperator::RequestDownload HttpQueryInfo failed, HTTP_QUERY_STATUS_CODE = " 
			<< dwResult << ", GetLastError = " << dw;
		dw = ERROR_INTERNET_INCORRECT_HANDLE_TYPE;

		// added by jianhua.chen for debug
		WCHAR chHttpRawHeader[2048] = {0};
		if(HttpQueryInfo(hRequest, HTTP_QUERY_RAW_HEADERS_CRLF, (LPVOID)chHttpRawHeader, &dwSize, NULL))
		{
			UC_LOG(ERROR) << "HttpQueryInfo return size:" << dwSize << ",http header:" << chHttpRawHeader << ",GetLastError =" << ::GetLastError();
		}

	}
	BOOL_CHECK(bRet);

	UC_LOG(INFO) << "HttpOperator::RequestDownload HttpQueryInfo successed, dwResult = " << dwResult;

	if (HTTP_STATUS_OK == dwResult)
	{
		// 升级服务器出于性能考虑，Http返回URL链接，客户端根据URL，从Apache服务器下载

		dwSize = MAXBLOCKSIZE;
		lpszData = new TCHAR[dwSize + 1];
		memset(lpszData, 0, sizeof(lpszData));

		_tcscpy_s(lpszData, dwSize, XTangFilePath);

		bRet = HttpQueryInfo(hRequest, HTTP_QUERY_CUSTOM, lpszData, &dwSize, NULL);
		if (!bRet)
		{
			UC_LOG(ERROR) << "HttpOperator::RequestDownload HttpQueryInfo failed, HTTP_QUERY_CUSTOM = " 
				<< lpszData << ", error:" << ::GetLastError();

			// 使用旧的升级方案

			// 200，请求成功，将HTTP Body数据写入到本地指定文件中

			// 长度
			dwSize = sizeof(int);
			dwFileSize = 0;
			bRet = HttpQueryInfo(hRequest, HTTP_QUERY_CONTENT_LENGTH, &dwFileSize, &dwSize, NULL);
			if (!bRet)
			{
				UC_LOG(ERROR) << "HttpOperator::RequestDownload HttpQueryInfo failed, HTTP_QUERY_CONTENT_LENGTH = " 
					<< dwFileSize << ", error:" << ::GetLastError();
			}
			BOOL_CHECK(bRet);

			// 读取Body的Data内容，写入到本地
			hFile = ::FindFirstFile(strFileName, &wfd);
			if (hFile != INVALID_HANDLE_VALUE)
			{
				::FindClose(hFile);
				::DeleteFile(strFileName);
			}
			strDirectory = strFileName.Left(strFileName.ReverseFind('\\'));

			m_folder.CycleCreateDirectory(strDirectory);

			hFile = ::CreateFile(strFileName, 
				GENERIC_READ | GENERIC_WRITE, 
				FILE_SHARE_READ | FILE_SHARE_WRITE, 
				NULL, CREATE_ALWAYS, NULL, NULL);
			if (hFile == INVALID_HANDLE_VALUE)
			{
				dwResult = ERROR_CREATE_FILE_FAILED;

				UC_LOG(ERROR) << "HttpOperator::RequestDownload CreateFile failed, strFileName = " 
					<< strFileName << ", error:" << ::GetLastError();
				hr = HRESULT_FROM_WIN32(::GetLastError());
				goto exit;
			}
			pBuffer = new BYTE[dwFileSize];
			BOOL bSuccess = ::InternetReadFile(hRequest, pBuffer, dwFileSize, &readNumber);
			if (!bSuccess)
			{
				UC_LOG(ERROR) << "HttpOperator::RequestDownload InternetReadFile failed, readNumber = " 
					<< readNumber << ", error:" << ::GetLastError();
			}
			BOOL_CHECK(bSuccess);

			bSuccess = ::WriteFile(hFile, pBuffer, readNumber, &nLen, NULL);
			if (!bSuccess)
			{
				UC_LOG(ERROR) << "HttpOperator::RequestDownload WriteFile failed, nLen = " 
					<< nLen << ", error:" << ::GetLastError();
			}
			BOOL_CHECK(bSuccess);
		} 
		else 
		{
			lpszData[dwSize] = L'\0';
			strUrlDown.Append(lpszData, dwSize);

			UC_LOG(INFO) << "HttpOperator::RequestDownload It is " << strUrlDown << " of upgrade file path.";

			// 获得文件大小
			dwSize = MAXBLOCKSIZE;
			memset(lpszData, 0, sizeof(lpszData));

			_tcscpy_s(lpszData, dwSize, XTangFileSize);

			bRet = HttpQueryInfo(hRequest, HTTP_QUERY_CUSTOM, lpszData, &dwSize, NULL);
			if (!bRet)
			{
				UC_LOG(ERROR) << "HttpOperator::RequestDownload HttpQueryInfo X-Tang-File-Size failed. error:" << ::GetLastError();
			}
			BOOL_CHECK(bRet);

			try {
				dwFileSize = _ttoi(lpszData);
				UC_LOG(INFO) << "dwFileSize = " << dwFileSize;
			} catch (...)
			{
				dwFileSize = (DWORD)-1;
				UC_LOG(ERROR) << "_ttoi(lpszData) failed!";
				hr = E_FAIL;
				goto exit;
			}

			// 结束前一个Http连接
			UC_LOG(INFO) << "Begin Cancel Prev HTTP Connect";
			if (hRequest != NULL)
			{
				::InternetCloseHandle(hRequest);
				hRequest = NULL;
			}
			if (hConnection != NULL)
			{
				::InternetCloseHandle(hConnection);
				hSession = NULL;
			}
			if (hSession != NULL)
			{
				::InternetCloseHandle(hSession);
				hSession = NULL;
			}
			
			if (pMsgProvider)
			{
				BOOL_CHECK(!pMsgProvider->IsCancel());
			}

			UC_LOG(INFO) << "Begin InternetOpen";
			hSession = ::InternetOpen(L"MSIE/1.0", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
			if (hSession == NULL)
			{
				UC_LOG(ERROR) << "InternetOpen Failed with errcode:" << ::GetLastError();
			}
			HANDLE_CHECK(hSession);

			// 下载指定的文件到本地，如果服务器出错，重试3次
			int nRetry = 3;
			while (nRetry-- > 0)
			{
				UC_LOG(INFO) << "Try InternetOpenUrl at " << 3 - nRetry << " times";
				hRequest = ::InternetOpenUrl(hSession, strUrlDown, NULL, 0, INTERNET_FLAG_DONT_CACHE, 0);
				if (hRequest == NULL)
				{
					UC_LOG(ERROR) << "InternetOpenUrl Failed with errcode:" << ::GetLastError() << ", url =" << strUrlDown;
				}
				HANDLE_CHECK(hRequest);

				// 服务器返回上传文件信息
				dwSize = sizeof(int);
				dwResult = 0;
				bRet = HttpQueryInfo(hRequest, HTTP_QUERY_STATUS_CODE | HTTP_QUERY_FLAG_NUMBER, &dwResult, &dwSize, NULL);
				if (!bRet)
				{
					DWORD dw = GetLastError();
					UC_LOG(ERROR) << "HttpOperator::RequestDownload HttpQueryInfo failed, "
						"HTTP_QUERY_STATUS_CODE = " << dwResult << ", GetLastError = " << dw;
				}
				BOOL_CHECK(bRet);

				if (TANG_APACHE_BAND_WIDTH == dwResult)
				{
					UC_LOG(INFO) << "HttpQueryInfo return TANG_APACHE_BAND_WIDTH,Try again!";
					// 重试
					if (hRequest != NULL)
					{
						::InternetCloseHandle(hRequest);
						hRequest = NULL;
					}

					// 等待3秒再试
					Sleep(3000);
				} 
				else if (HTTP_STATUS_OK == dwResult)
				{
					UC_LOG(INFO) << "HttpQueryInfo return HTTP_STATUS_OK";
					break;
				} 
				else 
				{
					UC_LOG(ERROR) << "HttpQueryInfo Failed!";
					// 出错
					hr = E_FAIL;
					goto exit;
				}
			}

			// Test whether the *.dwn file is exist.
			fileName = strFileName;
			fileName.Append(L".dwn");
			UC_LOG(INFO) << "fileName = " << fileName;

			hFile = ::CreateFile(fileName, 
				GENERIC_READ | GENERIC_WRITE, 
				FILE_SHARE_READ | FILE_SHARE_WRITE, 
				NULL, CREATE_ALWAYS, NULL, NULL);
			CHECK_IF(hFile == INVALID_HANDLE_VALUE, HRESULT_FROM_WIN32(GetLastError()));

			// Download file from internet.
			int nBuffer = 50 * MAXBLOCKSIZE;
			Temp = new BYTE[nBuffer];
			CHECK_IF_NULL(Temp);
			UC_LOG(INFO) << "Begin Download File!";
			while (readNumber > 0)
			{
				memset(Temp, 0, nBuffer);
				// 每次读取50KB
				BOOL bSuccess = ::InternetReadFile(hRequest, Temp, nBuffer, &readNumber);
				if (!bSuccess)
				{
					UC_LOG(ERROR) << "InternetReadFile Failed with errcode:" << GetLastError();
				}
				BOOL_CHECK(bSuccess);
				// TODO: If download error, how to do.

				bSuccess = ::WriteFile(hFile, Temp, readNumber, &nLen, NULL);
				if (!bSuccess)
				{
					UC_LOG(ERROR) << "WriteFile Failed with errcode:" << GetLastError();
				}
				BOOL_CHECK(bSuccess);

				if (pMsgProvider)
				{
					BOOL_CHECK(!pMsgProvider->IsCancel());

					nOffset += readNumber;

					pMsgProvider->DoResult(nOffset * 100 / dwFileSize);

					UC_LOG(INFO) << "HttpOperator::RequestDownload download, nOffset: " << nOffset << ", readNumber:" << readNumber << ", percent:" << nOffset * 100 / dwFileSize << " percent.";
				}

			}
			SAFE_DELETE_ARRAY(Temp);

			/**< 下载的文件大小与服务器上的不匹配，是为升级失败 modify by huajian.yin */
			UC_LOG(INFO) << "The actual download file size = " << nOffset << ", need to download file size = " << dwFileSize;
			if (nOffset < dwFileSize)
			{
				UC_LOG(ERROR) << "nOffset < dwFileSize";
				HR_CHECK(E_FAIL);
			}

			// Change file name from *.dwn to ordinal name, first must close the file.
			if (hFile != INVALID_HANDLE_VALUE)
			{
				::CloseHandle(hFile);
				hFile = INVALID_HANDLE_VALUE;
			}
			if (m_folder.IsFileExists(strFileName))
			{
				::DeleteFile(strFileName);
			}
			BOOL bMoveFileSuccess = ::MoveFileEx(fileName, strFileName, MOVEFILE_COPY_ALLOWED);
			if (!bMoveFileSuccess)
			{
				UC_LOG(ERROR) << "MoveFileEx Failed with errcode:" << GetLastError();
			}
			BOOL_CHECK(bMoveFileSuccess);
		}

		hr = S_OK;
	} else if ( HTTP_STATUS_NOT_FOUND == dwResult )
	{
		// 404
		UC_LOG(ERROR) << "HTTP status code, dwResult = " << HTTP_STATUS_NOT_FOUND;
		hr = E_FAIL;
	} else
	{
		UC_LOG(ERROR) << "dwResult = Other Error code:" << dwResult;
		// 500或其他错误
		hr = E_FAIL;
	}
	HR_CHECK(hr);
	BOOL_CHECK((HTTP_STATUS_OK == dwResult));

	// 下载成功
exit:
	if (FAILED(hr))
	{
		UC_LOG(ERROR) << "HttpOperator::RequestDownload exit with error:" << hr;
	}
	if (INVALID_HANDLE_VALUE != hFile)
	{
		::CloseHandle(hFile);
	}
	if ( NULL != lpszData)
	{
		delete[] lpszData;
		lpszData = NULL;
	}
	if ( NULL != pBuffer )
	{
		delete [] pBuffer;
		pBuffer = NULL;
	}
	if (hRequest != NULL)
	{
		::InternetCloseHandle(hRequest);
		hRequest = NULL;
	}
	if (hConnection != NULL)
	{
		::InternetCloseHandle(hConnection);
		hSession = NULL;
	}
	if (hSession != NULL)
	{
		::InternetCloseHandle(hSession);
		hSession = NULL;
	}
	return (DWORD)hr;
}

UpdateConfig HttpOperator::ReadBaseConfig(CString strConfigFileName, HRESULT& hr)
{
	UC_LOG(INFO) << "ReadBaseConfig, configFileName:" << strConfigFileName;

	UpdateConfig baseCfg;
	pugi::xml_document doc;
	wstring cfgFile(strConfigFileName.GetBuffer());
	pugi::xml_parse_result ret = doc.load_file(cfgFile.c_str());
	if (ret.status != pugi::status_ok) {
		UC_LOG(ERROR) << "ReadBaseConfig error! " << ret.status << ", file:" << cfgFile;
		hr = E_FAIL;
		return baseCfg;
	}

	//base config info
	xml_node cfgInfo = doc.child("release");
	baseCfg.m_releaseId = CString(cfgInfo.attribute("releaseId").as_string());
	baseCfg.m_strVersion = CString(cfgInfo.attribute("version").as_string());
	baseCfg.m_strSiteId = CString(cfgInfo.attribute("site").as_string());
	
	UC_LOG(INFO) << "HttpOperator::ReadBaseConfig, release id:" 
		<< baseCfg.m_releaseId << ", version:" << baseCfg.m_strVersion << ", site id:" << baseCfg.m_strSiteId;

	return baseCfg;
}

// 读取配置文件，将配置文件数据填入fl
HRESULT HttpOperator::ReadConfig(CString strConfigFileName, 
								 UpdateConfig& fl)
{
	HRESULT hr = S_OK;
	VARIANT variant;
	IXMLDOMDocument * pRecorderXML = NULL;
	VARIANT_BOOL bSuccess = VARIANT_TRUE;
	IXMLDOMNode* pNode = NULL;
	CComPtr<IXMLDOMNamedNodeMap> pDOMNamedNodeMap = NULL;
	CComPtr<IXMLDOMNode> pNamedItemNode = NULL;
	CComVariant varAttributeValue;
	CComPtr<IXMLDOMNodeList> pChildLst = NULL;
	CComPtr<IXMLDOMNode> pNextNode = NULL;
	
	::CoInitialize(NULL);
	hr =::CoCreateInstance(
		CLSID_DOMDocument, 
		NULL, 
		CLSCTX_INPROC_SERVER,
		IID_IXMLDOMDocument,
		(LPVOID*)(&pRecorderXML) );
	HR_CHECK(hr);
	ARG_CHECK(pRecorderXML);

	pRecorderXML->AddRef();
	variant.vt = VT_BSTR;
	UC_LOG(INFO) << "Config file name:" << strConfigFileName;
	variant.bstrVal = SysAllocString(strConfigFileName);
	hr = pRecorderXML->load(variant, &bSuccess);
	SysFreeString(variant.bstrVal);
	HR_CHECK(hr);

	hr = pRecorderXML->selectSingleNode(NODE_NAME_UPDATE_RELEASE, &pNode);
	HR_CHECK(hr);
	CHECK_IF_NULL(pNode);

	hr = pNode->get_attributes(&pDOMNamedNodeMap);
	HR_CHECK(hr);
	if( NULL != pDOMNamedNodeMap )
	{
		//Release id
		hr = pDOMNamedNodeMap->getNamedItem(NODE_NAME_UPDATE_RELEASEID, &pNamedItemNode);
		if (NULL != pNamedItemNode)
		{
			hr = pNamedItemNode->get_nodeValue(&varAttributeValue);
			HR_CHECK(hr);

			fl.m_releaseId = varAttributeValue.bstrVal;
		}

		pNamedItemNode = NULL;

		// version
		hr = pDOMNamedNodeMap->getNamedItem(NODE_NAME_UPDATE_VERSION, &pNamedItemNode);
		HR_CHECK(hr);
		if( NULL != pNamedItemNode )
		{
			hr = pNamedItemNode->get_nodeValue(&varAttributeValue);
			HR_CHECK(hr);

			fl.m_strVersion = varAttributeValue.bstrVal;
		}

		pNamedItemNode = NULL;
		//SiteId
		hr = pDOMNamedNodeMap->getNamedItem(L"site", &pNamedItemNode);
		HR_CHECK(hr);
		if( NULL != pNamedItemNode )
		{
			hr = pNamedItemNode->get_nodeValue(&varAttributeValue);
			HR_CHECK(hr);

			fl.m_strSiteId = varAttributeValue.bstrVal;
		}
	}


	// 取文件列表
	pChildLst = NULL;
	hr = pNode->get_childNodes(&pChildLst);
	HR_CHECK(hr);
	long len = 0;
	if (NULL != pChildLst )
	{
		hr = pChildLst->get_length(&len);
		HR_CHECK(hr);
	}
	// clear the text of all child node 
	for (long i = 0;i<len;i++)
	{
		pNextNode = NULL;
		hr = pChildLst->nextNode(&pNextNode);
		HR_CHECK(hr);
		if ( pNextNode != NULL)
		{
			pDOMNamedNodeMap = NULL;
			hr = pNextNode->get_attributes(&pDOMNamedNodeMap);
			HR_CHECK(hr);
			if( NULL != pDOMNamedNodeMap )
			{
				UpdateFile uf;
				// path
				pNamedItemNode = NULL;
				hr = pDOMNamedNodeMap->getNamedItem(NODE_NAME_UPDATE_PATH,&pNamedItemNode);
				HR_CHECK(hr);
				if( NULL != pNamedItemNode )
				{
					hr = pNamedItemNode->get_nodeValue(&varAttributeValue);
					HR_CHECK(hr);

					uf.m_strFileFullName = varAttributeValue.bstrVal;
				}

				// version
				pNamedItemNode = NULL;
				hr = pDOMNamedNodeMap->getNamedItem(NODE_NAME_UPDATE_VERSION,&pNamedItemNode);
				HR_CHECK(hr);
				if( NULL != pNamedItemNode )
				{
					hr = pNamedItemNode->get_nodeValue(&varAttributeValue);
					HR_CHECK(hr);

					uf.m_strVersion = varAttributeValue.bstrVal;
				}
				
				// checksum
				pNamedItemNode = NULL;
				hr = pDOMNamedNodeMap->getNamedItem(NODE_NAME_UPDATE_CHECKSUM,&pNamedItemNode);
				HR_CHECK(hr);
				if( NULL != pNamedItemNode )
				{
					hr = pNamedItemNode->get_nodeValue(&varAttributeValue);
					HR_CHECK(hr);

					uf.m_strChecksum = varAttributeValue.bstrVal;
				}

				// size
				pNamedItemNode = NULL;
				hr = pDOMNamedNodeMap->getNamedItem(NODE_NAME_UPDATE_SIZE,&pNamedItemNode);
				HR_CHECK(hr);
				if( NULL != pNamedItemNode )
				{
					hr = pNamedItemNode->get_nodeValue(&varAttributeValue);
					HR_CHECK(hr);

					uf.m_uSize = varAttributeValue.uintVal;
				}

				fl.m_components.push_back(uf);
			}
		}
	}
exit:
	if (NULL != pRecorderXML)
	{
		pRecorderXML->Release();
		pRecorderXML = NULL;
	}
	if (NULL != pNode)
	{
		pNode->Release();
		pNode = NULL;
	}
	::CoUninitialize();
	if (FAILED(hr))
	{
		UC_LOG(ERROR) << "HttpOperator::ReadConfig exit with error:" << hr;
	}
	return (DWORD)hr;
}

HRESULT HttpOperator::ReadNotes(CString strConfigFileName, NotesCfg& notes) {
	UC_LOG(INFO) << "ReadNotes, cfgFileName:" << strConfigFileName;

	pugi::xml_document doc;
	wstring cfgFile(strConfigFileName.GetBuffer());
	pugi::xml_parse_result ret = doc.load_file(cfgFile.c_str());
	if (ret.status != pugi::status_ok) {
		UC_LOG(ERROR) << "ReadNotes error! " << ret.status << ", file:" << cfgFile;
		return E_FAIL;
	}

	//version
	xml_node cfgVersion = doc.child("release");
	notes.m_strVersion = StringConvert::FromUTF8Str(cfgVersion.attribute("version").as_string());

	//release note
	notes.m_notes = StringConvert::FromUTF8Str(cfgVersion.child_value("notes"));

	UC_LOG(INFO) << "ReadNotes, version:" << notes.m_strVersion << ", notes:" << notes.m_notes;

	return S_OK;
}

// 读取配置文件，将配置文件数据填入fl
HRESULT HttpOperator::WriteConfig(CString strConfigFileName, 
								 UpdateConfig& fl)
{
	HRESULT hr = S_OK;
	IXMLDOMDocument * pRecorderXML = NULL;
	CComPtr<IXMLDOMProcessingInstruction> pProInstruction = NULL; // xml声明
	CComPtr<IXMLDOMElement> pRootElement = NULL, pElement = NULL; // 根节点(元素) 
	CComPtr<IXMLDOMAttribute> pAttrNode = NULL; // 属性
	VARIANT_BOOL bSuccess = VARIANT_TRUE;
	IXMLDOMNode* pNode = NULL;
	CComPtr<IXMLDOMNamedNodeMap> pDOMNamedNodeMap = NULL;
	CComPtr<IXMLDOMNode> pNamedItemNode = NULL;
	CComVariant varAttributeValue;
	CComPtr<IXMLDOMNodeList> pChildLst = NULL;
	CComPtr<IXMLDOMNode> pNextNode = NULL;
	
	::CoInitialize(NULL);
	hr =::CoCreateInstance(
		CLSID_DOMDocument, 
		NULL, 
		CLSCTX_INPROC_SERVER,
		IID_IXMLDOMDocument,
		(LPVOID*)(&pRecorderXML) );
	HR_CHECK(hr);
	ARG_CHECK(pRecorderXML);


	// (1)创建xml文档声明   
	hr = pRecorderXML->createProcessingInstruction((_bstr_t)L"xml", (_bstr_t)L"version=\"1.0\" encoding=\"utf-8\"", &pProInstruction);    
	HR_CHECK(hr);
	ARG_CHECK(pProInstruction);

	pNode = NULL;
	hr = pRecorderXML->appendChild((IXMLDOMNode*)pProInstruction, &pNode);   
	HR_CHECK(hr);

	// (2)创建根节点<Release>   
	hr = pRecorderXML->createElement((_bstr_t)NODE_NAME_UPDATE_RELEASE, &pRootElement);       
	HR_CHECK(hr);
	ARG_CHECK(pRootElement);

	hr = pRecorderXML->appendChild((IXMLDOMNode*)pRootElement, &pNode);   
	HR_CHECK(hr);

	pRootElement->setAttribute((_bstr_t)NODE_NAME_UPDATE_VERSION, (_variant_t)fl.m_strVersion); 

	for (UPDATEFILEVECTOR::iterator iter = fl.m_components.begin(); 
		iter != fl.m_components.end(); iter++)
	{
		// (3)创建节点<Release><file>       
		pElement = NULL;
		hr = pRecorderXML->createElement((_bstr_t)NODE_NAME_UPDATE_FILE, &pElement);   
		HR_CHECK(hr);
		ARG_CHECK(pElement);

		pNode = NULL;
		pRootElement->appendChild(pElement, &pNode); 

		// (4)设置节点属性<Release><file path="">   
		pElement->setAttribute((_bstr_t)NODE_NAME_UPDATE_PATH, (_variant_t)iter->m_strFileFullName); 
		pElement->setAttribute((_bstr_t)NODE_NAME_UPDATE_VERSION, (_variant_t)iter->m_strVersion); 
		pElement->setAttribute((_bstr_t)NODE_NAME_UPDATE_CHECKSUM, (_variant_t)iter->m_strChecksum); 
		pElement->setAttribute((_bstr_t)NODE_NAME_UPDATE_SIZE, (_variant_t)iter->m_uSize); 
	}

	hr = pRecorderXML->save((_variant_t)strConfigFileName);   
	HR_CHECK(hr);

exit:
	if (NULL != pRecorderXML)
	{
		pRecorderXML->Release();
	}
	::CoUninitialize();
	if (FAILED(hr))
	{
		UC_LOG(ERROR) << "HttpOperator::ReadConfig exit with error:" << hr;
	}
	return (DWORD)hr;
}

HRESULT 
HttpOperator::QueryInstallList(CString strUrl, CString strHttpPort, CString strAppId,
	CString strSiteId,	CString strSkinId, CString strConfigFileName, 
	CString strVersion, DWORD& dwResult)
{
	HRESULT             hr = S_OK;
	CString		        Url = L"";
	std::wstring        strProtocol;
	std::wstring		strFormatUrl;
	std::wstring		strFormatPath;
	std::wstring		strFormatParams;
	INTERNET_PORT		PointPort;

	Url = strUrl;
	Url.Append(_T("?a="));
	Url.Append(strAppId);
	Url.Append(_T("&s="));
	Url.Append(strSiteId);
	Url.Append(_T("&sk="));
	Url.Append(strSkinId);

    //打开&v=参数，本地版本信息
	if (strVersion.GetLength() > 0)
	{
		Url.Append(_T("&v="));
		Url.Append(strVersion);
	}

	UC_LOG(INFO) << "HttpOperator::QueryInstallList request url:" << Url << ", strConfigFileName:" << strConfigFileName;
	hr = Request(Url, strConfigFileName, dwResult);
	HR_CHECK(hr);

exit:
	if (FAILED(hr))
	{
		UC_LOG(ERROR) << "HttpOperator::QueryInstallList exit with error:" << hr;
	}
	return (DWORD)hr;
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
// HRESULT HttpOperator::ParseUrl(std::wstring strInUrl, INTERNET_PORT& port, std::wstring& strProtocol,
// 							   std::wstring& strOutUrl, std::wstring& strOutPath, 
// 							   std::wstring& strOutParams)
// {
// 	HRESULT hr = S_OK;
// 	CAtlRegExp<> reUrl;
// 	CAtlREMatchContext<> mcUrl;
// 	UINT nGroupIndex = 0;
// 	const CAtlREMatchContext<>::RECHAR* szStart = 0;
// 	const CAtlREMatchContext<>::RECHAR* szEnd = 0;
// 	ptrdiff_t nLength = 0;
// 	std::wstring str;
// 	std::wstring::size_type st;
// 	std::wstring::size_type stLength;
// 
// 	strOutUrl = _T("");
// 	strOutPath = _T("");
// 
// 	// 输入 http://search.microsoft.com/us/
// 	// five match groups: scheme, authority, path, query, fragment
// 	// 以()为分界标志，共分成5组
// 	// 第1组：{[^:/?#]+}:，^非，遇到:/?#则停止。输入http://时，取出http
// 	REParseError status = 
// 		reUrl.Parse(_T("({[^:/?#]+}://)?({[^:/?#]*})?({:[0-9]+})?(/{[^/&?#]*})?({?[^/]*})?"));
// 	if (REPARSE_ERROR_OK != status)
// 	{
// 		hr = E_INVALIDARG;
// 	}
// 	HR_CHECK(hr);
// 
// 	if (!reUrl.Match( strInUrl.c_str(),	&mcUrl))
// 	{
// 		hr = E_INVALIDARG;
// 	}
// 	HR_CHECK(hr);
// 
//     // protocol
//     if (mcUrl.m_uNumGroups >= 1)
//     {
//         nGroupIndex = 0;
//         mcUrl.GetMatch(nGroupIndex, &szStart, &szEnd);
//         nLength = szEnd - szStart;
// 
//         if (nLength > 0)
//         {
//             str = szStart;
//             str = str.substr(0, nLength);
// 
//             // 保证为http://或https://
//             strProtocol = str;
// 		} else {
// 			strProtocol = _T("http://");
// 		}
//     }
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
// 			str = str.substr(0, nLength);
// 
// 			// 保证为http://search.microsoft.com
// 			strOutUrl.append(str);
// 		}
// 	}
// 
// 	// port
// 	if (mcUrl.m_uNumGroups >= 3)
// 	{
// 		nGroupIndex = 2;
// 		mcUrl.GetMatch(nGroupIndex, &szStart, &szEnd);
// 		nLength = szEnd - szStart;
// 
// 		if (nLength > 0)
// 		{
// 			// 不能加端口号，上层会处理错误
// 			str = szStart;
// 			str = str.substr(1, nLength);
// 
// 			port = _ttoi(str.c_str());
// 
// 			//			strOutUrl.append(str);
// 		} else {
// 			// 保证为http://search.microsoft.com:端口
// 			if ((0 != port) && (80 != port))
// 			{
// 				// 指定端口
// 				//				_stprintf_s(strPort, MAX_BUF, _T(":%d"),   port);   
// 
// 				//				strOutUrl.append(strPort);
// 			} 
// 		}
// 	}
// 	// path
// 	if (mcUrl.m_uNumGroups >= 4)
// 	{
// 		nGroupIndex = 3;
// 		mcUrl.GetMatch(nGroupIndex, &szStart, &szEnd);
// 		nLength = szEnd - szStart;
// 
// 		if (nLength > 0)
// 		{
// 			str = szStart;
// 			str = str.substr(0, nLength);
// 
// 			// 保证为http://search.microsoft.com:端口/us
// 			strOutPath.append(_T("/"));
// 			strOutPath.append(str);
// 		}
// 	}
// 
// 	// 保证最后为 http://search.microsoft.com:端口/us/
// 	st = strOutPath.rfind('/');
// 	stLength = strOutPath.size() - 1;
// 	if (st != stLength)
// 	{
// 		strOutPath.append(_T("/"));
// 	}
// 
// 
// 	// 保证参数表 ?chk=2&ch=3
// 	if (mcUrl.m_uNumGroups >= 5)
// 	{
// 		nGroupIndex = 4;
// 		mcUrl.GetMatch(nGroupIndex, &szStart, &szEnd);
// 		nLength = szEnd - szStart;
// 
// 		if (nLength > 0)
// 		{
// 			str = szStart;
// 			str = str.substr(0, nLength);
// 
// 			// 保证为http://search.microsoft.com:端口/us
// 			strOutParams = str;
// 		}
// 	}
// 
// exit:
// 	if (FAILED(hr))
// 	{
// 		//LOG_ERR(L"HttpOperator::ParseUrl exit with error 0x%08X", hr);
// 	}
// 	return hr;
// }

/**
 * 读取指定配置文件的版本号
 * @param	[in] strConfigFileName: _T("E:\\Tang\\teamwork\\Tang\\Target\\Config.xml")
 *			[inout] strSiteId: site id.
 *			[inout] strVersion: version number.
 * @return	HRESULT: return S_OK if successful.
 */
HRESULT HttpOperator::ReadSiteXml(CString strConfigFileName, CString& strSiteId, 
								  CString& strVersion, CString& strLanguage, CString& strAppId)
{
	HRESULT hr = S_OK;
	VARIANT variant;
	IXMLDOMDocument * pRecorderXML = NULL;
	VARIANT_BOOL bSuccess = VARIANT_TRUE;
	IXMLDOMNode* pNode = NULL;
	CComPtr<IXMLDOMNamedNodeMap> pDOMNamedNodeMap = NULL;
	CComPtr<IXMLDOMNode> pNamedItemNode = NULL;
	CComVariant varAttributeValue;
	CComPtr<IXMLDOMNodeList> pChildLst = NULL;
	CComPtr<IXMLDOMNode> pNextNode = NULL;

	BOOL_CHECK( (!strConfigFileName.IsEmpty()) );

	strVersion = _T("");

	::CoInitialize(NULL);
	hr =::CoCreateInstance(
		CLSID_DOMDocument, 
		NULL, 
		CLSCTX_INPROC_SERVER,
		IID_IXMLDOMDocument,
		(LPVOID*)(&pRecorderXML) );
	HR_CHECK(hr);
	ARG_CHECK(pRecorderXML);

	pRecorderXML->AddRef();
	variant.vt = VT_BSTR;
	UC_LOG(INFO) << "Config file name:" << strConfigFileName;
	variant.bstrVal = SysAllocString(strConfigFileName);
	hr = pRecorderXML->load(variant, &bSuccess);
	SysFreeString(variant.bstrVal);
	HR_CHECK(hr);
	UC_LOG(INFO) << "load xml:" << bSuccess;

	hr = pRecorderXML->selectSingleNode(NODE_NAME_UPDATE_RELEASE, &pNode);
	HR_CHECK(hr);
	CHECK_IF_NULL(pNode);

	hr = pNode->get_attributes(&pDOMNamedNodeMap);
	HR_CHECK(hr);
	if( NULL != pDOMNamedNodeMap )
	{
		// 取版本号
		hr = pDOMNamedNodeMap->getNamedItem(NODE_NAME_UPDATE_VERSION, &pNamedItemNode);
		HR_CHECK(hr);
		if( NULL != pNamedItemNode )
		{
			hr = pNamedItemNode->get_nodeValue(&varAttributeValue);
			HR_CHECK(hr);

			strVersion = varAttributeValue.bstrVal;
		}

		// 取SiteId
		pNamedItemNode = NULL;
		hr = pDOMNamedNodeMap->getNamedItem(NODE_NAME_UPDATE_SITEID, &pNamedItemNode);
		HR_CHECK(hr);
		if( NULL != pNamedItemNode )
		{
			hr = pNamedItemNode->get_nodeValue(&varAttributeValue);
			HR_CHECK(hr);

			strSiteId = varAttributeValue.bstrVal;
		}

		// 取Language
		pNamedItemNode = NULL;
		hr = pDOMNamedNodeMap->getNamedItem(NODE_NAME_UPDATE_LANGUAGE, &pNamedItemNode);
		HR_CHECK(hr);
		if( NULL != pNamedItemNode )
		{
			hr = pNamedItemNode->get_nodeValue(&varAttributeValue);
			HR_CHECK(hr);

			strLanguage = varAttributeValue.bstrVal;
		}

		// 取AppId
		pNamedItemNode = NULL;
		hr = pDOMNamedNodeMap->getNamedItem(NODE_NAME_UPDATE_APPID, &pNamedItemNode);
		HR_CHECK(hr);
		if( NULL != pNamedItemNode )
		{
			hr = pNamedItemNode->get_nodeValue(&varAttributeValue);
			HR_CHECK(hr);

			strAppId = varAttributeValue.bstrVal;
		}
	}
exit:
	if (NULL != pRecorderXML)
	{
		pRecorderXML->Release();
	}
	return hr;
}

HRESULT HttpOperator::WriteSiteXml( CString strConfigFileName, CString strAppeId/* = _T("")*/, CString strSiteId /*= _T("")*/, 
								   CString strVersion /*= _T("")*/, CString strLanguage /*= _T("")*/ )
{
	HRESULT hr = S_OK;
	VARIANT variant;
	IXMLDOMDocument * pRecorderXML = NULL;
	VARIANT_BOOL bSuccess = VARIANT_TRUE;
	IXMLDOMNode* pNode = NULL;
	CComPtr<IXMLDOMNamedNodeMap> pDOMNamedNodeMap = NULL;
	CComPtr<IXMLDOMNode> pNamedItemNode = NULL;
	CComVariant varAttributeValue;
	CComPtr<IXMLDOMNodeList> pChildLst = NULL;
	CComPtr<IXMLDOMNode> pNextNode = NULL;

	BOOL_CHECK( (!strConfigFileName.IsEmpty()) );

	::CoInitialize(NULL);
	hr =::CoCreateInstance(
		CLSID_DOMDocument, 
		NULL, 
		CLSCTX_INPROC_SERVER,
		IID_IXMLDOMDocument,
		(LPVOID*)(&pRecorderXML) );
	HR_CHECK(hr);
	ARG_CHECK(pRecorderXML);

	pRecorderXML->AddRef();
	variant.vt = VT_BSTR;
	UC_LOG(INFO) << "Config file name:" << strConfigFileName;
	variant.bstrVal = SysAllocString(strConfigFileName);
	hr = pRecorderXML->load(variant, &bSuccess);
	SysFreeString(variant.bstrVal);
	HR_CHECK(hr);
	UC_LOG(INFO) << "load xml:" << bSuccess;

	hr = pRecorderXML->selectSingleNode(NODE_NAME_UPDATE_RELEASE, &pNode);
	HR_CHECK(hr);
	CHECK_IF_NULL(pNode);

	hr = pNode->get_attributes(&pDOMNamedNodeMap);
	HR_CHECK(hr);
	if( NULL != pDOMNamedNodeMap )
	{
		// 写版本号
		if (!strVersion.IsEmpty())
		{
			hr = pDOMNamedNodeMap->getNamedItem(NODE_NAME_UPDATE_VERSION, &pNamedItemNode);
			HR_CHECK(hr);
			if( NULL != pNamedItemNode )
			{
				varAttributeValue.vt = VT_BSTR;
				varAttributeValue.bstrVal = strVersion.AllocSysString();
				hr = pNamedItemNode->put_nodeValue(varAttributeValue);
				SysFreeString(varAttributeValue.bstrVal);
				HR_CHECK(hr);
			}
		}

		// 写SiteId
		if (!strSiteId.IsEmpty())
		{
			pNamedItemNode = NULL;
			hr = pDOMNamedNodeMap->getNamedItem(NODE_NAME_UPDATE_SITEID, &pNamedItemNode);
			HR_CHECK(hr);
			if( NULL != pNamedItemNode )
			{
				varAttributeValue.vt = VT_BSTR;
				varAttributeValue.bstrVal = strSiteId.AllocSysString();
				hr = pNamedItemNode->put_nodeValue(varAttributeValue);
				SysFreeString(varAttributeValue.bstrVal);
				HR_CHECK(hr);
			}
		}

		// 写Language
		if (!strLanguage.IsEmpty())
		{
			pNamedItemNode = NULL;
			hr = pDOMNamedNodeMap->getNamedItem(NODE_NAME_UPDATE_LANGUAGE, &pNamedItemNode);
			HR_CHECK(hr);
			if( NULL != pNamedItemNode )
			{
				varAttributeValue.vt = VT_BSTR;
				varAttributeValue.bstrVal = strLanguage.AllocSysString();
				hr = pNamedItemNode->put_nodeValue(varAttributeValue);
				SysFreeString(varAttributeValue.bstrVal);
				HR_CHECK(hr);
			}
		}
		// 写AppId
		if (!strAppeId.IsEmpty())
		{
			pNamedItemNode = NULL;
			hr = pDOMNamedNodeMap->getNamedItem(NODE_NAME_UPDATE_APPID, &pNamedItemNode);
			HR_CHECK(hr);
			if( NULL != pNamedItemNode )
			{
				varAttributeValue.vt = VT_BSTR;
				varAttributeValue.bstrVal = strAppeId.AllocSysString();
				hr = pNamedItemNode->put_nodeValue(varAttributeValue);
				SysFreeString(varAttributeValue.bstrVal);
				HR_CHECK(hr);
			}
		}
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