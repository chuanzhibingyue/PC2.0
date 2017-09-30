#include "InstallWnd.h"
#include "LogManager.h"
#include <shlobj.h>

using namespace uc;
/*************************************Macro******************************/
#define INSTALLDLL extern "C" __declspec(dllexport)
#define TIMER_PROGRESS 1234567					//mock进度条定时器
#define TIMER_PROGRESS_CYCLE 1000				//mock进度条定时器触发周期（ms）

/*************************************Macro******************************/
/*******************************global variable *************************/
CComModule _Module;
InstallWnd g_InstallWnd;		//主窗口全局对象
int g_nInstallType = 1;			//0--卸载，1--安装，2--从控制面板卸载
BOOL g_bCloseMainWnd = FALSE;	//是否关闭页面
int g_nCurProgress = 0;			//当前进度
int g_nProgressIndex = 0;		//当前进度所属结点索引
int g_arrProgress[6] = { 3, 10, 30, 60, 80, 100 };	//进度结点，起始默认为3
BOOL g_bInitLog = FALSE;
/*******************************global variable **************************/

/*************************export func  declaration************************/
INSTALLDLL int ShowMainWnd(char szProductCode[MAX_PATH], char szPackageCode[MAX_PATH], char szVersion[MAX_PATH]);								//显示安装/卸载主窗口
INSTALLDLL void GetInstallDir(char sDir[MAX_PATH]);			//获取安装目录
INSTALLDLL void PostProgress(int status, int percent);		//发送安装进度status- 状态（0-失败，1-正常); percent-进度
INSTALLDLL void WaitForUIFinish();							//阻塞系统安装程序等待用户关闭UI再退出
INSTALLDLL void SetInstallType(int nType);					//设置安装类型，0--卸载，1--安装，2--从控制面板卸载
INSTALLDLL int GetInstallType();							//获取安装类型,0--卸载，1--安装，2--从控制面板卸载
INSTALLDLL void CleanUpBeforUninstallFromCP();				//用于从控制面板卸载前的清理工作
INSTALLDLL void MyLog(int errCode, char sMsg[MAX_PATH]);	//用于IS脚本log输出
/*************************export function  declaration**********************/

void InitLog();
/*************************export function definition************************/
BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
	)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		InitLog();
		break;
	case DLL_THREAD_ATTACH:
		//MessageBox(NULL, L"DLL_THREAD_ATTACH", NULL, 0);
		break;
	case DLL_THREAD_DETACH:
		//MessageBox(NULL, L"DLL_THREAD_DETACH", NULL, 0);
		break;
	case DLL_PROCESS_DETACH:
		{
			if (g_bInitLog == TRUE)
			{
				UC_LOG(INFO) << "detach DLL!";
				//UC_LOG_FLUSH();
				LogManager::GetInstance().UnInit();
			}
			break;
		}
		
	}
	return TRUE;
}

void InitLog()
{
	if (g_bInitLog == TRUE)
		return;

	TCHAR MyDocument[MAX_PATH] = { 0 };
	::SHGetSpecialFolderPath(NULL, MyDocument, CSIDL_PERSONAL, FALSE);
	TCHAR logPath[MAX_PATH] = { 0 };
	wsprintf(logPath, L"%s\\%s", MyDocument, INSTALL_LOG_PATH);
	g_InstallWnd.CreateDir(logPath);
	char logPaht_A[MAX_PATH] = { 0 };
	int nLen = ::WideCharToMultiByte(CP_ACP, 0, logPath, MAX_PATH,
		logPaht_A, MAX_PATH, NULL, NULL);
	if (nLen > 0) 
	{
		g_bInitLog = TRUE;
		LogManager::GetInstance().Init(logPaht_A, "MsiDLL", "BeeInstall.log", false, 2, 0, 3);
		UC_LOG(INFO) << "InitLog finish.";
	}
}

INSTALLDLL void PostProgress(int status, int percent)
{
	
	UC_LOG(INFO) << "++++PostProgress enter!status = "<< status <<", percent = "<< percent << ", InstallType = "<< g_nInstallType << ", ProgressIndex = " << g_nProgressIndex;
	HWND hWnd = NULL;
	if (g_nInstallType == 1)
	{
		hWnd = FindWindow(NULL, INSTALL_WINDOW_TITLE);
	}
	else if (g_nInstallType == 0)
	{
		hWnd = FindWindow(NULL, UNINSTALL_WINDOW_TITLE);
	}
	else
	{
		if (percent == 100)
		{
			//控制面板卸载成功
			UC_LOG(INFO) << "Uninstall from control panel succeeded, begin clean up residues & Uninstall meeting.";
			g_InstallWnd.CleanInstallResidues(TRUE);	//清理残留
			g_InstallWnd.UninstallMeeting();			//卸载云会议
		}
		return;
	}

	if (!IsWindow(hWnd))
	{
		UC_LOG(ERROR) << "ERROR!! FindWindow is invalid.";
		return;
	}

	//设置mock progress定时器
	if (g_nProgressIndex == 0)
	{
		SetTimer(hWnd, TIMER_PROGRESS, TIMER_PROGRESS_CYCLE, NULL);
	}

	//安装/卸载完成
	if (percent == 100 )
	{
		KillTimer(hWnd, TIMER_PROGRESS);
		if (g_nInstallType == 1)
		{
			UC_LOG(INFO) << "Install succeeded! begin install meeting & create link & write reg flag.";
			//蜜蜂安装结束

			//解压第三方文件
// 			if (FALSE == g_InstallWnd.UnZipThirdPartyFile()){
// 				UC_LOG(ERROR) << "UnZipThirdPartyFile failed. install failed.";
// 				g_InstallWnd.WriteInstallInfoToReg();	//写reg标记
// 				SendMessage(hWnd, WM_PROGRESS, 0, percent);
// 				return;
// 			}
			
			g_InstallWnd.InstallMeeting();			//开始安装云会议
			g_InstallWnd.CreateAllLnk();			//创建快捷方式
			g_InstallWnd.WriteInstallInfoToReg();	//写reg标记
		}
		else
		{	
			UC_LOG(INFO) << "Uninstall from start menu succeeded! begin clean up residues & Uninstall meeting.";
			//卸载结束
			g_InstallWnd.CleanInstallResidues(TRUE);	//清理残留
			g_InstallWnd.UninstallMeeting();			//卸载云会议
		}	
	}
	
	g_nCurProgress = percent;
	SendMessage(hWnd, WM_PROGRESS, status, percent);
	g_nProgressIndex++;
}


DWORD WINAPI ShowMainWndThreadProc(__in LPVOID lpParam)
{
	UC_LOG(INFO) << "ShowMainWndThreadProc enter.";
	HWND parent = (HWND)0x0;
	g_InstallWnd.DoModal(parent);
	return 0;
}

INSTALLDLL int ShowMainWnd(char szProductCode[MAX_PATH], char szPackageCode[MAX_PATH], char szVersion[MAX_PATH])
{
	UC_LOG(INFO) << "***********  ShowMainWnd enter *************";
	UC_LOG(INFO) << " InstallType = " << g_nInstallType;
	
	if (g_nInstallType == 1) 
	{
		UC_LOG(INFO) << " productCode = " << szProductCode;
		UC_LOG(INFO) << " packageCode = " << szPackageCode;
		UC_LOG(INFO) << " version = " << szVersion;
	}

	//调用SetProcessDPIAware
	HINSTANCE hUser32 = LoadLibrary(_T("user32.dll"));
	if (hUser32)
	{
		typedef BOOL(WINAPI* LPSetProcessDPIAware)(void);
		LPSetProcessDPIAware pSetProcessDPIAware = (LPSetProcessDPIAware)GetProcAddress(hUser32, "SetProcessDPIAware");
		if (pSetProcessDPIAware)
		{
			pSetProcessDPIAware();
			UC_LOG(INFO) << "after SetProcessDPIAware";
		}
		FreeLibrary(hUser32);
	}

	if (g_nInstallType == 1)//安装
	{
		g_InstallWnd.SaveInstallCode(szProductCode, szPackageCode, szVersion);
	}

	//创建开始安装/卸载事件
	g_InstallWnd.m_hBeginExcute = CreateEvent(NULL, TRUE, FALSE, NULL);
	
	//在子线程中创建安装窗口，以避免阻塞msi安装进度
	HANDLE tdUI = CreateThread(NULL, 0,
		ShowMainWndThreadProc,
		NULL,
		0, NULL);

	//等待用户点击确定安装/卸载
	if (tdUI == NULL) {
		UC_LOG(INFO) << " createThread for show main window failed. error code = " << GetLastError();
		//UC_LOG_FLUSH();
		return 0;
	}

	WaitForSingleObject(g_InstallWnd.m_hBeginExcute, INFINITE);

	UC_LOG(INFO) << "WaitForSingleObject(m_hBeginExcute) return.";
	//用户点击关闭
	if (g_bCloseMainWnd == TRUE)
	{
		UC_LOG(INFO) << "--------user closed the main window, install exit.";
		//UC_LOG_FLUSH();
		return 0;
	}

	switch (g_nInstallType)
	{
	case 0:
		UC_LOG(INFO) << "+++Begin Uninstall from start menu.";
		break;
	case 1:
		Sleep(500);
		UC_LOG(INFO) << "Begin install, delete files : " << g_InstallWnd.m_szInstallDir;
		g_InstallWnd.DeleteFiles(g_InstallWnd.m_szInstallDir, TRUE);
		UC_LOG(INFO) << "+++Begin Install.";
		break;
	case 2:
		UC_LOG(INFO) << "+++Begin Uninstall from control panel.";
		break;
	default:
		UC_LOG(INFO) << "+++default: Begin Install.";
		break;
	}

	//开始安装之后
	//创建完成安装事件，以hold安装完成界面
	g_InstallWnd.m_hFinishInstall = CreateEvent(NULL, TRUE, FALSE, NULL);
	
	//init进度为3
	PostProgress(1, 3);
	Sleep(500);
	return 1;
}

INSTALLDLL void WaitForUIFinish()
{
	UC_LOG(INFO) << "WaitForUIFinish enter.";
	WaitForSingleObject(g_InstallWnd.m_hFinishInstall, INFINITE);
	UC_LOG(INFO) << "WaitForUIFinish return. user have clicked OK button.";
}

INSTALLDLL void GetInstallDir(char sDir[MAX_PATH])
{
	UC_LOG(INFO) << "GetInstallDir enter, m_szInstallDir = " << g_InstallWnd.m_szInstallDir;
	int nLen = ::WideCharToMultiByte(CP_ACP, 0, g_InstallWnd.m_szInstallDir, MAX_PATH,
		(LPSTR)sDir, MAX_PATH, NULL, NULL);
	UC_LOG(INFO) << "GetInstallDir enter. install directory = " << sDir;
}

INSTALLDLL void SetInstallType(int nType)
{
	UC_LOG(INFO) << "SetInstallType enter, type =" << nType;
	g_nInstallType = nType;
}

INSTALLDLL int GetInstallType()
{
	UC_LOG(INFO) << "GetInstallType enter, type =" << g_nInstallType;
	return g_nInstallType;
}

DWORD WINAPI CleanUpThreadProc(__in LPVOID lpParam)
{
	g_InstallWnd.KillRelationExe();
	g_InstallWnd.GetInstallInfoFromReg(TRUE);
	g_InstallWnd.CleanInstallResidues(TRUE, FALSE);
	g_InstallWnd.UninstallMeeting();
	return 0;
}

//从控制面板的卸载
INSTALLDLL void CleanUpBeforUninstallFromCP()
{
	UC_LOG(INFO) << "CleanUpBeforUninstallFromCP enter.";
	g_InstallWnd.KillRelationExe();
	g_InstallWnd.DelSendActionFromRBtn();
	g_InstallWnd.GetInstallInfoFromReg(TRUE);
}

INSTALLDLL void MyLog(int errCode, char sMsg[MAX_PATH])
{
	UC_LOG(INFO) << "InstallShield log, code: " << errCode << ", msg: " << sMsg;
}

