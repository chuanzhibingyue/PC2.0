#include "SharingServiceStrategy.h"
#include "UCDefs.h"

namespace ucclient {

	SharingServiceStrategy::SharingServiceStrategy(IUCEngine *engine, cefIntegration::CefClient *cefClient)
	: _engine(engine) 
	, _jsEventAssist(cefClient)
	, _need_create_window(true){
		_sharingService = (ISharingService*)_engine->QueryService(UCSharingService);
	}

	void SharingServiceStrategy::CreateShareWnd() {
		UC_LOG(INFO) << "CreateShareWnd";

		_mtx.lock();
		if (_need_create_window) {
			UC_LOG(INFO) << "Create new sharing window!";

			CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath() + _T("resources/sharingSkin"));
			CPaintManagerUI::ReloadSkin();

			_sharingFrame.Create(NULL, _T("SharingDesktop"), UI_WNDSTYLE_DIALOG, 0L, 0, 0, 360, 250);
			_sharingFrame.CenterWindow();
			::ShowWindow(_sharingFrame.GetHWND(), SW_HIDE);

			CDuiRect rcWnd;
			::GetWindowRect(_sharingFrame.GetHWND(), &rcWnd);
			_sharingFrame._childWnd = CreateWindow(TEXT("STATIC"), TEXT(""), WS_CHILD | WS_VISIBLE,
				0, 33, rcWnd.right - rcWnd.left, rcWnd.bottom - rcWnd.top - 33, _sharingFrame.GetHWND(), NULL, NULL, NULL);
			::ShowWindow(_sharingFrame._childWnd, SW_HIDE);

			_sharingFrame.SetChildHwnd(_sharingFrame._childWnd);

			::UpdateWindow(_sharingFrame._childWnd);
			::UpdateWindow(_sharingFrame.GetHWND());

			_need_create_window = false;
		}
		_mtx.unlock();

		UC_LOG(INFO) << "CreateShareWnd finish!";
	}

	int SharingServiceStrategy::StartDesktopShare() {
		UC_LOG(INFO) << "StartDesktopShare";
		return _sharingService->StartDesktopShare();
	}

	int SharingServiceStrategy::StopDesktopShare() {
		UC_LOG(INFO) << "StopDesktopShare";
		return _sharingService->StopDesktopShare();
	}

	int SharingServiceStrategy::StartDesktopView(const std::wstring& sharingName) {
		UC_LOG(INFO) << "StartDesktopView, sharingName:" << sharingName;

		CreateShareWnd();

		::ShowWindow(_sharingFrame.GetHWND(), SW_MAXIMIZE);
		::UpdateWindow(_sharingFrame.GetHWND());

		CDuiRect rcWnd;
		::GetWindowRect(_sharingFrame.GetHWND(), &rcWnd);
		::MoveWindow(_sharingFrame._childWnd, 2, 33, rcWnd.right - rcWnd.left - 8, rcWnd.bottom - rcWnd.top - 8, true);
		::ShowWindow(_sharingFrame._childWnd, SW_SHOW);
		::UpdateWindow(_sharingFrame._childWnd);
		_sharingFrame.m_pSharingName->SetText(sharingName.c_str());
		return _sharingService->StartDesktopView(_sharingFrame._childWnd);
	}

	int SharingServiceStrategy::StopDesktopView() {
		UC_LOG(INFO) << "StopDesktopView";

		CloseSharingView();
		return _sharingService->StopDesktopView();
	}

	void SharingServiceStrategy::CloseSharingView() {
		UC_LOG(INFO) << "CloseSharingView";

		::ShowWindow(_sharingFrame._childWnd, SW_HIDE);
		::ShowWindow(_sharingFrame.GetHWND(), SW_HIDE);

		::UpdateWindow(_sharingFrame._childWnd);
		::UpdateWindow(_sharingFrame.GetHWND());
	}

	void SharingServiceStrategy::OnErrorHandle(int statusCode) {
		std::stringstream eventDesc;
		eventDesc << "errorCode:" << statusCode;
		_jsEventAssist.MakeJSEventAndExecute(SHARING_SERVICE_ON_ERROR_EVENT, eventDesc.str());
	}

	void SharingServiceStrategy::OnDesktopShared(int conferenceId, int userId) {
		std::stringstream eventDesc;
		eventDesc << "conferenceId:" << conferenceId << ", shareUserId:" << userId;
		_jsEventAssist.MakeJSEventAndExecute(SHARING_SERVICE_ON_DESKTOP_SHARED_EVENT, eventDesc.str());
	}

	void SharingServiceStrategy::OnDesktopShareStoped(int conferenceId, int userId) {
		std::stringstream eventDesc;
		eventDesc << "conferenceId:" << conferenceId << ", shareUserId:" << userId;
		_jsEventAssist.MakeJSEventAndExecute(SHARING_SERVICE_ON_DESKTOP_SHARE_STOPED_EVENT, eventDesc.str());
	}

	void SharingServiceStrategy::OnDesktopViewerStarted(int conferenceId) {
		std::stringstream eventDesc;
		eventDesc << "conferenceId:" << conferenceId;
		_jsEventAssist.MakeJSEventAndExecute(SHARING_SERVICE_ON_DESKTOP_VIEWER_STARTED_EVENT, eventDesc.str());
	}

	void SharingServiceStrategy::OnDesktopViewerStopped(int conferenceId) {
		std::stringstream eventDesc;
		eventDesc << "conferenceId:" << conferenceId;
		_jsEventAssist.MakeJSEventAndExecute(SHARING_SERVICE_ON_DESKTOP_VIEWER_STOPPED_EVENT, eventDesc.str());
	}

}