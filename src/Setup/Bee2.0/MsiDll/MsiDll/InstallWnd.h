#include "HtmlControl.h"
#include "UIText.h"
class InstallWnd : public HtmlControl
{
public:
	InstallWnd();
	const char* OnFoo(int argc, const char* argv[]);
	void OnGetInstallDir(DISPPARAMS*, VARIANT*);			//获取安装路
	void OnSetCallBackHandler(DISPPARAMS*, VARIANT*);		//js注册回调方法
	void OnInstall(DISPPARAMS*, VARIANT*);					//开始安装
	void OnUnistall(DISPPARAMS*, VARIANT*);					//开始卸载
	void OnCheckInstalled(DISPPARAMS*, VARIANT*);			//检测当前安装状态：0 -- 未安装任何蜜蜂； 1-- 已安装蜜蜂2.0； 2---已经安装当前版本; 3--已经安装同一个包; 4 -- 已安装更新版本
	void OnRecoverInstall(DISPPARAMS*, VARIANT*);			//覆盖安装
	void OnCheckAppRunning(DISPPARAMS*, VARIANT*);			//检测客户端是否正在运行
	void OnSelectFolderDialog(DISPPARAMS*, VARIANT*);		//选择安装目录
	void OnIsOSVersionGreaterXPSP3(DISPPARAMS*, VARIANT*);	//判断当前系统版本号
	void OnIsHighDPI(DISPPARAMS*, VARIANT*);				//判断当前是否高分辨率
	BOOL GetInstallInfoFromReg(BOOL bOnlyInstallDir = FALSE);
public:
	void CreateAllLnk();
	void CleanOldVersion(BOOL bCleanLog = FALSE);
	BOOL WriteInstallInfoToReg();
	void KillRelationExe();
	void CleanInstallResidues(BOOL bCleanLog = FALSE, BOOL bCleanInstallDir = TRUE);
	void SaveInstallCode(char szProductCode[MAX_PATH], char szPackageCode[MAX_PATH], char szVersion[MAX_PATH]);
	static DWORD WINAPI CleanUpThreadProc(__in LPVOID lpParam);
	BOOL InstallMeeting();
	BOOL UninstallMeeting();
	BOOL CheckMeetingInstalled();
	BOOL WriteInstallMeetingFlag();		//在注册表标记云会议是否由蜜蜂安装 
	BOOL CreateDir(LPCWSTR strDesPath);
	void UnInstallOldVersion();
	BOOL DeleteFolder(const TCHAR* pszFolder, BOOL bAllowUndo = FALSE);
	void DeleteFiles(const TCHAR* lpDir, BOOL bDelDir = TRUE, const TCHAR* lpException = NULL);
	BOOL UnZipThirdPartyFile();
	void DelSendActionFromRBtn();
	std::wstring GetTextWithKey(const TCHAR* lpKey);
private:
	void Initialize();			//初始成员变量等
	BOOL GetProductCode(TCHAR** ppProductCode);
	BOOL GetPackageCode(TCHAR** ppPackageCode);
	BOOL GetVersionCode(TCHAR** ppVersionCode);
	void CloseRunningAppbyMsg(TCHAR* lpWndName);
	int FindRunningApp(TCHAR* lpProcName);
	BOOL IsEndWithBackSlashOrSlash(LPCTSTR lpPath);
	BOOL IsEndWithInstallDir(LPCTSTR lpPath);
	BOOL DeleteInstallInfoFromReg(BOOL bOld = FALSE);
	BOOL DeleteLnk(LPCTSTR lpszShortcut);
	BOOL CreateLnk(LPCTSTR szPath, LPCTSTR szLink, LPCTSTR szParam);
	BOOL DeleteLnkFromProgramsMenu(LPCTSTR pszDir);
	BOOL DeleteLnkFromQuickStartMenu(LPCTSTR pszLnk);
	BOOL DeleteLnkFromDesktop(LPCTSTR pszLnk);
	BOOL CreateDesktopLnk(LPCTSTR pszTargetPath, LPCTSTR pszLnk);
	BOOL CreateLnkToProgramsMenu(LPCTSTR pszTargetPath,
		const TCHAR *pszUninstallPath,
		const TCHAR *pszDir,
		const TCHAR *pszUninstallLnk,
		const TCHAR *pszTargetLnk
		);
	BOOL CreateLnkToQuickStartMenu(LPCTSTR pszTargetPath, const TCHAR *pszTargetLnk);
	BOOL CreateLnkToSendTo(LPCTSTR pszTargetPath, const TCHAR* pszLnk);
	BOOL AutoStartSoftware(BOOL bAutoStart = TRUE);
	void CloseRunningApp(TCHAR* lpProcName);
	BOOL EnableDebugPrivilege();
	BOOL FolderExist(LPCWSTR strPath);
	
	// 判断指定的文件是否存在
	BOOL FileExist(LPCWSTR strPath);
	//获取屏幕缩放转换因子
	float GetDeviceScaleFactor();

	void AddSendActionOnRBtn();
	
	bool IsOsVersionVistaOrGreater();

	//重启explorer.exe
	void StopExplorer();

	bool isPathOk(TCHAR* path);
public:
	void ShellExecuteExOpen(const std::wstring& appName, std::wstring& appPath, const std::wstring& parameters = L"", int showMode = SW_SHOWNORMAL, bool isAdmin = true, bool isSync = true, int timeout = INFINITE);

	template <typename T>
	std::basic_string<T>& str_replace(std::basic_string<T>& str, const std::basic_string<T>& old_value, const std::basic_string<T>& new_value) {
		for (typename std::basic_string<T>::size_type pos(0); pos != std::basic_string<T>::npos; pos += new_value.length()) {
			if ((pos = str.find(old_value, pos)) != std::basic_string<T>::npos) {
				str.replace(pos, old_value.length(), new_value);
			}
			else {
				break;
			}
		}
		return str;
	}
public:
	TCHAR m_szProductCode[MAX_PATH];
	TCHAR m_szPackageCode[MAX_PATH];
	TCHAR m_szVersion[MAX_PATH];
	BOOL m_bDeskLnk;		//桌面快捷
	BOOL m_bQuickStartLnk;	//快捷启动栏
	BOOL m_bAutoRun;		//开机自启动
	int m_nRecoverType;		//覆盖安装类型：0 -- 未安装任何蜜蜂； 1-- 已安装老蜜蜂； 2---已经安装当前版本; 3--已经安装同一个包; 4 -- 已安装更新版本
	std::wstring m_IniPath;

};