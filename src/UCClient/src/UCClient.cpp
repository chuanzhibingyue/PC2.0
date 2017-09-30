#include "UCManager.h"
#include <windows.h>

#include "CefClient.h"
#include "UCExtension.h"
#include "utils/FileUtil.h"
#include "ClientScheme.h"
#include "LogManager.h"
#include "UCDefs.h"
#include "Configuration.h"
#include "utils/Utils.h"
#include "gkclient/GKClientExport.h"
#include "common/ShellExt.h"

HINSTANCE hInst;
DWORD  mainThreadId;

using namespace uc;

void HandleUpdateFile() {
	TCHAR path[MAX_PATH] = { 0 };
	std::wstring strDir;
	std::wstring strUpdateTempDir;
	std::wstring strBatPath;

	::GetModuleFileName(NULL, path, MAX_PATH);
	strDir = path;
	size_t pos = strDir.find_last_of(L'\\');
	if (pos > 0) {
		strDir = strDir.substr(0, pos);
	}

	strUpdateTempDir = strDir;
	strBatPath = strDir;
	strBatPath.append(L"\\CopyScript.bat");

	//执行批处理文件来copy升级相关文件
	if (-1 != GetFileAttributes(strBatPath.c_str())) {
		SHELLEXECUTEINFO execInfo;
		ZeroMemory(&execInfo, sizeof(SHELLEXECUTEINFO));
		execInfo.cbSize = sizeof(SHELLEXECUTEINFO);
		execInfo.fMask = SEE_MASK_FLAG_DDEWAIT | SEE_MASK_NOCLOSEPROCESS;
		execInfo.lpVerb = L"open";
		execInfo.lpFile = L"cmd.exe";
		execInfo.nShow = SW_HIDE;
		WCHAR sParam[MAX_PATH] = { 0 };
		wsprintf(sParam, L"/C \"\"%s\"\"", strBatPath.c_str());
		execInfo.lpParameters = sParam;
		bool ret = ::ShellExecuteEx(&execInfo);
		if (ret && execInfo.hProcess != NULL) {
			::WaitForSingleObject(execInfo.hProcess, INFINITE);
			::CloseHandle(execInfo.hProcess);
		}
	}

	strUpdateTempDir.append(L"\\UpdateTemp");
	TCHAR arrDir[MAX_PATH] = { 0 };
	strUpdateTempDir.copy(arrDir, strUpdateTempDir.length(), 0);
	SHFILEOPSTRUCT    shFileOp;
	ZeroMemory((void*)&shFileOp, sizeof(SHFILEOPSTRUCT));
	shFileOp.wFunc = FO_DELETE;
	shFileOp.pFrom = arrDir;
	shFileOp.pTo = NULL;
	shFileOp.fFlags = FOF_SILENT | FOF_NOCONFIRMATION | FOF_NOCONFIRMMKDIR;
	SHFileOperation(&shFileOp);
}

wstring GetSendPathFromCmd(const wstring& cmdLine) {
	UC_LOG(INFO) << "GetSendPathFromCmd, cmdLine:" << cmdLine;

	const wstring sendMark = L"SendFileToBee:";
	size_t pos = cmdLine.find(sendMark);
	if (pos != string::npos) {
		return cmdLine.substr(pos + sendMark.length());
	}
	return L"";
}

bool IsRegShellExt() {
	std::wstring shellExtID;
	HKEY hAppKey = NULL;
	WCHAR buffer[MAX_PATH] = { 0 };
	DWORD bufferLen = MAX_PATH;

	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE,
		L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Shell Extensions\\Approved",
		0,
		KEY_READ | KEY_WOW64_64KEY,
		&hAppKey) == ERROR_SUCCESS) {
		::RegQueryValueEx(hAppKey, L"CopyLocationShl extension", NULL, NULL, reinterpret_cast<LPBYTE>(&buffer), &bufferLen);
		RegCloseKey(hAppKey);
	}
	else {
		UC_LOG(ERROR) << "RegOpenKeyEx error:" << GetLastError();
	}

	shellExtID = buffer;
	if (shellExtID.empty()) {
		return false;
	}
	return true;
}

void RegShellExt(const std::wstring& path) {
	UC_LOG(INFO) << "Need RegShellExt!";
	std::wstring parameter = L"type:0" + path;
	ShellExt::ShellExecuteExOpen(std::wstring(L"BeeRegMgr.exe"), path, parameter);

	UC_LOG(INFO) << "RegShellExt, path:" << path << ", parameter:" << parameter;
}

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow) {
	hInst = hInstance;
	mainThreadId = ::GetCurrentThreadId();

	HandleUpdateFile();

    std::wstring processPath;
	ucclient::FileUtil::ProcessPath(processPath);
	std::wstring cefSubProcessPath = processPath + L"\\bee.worker.exe";
	std::wstring cefLogPath = ucclient::FileUtil::GetAppDataPath() + L"\\Quanshi\\Bee\\log\\";
	std::wstring cefLogFile = cefLogPath + L"cef_log.txt";
	std::wstring cefOldLogFile = cefLogPath + L"cef_log_old.txt";
	std::wstring sendFilePath = GetSendPathFromCmd(lpCmdLine);

	UC_LOG(INFO) << "cefSubProcessPath: " << cefSubProcessPath;

	// handle cef log
	ucclient::FileUtil::CreateFolderRecursively(cefLogPath);
	DeleteFileW(cefOldLogFile.c_str());
	MoveFileExW(cefLogFile.c_str(), cefOldLogFile.c_str(), 0);

	// get settings (mutil-process, curl-log)
	std::wstring modulePath;
	ucclient::FileUtil::ProcessPath(modulePath);
	std::wstring configPath = modulePath + L"\\config\\bee.config";
	ucclient::Configuration xmlConfig(configPath);

	std::string multiInstance = xmlConfig.QueryStringValue("/configuration/multi-instance", "false");
	std::wstring title = StringConvert::FromUTF8Str(xmlConfig.QueryStringValue("/configuration/window/title", "bee"));
	HANDLE hMutex = CreateMutex(NULL, TRUE, L"quanshi_bee_mutex_app");
	Utils::ON_SCOPE_EXIT([&]{CloseHandle(hMutex); });
	//keep only one running bee.exe
	if (multiInstance.compare("false") == 0)
	{
		if (hMutex && ERROR_ALREADY_EXISTS == GetLastError()) {
			HWND hWnd = FindWindow(UCCLIENT_CLASS_NAME, title.c_str());
			if (hWnd != NULL) {
 				PostMessage(hWnd, WM_SYSCOMMAND, SC_RESTORE, 0);

				if (!sendFilePath.empty()) {
					TCHAR filePath[MAX_PATH] = { 0 };
					wcscpy(filePath, sendFilePath.c_str());
					COPYDATASTRUCT cds;
					cds.dwData = 0;
					cds.lpData = filePath;
					cds.cbData = (_tcslen(filePath) + 1)*sizeof(TCHAR);
					SendMessage(hWnd, WM_COPYDATA, (WPARAM)hInstance, (LPARAM)&cds);
				}

				DWORD dwThreadProcessID = GetWindowThreadProcessId(::GetForegroundWindow(), NULL);
				AttachThreadInput(dwThreadProcessID, GetCurrentThreadId(), TRUE);
				::SetForegroundWindow(hWnd);
				AttachThreadInput(dwThreadProcessID, GetCurrentThreadId(), FALSE);
			}

			//CloseHandle(hMutex);
			UC_LOG(ERROR) << "Another running bee.exe, so exit....";
			return 0;
		}
	}

	ucclient::UCManager::GetInstance().SetDefaultWinTitle(title);

	//launch executation process
	cefIntegration::CefClient *cefClient = new cefIntegration::CefClient();
	cefClient->InitializeMainProcess(hInstance, cefSubProcessPath, cefLogFile);

	// initiate and load login UI
	ucclient::UCManager::GetInstance().Initialize(cefClient);

	// 注册javascript扩展
	cefClient->RegisterBrowserDelegate(new ucclient::UCExtension());
	
	CefRegisterSchemeHandlerFactory("client", "javascript", new ucclient::ClientSchemeHandlerFactory(cefClient));

	//是否需要注册右键扩展菜单
	if (!IsRegShellExt()) {
		RegShellExt(processPath);
	}
	
	ucclient::UCManager::GetInstance().LoadDefaultHtml();

	if (!sendFilePath.empty()) {
		ucclient::UCManager::GetInstance().GetClientServiceStrategy()->SetSendFilePath(sendFilePath);
	}

	// 在主线程启动CEF消息循环
	CefRunMessageLoop();

	UC_LOG(WARNING) << "cef message loop is quit, now unInitialize UCManager";

	//release resource
	//CloseHandle(hMutex);
	ucclient::UCManager::GetInstance().UnInitialize();
	cefClient->Shutdown();
	delete cefClient;

	return 0;
}
