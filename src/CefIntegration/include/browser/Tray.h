/*
* Tray.h
*
*  Created on: Sep 16, 2015
*      Author: gang wang
*/

#ifndef TRAY_H_
#define TRAY_H_

#include <Windows.h>
#include <vector>
#include <string>
#include <Shellapi.h>

#include "WindowFeature.h"

namespace cefIntegration {

static const int WM_SYS_TRAY = WM_USER + 1;
static const int FLASH_TRAY_TIMEOUT_MS = 400;
static const int MOUSE_POINT_CHECK_TIMEOUT_MS = 120;

enum ToolTipIcon{
	TTI_NONE = 0,
	TTI_INFO,
	TTI_WARNING,
	TTI_ERROR
};

class CefWindow;

class Tray {
public:
	Tray();
	~Tray();

    static Tray* GetInstance();
    static void Release();

    void Tray::SetTray(CefWindow* window, const std::wstring & title, const std::wstring & imagePath, bool isFlash,
		std::vector<MenuFeature> &menuFeatures);

	void SetTrayBalloon(ToolTipIcon icon, const std::wstring& title, const std::wstring& text, int timeOut);

	bool OnSysTray(WPARAM wParam, LPARAM lParam );

	bool OnTimer(WPARAM wParam);

	bool OnClose();

    bool GetIconPosition(RECT &rect);
    void GetTrayWndPosition(RECT &rect);

    void FlashTray(bool isStart, int interval = FLASH_TRAY_TIMEOUT_MS);
    BOOL UpdateToolbarWndIcon();

private:
	void DeleteTray();
	void ShowContextMenu();
    HWND GetTrayNotifyWnd();
    bool CheckMouseOut(int nx, int ny, int ix, int iy);
private:

    static Tray* _instance;

	int _flashTimerId;
    bool _isFlash;
	HICON _currIcon;
	HICON _oldIcon;
	NOTIFYICONDATA _notifyIconData;
	bool _hasTray;
	int _trayId;

	std::vector<MenuFeature> _menuFeatures;

	CefWindow* _window;

    bool    _checkTimerOpenFlag;
    int     _checkTimerID;

    POINT   _pt;
    int     _posX;
    int     _posY;
};

}

#endif