/******************************************************************************
* Copyright (c) 2009~2010 All Rights Reserved by
*  G-Net Integrated Service co. Ltd. 

******************************************************************************/
/**
* @file UpdateManager.h 
* @brief 升级管理
* 
* @author Wei Chen (wei.chen@gnetis.com)
*
* @date Jun 18 2010
*
* @version 1.0.0
* 
* @warning null
*
* Revision History
* @if  CR/PR ID    Author          Date            Major Change @endif
* @bug 

******************************************************************************/
#pragma once
#include "HttpOperator.h"
#include "UpdateAdaptor.h"
#include "UpdateXml.h"
#include <UpdateManager.h>
#include "FolderOperator.h"

enum 
{
	UPGRADE_DEFAULT = 0,
	UPGRADE_CHECK,
	UPGRADE_DOWNLOAD,
	UPGRADE_INSTALL,
};

class UpdateManagerImpl : public UpdateManager, public IDownloadCallback
{
public:
	~UpdateManagerImpl(void);

    /**
     * Get pointer of UpdateManager as Singleton Design Mode.
     * @param 
     * @return UpdateManager*: return the pointer of UpdateManager.
	 * @exception 
	 * @pre 
	 * @post 
	 * @par 
	 * @code

	 * @endcode
	 * @since 
	 * @see 
     */
    static UpdateManagerImpl& 
    GetInstance(
    );

    /**
	* @brief Initialize UpdateManager instance.
     * @param [in] pMsgProvider: IMessageProvider point instance.
			  [in] pExternal: IExternal point instance.
	 * @return HRESULT: return S_OK if successful.
	 * @exception 
	 * @pre 
	 * @post 
	 * @par 
	 * @code

	 * @endcode
	 * @since 
	 * @see 
     */
	 HRESULT Init(void* userdata,
				  CheckUpdateCallBack checkCallBack,
				  ProgressCallBack processCallBack,
				  ErrorCallBack errCallBack, 
				  BOOL isBee = FALSE);

    /**
	 * @brief Uninitialize UpdateManager instance.
     * @param 
	 * @return HRESULT: return S_OK if successful.
	 * @exception 
	 * @pre 
	 * @post 
	 * @par 
	 * @code

	 * @endcode
	 * @since 
	 * @see 
     */
	HRESULT UnInit();

    /**
	 * @brief Setting UpdateAdaptor point of XML document.
     * @param [in] pXmlDoc: XML document instance, UI put the point.
	 * @return HRESULT: return S_OK if successful.
	 * @exception 
	 * @pre 
	 * @post 
	 * @par 
	 * @code

	 * @endcode
	 * @since 
	 * @see 
     */
	virtual HRESULT SetXmlDocument( IUnknown * pXmlDoc );

    /**
	 * @brief Remove UpdateAdaptor point document.
     * @param [in] pXmlDoc: XML document instance, UI put the point.
	 * @return HRESULT: return S_OK if successful.
	 * @exception 
	 * @pre 
	 * @post 
	 * @par 
	 * @code

	 * @endcode
	 * @since 
	 * @see 
     */
	virtual HRESULT RemoveXmlDocument( IUnknown * pXmlDoc );

	/**
	* @brief UI层调用该方法检测是否需要升级。
	该方法为异步调用，如果需要升级，升级的具体信息将更新到UpdateXml中。
	* @param [in] uAppId: Application id.
	[in] uSiteId: Site id.
	[in] uSkinId: Application skin id.
	[in] szServerUrl: Update server url.
	[in] uHttpPort: Update server http port.
	[in] uTimeout: Time out.
	* @return HRESULT: return S_OK if successful.
	* @exception
	* @pre
	* @post
	* @par
	* @code

	* @endcode
	* @since
	* @see
	*/
	virtual HRESULT CheckUpdates(UINT uAppId, UINT uSiteId, UINT uSkinId,
		BSTR szServerUrl, UINT uHttpPort, UINT uTimeout, BSTR szCmdLine);

    /**
	 * @brief UI层调用该方法进行升级操作
     * @param [in] uAppId: Application id.
			  [in] uSiteId: Site id.
			  [in] uSkinId: Application skin id.
			  [in] szServerUrl: Update server url.
			  [in] uHttpPort: Update server http port.
			  [in] uTimeout: Time out.
	 * @return HRESULT: return S_OK if successful.
	 * @exception 
	 * @pre 
	 * @post 
	 * @par 
	 * @code

	 * @endcode
	 * @since 
	 * @see 
     */
	virtual HRESULT Upgrade( UINT uAppId, UINT uSiteId, UINT uSkinId,
		BSTR szServerUrl, UINT uHttpPort, UINT uTimeout,BSTR szCmdLine );
	virtual HRESULT RealUpdate(BSTR szParam);
	virtual HRESULT CheckRunningClient(int *pCount){return S_OK;}
    /**
	 * @brief 在检测完升级信息后，如果需要升级，调用该方法启动升级包的下载。
			  该方法为异步方法，下载进度信息会即时更新到UpdateXml中。
	 * @param pXmlDoc: XML document instance, UI put the point.
	 * @return HRESULT: return S_OK if successful.
	 * @exception 
	 * @pre 
	 * @post 
	 * @par 
	 * @code

	 * @endcode
	 * @since 
	 * @see 
     */
	virtual HRESULT Download();

    /**
	 * @brief 在下载过程中，取消升级包的下载。
	 * @param 
	 * @return HRESULT: return S_OK if successful.
	 * @exception 
	 * @pre 
	 * @post 
	 * @par 
	 * @code

	 * @endcode
	 * @since 
	 * @see 
     */
	virtual HRESULT CancelDownload();

    /**
	 * @brief 升级包下载完毕后，调用该方法安装升级包。该方法为异步方法，安装进度信息会即时更新到UpdateXml中。
	 * @param 
	 * @return HRESULT: return S_OK if successful.
	 * @exception 
	 * @pre 
	 * @post 
	 * @par 
	 * @code

	 * @endcode
	 * @since 
	 * @see 
     */
	virtual HRESULT Install();

    /**
	 * @brief 升级包安装完毕后，会由于有些文件正在被使用而无法安装成功，
			需要重新启动后才能完成安装，调用该方法来重启客户端。
			该方法为同步方法，会立即重新启动客户端。
	 * @param [in] szTangCmd: 需要传递给TangClient的命令参数行
	 * @return HRESULT: return S_OK if successful.
	 * @exception 
	 * @pre 
	 * @post 
	 * @par 
	 * @code

	 * @endcode
	 * @since 
	 * @see 
     */
	virtual HRESULT Restart( BSTR szTangCmd );

	//added by dandan.wu 2012/6/27
	/**
	 * @brief 以快捷方式入会后，得到不同的siteid，若该站点下有TangClient.exe,
	        则要退出当前站点下的TangClient.exe，启动新站点下的TangClient.exe，
			调用此方法来开启客户端。此方法为同步方法，会立即重新启动客户端。
	 * @param [in] szTangCmd: 需要传递给TangClient的命令参数行
	 * @return HRESULT: return S_OK if successful.
	 * @exception 
	 * @pre 
	 * @post 
	 * @par 
	 * @code

	 * @endcode
	 * @since 
	 * @see 
     */
	virtual HRESULT StartAnotherSite( CString szTangCmd );

    /**
	 * @brief 删除一个文件。
	 * @param [in] szFullFileName: File full path and file name.
	 * @return HRESULT: return S_OK if successful.
	 * @exception 
	 * @pre 
	 * @post 
	 * @par 
	 * @code

	 * @endcode
	 * @since 
	 * @see 
     */
	virtual HRESULT RemoveFile( BSTR szFullFileName );

    /**
	 * @brief 删除一个文件夹。
	 * @param [in] szFullFolderName: Folder full path.
	 * @return HRESULT: return S_OK if successful.
	 * @exception 
	 * @pre 
	 * @post 
	 * @par 
	 * @code

	 * @endcode
	 * @since 
	 * @see 
     */
	virtual HRESULT RemoveFolder( BSTR szFullFolderName );

    /**
	 * @brief 向注册表增加或者修改一个URL Protocol。如果Protocol存在，则修改；否则，增加。
	 * @param [in] szProtocol: URL Protocol name.
			  [in] szFullAppName: Application full path and file name.
	 * @return HRESULT: return S_OK if successful.
	 * @exception 
	 * @pre 
	 * @post 
	 * @par 
	 * @code

	 * @endcode
	 * @since 
	 * @see 
     */
	virtual HRESULT UpdateUrlProtocol( BSTR szProtocol,	BSTR szFullAppName );

    /**
	 * @brief 从注册表删除一个URL Protocol。
	 * @param [in] szProtocol: URL Protocol name.
	 * @return HRESULT: return S_OK if successful.
	 * @exception 
	 * @pre 
	 * @post 
	 * @par 
	 * @code

	 * @endcode
	 * @since 
	 * @see 
     */
	virtual HRESULT RemoveUrlProtocol( BSTR szProtocol );

    /**
	 * @brief 向桌面增加或者修改一个快捷方式。
	 * @param [in] szName: Menu name.
			  [in] szFullAppName: Application full path and file name.
			  [in] szCmdArgs: Application command arguments.
			  [in] szIco: Application ico picture.
	 * @return HRESULT: return S_OK if successful.
	 * @exception 
	 * @pre 
	 * @post 
	 * @par 
	 * @code

	 * @endcode
	 * @since 
	 * @see 
     */
	virtual HRESULT UpdateDesktopLink( BSTR szName, BSTR szFullAppName, BSTR szCmdArgs, BSTR szIco );

    /**
	 * @brief 从桌面删除一个快捷方式。
	 * @param [in] szName: Desktop ico name.
	 * @return HRESULT: return S_OK if successful.
	 * @exception 
	 * @pre 
	 * @post 
	 * @par 
	 * @code

	 * @endcode
	 * @since 
	 * @see 
     */
	virtual HRESULT RemoveDesktopLink ( BSTR szName);

    /**
	 * @brief 向开始程序增加或者修改一个菜单。如果Name存在，则修改；否则，增加。
	 * @param [in] szName: Menu name.
			  [in] szFullAppName: Application full path and file name.
			  [in] szCmdArgs: Application command arguments.
			  [in] szIco: Application ico picture.
	 * @return HRESULT: return S_OK if successful.
	 * @exception 
	 * @pre 
	 * @post 
	 * @par 
	 * @code

	 * @endcode
	 * @since 
	 * @see 
     */
	virtual HRESULT UpdateStartMenu( BSTR szName, BSTR szFullAppName, BSTR szCmdArgs, BSTR szIco );

    /**
	 * @brief 从开始程序删除一组菜单。
	 * @param [in] szName: Menu name. 
	 * @return HRESULT: return S_OK if successful.
	 * @exception 
	 * @pre 
	 * @post 
	 * @par 
	 * @code

	 * @endcode
	 * @since 
	 * @see 
     */
	virtual HRESULT RemoveStartMenu ( BSTR szName );

    /**
	 * @brief 读取Cookie的值。
	 * @param [in] szUrl: URL name.
			  [in] szName: Cookie name.
			  [in] szContent: Cookie context.
	 * @return HRESULT: return S_OK if successful.
	 * @exception 
	 * @pre 
	 * @post 
	 * @par 
	 * @code

	 * @endcode
	 * @since 
	 * @see 
     */
	virtual HRESULT ReadCookie( BSTR szUrl, BSTR szName, BSTR* szContent );

    /**
	 * @brief 增加或者修改一个Cookie。如果URL和Name存在，则修改；否则，增加。
	 * @param [in] szUrl: URL name.
			  [in] szName: Cookie name.
			  [in] szContent: Cookie context.
	 * @return HRESULT: return S_OK if successful.
	 * @exception 
	 * @pre 
	 * @post 
	 * @par 
	 * @code

	 * @endcode
	 * @since 
	 * @see 
     */
	virtual HRESULT UpdateCookie( BSTR szUrl, BSTR szName, BSTR szContent );

    /**
 	 * @brief 删除一个Cookie。
	 * @param [in] szUrl: URL name.
			  [in] szName: Cookie name.
	 * @return HRESULT: return S_OK if successful.
	 * @exception 
	 * @pre 
	 * @post 
	 * @par 
	 * @code

	 * @endcode
	 * @since 
	 * @see 
     */
	virtual HRESULT RemoveCookie( BSTR szUrl, BSTR szName );

	virtual HRESULT SetSiteId( UINT nSiteId );
	virtual HRESULT GetSiteId( UINT* pSiteId );
	virtual HRESULT SetVersion( BSTR bstrVersion );
	virtual HRESULT GetVersion( BSTR* pbstrVersion );
	virtual HRESULT SetLanguage( BSTR bstrLanguage );
	virtual HRESULT GetLanguage( BSTR* pbstrLanguage );
	virtual HRESULT GetInstallPath( BSTR* pbstrPath );

	virtual HRESULT GetUpdateVersion( BSTR* pbstrVersion );
	virtual HRESULT GetAppId( UINT* nAppId );
	virtual HRESULT SetAppId( UINT nAppId );
	virtual HRESULT SetUpdateUrl_Version(BSTR szServerUrl);
	virtual HRESULT SetUpdateUrl_PackageInfo(BSTR szServerUrl);
	virtual HRESULT SetUpdateUrl_PackageDwon(BSTR szServerUrl);
	virtual HRESULT SetLoginInfo(BSTR szInfo);

    virtual HRESULT GetDownloadDomain( BSTR* pbstrDomain );

    /**
	 * @brief interface of IMessageConsumer.
	 * @param [in] pMsg: Message context.
	 * @return HRESULT: return S_OK if successful.
	 * @exception 
	 * @pre 
	 * @post 
	 * @par 
	 * @code

	 * @endcode
	 * @since 
	 * @see 
     */
	virtual HRESULT ConsumeMessage( const Message* pMsg );

public:
	static DWORD WINAPI
		CheckUpdatesThreadProc(
		__in LPVOID lpParam
		);

	HRESULT	CheckUpdatesProc();

	/**
     * The callback function of work thread for downloading.
     * @param nore.
     * @return DWORD: The return value of callback function, not to been noted.
     */
    static DWORD WINAPI
    UpgradeThreadProc(
        __in LPVOID lpParam
        );

	HRESULT	UpgradeProc();

	static DWORD WINAPI
		DownloadThreadProc(
		__in LPVOID lpParam
		);

	HRESULT	DownloadProc();

	static DWORD WINAPI
		InstallThreadProc(
		__in LPVOID lpParam
		);

	HRESULT	InstallProc();

	HRESULT MergeConfig(CString strLocalConfig, CString strUpdateConfig, CString& downLoadReleaseId);

	HRESULT DoResult(UINT nPercent);

	BOOL IsCancel();

public:
	HRESULT CreateInstallScript();
	HRESULT CreateInstallXML();
	HRESULT UpdateDownloadProgress(UINT nProgress);
	HRESULT GetLocalInfo(CString& strVersion);
	HRESULT GetLocalInfo(CString& strSiteId, CString& strVersion);
	HRESULT Initialize();
	HRESULT ResolveIt(LPCTSTR lpszLinkFile, LPCTSTR lpszPath, LPCTSTR lpszWorkingDir = _T(""), 
		LPTSTR lpszArguments = _T(""), CString lpszIconPath = _T(""));
	HRESULT AddError(DWORD dwResult);
	HRESULT ReadVersion(std::wstring strConfigFileName, CString& strVersion);
	HRESULT GetLocalVersion(std::wstring& strVersion);
	HRESULT CreateTangRoute(CString strAppPath, LPTSTR lpstrCmdLine, DWORD& dwResult);
	BOOL FindModule(CString strModuleName);
	CString GetFileVersion(CString strFile);
	BOOL UpdateVPrinterDriver();
	HRESULT ExecuteVPrinter();
	
	/*added by dandan.wu 2012/6/27
	 *创建一个批处理文件，先终止进程DaemonApp.exe，再终止进程TangClient.exe
	 */
	HRESULT CreateCheckUpdateScript();
	void KillReleateProcess();

	HRESULT CheckCommons(UINT uAppId, UINT uSiteId,
		UINT uSkinId, BSTR szServerUrl,
		UINT uHttpPort, UINT uTimeout, BSTR szCmdLine);

	HRESULT CommonsProc(DWORD& dwResult);

	HRESULT InitCommon();

	HRESULT InitBaseInfo();

private:
	UpdateManagerImpl();
	UpdateManagerImpl(const UpdateManagerImpl &);
	UpdateManagerImpl & operator = (const UpdateManagerImpl &);

	void KillBee();
	void StopExplorer();

	BOOL IsUpdateFile(CString fileName);

	BOOL FileExist(LPCWSTR strPath);

	void ForceDelFile(CString fileName);

	void DelInvalidFiles();

	void GetInstallDirFiles(const std::wstring& dir, UPDATEFILEVECTOR& files);

	void FileTracker(const UPDATEFILEVECTOR& files);

public:
	// 需要升级的目录
	CString m_strTargetPath;
	BOOL	m_bAnotherSite;
	// UI传入参数
	CString m_strAppId;
	CString m_strSiteId;
	CString m_strSkinId;
	CString m_strVersion;
	CString	m_strUpdateUrlVersion;
	CString	m_strUpdateUrlPackageInfo;
	CString	m_strUpdateUrlPackageDown;
	CString	m_sLoginInfo;
	CString	m_strHttpPort;
	//added by dandan.wu 2012/6/27
	CString m_strCmdLine;

	CString				m_strAppPath;
	CString				m_strBasePath;
	CString				m_strUpdateConfigName;


	UpdateConfig		m_updateConfig;//added by jianhua.chen
	UpdateConfig		m_upgradeConfig;
	
private:
	CString				m_strScriptFile;
	IExternal*			m_pExternal;
	IMessageProvider*	m_pMsgProvider;
	UpdateAdaptor*		m_pAdaptor;
	UpdateXml*			m_pXml;
	UpdateConfig*		m_pUpdates;
	UpdateConfig*		m_pCopyFiles;
	HttpOperator*		m_pHttp;
	CFolderOperator		m_folder;
	//added by dandan.wu 2012/6/28
	CString m_strStopScriptFile;

	UINT m_uTimeout;

	// 各线程句柄
	HANDLE	m_tdCheckUpdates;
	BOOL	m_bCheckUpdates;
	BOOL	m_bCancelCheckUpdates;

	HANDLE  m_tdDownload;
	BOOL	m_bDownloading;
	BOOL	m_bCancelDownloading;
	HANDLE	m_tdInstall;
	BOOL	m_bInstalling;
	BOOL	m_bCancelInstalling;

	void* m_userData;
	CheckUpdateCallBack m_checkCallBack;
	ProgressCallBack m_progressCallBack;
	ErrorCallBack m_errCallback;
};

class Compare
{
public:
	bool operator()(const UpdateFile& lhs, const UpdateFile& rhs)
	{
		return lhs.m_strFileFullName < rhs.m_strFileFullName;
	}
};
