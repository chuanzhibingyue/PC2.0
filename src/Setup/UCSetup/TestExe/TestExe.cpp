// TestExe.cpp : Implementation of WinMain


#include "stdafx.h"
#include "resource.h"
#include "TestExe.h"
#include "TangFrame.h"
#include "Util.h"

//#define DEFAULT_INDEX_PAGE_PATH     L"\\html\\page\\LoginView.html" /**< 默认加载页面*/

CAppModule _Module;
CMessageLoop theLoop;

LPTSTR lpstrCmdLine;

BOOL g_bInstall = FALSE;
BOOL g_bFromControlPannel = FALSE;
BOOL g_bRecoverInstall = FALSE;
TCHAR g_sVersion[MAX_PATH] = {0};
TCHAR g_sProductCode[MAX_PATH] = {0};

BOOL RunMyApp(LPTSTR lpstrCmdLine, BOOL &bInstall, BOOL &bFromControlPannel)
{
	CString csMsg;
	csMsg.Format(L"RunMyApp:cmd: %s", lpstrCmdLine);
	LOG_INFO(csMsg);
    if (NULL != _tcsstr(lpstrCmdLine, _T("-suninstallbee")))
    {
        if (NULL != _tcsstr(lpstrCmdLine, _T("-suninstallbeefromcp")))
        {
            //从控制面板卸载
            bInstall = FALSE;
            bFromControlPannel = TRUE;
			LOG_INFO(L"RunMyApp -suninstallbeefromcp");
        }
        else
        {
            //开始菜单卸载，为了解决文件卸载不掉的问题，需要调起一个以"-t"参数运行的新卸载程序
			LOG_INFO(L"RunMyApp -suninstallbee");
            Util::DoTransferDir();
            Util::RunNewSetup();
            return FALSE;
        }
    }
    else if (NULL != _tcsstr(lpstrCmdLine, _T("-tnewprocess")))
    {
        //新运行的卸载程序
        bInstall = FALSE;
        bFromControlPannel = FALSE;
		LOG_INFO(L"RunMyApp -tnewprocess");

    }
    else	//安装,参数包含版本号
    {
        bInstall = TRUE;
        bFromControlPannel = FALSE;
    }
    return TRUE;
}

int Run(LPTSTR lpstrCmdLine, int nCmdShow)
{
    _Module.AddMessageLoop(&theLoop);

    TangFrame wndFrame;
    if(wndFrame.CreateEx() == NULL)
    {
        return 0;
    }

    wndFrame.Initialize(g_bInstall, g_bFromControlPannel, g_sVersion, g_sProductCode);

   // wndFrame.show(SW_SHOW);

    int nResult = theLoop.Run();

    return nResult;
}

extern "C" int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpCmdLine, int nShowCmd)
{
	::InitLog();
	::SetLogLevel(LEVEL_INFO);

    if (!RunMyApp(lpCmdLine, g_bInstall, g_bFromControlPannel))
    {
        return 0;
    }

    if (!g_bInstall && !g_bFromControlPannel)
    {
        //是从开始菜单卸载的情况，防止新进程过快运行起来会退出，所以先睡下
        Sleep(200);
    }
    if (Util::AppAlreadyExist())
    {
        return TRUE;
    }

    HRESULT hRes = ::CoInitialize(NULL);
    ATLASSERT(SUCCEEDED(hRes));

    ::DefWindowProc(NULL, 0, 0, 0L);

    AtlInitCommonControls(ICC_COOL_CLASSES | ICC_BAR_CLASSES);



    CString csMsg;
    csMsg.Format(L"UCSetup:_tWinMain:Cmd: %s", lpCmdLine);
	LOG_INFO(csMsg);

	//获取命令行参数
	LPTSTR lpTemp;
	if (lpCmdLine == NULL ||  wcslen(lpCmdLine) == 0)
	{
		lpstrCmdLine = GetCommandLine();
	}
	else
	{
		lpstrCmdLine = lpCmdLine;
	}

	//解析安装参数 eg: c:\\TEMP\\云企1.0.msi -v1.00.0000 -k{37255CD4-BFF5-4192-AADF-1289427C36B5}
	TCHAR* strVersion = _tcsstr(lpstrCmdLine, _T(" -v"));	//版本号
	TCHAR* strKey = _tcsstr(lpstrCmdLine, _T(" -k"));		//产品ID
	if (NULL != strVersion && NULL != strKey)
	{
		*strVersion = '\0';
		*strKey = '\0';
		strVersion += 3;
		_tcscpy(g_sVersion, strVersion);

		strKey+=3;
		_tcscpy(g_sProductCode, strKey);
	}



    GUID guid;
    hRes = _Module.Init(NULL, hInstance, &guid);
    ATLASSERT(SUCCEEDED(hRes));

    AtlAxWinInit();

    hRes = _Module.RegisterClassObjects(CLSCTX_LOCAL_SERVER, REGCLS_SINGLEUSE);
    ATLASSERT(SUCCEEDED(hRes));

    int nResult = Run(lpCmdLine, nShowCmd);

    _Module.Term();

    ::CoUninitialize();

    return nResult;
}
