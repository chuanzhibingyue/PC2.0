#include "PowerMonitor.h"
#include "LogManager.h"

namespace ucclient {

using namespace uc;

const char* MonitorMessageWindowClassName = "uclient_MonitorMessageWindowClass";


MonitorMessageWindow::MonitorMessageWindow() {
	WNDCLASSA wc = { 0, MonitorMessageWindow::WndProc, 0, 0, 0, 0, 0, 0, 0, MonitorMessageWindowClassName };
	ATOM atom = ::RegisterClassA(&wc);

	_hwnd = ::CreateWindowA((const char *)atom, "uclient_MonitorMessageWindow",
		WS_OVERLAPPED, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, NULL, NULL, 0);
}

MonitorMessageWindow::~MonitorMessageWindow() {
	if (::IsWindow(_hwnd)) {
		::DestroyWindow(_hwnd);
		_hwnd = NULL;
	}
	UnregisterClassA(MonitorMessageWindowClassName, NULL);
}


 void MonitorMessageWindow::ProcessPowerBroadcastMessage(WPARAM wparam) {
	 UC_LOG(INFO) << "MonitorMessageWindow::ProcessPowerBroadcastMessage, WM_POWERBROADCAST message, event: " << wparam;
	switch (wparam) {
	case PBT_APMPOWERSTATUSCHANGE:
		UC_LOG(INFO) << "MonitorMessageWindow::ProcessPowerBroadcastMessage, Power status has changed";
		break;
	case PBT_APMRESUMEAUTOMATIC:
		UC_LOG(INFO) << "MonitorMessageWindow::ProcessPowerBroadcastMessage, Operation is resuming from a low-power state";
		break;
	case PBT_APMSUSPEND:
		UC_LOG(INFO) << "MonitorMessageWindow::ProcessPowerBroadcastMessage, System is suspending";
		break;
	default:
		return;
	}
}

 LRESULT CALLBACK MonitorMessageWindow::WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam) {
	switch (message) {
	case WM_POWERBROADCAST:
		ProcessPowerBroadcastMessage(wparam);
		return TRUE;
	default:
		return ::DefWindowProc(hwnd, message, wparam, lparam);
	}
}







}