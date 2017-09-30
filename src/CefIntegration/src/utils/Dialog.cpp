#include "Dialog.h"
#include "LogManager.h"
#include "common/StringConvert.h"

#include <sstream>

namespace cefIntegration {

using namespace uc;

void Dialog::RunOpenDialog(CefRefPtr<CefBrowser> browser, const CefString& title, const std::map<std::wstring, std::wstring> &filters, bool isMulti, int requestId) {
	std::vector<std::wstring> path;
	RunDialog(browser->GetHost()->GetWindowHandle(), title, filters, L"", isMulti, path, false);

	for (int i = 0; i < path.size(); i++) {
		UC_LOG(INFO) << "RunOpenDialog, path: " << path[i];
	}
	
	SendDialogMessage(browser, requestId, path);
}

void Dialog::RunSaveDialog(CefRefPtr<CefBrowser> browser, const CefString& title, const CefString& defaultFileName, const std::map<std::wstring, std::wstring> &filters, int requestId) {
	std::vector<std::wstring> path;
	std::wstring name = defaultFileName.ToWString();
	RunDialog(browser->GetHost()->GetWindowHandle(), title, filters, name, false, path, true);

    std::wstring expName=L"";
    int pos = name.find_last_of('.');
    if (pos != std::string::npos)
    {
        expName = name.substr(pos);
    }

	for (int i = 0; i < path.size(); i++) {
        if (!expName.empty()
            && path[i].find(expName) == std::string::npos)
        {
            path[i].append(expName);
        }
		UC_LOG(INFO) << "RunSaveDialog, path: " << path[i];
	}

	SendDialogMessage(browser, requestId, path);
}

void Dialog::SendDialogMessage(CefRefPtr<CefBrowser> browser, int requestId, std::vector<std::wstring> &path) {
	CefRefPtr<CefProcessMessage> response = CefProcessMessage::Create("InvokeCallback");
	CefRefPtr<CefListValue> responseArgs = response->GetArgumentList();

	responseArgs->SetInt(0, requestId);
	CefRefPtr<CefListValue> pathList = CefListValue::Create();
	int respIndex = 1;
	for (int i = 0; i < static_cast<int>(path.size()); ++i) {
		responseArgs->SetString(respIndex++ , path[i]);
	}

	browser->SendProcessMessage(PID_RENDERER, response);
}

void Dialog::RunDialog(HWND wnd, const CefString& title, const std::map<std::wstring, std::wstring> &filters, const std::wstring& defaultName, 
	bool isMulti, std::vector<std::wstring> &path, bool isSave) {
	OPENFILENAME ofn;

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = wnd;

#define FILE_PATH_SIZE 65535
    wchar_t filename[FILE_PATH_SIZE] = { 0 };
	memcpy(filename, defaultName.c_str(), defaultName.size() * sizeof(wchar_t));

	ofn.lpstrFile = filename;
    ofn.nMaxFile = FILE_PATH_SIZE;

	std::wstring strTitle;
	if (!title.empty()){
		strTitle = title.ToWString();
	}
	ofn.lpstrTitle = strTitle.c_str();

	
	ofn.lpstrInitialDir = NULL;

	if (!filters.empty()) {
        wchar_t filter[FILE_PATH_SIZE] = { 0 };

		int size = 0;
		for (std::map<std::wstring, std::wstring>::const_iterator iter = filters.begin(); iter != filters.end(); ++iter) {
			memcpy(filter + size, iter->first.c_str(), iter->first.size() * sizeof(wchar_t));
			size += iter->first.size();

			filter[size++] = '\0';
		
			memcpy(filter + size, iter->second.c_str(), iter->second.size()  * sizeof(wchar_t));
			size += iter->second.size();

			filter[size++] = '\0';
		}

		filter[size++] = '\0';
		ofn.lpstrFilter = filter;
	}
  
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR | OFN_EXPLORER | OFN_ENABLESIZING;
	if (isMulti){
		ofn.Flags |= OFN_ALLOWMULTISELECT;
	}
	else if (isSave){
		ofn.Flags |= OFN_OVERWRITEPROMPT;
	}

	bool success = false;
	if (isSave) {
		success = GetSaveFileName(&ofn);
	}
	else {
		success = GetOpenFileName(&ofn);
	}
	
	if (success){
	  std::vector<std::wstring> selections;
	  wchar_t* str = ofn.lpstrFile;
      if (isSave) {
          selections.push_back(std::wstring(str));
      }
      else {
          while (*str) {
              selections.push_back(std::wstring(str));
              str += (wcslen(str) + 1);
          }
      }

	  if(selections.size() == 1){
		  path.push_back(selections[0]);
	  }
	  else if(selections.size() > 1){
		  for (int i = 1; i < selections.size(); ++i) {
			  path.push_back(selections[0] + L"\\" +selections[i]);
		  }
	  }
  }
}

}