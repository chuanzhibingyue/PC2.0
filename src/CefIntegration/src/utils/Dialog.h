#ifndef DIALOG_H_
#define DIALOG_H_

#include <string>

#include "include/cef_browser.h"


namespace cefIntegration {

class Dialog {
public:
	void RunOpenDialog(CefRefPtr<CefBrowser> browser, const CefString& title, const std::map<std::wstring, std::wstring> &filters, bool isMulti, int requestId);

	void RunSaveDialog(CefRefPtr<CefBrowser> browser,  const CefString& title, const CefString& defaultFileName, const std::map<std::wstring, std::wstring> &filters, int requestId);
private:
	void RunDialog(HWND wnd, const CefString& title, const std::map<std::wstring, std::wstring> &filters, const std::wstring& defaultName, 
		bool isMulti, std::vector<std::wstring> &path, bool isSave);

	void SendDialogMessage(CefRefPtr<CefBrowser> browser, int requestId, std::vector<std::wstring> &path);
};

}

#endif