/*
 * Delegate.h
 *
 *  Created on: June 16, 2015
 *      Author: gang wang
 */

#ifndef DELEGATE_H_
#define DELEGATE_H_

#include "include/cef_app.h"
#include "include/cef_process_message.h"
#include "include/cef_browser.h"
#include "include/cef_v8.h"

namespace cefIntegration {

class RenderDelegate : public virtual CefBase {
public:
	virtual bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser, 
		CefProcessId source_process,
        CefRefPtr<CefProcessMessage> message) {return false;}
		
	virtual void OnContextCreated(CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context) {}

	virtual void OnContextReleased(CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context) {}

	//virtual void GetExtensionCode(std::string& code) {}

	virtual void OnWebKitInitialized(){}
};

class BrowserDelegate : public virtual CefBase {
public:
    virtual bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
        CefProcessId source_process, 
		CefRefPtr<CefProcessMessage> message) {return false;}
};

}

#endif