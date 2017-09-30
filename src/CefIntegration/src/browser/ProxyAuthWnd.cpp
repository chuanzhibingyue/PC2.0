#include "browser/ProxyAuthWnd.h"
#include "browser/ClientHandler.h"

namespace cefIntegration
{
    using namespace DuiLib;

    CProxyAuthWnd::CProxyAuthWnd(CefRefPtr<ClientHandler> callback)
    {
        _callback = callback;
    }
    LPCTSTR CProxyAuthWnd::GetWindowClassName() const
    {
        return _T("proxyFrame");
    }
    UINT CProxyAuthWnd::GetClassStyle() const
    {
        return CS_DBLCLKS;
    }
    void CProxyAuthWnd::OnFinalMessage(HWND /*hWnd*/)
    {
//         delete this;
    }

    void CProxyAuthWnd::Init() {
        _pCloseBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("closeBtn")));
        _pCancelBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("cancel")));
        _pOkBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("ok")));
        _pAccountEdt = static_cast<CEditUI*>(m_pm.FindControl(_T("account")));
        _pAccountEdt->SetFocus();
        _pPwdEdt = static_cast<CEditUI*>(m_pm.FindControl(_T("password")));
        _pPwdEdt->SetPasswordMode(true);
    }

    void CProxyAuthWnd::Notify(TNotifyUI& msg){
        if (msg.sType == _T("windowinit")) OnPrepare();
        else if (msg.sType == _T("click")) {
            if (msg.pSender == _pCloseBtn
                || msg.pSender == _pCancelBtn) {
                Close();
            }
            else if (msg.pSender == _pOkBtn)
            {
                std::wstring account = _pAccountEdt->GetText().GetData();
                std::wstring pwd = _pPwdEdt->GetText().GetData();
                _callback->GetAuthInfoCallback(account, pwd);
                Close();
            }
        }
        else if (msg.sType == _T("return")) {
            std::wstring account = _pAccountEdt->GetText().GetData();
            std::wstring pwd = _pPwdEdt->GetText().GetData();
            if (!account.empty() && !pwd.empty())
            {
                _callback->GetAuthInfoCallback(account, pwd);
                Close();
            }
        }
    }

    LRESULT CProxyAuthWnd::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
        styleValue &= ~WS_CAPTION;
        ::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

        m_pm.Init(m_hWnd);
        CDialogBuilder builder;
        //CDialogBuilderCallbackEx cb;
        CControlUI* pRoot = builder.Create(_T("authSkin.xml"), (UINT)0, NULL/*&cb*/, &m_pm);
        ASSERT(pRoot && "Failed to parse XML");
        m_pm.AttachDialog(pRoot);
        m_pm.AddNotifier(this);

        Init();
        return 0;
    }

    LRESULT CProxyAuthWnd::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        bHandled = FALSE;
        return 0;
    }

    LRESULT CProxyAuthWnd::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        //_callback->Release();
        ::PostQuitMessage(0L);

        bHandled = FALSE;
        return 0;
    }

    LRESULT CProxyAuthWnd::OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        if (::IsIconic(*this)) bHandled = FALSE;
        return (wParam == 0) ? TRUE : FALSE;
    }

    LRESULT CProxyAuthWnd::OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        return 0;
    }

    LRESULT CProxyAuthWnd::OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        return 0;
    }

    LRESULT CProxyAuthWnd::OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        POINT pt; pt.x = GET_X_LPARAM(lParam); pt.y = GET_Y_LPARAM(lParam);
        ::ScreenToClient(*this, &pt);

        RECT rcClient;
        ::GetClientRect(*this, &rcClient);

        RECT rcCaption = m_pm.GetCaptionRect();
        if (pt.x >= rcClient.left + rcCaption.left && pt.x < rcClient.right - rcCaption.right \
            && pt.y >= rcCaption.top && pt.y < rcCaption.bottom) {
            CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(pt));
            if (pControl && _tcscmp(pControl->GetClass(), _T("ButtonUI")) != 0 &&
                _tcscmp(pControl->GetClass(), _T("OptionUI")) != 0 &&
                _tcscmp(pControl->GetClass(), _T("TextUI")) != 0)
                return HTCAPTION;
        }

        return HTCLIENT;
    }

    LRESULT CProxyAuthWnd::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        SIZE szRoundCorner = m_pm.GetRoundCorner();
        if (!::IsIconic(*this) && (szRoundCorner.cx != 0 || szRoundCorner.cy != 0)) {
            CDuiRect rcWnd;
            ::GetWindowRect(*this, &rcWnd);
            rcWnd.Offset(-rcWnd.left, -rcWnd.top);
            rcWnd.right++; rcWnd.bottom++;
            HRGN hRgn = ::CreateRoundRectRgn(rcWnd.left, rcWnd.top, rcWnd.right, rcWnd.bottom, szRoundCorner.cx, szRoundCorner.cy);
            ::SetWindowRgn(*this, hRgn, TRUE);
            ::DeleteObject(hRgn);
        }

        bHandled = FALSE;
        return 0;
    }

    LRESULT CProxyAuthWnd::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        // 有时会在收到WM_NCDESTROY后收到wParam为SC_CLOSE的WM_SYSCOMMAND
        if (wParam == SC_CLOSE) {
            ::PostQuitMessage(0L);
            bHandled = TRUE;
            return 0;
        }
        BOOL bZoomed = ::IsZoomed(*this);
        LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
        if (::IsZoomed(*this) != bZoomed) {
            if (!bZoomed) {
                CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("maxbtn")));
                if (pControl) pControl->SetVisible(false);
                pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("restorebtn")));
                if (pControl) pControl->SetVisible(true);
            }
            else {
                CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("maxbtn")));
                if (pControl) pControl->SetVisible(true);
                pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("restorebtn")));
                if (pControl) pControl->SetVisible(false);
            }
        }
        return lRes;
    }

    LRESULT CProxyAuthWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        LRESULT lRes = 0;
        BOOL bHandled = TRUE;
        switch (uMsg) {
        case WM_CREATE:        lRes = OnCreate(uMsg, wParam, lParam, bHandled); break;
        case WM_CLOSE:         lRes = OnClose(uMsg, wParam, lParam, bHandled); break;
        case WM_DESTROY:       lRes = OnDestroy(uMsg, wParam, lParam, bHandled); break;
        case WM_NCACTIVATE:    lRes = OnNcActivate(uMsg, wParam, lParam, bHandled); break;
        case WM_NCCALCSIZE:    lRes = OnNcCalcSize(uMsg, wParam, lParam, bHandled); break;
        case WM_NCPAINT:       lRes = OnNcPaint(uMsg, wParam, lParam, bHandled); break;
        case WM_NCHITTEST:     lRes = OnNcHitTest(uMsg, wParam, lParam, bHandled); break;
        case WM_SIZE:          lRes = OnSize(uMsg, wParam, lParam, bHandled); break;
        case WM_SYSCOMMAND:    lRes = OnSysCommand(uMsg, wParam, lParam, bHandled); break;
        default:
            bHandled = FALSE;
        }
        if (bHandled) return lRes;
        if (m_pm.MessageHandler(uMsg, wParam, lParam, lRes)) return lRes;
        return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
    }

    CProxyAuthWnd::~CProxyAuthWnd()
    {
        //_callback = NULL;
    }

}