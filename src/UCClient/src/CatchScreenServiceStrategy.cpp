#include "CatchScreenServiceStrategy.h"
#include "common/StringConvert.h"
#include "LogManager.h"
#include "CatchScreenDll.h"
#include "Utils/FileUtil.h"

namespace ucclient {

using namespace uc;

CatchScreenServiceStrategy::CatchScreenServiceStrategy(cefIntegration::CefClient *cefClient) 
	: _cefClient(cefClient) {

}

int32_t CatchScreenServiceStrategy::StartCatchScreen(const wstring& path, CefRefPtr<CefBrowser> browser, int requestId) {
	_handle = browser->GetHost()->GetWindowHandle();
	_requestId = requestId;

	std::wstring csPath = path + L"\\Screenshot\\";
	if (_csPath.empty()) {
		// cleanup history screen image
		FileUtil::RemoveFolder(csPath);
		_csPath = csPath;
	}
	FileUtil::CreateFolder(csPath);

	CaptureScreen_SetDeflateParam(15, 5, 15, 15);
	int ret = CaptureScreen(this, csPath, ScreenCaptureCallBack);
	
	if (ret != 0) {
		UC_LOG(ERROR) << "CatchScreen, path:" << path;
	}
	return ret;
}

void _stdcall CatchScreenServiceStrategy::ScreenCaptureCallBack(void* userdata, int type, const wstring& imagePath) {
	((CatchScreenServiceStrategy*)(userdata))->OnScreenCaptured(type, StringConvert::ToUTF8Str(imagePath));
}

void CatchScreenServiceStrategy::OnScreenCaptured(int type, const string& imagePath) {
	CefRefPtr<CefProcessMessage> response = CefProcessMessage::Create("InvokeCallback");
	CefRefPtr<CefListValue> responseArgs = response->GetArgumentList();
	responseArgs->SetInt(0, _requestId);
	responseArgs->SetInt(1, 0);
	responseArgs->SetInt(2, type);
	responseArgs->SetString(3, imagePath);

	_cefClient->GetBrowser(_handle)->SendProcessMessage(PID_RENDERER, response);
}

}