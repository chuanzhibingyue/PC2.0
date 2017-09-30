/*
* IMService.cpp
*
*  Created on: Jun 1, 2015
*      Author: ethan
*/

#include "IMService.h"
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include <sstream> 
#include "MessageProcessor.h"
#include "MessageParser.h"
#include "UCEngine.h"
#include "common/StringConvert.h"
#include "DBManager.h"
#include "LogManager.h"
#include "client_service/ClientService.h"
#include "UCErrorCode.h"
#include "UCParserErrorCode.h"
#include "CommonDef.h"
#include "store_service/StoreService.h"


namespace uc {

IMService::IMService(IUCEngine *engine) {
	_engine = engine;
	_engine->registerListener(this);
}

IMService::~IMService() {
	_engine->removeListener(this);
}

int32_t IMService::GetChatHistory(MessageChannel channel, int64_t userId, int64_t count, int64_t startTime, int64_t endTime){

	std::list<MessageStoreItem*> msgList;
	int64_t starttime_t = startTime;
	int64_t endtime_t = endTime;
	int32_t update_flag_msgid = 0;

	MSG_STATUS status = theStore.GetMsgStore()->GetChatHistory(userId, SINGLE_CHAT, count, startTime, endTime, update_flag_msgid, msgList);
	if ( status != MSG_NEED_FROM_SERVER )
	{
		std::list<int64_t> seqList;
		std::list<MessageStoreItem*>::iterator iter = msgList.begin();
		for (; iter != msgList.end(); ++iter)
		{
			MessageStoreItem* item = (*iter);
			assert(item->msg);

			//已被撤回的数据不给UI抛消息
			if (item->state != STATE_REVOCATION){
				triggerEventEx(IUCEngineEvent, *_engine, OnMessageReceived(channel, *(item->msg), item->body, item->state));
			}

			UCHead head;
			item->msg->GetHead(head);
			if (GetCurUser().GetUserId() == head.GetFrom().GetUserId() && item->state == STATE_SENDSUCCESS){
				seqList.push_back(head.GetSeq());
			}
		}

		int64_t min_timestamp = 0;
		if (!msgList.empty())
		{
			MessageStoreItem* item = msgList.back();
			assert(item);
			assert(item->msg);

			UCHead head;
			item->msg->GetHead(head);

			min_timestamp = head.GetSeq();
		}
		triggerEvent(IIMServiceEvent, OnChatHistoryReceived(UC_SUCCEEDED, channel, userId, msgList.size(), min_timestamp));

		UC_LOG(INFO) << "im_service OnChatHistoryReceived from cache.result: " << UC_SUCCEEDED
			<< " userid : " << userId << " message_count : " << msgList.size() << " min_timestamp: " << min_timestamp;

		//获取当前发送消息的未读用户个数;
		if (!seqList.empty()){
			GetUnreadUserCount(userId, seqList);
		}

		if ( MSG_FROM_DB_SUCCESS == status )
		{
			theStore.GetMsgStore()->ClearMessageList( msgList );
		}
		return UC_SUCCEEDED;
	}

	HTTPRequest     request;
	stringstream	sStream;
	string         targetURL;

	targetURL = ((UCEngine*)_engine)->GetRequestURL(((ClientService*)_engine->QueryService(UCClientService))->GetLoginInfo().GetUccServerDomain(), 
		CONTENT_BY_ID_METHOD);

	sStream << targetURL << "?from_user_id=" << GetCurUser().GetUserId() << "&to_user_id=" << userId << "&type=" << 0 << 
		"&count=" << count << "&start_time=" << startTime << "&end_time=" << endTime <<
		"&session_id=" << ((ClientService*)_engine->QueryService(UCClientService))->GetLoginInfo().GetSessionId();

	request.Init(sStream.str(), HTTP_METHOD_GET);
	_engine->SendAsyncRequest(request, MakeRequestCallback(this, &IMService::OnGetChatHistoryResponse, userId, channel, count, starttime_t, endtime_t, startTime, endTime, update_flag_msgid));

	return UC_SUCCEEDED;
}

int32_t IMService::ResendMessage(int32_t msgId){
	
	MessageProcessor msgProcessor((UCEngine*)_engine);
	MessageStoreItem msgItem;
	HTTPRequest	request;
	BinMsg data;
	string body;

	if (!theStore.GetMsgStore()->GetMessageById(msgId, msgItem)) {
		UC_LOG(ERROR) << "Get message by msgId failed. msgId = " << msgId;
		return UC_FAILED;
	}
	assert(msgItem.msg);
	if (msgProcessor.EncodeMessageData(*msgItem.msg, data, body) != UC_SUCCEEDED) {
		UC_LOG(ERROR) << "encode text message failed: ";
		return UC_FAILED;
	}

	if (UC_SUCCEEDED == msgProcessor.CreateMessageRequest(data, request)) {
		UCHead head;
		msgItem.msg->GetHead(head);
		_engine->SendAsyncRequest(request, MakeRequestCallback(this, &IMService::OnSendMessageResponse, head.GetTo().GetUserId(), msgId, true));
		return UC_SUCCEEDED;
	}

	return UC_FAILED;
}

int32_t IMService::ForwardMessage(UCID userUid, int32_t msgId){
	
	MessageProcessor msgProcessor((UCEngine*)_engine);
	MessageStoreItem msgItem;
	HTTPRequest	request;
	UCMsg *msg = NULL;
	BinMsg data;
	string body;
	if (!theStore.GetMsgStore()->GetMessageById(msgId, msgItem)) {
		UC_LOG(ERROR) << "Get message by msgId failed. msgId = " << msgId;
		return UC_FAILED;
	}
	msgId = msgProcessor.CreateForwardMessage(SINGLE_CHAT, GetCurUser(), userUid, msgItem, msg);
	if (msg == NULL) {
		UC_LOG(ERROR) << "create text message failed. ";
		return UC_FAILED;
	}

// 	HTTPRequest	request;
// 	MessageProcessor msgProcessor((UCEngine*)_engine);
// 	if (UC_SUCCEEDED == msgProcessor.CreateForwardMessage(GetCurUser(), userUid, messageInfo, request)){
// 
// 		_engine->SendAsyncRequest(request, MakeRequestCallback(this, &IMService::OnSendMessageResponse, messageInfo.GetTo().GetUserId(), msgId));
// 		return UC_SUCCEEDED;
// 	}

	return UC_FAILED;
}

int32_t IMService::SendTextMessage(UCID toUser, int8_t textType, const std::string& textFormat, const std::string& textData, int32_t& msgId) {

	MessageProcessor msgProcessor((UCEngine*)_engine);
	HTTPRequest	request;
	UCMsg *msg = NULL;
	BinMsg data;
	string body;
	vector<UCID> atUsers;

	msgId = msgProcessor.CreateTextMessage(SINGLE_CHAT, GetCurUser(), toUser, textType, textFormat, textData, "", "", 0, 0, 0, atUsers, msg);
	if (msg == NULL) {
		UC_LOG(ERROR) << "create text message failed. ";
		return UC_FAILED;
	}

	if (msgProcessor.EncodeMessageData(*msg, data, body) != UC_SUCCEEDED) {
		UC_LOG(ERROR) << "encode text message failed: ";
		return UC_FAILED;
	}

	if (msgProcessor.SaveMessage(SINGLE_CHAT, toUser.GetUserId(), 0, 0, STATE_SENDING, msg, body) != UC_SUCCEEDED){
		UC_LOG(ERROR) << "save message in cache failed. relate_id: " << toUser.GetUserId();
		delete msg;
		msg = NULL;
		return UC_FAILED;
	}

	if (UC_SUCCEEDED == msgProcessor.CreateMessageRequest(data, request)) {
		_engine->SendAsyncRequest(request, MakeRequestCallback(this, &IMService::OnSendMessageResponse, toUser.GetUserId(), msgId, true));
		delete msg;
		msg = NULL;
		return UC_SUCCEEDED;
	}

	delete msg;
	msg = NULL;
	return UC_FAILED;
}

int32_t IMService::SendImageMessage(UCID userUid, const std::string& imageName, int32_t imageSize,
	const std::string& imageURL, const std::string& imageData, int32_t& msgId) {
	
	MessageProcessor msgProcessor((UCEngine*)_engine);
	HTTPRequest	request;
	UCMsg *msg = NULL;
	BinMsg data;
	string body;
	
	msgId = msgProcessor.CreateImageMessage(SINGLE_CHAT, GetCurUser(), userUid, imageName, imageSize, imageURL, imageData, "", -1, 0, 0, 0, msg);
	if (msg == NULL) {
		UC_LOG(ERROR) << "create image message failed.";
		return UC_FAILED;
	}

	if (msgProcessor.EncodeMessageData(*msg, data, body) != UC_SUCCEEDED) {
		UC_LOG(ERROR) << "encode image message failed.";
		return UC_FAILED;
	}
	
	if (msgProcessor.SaveMessage(SINGLE_CHAT, userUid.GetUserId(), 0, 0, STATE_SENDING, msg, body) != UC_SUCCEEDED){
		UC_LOG(ERROR) << "save message in cache failed. relate_id: " << userUid.GetUserId();
		delete msg;
		msg = NULL;
		return UC_FAILED;
	}

	if (UC_SUCCEEDED == msgProcessor.CreateMessageRequest(data, request)) {
		_engine->SendAsyncRequest(request, MakeRequestCallback(this, &IMService::OnSendMessageResponse, userUid.GetUserId(), msgId, true));
		delete msg;
		msg = NULL;
		return UC_SUCCEEDED;
	}

	delete msg;
	msg = NULL;
	return UC_FAILED;
}

int32_t IMService::SendAudioMessage(UCID userUid, const std::string& audioName, int32_t audioSize,
	const std::string& audioURL, int32_t audioDuration, int32_t& msgId) {
	
	MessageProcessor msgProcessor((UCEngine*)_engine);
	HTTPRequest	request;
	UCMsg *msg = NULL;
	BinMsg data;
	string body;

	msgId = msgProcessor.CreateAudioMessage(SINGLE_CHAT, GetCurUser(), userUid, audioName, audioSize, audioURL, audioDuration, 0, 0, 0, msg);
	if (msg == NULL) {
		UC_LOG(ERROR) << "create audio message failed.";
		return UC_FAILED;
	}

	if (msgProcessor.EncodeMessageData(*msg, data, body) != UC_SUCCEEDED) {
		UC_LOG(ERROR) << "encode audio message failed.";
		return UC_FAILED;
	}
	
	if (msgProcessor.SaveMessage(SINGLE_CHAT, userUid.GetUserId(), 0, 0, STATE_SENDING, msg, body) != UC_SUCCEEDED){
		UC_LOG(ERROR) << "save message in cache failed. relate_id: " << userUid.GetUserId();
		delete msg;
		msg = NULL;
		return UC_FAILED;
	}

	if (UC_SUCCEEDED == msgProcessor.CreateMessageRequest(data, request)) {
		_engine->SendAsyncRequest(request, MakeRequestCallback(this, &IMService::OnSendMessageResponse, userUid.GetUserId(), msgId, true));
		delete msg;
		msg = NULL;
		return UC_SUCCEEDED;
	}

	delete msg;
	msg = NULL;
	return UC_FAILED;
}

int32_t IMService::SendVideoMessage(UCID userUid, const std::string& videoName, int32_t videoSize,
	const std::string& videoURL, const std::string& videoData, int32_t videoDuration, int32_t& msgId) {

	MessageProcessor msgProcessor((UCEngine*)_engine);
	HTTPRequest	request;
	UCMsg *msg = NULL;
	BinMsg data;
	string body;

	msgId = msgProcessor.CreateVideoMessage(SINGLE_CHAT, GetCurUser(), userUid, videoName, videoSize, videoURL, videoData, videoDuration, 0, 0, 0, msg);
	if (msg == NULL) {
		UC_LOG(ERROR) << "create video message failed.";
		return UC_FAILED;
	}

	if (msgProcessor.EncodeMessageData(*msg, data, body) != UC_SUCCEEDED) {
		UC_LOG(ERROR) << "encode video message failed.";
		return UC_FAILED;
	}

	if (msgProcessor.SaveMessage(SINGLE_CHAT, userUid.GetUserId(), 0, 0, STATE_SENDING, msg, body) != UC_SUCCEEDED){
		UC_LOG(ERROR) << "save message in cache failed. relate_id: " << userUid.GetUserId();
		delete msg;
		msg = NULL;
		return UC_FAILED;
	}

	if (UC_SUCCEEDED == msgProcessor.CreateMessageRequest(data, request)) {
		_engine->SendAsyncRequest(request, MakeRequestCallback(this, &IMService::OnSendMessageResponse, userUid.GetUserId(), msgId, true));
		delete msg;
		msg = NULL;
		return UC_SUCCEEDED;
	}

	delete msg;
	msg = NULL;
	return UC_FAILED;
}

int32_t IMService::SendReportMessage(UCID userUid, const std::string& reportTitle, const std::string& reportUrl, OUT int32_t& msgId){

	MessageProcessor msgProcessor((UCEngine*)_engine);
	HTTPRequest	request;
	UCMsg *msg = NULL;
	BinMsg data;
	string body;

	msgId = msgProcessor.CreateReportMessage(SINGLE_CHAT, GetCurUser(), userUid, reportTitle, reportUrl, 0, msg);
	if (msg == NULL) {
		UC_LOG(ERROR) << "create report message failed.";
		return UC_FAILED;
	}

	if (msgProcessor.EncodeMessageData(*msg, data, body) != UC_SUCCEEDED) {
		UC_LOG(ERROR) << "encode report message failed.";
		return UC_FAILED;
	}

	if (msgProcessor.SaveMessage(SINGLE_CHAT, userUid.GetUserId(), 0, 0, STATE_SENDING, msg, body) != UC_SUCCEEDED){
		UC_LOG(ERROR) << "save message in cache failed. relate_id: " << userUid.GetUserId();
		delete msg;
		msg = NULL;
		return UC_FAILED;
	}

	if (UC_SUCCEEDED == msgProcessor.CreateMessageRequest(data, request)) {
		_engine->SendAsyncRequest(request, MakeRequestCallback(this, &IMService::OnSendMessageResponse, userUid.GetUserId(), msgId, false));
		delete msg;
		msg = NULL;
		return UC_SUCCEEDED;
	}

	delete msg;
	msg = NULL;
	return UC_FAILED;
}


int32_t IMService::SendRecordVideoMessage(UCID userUid, const std::string& videoName, int32_t videoSize, const std::string& videoURL, 
	const std::string& videoThumb, const std::string& videoThumbURL, int32_t videoDuration, int32_t operatorId, const std::string& operatorName,
	const std::string& operatorAvatar, int32_t& msgId) {

	MessageProcessor msgProcessor((UCEngine*)_engine);
	HTTPRequest	request;
	UCMsg *msg = NULL;
	BinMsg data;
	string body;

	msgId = msgProcessor.CreateRecordVideoMessage(SINGLE_CHAT, GetCurUser(), userUid, videoName, videoSize, videoURL, videoThumb, videoThumbURL, videoDuration, operatorId, operatorName, operatorAvatar, 0, 0, 0, msg);
	if (msg == NULL) {
		UC_LOG(ERROR) << "create record video message failed.";
		return UC_FAILED;
	}

	if (msgProcessor.EncodeMessageData(*msg, data, body) != UC_SUCCEEDED) {
		UC_LOG(ERROR) << "encode record video message failed.";
		return UC_FAILED;
	}

	if (msgProcessor.SaveMessage(SINGLE_CHAT, userUid.GetUserId(), 0, 0, STATE_SENDING, msg, body) != UC_SUCCEEDED){
		UC_LOG(ERROR) << "save record video message in cache failed. relate_id: " << userUid.GetUserId();
		delete msg;
		msg = NULL;
		return UC_FAILED;
	}

	if (UC_SUCCEEDED == msgProcessor.CreateMessageRequest(data, request)) {
		_engine->SendAsyncRequest(request, MakeRequestCallback(this, &IMService::OnSendMessageResponse, userUid.GetUserId(), msgId, true));
		delete msg;
		msg = NULL;
		return UC_SUCCEEDED;
	}

	delete msg;
	msg = NULL;
	return UC_FAILED;
}

int32_t IMService::SendFileMessage(UCID userUid, int32_t operation, const DocumentInfo& info, OUT int32_t& msgId)
{
	MessageProcessor msgProcessor((UCEngine*)_engine);
	HTTPRequest	request;
	UCMsg *msg = NULL;
	BinMsg data;
	string body;

	if ( operation == 0 )
	{
		msgId = msgProcessor.CreateSendFileMessage(SINGLE_CHAT, GetCurUser(), userUid, info.GetFileName(), info.GetFileSize(), info.GetFileUrl(), 0, "", 0, -1, 0, 0, 0, msg);
	}
	else if ( operation == 1 )
	{
		msgId = msgProcessor.CreateShareFileMessage(SINGLE_CHAT, GetCurUser(), userUid, info.GetFileName(), info.GetFileSize(), info.GetFileUrl(), 0, 0, 0, 0, msg);
	}
	else
	{
		UC_LOG(ERROR) << "do not support this type";
		return UC_FAILED;
	}
	
	if (msg == NULL) {
		UC_LOG(ERROR) << "create send file message failed.";
		return UC_FAILED;
	}

	if (msgProcessor.EncodeMessageData(*msg, data, body) != UC_SUCCEEDED) {
		UC_LOG(ERROR) << "encode send file message failed.";
		return UC_FAILED;
	}
	
	if (msgProcessor.SaveMessage(SINGLE_CHAT, userUid.GetUserId(), 0, 0, STATE_SENDING, msg, body) != UC_SUCCEEDED){
		UC_LOG(ERROR) << "save message in cache failed. relate_id: " << userUid.GetUserId();
		delete msg;
		msg = NULL;
		return UC_FAILED;
	}

	if (UC_SUCCEEDED == msgProcessor.CreateMessageRequest(data, request)) {
		_engine->SendAsyncRequest(request, MakeRequestCallback(this, &IMService::OnSendMessageResponse, userUid.GetUserId(), msgId, true));
		delete msg;
		msg = NULL;
		return UC_SUCCEEDED;
	}

	delete msg;
	msg = NULL;
	return UC_FAILED;
}

int32_t IMService::SendCloudFileMessage(UCID userUid, const std::string& fileName, int64_t fileSize,
	DocumentCloudType cloudType, const std::string& detailContent, OUT int32_t& msgId) {
	MessageProcessor msgProcessor((UCEngine*)_engine);
	HTTPRequest	request;
	UCMsg *msg = NULL;
	BinMsg data;
	string body;

	msgId = msgProcessor.CreateSendCloudFileMessage(SINGLE_CHAT, CFO_CloudFileCreate, GetCurUser(), userUid, GetCurUserName(), fileName, "", fileSize,
		1, 0, cloudType, detailContent, 0, 0, msg);

	if (msg == NULL) {
		UC_LOG(ERROR) << "send cloud file message failed.";
		return UC_FAILED;
	}

	if (msgProcessor.EncodeMessageData(*msg, data, body) != UC_SUCCEEDED) {
		UC_LOG(ERROR) << "encode send cloud message failed.";
		return UC_FAILED;
	}

	if (msgProcessor.SaveMessage(SINGLE_CHAT, userUid.GetUserId(), 0, 0, STATE_SENDING, msg, body) != UC_SUCCEEDED){
		UC_LOG(ERROR) << "save message in cache failed. relate_id: " << userUid.GetUserId();
		delete msg;
		msg = NULL;
		return UC_FAILED;
	}

	if (UC_SUCCEEDED == msgProcessor.CreateMessageRequest(data, request)) {
		_engine->SendAsyncRequest(request, MakeRequestCallback(this, &IMService::OnSendMessageResponse, userUid.GetUserId(), msgId, true));
		delete msg;
		msg = NULL;
		return UC_SUCCEEDED;
	}

	delete msg;
	msg = NULL;
	return UC_FAILED;
}

 int32_t IMService::DeleteFileMessage(UCID userUid, const std::string& fileName, int64_t fileId, OUT int32_t& msgId){

	 MessageProcessor msgProcessor((UCEngine*)_engine);
	 HTTPRequest	request;
	 UCMsg *msg = NULL;
	 BinMsg data;
	 string body;

	 msgId = msgProcessor.CreateDeleteFileMessage(SINGLE_CHAT, GetCurUser(), userUid, fileName, fileId, msg);
	 if (NULL == msg){
		 UC_LOG(ERROR) << "create delete file message failed.";
		 return UC_FAILED;
	 }

	 if (msgProcessor.EncodeMessageData(*msg, data, body) != UC_SUCCEEDED) {
		 UC_LOG(ERROR) << "encode delete file message failed.";
		 return UC_FAILED;
	 }

	 if (UC_SUCCEEDED == msgProcessor.CreateMessageRequest(data, request)) {
		 _engine->SendAsyncRequest(request, MakeRequestCallback(this, &IMService::OnSendMessageResponse, userUid.GetUserId(), msgId, false));
		 delete msg;
		 msg = NULL;
		 return UC_SUCCEEDED;
	 }
	 delete msg;
	 msg = NULL;
	 return UC_FAILED;
 }

 int32_t IMService::SendCodeMessage(UCID userUid, int32_t operation, const DocumentInfo& info, OUT int32_t& msgId){

	 MessageProcessor msgProcessor((UCEngine*)_engine);
	 HTTPRequest	request;
	 UCMsg *msg = NULL;
	 BinMsg data;
	 string body;

	 if ( operation == 0 )
	 {
		 msgId = msgProcessor.CreateSendCodeMessage(SINGLE_CHAT, GetCurUser(), userUid, GetCurUserName(), 0, info.GetFileName(), info.GetFileFormat(), 
			 info.GetThumbnail(), info.GetDescription(), info.GetFileSize(), info.GetFileUrl(), info.GetTotalLines(), info.GetCreateTime(), msg);
	 }
	 else if ( operation == 1 )
	 {
		 msgId = msgProcessor.CreateShareCodeMessage(SINGLE_CHAT, GetCurUser(), userUid, GetCurUserName(), 0, info.GetFileName(), info.GetFileFormat(), 
			 info.GetThumbnail(), info.GetDescription(), info.GetFileSize(), info.GetFileUrl(), info.GetTotalLines(), info.GetCreateTime(), msg);
	 }
	 else
	 {
		 UC_LOG(ERROR) << "do not support this type";
		 return UC_FAILED;
	 }
	
	 if (NULL == msg){
		 UC_LOG(ERROR) << "create share code message failed.";
		 return UC_FAILED;
	 }

	 if (msgProcessor.EncodeMessageData(*msg, data, body) != UC_SUCCEEDED) {
		 UC_LOG(ERROR) << "encode send code message failed.";
		 return UC_FAILED;
	 }

	 if (msgProcessor.SaveMessage(SINGLE_CHAT, userUid.GetUserId(), 0, 0, STATE_SENDING, msg, body) != UC_SUCCEEDED){
		 UC_LOG(ERROR) << "save message in cache failed. relate_id: " << userUid.GetUserId();
		 delete msg;
		 msg = NULL;
		 return UC_FAILED;
	 }

	 if (UC_SUCCEEDED == msgProcessor.CreateMessageRequest(data, request)) {
		 _engine->SendAsyncRequest(request, MakeRequestCallback(this, &IMService::OnSendMessageResponse, userUid.GetUserId(), msgId, true));
		 delete msg;
		 msg = NULL;
		 return UC_SUCCEEDED;
	 }
	 delete msg;
	 msg = NULL;
	 return UC_FAILED;
 }

int32_t IMService::SendInviteCallMessage(UCID userUid, const std::string& confId, const std::string& confPwd, int32_t hostId, int32_t& msgId){
	int64_t msgTime = 0;
	HTTPRequest	request;
	MessageProcessor msgProcessor((UCEngine*)_engine);

	if(UC_SUCCEEDED == msgProcessor.CreateCallInvitationMessage(SINGLE_CHAT, GetCurUser(), userUid, confId, confPwd, hostId, request, msgId, msgTime)){
		UC_LOG(INFO) << "CreateCallInvitationMessage, cur user:" << GetCurUser().GetUserId()
			<< ", userUid:" << userUid.GetUserId() << ", msgId:" << msgId << ", confId:" << confId;
		_engine->SendAsyncRequest(request, MakeRequestCallback(this, &IMService::OnSendMessageResponse, userUid.GetUserId(), msgId, false));
		return UC_SUCCEEDED;
	}
	return UC_FAILED;
}

int32_t IMService::SendInviteCallMessage(int callType, UCID userUid, const std::string& confId,
	const std::string& confPwd, int32_t hostId, int32_t& msgId, AudioService* service) {
	UC_LOG(INFO) << "SendInviteCallMessage, callType:" << callType << ", userId:" << userUid.GetUserId()
		<< ", confId:" << confId << ", confPwd:" << confPwd << ", hostId:" << hostId
		<< ", msgId:" << msgId;
	int64_t msgTime = 0;
	HTTPRequest	request;
	MessageProcessor msgProcessor((UCEngine*)_engine);

	if (UC_SUCCEEDED == msgProcessor.CreateCallInvitationMessage(SINGLE_CHAT, GetCurUser(), userUid, confId, confPwd, hostId, request, msgId, msgTime)){
		UC_LOG(INFO) << "CreateCallInvitationMessage, cur user:" << GetCurUser().GetUserId()
			<< ", userUid:" << userUid.GetUserId() << ", msgId:" << msgId << ", confId:" << confId;
		_engine->SendAsyncRequest(request, MakeRequestCallback(this, &IMService::OnSendInviteCallMessageResponse, std::stoi(confId), callType, userUid.GetUserId(), msgId, false, service));
		return UC_SUCCEEDED;
	}
	return UC_FAILED;
}

int32_t IMService::SendAcceptCallMessage(UCID userUid, const std::string& confId, int32_t hostId, int32_t& msgId){
	int64_t msgTime = 0;
	HTTPRequest	request;
	MessageProcessor msgProcessor((UCEngine*)_engine);

	if (UC_SUCCEEDED == msgProcessor.CreateCallAgreeMentMessage(SINGLE_CHAT, GetCurUser(), userUid, confId, hostId, request, msgId, msgTime)){
		UC_LOG(INFO) << "CreateCallAgreeMentMessage, cur user:" << GetCurUser().GetUserId()
			<< ", userUid:" << userUid.GetUserId() << ", msgId:" << msgId << ", confId:" << confId;
		_engine->SendAsyncRequest(request,
			MakeRequestCallback(this, &IMService::OnSendMessageResponse, userUid.GetUserId(), msgId, false));
		return UC_SUCCEEDED;
	}
	return UC_FAILED;	
}

int32_t IMService::SendRejectCallMessage(UCID userUid, const std::string& confId, int32_t hostId, const std::string& reason, int32_t& msgId){
	int64_t msgTime = 0;
	HTTPRequest	request;
	MessageProcessor msgProcessor((UCEngine*)_engine);

	if (UC_SUCCEEDED == msgProcessor.CreateCallRejectMessage(SINGLE_CHAT, GetCurUser(), userUid, confId, hostId, reason, request, msgId, msgTime)){
		UC_LOG(INFO) << "CreateCallRejectMessage, cur user:" << GetCurUser().GetUserId()
			<< ", userUid:" << userUid.GetUserId() << ", msgId:" << msgId << ", confId:" << confId;
		_engine->SendAsyncRequest(request,
			MakeRequestCallback(this, &IMService::OnSendMessageResponse, userUid.GetUserId(), msgId, false));
		return UC_SUCCEEDED;
	}
	return UC_FAILED;
}

int32_t IMService::SendFinishCallMessage(UCID userUid, const std::string& confId, int32_t hostId, const std::string& reason, int32_t& msgId){
	int64_t msgTime = 0;
	HTTPRequest	request;
	MessageProcessor msgProcessor((UCEngine*)_engine);

	if (UC_SUCCEEDED == msgProcessor.CreateCallFinishMessage(SINGLE_CHAT, GetCurUser(), userUid, confId, hostId, request, msgId, msgTime)){
		UC_LOG(INFO) << "CreateCallFinishMessage, cur user:" << GetCurUser().GetUserId()
			<< ", userUid:" << userUid.GetUserId() << ", msgId:" << msgId << ", confId:" << confId;
		_engine->SendAsyncRequest(request,
			MakeRequestCallback(this, &IMService::OnSendMessageResponse, userUid.GetUserId(), msgId, false));
		return UC_SUCCEEDED;
	}
	return UC_FAILED;
}

int32_t IMService::SendCancelCallMessage(UCID userUid, const string& phoneNum, int32_t& msgId){
	int64_t msgTime = 0;
	HTTPRequest	request;
	MessageProcessor msgProcessor((UCEngine*)_engine);

	if (UC_SUCCEEDED == msgProcessor.CreateCallCancelMessage(SINGLE_CHAT, GetCurUser(), userUid, phoneNum, request, msgId, msgTime)){
		UC_LOG(INFO) << "CreateCallCancelMessage, cur user:" << GetCurUser().GetUserId()
			<< ", userUid:" << userUid.GetUserId() << ", msgId:" << msgId;
		_engine->SendAsyncRequest(request,
			MakeRequestCallback(this, &IMService::OnSendMessageResponse, userUid.GetUserId(), msgId, false));
		return UC_SUCCEEDED;
	}
	return UC_FAILED;	
}

int32_t IMService::SendNotRespondCallRecordMessage(UCID userUid, const std::string& phoneNum, int32_t& msgId){
	int64_t msgTime = 0;
	HTTPRequest	request;
	MessageProcessor msgProcessor((UCEngine*)_engine);

	if (UC_SUCCEEDED == msgProcessor.CreateNotRespondCallRecordMessage(SINGLE_CHAT, GetCurUser(), userUid, phoneNum, request, msgId, msgTime)){
		UC_LOG(INFO) << "CreateNotRespondCallRecordMessage, cur user:" << GetCurUser().GetUserId()
			<< ", userUid:" << userUid.GetUserId() << ", msgId:" << msgId;
		_engine->SendAsyncRequest(request,
			MakeRequestCallback(this, &IMService::OnSendMessageResponse, userUid.GetUserId(), msgId, false));
		return UC_SUCCEEDED;
	}
	return UC_FAILED;	
}

// int32_t IMService::SendRejectCallRecordMessage( UCID userUid, int32_t& msgId){
// 
// 	HTTPRequest	request;
// 	MessageProcessor msgProcessor((UCEngine*)_engine);
// 
// 	if(msgProcessor.CreateRejectCallRecordMessage(SINGLE_CHAT, GetCurUser(), userUid, request, msgId)){
// 		_engine->SendAsyncRequest(request,
// 			MakeRequestCallback(this, &IMService::OnSendMessageResponse, userUid.GetUserId(), msgId));
// 		return UC_SUCCEEDED;
// 	}
// 	return UC_FAILED;
// }

int32_t IMService::SendConnectedCallRecordMessage(UCID userUid, int32_t druation, const std::string& phoneNum, int32_t& msgId){
	int64_t msgTime = 0;
	HTTPRequest	request;
	MessageProcessor msgProcessor((UCEngine*)_engine);

	if (UC_SUCCEEDED == msgProcessor.CreateConnectedCallRecordMessage(SINGLE_CHAT, GetCurUser(), userUid, druation, phoneNum, request, msgId, msgTime)){
		UC_LOG(INFO) << "CreateConnectedCallRecordMessage, cur user:" << GetCurUser().GetUserId()
			<< ", userUid:" << userUid.GetUserId() << ", msgId:" << msgId;
		_engine->SendAsyncRequest(request,
			MakeRequestCallback(this, &IMService::OnSendMessageResponse, userUid.GetUserId(), msgId, false));
		return UC_SUCCEEDED;
	}
	return UC_FAILED;
}

int32_t IMService::SendMessageRead(UCID userUid, int32_t msgId, int64_t msgSeq){
	
	HTTPRequest	request;
	MessageProcessor msgProcessor((UCEngine*)_engine);
	
	if(UC_SUCCEEDED == msgProcessor.CreateMessageReadMessage(SINGLE_CHAT, GetCurUser(), userUid, msgId, msgSeq, request)){
		_engine->SendAsyncRequest(request,
			MakeRequestCallback(this, &IMService::OnSendMessageReadResponse, userUid.GetUserId(), 0, msgId, msgSeq));
		return UC_SUCCEEDED;
	}
	return UC_FAILED;
} 

int32_t IMService::SendMessagesRead(int32_t toUserId){

	HTTPRequest     request;
	stringstream	sStream;
	string         targetURL;

	targetURL = ((UCEngine*)_engine)->GetRequestURL(((ClientService*)_engine->QueryService(UCClientService))->GetLoginInfo().GetUccServerDomain(), 
		MESSGAES_READ_METHOD);

	request.Init(targetURL, HTTP_METHOD_POST);
	request.AddHeaderInfo("Content-Type", "application/x-www-form-urlencoded");

	sStream  << "session_id=" << ((ClientService*)_engine->QueryService(UCClientService))->GetLoginInfo().GetSessionId()<<
		"&user_id=" << GetCurUser().GetUserId() << "&to_user_id=" << toUserId << "&type=0";

	request.SetBody(sStream.str());
	_engine->SendAsyncRequest(request, MakeRequestCallback(this, &IMService::OnSendMessageReadResponse, toUserId, 1, 0, (int64_t)0));
	return UC_SUCCEEDED;
}
	
int32_t IMService::SendMessagePlayed(UCID userUid, int32_t msgId, int64_t msgSeq) {

	HTTPRequest	request;
	MessageProcessor msgProcessor((UCEngine*)_engine);

	if(UC_SUCCEEDED == msgProcessor.CreateMessagePlayedMessage(SINGLE_CHAT, GetCurUser(), userUid, msgId, msgSeq, request)){
		_engine->SendAsyncRequest(request,
			MakeRequestCallback(this, &IMService::OnSendMessagePlayedResponse, userUid.GetUserId(), msgId, msgSeq));
		return UC_SUCCEEDED;
	}
	return UC_FAILED;
}

int32_t IMService::GetUnreadUserCount(int32_t userId, std::list<int64_t> seq){
	std::list<MessageReadInfo> infolist;
	theStore.GetMessageReadStore()->GetMessageReadList(ConvKey(SINGLE_CHAT, userId), seq, infolist);
	if (!infolist.empty()){
		triggerEvent(IIMServiceEvent, OnMessageUnreadUserCount(UC_SUCCEEDED, userId, infolist));
	}

	if (seq.empty()){
		return UC_SUCCEEDED;
	}

	HTTPRequest     request;
	stringstream	sStream;
	string         	targetURL;

	targetURL = ((UCEngine*)_engine)->GetRequestURL(((ClientService*)_engine->QueryService(UCClientService))->GetLoginInfo().GetUccServerDomain(),
		MESSAGE_UNREADUSER_COUNT_METHOD);

	request.Init(targetURL, HTTP_METHOD_POST);
	request.AddHeaderInfo("Content-Type", "application/x-www-form-urlencoded");

	wstringstream sData;
	sData << "[";
	for (std::list<int64_t>::iterator iter = seq.begin(); iter != seq.end(); ++iter){
		if (iter != seq.begin()){
			sData << ",";
		}
		sData << *iter;
	}
	sData << "]";

	sStream << "session_id=" << ((ClientService*)_engine->QueryService(UCClientService))->GetLoginInfo().GetSessionId() <<
		"&user_id=" << GetCurUser().GetUserId() << "&to_user_id=" << userId << "&type=" << 0 <<
		"&data=" << StringConvert::URLEncode(StringConvert::ToUTF8Str(sData.str()));

	request.SetBody(sStream.str());
	_engine->SendAsyncRequest(request, MakeRequestCallback(this, &IMService::OnGetUnreadUserCountResponse, userId, seq));

	return UC_SUCCEEDED;
}


int32_t IMService::GetMessageFromJSON(const boost::property_tree::wptree &jsonMsg, UCMsg *&msg, string &body) {
	MessageProcessor msgProcessor((UCEngine*)_engine);
	return msgProcessor.GetMessageFromJSON(jsonMsg, msg, body);
}

UCID IMService::GetCurUser(){
	UCID userUid;
	if (_engine){
		userUid = ((ClientService*)_engine->QueryService(UCClientService))->GetLoginInfo().GetCurUser();
	}
	return 	userUid;
};

std::string IMService::GetCurUserName(){
	std::string displayName = "";
	if (_engine){
		displayName = ((ClientService*)_engine->QueryService(UCClientService))->GetLoginInfo().GetDisplayName();
	}
	return 	displayName;
}

bool IMService::IsCurUser(UCID userUid){

	if (userUid.GetUserId() == GetCurUser().GetUserId()
		&& userUid.GetResId() == GetCurUser().GetResId()
		&& userUid.GetSiteId() == GetCurUser().GetSiteId()){
			return true;
	}
	return false;
}

int32_t IMService::OnGetChatHistoryResponse(int32_t errorCode, HTTPResponse* response, int64_t userId, MessageChannel channel, int64_t count, 
	int64_t uistartTime, int64_t uiendTime, int64_t sdkstarttime, int64_t sdkendtime, int32_t update_flag_msgid){

	UC_LOG(INFO) << "OnGetChatHistoryResponse UserId:" << userId <<
		"errorCode:" << errorCode << "response:" << (response != NULL ? response->GetContent() : "");

	int32_t msgCount = 0;
	int32_t serverCount = 0;
	int32_t result = UC_FAILED;
	std::list< MessageStoreItem* > msglist;
	std::list<int64_t> seqList;
	MessageProcessor msgProcessor((UCEngine*)_engine);
	if (UC_SUCCEEDED  ==  errorCode && NULL != response && 200 == response->GetStatusCode()) {
		wstringstream wsStream;
		wsStream.str(StringConvert::FromUTF8Str(response->GetContent()).c_str());
		boost::property_tree::wptree jsonRoot;
		boost::property_tree::wptree jsonChild;
		try{
			boost::property_tree::json_parser::read_json(wsStream, jsonRoot);
			result = jsonRoot.get<int32_t>(L"code");
		}
		catch (boost::property_tree::ptree_error & e) {
			UC_LOG(ERROR) << "encounter an exception when parse json from OnSendMessageResponse  response, exception: " << e.what();
			result = JSON_PARSE_ERROR;
		}
		if (0 == result) {
			UCMsg *msg = NULL;
			std::string body;
			try{
				jsonChild = jsonRoot.get_child(L"data");
			}
			catch (boost::property_tree::ptree_error & e) {
				UC_LOG(ERROR) << "encounter an exception when parse json from OnGetChatHistoryResponse response , exception: " << e.what();
				result = JSON_PARSE_ERROR;
			}
			BOOST_FOREACH(boost::property_tree::wptree::value_type& v, jsonChild){
				boost::property_tree::wptree jsonElement = v.second;
				serverCount++;
				try{
					if (msgProcessor.GetMessageFromJSON(jsonElement, msg, body) == UC_SUCCEEDED) {

						UCHead head;
						msg->GetHead(head);
						int64_t relateId = MessageProcessor::GetRelateId(SINGLE_CHAT, GetCurUser().GetUserId(), head.GetFrom().GetUserId(), head.GetTo().GetUserId());
						MessageStoreItem* msg_item = new MessageStoreItem();
						msg_item->msg = msg;
						msg_item->body = body.c_str();
						msg_item->relate_id = relateId;
						msg_item->state = STATE_SENDSUCCESS;
						msg_item->conversation_type = SINGLE_CHAT;
						msglist.push_back(msg_item);

						//获取当前用户发送消息的未读用户个数;
						if (head.GetFrom().GetUserId() == GetCurUser().GetUserId()){
							MessageReadUserInfo info;
							info.SetConversationType(SINGLE_CHAT);
							info.SetRelateId(relateId);
							info.SetMsgSeq(head.GetSeq());
							info.SetUnreadCount(jsonElement.get<int32_t>(L"unread_user_count", -1));
							if (info.GetUnreadCount() == MESSAGE_UNREAD_NONE){
								info.SetAllRead(true);
							}
							theStore.GetMessageReadStore()->SaveMessageReadInfo(ConvKey(SINGLE_CHAT, relateId), info);
						}
					}else{
						UC_LOG(ERROR) << "why parse a message json to thrift data return error";
						result = UC_JSON_PARSE_ONE_DATA_ERROR;
					}
				}catch (boost::property_tree::ptree_error & e) {
					UC_LOG(ERROR) << "encounter an exception when parse json from OnGetChatHistoryResponse single message, exception: " << e.what();
					result = UC_JSON_PARSE_ONE_DATA_ERROR;
				}
			}
		}
	} else {
		result = errorCode != UC_SUCCEEDED ? errorCode : (NULL != response && 200 != response->GetStatusCode() ? response->GetStatusCode() : UC_FAILED);
	}

	if (UC_SUCCEEDED == result || UC_JSON_PARSE_ONE_DATA_ERROR == result){
		bool ret = false;
		// 如果UCC返回数据解析失败，不做本地同步;
		if (UC_SUCCEEDED == result){
			ret = theStore.GetMsgStore()->SyncMessages(userId, SINGLE_CHAT, count, uistartTime, uiendTime, update_flag_msgid, msglist, (uiendTime == sdkendtime));
			if (!ret){
				result = UCAS_MESSAGE_SYNC_LOCAL_FAILED;
			}
		}
		// 本地同步成功或者UCC返回数据解析失败，把有效数据提供给UI;
		if ((ret || UC_JSON_PARSE_ONE_DATA_ERROR == result) || UCAS_MESSAGE_SYNC_LOCAL_FAILED == result){
			for (std::list<MessageStoreItem*>::iterator iter = msglist.begin(); iter != msglist.end(); ++iter)
			{
				MessageStoreItem* item = (*iter);
				if (item->state != STATE_REVOCATION){
					triggerEventEx(IUCEngineEvent, *_engine, OnMessageReceived(channel, *(item->msg), item->body, item->state));
				}	

				UCHead head;
				item->msg->GetHead(head);
				if (head.GetFrom().GetUserId() == GetCurUser().GetUserId() && item->state == STATE_SENDSUCCESS){
					seqList.push_back(head.GetSeq());
				}
			}
		}
	}

	int64_t min_timestamp = 0;
	if (!msglist.empty()){
		MessageStoreItem* item = msglist.back();
		assert(item);
		assert(item->msg);

		UCHead head;
		item->msg->GetHead(head);

		min_timestamp = head.GetSeq();
	}

	//如果Server单条数据解析失败，返回UI的消息个数为UI请求的值和Server返回的真实值之一，
	//否则使用从Server获取到的真实条数;
	//msgCount的值UI用来判断当前会话聊天历史是否获取完。
	if (UC_JSON_PARSE_ONE_DATA_ERROR == result){
		//当UI请求消息个数等于Server返回的消息个数，返回UI请求的值
		if (count == serverCount){
			msgCount = count;
		}
		else {
			//当SDK向Server请求时间不是0，则消息格式返回UI请求的值，并把请求时间作为最小时间点
			if (0 != sdkstarttime){
				msgCount = count;
				min_timestamp = sdkstarttime;
			}
			else{
				msgCount = serverCount;
			}
		}
	}else if (UC_SUCCEEDED == result){
		msgCount = msglist.size();
	}
	//如果Server单条数据解析失败，给UI的错误码默认为成功;
	if (UC_JSON_PARSE_ONE_DATA_ERROR == result || UCAS_MESSAGE_SYNC_LOCAL_FAILED == result){
		result = UC_SUCCEEDED;
	}

	triggerEvent(IIMServiceEvent, OnChatHistoryReceived(result, channel, userId, msgCount, min_timestamp));

	if (!seqList.empty()){
		GetUnreadUserCount(userId, seqList);
	}


	UC_LOG(INFO) << "im_service OnChatHistoryReceived from server. result: " << result
		<< " userid: " << userId << " message_count:" << msglist.size() << " min_timestamp: " << min_timestamp;

	theStore.GetMsgStore()->ClearMessageList(msglist);
	return result;
}

int32_t IMService::OnSendMessageResponse(int32_t errorCode, HTTPResponse *response, int32_t userId, int32_t msgId, bool saveUnread) {
	std::lock_guard<std::recursive_mutex> lock(_lock4UnReadMsg);

	UC_LOG(INFO) << "OnSendMessageResponse UserId:" << userId << " MsgId:" << msgId
			<< " errorCode:" << errorCode << " response:" << (response != NULL ? response->GetContent() : "");
	
	int32_t result = UC_FAILED;
	int64_t msgSeq = 0;
	int64_t sendTime = 0;

	if (UC_SUCCEEDED == errorCode && NULL != response && 200 == response->GetStatusCode()) {
		wstringstream wsStream;
		wsStream.str(StringConvert::FromUTF8Str(response->GetContent()).c_str());
		boost::property_tree::wptree jsonRoot;
		try{
			boost::property_tree::json_parser::read_json(wsStream, jsonRoot);
			result = jsonRoot.get<int32_t>(L"code");
			if (UC_SUCCEEDED == result) {
				boost::property_tree::wptree jsonChild = jsonRoot.get_child(L"success_data");
				BOOST_FOREACH(boost::property_tree::wptree::value_type v, jsonChild){
					result = v.second.get<int32_t>(L"code");
					msgId = v.second.get<int32_t>(L"msg_id");
					msgSeq = v.second.get<int64_t>(L"seq");
					sendTime = v.second.get<int64_t>(L"timestamp");
				}

				jsonChild = jsonRoot.get_child(L"failed_data");
				BOOST_FOREACH(boost::property_tree::wptree::value_type v, jsonChild){
					result = v.second.get<int32_t>(L"code");
					msgId = v.second.get<int32_t>(L"msg_id");
					msgSeq = v.second.get<int64_t>(L"seq");
					sendTime = v.second.get<int64_t>(L"timestamp");
				}
			}
		}catch (boost::property_tree::ptree_error & e) {
			UC_LOG(ERROR) << "encounter an exception when parse json from OnSendMessageResponse , exception: " << e.what();
			result = JSON_PARSE_ERROR;
		}
	} else {
		result = errorCode != UC_SUCCEEDED ? errorCode : (NULL != response && 200 != response->GetStatusCode() ? response->GetStatusCode() : UC_FAILED);
	}

	// 更新消息状态
	if (UC_SUCCEEDED == result) {
		theStore.GetMsgStore()->UpdateIMMessageState( userId, msgId, STATE_SENDSUCCESS, msgSeq );
	} else {
		theStore.GetMsgStore()->UpdateIMMessageState( userId, msgId, STATE_SENDFAIL, msgSeq );
	}

	triggerEvent(IIMServiceEvent, OnMessageSent(result, userId, msgId, msgSeq, sendTime));
	if (UC_SUCCEEDED == result && saveUnread){
		OnMessageUnreadUserNotify(userId, msgSeq);
	}

	return result;
}

int32_t IMService::OnSendInviteCallMessageResponse(int32_t errorCode, HTTPResponse *response, int32_t conferenceId, int32_t callType, int32_t userId, int32_t msgId, bool saveUnread, AudioService* service) {
    
#ifdef AUDIO_SERVICE
	int32_t result = OnSendMessageResponse(errorCode, response, userId, msgId, saveUnread);
	return service->OnInviteCallSend(result, userId, conferenceId, callType, SINGLE_CHAT);
#else
    return 0;
#endif
}

int32_t IMService::OnSendMessageReadResponse(int32_t errorCode, HTTPResponse *response, int32_t userId, int32_t readType, int32_t msgId, int64_t msgSeq) {

	UC_LOG(INFO) << "OnSendMessageReadResponse UserId:" << userId << "ReadType:" << readType <<
		"MsgId:" << msgId << "MsgSeq:" << msgSeq << "errorCode" << errorCode << "response:" << (response != NULL ? response->GetContent() : "");

	int32_t result = UC_FAILED;
	if (UC_SUCCEEDED  ==  errorCode && NULL != response && 200 == response->GetStatusCode()) {
		wstringstream wsStream;
		wsStream.str(StringConvert::FromUTF8Str(response->GetContent()).c_str());
		boost::property_tree::wptree jsonRoot;
		try {
			boost::property_tree::json_parser::read_json(wsStream, jsonRoot);
			result = jsonRoot.get<int32_t>(L"code");
		} catch (boost::property_tree::ptree_error & e) {
			UC_LOG(ERROR) << "encounter an exception when parse json from OnSendMessageReadResponse , exception: " << e.what();
			result = JSON_PARSE_ERROR;
		}		
	} else {
		result = errorCode != UC_SUCCEEDED ? errorCode : (NULL != response && 200 != response->GetStatusCode() ? response->GetStatusCode() : UC_FAILED);
	}
	
	triggerEvent(IIMServiceEvent, OnMessageRead(result, userId, readType, msgId, msgSeq));
	return result;
}

int32_t IMService::OnSendMessagePlayedResponse(int32_t errorCode, HTTPResponse *response, int32_t userId, int32_t msgId, int64_t msgSeq) {

	UC_LOG(INFO) << "OnSendMessageReadResponse UserId:" << userId << "MsgId:" << msgId <<
		"MsgSeq:" << msgSeq << "errorCode:" << errorCode << "response:" << (response != NULL ? response->GetContent() : "");

	int32_t result = UC_FAILED;
	if (UC_SUCCEEDED  ==  errorCode && NULL != response && 200 == response->GetStatusCode()) {
		wstringstream wsStream;
		wsStream.str(StringConvert::FromUTF8Str(response->GetContent()).c_str());
		boost::property_tree::wptree jsonRoot;
		try {
			boost::property_tree::json_parser::read_json(wsStream, jsonRoot);
			result = jsonRoot.get<int32_t>(L"code");
		} catch (boost::property_tree::ptree_error & e) {
			UC_LOG(ERROR) << "encounter an exception when parse json from OnSendMessagePlayedResponse , exception: " << e.what();
			result = JSON_PARSE_ERROR;
		}	
	}else{
		result = errorCode != UC_SUCCEEDED ? errorCode : (NULL != response && 200 != response->GetStatusCode() ? response->GetStatusCode() : UC_FAILED);
	}
	
	theStore.GetMsgStore()->UpdateIMMessageState( userId, msgId, MESSAGE_PLAYED, msgSeq );
	triggerEvent(IIMServiceEvent, OnMessagePlayed(result, userId, msgId, msgSeq));
	return result;
}

int32_t IMService::OnGetUnreadUserCountResponse(int32_t errorCode, HTTPResponse *response, int32_t userId, std::list<int64_t> seq){

	UC_LOG(INFO) << "OnGetUnreadUserCountResponse  userId: " << userId << " errorCode:"
		<< errorCode << " response:" << (response != NULL ? response->GetContent() : "");

	int32_t result = UC_FAILED;
	std::list<MessageReadInfo> infolist;
	if (UC_SUCCEEDED == errorCode && NULL != response && 200 == response->GetStatusCode()) {
		wstringstream wsStream;
		wsStream.str(StringConvert::FromUTF8Str(response->GetContent()).c_str());
		boost::property_tree::wptree jsonRoot;
		try{
			boost::property_tree::json_parser::read_json(wsStream, jsonRoot);
			result = jsonRoot.get<int32_t>(L"code");
			if (UC_SUCCEEDED == result){
				boost::property_tree::wptree jsonChild = jsonRoot.get_child(L"data");
				BOOST_FOREACH(boost::property_tree::wptree::value_type v, jsonChild)
				{
					MessageReadUserInfo info;
					info.SetConversationType(SINGLE_CHAT);
					info.SetRelateId(userId);
					info.SetMsgSeq(StringConvert::WStringToInt64(v.first));
					info.SetUnreadCount(StringConvert::WStringToInt(v.second.get_value(v.first)));
					if (MESSAGE_UNREAD_NONE == info.GetUnreadCount()){
						info.SetAllRead(true);
					}
					infolist.push_back(info);
					theStore.GetMessageReadStore()->SaveMessageReadInfo(ConvKey(SINGLE_CHAT, userId), info);
				}
			}
		}
		catch (boost::property_tree::ptree_error & e) {
			UC_LOG(ERROR) << "encounter an exception when parse json from OnGetUnreadUserCountResponse , exception: " << e.what();
			result = JSON_PARSE_ERROR;
		}
	}
	triggerEvent(IIMServiceEvent, OnMessageUnreadUserCount(result, userId, infolist));

	return result;
}
/**
 * 从ucas过来的消息只保存在memory中,database中不存储
 * history返回的消息将存储在memory和database中。
 */
int32_t IMService::OnMessageReceived(MessageChannel channel, UCMsg &msg, const string &body, MsgSendState state){
	
	UCHead head;
	msg.GetHead(head);
	bool isSave = false;
	bool saveUnread = false;

	switch(msg.GetMsgType()) {
	case UCNormalChatText: {
		OnTextMessageNotify(channel, head, msg, state, isSave, saveUnread);
		break;
	}
	case UCNormalChatMedia: {
		OnMediaMessageNotify(channel, head, msg, state, isSave, saveUnread);
		break;
	}
	case UCNormalChatReprot: {
		OnReportMessageNotify(channel, head, msg, state, isSave);
		break;
	}
	case UCNormalChatDocumentSend: {
		OnFileMessageNotify(channel, 0, head, msg, state, isSave, saveUnread);
		break;
	}
	case UCNormalChatDocumentForward: {
		OnFileMessageNotify(channel, 1, head, msg, state, isSave, saveUnread);
		break;
	}
	case UCNormalChatDocumentDel: {
		OnDeleteFileNotify(channel, head, msg, state,isSave);
		break;
	}
	case UCNormalChatCloudFileCreate: {
		OnCloudFileCreateNotify(channel, head, msg, state, isSave);
		break;
	}
	case UCNormalChatCodeCreate: {
		OnCodeMessageNotify(channel, 0, head, msg, state, isSave, saveUnread);
		break;
	 }
	case UCNormalChatCodeForward: {
		OnCodeMessageNotify(channel, 1, head, msg, state, isSave, saveUnread);
		break;
	}
	case UCAudioChatInvite: {
		OnInviteCallNotify(channel, head, msg, state, isSave);
		break;
	}
	case UCAudioChatReceive: {
		OnAcceptCallNotify(channel, head, msg, state, isSave);
		break;
	}
	case UCAudioChatReject: {
		OnRejectCallNofify(channel, head, msg, state, isSave);
		break;
	}
	case UCAudioChatStop: {
		OnFinishCallNotify(channel, head, msg, state,isSave);
		break;
	}
	case UCAudioChatCancelCall: {
		OnCancelCallNotify(channel, head, msg, state,isSave);
		break;
	}
	case UCAudioChatBlockCall: {
		OnNotRespondCallNotify(channel, head, msg, state, isSave);
		break;
	}
	case UCAudioChatRecentCall: {
		OnConnectedCallNotify(channel, head, msg, state, isSave);
		break;
	}
	case UCAckMessageRead:{
		OnMessageReadNotify(channel, head, msg, state, isSave);
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
	default:
		break;
	}

	if (UCAS == channel){
		int64_t relate_id = MessageProcessor::GetRelateId(SINGLE_CHAT, GetCurUser().GetUserId(), head.GetFrom().GetUserId(), head.GetTo().GetUserId());
		if (isSave)
		{
			MessageProcessor msgProcessor((UCEngine*)_engine);
			if (msgProcessor.SaveMessage(SINGLE_CHAT, relate_id, 0, 0, STATE_SENDSUCCESS, &msg, body) != UC_SUCCEEDED)
			{
				UC_LOG(ERROR) << "IMService::OnMessageReceived save message failed." << " msgType = " << msg.GetMsgType();
			}

			if (saveUnread && (head.GetFrom().GetUserId() == GetCurUser().GetUserId())){
				OnMessageUnreadUserNotify(relate_id, head.GetSeq());
			}
		}
		//增加规避方案：在没有收到ackread消息时，通过对方发送的实时消息处理小于该实时消息seq的未读计数;
		if (head.GetFrom().GetUserId() != GetCurUser().GetUserId()){
			std::list<MessageReadInfo> infoList;
			theStore.GetMessageReadStore()->UpdateMessageReadInfo(ConvKey(SINGLE_CHAT, relate_id), head.GetSeq(), infoList);
			if (!infoList.empty()){
				triggerEvent(IIMServiceEvent, OnMessageUnreadUserCount(UC_SUCCEEDED, relate_id, infoList))
			}
		}
	}

	return UC_SUCCEEDED;
}

void IMService::OnTextMessageNotify(MessageChannel channel, UCHead head, UCMsg& ucMsg, MsgSendState state, bool& isSave, bool& saveUnread){

	isSave = false;
	if (UCAS == channel && IsCurUser(head.GetFrom())) {
		return;
	}

	if (UCAS == channel)
	{
		head.SetId(DBManager::GetInstance().GetMessageID());
		ucMsg.SetHead(head);
	}
	
	TextContentMsg *msg = (TextContentMsg*)&ucMsg;
	if (NULL == msg){
		UC_LOG(ERROR) << "IMService::OnTextNofity msg is null";
		return ;
	}

	isSave = true;
	saveUnread = true;
	TextMessageInfo messageInfo;
	
	messageInfo.InfoFromUCHead(head);
	messageInfo.SetTextType(msg->get_type());
	messageInfo.SetTextContent(msg->get_text());
	messageInfo.SetTextFormat(msg->get_text_style());
	messageInfo.SetSendState(state);

	UC_LOG(INFO) << "IM OnTextMessageNotify. channel: " << channel << " from_userid: " << head.GetFrom().GetUserId() << 
		" to_userid: " << head.GetTo().GetUserId() << " text: " << messageInfo.GetTextContent();

	triggerEvent(IIMServiceEvent, OnTextMessageReceived(channel, &messageInfo));
}

void IMService::OnMediaMessageNotify(MessageChannel channel, UCHead head, UCMsg& ucMsg, MsgSendState state, bool& isSave, bool& saveUnread){
	
	isSave = false;
	if (UCAS == channel && IsCurUser(head.GetFrom())) {
		return;
	}

	if (UCAS == channel)
	{
		head.SetId(DBManager::GetInstance().GetMessageID());
		ucMsg.SetHead(head);
	}

	MediaContentMsg *msg = (MediaContentMsg*)&ucMsg;
	if (NULL == msg){
		UC_LOG(ERROR) << "IMService::OnMediaNotify msg is null";
		return ;
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
		triggerEvent(IIMServiceEvent, OnImageMessageReceived(channel, &messageInfo));

	} else if (ChatMediaType::MediaTypeAudio ==  msg->get_media_type()) {
		messageInfo.SetDuration(msg->get_media_duration());
		triggerEvent(IIMServiceEvent, OnAudioMessageReceived(channel, &messageInfo));

	} else if (ChatMediaType::MediaTypeVideo ==  msg->get_media_type()) {
		messageInfo.SetThumbnails(msg->get_media_thumb());
		messageInfo.SetDuration(msg->get_media_duration());
		triggerEvent(IIMServiceEvent, OnVideoMessageReceived(channel, &messageInfo));
	} else if (ChatMediaType::MediaTypeMeetingRecord == msg->get_media_type()) {
		messageInfo.SetThumbnails(msg->get_media_thumb());
		messageInfo.SetDuration(msg->get_media_duration());
		messageInfo.SetOperatorId(msg->get_operatorId());
		messageInfo.SetOperatorName(msg->get_operatorName());
		messageInfo.SetOperatorAvatar(msg->get_operatorAvatar());
		messageInfo.SetThumbnailsURL(msg->get_media_thumb_url());
		triggerEvent(IIMServiceEvent, OnRecordVideoReceived(channel, &messageInfo));
	}

	UC_LOG(INFO) << "IM OnImageMessageNotify. channel: " << channel << " from_userid: " << head.GetFrom().GetUserId() <<
		" to_userid: " << head.GetTo().GetUserId() << " media_type: " << msg->get_media_type();
}

void IMService::OnReportMessageNotify(MessageChannel channel, UCHead head, UCMsg& ucMsg, MsgSendState state, bool& isSave){

	isSave = false;
	if (UCAS == channel && IsCurUser(head.GetFrom())) {
		return;
	}

	if (UCAS == channel)
	{
		head.SetId(DBManager::GetInstance().GetMessageID());
		ucMsg.SetHead(head);
	}

	ReportContentMsg *msg = (ReportContentMsg*)&ucMsg;
	if (NULL == msg){
		UC_LOG(ERROR) << "IMService::OnReportMessageNotify msg is null";
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

	UC_LOG(INFO) << "IM OnReportMessageNotify. channel: " << channel << " from_userid: " << head.GetFrom().GetUserId() <<
		" to_userid: " << head.GetTo().GetUserId();

	triggerEvent(IIMServiceEvent, OnReportMessageReceived(channel, &messageInfo));
}

void IMService::OnFileMessageNotify(MessageChannel channel, int32_t operation, UCHead head, UCMsg& ucMsg, 
	MsgSendState state, bool& isSave, bool& saveUnread){
	
	isSave = false;
	if (UCAS == channel && IsCurUser(head.GetFrom())) {
		return;
	}

	if (UCAS == channel)
	{
		head.SetId(DBManager::GetInstance().GetMessageID());
		ucMsg.SetHead(head);
	}

	DocumentContentMsg *msg = (DocumentContentMsg*)&ucMsg;
	if (NULL == msg){
		UC_LOG(ERROR) << "IMService::OnSendFileNotify msg is null";
		return ;
	}

	isSave = true;
	saveUnread = true;
	FileMessageInfo messageInfo;

	messageInfo.InfoFromUCHead(head);
	messageInfo.SetFileName(msg->get_doc_name());
	messageInfo.SetFileSize(msg->get_size());
	messageInfo.SetFileUrl(msg->get_down_url());
	messageInfo.SetSendState(state);

	// 从db中查找是否有本地文件
	if (channel == HISTORY || channel == LIST || channel == CHAT || channel == SEARCH) {
		//检测文件是否被下载过
		std::string localPath{ "" };
		std::string fileurl{ "" };
		int64_t fileSize{ 0 };

		fileurl = MessageProcessor::RetrieveNoUserDownloadURL(msg->get_down_url());
		if (UC_SUCCEEDED == MessageProcessor::_GetLocalFilePathAndSize(fileurl, localPath, fileSize) &&
			UC_SUCCEEDED == MessageProcessor::CheckFile(localPath, fileSize)){
			messageInfo.SetFilePath(localPath);
		}
		else
			messageInfo.SetFilePath("");
	}

	triggerEvent(IIMServiceEvent, OnFileMessageReceived(channel, operation, &messageInfo));

	UC_LOG(INFO) << "im_service. OnFileMessageNotify. channel: " << channel << " filename: " << messageInfo.GetFileName()
		<< " filesize: " << messageInfo.GetFileSize() << " fileURL: " << messageInfo.GetFileUrl();
}

void IMService::OnDeleteFileNotify(MessageChannel channel, UCHead head, UCMsg& ucMsg, MsgSendState state, bool& isSave){

	isSave = false;
	if (UCAS == channel && IsCurUser(head.GetFrom())) {
		return;
	}

	//一对一文件删除消息不存储,不需要设置本地msgId;
// 	if (UCAS == channel)
// 	{
// 		head.SetId(DBManager::GetInstance().GetMessageID());
// 		ucMsg.SetHead(head);
// 	}
	
	DocumentDelContentMsg *msg = (DocumentDelContentMsg*)&ucMsg;
	if (NULL == msg){
		UC_LOG(ERROR) << "IMService::OnShareFileNotify msg is null";
		return ;
	}
	triggerEvent(IIMServiceEvent, OnFileDeleted(head.GetFrom(), head.GetId(), head.GetSeq(), msg->get_doc_name(),msg->get_contentId()));
}

void IMService::OnCloudFileCreateNotify(MessageChannel channel, UCHead head, UCMsg& ucMsg, MsgSendState state, bool& isSave) {
	
	isSave = false;
	if (UCAS == channel && IsCurUser(head.GetFrom())) {
		return;
	}

	if (UCAS == channel)
	{
		head.SetId(DBManager::GetInstance().GetMessageID());
		ucMsg.SetHead(head);
	}

	CloudFileContentMsg *msg = (CloudFileContentMsg*)&ucMsg;
	if (NULL == msg){
		UC_LOG(ERROR) << "IMService::OnCloudFileCreateNotify msg is null";
		return;
	}

	isSave = true;
	FileMessageInfo messageInfo;

	messageInfo.InfoFromUCHead(head);
	messageInfo.SetFileName(msg->get_fileName());
	messageInfo.SetFileSize(msg->get_size());
	messageInfo.SetCloudType(DocumentCloudType(msg->get_cloudType()));
	messageInfo.SetDetailContent(msg->get_detailContent());
	messageInfo.SetSendState(state);
	messageInfo.SetFileId(msg->get_contentId());

	triggerEvent(IIMServiceEvent, OnCloudFileCreateMessageReceived(channel, messageInfo));

	UC_LOG(INFO) << "im_service. OnCloudFileCreateNotify. filename: " << messageInfo.GetFileName()
		<< " filesize: " << messageInfo.GetFileSize() << " cloudType: " << messageInfo.GetCloudType();
}

void IMService::OnCodeMessageNotify(MessageChannel channel, int32_t operation, UCHead head, UCMsg& ucMsg, 
	MsgSendState state, bool& isSave, bool& saveUnread){

	isSave = false;
	if (UCAS == channel && IsCurUser(head.GetFrom())) {
		return;
	}

	if (UCAS == channel){
		head.SetId(DBManager::GetInstance().GetMessageID());
		ucMsg.SetHead(head);
	}

	CodeCreateContentMsg *msg = (CodeCreateContentMsg*)&ucMsg;
	if (NULL == msg){
		isSave = false;
		UC_LOG(ERROR) << "IMService::OnSendCodeNotify msg is null";
		return ;
	}

	isSave = true;
	saveUnread = true;
	CodeMessageInfo messageInfo;
	messageInfo.InfoFromUCHead(head);
	messageInfo.SetCodeId(msg->get_contentId());
	messageInfo.SetCodeTile(msg->get_title());
	messageInfo.SetLandType(msg->get_langType());
	messageInfo.SetCodeHead(msg->get_contentHead());
	messageInfo.SetCodeUrl(msg->get_downUrl());
	messageInfo.SetCodeDesc(msg->get_desc());
	messageInfo.SetCodeSize(msg->get_size());
	messageInfo.SetCodeLine(msg->get_totalLines());
	messageInfo.SetCreateTime(msg->get_createTime());
	messageInfo.SetCreatorId(msg->get_operatorid());
	messageInfo.SetCreatorName(msg->get_operatorName());
	messageInfo.SetSendState(state);

	// 从db中查找是否有本地文件
	if (channel == HISTORY || channel == LIST || channel == CHAT || channel == SEARCH) {
		std::string localPath = "";
		DBManager::GetInstance().GetFSLocalPath(messageInfo.GetCodeUrl(), localPath);
		messageInfo.SetCodePath(localPath);
	}
	triggerEvent(IIMServiceEvent, OnCodeMessageReceived(channel, operation, messageInfo));

	UC_LOG(INFO) << "im_service. OnCodeMessageNotify. channel: " << channel << " codetitle: " << messageInfo.GetCodeTile()
		<< " codedesc: " << messageInfo.GetCodeDesc() << " codeURL: " << messageInfo.GetCodeUrl()
		<< " codesize: " << messageInfo.GetCodeSize() << " codeline: " << messageInfo.GetCodeLine();
}

void IMService::OnMessageReadNotify(MessageChannel channel, UCHead head, UCMsg& ucMsg, MsgSendState state, bool& isSave){
	std::lock_guard<std::recursive_mutex> lock(_lock4UnReadMsg);
	isSave = false;
	if (UCAS == channel && IsCurUser(head.GetFrom())) {
		return;
	}

	AckReadMsg *msg = (AckReadMsg*)&ucMsg;
	if (NULL == msg){
		UC_LOG(ERROR) << "IMService::OnMessageReadNotify msg is null";
		return;
	}

	if (SessionType::SingleChat != msg->get_sessionType()){
		return;
	}
	
	if (head.GetFrom().GetUserId() == GetCurUser().GetUserId()){
		triggerEvent(IIMServiceEvent, OnMessageRead(UC_SUCCEEDED, head.GetTo().GetUserId(), 0, msg->get_id(), msg->get_seq()));
	}
	
	if (head.GetFrom().GetUserId() != GetCurUser().GetUserId()){

		UC_LOG(INFO) << "IMService::OnMessageReadNotify userId: " << head.GetTo().GetUserId() << " acked userId : " << msg->get_ackedUser().userID
			<< " read message seq :" << msg->get_seq();

		theStore.GetMessageReadStore()->UpdateMessageReadInfo(ConvKey(SINGLE_CHAT, head.GetTo().GetUserId()), msg->get_seq(), msg->get_ackedUser().userID);
		MessageReadInfo info;
		theStore.GetMessageReadStore()->GetMessageReadList(ConvKey(SINGLE_CHAT, head.GetTo().GetUserId()), msg->get_seq(), info);
		std::list<MessageReadInfo> list;
		list.push_back(info);
		triggerEvent(IIMServiceEvent, OnMessageUnreadUserCount(UC_SUCCEEDED, msg->get_ackedUser().userID, list));

	}
}

void IMService::OnMessageBatchReadNotify(MessageChannel channel, UCHead head, UCMsg& ucMsg, MsgSendState state, bool& isSave){
	isSave = false;
	if (UCAS == channel && IsCurUser(head.GetFrom())) {
		return;
	}

	BatchAckReadMsg *msg = (BatchAckReadMsg*)&ucMsg;
	if (NULL == msg){
		UC_LOG(ERROR) << "IMService::OnMessageReadNotify msg is null";
		return;
	}
	if (SessionType::SingleChat != msg->get_sessionType()){
		return;
	}

	if (msg->get_ackedUser().userID == GetCurUser().GetUserId()){
		triggerEvent(IIMServiceEvent, OnMessageRead(UC_SUCCEEDED, head.GetTo().GetUserId(), 1, 0, 0));
	}

	if (msg->get_ackedUser().userID != GetCurUser().GetUserId()){

		UC_LOG(INFO) << "IMService::OnMessageBatchReadNotify  acked userId: " << msg->get_ackedUser().userID
			<< " message send time :" << head.GetTimestamp() << " seq list size:" << msg->get_seqList().size();

		for (auto seq : msg->get_seqList()){
			theStore.GetMessageReadStore()->UpdateMessageReadInfo(ConvKey(SINGLE_CHAT, msg->get_ackedUser().userID), seq, msg->get_ackedUser().userID);
		}
		std::list<MessageReadInfo> list;
		theStore.GetMessageReadStore()->GetMessageReadList(ConvKey(SINGLE_CHAT, msg->get_ackedUser().userID), msg->get_seqList(), list);
		triggerEvent(IIMServiceEvent, OnMessageUnreadUserCount(UC_SUCCEEDED, msg->get_ackedUser().userID, list));
	}
}

void IMService::OnMessagePlayedNotify(MessageChannel channel, UCHead head, UCMsg& ucMsg, MsgSendState state, bool& isSave){

	isSave = false;
	if (UCAS == channel && IsCurUser(head.GetFrom())) {
		return;
	}

	AckPlayMsg *msg = (AckPlayMsg*)&ucMsg;
	if (NULL == msg){
		UC_LOG(ERROR) << "IMService::OnMessagePlayedNotify msg is null";
		return;
	}
	triggerEvent(IIMServiceEvent, OnMessagePlayed(UC_SUCCEEDED, head.GetTo().GetUserId(), msg->get_id(), msg->get_seq()));
	if (head.GetFrom().GetUserId() == GetCurUser().GetUserId()){
		int conversation_type = MessageProcessor::GetConversationType( head.GetAppId(), head.GetProtocoltype() );
		theStore.GetMsgStore()->SetMessageStatusBySeq(head.GetTo().GetUserId(), head.GetSeq(), conversation_type, 1);
	}
}

void IMService::OnInviteCallNotify(MessageChannel channel, UCHead head, UCMsg& ucMsg, MsgSendState state, bool& isSave){

	isSave = false;
	if (UCAS == channel && IsCurUser(head.GetFrom())) {
		return;
	}

	head.SetId(DBManager::GetInstance().GetMessageID());
	ucMsg.SetHead(head);

	AudioInviteContentMsg *msg = (AudioInviteContentMsg*)&ucMsg;
	if (NULL == msg) {
		UC_LOG(ERROR) << "IMService::OnInviteCallNotify msg is null";
		return;
	}
	isSave = true;

	triggerEvent(IIMServiceEvent, OnCallInvited(channel, UC_SUCCEEDED, head.GetFrom().GetUserId(), MessageProcessor::GetRelateId(SINGLE_CHAT, GetCurUser().GetUserId(), head.GetFrom().GetUserId(), head.GetTo().GetUserId())
		, msg->get_confId(), msg->get_confPwd(), msg->get_hostId(), head.GetId(), head.GetSeq(), head.GetTimestamp()));
}

void IMService::OnAcceptCallNotify(MessageChannel channel, UCHead head, UCMsg& ucMsg, MsgSendState state,  bool& isSave){

	isSave = false;
	if (UCAS == channel && IsCurUser(head.GetFrom())) {
		return;
	}

	AudioReceiveContentMsg *msg = (AudioReceiveContentMsg*)&ucMsg;
	if (NULL == msg){
		UC_LOG(ERROR) << "IMService::OnAcceptCallNotyfy msg is null";
		return;
	}
	isSave = true;
	triggerEvent(IIMServiceEvent, OnCallAccepted(channel, UC_SUCCEEDED, head.GetFrom().GetUserId(), MessageProcessor::GetRelateId(SINGLE_CHAT, GetCurUser().GetUserId(), head.GetFrom().GetUserId(), head.GetTo().GetUserId())
		, msg->get_confId(), head.GetId(), head.GetSeq(), head.GetTimestamp()));
}

void IMService::OnRejectCallNofify(MessageChannel channel, UCHead head, UCMsg& ucMsg, MsgSendState state, bool& isSave){
	UC_LOG(INFO) << "OnRejectCallNofify, channel:" << channel
		<< ", from:" << head.GetFrom().GetUserId() << ", cur:" << GetCurUser().GetUserId();
	isSave = false;
	if (UCAS == channel && IsCurUser(head.GetFrom())) {
		UC_LOG(INFO) << "IsCurUser";
		return;
	}

	if (UCAS == channel)
	{
		head.SetId(DBManager::GetInstance().GetMessageID());
		ucMsg.SetHead(head);
	}
	
	AudioRejectContentMsg *msg = (AudioRejectContentMsg*)&ucMsg;
	if (NULL == msg){
		UC_LOG(ERROR) << "msg is null";
		return;
	}

	triggerEvent(IIMServiceEvent, OnCallRejected(channel, UC_SUCCEEDED, head.GetFrom().GetUserId(), MessageProcessor::GetRelateId(SINGLE_CHAT, GetCurUser().GetUserId(), head.GetFrom().GetUserId(), head.GetTo().GetUserId()),
		head.GetId(), head.GetSeq(), head.GetTimestamp(),	msg->get_confId(), msg->get_hostId(), msg->get_reason()));

}

void IMService::OnFinishCallNotify(MessageChannel channel, UCHead head, UCMsg& ucMsg, MsgSendState state, bool& isSave){

	isSave = false;
	if (UCAS == channel && IsCurUser(head.GetFrom())) {
		return;
	}

	AudioStopContentMsg *msg = (AudioStopContentMsg*)&ucMsg;
	if (NULL == msg){
		UC_LOG(ERROR) << "IMService::OnFinishCallNotify msg is null";
		return;
	}
	isSave = true;
	triggerEvent(IIMServiceEvent, OnCallStopped(channel, UC_SUCCEEDED, head.GetFrom().GetUserId(), MessageProcessor::GetRelateId(SINGLE_CHAT, GetCurUser().GetUserId(), head.GetFrom().GetUserId(), head.GetTo().GetUserId()),
		msg->get_confId(),	head.GetId(), head.GetSeq(), head.GetTimestamp()));
}

void IMService::OnCancelCallNotify(MessageChannel channel, UCHead head, UCMsg& ucMsg, MsgSendState state,bool& isSave){
	
	isSave = false;
	if (UCAS == channel && IsCurUser(head.GetFrom())) {
		return;
	}

	if (UCAS == channel)
	{
		head.SetId(DBManager::GetInstance().GetMessageID());
		ucMsg.SetHead(head);
	}
	
	AudioCancelCallContentMsg *msg = (AudioCancelCallContentMsg*)&ucMsg;
	if (NULL == msg){
		UC_LOG(ERROR) << "IMService::OnCancelCallNotify msg is null";
		return;
	}

	isSave = true;
	triggerEvent(IIMServiceEvent, OnCallCanceled(channel, UC_SUCCEEDED, head.GetFrom().GetUserId(), MessageProcessor::GetRelateId(SINGLE_CHAT, GetCurUser().GetUserId(), head.GetFrom().GetUserId(), head.GetTo().GetUserId()),
		head.GetId(), head.GetSeq(), head.GetTimestamp(), msg->get_phoneNum()));
}

void IMService::OnNotRespondCallNotify(MessageChannel channel, UCHead head, UCMsg& ucMsg, MsgSendState state, bool& isSave){

	isSave = false;
	if (UCAS == channel && IsCurUser(head.GetFrom())) {
		return;
	}

	if (UCAS == channel)
	{
		head.SetId(DBManager::GetInstance().GetMessageID());
		ucMsg.SetHead(head);
	}
	
	AudioBlockCallContentMsg *msg = (AudioBlockCallContentMsg*)&ucMsg;
	if (NULL == msg){
		UC_LOG(ERROR) << "IMService::OnCancelCallNotify msg is null";
		return;
	}

	isSave = true;
	triggerEvent(IIMServiceEvent, OnCallNotResponded(channel, UC_SUCCEEDED, head.GetFrom().GetUserId(), MessageProcessor::GetRelateId(SINGLE_CHAT, GetCurUser().GetUserId(), head.GetFrom().GetUserId(), head.GetTo().GetUserId()),
		head.GetId(), head.GetSeq(), head.GetTimestamp()));

}

void IMService::OnConnectedCallNotify(MessageChannel channel, UCHead head, UCMsg& ucMsg, MsgSendState state,bool& isSave){

	isSave = false;
	if (UCAS == channel && IsCurUser(head.GetFrom())) {
		return;
	}

	if (UCAS == channel)
	{
		head.SetId(DBManager::GetInstance().GetMessageID());
		ucMsg.SetHead(head);
	}

	AudioRecentCallContentMsg *msg = (AudioRecentCallContentMsg*)&ucMsg;
	if (NULL == msg){
		UC_LOG(ERROR) << "IMService::OnCancelCallNotify msg is null";
		return;
	}

	isSave = true;
	triggerEvent(IIMServiceEvent, OnCallConnected(channel, UC_SUCCEEDED, head.GetFrom().GetUserId(), MessageProcessor::GetRelateId(SINGLE_CHAT, GetCurUser().GetUserId(), head.GetFrom().GetUserId(), head.GetTo().GetUserId()),
		head.GetId(), head.GetSeq(), head.GetTimestamp(), msg->get_duration(), msg->get_phoneNum()));

}

void IMService::OnMessageUnreadUserNotify(int32_t userId, int64_t msgSeq){
	
	std::list<MessageReadInfo> list;
	MessageReadUserInfo info;
	info.SetMsgSeq(msgSeq);
	info.SetConversationType(SINGLE_CHAT);
	info.SetRelateId(userId);
	info.SetUnreadCount(MESSAGE_UNREAD_ONE);
	info.AddUser(userId);
	list.push_back(info);
	theStore.GetMessageReadStore()->SaveMessageReadInfo(ConvKey(SINGLE_CHAT, userId), info);

	triggerEvent(IIMServiceEvent, OnMessageUnreadUserCount(UC_SUCCEEDED, (int32_t)userId, list));
}

} /* namespace uc */
