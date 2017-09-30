#include "FileUtil.h"
#include <Windows.h>
#include "ClientDefs.h"
#include "LogManager.h"

namespace cefIntegration {

using namespace uc;

std::wstring FileUtil::ConvertFileSeperator(const std::wstring& path) {
	std::wstringstream param;
	for (std::size_t i = 0; i < path.length(); ++i) {
		if (path[i] == L'/') {
			param << L'\\';
		}
		else {
			param << path[i];
		}
	}

	return param.str();
}

void FileUtil::ProcessPath(std::wstring& processPath) {
	WCHAR buffer[MAX_PATH] = { 0 };
    GetModuleFileName(NULL, buffer, MAX_PATH );

	processPath.append(buffer);
	std::wstring::size_type pos = processPath.find_last_of(L"\\");
	processPath = processPath.substr(0, pos);
}

bool FileUtil::IsFolder(std::wstring& path) {
	DWORD dwAttrib = GetFileAttributes(path.c_str());
	return (dwAttrib != INVALID_FILE_ATTRIBUTES &&
		(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}

std::string FileUtil::ProcessPath() {
	char buffer[MAX_PATH] = { 0 };
	GetModuleFileNameA(NULL, buffer, MAX_PATH);

	std::string processPath = buffer;
	std::string::size_type pos = processPath.find_last_of("\\");
	return processPath.substr(0, pos);
}

int FileUtil::OpenFolder(const std::wstring& path) {
	DWORD dwAttrib = GetFileAttributes(path.c_str());
	if (dwAttrib != INVALID_FILE_ATTRIBUTES) {
		if (dwAttrib & FILE_ATTRIBUTE_DIRECTORY) {
			return OpenFile(path);
		}
		else {
			return OpenFolderAndSelectFile(path);
		}
	}
	else {
		std::wstring processPath = ConvertFileSeperator(path);
		std::wstring::size_type pos = processPath.find_last_of(L"\\");
		processPath = processPath.substr(0, pos);

		return OpenFile(processPath);
	}
}

int FileUtil::OpenFolderAndSelectFile(const std::wstring& path) {
	SHELLEXECUTEINFOW sei = { sizeof(SHELLEXECUTEINFOW) };
	sei.fMask = SEE_MASK_NOCLOSEPROCESS | SEE_MASK_FLAG_NO_UI;
	sei.lpFile = L"explorer";
	std::wstring param = L"/select, ";
	param += ConvertFileSeperator(path);

	sei.lpParameters = param.c_str();
	sei.lpVerb = L"open";
	sei.nShow = SW_SHOWNORMAL;

	BOOL res = ShellExecuteExW(&sei);
	if (res == TRUE) {
		return NO_ERROR;
	}
	else {
		res = GetLastError();
		UC_LOG(ERROR) << "open folder error: " << res;

		if (res == ERROR_FILE_NOT_FOUND) {
			return CEF_FILE_NOT_FOUND;
		}
		else {
			return CEF_OPEN_FILE_ERROR;
		}
	}
}

bool FileUtil::CreateFolder(std::wstring& path) {
	DWORD res = 0;
	if (CreateDirectory(path.c_str(), NULL) || ERROR_ALREADY_EXISTS == (res = GetLastError())){
		return true;
	}
	else {
		return false;
	}
}

long long FileUtil::GetFileSize(const std::wstring& path) {
	WIN32_FIND_DATAW data;
	HANDLE h = ::FindFirstFileW(path.c_str(),&data);
	if (h == INVALID_HANDLE_VALUE) {
		return CEF_OPEN_FILE_ERROR;
	}
	FindClose(h);

	return data.nFileSizeLow |(long long) data.nFileSizeHigh << 32;
}

bool FileUtil::FileExist(std::wstring& path) {
	WIN32_FIND_DATA ffd;
	HANDLE hFind = FindFirstFile(path.c_str(), &ffd);
	if (INVALID_HANDLE_VALUE == hFind) {
		return false;
	}
	else {
		FindClose(hFind);
		return true;
	}
}

int FileUtil::DeleteFileEx(std::wstring& path) {
	BOOL res = ::DeleteFile(path.c_str());
	if (res > 0) {
		return NO_ERROR;
	}
	else {
		res = GetLastError();
		if (res == ERROR_FILE_NOT_FOUND) {
			return CEF_FILE_NOT_FOUND;
		}
		else {
			return res;
		}
	}
}

int FileUtil::OpenFile(const std::wstring& path) {

    std::wstring tmpPath = ConvertFileSeperator(path);

	SHELLEXECUTEINFOW sei = { sizeof(SHELLEXECUTEINFOW) };  
    sei.fMask = SEE_MASK_NOCLOSEPROCESS | SEE_MASK_FLAG_NO_UI;  
    sei.lpFile = tmpPath.c_str();
	sei.lpVerb = L"open";
	sei.nShow = SW_SHOWNORMAL;

    BOOL res = ShellExecuteExW(&sei);
	if (res == TRUE) {
		return NO_ERROR;
	}
	else {
		res = GetLastError();
        UC_LOG(ERROR) << "open file:" << tmpPath << ", error: " << res;

		if (res == ERROR_FILE_NOT_FOUND) {
			return CEF_FILE_NOT_FOUND;
		}
		else if (res == ERROR_NO_ASSOCIATION) {
			std::wstringstream param;
			param << L"shell32.dll,OpenAs_RunDLL ";
            param << tmpPath;// ConvertFileSeperator(path);

			UC_LOG(INFO) << "rundll32.exe parameters: " << param.str();

			HINSTANCE hInst = ShellExecute(NULL, NULL, L"rundll32.exe", param.str().c_str(), NULL, SW_SHOWNORMAL);
			if ((long)hInst < 32)
			{
				UC_LOG(ERROR) << "rundll32.exe open file error: " << hInst;
				return (int)hInst;
			}
			return NO_ERROR;
		}
		else {
			return CEF_OPEN_FILE_ERROR;
		}

	}
}

int FileUtil::SaveFile (const std::wstring& path, const std::string& content) {
	DWORD nBytesWritten;
	HANDLE hFile = CreateFile(path.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
                      FILE_ATTRIBUTE_NORMAL, NULL);
	if(hFile == INVALID_HANDLE_VALUE) {
		return CEF_OPEN_FILE_ERROR;
	}

	BOOL result = WriteFile(hFile, content.c_str(), content.size(), &nBytesWritten, NULL);
	if (TRUE != result) {
		DWORD err = ::GetLastError();;
		CloseHandle(hFile);
		return err;
	}

	CloseHandle(hFile);
	return CEF_SUCCESS;
}

int FileUtil::CopyFileEX(const std::wstring& src, const std::wstring& dest) {

	std::wstring::size_type pos = dest.find_last_of(L"\\");
	std::wstring directory = dest.substr(0, pos);
	CreateFolder(directory);

	if(!::CopyFile(src.c_str(), dest.c_str(), FALSE)) {
		return GetLastError();
	}
	
	return CEF_SUCCESS;
}

void FileUtil::GetDirectoryContent(const std::wstring& dir, std::vector<std::wstring> &contents) {
	WIN32_FIND_DATA ffd;
	wchar_t buffer[MAX_PATH]; 
	wsprintf(buffer, L"%s\\*.*", dir.c_str()); 
	
	HANDLE hFind = FindFirstFile(buffer, &ffd);
	if (INVALID_HANDLE_VALUE == hFind) {
		return;
	}

	do {
      if (!(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
		  contents.push_back(ffd.cFileName);
      }
   }
   while (FindNextFile(hFind, &ffd) != 0);
   DWORD  dwError = GetLastError();
   
   FindClose(hFind);
}

}