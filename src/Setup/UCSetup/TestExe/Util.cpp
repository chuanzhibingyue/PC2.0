#include "StdAfx.h"
#include "Util.h"
#include "FolderOperator.h"
#include "tinyxml/tinyxml.h"

#define UTIL_TEMP_UI_DIR		 L"UCUI"
#define UTIL_SETUP_NAME			L"\\Setup.exe"
#define SETUP_CMD				L" -tnewprocess"
#define SERVICE_NAME			L"UCDaemonSvc"
#define UCSVC_PATH				L"\\Update\\UCDaemonSvc.exe"
#define UCLOG_PATH				L"\\OnCloud\\ClientLog\\"
#define SERVICE_DESCRIPT		L"Enables the download and installation of Quanshi UC updates. \
If this service is disabled, this computer will not be able to use the Quanshi UC Automatic Updates feature." 

#define UCCMEETING_CONFIG		_T("\\Quanshi\\Quanshi Meeting\\6\\60000\\html\\config\\AppConfig.js")
CString Util::m_sOnCloudPath =	_T("");

Util::Util(void)
{
}

Util::~Util(void)
{
}

//记录安装信息到注册表，覆盖安装时直接沿用
BOOL Util::WriteInstallInfoToReg(TCHAR *pDir, TCHAR *sVersion, TCHAR* sProductCode, BOOL bAutoRun, BOOL bRunAfterInstall, BOOL bDeskLnk, BOOL bStartLnk)
{
    HKEY hAppKey = NULL;

    BSTR bsKeyPath = _T("Software\\Bee");
	DWORD dw;

    BSTR bsKeyInstall = _T("installDir");
	BSTR bsKeyProductCode = _T("ProductCode");
	BSTR bsKeyVersion = _T("Version");
	BSTR bsKeyAutoRun = _T("AutoRun");
	BSTR bsKeyRunAfterInstall = _T("RunAfterInstall");
	BSTR bsKeyDeskLnk = _T("DeskLnk");
	BSTR bsKeyStartLnk = _T("StartLnk");
	TCHAR sAutoRun[MAX_PATH] = {0};
	TCHAR sRunAfterInstall[MAX_PATH] = {0};
	TCHAR sDeskLnk[MAX_PATH] = {0};
	TCHAR sStartLnk[MAX_PATH] = {0};
	wsprintf(sAutoRun, _T("%d"), bAutoRun);
	wsprintf(sRunAfterInstall, _T("%d"), bRunAfterInstall);
	wsprintf(sDeskLnk, _T("%d"), bDeskLnk);
	wsprintf(sStartLnk, _T("%d"), bStartLnk);

    if (RegCreateKeyEx(HKEY_CURRENT_USER, 
        bsKeyPath, 
        0, 
        REG_NONE, 
        REG_OPTION_NON_VOLATILE, 
        KEY_WRITE|KEY_READ, 
        NULL, 
        &hAppKey, 
        &dw) == ERROR_SUCCESS) 
    {
        DWORD dwType = REG_SZ;
        ::RegSetValueEx(hAppKey, bsKeyInstall, 0, dwType, (LPBYTE)pDir, _tcslen(pDir)*sizeof(TCHAR));
		if (sProductCode != NULL && sVersion != NULL)
		{
			::RegSetValueEx(hAppKey, bsKeyVersion, 0, dwType, (LPBYTE)sVersion, _tcslen(sVersion)*sizeof(TCHAR));
			::RegSetValueEx(hAppKey, bsKeyProductCode, 0, dwType, (LPBYTE)sProductCode, _tcslen(sProductCode)*sizeof(TCHAR));
			::RegSetValueEx(hAppKey, bsKeyAutoRun, 0, dwType, (LPBYTE)sAutoRun, sizeof(TCHAR));
			//::RegSetValueEx(hAppKey, bsKeyRunAfterInstall, 0, dwType, (LPBYTE)sRunAfterInstall, sizeof(TCHAR));
			::RegSetValueEx(hAppKey, bsKeyDeskLnk, 0, dwType, (LPBYTE)sDeskLnk, sizeof(TCHAR));
			::RegSetValueEx(hAppKey, bsKeyStartLnk, 0, dwType, (LPBYTE)sStartLnk, sizeof(TCHAR));
		}
		
        RegCloseKey(hAppKey);
		LOG_INFO(_T("WriteInstallInfoToReg successful."));
    }
	
	
    return TRUE;
}

BOOL Util::ModifyInstallPolicy(LPCTSTR sMsiName, BOOL bAddorDel)
{
	BSTR bsKeyPath = _T("Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Associations");

	if (bAddorDel == TRUE)
	{
		DWORD dw;
		HKEY hAppKey = NULL;
		BSTR bsKeyLowRisk = _T("LowRiskFileTypes");
		TCHAR szValue[MAX_PATH] = { 0 };
		wsprintf(szValue, _T("%s"), sMsiName);
		DWORD ret = RegCreateKeyEx(HKEY_CURRENT_USER,
			bsKeyPath,
			0,
			REG_NONE,
			REG_OPTION_NON_VOLATILE,
			KEY_WRITE | KEY_READ,
			NULL,
			&hAppKey,
			&dw);
		if (ret == ERROR_SUCCESS)
		{
			DWORD dwType = REG_SZ;
			::RegSetValueEx(hAppKey, bsKeyLowRisk, 0, dwType, (LPBYTE)szValue, _tcslen(szValue)*sizeof(TCHAR));
			RegCloseKey(hAppKey);
			LOG_INFO(_T("ModifyInstallPolicy add successful."));
			return TRUE;
		}	
	}
	else
	{
		if (ERROR_SUCCESS == RegDeleteKey(HKEY_CURRENT_USER, bsKeyPath))
		{
			LOG_INFO(_T("ModifyInstallPolicy delete successful."));
			return TRUE;
		}
	}
	LOG_INFO(_T("ModifyInstallPolicy failed."));
	return FALSE;
}

//从注册表获取已安装程序的安装信息
BOOL Util::GetInstallInfoFromReg( TCHAR** ppDir, BOOL &bAutoRun, BOOL &bRunAfterInstall, BOOL &bDeskLnk, BOOL &bStartLnk, BOOL &bOld)
{
	HKEY hAppKey = NULL;
	bOld = FALSE;
	TCHAR* bsKeyPath_Old = {_T("Software\\UC")};
	TCHAR *bsKeyPath = {_T("Software\\Bee")};

	//先检测有没有装老版本，若装过则依照老版本的安装信息覆盖安装
	if (RegOpenKeyEx(HKEY_CURRENT_USER,
		bsKeyPath_Old,
		0,
		KEY_READ,
		&hAppKey) == ERROR_SUCCESS)
	{
		bOld = TRUE;
		DWORD dwType = REG_SZ;
		DWORD dwSize = MAX_PATH;
		BSTR bsKeyInstall = _T("installDir");
		BSTR bsKeyAutoRun = _T("AutoRun");
		BSTR bsKeyRunAfterInstall = _T("RunAfterInstall");
		BSTR bsKeyDeskLnk = _T("DeskLnk");
		BSTR bsKeyStartLnk = _T("StartLnk");
		TCHAR sAutoRun[MAX_PATH] = { 0 };
		TCHAR sRunAfterInstall[MAX_PATH] = { 0 };
		TCHAR sDeskLnk[MAX_PATH] = { 0 };
		TCHAR sStartLnk[MAX_PATH] = { 0 };

		//获取安装目录
		LONG result = ::RegQueryValueEx(hAppKey, bsKeyInstall, 0, &dwType, (LPBYTE)*ppDir, &dwSize);
		if (result != ERROR_SUCCESS)
			goto exit;

		//获取是否开机自启动
		dwSize = MAX_PATH;
		result = ::RegQueryValueEx(hAppKey, bsKeyAutoRun, 0, &dwType, (LPBYTE)&sAutoRun, &dwSize);

		//获取是否创建桌面LNK
		dwSize = MAX_PATH;
		result = ::RegQueryValueEx(hAppKey, bsKeyDeskLnk, 0, &dwType, (LPBYTE)&sDeskLnk, &dwSize);

		//获取是否创建启动栏LNK
		dwSize = MAX_PATH;
		result = ::RegQueryValueEx(hAppKey, bsKeyStartLnk, 0, &dwType, (LPBYTE)&sStartLnk, &dwSize);

		TCHAR* stopStr;
		bAutoRun = _tcstol(sAutoRun, &stopStr, 10);
		bRunAfterInstall = _tcstol(sRunAfterInstall, &stopStr, 10);
		bDeskLnk = _tcstol(sDeskLnk, &stopStr, 10);
		bStartLnk = _tcstol(sStartLnk, &stopStr, 10);
		RegCloseKey(hAppKey);
		return TRUE;
	}

	if (RegOpenKeyEx(HKEY_CURRENT_USER,
		bsKeyPath,
		0,
		KEY_READ,
		&hAppKey) == ERROR_SUCCESS)
	{
		DWORD dwType = REG_SZ;
		DWORD dwSize = MAX_PATH;
		BSTR bsKeyInstall = _T("installDir");
		BSTR bsKeyAutoRun = _T("AutoRun");
		BSTR bsKeyRunAfterInstall = _T("RunAfterInstall");
		BSTR bsKeyDeskLnk = _T("DeskLnk");
		BSTR bsKeyStartLnk = _T("StartLnk");
		TCHAR sAutoRun[MAX_PATH] = {0};
		TCHAR sRunAfterInstall[MAX_PATH] = {0};
		TCHAR sDeskLnk[MAX_PATH] = {0};
		TCHAR sStartLnk[MAX_PATH] = {0};

		//获取安装目录
		LONG result = ::RegQueryValueEx(hAppKey,bsKeyInstall, 0, &dwType, (LPBYTE)*ppDir, &dwSize);
		if (result != ERROR_SUCCESS)
			goto exit;

		//获取是否开机自启动
		dwSize = MAX_PATH;
		result =::RegQueryValueEx(hAppKey,bsKeyAutoRun, 0, &dwType, (LPBYTE)&sAutoRun, &dwSize);

		//获取是否创建桌面LNK
		dwSize = MAX_PATH;
		result =::RegQueryValueEx(hAppKey,bsKeyDeskLnk, 0, &dwType, (LPBYTE)&sDeskLnk, &dwSize);


		//获取是否创建启动栏LNK
		dwSize = MAX_PATH;
		result =::RegQueryValueEx(hAppKey,bsKeyStartLnk, 0, &dwType, (LPBYTE)&sStartLnk, &dwSize);

		//获取是否安装完后立即启动
// 		dwSize = MAX_PATH;
// 		result =::RegQueryValueEx(hAppKey,bsKeyRunAfterInstall, 0, &dwType, (LPBYTE)&sRunAfterInstall, &dwSize);
// 		if (result != ERROR_SUCCESS)
// 			goto exit;

		TCHAR* stopStr;
		bAutoRun = _tcstol(sAutoRun, &stopStr, 10);
		bRunAfterInstall = _tcstol(sRunAfterInstall, &stopStr, 10);
		bDeskLnk = _tcstol(sDeskLnk, &stopStr, 10);
		bStartLnk = _tcstol(sStartLnk, &stopStr, 10);
		RegCloseKey(hAppKey);
		return TRUE;
	}

exit:
	if(NULL != hAppKey)
		RegCloseKey(hAppKey);
	return FALSE;
}

//从注册表获取当前安装产品的标识ID;
//ppProductCode需在外部申请内存并释放
int Util::GetProductCode( TCHAR** ppProductCode)
{
	HKEY hAppKey = NULL;

	LOG_INFO(L"GetProductCode enter");
	//有没有装老版蜜蜂
	TCHAR *oldbsKeyPath = {_T("Software\\UC")};
	if (RegOpenKeyEx(HKEY_CURRENT_USER,
		oldbsKeyPath,
		0,
		KEY_READ,
		&hAppKey) == ERROR_SUCCESS)
	{
		DWORD dwType = REG_SZ;
		DWORD dwSize = MAX_PATH;
		BSTR bsKeyProductCode = _T("ProductCode");

		//获取产品ID信息
		if(::RegQueryValueEx(hAppKey,bsKeyProductCode, 0, &dwType, (LPBYTE)*ppProductCode, &dwSize) == ERROR_SUCCESS)
		{
			OutputDebugString(_T("GetProductCode find UC!"));
			LOG_INFO(L"GetProductCode find UC!");
			RegCloseKey(hAppKey);
			return 1;		//安装了老版蜜蜂
		}
	}

	TCHAR *bsKeyPath = {_T("Software\\Bee")};
	if (RegOpenKeyEx(HKEY_CURRENT_USER,
		bsKeyPath,
		0,
		KEY_READ,
		&hAppKey) == ERROR_SUCCESS)
	{
		DWORD dwType = REG_SZ;
		DWORD dwSize = MAX_PATH;
		BSTR bsKeyProductCode = _T("ProductCode");

		//获取产品ID信息
		if(::RegQueryValueEx(hAppKey,bsKeyProductCode, 0, &dwType, (LPBYTE)*ppProductCode, &dwSize) == ERROR_SUCCESS)
		{
			OutputDebugString(_T("GetProductCode find Bee!"));
			LOG_INFO(L"GetProductCode find Bee!");
			RegCloseKey(hAppKey);
			return 2;
		}
	}
	LOG_INFO(L"GetProductCode return 0!");
	return 0;
}

//从注册表获取当前安装产品的版本号;
//ppVersion需在外部申请内存并释放
BOOL Util::GetProductVersion( TCHAR** ppVersion )
{
	HKEY hAppKey = NULL;

	TCHAR *bsKeyPath = {_T("Software\\Bee")};

	if (RegOpenKeyEx(HKEY_CURRENT_USER,
		bsKeyPath,
		0,
		KEY_READ,
		&hAppKey) == ERROR_SUCCESS)
	{
		DWORD dwType = REG_SZ;
		DWORD dwSize = MAX_PATH;
		BSTR bsKeyProductCode = _T("Version");

		//获取产品ID信息
		if(::RegQueryValueEx(hAppKey,bsKeyProductCode, 0, &dwType, (LPBYTE)*ppVersion, &dwSize) == ERROR_SUCCESS)
		{
			RegCloseKey(hAppKey);
			return TRUE;
		}
		RegCloseKey(hAppKey);
		return FALSE;
	}

	return FALSE;
}

BOOL Util::DeleteInstallInfoFromReg(BOOL bDeleteOld)
{

	if (bDeleteOld == TRUE)
	{
		BSTR bsOldKeyPath = _T("Software\\UC");
		if (ERROR_SUCCESS == RegDeleteKey(HKEY_CURRENT_USER, bsOldKeyPath))
		{
			LOG_INFO(_T("DeleteInstallInfoFromReg Successed of UC!"));
			//return TRUE;
		}
	}


	BSTR bsKeyPath = _T("Software\\Bee");
	if (ERROR_SUCCESS == RegDeleteKey(HKEY_CURRENT_USER, bsKeyPath))
	{
		LOG_INFO(_T("DeleteInstallInfoFromReg Successed of Bee!"));
		//return TRUE;
	}

	return TRUE;
}


BOOL Util::UninstallFromControlPannel()
{
	LOG_INFO(_T("UninstallFromControlPannel be called!"));
    HKEY hAppKey = NULL;

    TCHAR *bsKeyPath = _T("Software\\Bee");

    TCHAR *bsKeyName = _T("Uninstall");
    DWORD dwSize = MAX_PATH;
    TCHAR szValue[MAX_PATH] = _T("");
    if (RegOpenKeyEx(HKEY_CURRENT_USER,
        bsKeyPath,
        0,
        KEY_READ,
        &hAppKey) == ERROR_SUCCESS)
    {
        DWORD dwType = REG_DWORD;
        RegQueryValueEx(hAppKey,
            bsKeyName, 
            NULL, 
            &dwType, 
            (LPBYTE)szValue, 
            &dwSize);
        RegCloseKey(hAppKey);
    }

    SHDeleteKey(HKEY_CURRENT_USER, _T("Software\\Bee"));
    if (0 == _tcscmp(szValue, _T("1")))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

BOOL Util::CreateLnk (LPCTSTR szPath, LPCTSTR szLink, LPCTSTR szParam)
{    
    HRESULT hres ;  
    IShellLink * psl ;  
    IPersistFile* ppf ;  
    int nBeginAt=CSIDL_DESKTOPDIRECTORY;  

	LOG_INFO(L"Util::CreateLnk, szPath = %s, szLink =%s, szParam = %s", szPath, szLink, szParam);
    CoInitialize(NULL);  

    //创建一个IShellLink实例  
    hres = CoCreateInstance( CLSID_ShellLink, NULL,CLSCTX_INPROC_SERVER, IID_IShellLink, (void **)&psl);  
    if( FAILED( hres))   return FALSE ;  

    //设置目标应用程序  
    psl -> SetPath( szPath) ;  

    //获取路径  
    CString strTemp = szPath;  
    CString strAppPath;  
    int nRet = strTemp.ReverseFind('\\');  
    if(nRet != -1)  
    {  
        strAppPath = strTemp.Left(nRet);  
    }  

    psl -> SetWorkingDirectory(strAppPath);   
    //从IShellLink获取其IPersistFile接口  
    //用于保存快捷方式的数据文件 (*.lnk)  

    hres = psl -> QueryInterface( IID_IPersistFile,(void**)&ppf) ;  

    if( FAILED( hres))
	{
		LOG_ERR(L"Util::CreateLnk, QueryInterface failed! errcode = %d", GetLastError());
        return FALSE ;  
	}
    psl->SetArguments(szParam);

// 	TCHAR sIconPath[MAX_PATH] = { 0 };
// 	wsprintf(sIconPath, L"%s\\resources\\html\\images\\bee.ico", strAppPath);
// 	psl->SetIconLocation(sIconPath, 0);
// 	LOG_INFO(L"Util::CreateLnk, seticonlocation icon path =%s, errno = %d", sIconPath, GetLastError());

    //调用IPersist:Save  
    //保存快捷方式的数据文件 (*.lnk)  
    hres = ppf -> Save( (LPWSTR)szLink, STGM_READWRITE) ;
	if( FAILED( hres))
	{
		LOG_ERR(L"Util::CreateLnk, Save failed! errcode = %d", GetLastError()); 
	}
    //释放IPersistFile和IShellLink接口  
    ppf -> Release( ) ;  
    psl -> Release( ) ;  
    return TRUE;  
}  

BOOL Util::DeleteLnk( LPCTSTR lpszShortcut)
{
    SHFILEOPSTRUCT fos ;
    ZeroMemory( &fos, sizeof(fos)) ;
    fos.hwnd = HWND_DESKTOP ;
    fos.wFunc = FO_DELETE ;
    fos.pFrom = lpszShortcut;
    fos.pTo = NULL ;
    fos.fFlags = FOF_SILENT | FOF_NOCONFIRMATION ;
    //删除快捷方式（*.lnk)
    if( 0 != SHFileOperation( &fos))
        return FALSE ;
    return TRUE ;
}

BOOL Util::DeleteFolder( LPCTSTR pszFolder, BOOL bAllowUndo)
{
	if ((NULL == pszFolder))
	{
	    return FALSE;
	}

	int iPathLen = _tcslen(pszFolder);
	if (iPathLen >= MAX_PATH)
	{
		return FALSE;
	}

	TCHAR tczFolder[MAX_PATH + 1];
	ZeroMemory(tczFolder, (MAX_PATH + 1)*sizeof(TCHAR));
	_tcscpy(tczFolder, pszFolder);
	tczFolder[iPathLen] = _T('\0');
	tczFolder[iPathLen + 1] = _T('\0');	//确保目录的路径以2个\0结尾

	SHFILEOPSTRUCT fos;
	ZeroMemory(&fos, sizeof(fos));
    fos.hwnd = NULL;
    fos.wFunc = FO_DELETE ;
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
    if( 0 != SHFileOperation( &fos))
        return FALSE ;
    return TRUE;
}

BOOL Util::DeleteLnkFromProgramsMenu(LPCTSTR pszDir)
{
    //删除程序菜单
    TCHAR   szPath[MAX_PATH];
    SHGetSpecialFolderPath(NULL,szPath,CSIDL_PROGRAMS,FALSE);
    _tcscat(szPath, pszDir);
    Util::DeleteFiles(szPath, TRUE, NULL);
    LOG_INFO(L"DeleteLnkFromProgramsMenu path = %s",szPath);
    return RemoveDirectory(szPath);
}

BOOL Util::DeleteLnkFromQuickStartMenu(LPCTSTR pszLnk)
{
	//删除快速启动菜单
	TCHAR   szPath[MAX_PATH];
	SHGetSpecialFolderPath(NULL,szPath,CSIDL_APPDATA,FALSE);
	_tcscat(szPath, pszLnk);

	LOG_INFO(L"DeleteLnkFromQuickStartMenu path = %s",szPath);
	return DeleteFile(szPath);
}

BOOL Util::DeleteLnkFromDesktop(LPCTSTR pszLnk)
{
    TCHAR   szPath[MAX_PATH];
    //删除桌面快捷方式
    SHGetSpecialFolderPath(NULL,szPath,CSIDL_DESKTOPDIRECTORY,FALSE);
    _tcscat(szPath, pszLnk);
    LOG_INFO(L"DeleteLnkFromDesktop, path = %s", szPath);
    return DeleteFile(szPath);
}

BOOL Util::CreateDesktopLnk(LPCTSTR pszTargetPath, LPCTSTR pszLnk)
{
    //创建桌面快捷方式
    TCHAR   szPath[MAX_PATH] = {0};
    SHGetSpecialFolderPath(NULL,szPath,CSIDL_DESKTOPDIRECTORY,FALSE);
    _tcscat(szPath, pszLnk);
	LOG_INFO(L"CreateDesktopLnk, pszTargetPath = %s, szPath = %s", pszTargetPath, szPath);
    return Util::CreateLnk(pszTargetPath, szPath, NULL);
}

BOOL Util::CreateLnkToProgramsMenu(LPCTSTR pszTargetPath, 
                                LPCTSTR pszUninstallPath,
                                LPCTSTR pszDir,
                                LPCTSTR pszUninstallLnk,
                                LPCTSTR pszTargetLnk
                                )
{
    TCHAR   szPath[MAX_PATH] = {0};
    SHGetSpecialFolderPath(NULL,szPath,CSIDL_PROGRAMS,FALSE);
    _tcscat(szPath, pszDir);
    CreateDirectory(szPath, NULL);
    TCHAR szLink[MAX_PATH] = {0};
    wsprintf(szLink, _T("%s%s"), szPath, pszUninstallLnk);
    Util::CreateLnk(pszUninstallPath, szLink, _T("-suninstallbee"));
    _tcscat(szPath, pszTargetLnk);

	LOG_INFO(L"CreateLnkToProgramsMenu, pszTargetPath = %s, szPath = %s", pszTargetPath, szPath);
    Util::CreateLnk(pszTargetPath, szPath, NULL);
    return TRUE;
}

BOOL Util::CreateLnkToQuickStartMenu(LPCTSTR pszTargetPath, 
									LPCTSTR pszTargetLnk
									)
{
	TCHAR   szPath[MAX_PATH] = {0};
	SHGetSpecialFolderPath(NULL,szPath,CSIDL_APPDATA,FALSE);
	_tcscat(szPath, pszTargetLnk);

	LOG_INFO(L"CreateLnkToStartMenu, pszTargetPath = %s, szPath = %s", pszTargetPath, szPath);
	Util::CreateLnk(pszTargetPath, szPath, NULL);
	return TRUE;
}

BOOL Util::MakePath(TCHAR *pPath, TCHAR *pName)
{
    TCHAR szPath[MAX_PATH] = {0};
    GetModuleFileName(NULL, szPath, sizeof(szPath));
    TCHAR *p = _tcsrchr(szPath, _T('\\'));
    int n = p - szPath;
    _tcsncpy(pPath, szPath, n);
    _tcscat(pPath, pName);

    return TRUE;
}

BOOL Util::IsEndWithBackSlashOrSlash(LPCTSTR lpPath) 
{ 
    char cLastChar = lpPath[_tcslen(lpPath) - 1];
    if (L'\\' == cLastChar || L'/' == cLastChar)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }

} 

VOID Util::DeleteFiles(LPCTSTR lpDir, BOOL bDelDir, LPCTSTR lpException) 
{ 
	CString sMsg = _T("");
    //在测试过程中发现如果lpDir=""，则会默认为进程所在盘符的根目录，如果在C盘，则默认为C:\\
    //为了防止此类情况发生，对路径首字母的有效性进行判断
    if (NULL == lpDir ||
        0 == _tcslen(lpDir))
    {
        return ;
    }

    //例子："D:\\Test"
	TCHAR szFind[MAX_PATH] = {0}; 

    _tcscpy(szFind,lpDir); //windows API 用lstrcpy，不是strcpy 

    if(!IsEndWithBackSlashOrSlash(szFind)) 
        _tcscat(szFind,_T("\\")); 

    _tcscat(szFind,_T("*.*")); //找所有文件 

	//sMsg.Format(_T("DeleteFiles::lpDir = %s ."), szFind);
	//LOG_INFO(sMsg);

    WIN32_FIND_DATA wfd; 
    HANDLE hFind=FindFirstFile(szFind,&wfd); 
	
    if(hFind==INVALID_HANDLE_VALUE) // 如果没有找到或查找失败 
	{
		LOG_INFO(_T("DeleteFiles::FindFirstFile return is Invalid."));
		return;
	}

    do 
    { 
        if(wfd.cFileName[0]==_T('.')) 
            continue; //过滤这两个目录

        if (NULL != lpException)
        {
            if (NULL != _tcsstr(lpException, wfd.cFileName))
            {
                continue;
            }
        }

        if(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) 
        { 

            TCHAR szFile[MAX_PATH]; 

            if(IsEndWithBackSlashOrSlash(lpDir)) 
                wsprintf(szFile,_T("%s%s"),lpDir,wfd.cFileName); 
            else 
                wsprintf(szFile,_T("%s\\%s"),lpDir,wfd.cFileName); 

            DeleteFiles(szFile, bDelDir, lpException); //如果找到的是目录，则进入此目录进行递归

            if (TRUE == bDelDir)
            {
                RemoveDirectory(szFile);
				sMsg.Format(_T("RemoveDirectory : %s ."), szFile);
				LOG_INFO(sMsg);
            }
        } 
        else 
        { 

            TCHAR szFile[MAX_PATH]; 
            if(IsEndWithBackSlashOrSlash(lpDir)) 
				wsprintf(szFile,_T("%s%s"),lpDir,wfd.cFileName); 
            else 
                wsprintf(szFile,_T("%s\\%s"),lpDir,wfd.cFileName); 

            DeleteFile(szFile); //对文件进行操作 
			//sMsg.Format(_T("DeleteFile : %s ."), szFile);
			//LOG_INFO(sMsg);
        } 
    } 
    while(FindNextFile(hFind,&wfd)); 

    FindClose(hFind); //关闭查找句柄 

	if (TRUE == bDelDir)
	{
		RemoveDirectory(lpDir);
	}
}

VOID Util::CreateDir(CString strDesPath)
{
	if (strDesPath.IsEmpty())
	{
		return;
	}

	CString strCreatePath;
	BOOL bRet = FALSE;
	DWORD dw = 0;
	CString sMsg;

	if ( !FolderExist(strDesPath) )
	{
		strCreatePath = strDesPath;

		strDesPath = strDesPath.Left(strDesPath.ReverseFind('\\'));
		Util::CreateDir(strDesPath);

		bRet = CreateDirectory(strCreatePath, NULL);
		if (!bRet)
		{
			dw = GetLastError();
			sMsg.Format(_T("CreateDirectory ( %s ) failed, errcode = %d"), strCreatePath, dw);
			LOG_INFO(sMsg);
		}
	}
}

// 判断指定的目录是否存在
BOOL Util::FolderExist(CString strPath)
{
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


// 判断指定的文件是否存在
BOOL Util::FileExist(CString strPath)
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

BOOL Util::AppAlreadyExist()
{
    HANDLE hMutex = CreateMutex(NULL, TRUE, _T("dsfsdfsfasfsf"));
    if (NULL != hMutex)
    {
        DWORD dwLastError = GetLastError();
        if (ERROR_ALREADY_EXISTS != dwLastError)
        {
            return FALSE;
        }
        else
        {
            CloseHandle(hMutex);
            return TRUE;
        }
    }
    else
    {
        CloseHandle(hMutex);
        return TRUE;
    }
}

// 判断系统版本
DWORD Util::JudgeOSVersion(void)
{
    OSVERSIONINFO Version;
    ZeroMemory(&Version, sizeof(OSVERSIONINFO));
    Version.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    GetVersionEx(&Version); 
    return Version.dwMajorVersion;
}

BOOL Util::MockLButtonDown()
{
    //判断OS版本
    if(JudgeOSVersion() <= 5 ) /**< 2000/WinXP */
    {
        LOG_INFO(_T("UCSetup Initialize: XP or 2000 OS"));
        return Util::MockLButtonDown(_T("添加或删除程序"), _T("确定"));
    }
    else /**< vista or win7 */
    {
        LOG_INFO(_T("UCSetup Initialize: Vista or Win7"));
        return Util::MockLButtonDown(_T("Windows Installer"), _T("确定"));
    }
}

BOOL Util::MockLButtonDown(LPCTSTR lpWndName, LPCTSTR lpBtnName)
{
    BOOL bRet = FALSE;
    int nTimes = 0;

    LOG_INFO(_T("UCSetup Util::MockLButtonDown do while"));
    Sleep(10);
    HWND hWnd = NULL;
    hWnd = FindWindow(_T("#32770"), lpWndName);
    if (NULL != hWnd)
    {
        LOG_INFO(_T("UCSetup Util::MockLButtonDown FindWindw"));
        ShowWindow(hWnd, SW_HIDE);
        HWND hSubWnd = FindWindowEx(hWnd, NULL, NULL, lpBtnName);
        if (hSubWnd != NULL)
        {
            LOG_INFO(_T("UCSetup Util::MockLButtonDown lbuttondown"));
            SendMessage(hSubWnd,WM_LBUTTONDOWN,0,0);
            SendMessage(hSubWnd,WM_LBUTTONUP,0,0);
        }
        else
        {
            LOG_INFO(_T("UCSetup Util::MockLButtonDown WM_CLOSE, IDOK, IDCANCEL, "));
            SendMessage(hWnd,WM_COMMAND, IDOK, 0);
            SendMessage(hWnd,WM_COMMAND, IDCANCEL, 0);
            SendMessage(hWnd,WM_CLOSE,0,0);
        }
        bRet = TRUE;
    }

    return bRet;
}

BOOL Util::NeedTransferDir()
{
    WCHAR wPath[MAX_PATH] = {0};
    WCHAR wSourcePath[MAX_PATH] = {0};

    if (!SHGetSpecialFolderPath(NULL,wPath,CSIDL_TEMPLATES,FALSE))//获取temp目录
    {
        return FALSE;
    }

    //     // 检测programfile下是否有可拷贝的文件
    //     CString strSourceFile = wSourcePath;
    //     strSourceFile += L"\\Quanshi";
    //     strSourceFile += TARGET_SHORTCUT_DIR_NAME;

    //LOG_INFO(strSourceFile);
    //     if (!PathFileExists(strSourceFile))
    //     {
    //         return FALSE;
    //     }

    //     // 计算flag文件具体路径
    //     CString strTargetFile = wPath;
    //     strTargetFile += L"\\Quanshi";
    //     strTargetFile += TARGET_SHORTCUT_DIR_NAME;

    //     if ( !PathFileExists(strTargetFile) )
    //     {
    //         return TRUE;
    //     }
    return FALSE;

}

HRESULT Util::DoTransferDir()
{
    HRESULT hr = S_OK;
    CString strSourcePath,strTargetPath,strResultFile;
    CFolderOperator fo;

    //find the source path and target path
    WCHAR wPath[MAX_PATH] = {0};
    DWORD dw = GetModuleFileName(NULL,wPath,MAX_PATH);
    if (dw==0)
    {
        hr = E_FAIL;	
        goto exit;
    }
    strSourcePath = wPath;
    strSourcePath = strSourcePath.Mid(0,strSourcePath.ReverseFind(L'\\'));
	Util::m_sOnCloudPath = strSourcePath.Mid(0, strSourcePath.ReverseFind(L'\\'));
    //	strSourcePath = strSourcePath.Mid(0,strSourcePath.ReverseFind(L'\\'));

    memset(wPath,0, MAX_PATH);
    GetTempPath(MAX_PATH, wPath);

    strTargetPath = wPath;
    strTargetPath += UTIL_TEMP_UI_DIR;


    //     if (PathFileExists(strResultFile))
    //     {
    //         if (!DeleteFile(strResultFile))
    //         {
    //             hr = E_FAIL;	
    //             goto exit;
    //         }
    // 
    //     }

	DWORD dstartTime = GetTickCount();
    hr = fo.CopyDirectory(strSourcePath,strTargetPath,FALSE);

    //     if (INVALID_HANDLE_VALUE == CreateFile(strResultFile,GENERIC_READ,0,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL))
    //     {
    //         hr = E_HANDLE;	
    //         goto exit;
    //     }
	DWORD dEnd = GetTickCount();
	DWORD time = dEnd - dstartTime;
exit:
    if (FAILED(hr))
    {
        //LOG_ERR(L"DoTransferDir failed, hr = %d", hr);
    }
    return hr;
}

BOOL Util::RunNewSetup()
{
    WCHAR wPath[MAX_PATH] = {0};
    //     if (!SHGetSpecialFolderPath(NULL,wPath,CSIDL_TEMPLATES,FALSE))
    //     {
    //         return FALSE;
    //     }
    GetTempPath(MAX_PATH, wPath);
    CString strAppPath = wPath;
    strAppPath += UTIL_TEMP_UI_DIR;
    strAppPath += UTIL_SETUP_NAME;
    if (!PathFileExists(strAppPath))
    {
        return FALSE;
    }

    strAppPath += SETUP_CMD;

    STARTUPINFO si;
    ::memset( &si, 0, sizeof( si ));
    si.cb = sizeof( si );
    PROCESS_INFORMATION pi;
    BOOL isSucceed = FALSE;
    isSucceed = CreateProcess(NULL,(LPTSTR)(LPCTSTR)strAppPath,NULL,NULL,FALSE,0,NULL,NULL,&si,&pi);
    if (!isSucceed)
    {
        //LOG_INFO(L"Starting TangRoute failed, error is %d", GetLastError());
        return FALSE;
    }

    return TRUE;
}


BOOL Util::InstallUpdateSvc(CString sInstallDir)
{
	CString sExePath = sInstallDir + UCSVC_PATH;
	ShellExecute(NULL, _T("open"), sExePath,_T("/install"), NULL, SW_HIDE);	
	//ShellExecute(NULL, _T("open"), _T("C:\\Windows\\System32\\cmd.exe"), sParam, NULL, SW_HIDE);
	LOG_INFO(L"InstallUpdateSvc, UCDaemonSvc.exe's Path: %s", sExePath);
	return TRUE;
}

BOOL Util::UnInstallUpdateSvc()
{
	SC_HANDLE hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);  

	if (hSCM == NULL)  
	{  
		LOG_ERR(L"UnInstallUpdateSvc, Couldn't open service manager");
		return FALSE;  
	}  

	SC_HANDLE hService = ::OpenService(hSCM, SERVICE_NAME, SERVICE_STOP | DELETE);  

	if (hService == NULL)  
	{  
		::CloseServiceHandle(hSCM);
		LOG_ERR(L"UnInstallUpdateSvc, Couldn't open service");
		return FALSE;  
	}  
	SERVICE_STATUS status;  
	::ControlService(hService, SERVICE_CONTROL_STOP, &status);  

	//删除服务   
	BOOL bDelete = ::DeleteService(hService);  
	::CloseServiceHandle(hService);  
	::CloseServiceHandle(hSCM);  

	if (bDelete)
	{
		LOG_INFO(L"UnInstallUpdateSvc successful!!"); 	
		return TRUE;  
	}

	LOG_ERR(L"UnInstallUpdateSvc, Service could not be deleted");  
	return FALSE;
}


//读取配置文件
BOOL Util::ReadConfig(CString& strConfig)
{
	CString sMsg = _T("");
	int errCode = 0;

	strConfig = _T("");
	TCHAR szPath[MAX_PATH]; 
	if( !GetModuleFileName( NULL, szPath, MAX_PATH ) )
	{
		errCode = GetLastError();
		sMsg.Format(_T("GetModuleFileName failed, szPath = %s, getlasterror = %d"), szPath, errCode);
		LOG_INFO(sMsg);
		return FALSE;
	}

	CString sTemp = szPath;
	int nPos = sTemp.ReverseFind('\\');
	sTemp = sTemp.Left(nPos);
	CString sCfg = _T("\\UCHost");
	sTemp += sCfg;

	FILE* pConfigFile = NULL;
	TCHAR szfile[128];
	memset( szfile, 0, sizeof(szfile) );
	wcscpy( szfile, sTemp.GetBuffer(0) );
	errCode = _wfopen_s(&pConfigFile,szfile,_T("r"));
	if( errCode != 0)
	{
		sMsg.Format(_T("open config file failed, file path = %s, getlasterror = %d"), szfile, errCode);
		LOG_INFO(sMsg);
		return FALSE;
	}
	char *buf = new char[1024*1024];
	size_t nReadCount = fread(buf,sizeof(char),1024*1024,pConfigFile);
	buf[nReadCount] = '\0';
	strConfig = buf;
	delete [] buf;

	fclose(pConfigFile);

	return TRUE;
}

BOOL Util::WriteHosts()
{
	LOG_INFO(L"Begin write hosts.");
	FILE* pHostFile = NULL;
	int errCode = 0;

	TCHAR szPath[MAX_PATH];
	GetWindowsDirectory(szPath,sizeof(szPath));
	CString strHostPath;
	strHostPath.Format(_T("%s\\System32\\drivers\\etc\\hosts"),szPath);
	TCHAR szfile[MAX_PATH];
	memset( szfile, 0, sizeof(szfile) );
	wcscpy( szfile, strHostPath.GetBuffer(0) );
	errCode = _wfopen_s(&pHostFile,szfile,_T("r"));
	if ( errCode !=0 )
	{
		LOG_ERR(L"Read hosts file failed! errCode = %d", errCode);
		return FALSE;
	}

	char *buf = new char[1024*1024];
	size_t nReadCount = fread(buf,sizeof(char),1024*1024,pHostFile);
	buf[nReadCount] = '\0';
	CString strContent(buf);
	delete [] buf;
	buf = NULL;
	fclose(pHostFile);

	CString sConfig;
	if(FALSE == ReadConfig(sConfig))
	{
		LOG_ERR(L"Read config file failed!");
		return FALSE;
	}

	CString strTemp = sConfig;
	strTemp += strContent;

	//这里如果hosts文件中第一个字符不是#和/n中的一个，那么会失败
// 	char cFirst = strTemp[0];
// 	if (cFirst != '#' &&
// 		cFirst != '\n')
// 	{
// 		strTemp = '\n' + strTemp;
// 	}

	//直接覆盖可能覆盖文字过短导致无法完全覆盖的BUG，所以采用用w+打开文件销毁原来内容，然后再写入新数据的方法
	pHostFile = NULL;
	errCode = _wfopen_s(&pHostFile,szfile,_T("w"));
	if ( errCode !=0 )
	{
		LOG_ERR(L"Write hosts file failed! errCode = %d", errCode);
		return FALSE;
	}

	fseek(pHostFile,0,0);
	buf = new char[1024*1024];
	WideCharToMultiByte(CP_ACP,0,strTemp.GetBuffer(0),strTemp.GetLength(),buf,1024*1024,0,0);
	fwrite(buf,sizeof(char),strTemp.GetLength(),pHostFile);
	fclose(pHostFile);
	delete [] buf;

	LOG_INFO(L"Write hosts Successed!");
	return TRUE;
}

BOOL Util::CleanHosts()
{
	LOG_INFO(L"Begin clean hosts.");
	FILE* pHostFile = NULL;
	int errCode = 0;

	TCHAR szPath[MAX_PATH];
	GetWindowsDirectory(szPath,sizeof(szPath));
	CString strHostPath;
	strHostPath.Format(_T("%s\\System32\\drivers\\etc\\hosts"),szPath);
	TCHAR szfile[MAX_PATH];
	memset( szfile, 0, sizeof(szfile) );
	wcscpy( szfile, strHostPath.GetBuffer(0) );
	errCode = _wfopen_s(&pHostFile,szfile,_T("r"));
	if ( errCode !=0 )
	{
		LOG_INFO(L"Read hosts file failed! errCode = %d", errCode);
		return FALSE;
	}

	char *buf = new char[1024*1024];
	size_t nReadCount = fread(buf,sizeof(char),1024*1024,pHostFile);
	buf[nReadCount] = '\0';
	CString strContent(buf);
	delete [] buf;
	buf = NULL;
	fclose(pHostFile);

	CString sConfig;
	if(FALSE == ReadConfig(sConfig))
	{
		LOG_ERR(L"Read config file failed!");
		return FALSE;
	}

	CString strTemp = sConfig;
	int pos = strContent.Find(sConfig,0);
	if (pos == -1)
	{
		//not find
		return FALSE;
	}
	else
	{
		strContent.Delete(pos,sConfig.GetLength());
	}

// 	char cFirst = strContent[0];
// 	if (cFirst != '#' &&
// 		cFirst != '\n')
// 	{
// 		strContent = '\n' + strContent;
// 	}

	//直接覆盖可能覆盖文字过短导致无法完全覆盖的BUG，所以采用用w+打开文件销毁原来内容，然后再写入新数据的方法
	pHostFile = NULL;
	errCode = _wfopen_s(&pHostFile,szfile,_T("w"));
	if ( errCode !=0 )
	{
		LOG_ERR(L"Write hosts file failed! errCode = %d", errCode);
		return FALSE;
	}

	fseek(pHostFile,0,0);

	buf = new char[1024*1024];
	WideCharToMultiByte(CP_ACP,0,strContent.GetBuffer(0),strContent.GetLength(),buf,1024*1024,0,0);
	fwrite(buf,sizeof(char),strContent.GetLength(),pHostFile);
	fclose(pHostFile);
	delete [] buf;

	LOG_INFO(L"Clean hosts Successed!");
	return TRUE;
}

BOOL Util::ChangMeetingCfg()
{
	//获取卸载程序路径并静默启动。
	TCHAR moduleFullName[MAX_PATH] = {0};
	HRESULT hr = ::SHGetSpecialFolderPath(NULL, moduleFullName, CSIDL_PROFILE, FALSE);
	if(_tcslen(moduleFullName) <=0)
		return FALSE;

	CString sPath(moduleFullName);
	//sPath = sPath.Left(sPath.ReverseFind('\\'));
	CString sUninstall;
	sUninstall.Format(_T("%s%s"), sPath, UCCMEETING_CONFIG);

	FILE* hFile;
	TCHAR szfile[MAX_PATH];
	memset( szfile, 0, sizeof(szfile) );
	wcscpy( szfile, sUninstall.GetBuffer(0) );
	if ( _wfopen_s(&hFile,szfile,_T("r+"))!=0 )
	{
		LOG_ERR(L"Read hosts file failed!");
		return FALSE;
	}

	char* buf = new char[1024*1024];
	size_t nReadCount = fread(buf,sizeof(char),1024*1024,hFile);
	buf[nReadCount] = '\0';
	CString sContent(buf);
	fclose(hFile);
	delete [] buf;
	buf = NULL;

	sContent.Replace(_T("needUpdate : true"), _T("needUpdate : false"));
	if ( _wfopen_s(&hFile,szfile,_T("w+"))!=0 )
	{
		LOG_ERR(L"Write hosts file failed!");
		return FALSE;
	}
	fseek(hFile,0,0);
	buf = new char[1024*1024];
	WideCharToMultiByte(CP_ACP,0,sContent.GetBuffer(0),sContent.GetLength(),buf,1024*1024,0,0);
	fwrite(buf,sizeof(char),sContent.GetLength(),hFile);
	fclose(hFile);
	delete [] buf;

	return TRUE;
}

//记录日志存放目录
BOOL Util::WriteLogCfg( CString sCfgPath )
{
	LOG_INFO(L"Util::WriteLogCfg enter, cfgPath = %s", sCfgPath);
	BOOL bSuccess = FALSE;
	//获取UC Log文件存放目录
	TCHAR moduleFullName[MAX_PATH] = {0};
	BOOL bRet = ::SHGetSpecialFolderPath(NULL, moduleFullName, CSIDL_PERSONAL, FALSE);		
	if( bRet == FALSE || _tcslen(moduleFullName) <= 0 )
	{
		return bRet;
	}
	CString sLogPath;
	sLogPath.Format(_T("%s%s"), moduleFullName, UCLOG_PATH);

	//修改LogConfig.xml中LogFolderDir结点的值 
	USES_CONVERSION;
	char * pFileName = T2A(sCfgPath); 
	TiXmlDocument* pCfg = new TiXmlDocument( pFileName);// 创建状态文件
	bool bret = pCfg->LoadFile(pFileName);
	if (bret)
	{
		TiXmlElement* root = pCfg->FirstChildElement( "LogConfig" );
		if ( root )
		{
			TiXmlElement* child = root->FirstChildElement("LogFolderDir");
			if (child)
			{
				child->Clear();
				TiXmlText* pValue1 = new TiXmlText(T2A(sLogPath));
				child->LinkEndChild(pValue1);
				pCfg->SaveFile(pFileName);
				bSuccess = TRUE;
			}			
		}
	}

	delete pCfg;
	return bSuccess;
}
#ifdef DDD

BOOL Util::DeleteLnkFromProgramsMenu(LPCTSTR pszDir)
{
    //删除开始菜单
    TCHAR   szPath[MAX_PATH];
    SHGetSpecialFolderPath(NULL,szPath,CSIDL_COMMON_PROGRAMS,FALSE);
    _tcscat(szPath, pszDir);
    _tcscat(szPath, L"\\0");
    return Util::DeleteFolder(szPath);
}

BOOL Util::DeleteLnkFromDesktop(LPCTSTR pszLnk)
{
    TCHAR   szPath[MAX_PATH];
    //删除桌面快捷方式
    SHGetSpecialFolderPath(NULL,szPath,CSIDL_COMMON_DESKTOPDIRECTORY,FALSE);
    _tcscat(szPath, pszLnk);
    return Util::DeleteLnk(szPath);
}




#endif