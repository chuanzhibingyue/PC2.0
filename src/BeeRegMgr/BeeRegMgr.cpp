// BeeRegMgr.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "BeeRegMgr.h"
#include "Shlobj.h"
#include <shellapi.h>
#include <string>

VOID SafeGetNativeSystemInfo(__out LPSYSTEM_INFO lpSystemInfo) {
	if (NULL == lpSystemInfo)
		return;
	typedef VOID(WINAPI *LPFN_GetNativeSystemInfo)(LPSYSTEM_INFO lpSystemInfo);
	LPFN_GetNativeSystemInfo nsInfo =
		(LPFN_GetNativeSystemInfo)GetProcAddress(GetModuleHandle(_T("kernel32")), "GetNativeSystemInfo");;
	if (NULL != nsInfo) {
		nsInfo(lpSystemInfo);
	}
	else {
		GetSystemInfo(lpSystemInfo);
	}
}

BOOL Is64System() {
	SYSTEM_INFO si;
	SafeGetNativeSystemInfo(&si);
	if (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64 ||
		si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_IA64) {
		return TRUE;
	}
	return FALSE;
}

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	std::wstring type;
	std::wstring installPath;
	std::wstring option;
	std::wstring cmd = lpCmdLine;
	const std::wstring mark = L"type:";
	size_t pos = cmd.find(mark);
	if (pos != std::wstring::npos) {
		type = cmd.substr(pos + mark.length(), 1);
		installPath = cmd.substr(pos + mark.length() + 1);
	}

	//0:install 1:unstall
	option = (type == L"0") ? L"/s " : L"/s /u ";
	std::wstring dllType = Is64System() ? L"\\x64" : L"\\win32";
	installPath = installPath + L"\\shellExt" + dllType + L"\\CopyLocation.dll";
	std::wstring parameter = option + installPath;
	SHELLEXECUTEINFO sei = { sizeof(SHELLEXECUTEINFO) };
	sei.fMask = SEE_MASK_NOCLOSEPROCESS;
	sei.lpVerb = TEXT("runas");
	sei.lpFile = L"regsvr32.exe";
	sei.lpParameters = parameter.c_str();
	sei.nShow = SW_SHOWNORMAL;
	BOOL ret = ShellExecuteEx(&sei);
	_ASSERT(ret);

	return 0;
}

