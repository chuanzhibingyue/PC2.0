/*
 * AudioService.h
 *
 *  Created on: May 6, 2015
 *      Author: ethan
 */

#ifndef AUDIOSERVICE_H_
#define AUDIOSERVICE_H_

#include "IAudioService.h"
#define BOOST_SPIRIT_THREADSAFE
#include <boost/property_tree/ptree.hpp>
#include "CallDef.h"
#include "IConferenceEvent.h"
#include "ConferenceLibraryAssist.h"
#include "conferencesdk/ConferenceWrapper.h"
#include "client_service/ClientService.h"

namespace uc {

using namespace boost::property_tree;
class CallRequestAssist;
class CallResponseAssist;
class HTTPResponse;
class ConferenceWrapper;

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
TypeName(const TypeName&);              \
void operator=(const TypeName&)

class AudioService : public IAudioService, IConferenceEvent {
public:
	AudioService(IUCEngine *engine);
	virtual ~AudioService();

	//服务器接口
	virtual int32_t CreateCallById(UCID id, int32_t type);
	virtual int32_t CreateCallByUserList(const std::wstring& groupName, std::vector<UserInfo>& userInfo);
	virtual int32_t JoinCall(int callType, const std::wstring& tempConferenceId, int64_t groupId = 0);
	virtual int32_t LeaveCall(const std::wstring& tempConferenceId);
	virtual int32_t InviteUsersToSingleCall(int callType, const std::wstring& groupName, std::vector<UserInfo>& userInfo, const std::wstring& conferenceId);
	virtual int32_t InviteUsersToGroupCall(int callType, std::vector<UserInfo>& userInfo, int groupId, const std::wstring& conferenceId);
	virtual int32_t GetUserCallStatus(std::vector<int32_t>& userIds, int32_t id, int32_t type);
	virtual int32_t GetHistoryPhoneNum();
	virtual int32_t GetLastAudioInvite(int32_t userId, int32_t requestId);

	//涉及SDK的接口
	virtual int EndConference();
	virtual int ReconnectConference();
	virtual int SwitchToPhone(const std::string &phoneNumber);
	virtual int SwitchToVOIP();
	virtual int MuteSelf();
	virtual int UnMuteSelf();
	virtual int HangupPhone();

	//音频检测
	virtual int32_t GetSpeakerNum();
	virtual int32_t GetMicrophoneNum();
	virtual std::wstring GetSpeakerList();
	virtual std::wstring GetMicrophoneList();
	virtual int32_t StartSpeakerDiagnosis(const std::wstring& speaker);
	virtual int32_t StartMikeDiagnosis(const std::wstring& mike);
	virtual int32_t StopSpeakerDiagnosis();
	virtual int32_t StopMikeDiagnosis();

	//视频检测
	virtual int32_t GetCameraNum();
	virtual std::wstring GetCameraList();
	virtual bool IsCameraAvailable();
	virtual int32_t CameraDiagnosis(const std::wstring& camera, void *pWindow);
	virtual int32_t StopCameraDiagnosis();

	virtual int EnableLoudSpeaker();
	virtual int DisableLoudSpeaker();

	//呼讨论组创建回调
	int OnCallGroupCreated(int32_t result, const GroupInfo& info);
	int OnCallGroupCreated(int32_t result, const GroupInfo& info, int conferenceId, int callType);

	//呼讨论组添加用户回调
	int OnAddMemberToGroupCall(int32_t result, int32_t groupId, int32_t conferenceId, int callType, const std::vector<UserInfo>& member);

	int OnInviteCallSend(int32_t errorCode, int32_t userId, int32_t conferenceId, int32_t callType, int32_t chatType);

	//login user info
	std::string _RetrieveServerDomain();
	int RetrieveUserID();
	std::string RetrieveSessionID();
	int RetrieveResID();

	void ChangeGroupCallType(int type);

private:
	DISALLOW_COPY_AND_ASSIGN(AudioService);
	//服务器接口辅助方法
	CallRequestAssist &_GetCallRequestAssist();
	CallResponseAssist &_GetCallResponseAssist();
	int _RetrieveClientType();

	int CreateCallByGroupInfo(const GroupInfo& info);

	void JoinConference(const std::string &joinKey, CallType callType, JoinType JoinType, int32_t conferenceId, UCID uid, int32_t tangUserId);
	int LeaveConference();

	int ChangeToGroupCall(const GroupInfo& info, int conferenceId, int callType);

protected:
	//callback
	int32_t OnCallCreated(int32_t errorCode, HTTPResponse* response, CallType callType, UCID uid);
	int32_t OnCallJoined(int32_t errorCode, HTTPResponse* response, CallType callType, int tempConferenceId, int64_t groupId);
	int32_t OnCallLeaved(int32_t errorCode, HTTPResponse* response, int tempConferenceId, int callType, int userId, bool neednotify = true);
	int32_t OnCallTypeChanged(int32_t errorCode, HTTPResponse* response, CallType callType, UCID uid, int conferenceId);
	int32_t OnGetCallStatusResponse(int32_t errorCode, HTTPResponse* response, int32_t id, int32_t type);
	int32_t OnLastAudioInviteGot(int32_t errorCode, HTTPResponse* response, int32_t userId, int32_t requestId);

	void GetSpeakerLevelProc(const std::wstring& speaker);
	void GetMikeLevelProc(const std::wstring& mike);
	void GetAudioStatusProc();

	//涉及SDK的回调函数
	virtual void OnConferenceJoined(int statusCode, int createType, int joinType, int conferenceId, int userId);
	virtual void OnConferenceEnded(int conferenceId, int callType, int statusCode);
	virtual void OnUserAdded(const ITangUser *tangUser, int conferenceId, int callType);
	virtual void OnUserRemoved(const ITangUser *tangUser, int conferenceId, int callType);
	virtual void OnUserPropertyChanged(const ITangUser *tangUser, int properType, int conferenceId, int callType);
	virtual void OnConferenceDisconnected(int conferenceId);
	virtual void OnConferenceReconnected(int statusCode, int conferenceId);
	virtual void OnCallPhoneFailed(const std::string &phoneNumber, const std::string &errorCode, int conferenceId);
	virtual void OnAudioStarted(int statusCode, int callType, int joinType, int conferenceId, UCID uid);
	virtual void OnLoudSpeakerStatusChanged(bool isLoud);
	virtual void OnCallClose(int conferenceId, int callType, int userId, bool neednotify = true);
	virtual void OnVoipNetworkQuality(int quality);
	virtual void OnMicrophoneStatusChanged(int type, int value);
	virtual void OnSpeakerStatusChanged(int type, int value);
	virtual void OnSpeakingStatusChanged(int conferenceId, int callType, int userId, int status);
	virtual void OnAudioServiceReady(int status);

private:
	int32_t InviteCall(CallType type, int32_t conferenceId, UCID uid, UCID privateId = {-1,-1,-1});
	int GetIndexByName(const std::string& name, const std::vector<string>& avDevList);
	std::string MakeParametersForAvDevList(const std::vector<std::string>& speakers);
	bool GetAudioDeviceStatus();

	std::vector<string> _speakerList;
	std::vector<string> _microphoneList;
	std::vector<string> _cameraList;
	int64_t _speakerLevel_timerId;
	int64_t _mikeLevel_timerId;
	int64_t _audio_check_timerId;
	bool _is_audio_device_ready;

	bool _isConferenceDisconnect;
};

} /* namespace uc */

#endif /* AUDIOSERVICE_H_ */
