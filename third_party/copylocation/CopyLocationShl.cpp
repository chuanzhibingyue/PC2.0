// CopyLocationShl.cpp : Implementation of CCopyLocationShl
#include "stdafx.h"
#include <atlconv.h>  // for ATL string conversion macros
#include <shlobj.h>

#include "CopyLocation.h"
#include "CopyLocationShl.h"

/////////////////////////////////////////////////////////////////////////////
// CCopyLocationShl

CCopyLocationShl::CCopyLocationShl()
{
	m_hCopyBmp = LoadBitmap(_Module.GetModuleInstance(), MAKEINTRESOURCE(IDB_BITMAP1));
}


HRESULT CCopyLocationShl::Initialize(
    LPCITEMIDLIST pidlFolder,
    LPDATAOBJECT  pDataObj,
    HKEY          hProgID
	)
{
	//_ASSERT(0);
	UNREFERENCED_PARAMETER(pidlFolder);
	UNREFERENCED_PARAMETER(hProgID);

	FORMATETC fmt = { CF_HDROP, NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL };
	STGMEDIUM stg = { TYMED_HGLOBAL };
	HDROP     hDrop;

    // Look for CF_HDROP data in the data object
    if (FAILED(pDataObj->GetData(&fmt, &stg)))
	{
        // Nope! Return an "invalid argument" error back to Explorer
        return E_INVALIDARG;
	}

    // Get a pointer to the actual data
    hDrop = (HDROP)GlobalLock(stg.hGlobal);
    if (NULL == hDrop)
	{
        ReleaseStgMedium(&stg);
        return E_INVALIDARG;
	}

    // Sanity check	- make sure there is at least one filename
	UINT uNumFiles = DragQueryFile(hDrop, 0xFFFFFFFF, NULL, 0);
    if (0 == uNumFiles)
	{
        GlobalUnlock(stg.hGlobal);
        ReleaseStgMedium(&stg);
        return E_INVALIDARG;
	}

	TCHAR szFile[MAX_PATH];
    for (UINT uFile = 0 ; uFile < uNumFiles ; uFile++)
	{
        // Get the next filename.
        if (0 == DragQueryFile(hDrop, uFile, szFile, MAX_PATH))
            continue;

        // Add the file name to our list of files (m_lsFiles).
        m_lsFiles.push_back(szFile);
	}

    GlobalUnlock(stg.hGlobal);
    ReleaseStgMedium(&stg);

    // If we found any files we can work with, return S_OK. Otherwise, return
    // E_INVALIDARG so we don't get called again for this right-click operation.
    return (m_lsFiles.size() > 0) ? S_OK : E_INVALIDARG;
}


HRESULT CCopyLocationShl::QueryContextMenu(
    HMENU hmenu,
    UINT  uMenuIndex, 
    UINT  uidFirstCmd,
    UINT  uidLastCmd,
    UINT  uFlags
	)
{
	UNREFERENCED_PARAMETER(uidLastCmd);

	// If the flags include CMF_DEFAULTONLY then we shouldn't do anything
	if ((uFlags & CMF_DEFAULTONLY) || m_lsFiles.size() > 1)
	{
        return MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_NULL, 0);
	}

    // Insert the Copy file path menu item
    InsertMenu(hmenu, uMenuIndex, MF_STRING | MF_BYPOSITION, uidFirstCmd, _T("通过蜜蜂发送到"));
    if (NULL != m_hCopyBmp)
    {
		SetMenuItemBitmaps(hmenu, uMenuIndex, MF_BYPOSITION, m_hCopyBmp, NULL);
    }

    // Insert the Copy file name menu item
// 	uMenuIndex++;
// 	uidFirstCmd++;
//     InsertMenu(hmenu, uMenuIndex, MF_STRING | MF_BYPOSITION, uidFirstCmd, _T("Copy File Name"));
//     if (NULL != m_hCopyBmp)
//     {
// 		SetMenuItemBitmaps(hmenu, uMenuIndex, MF_BYPOSITION, m_hCopyBmp, NULL);
//     }

    return MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_NULL, 2);
}


HRESULT CCopyLocationShl::GetCommandString(
    UINT_PTR  idCmd,
    UINT  uFlags,
    UINT* pwReserved,
    LPSTR pszName,
    UINT  cchMax
	)
{
	UNREFERENCED_PARAMETER(pwReserved);

	LPCTSTR szPrompt;
    USES_CONVERSION;

    // If Explorer is asking for a help string, copy our string into the
    // supplied buffer
    if (uFlags & GCS_HELPTEXT)
	{
		switch (idCmd)
		{
		case 0:  szPrompt = _T("Copy the full file path(s) to the clipboard"); break;
		case 1:  szPrompt = _T("Copy the file name(s) to the clipboard");      break;
		default: return E_INVALIDARG;
		}

        if (uFlags & GCS_UNICODE)
		{
            // We need to cast pszName to a Unicode string, and then use the
            // Unicode string copy API
            lstrcpynW((LPWSTR)pszName, T2CW(szPrompt), cchMax);
		}
        else
		{
            // Use the ANSI string copy API to return the help string
            lstrcpynA(pszName, T2CA(szPrompt), cchMax);
		}

        return S_OK;
	}

    return E_INVALIDARG;
}


HRESULT CCopyLocationShl::InvokeCommand(LPCMINVOKECOMMANDINFO pCmdInfo)
{
    // If lpVerb really points to a string, ignore this function call and bail out
    if (0 != HIWORD(pCmdInfo->lpVerb))
        return E_INVALIDARG;

    // Get the command index - the only valid commands are 0 and 1
	WORD wCmd = LOWORD(pCmdInfo->lpVerb);
	if (wCmd > 1)
		return E_INVALIDARG;

	string_list::const_iterator it, itEnd;
	tchar_string clipBuffer;
	tchar_string name;

	it = m_lsFiles.begin();
	itEnd = m_lsFiles.end();

	switch (wCmd)
	{
	case 0:
		clipBuffer = it->c_str();
		break;

// 	case 1:
// 		CopyFileName(name, *it);
//     	clipBuffer = name;
// 		break;
	default:
		break;
	}

    while (++it != itEnd)
	{
		clipBuffer += _T(";");
		switch (wCmd)
		{
		case 0:
			clipBuffer += it->c_str();
			break;

// 		case 1:
// 			CopyFileName(name, *it);
//     		clipBuffer += name;
// 			break;
		default:
			break;
		}
	}

	//send to bee
	std::wstring beePath = GetBeeInstallPath();
	std::wstring filePath = L"SendFileToBee:" + std::wstring(clipBuffer.begin(), clipBuffer.end());
	SHELLEXECUTEINFO sei = { sizeof(SHELLEXECUTEINFO) };
	sei.fMask = SEE_MASK_NOCLOSEPROCESS;
	//sei.lpVerb = TEXT("runas");
	sei.lpFile = L"Bee.exe";
	sei.lpDirectory = beePath.c_str();
	sei.lpParameters = filePath.c_str();
	sei.nShow = SW_SHOWNORMAL;
	BOOL ret = ShellExecuteEx(&sei);
	_ASSERT(ret);

	return S_OK;
}


void CCopyLocationShl::CopyFileName(tchar_string &name, const tchar_string &fullpath)
{
	size_t pos = fullpath.find_last_of(_T('\\'));
	name.assign(fullpath, pos + 1, fullpath.size() - pos - 1);
}

std::wstring CCopyLocationShl::GetBeeInstallPath()
{
	std::wstring installPath;
	HKEY hAppKey = NULL;
	WCHAR buffer[MAX_PATH] = { 0 };
	DWORD bufferLen = MAX_PATH;
	if (RegOpenKeyEx(HKEY_CURRENT_USER,
		L"Software\\Bee",
		0,
		KEY_READ | KEY_WOW64_64KEY,
		&hAppKey) == ERROR_SUCCESS)
	{
		::RegQueryValueEx(hAppKey, L"installDir", NULL, NULL, reinterpret_cast<LPBYTE>(&buffer), &bufferLen);
		RegCloseKey(hAppKey);
	}
	installPath = buffer;
	return installPath;
}
