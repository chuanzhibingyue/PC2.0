/*
*  CalendarMsgProcessor.h
*
*  Created on: June 17, 2015
*      Author: changan wu
*/

#ifndef CALENDARMSGPROCESSOR_H_
#define CALENDARMSGPROCESSOR_H_

#include "CalendarData.h"
#include "UCMessageType.h"
#include "MessageInfo.h"

namespace uc {
	class CalendarService;

	//日程相关消息处理器
	class CalendarMsgProcessor {
	public:
		CalendarMsgProcessor(CalendarService* service) {
			_calendarService = service;
		}

		/**
		* 处理约会邀请消息
		* @param channel	消息来源
		* @param msg		消息
		*/
		void ProcessMeetingInviteMsg(MessageChannel channel, IQInviteContentMsg& msg, MsgSendState state);

		/**
		* 处理约会更新消息
		* @param msg			消息
		*/
		void ProcessMeetingUpdateMsg(MessageChannel channel, IQInviteContentMsg& msg, MsgSendState state);

		/**
		* 处理约会转发消息
		* @param msg			消息
		*/
		void ProcessMeetingForwardMsg(MessageChannel channel, IQInviteContentMsg& msg, MsgSendState state);

		/**
		* 处理接受会议邀请消息
		* @param msg			消息
		*/
		void ProcessMeetingAcceptMsg(MessageChannel channel, IQReceiveContentMsg& msg, MsgSendState state);

		/**
		* 处理拒绝会议邀请消息
		* @param msg			消息
		*/
		void ProcessMeetingDenyMsg(MessageChannel channel, IQRejectContentMsg& msg, MsgSendState state);

		/**
		* 处理会议取消消息
		* @param msg			消息
		*/
		void ProcessMeetingCancelMsg(MessageChannel channel, ConfCancelContentMsg& msg, MsgSendState state);

		/**
		* 处理会议纪要创建消息
		* @param msg			消息
		*/
		void ProcessMinutesCreateMsg(MessageChannel channel, ConfSummaryContentMsg& msg, MsgSendState state, bool& isSave);

		/**
		* 处理会议纪要更新消息
		* @param msg			消息
		*/
		void ProcessMinutesUpdateMsg(MessageChannel channel, ConfSummaryContentMsg& msg, MsgSendState state, bool& isSave);

		/**
		* 处理会议授权消息
		* @param msg			消息
		*/
		void ProcessAccreditMsg(MessageChannel channel, ConfShareContentMsg& msg, MsgSendState state);
		void ProcessAccreditCancelMsg(MessageChannel channel, ConfShareContentMsg& msg, MsgSendState state);

		void ProcessMeetingChatTextMsg(MessageChannel channel, ConfChatContentMsg& msg, MsgSendState state, bool& isSave, bool& saveUnread);

		void ProcessMeetingChatMediaMsg(MessageChannel channel, ConfChatContentMsg& msg, MsgSendState state, bool& isSave, bool& saveUnread);

		void ProcessMeetingChatReportMsg(MessageChannel channel, ReportContentMsg& msg, MsgSendState state, bool& isSave);

		void ProcessMeetingAlerPartJoinMsg(MessageChannel channel, ConfAlertContentMsg& msg, MsgSendState state);

		void ProcessMeetingAlerPartQuitMsg(MessageChannel channel, ConfAlertContentMsg& msg, MsgSendState state);

		//处理远程协助邀请、接受、拒绝、结束消息
		void ProcessRemoteAssistanceInviteMsg(MessageChannel channel, RemoteAssistanceInviteContentMsg& msg, MsgSendState state);
		void ProcessRemoteAssistanceAcceptMsg(MessageChannel channel, RemoteAssistanceAcceptContentMsg& msg, MsgSendState state);
		void ProcessRemoteAssistanceRejectMsg(MessageChannel channel, RemoteAssistanceRejectContentMsg& msg, MsgSendState state);
		void ProcessRemoteAssistanceEndMsg(MessageChannel channel, RemoteAssistanceEndContentMsg& msg, MsgSendState state, bool& bSave);

		//处理会议讨论组收到文件的消息
		void ProcessMeetingDocumentUploadMsg(MessageChannel channel, ConfUploadContentMsg& msg, MsgSendState state, bool& bSave, bool& saveUnread);
		void ProcessMeetingDocumentDeleteMsg(MessageChannel channel, ConfDelDocContentMsg& msg, MsgSendState state, bool& bSave, bool& saveUnread);

		void ProcessMeetingRevocationMsg(MessageChannel channel, UCHead head, ConfChatContentMsg& msg, MsgSendState state, bool& isSave);
		
	private:
		CalendarService* _calendarService;
	};
}/*namespace uc*/

#endif /*CALENDARMSGPROCESSOR_H_*/
