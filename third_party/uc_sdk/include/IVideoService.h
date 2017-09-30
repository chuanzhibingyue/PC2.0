/*
 * IVideoService.h
 *
 *  Created on: May 6, 2015
 *      Author: ethan
 */

#ifndef IVIDEOSERVICE_H_
#define IVIDEOSERVICE_H_

#include "UCSDK.h"
#include "IUCEngine.h"
#include "ignettangbase.h"

namespace uc {

	interface UC_API IVideoService: public IUCService {
		virtual ~IVideoService() {};

		virtual int StartLocalView(TangVideoViewHandle pWindow, unsigned int nWidth, unsigned int nHeight) = 0;
		virtual int StopLocalView() = 0;
		virtual int StartRemoteView(unsigned int userId, TangVideoViewHandle pWindow) = 0;
		virtual int StopRemoteView(unsigned int userId) = 0;
		virtual int RequestResolution(unsigned int nUserID, unsigned int nWidth, unsigned int nHeight) = 0;
		virtual int ChangeShareCamera(unsigned int nUserID, int nDevicIndex) = 0;
	};

	interface UC_API IVideoServiceEvent {
		virtual ~IVideoServiceEvent() {};
	};
}

#endif /* IVIDEOSERVICE_H_ */
