#include "CalendarMsgProcessor.h"
#include <string>
#include "common/event.h"
#include "common/StringConvert.h"
#include "CalendarService.h"
#include "LogManager.h"
#include "DBManager.h"
#include "MessageInfo.h"
#include "../message_service/MessageProcessor.h"

namespace uc {

	void CalendarMsgProcessor::ProcessMeetingInviteMsg(MessageChannel channel, IQInviteContentMsg &msg, MsgSendState state) {
	UCHead head;
	msg.GetHead(head);	
	UCID from = head.GetFrom();
	UCID to = head.GetTo();
	UCID toPrivate = head.GetPrivate();
	UCID curUser = _calendarService->GetCurUser();
	UC_LOG(INFO) << "ProcessMeetingInviteMsg, Message head info: { From: (userId:" << from.GetUserId()
		<< ", resId:" << from.GetResId() << "), To: (userId:" << to.GetUserId() << ", resId:"
		<< to.GetResId() << "),  _pri: " << head.GetPri() << ", _id: " << head.GetId() 
		<< ", _private: (userId:" << toPrivate.GetUserId() << ", resId:" << toPrivate.GetResId()
		<< ") }, Current User info: (userId:" << curUser.GetUserId() << ", resId:" << curUser.GetResId()
		<< ", channel: " << channel;

// 	if (from.GetResId() == curUser.GetResId()) {
// 		UC_LOG(INFO) << "ProcessMeetingInviteMsg return, FROM resId is equal to CURUSER resId.";
// 		return;
// 	}

	//解析会议信息
	Meeting meeting;
	meeting.SetConferenceId(msg.get_confId());
	meeting.SetHostId(msg.get_hostId());
	meeting.SetStartTime(msg.get_startTime());
	meeting.SetAuthorizerId(msg.get_shareUserID());
	meeting.SetGroupId(msg.get_groupID());
	meeting.IsCycle(msg.get_isRecurrent());
	meeting.SetEventId(msg.get_eventId());
	meeting.SetParentId(msg.get_parentEventId());
	meeting.SetOriTime(msg.get_oriStartTime());
	_calendarService->ParseICalendarStr(msg.get_icalendar(), meeting);

	if (UCAS == channel && (from.GetResId() != curUser.GetResId())) {
		DBManager::GetInstance().SaveMeeting(meeting);
	}

	MessageInfo message;
	message.InfoFromUCHead(head);
	message.SetSendState(state);
	triggerEventEx(ICalendarServiceEvent, *_calendarService, OnMeetingInviteMessageReceived(channel, message, meeting));
}

	void CalendarMsgProcessor::ProcessMeetingUpdateMsg(MessageChannel channel, IQInviteContentMsg &msg, MsgSendState state) {
	UCHead head;
	msg.GetHead(head);
	UCID from = head.GetFrom();
	UCID to = head.GetTo();
	UCID toPrivate = head.GetPrivate();
	UCID curUser = _calendarService->GetCurUser();
	UC_LOG(INFO) << "ProcessMeetingUpdateMsg, Message head info: { From: (userId:" << from.GetUserId()
		<< ", resId:" << from.GetResId() << "), To: (userId:" << to.GetUserId() << ", resId:"
		<< to.GetResId() << "),  _pri: " << head.GetPri() << ", _id: " << head.GetId()
		<< ", _private: (userId:" << toPrivate.GetUserId() << ", resId:" << toPrivate.GetResId()
		<< ") }, Current User info: (userId:" << curUser.GetUserId() << ", resId:" << curUser.GetResId()
		<< ", channel: " << channel;

// 	if (from.GetResId() == curUser.GetResId()) {
// 		UC_LOG(INFO) << "ProcessMeetingUpdateMsg return, FROM resId is equal to CURUSER resId.";
// 		return;
// 	}

	Meeting meeting;
	meeting.SetConferenceId(msg.get_confId());
	meeting.SetHostId(msg.get_hostId());
	meeting.SetStartTime(msg.get_startTime());
	meeting.SetAuthorizerId(msg.get_shareUserID());
	meeting.SetGroupId(msg.get_groupID());
	meeting.IsCycle(msg.get_isRecurrent());
	meeting.SetEventId(msg.get_eventId());
	meeting.SetParentId(msg.get_parentEventId());
	//meeting.SetOriTime(msg.get_oriStartTime());
	_calendarService->ParseICalendarStr(msg.get_icalendar(), meeting);
	
	if (UCAS == channel && (from.GetResId() != curUser.GetResId())) {
		DBManager::GetInstance().UpdateMeeting(meeting, msg.get_eventId(), msg.get_oriStartTime(), msg.get_shareUserID());
	}

	MessageInfo message;
	message.InfoFromUCHead(head);
	message.SetSendState(state);
	triggerEventEx(ICalendarServiceEvent, *_calendarService, OnMeetingUpdateMessageReceived(channel, message, meeting));
}

	void CalendarMsgProcessor::ProcessMeetingForwardMsg(MessageChannel channel, IQInviteContentMsg &msg, MsgSendState state) {
	UCHead head;
	msg.GetHead(head);
	UCID from = head.GetFrom();
	UCID to = head.GetTo();
	UCID toPrivate = head.GetPrivate();
	UCID curUser = _calendarService->GetCurUser();
	UC_LOG(INFO) << "ProcessMeetingForwardMsg, Message head info: { From: (userId:" << from.GetUserId()
		<< ", resId:" << from.GetResId() << "), To: (userId:" << to.GetUserId() << ", resId:"
		<< to.GetResId() << "),  _pri: " << head.GetPri() << ", _id: " << head.GetId()
		<< ", _private: (userId:" << toPrivate.GetUserId() << ", resId:" << toPrivate.GetResId()
		<< ") }, Current User info: (userId:" << curUser.GetUserId() << ", resId:" << curUser.GetResId()
		<< ", channel: " << channel;

// 	if (from.GetResId() == curUser.GetResId()) {
// 		UC_LOG(INFO) << "ProcessMeetingForwardMsg return, FROM resId is equal to CURUSER resId.";
// 		return;
// 	}

	Meeting meeting;
	meeting.SetConferenceId(msg.get_confId());
	meeting.SetHostId(msg.get_hostId());
	meeting.SetStartTime(msg.get_startTime());
	meeting.SetAuthorizerId(msg.get_shareUserID());
	meeting.SetGroupId(msg.get_groupID());
	meeting.IsCycle(msg.get_isRecurrent());
	meeting.SetEventId(msg.get_eventId());
	meeting.SetParentId(msg.get_parentEventId());
	meeting.SetOriTime(msg.get_oriStartTime());
	_calendarService->ParseICalendarStr(msg.get_icalendar(), meeting);

	if (UCAS == channel && (from.GetResId() != curUser.GetResId())) {
		DBManager::GetInstance().SaveMeeting(meeting);
	}

	MessageInfo message;
	message.InfoFromUCHead(head);
	message.SetSendState(state);
	triggerEventEx(ICalendarServiceEvent, *_calendarService, OnMeetingForwardMessageReceived(channel, message, meeting));
}

	void CalendarMsgProcessor::ProcessMeetingAcceptMsg(MessageChannel channel, IQReceiveContentMsg& msg, MsgSendState state) {
	UCHead head;
	msg.GetHead(head);
	UCID from = head.GetFrom();
	UCID to = head.GetTo();
	UCID toPrivate = head.GetPrivate();
	UCID curUser = _calendarService->GetCurUser();
	UC_LOG(INFO) << "ProcessMeetingAcceptMsg, Message head info: { From: (userId:" << from.GetUserId()
		<< ", resId:" << from.GetResId() << "), To: (userId:" << to.GetUserId() << ", resId:"
		<< to.GetResId() << "),  _pri: " << head.GetPri() << ", _id: " << head.GetId()
		<< ", _private: (userId:" << toPrivate.GetUserId() << ", resId:" << toPrivate.GetResId()
		<< ") }, Current User info: (userId:" << curUser.GetUserId() << ", resId:" << curUser.GetResId()
		<< ", channel: " << channel;

// 	if (from.GetResId() == curUser.GetResId()) {
// 		UC_LOG(INFO) << "ProcessMeetingAcceptMsg return, FROM resId is equal to CURUSER resId.";
// 		return;
// 	}

	Meeting meeting;
	meeting.SetConferenceId(msg.get_confId());
	meeting.SetHostId(msg.get_hostId());
	meeting.SetStartTime(msg.get_startTime());
	meeting.SetAuthorizerId(msg.get_shareUserID());
	meeting.SetEventId(msg.get_eventId());
	meeting.SetParentId(msg.get_parentEventId());
	meeting.SetOriTime(msg.get_oriStartTime());
	_calendarService->ParseICalendarStr(msg.get_icalendar(), meeting);

	if (UCAS == channel ) {
		if (from.GetResId() != curUser.GetResId() || to.GetUserId() ==  curUser.GetUserId()) {
			DBManager::GetInstance().UpdateAttendees(meeting.GetAttendees(), msg.get_eventId(), msg.get_startTime(), msg.get_shareUserID());
		}	
	}
	else {
		//接受的历史消息也得缓存，因为可以拒绝后再接收
		meeting.SetLastUpdateTime(head.GetTimestamp());
		_calendarService->CacheMeeting(meeting);
	}

	//不展示会议接受消息
	MessageInfo message;
	message.InfoFromUCHead(head);
	message.SetSendState(state);
	triggerEventEx(ICalendarServiceEvent, *_calendarService, OnMeetingAcceptMessageReceived(channel, message, meeting));
}

	void CalendarMsgProcessor::ProcessMeetingDenyMsg(MessageChannel channel, IQRejectContentMsg& msg, MsgSendState state) {
	UCHead head;
	msg.GetHead(head);
	UCID from = head.GetFrom();
	UCID to = head.GetTo();
	UCID toPrivate = head.GetPrivate();
	UCID curUser = _calendarService->GetCurUser();
	UC_LOG(INFO) << "ProcessMeetingDenyMsg, Message head info: { From: (userId:" << from.GetUserId()
		<< ", resId:" << from.GetResId() << "), To: (userId:" << to.GetUserId() << ", resId:"
		<< to.GetResId() << "),  _pri: " << head.GetPri() << ", _id: " << head.GetId()
		<< ", _private: (userId:" << toPrivate.GetUserId() << ", resId:" << toPrivate.GetResId()
		<< ") }, Current User info: (userId:" << curUser.GetUserId() << ", resId:" << curUser.GetResId()
		<< ", channel: " << channel;;

	Meeting meeting;
	meeting.SetConferenceId(msg.get_confId());
	meeting.SetHostId(msg.get_hostId());
	meeting.SetStartTime(msg.get_startTime());
	meeting.SetAuthorizerId(msg.get_shareUserID());
	meeting.SetEventId(msg.get_eventId());
	meeting.SetParentId(msg.get_parentEventId());
	meeting.SetOriTime(msg.get_oriStartTime());
	_calendarService->ParseICalendarStr(msg.get_icalendar(), meeting);

	//当前终端拒绝的消息，不用再作数据处理，只需通知上层在会话列表展示
	if (from.GetResId() == curUser.GetResId() && UCAS == channel && to.GetUserId() != curUser.GetUserId()) {
		UC_LOG(INFO) << "ProcessMeetingDenyMsg return, FROM resId is equal to CURUSER resId.";
		MessageInfo message;
		message.InfoFromUCHead(head);
		message.SetSendState(state);
		triggerEventEx(ICalendarServiceEvent, *_calendarService, OnMeetingDenyMessageReceived(channel, message, meeting));
		return;
	}

	if (UCAS == channel) {
		DBManager::GetInstance().UpdateAttendees(meeting.GetAttendees(), msg.get_eventId(), msg.get_startTime(), msg.get_shareUserID());
	}
	else {
		//缓存下来，用于conversionlist同步展示会议详情
		meeting.SetLastUpdateTime(head.GetTimestamp());
		_calendarService->CacheMeeting(meeting);
	}

	MessageInfo message;
	message.InfoFromUCHead(head);
	message.SetSendState(state);
	triggerEventEx(ICalendarServiceEvent, *_calendarService, OnMeetingDenyMessageReceived(channel, message, meeting));
}

	void CalendarMsgProcessor::ProcessMeetingCancelMsg(MessageChannel channel, ConfCancelContentMsg& msg, MsgSendState state) {
	UCHead head;
	msg.GetHead(head);
	UCID from = head.GetFrom();
	UCID to = head.GetTo();
	UCID toPrivate = head.GetPrivate();
	UCID curUser = _calendarService->GetCurUser();
	UC_LOG(INFO) << "ProcessMeetingCancelMsg, Message head info: { From: (userId:" << from.GetUserId()
		<< ", resId:" << from.GetResId() << "), To: (userId:" << to.GetUserId() << ", resId:"
		<< to.GetResId() << "),  _pri: " << head.GetPri() << ", _id: " << head.GetId()
		<< ", _private: (userId:" << toPrivate.GetUserId() << ", resId:" << toPrivate.GetResId()
		<< ") }, Current User info: (userId:" << curUser.GetUserId() << ", resId:" << curUser.GetResId()
		<< ", channel: " << channel;

// 	if (from.GetResId() == curUser.GetResId()) {
// 		UC_LOG(INFO) << "ProcessMeetingCancelMsg return, FROM resId is equal to CURUSER resId.";
// 		return;
// 	}

	Meeting meeting;
	meeting.SetConferenceId(msg.get_confId());
	meeting.SetStartTime(msg.get_startTime());
	meeting.SetAuthorizerId(msg.get_shareUserID());
	meeting.SetEventId(msg.get_eventId());
	meeting.SetParentId(msg.get_parentEventId());
	meeting.SetOriTime(msg.get_oriStartTime());
	_calendarService->ParseICalendarStr(msg.get_icalendar(), meeting);
	
	//屏蔽主持人或代理人踢人的消息
	if (toPrivate.GetUserId() > 0) {
		if (curUser.GetUserId() == meeting.GetHostId() || meeting.GetAuthorizerId() == meeting.GetHostId()) {
			return;
		}
	}
	
	//屏蔽发给会议讨论组其他人的取消消息
	if (to.GetUserId() != curUser.GetUserId() && toPrivate.GetUserId() != curUser.GetUserId() && meeting.GetStatus() != MeetingStatus::MS_CANCELED){
		return;
	}
	
	meeting.SetStatus(MeetingStatus::MS_CANCELED);
	if (from.GetResId() != curUser.GetResId()) {
		if (UCAS == channel) {
			DBManager::GetInstance().UpdateMeetingStatus(MeetingStatus::MS_CANCELED, msg.get_eventId(), msg.get_startTime(), msg.get_shareUserID());
			DBManager::GetInstance().UpdateAttendees(meeting.GetAttendees(), msg.get_eventId(), msg.get_startTime(), msg.get_shareUserID());
		}
		else {
			//缓存下来，用于conversionlist同步展示会议详情
			_calendarService->CacheMeeting(meeting);
		}
	}

	MessageInfo message;
	message.InfoFromUCHead(head);
	message.SetSendState(state);
	triggerEventEx(ICalendarServiceEvent, *_calendarService, OnMeetingCancelMessageReceived(channel, message, meeting));
}

void CalendarMsgProcessor::ProcessMinutesCreateMsg(MessageChannel channel, ConfSummaryContentMsg& msg, MsgSendState state, bool& isSave) {
	UCHead head;
	msg.GetHead(head);
	UCID from = head.GetFrom();
	UCID to = head.GetTo();
	UCID toPrivate = head.GetPrivate();
	UCID curUser = _calendarService->GetCurUser();
	UC_LOG(INFO) << "ProcessMinutesCreateMsg, Message head info: { From: (userId:" << from.GetUserId()
		<< ", resId:" << from.GetResId() << "), To: (userId:" << to.GetUserId() << ", resId:"
		<< to.GetResId() << "),  _pri: " << head.GetPri() << ", _id: " << head.GetId()
		<< ", _private: (userId:" << toPrivate.GetUserId() << ", resId:" << toPrivate.GetResId()
		<< ") }, Current User info: (userId:" << curUser.GetUserId() << ", resId:" << curUser.GetResId()
		<< ", channel: " << channel;

	isSave = true;
	if (UCAS == channel){
		head.SetId(DBManager::GetInstance().GetMessageID());
		msg.SetHead(head);
	}

	MessageInfo message;
	message.InfoFromUCHead(head);
	message.SetSendState(state);

	triggerEventEx(ICalendarServiceEvent, *_calendarService, OnMinutesCreateMessageReceived(channel, message,
		msg.get_operatorid(), msg.get_updateTime(), msg.get_summary_id(), msg.get_summary_name(), msg.get_state(),\
		msg.get_eventId(), msg.get_confName(), msg.get_startTime()));
}

void CalendarMsgProcessor::ProcessMinutesUpdateMsg(MessageChannel channel, ConfSummaryContentMsg& msg, MsgSendState state, bool& isSave) {
	
	UCHead head;
	msg.GetHead(head);
	UCID from = head.GetFrom();
	UCID to = head.GetTo();
	UCID toPrivate = head.GetPrivate();
	UCID curUser = _calendarService->GetCurUser();
	UC_LOG(INFO) << "ProcessMinutesUpdateMsg, Message head info: { From: (userId:" << from.GetUserId()
		<< ", resId:" << from.GetResId() << "), To: (userId:" << to.GetUserId() << ", resId:"
		<< to.GetResId() << "),  _pri: " << head.GetPri() << ", _id: " << head.GetId()
		<< ", _private: (userId:" << toPrivate.GetUserId() << ", resId:" << toPrivate.GetResId()
		<< ") }, Current User info: (userId:" << curUser.GetUserId() << ", resId:" << curUser.GetResId()
		<< ", channel: " << channel;

	isSave = true;
	if (UCAS == channel){
		head.SetId(DBManager::GetInstance().GetMessageID());
		msg.SetHead(head);
	}

	MessageInfo message;
	message.InfoFromUCHead(head);
	message.SetSendState(state);
	triggerEventEx(ICalendarServiceEvent, *_calendarService, OnMinutesUpdateMessageReceived(channel, message,
		msg.get_operatorid(), msg.get_updateTime(), msg.get_summary_id(), msg.get_summary_name(), msg.get_state(),
		msg.get_eventId(), msg.get_confName(), msg.get_startTime()));
}

void CalendarMsgProcessor::ProcessMeetingChatTextMsg(MessageChannel channel, ConfChatContentMsg& msg, MsgSendState state, bool& isSave, bool& saveUnread){

	UCHead head;
	msg.GetHead(head);
	isSave = false;
	if (UCAS == channel && _calendarService->IsCurUser(head.GetFrom())) {
		return;
	}

	if (UCAS == channel){
		head.SetId(DBManager::GetInstance().GetMessageID());
		msg.SetHead(head);
	}
	isSave = true;
	saveUnread = true;
	ConfTextMessageInfo messageInfo;

	messageInfo.InfoFromUCHead(head);
	messageInfo.SetConferenceId(msg.get_confId());
	messageInfo.SetEventId(msg.get_eventId());
	messageInfo.SetStartTime(msg.get_startTime());
	TextContent textConent = msg.get_text();
	messageInfo.SetTextType(textConent.type);
	messageInfo.SetTextContent(textConent.text);
	messageInfo.SetTextFormat(textConent.text_style);
	messageInfo.SetSendState(state);
	
	triggerEventEx(ICalendarServiceEvent, *_calendarService, OnTextMessageReceived(channel, &messageInfo));
}

void CalendarMsgProcessor::ProcessMeetingChatMediaMsg(MessageChannel channel, ConfChatContentMsg& msg, MsgSendState state, bool& isSave, bool& saveUnread){
	isSave = false;
	UCHead head;
	msg.GetHead(head);

	if (UCAS == channel && _calendarService->IsCurUser(head.GetFrom())) {
		return;
	}

	if (UCAS == channel){
		head.SetId(DBManager::GetInstance().GetMessageID());
		msg.SetHead(head);
	}
	isSave = true;
	saveUnread = true;
	ConfMediaMessageInfo messageInfo;
	messageInfo.InfoFromUCHead(head);
	messageInfo.SetEventId(msg.get_eventId());
	messageInfo.SetConferenceId(msg.get_confId());
	messageInfo.SetStartTime(msg.get_startTime());
	MediaContent mediaConent = msg.get_media();
	messageInfo.SetFileName(mediaConent.media_filename);
	messageInfo.SetFileSize(mediaConent.media_filesize);
	messageInfo.SetFileUrl(mediaConent.media_down_url);
	messageInfo.SetSendState(state);

	if (ChatMediaType::MediaTypeImage ==  mediaConent.media_type) {
		messageInfo.SetThumbnails(mediaConent.media_thumb);
		triggerEventEx(ICalendarServiceEvent, *_calendarService, OnImageMessageReceived(channel, &messageInfo));

	} else if(ChatMediaType::MediaTypeAudio ==  mediaConent.media_type) {
		messageInfo.SetDuration(mediaConent.media_duration);
		triggerEventEx(ICalendarServiceEvent, *_calendarService, OnAudioMessageReceived(channel, &messageInfo));

	} else if(ChatMediaType::MediaTypeVideo == mediaConent.media_type) {
		messageInfo.SetThumbnails(mediaConent.media_thumb);
		messageInfo.SetDuration(mediaConent.media_duration);
		triggerEventEx(ICalendarServiceEvent, *_calendarService, OnVideoMessageReceived(channel, &messageInfo));
	}
	else if (ChatMediaType::MediaTypeMeetingRecord == mediaConent.media_type) {
		//会议视频消处处理
		messageInfo.SetThumbnails(mediaConent.media_thumb);
		messageInfo.SetDuration(mediaConent.media_duration);
		messageInfo.SetOperatorId(mediaConent.operatorId);
		messageInfo.SetOperatorName(mediaConent.operatorName);
		messageInfo.SetOperatorAvatar(mediaConent.operatorAvatar);
		messageInfo.SetThumbnailsURL(mediaConent.media_thumb_url);
		triggerEventEx(ICalendarServiceEvent, *_calendarService, OnRecordVideoMessageReceived(channel, &messageInfo));

	}
}


void CalendarMsgProcessor::ProcessMeetingChatReportMsg(MessageChannel channel, ReportContentMsg& msg, MsgSendState state, bool& isSave){

	isSave = false;
	UCHead head;
	msg.GetHead(head);
	if (UCAS == channel && _calendarService->IsCurUser(head.GetFrom())) {
		return;
	}

	if (UCAS == channel)
	{
		head.SetId(DBManager::GetInstance().GetMessageID());
		msg.SetHead(head);
	}


	isSave = true;
	ReportMessageInfo messageInfo;

	messageInfo.InfoFromUCHead(head);
	messageInfo.SetReportTitle(msg.get_reportTitle());
	messageInfo.SetReportUrl(msg.get_reportUrl());
	messageInfo.SetEventId(msg.get_eventId());
	messageInfo.SetExternalData(msg.get_externalData());
	messageInfo.SetSendState(state);

	UC_LOG(INFO) << "CalendarMsgProcessor OnReportMessageNotify. channel: " << channel << " from_userid: " << head.GetFrom().GetUserId() <<
		" to_userid: " << head.GetTo().GetUserId();

	triggerEventEx(ICalendarServiceEvent, *_calendarService, OnReportMessageReceived(channel, &messageInfo));
}

void CalendarMsgProcessor::ProcessAccreditMsg(MessageChannel channel, ConfShareContentMsg& msg, MsgSendState state) {
	
	UCHead head;
	msg.GetHead(head);
	UCID from = head.GetFrom();
	UCID to = head.GetTo();
	UCID toPrivate = head.GetPrivate();
	UCID curUser = _calendarService->GetCurUser();
	UC_LOG(INFO) << "ProcessAccreditMsg, Message head info: { From: (userId:" << from.GetUserId()
		<< ", resId:" << from.GetResId() << "), To: (userId:" << to.GetUserId() << ", resId:"
		<< to.GetResId() << "),  _pri: " << head.GetPri() << ", _id: " << head.GetId()
		<< ", _private: (userId:" << toPrivate.GetUserId() << ", resId:" << toPrivate.GetResId()
		<< ") }, Current User info: (userId:" << curUser.GetUserId() << ", resId:" << curUser.GetResId();

	MessageInfo message;
	message.InfoFromUCHead(head);
	message.SetSendState(state);
	triggerEventEx(ICalendarServiceEvent, *_calendarService, OnAccreditMessageReceived(channel, message));
}

void CalendarMsgProcessor::ProcessAccreditCancelMsg(MessageChannel channel, ConfShareContentMsg& msg, MsgSendState state) {
	
	UCHead head;
	msg.GetHead(head);
	UCID from = head.GetFrom();
	UCID to = head.GetTo();
	UCID toPrivate = head.GetPrivate();
	UCID curUser = _calendarService->GetCurUser();
	UC_LOG(INFO) << "ProcessAccreditCancelMsg, Message head info: { From: (userId:" << from.GetUserId()
		<< ", resId:" << from.GetResId() << "), To: (userId:" << to.GetUserId() << ", resId:"
		<< to.GetResId() << "),  _pri: " << head.GetPri() << ", _id: " << head.GetId()
		<< ", _private: (userId:" << toPrivate.GetUserId() << ", resId:" << toPrivate.GetResId()
		<< ") }, Current User info: (userId:" << curUser.GetUserId() << ", resId:" << curUser.GetResId();

	MessageInfo message;
	message.InfoFromUCHead(head);
	message.SetSendState(state);
	triggerEventEx(ICalendarServiceEvent, *_calendarService, OnAccreditCancelMessageReceived(channel, message));
}

void CalendarMsgProcessor::ProcessMeetingAlerPartJoinMsg(MessageChannel channel, ConfAlertContentMsg& msg, MsgSendState state) {
	MessageInfo message;
	UCHead head;

	msg.GetHead(head);
	message.InfoFromUCHead(head);
	message.SetSendState(state);
	triggerEventEx(ICalendarServiceEvent, *_calendarService, OnPartJoinMessageReceived(channel, message));
}

void CalendarMsgProcessor::ProcessMeetingAlerPartQuitMsg(MessageChannel channel, ConfAlertContentMsg& msg, MsgSendState state) {
	MessageInfo message;
	UCHead head;

	msg.GetHead(head);
	message.InfoFromUCHead(head);
	message.SetSendState(state);
	triggerEventEx(ICalendarServiceEvent, *_calendarService, OnPartCancelMessageReceived(channel, message));
}

void CalendarMsgProcessor::ProcessRemoteAssistanceInviteMsg(MessageChannel channel, RemoteAssistanceInviteContentMsg& msg, MsgSendState state) {
	UCHead head;
	msg.GetHead(head);

	int32_t from = head.GetFrom().GetUserId();
	int32_t to = head.GetTo().GetUserId();
	int32_t curUserId = _calendarService->GetUserId();
	std::string confId = msg.get_confId();
	std::string confPwd = msg.get_confPwd();
	UC_LOG(INFO) << "ProcessRemoteAssistanceInviteMsg, from: " << from
		<< ", to: " << to << ", curUserId:" << curUserId << ", confId: " << confId << ", confPwd: " << confPwd;

	MessageInfo message;
	message.InfoFromUCHead(head);
	message.SetSendState(state);
	triggerEventEx(ICalendarServiceEvent, *_calendarService, OnRemoteAssistanceInviteMsgReceived(channel, message, confId, confPwd));
}

void CalendarMsgProcessor::ProcessRemoteAssistanceAcceptMsg(MessageChannel channel, RemoteAssistanceAcceptContentMsg& msg, MsgSendState state) {
	UCHead head;
	msg.GetHead(head);

	int32_t from = head.GetFrom().GetUserId();
	int32_t to = head.GetTo().GetUserId();
	int32_t curUserId = _calendarService->GetUserId();
	std::string confId = msg.get_confId();
	UC_LOG(INFO) << "ProcessRemoteAssistanceAcceptMsg, from: " << from
		<< ", to: " << to << ", curUserId:" << curUserId << ", confId: " << confId;

	MessageInfo message;
	message.InfoFromUCHead(head);
	message.SetSendState(state);
	triggerEventEx(ICalendarServiceEvent, *_calendarService, OnRemoteAssistanceAcceptMsgReceived(channel, message, confId));
}

void CalendarMsgProcessor::ProcessRemoteAssistanceRejectMsg(MessageChannel channel, RemoteAssistanceRejectContentMsg& msg, MsgSendState state) {
	UCHead head;
	msg.GetHead(head);

	int32_t from = head.GetFrom().GetUserId();
	int32_t to = head.GetTo().GetUserId();
	int32_t curUserId = _calendarService->GetUserId();
	std::string confId = msg.get_confId();
	UC_LOG(INFO) << "ProcessRemoteAssistanceRejectMsg, from: " << from
		<< ", to: " << to << ", curUserId:" << curUserId << ", confId: " << confId;

	MessageInfo message;
	message.InfoFromUCHead(head);
	message.SetSendState(state);
	triggerEventEx(ICalendarServiceEvent, *_calendarService, OnRemoteAssistanceRejectMsgReceived(channel, message, confId));
}

void CalendarMsgProcessor::ProcessRemoteAssistanceEndMsg(MessageChannel channel, RemoteAssistanceEndContentMsg& msg, MsgSendState state, bool& bSave) {
	UCHead head;
	msg.GetHead(head);
	
	if (UCAS == channel){
		bSave = true;
		head.SetId(DBManager::GetInstance().GetMessageID());
		msg.SetHead(head);
	}

	int32_t from = head.GetFrom().GetUserId();
	int32_t to = head.GetTo().GetUserId();
	int32_t curUserId = _calendarService->GetUserId();
	std::string confId = msg.get_confId();
	UC_LOG(INFO) << "ProcessRemoteAssistanceEndMsg, from: " << from
		<< ", to: " << to << ", curUserId:" << curUserId << ", confId: " << confId;

	MessageInfo message;
	message.InfoFromUCHead(head);
	message.SetSendState(state);
	triggerEventEx(ICalendarServiceEvent, *_calendarService, OnRemoteAssistanceEndMsgReceived(channel, message, confId));
}

void CalendarMsgProcessor::ProcessMeetingDocumentUploadMsg(MessageChannel channel, ConfUploadContentMsg& msg, MsgSendState state, bool& isSave, bool& saveUnread) {
	isSave = true;
	saveUnread = true;
	UCHead head;
	msg.GetHead(head);
	if (UCAS == channel)
	{
		head.SetId(DBManager::GetInstance().GetMessageID());
		msg.SetHead(head);
	}

	if (UCAS == channel && _calendarService->IsCurUser(head.GetFrom())) {
		return;
	}

	DocumentInfo docInfo;
	docInfo.SetFileId(msg.get_contentId());
	docInfo.SetCreatorId(msg.get_operatorid());
	docInfo.SetFileUrl(msg.get_down_url());
	docInfo.SetFileSize(msg.get_size());
	docInfo.SetEventId(msg.get_eventId());
	docInfo.SetContentType(0);
	std::string fileName = msg.get_doc_name();
	docInfo.SetFileName(fileName);
	std::string fileFormat = "";
	std::size_t nPos = fileName.find_last_of(".");
	if (nPos != string::npos){
		fileFormat = fileName.substr(nPos + 1);
	}
	docInfo.SetFileFormat(fileFormat);

	//检测文件是否被下载过
	std::string localPath{ "" };
	std::string fileurl{ "" };
	int64_t fileSize{ 0 };
	fileurl = MessageProcessor::RetrieveNoUserDownloadURL(docInfo.GetFileUrl());
	if (UC_SUCCEEDED == MessageProcessor::_GetLocalFilePathAndSize(fileurl, localPath, fileSize) &&
		UC_SUCCEEDED == MessageProcessor::CheckFile(localPath, fileSize)){
		docInfo.SetFileLocalPath(localPath);
	}
	else
		docInfo.SetFileLocalPath("");

	MessageInfo	message;

	message.InfoFromUCHead(head);
	message.SetSendState(state);

	triggerEventEx(ICalendarServiceEvent, *_calendarService, OnDocumentUploadMessageReceived(channel, 0, msg.get_eventId(), message, docInfo));

	UC_LOG(INFO) << "calendar_service. OnDocumentUploadMessageReceived. filename: " << docInfo.GetFileName()
		<< ", fileURL: " << docInfo.GetFileUrl() << ", filesize: " << docInfo.GetFileSize()
		<< ", fileId: " << docInfo.GetFileId() << ", creator: " << docInfo.GetCreatorId()
		<< ", eventId: " << docInfo.GetEventId()<<",localpath:"<<docInfo.GetFileLocalPath();

	//boost::unique_lock<boost::recursive_mutex> lock(_documentMutex);
	//AddDocument(head.GetTo().GetUserId(), docInfo);

}

void CalendarMsgProcessor::ProcessMeetingDocumentDeleteMsg(MessageChannel channel, ConfDelDocContentMsg& msg, MsgSendState state, bool& isSave, bool& saveUnread)
{
	isSave = true;
	UCHead head;
	msg.GetHead(head);
	if (UCAS == channel)
	{
		head.SetId(DBManager::GetInstance().GetMessageID());
		msg.SetHead(head);
	}

	if (UCAS == channel && _calendarService->IsCurUser(head.GetFrom())) {
		return;
	}

	MessageInfo message;

	message.InfoFromUCHead(head);
	message.SetSendState(state);

	triggerEventEx(ICalendarServiceEvent, *_calendarService, OnDocumentDeleteMessageReceived(channel, message, msg.get_eventId(), msg.get_contentId(), msg.get_operatorid(), msg.get_doc_name()));
	UC_LOG(INFO) << "calendar_service. OnDocumentDeleteMessageReceived. filename: " << msg.get_doc_name()
		<< ", eventId: " << msg.get_eventId() << ", contentId: " << msg.get_contentId()
		<< ", operatorId: " << msg.get_operatorid();
}

void CalendarMsgProcessor::ProcessMeetingRevocationMsg(MessageChannel channel, UCHead head, ConfChatContentMsg& msg, MsgSendState state, bool& isSave){
	isSave = true;
	if (UCAS == channel){
		head.SetId(DBManager::GetInstance().GetMessageID());
		msg.SetHead(head);
	}

	if (UCAS == channel && _calendarService->IsCurUser(head.GetFrom())){
		return;
	}

	MessageInfo message;
	message.InfoFromUCHead(head);
	message.SetSendState(state);

	if (head.GetFrom().GetUserId() != _calendarService->GetCurUser().GetUserId() && channel == UCAS)
	{
		ConversationType conversation_type = MessageProcessor::GetConversationType(head.GetAppId(), head.GetProtocoltype());
		int64_t relate_id = MessageProcessor::GetRelateId(conversation_type, _calendarService->GetCurUser().GetUserId(), head.GetFrom().GetUserId(), head.GetTo().GetUserId());
		theStore.GetMsgStore()->UpdateRevocationMessageState(relate_id, msg.get_revocation().seq, conversation_type);
	}

	triggerEventEx(ICalendarServiceEvent, *_calendarService, OnMeetingMessageRevocationRecevied(channel, message, msg.get_eventId(), msg.get_revocation().seq));

}

};/*namespace uc*/
