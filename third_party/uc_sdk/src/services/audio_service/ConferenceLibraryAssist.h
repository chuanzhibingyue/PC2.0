/*
 * CallRequestAssist.h
 *
 *  Created on: 2015年8月3日
 *      Author: hualong.zhang
 */

#ifndef CONFERENCELIBRARYASSIST_H_
#define CONFERENCELIBRARYASSIST_H_

#include "conferencesdk/ConferenceDef.h"

#if defined(WIN32) || defined(WIN64)

namespace uc {

class ConferenceLibraryAssist {

public:
	ConferenceLibraryAssist();
	virtual ~ConferenceLibraryAssist();
	bool LoadConferenceLibrary();
	CREATE_CONFERENCE_FUN GetFunCreateConference();
	DELETE_CONFERENCE_FUN GetFunDeleteConference();
	CREATE_TANG_SERVICE_FUN GetFunCreateTangService();

private:
	void FreeConferenceLibrary();
	HMODULE _conferenceHandle;
};

}

#endif

#endif //CONFERENCELIBRARYASSIST_H_