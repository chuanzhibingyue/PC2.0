#include "GroupServiceMockClient.h"


GroupServiceMockClient::GroupServiceMockClient(){


}

GroupServiceMockClient::~GroupServiceMockClient(){


}

void GroupServiceMockClient::Init(IUCEngine *ucEngine){
	_groupService = (IGroupService*)ucEngine->QueryService(UCGroupService);
	_groupService->registerListener(this);
}

void GroupServiceMockClient::UnInit(){
	_groupService->removeListener(this);
}

void GroupServiceMockClient::GetGroupList(GroupStyle groupType){
	_groupService->GetGroupList(groupType);
}

void GroupServiceMockClient::GetGroupInfo(int64_t groupid){
	
}

void GroupServiceMockClient::CreateGroup(GroupStyle groupType, const std::string& groupName, std::vector<UserInfo>& userInfo,
	int32_t nameFlag){

	_groupService->CreateGroup(groupType, groupName, userInfo, nameFlag);
}

void GroupServiceMockClient::EndGroup(int64_t gourpId){
	_groupService->EndGroup(gourpId);
}

void GroupServiceMockClient::ExitGroup(int64_t gourpId){
	_groupService->ExitGroup(gourpId);
}

void GroupServiceMockClient::AddMember(int64_t groupId, std::vector<UserInfo>& memberList){
	_groupService->AddMember(groupId, memberList);
}

void GroupServiceMockClient::RemoveMember(int64_t groupId, const std::vector<int32_t>& memberList){
	_groupService->RemoveMember(groupId, memberList);
}

void GroupServiceMockClient::SetGroupName(int64_t groupId, const std::string& groupName){
	_groupService->SetGroupName(groupId, groupName);
}

void GroupServiceMockClient::SetGroupLogo(int64_t groupId, const std::string& logoURL){
	_groupService->SetGroupLogo(groupId, logoURL);
}

void GroupServiceMockClient::SetTopChat(int64_t groupId, int32_t state){
	_groupService->SetTopChat(groupId, state);
}

void GroupServiceMockClient::SetReminder(int64_t groupId, int32_t state, int32_t alertRule, int32_t alertTime){
	_groupService->SetReminder(groupId, state, alertRule, alertTime);
}

void GroupServiceMockClient::SetDoNotDisturb(int64_t groupId, int32_t state){
	_groupService->SetDoNotDisturb(groupId, state);
}

void GroupServiceMockClient::GetDocumentList(int64_t groupId){
	_groupService->GetDocumentList(groupId);
}

void GroupServiceMockClient::GetDocumentInfo(int64_t groupId, int64_t fileId){
	_groupService->GetDocumentInfo(groupId, fileId);
}

void GroupServiceMockClient::UploadDocument(int64_t groupId, const std::string& fileName, int32_t flieSize,
	const std::string& fileURL, int64_t uploadTime){
	_groupService->UploadDocument(groupId, fileName, flieSize, fileURL, uploadTime);
}

void GroupServiceMockClient::ShareDocument(int64_t groupId, const std::string& fileName, int32_t flieSize,
	const std::string& fileURL, int64_t uploadTime){
	_groupService->ShareDocument(groupId, fileName, flieSize, fileURL, uploadTime);
}

void GroupServiceMockClient::DeleteDocument(int64_t groupId, int64_t fileId){
	_groupService->DeleteDocument(groupId, fileId);
}

void GroupServiceMockClient::GetChatHistory(ConversationType type, int64_t groupId, int32_t count, int32_t startTime, int32_t endTime){
	_groupService->GetChatHistory(type, groupId, count, startTime, endTime);
}

void GroupServiceMockClient::ResendMessage(int32_t msgId){
	_groupService->ResendMessage(msgId);
}

void GroupServiceMockClient::SendText(ConversationType type, UCID groupUid, int8_t textType, std::string& textFromat, std::string& textData, OUT int32_t& msgId){
	_groupService->SendTextMessage(type, groupUid, textType, textFromat, textData, msgId);
}

void GroupServiceMockClient::SendImage(ConversationType type, UCID groupUid, std::string& imageName, int32_t imageSize,
	std::string& imageURL, std::string& imageData, OUT int32_t& msgId){
	_groupService->SendImageMessage(type, groupUid, imageName, imageSize, imageURL, imageData, msgId);
}

void GroupServiceMockClient::SendAudio(ConversationType type, UCID groupUid, std::string& audioName, int32_t audioSize,
	std::string& audioURL, int32_t audioDuration, OUT int32_t& msgId){
	_groupService->SendAudioMessage(type, groupUid, audioName, audioSize, audioURL, audioDuration, msgId);
}

void GroupServiceMockClient::SendVideo(ConversationType type, UCID groupUid, std::string& videoName, int32_t videoSize,
	std::string& videoURL, std::string& videoData, int32_t videoDuration, OUT int32_t& msgId){
	_groupService->SendVideoMessage(type, groupUid, videoName, videoSize, videoURL, videoData, videoDuration, msgId);
}

void GroupServiceMockClient::SendInvateCall(ConversationType type, UCID groupUid, const std::string& confId,
	const std::string& confPwd, int32_t hostId, OUT int32_t& msgId){
	_groupService->SendInviteCallMessage(type, groupUid, confId, confPwd, hostId, msgId);
}

void GroupServiceMockClient::SendAppectCall(ConversationType type, UCID groupUid, const std::string& confId, OUT int32_t& msgId){
	_groupService->SendAcceptCallMessage(type, groupUid, confId, msgId);
}

void GroupServiceMockClient::SendRejectCall(ConversationType type, UCID groupUid, const std::string& confId, OUT int32_t& msgId){
	_groupService->SendRejectCallMessage(type, groupUid, confId, msgId);
}

void GroupServiceMockClient::SendFinishCall(ConversationType type, UCID groupUid, const std::string& confId, OUT int32_t& msgId){
	_groupService->SendFinishCallMessage(type, groupUid, confId, msgId);
}

void GroupServiceMockClient::SendConnectCallRecord(ConversationType type, UCID groupUid, int32_t druation, OUT int32_t& msgId){
	_groupService->SendConnectedCallRecordMessage(type, groupUid, druation, msgId);
}

void GroupServiceMockClient::SendMessageRead(ConversationType type, UCID groupUid, int32_t msgId, int64_t msgSeq){
	_groupService->SendMessageRead(type, groupUid, msgId, msgSeq);
}

void GroupServiceMockClient::SendMessagesRead(int64_t groupId, std::vector<int32_t> chatId){
	_groupService->SendMessagesRead(groupId, chatId);
}

void GroupServiceMockClient::SendMessagesPalyed(ConversationType type, UCID groupUid, int32_t msgId, int64_t msgSeq){
	_groupService->SendMessagePlayed(type, groupUid, msgId, msgSeq);
}


//Event;
void GroupServiceMockClient::OnChatHistoryReceived(int32_t result, int64_t groupId, const std::list<MessageInfo*>& msgList){
	NotifyResult(result);
	cout << "OnChatHistoryReceived result: " << result << endl;
}

void GroupServiceMockClient::OnMessageSent(int32_t result, int64_t groupId, int32_t msgId, int64_t msgSeq, int64_t msgTime){
	NotifyResult(result);
	cout << "OnMessageSent result: " << result << endl;
}

void GroupServiceMockClient::OnTextMessageReceived(const TextMessageInfo *message){
	
}

void GroupServiceMockClient::OnImageMessageReceived(const MediaMessageInfo *message){


}

void GroupServiceMockClient::OnAudioMessageReceived(const MediaMessageInfo *message){


}

void GroupServiceMockClient::OnVideoMessageReceived(const MediaMessageInfo *message){


}

void GroupServiceMockClient::OnMessageRead(int32_t result, int64_t groupId, int32_t readType, int32_t msgId, int64_t msgSeq){
	NotifyResult(result);
	cout << "OnMessageRead result: " << result << endl;
}

void GroupServiceMockClient::OnMessagePlayed(int32_t result, int64_t groupId, int32_t msgId, int64_t msgSeq){
	NotifyResult(result);
	cout << "OnMessagePlayed result: " << result << endl;
}

void GroupServiceMockClient::OnCallInvited(int32_t result, ConversationType type, int64_t groupId, int32_t msgId, int64_t msgSeq, const std::string confId, const std::string confPwd, int32_t hostId){
	NotifyResult(result);
	cout << "OnCallInvited result: " << result << endl;
}

void GroupServiceMockClient::OnCallAccepted(int32_t result, ConversationType type, int64_t groupId, const std::string confId){
	NotifyResult(result);
	cout << "OnCallAccepted result: " << result << endl;
}

void GroupServiceMockClient::OnCallRejected(int32_t result, ConversationType type, int64_t groupId, const std::string confId){
	NotifyResult(result);
	cout << "OnCallRejected result: " << result << endl;
}

void GroupServiceMockClient::OnCallStopped(int32_t result, ConversationType type, int64_t groupId, const std::string confId){
	NotifyResult(result);
	cout << "OnCallStopped result: " << result << endl;
}

void GroupServiceMockClient::OnCallConnected(int32_t result, ConversationType type, int64_t groupId, int32_t duration){
	NotifyResult(result);
	cout << "OnCallConnected result: " << result << endl;
}

void GroupServiceMockClient::OnGroupCreated(int32_t result, int32_t operatorId, const GroupInfo *info){
	NotifyResult(result);
	cout << "OnGroupCreated result: " << result << endl;
}

void GroupServiceMockClient::OnGroupExited(int32_t result, int64_t groupId, int32_t userId){
	NotifyResult(result);
	cout << "OnGroupExited result: " << result << endl;
}

void GroupServiceMockClient::OnGroupEnded(int32_t result, int64_t groupId, int32_t operatorId){
	NotifyResult(result);
	cout << "OnGroupEnded result: " << result << endl;
}

void GroupServiceMockClient::OnMemberAdded(int32_t result, int64_t groupId, int32_t operatorId, const std::vector<int32_t>& memberList){
	NotifyResult(result);
	cout << "OnMemberAdded result: " << result << endl;
}

void GroupServiceMockClient::OnMemberRemoved(int32_t result, int64_t groupId, int32_t operatorId, const std::vector<int32_t>& memberList){
	NotifyResult(result);
	cout << "OnMemberRemoved result: " << result << endl;
}

void GroupServiceMockClient::OnGroupNameUpdated(int32_t result, int64_t groupId, int32_t operatorId, 
	const std::string& groupName, const std::string& pinYinName){
	NotifyResult(result);
	cout << "OnGroupNameUpdated result: " << result << endl;
}

void GroupServiceMockClient::OnGroupLogoUpdated(int32_t result, int64_t groupId, const std::string& logoURL){
	NotifyResult(result);
	cout << "OnGroupLogoUpdated result: " << result << endl;
}

void GroupServiceMockClient::OnDisturbSetted(int32_t result, int64_t groupId, int32_t isDisturb){
	NotifyResult(result);
	cout << "OnDisturbSetted result: " << result << endl;
}

void GroupServiceMockClient::OnTopChatSetted(int32_t result, int64_t groupId, int32_t isTop){
	NotifyResult(result);
	cout << "OnTopChatSetted result: " << result << endl;
}

void GroupServiceMockClient::OnReminderSetted(int32_t result, int64_t groupId, int32_t isAlert, int32_t remindRule, int32_t remindTime){
	NotifyResult(result);
	cout << "OnReminderSetted result: " << result << endl;
}

void GroupServiceMockClient::OnGroupListReceived(int32_t result, GroupStyle groupType, const std::list<GroupInfo*>& groupList){
	NotifyResult(result);
	cout << "OnGroupListReceived result: " << result << endl;
}

void GroupServiceMockClient::OnGroupInfoReceived(int32_t result, int64_t groupId, const GroupInfo *info){
	NotifyResult(result);
	cout << "OnGroupInfoReceived result: " << result << endl;
}

void GroupServiceMockClient::OnDocListReceived(int32_t result, int64_t groupId, const std::list<DocumentInfo*>& info){
	NotifyResult(result);
	cout << "OnDocListReceived result: " << result << endl;
}

void GroupServiceMockClient::OnDocInfoReceived(int32_t result, int64_t groupId, const DocumentInfo *info){
	NotifyResult(result);
	cout << "OnDocInfoReceived result: " << result << endl;
}

void GroupServiceMockClient::OnDocumentUploaded(int32_t result, int64_t groupId, int32_t operatorId, const DocumentInfo *info){
	NotifyResult(result);
	cout << "OnDocumentUploaded result: " << result << endl;
}

void GroupServiceMockClient::OnDocumentShared(int32_t result, int64_t groupId, int32_t operatorId, const DocumentInfo *info){
	NotifyResult(result);
	cout << "OnDocumentShared result: " << result << endl;
}

void GroupServiceMockClient::OnDocumentDeleted(int32_t result, int64_t groupId, int32_t operatorId, 
	const std::string& docName, int64_t fileId){
	NotifyResult(result);
	cout << "OnDocumentDeleted result: " << result << endl;
}
