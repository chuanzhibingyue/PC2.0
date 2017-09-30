#include "AppMessageType.h"
#include "LogManager.h"

namespace uc {

AppMsgType& AppMsgType::Instance() {
	static AppMsgType inst;
	return inst;
}

AppMsgType::AppMsgType() {
	Init();
}

void AppMsgType::Init() {
	InitTimeNews();
	InitSleep();
	InitAddressbook();
	InitAudio();
	InitAck();
	InitOrganization();
	InitMeeting();
	InitCalendar();
	InitFiletransfer();
	InitNotify();
	InitPresence();
	InitHeartbeat();
	InitChat();
	InitAPI();
	InitAssist();
	InitSyncMessage();
}

void AppMsgType::InitChat() {
	//////////////////////////////单人聊天//////////////////////////////
	MsgInst type;
	type.msgId   = ChatMessageId::TextContent;
	type.type    = UCNormalChatText;
	type.func    = CreateMsg::CreateText;
	_appMsgMap[AppId::AppChat][ChatMessageType::NormalChat].push_back(type);

	type.msgId   = ChatMessageId::MediaContent;
	type.type	 = UCNormalChatMedia;
	type.func    = CreateMsg::CreateMedia;
	_appMsgMap[AppId::AppChat][ChatMessageType::NormalChat].push_back(type);

	type.msgId   = ChatMessageId::DocumentSend;
	type.type	 = UCNormalChatDocumentSend;
	type.func    = CreateMsg::CreateDocument;
	_appMsgMap[AppId::AppChat][ChatMessageType::NormalChat].push_back(type);

	type.msgId   = ChatMessageId::DocumentForward;
	type.type	 = UCNormalChatDocumentForward;
	type.func    = CreateMsg::CreateDocument;
	_appMsgMap[AppId::AppChat][ChatMessageType::NormalChat].push_back(type);

	type.msgId   = ChatMessageId::DocumentDel;
	type.type	 = UCNormalChatDocumentDel;
	type.func    = CreateMsg::CreateDocumentDel;
	_appMsgMap[AppId::AppChat][ChatMessageType::NormalChat].push_back(type);

	type.msgId   = ChatMessageId::CodeCreate;
	type.type	 = UCNormalChatCodeCreate;
	type.func    = CreateMsg::CreateCodeCreate;
	_appMsgMap[AppId::AppChat][ChatMessageType::NormalChat].push_back(type);

	type.msgId   = ChatMessageId::CodeForward;
	type.type	 = UCNormalChatCodeForward;
	type.func    = CreateMsg::CreateCodeCreate;
	_appMsgMap[AppId::AppChat][ChatMessageType::NormalChat].push_back(type);

	type.msgId   = ChatMessageId::CodeDel;
	type.type	 = UCNormalChatCodeDel;
	type.func    = CreateMsg::CreateCodeDel;
	_appMsgMap[AppId::AppChat][ChatMessageType::NormalChat].push_back(type);

	type.msgId = ChatMessageId::CloudFileCreate;
	type.type = UCNormalChatCloudFileCreate;
	type.func = CreateMsg::CreateCloudFileContent;
	_appMsgMap[AppId::AppChat][ChatMessageType::NormalChat].push_back(type);

	type.msgId = ChatMessageId::ReportContent;
	type.type = UCNormalChatReprot;
	type.func = CreateMsg::CreateReport;
	_appMsgMap[AppId::AppChat][ChatMessageType::NormalChat].push_back(type);

	type.msgId = ChatMessageId::RevocationMsg;
	type.type = UCNormalRevocationMessage;
	type.func = CreateMsg::CreateRevocation;
	_appMsgMap[AppId::AppChat][ChatMessageType::NormalChat].push_back(type);

	////////////////////////////讨论组聊天///////////////////////////////
	type.msgId   = GroupMessageId::TextContent;
	type.type    = UCDiscussionChatText;
	type.func    = CreateMsg::CreateText;
	_appMsgMap[AppId::AppChat][ChatMessageType::DiscussionChat].push_back(type);

	type.msgId   = GroupMessageId::MediaContent;
	type.type    = UCDiscussionChatMedia;
	type.func    = CreateMsg::CreateMedia;
	_appMsgMap[AppId::AppChat][ChatMessageType::DiscussionChat].push_back(type);

	type.msgId = GroupMessageId::ReportContent;
	type.type = UCDiscussionChatReprot;
	type.func = CreateMsg::CreateReport;
	_appMsgMap[AppId::AppChat][ChatMessageType::DiscussionChat].push_back(type);

	type.msgId   = GroupMessageId::DisMemberAdd;
	type.type    = UCDiscussionMemberAdd;
	type.func    = CreateMsg::CreateGroupAdd;
	_appMsgMap[AppId::AppChat][ChatMessageType::DiscussionChat].push_back(type);

	type.msgId   = GroupMessageId::DisMemberDel;
	type.type    = UCDiscussionMemberDel;
	type.func    = CreateMsg::CreateGroupDel;
	_appMsgMap[AppId::AppChat][ChatMessageType::DiscussionChat].push_back(type);

	type.msgId   = GroupMessageId::DisNameUpdate;
	type.type    = UCDiscussionNameUpdate;
	type.func    = CreateMsg::CreateGroupNameUpdate;
	_appMsgMap[AppId::AppChat][ChatMessageType::DiscussionChat].push_back(type);

	type.msgId   = GroupMessageId::DiscussionCreate;
	type.type    = UCDiscussionCreate;
	type.func    = CreateMsg::CreateGroupCreate;
	_appMsgMap[AppId::AppChat][ChatMessageType::DiscussionChat].push_back(type);

	type.msgId   = GroupMessageId::DiscussionTop;
	type.type    = UCDiscussionTop;
	type.func    = CreateMsg::CreateGroupTop;
	_appMsgMap[AppId::AppChat][ChatMessageType::DiscussionChat].push_back(type);

	type.msgId   = GroupMessageId::SummaryCreate;
	type.type    = UCDiscussionSummaryCreate;
	type.func    = CreateMsg::CreateSummaryCreate;
	_appMsgMap[AppId::AppChat][ChatMessageType::DiscussionChat].push_back(type);

	type.msgId   = GroupMessageId::DocumentForward;
	type.type    = UCDiscussionDocumentForward;
	type.func = CreateMsg::CreateSummaryCreate;
	_appMsgMap[AppId::AppChat][ChatMessageType::DiscussionChat].push_back(type);

	type.msgId   = GroupMessageId::AlertRuleUpdate;
	type.type    = UCDiscussionAlertrule;
	type.func    = CreateMsg::CreateAlertrule;
	_appMsgMap[AppId::AppChat][ChatMessageType::DiscussionChat].push_back(type);

	type.msgId   = GroupMessageId::DiscussionClose;
	type.type    = UCDiscussionClose;
	type.func    = CreateMsg::CreateDiscussionClose;
	_appMsgMap[AppId::AppChat][ChatMessageType::DiscussionChat].push_back(type);

	type.msgId   = GroupMessageId::MsgNotifyUpdate;
	type.type    = UCDiscussionMsgNotifyUpdate;
	type.func    = CreateMsg::CreateMsgNotify;
	_appMsgMap[AppId::AppChat][ChatMessageType::DiscussionChat].push_back(type);

	type.msgId   = GroupMessageId::DisMemberKick;
	type.type    = UCDiscussionMemberKick;
	type.func    = CreateMsg::CreateGroupMemberKick;
	_appMsgMap[AppId::AppChat][ChatMessageType::DiscussionChat].push_back(type);

	type.msgId   = GroupMessageId::DocumentDel;
	type.type	 = UCDiscussionDocumentDel;
	type.func    = CreateMsg::CreateDocumentDel;
	_appMsgMap[AppId::AppChat][ChatMessageType::DiscussionChat].push_back(type);

	type.msgId   = GroupMessageId::CodeCreate;
	type.type	 = UCDiscussionCodeCreate;
	type.func    = CreateMsg::CreateCodeCreate;
	_appMsgMap[AppId::AppChat][ChatMessageType::DiscussionChat].push_back(type);

	type.msgId   = GroupMessageId::CodeForward;
	type.type	 = UCDiscussionCodeForward;
	type.func    = CreateMsg::CreateCodeCreate;
	_appMsgMap[AppId::AppChat][ChatMessageType::DiscussionChat].push_back(type);

	type.msgId   = GroupMessageId::CodeDel;
	type.type	 = UCDiscussionCodeDel;
	type.func    = CreateMsg::CreateCodeDel;
	_appMsgMap[AppId::AppChat][ChatMessageType::DiscussionChat].push_back(type);

	type.msgId   = GroupMessageId::CommentCreate;
	type.type	 = UCDiscussionCommentCreate;
	type.func    = CreateMsg::CreateComment;
	_appMsgMap[AppId::AppChat][ChatMessageType::DiscussionChat].push_back(type);

	type.msgId   = GroupMessageId::CommentDel;
	type.type	 = UCDiscussionCommentDel;
	type.func    = CreateMsg::CreateDelComment;
	_appMsgMap[AppId::AppChat][ChatMessageType::DiscussionChat].push_back(type);
	
	type.msgId   = GroupMessageId::DisAvatarUpdate;
	type.type    = UCDiscussionAvatarUpdate;
	type.func    = CreateMsg::CreateGroupAvatarUpdate;
	_appMsgMap[AppId::AppChat][ChatMessageType::DiscussionChat].push_back(type);

	type.msgId   = GroupMessageId::DisStateUpdate;
	type.type    = UCDiscussionStateUpdate;
	type.func    = CreateMsg::CreateGroupStateUpdate;
	_appMsgMap[AppId::AppChat][ChatMessageType::DiscussionChat].push_back(type);

	type.msgId = GroupMessageId::CloudFileCreate;
	type.type = UCDiscussionCloudFileCreate;
	type.func = CreateMsg::CreateCloudFileContent;
	_appMsgMap[AppId::AppChat][ChatMessageType::DiscussionChat].push_back(type);

	type.msgId = GroupMessageId::MemberRoleUpdate;
	type.type = UCDiscussionMemberRoleUpdate;
	type.func = CreateMsg::CreateMemberRoleUpdate;
	_appMsgMap[AppId::AppChat][ChatMessageType::DiscussionChat].push_back(type);

	type.msgId = GroupMessageId::RevocationMsg;
	type.type = UCDiscussionRevocationMessage;
	type.func = CreateMsg::CreateRevocation;
	_appMsgMap[AppId::AppChat][ChatMessageType::DiscussionChat].push_back(type);

	////////////////////////////群聊天//////////////////////////////
	type.msgId   = ClusterMessageId::TextContent;
	type.type	 = UCGroupChatText;
	type.func    = CreateMsg::CreateText;
	_appMsgMap[AppId::AppChat][ChatMessageType::GroupChat].push_back(type);

	type.msgId   = ClusterMessageId::MediaContent;
	type.type	 = UCGroupChatMedia;
	type.func    = CreateMsg::CreateMedia;
	_appMsgMap[AppId::AppChat][ChatMessageType::GroupChat].push_back(type);

	type.msgId = GroupMessageId::ReportContent;
	type.type = UCGroupChatReprot;
	type.func = CreateMsg::CreateReport;
	_appMsgMap[AppId::AppChat][ChatMessageType::GroupChat].push_back(type);

	type.msgId   = ClusterMessageId::DisMemberAdd;
	type.type	 = UCGroupMemberAdd;
	type.func    = CreateMsg::CreateGroupAdd;
	_appMsgMap[AppId::AppChat][ChatMessageType::GroupChat].push_back(type);

	type.msgId   = ClusterMessageId::DisMemberDel;
	type.type	 = UCGroupMemberDel;
	type.func    = CreateMsg::CreateGroupDel;
	_appMsgMap[AppId::AppChat][ChatMessageType::GroupChat].push_back(type);

	type.msgId   = ClusterMessageId::DisNameUpdate;
	type.type	 = UCGroupNameUpdate;
	type.func    = CreateMsg::CreateGroupNameUpdate;
	_appMsgMap[AppId::AppChat][ChatMessageType::GroupChat].push_back(type);

	type.msgId   = ClusterMessageId::DiscussionCreate;
	type.type	 = UCGroupCreate;
	type.func    = CreateMsg::CreateGroupCreate;
	_appMsgMap[AppId::AppChat][ChatMessageType::GroupChat].push_back(type);

	type.msgId   = ClusterMessageId::DiscussionTop;
	type.type	 = UCGroupTop;
	type.func    = CreateMsg::CreateGroupTop;
	_appMsgMap[AppId::AppChat][ChatMessageType::GroupChat].push_back(type);

	type.msgId   = ClusterMessageId::SummaryCreate;
	type.type	 = UCGroupSummaryCreate;
	type.func    = CreateMsg::CreateSummaryCreate;
	_appMsgMap[AppId::AppChat][ChatMessageType::GroupChat].push_back(type);

	type.msgId   = ClusterMessageId::DocumentForward;
	type.type	 = UCGroupDocumentForward;
	type.func	 = CreateMsg::CreateSummaryCreate;
	_appMsgMap[AppId::AppChat][ChatMessageType::GroupChat].push_back(type);

	type.msgId   = ClusterMessageId::MsgNotifyUpdate;
	type.type	 = UCGroupMsgNotifyUpdate;
	type.func    = CreateMsg::CreateMsgNotify;
	_appMsgMap[AppId::AppChat][ChatMessageType::GroupChat].push_back(type);

	type.msgId   = ClusterMessageId::DisMemberKick;
	type.type	 = UCGroupMemberKick;
	type.func    = CreateMsg::CreateGroupMemberKick;
	_appMsgMap[AppId::AppChat][ChatMessageType::GroupChat].push_back(type);

	type.msgId   = ClusterMessageId::DocumentDel;
	type.type	 = UCGroupDocumentDel;
	type.func    = CreateMsg::CreateDocumentDel;
	_appMsgMap[AppId::AppChat][ChatMessageType::GroupChat].push_back(type);

	type.msgId   = ClusterMessageId::CodeCreate;
	type.type	 = UCGroupCodeCreate;
	type.func    = CreateMsg::CreateCodeCreate;
	_appMsgMap[AppId::AppChat][ChatMessageType::GroupChat].push_back(type);

	type.msgId   = ClusterMessageId::CodeForward;
	type.type	 = UCGroupCodeForward;
	type.func    = CreateMsg::CreateCodeCreate;
	_appMsgMap[AppId::AppChat][ChatMessageType::GroupChat].push_back(type);

	type.msgId   = ClusterMessageId::CodeDel;
	type.type	 = UCGroupCodeDel;
	type.func    = CreateMsg::CreateCodeDel;
	_appMsgMap[AppId::AppChat][ChatMessageType::GroupChat].push_back(type);

	type.msgId   = ClusterMessageId::CommentCreate;
	type.type	 = UCGroupCommentCreate;
	type.func    = CreateMsg::CreateComment;
	_appMsgMap[AppId::AppChat][ChatMessageType::GroupChat].push_back(type);

	type.msgId   = ClusterMessageId::CommentDel;
	type.type	 = UCGroupCommentDel;
	type.func    = CreateMsg::CreateDelComment;
	_appMsgMap[AppId::AppChat][ChatMessageType::GroupChat].push_back(type);

	type.msgId   = ClusterMessageId::DisAvatarUpdate;
	type.type	 = UCGroupAvatarUpdate;
	type.func    = CreateMsg::CreateGroupAvatarUpdate;
	_appMsgMap[AppId::AppChat][ChatMessageType::GroupChat].push_back(type);

	type.msgId   = ClusterMessageId::DisStateUpdate;
	type.type	 = UCGroupStateUpdate;
	type.func    = CreateMsg::CreateGroupStateUpdate;
	_appMsgMap[AppId::AppChat][ChatMessageType::GroupChat].push_back(type);

	type.msgId = ClusterMessageId::CloudFileCreate;
	type.type = UCGroupCloudFileCreate;
	type.func = CreateMsg::CreateCloudFileContent;
	_appMsgMap[AppId::AppChat][ChatMessageType::GroupChat].push_back(type);

	type.msgId = ClusterMessageId::MemberRoleUpdate;
	type.type = UCGroupMemberRoleUpdate;
	type.func = CreateMsg::CreateMemberRoleUpdate;
	_appMsgMap[AppId::AppChat][ChatMessageType::GroupChat].push_back(type);

	type.msgId = ClusterMessageId::RevocationMsg;
	type.type = UCGroupRevocationMessage;
	type.func = CreateMsg::CreateRevocation;
	_appMsgMap[AppId::AppChat][ChatMessageType::GroupChat].push_back(type);

	type.msgId = ClusterMessageId::DiscussionClose;
	type.type = UCGroupClose;
	type.func = CreateMsg::CreateDiscussionClose;
	_appMsgMap[AppId::AppChat][ChatMessageType::GroupChat].push_back(type);

	////////////////////////////云盘讨论组//////////////////////////////
	type.msgId = CloudFileMessageId::TextContent;
	type.type = UCCloudDiscussionTextContent;
	type.func = CreateMsg::CreateText;
	_appMsgMap[AppId::AppChat][ChatMessageType::CloudFileChat].push_back(type);

	type.msgId = CloudFileMessageId::MediaContent;
	type.type = UCCloudDiscussionMediaContent;
	type.func = CreateMsg::CreateMedia;
	_appMsgMap[AppId::AppChat][ChatMessageType::CloudFileChat].push_back(type);


	type.msgId = CloudFileMessageId::ReportContent;
	type.type = UCCloudDiscussionChatReprot;
	type.func = CreateMsg::CreateReport;
	_appMsgMap[AppId::AppChat][ChatMessageType::CloudFileChat].push_back(type);

	type.msgId = CloudFileMessageId::DisMemberAdd;
	type.type = UCCloudDiscussionMemberAdd;
	type.func = CreateMsg::CreateGroupAdd;
	_appMsgMap[AppId::AppChat][ChatMessageType::CloudFileChat].push_back(type);

	type.msgId = CloudFileMessageId::DisMemberDel;
	type.type = UCCloudDiscussionGroupMemberDel;
	type.func = CreateMsg::CreateGroupDel;
	_appMsgMap[AppId::AppChat][ChatMessageType::CloudFileChat].push_back(type);

	type.msgId = CloudFileMessageId::DisNameUpdate;
	type.type = UCCloudDiscussionNameUpdate;
	type.func = CreateMsg::CreateGroupNameUpdate;
	_appMsgMap[AppId::AppChat][ChatMessageType::CloudFileChat].push_back(type);

	type.msgId = CloudFileMessageId::DiscussionCreate;
	type.type = UCCloudDiscussionCreate;
	type.func = CreateMsg::CreateGroupCreate;
	_appMsgMap[AppId::AppChat][ChatMessageType::CloudFileChat].push_back(type);

	type.msgId = CloudFileMessageId::DiscussionClose;
	type.type = UCCloudDiscussionClose;
	type.func = CreateMsg::CreateDiscussionClose;
	_appMsgMap[AppId::AppChat][ChatMessageType::CloudFileChat].push_back(type);

	type.msgId = CloudFileMessageId::MsgNotifyUpdate;
	type.type = UCCloudDiscussionMsgNotifyUpdate;
	type.func = CreateMsg::CreateMsgNotify;
	_appMsgMap[AppId::AppChat][ChatMessageType::CloudFileChat].push_back(type);

	type.msgId = CloudFileMessageId::DisMemberKick;
	type.type = UCCloudDiscussionMemberKick;
	type.func = CreateMsg::CreateGroupMemberKick;
	_appMsgMap[AppId::AppChat][ChatMessageType::CloudFileChat].push_back(type);

	type.msgId = CloudFileMessageId::CommentCreate;
	type.type = UCCloudDiscussionCommentCreate;
	type.func = CreateMsg::CreateComment;
	_appMsgMap[AppId::AppChat][ChatMessageType::CloudFileChat].push_back(type);

	type.msgId = CloudFileMessageId::CommentDel;
	type.type = UCCloudDiscussionCommentDel;
	type.func = CreateMsg::CreateDelComment;
	_appMsgMap[AppId::AppChat][ChatMessageType::CloudFileChat].push_back(type);

	type.msgId = CloudFileMessageId::DisAvatarUpdate;
	type.type = UCCloudDiscussionAvatarUpdate;
	type.func = CreateMsg::CreateGroupAvatarUpdate;
	_appMsgMap[AppId::AppChat][ChatMessageType::CloudFileChat].push_back(type);

	type.msgId = CloudFileMessageId::CloudFileCreate;
	type.type = UCCloudDiscussionCloudFileCreate;
	type.func = CreateMsg::CreateCloudFileContent;
	_appMsgMap[AppId::AppChat][ChatMessageType::CloudFileChat].push_back(type);

	type.msgId = CloudFileMessageId::CloudFileUpdate;
	type.type = UCCloudDiscussionCloudFileUpdate;
	type.func = CreateMsg::CreateCloudFileContent;
	_appMsgMap[AppId::AppChat][ChatMessageType::CloudFileChat].push_back(type);

	type.msgId = CloudFileMessageId::CloudFileDel;
	type.type = UCCloudDiscussionCloudFileDel;
	type.func = CreateMsg::CreateCloudFileContent;
	_appMsgMap[AppId::AppChat][ChatMessageType::CloudFileChat].push_back(type);

	type.msgId = CloudFileMessageId::CloudFileMove;
	type.type = UCCloudDiscussionCloudFileMove;
	type.func = CreateMsg::CreateCloudFileContent;
	_appMsgMap[AppId::AppChat][ChatMessageType::CloudFileChat].push_back(type);

	type.msgId = CloudFileMessageId::CloudFileRename;
	type.type = UCCloudDiscussionCloudFileRename;
	type.func = CreateMsg::CreateCloudFileContent;
	_appMsgMap[AppId::AppChat][ChatMessageType::CloudFileChat].push_back(type);

	type.msgId = CloudFileMessageId::MemberRoleUpdate;
	type.type = UCCloudDiscussionMemberRoleUpdate;
	type.func = CreateMsg::CreateMemberRoleUpdate;
	_appMsgMap[AppId::AppChat][ChatMessageType::CloudFileChat].push_back(type);

	type.msgId = CloudFileMessageId::RevocationMsg;
	type.type = UCCloudRevocationMessage;
	type.func = CreateMsg::CreateRevocation;
	_appMsgMap[AppId::AppChat][ChatMessageType::CloudFileChat].push_back(type);
}

void AppMsgType::InitHeartbeat() {
	MsgInst type;
	type.msgId   = PingMessageID::DefaultId;
	type.type    = UCHeartbeat;
	type.func    = CreateMsg::CreatePing;
	_appMsgMap[AppId::AppHeartbeat][PingMessageType::DefaultType].push_back(type);
}

void AppMsgType::InitPresence() {
	MsgInst type;
	type.msgId   = PresenceMessageID::DefaultId;
	type.type    = UCPresence;
	type.func    = CreateMsg::CreatePresence;
	_appMsgMap[AppId::AppPresence][PresenceMessageType::DefaultType].push_back(type);
}

void AppMsgType::InitNotify() {
	MsgInst type;
	type.msgId   = SystemMessageId::DefaultId;
	type.type    = UCNotifySystemWelcome;
	type.func    = CreateMsg::CreateSystemWelcome;
	_appMsgMap[AppId::AppNotify][SystemProtoMessageType::SystemWelcome].push_back(type);

	type.msgId   = ClientUpdateProtoMessageId::IphoneType;
	type.type    = UCNotifyIphoneClientUpdate;
	type.func    = CreateMsg::CreateClientUpdate;
	_appMsgMap[AppId::AppNotify][SystemProtoMessageType::ClientUpdate].push_back(type);

	type.msgId   = ClientUpdateProtoMessageId::AndroidType;
	type.type    = UCNotifyAndroidClientUpdate;
	type.func    = CreateMsg::CreateClientUpdate;
	_appMsgMap[AppId::AppNotify][SystemProtoMessageType::ClientUpdate].push_back(type);

	type.msgId   = ClientUpdateProtoMessageId::PCType;
	type.type    = UCNotifyPCClientUpdate;
	type.func    = CreateMsg::CreateClientUpdate;
	_appMsgMap[AppId::AppNotify][SystemProtoMessageType::ClientUpdate].push_back(type);

	type.msgId   = CompleteInfoMessageId::tagAdd;
	type.type    = UCNotifyTagAdd;
	type.func    = CreateMsg::CreateCompleteInfo;
	_appMsgMap[AppId::AppNotify][SystemProtoMessageType::CompleteInfoType].push_back(type);

	type.msgId   = CompleteInfoMessageId::tagUpdate;
	type.type    = UCNotifyTagUpdate;
	type.func    = CreateMsg::CreateCompleteInfo;
	_appMsgMap[AppId::AppNotify][SystemProtoMessageType::CompleteInfoType].push_back(type);

	type.msgId   = CompleteInfoMessageId::tagDel;
	type.type    = UCNotifyTagDel;
	type.func    = CreateMsg::CreateTagDel;
	_appMsgMap[AppId::AppNotify][SystemProtoMessageType::CompleteInfoType].push_back(type);

	type.msgId   = PwdRuleUpdateMessageId::OutOfDateType;
	type.type    = UCNotifyPwdRuleOutOfDate;
	type.func    = CreateMsg::CreatePwdExpire;
	_appMsgMap[AppId::AppNotify][SystemProtoMessageType::PwdRuleUpdate].push_back(type);

	type.msgId   = PwdRuleUpdateMessageId::ComplexType;
	type.type    = UCNotifyPwdRuleComplex;
	type.func    = CreateMsg::CreatePwdComplexUpdate;
	_appMsgMap[AppId::AppNotify][SystemProtoMessageType::PwdRuleUpdate].push_back(type);

	type.msgId   = ClientGuideMessageId::MobileClientGuide;
	type.type    = UCNotifyMobileClientGuide;
	type.func    = CreateMsg::CreateMobileClientGuide;
	_appMsgMap[AppId::AppNotify][SystemProtoMessageType::clientInstallGuide].push_back(type);

	type.msgId   = ClientGuideMessageId::PCClientGuide;
	type.type    = UCNotifyPCClientGuide;
	type.func    = CreateMsg::CreatePcClientGuide;
	_appMsgMap[AppId::AppNotify][SystemProtoMessageType::clientInstallGuide].push_back(type);

	type.msgId   = SystemMessageId::DefaultId;
	type.type    = UCNotifyPasswordModify;
	type.func    = CreateMsg::CreatePwdModify;
	_appMsgMap[AppId::AppNotify][SystemProtoMessageType::PasswordModify].push_back(type);

	type.msgId   = SystemMessageId::DefaultId;
	type.type    = UCNotifySensitiveWordDelete;
	type.func    = CreateMsg::CreateNull;
	_appMsgMap[AppId::AppNotify][SystemProtoMessageType::SensitiveWordDelete].push_back(type);

	type.msgId   = SystemMessageId::DefaultId;
	type.type    = UCNotifyGroupRemind;
	type.func    = CreateMsg::CreateGroupRemind;
	_appMsgMap[AppId::AppNotify][SystemProtoMessageType::GroupRemind].push_back(type);
}

void AppMsgType::InitFiletransfer() {
	MsgInst type;
	type.msgId   = FileTransmissionMessageId::TransInvite;
	type.type    = UCFiletransferInvite;
	type.func    = CreateMsg::CreateFsInvite;
	_appMsgMap[AppId::AppFiletransfer][FileTransmissionMessageType::DefaultType].push_back(type);

	type.msgId   = FileTransmissionMessageId::TransReceive;
	type.type    = UCFiletransferReceive;
	type.func    = CreateMsg::CreateFsReceive;
	_appMsgMap[AppId::AppFiletransfer][FileTransmissionMessageType::DefaultType].push_back(type);

	type.msgId   = FileTransmissionMessageId::TransReject;
	type.type    = UCFiletransferReject;
	type.func    = CreateMsg::CreateFsReject;
	_appMsgMap[AppId::AppFiletransfer][FileTransmissionMessageType::DefaultType].push_back(type);

	type.msgId   = FileTransmissionMessageId::TransStop;
	type.type    = UCFiletransferStop;
	type.func    = CreateMsg::CreateFsStop;
	_appMsgMap[AppId::AppFiletransfer][FileTransmissionMessageType::DefaultType].push_back(type);
}

void AppMsgType::InitCalendar() {
	MsgInst type;
	type.msgId   = ConfShareMessageId::ConfShareCreate;
	type.type    = UCCalendarConfShare;
	type.func    = CreateMsg::CreateConfShare;
	_appMsgMap[AppId::AppCalendar][CalendarMessageType::CalendarShareType].push_back(type);

	type.msgId   = ConfShareMessageId::ConfShareCancel;
	type.type    = UCCalendarConfShareCancel;
	type.func    = CreateMsg::CreateConfShare;
	_appMsgMap[AppId::AppCalendar][CalendarMessageType::CalendarShareType].push_back(type);

	type.msgId   = ConfShareMessageId::ConfShareGrantorDisable;
	type.type    = UCCalendarConfGrantorDisable;
	type.func    = CreateMsg::CreateConfShare;
	_appMsgMap[AppId::AppCalendar][CalendarMessageType::CalendarShareType].push_back(type);

	type.msgId   = ConfShareMessageId::ConfShareGranteeDisable;
	type.type    = UCCalendarConfGranteeDisable;
	type.func    = CreateMsg::CreateConfShare;
	_appMsgMap[AppId::AppCalendar][CalendarMessageType::CalendarShareType].push_back(type);

	type.msgId   = ConfShareMessageId::ConfShareGranteeTransfer;
	type.type    = UCCalendarConfGranteeTransfer;
	type.func    = CreateMsg::CreateConfShare;
	_appMsgMap[AppId::AppCalendar][CalendarMessageType::CalendarShareType].push_back(type);

	type.msgId   = ConfShareMessageId::ConfShareGrantorLeave;
	type.type    = UCCalendarConfGrantorLeave;
	type.func    = CreateMsg::CreateConfShare;
	_appMsgMap[AppId::AppCalendar][CalendarMessageType::CalendarShareType].push_back(type);

	type.msgId   = ConfShareMessageId::ConfShareGranteeLeave;
	type.type    = UCCalendarConfGranteeLeave;
	type.func    = CreateMsg::CreateConfShare;
	_appMsgMap[AppId::AppCalendar][CalendarMessageType::CalendarShareType].push_back(type);
}

void AppMsgType::InitMeeting() {
	MsgInst type;
	type.msgId   = InstantConfInviteMessageId::IQInvite;
	type.type    = UCMeetingInstantConfIQInvite;
	type.func    = CreateMsg::CreateInstantIQInvite;
	_appMsgMap[AppId::AppMeeting][ConfMessageType::NormalInstantConfInviteMsg].push_back(type);

	type.msgId   = InstantConfInviteMessageId::IQReceive;
	type.type    = UCMeetingInstantConfIQReceive;
	type.func    = CreateMsg::CreateInstantIQReceive;
	_appMsgMap[AppId::AppMeeting][ConfMessageType::NormalInstantConfInviteMsg].push_back(type);

	type.msgId   = InstantConfInviteMessageId::IQReject;
	type.type    = UCMeetingInstantConfIQReject;
	type.func    = CreateMsg::CreateInstantIQReject;
	_appMsgMap[AppId::AppMeeting][ConfMessageType::NormalInstantConfInviteMsg].push_back(type);

	type.msgId   = ConfInviteMessageId::IQInvite;
	type.type    = UCMeetingConfInviteIQInvite;
	type.func    = CreateMsg::CreateIQInvite;
	_appMsgMap[AppId::AppMeeting][ConfMessageType::ConfInviteMsg].push_back(type);

	type.msgId   = ConfInviteMessageId::IQReceive;
	type.type    = UCMeetingConfInviteIQReceive;
	type.func    = CreateMsg::CreateIQReceive;
	_appMsgMap[AppId::AppMeeting][ConfMessageType::ConfInviteMsg].push_back(type);

	type.msgId   = ConfInviteMessageId::IQReject;
	type.type    = UCMeetingConfInviteIQReject;
	type.func    = CreateMsg::CreateIQReject;
	_appMsgMap[AppId::AppMeeting][ConfMessageType::ConfInviteMsg].push_back(type);

	type.msgId   = ConfInviteMessageId::IQInvite;
	type.type    = UCMeetingConfUpdateIQInvite;
	type.func    = CreateMsg::CreateIQInvite;
	_appMsgMap[AppId::AppMeeting][ConfMessageType::ConfUpdateMsg].push_back(type);

	type.msgId   = ConfInviteMessageId::IQReceive;
	type.type    = UCMeetingConfUpdateIQReceive;
	type.func    = CreateMsg::CreateIQReceive;
	_appMsgMap[AppId::AppMeeting][ConfMessageType::ConfUpdateMsg].push_back(type);

	type.msgId   = ConfInviteMessageId::IQReject;
	type.type    = UCMeetingConfUpdateIQReject;
	type.func    = CreateMsg::CreateIQReject;
	_appMsgMap[AppId::AppMeeting][ConfMessageType::ConfUpdateMsg].push_back(type);

	type.msgId   = ConfCancelMessageId::DefaultId;
	type.type    = UCMeetingConfCancel;
	type.func    = CreateMsg::CreateConfCancel;
	_appMsgMap[AppId::AppMeeting][ConfMessageType::ConfCancelMsg].push_back(type);

	type.msgId   = ConfCancelMessageId::RecurrentConfCancel;
	type.type    = UCMeetingRecurrentConfCancel;
	type.func    = CreateMsg::CreateConfCancel;
	_appMsgMap[AppId::AppMeeting][ConfMessageType::ConfCancelMsg].push_back(type);

	type.msgId   = ConfInviteMessageId::IQInvite;
	type.type    = UCMeetingConfForwardIQInvite;
	type.func    = CreateMsg::CreateIQInvite;
	_appMsgMap[AppId::AppMeeting][ConfMessageType::ConfForwardMsg].push_back(type);

	type.msgId   = ConfInviteMessageId::IQReceive;
	type.type    = UCMeetingConfForwardIQReceive;
	type.func    = CreateMsg::CreateIQReceive;
	_appMsgMap[AppId::AppMeeting][ConfMessageType::ConfForwardMsg].push_back(type);

	type.msgId   = ConfInviteMessageId::IQReject;
	type.type    = UCMeetingConfForwardIQReject;
	type.func    = CreateMsg::CreateIQReject;
	_appMsgMap[AppId::AppMeeting][ConfMessageType::ConfForwardMsg].push_back(type);

	type.msgId   = ConfAlertMessageId::FifteenMinutesAlert;
	type.type    = UCMeetingConfFifteenMinutesAlert;
	type.func    = CreateMsg::CreateConfAlert;
	_appMsgMap[AppId::AppMeeting][ConfMessageType::ConfAlertMsg].push_back(type);

	type.msgId   = ConfAlertMessageId::ZeroMinutesAlert;
	type.type    = UCMeetingConfZeroMinutesAlert;
	type.func    = CreateMsg::CreateConfAlert;
	_appMsgMap[AppId::AppMeeting][ConfMessageType::ConfAlertMsg].push_back(type);

	type.msgId   = ConfAlertMessageId::ConfStartAlert;
	type.type    = UCMeetingConfAlertStart;
	type.func    = CreateMsg::CreateConfAlert;
	_appMsgMap[AppId::AppMeeting][ConfMessageType::ConfAlertMsg].push_back(type);

	type.msgId   = ConfAlertMessageId::ConfEndAlert;
	type.type    = UCMeetingConfAlertEnd;
	type.func    = CreateMsg::CreateConfAlert;
	_appMsgMap[AppId::AppMeeting][ConfMessageType::ConfAlertMsg].push_back(type);

	type.msgId   = ConfAlertMessageId::PartJoinAlert;
	type.type    = UCMeetingConfAlertPartJoin;
	type.func    = CreateMsg::CreateConfAlert;
	_appMsgMap[AppId::AppMeeting][ConfMessageType::ConfAlertMsg].push_back(type);

	type.msgId   = ConfAlertMessageId::PartQuitAlert;
	type.type    = UCMeetingConfAlertPartQuit;
	type.func    = CreateMsg::CreateConfAlert;
	_appMsgMap[AppId::AppMeeting][ConfMessageType::ConfAlertMsg].push_back(type);

// 	type.msgId   = ConfShareMessageId::ConfShareCreate;
// 	type.type    = UCMeetingConfShare;
// 	type.func    = CreateMsg::CreateConfShare;
// 	_appMsgMap[AppId::AppMeeting][ConfMessageType::ConfShareMsg].push_back(type);
// 
// 	type.msgId   = ConfShareMessageId::ConfShareCancel;
// 	type.type    = UCMeetingConfShareCancel;
// 	type.func    = CreateMsg::CreateConfShare;
// 	_appMsgMap[AppId::AppMeeting][ConfMessageType::ConfShareMsg].push_back(type);
// 
// 	type.msgId   = ConfShareMessageId::ConfShareGrantorDisable;
// 	type.type    = UCMeetingConfShareGrantorDisable;
// 	type.func    = CreateMsg::CreateConfShare;
// 	_appMsgMap[AppId::AppMeeting][ConfMessageType::ConfShareMsg].push_back(type);
// 
// 	type.msgId   = ConfShareMessageId::ConfShareGranteeDisable;
// 	type.type    = UCMeetingConfShareGranteeDisable;
// 	type.func    = CreateMsg::CreateConfShare;
// 	_appMsgMap[AppId::AppMeeting][ConfMessageType::ConfShareMsg].push_back(type);
// 
// 	type.msgId   = ConfShareMessageId::ConfShareGranteeTransfer;
// 	type.type    = UCMeetingConfShareGranteeTransfer;
// 	type.func    = CreateMsg::CreateConfShare;
// 	_appMsgMap[AppId::AppMeeting][ConfMessageType::ConfShareMsg].push_back(type);
// 
// 	type.msgId   = ConfShareMessageId::ConfShareGrantorLeave;
// 	type.type    = UCMeetingConfShareGrantorLeave;
// 	type.func    = CreateMsg::CreateConfShare;
// 	_appMsgMap[AppId::AppMeeting][ConfMessageType::ConfShareMsg].push_back(type);
// 
// 	type.msgId   = ConfShareMessageId::ConfShareGranteeLeave;
// 	type.type    = UCMeetingConfShareGranteeLeave;
// 	type.func    = CreateMsg::CreateConfShare;
// 	_appMsgMap[AppId::AppMeeting][ConfMessageType::ConfShareMsg].push_back(type);

	type.msgId   = InstantConfInviteMessageId::IQInvite;
	type.type    = UCMeetingDiscussionInstantConfInviteIQInvite;
	type.func    = CreateMsg::CreateInstantIQInvite;
	_appMsgMap[AppId::AppMeeting][ConfMessageType::DiscussionInstantConfInviteMsg].push_back(type);

	type.msgId   = InstantConfInviteMessageId::IQReceive;
	type.type    = UCMeetingDiscussionInstantConfInviteIQReceive;
	type.func    = CreateMsg::CreateInstantIQReceive;
	_appMsgMap[AppId::AppMeeting][ConfMessageType::DiscussionInstantConfInviteMsg].push_back(type);

	type.msgId   = InstantConfInviteMessageId::IQReject;
	type.type    = UCMeetingDiscussionInstantConfInviteIQReject;
	type.func    = CreateMsg::CreateInstantIQReject;
	_appMsgMap[AppId::AppMeeting][ConfMessageType::DiscussionInstantConfInviteMsg].push_back(type);

	type.msgId   = ConfChatMessageId::TextType;
	type.type    = UCMeetingChatText;
	type.func    = CreateMsg::CreateConfChat;
	_appMsgMap[AppId::AppMeeting][ConfMessageType::ConfChatMsg].push_back(type);

	type.msgId   = ConfChatMessageId::MediaType;
	type.type    = UCMeetingChatMedia;
	type.func    = CreateMsg::CreateConfChat;
	_appMsgMap[AppId::AppMeeting][ConfMessageType::ConfChatMsg].push_back(type);

	type.msgId = ConfChatMessageId::RevocationMsg;
	type.type = UCMeetingRevocationMessage;
	type.func = CreateMsg::CreateConfChat;
	_appMsgMap[AppId::AppMeeting][ConfMessageType::ConfChatMsg].push_back(type);


	type.msgId = ConfChatMessageId::TextType;
	type.type = UCMeetingChatReprot;
	type.func = CreateMsg::CreateReport;
	_appMsgMap[AppId::AppMeeting][ConfMessageType::ConfReportMsg].push_back(type);

	type.msgId   = ConfSummaryMessageId::SummaryCreate;
	type.type    = UCMeetingSummaryCreate;
	type.func    = CreateMsg::CreateConfSummary;
	_appMsgMap[AppId::AppMeeting][ConfMessageType::ConfSummaryMsg].push_back(type);

	type.msgId   = ConfSummaryMessageId::SummaryUpdate;
	type.type    = UCMeetingSummaryUpdate;
	type.func    = CreateMsg::CreateConfSummary;
	_appMsgMap[AppId::AppMeeting][ConfMessageType::ConfSummaryMsg].push_back(type);

	type.msgId   = ConfSummaryMessageId::SummaryForward;
	type.type    = UCMeetingSummaryForward;
	type.func    = CreateMsg::CreateConfSummary;
	_appMsgMap[AppId::AppMeeting][ConfMessageType::ConfSummaryMsg].push_back(type);

	type.msgId   = ConfStateMessageId::callState;
	type.type    = UCMeetingCallState;
	type.func    = CreateMsg::CreateCallState;
	_appMsgMap[AppId::AppMeeting][ConfMessageType::ConfStateMsg].push_back(type);

	type.msgId = ConfFileMessageId::ConfFileUpload;
	type.type = UCMeetingFileUpload;
	type.func = CreateMsg::CreateConfUpload;
	_appMsgMap[AppId::AppMeeting][ConfMessageType::ConfUploadMsg].push_back(type);

	type.msgId = ConfFileMessageId::ConfFileUpload;
	type.type = UCMeetingDelDoc;
	type.func = CreateMsg::CreateConfDelDoc;
	_appMsgMap[AppId::AppMeeting][ConfMessageType::ConfDelDocMsg].push_back(type);
}

void AppMsgType::InitOrganization() {
	MsgInst type;
	type.msgId   = OrganizeProtoMessageId::DeptUpdate;
	type.type    = UCOrganizationDeptUpdate;
	type.func    = CreateMsg::CreateDeptUpdate;
	_appMsgMap[AppId::AppOrganization][OrganizeProtoMessageType::OrganizeType].push_back(type);

	type.msgId   = OrganizeProtoMessageId::DeptTransfer;
	type.type    = UCOrganizationDeptTransfer;
	type.func    = CreateMsg::CreateDeptTransfer;
	_appMsgMap[AppId::AppOrganization][OrganizeProtoMessageType::OrganizeType].push_back(type);

	type.msgId   = OrganizeProtoMessageId::PositionUpdate;
	type.type    = UCOrganizationPositionUpdate;
	type.func    = CreateMsg::CreatePositionUpdate;
	_appMsgMap[AppId::AppOrganization][OrganizeProtoMessageType::OrganizeType].push_back(type);

	type.msgId   = OrganizeProtoMessageId::EmployeeEntry;
	type.type    = UCOrganizationEmployeeEntry;
	type.func    = CreateMsg::CreateEmployeeEntry;
	_appMsgMap[AppId::AppOrganization][OrganizeProtoMessageType::OrganizeType].push_back(type);

	type.msgId   = OrganizeProtoMessageId::EmployeeLeave;
	type.type    = UCOrganizationEmployeeLeave;
	type.func    = CreateMsg::CreateEmployeeLeave;
	_appMsgMap[AppId::AppOrganization][OrganizeProtoMessageType::OrganizeType].push_back(type);

	type.msgId   = OrganizeProtoMessageId::EmployeeRightsUpdate;
	type.type    = UCOrganizationEmployeeRightsUpdate;
	type.func    = CreateMsg::CreateEmployeeRightsChange;
	_appMsgMap[AppId::AppOrganization][OrganizeProtoMessageType::OrganizeType].push_back(type);

	type.msgId   = OrganizeProtoMessageId::DeptParentChange;
	type.type    = UCOrganizationDeptParentChange;
	type.func    = CreateMsg::CreateDeptparentChange;
	_appMsgMap[AppId::AppOrganization][OrganizeProtoMessageType::OrganizeType].push_back(type);

	type.msgId   = OrganizeProtoMessageId::CompanyJoin;
	type.type    = UCOrganizationCompanyJoin;
	type.func    = CreateMsg::CreateCompanyJoin;
	_appMsgMap[AppId::AppOrganization][OrganizeProtoMessageType::OrganizeType].push_back(type);

	type.msgId   = OrganizeProtoMessageId::DeptDelete;
	type.type    = UCOrganizationDeptDelete;
	type.func    = CreateMsg::CreateDeptDelete;
	_appMsgMap[AppId::AppOrganization][OrganizeProtoMessageType::OrganizeType].push_back(type);

	type.msgId   = OrganizeProtoMessageId::EmployeeEntryConfirm;
	type.type    = UCOrganizationEmployeeEntryConfirm;
	type.func    = CreateMsg::CreateEmployeeEntryConfirm;
	_appMsgMap[AppId::AppOrganization][OrganizeProtoMessageType::OrganizeType].push_back(type);

	type.msgId   = OrganizeProtoMessageId::EmployeeLeaveConfirm;
	type.type    = UCOrganizationEmployeeLeaveConfirm;
	type.func    = CreateMsg::CreateEmployeeLeaveConfirm;
	_appMsgMap[AppId::AppOrganization][OrganizeProtoMessageType::OrganizeType].push_back(type);

	type.msgId   = OrganizeProtoMessageId::DeptTransferConfirm;
	type.type    = UCOrganizationDeptTransferConfirm;
	type.func    = CreateMsg::CreateDeptTransferConfirm;
	_appMsgMap[AppId::AppOrganization][OrganizeProtoMessageType::OrganizeType].push_back(type);
}

void AppMsgType::InitAck() {
	MsgInst type;
	type.msgId   = AckMessageID::AckSent;
	type.type    = UCAckMessageSent;
	type.func    = CreateMsg::CreateAckSent;
	_appMsgMap[AppId::AppAck][AckMessageType::DefaultType].push_back(type);

	type.msgId   = AckMessageID::AckSent;
	type.type    = UCAckMessageBatchSent;
	type.func    = CreateMsg::CreateAckSent;
	_appMsgMap[AppId::AppAck][AckMessageType::BatchAckType].push_back(type);

	type.msgId   = AckMessageID::AckReceived;
	type.type    = UCAckMessageReceived;
	type.func    = CreateMsg::CreateAckRecv;
	_appMsgMap[AppId::AppAck][AckMessageType::DefaultType].push_back(type);

	type.msgId   = AckMessageID::AckReceived;
	type.type    = UCAckMessageBatchReceived;
	type.func    = CreateMsg::CreateAckRecv;
	_appMsgMap[AppId::AppAck][AckMessageType::BatchAckType].push_back(type);

	type.msgId   = AckMessageID::AckRead;
	type.type    = UCAckMessageRead;
	type.func    = CreateMsg::CreateAckRead;
	_appMsgMap[AppId::AppAck][AckMessageType::DefaultType].push_back(type);

	type.msgId   = AckMessageID::AckRead;
	type.type    = UCAckMessageBatchRead;
	type.func	 = CreateMsg::CreateBatchAckRead;
	_appMsgMap[AppId::AppAck][AckMessageType::BatchAckType].push_back(type);

	type.msgId   = AckMessageID::AckPlay;
	type.type    = UCAckMessagePlay;
	type.func    = CreateMsg::CreateAckPlay;
	_appMsgMap[AppId::AppAck][AckMessageType::DefaultType].push_back(type);

	type.msgId   = AckMessageID::AckPlay;
	type.type    = UCAckMessageBatchPlay;
	type.func    = CreateMsg::CreateAckPlay;
	_appMsgMap[AppId::AppAck][AckMessageType::BatchAckType].push_back(type);
}

void AppMsgType::InitAudio() {
	MsgInst type;
	type.msgId   = AudioChatMessageId::AudioInvite;
	type.type    = UCAudioChatInvite;
	type.func    = CreateMsg::CreateAudioInvite;
	_appMsgMap[AppId::AppAudio][AudioChatMessageType::DefaultType].push_back(type);

	type.msgId   = AudioChatMessageId::AudioReceive;
	type.type    = UCAudioChatReceive;
	type.func    = CreateMsg::CreateAudioReceive;
	_appMsgMap[AppId::AppAudio][AudioChatMessageType::DefaultType].push_back(type);

	type.msgId   = AudioChatMessageId::AudioReject;
	type.type    = UCAudioChatReject;
	type.func    = CreateMsg::CreateAudioReject;
	_appMsgMap[AppId::AppAudio][AudioChatMessageType::DefaultType].push_back(type);

	type.msgId   = AudioChatMessageId::AudioStop;
	type.type    = UCAudioChatStop;
	type.func    = CreateMsg::CreateAudioStop;
	_appMsgMap[AppId::AppAudio][AudioChatMessageType::DefaultType].push_back(type);

	type.msgId   = AudioChatMessageId::AudioCancelCall;
	type.type    = UCAudioChatCancelCall;
	type.func    = CreateMsg::CreateAudioCancelCall;
	_appMsgMap[AppId::AppAudio][AudioChatMessageType::DefaultType].push_back(type);

	type.msgId   = AudioChatMessageId::AudioBlockCall;
	type.type    = UCAudioChatBlockCall;
	type.func    = CreateMsg::CreateAudioBlockCall;
	_appMsgMap[AppId::AppAudio][AudioChatMessageType::DefaultType].push_back(type);

	type.msgId   = AudioChatMessageId::AudioRejectCall;
	type.type    = UCAudioChatRejectCall;
	type.func    = CreateMsg::CreateAudioRejectCall;
	_appMsgMap[AppId::AppAudio][AudioChatMessageType::DefaultType].push_back(type);

	type.msgId   = AudioChatMessageId::AudioRecentCall;
	type.type    = UCAudioChatRecentCall;
	type.func    = CreateMsg::CreateAudioRecentCall;
	_appMsgMap[AppId::AppAudio][AudioChatMessageType::DefaultType].push_back(type);

	type.msgId   = AudioCallMessageId::AudioBlockCall;
	type.type    = UCAudioCallBlockCall;
	type.func    = CreateMsg::CreateAudioBlockCall;
	_appMsgMap[AppId::AppAudio][AudioChatMessageType::AudioCallRecord].push_back(type);

	type.msgId   = AudioCallMessageId::AudioRejectCall;
	type.type    = UCAudioCallRejectCall;
	type.func    = CreateMsg::CreateAudioRejectCall;
	_appMsgMap[AppId::AppAudio][AudioChatMessageType::AudioCallRecord].push_back(type);

	type.msgId   = AudioCallMessageId::AudioRecentCall;
	type.type    = UCAudioCallRecentCall;
	type.func    = CreateMsg::CreateAudioRecentCall;
	_appMsgMap[AppId::AppAudio][AudioChatMessageType::AudioCallRecord].push_back(type);

	type.msgId   = AudioCallMessageId::AudioBlockCall;
	type.type    = UCAudioUnfamiliarBlockCall;
	type.func    = CreateMsg::CreateAudioBlockCall;
	_appMsgMap[AppId::AppAudio][AudioChatMessageType::AudioUnfamiliarCallRecord].push_back(type);

	type.msgId   = AudioCallMessageId::AudioRejectCall;
	type.type    = UCAudioUnfamiliarRejectCall;
	type.func    = CreateMsg::CreateAudioRejectCall;
	_appMsgMap[AppId::AppAudio][AudioChatMessageType::AudioUnfamiliarCallRecord].push_back(type);

	type.msgId   = AudioCallMessageId::AudioRecentCall;
	type.type    = UCAudioUnfamiliarRecentCall;
	type.func    = CreateMsg::CreateAudioRecentCall;
	_appMsgMap[AppId::AppAudio][AudioChatMessageType::AudioUnfamiliarCallRecord].push_back(type);

	///////////////////////////////项目组音频呼叫////////////////////////////////////////////
	type.msgId   = AudioChatMessageId::AudioInvite;
	type.type    = UCAudioDiscussionInvite;
	type.func    = CreateMsg::CreateAudioInvite;
	_appMsgMap[AppId::AppAudio][AudioChatMessageType::DiscussionType].push_back(type);

	type.msgId   = AudioChatMessageId::AudioReceive;
	type.type    = UCAudioDiscussionReceive;
	type.func    = CreateMsg::CreateAudioReceive;
	_appMsgMap[AppId::AppAudio][AudioChatMessageType::DiscussionType].push_back(type);

	type.msgId   = AudioChatMessageId::AudioReject;
	type.type    = UCAudioDiscussionReject;
	type.func    = CreateMsg::CreateAudioReject;
	_appMsgMap[AppId::AppAudio][AudioChatMessageType::DiscussionType].push_back(type);

	type.msgId   = AudioChatMessageId::AudioStop;
	type.type    = UCAudioDiscussionStop;
	type.func    = CreateMsg::CreateAudioStop;
	_appMsgMap[AppId::AppAudio][AudioChatMessageType::DiscussionType].push_back(type);

	type.msgId   = AudioChatMessageId::AudioCancelCall;
	type.type    = UCAudioDiscussionCancelCall;
	type.func    = CreateMsg::CreateAudioCancelCall;
	_appMsgMap[AppId::AppAudio][AudioChatMessageType::DiscussionType].push_back(type);

	type.msgId   = AudioChatMessageId::AudioBlockCall;
	type.type    = UCAudioDiscussionBlockCall;
	type.func    = CreateMsg::CreateAudioBlockCall;
	_appMsgMap[AppId::AppAudio][AudioChatMessageType::DiscussionType].push_back(type);

	type.msgId   = AudioChatMessageId::AudioRejectCall;
	type.type    = UCAudioDiscussionRejectCall;
	type.func    = CreateMsg::CreateAudioRejectCall;
	_appMsgMap[AppId::AppAudio][AudioChatMessageType::DiscussionType].push_back(type);

	type.msgId   = AudioChatMessageId::AudioRecentCall;
	type.type    = UCAudioDiscussionRecentCall;
	type.func    = CreateMsg::CreateAudioRecentCall;
	_appMsgMap[AppId::AppAudio][AudioChatMessageType::DiscussionType].push_back(type);

	/////////////////////////////////////群聊天////////////////////////////////
	type.msgId   = AudioChatMessageId::AudioInvite;
	type.type    = UCAudioGroupInvite;
	type.func    = CreateMsg::CreateAudioInvite;
	_appMsgMap[AppId::AppAudio][AudioChatMessageType::GroupType].push_back(type);

	type.msgId   = AudioChatMessageId::AudioReceive;
	type.type    = UCAudioGroupReceive;
	type.func    = CreateMsg::CreateAudioReceive;
	_appMsgMap[AppId::AppAudio][AudioChatMessageType::GroupType].push_back(type);

	type.msgId   = AudioChatMessageId::AudioReject;
	type.type    = UCAudioGroupReject;
	type.func    = CreateMsg::CreateAudioReject;
	_appMsgMap[AppId::AppAudio][AudioChatMessageType::GroupType].push_back(type);

	type.msgId   = AudioChatMessageId::AudioStop;
	type.type    = UCAudioGroupStop;
	type.func    = CreateMsg::CreateAudioStop;
	_appMsgMap[AppId::AppAudio][AudioChatMessageType::GroupType].push_back(type);

	type.msgId   = AudioChatMessageId::AudioCancelCall;
	type.type    = UCAudioGroupCancelCall;
	type.func    = CreateMsg::CreateAudioCancelCall;
	_appMsgMap[AppId::AppAudio][AudioChatMessageType::GroupType].push_back(type);

	type.msgId   = AudioChatMessageId::AudioBlockCall;
	type.type    = UCAudioGroupBlockCall;
	type.func    = CreateMsg::CreateAudioBlockCall;
	_appMsgMap[AppId::AppAudio][AudioChatMessageType::GroupType].push_back(type);

	type.msgId   = AudioChatMessageId::AudioRejectCall;
	type.type    = UCAudioGroupRejectCall;
	type.func    = CreateMsg::CreateAudioRejectCall;
	_appMsgMap[AppId::AppAudio][AudioChatMessageType::GroupType].push_back(type);

	type.msgId   = AudioChatMessageId::AudioRecentCall;
	type.type    = UCAudioGroupRecentCall;
	type.func    = CreateMsg::CreateAudioRecentCall;
	_appMsgMap[AppId::AppAudio][AudioChatMessageType::GroupType].push_back(type);
}


void AppMsgType::InitAssist() {
	MsgInst type;
	type.msgId = RemoteAssistanceMessageId::RemoteAssistanceInvite;
	type.type = UCRemoteAssistInvite;
	type.func = CreateMsg::CreateAssistInviteMessage;
	_appMsgMap[AppId::AppRemoteAssistance][RemoteAssistanceMessageType::DefaultType].push_back(type);

	type.msgId = RemoteAssistanceMessageId::RemoteAssistanceAccept;
	type.type = UCRemoteAssistAccept;
	type.func = CreateMsg::CreateAssistAcceptMessage;
	_appMsgMap[AppId::AppRemoteAssistance][RemoteAssistanceMessageType::DefaultType].push_back(type);

	type.msgId = RemoteAssistanceMessageId::RemoteAssistanceReject;
	type.type = UCRemoteAssistReject;
	type.func = CreateMsg::CreateAssistRejectMessage;
	_appMsgMap[AppId::AppRemoteAssistance][RemoteAssistanceMessageType::DefaultType].push_back(type);

	type.msgId = RemoteAssistanceMessageId::RemoteAssistanceEnd;
	type.type = UCRemoteAssistEnd;
	type.func = CreateMsg::CreateAssistEndMessage;
	_appMsgMap[AppId::AppRemoteAssistance][RemoteAssistanceMessageType::DefaultType].push_back(type);
}


void AppMsgType::InitSyncMessage(){
	MsgInst type;
	type.msgId = SyncMessageId::SessionTopSet;
	type.type = UCSessionTopSet;
	type.func = CreateMsg::CreatSessionTopContent;
	_appMsgMap[AppId::AppSync][SyncMessageType::SessionTopType].push_back(type);

	type.msgId = SyncMessageId::SessionTopUnset;
	type.type = UCSessionTopUnset;
	type.func = CreateMsg::CreatSessionTopContent;
	_appMsgMap[AppId::AppSync][SyncMessageType::SessionTopType].push_back(type);

	type.msgId = SyncMessageId::AtMessageReaded;
	type.type = UCAtMessageRead;
	type.func = CreateMsg::CreatAtMessageReadedContent;
	_appMsgMap[AppId::AppSync][SyncMessageType::AtMessageType].push_back(type);

	type.msgId = SyncMessageId::UserLoginNotify;
	type.type = UCUserLoginNotify;
	type.func = CreateMsg::CreateUserLoginNotify;
	_appMsgMap[AppId::AppSync][SyncMessageType::UserLoginType].push_back(type);
}

void AppMsgType::InitAddressbook() {
	MsgInst type;
	type.msgId   = CardUpdateMessageId::DefaultId;
	type.type    = UCAddressBookCardUpdate;
	type.func    = CreateMsg::CreateCardUpdate;
	_appMsgMap[AppId::AppAddressBook][AddressBookMessageType::CardUpdateMsg].push_back(type);

	type.msgId   = ContacterRelationUpdateMessageId::ContacterAdd;
	type.type    = UCAddressBookContacterAdd;
	type.func    = CreateMsg::CreateContacterAdd;
	_appMsgMap[AppId::AppAddressBook][AddressBookMessageType::ContacterRelationUpdateMsg].push_back(type);

	type.msgId   = ContacterRelationUpdateMessageId::ContacterDel;
	type.type    = UCAddressBookContacterDel;
	type.func    = CreateMsg::CreateContacterDel;
	_appMsgMap[AppId::AppAddressBook][AddressBookMessageType::ContacterRelationUpdateMsg].push_back(type);
}

void AppMsgType::InitSleep() {
	MsgInst type;
	type.msgId   = SleepMessageId::IQRequest;
	type.type    = UCSleepIQRequest;
	type.func    = CreateMsg::CreateSleep;
	_appMsgMap[AppId::AppSleep][SleepMessageType::DefaultType].push_back(type);

	type.msgId   = SleepMessageId::IQConfirm;
	type.type    = UCSleepIQConfirm;
	type.func    = CreateMsg::CreateWakeUp;
	_appMsgMap[AppId::AppSleep][SleepMessageType::DefaultType].push_back(type);
}

void AppMsgType::InitTimeNews() {
	MsgInst type;
	type.msgId   = TimeNewsMessageId::WB_ATTENTION;
	type.type    = UCTimeNewsAttention;
	type.func    = CreateMsg::CreateWebMessage;
	_appMsgMap[AppId::AppTimeNews][TimeNewsMessageType::DefaultType].push_back(type);

	type.msgId   = TimeNewsMessageId::WB_TIMENEWS_PUBLISH;
	type.type    = UCTimeNewsPublish;
	type.func    = CreateMsg::CreateWebMessage;
	_appMsgMap[AppId::AppTimeNews][TimeNewsMessageType::DefaultType].push_back(type);

	type.msgId   = TimeNewsMessageId::WB_TIMENEWS_COMMENT;
	type.type    = UCTimeNewsComment;
	type.func    = CreateMsg::CreateWebMessage;
	_appMsgMap[AppId::AppTimeNews][TimeNewsMessageType::DefaultType].push_back(type);

	type.msgId   = TimeNewsMessageId::WB_TIMENEWS_FORWARD;
	type.type    = UCTimeNewsForward;
	type.func    = CreateMsg::CreateWebMessage;
	_appMsgMap[AppId::AppTimeNews][TimeNewsMessageType::DefaultType].push_back(type);

	type.msgId   = TimeNewsMessageId::WB_TIMENEWS_PRAISE;
	type.type    = UCTimeNewsPraise;
	type.func    = CreateMsg::CreateWebMessage;
	_appMsgMap[AppId::AppTimeNews][TimeNewsMessageType::DefaultType].push_back(type);
}

void AppMsgType::InitAPI() {
	MsgInst type;
	type.msgId   = APIMessageId::Text;
	type.type    = UCAPIText;
	type.func    = CreateMsg::CreateAPIText;
	_appMsgMap[AppId::AppAPI][APIMessageType::AppMsg].push_back(type);

	type.msgId = APIMessageId::Image;
	type.type = UCAPIImage;
	type.func = CreateMsg::CreateAPIImage;
	_appMsgMap[AppId::AppAPI][APIMessageType::AppMsg].push_back(type);

	type.msgId = APIMessageId::File;
	type.type = UCAPIFile;
	type.func = CreateMsg::CreateAPIFile;
	_appMsgMap[AppId::AppAPI][APIMessageType::AppMsg].push_back(type);

	type.msgId = APIMessageId::OA;
	type.type = UCAPIOA;
	type.func = CreateMsg::CreateAPIOA;
	_appMsgMap[AppId::AppAPI][APIMessageType::AppMsg].push_back(type);

	type.msgId = APIMessageId::Customized;
	type.type = UCAPICustomizedContent;
	type.func = CreateMsg::CreateAPICustomizedContent;
	_appMsgMap[AppId::AppAPI][APIMessageType::AppMsg].push_back(type);

	type.msgId = APIAckMessageId::AckSent;
	type.type = UCAPIAck;
	type.func = CreateMsg::CreateAPIAck;
	_appMsgMap[AppId::AppAPI][APIMessageType::AppAck].push_back(type);

	type.msgId = APIControlMessageId::OAStatusChange;
	type.type = UCAPIOAStatusChange;
	type.func = CreateMsg::CreateAPIOAStatusChange;
	_appMsgMap[AppId::AppAPI][APIMessageType::AppControl].push_back(type);

	type.msgId = APIControlMessageId::AppEventChangeNotify;
	type.type = UCAPIAppEventChange;
	type.func = CreateMsg::CreateAPIAppEvnentChange;
	_appMsgMap[AppId::AppAPI][APIMessageType::AppControl].push_back(type);

	type.msgId = APIControlMessageId::AppChangeNotify;
	type.type = UCAPIAppChangeNotify;
	type.func = CreateMsg::CreateAPIAppChangeNotify;
	_appMsgMap[AppId::AppAPI][APIMessageType::AppControl].push_back(type);


}

UCMsg* AppMsgType::GetMsg(const UcMessageHead *head) {
	map<int, map<int, vector<MsgInst> > >::iterator appItor 
		= _appMsgMap.find(head->appid);
	if (appItor != _appMsgMap.end()) {
		map<int, vector<MsgInst> >::iterator typeItor = appItor->second.find(head->protocoltype);
		if (typeItor != appItor->second.end()) {
			for (vector<MsgInst>::iterator inst_itor = typeItor->second.begin();
				inst_itor != typeItor->second.end(); ++inst_itor) {
				if (inst_itor->msgId == head->protocolid) {
					return inst_itor->func(inst_itor->type);
				}
			}
		}
	}

	UC_LOG(ERROR) << "GetMsg by head return NULL! appid:" << head->appid 
		<< ", protocoltype:" << head->protocoltype << ", protocolid:" << head->protocolid;
	return NULL;
}

int AppMsgType::GetMsgID(MsgType type, int& appId, int& protocoltype, int& protocolId) {
	for (map<int, map<int, vector<MsgInst> > >::iterator appItor = _appMsgMap.begin();
		appItor != _appMsgMap.end(); ++appItor) {
		for (map<int, vector<MsgInst> >::iterator typeItor = appItor->second.begin();
			typeItor != appItor->second.end(); ++typeItor)
		{
			for (vector<MsgInst>::iterator idItor = typeItor->second.begin();
				idItor != typeItor->second.end(); ++idItor) {
				if (idItor->type == type) {
					appId = appItor->first;
					protocoltype = typeItor->first;
					protocolId = idItor->msgId;
					return 0;
				}
			}
		}
	}

	UC_LOG(ERROR) << "GetMsgID return error! type:" << type;
	return -1;
}
} //namespace uc