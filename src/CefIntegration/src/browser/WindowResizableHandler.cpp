#include "browser/WindowResizableHandler.h"
#include <windowsx.h>

#include <sstream>


#define HITTEST_EDGE_GAP	7
#define WINDOW_EDGE_EXPAND	1

WindowResizableHandler::WindowResizableHandler(HWND hwnd) : _hwnd(hwnd), _hitType(HTNOWHERE),
	_state(WS_NORMAL), _isButtonDown(false)
{
}


WindowResizableHandler::~WindowResizableHandler()
{
}

void WindowResizableHandler::ChangeCursorByHitType()
{
	HCURSOR cursor = NULL;

	if (_hitType == HTLEFT || _hitType == HTRIGHT)
	{
		cursor = ::LoadCursor(NULL, IDC_SIZEWE);
	}
	else if (_hitType == HTBOTTOM || _hitType == HTTOP)
	{
		cursor = ::LoadCursor(NULL, IDC_SIZENS);
	}
	else if (_hitType == HTTOPRIGHT || _hitType == HTBOTTOMLEFT)
    {
        cursor = ::LoadCursor(NULL, IDC_SIZENESW);
    }
	else if (_hitType == HTTOPLEFT || _hitType == HTBOTTOMRIGHT)
	{
		 cursor = ::LoadCursor(NULL, IDC_SIZENWSE);  
	}

	if (cursor != NULL)
	{
		::SetCursor(cursor);
	}
}

void WindowResizableHandler::OnLButtonDown(WPARAM wParam, LPARAM lParam) {	
	_isButtonDown = true;

    // store cursor and window information
	int x = GET_X_LPARAM(lParam);
	int y = GET_Y_LPARAM(lParam);
    LParamToScreenPoint(lParam, _downPt);
    ::GetWindowRect(_hwnd, &_downRect);
    
    CalculateHitType(x, y);

	if (CanResize())
    {
		_state = WS_RESIZING_BEGIN;
		
		// the browser doesn't respond the edge event
		if (x == 0)
		{
			x = 1;
		}
		if (y == 0)
		{
			y = 1;
		}

        // when clicking the window edges, expand the size from the edge
        ResizeByHitType();

        // redirect the mouse event, when hitType is HTLEFT, HTTOP, HTTOPRIGHT, HTTOPLEFT, HTBOTTOMLEFT
        // int redirectedX, redirectedY;
        RedirectMouseEvent(x, y, _downRect, &x, &y);
    }
}

void WindowResizableHandler::OnLButtonUp(WPARAM wParam, LPARAM lParam)
{
	_isButtonDown = false;
	
	if (_state == WS_RESIZING || _state == WS_RESIZING_BEGIN)
	{
		int x = GET_X_LPARAM(lParam);
		int y = GET_Y_LPARAM(lParam);
		POINT pt;
		LParamToScreenPoint(lParam, pt);
	
		RedirectMouseEvent(x, y, _downRect, &x, &y);

		RestoreByHitType(pt);

		_state = WS_NORMAL;
	}
    else if (_state == WS_MAXMIZING)
    {
		_state = WS_MAXIMIZED;
	}
	else if(_state == WS_NORMALING)
	{
		_state = WS_NORMAL;
	}
}

void WindowResizableHandler::OnMouseMove(WPARAM wParam, LPARAM lParam)
{
    POINT pt;
    LParamToScreenPoint(lParam, pt);
    if (_state == WS_NORMAL || _state ==WS_MAXIMIZED) {
        CalculateHitType(pt.x, pt.y);
    }

	static int triggerTimes = 0;
	int x = GET_X_LPARAM(lParam), y = GET_Y_LPARAM(lParam);
	if (_state == WS_RESIZING_BEGIN)
	{
		triggerTimes = 0;
		_state = WS_RESIZING;
	}

	if (_state == WS_RESIZING || 
		(_state == WS_NORMAL && !_isButtonDown))
	{
		ChangeCursorByHitType();
	}

	if (_state == WS_RESIZING)
	{
		MoveByHitType(pt);
        RedirectMovePosition(x, y, pt.x, pt.y, _downRect, &x, &y);

		// fix the bug for js resizable: the real resize orientation is not totally determind by the button down point
        
		EnsureBrowserResizeHandle(&triggerTimes, &x, &y, _downRect);
	}
}

void WindowResizableHandler::EnsureBrowserResizeHandle(int* triggerTimes, int* x, int* y, RECT& rect)
{
	if ((*triggerTimes) ++ > 3)
	{
		return;
	}

	if (_hitType == HTBOTTOMRIGHT|| _hitType == HTTOPLEFT || 
		_hitType == HTTOPRIGHT || _hitType == HTBOTTOMLEFT)
	{
		if ( (*y) < rect.bottom - rect.top - HITTEST_EDGE_GAP - WINDOW_EDGE_EXPAND)
		{
			(*y) = rect.bottom - rect.top - HITTEST_EDGE_GAP - WINDOW_EDGE_EXPAND + 1;
		}
		if ((*x) < rect.right - rect.left - HITTEST_EDGE_GAP - WINDOW_EDGE_EXPAND)
		{
			(*x) = rect.right - rect.left -HITTEST_EDGE_GAP - WINDOW_EDGE_EXPAND + 1;
		}
	}
	else if (_hitType == HTRIGHT || _hitType == HTLEFT)
	{
		if ( (*y) <= HITTEST_EDGE_GAP)
		{
			(*y) = HITTEST_EDGE_GAP + 1;
		}
		else if ( (*y) > rect.bottom - rect.top - HITTEST_EDGE_GAP - WINDOW_EDGE_EXPAND - 1)
		{
			(*y) = rect.bottom - rect.top - HITTEST_EDGE_GAP - WINDOW_EDGE_EXPAND - 2;
		}
	}
	else if (_hitType == HTBOTTOM || _hitType == HTTOP)
	{
		if ( (*x) > rect.right - rect.left - HITTEST_EDGE_GAP - WINDOW_EDGE_EXPAND)
		{
			(*x) = rect.right - rect.left - HITTEST_EDGE_GAP - WINDOW_EDGE_EXPAND - 1;
		}
		else if ( (*x) <= HITTEST_EDGE_GAP)
		{
			(*x) = HITTEST_EDGE_GAP + 1;
		}
	}

}

void WindowResizableHandler::CorrectHTMLSize(int width, int height, bool disable)
{
    std::stringstream codeStream;
	RECT rect;
	::GetWindowRect(_hwnd, &rect);
    codeStream <<"var event = document.createEvent('HTMLEvents');"
       <<"event.initEvent('correctsize', true, true);"
	   << "event.width =" << width << ";event.height =" << height
	   <<";event.disableResizable =" << disable
       <<";event.eventType = 'message';document.dispatchEvent(event);";

    const std::string codeString = codeStream.str();
    ExecJS(codeString);
}

void WindowResizableHandler::ExecJS(const std::string& str)
{
    CefRefPtr<CefFrame> frame= _browser->GetMainFrame();
    _browser->GetMainFrame()->ExecuteJavaScript(
          str, "", 0);
}

void WindowResizableHandler::LParamToScreenPoint(LPARAM lParam, POINT& pt)
{
    pt.x = GET_X_LPARAM(lParam);
    pt.y = GET_Y_LPARAM(lParam);

    ::ClientToScreen(_hwnd, &pt);
}


void WindowResizableHandler::GetHTMLRect(RECT& rect)
{
    ::GetWindowRect(_hwnd, &rect);
	if (_state != WS_MAXIMIZED)
	{
		rect.bottom -= WINDOW_EDGE_EXPAND;
		rect.right -= WINDOW_EDGE_EXPAND;
	}
    
}

void WindowResizableHandler::CalculateHitType(int x, int y)
{
    POINT pt;
    pt.x = x;
    pt.y = y;

    RECT rect;
    GetHTMLRect(rect);
    
    _hitType = HTNOWHERE;

	RECT edgeRect;
	edgeRect.left = rect.left + HITTEST_EDGE_GAP + 1;
	edgeRect.top = rect.top + HITTEST_EDGE_GAP + 1;
	edgeRect.right = rect.right - HITTEST_EDGE_GAP - 1;
	edgeRect.bottom = rect.bottom - HITTEST_EDGE_GAP - 1;
	if (::PtInRect(&edgeRect, pt)) {
		_hitType =  HTCLIENT;
		return;
	}

	edgeRect.left = rect.right - HITTEST_EDGE_GAP;
	edgeRect.top = rect.bottom - HITTEST_EDGE_GAP;
	edgeRect.right = rect.right;
	edgeRect.bottom = rect.bottom;
	if (::PtInRect(&edgeRect, pt)) {
		_hitType =  HTBOTTOMRIGHT;
		return;
	}

	edgeRect.left = rect.left;
	edgeRect.top = rect.top;
	edgeRect.right = rect.left + HITTEST_EDGE_GAP;
	edgeRect.bottom = rect.top + HITTEST_EDGE_GAP;
	if (::PtInRect(&edgeRect, pt)) {
		_hitType =  HTTOPLEFT;
		return;
	}

	edgeRect.left = rect.right - HITTEST_EDGE_GAP;
	edgeRect.top = rect.top;
	edgeRect.right = rect.right;
	edgeRect.bottom = rect.top + HITTEST_EDGE_GAP;
	if (::PtInRect(&edgeRect, pt)) {
		_hitType =  HTTOPRIGHT;
		return;
	}

	edgeRect.left = rect.left;
	edgeRect.top = rect.bottom - HITTEST_EDGE_GAP;
	edgeRect.right = rect.left + HITTEST_EDGE_GAP;
	edgeRect.bottom = rect.bottom;
	if (::PtInRect(&edgeRect, pt)) {
		_hitType =  HTBOTTOMLEFT;
		return;
	}

	edgeRect.left = rect.right - HITTEST_EDGE_GAP;
	edgeRect.top = rect.top;
	edgeRect.right = rect.right;
	edgeRect.bottom = rect.bottom;
	if (::PtInRect(&edgeRect, pt)) {
		_hitType =  HTRIGHT;
		return;
	}

	edgeRect.left = rect.left;
	edgeRect.top = rect.bottom - HITTEST_EDGE_GAP;
	edgeRect.right = rect.right;
	edgeRect.bottom = rect.bottom;
	if (::PtInRect(&edgeRect, pt)) {
		_hitType =  HTBOTTOM;
		return;
	}

	edgeRect.left = rect.left;
	edgeRect.top = rect.top;
	edgeRect.right = rect.left + HITTEST_EDGE_GAP;
	edgeRect.bottom = rect.bottom;
	if (::PtInRect(&edgeRect, pt)) {
		_hitType =  HTLEFT;
		return;
	}

	edgeRect.left = rect.left;
	edgeRect.top = rect.top;
	edgeRect.right = rect.right;
	edgeRect.bottom = rect.top + HITTEST_EDGE_GAP;
	if (::PtInRect(&edgeRect, pt)) {
		_hitType =  HTLEFT;
		return;
	}
}

void  WindowResizableHandler::ResizeByHitType()
{
    int screenWidth = ::GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = ::GetSystemMetrics(SM_CYSCREEN);

    int width = _downRect.right - _downRect.left;
    int height = _downRect.bottom - _downRect.top;

    switch (_hitType)
    {
        case HTRIGHT:
            width = screenWidth - _downRect.left;
            break;
        case HTBOTTOM:
            height = screenHeight- _downRect.top;
            break;
        case HTBOTTOMRIGHT:
            width = screenWidth - _downRect.left;
            height = screenHeight- _downRect.top;
            break;
        case HTLEFT:
            width = screenWidth;
            break;
        case HTTOP:
            height = screenHeight;
            break;
        case HTTOPLEFT:
            width = screenWidth;
            height = screenHeight;
            break;
        case HTTOPRIGHT:
            width = screenWidth - _downRect.left;
            height = screenHeight;
            break;
        case HTBOTTOMLEFT:
            width = screenWidth;
            height = screenHeight- _downRect.top;
            break;
        default:
            return ;
    }

	::SetWindowPos(_hwnd, 0, 0, 0, width, height, SWP_NOMOVE | SWP_NOZORDER);
}

void  WindowResizableHandler::RestoreByHitType(POINT& pt)
{
    int originWidth = _downRect.right - _downRect.left;
    int originHeigth = _downRect.bottom - _downRect.top;

    int width = originWidth;
    int height = originHeigth;

    switch (_hitType)
    {
        case HTRIGHT:
            width = originWidth + pt.x - _downPt.x;
            break;
        case HTLEFT:
            width = originWidth - pt.x + _downPt.x;
            break;
        case HTBOTTOM:
            height = originHeigth + pt.y - _downPt.y;
			break;
        case HTTOP:
            height = originHeigth - pt.y + _downPt.y;
            break;
        case HTBOTTOMRIGHT:
            width = originWidth + pt.x - _downPt.x;
            height = originHeigth + pt.y - _downPt.y;
            break;
        case HTTOPLEFT:
            width = originWidth - pt.x + _downPt.x;
            height = originHeigth - pt.y + _downPt.y;
            break;
        case HTTOPRIGHT:
            width = originWidth + pt.x - _downPt.x;
            height = originHeigth - pt.y + _downPt.y;
            break;
        case HTBOTTOMLEFT:
            width = originWidth - pt.x + _downPt.x;
            height = originHeigth + pt.y - _downPt.y;
            break;
        default:
            ;
    }

	::SetWindowPos(_hwnd, 0, 0, 0, width, height, SWP_NOMOVE | SWP_NOZORDER);
	
	CorrectHTMLSize(width - 1, height - 1);

    _hitType = HTNOWHERE;
}

void WindowResizableHandler::MoveByHitType(POINT& pt)
{
    int x = _downRect.left;
    int y = _downRect.top;

    switch (_hitType)
    {
        case HTLEFT:
        case HTBOTTOMLEFT:
			x = _downRect.left + pt.x - _downPt.x;
            break;
        case HTTOP:
        case HTTOPRIGHT:
			//x = pt.x;
			y = _downRect.top + pt.y - _downPt.y;
            break;
        case HTTOPLEFT:
			x = _downRect.left + pt.x - _downPt.x;
			y = _downRect.top + pt.y - _downPt.y;
            break;
        default:
            return;
    }

     ::SetWindowPos(_hwnd, 0, x, y, 0, 0, SWP_NOZORDER|SWP_NOACTIVATE|SWP_NOSIZE);
}


void WindowResizableHandler::OnCursorChange(CefRefPtr<CefBrowser> browser,
    CefCursorHandle cursor) 
{
    if (!::IsWindow(_hwnd))
    {
        return ;
    }
	
	if (IsInEdge())
	{
		return;
	}
	else
	{
		SetClassLongPtr(_hwnd, GCL_HCURSOR,
			static_cast<LONG>(reinterpret_cast<LONG_PTR>(cursor)));
		SetCursor(cursor);
	}
}


void WindowResizableHandler::RedirectMouseEvent(int clientX, int clientY, RECT rect, int* redirectedX, int* redirectedY)
{
    switch(_hitType)
    {
        case HTLEFT:
			*redirectedX = rect.right - rect.left - clientX- WINDOW_EDGE_EXPAND;
		   *redirectedY = clientY;
           break;
        case HTTOP:
			*redirectedX = clientX;
           *redirectedY = rect.bottom - rect.top - clientY - WINDOW_EDGE_EXPAND;
           break;
        case HTTOPRIGHT:
			*redirectedX = clientX;
            *redirectedY = rect.bottom - rect.top - clientY- WINDOW_EDGE_EXPAND;
            break;
        case HTTOPLEFT:
            *redirectedX = rect.right - rect.left - clientX- WINDOW_EDGE_EXPAND;
            *redirectedY = rect.bottom - rect.top - clientY- WINDOW_EDGE_EXPAND;
            break;
        case HTBOTTOMLEFT:
           *redirectedX = rect.right - rect.left - clientX- WINDOW_EDGE_EXPAND;
		   *redirectedY = clientY;
           break;
    }
	
	if (_hitType == HTTOPRIGHT || _hitType == HTBOTTOMLEFT)
    {
        HCURSOR cursor = ::LoadCursor(NULL,IDC_SIZENESW);
		::SetCursor(cursor);
    }
}


void WindowResizableHandler::RedirectMovePosition(int clientX, int clientY, int screenX, int screenY, 
	RECT rect, int* redirectedX, int* redirectedY)
{
    switch(_hitType)
    {
        case HTLEFT:
           *redirectedX = rect.right - rect.left - screenX + _downPt.x - WINDOW_EDGE_EXPAND - clientX;
		   *redirectedY = clientY;
           break;
        case HTTOP:
			*redirectedX = clientX;
           *redirectedY = rect.bottom - rect.top - screenY + _downPt.y - WINDOW_EDGE_EXPAND - clientY;
           break;
        case HTTOPRIGHT:
			*redirectedX = clientX;
            *redirectedY = rect.bottom - rect.top - screenY + _downPt.y - WINDOW_EDGE_EXPAND - clientY;
            break;
        case HTTOPLEFT:
            *redirectedX = rect.right - rect.left - screenX + _downPt.x - WINDOW_EDGE_EXPAND- clientX;
            *redirectedY = rect.bottom - rect.top - screenY + _downPt.y - WINDOW_EDGE_EXPAND - clientY;
            break;
        case HTBOTTOMLEFT:
           *redirectedX = rect.right - rect.left - screenX + _downPt.x - WINDOW_EDGE_EXPAND- clientX;
		   *redirectedY = clientY;
           break;
    }
}

/*
void WindowResizableHandler::MaximizeWindow()
{         
    RECT rect;
    BOOL fResult = SystemParametersInfo(SPI_GETWORKAREA,
        0, &rect, 0);

    SetSize(rect.right -rect.left, rect.bottom - rect.top);
	Move(0,0);

    _state = WS_MAXMIZING;
	correctHTMLSize(rect.right -rect.left, rect.bottom - rect.top, true);
}

void WindowResizableHandler::NormonizeWindow()
{         
	SetSize(normalRect.right -normalRect.left, normalRect.bottom - normalRect.top);
	Move(normalRect.left,normalRect.top);
	
	correctHTMLSize( normalRect.right -normalRect.left - WINDOW_EDGE_EXPAND, 
		normalRect.bottom - normalRect.top - WINDOW_EDGE_EXPAND);

	_state = WS_NORMALING;
}
*/
bool WindowResizableHandler::CanResize()
{
	if (_state == WS_NORMAL && IsInEdge())
	{
		return true;
	}

	return false;
}

bool WindowResizableHandler::IsInEdge()
{
	return (_hitType != HTCLIENT && _hitType != HTNOWHERE) ? true : false;
}