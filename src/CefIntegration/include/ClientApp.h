/*
 * ClientApp.h
 *
 *  Created on: June 16, 2015
 *      Author: gang wang
 */

#ifndef CLIENT_APP_H_
#define CLIENT_APP_H_

#include <set>

#include "Delegate.h"
#include "renderer/ClientExtensionHandler.h"
#include "include/cef_app.h"
#include "include/cef_scheme.h"
#include "include/cef_v8.h"

namespace cefIntegration {

class Proxy;

class ClientApp : public CefApp,
				  public CefBrowserProcessHandler,
                  public CefRenderProcessHandler {
public:
    ClientApp();

    void RegisterRenderDelegate(RenderDelegate* renderDelegate);
	void RegisterExtension(const CefString& extensionName, const CefString& javascriptcode);

private:
    // CefApp methods.
	virtual void OnBeforeCommandLineProcessing(const CefString& process_type,
		CefRefPtr<CefCommandLine> command_line);
	virtual void OnRegisterCustomSchemes(CefRefPtr<CefSchemeRegistrar> registrar) OVERRIDE;
    virtual CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler() OVERRIDE { return this;}

    // CefBrowserProcessHandler methods
    virtual void OnContextInitialized() OVERRIDE;
    virtual CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler() {return this;}

    // CefRenderProcessHandler methods.
    virtual void OnWebKitInitialized() OVERRIDE;
    virtual void OnContextCreated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame,
        CefRefPtr<CefV8Context> context) OVERRIDE;
    virtual void OnContextReleased(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame,
        CefRefPtr<CefV8Context> context) OVERRIDE;
    virtual bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser, CefProcessId source_process,
        CefRefPtr<CefProcessMessage> message) OVERRIDE;

private:
	bool LoadCefExtensionCode(std::string & code);
    bool GetProxyCommond(std::wstring &cmmdLine);
	std::wstring GetPluginPath();
private:
    typedef std::set<CefRefPtr<RenderDelegate> > RenderDelegateSet;
    RenderDelegateSet _renderDelegates;

    CefRefPtr<ClientExtensionHandler> _v8Handler;

    Proxy*   _ieProxy;
  IMPLEMENT_REFCOUNTING(ClientApp);
};

}

#endif