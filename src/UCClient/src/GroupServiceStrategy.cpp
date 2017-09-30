#include "GroupServiceStrategy.h"
#include "ServiceObjectConversion.h"
#include "UCDefs.h"
#include "IClientService.h"
#include "utils/Thumbnails.h"

namespace ucclient {
GroupServiceStrategy::GroupServiceStrategy(IUCEngine *engine, cefIntegration::CefClient *cefClient) 
	: _engine(engine), _cefClient(cefClient) {
	_groupService = (IGroupService*)_engine->QueryService(UCGroupService);
}

int32_t GroupServiceStrategy::GetChatHistory(MessageChannel channel, ConversationType type, int64_t groupId, int32_t count, int64_t startTime, int64_t endTime) {
	std::list<MessageInfo*> msgList;
	return _groupService->GetChatHistory(channel, type, groupId, count, startTime, endTime);
}

int32_t GroupServiceStrategy::GetSearchChatHistory(ConversationType type, int64_t relateId, int32_t count, int64_t startTime, int64_t endTime, int32_t reverse){
	return _groupService->GetSearchChatHistory(type, relateId, count, startTime, endTime, reverse);
}

int32_t GroupServiceStrategy::ResendMessage(int32_t msgId){
	return _groupService->ResendMessage(msgId);
}

int32_t GroupServiceStrategy::ForwardMessage(UCID& userUid, int32_t msgId){
	return _groupService->ForwardMessage(userUid, msgId);
}


int32_t GroupServiceStrategy::RevocationMessage(int64_t& touserId, ConversationType type, const int64_t& seqId, OUT int32_t& msgId){
	return _groupService->RevocationMessage(touserId, type, seqId, msgId);
}

int32_t GroupServiceStrategy::SendTextMessage(ConversationType type, UCID groupId, int8_t textType, std::wstring& textFormat, std::wstring& textData, std::vector<UCID>& atUsers, OUT int32_t& msgId) {

	std::string utf8Format = StringConvert::ToUTF8Str(textFormat);
	std::string utf8Text = StringConvert::ToUTF8Str(textData);
	return _groupService->SendTextMessage(type, groupId, textType, utf8Format, utf8Text, atUsers, msgId);
}

int32_t GroupServiceStrategy::SendImageMessage(ConversationType type, UCID groupId, std::wstring& imageName, int32_t imageSize,
	std::wstring& imageURL, std::wstring& imagePath, OUT int32_t& msgId) {

	std::string utf8ImageName = StringConvert::ToUTF8Str(imageName);
	std::string utf8ImageURL = StringConvert::ToUTF8Str(imageURL);
	std::wstring thumbnailPath = L"";
	std::string thunmbnailData = "";

	IClientService *service = (IClientService*)_engine->QueryService(UCClientService);
	if (service != NULL) {
		thumbnailPath = StringConvert::FromUTF8Str(service->GetDocumentDataDir(IMAGE_DIR));
	}

	ThumbnailsUtil::CreateThumbnails(imagePath, thumbnailPath, thunmbnailData);

	return _groupService->SendImageMessage(type, groupId, utf8ImageName, imageSize, utf8ImageURL, thunmbnailData, msgId);
}

int32_t GroupServiceStrategy::SendAudioMessage(ConversationType type, UCID groupId, std::wstring& audioName, int32_t audioSize,
	std::wstring& audioURL, int32_t audioDuration, OUT int32_t& msgId) {
		
	std::string utf8AudioName = StringConvert::ToUTF8Str(audioName);
	std::string utf8AudioURL = StringConvert::ToUTF8Str(audioURL);

	return _groupService->SendAudioMessage(type, groupId, utf8AudioName, audioSize, utf8AudioURL,
		audioDuration, msgId);
}

int32_t GroupServiceStrategy::SendVideoMessage(ConversationType type, UCID groupUid, std::wstring& videoName, int32_t videoSize,
	std::wstring& videoURL, std::wstring& videoPath, int32_t videoDuration, OUT int32_t& msgId) {

	std::string utf8VideoName = StringConvert::ToUTF8Str(videoName);
	std::string utf8VideoURL = StringConvert::ToUTF8Str(videoURL);
	std::string utf8VideoData = StringConvert::ToUTF8Str(videoPath);

	return _groupService->SendVideoMessage(type, groupUid, utf8VideoName, videoSize, utf8VideoURL,
		utf8VideoData, videoDuration, msgId);	
}

int32_t GroupServiceStrategy::SendReportMessage(ConversationType type, UCID groupUid, const std::wstring& reportTitle, const std::wstring& reportUrl, OUT int32_t& msgId){
	
	std::string utf8ReportTitle = StringConvert::ToUTF8Str(reportTitle);
	std::string utf8ReportURL = StringConvert::ToUTF8Str(reportUrl);

	return _groupService->SendReportMessage(type, groupUid, utf8ReportTitle, utf8ReportURL, msgId);
}

int32_t GroupServiceStrategy::SendCloudFileMessage(ConversationType type, CloudFileOperation operation, UCID groupUid, const std::wstring& fileName, 
	const std::wstring& fileNameTo, int64_t fileSize, int8_t count, int8_t isDir, int8_t cloudType, const std::wstring& detailContent, 
	bool isSave, OUT int32_t& msgId) {
	return _groupService->SendCloudFileMessage(type, operation, groupUid, StringConvert::ToUTF8Str(fileName), StringConvert::ToUTF8Str(fileNameTo),
		fileSize, count, isDir, cloudType, StringConvert::ToUTF8Str(detailContent), isSave, msgId);
}

int32_t GroupServiceStrategy::SendRecordVideoMessage(ConversationType type, UCID& groupId, const std::wstring& videoName, int32_t videoSize,
	const std::wstring& videoURL, const std::wstring& videoThumb, const std::wstring& videoThumbURL, int32_t videoDuration, int32_t operatorId, const std::wstring& operatorName,
	const std::wstring& operatorAvatar, int32_t& msgId) {
	std::string utf8VideoName = StringConvert::ToUTF8Str(videoName);
	std::string utf8VideoURL = StringConvert::ToUTF8Str(videoURL);
	std::string utf8VideoThumbData = StringConvert::ToUTF8Str(videoThumb);
	std::string utf8VideoThumbURL = StringConvert::ToUTF8Str(videoThumbURL);
	std::string utf8OperatorName = StringConvert::ToUTF8Str(operatorName);
	std::string utf8OperatorAvatar = StringConvert::ToUTF8Str(operatorAvatar);

	return _groupService->SendRecordVideoMessage(type, groupId, utf8VideoName, videoSize, utf8VideoURL,
		utf8VideoThumbData, utf8VideoThumbURL, videoDuration, operatorId, utf8OperatorName, utf8OperatorAvatar, msgId);
}

int32_t GroupServiceStrategy::SendInviteCallMessage(ConversationType type, UCID groupId, const std::wstring& confId, 
	const std::wstring& confPwd, int32_t hostId, OUT int32_t& msgId) {

		std::string utf8ConfId = StringConvert::ToUTF8Str(confId);
		std::string utf8ConfPwd = StringConvert::ToUTF8Str(confPwd);

	return _groupService->SendInviteCallMessage(type, groupId, utf8ConfId, utf8ConfPwd, hostId, msgId);	
}

int32_t GroupServiceStrategy::SendAcceptCallMessage(ConversationType type, UCID groupId, const std::wstring& confId, int32_t hostId, OUT int32_t& msgId) {

	std::string utf8ConfId = StringConvert::ToUTF8Str(confId);

	return _groupService->SendAcceptCallMessage(type, groupId, utf8ConfId, hostId, msgId);
}

int32_t GroupServiceStrategy::SendRejectCallMessage(ConversationType type, UCID groupId, const std::wstring& confId, 
	int32_t hostId, const std::wstring& reason, OUT int32_t& msgId) {
	
	std::string utf8ConfId = StringConvert::ToUTF8Str(confId);
	std::string utf8Reason = StringConvert::ToUTF8Str(reason);
	return _groupService->SendRejectCallMessage(type, groupId, utf8ConfId, hostId, utf8Reason, msgId);
}

int32_t GroupServiceStrategy::SendFinishCallMessage(ConversationType type, UCID groupId, const std::wstring& confId, 
	int32_t hostId, const std::wstring& reason, OUT int32_t& msgId) {

	std::string utf8ConfId = StringConvert::ToUTF8Str(confId);
	std::string utf8Reason = StringConvert::ToUTF8Str(reason);
	return _groupService->SendRejectCallMessage(type, groupId, utf8ConfId, hostId, utf8Reason, msgId);
}


int32_t GroupServiceStrategy::SendCancelCallMessage(ConversationType type, UCID groupUid, const string& phoneNum, OUT int32_t& msgId) {
	return _groupService->SendCancelCallMessage(type, groupUid, phoneNum, msgId);
}

/*
int32_t GroupServiceStrategy::SendNotRespondCallRecordMessage(ConversationType type, UCID groupId, OUT int32_t& msgId) {
	return _groupService->SendNotRespondCallRecordMessage(type, groupId, msgId);	
}

int32_t GroupServiceStrategy::SendRejectCallRecordMessage(ConversationType type, UCID groupId, OUT int32_t& msgId) {
	return _groupService->SendRejectCallRecordMessage(type, groupId, msgId);	
}
*/

int32_t GroupServiceStrategy::SendConnectedCallRecordMessage(ConversationType type, UCID groupId, int32_t druation, const std::string& phoneNum, OUT int32_t& msgId) {
	return _groupService->SendConnectedCallRecordMessage(type, groupId, druation, phoneNum, msgId);
}

int32_t GroupServiceStrategy::SendMessageRead(ConversationType type, UCID groupId, int32_t msgId, int64_t msgSeq) {
	return _groupService->SendMessageRead(type, groupId, msgId, msgSeq);	
}

int32_t GroupServiceStrategy::SendMessagesReadById(ConversationType type, int64_t groupId){
	return _groupService->SendMessagesRead(type, groupId);
}

int32_t GroupServiceStrategy::SendMessagePlayed(ConversationType type, UCID groupId, int32_t msgId, int64_t msgSeq) {
	return _groupService->SendMessagePlayed(type, groupId, msgId, msgSeq);	
}

int32_t GroupServiceStrategy::GetUnreadUserList(ConversationType type, int64_t groupId, std::list<int64_t> seq){
	return _groupService->GetUnreadUserList(type, groupId, seq);
}

int32_t GroupServiceStrategy::GetGroupList(GroupStyle groupType, int32_t page, int32_t count, int32_t lastGroupTime, bool allData) {
	return _groupService->GetGroupList(groupType, page, count, lastGroupTime, allData);	
}

int32_t GroupServiceStrategy::GetGroupInfo(int64_t groupId, GroupInfo &group, bool forced, int32_t mountId) {
	return _groupService->GetGroupInfo(groupId, group, forced, mountId);
}

int32_t GroupServiceStrategy::CreateGroup(GroupStyle groupType, const std::wstring& groupName, const std::wstring& logourl,
	std::vector<UserInfo>& userInfo, int32_t nameFlag, int32_t memberType, int32_t mountId) {
	std::string utf8GroupName = StringConvert::ToUTF8Str(groupName);
	std::string utf8Logourl = StringConvert::ToUTF8Str(logourl);
	return _groupService->CreateGroup(groupType, utf8GroupName, utf8Logourl, userInfo, nameFlag, memberType, mountId);
}

int32_t GroupServiceStrategy::EndGroup(int64_t groupId) {
	return _groupService->EndGroup(groupId);
}

int32_t GroupServiceStrategy::ExitGroup(int64_t groupId) {
	return _groupService->ExitGroup(groupId);
}

int32_t GroupServiceStrategy::AddMember(int64_t groupId, const std::vector<UserInfo>& userInfo) {
	return _groupService->AddMember(groupId, userInfo);
}

int32_t GroupServiceStrategy::SetAdmin(int64_t groupId, SetAdminType type, const std::vector<int32_t>& adminList) {
	return _groupService->SetAdmin(groupId, type, adminList);
}

int32_t GroupServiceStrategy::RemoveMember(int64_t groupId, const std::vector<int32_t>& members) {
	return _groupService->RemoveMember(groupId, members);
}

int32_t GroupServiceStrategy::SetGroupName(int64_t groupId, const std::wstring& groupName) {
	std::string utf8GroupName = StringConvert::ToUTF8Str(groupName);
	return _groupService->SetGroupName(groupId, utf8GroupName);
}

int32_t GroupServiceStrategy::GetDocumentList(int64_t groupId, int32_t page, int32_t count) {
	return _groupService->GetDocumentList(groupId, page, count);
}

int32_t GroupServiceStrategy::UploadCodeSnippet(int64_t groupId, int32_t operation, const std::wstring& codeTitle, const std::wstring& langType, const std::wstring& codeHead, 
	const std::wstring& codeDesc, int32_t codeSize, const std::wstring& codeUrl, int32_t codeLine, OUT int32_t& msgId)
{
	std::string utf8FileName = StringConvert::ToUTF8Str(codeTitle);
	std::string utf8Format = StringConvert::ToUTF8Str(langType);
	std::string utf8CodeHead = StringConvert::ToUTF8Str(codeHead);
	std::string utf8Desc = StringConvert::ToUTF8Str(codeDesc);
	std::string utf8FileURL = StringConvert::ToUTF8Str(codeUrl);

	DocumentInfo info;
	info.SetFileName( utf8FileName );
	info.SetFileFormat( utf8Format );
	info.SetThumbnail( utf8CodeHead );
	info.SetDescription( utf8Desc );
	info.SetFileSize( codeSize );
	info.SetTotalLines( codeLine );
	info.SetFileUrl( utf8FileURL );

	return _groupService->UploadCodeSnippet(groupId, operation, info, msgId);
}

int32_t GroupServiceStrategy::UploadCloudDocument(int64_t groupId, int32_t operation, int32_t isSendMessage, int32_t mountId,
	const std::wstring& fileName, int64_t fileSize, DocumentCloudType cloudType, const std::wstring& hashCode,
	const std::wstring& detailContent, OUT int32_t& msgId) {
	std::string utf8FileName = StringConvert::ToUTF8Str(fileName);
	std::string utf8DetailContent = StringConvert::ToUTF8Str(detailContent);

	DocumentInfo info;
	info.SetFileName(utf8FileName);
	info.SetFileSize(fileSize);
	info.SetCloudType(DocumentCloudType(cloudType));
	info.SetDetailContent(utf8DetailContent);
	info.SetMountId(mountId);
	info.SetDetailType(1);
	info.SetHashCode(StringConvert::ToUTF8Str(hashCode));

	return _groupService->UploadDocument(groupId, operation, isSendMessage, info, msgId);
}

int32_t GroupServiceStrategy::BatchUploadCloudDocument(int64_t groupId, int32_t operation, int32_t isSendMessage, int32_t mountId, const std::wstring& fileName,
	int64_t fileSize, DocumentCloudType cloudType, const std::wstring& hashCode, const std::wstring& detailContent) {
	std::string utf8FileName = StringConvert::ToUTF8Str(fileName);
	std::string utf8DetailContent = StringConvert::ToUTF8Str(detailContent);

	DocumentInfo info;
	info.SetFileName(utf8FileName);
	info.SetFileSize(fileSize);
	info.SetCloudType(DocumentCloudType(cloudType));
	info.SetDetailContent(utf8DetailContent);
	info.SetMountId(mountId);
	info.SetDetailType(1);
	info.SetHashCode(StringConvert::ToUTF8Str(hashCode));

	return _groupService->BatchUploadDocument(groupId, operation, info);
}

int32_t GroupServiceStrategy::UploadDocument(int64_t groupId, int32_t operation, const std::wstring& fileName, int64_t fileSize,
	const std::wstring& fileURL, OUT int32_t& msgId) {

	std::string utf8FileName = StringConvert::ToUTF8Str(fileName);
	std::string utf8FileURL = StringConvert::ToUTF8Str(fileURL);

	DocumentInfo info;
	info.SetFileName(utf8FileName);
	info.SetFileSize(fileSize);
	info.SetFileUrl(utf8FileURL);

	return _groupService->UploadDocument(groupId, operation, 1, info, msgId);
}

int32_t GroupServiceStrategy::DeleteDocument(int64_t groupId, int64_t fileId) {
	return _groupService->DeleteDocument(groupId, fileId);
}

int32_t GroupServiceStrategy::GetDocumentInfo(int64_t groupId, int64_t fileId, const std::wstring &cloudFileInfo) {
	return _groupService->GetDocumentInfo(groupId, fileId, StringConvert::ToUTF8Str(cloudFileInfo));
}

int32_t GroupServiceStrategy::GetCommentList(int64_t groupId, int64_t fileId, int32_t count, int64_t startTime, int64_t endTime){

	return _groupService->GetCommentList(groupId, fileId, count, startTime, endTime);
}

int32_t GroupServiceStrategy::CreateComment(int64_t groupId, int64_t fileId, int32_t fileType, const std::wstring& fileTitle, 
	const std::wstring content, int32_t &msgId){

	std::string utf8FileTitle = StringConvert::ToUTF8Str(fileTitle);
	std::string utf8Content = StringConvert::ToUTF8Str(content);

	return _groupService->CreateComment(groupId, fileId, fileType, utf8FileTitle, utf8Content, msgId);
}

int32_t GroupServiceStrategy::GetAtMessageList(int64_t groupId, int32_t page, int32_t count) {
	return _groupService->GetAtMessageList(groupId, page, count);
}

int32_t GroupServiceStrategy::DeleteAtMessage(ConversationType type, int64_t groupId, std::vector<int64_t> seqList) {
	return _groupService->DeleteAtMessage(type, groupId, seqList);
}

int32_t GroupServiceStrategy::SetGroupLogo(int64_t groupId, const std::wstring& logoURL){
	std::string utf8LogoURL = StringConvert::ToUTF8Str(logoURL);

	return _groupService->SetGroupLogo(groupId, utf8LogoURL);
}

int32_t GroupServiceStrategy::UpdateGroupLogo(int64_t groupId, const std::wstring& localPath){
	std::string utf8lLocalPath = StringConvert::ToUTF8Str(localPath);
	return _groupService->UpdateGroupLogo(groupId, utf8lLocalPath);
}

int32_t GroupServiceStrategy::SetTopChat(int64_t groupId, int32_t state) {
	return _groupService->SetTopChat(groupId, state);
}

int32_t GroupServiceStrategy::SetDoNotDisturb(int64_t groupId, int32_t state) {
	return _groupService->SetDoNotDisturb(groupId, state);
}

int32_t GroupServiceStrategy::SetReminder(int64_t groupId, int32_t state, int32_t alertRule, int32_t alertTime) {
	return _groupService->SetReminder(groupId, state, alertRule, alertTime);
}

void GroupServiceStrategy::OnChatHistoryReceived(int32_t result, MessageChannel channel, ConversationType type, int64_t groupId, int32_t msgCount, int64_t min_timestamp) {
	std::wstringstream jsScript;

	jsScript << L"var e = new CustomEvent(\"" << GROUP_SERVICE_CHAT_HISTORY_RECEIVED_EVENT
		<< "\",{detail:" << "{errorCode: " << result << ", groupId:"
		<< groupId << ", channel: " << channel << ", conversationType: " << type << ", msgCount:" << msgCount << ", minTimestamp: " << min_timestamp;

	jsScript << "},});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnChatHistoryReceived event to JS";
}

void GroupServiceStrategy::OnSearchChatHistoryReceived(int32_t result, ConversationType type, int64_t relateId, int32_t msgCount, int64_t min_timestamp, int64_t max_timestamp){

	std::wstringstream jsScript;

	jsScript << L"var e = new CustomEvent(\"" << GROUP_SERVICE_SEARCH_CHAT_HISTORY_RECEIVED_EVENT
		<< "\",{detail:" << "{errorCode: " << result << ", relateId:"
		<< relateId << ", conversationType: " << type << ", msgCount:" << msgCount << ", minTimestamp: " << min_timestamp
		<< ", maxTimestamp: " << max_timestamp;

	jsScript << "},});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnChatHistoryReceived event to JS";
}

void GroupServiceStrategy::OnMessageSent(int32_t result, ConversationType type, int64_t groupId, int32_t msgId, int64_t msgSeq, int64_t msgTime) {
	std::wstringstream jsScript;

	jsScript << L"var e = new CustomEvent(\"" << GROUP_SERVICE_MESSAGE_SENT_EVENT
		<< "\",{detail:" << "{errorCode: " << result << ",groupId:"
		<< groupId << ", msgId:" << msgId << ", msgSeq:" << msgSeq << ", msgTime:" << msgTime
		<< ", conversationType:" << type
		<< "},});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnMessageSent event to JS";
}

void GroupServiceStrategy::OnTextMessageReceived(MessageChannel channel, const TextMessageInfo *message) {
	if (message == NULL) {
		UC_LOG(ERROR) << "fail to dispatch OnTextMessageReceived event to JS, message is NULL";
		return;
	}

	std::wstringstream jsScript;

	jsScript << L"var e = new CustomEvent(\"" << GROUP_SERVICE_TEXT_MESSAGE_RECEIVED_EVENT
		<< "\", {detail:" 
		<< "{channel: " << channel << ", ";

	jsScript << ServiceObjectConversion::AppendTextMessageInfoToJSON(const_cast<TextMessageInfo&>(*message));
	
	jsScript << "}" << "});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnTextMessageReceived event to JS";
}

void GroupServiceStrategy::OnImageMessageReceived(MessageChannel channel, const MediaMessageInfo *message) {
	if (message == NULL) {
		UC_LOG(ERROR) << "fail to dispatch OnImageMessageReceived event to JS, message is NULL";
		return;
	}

	std::wstringstream jsScript;

	jsScript <<L"var e = new CustomEvent(\"" << GROUP_SERVICE_IMAGE_RECEIVED_EVENT
			 << "\", {detail:" << "{channel: " << channel << ", ";
	jsScript << ServiceObjectConversion::AppendMediaMessageInfoToJSON(const_cast<MediaMessageInfo&>(*message));
	jsScript << "}" << "});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnImageReceived event to JS";
}

void GroupServiceStrategy::OnAudioMessageReceived(MessageChannel channel, const MediaMessageInfo *message) {
	if (message == NULL) {
		UC_LOG(ERROR) << "fail to dispatch OnAudioMessageReceived event to JS, message is NULL";
		return;
	}

	std::wstringstream jsScript;

	jsScript<<L"var e = new CustomEvent(\"" << GROUP_SERVICE_AUDIO_MESSAGE_RECEIVED_EVENT
		<< "\",{detail:" 
		<< "{channel: " << channel << ", ";

	jsScript << ServiceObjectConversion::AppendMediaMessageInfoToJSON(const_cast<MediaMessageInfo&>(*message));
	jsScript << "}" << "});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnAudioMessageReceived event to JS";
}

void GroupServiceStrategy::OnVideoMessageReceived(MessageChannel channel, const MediaMessageInfo *message) {
	
	if (message == NULL) {
		UC_LOG(ERROR) << "fail to dispatch OnVideoMessageReceived event to JS, message is NULL";
		return;
	}

	std::wstringstream jsScript;

	jsScript <<L"var e = new CustomEvent(\"" << GROUP_SERVICE_VIDEO_MESSAGE_RECEIVED_EVENT
			 << "\", {detail:" << "{channel: " << channel << ", ";
	jsScript << ServiceObjectConversion::AppendMediaMessageInfoToJSON(const_cast<MediaMessageInfo&>(*message));
	jsScript << "}" << "});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnVideoMessageReceived event to JS";
}

void GroupServiceStrategy::OnReportMessageReceived(MessageChannel channel, const ReportMessageInfo *message){
	
	if (message == NULL) {
		UC_LOG(ERROR) << "fail to dispatch OnReportMessageReceived event to JS, message is NULL";
		return;
	}

	std::wstringstream jsScript;

	jsScript << L"var e = new CustomEvent(\"" << GROUP_SERVICE_REPORT_MESSAGE_RECEIVED_EVENT
		<< "\", {detail:" << "{channel: " << channel << ", ";
	jsScript << ServiceObjectConversion::AppendReportMessageInfoToJSON(const_cast<ReportMessageInfo&>(*message));
	jsScript << "}" << "});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnReportMessageReceived event to JS";
}

void GroupServiceStrategy::OnCloudFileModifiedMessageReceived(MessageChannel channel, CloudFileOperation operation, const FileMessageInfo &message) {
	std::wstringstream jsScript;

	jsScript << L"var e = new CustomEvent(\"" << GROUP_SERVICE_CLOUDFILE_MODIFIED_RECEIVED_EVENT
		<< L"\", {detail:" << L"{channel: " << channel << L", operation: " << operation << L", ";

	jsScript << ServiceObjectConversion::AppendFileMessageInfoToJSON(message);
	jsScript << L"}" << L"});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnCloudFileModifiedMessageReceived event to JS";
}

void GroupServiceStrategy::OnRecordVideoReceived(MessageChannel channel, const MediaMessageInfo *message) {
	std::wstringstream jsScript;
	jsScript << L"var e = new CustomEvent(\"" << GROUP_SERVICE_RECORD_VIDEO_MESSAGE_RECEIVED_EVENT
		<< "\",{detail:" << "{channel: " << channel << ", ";
	jsScript << ServiceObjectConversion::AppendMediaMessageInfoToJSON(const_cast<MediaMessageInfo&>(*message));

	jsScript << "}" << "});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnRecordVideoReceived event to JS";
}

void GroupServiceStrategy::OnGroupCreateMessageReceived(MessageChannel channel, const MessageInfo &message, const GroupInfo& group) {
	std::wstringstream jsScript;

	jsScript << L"var e = new CustomEvent(\"" << GROUP_SERVICE_GROUP_CREATE_MESSAGE_RECEIVED_EVENT
			 << "\", {detail:" << "{channel: " << channel << ", ";
	jsScript << ServiceObjectConversion::AppendMessageInfoToJSON(message);
	jsScript << ServiceObjectConversion::AppendGroupInfoToJSON(group);
	jsScript << "}" << "});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnGroupCreateMessageReceived event to JS";
}

void GroupServiceStrategy::OnGroupCloseMessageReceived(MessageChannel channel, const MessageInfo &message, int32_t operatorId) {
	std::wstringstream jsScript;

	jsScript << L"var e = new CustomEvent(\"" << GROUP_SERVICE_GROUP_CLOSE_MESSAGE_RECEIVED_EVENT
		<< "\", {detail:" << "{channel: " << channel << ", ";
	jsScript << ServiceObjectConversion::AppendMessageInfoToJSON(message);
	jsScript << "operatorId:" << operatorId << ", }});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnGroupCloseMessageReceived event to JS";
}

void GroupServiceStrategy::OnGroupMemberDelMessageReceived(MessageChannel channel, const MessageInfo &message, int32_t operatorId, int32_t groupId,
			int32_t memberId, const std::string &avatar, const std::string &groupName, const std::string &namePinYin, int8_t nameFlag) {
				
	std::wstringstream jsScript;

	jsScript <<L"var e = new CustomEvent(\"" << GROUP_SERVICE_GROUPMEMBER_DEL_MESSAGE_RECEIVED_EVENT
			 << "\", {detail:" << "{channel: " << channel << ", ";
	jsScript << ServiceObjectConversion::AppendMessageInfoToJSON(message);
	jsScript << "operatorId:" << operatorId << ", " << "groupId:" << groupId << ", "
		<< "memberId:" << memberId << ", " << "avatar:\"" << UTF2WJSON(avatar) << "\", "
			 << "groupName:\"" << UTF2WJSON(groupName) << "\", "
			 << "NamePinYin:\"" << UTF2WJSON(namePinYin) << "\", "
			 << "nameFlag:" << nameFlag << ", ";
		
	jsScript << "}});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnGroupMemberDelMessageReceived event to JS";
}

void GroupServiceStrategy::OnGroupMemberAddMessageReceived(MessageChannel channel, const MessageInfo &message, int32_t operatorId, int32_t groupId,
	const std::vector<UserInfo> &memberList, const std::string &avatar, const std::string &groupName,
			const std::string &namePinYin, int8_t nameFlag) {

	std::wstringstream jsScript;

	jsScript <<L"var e = new CustomEvent(\"" << GROUP_SERVICE_GROUPMEMBER_ADD_MESSAGE_RECEIVED_EVENT
			 << "\", {detail:" << "{channel: " << channel << ", ";
	jsScript << ServiceObjectConversion::AppendMessageInfoToJSON(message);
	jsScript << "operatorId:" << operatorId << ", " << "groupId:" << groupId << ", ";

	jsScript << "members:[";
	for (int32_t index = 0; index < memberList.size(); index ++ ) {
		jsScript << "{userId:" << memberList.at(index).ucid.GetUserId()
			<< ", role: " << memberList.at(index).role << "},";
	}
	
	jsScript << "],";
	jsScript << "avatar:\"" << UTF2WJSON(avatar) << "\", "
		<< "groupName:\"" << UTF2WJSON(groupName) << "\", "
		<< "NamePinYin:\"" << UTF2WJSON(namePinYin) << "\", "
		<< "nameFlag:" << nameFlag << ", ";
		
		
	jsScript << "}});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnGroupMemberAddMessageReceived event to JS";

}

void GroupServiceStrategy::OnGroupMemberKickMessageReceived(MessageChannel channel, const MessageInfo &message, int32_t operatorId, int32_t groupId,
			const std::vector<int32_t> &memberList, const std::string &avatar, const std::string &groupName, const std::string &namePinYin,
			int8_t nameFlag) {

	std::wstringstream jsScript;

	jsScript <<L"var e = new CustomEvent(\"" << GROUP_SERVICE_GROUPMEMBER_KICK_MESSAGE_RECEIVED_EVENT
			 << "\", {detail:" << "{channel: " << channel << ", ";
	jsScript << ServiceObjectConversion::AppendMessageInfoToJSON(message);
	jsScript << "operatorId:" << operatorId << ", " << "groupId:" << groupId << ", ";

	jsScript << "members:[";
	for (int32_t index = 0; index < memberList.size(); index ++ ) {
		jsScript << "{userId:" << memberList.at(index) << "},";
	}
	
	jsScript << "],";
	jsScript << "avatar:\"" << UTF2WJSON(avatar) << "\", "
		<< "groupName:\"" << UTF2WJSON(groupName) << "\", "
		<< "NamePinYin:\"" << UTF2WJSON(namePinYin) << "\", "
			 << "nameFlag:" << nameFlag << ", ";
		
		
	jsScript << "}});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnGroupMemberKickMessageReceived event to JS";
}

void GroupServiceStrategy::OnGroupNameUpdateMessageReceived(MessageChannel channel, const MessageInfo &message, int32_t operatorId, int32_t groupId,
			const std::string &groupName, const std::string &namePinYin, int8_t nameFlag) {

	std::wstringstream jsScript;

	jsScript <<L"var e = new CustomEvent(\"" << GROUP_SERVICE_GROUPNAME_UPDATE_MESSAGE_RECEIVED_EVENT
			 << "\", {detail:" << "{channel: " << channel << ", ";

	jsScript << ServiceObjectConversion::AppendMessageInfoToJSON(message);
	jsScript << "operatorId:" << operatorId << ", " << "groupId:" << groupId << ", ";
	jsScript << "groupName:\"" << UTF2WJSON(groupName) << "\", "
		<< "NamePinYin:\"" << UTF2WJSON(namePinYin) << "\", "
			 << "nameFlag:" << nameFlag << ", ";
		
	jsScript << "}});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnGroupNameUpdateMessageReceived event to JS";

}

void GroupServiceStrategy::OnGroupDisturbSetMessageReceived(MessageChannel channel, const MessageInfo &message, int32_t groupId, int8_t state) {
	std::wstringstream jsScript;

	jsScript <<L"var e = new CustomEvent(\"" << GROUP_SERVICE_GROUPDISTURB_SET_MESSAGE_RECEIVED_EVENT
			 << "\", {detail:" << "{channel: " << channel << ", ";

	jsScript << ServiceObjectConversion::AppendMessageInfoToJSON(message);
	jsScript << "groupId:" << groupId << ", " << "state:" << state << ", ";
	jsScript << "}});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnGroupDisturbSetMessageReceived event to JS";
}

void GroupServiceStrategy::OnGroupTopMessageReceived(MessageChannel channel, const MessageInfo &message, int32_t operatorId, int32_t groupId, int8_t isTop) {
	std::wstringstream jsScript;

	jsScript << L"var e = new CustomEvent(\"" << GROUP_SERVICE_GROUPTOP_MESSAGE_RECEIVED_EVENT
			 << "\", {detail:" << "{channel: " << channel << ", ";

	jsScript << ServiceObjectConversion::AppendMessageInfoToJSON(message);
	jsScript << "operatorId: " << operatorId << ", " << "groupId:" << groupId << ", " << "isTop:" << isTop << ", ";
	jsScript << "}});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnGroupTopMessageReceived event to JS";
}

void GroupServiceStrategy::OnGroupAvatarUpdateMessageReceived(MessageChannel channel, const MessageInfo &message, int32_t operatorId,
			const std::string &avatar, int32_t groupId) {

	std::wstringstream jsScript;

	jsScript << L"var e = new CustomEvent(\"" << GROUP_SERVICE_GROUPAVATAR_UPDATE_MESSAGE_RECEIVED
			 << "\", {detail:" << "{channel: " << channel << ", ";

	jsScript << ServiceObjectConversion::AppendMessageInfoToJSON(message);
	jsScript << "operatorId: " << operatorId << ", " << "groupId:" << groupId << ", " 
		<< "avatar:\"" << UTF2WJSON(avatar) << "\", ";
	jsScript << "}});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnGroupAvatarUpdateMessageReceived event to JS";
}


void GroupServiceStrategy::OnGroupStatusInfoUpdateMessageReceived(MessageChannel channel, const MessageInfo &message, int32_t groupId,
	int32_t status, const std::string &statusInfo){
	
	std::wstringstream jsScript;
	jsScript << L"var e = new CustomEvent(\"" << GROUP_SERVICE_GROUPSTATUSINFO_UPDATE_MESSAGE_RECEIVED
		<< "\", {detail:" << "{channel: " << channel << ", ";

	jsScript << ServiceObjectConversion::AppendMessageInfoToJSON(message);
	jsScript << "groupId: " << groupId << ", " << "status:" << status << ", "
		<< "statusInfo:\"" << UTF2WJSON(statusInfo) << "\", ";
	jsScript << "}});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnGroupStatusInfoUpdateMessageReceived event to JS";
}

void GroupServiceStrategy::OnGroupReminderSetMessageReceived(MessageChannel channel, const MessageInfo &message, int32_t groupId,
	int8_t state, int32_t rule, int32_t time) {

	std::wstringstream jsScript;

	jsScript << L"var e = new CustomEvent(\"" << GROUP_SERVICE_GROUPREMINDER_SET_MESSAGE_RECEIVED
			 << "\", {detail:" << "{channel: " << channel << ", ";

	jsScript << ServiceObjectConversion::AppendMessageInfoToJSON(message);
	jsScript << "state: " << state << ", " << "groupId:" << groupId << ", " 
			 << "rule:" << rule << ", " << "time:" << time << ",";
	jsScript << "}});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnGroupReminderSetMessageReceived event to JS";

}

void GroupServiceStrategy::OnCodeSnippetUploadMessageReceived(MessageChannel channel, int32_t operation, const MessageInfo &message, const DocumentInfo &doc)
{
	std::wstringstream jsScript;
	jsScript << L"var e = new CustomEvent(\"" << GROUP_SERVICE_CODESNIPPET_UPLOAD_MESSAGE_RECEIVED
			 << "\", {detail:" << "{channel: " << channel << ", operation: " << operation << ", ";

	jsScript << ServiceObjectConversion::AppendMessageInfoToJSON(message);
	jsScript << ServiceObjectConversion::AppendDocumentInfoToJSON(doc);
	jsScript << "}});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnCodeSnippetUploadMessageReceived event to JS";
}

void GroupServiceStrategy::OnDocumentUploadMessageReceived(MessageChannel channel, int32_t operation, const MessageInfo &message, const DocumentInfo &doc) {
	std::wstringstream jsScript;

	jsScript << L"var e = new CustomEvent(\"" << GROUP_SERVICE_DOCUMENTUPLOAD_MESSAGE_RECEIVED
			 << "\", {detail:" << "{channel: " << channel << ", operation: " << operation << ", ";

	jsScript << ServiceObjectConversion::AppendMessageInfoToJSON(message);
	jsScript << ServiceObjectConversion::AppendDocumentInfoToJSON(doc);

	jsScript << "}});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnDocumentUploadMessageReceived event to JS";
}

void GroupServiceStrategy::OnCloudFileCreateMessageReceived(MessageChannel channel, const FileMessageInfo &message) {
	std::wstringstream jsScript;

	jsScript << L"var e = new CustomEvent(\"" << GROUP_SERVICE_CLOUDFILE_CREATE_MESSAGE_RECEIVED
		<< "\", {detail:" << "{channel: " << channel << ", ";

	jsScript << ServiceObjectConversion::AppendFileMessageInfoToJSON(message);
	jsScript << "}" << "});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnFileMessageReceived event to JS";
}

void GroupServiceStrategy::OnDocumentDeleteMessageReceived(MessageChannel channel, const MessageInfo &message, int32_t operatorId,
	int64_t contentId, const std::string &docName) {
		
	std::wstringstream jsScript;
	
	jsScript << L"var e = new CustomEvent(\"" << GROUP_SERVICE_DOCUMENTDELETE_MESSAGE_RECEIVED
			 << "\", {detail:" << "{channel: " << channel << ", ";
	jsScript << ServiceObjectConversion::AppendMessageInfoToJSON(message);
	jsScript << "operatorId: " << operatorId << ", " << "contentId:" << contentId << ", " 
		<< "docName:\"" << UTF2WJSON(docName) << "\", ";
	jsScript << "}});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnDocumentDeleteMessageReceived event to JS";

}

void GroupServiceStrategy::OnCommentCreateMessageReceived(MessageChannel channel, const MessageInfo &message, const CommentInfo& commentInfo){

	std::wstringstream jsScript;

	jsScript << L"var e = new CustomEvent(\"" << GROUP_SERVICE_COMMENTCREATE_MESSAGE_RECEIVED
			 << "\", {detail:" << "{channel: " << channel << ", ";
	jsScript << ServiceObjectConversion::AppendMessageInfoToJSON(message);
	jsScript << ServiceObjectConversion::AppendCommentInfoToJSON(commentInfo);
	jsScript << "}});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnCommentCreateMessageReceived event to JS";

}

void GroupServiceStrategy::OnMessageRead(int32_t result, ConversationType type, int64_t groupId, int32_t readType, int32_t msgId, int64_t msgSeq) {
	std::wstringstream jsScript;
	jsScript<<L"var e = new CustomEvent(\"" << GROUP_SERVICE_MESSAGE_READ_EVENT
		<< "\",{detail:" << "{errorCode:" << result << ", conversationType:" << type << ", groupId:" << groupId
		<< ", readType:" << readType << ", msgId:" << msgId << ", msgSeq:"<< msgSeq
		<< "},});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnMessageRead event to JS";
}

void GroupServiceStrategy::OnMessagePlayed(int32_t result, int64_t groupId, 
	int32_t msgId, int64_t msgSeq) {
	std::wstringstream jsScript;
	jsScript<<L"var e = new CustomEvent(\"" << GROUP_SERVICE_MESSAGE_PLAYED_EVENT
		<< "\",{detail:" << "{errorCode:" << result << ",groupId:" << groupId 
		<< ", msgId:" << msgId << ", msgSeq:"<< msgSeq
		<< "},});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnMessagePlayed event to JS";
}

void GroupServiceStrategy::OnMessageUnreadUserCount(int32_t result, ConversationType type, 
	int64_t groupId, const std::list<MessageReadInfo>& info){

	std::wstringstream jsScript;
	jsScript << L"var e = new CustomEvent(\"" << GROUP_SERVICE_MESSAGE_UNREAD_USER_COUNT_EVENT
		<< "\",{detail:" << "{errorCode:" << result << ", conversationType:" << type << ", groupId:" << groupId
		<< ", info:[";

	for (std::list<MessageReadInfo>::const_iterator iter = info.begin(); iter != info.end(); ++iter) {
		jsScript << ServiceObjectConversion::ConvertMessageReadInfoToJSON(*iter) << ",";
	}

	jsScript << "]},});document.dispatchEvent(e);";


	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnMessageUnreadUserCount event to JS";
}

void GroupServiceStrategy::OnMessageUnreadUserList(int32_t result, ConversationType type, 
	int64_t groupId, const std::list<MessageReadUserInfo>& info){

	std::wstringstream jsScript;

	jsScript << L"var e = new CustomEvent(\"" << GROUP_SERVICE_MESSAGE_UNREAD_USER_LIST_EVENT
		<< "\",{detail:" << "{errorCode:" << result << ", conversationType:" << type << ", groupId:" << groupId
		<< ", info:[";

	for (std::list<MessageReadUserInfo>::const_iterator iter = info.begin(); iter != info.end(); ++iter) {
		jsScript << ServiceObjectConversion::ConvertMessageReadUserInfoToJSON(*iter) << ",";
	}

	jsScript << "]},});document.dispatchEvent(e);";


	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnMessageUnreadUserList event to JS";
}

void GroupServiceStrategy::OnCallInvited(MessageChannel channel, int32_t result, ConversationType type,
	int32_t userId, int64_t groupId, int32_t msgId, int64_t msgSeq, int64_t msgTime, const std::string confId,
	const std::string confPwd, int32_t hostId) {
	std::wstringstream jsScript;
	jsScript<<L"var e = new CustomEvent(\"" << GROUP_SERVICE_CALL_INVITED_EVENT
		<< "\",{detail:" << "{errorCode:" << result 
		<< ", channel: " << channel 
		<< ", userId:" << userId
		<< ", groupId:" << groupId 
		<< ", msgId:" << msgId << ", msgSeq:" << msgSeq << ", msgTime:" << msgTime
		<< ", conversationType:" << type << ", hostId:"<< hostId
		<< ", confId:\"" << UTF2WJSON(confId) << "\", confPwd:\"" << UTF2WJSON(confPwd)
		<< "\"},});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnCallInvited event to JS";
}

void GroupServiceStrategy::OnCallAccepted(MessageChannel channel, int32_t result, ConversationType type,
	int32_t userId, int64_t groupId, const std::string confId, int32_t msgId, int64_t msgSeq, int64_t msgTime) {
	std::wstringstream jsScript;
	jsScript<<L"var e = new CustomEvent(\"" << GROUP_SERVICE_CALL_ACCEPTED_EVENT
		<< "\",{detail:" << "{errorCode:" << result 
		<< ", channel: " << channel 
		<< ", userId:" << userId
		<< ", groupId:" << groupId
		<< ", msgId:" << msgId << ", msgSeq:" << msgSeq << ", msgTime:" << msgTime
		<< ", conversationType:" << type << ", confId:\"" << UTF2WJSON(confId)
		<< "\"},});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnCallInvited event to JS";
}

void GroupServiceStrategy::OnCallRejected(MessageChannel channel, int32_t result, ConversationType type,
	int32_t userId, int64_t groupId, const std::string confId, int32_t hostId, std::string reason,
	int32_t msgId, int64_t msgSeq, int64_t msgTime) {
	std::wstringstream jsScript;
	jsScript<<L"var e = new CustomEvent(\"" << GROUP_SERVICE_CALL_REJECTED_EVENT
		<< "\",{detail:" << "{errorCode:" << result 
		<< ", channel: " << channel 
		<< ", userId:" << userId
		<< ", groupId:" << groupId
		<< ", conversationType:" << type << ", confId:\"" << UTF2WJSON(confId) << "\""
		<< ", hostId:" << hostId
		<< ", reason:\"" << UTF2WJSON(reason) 
		<< "\", msgId:" << msgId << ", msgSeq:" << msgSeq << ", msgTime:" << msgTime
		<< "},});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnCallRejected event to JS";
}

void GroupServiceStrategy::OnCallStopped(MessageChannel channel, int32_t result, ConversationType type,
	int32_t userId, int64_t groupId, const std::string confId,
	int32_t msgId, int64_t msgSeq, int64_t msgTime) {
	std::wstringstream jsScript;
	jsScript<<L"var e = new CustomEvent(\"" << GROUP_SERVICE_CALL_STOPPED_EVENT
		<< "\",{detail:" << "{errorCode:" << result 
		<< ", channel: " << channel 
		<< ", userId:" << userId
		<< ", groupId:" << groupId
		<< ", conversationType:" << type << ", confId:\"" << UTF2WJSON(confId)
		<< "\", msgId:" << msgId << ", msgSeq:" << msgSeq << ", msgTime:" << msgTime
		<< "},});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnCallStopped event to JS";
}

void GroupServiceStrategy::OnCallConnected(MessageChannel channel, int32_t result, ConversationType type,
	int32_t userId, int64_t groupId, int32_t duration, const std::string& phoneNum,
	int32_t msgId, int64_t msgSeq, int64_t msgTime) {
	std::wstringstream jsScript;
	jsScript<<L"var e = new CustomEvent(\"" << GROUP_SERVICE_CALL_CONNECTED_EVENT
		<< "\",{detail:" << "{errorCode:" << result 
		<< ", channel: " << channel 
		<< ", userId:" << userId
		<< ", groupId:" << groupId
		<< ", conversationType:" << type << ", duration:" << duration
		<< ", phoneNum: \"" << UTF2WJSON(phoneNum)
		<< "\", msgId:" << msgId << ", msgSeq:" << msgSeq << ", msgTime:" << msgTime
		<< "},});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnCallConnected event to JS";
}

void GroupServiceStrategy::OnCallCanceled(MessageChannel channel, int32_t result, ConversationType type, 
	int32_t userId, int64_t groupId, int32_t msgId, int64_t msgSeq, int64_t msgTime, const std::string& phoneNum) {
	std::wstringstream jsScript;
	jsScript << L"var e = new CustomEvent(\"" << GROUP_SERVICE_CALL_CANCELLED_EVENT
		<< "\",{detail:" << "{errorCode:" << result
		<< ", channel: " << channel
		<< ", userId:" << userId
		<< ", conversationType:" << type << ", groupId:" << groupId
		<< ", msgId:" << msgId << ", msgSeq:" << msgSeq << ", msgTime:" << msgTime
		<< ", phoneNum: \"" << UTF2WJSON(phoneNum)
		<< "\"},});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnCallCanceled event to JS";
}

void GroupServiceStrategy::OnGroupCreated(int32_t result, int32_t operatorId, const GroupInfo& info) {
	std::wstringstream jsScript;

	jsScript << L"var e = new CustomEvent(\"" << GROUP_SERVICE_GROUP_CREATED_EVENT
		<< "\",{detail:" << "{errorCode:" << result << ",operatorId:" << operatorId;

	jsScript << ", group:" << ServiceObjectConversion::ConvertGroupInfoToJSON(info);
	jsScript << "},});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnGroupCreated event to JS";
}

void GroupServiceStrategy::OnGroupExited(int32_t result, int64_t groupId, 
	int32_t userId) {
	std::wstringstream jsScript;
	jsScript<<L"var e = new CustomEvent(\"" << GROUP_SERVICE_GROUP_EXITED_EVENT
		<< "\",{detail:" << "{errorCode:" << result << ",groupId:" << groupId 
		<< ", userId:" << userId << "},});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnGroupExited event to JS";
}

void GroupServiceStrategy::OnGroupEnded(int32_t result, int64_t groupId, 
	int32_t operatorId) {
	std::wstringstream jsScript;
	jsScript<<L"var e = new CustomEvent(\"" << GROUP_SERVICE_GROUP_ENDEDED_EVENT
		<< "\",{detail:" << "{errorCode:" << result << ",groupId:" << groupId 
		<< ", operatorId:" << operatorId << "},});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnGroupEnded event to JS";
}

void GroupServiceStrategy::OnMemberAdded(int32_t result, int64_t groupId, 
	int32_t operatorId, const std::vector<UserInfo>& memberId) {
	std::wstringstream jsScript;
	jsScript << L"var e = new CustomEvent(\"" << GROUP_SERVICE_GROUP_MEMBER_ADDED_EVENT
		<< "\",{detail:" << "{errorCode:" << result << ",groupId:" << groupId 
		<< ", operatorId:" << operatorId << ", memberIds:[";

	for (std::size_t i = 0; i < memberId.size(); ++i) {
		jsScript << memberId[i].ucid.GetUserId() << ",";
	}

	jsScript << "]},});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnMemberAdded event to JS";
}

void GroupServiceStrategy::OnMemberRemoved(int32_t result, int64_t groupId, 
	int32_t operatorId, const std::vector<int32_t>& memberId) {
	std::wstringstream jsScript;
	jsScript << L"var e = new CustomEvent(\"" << GROUP_SERVICE_GROUP_MEMBER_REMOVED_EVENT
		<< "\",{detail:" << "{errorCode:" << result << ",groupId:" << groupId 
		<< ", operatorId:" << operatorId << ", memberIds:[";

	for (std::size_t i = 0; i < memberId.size(); ++i) {
		jsScript << memberId[i] << ",";
	}

	jsScript << "]},});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnMemberRemoved event to JS";
}			

void GroupServiceStrategy::OnGroupNameUpdated(int32_t result, int64_t groupId, 
	int32_t operatorId, const std::string& groupName, 
	const std::string& pinYinName) {
	std::wstringstream jsScript;
	jsScript << L"var e = new CustomEvent(\"" << GROUP_SERVICE_GROUP_NAME_UPDATED_EVENT
		<< "\",{detail:" << "{errorCode:" << result << ",groupId:" << groupId 
		<< ", operatorId:" << operatorId << ", groupName:\"" << UTF2WJSON(groupName)
		<< "\",pinyin:\"" << UTF2WJSON(pinYinName) << "\"},});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnGroupNameUpdated event to JS";
}

void GroupServiceStrategy::OnGroupLogoUpdated(int32_t result, int64_t groupId, 
	const std::string& logoURL) {
	std::wstringstream jsScript;
	jsScript << L"var e = new CustomEvent(\"" << GROUP_SERVICE_GROUP_LOGO_UPDATED_EVENT
		<< "\",{detail:" << "{errorCode:" << result << ",groupId:" << groupId 
		<< ",logoURL:\"" << UTF2WJSON(logoURL) << "\"},});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnGroupLogoUpdated event to JS";
}

void GroupServiceStrategy::OnDisturbSet(int32_t result, int64_t groupId, 
	int32_t isDisturb) {
	std::wstringstream jsScript;
	jsScript << L"var e = new CustomEvent(\"" << GROUP_SERVICE_DISTURB_SET_EVENT
		<< "\",{detail:" << "{errorCode:" << result << ",groupId:" << groupId 
		<< ",isDisturb:" << isDisturb << "},});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnDisturbSet event to JS";
}

void GroupServiceStrategy::OnTopChatSet(int32_t result, int64_t groupId, 
	int32_t isTop) {
	std::wstringstream jsScript;
	jsScript << L"var e = new CustomEvent(\"" << GROUP_SERVICE_TOP_CHAT_EVENT
		<< "\",{detail:" << "{errorCode:" << result << ",groupId:" << groupId 
		<< ",isTop:" << isTop << "},});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnTopChatSet event to JS";
}

void GroupServiceStrategy::OnReminderSet(int32_t result, int64_t groupId, 
	int32_t isAlert, int32_t remindRule, int32_t remindTime) {
	std::wstringstream jsScript;
	jsScript << L"var e = new CustomEvent(\"" << GROUP_SERVICE_REMINDER_SET
		<< "\",{detail:" << "{errorCode:" << result << ",groupId:" << groupId 
		<< ",isAlert:" << isAlert << ",remindRule:" << remindRule
		<< ",remindTime:" << remindTime 
		<< "},});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnReminderSet event to JS";
}

void GroupServiceStrategy::OnGroupListReceived(int32_t result, GroupStyle groupType, bool allData,
	const std::list<GroupInfo>& info) {
	std::wstringstream jsScript;
	jsScript << L"var e = new CustomEvent(\"" << GROUP_SERVICE_GROUP_LIST_RECEIVED
		<< "\",{detail:" << "{errorCode:" << result << ",groupType:" << groupType
		<< ", allData:"<< allData << ", groups:[";

	for (std::list<GroupInfo>::const_iterator iter = info.begin(); iter != info.end(); ++iter) {
		jsScript << ServiceObjectConversion::ConvertGroupInfoToJSON(*iter) << ",";	
	}

	jsScript << "]},});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnGroupListReceived event to JS";
}

void GroupServiceStrategy::OnGroupInfoReceived(int32_t result, int64_t groupId, int32_t mountId,
	const GroupInfo& info) {
	std::wstringstream jsScript;
	jsScript<<L"var e = new CustomEvent(\"" << GROUP_SERVICE_GROUP_INFO_RECEIVED
		<< "\",{detail:" << "{errorCode:" << result << ",groupId:" << groupId
		<< ",mountId:" << mountId;

	jsScript << ", group:" << ServiceObjectConversion::ConvertGroupInfoToJSON(info);
	jsScript << "},});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnGroupInfoReceived event to JS";
}

void GroupServiceStrategy::OnDocListReceived(int32_t result, int64_t groupId, 
	const std::list<DocumentInfo>& info) {
	std::wstringstream jsScript;
	jsScript << L"var e = new CustomEvent(\"" << GROUP_SERVICE_DOC_LIST_RECEIVED
		<< "\",{detail:" << "{errorCode:" << result << ",groupId:" << groupId
		<< ", documents:[";

	// TODO xuejian
	for (std::list<DocumentInfo>::const_iterator iter = info.begin(); iter != info.end(); ++iter) {
		jsScript << ServiceObjectConversion::ConvertDocumentInfoToJSON(*iter) << ",";
	}

	jsScript << "]},});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnDocListReceived event to JS";
}

void GroupServiceStrategy::OnDocInfoReceived(int32_t result, int64_t groupId, const DocumentInfo& info) {
	std::wstringstream jsScript;
	jsScript<<L"var e = new CustomEvent(\"" << GROUP_SERVICE_DOC_INFO_RECEIVED
		<< "\",{detail:" << "{errorCode:" << result << ",groupId:" << groupId;

	// TODO xuejian
	jsScript << ", document:" << ServiceObjectConversion::ConvertDocumentInfoToJSON(info);
	jsScript << "},});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnDocInfoReceived event to JS";
}

void GroupServiceStrategy::OnCodeSnippetUploaded(int32_t result, int64_t groupId, int32_t operatorId, int32_t operation, const DocumentInfo& info)
{
	std::wstringstream jsScript;
	jsScript<<L"var e = new CustomEvent(\"" << GROUP_SERVICE_CODESNIPPET_UPLOADED_RECEIVED
		<< "\",{detail:" << "{errorCode:" << result << ",groupId:" << groupId
		<< ",operatorId:" << operatorId << ",operation:" << operation << ",msgId:" << info.GetMsgId();

	jsScript << ", document:" << ServiceObjectConversion::ConvertDocumentInfoToJSON(info);
	jsScript << "},});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnCodeSnippetUploaded event to JS";
}

void GroupServiceStrategy::OnDocumentUploaded(int32_t result, int64_t groupId, int32_t operatorId, int32_t operation, const DocumentInfo& info) {
	std::wstringstream jsScript;
	jsScript<<L"var e = new CustomEvent(\"" << GROUP_SERVICE_DOC_UPLOADED_RECEIVED
		<< "\",{detail:" << "{errorCode:" << result << ",groupId:" << groupId
		<< ",operatorId:" << operatorId << ",operation:" << operation << ",msgId:" << info.GetMsgId();

	jsScript << ", document:" << ServiceObjectConversion::ConvertDocumentInfoToJSON(info);
	jsScript << "},});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnDocumentUploaded event to JS";
}

void GroupServiceStrategy::OnDocumentDeleted(int32_t result, int64_t groupId, 
	int32_t operatorId, const std::string& docName, int64_t fileId) {
	std::wstringstream jsScript;
	jsScript << L"var e = new CustomEvent(\"" << GROUP_SERVICE_DOC_DELETED
		<< "\",{detail:" << "{errorCode:" << result << ",groupId:" << groupId
		<< ",operatorId:" << operatorId << ",docName:\"" << UTF2WJSON(docName)
		<< "\",fileId:" << fileId;

	jsScript << "},});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnDocumentDeleted event to JS";
}

void GroupServiceStrategy::OnCommentListReceived(int32_t result, int64_t groupId, int64_t fileId, const std::list<CommentInfo>& info){
	std::wstringstream jsScript;
	jsScript << L"var e = new CustomEvent(\"" << GROUP_SERVICE_COMMENT_LIST_RECEIVED
		<< "\",{detail:" << "{errorCode:" << result << ",groupId:" << groupId
		<< ", comments:[";

	for (std::list<CommentInfo>::const_iterator iter = info.begin(); iter != info.end(); ++iter) {
		jsScript << ServiceObjectConversion::ConvertCommentInfoToJSON(*iter) << ",";
	}

	jsScript << "]},});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnDocListReceived event to JS";
} 
 
void GroupServiceStrategy::OnCommentCreated(int32_t result, int64_t groupId, const CommentInfo& info){
	std::wstringstream jsScript;
	jsScript <<L"var e = new CustomEvent(\"" << GROUP_SERVICE_COMMENT_CREATED
			 << "\",{detail:" << "{errorCode:" << result << ",groupId:" << groupId
			 << ",msgId:" << info.GetMsgId();
	jsScript << ", comment:" << ServiceObjectConversion::ConvertCommentInfoToJSON(info);
	jsScript << "},});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnCommentCreated event to JS";
}

void GroupServiceStrategy::OnMemberRoleUpdated(MessageChannel channel, const RoleUpdatedMessageInfo &info) {
	std::wstringstream jsScript;
	jsScript << L"var e = new CustomEvent(\"" << GROUP_SERVICE_MEMBER_ROLE_UPDATED_EVENT
		<< "\",{detail:" << "{channel:" << channel <<", ";

	jsScript << ServiceObjectConversion::AppendRoleUpdatedMessageInfoToJSON(info);

	jsScript << "}" << "});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnMemberRoleUpdated event to JS";
}


void GroupServiceStrategy::OnAtMessageListReceived(int32_t result, int64_t groupId, const std::map<int64_t, int64_t>& atMsg) {

	std::wstringstream jsScript;
	jsScript << L"var e = new CustomEvent(\"" << GROUP_SERVICE_AT_MESSAGE_LIST_RECEIVED
		<< "\",{detail:" << "{errorCode:" << result << ",groupId:" << groupId
		<< ", atMsg:[";

	for (std::map<int64_t, int64_t>::const_iterator iter = atMsg.begin(); iter != atMsg.end(); ++iter) {
		jsScript << "{ " << "sender:" << iter->second << ", seq:" << iter->first << "}, ";
	}

	jsScript << "]},});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnAtMessageListReceived event to JS";
}

void GroupServiceStrategy::OnAtMessageDeleted(int32_t result, ConversationType type, int64_t groupId, const std::vector<int64_t>& seqList)
{
	std::wstringstream jsScript;
	jsScript << L"var e = new CustomEvent(\"" << GROUP_SERVICE_AT_MESSAGE_DELETED
		<< "\",{detail:" << "{errorCode:" << result << ",conversationType:" << type << ",groupId:" << groupId << ", atSeq:[";

	for (std::vector<int64_t>::const_iterator iter = seqList.begin(); iter != seqList.end();) {
		if (iter != seqList.begin()){
			jsScript << ",";
		}
		jsScript << *iter;
		iter++;
	}

	jsScript << "]},});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnAtMessageDeleted event to JS";
}

void GroupServiceStrategy::OnRevocationMessage(int32_t result, int64_t groupId, ConversationType type, int64_t msgSeq, int32_t msgId, int64_t msgTime)
{
	std::wstringstream jsScript;

	jsScript << L"var e = new CustomEvent(\"" << GROUP_SERVICE_MSG_REVOCATION_EVENT
		<< "\",{detail:" << "{errorCode: " << result << ",groupId:"
		<< groupId << ", msgId:" << msgId << ", msgSeq:" << msgSeq << ", msgTime:" << msgTime
		<< ", revocationType:" << type
		<< "},});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnRevocationMessage event to JS";
}

void GroupServiceStrategy::OnRevocationMesssageReceived(MessageChannel channel, const MessageInfo &message, int64_t seqId, int16_t type){
	std::wostringstream jsScript;
	jsScript << L"var e= new CustomEvent(\"" << GROUP_SERVICE_MESSAGE_REVOCATION_EVENT
		<< "\",{detail:" << "{channel:" << channel << ",";
	jsScript << ServiceObjectConversion::AppendMessageInfoToJSON(message);
	jsScript << "seqId:" << seqId << "," << "type:" << type << ", ";

	jsScript << "}});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnRevocationMesssageReceived event to JS";
}

}