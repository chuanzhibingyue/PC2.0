/******************************************************************************
* Copyright (c) 2009~2010 All Rights Reserved by
*  G-Net Integrated Service co. Ltd. 

******************************************************************************/
/**
* @file PhoneService.cpp
* @brief This class implements all the interfaces inherited from NativeActionDispatch.
* 
* @author Xiaogang Yu (xiaogang.yu@gnetis.com)
*
* @date Apr. 21 2010
*
* @version 1.0.0
* 
* @warning null
*
* Revision History
* @if  CR/PR ID    Author          Date            Major Change @endif
* @bug 

******************************************************************************/
#include "StdAfx.h"
#include "UpdateAdaptor.h"
#include <Debug.h>
#include <GlobalDefine.h>
#include <simpdata.h>

UpdateAdaptor::UpdateAdaptor()
:NativeActionDispImpl<IUpdate>(),
m_pUpdateManager(NULL)
{
	DisableSelfDelete();
}

UpdateAdaptor::UpdateAdaptor(UpdateManager* pUpdateManager)
:NativeActionDispImpl<IUpdate>()
{
	DisableSelfDelete();
	ASSERT(pUpdateManager);
	m_pUpdateManager = pUpdateManager;
}

UpdateAdaptor::~UpdateAdaptor(void)
{
}

HRESULT UpdateAdaptor::SetXmlDocument(IUnknown * pXmlDoc)
{
    HRESULT hr = S_OK;
    ARG_CHECK(m_pUpdateManager); 
    ARG_CHECK(pXmlDoc);
    hr = m_pUpdateManager->SetXmlDocument(pXmlDoc);
    HR_CHECK(hr);

exit:
    if (FAILED(hr))
    {
        //LOG_ERR(L"UpdateAdaptor::SetXmlDocument exit with error 0x%08X", hr);
    }
    return HResult_To_App_Layer( hr );
}

HRESULT UpdateAdaptor::RemoveXmlDocument( IUnknown * pXmlDoc )
{
    HRESULT hr = S_OK;
    ARG_CHECK(m_pUpdateManager); 
    ARG_CHECK(pXmlDoc);
    hr = m_pUpdateManager->RemoveXmlDocument(pXmlDoc);
    HR_CHECK(hr);

exit:
    if (FAILED(hr))
    {
        //LOG_ERR(L"UpdateAdaptor::RemoveXmlDocument exit with error 0x%08X", hr);
    }
    return HResult_To_App_Layer( hr );
}

HRESULT UpdateAdaptor::CheckUpdates(UINT uAppId, UINT uSiteId,	UINT uSkinId,
							BSTR szServerUrl, UINT uHttpPort, UINT uTimeout,BSTR szCmdLine)
{
	HRESULT hr = S_OK;
	ARG_CHECK(m_pUpdateManager); 

	hr = m_pUpdateManager->Upgrade(uAppId, uSiteId, uSkinId,
		szServerUrl, uHttpPort, uTimeout, szCmdLine);
	HR_CHECK(hr);

exit:
	if (FAILED(hr))
	{
		//LOG_ERR(L"UpdateAdaptor::CheckUpdates exit with error 0x%08X", hr);
	}
	return HResult_To_App_Layer( hr );
}

HRESULT UpdateAdaptor::Download()
{
	HRESULT hr = S_OK;
	ARG_CHECK(m_pUpdateManager); 
	hr = m_pUpdateManager->Download();
	HR_CHECK(hr);

exit:
	if (FAILED(hr))
	{
		//LOG_ERR(L"UpdateAdaptor::Download exit with error 0x%08X", hr);
	}
	return HResult_To_App_Layer( hr );
}

HRESULT UpdateAdaptor::CancelDownload()
{
	HRESULT hr = S_OK;
	ARG_CHECK(m_pUpdateManager); 
	hr = m_pUpdateManager->CancelDownload();
	HR_CHECK(hr);

exit:
	if (FAILED(hr))
	{
		//LOG_ERR(L"UpdateAdaptor::CancelDownload exit with error 0x%08X", hr);
	}
	return HResult_To_App_Layer( hr );
}

HRESULT UpdateAdaptor::Install()
{
	HRESULT hr = S_OK;
	ARG_CHECK(m_pUpdateManager); 

	hr = m_pUpdateManager->Install();
	HR_CHECK(hr);

exit:
	if (FAILED(hr))
	{
		//LOG_ERR(L"UpdateAdaptor::Install exit with error 0x%08X", hr);
	}
	return HResult_To_App_Layer( hr );
}

HRESULT UpdateAdaptor::Restart( BSTR szTangCmd )
{
	HRESULT hr = S_OK;
	ARG_CHECK(m_pUpdateManager); 

	hr = m_pUpdateManager->Restart(szTangCmd);
	HR_CHECK(hr);

exit:
	if (FAILED(hr))
	{
		//LOG_ERR(L"UpdateAdaptor::Restart exit with error 0x%08X", hr);
	}
	return HResult_To_App_Layer( hr );
}

HRESULT UpdateAdaptor::RemoveFile (
						   BSTR szFullFileName)
{
	HRESULT hr = S_OK;
	ARG_CHECK(m_pUpdateManager); 

	hr = m_pUpdateManager->RemoveFile(szFullFileName);
	HR_CHECK(hr);

exit:
	if (FAILED(hr))
	{
		//LOG_ERR(L"UpdateAdaptor::RemoveFile exit with error 0x%08X", hr);
	}
	return HResult_To_App_Layer( hr );
}

HRESULT UpdateAdaptor::RemoveFolder (
							 BSTR szFullFolderName)
{
	HRESULT hr = S_OK;
	ARG_CHECK(m_pUpdateManager); 

	hr = m_pUpdateManager->RemoveFolder(szFullFolderName);
	HR_CHECK(hr);

exit:
	if (FAILED(hr))
	{
		//LOG_ERR(L"UpdateAdaptor::RemoveFolder exit with error 0x%08X", hr);
	}
	return HResult_To_App_Layer( hr );
}

HRESULT UpdateAdaptor::UpdateUrlProtocol (
								  BSTR szProtocol,
								  BSTR szFullAppName)
{
	HRESULT hr = S_OK;
	ARG_CHECK(m_pUpdateManager); 

	hr = m_pUpdateManager->UpdateUrlProtocol(szProtocol, szFullAppName);
	HR_CHECK(hr);

exit:
	if (FAILED(hr))
	{
		//LOG_ERR(L"UpdateAdaptor::UpdateUrlProtocol exit with error 0x%08X", hr);
	}
	return HResult_To_App_Layer( hr );
}

HRESULT UpdateAdaptor::RemoveUrlProtocol (
								  BSTR szProtocol)
{
	HRESULT hr = S_OK;
	ARG_CHECK(m_pUpdateManager); 

	hr = m_pUpdateManager->RemoveUrlProtocol(szProtocol);
	HR_CHECK(hr);

exit:
	if (FAILED(hr))
	{
		//LOG_ERR(L"UpdateAdaptor::RemoveUrlProtocol exit with error 0x%08X", hr);
	}
	return HResult_To_App_Layer( hr );
}

HRESULT UpdateAdaptor::UpdateDesktopLink(
								 BSTR szName,
								 BSTR szFullAppName,
								 BSTR szCmdArgs,
								 BSTR szIco)
{
	HRESULT hr = S_OK;
	ARG_CHECK(m_pUpdateManager); 

	hr = m_pUpdateManager->UpdateDesktopLink(szName, szFullAppName, szCmdArgs, szIco);
	HR_CHECK(hr);

exit:
	if (FAILED(hr))
	{
		//LOG_ERR(L"UpdateAdaptor::UpdateDesktopLink exit with error 0x%08X", hr);
	}
	return HResult_To_App_Layer( hr );
}

HRESULT UpdateAdaptor::RemoveDesktopLink (
								  BSTR szName)
{
	HRESULT hr = S_OK;
	ARG_CHECK(m_pUpdateManager); 

	hr = m_pUpdateManager->RemoveDesktopLink(szName);
	HR_CHECK(hr);

exit:
	if (FAILED(hr))
	{
		//LOG_ERR(L"UpdateAdaptor::RemoveDesktopLink exit with error 0x%08X", hr);
	}
	return HResult_To_App_Layer( hr );
}

HRESULT UpdateAdaptor::UpdateStartMenu(
							   BSTR szName,
							   BSTR szFullAppName,
							   BSTR szCmdArgs,
							   BSTR szIco)
{
	HRESULT hr = S_OK;
	ARG_CHECK(m_pUpdateManager); 

	hr = m_pUpdateManager->UpdateStartMenu(szName, szFullAppName, szCmdArgs, szIco);
	HR_CHECK(hr);

exit:
	if (FAILED(hr))
	{
		//LOG_ERR(L"UpdateAdaptor::UpdateStartMenu exit with error 0x%08X", hr);
	}
	return HResult_To_App_Layer( hr );
}

HRESULT UpdateAdaptor::RemoveStartMenu (
								BSTR szName)
{
	HRESULT hr = S_OK;
	ARG_CHECK(m_pUpdateManager); 

	hr = m_pUpdateManager->RemoveStartMenu(szName);
	HR_CHECK(hr);

exit:
	if (FAILED(hr))
	{
		//LOG_ERR(L"UpdateAdaptor::RemoveStartMenu exit with error 0x%08X", hr);
	}
	return HResult_To_App_Layer( hr );
}

HRESULT WINAPI UpdateAdaptor::ReadCookie( BSTR szUrl, BSTR szName, BSTR* szContent )
{
	HRESULT hr = S_OK;
	//LOG_INFO(L"UpdateAdaptor::ReadCookie doing the function");

	ARG_CHECK(m_pUpdateManager); 
	CHECK_IF(szContent == NULL, E_INVALIDARG);
    *szContent = NULL;
	hr = m_pUpdateManager->ReadCookie(szUrl, szName, szContent);
	HR_CHECK(hr);

exit:
	if (FAILED(hr))
	{
		//LOG_ERR(L"UpdateAdaptor::UpdateCookie exit with error 0x%08X", hr);
	}
	return HResult_To_App_Layer( hr );
}

HRESULT UpdateAdaptor::UpdateCookie(
							BSTR szUrl, 
							BSTR szName,
							BSTR szContent)
{
	HRESULT hr = S_OK;
	//LOG_INFO(L"UpdateAdaptor::UpdateCookie doing the function");
	ARG_CHECK(m_pUpdateManager); 

	hr = m_pUpdateManager->UpdateCookie(szUrl, szName, szContent);
	HR_CHECK(hr);

exit:
	if (FAILED(hr))
	{
		//LOG_ERR(L"UpdateAdaptor::UpdateCookie exit with error 0x%08X", hr);
	}
	return HResult_To_App_Layer( hr );
}

HRESULT UpdateAdaptor::RemoveCookie (
							 BSTR szUrl, 
							 BSTR szName)
{
	HRESULT hr = S_OK;
	ARG_CHECK(m_pUpdateManager); 

	hr = m_pUpdateManager->RemoveCookie(szUrl, szName);
	HR_CHECK(hr);

exit:
	if (FAILED(hr))
	{
		//LOG_ERR(L"UpdateAdaptor::RemoveCookie exit with error 0x%08X", hr);
	}
	return HResult_To_App_Layer( hr );
}

HRESULT UpdateAdaptor::SetSiteId( UINT nSiteId )
{
	HRESULT hr = S_OK;
	ARG_CHECK(m_pUpdateManager); 

	hr = m_pUpdateManager->SetSiteId(nSiteId);
	HR_CHECK(hr);

exit:
	if (FAILED(hr))
	{
		//LOG_ERR(L"UpdateAdaptor::SetSiteId exit with error 0x%08X", hr);
	}
	return HResult_To_App_Layer( hr );
}

HRESULT UpdateAdaptor::GetSiteId( UINT* nSiteId)
{
	HRESULT hr = S_OK;
	ARG_CHECK(nSiteId);
	CHECK_IF_NULL(m_pUpdateManager); 

	hr = m_pUpdateManager->GetSiteId(nSiteId);
	HR_CHECK(hr);

exit:
	if (FAILED(hr))
	{
		//LOG_ERR(L"UpdateAdaptor::GetSiteId exit with error 0x%08X", hr);
	}
	return HResult_To_App_Layer( hr );
}

HRESULT UpdateAdaptor::SetVersion( BSTR bstrVersion )
{
	HRESULT hr = S_OK;
	ARG_CHECK(m_pUpdateManager); 

	hr = m_pUpdateManager->SetVersion(bstrVersion);
	HR_CHECK(hr);

exit:
	if (FAILED(hr))
	{
		//LOG_ERR(L"UpdateAdaptor::SetVersion exit with error 0x%08X", hr);
	}
	return HResult_To_App_Layer( hr );
}

HRESULT UpdateAdaptor::GetVersion( BSTR* pbstrVersion)
{
	HRESULT hr = S_OK;
	ARG_CHECK(pbstrVersion);
	CHECK_IF_NULL(m_pUpdateManager); 

	hr = m_pUpdateManager->GetVersion(pbstrVersion);
	HR_CHECK(hr);

exit:
	if (FAILED(hr))
	{
		//LOG_ERR(L"UpdateAdaptor::GetVersion exit with error 0x%08X", hr);
	}
	return HResult_To_App_Layer( hr );
}

HRESULT UpdateAdaptor::SetLanguage( BSTR bstrLanguage )
{
	HRESULT hr = S_OK;
	ARG_CHECK(m_pUpdateManager); 

	hr = m_pUpdateManager->SetLanguage(bstrLanguage);
	HR_CHECK(hr);

exit:
	if (FAILED(hr))
	{
		//LOG_ERR(L"UpdateAdaptor::SetLanguage exit with error 0x%08X", hr);
	}
	return HResult_To_App_Layer( hr );
}

HRESULT UpdateAdaptor::GetLanguage( BSTR* bstrLanguage )
{
	HRESULT hr = S_OK;
	ARG_CHECK(bstrLanguage);
	CHECK_IF_NULL(m_pUpdateManager); 

	hr = m_pUpdateManager->GetLanguage(bstrLanguage);
	HR_CHECK(hr);

exit:
	if (FAILED(hr))
	{
		//LOG_ERR(L"UpdateAdaptor::GetLanguage exit with error 0x%08X", hr);
	}
	return HResult_To_App_Layer( hr );
}

HRESULT UpdateAdaptor::GetInstallPath( BSTR* bstrPath )
{
	HRESULT hr = S_OK;
	ARG_CHECK(bstrPath);
	CHECK_IF_NULL(m_pUpdateManager); 

	hr = m_pUpdateManager->GetInstallPath(bstrPath);
	HR_CHECK(hr);

exit:
	if (FAILED(hr))
	{
		//LOG_ERR(L"UpdateAdaptor::GetInstallPath exit with error 0x%08X", hr);
	}
	return HResult_To_App_Layer( hr );
}

HRESULT UpdateAdaptor::GetUpdateVersion( BSTR* pbstrVersion )
{
	HRESULT hr = S_OK;
	ARG_CHECK(pbstrVersion);
	CHECK_IF_NULL(m_pUpdateManager); 

	hr = m_pUpdateManager->GetUpdateVersion(pbstrVersion);
	HR_CHECK(hr);

exit:
	if (FAILED(hr))
	{
		//LOG_ERR(L"UpdateAdaptor::GetUpdateVersion exit with error 0x%08X", hr);
	}
	return HResult_To_App_Layer( hr );
}

HRESULT UpdateAdaptor::GetDownloadDomain( BSTR* pbstrDomain )
{
    HRESULT hr = S_OK;
    ARG_CHECK(pbstrDomain);
    CHECK_IF_NULL(m_pUpdateManager); 

    hr = m_pUpdateManager->GetDownloadDomain(pbstrDomain);
    HR_CHECK(hr);

exit:
    if (FAILED(hr))
    {
        //LOG_ERR(L"UpdateAdaptor::GetDownloadDomain exit with error 0x%08X", hr);
    }
    return HResult_To_App_Layer( hr );
}
