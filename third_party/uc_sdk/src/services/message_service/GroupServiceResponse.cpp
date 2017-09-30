/*
 * GroupService.cpp
 *
 *  Created on: Jun 4, 2015
 *      Author: yongshuai.cheng
 */

#include "GroupService.h"
#define BOOST_SPIRIT_THREADSAFE
#include <boost/foreach.hpp>
#include "boost/property_tree/json_parser.hpp"
#include "MessageProcessor.h"
#include "UCEngine.h"
#include "common/StringConvert.h"
#include "MessageParser.h"
#include "DBManager.h"
#include "client_service/ClientService.h"
#include "UCErrorCode.h"
#include "store_service/StoreService.h"
#include "rapidjson/document.h"


namespace uc {


int32_t GroupService::OnGetChatHistoryResponse(int32_t errorCode, HTTPResponse *response, int64_t groupId, MessageChannel channel, 
		ConversationType type, int64_t count, int64_t uistartTime, int64_t uiendTime, int64_t sdkstarttime, int64_t sdkendtime, int32_t update_flag_msgid){

	UC_LOG(INFO) << "OnGetChatHistoryResponse GroupId:" << groupId << 
		" errorCode:" << errorCode << " response:" << (response != NULL ? response->GetContent() : "");
	
	int32_t msgCount = 0;
	int32_t serverCount = 0;
	int32_t result = UC_FAILED;
	MessageProcessor msgProcessor((UCEngine*)_engine);
	std::list<int64_t> seqList;
	std::list<MessageStoreItem*> msgList;
	if (UC_SUCCEEDED == errorCode && NULL != response && 200 == response->GetStatusCode()) {
		wstringstream wsStream;
		wsStream.str(StringConvert::FromUTF8Str(response->GetContent().c_str()));
		boost::property_tree::wptree jsonRoot;
		boost::property_tree::wptree jsonChild;
		try{
			boost::property_tree::json_parser::read_json(wsStream, jsonRoot);
			result = jsonRoot.get<int32_t>(L"code");
		}
		catch (boost::property_tree::ptree_error & e) {
			UC_LOG(ERROR) << "encounter an exception when parse json from OnGetChatHistoryResponse response, exception: " << e.what();
			result = JSON_PARSE_ERROR;
		}
		if(0 == result) {
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
						int64_t relateId = MessageProcessor::GetRelateId(MessageProcessor::GetConversationType(head.GetAppId(), head.GetProtocoltype()), GetCurUser().GetUserId(), head.GetFrom().GetUserId(), head.GetTo().GetUserId());
						MessageStoreItem* msg_item = new MessageStoreItem();
						msg_item->msg = msg;
						msg_item->body = body.c_str();
						msg_item->relate_id = relateId;
						msg_item->state = STATE_SENDSUCCESS;
						msg_item->conversation_type = MessageProcessor::GetConversationType(head.GetAppId(), head.GetProtocoltype());
						msgList.push_back(msg_item);

						//获取当前用户发送消息的未读用户个数;
						if (head.GetFrom().GetUserId() == GetCurUser().GetUserId()){
							MessageReadUserInfo info;
							info.SetConversationType(type);
							info.SetRelateId(relateId);
							info.SetMsgSeq(head.GetSeq());
							info.SetUnreadCount(jsonElement.get<int32_t>(L"unread_user_count", -1));
							if (info.GetUnreadCount() == MESSAGE_UNREAD_NONE){
								info.SetAllRead(true);
							}
							
							theStore.GetMessageReadStore()->SaveMessageReadInfo(ConvKey(type, relateId), info);							
						}
					}
					else{
						UC_LOG(ERROR) << "why parse a message json to thrift data return error";
						result = UC_JSON_PARSE_ONE_DATA_ERROR;
					}
				}
				catch (boost::property_tree::ptree_error & e){
					UC_LOG(ERROR) << "encounter an exception when parse json from OnGetChatHistoryResponse single meessage, exception: " << e.what();
					result = UC_JSON_PARSE_ONE_DATA_ERROR;
				}
			}
		}
	}else{
		result = errorCode != UC_SUCCEEDED ? errorCode : (NULL != response && 200 != response->GetStatusCode() ?  response->GetStatusCode() : UC_FAILED);
	}

	if (UC_SUCCEEDED == result || UC_JSON_PARSE_ONE_DATA_ERROR == result){
		bool ret = false;
		// 如果UCC返回数据解析失败，不做本地同步;
		if (UC_SUCCEEDED == result){
			ret = theStore.GetMsgStore()->SyncMessages(groupId, type, count, uistartTime, uiendTime, update_flag_msgid, msgList, (uiendTime == sdkendtime));
			if (!ret){
				result = UCAS_MESSAGE_SYNC_LOCAL_FAILED;
			}
		}
		// 本地同步成功或者UCC返回数据解析失败，把有效数据提供给UI;
		if ((ret || UC_JSON_PARSE_ONE_DATA_ERROR == result) || UCAS_MESSAGE_SYNC_LOCAL_FAILED == result){
			for ( std::list<MessageStoreItem*>::iterator iter = msgList.begin(); iter != msgList.end(); ++iter ){
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
	if (!msgList.empty()){
		MessageStoreItem* item = msgList.back();
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
		}else {
			//当SDK向Server请求时间不是0，则消息格式返回UI请求的值，并把请求时间作为最小时间点
			if (0 != sdkstarttime){
				msgCount = count;
				min_timestamp = sdkstarttime;
			}else{
				msgCount = serverCount;
			}
		}
	}else if (UC_SUCCEEDED == result){
		msgCount = msgList.size();
	}
	//如果Server单条数据解析失败，给UI的错误码默认为成功;
	if (UC_JSON_PARSE_ONE_DATA_ERROR == result || UCAS_MESSAGE_SYNC_LOCAL_FAILED == result){
		result = UC_SUCCEEDED;
	}

	triggerEvent(IGroupServiceEvent, OnChatHistoryReceived(result, channel, type, groupId, msgCount, min_timestamp));

	if (!seqList.empty()){
		GetUnreadUserCount(type, groupId, seqList);
	}

	UC_LOG(INFO) << "group_service OnChatHistoryReceived from server. result: " << result
		<< " type: " << type << " groupId: " << groupId << " message_count:" << msgList.size() << " min_timestamp: " << min_timestamp;

	theStore.GetMsgStore()->ClearMessageList(msgList);
	return result;
}

int32_t GroupService::OnGeSearchChatHistoryResponse(int32_t errorCode, HTTPResponse *response, int64_t relateId, ConversationType type, int64_t count, int64_t uistartTime, int64_t uiendTime){

	UC_LOG(INFO) << "OnGeSearchtChatHistoryResponse relateId:" << relateId <<
		" errorCode:" << errorCode << " response:" << (response != NULL ? response->GetContent() : "");

	int32_t msgCount = 0;
	int32_t serverCount = 0;
	int32_t result = UC_FAILED;
	MessageProcessor msgProcessor((UCEngine*)_engine);
	std::list<int64_t> seqList;
	std::list<MessageStoreItem*> msgList;
	if (UC_SUCCEEDED == errorCode && NULL != response && 200 == response->GetStatusCode()) {
		wstringstream wsStream;
		wsStream.str(StringConvert::FromUTF8Str(response->GetContent().c_str()));
		boost::property_tree::wptree jsonRoot;
		boost::property_tree::wptree jsonChild;
		try{
			boost::property_tree::json_parser::read_json(wsStream, jsonRoot);
			result = jsonRoot.get<int32_t>(L"code");
		}
		catch (boost::property_tree::ptree_error & e) {
			UC_LOG(ERROR) << "encounter an exception when parse json from OnGeSearchtChatHistoryResponse response, exception: " << e.what();
			result = JSON_PARSE_ERROR;
		}
		if (0 == result) {
			UCMsg *msg = NULL;
			std::string body;
			try{
				jsonChild = jsonRoot.get_child(L"data");
			}
			catch (boost::property_tree::ptree_error & e) {
				UC_LOG(ERROR) << "encounter an exception when parse json from OnGeSearchtChatHistoryResponse response , exception: " << e.what();
				result = JSON_PARSE_ERROR;
			}
			BOOST_FOREACH(boost::property_tree::wptree::value_type& v, jsonChild){
				boost::property_tree::wptree jsonElement = v.second;
				serverCount++;
				try{
					if (msgProcessor.GetMessageFromJSON(jsonElement, msg, body) == UC_SUCCEEDED) {

						UCHead head;
						msg->GetHead(head);
						head.SetId(DBManager::GetInstance().GetMessageID());  //获取搜索消息前后的消息，不入库，该类请求过来的消息没有本地消息ID;
						msg->SetHead(head);
						int64_t relateId = MessageProcessor::GetRelateId(MessageProcessor::GetConversationType(head.GetAppId(), head.GetProtocoltype()), GetCurUser().GetUserId(), head.GetFrom().GetUserId(), head.GetTo().GetUserId());
						MessageStoreItem* msg_item = new MessageStoreItem();
						msg_item->msg = msg;
						msg_item->body = body.c_str();
						msg_item->relate_id = relateId;
						msg_item->state = STATE_SENDSUCCESS;
						msg_item->conversation_type = MessageProcessor::GetConversationType(head.GetAppId(), head.GetProtocoltype());
						msgList.push_back(msg_item);

						//获取当前用户发送消息的未读用户个数;
						if (head.GetFrom().GetUserId() == GetCurUser().GetUserId()){
							MessageReadUserInfo info;
							info.SetConversationType(type);
							info.SetRelateId(relateId);
							info.SetMsgSeq(head.GetSeq());
							info.SetUnreadCount(jsonElement.get<int32_t>(L"unread_user_count", -1));
							if (info.GetUnreadCount() == MESSAGE_UNREAD_NONE){
								info.SetAllRead(true);
							}

							theStore.GetMessageReadStore()->SaveMessageReadInfo(ConvKey(type, relateId), info);
						}
					}
				}
				catch (boost::property_tree::ptree_error & e){
					UC_LOG(ERROR) << "encounter an exception when parse json from OnGetChatHistoryResponse single meessage, exception: " << e.what();
					result = UC_JSON_PARSE_ONE_DATA_ERROR;
				}
			}
		}
	}
	else{
		result = errorCode != UC_SUCCEEDED ? errorCode : (NULL != response && 200 != response->GetStatusCode() ? response->GetStatusCode() : UC_FAILED);
	}

	if (UC_SUCCEEDED == result || UC_JSON_PARSE_ONE_DATA_ERROR == result){

		//按时间戳进行降序;
		msgList.sort(
			[&](MessageStoreItem* first, MessageStoreItem* second)->bool{
			assert(first);
			assert(first->msg);
			assert(second);
			assert(second->msg);
			UCHead first_head;
			UCHead second_Head;
			first->msg->GetHead(first_head);
			second->msg->GetHead(second_Head);
			return (first_head.GetSeq() > second_Head.GetSeq()); });

		for (std::list<MessageStoreItem*>::iterator iter = msgList.begin(); iter != msgList.end(); ++iter){
			MessageStoreItem* item = (*iter);
			triggerEventEx(IUCEngineEvent, *_engine, OnMessageReceived(SEARCH, *(item->msg), item->body, item->state));
		}
	}

	int64_t min_timestamp = uistartTime;
	int64_t max_timestamp = uiendTime;
	if (!msgList.empty()){
		MessageStoreItem* item_front = msgList.front();
		MessageStoreItem* item_end = msgList.back();
		assert(item_front);
		assert(item_front->msg);
		assert(item_end);
		assert(item_end->msg);

		UCHead head_front;
		item_front->msg->GetHead(head_front);
		max_timestamp = head_front.GetSeq();

		UCHead head_end;
		item_end->msg->GetHead(head_end);
		min_timestamp = head_end.GetSeq();
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
			//UI请求的开始时间时间不是0，则消息格式返回UI请求的值，并把请求时间作为最小时间点
			if (0 != uistartTime){
				msgCount = count;
			}
			else{
				msgCount = serverCount;
			}
		}
	}
	else if (UC_SUCCEEDED == result){
		msgCount = msgList.size();
	}
	//如果Server单条数据解析失败，给UI的错误码默认为成功;
	if (UC_JSON_PARSE_ONE_DATA_ERROR == result){
		result = UC_SUCCEEDED;
	}

	triggerEvent(IGroupServiceEvent, OnSearchChatHistoryReceived(result, type, relateId, msgCount, min_timestamp, max_timestamp));


	UC_LOG(INFO) << "group_service OnChatHistoryReceived from server. result: " << result
		<< " type: " << type << " relateId: " << relateId << " message_count:" << msgList.size() << " min_timestamp: " << min_timestamp
		<< " max_timestamp: " << max_timestamp;

	theStore.GetMsgStore()->ClearMessageList(msgList);
	return result;
}

int32_t GroupService::OnSendMessageResponse(int32_t errorCode, HTTPResponse *response, ConversationType type, int64_t groupId, int32_t msgId, bool saveUnread){

	UC_LOG(INFO) << "OnSendMessageResponse GroupId:" << groupId << " MsgId:" << msgId <<
		" errorCode:" << errorCode << " response:" << (response != NULL ? response->GetContent() : "");

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
			if (UC_SUCCEEDED == result){
				boost::property_tree::wptree jsonChild = jsonRoot.get_child(L"success_data");
				BOOST_FOREACH(boost::property_tree::wptree::value_type v, jsonChild){
					result = v.second.get<int32_t>(L"code");
					msgSeq = v.second.get<int64_t>(L"seq");
					sendTime = v.second.get<int64_t>(L"timestamp");
				}
				jsonChild = jsonRoot.get_child(L"failed_data");
				BOOST_FOREACH(boost::property_tree::wptree::value_type v, jsonChild){
					result = v.second.get<int32_t>(L"code");
					msgSeq = v.second.get<int64_t>(L"seq");
					sendTime = v.second.get<int64_t>(L"timestamp");
				}
			}
		}catch (boost::property_tree::ptree_error & e) {
			UC_LOG(ERROR) << "encounter an exception when parse json from OnSendMessageResponse , exception: " << e.what();
			result = JSON_PARSE_ERROR;
		}
	}else{
		result = errorCode != UC_SUCCEEDED ? errorCode : (NULL != response && 200 != response->GetStatusCode() ? response->GetStatusCode() : UC_FAILED);
	}
	
	if (UC_SUCCEEDED == result){
		theStore.GetMsgStore()->UpdateGroupMessageState( groupId, msgId, STATE_SENDSUCCESS, msgSeq);
		
	}else{
		theStore.GetMsgStore()->UpdateGroupMessageState( groupId, msgId, STATE_SENDFAIL, msgSeq);
	}
	
	triggerEvent(IGroupServiceEvent, OnMessageSent(result, type, groupId, msgId, msgSeq, sendTime));

	if (UC_SUCCEEDED == result && saveUnread){
		OnMessageReadUserNotify(type, groupId, msgSeq); 
	}

	return result;
}

int32_t GroupService::OnRevocationMessageResponse(int32_t errorCode, HTTPResponse *response, int64_t to_userId, ConversationType type, int64_t seqId, int32_t msgId)
{
	UC_LOG(INFO) << "OnRevocationMessageResponse GroupId:" << to_userId << " RevocationType:" << type << " SeqId:" << seqId <<
		" MsgId:" << msgId <<" errorCode:" << errorCode << " response:" << (response != NULL ? response->GetContent() : "");

	int32_t result = UC_FAILED;
	int64_t sendTime = seqId;//值一样
	int64_t relateId = to_userId;

	if (UC_SUCCEEDED == errorCode && NULL != response && 200 == response->GetStatusCode()) {
		wstringstream wsStream;
		wsStream.str(StringConvert::FromUTF8Str(response->GetContent()).c_str());
		boost::property_tree::wptree jsonRoot;
		try{
			boost::property_tree::json_parser::read_json(wsStream, jsonRoot);
			result = jsonRoot.get<int32_t>(L"code");
			}
		catch (boost::property_tree::ptree_error & e) {
			UC_LOG(ERROR) << "encounter an exception when parse json from OnRevocationMessageResponse , exception: " << e.what();
			result = JSON_PARSE_ERROR;
		}
	}
	else{
		result = errorCode != UC_SUCCEEDED ? errorCode : (NULL != response && 200 != response->GetStatusCode() ? response->GetStatusCode() : UC_FAILED);
	}

	if (UC_SUCCEEDED == result)
	{
		theStore.GetMsgStore()->UpdateRevocationMsgFromDB(relateId, STATE_REVOCATION, type, seqId);
		theStore.GetMsgStore()->UpdateRevocationMessageState(to_userId, sendTime, type);
	}

	triggerEvent(IGroupServiceEvent, OnRevocationMessage(result, to_userId, type, seqId, msgId, sendTime));
	return result;
}

int32_t GroupService::OnSendInviteCallMessageResponse(int32_t errorCode, HTTPResponse *response, int32_t conferenceId, int32_t callType, ConversationType type, int64_t groupId, int32_t msgId, bool saveUnread, AudioService* service) {
#ifdef AUDIO_SERVICE
	int32_t result = OnSendMessageResponse(errorCode, response, type, groupId, msgId, saveUnread);
	return service->OnInviteCallSend(result, groupId, conferenceId, callType, type);
#else
    return 0;
#endif
}

int32_t GroupService::OnSendMessageReadResponse(int32_t errorCode, HTTPResponse *response, ConversationType type, int64_t groupId,
	int32_t readType, int32_t msgId, int64_t msgSeq){
	
	UC_LOG(INFO) << "OnSendMessageReadResponse ConversationType:" << type <<" GroupId:" << groupId << " ReadType:" << readType <<
		" MsgId:" << msgId << " MsgSeq:" << msgSeq << " errorCode:" << errorCode << 
		" response:" << (response != NULL ? response->GetContent() : "");

	int32_t result = UC_FAILED;
	if (UC_SUCCEEDED  ==  errorCode && NULL != response && 200 == response->GetStatusCode()) {
		wstringstream wsStream;
		wsStream.str(StringConvert::FromUTF8Str(response->GetContent()).c_str());
		boost::property_tree::wptree jsonRoot;
		try{
			boost::property_tree::json_parser::read_json(wsStream, jsonRoot);
			result = jsonRoot.get<int32_t>(L"code");
		}catch (boost::property_tree::ptree_error & e) {
			UC_LOG(ERROR) << "encounter an exception when parse json from OnSendMessageReadResponse , exception: " << e.what();
			result = JSON_PARSE_ERROR;
		}
	}else{
		result = errorCode != UC_SUCCEEDED ? errorCode : (NULL != response && 200 != response->GetStatusCode() ? response->GetStatusCode() : UC_FAILED);
	}
	
	triggerEvent(IGroupServiceEvent, OnMessageRead(result, type, groupId, readType, msgId, msgSeq));
	return result;
}

int32_t GroupService::OnSendMessagePlayedResponse(int32_t errorCode, HTTPResponse *response, int64_t groupId, int32_t msgId, int64_t msgSeq) {

	UC_LOG(INFO) << "OnSendMessagePlayedResponse GroupId:" << groupId << " MsgId:" << msgId << 
		" MsgSeq:" << msgSeq << " errorCode:" << errorCode << " response:" << (response != NULL ? response->GetContent() : "");
	
	int32_t result = UC_FAILED;
	if (UC_SUCCEEDED  ==  errorCode && NULL != response && 200 == response->GetStatusCode()) {
		wstringstream wsStream;
		wsStream.str(StringConvert::FromUTF8Str(response->GetContent()).c_str());
		boost::property_tree::wptree jsonRoot;
		try{
			boost::property_tree::json_parser::read_json(wsStream, jsonRoot);
			result = jsonRoot.get<int32_t>(L"code");
		}catch (boost::property_tree::ptree_error & e) {
			UC_LOG(ERROR) << "encounter an exception when parse json from OnSendMessagePlayedResponse , exception: " << e.what();
			result = JSON_PARSE_ERROR;
		}
	} else {
		result = errorCode != UC_SUCCEEDED ? errorCode : (NULL != response && 200 != response->GetStatusCode() ? response->GetStatusCode() : UC_FAILED);
	}

	theStore.GetMsgStore()->UpdateGroupMessageState( groupId, msgId, MESSAGE_PLAYED, msgSeq );
	triggerEvent(IGroupServiceEvent, OnMessagePlayed(result, groupId, msgId, msgSeq));
	return result;
}

int32_t GroupService::GetGroupCreatedResponse(int32_t errorCode, HTTPResponse *response, GroupInfo& groupInfo) {
	int32_t result = UC_FAILED;
	if (UC_SUCCEEDED == errorCode && NULL != response && 200 == response->GetStatusCode()) {
		rapidjson::Document document;
		if (!document.Parse<0>(response->GetContent().c_str()).HasParseError()) {
			result = document["code"].GetInt();
			if (0 == result) {
				rapidjson::Value &data = document["data"];
				for (rapidjson::SizeType i = 0; i < data.Size(); i++) {
					ParseGroupInfo(data[i], groupInfo);

					theStore.GetGroupListStore()->UpdateGroupInfo(groupInfo);
				}
			}
		}
		else{
			UC_LOG(ERROR) << "encounter an exception when parse json from OnCreateGroupResponse , exception: " << document.GetParseError();
			result = JSON_PARSE_ERROR;
		}
	}
	else{
		result = errorCode != UC_SUCCEEDED ? errorCode : (NULL != response && 200 != response->GetStatusCode() ? response->GetStatusCode() : UC_FAILED);
	}
	return result;
}

int32_t GroupService::OnGetUnreadUserCountResponse(int32_t errorCode, HTTPResponse *response, ConversationType type, int64_t groupId, std::list<int64_t> seq){
	
	UC_LOG(INFO) << "OnGetUnreadUserCountResponse ConversationType: " << type << " groupId: " << groupId << " errorCode:"
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
					info.SetConversationType(type);
					info.SetRelateId(groupId);
					info.SetMsgSeq(StringConvert::WStringToInt64(v.first));
					info.SetUnreadCount(StringConvert::WStringToInt(v.second.get_value(v.first)));
					if (MESSAGE_UNREAD_NONE == info.GetUnreadCount()){
						info.SetAllRead(true);
					}
					infolist.push_back(info);
					theStore.GetMessageReadStore()->SaveMessageReadInfo(ConvKey(type, groupId), info);
				}
			}
		}
		catch (boost::property_tree::ptree_error & e) {
			UC_LOG(ERROR) << "encounter an exception when parse json from OnGetUnreadUserCountResponse , exception: " << e.what();
			result = JSON_PARSE_ERROR;
		}
	}
	triggerEvent(IGroupServiceEvent, OnMessageUnreadUserCount(result, type, groupId, infolist));

	return result;
}

int32_t GroupService::OnGetUnreadUserListResponse(int32_t errorCode, HTTPResponse *response, ConversationType type, int64_t groupId, std::list<int64_t> seq){
	
	UC_LOG(INFO) << "OnGetUnreadUserListResponse ConversationType: " << type << " groupId: "<< groupId << " errorCode:" 
		<< errorCode << " response:" << (response != NULL ? response->GetContent() : "");

	int32_t result = UC_FAILED;
	std::list<MessageReadUserInfo> infolist;
	if (UC_SUCCEEDED == errorCode && NULL != response && 200 == response->GetStatusCode()) {
		wstringstream wsStream;
		wsStream.str(StringConvert::FromUTF8Str(response->GetContent()).c_str());
		boost::property_tree::wptree jsonRoot;
		try{
			boost::property_tree::json_parser::read_json(wsStream, jsonRoot);
			result = jsonRoot.get<int32_t>(L"code"); 
			if (UC_SUCCEEDED == result){
				//boost::property_tree::wptree jsonChild = jsonRoot.get_child(L"data");
				BOOST_FOREACH(boost::property_tree::wptree::value_type& v, jsonRoot.get_child(L"data"))
				{
					MessageReadUserInfo info;
					info.SetConversationType(type);
					info.SetRelateId(groupId);
					info.SetMsgSeq(StringConvert::WStringToInt64(v.first));
					BOOST_FOREACH(boost::property_tree::wptree::value_type& v1, v.second)
					{
						wstring userId = v1.second.get_value(v.first);
						info.AddUser(StringConvert::WStringToInt(userId));
					}			
					info.SetUnreadCount(info.GetUser().size());
					if (info.GetUser().empty()){
						info.SetAllRead(true);
					}
					infolist.push_back(info);
					theStore.GetMessageReadStore()->SaveMessageReadInfo(ConvKey(type, groupId), info);
				}
			}
		}
		catch (boost::property_tree::ptree_error & e) {
			UC_LOG(ERROR) << "encounter an exception when parse json from OnGetUnreadUserListResponse , exception: " << e.what();
			result = JSON_PARSE_ERROR;
		}

	}
	triggerEvent(IGroupServiceEvent, OnMessageUnreadUserList(result, type, groupId, infolist));
	return result;
}

int32_t GroupService::OnCreateGroupResponse(int32_t errorCode, HTTPResponse *response, int32_t mountId){

	UC_LOG(INFO) << "OnCreateGroupResponse errorCode:" << errorCode << " response:" << (response != NULL ? response->GetContent() : "");
	GroupInfo groupInfo;
	int32_t result = GetGroupCreatedResponse(errorCode, response, groupInfo);
	if (result != UC_SUCCEEDED) {
		UC_LOG(ERROR) << "GetGroupCreatedResponse return error:" << result;
		groupInfo.SetMountId(mountId);
	}
	triggerEvent(IGroupServiceEvent, OnGroupCreated(result,	GetCurUser().GetUserId(), groupInfo));
	return result;
}

int32_t GroupService::OnCreateCallGroupResponse(int32_t errorCode, HTTPResponse *response, AudioService* service) {
	UC_LOG(INFO) << "OnCreateCallGroupResponse errorCode:" << errorCode << " response:" << (response != NULL ? response->GetContent() : "");
    
#ifdef AUDIO_SERVICE
	GroupInfo groupInfo;
	int32_t result = GetGroupCreatedResponse(errorCode, response, groupInfo);
	if (result != UC_SUCCEEDED) {
		UC_LOG(ERROR) << "GetGroupCreatedResponse return error:" << result;
	}
	return service->OnCallGroupCreated(result, groupInfo);
#else
    return 0;
#endif
}

int32_t GroupService::OnCreateCallGroupResponse(int32_t errorCode, HTTPResponse *response, 
	AudioService* service, int conferenceId, int callType) {
	UC_LOG(INFO) << "OnCreateCallGroupResponse errorCode:" << errorCode << ", conferenceId:" << conferenceId
		<< " response:" << (response != NULL ? response->GetContent() : "");

#ifdef AUDIO_SERVICE
	GroupInfo groupInfo;
	int32_t result = GetGroupCreatedResponse(errorCode, response, groupInfo);
	if (result != UC_SUCCEEDED) {
		UC_LOG(ERROR) << "GetGroupCreatedResponse return error:" << result;
	}
	return service->OnCallGroupCreated(result, groupInfo, conferenceId, callType);
#else
    return 0;
#endif
}

int32_t GroupService::OnExitGroupResponse(int32_t errorCode, HTTPResponse *response, int64_t groupId){

	UC_LOG(INFO) << "OnExitGroupResponse GroupId:" << groupId << 
		" errorCode:" << errorCode << " response:" << (response != NULL ? response->GetContent() : "");

	int32_t result = UC_FAILED;
	if (UC_SUCCEEDED == errorCode && NULL != response && 200 == response->GetStatusCode()) {
		wstringstream wsStream;
		wsStream.str(StringConvert::FromUTF8Str(response->GetContent()).c_str());
		boost::property_tree::wptree jsonRoot;
		try{
			boost::property_tree::json_parser::read_json(wsStream, jsonRoot);
			result = jsonRoot.get<int32_t>(L"code");
			if (0 == result){
				theStore.GetGroupListStore()->DeleteMember(groupId, GetCurUser().GetUserId(), true);
		}
		}catch (boost::property_tree::ptree_error & e) {
			UC_LOG(ERROR) << "encounter an exception when parse json from OnExitGroupResponse , exception: " << e.what();
			result = JSON_PARSE_ERROR;
		}
	}else{
		result = errorCode != UC_SUCCEEDED ? errorCode : (NULL != response && 200 != response->GetStatusCode() ? response->GetStatusCode() : UC_FAILED);
	}

	triggerEvent(IGroupServiceEvent, OnGroupExited(result, groupId, GetCurUser().GetUserId()));
	return result;
}

int32_t GroupService::OnEndGroupResponse(int32_t errorCode, HTTPResponse *response, int64_t groupId){

	UC_LOG(INFO) << "OnEndGroupResponse GroupId:" << groupId << 
		" errorCode:" << errorCode << " response:" << (response != NULL ? response->GetContent() : "");

	int32_t result = UC_FAILED;
	if (UC_SUCCEEDED == errorCode && NULL != response && 200 == response->GetStatusCode()) {
		wstringstream wsStream;
		wsStream.str(StringConvert::FromUTF8Str(response->GetContent()).c_str());
		boost::property_tree::wptree jsonRoot;
		try{
			boost::property_tree::json_parser::read_json(wsStream, jsonRoot);
			result = jsonRoot.get<int32_t>(L"code");
			if (0 == result){
				theStore.GetGroupListStore()->CloseGroup(groupId);
			}
		}catch (boost::property_tree::ptree_error & e) {
			UC_LOG(ERROR) << "encounter an exception when parse json from OnEndGroupResponse , exception: " << e.what();
			result = JSON_PARSE_ERROR;
		}
	}else{
		result = errorCode != UC_SUCCEEDED ? errorCode : (NULL != response && 200 != response->GetStatusCode() ? response->GetStatusCode() : UC_FAILED);
	}
	
	triggerEvent(IGroupServiceEvent, OnGroupEnded(result, groupId, GetCurUser().GetUserId()));
	return result;
}

int32_t GroupService::GetAddMemberResponse(int32_t errorCode, HTTPResponse *response, 
	int64_t groupId) {
	int32_t result = UC_FAILED;
	if (UC_SUCCEEDED == errorCode && NULL != response && 200 == response->GetStatusCode()) {
		wstringstream wsStream;
		wsStream.str(StringConvert::FromUTF8Str(response->GetContent()).c_str());
		boost::property_tree::wptree jsonData;
		try{
			boost::property_tree::json_parser::read_json(wsStream, jsonData);
			result = jsonData.get<int32_t>(L"code");
			if (0 == result){
				// 获取新增的讨论组成员信息，更新本地缓存，并响应OnMemeberAdded;
				boost::property_tree::wptree jsonRoot = jsonData.get_child(L"data");
				BOOST_FOREACH(boost::property_tree::wptree::value_type& v1, jsonRoot){
					boost::property_tree::wptree jsonChild = v1.second;
					boost::property_tree::wptree jsonElement = jsonChild.get_child(L"member_list");
					BOOST_FOREACH(boost::property_tree::wptree::value_type& v2, jsonElement){
						boost::property_tree::wptree jsonValue = v2.second;
						MemberInfo memberInfo;
						memberInfo.userId = jsonValue.get<int32_t>(L"user_id");
						memberInfo.joinTime = jsonValue.get<int32_t>(L"join_time");
						if (0 == jsonValue.get<int32_t>(L"join_state")){

							theStore.GetGroupListStore()->AddMemberInGroup(groupId, memberInfo, false);
						}
					}

					// 判断讨论组名称是否有变化，如果有变化更新本地缓存，并响应OnGroupNameUpdated事件;
					// 判断讨论组头像是否有变化，如果有变化更新本地缓存，并响应OnGroupLogoUpdated;
					if (2 == jsonChild.get<int32_t>(L"is_display")) {
						string groupName = StringConvert::ToUTF8Str(jsonChild.get<wstring>(L"group_name", L""));
						string groupPinYin = StringConvert::ToUTF8Str(jsonChild.get<wstring>(L"group_pinyin", L""));

						if (theStore.GetGroupListStore()->UpdateGroupName(groupId, groupName, groupPinYin))
							triggerEvent(IGroupServiceEvent, OnGroupNameUpdated(result, groupId, 0, groupName, groupPinYin));

						boost::property_tree::wptree jsonElement = jsonChild.get_child(L"group_logo");
						std::wstring value = L"";
						std::string avatar = ";";

						BOOST_FOREACH(boost::property_tree::wptree::value_type& v, jsonElement){
							boost::property_tree::wptree jsonValue = v.second;
							value += jsonValue.get_value<wstring>() + L",";
						}

						avatar = StringConvert::ToUTF8Str(value);
						if (theStore.GetGroupListStore()->UpdateGroupAvatarIndex(groupId, avatar))
							triggerEvent(IGroupServiceEvent, OnGroupLogoUpdated(result, groupId, avatar));
					}
				}
			}
		}
		catch (boost::property_tree::ptree_error & e) {
			UC_LOG(ERROR) << "encounter an exception when parse json from OnAddMemberResponse , exception: " << e.what();
			result = JSON_PARSE_ERROR;
		}
	}
	else{
		result = errorCode != UC_SUCCEEDED ? errorCode : (NULL != response && 200 != response->GetStatusCode() ? response->GetStatusCode() : UC_FAILED);
	}
	return result;
}

int32_t GroupService::OnAddMemberResponse(int32_t errorCode, HTTPResponse *response, int64_t groupId, std::vector<UserInfo> member){

	UC_LOG(INFO) << "OnAddMemberResponse GroupId:" << groupId << 
		" errorCode:" << errorCode << " response:" << (response != NULL ? response->GetContent() : "");

	int32_t result = GetAddMemberResponse(errorCode, response, groupId);
	triggerEvent(IGroupServiceEvent, OnMemberAdded(result, groupId, GetCurUser().GetUserId(), member));
	return result;
}

int32_t GroupService::OnAddMemberToGroupCallResponse(int32_t errorCode, HTTPResponse *response, 
	int64_t groupId, std::vector<UserInfo> member, int conferenceId, int callType, AudioService* service) {
	UC_LOG(INFO) << "OnAddMemberToGroupCallResponse, groupId:" << groupId 
		<< ", callType:" << callType << ", errcode:" << errorCode 
		<< ", response:" << (response != NULL ? response->GetContent() : "");

#ifdef AUDIO_SERVICE
	int result = GetAddMemberResponse(errorCode, response, groupId);
	UC_LOG(INFO) << "OnAddMemberToGroupCallResponse, GetAddMemberResponse return error:" << result;
	return service->OnAddMemberToGroupCall(result, groupId, conferenceId, callType, member);
#else
    return 0;
#endif
}

int32_t GroupService::OnSetAdminResponse(int32_t errorCode, HTTPResponse *response, int64_t groupId, SetAdminType type, 
	std::vector<int32_t> adminList) {
	UC_LOG(INFO) << "OnSetAdminResponse GroupId:" << groupId <<
		" errorCode:" << errorCode << " response:" << (response != NULL ? response->GetContent() : "");

	int32_t result = UC_FAILED;
	if (UC_SUCCEEDED == errorCode && NULL != response && 200 == response->GetStatusCode()) {
		wstringstream wsStream;
		wsStream.str(StringConvert::FromUTF8Str(response->GetContent()).c_str());
		boost::property_tree::wptree jsonRoot;
		try{
			boost::property_tree::json_parser::read_json(wsStream, jsonRoot);
			result = jsonRoot.get<int32_t>(L"code");
		}
		catch (boost::property_tree::ptree_error & e) {
			UC_LOG(ERROR) << "encounter an exception when parse json from OnSetAdminResponse , exception: " << e.what();
			result = JSON_PARSE_ERROR;
		}
	}
	else{
		result = errorCode != UC_SUCCEEDED ? errorCode : (NULL != response && 200 != response->GetStatusCode() ? response->GetStatusCode() : UC_FAILED);
	}

	triggerEvent(IGroupServiceEvent, OnAdminSet(result, groupId, type, adminList));
	return result;
}

int32_t GroupService::OnRemoveMemberResponse(int32_t errorCode, HTTPResponse *response, int64_t groupId, std::vector<int32_t> memberList){

	UC_LOG(INFO) << "OnRemoveMemberResponse GroupId:" << groupId << 
		" errorCode:" << errorCode << " response:" << (response != NULL ? response->GetContent() : "");

	int32_t result = UC_FAILED;
	if (UC_SUCCEEDED == errorCode && NULL != response && 200 == response->GetStatusCode()) {
		wstringstream wsStream;
		wsStream.str(StringConvert::FromUTF8Str(response->GetContent()).c_str());
		boost::property_tree::wptree jsonRoot;
		try{
			boost::property_tree::json_parser::read_json(wsStream, jsonRoot);
			result = jsonRoot.get<int32_t>(L"code");
			if (UC_SUCCEEDED == result){
				theStore.GetGroupListStore()->RemoveMemberInfoById(groupId, memberList);
			}
		}catch (boost::property_tree::ptree_error & e) {
			UC_LOG(ERROR) << "encounter an exception when parse json from OnRemoveMemberResponse , exception: " << e.what();
			result = JSON_PARSE_ERROR;
		}
	}else{
		result = errorCode != UC_SUCCEEDED ? errorCode : (NULL != response && 200 != response->GetStatusCode() ? response->GetStatusCode() : UC_FAILED);
	}

	triggerEvent(IGroupServiceEvent, OnMemberRemoved(result, groupId, GetCurUser().GetUserId(), memberList));
	return result;
}

int32_t GroupService::OnSetGroupNameResponse(int32_t errorCode, HTTPResponse *response, int64_t groupId){

	UC_LOG(INFO) << "OnSetGroupNameResponse GroupId:" << groupId << 
		" errorCode:" << errorCode << " response:" << (response != NULL ? response->GetContent() : "");

	int32_t result = UC_FAILED;
	std::wstring value = L"";
	std::string groupName = "";
	std::string pinYinName = "";

	if (UC_SUCCEEDED == errorCode && NULL != response && 200 == response->GetStatusCode()) {
		wstringstream wsStream;
		wsStream.str(StringConvert::FromUTF8Str(response->GetContent()).c_str());
		boost::property_tree::wptree jsonRoot;
		try{
			boost::property_tree::json_parser::read_json(wsStream, jsonRoot);
			result = jsonRoot.get<int32_t>(L"code");
			if(0 == result){
				boost::property_tree::wptree jsonChild = jsonRoot.get_child(L"data");
				BOOST_FOREACH(boost::property_tree::wptree::value_type v, jsonChild){
					value = v.second.get<std::wstring>(L"group_name", L"");
					groupName = StringConvert::ToUTF8Str(value);
					value = v.second.get<std::wstring>(L"group_pinyin", L"");
					pinYinName = StringConvert::ToUTF8Str(value);
				}

				theStore.GetGroupListStore()->UpdateGroupName(groupId, groupName, pinYinName);
			}
		}catch (boost::property_tree::ptree_error & e) {
			UC_LOG(ERROR) << "encounter an exception when parse json from OnSetGroupNameResponse , exception: " << e.what();
			result = JSON_PARSE_ERROR;
		}
	} else {
		result = errorCode != UC_SUCCEEDED ? errorCode : (NULL != response && 200 != response->GetStatusCode() ? response->GetStatusCode() : UC_FAILED);
	}
	
	triggerEvent(IGroupServiceEvent, OnGroupNameUpdated(result, groupId, GetCurUser().GetUserId(),
		groupName, pinYinName));

	return result;
}

int32_t GroupService::OnSetGroupLogoResponse(int32_t errorCode, HTTPResponse *response, int64_t groupId, std::string logoUrl) {

	UC_LOG(INFO) << "OnSetGroupLogoResponse GroupId:" << groupId << 
		" errorCode:" << errorCode << " response:" << (response != NULL ? response->GetContent() : "");

	int32_t result = UC_FAILED;
	if (UC_SUCCEEDED == errorCode && NULL != response && 200 == response->GetStatusCode()) {
		wstringstream wsStream;
		wsStream.str(StringConvert::FromUTF8Str(response->GetContent()).c_str());
		boost::property_tree::wptree jsonRoot;
		try{
			boost::property_tree::json_parser::read_json(wsStream, jsonRoot);
			result = jsonRoot.get<int32_t>(L"code");
			if(0 == result){
				theStore.GetGroupListStore()->UpdateGroupAvatarIndex(groupId, logoUrl);
			}
		}catch (boost::property_tree::ptree_error & e) {
			UC_LOG(ERROR) << "encounter an exception when parse json from OnSetGroupLogoResponse , exception: " << e.what();
			result = JSON_PARSE_ERROR;
		}
	}else{
		result = errorCode != UC_SUCCEEDED ? errorCode : (NULL != response && 200 != response->GetStatusCode() ? response->GetStatusCode() : UC_FAILED);
	}

	triggerEvent(IGroupServiceEvent, OnGroupLogoUpdated(result, groupId, logoUrl));
	return result;
}

int32_t GroupService::OnSetTotChatResponse(int32_t errorCode, HTTPResponse *response, int64_t groupId, int32_t state) {

	UC_LOG(INFO) << "OnSetGroupLogoResponse GroupId:" << groupId << " state:" << state <<
 		" errorCode:" << errorCode << " response:" << (response != NULL ? response->GetContent() : "");

	int32_t result = UC_FAILED;
	if (UC_SUCCEEDED == errorCode && NULL != response && 200 == response->GetStatusCode()) {
		wstringstream wsStream;
		wsStream.str(StringConvert::FromUTF8Str(response->GetContent()).c_str());
		boost::property_tree::wptree jsonRoot;
		try{
			boost::property_tree::json_parser::read_json(wsStream, jsonRoot);
			result = jsonRoot.get<int32_t>(L"code");
			if (0 == result){
				theStore.GetGroupListStore()->SetGroupTop(groupId, state);
			}
		}catch (boost::property_tree::ptree_error & e) {
			UC_LOG(ERROR) << "encounter an exception when parse json from OnSetTotChatResponse , exception: " << e.what();
			result = JSON_PARSE_ERROR;
		}
	}else{
		result = errorCode != UC_SUCCEEDED ? errorCode : (NULL != response && 200 != response->GetStatusCode() ? response->GetStatusCode() : UC_FAILED);
	}
	
	triggerEvent(IGroupServiceEvent, OnTopChatSet(result, groupId, state));
	return result;
}

int32_t GroupService::OnSetDisturbResponse(int32_t errorCode, HTTPResponse *response, int64_t groupId, int32_t state) {

	UC_LOG(INFO) << "OnSetDisturbResponse GroupId:" << groupId << " state:" << state <<
		" errorCode:" << errorCode << " response:" << (response != NULL ? response->GetContent() : "");

	int32_t result = UC_FAILED;
	if (UC_SUCCEEDED == errorCode && NULL != response && 200 == response->GetStatusCode()) {
		wstringstream wsStream;

		wsStream.str(StringConvert::FromUTF8Str(response->GetContent()).c_str());
		boost::property_tree::wptree jsonRoot;
		try{
			boost::property_tree::json_parser::read_json(wsStream, jsonRoot);
			result = jsonRoot.get<int32_t>(L"code");
			if (0 == result){
				theStore.GetGroupListStore()->SetGroupDisturb(groupId, state);
			}
		}catch (boost::property_tree::ptree_error & e) {
			UC_LOG(ERROR) << "encounter an exception when parse json from OnSetDisturbResponse , exception: " << e.what();
			result = JSON_PARSE_ERROR;
		}
	}else{
		result = errorCode != UC_SUCCEEDED ? errorCode : (NULL != response && 200 != response->GetStatusCode() ? response->GetStatusCode() : UC_FAILED);
	}

	triggerEvent(IGroupServiceEvent, OnDisturbSet(result, groupId, state));
	return result;
}

int32_t GroupService::OnSetReminderResponse(int32_t errorCode, HTTPResponse *response, int64_t groupId, int32_t state, int32_t alertRule, int32_t alertTime) {

	UC_LOG(INFO) << "OnSetReminderResponse GroupId:" << groupId << " state:" << state <<
		" errorCode:" << errorCode << " response:" << (response != NULL ? response->GetContent() : "");

	int32_t result = UC_FAILED;
	if (UC_SUCCEEDED == errorCode && NULL != response && 200 == response->GetStatusCode()) {
		wstringstream wsStream;
		wsStream.str(StringConvert::FromUTF8Str(response->GetContent()).c_str());
		boost::property_tree::wptree jsonRoot;
		try{
			boost::property_tree::json_parser::read_json(wsStream, jsonRoot);
			result = jsonRoot.get<int32_t>(L"code");
			if (0 == result){
				theStore.GetGroupListStore()->SetGroupAlertCondition(groupId, state, alertRule, alertTime);
			}
		}catch (boost::property_tree::ptree_error & e) {
			UC_LOG(ERROR) << "encounter an exception when parse json from OnSetReminderResponse , exception: " << e.what();
			result = JSON_PARSE_ERROR;
		}
	}else{
		result = errorCode != UC_SUCCEEDED ? errorCode : (NULL != response && 200 != response->GetStatusCode() ? response->GetStatusCode() : UC_FAILED);
	}

	triggerEvent(IGroupServiceEvent, OnReminderSet(result, groupId, state, alertRule, alertTime));
	return result;
}

int32_t GroupService::OnGetGroupInfoResponse(int32_t errorCode, HTTPResponse *response, int64_t groupId, int32_t mountId) {

	UC_LOG(INFO) << "OnGetGroupInfoResponse GroupId:" << groupId << 
		" errorCode:" << errorCode << " response:" << (response != NULL ? response->GetContent() : "");

	int32_t result = UC_FAILED;
	GroupInfo groupInfo;
	if (UC_SUCCEEDED == errorCode && NULL != response && 200 == response->GetStatusCode()) {
		rapidjson::Document document;
		if (!document.Parse<0>(response->GetContent().c_str()).HasParseError()) {
			result = document["code"].GetInt();
			if (0 == result) {
				rapidjson::Value &data = document["data"];
				for (rapidjson::SizeType i = 0; i < data.Size(); i++) {
					ParseGroupInfo(data[i], groupInfo);

					theStore.GetGroupListStore()->UpdateGroupInfo(groupInfo);
				}
			}
		}
		else{
			UC_LOG(ERROR) << "encounter an exception when parse json from OnGetGroupInfoResponse , exception: " << document.GetParseError();
			result = JSON_PARSE_ERROR;
		}
	} else {
		result = errorCode != UC_SUCCEEDED ? errorCode : (NULL != response && 200 != response->GetStatusCode() ? response->GetStatusCode() : UC_FAILED);
	}
	// 之前更新了groupInfo,这里需要从内存中重新拿（因为从服务器获取的groupInfo没有本地头像地址）
	// groupId有可能是0，所以用groupInfo.GetId()
	GroupInfo info;
	theStore.GetGroupListStore()->GetGroupInfo(groupInfo.GetId(), info);

	if (mountId == 0) {
		mountId = groupInfo.GetMountId();
	}
	triggerEvent(IGroupServiceEvent, OnGroupInfoReceived(result, groupInfo.GetId(), mountId, info));
	return result;
}

int32_t GroupService::OnGetGroupListResponse(int32_t errorCode, HTTPResponse *response, GroupStyle groupType, bool update, bool allData) {
	UC_LOG(INFO) << "OnGetGroupListResponse errorCode:" << errorCode << 
		" response:" << (response != NULL ? response->GetContent() : "");
	int32_t serverCount = 0;
	GroupList groupList;
	int32_t result = UC_FAILED;
	if (UC_SUCCEEDED == errorCode && NULL != response && 200 == response->GetStatusCode()) {
		rapidjson::Document document;
		if (!document.Parse<0>(response->GetContent().c_str()).HasParseError()) {
			result = document["code"].GetInt();
			if (0 == result) {
				rapidjson::Value &data = document["data"];
				for (rapidjson::SizeType i = 0; i < data.Size(); i++) {
					serverCount++;
					GroupInfo groupInfo;
					ParseGroupInfo(data[i], groupInfo);
					groupList.push_back(groupInfo);
				}
			}
		}
		else{
			groupList.clear();
			UC_LOG(ERROR) << "encounter an exception when parse json from OnGetGroupListResponse , exception: " << document.GetParseError();
			result = JSON_PARSE_ERROR;
		}
	}else{
		result = errorCode != UC_SUCCEEDED ? errorCode : (NULL != response && 200 != response->GetStatusCode() ? response->GetStatusCode() : UC_FAILED);
	}

	bool synced = false;
	{
		boost::unique_lock<boost::recursive_mutex> lock(_grouplist_status_mutex);
	
		if (_sync_status == STATUS_DOING)
		{
			synced = true;
			_sync_status = STATUS_DONE;
		}

		theStore.GetGroupListStore()->StoreGroupList(groupList, synced); // 调用 store的方法存储到内存或者DB
		groupList.clear();
		
		// 根据list cache的数据发送Event
		for (list_group_call_cache_item & item : _list_group_call_cache)
		{
			theStore.GetGroupListStore()->GetGroupList(item._type, item._page, item._count, item._last_time, item._all_data, false, groupList);
			triggerEvent(IGroupServiceEvent, OnGroupListReceived(result, item._type, item._all_data, groupList));
		}
		_list_group_call_cache.clear();
		
	}

	//每次同步1000条数据，当数据超过1000条时，需要再次请求;
	if (update && (serverCount == SYNC_GROUP_NUMBER) && (result == UC_SUCCEEDED))
	{
		GetGroupList(ALL_GROUP_TYPE, 1, SYNC_GROUP_NUMBER, theStore.GetGroupListStore()->GetLastModifyTime(), true, true);
	}

	return result;
}

int32_t GroupService::OnGetDocListResponse(int32_t errorCode, HTTPResponse *response, int64_t groupId) {
	
	UC_LOG(INFO) << "OnGetDocListResponse GroupId" << groupId << " errorCode:" << errorCode << 
		" response:" << (response != NULL ? response->GetContent() : "");

	int32_t result = UC_FAILED;
	DocumentList docList;
	if (UC_SUCCEEDED == errorCode && NULL != response && 200 == response->GetStatusCode()) {
		wstringstream wsStream;
		wsStream.str(StringConvert::FromUTF8Str(response->GetContent()).c_str());
		boost::property_tree::wptree jsonRoot;
		try{
			boost::property_tree::json_parser::read_json(wsStream, jsonRoot);
			result = jsonRoot.get<int32_t>(L"code");
			if(0 == result){
				boost::property_tree::wptree jsonChild = jsonRoot.get_child(L"data");
				BOOST_FOREACH(boost::property_tree::wptree::value_type& v, jsonChild){
					DocumentInfo docInfo;
					GetDocumentInfo(1, v.second, docInfo);
					docList.push_back(docInfo);
				}
			}
		}catch (boost::property_tree::ptree_error & e) {
			docList.clear();
			UC_LOG(ERROR) << "encounter an exception when parse json from OnGetDocListResponse , exception: " << e.what();
			result = JSON_PARSE_ERROR;
		}
	}else{
		result = errorCode != UC_SUCCEEDED ? errorCode : (NULL != response && 200 != response->GetStatusCode() ? response->GetStatusCode() : UC_FAILED);
	}

	triggerEvent(IGroupServiceEvent, OnDocListReceived(result, groupId, docList));

	if (docList.size() > 0){
		boost::unique_lock<boost::recursive_mutex> lock(_documentMutex);
		AddDocumentList(groupId, docList);
	}
	return result;
}

int32_t GroupService::OnGetDocumentInfoResponse(int32_t errorCode, HTTPResponse *response, int64_t groupId, int64_t fileId) {

	UC_LOG(INFO) << "OnGetDocumentInfoResponse GroupId" << groupId << " filedId:" << fileId << " errorCode:" << errorCode << 
		" response:" << (response != NULL ? response->GetContent() : "");

	DocumentInfo docInfo ;
	int32_t result = UC_FAILED;
	if (UC_SUCCEEDED == errorCode && NULL != response && 200 == response->GetStatusCode()){
		wstringstream wsStream;
		wsStream.str(StringConvert::FromUTF8Str(response->GetContent()).c_str());
		boost::property_tree::wptree jsonRoot;
		try{
			boost::property_tree::json_parser::read_json(wsStream, jsonRoot);
			result = jsonRoot.get<int32_t>(L"code");
			if(0 == result){
				boost::property_tree::wptree jsonChild = jsonRoot.get_child(L"data");
				BOOST_FOREACH(boost::property_tree::wptree::value_type v, jsonChild){
					GetDocumentInfo(1, v.second, docInfo);
				}
				boost::unique_lock<boost::recursive_mutex> lock(_documentMutex);
				DocumentList* docList = GetDocumentListById(groupId);
				if (docList != 0){
					docList->push_back(docInfo);
				}
			}
		}catch (boost::property_tree::ptree_error & e) {
			UC_LOG(ERROR) << "encounter an exception when parse json from OnGetDocumentInfoResponse , exception: " << e.what();
			result = JSON_PARSE_ERROR;
		}
	}else{
		result = errorCode != UC_SUCCEEDED ? errorCode : (NULL != response && 200 != response->GetStatusCode() ? response->GetStatusCode() : UC_FAILED);
	}
	triggerEvent(IGroupServiceEvent, OnDocInfoReceived(result, groupId, docInfo));
	return result;
}

int32_t GroupService::OnUploadDocumentResponse(int32_t errorCode, HTTPResponse *response, int64_t groupId, int32_t content_type, int32_t operation, int32_t msgId, std::string fileName) {
	UC_LOG(INFO) << "OnUploadDocumentResponse GroupId" << groupId << " fileName:" << fileName	<<
		" errorCode:" << errorCode << " response:" << (response != NULL ? response->GetContent() : "");

	int32_t result = UC_FAILED;
	DocumentInfo docInfo;
	docInfo.SetMsgId(msgId);
	if (UC_SUCCEEDED == errorCode && NULL != response && 200 == response->GetStatusCode()) {
		wstringstream wsStream;
		wsStream.str(StringConvert::FromUTF8Str(response->GetContent()).c_str());
		boost::property_tree::wptree jsonRoot;
		try{
			boost::property_tree::json_parser::read_json(wsStream, jsonRoot);
			result = jsonRoot.get<int32_t>(L"code");
			if (0 == result){
				boost::property_tree::wptree jsonChild = jsonRoot.get_child(L"data");
				GetDocumentInfo(0, jsonChild, docInfo);
				boost::unique_lock<boost::recursive_mutex> lock(_documentMutex);
				AddDocument(groupId, docInfo);
			}
		}catch (boost::property_tree::ptree_error & e) {
			UC_LOG(ERROR) << "encounter an exception when parse json from OnUploadDocumentResponse , exception: " << e.what();
			result = JSON_PARSE_ERROR;
		}
	}else{
		result = errorCode != UC_SUCCEEDED ? errorCode : (NULL != response && 200 != response->GetStatusCode() ? response->GetStatusCode() : UC_FAILED);
	}
	if (UC_SUCCEEDED != result){
		docInfo.SetFileName(fileName);
		docInfo.SetContentType(content_type);
	}

	if ( content_type == 0 ) //document
	{
		triggerEvent(IGroupServiceEvent, OnDocumentUploaded(result, groupId, GetCurUser().GetUserId(), operation, docInfo));
	}
	else if ( content_type == 1 ) //code
	{
		triggerEvent(IGroupServiceEvent, OnCodeSnippetUploaded(result, groupId, GetCurUser().GetUserId(), operation, docInfo));
	}
	else
	{
		UC_LOG(ERROR) << "don't support this type so far. content_type: " << content_type;
	}
	return result;
}

int32_t GroupService::OnBatchUploadDocumentResponse(int32_t errorCode, HTTPResponse *response, int64_t groupId) {
	UC_LOG(INFO) << "OnBatchUploadDocumentResponse GroupId" << groupId <<
		", errorCode:" << errorCode << " response:" << (response != NULL ? response->GetContent() : "");

	return UC_SUCCEEDED;
}

int32_t GroupService::OnDeleteDocumentResponse(int32_t errorCode, HTTPResponse *response, int64_t groupId, int64_t contentId) {

	UC_LOG(INFO) << "OnDeleteDocumentResponse GroupId" << groupId << " fileId:" << contentId << " errorCode:" << errorCode << 
		" response:" << (response != NULL ? response->GetContent() : "");

	int32_t result = UC_FAILED;
	std::wstring docName = L"";

	if (UC_SUCCEEDED == errorCode && NULL != response && 200 == response->GetStatusCode()) {
		wstringstream wsStream;
		wsStream.str(StringConvert::FromUTF8Str(response->GetContent()).c_str());
		boost::property_tree::wptree jsonRoot;
		try{
			boost::property_tree::json_parser::read_json(wsStream, jsonRoot);
			result = jsonRoot.get<int32_t>(L"code");
			if (0 == result){
				RemoveDocumentInfoById(groupId, contentId);
			}
		}catch (boost::property_tree::ptree_error & e) {
			UC_LOG(ERROR) << "encounter an exception when parse json from OnDeleteDocumentResponse , exception: " << e.what();
			result = JSON_PARSE_ERROR;
		}
	} else {
		result = errorCode != UC_SUCCEEDED ? errorCode : (NULL != response && 200 != response->GetStatusCode() ? response->GetStatusCode() : UC_FAILED);
	}

	triggerEvent(IGroupServiceEvent, OnDocumentDeleted(result, groupId, GetCurUser().GetUserId(),
		StringConvert::ToUTF8Str(docName), contentId));
	return result;

}

int32_t GroupService::OnGetCommentListResponse(int32_t errorCode, HTTPResponse *response, int64_t groupId, int64_t fileId) {

	UC_LOG(INFO) << "OnGetCommentListResponse GroupId" << groupId << " fileId:" << fileId << " errorCode:" << errorCode << 
		" response:" << (response != NULL ? response->GetContent() : "");

	int32_t result = UC_FAILED;
	CommentList commentList;
	if (UC_SUCCEEDED == errorCode && NULL != response && 200 == response->GetStatusCode()) {
		wstringstream wsStream;
		wsStream.str(StringConvert::FromUTF8Str(response->GetContent()).c_str());
		boost::property_tree::wptree jsonRoot;
		try{
			boost::property_tree::json_parser::read_json(wsStream, jsonRoot);
			result = jsonRoot.get<int32_t>(L"code");
			if (0 == result){
				boost::property_tree::wptree jsonChild = jsonRoot.get_child(L"data");
				BOOST_FOREACH(boost::property_tree::wptree::value_type v, jsonChild){
					CommentInfo commentInfo;
					GetCommentInfo(v.second, commentInfo);
					commentList.push_back(commentInfo);
				}
			}
		}catch (boost::property_tree::ptree_error & e) {
			UC_LOG(ERROR) << "encounter an exception when parse json from OnDeleteDocumentResponse , exception: " << e.what();
			result = JSON_PARSE_ERROR;
		}
	} else {
		result = errorCode != UC_SUCCEEDED ? errorCode : (NULL != response && 200 != response->GetStatusCode() ? response->GetStatusCode() : UC_FAILED);
	}

	triggerEvent(IGroupServiceEvent, OnCommentListReceived(result, groupId, fileId,	commentList));
	return result;
}

int32_t GroupService::OnCreateCommentResponse(int32_t errorCode, HTTPResponse *response, int64_t groupId, int64_t fileId, std::string content, int32_t msgId){

	UC_LOG(INFO) << "OnGetCommentListResponse GroupId" << groupId << " fileId:" << fileId << " errorCode:" << errorCode << 
		" response:" << (response != NULL ? response->GetContent() : "");

	int32_t result = UC_FAILED;
	CommentInfo commentInfo;
	commentInfo.SetMsgId(msgId);

	if (UC_SUCCEEDED == errorCode && NULL != response && 200 == response->GetStatusCode()) {
		wstringstream wsStream;
		wsStream.str(StringConvert::FromUTF8Str(response->GetContent()).c_str());
		boost::property_tree::wptree jsonRoot;
		try{
			boost::property_tree::json_parser::read_json(wsStream, jsonRoot);
			result = jsonRoot.get<int32_t>(L"code");
			if (0 == result){
				boost::property_tree::wptree jsonChild = jsonRoot.get_child(L"data");
				BOOST_FOREACH(boost::property_tree::wptree::value_type v, jsonChild){
					commentInfo.SetCommentId(v.second.get<int64_t>(L"comments_id", 0));
					commentInfo.SetCreateTime(v.second.get<int64_t>(L"create_time", 0));
				}
			}
		}catch (boost::property_tree::ptree_error & e) {
			UC_LOG(ERROR) << "encounter an exception when parse json from OnDeleteDocumentResponse , exception: " << e.what();
			result = JSON_PARSE_ERROR;
		}
	} else {
		result = errorCode != UC_SUCCEEDED ? errorCode : (NULL != response && 200 != response->GetStatusCode() ? response->GetStatusCode() : UC_FAILED);
	}

	commentInfo.SetFileId(fileId);
	commentInfo.SetUserId(GetCurUser().GetUserId());
	commentInfo.SetUserName(GetCurUserName());
	commentInfo.SetContent(content);
	
	triggerEvent(IGroupServiceEvent, OnCommentCreated(result, groupId, commentInfo));
	return result;
}


int32_t GroupService::OnGetAtMessageListResponse(int32_t errorCode, HTTPResponse* response, int64_t groupId) {
	UC_LOG(INFO) << "OnGetAtMessageListResponse GroupId" << groupId << ", errorCode:" << errorCode <<
		", response:" << (response != NULL ? response->GetContent() : "");

	int32_t result = UC_FAILED;
	std::map<int64_t, int64_t> atMsgList;
	if (UC_SUCCEEDED == errorCode && NULL != response && 200 == response->GetStatusCode()) {
		wstringstream wsStream;
		wsStream.str(StringConvert::FromUTF8Str(response->GetContent()).c_str());
		boost::property_tree::wptree jsonRoot;
		try{
			boost::property_tree::json_parser::read_json(wsStream, jsonRoot);
			result = jsonRoot.get<int32_t>(L"code");
			if (0 == result){
				boost::property_tree::wptree jsonChild = jsonRoot.get_child(L"data");
				BOOST_FOREACH(boost::property_tree::wptree::value_type v, jsonChild){
					boost::property_tree::wptree jsonElement = v.second;
					atMsgList[jsonElement.get<int64_t>(L"seq")] = jsonElement.get<int64_t>(L"sender");
				}
			}
		}
		catch (boost::property_tree::ptree_error & e) {
			UC_LOG(ERROR) << "encounter an exception when parse json from OnGetAtMessageListResponse , exception: " << e.what();
			result = JSON_PARSE_ERROR;
		}
	}
	else {
		result = errorCode != UC_SUCCEEDED ? errorCode : (NULL != response && 200 != response->GetStatusCode() ? response->GetStatusCode() : UC_FAILED);
	}

	if (result == UC_SUCCEEDED) {
		CacheAtMsgList(groupId, atMsgList);
	}

	triggerEvent(IGroupServiceEvent, OnAtMessageListReceived(result, groupId, atMsgList));
	return result;

}

int32_t GroupService::OnDeleteAtMessageResponse(int32_t errorCode, HTTPResponse* response, ConversationType type, int64_t groupId, std::vector<int64_t> seqList) {
	UC_LOG(INFO) << "OnDeleteAtMessageResponse  ConversationType:" << type << ",GroupId:" << groupId << ", errorCode:" << errorCode <<
		", response:" << (response != NULL ? response->GetContent() : "");

	int32_t result = UC_FAILED;
	if (UC_SUCCEEDED == errorCode && NULL != response && 200 == response->GetStatusCode()) {
		wstringstream wsStream;
		wsStream.str(StringConvert::FromUTF8Str(response->GetContent()).c_str());
		boost::property_tree::wptree jsonRoot;
		try{
			boost::property_tree::json_parser::read_json(wsStream, jsonRoot);
			result = jsonRoot.get<int32_t>(L"code");
		}
		catch (boost::property_tree::ptree_error & e) {
			UC_LOG(ERROR) << "encounter an exception when parse json from OnDeleteAtMessageResponse , exception: " << e.what();
			result = JSON_PARSE_ERROR;
		}
	}
	else {
		result = errorCode != UC_SUCCEEDED ? errorCode : (NULL != response && 200 != response->GetStatusCode() ? response->GetStatusCode() : UC_FAILED);
	}

	//删除成功后，同步删除缓存
	if (result == UC_SUCCEEDED)
	{
		DeleteAtMsgFromCache(groupId, seqList);
	}

	triggerEvent(IGroupServiceEvent, OnAtMessageDeleted(result, type, groupId, seqList));
	return result;
}


} /* namespace uc */
