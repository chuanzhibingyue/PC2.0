#include"InstallWnd.h"
#include <shlobj.h>
#include <TlHelp32.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "LogManager.h"
#include "XUnzip.h"
#include <io.h>
#include <vector>
using namespace uc;

#define IS_CHARACTER(ch) ((ch >= 65 && ch<=90) || (ch>=97 && ch<=122))
#define IS_SPECIAL_CHARACTER(ch) (ch=='*' || ch == '"' || ch == '?' || ch == '<' || ch == '>' || ch =='|')  
extern BOOL g_bCloseMainWnd;
InstallWnd::InstallWnd() : HtmlControl(L"index.html")
{
	Initialize();
	CONNECT_JS_CALL_HANDLER(GetInstallDir, InstallWnd::OnGetInstallDir);
	CONNECT_JS_CALL_HANDLER(SetCallBackHandler, InstallWnd::OnSetCallBackHandler);
	CONNECT_JS_CALL_HANDLER(Install, InstallWnd::OnInstall);
	CONNECT_JS_CALL_HANDLER(Uninstall, InstallWnd::OnUnistall);
	CONNECT_JS_CALL_HANDLER(CheckInstalled, InstallWnd::OnCheckInstalled);
	CONNECT_JS_CALL_HANDLER(RecoverInstall, InstallWnd::OnRecoverInstall);
	CONNECT_JS_CALL_HANDLER(CheckAppRunning, InstallWnd::OnCheckAppRunning);
	CONNECT_JS_CALL_HANDLER(SelectFolderDialog, InstallWnd::OnSelectFolderDialog);
	CONNECT_JS_CALL_HANDLER(IsOSVersionGreaterXPSP3, InstallWnd::OnIsOSVersionGreaterXPSP3);
	CONNECT_JS_CALL_HANDLER(IsHighDPI, InstallWnd::OnIsHighDPI);
	CONNECT_JS_CALL_SIMPLE_HANDLER(Foo, InstallWnd::OnFoo);

}


void InstallWnd::Initialize()
{
	memset(m_szInstallDir, 0, MAX_PATH);
	memset(m_szTargetPath, 0, MAX_PATH);
	memset(m_szProductCode, 0, MAX_PATH);
	memset(m_szPackageCode, 0, MAX_PATH);
	memset(m_szOldInstallDir, 0, MAX_PATH);
	TCHAR szPath[MAX_PATH] = { 0 };
	SHGetSpecialFolderPath(NULL, szPath, CSIDL_PROFILE, FALSE);
	wsprintf(m_szInstallDir, _T("%s%s"), szPath, INSTALL_DIR);
	wsprintf(m_szTargetPath, _T("%s\\%s"), m_szInstallDir, TARGET_NAME);
	m_bDeskLnk = m_bQuickStartLnk = m_bAutoRun = 1;
	m_nRecoverType = 0;
}


//test
const char* InstallWnd::OnFoo(int argc, const char* argv[])
{
	if (argc > 0)
	{
		const char* a = argv[0];
		int cc = 0;
		//BSTR s(a);
	}

	DISPPARAMS empty = { NULL, NULL, 0, 0 };
	VARIANT retval;
	CallJS(m_callbackHandler, &empty, &retval);
	return "ok";
}

void InstallWnd::OnGetInstallDir(DISPPARAMS*, VARIANT* retVal)
{
	retVal->vt = VT_BSTR;
	retVal->bstrVal = ::SysAllocString(m_szInstallDir);
}

void InstallWnd::OnSetCallBackHandler(DISPPARAMS*params, VARIANT*)
{
	BSTR handler = params->rgvarg[0].bstrVal;
	m_callbackHandler = SysAllocString(handler);
}


BOOL InstallWnd::GetProductCode(TCHAR** ppProductCode)
{
	HKEY hAppKey = NULL;

	TCHAR *bsKeyPath = { REG_SOFTWARE };

	if (RegOpenKeyEx(HKEY_CURRENT_USER,
		bsKeyPath,
		0,
		KEY_QUERY_VALUE,
		&hAppKey) == ERROR_SUCCESS)
	{
		DWORD dwType = REG_SZ;
		DWORD dwSize = MAX_PATH;
		TCHAR *bsKeyProductCode = REG_PRODEUCTCODE;

		//获取产品ID信息
		if (::RegQueryValueEx(hAppKey, bsKeyProductCode, 0, &dwType, (LPBYTE)*ppProductCode, &dwSize) == ERROR_SUCCESS)
		{
			RegCloseKey(hAppKey);
			return TRUE;
		}
		RegCloseKey(hAppKey);
		return FALSE;
	}

	return FALSE;
}


void InstallWnd::OnIsOSVersionGreaterXPSP3(DISPPARAMS*, VARIANT* retVal)
{
	retVal->vt = VT_BOOL;
	retVal->boolVal = TRUE;
	OSVERSIONINFOEX Version;
	ZeroMemory(&Version, sizeof(OSVERSIONINFOEX));
	Version.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	if (GetVersionEx((OSVERSIONINFO*)&Version))
	{
		int nMajor = Version.dwMajorVersion;
		int nMinor = Version.dwMinorVersion;
		int nSPMajor = Version.wServicePackMajor;
		TCHAR sLog[MAX_PATH] = { 0 };
		wsprintf(sLog, _T("IsOSVersionGreaterXPSP3: MajorVersion = %d, MinorVersion = %d, ServicePackMajor = %d"), nMajor, nMinor, nSPMajor);
		UC_LOG(INFO) << sLog;
		//XPSP3 --> 5.1 SP3
		if ((nMajor < 5) || (nMajor == 5 && nMinor < 1)
			|| (nMajor == 5 && nMinor == 1 && nSPMajor < 3))
		{
			retVal->boolVal = FALSE;
		}
	}
}

void InstallWnd::OnIsHighDPI(DISPPARAMS*, VARIANT* retVal)
{
	retVal->vt = VT_BOOL;
	retVal->boolVal = FALSE;
	float fact = GetDeviceScaleFactor();
	if (fact >= 1.20) {
		retVal->boolVal = TRUE;
	}	
}

//检测当前安装状态：0 -- 未安装任何蜜蜂； 1-- 已安装老蜜蜂； 2---已经安装当前版本; 3--已经安装同一个包; 4 -- 已安装更新版本
void InstallWnd::OnCheckInstalled(DISPPARAMS*, VARIANT* retVal)
{
	UC_LOG(INFO) << "OnCheckInstalled enter.";
	HKEY hAppKey = NULL;
	TCHAR *bsKey = { REG_SOFTWARE };

	TCHAR* productKey = new TCHAR[MAX_PATH];
	TCHAR* packageKey = new TCHAR[MAX_PATH];
	TCHAR* versionKey = new TCHAR[MAX_PATH];
	BOOL bFindProductCode = GetProductCode(&productKey);
	BOOL bFindPackageCode = GetPackageCode(&packageKey);
	BOOL bFindVersion = GetVersionCode(&versionKey);
	if (bFindProductCode == TRUE) //已经安装蜜蜂
	{
		if (bFindPackageCode == TRUE) //已安装当前版本
		{		
			m_nRecoverType = 2;	//非同一个包,且当前要安装的版号更新
			if (0 == StrCmpIW(packageKey, m_szPackageCode)) //同一个包
			{				
				m_nRecoverType = 3;
			}
			else
			{					
				if (bFindVersion == TRUE)
				{			
					if(StrCmpIW(versionKey, m_szVersion) > 0) //要安装的版本低于已经安装版本
					{
						m_nRecoverType = 4;
					}
				}
			}
		}
		else
		{
			//已安装老蜜蜂
			m_nRecoverType = 1;
		}

	}
	else 
	{
		m_nRecoverType = 0;
	}

	UC_LOG(INFO) << "OnCheckInstalled return. recover type = " << m_nRecoverType;
	delete[] productKey;
	delete[] packageKey;
	delete[] versionKey;
	retVal->vt = VT_INT;
	retVal->intVal = m_nRecoverType;
}

void InstallWnd::OnInstall(DISPPARAMS*params, VARIANT* retVal)
{
	UC_LOG(INFO) << "OnInstall enter.";
	TCHAR* stopStr = NULL;
	retVal->vt = VT_INT;
	retVal->intVal = 0;

	//判断用户输入的目录是否有效
	TCHAR* inputVal = params->rgvarg[3].bstrVal;
	int len = _tcslen(inputVal);
	if (inputVal == NULL || len < 2 || len > 250)
	{
		UC_LOG(INFO) << "OnInstall custom install path is null.";
		return;
	}

	TCHAR installDir[MAX_PATH] = { 0 };
	if (!IsEndWithInstallDir(inputVal))
	{
		wsprintf(installDir, _T("%s%s"), inputVal, INSTALL_DIR_TEMP);
	}
	else{
		_tcscpy_s(installDir, inputVal);
	}

	if (!isPathOk(installDir) || !CreateDir(installDir) || !FolderExist(installDir)){
		UC_LOG(INFO) << "OnInstall custom install path failed.";
		return;
	}

	retVal->intVal = 1;
	wcscpy_s(m_szInstallDir, installDir);
	wsprintf(m_szTargetPath, _T("%s\\%s"), m_szInstallDir, TARGET_NAME);

	m_bDeskLnk = _tcstol(params->rgvarg[2].bstrVal, &stopStr, 10);
	m_bQuickStartLnk = _tcstol(params->rgvarg[1].bstrVal, &stopStr, 10);
	m_bAutoRun = _tcstol(params->rgvarg[0].bstrVal, &stopStr, 10);
	KillRelationExe();
	SetEvent(m_hBeginExcute);
}

void InstallWnd::OnUnistall(DISPPARAMS*params, VARIANT*)
{
	UC_LOG(INFO) << "OnUnistall enter.";
	KillRelationExe();
	GetInstallInfoFromReg(TRUE);
	DelSendActionFromRBtn();
	SetEvent(m_hBeginExcute);
}

void InstallWnd::CloseRunningAppbyMsg(TCHAR* lpWndName)
{
	HWND hWnd = FindWindow(NULL, lpWndName);
	if (hWnd != NULL)
	{
		::PostMessage(hWnd, WM_CLOSE, NULL, NULL);
	}
}


BOOL InstallWnd::EnableDebugPrivilege()
{
	HANDLE hToken;
	BOOL fOk = FALSE;
	if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken))
	{
		TOKEN_PRIVILEGES tp;
		tp.PrivilegeCount = 1;
		if (!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &tp.Privileges[0].Luid))
		{
			//LOG_INFO("LookupPrivilegeValue fail");
		}
		tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
		if (!AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(tp), NULL, NULL))
		{
			//LOG_INFO("AdjustTokenPrivileges fail");
		}

		fOk = (GetLastError() == ERROR_SUCCESS);
		CloseHandle(hToken);
	}
	return fOk;
}

void InstallWnd::CloseRunningApp(TCHAR* lpProcName)
{
	EnableDebugPrivilege();
	PROCESSENTRY32 pe = { sizeof(pe) };
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
				if (StrCmpIW(pe.szExeFile, lpProcName) == 0)
				{
					proid = pe.th32ProcessID;

					hprocess = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, proid);

					if (hprocess != NULL)
					{
						UC_LOG(INFO) << "CloseRunningApp Kill process = " << lpProcName;
						::TerminateProcess(hprocess, 0);
						WaitForSingleObject(hprocess, 500);
						UC_LOG(INFO) << "CloseRunningApp Kill process succeed. process name = " << lpProcName;
						::CloseHandle(hprocess);
					}
				}

			} while (Process32Next(hSnapshot, &pe));
		}
		CloseHandle(hSnapshot);
	}
}

DWORD WINAPI InstallWnd::CleanUpThreadProc(__in LPVOID lpParam)
{
	InstallWnd* pWnd = (InstallWnd*)lpParam;
	if (pWnd->m_nRecoverType == 1)
	{
		//清理老版本
		pWnd->UnInstallOldVersion();
		pWnd->CleanOldVersion(FALSE);
	}
// 	else
// 	{
// 		//覆盖新版本：为了防止msi自己的覆盖安装不能完全清理安装目录，特作此辅助清理残留
// 		pWnd->CleanInstallResidues(FALSE);
// 	}
	return 0;
}

void InstallWnd::OnRecoverInstall(DISPPARAMS *params, VARIANT*)
{
	UC_LOG(INFO) << "OnRecoverInstall enter. recover type = " << m_nRecoverType;
	TCHAR strMsg[MAX_PATH] = { L"OnRecoverInstall" };
	//关闭相关进程
	KillRelationExe();

	//从注册表读出之前的安装信息
	GetInstallInfoFromReg();

	//反注册右键菜单
	if (m_nRecoverType > 1)
	{
		DelSendActionFromRBtn();
	}

	//先卸载老版本
	if (m_nRecoverType == 1)
	{
		g_bCloseMainWnd = TRUE;
		UnInstallOldVersion();
		PostMessage(WM_CLOSE);
	}
	else
	{
		SetEvent(m_hBeginExcute);
	}
}

void InstallWnd::UnInstallOldVersion()
{
	UC_LOG(INFO) << "UnInstallOldVersion enter.";
	TCHAR* sProductCode = new TCHAR[MAX_PATH];
	int bRet = GetProductCode(&sProductCode);
	TCHAR szUninstallCmd[MAX_PATH];
	wsprintf(szUninstallCmd, _T("/X %s"), sProductCode);
	HINSTANCE hIns = ShellExecute(NULL, _T("open"), L"msiexec.exe",
		szUninstallCmd, NULL, SW_SHOW);
	UC_LOG(INFO) << "after execute msiexec.exe, error code =  " << GetLastError();
	delete[] sProductCode;
	return;
}


void InstallWnd::OnCheckAppRunning(DISPPARAMS*, VARIANT* retVal)
{
	retVal->vt = VT_BOOL;
	if (FindRunningApp(MAIN_PROCESS) || FindRunningApp(WORKER_PROCESS))
	{
		
		retVal->boolVal = TRUE;
	}
	else {
		retVal->boolVal = FALSE;
	}
}


int InstallWnd::FindRunningApp(TCHAR* lpProcName)
{
	//EnableDebugPrivilege();

	BOOL bRet = FALSE;
	PROCESSENTRY32 pe = { sizeof(pe) };
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
				if (StrCmpIW(pe.szExeFile, lpProcName) == 0)
				{
					proid = pe.th32ProcessID;

					hprocess = ::OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, proid);

					if (hprocess != NULL)
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

BOOL InstallWnd::DeleteFolder(const TCHAR* pszFolder, BOOL bAllowUndo)
{

	if ((NULL == pszFolder))
	{
		return FALSE;
	}

	UC_LOG(INFO) << "DeleteFolder enter. folder path = " << pszFolder;

	int iPathLen = _tcslen(pszFolder);
	if (iPathLen > MAX_PATH)
	{
		return FALSE;
	}

	TCHAR tczFolder[MAX_PATH + 1];
	ZeroMemory(tczFolder, (MAX_PATH + 1)*sizeof(TCHAR));
	wcscpy_s(tczFolder, pszFolder);
	tczFolder[iPathLen] = _T('\0');
	tczFolder[iPathLen + 1] = _T('\0');	//确保目录的路径以2个\0结尾

	SHFILEOPSTRUCT fos;
	ZeroMemory(&fos, sizeof(fos));
	fos.hwnd = NULL;
	fos.wFunc = FO_DELETE;
	fos.fFlags = FOF_NO_UI;		//不显示任何界面
	fos.pFrom = tczFolder;
	fos.pTo = NULL;
	if (bAllowUndo)
	{
		fos.fFlags |= FOF_ALLOWUNDO;	//删除到回收站
	}
	else
	{
		fos.fFlags &= ~FOF_ALLOWUNDO;	//直接删除，不放入回收站
	}

	// 删除文件夹及其内容
	int ret = SHFileOperation(&fos);
	if (0 != ret)
	{
		UC_LOG(ERROR) << "after call SHFileOperation failed, return: " << ret;
		return FALSE;
	}
	return TRUE;
}

BOOL InstallWnd::IsEndWithBackSlashOrSlash(LPCTSTR lpPath)
{
	if (lpPath == NULL)
	{
		return FALSE;
	}

	TCHAR cLastChar = lpPath[_tcslen(lpPath) - 1];
	if (L'\\' == cLastChar || L'/' == cLastChar)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}

}

BOOL InstallWnd::IsEndWithInstallDir(LPCTSTR lpPath)
{
	if (lpPath == NULL)
	{
		return FALSE;
	}


	int len = _tcslen(lpPath);
	int lenTemp = _tcslen(INSTALL_DIR_TEMP);
	//D:\Bee2.0
	if (len < lenTemp+2)
	{
		return FALSE;
	}
	std::wstring s(lpPath);
	s = s.substr(len - lenTemp, lenTemp);
	if (StrCmpIW(s.c_str(), INSTALL_DIR_TEMP) == 0)
	{
		return TRUE;
	}

	return FALSE;
}


void InstallWnd::StopExplorer()
{
	UC_LOG(INFO) << "StopExplorer begin!";

	HANDLE hProcess;
	DWORD dwProcessId;
	GetWindowThreadProcessId(FindWindow(TEXT("Progman"), TEXT("Program Manager")), &dwProcessId);
	hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, dwProcessId);
	TerminateProcess(hProcess, 0);
	WaitForSingleObject(hProcess, 1000);
	CloseHandle(hProcess);

	UC_LOG(INFO) << "StopExplorer end!";
}

void InstallWnd::DeleteFiles(const TCHAR* lpDir, BOOL bDelDir, const TCHAR* lpException)
{

	//在测试过程中发现如果lpDir=""，则会默认为进程所在盘符的根目录，如果在C盘，则默认为C:\\
		    //为了防止此类情况发生，对路径首字母的有效性进行判断
	if (NULL == lpDir ||
		0 == _tcslen(lpDir))
	{
		return;
	}

	//例子："D:\\Test"
	TCHAR szFind[MAX_PATH] = { 0 };

	wcscpy_s(szFind, lpDir); 

	if (!IsEndWithBackSlashOrSlash(szFind))
		wcscat_s(szFind, _T("\\"));

	wcscat_s(szFind, _T("*.*")); //找所有文件 


	WIN32_FIND_DATA wfd;
	HANDLE hFind = FindFirstFile(szFind, &wfd);

	if (hFind == INVALID_HANDLE_VALUE) // 如果没有找到或查找失败 
	{
		return;
	}

	vector<std::wstring> vTools;
	do
	{
		if (wfd.cFileName[0] == _T('.'))
			continue; //过滤这两个目录

		if (NULL != lpException)
		{
			if (NULL != _tcsstr(lpException, wfd.cFileName))
			{
				continue;
			}
		}

		if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{

			TCHAR szFile[MAX_PATH];

			if (IsEndWithBackSlashOrSlash(lpDir))
				wsprintf(szFile, _T("%s%s"), lpDir, wfd.cFileName);
			else
				wsprintf(szFile, _T("%s\\%s"), lpDir, wfd.cFileName);

			DeleteFiles(szFile, bDelDir, lpException); //如果找到的是目录，则进入此目录进行递归

			if (TRUE == bDelDir)
			{
				RemoveDirectory(szFile);
			}
		}
		else
		{

			TCHAR szFile[MAX_PATH];
			if (IsEndWithBackSlashOrSlash(lpDir))
				wsprintf(szFile, _T("%s%s"), lpDir, wfd.cFileName);
			else
				wsprintf(szFile, _T("%s\\%s"), lpDir, wfd.cFileName);

			if (StrCmpIW(wfd.cFileName, _T("rm.exe")) == 0 || StrCmpIW(wfd.cFileName, _T("cygwin1.dll")) == 0
				|| StrCmpIW(wfd.cFileName, _T("cygintl-8.dll")) == 0 || StrCmpIW(wfd.cFileName, _T("cygiconv-2.dll")) == 0
				|| StrCmpIW(wfd.cFileName, _T("cyggcc_s-1.dll")) == 0)
			{
				std::wstring str(szFile);
				vTools.push_back(str);
				continue;
			}

			BOOL bRet = DeleteFile(szFile); //对文件进行操作
			if (bRet == FALSE )
			{
				UC_LOG(ERROR) << "Delete file failed: " << szFile << ". get last error code = " << GetLastError();
				
				//使用rm.exe删除普通API无法删除的文件
				std::wstring sPathTools(m_szInstallDir);
				sPathTools += L"\\tools";
				std::wstring sRM = sPathTools + L"\\rm.exe";
				if (FileExist(sRM.c_str()))
				{
					std::wstringstream ss;
					ss << "\"" << szFile << "\"";					
					ShellExecuteExOpen(_T("rm.exe"), sPathTools, ss.str().c_str(), SW_HIDE, false);
					UC_LOG(INFO) << "After execute rm.exe";
				}
				else
				{
					UC_LOG(INFO) << "not found rm.exe";
				}

				//重启资源管理器以删除被占用的dll
// 				if (StrCmpIW(wfd.cFileName, _T("CopyLocation.dll")) == 0) {
// 					StopExplorer();
// 					bRet = DeleteFile(szFile);
// 					UC_LOG(INFO) << "After restart explorer.exe, delete file return " << bRet;
// 				}
			}
			
		}
	} while (FindNextFile(hFind, &wfd));

	FindClose(hFind); //关闭查找句柄 

	if (TRUE == bDelDir)
	{
		RemoveDirectory(lpDir);
	}

	//最后再删除tools目录
	for (int i = 0; i < vTools.size(); i++) {
		DeleteFile(vTools[i].c_str());
	}
}

void InstallWnd::KillRelationExe()
{
	UC_LOG(INFO) << "KillRelationExe enter.";
	//先发WM_CLOSE消息,再强杀
	CloseRunningAppbyMsg(CLIENT_WINDOW_TITLE);
	CloseRunningApp(MAIN_PROCESS);
	CloseRunningApp(WORKER_PROCESS);
	CloseRunningApp(UPDATE_PROCESS);
	CloseRunningApp(MEETING_PROCESS);
	CloseRunningApp(MEETING_WORKER_PROCESS);
	CloseRunningApp(MEETING_CHAT_PROCESS);
}

//用于卸载或覆盖安装时清理安装目录、log、注册表残留等
void InstallWnd::CleanInstallResidues(BOOL bCleanLog, BOOL bCleanInstallDir)
{
	UC_LOG(INFO) << "CleanInstallResidues enter, bCleanLog = " << bCleanLog << ", bCleanInstallDir = " << bCleanInstallDir;
	//删除安装目录
	if (bCleanInstallDir == TRUE)
	{
		DeleteFiles(m_szInstallDir);
	}
	
	//删除快捷方式
	DeleteLnkFromDesktop(DESKTOP_LNK);
	DeleteLnkFromProgramsMenu(STARTPROGRAM_FLODER_LNK);
	DeleteLnkFromQuickStartMenu(QUICKSTART_LNK);

	//删除log目录
	if (bCleanLog)
	{
		TCHAR moduleFullName[MAX_PATH] = { 0 };
		BOOL bRet = ::SHGetSpecialFolderPath(NULL, moduleFullName, CSIDL_LOCAL_APPDATA, FALSE);
		if (bRet == TRUE && _tcslen(moduleFullName) > 0)
		{
			TCHAR szLogPah[MAX_PATH] = { 0 };
			wsprintf(szLogPah, L"%s%s", moduleFullName, APP_LOG_PATH);
			DeleteFolder(szLogPah);
		}
	}

	//清理注册表
	DeleteInstallInfoFromReg(FALSE);
	AutoStartSoftware(FALSE);
}

//强制清理老蜜蜂
void InstallWnd::CleanOldVersion(BOOL bCleanLog /*= FALSE*/)
{
	UC_LOG(INFO) << "CleanOldVersion enter. bCleanLog = " << bCleanLog;
	
	//删除默认安装目录
	TCHAR szPath[MAX_PATH] = { 0 };
	TCHAR szOldInstallDir[MAX_PATH] = { 0 };
	SHGetSpecialFolderPath(NULL, szPath, CSIDL_PROFILE, FALSE);
	wsprintf(szOldInstallDir, _T("%s%s"), szPath, INSTALL_DIR_OLD);
	DeleteFolder(szOldInstallDir);
	
	//删除当前安装目录
	if (m_nRecoverType == 1)
	{
		DeleteFolder(m_szOldInstallDir);
	}
	
	DeleteFolder(m_szInstallDir);

	//删除快捷方式
	DeleteLnkFromDesktop(DESKTOP_LNK);
	DeleteLnkFromProgramsMenu(STARTPROGRAM_FLODER_LNK);
	DeleteLnkFromQuickStartMenu(QUICKSTART_LNK);

	//删除log目录
	if (bCleanLog)
	{
		TCHAR moduleFullName[MAX_PATH] = { 0 };
		BOOL bRet = ::SHGetSpecialFolderPath(NULL, moduleFullName, CSIDL_LOCAL_APPDATA, FALSE);
		if (bRet == TRUE && _tcslen(moduleFullName) > 0)
		{
			TCHAR szLogPah[MAX_PATH] = { 0 };
			wsprintf(szLogPah, L"%s%s", moduleFullName, APP_LOG_PATH);
			DeleteFolder(szLogPah);
		}
	}

	//清理注册表
	DeleteInstallInfoFromReg(TRUE);

	//关闭开机自启动项
	AutoStartSoftware(FALSE);
}

BOOL InstallWnd::AutoStartSoftware(BOOL bAutoStart)
{
	UC_LOG(INFO) << "AutoStartSoftware enter, bAutoStart = " << bAutoStart;
	HKEY hAppKey = NULL;

	BSTR bsKeyPath = _T("Software\\Microsoft\\Windows\\CurrentVersion\\Run");

	BSTR bsKeyName = REG_AUTO_RUN;
	DWORD dw;

	if (RegCreateKeyEx(HKEY_CURRENT_USER, //开机自启动只针对当前用户
		bsKeyPath,
		0,
		REG_NONE,
		REG_OPTION_NON_VOLATILE,
		KEY_WRITE | KEY_READ,
		NULL,
		&hAppKey,
		&dw) == ERROR_SUCCESS)
	{
		if (bAutoStart)
		{
			DWORD dwType = REG_SZ;
			::RegSetValueEx(hAppKey, bsKeyName, 0, dwType,
				(LPBYTE)m_szTargetPath, _tcslen(m_szTargetPath)*sizeof(TCHAR));
		}
		else
		{
			RegDeleteValue(hAppKey, bsKeyName);
		}
		RegCloseKey(hAppKey);
	}

	return TRUE;
}


BOOL InstallWnd::DeleteLnk(LPCTSTR lpszShortcut)
{
	SHFILEOPSTRUCT fos;
	ZeroMemory(&fos, sizeof(fos));
	fos.hwnd = HWND_DESKTOP;
	fos.wFunc = FO_DELETE;
	fos.pFrom = lpszShortcut;
	fos.pTo = NULL;
	fos.fFlags = FOF_SILENT | FOF_NOCONFIRMATION;
	//删除快捷方式（*.lnk)
	if (0 != SHFileOperation(&fos))
		return FALSE;
	return TRUE;
}


//从注册表获取已安装程序的安装信息
BOOL InstallWnd::GetInstallInfoFromReg(BOOL bOnlyInstallDir)
{
	UC_LOG(INFO) << "GetInstallInfoFromReg enter";
	HKEY hAppKey = NULL;
	TCHAR *bsKeyPath = { REG_SOFTWARE };

	//先检测有没有装老版本，若装过则依照老版本的安装信息覆盖安装
	if (RegOpenKeyEx(HKEY_CURRENT_USER,
		bsKeyPath,
		0,
		KEY_READ,
		&hAppKey) == ERROR_SUCCESS)
	{
		DWORD dwType = REG_SZ;
		DWORD dwSize = MAX_PATH;
		BSTR bsKeyInstall = REG_INSTALLDIR;
		BSTR bsKeyAutoRun = REG_AUTORUN;
		BSTR bsKeyDeskLnk = REG_DESKLNK;
		BSTR bsKeyStartLnk = REG_STARTLNK;
		TCHAR sAutoRun[MAX_PATH] = { 0 };
		TCHAR sRunAfterInstall[MAX_PATH] = { 0 };
		TCHAR sDeskLnk[MAX_PATH] = { 0 };
		TCHAR sStartLnk[MAX_PATH] = { 0 };

		//获取安装目录
		TCHAR* pInstallDir = new TCHAR[MAX_PATH];
		memset(pInstallDir, 0, MAX_PATH);
		LONG result = ::RegQueryValueEx(hAppKey, bsKeyInstall, 0, &dwType, (LPBYTE)pInstallDir, &dwSize);
		if (result == 0 && m_nRecoverType != 1)
		{
			if (m_nRecoverType == 1)
			{
				_tcscpy_s(m_szOldInstallDir, pInstallDir);
			}
			else
			{
				_tcscpy_s(m_szInstallDir, pInstallDir);
			}
			wsprintf(m_szTargetPath, _T("%s\\%s"), m_szInstallDir, TARGET_NAME);
		}

		delete[] pInstallDir;

		if (bOnlyInstallDir == TRUE)
		{ 
			RegCloseKey(hAppKey);
			return TRUE;
		}

		//获取是否开机自启动
		TCHAR* stopStr = NULL;
		dwSize = MAX_PATH;
		result = ::RegQueryValueEx(hAppKey, bsKeyAutoRun, 0, &dwType, (LPBYTE)&sAutoRun, &dwSize);
		if (result == 0)
		{
			m_bAutoRun = _tcstol(sAutoRun, &stopStr, 10);
		}
		

		//获取是否创建桌面LNK
		dwSize = MAX_PATH;
		result = ::RegQueryValueEx(hAppKey, bsKeyDeskLnk, 0, &dwType, (LPBYTE)&sDeskLnk, &dwSize);


		//获取是否创建启动栏LNK
		dwSize = MAX_PATH;
		result = ::RegQueryValueEx(hAppKey, bsKeyStartLnk, 0, &dwType, (LPBYTE)&sStartLnk, &dwSize);
		if (result == 0)
		{
			m_bQuickStartLnk = _tcstol(sStartLnk, &stopStr, 10);
		}		
		
		RegCloseKey(hAppKey);	
		return TRUE;
	}

	return FALSE;
}

BOOL InstallWnd::DeleteLnkFromProgramsMenu(LPCTSTR pszDir)
{
	//删除程序菜单
	TCHAR   szPath[MAX_PATH];
	SHGetSpecialFolderPath(NULL, szPath, CSIDL_PROGRAMS, FALSE);
	wcscat_s(szPath, pszDir);
	DeleteFiles(szPath, TRUE, NULL);
	UC_LOG(INFO) << "DeleteLnkFromProgramsMenu enter. path = " << szPath;
	return RemoveDirectory(szPath);
}

BOOL InstallWnd::DeleteLnkFromQuickStartMenu(LPCTSTR pszLnk)
{
	//删除快速启动菜单
	TCHAR   szPath[MAX_PATH];
	SHGetSpecialFolderPath(NULL, szPath, CSIDL_APPDATA, FALSE);
	wcscat_s(szPath, pszLnk);

	UC_LOG(INFO) << "DeleteLnkFromProgramsMenu enter. path = " << szPath;
	return DeleteFile(szPath);
}

BOOL InstallWnd::DeleteLnkFromDesktop(LPCTSTR pszLnk)
{
	TCHAR   szPath[MAX_PATH];
	//删除桌面快捷方式
	SHGetSpecialFolderPath(NULL, szPath, CSIDL_DESKTOPDIRECTORY, FALSE);
	wcscat_s(szPath, pszLnk);
	UC_LOG(INFO) << "DeleteLnkFromProgramsMenu enter. path = " << szPath;
	return DeleteFile(szPath);
}

BOOL InstallWnd::CreateDesktopLnk(LPCTSTR pszTargetPath, LPCTSTR pszLnk)
{
	//创建桌面快捷方式
	TCHAR   szPath[MAX_PATH] = { 0 };
	SHGetSpecialFolderPath(NULL, szPath, CSIDL_DESKTOPDIRECTORY, FALSE);
	wcscat_s(szPath, pszLnk);
	BOOL bRet = CreateLnk(pszTargetPath, szPath, NULL);
	UC_LOG(INFO) << "CreateDesktopLnk return " << bRet;
	return bRet;
}

BOOL InstallWnd::CreateLnkToSendTo(LPCTSTR pszTargetPath, const TCHAR* pszLnk)
{
	//创建发送到快捷方式
	TCHAR   szPath[MAX_PATH] = { 0 };
	SHGetSpecialFolderPath(NULL, szPath, CSIDL_SENDTO, FALSE);
	wcscat_s(szPath, pszLnk);
	BOOL bRet = CreateLnk(pszTargetPath, szPath, L"--sendto");
	UC_LOG(INFO) << "CreateLnkToSendTo return " << bRet;
	return bRet;
}


BOOL InstallWnd::CreateLnkToProgramsMenu(LPCTSTR pszTargetPath,
	LPCTSTR pszUninstallPath,
	LPCTSTR pszDir,
	LPCTSTR pszUninstallLnk,
	LPCTSTR pszTargetLnk
	)
{
	TCHAR   szPath[MAX_PATH] = { 0 };
	SHGetSpecialFolderPath(NULL, szPath, CSIDL_PROGRAMS, FALSE);
	wcscat_s(szPath, pszDir);
	CreateDirectory(szPath, NULL);
	TCHAR szLink[MAX_PATH] = { 0 };
	wsprintf(szLink, _T("%s%s"), szPath, pszUninstallLnk);
	TCHAR szUninstallCmd[MAX_PATH] = { 0 };
	wsprintf(szUninstallCmd, _T("/X %s /qn"), m_szProductCode);//_T("/X {E8522F1C-5BEC-4261-B1FC-7688D621C7DF} /qn")
	CreateLnk(pszUninstallPath, szLink, szUninstallCmd);
	wcscat_s(szPath, pszTargetLnk);

	UC_LOG(INFO) << "CreateLnkToProgramsMenu, lnkPath = " << szPath;
	return CreateLnk(pszTargetPath, szPath, NULL);;
}

BOOL InstallWnd::CreateLnkToQuickStartMenu(LPCTSTR pszTargetPath,LPCTSTR pszTargetLnk)
{
	TCHAR   szPath[MAX_PATH] = { 0 };
	SHGetSpecialFolderPath(NULL, szPath, CSIDL_APPDATA, FALSE);
	wcscat_s(szPath, pszTargetLnk);

	UC_LOG(INFO) << "CreateLnkToQuickStartMenu, LnkPath = " <<szPath;
	return CreateLnk(pszTargetPath, szPath, NULL);
}

BOOL InstallWnd::DeleteInstallInfoFromReg(BOOL bOld)
{
	UC_LOG(INFO) << "DeleteInstallInfoFromReg enter. bOld = " << bOld;
	if (bOld)
	{
		//清理老版本注册表残留
		TCHAR* productKey = new TCHAR[MAX_PATH];
		if (GetProductCode(&productKey) == TRUE)
		{
			TCHAR bsKeyPath_x32[MAX_PATH] = { 0 };
			TCHAR bsKeyPath_x64[MAX_PATH] = { 0 }; 
			wsprintf(bsKeyPath_x32, L"Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\%s", productKey);
			wsprintf(bsKeyPath_x64, L"SOFTWARE\\Wow6432Node\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\%s", productKey);
			RegDeleteKey(HKEY_LOCAL_MACHINE, bsKeyPath_x32);
			RegDeleteKey(HKEY_LOCAL_MACHINE, bsKeyPath_x64);
		}

		TCHAR bsKeyMeetingFlag[MAX_PATH] = { L"Software\\BeeInstallMeeting" };
		RegDeleteKey(HKEY_CURRENT_USER, bsKeyMeetingFlag);
		delete [] productKey;
	}

	//新版本只有Bee
	TCHAR bsKeyBee[MAX_PATH] = { REG_SOFTWARE };
	//RegDeleteKey(HKEY_CURRENT_USER, bsKeyBee);
	//删除value保留key 
	HKEY hKeyBee;
	if (ERROR_SUCCESS == RegOpenKeyEx(HKEY_CURRENT_USER, bsKeyBee, 0, KEY_ALL_ACCESS, &hKeyBee))
	{
		UC_LOG(INFO) << "DeleteInstallInfoFromReg open Bee key succeed." << bOld;
		RegDeleteValue(hKeyBee, REG_PRODEUCTCODE);
		RegDeleteValue(hKeyBee, REG_AUTORUN);
		RegDeleteValue(hKeyBee, REG_DESKLNK);
		RegDeleteValue(hKeyBee, REG_INSTALLDIR);
		RegDeleteValue(hKeyBee, REG_STARTLNK);
		RegDeleteValue(hKeyBee, REG_VERSION);
		RegDeleteValue(hKeyBee, REG_PACKAGECODE);
		RegCloseKey(hKeyBee);
	}
	return TRUE;
}

BOOL InstallWnd::CreateLnk(LPCTSTR szPath, LPCTSTR szLink, LPCTSTR szParam)
{
	HRESULT hres;
	IShellLink * psl;
	IPersistFile* ppf;
	int nBeginAt = CSIDL_DESKTOPDIRECTORY;

	//UC_LOG(INFO) << "CreateLnk, szPath = " << (szPath != NULL ? szPath : _T("")) << ", szLink = " << (szLink != NULL ? szLink : _T("")) << ", szParam = " << (szParam != NULL ? szParam : _T(""));
	CoInitialize(NULL);

	//创建一个IShellLink实例  
	hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (void **)&psl);
	if (FAILED(hres))   return FALSE;

	//设置目标应用程序 
	std::wstringstream ss;
	std::wstring strPath(szPath);
	ss << "\"" << strPath << "\"";
	psl->SetPath(ss.str().c_str());
	
	//获取工作路径  
	if (std::wstring::npos != strPath.find_last_of(L"\\")){
		strPath = strPath.substr(0, strPath.find_last_of(L"\\"));
	}

	ss.clear();
	ss << "";
	ss << "\"" << strPath << "\"";
	psl->SetWorkingDirectory(ss.str().c_str());
	
	//从IShellLink获取其IPersistFile接口  
	//用于保存快捷方式的数据文件 (*.lnk)  
	hres = psl->QueryInterface(IID_IPersistFile, (void**)&ppf);

	if (FAILED(hres))
	{
		UC_LOG(ERROR) << "CreateLnk, QueryInterface failed! error code = " <<  GetLastError();
		return FALSE;
	}
	psl->SetArguments(szParam);

	// 	TCHAR sIconPath[MAX_PATH] = { 0 };
	// 	wsprintf(sIconPath, L"%s\\resources\\html\\images\\bee.ico", strAppPath);
	// 	psl->SetIconLocation(sIconPath, 0);
	// 	UC_LOG(ERROR) << "CreateLnk, seticonlocation icon path = " << sIconPath << ", error code = " << GetLastError();

	//调用IPersist:Save  
	//保存快捷方式的数据文件 (*.lnk)  
	hres = ppf->Save((LPWSTR)szLink, STGM_READWRITE);
	if (FAILED(hres))
	{
		UC_LOG(ERROR) << "CreateLnk, Save failed! error code = " << GetLastError();
	}
	//释放IPersistFile和IShellLink接口  
	ppf->Release();
	psl->Release();
	return TRUE;
}

//创建各快捷方式
void InstallWnd::CreateAllLnk()
{
	UC_LOG(INFO) << "CreateAllLnk enter. bDeskLnk = " << m_bDeskLnk << ", bQuickStartLnk = " << m_bQuickStartLnk <<", bAutoRun = " << m_bAutoRun;
	if (m_bDeskLnk)
	{
		CreateDesktopLnk(m_szTargetPath, DESKTOP_LNK);
	}

	//程序列表
	TCHAR szMsiexecPath[MAX_PATH] = { 0 };
	SHGetSpecialFolderPath(NULL, szMsiexecPath, CSIDL_SYSTEM, FALSE);
	wsprintf(szMsiexecPath, _T("%s%s"), szMsiexecPath, MSIEXEC);
	CreateLnkToProgramsMenu(m_szTargetPath, szMsiexecPath,
		STARTPROGRAM_FLODER_LNK, STARTPROGRAM_UNINSTALL_LNK, STARTPROGRAM_LNK);

	//快速启动栏
	if (m_bQuickStartLnk) {
		CreateLnkToQuickStartMenu(m_szTargetPath, QUICKSTART_LNK);
	}
	
	//开机自启动
	AutoStartSoftware(m_bAutoRun);
}


void InstallWnd::OnSelectFolderDialog(DISPPARAMS* params, VARIANT* retVal)
{
	retVal->vt = VT_BSTR;

	ATLASSERT(params->cArgs == 1);

	HRESULT hr = S_OK;
	BROWSEINFO bi;

	//init the browser info struct.
	bi.hwndOwner = m_hWnd;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = NULL;
	bi.lpszTitle = params->rgvarg[0].bstrVal;
	//BIF_EDITBOX
	bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;
	bi.lpfn = NULL;
	bi.lParam = 0;
	bi.iImage = 0;
	LPITEMIDLIST pList = NULL;
	BOOL bExistFloder = FALSE;
	BOOL bNewCreate = FALSE;
	TCHAR szSelPath[MAX_PATH] = { 0 };
	ZeroMemory(szSelPath, MAX_PATH);

	if ((pList = SHBrowseForFolder(&bi)) != NULL)
	{
		SHGetPathFromIDList(pList, szSelPath);
		int nlen = _tcslen(szSelPath);
		//文件目录过深或文件夹名称过长
		if (nlen > 150)
		{
			hr = E_FAIL;
			retVal->bstrVal = ::SysAllocString(_T(""));
			goto exit;
		}

		if (_tcslen(szSelPath) <= 3)
		{
			szSelPath[_tcslen(szSelPath) - 1] = _T('\0');
		}

		//判断是否有读写权限
		if (0 != _taccess(szSelPath, 06))
		{
			hr = E_ACCESSDENIED;
			retVal->bstrVal = ::SysAllocString(_T(""));
			goto exit;
		}

		if (!IsEndWithInstallDir(szSelPath))
		{
			wsprintf(szSelPath, _T("%s%s"), szSelPath, INSTALL_DIR_TEMP);
		}

		bExistFloder = FolderExist(szSelPath);

		if (!bExistFloder)
		{
			CreateDir(szSelPath);
			bExistFloder = FolderExist(szSelPath);
			bNewCreate = TRUE;
		}

		if (bExistFloder)
		{
			//判断是否有权限访问
			struct _stat statbuf;
			int ret = _wstat(szSelPath, &statbuf);
			if (ret != 0 || !(statbuf.st_mode & S_IREAD) || !(statbuf.st_mode & S_IWRITE)) {
				hr = E_ACCESSDENIED;
				retVal->bstrVal = ::SysAllocString(_T(""));
				goto exit;
			}
			
			wcscpy_s(m_szInstallDir, szSelPath);
			wsprintf(m_szTargetPath, _T("%s\\%s"), m_szInstallDir, TARGET_NAME);
			retVal->bstrVal = ::SysAllocString(m_szInstallDir);

		}
		else
		{
			hr = E_ACCESSDENIED;
			retVal->bstrVal = ::SysAllocString(_T(""));
			goto exit;
		}
	}
	else
	{
		retVal->bstrVal = ::SysAllocString(m_szInstallDir);
	}

	UC_LOG(INFO) << "After OnSelectFolderDialog, install directory = " << m_szInstallDir;
exit:
	//删除自己创建的文件夹
	if (bExistFloder == TRUE && bNewCreate == TRUE)
	{
		DeleteFolder(szSelPath);
	}
	return;
}

// 判断指定的目录是否存在
BOOL InstallWnd::FolderExist(LPCWSTR strPath)
{
	if (strPath == NULL)
	{
		return FALSE;
	}
	WIN32_FIND_DATA   wfd;
	BOOL rValue = FALSE;

	HANDLE hFind = FindFirstFile(strPath, &wfd);
	if ((hFind != INVALID_HANDLE_VALUE) && (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
	{
		rValue = TRUE;
	}
	FindClose(hFind);
	return rValue;
}


BOOL InstallWnd::CreateDir(LPCWSTR strDesPath)
{
	if (strDesPath == NULL)
	{
		return FALSE;
	}

	BOOL bRet = FALSE;
	DWORD dw = 0;

	if (!FolderExist(strDesPath))
	{
		std::wstring strCreatePath(strDesPath);
		std::wstring sTemp(strDesPath);
		if (std::wstring::npos != sTemp.find_last_of(L"\\"))
		{
			sTemp = sTemp.substr(0, sTemp.find_last_of(L"\\"));
			CreateDir(sTemp.c_str());
		}

		BOOL bRet = CreateDirectory(strCreatePath.c_str(), NULL);
		if (!bRet)
		{
			dw = GetLastError();
			UC_LOG(ERROR) << "CreateDirectory failed, path = " << strCreatePath << ", error code = " << dw;
			return FALSE;
		}
	}

	return TRUE;
}

//记录安装信息到注册表，覆盖安装时直接沿用
BOOL InstallWnd::WriteInstallInfoToReg()
{
	UC_LOG(INFO) << "WriteInstallInfoToReg enter. install dir = " << m_szInstallDir << ", product code = " << m_szProductCode;
	HKEY hAppKey = NULL;

	BSTR bsKeyPath = REG_SOFTWARE;
	DWORD dw;

	BSTR bsKeyInstall = REG_INSTALLDIR;
	BSTR bsKeyProductCode = REG_PRODEUCTCODE;
	BSTR bsKeyPackageCode = REG_PACKAGECODE;
	BSTR bsKeyVersion = REG_VERSION;
	BSTR bsKeyAutoRun = REG_AUTORUN;
	BSTR bsKeyDeskLnk = REG_DESKLNK;
	BSTR bsKeyStartLnk = REG_STARTLNK;
	TCHAR sAutoRun[MAX_PATH] = { 0 };
	TCHAR sDeskLnk[MAX_PATH] = { 0 };
	TCHAR sStartLnk[MAX_PATH] = { 0 };
	wsprintf(sAutoRun, _T("%d"), m_bAutoRun);
	wsprintf(sDeskLnk, _T("%d"), m_bDeskLnk);
	wsprintf(sStartLnk, _T("%d"), m_bQuickStartLnk);

	if (RegCreateKeyEx(HKEY_CURRENT_USER,
		bsKeyPath,
		0,
		REG_NONE,
		REG_OPTION_NON_VOLATILE,
		KEY_WRITE | KEY_READ,
		NULL,
		&hAppKey,
		&dw) == ERROR_SUCCESS)
	{
		DWORD dwType = REG_SZ;
		::RegSetValueEx(hAppKey, bsKeyInstall, 0, dwType, (LPBYTE)m_szInstallDir, _tcslen(m_szInstallDir)*sizeof(TCHAR));
		::RegSetValueEx(hAppKey, bsKeyVersion, 0, dwType, (LPBYTE)m_szVersion, _tcslen(m_szVersion)*sizeof(TCHAR));
		::RegSetValueEx(hAppKey, bsKeyProductCode, 0, dwType, (LPBYTE)m_szProductCode, _tcslen(m_szProductCode)*sizeof(TCHAR));
		::RegSetValueEx(hAppKey, bsKeyPackageCode, 0, dwType, (LPBYTE)m_szPackageCode, _tcslen(m_szPackageCode)*sizeof(TCHAR));
		::RegSetValueEx(hAppKey, bsKeyAutoRun, 0, dwType, (LPBYTE)sAutoRun, sizeof(TCHAR));
		::RegSetValueEx(hAppKey, bsKeyDeskLnk, 0, dwType, (LPBYTE)sDeskLnk, sizeof(TCHAR));
		::RegSetValueEx(hAppKey, bsKeyStartLnk, 0, dwType, (LPBYTE)sStartLnk, sizeof(TCHAR));

		RegCloseKey(hAppKey);
		UC_LOG(INFO) << "WriteInstallInfoToReg succeeded.";
	}

	AddSendActionOnRBtn();


	return TRUE;
}

void InstallWnd::SaveInstallCode(char szProductCode[MAX_PATH], char szPackageCode[MAX_PATH], char szVersion[MAX_PATH])
{
	TCHAR* lpsBuffer = new TCHAR[MAX_PATH];
	memset(lpsBuffer, 0, MAX_PATH);
	int nLen = ::MultiByteToWideChar(CP_ACP, 0, szProductCode, (int)strlen(szProductCode),
		lpsBuffer, MAX_PATH);
	if (nLen > 0)
	{
		_tcscpy_s(m_szProductCode, lpsBuffer);
	}

	memset(lpsBuffer, 0, MAX_PATH);
	nLen = ::MultiByteToWideChar(CP_ACP, 0, szPackageCode, (int)strlen(szPackageCode),
		lpsBuffer, MAX_PATH);
	if (nLen > 0)
	{
		_tcscpy_s(m_szPackageCode, lpsBuffer);
	}

	memset(lpsBuffer, 0, MAX_PATH);
	nLen = ::MultiByteToWideChar(CP_ACP, 0, szVersion, (int)strlen(szVersion),
		lpsBuffer, MAX_PATH);
	if (nLen > 0)
	{
		_tcscpy_s(m_szVersion, lpsBuffer);
	}

	delete[] lpsBuffer;
}

BOOL InstallWnd::GetPackageCode(TCHAR** ppPackageCode)
{
	HKEY hAppKey = NULL;

	TCHAR *bsKeyPath = { REG_SOFTWARE };

	if (RegOpenKeyEx(HKEY_CURRENT_USER,
		bsKeyPath,
		0,
		KEY_QUERY_VALUE,
		&hAppKey) == ERROR_SUCCESS)
	{
		DWORD dwType = REG_SZ;
		DWORD dwSize = MAX_PATH;
		TCHAR *bsKeyPackageCode = REG_PACKAGECODE;

		//获取产品ID信息
		if (::RegQueryValueEx(hAppKey, bsKeyPackageCode, 0, &dwType, (LPBYTE)*ppPackageCode, &dwSize) == ERROR_SUCCESS)
		{
			RegCloseKey(hAppKey);
			return TRUE;
		}
		RegCloseKey(hAppKey);
		return FALSE;
	}

	return FALSE;
}

BOOL InstallWnd::GetVersionCode(TCHAR** ppVersionCode)
{
	HKEY hAppKey = NULL;

	TCHAR *bsKeyPath = { REG_SOFTWARE };

	if (RegOpenKeyEx(HKEY_CURRENT_USER,
		bsKeyPath,
		0,
		KEY_QUERY_VALUE,
		&hAppKey) == ERROR_SUCCESS)
	{
		DWORD dwType = REG_SZ;
		DWORD dwSize = MAX_PATH;
		TCHAR *bsKeyVersionCode = REG_VERSION;

		//获取产品ID信息
		if (::RegQueryValueEx(hAppKey, bsKeyVersionCode, 0, &dwType, (LPBYTE)*ppVersionCode, &dwSize) == ERROR_SUCCESS)
		{
			RegCloseKey(hAppKey);
			return TRUE;
		}
		RegCloseKey(hAppKey);
		return FALSE;
	}

	return FALSE;
}

// 安装云会议
BOOL InstallWnd::InstallMeeting()
{
	//从安装目录调起ucmeeting的安装程序
	TCHAR sMeetingExe[MAX_PATH] = { 0 };
	wsprintf(sMeetingExe, _T("%s\\%s"), m_szInstallDir, MEETING_SETUP);
	UC_LOG(INFO) << "InstallMeeting enter. meeting installer path = " << sMeetingExe;
	if (FALSE == FileExist(sMeetingExe))
	{
		UC_LOG(ERROR) << "InstallMeeting UCCMeetingSetup.exe is not exist!";
		return FALSE;
	}

	CloseRunningApp(MEETING_PROCESS);
	CloseRunningApp(MEETING_WORKER_PROCESS);
	CloseRunningApp(MEETING_CHAT_PROCESS);
	SHELLEXECUTEINFO shex;
	LPCTSTR pszParameters = NULL;
	LPCTSTR pszDirectory = NULL;
	memset(&shex, 0, sizeof(shex));

	shex.cbSize = sizeof(SHELLEXECUTEINFO);
	shex.fMask = 0;
	shex.hwnd = NULL;
	shex.lpVerb = L"open";
	shex.lpFile = sMeetingExe;
	shex.lpParameters = L" /S";
	shex.lpDirectory = pszDirectory;
	shex.nShow = SW_HIDE;

	BOOL bRet = ::ShellExecuteEx(&shex);
	if (bRet == FALSE)
	{
		UC_LOG(ERROR) << "InstallMeeting, ShellExecuteEx return " << bRet << ", GetLastError =!" << GetLastError();
		return FALSE;
	}

	//TCHAR bsKeyBee[MAX_PATH] = { MEETING_REG_FLAG };
	//RegDeleteKey(HKEY_CURRENT_USER, bsKeyBee);
	UC_LOG(INFO) << "success in execute meeting installer!";
	return TRUE;
}

BOOL InstallWnd::CheckMeetingInstalled()
{
	UC_LOG(INFO) << "CheckMeetingInstalled enter.";
	HKEY hAppKey = NULL;
	TCHAR *bsKeyPath = { MEETING_REG_FLAG };

	if (RegOpenKeyEx(HKEY_CURRENT_USER,
		bsKeyPath,
		0,
		KEY_READ,
		&hAppKey) == ERROR_SUCCESS)
	{
		char dwValue[256];
		DWORD dwType = REG_DWORD;
		DWORD dwSize = sizeof(dwValue);
		if (::RegQueryValueEx(hAppKey, MEETING_REG_INSTALLFLAG, 0, &dwType, (LPBYTE)&dwValue, &dwSize) == ERROR_SUCCESS)
		{
			RegCloseKey(hAppKey);
			UC_LOG(INFO) << "CheckMeetingInstalled result is TRUE.";
			return TRUE;
		}
		RegCloseKey(hAppKey);
	}

	UC_LOG(INFO) << "CheckMeetingInstalled result is FALSE.";
	return FALSE;
}




//卸载云会议，
BOOL InstallWnd::UninstallMeeting()
{
	//判断当前是否有会议端正在运行
	CloseRunningApp(MEETING_PROCESS);
	CloseRunningApp(MEETING_WORKER_PROCESS);
	CloseRunningApp(MEETING_CHAT_PROCESS);

	//获取卸载程序路径并静默启动。
	TCHAR moduleFullName[MAX_PATH] = { 0 };
	HRESULT hr = ::SHGetSpecialFolderPath(NULL, moduleFullName, CSIDL_PROFILE, FALSE);
	if (_tcslen(moduleFullName) <= 0)
		return FALSE;

	TCHAR sPath[MAX_PATH] = { 0 };
	wsprintf(sPath, _T("%s%s"), moduleFullName, MEETING_UNINSTALL);
	
	UC_LOG(INFO) << "UninstallMeeting enter. uninst.exe path = " << sPath;

	STARTUPINFO si;
	::memset(&si, 0, sizeof(si));
	si.cb = sizeof(si);
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_HIDE;
	PROCESS_INFORMATION pi;
	TCHAR szCommandLine[] = L" /S";
	BOOL isSucceed = CreateProcess(
		sPath, szCommandLine, NULL,
		NULL, FALSE, CREATE_NO_WINDOW, NULL,
		NULL, &si, &pi);
	if (isSucceed)
	{
		UC_LOG(INFO) << "Uninstall Meeting succeeded.";
		return TRUE;
	}
	else
	{
		UC_LOG(ERROR) << "Uninstall Meeting failed. error code = " << GetLastError();
		return FALSE;
	}
}


BOOL InstallWnd::FileExist(LPCWSTR strPath)
{
	WIN32_FIND_DATA   wfd;
	BOOL rValue = FALSE;

	HANDLE hFind = FindFirstFile(strPath, &wfd);
	if ((hFind != INVALID_HANDLE_VALUE))
	{
		rValue = TRUE;
	}
	FindClose(hFind);
	return rValue;
}

BOOL InstallWnd::WriteInstallMeetingFlag()
{
	HKEY hAppKey = NULL;
	TCHAR *bsKeyPath = { REG_INSTALLMEETING_FLAG };
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

float InstallWnd::GetDeviceScaleFactor()
{
	static float scale_factor = 1.0;
	static bool initialized = false;

	if (!initialized) {
		// This value is safe to cache for the life time of the app since the user
		// must logout to change the DPI setting. This value also applies to all
		// screens.
		HDC screen_dc = ::GetDC(NULL);
		int dpi_x = GetDeviceCaps(screen_dc, LOGPIXELSX);
		float temp_scale_factor = static_cast<float>(dpi_x) / 96.0f;
		::ReleaseDC(NULL, screen_dc);

		int MainWndHeight = m_height;
		int screenHeight = ::GetSystemMetrics(SM_CYSCREEN);
		while (screenHeight < temp_scale_factor * MainWndHeight && temp_scale_factor > 1.0) {
			temp_scale_factor -= 0.25;
		}

		float optimation_rate = (screenHeight >= 1080) ? 0.9 : 0.8;
		if (screenHeight * optimation_rate < temp_scale_factor * MainWndHeight) {
			temp_scale_factor -= 0.25;
		}

		if (temp_scale_factor < 1.0) {
			temp_scale_factor = 1.0;
		}

		scale_factor = temp_scale_factor;
		initialized = true;
	}

	return scale_factor;
}

BOOL InstallWnd::UnZipThirdPartyFile() {

	UC_LOG(INFO) << "UnZipThirdPartyFile, install directory = " << m_szInstallDir;
	TCHAR zipFilePath[MAX_PATH] = { 0 };
	wsprintf(zipFilePath, _T("%s%s"), m_szInstallDir, L"\\Conference.zip");
	// zip解压
	if (_waccess(zipFilePath, 0) != -1)
	{
		SetCurrentDirectory(m_szInstallDir);
		HZIP hz = OpenZip((LPTSTR)(LPCTSTR)zipFilePath, 0, ZIP_FILENAME);
		if (hz == NULL)
		{
			UC_LOG(ERROR) << "open zip failed. path = " << zipFilePath;
			return FALSE;
		}

		ZIPENTRYW ze;
		GetZipItem(hz, -1, &ze);
		int numitems = ze.index;
		UC_LOG(INFO) << "zip file number = " << numitems;
		for (int i = 0; i < numitems; i++)
		{
			GetZipItem(hz, i, &ze);
			DWORD res = UnzipItem(hz, i, ze.name, 0, ZIP_FILENAME);
			UC_LOG(INFO) << "UnzipItem return: " << res;
			SetFileAttributes(ze.name, FILE_ATTRIBUTE_NORMAL);
		}
		CloseZip(hz);

		DeleteFile(zipFilePath);
		return TRUE;
	} 

	UC_LOG(INFO) << "not found zip file, path = " << zipFilePath;
	return TRUE;
}

bool InstallWnd::IsOsVersionVistaOrGreater()
{
	OSVERSIONINFOEX    ovex;
	CHAR  szVersionInfo[1024];
	*szVersionInfo = '\x00';
	//设置参数的大小，调用并判断是否成功
	ovex.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	if (!GetVersionEx((LPOSVERSIONINFO)(&ovex)))
	{
		UC_LOG(ERROR)  << "check OS version failed.";
		return false;
	}

	UC_LOG(INFO) << "check OS version success, major version = " << ovex.dwMajorVersion;
	//通过版本号，判断是否是vista及之后版本
	if (ovex.dwMajorVersion > 5)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//检查并根据系统版本选择打开程序方式
void InstallWnd::ShellExecuteExOpen(const std::wstring& appName, std::wstring& appPath, const std::wstring& parameters, int showMode /*= SW_SHOWNORMAL*/, bool isAdmin /*= false*/, bool isSync /*= true*/, int timeout /*= INFINITE*/)
{
	if (IsOsVersionVistaOrGreater())
	{
		SHELLEXECUTEINFO sei = { sizeof(SHELLEXECUTEINFO) };
		sei.fMask = SEE_MASK_NOCLOSEPROCESS;
		if (isAdmin) {
			sei.lpVerb = TEXT("runas");
		}	
		sei.lpFile = appName.c_str();
		sei.lpDirectory = appPath.c_str();
		sei.lpParameters = parameters.c_str();
		sei.nShow = showMode;
		if (ShellExecuteEx(&sei))
		{
			UC_LOG(INFO) << "ShellExecuteEx create process: [" << appName << "] success. waiting for execute finish...";
			if (isSync)
			{
				WaitForSingleObject(sei.hProcess, timeout);
				CloseHandle(sei.hProcess);
			}
			UC_LOG(INFO) << "the process: [" << appName << "] execute finish.";
		}
		else
		{
			DWORD dwStatus = GetLastError();
			if (dwStatus == ERROR_CANCELLED)
			{
				UC_LOG(ERROR) << "ShellExecuteEx failed, privilege failed";
			}
			else if (dwStatus == ERROR_FILE_NOT_FOUND)
			{
				UC_LOG(ERROR) << "ShellExecuteEx failed, execute file not found.";
			}
		}
	}
	else
	{
		//str_replace(appPath, wstring(L"\\"), wstring(L"\\\\"));
		//ShellExecute(NULL, _T("open"), appName.c_str(), parameters.c_str(), appPath.c_str(), SW_SHOWNORMAL);

		PROCESS_INFORMATION ProcessInfo;
		STARTUPINFO StartupInfo;
		ZeroMemory(&StartupInfo, sizeof(StartupInfo));
		StartupInfo.cb = sizeof StartupInfo;
		//Only compulsory field
		std::wstringstream ss;
		ss << appPath << "\\" << appName;
		if (CreateProcessW((LPWSTR)ss.str().c_str(), (LPWSTR)parameters.c_str(), NULL, NULL, FALSE, 0, NULL, (LPWSTR)appPath.c_str(), &StartupInfo, &ProcessInfo))
		{
			UC_LOG(INFO) << "CreateProcessW create process: [" << appName << "] success. waiting for execute finish...";
			if (isSync)
			{
				WaitForSingleObject(ProcessInfo.hProcess, timeout);
				CloseHandle(ProcessInfo.hThread);
				CloseHandle(ProcessInfo.hProcess);			
			}
			UC_LOG(INFO) << "the process: [" << appName << "] execute finish.";
		}
		else
		{
			UC_LOG(ERROR) << "The process could not be started. process name = " << appName << ", directory =" << appPath << ", error code = " << GetLastError();
		}
	}
}


void InstallWnd::AddSendActionOnRBtn() 
{
	UC_LOG(INFO) << "AddSendActionOnRBtn has called.";
	std::wstring parameter = L"type:0" + std::wstring(m_szInstallDir);
	ShellExecuteExOpen(std::wstring(REGMANGER_PROCESS), std::wstring(m_szInstallDir), parameter);
}

void InstallWnd::DelSendActionFromRBtn() {
	UC_LOG(INFO) << "DelSendActionFromRBtn has called.";

	TCHAR sPath[MAX_PATH] = { 0 };
	wsprintf(sPath, _T("%s\\%s"), m_szInstallDir, REGMANGER_PROCESS);
	if (FALSE == FileExist(sPath)) {
		UC_LOG(ERROR) << "not found BeeRegMgr.exe.";
		return;
	}

	std::wstring parameter = L"type:1" + std::wstring(m_szInstallDir);
	ShellExecuteExOpen(std::wstring(REGMANGER_PROCESS), std::wstring(m_szInstallDir), parameter);
}

bool InstallWnd::isPathOk(TCHAR* path)
{
	if (path == NULL)
	{
		return false;
	}

	int len = _tcslen(path);
	if (len < 3)
	{
		return false;
	}

	for (int i = 0; i < len; i++)
	{

		TCHAR ch = *(path + i);
		if (i == 0 && !IS_CHARACTER(ch))
		{
			return false;
		}
		if (i==1 && ch != ':')
		{
			return false;
		}
		if (i==2 && !(ch == '\\' || ch == '\/'))
		{
			return false;
		}

		if (IS_SPECIAL_CHARACTER(ch))
		{
			return false;
		}

	}

	return true;
}

std::wstring InstallWnd::GetTextWithKey(const TCHAR* lpKey)
{
	if (m_IniPath.length() <= 0)
	{
		TCHAR exeFullPath[MAX_PATH] = { 0 };
		GetModuleFileName(NULL, exeFullPath, MAX_PATH);
		std::wstring strIni(exeFullPath);
		strIni = strIni.substr(0, strIni.find_last_of('\\'));
		strIni.append(_T("\\text.ini"));
		m_IniPath = strIni;

		UC_LOG(INFO) << L"ini path = " << strIni.c_str();
	}

	TCHAR val[MAX_PATH] = { 0 };
	GetPrivateProfileString(_T("TEXT"), lpKey, _T(""), val, MAX_PATH, m_IniPath.c_str());
	std::wstring s(val);
	return s;
}
