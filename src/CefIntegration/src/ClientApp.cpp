#include "ClientApp.h"
#include "ClientDefs.h"
#include "resources/resource.h"
#include "LogManager.h"
#include "browser/Proxy.h"

#include <sstream>
#include <cassert>

namespace cefIntegration{

using namespace uc;

ClientApp::ClientApp() {
	_v8Handler = new ClientExtensionHandler();
	RegisterRenderDelegate(_v8Handler.get());

    _ieProxy = new Proxy;
}

std::wstring ClientApp::GetPluginPath(){
	WCHAR szPath[MAX_PATH] = { 0 };
	GetModuleFileName(NULL, szPath, MAX_PATH);
	std::wstring strPath = szPath;
	size_t nPos = strPath.find_last_of(L"\\");
	if (nPos == 0){
		nPos = strPath.find_last_of('/');
		if (nPos == 0){
			return L"";
		}
	}
	strPath = strPath.substr(0, nPos + 1);
	strPath = strPath + L"plugin\\UCPlugin.dll;application/x-ppapi-uc";

	return strPath;
}

void ClientApp::OnBeforeCommandLineProcessing(const CefString& process_type,
	CefRefPtr<CefCommandLine> command_line) {

	//×¢²á²å¼þ£»
	std::wstring strPath = GetPluginPath();
	command_line->AppendSwitch("--ppapi-out-of-process");
	command_line->AppendSwitchWithValue("--register-pepper-plugins", strPath);

    command_line->AppendSwitchWithValue("--ppapi-flash-version", "19.0.0.245");
    command_line->AppendSwitchWithValue("--ppapi-flash-path", "plugin\\pepflashplayer.dll");

	CefString disableGPU("disable-gpu"); 
	CefString disableWebSecurity("disable-web-security"); 

	command_line->AppendSwitch(disableGPU);
    command_line->AppendSwitch("disable-gpu-compositing");
	command_line->AppendSwitch(disableWebSecurity);
	command_line->AppendSwitch("enable-begin-frame-scheduling");
	command_line->AppendArgument("disable-direct-write");
	command_line->AppendArgument("--touch-events=\"enabled\"");
    command_line->AppendArgument("--num-raster-threads=1");
    command_line->AppendArgument("ignore-gpu-blacklist");
    command_line->AppendArgument("--disable-zero-copy");

    std::wstring proxyCommand;
    if (GetProxyCommond(proxyCommand)) // proxy commondline
    {
        command_line->AppendSwitch(proxyCommand);
    }
    //command_line->AppendSwitch("process-per-site");//only one process
}

bool ClientApp::GetProxyCommond(std::wstring &proxyCommand){
    proxyCommand = L"";
    std::wstring proxyValue;
    int proxyType = _ieProxy->GetProxyType();
    if (proxyType == Proxy_Invalid
        || proxyType == Proxy_No)
    {
        return false;
    }

    if (Proxy_Pac == proxyType)
    {
        _ieProxy->GetPacUrlProxy(proxyValue);
        proxyCommand = L"--proxy-pac-url=\"";
        proxyCommand += proxyValue;
        proxyCommand += L"\"";
    }
    else if (Proxy_Server == proxyType)
    {
        _ieProxy->GetServerProxy(proxyValue);
        proxyCommand = L"--proxy-server=\"";
        proxyCommand += proxyValue;
        proxyCommand += L"\"";

        //proxyCommand = L"proxy-server=\"10.0.1.3:8080\"";
    }
    UC_LOG(INFO) << "proxy command line:" << proxyCommand;

    return true;
}
void ClientApp::OnRegisterCustomSchemes(CefRefPtr<CefSchemeRegistrar> registrar) {
    registrar->AddCustomScheme("client", true, false, false);
}

void ClientApp::OnContextInitialized() {
}

void ClientApp::RegisterExtension(const CefString& extensionName, const CefString& javascriptcode) {
	CefRegisterExtension(extensionName, javascriptcode,  _v8Handler.get());
}

void ClientApp::OnWebKitInitialized() {
	UC_LOG(INFO) << "OnWebKitInitialized";

	if (_v8Handler.get()) {
		std::stringstream codeStream;

		std::string code;
		// Register the uc extension.
		if (LoadCefExtensionCode(code)) {
			CefRegisterExtension(L"v8/cef_app", code,  _v8Handler.get());
		}

		RenderDelegateSet::iterator iter = _renderDelegates.begin();
		for (; iter != _renderDelegates.end(); ++iter) {
			(*iter)->OnWebKitInitialized();
		}
	}
}

bool ClientApp::LoadCefExtensionCode(std::string & code) {
	HMODULE hMod = GetModuleHandle(L"CefIntegration.dll");
    HRSRC hRes = FindResource(hMod, MAKEINTRESOURCE(IDS_CEF_EXTENSIONS), MAKEINTRESOURCE(BINARY));
    DWORD dwSize;
    LPBYTE pBytes = NULL;

    if(hRes)
    {
        HGLOBAL hGlob = LoadResource(hMod, hRes);
        if(hGlob)
        {
            dwSize = SizeofResource(hMod, hRes);
            pBytes = (LPBYTE)LockResource(hGlob);
        }
    }
	else {
		int err = GetLastError();
	}

	if (pBytes) {
		code.append((const char *)pBytes, dwSize);
		return true;
	}

	return false;
}
void ClientApp::OnContextCreated(CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context) {
	RenderDelegateSet::iterator iter = _renderDelegates.begin();
	for (; iter != _renderDelegates.end(); ++iter) {
		(*iter)->OnContextCreated(browser, frame, context);
	}
}

void ClientApp::OnContextReleased(CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context) {
	RenderDelegateSet::iterator iter = _renderDelegates.begin();
	for (; iter != _renderDelegates.end(); ++iter) {
		(*iter)->OnContextReleased(browser, frame, context);
	}
}

bool ClientApp::OnProcessMessageReceived(CefRefPtr<CefBrowser> browser, 
	CefProcessId source_process, CefRefPtr<CefProcessMessage> message) {
	assert(source_process == PID_BROWSER);

    bool handled = false;

    // Execute delegate callbacks.
    RenderDelegateSet::iterator iter = _renderDelegates.begin();
    for (; iter != _renderDelegates.end() && !handled; ++iter) 
	{
		handled = (*iter)->OnProcessMessageReceived(browser, source_process, message);
    }
    
  return handled;
}

void ClientApp::RegisterRenderDelegate(RenderDelegate* renderDelegate) {
	_renderDelegates.insert(renderDelegate);
}

}