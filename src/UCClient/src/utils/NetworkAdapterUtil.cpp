#include <winsock2.h>
#include "NetworkAdapterUtil.h"
#include "LogManager.h"
#include "../UCDefs.h"
#include "common/platform.h"

#pragma comment(lib, "IPHLPAPI.lib")

namespace ucclient {

using namespace uc;

bool NetworkAdapterUtil::GetPipAddresses(PIP_ADAPTER_ADDRESSES *pipAddresses) {
	if (pipAddresses == NULL) {
		UC_LOG(ERROR) << "error pipAddresses.";
		return false;
	}

	static const int maxTries = 3;
	static const DWORD flags = GAA_FLAG_SKIP_FRIENDLY_NAME | GAA_FLAG_SKIP_ANYCAST |
			GAA_FLAG_SKIP_MULTICAST | GAA_FLAG_SKIP_DNS_SERVER;
	//static const DWORD flags = GAA_FLAG_INCLUDE_PREFIX;

	ULONG len = 16384;
	int iterations = 0;
	DWORD ret = ERROR_SUCCESS;
	
	do {
		*pipAddresses = (IP_ADAPTER_ADDRESSES *) new char[len];
		if (*pipAddresses == NULL) {
			UC_LOG(ERROR) << "Memory allocation failed for IP_ADAPTER_ADDRESSES struct.";
			return false;
		}

		ret = GetAdaptersAddresses(AF_UNSPEC, flags, NULL, *pipAddresses, &len);
		if (ret != ERROR_SUCCESS) {
			delete *pipAddresses;
			*pipAddresses = NULL;
		}

		iterations++;
	} while ((ret == ERROR_BUFFER_OVERFLOW) && (iterations < maxTries));

	if (ret == ERROR_SUCCESS) {
		return true;
	}
	else {
		*pipAddresses = NULL;
		return false;
	}
}

bool NetworkAdapterUtil::GetEthernetMAC(std::wstring &mac) {
	PIP_ADAPTER_ADDRESSES pipAddresses = NULL;
	bool res = GetPipAddresses(&pipAddresses);
	
	if (res) {
		PIP_ADAPTER_ADDRESSES currAddresses = pipAddresses;
		while (currAddresses) {
			if(currAddresses->IfType == IF_TYPE_ETHERNET_CSMACD && 
				currAddresses->PhysicalAddressLength >= 6) {
				
				wchar_t szBuffer[512];
				memset(szBuffer, 0, 512* sizeof(wchar_t));
				swprintf_s(szBuffer, 512, L"%.2X%.2X%.2X%.2X%.2X%.2X",
					currAddresses->PhysicalAddress[0] , currAddresses->PhysicalAddress[1], 
					currAddresses->PhysicalAddress[2] , currAddresses->PhysicalAddress[3], 
					currAddresses->PhysicalAddress[4], currAddresses->PhysicalAddress[5]);

				mac = szBuffer;
				res = true;

				break;
			}
					
			currAddresses = currAddresses->Next;
		}
	}

	if (!res) {
		UC_LOG(ERROR) << "GetEthernetMAC fail";
	}
	
	if (pipAddresses != NULL) {
		delete pipAddresses;
	}
	
	return res;
}

ProxyInfo NetworkAdapterUtil::GetProxyInfo() {
	ProxyInfo info = {false, L""};

	HKEY key;
	int32_t res = UCCLIENT_ERROR;
	if((res = RegOpenKeyEx(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings", 0, KEY_QUERY_VALUE,  
		&key)) == ERROR_SUCCESS) {	
		DWORD type = REG_DWORD;
		DWORD isEnable;
		DWORD len = sizeof(isEnable);
		res = RegQueryValueEx(key, L"ProxyEnable", NULL, &type, (LPBYTE)&isEnable, &len);
		if (res == ERROR_SUCCESS) {
			info.isEnable = (isEnable == 1);
		}

		len = 256;
		WCHAR buffer[256];
		type = REG_SZ;
		
		res = RegQueryValueEx(key, L"ProxyServer", NULL, &type, (LPBYTE)buffer, &len);
		if (res == ERROR_SUCCESS) {
			info.proxyServer = buffer;
		}
		
		RegCloseKey(key);
	}

	return info;
}

}