/*
* OpenApiService.cpp
*
*  Created on: Sep 9, 2015
*      Author: yongshuai.cheng
*/

#include "OpenApiService.h"
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include <sstream> 
#include "message_service/MessageProcessor.h"
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

OpenApiService::OpenApiService(IUCEngine *engine) {
	_engine = engine;
	_engine->registerListener(this);
}

OpenApiService::~OpenApiService() {
	_engine->removeListener(this);
}

int32_t OpenApiService::GetAppList(){
	
	HTTPRequest     request;
	stringstream	sStream;
	string         	targetURL;

	boost::property_tree::wptree jsonRoot;

	targetURL = ((UCEngine*)_engine)->GetRequestURL(((ClientService*)_engine->QueryService(UCClientService))->GetLoginInfo().GetUccServerDomain(),
		APP_LIST_METHOD);

	request.Init(targetURL, HTTP_METHOD_POST);
	request.AddHeaderInfo("Content-Type", "application/json");

	jsonRoot.put(L"userId", GetCurUser().GetUserId());
	jsonRoot.put(L"sessionId", StringConvert::FromUTF8Str((
		(ClientService*)_engine->QueryService(UCClientService))->GetLoginInfo().GetSessionId()));
	wstringstream sData;
	boost::property_tree::json_parser::write_json(sData, jsonRoot);

	sStream << StringConvert::ToUTF8Str(sData.str());
	request.SetBody(sStream.str());
	((UCEngine*)_engine)->SendAsyncRequest(request, MakeRequestCallback(this, &OpenApiService::OnGetAppListResponse));

	return UC_SUCCEEDED;
}

int32_t OpenApiService::GetAppInfo(const std::list<int64_t>& appIds){

	HTTPRequest     request;
	stringstream	sStream;
	string         	targetURL;

	boost::property_tree::wptree jsonRoot;
	boost::property_tree::wptree jsonChild;
	boost::property_tree::wptree jsonElement;

	targetURL = ((UCEngine*)_engine)->GetRequestURL(((ClientService*)_engine->QueryService(UCClientService))->GetLoginInfo().GetUccServerDomain(),
		APP_INFO_METHOD);

	request.Init(targetURL, HTTP_METHOD_POST);
	request.AddHeaderInfo("Content-Type", "application/json");

	jsonRoot.put(L"userId", GetCurUser().GetUserId());
	jsonRoot.put(L"sessionId", StringConvert::FromUTF8Str((
		(ClientService*)_engine->QueryService(UCClientService))->GetLoginInfo().GetSessionId()));

	std::list<int64_t>::const_iterator iter = appIds.begin();
	for (; iter != appIds.end(); ++iter){
		jsonElement.put(L"", *iter);
		jsonChild.push_back(std::make_pair(L"", jsonElement));
	}
	jsonRoot.put_child(L"appIds", jsonChild);
	wstringstream sData;
	boost::property_tree::json_parser::write_json(sData, jsonRoot);

	sStream << StringConvert::ToUTF8Str(sData.str());
	request.SetBody(sStream.str());
	((UCEngine*)_engine)->SendAsyncRequest(request, MakeRequestCallback(this, &OpenApiService::OnGetAppInfoResponse, appIds));

	return UC_SUCCEEDED;
}

int32_t OpenApiService::GetOAMessageStatus(const std::list<AppMessageInfo> &infos){

	HTTPRequest     request;
	stringstream	sStream;
	string         	targetURL;

	targetURL = ((UCEngine*)_engine)->GetRequestURL(((ClientService*)_engine->QueryService(UCClientService))->GetLoginInfo().GetUccServerDomain(),
		OA_MESSAGE_STATUS_METHOD);

	request.Init(targetURL, HTTP_METHOD_POST);
	request.AddHeaderInfo("Content-Type", "application/x-www-form-urlencoded");
	
	boost::property_tree::wptree jsonChild;
	wstringstream sData;
	sData << "[";
	std::list<AppMessageInfo>::const_iterator iter = infos.begin();
	for (; iter != infos.end(); ++iter){
		jsonChild.put(L"app_id", iter->appId);
		jsonChild.put(L"seq", iter->msgSeq);
		wstringstream tData;
		boost::property_tree::json_parser::write_json(tData, jsonChild);
		if (iter != infos.begin())
			sData << ",";
		sData << tData.str();
	}
	sData << "]";

	sStream << "session_id=" << ((ClientService*)_engine->QueryService(UCClientService))->GetLoginInfo().GetSessionId()  <<
		"&user_id=" << GetCurUser().GetUserId() << "&data=" << StringConvert::URLEncode(StringConvert::ToUTF8Str(sData.str()));
	request.SetBody(sStream.str());
	((UCEngine*)_engine)->SendAsyncRequest(request, MakeRequestCallback(this, &OpenApiService::OnGetOAMessageStatusResponse, infos));

	return UC_SUCCEEDED;
}

int32_t OpenApiService::UpdateOAMessageStatus(const std::list<AppMessageStatusInfo> &infos){

	HTTPRequest     request;
	stringstream	sStream;
	string         	targetURL;
	
	targetURL = ((UCEngine*)_engine)->GetRequestURL(((ClientService*)_engine->QueryService(UCClientService))->GetLoginInfo().GetUccServerDomain(),
		UPDATE_OA_MESSGAE_STATUS_METHOD);

	request.Init(targetURL, HTTP_METHOD_POST);
	request.AddHeaderInfo("Content-Type", "application/x-www-form-urlencoded");

	boost::property_tree::wptree jsonChild;
	wstringstream sData;
	sData << "[";
	std::list<AppMessageStatusInfo>::const_iterator iter = infos.begin();
	for (; iter != infos.end(); ++iter){
		jsonChild.put(L"app_id", iter->appId);
		jsonChild.put(L"seq", iter->msgSeq);
		jsonChild.put(L"status", iter->status);
		wstringstream tData;
		boost::property_tree::json_parser::write_json(tData, jsonChild);
		if (iter != infos.begin())
			sData << ",";
		sData << tData.str();
	}
	sData << "]";

	sStream << "session_id=" << ((ClientService*)_engine->QueryService(UCClientService))->GetLoginInfo().GetSessionId() <<
		"&user_id=" << GetCurUser().GetUserId() << "&data=" << StringConvert::URLEncode(StringConvert::ToUTF8Str(sData.str()));
	request.SetBody(sStream.str());
	((UCEngine*)_engine)->SendAsyncRequest(request, MakeRequestCallback(this, &OpenApiService::OnUpdateOAMessageStatusResponse, infos));

	return UC_SUCCEEDED;

}

int32_t OpenApiService::GetMessageAckStatus(const std::list<AppMessageInfo> &infos){

	HTTPRequest     request;
	stringstream	sStream;
	string         	targetURL;

	targetURL = ((UCEngine*)_engine)->GetRequestURL(((ClientService*)_engine->QueryService(UCClientService))->GetLoginInfo().GetUccServerDomain(),
		MESSAGE_ACK_STATUS_METHOD);

	request.Init(targetURL, HTTP_METHOD_POST);
	request.AddHeaderInfo("Content-Type", "application/x-www-form-urlencoded");

	boost::property_tree::wptree jsonChild;
	wstringstream sData;
	sData << "[";
	std::list<AppMessageInfo>::const_iterator iter = infos.begin();
	for (; iter != infos.end(); ++iter){
		jsonChild.put(L"app_id", iter->appId);
		jsonChild.put(L"seq", iter->msgSeq);
		wstringstream tData;
		boost::property_tree::json_parser::write_json(tData, jsonChild);
		if (iter != infos.begin())
			sData << ",";
		sData << tData.str();
	}
	sData << "]";

	sStream << "session_id=" << ((ClientService*)_engine->QueryService(UCClientService))->GetLoginInfo().GetSessionId() <<
		"&user_id=" << GetCurUser().GetUserId() << "&data=" << StringConvert::URLEncode(StringConvert::ToUTF8Str(sData.str()));
	request.SetBody(sStream.str());
	((UCEngine*)_engine)->SendAsyncRequest(request, MakeRequestCallback(this, &OpenApiService::OnGetMessageAckStatusResponse, infos));

	return UC_SUCCEEDED;
}

int32_t OpenApiService::GetChatHistory(int64_t appId, int64_t count, int64_t startTime, int64_t endTime){

	std::list<MessageStoreItem*> msgList;
	int64_t starttime_t = startTime;
	int64_t endtime_t = endTime;
	int32_t update_flag_msgid = 0;

	MSG_STATUS status = theStore.GetMsgStore()->GetChatHistory(appId, OPENAPI_CHAT, count, startTime, endTime, update_flag_msgid, msgList);
	if (status != MSG_NEED_FROM_SERVER){
		std::list<MessageStoreItem*>::iterator iter = msgList.begin();
		for (; iter != msgList.end(); ++iter){
			MessageStoreItem* item = (*iter);
			assert(item->msg);
			triggerEventEx(IUCEngineEvent, *_engine, OnMessageReceived(CHAT, *(item->msg), item->body, item->state));
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
		triggerEvent(IOpenApiServiceEvent, OnChatHistoryReceived(UC_SUCCEEDED, appId, msgList.size(), min_timestamp));

		UC_LOG(INFO) << "openapi_service OnChatHistoryReceived from cache. result: " << UC_SUCCEEDED
			<< " appId: " << appId << " message_count:" << msgList.size() << " min_timestamp: " << min_timestamp;

		if (MSG_FROM_DB_SUCCESS == status){
			theStore.GetMsgStore()->ClearMessageList(msgList);
		}
		return UC_SUCCEEDED;
	}

	HTTPRequest     request;
	stringstream	sStream;
	string         targetURL;

	targetURL = ((UCEngine*)_engine)->GetRequestURL(((ClientService*)_engine->QueryService(UCClientService))->GetLoginInfo().GetUccServerDomain(), 
		CONTENT_BY_ID_METHOD);

	sStream << targetURL << "?from_user_id=" << GetCurUser().GetUserId() << "&to_user_id=" << appId << "&type=" << 3 <<
		"&count=" << count << "&start_time=" << startTime << "&end_time=" << endTime <<
		"&session_id=" << ((ClientService*)_engine->QueryService(UCClientService))->GetLoginInfo().GetSessionId();

	request.Init(sStream.str(), HTTP_METHOD_GET);
	((UCEngine*)_engine)->SendAsyncRequest(request, MakeRequestCallback(this, &OpenApiService::OnGetChatHistoryResponse, appId, count, starttime_t, endtime_t, startTime, endTime,update_flag_msgid));

	return UC_SUCCEEDED;
}

int32_t OpenApiService::ResendMessage(int32_t msgId){
	
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
		_engine->SendAsyncRequest(request, MakeRequestCallback(this, &OpenApiService::OnSendMessageResponse, (int64_t)head.GetTo().GetUserId(), msgId));
		return UC_SUCCEEDED;
	}

	return UC_FAILED;
}

int32_t OpenApiService::SendTextMessage(UCID toApp, const std::string& textTitle, const std::string& textContent, int8_t textType,
	const std::string& detailContent, OUT int32_t& msgId){

	MessageProcessor msgProcessor((UCEngine*)_engine);
	HTTPRequest	request;
	UCMsg *msg = NULL;
	BinMsg data;
	string body;
	vector<UCID> atUsers;

	msgId = msgProcessor.CreateTextMessage(OPENAPI_CHAT, GetCurUser(), toApp, textType, "", textContent, textTitle, detailContent, 0, 0, 0, atUsers, msg);
	if (msg == NULL) {
		UC_LOG(ERROR) << "create text message failed. ";
		return UC_FAILED;
	}

	if (msgProcessor.EncodeMessageData(*msg, data, body) != UC_SUCCEEDED) {
		UC_LOG(ERROR) << "encode text message failed: ";
		return UC_FAILED;
	}

	if (msgProcessor.SaveMessage(OPENAPI_CHAT, toApp.GetUserId(), 0, 0, STATE_SENDING, msg, body) != UC_SUCCEEDED){
		UC_LOG(ERROR) << "save message in cache failed. relate_id: " << toApp.GetUserId();
		delete msg;
		msg = NULL;
		return UC_FAILED;
	}

	if (UC_SUCCEEDED == msgProcessor.CreateMessageRequest(data, request)) {
		_engine->SendAsyncRequest(request, MakeRequestCallback(this, &OpenApiService::OnSendMessageResponse, (int64_t)toApp.GetUserId(), msgId));
		delete msg;
		msg = NULL;
		return UC_SUCCEEDED;
	}

	delete msg;
	msg = NULL;
	return UC_FAILED;
}

int32_t OpenApiService::SendImageMessage(UCID toApp, const std::string& imageId, int8_t imageType, const std::string imageData, OUT int32_t& msgId){
	
	MessageProcessor msgProcessor((UCEngine*)_engine);
	HTTPRequest	request;
	UCMsg *msg = NULL;
	BinMsg data;
	string body;
	
	msgId = msgProcessor.CreateImageMessage(OPENAPI_CHAT, GetCurUser(), toApp, "", 0, "", imageData, imageId, imageType, 0, 0, 0, msg);
	if (msg == NULL) {
		UC_LOG(ERROR) << "create image message failed.";
		return UC_FAILED;
	}

	if (msgProcessor.EncodeMessageData(*msg, data, body) != UC_SUCCEEDED) {
		UC_LOG(ERROR) << "encode image message failed.";
		return UC_FAILED;
	}
	
	if (msgProcessor.SaveMessage(OPENAPI_CHAT, toApp.GetUserId(), 0, 0, STATE_SENDING, msg, body) != UC_SUCCEEDED){
		UC_LOG(ERROR) << "save message in cache failed. relate_id: " << toApp.GetUserId();
		delete msg;
		msg = NULL;
		return UC_FAILED;
	}

	if (UC_SUCCEEDED == msgProcessor.CreateMessageRequest(data, request)) {
		_engine->SendAsyncRequest(request, MakeRequestCallback(this, &OpenApiService::OnSendMessageResponse, (int64_t)toApp.GetUserId(), msgId));
		delete msg;
		msg = NULL;
		return UC_SUCCEEDED;
	}

	delete msg;
	msg = NULL;
	return UC_FAILED;
}

int32_t OpenApiService::SendFileMessage(UCID toApp, const std::string& fileId, int8_t fileType, const std::string fileName,
	int64_t fileSize, OUT int32_t& msgId){
	MessageProcessor msgProcessor((UCEngine*)_engine);
	HTTPRequest	request;
	UCMsg *msg = NULL;
	BinMsg data;
	string body;

	
	msgId = msgProcessor.CreateSendFileMessage(OPENAPI_CHAT, GetCurUser(), toApp, fileName, 0, "", 0, fileId, fileSize, fileType, 0, 0, 0, msg);
	if (msg == NULL) {
		UC_LOG(ERROR) << "create send file message failed.";
		return UC_FAILED;
	}

	if (msgProcessor.EncodeMessageData(*msg, data, body) != UC_SUCCEEDED) {
		UC_LOG(ERROR) << "encode send file message failed.";
		return UC_FAILED;
	}

	if (msgProcessor.SaveMessage(OPENAPI_CHAT, toApp.GetUserId(), 0, 0, STATE_SENDING, msg, body) != UC_SUCCEEDED){
		UC_LOG(ERROR) << "save message in cache failed. relate_id: " << toApp.GetUserId();
		delete msg;
		msg = NULL;
		return UC_FAILED;
	}

	if (UC_SUCCEEDED == msgProcessor.CreateMessageRequest(data, request)) {
		_engine->SendAsyncRequest(request, MakeRequestCallback(this, &OpenApiService::OnSendMessageResponse, (int64_t)toApp.GetUserId(), msgId));
		delete msg;
		msg = NULL;
		return UC_SUCCEEDED;
	}

	delete msg;
	msg = NULL;
	return UC_FAILED;
}

int32_t OpenApiService::SendMessageRead(UCID appUid, int32_t msgId, int64_t msgSeq){

	HTTPRequest	request;
	MessageProcessor msgProcessor((UCEngine*)_engine);

	if (UC_SUCCEEDED == msgProcessor.CreateMessageReadMessage(SINGLE_CHAT, GetCurUser(), appUid, msgId, msgSeq, request)){
		_engine->SendAsyncRequest(request,
			MakeRequestCallback(this, &OpenApiService::OnSendMessageReadResponse, (int64_t)appUid.GetUserId(), 0, msgId, msgSeq));
		return UC_SUCCEEDED;
	}
	return UC_FAILED;
}

int32_t OpenApiService::SendMessagesRead(int64_t toAppId){

	HTTPRequest     request;
	stringstream	sStream;
	string         targetURL;

	targetURL = ((UCEngine*)_engine)->GetRequestURL(((ClientService*)_engine->QueryService(UCClientService))->GetLoginInfo().GetUccServerDomain(),
		MESSGAES_READ_METHOD);

	request.Init(targetURL, HTTP_METHOD_POST);
	request.AddHeaderInfo("Content-Type", "application/x-www-form-urlencoded");

	sStream << "session_id=" << ((ClientService*)_engine->QueryService(UCClientService))->GetLoginInfo().GetSessionId() <<
		"&user_id=" << GetCurUser().GetUserId() << "&to_user_id=" << toAppId << "&type=3";

	request.SetBody(sStream.str());
	_engine->SendAsyncRequest(request, MakeRequestCallback(this, &OpenApiService::OnSendMessageReadResponse, toAppId, 1, 0, (int64_t)0));
	return UC_SUCCEEDED;
}


int32_t OpenApiService::SendRequest(int32_t requestId, RequestMethod method, const std::string &url, const std::string &body, const std::list<HeadInfo> &heads){

	HTTPRequest     request;

	if (HTTP_GET_METHOD == method){
		request.Init(url, HTTP_METHOD_GET);
	}
	else if (HTTP_POST_METHOD == method){
		request.Init(url, HTTP_METHOD_POST);
		request.SetBody(body);
	}

	std::list<HeadInfo>::const_iterator iter = heads.begin();
	for (; iter != heads.end(); ++iter){
		request.AddHeaderInfo(iter->headKey, iter->headValue);
	}
	((UCEngine*)_engine)->SendAsyncRequest(request, MakeRequestCallback(this, &OpenApiService::OnSendRequestResponse, requestId));

	return UC_SUCCEEDED;
}

void OpenApiService::ParseAppInfo(const boost::property_tree::wptree &ptParse, AppInfo& info){
	
	info.SetAppId(ptParse.get<int32_t>(L"appId", 0));
	info.SetAppTitle(StringConvert::ToUTF8Str(ptParse.get<wstring>(L"title", L"")));
	info.SetAppDesc(StringConvert::ToUTF8Str(ptParse.get<wstring>(L"desc", L"")));
	info.SetAppLogo(StringConvert::ToUTF8Str(ptParse.get<wstring>(L"logoURL", L"")));
	info.SetPcUrl(StringConvert::ToUTF8Str(ptParse.get<wstring>(L"pcURL", L"")));
	info.SetIosUrl(StringConvert::ToUTF8Str(ptParse.get<wstring>(L"iosURL", L"")));
	info.SetAndroidUrl(StringConvert::ToUTF8Str(ptParse.get<wstring>(L"androidURL", L"")));
	info.SetEnableSendMessage(ptParse.get<int32_t>(L"enableSendMessage", 0));
	info.SetStatus(ptParse.get<int32_t>(L"status", 0));
	info.SetEventUrl(StringConvert::ToUTF8Str(ptParse.get<wstring>(L"checkEventURL", L"")));
	info.SetAccess(ptParse.get<int32_t>(L"access ", 0));
}

UCID OpenApiService::GetCurUser(){
	UCID userUid;
	if (_engine){
		userUid = ((ClientService*)_engine->QueryService(UCClientService))->GetLoginInfo().GetCurUser();
	}
	return 	userUid;
};

std::string OpenApiService::GetCurUserName(){
	std::string displayName = "";
	if (_engine){
		displayName = ((ClientService*)_engine->QueryService(UCClientService))->GetLoginInfo().GetDisplayName();
	}
	return 	displayName;
}

bool OpenApiService::IsCurUser(UCID userUid){

	if (userUid.GetUserId() == GetCurUser().GetUserId()
		&& userUid.GetResId() == GetCurUser().GetResId()
		&& userUid.GetSiteId() == GetCurUser().GetSiteId()){
			return true;
	}
	return false;
}

int32_t OpenApiService::OnGetAppListResponse(int32_t errorCode, HTTPResponse* response){

	UC_LOG(INFO) << "OnGetAppListResponse errorCode:" << errorCode << "response:" << (response != NULL ? response->GetContent() : "");
	int32_t result = UC_FAILED;
	std::list<AppInfo> appList;
	if (UC_SUCCEEDED == errorCode && NULL != response && 200 == response->GetStatusCode()) {
		wstringstream wsStream;
		wsStream.str(StringConvert::FromUTF8Str(response->GetContent()).c_str());
		boost::property_tree::wptree jsonRoot;
		try{
			boost::property_tree::json_parser::read_json(wsStream, jsonRoot);
			result = jsonRoot.get<int32_t>(L"code");
			if (0 == result) {
				boost::property_tree::wptree jsonChild = jsonRoot.get_child(L"data");
				BOOST_FOREACH(boost::property_tree::wptree::value_type& v, jsonChild){
					AppInfo appInfo;
					ParseAppInfo(v.second, appInfo);
					appList.push_back(appInfo);
				}
			}
		}
		catch (boost::property_tree::ptree_error & e) {
			UC_LOG(ERROR) << "encounter an exception when parse json from OnGetAppListResponse, exception: " << e.what();
			result = JSON_PARSE_ERROR;
		}
	}
	else {
		result = errorCode != UC_SUCCEEDED ? errorCode : (NULL != response && 200 != response->GetStatusCode() ? response->GetStatusCode() : UC_FAILED);
	}

	triggerEvent(IOpenApiServiceEvent, OnAppListReceived(result, appList));

	return result;
}

int32_t OpenApiService::OnGetAppInfoResponse(int32_t errorCode, HTTPResponse* response, std::list<int64_t> appIds){

	UC_LOG(INFO) << "OnGetAppInfoResponse errorCode:" << errorCode << "response:" << (response != NULL ? response->GetContent() : "");
	int32_t result = UC_FAILED;
	std::list<AppInfo> appList;
	if (UC_SUCCEEDED == errorCode && NULL != response && 200 == response->GetStatusCode()) {
		wstringstream wsStream;
		wsStream.str(StringConvert::FromUTF8Str(response->GetContent()).c_str());
		boost::property_tree::wptree jsonRoot;
		try{
			boost::property_tree::json_parser::read_json(wsStream, jsonRoot);
			result = jsonRoot.get<int32_t>(L"code");
			if (0 == result) {
				boost::property_tree::wptree jsonChild = jsonRoot.get_child(L"data");
				BOOST_FOREACH(boost::property_tree::wptree::value_type& v, jsonChild){
					AppInfo appInfo;
					ParseAppInfo(v.second, appInfo);
					appList.push_back(appInfo);
				}
			}
		}
		catch (boost::property_tree::ptree_error & e) {
			UC_LOG(ERROR) << "encounter an exception when parse json from OnGetAppListResponse, exception: " << e.what();
			result = JSON_PARSE_ERROR;
		}
	}
	else {
		result = errorCode != UC_SUCCEEDED ? errorCode : (NULL != response && 200 != response->GetStatusCode() ? response->GetStatusCode() : UC_FAILED);
	}

	triggerEvent(IOpenApiServiceEvent, OnAppInfoReceived(result, appList));

	return result;
}

int32_t OpenApiService::OnGetChatHistoryResponse(int32_t errorCode, HTTPResponse* response, int64_t appId, int64_t count, int64_t uistartTime, int64_t uiendTime, int64_t sdkstarttime, int64_t sdkendtime, int32_t update_flag_msgid){

	UC_LOG(INFO) << "OnGetChatHistoryResponse AppId:" << appId <<
		"errorCode:" << errorCode << "response:" << (response != NULL ? response->GetContent() : "");

	int32_t msgCount = 0;
	int32_t serverCount = 0;
	int32_t result = UC_FAILED;
	MessageProcessor msgProcessor((UCEngine*)_engine);
	std::list<MessageStoreItem*> msgList;
	if (UC_SUCCEEDED == errorCode && NULL != response && 200 == response->GetStatusCode()) {
		wstringstream wsStream;
		wsStream.str(StringConvert::FromUTF8Str(response->GetContent().c_str()));
		boost::property_tree::wptree jsonRoot;
		boost::property_tree::wptree jsonChild;
		try{
			boost::property_tree::json_parser::read_json(wsStream, jsonRoot);
			result = jsonRoot.get<int32_t>(L"code");
		}catch (boost::property_tree::ptree_error & e) {
				UC_LOG(ERROR) << "encounter an exception when parse json from OnGetChatHistoryResponse response, exception: " << e.what();
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
						MessageStoreItem* msg_item = new MessageStoreItem();
						msg_item->msg = msg;
						msg_item->body = body.c_str();
						msg_item->relate_id = MessageProcessor::GetRelateId(MessageProcessor::GetConversationType(head.GetAppId(), head.GetProtocoltype()), GetCurUser().GetUserId(), head.GetFrom().GetUserId(), head.GetTo().GetUserId());
						msg_item->state = STATE_SENDSUCCESS;
						msg_item->conversation_type = MessageProcessor::GetConversationType(head.GetAppId(), head.GetProtocoltype());
						msg_item->oa_last_status = jsonElement.get<int32_t>(L"oa_last_status", -1);
						msg_item->app_ack_sent = jsonElement.get<int32_t>(L"app_ack_sent’", 1);			      
						msg_item->oa_external_data = StringConvert::ToUTF8Str(jsonElement.get<std::wstring>(L"oa_external_data", L""));
						msgList.push_back(msg_item);
					}else{
						UC_LOG(ERROR) << "why parse a message json to thrift data return error";
						result = UC_JSON_PARSE_ONE_DATA_ERROR;
					}
				}catch (boost::property_tree::ptree_error & e) {
					UC_LOG(ERROR) << "encounter an exception when parse json from OnGetChatHistoryResponse single message  , exception: " << e.what();
					result = UC_JSON_PARSE_ONE_DATA_ERROR;
				}
			}
		}
	}
	else{
		result = errorCode != UC_SUCCEEDED ? errorCode : (NULL != response && 200 != response->GetStatusCode() ? response->GetStatusCode() : UC_FAILED);
	}

	if (UC_SUCCEEDED == result || UC_JSON_PARSE_ONE_DATA_ERROR == result){

		bool ret = false;
		// 如果UCC返回数据解析失败，不做本地同步;
		if (UC_SUCCEEDED == result){
			ret = theStore.GetMsgStore()->SyncMessages(appId, OPENAPI_CHAT, count, uistartTime, uiendTime, update_flag_msgid, msgList, (uiendTime == sdkendtime));
			if (!ret){
				result = UCAS_MESSAGE_SYNC_LOCAL_FAILED;
			}
		}
		// 本地同步成功或者UCC返回数据解析失败，把有效数据提供给UI;
		if (ret || UC_JSON_PARSE_ONE_DATA_ERROR == result){
			for (std::list<MessageStoreItem*>::iterator iter = msgList.begin(); iter != msgList.end(); ++iter){
				MessageStoreItem* item = (*iter);
				triggerEventEx(IUCEngineEvent, *_engine, OnMessageReceived(CHAT, *(item->msg), item->body, item->state));
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
		msgCount = msgList.size();
	}
	//如果Server单条数据解析失败，给UI的错误码默认为成功;
	if (UC_JSON_PARSE_ONE_DATA_ERROR == result){
		result = UC_SUCCEEDED;
	}

	triggerEvent(IOpenApiServiceEvent, OnChatHistoryReceived(result, appId, msgCount, min_timestamp));

	UC_LOG(INFO) << "openapi_service OnChatHistoryReceived from server. result: " << result 
		<< " appId: " << appId << " message_count:" << msgList.size() << " min_timestamp: " << min_timestamp;

	theStore.GetMsgStore()->ClearMessageList(msgList);
	return result;
}

int32_t OpenApiService::OnGetOAMessageStatusResponse(int32_t errorCode, HTTPResponse *response, const std::list<AppMessageInfo> infos){

	UC_LOG(INFO) << "OnGetOAStatusResponse errorCode:" << errorCode << " response:" << (response != NULL ? response->GetContent() : "");

	int32_t result = UC_FAILED;
	std::list<AppMessageStatusInfo> statusList;
	if (UC_SUCCEEDED == errorCode && NULL != response && 200 == response->GetStatusCode()) {
		wstringstream wsStream;
		wsStream.str(StringConvert::FromUTF8Str(response->GetContent()).c_str());
		boost::property_tree::wptree jsonRoot;
		try{
			boost::property_tree::json_parser::read_json(wsStream, jsonRoot);
			result = jsonRoot.get<int32_t>(L"code");
			if (UC_SUCCEEDED == result) {
				boost::property_tree::wptree jsonChild = jsonRoot.get_child(L"data");
				BOOST_FOREACH(boost::property_tree::wptree::value_type v, jsonChild){
					AppMessageStatusInfo statusInfo;
					statusInfo.appId = v.second.get<int32_t>(L"app_id");
					statusInfo.msgSeq = v.second.get<int64_t>(L"seq");
					statusInfo.status = v.second.get<int16_t>(L"status", -1);
					statusInfo.externalData = StringConvert::ToUTF8Str(v.second.get<wstring>(L"externalData", L""));
					statusList.push_back(statusInfo);
				}
			}
		}
		catch (boost::property_tree::ptree_error & e) {
			UC_LOG(ERROR) << "encounter an exception when parse json from OnGetOAStatusResponse , exception: " << e.what();
			result = JSON_PARSE_ERROR;
		}
	}
	else {
		result = errorCode != UC_SUCCEEDED ? errorCode : (NULL != response && 200 != response->GetStatusCode() ? response->GetStatusCode() : UC_FAILED);
	}

	if (UC_SUCCEEDED != result){
		for (std::list<AppMessageInfo>::const_iterator iter = infos.begin();
			iter != infos.end(); ++iter){
			AppMessageStatusInfo info;
			info.appId = iter->appId;
			info.msgSeq = iter->msgSeq;
			info.status = -1;
			statusList.push_back(info);
		}
	}
	
	triggerEvent(IOpenApiServiceEvent, OnOAMessageStatusReceived(result, statusList));
	return result;
}

int32_t OpenApiService::OnUpdateOAMessageStatusResponse(int32_t errorCode, HTTPResponse *response, const std::list<AppMessageStatusInfo> infos){

	UC_LOG(INFO) << "OnGetOAStatusResponse errorCode:" << errorCode << " response:" << (response != NULL ? response->GetContent() : "");

	int32_t result = UC_FAILED;
	std::list<AppMessageStatusInfo> statusList;
	if (UC_SUCCEEDED == errorCode && NULL != response && 200 == response->GetStatusCode()) {
		wstringstream wsStream;
		wsStream.str(StringConvert::FromUTF8Str(response->GetContent()).c_str());
		boost::property_tree::wptree jsonRoot;
		try{
			boost::property_tree::json_parser::read_json(wsStream, jsonRoot);
			result = jsonRoot.get<int32_t>(L"code");
			if (UC_SUCCEEDED == result) {
				boost::property_tree::wptree jsonChild = jsonRoot.get_child(L"data");
				BOOST_FOREACH(boost::property_tree::wptree::value_type v, jsonChild){
					AppMessageStatusInfo statusInfo;
					statusInfo.appId = v.second.get<int32_t>(L"app_id");
					statusInfo.msgSeq = v.second.get<int64_t>(L"seq");
					statusInfo.status = v.second.get<int16_t>(L"status", -1);
					statusInfo.externalData = StringConvert::ToUTF8Str(v.second.get<wstring>(L"externalData", L""));
					statusList.push_back(statusInfo);
				}
			}
		}
		catch (boost::property_tree::ptree_error & e) {
			UC_LOG(ERROR) << "encounter an exception when parse json from OnGetOAStatusResponse , exception: " << e.what();
			result = JSON_PARSE_ERROR;
		}
	}
	else {
		result = errorCode != UC_SUCCEEDED ? errorCode : (NULL != response && 200 != response->GetStatusCode() ? response->GetStatusCode() : UC_FAILED);
	}

	if (UC_SUCCEEDED != result){
		statusList = infos;
	}

	triggerEvent(IOpenApiServiceEvent, OnOAMessageStatusUpdated(result, statusList));
	return result;

}

int32_t OpenApiService::OnGetMessageAckStatusResponse(int32_t errorCode, HTTPResponse *response, const std::list<AppMessageInfo> infos){

	UC_LOG(INFO) << "OnGetMessageAckStatusResponse errorCode:" << errorCode << " response:" << (response != NULL ? response->GetContent() : "");

	int32_t result = UC_FAILED;
	std::list<AppMessageStatusInfo> statusList;
	if (UC_SUCCEEDED == errorCode && NULL != response && 200 == response->GetStatusCode()) {
		wstringstream wsStream;
		wsStream.str(StringConvert::FromUTF8Str(response->GetContent()).c_str());
		boost::property_tree::wptree jsonRoot;
		try{
			boost::property_tree::json_parser::read_json(wsStream, jsonRoot);
			result = jsonRoot.get<int32_t>(L"code");
			if (UC_SUCCEEDED == result) {
				boost::property_tree::wptree jsonChild = jsonRoot.get_child(L"data");
				BOOST_FOREACH(boost::property_tree::wptree::value_type v, jsonChild){
					AppMessageStatusInfo statusInfo;
					statusInfo.appId = v.second.get<int32_t>(L"app_id");
					statusInfo.msgSeq = v.second.get<int64_t>(L"seq");
					statusInfo.status = v.second.get<int16_t>(L"acksent", 1);
					statusList.push_back(statusInfo);
				}
			}
		}
		catch (boost::property_tree::ptree_error & e) {
			UC_LOG(ERROR) << "encounter an exception when parse json from OnGetMessageAckStatusResponse , exception: " << e.what();
			result = JSON_PARSE_ERROR;
		}
	}
	else {
		result = errorCode != UC_SUCCEEDED ? errorCode : (NULL != response && 200 != response->GetStatusCode() ? response->GetStatusCode() : UC_FAILED);
	}

	if (UC_SUCCEEDED != result){
		for (std::list<AppMessageInfo>::const_iterator iter = infos.begin();
			iter != infos.end(); ++iter){
			AppMessageStatusInfo info;
			info.appId = iter->appId;
			info.msgSeq = iter->msgSeq;
			info.status = 1;
			statusList.push_back(info);
		}
	}

	triggerEvent(IOpenApiServiceEvent, OnMessageAckStatusReceived(result, statusList));
	return result;
}

int32_t OpenApiService::OnSendMessageResponse(int32_t errorCode, HTTPResponse *response, int64_t appId, int32_t msgId) {

	UC_LOG(INFO) << "OnSendMessageResponse AppId:" << appId << " MsgId:" << msgId
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

	if (UC_SUCCEEDED == result) {
		theStore.GetMsgStore()->UpdateOpenAPIMessageState(appId, msgId, STATE_SENDSUCCESS, msgSeq);
	} else {
		theStore.GetMsgStore()->UpdateOpenAPIMessageState(appId, msgId, STATE_SENDFAIL, msgSeq);
	}

	triggerEvent(IOpenApiServiceEvent, OnMessageSent(result, appId, msgId, msgSeq, sendTime));
	return result;
}

int32_t OpenApiService::OnSendRequestResponse(int32_t errorCode, HTTPResponse *response, int32_t requestId){

	UC_LOG(INFO) << "OnSendRequestResponse  errorCode:" << errorCode << " response:" << (response != NULL ? response->GetContent() : "");

	int32_t result = UC_FAILED;
	std::string respContent = "";
	if (UC_SUCCEEDED == errorCode && NULL != response && 200 == response->GetStatusCode()) {
		result = UC_SUCCEEDED;
		respContent = response->GetContent();
	}
	else {
		result = errorCode != UC_SUCCEEDED ? errorCode : (NULL != response && 200 != response->GetStatusCode() ? response->GetStatusCode() : UC_FAILED);
	}

	triggerEvent(IOpenApiServiceEvent, OnResponseReceived(result, requestId, respContent));
	return 0;
}

int32_t OpenApiService::OnSendMessageReadResponse(int32_t errorCode, HTTPResponse *response, int64_t appId, int32_t readType,
	int32_t msgId, int64_t msgSeq){
	
	UC_LOG(INFO) << "OnSendMessageReadResponse UserId:" << appId << "ReadType:" << readType <<
		"MsgId:" << msgId << "MsgSeq:" << msgSeq << "errorCode" << errorCode << "response:" << (response != NULL ? response->GetContent() : "");

	int32_t result = UC_FAILED;
	if (UC_SUCCEEDED == errorCode && NULL != response && 200 == response->GetStatusCode()) {
		wstringstream wsStream;
		wsStream.str(StringConvert::FromUTF8Str(response->GetContent()).c_str());
		boost::property_tree::wptree jsonRoot;
		try {
			boost::property_tree::json_parser::read_json(wsStream, jsonRoot);
			result = jsonRoot.get<int32_t>(L"code");
		}
		catch (boost::property_tree::ptree_error & e) {
			UC_LOG(ERROR) << "encounter an exception when parse json from OnSendMessageReadResponse , exception: " << e.what();
			result = JSON_PARSE_ERROR;
		}
	}
	else {
		result = errorCode != UC_SUCCEEDED ? errorCode : (NULL != response && 200 != response->GetStatusCode() ? response->GetStatusCode() : UC_FAILED);
	}

	triggerEvent(IOpenApiServiceEvent, OnMessageRead(result, appId, readType, msgId, msgSeq));
	return result;
}

/**
 * 从ucas过来的消息只保存在memory中,database中不存储;
 * history返回的消息将存储在memory和database中;
 */
int32_t OpenApiService::OnMessageReceived(MessageChannel channel, UCMsg &msg, const string &body, MsgSendState state){
	
	UCHead head;
	msg.GetHead(head);
	bool isSave = false;

	switch(msg.GetMsgType()) {
	case UCAPIText: {
		OnTextMessageNotify(channel, head, msg,state, isSave);
		break;
	}
	case UCAPIImage: {
		OnMediaMessageNotify(channel, head, msg, state,isSave);
		break;
	}
	case UCAPIFile: {
		OnFileMessageNotify(channel, head, msg, state,isSave);
		break;
	}
	case UCAPIOA:{
		OnOAMessageNotify(channel, head, msg, state,isSave);
		break;
	}
	case UCAPICustomizedContent:{
		OnCustomizedNotify(channel, head, msg, state, isSave);
		break;
	}
	case UCAPIOAStatusChange:{
		OnOAStatusChangedNotify(channel, head, msg,state, isSave);
		break;
	}
	case UCAPIAppEventChange: {
		OnAppEventChangeNotify(channel, head, msg, state, isSave);
		break;
	}
	case UCAPIAppChangeNotify: {
		OnAppStatusChangeNotify(channel, head, msg, state, isSave);
		break;
	}
	case UCAPIAck: {
		OnAckSentMessageNotify(channel, head, msg,state, isSave);
		break;
	}
	case UCAckMessageRead: {
		OnMessageReadNotify(channel, head, msg, state, isSave);
		break;
	}
	case UCAckMessageBatchRead: {
		OnMessageReadNotify(channel, head, msg, state, isSave);
		break;
	}
	default:
		break;
	}

	if (isSave && UCAS == channel){
		int64_t relate_id = MessageProcessor::GetRelateId(OPENAPI_CHAT, GetCurUser().GetUserId(), head.GetFrom().GetUserId(), head.GetTo().GetUserId());
		MessageProcessor msgProcessor((UCEngine*)_engine);
		if (msgProcessor.SaveMessage(OPENAPI_CHAT, relate_id, 0, 0, STATE_SENDSUCCESS, &msg, body) != UC_SUCCEEDED )
		{
			UC_LOG(ERROR) << "OpenApiService::OnMessageReceived save message failed." << " msgType = " << msg.GetMsgType();
		}
	}

	return UC_SUCCEEDED;
}

void OpenApiService::OnAppStatusChangeNotify(MessageChannel channel, UCHead head, UCMsg& ucMsg, MsgSendState state, bool& isSave){
	isSave = false;
	if (UCAS == channel && IsCurUser(head.GetFrom())) {
		return;
	}

	APIAppChangeNotifyContentMsg *msg = (APIAppChangeNotifyContentMsg*)&ucMsg;
	if (NULL == msg){
		UC_LOG(ERROR) << "OpenApiService::OnAppEventChangeNotify msg is null";
		return;
	}

	MessageInfo messageInfo;
	messageInfo.InfoFromUCHead(head);
	messageInfo.SetSendState(state);

	triggerEvent(IOpenApiServiceEvent, OnAppStatusChangedReceived(channel, messageInfo, msg->get_appId(), msg->get_event(),
		msg->get_externalData()));
}
void OpenApiService::OnAppEventChangeNotify(MessageChannel channel, UCHead head, UCMsg& ucMsg, MsgSendState state, bool& isSave){
	isSave = false;
	if (UCAS == channel && IsCurUser(head.GetFrom())) {
		return;
	}
	APIAppEventChangeNotifyMsg *msg = (APIAppEventChangeNotifyMsg*)&ucMsg;
	if (NULL == msg){
		UC_LOG(ERROR) << "OpenApiService::OnAppEventChangeNotify msg is null";
		return;
	}

	MessageInfo messageInfo;
	messageInfo.InfoFromUCHead(head);
	messageInfo.SetSendState(state);

	triggerEvent(IOpenApiServiceEvent, OnAppEventChangedReceived(channel, messageInfo, msg->get_appId(), msg->get_eventCount(),
		msg->get_externalData()));
}

void OpenApiService::OnTextMessageNotify(MessageChannel channel, UCHead head, UCMsg& ucMsg, MsgSendState state, bool& isSave){

	isSave = false;
	if (UCAS == channel && IsCurUser(head.GetFrom())) {
		return;
	}

	if (UCAS == channel)
	{
		head.SetId(DBManager::GetInstance().GetMessageID());
		ucMsg.SetHead(head);
	}
	
	APITextContentMsg *msg = (APITextContentMsg*)&ucMsg;
	if (NULL == msg){
		UC_LOG(ERROR) << "OpenApiService::OnTextNofity msg is null";
		return ;
	}

	isSave = true;
	MessageInfo messageInfo;
	messageInfo.InfoFromUCHead(head);
	messageInfo.SetSendState(state);
	if (CHAT == channel || CHAT == HISTORY){
		MessageStoreItem msgItem;
		int64_t relate_id = MessageProcessor::GetRelateId(OPENAPI_CHAT, GetCurUser().GetUserId(),
			head.GetFrom().GetUserId(), head.GetTo().GetUserId());
		if (theStore.GetMsgStore()->GetOpenApiMessageById(relate_id, head.GetSeq(), msgItem)){
			messageInfo.SetAppAckSent(msgItem.app_ack_sent);
		}
	}else if (LIST == channel){
		ConversationInfo info;
		if (theStore.GetConvStore()->GetConversationMessage(head.GetSeq(), info)){
			messageInfo.SetAppAckSent(info.GetAppAckSent());
		}
	}

	triggerEvent(IOpenApiServiceEvent, OnTextMessageReceived(channel, messageInfo, msg->get_title(), msg->get_content(),
		msg->get_detailType(), msg->get_detailContent(), msg->get_detailAuth()));
}

void OpenApiService::OnMediaMessageNotify(MessageChannel channel, UCHead head, UCMsg& ucMsg, MsgSendState state, bool& isSave){
	
	isSave = false;
	if (UCAS == channel && IsCurUser(head.GetFrom())) {
		return;
	}

	if (UCAS == channel)
	{
		head.SetId(DBManager::GetInstance().GetMessageID());
		ucMsg.SetHead(head);
	}

	APIImageContentMsg *msg = (APIImageContentMsg*)&ucMsg;
	if (NULL == msg){
		UC_LOG(ERROR) << "OpenApiService::OnMediaNotify msg is null";
		return ;
	}

	isSave = true;
	MessageInfo messageInfo;

	messageInfo.InfoFromUCHead(head);
	messageInfo.SetSendState(state);

	if (CHAT == channel || CHAT == HISTORY){
		MessageStoreItem msgItem;
		int64_t relate_id = MessageProcessor::GetRelateId(OPENAPI_CHAT, GetCurUser().GetUserId(),
			head.GetFrom().GetUserId(), head.GetTo().GetUserId());
		if (theStore.GetMsgStore()->GetOpenApiMessageById(relate_id, head.GetSeq(), msgItem)){
			messageInfo.SetAppAckSent(msgItem.app_ack_sent);
		}
	}
	else if (LIST == channel){
		ConversationInfo info;
		if (theStore.GetConvStore()->GetConversationMessage(head.GetSeq(), info)){
			messageInfo.SetAppAckSent(info.GetAppAckSent());
		}
	}

	triggerEvent(IOpenApiServiceEvent, OnImageMessageReceived(channel, messageInfo, msg->get_mediaId(),
		msg->get_detailType(), msg->get_mediaThumb()));
}

void OpenApiService::OnFileMessageNotify(MessageChannel channel, UCHead head, UCMsg& ucMsg, MsgSendState state,bool& isSave){
	
	isSave = false;
	if (UCAS == channel && IsCurUser(head.GetFrom())) {
		return;
	}

	if (UCAS == channel)
	{
		head.SetId(DBManager::GetInstance().GetMessageID());
		ucMsg.SetHead(head);
	}

	APIFileContentMsg *msg = (APIFileContentMsg*)&ucMsg;
	if (NULL == msg){
		UC_LOG(ERROR) << "OpenApiService::OnSendFileNotify msg is null";
		return ;
	}

	isSave = true;
	MessageInfo messageInfo;

	messageInfo.InfoFromUCHead(head);
	messageInfo.SetSendState(state);

	if (CHAT == channel || channel == HISTORY){
		MessageStoreItem msgItem;
		int64_t relate_id = MessageProcessor::GetRelateId(OPENAPI_CHAT, GetCurUser().GetUserId(),
			head.GetFrom().GetUserId(), head.GetTo().GetUserId());
		if (theStore.GetMsgStore()->GetOpenApiMessageById(relate_id, head.GetSeq(), msgItem)){
			messageInfo.SetAppAckSent(msgItem.app_ack_sent);
		}
	}
	else if (LIST == channel){
		ConversationInfo info ;
		if (theStore.GetConvStore()->GetConversationMessage(head.GetSeq(), info)){
			messageInfo.SetAppAckSent(info.GetAppAckSent());
		}
	}

	triggerEvent(IOpenApiServiceEvent, OnFileMessageReceived(channel, messageInfo, msg->get_mediaId(),
		msg->get_detailType(), msg->get_fileName(), msg->get_fileSize()));
}

void OpenApiService::OnAckSentMessageNotify(MessageChannel channel, UCHead head, UCMsg& ucMsg, MsgSendState state,bool& isSave){
	
	isSave = false;
	if (UCAS == channel && IsCurUser(head.GetFrom())) {
		return;
	}
	
	APIAckSentContentMsg *msg = (APIAckSentContentMsg*)&ucMsg;
	if (NULL == msg){
		UC_LOG(ERROR) << "OpenApiService::OnAckSentMessageNotify msg is null";
		return;
	}

	MessageInfo messageInfo;
	messageInfo.InfoFromUCHead(head);
	messageInfo.SetSendState(state);

	triggerEvent(IOpenApiServiceEvent, OnPushMessageAck(channel, messageInfo, msg->get_result(),
		msg->get_id(), msg->get_seq(), msg->get_conversation()));

	if (UCAS == channel){
		theStore.GetMsgStore()->UpdateOpenAPIMessageAckState(head.GetFrom().GetUserId(), msg->get_seq(), msg->get_result());
		theStore.GetConvStore()->UpdataConversationMessageAckState(msg->get_seq(), msg->get_result());
	}
}

void OpenApiService::OnOAMessageNotify(MessageChannel channel, UCHead head, UCMsg& ucMsg, MsgSendState state, bool& isSave){
	
	isSave = false;
	if (UCAS == channel && IsCurUser(head.GetFrom())) {
		return;
	}
	if (UCAS == channel)
	{
		head.SetId(DBManager::GetInstance().GetMessageID());
		ucMsg.SetHead(head);
	}

	APIOAContentMsg *msg = (APIOAContentMsg*)&ucMsg;
	if (NULL == msg){
		UC_LOG(ERROR) << "OpenApiService::OnOAMessageNotify msg is null";
		return;
	}

	isSave = true;
	MessageInfo messageInfo;
	messageInfo.InfoFromUCHead(head);
	messageInfo.SetSendState(state);

	if (CHAT == channel || CHAT == HISTORY){
		MessageStoreItem msgItem;
		int64_t relate_id = MessageProcessor::GetRelateId(OPENAPI_CHAT, GetCurUser().GetUserId(),
			head.GetFrom().GetUserId(), head.GetTo().GetUserId());
		if (theStore.GetMsgStore()->GetOpenApiMessageById(relate_id, head.GetSeq(), msgItem)){
			messageInfo.SetOALastStatus(msgItem.oa_last_status);
			messageInfo.SetOAExternalData(msgItem.oa_external_data);
		}
	}
	else if (LIST == channel){
		ConversationInfo info;
		if (theStore.GetConvStore()->GetConversationMessage(head.GetSeq(), info)){
			messageInfo.SetOALastStatus(info.GetOALastStatus());
			messageInfo.SetOAExternalData(info.GetOAExternalData());
		}
	}
	
	triggerEvent(IOpenApiServiceEvent, OnOAMessageReceived(channel, messageInfo, msg->get_title(), 
		msg->Get_APIOAContent().__isset.status? msg->get_status(): -1, msg->get_color(),
		msg->get_bkColor(), msg->get_elements(), msg->get_detailURL(), msg->get_detailAuth(), 
		msg->get_titleElements(), msg->get_titleStyle(), msg->get_customizedType(), msg->get_customizedData()));
}

void OpenApiService::OnCustomizedNotify(MessageChannel channel, UCHead head, UCMsg& ucMsg, MsgSendState state, bool& isSave){

	isSave = false;
	if (UCAS == channel && IsCurUser(head.GetFrom())) {
		return;
	}
	if (UCAS == channel)
	{
		head.SetId(DBManager::GetInstance().GetMessageID());
		ucMsg.SetHead(head);
	}

	APICustomizedContentMsg *msg = (APICustomizedContentMsg*)&ucMsg;
	if (NULL == msg){
		UC_LOG(ERROR) << "OpenApiService::OnCustomizedNotify msg is null";
		return;
	}

	MessageInfo messageInfo;
	messageInfo.InfoFromUCHead(head);
	messageInfo.SetSendState(state);


	triggerEvent(IOpenApiServiceEvent, OnCustomizedMessageReceived(channel, messageInfo, msg->get_data1(), 
		msg->get_data2(), msg->get_data3(), msg->get_data4()));
}

void OpenApiService::OnOAStatusChangedNotify(MessageChannel channel, UCHead head, UCMsg& ucMsg, MsgSendState state,bool& isSave){
	
	isSave = false;
	if (UCAS == channel && IsCurUser(head.GetFrom())) {
		return;
	}

	APIOAStatusChangeContentMsg *msg = (APIOAStatusChangeContentMsg*)&ucMsg;
	if (NULL == msg){
		UC_LOG(ERROR) << "OpenApiService::OnOAStatusChangedNotify msg is null";
		return;
	}

	MessageInfo messageInfo;
	messageInfo.InfoFromUCHead(head);
	messageInfo.SetSendState(state);

	AppMessageStatusInfo statusInfo;
	statusInfo.appId = head.GetFrom().GetUserId();
	statusInfo.msgSeq = msg->get_seq();
	statusInfo.status = msg->get_newStatus();
	statusInfo.externalData = msg->get_externalData();

	triggerEvent(IOpenApiServiceEvent, OnOAMessageStatusChangedReceived(channel, messageInfo, statusInfo));

	if (UCAS == channel){
		theStore.GetMsgStore()->UpdateOpenAPIOAMessageState(statusInfo.appId, statusInfo.msgSeq, statusInfo.status, statusInfo.externalData);
		theStore.GetConvStore()->UpdataConversationOAMessageState(statusInfo.msgSeq, statusInfo.status, statusInfo.externalData);
	}
}

void OpenApiService::OnMessageReadNotify(MessageChannel channel, UCHead head, UCMsg& ucMsg, MsgSendState state, bool& isSave){
	
	isSave = false;
	if (UCAS == channel && IsCurUser(head.GetFrom())) {
		return;
	}

	AckReadMsg *msg = (AckReadMsg*)&ucMsg;
	if (NULL == msg){
		UC_LOG(ERROR) << "OpenApiService::OnMessageReadNotify msg is null";
		return;
	}
	
	if (SessionType::APIType == msg->get_sessionType()){
		triggerEvent(IOpenApiServiceEvent, OnMessageRead(UC_SUCCEEDED, head.GetTo().GetUserId(), 0, msg->get_id(), msg->get_seq()));
	}
}

void OpenApiService::OnMessageBatchReadNotify(MessageChannel channel, UCHead head, UCMsg& ucMsg, MsgSendState state, bool& isSave){
	isSave = false;
	if (UCAS == channel && IsCurUser(head.GetFrom())) {
		return;
	}

	BatchAckReadMsg *msg = (BatchAckReadMsg*)&ucMsg;
	if (NULL == msg){
		UC_LOG(ERROR) << "OpenApiService::OnMessageReadNotify msg is null";
		return;
	}
	if (SessionType::APIType == msg->get_sessionType()){
		triggerEvent(IOpenApiServiceEvent, OnMessageRead(UC_SUCCEEDED, head.GetTo().GetUserId(), 1, 0, 0));
	}
	
}

} /* namespace uc */
