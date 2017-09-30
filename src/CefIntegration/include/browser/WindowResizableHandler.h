#ifndef WINDOW_RESIZABLE_HANDLER
#define WINDOW_RESIZABLE_HANDLER

#include "Windows.h"
#include "include/cef_browser.h"

class WindowResizableHandler {
public:
	WindowResizableHandler(HWND hwnd);
	~WindowResizableHandler();

	void OnLButtonDown(WPARAM wParam, LPARAM lParam);
	void OnLButtonUp(WPARAM wParam, LPARAM lParam);
	void OnMouseMove(WPARAM wParam, LPARAM lParam);
	void OnCursorChange(CefRefPtr<CefBrowser> browser, CefCursorHandle cursor);

private:
	void ChangeCursorByHitType();
	void EnsureBrowserResizeHandle(int* triggerTimes, int* x, int* y, RECT& rect);
	void CorrectHTMLSize(int width, int height, bool disable = false);
	void LParamToScreenPoint(LPARAM lParam, POINT& pt);
	void GetHTMLRect(RECT& rect);
	void CalculateHitType(int x, int y);
	void ResizeByHitType();
	void RestoreByHitType(POINT& pt);
	void MoveByHitType(POINT& pt);
	void RedirectMouseEvent(int clientX, int clientY, RECT rect, int* redirectedX, int* redirectedY);
	void RedirectMovePosition(int clientX, int clientY, int screenX, int screenY, 
		RECT rect, int* redirectedX, int* redirectedY);
	void ExecJS(const std::string& str);
	bool IsInEdge();
	bool CanResize();

private:
	HWND _hwnd;
	int _hitType;
	RECT _downRect;
	POINT _downPt;
	bool _isButtonDown;

	enum WindowStatus {
		WS_RESIZING_BEGIN, WS_RESIZING, WS_NORMALING, WS_NORMAL, WS_MAXMIZING, WS_MAXIMIZED
	};
	WindowStatus _state;

	CefRefPtr<CefBrowser> _browser;
};

#endif