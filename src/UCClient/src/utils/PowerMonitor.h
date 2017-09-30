#ifndef POWER_MONITOR_H_ 
#define POWER_MONITOR_H_

#include <Windows.h>

namespace ucclient {

class MonitorMessageWindow {
public:
	MonitorMessageWindow();

	~MonitorMessageWindow();

private:
	static void ProcessPowerBroadcastMessage(WPARAM wparam);

	static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);

private:
	HWND _hwnd;
};

class PowerMonitor {
public:
	PowerMonitor(){}

private:
	MonitorMessageWindow _win;
};

}

#endif