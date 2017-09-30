/*
 * CalendarServiceStrategy.h
 *
 *  Created on: Aug 19, 2015
 *      Author: hualong zhang
 */

#ifndef AUDIO_SERVICE_STRATEGY_ASSIST_H_
#define AUDIO_SERVICE_STRATEGY_ASSIST_H_

#include "IAudioService.h"
#include "CefClient.h"
#include <string>
#include <map>

namespace ucclient {

using namespace uc;
using namespace cefIntegration;

class AudioServiceStrategyAssist {
public:
	AudioServiceStrategyAssist();
	~AudioServiceStrategyAssist();
	int CallAudioServiceMethod(const std::string &method, int argLen, CefRefPtr<CefListValue> arguments, int requestId);

private:
	int _CreateCallById(int argLen, CefRefPtr<CefListValue> arguments, int requestId);
	int _CreateCallByUserList(int argLen, CefRefPtr<CefListValue> arguments, int requestId);
	int _JoinCall(int argLen, CefRefPtr<CefListValue> arguments, int requestId);
	int _LeaveCall(int argLen, CefRefPtr<CefListValue> arguments, int requestId);
	int _ReconnectConference(int argLen, CefRefPtr<CefListValue> arguments, int requestId);
	int _SwitchToPhone(int argLen, CefRefPtr<CefListValue> arguments, int requestId);
	int _SwitchToVOIP(int argLen, CefRefPtr<CefListValue> arguments, int requestId);
	int _MuteSelf(int argLen, CefRefPtr<CefListValue> arguments, int requestId);
	int _UnMuteSelf(int argLen, CefRefPtr<CefListValue> arguments, int requestId);
	int _HangupPhone(int argLen, CefRefPtr<CefListValue> arguments, int requestId);
	int _InviteUsersToSingleCall(int argLen, CefRefPtr<CefListValue> arguments, int requestId);
	int _InviteUsersToGroupCall(int argLen, CefRefPtr<CefListValue> arguments, int requestId);
	int _GetUserCallStatus(int argLen, CefRefPtr<CefListValue> arguments, int requestId);
	int _GetHistoryPhoneNum(int argLen, CefRefPtr<CefListValue> arguments, int requestId);
	int _GetLastAudioInvite(int argLen, CefRefPtr<CefListValue> arguments, int requestId);

	int _StartSpeakerDiagnosis(int argLen, CefRefPtr<CefListValue> arguments, int requestId);
	int _StartMikeDiagnosis(int argLen, CefRefPtr<CefListValue> arguments, int requestId);
	int _StopSpeakerDiagnosis(int argLen, CefRefPtr<CefListValue> arguments, int requestId);
	int _StopMikeDiagnosis(int argLen, CefRefPtr<CefListValue> arguments, int requestId);

	void GetUserList(CefRefPtr<CefListValue>& ceflist, std::vector<UserInfo>& userInfo);

	typedef int (AudioServiceStrategyAssist::*pClassFun)(int, CefRefPtr<CefListValue>, int requestId);
	typedef std::map<std::string, pClassFun> KeyValue;
	typedef std::map<std::string, pClassFun>::const_iterator KeyValueIter;
	KeyValue _keyValue;
};

}

#endif //AUDIO_SERVICE_STRATEGY_ASSIST_H_