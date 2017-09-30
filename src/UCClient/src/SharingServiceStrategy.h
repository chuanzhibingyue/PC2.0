#ifndef SHARING_SERVICE_STRATEGY_H
#define SHARING_SERVICE_STRATEGY_H

#include "ISharingService.h"
#include "CefClient.h"
#include "assist/JSEventAssist.h"
#include "SharingWnd.h"

#include <mutex>

namespace ucclient {

using namespace uc;

class SharingServiceStrategy : public ISharingServiceEvent {
public:
	SharingServiceStrategy(IUCEngine *engine, cefIntegration::CefClient *cefClient);

	//开始共享桌面
	virtual int StartDesktopShare();

	//停止桌面共享
	virtual int StopDesktopShare();

	//开始观看桌面
	virtual int StartDesktopView(const std::wstring& sharingName);

	//停止观看桌面
	virtual int StopDesktopView();

	void CloseSharingView();

	//////////////////////////////////////event 事件回调////////////////////////////////////////////
	virtual void OnErrorHandle(int statusCode);
	virtual void OnDesktopShared(int conferenceId, int userId);
	virtual void OnDesktopShareStoped(int conferenceId, int userId);
	virtual void OnDesktopViewerStarted(int conferenceId);
	virtual void OnDesktopViewerStopped(int conferenceId);

private:
	void CreateShareWnd();

private:
	ISharingService *_sharingService;
	IUCEngine *_engine;
	JSEventAssist _jsEventAssist;
	CSharingWnd _sharingFrame;

	std::mutex _mtx;
	bool _need_create_window;
};

}

#endif //SHARING_SERVICE_STRATEGY_H