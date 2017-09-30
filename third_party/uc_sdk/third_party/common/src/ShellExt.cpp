#include "ShellExt.h"

#include <sstream>
#include <shlobj.h>
#include <shellapi.h>

namespace uc {

	bool ShellExt::IsOsVersionVistaOrGreater() {
		OSVERSIONINFOEX    ovex;
		CHAR  szVersionInfo[1024];
		*szVersionInfo = '\x00';
		ovex.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

		if (!GetVersionEx((LPOSVERSIONINFO)(&ovex))) {
			return false;
		}

		if (ovex.dwMajorVersion > 5) {
			return true;
		}
		return false;
	}

	void ShellExt::ShellExecuteExOpen(
		const std::wstring& appName, const std::wstring& appPath, 
		const std::wstring& parameters,
		int showMode /*= SW_SHOWNORMAL*/, bool isAdmin /*= false*/, 
		bool isSync /*= true*/, int timeout /*= INFINITE*/) {
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
			if (ShellExecuteEx(&sei)) {
				if (isSync) {
					WaitForSingleObject(sei.hProcess, timeout);
					CloseHandle(sei.hProcess);
				}
			}
		}
		else {
			PROCESS_INFORMATION ProcessInfo;
			STARTUPINFO StartupInfo;
			ZeroMemory(&StartupInfo, sizeof(StartupInfo));
			StartupInfo.cb = sizeof StartupInfo;
			//Only compulsory field
			std::wstringstream ss;
			ss << appPath << "\\" << appName;
			if (CreateProcessW((LPWSTR)ss.str().c_str(), (LPWSTR)parameters.c_str(), 
				NULL, NULL, FALSE, 0, NULL, (LPWSTR)appPath.c_str(), &StartupInfo, &ProcessInfo)) {
				if (isSync) {
					WaitForSingleObject(ProcessInfo.hProcess, timeout);
					CloseHandle(ProcessInfo.hThread);
					CloseHandle(ProcessInfo.hProcess);
				}
			}
		}
	}
}