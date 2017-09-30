#pragma once
#include "Delegate.h"

namespace gkclient {
	using namespace cefIntegration;
	typedef std::map<std::pair<std::string, int>, std::pair<CefRefPtr<CefV8Context>, CefRefPtr<CefV8Value>>> CallbackMap;
	class GKClientRenderDelegate : public RenderDelegate
	{
	public:
		virtual bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
			CefProcessId source_process, CefRefPtr<CefProcessMessage> message);
		virtual void OnContextReleased(CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context);
		virtual void OnContextCreated(CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context);
		virtual void OnWebKitInitialized();
		IMPLEMENT_REFCOUNTING(GKClientRenderDelegate);

	public:
		void SetMessageCallback(const std::string& message_name,
			int browser_id,
			CefRefPtr<CefV8Context> context,
			CefRefPtr<CefV8Value> function);
	private:
		CallbackMap m_CallbackMap;
	};
}

