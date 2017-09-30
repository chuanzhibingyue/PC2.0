/*
 * VideoService.h
 *
 *  Created on: May 6, 2015
 *      Author: ethan
 */

#ifndef VIDEOSERVICE_H_
#define VIDEOSERVICE_H_

#include "IVideoService.h"
#include "conferencesdk/ConferenceWrapper.h"

namespace uc {

	class VideoService : public IVideoService, IVideoEvent {
public:
	VideoService(IUCEngine *engine);
	virtual ~VideoService();

	virtual int StartLocalView(TangVideoViewHandle pWindow, unsigned int nWidth, unsigned int nHeight);
	virtual int StopLocalView();

	virtual int StartRemoteView(unsigned int userId, TangVideoViewHandle pWindow);
	virtual int StopRemoteView(unsigned int userId);

	virtual int RequestResolution(unsigned int nUserID, unsigned int nWidth, unsigned int nHeight);
	virtual int ChangeShareCamera(unsigned int nUserID, int nDevicIndex);

	virtual void OnVideoPropertyChanged();
	virtual void OnVideoInstanceAdded(unsigned int userId);
	virtual void OnVideoInstanceRemoved(unsigned int userId);
	virtual void OnSessionErrorHandle();
};

} /* namespace uc */

#endif /* VIDEOSERVICE_H_ */
