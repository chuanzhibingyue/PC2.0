#include "AudioServiceJSEventAssist.h"
#include "LogManager.h"
#include "common/StringConvert.h"

namespace ucclient {
	using namespace uc;

	AudioServiceJSEventAssist::AudioServiceJSEventAssist(cefIntegration::CefClient *cefClient)
	: JSEventAssist(cefClient) {

	}

	AudioServiceJSEventAssist::~AudioServiceJSEventAssist() {

	}

	std::string AudioServiceJSEventAssist::MakeParametersForOnCallCreated(int32_t errorCode, int callType, 
		int tempConferenceId, int id) {
		KeyValue parameters;
		parameters.insert(make_pair("errorCode", StringConvert::Convert2String(errorCode)));
		parameters.insert(make_pair("conferenceId", _AddSingleQuoteSurroundString(StringConvert::Convert2String(tempConferenceId))));
		parameters.insert(make_pair("callType", StringConvert::Convert2String(callType)));
		parameters.insert(make_pair("userId", StringConvert::Convert2String(id)));

		return _RetrieveParameters(parameters);
	}

	std::string AudioServiceJSEventAssist::MakeParametersForOnCallJoined(int32_t errorCode, int type, 
		int tempConferenceId, int id) {
		KeyValue parameters;
		parameters.insert(make_pair("errorCode", StringConvert::Convert2String(errorCode)));
		parameters.insert(make_pair("conferenceId", _AddSingleQuoteSurroundString(StringConvert::Convert2String(tempConferenceId))));
		parameters.insert(make_pair("callType", StringConvert::Convert2String(type)));
		parameters.insert(make_pair("userId", StringConvert::Convert2String(id)));

		return _RetrieveParameters(parameters);
	}

	std::string AudioServiceJSEventAssist::MakeParametersForOnCallLeaved(int32_t errorCode, int tempConferenceId, int callType, int userId) {
		KeyValue parameters;
		parameters.insert(make_pair("errorCode", StringConvert::Convert2String(errorCode)));
		parameters.insert(make_pair("conferenceId", _AddSingleQuoteSurroundString(StringConvert::Convert2String(tempConferenceId))));
		parameters.insert(make_pair("callType", StringConvert::Convert2String(callType)));
		parameters.insert(make_pair("userId", StringConvert::Convert2String(userId)));

		return _RetrieveParameters(parameters);
	}

	std::string AudioServiceJSEventAssist::MakeParametersForOnConferenceEnded(int conferenceId, int callType, int statusCode) {
		KeyValue parameters;
		parameters.insert(make_pair("errorCode", StringConvert::Convert2String(statusCode)));
		parameters.insert(make_pair("conferenceId", _AddSingleQuoteSurroundString(StringConvert::Convert2String(conferenceId))));
		parameters.insert(make_pair("callType", StringConvert::Convert2String(callType)));

		return _RetrieveParameters(parameters);
	}

	std::string AudioServiceJSEventAssist::MakeParametersForStatus(int statusCode, int conferenceId) {
		KeyValue parameters;
		parameters.insert(make_pair("conferenceId", _AddSingleQuoteSurroundString(StringConvert::Convert2String(conferenceId))));
		parameters.insert(make_pair("errorCode", StringConvert::Convert2String(statusCode)));
		return _RetrieveParameters(parameters);
	}

	std::string AudioServiceJSEventAssist::MakeParametersForITangUser(const ITangUser *tangUser, int conferenceId, int callType) {
		KeyValue parameters;
		parameters.insert(make_pair("conferenceId", _AddSingleQuoteSurroundString(StringConvert::Convert2String(conferenceId))));
		parameters.insert(make_pair("tangUserID", StringConvert::Convert2String(tangUser->ReturnUserID())));
		parameters.insert(make_pair("userID", tangUser->ReturnUserName()));
		parameters.insert(make_pair("isMute", StringConvert::Convert2String(tangUser->IsMute())));
		parameters.insert(make_pair("audioType", StringConvert::Convert2String(tangUser->ReturnAudioType())));
		parameters.insert(make_pair("phoneNumber", _AddSingleQuoteSurroundString(tangUser->ReturnPhoneNumber())));
		parameters.insert(make_pair("callType", StringConvert::Convert2String(callType)));
		
		return _RetrieveParameters(parameters);
	}

	std::string AudioServiceJSEventAssist::MakeParametersForITangUserAndPropertyType(const ITangUser *tangUser,
		int propertyType, int conferenceId, int callType) {
		return _ConcatenateTwoJSParameters(MakeParametersForITangUser(tangUser, conferenceId, callType),
			_RetrieveParameter("propertyType", propertyType));
	}

	std::string AudioServiceJSEventAssist::MakeParametersForOnCallPhoneFailed(const std::string &phoneNumber,
		const std::string &errorCode, int conferenceId) {
		KeyValue parameters;
		parameters.insert(make_pair("conferenceId", _AddSingleQuoteSurroundString(StringConvert::Convert2String(conferenceId))));
		parameters.insert(make_pair("phoneNumber", _AddSingleQuoteSurroundString(phoneNumber)));
		parameters.insert(make_pair("errorCode", _AddSingleQuoteSurroundString(errorCode)));

		return _RetrieveParameters(parameters);
	}

	std::string AudioServiceJSEventAssist::MakeParametersForConferenceId(int conferenceId) {
		KeyValue parameters;
		parameters.insert(make_pair("conferenceId", _AddSingleQuoteSurroundString(StringConvert::Convert2String(conferenceId))));
		return _RetrieveParameters(parameters);
	}

	std::string AudioServiceJSEventAssist::MakeParametersForOnVoipNetworkQuality(int conferenceId, int callType, 
		int userId, int quality) {
		KeyValue parameters;
		parameters.insert(make_pair("conferenceId", _AddSingleQuoteSurroundString(StringConvert::Convert2String(conferenceId))));
		parameters.insert(make_pair("userID", StringConvert::Convert2String(userId)));
		parameters.insert(make_pair("callType", StringConvert::Convert2String(callType)));
		parameters.insert(make_pair("quality", StringConvert::Convert2String(quality)));

		return _RetrieveParameters(parameters);
	}

	std::string AudioServiceJSEventAssist::MakeParametersForOnVolumeStatusChanged(int type, int value) {
		KeyValue parameters;
		parameters.insert(make_pair("type", StringConvert::Convert2String(type)));
		parameters.insert(make_pair("value", StringConvert::Convert2String(value)));
		return _RetrieveParameters(parameters);
	}

	std::string AudioServiceJSEventAssist::MakeParametersForOnSpeakingStatusChanged(int conferenceId, int callType, int userId, int status) {
		KeyValue parameters;
		parameters.insert(make_pair("conferenceId", _AddSingleQuoteSurroundString(StringConvert::Convert2String(conferenceId))));
		parameters.insert(make_pair("userID", StringConvert::Convert2String(userId)));
		parameters.insert(make_pair("callType", StringConvert::Convert2String(callType)));
		parameters.insert(make_pair("isSpeaking", StringConvert::Convert2String(status)));

		return _RetrieveParameters(parameters);
	}

	std::string AudioServiceJSEventAssist::MakeParametersForOnAddMemberToCall(int32_t errorCode, int callType, int tempConferenceId, int id) {
		KeyValue parameters;
		parameters.insert(make_pair("errorCode", StringConvert::Convert2String(errorCode)));
		parameters.insert(make_pair("conferenceId", _AddSingleQuoteSurroundString(StringConvert::Convert2String(tempConferenceId))));
		parameters.insert(make_pair("callType", StringConvert::Convert2String(callType)));
		parameters.insert(make_pair("userID", StringConvert::Convert2String(id)));

		return _RetrieveParameters(parameters);
	}

	std::string AudioServiceJSEventAssist::MakeParametersForOnUserCallStatusGot(int32_t errorCode, 
		const std::vector<UserCallStatus>& userStatus, int32_t id, int32_t type) {
		std::stringstream status;
		status << "errorCode:" << errorCode << ", id:" << id << ", type:" << type << ", callStatus:[";
		for (std::vector<UserCallStatus>::const_iterator itor = userStatus.begin();
			itor != userStatus.end(); ++itor) {
			if (itor != userStatus.begin())
				status << ",";
			status << "{userID:" << itor->userId << ", status:" << itor->status << "}";
		}
		status << "]";
		return status.str();
	}

	std::string AudioServiceJSEventAssist::MakeParametersForOnHistoryPhoneNumGot(int32_t errorCode,
		const std::list<std::string>& phoneNums) {
		std::stringstream phoneList;
		phoneList << "errorCode:" << errorCode << ", phoneNumbers:[";
		for (std::list<string>::const_iterator itor = phoneNums.begin();
			itor != phoneNums.end(); ++itor) {
			if (itor != phoneNums.begin())
				phoneList << ",";
			phoneList << "\"" << *itor << "\"";
		}
		phoneList << "]";
		return phoneList.str();
	}

	std::string AudioServiceJSEventAssist::MakeParametersForOnCallInvited(int32_t errorCode, int32_t userId, int32_t conferenceId, int callType, int chatType) {
		KeyValue parameters;
		parameters.insert(make_pair("errorCode", StringConvert::Convert2String(errorCode)));
		parameters.insert(make_pair("userID", StringConvert::Convert2String(userId)));
		parameters.insert(make_pair("conferenceId", _AddSingleQuoteSurroundString(StringConvert::Convert2String(conferenceId))));
		parameters.insert(make_pair("callType", StringConvert::Convert2String(callType)));
		parameters.insert(make_pair("chatType", StringConvert::Convert2String(chatType)));

		return _RetrieveParameters(parameters);
	}
}