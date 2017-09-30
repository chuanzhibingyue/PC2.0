/*
 * CefWindow.h
 *
 *  Created on: June 27, 2015
 *      Author: gang wang
 */

#ifndef CEF_WINDOW_H_
#define CEF_WINDOW_H_

#include "include/cef_render_handler.h"
#include "ClientHandler.h"
#include "ClientDefs.h"
#include "browser/Tray.h"
#include "browser/WindowManager.h"

#include <vector>
#include <string>

namespace cefIntegration {
    enum CurrentWindowStatus {
        CWS_NORMAL,
        CWS_HIDE,
        CWS_MINIMIZED,
        CWS_MAXIMIZED,
        CWS_FOREGROUND
    };

class CefWindow : public virtual CefBase{
public:
	friend class Tray;

	CefWindow(const std::wstring& name, CefClient *client);
	virtual ~CefWindow() {}
	
	void Create(const WindowFeature& feature, std::wstring path, HWND hwndParent);

	void SetIconWithNumber(int number);

	virtual void EnableDrag(bool enable);

	void SetTop(bool isTop);

	void Minimize();

	void Maximize();

	void FullScreen();

	void Restore();

	void Show();

	void Hide();

	void Close();

	void CloseWindowOnly();

	void SetSize(int width, int height);

	void GetWindowPos(int& x, int& y, int& cx, int& cy);

	void DisableDBClick();

	void EnableDBClick();

	void Move(int x, int y, bool transfer = true);

	void CenterWindow();

	void Navigate(const std::wstring &url);

	void SetForeground();

	void SetCaptionArea(const std::vector<RECT> & captions);

	void SetTray(const std::wstring & title, const std::wstring & imagePath, bool isFlash,
		std::vector<MenuFeature> &menuFeatures);

    void ReSetTray();

    void FlashTray(bool isFlash, int interval);
    void GetTrayPos(std::wstring & trayPos);
	void SetTrayBalloon(ToolTipIcon icon, const std::wstring& title, const std::wstring& text, int timeOut);

	void FlashWindow(UINT nCount, UINT nTimeout);

	void RegisterHotKey(int id, int modifiers, int key);

	void UnRegisterHotKey(int id);

	virtual void GetDroppedFiles(std::vector<CefString> & files){}

	virtual void SetToolTip(const std::wstring &text) {}

	virtual void SetCaretPosition(int x, int y, int height) {}

	/**
     * 获取浏览器实际依附的窗口句柄
     * @return 实际窗口句柄
     */
	HWND GetWnd() { return _hwnd; }

	std::wstring& GetName() {return _name;}

	virtual LRESULT WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

    void NotifyMouseEvent(const std::wstring &code);

    int GetWinstatus(){
        return _winStatus;
    }

	bool IsClosed() { return _closed; }
protected:
	/**
     * 获取窗口对应的浏览器对象
     * @return 浏览器对象
     */
	CefRefPtr<CefBrowser> GetBrowser();

	virtual void createWindow(HWND &hwnd, bool isLayered, const std::wstring &title, HWND hwndParent) = 0;

	virtual void CreateBrowser(const std::wstring &path) = 0;

private:
	ATOM RegisterWndClass();

	void InitializeFeature(const WindowFeature& feature, HWND hwndParent);

	void RemoveTaskBarIcon();

	bool OnClose();

	bool IsInCaption(POINT &pt);

	UINT OnNCHitTest(WPARAM wParam, LPARAM lParam);

	bool OnTimer(WPARAM wParam);

	bool OnGetMinMaxInfo(LPARAM lParam);

	bool OnMouseMove();

	bool OnNCLButtonDBClick();

	bool OnSize(WPARAM wParam);

	bool OnMouseHover();

	bool OnMouseLeave();

	bool OnSysCommand(WPARAM wParam);

	bool OnCommand(WPARAM wParam);

	bool OnHotKey(WPARAM wParam);

	bool OnDestory();

	bool OnBeforeSubWindowCreate();

	bool OnActiveMeeting();

	bool OnCopyData(COPYDATASTRUCT* pCopyDataStruct);

    bool OnWindowActive(WPARAM wParam);

	void SetWinstatus(CurrentWindowStatus status);

private:
    UINT        _uTaskbarRestartMessage;
protected:
    CefClient *_client;

	HWND _hwnd;

	bool _isModal;
	bool _isResizeable;

	std::vector<RECT> _captionAreas;

	std::wstring _name;
	int _minWidth;
	int _minHeight;

	CurrentWindowStatus _winStatus;
	CurrentWindowStatus _preStatus;

	// 托盘闪烁
	Tray* _tray;
    std::wstring _trayTitle;
    std::wstring _trayImagePath;
    std::vector<MenuFeature> _trayMenuFeatures;

	BOOL _trackingMouse;

	BehaviorIfSCClose _behaviorIfSCClose;
	bool _enableDrag;

	int _leftBorder;
	int _topBorder;
	int _rightBorder;
	int _bottomBorder;

	bool _closed;
	bool _closedWindownOnly;

	float _deviceScaleFactor;

	IMPLEMENT_REFCOUNTING(CefWindow);
};

}

#endif