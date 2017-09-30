/******************************************************************************
* Copyright (c) 2009~2010 All Rights Reserved by
*  G-Net Integrated Service co. Ltd. 

******************************************************************************/
/**
* @file TangView.cpp
* @brief implmentation of the TangFrame class.
* 
* @author Hongwei Hu (hongwei.hu@gnetis.com)
*
* @date Janu. 26 2010
*
* @version 1.0.0
* 
* @warning 
*
* Revision History
* @if  CR/PR ID    Author          Date            Major Change @endif
* @bug CR0001      Hongwei Hu     Janu. 26 2010   create version 1.0.0\n

******************************************************************************/
#include "stdafx.h"
#include <strsafe.h>
#include <WtsApi32.h>
#include <tlhelp32.h>
#include "TangFrame.h"
#include "TestExe.h"
#include "Util.h"
#include <io.h>
#include <sys/stat.h>
#include <sys/types.h>

#pragma comment(lib, "WtsApi32.lib")

#define   BIF_NEWDIALOGSTYLE    0x0040 
#define INSTALLATION_SIZE       83

#define DEFAULT_INDEX_PAGE_PATH             L"\\index.html" /**< 默认加载页面*/
#define DEFAULT_UNLOAD_PAGE_PATH            L"\\unload.html" /**< 默认加载页面*/

//暂时不能释放的文件
//公用
#define NEED_FILE_CLOSE						L"\\close.png"
#define NEED_FILE_CLOSE_ACTIVE				L"\\close_active.png"
#define NEED_FILE_CLOSE_DISABLE				L"\\close_disabled.png"
#define NEED_FILE_CLOSE_HOVER				L"\\close_hover.png"
#define NEED_FILE_CLOSE_HOVER2				L"\\close-hover.png"
#define NEED_FILE_CLOSE_NORMAL				L"\\close_normal.png"
#define NEED_FILE_MINI						L"\\min.png"
#define NEED_FILE_MINI_ACTIVE				L"\\min_active.png"
#define NEED_FILE_MINI_DISABLE				L"\\min_disabled.png"
#define NEED_FILE_MINI_HOVER				L"\\min_hover.png"
#define NEED_FILE_MINI_NORMAL				L"\\min_normal.png"
//仅安装需要
#define INSTALL_NEED_FILE_SUCCESS_ACTIVE    L"\\btn-success-active.png"
#define INSTALL_NEED_FILE_SUCCESS_HOVER     L"\\btn-success-hover.png"
#define INSTALL_NEED_FILE_SUCCESS_NORMAL    L"\\btn-success-normal.png"
#define INSTALL_NEED_FILE_CONTINUE_ACTIVE   L"\\setup-continue-active.png"
#define INSTALL_NEED_FILE_CONTINUE_HOVER    L"\\setup-continue-hover.png"
#define INSTALL_NEED_FILE_CONTINUE_NORMAL   L"\\setup-continue-normal.png"
#define INSTALL_NEED_FILE_STOP_ACTIVE		L"\\setup-stop-active.png"
#define INSTALL_NEED_FILE_STOP_HOVER		L"\\setup-stop-hover.png"
#define INSTALL_NEED_FILE_STOP_NORMAL		L"\\setup-stop-normal.png"
#define INSTALL_NEED_FILE_PNG_1				L"\\1.png"
#define INSTALL_NEED_FILE_PNG_2				L"\\2.png"
#define INSTALL_NEED_FILE_PNG_3				L"\\3.png"
#define INSTALL_NEED_FILE_PNG_4				L"\\4.png"
#define INSTALL_NEED_FILE_PNG_5				L"\\5.png"

//仅卸载需要
#define UINSTALL_NEED_FILE_OK_ACTIVE		L"\\ok_active.png"
#define UINSTALL_NEED_FILE_OK_HOVER			L"\\ok_hover.png"
#define UINSTALL_NEED_FILE_OK_NORMAL		L"\\ok_normal.png"

#define UI_NEED_FILE_NO                 28			//安装时需要暂时不被释放的文件
TCHAR g_szUIFiles[UI_NEED_FILE_NO][MAX_PATH] = {
	NEED_FILE_CLOSE_ACTIVE,
	NEED_FILE_CLOSE_DISABLE,
	NEED_FILE_CLOSE_HOVER,
	NEED_FILE_CLOSE_NORMAL,
	NEED_FILE_MINI_ACTIVE,
	NEED_FILE_MINI_DISABLE,
	NEED_FILE_MINI_HOVER,
	NEED_FILE_MINI_NORMAL,
	INSTALL_NEED_FILE_SUCCESS_ACTIVE,
	INSTALL_NEED_FILE_SUCCESS_HOVER,
	INSTALL_NEED_FILE_SUCCESS_NORMAL,
	INSTALL_NEED_FILE_PNG_1,
	INSTALL_NEED_FILE_PNG_2,
	INSTALL_NEED_FILE_PNG_3,
	INSTALL_NEED_FILE_PNG_4,
	INSTALL_NEED_FILE_PNG_5,
	INSTALL_NEED_FILE_CONTINUE_ACTIVE,
	INSTALL_NEED_FILE_CONTINUE_HOVER,
	INSTALL_NEED_FILE_CONTINUE_NORMAL,
	INSTALL_NEED_FILE_STOP_ACTIVE,
	INSTALL_NEED_FILE_STOP_HOVER,
	INSTALL_NEED_FILE_STOP_NORMAL,
	UINSTALL_NEED_FILE_OK_ACTIVE,
	UINSTALL_NEED_FILE_OK_HOVER,
	UINSTALL_NEED_FILE_OK_NORMAL
};

HANDLE g_hUIFiles[UI_NEED_FILE_NO] = { NULL };

#define INSTALL_STATUS_ING      1
#define INSTALL_STATUS_ED       2

#define TIMER_ID_UPDATE_STATUS          1
#define TIMER_ID_INSTALLED              2
#define TIMER_ID_CHECK_INSTALL          3
#define TIMER_ID_INSTALLMEETING			4

#define TIMER_ID_UNLOAD_CONTROLPANNEL   10
#define PROCESS_UPDATE_TIME             500
#define PROCESS_CHECK_INSTALL           3000	//3检测一次。
#define PROCESS_CHECK_INSTALL_MEETING   2000

#define UCCLIENT_PROCESS_OLD        _T("UCClient.exe")
#define UCCHAT_PROCESS_OLD          _T("UCChat.exe")
#define UCSQLAGENT_PROCESS_OLD      _T("SqlAgent.exe")

#define UC_BEE_PROCESS				_T("bee.exe")
#define UC_BEE_WORKER_PROCESS       _T("bee.worker.exe")

#define MSI_PROCESS             _T("msiexec.exe")
#define NOTIFY_MSI_TO_EXIT      _T("UC_INSTALL_NOTIFY_MSI_TO_EXIT")
#define UCCMEETING_SETUP		_T("UCCMeetingSetup.exe")
#define UCCMEETING_SETUP_PATH	_T("\\UCCMeetingSetup.exe")
#define UCCMEETING_PROCESS		_T("TangClient.exe")
#define REG_INSTALLMEETING_FLAG_OLD	_T("Software\\UCInstallMeeting")
#define REG_INSTALLMEETING_FLAG	_T("Software\\BeeInstallMeeting")
#define UCCMEETING_UNINSTALL	_T("\\Quanshi\\Quanshi Meeting\\6\\60000\\uninst.exe")
#define UCCMEETING_TANGROUTE	_T("\\Quanshi\\Quanshi Meeting\\TangRoute.exe")

#define MAX_POST_INT_TIMES      5
#define MAX_MSIEXEC_COPY        2
#define MAX_INDEX               4
int g_nIndex = -1;
int g_checkTimes = 0;
exception_type g_type = TYPE_DEFAULT;
int g_nProcess[MAX_POST_INT_TIMES] = {10, 20, 40, 60, 100};
BOOL g_bInstallMeeting = FALSE;

#define TARGET_NAME         _T("\\bee.exe")
#define TARGET_LNK          _T("\\蜜蜂.lnk")
#define TARGET_QUICK_START_LNK _T("\\Microsoft\\Internet Explorer\\Quick Launch\\蜜蜂.lnk")
#define TARGET_QUICK_START_PINNED_LNK _T("\\Microsoft\\Internet Explorer\\Quick Launch\\User Pinned\\StartMenu\\蜜蜂.lnk")
#define TARGET_LNK_OLD      _T("\\蜜蜂.lnk")
#define SETUP_NAME          _T("\\setup.exe")
#define SETUP_LNK_OLD           _T("\\卸载蜜蜂.lnk")
#define SETUP_LNK_NEW           _T("\\卸载蜜蜂.lnk")
#define START_PROGRAM_UC    _T("\\蜜蜂")
#define START_PROGRAM_UC_OLD    _T("\\蜜蜂")
#define REG_AUTO_RUN_UC_OLD     _T("UCClient")
#define REG_AUTO_RUN_UC     _T("Bee")
#define START_PROGRAM_UI    _T("\\setup")
//#define INSTALL_DIR_TEMP    _T("\\OnCloud")
#define INSTALL_DIR_TEMP    _T("\\Bee")
#define INSTALL_DIR_TEMP2	_T("\\Quanshi\\Bee")
#define UCUPDATE_LOG_CONFIG	_T("\\Update\\LogConfig.xml")
#define IS_INSTALL_MEETING	1
#define SETUP_WINDOW_TITLE L"QuanshiBee"
#define CLIENT_WINDOW_TITLE L"蜜蜂"
extern LPTSTR lpstrCmdLine;
extern BOOL g_bFromControlPannel;

TangFrame::TangFrame()
: NativeActionDispImpl<ITangWnd>()
, m_bShadowLess( FALSE )
, m_bModalDisplay( FALSE )
, m_bBlockMsg( FALSE )
, m_uModalDialogNeedMsg( 0 )
, m_uTimer( 0 )
, m_uMinWinHeight( 35 )
, m_bAutoKill( FALSE )
, m_bAdsorption( FALSE )
, m_bAllowReSize( FALSE )
, m_uElapse( 60*1000 )
, m_pUtil( NULL )
, m_strDefaultPage( NULL )
, m_bMaximized( FALSE )
, m_strMinHandler( NULL )
, m_strMaxHandler( NULL )
, m_strRestoreHandler( NULL )
, m_strCloseHandler( NULL )
, m_strLostFocusHanlder( NULL )
, m_strWindowMoveHandler( NULL )
, m_strSizeChangedHandler( NULL )
, m_xVal(0)
, m_yVal(0)
, m_uTimer_Adsorption( 2 )
, m_uElapse_Adsorption( 2*1000 )
, m_bEnable(FALSE)
, m_bMobileDirection( FALSE )
, m_uWidth(400)
, m_uHeight(300)
, m_strCallBackHandler( NULL )
, m_bInstallorUnload(TRUE)
//, m_bFromControlPannel(FALSE)
, m_nInstalled(0)
, m_type(TYPE_DEFAULT)
, m_bRecoverInstall(FALSE)
, m_bAutoRun(TRUE)
, m_bDeskLnk(TRUE)
, m_bQuickStartLnk(TRUE)
, m_bAlreadHold(FALSE)
{
    m_pUtil = new UtilAdaptor();
    memset(m_installDir, 0, MAX_PATH);
    memset(m_installPath, 0, MAX_PATH);
    TCHAR   szPath[MAX_PATH] = {0};
    SHGetSpecialFolderPath(NULL,szPath,CSIDL_PROFILE,FALSE);
	wsprintf(m_installDir, _T("%s\\Quanshi%s"), szPath, INSTALL_DIR_TEMP);
    wsprintf(m_installPath, _T("%s%s"), m_installDir, TARGET_NAME);
    DisableSelfDelete();

	OutputDebugString(_T("+++++TangFrame init."));
	OutputDebugString(m_installDir);
	m_ucorbeeinstalled = 0;
    //Util::MockLButtonDown(_T("Windows Installer"), _T("确定"));

    //Util::DeleteLnkFromStartMenu(START_PROGRAM_UC);
    //Util::DeleteLnkFromDesktop(TARGET_LNK);
}

TangFrame::~TangFrame()
{
    //recycle url string memory.
    SAFE_DELETE_ARRAY(m_strDefaultPage);

    //recycle ui message handler alloc memory.
    SYSFREESTRING(m_strMinHandler);
    SYSFREESTRING(m_strMaxHandler);
    SYSFREESTRING(m_strCloseHandler);
    SYSFREESTRING(m_strRestoreHandler);
    SYSFREESTRING(m_strLostFocusHanlder);
    SYSFREESTRING(m_strWindowMoveHandler);
    SYSFREESTRING(m_strSizeChangedHandler);

    SAFE_DELETE( m_pUtil );
}

BOOL TangFrame::PreTranslateMessage(MSG* pMsg)
{
    if(CFrameWindowImpl<TangFrame>::PreTranslateMessage(pMsg))
        return TRUE;

    CRect rect;
    ::GetClientRect(m_hWnd,&rect);
    int xPos = GET_X_LPARAM(pMsg->lParam); 
    int yPos = GET_Y_LPARAM(pMsg->lParam);
    POINT pt;
    pt.x = xPos;
    pt.y = yPos;
    HWND hWndSub = ::GetActiveWindow();

    static HCURSOR shArrayCursor    = NULL; // 默认鼠标
    static HCURSOR shNSCursor       = NULL; // 上下箭头鼠标
    static HCURSOR shAllCursor      = NULL; // 上下左右箭头鼠标
    static HCURSOR shCurrentCursor  = NULL;

    if(!shArrayCursor)
    {
        shArrayCursor = GetCursor();
    }

    if(WM_MOUSEMOVE == pMsg->message)
    {
//        shCurrentCursor = GetCursor();//获取当前鼠标
        //在标题栏内设置鼠标样式为十字箭头
//         if(m_rTitle.PtInRect(pt))
//         {
//             if ((hWndSub == m_hWnd) )
//             {
//                 shAllCursor = LoadCursor(NULL,IDC_ARROW);
//                 SetCursor(shAllCursor);
//             }
//         }
//         else
//         {
//             if( (shCurrentCursor != shNSCursor) && (shCurrentCursor != shAllCursor) )
//             {
//                 //外部设置的鼠标
//                 SetCursor(shCurrentCursor);
//             }
//             else
//             {
//                 SetCursor(shArrayCursor);
//             }
//         }

//         if ( m_bAllowReSize )
//         {
//             if(hWndSub == m_hWnd)
//             {
//                 if(yPos >= rect.bottom - 5)
//                 {
//                     shNSCursor = LoadCursor(NULL,IDC_SIZENS);
//                     SetCursor(shNSCursor);
//                     if (MK_LBUTTON & pMsg->wParam)
//                     {
//                         ::SendMessage(m_hWnd, WM_SYSCOMMAND, SC_SIZE | WMSZ_BOTTOM, pMsg->lParam);
//                     }
//                 }
//             }
//         }
    }

    if (WM_LBUTTONDOWN == pMsg->message)
    {
        POINT pt;
        pt.x = LOWORD(pMsg->lParam); 
        pt.y = HIWORD(pMsg->lParam); 
        // 判断当前鼠标是否点在伪标题栏内，是，则发送消息
        if(m_rTitle.PtInRect(pt))
        {
            HWND hWndSub = ::GetActiveWindow();
            if (hWndSub == m_hWnd)
            {
                ReleaseCapture();
                //SetCursor(LoadCursor(NULL,IDC_ARROW));
                ::PostMessage (m_hWnd, WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM (pt.x, pt.y));
            }
        }
    } 
    return m_view.PreTranslateMessage(pMsg);
}

BOOL TangFrame::OnIdle()
{
    UIUpdateToolBar();
    return FALSE;
}

LRESULT TangFrame::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    HRESULT hr = S_OK;
    //remove the system menu.
    SetMenu(NULL);

    //set external to view.
    m_view.SetExternal(&m_external);

    //create tang view .
	m_hWndClient = m_view.Create(m_hWnd, rcDefault, TEXT("about:blank"),
        WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, WS_EX_CLIENTEDGE);

    hr = m_external.PutPluginObject(ADAPTOR_NAME_MAINWND,(IDispatch*)this);

    //to disable to drag file and directory.
    ::DragAcceptFiles(m_hWnd,FALSE);
    ::DragAcceptFiles(m_hWndClient,FALSE);

    ModifyStyle(WS_VSCROLL|WS_HSCROLL|WS_MINIMIZE|WS_TILEDWINDOW,WS_SYSMENU|WS_MINIMIZEBOX);
    ::SetClassLong(m_hWnd,GCL_STYLE,::GetClassLong(m_hWnd, GCL_STYLE)|CS_DROPSHADOW);

    m_view.ModifyStyleEx(WS_EX_CLIENTEDGE|WS_EX_RIGHTSCROLLBAR,0,SWP_NOSIZE|SWP_NOMOVE|SWP_NOZORDER|SWP_NOACTIVATE );
    m_view.SetWindowPos(NULL,0,0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), SWP_NOZORDER|SWP_NOACTIVATE);

    //register shadow less window class.
    RegisterShadowlessWndClass();

    // register object for message filtering and idle updates
    CMessageLoop* pLoop = _Module.GetMessageLoop();
    ATLASSERT(pLoop != NULL);
    pLoop->AddMessageFilter(this);
    pLoop->AddIdleHandler(this);

    ::WTSRegisterSessionNotification( m_hWnd,NOTIFY_FOR_THIS_SESSION );

	::SetWindowText(m_hWnd, SETUP_WINDOW_TITLE);

    //设置拖动时显示窗口内容选项
    SystemParametersInfo(SPI_SETDRAGFULLWINDOWS, TRUE, NULL, NULL);

    //m_view.Navigate(DEFAULT_INDEX_PAGE_PATH, TRUE);

    return 0;
}

LRESULT TangFrame::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
    bHandled = FALSE;

    //clear shadow of dialog.
    if (m_bShadowLess)
    {
        ::SetClassLong(m_hWnd,GCL_STYLE,::GetClassLong(m_hWnd, GCL_STYLE)^CS_DROPSHADOW);
    }

    // unregister message filtering and idle updates
    CMessageLoop* pLoop = _Module.GetMessageLoop();
    ATLASSERT(pLoop != NULL);
    pLoop->RemoveMessageFilter(this);
    pLoop->RemoveIdleHandler(this);

    m_external.RemovePluginObject(ADAPTOR_NAME_MAINWND);

    WTSUnRegisterSessionNotification(m_hWnd);

    return 1;
}

#define NTDDI_VERSION NTDDI_LONGHORN
#define WIN32_LEAN_AND_MEAN
#define IMPLEMENT_VISTA_TOOLS
#include "VistaTools.cxx"
HRESULT TangFrame::RunProgram()
{
    CString cs;
    cs.Format(_T("UCSetup:RunProgram:run Program:%s"), m_installPath);
    LOG_INFO(cs);

    if (Util::JudgeOSVersion() <= 5)
    {
        ShellExecute(NULL, NULL, m_installPath, NULL, NULL, SW_SHOWNORMAL);
    }
    else
    {
		//降权和提权操作 http://blog.csdn.net/wsyjz/article/details/7080197
        RunAsStdUser( NULL, m_installPath );
    }

    return HResult_To_App_Layer( S_OK );
}

void TangFrame::PostDataToJS(BOOL bSuccess, int nValue)
{
	TCHAR szLog[MAX_PATH] = {0};
	wsprintf(szLog, _T("Setup.exe::PostDataToJS, bSuccess = %d, nValue = %d"), bSuccess, nValue);
	OutputDebugString(szLog);

	CString sLog;
	sLog.Format(L"Setup.exe::PostDataToJS, bSuccess = %d, nValue = %d.", bSuccess, nValue);
	LOG_INFO(sLog);

    VARIANT * pVariant = new VARIANT[2];
    VARIANT varRetVal;
    VariantInit(&varRetVal);
    if ( m_strCallBackHandler )
    {
        pVariant[0].vt = VT_UINT;
        pVariant[0].uintVal = nValue;
        pVariant[1].vt = VT_BOOL;
        pVariant[1].uintVal = bSuccess;
        fireEvent(m_strCallBackHandler, pVariant, 2, &varRetVal);
    }
}


void TangFrame::StartMsi(LPTSTR szFileName, LPTSTR szParameters)
{
	SetLastError(0);
    HINSTANCE hIns = ShellExecute(NULL, _T("open"), szFileName,
        szParameters, NULL, SW_HIDE);
	
// 	//先以隐藏窗口的模式拉起cmd，再在cmd中执行静默启动msi的命令
// 	TCHAR sParam[MAX_PATH] = { 0 };
// 	wsprintf(sParam, _T("/C \"\"%s\" %s\""), szFileName, szParameters);
// 	HINSTANCE hIns = ShellExecute(NULL, _T("open"), _T("cmd.exe"), sParam,
// 		NULL, SW_HIDE);

	int iRet = (int)hIns;
	TCHAR sMsg[MAX_PATH] = {0};
	wsprintf(sMsg, _T("StartMsi::ShellExecute return %d, GetLastError = %d"), iRet, GetLastError());
	OutputDebugString(sMsg);
	LOG_INFO(sMsg);

	wsprintf(sMsg, _T("StarMSI:: SetTimer:: TIMER_ID_INSTALL"));
	OutputDebugString(sMsg);

	KillAllTimer();
	g_nIndex = -1;
	g_checkTimes = 0;
    SetTimer(TIMER_ID_UPDATE_STATUS, PROCESS_UPDATE_TIME);
    SetTimer(TIMER_ID_CHECK_INSTALL, PROCESS_CHECK_INSTALL);
}

int TangFrame::CheckIsInstalled()
{
	HKEY hAppKey = NULL;
	TCHAR *bsKey = {_T("Software\\Bee")};

	TCHAR* productKey = new TCHAR[MAX_PATH];
	m_ucorbeeinstalled = Util::GetProductCode(&productKey);
	return m_ucorbeeinstalled;
// 	else//为了兼容旧版本
// 	{
// 		TCHAR *bsKeyPath = {_T("Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\{37255CD4-BFF5-4192-AADF-1289427C36B5}")};
// 
// 		if (RegOpenKeyEx(HKEY_LOCAL_MACHINE,
// 			bsKeyPath,
// 			0,
// 			KEY_READ,
// 			&hAppKey) == ERROR_SUCCESS)
// 		{
// 			RegCloseKey(hAppKey);
// 			return TRUE;
// 		}
// 	}

	//return FALSE;
}

//安装/卸载过程是否完成
BOOL TangFrame::CheckInstallEnd()
{
	HKEY hAppKey = NULL;
	TCHAR reg[MAX_PATH] = {0};
	TCHAR reg2[MAX_PATH] = {0};
	wsprintf(reg, _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\%s"), m_sProductCode);
	wsprintf(reg2, _T("SOFTWARE\\Wow6432Node\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\%s"), m_sProductCode);
	TCHAR *bsKeyPath = {reg};
	TCHAR *bsKeyPath2 = {reg2};
	LOG_INFO(bsKeyPath);
	LOG_INFO(bsKeyPath2);

	BOOL bFind = FALSE;
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE,
		bsKeyPath,
		0,
		KEY_READ,
		&hAppKey) == ERROR_SUCCESS)
	{
		RegCloseKey(hAppKey);
		bFind = TRUE;
	}

	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE,
		bsKeyPath2,
		0,
		KEY_READ,
		&hAppKey) == ERROR_SUCCESS)
	{
		RegCloseKey(hAppKey);
		bFind = TRUE;
	}

	TCHAR sMsg[MAX_PATH] = {0};
	wsprintf(sMsg, _T("CheckInstallEnd return = %d"), bFind);
	LOG_INFO(sMsg);
	return bFind;
}

//安装/卸载过程是否完成
BOOL TangFrame::CheckInstallEnd_NA()
{
	LOG_INFO(L"CheckInstallEnd_NA enter!!!");
	HKEY hAppKey = NULL;
	TCHAR reg[MAX_PATH] = {0};
	wsprintf(reg, _T("Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\%s"), m_sProductCode);
	TCHAR *bsKeyPath = {reg};
	LOG_INFO(bsKeyPath);

	if (RegOpenKeyEx(HKEY_CURRENT_USER,
		bsKeyPath,
		0,
		KEY_READ,
		&hAppKey) == ERROR_SUCCESS)
	{
		RegCloseKey(hAppKey);
		LOG_INFO(L"CheckInstallEnd_NA return TRUE.");
		return TRUE;
	}
	LOG_INFO(L"CheckInstallEnd_NA return FALSE.");
	return FALSE;
}

//检测当前版本号是否高于已经安装版本号。
BOOL TangFrame::CheckVersion()
{
// 	TCHAR* sRegVersion = new TCHAR[MAX_PATH];
// 	BOOL bRet = Util::GetProductVersion( &sRegVersion);
// 	
// 	if(bRet == TRUE)
// 	{
// 		//已经安装版本高于当前版本
// 		if(_tcscmp(sRegVersion, m_sVersion) > 0)
// 		{
// 			delete [] sRegVersion;
// 			LOG_INFO(L"CheckVersion result is FALSE.");
// 			return FALSE;
// 		}
// 	}
// 
// 	delete [] sRegVersion;

	//其他情况均认为可以覆盖安装
	LOG_INFO(L"CheckVersion result is TRUE.");
	return TRUE;
}

//参数需要定一下，是C++来获取参数，还是由JS传
HRESULT TangFrame::AutoStartSoftware(BOOL bAutoStart)
{
    HKEY hAppKey = NULL;

    BSTR bsKeyPath = _T("Software\\Microsoft\\Windows\\CurrentVersion\\Run");

    BSTR bsKeyName = REG_AUTO_RUN_UC;
	BSTR bsOldKeyName = REG_AUTO_RUN_UC_OLD;
    DWORD dw;

    if (RegCreateKeyEx(HKEY_CURRENT_USER, //开机自启动只针对当前用户
        bsKeyPath, 
        0, 
        REG_NONE, 
        REG_OPTION_NON_VOLATILE, 
        KEY_WRITE|KEY_READ, 
        NULL, 
        &hAppKey, 
        &dw) == ERROR_SUCCESS) 
    {
        if (bAutoStart)
        {
            DWORD dwType = REG_SZ;
            ::RegSetValueEx(hAppKey, bsKeyName, 0, dwType, 
                (LPBYTE)m_installPath, _tcslen(m_installPath)*sizeof(TCHAR));
        }
        else
        {
			RegDeleteValue(hAppKey, bsOldKeyName);
            RegDeleteValue(hAppKey, bsKeyName);
        }
        RegCloseKey(hAppKey);
    }

    return S_OK;
}

STDMETHODIMP TangFrame::selectFolderDialog(BSTR strDialogTitle,BSTR *pstrDir)
{
	HRESULT hr = S_OK;
	BROWSEINFO bi;

	//check the arguments.
	//CHECK_IF(NULL == strDialogTitle, E_INVALIDARG);
	//CHECK_IF(NULL == pstrDir, E_INVALIDARG);

	//init the browser info struct.
	bi.hwndOwner = m_hWnd;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = NULL;
	bi.lpszTitle = strDialogTitle;
	//BIF_EDITBOX
	bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;
	bi.lpfn = NULL;
	bi.lParam = 0;
	bi.iImage = 0;
	LPITEMIDLIST pList = NULL;
	BOOL bExistFloder = FALSE;
	BOOL bNewCreate = FALSE;
	TCHAR szSelPath[MAX_PATH] = {0};
	ZeroMemory(szSelPath,MAX_PATH);

	if((pList = SHBrowseForFolder(&bi)) != NULL)
	{  
		SHGetPathFromIDList(pList,szSelPath);
		int nlen = _tcslen(szSelPath);
		//文件目录过深或文件夹名称过长
		if (nlen >100)
		{
			hr = E_FAIL;
			*pstrDir = ::SysAllocString(_T(""));
			goto exit;
		}

		if (_tcslen(szSelPath)<=3)
		{
			szSelPath[_tcslen(szSelPath) - 1] = _T('\0');
		}

		//判断是否有读写权限
		if (0 != _taccess(szSelPath, 06))
		{
			hr = E_ACCESSDENIED;
			*pstrDir = ::SysAllocString(_T(""));
			goto exit;
		}
		
		_tcscat(szSelPath, INSTALL_DIR_TEMP);
		bExistFloder = Util::FolderExist(szSelPath);

		if (!bExistFloder)
		{
			Util::CreateDir(szSelPath);
			bExistFloder = Util::FolderExist(szSelPath);
			bNewCreate = TRUE;
		}
		
		if (bExistFloder)
		{
			//判断是否有权限访问
			struct _stat statbuf;
			int ret = _wstat(szSelPath,&statbuf);
			if (ret != 0 || !(statbuf.st_mode & S_IREAD) || !(statbuf.st_mode & S_IWRITE)) {
				hr = E_ACCESSDENIED;
				*pstrDir = ::SysAllocString(_T(""));
				goto exit;
			}

			_tcscpy(m_installDir, szSelPath);
			wsprintf(m_installPath, _T("%s%s"), m_installDir, TARGET_NAME);
			*pstrDir = ::SysAllocString(m_installDir);
	
		}
		else 
		{
			hr = E_ACCESSDENIED;
			*pstrDir = ::SysAllocString(_T(""));
			goto exit;
		}
	}
	else
	{
		*pstrDir = ::SysAllocString(m_installDir);
	}

exit:
	if (FAILED(hr))
	{
		LOG_ERR(L"selectFolderDialog::Select Dir Failed!");
	}

	//删除自己创建的文件夹
	if (bExistFloder == TRUE && bNewCreate == TRUE)
	{
		Util::DeleteFolder(szSelPath);
	}
	return HResult_To_App_Layer(hr);
}

STDMETHODIMP TangFrame::CheckInstall(int *pbInstalledUCorBee)
{
 
   *pbInstalledUCorBee = CheckIsInstalled();


    return HResult_To_App_Layer(S_OK);
}

STDMETHODIMP TangFrame::CheckIsNewVersion( BOOL *pbNew )
{
	if (CheckVersion())
	{
		*pbNew = TRUE;
	}
	else
	{
		*pbNew = FALSE;
	}

	return HResult_To_App_Layer(S_OK);
}

//是否需要安装云会议，如果需要安装进度需要整合。
//需要安装云会议的条件：当前没有云会议在运行，且之前没有安装云会议，如果装了，则必须是由蜜蜂安装上去的。
STDMETHODIMP TangFrame::CheckNeedInstallMeeting(BOOL *pbNeedInstall)
{
	*pbNeedInstall = FALSE;
	//判断当前是否有会议端正在运行,如果正在运行，无法安装云会议。
	int nApp = FindRunningApp(UCCMEETING_PROCESS);
	if (nApp <= 0)
	{
		//是否已经安装了云会议
		if (CheckMeetingInstalled())
		{
			//是否由蜜蜂所装，如果是则需要卸载重装
			HKEY hAppKey = NULL;
			TCHAR *bsOldKeyPath = { REG_INSTALLMEETING_FLAG_OLD };
			TCHAR *bsKeyPath = { REG_INSTALLMEETING_FLAG };
			if ((RegOpenKeyEx(HKEY_CURRENT_USER,
				bsOldKeyPath,
				0,
				KEY_READ,
				&hAppKey) == ERROR_SUCCESS) 
			|| (RegOpenKeyEx(HKEY_CURRENT_USER,
				bsKeyPath,
				0,
				KEY_READ,
				&hAppKey) == ERROR_SUCCESS))
			{

				*pbNeedInstall = TRUE;
				RegCloseKey(hAppKey);
			}
		}
		else
		{
			*pbNeedInstall = TRUE;
		}
	}

	CString sMsg;
	sMsg.Format(_T("CheckNeedInstallMeeting return %d"), *pbNeedInstall);
	LOG_INFO(sMsg);

	return HResult_To_App_Layer(S_OK);
}

STDMETHODIMP TangFrame::CheckAppRunning(BOOL *pbRunning)
{
	if (m_ucorbeeinstalled == 1)//装了老蜜蜂
	{
		if (FindRunningApp(UCCLIENT_PROCESS_OLD) || FindRunningApp(UCCHAT_PROCESS_OLD) || FindRunningApp(UCSQLAGENT_PROCESS_OLD))
		{
			*pbRunning = TRUE;
		}
	}else if (m_ucorbeeinstalled == 2)		//装了bee
	{
		if (FindRunningApp(UC_BEE_PROCESS) || FindRunningApp(UC_BEE_WORKER_PROCESS) )
		{
			*pbRunning = TRUE;
		}
	}

	*pbRunning = FALSE;
    return HResult_To_App_Layer(S_OK);
}

int TangFrame::FindRunningApp(TCHAR* lpProcName)
{
    EnableDebugPrivilege();

    BOOL bRet = FALSE;
    PROCESSENTRY32 pe = {sizeof(pe)};
    int nRet;
    int nRunningCopy = 0;

    DWORD proid;
    HANDLE hprocess;

    //创建进程快照
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if (hSnapshot != INVALID_HANDLE_VALUE)	
    {
        //从进程快照中遍历进程
        nRet = Process32First(hSnapshot, &pe);

        if (nRet)	
        {	
            do 
            {
                //判断进程名称是否和我们要找的进程名称相同
                if(StrCmpIW(pe.szExeFile, lpProcName) == 0)	
                {
                    proid=pe.th32ProcessID;

                    hprocess=::OpenProcess(PROCESS_QUERY_INFORMATION,FALSE,proid);

                    if(hprocess!=NULL)
                    {
                        //LOG_INFO("hprocess!=NULL");
                        ::CloseHandle(hprocess);
                        ++nRunningCopy;
                    }
                    //LOG_INFO("Find CloseHandle");
                }

            } while (Process32Next(hSnapshot, &pe));    
        }
        CloseHandle(hSnapshot);
    }
    //LOG_INFO("End CloseRunningApp");
    return nRunningCopy;
}

STDMETHODIMP TangFrame::ReInstall(BSTR *pstrDir)
{
	OutputDebugString(_T("+++ReInstall enter!!!!"));
	OutputDebugString(m_installDir);
    HRESULT hr = S_OK;
    *pstrDir = ::SysAllocString(m_installDir);
exit:
    if (FAILED(hr))
    {
        //LOG_ERR(L"Main frame:open folder dialog with error: 0x%08X", hr);
    }
    return HResult_To_App_Layer(hr);
}

STDMETHODIMP TangFrame::Uninstall()
{
	m_bInstallorUnload = FALSE;
    HRESULT hr = S_OK;
	OutputDebugString(_T("Uninstall enter!!!!"));
	LOG_INFO(L"Uninstall enter!!!!");
    m_nInstalled = INSTALL_STATUS_ING;
       
    //重启msi安装包
	TCHAR* sProductCode = new TCHAR[MAX_PATH];
	int bRet = Util::GetProductCode(&sProductCode);
	if (bRet == 1)
	{
		OutputDebugString(_T("Uninstall::after GetProductCode return 1"));
		LOG_INFO(L"Uninstall::after GetProductCode return 1 -- uc.");
		CloseRunningAppbyMsg(CLIENT_WINDOW_TITLE);
		CloseRunningApp(UCCHAT_PROCESS_OLD);
		CloseRunningApp(UCCLIENT_PROCESS_OLD);
		CloseRunningApp(UCSQLAGENT_PROCESS_OLD);
	}
	else if (bRet == 2)
	{
		OutputDebugString(_T("Uninstall::after GetProductCode return 2"));
		LOG_INFO(L"Uninstall::after GetProductCode return 2 -- bee.");
		CloseRunningAppbyMsg(CLIENT_WINDOW_TITLE);
		CloseRunningApp(UC_BEE_PROCESS);
		CloseRunningApp(UC_BEE_WORKER_PROCESS);
	}
	else 
	{
		//未找到product code 
		LOG_INFO(L"Uninstall::after GetProductCode return 0 -- ERROR!!.");
		delete[] sProductCode;
		sProductCode = NULL;
		MessageBox(_T("您尚未安装此产品."));
		close();
		return S_OK;
	}

	TCHAR sMsg[MAX_PATH] = {0};
	wsprintf(sMsg, _T("Uninstall::GetProductCode Ret = %d"), bRet);
	LOG_INFO(sMsg);

	TCHAR szUninstallCmd[MAX_PATH] = {0};
	TCHAR MyDocument[MAX_PATH] = { 0 };
	bRet = ::SHGetSpecialFolderPath(NULL, MyDocument, CSIDL_PERSONAL, FALSE);
	if (bRet == TRUE && _tcslen(MyDocument) > 0)
	{
		wsprintf(szUninstallCmd, _T("/X %s /qn /l*v \"%s\\Quanshi Files\\Bee\\Log\\Install\\MsiUninstall.log\""), sProductCode, MyDocument);		
	}
	else
	{
		wsprintf(szUninstallCmd, _T("/X %s /qn"), sProductCode);
	}

	_tcscpy(m_sProductCode, sProductCode);
	delete[] sProductCode;

	CString sLog;
	sLog.Format(L"Uninstall cmd = %s", szUninstallCmd);
	LOG_INFO(sLog);
	StartMsi(_T("msiexec.exe"), szUninstallCmd);
			

	//卸载云会议
	if (IS_INSTALL_MEETING && m_bRecoverInstall != TRUE)
	{
		UninstallMeeting();
	}
exit:
    if (FAILED(hr))
    {
        //LOG_ERR(L"Main frame:open folder dialog with error: 0x%08X", hr);
    }
    return HResult_To_App_Layer(hr);
}

STDMETHODIMP TangFrame::setWinTitleRect(UINT px, UINT py, UINT x, UINT y)
{
    HRESULT hr = S_OK;
    //LOG_INFO(L"set TangFrame title rect:left_top_point(%d ,%d), width=%d, height=%d", px, py, x, y);
    CRect rc(px,py,px+x,py+y);
    m_rTitle = rc;
    return HResult_To_App_Layer( hr );
}

STDMETHODIMP TangFrame::show(int uFlags)
{
    ::ShowWindow(m_hWnd,uFlags);
    return HResult_To_App_Layer(S_OK);
}

STDMETHODIMP TangFrame::setTitle(BSTR title)
{
    ::SetWindowText(m_hWnd, title);
    return HResult_To_App_Layer( S_OK );
}

//最小化窗体
STDMETHODIMP TangFrame::minimize()
{
    ::ShowWindow(m_hWnd, SW_MINIMIZE);
    return HResult_To_App_Layer( S_OK );
}


// BOOL SetEveryoneDACLReal(IN PCHAR pStrObjectName, IN SE_OBJECT_TYPE nObjectType)
// {
//     BOOL bRet = FALSE;
// 
//     PACL pOldDACL=NULL;
//     PACL pNewDACL=NULL;
//     EXPLICIT_ACCESS ea;
// 
//     ZeroMemory(&ea, sizeof(EXPLICIT_ACCESS));
// 
//     if(ERROR_SUCCESS == GetNamedSecurityInfo(pStrObjectName, nObjectType, DACL_SECURITY_INFORMATION, NULL, NULL, &pOldDACL, NULL, NULL))
//     {
//         BuildExplicitAccessWithName(&ea,"Everyone",GENERIC_ALL,SET_ACCESS, SUB_CONTAINERS_AND_OBJECTS_INHERIT);
// 
//         if(ERROR_SUCCESS == SetEntriesInAcl(1, &ea, pOldDACL, &pNewDACL))
//         {
//             if(ERROR_SUCCESS == SetNamedSecurityInfo(pStrObjectName, nObjectType, DACL_SECURITY_INFORMATION, NULL, NULL, pNewDACL, NULL))
//             {
//                 bRet = TRUE;
//             }
// 
//             LocalFree((HLOCAL) pNewDACL);
//         }
//     }
// 
//     return bRet;
// }
// 
// BOOL SetEveryoneDACL(IN PCHAR pStrObjectName, IN SE_OBJECT_TYPE nObjectType, HKEY hPredefineKey)
// {
//     if(!pStrObjectName || strlen(pStrObjectName)<1)
//         return FALSE;
// 
//     BOOL bRet = FALSE;
// 
//     switch (nObjectType)
//     {
//     case SE_FILE_OBJECT:
//     case SE_KERNEL_OBJECT:
//         {
//             bRet = SetEveryoneDACLReal(pStrObjectName, nObjectType);
//         }
//         break;
//     case SE_REGISTRY_KEY:
//         {
//             char strKeyName[MAX_PATH];
// 
//             switch ((UINT)hPredefineKey)
//             {
//             case HKEY_CLASSES_ROOT:
//                 {
//                     sprintf(strKeyName, "%s\\%s", "CLASSES_ROOT", pStrObjectName);
//                 }
//                 break;
//             case HKEY_CURRENT_USER:
//                 {
//                     sprintf(strKeyName, "%s\\%s", "CURRENT_USER", pStrObjectName);
//                 }
//                 break;
//             case HKEY_LOCAL_MACHINE:
//                 {
//                     sprintf(strKeyName, "%s\\%s", "MACHINE", pStrObjectName);
//                 }
//                 break;
//             case HKEY_USERS:
//                 {
//                     sprintf(strKeyName, "%s\\%s", "USERS", pStrObjectName);
//                 }
//                 break;
//             default:
//                 {
//                     //DebugDumpA("JCommon : Set EveryoneDACL: The PredefinedName:%d Unknown!", hPredefineKey);
//                 }
//                 break;
//             }
// 
//             bRet = SetEveryoneDACLReal(strKeyName, nObjectType);
//         }
//         break;
//     default:
//         {
//             //DebugDumpA("JCommon : Set EveryoneDACL: The ObjectType: %d with name:%s Unknown!"); 
//         }
//         break;
//     }
// 
//     return bRet;
// }

void NotifyMsiToExit(LPTSTR pEventName)
{

    // Ensure just only one application can be run
    HANDLE hEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, pEventName);
    if (NULL != hEvent)
    {
        SetEvent(hEvent);
        CloseHandle(hEvent);
    }
    return ;
}

STDMETHODIMP TangFrame::close(BOOL bStartNow)
{
    ShowWindow( SW_HIDE );

	//安装/卸载完成时释放被占用的文件
	if (m_bAlreadHold == TRUE)
	{
		for (int i = 0; i < UI_NEED_FILE_NO; i++)
		{
			if (NULL != g_hUIFiles[i])
			{
				CloseHandle(g_hUIFiles[i]);
				g_hUIFiles[i] = NULL;
				DeleteFile(g_szUIFiles[i]);
			}
		}
	}
	
	//删除云会议安装包。
	DeleteMeetingSetupExe();

	LOG_INFO(L"TangFrame::close::Installed:%d, InstallStatus:%d, autoRun:%d, runAfterInstalled:%d, m_type:%d, installPath=%s",
		m_bInstallorUnload, m_nInstalled, m_bAutoRun, m_bInstallorUnload, m_type, m_installPath);

    if (m_bInstallorUnload)
    {
        if (INSTALL_STATUS_ED == m_nInstalled)
        {
            LOG_INFO(L"UCSetup:close:unloaded,exit");
            if (m_type != TYPE_NOT_COMPLETE)
            {
                LOG_INFO(L"UCSetup:close:unloaded,exit successful.");
                if (bStartNow)
                {
                    RunProgram();
                }
            }
            else
            {
                LOG_ERR(L"UCSetup:close:unloaded,exit failed.");
            }

        }
        else if (INSTALL_STATUS_ING == m_nInstalled)
        {
            //正在安装
            LOG_INFO(L"UCSetup:close:installing, this is imposable");
        }
        else
        {
            //未安装
            LOG_INFO(L"UCSetup:close:not install, close msiexec.exe");
            CloseRunningApp(MSI_PROCESS);
        }
    }
    else
    {
        //是卸载的情况
        if (INSTALL_STATUS_ED == m_nInstalled)
        {
            LOG_INFO(L"UCSetup:close:unloaded, exit");
        }
        else if (INSTALL_STATUS_ING == m_nInstalled)
        {
            //正在卸载
            LOG_INFO(L"UCSetup:close:unloading, this is imposable");
        }
        else
        {
            //未卸载
            LOG_INFO(L"UCSetup:close:not unload close msiexec.exe");
            CloseRunningApp(MSI_PROCESS);
        }
    }

	::UninitLog();
    exit(0);
    return HResult_To_App_Layer( S_OK );
}


STDMETHODIMP TangFrame::navigate(BSTR relativePath,BOOL bLocalPage)
{
    HRESULT hr = S_OK;
    //CHECK_IF_NULL(relativePath);
    hr = m_view.Navigate(relativePath,bLocalPage);
    //HR_CHECK(hr);

exit:
    if (FAILED(hr))
    {
        LOG_ERR(L"TangFrame:Translate to url:%s with error: 0x%08X",relativePath,hr);
    }

    return HResult_To_App_Layer( hr );
}

//安装云会议
BOOL TangFrame::InstallMeeting()
{
	//如果需要安装云会议，杀掉TIMER_ID_INSTALLED
	KillTimer(TIMER_ID_INSTALLED);

	//从安装目录调起ucmeeting的安装程序
	CString sMeetingExe;
	sMeetingExe.Format(L"%s\\%s", m_installDir, UCCMEETING_SETUP);
	LOG_INFO(L"InstallMeeting UCCMeetingSetup.exe path:");
	LOG_INFO(sMeetingExe);
	if (FALSE == Util::FileExist(sMeetingExe))
	{
		LOG_ERR(L"InstallMeeting UCCMeetingSetup.exe is not exist!");
		return FALSE;
	}

	//静默式调同级目录下的云会议安装程序
// 	STARTUPINFO si;
// 	::memset( &si, 0, sizeof( si ));
// 	si.cb = sizeof( si );
// 	si.dwFlags = STARTF_USESHOWWINDOW;
// 	si.wShowWindow = SW_HIDE;
// 	PROCESS_INFORMATION pi;
// 	TCHAR szCommandLine[]= L" /S";
// 	BOOL isSucceed = CreateProcess(
// 		sMeetingExe,szCommandLine,NULL,
// 		NULL, FALSE, CREATE_NO_WINDOW,NULL,
// 		NULL, &si, &pi );
// 	
// 	if (!isSucceed)
// 	{
// 		DWORD dErr = GetLastError();
// 		LOG_ERR(L"CreateProcess MeetingSetup Failed! ErrorCode = %d", dErr);
// 		//DeleteMeetingSetupExe();
// 		return FALSE;
// 	}

	SHELLEXECUTEINFO shex;
	LPCTSTR pszParameters = NULL;
	LPCTSTR pszDirectory = NULL;
	memset( &shex, 0, sizeof( shex) );

	shex.cbSize			= sizeof( SHELLEXECUTEINFO ); 
	shex.fMask			= 0; 
	shex.hwnd			= NULL;
	shex.lpVerb			= L"open"; 
	shex.lpFile			= sMeetingExe;
	shex.lpParameters	= L" /S"; 
	shex.lpDirectory	= pszDirectory; 
	shex.nShow			= SW_HIDE; 

	LOG_INFO(L"Run UCCMeetingSetup.exe begin!");

	BOOL bRet = ::ShellExecuteEx(&shex);
	if (bRet == FALSE)
	{
		LOG_ERR(L"InstallMeeting, ShellExecuteEx return [%d], GetLastError =[%d]!", bRet, GetLastError());
		return FALSE;
	}

	g_bInstallMeeting = TRUE;
	LOG_INFO(L"Run UCCMeetingSetup.exe successful!");
	SetTimer(TIMER_ID_INSTALLMEETING, PROCESS_CHECK_INSTALL_MEETING);
	return TRUE;


	//创建线程通过检测注册表判断云会议是否安装成功
// 	BOOL bInstallSuccess = FALSE;
// 	HANDLE hInstallThread = CreateThread(NULL, 0, InstallMeetingPrc, (LPVOID)&bInstallSuccess, 0, NULL);
// 	if( WAIT_OBJECT_0 == WaitForSingleObject(hInstallThread, 50000)
// 		&& bInstallSuccess == TRUE)
// 	{
// 		LOG_INFO(L"Install Meeting Successed!");
// 
// 		HKEY hAppKey = NULL;
// 		TCHAR *bsKeyPath = {REG_INSTALLMEETING_FLAG};
// 		size_t nflag;
// 		if (RegCreateKeyEx(HKEY_CURRENT_USER,
// 			bsKeyPath,
// 			0,
// 			NULL,
// 			REG_OPTION_NON_VOLATILE,
// 			KEY_ALL_ACCESS,
// 			NULL,
// 			&hAppKey,
// 			(LPDWORD)&nflag) == ERROR_SUCCESS)
// 		{
// 			RegCloseKey(hAppKey);
// 		}
// 		return TRUE;
// 	}
// 
// 	LOG_INFO(L"Install Meeting Failed!");
//	return FALSE;
}

//检测是否安装了云会议。
BOOL TangFrame::CheckMeetingInstalled()
{
	if (IS_INSTALL_MEETING == 0)
	{
		return TRUE;
	}
	HKEY hAppKey = NULL;
	TCHAR *bsKeyPath = {_T("Software\\Tang")};

	if (RegOpenKeyEx(HKEY_CURRENT_USER,
		bsKeyPath,
		0,
		KEY_READ,
		&hAppKey) == ERROR_SUCCESS)
	{
		char dwValue[256];
		DWORD dwType = REG_DWORD;
		DWORD dwSize = sizeof(dwValue);
		if (::RegQueryValueEx(hAppKey,_T("ultimate"), 0, &dwType, (LPBYTE)&dwValue, &dwSize) == ERROR_SUCCESS)
		{	
			RegCloseKey(hAppKey);
			LOG_INFO(L"CheckMeetingInstalled result is TRUE.");
			return TRUE;
		}
		RegCloseKey(hAppKey);
	}

	//判断当前是否有会议端正在运行
// 	int nApp = FindRunningApp(UCCMEETING_PROCESS);
// 	if (nApp >0)
// 	{
// 		LOG_INFO(L"TangClient is Running, CheckMeetingInstalled result is TRUE."));
// 		return TRUE;
// 	}
// 	TCHAR moduleFullName[MAX_PATH] = {0};
// 	HRESULT hr = ::SHGetSpecialFolderPath(NULL, moduleFullName, CSIDL_PERSONAL, FALSE);
// 	if(_tcslen(moduleFullName) <=0)
// 		return FALSE;
// 
// 	CString sPath(moduleFullName);
// 	sPath = sPath.Left(sPath.ReverseFind('\\'));
// 	CString sTangRoute;
// 	sTangRoute.Format(_T("%s%s"), sPath, UCCMEETING_TANGROUTE);
// 	if(_taccess(sTangRoute.GetBuffer(0), 0) != -1)
// 	{
// 		return TRUE;
// 	}
	LOG_INFO(L"CheckMeetingInstalled result is FALSE.");
	return FALSE;
}

//卸载云会议，条件：1）由UC安装;2)当前没有正在运行的会议程序
BOOL TangFrame::UninstallMeeting()
{
	//先检查注册表判断是否是由uc安装
	HKEY hAppKey = NULL;
	TCHAR *bsKeyPath = {REG_INSTALLMEETING_FLAG};
	if (RegOpenKeyEx(HKEY_CURRENT_USER,
		bsKeyPath,
		0,
		KEY_READ,
		&hAppKey) == ERROR_SUCCESS)
	{
		RegCloseKey(hAppKey);

		//判断当前是否有会议端正在运行
		int nApp = FindRunningApp(UCCMEETING_PROCESS);
		if (nApp >0)
		{
			LOG_INFO(L"TangClient is Running, uninstall meeting failed.");
			return FALSE;
		}
		
		//获取卸载程序路径并静默启动。
		TCHAR moduleFullName[MAX_PATH] = {0};
		HRESULT hr = ::SHGetSpecialFolderPath(NULL, moduleFullName, CSIDL_PROFILE, FALSE);
		if(_tcslen(moduleFullName) <=0)
			return FALSE;

		CString sPath(moduleFullName);
		//sPath = sPath.Left(sPath.ReverseFind('\\'));
		CString sUninstall;
		sUninstall.Format(_T("%s%s"), sPath, UCCMEETING_UNINSTALL);

		STARTUPINFO si;
		::memset( &si, 0, sizeof( si ));
		si.cb = sizeof( si );
		si.dwFlags = STARTF_USESHOWWINDOW;
		si.wShowWindow = SW_HIDE;
		PROCESS_INFORMATION pi;
		TCHAR szCommandLine[]= L" /S";
		BOOL isSucceed = CreateProcess(
			sUninstall,szCommandLine,NULL,
			NULL, FALSE, CREATE_NO_WINDOW,NULL,
			NULL, &si, &pi );
		if(isSucceed)
		{
			LOG_INFO(L"Uninstall meeting successed.");
			RegDeleteKey(HKEY_CURRENT_USER, bsKeyPath);
			return TRUE;
		}
		else
		{
			LOG_ERR(L"Uninstall meeting CreateProcess Failed!, uninstall path = %s",sUninstall);
			return FALSE;
		}
	}
	else
	{
		LOG_INFO(L"UCCMeeting isn't installed by UC.");
		return TRUE;
	}
}

STDMETHODIMP TangFrame::InstallUCCMeeting( BOOL *pbSuccessed )
{
	if (IS_INSTALL_MEETING)
	{
		if (InstallMeeting())
		{
			*pbSuccessed = TRUE;
		}
		else
		{
			*pbSuccessed = FALSE;
		}
	}

	return HResult_To_App_Layer(S_OK);
}

STDMETHODIMP TangFrame::RecoverInstall()
{
	LOG_INFO(L"RecoverInstall be Called!");
	m_bRecoverInstall = TRUE;
// 	CString sTemp(lpstrCmdLine);
// 	sTemp = sTemp.Right(sTemp.GetLength() - sTemp.ReverseFind('\\') -1);
// 	Util::ModifyInstallPolicy(sTemp, TRUE);
	Uninstall();
	return S_OK;
}

//在注册表标记云会议是否由UC安装 
BOOL TangFrame::WriteInstallMeetingFlag()
{
	LOG_INFO(L"WriteInstallMeetingFlag be called!");
	HKEY hAppKey = NULL;
	TCHAR *bsKeyPath = {REG_INSTALLMEETING_FLAG};
	size_t nflag;
	if (RegCreateKeyEx(HKEY_CURRENT_USER,
		bsKeyPath,
		0,
		NULL,
		REG_OPTION_NON_VOLATILE,
		KEY_WRITE,
		NULL,
		&hAppKey,
		(LPDWORD)&nflag) == ERROR_SUCCESS)
	{
		RegCloseKey(hAppKey);
		return TRUE;
	}
	return FALSE;
}

//删除云会议安装包 
BOOL TangFrame::DeleteMeetingSetupExe()
{
	TCHAR exePath[MAX_PATH] = {0};
	_tcscpy(exePath, m_installDir);
	_tcscat(exePath, UCCMEETING_SETUP_PATH);

	LOG_INFO(L"Delete MeetingSetupExe! exePath = %s", exePath);
	return DeleteFile(exePath);
}

//云会议安装过程检测
DWORD WINAPI InstallMeetingPrc(LPVOID lp)
{
	BOOL* bSuccess = (BOOL*)lp;
	HKEY hAppKey = NULL;
	TCHAR *bsKeyPath = {_T("Software\\Tang")};
	while(TRUE)
	{
		TangFrame frame;
		int nApp = frame.FindRunningApp(UCCMEETING_SETUP);
		//云会议安装程序已退出
		if (nApp <=0)
		{
			LOG_INFO(L"UCCMeetingSetup.exe already exit.");
			hAppKey = NULL;
			if (RegOpenKeyEx(HKEY_CURRENT_USER,
				bsKeyPath,
				0,
				KEY_READ,
				&hAppKey) == ERROR_SUCCESS)
			{
				char dwValue[256];
				DWORD dwType = REG_DWORD;
				DWORD dwSize = 256;
				if (::RegQueryValueEx(hAppKey,_T("ultimate"), 0, &dwType, (LPBYTE)&dwValue, &dwSize) == ERROR_SUCCESS)
				{
					*bSuccess = TRUE;
				}
				else
				{
					*bSuccess = TRUE;
				}
				RegCloseKey(hAppKey);
				return TRUE;
			}
			else
				return FALSE;
		}

		Sleep(2000);
	}
}

STDMETHODIMP TangFrame::setCallBackHandler(BSTR str)
{
    HRESULT hr = S_OK;
    //ARG_CHECK(str);

    SYSFREESTRING(m_strCallBackHandler);
    m_strCallBackHandler = SysAllocString(str);

exit:
    if (FAILED(hr))
    {
        LOG_ERR(L"TangFrame: Set window height changed handler with error: 0x%08X",hr);
    }
    return hr;
}

STDMETHODIMP TangFrame::setSpaceCallBackHandler(BSTR str)
{
    HRESULT hr = S_OK;

    _ULARGE_INTEGER freespace,totalspace,userspace;

    TCHAR drive[3] = {0};
    _tcsncpy(drive, m_installDir, 2);
    ::GetDiskFreeSpaceEx(drive,&userspace,&totalspace,&freespace);
    int freeM = freespace.QuadPart/1024/1024;
    VARIANT * pVariant = new VARIANT[2];
    VARIANT varRetVal;
    VariantInit(&varRetVal);
    //if ( m_strCallBackHandler )
    {
        pVariant[0].vt = VT_UINT;
        pVariant[0].uintVal = INSTALLATION_SIZE;
        pVariant[1].vt = VT_UINT;
        pVariant[1].uintVal = freeM;
        fireEvent(str, pVariant, 2, &varRetVal);
    }

exit:
    if (FAILED(hr))
    {
        LOG_ERR(L"TangFrame: Set window height changed handler with error: 0x%08X",hr);
    }
    return hr;
}

STDMETHODIMP TangFrame::setSize(int width, int height)
{
    RECT windowRect, clientRect;
    ::GetWindowRect(m_hWnd, &windowRect);
    ::GetClientRect(m_hWnd, &clientRect);

    DWORD deltaWidth = 0;
    DWORD deltaHeight = 0;
    //窗口在屏幕之外(分辨率改变过程中)
    if( windowRect.left != -32000 || windowRect.top != -32000 )
    {
        deltaWidth  = (windowRect.right - windowRect.left) -
            (clientRect.right - clientRect.left);
        deltaHeight = (windowRect.bottom - windowRect.top) -
            (clientRect.bottom - clientRect.top);
    }
    else
    {
        windowRect.left = windowRect.top = 0;
        CenterWindow(::GetDesktopWindow());
    }

    windowRect.right = windowRect.left + width + deltaWidth;
    windowRect.bottom = windowRect.top + height + deltaHeight;

    ModifyStyle(WS_MAXIMIZE|WS_MINIMIZE,NULL);
    ::SetWindowPos(m_hWnd, 0, windowRect.left, windowRect.top,
        windowRect.right - windowRect.left, windowRect.bottom - windowRect.top,
        SWP_NOMOVE | SWP_NOZORDER);

    ::SetWindowPos(m_hWndClient, 0, windowRect.left, windowRect.top,
        windowRect.right - windowRect.left, windowRect.bottom - windowRect.top,
        SWP_NOMOVE | SWP_NOZORDER);

    m_uWidth = width;
    m_uHeight = height;
    m_bMaximized = FALSE;
    return HResult_To_App_Layer( S_OK );
}

STDMETHODIMP TangFrame::setPos(int left, int top)
{
    ::SetWindowPos(m_hWnd, 0, left, top, 0, 0, 
        SWP_NOSIZE | SWP_NOACTIVATE | SWP_NOZORDER);
    return HResult_To_App_Layer( S_OK );
}

STDMETHODIMP TangFrame::hide()
{
    ::ShowWindow(m_hWnd, SW_HIDE);
    return HResult_To_App_Layer( S_OK );
}

STDMETHODIMP TangFrame::DoInstall(BOOL bDeskLnk, BOOL bStartLnk, BOOL bAutoRun)
{
	LOG_INFO(L"DonInstall enter!");

    m_bAutoRun = bAutoRun;
    m_runAfterInstalled = TRUE;
	m_bDeskLnk = bDeskLnk;
	m_bQuickStartLnk = bStartLnk;

	//避免老版本因注册表破坏而导致的残留。下个版本去掉
	if (m_bRecoverInstall == FALSE) {
		CleanOldVersion();
	}
//     if (CheckIsInstalled())
//     {
//         //已经安装
//         m_nInstalled = INSTALL_STATUS_ING;
// 
//         //关闭msiexec.exe进程
//         //CloseRunningApp(MSI_PROCESS);
//         //MessageBox(L"ddddd");
//         //重启msi安装包
//         //静默修复
// 		StartMsi(_T("msiexec.exe"), _T("/fa {37255CD4-BFF5-4192-AADF-1289427C36B5} /qn"));
// 
//     }
//     else
    {
        //还未安装
        m_nInstalled = INSTALL_STATUS_ING;
		m_bInstallorUnload = TRUE;

		//删掉原有目录下的文件
		LOG_INFO(L"DonInstall::begin DeleteFolder! Path = %s", m_installDir);
		Util::DeleteFolder(m_installDir);
		LOG_INFO(L"DonInstall::end DeleteFolder!");

		//安装程序自己创建文件夹，保证安装目录的所有者是admin
		CString sDir(m_installDir);
		Util::CreateDir(sDir);
		
		sDir = sDir.Left(sDir.ReverseFind('\\'));
		//先把安装目录写到注册表，再由msi调用DLL去注册表读出安装目录
		LOG_INFO(L"TangFrame::DoInstall-WriteInstallInfoToReg, installDir = %s, bAutoRun=%d, runAfterInstalled=%d", sDir, m_bAutoRun, m_runAfterInstalled);
		Util::WriteInstallInfoToReg(sDir.GetBuffer(sDir.GetLength()), NULL, NULL, m_bAutoRun, m_runAfterInstalled);

        //关闭msiexec.exe进程
        //CloseRunningApp(MSI_PROCESS);

        //重启msi安装包
 		TCHAR sParam[MAX_PATH] = {0};
		TCHAR MyDocument[MAX_PATH] = {0};
		BOOL bRet = ::SHGetSpecialFolderPath(NULL, MyDocument, CSIDL_PERSONAL, FALSE);		
		if( bRet == TRUE && _tcslen(MyDocument) > 0 )
		{
			wsprintf(sParam, _T("/qn /l*v \"%s\\Quanshi Files\\Bee\\Log\\Install\\MsiInstall.log\""), MyDocument);
		}
		else
		{
			wsprintf(sParam, _T("/qn"));
		}

		_tcscpy(m_sProductCode, m_sNewProductCode);
		
		LOG_INFO(L"DoInstall::StartMsi lpstrCmdLine = %s, lpstrParameters = %s", lpstrCmdLine, sParam);
		StartMsi(lpstrCmdLine,sParam);//qn /l*v D:\\test\\9999.txt
    }

    return S_OK;
}


LRESULT TangFrame::OnDesktopChange( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
    int cxScreen = 0;
    int cyScreen = 0;
    switch(uMsg)
    {
    case WM_DISPLAYCHANGE:
        {

            break;
        }
    case WM_SETTINGCHANGE:
        {
            if (wParam == SPI_SETWORKAREA /*&& m_bMaximized*/)
            {

            }
            break;
        }
    default:
        break;
    }

    return 0;
}

LRESULT TangFrame::OnSysCommand( UINT uMsg,WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
    BSTR bstrhandler = NULL;
    bHandled = FALSE;
    VARIANT varRetVal;
    VARIANT varParam;        //message id.

    switch(wParam)
    {
    case SC_DEFAULT:
    case SC_CLOSE:
        //case WM_CLOSE:        //WM_CLOSE message will not be looped in this message handler.
        {
            if (m_strCloseHandler)
            {
                bstrhandler = m_strCloseHandler;
                bHandled = TRUE;
            }
            break;
        }
    case SC_MINIMIZE:
        {
            if (m_strMinHandler)
            {
                bstrhandler = m_strMinHandler;
                bHandled = TRUE;
            }
            break;
        }
    case SC_MAXIMIZE:
        {
            if (m_strMaxHandler)
            {
                bstrhandler = m_strMaxHandler;
                bHandled = TRUE;
            }
            break;
        }
    case SC_RESTORE:
        {
            if (m_strRestoreHandler)
            {
                bstrhandler = m_strRestoreHandler;
                bHandled = TRUE;
            }

            //added by hongwei.hu to resolve bug 14655 on 2/14/2011.
            if (m_bMaximized)
            {
                PostMessage(WM_TANG_WNDSIZE_SYNC);
            }

            // restore();
            break;
        }
    default:
        break;
    }

    return 0;
}

LRESULT TangFrame::OnHotKey( UINT uMsg,WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
    bHandled = FALSE;
    std::map<UINT32,_bstr_t>::iterator iter;

    return 0;
}

LRESULT TangFrame::OnSizeSync( UINT uMsg,WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
    int cxScreen = GetSystemMetrics(SM_CXSCREEN);
    int cyScreen = GetSystemMetrics(SM_CYSCREEN);
    //    DynamicWndSize(cxScreen,cyScreen);
    bHandled = TRUE;
    return 0;
}

LRESULT TangFrame::OnClose( UINT uMsg,WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
    //CString csF;
    //csF.Format(L"%d", m_nInstalled);
    //MessageBox(csF);
    if (INSTALL_STATUS_ING == m_nInstalled)
    {
        //MessageBox(L"dd");
        //截止关闭
        bHandled = TRUE;
    }
    else
    {
        //MessageBox(L"ss");
        bHandled = FALSE;
        close();
    }
    return 0;
}

LRESULT TangFrame::OnTimer( UINT uMsg,WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
    if (wParam == TIMER_ID_UPDATE_STATUS) 
    {
        BOOL bSuccess = TRUE;
        CString cs;
        static int nProcess = 0;
        if (m_type == TYPE_COMPLETE)
        {
			LOG_INFO(L"OnTimer -- TIMER_ID_UPDATE_STATUS, m_type == COMPLETE");
			m_type = TYPE_DEFAULT;
			g_type = TYPE_DEFAULT;
			g_nIndex = MAX_INDEX;
			nProcess = 100;
        }
        else if (m_type == TYPE_NOT_COMPLETE)
        {
			LOG_INFO(L"OnTimer -- TIMER_ID_UPDATE_STATUS, m_type == TYPE_NOT_COMPLETE");
			m_type = TYPE_DEFAULT;
			g_type = TYPE_DEFAULT;
            g_nIndex = MAX_INDEX;
            nProcess = 100;
            bSuccess = FALSE;
			if (m_bInstallorUnload == 1)
			{
				Util::DeleteInstallInfoFromReg();
			}
        }
 
        if(-1 == g_nIndex)
        {
            LOG_INFO(L"OnTimer -- TIMER_ID_UPDATE_STATUS:g_nIndex: -1, continue...");
            return 0;
        }

		nProcess += 4;
		TCHAR slog[MAX_PATH] = {0};
		wsprintf(slog, _T("OnTimer -- TIMER_ID_UPDATE_STATUS, process += 5, process = %d, g_Index = %d"),nProcess, g_nIndex);
		LOG_INFO(slog);
		//保证进度在当前index和下一个index之间
        if (nProcess < g_nProcess[g_nIndex])
        {
            nProcess = g_nProcess[g_nIndex] -1;
        }
		else if (g_nIndex < 4 && nProcess >= g_nProcess[g_nIndex + 1])
        {
			nProcess = g_nProcess[g_nIndex + 1] - 1;
        }
        LOG_INFO(L"OnTimer -- TIMER_ID_UPDATE_STATUS,g_nIndex:%d, nProcess: %d", g_nIndex, nProcess);

        if(nProcess >= 100 && g_nIndex == 4)
        {
            KillTimer(TIMER_ID_UPDATE_STATUS);
            m_nInstalled = INSTALL_STATUS_ED;
            nProcess = 100;

            if (bSuccess)
            {
                if (m_bInstallorUnload)
                {
                    //创建桌面和开始菜单快捷方式
                    LOG_INFO(L"UCSetup:OnTimer, install end, create Lnk");
                    TCHAR szSetupPath[MAX_PATH] = {0};
                    wsprintf(szSetupPath, _T("%s%s%s"), m_installDir, START_PROGRAM_UI, SETUP_NAME);
					if (m_bDeskLnk)
					{
						LOG_INFO(L"UCSetup:OnTimer, install end, create desktop Lnk");
						Util::CreateDesktopLnk(m_installPath, TARGET_LNK);
					}

					//程序列表
					Util::CreateLnkToProgramsMenu(m_installPath, szSetupPath,
						START_PROGRAM_UC, SETUP_LNK_NEW, TARGET_LNK);
                   
					//快速启动栏
					if (m_bQuickStartLnk) {
						LOG_INFO(L"UCSetup:OnTimer, install end, create quick launch Lnk");
						Util::CreateLnkToQuickStartMenu(m_installPath, TARGET_QUICK_START_LNK);
                    }
                    
					CString sDir = m_installPath;
					sDir = sDir.Mid(0, sDir.ReverseFind(L'\\'));

					//将安装信息（版本、产品ID、是否立即启动、是否开机自启动）
					Util::WriteInstallInfoToReg(m_installDir, m_sVersion, m_sProductCode, m_bAutoRun, m_runAfterInstalled, m_bDeskLnk, m_bQuickStartLnk);
					AutoStartSoftware(m_bAutoRun);
                }
                else
                {
                    LOG_INFO(L"UCSetup:OnTimer: uninstall end, delete Lnk");

					//删除旧版本的的快捷方式
					Util::DeleteLnkFromProgramsMenu(START_PROGRAM_UC_OLD);
					Util::DeleteLnkFromDesktop(TARGET_LNK_OLD);

                    Util::DeleteLnkFromProgramsMenu(START_PROGRAM_UC);
                    Util::DeleteLnkFromDesktop(TARGET_LNK);
					Util::DeleteLnkFromQuickStartMenu(TARGET_QUICK_START_LNK);
					Util::DeleteLnkFromQuickStartMenu(TARGET_QUICK_START_PINNED_LNK);
					//删除注册表之前,获取之前的安装信息，安装过程要用到
					LOG_INFO(_T("uninstall complete!before reinstall!"));
					TCHAR* pInstallDir = new TCHAR[MAX_PATH];
					BOOL bAuto, bRunNow, bDeskLnk, bStartLnk, bOld;
					BOOL bRet = Util::GetInstallInfoFromReg(&pInstallDir, bAuto, bRunNow, bDeskLnk, bStartLnk, bOld);
					if (bRet == TRUE)
					{
						if (bOld == TRUE)
						{				
							//删除老的安装目录 UserName\\G-Net\\OnCloud
							Util::DeleteFolder(pInstallDir);	
							//设置安装配置为默认
							SetDefaultInstallConfig();
							LOG_INFO(_T("GetInstallInfoFromReg from old version."));
						}
						else
						{
							//老版本注册表中无desklnk、startLnk，覆盖老版本时忽略注册表，采用默认值。
							m_bDeskLnk = bDeskLnk;
							m_bQuickStartLnk = bStartLnk;
							_tcscpy(m_installDir, pInstallDir);
						}
						LOG_INFO(_T("GetInstallInfoFromReg install dir: "));
						LOG_INFO(m_installDir);

						wsprintf(m_installPath, _T("%s%s"), m_installDir, TARGET_NAME);
						m_bAutoRun = bAuto;												
					}
					else
					{
						SetDefaultInstallConfig();
					}

					delete[] pInstallDir;

					TCHAR sMsg[MAX_PATH] = { 0 };
					wsprintf(sMsg, _T("m_bRecoverInstall = %d, GetInstallInfoFromReg Result = %d"), m_bRecoverInstall, bRet);
					LOG_INFO(sMsg);
					//避免残留，再清理一次
					LOG_INFO(L"OnTimer::begin delete install dir! Path = %s", m_installDir);
					
					Util::DeleteFolder(m_installDir);
					Util::DeleteInstallInfoFromReg(TRUE);

					//清理老客户端Log目录
					TCHAR personalDir[MAX_PATH] = {0};
					bRet = ::SHGetSpecialFolderPath(NULL, personalDir, CSIDL_PERSONAL, FALSE);
					if (bRet == TRUE && _tcslen(personalDir) > 0)
					{
						CString sLogPath;
						sLogPath.Format(_T("%s%s"), personalDir, L"\\OnCloud");
						LOG_INFO(L"OnTimer::begin delete Old Log Dir! Path = %s", sLogPath);
						Util::DeleteFolder(sLogPath);
						LOG_INFO(L"OnTimer::end delete Old Log Dir!");

						sLogPath.Format(_T("%s%s"), personalDir, L"\\UCInstallLog");
						LOG_INFO(L"OnTimer::begin delete Old Install Log Dir! Path = %s", sLogPath);
						Util::DeleteFolder(sLogPath); //Util::DeleteFiles(sLogPath, TRUE, NULL); //L"uc_db"
						LOG_INFO(L"OnTimer::end delete Old Log Dir!");
					}

					//清理新客户端Log目录
					TCHAR moduleFullName[MAX_PATH] = {0};
					bRet = ::SHGetSpecialFolderPath(NULL, moduleFullName, CSIDL_LOCAL_APPDATA, FALSE);		
					if( bRet == TRUE && _tcslen(moduleFullName) > 0 && m_bRecoverInstall == FALSE)
					{
						CString sLogPath;
						sLogPath.Format(_T("%s%s"), moduleFullName, L"\\Quanshi\\Bee");
						LOG_INFO(L"OnTimer::begin delete Log Dir! Path = %s", sLogPath);
						//Util::DeleteFiles(sLogPath, TRUE, NULL); 
						Util::DeleteFolder(sLogPath);
						LOG_INFO(L"OnTimer::end delete Log Dir!");
					}
					
                    AutoStartSoftware(FALSE);
                }

				if(m_bRecoverInstall == TRUE && m_bInstallorUnload == FALSE)
				{
					//覆盖安装的卸载完成，隔一秒再安装，防止卸载未完全完成
					SetTimer(TIMER_ID_INSTALLED, 1000);
					//杀死检测定时器
					KillTimer(TIMER_ID_CHECK_INSTALL);
				}
            }
        }

		//覆盖安装，进度合并，卸载占60%， 安装占40%
		int MergeProcess = nProcess;
		if(m_bRecoverInstall == TRUE )
		{
			if(m_bInstallorUnload == FALSE)//覆盖安装的卸载过程
			{
				//nProcess = (nProcess>=100?0:nProcess);//卸载完成，将进度设为60%
				MergeProcess *= 0.6;
			}
			else	//覆盖安装的安装过程
			{
				MergeProcess = 60+ (MergeProcess*0.4);
			}
		}

        PostDataToJS(bSuccess, MergeProcess);
        
        cs.Format(_T("UCSetup:OnTimer: after PostDataToJS(%d, %d)\n"), bSuccess, MergeProcess);
		wsprintf(slog, _T("+++++UCSetup:OnTimer: after PostDataToJS(%d, %d)"), bSuccess, MergeProcess);
		OutputDebugString(slog);
        LOG_INFO(cs);
    }
    else if (wParam == TIMER_ID_INSTALLED)
    {
		LOG_INFO(L"OTimer -- TIMER_ID_INSTALLED Enter! g_bInstallMeeting = %d", g_bInstallMeeting);
		KillTimer(TIMER_ID_INSTALLED);
		//覆盖安装：卸载完成，开始安装
		if(m_bRecoverInstall == TRUE && m_bInstallorUnload ==FALSE)
		{
			LOG_INFO(L"RecoverInstall Begin Install!");
			g_nIndex = -1;
			DoInstall(m_bDeskLnk, m_bQuickStartLnk, m_bAutoRun);
		}
		else if(g_bInstallMeeting == TRUE)
		{
			//UC的安装已经完成了
			LOG_INFO(L"OTimer::TIMER_ID_INSTALLED, g_bInstallMeeting = TRUE");
		}
    }
    else if(wParam == TIMER_ID_UNLOAD_CONTROLPANNEL)
    {
        if (Util::MockLButtonDown())
        {
            SetForegroundWindow(m_hWnd);
            KillTimer(TIMER_ID_UNLOAD_CONTROLPANNEL);
        }
    }
    else if(wParam == TIMER_ID_CHECK_INSTALL)
    {
        //检测Msi运行进程个数，在执行安装或卸载时，会有三个程序副本
        int nRunningCopy = FindRunningApp(MSI_PROCESS);
		LOG_INFO(L"UCSetup:OnTimer -- TIMER_ID_CHECK_INSTALL, FindRunningApp(MSI_PROCESS) return %d", nRunningCopy);
	
		g_checkTimes++;
		if (g_checkTimes < 5)
		{			
			LOG_INFO(L"UCSetup:OnTimer -- TIMER_ID_CHECK_INSTALL, g_checkTimes = %d", g_checkTimes);
			return 0;
		}

        if (nRunningCopy < MAX_MSIEXEC_COPY)//MAX_MSIEXEC_COPY
        {

			//检测注册表
			BOOL bInstalled = CheckInstallEnd();
            if (m_bInstallorUnload == bInstalled)
            {
                //在此种情况下，如果是安装过程，则注册表项已经写上
                //如果是卸载过程，则注册表项已经销毁
                g_type = TYPE_COMPLETE;					
				m_type = g_type;
				KillTimer(TIMER_ID_CHECK_INSTALL);
				LOG_INFO(L"UCSetup:OnTimer -- TIMER_ID_CHECK_INSTALL g_type:%d", g_type);
				LOG_INFO(L"UCSetup:OnTimer:TIMER_ID_CHECK_INSTALL,Complete!! m_bInstallorUnload == bInstalled = %d.", m_bInstallorUnload );
            }
            else if (nRunningCopy == 0)
            {
				//出问题了，可能是msi进程被强杀
				g_type = TYPE_NOT_COMPLETE;
				m_type = g_type;
				LOG_ERR(L"UCSetup:OnTimer -- TIMER_ID_CHECK_INSTALL g_type:%d", g_type);
				LOG_INFO(L"UCSetup:OnTimer -- TIMER_ID_CHECK_INSTALL, Error!!! m_bInstallorUnload = %d,bInstalled = %d.", m_bInstallorUnload, bInstalled);
				KillTimer(TIMER_ID_CHECK_INSTALL);
            }
        }
        else
        {
            LOG_INFO(L"UCSetup:OnTimer -- TIMER_ID_CHECK_INSTALL msiexec is running, continue....");
        }
			
        return 0;
    }
	else if(wParam == TIMER_ID_INSTALLMEETING)	//安装云会议。
	{
		//杀死检测定时器
		KillTimer(TIMER_ID_CHECK_INSTALL);
		KillTimer(TIMER_ID_UPDATE_STATUS);

		g_bInstallMeeting = TRUE;
		static int meetingProc = 75;
		bool bDelay = false;
		if(meetingProc >= 80)	//5秒之后开始轮循检测注册表
		{
			int nApp = FindRunningApp(UCCMEETING_SETUP);
			if(TRUE == CheckMeetingInstalled() && nApp == 0)
			{
				g_bInstallMeeting = FALSE;
				LOG_INFO(L"OnTimer::TIMER_ID_INSTALLMEETING::Meeting Installed!");
				WriteInstallMeetingFlag();
				DeleteMeetingSetupExe();
				KillTimer(TIMER_ID_INSTALLMEETING);
				meetingProc = 100;
			}
			else if(meetingProc >165)	//正常情况20秒左右可装完，若90秒未装完，认为安装失败。
			{
				g_bInstallMeeting = FALSE;
				LOG_INFO(L"OnTimer::TIMER_ID_INSTALLMEETING::Meeting Install out time!");
				DeleteMeetingSetupExe();
				meetingProc = 101;
				KillTimer(TIMER_ID_INSTALLMEETING);
			}
			else if(meetingProc >= 99)
			{
				bDelay = true;
			}
		}
		if(bDelay)
			PostDataToJS(TRUE, 99);
		else
			PostDataToJS(TRUE, meetingProc);

		meetingProc += 2;
	}

    if (wParam == m_uTimer) 
    {
        // process the close modal dialog timer 
        //        LOG_INFO(L"TangFrame:OnTimer to close modal dialog.");
        HWND hwnd = ::GetLastActivePopup(m_hWnd);        //? use GetActiveWindow() GetForegroundWindow() GetLastActivePopup()
        ::EndDialog(hwnd,0);
        m_bModalDisplay = FALSE;

        //kill the timer.
        ::KillTimer(m_hWnd,m_uTimer);
        m_uTimer = 0;
    }

    if (wParam == m_uTimer_Adsorption)
    {
        if (m_hSonWnd == NULL)
        {
            ::KillTimer(m_hWnd,m_uTimer_Adsorption);
        }
        CRect rect;
        CRect rect_son;
        BOOL bRes = FALSE;
        ::GetWindowRect(m_hSonWnd,&rect_son);
        ::GetWindowRect(m_hWnd, &rect);
        if ( (rect.left+m_xVal != rect_son.left) || (rect.top+m_yVal != rect_son.top) )
        {
            //            LOG_INFO(L"AdsorptionWindow need to move");
            if ( !(::IsIconic(m_hWnd)) 
                && (rect.left > 0 || rect.left + rect.Width() > 0 || rect.left + m_xVal > 0) )
            {
                bRes = ::SetWindowPos(m_hSonWnd, NULL, rect.left + m_xVal, rect.top + m_yVal, 0, 0, 
                    SWP_NOSIZE | SWP_NOACTIVATE | SWP_NOZORDER );
            }

        } 
        else
        {
            //            LOG_DEBUG(L"AdsorptionWindow need not to move");
        }

    }

    return 0;
}

LRESULT TangFrame::OnSessionStateChange( UINT uMsg,WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
    switch (wParam)
    {
    case WTS_SESSION_LOCK:
        {
            //MessageBoxA(NULL,"lock","lock",1);
            //hide(TRUE);
            //            LOG_INFO(L"screenlock status = lock.");

            break;
        }
    case WTS_SESSION_UNLOCK:
        {
            //MessageBoxA(NULL,"Unlock","Unlock",1);
            //hide(FALSE);


            break;
        }
    case WTS_SESSION_LOGON:
    case WTS_SESSION_LOGOFF:
    case WTS_REMOTE_CONNECT:
    case WTS_REMOTE_DISCONNECT:
    case WTS_SESSION_REMOTE_CONTROL:
    case WTS_CONSOLE_DISCONNECT:
    case WTS_CONSOLE_CONNECT:

        break;

    default:
        break;
    }
    return 0;
}

LRESULT TangFrame::OnActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    bHandled  = FALSE;
    VARIANT varRetVal;
    VARIANT varParam;        //message id.

    /**< 鍒濆鍖朧ARIANT鍙橀噺 */
    VariantInit(&varRetVal);
    VariantInit(&varParam);

    //notify the event.
    if (!wParam)
    {
        varParam.vt = VT_UINT;
        varParam.uintVal = wParam;
        if(m_strLostFocusHanlder)
        {
            //             LOG_INFO(L"NcActivate,ready to call js's LostFocusHanlder");
            //             fireEvent(m_strLostFocusHanlder, &varParam, 1, &varRetVal);    /**< 鍙戦�佺獥鍙ｅけ鍘荤劍鐐逛簨浠?*/
            //             bHandled = TRUE;
        }
    }

    return 0;
}

LRESULT TangFrame::OnMoving(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    //    LOG_DEBUG(L"TangFrame-----window moving");
    if (m_bAdsorption)
    {
        VARIANT varRetVal;
        VARIANT varParam;        //message id.

        bHandled = FALSE;
        if (m_strWindowMoveHandler)
        {
            varParam.vt = VT_UINT;
            varParam.uintVal = uMsg;  
            //            fireEvent(m_strWindowMoveHandler, &varParam, 1, &varRetVal);
            bHandled = TRUE;
        }
    }

    {//璁剧疆鍚搁檮绐楀彛鐨勪綅缃?
        CRect rect;
        BOOL bRes = FALSE;
        ::GetWindowRect(m_hWnd, &rect);
        bRes = ::SetWindowPos(m_hSonWnd, NULL, rect.left + m_xVal, rect.top + m_yVal, 0, 0, 
            SWP_NOSIZE | SWP_NOACTIVATE | SWP_NOZORDER );
    }

    {
        LPRECT pRc = (LPRECT)lParam;
        pRc->right = pRc->left + m_uWidth; 
        pRc->bottom = pRc->top + m_uHeight; 
    }
    return 0;
}

LRESULT TangFrame::OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    //    LOG_DEBUG(L"OnGetMinMaxInfo");
    LPMINMAXINFO pMaxInfo= (MINMAXINFO *)lParam;
    POINT pt;
    pt.x = 0;
    pt.y = m_uMinWinHeight;
    pMaxInfo->ptMinTrackSize = pt;

    return 0;
}

LRESULT TangFrame::OnSizing(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    bHandled = FALSE;
    LPRECT pRect = (LPRECT)lParam;
    //    LOG_INFO(L"TangFrame::OnSizing rect info:width:[%d],rect height:[%d]",pRect->right-pRect->left,pRect->bottom-pRect->top);
    VARIANT * pVariant = new VARIANT[2];
    VARIANT varRetVal;
    VariantInit(&varRetVal);
    if ( m_strSizeChangedHandler )
    {
        pVariant[0].vt = VT_UINT;
        pVariant[0].uintVal = pRect->bottom - pRect->top; //height
        pVariant[1].vt = VT_UINT;
        pVariant[1].uintVal = pRect->right - pRect->left; //width
        //        fireEvent(m_strSizeChangedHandler, pVariant, 2, &varRetVal);
        bHandled = TRUE;
    }
    SAFE_DELETE_ARRAY( pVariant );

    //::GetWindowRect(m_hWnd,&rect_t);
    m_uWidth = pRect->right - pRect->left;
    m_uHeight = pRect->bottom - pRect->top;

    return 0;
}

LRESULT TangFrame::OnTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    CString csMsg;
    csMsg.Format(_T("++++UCSetup:OnTest:Success: %d, Process: %d"), wParam, lParam);
    LOG_INFO(csMsg);
	TCHAR szLog[MAX_PATH] = {0};
	wsprintf(szLog, _T("+++++++UCSetup:OnTest:Success: %d, Process: %d"), wParam, lParam);
	OutputDebugString(szLog);
	g_checkTimes = 5;
    if(wParam)
    {
//         if(100 == lParam)
//         {
//             //安装完成后，6秒钟关闭程序
//             SetTimer(TIMER_ID_INSTALLED, PROCESS_CLOSE_TIME);
//         }
        ++g_nIndex;
		csMsg.Format(_T("++++UCSetup:OnTest:g_nIndex = %d"), g_nIndex);
		LOG_INFO(csMsg);
    }

	if (m_bAlreadHold == FALSE)
	{
		LOG_INFO(L"OnTest, Hold UI Files.");
		m_bAlreadHold = TRUE;
		//占住安装、卸载完成或失败界面的所有资源的文件。
		for (int i = 0; i < UI_NEED_FILE_NO; i++)
		{
			TCHAR szPath[MAX_PATH] = { 0 };
			Util::MakePath(szPath, g_szUIFiles[i]);
			_tcscpy(g_szUIFiles[i], szPath);
			g_hUIFiles[i] = CreateFile(szPath,
				GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
		}
	}
    return 0;
}

HRESULT TangFrame::GetExternal( External **ppExternal )
{
    HRESULT hr  = S_OK;

    //    CHECK_IF_NULL(ppExternal);

    *ppExternal = &m_external;

    return hr;
}

HRESULT TangFrame::GetWebBrowserInstance(IDispatch **ppBrowser)
{
    HRESULT hr  = S_OK;

    //    CHECK_IF_NULL(ppBrowser);

    m_view.GetWebBrowserInstance(ppBrowser);

exit:
    if(FAILED(hr))
    {
        //log the error.
        //        LOG_ERR(L"Get ie external with error: 0x%08X", hr);
    }

    return hr;
}

void TangFrame::RegisterShadowlessWndClass()
{
    WNDCLASS wc;
    ::GetClassInfo(GetModuleHandle(NULL), _T("#32770"), &wc);

    // change the name of the class.
    wc.lpszClassName = _T("ShadowlessDialogClass");

    // register this class so that we can use it.
    RegisterClass(&wc);
}

HRESULT TangFrame::Initialize(BOOL bInstall, BOOL bFromControlPannel, TCHAR* sVersion, TCHAR* sProductCode)
{
    CString cs;
    cs.Format(_T("UCSetup Initialize: bFromControlPannel =[%d]"), bFromControlPannel);
    LOG_INFO(cs);
    TCHAR* szDefaultPage = NULL;
    m_bInstallorUnload = bInstall;
	_tcscpy(m_sVersion, sVersion);
	_tcscpy(m_sNewProductCode, sProductCode);
    //m_bFromControlPannel = bFromControlPannel;
    if(bInstall)
    {
        szDefaultPage = DEFAULT_INDEX_PAGE_PATH;
    }
    else
    {
        szDefaultPage = DEFAULT_UNLOAD_PAGE_PATH;
    }

    LOG_INFO(L"UCSetup Initialize:close msiexec.exe");
    CloseRunningApp(MSI_PROCESS);

    if (bFromControlPannel)
    {
        SetTimer(TIMER_ID_UNLOAD_CONTROLPANNEL, 10, NULL);

    }

    m_external.PutPluginObject(ADAPTOR_NAME_UTILITY, (IDispatch*)m_pUtil);

    m_view.Navigate(szDefaultPage, TRUE);

    return S_OK;
}

HRESULT TangFrame::UnInitialize()
{
    m_external.RemovePluginObject(ADAPTOR_NAME_UTILITY);

    return S_OK;
}

BOOL TangFrame::EnableDebugPrivilege()
{
    HANDLE hToken;
    BOOL fOk=FALSE;
    if(OpenProcessToken(GetCurrentProcess(),TOKEN_ADJUST_PRIVILEGES,&hToken))
    {
        TOKEN_PRIVILEGES tp;
        tp.PrivilegeCount=1;
        if(!LookupPrivilegeValue(NULL,SE_DEBUG_NAME,&tp.Privileges[0].Luid))
        {
            //LOG_INFO("LookupPrivilegeValue fail");
        }
        tp.Privileges[0].Attributes=SE_PRIVILEGE_ENABLED;
        if(!AdjustTokenPrivileges(hToken,FALSE,&tp,sizeof(tp),NULL,NULL))
        {
            //LOG_INFO("AdjustTokenPrivileges fail");
        }

        fOk=(GetLastError()==ERROR_SUCCESS);
        CloseHandle(hToken);
    }
    return fOk;
}

void TangFrame::CloseRunningApp(TCHAR* lpProcName)
{
	OutputDebugString(_T("CloseRunningApp start::"));
	OutputDebugString(lpProcName);
	CString sLog;
	sLog.Format(L"CloseRunningApp enter, process name = %s", lpProcName);
	LOG_INFO(sLog);
    EnableDebugPrivilege();
    PROCESSENTRY32 pe = {sizeof(pe)};
    int nRet;

    DWORD proid;
    HANDLE hprocess;

    //创建进程快照
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if (hSnapshot != INVALID_HANDLE_VALUE)	
    {
        //从进程快照中遍历进程
        nRet = Process32First(hSnapshot, &pe);

        if (nRet)	
        {	
            do 
            {
                //判断进程名称是否和我们要找的进程名称相同
                if(StrCmpIW(pe.szExeFile, lpProcName) == 0)	
                {
                    proid=pe.th32ProcessID;

                    hprocess=::OpenProcess(PROCESS_ALL_ACCESS,FALSE,proid);

                    if(hprocess!=NULL)
                    {
                        //LOG_INFO("hprocess!=NULL");
						TCHAR szlog[MAX_PATH] = {0};
						wsprintf(szlog, _T("+++++Kill process: %s"),lpProcName);
						OutputDebugString(szlog);
						sLog.Format(L"CloseRunningApp Kill process = [%s], success.", lpProcName);
						LOG_INFO(sLog);
                        ::TerminateProcess(hprocess,0);
                        ::CloseHandle(hprocess);
                    }
                    //LOG_INFO("Find CloseHandle");
                }

            } while (Process32Next(hSnapshot, &pe));    
        }
        CloseHandle(hSnapshot);
    }
    //LOG_INFO("End CloseRunningApp");
}

HRESULT TangFrame::fireEvent(BSTR handler, VARIANT* parameters,
                             DWORD nParams,VARIANT* varRet)
{
    HRESULT hr = S_OK;
    IDispatch* pDispatch = NULL;
    CComPtr<IWebBrowser2> pBrowser;

    //get the web browser dispatch instance.
    if ( handler == NULL )
    {
        return E_INVALIDARG;
    }
    m_view.GetWebBrowserInstance(&pDispatch);

    //query the web browser instance.
    //CHECK_IF_NULL(pDispatch);
    hr = pDispatch->QueryInterface(IID_IWebBrowser2,(void **)&pBrowser);
    pDispatch->Release();
    //HR_CHECK(hr);
    //CHECK_IF_NULL(pBrowser);

    if(!pBrowser)
        return S_FALSE;

    IHTMLDocument2* pHTDoc;
    hr = pBrowser->get_Document((IDispatch**) &pHTDoc);
    if(SUCCEEDED(hr) && pHTDoc)
    {
        IHTMLWindow2* pHTWnd = NULL;
        hr = pHTDoc->get_parentWindow(&pHTWnd);	
        if(SUCCEEDED(hr) && pHTWnd)
        {
            UINT errArg;
            DISPID dispid;

            hr = pHTWnd->GetIDsOfNames(IID_NULL, &handler, 1,
                LOCALE_SYSTEM_DEFAULT, &dispid);
            if(SUCCEEDED(hr))
            {
                DISPPARAMS dParams = { parameters, 0, nParams, 0 };
                if (varRet)
                {
                    ::VariantInit(varRet);
                }
                hr = pHTWnd->Invoke(dispid, IID_NULL, LOCALE_SYSTEM_DEFAULT,
                    DISPATCH_METHOD, &dParams, varRet, 0, &errArg);
                if (varRet->vt == VT_BSTR && varRet->bstrVal != NULL)
                {
                    //LOG_INFO(L"Return value:%s",varRet->bstrVal);
                }

                if(SUCCEEDED(hr))
                {
                    pHTWnd->Release();
                    pHTDoc->Release();
                    return S_OK;
                }
            }
            pHTWnd->Release();
        }
        pHTDoc->Release();
    }

exit:
    return HResult_To_App_Layer( hr );
}

void TangFrame::KillAllTimer()
{
	KillTimer(TIMER_ID_UPDATE_STATUS);
	KillTimer(TIMER_ID_CHECK_INSTALL);
	KillTimer(TIMER_ID_INSTALLED);
	KillTimer(TIMER_ID_UNLOAD_CONTROLPANNEL);
	KillTimer(TIMER_ID_INSTALLMEETING);
}

//设置默认安装信息
void TangFrame::SetDefaultInstallConfig()
{
	LOG_INFO(L"SetDefaultInstallConfig enter.");
	memset(m_installDir, 0, MAX_PATH);
	memset(m_installPath, 0, MAX_PATH);
	TCHAR   szPath[MAX_PATH] = { 0 };
	SHGetSpecialFolderPath(NULL, szPath, CSIDL_PROFILE, FALSE);
	wsprintf(m_installDir, _T("%s\\Quanshi%s"), szPath, INSTALL_DIR_TEMP);
	wsprintf(m_installPath, _T("%s%s"), m_installDir, TARGET_NAME);
	m_bDeskLnk = m_bQuickStartLnk = m_bAutoRun = TRUE;
}

void TangFrame::CleanOldVersion() 
{
	LOG_INFO(L"CleanOldVersion enter.");

	//关闭客户端
	CloseRunningAppbyMsg(CLIENT_WINDOW_TITLE);
	CloseRunningApp(UCCHAT_PROCESS_OLD);
	CloseRunningApp(UCCLIENT_PROCESS_OLD);
	CloseRunningApp(UCSQLAGENT_PROCESS_OLD);
	
	//清理旧版安装目录
	TCHAR oldInstallDir[MAX_PATH] = { 0 };
	TCHAR   szPath[MAX_PATH] = { 0 };
	SHGetSpecialFolderPath(NULL, szPath, CSIDL_PROFILE, FALSE);
	wsprintf(oldInstallDir, _T("%s\\G-Net\\OnCloud"));
	Util::DeleteFolder(oldInstallDir);

	//删除旧版本的的快捷方式
	Util::DeleteLnkFromProgramsMenu(START_PROGRAM_UC_OLD);
	Util::DeleteLnkFromDesktop(TARGET_LNK_OLD);
}

void TangFrame::CloseRunningAppbyMsg(TCHAR* lpWndName)
{
	HWND hWnd = FindWindow(NULL, lpWndName);
	if (hWnd != NULL)
	{
		::PostMessage(hWnd, WM_CLOSE, NULL, NULL);
	}
}
