#ifndef NETWORK_MONITOR_H_ 
#define NETWORK_MONITOR_H_

#include "Windows.h"
#include "Iphlpapi.h"
#include "common/event.h"

namespace ucclient {

enum ConnectionState {
	CS_CONNECTION_UNAVAILBALE = -1,		// state has not been checked
	CS_CONNECTION_OK,					// online network, include ETHERNET, WIFI, 2G/3G/4G
	CS_CONNECTION_PPP,					// PPP network (vpn)
	CS_CONNECTION_ERROR,				// offline
};

class NetworkListener {
public:
	virtual void OnNetWorkChanged(ConnectionState state) = 0;
};

class NetworkMonitor : public uc::Event<NetworkListener>{
public:
	NetworkMonitor();
	~NetworkMonitor();

	void WatchForNetworkChange();

	void StopWatching();

	void OnNetWorkChanged(ConnectionState state);
private:
	void CheckConnectionState();

	void TriggerNetworkEvent(ConnectionState state);

	void updateConnectState();

	void OnNetworkChanged();

	static void CALLBACK NetworkMonitor::WaitCallback(void* param, BOOLEAN timed_out);

private:
	HANDLE _handle;
	OVERLAPPED _overlap;
	ConnectionState _curState;
};

}

#endif