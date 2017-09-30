/*
 * CefClient.h
 *
 *  Created on: June 16, 2015
 *      Author: gang wang
 */

#ifndef CEF_CLIENT_H_
#define CEF_CLIENT_H_

#include "Delegate.h"
#include "ClientApp.h"
#include "browser/ClientHandler.h"
#include "ClientDefs.h"
#include "browser/WindowManager.h"

namespace cefIntegration {

class CEF_CLIENT_API CefClient {
public:
	CefClient();
	
	~CefClient();

	/**
     * 在主进程初始化CEF
     * @param HINSTANCE 应用程序句柄
     */
	int InitializeMainProcess(HINSTANCE hInstance, const std::wstring &subProcessPath, const std::wstring &logPath);
	
	/**
     * 在子进程初始化CEF
     * @param hwnd 应用程序句柄
     */
	int InitializeSubProcess(HINSTANCE hInstance);
	
	void Quit();

	/**
     * 创建浏览器
     * @param hwnd 窗口句柄
	 * @param url 加载的url
	 * @param istransparent 是否透明
     */
	void CreateBrowser(HWND hwnd, const std::wstring &url, bool istransparent);
	
	/**
     * 离屏渲染下，设置窗口句柄对应的CefRenderHandler
     * @param hwnd 窗口句柄
	 * @param url 加载的url
     */
	void SetRenderHandler(CefWindowHandle hwnd, CefRefPtr<CefRenderHandler> handler);

	void RemoveRenderHandler(CefWindowHandle hwnd);

	/**
     * 注册主进程消息监听器
     * @param delegate 消息监听器
    */
	void RegisterBrowserDelegate(BrowserDelegate *browserDelegate);
	
	void RegisterRenderDelegate(RenderDelegate *browserDelegate);

	void RegisterExtension(const CefString& extensionName, const CefString& javascriptcode);

	/**
     * 在所有浏览器中执行JS代码
     * @param delegate 消息监听器
    */
	void ExecuteJSForAllBrowsers(const std::wstring &code);
	void ExecuteJSForAllBrowsers(const std::string &code);

	/**
     * 关闭CEF，释放资源
    */
	void Shutdown();

    void SetAuthInfo(const std::wstring &account, const std::wstring &pwd);
    void GetAuthInfo(std::wstring &account, std::wstring &pwd);

    void GetTrayPos(CefRefPtr<CefBrowser> browser, std::wstring & trayPos);
    int GetWindowStatue(CefRefPtr<CefBrowser> browser);
    int IsWindowOpen(CefRefPtr<CefBrowser> browser, const std::wstring name);

	WindowManager* getWindowManager() {return _windowManager;}

	CefRefPtr<ClientApp> getApp() {return _app;}
	CefRefPtr<ClientHandler> getHandler() {return _handler;}

	CefRefPtr<CefBrowser> GetBrowser(CefWindowHandle cwHandle);

private:
	WindowManager *_windowManager;

	CefRefPtr<ClientApp> _app;
	CefRefPtr<ClientHandler> _handler;
};

}
#endif