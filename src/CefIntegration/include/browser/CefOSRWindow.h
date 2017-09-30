
#ifndef CEF_OSR_WINDOW_H_
#define CEF_OSR_WINDOW_H_

#include "CefWindow.h"
#include "browser/drag_drop/osr_dragdrop_events.h"
#include "browser/drag_drop/osr_dragdrop_win.h"
#include "include/cef_render_handler.h"
#include <atlcomcli.h>
#include <vector>

namespace cefIntegration {

class CEF_CLIENT_API CefOSRWindow : public CefWindow,
					 public CefRenderHandler,
					 public OsrDragEvents {
public:		 
	CefOSRWindow(const std::wstring& name, CefClient *client);
	virtual ~CefOSRWindow();

	virtual void EnableDrag(bool enable);

	virtual void SetToolTip(const std::wstring &text);

	virtual void GetDroppedFiles(std::vector<CefString> & files);

	virtual void SetCaretPosition(int x, int y, int height);

	virtual LRESULT WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

protected:
	virtual void createWindow(HWND &hwnd, bool isLayered, const std::wstring &title, HWND hwndParent);

	virtual void CreateBrowser(const std::wstring &path);

private:
	// CefRenderHandler method
	virtual bool GetRootScreenRect(CefRefPtr<CefBrowser> browser, CefRect& rect);
	virtual bool GetViewRect(CefRefPtr<CefBrowser> browser, CefRect& rect);
	virtual bool GetScreenPoint(CefRefPtr<CefBrowser> browser, int viewX,
    	int viewY, int& screenX, int& screenY) ;
	virtual void OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type,
    	const RectList& dirtyRects, const void* buffer, int width, int height);
	virtual void OnCursorChange(CefRefPtr<CefBrowser> browser,
		CefCursorHandle cursor, CursorType type, const CefCursorInfo& custom_cursor_info);
	virtual bool GetScreenInfo(CefRefPtr<CefBrowser> browser, CefScreenInfo& screen_info);
	virtual bool CefOSRWindow::StartDragging(CefRefPtr<CefBrowser> browser, CefRefPtr<CefDragData> drag_data,
		DragOperationsMask allowed_ops, int x, int y);
	virtual void UpdateDragCursor(CefRefPtr<CefBrowser> browser, DragOperation operation);

	// OsrDragEvents method
	virtual CefBrowserHost::DragOperationsMask OnDragEnter(CefRefPtr<CefDragData> drag_data,
		CefMouseEvent ev, CefBrowserHost::DragOperationsMask effect);
	virtual CefBrowserHost::DragOperationsMask OnDragOver(CefMouseEvent ev, CefBrowserHost::DragOperationsMask effect);
	virtual void OnDragLeave();
	virtual CefBrowserHost::DragOperationsMask OnDrop(CefMouseEvent ev, CefBrowserHost::DragOperationsMask effect);

	// custom method

	// WM_CREATE
	bool OnClose();

	// WM_SIZE
	bool OnSize(WPARAM wParam);

	// WM_LBUTTONDOWN
	bool OnLButtonDown(int x, int y, WPARAM wParam);

	// WM_RBUTTONDOWN
	bool OnRButtonDown(int x, int y, WPARAM wParam);

	// WM_MBUTTONDOWN
	bool OnMButtonDown(int x, int y, WPARAM wParam);

	// WM_LBUTTONUP
	bool OnLButtonUp(int x, int y, WPARAM wParam);

	// WM_RBUTTONUP
	bool OnRButtonUp(int x, int y, WPARAM wParam);

	// WM_MBUTTONUP
	bool OnMButtonUp(int x, int y, WPARAM wParam);

	// WM_MOUSEMOVE
	bool OnMouseMove(int x, int y, WPARAM wParam);

	// WM_MOUSELEAVE
	bool OnMouseLeave(int x, int y, WPARAM wParam);
	
	// WM_MOUSEWHEEL
	bool OnMouseWheel(int x, int y, WPARAM wParam);

	// WM_SETFOCUS, WM_KILLFOCUS
	bool OnFocus(UINT uMsg);

	// WM_CAPTURECHANGED, WM_CANCELMODE
	bool OnCaptureLost();

	//WM_SYSKEYDOWN, WM_SYSKEYUP, WM_KEYDOWN, WM_KEYUP, WM_CHAR
	bool OnKeyboard(UINT uMsg, WPARAM wParam, LPARAM lParam);
	
	// WM_PAINT
	bool OnPaint();

	// WM_MOUSEHOVER
	bool OnMouseHover();

	void ShowToolTip();

	void HideToolTip();

private:
	void OnBrowserMouseClick(int x, int y, WPARAM wParam, cef_mouse_button_type_t button, bool mouseUp);
	void OnBrowserMouseMove(int x, int y, WPARAM wParam, bool isLeave);
	void PreProcessMouseMessage(UINT uMsg, int x, int y);
	HBITMAP CreateDIB(const void* buffer, int width, int height);
	void OnPaint(HBITMAP bitMap, int width, int height);
	bool OnIMEMessage(WPARAM wParam, LPARAM lParam);
	void SetIMEPosition(HWND hWnd, int x, int y, int height);

protected:
	bool _enableDrag;

private:

	// cef提供渲染的图像
	HBITMAP _bitMap;

	// mouse event variables

	// 是否正在跟踪鼠标移动
	bool _mouseTracking;	

	// 上一次鼠标点击的x坐标	
	int _lastClickX;

	// 上一次鼠标点击的y坐标	
	int _lastClickY;

	// 上一次鼠标点击的消息
 	int _lastClickMsg;

 	//上次连续点击鼠标的次数
	int _lastClickCount;

	// 上次鼠标点击的时间
 	double _lastClickTime;

	CComPtr<cefIntegration::DropTargetWin> _droptTarget;
	CefRenderHandler::DragOperation _currentDragOp;

	std::vector<CefString> _droppedFiles;

	// tooltip
	HWND _tooltipHwnd;
	bool _showTip;
	std::wstring _curTipText;
	std::wstring _tipText;

	POINT _caretPt;
	int _fontHeight;

	IMPLEMENT_REFCOUNTING(CefOSRWindow);
};

}

#endif