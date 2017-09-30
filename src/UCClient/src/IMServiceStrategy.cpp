#include "IMServiceStrategy.h"
#include "ServiceObjectConversion.h"
#include "utils/Thumbnails.h"
#include "UCDefs.h"
#include "IClientService.h"

namespace ucclient {

IMServiceStrategy::IMServiceStrategy(IUCEngine *engine, cefIntegration::CefClient *cefClient) 
	: _engine(engine), _cefClient(cefClient) {
	_imService = (IIMService*)_engine->QueryService(UCIMService);
}
 
int32_t IMServiceStrategy::GetChatHistory(MessageChannel channel, int64_t userId, int32_t count, int64_t startTime, int64_t endTime) {
	return _imService->GetChatHistory(channel, userId, count, startTime, endTime);
}

int32_t IMServiceStrategy::ResendMessage(int32_t msgId){
	return _imService->ResendMessage(msgId);
}

int32_t IMServiceStrategy::ForwardMessage(UCID& userUid, int32_t msgId){
	return _imService->ForwardMessage(userUid, msgId);
}

int32_t IMServiceStrategy::SendTextMessage(UCID &jid, int8_t textType, std::wstring& textFormat, 
	const std::wstring& textData, int32_t& msgId) {

	std::string utf8Format = StringConvert::ToUTF8Str(textFormat);
	std::string utf8Text = StringConvert::ToUTF8Str(textData);
	return _imService->SendTextMessage(jid, textType, utf8Format, utf8Text, msgId);
}

int32_t IMServiceStrategy::SendImageMessage(UCID &jid, const std::wstring& imageName, int32_t imageSize,
	const std::wstring& imageURL, const std::wstring& imagePath, int32_t& msgId) {
		
	std::string utf8ImageName = StringConvert::ToUTF8Str(imageName);
	std::string utf8ImageURL = StringConvert::ToUTF8Str(imageURL);
	std::wstring thumbnailPath = L"";
	std::string thunmbnailData = "";
	
	IClientService *service = (IClientService*)_engine->QueryService(UCClientService);
	if (service != NULL) {
		thumbnailPath = StringConvert::FromUTF8Str(service->GetDocumentDataDir(IMAGE_DIR));
	}

	ThumbnailsUtil::CreateThumbnails(imagePath, thumbnailPath, thunmbnailData);

	return _imService->SendImageMessage(jid, utf8ImageName, imageSize, utf8ImageURL, thunmbnailData, msgId);
}

int32_t IMServiceStrategy::SendAudioMessage(UCID& jid, const std::wstring& audioName, int32_t audioSize,
	const std::wstring& audioURL, int32_t audioDuration, int32_t& msgId) {

	std::string utf8AuidoName = StringConvert::ToUTF8Str(audioName);
	std::string utf8AudioURL = StringConvert::ToUTF8Str(audioURL);

	return _imService->SendAudioMessage(jid, utf8AuidoName, audioSize, utf8AudioURL, 
		audioDuration, msgId);
}

int32_t IMServiceStrategy::SendVideoMessage(UCID& jid, const std::wstring& videoName, int32_t videoSize,
	const std::wstring& videoURL, const std::wstring& videoPath, int32_t videoDuration, OUT int32_t& msgId) {

	std::string utf8VideoName = StringConvert::ToUTF8Str(videoName);
	std::string utf8VideoURL = StringConvert::ToUTF8Str(videoURL);
	std::string utf8VideoData = StringConvert::ToUTF8Str(videoPath);

	return _imService->SendVideoMessage(jid, utf8VideoName, videoSize, utf8VideoURL, 
		utf8VideoData, videoDuration, msgId);
}


int32_t IMServiceStrategy::SendReportMessage(UCID jid, const std::wstring& reportTitle, const std::wstring& reportUrl, OUT int32_t& msgId){

	std::string utf8ReportTitle = StringConvert::ToUTF8Str(reportTitle);
	std::string utf8ReportURL = StringConvert::ToUTF8Str(reportUrl);

	return _imService->SendReportMessage(jid, utf8ReportTitle, utf8ReportURL, msgId);
}

int32_t IMServiceStrategy::SendRecordVideoMessage(UCID& userUid, const std::wstring& videoName, int32_t videoSize, 
	const std::wstring& videoURL, const std::wstring& videoThumb, const std::wstring& videoThumbURL, int32_t videoDuration, int32_t operatorId,
	const std::wstring& operatorName, const std::wstring& operatorAvatar, int32_t& msgId) {
	std::string utf8VideoName = StringConvert::ToUTF8Str(videoName);
	std::string utf8VideoURL = StringConvert::ToUTF8Str(videoURL);
	std::string utf8VideoThumbData = StringConvert::ToUTF8Str(videoThumb);
	std::string utf8VideoTHumbURL = StringConvert::ToUTF8Str(videoThumbURL);
	std::string utf8OperatorName = StringConvert::ToUTF8Str(operatorName);
	std::string utf8OperatorAvatar = StringConvert::ToUTF8Str(operatorAvatar);

	return _imService->SendRecordVideoMessage(userUid, utf8VideoName, videoSize, utf8VideoURL,
		utf8VideoThumbData, utf8VideoTHumbURL, videoDuration, operatorId, utf8OperatorName, utf8OperatorAvatar, msgId);
}


int32_t IMServiceStrategy::SendFileMessage(UCID& jid, int32_t operation, const std::wstring& fileName, int32_t fileSize,
	const std::wstring& fileURL, int64_t fileId, OUT int32_t& msgId) {

	std::string utf8FileName = StringConvert::ToUTF8Str(fileName);
	std::string utf8FileURL = StringConvert::ToUTF8Str(fileURL);

	DocumentInfo info;
	info.SetFileName( utf8FileName );
	info.SetFileUrl( utf8FileURL );
	info.SetFileSize( fileSize );
	info.SetFileId( fileId );

	return _imService->SendFileMessage(jid, operation, info, msgId);
}

int32_t IMServiceStrategy::SendCloudFileMessage(UCID userUid, const std::wstring& fileName, int64_t fileSize,
	DocumentCloudType cloudType, const std::wstring& detailContent, OUT int32_t& msgId) {
	return _imService->SendCloudFileMessage(userUid, StringConvert::ToUTF8Str(fileName), fileSize, cloudType,
		StringConvert::ToUTF8Str(detailContent), msgId);
}

int32_t IMServiceStrategy::DeleteFileMessage(UCID& jid, const std::wstring& fileName, int64_t fileId, 
	int32_t& msgId) {

	std::string utf8FileName = StringConvert::ToUTF8Str(fileName);

	return _imService->DeleteFileMessage(jid, utf8FileName, fileId, msgId);
}

int32_t IMServiceStrategy::SendCodeMessage(UCID userUid, int32_t operation, int64_t codeId, const std::wstring& codeTitle, const std::wstring& langType, const std::wstring& codeHead, 
	const std::wstring& codeDesc, int32_t codeSize, const std::wstring& codeUrl, int32_t codeLine, int64_t createTime, OUT int32_t& msgId){
	std::string utf8CodeTitle = StringConvert::ToUTF8Str(codeTitle);
	std::string utf8LangType = StringConvert::ToUTF8Str(langType);
	std::string utf8CodeHead = StringConvert::ToUTF8Str(codeHead);
	std::string utf8CodeDesc = StringConvert::ToUTF8Str(codeDesc);
	std::string utf8CodeUrl = StringConvert::ToUTF8Str(codeUrl);

	DocumentInfo info;
	info.SetFileName(utf8CodeTitle );
	info.SetFileFormat( utf8LangType );
	info.SetThumbnail( utf8CodeHead );
	info.SetDescription( utf8CodeDesc );
	info.SetFileUrl( utf8CodeUrl );
	info.SetFileId( codeId );
	info.SetTotalLines(codeLine);
	info.SetFileSize(codeSize);
	info.SetCreateTime(createTime);

	return _imService->SendCodeMessage(userUid,operation, info, msgId);
}

int32_t IMServiceStrategy::SendInviteCallMessage(UCID& jid, const std::wstring& confId, 
	const std::wstring& confPwd, int32_t hostId, OUT int32_t& msgId) {

	std::string utf8ConfId = StringConvert::ToUTF8Str(confId);
	std::string utf8ConfPwd = StringConvert::ToUTF8Str(confPwd);

	return _imService->SendInviteCallMessage(jid, utf8ConfId, utf8ConfPwd, hostId, msgId);
}

int32_t IMServiceStrategy::SendAcceptCallMessage(UCID& jid, const std::wstring& confId, 
	int32_t hostId, int32_t& msgId) {

	std::string utf8ConfId = StringConvert::ToUTF8Str(confId);

	return _imService->SendAcceptCallMessage(jid, utf8ConfId, hostId, msgId);
}

int32_t IMServiceStrategy::SendRejectCallMessage(UCID& jid, const std::wstring& confId, int32_t hostId,
	const std::wstring& reason, int32_t& msgId) {

	std::string utf8ConfId = StringConvert::ToUTF8Str(confId);
	std::string utf8Reason = StringConvert::ToUTF8Str(reason);
	return _imService->SendRejectCallMessage(jid, utf8ConfId, hostId, utf8Reason, msgId);
}

int32_t IMServiceStrategy::SendFinishCallMessage(UCID& jid, const std::wstring& confId, int32_t hostId,
	const std::wstring& reason, int32_t& msgId) {

	std::string utf8ConfId = StringConvert::ToUTF8Str(confId);
	std::string utfReason = StringConvert::ToUTF8Str(reason);
	return _imService->SendFinishCallMessage(jid, utf8ConfId, hostId, utfReason, msgId);
}

int32_t IMServiceStrategy::SendCancelCallMessage(UCID& jid, const std::string& phoneNum, OUT int32_t& msgId) {
	return _imService->SendCancelCallMessage(jid, phoneNum, msgId);
}

int32_t IMServiceStrategy::SendNotRespondCallRecordMessage(UCID& jid, const string& phoneNum, OUT int32_t& msgId) {
	return _imService->SendNotRespondCallRecordMessage(jid, phoneNum, msgId);
}

/*
int32_t IMServiceStrategy::SendRejectCallRecordMessage(UCID& jid, OUT int32_t& msgId) {
	return _imService->SendRejectCallRecordMessage(jid,  msgId);
}
*/

int32_t IMServiceStrategy::SendConnectedCallRecordMessage(UCID& jid, int32_t druation, const std::string& phoneNum, int32_t& msgId) {
	return _imService->SendConnectedCallRecordMessage(jid, druation, phoneNum, msgId);
}

int32_t IMServiceStrategy::SendMessageRead(UCID& jid, int32_t msgId, int64_t msgSeq) {
	return _imService->SendMessageRead(jid, msgId, msgSeq);
}

int32_t IMServiceStrategy::SendMessagesReadById(int32_t toUserId){
	return _imService->SendMessagesRead(toUserId);
}

int32_t IMServiceStrategy::SendMessagePlayed(UCID& jid, int32_t msgId, int64_t msgSeq) {
	return _imService->SendMessagePlayed(jid, msgId,  msgSeq);
}

void IMServiceStrategy::OnChatHistoryReceived(int32_t result, MessageChannel channel, int64_t userId, int32_t msgCount, int64_t min_timestamp) {
	std::wstringstream jsScript;
	jsScript << L"var e = new CustomEvent(\"" << IM_SERVICE_CHAT_HISTORY_RECEIVED_EVENT
		<< "\",{detail:" << "{errorCode: " << result << ", channel: " << channel << ", userId: "
		<< userId  << ", msgCount: " << msgCount << ", minTimestamp: " << min_timestamp << "},});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnChatHistoryReceived event to JS";
}

void IMServiceStrategy::OnMessageSent(int32_t result, int32_t userId, int32_t msgId, int64_t msgSeq, int64_t msgTime) {
	std::wstringstream jsScript;
	jsScript<<L"var e = new CustomEvent(\"" << IM_SERVICE_MESSAGE_SENT_EVENT
		<< "\",{detail:" << "{errorCode: " << result << ", userId: " 
		<< userId << ", msgId: "<< msgId << ", msgSeq:" << msgSeq << ", msgTime:" << msgTime
		<< "},});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnMessageSent event to JS";
}

void IMServiceStrategy::OnTextMessageReceived(MessageChannel channel, const TextMessageInfo *message) {
	if (message == NULL) {
		UC_LOG(ERROR) << "fail to dispatch OnTextMessageReceived event to JS, message is NULL";
		return;
	}

	std::wstringstream jsScript;

	jsScript << L"var e = new CustomEvent(\"" << IM_SERVICE_TEXT_MESSAGE_RECEIVED_EVENT
			 << "\", {detail:" << "{channel: " << channel << ", ";
	jsScript << ServiceObjectConversion::AppendTextMessageInfoToJSON(const_cast<TextMessageInfo&>(*message));
	jsScript << "}" << "});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnTextMessageReceived event to JS";
}

void IMServiceStrategy::OnImageMessageReceived(MessageChannel channel, const MediaMessageInfo *message) {
	if (message == NULL) {
		UC_LOG(ERROR) << "fail to dispatch OnImageMessageReceived event to JS, message is NULL";
		return;
	}

	std::wstringstream jsScript;
	
	jsScript << L"var e = new CustomEvent(\"" << IM_SERVICE_IMAGE_RECEIVED_EVENT
			 << "\", {detail:" << "{channel: " << channel << ", ";
	jsScript << ServiceObjectConversion::AppendMediaMessageInfoToJSON(const_cast<MediaMessageInfo&>(*message));
	jsScript << "}" << "});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnImageReceived event to JS";
}

void IMServiceStrategy::OnAudioMessageReceived(MessageChannel channel, const MediaMessageInfo *message) {
	if (message == NULL) {
		UC_LOG(ERROR) << "fail to dispatch OnAudioMessageReceived event to JS, message is NULL";
		return;
	}

	std::wstringstream jsScript;

	jsScript << L"var e = new CustomEvent(\"" << IM_SERVICE_AUDIO_MESSAGE_RECEIVED_EVENT
			 << "\", {detail:" << "{channel: " << channel << ", ";
	jsScript << ServiceObjectConversion::AppendMediaMessageInfoToJSON(const_cast<MediaMessageInfo&>(*message));
	jsScript << "}" << "});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnAudioMessageReceived event to JS";
}

void IMServiceStrategy::OnVideoMessageReceived(MessageChannel channel, const MediaMessageInfo *message) {
	
	if (message == NULL) {
		UC_LOG(ERROR) << "fail to dispatch OnVideoMessageReceived event to JS, message is NULL";
		return;
	}

	std::wstringstream jsScript;

	jsScript << L"var e = new CustomEvent(\"" << IM_SERVICE_VIDEO_MESSAGE_RECEIVED_EVENT
			 << "\", {detail:" << "{channel: " << channel << ", ";
	jsScript << ServiceObjectConversion::AppendMediaMessageInfoToJSON(const_cast<MediaMessageInfo&>(*message));
	jsScript << "}" << "});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnVideoMessageReceived event to JS";
}


void IMServiceStrategy::OnReportMessageReceived(MessageChannel channel, const ReportMessageInfo *message){

	if (message == NULL) {
		UC_LOG(ERROR) << "fail to dispatch OnReportMessageReceived event to JS, message is NULL";
		return;
	}

	std::wstringstream jsScript;

	jsScript << L"var e = new CustomEvent(\"" << IM_SERVICE_REPORT_MESSAGE_RECEIVED_EVENT
		<< "\", {detail:" << "{channel: " << channel << ", ";
	jsScript << ServiceObjectConversion::AppendReportMessageInfoToJSON(const_cast<ReportMessageInfo&>(*message));
	jsScript << "}" << "});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnReportMessageReceived event to JS";
}

void IMServiceStrategy::OnRecordVideoReceived(MessageChannel channel, const MediaMessageInfo *message) {
	std::wstringstream jsScript;
	jsScript << L"var e = new CustomEvent(\"" << IM_SERVICE_RECORD_VIDEO_MESSAGE_RECEIVED_EVENT
		<< "\",{detail:" << "{channel: " << channel << ", ";
	jsScript << ServiceObjectConversion::AppendMediaMessageInfoToJSON(const_cast<MediaMessageInfo&>(*message));

	jsScript << "}" << "});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnRecordVideoReceived event to JS";
}


void IMServiceStrategy::OnFileMessageReceived(MessageChannel channel, int32_t operation, const FileMessageInfo *message) {
	if (message == NULL) {
		UC_LOG(ERROR) << "fail to dispatch OnFileMessageReceived event to JS, message is NULL";
		return;
	}

	std::wstringstream jsScript;

	jsScript << L"var e = new CustomEvent(\"" << IM_SERVICE_FILE_RECEIVED_EVENT
			 << "\", {detail:" << "{channel: " << channel << ", operation: " << operation << ", ";
	jsScript << ServiceObjectConversion::AppendFileMessageInfoToJSON(const_cast<FileMessageInfo&>(*message));
	jsScript << "}" << "});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnFileMessageReceived event to JS";
}

void IMServiceStrategy::OnCloudFileCreateMessageReceived(MessageChannel channel, const FileMessageInfo &message) {
	std::wstringstream jsScript;

	jsScript << L"var e = new CustomEvent(\"" << IM_SERVICE_CLOUDFILE_CREATE_RECEIVED_EVENT
		<< "\", {detail:" << "{channel: " << channel << ", ";

	jsScript << ServiceObjectConversion::AppendFileMessageInfoToJSON(message);
	jsScript << "}" << "});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnFileMessageReceived event to JS";
}

void IMServiceStrategy::OnFileDeleted(UCID jid,  int32_t msgId, int64_t msgSeq,
	const std::string& fileName, int64_t fileId) {
	std::wstringstream jsScript;

	jsScript <<L"var e = new CustomEvent(\"" << IM_SERVICE_FILE_DELETED_EVENT
			 << "\",{detail:" << "{jid: " << ServiceObjectConversion::ConvertUCIDToJSON(jid) 
			 << ", msgId: "<< msgId << ",msgSeq:" << msgSeq
			 <<", fileName: \"" << UTF2WJSON(fileName) 
			 << "\", fileId:"<< fileId << "},});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnFileDeleted event to JS";
}

void IMServiceStrategy::OnCodeMessageReceived(MessageChannel channel, int32_t operation, const CodeMessageInfo& message){

	std::wstringstream jsScript;

	jsScript << L"var e = new CustomEvent(\"" << IM_SERVICE_CODE_RECEIVED_EVENT
			 << "\", {detail:" 
			 << "{channel: " << channel << ", " << "operation: " << operation << ", " ;
	jsScript << ServiceObjectConversion::AppendCodeMessageInfoToJSON(const_cast<CodeMessageInfo&>(message));
	jsScript << "}" << "});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnCodeMessageReceived event to JS";
}

void IMServiceStrategy::OnMessageRead(int32_t result, int32_t userId, int32_t readType, 
	int32_t msgId, int64_t msgSeq) {
	std::wstringstream jsScript;
	jsScript<<L"var e = new CustomEvent(\"" << IM_SERVICE_MESSAGE_READ_EVENT
		<< "\",{detail:" << "{errorCode: " << result <<", userId: " << userId 
		<< ", readType:"<< readType << ", msgId: " << msgId
		<< ", msgSeq: " << msgSeq << "},});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnMessageReaded event to JS";
}

void IMServiceStrategy::OnMessagePlayed(int32_t result, int32_t userId, int32_t msgId, int64_t msgSeq) {
	std::wstringstream jsScript;
	jsScript<<L"var e = new CustomEvent(\"" << IM_SERVICE_MESSAGE_PLAYED_EVENT
		<< "\",{detail:" << "{errorCode: " << result <<", userId: " << userId 
		<< ", msgId: " << msgId << ", msgSeq:" << msgSeq << "},});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnMessagePlayed event to JS";
}

void IMServiceStrategy::OnMessageUnreadUserCount(int32_t result, int32_t userId, const std::list<MessageReadInfo>& info){

	std::wstringstream jsScript;
	jsScript << L"var e = new CustomEvent(\"" << IM_SERVICE_MESSAGE_UNREAD_USER_COUNT_EVENT
		<< "\",{detail:" << "{errorCode:" << result << ", userId:" << userId
		<< ", info:[";

	for (std::list<MessageReadInfo>::const_iterator iter = info.begin(); iter != info.end(); ++iter) {
		jsScript << ServiceObjectConversion::ConvertMessageReadInfoToJSON(*iter) << ",";
	}

	jsScript << "]},});document.dispatchEvent(e);";

 	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnMessageUnreadUserCount event to JS";
}

void IMServiceStrategy::OnCallInvited(MessageChannel channel, int32_t result, int32_t fromUserId, int32_t userId, 
	const std::string confId, const std::string confPwd, int32_t hostId, int32_t msgId, int64_t msgSeq, int64_t msgTime) {
	std::wstringstream jsScript;
	jsScript<<L"var e = new CustomEvent(\"" << IM_SERVICE_CALL_INVITED_EVENT
		<< "\",{detail:" << "{errorCode: " << result
		<< ", channel: " << channel
		<< ", fromUserId: " << fromUserId
		<< ", userId: " << userId 
		<< ", confId: \"" << UTF2WJSON(confId) << "\", confPwd: \"" << UTF2WJSON(confPwd)
		<< "\", hostId:" << hostId
		<< ", msgId: " << msgId << ", msgSeq:" << msgSeq << ", msgTime:" << msgTime
		<< "},});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnCallInvited event to JS";
}

void IMServiceStrategy::OnCallAccepted(MessageChannel channel, int32_t result, int32_t fromUserId, int32_t userId, 
	const std::string confId, int32_t msgId, int64_t msgSeq, int64_t msgTime) {
	std::wstringstream jsScript;
	jsScript<<L"var e = new CustomEvent(\"" << IM_SERVICE_CALL_ACCEPTED_EVENT
		<< "\",{detail:" << "{errorCode: " << result 
		<< ", channel: " << channel
		<< ", fromUserId: " << fromUserId
		<< ", userId: " << userId 
		<< ", confId: \"" << UTF2WJSON(confId)
		<< "\", msgId: " << msgId << ", msgSeq:" << msgSeq << ", msgTime:" << msgTime
		<< "},});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnCallAccepted event to JS";
}

void IMServiceStrategy::OnCallRejected(MessageChannel channel, int32_t result, int32_t fromUserId, int32_t userId,
	int32_t msgId, int64_t msgSeq, int64_t msgTime, const std::string confId, int32_t hostId, std::string reason) {
	std::wstringstream jsScript;
	jsScript<<L"var e = new CustomEvent(\"" << IM_SERVICE_CALL_REJECTED_EVENT
		<< "\",{detail:" << "{errorCode: " << result 
		<< ", channel: " << channel
		<< ", msgId: " << msgId << ",msgSeq:" << msgSeq << ", msgTime:" << msgTime
		<< ", fromUserId: " << fromUserId
		<< ", userId: " << userId 
		<< ", confId: \"" << UTF2WJSON(confId) << "\""
		<< ", hostId: " << hostId
		<< ", reason: \"" << UTF2WJSON(reason) << "\"},});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnCallRejected event to JS";
}

void IMServiceStrategy::OnCallStopped(MessageChannel channel, int32_t result, int32_t fromUserId, int32_t userId, const std::string confId,
	int32_t msgId, int64_t msgSeq, int64_t msgTime) {
	std::wstringstream jsScript;
	jsScript<<L"var e = new CustomEvent(\"" << IM_SERVICE_CALL_STOPPED_EVENT
		<< "\",{detail:" << "{errorCode: " << result 
		<< ", channel: " << channel
		<< ", fromUserId: " << fromUserId
		<< ", userId: " << userId 
		<< ", msgId: " << msgId << ",msgSeq:" << msgSeq << ", msgTime:" << msgTime
		<< ", confId: \"" << UTF2WJSON(confId) << "\"},});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnCallStopped event to JS";
}

void IMServiceStrategy::OnCallCanceled(MessageChannel channel, int32_t result, int32_t fromUserId, int32_t userId, 
	int32_t msgId, int64_t msgSeq, int64_t msgTime, const std::string& phoneNum) {
	std::wstringstream jsScript;
	jsScript<<L"var e = new CustomEvent(\"" << IM_SERVICE_CALL_CANCELLED_EVENT
		<< "\",{detail:" << "{errorCode: " << result
		<< ", channel: " << channel
		<< ", msgId: " << msgId << ",msgSeq:" << msgSeq << ", msgTime:" << msgTime
		<< ", fromUserId: " << fromUserId
		<< ", userId: " << userId 
		<< ", phoneNum:\"" << UTF2WJSON(phoneNum)
		<< "\"},});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnCallCanceled event to JS";
}

void IMServiceStrategy::OnCallNotResponded(MessageChannel channel, int32_t result, int32_t fromUserId, 
	int32_t userId, int32_t msgId, int64_t msgSeq, int64_t msgTime) {
	std::wstringstream jsScript;
	jsScript<<L"var e = new CustomEvent(\"" << IM_SERVICE_CALL_NOT_RESPONDED_EVENT
		<< "\",{detail:" << "{errorCode: " << result
		<< ", channel: " << channel
		<< ", msgId: " << msgId << ",msgSeq:" << msgSeq << ", msgTime:" << msgTime
		<< ", fromUserId: " << fromUserId
		<< ", userId: " << userId 
		<< "},});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnCallNotResponded event to JS";
}

void IMServiceStrategy::OnCallConnected(MessageChannel channel, int32_t result, int32_t fromUserId, 
	int32_t userId, int32_t msgId, int64_t msgSeq, int64_t msgTime,
	int32_t duration, const std::string& phoneNum) {
	std::wstringstream jsScript;
	jsScript<<L"var e = new CustomEvent(\"" << IM_SERVICE_CALL_CONNECTED_EVENT
		<< "\",{detail:" << "{errorCode: " << result
		<< ", channel: " << channel
		<< ", msgId: " << msgId << ",msgSeq:" << msgSeq << ", msgTime:" << msgTime
		<< ", fromUserId: " << fromUserId
		<< ", userId: " << userId 
		<< ", duration: " << duration 
		<< ", phoneNum: \"" << UTF2WJSON(phoneNum) << "\"},});document.dispatchEvent(e);";

	_cefClient->ExecuteJSForAllBrowsers(jsScript.str());

	UC_LOG(INFO) << "dispatch OnCallConnected event to JS";
}


}