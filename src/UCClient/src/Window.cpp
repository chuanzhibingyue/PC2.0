#include "Window.h"
#include "Windowsx.h"
#include <string.h>
#include "utils/FileUtil.h"
#include <sstream>

extern HINSTANCE hInst;

namespace uc {

static const wchar_t  windowClassname[] = L"UC CLIENT CLASS";
static const wchar_t  windowName[] = L"UC CLIENT WINDOW";
static const wchar_t  windowPointerProperty[] = L"Window Pointer";
static const int HITTEST_EDGE_GAP = 5;
static const int WM_SYS_TRAY =  WM_USER + 1;

Window::Window(const std::wstring& name, CefClient *client) : CefOSRWindow(client), 
	_hwnd(NULL), _isModal(false), _name(name), _isResizeable(false), _minWidth(0),
	_minHeight(0), _trayId(0), _maximized(false) {
}

Window::~Window(){
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	Window* window = (Window*)::GetProp(hWnd, windowPointerProperty);
	
	if (window) {
		 return window->WindowProc(message, wParam, lParam);
	}
	
	return DefWindowProc(hWnd, message, wParam, lParam);
}

void Window::Create(const std::wstring &title, std::wstring &url, HWND hwndParent, bool isModal, bool isResizeable,
	int minWidth, int minHeight, bool hasTaskBarIcon) {
	static bool registered = false;
	if (!registered) {
		RegisterWndClass();
		registered = true;
	}

	_hwnd = ::CreateWindowEx(WS_EX_LAYERED, windowClassname, title.c_str(),
		WS_POPUP&~(WS_CAPTION|WS_BORDER), CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, NULL, NULL, hInst, NULL);

	if (!hasTaskBarIcon) {
		DWORD exStyle = ::GetWindowLong(_hwnd,GWL_EXSTYLE);
		::SetWindowLong(_hwnd,GWL_EXSTYLE, exStyle|WS_EX_TOOLWINDOW);
	}

	_isModal = isModal;
	if (_isModal) {
		::EnableWindow(hwndParent, false);
	}

	_isResizeable = isResizeable;
	_minWidth = minWidth;
	_minHeight = minHeight;

	::SetProp(_hwnd, windowPointerProperty, (HANDLE)this);

	_client->SetRenderHandler(_hwnd, this);
	_client->CreateTransparentBrowser(_hwnd, url);
}

void Window::SetTop(bool isTop) {
	if (isTop) {
		::SetWindowPos(_hwnd,HWND_TOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	}
	else {
		::SetWindowPos(_hwnd,HWND_NOTOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	}
}

void Window::Minimize() {
	::ShowWindow(_hwnd, SW_MINIMIZE);
}

void Window::Maximize() {
	_maximized = true;
	::ShowWindow(_hwnd, SW_MAXIMIZE);
}

void Window::Restore() {
	//if (_maximized) {
		//_maximized = false;
	//}

	::ShowWindow(_hwnd, SW_RESTORE);
}

void Window::Show() {
	// 临时解决输入框点击无效果的问题
	//::ShowWindow(_hwnd, SW_SHOWNOACTIVATE);
	::ShowWindow(_hwnd, SW_NORMAL);
	::UpdateWindow(_hwnd);
}

void Window::Hide() {
	::ShowWindow(_hwnd, SW_HIDE);
}

void Window::Close() {
	::SendMessage(_hwnd , WM_CLOSE, 0, 0);
}

void Window::SetSize(int width, int height) {
	::SetWindowPos(_hwnd, 0, 0, 0, width, height, SWP_NOMOVE | SWP_NOZORDER);
}

void Window::Move(int x, int y) {
    ::SetWindowPos(_hwnd, 0, x, y, 0, 0, SWP_NOZORDER|SWP_NOACTIVATE|SWP_NOSIZE);
}

void Window::CenterWindow() {
	HWND hwndParent;
    RECT rectWindow, rectParent;
	
	GetWindowRect(_hwnd, &rectWindow);
	int nWidth = rectWindow.right - rectWindow.left;
	int	nHeight = rectWindow.bottom - rectWindow.top;

	int nX, nY;

	if ((hwndParent = GetParent(_hwnd)) != NULL)
    {
		GetWindowRect(hwndParent, &rectParent);
	}
	else {
		rectParent.left = rectParent.top = 0;
		rectParent.right = GetSystemMetrics(SM_CXSCREEN);
		rectParent.bottom = GetSystemMetrics(SM_CYSCREEN);
	}
 
	nX = ((rectParent.right - rectParent.left) - nWidth) / 2 + rectParent.left;
    nY = ((rectParent.bottom - rectParent.top) - nHeight) / 2 + rectParent.top;

	Move(nX, nY);
}

void Window::Navigate(const std::wstring &url) {
	CefRefPtr<CefFrame> frame = GetBrowser()->GetMainFrame();
	frame->LoadURL(url);
}

void Window::SetForeground() {
	::SetForegroundWindow(_hwnd);
}

void Window::SetCaptionArea(const std::vector<RECT> & captions) {
	_captionAreas = captions;
}

ATOM Window::RegisterWndClass()
{
    static ATOM classAtom = 0;
    if (classAtom == 0) 
    {
        WNDCLASSEX wcex;

        ::ZeroMemory (&wcex, sizeof (wcex));
        wcex.cbSize = sizeof(WNDCLASSEX);

		wcex.style			= CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc   = WndProc;
        wcex.hInstance     = ::hInst;
        wcex.hCursor       = ::LoadCursor(NULL, IDC_ARROW);
        wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
        wcex.lpszClassName = windowClassname;

        classAtom = RegisterClassEx(&wcex);
    }
    return classAtom;
}

void Window::OnClose() {
	if (_isModal) {
		HWND hwndParent = GetParent(_hwnd);
		if (hwndParent != NULL) {
			::EnableWindow(hwndParent, true);
		}
	}
	_client->RemoveRenderHandler(_hwnd);

	DeleteTray();
}

bool Window::IsInCaption(POINT &pt) {
	if (_captionAreas.empty()) {
		return false;
	}

	RECT rect;
	::GetClientRect(_hwnd, &rect);
	//GetWindowRect(_hwnd, &rect);

	// 支持动态设定标题栏
	for (std::size_t i = 0; i < _captionAreas.size(); ++i) {
		RECT caption = _captionAreas[i];
		if (_captionAreas[i].right - _captionAreas[i].left <= 0) {
			caption.right = rect.right - (_captionAreas[i].left - _captionAreas[i].right);
		}

		if (_captionAreas[i].bottom - _captionAreas[i].top <= 0) {
			caption.bottom = rect.bottom - (_captionAreas[i].top - _captionAreas[i].bottom);
		}

		if(::PtInRect(&caption, pt)) {
			return true;
		}
	}

	return false;
}

UINT Window::OnNCHitTest(WPARAM wParam, LPARAM lParam) {
	POINT pt;
	pt.x = GET_X_LPARAM(lParam);
    pt.y = GET_Y_LPARAM(lParam);

	ScreenToClient(_hwnd, &pt);

	if (IsInCaption(pt)) {
		 return HTCAPTION;
	}
	
	if (_isResizeable && !_maximized) {
		RECT clientRect;
		::GetClientRect(_hwnd,&clientRect);

		RECT edgeRect;

		// 右下边框
		edgeRect.left = clientRect.right - HITTEST_EDGE_GAP;
		edgeRect.top = clientRect.bottom - HITTEST_EDGE_GAP;
		edgeRect.right = clientRect.right;
		edgeRect.bottom = clientRect.bottom;
		if (::PtInRect(&edgeRect, pt)) {
			return HTBOTTOMRIGHT;
		}

		// 左下边框
		edgeRect.left = clientRect.left;
		edgeRect.top = clientRect.bottom - HITTEST_EDGE_GAP;
		edgeRect.right = clientRect.left + HITTEST_EDGE_GAP;
		edgeRect.bottom = clientRect.bottom;
		if (::PtInRect(&edgeRect, pt)) {
			return HTBOTTOMLEFT;
		}
	
		// 左上边框
		edgeRect.left = clientRect.left;
		edgeRect.top = clientRect.top;
		edgeRect.right = clientRect.left + HITTEST_EDGE_GAP;
		edgeRect.bottom = clientRect.top + HITTEST_EDGE_GAP;
		if (::PtInRect(&edgeRect, pt)) {
			return HTTOPLEFT;
		}
	
		// 右上边框
		edgeRect.left = clientRect.right - HITTEST_EDGE_GAP;
		edgeRect.top = clientRect.top;
		edgeRect.right = clientRect.right;
		edgeRect.bottom = clientRect.top + HITTEST_EDGE_GAP;
		if (::PtInRect(&edgeRect, pt)) {
			return HTTOPRIGHT;
		}

		// 左边框
		edgeRect.left = clientRect.left;
		edgeRect.top = clientRect.top;
		edgeRect.right = edgeRect.left + HITTEST_EDGE_GAP;
		edgeRect.bottom = clientRect.bottom;

		if (::PtInRect(&edgeRect, pt)) {
			return HTLEFT;
		}

		// 右边框
		edgeRect.left = clientRect.right - HITTEST_EDGE_GAP;
		edgeRect.top = clientRect.top;
		edgeRect.right = clientRect.right;
		edgeRect.bottom = clientRect.bottom;
		if (::PtInRect(&edgeRect, pt)) {
			return HTRIGHT;
		}

		// 下边框
		edgeRect.left = clientRect.left;
		edgeRect.top = clientRect.bottom - HITTEST_EDGE_GAP;
		edgeRect.right = clientRect.right;
		edgeRect.bottom = clientRect.bottom;
		if (::PtInRect(&edgeRect, pt)) {
			return HTBOTTOM;
		}

		// 上边框
		edgeRect.left = clientRect.left;
		edgeRect.top = clientRect.top;
		edgeRect.right = clientRect.right;
		edgeRect.bottom = clientRect.top + HITTEST_EDGE_GAP;
		if (::PtInRect(&edgeRect, pt)) {
			return HTTOP;
		}	
	}
	
	return DefWindowProc(_hwnd, WM_NCHITTEST, wParam, lParam);
}

LRESULT Window::OnGetMinMaxInfo(LPARAM lParam) {
	MINMAXINFO *mminfo = (PMINMAXINFO)lParam;  
   
	// 取屏幕大小（除去任务栏）
	RECT rect;
    BOOL fResult = SystemParametersInfo(SPI_GETWORKAREA,
        0, &rect, 0);

    mminfo->ptMaxSize.x = rect.right - rect.left;  
    mminfo->ptMaxSize.y = rect.bottom - rect.top;  

    mminfo->ptMaxPosition.x=0;  
    mminfo->ptMaxPosition.y=0;

	mminfo->ptMinTrackSize.x = _minWidth; 
    mminfo->ptMinTrackSize.y = _minHeight; 

	return 0;
}

void Window::SetTray(const std::wstring & title, const std::wstring & imageName, 
	std::vector<MenuFeature> &menuFeatures) {
	_menuFeatures = menuFeatures;
	static int id = 1;

	NOTIFYICONDATA nid = {};
	nid.cbSize = sizeof(nid);
	nid.hWnd = _hwnd;
	nid.uFlags = NIF_ICON | NIF_TIP  | NIF_MESSAGE;
	nid.uCallbackMessage = WM_SYS_TRAY;

	std::wstring modulePath;
	FileUtil::ProcessPath(modulePath);
	std::wstring imagePath = modulePath + L"/resources/icons/" + imageName.c_str();

	nid.hIcon = (HICON)LoadImage(hInst, imagePath.c_str(), IMAGE_ICON, 0, 0, LR_LOADFROMFILE); 
	nid.uID = _trayId = id++;
	wcsncpy_s(nid.szTip, sizeof(nid.szTip)/sizeof(WCHAR), title.c_str(), title.size());

	 Shell_NotifyIcon(NIM_ADD,&nid);

	 if(nid.hIcon && DestroyIcon(nid.hIcon)) {
		 nid.hIcon = NULL;
	 }
}

void Window::ShowContextMenu()
{
	POINT pt;
	GetCursorPos(&pt);
/*
	std::stringstream ss;
	ss << pt.x << ", " << pt.y << "\n";

	::OutputDebugStringA(ss.str().c_str());
	*/
	HMENU menu = CreatePopupMenu();
	if(menu)
	{
		for (std::size_t i = 0; i < _menuFeatures.size(); ++i) {
			MENUITEMINFO mif;  
			mif.cbSize = sizeof(MENUITEMINFO);  
			mif.dwItemData  = NULL;  
			mif.dwTypeData = (LPWSTR)_menuFeatures[i].label.c_str();  
			mif.cch = _menuFeatures[i].label.size();
			mif.fMask = MIIM_ID;// | MIIM_STRING;  
			mif.fType = MIIM_STRING;  
			mif.wID = _menuFeatures[i].id;  

			if (!_menuFeatures[i].label.empty()) {
				mif.fMask |= MIIM_STRING;

				if (!_menuFeatures[i].icon.empty()) {
					mif.fMask |= MIIM_BITMAP;
					std::wstring modulePath;
					FileUtil::ProcessPath(modulePath);
					std::wstring imagePath = modulePath + L"/resources/icons/" + _menuFeatures[i].icon.c_str();
					HBITMAP bitmap = (HBITMAP) ::LoadImage(hInst, imagePath.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
					mif.hbmpItem = bitmap;
				}
			}
			else {
				mif.fMask |= MIIM_TYPE;
				mif.fType = MF_SEPARATOR;
			}
			
			InsertMenuItem (menu, i, TRUE, &mif);
		}
		
		//SendMessage(_hwnd, WM_NULL, 0, 0);

		SetForegroundWindow(_hwnd);
		TrackPopupMenu(menu, TPM_BOTTOMALIGN, pt.x, pt.y, 0, _hwnd, NULL );
		::PostMessage(_hwnd, WM_NULL, 0, 0);
		//DestroyMenu(menu);
	}
}

void Window::DeleteTray() {
	if (_trayId > 0) {
		NOTIFYICONDATA nid; 
		nid.cbSize = sizeof(NOTIFYICONDATA);
		nid.hWnd = _hwnd;
		nid.uID = _trayId;
		Shell_NotifyIcon(NIM_DELETE, &nid);

		_trayId = -1;
	}
}

 LRESULT Window::OnSysTray( LPARAM lParam)  {
	if (lParam == WM_RBUTTONUP || lParam == WM_CONTEXTMENU) {
		ShowContextMenu();
	}
	else if (lParam == WM_LBUTTONUP) {
		Restore();
	}
	return 0;
}

LRESULT Window::OnCommand(WPARAM wParam) {
	int  menuId  = LOWORD(wParam);
	std::wstringstream jsScript;

	jsScript <<  L"var e = new CustomEvent('" << "MenuClicked" 
		<< "',{detail:" << "{id: " << menuId << "},});document.dispatchEvent(e);";

	this->GetBrowser()->GetMainFrame()->ExecuteJavaScript(jsScript.str(), "", 0);
	return 0;
}

void Window:: OnSize(WPARAM wParam) {
	if (SIZE_RESTORED == wParam && _maximized) {
		_maximized = false;
	}
}

LRESULT Window::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
		case WM_NCHITTEST:
			return OnNCHitTest(wParam, lParam);
			break;
		case WM_GETMINMAXINFO:
			return OnGetMinMaxInfo(lParam);
		case WM_SIZE:
			OnSize(wParam);
			break;
		case WM_CLOSE:
			OnClose();
			break;
		case WM_COMMAND: 
			return OnCommand(wParam);
		case WM_SYS_TRAY:
			return OnSysTray(lParam);
		default:
			break;
	}

	return CefOSRWindow::WindowProc(message, wParam, lParam);
}

}
