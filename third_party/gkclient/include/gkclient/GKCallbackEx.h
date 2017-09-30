#pragma once
#include "GKCallback.h"
#include "CefClient.h"
using namespace cefIntegration;

class GKCallbackEx :public GKCallback
{
public:
	GKCallbackEx();
	~GKCallbackEx();
	void SetClient(cefIntegration::CefClient* client);
	virtual void CefFunctionCallback(const int& hwnd, const wstring& name, const wstring& json, const int& num);
	virtual void ExecuteJSForBrowser(const int& hwnd, const wstring& content);
	virtual void SetTrayBalloon(const int& hwnd, int icon, const wstring& title, const wstring& text, int timeout);
	virtual void SetUserMessage(const wstring& content);
private:
	cefIntegration::CefClient* m_client;
};

