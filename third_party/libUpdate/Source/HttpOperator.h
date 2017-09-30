/******************************************************************************
* Copyright (c) 2009~2010 All Rights Reserved by
*  G-Net Integrated Service co. Ltd. 

******************************************************************************/
/**
* @file HttpOperator.h 
* @brief 封装HTTP操作
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
#include "UpdateInfoDef.h"
#include <Debug.h>
#include <Log.h>
#include <string>
#include <WinInet.h>
#pragma comment(lib, "Wininet.lib")
#include "FolderOperator.h"
// 错误码
enum ResultErrorCode
{
	ERROR_PARAMETER				= 1,		// 参数错误
	ERROR_SERVER_EXCEPTION		= 3,		// 服务器异常
	ERROR_CREATE_FILE_FAILED	= 4,		// 创建下载文件失败
	ERROR_OPERATION_SUCCESS
};

class IDownloadCallback
{
public:
	virtual BOOL IsCancel() = 0;
	// Percent：文件下载百分比
	virtual HRESULT DoResult(UINT Percent) = 0;
};

class HttpOperator
{
public:
	/**
	 * 查询安装文件list
	 * @param [in] strUrl: 升级服务器URL
			  [in] strHttpPort: 升级服务器端口
			  [in] strAppId: AppId
			  [in] strSiteId: SiteId
			  [in] strSkinId: SkinId
			  [in] strConfigFileName: version配置文件
			  [in][out] fl: 需要升级的文件列表
	 * @return HRESULT: return S_OK if successful.
	 */
	HRESULT QueryInstallList(CString strUrl, CString strHttpPort, CString strAppId,
		CString strSiteId,	CString strSkinId, CString strConfigFileName, 
		CString strVersion, DWORD& dwResult);

	/**
	 * 请求下载文件列表
	 * @param [in] strUrl: 升级服务器URL
			  [in] strHttpPort: 升级服务器端口
			  [in] strZipFileName: 本地下载的zip文件名
			  [in] fl: 需要升级的文件列表
	 * @return HRESULT: return S_OK if successful.
	 */
	HRESULT RequestDownload(CString strUrl, CString strHttpPort, 
		CString strFileName, UpdateConfig& fl, DWORD& dwResult, 	
		IDownloadCallback*	pMsgProvider, CString strUrlDown, CString downLoadReleaseId);

	/**
	 * 读取本地配置文件，填充UpdateConfig
	 * @param	[in] strConfigFileName: 本地配置文件
				[in][out] fl: 文件列表
	 * @return HRESULT: return S_OK if successful.
	 */
	HRESULT ReadConfig(CString strConfigFileName, UpdateConfig& fl);

	HRESULT ReadNotes(CString strConfigFileName, NotesCfg& notes);

	UpdateConfig ReadBaseConfig(CString strConfigFileName, HRESULT& hr);

	/**
	 * 写本地配置文件
	 * @param	[in] strConfigFileName: 本地配置文件
				[in] fl: 文件列表
	 * @return HRESULT: return S_OK if successful.
	 */
	HRESULT WriteConfig(CString strConfigFileName, UpdateConfig& fl);

	/**
	 * 读取指定配置文件的Siteid和version
	 * @param	[in] strConfigFileName: _T("E:\\Tang\\teamwork\\Tang\\Target\\Config.xml")
	 *			[inout] strSiteId: site id.
	 *			[inout] strVersion: version number.
	 * @return	HRESULT: return S_OK if successful.
	 */
	HRESULT ReadSiteXml(CString strConfigFileName, 
		CString& strSiteId, 
		CString& strVersion, 
		CString& strLanguage,
		CString& strAppId);

	/**
	* 写入指定配置文件的Siteid和version
	 * @param	[in] strConfigFileName: _T("E:\\Tang\\teamwork\\Tang\\Target\\Config.xml")
	 *			[inout] strSiteId: site id.
	 *			[inout] strVersion: version number.
	 * @return	HRESULT: return S_OK if successful.
	 */
	HRESULT WriteSiteXml(CString strConfigFileName, CString strAppId = _T(""), CString strSiteId = _T(""), 
		CString strVersion = _T(""), CString strLanguage = _T(""));

// 	/**
// 	 * 解析URL
// 	 * @param	[in] strConfigFileName: _T("E:\\Tang\\teamwork\\Tang\\Target\\Config.xml")
// 	 *			[out] strVersion: 4.3
// 	 * @return	HRESULT: return S_OK if successful.
// 	 */
// 	HRESULT ParseUrl(std::wstring strInUrl, INTERNET_PORT& port, std::wstring& strProtocol, 
// 		std::wstring& strOutUrl, std::wstring& strOutPath, std::wstring& strOutParams);
private:
	
	/**
	 * 向版本服务器发起请求
	 * @param	[in] strUrl: 升级服务器URL
				[in] strFileName: 请求成功后写入的本地文件名
				[in][out] dwResult: 请求结果
	 * @return HRESULT: return S_OK if successful.
	 */
	HRESULT Request(CString strUrl, CString strFileName, DWORD& dwResult);

private:
	CFolderOperator m_folder;

public:
    static BOOL m_bServerTellNeedUpdate;
};