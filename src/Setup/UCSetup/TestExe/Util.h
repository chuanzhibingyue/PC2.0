#pragma once

class Util
{
public:
    Util(void);
public:
    ~Util(void);
    static BOOL WriteInstallInfoToReg(TCHAR *pDir, TCHAR *sVersion, TCHAR* sProductCode, BOOL bAutoRun = TRUE, BOOL bRunAfterInstall = TRUE, BOOL bDeskLnk=TRUE, BOOL bStartLnk=TRUE);
	static BOOL ModifyInstallPolicy(LPCTSTR sMsiName, BOOL bAddorDel);
	static BOOL GetInstallInfoFromReg(TCHAR** ppDir,BOOL &bAutoRun, BOOL &bRunAfterInstall, BOOL &bDeskLnk, BOOL &bStartLnk, BOOL &bOld);
	static int GetProductCode(TCHAR** ppProductCode);
	static BOOL GetProductVersion(TCHAR** ppVersion);
	static BOOL DeleteInstallInfoFromReg(BOOL bDeleteOld = FALSE);
    static BOOL DeleteLnk( LPCTSTR lpszShortcut);
    static BOOL CreateLnk(LPCTSTR szPath, LPCTSTR szLink, LPCTSTR szParam);
    static BOOL DeleteFolder(LPCTSTR pszFolder, BOOL bAllowUndo = FALSE);
    static BOOL DeleteLnkFromProgramsMenu(LPCTSTR pszDir);
	static BOOL DeleteLnkFromQuickStartMenu(LPCTSTR pszLnk);
    static BOOL DeleteLnkFromDesktop(LPCTSTR pszLnk);
    static BOOL CreateDesktopLnk(LPCTSTR pszTargetPath, LPCTSTR pszLnk);
    static BOOL CreateLnkToProgramsMenu(LPCTSTR pszTargetPath, 
        const TCHAR *pszUninstallPath,
        const TCHAR *pszDir,
        const TCHAR *pszUninstallLnk,
        const TCHAR *pszTargetLnk
        );
	static BOOL CreateLnkToQuickStartMenu(LPCTSTR pszTargetPath, const TCHAR *pszTargetLnk);
    static BOOL MakePath(TCHAR *pPath, TCHAR *pName);
    static BOOL IsEndWithBackSlashOrSlash(LPCTSTR lpPath) ;
    static VOID DeleteFiles(LPCTSTR lpDir, BOOL bDelDir, LPCTSTR lpException) ;
	static VOID CreateDir(CString strDesPath);
	static BOOL FolderExist(CString strPath);
	static BOOL FileExist(CString strPath);
    static BOOL AppAlreadyExist();
    static BOOL UninstallFromControlPannel();
    static BOOL MockLButtonDown(LPCTSTR lpWndName, LPCTSTR lpBtnName);

    static BOOL NeedTransferDir();
    static HRESULT DoTransferDir();
    static BOOL RunNewSetup();
    static DWORD JudgeOSVersion(void);
    static BOOL MockLButtonDown();
	//安裝、御载更新监听服务
	static BOOL InstallUpdateSvc(CString sInstallDir);
	static BOOL UnInstallUpdateSvc();
	static BOOL WriteHosts();
	static BOOL ReadConfig(CString& strConfig);
	static BOOL CleanHosts();
	static BOOL ChangMeetingCfg();
	static BOOL WriteLogCfg(CString sfilepath);
public:
	static CString m_sOnCloudPath;
};
