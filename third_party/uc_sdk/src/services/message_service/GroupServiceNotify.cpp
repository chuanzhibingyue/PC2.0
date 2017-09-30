/*
 * GroupService.cpp
 *
 *  Created on: Jun 12, 2015
 *      Author: yongshuai.cheng
 */

#include "GroupService.h"
#include "MessageProcessor.h"
#include "UCEngine.h"
#include "common/StringConvert.h"
#include "MessageParser.h"
#include "DBManager.h"
#include "client_service/ClientService.h"
#include "UCErrorCode.h"
#include "CommonDef.h"
#include "UCParserErrorCode.h"
#include "store_service/StoreService.h"


namespace uc {

	int32_t GroupService::OnMessageReceived(MessageChannel channel, UCMsg &msg, const string &body, MsgSendState state){
	bool isSave = false;
	bool saveUnread = false;
	UCHead head;
	msg.GetHead(head);

	switch(msg.GetMsgType()) {
	case UCDiscussionCreate:
	case UCGroupCreate: 
	case UCCloudDiscussionCreate: {
		OnCreateGroupNotify(channel, head, msg, state,isSave);
		break;
	}
	case UCGroupClose:
	case UCDiscussionClose:
	case UCCloudDiscussionClose: {
		OnCloseGroupNotify(channel, head, msg, state, isSave);
		break;
	}
	case UCDiscussionMemberDel:
	case UCGroupMemberDel:
	case UCCloudDiscussionGroupMemberDel: {
		OnExitGroupNotify(channel, head, msg, state,isSave);
		break;
	}
	case UCDiscussionMemberAdd:
	case UCGroupMemberAdd: 
	case UCCloudDiscussionMemberAdd: {
		OnAddMemberNotify(channel, head, msg, state,isSave);
		break;
	}
	case UCDiscussionMemberKick: 
	case UCGroupMemberKick: 
	case UCCloudDiscussionMemberKick: {
		OnKickMemberNotify(channel, head, msg, state, isSave);
		break;
	}
	case UCDiscussionNameUpdate:
	case UCGroupNameUpdate: 
	case UCCloudDiscussionNameUpdate: {
		OnUpdateNameNotify(channel, head, msg, state,isSave);
		break;
	}
	case UCDiscussionAvatarUpdate:
	case UCGroupAvatarUpdate: 
	case UCCloudDiscussionAvatarUpdate: {
		OnUpdateAvatarNotify(channel, head, msg, state,isSave);
		break;
	}
	case UCDiscussionTop:
	case UCGroupTop: {
		OnSetTopNotify(channel, head, msg, state,isSave);
		break;
	}
	case UCDiscussionAlertrule: {
		OnSetReminderNotify(channel, head, msg, state,isSave);
		break;
	}
	case UCDiscussionMsgNotifyUpdate:
	case UCGroupMsgNotifyUpdate: 
	case UCCloudDiscussionMsgNotifyUpdate: {
		OnSetDisturbNotify(channel, head, msg, state,isSave);
		break;
	}
	case UCDiscussionSummaryCreate:
	case UCGroupSummaryCreate: {
		OnUploadDocumentNotify(channel, 0, head, msg,state, isSave, saveUnread);
		break;
	}
	case UCDiscussionDocumentForward:
	case UCGroupDocumentForward: {
		OnUploadDocumentNotify(channel, 1, head, msg, state,isSave, saveUnread);
		break;
	}
	case UCGroupCodeCreate:
	case UCDiscussionCodeCreate: {
		OnUploadCodeSnippetNotify(channel, 0, head, msg, state,isSave, saveUnread);
		break;
	 }
	case UCGroupCodeForward:
	case UCDiscussionCodeForward: {
		OnUploadCodeSnippetNotify(channel, 1, head, msg, state,isSave, saveUnread);
		break;
	}
	case UCDiscussionDocumentDel:
	case UCGroupDocumentDel: {
		OnDeleteDocumentNotify(channel, head, msg, state,isSave);
		break;
	}
	case UCDiscussionCommentCreate:
	case UCGroupCommentCreate:
	case UCCloudDiscussionCommentCreate: {
		OnCreateCommentNotify(channel, head, msg,state, isSave, saveUnread);
		break;
	}
	case UCDiscussionChatText:
	case UCGroupChatText: 
	case UCCloudDiscussionTextContent: {
		OnTextMessageNotify(channel, head, msg, state,isSave, saveUnread);
		break;
	}
	case UCDiscussionChatMedia:
	case UCGroupChatMedia: 
	case UCCloudDiscussionMediaContent: {
		OnMediaMessageNotify(channel, head, msg,state, isSave, saveUnread);
		break;
	}
	case UCDiscussionChatReprot:
	case UCGroupChatReprot:
	case UCCloudDiscussionChatReprot: {
		OnReportMessageNotify(channel, head, msg, state, isSave);
		break;
	}
	case UCDiscussionStateUpdate:
	case UCGroupStateUpdate: {
		OnStatusInfoUpdate(channel, head, msg, state, isSave);
		break;
	}
	case UCAckMessageRead:{
		OnMessageReadNotify(channel, head, msg, state,isSave);
		break;
	}
	case UCAckMessageBatchRead: {
		OnMessageBatchReadNotify(channel, head, msg, state, isSave);
		break;
	}
	case UCAckMessagePlay: {
		OnMessagePlayedNotify(channel, head, msg, state,isSave);
		break;
	}
	case UCAtMessageRead: {
		OnAtMessageReadNotify(channel, head, msg, state, isSave);
		break;
	}
	case UCAudioDiscussionInvite: {
		OnInviteCallNotify(channel, PROJECT_CHAT, head, msg,state, isSave);
		break;
	}
	case UCAudioGroupInvite: {
		OnInviteCallNotify(channel, GROUP_CHAT, head, msg, state,isSave);
		break;
	}
	case UCAudioDiscussionReceive: {
		OnAcceptCallNotify(channel, PROJECT_CHAT, head, msg, state,isSave);
		break;
	}
	case UCAudioGroupReceive: {
		OnAcceptCallNotify(channel, GROUP_CHAT, head, msg, state, isSave);
		break;
	}
	case UCAudioDiscussionReject: {
		OnRejectCallNofify(channel, PROJECT_CHAT, head, msg,state, isSave);
		break;
	}
	case UCAudioGroupReject: {
		OnRejectCallNofify(channel, GROUP_CHAT, head, msg, state, isSave);
		break;
	}
	case UCAudioDiscussionStop: {
		OnFinishCallNotify(channel, PROJECT_CHAT, head, msg, state,isSave);
		break;
	}
	case UCAudioGroupStop: {
		OnFinishCallNotify(channel, GROUP_CHAT, head, msg, state, isSave);
		break;
	}
	case UCAudioDiscussionRecentCall: {
		OnConnectedCallNotify(channel, PROJECT_CHAT, head, msg,state, isSave);
		break;
	}
	case UCAudioGroupRecentCall: {
		OnConnectedCallNotify(channel, GROUP_CHAT, head, msg,state, isSave);
		break;
	}
	case UCDiscussionCloudFileCreate:
	case UCGroupCloudFileCreate:
	case UCCloudDiscussionCloudFileCreate:
	case UCCloudDiscussionCloudFileUpdate:
	case UCCloudDiscussionCloudFileDel:
	case UCCloudDiscussionCloudFileMove:
	case UCCloudDiscussionCloudFileRename: {
		OnCloudFileModifiedNotify(msg.GetMsgType(), channel, head, msg, state, isSave);
		break;
	}
	case UCDiscussionMemberRoleUpdate:
	case UCGroupMemberRoleUpdate:
	case UCCloudDiscussionMemberRoleUpdate:
		OnMemberRoleUpdateNotify(channel, head, msg, state, isSave);
		break;
	case UCAudioDiscussionCancelCall:{
		OnCancelCallNotify(channel, PROJECT_CHAT, head, msg, state, isSave);
		break;
	}
	case UCAudioGroupCancelCall: {
		OnCancelCallNotify(channel, GROUP_CHAT, head, msg, state, isSave);
		break;
	}
	case UCNormalRevocationMessage:
	case UCDiscussionRevocationMessage:
	case UCGroupRevocationMessage:
	case UCCloudRevocationMessage:{
		OnRevocationNotify(channel, head, msg, state, isSave);
		break;
	}
	default:
		break;
	}

	if (isSave && UCAS == channel){
		MessageProcessor msgProcessor((UCEngine*)_engine);
		ConversationType type = MessageProcessor::GetConversationType(head.GetAppId(), head.GetProtocoltype());

		int64_t relate_id = MessageProcessor::GetRelateId(type, GetCurUser().GetUserId(), head.GetFrom().GetUserId(), head.GetTo().GetUserId());
		if (msgProcessor.SaveMessage(type, relate_id, 0, 0, STATE_SENDSUCCESS, &msg, body) != UC_SUCCEEDED)
		{
			UC_LOG(ERROR) << "GroupService::OnMessageReceived save message failed." << " msgType = " << msg.GetMsgType();
		}

		if (saveUnread && (head.GetFrom().GetUserId() == GetCurUser().GetUserId())){
			OnMessageReadUserNotify(type, head.GetTo().GetUserId(), head.GetSeq());
		}
	}

	return UC_SUCCEEDED;
}

void GroupService::OnCreateGroupNotify(MessageChannel channel, UCHead head, UCMsg& ucMsg, MsgSendState state,bool& isSave){

	isSave = true;
	if (UCAS == channel)
	{
		head.SetId(DBManager::GetInstance().GetMessageID());
		ucMsg.SetHead(head);
	}
	
	GroupCreateContentMsg *msg = (GroupCreateContentMsg*)&ucMsg;
	if (NULL == msg){
		isSave = false;
		UC_LOG(ERROR) << "GroupService::OnCreateGroupNotify msg is null";
		return;
	}

	if (UCAS == channel && IsCurUser(head.GetFrom())) {
		theStore.GetMsgStore()->InitializeCache(msg->get_groupid(), MessageProcessor::GetConversationType(head.GetAppId(), head.GetProtocoltype()) );
	}

	GroupInfo groupInfo;
	groupInfo.SetId(msg->get_groupid());
	groupInfo.SetSiteId(msg->get_siteID());
	groupInfo.SetName(msg->get_group_name());
	groupInfo.SetPinYinName(msg->get_name_pinyin());
	groupInfo.SetCreateTime(msg->get_create_time());
	groupInfo.SetAvatarIndex(msg->get_avatar());
	groupInfo.SetMemberCount(msg->get_memberCount());
	groupInfo.SetGroupStatus(msg->get_group_status());
	groupInfo.SetGroupType(msg->get_group_type());
	groupInfo.SetMemberType(msg->get_group_status() >> 2 & 1);
	groupInfo.SetAdminId(msg->get_operatorid());
	groupInfo.SetStatusInfo(msg->get_status_info());

	if (groupInfo.GetGroupType() == CLOUD_GROUP_TYPE) {
		int mountId = GroupInfo::ParseMountId(msg->get_cloud_library_info());
		if (mountId == 0) {
			UC_LOG(ERROR) << "cloud group is created but has no mount_id";
			return;
		}
		groupInfo.SetMountId(mountId);
	}

	MemberList memberList;
	for(unsigned int i = 0; i < msg->get_memberList().size(); ++i){
		MemberInfo memberInfo;
		memberInfo.userId =  msg->get_memberList()[i].memberId;
		memberInfo.joinTime =  msg->get_memberList()[i].join_time;
		memberInfo.role = GroupRole(msg->get_memberList()[i].role);
		memberList.push_back(memberInfo);
		if (memberInfo.userId == GetCurUser().GetUserId()){
			groupInfo.SetJoinState(msg->get_memberList()[i].join_state);
		}
	}
	if (memberList.size() > 0){
		groupInfo.AddMember(memberList);
	}

	MessageInfo message;

	message.InfoFromUCHead(head);
	message.SetSendState(state);

	triggerEvent(IGroupServiceEvent, OnGroupCreateMessageReceived(channel, message, groupInfo));
	
	//从UCAS过来的消息并且不是当前用户才会操作memory;
	if (UCAS != channel || IsCurUser(head.GetFrom())){
		return;
	}
	
	theStore.GetGroupListStore()->UpdateGroupInfo(groupInfo);
}

void GroupService::OnExitGroupNotify(MessageChannel channel, UCHead head, UCMsg& ucMsg, MsgSendState state, bool& isSave){

	isSave = true;
	if (UCAS == channel)
	{
		head.SetId(DBManager::GetInstance().GetMessageID());
		ucMsg.SetHead(head);
	}

	GroupMemberDelContentMsg *msg = (GroupMemberDelContentMsg*)&ucMsg;
	if(!msg){
		isSave = false;
		UC_LOG(ERROR) << "GroupService::OnExitGroupNotify msg is null";
		return;
	}

	MessageInfo message;

	message.InfoFromUCHead(head);
	message.SetSendState(state);
	triggerEvent(IGroupServiceEvent, OnGroupMemberDelMessageReceived(channel, message, msg->get_operatorid(), msg->get_groupid(),
			msg->get_memberId(), msg->get_avatar(), msg->get_group_name(), msg->get_name_pinyin(), msg->get_name_flag()));

	//从UCAS过来的消息并且不是当前用户才会操作memory;
	if (channel != UCAS || IsCurUser(head.GetFrom())){
		return;
	}

	if (!theStore.GetGroupListStore()->UpdateGroupName(msg->get_groupid(), msg->get_group_name(), msg->get_name_pinyin())){

		UC_LOG(ERROR) << "GroupService::OnExitGroupNotify save group name failed";
	}

	if (!theStore.GetGroupListStore()->UpdateGroupAvatarIndex(msg->get_groupid(), msg->get_avatar())){

		UC_LOG(ERROR) << "GroupService::OnExitGroupNotify save group avatar failed";
	}

	theStore.GetGroupListStore()->DeleteMember(msg->get_groupid(), msg->get_memberId(), 
		[](int32_t id1, int32_t id2)->bool{return id1 == id2; }(GetCurUser().GetUserId(), msg->get_memberId()));

}

void GroupService::OnCloseGroupNotify(MessageChannel channel, UCHead head, UCMsg& ucMsg, MsgSendState state,bool& isSave){

	isSave = true;
	if (UCAS == channel)
	{
		head.SetId(DBManager::GetInstance().GetMessageID());
		ucMsg.SetHead(head);
	}

	DiscussionCloseMsg *msg = (DiscussionCloseMsg*)&ucMsg;
	if(!msg){
		isSave = false;
		UC_LOG(ERROR) << "GroupService::OnEndGroupNotify msg is null";
		return;
	}
	
	MessageInfo	message;

	message.InfoFromUCHead(head);
	message.SetSendState(state);

	triggerEvent(IGroupServiceEvent, OnGroupCloseMessageReceived(channel, message, msg->get_operatorId()));

	//从UCAS过来的消息并且不是当前用户才会操作memory;
	if (channel != UCAS || IsCurUser(head.GetFrom())) {
		return;
	}

	theStore.GetGroupListStore()->CloseGroup(head.GetTo().GetUserId());

}

void GroupService::OnAddMemberNotify(MessageChannel channel, UCHead head, UCMsg& ucMsg, MsgSendState state,bool& isSave){

	isSave = true;
	if (UCAS == channel)
	{
		head.SetId(DBManager::GetInstance().GetMessageID());
		ucMsg.SetHead(head);
	}

	GroupMemberAddContentMsg *msg = (GroupMemberAddContentMsg*)&ucMsg;
	if(!msg){
		isSave = false;
		UC_LOG(ERROR) << "GroupService::OnAddMemberNotify msg is null";
		return;
	}

	std::vector<UserInfo> memberList;
	for (unsigned int i = 0; i < msg->get_memberList().size(); ++i){
		UserInfo user;
		user.ucid.SetUserId(msg->get_memberList()[i].memberId);
		user.role = GroupRole(msg->get_memberList()[i].role);

		memberList.push_back(user);
	}

	MessageInfo message;

	message.InfoFromUCHead(head);
	message.SetSendState(state);

	triggerEvent(IGroupServiceEvent, OnGroupMemberAddMessageReceived(channel, message, msg->get_operatorid(), msg->get_groupid(),
			memberList, msg->get_avatar(), msg->get_group_name(), msg->get_name_pinyin(), msg->get_name_flag()));
	
	//从UCAS过来的消息并且不是当前用户才会操作memory;
	if (channel != UCAS || IsCurUser(head.GetFrom())) {
		return;
	}

	if (!theStore.GetGroupListStore()->UpdateGroupName(msg->get_groupid(), msg->get_group_name(), msg->get_name_pinyin())){
		UC_LOG(ERROR) << "GroupService::OnAddMemberNotify save group name failed";
	}

	if (!theStore.GetGroupListStore()->UpdateGroupAvatarIndex(msg->get_groupid(), msg->get_avatar())){
		UC_LOG(ERROR) << "GroupService::OnAddMemberNotify save group avatar failed";
	}

	for (auto & info : msg->get_memberList())
	{
		MemberInfo memberInfo(info.memberId,info.join_time);
		theStore.GetGroupListStore()->AddMemberInGroup(msg->get_groupid(), memberInfo, 
			[&](int32_t id1, int32_t id2) -> bool{
			if (id1 == id2)
			{
				GroupInfo info;
				GetGroupInfo(msg->get_groupid(), info, true);
				return true;
			}
			return false;
		}(GetCurUser().GetUserId(), memberInfo.userId));
	}
}

void GroupService::OnKickMemberNotify(MessageChannel channel, UCHead head, UCMsg& ucMsg, MsgSendState state,bool& isSave){

	isSave = true;
	if (UCAS == channel)
	{
		head.SetId(DBManager::GetInstance().GetMessageID());
		ucMsg.SetHead(head);
	}

	GroupMemberKickContentMsg *msg = (GroupMemberKickContentMsg*)&ucMsg;
	if(!msg){
		isSave = false;
		UC_LOG(ERROR) << "GroupService::OnRemoveMemberNotify msg is null";
		return;
	}

	MessageInfo message;

	message.InfoFromUCHead(head);
	message.SetSendState(state);

	triggerEvent(IGroupServiceEvent, OnGroupMemberKickMessageReceived(channel, message, msg->get_operatorid(), msg->get_groupid(),
			msg->get_memberList(), msg->get_avatar(), msg->get_group_name(), msg->get_name_pinyin(), msg->get_name_flag()));
	
	//实时消息才处理;
	if (UCAS == channel){

		// 操作者需要处理踢人群聊名字，头像更新的逻辑 （ucc返回没有这两项）
		if (!theStore.GetGroupListStore()->UpdateGroupName(msg->get_groupid(), msg->get_group_name(), msg->get_name_pinyin())){
			UC_LOG(ERROR) << "GroupService::OnRemoveMemberNotify save group name failed";
		}

		if (!theStore.GetGroupListStore()->UpdateGroupAvatarIndex(msg->get_groupid(), msg->get_avatar())){
			UC_LOG(ERROR) << "GroupService::OnRemoveMemberNotify save group avatar failed";
		}
	}

	
	//从UCAS过来的消息并且不是当前用户才会操作memory;
	if (channel != UCAS || IsCurUser(head.GetFrom())) {
		return;
	}

	for (auto info : msg->get_memberList())
	{
		theStore.GetGroupListStore()->DeleteMember(msg->get_groupid(), info, [](int32_t id1, int32_t id2)->bool{return id1 == id2; }(GetCurUser().GetUserId(), info));
	}
}

void GroupService::OnUpdateNameNotify(MessageChannel channel, UCHead head, UCMsg& ucMsg, MsgSendState state, bool& isSave){

	isSave = true;
	if (UCAS == channel)
	{
		head.SetId(DBManager::GetInstance().GetMessageID());
		ucMsg.SetHead(head);
	}

	GroupNameUpdateContentMsg *msg = (GroupNameUpdateContentMsg*)&ucMsg;
	if(!msg){
		isSave = false;
		UC_LOG(ERROR) << "GroupService::OnUpdateNameNotify msg is null";
		return;
	}

	MessageInfo message;
	message.InfoFromUCHead(head);
	message.SetSendState(state);
	triggerEvent(IGroupServiceEvent, OnGroupNameUpdateMessageReceived(channel, message, msg->get_operatorid(), msg->get_groupid(),
			msg->get_group_name(), msg->get_name_pinyin(), msg->get_name_flag()));

	//从UCAS过来的消息并且不是当前用户才会操作memory;
	if (UCAS != channel || IsCurUser(head.GetFrom())) {
		return;
	}

	theStore.GetGroupListStore()->UpdateGroupName(msg->get_groupid(), msg->get_group_name(), msg->get_name_pinyin());

}

void GroupService::OnUpdateAvatarNotify(MessageChannel channel, UCHead head, UCMsg& ucMsg, MsgSendState state,bool& isSave){

	isSave = true;
	if (UCAS == channel)
	{
		head.SetId(DBManager::GetInstance().GetMessageID());
		ucMsg.SetHead(head);
	}


	GroupAvatarUpdateContentMsg *msg = (GroupAvatarUpdateContentMsg*)&ucMsg;
	if(!msg){
		isSave = false;
		UC_LOG(ERROR) << "GroupService::OnUpdateAvatarNotify msg is null";
		return;
	}

	MessageInfo message;
	message.InfoFromUCHead(head);
	message.SetSendState(state);
	triggerEvent(IGroupServiceEvent, OnGroupAvatarUpdateMessageReceived (channel, message, msg->get_operatorid(),
			msg->get_avatar(), msg->get_groupid()));


	//从UCAS过来的消息并且不是当前用户才会操作memory;
	if (UCAS != channel || IsCurUser(head.GetFrom())) {
		return;
	}

	theStore.GetGroupListStore()->UpdateGroupAvatarIndex(msg->get_groupid(), msg->get_avatar());
}

void GroupService::OnSetDisturbNotify(MessageChannel channel, UCHead head, UCMsg& ucMsg, MsgSendState state,bool& isSave){

	isSave = false;

	MsgNotifyContentMsg *msg = (MsgNotifyContentMsg*)&ucMsg;
	if (!msg) {
		UC_LOG(ERROR) << "GroupService::OnSetDisturbNotify msg is null";
		return;
	}
	
	MessageInfo message;
	message.InfoFromUCHead(head);
	message.SetSendState(state);
	triggerEvent(IGroupServiceEvent, OnGroupDisturbSetMessageReceived(channel, message, msg->get_groupid(), msg->get_state()));

	//从UCAS过来的消息并且不是当前用户才会操作memory;
	if (UCAS != channel || IsCurUser(head.GetFrom())) {
		return;
	}

	theStore.GetGroupListStore()->SetGroupDisturb(msg->get_groupid(), msg->get_state());
}

void GroupService::OnSetTopNotify(MessageChannel channel, UCHead head, UCMsg& ucMsg, MsgSendState state, bool& isSave){

	isSave = false;
	if (UCAS == channel && IsCurUser(head.GetFrom())) {
		return;
	}

	GroupTopContentMsg *msg = (GroupTopContentMsg*)&ucMsg;
	if(!msg){
		UC_LOG(ERROR) << "GroupService::OnSetTopNotify msg is null";
		return;
	}
	
	MessageInfo message;
	message.InfoFromUCHead(head);
	message.SetSendState(state);
	triggerEvent(IGroupServiceEvent, OnGroupTopMessageReceived(channel, message, msg->get_groupid(), msg->get_operatorid(),
			msg->get_isTop()));

	//从UCAS过来的消息并且不是当前用户才会操作memory;
	if (UCAS != channel || IsCurUser(head.GetFrom())) {
		return;
	}

	theStore.GetGroupListStore()->SetGroupTop(msg->get_groupid(), msg->get_isTop());
}

void GroupService::OnSetReminderNotify(MessageChannel channel, UCHead head, UCMsg& ucMsg, MsgSendState state, bool& isSave){

	isSave = false;

	AlertRuleContentMsg *msg = (AlertRuleContentMsg*)&ucMsg;
	if(!msg){
		UC_LOG(ERROR) << "GroupService::OnSetReminderNotify msg is null";
		return;
	}

	MessageInfo message;
	message.InfoFromUCHead(head);
	message.SetSendState(state);
	triggerEvent(IGroupServiceEvent, OnGroupReminderSetMessageReceived(channel, message, msg->get_groupid(), msg->get_state(), msg->get_rule(), msg->get_time()));

	//从UCAS过来的消息并且不是当前用户才会操作memory;
	if (UCAS != channel || IsCurUser(head.GetFrom())) {
		return;
	}

	theStore.GetGroupListStore()->SetGroupAlertCondition(msg->get_groupid(), msg->get_state(), msg->get_rule(), msg->get_time());
}

void GroupService::OnUploadDocumentNotify(MessageChannel channel, int32_t operation, UCHead head, UCMsg& ucMsg, 
	MsgSendState state, bool& isSave, bool& saveUnread){

	isSave = true;
	saveUnread = true;
	if (UCAS == channel)
	{
		head.SetId(DBManager::GetInstance().GetMessageID());
		ucMsg.SetHead(head);
	}

	if (UCAS == channel && IsCurUser(head.GetFrom())) {
		return;
	}

	SummaryCreateContentMsg *msg = (SummaryCreateContentMsg*)&ucMsg;
	if (NULL == msg){
		isSave = false;
		UC_LOG(ERROR) << "GroupService::OnUploadDocumentNotify msg is null";
		return;
	}
	DocumentInfo docInfo;
	docInfo.SetFileId(msg->get_contentId());
	docInfo.SetFileName(msg->get_summary_name());
	docInfo.SetFileFormat(GetFileFormat(msg->get_summary_name()));
	docInfo.SetCreatorId(msg->get_operatorid());
	docInfo.SetFileUrl(msg->get_down_url());
	docInfo.SetFileSize(msg->get_size());
	docInfo.SetContentType(0);

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

	triggerEvent(IGroupServiceEvent, OnDocumentUploadMessageReceived(channel, operation, message, docInfo));

	UC_LOG(INFO) << "group_service. OnUploadDocumentNotify. filename: " << docInfo.GetFileName()
		<< " fileURL: " << docInfo.GetFileUrl() << " filesize: " << docInfo.GetFileSize()
		<< " fileId: " << docInfo.GetFileId() << " creator: " << docInfo.GetCreatorId();

	boost::unique_lock<boost::recursive_mutex> lock(_documentMutex);
	AddDocument(head.GetTo().GetUserId(), docInfo);
}

void GroupService::OnUploadCodeSnippetNotify(MessageChannel channel, int32_t operation, UCHead head, UCMsg& ucMsg,
	MsgSendState state, bool& isSave, bool& saveUnread)
{
	isSave = true;
	saveUnread = true;
	if (UCAS == channel)
	{
		head.SetId(DBManager::GetInstance().GetMessageID());
		ucMsg.SetHead(head);
	}

	if (UCAS == channel && IsCurUser(head.GetFrom())) {
		return;
	}

	CodeCreateContentMsg *msg = (CodeCreateContentMsg*)&ucMsg;
	if (NULL == msg){
		isSave = false;
		UC_LOG(ERROR) << "GroupService::OnUploadCodeSnippetNotify msg is null";
		return ;
	}

	DocumentInfo docInfo;
	docInfo.SetFileId(msg->get_contentId());
	docInfo.SetFileName(msg->get_title());
	docInfo.SetCreatorId(msg->get_operatorid());
	docInfo.SetCreatorName(msg->get_operatorName());
	docInfo.SetFileUrl(msg->get_downUrl());
	docInfo.SetFileSize(msg->get_size());
	docInfo.SetFileFormat(msg->get_langType());
	docInfo.SetTotalLines(msg->get_totalLines());
	docInfo.SetThumbnail(msg->get_contentHead());
	docInfo.SetDescription(msg->get_desc());
	docInfo.SetCreateTime(msg->get_createTime());
	docInfo.SetContentType(1);

	MessageInfo	message;

	message.InfoFromUCHead(head);
	message.SetSendState(state);

	triggerEvent(IGroupServiceEvent, OnCodeSnippetUploadMessageReceived(channel, operation, message, docInfo));

	UC_LOG(INFO) << "group_service. OnUploadCodeSnippetNotify. filename: " << docInfo.GetFileName()
		<< " fileURL: " << docInfo.GetFileUrl() << " filesize: " << docInfo.GetFileSize()
		<< " fileId: " << docInfo.GetFileId() << " creator: " << docInfo.GetCreatorId();

	boost::unique_lock<boost::recursive_mutex> lock(_documentMutex);
	AddDocument(head.GetTo().GetUserId(), docInfo);
}

void GroupService::OnDeleteDocumentNotify(MessageChannel channel, UCHead head, UCMsg& ucMsg, MsgSendState state, bool& isSave){

	isSave = true;
	if (UCAS == channel)
	{
		head.SetId(DBManager::GetInstance().GetMessageID());
		ucMsg.SetHead(head);
	}

	DocumentDelContentMsg *msg = (DocumentDelContentMsg*)&ucMsg;
	if (NULL == msg){
		isSave = false;
		UC_LOG(ERROR) << "GroupService::OnDeleteDocumentNotify msg is null";
		return;
	}

	MessageInfo message;

	message.InfoFromUCHead(head);
	message.SetSendState(state);

	triggerEvent(IGroupServiceEvent, OnDocumentDeleteMessageReceived(channel, message, msg->get_operatorid(), msg->get_contentId(),
			msg->get_doc_name()));

	boost::unique_lock<boost::recursive_mutex> lock(_documentMutex);
	RemoveDocumentInfoById(head.GetTo().GetUserId(), msg->get_contentId());
}

void GroupService::OnCreateCommentNotify(MessageChannel channel, UCHead head, UCMsg& ucMsg, MsgSendState state, bool& isSave, bool& saveUnread){

	isSave = true;
	saveUnread = true;
	if (UCAS == channel){
		head.SetId(DBManager::GetInstance().GetMessageID());
		ucMsg.SetHead(head);
	}
	//TODO： 操作者是否过滤此消息待定，看UI需求；

	CommentCreateContentMsg *msg = (CommentCreateContentMsg*)&ucMsg;
	if (NULL == msg){
		isSave = false;
		UC_LOG(ERROR) << "GroupService::OnCreateCommentNotify msg is null";
		return;
	}

	MessageInfo message;
	message.InfoFromUCHead(head);
	message.SetSendState(state);
	
	CommentInfo commentInfo;
	commentInfo.SetFileId(msg->get_contentId());
	commentInfo.SetFileTitle(msg->get_contentName());
	commentInfo.SetFileType(msg->get_contentType());
	commentInfo.SetCommentId(msg->get_commentId());
	commentInfo.SetUserId(msg->get_operatorid());
	commentInfo.SetUserName(msg->get_operatorName());
	commentInfo.SetCreateTime(msg->get_createTime());
	commentInfo.SetContent(StringConvert::URLDecode(msg->get_commentDesc()));
	commentInfo.SetCloudType(DocumentCloudType(msg->get_cloudType()));

	triggerEvent(IGroupServiceEvent, OnCommentCreateMessageReceived(channel, message, commentInfo));
}

void GroupService::OnTextMessageNotify(MessageChannel channel, UCHead head, UCMsg& ucMsg, MsgSendState state, bool& isSave, bool& saveUnread){

	isSave = false;
	if (UCAS == channel && IsCurUser(head.GetFrom())) {
		return;
	}

	if (UCAS == channel){
		head.SetId(DBManager::GetInstance().GetMessageID());
		ucMsg.SetHead(head);

		//缓存实时的@自己的消息
		std::vector<UCID> vAtUsers = head.GetGroupAtUsers();
		for (const UCID& ucid : vAtUsers) {
			if (ucid.GetUserId() == GetCurUser().GetUserId()) {
				CacheAtMsg(head.GetTo().GetUserId(), AtMsgInfo(head.GetFrom().GetUserId(), head.GetSeq()));
			}
		}
	}

	TextContentMsg *msg = (TextContentMsg*)&ucMsg;
	if (NULL == msg){
		UC_LOG(ERROR) << "GroupService::OnTextNotify msg is null";
		return;
	}

	isSave = true;
	saveUnread = true;
	TextMessageInfo messageInfo;
	
	messageInfo.InfoFromUCHead(head);
	messageInfo.SetTextType(msg->get_type());
	messageInfo.SetTextContent(msg->get_text());
	messageInfo.SetTextFormat(msg->get_text_style());
	messageInfo.SetSendState(state);

	UC_LOG(INFO) << "Group OnTextMessageNotify. groupid: " << head.GetTo().GetUserId() << 
		" text: " << messageInfo.GetTextContent();

	triggerEvent(IGroupServiceEvent, OnTextMessageReceived(channel, &messageInfo));
}

void GroupService::OnMediaMessageNotify(MessageChannel channel, UCHead head, UCMsg& ucMsg, MsgSendState state, bool& isSave, bool& saveUnread){

	isSave = false;
	if (UCAS == channel && IsCurUser(head.GetFrom())) {
		return;
	}

	if (UCAS == channel){
		head.SetId(DBManager::GetInstance().GetMessageID());
		ucMsg.SetHead(head);
	}

	MediaContentMsg *msg = (MediaContentMsg*)&ucMsg;
	if (NULL == msg){
		UC_LOG(ERROR) << "GroupService::OnMediaNotify msg is null";
		return;
	}

	isSave = true;
	saveUnread = true;
	MediaMessageInfo messageInfo;

	messageInfo.InfoFromUCHead(head);
	messageInfo.SetFileName(msg->get_media_filename());
	messageInfo.SetFileSize(msg->get_media_filesize());
	messageInfo.SetFileUrl(msg->get_media_down_url());
	messageInfo.SetSendState(state);
	// 从db中查找是否有本地文件
	if (channel == HISTORY || channel == LIST || channel == CHAT || channel == SEARCH) {
		std::string localPath = "";

		DBManager::GetInstance().GetFSLocalPath(messageInfo.GetFileUrl(), localPath);
		messageInfo.SetFilePath(localPath);
	}

	if (ChatMediaType::MediaTypeImage ==  msg->get_media_type()) {
		messageInfo.SetThumbnails(msg->get_media_thumb());
		triggerEvent(IGroupServiceEvent, OnImageMessageReceived(channel, &messageInfo));

	} else if(ChatMediaType::MediaTypeAudio ==  msg->get_media_type()) {
		messageInfo.SetDuration(msg->get_media_duration());
		triggerEvent(IGroupServiceEvent, OnAudioMessageReceived(channel, &messageInfo));

	} else if(ChatMediaType::MediaTypeVideo ==  msg->get_media_type()) {
		messageInfo.SetThumbnails(msg->get_media_thumb());
		messageInfo.SetDuration(msg->get_media_duration());
		triggerEvent(IGroupServiceEvent, OnVideoMessageReceived(channel, &messageInfo));
	} else if (ChatMediaType::MediaTypeMeetingRecord == msg->get_media_type()) {
		messageInfo.SetThumbnails(msg->get_media_thumb());
		messageInfo.SetThumbnailsURL(msg->get_media_thumb_url());
		messageInfo.SetDuration(msg->get_media_duration());
		messageInfo.SetOperatorId(msg->get_operatorId());
		messageInfo.SetOperatorName(msg->get_operatorName());
		messageInfo.SetOperatorAvatar(msg->get_operatorAvatar());
		triggerEvent(IGroupServiceEvent, OnRecordVideoReceived(channel, &messageInfo));
	 }
}

void GroupService::OnReportMessageNotify(MessageChannel channel, UCHead head, UCMsg& ucMsg, MsgSendState state, bool& isSave){
	
	isSave = false;
	if (UCAS == channel && IsCurUser(head.GetFrom())) {
		return;
	}

	if (UCAS == channel){
		head.SetId(DBManager::GetInstance().GetMessageID());
		ucMsg.SetHead(head);
	}

	ReportContentMsg *msg = (ReportContentMsg*)&ucMsg;
	if (NULL == msg){
		UC_LOG(ERROR) << "GroupService::OnReportMessageNotify msg is null";
		return;
	}

	isSave = true;
	ReportMessageInfo messageInfo;

	messageInfo.InfoFromUCHead(head);
	messageInfo.SetReportTitle(msg->get_reportTitle());
	messageInfo.SetReportUrl(msg->get_reportUrl());
	messageInfo.SetEventId(msg->get_eventId());
	messageInfo.SetExternalData(msg->get_externalData());
	messageInfo.SetSendState(state);

	UC_LOG(INFO) << "Group OnReportMessageNotify. groupid: " << head.GetTo().GetUserId();

	triggerEvent(IGroupServiceEvent, OnReportMessageReceived(channel, &messageInfo));
}


void GroupService::OnStatusInfoUpdate(MessageChannel channel, UCHead head, UCMsg& ucMsg, MsgSendState state, bool& isSave) {
	isSave = false;

	if (channel != UCAS) // group status (call status,不用存)
		return;

	if (UCAS == channel && IsCurUser(head.GetFrom())) {
		return;
	}

	GroupStateUpdateContentMsg *msg = (GroupStateUpdateContentMsg*)&ucMsg;
	if (NULL == msg){
		UC_LOG(ERROR) << "GroupService::OnStatusInfoUpdate msg is null";
		return;
	}

	MessageInfo message;
	message.InfoFromUCHead(head);
	message.SetSendState(state);

	int groupId = head.GetTo().GetUserId();
	int status = msg->get_group_status();
	string statusInfo = msg->get_status_info();
	theStore.GetGroupListStore()->UpdateStatusInfo(groupId, statusInfo);

	UC_LOG(INFO) << "OnStatusInfoUpdate, groupId:" << groupId << ", groupStatus:" << status << ", statusInfo:" << statusInfo;

	triggerEvent(IGroupServiceEvent, OnGroupStatusInfoUpdateMessageReceived(channel, message, groupId, status, statusInfo));
}

void GroupService::OnMessageReadNotify(MessageChannel channel, UCHead head, UCMsg& ucMsg, MsgSendState state, bool& isSave){

	isSave = false;
	if (UCAS == channel && IsCurUser(head.GetFrom())) {
		return;
	}

	AckReadMsg *msg = (AckReadMsg*)&ucMsg;
	if (NULL == msg){
		UC_LOG(ERROR) << "GroupService::OnMessageReadNotify msg is null";
		return;
	}

	ConversationType type = NONE_CHAT;
	bool flag = false;
	if (SessionType::GroupChat == msg->get_sessionType()){
		flag = true;
		type = GROUP_CHAT;
	}
	else if (SessionType::DiscussionChat == msg->get_sessionType()){
		flag = true;
		type = PROJECT_CHAT;
	}
	else if (SessionType::ConferenceType == msg->get_sessionType()){
		flag = true;
		type = CONFERENCE_CHAT;
	}
	else if (SessionType::CloudFileChat == msg->get_sessionType()){
		flag = true;
		type = CLOUD_CHAT;
	}

	if (flag && (head.GetFrom().GetUserId() == GetCurUser().GetUserId())){
		triggerEvent(IGroupServiceEvent, OnMessageRead(UC_SUCCEEDED, type, head.GetTo().GetUserId(), 0, msg->get_id(), msg->get_seq()));
	}

	if (flag && (head.GetFrom().GetUserId() != GetCurUser().GetUserId())){
	
		UC_LOG(INFO) << "GroupService::OnMessageReadNotify  groupId: " << head.GetTo().GetUserId() <<" acked userId : " << msg->get_ackedUser().userID
			<< " read message seq :" << msg->get_seq();

		theStore.GetMessageReadStore()->UpdateMessageReadInfo(ConvKey(type, head.GetTo().GetUserId()), msg->get_seq(), msg->get_ackedUser().userID);
		
		MessageReadInfo info;
		std::list<MessageReadInfo> list;
		theStore.GetMessageReadStore()->GetMessageReadList(ConvKey(type, head.GetTo().GetUserId()), msg->get_seq(), info);
		list.push_back(info);
		triggerEvent(IGroupServiceEvent, OnMessageUnreadUserCount(UC_SUCCEEDED, type, head.GetTo().GetUserId(), list));
	}
}

void GroupService::OnMessageBatchReadNotify(MessageChannel channel, UCHead head, UCMsg& ucMsg, MsgSendState state, bool& isSave){
	isSave = false;
	if (UCAS == channel && IsCurUser(head.GetFrom())) {
		return;
	}

	BatchAckReadMsg *msg = (BatchAckReadMsg*)&ucMsg;
	if (NULL == msg){
		UC_LOG(ERROR) << "GroupService::OnMessageBatchReadNotify msg is null";
		return;
	}
	
	ConversationType type = NONE_CHAT;
	bool flag = false;
	if (SessionType::GroupChat == msg->get_sessionType()){
		flag = true;
		type = GROUP_CHAT;
	}else if (SessionType::DiscussionChat == msg->get_sessionType()){
		flag = true;
		type = PROJECT_CHAT;
	}else if (SessionType::ConferenceType == msg->get_sessionType()){
		flag = true;
		type = CONFERENCE_CHAT;	
	}
	else if (SessionType::CloudFileChat == msg->get_sessionType()){
		flag = true;
		type = CLOUD_CHAT;
	}

	if (flag && (msg->get_ackedUser().userID == GetCurUser().GetUserId())){
		triggerEvent(IGroupServiceEvent, OnMessageRead(UC_SUCCEEDED, type, head.GetTo().GetUserId(), 1, 0, 0));
	}

	if (flag && (msg->get_ackedUser().userID != GetCurUser().GetUserId())){
		UC_LOG(INFO) << "GroupService::OnMessageBatchReadNotify groupId: " << head.GetTo().GetUserId() << " acked userId: " << msg->get_ackedUser().userID
			<< " message send time :" << head.GetTimestamp() << " seq list size:" << msg->get_seqList().size();
		
		for (auto seq : msg->get_seqList()){
			theStore.GetMessageReadStore()->UpdateMessageReadInfo(ConvKey(type, head.GetTo().GetUserId()), seq, msg->get_ackedUser().userID);
		}
		std::list<MessageReadInfo> list;
		theStore.GetMessageReadStore()->GetMessageReadList(ConvKey(type, head.GetTo().GetUserId()), msg->get_seqList(), list);
		triggerEvent(IGroupServiceEvent, OnMessageUnreadUserCount(UC_SUCCEEDED, type, head.GetTo().GetUserId(), list));
	}
}

void GroupService::OnMessagePlayedNotify(MessageChannel channel, UCHead head, UCMsg& ucMsg, MsgSendState state, bool& isSave){

	isSave = false;
	if (UCAS == channel && IsCurUser(head.GetFrom())) {
		return;
	}

	AckPlayMsg *msg = (AckPlayMsg*)&ucMsg;
	if (NULL == msg){
		UC_LOG(ERROR) << "GroupService::OnMessagePlayedNotify msg is null";
		return;
	}
	triggerEvent(IGroupServiceEvent, OnMessagePlayed(UC_SUCCEEDED, head.GetTo().GetUserId(), msg->get_id(), msg->get_seq()));
	if (head.GetFrom().GetUserId() == GetCurUser().GetUserId()){
		int64_t conversation_type = MessageProcessor::GetConversationType( head.GetAppId(), head.GetProtocoltype() );
		theStore.GetMsgStore()->SetMessageStatusBySeq(head.GetTo().GetUserId(), head.GetSeq(), conversation_type, 1);
	}
}

void GroupService::OnAtMessageReadNotify(MessageChannel channel, UCHead head, UCMsg& ucMsg, MsgSendState state, bool& isSave){
	
	isSave = false;
	if (UCAS == channel && IsCurUser(head.GetFrom())) {
		return;
	}

	AtMessageReadedContentMsg *msg = (AtMessageReadedContentMsg*)&ucMsg;
	if (NULL == msg){
		UC_LOG(ERROR) << "GroupService::AtMessageReadedContentMsg msg is null";
		return;
	}

	ConversationType type = NONE_CHAT;
	bool flag = false;
	if (SessionType::GroupChat == msg->get_sessionType()){
		flag = true;
		type = GROUP_CHAT;
	}
	else if (SessionType::DiscussionChat == msg->get_sessionType()){
		flag = true;
		type = PROJECT_CHAT;
	}
	else if (SessionType::ConferenceType == msg->get_sessionType()){
		flag = true;
		type = CONFERENCE_CHAT;
	}
	else if (SessionType::CloudFileChat == msg->get_sessionType()){
		flag = true;
		type = CLOUD_CHAT;
	}

	if (flag && (head.GetFrom().GetUserId() == GetCurUser().GetUserId())){
		UC_LOG(INFO) << "OnAtMessageReadNotify delete at message from cache. to userId = " << msg->get_to().userID;
		DeleteAtMsgFromCache(msg->get_to().userID, msg->get_seqList());
		triggerEvent(IGroupServiceEvent, OnAtMessageDeleted(UC_SUCCEEDED, type, msg->get_to().userID, msg->get_seqList()));
	}
}

//TODO:群、项目组呼叫需求待定;
void GroupService::OnInviteCallNotify(MessageChannel channel, ConversationType type, UCHead head, UCMsg& ucMsg, MsgSendState state, bool& isSave){

#ifdef AUDIO_SERVICE
	AudioService* audioService = (AudioService*)_engine->QueryService(UCAudioService);
	audioService->ChangeGroupCallType(GROUP_CHAT); //讨论组
#endif
    
	isSave = false;
	if (UCAS == channel)
	{
		head.SetId(DBManager::GetInstance().GetMessageID());
		ucMsg.SetHead(head);
	}

	if (UCAS == channel && IsCurUser(head.GetFrom())) {
		return;
	}

	AudioInviteContentMsg *msg = (AudioInviteContentMsg*)&ucMsg;
	if (NULL == msg){
		isSave = false;
		UC_LOG(ERROR) << "GroupService::OnInviteCallNotify msg is null";
		return;
	}
	isSave = true;

	triggerEvent(IGroupServiceEvent, OnCallInvited(channel, UC_SUCCEEDED, type,
		head.GetFrom().GetUserId(), head.GetTo().GetUserId(), head.GetId(), head.GetSeq(), head.GetTimestamp(), 
		msg->get_confId(), msg->get_confPwd(), msg->get_hostId()));
}

void GroupService::OnAcceptCallNotify(MessageChannel channel, ConversationType type, UCHead head, UCMsg& ucMsg, MsgSendState state, bool& isSave){

	isSave = false;
	if (UCAS == channel && IsCurUser(head.GetFrom())) {
		return;
	}

	AudioReceiveContentMsg *msg = (AudioReceiveContentMsg*)&ucMsg;
	if (NULL == msg){
		UC_LOG(ERROR) << "GroupService::OnAcceptCallNotyfy msg is null";
		return;
	}
	isSave = true;
	triggerEvent(IGroupServiceEvent, OnCallAccepted(channel, UC_SUCCEEDED, type, 
		head.GetFrom().GetUserId(), head.GetTo().GetUserId(), msg->get_confId(),
		head.GetId(), head.GetSeq(), head.GetTimestamp()));
}

void GroupService::OnRejectCallNofify(MessageChannel channel, ConversationType type, UCHead head, UCMsg& ucMsg, MsgSendState state, bool& isSave){

	isSave = false;
	if (UCAS == channel && IsCurUser(head.GetFrom())) {
		return;
	}

	AudioRejectContentMsg *msg = (AudioRejectContentMsg*)&ucMsg;
	if (NULL == msg){
		UC_LOG(ERROR) << "GroupService::OnRejectCallNofify msg is null";
		return;
	}

	triggerEvent(IGroupServiceEvent, OnCallRejected(channel, UC_SUCCEEDED, type, 
		head.GetFrom().GetUserId(), head.GetTo().GetUserId(), msg->get_confId(), msg->get_hostId(), msg->get_reason(),
		head.GetId(), head.GetSeq(), head.GetTimestamp()));
}

void GroupService::OnFinishCallNotify(MessageChannel channel, ConversationType type, UCHead head, UCMsg& ucMsg, MsgSendState state, bool& isSave){

	isSave = false;
	if (UCAS == channel && IsCurUser(head.GetFrom())) {
		return;
	}

	AudioStopContentMsg *msg = (AudioStopContentMsg*)&ucMsg;
	if (NULL == msg){
		UC_LOG(ERROR) << "GroupService::OnFinishCallNotify msg is null";
		return;
	}
	isSave = true;
	triggerEvent(IGroupServiceEvent, OnCallStopped(channel, UC_SUCCEEDED, type, 
		head.GetFrom().GetUserId(), head.GetTo().GetUserId(), msg->get_confId(),
		head.GetId(), head.GetSeq(), head.GetTimestamp()));
}

void GroupService::OnConnectedCallNotify(MessageChannel channel, ConversationType type, UCHead head, UCMsg& ucMsg, MsgSendState state,bool& isSave){

	isSave = false;
	if (UCAS == channel && IsCurUser(head.GetFrom())) {
		return;
	}

	AudioRecentCallContentMsg *msg = (AudioRecentCallContentMsg*)&ucMsg;
	if (NULL == msg){
		UC_LOG(ERROR) << "GroupService::OnConnectedCallNotify msg is null";
		return;
	}
	isSave = true;
	triggerEvent(IGroupServiceEvent, OnCallConnected(channel, UC_SUCCEEDED, type, 
		head.GetFrom().GetUserId(), head.GetTo().GetUserId(), msg->get_duration(), msg->get_phoneNum(),
		head.GetId(), head.GetSeq(), head.GetTimestamp()));
}

void GroupService::OnCancelCallNotify(MessageChannel channel, ConversationType type, UCHead head, UCMsg& ucMsg, MsgSendState state, bool& isSave) {
	isSave = false;
	if (UCAS == channel && IsCurUser(head.GetFrom())) {
		return;
	}

	AudioCancelCallContentMsg *msg = (AudioCancelCallContentMsg*)&ucMsg;
	if (NULL == msg){
		UC_LOG(ERROR) << "GroupService::OnCancelCallNotify msg is null";
		return;
	}
	isSave = true;
	triggerEvent(IGroupServiceEvent, OnCallCanceled(channel, UC_SUCCEEDED, type, 
		head.GetFrom().GetUserId(), head.GetTo().GetUserId(), head.GetId(), head.GetSeq(), head.GetTimestamp(), msg->get_phoneNum()));
}

void GroupService::OnMessageReadUserNotify(ConversationType type, int64_t groupId, int64_t msgSeq){


	int32_t  count = GetGroupMemberCount(groupId);
	if (MESSAGE_UNREAD_DEFAULT == count){
		std::list<int64_t> seqlist;
		seqlist.push_back(msgSeq);
		GetUnreadUserCount(type, groupId, seqlist);
		
	}
	else
	{
		std::list<MessageReadInfo> list;
		MessageReadUserInfo info;

		info.SetMsgSeq(msgSeq);
		info.SetConversationType(type);
		info.SetRelateId(groupId);
		info.SetUnreadCount(GetGroupMemberCount(groupId));
		if (info.GetUnreadCount() == MESSAGE_UNREAD_NONE){
			info.SetAllRead(true);
		}
		UserIdList userIdList = GetGroupMemberList(groupId);
		if (!userIdList.empty()){
			info.AddUser(userIdList);
		}
		list.push_back(info);
		theStore.GetMessageReadStore()->SaveMessageReadInfo(ConvKey(type, groupId), info);

		triggerEvent(IGroupServiceEvent, OnMessageUnreadUserCount(UC_SUCCEEDED, type, groupId, list));
	}	
}

void GroupService::OnRevocationNotify(MessageChannel channel, UCHead head, UCMsg& ucMsg, MsgSendState state, bool& isSave)
{
	isSave = true;
	if (UCAS == channel)
	{
		head.SetId(DBManager::GetInstance().GetMessageID());
		ucMsg.SetHead(head);
	}

	if (UCAS == channel && IsCurUser(head.GetFrom())) {
		return;
	}

	RevocationContentMsg *msg = (RevocationContentMsg*)&ucMsg;
	if (NULL == msg){
		UC_LOG(ERROR) << "GroupService::OnRevocationNotify msg is null";
		return;
	}

	MessageInfo message;
	message.InfoFromUCHead(head);
	message.SetSendState(state);

	if (head.GetFrom().GetUserId() != GetCurUser().GetUserId() && channel == UCAS)
	{
		ConversationType conversation_type = MessageProcessor::GetConversationType(head.GetAppId(), head.GetProtocoltype());
		int64_t relate_id = MessageProcessor::GetRelateId(conversation_type, GetCurUser().GetUserId(), head.GetFrom().GetUserId(), head.GetTo().GetUserId());
		theStore.GetMsgStore()->UpdateRevocationMessageState(relate_id, msg->get_seq(), conversation_type);
	}
	
	triggerEvent(IGroupServiceEvent, OnRevocationMesssageReceived(channel, message, msg->get_seq(), msg->get_type()));
}

void GroupService::OnCloudFileModifiedNotify(MsgType msgType, MessageChannel channel, UCHead head, UCMsg& ucMsg, MsgSendState state, bool& isSave) {
	CloudFileOperation operation = GetCloudFileOperation(msgType);

	isSave = false;
	// 云盘讨论组消息通过ucas通知UI
	if ((!IsUCCloudDiscussionCloudFileMsg(msgType)) && UCAS == channel && IsCurUser(head.GetFrom())) {
		// 项目组中调uploadclouddocument，保存ucas消息
		if (msgType == UCDiscussionCloudFileCreate || msgType == UCGroupCloudFileCreate) {
			isSave = true;
		}
		return;
	}

	if (UCAS == channel)
	{
		head.SetId(DBManager::GetInstance().GetMessageID());
		ucMsg.SetHead(head);
	}

	CloudFileContentMsg *msg = (CloudFileContentMsg*)&ucMsg;
	if (NULL == msg){
		UC_LOG(ERROR) << "GroupService::OnCloudFileModifiedNotify msg is null";
		return;
	}

	isSave = true;
	FileMessageInfo messageInfo;

	messageInfo.InfoFromUCHead(head);
	messageInfo.SetFileName(msg->get_fileName());
	messageInfo.SetFileSize(msg->get_size());
	messageInfo.SetFileId(msg->get_contentId());
	messageInfo.SetCloudType(DocumentCloudType(msg->get_cloudType()));
	messageInfo.SetDetailContent(msg->get_detailContent());
	messageInfo.SetSendState(state);

	triggerEvent(IGroupServiceEvent, OnCloudFileModifiedMessageReceived(channel, operation, messageInfo));

	UC_LOG(INFO) << "GroupService. OnCloudFileModifiedNotify. filename: " << messageInfo.GetFileName()
		<< " filesize: " << messageInfo.GetFileSize() << " cloudType: " << messageInfo.GetCloudType();
}

void GroupService::OnMemberRoleUpdateNotify(MessageChannel channel, UCHead head, UCMsg& ucMsg, MsgSendState state, bool& isSave) {
	isSave = true;

	if (UCAS == channel) {
		head.SetId(DBManager::GetInstance().GetMessageID());
		ucMsg.SetHead(head);
	}

	MemberRoleUpdateContentMsg *msg = (MemberRoleUpdateContentMsg*)&ucMsg;
	if (NULL == msg) {
		UC_LOG(ERROR) << "GroupService::OnMemberRoleUpdateNotify msg is null";
		return;
	}

	RoleUpdatedMessageInfo msgInfo;
	msgInfo.InfoFromUCHead(head);
	msgInfo.SetSendState(state);
	
	const std::map<int32_t, int8_t>& roles = msg->get_roleMap();
	for (std::map<int32_t, int8_t>::const_iterator iter = roles.begin(); iter != roles.end(); ++iter) {
		msgInfo.AddRoles(iter->first, GroupRole(iter->second));
	}

	triggerEvent(IGroupServiceEvent, OnMemberRoleUpdated(channel, msgInfo));

	UC_LOG(INFO) << "GroupService. OnMemberRoleUpdateNotify. groupId: " << head.GetTo().GetUserId();
}

bool GroupService::IsUCCloudDiscussionCloudFileMsg(MsgType msgType) {
	if (UCCloudDiscussionCloudFileCreate == msgType || UCCloudDiscussionCloudFileUpdate == msgType || UCCloudDiscussionCloudFileDel == msgType ||
		UCCloudDiscussionCloudFileMove == msgType || UCCloudDiscussionCloudFileRename == msgType) {
		return true;
	}

	return false;
}

CloudFileOperation GroupService::GetCloudFileOperation(MsgType msgType) {
	switch (msgType)
	{
	case UCCloudDiscussionCloudFileCreate:
		return CFO_CloudFileCreate;
	case UCCloudDiscussionCloudFileUpdate:
		return CFO_CloudFileUpdate;
	case UCCloudDiscussionCloudFileDel:
		return CFO_CloudFileDel;
	case UCCloudDiscussionCloudFileMove:
		return CFO_CloudFileMove;
	case UCCloudDiscussionCloudFileRename:
		return CFO_CloudFileRename;
	default:
		return CFO_CloudFileCreate;
	}

}

} /* namespace uc */
