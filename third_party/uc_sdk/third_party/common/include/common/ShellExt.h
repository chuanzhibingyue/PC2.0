#ifndef SHELL_EXT_H
#define SHELL_EXT_H

#include <string>

#include "common/platform.h"

namespace uc {

using namespace std;

class UC_API ShellExt
{
public:
	static bool IsOsVersionVistaOrGreater();
	static void ShellExecuteExOpen(const wstring& appName, const wstring& appPath,
		const wstring& parameters = L"", int showMode = SW_SHOWNORMAL,
		bool isAdmin = true, bool isSync = true, int timeout = INFINITE);

};

}

#endif // SHELL_EXT_H

