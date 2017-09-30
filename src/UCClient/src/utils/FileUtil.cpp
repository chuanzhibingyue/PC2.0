#include "FileUtil.h"
#include <Windows.h>
#include "../UCDefs.h"
#include "LogManager.h"
#include "Shlobj.h"
#include "common/StringConvert.h"

namespace ucclient {

void FileUtil::ProcessPath(std::wstring& processPath) {
	WCHAR buffer[MAX_PATH];
	GetModuleFileName(NULL, buffer, MAX_PATH);

	processPath.append(buffer);
	std::wstring::size_type pos = processPath.find_last_of(L"\\");
	processPath = processPath.substr(0, pos);
}

std::string FileUtil::ProcessPath() {
	std::wstring processPath;
	ProcessPath(processPath);

	return uc::StringConvert::ToUTF8Str(processPath);
}

bool FileUtil::CreateFolder(const std::wstring& path) {
	DWORD res = 0;
	if (CreateDirectory(path.c_str(), NULL) || ERROR_ALREADY_EXISTS == (res = GetLastError())){
		return true;
	}
	else {
		return false;
	}
}

bool FileUtil::CreateFolderRecursively(const std::wstring& path) {
	DWORD res = SHCreateDirectoryEx(NULL, path.c_str(), NULL);
	if (res == ERROR_SUCCESS || res == ERROR_FILE_EXISTS  || res == ERROR_ALREADY_EXISTS) {
		return true;
	}

	return false;
}

std::wstring FileUtil::GetDocumentPath() {
	WCHAR path[MAX_PATH];
	SHGetFolderPathW(NULL, CSIDL_PERSONAL, NULL, 0, path);
	return path;
}

std::wstring FileUtil::GetAppDataPath() {
	WCHAR path[MAX_PATH];
	SHGetFolderPathW(NULL, CSIDL_LOCAL_APPDATA, NULL, 0, path);
	return path;
}

std::string FileUtil::GetAppDataPathA() {
	char path[MAX_PATH];
	SHGetFolderPathA(NULL, CSIDL_LOCAL_APPDATA, NULL, 0, path);
	return path;
}

void FileUtil::RemoveFolder(const std::wstring& path) {
	std::wstring currentPath = path + L"//*.*";

    WIN32_FIND_DATAW FindFileData;        
    ZeroMemory(&FindFileData, sizeof(WIN32_FIND_DATAA));  
    HANDLE hFile = FindFirstFileW(currentPath.c_str(), &FindFileData);  
    BOOL IsFinded = TRUE;  
    while(IsFinded)  
    {  
        IsFinded = FindNextFileW(hFile, &FindFileData);
		if (wcslen(FindFileData.cFileName) == 0)
			break;

        if( wcscmp(FindFileData.cFileName, L".") != 0 && wcscmp(FindFileData.cFileName, L"..") != 0 ) 
        {  
            std::wstring strFileName = path + L"\\" + FindFileData.cFileName;   
            if (GetFileAttributesW(strFileName.c_str()) & FILE_ATTRIBUTE_DIRECTORY)   
            {      
                RemoveFolder(strFileName);  
            }  
            else  
            {  
                DeleteFileW(strFileName.c_str());  
            }  
        }  
    }  
    FindClose(hFile);

	RemoveDirectory(path.c_str());
}

}