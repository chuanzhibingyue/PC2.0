#include "StdAfx.h"
#include "IeEvent.h"
#include <exdispid.h>

CDWebEventSink::CDWebEventSink(void)
{
	m_dwRef = 1;
	m_pBroswer2 = NULL;
	m_dwCookie = 0;
	m_strCurrentUrl = L"";
	m_bAllowNavigate = TRUE;
}

CDWebEventSink::~CDWebEventSink(void)
{

}

STDMETHODIMP CDWebEventSink::QueryInterface(REFIID riid,void **ppvObject)
{
	if(IsBadWritePtr(ppvObject,sizeof(void*))) return E_POINTER;

	*ppvObject=NULL;

	if(IsEqualIID(riid,IID_IUnknown) || IsEqualIID(riid,IID_IDispatch) || IsEqualIID(riid,DIID_DWebBrowserEvents2))
	{
		AddRef();
		*ppvObject=this;
		return S_OK;
	}
	else
	{
		return E_NOINTERFACE;
	}
}

STDMETHODIMP_(ULONG) CDWebEventSink::AddRef()
{
	InterlockedIncrement(&m_dwRef);
	return m_dwRef;
}

STDMETHODIMP_(ULONG) CDWebEventSink::Release()
{
	InterlockedDecrement(&m_dwRef);
	if (m_dwRef == 0)
		delete this;
	return m_dwRef;
}

// We don't need to implement the next three methods because we are just a pure event sink
// We only care about Invoke() which is what IE calls to notify us of events

STDMETHODIMP CDWebEventSink::GetTypeInfoCount(UINT *pctinfo)
{
	UNREFERENCED_PARAMETER(pctinfo);
	*pctinfo=0;
	return S_OK;
}

STDMETHODIMP CDWebEventSink::GetTypeInfo(UINT iTInfo,LCID lcid,ITypeInfo **ppTInfo)
{
	UNREFERENCED_PARAMETER(iTInfo);
	UNREFERENCED_PARAMETER(lcid);
	UNREFERENCED_PARAMETER(ppTInfo);

	*ppTInfo = NULL;
	return E_NOTIMPL;
}

STDMETHODIMP CDWebEventSink::GetIDsOfNames(REFIID riid,LPOLESTR *rgszNames,UINT cNames,LCID lcid,DISPID *rgDispId)
{
	UNREFERENCED_PARAMETER(riid);
	UNREFERENCED_PARAMETER(rgszNames);
	UNREFERENCED_PARAMETER(cNames);
	UNREFERENCED_PARAMETER(lcid);
	UNREFERENCED_PARAMETER(rgDispId);

	return E_NOTIMPL;
}

//void CDWebEventSink::OnBeforeNavigate2(IDispatch* pDisp, VARIANT* URL, VARIANT* Flags,
//									   VARIANT* TargetFrameName, VARIANT* PostData,
//									   VARIANT* Headers, VARIANT_BOOL* Cancel)
//{
//	//CString sURL = URL->bstrVal;
//	// You can set *Cancel to VARIANT_TRUE to stop the   
//	// navigation from happening. For example, to stop   
//	// navigates to evil tracking companies like doubleclick.net:  
//
//	//if ( sURL.Find ( _T("doubleclick.net") ) > 0 )
//		*Cancel = VARIANT_TRUE;
//}

// This is called by IE to notify us of events
// Full documentation about all the events supported by DWebBrowserEvents2 can be found at
// http://msdn.microsoft.com/en-us/library/aa768283(VS.85).aspx
STDMETHODIMP CDWebEventSink::Invoke(DISPID dispIdMember,REFIID riid,LCID lcid,WORD wFlags,DISPPARAMS *pDispParams,VARIANT *pVarResult,EXCEPINFO *pExcepInfo,UINT *puArgErr)
{
	UNREFERENCED_PARAMETER(lcid);
	UNREFERENCED_PARAMETER(wFlags);
	UNREFERENCED_PARAMETER(pVarResult);
	UNREFERENCED_PARAMETER(pExcepInfo);
	UNREFERENCED_PARAMETER(puArgErr);

	//if(!IsEqualIID(riid,IID_NULL)) return DISP_E_UNKNOWNINTERFACE; // riid should always be IID_NULL

	switch (dispIdMember)
	{
	case DISPID_BEFORENAVIGATE2:
		//OG_INFO(L"[CDWebEventSink] Get ie control DISPID_BEFORENAVIGATE2 event");
		{
			if (pDispParams->cArgs >= 5 && pDispParams->rgvarg[5].vt == (VT_BYREF|VT_VARIANT))
			{ 
				CComVariant varURL(*pDispParams->rgvarg[5].pvarVal); 
				varURL.ChangeType(VT_BSTR); //转化要访问的网址为char *型 
				m_strCurrentUrl = varURL.bstrVal;
				//LOG_INFO(L"[CDWebEventSink] ie control want to navigate to:[%s]",m_strCurrentUrl.c_str());
			}
			//判断要跳转到的页面
			if(!m_bAllowNavigate)
			{
				//LOG_INFO(L"[CDWebEventSink] already navigate to main html");
				*pDispParams->rgvarg[0].pboolVal = TRUE;
			}
#define MAIN_HTML L"main.html"
			size_t szIndex = m_strCurrentUrl.find(MAIN_HTML);
			if(szIndex != std::wstring::npos)
			{
				//LOG_INFO(L"[CDWebEventSink] navigate to main html");
				m_bAllowNavigate = FALSE;
			}
		}
		//LOG_ERR(L"handle BEFORENAVIGATE2 event,url is:[%s]",*pDispParams->rgvarg[1].pbstrVal);
		//OnBeforeNavigate2(
		//	(IDispatch*)pDispParams->rgvarg[0].byref,
		//	&(pDispParams->rgvarg[1]),
		//	&pDispParams->rgvarg[2],
		//	&(pDispParams->rgvarg[3]),
		//	&(pDispParams->rgvarg[4]),
		//	&(pDispParams->rgvarg[5]),
		//	pDispParams->rgvarg[6].pboolVal
		//	);
		break;
	//case DISPID_NAVIGATECOMPLETE2:
	//	if (NULL!=m_lpfnNavigateComplete2)
	//	{
	//		m_lpfnNavigateComplete2(
	//			(IDispatch*)pDispParams->rgvarg[0].byref,
	//			(LPOLESTR)pDispParams->rgvarg[1].pbstrVal
	//			);
	//	}
	//	break;
	default:
		break;
	}

	return S_OK;
}

HRESULT CDWebEventSink::ConnectIE(IWebBrowser2* pBroswer)
{
	HRESULT hr = S_OK;

	//LOG_INFO(L"[CDWebEventSink] ConnectIE start");

	//ARG_CHECK(pBroswer);

	m_pBroswer2 = pBroswer;

	hr = AtlAdvise(m_pBroswer2, (IUnknown *)this, DIID_DWebBrowserEvents2, &m_dwCookie);

exit:
	if(FAILED(hr))
	{
		//LOG_ERR(L"[CDWebEventSink] ConnectIE failed with error:0x%08X",hr);
	}

	return hr;
}

HRESULT CDWebEventSink::DisconnectIE(void)
{
	HRESULT hr = S_OK;
	
	//LOG_INFO(L"[CDWebEventSink] DisconnectIE start");

	if (m_dwCookie != 0 && m_pBroswer2 !=NULL)
	{
		hr=AtlUnadvise(m_pBroswer2, DIID_DWebBrowserEvents, m_dwCookie);
		m_dwCookie = 0;
	}


exit:
	if(FAILED(hr))
	{
		//LOG_ERR(L"[CDWebEventSink] DisconnectIE failed with error:0x%08X",hr);
	}

	return hr;
}

void CDWebEventSink::SetAllowToNavigate(BOOL bAllow)
{
	m_bAllowNavigate = bAllow;
}