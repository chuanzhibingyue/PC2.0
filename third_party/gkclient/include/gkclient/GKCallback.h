#pragma once
#include <string>
using namespace std;

class GKCallback
{
public:
	virtual ~GKCallback(){}
	virtual void CefFunctionCallback(const int& hwnd, const wstring& name, const wstring& json, const int& num) = 0;
	virtual void ExecuteJSForBrowser(const int& hwnd, const wstring& content) = 0;
	virtual void SetUserMessage(const wstring& content) = 0;
	virtual void SetTrayBalloon(const int& hwnd, int icon, const wstring& title, const wstring& text, int timeout) = 0;
};

