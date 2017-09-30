#include <atlbase.h>
CComModule _Module;

typedef int(*MyFunc)(LPTSTR nVal1, int nVal2);
typedef void(*PostProgress)(int status, int percent);
typedef void(*ShowMainWnd)(char szProductCode[MAX_PATH], char szPackageCode[MAX_PATH], char szVersion[MAX_PATH]);

int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	_Module.Init(NULL, hInstance, &LIBID_ATLLib);

	HMODULE hm = LoadLibrary("MsiDll.dll");
	ShowMainWnd showMain;
	showMain = (ShowMainWnd)GetProcAddress(hm, "ShowMainWnd" /* 这里的名字必须与dll导出函数名相同 */);
	if (showMain)
	{
		char szProductCode[MAX_PATH] = { "{productcode-aabb-ccdd}" };
		char szPackageCode[MAX_PATH] = { "{packagecode-aabb-ccdd}" };
		char szVersion[MAX_PATH] = { "1.00.000" };
		showMain(szProductCode, szPackageCode, szVersion);
	}
	
	Sleep(100000);
	_Module.Term();
	return 0;
}