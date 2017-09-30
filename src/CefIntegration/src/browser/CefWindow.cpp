#include "browser/CefWindow.h"
#include "LogManager.h"
#include "utils/FileUtil.h"
#include "ClientDefs.h"
#include "CefClient.h"
#include "cximage/ximage.h"
#include <Windowsx.h>
#include <sstream>
#include <string.h>
#include "browser/WindowManager.h"
#include "browser/JsExtDef.h"
#include "browser/OsrUtil.h"
#include "browser/drag_drop/geometry_util.h"
#include "common/StringConvert.h"

extern HINSTANCE hModuleInst;

namespace cefIntegration {

using namespace uc;

#define WM_BEFORE_SUB_WINDOW_CREATE		(WM_USER + 2)
#define WM_ACTIVE_MEETING (WM_USER + 3)		//托盘菜单，用于激活云会议（预留）

static const wchar_t  windowPointerProperty[] = L"Window Pointer";
static const int HITTEST_EDGE_GAP = 5;
static HHOOK hook = NULL;
static CefWindow *window = NULL;

static LRESULT CALLBACK HookProc(int code, WPARAM wParam, LPARAM lParam) {
	if (code != HCBT_CREATEWND){
		return CallNextHookEx(hook, code, wParam, lParam);
	}

	LPCREATESTRUCT lpcs = ((LPCBT_CREATEWND)lParam)->lpcs;
	if (code == HCBT_CREATEWND && lpcs->lpCreateParams && lpcs->lpCreateParams == (LPVOID)window) {
		HWND hWnd = (HWND)wParam;
		::SetProp(hWnd, windowPointerProperty, (HANDLE)window);
	}

	return CallNextHookEx(NULL, code, wParam, lParam);
}

static void RegisterWindowCreateHook(CefWindow *win) {
	if (hook) {
		return;
	}

	hook = ::SetWindowsHookEx(WH_CBT, HookProc, NULL, ::GetCurrentThreadId());
	window = win;
}

static void UnRegisterWindowCreateHook() {
	::UnhookWindowsHookEx(hook);
	hook = NULL;
	window = NULL;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	CefWindow* window = (CefWindow*)::GetProp(hWnd, windowPointerProperty);

	if (window) {
		return window->WindowProc(hWnd, message, wParam, lParam);
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

ATOM CefWindow::RegisterWndClass()
{
	static ATOM classAtom = 0;
	if (classAtom == 0)
	{
		WNDCLASSEX wcex;

		::ZeroMemory(&wcex, sizeof(wcex));
		wcex.cbSize = sizeof(WNDCLASSEX);

		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = WndProc;
		wcex.hInstance = ::hModuleInst;
		wcex.hCursor = ::LoadCursor(NULL, IDC_ARROW);
		wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wcex.lpszClassName = UCCLIENT_CLASS_NAME;
		wcex.hIcon = LoadIcon(::hModuleInst, MAKEINTRESOURCE(103));

		classAtom = RegisterClassEx(&wcex);
	}
	return classAtom;
}

CefWindow::CefWindow(const std::wstring& name, CefClient *client) : _client(client),
_hwnd(NULL), _isModal(false), _name(name), _isResizeable(false), _minWidth(0), _minHeight(0), _winStatus(CWS_HIDE), _preStatus(CWS_HIDE),
_trackingMouse(FALSE), _behaviorIfSCClose(BSC_CLOSE_WINDOW), _leftBorder(0), _topBorder(0), _rightBorder(0), _bottomBorder(0), _closed(false),
_closedWindownOnly(false), _deviceScaleFactor(OsrUtil::GetDeviceScaleFactor()), _tray(NULL), _uTaskbarRestartMessage(0){
}

CefRefPtr<CefBrowser> CefWindow::GetBrowser() {
	return _client->getHandler()->GetBrowser(_hwnd);
}

void CefWindow::Create(const WindowFeature& feature, std::wstring path, HWND hwndParent) {
    UC_LOG(INFO) << "create window, name:" << feature.title << ", path: " << path << ", enableDrag: " << feature.enableDrag << ", behaviorIfSCClose: " << feature.behaviorIfSCClose
        << ", modal:" << feature.modal << ",left:" << feature.left << ",top:" << feature.top << ",width:" << feature.width << ",height:" << feature.height;

    UC_LOG(INFO) << "_deviceScaleFactor: " << _deviceScaleFactor;
	// register window class
	static bool registered = false;
	if (!registered) {
		RegisterWndClass();
		registered = true;
	}

	// modal模式必有父窗口
	if ((!feature.modal) && (!feature.isChild)) {
		hwndParent = NULL;
	}

	if ((hwndParent != NULL)) {
		::SendMessage(hwndParent, WM_BEFORE_SUB_WINDOW_CREATE, NULL, NULL);
	}

	// create window
	RegisterWindowCreateHook(this);
	createWindow(_hwnd, feature.isLayered, feature.title, hwndParent);
	UnRegisterWindowCreateHook();

    if (NULL == _hwnd) {
        UC_LOG(ERROR) << "createWindow failed, path: " << path;
    }
	InitializeFeature(feature, hwndParent);

	CreateBrowser(path);

	if (feature.isShow) {
		::ShowWindow(_hwnd, SW_SHOWNORMAL);
		::UpdateWindow(_hwnd);
        SetWinstatus(CWS_NORMAL);
	    UC_LOG(INFO) << "CefWindow::Create path: " << path  << ", is show, hwnd: " << this->GetWnd();
	}
	
	if (feature.isTop) {
		SetTop(true);
	}

	UC_LOG(INFO) << "CefWindow::Create path: " << path  << ", hwnd: " << this->GetWnd();
}

void CefWindow::InitializeFeature(const WindowFeature& feature, HWND hwndParent) {
	
	if (!feature.hasTaskBarIcon) {
		RemoveTaskBarIcon();
	}

	_isModal = feature.modal;
	if (_isModal) {
		::EnableWindow(hwndParent, false);
	}

	_isResizeable = feature.resizable;
	_minWidth = LogicalToDevice(feature.minWidth, _deviceScaleFactor);
	_minHeight = LogicalToDevice(feature.minHeight, _deviceScaleFactor);
	_behaviorIfSCClose = feature.behaviorIfSCClose;

	_leftBorder = LogicalToDevice(feature.leftBorder, _deviceScaleFactor);
	_topBorder = LogicalToDevice(feature.topBorder, _deviceScaleFactor);
	_rightBorder = LogicalToDevice(feature.rightBorder, _deviceScaleFactor);
	_bottomBorder = LogicalToDevice(feature.bottomBorder, _deviceScaleFactor);

	SetSize(feature.width, feature.height);
	if (feature.centerScreen) {
		CenterWindow();
	}
	else {
        Move(feature.left, feature.top);
	}

	SetCaptionArea(feature.captions);

	EnableDrag(feature.enableDrag);

	if (feature.isMaximize) {
		Maximize();
	}
}

void CefWindow::SetIconWithNumber(int number) {
	HICON hIcon = LoadIcon(::hModuleInst, MAKEINTRESOURCE(103));
	if (number != 0) {
		CxImage img;
		bool res = img.CreateFromHICON(hIcon);

		CxImage::CXTEXTINFO cti;
		img.InitTextInfo(&cti);
		cti.lfont.lfCharSet = ANSI_CHARSET;
		lstrcpy(cti.lfont.lfFaceName, _T("Arial"));
		cti.lfont.lfWeight = FW_THIN;
		cti.lfont.lfHeight = 12;
		cti.bcolor = RGB(255, 0, 0);
		cti.fcolor = RGB(255, 255, 255);
		cti.b_round = 100;

		if (number > 99) {
			cti.lfont.lfHeight = 10;
			::_itow(99, cti.text, 10);
			wcscat(cti.text, L"+");
		}
		else {
			::_itow(number, cti.text, 10);
		}
		img.DrawStringEx(NULL, img.GetWidth() - 14, 14, &cti);

		hIcon = img.MakeIcon();

	}

	SendMessage(_hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
}
void CefWindow::EnableDrag(bool enable) {
	::DragAcceptFiles(_hwnd, enable ? TRUE : FALSE);
}

void CefWindow::RemoveTaskBarIcon() {
	DWORD exStyle = ::GetWindowLong(_hwnd, GWL_EXSTYLE);
	::SetWindowLong(_hwnd, GWL_EXSTYLE, exStyle | WS_EX_TOOLWINDOW);
}

void CefWindow::SetTop(bool isTop) {
	if (isTop) {
		::SetWindowPos(_hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	}
	else {
		::SetWindowPos(_hwnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	}
}

void CefWindow::Minimize() {
	::ShowWindow(_hwnd, SW_MINIMIZE);
}

void CefWindow::Maximize() {
	::ShowWindow(_hwnd, SW_MAXIMIZE);
}

void CefWindow::FullScreen() {
	HWND    hDesk;
	RECT    rc;
	hDesk = ::GetDesktopWindow();
	::GetWindowRect(hDesk, &rc);
	::SetWindowLong(_hwnd, GWL_STYLE, WS_BORDER);
	::SetWindowPos(_hwnd, HWND_TOPMOST, 0, 0, rc.right, rc.bottom, SWP_SHOWWINDOW);
}

void CefWindow::Restore() {
	::ShowWindow(_hwnd, SW_RESTORE);
    ::SetForegroundWindow(_hwnd);
}

void CefWindow::Show() {
	if (_winStatus == CWS_MINIMIZED) {
		Restore();
	}
	else if (_winStatus == CWS_HIDE){
		SetWinstatus(_preStatus != CWS_HIDE ? _preStatus : CWS_NORMAL);
	}

	::ShowWindow(_hwnd, SW_SHOW);
	::UpdateWindow(_hwnd);

	DWORD dwThreadProcessID = GetWindowThreadProcessId(::GetForegroundWindow(), NULL);
	AttachThreadInput(dwThreadProcessID, GetCurrentThreadId(), TRUE);
	::SetForegroundWindow(_hwnd);
	AttachThreadInput(dwThreadProcessID, GetCurrentThreadId(), FALSE);
}

void CefWindow::Hide() {
	::ShowWindow(_hwnd, SW_HIDE);
	SetWinstatus(CWS_HIDE);
}

void CefWindow::Close() {
	::SendMessage(_hwnd, WM_CLOSE, 0, 0);
}

void CefWindow::CloseWindowOnly() {
	_closedWindownOnly = true;
	Close();
}

void CefWindow::SetSize(int width, int height) {
	width = LogicalToDevice(width, _deviceScaleFactor);
	height = LogicalToDevice(height, _deviceScaleFactor);

	::SetWindowPos(_hwnd, 0, 0, 0, width, height, SWP_NOMOVE | SWP_NOZORDER);
}

void CefWindow::GetWindowPos(int& x, int& y, int& cx, int& cy) {
	RECT    rc;
	GetWindowRect(_hwnd, &rc);
	x = rc.left;
	y = rc.top;

	cx = rc.right;
	cy = rc.bottom;
}

void CefWindow::DisableDBClick() {
	_isResizeable = false;
}

void CefWindow::EnableDBClick() {
	_isResizeable = true;
}

void CefWindow::Move(int x, int y, bool transfer) {
    if ( x < 0 ) {
        x = 0;
    }
    if (y < 0) {
        y = 0;
    }

    if (transfer) {
        ::SetWindowPos(_hwnd, 0, LogicalToDevice(x, _deviceScaleFactor), LogicalToDevice(y, _deviceScaleFactor),
            0, 0, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOSIZE);
        return;
    }
	::SetWindowPos(_hwnd, 0, x, y, 0, 0, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOSIZE);
}

void CefWindow::CenterWindow() {
	HWND hwndParent;
	RECT rectWindow, rectParent;

	GetWindowRect(_hwnd, &rectWindow);
	int nWidth = rectWindow.right - rectWindow.left;
	int	nHeight = rectWindow.bottom - rectWindow.top;

	int nX, nY;

	if ((hwndParent = GetParent(_hwnd)) != NULL) {
		GetWindowRect(hwndParent, &rectParent);
	}
	else {
		rectParent.left = rectParent.top = 0;
		rectParent.right = GetSystemMetrics(SM_CXSCREEN);
		rectParent.bottom = GetSystemMetrics(SM_CYSCREEN);
	}

	nX = ((rectParent.right - rectParent.left) - nWidth) / 2 + rectParent.left;
	nY = ((rectParent.bottom - rectParent.top) - nHeight) / 2 + rectParent.top;

	Move(nX, nY, false);
}

void CefWindow::Navigate(const std::wstring &url) {
	CefRefPtr<CefFrame> frame = GetBrowser()->GetMainFrame();
	frame->LoadURL(url);
}

void CefWindow::SetForeground() {
	::SetForegroundWindow(_hwnd);
}

void CefWindow::SetCaptionArea(const std::vector<RECT> & captions) {
	_captionAreas.clear();
	for (int i = 0; i < captions.size(); ++i) {
		RECT rect;
		rect.left = LogicalToDevice(captions[i].left, _deviceScaleFactor);
		rect.top = LogicalToDevice(captions[i].top, _deviceScaleFactor);
		rect.right = LogicalToDevice(captions[i].right, _deviceScaleFactor);
		rect.bottom = LogicalToDevice(captions[i].bottom, _deviceScaleFactor);

		_captionAreas.push_back(rect);
	}
}

bool CefWindow::OnClose() {
    UC_LOG(INFO) << "OnClose:" << _name << ",hwnd:" << _hwnd;

	_closed = true;

	if (_isModal) {
		HWND hwndParent = GetParent(_hwnd);
		if (hwndParent != NULL) {
			::EnableWindow(hwndParent, true);
		}
	}

    if (NULL != _tray)
    {
        _tray->OnClose();
        Tray::GetInstance()->Release();
    }

	if (!_closedWindownOnly) {
		GetBrowser()->GetHost()->CloseBrowser(false);
	}
	
	return false;
}

bool CefWindow::IsInCaption(POINT &pt) {
	if (_captionAreas.empty()) {
		return false;
	}

	RECT rect;
	::GetClientRect(_hwnd, &rect);

	// support custom caption
	for (std::size_t i = 0; i < _captionAreas.size(); ++i) {
		RECT caption = _captionAreas[i];
		if (_captionAreas[i].right - _captionAreas[i].left <= 0) {
			caption.right = rect.right - (_captionAreas[i].left - _captionAreas[i].right);
		}

		if (_captionAreas[i].bottom - _captionAreas[i].top <= 0) {
			caption.bottom = rect.bottom - (_captionAreas[i].top - _captionAreas[i].bottom);
		}

		if (::PtInRect(&caption, pt)) {
			return true;
		}
	}

	return false;
}

UINT CefWindow::OnNCHitTest(WPARAM wParam, LPARAM lParam) {
	POINT pt;
	pt.x = GET_X_LPARAM(lParam);
	pt.y = GET_Y_LPARAM(lParam);

	ScreenToClient(_hwnd, &pt);

	if (IsInCaption(pt)) {
		return HTCAPTION;
	}

	if (_isResizeable && (_winStatus != CWS_MAXIMIZED)) {
		RECT clientRect;
		::GetClientRect(_hwnd, &clientRect);

		int edge = LogicalToDevice(HITTEST_EDGE_GAP, _deviceScaleFactor);
		RECT edgeRect;

		edgeRect.left = clientRect.right - edge - _rightBorder;
		edgeRect.top = clientRect.bottom - edge - _bottomBorder;
		edgeRect.right = clientRect.right - _rightBorder;
		edgeRect.bottom = clientRect.bottom - _bottomBorder;
		if (::PtInRect(&edgeRect, pt)) {
			return HTBOTTOMRIGHT;
		}

		edgeRect.left = clientRect.left + _leftBorder;
		edgeRect.top = clientRect.bottom - edge - _bottomBorder;
		edgeRect.right = clientRect.left + edge + _leftBorder;
		edgeRect.bottom = clientRect.bottom - _bottomBorder;
		if (::PtInRect(&edgeRect, pt)) {
			return HTBOTTOMLEFT;
		}

		edgeRect.left = clientRect.left + _leftBorder;
		edgeRect.top = clientRect.top + _topBorder;
		edgeRect.right = clientRect.left + edge + _leftBorder;
		edgeRect.bottom = clientRect.top + edge + _topBorder;
		if (::PtInRect(&edgeRect, pt)) {
			return HTTOPLEFT;
		}

		edgeRect.left = clientRect.right - edge - _rightBorder;
		edgeRect.top = clientRect.top + _topBorder;
		edgeRect.right = clientRect.right - _rightBorder;
		edgeRect.bottom = clientRect.top + edge + _topBorder;
		if (::PtInRect(&edgeRect, pt)) {
			return HTTOPRIGHT;
		}

		edgeRect.left = clientRect.left + _leftBorder;
		edgeRect.top = clientRect.top + _topBorder;
		edgeRect.right = clientRect.left + edge + _leftBorder;
		edgeRect.bottom = clientRect.bottom - _bottomBorder;

		if (::PtInRect(&edgeRect, pt)) {
			return HTLEFT;
		}

		edgeRect.left = clientRect.right - edge - _rightBorder;
		edgeRect.top = clientRect.top + _topBorder;
		edgeRect.right = clientRect.right - _rightBorder;
		edgeRect.bottom = clientRect.bottom - _bottomBorder;
		if (::PtInRect(&edgeRect, pt)) {
			return HTRIGHT;
		}

		edgeRect.left = clientRect.left + _leftBorder;
		edgeRect.top = clientRect.bottom - edge - _bottomBorder;
		edgeRect.right = clientRect.right - _rightBorder;
		edgeRect.bottom = clientRect.bottom - _bottomBorder;
		if (::PtInRect(&edgeRect, pt)) {
			return HTBOTTOM;
		}

		edgeRect.left = clientRect.left + _leftBorder;
		edgeRect.top = clientRect.top + _topBorder;
		edgeRect.right = clientRect.right - _rightBorder;
		edgeRect.bottom = clientRect.top + edge + _topBorder;
		if (::PtInRect(&edgeRect, pt)) {
			return HTTOP;
		}
	}

	return DefWindowProc(_hwnd, WM_NCHITTEST, wParam, lParam);
}

bool CefWindow::OnTimer(WPARAM wParam) {
    Tray::GetInstance()->OnTimer(wParam);
    return true;
}

bool CefWindow::OnGetMinMaxInfo(LPARAM lParam) {
	RECT rect;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &rect, 0);

	MINMAXINFO *mminfo = (PMINMAXINFO)lParam;
	mminfo->ptMaxSize.x = rect.right - rect.left;
	mminfo->ptMaxSize.y = rect.bottom - rect.top;

	mminfo->ptMaxPosition.x = 0;
	mminfo->ptMaxPosition.y = 0;

	mminfo->ptMinTrackSize.x = _minWidth;
	mminfo->ptMinTrackSize.y = _minHeight;

	return true;
}

void CefWindow::ReSetTray() {
    UC_LOG(INFO) << "reset tray: title:" << _trayTitle << ", imagepath:" << _trayImagePath;
    if (NULL != _tray) {
        delete _tray;
    }
	if (_trayTitle.empty())
	{
		UC_LOG(ERROR) << "reset tray but title.empty()" << _hwnd;
	}
    _tray = Tray::GetInstance(); // 主面板才会调用该接口，所以初始化一次
    _tray->SetTray(this, _trayTitle, _trayImagePath, false, _trayMenuFeatures);
}

void CefWindow::SetTray(const std::wstring & title, const std::wstring & imagePath, bool isFlash,
	std::vector<MenuFeature> &menuFeatures) {

    UC_LOG(INFO) << "register taskbarcreated message."; // 主面板设置托盘图标时才注册重绘消息
    _uTaskbarRestartMessage = ::RegisterWindowMessage(TEXT("TaskbarCreated"));

    if (NULL == _tray) {
        UC_LOG(INFO) << "set tray: title:" << title << ", imagepath:" << imagePath;
        _tray = Tray::GetInstance(); // 主面板才会调用该接口，所以初始化一次
    }
    _tray->SetTray(this, title, imagePath, isFlash, menuFeatures);

    _trayTitle = title;
    _trayImagePath = imagePath;
    _trayMenuFeatures = menuFeatures;
}

void CefWindow::FlashTray(bool isFlash, int interval) {
    Tray::GetInstance()->FlashTray(isFlash, interval);
}

void CefWindow::GetTrayPos(std::wstring & trayPos) {
    RECT rect;

    Tray::GetInstance()->GetIconPosition(rect);

    int x = DeviceToLogical(rect.left, _deviceScaleFactor);
    int y = DeviceToLogical(rect.top, _deviceScaleFactor);
    int  width = DeviceToLogical(GetSystemMetrics(SM_CXSCREEN), _deviceScaleFactor);

    // {"x":1,"y":2,"w":3}
    wstringstream ss;
    ss << L"{\"x\":" << x << L",\"y\":" << y << L",\"w\":" << width - x << L"}";
    trayPos = ss.str();
    UC_LOG(INFO) << "GetTrayPos: " << trayPos; // 记录c++给js返回的pos位置
}

void CefWindow::SetTrayBalloon(ToolTipIcon icon, const std::wstring& title, const std::wstring& text, int timeOut) {
	Tray::GetInstance()->SetTrayBalloon(icon, title, text, timeOut);
}

void CefWindow::FlashWindow(UINT nCount, UINT nTimeout) {
	if (GetActiveWindow() == _hwnd) {
		return;
	}

	FLASHWINFO fwinfo;
	fwinfo.cbSize = sizeof(FLASHWINFO);
	fwinfo.hwnd = _hwnd;
	fwinfo.dwFlags = 3;
	fwinfo.uCount = nCount;
	fwinfo.dwTimeout = nTimeout;

	FlashWindowEx(&fwinfo);
}

bool CefWindow::OnMouseMove() {
	if (_trackingMouse == FALSE) {
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(TRACKMOUSEEVENT);
		tme.dwFlags = TME_HOVER | TME_LEAVE;
		tme.hwndTrack = _hwnd;
		tme.dwHoverTime = 500;
		TrackMouseEvent(&tme);
		_trackingMouse = TRUE;
	}

	return false;
}

bool CefWindow::OnNCLButtonDBClick() {
	if (!_isResizeable) {
		return true;
	}

	if (_winStatus == CWS_MAXIMIZED) {
		Restore();
	}
	else {
		Maximize();
	}

	return true;
}

bool CefWindow::OnSize(WPARAM wParam) {
	if (SIZE_RESTORED == wParam) {
		if (_winStatus == CWS_MAXIMIZED) {
			CefRefPtr<CefBrowser> browser = this->GetBrowser();
			if (browser != NULL && browser.get()) {
				std::wstringstream jsScript;
				jsScript << L"var e = new CustomEvent('" << RESTORED_FROM_MAXIMIZED_EVENT << "');document.dispatchEvent(e);";
				browser->GetMainFrame()->ExecuteJavaScript(jsScript.str(), "", 0);
			}
		}

		SetWinstatus(CWS_NORMAL);
	}
	else if (SIZE_MAXIMIZED == wParam) {
		if (_winStatus == CWS_NORMAL) {
			CefRefPtr<CefBrowser> browser = this->GetBrowser();
			if (browser != NULL && browser.get()) {
				std::wstringstream jsScript;
				jsScript << L"var e = new CustomEvent('" << MAXIMIZED_FROM_NORMAL_EVENT << "');document.dispatchEvent(e);";
				browser->GetMainFrame()->ExecuteJavaScript(jsScript.str(), "", 0);
			}
		}

		SetWinstatus(CWS_MAXIMIZED);
	}
	else if (SIZE_MINIMIZED == wParam) {
		SetWinstatus(CWS_MINIMIZED);
	}

	return false;
}

void CefWindow::SetWinstatus(CurrentWindowStatus status) {
	if (status == CWS_HIDE) {
		if (_winStatus != CWS_HIDE) {
			_preStatus = _winStatus;
		}
		_winStatus = status;
	}
	else {
		_preStatus = _winStatus;
		_winStatus = status;
	}
}

bool CefWindow::OnMouseHover() {
	_trackingMouse = FALSE;

	return true;
}

bool CefWindow::OnMouseLeave() {
	_trackingMouse = FALSE;

	return false;
}

bool CefWindow::OnSysCommand(WPARAM wParam) {
    if (wParam == SC_CLOSE) {
        if (_behaviorIfSCClose == BSC_HIDE) {
            Hide();
        }
        else if (_behaviorIfSCClose == BSC_QUIT) {
            _client->getHandler()->Quit();
        }
        else if (_behaviorIfSCClose == BSC_MINIMIZE) {
            ShowWindow(_hwnd, SW_MINIMIZE);
        }
        else {
            return false;
        }
        return true;
    }
    else if (wParam == SC_RESTORE) {
//         ::ShowWindow(_hwnd, SW_RESTORE);
//         ::SetForegroundWindow(_hwnd);
        Show();
        Tray::GetInstance()->FlashTray(false);
        return true;
    }

    return false;
}

bool CefWindow::OnCommand(WPARAM wParam) {
	int  menuId = LOWORD(wParam);
	std::wstringstream jsScript;
	jsScript << L"var e = new CustomEvent('" << MENU_CLICKED_EVENT << "',{detail:" << "{id: " << menuId << "},});document.dispatchEvent(e);";
	this->GetBrowser()->GetMainFrame()->ExecuteJavaScript(jsScript.str(), "", 0);

	return false;
}

bool CefWindow::OnHotKey(WPARAM wParam) {
	int  identifier = LOWORD(wParam);
	std::wstringstream jsScript;
	jsScript << L"var e = new CustomEvent('" << HOTKEY_EVENT << "',{detail:" << "{id: " << identifier << "},});document.dispatchEvent(e);";
	_client->getHandler()->ExecuteJSForAllBrowsers(jsScript.str());

	return false;
}


bool CefWindow::OnDestory() {
    UC_LOG(INFO) << "OnDestory： " << _name << ",hwnd:" << _hwnd;

	if (!_closed) {
		OnClose();
	}

	RemoveProp(_hwnd, windowPointerProperty);
	_client->getWindowManager()->RemoveWindow(_hwnd);

	return true;
}

void CefWindow::RegisterHotKey(int id, int modifiers, int key) {
	::UnregisterHotKey(_hwnd, id);
	BOOL res = ::RegisterHotKey(_hwnd, id, modifiers, key);
	if (res == 0) {
		UC_LOG(ERROR) << "register hotkey error, res" << ::GetLastError();
	}
}

void CefWindow::UnRegisterHotKey(int id) {
	::UnregisterHotKey(_hwnd, id);
}

bool CefWindow::OnBeforeSubWindowCreate() {
	if (_winStatus == CWS_MINIMIZED) {
		Restore();
	}

	return true;
}

LRESULT CefWindow::WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
    case WM_ACTIVATE: 
        PROCESS_MESSAGE(OnWindowActive(wParam));
	case WM_NCHITTEST:
		return OnNCHitTest(wParam, lParam);
	case WM_GETMINMAXINFO:
		PROCESS_MESSAGE(OnGetMinMaxInfo(lParam));
	case WM_NCLBUTTONDBLCLK:
		PROCESS_MESSAGE(OnNCLButtonDBClick());
	case WM_SIZE:
		PROCESS_MESSAGE(OnSize(wParam));
	case WM_TIMER:
		PROCESS_MESSAGE(OnTimer(wParam));
	case WM_CLOSE:
		PROCESS_MESSAGE(OnClose());
	case WM_COMMAND:
		PROCESS_MESSAGE(OnCommand(wParam));
	case WM_SYSCOMMAND:
		PROCESS_MESSAGE(OnSysCommand(wParam));
	case WM_SYS_TRAY:
        PROCESS_MESSAGE(Tray::GetInstance()->OnSysTray(wParam, lParam));
	case WM_MOUSEMOVE:
		PROCESS_MESSAGE(OnMouseMove());
	case WM_MOUSELEAVE:
		PROCESS_MESSAGE(OnMouseLeave());
	case WM_MOUSEHOVER:
		PROCESS_MESSAGE(OnMouseHover());
	case WM_HOTKEY:
		PROCESS_MESSAGE(OnHotKey(wParam));
	case WM_DESTROY:
		PROCESS_MESSAGE(OnDestory());
	case WM_BEFORE_SUB_WINDOW_CREATE:
		PROCESS_MESSAGE(OnBeforeSubWindowCreate());
	case WM_ACTIVE_MEETING:
		PROCESS_MESSAGE(OnActiveMeeting());
	case WM_COPYDATA:
		PROCESS_MESSAGE(OnCopyData((COPYDATASTRUCT*)lParam));
	default:
        if (message != WM_NULL && (message == _uTaskbarRestartMessage)) {
            ReSetTray(); //重画系统图标 
        }
        break;
	}

    return DefWindowProc(hWnd, message, wParam, lParam);
}

void CefWindow::NotifyMouseEvent(const std::wstring &code)
{
    _client->ExecuteJSForAllBrowsers(code);
}

bool CefWindow::OnWindowActive(WPARAM wParam)
{
    if ((L"main" == _name) && (wParam == WA_ACTIVE || wParam == WA_CLICKACTIVE)) {
        UC_LOG(INFO) << "main window active, WM_ACTIVATE wparam: " << wParam << ", hwnd:" << _hwnd;
        std::wstringstream jsScript;
        jsScript << L"var e = new CustomEvent(\"" << MAIN_WINDOW_ACTIVE_EVENT
            << "\",{detail:" << "{},});" << "document.dispatchEvent(e);";
        _client->ExecuteJSForAllBrowsers(jsScript.str());
    }
    return true;
}

//用于蜜蜂托盘菜单击活云会议（预留）
bool CefWindow::OnActiveMeeting()
{
	//激活云会议
	HANDLE hEvent = OpenEvent(EVENT_MODIFY_STATE, FALSE, L"Global\\ActiveMeeting");
	if (NULL != hEvent) {
		SetEvent(hEvent);
		return true;
	}

	return false;
}

bool CefWindow::OnCopyData(COPYDATASTRUCT* pCopyDataStruct)
{
	LPCTSTR pszText = (LPCTSTR)(pCopyDataStruct->lpData);
	DWORD dwLength = (DWORD)(pCopyDataStruct->cbData);
	TCHAR szRecvText[MAX_PATH] = { 0 };
	memcpy(szRecvText, pszText, dwLength);
	wstring filePath(szRecvText);
	StringConvert::Replace(filePath, L"\\", L"\\\\");

	std::wstringstream jsScript;
	jsScript << L"var e = new CustomEvent(\"" << SEND_FILE_TO_BEE_EVENT
		<< "\",{detail:" << "{filePath:\"" << filePath
		<< "\"},});" << "document.dispatchEvent(e);";
	_client->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "OnCopyData, file path:" << jsScript.str();
	return true;
}

}