#ifndef OSR_UTIL_H_
#define OSR_UTIL_H_

#include <windows.h>

namespace cefIntegration {

class OsrUtil{
public:
	static int GetCefMouseModifiers(WPARAM wparam);

	static bool IsKeyDown(WPARAM wparam);

	static int GetCefKeyboardModifiers(WPARAM wparam, LPARAM lparam);

	static float GetDeviceScaleFactor();
};

}

#endif