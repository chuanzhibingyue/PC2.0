#include "browser/Tray.h"
#include "cximage/ximage.h"
#include "browser/CefWindow.h"
#include "browser/JsExtDef.h"
#include "LogManager.h"

#include <Commctrl.h>

namespace cefIntegration {

using namespace uc;

Tray* Tray::_instance = NULL;

Tray::Tray()
    :_flashTimerId(1)
    , _currIcon(NULL)
    , _oldIcon(NULL)
    , _hasTray(false)
    , _trayId(0)
    , _checkTimerOpenFlag(false)
    , _checkTimerID(2)
    , _posX(0)
    , _posY(0)
    , _isFlash(false)
{
    _pt = { 0 };
	memset(&_notifyIconData, 0, sizeof(_notifyIconData));
}

Tray::~Tray() {
	if (_currIcon && DestroyIcon(_currIcon)) {
		_currIcon = NULL;
	}

	if (_oldIcon && DestroyIcon(_oldIcon)) {
		_oldIcon = NULL;
	}

    if (_checkTimerOpenFlag)
    {
        ::KillTimer(_window->GetWnd(), _checkTimerID);
        _checkTimerOpenFlag = false;
    }
	_instance = NULL;
}

Tray* Tray::GetInstance(){
    if (NULL == _instance)
    {
        _instance = new Tray();
    }
    return _instance;
}

void Tray::Release() {
    delete _instance;
    _instance = NULL;
}

void Tray::SetTray(CefWindow* window, const std::wstring & title, const std::wstring & imagePath, bool isFlash,
	std::vector<MenuFeature> &menuFeatures) {
	UC_LOG(INFO) << "set tray, old trayId: " << _trayId << ", _hasTray: " << _hasTray << ", title: "
		<< title << ", imagePath: " << imagePath << ", isFlash: " << isFlash;

    _window = window;
	_menuFeatures = menuFeatures;
	static int id = 1;

	static std::wstring PREFIX = L"file:///";
	std::wstring image_path = imagePath;
	size_t pos = imagePath.find(PREFIX);
	if (pos != std::string::npos) {
		image_path = imagePath.substr(PREFIX.size(), imagePath.length());
	}

	if (!_hasTray) {
		_notifyIconData.cbSize = sizeof(_notifyIconData);
		_notifyIconData.hWnd = _window->GetWnd();
		_notifyIconData.uFlags = NIF_ICON | NIF_MESSAGE;
		_notifyIconData.uCallbackMessage = WM_SYS_TRAY;
		_notifyIconData.uID = _trayId = id++;
	}

	if (!title.empty()) {
		_notifyIconData.uFlags |= NIF_TIP;
		wcsncpy_s(_notifyIconData.szTip, sizeof(_notifyIconData.szTip) / sizeof(WCHAR), title.c_str(), title.size());
	}

	if (_oldIcon && DestroyIcon(_oldIcon)) {
		_oldIcon = NULL;
	}

	_oldIcon = _currIcon;

	_notifyIconData.hBalloonIcon = _notifyIconData.hIcon = _currIcon = (HICON)::LoadImage(NULL,
		imagePath.c_str(), IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE);
	if (_currIcon == NULL) {
		int res = ::GetLastError();
		UC_LOG(ERROR) << "load tray icon failed, res: " << res << ", path: " << imagePath;
	}

	int res = 0;
	if (!_hasTray) {
        Shell_NotifyIcon(NIM_DELETE, &_notifyIconData);
		res = Shell_NotifyIcon(NIM_ADD, &_notifyIconData);
	}
	else {
		res = Shell_NotifyIcon(NIM_MODIFY, &_notifyIconData);
	}
    _hasTray = true;

	if (!res) {
		UC_LOG(ERROR) << "modify or add tray error, res: " << ::GetLastError();
	}
	UC_LOG(INFO) << "tray is set, new trayId: " << _trayId;
}

void Tray::SetTrayBalloon(ToolTipIcon icon, const std::wstring& title, const std::wstring& text, int timeOut){
	_notifyIconData.uFlags = NIF_INFO;
	_notifyIconData.dwInfoFlags = NIIF_WARNING;
	wcsncpy_s(_notifyIconData.szInfoTitle, sizeof(_notifyIconData.szInfoTitle) / sizeof(WCHAR), title.c_str(), title.size());
	wcsncpy_s(_notifyIconData.szInfo, sizeof(_notifyIconData.szInfo) / sizeof(WCHAR), text.c_str(), text.size());

	_notifyIconData.uTimeout = timeOut;

	Shell_NotifyIcon(NIM_MODIFY, &_notifyIconData);
	_notifyIconData.uFlags ^= NIF_INFO;
}

void Tray::DeleteTray() {
	UC_LOG(INFO) << "delete tray, trayId: " << _trayId;

	if (_trayId > 0) {
		if (!Shell_NotifyIcon(NIM_DELETE, &_notifyIconData)) {
			UC_LOG(ERROR) << "delete tray error, res: " << ::GetLastError();
		}

		_trayId = -1;
	}

	_hasTray = false;
}

void Tray::FlashTray(bool isStart, int interval) {
    if (!_hasTray) {
        return;
    }

    if (isStart && !_isFlash) {
		::SetTimer(_window->GetWnd(), _flashTimerId, interval, NULL);
        _isFlash = true;
        UC_LOG(INFO) << " start flash tray.";
        return;
	}

    if (!isStart && _isFlash) {
        _isFlash = false;
        ::KillTimer(_window->GetWnd(), _flashTimerId);
        _notifyIconData.hIcon = _currIcon;
        Shell_NotifyIcon(NIM_MODIFY, &_notifyIconData);
        UC_LOG(INFO) << " stop flash tray.";
    }
    return;
}

bool Tray::GetIconPosition(RECT &rect) {

    if (!_hasTray) {
        return false;
    }

	struct TANGTBBUTTON
	{
		int iBitmap;
		int idCommand;
		BYTE fsState;
		BYTE fsStyle;
#ifdef _WIN64
		BYTE bReserved[6];
#elif defined(_WIN32)
		BYTE bReserved[2];
#endif
		__int64 dwData;
		INT_PTR iString;
	};

    HWND trayWnd = GetTrayNotifyWnd();

	// 获取系统托盘句柄所属的进程id
	DWORD trayProcessID = -1;
	GetWindowThreadProcessId(trayWnd, &trayProcessID);
	if (trayProcessID <= 0) {
		return false;
	}

	HANDLE hTrayProc = OpenProcess(PROCESS_ALL_ACCESS, 0, trayProcessID);
	if (hTrayProc == NULL) {
		return false;
	}
	
	// 计算托盘下的按钮数量
	int buttonCount = SendMessage(trayWnd, TB_BUTTONCOUNT, 0, 0);
	LPVOID lpData = VirtualAllocEx(hTrayProc, NULL, sizeof(TANGTBBUTTON), MEM_COMMIT, PAGE_READWRITE);
	if (lpData == NULL || buttonCount < 1)
	{
		CloseHandle(hTrayProc);
		return false;
	}

	bool iconFound = false;

	for (int button = 0; button < buttonCount; button++) {
		// 读button数据
		DWORD dwBytesRead = -1;
		TANGTBBUTTON buttonData;
		SendMessage(trayWnd, TB_GETBUTTON, button, (LPARAM)lpData);
		ReadProcessMemory(hTrayProc, lpData, &buttonData, sizeof(TANGTBBUTTON), &dwBytesRead);
		if (dwBytesRead < sizeof(TANGTBBUTTON)) {
			continue;
		}

		// 读button extra数据
		DWORD dwExtraData[2] = { 0, 0 };
		BOOL res = ReadProcessMemory(hTrayProc, (LPVOID)buttonData.dwData, dwExtraData, sizeof(dwExtraData), &dwBytesRead);
		int x = ::GetLastError();
		if (dwBytesRead < sizeof(dwExtraData)) {
			continue;
		}

		HWND hWndOfIconOwner = (HWND)dwExtraData[0];
		int  iIconId = (int)dwExtraData[1];

		if (hWndOfIconOwner != _window->GetWnd()) {
			continue;
		}

		//we found our icon - in WinXP it could be hidden - let's check it:
		if (buttonData.fsState & TBSTATE_HIDDEN) {
			break;
		}

		//now just ask a tool bar of rectangle of our icon
		RECT rcPosition = { 0, 0 };
		SendMessage(trayWnd, TB_GETITEMRECT, button, (LPARAM)lpData);
		ReadProcessMemory(hTrayProc, lpData, &rcPosition, sizeof(RECT), &dwBytesRead);

		if (dwBytesRead < sizeof(RECT)) {
			continue;
		}

		MapWindowPoints(trayWnd, NULL, (LPPOINT)&rcPosition, 2);
		rect = rcPosition;

		iconFound = true;
		break;
	}

	VirtualFreeEx(hTrayProc, lpData, NULL, MEM_RELEASE);
	CloseHandle(hTrayProc);

    if (!iconFound)// 如果没有找到，直接拿到托盘区的最边缘坐标
    {
        GetWindowRect(trayWnd, &rect);
    }
	return iconFound;
}

BOOL Tray::UpdateToolbarWndIcon()
{
    HWND hWnd = GetTrayNotifyWnd();
    if (hWnd)
    {
        RECT rcNotify;
        int x = 0, y = 0;
        ::GetWindowRect(hWnd, &rcNotify);
        for (x = 1; x < rcNotify.right - rcNotify.left - 1; x += 8)
        {
            for (y = 1; y < rcNotify.bottom - rcNotify.top - 1; y += 8)
            {
                ::SendMessage(hWnd, WM_MOUSEMOVE, 0, MAKELPARAM(x, y));
            }
        }
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

HWND Tray::GetTrayNotifyWnd() {
	HWND hwnd = FindWindow(L"Shell_TrayWnd", NULL);
	if (hwnd != NULL) {
		if ((hwnd = FindWindowEx(hwnd, 0, L"TrayNotifyWnd", NULL)) != NULL) {
			HWND hWndPaper = FindWindowEx(hwnd, 0, L"SysPager", NULL);
			if (hWndPaper != NULL) {
				hwnd = FindWindowEx(hWndPaper, 0, L"ToolbarWindow32", NULL);
			}
			else {
				hwnd = FindWindowEx(hwnd, 0, L"ToolbarWindow32", NULL);
			}
		}
	}
	return hwnd;
}

void Tray::GetTrayWndPosition(RECT &rect) {
	HWND hwnd = FindWindow(L"Shell_TrayWnd", NULL);
	if (hwnd != NULL) {
		if ((hwnd = FindWindowEx(hwnd, 0, L"TrayNotifyWnd", NULL)) != NULL) {
            GetWindowRect(hwnd, &rect);
		}
	}
}

bool Tray::OnSysTray(WPARAM wParam, LPARAM lParam)  {
    if (!_hasTray) {
        return true;
    }

	if (lParam == WM_RBUTTONUP || lParam == WM_CONTEXTMENU) {
		ShowContextMenu();
	}
	else if (lParam == WM_LBUTTONUP) {
// 		if (_window->_winStatus != CWS_MAXIMIZED) {
// 			_window->Restore();
// 		}
        _window->Show();
        FlashTray(false);
	}
	else if (lParam == WM_MOUSEMOVE)
    {
        if (_isFlash) // 鼠标进入，并且图标在闪
        {
            GetCursorPos(&_pt);
            if (!_checkTimerOpenFlag)
            {
                ::SetTimer(_window->GetWnd(), _checkTimerID, MOUSE_POINT_CHECK_TIMEOUT_MS, NULL);
            }
        }
	}
// 	else if (lParam == WM_LBUTTONDOWN) {
//         _window->Restore();
//         FlashTray(false);
// 	}

	return true;
}

void Tray::ShowContextMenu()
{
    if (!_hasTray) {
        return;
    }

	POINT pt;
	GetCursorPos(&pt);

	HMENU menu = CreatePopupMenu();
	if (menu)
	{
		for (std::size_t i = 0; i < _menuFeatures.size(); ++i) {
			MENUITEMINFO mif;
			mif.cbSize = sizeof(MENUITEMINFO);
			mif.dwItemData = NULL;
			mif.dwTypeData = (LPWSTR)_menuFeatures[i].label.c_str();
			mif.cch = _menuFeatures[i].label.size();
			mif.fMask = MIIM_ID;// | MIIM_STRING;  
			mif.fType = MIIM_STRING;
			mif.wID = _menuFeatures[i].id;

			if (!_menuFeatures[i].label.empty()) {
				mif.fMask |= MIIM_STRING;

				if (!_menuFeatures[i].icon.empty()) {
					mif.fMask |= MIIM_BITMAP;

					CxImage image;
					image.Load(_menuFeatures[i].icon.c_str());
					HBITMAP bitmap = image.MakeBitmap();
					mif.hbmpItem = bitmap;
				}
			}
			else {
				mif.fMask |= MIIM_TYPE;
				mif.fType = MF_SEPARATOR;
			}

			InsertMenuItem(menu, i, TRUE, &mif);
		}

		//SendMessage(_hwnd, WM_NULL, 0, 0);
        HWND hwndTmp = _window->GetWnd();
        SetForegroundWindow(hwndTmp);
        TrackPopupMenu(menu, TPM_BOTTOMALIGN, pt.x, pt.y, 0, hwndTmp, NULL);
        ::PostMessage(hwndTmp, WM_NULL, 0, 0);
		//DestroyMenu(menu);
	}
}

bool Tray::OnTimer(WPARAM wParam) {
    if (!_hasTray) {
        return false;
    }

	if (wParam == _flashTimerId) {
        // 先处理容错，本不该闪的情况，直接停止闪烁
        if (_window->_hwnd == ::GetForegroundWindow()) {
            ::KillTimer(_window->GetWnd(), _flashTimerId);
            _notifyIconData.hIcon = _currIcon;
            Shell_NotifyIcon(NIM_MODIFY, &_notifyIconData);
            _isFlash = false;
            return true;
        }

		static bool hasIcon = true;
		if (hasIcon) {
			_notifyIconData.hIcon = _currIcon;
		}
		else {
			_notifyIconData.hIcon = _oldIcon;
		}
		hasIcon = !hasIcon;

        if (_isFlash) {
		    Shell_NotifyIcon(NIM_MODIFY, &_notifyIconData);
        }
		return true;
	}
    else if (wParam == _checkTimerID)
    {
        // 鼠标进入和退出的操作，请参考网络 博客http://blog.163.com/zjlovety@126/blog/static/22418624201142763542917
        int i, j;
        POINT pt = { 0 };
        GetCursorPos(&pt); //' 120毫秒后再次得到鼠标坐标
        i = pt.x - _pt.x;
        j = pt.y - _pt.y;
        if (i == 0 && j == 0 && _posX == 0 && _posY == 0) // 坐标没变，说明鼠标第一次停住了
        {
            _posX = pt.x;// 保存停留的数据
            _posY = pt.y;

            std::wstringstream jsScript;
            jsScript << L"var e = new CustomEvent(\"" << MOUSE_IN_ICON_EVENT
                << "\",{detail:" << "{},});" << "document.dispatchEvent(e);";
            _window->NotifyMouseEvent(jsScript.str());
        }
        else if (CheckMouseOut(pt.x, pt.y, _posX, _posY))
        {
            _posX = 0;
            _posY = 0;

            std::wstringstream jsScript;
            jsScript << L"var e = new CustomEvent(\"" << MOUSE_LEAVE_ICON_EVENT
                << "\",{detail:" << "{},});" << "document.dispatchEvent(e);";
            _window->NotifyMouseEvent(jsScript.str());
        }
        ::KillTimer(_window->GetWnd(), _checkTimerID);
        _checkTimerOpenFlag = false;

        return true;
    }
	return false;
}

bool Tray::OnClose() {
	DeleteTray();

	return false;
}

bool Tray::CheckMouseOut(int nx, int ny, int ix, int iy) {
    RECT rectIcon, rectTray;
    GetTrayWndPosition(rectTray);
    if (GetIconPosition(rectIcon))
    {
        if ((nx - rectIcon.left) > (rectIcon.right - rectIcon.left - 2) || (nx < rectIcon.left + 1))
        {
            return true;
        }

        if (rectTray.bottom == rectIcon.bottom)
        {
            if (ny - rectIcon.top > rectIcon.bottom - rectIcon.top - 1 || ny < rectIcon.top + 1)
            {
                return true;
            }
            else if (ny - rectIcon.top > rectIcon.bottom - rectIcon.top - 2 || ny < rectIcon.top + 2)
            {
                return true;
            }
        }
    }
    else
    {
        int hFlowArea;
        RECT rect;
        HWND hwnd = FindWindow(L"NotifyIconOverflowWindow", NULL);
        GetWindowRect(hwnd, &rect);
        int wFlow = rect.right - rect.left;
        int hFlow = rect.bottom - rect.top;
        hFlowArea = hFlow - 75;

        if ((ny - rect.top - 15 - 32 * int((iy - rect.top -15) / 32) > 30)
            || (ny < rect.top + 15 + 32 * int((iy - rect.top - 15)/32)+2))
        {
            return true;
        }
        else if (wFlow > 93)
        {
            if ((nx - rect.left - 16 - 32*int((ix - rect.left - 16)/32) > 30)
                || (nx < rect.left + 16 + 32*int((ix - rect.left - 16)/32) + 1))
            {
                return true;
            }
            else if ((nx - rect.left - 29 - 32 * int((ix - rect.left - 29) / 32) > 30)
                || (nx < rect.left + 29 + 32 * int((ix - rect.left - 29) / 32) + 1))
            {
                return true;
            }
        }
    }
    return false;
}

}