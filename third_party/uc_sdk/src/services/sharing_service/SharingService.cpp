/*
 * SharingService.cpp
 *
 *  Created on: May 6, 2015
 *      Author: ethan
 */

#include "SharingService.h"

namespace uc {

SharingService::SharingService(IUCEngine *engine) {
	_engine = engine;
	ConferenceWrapper::GetConferenceWrapper().SetDesktopEvent(this);
	_sharingType = NO_SHARE;
}

SharingService::~SharingService() {
	// TODO Auto-generated destructor stub
}

int SharingService::StartDesktopShare() {
	_sharingType = SHARE_USER;
	return ConferenceWrapper::GetConferenceWrapper().StartDesktopShare();
}

int SharingService::StopDesktopShare() {
	_sharingType = NO_SHARE;
	return ConferenceWrapper::GetConferenceWrapper().StopDesktopShare();
}

int SharingService::StartDesktopView(VIEW view) {
	_sharingType = VIEW_USER;
	return ConferenceWrapper::GetConferenceWrapper().StartDesktopView(view);
}

int SharingService::StopDesktopView() {
	_sharingType = NO_SHARE;
	return ConferenceWrapper::GetConferenceWrapper().StopDesktopView();
}

int SharingService::Close() {
	int ret = 0;
	if (_sharingType == SHARE_USER) {
		ret = StopDesktopShare();
	}
	else if (_sharingType == VIEW_USER) {
		ret = StopDesktopView();
	}
	else {
		//
	}
	return ret;
}

void SharingService::OnErrorHandle(int statusCode) {
	UC_LOG(INFO) << "OnErrorHandle, status code:" << statusCode;
	triggerEvent(ISharingServiceEvent, OnErrorHandle(statusCode));
}

void SharingService::OnDesktopShared(int conferenceId, int userId) {
	UC_LOG(INFO) << "OnDesktopShared";
	triggerEvent(ISharingServiceEvent, OnDesktopShared(conferenceId, userId));
}

void SharingService::OnDesktopShareStoped(int conferenceId, int userId) {
	UC_LOG(INFO) << "OnDesktopShareStoped";
	triggerEvent(ISharingServiceEvent, OnDesktopShareStoped(conferenceId, userId));
}

void SharingService::OnDesktopViewerStarted(int conferenceId) {
	UC_LOG(INFO) << "OnDesktopViewerStarted";
	triggerEvent(ISharingServiceEvent, OnDesktopViewerStarted(conferenceId));
}

void SharingService::OnDesktopViewerStopped(int conferenceId) {
	UC_LOG(INFO) << "OnDesktopViewerStopped";
	triggerEvent(ISharingServiceEvent, OnDesktopViewerStopped(conferenceId));
}

} /* namespace uc */
