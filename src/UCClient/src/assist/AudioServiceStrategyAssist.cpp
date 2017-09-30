#include "AudioServiceStrategyAssist.h"
#include "CefJSConversion.h"
#include "../UCDefs.h"
#include "../UCManager.h"
#include "LogManager.h"

namespace ucclient {
using namespace cefIntegration;

AudioServiceStrategyAssist::AudioServiceStrategyAssist() {
	_keyValue.insert(make_pair("CreateCallById", &AudioServiceStrategyAssist::_CreateCallById));
	_keyValue.insert(make_pair("CreateCallByUserList", &AudioServiceStrategyAssist::_CreateCallByUserList));
	_keyValue.insert(make_pair("JoinCall",  &AudioServiceStrategyAssist::_JoinCall));
	_keyValue.insert(make_pair("LeaveCall",  &AudioServiceStrategyAssist::_LeaveCall));
	_keyValue.insert(make_pair("ReconnectConference",  &AudioServiceStrategyAssist::_ReconnectConference));
	_keyValue.insert(make_pair("SwitchToPhone",  &AudioServiceStrategyAssist::_SwitchToPhone));
	_keyValue.insert(make_pair("SwitchToVOIP",  &AudioServiceStrategyAssist::_SwitchToVOIP));
	_keyValue.insert(make_pair("MuteSelf",  &AudioServiceStrategyAssist::_MuteSelf));
	_keyValue.insert(make_pair("UnMuteSelf",  &AudioServiceStrategyAssist::_UnMuteSelf));
	_keyValue.insert(make_pair("HangupPhone", &AudioServiceStrategyAssist::_HangupPhone));
	_keyValue.insert(make_pair("InviteUsersToSingleCall", &AudioServiceStrategyAssist::_InviteUsersToSingleCall));
	_keyValue.insert(make_pair("InviteUsersToGroupCall", &AudioServiceStrategyAssist::_InviteUsersToGroupCall));
	_keyValue.insert(make_pair("GetUserCallStatus", &AudioServiceStrategyAssist::_GetUserCallStatus));
	_keyValue.insert(make_pair("GetHistoryPhoneNum", &AudioServiceStrategyAssist::_GetHistoryPhoneNum));
	_keyValue.insert(make_pair("GetLastAudioInvite", &AudioServiceStrategyAssist::_GetLastAudioInvite));

	_keyValue.insert(make_pair("StartSpeakerDiagnosis", &AudioServiceStrategyAssist::_StartSpeakerDiagnosis));
	_keyValue.insert(make_pair("StartMikeDiagnosis", &AudioServiceStrategyAssist::_StartMikeDiagnosis));
	_keyValue.insert(make_pair("StopSpeakerDiagnosis", &AudioServiceStrategyAssist::_StopSpeakerDiagnosis));
	_keyValue.insert(make_pair("StopMikeDiagnosis", &AudioServiceStrategyAssist::_StopMikeDiagnosis));
}

AudioServiceStrategyAssist::~AudioServiceStrategyAssist() {

}

int AudioServiceStrategyAssist::CallAudioServiceMethod(const std::string &method, int argLen, CefRefPtr<CefListValue> arguments, int requestId) {
	KeyValueIter iter = _keyValue.find(method);
	if (iter == _keyValue.end()) {
		UC_LOG(ERROR) << "There is no method: " << method << " in class AudioServiceStrategyAssist";
		return UCCLIENT_METHOD_NOTFOUND;
	}

	pClassFun fun = iter->second;
	return (this->*fun)(argLen, arguments, requestId);
}

int AudioServiceStrategyAssist::_CreateCallById(int argLen, CefRefPtr<CefListValue> arguments, int requestId) {
	bool isParameterValid = (argLen == 6)
		&& arguments->GetType(4) == VTYPE_DICTIONARY
		&& (CefJSConversion::IsJsNumber(arguments, 5));

	if (!isParameterValid) {
		return UCCLIENT_ARGUMENT_ERROR;
	}
	else {
		UCID ucid;
		ucid.SetUserId((int32_t)arguments->GetDictionary(4)->GetDouble(L"userId"));
		ucid.SetSiteId((int32_t)arguments->GetDictionary(4)->GetDouble(L"siteId"));
		ucid.SetResId((int32_t)arguments->GetDictionary(4)->GetDouble(L"resId"));
		int64_t type = (int64_t)CefJSConversion::JSNumberToDouble(arguments, 5);
		return UCManager::GetInstance().GetAudioServiceStrategy()->CreateCallById(ucid, type);
	}
}

int AudioServiceStrategyAssist::_CreateCallByUserList(int argLen, CefRefPtr<CefListValue> arguments, int requestId) {
	bool isParameterValid = (argLen == 6)
		&& (arguments->GetType(4) == VTYPE_STRING
		&& arguments->GetType(5) == VTYPE_LIST);

	if (!isParameterValid) {
		return UCCLIENT_ARGUMENT_ERROR;
	}
	else {
		wstring groupName = arguments->GetString(4).ToWString();
		CefRefPtr<CefListValue> ceflist = arguments->GetList(5);
		std::vector<UserInfo> userInfo;
		GetUserList(ceflist, userInfo);

		return UCManager::GetInstance().GetAudioServiceStrategy()->CreateCallByUserList(groupName, userInfo);
	}
}

int AudioServiceStrategyAssist::_JoinCall(int argLen, CefRefPtr<CefListValue> arguments, int requestId) {
	bool isParameterValid = (argLen == 7)
		&& (CefJSConversion::IsJsNumber(arguments, 4))
		&& (arguments->GetType(5) == VTYPE_STRING)
		&& (CefJSConversion::IsJsNumber(arguments, 6));

	if (!isParameterValid) {
		return UCCLIENT_ARGUMENT_ERROR;
	}

	int callType = arguments->GetInt(4);
	wstring tempConferenceId = arguments->GetString(5).ToWString();
	int64_t groupId = (int64_t) CefJSConversion::JSNumberToDouble(arguments, 6);
	return UCManager::GetInstance().GetAudioServiceStrategy()->JoinCall(callType, tempConferenceId, groupId);
}

int AudioServiceStrategyAssist::_LeaveCall(int argLen, CefRefPtr<CefListValue> arguments, int requestId) {
	bool isParameterValid = (argLen == 5)
		&& (arguments->GetType(4) == VTYPE_STRING);

	if (!isParameterValid)
	{
		return UCCLIENT_ARGUMENT_ERROR;
	}
	
	wstring tempConferenceId = arguments->GetString(4).ToWString();
	return UCManager::GetInstance().GetAudioServiceStrategy()->LeaveCall(tempConferenceId);
}

int AudioServiceStrategyAssist::_ReconnectConference(int argLen, CefRefPtr<CefListValue> arguments, int requestId) {
	bool isParameterValid = (argLen == 4);

	if (!isParameterValid)
	{
		return UCCLIENT_ARGUMENT_ERROR;
	}

	return UCManager::GetInstance().GetAudioServiceStrategy()->ReconnectConference();
}

int AudioServiceStrategyAssist::_SwitchToPhone(int argLen, CefRefPtr<CefListValue> arguments, int requestId) {
	bool isParameterValid = (argLen == 5)
		&& (arguments->GetType(4) == VTYPE_STRING);

	if (!isParameterValid)
	{
		return UCCLIENT_ARGUMENT_ERROR;
	}

	std::string phoneNumber = arguments->GetString(4);
	return UCManager::GetInstance().GetAudioServiceStrategy()->SwitchToPhone(phoneNumber);
}

int AudioServiceStrategyAssist::_SwitchToVOIP(int argLen, CefRefPtr<CefListValue> arguments, int requestId) {
	bool isParameterValid = (argLen == 4);

	if (!isParameterValid)
	{
		return UCCLIENT_ARGUMENT_ERROR;
	}

	return UCManager::GetInstance().GetAudioServiceStrategy()->SwitchToVOIP();
}

int AudioServiceStrategyAssist::_MuteSelf(int argLen, CefRefPtr<CefListValue> arguments, int requestId) {
	bool isParameterValid = (argLen == 4);

	if (!isParameterValid)
	{
		return UCCLIENT_ARGUMENT_ERROR;
	}

	return UCManager::GetInstance().GetAudioServiceStrategy()->MuteSelf();
}

int AudioServiceStrategyAssist::_UnMuteSelf(int argLen, CefRefPtr<CefListValue> arguments, int requestId) {
	bool isParameterValid = (argLen == 4);

	if (!isParameterValid)
	{
		return UCCLIENT_ARGUMENT_ERROR;
	}

	return UCManager::GetInstance().GetAudioServiceStrategy()->UnMuteSelf();
}

int AudioServiceStrategyAssist::_HangupPhone(int argLen, CefRefPtr<CefListValue> arguments, int requestId) {
	bool isParameterValid = (argLen == 4);

	if (!isParameterValid)
	{
		return UCCLIENT_ARGUMENT_ERROR;
	}

	return UCManager::GetInstance().GetAudioServiceStrategy()->HangupPhone();
}

int AudioServiceStrategyAssist::_InviteUsersToSingleCall(int argLen, CefRefPtr<CefListValue> arguments, int requestId) {
	bool isParameterValid = (argLen == 8)
		&& (arguments->GetType(4) == VTYPE_INT
		&& arguments->GetType(5) == VTYPE_STRING
		&& arguments->GetType(6) == VTYPE_LIST
		&& arguments->GetType(7) == VTYPE_STRING);

	if (!isParameterValid) {
		return UCCLIENT_ARGUMENT_ERROR;
	}
	else {
		int callType = arguments->GetInt(4);
		wstring groupName = arguments->GetString(5).ToWString();
		CefRefPtr<CefListValue> ceflist = arguments->GetList(6);
		std::vector<UserInfo> userInfo;
		GetUserList(ceflist, userInfo);
		wstring conferenceId = arguments->GetString(7).ToWString();
		return UCManager::GetInstance().GetAudioServiceStrategy()->InviteUsersToSingleCall(callType, groupName, userInfo, conferenceId);
	}
}

int AudioServiceStrategyAssist::_InviteUsersToGroupCall(int argLen, CefRefPtr<CefListValue> arguments, int requestId) {
	bool isParameterValid = (argLen == 8)
		&& (arguments->GetType(4) == VTYPE_INT
		&& arguments->GetType(5) == VTYPE_LIST
		&& arguments->GetType(6) == VTYPE_INT
		&& arguments->GetType(7) == VTYPE_STRING);

	if (!isParameterValid) {
		return UCCLIENT_ARGUMENT_ERROR;
	}
	else {
		int callType = arguments->GetInt(4);
		CefRefPtr<CefListValue> ceflist = arguments->GetList(5);
		std::vector<UserInfo> userInfo;
		GetUserList(ceflist, userInfo);
		int groupId = arguments->GetInt(6);
		wstring conferenceId = arguments->GetString(7).ToWString();
		return UCManager::GetInstance().GetAudioServiceStrategy()->InviteUsersToGroupCall(callType, userInfo, groupId, conferenceId);
	}
}

int AudioServiceStrategyAssist::_GetUserCallStatus(int argLen, CefRefPtr<CefListValue> arguments, int requestId) {
	bool isParameterValid = (argLen == 7)
		&& (arguments->GetType(4) == VTYPE_LIST)
		&& (arguments->GetType(5) == VTYPE_INT)
		&& (arguments->GetType(6) == VTYPE_INT);

	if (!isParameterValid) {
		return UCCLIENT_ARGUMENT_ERROR;
	}
	else {
		std::vector<int32_t> userIds;
		CefRefPtr<CefListValue> ceflist = arguments->GetList(4);
		for (std::size_t i = 0; i < ceflist->GetSize(); ++i) {
			userIds.push_back(ceflist->GetInt(i));
		}
		if (userIds.empty()) 
			return UCCLIENT_ARGUMENT_ERROR;

		int32_t id = arguments->GetInt(5);
		int32_t type = arguments->GetInt(6);
		return UCManager::GetInstance().GetAudioServiceStrategy()->GetUserCallStatus(userIds, id, type);
	}
}

int AudioServiceStrategyAssist::_GetHistoryPhoneNum(int argLen, CefRefPtr<CefListValue> arguments, int requestId) {
	bool isParameterValid = (argLen == 4);

	if (!isParameterValid)
	{
		return UCCLIENT_ARGUMENT_ERROR;
	}

	return UCManager::GetInstance().GetAudioServiceStrategy()->GetHistoryPhoneNum();
}

int AudioServiceStrategyAssist::_GetLastAudioInvite(int argLen, CefRefPtr<CefListValue> arguments, int requestId) {
	bool isParameterValid = (argLen == 5)
		&& arguments->GetType(4) == VTYPE_INT;

	if (!isParameterValid)
	{
		return UCCLIENT_ARGUMENT_ERROR;
	}
	else {
		int32_t userId = arguments->GetInt(4);
		return UCManager::GetInstance().GetAudioServiceStrategy()->GetLastAudioInvite(userId, requestId);
	}
}

int AudioServiceStrategyAssist::_StartSpeakerDiagnosis(int argLen, CefRefPtr<CefListValue> arguments, int requestId) {
	bool isParameterValid = (argLen == 5)
		&& arguments->GetType(4) == VTYPE_STRING;

	if (!isParameterValid)
	{
		return UCCLIENT_ARGUMENT_ERROR;
	}
	else {
		wstring speak = arguments->GetString(4).ToWString();
		return UCManager::GetInstance().GetAudioServiceStrategy()->StartSpeakerDiagnosis(speak);
	}
}

int AudioServiceStrategyAssist::_StartMikeDiagnosis(int argLen, CefRefPtr<CefListValue> arguments, int requestId) {
	bool isParameterValid = (argLen == 5)
		&& arguments->GetType(4) == VTYPE_STRING;

	if (!isParameterValid)
	{
		return UCCLIENT_ARGUMENT_ERROR;
	}
	else {
		wstring mike = arguments->GetString(4).ToWString();
		return UCManager::GetInstance().GetAudioServiceStrategy()->StartMikeDiagnosis(mike);
	}
}

int AudioServiceStrategyAssist::_StopSpeakerDiagnosis(int argLen, CefRefPtr<CefListValue> arguments, int requestId) {
	bool isParameterValid = (argLen == 4);

	if (!isParameterValid)
	{
		return UCCLIENT_ARGUMENT_ERROR;
	}

	return UCManager::GetInstance().GetAudioServiceStrategy()->StopSpeakerDiagnosis();
}

int AudioServiceStrategyAssist::_StopMikeDiagnosis(int argLen, CefRefPtr<CefListValue> arguments, int requestId) {
	bool isParameterValid = (argLen == 4);

	if (!isParameterValid)
	{
		return UCCLIENT_ARGUMENT_ERROR;
	}

	return UCManager::GetInstance().GetAudioServiceStrategy()->StopMikeDiagnosis();
}

void AudioServiceStrategyAssist::GetUserList(CefRefPtr<CefListValue>& ceflist, std::vector<UserInfo>& userInfo) {
	UserInfo info;
	for (std::size_t i = 0; i < ceflist->GetSize(); ++i) {
		if (ceflist->GetType(i) == VTYPE_DICTIONARY) {
			CefRefPtr<CefDictionaryValue> dicValues = ceflist->GetDictionary(i)->GetDictionary(L"uid");
			info.ucid.SetUserId(dicValues->GetDouble(L"userId"));
			info.ucid.SetSiteId(dicValues->GetDouble(L"siteId"));
			info.ucid.SetResId(dicValues->GetDouble(L"resId"));
			info.userName = ceflist->GetDictionary(i)->GetString(L"userName");
			userInfo.push_back(info);
		}
	}
}


}