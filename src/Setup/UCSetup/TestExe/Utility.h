#ifndef GNET_TANG_CLIENT_CONFMGR_UTILITY_H
#define GNET_TANG_CLIENT_CONFMGR_UTILITY_H
#include <tchar.h>
#include <windows.h>
class Utility
{
public:
	Utility();
	~Utility();

	static HRESULT GetFileFullPath(TCHAR *relativePath, TCHAR **strUrl);
};

#endif  //GNET_TANG_CLIENT_CONFMGR_UTILITY_H
