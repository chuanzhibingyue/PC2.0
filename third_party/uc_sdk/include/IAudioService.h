/*
* IAudioService.h
*
*  Created on: May 6, 2015
*      Author: ethan
*/

#ifndef IAUDIOSERVICE_H_
#define IAUDIOSERVICE_H_

#include "UCSDK.h"
#include "IUCEngine.h"
#include "ITangUser.h"
#include "IConferenceEvent.h"
#include "IConference.h"
#include "IGroupService.h"
#include "audio_service/CallDef.h"

namespace uc {
	interface UC_API IAudioServiceEvent;

	interface UC_API IAudioService: public IUCService, IConference, Event<IAudioServiceEvent> {
		virtual ~IAudioService() {};

		/**
		* 根据指定ID创建呼
		* @param id			讨论组ID或者用户ID
		* @param type		Project: 1, Group: 2, Single: 3
		*/
		virtual int32_t CreateCallById(UCID id, int32_t type) = 0;

		/**
		* 创建呼
		* @param groupName			讨论组名称
		* @param logourl			讨论组头像地址
		* @param userInfo			讨论组成员列表
		*/
		virtual int32_t CreateCallByUserList(const std::wstring& groupName, std::vector<UserInfo>& userInfo) = 0;

		/**
		* 加入呼
		* @param callType Project: 1, Group: 2, Single: 3
		* @param tempConferenceId	会议ID
		* @param groupId			讨论组ID，如果是在讨论组中加入呼，需要填写该值
		*/
		virtual int32_t JoinCall(int callType, const std::wstring& tempConferenceId, int64_t groupId = 0) = 0;

		/**
		* 退出呼
		* @param tempConferenceId	呼会议ID
		*/
		virtual int32_t LeaveCall(const std::wstring& tempConferenceId) = 0;

		/**
		* 邀请其他用户加入当前一对一call
		* @param callType Project: 1, Group: 2, Single: 3
		* @param groupName 讨论组名称
		* @param userInfo 用户列表
		* @param conferenceId 待加入的会议ID
		*/
		virtual int32_t InviteUsersToSingleCall(int callType, const std::wstring& groupName, std::vector<UserInfo>& userInfo, const std::wstring& conferenceId) = 0;

		/**
		* 邀请其他用户加入当前群call
		* @param callType Project: 1, Group: 2, Single: 3
		* @param userInfo 用户列表
		* @param groupId  待加入群ID
		* @param conferenceId 待加入的会议ID
		*/
		virtual int32_t InviteUsersToGroupCall(int callType, std::vector<UserInfo>& userInfo, int groupId, const std::wstring& conferenceId) = 0;

		/**
		* 获取用户呼状态
		* @param userIds 用户ID
		*/
		virtual int32_t GetUserCallStatus(std::vector<int32_t>& userIds, int32_t id, int32_t type) = 0;

		/**
		* 获取历史呼号码
		*/
		virtual int32_t GetHistoryPhoneNum() = 0;

		/**
		* @brief 清除上一次呼信息
		*/
		virtual int32_t GetLastAudioInvite(int32_t userId, int32_t requestId) = 0;

		/**
		* @brief 获取扬声器数量
		*/
		virtual int32_t GetSpeakerNum() = 0;

		/**
		* @brief 获取麦克风数量
		*/
		virtual int32_t GetMicrophoneNum() = 0;

		/**
		* @brief 获取扬声器列表
		*/
		virtual std::wstring GetSpeakerList() = 0;

		/**
		* @brief 获取麦克风列表
		*/
		virtual std::wstring GetMicrophoneList() = 0;

		/**
		* @brief 扬声器诊断
		*/
		virtual int32_t StartSpeakerDiagnosis(const std::wstring& speaker) = 0;

		/**
		* @brief 麦克风诊断
		*/
		virtual int32_t StartMikeDiagnosis(const std::wstring& mike) = 0;

		/**
		* @brief 停止扬声器诊断
		*/
		virtual int32_t StopSpeakerDiagnosis() = 0;

		/**
		* @brief 停止麦克风诊断
		*/
		virtual int32_t StopMikeDiagnosis() = 0;

		/**
		* @brief 获取摄像头数量
		*/
		virtual int32_t GetCameraNum() = 0;

		/**
		* @brief 获取摄像头列表
		*/
		virtual std::wstring GetCameraList() = 0;

		/**
		* @brief 判断摄像头是否可用
		*/
		virtual bool IsCameraAvailable() = 0;

		/**
		* @brief 启动视频预览
		*/
		virtual int32_t CameraDiagnosis(const std::wstring& camera, void *pWindow) = 0;

		/**
		* @brief 结束视频预览
		*/
		virtual int32_t StopCameraDiagnosis() = 0;
	};

	interface UC_API IAudioServiceEvent : public IConferenceEvent {
		virtual ~IAudioServiceEvent() {};

		/**
		* 创建呼结果的返回事件
		* @param errorCode				错误码
		* @param tempConferenceId		呼会议Id
		* @param tempUserId				呼用户Id
		* @param groupId			    讨论组Id
		*/
		virtual void OnCallCreated(int32_t errorCode, int type, int tempConferenceId, int Id) = 0;

		/**
		* 加入呼结果的返回事件
		* @param errorCode				错误码
		* @param tempConferenceId		呼会议Id
		* @param tempUserId				呼用户Id
		* @param groupId				讨论组Id
		*/
		virtual void OnCallJoined(int32_t errorCode, int tempConferenceId, int tempUserId, int groupId) = 0;

		/**
		* 离开呼结果的返回事件
		* @param errorCode				错误码
		* @param tempConferenceId		呼会议Id
		* @param callType				0:一对一呼 1:群呼
		* @param userId					用户ID或群ID
		*/
		virtual void OnCallLeaved(int32_t errorCode, int tempConferenceId, int callType, int userId) = 0;

		/**
		* VOIP网络质量回调事件
		* @param conferenceId		呼会议Id
		* @param callType			0:一对一呼 1:群呼
		* @param userId				用户ID
		* @param quality			网络质量 0:非常好;1:很好;2:普通;3:差;4:很差
		*/
		virtual void OnNetworkQualityChanged(int conferenceId, int callType, int userId, int quality) = 0;

		/**
		* 麦克风状态变更回调事件
		* @param type				0：level 1：volume
		* @param value			    大小
		*/
		virtual void OnMicrophoneStatusChanged(int type, int value) = 0;

		/**
		* 扬声器状态变更回调事件
		* @param type				0：level 1：volume
		* @param value			    大小
		*/
		virtual void OnSpeakerStatusChanged(int type, int value) = 0;

		/**
		* 音频设备状态变更
		* @param isReady true：由音频设备 false：无音频设备
		*/
		virtual void OnAudioDeviceStatusChanged(bool isReady) = 0;

		/**
		* 邀请用户加入当前群呼回调事件
		*/
		virtual void OnAddMemberToGroupCall(int32_t errorCode, int callType, int tempConferenceId, int id) = 0;

		/**
		* 邀请用户加入当前一对一呼回调事件
		*/
		virtual void OnAddMemberToSingleCall(int32_t errorCode, int callType, int tempConferenceId, int id) = 0;

		/**
		* 获取用户呼状态回调事件
		*/
		virtual void OnUserCallStatusGot(int32_t errorCode, const vector<UserCallStatus>& userStatus, int32_t id, int32_t type) = 0;

		/**
		* 获取用户呼历史电话号码
		*/
		virtual void OnHistoryPhoneNumGot(int32_t errorCode, const std::list<string>& phoneNums) = 0;

		virtual void OnSpeakerLevelGot(const std::wstring& speaker, int level) = 0;

		virtual void OnMikeLevelGot(const std::wstring& mike, int level) = 0;

		virtual void OnSpeakerPlayFinished(const std::wstring& speaker) = 0;

		virtual void OnInviteCallSend(int32_t errorCode, int32_t userId, int32_t conferenceId, int callType, int32_t chatType) = 0;

		virtual void OnLastAudioInviteGot(int32_t errorCode, int32_t conferenceId, int32_t from, int32_t to, int32_t type, int32_t requestId) = 0;
	};
}

#endif /* IAUDIOSERVICE_H_ */
