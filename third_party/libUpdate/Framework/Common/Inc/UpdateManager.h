/******************************************************************************
* Copyright (c) 2009~2010 All Rights Reserved by
*  G-Net Integrated Service co. Ltd. 

******************************************************************************/
/**
* @file IUpdateManager.h 
* @brief UpdateManager½Ó¿Ú
* 
* @author Wei Chen (wei.chen@gnetis.com)
*
* @date Jun 18 2010
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
#include <imessageprovider.h>
#include <IExternal.h>

typedef void (CALLBACK * CheckUpdateCallBack)(void* userdata, int type, BOOL isNeedUpdate, const std::wstring& version, const std::wstring& notes);
typedef void (CALLBACK * ProgressCallBack)(void* userdata, int type, int value);
typedef void (CALLBACK * ErrorCallBack)(void* userdata, int type, int err, const std::wstring& version);

class UpdateManager
{
public:
	UpdateManager() {
	}

	virtual ~UpdateManager() {

	}

	virtual HRESULT Init(void* userdata,
		CheckUpdateCallBack checkCallBack,
		ProgressCallBack processCallBack,
		ErrorCallBack errCallBack,
		BOOL isBee) = 0;

	virtual HRESULT SetXmlDocument( IUnknown * pXmlDoc ) = 0;
	virtual HRESULT RemoveXmlDocument( IUnknown * pXmlDoc ) = 0;
	virtual HRESULT CheckUpdates(UINT uAppId, UINT uSiteId, UINT uSkinId,
		BSTR szServerUrl, UINT uHttpPort, UINT uTimeout, BSTR szCmdLine) = 0;
	virtual HRESULT Upgrade( UINT uAppId, UINT uSiteId, UINT uSkinId,
		BSTR szServerUrl, UINT uHttpPort, UINT uTimeout,BSTR szCmdLine) = 0;
	virtual HRESULT RealUpdate(BSTR szParam) = 0;
	virtual HRESULT CheckRunningClient(int *pCount) = 0;
	virtual HRESULT Download() = 0;
	virtual HRESULT CancelDownload() = 0;
	virtual HRESULT Install() = 0;
	virtual HRESULT Restart( BSTR szURLProtocol ) = 0;
	virtual HRESULT RemoveFile( BSTR szFullFileName ) = 0;
	virtual HRESULT RemoveFolder( BSTR szFullFolderName ) = 0;
	virtual HRESULT UpdateUrlProtocol( BSTR szProtocol,	BSTR szFullAppName ) = 0;
	virtual HRESULT RemoveUrlProtocol( BSTR szProtocol ) = 0;
	virtual HRESULT UpdateDesktopLink( BSTR szName, BSTR szFullAppName, BSTR szCmdArgs, BSTR szIco ) = 0;
	virtual HRESULT RemoveDesktopLink ( BSTR szName) = 0;
	virtual HRESULT UpdateStartMenu( BSTR szName, BSTR szFullAppName, BSTR szCmdArgs, BSTR szIco ) = 0;
	virtual HRESULT RemoveStartMenu ( BSTR szName ) = 0;
	virtual HRESULT ReadCookie( BSTR szUrl, BSTR szName, BSTR* szContent ) = 0;
	virtual HRESULT UpdateCookie( BSTR szUrl, BSTR szName, BSTR szContent ) = 0;
	virtual HRESULT RemoveCookie( BSTR szUrl, BSTR szName ) = 0;
	virtual HRESULT SetSiteId( UINT nSiteId ) = 0;
	virtual HRESULT GetSiteId( UINT* nSiteId ) = 0;
	virtual HRESULT SetVersion( BSTR bstrVersion ) = 0;
	virtual HRESULT GetVersion( BSTR* pbstrVersion ) = 0;
	virtual HRESULT SetLanguage( BSTR bstrLanguage ) = 0;
	virtual HRESULT GetLanguage( BSTR* bstrLanguage ) = 0;
	virtual HRESULT GetInstallPath( BSTR* bstrPath ) = 0;
	virtual HRESULT GetUpdateVersion( BSTR* pbstrVersion ) = 0;
	virtual HRESULT GetAppId( UINT* nAppId ) = 0;
	virtual HRESULT SetAppId( UINT nAppId ) = 0;
	virtual HRESULT SetUpdateUrl_Version(BSTR szServerUrl) = 0;
	virtual HRESULT SetUpdateUrl_PackageInfo(BSTR szServerUrl) = 0;
	virtual HRESULT SetUpdateUrl_PackageDwon(BSTR szServerUrl) = 0;
	virtual HRESULT SetLoginInfo(BSTR szInfo) = 0;
    virtual HRESULT GetDownloadDomain( BSTR* pbstrDomain ) = 0;
protected:
	virtual HRESULT UnInit() = 0;
};