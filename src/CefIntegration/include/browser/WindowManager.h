/*
 * WindowManager.h
 *
 *  Created on: July 18, 2015
 *      Author: gang wang
 */

#ifndef WINDOW_MANAGER_H_
#define WINDOW_MANAGER_H_

#include <string>
#include <map>

#include "CefWindow.h"
#include "include/cef_base.h"
#include "WindowFeature.h"
#include "ClientDefs.h"
#include "include/cef_browser.h"

namespace cefIntegration {

enum AfterBehaviorOpenWindow {
	ABOW_DEFAULT = 0,
	ABOW_CLOSE_ALL = 1
};

class CefClient;

class CEF_CLIENT_API WindowManager {
public:
	WindowManager(CefClient *cefClient);

	~WindowManager();

	void Open(CefRefPtr<CefBrowser> browser, const std::wstring &url, const std::wstring &name, 
		const WindowFeature& feature, AfterBehaviorOpenWindow afterBehavior);

	void Navigate(CefRefPtr<CefBrowser> browser, const std::wstring &url);

	void Minimize(CefRefPtr<CefBrowser> browser);

	void Maximize(CefRefPtr<CefBrowser> browser);

	void FullScreen(CefRefPtr<CefBrowser> browser);
	
	void Show(CefRefPtr<CefBrowser> browser);

	void SetCaptions(CefRefPtr<CefBrowser> browser, const std::vector<RECT> & rect);

	void Restore(CefRefPtr<CefBrowser> browser);

	void Close(CefRefPtr<CefBrowser> browser, const std::wstring &name);

	void Hide(CefRefPtr<CefBrowser> browser);

	void HideAll();

	void SetForeground(CefRefPtr<CefBrowser> browser);

	void SetTray(CefRefPtr<CefBrowser> browser, const std::wstring & title, const std::wstring & imageName, bool isFlash, 
		std::vector<MenuFeature> &menuFeatures);

    void FlashTray(CefRefPtr<CefBrowser> browser, bool isFlash, int interval);
    void GetTrayPos(CefRefPtr<CefBrowser> browser, std::wstring & trayPos);
    int GetWindowStatue(CefRefPtr<CefBrowser> browser);
    int IsWindowOpen(CefRefPtr<CefBrowser> browser, const std::wstring name);

	void SetTrayBalloon(CefRefPtr<CefBrowser> browser, ToolTipIcon icon, std::wstring& title, std::wstring& text, int timeOut);
	
	void FlashWindow(CefRefPtr<CefBrowser> browser, UINT nCount, UINT nTimeout);
    void Move(CefRefPtr<CefBrowser> browser, int x, int y);

	void DisableDBClick(CefRefPtr<CefBrowser> browser);
	void EnableDBClick(CefRefPtr<CefBrowser> browser);

	void SetIconWithNumber(CefRefPtr<CefBrowser> browser, int number);

	void SetTop(CefRefPtr<CefBrowser> browser, bool isTop);

	void SetSize(CefRefPtr<CefBrowser> browser, int width, int height);

	void GetWindowPos(CefRefPtr<CefBrowser> browser, std::wstring& windowPos);

	void CenterWindow(CefRefPtr<CefBrowser> browser);

	int GetDroppedFiles(CefRefPtr<CefBrowser> browser, std::wstring &names);

	HWND GetWindowHwnd(const std::wstring &name);

	void RegisterHotKey(CefRefPtr<CefBrowser> browser, int id, int modifiers, int key);

	void UnRegisterHotKey(CefRefPtr<CefBrowser> browser, int id);

	CefRefPtr<CefWindow> GetWindow(HWND hwnd);

	CefRefPtr<CefWindow> RemoveWindow(HWND hwnd);

	void CheckIfQuit();

	void SetCaretPosition(CefRefPtr<CefBrowser> browser, int x, int y, int height);
private:
	void CreateUrlPath(const std::wstring &url, std::wstring &path);
private:
	CefClient *_cefClient;

	typedef std::map<std::wstring, CefRefPtr<CefWindow> > WindowNameMap;
	WindowNameMap _windowNameMap;

	typedef std::map<HWND, CefRefPtr<CefWindow> > HWNDMap;
	HWNDMap _hwndMap;

	bool _isOpening;
};

}
#endif


