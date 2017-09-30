#include "browser/WindowManager.h"
#include "utils/FileUtil.h"
#include <sstream>

#include "include/cef_runnable.h"
#include "CefClient.h"
#include "LogManager.h"
#include "browser/CefOSRWindow.h"
#include "common/StringConvert.h"

namespace cefIntegration {

using namespace uc;

WindowManager::WindowManager(CefClient *cefClient) : _cefClient(cefClient), _isOpening(false){

}

WindowManager::~WindowManager() {

}

void WindowManager::CreateUrlPath(const std::wstring &url, std::wstring &path) {
	std::wstring modulePath;
	FileUtil::ProcessPath(modulePath);

	std::wstringstream ss;
	ss << modulePath << L"//resources/html/" << url;

	path = ss.str();
}

void WindowManager::Navigate(CefRefPtr<CefBrowser> browser, const std::wstring &url) {
	std::wstring path;
	CreateUrlPath(url, path);

	CefRefPtr<CefFrame> frame = browser->GetMainFrame();
	frame->LoadURL(path);
}

void WindowManager::Open(CefRefPtr<CefBrowser> browser, const std::wstring &url, const std::wstring &name,
	const WindowFeature& feature, AfterBehaviorOpenWindow afterBehavior) {
	_isOpening = true;
	WindowNameMap::iterator iter = _windowNameMap.find(name);
	if (iter != _windowNameMap.end()) {
		if (feature.refreshIfExist) {
			std::wstring path;
			CreateUrlPath(url, path);
			iter->second->Navigate(path);
		}

        iter->second->Show();
		iter->second->SetForeground();
	}
	else {
		std::wstring path;
		CreateUrlPath(url, path);
		CefRefPtr<CefWindow> window = new cefIntegration::CefOSRWindow(name, _cefClient);
		HWND hwndParent = (browser != NULL && browser.get()) ? browser->GetHost()->GetWindowHandle() : NULL;
		window->Create(feature, path, hwndParent);

		_windowNameMap[name] = window;
		_hwndMap[window->GetWnd()] = window;

		if (afterBehavior == ABOW_CLOSE_ALL) {
			_cefClient->getHandler()->CloseAllWindowExcept(true, window->GetWnd());
		}
	}

	_isOpening = false;
}
	

void WindowManager::Minimize(CefRefPtr<CefBrowser> browser) {
    HWND hwnd = browser->GetHost()->GetWindowHandle();
	CefRefPtr<CefWindow> window = GetWindow(browser->GetHost()->GetWindowHandle());
	if (window != NULL) {
		window->Minimize();
	}
}

void WindowManager::Maximize(CefRefPtr<CefBrowser> browser) {
	CefRefPtr<CefWindow> window = GetWindow(browser->GetHost()->GetWindowHandle());
	if (window != NULL) {
		window->Maximize();
	}
}

void WindowManager::FullScreen(CefRefPtr<CefBrowser> browser) {
	CefRefPtr<CefWindow> window = GetWindow(browser->GetHost()->GetWindowHandle());
	if (window != NULL) {
		window->FullScreen();
	}
}

void WindowManager::Restore(CefRefPtr<CefBrowser> browser) {
	CefRefPtr<CefWindow> window = GetWindow(browser->GetHost()->GetWindowHandle());
	if (window != NULL) {
		window->Restore();
	}
}

void WindowManager::Hide(CefRefPtr<CefBrowser> browser) {
	CefRefPtr<CefWindow> window = GetWindow(browser->GetHost()->GetWindowHandle());
	if (window != NULL) {
		window->Hide();
	}
}

void WindowManager::HideAll() {
	for (HWNDMap::iterator iter = _hwndMap.begin(); iter != _hwndMap.end(); ++iter) {
		iter->second->Hide();
	}
}

void WindowManager::Show(CefRefPtr<CefBrowser> browser) {
	CefRefPtr<CefWindow> window = GetWindow(browser->GetHost()->GetWindowHandle());
	if (window != NULL) {
		window->Show();
	}
}

void WindowManager::SetCaptions(CefRefPtr<CefBrowser> browser, const std::vector<RECT> & rect) {
	CefRefPtr<CefWindow> window = GetWindow(browser->GetHost()->GetWindowHandle());
	if (window != NULL) {
		window->SetCaptionArea(rect);
	}
}

void WindowManager::Close(CefRefPtr<CefBrowser> browser, const std::wstring &name) {
	CefRefPtr<CefWindow> window;
	if (name.empty()) {
		HWND hwnd = browser->GetHost()->GetWindowHandle();
		window = GetWindow(hwnd);
	}
	else {
		WindowNameMap::iterator iter = _windowNameMap.find(name);
		if (iter != _windowNameMap.end()) {
			window = iter->second;
		}
	}

	if (window != NULL) {
		window->Close();
	}
}

CefRefPtr<CefWindow> WindowManager::RemoveWindow(HWND hwnd) {
	CefRefPtr<CefWindow> window = GetWindow(hwnd);
	if (window != NULL) {
		_windowNameMap.erase(window->GetName());
		_hwndMap.erase(hwnd);
	}

	CheckIfQuit();

	return window;
}

void WindowManager::SetForeground(CefRefPtr<CefBrowser> browser) {
	CefRefPtr<CefWindow> window = GetWindow(browser->GetHost()->GetWindowHandle());
	if (window != NULL) {
		window->SetForeground();
	}
}

void WindowManager::SetTray(CefRefPtr<CefBrowser> browser, const std::wstring & title, 
	const std::wstring & imageName, bool isFlash, std::vector<MenuFeature> &menuFeatures) {
	CefRefPtr<CefWindow> window = GetWindow(browser->GetHost()->GetWindowHandle());
	if (window != NULL) {
		window->SetTray(title, imageName, isFlash, menuFeatures);
	}
}

void WindowManager::FlashTray(CefRefPtr<CefBrowser> browser, bool isFlash, int interval) {
    CefRefPtr<CefWindow> window = GetWindow(browser->GetHost()->GetWindowHandle());
    if (window != NULL) {
        // 本不应该闪的情况，c++部分直接停止闪
        if (isFlash && (CWS_FOREGROUND == GetWindowStatue(browser)))
        {
            window->FlashTray(false, FLASH_TRAY_TIMEOUT_MS);
            return;
        }
        window->FlashTray(isFlash, interval);
    }
}

void WindowManager::Move(CefRefPtr<CefBrowser> browser, int x, int y) {
    CefRefPtr<CefWindow> window = GetWindow(browser->GetHost()->GetWindowHandle());
    if (window != NULL) {
        window->Move(x, y);
    }
}

void WindowManager::DisableDBClick(CefRefPtr<CefBrowser> browser) {
	CefRefPtr<CefWindow> window = GetWindow(browser->GetHost()->GetWindowHandle());
	if (window != NULL) {
		window->DisableDBClick();
	}
}

void WindowManager::EnableDBClick(CefRefPtr<CefBrowser> browser) {
	CefRefPtr<CefWindow> window = GetWindow(browser->GetHost()->GetWindowHandle());
	if (window != NULL) {
		window->EnableDBClick();
	}
}

void WindowManager::GetTrayPos(CefRefPtr<CefBrowser> browser, std::wstring & trayPos) {
    CefRefPtr<CefWindow> window = GetWindow(browser->GetHost()->GetWindowHandle());
    if (window != NULL) {
        window->GetTrayPos(trayPos);
    }
}

void WindowManager::SetTrayBalloon(CefRefPtr<CefBrowser> browser, ToolTipIcon icon,
	std::wstring& title, std::wstring& text, int timeOut) {
	CefRefPtr<CefWindow> window = GetWindow(browser->GetHost()->GetWindowHandle());
	if (window != NULL) {
		window->SetTrayBalloon(icon, title, text, timeOut);
	}
}

void WindowManager::FlashWindow(CefRefPtr<CefBrowser> browser, UINT nCount, UINT nTimeout) {
	CefRefPtr<CefWindow> window = GetWindow(browser->GetHost()->GetWindowHandle());
	if (window != NULL) {
		window->FlashWindow(nCount, nTimeout);
	}
}

void WindowManager::SetIconWithNumber(CefRefPtr<CefBrowser> browser, int number) {
	CefRefPtr<CefWindow> window = GetWindow(browser->GetHost()->GetWindowHandle());
	if (window != NULL) {
		window->SetIconWithNumber(number);
	}
}

void WindowManager::SetTop(CefRefPtr<CefBrowser> browser, bool isTop) {
	CefRefPtr<CefWindow> window = GetWindow(browser->GetHost()->GetWindowHandle());
	if (window != NULL) {
		window->SetTop(isTop);
	}
}

void WindowManager::SetSize(CefRefPtr<CefBrowser> browser,int width, int height) {
	CefRefPtr<CefWindow> window = GetWindow(browser->GetHost()->GetWindowHandle());
	if (window != NULL) {
		window->SetSize(width, height);
	}
}

void WindowManager::GetWindowPos(CefRefPtr<CefBrowser> browser, std::wstring& windowPos) {
	std::wstringstream stream;
	stream << L"[";
	CefRefPtr<CefWindow> window = GetWindow(browser->GetHost()->GetWindowHandle());
	if (window != NULL) {
		int x = 0;
		int y = 0;
		int cx = 0;
		int cy = 0;
		window->GetWindowPos(x, y, cx, cy);
		stream << x << L"," << y << L"," << cx << L"," << cy;
	}
	stream << L"]";
	windowPos = stream.str();
}

void WindowManager::CenterWindow(CefRefPtr<CefBrowser> browser){
	CefRefPtr<CefWindow> window = GetWindow(browser->GetHost()->GetWindowHandle());
	if (window != NULL) {
		window->CenterWindow();
	}
}

int WindowManager::GetDroppedFiles(CefRefPtr<CefBrowser> browser, std::wstring &names) {
	CefRefPtr<CefWindow> window = GetWindow(browser->GetHost()->GetWindowHandle());
	if (window != NULL) {
		std::vector<CefString>  files;
		window->GetDroppedFiles(files);

		if (files.size() > 0) {
			std::wstringstream stream;
			stream <<  L"[" ;
			for (unsigned int i = 0; i < files.size(); i++) {
				stream << L"{\"path\": ";
				std::wstring file(files[i]);
				replace(file.begin(), file.end(), '\\', '/');
				stream <<  L"\"";
				stream << UTF2WJSON(W2UTF(file));
				stream << L"\"";
				stream << L", \"attribute\": " << (FileUtil::IsFolder(file) ? 1 : 0);
				stream << L"}";

				if (i < files.size() - 1) {
					stream << L", ";
				}
			}
			stream << L"]";
			names = stream.str();

			return CEF_SUCCESS;
		}
	}

	return CEF_DROPPED_FILE_NOT_FOUND;
}

HWND WindowManager::GetWindowHwnd(const std::wstring &name){
	HWND hwnd;
	WindowNameMap::iterator iter = _windowNameMap.find(name);
	if (iter != _windowNameMap.end()){
		hwnd = iter->second->GetWnd();
	}
	return hwnd;
}

void WindowManager::RegisterHotKey(CefRefPtr<CefBrowser> browser, int id, int modifiers, int key) {
	CefRefPtr<CefWindow> window = GetWindow(browser->GetHost()->GetWindowHandle());
	if (window != NULL) {
		window->RegisterHotKey(id, modifiers, key);
	}
}

void WindowManager::UnRegisterHotKey(CefRefPtr<CefBrowser> browser, int id) {
	CefRefPtr<CefWindow> window = GetWindow(browser->GetHost()->GetWindowHandle());
	if (window != NULL) {
		window->UnRegisterHotKey(id);
	}
}

CefRefPtr<CefWindow> WindowManager::GetWindow(HWND hwnd) {
	HWNDMap::iterator iter = _hwndMap.find(hwnd);
	if (iter != _hwndMap.end()) {
		return iter->second;
	}
	else {
		return NULL;
	}
}

void WindowManager::SetCaretPosition(CefRefPtr<CefBrowser> browser, int x, int y, int height) {
	CefRefPtr<CefWindow> window = GetWindow(browser->GetHost()->GetWindowHandle());
	if (window != NULL) {
		window->SetCaretPosition(x, y, height);
	}
}

void WindowManager::CheckIfQuit() {
	if (_hwndMap.empty() && !_isOpening) {
		UC_LOG(INFO) << "all windows have been closed, quit the app!";
		_cefClient->Quit();
	}
}

int WindowManager::GetWindowStatue(CefRefPtr<CefBrowser> browser) {
    int statue = CWS_NORMAL;
	CefRefPtr<CefWindow> window = GetWindow(browser->GetHost()->GetWindowHandle());
	if (window != NULL) {
        statue = window->GetWinstatus();
	}

    HWND askWnd = browser->GetHost()->GetWindowHandle();
    HWND frontWnd = ::GetForegroundWindow();
    if ((statue == CWS_NORMAL || statue == CWS_MAXIMIZED) && (askWnd == frontWnd)) // 正常和最大化时，需要看是否在最前端显示
    {
        statue = CWS_FOREGROUND;
    }

    UC_LOG(INFO) << "main window status:" << statue;
    return statue;
}

int WindowManager::IsWindowOpen(CefRefPtr<CefBrowser> browser, const std::wstring name) {
    int ret = 0;
    WindowNameMap::iterator iter = _windowNameMap.find(name);
    if (iter != _windowNameMap.end()) {
        ret = 1;
    }
    return ret;
}

}