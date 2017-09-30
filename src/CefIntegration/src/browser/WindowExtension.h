/*
 * WindowExtension.h
 *
 *  Created on: August 26, 2015
 *      Author: gang wang
 */

#ifndef WINDOW_EXTENSION_H_
#define WINDOW_EXTENSION_H_

#include <vector>
#include "Delegate.h"
#include "browser/WindowManager.h"

namespace cefIntegration {

class CefClient;

class WindowExtension : public BrowserDelegate {
public:
	WindowExtension(CefClient* cefClient);

	virtual bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
        CefProcessId source_process, CefRefPtr<CefProcessMessage> message);
	
	void AppMethodProcess(CefRefPtr<CefBrowser> browser, CefString method, int argLen, 
		CefRefPtr<CefListValue> arguments, int &res, CefRefPtr<CefListValue> responseArgs);
private:
	void DialogMethodProcess(CefRefPtr<CefBrowser> browser, CefString method, int argLen, 
		CefRefPtr<CefListValue> arguments, int &res, CefRefPtr<CefListValue> responseArgs, int requestId);

	void WindowMethodProcess(CefRefPtr<CefBrowser> browser, CefString method, int argLen, 
		CefRefPtr<CefListValue> arguments, int &res, CefRefPtr<CefListValue> responseArgs);

	bool ConvertDictionaryToWindowFeature(CefRefPtr<CefDictionaryValue> dictionary, 
		WindowFeature &feature);
	void ConverListToCaptions(CefRefPtr<CefListValue> captionList, std::vector<RECT> &captions) ;
	bool ConvertDictionaryToMenuFeature(CefRefPtr<CefDictionaryValue> dictionary, 
		MenuFeature &feature) ;

	IMPLEMENT_REFCOUNTING(WindowExtension);

	CefClient* _cefClient;
	//WindowManager* _windowManager;
};

}

#endif