#ifndef CATCHSCREEN_SERVICE_STRATEGY_H_
#define CATCHSCREEN_SERVICE_STRATEGY_H_

#include "UCSDK.h"
#include "CefClient.h"
#include "IUCEngine.h"
#include <string>

namespace ucclient {

using namespace std;
using namespace uc;

class CatchScreenServiceStrategy  {
public:
	CatchScreenServiceStrategy(cefIntegration::CefClient *cefClient);
	int32_t StartCatchScreen(const wstring& path, CefRefPtr<CefBrowser> browser, int requestId);
	void OnScreenCaptured(int type, const string& imagePath);

	static void _stdcall ScreenCaptureCallBack(void* userdata, int type, const wstring& szfileName );

private:
	cefIntegration::CefClient *_cefClient;
	CefWindowHandle _handle;
	int _requestId;
	std::wstring _csPath;
};

}

#endif //CATCHSCREEN_SERVICE_STRATEGY_H_