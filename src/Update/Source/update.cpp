#include "stdafx.h"
#include <exdisp.h>
#include <comdef.h>
#include "ControlEx.h"
#include "UpdateManagerImpl.h"
#include "LogManager.h"
#include "common/StringConvert.h"

#include <ShellAPI.h>

using namespace uc;

class CUpgradeFrameWnd : public CWindowWnd, public INotifyUI
{
public:
	CUpgradeFrameWnd() { };
	LPCTSTR GetWindowClassName() const { return _T("UIMainFrame"); };
	UINT GetClassStyle() const { return CS_DBLCLKS; };
	void OnFinalMessage(HWND /*hWnd*/) { delete this; };

	void Init() {
		m_pCloseBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("closebtn")));
		m_pMaxBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("maxbtn")));
		m_pRestoreBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("restorebtn")));
		m_pMinBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("minbtn")));
		m_pProgress = static_cast<CProgressUI*>(m_pm.FindControl(_T("updateProgress")));
		m_percent = static_cast<CTextUI*>(m_pm.FindControl(_T("percentText")));
		m_prompt = static_cast<CTextUI*>(m_pm.FindControl(_T("promptText")));
		m_updateResult = static_cast<CTextUI*>(m_pm.FindControl(_T("updateResultText")));
		m_updateResult->SetVisible(false);
		m_avatar = static_cast<CTextUI*>(m_pm.FindControl(_T("avatarText")));
		m_featuresText = static_cast<CTextUI*>(m_pm.FindControl(_T("featuresText")));
		m_cancelBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("cancelBtn")));
		m_cancelBtn->SetVisible(false);
		m_tryBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("tryBtn")));
		m_tryBtn->SetVisible(false);
		m_manualUpdate = static_cast<CButtonUI*>(m_pm.FindControl(_T("manualBtn")));
		m_manualUpdate->SetVisible(false);
		m_avatar->SetVisible(false);
		m_pCloseBtn->SetEnabled(false);
	}

	void OnPrepare() {
	}

	void SetProgress(int value) {
		m_pProgress->SetValue(value);
		CString percent;
		percent.Format(_T("%d%%"), value);
		m_percent->SetText(percent);

		if (value == 100) {
			m_featuresText->SetVisible(false);
			m_percent->SetVisible(false);
			m_prompt->SetVisible(false);
			m_pProgress->SetVisible(false);
			m_avatar->SetVisible(true);
			m_pCloseBtn->SetEnabled(true);
			m_updateResult->SetVisible(true);
			m_updateResult->SetBkImage(CPaintManagerUI::GetInstancePath() + _T("skin/success.png"));
		}
	}

	void SetErrorText(int type, int err) {
		m_featuresText->SetVisible(false);
		m_percent->SetVisible(false);
		m_prompt->SetVisible(false);
		m_pProgress->SetVisible(false);
		m_avatar->SetVisible(true);
		m_cancelBtn->SetVisible(true);
		m_tryBtn->SetVisible(true);
		m_manualUpdate->SetVisible(true);
		m_pCloseBtn->SetEnabled(true);
		m_updateResult->SetVisible(true);
		m_updateResult->SetBkImage(CPaintManagerUI::GetInstancePath() + _T("skin/failed.png"));
	}

	void ManualInstall(const std::wstring& installPath) {
		HINSTANCE result = ShellExecute(NULL, L"open", installPath.c_str(), NULL, NULL, SW_SHOWNORMAL);
		if ((long)result < 32) {
			UC_LOG(ERROR) << "ShellExecute default error:" << ::GetLastError() << ", result:" << (long)result;
			result = ShellExecute(NULL, L"open", L"iexplore.exe", installPath.c_str(), NULL, SW_SHOWNORMAL);
		}
		UC_LOG(INFO) << "ManualInstall, path:" << installPath << ", error:" << ::GetLastError() << ", result:" << (long)result;
		CloseHandle(result);
		PostQuitMessage(0);
	}

	void Notify(TNotifyUI& msg)
	{
		if( msg.sType == _T("windowinit") ) OnPrepare();
		else if( msg.sType == _T("click") ) {
			if( msg.pSender == m_pCloseBtn ) {
				PostQuitMessage(0);
				return; 
			}
			else if( msg.pSender == m_pMinBtn ) { 
				SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0); return; }
			else if( msg.pSender == m_pMaxBtn ) { 
				SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0); return; }
			else if( msg.pSender == m_pRestoreBtn ) { 
				SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0); return; }
			else if ( msg.pSender == m_cancelBtn ) {
				PostQuitMessage(0); return; }
			else if (msg.pSender == m_tryBtn) {
				m_avatar->SetVisible(false);
				m_cancelBtn->SetVisible(false);
				m_tryBtn->SetVisible(false);
				m_manualUpdate->SetVisible(false);
				m_pCloseBtn->SetEnabled(false);
				m_updateResult->SetVisible(false);
				m_featuresText->SetVisible(true);
				m_percent->SetVisible(true);
				m_prompt->SetVisible(true);
				m_pProgress->SetVisible(true);
				UpdateManagerImpl::GetInstance().Upgrade(m_serverCfg._appId, 
					m_serverCfg._siteId, m_serverCfg._skinId, (_bstr_t)m_serverCfg._upgradeServer.c_str(), m_serverCfg._port, m_serverCfg._timeOut, L"");
				return;
			}
			else if (msg.pSender == m_manualUpdate ) {
				ManualInstall(m_serverCfg._installPath);
				return;
			}
			else {}
		}
		else if(msg.sType==_T("selectchanged"))
		{
			CDuiString name = msg.pSender->GetName();
			CTabLayoutUI* pControl = static_cast<CTabLayoutUI*>(m_pm.FindControl(_T("switch")));
			if(name==_T("examine"))
				 pControl->SelectItem(0);
			else if(name==_T("trojan"))
				 pControl->SelectItem(1);
			else if(name==_T("plugins"))
				pControl->SelectItem(2);
			else if(name==_T("vulnerability"))
				pControl->SelectItem(3);
			else if(name==_T("rubbish"))
				pControl->SelectItem(4);
			else if(name==_T("cleanup"))
				pControl->SelectItem(5);
			else if(name==_T("fix"))
				pControl->SelectItem(6);
			else if(name==_T("tool"))
				pControl->SelectItem(7);
		}
	}

	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
		styleValue &= ~WS_CAPTION;
		::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

		m_pm.Init(m_hWnd);
		CDialogBuilder builder;
		CDialogBuilderCallbackEx cb;
		CControlUI* pRoot = builder.Create(_T("skin.xml"), (UINT)0,  &cb, &m_pm);
		ASSERT(pRoot && "Failed to parse XML");
		m_pm.AttachDialog(pRoot);
		m_pm.AddNotifier(this);

		Init();

		m_percent->SetText(L"%0");

		return 0;
	}

	LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		bHandled = FALSE;
		return 0;
	}

	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		::PostQuitMessage(0L);

		bHandled = FALSE;
		return 0;
	}

	LRESULT OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
        if( ::IsIconic(*this) ) bHandled = FALSE;
        return (wParam == 0) ? TRUE : FALSE;
	}

	LRESULT OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		return 0;
	}

	LRESULT OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		return 0;
	}

	LRESULT OnNcLButtonDBLCLK(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		return 0;
	}

	LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		POINT pt; pt.x = GET_X_LPARAM(lParam); pt.y = GET_Y_LPARAM(lParam);
		::ScreenToClient(*this, &pt);

		RECT rcClient;
		::GetClientRect(*this, &rcClient);

		RECT rcCaption = m_pm.GetCaptionRect();
		if( pt.x >= rcClient.left + rcCaption.left && pt.x < rcClient.right - rcCaption.right \
			&& pt.y >= rcCaption.top && pt.y < rcCaption.bottom ) {
				CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(pt));
				if( pControl && _tcscmp(pControl->GetClass(), _T("ButtonUI")) != 0 && 
					_tcscmp(pControl->GetClass(), _T("OptionUI")) != 0 &&
					_tcscmp(pControl->GetClass(), _T("TextUI")) != 0 )
					return HTCAPTION;
		}

		return HTCLIENT;
	}

	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
        SIZE szRoundCorner = m_pm.GetRoundCorner();
        if( !::IsIconic(*this) && (szRoundCorner.cx != 0 || szRoundCorner.cy != 0) ) {
            CDuiRect rcWnd;
            ::GetWindowRect(*this, &rcWnd);
            rcWnd.Offset(-rcWnd.left, -rcWnd.top);
            rcWnd.right++; rcWnd.bottom++;
            HRGN hRgn = ::CreateRoundRectRgn(rcWnd.left, rcWnd.top, rcWnd.right, rcWnd.bottom, szRoundCorner.cx, szRoundCorner.cy);
            ::SetWindowRgn(*this, hRgn, TRUE);
            ::DeleteObject(hRgn);
        }

        bHandled = FALSE;
        return 0;
	}

	LRESULT OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		MONITORINFO oMonitor = {};
		oMonitor.cbSize = sizeof(oMonitor);
		::GetMonitorInfo(::MonitorFromWindow(*this, MONITOR_DEFAULTTOPRIMARY), &oMonitor);
		CDuiRect rcWork = oMonitor.rcWork;
		rcWork.Offset(-oMonitor.rcMonitor.left, -oMonitor.rcMonitor.top);

		LPMINMAXINFO lpMMI = (LPMINMAXINFO) lParam;
		lpMMI->ptMaxPosition.x	= rcWork.left;
		lpMMI->ptMaxPosition.y	= rcWork.top;
		lpMMI->ptMaxSize.x		= rcWork.right;
		lpMMI->ptMaxSize.y		= rcWork.bottom;

		bHandled = FALSE;
		return 0;
	}

	LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		// 有时会在收到WM_NCDESTROY后收到wParam为SC_CLOSE的WM_SYSCOMMAND
		if( wParam == SC_CLOSE ) {
			::PostQuitMessage(0L);
			bHandled = TRUE;
			return 0;
		}
		BOOL bZoomed = ::IsZoomed(*this);
		LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
		if( ::IsZoomed(*this) != bZoomed ) {
			if( !bZoomed ) {
				CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("maxbtn")));
				if( pControl ) pControl->SetVisible(false);
				pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("restorebtn")));
				if( pControl ) pControl->SetVisible(true);
			}
			else {
				CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("maxbtn")));
				if( pControl ) pControl->SetVisible(true);
				pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("restorebtn")));
				if( pControl ) pControl->SetVisible(false);
			}
		}
		return lRes;
	}

	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		LRESULT lRes = 0;
		BOOL bHandled = TRUE;
		switch( uMsg ) {
		case WM_CREATE:        lRes = OnCreate(uMsg, wParam, lParam, bHandled); break;
		case WM_CLOSE:         lRes = OnClose(uMsg, wParam, lParam, bHandled); break;
		case WM_DESTROY:       lRes = OnDestroy(uMsg, wParam, lParam, bHandled); break;
		case WM_NCACTIVATE:    lRes = OnNcActivate(uMsg, wParam, lParam, bHandled); break;
		case WM_NCCALCSIZE:    lRes = OnNcCalcSize(uMsg, wParam, lParam, bHandled); break;
		case WM_NCPAINT:       lRes = OnNcPaint(uMsg, wParam, lParam, bHandled); break;
		case WM_NCHITTEST:     lRes = OnNcHitTest(uMsg, wParam, lParam, bHandled); break;
		case WM_SIZE:          lRes = OnSize(uMsg, wParam, lParam, bHandled); break;
		case WM_GETMINMAXINFO: lRes = OnGetMinMaxInfo(uMsg, wParam, lParam, bHandled); break;
		case WM_SYSCOMMAND:    lRes = OnSysCommand(uMsg, wParam, lParam, bHandled); break;
		case WM_NCLBUTTONDBLCLK: lRes = OnNcLButtonDBLCLK(uMsg, wParam, lParam, bHandled); break;
		default:
		bHandled = FALSE;
		}
		if( bHandled ) return lRes;
		if( m_pm.MessageHandler(uMsg, wParam, lParam, lRes) ) return lRes;
		return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	}

	//升级进度
	static void _stdcall ProgressCallBack(void* userdata, int type, int value) {
		if (type == UPGRADE_DEFAULT){
			return;
		}

		CUpgradeFrameWnd* frame = (CUpgradeFrameWnd*)(userdata);
		frame->SetProgress(value);
	}

	//升级错误处理
	static void _stdcall ErrorCallBack(void* userdata, int type, int err, const std::wstring& version) {
		CUpgradeFrameWnd* frame = (CUpgradeFrameWnd*)(userdata);
		frame->SetErrorText(type, err);
	}

	//升级检测
	static void _stdcall CheckUpdateCallBack(void* userdata, int type, BOOL isNeedUpdate, const std::wstring& version, const std::wstring& notes) {
		//LOG ERROR
	}

	void SetServerCfg(const ServerCfg& cfg) { m_serverCfg = cfg; }

public:
	CPaintManagerUI m_pm;

private:
	CButtonUI* m_pCloseBtn;
	CButtonUI* m_pMaxBtn;
	CButtonUI* m_pRestoreBtn;
	CButtonUI* m_pMinBtn;
	CButtonUI* m_cancelBtn;
	CButtonUI* m_tryBtn;
	CButtonUI* m_manualUpdate;
	CProgressUI* m_pProgress;
	CTextUI* m_percent;
	CTextUI* m_prompt;
	CTextUI* m_updateResult;
	CTextUI* m_avatar;
	CTextUI* m_featuresText;

	ServerCfg m_serverCfg;
};

void SetUpdateUrl(LPSTR lpCmdLine, ServerCfg& cfg) {
	std::vector<std::string> serverInfo;
	StringConvert::Split(string(lpCmdLine), string(";"), serverInfo);
	if (serverInfo.size() != 10) {
		return;
	}
	cfg._upgradeServer = StringConvert::FromUTF8Str(serverInfo[0]);
	cfg._packageInfo = StringConvert::FromUTF8Str(serverInfo[1]);
	cfg._packageDownload = StringConvert::FromUTF8Str(serverInfo[2]);
	cfg._port = std::atoi(serverInfo[3].c_str());
	cfg._appId = std::atoi(serverInfo[4].c_str());
	cfg._siteId = std::atoi(serverInfo[5].c_str());
	cfg._skinId = std::atoi(serverInfo[6].c_str());
	cfg._timeOut = std::atoi(serverInfo[7].c_str());
	cfg._logPath = serverInfo[8];
	cfg._installPath = StringConvert::FromUTF8Str(serverInfo[9]);

	UpdateManagerImpl::GetInstance().SetUpdateUrl_Version((BSTR)cfg._upgradeServer.c_str());
	UpdateManagerImpl::GetInstance().SetUpdateUrl_PackageInfo((BSTR)cfg._packageInfo.c_str());
	UpdateManagerImpl::GetInstance().SetUpdateUrl_PackageDwon((BSTR)cfg._packageDownload.c_str());
	UpdateManagerImpl::GetInstance().SetLoginInfo(CString(lpCmdLine).AllocSysString());
}

string UnicodeConvertAscii(wstring wstrsrc)
{
	int nLength = ::WideCharToMultiByte(CP_OEMCP, 0, wstrsrc.c_str(), -1, NULL, 0, NULL, NULL);
	if (nLength <= 0) return string("");
	char *szbuffer = new char[nLength + 2];
	::WideCharToMultiByte(CP_OEMCP, 0, wstrsrc.c_str(), -1, szbuffer, nLength, NULL, NULL);
	string strnew = szbuffer;
	delete[] szbuffer;
	return strnew;
}


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	std::string processCmd;
	if (NULL == lpCmdLine || 0 == strlen(lpCmdLine)) {
		LPWSTR cmd = GetCommandLine();
		processCmd = uc::StringConvert::ToUTF8Str(cmd);
	}
	else {
		processCmd = lpCmdLine;
	}

	//parser cmdline
	ServerCfg cfg;
	SetUpdateUrl((LPSTR)processCmd.c_str(), cfg);
	
	CPaintManagerUI::SetInstance(hInstance);
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath() + _T("skin"));

	string path = cfg._logPath;
	// covert logPath to ascii
	std::wstring logpath = StringConvert::FromUTF8Str(path);
	std::string log = UnicodeConvertAscii(logpath);

	LogManager::GetInstance().Init(log, "update", "update.log");

	HRESULT Hr = ::CoInitialize(NULL);
	if( FAILED(Hr) ) return 0;

	CUpgradeFrameWnd* pFrame = new CUpgradeFrameWnd();
	if( pFrame == NULL ) return 0;
	pFrame->Create(NULL, _T("UC Upgrade"), UI_WNDSTYLE_FRAME, 0L, 0, 0, 800, 572);
	pFrame->CenterWindow();
	::ShowWindow(*pFrame, SW_SHOW);

	UC_LOG(WARNING) << "update WinMain processCmd:" << processCmd;
	UpdateManagerImpl::GetInstance().Init(pFrame, CUpgradeFrameWnd::CheckUpdateCallBack, CUpgradeFrameWnd::ProgressCallBack, CUpgradeFrameWnd::ErrorCallBack);
	pFrame->SetServerCfg(cfg);
	UpdateManagerImpl::GetInstance().Upgrade(cfg._appId, cfg._siteId, cfg._skinId, (_bstr_t)cfg._upgradeServer.c_str(), cfg._port, cfg._timeOut, L"");
	CPaintManagerUI::MessageLoop();

	::CoUninitialize();
	if (pFrame != NULL) {
		delete pFrame;
		pFrame = NULL;
	}
	LogManager::GetInstance().UnInit();
	return 0;
}