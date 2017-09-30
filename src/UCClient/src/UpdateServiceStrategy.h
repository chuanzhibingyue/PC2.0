#ifndef UPDATE_SERVICE_STRATEGY_H_
#define UPDATE_SERVICE_STRATEGY_H_

#include "UCSDK.h"
#include "IClientService.h"
#include "CefClient.h"

#include <string>

namespace ucclient {

using namespace std;
using namespace uc;

struct AppConfig {
	int appId;
	int siteId;
	int skinId;
	int timeOut;
	AppConfig() :appId(30), siteId(50000),
		skinId(1), timeOut(30000) {
	}
};

class UpdateServiceStrategy  {
public:
	UpdateServiceStrategy(IUCEngine *engine, cefIntegration::CefClient *cefClient);
	int CheckUpdates(int requestId);
	int Update();
	wstring GetLocalVersion();

	void OnUpdatesChecked(BOOL isNeedUpdate, const wstring& version, const wstring& notes);
	void OnProgressChecked(int value);
	void OnError(int type, int err, const std::wstring& version);

	//callback
	static void _stdcall CheckUpdatesCallBack(void* userdata, int type, BOOL isNeedUpdate, const wstring& version, const wstring& notes);
	static void _stdcall ErrorCallBack(void* userdata, int type, int err, const wstring& version);
	static void _stdcall ProgressCallBack(void* userdata, int type, int value);
private:
	void GetAppConfig();
	int GetHttpPort(const string& server);
	void InitUpdate(string& upgradeServer, string& packageInfo, string& packageDownload, int& httpPort);
	wstring GetDownLoadUrl();
private:
	cefIntegration::CefClient *_cefClient;
	IUCEngine *_engine;
	AppConfig _appConfig;
};

}

#endif //UPDATE_SERVICE_STRATEGY_H_