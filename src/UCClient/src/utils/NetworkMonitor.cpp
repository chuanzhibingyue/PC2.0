#include "Winsock2.h"
#include "NetworkMonitor.h"
#include "LogManager.h"
#include "NetworkAdapterUtil.h"
#include "common/CallOnMainThread.h"

namespace ucclient {

using namespace uc;

NetworkMonitor::NetworkMonitor() : _curState(CS_CONNECTION_UNAVAILBALE) {
	_overlap.hEvent = WSACreateEvent();
}

NetworkMonitor::~NetworkMonitor() {
	WSACloseEvent(_overlap.hEvent);
}

void NetworkMonitor::WatchForNetworkChange() {
	_handle = NULL;
	DWORD res = NotifyAddrChange(&_handle, &_overlap);
	if (res != ERROR_IO_PENDING) {
		UC_LOG(ERROR) << "NetworkMonitor, fail to register event of NotifyAddrChange, res: " << res;
		return;
	}

	static const DWORD flags = WT_EXECUTEINWAITTHREAD | WT_EXECUTEONLYONCE;
	if (!::RegisterWaitForSingleObject(&_handle, _overlap.hEvent, WaitCallback, this,
		INFINITE, flags)) {
		UC_LOG(ERROR) << "NetworkMonitor, RegisterWaitForSingleObject failed";
		return;
	}
}

void NetworkMonitor::StopWatching() {
	if (!UnregisterWaitEx(_handle, INVALID_HANDLE_VALUE)) {
		UC_LOG(ERROR) << "NetworkMonitor, UnregisterWaitEx failed";
	}
}

void CALLBACK NetworkMonitor::WaitCallback(void* param, BOOLEAN timed_out) {
	NetworkMonitor *monitor = (NetworkMonitor *)param;
	if (monitor != NULL) {
		monitor->WatchForNetworkChange();
		monitor->CheckConnectionState();
	}
}

void NetworkMonitor::CheckConnectionState() {
	ConnectionState preState = _curState;
	updateConnectState();

	UC_LOG(INFO) << "NetworkMonitor, CheckConnectionState, preState:" << preState << ", curState:" << _curState;

	if (preState == CS_CONNECTION_UNAVAILBALE || preState != _curState) {
		// connection state is changed, so fire the event.
		CallOnMainThread(this, &NetworkMonitor::TriggerNetworkEvent, _curState);
	}
}

void NetworkMonitor::TriggerNetworkEvent(ConnectionState state) {
	triggerEvent(NetworkListener, OnNetWorkChanged(state));
}

void NetworkMonitor::updateConnectState() {
	PIP_ADAPTER_ADDRESSES pipAddresses = NULL;
	bool res = NetworkAdapterUtil::GetPipAddresses(&pipAddresses);

	ConnectionState state = CS_CONNECTION_ERROR;

	if (res) {
		PIP_ADAPTER_ADDRESSES currAddresses;
		for (currAddresses = pipAddresses; currAddresses; currAddresses = currAddresses->Next) {
			UC_LOG(INFO) << "NetworkMonitor, updateConnectState,"
				<< " operstatus:" << currAddresses->OperStatus
				<< " IfType:" << currAddresses->IfType
				<< " TunnelType:" << currAddresses->TunnelType
				<< " Description:" << currAddresses->Description;

			/*
			* Conditions of online link:
			* (1) The interface is up;
			* (2) This is not a software loopback network interface;
			* (3) Not a tunnel;
			* (4) Not VMware Network Adapter
			*/
			if ((currAddresses->OperStatus == IfOperStatusUp) && (currAddresses->IfType != IF_TYPE_SOFTWARE_LOOPBACK)) {
				if (wcsstr(currAddresses->Description, L"VMnet") != NULL) {
					continue;
				}

				if (currAddresses->IfType == IF_TYPE_TUNNEL) {
					continue;
				}

				if (currAddresses->IfType == IF_TYPE_PPP) {
					state = CS_CONNECTION_PPP;
					break;
				}

				state = CS_CONNECTION_OK;
			}
		}
	}

	_curState = state;

	if (pipAddresses != NULL) {
		delete pipAddresses;
	}
}

}