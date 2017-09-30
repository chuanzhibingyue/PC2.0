/*
 * Window.h
 *
 *  Created on: June 27, 2015
 *      Author: gang wang
 */

#ifndef WINDOW_H_
#define WINDOW_H_

#include "CefOSRWindow.h"

namespace uc {

struct MenuFeature{
	int id;
	std::wstring icon;
	std::wstring label;
};

class Window : public CefOSRWindow {
public:
	Window(const std::wstring& name, CefClient *client);
	virtual ~Window();

	void Create(const std::wstring &title, std::wstring &url, HWND hwndParent, bool isModal, bool isResizeable,
		int minWidth, int minHeight, bool hasTaskBarIcon) ;

	void SetTop(bool isTop);

	void SetSize(int width, int height);

	void Move(int x, int y);

	void Minimize();

	void Maximize();

	void Restore();

	void Show();

	void Hide();

	void Close();

	void CenterWindow();

	void SetForeground();

	void Navigate(const std::wstring &url);

	void SetCaptionArea(const std::vector<RECT> & captions);

	void SetTray(const std::wstring & title, const std::wstring & imageName, std::vector<MenuFeature> &menuFeatures) ;

	void DeleteTray();

	virtual HWND GetWnd() {return _hwnd;}

	std::wstring& GetName() {return _name;}

	void ShowContextMenu();

	LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
private:
	UINT OnNCHitTest(WPARAM wParam, LPARAM lParam);

	void OnClose();

	void OnSize(WPARAM wParam);

	LRESULT OnCommand(WPARAM wParam) ;

	LRESULT OnSysTray(LPARAM lParam) ;

	ATOM RegisterWndClass();

	LRESULT Window::OnGetMinMaxInfo(LPARAM lParam);

	bool IsInCaption(POINT &pt);

private:
	HWND _hwnd;

	bool _isModal;
	bool _isResizeable;

	std::vector<RECT> _captionAreas;

	std::wstring _name;
	int _minWidth;
	int _minHeight;
	
	int _trayId;
	std::vector<MenuFeature> _menuFeatures;

	bool _maximized;

	IMPLEMENT_REFCOUNTING(Window);
};

}
#endif