#include "stdafx.h"
#include <strsafe.h>
#include "TangView.h"
#include "Utility.h"

/* common micro definition for tang window */
#define WD_ROUND_CORNER_SIZE    (5)

TangView::TangView()
    : m_cornerSize(WD_ROUND_CORNER_SIZE)
    , m_bRoundCorner(TRUE)
    , m_iDvalue(0)
{

}

TangView::~TangView()
{
    m_eventHandler.DisconnectIE();
}

BOOL TangView::PreTranslateMessage(MSG* pMsg)
{
    if((pMsg->message < WM_KEYFIRST || pMsg->message > WM_KEYLAST) &&
        (pMsg->message < WM_MOUSEFIRST || pMsg->message > WM_MOUSELAST))
        return FALSE;

    //to disable web browser page back forward and refresh.
    if (pMsg->message == WM_KEYDOWN && 
        (pMsg->wParam == VK_BACK || pMsg->wParam == VK_F5))
        return FALSE;

    // give HTML page a chance to translate this message
    return (BOOL)SendMessage(WM_FORWARDMSG, 0, (LPARAM)pMsg);
}

HWND TangView::Create(HWND hWndParent, _U_RECT rect, LPCTSTR szWindowName,
                      DWORD dwStyle, DWORD dwExStyle,    _U_MENUorID MenuOrID, LPVOID lpCreateParam)
{
    //get current application path
    std::wstring wstrPathValue;
    WCHAR moduleFullName[MAX_PATH] = {0};
    DWORD dwErr = ::GetModuleFileName(NULL, moduleFullName, MAX_PATH);
    wstrPathValue = moduleFullName;
    size_t loopIndex = wstrPathValue.find_last_of(L'\\');
    if (loopIndex > 0)
    {
        wstrPathValue = wstrPathValue.substr(0,loopIndex);
    }

    HWND hwnd = CWindowImpl<TangView, CAxWindow>::Create(hWndParent,
        rect,wstrPathValue.c_str(), dwStyle, dwExStyle, MenuOrID, lpCreateParam);
    IUnknown *host;
    IUnknown *service;
    IDocHostUIHandlerDispatch *uihandle;
    LRESULT hr = S_OK;
    if (AtlAxGetHost(m_hWnd,&host)==S_OK) 
    {
        hr = QueryInterface(IID_IServiceProvider,(void**)&service);
        ATLASSERT( SUCCEEDED(hr) );
        ::CoCreateInstance(CLSID_InternetSecurityManager,
            NULL, CLSCTX_INPROC_SERVER, IID_IInternetSecurityManager, (void**)&m_pSecurityMgr);
        IObjectWithSite *controlsite;
        hr = host->QueryInterface(IID_IObjectWithSite, (void**)&controlsite);
        ATLASSERT( SUCCEEDED(hr) );
        controlsite->SetSite(service);

        hr = QueryInterface(IID_IDocHostUIHandlerDispatch,(void**)&uihandle);
        ATLASSERT( SUCCEEDED(hr) );

        IAxWinHostWindow *ihost;
        hr = host->QueryInterface(IID_IAxWinHostWindow, (void**)&ihost);
        ATLASSERT( SUCCEEDED(hr) );
        hr = ihost->SetExternalUIHandler(uihandle);

        IAxWinAmbientDispatchEx *dispex;
        hr = host->QueryInterface(IID_IAxWinAmbientDispatchEx, (void**)&dispex);
        ATLASSERT( SUCCEEDED(hr) );
        dispex->SetAmbientDispatch(static_cast<IDispatch*>(this));
    }

    //IWebBrowser2* brw;
    hr = QueryControl(IID_IWebBrowser2, (void**)&m_broswer);
    ATLASSERT( SUCCEEDED(hr) );

    m_eventHandler.ConnectIE(m_broswer);

    VARIANT_BOOL var_bool;
    var_bool = VARIANT_FALSE;
    hr = m_broswer->put_RegisterAsDropTarget(var_bool); //register IE engine as no dropped support.
    ATLASSERT( SUCCEEDED(hr) );

    VARIANT var;
    var.vt = VT_I4;
    var.iVal = navNoHistory;
    hr = m_broswer->Navigate(_bstr_t(szWindowName), &var, 0, 0, 0);
    ATLASSERT( SUCCEEDED(hr) );

    return hwnd;
}

HRESULT STDMETHODCALLTYPE TangView::ShowMessage( /* [in] */ HWND hwnd, /* [in] */ __in __nullterminated LPOLESTR lpstrText, /* [in] */ __in __nullterminated LPOLESTR lpstrCaption, /* [in] */ DWORD dwType, /* [in] */ __in __nullterminated LPOLESTR lpstrHelpFile, /* [in] */ DWORD dwHelpContext, /* [out] */ LRESULT *plResult )
{
    return S_OK;
}

HRESULT STDMETHODCALLTYPE TangView::ShowHelp( /* [in] */ HWND hwnd, /* [in] */ __in __nullterminated LPOLESTR pszHelpFile, /* [in] */ UINT uCommand, /* [in] */ DWORD dwData, /* [in] */ POINT ptMouse, /* [out] */ IDispatch *pDispatchObjectHit )
{
    return S_OK;
}

HRESULT STDMETHODCALLTYPE TangView::SetSecuritySite( /* [unique][in] */ __RPC__in_opt IInternetSecurityMgrSite *pSite )
{
    HRESULT     hr = S_OK;

    if (m_pSecurityMgr) 
    {
        hr = m_pSecurityMgr->SetSecuritySite(pSite);
    }

    return hr;
}

HRESULT STDMETHODCALLTYPE TangView::GetSecuritySite( /* [out] */ __RPC__deref_out_opt IInternetSecurityMgrSite **ppSite )
{
    HRESULT     hr = S_OK;

    if (m_pSecurityMgr)
    {
        return m_pSecurityMgr->GetSecuritySite(ppSite);
    }

    return E_FAIL;
}

HRESULT STDMETHODCALLTYPE TangView::MapUrlToZone( /* [in] */ __RPC__in LPCWSTR pwszUrl, /* [out] */ __RPC__out DWORD *pdwZone, /* [in] */ DWORD dwFlags )
{
    *pdwZone = URLZONE_LOCAL_MACHINE;
    return S_OK;
}

HRESULT STDMETHODCALLTYPE TangView::SetZoneMapping( /* [in] */ DWORD dwZone, /* [in] */ __RPC__in LPCWSTR lpszPattern, /* [in] */ DWORD dwFlags )
{
    return S_OK;
}

HRESULT STDMETHODCALLTYPE TangView::GetZoneMappings( /* [in] */ DWORD dwZone, /* [out] */ __RPC__deref_out_opt IEnumString **ppenumString, /* [in] */ DWORD dwFlags )
{
    return S_OK;
}

HRESULT STDMETHODCALLTYPE TangView::GetSecurityId( /* [in] */ __RPC__in LPCWSTR pwszUrl, /* [size_is][out] */ __RPC__out_ecount_full(*pcbSecurityId) BYTE *pbSecurityId, /* [out][in] */ __RPC__inout DWORD *pcbSecurityId, /* [in] */ DWORD_PTR dwReserved )
{
    HRESULT     hr = S_OK;

    if (m_pSecurityMgr)
    {
        hr = m_pSecurityMgr->GetSecurityId(pwszUrl, pbSecurityId, pcbSecurityId, dwReserved);
    }

    return hr;
}

HRESULT STDMETHODCALLTYPE TangView::ProcessUrlAction( /* [in] */ __RPC__in LPCWSTR pwszUrl, /* [in] */ DWORD dwAction, /* [size_is][out] */ __RPC__out_ecount_full(cbPolicy) BYTE *pPolicy, /* [in] */ DWORD cbPolicy, /* [in] */ __RPC__in BYTE *pContext, /* [in] */ DWORD cbContext, /* [in] */ DWORD dwFlags, /* [in] */ DWORD dwReserved )
{
    if(pPolicy && cbPolicy>= 4)
    {
        *pPolicy = URLPOLICY_ALLOW;
        return S_OK;
    }

    if (m_pSecurityMgr)
    {
        return m_pSecurityMgr->ProcessUrlAction(pwszUrl, dwAction, pPolicy, cbPolicy, pContext, cbContext, dwFlags, dwReserved);
    }

    return E_FAIL;
}

HRESULT STDMETHODCALLTYPE TangView::QueryCustomPolicy( /* [in] */ __RPC__in LPCWSTR pwszUrl, /* [in] */ __RPC__in REFGUID guidKey, /* [size_is][size_is][out] */ __RPC__deref_out_ecount_full_opt(*pcbPolicy) BYTE **ppPolicy, /* [out] */ __RPC__out DWORD *pcbPolicy, /* [in] */ __RPC__in BYTE *pContext, /* [in] */ DWORD cbContext, /* [in] */ DWORD dwReserved )
{
    if (m_pSecurityMgr)
    {
        return m_pSecurityMgr->QueryCustomPolicy(pwszUrl, guidKey, ppPolicy, pcbPolicy, pContext, cbContext, dwReserved);
    }

    return E_FAIL;
}

HRESULT TangView::SetExternal( External *pExternal )
{
    HRESULT     hr          = S_OK;

    //CHECK_IF_NULL(pExternal);
    m_pExternal.Attach(pExternal);

    return hr;
}

HRESULT TangView::Navigate( BSTR relativePath,BOOL bLocalPage )
{
    HRESULT                        hr = S_OK;
    TCHAR*                        strUrl;
    VARIANT                     var;

    //CHECK_IF_NULL(m_broswer);
    //CHECK_IF_NULL(relativePath);

    //added by jianhua.chen for navigate
    m_eventHandler.SetAllowToNavigate(TRUE);

    //add on 2011-6-5.
    //passing no history flag to IE engine to disable browser navigate to history page.
    var.vt = VT_I4;
    var.iVal = navNoHistory;
    if (bLocalPage)
    {
        hr = Utility::GetFileFullPath(relativePath, &strUrl);
        //HR_CHECK(hr);
        hr = m_broswer->Navigate(strUrl,&var,NULL,NULL,NULL);
        //HR_CHECK(hr);
    }
    else
    {
        hr = m_broswer->Navigate(relativePath,&var,NULL,NULL,NULL);
        //HR_CHECK(hr);
    }

    return hr;
}

HRESULT TangView::GetWebBrowserInstance( IDispatch **ppBrowser )
{
    HRESULT                        hr = S_OK;

    //CHECK_IF(NULL==ppBrowser,E_INVALIDARG);
    //CHECK_IF(NULL==m_broswer,E_FAIL);
    hr = m_broswer->QueryInterface(IID_IDispatch,(void **)ppBrowser);
    //HR_CHECK(hr);
    return hr;
}

LRESULT TangView::OnSize( UINT uMsg,WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
    RECT rect;
    CRgn rgn;
    CRgn rgn_t;

    //create round corner.
    ::GetWindowRect(::GetParent(m_hWnd),&rect);
    if (m_bRoundCorner)
    {
        rgn.CreateRoundRectRgn(0,0,(rect.right-rect.left),(rect.bottom-rect.top),m_cornerSize,m_cornerSize);
    }
    else
    {
        rgn.CreateRectRgn(0,0,(rect.right-rect.left),(rect.bottom-rect.top));
    }

    /**< set Trapezoid Size */
    if (m_iDvalue > 0)
    {
        POINT Points[4];
        Points[0].x = 0;
        Points[0].y = 0;

        Points[1].x = (rect.right - rect.left);
        Points[1].y = 0;

        Points[2].x = (rect.right - rect.left);
        Points[2].y = (rect.bottom - rect.top) - 8;

        Points[3].x = 0;
        Points[3].y = (rect.bottom - rect.top) - 5 - m_iDvalue;

        rgn_t.CreatePolygonRgn(Points, 4, ALTERNATE);
        ::CombineRgn(rgn, rgn, rgn_t, RGN_AND);
    }

    ::SetWindowRgn(::GetParent(m_hWnd),(HRGN)rgn, TRUE);

    return 0;
}

HRESULT TangView::ShowRoundCorner( BOOL bShow )
{
    m_bRoundCorner = bShow;
    return S_OK;
}