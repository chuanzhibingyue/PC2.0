#include "UtilAdaptor.h"
#include <ShlObj.h>
#include <TlHelp32.h>

UtilAdaptor::UtilAdaptor(void):NativeActionDispImpl<ITangUtil>()
{

}

UtilAdaptor::~UtilAdaptor(void)
{

}

STDMETHODIMP UtilAdaptor::GetModulePath(BSTR *strModulepath)
{
	HRESULT hr = S_OK;
	std::wstring				wstrPathValue;
	WCHAR						moduleFullName[MAX_PATH] = {0};

	DWORD dwErr = ::GetModuleFileName(NULL, moduleFullName, MAX_PATH);
	wstrPathValue = moduleFullName;
	size_t loopIndex = wstrPathValue.find_last_of(L'\\');
	if (loopIndex > 0)
	{
		wstrPathValue = wstrPathValue.substr(0,loopIndex);
	}
	
	//CHECK_IF_NULL(wstrPathValue.c_str());
	*strModulepath =::SysAllocString(wstrPathValue.c_str());

exit:
	if (FAILED(hr))
	{
		//LOG_ERR(L"UtilAdaptor:Get module path with error: 0x%08X", hr);
	}
	return HResult_To_App_Layer( hr );
}

STDMETHODIMP UtilAdaptor::GetSpecialFolderPath(UINT32 iRequest,BSTR *strPath)
{	
	HRESULT hr = S_OK;
	WCHAR szPath[MAX_PATH + 1] = {0};
	//ARG_CHECK(strPath);

	hr = ::SHGetSpecialFolderPathW(NULL, szPath, iRequest, FALSE);		
	//HR_CHECK(hr);
	//BOOL_CHECK( wcslen(szPath) > 0 );
	*strPath =::SysAllocString(szPath);

exit:
	if (FAILED(hr))
	{
		//LOG_ERR(L"UtilAdaptor:Get special folder path with error: 0x%08X", hr);
	}
	return HResult_To_App_Layer( hr );
}

STDMETHODIMP UtilAdaptor::GetProcessCount(
    /*[in]*/ BSTR bstrProcessName,
    /*[out,retval]*/ int* nProcessCount)
{
    HRESULT hr = S_OK;
    HANDLE hProcessSnap = NULL;
    PROCESSENTRY32 pe = {0};
    unsigned int nFound = 0;
    if (::SysStringLen( bstrProcessName ) <= 0)
    {
        //ARG_CHECK(NULL);
        goto exit;
    }

    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
    if( INVALID_HANDLE_VALUE == hProcessSnap )
    {
        //HR_CHECK(E_FAIL);
        goto exit;
    }

    pe.dwSize = sizeof(PROCESSENTRY32);
    if( Process32First( hProcessSnap, &pe ) ) 
    {
        do 
        {
            if( 0 == _tcsicmp( bstrProcessName, pe.szExeFile ) 
                && pe.th32ProcessID != ::GetCurrentProcessId() ) 
            {
                ++nFound;
            }
        } 
        while( Process32Next( hProcessSnap, &pe) );
    }
    CloseHandle( hProcessSnap );

exit:

    *nProcessCount = nFound;

    if (FAILED(hr))
    {
        //LOG_ERR(L"UtilAdaptor:Get Process Count with error: 0x%08X", hr);
    }
    return HResult_To_App_Layer( hr );
}