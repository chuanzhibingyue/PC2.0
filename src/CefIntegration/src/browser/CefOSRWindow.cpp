#include "browser/CefOSRWindow.h"
#include "browser/OsrUtil.h"
#include "utils/FileUtil.h"
#include "browser/drag_drop/geometry_util.h"
#include "CefClient.h"

#include <Windowsx.h>
#include <Commctrl.h>
#include "LogManager.h"

namespace cefIntegration {
	using namespace uc;
CefOSRWindow::CefOSRWindow(const std::wstring& name, CefClient *client) : CefWindow(name, client),
		_bitMap(NULL), _mouseTracking(false), _lastClickX(0), _lastClickY(0), _lastClickMsg(-1),
		_lastClickCount(0), _droptTarget(NULL), _tooltipHwnd(NULL), _showTip(false), _fontHeight(-1){
	_caretPt.x = _caretPt.y = 0;
}

CefOSRWindow::~CefOSRWindow() {
	if(_bitMap) {
		DeleteObject(_bitMap);
        _bitMap = NULL;
	}
}

void CefOSRWindow::EnableDrag(bool enable) {
	CefWindow::EnableDrag(enable);

	if ((_droptTarget == NULL) && enable) {
		_droptTarget = DropTargetWin::Create(this, GetWnd());
		HRESULT res = RegisterDragDrop(GetWnd(), _droptTarget);
	}
}

bool CefOSRWindow::OnClose() {
	if (_droptTarget != NULL){
		RevokeDragDrop(GetWnd());
		_droptTarget = NULL;
	}

	_client->getHandler()->RemoveRenderHandler(_hwnd);

	return false;
}

bool CefOSRWindow::OnSize(WPARAM wParam) {
	if (SIZE_MINIMIZED == wParam)
		return false;

	if (SIZE_RESTORED == wParam && _winStatus == CWS_MINIMIZED)
		return false;

	CefRefPtr<CefBrowser> browser = GetBrowser();

    if (browser != NULL && browser.get()) {
    	browser->GetHost()->WasResized();
    }
	
	return false;
}

bool CefOSRWindow::OnLButtonDown(int x, int y, WPARAM wParam) {
	OnBrowserMouseClick(x, y, wParam, MBT_LEFT, false);
	return false;
}

bool CefOSRWindow::OnRButtonDown(int x, int y, WPARAM wParam) {
	OnBrowserMouseClick(x, y, wParam, MBT_RIGHT, false);
	return false;
}

bool CefOSRWindow::OnMButtonDown(int x, int y, WPARAM wParam) {
	OnBrowserMouseClick(x, y, wParam, MBT_MIDDLE, false);
	return false;
}

bool CefOSRWindow::OnLButtonUp(int x, int y, WPARAM wParam) {
	OnBrowserMouseClick(x, y, wParam, MBT_LEFT, true);
	return false;
}

bool CefOSRWindow::OnRButtonUp(int x, int y, WPARAM wParam) {
	OnBrowserMouseClick(x, y, wParam, MBT_RIGHT, true);
	return false;
}

bool CefOSRWindow::OnMButtonUp(int x, int y, WPARAM wParam) {
	OnBrowserMouseClick(x, y, wParam, MBT_MIDDLE, true);
	return false;
}

bool CefOSRWindow::OnMouseMove(int x, int y, WPARAM wParam) {
	if (!_mouseTracking) {
        // tracking mouse leave.
        TRACKMOUSEEVENT tme;
        tme.cbSize = sizeof(TRACKMOUSEEVENT);
        tme.dwFlags = TME_LEAVE;
        tme.hwndTrack = GetWnd();
        TrackMouseEvent(&tme);
        _mouseTracking = true;
    }
	OnBrowserMouseMove(x, y, wParam, false);
	return false;
}

bool CefOSRWindow::OnMouseLeave(int x, int y, WPARAM wParam) {
	if (_mouseTracking) 
    {
        // Stop tracking mouse leave.
        TRACKMOUSEEVENT tme;
        tme.cbSize = sizeof(TRACKMOUSEEVENT);
        tme.dwFlags = TME_LEAVE & TME_CANCEL;
        tme.hwndTrack = GetWnd();
        TrackMouseEvent(&tme);
        _mouseTracking = false;
    }

    OnBrowserMouseMove(0, 0, wParam, true);
	return false;
}

bool CefOSRWindow::OnMouseWheel(int x, int y, WPARAM wParam)
{
    CefRefPtr<CefBrowser> browser = GetBrowser();
    if (browser != NULL && browser.get()) {
        POINT screen_point = {x, y};
        HWND scrolled_wnd = ::WindowFromPoint(screen_point);
        HWND hwnd = GetWnd();
        if (scrolled_wnd != hwnd) {
            return false;
        }

        ::ScreenToClient(hwnd, &screen_point);
        int delta = GET_WHEEL_DELTA_WPARAM(wParam);

        CefMouseEvent mouse_event;
        mouse_event.x = screen_point.x;
        mouse_event.y = screen_point.y;
		DeviceToLogical(mouse_event, _deviceScaleFactor);
		mouse_event.modifiers = OsrUtil::GetCefMouseModifiers(wParam);

		browser->GetHost()->SendMouseWheelEvent(mouse_event, OsrUtil::IsKeyDown(VK_SHIFT) ? delta : 0,
			!OsrUtil::IsKeyDown(VK_SHIFT) ? delta : 0);
    }
	return false;
}

bool CefOSRWindow::OnFocus(UINT uMsg)
{
    CefRefPtr<CefBrowser> browser = GetBrowser();
    if (browser != NULL && browser.get()) {
        browser->GetHost()->SendFocusEvent(uMsg == WM_SETFOCUS);
    }
	return false;
}

bool CefOSRWindow::OnCaptureLost()
{
    CefRefPtr<CefBrowser> browser = GetBrowser();
    if (browser != NULL && browser.get()) {
        browser->GetHost()->SendCaptureLostEvent();
    } 
	return false;
}

bool CefOSRWindow::OnKeyboard(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    CefKeyEvent keyEvent;
    keyEvent.windows_key_code = wParam;
    keyEvent.native_key_code = lParam;
    keyEvent.is_system_key = (uMsg == WM_SYSCHAR || 
        uMsg == WM_SYSKEYDOWN || uMsg == WM_SYSKEYUP);

    if (uMsg == WM_KEYDOWN || uMsg == WM_SYSKEYDOWN)
    {
        keyEvent.type = KEYEVENT_RAWKEYDOWN;
    } 
    else if (uMsg == WM_KEYUP || uMsg == WM_SYSKEYUP)
    {
        keyEvent.type = KEYEVENT_KEYUP;
    }
    else
    {
        keyEvent.type = KEYEVENT_CHAR;
    }
	keyEvent.modifiers = OsrUtil::GetCefKeyboardModifiers(wParam, lParam);

    CefRefPtr<CefBrowser> browser = GetBrowser();
    if (browser != NULL && browser.get()) {
        browser->GetHost()->SendKeyEvent(keyEvent);
    }
	return false;
}

void CefOSRWindow::OnBrowserMouseMove(int x, int y, WPARAM wParam, bool isLeave)
{
    CefRefPtr<CefBrowser> browser = GetBrowser();

    if (browser != NULL && browser.get()) {
        CefMouseEvent mouse_event;
        mouse_event.x = x;
        mouse_event.y = y;
		DeviceToLogical(mouse_event, _deviceScaleFactor);
		mouse_event.modifiers = OsrUtil::GetCefMouseModifiers(wParam);
        browser->GetHost()->SendMouseMoveEvent(mouse_event, isLeave);
    }
}

void CefOSRWindow::OnBrowserMouseClick(int x, int y, WPARAM wParam, cef_mouse_button_type_t button,
	bool mouseUp) {
	if (!mouseUp) {
		HWND hwnd = GetWnd();
		::SetCapture(hwnd);
		::SetFocus(hwnd);

		// 解决无焦点问题
		::PostMessage(GetWnd(), WM_SETFOCUS, NULL,NULL);
	} else if (GetCapture() == GetWnd()) {
		::ReleaseCapture();
	}

    CefRefPtr<CefBrowser> browser = GetBrowser();

    if (browser != NULL && browser.get()) {
    	CefMouseEvent mouse_event;
    	mouse_event.x = x;
    	mouse_event.y = y;

		DeviceToLogical(mouse_event, _deviceScaleFactor);

		mouse_event.modifiers = OsrUtil::GetCefMouseModifiers(wParam);
    	browser->GetHost()->SendMouseClickEvent(mouse_event, button, mouseUp, 
        	_lastClickCount);
    }  
}

bool CefOSRWindow::GetRootScreenRect(CefRefPtr<CefBrowser> browser, CefRect& rect) {
    RECT root_rect;
	HWND hwnd = GetWnd();
    HWND root_window = GetAncestor(hwnd, GA_ROOT);
    if ( ::IsWindow(hwnd) && ::GetWindowRect(root_window, &root_rect)) {
        rect.x = root_rect.left;
        rect.y = root_rect.top;
        rect.width = root_rect.right - root_rect.left;
        rect.height = root_rect.bottom - root_rect.top;

        return true;
    }
    return false;
}

bool CefOSRWindow::GetViewRect(CefRefPtr<CefBrowser> browser, CefRect& rect) {
    RECT clientRect;
    HWND hwnd = GetWnd();
    if ((!::IsWindow(hwnd)) || (!::GetWindowRect(hwnd, &clientRect))) {
        return false;
    }
    
    rect.x = rect.y = 0;
	rect.width = DeviceToLogical(clientRect.right - clientRect.left,
		_deviceScaleFactor);
	rect.height = DeviceToLogical(clientRect.bottom - clientRect.top,
		_deviceScaleFactor);

    return true;
}

bool CefOSRWindow::GetScreenPoint(CefRefPtr<CefBrowser> browser, int viewX,
    int viewY, int& screenX, int& screenY) {
	HWND hwnd = GetWnd();
    if (!::IsWindow(hwnd)) {
        return false;
    }

	POINT screen_pt = { LogicalToDevice(viewX, _deviceScaleFactor),
		LogicalToDevice(viewY, _deviceScaleFactor) };
    ::ClientToScreen(hwnd, &screen_pt);
    screenX = screen_pt.x;
    screenY = screen_pt.y;
    return true;
}

HBITMAP CefOSRWindow::CreateDIB(const void* buffer, int width, int height) {
	BITMAPINFOHEADER info;
	info.biSize = sizeof(BITMAPINFOHEADER);
	info.biWidth = width;
	info.biHeight = -height;
	info.biPlanes = 1;
	info.biBitCount = 32;
	info.biCompression = BI_RGB;
	info.biSizeImage = 0;
	info.biXPelsPerMeter = 0;
	info.biYPelsPerMeter = 0;
	info.biClrUsed = 0;
	info.biClrImportant = 0;

	void* bits = NULL;
	HBITMAP bitmap = CreateDIBSection(NULL, reinterpret_cast<BITMAPINFO*>(&info), DIB_RGB_COLORS, &bits, NULL, 0);

	if (bitmap != NULL) {
		memcpy(bits, buffer, width * height * 4);
	}
	return bitmap;

}
void CefOSRWindow::OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList& dirtyRects, 
	const void* buffer, int width, int height) {
	// create device independent bitmap, to support displayers of 15/16/24/32 color bit depth
	HBITMAP bitMap = CreateDIB(buffer, width, height);
	if (bitMap != NULL) {
		OnPaint(bitMap, width, height);

		::DeleteObject(bitMap);
	}
}

void CefOSRWindow::OnPaint(HBITMAP bitMap, int width, int height) {
	HWND hwnd = GetWnd();
	HDC renderDC = ::GetDC(hwnd);
	HDC memoryDC = CreateCompatibleDC(renderDC);

	RECT winRect;
	::GetWindowRect(hwnd, &winRect);

	SIZE sz = { width , height };
	POINT pptSrc = { 0, 0 };
	POINT pptDst = { winRect.left, winRect.top };

	HBITMAP old = (HBITMAP)SelectObject(memoryDC, bitMap);

	int exStyle = GetWindowLong(hwnd, GWL_EXSTYLE);
	if (exStyle & WS_EX_LAYERED)
	{
		SIZE sz = { width, height };
		POINT pptSrc = { 0, 0 };
		POINT pptDst = { winRect.left, winRect.top };

		BLENDFUNCTION blendFunc;
		blendFunc.BlendOp = AC_SRC_OVER;
		blendFunc.AlphaFormat = AC_SRC_ALPHA;
		blendFunc.BlendFlags = 0;
		blendFunc.SourceConstantAlpha = 255;
		UpdateLayeredWindow(hwnd, renderDC, &pptDst, &sz, memoryDC, &pptSrc, RGB(0, 0, 0), &blendFunc, ULW_ALPHA);
	}
	else
	{
		::BitBlt(renderDC, 0, 0, winRect.right - winRect.left, winRect.bottom - winRect.top, memoryDC, 0, 0, SRCCOPY);
	}

	::SelectObject(memoryDC, old);
	::DeleteDC(memoryDC);
	::ReleaseDC(hwnd, renderDC);
}

void CefOSRWindow::SetIMEPosition(HWND hWnd, int x, int y, int height) {
	HIMC hIMC = ImmGetContext(hWnd);
	if (ImmGetOpenStatus(hIMC)) {
		COMPOSITIONFORM cf = { 0 };
		cf.dwStyle = CFS_POINT;
		cf.ptCurrentPos.x = x;
		cf.ptCurrentPos.y = y;

		BOOL ret = ImmSetCompositionWindow(hIMC, &cf);
		if (ret) {
			if (height > 0) {
				LOGFONT lf = { 0 };
				lf.lfHeight = height;
				ImmSetCompositionFont(hIMC, &lf);
			}
		}
		else {
			UC_LOG(ERROR) << "ImmSetCompositionWindow error, result: " << ::GetLastError();
		}
	}
	ImmReleaseContext(hWnd, hIMC);
}

bool CefOSRWindow::OnIMEMessage(WPARAM wParam, LPARAM lParam) {
	SetIMEPosition(this->GetWnd(), _caretPt.x, _caretPt.y, _fontHeight);
	return false;
}

void CefOSRWindow::PreProcessMouseMessage(UINT uMsg, int x, int y)
{
    LONG currentTime = GetMessageTime();

    bool cancelPreviousClick =
        (abs(_lastClickX - x) > (GetSystemMetrics(SM_CXDOUBLECLK) / 2))
        || (abs(_lastClickY - y) > (GetSystemMetrics(SM_CYDOUBLECLK) / 2))
        || ((currentTime - _lastClickTime) > GetDoubleClickTime());

    if (cancelPreviousClick &&
        (uMsg == WM_MOUSEMOVE || uMsg == WM_MOUSELEAVE)) {
        _lastClickCount = _lastClickX = _lastClickY = _lastClickTime = 0;
    }

    if (uMsg == WM_LBUTTONDOWN || uMsg == WM_RBUTTONDOWN || 
        uMsg == WM_MBUTTONDOWN) {
        _lastClickTime = currentTime;

        if (!cancelPreviousClick && (uMsg == _lastClickMsg)) 
        {
            ++_lastClickCount;
        } 
        else 
        {
            _lastClickCount = 1;
            _lastClickX = x;
            _lastClickY = y;
        }

        _lastClickMsg = uMsg;
    }
}

bool CefOSRWindow::GetScreenInfo(CefRefPtr<CefBrowser> browser, CefScreenInfo& screen_info)
{
	int screenWidth = ::GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = ::GetSystemMetrics(SM_CYSCREEN);
	CefRect rect(0, 0, DeviceToLogical(screenWidth, _deviceScaleFactor), 
		DeviceToLogical(screenHeight, _deviceScaleFactor));
	screen_info.rect = rect;

	RECT workArea;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &workArea, 0);
	CefRect aRect(0, 0, DeviceToLogical(workArea.right - workArea.left, _deviceScaleFactor),
		DeviceToLogical(workArea.bottom - workArea.top, _deviceScaleFactor));
	screen_info.available_rect = aRect;
	screen_info.device_scale_factor = _deviceScaleFactor;
	
	return true;
}

bool CefOSRWindow::StartDragging(CefRefPtr<CefBrowser> browser, CefRefPtr<CefDragData> drag_data,
	DragOperationsMask allowed_ops, int x, int y) {
	if (!_droptTarget)
		return false;

	_currentDragOp = DRAG_OPERATION_NONE;
	CefBrowserHost::DragOperationsMask result =
		_droptTarget->StartDragging(browser, drag_data, allowed_ops, x, y);
	_currentDragOp = DRAG_OPERATION_NONE;
	POINT pt = {};
	GetCursorPos(&pt);
	ScreenToClient(GetWnd(), &pt);

	browser->GetHost()->DragSourceEndedAt(
		DeviceToLogical(pt.x, _deviceScaleFactor),
		DeviceToLogical(pt.y, _deviceScaleFactor),
		result);
	browser->GetHost()->DragSourceSystemDragEnded();
	return true;
}

void CefOSRWindow::UpdateDragCursor(CefRefPtr<CefBrowser> browser, 
	CefRenderHandler::DragOperation operation) {
	_currentDragOp = operation;
}

void CefOSRWindow::OnCursorChange(CefRefPtr<CefBrowser> browser,
	CefCursorHandle cursor, CursorType type, const CefCursorInfo& custom_cursor_info) {
	HWND hwnd = GetWnd();
	if (!::IsWindow(hwnd))
		return;

	SetClassLongPtr(hwnd, GCLP_HCURSOR,
		static_cast<LONG>(reinterpret_cast<LONG_PTR>(cursor)));
	SetCursor(cursor);
}

bool CefOSRWindow::OnPaint() {
	PAINTSTRUCT ps;
	BeginPaint(this->GetWnd(), &ps);
	EndPaint(this->GetWnd(), &ps);

	CefRefPtr<CefBrowser> browser = GetBrowser();
	if (browser.get())
		browser->GetHost()->Invalidate(PET_VIEW);
	return true;
}

bool CefOSRWindow::OnMouseHover() {
	ShowToolTip();

	return false;
}

void CefOSRWindow::SetToolTip(const std::wstring & text) {
	_tipText = text;
	if (_showTip && _tipText.empty()) {
		HideToolTip();
	}
}

void CefOSRWindow::ShowToolTip() {
	if (_showTip && _curTipText.compare(_tipText) == 0) {
		return;
	}
	else if (_curTipText.compare(_tipText) != 0) {
		HideToolTip();
	}

	if (_tooltipHwnd == NULL) {
		_tooltipHwnd = CreateWindowEx(NULL, TOOLTIPS_CLASS, NULL, WS_POPUP | TTS_ALWAYSTIP,
			CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, _hwnd, NULL, hModuleInst, NULL);
	}

	TOOLINFO toolInfo = { 0 };
	toolInfo.cbSize = sizeof(toolInfo);
	toolInfo.hwnd = _hwnd;
	toolInfo.uFlags = TTF_IDISHWND | TTF_SUBCLASS;
	toolInfo.uId = (UINT_PTR)_tooltipHwnd;

	toolInfo.lpszText = const_cast<wchar_t *>(_tipText.c_str());

	_curTipText = _tipText;
	SendMessage(_tooltipHwnd, TTM_ADDTOOL, 0, (LPARAM)&toolInfo);
	SendMessage(_tooltipHwnd, TTM_TRACKACTIVATE, TRUE, (LPARAM)&toolInfo);

	_showTip = true;
}

void CefOSRWindow::HideToolTip() {
	if (_tooltipHwnd != NULL && _showTip) {
		TOOLINFO toolInfo = { 0 };
		toolInfo.cbSize = sizeof(toolInfo);
		toolInfo.hwnd = _hwnd;
		toolInfo.uFlags = TTF_IDISHWND | TTF_SUBCLASS;
		toolInfo.uId = (UINT_PTR)_tooltipHwnd;

		SendMessage(_tooltipHwnd, TTM_DELTOOL, 0, (LPARAM)&toolInfo);
		SendMessage(_tooltipHwnd, TTM_TRACKACTIVATE, FALSE, (LPARAM)&toolInfo);
	}

	_showTip = false;
}

LRESULT CefOSRWindow::WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	int x = GET_X_LPARAM(lParam);
	int y = GET_Y_LPARAM(lParam);
	PreProcessMouseMessage(message, x, y);
	
	switch (message) {
		case WM_CLOSE:
			PROCESS_MESSAGE(OnClose());
		case WM_MOUSEHOVER:
			PROCESS_MESSAGE(OnMouseHover());
		case WM_SIZE:
			PROCESS_MESSAGE(OnSize(wParam));
		case WM_LBUTTONDOWN:
			PROCESS_MESSAGE(OnLButtonDown(x, y, wParam));
		case WM_RBUTTONDOWN:
			PROCESS_MESSAGE(OnRButtonDown(x, y, wParam));
		case WM_MBUTTONDOWN:
			PROCESS_MESSAGE(OnMButtonDown(x, y, wParam));
		case WM_LBUTTONUP:
			PROCESS_MESSAGE(OnLButtonUp(x, y, wParam));
		case WM_RBUTTONUP:
			PROCESS_MESSAGE(OnRButtonUp(x, y, wParam));
		case WM_MBUTTONUP:
			PROCESS_MESSAGE(OnMButtonUp(x, y, wParam));
		case WM_MOUSEMOVE:
			PROCESS_MESSAGE(OnMouseMove(x, y, wParam));
		case WM_MOUSELEAVE:
			PROCESS_MESSAGE(OnMouseLeave(x, y, wParam));
		case WM_MOUSEWHEEL:
			PROCESS_MESSAGE(OnMouseWheel(x, y, wParam));
		case WM_SETFOCUS:
		case WM_KILLFOCUS:
			PROCESS_MESSAGE(OnFocus(message));
		case WM_CAPTURECHANGED:
		case WM_CANCELMODE:
			PROCESS_MESSAGE(OnCaptureLost());
		case WM_SYSKEYDOWN:
		case WM_SYSKEYUP:
		case WM_KEYDOWN:
		case WM_KEYUP:
		case WM_CHAR:
			PROCESS_MESSAGE(OnKeyboard(message, wParam, lParam));
		case WM_PAINT: 
			PROCESS_MESSAGE(OnPaint());
		case WM_IME_COMPOSITION:
		case WM_IME_STARTCOMPOSITION:
			PROCESS_MESSAGE(OnIMEMessage(wParam, lParam));
		default:
			break;	
	}

	return CefWindow::WindowProc(hWnd, message, wParam, lParam);
}

void CefOSRWindow::createWindow(HWND &hwnd, bool isLayered, const std::wstring &title, HWND hwndParent) {

	if (isLayered){
		hwnd = ::CreateWindowEx(0, UCCLIENT_CLASS_NAME, title.c_str(),
			WS_POPUP | WS_MINIMIZEBOX|WS_CLIPCHILDREN &~(WS_CAPTION | WS_BORDER), CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
			CW_USEDEFAULT, hwndParent, NULL, hModuleInst, (LPVOID)this);
	}else{
		hwnd = ::CreateWindowEx(WS_EX_LAYERED, UCCLIENT_CLASS_NAME, title.c_str(),
			WS_POPUP | WS_MINIMIZEBOX &~(WS_CAPTION | WS_BORDER), CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
			CW_USEDEFAULT, hwndParent, NULL, hModuleInst, (LPVOID)this);
	}
}

void CefOSRWindow::CreateBrowser(const std::wstring &path) {
	_client->getHandler()->SetRenderHandler(_hwnd, this);
	_client->CreateBrowser(_hwnd, path, true);
}

void CefOSRWindow::GetDroppedFiles(std::vector<CefString> & files) {
	files = _droppedFiles;
	_droppedFiles.clear();
}

void CefOSRWindow::SetCaretPosition(int x, int y, int height) {
	_caretPt.x = LogicalToDevice(x, _deviceScaleFactor);
	_caretPt.y = LogicalToDevice(y, _deviceScaleFactor);
	_fontHeight = height;
}

CefBrowserHost::DragOperationsMask CefOSRWindow::OnDragEnter(CefRefPtr<CefDragData> drag_data,
	CefMouseEvent ev, CefBrowserHost::DragOperationsMask effect) {
	CefRefPtr<CefBrowser> browser = GetBrowser();
	if (browser && browser.get()) {
		if (drag_data->IsFile()) {
			_droppedFiles.clear();
			drag_data->GetFileNames(_droppedFiles);
			
			// remove filter of directory, for goukuai support upload folder
			/*
			for (std::vector<CefString>::iterator iter = _droppedFiles.begin(); iter != _droppedFiles.end();) {
				if (FileUtil::IsFolder((*iter).ToWString())) {
					iter = _droppedFiles.erase(iter);
				}
				else {
					++iter;
				} 
			}
			*/
		}
	}
		
	DeviceToLogical(ev, _deviceScaleFactor);
	browser->GetHost()->DragTargetDragEnter(drag_data, ev, effect);
	browser->GetHost()->DragTargetDragOver(ev, effect);
	return _currentDragOp;
}

CefBrowserHost::DragOperationsMask CefOSRWindow::OnDragOver(CefMouseEvent ev, 
	CefBrowserHost::DragOperationsMask effect) {
	CefRefPtr<CefBrowser> browser = GetBrowser();
	if (browser && browser.get()) {
		DeviceToLogical(ev, _deviceScaleFactor);
		browser->GetHost()->DragTargetDragOver(ev, effect);
	}
	return _currentDragOp;
}

void CefOSRWindow::OnDragLeave() {
	CefRefPtr<CefBrowser> browser = GetBrowser();
	if (browser && browser.get()) {
		browser->GetHost()->DragTargetDragLeave();
	}
}

CefBrowserHost::DragOperationsMask CefOSRWindow::OnDrop(CefMouseEvent ev, 
	CefBrowserHost::DragOperationsMask effect) {
	CefRefPtr<CefBrowser> browser = GetBrowser();
	if (browser && browser.get()) {
		DeviceToLogical(ev, _deviceScaleFactor);
		browser->GetHost()->DragTargetDragOver(ev, effect);
		browser->GetHost()->DragTargetDrop(ev);
	}
	return _currentDragOp;
}

}