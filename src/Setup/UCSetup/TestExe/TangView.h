#pragma once
#include <atlbase.h>
#include <atlapp.h>
#include "IeEvent.h"
#include "IDocHostUIHandlerDispImpl.h"
#include "External.h"
#include <comdef.h>
#include <comdefsp.h>

class TangView : public CWindowImpl<TangView, CAxWindow>,
    public CComObjectRootEx<CComSingleThreadModel>,
    public IDispatchImpl<IInternetSecurityManager>,
    public IDispatchImpl<IDocHostShowUI>,
    public IDocHostUIHandlerDispImpl<TangView>,
    public IServiceProviderImpl<TangView>
{
public:
    DECLARE_WND_SUPERCLASS(NULL, CAxWindow::GetWndClassName())

    TangView();
    ~TangView();

    BOOL PreTranslateMessage(MSG* pMsg);

    BEGIN_MSG_MAP(TangView)
        MESSAGE_HANDLER(WM_SIZE, OnSize)
    END_MSG_MAP()

    BEGIN_COM_MAP(TangView)
        COM_INTERFACE_ENTRY(IServiceProvider)
        COM_INTERFACE_ENTRY_IID(IID_IInternetSecurityManager, IInternetSecurityManager)
        COM_INTERFACE_ENTRY_IID(IID_IDocHostUIHandlerDispatch, IDocHostUIHandlerDispatch)
        COM_INTERFACE_ENTRY_IID(IID_IDocHostShowUI, IDocHostShowUI)
    END_COM_MAP()

    BEGIN_SERVICE_MAP(TangView)
        SERVICE_ENTRY(IID_IInternetSecurityManager)
        SERVICE_ENTRY(IID_IServiceProvider)
    END_SERVICE_MAP()

    HWND Create(HWND hWndParent, _U_RECT rect = NULL, LPCTSTR szWindowName = NULL,
        DWORD dwStyle = 0, DWORD dwExStyle = 0, _U_MENUorID MenuOrID = 0U, LPVOID lpCreateParam = NULL);
    LRESULT OnSize(UINT uMsg,WPARAM wParam, LPARAM lParam, BOOL& bHandled);

    //Interface IDocHostShowUI
public:
    virtual HRESULT STDMETHODCALLTYPE ShowMessage( 
        /* [in] */ HWND hwnd,
        /* [in] */ LPOLESTR lpstrText,
        /* [in] */ LPOLESTR lpstrCaption,
        /* [in] */ DWORD dwType,
        /* [in] */ LPOLESTR lpstrHelpFile,
        /* [in] */ DWORD dwHelpContext,
        /* [out] */ LRESULT *plResult);

    virtual HRESULT STDMETHODCALLTYPE ShowHelp( 
        /* [in] */ HWND hwnd,
        /* [in] */ LPOLESTR pszHelpFile,
        /* [in] */ UINT uCommand,
        /* [in] */ DWORD dwData,
        /* [in] */ POINT ptMouse,
        /* [out] */ IDispatch *pDispatchObjectHit);

    //Interface InternetSecurityManager
public:
    virtual HRESULT STDMETHODCALLTYPE SetSecuritySite( 
        /* [unique][in] */ __RPC__in_opt IInternetSecurityMgrSite *pSite);

    virtual HRESULT STDMETHODCALLTYPE GetSecuritySite( 
        /* [out] */ __RPC__deref_out_opt IInternetSecurityMgrSite **ppSite);

    virtual HRESULT STDMETHODCALLTYPE MapUrlToZone( 
        /* [in] */ __RPC__in LPCWSTR pwszUrl,
        /* [out] */ __RPC__out DWORD *pdwZone,
        /* [in] */ DWORD dwFlags);

    virtual HRESULT STDMETHODCALLTYPE GetSecurityId( 
        /* [in] */ __RPC__in LPCWSTR pwszUrl,
        /* [size_is][out] */ __RPC__out_ecount_full(*pcbSecurityId) BYTE *pbSecurityId,
        /* [out][in] */ __RPC__inout DWORD *pcbSecurityId,
        /* [in] */ DWORD_PTR dwReserved);

    virtual HRESULT STDMETHODCALLTYPE ProcessUrlAction( 
        /* [in] */ __RPC__in LPCWSTR pwszUrl,
        /* [in] */ DWORD dwAction,
        /* [size_is][out] */ __RPC__out_ecount_full(cbPolicy) BYTE *pPolicy,
        /* [in] */ DWORD cbPolicy,
        /* [in] */ __RPC__in BYTE *pContext,
        /* [in] */ DWORD cbContext,
        /* [in] */ DWORD dwFlags,
        /* [in] */ DWORD dwReserved);

    virtual HRESULT STDMETHODCALLTYPE QueryCustomPolicy( 
        /* [in] */ __RPC__in LPCWSTR pwszUrl,
        /* [in] */ __RPC__in REFGUID guidKey,
        /* [size_is][size_is][out] */ __RPC__deref_out_ecount_full_opt(*pcbPolicy) BYTE **ppPolicy,
        /* [out] */ __RPC__out DWORD *pcbPolicy,
        /* [in] */ __RPC__in BYTE *pContext,
        /* [in] */ DWORD cbContext,
        /* [in] */ DWORD dwReserved);

    virtual HRESULT STDMETHODCALLTYPE SetZoneMapping( 
        /* [in] */ DWORD dwZone,
        /* [in] */ __RPC__in LPCWSTR lpszPattern,
        /* [in] */ DWORD dwFlags);

    virtual HRESULT STDMETHODCALLTYPE GetZoneMappings( 
        /* [in] */ DWORD dwZone,
        /* [out] */ __RPC__deref_out_opt IEnumString **ppenumString,
        /* [in] */ DWORD dwFlags);

public:
    HRESULT SetExternal(External* pExternal);
    HRESULT Navigate(BSTR bstrURL, BOOL bLocalPage = TRUE);
    HRESULT GetWebBrowserInstance(IDispatch** ppBrowser);
    HRESULT ShowRoundCorner(BOOL bIsShow);

public:
    CComPtr<IDispatch> m_pExternal;

private:
    int             m_cornerSize;
    BOOL            m_bRoundCorner;
    int             m_iDvalue; /**< 梯形窗体两边之差 */

    CComPtr<IInternetSecurityManager> m_pSecurityMgr;
    CComPtr<IWebBrowser2> m_broswer;
    CDWebEventSink m_eventHandler;
};
