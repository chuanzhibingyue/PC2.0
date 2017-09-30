/*
 * CalendarServiceStrategy.h
 *
 *  Created on: Aug 19, 2015
 *      Author: hualong zhang
 */

#ifndef AUDIO_SERVICE_JS_EVENT_ASSIST_H_
#define AUDIO_SERVICE_JS_EVENT_ASSIST_H_

#include "JSEventAssist.h"
#include "common/platform.h"
#include "ITangUser.h"
#include "audio_service/CallDef.h"
#include <vector>
#include <list>

namespace ucclient {
using namespace uc;
class AudioServiceJSEventAssist : virtual public JSEventAssist {
public:
	AudioServiceJSEventAssist(cefIntegration::CefClient *cefClient);
	virtual ~AudioServiceJSEventAssist();
	std::string MakeParametersForOnCallCreated(int32_t errorCode, int type, int tempConferenceId, int Id);
	std::string MakeParametersForOnCallJoined(int32_t errorCode, int type, int tempConferenceId, int id);
	std::string MakeParametersForOnCallLeaved(int32_t errorCode, int tempConferenceId, int callType, int userId);
	std::string MakeParametersForOnConferenceEnded(int conferenceId, int callType, int statusCode);
	std::string MakeParametersForStatus(int statusCode, int conferenceId);
	std::string MakeParametersForITangUser(const ITangUser *tangUser, int conferenceId, int callType);
	std::string MakeParametersForITangUserAndPropertyType(const ITangUser *tangUser, int propertyType, int conferenceId, int callType);
	std::string MakeParametersForOnCallPhoneFailed(const std::string &phoneNumber, const std::string &errorCode, int conferenceId);
	std::string MakeParametersForConferenceId(int conferenceId);
	std::string MakeParametersForOnVoipNetworkQuality(int conferenceId, int callType, int userId, int quality);
	std::string MakeParametersForOnVolumeStatusChanged(int type, int value);
	std::string MakeParametersForOnSpeakingStatusChanged(int conferenceId, int callType, int userId, int status);
	std::string MakeParametersForOnAddMemberToCall(int32_t errorCode, int callType, int tempConferenceId, int id);
	std::string MakeParametersForOnUserCallStatusGot(int32_t errorCode, const std::vector<UserCallStatus>& userStatus, int32_t id, int32_t type);
	std::string MakeParametersForOnHistoryPhoneNumGot(int32_t errorCode, const std::list<std::string>& phoneNums);
	std::string MakeParametersForOnCallInvited(int32_t errorCode, int32_t userId, int32_t conferenceId, int callType, int chatType);
};

}

#endif //AUDIO_SERVICE_JS_EVENT_ASSIST_H_