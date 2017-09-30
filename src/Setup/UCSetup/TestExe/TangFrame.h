/******************************************************************************
* Copyright (c) 2009~2010 All Rights Reserved by
*  G-Net Integrated Service co. Ltd. 
******************************************************************************/
/**
* @file TangView.h
* @brief interface of the TangFrame class.
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
#pragma once

/*----------------------------HEADER FILE INCLUDES---------------------------*/
#include "resource.h"
#include "TangView.h"
#include "UtilAdaptor.h"
#include "IExternal.h"
//#include "ViewArea.h"
#include "NativeActionDispImpl.h"
#include "TestExe.h"
#include <comdef.h>
#include <comdefsp.h>

#define MAIN_WND_NAME_STR       L"UCClientMainFrame"
#define WM_TANG_WNDSIZE_SYNC    (WM_APP + 2)
#define PROCESS_MSG 1233
typedef enum
{
    TYPE_DEFAULT = 1,
    TYPE_COMPLETE,
    TYPE_NOT_COMPLETE
}exception_type;

class TangFrame :
    public CFrameWindowImpl<TangFrame>,
    public CUpdateUI<TangFrame>,
    public CMessageFilter,
    public CIdleHandler,
    public NativeActionDispImpl<ITangWnd>
{
public:
    DECLARE_FRAME_WND_CLASS(MAIN_WND_NAME_STR, IDR_MAINFRAME)

    TangFrame();
    ~TangFrame();

    virtual BOOL PreTranslateMessage(MSG* pMsg);
    virtual BOOL OnIdle();

    BEGIN_UPDATE_UI_MAP(TangFrame)
    END_UPDATE_UI_MAP()

    BEGIN_MSG_MAP(TangFrame)
        MESSAGE_HANDLER(WM_CREATE, OnCreate)
        MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
        MESSAGE_HANDLER(WM_SETTINGCHANGE, OnDesktopChange)
        MESSAGE_HANDLER(WM_DISPLAYCHANGE, OnDesktopChange)
        MESSAGE_HANDLER(WM_SYSCOMMAND, OnSysCommand)
        MESSAGE_HANDLER(WM_HOTKEY, OnHotKey)
        MESSAGE_HANDLER(WM_CLOSE, OnClose)
        MESSAGE_HANDLER(WM_TIMER, OnTimer)
        MESSAGE_HANDLER(WM_TANG_WNDSIZE_SYNC, OnSizeSync)
        MESSAGE_HANDLER(WM_WTSSESSION_CHANGE,OnSessionStateChange)
        MESSAGE_HANDLER(WM_ACTIVATE, OnActivate)
        MESSAGE_HANDLER(WM_MOVING, OnMoving)
        MESSAGE_HANDLER(WM_GETMINMAXINFO, OnGetMinMaxInfo)
        MESSAGE_HANDLER(WM_SIZING, OnSizing)
        MESSAGE_HANDLER(PROCESS_MSG, OnTest)
        CHAIN_MSG_MAP(CUpdateUI<TangFrame>)
        CHAIN_MSG_MAP(CFrameWindowImpl<TangFrame>)
    END_MSG_MAP()

    LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnDesktopChange(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnSysCommand(UINT uMsg,WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnHotKey(UINT uMsg,WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnClose(UINT uMsg,WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnTimer(UINT uMsg,WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnSizeSync(UINT uMsg,WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnSessionStateChange(UINT uMsg,WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnMoving(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnSizing(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

public:
    STDMETHOD(show)(int uFlags);
    STDMETHOD(hide)();
    STDMETHOD(close)(BOOL bStartNow = FALSE);
    STDMETHOD(setTitle)(BSTR bstrTitle);
    STDMETHOD(setSize)(int, int);
    STDMETHOD(setPos)(int, int);
    STDMETHOD(navigate)(BSTR bstrRelativePath, BOOL bIsLocalPage = TRUE);

    STDMETHOD(setCallBackHandler)(BSTR);
    STDMETHOD(setSpaceCallBackHandler)(BSTR);
	STDMETHOD(minimize)();
	STDMETHOD(setWinTitleRect)(UINT px = 0,UINT py = 0,UINT x = 0,UINT y = 0);

	STDMETHOD(selectFolderDialog)(BSTR strDialogTitle,BSTR *pstrDir);

    STDMETHOD(CheckInstall)(int *pbInstalled);
    STDMETHOD(CheckAppRunning)(BOOL *pbRunning);
    STDMETHOD(DoInstall)(BOOL bDeskLnk, BOOL bStartLnk, BOOL bAutoRun);
    STDMETHOD(ReInstall)(BSTR *pstrDir);
	STDMETHOD(RecoverInstall)();
    STDMETHOD(Uninstall)();
	STDMETHOD(CheckNeedInstallMeeting)(BOOL *pbNeedInstall);
	STDMETHOD(CheckIsNewVersion)(BOOL *pbNew);
	STDMETHOD(InstallUCCMeeting)(BOOL *pbSuccessed);

public:
    HRESULT GetExternal( External **ppExternal );
    HRESULT GetWebBrowserInstance(IDispatch **ppBrowser);
    void RegisterShadowlessWndClass();
    HRESULT Initialize(BOOL bInstall, BOOL bFromControlPannel, TCHAR* sVersion, TCHAR* sProductCode);
    HRESULT UnInitialize();

	BOOL EnableDebugPrivilege();
	void CloseRunningApp(TCHAR* lpProcName);
	void CloseRunningAppbyMsg(TCHAR* lpWndName);
    int FindRunningApp(TCHAR* lpProcName);
    HRESULT fireEvent(BSTR handler, VARIANT* parameters, DWORD nParams,VARIANT* varRet);

private:
	void KillAllTimer();

public:
    HWND m_hSonWnd;
    BOOL m_bMaximized;
    BOOL m_bShadowLess;
    BOOL m_bModalDisplay;
    BOOL m_bBlockMsg;
    BOOL m_bAutoKill;
    UINT32 m_uModalDialogNeedMsg;
    UINT m_uTimer;
    UINT m_uElapse;
    CRect m_rTitle; /**< 绐楀彛浼爣棰樻爮 */
    BOOL m_bAdsorption; /**< 绉诲姩鍏宠仈鏍囪 */
    UINT m_uMinWinHeight; /**< 绐楀彛鏈�灏忛珮搴?*/
    BOOL m_bAllowReSize; /**< 鏄惁鍏佽鎷栧姩鏀瑰彉绐楀彛澶у皬 */
    int m_xVal;
    int m_yVal;

    //绐楀彛鍏宠仈绉诲姩鐩稿叧鍙橀噺
    UINT m_uTimer_Adsorption;
    UINT m_uElapse_Adsorption;

    //绂佺敤绐楀彛鏍囪
    BOOL m_bEnable;

    //涓婁笅绉诲姩绐楀彛鐨勫悕绉?
    BOOL m_bMobileDirection;

    //绐楀彛瀹介珮
    UINT32 m_uWidth;
    UINT32 m_uHeight;

protected:
    BSTR                    m_strMinHandler;
    BSTR                    m_strLostFocusHanlder;/**< 澶卞幓鐒︾偣鐨?*/
    BSTR                    m_strMaxHandler;
    BSTR                    m_strCloseHandler;
    BSTR                    m_strRestoreHandler;
    BSTR                    m_strWindowMoveHandler;
    BSTR                    m_strSizeChangedHandler;
    BSTR					m_strCallBackHandler;
    BSTR                    m_strSpaceCallBackHandler;
    std::map<UINT,_bstr_t>  m_hotkeyHandlerMap;
    std::map<int,UINT>      m_hotkeyIDMap;

private:
    //开机自动启动指定程序
    HRESULT AutoStartSoftware(BOOL bAutoStart);
    //安装完运行程序
    HRESULT RunProgram();
    int CheckIsInstalled();
	BOOL CheckInstallEnd();
	BOOL CheckInstallEnd_NA();
	BOOL CheckVersion();
    void PostDataToJS(BOOL bSuccess, int nValue);
    void StartMsi(LPTSTR szFileName, LPTSTR szParameters);
	BOOL InstallMeeting();
	BOOL UninstallMeeting();
	BOOL CheckMeetingInstalled();
	BOOL WriteInstallMeetingFlag();
	BOOL DeleteMeetingSetupExe();
	void SetDefaultInstallConfig();
	void CleanOldVersion();
    TCHAR*                  m_strDefaultPage; /**< 榛樿椤甸潰 */
    CComObject<TangView>    m_view;
    UtilAdaptor*            m_pUtil;
    External                m_external;
    TCHAR                    m_installDir[MAX_PATH];
    TCHAR                    m_installPath[MAX_PATH];
	TCHAR					m_sVersion[MAX_PATH];
	TCHAR					m_sNewProductCode[MAX_PATH];
	TCHAR					m_sProductCode[MAX_PATH];

	BOOL m_bRecoverInstall;
    int m_nInstalled;
    exception_type m_type;

	BOOL m_bAutoRun;				//开机自启动
	BOOL m_runAfterInstalled;	//安装完成后立即运行
	BOOL m_bInstallorUnload;	//1 --安装，0 -- 卸载
	BOOL m_bDeskLnk;			//创建桌面快捷键
	BOOL m_bQuickStartLnk;		//创建快捷启动栏快捷键
	int m_ucorbeeinstalled;		//当前安装的产品 1 -- 老版蜜蜂 2-- 新版蜜蜂
	BOOL m_bAlreadHold;			//是否已经占用UI资源文件
};
DWORD WINAPI InstallMeetingPrc(LPVOID lp);
