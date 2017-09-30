#include <windows.h>

#include "CefClient.h"
#include "UCClientRenderDelegate.h"
#include "LogManager.h"
#include "Shlobj.h"
#include "gkclient/GKClientRenderDelegate.h"

using namespace uc;

void initLog() {
	char path[MAX_PATH];
	SHGetFolderPathA(NULL, CSIDL_LOCAL_APPDATA, NULL, 0, path);

	std::string logPath = path;
	logPath += "\\Quanshi\\Bee\\log";
	LogManager::GetInstance().Init(logPath, "È«Ê±ÃÛ·ä", "worker.log", false, 2, 0, 10, 0, 20, 2);
}

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPTSTR lpCmdLine, int nCmdShow) {
	initLog();

	UC_LOG(INFO) << "sub process OK!";

	cefIntegration::CefClient cefClient;
	
	cefClient.RegisterRenderDelegate(new ucSubProcess::UCClientRenderDelegate(&cefClient));
	cefClient.RegisterRenderDelegate(new gkclient::GKClientRenderDelegate());

	int res = cefClient.InitializeSubProcess(hInstance);

	LogManager::GetInstance().UnInit();
	return res;
}