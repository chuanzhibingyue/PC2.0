/*
 * CalendarService.h
 *
 *  Created on: Feb 9, 2015
 *      Author: ethan
 */

#ifndef CALENDARSERVICE_H_
#define CALENDARSERVICE_H_

#include "CalendarData.h"
#include "ICalendarService.h"
#include "IUCEngine.h"
#define BOOST_SPIRIT_THREADSAFE
#include "boost/property_tree/ptree.hpp"

namespace uc {
class CalendarRequestFactory;
class CalendarMsgProcessor;

class CalendarService : public ICalendarService, public IUCEngineEvent{
public:
	CalendarService(IUCEngine *engine);
	virtual ~CalendarService();
	/**
	* 创建会议
	* @param attendees			参会人列表,外部联系人以email地址传送, 内部联系人以userid方式传递;多个参会人之间分号分隔
	* @param title				会议标题
	* @param location			会议地点
	* @param summary			会议摘要
	* @param timeZone			时区
	* @param hostName			主持人名称
	* @param startTime			会议开始时间，UTC Unix时间戳
	* @param duration			会议时长
	* @param authorizerId		授权人ID
	* @param cycleRole			周期规则
	* @param isGnet				是否全时云会议,默认为true
	* @param isCycle			是否周期会议,默认为false
	* @param isWholeDay			是否全天会议，默认为false
	* @param language			会议语言1--中文，2--英文，默认为1
	* @param emailLanguage		发送邮件语言1--中文，2--英文，默认为1
	* @return					成功返回0，否则返回错误码
	*/
	virtual int32_t CreateMeeting(
		const std::vector<Attendee>& attendees,
		const std::string& title,
		const std::string& location,
		const std::string& summary,
		const std::string& timeZone,
		const std::string& hostName,
		const std::string& extrasInfo,
		int64_t startTime,
		int32_t duration,
		int32_t authorizerId,
		CycleRole& cycleRole,
		int32_t isGnet = 1,
		int32_t isCycle = 0,
		int32_t isWholeDay = 0,
		int32_t language = 1,
		int32_t emailLanguage = 1
		);


	/**
	* 变更会议
	* @param eventID			事件ID
	* @param oriTime			原会议开始时间(若周期整场可填0，周期单场则填该单场开始时间)
	* @param attendees			参会人列表,外部联系人以email地址传送, 内部联系人以userid方式传递;多个参会人之间分号分隔
	* @param title				会议标题
	* @param location			会议地点
	* @param summary			会议摘要
	* @param timeZone			时区
	* @param startTime			会议开始时间，UTC Unix时间戳
	* @param duration			会议时长
	* @param authorizerId		授权人ID
	* @param cycleRole			周期规则
	* @param isGnet				是否全时云会议,默认为true
	* @param isCycle			是否周期会议,默认为false
	* @param isWholeDay			是否全天会议，默认为false
	* @param language			会议语言1--中文，2--英文，默认为1
	* @param emailLanguage		发送邮件语言1--中文，2--英文，默认为1
	* @return					成功返回0，否则返回错误码
	*/
	virtual int32_t UpdateMeeting(
		int64_t eventId,
		int64_t oriTime,
		const std::vector<Attendee>& attendees,
		const std::string& title,
		const std::string& location,
		const std::string& summary,
		const std::string& timeZone,
		const std::string& extrasInfo,
		int64_t startTime,
		int32_t duration,
		int32_t authorizerId,
		CycleRole& cycleRole,
		int32_t isGnet = 1,
		int32_t isCycle = 0,
		int32_t isWholeDay = 0,
		int32_t language = 1,
		int32_t emailLanguage = 1
		);


	/**
	* 取消会议
	* @param eventID			事件ID
	* @param startTime			周期单场会议开始时间，周期整场传0即可,默认为0
	* @param authorizerId		授权人ID,无授权人时传0时,默认为0
	* @param language			会议语言（1--中文，2--英文），默认为1
	* @param emailLanguage		发送邮件语言（1--中文，2--英文），默认为1
	* @return					成功返回0，否则返回错误码
	*/
	virtual int32_t CancelMeeting(
		int64_t eventId,
		int64_t startTime = 0,
		int32_t authorizerId = 0,
		int32_t language = 1,
		int32_t emailLanguage = 1
		);


	/**
	* 转发会议
	* @param eventId				事件ID
	* @param attendees				参会人列表
	* @param authorizerId			授权人ID
	* @param startTime				要转发的周期单场开始时间,unix时间戳
	* @param language				会议语言（1--中文，2--英文），默认为1
	* @param emailLanguage			发送邮件语言（1--中文，2--英文），默认为1
	* @return						成功返回0，否则返回错误码
	*/
	virtual int32_t ForwardMeeting(
		int64_t eventId,
		const std::vector<Attendee>& attendees,
		int64_t startTime = 0,
		int32_t authorizerId = 0,
		int32_t language = 1,
		int32_t emailLanguage = 1
		);


	/**
	* 接受会议邀请
	* @param eventId				事件ID
	* @param startTime				接受拒绝循环会议单场时使用
	* @param authorizerId			授权人ID
	* @return						成功返回0，否则返回错误码
	*/
	virtual int32_t AcceptMeetingInvitation(
		int64_t eventId,
		int64_t startTime = 0,
		int32_t authorizerId = 0
		);


	/**
	* 拒绝会议邀请
	* @param eventId				事件ID
	* @param startTime				接受拒绝循环会议单场时使用
	* @param authorizerId			授权人ID
	* @return						成功返回0，否则返回错误码
	*/
	virtual int32_t DenyMeetingInvitation(
		int64_t eventId,
		int64_t startTime = 0,
		int32_t authorizerId = 0
		);


	/**
	* 获取会议调起串
	* @param confId				会议ID
	* @param password			参会密码
	* @return					成功返回0，否则返回错误码
	*/
	virtual int32_t GetMeetingStartURL(const std::string& confId, const std::string& password, std::string& startURL, int32_t& siteId);

	/**
	* 获取通过web入会的调起串
	* @param confId				会议ID
	* @param mail				邮件账号
	* @param password			参会密码
	* @param displayName		会议监控中显示的名字（必填）
	* @return					成功返回0，否则返回错误码
	*/
	virtual int32_t GetMeetingWebStartURL(
		const std::string& confId,
		const std::string& mail,
		const std::string& password,
		const std::string& displayName
		);

	/**
	* 更新授权设置
	* @param authorizedPersonId		被授权人ID
	* @param status					授权状态：授权1， 取消0
	* @return						成功返回0，否则返回错误码
	*/
	virtual int32_t UpdateAccreditSetting(int32_t authorizedPersonId, int32_t status);

	/**
	* 获取授权用户列表 （包括授权与被授权用户）
	* @param [out] accreditList		授权人列表
	* @return						成功返回0，否则返回错误码
	*/
	virtual int32_t GetAccreditUserList(OUT vector<Accrediting>& accreditList);

	/**
	* 获取会议列表
	* @param startTime				开始时间，UTC时间戳,以秒为单位
	* @param endTime				结束时间，UTC时间戳,以秒为单位
	* @param limit					请求的条数，默认为200
	* @param authorizerId			授权人ID
	* @param eventIdFilter			eventid过滤器，大于0有效
	* @param refreshflag			刷新类型，0 -- 本地+server, 1--强制刷新，只从server取数据，2--仅从本地取数据
	* @param filterAsc				过滤规则，与eventIdFilter联合使用。True：过滤开始时间为搜索开始时间头， 并且eventid<= eventIdFilter的会议。False：过滤开始时间为搜索开始时间尾， 并且eveintid〉=eventIdFilter的会议。默认true
	* @param [out] requestId		请求ID,用于回调时区分是哪次调用的返回
	* @param [out] infos			同步返回的会议信息列表
	* @return						成功返回0，否则返回错误码
	*/
	virtual int32_t GetMeetingList(
		int64_t startTime,
		int64_t endTime,
		int32_t limit,
		int32_t authorizerId,
		int64_t eventIdFilter,
		bool filterAsc,
		int32_t refreshflag,
		OUT int32_t& requestId,
		OUT vector<Meeting>& meetings
		);

	/**
	* 获取会议简明列表
	* @param startTime				开始时间，UTC时间戳,以秒为单位
	* @param endTime				结束时间，UTC时间戳,以秒为单位
	* @param authorizerId			授权人ID
	* @param [out] requestId		请求ID,用于回调时区分是哪次调用的返回
	* @param [out] meetings			同步返回的会议信息列表
	*/
	virtual int32_t GetBriefMeetingList(int64_t startTime, int64_t endTime, int32_t authorizerId, OUT int32_t& requestId, OUT vector<Meeting>& meetings);

	/**
	* 通过操作时间获取变化过的简要会议信息
	* @param actionTime				会议操作时间, UTC时间戳
	* @param authorizerId			授权人ID
	*/
	virtual int32_t GetBriefChangedMeetingList(int64_t startTime, int32_t authorizerId = 0);


	/**
	* 根据会议ID获取单场会议详情（后期废除，统一使用批量接口GetMeetingInfoByIds）
	* @param conferenceId			会议ID
	* @param startTime				限定范围的起始时间
	* @param startTime				限定范围的终止时间
	* @param authorizerId			授权人ID
	* @param [out]infos				会议信息列表
	* @return						成功返回0，否则返回错误码
	*/
	virtual int32_t GetMeetingInfoById(
		int64_t eventId,
		int64_t startTime,
		int64_t endTime,
		int32_t authorizerId,
		OUT vector<Meeting>& meetings
		);

	/**
	* 根据多个会议ID获取多场会议详情
	* @param eventIds				事件ID列表
	* @param startTime				起始时间
	* @param endTime				终止时间
	* @param authorizerId			授权人ID
	* @param [out] requestId		本次请求ID,用于多次调用接口时区分哪一次的返回
	* @param [out] infos			会议信息列表
	* @return						成功返回0，否则返回错误码
	*/
	virtual int32_t GetMeetingInfoByIds(
		const std::vector<int64_t>& eventIds,
		int64_t startTime,
		int64_t endTime,
		int32_t authorizerId,
		OUT int32_t& requestId,
		OUT vector<Meeting>& meetings
		);



	/**
	* 获取用户忙闲日程列表
	* @param userIdList				用户ID列表
	* @param startTime				开始日期，unix时间戳
	* @param endTime				结束日期，unix时间戳
	* @param timeInterval			时间间隔，在5-60之间
	* @param eventIdFilter			eventid过滤器，大于0有效
	* @param [out]requestId			本次请求ID
	* @return						成功返回0，否则返回错误码
	*/
	virtual int32_t GetUserSchedule(
		const std::vector<int32_t>& userIdList,
		int64_t startTime,
		int64_t endTime,
		int32_t timeInterval,
		int64_t eventIdFilter,
		OUT int32_t& requestId
		);

	/**
	* 创建会议纪要
	* @param eventId				事件ID
	* @param startTime				会议开始时间
	* @param title					会议纪要标题
	* @param minutes				会议纪要内容
	* @return						成功返回0，否则返回错误码
	*/
	virtual int32_t CreateMinutes(
		int64_t eventId,
		int64_t startTime,
		const std::string& title,
		const std::string& minutes
		);

	/**
	* 更新会议纪要
	* @param summaryId				会议纪要ID
	* @param title					会议纪要标题
	* @param minutes				会议纪要内容
	* @return						成功返回0，否则返回错误码
	*/
	virtual int32_t UpdateMinutes(
		int64_t eventId,
		int64_t minutesId,
		const std::string& title,
		const std::string& minutes
		);

	/**
	* 获取会议纪要详情
	* @param minutesId				会议纪要ID
	* @return						成功返回0，否则返回错误码
	*/
	virtual int32_t GetMinutesDetails(int64_t eventId, int64_t minutesId);

	/**
	* 根据会议ID 获取会议纪要列表
	* @param eventId				事件ID
	* @param startTime				会议开始时间
	* @return						成功返回0，否则返回错误码
	*/
	virtual int32_t GetMinutesList(int64_t eventId, int64_t startTime = 0);

	/**
	* 删除会议视频录象
	* @param eventId				事件ID
	* @param confId					会议ID
	* @param startTime				会议开始时间
	* @return						成功返回0，否则返回错误码
	*/
	virtual int32_t DeleteMeetingRecording(int64_t eventId, const std::string& confId, int64_t startTime = 0);

	/**
	* 撤销会议消息
	* @param sessionType			会话类型
	* @param seqId					seqId
	* @param eventId				eventId
	* @return						成功返回0，否则返回错误码
	*/
	virtual int32_t RevocationMeetingMessage(int64_t groupId, int64_t eventId, int64_t seqId, int32_t& msgId);

	/**
	* 创建远程协助
	* @param userId				对方用户ID
	*/
	virtual int32_t CreateRemoteAssistance(int32_t userId);

   /**
	* 重发消息;
	* @param msgId              消息ID;
	* @return                   错误码;
	*/
	virtual int32_t ResendMessage(int32_t msgId);

   /**
	* 转发消息;
	* @param groupUid 			接收者ID;
	* @param msgId              消息ID;
	* @return                   错误码;
	*/
	virtual int32_t ForwardMessage(UCID groupUid, int32_t msgId);

	virtual int32_t SendTextMessage(int64_t eventId, int64_t startTime, UCID groupUid, int8_t textType, const std::string& textFormat, const std::string& textData, OUT int32_t& msgId);

	virtual int32_t SendImageMessage(int64_t eventId, int64_t startTime, UCID groupUid, const std::string& imageName, int32_t imageSize,
	 const std::string& imageURL, const std::string& imageData, OUT int32_t& msgId);

	virtual int32_t SendAudioMessage(int64_t eventId, int64_t startTime, UCID groupUid, const std::string& audioName, int32_t audioSize,
	 const std::string& audioURL, int32_t audioDuration, OUT int32_t& msgId);

	virtual int32_t SendVideoMessage(int64_t eventId, int64_t startTime, UCID groupUid, const std::string& videoName, int32_t videoSize,
	 const std::string& videoURL, const std::string& videoData, int32_t videoDuration, OUT int32_t& msgId);

	virtual int32_t SendRemoteAssistanceInviteMessage(UCID& uid, const std::string& confId, const std::string& confPwd, OUT int32_t& msgId);
	virtual int32_t SendRemoteAssistanceAcceptMessage(UCID& uid, const std::string& confId, OUT int32_t& msgId);
	virtual int32_t SendRemoteAssistanceRejectMessage(UCID& uid, const std::string& confId, OUT int32_t& msgId);

	virtual int32_t OnMessageReceived(MessageChannel channel, UCMsg &msg, const string &body, MsgSendState state);

	virtual void OnUCASConnected(int32_t errorCode);

	virtual void OnUCASDisconnected(int32_t errorCode);

	virtual int32_t SetMeetingAlarm(int32_t alarmStatus, int32_t alarmTime = 180);

	virtual int32_t UploadMeetingFile(int64_t eventId, int32_t operation, const DocumentInfo& info, int32_t& msgId);

	virtual int32_t DeleteMeetingFile(int64_t minutesId);

	virtual int32_t GetMeetingSiteId();

	//会议室接口
	virtual int32_t CreateMeetingRoom(
		const std::vector<Attendee>& attendees,
		const std::string& title,
		const std::string& remarks,
		const std::string& password,
		int64_t startTime,
		int64_t endTime,
		int64_t roomHostId,
		int64_t orgId,
		int32_t isSetPassword = 1,
		int32_t defaultAttendsVisible = 1,
		int32_t defaultAttendsJoin = 1);

	virtual int32_t UpdateMeetingRoom(
		std::string& roomId,
		const std::vector<Attendee>& attendees,
		const std::string& title,
		const std::string& remarks,
		const std::string& password,
		int64_t startTime,
		int64_t endTime,
		int64_t roomHostId,
		int64_t orgId,
		int32_t isSetPassword = 1,
		int32_t defaultAttendsVisible = 1,
		int32_t defaultAttendsJoin = 1);

	virtual int32_t CreateMeetingInMeetingRoom(
		const std::vector<Attendee>& attendees,
		const std::string& roomId,
		const std::string& title,
		const std::string& summary,
		int64_t startTime,
		int32_t duration);

	virtual int32_t GetMeetingRoomList(int64_t orgId);

	virtual int32_t CheckJoinMeetingRoom(const std::string& roomId);

	virtual int32_t CheckMeetingRoomPassword(const std::string& roomId, const std::string& password);

	virtual int32_t DeleteMeetingRoom(const std::string& roomId);

	virtual int32_t CheckRightsOfCreateRoom();

	virtual int32_t GetUserBusyFreeInfo(const std::vector<int32_t>& userIdList, int64_t startTime, int64_t endTime, int32_t shareUserId, OUT int32_t& requestId);
protected:
	//各接口回调
	int32_t OnMeetingCreated(int32_t errorCode, HTTPResponse* response, Meeting* meetingInfo);
	int32_t OnRemotAssistanceCreated(int32_t errorCode, HTTPResponse* response, int32_t userId);
	int32_t OnMeetingUpdated(int32_t errorCode, HTTPResponse* response, int64_t eventId, int64_t oriStart, int32_t authorizerId, Meeting* meeting);
	int32_t OnMeetingCanceled(int32_t errorCode, HTTPResponse* response, int64_t eventId, int64_t startTime, int32_t authorizerId);
	int32_t OnMeetingStartURLReceived(int32_t errorCode, HTTPResponse* response, std::string confId);
	int32_t OnConferenceStartUrlReceivedByConfId(int32_t errorCode, HTTPResponse* response, std::string confId);
	int32_t OnMeetingWebStartURLReceived(int32_t errorCode, HTTPResponse* response, std::string confId);
	int32_t OnAccreditSettingUpdated(int32_t errorCode, HTTPResponse* response, int32_t authorizedPersonId, int32_t status);
	int32_t OnAccreditUserListReceived(int32_t errorCode, HTTPResponse* response);
	int32_t OnMeetingListReceived(int32_t errorCode, HTTPResponse* response, int32_t requestId, int32_t authorizerId, int64_t startTime, int64_t endTime, int64_t eventIdFilter, vector<Meeting> localMeetings);
	int32_t OnBriefMeetingListReceived(int32_t errorCode, HTTPResponse* response, int32_t requestId, int32_t authorizerId, int64_t startTime, int64_t endTime);
	int32_t OnBriefChangedMeetingListReceived(int32_t errorCode, HTTPResponse* response, int32_t authorizerId, int64_t startTime);
	int32_t OnMeetingInvitationAccepted(int32_t errorCode, HTTPResponse* response, int64_t eventId, int32_t authorizerId, int64_t startTime);
	int32_t OnMeetingInvitationDenied(int32_t errorCode, HTTPResponse* response, int64_t eventId, int32_t authorizerId, int64_t startTime);
	int32_t OnUserScheduleReceived(int32_t errorCode, HTTPResponse* response, int64_t startTime, int64_t endTime, int32_t timeInterval, int64_t eventIdFilter, std::vector<int32_t> userIdList, int32_t requestId);
	int32_t OnMeetingForwarded(int32_t errorCode, HTTPResponse* response, int64_t eventId, int64_t startTime, std::vector<Attendee> attendees, int32_t authorizerId);
	int32_t OnMeetingInfoReceivedById(int32_t errorCode, HTTPResponse* response, int64_t eventId, int64_t startTime, int32_t authorizerId);
	int32_t OnMeetingInfoReceivedByIds(int32_t errorCode, HTTPResponse* response, int32_t requestId, int32_t authorizerId);
	int32_t OnMinutesCreated(int32_t errorCode, HTTPResponse* response, int64_t eventId, int64_t startTime, std::string title);
	int32_t OnMinutesUpdated(int32_t errorCode, HTTPResponse* response, int64_t eventId, int64_t minutesId, std::string title);
	int32_t OnMinutesDetailsReceived(int32_t errorCode, HTTPResponse* response, int64_t eventId, int64_t minutesId);
	int32_t OnMinutesListReceived(int32_t errorCode, HTTPResponse* response, int64_t eventId, int64_t startTime);
	int32_t OnMeetingRecordingDeleted(int32_t errorCode, HTTPResponse* response, int64_t eventId, int64_t startTime);
	int32_t OnSendMessageResponse(int32_t errorCode, HTTPResponse *response, int32_t groupId, int64_t eventId, int64_t startTime, int32_t msgId, bool saveUnread);
	int32_t OnSendRemoteAssistanceInviteMsgResponse(int32_t errorCode, HTTPResponse *response, std::string conferenceId, int32_t msgId, int64_t msgTime);
	int32_t OnSendRemoteAssistanceAcceptMsgResponse(int32_t errorCode, HTTPResponse *response, std::string conferenceId, int32_t msgId, int64_t msgTime);
	int32_t OnSendRemoteAssistanceRejectMsgResponse(int32_t errorCode, HTTPResponse *response, std::string conferenceId, int32_t msgId, int64_t msgTime);
	int32_t OnMeetingFileUploaded(int32_t errorCode, HTTPResponse *response, int64_t eventId, int32_t operation, int32_t msgId, DocumentInfo info);
	int32_t OnMeetingFileDeleted(int32_t errorCode, HTTPResponse *response, int64_t minutesId);
	int32_t OnMeetingMessageRevocationResponse(int32_t errorCode, HTTPResponse *response, int64_t eventId, int64_t groupId, int64_t seqId, int32_t msgId);
	void OnMeetingAlarmed();
	int32_t OnMeetingRoomCreated(int32_t errorCode, HTTPResponse* response, MeetingRoom room);
	int32_t OnMeetingRoomUpdated(int32_t errorCode, HTTPResponse* response, MeetingRoom room);
	int32_t OnMeetingCreatedInMeetingRoom(int32_t errorCode, HTTPResponse* response, std::string roomId, Meeting meeting);
	int32_t OnMeetingRoomListReceived(int32_t errorCode, HTTPResponse* response, int64_t orgId);
	int32_t OnCheckJoinMeetingRoom(int32_t errorCode, HTTPResponse* response, std::string roomId);
	int32_t OnCheckMeetingRoomPassword(int32_t errorCode, HTTPResponse* response, std::string roomId, std::string password);
	int32_t OnMeetingRoomDeleted(int32_t errorCode, HTTPResponse* response, std::string roomId);
	int32_t OnRightsChecked(int32_t errorCode, HTTPResponse* response);
	int32_t OnUserBusyFreeInfoReceived(int32_t errorCode, HTTPResponse* response, int64_t startTime, int64_t endTime, std::vector<int32_t> userIdList, int32_t shareUserId, int32_t requestId);
	
public:
	/**
	* 获取请求URL
	* @param methodUrl				接口
	* @return						完整的request rul
	*/
	std::string GetRequestUrl(const std::string &urlMethod);

	/**
	* 获取当前用户ID
	* @param
	* @return	用户ID
	*/
	int32_t GetUserId();

	/**
	* 获取当前用户UID
	* @param
	* @return	用户ID
	*/
	UCID GetCurUser();

	/**
	* 获取当前用户名
	* @param
	* @return	用户名
	*/
	std::string GetAccount();

	/**
	* 获取当前SessionID
	* @param
	* @return	sessionID
	*/
	std::string GetSessionId();

	/**
	* 获取当前domain
	* @param
	* @return	domain
	*/
	std::string GetDoMain();
	std::string GetRemoteAvatar();
	std::string GetUserName();

	bool IsCurUser(UCID userUid);
	bool IsNumber(const string& str);
	int64_t StringToInt64(const string& str);
	int32_t StringToInt32(const string& str);
	bool StringToBool(const string& str);
	bool ParseMeeting(const boost::property_tree::wptree &ptParse, Meeting *info);
	bool IsDeniedMeeting(const Meeting& meeting);
	bool IsCanceledMeeting(const Meeting& meeting);
	int64_t GetCurrentUnixTime();
	void ParseICalendarStr(const std::string& icalendar, vector<Meeting>& infos);
	void ParseICalendarStr(const std::string& icalendar, Meeting& info);
	void CacheMeeting(const Meeting& meeting);
	void UpdateCahcheMeetingStatus(uc::AttendeeStatus status, int64_t eventId, int64_t startTime = 0, int32_t authorizerId = 0);
	int32_t GetConferenceIdByEventId(int64_t eventId, int64_t startTime = 0, int32_t authorizerId = 0);
	bool GetMeetingInfoByEventId(Meeting& meeting, int64_t eventId, int64_t startTime = 0, int32_t authorizerId = 0);
	int32_t ConvertAddressStr2Obj(const std::string& addressStr, Address& addressObj);
	void SetMeetingAddressValueFromExtrasInfo(Meeting& meeting);
	void SetMeetingAddressValueFromExtrasInfo(vector<Meeting>& meetings);

protected:
	void CacheMeetings(std::vector<Meeting>& meetings);
	void SetMeetingAuthorizerId(std::vector<Meeting>& meetings, int32_t authorizerId);
	int32_t MakeRequestId();
	void UpdateSyncRange(int32_t userId, int64_t startTime, int64_t endTime);
	bool IsTimeInSyncRange(int32_t userId, int64_t time);
	bool haveOldMeeting(const std::vector<Meeting>& meetings);
	int64_t GetSyncRangStart(int32_t userId);
	int64_t GetSyncRangEnd(int32_t userId);
	void SortMeetingList(std::vector<Meeting>& meetings);
	void EraseNoDisplayedMeeting(std::vector<Meeting>& meetings);
	bool FindInCacheMeeting(int64_t eventId, int64_t startTime, int32_t authorizerId, Meeting& meeting);
	bool FindInCacheMeeting(const std::string& confId, Meeting& meeting);
	int64_t GetTodayTime();
	void CacheMeetingList();
	void OnCacheConferenceListReceived(int32_t errorCode, HTTPResponse* response, int64_t startTime);
	void MergeBusyTime(int64_t startTime, int64_t endTime, int32_t timeInterval, std::vector<int32_t> times, std::vector<TimeSlot>& timeSlots);
	int64_t diff_tm(struct tm *a, struct tm *b);
	int32_t GetGroupMemberList(int64_t groupId, int32_t& count, UserIdList& list);
	int64_t GetEventIdByGroupId(int64_t groupId);
	int32_t OnMessageUnreadUserNotify(int64_t groupId, int64_t seq);
private:
	CalendarRequestFactory * _calendarRequestFactory;			//日程相关request生成器
	CalendarMsgProcessor * _calendarMsgProcessor;				//日程相关消息处理器
	std::map<int32_t, SyncRange> _syncInfo;						//保存与server会议信息同步信息
	std::vector<Meeting> _cacheMeetings;						//用于缓存getconferenceInfobyId接口返回的数据
	int32_t _alarmTime;			//会议提醒时间，单位秒，默认会前180秒
	int64_t _alarmId;			//会议提醒定时器ID
	bool _bAlreadyCleanup;		//是否已经清空数据库
};

class CalendarServiceEvent : public ICalendarServiceEvent {

};
} /* namespace uc */
#endif /* CALENDARSERVICE_H_ */
