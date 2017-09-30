#include "AudioServiceStrategy.h"
#include "ServiceObjectConversion.h"
#include "UCDefs.h"
#include "UCManager.h"

namespace ucclient {

AudioServiceStrategy::AudioServiceStrategy(IUCEngine *engine, cefIntegration::CefClient *cefClient) 
	: _engine(engine)
	, _jsEventAssist(cefClient) {
	_audioService = (IAudioService*)_engine->QueryService(UCAudioService);
}

int32_t AudioServiceStrategy::CreateCallById(UCID id, int32_t type) {
	UC_LOG(INFO) << "CreateCallById, id:" << id.GetUserId() << ", type:" << type;
	return _audioService->CreateCallById(id, type);
}

int32_t AudioServiceStrategy::CreateCallByUserList(const std::wstring& groupName, std::vector<UserInfo>& userInfo) {
	UC_LOG(INFO) << "CreateCallByUserList, groupName:" << groupName << ", user count:" << userInfo.size();
	return _audioService->CreateCallByUserList(groupName, userInfo);
}

int32_t AudioServiceStrategy::JoinCall(int callType, const std::wstring& tempConferenceId, int64_t groupId) {
	UC_LOG(INFO) << "JoinCall, callType:" << callType << ", conferenceId:" << tempConferenceId << ", groupId:" << groupId;
	return _audioService->JoinCall(callType, tempConferenceId, groupId);
}

int32_t AudioServiceStrategy::InviteUsersToSingleCall(int callType, const std::wstring& groupName, 
	std::vector<UserInfo>& userInfo, const std::wstring& conferenceId) {
	UC_LOG(INFO) << "InviteUsersToSingleCall, callType:" << callType << ", groupName:" << groupName
		<< ", user count:" << userInfo.size() << ", conferenceId:" << conferenceId;
	return _audioService->InviteUsersToSingleCall(callType, groupName, userInfo, conferenceId);
}

int32_t AudioServiceStrategy::InviteUsersToGroupCall(int callType, std::vector<UserInfo>& userInfo, int groupId, const std::wstring& conferenceId) {
	UC_LOG(INFO) << "InviteUsersToGroupCall, callType:" << callType << ", groupId:" << groupId << ", conferenceId:" << conferenceId;
	return _audioService->InviteUsersToGroupCall(callType, userInfo, groupId, conferenceId);
}

int32_t AudioServiceStrategy::LeaveCall(const std::wstring& tempConferenceId) {
	UC_LOG(INFO) << "LeaveCall, conferenceId:" << tempConferenceId;
	return _audioService->LeaveCall(tempConferenceId);
}

int32_t AudioServiceStrategy::ReconnectConference() {
	UC_LOG(INFO) << "ReconnectConference";
	return _audioService->ReconnectConference();
}

int32_t AudioServiceStrategy::SwitchToPhone(const std::string &phoneNumber) {
	UC_LOG(INFO) << "SwitchToPhone, phoneNumber:" << phoneNumber;
	return _audioService->SwitchToPhone(phoneNumber);
}

int32_t AudioServiceStrategy::SwitchToVOIP() {
	UC_LOG(INFO) << "SwitchToVOIP";
	return _audioService->SwitchToVOIP();
}

int32_t AudioServiceStrategy::MuteSelf() {
	UC_LOG(INFO) << "MuteSelf";
	return _audioService->MuteSelf();
}

int32_t AudioServiceStrategy::UnMuteSelf() {
	UC_LOG(INFO) << "UnMuteSelf";
	return _audioService->UnMuteSelf();
}

int32_t AudioServiceStrategy::HangupPhone() {
	UC_LOG(INFO) << "HangupPhone";
	return _audioService->HangupPhone();
}

int32_t AudioServiceStrategy::GetUserCallStatus(std::vector<int32_t>& userIds, int32_t id, int32_t type) {
	UC_LOG(INFO) << "GetUserCallStatus";
	return _audioService->GetUserCallStatus(userIds, id, type);
}

int32_t AudioServiceStrategy::GetHistoryPhoneNum() {
	UC_LOG(INFO) << "GetHistoryPhoneNum";
	return _audioService->GetHistoryPhoneNum();
}

int32_t AudioServiceStrategy::GetSpeakerNum() {
	UC_LOG(INFO) << "GetSpeakerNum";
	return _audioService->GetSpeakerNum();
}

int32_t AudioServiceStrategy::GetMicrophoneNum() {
	UC_LOG(INFO) << "GetMicrophoneNum";
	return _audioService->GetMicrophoneNum();
}

//ªÒ»°—Ô…˘∆˜¡–±Ì
std::wstring AudioServiceStrategy::GetSpeakerList() {
	UC_LOG(INFO) << "GetSpeakerList";
	return _audioService->GetSpeakerList();
}

//ªÒ»°¬ÛøÀ∑Á¡–±Ì
std::wstring AudioServiceStrategy::GetMicrophoneList() {
	UC_LOG(INFO) << "GetMicrophoneList";
	return _audioService->GetMicrophoneList();
}

//—Ô…˘∆˜’Ô∂œ
int32_t AudioServiceStrategy::StartSpeakerDiagnosis(const std::wstring& speaker) {
	UC_LOG(INFO) << "StartSpeakerDiagnosis, speaker:" << speaker;
	return _audioService->StartSpeakerDiagnosis(speaker);
}

//¬ÛøÀ∑Á’Ô∂œ
int32_t AudioServiceStrategy::StartMikeDiagnosis(const std::wstring& mike) {
	UC_LOG(INFO) << "StartMikeDiagnosis, mike:" << mike;
	return _audioService->StartMikeDiagnosis(mike);
}

//Õ£÷π—Ô…˘∆˜’Ô∂œ
int32_t AudioServiceStrategy::StopSpeakerDiagnosis() {
	UC_LOG(INFO) << "StopSpeakerDiagnosis";
	return _audioService->StopSpeakerDiagnosis();
}

//Õ£÷π¬ÛøÀ∑Á’Ô∂œ
int32_t AudioServiceStrategy::StopMikeDiagnosis() {
	UC_LOG(INFO) << "StopMikeDiagnosis";
	return _audioService->StopMikeDiagnosis();
}

//ªÒ»°…„œÒÕ∑¡–±Ì
std::wstring AudioServiceStrategy::GetCameraList() {
	UC_LOG(INFO) << "GetCameraList";
	return _audioService->GetCameraList();
}

//…„œÒÕ∑≤‚ ‘
int32_t AudioServiceStrategy::CameraDiagnosis(const std::wstring& camera, void *pWindow) {
	UC_LOG(INFO) << "CameraDiagnosis, camera:" << camera;
	return _audioService->CameraDiagnosis(camera, pWindow);
}

//Õ£÷π…„œÒÕ∑≤‚ ‘
int32_t AudioServiceStrategy::StopCameraDiagnosis() {
	UC_LOG(INFO) << "StopCameraDiagnosis";
	return _audioService->StopCameraDiagnosis();
}

int32_t AudioServiceStrategy::GetLastAudioInvite(int32_t userId, int32_t requestId) {
	UC_LOG(INFO) << "GetLastAudioInvite, userId:" << userId << ", requestId:" << requestId;
	return _audioService->GetLastAudioInvite(userId, requestId);
}

void AudioServiceStrategy::OnCallCreated(int32_t errorCode, int callType, int tempConferenceId, int id) {
	std::string parameters = _jsEventAssist.MakeParametersForOnCallCreated(errorCode, callType, tempConferenceId, id);
	_jsEventAssist.MakeJSEventAndExecute(AUDIO_SERVICE_ON_CALL_CREATED_EVENT, parameters);
}

void AudioServiceStrategy::OnCallJoined(int32_t errorCode, int callType, int tempConferenceId, int id) {
	std::string parameters = _jsEventAssist.MakeParametersForOnCallJoined(errorCode, callType, tempConferenceId, id);
	_jsEventAssist.MakeJSEventAndExecute(AUDIO_SERVICE_ON_CALL_JOINED_EVENT, parameters);
}

void AudioServiceStrategy::OnCallLeaved(int32_t errorCode, int tempConferenceId, int callType, int userId) {
	UCManager::GetInstance().GetSharingServiceStrategy()->CloseSharingView();
	std::string parameters = _jsEventAssist.MakeParametersForOnCallLeaved(errorCode, tempConferenceId, callType, userId);
	_jsEventAssist.MakeJSEventAndExecute(AUDIO_SERVICE_ON_CALL_LEAVED_EVENT, parameters);
}

void AudioServiceStrategy::OnConferenceEnded(int conferenceId, int callType, int statusCode) {
	UCManager::GetInstance().GetSharingServiceStrategy()->CloseSharingView();
	std::string parameters = _jsEventAssist.MakeParametersForOnConferenceEnded(conferenceId, callType, statusCode);
	_jsEventAssist.MakeJSEventAndExecute(AUDIO_SERVICE_ON_CONFERENCE_ENDED_EVENT, parameters);
}

void AudioServiceStrategy::OnConferenceDisconnected(int conferenceId) {
	std::string parameters = _jsEventAssist.MakeParametersForConferenceId(conferenceId);
	_jsEventAssist.MakeJSEventAndExecute(AUDIO_SERVICE_ON_CONFERENCE_DISCONNECTED_EVENT, parameters);
}

void AudioServiceStrategy::OnConferenceReconnected(int statusCode, int conferenceId) {
	std::string parameters = _jsEventAssist.MakeParametersForStatus(statusCode, conferenceId);
	_jsEventAssist.MakeJSEventAndExecute(AUDIO_SERVICE_ON_CONFERENCE_RECONNECTED_EVENT, parameters);
}

void AudioServiceStrategy::OnUserAdded(const ITangUser *tangUser, int conferenceId, int callType) {
	std::string parameters = _jsEventAssist.MakeParametersForITangUser(tangUser, conferenceId, callType);
	_jsEventAssist.MakeJSEventAndExecute(AUDIO_SERVICE_ON_USER_ADDED_EVENT, parameters);
}

void AudioServiceStrategy::OnUserRemoved(const ITangUser *tangUser, int conferenceId, int callType) {
	std::string parameters = _jsEventAssist.MakeParametersForITangUser(tangUser, conferenceId, callType);
	_jsEventAssist.MakeJSEventAndExecute(AUDIO_SERVICE_ON_USER_REMOVED_EVENT, parameters);
}

void AudioServiceStrategy::OnUserPropertyChanged(const ITangUser *tangUser, int properType, int conferenceId, int callType) {
	std::string parameters = _jsEventAssist.MakeParametersForITangUserAndPropertyType(tangUser, properType, conferenceId, callType);
	_jsEventAssist.MakeJSEventAndExecute(AUDIO_SERVICE_ON_USER_PROPERTY_CHANGED_EVENT, parameters);
}

void AudioServiceStrategy::OnCallPhoneFailed(const std::string &phoneNumber, const std::string &errorCode, int conferenceId) {
	std::string parameters = _jsEventAssist.MakeParametersForOnCallPhoneFailed(phoneNumber, errorCode, conferenceId);
	_jsEventAssist.MakeJSEventAndExecute(AUDIO_SERVICE_ON_CALL_PHONE_FAILED_EVENT, parameters);
}

void AudioServiceStrategy::OnNetworkQualityChanged(int conferenceId, int callType, int userId, int quality) {
	std::string parameters = _jsEventAssist.MakeParametersForOnVoipNetworkQuality(conferenceId, callType, userId, quality);
	_jsEventAssist.MakeJSEventAndExecute(AUDIO_SERVICE_ON_VOIP_NETWORK_QUALITY_CHANGED_EVENT, parameters);
}

void AudioServiceStrategy::OnMicrophoneStatusChanged(int type, int value) {
	std::string parameters = _jsEventAssist.MakeParametersForOnVolumeStatusChanged(type, value);
	_jsEventAssist.MakeJSEventAndExecute(AUDIO_SERVICE_ON_MICROPHONE_STATUS_CHANGED_EVENT, parameters);
}

void AudioServiceStrategy::OnSpeakerStatusChanged(int type, int value) {
	std::string parameters = _jsEventAssist.MakeParametersForOnVolumeStatusChanged(type, value);
	_jsEventAssist.MakeJSEventAndExecute(AUDIO_SERVICE_ON_SPEAKER_STATUS_CHANGED_EVENT, parameters);
}

void AudioServiceStrategy::OnSpeakingStatusChanged(int conferenceId, int callType, int userId, int status) {
	std::string parameters = _jsEventAssist.MakeParametersForOnSpeakingStatusChanged(conferenceId, callType, userId, status);
	_jsEventAssist.MakeJSEventAndExecute(AUDIO_SERVICE_ON_SPEAKING_STATUS_CHANGED_EVENT, parameters);
}

void AudioServiceStrategy::OnAddMemberToGroupCall(int32_t errorCode, int callType, int tempConferenceId, int id) {
	std::string parameters = _jsEventAssist.MakeParametersForOnAddMemberToCall(errorCode, callType, tempConferenceId, id);
	_jsEventAssist.MakeJSEventAndExecute(AUDIO_SERVICE_ON_USERS_ADDED_TO_CALL_EVENT, parameters);
}

void AudioServiceStrategy::OnAddMemberToSingleCall(int32_t errorCode, int callType, int tempConferenceId, int id) {
	std::string parameters = _jsEventAssist.MakeParametersForOnAddMemberToCall(errorCode, callType, tempConferenceId, id);
	_jsEventAssist.MakeJSEventAndExecute(AUDIO_SERVICE_ON_USERS_ADDED_TO_CALL_EVENT, parameters);
}

void AudioServiceStrategy::OnUserCallStatusGot(int32_t errorCode, const vector<UserCallStatus>& userStatus, int32_t id, int32_t type) {
	std::string parameters = _jsEventAssist.MakeParametersForOnUserCallStatusGot(errorCode, userStatus, id, type);
	_jsEventAssist.MakeJSEventAndExecute(AUDIO_SERVICE_ON_USER_CALL_STATUS_GOT_EVENT, parameters);
}

void AudioServiceStrategy::OnHistoryPhoneNumGot(int32_t errorCode, const std::list<string>& phoneNums) {
	std::string parameters = _jsEventAssist.MakeParametersForOnHistoryPhoneNumGot(errorCode, phoneNums);
	_jsEventAssist.MakeJSEventAndExecute(AUDIO_SERVICE_ON_HISTORY_PHONENUM_GOT_EVENT, parameters);
}

void AudioServiceStrategy::OnSpeakerLevelGot(const std::wstring& speaker, int level) {
	std::stringstream levelStream;
	levelStream << "speakerName:\"" << StringConvert::ToUTF8Str(speaker) << "\", speakerLevel:" << level;
	_jsEventAssist.MakeJSEventAndExecute(AUDIO_SERVICE_ON_SPEAKER_LEVEL_GOT_EVENT, levelStream.str());
}

void AudioServiceStrategy::OnMikeLevelGot(const std::wstring& mike, int level) {
	std::stringstream levelStream;
	levelStream << "mikeName:\"" << StringConvert::ToUTF8Str(mike) << "\", mikeLevel:" << level;
	_jsEventAssist.MakeJSEventAndExecute(AUDIO_SERVICE_ON_MIKE_LEVEL_GOT_EVENT, levelStream.str());
}

void AudioServiceStrategy::OnSpeakerPlayFinished(const std::wstring& speaker) {
	std::stringstream playFinish;
	playFinish << "speakerName:\"" << StringConvert::ToUTF8Str(speaker) << "\"";
	_jsEventAssist.MakeJSEventAndExecute(AUDIO_SERVICE_ON_SPEAKER_PLAY_FINISHED_EVENT, playFinish.str());
}

void AudioServiceStrategy::OnInviteCallSend(int32_t errorCode, int32_t userId, int32_t conferenceId, int callType, int32_t chatType) {
	std::string parameters = _jsEventAssist.MakeParametersForOnCallInvited(errorCode, userId, conferenceId, callType, chatType);
	_jsEventAssist.MakeJSEventAndExecute(AUDIO_SERVICE_ON_INVITE_CALL_SEND_EVENT, parameters);
}

void AudioServiceStrategy::OnAudioServiceReady(int status) {
	std::stringstream audioReady;
	audioReady << "audioReadyStatus:" << status;
	_jsEventAssist.MakeJSEventAndExecute(AUDIO_SERVICE_ON_AUDIO_SERVICE_READY_EVENT, audioReady.str());
}

void AudioServiceStrategy::OnAudioDeviceStatusChanged(bool isReady) {
	std::stringstream audioDeviceStatus;
	audioDeviceStatus << "audioDeviceStatusChanged:" << (isReady)? 1:0;
	_jsEventAssist.MakeJSEventAndExecute(AUDIO_SERVICE_ON_AUDIO_DEVICE_STATUS_CHANGED_EVENT, audioDeviceStatus.str());
}

void AudioServiceStrategy::OnLastAudioInviteGot(int32_t errorCode, int32_t conferenceId, int32_t from, int32_t to, int32_t type, int32_t requestId) {
	std::stringstream lastInviteStream;
	lastInviteStream << "errorCode:" << errorCode << ", conferenceId:" << conferenceId
		<< ", from:" << from << ", to:" << to << ", type:" << type << ", requestId:" << requestId;
	_jsEventAssist.MakeJSEventAndExecute(AUDIO_SERVICE_ON_LAST_AUDIO_INVITE_GOT_EVENT, lastInviteStream.str());
}

}