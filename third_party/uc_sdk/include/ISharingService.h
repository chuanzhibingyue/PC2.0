/*
 * ISharingService.h
 *
 *  Created on: May 6, 2015
 *      Author: ethan
 */

#ifndef ISHARINGSERVICE_H_
#define ISHARINGSERVICE_H_

#include "UCSDK.h"
#include "IUCEngine.h"

#ifdef WIN32
	typedef HWND VIEW;
#else
	typedef UIImageView* VIEW;
#endif

namespace uc {
	interface UC_API ISharingServiceEvent;

	interface UC_API ISharingService : public IUCService, Event<ISharingServiceEvent> {
		virtual ~ISharingService() {};

		virtual int StartDesktopShare() = 0;
		virtual int StopDesktopShare() = 0;
		virtual int StartDesktopView(VIEW view) = 0;
		virtual int StopDesktopView() = 0;
	};

	interface UC_API ISharingServiceEvent {
		virtual ~ISharingServiceEvent() {};

		virtual void OnErrorHandle(int statusCode) = 0;
		virtual void OnDesktopShared(int conferenceId, int userId) = 0;
		virtual void OnDesktopShareStoped(int conferenceId, int userId) = 0;
		virtual void OnDesktopViewerStarted(int conferenceId) = 0;
		virtual void OnDesktopViewerStopped(int conferenceId) = 0;
	};
}

#endif /* ISHARINGSERVICE_H_ */
