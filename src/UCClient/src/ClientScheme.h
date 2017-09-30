#pragma once

#include <windows.h>

#include "include/cef_resource_handler.h"
#include "include/cef_scheme.h"

namespace ucclient {

class ClientSchemeHandler : public CefResourceHandler {
public:
    ClientSchemeHandler(CefRefPtr<CefBrowser> browser, cefIntegration::CefClient *cefClient) : _browser(browser), _cefClient(cefClient), _offset(0){}

	virtual bool ProcessRequest(CefRefPtr<CefRequest> request, CefRefPtr<CefCallback> callback);
	virtual void GetResponseHeaders(CefRefPtr<CefResponse> response, int64& response_length, CefString& redirectUrl);
	virtual bool ReadResponse(void* data_out, int bytes_to_read, int& bytes_read, CefRefPtr<CefCallback> callback);
	virtual void Cancel() OVERRIDE {}
	void ParseUrl(std::string & url, std::wstring& module, std::wstring& name, std::vector<std::wstring>& params);

private:
	int handleContactService(const std::wstring& method, const std::vector<std::wstring>& params, std::wstring &data);
	int handleClientService(const std::wstring& method, const std::vector<std::wstring>& params, std::wstring &data);
    int handleCalendarService(const std::wstring& method, const std::vector<std::wstring>& params, std::wstring &data);
	int handleSystemService(const std::wstring& method, const std::vector<std::wstring>& params, std::wstring &data);
	int handleFileTransferService(const std::wstring& method, const std::vector<std::wstring>& params, std::wstring &data);
	int handleGroupService(const std::wstring& method, const std::vector<std::wstring>& params, std::wstring &data);
	int handleExternalService(const std::wstring& method, const std::vector<std::wstring>& params, std::wstring &data);
	int handleUpdateService(const std::wstring& method, const std::vector<std::wstring>& params, std::wstring &data);
	int handleMicroblogService(const std::wstring& method, const std::vector<std::wstring>& params, std::wstring &data);
	int handleUtilService(const std::wstring& method, const std::vector<std::wstring>& params, std::wstring &data);
    int handleWindow(const std::wstring& method, const std::vector<std::wstring>& params, std::wstring &data);
	int handleAudioService(const std::wstring& method, const std::vector<std::wstring>& params, std::wstring &data);

private:
	CefRefPtr<CefBrowser> _browser;
    cefIntegration::CefClient *_cefClient;
	std::string _data;			// multi byte received from or sent to javascript
	std::wstring _dataW;			// utf-16

	size_t _offset; 	
	int _status;

	IMPLEMENT_REFCOUNTING(ClientSchemeHandler);
};

class ClientSchemeHandlerFactory : public CefSchemeHandlerFactory {
public:
    ClientSchemeHandlerFactory(cefIntegration::CefClient *cefClient) {
        _cefClient = cefClient;
    }

    ~ClientSchemeHandlerFactory() {
    }

	// Return a new scheme handler instance to handle the request.
	virtual CefRefPtr<CefResourceHandler> Create(CefRefPtr<CefBrowser> browser,
                                               CefRefPtr<CefFrame> frame,
                                               const CefString& scheme_name,
                                               CefRefPtr<CefRequest> request)
                                               OVERRIDE {
		return new ClientSchemeHandler(browser, _cefClient);
	}

	IMPLEMENT_REFCOUNTING(ClientSchemeHandlerFactory);

private:
    cefIntegration::CefClient *_cefClient;
};

}