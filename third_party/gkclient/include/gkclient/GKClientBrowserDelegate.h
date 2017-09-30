#pragma once
#include "Delegate.h"
#include "CefClient.h"
#include "GKCallbackEx.h"

namespace gkclient {
	using namespace cefIntegration;
	class  GKClientBrowserDelegate : public BrowserDelegate
	{
	public:
		GKClientBrowserDelegate(cefIntegration::CefClient* client,GKCallbackEx* callback);
		virtual bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
			CefProcessId source_process,
			CefRefPtr<CefProcessMessage> message);
		IMPLEMENT_REFCOUNTING(GKClientBrowserDelegate);

		void DoMessage(CefRefPtr<CefBrowser> browser, CefString name, CefRefPtr<CefListValue> arguments,HWND hwnd);
	private:
		cefIntegration::CefClient* m_client;
		GKCallbackEx* m_callback;
	};
}