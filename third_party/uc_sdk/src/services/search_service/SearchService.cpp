/*
 * ClientService.cpp
 *
 *  Created on: Feb 9, 2015
 *      Author: ethan
 */

#include "SearchService.h"
#include "DBManager.h"
#include "HTTPRequestCallback.h"
#include "client_service/ClientService.h"
#include "message_service/GroupService.h"
#include "message_service/MessageProcessor.h"
#include "calendar_service/CalendarService.h"
#include <boost/filesystem.hpp>
#include "CommonDef.h"
#include "UCEngine.h"
#include "UCErrorCode.h"


namespace uc {

using namespace std;

SearchService::SearchService(IUCEngine *engine) {
	_engine = engine;
	_engine->registerListener(this);
}

SearchService::~SearchService() {
	_engine->removeListener(this);
}

int SearchService::InitRequest(const string& key, int32_t page, int32_t count, const string& scope, HTTPRequest& request) {
	stringstream	sStream;
	ClientService* clientService = (ClientService*)_engine->QueryService(UCClientService);

	if(clientService == NULL) {
		UC_LOG(ERROR) << "Query client service return NULL!";
		return UC_FAILED;
	}

	const LoginInfo& loginInfo = clientService->GetLoginInfo();
	sStream << UCEngine::GetRequestURL(loginInfo.GetUccServerDomain(), SEARCH_MESSAGE_LIST_METHOD)
	<< "?session_id=" << loginInfo.GetSessionId() << "&user_id=" << loginInfo.GetUserId()
	<< "&customer_code=" << loginInfo.GetCustomerCode() << "&query=" << StringConvert::URLEncode(key) << "&scope=" << scope
	<< "&page=" << page << "&count=" << count;
	request.Init(sStream.str(), HTTP_METHOD_GET);

	UC_LOG(WARNING) << "Search server request url:" << sStream.str();

	return UC_SUCCEEDED;
}

int SearchService::SearchServer(const string& key, int32_t page, int32_t count, const std::string& scope, int requestId) {
	HTTPRequest	request;
	if (InitRequest(key, page, count, scope, request) != 0) {
		UC_LOG(ERROR) << "SearchServer InitRequest return error, key:" << key << ", page:" << page << ", count:" << count << ", scope:" << scope;
		return UC_FAILED;
	}

	_engine->SendAsyncRequest(request, MakeRequestCallback(this, &SearchService::OnServerSearched, requestId));
	return UC_SUCCEEDED;
}

int SearchService::SearchMessage(ConversationType type, int64_t relateId, const string& key, int32_t page, int32_t count, int requestId){

	HTTPRequest	request;

	ClientService* clientService = (ClientService*)_engine->QueryService(UCClientService);
	if (clientService == NULL) {
		UC_LOG(ERROR) << "Query client service return NULL!";
		return UC_FAILED;
	}
	const LoginInfo& loginInfo = clientService->GetLoginInfo();
	stringstream	sStream;

	sStream << UCEngine::GetRequestURL(loginInfo.GetUccServerDomain(), SEARCH_MESSAGE_LIST_METHOD)
		<< "?session_id=" << loginInfo.GetSessionId() << "&user_id=" << loginInfo.GetUserId()
		<< "&customer_code=" << loginInfo.GetCustomerCode() << "&query=" << StringConvert::URLEncode(key) << "&scope=[" << 31
		<< "]&page=" << page << "&count=" << count << "&to_user_id=" << relateId;

	if (type == CONFERENCE_CHAT || type == GROUP_CHAT || type == PROJECT_CHAT){
		sStream << "&group_type=" << type;
	}else if (type == CLOUD_CHAT){
		sStream << "&group_type=" << 3;
	}
	
	request.Init(sStream.str(), HTTP_METHOD_GET);

	UC_LOG(WARNING) << "Search server request url:" << sStream.str();

	_engine->SendAsyncRequest(request, MakeRequestCallback(this, &SearchService::OnServerSearched, requestId));
	return UC_SUCCEEDED;
}

int SearchService::SearchLocal(const string& key, int32_t page, int32_t count, const std::list<int32_t>& scope, int requestId) {
	list<ContactInfo*>contacts;
	list<GroupInfo> groups;
	int type = 0;
	for (std::list<int32_t>::const_iterator itor = scope.begin();
		itor != scope.end(); ++itor) {
			type = *itor;
			if (type == 1) { 
				//contact
				DBManager::GetInstance().SearchContact(key, page, count, contacts);
			} else if (type == 2) {
				//group
				DBManager::GetInstance().SearchGroup(key, page, count, groups);
			} else {
				UC_LOG(WARNING) << "Invalid scope! scope type:" << type;
			}
	}
	triggerEvent(ISearchServiceEvent, OnLocalSearched(UC_SUCCEEDED, requestId, contacts, groups));
	DelContainer(contacts);
	groups.clear();
	return UC_SUCCEEDED;
}

int32_t SearchService::OnServerSearched(int32_t errorCode, HTTPResponse* response, int requestId) {
	UC_LOG(WARNING) << "Search server response:" << response->GetContent().c_str();
	list<ContactInfo*> contacts;
	list<GroupInfo> groups;
	list<GroupInfo> groupChats;
	list<Meeting*> conferences;
	list<GroupInfo> cloudGroups;
	list<SearchMessageInfo*> messages;
	int32_t chatMsgNum = 0;
	list<MessageInfo*> chatMessages;
	if (errorCode != 0 || response->GetStatusCode() != 200) {
		triggerEvent(ISearchServiceEvent, OnServerSearched(UC_FAILED, requestId, contacts, groups, groupChats, cloudGroups, 
			conferences, messages, chatMsgNum, chatMessages));
		UC_LOG(ERROR) << "OnAllSearched response error:" << errorCode << ", response status code:" << response->GetStatusCode();
		return -1;
	}

	boost::property_tree::wptree ptRoot;
	int code = GetResponseCode(response, ptRoot);
	if (code != 0) {
		triggerEvent(ISearchServiceEvent, OnServerSearched(code, requestId, contacts, groups, groupChats, cloudGroups, conferences, messages, chatMsgNum, chatMessages));
		UC_LOG(ERROR) << "OnAllSearched response code:" << code;
		return code;
	}

	chatMsgNum = ptRoot.get<int32_t>(L"p2p_group_msg_count", 0);
	boost::property_tree::wptree ptData;
	ptData = ptRoot.get_child(L"data");
	BOOST_FOREACH(const boost::property_tree::wptree::value_type &value, ptData){
		const boost::property_tree::wptree& arrayValue = value.second;

		if (arrayValue.find(L"user") != arrayValue.not_found()) {
			const boost::property_tree::wptree& userArray = arrayValue.get_child(L"user");
			GetContacts(userArray, contacts);
		}

		if (arrayValue.find(L"group") != arrayValue.not_found()) {
			const boost::property_tree::wptree& groupArray = arrayValue.get_child(L"group");
			GetGroups(groupArray, groups);
		}

		if (arrayValue.find(L"group_chat") != arrayValue.not_found()) {
			const boost::property_tree::wptree& groupChatArray = arrayValue.get_child(L"group_chat");
			GetGroups(groupChatArray, groupChats);
		}

		if (arrayValue.find(L"cloud_group") != arrayValue.not_found()) {
			const boost::property_tree::wptree& cloudGroupArray = arrayValue.get_child(L"cloud_group");
			GetGroups(cloudGroupArray, cloudGroups);
		}

		if (arrayValue.find(L"conf") != arrayValue.not_found()) {
			const boost::property_tree::wptree& confArray = arrayValue.get_child(L"conf");
			GetConferences(confArray, conferences);
		}

		if (arrayValue.find(L"msg") != arrayValue.not_found()) {
			const boost::property_tree::wptree& msgArray = arrayValue.get_child(L"msg");
			GetGlobalMessages(msgArray, messages);
		}

		if (arrayValue.find(L"p2p_group_msg") != arrayValue.not_found()) {
			const boost::property_tree::wptree& msgArray = arrayValue.get_child(L"p2p_group_msg");
			GetMessages(msgArray, chatMessages);
		}
	}
	triggerEvent(ISearchServiceEvent, OnServerSearched(code, requestId, contacts, groups, groupChats, cloudGroups, conferences, messages, chatMsgNum, chatMessages));
	DelContainer(contacts);
	DelContainer(conferences);
	DelContainer(messages);
	DelContainer(chatMessages);

	return code;
}


int SearchService::GetResponseCode(HTTPResponse* response, boost::property_tree::wptree& ptRoot) {
	wstringstream jsonStream;
	jsonStream.str(StringConvert::FromUTF8Str(response->GetContent().c_str()));
	boost::property_tree::json_parser::read_json(jsonStream, ptRoot);
	return ptRoot.get<int32_t>(L"code");
}

void SearchService::GetContacts(const boost::property_tree::wptree& ptParse, list<ContactInfo*>& contacts) {
	BOOST_FOREACH(const boost::property_tree::wptree::value_type &value, ptParse){
		boost::property_tree::wptree jsonValue = value.second;
		if (!jsonValue.empty()) {
			ContactInfoImpl* impl = ContactInfoImpl::CreateContactInfoImpl(jsonValue,false);
			string localPath;
			if (DBManager::GetInstance().GetFSLocalPath(impl->GetRemoteAvatarUrl(), localPath, true) != UC_SUCCEEDED) {
				localPath = "";
			}
			else {
				if (!localPath.empty()) {
					boost::filesystem::path path = StringConvert::FromUTF8Str(localPath);
					if (!boost::filesystem::exists(path) || boost::filesystem::file_size(path) <= 0) {
						localPath = "";
					}
				}
			}
			impl->SetLocalAvatar(localPath);
			ContactInfo* info = new ContactInfo(impl);
			contacts.push_back(info);
		}
	}
}

void SearchService::GetGroups(const boost::property_tree::wptree& ptParse, list<GroupInfo>& groups) {
	GroupService* groupService = (GroupService*)_engine->QueryService(UCGroupService);
	if(groupService == NULL) {
		UC_LOG(ERROR) << "Query group service return NULL!";
		return;
	}

	BOOST_FOREACH(const boost::property_tree::wptree::value_type &value, ptParse){
		boost::property_tree::wptree jsonValue = value.second;
		if (!jsonValue.empty()) {
			std::wstringstream ss;
			boost::property_tree::json_parser::write_json(ss, jsonValue);

			rapidjson::Document document;
			document.Parse<0>(StringConvert::ToUTF8Str(ss.str()).c_str());

			GroupInfo info;
			groupService->ParseGroupInfo(document, info, true);
			groups.push_back(info);
		}
	}
}

void SearchService::GetConferences(const boost::property_tree::wptree& ptParse, list<Meeting*>& meetings) {
 	CalendarService* calendarService = (CalendarService*)_engine->QueryService(UCCalendarService);
 	if(calendarService == NULL) {
 		UC_LOG(ERROR) << "Query calendar service return NULL!";
 		return;
 	}

	BOOST_FOREACH(const boost::property_tree::wptree::value_type &value, ptParse){
 		boost::property_tree::wptree jsonValue = value.second;
		if (!jsonValue.empty()) {
			Meeting* meeting = new Meeting();
			calendarService->ParseMeeting(jsonValue, meeting);
			meetings.push_back(meeting);
		}
	}
}

void SearchService::GetMessages(const boost::property_tree::wptree& ptParse, list<MessageInfo*>& messages) {
	MessageProcessor msgProcessor((UCEngine*)_engine);
	BOOST_FOREACH(const boost::property_tree::wptree::value_type &value, ptParse){
		boost::property_tree::wptree jsonValue = value.second;
		if (!jsonValue.empty()) {
			MessageInfo* info = msgProcessor.GetMessageInfo(jsonValue);
			if (info != NULL) {
				try{
					info->SetMatchCount(jsonValue.get<int64_t>(L"match_count"));
					if (info->GetAppId() == AppId::AppMeeting){
						info->SetSearchEventId(jsonValue.get<int32_t>(L"event_id"));
					}	
				}
				catch (boost::property_tree::ptree_error& e){
					UC_LOG(ERROR) << "parse match_count and event_Id, exception: " << e.what();
				}	
				messages.push_back(info);
			}
		}
	}
	messages.sort([&](MessageInfo* firstItem, MessageInfo* secondItem)->bool{ return firstItem->GetMsgSeq() > secondItem->GetMsgSeq(); });
}

void SearchService::GetGlobalMessages(const boost::property_tree::wptree& ptParse, SearchMessageList& messages){

	MessageProcessor msgProcessor((UCEngine*)_engine);

	list<MessageInfo*> msgList;
	GetMessages(ptParse, msgList);

	for (auto &msginfo : msgList){
		ConversationType type = msgProcessor.GetConversationType(msginfo->GetAppId(), msginfo->GetProtocolType());
		int64_t relateId = msgProcessor.GetRelateId(type, GetCurUser().GetUserId(), msginfo->GetFrom().GetUserId(), msginfo->GetTo().GetUserId());	
	
		SearchMessageList::iterator iter = std::find_if(messages.begin(), messages.end(), [&](SearchMessageInfo* item){ return item->GetRelateId() == relateId; });
		if (iter != messages.end()){	
			SearchMessageInfo * info = *iter;
			info->AddMessageInfo(msginfo);
			info->SetTimeStamp(info->GetTimeStamp() > msginfo->GetMsgTime() ? info->GetTimeStamp() : msginfo->GetMsgTime());
		}else{
			SearchMessageInfo * info = new SearchMessageInfo;
			info->SetRelateId(relateId);
			info->SetConversationType(type);
			info->SetTimeStamp(msginfo->GetMsgTime());
			info->AddMessageInfo(msginfo);
			info->SetMatchCounts(msginfo->GetMatchCount());
			info->SetEventId(msginfo->GetSearchEventId());
			messages.push_back(info);
		}
	}
	
	messages.sort([&](SearchMessageInfo* firstItem, SearchMessageInfo* secondItem)->bool{ return firstItem->GetTimeStamp() > secondItem->GetTimeStamp(); });
}

UCID SearchService::GetCurUser(){
	UCID userUid;
	if (_engine){
		userUid = ((ClientService*)_engine->QueryService(UCClientService))->GetLoginInfo().GetCurUser();
	}
	return 	userUid;
}


} /* namespace uc */
