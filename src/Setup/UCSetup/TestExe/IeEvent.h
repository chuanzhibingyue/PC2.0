#pragma once

#include <ExDisp.h>
#include <Mshtml.h>
#include <atlbase.h>
#include <atlcom.h>
#include <string>
#include <comdef.h>
#include <comdefsp.h>


class CDWebEventSink : public DWebBrowserEvents2
{
public:
	CDWebEventSink();
	~CDWebEventSink();
protected:
	LONG m_dwRef;

	//void OnBeforeNavigate2(IDispatch* pDisp, VARIANT* URL, VARIANT* Flags,
	//	VARIANT* TargetFrameName, VARIANT* PostData,
	//	VARIANT* Headers, VARIANT_BOOL* Cancel);
public:
	STDMETHODIMP QueryInterface(REFIID riid,void **ppvObject);
	STDMETHODIMP_(ULONG) AddRef();
	STDMETHODIMP_(ULONG) Release();
	// IDispatch methods
	STDMETHODIMP GetTypeInfoCount(UINT *pctinfo);
	STDMETHODIMP GetTypeInfo(UINT iTInfo,LCID lcid,ITypeInfo **ppTInfo);
	STDMETHODIMP GetIDsOfNames(REFIID riid,LPOLESTR *rgszNames,UINT cNames,LCID lcid,DISPID *rgDispId);
	STDMETHODIMP Invoke(DISPID dispIdMember,REFIID riid,LCID lcid,WORD wFlags,DISPPARAMS *pDispParams,VARIANT *pVarResult,EXCEPINFO *pExcepInfo,UINT *puArgErr);

public:
	HRESULT ConnectIE(IWebBrowser2* pBroswer);
	HRESULT DisconnectIE(void);
	void SetAllowToNavigate(BOOL bAllow);

private:
	IWebBrowser2* m_pBroswer2;
	DWORD m_dwCookie;

	std::wstring m_strCurrentUrl;
	BOOL m_bAllowNavigate;
};

