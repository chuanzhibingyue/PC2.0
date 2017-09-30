#ifndef UCPARSERDEF_H
#define UCPARSERDEF_H

#include "uc_types.h"

namespace uc {

enum MsgType {
	UCInvalidTMsgype = -1,
	UCNormalChatText,				//appid:1 prototype:1 protocolid:1 TextContentMsg
	UCNormalChatMedia,				//appid:1 prototype:1 protocolid:2 MediaContentMsg
	UCNormalChatReprot,	            //appid:1 prototype:1 protocolid:10 ReportContentMsg
	UCNormalChatDocumentSend,       //appid:1 prototype:1 protocolid:3 DocumentContentMsg
	UCNormalChatDocumentForward,    //appid:1 prototype:1 protocolid:4 DocumentContentMsg
	UCNormalChatDocumentDel,		//appid:1 prototype:1 protocolid:5 DocumentDelContentMsg
	UCNormalChatCodeCreate,         //appid:1 prototype:1 protocolid:6 CodeCreateContentMsg
	UCNormalChatCodeForward,        //appid:1 prototype:1 protocolid:7 CodeCreateContentMsg
	UCNormalChatCodeDel,            //appid:1 prototype:1 protocolid:8 CodeDelContentMsg
	UCNormalChatCloudFileCreate,	//appid:1 prototype:1 protocolid:9 CloudFileContentMsg
	UCNormalRevocationMessage,      //appid:1 prototype:1 protocolid:29 RevocationContentMsg
	UCDiscussionChatText,			//appid:1 prototype:2 protocolid:1 TextContentMsg
	UCDiscussionChatMedia,			//appid:1 prototype:2 protocolid:2 MediaContentMsg
	UCDiscussionChatReprot,	        //appid:1 prototype:2 protocolid:28 ReportContentMsg
	UCDiscussionMemberAdd,			//appid:1 prototype:2 protocolid:3 GroupMemberAddContentMsg
	UCDiscussionMemberDel,			//appid:1 prototype:2 protocolid:4 GroupMemberDelContentMsg
	UCDiscussionNameUpdate,			//appid:1 prototype:2 protocolid:5 GroupNameUpdateContentMsg
	UCDiscussionCreate,				//appid:1 prototype:2 protocolid:6 GroupCreateContentMsg
	UCDiscussionTop,				//appid:1 prototype:2 protocolid:7 GroupTopContentMsg
	UCDiscussionSummaryCreate,		//appid:1 prototype:2 protocolid:8 SummaryCreateContentMsg
	UCDiscussionDocumentForward,	//appid:1 prototype:2 protocolid:9 SummaryCreateContentMsg
	UCDiscussionAlertrule,			//appid:1 prototype:2 protocolid:10 AlertRuleContentMsg
	UCDiscussionClose,				//appid:1 prototype:2 protocolid:11 DiscussionCloseMsg
	UCDiscussionMsgNotifyUpdate,    //appid:1 prototype:2 protocolid:12 MsgNotifyContentMsg
	UCDiscussionMemberKick,			//appid:1 prototype:2 protocolid:13 GroupMemberKickContentMsg
	UCDiscussionDocumentDel,		//appid:1 prototype:2 protocolid:14 DocumentDelContentMsg
	UCDiscussionCodeCreate,         //appid:1 prototype:2 protocolid:15 CodeCreateContentMsg
	UCDiscussionCodeForward,        //appid:1 prototype:2 protocolid:16 CodeCreateContentMsg
	UCDiscussionCodeDel,            //appid:1 prototype:2 protocolid:17 CodeDelContentMsg
	UCDiscussionCommentCreate,      //appid:1 prototype:2 protocolid:18 CommentCreateContentMsg
	UCDiscussionCommentDel,         //appid:1 prototype:2 protocolid:19 CommentDelContentMsg
	UCDiscussionAvatarUpdate,		//appid:1 prototype:2 protocolid:20 GroupAvatarUpdateContentMsg
	UCDiscussionStateUpdate,		//appid:1 prototype:2 protocolid:21 GroupStateUpdateContentMsg
	UCDiscussionCloudFileCreate,	//appid:1 prototype:2 protocolid:22 CloudFileContentMsg
	UCDiscussionMemberRoleUpdate,	//appid:1 prototype:2 protocolid:27 MemberRoleUpdateContentMsg
	UCDiscussionRevocationMessage,  //appid:1 prototype:2 protocolid:29 RevocationContentMsg
	UCGroupChatText,				//appid:1 prototype:3 protocolid:1 TextContentMsg
	UCGroupChatMedia,               //appid:1 prototype:3 protocolid:2 MediaContentMsg
	UCGroupChatReprot,	            //appid:1 prototype:3 protocolid:28 ReportContentMsg
	UCGroupMemberAdd,				//appid:1 prototype:3 protocolid:3 GroupMemberAddContentMsg
	UCGroupMemberDel,				//appid:1 prototype:3 protocolid:4 GroupMemberDelContentMsg
	UCGroupNameUpdate,              //appid:1 prototype:3 protocolid:5 GroupNameUpdateContentMsg
	UCGroupCreate,					//appid:1 prototype:3 protocolid:6 GroupCreateContentMsg
	UCGroupTop,						//appid:1 prototype:3 protocolid:7 GroupTopContentMsg
	UCGroupSummaryCreate,			//appid:1 prototype:3 protocolid:8 SummaryCreateContentMsg
	UCGroupDocumentForward,			//appid:1 prototype:3 protocolid:9 SummaryCreateContentMsg
	UCGroupMsgNotifyUpdate,			//appid:1 prototype:3 protocolid:12 MsgNotifyContentMsg
	UCGroupMemberKick,				//appid:1 prototype:3 protocolid:13 GroupMemberKickContentMsg
	UCGroupDocumentDel,				//appid:1 prototype:3 protocolid:14 DocumentDelContentMsg
	UCGroupCodeCreate,				//appid:1 prototype:3 protocolid:15 CodeCreateContentMsg
	UCGroupCodeForward,				//appid:1 prototype:3 protocolid:16 CodeCreateContentMsg
	UCGroupCodeDel,					//appid:1 prototype:3 protocolid:17 CodeDelContentMsg
	UCGroupCommentCreate,			//appid:1 prototype:3 protocolid:18 CommentCreateContentMsg
	UCGroupCommentDel,				//appid:1 prototype:3 protocolid:19 CommentDelContentMsg
	UCGroupAvatarUpdate,			//appid:1 prototype:3 protocolid:20 GroupAvatarUpdateContentMsg
	UCGroupStateUpdate,				//appid:1 prototype:3 protocolid:21 GroupStateUpdateContentMsg
	UCGroupCloudFileCreate,			//appid:1 prototype:3 protocolid:22 CloudFileContentMsg
	UCGroupMemberRoleUpdate,		//appid:1 prototype:3 protocolid:27 MemberRoleUpdateContentMsg
	UCGroupRevocationMessage,       //appid:1 prototype:3 protocolid:29 RevocationContentMsg
	UCGroupClose,				    //appid:1 prototype:3 protocolid:11 GroupCloseMsg
	UCCloudDiscussionTextContent,						//appid:1 prototype:4 protocolid:1 TextContentMsg
	UCCloudDiscussionMediaContent,						//appid:1 prototype:4 protocolid:2 MediaContentMsg
	UCCloudDiscussionChatReprot,	                    //appid:1 prototype:4 protocolid:28 ReportContentMsg
	UCCloudDiscussionMemberAdd,							//appid:1 prototype:4 protocolid:3 GroupMemberAddContentMsg
	UCCloudDiscussionGroupMemberDel,					//appid:1 prototype:4 protocolid:4 GroupMemberDelContentMsg
	UCCloudDiscussionNameUpdate,						//appid:1 prototype:4 protocolid:5 GroupNameUpdateContentMsg
	UCCloudDiscussionCreate,							//appid:1 prototype:4 protocolid:6 GroupCreateContentMsg
	UCCloudDiscussionClose,								//appid:1 prototype:4 protocolid:11 DiscussionCloseMsg
	UCCloudDiscussionMsgNotifyUpdate,					//appid:1 prototype:4 protocolid:12 MsgNotifyContentMsg
	UCCloudDiscussionMemberKick,						//appid:1 prototype:4 protocolid:13 GroupMemberKickContentMsg
	UCCloudDiscussionCommentCreate,						//appid:1 prototype:4 protocolid:18 CommentCreateContentMsg
	UCCloudDiscussionCommentDel,						//appid:1 prototype:4 protocolid:19 CommentDelContentMsg
	UCCloudDiscussionAvatarUpdate,						//appid:1 prototype:4 protocolid:20 GroupAvatarUpdateContentMsg
	UCCloudDiscussionCloudFileCreate,					//appid:1 prototype:4 protocolid:22 CloudFileContentMsg
	UCCloudDiscussionCloudFileUpdate,					//appid:1 prototype:4 protocolid:23 CloudFileContentMsg
	UCCloudDiscussionCloudFileDel,						//appid:1 prototype:4 protocolid:24 CloudFileContentMsg
	UCCloudDiscussionCloudFileMove,						//appid:1 prototype:4 protocolid:25 CloudFileContentMsg
	UCCloudDiscussionCloudFileRename,					//appid:1 prototype:4 protocolid:26 CloudFileContentMsg
	UCCloudDiscussionMemberRoleUpdate,					//appid:1 prototype:4 protocolid:27 MemberRoleUpdateContentMsg
	UCCloudRevocationMessage,							//appid:1 prototype:4 protocolid:29 RevocationContentMsg
	UCHeartbeat,					//appid:2 prototype:1 protocolid:1 MessagePingMsg
	UCPresence,						//appid:3 prototype:1 protocolid:1 MessagePresenceMsg
	UCNotifySystemWelcome,			//appid:4 prototype:1 protocolid:1 SystemWelcomeContentMsg
	UCNotifyIphoneClientUpdate,     //appid:4 prototype:2 protocolid:1 ClientUpdateContentMsg
	UCNotifyAndroidClientUpdate,    //appid:4 prototype:2 protocolid:2 ClientUpdateContentMsg
	UCNotifyPCClientUpdate,			//appid:4 prototype:2 protocolid:3 ClientUpdateContentMsg
	UCNotifyTagAdd,					//appid:4 prototype:3 protocolid:1 CompleteInfoContentMsg
	UCNotifyTagUpdate,				//appid:4 prototype:3 protocolid:2 CompleteInfoContentMsg
	UCNotifyTagDel,					//appid:4 prototype:3 protocolid:3 TagDelContentMsg
	UCNotifyPwdRuleOutOfDate,		//appid:4 prototype:4 protocolid:1 PwdExpireContentMsg
	UCNotifyPwdRuleComplex,			//appid:4 prototype:4 protocolid:2 PwdComplexUpdateContentMsg
	UCNotifyMobileClientGuide,		//appid:4 prototype:5 protocolid:1 MobileClientGuideMsg
	UCNotifyPCClientGuide,			//appid:4 prototype:5 protocolid:2 PCClientGuideMsg
	UCNotifyPasswordModify,			//appid:4 prototype:6 protocolid:1 PasswordModifyContentMsg
	UCNotifySensitiveWordDelete,	//appid:4 prototype:7 protocolid:1 NullMsg
	UCNotifyGroupRemind,			//appid:4 prototype:8 protocolid:1 GroupRemindContentMsg
	UCFiletransferInvite,			//appid:5 prototype:1 protocolid:1 FileTransmissionInviteContentMsg
	UCFiletransferReceive,			//appid:5 prototype:1 protocolid:2 FileTransmissionReceiveContentMsg
	UCFiletransferReject,			//appid:5 prototype:1 protocolid:3 FileTransmissionRejectContentMsg
	UCFiletransferStop,				//appid:5 prototype:1 protocolid:4 FileTransmissionStopContentMsg
	UCCalendarConfShare,			//appid:6 prototype:7 protocolid:1 ConfShareContentMsg
	UCCalendarConfShareCancel,		//appid:6 prototype:7 protocolid:2 ConfShareContentMsg
	UCCalendarConfGrantorDisable,	//appid:6 prototype:7 protocolid:3 ConfShareContentMsg
	UCCalendarConfGranteeDisable,	//appid:6 prototype:7 protocolid:4 ConfShareContentMsg
	UCCalendarConfGranteeTransfer,	//appid:6 prototype:7 protocolid:5 ConfShareContentMsg
	UCCalendarConfGrantorLeave,		//appid:6 prototype:7 protocolid:6 ConfShareContentMsg
	UCCalendarConfGranteeLeave,		//appid:6 prototype:7 protocolid:7 ConfShareContentMsg
	UCMeetingInstantConfIQInvite,	//appid:7 prototype:1 protocolid:1 InstantIQInviteContentMsg
	UCMeetingInstantConfIQReceive,	//appid:7 prototype:1 protocolid:2 InstantIQReceiveContentMsg
	UCMeetingInstantConfIQReject,	//appid:7 prototype:1 protocolid:3 InstantIQRejectContentMsg
	UCMeetingConfInviteIQInvite,	//appid:7 prototype:2 protocolid:1 IQInviteContentMsg
	UCMeetingConfInviteIQReceive,	//appid:7 prototype:2 protocolid:2 IQReceiveContentMsg
	UCMeetingConfInviteIQReject,	//appid:7 prototype:2 protocolid:3 IQRejectContentMsg
	UCMeetingConfUpdateIQInvite,	//appid:7 prototype:3 protocolid:1 IQInviteContentMsg
	UCMeetingConfUpdateIQReceive,	//appid:7 prototype:3 protocolid:2 IQReceiveContentMsg
	UCMeetingConfUpdateIQReject,	//appid:7 prototype:3 protocolid:3 IQRejectContentMsg
	UCMeetingConfCancel,			//appid:7 prototype:4 protocolid:1 ConfCancelContentMsg
	UCMeetingRecurrentConfCancel,	//appid:7 prototype:4 protocolid:2 ConfCancelContentMsg
	UCMeetingConfForwardIQInvite,	//appid:7 prototype:5 protocolid:1 IQInviteContentMsg
	UCMeetingConfForwardIQReceive,	//appid:7 prototype:5 protocolid:2 IQReceiveContentMsg
	UCMeetingConfForwardIQReject,	//appid:7 prototype:5 protocolid:3 IQRejectContentMsg
	UCMeetingConfFifteenMinutesAlert,	//appid:7 prototype:6 protocolid:1 ConfAlertContentMsg
	UCMeetingConfZeroMinutesAlert,		//appid:7 prototype:6 protocolid:2 ConfAlertContentMsg
	UCMeetingConfAlertStart,			//appid:7 prototype:6 protocolid:3 ConfAlertContentMsg
	UCMeetingConfAlertEnd,				//appid:7 prototype:6 protocolid:4 ConfAlertContentMsg
	UCMeetingConfAlertPartJoin,			//appid:7 prototype:6 protocolid:5 ConfAlertContentMsg
	UCMeetingConfAlertPartQuit,			//appid:7 prototype:6 protocolid:6 ConfAlertContentMsg
// 	UCMeetingConfShare,					//appid:7 prototype:7 protocolid:1 ConfShareContentMsg
// 	UCMeetingConfShareCancel,			//appid:7 prototype:7 protocolid:2 ConfShareContentMsg
// 	UCMeetingConfShareGrantorDisable,	//appid:7 prototype:7 protocolid:3 ConfShareContentMsg
// 	UCMeetingConfShareGranteeDisable,	//appid:7 prototype:7 protocolid:4 ConfShareContentMsg
// 	UCMeetingConfShareGranteeTransfer,	//appid:7 prototype:7 protocolid:5 ConfShareContentMsg
// 	UCMeetingConfShareGrantorLeave,		//appid:7 prototype:7 protocolid:6 ConfShareContentMsg
// 	UCMeetingConfShareGranteeLeave,		//appid:7 prototype:7 protocolid:7 ConfShareContentMsg
	UCMeetingDiscussionInstantConfInviteIQInvite,	//appid:7 prototype:8 protocolid:1 InstantIQInviteContentMsg
	UCMeetingDiscussionInstantConfInviteIQReceive,	//appid:7 prototype:8 protocolid:2 InstantIQReceiveContentMsg
	UCMeetingDiscussionInstantConfInviteIQReject,	//appid:7 prototype:8 protocolid:3 InstantIQRejectContentMsg
	UCMeetingChatText,			//appid:7 prototype:9 protocolid:1 ConfChatContentMsg
	UCMeetingChatMedia,			//appid:7 prototype:9 protocolid:2 ConfChatContentMsg
	UCMeetingChatReprot,	    //appid:7 prototype:14 protocolid:1 ReportContentMsg
	UCMeetingSummaryCreate,		//appid:7 prototype:10 protocolid:1 ConfSummaryContentMsg
	UCMeetingSummaryUpdate,		//appid:7 prototype:10 protocolid:2 ConfSummaryContentMsg
	UCMeetingSummaryForward,	//appid:7 prototype:10 protocolid:3 ConfSummaryContentMsg
	UCMeetingCallState,			//appid:7 prototype:11 protocolid:1 CallStateContentMsg
	UCMeetingFileUpload,		//appid:7 prototype:12 protocolid:1 CreateConfUpload
	UCMeetingDelDoc,			//appid:7 prototype:13 protocolid:1 CreateConfDelDoc
	UCMeetingRevocationMessage, //appid:7 prototype:9  protocolid:3 RevocationContentMsg
	UCOrganizationDeptUpdate,		//appid:8 prototype:1 protocolid:1 DeptUpdateContentMsg
	UCOrganizationDeptTransfer,		//appid:8 prototype:1 protocolid:2 DeptTransferContentMsg
	UCOrganizationPositionUpdate,	//appid:8 prototype:1 protocolid:3 PositionUpdateContentMsg
	UCOrganizationEmployeeEntry,	//appid:8 prototype:1 protocolid:4 EmployeeEntryContentMsg
	UCOrganizationEmployeeLeave,	//appid:8 prototype:1 protocolid:5 EmployeeLeaveContentMsg
	UCOrganizationEmployeeRightsUpdate, //appid:8 prototype:1 protocolid:6 EmployeeRightsChangeContentMsg
	UCOrganizationDeptParentChange,		//appid:8 prototype:1 protocolid:8 DeptParentChangeContentMsg
	UCOrganizationCompanyJoin,			//appid:8 prototype:1 protocolid:9 CompanyJoinContentMsg
	UCOrganizationDeptDelete,			//appid:8 prototype:1 protocolid:10 DeptDeleteContentMsg
	UCOrganizationEmployeeEntryConfirm, //appid:8 prototype:1 protocolid:11 EmployeeEntryConfirmContentMsg
	UCOrganizationEmployeeLeaveConfirm, //appid:8 prototype:1 protocolid:12 EmployeeLeaveConfirmContentMsg
	UCOrganizationDeptTransferConfirm,  //appid:8 prototype:1 protocolid:13 DeptTransferConfirmContentMsg
	UCAckMessageSent,			//appid:10 prototype:1 protocolid:1 AckSentMsg
	UCAckMessageBatchSent,		//appid:10 prototype:2 protocolid:1 AckSentMsg
	UCAckMessageReceived,		//appid:10 prototype:1 protocolid:2 AckReceivedMsg
	UCAckMessageBatchReceived,	//appid:10 prototype:2 protocolid:2 AckReceivedMsg
	UCAckMessageRead,			//appid:10 prototype:1 protocolid:3 AckReadMsg
	UCAckMessageBatchRead,		//appid:10 prototype:2 protocolid:3 BatchAckReadMsg
	UCAckMessagePlay,			//appid:10 prototype:1 protocolid:4 AckPlayMsg
	UCAckMessageBatchPlay,		//appid:10 prototype:2 protocolid:4 AckPlayMsg
	UCAudioChatInvite,		//appid:11 prototype:1 protocolid:1 AudioInviteContentMsg
	UCAudioChatReceive,		//appid:11 prototype:1 protocolid:2 AudioReceiveContentMsg
	UCAudioChatReject,		//appid:11 prototype:1 protocolid:3 AudioRejectContentMsg
	UCAudioChatStop,		//appid:11 prototype:1 protocolid:4 AudioStopContentMsg
	UCAudioChatCancelCall,	//appid:11 prototype:1 protocolid:5 AudioCancelCallContentMsg
	UCAudioChatBlockCall,	//appid:11 prototype:1 protocolid:6 AudioBlockCallContentMsg
	UCAudioChatRejectCall,	//appid:11 prototype:1 protocolid:7 AudioRejectCallContentMsg
	UCAudioChatRecentCall,	//appid:11 prototype:1 protocolid:8 AudioRecentCallContentMsg
	UCAudioCallBlockCall,	//appid:11 prototype:2 protocolid:1 AudioBlockCallContentMsg
	UCAudioCallRejectCall,	//appid:11 prototype:2 protocolid:2 AudioRejectCallContentMsg
	UCAudioCallRecentCall,	//appid:11 prototype:2 protocolid:3 AudioRecentCallContentMsg
	UCAudioUnfamiliarBlockCall,		//appid:11 prototype:3 protocolid:1 AudioBlockCallContentMsg
	UCAudioUnfamiliarRejectCall,	//appid:11 prototype:3 protocolid:2 AudioRejectCallContentMsg
	UCAudioUnfamiliarRecentCall,	//appid:11 prototype:3 protocolid:3 AudioRecentCallContentMsg
	UCAudioDiscussionInvite,		//appid:11 prototype:4 protocolid:1 AudioInviteContentMsg
	UCAudioDiscussionReceive,		//appid:11 prototype:4 protocolid:2 AudioReceiveContentMsg
	UCAudioDiscussionReject,		//appid:11 prototype:4 protocolid:3 AudioRejectContentMsg
	UCAudioDiscussionStop,			//appid:11 prototype:4 protocolid:4 AudioStopContentMsg
	UCAudioDiscussionCancelCall,	//appid:11 prototype:4 protocolid:5 AudioCancelCallContentMsg
	UCAudioDiscussionBlockCall,		//appid:11 prototype:4 protocolid:6 AudioBlockCallContentMsg
	UCAudioDiscussionRejectCall,	//appid:11 prototype:4 protocolid:7 AudioRejectCallContentMsg
	UCAudioDiscussionRecentCall,	//appid:11 prototype:4 protocolid:8 AudioRecentCallContentMsg
	UCAudioGroupInvite,				//appid:11 prototype:5 protocolid:1 AudioInviteContentMsg
	UCAudioGroupReceive,			//appid:11 prototype:5 protocolid:2 AudioReceiveContentMsg
	UCAudioGroupReject,				//appid:11 prototype:5 protocolid:3 AudioRejectContentMsg
	UCAudioGroupStop,				//appid:11 prototype:5 protocolid:4 AudioStopContentMsg
	UCAudioGroupCancelCall,			//appid:11 prototype:5 protocolid:5 AudioCancelCallContentMsg
	UCAudioGroupBlockCall,			//appid:11 prototype:5 protocolid:6 AudioBlockCallContentMsg
	UCAudioGroupRejectCall,			//appid:11 prototype:5 protocolid:7 AudioRejectCallContentMsg
	UCAudioGroupRecentCall,			//appid:11 prototype:5 protocolid:8 AudioRecentCallContentMsg
	UCAddressBookCardUpdate,	//appid:12 prototype:1 protocolid:1 CardUpdateContentMsg
	UCAddressBookContacterAdd,  //appid:12 prototype:2 protocolid:1 ContacterAddContentMsg
	UCAddressBookContacterDel,	//appid:12 prototype:2 protocolid:2 ContacterDelContentMsg
	UCSleepIQRequest,     //appid:13 prototype:1 protocolid:1 SleepSetMsg
	UCSleepIQConfirm,	  //appid:13 prototype:1 protocolid:2 WakeupResultMsg
	UCTimeNewsAttention,  //appid:14 prototype:1 protocolid:1 WBMessageContentMsg
	UCTimeNewsPublish,	  //appid:14 prototype:1 protocolid:2 WBMessageContentMsg
	UCTimeNewsComment,	  //appid:14 prototype:1 protocolid:3 WBMessageContentMsg
	UCTimeNewsForward,	  //appid:14 prototype:1 protocolid:4 WBMessageContentMsg
	UCTimeNewsPraise,	  //appid:14 prototype:1 protocolid:5 WBMessageContentMsg
	UCRemoteAssistInvite,	//appid:17 prototype:1 protocolid:1 RemoteAssistanceInviteContentMsg
	UCRemoteAssistAccept,	//appid:17 prototype:1 protocolid:2 RemoteAssistanceAcceptContentMsg
	UCRemoteAssistReject,	//appid:17 prototype:1 protocolid:3 RemoteAssistanceRejectContentMsg
	UCRemoteAssistEnd,		//appid:17 prototype:1 protocolid:4 RemoteAssistanceEndContentMsg
	UCSessionTopSet,        //appid:18 prototype:1 protocolid:1 SessionTopContentMsg
	UCSessionTopUnset,      //appid:18 prototype:1 protocolid:2 SessionTopContentMsg
	UCAtMessageRead,      //appid:18 prototype:2 protocolid:16 AtMessageReadedContentMsg
	UCUserLoginNotify,    //appid:18 prototype:3 protocolid:32 UserLoginNotifyContentMsg
	UCAPIText,			  //appid:256 prototype:1 protocolid:1 APITextContentMsg
	UCAPIImage,			  //appid:256 prototype:1 protocolid:2 APIImageContentMsg
	UCAPIFile,			  //appid:256 prototype:1 protocolid:3 APIFileContentMsg
	UCAPIOA,              //appid:256 prototype:1 protocolid:4 APIOAContentMsg
	UCAPICustomizedContent, ////appid:256 prototype:1 protocolid:5 APICustomizedContentMsg
	UCAPIAck,			  //appid:256 prototype:2 protocolid:1 APIAckSentContentMsg
	UCAPIOAStatusChange,  //appid:256 prototype:3 protocolid:1 APIOAStatusChangeContentMsg
	UCAPIAppEventChange,  //appid:256 prototype:3 protocolid:3 APIAppEventChangeNotifyMsg
	UCAPIAppChangeNotify, //appid:256 prototype:3 protocolid:2 APIAppChangeNotifyContentMsg
	UCNullMessage		  //reserve
};

struct MsgRecord {
	UcMessageHead *head;
	UcMessageBody *body;

	MsgRecord() {
		head = NULL;
		body = NULL;
	}

	~MsgRecord() {
		if (head != NULL) {
			delete head;
			head = NULL;
		}

		if (body != NULL) {
			delete body;
			body = NULL;
		}
	}
};

struct BinMsg {
	char* _msg;
	int _msgLen;

	BinMsg():_msg(NULL), _msgLen(0) {
	}

	~BinMsg() {
		if (_msg != NULL) {
			delete []_msg;
			_msg = NULL;
		}
		_msgLen = 0;
	}
};

} //namespace uc

#endif //UCPARSERDEF_H