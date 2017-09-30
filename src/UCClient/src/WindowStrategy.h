/*
 * WindowStrategy.h
 *
 *  Created on: July 18, 2015
 *      Author: gang wang
 */

#ifndef WINDOW_STRATEGY_H_
#define WINDOW_STRATEGY_H_

#include <string>
#include <map>

#include "CefClient.h"
#include "Window.h"
#include "include/cef_base.h"

namespace uc {

struct WindowFeature {
	WindowFeature() : left(0), top(0), width(0), height(0), centerScreen(false),
		refreshIfExist(false), modal(false), resizable(false),
		minWidth(0), minHeight(0),hasTaskBarIcon(true), isTop(false){
	}

	std::wstring title;
	int left;
	int top;
	int width;
	int height;
	bool centerScreen;
	bool refreshIfExist;
	bool modal;
	bool resizable;
	int minWidth;
	int minHeight;
	bool hasTaskBarIcon;
	bool isTop;
	std::vector<RECT> captions;
};

class WindowStrategy {
public:
	WindowStrategy(CefClient *cefClient);

	~WindowStrategy();

	void Open(CefRefPtr<CefBrowser> browser, const std::wstring &url, const std::wstring &name, 
		const WindowFeature& feature);

	void Navigate(CefRefPtr<CefBrowser> browser, const std::wstring &url);

	void Minimize(CefRefPtr<CefBrowser> browser);

	void Maximize(CefRefPtr<CefBrowser> browser);
	
	void Show(CefRefPtr<CefBrowser> browser);

	void SetCaptions(CefRefPtr<CefBrowser> browser, const std::vector<RECT> & rect);

	void Restore(CefRefPtr<CefBrowser> browser);

	void Close(CefRefPtr<CefBrowser> browser);

	void Hide(CefRefPtr<CefBrowser> browser);

	void SetForeground(CefRefPtr<CefBrowser> browser);

	void SetTray(CefRefPtr<CefBrowser> browser, const std::wstring & title, const std::wstring & imageName, 
		std::vector<MenuFeature> &menuFeatures);

	void SetTop(CefRefPtr<CefBrowser> browser, bool isTop);
private:
	void CreateUrlPath(const std::wstring &url, std::wstring &path);
	 CefRefPtr<Window> GetWindow(HWND hwnd);

private:
	CefClient *_cefClient;

	typedef std::map<std::wstring, CefRefPtr<Window>> WindowNameMap;
	WindowNameMap _windowNameMap;

	typedef std::map<HWND, CefRefPtr<Window>> HWNDMap;
	HWNDMap _hwndMap;
};

}
#endif


