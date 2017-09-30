#include "CefClient.h"
#include "ClientDefs.h"
#include "browser/WindowExtension.h"

#include <string>
#include <sstream>
#include <codecvt>

HINSTANCE hModuleInst;

namespace cefIntegration {

CefClient::CefClient() : _windowManager(NULL){
	_app = new ClientApp();
}

CefClient::~CefClient() {
	if (_windowManager != NULL) {
		delete _windowManager;
		_windowManager = NULL;
	}
}

int CefClient::InitializeSubProcess(HINSTANCE hInstance) {
	CefMainArgs mainArgs(hInstance);
	return CefExecuteProcess(mainArgs, _app.get(), NULL);
}

int CefClient::InitializeMainProcess(HINSTANCE hInstance, const std::wstring& subProcessPath, const std::wstring& logPath) {
	// Enable High-DPI support on Windows 7 or newer.
	CefEnableHighDPISupport();
	
	hModuleInst = hInstance;
	CefMainArgs mainArgs(hInstance);
	
	CefExecuteProcess(mainArgs, _app.get(), NULL);

	CefSettings settings;
	// 设置子进程exe的路径
	CefString(&settings.browser_subprocess_path).FromWString(subProcessPath);
	settings.multi_threaded_message_loop = false;
	settings.no_sandbox = true;
	settings.log_severity = LOGSEVERITY_VERBOSE;
	CefString(&settings.log_file).FromWString(logPath);

	// 设置local storage
// 	std::wstring pathLocalStorage = logPath.substr(0, logPath.find(L"log"));
// 	pathLocalStorage += L"localStorage\\";
// 	CefString(&settings.cache_path).FromWString(pathLocalStorage);

	if (CefInitialize(mainArgs, settings, _app.get(), NULL)) {
        _handler = new ClientHandler(hInstance, this);

		_windowManager = new WindowManager(this);
		RegisterBrowserDelegate(new WindowExtension(this));

		return CEF_SUCCESS;
	}
	else {
		return CEF_INITIALIZE_ERROR;
	}
}

void CefClient::Quit() {
	_handler->Quit();
}

void CefClient::CreateBrowser(HWND hwnd, const std::wstring &url, bool istransparent) {
	CefWindowInfo info;
	if (istransparent) {
		info.SetAsWindowless(hwnd, TRUE);
	}
	else {
		RECT rect;
		GetClientRect(hwnd, &rect);
		info.SetAsChild(hwnd, rect);
	}

	CefBrowserSettings browser_settings;
	CefBrowserHost::CreateBrowser(info, _handler.get(), url, browser_settings, NULL);
}

void CefClient::SetRenderHandler(CefWindowHandle hwnd, CefRefPtr<CefRenderHandler> handler) {
	_handler->SetRenderHandler(hwnd, handler);
}

void CefClient::RemoveRenderHandler(CefWindowHandle hwnd) {
	_handler->RemoveRenderHandler(hwnd);
}

void CefClient::RegisterBrowserDelegate(BrowserDelegate *browserDelegate) { 
	_handler->RegisterBrowserDelegate(browserDelegate);
}

void CefClient::RegisterRenderDelegate(RenderDelegate *browserDelegate) {
	_app->RegisterRenderDelegate(browserDelegate);
}

void CefClient::RegisterExtension(const CefString& extensionName, const CefString& javascriptcode) {
	_app->RegisterExtension(extensionName, javascriptcode);
}

void CefClient::ExecuteJSForAllBrowsers(const std::wstring &code) {
	_handler->ExecuteJSForAllBrowsers(code);
}

void CefClient::ExecuteJSForAllBrowsers(const std::string &code) {
	std::wstring wdata;
	std::wstring_convert<std::codecvt_utf8<wchar_t> > convert;
	try {
		wdata = convert.from_bytes(code);
	} catch(const std::range_error& ) {
		wdata = convert.from_bytes(code.substr(0, convert.converted()));
	}

	_handler->ExecuteJSForAllBrowsers(wdata);
}

void CefClient::Shutdown() {
	CefShutdown();
}

CefRefPtr<CefBrowser> CefClient::GetBrowser(CefWindowHandle cwHandle) {
	return _handler->GetBrowser(cwHandle);
}

void CefClient::SetAuthInfo(const std::wstring &account, const std::wstring &pwd) {
    _handler->SetProxyAuthInfo(account, pwd);
}

void CefClient::GetAuthInfo(std::wstring &account, std::wstring &pwd)
{
    _handler->GetProxyAuthInfo(account, pwd);
}

void CefClient::GetTrayPos(CefRefPtr<CefBrowser> browser, std::wstring & trayPos)
{
    _windowManager->GetTrayPos(browser, trayPos);
}

int CefClient::GetWindowStatue(CefRefPtr<CefBrowser> browser)
{
    return _windowManager->GetWindowStatue(browser);
}

int CefClient::IsWindowOpen(CefRefPtr<CefBrowser> browser, const std::wstring name)
{
    return _windowManager->IsWindowOpen(browser, name);
}

}