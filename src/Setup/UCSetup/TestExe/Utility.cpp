/******************************************************************************
* Copyright (c) 2009~2010 All Rights Reserved by
*  G-Net Integrated Service co. Ltd.

******************************************************************************/
/**
* @file Utility.cpp
* @brief Defined commom functions for conference management module.
*
* @author Hongwei Hu (hongwei.hu@gnetis.com)
*
* @date Janu. 25 2010
*
* @version 1.0.0
*
* @warning
*
* Revision History
* @if  CR/PR ID    Author          Date            Major Change @endif
* @bug

******************************************************************************/

/*----------------------------HEADER FILE INCLUDES---------------------------*/
#include "Utility.h"
#include <strsafe.h>
#include <string>
/**
* @brief generate the full path of a specific file.
* @param [in] relativePath. the relative path of the file.
* @param [in out] strUrl. full path of the input file.
* @return HRESULT return S_OK if successful, otherwise return error code.
*/
HRESULT Utility::GetFileFullPath( TCHAR *relativePath, TCHAR **strUrl )
{
	HRESULT                     hr = S_OK;
	std::wstring				wstrPathValue;
	WCHAR						moduleFullName[MAX_PATH] = {0};

	//CHECK_IF_NULL(relativePath);
	//CHECK_IF_NULL(strUrl);

	DWORD dwErr = ::GetModuleFileName(NULL, moduleFullName, MAX_PATH);
	wstrPathValue = moduleFullName;
	size_t loopIndex = wstrPathValue.find_last_of(L'\\');
	if (loopIndex > 0)
	{
		wstrPathValue = wstrPathValue.substr(0,loopIndex);
		//wstrPathValue.append(L"\\html\\");
		//TODO: try html folder, if not existing,throw error.

		wstrPathValue.append(relativePath);
		size_t maxCount = wstrPathValue.length()* sizeof(TCHAR);
		*strUrl = new TCHAR[maxCount];
		::StringCchCopy(*strUrl, wstrPathValue.length() + 1,  wstrPathValue.c_str());
	}

exit:
	if (FAILED(hr))
	{
		//log the error.
		//LOG_ERR(L"Get html file url with error: 0x%08X", hr);
	}

	return hr;
}

