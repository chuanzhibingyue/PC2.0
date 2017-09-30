#include "StdAfx.h"
#include "UpdateXml.h"
#include <Debug.h>
UpdateXml::UpdateXml(void)
{
	::CoInitialize(NULL);
	m_pXmlWrapper = new XmlWrapper(UPDATE_SERVICE_XML_STRING);
}

UpdateXml::~UpdateXml(void)
{
	SAFE_DELETE(m_pXmlWrapper);
	::CoUninitialize();
}

HRESULT UpdateXml::AddXmlDocument( IUnknown * pXmlDoc )
{
    HRESULT hr = S_OK;
    hr = m_pXmlWrapper->AddXmlDocument(pXmlDoc);
    HR_CHECK(hr);
exit:
    if (FAILED(hr))
    {
        ////LOG_ERR(L"UpdateXml::AddXmlDocument exit with error 0x%08X", hr);
    }
    return hr;
}

HRESULT UpdateXml::RemoveXmlDocument( IUnknown * pXmlDoc )
{
    HRESULT hr = S_OK;
    hr = m_pXmlWrapper->DeleteXmlDocument(pXmlDoc);
    HR_CHECK(hr);

exit:
    if (FAILED(hr))
    {
        ////LOG_ERR(L"WhiteboardXML::RemoveXmlDocument exit with error 0x%08X", hr);
    }
    return hr;
}


HRESULT UpdateXml::SetErrorStatus (
						UINT32 uErrorInfo,
						BSTR szErrorDetail)
{
	HRESULT hr = S_OK;

	_bstr_t bstrNodePath = _bstr_t(NODE_NAME_UPDATE_TANG_UPDATE) 
		+ _bstr_t(L"/")
		+ _bstr_t(NODE_NAME_PHONE_ERROR_INFO);

	_bstr_t bstrErrorInfo = ConvertIntToBSTR(uErrorInfo);
	_bstr_t bstrOperation = _bstr_t(NODE_NAME_UPDATE_TANG_UPDATE) 
		+ _bstr_t(L"/")
		+ _bstr_t(NODE_NAME_PHONE_ERROR_OPERATION);

	ARG_CHECK(m_pXmlWrapper);

	hr =  m_pXmlWrapper->EditNode(bstrNodePath, bstrErrorInfo);
	HR_CHECK(hr);

	hr =  m_pXmlWrapper->EditNode(bstrOperation, szErrorDetail);
	HR_CHECK(hr);

exit:
	if (FAILED(hr))
	{
		////LOG_ERR(L"UpdateXml::SetErrorStatus exit with error 0x%08X", hr);
	}
	return hr;
}
HRESULT UpdateXml::SetVersion (
					BSTR szVersion)
{
	HRESULT hr = S_OK;

	_bstr_t bstrNodePath = _bstr_t(NODE_NAME_UPDATE_TANG_UPDATE) 
		+ _bstr_t(L"/")
		+ _bstr_t(NODE_NAME_UPDATE_VERSION);

	ARG_CHECK(m_pXmlWrapper);

	hr =  m_pXmlWrapper->EditNode(bstrNodePath, szVersion);
	HR_CHECK(hr);

exit:
	if (FAILED(hr))
	{
		////LOG_ERR(L"UpdateXml::SetVersion exit with error 0x%08X", hr);
	}
	return hr;
}

HRESULT UpdateXml::SetNeedUpdate(
					  BOOL bNeedUpdate)
{
	HRESULT hr = S_OK;

	_bstr_t bstrNodePath = _bstr_t(NODE_NAME_UPDATE_TANG_UPDATE) 
		+ _bstr_t(L"/")
		+ _bstr_t(NODE_NAME_UPDATE_NEED_TO_UPDATE);

	_bstr_t bstrNeedUpdate = ConvertIntToBSTR(bNeedUpdate);

	ARG_CHECK(m_pXmlWrapper);
	hr =  m_pXmlWrapper->EditNode(bstrNodePath, bstrNeedUpdate);
	HR_CHECK(hr);

exit:
	if (FAILED(hr))
	{
		////LOG_ERR(L"UpdateXml::SetNeedUpdate exit with error 0x%08X", hr);
	}
	return hr;
}

HRESULT UpdateXml::SetNeedRestart(
					   BOOL bNeedRestart)
{
	HRESULT hr = S_OK;

	_bstr_t bstrNodePath = _bstr_t(NODE_NAME_UPDATE_TANG_UPDATE) 
		+ _bstr_t(L"/")
		+ _bstr_t(NODE_NAME_UPDATE_NEED_TO_RESTART);

	_bstr_t bstrNeedRestart = ConvertIntToBSTR(bNeedRestart);

	ARG_CHECK(m_pXmlWrapper);
	hr =  m_pXmlWrapper->EditNode(bstrNodePath, bstrNeedRestart);
	HR_CHECK(hr);

exit:
	if (FAILED(hr))
	{
		////LOG_ERR(L"UpdateXml::SetNeedRestart exit with error 0x%08X", hr);
	}
	return hr;
}
HRESULT UpdateXml::SetQueryStatus(
					   UINT uProgress)
{
	HRESULT hr = S_OK;

	_bstr_t bstrNodePath = _bstr_t(NODE_NAME_UPDATE_TANG_UPDATE) 
		+ _bstr_t(L"/")
		+ _bstr_t(NODE_NAME_UPDATE_QUERY_PROGRESS);

	_bstr_t bstrProgress = ConvertIntToBSTR(uProgress);

	ARG_CHECK(m_pXmlWrapper);
	hr =  m_pXmlWrapper->EditNode(bstrNodePath, bstrProgress);
	HR_CHECK(hr);

exit:
	if (FAILED(hr))
	{
		////LOG_ERR(L"UpdateXml::SetQueryStatus exit with error 0x%08X", hr);
	}
	return hr;
}

HRESULT UpdateXml::SetDownloadStatus(
						  UINT uProgress)
{
	HRESULT hr = S_OK;

	_bstr_t bstrNodePath = _bstr_t(NODE_NAME_UPDATE_TANG_UPDATE) 
		+ _bstr_t(L"/")
		+ _bstr_t(NODE_NAME_UPDATE_DOWNLOAD_PROGRESS);

	_bstr_t bstrProgress = ConvertIntToBSTR(uProgress);

	ARG_CHECK(m_pXmlWrapper);
	hr =  m_pXmlWrapper->EditNode(bstrNodePath, bstrProgress);
	HR_CHECK(hr);

exit:
	if (FAILED(hr))
	{
		////LOG_ERR(L"UpdateXml::SetDownloadStatus exit with error 0x%08X", hr);
	}
	return hr;
}
HRESULT UpdateXml::SetInstallStatus(
						 UINT uProgress)
{
	HRESULT hr = S_OK;

	_bstr_t bstrNodePath = _bstr_t(NODE_NAME_UPDATE_TANG_UPDATE) 
		+ _bstr_t(L"/")
		+ _bstr_t(NODE_NAME_UPDATE_INSTALL_PROGRESS);

	_bstr_t bstrProgress = ConvertIntToBSTR(uProgress);

	ARG_CHECK(m_pXmlWrapper);
	hr =  m_pXmlWrapper->EditNode(bstrNodePath, bstrProgress);
	HR_CHECK(hr);

exit:
	if (FAILED(hr))
	{
		////LOG_ERR(L"UpdateXml::SetInstallStatus exit with error 0x%08X", hr);
	}
	return hr;
}


HRESULT UpdateXml::InitXmlData(UINT uError, BSTR detail, BOOL bNeedToUpdate, BOOL bNeedToRestart,
							   UINT uQueryProgress, UINT uDownloadProgress, UINT uInstallProgress)
{
	// 该函数内部将会议的状态节点和错误节点都创建好，以后的操作只是更新数据。
	HRESULT hr = S_OK;
	_bstr_t bstrNodePath;
	ARG_CHECK(m_pXmlWrapper);

	hr = SetErrorStatus(uError, detail);
	HR_CHECK(hr);
	hr = SetNeedUpdate(bNeedToUpdate);
	HR_CHECK(hr);
	hr = SetNeedRestart(bNeedToRestart);
	HR_CHECK(hr);
	hr = SetQueryStatus(uQueryProgress);
	HR_CHECK(hr);
	hr = SetDownloadStatus(uDownloadProgress);
	HR_CHECK(hr);
	hr = SetInstallStatus(uInstallProgress);
	HR_CHECK(hr);

exit:
	if (FAILED(hr))
	{
		////LOG_ERR(L"UpdateXml::InitXmlData exit with error 0x%08X", hr);
	}
	return hr;
}
