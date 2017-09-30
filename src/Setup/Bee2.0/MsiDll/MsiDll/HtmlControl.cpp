#define  _WIN32_WINNT 0x0501
#include "HtmlControl.h"
#include <string>
#include "LogManager.h"
using namespace uc;
#define MAINWND_INSTALL		L"\\index.html"		//安装页面
#define MAINWND_UNINSTALL	L"\\unload.html"	//卸载页面
#define TIMER_PROGRESS 1234567					//mock进度条定时器
#define UCCMEETING_SETUP_PATH	L"\\UCCMeetingSetup.exe"	//云会议安装包名称
extern int g_arrProgress[6];
extern int g_nCurProgress;
extern int g_nProgressIndex;
extern BOOL g_bCloseMainWnd;
extern int g_nInstallType;
HtmlControl::HtmlControl(BSTR start)
	: startPagePath(start), isMaximized(false)
{
	// connect js handlers
	CONNECT_JS_CALL_HANDLER(Show, HtmlControl::OnShow);
	CONNECT_JS_CALL_HANDLER(Close, HtmlControl::OnClose);
	CONNECT_JS_CALL_HANDLER(SetTitle, HtmlControl::OnSetTitle);
	CONNECT_JS_CALL_HANDLER(DragWindow, HtmlControl::OnDragWindow);
	CONNECT_JS_CALL_HANDLER(ResizeWindow, HtmlControl::OnResizeWindow);
	CONNECT_JS_CALL_HANDLER(MinimizeWindow, HtmlControl::OnMinimizeWindow);
	CONNECT_JS_CALL_HANDLER(ToggleMaximizeNormalWindow, HtmlControl::OnToggleMaximizeNormalWindow);
	CONNECT_JS_CALL_HANDLER(SetOpacity, HtmlControl::OnSetOpacity);

	//add 
	CONNECT_JS_CALL_HANDLER(SetSize, HtmlControl::OnSetSize);
	CONNECT_JS_CALL_HANDLER(SetTitleRect, HtmlControl::OnSetTitleRect);

	m_height = 420;
}

int HtmlControl::DoModal(HWND parent /*= NULL*/)
{
	BOOL isParentEnabled;
	if (parent)
	{	// disable parent window
		isParentEnabled = ::IsWindowEnabled(parent);
		if (isParentEnabled)
			::EnableWindow(parent, FALSE);
	}
	RECT rect = { 0, 0, 0, 0 };
	Create(parent, rect, _T(""), WS_POPUP | WS_VISIBLE | WS_EX_TOPMOST);
	int retcode = MessageLoop();
	SetForegroundWindow(parent);
	DestroyWindow();
	if (parent && isParentEnabled)
		::EnableWindow(parent, TRUE); // enable parent window	
	
	//窗口销毁后通知主线程退出
	UC_LOG(INFO) << "after window destroyed, setevent.";
	SetEvent(m_hBeginExcute);
	SetEvent(m_hFinishInstall);

	return retcode;
}

HRESULT HtmlControl::CallJS(BSTR func, DISPPARAMS* params, VARIANT* retval)
{
	HRESULT hr = S_OK;
	IWebBrowser2* browser;
	hr = wndIE.QueryControl(__uuidof(IWebBrowser2), (void**)&browser);
	ATLASSERT(SUCCEEDED(hr));
	IDispatch* disp;
	hr = browser->get_Document(&disp);
	ATLASSERT(SUCCEEDED(hr));
	IHTMLDocument2* doc;
	hr = disp->QueryInterface(__uuidof(IHTMLDocument2), (void**)&doc);
	ATLASSERT(SUCCEEDED(hr));
	IDispatch* spDisp;
	hr = doc->get_Script(&spDisp);
	ATLASSERT(SUCCEEDED(hr));
	USES_CONVERSION;
	OLECHAR FAR* name = func;
	DISPID dispid;
	hr = spDisp->GetIDsOfNames(IID_NULL, &name, 1, LOCALE_SYSTEM_DEFAULT, &dispid);
	if (FAILED(hr))
		return hr;
	hr = spDisp->Invoke(dispid, IID_NULL, LOCALE_SYSTEM_DEFAULT,
		DISPATCH_METHOD, params, retval, NULL, NULL);
	return hr;
}

//------------------------- IDispatch methods used for calls from JS -------------------------------

STDMETHODIMP HtmlControl::QueryInterface(REFIID riid, void **ppvObject)
{
	HRESULT hr = E_NOINTERFACE;
	if (riid == __uuidof(IDispatch))
	{
		*ppvObject = (IDispatch*)this;
		hr = S_OK;
	}
	else if (riid == __uuidof(DWebBrowserEvents2))
	{
		*ppvObject = (DWebBrowserEvents2*)this;
		hr = S_OK;
	}
	return hr;
}

STDMETHODIMP HtmlControl::GetIDsOfNames(REFIID, LPOLESTR *rgszNames, UINT, LCID, DISPID *rgDispId)
{
	*rgDispId = NameId(rgszNames[0]);
	return S_OK;
}

STDMETHODIMP HtmlControl::Invoke(DISPID dispIdMember, REFIID, LCID, WORD, DISPPARAMS* pDispParams,
	VARIANT* pVarResult, EXCEPINFO*, UINT*)
{
	HRESULT hr = E_NOTIMPL;
	if (dispIdMember >= BASE_DIPSPID)
		hr = Invoke(dispIdMember, pDispParams, pVarResult);
	return hr;
}
//--------------------------------- default JS event handlers --------------------------------------

void HtmlControl::OnShow(DISPPARAMS* params, VARIANT*)
{
	ATLASSERT(params->cArgs >= 2);
	int width = params->rgvarg[1].intVal + CutSize * 2;
	int height = params->rgvarg[0].intVal + CutSize * 2;
	RECT parent;
	if (GetParent().IsWindow())
		GetParent().GetWindowRect(&parent);
	else
		::GetWindowRect(GetDesktopWindow(), &parent);
	int x, y;
	if (params->cArgs == 2)
	{	// position - center
		x = (parent.left + parent.right - width) / 2;
		y = (parent.bottom + parent.top - height) / 2;
	}
	else if (params->cArgs == 4)
	{	// x/y position
		y = params->rgvarg[2].intVal + parent.top;
		x = params->rgvarg[3].intVal + parent.left;
	}
	else if (params->cArgs == 5 &&
		params->rgvarg[4].vt == VT_BSTR && wcscmp(params->rgvarg[4].bstrVal, L"mouse") == 0)
	{	// relative to mouse position
		POINT mousePos;
		GetCursorPos(&mousePos);
		y = params->rgvarg[2].intVal + mousePos.y;
		x = params->rgvarg[3].intVal + mousePos.x;
	}
	MoveWindow(x, y, width, height);
	m_posX = x;
	m_posY = y;
	FitToHost();
	ShowWindow(SW_SHOW);
}

void HtmlControl::OnSetSize(DISPPARAMS* params, VARIANT*)
{
	int width = params->rgvarg[1].intVal + CutSize * 2;
	int height = params->rgvarg[0].intVal + CutSize * 2;
	m_height = height;
	MoveWindow(m_posX, m_posY, width, height);
	FitToHost();
}

void HtmlControl::OnSetTitleRect(DISPPARAMS* params, VARIANT*)
{
	int posX = params->rgvarg[3].intVal + CutSize * 2;
	int posY = params->rgvarg[2].intVal + CutSize * 2;
	int width = params->rgvarg[1].intVal + CutSize * 2;
	int height = params->rgvarg[0].intVal + CutSize * 2;
	CRect rc(posX, posY, posX + width, posY + height);
	m_rTitle = rc;
}


void HtmlControl::OnClose(DISPPARAMS* params, VARIANT*)
{
	
	int retcode = -1; // undefined
	if (params->cArgs == 1)
		retcode = params->rgvarg[0].uintVal;
	UC_LOG(INFO) << "OnClose enter. params = " << retcode << ", TargetPath = " << m_szTargetPath;
	KillTimer(TIMER_PROGRESS);
	if (retcode == 1)
	{
		//删除云会议安装包。
		//DeleteMeetingSetupExe();
		//调起蜜蜂
		HINSTANCE hs = ShellExecute(NULL, _T("open"), _T("bee.exe"), NULL, m_szInstallDir, SW_SHOW);
		UC_LOG(INFO) << "ShellExecute open bee.exe. return code = " << hs;
	}

	// do exit
	//通知msi程序退出
	Sleep(200);
	g_bCloseMainWnd = TRUE;
	UC_LOG(INFO) << "OnClose, after set finish event.";
	//UC_LOG_FLUSH();
	PostMessage(WM_CLOSE, retcode, 0);
}

void HtmlControl::OnSetTitle(DISPPARAMS* params, VARIANT*)
{
	ATLASSERT(params->cArgs == 1);
	SetWindowText(params->rgvarg[0].bstrVal);
}

void HtmlControl::OnDragWindow(DISPPARAMS*, VARIANT*)
{
	//PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, NULL);
}

void HtmlControl::OnResizeWindow(DISPPARAMS* params, VARIANT*)
{
	int dir = HTBOTTOMRIGHT;
	if (params->cArgs == 1)
		dir = params->rgvarg[0].intVal;
	PostMessage(WM_NCLBUTTONDOWN, dir, NULL);
}

void HtmlControl::OnMinimizeWindow(DISPPARAMS*, VARIANT*)
{
	ShowWindow(SW_SHOWMINIMIZED);
}

void HtmlControl::OnToggleMaximizeNormalWindow(DISPPARAMS*, VARIANT*)
{
	ShowWindow(isMaximized ? SW_SHOWNORMAL : SW_SHOWMAXIMIZED);
	isMaximized = !isMaximized;
	BOOL dummy;
	OnSizing(0, 0, 0, dummy);
}

void HtmlControl::OnSetOpacity(DISPPARAMS* params, VARIANT*)
{
	int opacity = params->cArgs == 1 ? params->rgvarg[0].uintVal : 100;
	SetWindowLong(GWL_EXSTYLE, GetWindowLong(GWL_EXSTYLE) | WS_EX_LAYERED);
	SetLayeredWindowAttributes(m_hWnd, 0, opacity * 255 / 100, LWA_ALPHA);
}

//------------------------------------ window message handlers -------------------------------------

int HtmlControl::MessageLoop()
{
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (WM_SYSKEYDOWN == msg.message
			&& VK_F4 == msg.wParam)
			continue;

		TranslateMessage(&msg);
		if (msg.message == WM_CLOSE)
			break;
		if (msg.message == WM_KEYDOWN && msg.message >= WM_KEYFIRST && msg.message <= WM_KEYLAST)
			SendMessage(m_hWnd, msg.message, msg.wParam, msg.lParam); // resend msgs to web ctrl
		if (msg.message == WM_LBUTTONDOWN)
		{
			SendMessage(m_hWnd, msg.message, msg.wParam, msg.lParam);
		}
		DispatchMessage(&msg);
	}
	return (int)msg.wParam;
}

LRESULT HtmlControl::OnCreate(UINT, WPARAM, LPARAM, BOOL&)
{
	// create control
	RECT rect;
	GetClientRect(&rect);
	wndIE.Create(m_hWnd, rect, _T(""), WS_CHILD | WS_VISIBLE, 0);
	wndIE.SetExternalDispatch(this);
	// navigate control to resource path (we need full path name here)
	TCHAR module[MAX_PATH];
	GetModuleFileName(_AtlBaseModule.GetModuleInstance(), module, _countof(module));
	CComBSTR bstrURL(L"");
	std::wstring str(module);
	if (std::wstring::npos != str.find_last_of(L"\\"))
	{
		str = str.substr(0, str.find_last_of(L"\\"));
	}

	bstrURL.Append(str.c_str()); 
	if (g_nInstallType == 0)
	{
		bstrURL.Append(MAINWND_UNINSTALL);
	}
	else
	{
		bstrURL.Append(MAINWND_INSTALL);
	}
	 
	wndIE.CreateControl(bstrURL);
	// connect web browser events
	HRESULT hr;
	DWORD cookie;
	IWebBrowser2* browser;
	wndIE.QueryControl(IID_IWebBrowser2, (void**)&browser);
	ATLASSERT(browser);
	hr = AtlAdvise(browser, this, __uuidof(DWebBrowserEvents2), &cookie);
	ATLASSERT(SUCCEEDED(hr));
	// don't allow url drag and drop (good for security)
	hr = browser->put_RegisterAsDropTarget(VARIANT_FALSE);
	ATLASSERT(SUCCEEDED(hr));
	browser->Release();
	// final luxury
	wndIE.SetFocus();
	RemoveFrame();
	return 0;
}

LRESULT HtmlControl::OnKeyDown(UINT message, WPARAM wParam, LPARAM lParam, BOOL&)
{
	// resend the keystrokes to web browser
	return 0;
	MSG msg;
	msg.message = message;
	msg.wParam = wParam;
	msg.lParam = lParam;
	IOleInPlaceActiveObject* ole;
	wndIE.QueryControl(IID_IOleInPlaceActiveObject, (void**)&ole);
	ATLASSERT(ole);
	ole->TranslateAccelerator(&msg);
	ole->Release();
	return 0;
}

LRESULT HtmlControl::OnSizing(UINT, WPARAM, LPARAM, BOOL&)
{
	FitToHost();
	return 0;
}

//发送安装进度给js
LRESULT HtmlControl::OnProgress(UINT message, WPARAM wParam, LPARAM lParam, BOOL&)
{
	VARIANT * pVariant = new VARIANT[2];
	VARIANT varRetVal;
	VariantInit(&varRetVal);
	if (m_callbackHandler)
	{
		pVariant[0].vt = VT_UINT;
		pVariant[0].uintVal = lParam;
		pVariant[1].vt = VT_BOOL;
		pVariant[1].uintVal = wParam;
		DISPPARAMS empty = { pVariant, NULL, 2, 0 };
		CallJS(m_callbackHandler, &empty, &varRetVal);
	}
	return 0;
}
//-------------------------------------- visual effects --------------------------------------------

void HtmlControl::RemoveFrame()
{
	return;
	// this is the only way I could find to remove that stupid frame
	RECT rect;
	GetClientRect(&rect);
	rect.left += CutSize;
	rect.top += CutSize;
	rect.right += CutSize;
	rect.bottom += CutSize;
	HRGN rgn = CreateRectRgn(rect.left, rect.top, rect.right, rect.bottom);
	SetWindowRgn(rgn, TRUE);
	DeleteObject(rgn);
}

void HtmlControl::FitToHost()
{
	RECT rect;
	GetClientRect(&rect);
	wndIE.MoveWindow(&rect);
	RemoveFrame();
}

//------------------------------------------- js calls ---------------------------------------------

void HtmlControl::ConnectHandler(BSTR name, CallbackFunction func, bool simple)
{
	ATLASSERT(jsHandlers.find(name) == jsHandlers.end());
	JsCallHandler handler;
	handler.id = BASE_DIPSPID + (DISPID)jsHandlers.size(); // generate a unique id
	handler.callback.normal = func;
	handler.simple = simple;
	jsHandlers[name] = handler;
}

DISPID HtmlControl::NameId(BSTR name) const
{
	JsHandlers::const_iterator it;
	it = jsHandlers.find(name);
	//ATLASSERT(it != jsHandlers.end());
	if (it != jsHandlers.end())
	{
		return it->second.id;
	}
	else
	{
		return 100003;
	}
	
}

HRESULT HtmlControl::Invoke(DISPID id, DISPPARAMS* params, VARIANT* retval)
{
	JsHandlers::iterator it;
	for (it = jsHandlers.begin(); it->second.id != id && it != jsHandlers.end(); it++);
	if (it == jsHandlers.end()) return E_NOTIMPL;
	if (!it->second.simple)
	{
		(this->*it->second.callback.normal)(params, retval);
	}
	else
	{
		int count = params->cArgs;
		if (count > 256) return E_INVALIDARG; // no more than 256 args
		// convert args
		const char* argv[256] = { 0 };
		count--;
		for (int i = 0; count >= 0; i++, count--)
			argv[i] = InPlaceWToA(params->rgvarg[count].bstrVal);
		const char* retstr = (this->*it->second.callback.simple)(params->cArgs, argv);
		if (retval)
		{
			CComVariant res(static_cast<LPSTR>((LPSTR)retstr));
			res.Detach(retval);
		}
	}
	return S_OK;
}

char* HtmlControl::InPlaceWToA(BSTR bstr) const
{	// convert a BSTR to char* in place
	if (bstr[0] > 0xFF) return (char*)bstr;
	WCHAR* wp = bstr;
	char* p = (char*)bstr;
	while (*(p++) = (char)*(wp++));
	return (char*)bstr;
}

LRESULT HtmlControl::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	g_nCurProgress += 3;
	if (g_nCurProgress >= g_arrProgress[g_nProgressIndex])
	{
		g_nCurProgress = g_arrProgress[g_nProgressIndex] - 1;
	}
	VARIANT * pVariant = new VARIANT[2];
	VARIANT varRetVal;
	VariantInit(&varRetVal);
	if (m_callbackHandler)
	{
		pVariant[0].vt = VT_UINT;
		pVariant[0].uintVal = g_nCurProgress;
		pVariant[1].vt = VT_BOOL;
		pVariant[1].uintVal = TRUE;
		DISPPARAMS empty = { pVariant, NULL, 2, 0 };
		CallJS(m_callbackHandler, &empty, &varRetVal);
	}
	return 0;
}

void HtmlControl::DeleteMeetingSetupExe()
{
	TCHAR exePath[MAX_PATH] = { 0 };
	_tcscpy_s(exePath, m_szInstallDir);
	_tcscat_s(exePath, UCCMEETING_SETUP_PATH);
	DeleteFile(exePath);
	UC_LOG(INFO) << "DeleteMeetingSetupExe enter. exe path = " << exePath;
}

LRESULT HtmlControl::OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL&)
{
	POINT pt;
	pt.x = LOWORD(lParam);
	pt.y = HIWORD(lParam);
	// 判断当前鼠标是否点在伪标题栏内，是，则发送消息
	if (m_rTitle.PtInRect(pt))
	{
		HWND hWndSub = ::GetActiveWindow();
		if (hWndSub == m_hWnd)
		{
			ReleaseCapture();
			SetCursor(LoadCursor(NULL,IDC_ARROW));
			::PostMessage(m_hWnd, WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(pt.x, pt.y));
		}
	}

	return 0;
}

