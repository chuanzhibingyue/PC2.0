#include "Clipboard.h"
#include "common/StringConvert.h"
#include "cximage/ximage.h"
#include "LogManager.h"
#include <sstream>
#include <atlimage.h>
#include <ShlObj.h>

namespace ucclient {

using namespace uc;

UINT nFormats[6] = {
	0, // HTML placeholder
	CF_UNICODETEXT,
	CF_TEXT,
	CF_DIB,
	CF_HDROP,
	CF_BITMAP
};

Clipboard::Clipboard() {
	nFormats[0] = RegisterClipboardFormatW(L"HTML Format");
}

Clipboard::~Clipboard() {
}

std::wstring Clipboard::getTempFilePath() const {
	static std::wstring tempPath;
	if (tempPath.empty()) {
		WCHAR szTempPath[MAX_PATH] = { 0 };
		GetTempPath(MAX_PATH, szTempPath);
		tempPath = std::wstring(szTempPath) + L"Quanshi\\image";
	}

	return tempPath;
}

std::string Clipboard::getClipboardDataFormat(HWND wnd) {
	if (!OpenClipboard(wnd)) 
		return "";
	
	UINT uFormat = ::GetPriorityClipboardFormat (nFormats, sizeof(nFormats));
	if (uFormat > 0) {
		switch (uFormat) {  
		case CF_UNICODETEXT:
		case CF_TEXT:  
			return "CF_TEXT";
		case CF_HDROP: 
			return "CF_HDROP";
		case CF_BITMAP:
		case CF_DIB: 
			return "CF_BITMAP";
		default:
			if (nFormats[0] == uFormat) {
				return "CF_HTML";
			}
		}  
	}
	CloseClipboard();
	
	return "";
}

std::vector<std::string> Clipboard::getClipboardData(HWND wnd) {
	std::vector<std::string> data;

	if (!OpenClipboard(wnd)) 
		return data;
	
	UINT uFormat = ::GetPriorityClipboardFormat (nFormats, sizeof(nFormats));
	if (uFormat > 0) {
		switch (uFormat) { 
		case CF_UNICODETEXT:
		case CF_TEXT:  
				data = getTextData(uFormat);
			break;
		case CF_HDROP: 
				data = getFileData();
			break;
		case CF_BITMAP:
		case CF_DIB: 
				data = getBitmapData();
			break;
		default:
			if (nFormats[0] == uFormat) {
				data = getTextData(uFormat);
			}
			break;
		}  
	}
	CloseClipboard();

	return data;
}

std::vector<std::string> Clipboard::getTextData(unsigned int format) {
	std::vector<std::string> data;

	HGLOBAL hGlobal = NULL;
	hGlobal = GetClipboardData(format);
	if (!hGlobal)
		return data;
	
	if ( CF_UNICODETEXT == format ) {
		WCHAR * szGlobal = (WCHAR *)::GlobalLock(hGlobal);
		if ( NULL != szGlobal && 0 <= wcslen(szGlobal) ) {
			data.push_back(StringConvert::ToUTF8Str(std::wstring(szGlobal)));
		}
		GlobalUnlock(hGlobal);
	}
	else {
		char * szGlobal = (char *)::GlobalLock(hGlobal);
		int size = ::GlobalSize(hGlobal);
		std::string str(szGlobal, size);

		if (!str.empty()) {
			data.push_back(str);
		}
		GlobalUnlock(hGlobal);
	}
	
	return data;
}

std::vector<std::string> Clipboard::getBitmapData() {
	std::vector<std::string> data;

	SYSTEMTIME sysTime;
	::GetLocalTime(&sysTime);

	char fileName[256] = { 0 };
	sprintf(fileName, "UCCBPic_%.4d%.2d%.2d_%.2d%.2d%.2d%.3d.png", sysTime.wYear,sysTime.wMonth,sysTime.wDay, 
		   sysTime.wHour, sysTime.wMinute, sysTime.wSecond, sysTime.wMilliseconds);
	
	SHCreateDirectoryEx(NULL, getTempFilePath().c_str(), NULL);
	std::wstring picturePathName = getTempFilePath() + L"\\" + StringConvert::FromUTF8Str(fileName);

	HBITMAP hBitmap = (HBITMAP)GetClipboardData(CF_BITMAP);
	if (hBitmap) {
		CImage image;
		image.Attach(hBitmap);
		image.Save(picturePathName.c_str());
		data.push_back(StringConvert::ToUTF8Str(picturePathName));
	}
	
	return data;
}

std::vector<std::string> Clipboard::getFileData() {
	std::vector<std::string> data;

	HDROP hDrop = (HDROP)::GetClipboardData(CF_HDROP);
	if (hDrop == NULL) 
		return data;
	
	// Find out how many file names the HDROP contains.
	int nCount = ::DragQueryFile (hDrop, (UINT) -1, NULL, 0);
	TCHAR szFile[MAX_PATH] = { 0 };
	for (int i = 0; i < nCount; i++) {
		::DragQueryFile(hDrop, i, szFile, sizeof(szFile)/sizeof(TCHAR));

		if (GetFileAttributes(szFile) & FILE_ATTRIBUTE_DIRECTORY)
			continue;

		data.push_back(StringConvert::ToUTF8Str(std::wstring(szFile)));
	}

	return data;
}

bool Clipboard::setClipboardData(const std::string& filePath, HWND wnd){
	if (!OpenClipboard(wnd)) {
		UC_LOG(ERROR) << "OpenClipboard error, res: " << ::GetLastError();
		return false;
	}

	CxImage image;
	bool success = false;
	if (image.Load(StringConvert::FromUTF8Str(filePath).c_str())) {
		EmptyClipboard();
		SetClipboardData(CF_DIB, image.CopyToHandle());
		success = true;
	}
	else {
		UC_LOG(ERROR) << "load file error, file path: " << filePath;
	}

	CloseClipboard();

	return success;
}

}