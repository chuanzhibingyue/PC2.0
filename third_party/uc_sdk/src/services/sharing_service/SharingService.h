/*
 * SharingService.h
 *
 *  Created on: May 6, 2015
 *      Author: ethan
 */

#ifndef SHARINGSERVICE_H_
#define SHARINGSERVICE_H_

#include "ISharingService.h"
#include "conferencesdk/ConferenceWrapper.h"

namespace uc {

	enum ShareType {
		NO_SHARE = 0, 
		SHARE_USER, //共享者
		VIEW_USER //观看者
	};

	class SharingService : public ISharingService, ISharingServiceEvent {
public:
	SharingService(IUCEngine *engine);
	virtual ~SharingService();

	//开始共享桌面
	virtual int StartDesktopShare();
	//停止共享桌面
	virtual int StopDesktopShare();

	//开始观看桌面
	virtual int StartDesktopView(VIEW view);
	//停止观看桌面
	virtual int StopDesktopView();

	int Close();

	//回调事件
	virtual void OnErrorHandle(int statusCode);
	virtual void OnDesktopShared(int conferenceId, int userId);
	virtual void OnDesktopShareStoped(int conferenceId, int userId);
	virtual void OnDesktopViewerStarted(int conferenceId);
	virtual void OnDesktopViewerStopped(int conferenceId);

	private:
		ShareType _sharingType;
};

} /* namespace uc */

#endif /* SHARINGSERVICE_H_ */
