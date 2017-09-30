#include "WindowStrategy.h"
#include "utils/FileUtil.h"
#include <sstream>

#include "include/cef_runnable.h"

namespace uc {

WindowStrategy::WindowStrategy(CefClient *cefClient) : _cefClient(cefClient){

}

WindowStrategy::~WindowStrategy() {
	/*
	for (WindowMap::iterator iter = _windowMap.begin(); iter != _windowMap.end(); ) {
		if (iter->second != NULL) {
			delete iter->second;
		}
		iter = _windowMap.erase(iter);
	}*/
}

void WindowStrategy::CreateUrlPath(const std::wstring &url, std::wstring &path) {
	std::wstring modulePath;
	FileUtil::ProcessPath(modulePath);

	std::wstringstream ss;
	ss << modulePath << L"//resources/html/" << url;

	path = ss.str();
}

void WindowStrategy::Navigate(CefRefPtr<CefBrowser> browser, const std::wstring &url) {
	std::wstring path;
	CreateUrlPath(url, path);

	CefRefPtr<CefFrame> frame = browser->GetMainFrame();
	frame->LoadURL(path);
}

void WindowStrategy::Open(CefRefPtr<CefBrowser> browser, const std::wstring &url, const std::wstring &name, 
	const WindowFeature& feature) {
	WindowNameMap::iterator iter = _windowNameMap.find(name);
	if (iter != _windowNameMap.end()) {
		if (feature.refreshIfExist) {
			std::wstring path;
			CreateUrlPath(url, path);
			iter->second->Navigate(path);
		}

		iter->second->SetForeground();
		return;
	}

	std::wstring path;
	CreateUrlPath(url, path);

	CefRefPtr<Window> window = new uc::Window(name, _cefClient);
	_windowNameMap[name] = window;

	HWND hwndParent = NULL;
	if (browser != NULL && browser.get()) {
		hwndParent = browser->GetHost()->GetWindowHandle();
	}
	window->Create(feature.title, path, hwndParent, feature.modal, feature.resizable, feature.minWidth, feature.minHeight, feature.hasTaskBarIcon);
	_hwndMap[window->GetWnd()] = window;

	window->SetSize(feature.width, feature.height);

	if (feature.centerScreen) {
		window->CenterWindow();
	}
	else {
		window->Move(feature.left, feature.top);
	}
	
	window->SetCaptionArea(feature.captions);
	
	if (feature.isTop) {
		window->SetTop(true);
	}

	window->Show();
}

void WindowStrategy::Minimize(CefRefPtr<CefBrowser> browser) {
	CefRefPtr<Window> window = GetWindow(browser->GetHost()->GetWindowHandle());
	if (window != NULL) {
		window->Minimize();
	}
}

void WindowStrategy::Maximize(CefRefPtr<CefBrowser> browser) {
	CefRefPtr<Window> window = GetWindow(browser->GetHost()->GetWindowHandle());
	if (window != NULL) {
		window->Maximize();
	}
}

void WindowStrategy::Restore(CefRefPtr<CefBrowser> browser) {
	CefRefPtr<Window> window = GetWindow(browser->GetHost()->GetWindowHandle());
	if (window != NULL) {
		window->Restore();
	}
}

void WindowStrategy::Hide(CefRefPtr<CefBrowser> browser) {
	CefRefPtr<Window> window = GetWindow(browser->GetHost()->GetWindowHandle());
	if (window != NULL) {
		window->Hide();
	}
}

void WindowStrategy::Show(CefRefPtr<CefBrowser> browser) {
	CefRefPtr<Window> window = GetWindow(browser->GetHost()->GetWindowHandle());
	if (window != NULL) {
		window->Show();
	}
}

void WindowStrategy::SetCaptions(CefRefPtr<CefBrowser> browser, const std::vector<RECT> & rect) {
	CefRefPtr<Window> window = GetWindow(browser->GetHost()->GetWindowHandle());
	if (window != NULL) {
		window->SetCaptionArea(rect);
	}
}

void WindowStrategy::Close(CefRefPtr<CefBrowser> browser) {
	HWND hwnd = browser->GetHost()->GetWindowHandle();
	CefRefPtr<Window> window = GetWindow(hwnd);
	if (window != NULL) {
		_hwndMap.erase(hwnd);
		_windowNameMap.erase(window->GetName());
		window->Close();
	}
}

void WindowStrategy::SetForeground(CefRefPtr<CefBrowser> browser) {
	CefRefPtr<Window> window = GetWindow(browser->GetHost()->GetWindowHandle());
	if (window != NULL) {
		window->SetForeground();
	}
}

void WindowStrategy::SetTray(CefRefPtr<CefBrowser> browser, const std::wstring & title, 
	const std::wstring & imageName, std::vector<MenuFeature> &menuFeatures) {
	CefRefPtr<Window> window = GetWindow(browser->GetHost()->GetWindowHandle());
	if (window != NULL) {
		window->SetTray(title, imageName, menuFeatures);
	}
}

void WindowStrategy::SetTop(CefRefPtr<CefBrowser> browser, bool isTop) {
	CefRefPtr<Window> window = GetWindow(browser->GetHost()->GetWindowHandle());
	if (window != NULL) {
		window->SetTop(isTop);
	}
}

CefRefPtr<Window> WindowStrategy::GetWindow(HWND hwnd) {
	HWNDMap::iterator iter = _hwndMap.find(hwnd);
	if (iter != _hwndMap.end()) {
		return iter->second;
	}
	else {
		return NULL;
	}
}

}