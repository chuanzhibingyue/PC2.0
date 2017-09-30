/******************************************************************************
* Copyright (c) 2009~2010 All Rights Reserved by
*  G-Net Integrated Service co. Ltd. 

******************************************************************************/
/**
* @file UpdateAdaptor.h
* @brief 升级操作包装类，供应用层调用1
* 
* @author Wei Chen (wei.chen@gnetis.com)
*
* @date Jun 17 2010
*
* @version 1.0.0
* 
* @warning null
*
* Revision History
* @if  CR/PR ID    Author          Date            Major Change @endif
* @bug 

******************************************************************************/

/*! \addtogroup Utility
* 提供UI使用的一些工具集合，如果有和业务会议、IM无关的业务，请来这里查找
*  @{
*/

#pragma once

#include "UCUpdate.h"
#include <NativeActionDispImpl.h>
#include "..\..\Common\Inc\UpdateManager.h"
/**
* @class UpdateAdaptor
* @brief 给UI自动更新相关支持的功能
* 
* @author Wei Chen
* @version 1.1.0
*/
class UpdateAdaptor : public NativeActionDispImpl<IUpdate>
{
public:
	UpdateAdaptor();
	UpdateAdaptor(UpdateManager* pUpdateManager);
	~UpdateAdaptor(void);

	HRESULT WINAPI SetXmlDocument( IUnknown * pXmlDoc );
	HRESULT WINAPI RemoveXmlDocument( IUnknown * pXmlDoc );
	HRESULT WINAPI CheckUpdates( UINT uAppId, UINT uSiteId, UINT uSkinId, 
		BSTR szServerUrl, UINT uHttpPort, UINT uTimeout,BSTR szCmdLine );
	/**
    * @brief 下载升级包
    * @return HRESULT return S_OK if successful, otherwise return error code.
    */
	HRESULT WINAPI Download();
	HRESULT WINAPI CancelDownload();
	HRESULT WINAPI Install();
	HRESULT WINAPI Restart( BSTR szTangCmd );
	HRESULT WINAPI RemoveFile( BSTR szFullFileName );
	HRESULT WINAPI RemoveFolder( BSTR szFullFolderName );
	HRESULT WINAPI UpdateUrlProtocol( BSTR szProtocol,	BSTR szFullAppName );
	HRESULT WINAPI RemoveUrlProtocol( BSTR szProtocol );
	HRESULT WINAPI UpdateDesktopLink( BSTR szName, BSTR szFullAppName, BSTR szCmdArgs, BSTR szIco );
	HRESULT WINAPI RemoveDesktopLink ( BSTR szName);
	HRESULT WINAPI UpdateStartMenu( BSTR szName, BSTR szFullAppName, BSTR szCmdArgs, BSTR szIco );
	HRESULT WINAPI RemoveStartMenu ( BSTR szName );
	HRESULT WINAPI ReadCookie( BSTR szUrl, BSTR szName, BSTR* szContent );
	HRESULT WINAPI UpdateCookie( BSTR szUrl, BSTR szName, BSTR szContent );
	HRESULT WINAPI RemoveCookie( BSTR szUrl, BSTR szName );
	HRESULT WINAPI SetSiteId( UINT nSiteId );
	HRESULT WINAPI GetSiteId( UINT* nSiteId );
	HRESULT WINAPI SetVersion( BSTR bstrVersion );
	HRESULT WINAPI GetVersion( BSTR* pbstrVersion );
	HRESULT WINAPI SetLanguage( BSTR bstrLanguage );
	HRESULT WINAPI GetLanguage( BSTR* bstrLanguage );
	HRESULT WINAPI GetInstallPath( BSTR* bstrPath );
	HRESULT WINAPI GetUpdateVersion( BSTR* pbstrVersion );
    HRESULT WINAPI GetDownloadDomain( BSTR* pbstrDomain );

private:
	UpdateManager* m_pUpdateManager;
};

/*! @} */