#include "Environment.h"
#include "LogManager.h"
#include "../UCDefs.h"

#include <Shlobj.h>
#include <string.h>


namespace ucclient {

using namespace uc;

const WCHAR* AUTO_START_SUB_REGKEY = L"Bee";
const WCHAR* AUTO_START_REGKEY = L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run";

int32_t Environment::EnableAutoStart() {
	WCHAR buffer[MAX_PATH];
	GetModuleFileName(NULL, buffer, MAX_PATH);

	HKEY key; 
	int32_t res = RegOpenKeyEx(HKEY_CURRENT_USER, AUTO_START_REGKEY,
		0, KEY_ALL_ACCESS | KEY_WOW64_64KEY, &key);
	int x = ::wcslen(buffer);
	if (res == ERROR_SUCCESS) {
		res = RegSetValueEx(key, AUTO_START_SUB_REGKEY, 0, REG_SZ, (BYTE*)buffer, ::wcslen(buffer) * 2);
		RegCloseKey(key);
	}
	
	if (res != ERROR_SUCCESS) {
		UC_LOG(ERROR) << "fail to EnableAutoStart , res: " << res;	
		return res;
	}

	return UCCLIENT_SUCCESS;
}

int32_t Environment::IsAutoStart() {
	HKEY key;
	int32_t res = RegOpenKeyEx(HKEY_CURRENT_USER, AUTO_START_REGKEY,
		0, KEY_ALL_ACCESS | KEY_WOW64_64KEY, &key);
	if (res == ERROR_SUCCESS) {
		WCHAR buffer[MAX_PATH];
		DWORD dataSize;
		DWORD dataType;
		res = RegQueryValueEx(key, AUTO_START_SUB_REGKEY, 0, &dataType, (BYTE*)buffer, &dataSize);
		RegCloseKey(key);
	}

	if (res != ERROR_SUCCESS) {
		UC_LOG(ERROR) << "RegQueryValueEx Bee error , res: " << res;
		return res;
	}

	return UCCLIENT_SUCCESS;
}

int32_t Environment::DisableAutoStart() {
	HKEY key;
	int32_t res = RegOpenKeyEx(HKEY_CURRENT_USER, AUTO_START_REGKEY,
		0, KEY_ALL_ACCESS | KEY_WOW64_64KEY, &key);
	if (res == ERROR_SUCCESS) {
		res = RegDeleteValue(key, AUTO_START_SUB_REGKEY);
		RegCloseKey(key);
	}

	if (res != ERROR_SUCCESS) {
		UC_LOG(ERROR) << "fail to DisableAutoStart , res: " << res;
		return res;
	}

	return UCCLIENT_SUCCESS;
}

}