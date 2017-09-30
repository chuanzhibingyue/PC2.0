#include "UCClientRenderDelegate.h"
#include "resource.h"
#include "Windows.h"
#include "CefClient.h"

namespace ucSubProcess {

UCClientRenderDelegate::UCClientRenderDelegate(cefIntegration::CefClient * cefClient) : _cefClient(cefClient){
}

/*
void UCClientRenderDelegate::GetExtensionCode(std::string& code) {
	HRSRC hRes = FindResource(NULL, MAKEINTRESOURCE(IDS_UCCLIENT_EXTENSIONS), MAKEINTRESOURCE(UCCLIENT_BINARY));
	DWORD dwSize;
    LPBYTE pBytes = NULL;

    if(hRes)
    {
        HGLOBAL hGlob = LoadResource(NULL, hRes);
        if(hGlob)
        {
            dwSize = SizeofResource(NULL, hRes);
            pBytes = (LPBYTE)LockResource(hGlob);
        }
    } 
	else {
		int err = GetLastError();
	}

	if (pBytes) {
		code.append((const char *)pBytes, dwSize);
		//_cefClient->RegisterExtension(L"v8/uc_app", code);
	}
}
*/

void UCClientRenderDelegate::OnWebKitInitialized() {
	std::string code;
	HRSRC hRes = FindResource(NULL, MAKEINTRESOURCE(IDS_UCCLIENT_EXTENSIONS), MAKEINTRESOURCE(UCCLIENT_BINARY));
	DWORD dwSize;
    LPBYTE pBytes = NULL;

    if(hRes)
    {
        HGLOBAL hGlob = LoadResource(NULL, hRes);
        if(hGlob)
        {
            dwSize = SizeofResource(NULL, hRes);
            pBytes = (LPBYTE)LockResource(hGlob);
        }
    }
	else {
		int err = GetLastError();
	}

	if (pBytes) {
		code.append((const char *)pBytes, dwSize);
		_cefClient->RegisterExtension(L"v8/uc_app", code);
	}
}

}