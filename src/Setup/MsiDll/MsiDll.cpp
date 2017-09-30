#include <STDIO.H>
#include <WINDOWS.H>
#include <tlhelp32.h>
#include <shlobj.h>
#include "XUnzip.h"

#define PROCESS_MSG 1233
BOOL IsVista();
BOOL GetProductCode( TCHAR** ppProductCode );
//HRESULT GetElevationType(TOKEN_ELEVATION_TYPE * ptet);
//HRESULT IsElevated(BOOL * pbElevated);
#define SETUP_WINDOW_TITLE "QuanshiBee"

extern "C" int _declspec(dllexport) IsAppRun(char lpProcName[MAX_PATH])  
{
	OutputDebugString("StartMsiDLL");
	PROCESSENTRY32 pe = {sizeof(pe)};
	int nRet;

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
				if(strcmp(pe.szExeFile, lpProcName) == 0)	
				{
					//printf("进程名称：%s\n",pe.szExeFile);
					CloseHandle(hSnapshot);
					OutputDebugString("Find CloseHandle");
					return 1;
				}
				
            } while (Process32Next(hSnapshot, &pe));    
        }

		OutputDebugString("Not Find CloseHandle");
        CloseHandle(hSnapshot);
    }
	return 0;  
}

extern "C" BOOL _declspec(dllexport) DeleteServer(char lpServerName[MAX_PATH])
{
	SERVICE_STATUS ssStatus; 

	SC_HANDLE sch=::OpenSCManager(NULL,NULL,SC_MANAGER_CREATE_SERVICE);
    if( NULL==sch )
	{
		OutputDebugString("openSCManager failed!");

		int nSCManagerError = GetLastError();
		if (nSCManagerError == ERROR_ACCESS_DENIED)
		{
			OutputDebugString("ERROR_ACCESS_DENIED");
		}
		else if (nSCManagerError == ERROR_DATABASE_DOES_NOT_EXIST)
		{
			OutputDebugString("ERROR_DATABASE_DOES_NOT_EXIST");
		}
		else if (nSCManagerError == ERROR_INVALID_PARAMETER)
		{
			OutputDebugString("ERROR_INVALID_PARAMETER");
		}
		else
		{
			OutputDebugString("I dont known");
		}
        return false;
    }
    SC_HANDLE sc=::OpenService(sch,lpServerName,SERVICE_ALL_ACCESS);
    if( NULL==sc )
	{
		OutputDebugString("open services failed!!");
        ::CloseHandle(sch);
        ::CloseHandle(sc);
        return false;
    }

	if (!ControlService(sc,SERVICE_CONTROL_STOP,&ssStatus))
	{
		OutputDebugString("Stop services failed!!");
		return false;
	}
	
	
    if(::DeleteService(sc))
	{
        OutputDebugString("Delete Success");
    }
	else
	{
        OutputDebugString("Delete Error");
    }

	CloseServiceHandle(sch);
	CloseServiceHandle(sc);

    return true;
}


extern "C" BOOL _declspec(dllexport) RunAdminExe(char lpExePath[MAX_PATH], char lpCMD[MAX_PATH])
{
	OutputDebugString("RunAdminExe Start!!");

	BOOL bIsVista = IsVista();

	if (bIsVista == FALSE)
	{
		//如果是XP系统，则不需要调用exe去提升权限
		OutputDebugString("Not Vista");
		return FALSE;
	}

	OutputDebugString("Is Vista");

	//Vista以上的系统，需要查看当前系统是否有Admin权限
	
	BOOL bElevated = FALSE;
	
// 	if ( FAILED( ::IsElevated( &bElevated ) ) )
// 	{
// 		OutputDebugString("IsElevated Fail");
// 		return FALSE;
// 	}
// 
// 	if (bElevated == TRUE)
// 	{
// 		//bElevated == TRUE不需要提升权限
// 		OutputDebugString("Do not Need Elevated");
// 		return FALSE;
// 	}

	//需要提升权限
	OutputDebugString("Need Elevated");

	SHELLEXECUTEINFO shex;

	LPCTSTR pszParameters = NULL;
	LPCTSTR pszDirectory = NULL;
	
	memset( &shex, 0, sizeof( shex) );
	
	shex.cbSize			= sizeof( SHELLEXECUTEINFO ); 
	shex.fMask			= 0; 
	shex.hwnd			= NULL;

	if (bIsVista == FALSE)
	{
		shex.lpVerb			= "open"; 
	}
	else
	{
		shex.lpVerb			= "runas"; 
	}
	
	shex.lpFile			= lpExePath;
	shex.lpParameters	= lpCMD; 
	shex.lpDirectory	= pszDirectory; 
	shex.nShow			= SW_NORMAL; 
	
	OutputDebugString("ShellExecuteEx Start!!");
	return ::ShellExecuteEx(&shex);
}

BOOL IsVista()
{
	OSVERSIONINFO osver;
	
	osver.dwOSVersionInfoSize = sizeof( OSVERSIONINFO );
	
	if (	::GetVersionEx( &osver ) && 
		osver.dwPlatformId == VER_PLATFORM_WIN32_NT && 
		(osver.dwMajorVersion >= 6 ) )
		return TRUE;
	
	return FALSE;
}

// HRESULT GetElevationType(TOKEN_ELEVATION_TYPE * ptet)
// {
// 	HRESULT hResult = E_FAIL;
// 	HANDLE hToken	= NULL;
// 	
// 	if ( !::OpenProcessToken( 
// 		::GetCurrentProcess(), 
// 		TOKEN_QUERY, 
// 		&hToken ) )
// 	{
// 		OutputDebugString("GetElevationType OpenProcessToken Error");
// 		return hResult;
// 	}
// 	
// 	DWORD dwReturnLength = 0;
// 	
// 	if ( !::GetTokenInformation(
// 		hToken,
// 		TokenElevationType,
// 		ptet,
// 		sizeof( *ptet ),
// 		&dwReturnLength ) )
// 	{
// 		OutputDebugString("GetElevationType GetTokenInformation Error");
// 	}
// 	else
// 	{
// 		hResult = S_OK;
// 	}
// 	
// 	::CloseHandle( hToken );
// 	
// 	return hResult;
// }

// HRESULT IsElevated(BOOL * pbElevated)
// {
// 	OutputDebugString("Enter IsElevated");
// 	
// 	HRESULT hResult = E_FAIL;
// 	HANDLE hToken	= NULL;
// 	
// 	if ( !::OpenProcessToken( 
// 		::GetCurrentProcess(), 
// 		TOKEN_QUERY, 
// 		&hToken ) )
// 	{
// 		OutputDebugString("IsElevated OpenProcessToken Error");
// 		return hResult;
// 	}
// 	
// 	TOKEN_ELEVATION te = { 0 };
// 	DWORD dwReturnLength = 0;
// 	
// 	if ( !::GetTokenInformation(
// 		hToken,
// 		TokenElevation,
// 		&te,
// 		sizeof( te ),
// 		&dwReturnLength ) )
// 	{
// 		OutputDebugString("IsElevated GetTokenInformation Error");
// 	}
// 	else
// 	{
// 		
// 		hResult = te.TokenIsElevated ? S_OK : S_FALSE; 
// 		
// 		if ( pbElevated)
// 		{
// 			*pbElevated = (te.TokenIsElevated != 0);
// 		}
// 	}
// 	
// 	::CloseHandle( hToken );
// 	
// 	return hResult;
// }


extern "C" BOOL _declspec(dllexport) RunMsiUI(char lpExePath[MAX_PATH], char lpCMD[MAX_PATH])
{
	OutputDebugString("MSIDLL::RunMsiUI Start!!");
	
	SHELLEXECUTEINFO shex;
	
	LPCTSTR pszParameters = NULL;
	LPCTSTR pszDirectory = NULL;
	
	memset( &shex, 0, sizeof( shex) );
	
	shex.cbSize			= sizeof( SHELLEXECUTEINFO ); 
	shex.fMask			= 0; 
	shex.hwnd			= NULL;
	
	
	shex.lpVerb			= "runas"; 
	
	
	shex.lpFile			= lpExePath;
	shex.lpParameters	= lpCMD; 
	shex.lpDirectory	= pszDirectory; 
	shex.nShow			= SW_NORMAL; 
	
	OutputDebugString("ShellExecuteEx Start!!");
	return ::ShellExecuteEx(&shex);
}



extern "C" BOOL _declspec(dllexport) WaitEvent(int nTime)
{
	OutputDebugString("WaitEvent!!");

    HANDLE hEvent = CreateEvent(NULL, TRUE, FALSE, "UC_INSTALL_NOTIFY_MSI_TO_EXIT");
	DWORD dRet = 0;
	if (nTime <= 0)
	{
		nTime = INFINITE;
	}

    if (WAIT_OBJECT_0 == WaitForSingleObject(hEvent, nTime))
    {
		OutputDebugString("WaitEvent!! ok");
        CloseHandle(hEvent);
        return TRUE;
    }
	else
	{
		OutputDebugString("WaitEvent!! fail");
		CloseHandle(hEvent);
		return FALSE;
	}
}

extern "C" BOOL _declspec(dllexport) PostInt(int nFlag, int nCode)
{
	char cs[MAX_PATH];
	sprintf(cs, "MsiDLL::Flag:%d, Code:%d", nFlag, nCode);
	OutputDebugString(cs);
	HWND hWnd = FindWindow(NULL, SETUP_WINDOW_TITLE);
	if (!IsWindow(hWnd))
	{
		OutputDebugString("FindWindow is invalid");
	}
	
    long res = SendMessage(hWnd, PROCESS_MSG, nFlag, nCode);
	sprintf(cs, "MsiDLL::SendMessage return :%d, errcode:%d", res, GetLastError());
	OutputDebugString(cs);
	return TRUE;
}

extern "C" BOOL _declspec(dllexport) RunExe(char lpExePath[MAX_PATH], char lpCMD[MAX_PATH])
{
	ShellExecute(NULL, NULL, lpExePath, lpCMD, NULL, SW_SHOWNORMAL);
	return TRUE;
}

extern "C" VOID _declspec(dllexport) KillExe(char lpProcName[MAX_PATH])
{
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
                if(strcmp(pe.szExeFile, lpProcName) == 0)	
                {
                    proid=pe.th32ProcessID;
					
                    hprocess=::OpenProcess(PROCESS_ALL_ACCESS,FALSE,proid);
					
                    if(hprocess!=NULL)
                    {
                        //OutputDebugString("hprocess!=NULL");
                        ::TerminateProcess(hprocess,0);
                        ::CloseHandle(hprocess);
                    }
                    //OutputDebugString("Find CloseHandle");
                }
				
            } while (Process32Next(hSnapshot, &pe));    
        }
        CloseHandle(hSnapshot);
    }
    //OutputDebugString("End CloseRunningApp");
}

extern "C" VOID _declspec(dllexport) debugLog1(const char *format, ...)
{
	va_list pvar;
	static char strDebugStringA[MAX_PATH*2] = {0};
	va_start(pvar, format);
	vsprintf(strDebugStringA+strlen(strDebugStringA), format, pvar);
	va_end(pvar);
	sprintf(strDebugStringA+strlen(strDebugStringA), "\n");
	OutputDebugStringA(strDebugStringA);
}

extern "C" VOID _declspec(dllexport) debugLog(char lpLog[MAX_PATH])
{
	OutputDebugString(lpLog);
}

#include <Shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")
extern "C" BOOL _declspec(dllexport) getInstallDir(char lpInstallDir[MAX_PATH])
{
//	char  szPath[MAX_PATH] = {0};
// 	SHGetSpecialFolderPath(NULL,szPath,CSIDL_PROFILE,FALSE);
// 	sprintf(lpInstallDir, "%s\\Quanshi", szPath);
// 	return TRUE;

	BOOL bRet = FALSE;
	HKEY hAppKey = NULL;
	
    TCHAR *bsKeyPath = {"Software\\Bee"};
	
    TCHAR *bsKeyName = {"installDir"};
    DWORD dwSize = MAX_PATH;
    if (RegOpenKeyEx(HKEY_CURRENT_USER,
        bsKeyPath,
        0,
        KEY_ALL_ACCESS,
        &hAppKey) == ERROR_SUCCESS)
    {
        DWORD dwType = REG_SZ;
        RegQueryValueEx(hAppKey,
            bsKeyName, 
            NULL, 
            &dwType, 
            (LPBYTE)lpInstallDir, 
            &dwSize);
        RegCloseKey(hAppKey);
		bRet = TRUE;
    }

	return bRet;

}

extern "C" VOID _declspec(dllexport) DelInstallDir()
{
	SHDeleteKey(HKEY_CURRENT_USER,"Software\\Bee");
}

extern "C" VOID _declspec(dllexport) WriteUninstallToReg()
{
    HKEY hAppKey = NULL;
	
    TCHAR *szKeyPath = "Software\\Bee";
	
    TCHAR * szKeyName = "Uninstall";
    DWORD dw;
    LPCTSTR pUnistall = "1";
    if (RegCreateKeyEx(HKEY_CURRENT_USER, 
        szKeyPath, 
        0, 
        REG_NONE, 
        REG_OPTION_NON_VOLATILE, 
        KEY_WRITE|KEY_READ, 
        NULL, 
        &hAppKey, 
        &dw) == ERROR_SUCCESS) 
    {
        DWORD dwType = REG_SZ;
        ::RegSetValueEx(hAppKey, szKeyName, 0, dwType, (LPBYTE)pUnistall, strlen(pUnistall)*sizeof(TCHAR));
        RegCloseKey(hAppKey);
    }
}

extern "C" BOOL _declspec(dllexport) CheckInstalled()
{
	HKEY hAppKey = NULL;
	TCHAR *bsKey = {"Software\\Bee"};

	TCHAR* productKey = new TCHAR[MAX_PATH];
	if (GetProductCode(&productKey) == TRUE)
	{
		return TRUE;
	}
// 	else//为了兼容旧版本
// 	{
// 		TCHAR *bsKeyPath = {"Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\{37255CD4-BFF5-4192-AADF-1289427C36B5}"};
// 
// 		if (RegOpenKeyEx(HKEY_LOCAL_MACHINE,
// 			bsKeyPath,
// 			0,
// 			KEY_ALL_ACCESS,
// 			&hAppKey) == ERROR_SUCCESS)
// 		{
// 			RegCloseKey(hAppKey);
// 			return TRUE;
// 		}
// 	}

	return FALSE;
}

//从注册表获取当前安装产品的标识ID;
//ppProductCode需在外部申请内存并释放
BOOL GetProductCode( TCHAR** ppProductCode )
{
	HKEY hAppKey = NULL;

	TCHAR *bsKeyPath = {"Software\\Bee"};

	if (RegOpenKeyEx(HKEY_CURRENT_USER,
		bsKeyPath,
		0,
		KEY_ALL_ACCESS,
		&hAppKey) == ERROR_SUCCESS)
	{
		DWORD dwType = REG_SZ;
		DWORD dwSize = MAX_PATH;
		TCHAR *bsKeyProductCode = "ProductCode";

		//获取产品ID信息
		if(::RegQueryValueEx(hAppKey,bsKeyProductCode, 0, &dwType, (LPBYTE)*ppProductCode, &dwSize) == ERROR_SUCCESS)
		{
			RegCloseKey(hAppKey);
			return TRUE;
		}
		RegCloseKey(hAppKey);
		return FALSE;
	}

	return FALSE;
}

// 安全的取得真实系统信息
VOID SafeGetNativeSystemInfo( LPSYSTEM_INFO lpSystemInfo)
{
    if (NULL==lpSystemInfo)    return;
    typedef VOID (WINAPI *LPFN_GetNativeSystemInfo)(LPSYSTEM_INFO lpSystemInfo);
    LPFN_GetNativeSystemInfo fnGetNativeSystemInfo = (LPFN_GetNativeSystemInfo)GetProcAddress( GetModuleHandle("kernel32"), "GetNativeSystemInfo");;
    if (NULL != fnGetNativeSystemInfo)
    {
        fnGetNativeSystemInfo(lpSystemInfo);
    }
    else
    {
        GetSystemInfo(lpSystemInfo);
    }
}

#define PROCESSOR_ARCHITECTURE_AMD64 9
// 获取操作系统位数
extern "C" int _declspec(dllexport) GetSystemBits()
{
    SYSTEM_INFO si;
    SafeGetNativeSystemInfo(&si);
	if (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64 ||
        si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_IA64 )
    {
        return 64;
    }
    return 32;
}