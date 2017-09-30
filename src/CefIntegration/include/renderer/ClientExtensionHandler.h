/*
 * ClientExtensionHandler.h
 *
 *  Created on: June 26, 2015
 *      Author: gang wang
 */
#ifndef CLIENT_EXTENSION_HANDLER_H_
#define CLIENT_EXTENSION_HANDLER_H_

#include "include/cef_v8.h"
#include "Delegate.h"

namespace cefIntegration {

class ClientExtensionHandler : public CefV8Handler,
							   public RenderDelegate {
public:
	ClientExtensionHandler();

	// CefV8Handler 方法
	virtual bool Execute(const CefString& name, CefRefPtr<CefV8Value> object,
        const CefV8ValueList& arguments, CefRefPtr<CefV8Value>& retval,
        CefString& exception);
	
	// RenderDelegate 方法
	virtual bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser, 
		CefProcessId source_process, CefRefPtr<CefProcessMessage> message);
	
	virtual void OnContextReleased(CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context);
			
private:
	void CallNativeMethodAync(const CefV8ValueList& arguments, const CefString& messageName);
	
private:
	int _requestId;
	
	typedef std::map<int32, std::pair< CefRefPtr<CefV8Context>, CefRefPtr<CefV8Value> > > JSCallbackMap;
    JSCallbackMap _callbackMap;
	
	IMPLEMENT_REFCOUNTING(ClientExtensionHandler);
};

}
#endif