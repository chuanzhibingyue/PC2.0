/*
 * CalendarServiceStrategy.h
 *
 *  Created on: July 7, 2015
 *      Author: gang wang
 */

#ifndef CALENDAR_SERVICE_STRATEGY_H_
#define CALENDAR_SERVICE_STRATEGY_H_

#include "ICalendarService.h"
#include "CefClient.h"

namespace ucclient {

using namespace uc;

class CalendarServiceStrategy : public ICalendarServiceEvent {
public:
	CalendarServiceStrategy(IUCEngine *engine, cefIntegration::CefClient *cefClient);
	
	int32_t CreateMeeting(const std::vector<Attendee>& attendees, const std::wstring& title, const std::wstring& location,
		const std::wstring& summary, const std::wstring& timeZone, const std::wstring& hostName, const std::wstring extrasInfo, int64_t startTime, int32_t duration,
		int32_t authorizerId, CycleRole& cycleRole, int32_t isGnet, int32_t isCycle, int32_t isWholeDay, int32_t language, int32_t emailLanguage);

	int32_t CreateRemoteAssistance(int32_t userId);

	int32_t UpdateMeeting(int64_t eventId, int64_t oriTime, const std::vector<Attendee>& attendees,
		const std::wstring& title, const std::wstring& location, const std::wstring& summary,
		const std::wstring& timeZone, const std::wstring extrasInfo, int64_t startTime, int32_t duration, int32_t authorizerId,
		CycleRole& cycleRole, int32_t isGnet, int32_t isCycle, int32_t isWholeDay, int32_t language, int32_t emailLanguage);

	int32_t CancelMeeting(int64_t eventId, int64_t startTime, int32_t authorizerId, int32_t language, int32_t emailLanguage);

	int32_t GetMeetingStartURL(const std::wstring& confId, const std::wstring& password, std::string& startURL, int32_t& siteId);

	int32_t GetMeetingWebStartURL(const std::wstring& confId, const std::wstring& mail,
		const std::wstring& password, const std::wstring& displayName);

	int32_t UpdateAccreditSetting(int32_t authorizedPersonId, int32_t status);

	int32_t GetAccreditUserList(OUT std::vector<Accrediting>& infos);

	int32_t GetMeetingList(int64_t startTime, int64_t endTime, int32_t limit, int32_t authorizerId,
		int64_t eventIdFilter, bool filterAsc, int32_t refreshflag, OUT int32_t& requestId, OUT vector<Meeting>& meetings);
	
	int32_t GetBriefMeetingList(int64_t startTime, int64_t endTime, int32_t authorizerId, OUT int32_t& requestId, OUT vector<Meeting>& meetings);

	int32_t GetBriefChangedMeetingList(int64_t startTime, int32_t authorizerId = 0);

	int32_t AcceptMeetingInvitation(int64_t eventId, int64_t startTime = 0, int32_t authorizerId = 0);

	int32_t DenyMeetingInvitation(int64_t eventId, int64_t startTime = 0, int32_t authorizerId = 0);

	int32_t GetMeetingInfoById( int64_t eventId, int64_t startTime, int64_t endTime, 
		int32_t authorizerId, OUT vector<Meeting>& meetings );

	int32_t GetMeetingInfoByIds(const std::vector<int64_t>& eventIds, int64_t startTime, int64_t endTime, int32_t authorizerId,
		OUT int32_t& requestId, OUT vector<Meeting>& meetings );

	int32_t ForwardMeeting(int64_t eventId, const std::vector<Attendee>& attendees,
		int64_t startTime, int32_t authorizerId, int32_t language, int32_t emailLanguage);

	int32_t GetUserSchedule(const std::vector<int32_t>& userIdList, int64_t startTime, 
		int64_t endTime, int32_t timeInterval, int64_t eventIdFilter, OUT int32_t& requestId);

	int32_t GetUserBusyFreeInfo(const std::vector<int32_t>& userIdList, int64_t startTime,
		int64_t endTime, int32_t shareUserId, OUT int32_t& requestId);

	int32_t CreateMinutes(int64_t eventId, int64_t startTime, const std::wstring& title, const std::wstring& minutes);

	int32_t UpdateMinutes(int64_t eventId, int64_t minutesId, const std::wstring& title, const std::wstring& minutes);

	int32_t GetMinutesDetails(int64_t eventId, int64_t minutesId);

	int32_t GetMinutesList(int64_t eventId, int64_t startTime);

	int32_t DeleteMeetingRecording(int64_t eventId, const std::wstring& confId, int64_t startTime);

	int32_t UploadMeetingFile(int64_t eventId, int32_t operation, const std::wstring& fileName, int64_t fileSize,
		const std::wstring& fileURL, int64_t createTime, OUT int32_t& msgId);

	int32_t UploadCloudFile(int64_t eventId, int32_t operation, int32_t mountId,
		const std::wstring& fileName, int64_t fileSize, DocumentCloudType cloudType, const std::wstring& hashCode,
		const std::wstring& detailContent, OUT int32_t& msgId);

	int32_t DeleteMeetingFile(int64_t minutesId);

	int32_t SetMeetingAlarm(int32_t alarmStatus, int32_t alarmTime);
	
	int32_t ResendMessage(int32_t msgId);

	int32_t ForwardMessage(UCID& userUid, int32_t msgId);

	int32_t SendTextMessage(int64_t eventId, int64_t startTime, UCID groupId, int8_t textType, std::wstring& textFormat, std::wstring& textData, OUT int32_t& msgId);

	int32_t SendImageMessage(int64_t eventId, int64_t startTime, UCID groupId, std::wstring& imageName, int32_t imageSize,
		std::wstring& imageURL, std::wstring& imagePath, OUT int32_t& msgId);

	int32_t SendAudioMessage(int64_t eventId, int64_t startTime, UCID groupId, std::wstring& audioName, int32_t audioSize,
		std::wstring& audioURL, int32_t audioDuration, OUT int32_t& msgId);

	int32_t SendVideoMessage(int64_t eventId, int64_t startTime, UCID groupId, std::wstring& videoName, int32_t videoSize,
		std::wstring& videoURL, std::wstring& videoPath, int32_t videoDuration, OUT int32_t& msgId);

	int32_t SendRemotAssistanceInviteMsg(UCID& uid, std::wstring& confId, std::wstring& confPwd, OUT int32_t& msgId);
	int32_t SendRemotAssistanceAcceptMsg(UCID& uid, std::wstring& confId, OUT int32_t& msgId);
	int32_t SendRemotAssistanceRejectMsg(UCID& uid, std::wstring& confId, OUT int32_t& msgId);

	int32_t RevocationMeetingMessage(int64_t groupId, int64_t eventId, int64_t seqId, int32_t& msgId);

	/**
	 * 启动云会议客户端
	 */
	int32_t StartMeetingClient(const std::wstring& params);

	/**
	 * 检查云会议客户端环境
	 */
	int32_t CheckMeetingClient();

	/**
	* 杀死云会议客户端
	*/
	int32_t KillMeetingClient();

	//会议室接口
	int32_t CreateMeetingRoom(
		const std::vector<Attendee>& attendees,
		const std::wstring& title,
		const std::wstring& remarks,
		const std::wstring& password,
		int64_t startTime,
		int64_t endTime,
		int64_t roomHostId,
		int64_t orgId,
		int32_t isSetPassword = 1,
		int32_t defaultAttendsVisible = 1,
		int32_t defaultAttendsJoin = 1);

	int32_t UpdateMeetingRoom(
		std::wstring& roomId,
		const std::vector<Attendee>& attendees,
		const std::wstring& title,
		const std::wstring& remarks,
		const std::wstring& password,
		int64_t startTime,
		int64_t endTime,
		int64_t roomHostId,
		int64_t orgId,
		int32_t isSetPassword = 1,
		int32_t defaultAttendsVisible = 1,
		int32_t defaultAttendsJoin = 1);

	int32_t CreateMeetingInMeetingRoom(
		const std::vector<Attendee>& attendees,
		const std::wstring& roomId,
		const std::wstring& title,
		const std::wstring& summary,
		int64_t startTime,
		int32_t duration);

	int32_t GetMeetingRoomList(int64_t orgId);

	int32_t CheckJoinMeetingRoom(const std::wstring& roomId);

	int32_t CheckMeetingRoomPassword(const std::wstring& roomId, const std::wstring& password);

	int32_t DeleteMeetingRoom(const std::wstring& roomId);

	int32_t CheckRightsOfCreateRoom();

	/**
	* 创建会议结果的返回事件
	* @param errorCode				错误码
	* @param meeting				会议信息
	* @return
	*/
	virtual void OnMeetingCreated(int32_t errorCode, Meeting& meeting);

	/**
	* 远程协助创建完成的返回事件
	* @param errorCode				错误码
	* @param confId					会议ID
	* @param passWd					参会人密码
	* @param startURL				主持人主会调起串
	* @param userId					对方ID
	* @return
	*/
	virtual void OnRemotAssistanceCreated(int32_t errorCode, const std::string& confId, const std::string& passWd, const std::string& startURL, int32_t userId);

	/**
	* 更新会议结果的返回事件
	* @param errorCode				错误码
	* @param meeting				会议信息
	*/
	virtual void OnMeetingUpdated(int32_t errorCode, int64_t eventId, int64_t startTime, Meeting& meeting);

	/**
	* 取消会议结果的返回事件
	* @param errorCode				错误码
	* @param conferenceId			会议ID
	* @param eventId				日程事件ID
	* @param startTime				会议开始时间，UTC Unix时间戳
	* @param authorizerId			授权人ID
	*/
	virtual void OnMeetingCanceled(int32_t errorCode, int64_t eventId, int64_t startTime = 0, int32_t authorizerId = 0);

	/**
	* 获取会议调起串结果的返回事件
	* @param errorCode				错误码
	* @param confId					会议ID
	* @param startURL				调起串
	*/
	virtual void OnMeetingStartURLReceived(int32_t errorCode, const std::string& confId, const std::string& startURL);

	/**
	* 通过邮件获取会议调起串结果的返回事件
	* @param errorCode				错误码
	* @param confId					会议ID
	* @param startURL				调起串
	*/
	virtual void OnMeetingWebStartURLReceived(int32_t errorCode, const std::string& confId, const std::string& startURL);

	/**
	* 更新授权调协结果的返回事件
	* @param errorCode				错误码
	* @param errorMsg				错误描述
	* @param authorizedPersonId		授权或取消授权的人的ID
	* @param status					授权状态 1：授权, 0：取消授权
	*/
	virtual void OnAccreditSettingUpdated(int32_t errorCode, int32_t authorizedPersonId, int32_t status);

	/**
	* 获取授权用户列表结果的返回事件
	* @param errorCode				错误码
	* @param accreditUserList		日程共享用户列表
	*/
	virtual void OnAccreditUserListReceived(int32_t errorCode, std::vector<Accrediting>& accreditUserList);

	/**
	* 获取会议列表结果的返回事件
	* @param errorCode				错误码
	* @param requestId				本次请求ID,用于多次调用接口时区分哪一次的返回
	* @param startTime				起始时间
	* @param endTime				终止时间
	* @param eventIdFilter			过滤条件
	* @param authorizerId			授权人ID
	* @param meetings				会议信息列表
	*/
	virtual void OnMeetingListReceived(int32_t errorCode, int32_t requestId, int64_t startTime, int64_t endTime, int64_t eventIdFilter, int32_t authorizerId, std::vector<Meeting>& meetings);

	/**
	* 获取简明会议列表结果的返回事件
	* @param errorCode				错误码
	* @param requestId				本次请求ID,用于多次调用接口时区分哪一次的返回
	* @param startTime				起始时间
	* @param endTime				终止时间
	* @param authorizerId			授权人ID
	* @param meetings				会议信息列表
	*/
	virtual void OnBriefMeetingListReceived(int32_t errorCode, int32_t requestId, int64_t startTime, int64_t endTime, int32_t authorizerId, std::vector<Meeting>& meetings);

	/**
	* 获取变更的简明会议列表结果的返回事件
	* @param errorCode				错误码
	* @param startTime				上次更新时间
	* @param authorizerId			授权人ID
	* @param meetings				会议信息列表
	*/
	virtual void OnBriefChangedMeetingListReceived(int32_t errorCode, int64_t startTime, int32_t authorizerId, std::vector<Meeting>& meetings);

	/**
	* 获取会议详情结果的返回事件
	* @param errorCode				错误码
	* @param eventId				事件ID
	* @param authorizerId			授权人ID
	* @param meetings				会议信息列表
	*/
	virtual void OnMeetingInfoReceivedById(int32_t errorCode, int64_t eventId, int32_t authorizerId, std::vector<Meeting>& meetings);

	/**
	* 批量获取会议详情结果的返回事件
	* @param errorCode				错误码
	* @param requestId				本次请求ID,用于多次调用接口时区分哪一次的返回
	* @param authorizerId			授权人ID
	* @param meetings				会议信息列表
	*/
	virtual void OnMeetingInfoReceivedByIds(int32_t errorCode, int32_t requesetId, int32_t authorizerId, std::vector<Meeting>& meetings);

	/**
	* 接受会议邀请结果的返回事件
	* @param errorCode				错误码
	* @param eventId				事件ID
	* @param startTime				开始时间
	* @param authorizerId			授权人ID
	*/
	virtual void OnMeetingInvitationAccepted(int32_t errorCode, int64_t eventId, int64_t startTime = 0, int32_t authorizerId = 0);

	/**
	* 拒绝会议邀请结果的返回事件
	* @param errorCode				错误码
	* @param eventId				事件ID
	* @param startTime				开始时间
	* @param authorizerId			授权人ID
	*/
	virtual void OnMeetingInvitationDenied(int32_t errorCode, int64_t eventId, int64_t startTime = 0, int32_t authorizerId = 0);

	/**
	* 转发会议邀请结果的返回事件
	* @param errorCode				错误码
	* @param meeting				会议信息
	*/
	virtual void OnMeetingForwarded(int32_t errorCode, Meeting& meeting);

	/**
	* 获取用户日程列表结果的返回事件
	* @param errorCode				错误码
	* @param requestId				本次请求ID,用于多次调用接口时区分哪一次的返回
	* @param schedules				日程信息列表
	*/
	virtual void OnUserScheduleReceived(int32_t errorCode, int32_t requestId, std::vector<UserSchedule>& schedules);

	/**
	* 创建会议纪要结果的返回事件
	* @param errorCode				错误码
	* @param eventId				事件ID
	* @param startTime				会议开始时间
	* @param minutesId				会议纪要ID
	* @param minutesTitle			会议纪要名称
	*/
	virtual void OnMinutesCreated(
		int32_t errorCode,
		int64_t eventId,
		int64_t startTime,
		int64_t MinutesId,
		const std::string& minutesTitle
		);

	/**
	* 更新会议纪要结果的返回事件
	* @param errorCode				错误码
	* @param eventId 事件ID
	* @param minutesId				会议纪要ID
	* @param minutesTitle			会议纪要名称
	*/
	virtual void OnMinutesUpdated(
		int32_t errorCode,
		int64_t eventId,
		int64_t minutesId,
		const std::string& minutesTitle
		);

	/**
	* 获取会议纪要详情结果的返回事件
	* @param errorCode				错误码
	* @param eventId 事件ID
	* @param minutesId				会议纪要ID
	* @param minutes				会议纪要信息
	*/
	virtual void OnMinutesDetailsReceived(
		int32_t errorCode,
		int64_t eventId,
		int64_t minutesId,
		const Minutes& minutes
		);

	/**
	* 根据会议ID获取会议纪要列表结果的返回事件
	* @param errorCode				错误码
	* @param eventId				事件ID
	* @param startTime				会议开始时间
	* @param minutesList			会议纪要信息列表
	*/
	virtual void OnMinutesListReceived(
		int32_t errorCode,
		int64_t eventId,
		int64_t startTime,
		std::vector<Minutes>& minutesList
		);

	/**
	* 删除会议录像结果的返回事件
	* @param errorCode				错误码
	* @param eventId				会议事件ID
	* @param startTime				开始时间
	*/
	virtual void OnMeetingRecordingDeleted(int32_t errorCode, int64_t eventId, int64_t startTime = 0);

	/**
	* 会议提醒事件
	* @param meetings				需要提醒的会议信息列表
	*/
	virtual void OnMeetingAlarmed(std::vector<Meeting>& meetings);

	virtual void OnMeetingFileUploaded(int32_t errorCode, int64_t eventId, int32_t operation, int32_t operatorId, const DocumentInfo& info);

	virtual void OnMeetingFileDeleted(int32_t errorCode, int64_t minutesId, int32_t operatorId);

	virtual void OnMessageSent(int32_t result, int64_t groupId, int64_t eventId, int64_t startTime, int32_t msgId, int64_t msgSeq, int64_t msgTime);

	virtual void OnTextMessageReceived(MessageChannel channel, const ConfTextMessageInfo *message);

	virtual void OnImageMessageReceived(MessageChannel channel, const ConfMediaMessageInfo *message);

	virtual void OnAudioMessageReceived(MessageChannel channel, const ConfMediaMessageInfo *message);

	virtual void OnVideoMessageReceived(MessageChannel channel, const ConfMediaMessageInfo *message);

	virtual void OnReportMessageReceived(MessageChannel channel, const ReportMessageInfo *message);

	virtual void OnRecordVideoMessageReceived(MessageChannel channel, const ConfMediaMessageInfo *message);

	virtual void OnMessageUnreadUserCount(int32_t result, int64_t groupId, const std::list<MessageReadInfo>& info);

	/**
	* 接收到会议邀请的消息
	* @param channel 消息来源
	* @param message 消息
	* @param conference 会议信息
	*/
	virtual void OnMeetingInviteMessageReceived(MessageChannel channel, const MessageInfo &message, const Meeting &meeting);

	/**
	* 接收到会议更新的消息
	* @param channel 消息来源
	* @param message 消息
	* @param meeting 会议信息
	*/
	virtual void OnMeetingUpdateMessageReceived(MessageChannel channel, const MessageInfo &message, const Meeting &meeting);

	/**
	* 接收到会议转发的消息
	* @param channel 消息来源
	* @param message 消息
	* @param meeting 会议信息
	*/
	virtual void OnMeetingForwardMessageReceived(MessageChannel channel, const MessageInfo &message, const Meeting &meeting);

	/**
	* 接收到会议请求被接受的消息
	* @param channel 消息来源
	* @param message 消息
	* @param meeting 会议信息
	*/
	virtual void OnMeetingAcceptMessageReceived(MessageChannel channel, const MessageInfo &message, const Meeting &meeting);

	/**
	* 接收到拒绝会议的消息
	* @param channel 消息来源
	* @param message 消息
	* @param meeting 会议信息
	*/
	virtual void OnMeetingDenyMessageReceived(MessageChannel channel, const MessageInfo &message, const Meeting &meeting);

	/**
	* 接收到会议取消的消息
	* @param channel 消息来源
	* @param message 消息
	* @param meeting 会议信息
	*/
	virtual void OnMeetingCancelMessageReceived(MessageChannel channel, const MessageInfo &message, const Meeting &meeting);

	/**
	* 收到会议纪要创建的消息
	* @param channel		消息来源
	* @param message		消息
	* @param operatorId		操作者ID
	* @param updateTime		更新时间
	* @param minutesId		会议纪要ID
	* @param minutesTitle	会议纪要标题
	* @int32_t state		会议纪要状态
	* @param eventId		所属事件ID
	* @param confName		会议名称
	* @param startTime		所属事件开始时间
	*/
	virtual void OnMinutesCreateMessageReceived(MessageChannel channel, const MessageInfo &message, int32_t operatorId,
		int64_t updateTime, int64_t minutesId, const std::string &minutesTitle, int32_t state, int64_t eventId, const std::string &confName, int64_t startTime = 0);

	/**
	* 接收到会议纪要更新的消息
	* @param channel		消息来源
	* @param message		消息
	* @param operatorId		操作者ID
	* @param updateTime		更新时间
	* @param minutesId		会议纪要ID
	* @param minutesTitle	会议纪要标题
	* @int32_t state		会议纪要状态
	* @param eventId		所属事件ID
	* @param confName		会议名称
	* @param startTime		所属事件开始时间
	*/
	virtual void OnMinutesUpdateMessageReceived(MessageChannel channel, const MessageInfo &message, int32_t operatorId,
		int64_t updateTime, int64_t minutesId, const std::string &minutesTitle, int32_t state, int64_t eventId, const std::string &confName, int64_t startTime = 0);

	/**
	* 接收到授权消息
	* @param channel 消息来源
	* @param message 消息
	*/
	virtual void OnAccreditMessageReceived(MessageChannel channel, const MessageInfo &message);

	/**
	* 接收到取消授权消息
	* @param channel 消息来源
	* @param message 消息
	*/
	virtual void OnAccreditCancelMessageReceived(MessageChannel channel, const MessageInfo &message);

	virtual void OnPartJoinMessageReceived(MessageChannel channel, const MessageInfo &message);

	virtual void OnPartCancelMessageReceived(MessageChannel channel, const MessageInfo &message);

	//远程协助邀请、接受、拒绝消息发送完成响应事件
	virtual void OnRemoteAssistanceInviteMsgSent(int32_t result, const std::string& conferenceId, int32_t msgId, int64_t msgSeq, int64_t msgTime);
	virtual void OnRemoteAssistanceAcceptMsgSent(int32_t result, const std::string& conferenceId, int32_t msgId, int64_t msgSeq, int64_t msgTime);
	virtual void OnRemoteAssistanceRejectMsgSent(int32_t result, const std::string& conferenceId, int32_t msgId, int64_t msgSeq, int64_t msgTime);
	
	//收到远程协助邀请、接受、拒绝、结束消息事件
	virtual void OnRemoteAssistanceInviteMsgReceived(MessageChannel channel, const MessageInfo &message, const std::string& confId, const std::string& confPwd);
	virtual void OnRemoteAssistanceAcceptMsgReceived(MessageChannel channel, const MessageInfo &message, const std::string& confId);
	virtual void OnRemoteAssistanceRejectMsgReceived(MessageChannel channel, const MessageInfo &message, const std::string& confId);
	virtual void OnRemoteAssistanceEndMsgReceived(MessageChannel channel, const MessageInfo &message, const std::string& confId);
	virtual void OnDocumentUploadMessageReceived(MessageChannel channel, int32_t operation, int64_t eventId, const MessageInfo &message, const DocumentInfo &doc);
	virtual void OnDocumentDeleteMessageReceived(MessageChannel channel, const MessageInfo &message, int64_t eventId, int64_t contentId, int32_t operatorId, const std::string &docName);
	virtual void OnMeetingRoomCreated(int32_t errorCode, MeetingRoom& room);
	virtual void OnMeetingRoomUpdated(int32_t errorCode, MeetingRoom& room);
	virtual void OnMeetingCreatedInMeetingRoom(int32_t errorCode, const std::string& roomId, const Meeting& meeting);
	virtual void OnMeetingRoomListReceived(int32_t errorCode, int64_t orgId, std::list<MeetingRoom>& rooms);
	virtual void OnCheckJoinMeetingRoom(int32_t errorCode, const std::string& roomId, int32_t hasPassword, int32_t isValid, int32_t mayJoin);
	virtual void OnCheckMeetingRoomPassword(int32_t errorCode, const std::string& roomId, const std::string& password, int32_t value);
	virtual void OnMeetingRoomDeleted(int32_t errorCode, const std::string& roomId);
	virtual void OnRightsChecked(int32_t errorCode);
	virtual void OnUserBusyFreeInfoReceived(int32_t errorCode, int32_t requestId, std::vector<UserBusyFreeInfo>& schedules);

	virtual void OnMeetingMessageRevocation(int32_t result, int64_t msgSeq, int32_t msgId, int64_t msgTime, int64_t eventId);
	virtual void OnMeetingMessageRevocationRecevied(MessageChannel channel, const MessageInfo &message, int64_t eventId, int64_t seqId);

	void NotifyMeetingReadied();
	void NotifyMeetingJoined();
	void NotifyMeetingJoinFailed();
	void NotifyMeetingClosed();
	bool IsMeetingRunning();
private:
	cefIntegration::CefClient *_cefClient;
	ICalendarService *_calendarService;
	IUCEngine *_engine;
};

}

#endif