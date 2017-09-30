/*
 * ConferenceLibraryAssist.cpp
 *
 *  Created on: Aug 3, 2015
 *      Author: hualong.zhang
 */

#include <iostream>
#include "ConferenceLibraryAssist.h"
#include "conferencesdk/ConferenceDef.h"

#if defined(WIN32) || defined(WIN64)

namespace uc {
	using namespace std;


	ConferenceLibraryAssist::ConferenceLibraryAssist()
	:_conferenceHandle(NULL){
		LoadConferenceLibrary();
		UC_LOG(INFO) << "LoadConferenceLibrary";
	}

	ConferenceLibraryAssist::~ConferenceLibraryAssist() {
		FreeConferenceLibrary();
		UC_LOG(INFO) << "FreeConferenceLibrary";
	}

	bool ConferenceLibraryAssist::LoadConferenceLibrary() {
		TCHAR appPath[MAX_PATH] ={0};
		::GetModuleFileName(NULL, appPath, MAX_PATH);

		std::wstring currentDir = appPath;
		int lastOf =  currentDir.rfind('\\');
		std::wstring conferenceDllDir = currentDir.substr(0, lastOf);
		conferenceDllDir.append(L"\\Conference\\Conference.dll");

		_conferenceHandle = LoadLibraryEx(conferenceDllDir.c_str(), NULL, LOAD_WITH_ALTERED_SEARCH_PATH);
		if (NULL == _conferenceHandle)
		{
			UC_LOG(ERROR) << "LoadLibrary Conference.dll at directory: " << conferenceDllDir << ", error, error code: " << GetLastError();
			return false;
		}

		UC_LOG(WARNING) << "LoadConferenceLibrary success, conferenceDllDir:" << conferenceDllDir;
		return true;
	}

	CREATE_CONFERENCE_FUN ConferenceLibraryAssist::GetFunCreateConference() {
		return (CREATE_CONFERENCE_FUN)GetProcAddress(_conferenceHandle, "createGNetTangConferenceWithAESECB128");
	}

	DELETE_CONFERENCE_FUN ConferenceLibraryAssist::GetFunDeleteConference() {
		return (DELETE_CONFERENCE_FUN)GetProcAddress(_conferenceHandle, "deleteGNetTangConference");
	}

	CREATE_TANG_SERVICE_FUN ConferenceLibraryAssist::GetFunCreateTangService() {
		return (CREATE_TANG_SERVICE_FUN)GetProcAddress(_conferenceHandle, "getGNetTangService");
	}

	void ConferenceLibraryAssist::FreeConferenceLibrary() {
		if (NULL != _conferenceHandle)
		{
			FreeLibrary(_conferenceHandle);
			_conferenceHandle = NULL;
		}

		UC_LOG(WARNING) << "FreeConferenceLibrary success";
	}
}

#endif