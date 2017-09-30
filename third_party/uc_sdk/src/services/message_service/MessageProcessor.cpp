/*
 * MessageProcessor.cpp
 *
 *  Created on: June 9, 2015
 *      Author: yongshuai.cheng
 */

#include "MessageProcessor.h"
#include <sstream>
#include "MessageParser.h"
#include "uc_types.h"
#include "common/platform.h"
#include "common/ZipWrapper.h"
#include "LogManager.h"
#include "client_service/ClientService.h"
#include "UCParserErrorCode.h"
#include "CommonDef.h"
#include "UCTime.h"
#include "IMService.h"
#include "DBManager.h"
#include "boost/property_tree/json_parser.hpp"
#include "store_service/StoreService.h"
#include "filetransfer_service/FileHelp.hpp"

namespace uc {

MessageProcessor::MessageProcessor(UCEngine *engine) {
	_engine = engine;
}

MessageProcessor::~MessageProcessor() {

}

// int32_t  MessageProcessor::CreateResendMessage(MessageInfo messageInfo, HTTPRequest &request){
// 	
// 	int32_t result = 0;
// 	char *msgData = NULL;
// 	int32_t dataLen = 0;
// 	UCHead head;
// 	head.SetAppId(messageInfo.GetAppId());
// 	head.SetVersion(messageInfo.GetVersion());
// 	head.SetProtocolId(messageInfo.GetProtocolId());
// 	head.SetProtocoltype(messageInfo.GetProtocolType());
// 	head.SetFrom(messageInfo.GetFrom());
// 	head.SetTo(messageInfo.GetTo());
// 	head.SetSeq(0);
// 	head.SetConversation(0);
// 	head.SetTimestamp(0);
// 	if (SINGLE_CHAT == messageInfo.GetConversationType()){
// 		head.SetPri(PriType::thrift_type);
// 		head.SetChannelPri(ChannelPriType::channel0);
// 	}else if(CONFERENCE_CHAT == messageInfo.GetConversationType()){
// 		head.SetPri(PriType::thrift_type|PriType::group_type);
// 		head.SetChannelPri(ChannelPriType::channel1);
// 	}else{
// 		head.SetPri(PriType::thrift_type|PriType::group_type);
// 		head.SetChannelPri(ChannelPriType::channel0);
// 	}
// 	head.SetControlPri(ControlPriType::unread_count_type | ControlPriType::display_type
// 		| ControlPriType::webpush_send_type);
// 
// 	
// // 	if(!StringConvert::Base64Decode(messageInfo.GetMsgData().c_str(), messageInfo.GetMsgData().length(), msgData, dataLen)){
// // 		UC_LOG(ERROR) << "Base64Decode error.";
// // 		return UC_FAILED;
// // 	}
// 	dataLen = messageInfo.GetMsgData().length();
// 	msgData = StringConvert::Base64Decode(messageInfo.GetMsgData().c_str(), dataLen);
// 	if (msgData == NULL) {
// 		UC_LOG(ERROR) << "Base64Decode error.";
// 		return UC_FAILED;
// 	}
// 
// 	BinMsg destMsg;
// 	result = MsgParser::EncodeMessage(head, msgData, dataLen, &destMsg);
// 	if (ParserError::PARSER_SUCCESS != result){
// 		delete []msgData;
// 		msgData = NULL;
// 		UC_LOG(ERROR) << "msgparser encode message error: " << result;
// 		return UC_FAILED;
// 	}
// 	
// 	delete []msgData;
// 	msgData = NULL;
// 	return CreateMessageRequest(destMsg, request);
// }

int32_t MessageProcessor::CreateForwardMessage(ConversationType type, UCID fromUid, UCID toUid, MessageStoreItem &msgItem, OUT UCMsg *&msg){

	int32_t result = 0;
	char *msgData = NULL;
	int32_t dataLen = 0;
	UCHead oldMsgHead;
	msgItem.msg->GetHead(oldMsgHead);

	UCHead head;
	head.SetVersion(oldMsgHead.GetVersion());
	head.SetFrom(fromUid);
	head.SetTo(toUid);
	head.SetSeq(0);
	head.SetConversation(0);
	head.SetTimestamp(0);
	head.SetControlPri(ControlPriType::unread_count_type | ControlPriType::display_type
		| ControlPriType::webpush_send_type);
	if (SINGLE_CHAT == type){
		head.SetPri(PriType::thrift_type);
		head.SetChannelPri(ChannelPriType::channel0);
	}else if(PROJECT_CHAT == type){
		head.SetPri(PriType::thrift_type|PriType::group_type);
		head.SetChannelPri(ChannelPriType::channel0);
	}else if(GROUP_CHAT == type){
		head.SetPri(PriType::thrift_type|PriType::group_type);
		head.SetChannelPri(ChannelPriType::channel0);
	}else if(CONFERENCE_CHAT == type){
		head.SetPri(PriType::thrift_type|PriType::group_type);
		head.SetChannelPri(ChannelPriType::channel1);
	}
	
	if (AppId::AppChat == oldMsgHead.GetAppId() && 
		ChatMessageType::NormalChat == oldMsgHead.GetProtocoltype()){
		
	}else if (AppId::AppChat == oldMsgHead.GetAppId() && 
		ChatMessageType::DiscussionChat == oldMsgHead.GetProtocoltype()){

	}else if (AppId::AppChat == oldMsgHead.GetAppId() && 
		ChatMessageType::GroupChat == oldMsgHead.GetProtocoltype()){
		
	}else if (AppId::AppMeeting == oldMsgHead.GetAppId() && 
		ConfMessageType::ConfChatMsg == oldMsgHead.GetProtocoltype()){
		
	}
	
	return head.GetId();
}

int32_t MessageProcessor::CreateTextMessage(ConversationType type, UCID fromUid, UCID toUid, int8_t textType, 
	const std::string& textFormat, const std::string& textData, const std::string& textTitle, const std::string& textDetial,
	int32_t conferenceId, int64_t eventId, int64_t startTime, const std::vector<UCID>& atUsers, UCMsg *&msg) {
	
	UCHead head;

	head.SetVersion(MESSAGE_VERSION);
	head.SetId(DBManager::GetInstance().GetMessageID());
	head.SetSeq(0);
	head.SetTimestamp(uc::Time::GetSysTime());
	head.SetFrom(fromUid);
	head.SetTo(toUid);
	head.SetConversation(0);
	head.SetControlPri(ControlPriType::unread_count_type | ControlPriType::display_type
		| ControlPriType::webpush_send_type);
	head.SetGroupAtUsers(atUsers);
	if (SINGLE_CHAT == type) {
		head.SetAppId(AppId::AppChat);
		head.SetPri(PriType::thrift_type);
		head.SetProtocoltype(ChatMessageType::NormalChat);
		head.SetProtocolId(ChatMessageId::TextContent);
		head.SetChannelPri(ChannelPriType::channel0);
	
		msg = new TextContentMsg();
		msg->SetHead(head);
		((TextContentMsg*)msg)->set_type(textType);
		((TextContentMsg*)msg)->set_text(textData);
		((TextContentMsg*)msg)->set_text_style(textFormat);
	} else if (PROJECT_CHAT == type) {
		head.SetAppId(AppId::AppChat);
		head.SetPri(PriType::thrift_type|PriType::group_type);
		head.SetProtocoltype(ChatMessageType::DiscussionChat);
		head.SetProtocolId(GroupMessageId::TextContent);
		head.SetChannelPri(ChannelPriType::channel0);

		msg = new TextContentMsg();
		msg->SetHead(head);
		((TextContentMsg*)msg)->set_type(textType);
		((TextContentMsg*)msg)->set_text(textData);
		((TextContentMsg*)msg)->set_text_style(textFormat);
	} else if (GROUP_CHAT == type) {
		head.SetAppId(AppId::AppChat);
		head.SetPri(PriType::thrift_type|PriType::group_type);
		head.SetProtocoltype(ChatMessageType::GroupChat);
		head.SetProtocolId(ClusterMessageId::TextContent);
		head.SetChannelPri(ChannelPriType::channel0);

		msg = new TextContentMsg();
		msg->SetHead(head);
		((TextContentMsg*)msg)->set_type(textType);
		((TextContentMsg*)msg)->set_text(textData);
		((TextContentMsg*)msg)->set_text_style(textFormat);
	} else if (CONFERENCE_CHAT == type) {
		head.SetAppId(AppId::AppMeeting);
		head.SetPri(PriType::thrift_type|PriType::group_type);
		head.SetProtocoltype(ConfMessageType::ConfChatMsg);
		head.SetProtocolId(ConfChatMessageId::TextType);
		head.SetChannelPri(ChannelPriType::channel1);

		msg = new ConfChatContentMsg();
		msg->SetHead(head);
		((ConfChatContentMsg*)msg)->set_eventId(eventId);
		((ConfChatContentMsg*)msg)->set_confId(conferenceId);
		((ConfChatContentMsg*)msg)->set_startTime(startTime);
		TextContent textContent;
		textContent.__set_type(textType);
		textContent.__set_text(textData);
		textContent.__set_text_style(textFormat);
		((ConfChatContentMsg*)msg)->set_text(textContent);
	} else if (OPENAPI_CHAT == type){
		head.SetAppId(AppId::AppAPI);
		head.SetPri(PriType::thrift_type | PriType::direct_type);
		head.SetProtocoltype(APIMessageType::AppMsg);
		head.SetProtocolId(APIMessageId::Text);
		head.SetChannelPri(ChannelPriType::channel0);

		msg = new APITextContentMsg();
		msg->SetHead(head);
		((APITextContentMsg*)msg)->set_title(textTitle);
		((APITextContentMsg*)msg)->set_content(textData);
		((APITextContentMsg*)msg)->set_detailType(textType);
		((APITextContentMsg*)msg)->set_detailContent(textDetial);
	}
	else if (CLOUD_CHAT == type) {
		head.SetAppId(AppId::AppChat);
		head.SetPri(PriType::thrift_type | PriType::group_type);
		head.SetProtocoltype(ChatMessageType::CloudFileChat);
		head.SetProtocolId(GroupMessageId::TextContent);
		head.SetChannelPri(ChannelPriType::channel0);

		msg = new TextContentMsg();
		msg->SetHead(head);
		((TextContentMsg*)msg)->set_type(textType);
		((TextContentMsg*)msg)->set_text(textData);
		((TextContentMsg*)msg)->set_text_style(textFormat);
	}
	
	return head.GetId();
}

int32_t MessageProcessor::CreateImageMessage(ConversationType type, UCID fromUid, UCID targetUid, const std::string& imageName,
	int32_t imageSize, const std::string& imageURL, const std::string& imageData, const std::string& imageId,
	int8_t imageType, int32_t conferenceId, int64_t eventId, int64_t startTime, UCMsg *&msg) {
	
	UCHead head;
	head.SetVersion(MESSAGE_VERSION);
	head.SetId(DBManager::GetInstance().GetMessageID());
	head.SetSeq(0);
	head.SetTimestamp(uc::Time::GetSysTime());
	head.SetFrom(fromUid);
	head.SetTo(targetUid);
	head.SetConversation(0);
	head.SetControlPri(ControlPriType::unread_count_type | ControlPriType::display_type
		| ControlPriType::webpush_send_type);
	if (SINGLE_CHAT == type) {
		head.SetAppId(AppId::AppChat);
		head.SetPri(PriType::thrift_type);
		head.SetProtocoltype(ChatMessageType::NormalChat);
		head.SetProtocolId(ChatMessageId::MediaContent);
		head.SetChannelPri(ChannelPriType::channel0);
		
		msg = new MediaContentMsg;
		msg->SetHead(head);
		((MediaContentMsg*)msg)->set_media_filename(imageName);
		((MediaContentMsg*)msg)->set_media_filesize(imageSize);
		((MediaContentMsg*)msg)->set_media_type(ChatMediaType::MediaTypeImage);
		((MediaContentMsg*)msg)->set_media_down_url(imageURL);
		((MediaContentMsg*)msg)->set_media_thumb(imageData);

	} else if(PROJECT_CHAT == type) {
		head.SetAppId(AppId::AppChat);
		head.SetPri(PriType::thrift_type | PriType::group_type);
		head.SetProtocoltype(ChatMessageType::DiscussionChat);
		head.SetProtocolId(GroupMessageId::MediaContent);
		head.SetChannelPri(ChannelPriType::channel0);

		msg = new MediaContentMsg;
		msg->SetHead(head);
		((MediaContentMsg*)msg)->set_media_filename(imageName);
		((MediaContentMsg*)msg)->set_media_filesize(imageSize);
		((MediaContentMsg*)msg)->set_media_type(ChatMediaType::MediaTypeImage);
		((MediaContentMsg*)msg)->set_media_down_url(imageURL);
		((MediaContentMsg*)msg)->set_media_thumb(imageData);
	} else if(GROUP_CHAT == type) {
		head.SetAppId(AppId::AppChat);
		head.SetPri(PriType::thrift_type | PriType::group_type);
		head.SetProtocoltype(ChatMessageType::GroupChat);
		head.SetProtocolId(ClusterMessageId::MediaContent);
		head.SetChannelPri(ChannelPriType::channel0);
	
		msg = new MediaContentMsg;
		msg->SetHead(head);
		((MediaContentMsg*)msg)->set_media_filename(imageName);
		((MediaContentMsg*)msg)->set_media_filesize(imageSize);
		((MediaContentMsg*)msg)->set_media_type(ChatMediaType::MediaTypeImage);
		((MediaContentMsg*)msg)->set_media_down_url(imageURL);
		((MediaContentMsg*)msg)->set_media_thumb(imageData);

	} else if(CONFERENCE_CHAT == type) {
		head.SetAppId(AppId::AppMeeting);
		head.SetPri(PriType::thrift_type | PriType::group_type);
		head.SetProtocoltype(ConfMessageType::ConfChatMsg);
		head.SetProtocolId(ConfChatMessageId::MediaType);
		head.SetChannelPri(ChannelPriType::channel1);
		
		msg = new ConfChatContentMsg;
		msg->SetHead(head);
		((ConfChatContentMsg*)msg)->set_eventId(eventId);
		((ConfChatContentMsg*)msg)->set_confId(conferenceId);
		((ConfChatContentMsg*)msg)->set_startTime(startTime);		
		MediaContent mediaContent;
		mediaContent.__set_media_filename(imageName);
		mediaContent.__set_media_filesize(imageSize);
		mediaContent.__set_media_type(ChatMediaType::MediaTypeImage);
		mediaContent.__set_media_down_url(imageURL);
		mediaContent.__set_media_thumb(imageData);
		((ConfChatContentMsg*)msg)->set_media(mediaContent);
	} else if (OPENAPI_CHAT == type){
		head.SetAppId(AppId::AppAPI);
		head.SetPri(PriType::thrift_type | PriType::direct_type);
		head.SetProtocoltype(APIMessageType::AppMsg);
		head.SetProtocolId(APIMessageId::Image);
		head.SetChannelPri(ChannelPriType::channel0);

		msg = new APIImageContentMsg();
 		msg->SetHead(head);
		((APIImageContentMsg*)msg)->set_mediaId(imageId);
		((APIImageContentMsg*)msg)->set_detailType(imageType);
		((APIImageContentMsg*)msg)->set_mediaThumb(imageData);
	}
	else if (CLOUD_CHAT == type) {
		head.SetAppId(AppId::AppChat);
		head.SetPri(PriType::thrift_type | PriType::group_type);
		head.SetProtocoltype(ChatMessageType::CloudFileChat);
		head.SetProtocolId(GroupMessageId::MediaContent);
		head.SetChannelPri(ChannelPriType::channel0);

		msg = new MediaContentMsg;
		msg->SetHead(head);
		((MediaContentMsg*)msg)->set_media_filename(imageName);
		((MediaContentMsg*)msg)->set_media_filesize(imageSize);
		((MediaContentMsg*)msg)->set_media_type(ChatMediaType::MediaTypeImage);
		((MediaContentMsg*)msg)->set_media_down_url(imageURL);
		((MediaContentMsg*)msg)->set_media_thumb(imageData);
	}

	return head.GetId();
}

int32_t MessageProcessor::CreateAudioMessage(ConversationType type, UCID fromUid, UCID targetUid, const std::string& audioName,
	int32_t audioSize, const std::string& audioURL, int32_t audioDuration, int32_t conferenceId, int64_t eventId, int64_t startTime, UCMsg *&msg){

	UCHead head;
	head.SetVersion(MESSAGE_VERSION);
	head.SetId(DBManager::GetInstance().GetMessageID());
	head.SetSeq(0);
	head.SetTimestamp(uc::Time::GetSysTime());
	head.SetFrom(fromUid);
	head.SetTo(targetUid);
	head.SetConversation(0);
	head.SetControlPri(ControlPriType::unread_count_type | ControlPriType::display_type
		| ControlPriType::webpush_send_type);
	if (SINGLE_CHAT == type) {
		head.SetAppId(AppId::AppChat);
		head.SetPri(PriType::thrift_type | PriType::audio_type);
		head.SetProtocoltype(ChatMessageType::NormalChat);
		head.SetProtocolId(ChatMessageId::MediaContent);
		head.SetChannelPri(ChannelPriType::channel0);
		
		msg = new MediaContentMsg;
		msg->SetHead(head);
		((MediaContentMsg*)msg)->set_media_filename(audioName);
		((MediaContentMsg*)msg)->set_media_filesize(audioSize);
		((MediaContentMsg*)msg)->set_media_type(ChatMediaType::MediaTypeAudio);
		((MediaContentMsg*)msg)->set_media_down_url(audioURL);
		((MediaContentMsg*)msg)->set_media_thumb("");
		((MediaContentMsg*)msg)->set_media_duration(audioDuration);
	
	} else if(PROJECT_CHAT == type) {
		head.SetAppId(AppId::AppChat);
		head.SetPri(PriType::thrift_type | PriType::group_type | PriType::audio_type);
		head.SetProtocoltype(ChatMessageType::DiscussionChat);
		head.SetProtocolId(GroupMessageId::MediaContent);
		head.SetChannelPri(ChannelPriType::channel0);

		msg = new MediaContentMsg;
		msg->SetHead(head);
		((MediaContentMsg*)msg)->set_media_filename(audioName);
		((MediaContentMsg*)msg)->set_media_filesize(audioSize);
		((MediaContentMsg*)msg)->set_media_type(ChatMediaType::MediaTypeAudio);
		((MediaContentMsg*)msg)->set_media_down_url(audioURL);
		((MediaContentMsg*)msg)->set_media_thumb("");
		((MediaContentMsg*)msg)->set_media_duration(audioDuration);

	} else if(GROUP_CHAT == type) {
		head.SetAppId(AppId::AppChat);
		head.SetPri(PriType::thrift_type | PriType::group_type | PriType::audio_type);
		head.SetProtocoltype(ChatMessageType::GroupChat);
		head.SetProtocolId(ClusterMessageId::MediaContent);
		head.SetChannelPri(ChannelPriType::channel0);

		msg = new MediaContentMsg;
		msg->SetHead(head);
		((MediaContentMsg*)msg)->set_media_filename(audioName);
		((MediaContentMsg*)msg)->set_media_filesize(audioSize);
		((MediaContentMsg*)msg)->set_media_type(ChatMediaType::MediaTypeAudio);
		((MediaContentMsg*)msg)->set_media_down_url(audioURL);
		((MediaContentMsg*)msg)->set_media_thumb("");
		((MediaContentMsg*)msg)->set_media_duration(audioDuration);

	} else if(CONFERENCE_CHAT == type) {
		head.SetAppId(AppId::AppMeeting);
		head.SetPri(PriType::thrift_type | PriType::group_type | PriType::audio_type);
		head.SetProtocoltype(ConfMessageType::ConfChatMsg);
		head.SetProtocolId(ConfChatMessageId::MediaType);
		head.SetChannelPri(ChannelPriType::channel1);
		
		msg = new ConfChatContentMsg;
		msg->SetHead(head);
		((ConfChatContentMsg*)msg)->set_eventId(eventId);
		((ConfChatContentMsg*)msg)->set_confId(conferenceId);
		((ConfChatContentMsg*)msg)->set_startTime(startTime);
		MediaContent mediaContent;
		mediaContent.__set_media_filename(audioName);
		mediaContent.__set_media_filesize(audioSize);
		mediaContent.__set_media_type(ChatMediaType::MediaTypeAudio);
		mediaContent.__set_media_down_url(audioURL);
		mediaContent.__set_media_thumb("");
		mediaContent.__set_media_duration(audioDuration);
		((ConfChatContentMsg*)msg)->set_media(mediaContent);
	}
	else if (CLOUD_CHAT == type) {
		head.SetAppId(AppId::AppChat);
		head.SetPri(PriType::thrift_type | PriType::group_type | PriType::audio_type);
		head.SetProtocoltype(ChatMessageType::CloudFileChat);
		head.SetProtocolId(ClusterMessageId::MediaContent);
		head.SetChannelPri(ChannelPriType::channel0);

		msg = new MediaContentMsg;
		msg->SetHead(head);
		((MediaContentMsg*)msg)->set_media_filename(audioName);
		((MediaContentMsg*)msg)->set_media_filesize(audioSize);
		((MediaContentMsg*)msg)->set_media_type(ChatMediaType::MediaTypeAudio);
		((MediaContentMsg*)msg)->set_media_down_url(audioURL);
		((MediaContentMsg*)msg)->set_media_thumb("");
		((MediaContentMsg*)msg)->set_media_duration(audioDuration);
	}

	return head.GetId();
}

int32_t MessageProcessor::CreateVideoMessage(ConversationType type, UCID fromUid, UCID targetUid, const std::string& videoName,
	int32_t videoSize, const std::string& videoURL, const std::string& videoData, int32_t videoDuration, int32_t conferenceId,
	int64_t eventId, int64_t startTime,	UCMsg *&msg) {

	UCHead head;
	head.SetVersion(MESSAGE_VERSION);
	head.SetId(DBManager::GetInstance().GetMessageID());
	head.SetSeq(0);
	head.SetTimestamp(uc::Time::GetSysTime());
	head.SetFrom(fromUid);
	head.SetTo(targetUid);
	head.SetConversation(0);
	head.SetControlPri(ControlPriType::unread_count_type | ControlPriType::display_type
		| ControlPriType::webpush_send_type);
	if (SINGLE_CHAT == type){
		head.SetAppId(AppId::AppChat);
		head.SetPri(PriType::thrift_type);
		head.SetProtocoltype(ChatMessageType::NormalChat);
		head.SetProtocolId(ChatMessageId::MediaContent);
		head.SetChannelPri(ChannelPriType::channel0);

		msg = new MediaContentMsg;
		msg->SetHead(head);
		((MediaContentMsg*)msg)->set_media_filename(videoName);
		((MediaContentMsg*)msg)->set_media_filesize(videoSize);
		((MediaContentMsg*)msg)->set_media_type(ChatMediaType::MediaTypeVideo);
		((MediaContentMsg*)msg)->set_media_down_url(videoURL);
		((MediaContentMsg*)msg)->set_media_thumb(videoData);
		((MediaContentMsg*)msg)->set_media_duration(videoDuration);

	} else if (PROJECT_CHAT == type) {
		head.SetAppId(AppId::AppChat);
		head.SetPri(PriType::thrift_type | PriType::group_type);
		head.SetProtocoltype(ChatMessageType::DiscussionChat);
		head.SetProtocolId(GroupMessageId::MediaContent);
		head.SetChannelPri(ChannelPriType::channel0);

		msg = new MediaContentMsg;
		msg->SetHead(head);
		((MediaContentMsg*)msg)->set_media_filename(videoName);
		((MediaContentMsg*)msg)->set_media_filesize(videoSize);
		((MediaContentMsg*)msg)->set_media_type(ChatMediaType::MediaTypeVideo);
		((MediaContentMsg*)msg)->set_media_down_url(videoURL);
		((MediaContentMsg*)msg)->set_media_thumb(videoData);
		((MediaContentMsg*)msg)->set_media_duration(videoDuration);

	} else if (GROUP_CHAT == type){
		head.SetAppId(AppId::AppChat);
		head.SetPri(PriType::thrift_type | PriType::group_type);
		head.SetProtocoltype(ChatMessageType::GroupChat);
		head.SetProtocolId(ClusterMessageId::MediaContent);
		head.SetChannelPri(ChannelPriType::channel0);

		msg = new MediaContentMsg;
		msg->SetHead(head);
		((MediaContentMsg*)msg)->set_media_filename(videoName);
		((MediaContentMsg*)msg)->set_media_filesize(videoSize);
		((MediaContentMsg*)msg)->set_media_type(ChatMediaType::MediaTypeVideo);
		((MediaContentMsg*)msg)->set_media_down_url(videoURL);
		((MediaContentMsg*)msg)->set_media_thumb(videoData);
		((MediaContentMsg*)msg)->set_media_duration(videoDuration);

	} else if (CONFERENCE_CHAT == type){
		head.SetAppId(AppId::AppMeeting);
		head.SetPri(PriType::thrift_type | PriType::group_type);
		head.SetProtocoltype(ConfMessageType::ConfChatMsg);
		head.SetProtocolId(ConfChatMessageId::MediaType);
		head.SetChannelPri(ChannelPriType::channel1);
		
		msg = new ConfChatContentMsg;
		msg->SetHead(head);
		((ConfChatContentMsg*)msg)->set_eventId(eventId);
		((ConfChatContentMsg*)msg)->set_confId(conferenceId);
		((ConfChatContentMsg*)msg)->set_startTime(startTime);
		MediaContent mediaContent;
		mediaContent.__set_media_filename(videoName);
		mediaContent.__set_media_filesize(videoSize);
		mediaContent.__set_media_type(ChatMediaType::MediaTypeVideo);
		mediaContent.__set_media_down_url(videoURL);
		mediaContent.__set_media_thumb(videoData);
		mediaContent.__set_media_duration(videoDuration);
		((ConfChatContentMsg*)msg)->set_media(mediaContent);
	}	
	return head.GetId();
}

int32_t MessageProcessor::CreateReportMessage(ConversationType type, UCID fromUid, UCID targetUid, const std::string& reportTitle,
	const std::string& reportUrl, int64_t eventId, OUT UCMsg *&msg){


	UCHead head;
	head.SetVersion(MESSAGE_VERSION);
	head.SetId(DBManager::GetInstance().GetMessageID());
	head.SetSeq(0);
	head.SetTimestamp(uc::Time::GetSysTime());
	head.SetFrom(fromUid);
	head.SetTo(targetUid);
	head.SetConversation(0);
	head.SetControlPri(ControlPriType::unread_count_type | ControlPriType::display_type
		| ControlPriType::webpush_send_type);
	if (SINGLE_CHAT == type){
		head.SetAppId(AppId::AppChat);
		head.SetPri(PriType::thrift_type);
		head.SetProtocoltype(ChatMessageType::NormalChat);
		head.SetProtocolId(ChatMessageId::ReportContent);
		head.SetChannelPri(ChannelPriType::channel0);
	}
	else if (PROJECT_CHAT == type) {
		head.SetAppId(AppId::AppChat);
		head.SetPri(PriType::thrift_type | PriType::group_type);
		head.SetProtocoltype(ChatMessageType::DiscussionChat);
		head.SetProtocolId(GroupMessageId::ReportContent);
		head.SetChannelPri(ChannelPriType::channel0);

	}
	else if (GROUP_CHAT == type){
		head.SetAppId(AppId::AppChat);
		head.SetPri(PriType::thrift_type | PriType::group_type);
		head.SetProtocoltype(ChatMessageType::GroupChat);
		head.SetProtocolId(ClusterMessageId::ReportContent);
		head.SetChannelPri(ChannelPriType::channel0);
	}
	else if (CONFERENCE_CHAT == type){
		head.SetAppId(AppId::AppMeeting);
		head.SetPri(PriType::thrift_type | PriType::group_type);
		head.SetProtocoltype(ConfMessageType::ConfReportMsg);
		head.SetProtocolId(ConfChatMessageId::TextType);
		head.SetChannelPri(ChannelPriType::channel1);
	}

	msg = new ReportContentMsg;
	msg->SetHead(head);
	((ReportContentMsg*)msg)->set_reportTitle(reportTitle);
	((ReportContentMsg*)msg)->set_reportUrl(reportUrl);
	((ReportContentMsg*)msg)->set_eventId(eventId);

	return head.GetId();
}

int32_t MessageProcessor::CreateRecordVideoMessage(ConversationType type, UCID fromUid, UCID targetUid, const std::string& videoName, 
	int32_t videoSize, const std::string& videoURL, const std::string& videoThumb, const std::string& videoThumbURL, int32_t videoDuration, int32_t operatorId,
	const std::string& operatorName, const std::string& operatorAvatar, int32_t conferenceId, int64_t eventId, int64_t startTime, OUT UCMsg *&msg) {
	
	UCHead head;
	head.SetVersion(MESSAGE_VERSION);
	head.SetId(DBManager::GetInstance().GetMessageID());
	head.SetSeq(0);
	head.SetTimestamp(uc::Time::GetSysTime());
	head.SetFrom(fromUid);
	head.SetTo(targetUid);
	head.SetConversation(0);
	head.SetControlPri(ControlPriType::unread_count_type | ControlPriType::display_type
		| ControlPriType::webpush_send_type);
	if (SINGLE_CHAT == type){
		head.SetAppId(AppId::AppChat);
		head.SetPri(PriType::thrift_type);
		head.SetProtocoltype(ChatMessageType::NormalChat);
		head.SetProtocolId(ChatMessageId::MediaContent);
		head.SetChannelPri(ChannelPriType::channel0);

		msg = new MediaContentMsg;
		msg->SetHead(head);
		((MediaContentMsg*)msg)->set_media_filename(videoName);
		((MediaContentMsg*)msg)->set_media_filesize(videoSize);
		((MediaContentMsg*)msg)->set_media_type(ChatMediaType::MediaTypeMeetingRecord);
		((MediaContentMsg*)msg)->set_media_down_url(videoURL);
		((MediaContentMsg*)msg)->set_media_thumb(videoThumb);
		((MediaContentMsg*)msg)->set_media_thumb_url(videoThumbURL);
		((MediaContentMsg*)msg)->set_media_duration(videoDuration);
		((MediaContentMsg*)msg)->set_operatorId(operatorId);
		((MediaContentMsg*)msg)->set_operatorName(operatorName);
		((MediaContentMsg*)msg)->set_operatorAvatar(operatorAvatar);
	}
	else if (PROJECT_CHAT == type) {
		head.SetAppId(AppId::AppChat);
		head.SetPri(PriType::thrift_type | PriType::group_type);
		head.SetProtocoltype(ChatMessageType::DiscussionChat);
		head.SetProtocolId(GroupMessageId::MediaContent);
		head.SetChannelPri(ChannelPriType::channel0);

		msg = new MediaContentMsg;
		msg->SetHead(head);
		((MediaContentMsg*)msg)->set_media_filename(videoName);
		((MediaContentMsg*)msg)->set_media_filesize(videoSize);
		((MediaContentMsg*)msg)->set_media_type(ChatMediaType::MediaTypeMeetingRecord);
		((MediaContentMsg*)msg)->set_media_down_url(videoURL);
		((MediaContentMsg*)msg)->set_media_thumb(videoThumb);
		((MediaContentMsg*)msg)->set_media_thumb_url(videoThumbURL);
		((MediaContentMsg*)msg)->set_media_duration(videoDuration);
		((MediaContentMsg*)msg)->set_operatorId(operatorId);
		((MediaContentMsg*)msg)->set_operatorName(operatorName);
		((MediaContentMsg*)msg)->set_operatorAvatar(operatorAvatar);

	}
	else if (GROUP_CHAT == type){
		head.SetAppId(AppId::AppChat);
		head.SetPri(PriType::thrift_type | PriType::group_type);
		head.SetProtocoltype(ChatMessageType::GroupChat);
		head.SetProtocolId(ClusterMessageId::MediaContent);
		head.SetChannelPri(ChannelPriType::channel0);

		msg = new MediaContentMsg;
		msg->SetHead(head);
		((MediaContentMsg*)msg)->set_media_filename(videoName);
		((MediaContentMsg*)msg)->set_media_filesize(videoSize);
		((MediaContentMsg*)msg)->set_media_type(ChatMediaType::MediaTypeMeetingRecord);
		((MediaContentMsg*)msg)->set_media_down_url(videoURL);
		((MediaContentMsg*)msg)->set_media_thumb(videoThumb);
		((MediaContentMsg*)msg)->set_media_thumb_url(videoThumbURL);
		((MediaContentMsg*)msg)->set_media_duration(videoDuration);
		((MediaContentMsg*)msg)->set_operatorId(operatorId);
		((MediaContentMsg*)msg)->set_operatorName(operatorName);
		((MediaContentMsg*)msg)->set_operatorAvatar(operatorAvatar);

	}
	else if (CONFERENCE_CHAT == type){
		head.SetAppId(AppId::AppMeeting);
		head.SetPri(PriType::thrift_type | PriType::group_type);
		head.SetProtocoltype(ConfMessageType::ConfChatMsg);
		head.SetProtocolId(ConfChatMessageId::MediaType);
		head.SetChannelPri(ChannelPriType::channel1);

		msg = new ConfChatContentMsg;
		msg->SetHead(head);
		((ConfChatContentMsg*)msg)->set_eventId(eventId);
		((ConfChatContentMsg*)msg)->set_confId(conferenceId);
		((ConfChatContentMsg*)msg)->set_startTime(startTime);
		MediaContent mediaContent;
		mediaContent.__set_media_filename(videoName);
		((MediaContentMsg*)msg)->set_media_filesize(videoSize);
		((MediaContentMsg*)msg)->set_media_type(ChatMediaType::MediaTypeMeetingRecord);
		((MediaContentMsg*)msg)->set_media_down_url(videoURL);
		((MediaContentMsg*)msg)->set_media_thumb(videoThumb);
		((MediaContentMsg*)msg)->set_media_thumb_url(videoThumbURL);
		((MediaContentMsg*)msg)->set_media_duration(videoDuration);
		((MediaContentMsg*)msg)->set_operatorId(operatorId);
		((MediaContentMsg*)msg)->set_operatorName(operatorName);
		((MediaContentMsg*)msg)->set_operatorAvatar(operatorAvatar);
		((ConfChatContentMsg*)msg)->set_media(mediaContent);
	}
	return head.GetId();
}
int32_t MessageProcessor::CreateSendFileMessage( ConversationType type, UCID fromUid, UCID targetUid, const std::string& fileName,
	int32_t fileSize, const std::string& fileURL, int64_t fileId, const std::string& apiFileId, int64_t apiFileSize, int8_t fileType,
	int64_t eventId, int64_t conferenceId, int64_t startTime, UCMsg *&msg) {
	
	UCHead head;

	head.SetVersion(MESSAGE_VERSION);
	
	head.SetId(DBManager::GetInstance().GetMessageID());
	head.SetSeq(0);
	head.SetTimestamp(uc::Time::GetSysTime());
	head.SetFrom(fromUid);
	head.SetTo(targetUid);
	head.SetConversation(0);
	head.SetControlPri(ControlPriType::unread_count_type | ControlPriType::display_type
		| ControlPriType::webpush_send_type);
	if (SINGLE_CHAT == type) {
		head.SetAppId(AppId::AppChat);
		head.SetChannelPri(ChannelPriType::channel0);
		head.SetPri(PriType::thrift_type);
		head.SetProtocoltype(ChatMessageType::NormalChat);
		head.SetProtocolId(ChatMessageId::DocumentSend);

		msg = new DocumentContentMsg();
		((DocumentContentMsg*)msg)->SetHead(head);
		((DocumentContentMsg*)msg)->set_operatorid(fromUid.GetUserId());
		((DocumentContentMsg*)msg)->set_doc_name(fileName);
		((DocumentContentMsg*)msg)->set_size(fileSize);
		((DocumentContentMsg*)msg)->set_down_url(fileURL);
		((DocumentContentMsg*)msg)->set_contentId(fileId);
	} else if (PROJECT_CHAT == type) {
		head.SetAppId(AppId::AppChat);
		head.SetChannelPri(ChannelPriType::channel0);
		head.SetPri(PriType::thrift_type | PriType::group_type);
		head.SetProtocoltype(ChatMessageType::DiscussionChat);
		head.SetProtocolId(GroupMessageId::SummaryCreate);
	
		msg = new SummaryCreateContentMsg();
		((SummaryCreateContentMsg*)msg)->SetHead(head);
		((SummaryCreateContentMsg*)msg)->set_operatorid(fromUid.GetUserId());
		((SummaryCreateContentMsg*)msg)->set_summary_name(fileName);
		((SummaryCreateContentMsg*)msg)->set_size(fileSize);
		((SummaryCreateContentMsg*)msg)->set_down_url(fileURL);
		((SummaryCreateContentMsg*)msg)->set_contentId(fileId);
	} else if (GROUP_CHAT == type) {
		head.SetAppId(AppId::AppChat);
		head.SetChannelPri(ChannelPriType::channel0);
		head.SetPri(PriType::thrift_type | PriType::group_type);
		head.SetProtocoltype(ChatMessageType::GroupChat);
		head.SetProtocolId(ClusterMessageId::SummaryCreate);
		
		msg = new SummaryCreateContentMsg();
		((SummaryCreateContentMsg*)msg)->SetHead(head);
		((SummaryCreateContentMsg*)msg)->set_operatorid(fromUid.GetUserId());
		((SummaryCreateContentMsg*)msg)->set_summary_name(fileName);
		((SummaryCreateContentMsg*)msg)->set_size(fileSize);
		((SummaryCreateContentMsg*)msg)->set_down_url(fileURL);
		((SummaryCreateContentMsg*)msg)->set_contentId(fileId);
	} else if (CONFERENCE_CHAT == type) {
		// TODO
	}else if (OPENAPI_CHAT == type){
		head.SetAppId(AppId::AppAPI);
		head.SetChannelPri(ChannelPriType::channel0);
		head.SetPri(PriType::thrift_type | PriType::direct_type);
		head.SetProtocoltype(APIMessageType::AppMsg);
		head.SetProtocolId(APIMessageId::File);
		head.SetChannelPri(ChannelPriType::channel0);

		msg = new APIFileContentMsg();
 		msg->SetHead(head);
 		((APIFileContentMsg*)msg)->set_mediaId(apiFileId);
 		((APIFileContentMsg*)msg)->set_fileName(fileName);
 		((APIFileContentMsg*)msg)->set_detailType(fileType);
		((APIFileContentMsg*)msg)->set_fileSize(apiFileSize);
	}

	return head.GetId();
}

int32_t MessageProcessor::CreateSendCloudFileMessage(ConversationType type, CloudFileOperation operation, UCID fromUid, UCID targetUid,
	const std::string& operatorName, const std::string& fileName, const std::string& fileNameTo, int64_t fileSize,
	int8_t count, int8_t isDir, int8_t cloudType, const std::string& detailContent, int64_t createTime,
	int64_t fileId, OUT UCMsg *&msg) {
	if ((type != CLOUD_CHAT && operation != CFO_CloudFileCreate)) {
		return MESSAGE_NOT_SUPPORTED;
	}

	UCHead head;
	if (SINGLE_CHAT == type) {
		head.SetPri(PriType::thrift_type);
		head.SetProtocoltype(ChatMessageType::NormalChat);
		head.SetProtocolId(ChatMessageId::CloudFileCreate);
	}
	else if (PROJECT_CHAT == type) {
		head.SetPri(PriType::thrift_type | PriType::group_type);
		head.SetProtocoltype(ChatMessageType::DiscussionChat);
		head.SetProtocolId(GroupMessageId::CloudFileCreate);
	}
	else if (GROUP_CHAT == type) {
		head.SetPri(PriType::thrift_type | PriType::group_type);
		head.SetProtocoltype(ChatMessageType::GroupChat);
		head.SetProtocolId(ClusterMessageId::CloudFileCreate);
	}
	else if (CLOUD_CHAT == type) {
		head.SetPri(PriType::thrift_type | PriType::group_type);
		head.SetProtocoltype(ChatMessageType::CloudFileChat);
		head.SetProtocolId(GetCloudFileMessageIdByOperation(operation));
	}
	else {
		return MESSAGE_NOT_SUPPORTED;
	}

	head.SetVersion(MESSAGE_VERSION);
	head.SetId(DBManager::GetInstance().GetMessageID());
	head.SetSeq(0);
	head.SetTimestamp(uc::Time::GetSysTime());
	head.SetFrom(fromUid);
	head.SetTo(targetUid);
	head.SetConversation(0);
	head.SetControlPri(ControlPriType::unread_count_type | ControlPriType::display_type
		| ControlPriType::webpush_send_type);
	head.SetAppId(AppId::AppChat);
	head.SetChannelPri(ChannelPriType::channel0);
	
	msg = new CloudFileContentMsg();
	((CloudFileContentMsg*)msg)->SetHead(head);
	((CloudFileContentMsg*)msg)->set_operatorId(fromUid.GetUserId());
	((CloudFileContentMsg*)msg)->set_operatorName(operatorName);
	((CloudFileContentMsg*)msg)->set_fileName(fileName);
	((CloudFileContentMsg*)msg)->set_fileNameTo(fileNameTo);
	((CloudFileContentMsg*)msg)->set_size(fileSize);
	((CloudFileContentMsg*)msg)->set_isDir(isDir);
	((CloudFileContentMsg*)msg)->set_cloudType(cloudType);
	((CloudFileContentMsg*)msg)->set_fileCounts(count);

	if (cloudType == CloudType::GoKuai) {
		((CloudFileContentMsg*)msg)->set_detailType(GoKuaiDetailType::FileOperate);
	}
	else if (cloudType == CloudType::AnZhen) {
		((CloudFileContentMsg*)msg)->set_detailType(AnZhenDetailType::FileURL);
	}

	((CloudFileContentMsg*)msg)->set_detailContent(detailContent);
	((CloudFileContentMsg*)msg)->set_createTime(createTime);
	((CloudFileContentMsg*)msg)->set_contentId(fileId);

	return head.GetId();
}

int32_t MessageProcessor::CreateShareFileMessage(ConversationType type, UCID fromUid, UCID targetUid, const std::string& fileName,int32_t fileSize, 
	const std::string& fileURL, int64_t fileId, int64_t eventId, int64_t conferenceId, int64_t startTime, UCMsg *&msg) {

	
	UCHead head;

	head.SetVersion(MESSAGE_VERSION);
	
	head.SetId(DBManager::GetInstance().GetMessageID());
	head.SetSeq(0);
	head.SetTimestamp(uc::Time::GetSysTime());
	head.SetFrom(fromUid);
	head.SetTo(targetUid);
	head.SetConversation(0);
	head.SetControlPri(ControlPriType::unread_count_type | ControlPriType::display_type
		| ControlPriType::webpush_send_type);

	if (SINGLE_CHAT == type){
		head.SetAppId(AppId::AppChat);
		head.SetPri(PriType::thrift_type);
		head.SetProtocoltype(ChatMessageType::NormalChat);
		head.SetProtocolId(ChatMessageId::DocumentForward);
		head.SetChannelPri(ChannelPriType::channel0);

		msg = new DocumentContentMsg;
		((DocumentContentMsg*)msg)->SetHead(head);
		((DocumentContentMsg*)msg)->set_operatorid(fromUid.GetUserId());
		((DocumentContentMsg*)msg)->set_doc_name(fileName);
		((DocumentContentMsg*)msg)->set_size(fileSize);
		((DocumentContentMsg*)msg)->set_down_url(fileURL);
		((DocumentContentMsg*)msg)->set_contentId(fileId);

	}else if(PROJECT_CHAT == type){
		head.SetAppId(AppId::AppChat);
		head.SetPri(PriType::thrift_type | PriType::group_type);
		head.SetProtocoltype(ChatMessageType::DiscussionChat);
		head.SetProtocolId(GroupMessageId::DocumentForward);
		head.SetChannelPri(ChannelPriType::channel0);

		msg = new SummaryCreateContentMsg;
		((SummaryCreateContentMsg*)msg)->SetHead(head);
		((SummaryCreateContentMsg*)msg)->set_operatorid(fromUid.GetUserId());
		((SummaryCreateContentMsg*)msg)->set_summary_name(fileName);
		((SummaryCreateContentMsg*)msg)->set_size(fileSize);
		((SummaryCreateContentMsg*)msg)->set_down_url(fileURL);
		((SummaryCreateContentMsg*)msg)->set_contentId(fileId);

	}else if(GROUP_CHAT == type){
		head.SetAppId(AppId::AppChat);
		head.SetPri(PriType::thrift_type | PriType::group_type);
		head.SetProtocoltype(ChatMessageType::GroupChat);
		head.SetProtocolId(ClusterMessageId::DocumentForward);
		head.SetChannelPri(ChannelPriType::channel0);

		msg = new SummaryCreateContentMsg;
		((SummaryCreateContentMsg*)msg)->SetHead(head);
		((SummaryCreateContentMsg*)msg)->set_operatorid(fromUid.GetUserId());
		((SummaryCreateContentMsg*)msg)->set_summary_name(fileName);
		((SummaryCreateContentMsg*)msg)->set_size(fileSize);
		((SummaryCreateContentMsg*)msg)->set_down_url(fileURL);
		((SummaryCreateContentMsg*)msg)->set_contentId(fileId);
	}
	


	return head.GetId();
}

int32_t MessageProcessor::CreateDeleteFileMessage(ConversationType type, UCID fromUid, UCID targetUid, const std::string& fileName,
	int64_t fileId, UCMsg *&msg){

	msg = new DocumentDelContentMsg;
	UCHead head; 
	head.SetVersion(MESSAGE_VERSION);
	head.SetAppId(AppId::AppChat);
	if (SINGLE_CHAT == type){
		head.SetPri(PriType::thrift_type);
		head.SetControlPri(0);
		head.SetProtocoltype(ChatMessageType::NormalChat);
		head.SetProtocolId(ChatMessageId::DocumentDel);
	}else if(PROJECT_CHAT == type){
		head.SetPri(PriType::thrift_type | PriType::group_type);
		head.SetControlPri(ControlPriType::unread_count_type | ControlPriType::display_type
			| ControlPriType::webpush_send_type);
		head.SetProtocoltype(ChatMessageType::DiscussionChat);
		head.SetProtocolId(GroupMessageId::DocumentDel);
	}else if(GROUP_CHAT == type){
		head.SetPri(PriType::thrift_type | PriType::group_type);
		head.SetControlPri(ControlPriType::unread_count_type | ControlPriType::display_type
			| ControlPriType::webpush_send_type);
		head.SetProtocoltype(ChatMessageType::GroupChat);
		head.SetProtocolId(ClusterMessageId::DocumentDel);
	}
	head.SetChannelPri(ChannelPriType::channel0);
	head.SetId(DBManager::GetInstance().GetMessageID());
	head.SetSeq(0);
	head.SetTimestamp(uc::Time::GetSysTime());
	head.SetFrom(fromUid);
	head.SetTo(targetUid);
	head.SetConversation(0);

	if (msg != NULL ){
		((DocumentDelContentMsg*)msg)->SetHead(head);
		((DocumentDelContentMsg*)msg)->set_operatorid(fromUid.GetUserId());
		((DocumentDelContentMsg*)msg)->set_doc_name(fileName);
		((DocumentDelContentMsg*)msg)->set_contentId(fileId);
	}

	return head.GetId();
}

int32_t MessageProcessor::CreateSendCodeMessage(ConversationType type, UCID fromUid, UCID targetUid, const std::string& creatorName, 
	int64_t codeId, const std::string& codeTitle, const std::string& langType, const std::string& codeHead, const std::string& codeDesc, 
	int32_t codeSize, const std::string& codeUrl, int32_t codeLine, int64_t createTime, UCMsg *&msg){

	msg = new CodeCreateContentMsg();
	UCHead head;
	head.SetVersion(MESSAGE_VERSION);
	head.SetAppId(AppId::AppChat);

	if (SINGLE_CHAT == type) {
		head.SetPri(PriType::thrift_type);
		head.SetProtocoltype(ChatMessageType::NormalChat);
		head.SetProtocolId(ChatMessageId::CodeCreate);
	} else if (PROJECT_CHAT == type) {
		head.SetPri(PriType::thrift_type | PriType::group_type);
		head.SetProtocoltype(ChatMessageType::DiscussionChat);
		head.SetProtocolId(GroupMessageId::CodeCreate);
	} else if (GROUP_CHAT == type) {
		head.SetPri(PriType::thrift_type | PriType::group_type);
		head.SetProtocoltype(ChatMessageType::GroupChat);
		head.SetProtocolId(ClusterMessageId::CodeCreate);
	} 
	head.SetChannelPri(ChannelPriType::channel0);
	head.SetControlPri(ControlPriType::unread_count_type | ControlPriType::display_type
		| ControlPriType::webpush_send_type);

	head.SetId(DBManager::GetInstance().GetMessageID());
	head.SetSeq(0);
	head.SetTimestamp(uc::Time::GetSysTime());
	head.SetFrom(fromUid);
	head.SetTo(targetUid);
	head.SetConversation(0);

	if (msg != NULL )
	{
		((CodeCreateContentMsg*)msg)->SetHead(head);
		((CodeCreateContentMsg*)msg)->set_operatorid(fromUid.GetUserId());
		((CodeCreateContentMsg*)msg)->set_operatorName(creatorName);
		((CodeCreateContentMsg*)msg)->set_contentId(codeId);
		((CodeCreateContentMsg*)msg)->set_title(codeTitle);
		((CodeCreateContentMsg*)msg)->set_langType(langType);
		((CodeCreateContentMsg*)msg)->set_contentHead(codeHead);
		((CodeCreateContentMsg*)msg)->set_downUrl(codeUrl);
		((CodeCreateContentMsg*)msg)->set_desc(codeDesc);
		((CodeCreateContentMsg*)msg)->set_size(codeSize);
		((CodeCreateContentMsg*)msg)->set_totalLines(codeLine);
		((CodeCreateContentMsg*)msg)->set_createTime(createTime);
	}
	return head.GetId();
}

int32_t MessageProcessor::CreateShareCodeMessage(ConversationType type, UCID fromUid, UCID targetUid, const std::string& creatorName, 
	int64_t codeId, const std::string& codeTitle, const std::string& langType, const std::string& codeHead, const std::string& codeDesc, 
	int32_t codeSize, const std::string& codeUrl, int32_t codeLine, int64_t createTime,  UCMsg *&msg){

	msg = new CodeCreateContentMsg();
	UCHead head;
	head.SetVersion(MESSAGE_VERSION);
	head.SetAppId(AppId::AppChat);

	if (SINGLE_CHAT == type) {
		head.SetPri(PriType::thrift_type);
		head.SetProtocoltype(ChatMessageType::NormalChat);
		head.SetProtocolId(ChatMessageId::CodeForward);
	} else if (PROJECT_CHAT == type) {
		head.SetPri(PriType::thrift_type | PriType::group_type);
		head.SetProtocoltype(ChatMessageType::DiscussionChat);
		head.SetProtocolId(GroupMessageId::CodeForward);
	} else if (GROUP_CHAT == type) {
		head.SetPri(PriType::thrift_type | PriType::group_type);
		head.SetProtocoltype(ChatMessageType::GroupChat);
		head.SetProtocolId(ClusterMessageId::CodeForward);
	} 
	head.SetChannelPri(ChannelPriType::channel0);
	head.SetControlPri(ControlPriType::unread_count_type | ControlPriType::display_type
		| ControlPriType::webpush_send_type);

	head.SetId(DBManager::GetInstance().GetMessageID());
	head.SetSeq(0);
	head.SetTimestamp(uc::Time::GetSysTime());
	head.SetFrom(fromUid);
	head.SetTo(targetUid);
	head.SetConversation(0);

	if (msg != NULL )
	{
		((CodeCreateContentMsg*)msg)->SetHead(head);
		((CodeCreateContentMsg*)msg)->set_operatorid(fromUid.GetUserId());
		((CodeCreateContentMsg*)msg)->set_operatorName(creatorName);
		((CodeCreateContentMsg*)msg)->set_contentId(codeId);
		((CodeCreateContentMsg*)msg)->set_title(codeTitle);
		((CodeCreateContentMsg*)msg)->set_langType(langType);
		((CodeCreateContentMsg*)msg)->set_contentHead(codeHead);
		((CodeCreateContentMsg*)msg)->set_downUrl(codeUrl);
		((CodeCreateContentMsg*)msg)->set_desc(codeDesc);
		((CodeCreateContentMsg*)msg)->set_size(codeSize);
		((CodeCreateContentMsg*)msg)->set_totalLines(codeLine);
		((CodeCreateContentMsg*)msg)->set_createTime(createTime);
	}
	return head.GetId();
}

int32_t MessageProcessor::CreateCallInvitationMessage(ConversationType type, UCID fromUid, UCID targetUid, const std::string& confId,
		const std::string& confPwd, int32_t hostId, HTTPRequest &request, int32_t &msgId, int64_t &msgTime, UCID privateId) {

	AudioInviteContentMsg msg;
	UCHead head;

	msgId = DBManager::GetInstance().GetMessageID();
	msgTime = uc::Time::GetSysTime();
	head.SetVersion(MESSAGE_VERSION);
	head.SetAppId(AppId::AppAudio);
	if (SINGLE_CHAT == type){
		head.SetPri(PriType::thrift_type);
		head.SetProtocoltype(AudioChatMessageType::DefaultType);
	}else if(PROJECT_CHAT == type){
		head.SetPri(PriType::thrift_type | PriType::group_type);
		head.SetProtocoltype(AudioChatMessageType::DiscussionType);
	}else if(GROUP_CHAT == type){
		head.SetPri(PriType::thrift_type | PriType::group_type);
		head.SetProtocoltype(AudioChatMessageType::GroupType);
	}
	head.SetControlPri(ControlPriType::webpush_send_type);
	head.SetProtocolId(AudioChatMessageId::AudioInvite);
	head.SetChannelPri(ChannelPriType::channel0);
	head.SetId(msgId);
	head.SetSeq(0);
	head.SetTimestamp(msgTime);
	head.SetFrom(fromUid);
	head.SetTo(targetUid);
	head.SetConversation(0);
	head.SetPrivate(privateId);

	msg.SetHead(head);
	msg.set_confId(confId);
	msg.set_confPwd(confPwd);
	msg.set_hostId(hostId);
	
	return CreateMessageRequest(msg, request);
}

int32_t MessageProcessor::CreateCallAgreeMentMessage(ConversationType type, UCID fromUid, UCID targetUid,
	const std::string& confId, int32_t hostId, HTTPRequest &request, int32_t &msgId, int64_t &msgTime) {

	AudioReceiveContentMsg msg;
	UCHead head;

	msgId = DBManager::GetInstance().GetMessageID();
	msgTime = uc::Time::GetSysTime();
	head.SetVersion(MESSAGE_VERSION);
	head.SetAppId(AppId::AppAudio);
	if (SINGLE_CHAT == type){
		head.SetPri(PriType::thrift_type);
		head.SetProtocoltype(AudioChatMessageType::DefaultType);
	}else if(PROJECT_CHAT == type){
		head.SetPri(PriType::thrift_type | PriType::group_type);
		head.SetProtocoltype(AudioChatMessageType::DiscussionType);
	}else if(GROUP_CHAT == type){
		head.SetPri(PriType::thrift_type | PriType::group_type);
		head.SetProtocoltype(AudioChatMessageType::GroupType);
	}
	head.SetProtocolId(AudioChatMessageId::AudioReceive);
	head.SetChannelPri(ChannelPriType::channel0);
	head.SetControlPri(0);
	head.SetId(msgId);
	head.SetSeq(0);
	head.SetTimestamp(msgTime);
	head.SetFrom(fromUid);
	head.SetTo(targetUid);
	head.SetConversation(0);

	msg.SetHead(head);
	msg.set_confId(confId);
	msg.set_hostId(hostId);
	
	return CreateMessageRequest(msg, request);
}

int32_t MessageProcessor::CreateCallRejectMessage(ConversationType type, UCID fromUid, UCID targetUid,
	const std::string& confId, int32_t hostId, const std::string& reason, 
	HTTPRequest &request, int32_t &msgId, int64_t &msgTime) {
	
	AudioRejectContentMsg msg;
	UCHead head;
	
	msgId = DBManager::GetInstance().GetMessageID();
	msgTime = uc::Time::GetSysTime();
	head.SetVersion(MESSAGE_VERSION);
	head.SetAppId(AppId::AppAudio);
	if (SINGLE_CHAT == type){
		head.SetPri(PriType::thrift_type);
		head.SetProtocoltype(AudioChatMessageType::DefaultType);
	}else if(PROJECT_CHAT == type){
		head.SetPri(PriType::thrift_type | PriType::group_type);
		head.SetProtocoltype(AudioChatMessageType::DiscussionType);
	}else if(GROUP_CHAT == type){
		head.SetPri(PriType::thrift_type | PriType::group_type);		
		head.SetProtocoltype(AudioChatMessageType::GroupType);
	}
	head.SetControlPri(0);
	head.SetProtocolId(AudioChatMessageId::AudioReject);
	head.SetChannelPri(ChannelPriType::channel0);
	head.SetId(msgId);
	head.SetSeq(0);
	head.SetTimestamp(msgTime);
	head.SetFrom(fromUid);
	head.SetTo(targetUid);
	head.SetConversation(0);

	msg.SetHead(head);
	msg.set_confId(confId);
	msg.set_hostId(hostId);
	msg.set_reason(reason);
	
	return CreateMessageRequest(msg, request);
}

int32_t MessageProcessor::CreateCallFinishMessage(ConversationType type, UCID fromUid, UCID targetUid,
	const std::string& confId, int32_t hostId, HTTPRequest &request, int32_t &msgId, int64_t &msgTime) {
	
	AudioStopContentMsg msg;
	UCHead head;
	
	msgId = DBManager::GetInstance().GetMessageID();
	msgTime = uc::Time::GetSysTime();
	head.SetVersion(MESSAGE_VERSION);
	head.SetAppId(AppId::AppAudio);
	if (SINGLE_CHAT == type){
		head.SetPri(PriType::thrift_type);
		head.SetProtocoltype(AudioChatMessageType::DefaultType);
	}else if(PROJECT_CHAT == type){
		head.SetPri(PriType::thrift_type | PriType::group_type);
		head.SetProtocoltype(AudioChatMessageType::DiscussionType);
	}else if(GROUP_CHAT == type){
		head.SetPri(PriType::thrift_type | PriType::group_type);
		head.SetProtocoltype(AudioChatMessageType::GroupType);
	}
	head.SetProtocolId(AudioChatMessageId::AudioStop);
	head.SetChannelPri(ChannelPriType::channel0);
	head.SetControlPri(0);
	head.SetId(msgId);
	head.SetSeq(0);
	head.SetTimestamp(msgTime);
	head.SetFrom(fromUid);
	head.SetTo(targetUid);
	head.SetConversation(0);

	msg.SetHead(head);
	msg.set_confId(confId);
	msg.set_hostId(hostId);
	
	return CreateMessageRequest(msg, request);
}

int32_t MessageProcessor::CreateCallCancelMessage(ConversationType type, UCID fromUid, UCID targetUid, const string& phoneNum, HTTPRequest &request, int32_t &msgId, int64_t &msgTime) {
	
	AudioCancelCallContentMsg msg;
	UCHead head;
	
	msgId = DBManager::GetInstance().GetMessageID();
	msgTime = uc::Time::GetSysTime();
	head.SetVersion(MESSAGE_VERSION);
	head.SetAppId(AppId::AppAudio);
	if (SINGLE_CHAT == type){
		head.SetPri(PriType::thrift_type);
		head.SetProtocoltype(AudioChatMessageType::DefaultType);
	}else if(PROJECT_CHAT == type){
		head.SetPri(PriType::thrift_type | PriType::group_type);
		head.SetProtocoltype(AudioChatMessageType::DiscussionType);
	}else if(GROUP_CHAT == type){
		head.SetPri(PriType::thrift_type | PriType::group_type);
		head.SetProtocoltype(AudioChatMessageType::GroupType);
	}
	head.SetControlPri(0);
	head.SetProtocolId(AudioChatMessageId::AudioCancelCall);
	head.SetChannelPri(ChannelPriType::channel0);
	head.SetId(msgId);
	head.SetSeq(0);
	head.SetTimestamp(msgTime);
	head.SetFrom(fromUid);
	head.SetTo(targetUid);
	head.SetConversation(0);

	msg.SetHead(head);
	msg.set_phoneNum(phoneNum);
	
	return CreateMessageRequest(msg, request);
}

int32_t MessageProcessor::CreateNotRespondCallRecordMessage(ConversationType type, UCID fromUid, UCID targetUid, const std::string& phoneNum, HTTPRequest &request, int32_t &msgId, int64_t &msgTime) {

	AudioBlockCallContentMsg msg;
	UCHead head;
	
	msgId = DBManager::GetInstance().GetMessageID();
	msgTime = uc::Time::GetSysTime();
	head.SetVersion(MESSAGE_VERSION);
	head.SetAppId(AppId::AppAudio);
	if (SINGLE_CHAT == type){
		head.SetPri(PriType::thrift_type);
		head.SetProtocoltype(AudioChatMessageType::DefaultType);
	}else if(PROJECT_CHAT == type){
		head.SetPri(PriType::thrift_type | PriType::group_type);
		head.SetProtocoltype(AudioChatMessageType::DiscussionType);
	}else if(GROUP_CHAT == type){
		head.SetPri(PriType::thrift_type | PriType::group_type);
		head.SetProtocoltype(AudioChatMessageType::GroupType);
	}
	head.SetControlPri(0);
	head.SetProtocolId(AudioChatMessageId::AudioBlockCall);
	head.SetChannelPri(ChannelPriType::channel0);
	head.SetId(msgId);
	head.SetSeq(0);
	head.SetTimestamp(msgTime);
	head.SetFrom(fromUid);
	head.SetTo(targetUid);
	head.SetConversation(0);

	msg.SetHead(head);
	msg.set_phoneNum(phoneNum);
	
	return CreateMessageRequest(msg, request);
}

int32_t MessageProcessor::CreateRejectCallRecordMessage(ConversationType type, UCID fromUid, UCID targetUid, const string& phoneNum, HTTPRequest &request, int32_t &msgId, int64_t &msgTime) {

	AudioRejectCallContentMsg msg;
	UCHead head;

	msgId = DBManager::GetInstance().GetMessageID();
	msgTime = uc::Time::GetSysTime();
	head.SetVersion(MESSAGE_VERSION);
	head.SetAppId(AppId::AppAudio);
	if (SINGLE_CHAT == type){
		head.SetPri(PriType::thrift_type);
		head.SetProtocoltype(AudioChatMessageType::DefaultType);
	}else if(PROJECT_CHAT == type){
		head.SetPri(PriType::thrift_type | PriType::group_type);
		head.SetProtocoltype(AudioChatMessageType::DiscussionType);
	}else if(GROUP_CHAT == type){
		head.SetPri(PriType::thrift_type | PriType::group_type);
		head.SetProtocoltype(AudioChatMessageType::GroupType);
	}
	head.SetProtocolId(AudioChatMessageId::AudioRejectCall);
	head.SetChannelPri(ChannelPriType::channel0);
	head.SetControlPri(0);
	head.SetId(msgId);
	head.SetSeq(0);
	head.SetTimestamp(msgTime);
	head.SetFrom(fromUid);
	head.SetTo(targetUid);
	head.SetConversation(0);

	msg.SetHead(head);
	msg.set_phoneNum(phoneNum);

	return CreateMessageRequest(msg, request);
}

int32_t MessageProcessor::CreateConnectedCallRecordMessage(ConversationType type, UCID fromUid, UCID targetUid, 
	int32_t druation, const string& phoneNum, HTTPRequest &request, int32_t &msgId, int64_t &msgTime) {

	AudioRecentCallContentMsg msg;
	UCHead head;

	msgId = DBManager::GetInstance().GetMessageID();
	msgTime = uc::Time::GetSysTime();
	head.SetVersion(MESSAGE_VERSION);
	head.SetAppId(AppId::AppAudio);
	if (SINGLE_CHAT == type){
		head.SetPri(PriType::thrift_type);
		head.SetProtocoltype(AudioChatMessageType::DefaultType);
	}else if(PROJECT_CHAT == type){
		head.SetPri(PriType::thrift_type | PriType::group_type);
		head.SetProtocoltype(AudioChatMessageType::DiscussionType);
	}else if(GROUP_CHAT == type){
		head.SetPri(PriType::thrift_type | PriType::group_type);
		head.SetProtocoltype(AudioChatMessageType::GroupType);
	}
	head.SetControlPri(ControlPriType::unread_count_type | ControlPriType::display_type);
	head.SetProtocolId(AudioChatMessageId::AudioRecentCall);
	head.SetChannelPri(0x0001);
	head.SetId(msgId);
	head.SetSeq(0);
	head.SetTimestamp(msgTime);
	head.SetFrom(fromUid);
	head.SetTo(targetUid);
	head.SetConversation(0);

	msg.SetHead(head);
	msg.set_duration(druation);
	msg.set_phoneNum(phoneNum);

	BinMsg data;
	string body;
	if (EncodeMessageData(msg, data, body) != UC_SUCCEEDED) {
		UC_LOG(ERROR) << "EncodeMessageData. relate_id: " << targetUid.GetUserId();
		return UC_FAILED;
	}

	if (SaveMessage(type, targetUid.GetUserId(), 0, 0, STATE_SENDING, &msg, body) != UC_SUCCEEDED){
		UC_LOG(ERROR) << "save message in cache failed. relate_id: " << targetUid.GetUserId() << ", type:" << type;
		return UC_FAILED;
	}

	return CreateMessageRequest(msg, request);
}

int32_t MessageProcessor::CreateMessageReadMessage(ConversationType type, UCID fromUid, UCID targetUid, int32_t msgId, int64_t msgSeq, HTTPRequest &request){

	AckReadMsg msg;
	UCHead head;

	msgId = DBManager::GetInstance().GetMessageID();
	head.SetVersion(MESSAGE_VERSION);
	head.SetAppId(AppId::AppAck);
	if (type == SINGLE_CHAT || type == OPENAPI_CHAT){
		head.SetPri(PriType::thrift_type);
	}
	if (type == GROUP_CHAT || type == PROJECT_CHAT || type == CONFERENCE_CHAT){
		head.SetPri(PriType::thrift_type | PriType::group_type);
	}
	head.SetProtocoltype(AckMessageType::DefaultType);
	head.SetProtocolId(AckMessageID::AckRead);
	head.SetChannelPri(ChannelPriType::channel0);
	head.SetControlPri(0);
	head.SetId(msgId);
	head.SetSeq(msgSeq);
	head.SetTimestamp(uc::Time::GetSysTime());
	head.SetFrom(fromUid);
	head.SetTo(targetUid);
	head.SetConversation(0);

	msg.SetHead(head);
	msg.set_id(msgId);
	msg.set_seq(msgSeq);
	switch (type)
	{
	case CONFERENCE_CHAT:
		msg.set_sessionType(SessionType::ConferenceType);
		break;
	case PROJECT_CHAT:
		msg.set_sessionType(SessionType::DiscussionChat);
		break;
	case GROUP_CHAT:
		msg.set_sessionType(SessionType::GroupChat);
		break;
	case SINGLE_CHAT:
		msg.set_sessionType(SessionType::SingleChat);
		break;
	case OPENAPI_CHAT:
		msg.set_sessionType(SessionType::APIType);
		break;
	case ORG_CHAT:
		msg.set_sessionType(SessionType::OrganizationType);
		break;
	default:
		break;
	}

	return CreateMessageRequest(msg, request);
}

int32_t MessageProcessor::CreateMessagePlayedMessage(ConversationType type, UCID fromUid, UCID targetUid, int32_t msgId, int64_t msgSeq, HTTPRequest &request){

	AckReadMsg msg;
	UCHead head;

	msgId = DBManager::GetInstance().GetMessageID();
	head.SetVersion(MESSAGE_VERSION);
	head.SetAppId(AppId::AppAck);
	head.SetProtocoltype(AckMessageType::DefaultType);
	head.SetProtocolId(AckMessageID::AckPlay);
	head.SetChannelPri(ChannelPriType::channel0);
	head.SetControlPri(0);
	head.SetId(msgId);
	head.SetSeq(msgSeq);
	head.SetTimestamp(uc::Time::GetSysTime());
	head.SetFrom(fromUid);
	head.SetTo(targetUid);
	head.SetConversation(0);

	msg.SetHead(head);
	msg.set_id(msgId);
	msg.set_seq(msgSeq);

	return CreateMessageRequest(msg, request);
}

int32_t MessageProcessor::EncodeMessageData(UCMsg &msg, BinMsg &data, string &body) {
	int32_t result = UC_FAILED;

	result = MsgParser::EncodeMessage(&msg, &data);
	if ((ParserError::PARSER_SUCCESS != result)) {
		UC_LOG(ERROR) << "msgparser encode message error: " << result;
		return UC_FAILED;
	}

	if (!StringConvert::Base64Encode(data._msg + (data._msgLen - msg.GetBodyLen()), msg.GetBodyLen(), body)) {
		UC_LOG(ERROR) << "base64encode error";
		return result;
	}

	return UC_SUCCEEDED;
}

int32_t MessageProcessor::CreateMessageRequest(UCMsg &msg, HTTPRequest &request) {

	BinMsg destMsg;
	int32_t result = 0;

	result = MsgParser::EncodeMessage(&msg, &destMsg);
	if ((ParserError::PARSER_SUCCESS != result)) {
		UC_LOG(ERROR) << "msgparser encode message error: " << result;
		return UC_FAILED;
	}

	return CreateMessageRequest(destMsg, request);
}

int32_t MessageProcessor::CreateMessageRequest(BinMsg &destMsg, HTTPRequest &request){
	
	int32_t result = 0;
	unsigned char *msgData = NULL;
	int32_t msgDataLen = destMsg._msgLen + 4;

	msgData = new unsigned char[msgDataLen];
	memcpy(msgData, &destMsg._msgLen, 4);
	memcpy(msgData + 4, destMsg._msg, destMsg._msgLen);

	unsigned long zipDataLen = msgDataLen;
	unsigned char *zipData = new unsigned char[zipDataLen];
	result = ZipWrapper::Compress(msgData, msgDataLen, zipData, &zipDataLen);
	if (0 != result) {
		UC_LOG(ERROR) << "gzip compress error: " << result;
		delete []msgData;
		msgData = NULL;
		delete []zipData;
		zipData = NULL;
		return UC_FAILED;
	}

	stringstream sStream;
	ClientService *clientService = (ClientService*)_engine->QueryService(UCClientService);

	assert(clientService != NULL);
	sStream << UCEngine::GetRequestURL(clientService->GetLoginInfo().GetUccServerDomain(), MESSAGE_SEND_METHOD) <<
		"?session_id=" << clientService->GetLoginInfo().GetSessionId() <<
		"&user_id=" << clientService->GetLoginInfo().GetUserId();

	request.Init(sStream.str(), HTTP_METHOD_POST);
	request.AddHeaderInfo("Content-Type", "application/x-www-form-urlencoded");
	request.AddHeaderInfo("Content-Encoding", "gzip");
	request.SetBody(string((char*)zipData, zipDataLen));

	delete []msgData;
	msgData = NULL;
	delete []zipData;
	zipData = NULL;
	return UC_SUCCEEDED;
}

MessageInfo* MessageProcessor::GetMessageInfo(boost::property_tree::wptree jsonData){

	UCHead head;

	head.SetId(jsonData.get<int32_t>(L"id"));
	head.SetSeq(jsonData.get<int64_t>(L"seq"));
	head.SetTimestamp(jsonData.get<int64_t>(L"timestamp"));
	head.SetAppId(jsonData.get<int32_t>(L"app_id"));
	head.SetProtocoltype(jsonData.get<int32_t>(L"protocol_type"));
	head.SetProtocolId(jsonData.get<int32_t>(L"protocol_id"));
	head.SetVersion(jsonData.get<int32_t>(L"msg_version", 0));
	head.SetFrom(UCID(jsonData.get<int32_t>(L"from_user_id"),
		jsonData.get<int32_t>(L"from_site_id"),
		jsonData.get<int32_t>(L"from_res_id")));

	head.SetTo(UCID(jsonData.get<int32_t>(L"to_user_id"),
		jsonData.get<int32_t>(L"to_site_id"),
		jsonData.get<int32_t>(L"to_res_id")));

	head.SetConversation(jsonData.get<int32_t>(L"conversation"));

	//搜索返回的content是明文
	string content = StringConvert::ToUTF8Str(jsonData.get<wstring>(L"content", L""));
	MessageInfo *message = NULL;

	if ( (head.GetAppId() == 1 && head.GetProtocoltype() == 1 && head.GetProtocolId() == 1) //UCNormalChatText
		|| (head.GetAppId() == 1 && head.GetProtocoltype() == 2 && head.GetProtocolId() == 1) //UCDiscussionChatText
		|| (head.GetAppId() == 1 && head.GetProtocoltype() == 3 && head.GetProtocolId() == 1)  //UCGroupChatText
		|| (head.GetAppId() == 1 && head.GetProtocoltype() == 4 && head.GetProtocolId() == 1)  //UCCloudChatText 
		) {
		message = new TextMessageInfo();

		((TextMessageInfo*)message)->InfoFromUCHead(head);
		((TextMessageInfo*)message)->SetTextContent(content);

	} else if (head.GetAppId() == 7 && head.GetProtocoltype() == 9 && head.GetProtocolId() == 1) { //UCMeetingChatText
		message = new ConfTextMessageInfo();

		((ConfTextMessageInfo*)message)->InfoFromUCHead(head);
		((ConfTextMessageInfo*)message)->SetTextContent(content);
	}  else {
		UC_LOG(INFO) << "appid: " << head.GetAppId() << ", protocoltype:" << head.GetProtocoltype() << "protocolId:" << head.GetProtocolId();
	}

	return message;
}

int32_t MessageProcessor::GetUCMsg(UCHead &head, const string &body, UCMsg *&msg) {
	std::string data;
	int32_t dataLen = 0;

	// base64 decode
	if (!StringConvert::Base64Decode(body.data(), body.length(), data ) ) {
		UC_LOG(ERROR) << "Base64Decode error:" << body;
		return UC_FAILED;
	}

	// base64 decode
// 	dataLen = body.length();
// 	data = StringConvert::Base64Decode(body.c_str(), dataLen);
// 	if (data == NULL) {
// 		UC_LOG(ERROR) << "Base64Decode error:" << body;
// 		return UC_FAILED;
// 	}

	// thrift decode
	int32_t result = MsgParser::DecodeBody(head, (char*)data.c_str(), data.length(), msg);

	if (result != 0) {
		UC_LOG(ERROR) << "MsgParser:DecodeMessage error:" << result << "; content: " << body;
		return UC_FAILED;
	}
	return UC_SUCCEEDED;
}

void MessageProcessor::GetIdsFromUCMsg(const UCMsg &msg, int64_t &eventId, int64_t &conferenceId, int64_t &authorizerId) {

	int32_t type = msg.GetMsgType();

	eventId = -1;
	conferenceId = 0;
	authorizerId = 0;

	if (type == UCMeetingConfInviteIQInvite || type == UCMeetingConfUpdateIQInvite || type == UCMeetingConfForwardIQInvite) {
		IQInviteContentMsg &inviteMsg = (IQInviteContentMsg&)msg;
		eventId = inviteMsg.get_eventId();
		conferenceId = inviteMsg.get_confId();
		authorizerId = inviteMsg.get_shareUserID();

	} else if (type == UCMeetingConfInviteIQReceive || type == UCMeetingConfInviteIQReject) {
		IQReceiveContentMsg &receiveMsg = (IQReceiveContentMsg&)msg;
		eventId = receiveMsg.get_eventId();
		conferenceId = receiveMsg.get_confId();
		authorizerId = receiveMsg.get_shareUserID();

	} else if (type == UCMeetingConfCancel) {
		ConfCancelContentMsg &cancelMsg = (ConfCancelContentMsg&)msg;
		eventId = cancelMsg.get_eventId();
		conferenceId = cancelMsg.get_confId();
		authorizerId = cancelMsg.get_shareUserID();

	} else if (type == UCMeetingSummaryCreate || type == UCMeetingSummaryUpdate) {
		ConfSummaryContentMsg &summaryMsg = (ConfSummaryContentMsg&)msg;
		eventId = summaryMsg.get_eventId();
		conferenceId = summaryMsg.get_confId();

	} else if (type == UCMeetingChatText || type == UCMeetingChatMedia || type == UCMeetingRevocationMessage) {
		ConfChatContentMsg &chatMsg = (ConfChatContentMsg&)msg;
		eventId = chatMsg.get_eventId();
		conferenceId = chatMsg.get_confId();
	}
	else if (type == UCMeetingFileUpload)
	{
		ConfUploadContentMsg &uploadMsg = (ConfUploadContentMsg&)msg;
		eventId = uploadMsg.get_eventId();
	}
	else if (type == UCMeetingDelDoc)
	{
		ConfDelDocContentMsg &delMsg = (ConfDelDocContentMsg&)msg;
		eventId = delMsg.get_eventId();
	}
	else {
		assert(true);
		UC_LOG(INFO) << "GetIdsFromUCMsg: " << type;
		conferenceId = -2;
	}
}


int32_t MessageProcessor::GetMessageFromJSON(boost::property_tree::wptree jsonData, UCMsg *&msg, string &body) {

	UCHead head;

	head.SetId(jsonData.get<int32_t>(L"id"));
	head.SetSeq(jsonData.get<int64_t>(L"seq"));
	head.SetTimestamp(jsonData.get<int64_t>(L"timestamp"));
	head.SetAppId(jsonData.get<int32_t>(L"app_id"));
	head.SetProtocoltype(jsonData.get<int32_t>(L"protocol_type"));
	head.SetProtocolId(jsonData.get<int32_t>(L"protocol_id"));
	head.SetVersion(jsonData.get<int32_t>(L"msg_version", 0));
	head.SetFrom(UCID(jsonData.get<int32_t>(L"from_user_id"),
			jsonData.get<int32_t>(L"from_site_id"),
			jsonData.get<int32_t>(L"from_res_id")));

	head.SetTo(UCID(jsonData.get<int32_t>(L"to_user_id"),
			jsonData.get<int32_t>(L"to_site_id"),
			jsonData.get<int32_t>(L"to_res_id")));

	head.SetConversation(jsonData.get<int32_t>(L"conversation", 0));
	try{
		head.SetPri(jsonData.get<int32_t>(L"pri"));
	}
	catch (boost::property_tree::ptree_error & e){
		UC_LOG(ERROR) << "encounter an exception when parse json from GetMessageFromJSON , exception: " << e.what();
		if (jsonData.get<int32_t>(L"is_group") == 1){
			head.SetPri(PriType::thrift_type | PriType::group_type);
		}else{
			head.SetPri(PriType::thrift_type);
		}
	}
	
	//head.SetChannelPri(jsonData.get<int32_t>(L"channelpri"));
	//head.SetControlPri(jsonData.get<int32_t>(L"controlPri"));

	body = StringConvert::ToUTF8Str(jsonData.get<wstring>(L"content", L""));

	return MessageProcessor::GetUCMsg(head, body, msg);
}

int32_t MessageProcessor::SaveMessage(ConversationType type, int64_t relateId, int32_t flag, int32_t isPlayed,
	MsgSendState state, UCMsg *msg, const string &body) {

	if ( NULL == msg ){
		return UC_FAILED;
	}

	UCHead head;
	msg->GetHead( head );

	// store message
	MessageStoreItem msg_item(false);
	msg_item.msg		= msg;
	msg_item.body		= body.c_str();
	msg_item.relate_id	= relateId;
	msg_item.state     = state;
	msg_item.conversation_type	= type;
	if ( SINGLE_CHAT == type )
	{
		theStore.GetMsgStore()->SaveIMMessage(relateId, msg_item);
	}
	else if ( OPENAPI_CHAT == type )
	{
		theStore.GetMsgStore()->SaveOpenAPIMessage(relateId, msg_item);
	}
	else
	{
		theStore.GetMsgStore()->SaveGroupMessage(relateId, msg_item);
	}
	UC_LOG(INFO) << "MessageProcessor save message in cache. type: " << type << " relateId: "
		<< relateId << " content: " << body.c_str();

	// store conversation
	ConversationInfoInternal conv_info(false);
	conv_info.SetBody( body.c_str() );
	conv_info.SetLastMsgSeq( head.GetSeq() );
	conv_info.SetConversation( head.GetConversation() );
	conv_info.SetLastMsgTime( head.GetTimestamp() );
	conv_info.SetMsg( msg );
	conv_info.SetRelateId( relateId );
	conv_info.SetSendState( state );
	conv_info.SetType( type );
	theStore.GetConvStore()->SaveConversation( conv_info );

	return UC_SUCCEEDED;
}


void MessageProcessor::ClearMessageList(std::list<MessageInfo*> msgList){

	std::list<MessageInfo*>::iterator it = msgList.begin();
	for (; it != msgList.end(); ++it){
		MessageInfo *info = *it;
		delete info;
		info = NULL;
	}
	msgList.clear();
}

ConversationType MessageProcessor::GetConversationType(int32_t appId, int32_t protocolType) {

	ConversationType type = NONE_CHAT;

	if ((appId == AppId::AppChat) && (protocolType == ChatMessageType::NormalChat)) {
		type = SINGLE_CHAT;
	}
	else if ((appId == AppId::AppChat) && (protocolType == ChatMessageType::DiscussionChat)) {
		type = PROJECT_CHAT;
	}
	else if ((appId == AppId::AppChat) && (protocolType == ChatMessageType::GroupChat)) {
		type = GROUP_CHAT;
	}
	else if ((appId == AppId::AppChat) && (protocolType == ChatMessageType::CloudFileChat)) {
		type = CLOUD_CHAT;
	}
	else if (appId == AppId::AppMeeting) {
		type = CONFERENCE_CHAT;
	}
	else if (appId == AppId::AppCalendar) {
		type = SINGLE_CHAT;
	}
	else if ((appId == AppId::AppAudio) && ((protocolType == AudioChatMessageType::DefaultType) || (protocolType == AudioChatMessageType::AudioCallRecord))) {
		type = SINGLE_CHAT;
	}
	else if ((appId == AppId::AppAudio) && (protocolType == AudioChatMessageType::DiscussionType)) {
		type = PROJECT_CHAT;
	}
	else if ((appId == AppId::AppAudio) && (protocolType == AudioChatMessageType::GroupType)) {
		type = GROUP_CHAT;
	}
	else if (appId == AppId::AppOrganization) {
		type = ORG_CHAT;
	}
	else if (appId == AppId::AppAPI){
		type = OPENAPI_CHAT;
	}

	return type;
}

int64_t MessageProcessor::GetRelateId(ConversationType type, int64_t curUid, int64_t fromUid, int64_t toUid) {
	int64_t relateId = 0;

	if (type == SINGLE_CHAT) {
		if (curUid == fromUid) {
			relateId = toUid;
		} else {
			relateId = fromUid;
		}
	}
	else if (type == GROUP_CHAT || type == PROJECT_CHAT || type == CLOUD_CHAT) {
		relateId = toUid;
	} else if (type == CONFERENCE_CHAT) {
		relateId = toUid;
	} else if (type == OPENAPI_CHAT){
		if (curUid == fromUid) {
			relateId = toUid;
		}
		else {
			relateId = fromUid;
		}
	}

	return relateId;
}

int16_t MessageProcessor::GetCloudFileMessageIdByOperation(CloudFileOperation operation) {
	switch (operation) {
	case CFO_CloudFileCreate:
		return CloudFileMessageId::CloudFileCreate;
	case CFO_CloudFileUpdate:
		return CloudFileMessageId::CloudFileUpdate;
	case CFO_CloudFileDel:
		return CloudFileMessageId::CloudFileDel;
	case CFO_CloudFileMove:
		return CloudFileMessageId::CloudFileMove;
	case CFO_CloudFileRename:
		return CloudFileMessageId::CloudFileRename;
	default:
		return CloudFileMessageId::CloudFileCreate;
	}
}
int32_t MessageProcessor::CreateAssistInvitationMessage(ConversationType type, UCID fromUid, UCID targetUid, const std::string& confId, const std::string& confPwd, OUT HTTPRequest &request, OUT int32_t &msgId, OUT int64_t &msgTime) {
	RemoteAssistanceInviteContentMsg msg;
	UCHead head;

	msgId = DBManager::GetInstance().GetMessageID();
	msgTime = uc::Time::GetSysTime();
	head.SetVersion(MESSAGE_VERSION);
	head.SetAppId(AppId::AppRemoteAssistance);
	if (SINGLE_CHAT == type){
		head.SetPri(PriType::thrift_type);
		head.SetProtocoltype(RemoteAssistanceMessageType::DefaultType);
		head.SetControlPri(0);
	}

	head.SetProtocolId(RemoteAssistanceMessageId::RemoteAssistanceInvite);
	head.SetChannelPri(ChannelPriType::channel0);
	head.SetId(msgId);
	head.SetSeq(0);
	head.SetTimestamp(msgTime);
	head.SetFrom(fromUid);
	head.SetTo(targetUid);
	head.SetConversation(0);

	msg.SetHead(head);
	msg.set_confId(confId);
	msg.set_confPwd(confPwd);

	return CreateMessageRequest(msg, request);
}

int32_t MessageProcessor::CreateAssistAcceptMessage(ConversationType type, UCID fromUid, UCID targetUid, const std::string& confId, OUT HTTPRequest &request, OUT int32_t &msgId, OUT int64_t &msgTime) {
	RemoteAssistanceAcceptContentMsg msg;
	UCHead head;

	msgId = DBManager::GetInstance().GetMessageID();
	msgTime = uc::Time::GetSysTime();
	head.SetVersion(MESSAGE_VERSION);
	head.SetAppId(AppId::AppRemoteAssistance);
	if (SINGLE_CHAT == type){
		head.SetPri(PriType::thrift_type);
		head.SetProtocoltype(RemoteAssistanceMessageType::DefaultType);
		head.SetControlPri(0);
	}

	head.SetProtocolId(RemoteAssistanceMessageId::RemoteAssistanceAccept);
	head.SetChannelPri(ChannelPriType::channel0);
	head.SetId(msgId);
	head.SetSeq(0);
	head.SetTimestamp(msgTime);
	head.SetFrom(fromUid);
	head.SetTo(targetUid);
	head.SetConversation(0);

	msg.SetHead(head);
	msg.set_confId(confId);

	return CreateMessageRequest(msg, request);
}

int32_t MessageProcessor::CreateAssistRejectMessage(ConversationType type, UCID fromUid, UCID targetUid, const std::string& confId, OUT HTTPRequest &request, OUT int32_t &msgId, OUT int64_t &msgTime) {
	RemoteAssistanceRejectContentMsg msg;
	UCHead head;

	msgId = DBManager::GetInstance().GetMessageID();
	msgTime = uc::Time::GetSysTime();
	head.SetVersion(MESSAGE_VERSION);
	head.SetAppId(AppId::AppRemoteAssistance);
	if (SINGLE_CHAT == type){
		head.SetPri(PriType::thrift_type);
		head.SetProtocoltype(RemoteAssistanceMessageType::DefaultType);
		head.SetControlPri(0);
	}

	head.SetProtocolId(RemoteAssistanceMessageId::RemoteAssistanceReject);
	head.SetChannelPri(ChannelPriType::channel0);
	head.SetId(msgId);
	head.SetSeq(0);
	head.SetTimestamp(msgTime);
	head.SetFrom(fromUid);
	head.SetTo(targetUid);
	head.SetConversation(0);

	msg.SetHead(head);
	msg.set_confId(confId);

	return CreateMessageRequest(msg, request);
}

//在文件下载存入数据库时已使用此方法，后期优化可以考虑使用公共接口
int32_t MessageProcessor::_GetLocalFilePathAndSize(const std::string &fileURL, std::string &filePath, int64_t &fileSize){
	return DBManager::GetInstance().GetFSLocalPathAndSize(fileURL, filePath, fileSize);
}

//在文件下载存入数据库时已使用此方法，后期优化可以考虑使用公共接口
int32_t MessageProcessor::CheckFile(const std::string &filePath, int64_t fileSize){
	if (fileSize == 0) {
		return FT_ERROR_FILE_SIZE;
	}

	try
	{
		if (!filehelp::exists(filePath)) {
			UC_LOG(INFO) << "GroupService::CheckFile error. file : '" << filePath << "' is not exist.";
			return FT_ERROR_FILE_NOT_EXIST;
		}

		int64_t size = filehelp::file_size(filePath);
		if (size != fileSize) {
			return FT_ERROR_FILE_SIZE;
		}
	}
	catch (fs::filesystem_error e) {
		boost::system::error_code code = e.code();
		UC_LOG(ERROR) << "GroupService::CheckFile， encounter an exception when file operator , exception: error code: "
			<< code.value() << ", error message:" << e.what();
		return FT_ERROR_FILE_CAN_NOT_OPERATE;
	}

	return UC_SUCCEEDED;
}

//在文件下载存入数据库时已使用此方法，后期优化可以考虑使用公共接口
std::string MessageProcessor::RetrieveNoUserDownloadURL(const std::string &fileurl){
	//去掉原来的userId
	std::string url = fileurl;
	int32_t pos = url.find("&u=");
	if (-1 != pos) {
		url = url.substr(0, pos);
	}
	return url;
}

} /* namespace uc */
