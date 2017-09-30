/*
 * CalendarServiceStrategy.h
 *
 *  Created on: Aug 19, 2015
 *      Author: hualong zhang
 */

#ifndef AUDIO_SERVICE_STRATEGY_H_
#define AUDIO_SERVICE_STRATEGY_H_

#include "IAudioService.h"
#include "CefClient.h"
#include "assist/AudioServiceJSEventAssist.h"
#include "IGroupService.h"
#include "audio_service/CallDef.h"

namespace ucclient {

using namespace uc;

class AudioServiceStrategy : public IAudioServiceEvent {
public:
	AudioServiceStrategy(IUCEngine *engine, cefIntegration::CefClient *cefClient);

	/**
	* 根据id创建呼
	* @param id 用户ID或讨论组ID
	* @param type Project: 1, Group: 2, Single: 3
	*/
	virtual int32_t CreateCallById(UCID id, int32_t type);

	/**
	* 根据人员列表创建呼
	* @param groupName 讨论组名称
	* @param userInfo  讨论组成员列表
	*/
	virtual int32_t CreateCallByUserList(const std::wstring& groupName, std::vector<UserInfo>& userInfo);

	/**
	* 加入呼
	* @param conferenceId 呼会议ID
	* @param callType Project: 1, Group: 2, Single: 3
	* @param groupId   讨论组ID，缺省为一对一呼
	*/
	virtual int32_t JoinCall(int callType, const std::wstring& conferenceId, int64_t groupId = 0);

	/**
	* 邀请其他用户加入当前一对一call
	* @param callType Project: 1, Group: 2, Single: 3
	* @param groupName 讨论组名称
	* @param userInfo 用户列表
	* @param conferenceId 待加入的会议ID
	*/
	virtual int32_t InviteUsersToSingleCall(int callType, const std::wstring& groupName, std::vector<UserInfo>& userInfo, const std::wstring& conferenceId);

	/**
	* 邀请其他用户加入当前群call
	* @param callType Project: 1, Group: 2, Single: 3,
	* @param userInfo 用户列表
	* @param groupId  待加入群ID
	* @param conferenceId 待加入的会议ID
	*/
	virtual int32_t InviteUsersToGroupCall(int callType, std::vector<UserInfo>& userInfo, int groupId, const std::wstring& conferenceId);

	/**
	* 离开呼
	* @param conferenceId 呼会议ID
	*/
	virtual int32_t LeaveCall(const std::wstring& conferenceId);

	/**
	* @brief 获取指定用户是否在呼中
	* @param userIds 用户ID
	*/
	virtual int32_t GetUserCallStatus(std::vector<int32_t>& userIds, int32_t id, int32_t type);

	/**
	* 呼会议重连操作
	*/
	virtual int32_t ReconnectConference();

	/**
	* @brief 切换到电话
	* @param phoneNumber 电话号码，手机号码：151********；固定电话：0296568****
	*/
	virtual int32_t SwitchToPhone(const std::string &phoneNumber);

	/**
	* @brief 切换到VOIP
	*/
	virtual int32_t SwitchToVOIP();

	/**
	* @brief 自我静音
	*/
	virtual int32_t MuteSelf();

	/**
	* @brief 解除自我静音
	*/
	virtual int32_t UnMuteSelf();

	virtual int32_t HangupPhone();

	/**
	* @brief 获取外呼历史号码
	*/
	virtual int32_t GetHistoryPhoneNum();

	//////////////////////////////////////音频检测///////////////////////////////////////
	/**
	* @brief 获取扬声器数量
	*/
	virtual int32_t GetSpeakerNum();

	/**
	* @brief 获取麦克风数量
	*/
	virtual int32_t GetMicrophoneNum();

	/**
	* @brief 获取扬声器列表
	*/
	virtual std::wstring GetSpeakerList();

	/**
	* @brief 获取麦克风列表
	*/
	virtual std::wstring GetMicrophoneList();

	/**
	* @brief 扬声器诊断
	*/
	virtual int32_t StartSpeakerDiagnosis(const std::wstring& speaker);

	/**
	* @brief 麦克风诊断
	*/
	virtual int32_t StartMikeDiagnosis(const std::wstring& mike);

	/**
	* @brief 停止扬声器诊断
	*/
	virtual int32_t StopSpeakerDiagnosis();

	/**
	* @brief 停止麦克风诊断
	*/
	virtual int32_t StopMikeDiagnosis();

	/**
	* @brief 获取摄像头列表
	*/
	virtual std::wstring GetCameraList();

	/**
	* @brief 摄像头检测
	*/
	virtual int32_t CameraDiagnosis(const std::wstring& camera, void *pWindow);

	/**
	* @brief 停止摄像头检测
	*/
	virtual int32_t StopCameraDiagnosis();

	/**
	* @brief 清除上一次呼信息
	*/
	virtual int32_t GetLastAudioInvite(int32_t userId, int32_t requestId);

	//////////////////////////////////////event 事件回调////////////////////////////////////////////
	virtual void OnCallCreated(int32_t errorCode, int callType, int tempConferenceId, int id);

	virtual void OnCallJoined(int32_t errorCode, int callType, int tempConferenceId, int id);

	virtual void OnCallLeaved(int32_t errorCode, int tempConferenceId, int callType, int userId);

	virtual void OnConferenceJoined(int statusCode, int createType, int joinType, int conferenceId, int userId) {}

	virtual void OnUserAdded(const ITangUser *tangUser, int conferenceId, int callType);

	virtual void OnUserRemoved(const ITangUser *tangUser, int conferenceId, int callType);

	virtual void OnUserPropertyChanged(const ITangUser *tangUser, int properType, int conferenceId, int callType);

	virtual void OnConferenceEnded(int conferenceId, int callType, int statusCode);

	virtual void OnConferenceDisconnected(int conferenceId);

	virtual void OnConferenceReconnected(int statusCode, int conferenceId);
    
	virtual void OnCallPhoneFailed(const std::string &phoneNumber, const std::string &errorCode, int conferenceId);

	virtual void OnAudioStarted(int statusCode, int callType, int joinType, int conferenceId, UCID uid) {}

	virtual void OnLoudSpeakerStatusChanged(bool isLoud) {}

	virtual void OnCallClose(int conferenceId, int callType, int userId, bool neednotify) {}

	virtual void OnVoipNetworkQuality(int quality) {}

	virtual void OnNetworkQualityChanged(int conferenceId, int callType, int userId, int quality);

	virtual void OnMicrophoneStatusChanged(int type, int value);

	virtual void OnSpeakerStatusChanged(int type, int value);

	virtual void OnSpeakingStatusChanged(int conferenceId, int callType, int userId, int status);

	virtual void OnAddMemberToGroupCall(int32_t errorCode, int callType, int tempConferenceId, int id);

	virtual void OnAddMemberToSingleCall(int32_t errorCode, int callType, int tempConferenceId, int id);

	virtual void OnUserCallStatusGot(int32_t errorCode, const vector<UserCallStatus>& userStatus, int32_t id, int32_t type);

	virtual void OnHistoryPhoneNumGot(int32_t errorCode, const std::list<string>& phoneNums);

	virtual void OnSpeakerLevelGot(const std::wstring& speaker, int level);

	virtual void OnMikeLevelGot(const std::wstring& mike, int level);

	virtual void OnSpeakerPlayFinished(const std::wstring& speaker);

	virtual void OnInviteCallSend(int32_t errorCode, int32_t userId, int32_t conferenceId, int callType, int32_t chatType);

	virtual void OnAudioServiceReady(int status);

	virtual void OnAudioDeviceStatusChanged(bool isReady);

	virtual void OnLastAudioInviteGot(int32_t errorCode, int32_t conferenceId, int32_t from, int32_t to, int32_t type, int32_t requestId);

private:
	IAudioService *_audioService;
	IUCEngine *_engine;
	AudioServiceJSEventAssist _jsEventAssist;
};

}

#endif //AUDIO_SERVICE_STRATEGY_H_