/*
* Proxy.h
*
*  Created on: 2015.11.10
*      Author: he zhuo
*/

#ifndef PROXYAUTHWND_H_
#define PROXYAUTHWND_H_

#include "include/cef_client.h"
#include "UIlib.h"

namespace cefIntegration
{
    class ClientHandler;

    using namespace DuiLib;

    class CProxyAuthWnd : public CWindowWnd, public INotifyUI
    {
    public:
        CProxyAuthWnd(CefRefPtr<ClientHandler> callback);
        ~CProxyAuthWnd();
        LPCTSTR GetWindowClassName() const;
        UINT GetClassStyle() const;
        void OnFinalMessage(HWND /*hWnd*/);

        void Init();

        void OnPrepare() {
        }

        void Notify(TNotifyUI& msg);

        LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

        LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
        LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

        LRESULT OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

        LRESULT OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
        LRESULT OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

        LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

        LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

        LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
        LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

    public:
        CPaintManagerUI m_pm;
    private:
        CButtonUI* _pCloseBtn;
        CButtonUI* _pCancelBtn;
        CButtonUI* _pOkBtn;
        CEditUI* _pAccountEdt;
        CEditUI* _pPwdEdt;

        CefRefPtr<ClientHandler> _callback;
    };
}

#endif //AUTHCALLBACK_H_