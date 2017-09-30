#include "Utils.h"
#include <windows.h>

namespace Utils {

bool isWindowVisible(const std::wstring& class_name, const std::wstring& caption) {
	HWND hWnd = ::FindWindowEx(NULL, 0, class_name.size() ? class_name.c_str() : NULL, caption.size() ? caption.c_str() : NULL);
	return ::IsWindowVisible(hWnd);
}

}