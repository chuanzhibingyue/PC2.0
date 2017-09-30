/*
 * ClientHandler.h
 *
 *  Created on: June 27, 2015
 *      Author: gang wang
 */

#ifndef CLIENT_HANDLER_H_
#define CLIENT_HANDLER_H_

#include <set>

#include "include/cef_client.h"
#include "Delegate.h"

extern HINSTANCE hModuleInst;

namespace cefIntegration {

class CefClient;

class ClientHandler : public ::CefClient,
                      public CefLifeSpanHandler,
                      public CefLoadHandler,
                      public CefDisplayHandler, 
                      public CefRenderHandler,
                      public CefContextMenuHandler,
					  public CefRequestHandler,
					  public CefDragHandler,
                      public CefKeyboardHandler{
public:
    //ClientHandler methods
    ClientHandler(HINSTANCE hInstance, cefIntegration::CefClient *client);
    virtual ~ClientHandler();

	void Quit();
	void CloseAllWindowExcept(bool hasExcept = false, HWND hwnd = 0);
	void ExecuteJSForAllBrowsers(const std::wstring &code);
    void RegisterBrowserDelegate(BrowserDelegate* browserDelegate);
    void SetMainWnd(HWND mainWnd) {_mainWnd = mainWnd;}

    void SetRenderHandler(CefWindowHandle hwnd, CefRefPtr<CefRenderHandler> handler) {
    	_renderHandlers[hwnd] = handler;
    }

    void SetProxyAuthInfo(const std::wstring &account, const std::wstring &pwd);
    void GetProxyAuthInfo(std::wstring &account, std::wstring &pwd);
    void GetAuthInfoCallback(std::wstring &account, std::wstring &pwd);

	void RemoveRenderHandler(CefWindowHandle hwnd) {
		_renderHandlers.erase(hwnd);
	}

    CefRefPtr<CefBrowser> GetBrowser() {return _browser;}
    CefRefPtr<CefBrowser> GetBrowser(CefWindowHandle);

public:
    // CefClient methods
    virtual CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() OVERRIDE { return this;}
    virtual CefRefPtr<CefLoadHandler> GetLoadHandler() OVERRIDE {return this;}
    virtual CefRefPtr<CefDisplayHandler> GetDisplayHandler() OVERRIDE {return this;}
	virtual CefRefPtr<CefContextMenuHandler> GetContextMenuHandler() {return this;}
	virtual CefRefPtr<CefRequestHandler> GetRequestHandler() {return this;}
	virtual CefRefPtr<CefDragHandler> GetDragHandler() {return this;}
	virtual CefRefPtr<CefKeyboardHandler> GetKeyboardHandler() {return this;}
    virtual bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
										CefProcessId source_process, 
										CefRefPtr<CefProcessMessage> message) OVERRIDE;

    // CefLifeSpanHandler methods
	virtual bool ClientHandler::OnBeforePopup(
										CefRefPtr<CefBrowser> browser,
										CefRefPtr<CefFrame> frame,
										const CefString& target_url,
										const CefString& target_frame_name,
										CefLifeSpanHandler::WindowOpenDisposition target_disposition,
										bool user_gesture,
										const CefPopupFeatures& popupFeatures,
										CefWindowInfo& windowInfo,
										CefRefPtr<CefClient>& client,
										CefBrowserSettings& settings,
										bool* no_javascript_access) OVERRIDE;
    virtual void OnAfterCreated(CefRefPtr<CefBrowser> browser) OVERRIDE;
    virtual bool DoClose(CefRefPtr<CefBrowser> browser) OVERRIDE;
    virtual void OnBeforeClose(CefRefPtr<CefBrowser> browser) OVERRIDE;

    // CefLoadHandler methods
    virtual void OnLoadStart(CefRefPtr<CefBrowser> browser, 
							CefRefPtr<CefFrame> frame) OVERRIDE;
    virtual void OnLoadEnd(CefRefPtr<CefBrowser> browser,
							CefRefPtr<CefFrame> frame, 
							int httpStatusCode) OVERRIDE;
    virtual void OnLoadError(CefRefPtr<CefBrowser> browser,
							CefRefPtr<CefFrame> frame, 
							ErrorCode errorCode,
							const CefString& errorText, 
							const CefString& failedUrl) OVERRIDE;


    // CefDisplayHandler methods
    virtual void OnLoadingStateChange(CefRefPtr<CefBrowser> browser,
									bool isLoading, 
									bool canGoBack, 
									bool canGoForward) OVERRIDE;

    virtual CefRefPtr<CefRenderHandler> GetRenderHandler() OVERRIDE {
        return this;
    }
	virtual bool OnTooltip(CefRefPtr<CefBrowser> browser, CefString& text);

    // CefContextMenuHandler methods
    virtual void OnBeforeContextMenu(CefRefPtr<CefBrowser> browser,
                                     CefRefPtr<CefFrame> frame,
                                     CefRefPtr<CefContextMenuParams> params,
                                     CefRefPtr<CefMenuModel> model);
	
	// CefRequestHandler
	virtual bool OnBeforeBrowse(CefRefPtr<CefBrowser> browser, 
		                        CefRefPtr<CefFrame> frame, 
								CefRefPtr<CefRequest> request, 
								bool is_redirect);
	virtual void OnRenderProcessTerminated(CefRefPtr<CefBrowser> browser,
								TerminationStatus status);
    virtual bool GetAuthCredentials(CefRefPtr<CefBrowser> browser,
        CefRefPtr<CefFrame> frame,
        bool isProxy,
        const CefString& host,
        int port,
        const CefString& realm,
        const CefString& scheme,
        CefRefPtr<CefAuthCallback> callback);

	// CefRenderHandler
	virtual bool GetRootScreenRect(CefRefPtr<CefBrowser> browser,
                                 CefRect& rect) OVERRIDE;
	virtual bool GetViewRect(CefRefPtr<CefBrowser> browser,
                           CefRect& rect) OVERRIDE;
	virtual bool GetScreenPoint(CefRefPtr<CefBrowser> browser,
                              int viewX,
                              int viewY,
                              int& screenX,
                              int& screenY) OVERRIDE;
	virtual bool GetScreenInfo(CefRefPtr<CefBrowser> browser,
                             CefScreenInfo& screen_info) OVERRIDE;
	virtual void OnPopupShow(CefRefPtr<CefBrowser> browser, bool show) OVERRIDE;
	virtual void OnPopupSize(CefRefPtr<CefBrowser> browser,
                           const CefRect& rect) OVERRIDE;
	virtual void OnPaint(CefRefPtr<CefBrowser> browser,
                       PaintElementType type,
                       const RectList& dirtyRects,
                       const void* buffer,
                       int width,
                       int height) OVERRIDE;
	virtual void OnCursorChange(CefRefPtr<CefBrowser> browser,
						CefCursorHandle cursor,
						CursorType type,
						const CefCursorInfo& custom_cursor_info) OVERRIDE;
	virtual bool StartDragging(CefRefPtr<CefBrowser> browser,
						CefRefPtr<CefDragData> drag_data,
						CefRenderHandler::DragOperationsMask allowed_ops,
						int x, int y);
	void UpdateDragCursor(CefRefPtr<CefBrowser> browser, CefRenderHandler::DragOperation operation);

	// CefKeyboardHandler
	bool OnPreKeyEvent(CefRefPtr<CefBrowser> browser, const CefKeyEvent& event,
		CefEventHandle os_event, bool* is_keyboard_shortcut) ;

private:
	void _closeModalDialog(HWND hOwner);

private:
	CefRefPtr<CefRenderHandler> GetRenderHandler(CefRefPtr<CefBrowser> browser);

	void ShowDevTools(CefRefPtr<CefBrowser> browser,
		const CefPoint& inspect_element_at);

	bool HasBrowsers() {
		EnterCriticalSection(&_cs);
		bool hasBrowser = !_browsers.empty();
		LeaveCriticalSection(&_cs);
		return hasBrowser;
	}

	bool HandleURLProtocol(const std::wstring& url);

	void _executeJSForAllBrowsers(const std::wstring& code);

    bool GetAuthInfoFromUserInput(std::wstring &account, std::wstring &pwd);
private:
	cefIntegration::CefClient *_cefClient;

	typedef std::set<CefRefPtr<BrowserDelegate> > BrowserDelegateSet;
	BrowserDelegateSet _browserDelegates;

    typedef std::map< CefWindowHandle, CefRefPtr<CefBrowser> > BrowserWindowMap;
  	BrowserWindowMap _browsers;

  	typedef std::map< CefWindowHandle, CefRefPtr<CefRenderHandler> > RenderHandlerMap;
  	RenderHandlerMap _renderHandlers;

	CRITICAL_SECTION _cs;

    // browser for main window
    CefRefPtr<CefBrowser> _browser;

    HWND _mainWnd;

	// 退出cef程序标志
	bool _closing;

    // proxy auth info
    std::wstring    _account; 
    std::wstring    _pwd;
    int             _AuthcallbackNum;

	IMPLEMENT_REFCOUNTING(ClientHandler);
};

}

#endif