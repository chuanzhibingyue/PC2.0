#pragma once

#include <atlbase.h>
#include <atlwin.h>
#include <atltypes.h>
#include <string>
#include <map>
#include <mutex>

extern CComModule _Module;
#define WM_PROGRESS (WM_USER+1900)
#define CONNECT_JS_CALL_HANDLER(name, func) ConnectHandler(L#name, (CallbackFunction)&func)
#define CONNECT_JS_CALL_SIMPLE_HANDLER(name, func) ConnectHandler(L#name, (CallbackFunction)&func, true)

/**
* A wrapper for html based dialogs
*/
class HtmlControl : public CWindowImpl<HtmlControl>, public DWebBrowserEvents2
{
public:
	HtmlControl(BSTR start);
	virtual ~HtmlControl() {}
	int DoModal(HWND parent = NULL);
	HRESULT CallJS(BSTR func, DISPPARAMS* params, VARIANT* retval);
private:
	HWND parent;
	CComBSTR startPagePath;
	CAxWindow wndIE;

private: // IDispatch methods
	STDMETHOD(QueryInterface)(REFIID riid, void **ppvObject);
	ULONG STDMETHODCALLTYPE AddRef(void) { return 1; }
	ULONG STDMETHODCALLTYPE Release(void) { return 1; }
	STDMETHOD(GetTypeInfoCount)(UINT*) { return E_NOTIMPL; }
	STDMETHOD(GetTypeInfo)(UINT, LCID, ITypeInfo**) { return E_NOTIMPL; }

protected: // IDispatch methods used for calls from JS
	STDMETHOD(GetIDsOfNames)(REFIID, LPOLESTR *rgszNames, UINT, LCID, DISPID *rgDispId);
	STDMETHOD(Invoke)(DISPID dispIdMember, REFIID, LCID, WORD, DISPPARAMS* pDispParams,
		VARIANT* pVarResult, EXCEPINFO*, UINT*);

private: // default JS event handlers
	void OnShow(DISPPARAMS*, VARIANT*);
	void OnClose(DISPPARAMS*, VARIANT*);
	void OnSetTitle(DISPPARAMS*, VARIANT*);
	void OnDragWindow(DISPPARAMS*, VARIANT*);
	void OnResizeWindow(DISPPARAMS*, VARIANT*);
	void OnMinimizeWindow(DISPPARAMS*, VARIANT*);
	void OnToggleMaximizeNormalWindow(DISPPARAMS*, VARIANT*);
	void OnSetOpacity(DISPPARAMS* params, VARIANT*);
	//add by wca
	void OnSetSize(DISPPARAMS*, VARIANT*);
	void OnSetTitleRect(DISPPARAMS*, VARIANT*);

	bool isMaximized;


private: // window message handlers
	int MessageLoop();
	BEGIN_MSG_MAP(HtmlControl)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
		MESSAGE_HANDLER(WM_SIZING, OnSizing)
		MESSAGE_HANDLER(WM_PROGRESS, OnProgress)
		MESSAGE_HANDLER(WM_TIMER, OnTimer);
		MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
	END_MSG_MAP()
	LRESULT OnCreate(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnKeyDown(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnSizing(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnProgress(UINT, WPARAM, LPARAM, BOOL&);
	LRESULT OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&);
	LRESULT OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&);

public: // visual effects
	static const int CutSize = 0;
private:
	void RemoveFrame();
	void FitToHost();
	void DeleteMeetingSetupExe();

public: // js calls 
	typedef void (HtmlControl::*CallbackFunction)(DISPPARAMS* params, VARIANT* retval);
	//typedef std::wstring (HtmlControl::*CallbackFunctionWithReturn)(DISPPARAMS* params, VARIANT* retval);
	typedef const char* (HtmlControl::*SimpleCallbackFunction)(int argc, const char** argv);
protected:
	// simple callback is defined as char* func(int argc, char* argv[])
	void ConnectHandler(BSTR name, CallbackFunction func, bool simple = false);
private:
	DISPID NameId(BSTR name) const;
	HRESULT Invoke(DISPID id, DISPPARAMS* params, VARIANT* retval);
	char* InPlaceWToA(BSTR bstr) const;
	struct JsCallHandler
	{
		DISPID id;
		union { CallbackFunction normal; SimpleCallbackFunction simple; } callback;
		bool simple;
	};
	static const DISPID BASE_DIPSPID = 100000;
	typedef std::map<std::wstring, JsCallHandler> JsHandlers;
	JsHandlers jsHandlers;
public:
	int m_posX;
	int m_posY;
	int m_height;
	CRect m_rTitle;
	BSTR m_callbackHandler;		//js注册的回调方法
	HANDLE m_hFinishInstall;	//安装完成事件
	HANDLE m_hBeginExcute;		//安装/卸载启动事件
	TCHAR m_szInstallDir[MAX_PATH];
	TCHAR m_szOldInstallDir[MAX_PATH];
	TCHAR m_szTargetPath[MAX_PATH];
};