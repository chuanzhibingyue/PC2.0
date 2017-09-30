/*
 * NetworkAdapterUtil.h
 *
 *  Created on: June 30, 2015
 *      Author: gang wang
 */

#ifndef NETWORK_ADAPTER_UTIL_H_
#define NETWORK_ADAPTER_UTIL_H_

#include <string>
#include <Iphlpapi.h>

namespace ucclient {

struct ProxyInfo {
	bool isEnable;
	std::wstring proxyServer;
};

class NetworkAdapterUtil {
public:
	static bool GetPipAddresses(PIP_ADAPTER_ADDRESSES *pipAddresses);

	static bool GetEthernetMAC(std::wstring &mac);

	static ProxyInfo GetProxyInfo();
};

}

#endif

