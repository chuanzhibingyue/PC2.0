/*
 * ICalendarService.h
 *
 *  Created on: Feb 11, 2015
 *      Author: ethan
 */

#ifndef ICALENDARSERVICE_H_
#define ICALENDARSERVICE_H_

#include "UCSDK.h"
#include "IUCEngine.h"
#include "common/event.h"
#include <vector>
#include "CalendarData.h"
#include "MessageInfo.h"
#include "MessageReadInfo.h"

namespace uc {
interface UC_API ICalendarServiceEvent;

interface UC_API ICalendarService : public IUCService, Event<ICalendarServiceEvent> {
	virtual ~ICalendarService() {}

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
		) = 0;

	
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
									) = 0;


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
		) = 0;


	/**
	* 转发会议
	* @param eventId				事件ID
	* @param attendees				参会人列表
	* @param startTime				要转发的周期单场开始时间,unix时间戳
	* @param authorizerId			授权人ID
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
		) = 0;


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
		) = 0;


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
		) = 0;


	/**
	* 获取会议调起串
	* @param confId				会议ID
	* @param password			参会密码
	* @[out]param startURL		入会吊起串
	* @param siteId				内部区分会议版本使用，上层忽略
	* @return					成功返回0，否则返回错误码
	*/
	virtual int32_t GetMeetingStartURL(const std::string& confId, const std::string& password, std::string& startURL, int32_t& siteId) = 0;

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
		) = 0;

	/**
	* 更新授权设置
	* @param authorizedPersonId		被授权人ID
	* @param status					授权状态：授权1， 取消0
	* @return						成功返回0，否则返回错误码
	*/
	virtual int32_t UpdateAccreditSetting( int32_t authorizedPersonId, int32_t status ) = 0;

	/**
	* 获取授权用户列表 （包括授权与被授权用户）
	* @param [out] accreditList		授权人列表 
	* @return						成功返回0，否则返回错误码
	*/
	virtual int32_t GetAccreditUserList(OUT vector<Accrediting>& accreditList) = 0;

	/**
	* 获取会议列表
	* @param startTime				开始时间，UTC时间戳,以秒为单位
	* @param endTime				结束时间，UTC时间戳,以秒为单位
	* @param limit					请求的条数，默认为200
	* @param authorizerId			授权人ID
	* @param eventIdFilter			eventid过滤器，大于0有效
	* @param refreshflag			刷新类型，0 -- 本地+server, 1--强制刷新，只从server取数据，2--仅从本地取数据 
	* @param bfilterAsc				过滤规则，与eventIdFilter联合使用。True：过滤开始时间为搜索开始时间头， 并且eventid<= eventIdFilter的会议。False：过滤开始时间为搜索开始时间尾， 并且eveintid〉=eventIdFilter的会议。默认true
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
									bool bfilterAsc,
									int32_t refreshflag,
									OUT int32_t& requestId,
									OUT vector<Meeting>& meetings
									) = 0;
	
	/**
	* 获取会议简明列表
	* @param startTime				开始时间，UTC时间戳,以秒为单位
	* @param endTime				结束时间，UTC时间戳,以秒为单位
	* @param authorizerId			授权人ID
	* @param [out] requestId		请求ID,用于回调时区分是哪次调用的返回
	* @param [out] meetings			同步返回的会议信息列表
	*/
	virtual int32_t GetBriefMeetingList(int64_t startTime, int64_t endTime, int32_t authorizerId, OUT int32_t& requestId, OUT vector<Meeting>& meetings) = 0;

	/**
	* 通过操作时间获取变化过的简要会议信息
	* @param startTime				上一次更新时间, UTC时间戳
	* @param authorizerId			授权人ID
	*/
	virtual int32_t GetBriefChangedMeetingList(int64_t startTime, int32_t authorizerId = 0) = 0;

	
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
										) = 0;

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
										) = 0;



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
									) = 0;

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
									) = 0;

	/**
	* 更新会议纪要
	* @param eventId 				事件ID
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
									) = 0;

	/**
	* 获取会议纪要详情
	* @param eventId 				事件ID
	* @param minutesId				会议纪要ID
	* @return						成功返回0，否则返回错误码
	*/
	virtual int32_t GetMinutesDetails(int64_t eventId, int64_t minutesId) = 0;

	/**
	* 根据会议ID 获取会议纪要列表
	* @param eventId				事件ID
	* @param startTime				会议开始时间
	* @return						成功返回0，否则返回错误码
	*/
	virtual int32_t GetMinutesList(int64_t eventId, int64_t startTime = 0) = 0;

	/**
	* 删除会议视频录象
	* @param eventId				事件ID
	* @param confId					会议ID
	* @param startTime				会议开始时间
	* @return						成功返回0，否则返回错误码
	*/
	virtual int32_t DeleteMeetingRecording(int64_t eventId, const std::string& confId, int64_t startTime = 0) = 0;

	/**
	* 撤销会议消息
	* @param confId				会议Id
	* @param sessionType			会话类型
	* @param seqId					seqId
	* @param eventId				eventId
	* @return						成功返回0，否则返回错误码
	*/
	virtual int32_t RevocationMeetingMessage(int64_t groupId, int64_t eventId, int64_t seqId, int32_t& msgId) = 0;

   /**
	* 重发消息;
	* @param msgId              消息ID;
	* @return                   错误码;
	*/
	virtual int32_t ResendMessage(int32_t msgId) = 0;

   /**
	* 转发消息;
	* @param groupUid 			接收者ID;
	* @param msgId              消息ID;
	* @return                   错误码;
	*/
	virtual int32_t ForwardMessage(UCID groupUid, int32_t msgId) = 0;

	/**
	* 发送聊天文本消息接口;
	* @param eventId				会议ID;
	* @param conferenceId			全时会议id;
	* @param startTime			    会议开始时间;
	* @param groupUid               会议讨论组UID;
	* @param textType               文本类型：0x01:纯文本，0x02:MIME格式;
	* @param textFormat		        文本格式;
	* @param textData               文本内容;
	* @param [out]msgId             消息ID;
	* @return						成功返回0，否则返回错误码;
	*/
	virtual int32_t SendTextMessage(int64_t eventId, int64_t startTime, UCID groupUid, int8_t textType, const std::string& textFormat, const std::string& textData, OUT int32_t& msgId) = 0;

	/**
	 * 发送聊天图片消息接口;
	 * @param eventId				会议ID;
	 * @param conferenceId			全时会议id;
	 * @param startTime			    会议开始时间;
	 * @param groupUid              会议讨论组UID;
	 * @param imageName	            图片名称;
	 * @param imageSize	            图片大小;
	 * @param imageURL	            图片下载路径;
	 * @param imageData	            图片缩略图;
	 * @param [out]msgId            消息ID;
	 * @return			            错误码;
     */
	virtual int32_t SendImageMessage(int64_t eventId, int64_t startTime, UCID groupUid, const std::string& imageName, int32_t imageSize,
		const std::string& imageURL, const std::string& imageData, OUT int32_t& msgId) = 0;

	/**
	 * 发送聊天音频消息接口;
	 * @param eventId				会议ID;
	 * @param conferenceId			全时会议id;
	 * @param startTime			    会议开始时间;
	 * @param groupUid              会议讨论组UID;
	 * @param audioName             音频名称;
	 * @param audioSize             音频大小;
	 * @param audioURL              音频下载路径;
	 * @param audioDuration         音频时长;
	 * @param [out]msgId            消息ID;
	 * @return                      错误码;
      */
	virtual int32_t SendAudioMessage(int64_t eventId, int64_t startTime, UCID groupUid, const std::string& audioName, int32_t audioSize,
	 const std::string& audioURL, int32_t audioDuration, OUT int32_t& msgId) = 0;

	/**
	 * 发送聊天视频消息接口;
	 * @param eventId				会议ID;
	 * @param conferenceId			全时会议id;
	 * @param startTime			    会议开始时间;
	 * @param groupUid              会议讨论组UID;
	 * @param videoName             视频名称;
	 * @param videoSize             视频大小;
	 * @param videoURL              视频下载路径;
	 * @param videoData             视频缩略图;
	 * @param videoDuration         视频时长;
	 * @param [out]msgId            消息ID;
	 * @return                      错误码;
     */
	virtual int32_t SendVideoMessage(int64_t eventId, int64_t startTime, UCID groupUid, const std::string& videoName, int32_t videoSize,
	 const std::string& videoURL, const std::string& videoData, int32_t videoDuration, OUT int32_t& msgId) = 0;

	/**
	* 设置会议提醒;
	* @param alarmStatus			会议提醒状态：1--开启， 0--关闭
	* @param conferenceId			会议提醒时间，单位秒，默认会前180秒
	* @return                       成功返回0，否则返回错误码;
	*/
	virtual int32_t SetMeetingAlarm(int32_t alarmStatus, int32_t alarmTime = 180) = 0;

	/**
	* 创建远程协助
	* @param userId				对方用户ID
	*/
	virtual int32_t CreateRemoteAssistance(int32_t userId) = 0;


	//发送远程协助邀请、接受、拒绝消息
	virtual int32_t SendRemoteAssistanceInviteMessage(UCID& uid, const std::string& confId, const std::string& confPwd, OUT int32_t& msgId) = 0;
	virtual int32_t SendRemoteAssistanceAcceptMessage(UCID& uid, const std::string& confId, OUT int32_t& msgId) = 0;
	virtual int32_t SendRemoteAssistanceRejectMessage(UCID& uid, const std::string& confId, OUT int32_t& msgId) = 0;

	//上传会议文档
	virtual int32_t UploadMeetingFile(int64_t eventId, int32_t operation, const DocumentInfo& info, int32_t& msgId) = 0;
	//删除会议文档
	virtual int32_t DeleteMeetingFile(int64_t minutesId) = 0;

	//会议室接口
	/**
	* 创建会议室;
	* @param attendees				参会人列表
	* @param title					会议室标题，长度小于1000
	* @param remarks				会议室备注
	* @param password				会议室密码
	* @param startTime				会议室有效期的开始时间
	* @param endTime				会议室有效期的结束时间
	* @param roomHostId				会议室主持人ID
	* @param orgId					组织ID
	* @param isSetPassword			是否设置会议室密码，1为是，0为否， 默认为1， 
	* @param defaultAttendsVisible	是否只允许默认参与者可见，1为是，0为否， 默认为1
	* @param defaultAttendsJoin		是否只允许只允许默认参与者加入，1为是，0为否， 默认为1， 
	* @return                       成功返回0，否则返回错误码;
	*/
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
		int32_t defaultAttendsJoin = 1) = 0;

	/**
	* 更新会议室;
	* @param roomId					会议室ID
	* @param attendees				参会人列表
	* @param title					会议室标题，长度小于1000
	* @param remarks				会议室备注
	* @param password				会议室密码
	* @param startTime				会议室有效期的开始时间
	* @param endTime				会议室有效期的结束时间
	* @param roomHostId				会议室主持人ID
	* @param orgId					组织ID
	* @param isSetPassword			是否设置会议室密码，1为是，0为否， 默认为1，
	* @param defaultAttendsVisible	是否只允许默认参与者可见，1为是，0为否， 默认为1
	* @param defaultAttendsJoin		是否只允许只允许默认参与者加入，1为是，0为否， 默认为1，
	* @return                       成功返回0，否则返回错误码;
	*/
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
		int32_t defaultAttendsJoin = 1) = 0;

	//会议室接口
	/**
	* 创建会议室会议;
	* @param attendees				参会人列表
	* @param roomId					会议室ID
	* @param title					会议标题，长度小于1000
	* @param summary				会议备注
	* @param startTime				会议开始时间
	* @param duration					会议时长
	* @return                       成功返回0，否则返回错误码;
	*/
	virtual int32_t CreateMeetingInMeetingRoom(
		const std::vector<Attendee>& attendees,
		const std::string& roomId,
		const std::string& title,
		const std::string& summary,
		int64_t startTime,
		int32_t duration) = 0;

	/**
	* 获取会议室列表;
	* @param orgId					组织ID
	* @return                       成功返回0，否则返回错误码;
	*/
	virtual int32_t GetMeetingRoomList(int64_t orgId) = 0;

	/**
	* 检查会议室是否可以入会;
	* @param roomId					会议室ID
	* @return                       成功返回0，否则返回错误码;
	*/
	virtual int32_t CheckJoinMeetingRoom(const std::string& roomId) = 0;

	/**
	* 检查会议室密码是否有效;
	* @param roomId					会议室ID
	* @param password				会议室密码
	* @return                       成功返回0，否则返回错误码;
	*/
	virtual int32_t CheckMeetingRoomPassword(const std::string& roomId, const std::string& password) = 0;

	/**
	* 删除会议室;
	* @param roomId					会议室ID
	* @return                       成功返回0，否则返回错误码;
	*/
	virtual int32_t DeleteMeetingRoom(const std::string& roomId) = 0;

	virtual int32_t CheckRightsOfCreateRoom() = 0;

	virtual int32_t GetUserBusyFreeInfo(const std::vector<int32_t>& userIdList, int64_t startTime, int64_t endTime, int32_t shareUserId, OUT int32_t& requestId) = 0;
};

interface UC_API ICalendarServiceEvent {
	virtual ~ICalendarServiceEvent() {};

	/**
	* 创建会议结果的返回事件
	* @param errorCode				错误码
	* @param meeting				会议信息
	* @return
	*/
	virtual void OnMeetingCreated( int32_t errorCode, Meeting& meeting) = 0;

	/**
	* 远程协助创建完成的返回事件
	* @param errorCode				错误码
	* @param confId					会议ID
	* @param passWd					参会人密码
	* @param startURL				主持人主会调起串
	* @param userId					对方ID
	* @return
	*/
	virtual void OnRemotAssistanceCreated(int32_t errorCode, const std::string& confId, const std::string& passWd, const std::string& startURL, int32_t userId) = 0;

	/**
	* 更新会议结果的返回事件
	* @param errorCode				错误码
	* @param meeting				会议信息
	*/
	virtual void OnMeetingUpdated(int32_t errorCode, int64_t eventId, int64_t startTime, Meeting& meeting) = 0;

	/**
	* 取消会议结果的返回事件
	* @param errorCode				错误码
	* @param conferenceId			会议ID
	* @param eventId				日程事件ID
	* @param startTime				会议开始时间，UTC Unix时间戳
	* @param authorizerId			授权人ID
	*/
	virtual void OnMeetingCanceled(int32_t errorCode, int64_t eventId, int64_t startTime = 0, int32_t authorizerId = 0) = 0;

	/**
	* 获取会议调起串结果的返回事件
	* @param errorCode				错误码
	* @param confId					会议ID
	* @param startURL				调起串
	*/
	virtual void OnMeetingStartURLReceived(int32_t errorCode, const std::string& confId, const std::string& startURL) = 0;

	/**
	* 通过邮件获取会议调起串结果的返回事件
	* @param errorCode				错误码
	* @param confId					会议ID
	* @param startURL				调起串
	*/
	virtual void OnMeetingWebStartURLReceived(int32_t errorCode, const std::string& confId, const std::string& startURL) = 0;

	/**
	* 更新授权调协结果的返回事件
	* @param errorCode				错误码
	* @param errorMsg				错误描述
	* @param authorizedPersonId		授权或取消授权的人的ID
	* @param status					授权状态 1：授权, 0：取消授权
	*/
	virtual void OnAccreditSettingUpdated(int32_t errorCode, int32_t authorizedPersonId, int32_t status) = 0;

	/**
	* 获取授权用户列表结果的返回事件
	* @param errorCode				错误码
	* @param accreditUserList		日程共享用户列表
	*/
	virtual void OnAccreditUserListReceived(int32_t errorCode, std::vector<Accrediting>& accreditUserList) = 0;

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
	virtual void OnMeetingListReceived(int32_t errorCode, int32_t requestId, int64_t startTime, int64_t endTime, int64_t eventIdFilter, int32_t authorizerId, std::vector<Meeting>& meetings) = 0;

	/**
	* 获取简明会议列表结果的返回事件
	* @param errorCode				错误码
	* @param requestId				本次请求ID,用于多次调用接口时区分哪一次的返回
	* @param startTime				起始时间
	* @param endTime				终止时间
	* @param authorizerId			授权人ID
	* @param meetings				会议信息列表
	*/
	virtual void OnBriefMeetingListReceived(int32_t errorCode, int32_t requestId, int64_t startTime, int64_t endTime, int32_t authorizerId, std::vector<Meeting>& meetings) = 0;
	
	/**
	* 获取变更的简明会议列表结果的返回事件
	* @param errorCode				错误码
	* @param actionTime				上次更新时间
	* @param authorizerId			授权人ID
	* @param meetings				会议信息列表
	*/
	virtual void OnBriefChangedMeetingListReceived(int32_t errorCode, int64_t startTime, int32_t authorizerId, std::vector<Meeting>& meetings) = 0;

	/**
	* 获取会议详情结果的返回事件
	* @param errorCode				错误码
	* @param eventId				事件ID
	* @param authorizerId			授权人ID
	* @param meetings				会议信息列表
	*/
	virtual void OnMeetingInfoReceivedById(int32_t errorCode, int64_t eventId, int32_t authorizerId, std::vector<Meeting>& meetings) = 0;

	/**
	* 批量获取会议详情结果的返回事件
	* @param errorCode				错误码
	* @param requestId				本次请求ID,用于多次调用接口时区分哪一次的返回
	* @param authorizerId			授权人ID
	* @param meetings				会议信息列表
	*/
	virtual void OnMeetingInfoReceivedByIds(int32_t errorCode, int32_t requesetId, int32_t authorizerId, std::vector<Meeting>& meetings) = 0;

	/**
	* 接受会议邀请结果的返回事件
	* @param errorCode				错误码
	* @param eventId				事件ID
	* @param startTime				开始时间
	* @param authorizerId			授权人ID
	*/
	virtual void OnMeetingInvitationAccepted(int32_t errorCode, int64_t eventId, int64_t startTime = 0, int32_t authorizerId = 0) = 0;

	/**
	* 拒绝会议邀请结果的返回事件
	* @param errorCode				错误码
	* @param eventId				事件ID
	* @param startTime				开始时间
	* @param authorizerId			授权人ID
	*/
	virtual void OnMeetingInvitationDenied(int32_t errorCode, int64_t eventId, int64_t startTime = 0, int32_t authorizerId = 0) = 0;

	/**
	* 转发会议邀请结果的返回事件
	* @param errorCode				错误码
	* @param meeting				会议信息
	*/
	virtual void OnMeetingForwarded(int32_t errorCode, Meeting& meeting) = 0;

	/**
	* 获取用户日程列表结果的返回事件
	* @param errorCode				错误码
	* @param requestId				本次请求ID,用于多次调用接口时区分哪一次的返回
	* @param schedules				日程信息列表 
	*/
	virtual void OnUserScheduleReceived(int32_t errorCode, int32_t requestId, std::vector<UserSchedule>& schedules) = 0;

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
									) = 0;

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
									) = 0;

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
											) = 0;

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
		) = 0;

	/**
	* 删除会议录像结果的返回事件
	* @param errorCode				错误码
	* @param eventId				会议事件ID
	* @param startTime				开始时间
	*/
	virtual void OnMeetingRecordingDeleted(int32_t errorCode, int64_t eventId, int64_t startTime = 0) = 0;

	/**
	* 会议提醒事件
	* @param meetings				需要提醒的会议信息列表
	*/
	virtual void OnMeetingAlarmed(std::vector<Meeting>& meetings) = 0;

	/**
	* 会议文件上传事件
	* @param meetings				需要提醒的会议信息列表
	*/
	virtual void OnMeetingFileUploaded(int32_t errorCode, int64_t eventId, int32_t operation, int32_t operatorId, const DocumentInfo& info) = 0;

	/**
	* 会议文件删除事件
	* @param meetings				需要提醒的会议信息列表
	*/
	virtual void OnMeetingFileDeleted(int32_t errorCode, int64_t minutesId, int32_t operatorId) = 0;

	/**
	 * 消息发送结果;
	 * @param result	     错误信息;
	 * @param groupId	     会议讨论组ID;
	 * @param eventId	     会议event ID;
	 * @param startTime	     会议开始时间;
	 * @param msgId          本地消息ID;
	 * @param msgSeq         服务器消息ID;
	 * @param msgTime        服务器消息发送时间;
	 */
	virtual void OnMessageSent(int32_t result, int64_t groupId, int64_t eventId, int64_t startTime, int32_t msgId, int64_t msgSeq, int64_t msgTime) = 0;

	/**
	* 接收文本聊天消息响应事件;
	* @param channel 消息来源
	* @param message 文本消息对象;
	*/
	virtual void OnTextMessageReceived(MessageChannel channel, const ConfTextMessageInfo *message) = 0;

	/**
	* 接收图片消息响应事件;
	* @param channel 消息来源
	* @param message 图片消息对象;
	*/
	virtual void OnImageMessageReceived(MessageChannel channel, const ConfMediaMessageInfo *message) = 0;

	/**
	* 接收音频消息响应事件;
	* @param channel 消息来源
	* @param message 音频消息对象;
	*/
	virtual void OnAudioMessageReceived(MessageChannel channel, const ConfMediaMessageInfo *message) = 0;

	/**
	* 接收视频消息响应事件;
	* @param channel 消息来源
	* @param message 视频消息对象;
	*/
	virtual void OnVideoMessageReceived(MessageChannel channel, const ConfMediaMessageInfo *message) = 0;

	/**
	* 接收报告消息响应事件;
	* @param channel 消息来源
	* @param message 报告消息对象;
	*/
	virtual void OnReportMessageReceived(MessageChannel channel, const ReportMessageInfo *message) = 0;

	/**
	* 接收会议视频录制消息响应事件;
	* @param channel 消息来源
	* @param message 视频消息对象;
	*/
	virtual void OnRecordVideoMessageReceived(MessageChannel channel, const ConfMediaMessageInfo *message) = 0;

	/**
	* 接收消息未读用户个数消息;
	* @param result  		 错误信息;
	* @param groupId         讨论组ID;
	* @param info            消息未读信息列表;
	*/
	virtual void OnMessageUnreadUserCount(int32_t result, int64_t groupId, const std::list<MessageReadInfo>& info) = 0;

	/**
	 * 接收到会议邀请的消息
	 * @param channel 消息来源
	 * @param message 消息
	 * @param conference 会议信息
	 */
	virtual void OnMeetingInviteMessageReceived(MessageChannel channel, const MessageInfo &message, const Meeting &meeting) = 0;

	/**
	 * 接收到会议更新的消息
	 * @param channel 消息来源
	 * @param message 消息
	 * @param meeting 会议信息
	 */
	virtual void OnMeetingUpdateMessageReceived(MessageChannel channel, const MessageInfo &message, const Meeting &meeting) = 0;

	/**
	 * 接收到会议转发的消息
	 * @param channel 消息来源
	 * @param message 消息
	 * @param meeting 会议信息
	 */
	virtual void OnMeetingForwardMessageReceived(MessageChannel channel, const MessageInfo &message, const Meeting &meeting) = 0;

	/**
	 * 接收到会议请求被接受的消息
	 * @param channel 消息来源
	 * @param message 消息
	 * @param meeting 会议信息
	 */
	virtual void OnMeetingAcceptMessageReceived(MessageChannel channel, const MessageInfo &message, const Meeting &meeting) = 0;

	/**
	 * 接收到拒绝会议的消息
	 * @param channel 消息来源
	 * @param message 消息
	 * @param meeting 会议信息
	 */
	virtual void OnMeetingDenyMessageReceived(MessageChannel channel, const MessageInfo &message, const Meeting &meeting) = 0;

	/**
	 * 接收到会议取消的消息
	 * @param channel 消息来源
	 * @param message 消息
	 * @param meeting 会议信息
	 */
	virtual void OnMeetingCancelMessageReceived(MessageChannel channel, const MessageInfo &message, const Meeting &meeting) = 0;

	/**
	 * 收到会议纪要创建的消息
	 * @param channel		消息来源
	 * @param message		消息
	 * @param operatorId	操作者ID
	 * @param updateTime	更新时间
	 * @param minutesId		会议纪要ID
	 * @param minutesTitle	会议纪要标题
	 * @int32_t state		会议纪要状态
	 * @param eventId		所属事件ID
	 * @param confName		会议名称
	 * @param startTime		所属事件开始时间
	 */
	virtual void OnMinutesCreateMessageReceived(MessageChannel channel, const MessageInfo &message, int32_t operatorId,
		int64_t updateTime, int64_t minutesId, const std::string &minutesTitle, int32_t state, int64_t eventId, const std::string &confName, int64_t startTime = 0) = 0;

	/**
	 * 接收到会议纪要更新的消息
	 * @param channel		消息来源
	 * @param message		消息
	 * @param operatorId	操作者ID
	 * @param updateTime	更新时间
	 * @param minutesId		会议纪要ID
	 * @param minutesTitle	会议纪要标题
	 * @int32_t state		会议纪要状态
	 * @param eventId		所属事件ID
	 * @param confName		会议名称
	 * @param startTime		所属事件开始时间
	 */
	virtual void OnMinutesUpdateMessageReceived(MessageChannel channel, const MessageInfo &message, int32_t operatorId,
		int64_t updateTime, int64_t minutesId, const std::string &minutesTitle, int32_t state, int64_t eventId, const std::string &confName, int64_t startTime = 0) = 0;

	/**
	* 接收到授权消息
	* @param channel 消息来源
	* @param message 消息
	*/
	virtual void OnAccreditMessageReceived(MessageChannel channel, const MessageInfo &message) = 0;

	/**
	* 接收到取消授权消息
	* @param channel 消息来源
	* @param message 消息
	*/
	virtual void OnAccreditCancelMessageReceived( MessageChannel channel, const MessageInfo &message) = 0;

	/**
	 *  接收到入会的通知消息
	 */
	virtual void OnPartJoinMessageReceived(MessageChannel channel, const MessageInfo &message) = 0;

	/**
	 * 接收到退出会议的通知消息
	 */
	virtual void OnPartCancelMessageReceived(MessageChannel channel, const MessageInfo &message) = 0;

	/**
	* 远程协助邀请、接受、拒绝消息发送完成事件
	*/
	virtual void OnRemoteAssistanceInviteMsgSent(int32_t result, const std::string& conferenceId, int32_t msgId, int64_t msgSeq, int64_t msgTime) = 0;
	virtual void OnRemoteAssistanceAcceptMsgSent(int32_t result, const std::string& conferenceId, int32_t msgId, int64_t msgSeq, int64_t msgTime) = 0;
	virtual void OnRemoteAssistanceRejectMsgSent(int32_t result, const std::string& conferenceId, int32_t msgId, int64_t msgSeq, int64_t msgTime) = 0;

	virtual void OnRemoteAssistanceInviteMsgReceived(MessageChannel channel, const MessageInfo &message, const std::string& confId, const std::string& confPwd) = 0;
	virtual void OnRemoteAssistanceAcceptMsgReceived(MessageChannel channel, const MessageInfo &message, const std::string& confId) = 0;
	virtual void OnRemoteAssistanceRejectMsgReceived(MessageChannel channel, const MessageInfo &message, const std::string& confId) = 0;
	virtual void OnRemoteAssistanceEndMsgReceived(MessageChannel channel, const MessageInfo &message, const std::string& confId) = 0;

	/**
	* 接收到文件上传的消息
	* @param channel
	* @param operation 0: create,  1: share
	* @param message
	* @param doc
	*/
	virtual void OnDocumentUploadMessageReceived(MessageChannel channel, int32_t operation, int64_t eventId, const MessageInfo &message, const DocumentInfo &doc) = 0;
	virtual void OnDocumentDeleteMessageReceived(MessageChannel channel, const MessageInfo &message, int64_t eventId, int64_t contentId, int32_t operatorId, const std::string &docName) = 0;

	virtual void OnMeetingRoomCreated(int32_t errorCode, MeetingRoom& room) = 0;
	virtual void OnMeetingRoomUpdated(int32_t errorCode, MeetingRoom& room) = 0;
	virtual void OnMeetingRoomListReceived(int32_t errorCode, int64_t orgId, std::list<MeetingRoom>& rooms) = 0;
	virtual void OnMeetingCreatedInMeetingRoom(int32_t errorCode, const std::string& roomId, const Meeting& meeting) = 0;
	virtual void OnCheckJoinMeetingRoom(int32_t errorCode, const std::string& roomId, int32_t hasPassword, int32_t isValid, int32_t mayJoin) = 0;
	virtual void OnCheckMeetingRoomPassword(int32_t errorCode, const std::string& roomId, const std::string& password, int32_t result) = 0;
	virtual void OnMeetingRoomDeleted(int32_t errorCode, const std::string& roomId) = 0;
	virtual void OnRightsChecked(int32_t errorCode) = 0;
	virtual void OnUserBusyFreeInfoReceived(int32_t errorCode, int32_t requestId, std::vector<UserBusyFreeInfo>& schedules) = 0;

	/**
	* 会议消息撤回事件
	* @param result				  回调结果
	* @param conferenceId         会议ID
	* @param msgSeq				  seqId
	* @param msgId				  消息ID
	* @param msgTime			  撤销消息时间
	* @param eventId			  会议的eventId
	*/
	virtual void OnMeetingMessageRevocation(int32_t result, int64_t msgSeq, int32_t msgId, int64_t msgTime, int64_t eventId) = 0;

	/**
	* 会议消息撤回事件接收
	* @param channel		消息来源
	* @param message        消息属性
	* @param eventId	    会议eventId
	* @param confId			会议ID
	* @param seqId			seq
	*/
	virtual void OnMeetingMessageRevocationRecevied(MessageChannel channel, const MessageInfo &message, int64_t eventId, int64_t seqId) = 0;
};
} /*namespace uc*/

#endif /* ICALENDARSERVICE_H_ */
