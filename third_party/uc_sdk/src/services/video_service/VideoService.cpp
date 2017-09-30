/*
 * VideoService.cpp
 *
 *  Created on: May 6, 2015
 *      Author: ethan
 */

#include "VideoService.h"

namespace uc {

VideoService::VideoService(IUCEngine *engine) {
	// TODO Auto-generated constructor stub
	_engine = engine;
	ConferenceWrapper::GetConferenceWrapper().SetVedioEvent(this);
}

VideoService::~VideoService() {
	// TODO Auto-generated destructor stub
}

int VideoService::StartLocalView(TangVideoViewHandle pWindow, unsigned int nWidth, unsigned int nHeight) {
	int ret = ConferenceWrapper::GetConferenceWrapper().StartLocalView(pWindow, nWidth, nHeight);
	if (ret != 0) {
		unsigned int userId = ConferenceWrapper::GetConferenceWrapper().GetSelfUserID();
		UC_LOG(ERROR) << "StartLocalView return error:" << ret << ", userId:" << userId;
		//triggerEvent(IVideoEvent, OnLocalViewStarted(userId, ret));
	}
	return ret;
}

int VideoService::StopLocalView() {
	int ret = ConferenceWrapper::GetConferenceWrapper().StopLocalView();
	if (ret != 0) {
		unsigned int userId = ConferenceWrapper::GetConferenceWrapper().GetSelfUserID();
		UC_LOG(ERROR) << "StopLocalView return error:" << ret << ", userId:" << userId;
		//triggerEvent(IVideoEvent, OnLocalViewStopped(userId, ret));
	}
	return ret;
}

int VideoService::StartRemoteView(unsigned int userId, TangVideoViewHandle pWindow) {
	return ConferenceWrapper::GetConferenceWrapper().StartRemoteView(userId, pWindow);
}

int VideoService::StopRemoteView(unsigned int userId) {
	return ConferenceWrapper::GetConferenceWrapper().StopRemoteView(userId);
}

int VideoService::RequestResolution(unsigned int nUserID, unsigned int nWidth, unsigned int nHeight) {
	return ConferenceWrapper::GetConferenceWrapper().RequestResolution(nUserID, nWidth, nHeight);
}

int VideoService::ChangeShareCamera(unsigned int nUserID, int nDevicIndex) {
	return ConferenceWrapper::GetConferenceWrapper().ChangeShareCamera(nUserID, nDevicIndex);
}

void VideoService::OnVideoPropertyChanged() {

}

void VideoService::OnVideoInstanceAdded(unsigned int userId) {
	if (ConferenceWrapper::GetConferenceWrapper().GetSelfUserID() == userId) {
		//自己启动共享视频
		//triggerEvent(IVideoEvent, OnLocalViewStarted(userId, 0));
	}
	else {
		//triggerEvent(IVideoEvent, OnVideoInstanceAdded(userId, 0));
	}
}

void VideoService::OnVideoInstanceRemoved(unsigned int userId) {
	if (ConferenceWrapper::GetConferenceWrapper().GetSelfUserID() == userId) {
		//自己结束共享视频
		//triggerEvent(IVideoEvent, OnLocalViewStopped(userId));
	}
	else {
		//triggerEvent(IVideoEvent, OnVideoInstanceRemoved(userId));
	}
}

void VideoService::OnSessionErrorHandle() {

}

} /* namespace uc */
