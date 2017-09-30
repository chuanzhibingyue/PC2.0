/*
 * AudioService.cpp
 *
 *  Created on: May 6, 2015
 *      Author: ethan
 */

#include "AudioService.h"
#include "LogManager.h"
#include "CallRequestAssist.h"
#include "CallResponseAssist.h"
#include "UCErrorCode.h"
#include "HTTPRequestCallback.h"
#include "common/event.h"
#include "Macro.h"
#include "HTTPResponseAssist.h"
#include "message_service/GroupService.h"
#include "message_service/IMService.h"
#include "contact_service/ContactService.h"
#include "sharing_service/SharingService.h"
#include "common/CallOnMainThread.h"
#include "DBManager.h"

namespace uc {

AudioService::AudioService(IUCEngine *engine) {
	_engine = engine;
	string logPath = _engine->GetConfig().logConfig.logPath;
	ConferenceWrapper::SetLogPath(logPath);
	ConferenceWrapper::GetConferenceWrapper().SetAudioEvent(this);
}

AudioService::~AudioService() {
	// TODO Auto-generated destructor stub
}

int32_t AudioService::CreateCallById(UCID id, int32_t type) {
	UC_LOG(INFO) << "CreateCallById enter, user Id:" << id.GetUserId() 
		<< ", site id:" << id.GetSiteId() << ", res id:" << id.GetResId() << ", type:" << type;
	int64_t groupId = 0;
	CallType callType = (CallType)type;
	if (type != SINGLE_CALL) {
		groupId = id.GetUserId();
	}

	HTTPRequest request;
	_GetCallRequestAssist().MakeRequestOfCreateCall(groupId, request);
	_engine->SendAsyncRequest(request, MakeRequestCallback(this, &AudioService::OnCallCreated, callType, id));
	return UC_SUCCEEDED;
}

int32_t AudioService::CreateCallByUserList(const std::wstring& groupName, std::vector<UserInfo>& userInfo) {
	UC_LOG(INFO) << "CreateCallByUserList enter, groupName:" << groupName << ", members count:" << userInfo.size();
	GroupService* grpService = (GroupService*)_engine->QueryService(UCGroupService);
	return grpService->CreateCallGroup(MUTILCHAT_GROUP_TYPE, StringConvert::ToUTF8Str(groupName), userInfo, this);
}

int AudioService::CreateCallByGroupInfo(const GroupInfo& info) {
	UC_LOG(INFO) << "CreateCallByGroupInfo enter, groupId:" << info.GetId() << ", name:" << info.GetName() << ", type:" << info.GetGroupType();
	HTTPRequest request;
	_GetCallRequestAssist().MakeRequestOfCreateCall(info.GetId(), request);
	UCID uid;
	uid.SetUserId(info.GetId());
	uid.SetSiteId(info.GetSiteId());
	uid.SetResId(0);
	_engine->SendAsyncRequest(request, MakeRequestCallback(this, &AudioService::OnCallCreated, (CallType)info.GetGroupType(), uid));
	return UC_SUCCEEDED;
}

int32_t AudioService::JoinCall(int callType, const std::wstring& tempConferenceId, int64_t groupId) {
	UC_LOG(INFO) << "JoinCall enter, callType:" << callType << ", tempConferenceId:"<< tempConferenceId << ", groupId : " << groupId;
	CallType type = (CallType)callType;
	int confId = std::atoi(StringConvert::ToUTF8Str(tempConferenceId).c_str());
	HTTPRequest request;
	_GetCallRequestAssist().MakeRequestOfJoinCall(confId, groupId, request);
	_engine->SendAsyncRequest(request, MakeRequestCallback(this, &AudioService::OnCallJoined, type, confId, groupId));
	return UC_SUCCEEDED;
}

int32_t AudioService::InviteUsersToSingleCall(int callType, const std::wstring& groupName, 
	std::vector<UserInfo>& userInfo, const std::wstring& conferenceId) {
	UC_LOG(INFO) << "InviteUsersToSingleCall, groupName:" << groupName << ", user count:" << userInfo.size() << ", conferenceId:" << conferenceId;
	GroupService* grpService = (GroupService*)_engine->QueryService(UCGroupService);
	int confId = std::atoi(StringConvert::ToUTF8Str(conferenceId).c_str());
	return grpService->CreateCallGroup(MUTILCHAT_GROUP_TYPE, StringConvert::ToUTF8Str(groupName), userInfo, this, confId, callType);
}

int32_t AudioService::InviteUsersToGroupCall(int callType, std::vector<UserInfo>& userInfo, int groupId, const std::wstring& conferenceId) {
	UC_LOG(INFO) << "InviteUsersToGroupCall, groupId:" << groupId << ", conferenceId:" << conferenceId;
	GroupService* grpService = (GroupService*)_engine->QueryService(UCGroupService);
	int confId = std::atoi(StringConvert::ToUTF8Str(conferenceId).c_str());
	return grpService->AddMemberToGroupCall(groupId, userInfo, confId, callType, this);
}

int32_t AudioService::GetUserCallStatus(std::vector<int32_t>& userIds, int32_t id, int32_t type) {
	UC_LOG(INFO) << "GetUserCallStatus, user count:" << userIds.size();
	HTTPRequest request;
	_GetCallRequestAssist().MakeRequestOfGetUserCallStatus(userIds, request);
	_engine->SendAsyncRequest(request, MakeRequestCallback(this, &AudioService::OnGetCallStatusResponse, id, type));

	return UC_SUCCEEDED;
}

int32_t AudioService::LeaveCall(const std::wstring& tempConferenceId) {
	UC_LOG(INFO) << "LeaveCall, tempConferenceId:" << tempConferenceId;

	//close share
	SharingService* shareService = (SharingService*)_engine->QueryService(UCSharingService);
	shareService->Close();

	Timer::GetInstance().Cancel(_audio_check_timerId);
	//leave conference
	int result = LeaveConference();
	if (result != UC_SUCCEEDED) {
		UC_LOG(ERROR) << "LeaveConference return error:" << result;
		CallInfo info;
		ConferenceWrapper::GetConferenceWrapper().GetCallInfo(info);
		int confId = std::atoi(StringConvert::ToUTF8Str(tempConferenceId).c_str());
		OnCallClose(confId, info.callType, info.callId);
	}
	return UC_SUCCEEDED;
}

CallRequestAssist &AudioService::_GetCallRequestAssist() {
	static CallRequestAssist _callRequestAssist(this, _RetrieveClientType());
	return _callRequestAssist;
}

CallResponseAssist &AudioService::_GetCallResponseAssist() {
	static CallResponseAssist _callResponseAssist;
	return _callResponseAssist;
}

std::string AudioService::_RetrieveServerDomain() {
	ClientService* clientService = (ClientService*)_engine->QueryService(UCClientService);
	return clientService->GetLoginInfo().GetUccServerDomain();
}

int AudioService::RetrieveUserID() {
	ClientService* clientService = (ClientService*)_engine->QueryService(UCClientService);
	return clientService->GetLoginInfo().GetUserId();
}

std::string AudioService::RetrieveSessionID() {
	ClientService* clientService = (ClientService*)_engine->QueryService(UCClientService);
	return clientService->GetLoginInfo().GetSessionId();
}

int AudioService::RetrieveResID() {
	ClientService* clientService = (ClientService*)_engine->QueryService(UCClientService);
	return clientService->GetLoginInfo().GetResId();
}

int AudioService::_RetrieveClientType() {
	ClientService* clientService = (ClientService*)_engine->QueryService(UCClientService);
	return clientService->GetLoginInfo().GetClientType();
}

void AudioService::ChangeGroupCallType(int type) {
	UC_LOG(INFO) << "ChangeGroupCallType type:" << type;
	ConferenceWrapper::GetConferenceWrapper().ChangeGroupCallType(type);
}

int32_t AudioService::InviteCall(CallType type, int32_t conferenceId, UCID uid, UCID privateId) {
	stringstream confid;
	confid << conferenceId;
	int32_t msgId = 0;
	int32_t ret = 0;
	ClientService* clientService = (ClientService*)_engine->QueryService(UCClientService);
	int32_t hostId = clientService->GetCurUCID().GetUserId();
	if (type == SINGLE_CALL) {
		IMService* iMService = (IMService*)_engine->QueryService(UCIMService);
		ret = iMService->SendInviteCallMessage(type, uid, confid.str(), "", hostId, msgId, this);
	}
	else {
		ConversationType conversationType;
		if (type == PROJECT_CALL) {
			conversationType = PROJECT_CHAT;
		} else {
			conversationType = GROUP_CHAT;
		}
		GroupService* grpService = (GroupService*)_engine->QueryService(UCGroupService);
		ret = grpService->SendInviteCallMessage(type, conversationType, uid, confid.str(), "", hostId, msgId, privateId, this);
	}
	return ret;
}

int AudioService::ChangeToGroupCall(const GroupInfo& info, int conferenceId, int callType) {
	UC_LOG(INFO) << "AddToGroupCall enter, groupId:" << info.GetId() << ", name:" << info.GetName();
	HTTPRequest request;
	_GetCallRequestAssist().MakeRequestOfChangeCallType(info.GetId(), conferenceId, request);
	UCID uid;
	uid.SetUserId(info.GetId());
	uid.SetSiteId(info.GetSiteId());
	uid.SetResId(0);
	_engine->SendAsyncRequest(request, MakeRequestCallback(this, &AudioService::OnCallTypeChanged, (CallType)callType, uid, conferenceId));
	return UC_SUCCEEDED;
}

int32_t AudioService::GetHistoryPhoneNum() {
	std::list<string> phoneNums;
	int ret = DBManager::GetInstance().GetHistoryPhoneNum(phoneNums);
	if (ret != UC_SUCCEEDED) {
		UC_LOG(ERROR) << "GetHistoryPhoneNum return error:" << ret;
	}
	triggerEvent(IAudioServiceEvent, OnHistoryPhoneNumGot(UC_SUCCEEDED, phoneNums));
	return UC_SUCCEEDED;
}

int32_t AudioService::GetLastAudioInvite(int32_t userId, int32_t requestId) {
	UC_LOG(INFO) << "GetLastAudioInvite enter, userId:" << userId;
	HTTPRequest request;
	_GetCallRequestAssist().MakeRequestOfGetLastAudioInvite(userId, request);
	_engine->SendAsyncRequest(request, MakeRequestCallback(this, &AudioService::OnLastAudioInviteGot, userId, requestId));
	return UC_SUCCEEDED;
}

//////////////////////////////////////////////////////////////////////////
//callback
//////////////////////////////////////////////////////////////////////////
int AudioService::OnCallGroupCreated(int32_t result, const GroupInfo& info) {
	UC_LOG(INFO) << "OnCallGroupCreated result:" << result << ", id:" << info.GetId() 
		<< ", name:" << info.GetName();
	if (result != UC_SUCCEEDED) {
		UC_LOG(ERROR) << "create group return error:" << result;
		triggerEvent(IAudioServiceEvent, OnCallCreated(result, info.GetGroupType(), 0, info.GetId()));
		return result;
	}
	return CreateCallByGroupInfo(info);
}

int AudioService::OnCallGroupCreated(int32_t result, const GroupInfo& info, int conferenceId, int callType) {
	UC_LOG(INFO) << "OnCallGroupCreated result:" << result << ", id:" << info.GetId()
		<< ", name:" << info.GetName() << ", conferenceId:" << conferenceId;
	if (result != UC_SUCCEEDED) {
		UC_LOG(ERROR) << "create group return error:" << result << ", groupId:" << info.GetId() << ", conferenceId:" << conferenceId;
		triggerEvent(IAudioServiceEvent, OnAddMemberToSingleCall(result, callType, conferenceId, info.GetId()));
		return result;
	}
	return ChangeToGroupCall(info, conferenceId, callType);
}

int32_t AudioService::OnCallCreated(int32_t errorCode, HTTPResponse* response, CallType callType, UCID uid) {
	UC_LOG(INFO) << "callback:OnCallCreated, errorCode:" << errorCode 
		<< ", callType:" << callType << ", uid:" << uid.GetUserId()
		<< ", response:" << (response != NULL ? response->GetContent() : "");
	int32_t result = _GetCallResponseAssist().GuardStatementForCallBack(errorCode, response);
	if (result != UC_SUCCEEDED) {
		UC_LOG(ERROR) << "GuardStatementForCallBack return error:" << result;
		triggerEvent(IAudioServiceEvent, OnCallCreated(result, (int)callType, 0, uid.GetUserId()));
		return result;
	}

	int tangConferenceId = 0;
	int tangUserId = 0;
	string token = "";
	_GetCallResponseAssist().RetrieveConfrenceIdAndReserveCode(tangConferenceId);
	_GetCallResponseAssist().RetrieveUserIdAndToken(tangUserId, token);
	UC_LOG(INFO) << "OnCallCreated token:" << token;

	//join conference
	CallOnMainThread(this, &AudioService::JoinConference, 
		token, callType, Create_Call, tangConferenceId, uid, tangUserId);

	return UC_SUCCEEDED;
}

int32_t AudioService::OnCallJoined(int32_t errorCode, HTTPResponse* response, CallType callType, int tempConferenceId, int64_t groupId) {
	UC_LOG(INFO) << "callback: OnCallJoined, errorCode:" << errorCode 
		<< ", tempConferenceId:" << tempConferenceId << ", groupId:" << groupId
		<< ", response:" << (response != NULL ? response->GetContent() : "");
	int32_t result = _GetCallResponseAssist().GuardStatementForCallBack(errorCode, response);
	if (UC_SUCCEEDED != result)
	{
		UC_LOG(ERROR) << "GuardStatementForCallBack return error:" << result << ", tempConferenceId:" << tempConferenceId;
		triggerEvent(IAudioServiceEvent, OnCallJoined(result, (int)callType, tempConferenceId, groupId));
		return result;
	}

	string token = "";
	int tangUserId = 0;
	_GetCallResponseAssist().RetrieveUserIdAndToken(tangUserId, token);
	UC_LOG(INFO) << "OnCallJoined token:" << token;
	if (token.empty()) {
		triggerEvent(IAudioServiceEvent, OnCallJoined(-1, (int)callType, tempConferenceId, groupId));
		return UC_FAILED;
	}

	//join conference
	UCID uid;
	uid.SetUserId(groupId);
	CallOnMainThread(this, &AudioService::JoinConference,
		token, callType, Join_Call, tempConferenceId, uid, tangUserId);
	return UC_SUCCEEDED;
}

int32_t AudioService::OnCallTypeChanged(int32_t errorCode, HTTPResponse* response, CallType callType, UCID uid, int conferenceId) {
	UC_LOG(INFO) << "callback: OnCallTypeChanged, errorCode:" << errorCode
		<< ", callType:" << callType << ", uid:" << uid.GetUserId()
		<< ", response:" << (response != NULL ? response->GetContent() : "");
	int32_t result = UC_SUCCEEDED;
	do {
		result = _GetCallResponseAssist().GuardStatementForCallBack(errorCode, response);
		if (result != UC_SUCCEEDED) {
			UC_LOG(ERROR) << "GuardStatementForCallBack return error:" << result << ", conferenceId:" << conferenceId;
			break;
		}

		result = InviteCall(callType, conferenceId, uid);
		if (result != UC_SUCCEEDED) {
			UC_LOG(ERROR) << "InviteCall return error" << result << ", conferenceId:" << conferenceId << ", uid:" << uid.GetUserId();
		}
	} while (false);

	triggerEvent(IAudioServiceEvent, OnAddMemberToSingleCall(result, callType, conferenceId, uid.GetUserId()));

	return result;
}

int32_t AudioService::OnGetCallStatusResponse(int32_t errorCode, HTTPResponse* response, int32_t id, int32_t type) {
	UC_LOG(INFO) << "callback: OnGetCallStatusResponse, errorCode:" << errorCode
		<< ", response:" << (response != NULL ? response->GetContent() : "");
	vector<UserCallStatus> userStatus;
	int32_t result = _GetCallResponseAssist().GuardStatementForCallBack(errorCode, response);
	if (result != UC_SUCCEEDED) {
		UC_LOG(ERROR) << "GuardStatementForCallBack return error:" << result;
		triggerEvent(IAudioServiceEvent, OnUserCallStatusGot(result, userStatus, id, type));
		return result;
	}
	_GetCallResponseAssist().RetrieveUserStatus(userStatus);
	triggerEvent(IAudioServiceEvent, OnUserCallStatusGot(0, userStatus, id, type));

	return result;
}

int32_t AudioService::OnLastAudioInviteGot(int32_t errorCode, HTTPResponse* response, int32_t userId, int32_t requestId) {
	UC_LOG(INFO) << "callback: OnLastAudioInviteGot, errorCode:" << errorCode
		<< ", response:" << (response != NULL ? response->GetContent() : "") << ", userId:" << userId;
	int32_t confId = 0;
	int32_t from = 0;
	int32_t to = 0;
	int32_t type = 0;
	int32_t result = 0;
	do {
		result = _GetCallResponseAssist().GuardStatementForCallBack(errorCode, response);
		if (result != UC_SUCCEEDED) {
			UC_LOG(ERROR) << "OnLastAudioInviteGot return error:" << result;
			break;
		}

		_GetCallResponseAssist().RetrieveCallInfo(confId, from, to, type);
	} while (false);
	triggerEvent(IAudioServiceEvent, OnLastAudioInviteGot(result, confId, from, to, type, requestId));

	return result;
}

int32_t AudioService::OnCallLeaved(int32_t errorCode, HTTPResponse* response, int tempConferenceId, int callType, int userId, bool neednotify) {
	UC_LOG(INFO) << "callback: OnCallLeaved, errorCode:" << errorCode 
		<< ", tempConferenceId:" << tempConferenceId << ", callType:" << callType << ", userId:" << userId
		<< ", neednotify:" << neednotify
		<< ", response:" << (response != NULL ? response->GetContent() : "");
	int32_t result = _GetCallResponseAssist().GuardStatementForCallBack(errorCode, response);
	if (result != UC_SUCCEEDED) {
		UC_LOG(ERROR) << "GuardStatementForCallBack return error:" << result << ", tempConferenceId:" << tempConferenceId;
	}
	if (neednotify) {
		triggerEvent(IAudioServiceEvent, OnCallLeaved(result, tempConferenceId, callType, userId));
	}
	return result;
}

void AudioService::OnCallClose(int conferenceId, int callType, int userId, bool neednotify) {
	CallInfo callInfo;
	ConferenceWrapper::GetConferenceWrapper().GetCallInfo(callInfo);
	UC_LOG(INFO) << "OnCallClose, conferenceId:" << conferenceId 
		<< ", tempUserId:" << callInfo.tangUserId << ", userId:" << userId << ", neednotify:" << neednotify;
	HTTPRequest request;
	_GetCallRequestAssist().MakeRequestOfLeaveCall(conferenceId, callInfo.tangUserId, request);
	_engine->SendAsyncRequest(request, MakeRequestCallback(this, &AudioService::OnCallLeaved, conferenceId, callType, userId, neednotify));
}

int AudioService::OnAddMemberToGroupCall(int32_t result, 
	int32_t groupId, int32_t conferenceId, int callType, const std::vector<UserInfo>& member) {
	UCID uid;
	do {
		if (result != UC_SUCCEEDED) {
			UC_LOG(ERROR) << "OnAddMemberToGroupCall, result:" << result << ", groupid:" << groupId 
				<< ", conferenceId:" << conferenceId << ", member count:" << member.size();
			break;
		}

		GroupInfo info;
		GroupService* grpService = (GroupService*)_engine->QueryService(UCGroupService);
		result = grpService->GetGroupInfo(groupId, info);
		if (result != UC_SUCCEEDED && result != UC_LOCALDATA_SUCCESSED) {
			UC_LOG(INFO) << "GetGroupInfo return error:" << result << ", groupId:" << groupId;
			break;
		}

		uid.SetUserId(info.GetId());
		uid.SetSiteId(info.GetSiteId());
		uid.SetResId(0);

		for (std::vector<UserInfo>::const_iterator itor = member.begin();
			itor != member.end(); ++itor) {
			UC_LOG(INFO) << "InviteCall, privateId, userId:"
				<< (*itor).ucid.GetUserId() << ", resId:" << (*itor).ucid.GetResId()
				<< ", siteId:" << (*itor).ucid.GetSiteId();
			result = InviteCall((CallType)callType, conferenceId, uid, (*itor).ucid);
			if (result != UC_SUCCEEDED) {
				UC_LOG(ERROR) << "InviteCall return error" << result
					<< ", conferenceId:" << conferenceId << ", userId:" << uid.GetUserId();
			}
		}
	} while (false);

	triggerEvent(IAudioServiceEvent, OnAddMemberToGroupCall(result, callType, conferenceId, uid.GetUserId()));
	
	return result;
}

int AudioService::OnInviteCallSend(int32_t errorCode, int32_t userId, int32_t conferenceId, int32_t callType, int32_t chatType) {
	if (errorCode != UC_SUCCEEDED) {
		UC_LOG(ERROR) << "OnCallInvited, errorCode:" << errorCode
			<< ", userId:" << userId << ", conferenceId:" << conferenceId
			<< ", callType:" << callType << ", chatType:" << chatType;
		triggerEvent(IAudioServiceEvent, OnInviteCallSend(errorCode, userId, conferenceId, callType, chatType));
	}
	return errorCode;
}

void AudioService::OnVoipNetworkQuality(int quality) {
	CallInfo callInfo;
	ConferenceWrapper::GetConferenceWrapper().GetCallInfo(callInfo);
	int curUserId = RetrieveUserID();
	UC_LOG(INFO) << "OnVoipNetworkQuality, conferenceId:" << callInfo.conferenceId << ", curUserId:" << curUserId;
	triggerEvent(IAudioServiceEvent, OnNetworkQualityChanged(callInfo.conferenceId, callInfo.callType, curUserId, quality));
}

void AudioService::OnMicrophoneStatusChanged(int type, int value) {
	UC_LOG(INFO) << "OnMicrophoneStatusChanged, type:" << type << ", value:" << value;
	triggerEvent(IAudioServiceEvent, OnMicrophoneStatusChanged(type, value));
}

void AudioService::OnSpeakerStatusChanged(int type, int value) {
	UC_LOG(INFO) << "OnSpeakerStatusChanged, type:" << type << ", value:" << value;
	triggerEvent(IAudioServiceEvent, OnSpeakerStatusChanged(type, value));
}

void AudioService::OnSpeakingStatusChanged(int conferenceId, int callType, int userId, int status) {
	UC_LOG(INFO) << "OnSpeakingStatusChanged, conferenceId:" << conferenceId
		<< ", callType:" << callType << ", userId:" << userId << ", isSpeaking:" << status;
	triggerEvent(IAudioServiceEvent, OnSpeakingStatusChanged(conferenceId, callType, userId, status));
}

void AudioService::OnAudioServiceReady(int status) {
	UC_LOG(INFO) << "OnAudioServiceReady, status:" << status;
	triggerEvent(IAudioServiceEvent, OnAudioServiceReady(status));
}

void AudioService::JoinConference(const std::string &joinKey, CallType callType, JoinType JoinType, 
	int32_t conferenceId, UCID uid, int32_t tangUserId) {
	_isConferenceDisconnect = false;
	int ret = ConferenceWrapper::GetConferenceWrapper().JoinConference(joinKey, callType, JoinType, conferenceId, uid, tangUserId);
	UC_LOG(INFO) << "JoinConference return " << ret << ", token:" << joinKey 
		<< ", callType:" << callType << ", JoinType:" << JoinType
		<< ", conferenceId:" << conferenceId << ", userId:" << uid.GetUserId() << ", tangUserId:" << tangUserId;
	if (ret != UC_SUCCEEDED) {
		if (JoinType == Create_Call) {
			triggerEvent(IAudioServiceEvent, OnCallCreated(ret, (int)callType, conferenceId, uid.GetUserId()));
		}
		else {
			triggerEvent(IAudioServiceEvent, OnCallJoined(ret, (int)callType, conferenceId, uid.GetUserId()));
		}
		OnCallClose(conferenceId, callType, uid.GetUserId(), false);
	}
}

int AudioService::LeaveConference() {
	return ConferenceWrapper::GetConferenceWrapper().LeaveConference();
}

int AudioService::EndConference() {
	UC_LOG(INFO) << "EndConference!";
	SharingService* shareService = (SharingService*)_engine->QueryService(UCSharingService);
	shareService->Close();
	Timer::GetInstance().Cancel(_audio_check_timerId);
	return ConferenceWrapper::GetConferenceWrapper().EndConference();
}

int AudioService::ReconnectConference() {
	return ConferenceWrapper::GetConferenceWrapper().ReconnectConference();
}

int AudioService::SwitchToPhone(const std::string &phoneNumber) {
	int ret = DBManager::GetInstance().SaveCallPhoneNum(phoneNumber);
	if (ret != UC_SUCCEEDED) {
		UC_LOG(ERROR) << "SaveCallPhoneNum return error:" << ret << ", phoneNumber:" << phoneNumber;
	}
	return ConferenceWrapper::GetConferenceWrapper().SwitchToPhone(phoneNumber);
}

int AudioService::SwitchToVOIP() {
	return ConferenceWrapper::GetConferenceWrapper().SwitchToVOIP();
}

int AudioService::MuteSelf() {
	return ConferenceWrapper::GetConferenceWrapper().MuteSelf();
}

int AudioService::UnMuteSelf() {
	return ConferenceWrapper::GetConferenceWrapper().UnMuteSelf();
}

int AudioService::HangupPhone() {
	return ConferenceWrapper::GetConferenceWrapper().HangupPhone();
}

int AudioService::EnableLoudSpeaker() {
	return UC_SUCCEEDED;
}

int AudioService::DisableLoudSpeaker() {
	return UC_SUCCEEDED;
}

int32_t AudioService::GetSpeakerNum() {
	return ConferenceWrapper::GetConferenceWrapper().GetSpeakerNum();
}

int32_t AudioService::GetMicrophoneNum() {
	return ConferenceWrapper::GetConferenceWrapper().GetMicrophoneNum();
}

std::wstring AudioService::GetSpeakerList() {
	std::wstring devList;
	_speakerList.clear();
	int ret = ConferenceWrapper::GetConferenceWrapper().GetSpeakerList(_speakerList);
	if (ret != UC_SUCCEEDED) {
		UC_LOG(ERROR) << "GetSpeakerList return error:" << ret;
		return devList;
	}

	devList = StringConvert::FromUTF8Str(MakeParametersForAvDevList(_speakerList));
	UC_LOG(INFO) << "GetSpeakerList, devList:" << devList;
	return devList;
}

std::wstring AudioService::GetMicrophoneList() {
	std::wstring devList;
	_microphoneList.clear();
	int ret = ConferenceWrapper::GetConferenceWrapper().GetMicrophoneList(_microphoneList);
	if (ret != UC_SUCCEEDED) {
		UC_LOG(ERROR) << "GetMicrophoneList return error:" << ret;
		return devList;
	}

	devList = StringConvert::FromUTF8Str(MakeParametersForAvDevList(_microphoneList));
	UC_LOG(INFO) << "GetMicrophoneList, devList:" << devList;
	return devList;
}

int32_t AudioService::StartSpeakerDiagnosis(const std::wstring& speaker) {
	int index = GetIndexByName(StringConvert::ToUTF8Str(speaker), _speakerList);
	int ret = ConferenceWrapper::GetConferenceWrapper().SpeakerDiagnosis(index);
	if (ret != 0) {
		UC_LOG(ERROR) << "SpeakerDiagnosis error!";
		return ret;
	}

	static int interval = 500; //500ms
	Timer::GetInstance().Cancel(_speakerLevel_timerId);
	Timer::GetInstance().Schedule(MakeCallback(this, &AudioService::GetSpeakerLevelProc, speaker), interval, interval, &_speakerLevel_timerId);
	return UC_SUCCEEDED;
}

std::string AudioService::MakeParametersForAvDevList(const std::vector<std::string>& devList) {
	std::stringstream stream;
	stream << "[";
	for (std::vector<string>::const_iterator itor = devList.begin();
		itor != devList.end(); ++itor) {
		if (itor != devList.begin())
			stream << ",";
		stream << "\"" << *itor << "\"";
	}
	stream << "]";
	return stream.str();
}

void AudioService::GetSpeakerLevelProc(const std::wstring& speaker) {
	bool isPlay = false;
	if ( (ConferenceWrapper::GetConferenceWrapper().IsPlayFile(isPlay) == 0) 
		&& !isPlay) {
		Timer::GetInstance().Cancel(_speakerLevel_timerId);
		triggerEvent(IAudioServiceEvent, OnSpeakerPlayFinished(speaker));
		return;
	}

	int level = 0; //取值范围：0~32768
	int ret = ConferenceWrapper::GetConferenceWrapper().GetSpeakerLevel(level);
	if (ret != 0) {
		UC_LOG(ERROR) << "GetSpeakerLevel return error:" << ret;
		return;
	}
	triggerEvent(IAudioServiceEvent, OnSpeakerLevelGot(speaker, level));
}

int32_t AudioService::StartMikeDiagnosis(const std::wstring& mike) {
	int index = GetIndexByName(StringConvert::ToUTF8Str(mike), _microphoneList);
	int ret = ConferenceWrapper::GetConferenceWrapper().MikeDiagnosis(index);
	if (ret != 0) {
		UC_LOG(ERROR) << "MikeDiagnosis error!";
		return ret;
	}

	static int interval = 500; //500ms
	Timer::GetInstance().Cancel(_mikeLevel_timerId);
	Timer::GetInstance().Schedule(MakeCallback(this, &AudioService::GetMikeLevelProc, mike), interval, interval, &_mikeLevel_timerId);
	return UC_SUCCEEDED;
}

void AudioService::GetMikeLevelProc(const std::wstring& mike) {
	int level = 0; //取值范围：0~32768
	int ret = ConferenceWrapper::GetConferenceWrapper().GetMicrophoneLevel(level);
	if (ret != 0) {
		UC_LOG(ERROR) << "GetMikeLevelProc return error:" << ret;
		return;
	}
	triggerEvent(IAudioServiceEvent, OnMikeLevelGot(mike, level));
}

int AudioService::GetIndexByName(const std::string& name, const std::vector<string>& avDevList) {
	int index = 0;
	for (std::vector<string>::const_iterator itor = avDevList.begin();
		itor != avDevList.end(); ++itor) {
		if (*itor == name) {
			return index;
		}
		index++;
	}
	return -1;
}

int32_t AudioService::StopSpeakerDiagnosis() {
	int ret = 0;
	Timer::GetInstance().Cancel(_speakerLevel_timerId);
	bool isPlay = false;
	if ( (ConferenceWrapper::GetConferenceWrapper().IsPlayFile(isPlay) == 0) 
		&& isPlay ) {
		ret = ConferenceWrapper::GetConferenceWrapper().StopPlayFile();
	}
	return ret;
}

int32_t AudioService::StopMikeDiagnosis() {
	Timer::GetInstance().Cancel(_mikeLevel_timerId);
	return ConferenceWrapper::GetConferenceWrapper().StopLoopAudio();
}

int32_t AudioService::GetCameraNum() {
	return ConferenceWrapper::GetConferenceWrapper().GetCameraNum();
}

std::wstring AudioService::GetCameraList() {
	std::wstring cameraList;
	_cameraList.clear();
	int ret = ConferenceWrapper::GetConferenceWrapper().GetCameraList(_cameraList);
	if (ret != UC_SUCCEEDED) {
		UC_LOG(ERROR) << "GetCameraList return error:" << ret;
		return cameraList;
	}

	cameraList = StringConvert::FromUTF8Str(MakeParametersForAvDevList(_cameraList));
	UC_LOG(INFO) << "GetCameraList, cameraList:" << cameraList;
	return cameraList;
}

bool AudioService::IsCameraAvailable() {
	return ConferenceWrapper::GetConferenceWrapper().IsCameraAvailable();
}

int32_t AudioService::CameraDiagnosis(const std::wstring& camera, void *pWindow) {
	int index = GetIndexByName(StringConvert::ToUTF8Str(camera), _cameraList);
	return ConferenceWrapper::GetConferenceWrapper().CameraDiagnosis(index, pWindow);
}

int32_t AudioService::StopCameraDiagnosis() {
	return ConferenceWrapper::GetConferenceWrapper().StopCameraDiagnosis();
}

void AudioService::OnConferenceJoined(int statusCode, int callType, int joinType, int conferenceId, int userId) {
	UC_LOG(INFO) << "OnConferenceJoined, statusCode:" << statusCode << ", conferenceId:" << conferenceId << ", joinType:" << joinType;
	if (statusCode == 0)
		return;
	if (joinType == Create_Call) {
		triggerEvent(IAudioServiceEvent, OnCallCreated(statusCode, callType, conferenceId, userId));
	} else {
		triggerEvent(IAudioServiceEvent, OnCallJoined(statusCode, callType, conferenceId, userId));
	}
	OnCallClose(conferenceId, callType, userId, false);
}

void AudioService::OnConferenceEnded(int conferenceId, int callType, int statusCode) {
	triggerEvent(IAudioServiceEvent, OnConferenceEnded(conferenceId, callType, statusCode));
	OnCallClose(conferenceId, -1, -1, false);
}

void AudioService::OnConferenceDisconnected(int conferenceId) {
	_isConferenceDisconnect = true;
	UC_LOG(INFO) << "OnConferenceDisconnected, conferenceId:" << conferenceId;
	triggerEvent(IAudioServiceEvent, OnConferenceDisconnected(conferenceId));
}

void AudioService::OnUserAdded(const ITangUser *tangUser, int conferenceId, int callType) {
	triggerEvent(IAudioServiceEvent, OnUserAdded(tangUser, conferenceId, callType));
}

void AudioService::OnUserRemoved(const ITangUser *tangUser, int conferenceId, int callType) {
	if (ConferenceWrapper::GetConferenceWrapper().GetParticipantNum() <= 1 && !_isConferenceDisconnect && callType == SINGLE_CALL) {
		UC_LOG(WARNING) << "Need to end conference!";
		EndConference();
	}
	else {
		triggerEvent(IAudioServiceEvent, OnUserRemoved(tangUser, conferenceId, callType));
	}
}

void AudioService::OnUserPropertyChanged(const ITangUser *tangUser, int properType, int conferenceId, int callType) {
	triggerEvent(IAudioServiceEvent, OnUserPropertyChanged(tangUser, properType, conferenceId, callType));
}

void AudioService::OnConferenceReconnected(int statusCode, int conferenceId) {
	_isConferenceDisconnect = false;
	triggerEvent(IAudioServiceEvent, OnConferenceReconnected(statusCode, conferenceId));
}

void AudioService::OnCallPhoneFailed(const std::string &phoneNumber, const std::string &errorCode, int conferenceId) {
	triggerEvent(IAudioServiceEvent, OnCallPhoneFailed(phoneNumber, errorCode, conferenceId));
}

void AudioService::OnAudioStarted(int statusCode, int callType, int joinType, int conferenceId, UCID uid) {
	//create call(join call) event call back
	if (joinType == Create_Call) {
		triggerEvent(IAudioServiceEvent, OnCallCreated(statusCode, callType, conferenceId, uid.GetUserId()));
		if (statusCode == 0) {
			int result = InviteCall((CallType)callType, conferenceId, uid);
			if (result != UC_SUCCEEDED) {
				UC_LOG(ERROR) << "OnAudioStarted InviteCall return error" << result;
				triggerEvent(IAudioServiceEvent, OnCallCreated(result, callType, conferenceId, uid.GetUserId()));
				return;
			}
		}
	}
	else {
		triggerEvent(IAudioServiceEvent, OnCallJoined(statusCode, callType, conferenceId, uid.GetUserId()));
	}

	if (statusCode == 0) {
		UC_LOG(WARNING) << "Schedule _audio_check_timer!";
		static int interval = 30000; //30s
		Timer::GetInstance().Cancel(_audio_check_timerId);
		_is_audio_device_ready = GetAudioDeviceStatus();
		Timer::GetInstance().Schedule(MakeCallback(this, &AudioService::GetAudioStatusProc), interval, interval, &_audio_check_timerId);
	}
	else {
		OnCallClose(conferenceId, callType, uid.GetUserId(), false);
	}
}

bool AudioService::GetAudioDeviceStatus() {
	return (GetMicrophoneNum() > 0) && (GetSpeakerNum() > 0);
}

void AudioService::GetAudioStatusProc() {
	UC_LOG(INFO) << "GetAudioStatusProc, _audio_check_timerId:" << _audio_check_timerId;
	if (GetAudioDeviceStatus()) {
		if (!_is_audio_device_ready) {
			_is_audio_device_ready = true;
			triggerEvent(IAudioServiceEvent, OnAudioDeviceStatusChanged(_is_audio_device_ready));
		}
	}
	else {
		if (_is_audio_device_ready) {
			_is_audio_device_ready = false;
			triggerEvent(IAudioServiceEvent, OnAudioDeviceStatusChanged(_is_audio_device_ready));
		}
	}
}

void AudioService::OnLoudSpeakerStatusChanged(bool isLoud) {
	//免提状态 true: 打开； false: 关闭 (windows端不支持)
}

} /* namespace uc */
