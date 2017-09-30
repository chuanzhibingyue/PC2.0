#include "browser/ClientHandler.h"
#include "ClientDefs.h"
#include "CefClient.h"
#include "include/cef_runnable.h"
#include "LogManager.h"
#include "Utils/FileUtil.h"
#include "pugixml.hpp"
#include "common/CallOnMainThread.h"
#include "common/StringConvert.h"
#include "browser/Proxy.h"
#include "browser/ProxyAuthWnd.h"

namespace cefIntegration {

using namespace uc;
const int MAX_RELOAD_COUNT = 3;

ClientHandler::ClientHandler(HINSTANCE hInstance, cefIntegration::CefClient *client)
    : _cefClient(client), _closing(false), _account(L""), _pwd(L""), _AuthcallbackNum(0){

    InitializeCriticalSection(&_cs);
    CPaintManagerUI::SetInstance(hInstance);
}

ClientHandler::~ClientHandler() {
	DeleteCriticalSection(&_cs);
}

void ClientHandler::RegisterBrowserDelegate(BrowserDelegate* browserDelegate) {
	_browserDelegates.insert(browserDelegate);
}

CefRefPtr<CefBrowser> ClientHandler::GetBrowser(CefWindowHandle handle) {
	CefRefPtr<CefBrowser> browser = NULL;

	EnterCriticalSection(&_cs);

    BrowserWindowMap::iterator iter = _browsers.find(handle);
    if (iter != _browsers.end()) {
		browser = iter->second;
    }

	LeaveCriticalSection(&_cs);

	return browser;
}

void ClientHandler::ExecuteJSForAllBrowsers(const std::wstring &code) {
	UC_LOG(INFO) << "ExecuteJSForAllBrowsers, script: " << code;

	_executeJSForAllBrowsers(code);
}

void ClientHandler::_executeJSForAllBrowsers(const std::wstring& code) {
	EnterCriticalSection(&_cs);

	for (BrowserWindowMap::iterator iter = _browsers.begin();
		iter != _browsers.end(); ++iter) {
		CefRefPtr<CefFrame> frame = iter->second->GetMainFrame();
		if (frame){
			frame->ExecuteJavaScript(code, "", 0);
		}
	}

	LeaveCriticalSection(&_cs);
}

void ClientHandler::Quit() {
	_closing = true;

    if (!HasBrowsers()) {
		UC_LOG(ERROR) << "quit cef message loop";
        CefQuitMessageLoop();
    }
    else {
		CloseAllWindowExcept();
    }
}

void ClientHandler::CloseAllWindowExcept(bool hasExcept, HWND hwnd) {
	EnterCriticalSection(&_cs);

	std::vector<HWND> hWndData;
	BrowserWindowMap::const_iterator iter = _browsers.begin();
	for (; iter != _browsers.end(); ++iter) {
		if ( (!hasExcept) || (hasExcept && hwnd != iter->second->GetHost()->GetWindowHandle())) {
			hWndData.push_back(iter->second->GetHost()->GetWindowHandle());
		}
	}

	for (int i = hWndData.size() - 1; i >= 0; i--) {
		_closeModalDialog(hWndData[i]);
		::PostMessage(hWndData[i], WM_CLOSE, 0, 0);
	}

	LeaveCriticalSection(&_cs);
}

bool ClientHandler::OnProcessMessageReceived(CefRefPtr<CefBrowser> browser, 
    CefProcessId source_process, CefRefPtr<CefProcessMessage> message) 
{
    bool handled = false;
    BrowserDelegateSet::iterator it = _browserDelegates.begin();
    for (; it != _browserDelegates.end() && !handled; ++it) 
    {
        handled = (*it)->OnProcessMessageReceived(browser, source_process, message);
    }

    return handled;
}

void ClientHandler::ShowDevTools(CefRefPtr<CefBrowser> browser, const CefPoint& inspect_element_at) {
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(std::string(FileUtil::ProcessPath() + "\\config\\bee.config").c_str());
	if (result.status == pugi::status_ok) {
		pugi::xml_node node = doc.first_element_by_path("/configuration/devtool/enable");
		if (node.empty() || std::string(node.child_value()) != "true") {
			return;
		}
	}

	CefWindowInfo windowInfo;
	CefBrowserSettings settings;

	windowInfo.SetAsPopup(browser->GetHost()->GetWindowHandle(), L"DEV TOOL");
	browser->GetHost()->ShowDevTools(windowInfo, this, settings,
		inspect_element_at);
}

void ClientHandler::OnBeforeContextMenu(CefRefPtr<CefBrowser> browser,
                                   CefRefPtr<CefFrame> frame,
                                   CefRefPtr<CefContextMenuParams> params,
                                   CefRefPtr<CefMenuModel> model) {
	//ShowDevTools(browser, CefPoint());
}

bool ClientHandler::OnBeforePopup(
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
									bool* no_javascript_access) {
	if (target_url.empty()) {
		return true;
	}

	//forbidden show blank CEF browser for url
	return HandleURLProtocol(target_url.ToWString());
}

void ClientHandler::OnAfterCreated(CefRefPtr<CefBrowser> browser) {
    REQUIRE_UI_THREAD();
	EnterCriticalSection(&_cs);
	_browsers[browser->GetHost()->GetWindowHandle()] = browser;
	LeaveCriticalSection(&_cs);
}

bool ClientHandler::DoClose(CefRefPtr<CefBrowser> browser) {
    REQUIRE_UI_THREAD();

	// close native window if the window is not closed when js
	// use browser api:window.close();
	CefRefPtr<CefWindow> window = _cefClient->getWindowManager()->GetWindow(browser->GetHost()
		->GetWindowHandle());
	if (window != NULL && window.get() && !window->IsClosed()) {
		window->CloseWindowOnly();
	}

    return false;
}

void ClientHandler::OnBeforeClose(CefRefPtr<CefBrowser> browser) {
    REQUIRE_UI_THREAD();

	EnterCriticalSection(&_cs);
    _browsers.erase(browser->GetHost()->GetWindowHandle());
	LeaveCriticalSection(&_cs);

	if (_closing && !HasBrowsers()) {
		CefQuitMessageLoop();
	}
}

bool ClientHandler::OnTooltip(CefRefPtr<CefBrowser> browser, CefString& text) {
	std::wstring strText = text.ToWString();
	CefRefPtr<CefWindow> window = _cefClient->getWindowManager()->GetWindow(browser->GetHost()
		->GetWindowHandle());
	if (window != NULL && window.get()) {
		window->SetToolTip(strText);
	}
	
	return true;
}

void ClientHandler::OnLoadStart(CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefFrame> frame) {
    REQUIRE_UI_THREAD();
}

void ClientHandler::OnLoadEnd(CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefFrame> frame, int httpStatusCode) {
    REQUIRE_UI_THREAD();
}

void ClientHandler::OnLoadError(CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefFrame> frame, ErrorCode errorCode,
    const CefString& errorText, const CefString& failedUrl) {
    REQUIRE_UI_THREAD();
}

void ClientHandler::OnLoadingStateChange(CefRefPtr<CefBrowser> browser,
                                         bool isLoading,
                                         bool canGoBack,
                                         bool canGoForward) {
    REQUIRE_UI_THREAD();
}

bool ClientHandler::GetRootScreenRect(CefRefPtr<CefBrowser> browser,
    CefRect& rect) {
    CefRefPtr<CefRenderHandler> handler = GetRenderHandler(browser);
    if (handler != NULL && handler.get()) {
        return handler->GetRootScreenRect(browser, rect);
    }
    return false;
}

bool ClientHandler::GetViewRect(CefRefPtr<CefBrowser> browser, CefRect& rect) {
    CefRefPtr<CefRenderHandler> handler = GetRenderHandler(browser);
    if (handler != NULL && handler.get()) {
		return handler->GetViewRect(browser, rect);
    }
    return false;
}

bool ClientHandler::GetScreenPoint(CefRefPtr<CefBrowser> browser,
                                   int viewX,
                                   int viewY,
                                   int& screenX,
                                   int& screenY) {
    CefRefPtr<CefRenderHandler> handler = GetRenderHandler(browser);
    if (handler != NULL && handler.get()) {
		return handler->GetScreenPoint(browser, viewX, viewY, screenX, screenY);
    }
    return false;
}

bool ClientHandler::GetScreenInfo(CefRefPtr<CefBrowser> browser,
                                  CefScreenInfo& screen_info) {
    CefRefPtr<CefRenderHandler> handler = GetRenderHandler(browser);
    if (handler != NULL && handler.get()) {
		return handler->GetScreenInfo(browser, screen_info);
    }
    return false;
}

void ClientHandler::OnPopupShow(CefRefPtr<CefBrowser> browser,
                                bool show) {
    CefRefPtr<CefRenderHandler> handler = GetRenderHandler(browser);
    if (handler != NULL && handler.get()) {
        handler->OnPopupShow(browser, show);
    }
}

void ClientHandler::OnPopupSize(CefRefPtr<CefBrowser> browser,
                                const CefRect& rect) {
    CefRefPtr<CefRenderHandler> handler = GetRenderHandler(browser);
    if (handler != NULL && handler.get()) {
        handler->OnPopupSize(browser, rect);
    }
}

void ClientHandler::OnPaint(CefRefPtr<CefBrowser> browser,
                            PaintElementType type,
                            const RectList& dirtyRects,
                            const void* buffer,
                            int width,
                            int height) {
    CefRefPtr<CefRenderHandler> handler = GetRenderHandler(browser);
    if (handler != NULL && handler.get()) {
        handler->OnPaint(browser, type, dirtyRects, buffer, width, height);
    }
}

void ClientHandler::OnCursorChange(CefRefPtr<CefBrowser> browser,
							CefCursorHandle cursor,
							CursorType type,
							const CefCursorInfo& custom_cursor_info) {
	CefRefPtr<CefRenderHandler> handler = GetRenderHandler(browser);
    if (handler != NULL && handler.get()) {
		handler->OnCursorChange(browser, cursor, type, custom_cursor_info);
    }
}

CefRefPtr<CefRenderHandler> ClientHandler::GetRenderHandler(CefRefPtr<CefBrowser> browser) {
    RenderHandlerMap::iterator iter = _renderHandlers.find(browser->GetHost()->GetWindowHandle());
    if (iter != _renderHandlers.end()) {
        return iter->second;
    }
    return NULL;
}

bool ClientHandler::OnBeforeBrowse(CefRefPtr<CefBrowser> browser, 
								CefRefPtr<CefFrame> frame, 
								CefRefPtr<CefRequest> request, 
								bool is_redirect) {
	if (!frame->IsMain()) {
		return false;
	}

	// handle "mailto:"
	return HandleURLProtocol(request->GetURL().ToWString());
}

bool ClientHandler::HandleURLProtocol(const std::wstring& url) {
	static std::wstring internal_file_prefix = L"file:///";
	static std::wstring internal_tool_prefix = L"chrome-devtools://";
	std::wstring url_open = url;

	bool allow_open = url.substr(0, internal_file_prefix.size()) != internal_file_prefix 
		              && url.substr(0, internal_tool_prefix.size()) != internal_tool_prefix;
	if (!allow_open) {
		//handle case: "file:///****/html/page/www.baidu.com"
		static std::wstring html_page = L"\/html\/page\/";

		//hanlde parameter case: file:///****/html/page/*.html?parame1=100
		std::wstring file_name = url.substr(internal_file_prefix.size());
		if (file_name.find(L"?") != std::wstring::npos) {
			file_name = file_name.substr(0, file_name.find(L"?"));
		}

		//make sure it's the local file
		std::wstring decoded_file_name = UTF2W(StringConvert::URLDecode(W2UTF(file_name)));
		bool invalid_file = FileUtil::GetFileSize(file_name) == CEF_OPEN_FILE_ERROR
							&& FileUtil::GetFileSize(decoded_file_name) == CEF_OPEN_FILE_ERROR;

		std::wstring url_candidate = url.substr(url.find(html_page) + html_page.size());

		// 1. there is . in url; &&
		// 2. come from html page &&
		// 3. it's an invalid local file
		// then treat it as a url and can open in browser
		// frankly it's not a good method to handle case: "file:///****/html/page/www.baidu.com"
		if (url.find(html_page) != std::wstring::npos
			&& url_candidate.find_first_of(L".") != std::wstring::npos
			&& invalid_file) {
			allow_open = true;
			url_open = url_candidate;
			UC_LOG(INFO) << L"ClientHandler::HandleURLProtocol especial url:" << url;
		}
	}

	if (allow_open) {
		ShellExecuteW(NULL, L"Open", (LPCWSTR)url_open.c_str(), NULL, NULL, SW_SHOWNORMAL);
		return true;
	}
	
	return false;
}

bool ClientHandler::GetAuthCredentials(CefRefPtr<CefBrowser> browser,
                                       CefRefPtr<CefFrame> frame,
                                       bool isProxy,
                                       const CefString& host,
                                       int port,
                                       const CefString& realm,
                                       const CefString& scheme,
                                       CefRefPtr<CefAuthCallback> callback){

    if (isProxy && _AuthcallbackNum < 3) {
        // 如果已经发生过认证，但再次需要认证，说明之前的账户和密码错误，则清空之前的数据，让用户继续手动输入
        if (_AuthcallbackNum > 0) {
            _account = L"";
            _pwd = L"";
        }

        if (_account.empty() || _account.empty()) {
            if (!GetAuthInfoFromUserInput(_account, _pwd)) {
                return false;
            }
        }

        if (!_account.empty() && !_pwd.empty()) {
            callback->Continue(_account, _pwd);
            _AuthcallbackNum++;
            UC_LOG(INFO) << L"ClientHandler::GetAuthCredentials Continue time:" << _AuthcallbackNum << ", account:" << _account << ", pwd:" << _pwd;
        }
        return true;
    }

    if (!isProxy)
    {
        UC_LOG(INFO) << L"ClientHandler::GetAuthCredentials host:" << host.c_str() << "is not a correct proxy server.";
    }

    // 3次失败后重新初始化状态
    _AuthcallbackNum = 0;
    _account = L"";
    _pwd = L"";

    return false;
}

void ClientHandler::OnRenderProcessTerminated(CefRefPtr<CefBrowser> browser,
	TerminationStatus status) {
	static int crashCount = 0;
	if (crashCount < MAX_RELOAD_COUNT) {
		browser->GetMainFrame()->LoadURL(browser->GetMainFrame()->GetURL());
		++crashCount;
	}
}

bool ClientHandler::StartDragging(CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefDragData> drag_data,
	CefRenderHandler::DragOperationsMask allowed_ops,
	int x, int y) {
	CefRefPtr<CefRenderHandler> handler = GetRenderHandler(browser);
	if (handler != NULL && handler.get()) {
		return handler->StartDragging(browser, drag_data, allowed_ops, x, y);
	}
	return false;
}

void ClientHandler::UpdateDragCursor(CefRefPtr<CefBrowser> browser, 
	CefRenderHandler::DragOperation operation) {
	CefRefPtr<CefRenderHandler> handler = GetRenderHandler(browser);
	if (handler != NULL && handler.get()) {
		handler->UpdateDragCursor(browser, operation);
	}
}

void ClientHandler::_closeModalDialog(HWND hOwner) {
	if (hOwner == NULL)
		return;

	if (::IsWindowEnabled(hOwner))
		return;

	HWND hWnd = hOwner;
	HWND hWndChild = ::GetWindow(GetDesktopWindow(), GW_CHILD);
	while (hWndChild && ::IsWindow(hWndChild)) {
		bool isCandidate = (::GetWindowLong(hWndChild, GWL_STYLE) & WS_POPUP) && ::IsWindowVisible(hWndChild);
		if (!isCandidate) {
			hWndChild = ::GetWindow(hWndChild, GW_HWNDNEXT);
			continue;
		}
		
		HWND hRootParent = ::GetParent(hWndChild);
		while (::GetParent(hRootParent) != NULL) {
			hRootParent = ::GetParent(hRootParent);
		}

		if (hWnd == hRootParent) {
			DWORD dwProcessID = 0;
			DWORD dwThreadID = GetWindowThreadProcessId(hWndChild, &dwProcessID);
			if (dwProcessID == ::GetCurrentProcessId()) {
				SendMessageTimeout(hWndChild, WM_CLOSE, 0, 0, SMTO_ABORTIFHUNG, 100, NULL);
			}
		}
	
		hWndChild = ::GetWindow(hWndChild, GW_HWNDNEXT);
	}
}

bool ClientHandler::OnPreKeyEvent(CefRefPtr<CefBrowser> browser, const CefKeyEvent& event,
	CefEventHandle os_event, bool* is_keyboard_shortcut) {
	// F12 key
	if (event.type == KEYEVENT_RAWKEYDOWN && event.windows_key_code == 0x7B &&
		event.modifiers == EVENTFLAG_CONTROL_DOWN) {
		ShowDevTools(browser, CefPoint());
		return true;
	}
	
	return false;
}

void ClientHandler::SetProxyAuthInfo(const std::wstring &account, const std::wstring &pwd)
{
    UC_LOG(INFO) << "Set Auth Info, account:" << account << ", pwd:" << pwd;
    _account = account;
    _pwd = pwd;

    // console test
   // GetAuthInfoFromUserInput(_account, _pwd);
}

void ClientHandler::GetProxyAuthInfo(std::wstring &account, std::wstring &pwd)
{
    account = _account;
    pwd = _pwd;
    UC_LOG(INFO) << "get auth info, account:" << account << ", pwd:" << pwd;
}

bool ClientHandler::GetAuthInfoFromUserInput(std::wstring &account, std::wstring &pwd)
{
    CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath() + _T("resources/authSkin"));
    CPaintManagerUI::ReloadSkin();

    CProxyAuthWnd* pFrame = new CProxyAuthWnd(this);
    if (pFrame == NULL) return false;

    HWND authWnd = pFrame->Create(NULL, _T("proxy auth"), UI_WNDSTYLE_FRAME, 0L, 0, 0, 360, 250);
    pFrame->CenterWindow();
    ::ShowWindow(*pFrame, SW_SHOW);
	::SetForegroundWindow(*pFrame);

    CPaintManagerUI::MessageLoop();
    delete pFrame;
    pFrame = NULL;

    return true;
}

void ClientHandler::GetAuthInfoCallback(std::wstring &account, std::wstring &pwd)
{
    _account = account;
    _pwd = pwd;
    UC_LOG(INFO) << "get auth dialog callbcak, account:" << account << ", pwd:" << pwd;
}

}