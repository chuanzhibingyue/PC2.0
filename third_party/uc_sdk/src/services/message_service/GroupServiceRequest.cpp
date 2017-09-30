/*
 * GroupService.cpp
 *
 *  Created on: Jun 15, 2015
 *      Author: yongshuai.cheng
 */

#include "GroupService.h"
#include "UCEngine.h"
#include "common/StringConvert.h"
#include "HTTPRequestCallback.h"
#include "client_service/ClientService.h"
#include "boost/property_tree/json_parser.hpp"
#include "MessageProcessor.h"
#include "DBManager.h"
#include "UCErrorCode.h"
#include "CommonDef.h"
#include "store_service/StoreService.h"
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include "audio_service/CallDef.h"

namespace uc {

const int64_t BATCH_UPLOAD_DOCUMENT_DELAY = 1000;
int32_t GroupService::GetGroupList(GroupStyle groupType, int32_t page /* = 1 */, int32_t count /* = 200 */, int32_t lastGroupTime /* = 0 */, bool allData, bool update){

	HTTPRequest     request;
	stringstream	wsStream;
	string         	targetURL;

	// 1.先从store中获取
	GroupList grouplist;
	GET_STATUS status = theStore.GetGroupListStore()->GetGroupList(groupType, page, count, lastGroupTime, allData, update, grouplist);
	if (status != GROUPLIST_NEED_FROM_SERVER)
	{
		triggerEvent(IGroupServiceEvent, OnGroupListReceived(UC_SUCCEEDED, groupType, allData, grouplist));
		return UC_SUCCEEDED;
	}

	// 2. store中不是最新的, 从服务获取增量数据
	// 2.1 cache 当前的请求参数（给异步回调转发）
	if (!update)
	{
		boost::unique_lock<boost::recursive_mutex> lock(_grouplist_status_mutex);
		list_group_call_cache_item cache_item;
		cache_item._type = groupType;
		cache_item._page = page;
		cache_item._count = count;
		cache_item._last_time = lastGroupTime;
		cache_item._all_data = allData;
		_list_group_call_cache.push_back(cache_item);

		if (_sync_status == STATUS_DOING)
			return UC_SUCCEEDED;

		if (_sync_status == STATUS_DONE)
		{
			UC_LOG(ERROR) << "_sync_status is Done, but there is no data in db or memory... get data again!!!";
		}
	}
	// 2.2 更新状态 (不用反复请求)
	_sync_status = STATUS_DOING;

	// 2.3 grouplist空，说明DB中也是空的，从服务获取全部grouplist
	if (grouplist.empty())
	{
		lastGroupTime = 0;
	}
	else // 2.4 获取增量的grouplist
	{
		lastGroupTime = [](GroupList & list)->int32_t{
			int32_t ret = 0;
			for (GroupInfo info : list)
			{
				if (info.GetModifyTime() > ret)
					ret = info.GetModifyTime();
			}
			return ret - 1;
		}(grouplist);
	}
	targetURL = ((UCEngine*)_engine)->GetRequestURL(((ClientService*)_engine->QueryService(UCClientService))->GetLoginInfo().GetUccServerDomain(),
		GROUP_LIST_METHOD);

	wsStream << targetURL << "?user_id=" << GetCurUser().GetUserId() <<
		"&session_id=" << ((ClientService*)_engine->QueryService(UCClientService))->GetLoginInfo().GetSessionId() <<
		"&page=" << page << "&count=" << count << "&last_group_time=" << lastGroupTime << "&sort=" << 1;


	request.Init(wsStream.str(), HTTP_METHOD_GET);

	UC_LOG(INFO) << "GroupService::GetGroupList request";
	_engine->SendAsyncRequest(request, MakeRequestCallback(this, &GroupService::OnGetGroupListResponse, groupType, update, allData));

	return UC_SUCCEEDED;
}

int32_t GroupService::GetGroupInfo(int64_t groupId, GroupInfo &groupInfo, bool forced, int32_t mountId){
	if (groupId == 0 && mountId == 0) {
		UC_LOG(ERROR) << "GetGroupInfo parameter error, groupId and mountId can not both be 0";
		return UC_FAILED;
	}

	bool ret;
	if (!forced)
	{
		if (groupId != 0) {
			ret = theStore.GetGroupListStore()->GetGroupInfo(groupId, groupInfo);
		}
		else if (mountId != 0){
			ret = theStore.GetGroupListStore()->GetGroupInfoByMountId(mountId, groupInfo);
		}

		if (ret){
			return UC_LOCALDATA_SUCCESSED;
		}

		UC_LOG(WARNING) << "get GetGroupInfo, but there is no Data in memory, groupId=" << groupId;
	}
	HTTPRequest     request;
	stringstream	wsStream;
	string         	targetURL;

	targetURL = ((UCEngine*)_engine)->GetRequestURL(((ClientService*)_engine->QueryService(UCClientService))->GetLoginInfo().GetUccServerDomain(),
		GROUP_INFO_METHOD);

	if (groupId != 0) {
		wsStream << targetURL << "?group_id=" << groupId;
	}
	else if (mountId != 0){
		wsStream << targetURL << "?mount_id=" << mountId;
	}

	wsStream << "&user_id=" << GetCurUser().GetUserId() <<
		"&session_id=" << ((ClientService*)_engine->QueryService(UCClientService))->GetLoginInfo().GetSessionId();

	request.Init(wsStream.str(), HTTP_METHOD_GET);
	_engine->SendAsyncRequest(request, MakeRequestCallback(this, &GroupService::OnGetGroupInfoResponse, groupId, mountId));

	return UC_SUCCEEDED;
}

void GroupService::SetGroupCreateRequest(GroupStyle groupType, const std::string& groupName, const std::string& logourl,
	std::vector<UserInfo>& userInfo, int32_t nameFlag, int32_t memberType, int32_t mountId, HTTPRequest& request){

	stringstream	sStream;
	string         	targetURL;

	boost::property_tree::wptree jsonRoot;
	boost::property_tree::wptree jsonChild;
	boost::property_tree::wptree jsonElement;

	targetURL = ((UCEngine*)_engine)->GetRequestURL(((ClientService*)_engine->QueryService(UCClientService))->GetLoginInfo().GetUccServerDomain(),
		GROUP_CREATE_METHOD);

	request.Init(targetURL, HTTP_METHOD_POST);
	request.AddHeaderInfo("Content-Type", "application/x-www-form-urlencoded");

	for (unsigned int i = 0; i < userInfo.size(); ++i){
		jsonElement.put(L"user_id", userInfo[i].ucid.GetUserId());
		jsonElement.put(L"name_pinyin", StringConvert::FromUTF8Str(userInfo[i].userName));
		jsonElement.put(L"is_admin", userInfo[i].role);
		jsonChild.push_back(std::make_pair(L"", jsonElement));
	}

	jsonRoot.put_child(L"member", jsonChild);
	wstringstream sData;
	boost::property_tree::json_parser::write_json(sData, jsonRoot);

	sStream << "session_id=" << ((ClientService*)_engine->QueryService(UCClientService))->GetLoginInfo().GetSessionId() <<
		"&user_id=" << GetCurUser().GetUserId() << "&group_name=" << StringConvert::URLEncode(groupName)
		<< "&data=" << StringConvert::URLEncode(StringConvert::ToUTF8Str(sData.str())) << "&name_flag=" << nameFlag << "&is_display=" << (int32_t)groupType
		<< "&group_status=" << (memberType << 2);

	if (!logourl.empty()) {
		sStream << "&logo_url=" << StringConvert::URLEncode(logourl);
	}

	if (groupType == CLOUD_GROUP_TYPE) {
		sStream << "&cloud_library_info={\"mount_id\": " << mountId << "}";
	}

	request.SetBody(sStream.str());
}

int32_t GroupService::CreateGroup(GroupStyle groupType, const std::string& groupName, const std::string& logourl,
	std::vector<UserInfo>& userInfo, int32_t nameFlag, int32_t memberType, int32_t mountId){
	HTTPRequest     request;
	SetGroupCreateRequest(groupType, groupName, logourl, userInfo, nameFlag, memberType, mountId, request);
	_engine->SendAsyncRequest(request, MakeRequestCallback(this, &GroupService::OnCreateGroupResponse, mountId));
	return UC_SUCCEEDED;
}

int32_t GroupService::CreateCallGroup(GroupStyle groupType, const std::string& groupName,
	std::vector<UserInfo>& userInfo, AudioService* service) {
	HTTPRequest     request;
	SetGroupCreateRequest(groupType, groupName, "", userInfo, 1, 0, 0, request);
	_engine->SendAsyncRequest(request, MakeRequestCallback(this, &GroupService::OnCreateCallGroupResponse, service));
	return UC_SUCCEEDED;
}

int32_t GroupService::CreateCallGroup(GroupStyle groupType, const std::string& groupName,
	std::vector<UserInfo>& userInfo, AudioService* service, int conferenceId, int callType) {
	HTTPRequest     request;
	SetGroupCreateRequest(groupType, groupName, "", userInfo, 1, 0, 0, request);
	_engine->SendAsyncRequest(request, MakeRequestCallback(this, &GroupService::OnCreateCallGroupResponse, service, conferenceId, callType));
	return UC_SUCCEEDED;
}

int32_t GroupService::AddMemberToGroupCall(int64_t groupId, const std::vector<UserInfo>& memberList,
	int conferenceId, int callType, AudioService* service) {
	HTTPRequest     request;
	SetAddMemberRequest(groupId, memberList, request);
	_engine->SendAsyncRequest(request, MakeRequestCallback(this, &GroupService::OnAddMemberToGroupCallResponse, groupId, memberList, conferenceId, callType, service));
	return UC_SUCCEEDED;
}

int32_t GroupService::EndGroup(int64_t groupId){

	HTTPRequest     request;
	stringstream	sStream;
	string         	targetURL;

	targetURL = ((UCEngine*)_engine)->GetRequestURL(((ClientService*)_engine->QueryService(UCClientService))->GetLoginInfo().GetUccServerDomain(),
		GROUP_END_METHOD);

	request.Init(targetURL, HTTP_METHOD_POST);
	request.AddHeaderInfo("Content-Type", "application/x-www-form-urlencoded");

	sStream << "session_id=" << ((ClientService*)_engine->QueryService(UCClientService))->GetLoginInfo().GetSessionId() <<
		"&user_id=" << GetCurUser().GetUserId() << "&group_id=" << groupId;

	request.SetBody(sStream.str());
	_engine->SendAsyncRequest(request, MakeRequestCallback(this, &GroupService::OnEndGroupResponse, groupId));

	return UC_SUCCEEDED;
}

int32_t GroupService::ExitGroup(int64_t groupId){

	HTTPRequest     request;
	stringstream	sStream;
	string         	targetURL;

	targetURL = ((UCEngine*)_engine)->GetRequestURL(((ClientService*)_engine->QueryService(UCClientService))->GetLoginInfo().GetUccServerDomain(),
		GROUP_EXIT_METHOD);

	request.Init(targetURL, HTTP_METHOD_POST);
	request.AddHeaderInfo("Content-Type", "application/x-www-form-urlencoded");

	sStream << "session_id=" << ((ClientService*)_engine->QueryService(UCClientService))->GetLoginInfo().GetSessionId() <<
		"&user_id=" << GetCurUser().GetUserId() << "&group_id=" << groupId;

	request.SetBody(sStream.str());
	_engine->SendAsyncRequest(request, MakeRequestCallback(this, &GroupService::OnExitGroupResponse, groupId));

	return UC_SUCCEEDED;
}
int32_t GroupService::SetAddMemberRequest(int64_t groupId, const std::vector<UserInfo>& memberList, HTTPRequest& request) {
	if (0 == memberList.size()){
		return UC_SUCCEEDED;
	}

	stringstream	sStream;
	string         	targetURL;

	boost::property_tree::wptree jsonRoot;
	boost::property_tree::wptree jsonChild;
	boost::property_tree::wptree jsonElement;

	targetURL = ((UCEngine*)_engine)->GetRequestURL(((ClientService*)_engine->QueryService(UCClientService))->GetLoginInfo().GetUccServerDomain(),
		GROUP_MEMEBER_ADD_METHOD);

	request.Init(targetURL, HTTP_METHOD_POST);
	request.AddHeaderInfo("Content-Type", "application/x-www-form-urlencoded");

	jsonRoot.put(L"group_id", groupId);
	for (unsigned int i = 0; i < memberList.size(); ++i){

		jsonElement.put(L"user_id", memberList[i].ucid.GetUserId());
		jsonElement.put(L"name_pinyin", StringConvert::FromUTF8Str(memberList[i].userName));
		jsonElement.put(L"is_admin", memberList[i].role);
		jsonChild.push_back(std::make_pair(L"", jsonElement));
	}
	jsonRoot.put_child(L"member", jsonChild);

	wstringstream sData;
	boost::property_tree::json_parser::write_json(sData, jsonRoot);

	sStream << "session_id=" << ((ClientService*)_engine->QueryService(UCClientService))->GetLoginInfo().GetSessionId() <<
		"&user_id=" << GetCurUser().GetUserId() << "&data=" << StringConvert::URLEncode(StringConvert::ToUTF8Str(sData.str()));

	request.SetBody(sStream.str());
	return UC_SUCCEEDED;
}

int32_t GroupService::AddMember(int64_t groupId, const std::vector<UserInfo>& memberList){
	HTTPRequest     request;
	SetAddMemberRequest(groupId, memberList, request);
	_engine->SendAsyncRequest(request, MakeRequestCallback(this, &GroupService::OnAddMemberResponse, groupId, memberList));

	return UC_SUCCEEDED;
}

int32_t GroupService::SetAdmin(int64_t groupId, SetAdminType type, const std::vector<int32_t>& adminList) {
	if (0 == adminList.size()){
		return ERROR_INPUT_PARAMETER;
	}

	HTTPRequest     request;
	stringstream	sStream;
	string         	targetURL;

	targetURL = ((UCEngine*)_engine)->GetRequestURL(((ClientService*)_engine->QueryService(UCClientService))->GetLoginInfo().GetUccServerDomain(),
		GROUP_SET_ADMIN_METHOD);

	request.Init(targetURL, HTTP_METHOD_POST);
	request.AddHeaderInfo("Content-Type", "application/x-www-form-urlencoded");

	rapidjson::Document doc;
	rapidjson::Value value;
	value.SetArray();
	for (unsigned int i = 0; i < adminList.size(); ++i){
		value.PushBack(adminList[i], doc.GetAllocator());
	}

	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	value.Accept(writer);

	std::string adminStr = buffer.GetString();

	sStream << "session_id=" << ((ClientService*)_engine->QueryService(UCClientService))->GetLoginInfo().GetSessionId() <<
		"&user_id=" << GetCurUser().GetUserId() << "&type=" << type
		<< "&admin_list=" << StringConvert::URLEncode(adminStr.c_str());

	request.SetBody(sStream.str());
	_engine->SendAsyncRequest(request, MakeRequestCallback(this, &GroupService::OnSetAdminResponse, groupId, type, adminList));

	return UC_SUCCEEDED;
}

int32_t GroupService::RemoveMember(int64_t groupId, const std::vector<int32_t>& memberList){

	if (memberList.size() == 0){
		return UC_SUCCEEDED;
	}
	HTTPRequest     request;
	stringstream	sStream;
	string         	targetURL;

	boost::property_tree::wptree jsonRoot;
	boost::property_tree::wptree jsonChild;
	boost::property_tree::wptree jsonElement;

	targetURL = ((UCEngine*)_engine)->GetRequestURL(((ClientService*)_engine->QueryService(UCClientService))->GetLoginInfo().GetUccServerDomain(),
		GROUP_MEMEBER_DEL_METHOD);
	request.Init(targetURL, HTTP_METHOD_POST);
	request.AddHeaderInfo("Content-Type", "application/x-www-form-urlencoded");

	for (unsigned int i = 0; i < memberList.size(); i++){
		jsonElement.put_value<int32_t>(memberList[i]);
		jsonChild.push_back(std::make_pair(L"", jsonElement));
	}
	jsonRoot.put_child(L"member_ids", jsonChild);
	jsonRoot.put(L"user_id", GetCurUser().GetUserId());
	wstringstream sData;
	boost::property_tree::json_parser::write_json(sData, jsonRoot);

	sStream << "session_id=" << ((ClientService*)_engine->QueryService(UCClientService))->GetLoginInfo().GetSessionId() <<
		"&user_id=" << GetCurUser().GetUserId() << "&group_id=" << groupId << "&data=" << StringConvert::ToUTF8Str(sData.str());

	request.SetBody(sStream.str());
	_engine->SendAsyncRequest(request, MakeRequestCallback(this, &GroupService::OnRemoveMemberResponse, groupId, memberList));

	return UC_SUCCEEDED;
}

int32_t GroupService::SetGroupName(int64_t groupId, const std::string& groupName){

	HTTPRequest     request;
	stringstream	sStream;
	string         	targetURL;
	boost::property_tree::wptree jsonData;

	targetURL = ((UCEngine*)_engine)->GetRequestURL(((ClientService*)_engine->QueryService(UCClientService))->GetLoginInfo().GetUccServerDomain(),
		GROUP_NAME_UPDATE_METHOD);

	request.Init(targetURL, HTTP_METHOD_POST);
	request.AddHeaderInfo("Content-Type", "application/x-www-form-urlencoded");

	jsonData.put(L"group_id", groupId);
	jsonData.put(L"group_name", StringConvert::FromUTF8Str(groupName));

	wstringstream sData;
	boost::property_tree::json_parser::write_json(sData, jsonData);

	sStream << "session_id=" << ((ClientService*)_engine->QueryService(UCClientService))->GetLoginInfo().GetSessionId() <<
		"&user_id=" << GetCurUser().GetUserId() << "&data=" << StringConvert::URLEncode(StringConvert::ToUTF8Str(sData.str()));
	request.SetBody(sStream.str());
	_engine->SendAsyncRequest(request, MakeRequestCallback(this, &GroupService::OnSetGroupNameResponse, groupId));

	return UC_SUCCEEDED;
}

int32_t GroupService::GetDocumentList(int64_t groupId, int32_t page, int32_t count){

	// 	DocumentMap::iterator  iter = _documentMap.find(groupId);
	// 	if (iter != _documentMap.end()){
	// 		triggerEvent(IGroupServiceEvent, OnDocListReceived(UC_SUCCEEDED, groupId, iter->second));
	// 		return UC_SUCCEEDED;
	// 	}

	HTTPRequest     request;
	stringstream	sStream;
	string         	targetURL;

	targetURL = ((UCEngine*)_engine)->GetRequestURL(((ClientService*)_engine->QueryService(UCClientService))->GetLoginInfo().GetUccServerDomain(),
		GROUP_DOC_LIST_METHOD);

	sStream << targetURL << "?session_id=" << ((ClientService*)_engine->QueryService(UCClientService))->GetLoginInfo().GetSessionId() <<
		"&user_id=" << GetCurUser().GetUserId() << "&group_id=" << groupId << "&page=" << page << "&count=" << count;

	request.Init(sStream.str(), HTTP_METHOD_GET);
	_engine->SendAsyncRequest(request, MakeRequestCallback(this, &GroupService::OnGetDocListResponse, groupId));

	return UC_SUCCEEDED;
}

int32_t GroupService::UploadDocument(int64_t groupId, int32_t operation, int32_t isSendMessage, const DocumentInfo& info,
	int32_t& msgId) {

	msgId = DBManager::GetInstance().GetMessageID();

	boost::property_tree::wptree jsonData;
	ConvertDocumentToJson(groupId, operation, info, false, jsonData);

	wstringstream sData;
	boost::property_tree::json_parser::write_json(sData, jsonData);

	HTTPRequest     request;
	stringstream	sStream;
	string         	targetURL;
	const LoginInfo &loginInfo = ((ClientService*)_engine->QueryService(UCClientService))->GetLoginInfo();

	targetURL = ((UCEngine*)_engine)->GetRequestURL(loginInfo.GetUccServerDomain(), GROUP_DOC_SEND_METHOD);

	request.Init(targetURL, HTTP_METHOD_POST);
	request.AddHeaderInfo("Content-Type", "application/x-www-form-urlencoded");

	sStream << "session_id=" << loginInfo.GetSessionId() <<
		"&user_id=" << GetCurUser().GetUserId() << "&group_id=" << groupId << "&operation=" << operation << "&content_type=" << 0 <<
		"&data=" << StringConvert::URLEncode(StringConvert::ToUTF8Str(sData.str())) << "&need_send_msg=" << isSendMessage;

	request.SetBody(sStream.str());
	_engine->SendAsyncRequest(request, MakeRequestCallback(this, &GroupService::OnUploadDocumentResponse, groupId, 0, operation, msgId, info.GetFileName()));
	return UC_SUCCEEDED;
}

int32_t GroupService::BatchUploadDocument(int64_t groupId, int32_t operation, const DocumentInfo &info) {
	DocumentUploadOperation opt;
	opt.operation = operation;
	opt.info = info;

	boost::unique_lock<boost::recursive_mutex> lock(_batchUploadOptsMutex);

	std::map<int64_t, std::list<DocumentUploadOperation>>::iterator iter = _batchUploadOpts.find(groupId);
	if (iter != _batchUploadOpts.end()) {
		iter->second.push_back(opt);
	}
	else {
		std::list<DocumentUploadOperation> optList;
		optList.push_back(opt);

		_batchUploadOpts[groupId] = std::move(optList);
	}

	if (_batchUploadTimerId < 0) {
		Timer::GetInstance().Schedule(MakeCallback(this, &GroupService::OnBatchUploadDocumentFired),
			BATCH_UPLOAD_DOCUMENT_DELAY, BATCH_UPLOAD_DOCUMENT_DELAY, &_batchUploadTimerId);
	}
	lock.unlock();

	return UC_SUCCEEDED;
}

int32_t GroupService::UploadCodeSnippet(int64_t groupId, int32_t operation, const DocumentInfo& info, int32_t& msgId)
{
	msgId = DBManager::GetInstance().GetMessageID();
	HTTPRequest     request;
	stringstream	sStream;
	string         	targetURL;
	const LoginInfo &loginInfo = ((ClientService*)_engine->QueryService(UCClientService))->GetLoginInfo();

	targetURL = ((UCEngine*)_engine)->GetRequestURL(loginInfo.GetUccServerDomain(), GROUP_DOC_SEND_METHOD);

	request.Init(targetURL, HTTP_METHOD_POST);
	request.AddHeaderInfo("Content-Type", "application/x-www-form-urlencoded");

	boost::property_tree::wptree jsonData;

	jsonData.put(L"group_id", groupId);
	jsonData.put(L"user_id", GetCurUser().GetUserId());
	jsonData.put(L"user_name", StringConvert::FromUTF8Str(GetCurUserName()));
	jsonData.put(L"file_url", StringConvert::FromUTF8Str(info.GetFileUrl()));
	jsonData.put(L"title", StringConvert::FromUTF8Str(info.GetFileName()));
	jsonData.put(L"format", StringConvert::FromUTF8Str(info.GetFileFormat()));
	jsonData.put(L"size", info.GetFileSize());
	jsonData.put(L"total_lines", info.GetTotalLines());
	jsonData.put(L"thumbnail", StringConvert::FromUTF8Str(info.GetThumbnail()));
	jsonData.put(L"description", StringConvert::FromUTF8Str(info.GetDescription()));

	wstringstream sData;
	boost::property_tree::json_parser::write_json(sData, jsonData);

	sStream << "session_id=" << loginInfo.GetSessionId() <<
		"&user_id=" << GetCurUser().GetUserId() << "&group_id=" << groupId << "&operation=" << operation << "&content_type=" << 1 <<
		"&data=" << StringConvert::URLEncode(StringConvert::ToUTF8Str(sData.str()));

	request.SetBody(sStream.str());
	_engine->SendAsyncRequest(request, MakeRequestCallback(this, &GroupService::OnUploadDocumentResponse, groupId, 1, operation, msgId, info.GetFileName()));

	return UC_SUCCEEDED;
}

int32_t GroupService::DeleteDocument(int64_t groupId, int64_t fileId){

	HTTPRequest     request;
	stringstream	sStream;
	string         	targetURL;

	targetURL = ((UCEngine*)_engine)->GetRequestURL(((ClientService*)_engine->QueryService(UCClientService))->GetLoginInfo().GetUccServerDomain(),
		GROUP_DOC_DELETE_METHOD);

	request.Init(targetURL, HTTP_METHOD_POST);
	request.AddHeaderInfo("Content-Type", "application/x-www-form-urlencoded");

	sStream << "session_id=" << ((ClientService*)_engine->QueryService(UCClientService))->GetLoginInfo().GetSessionId() <<
		"&user_id=" << GetCurUser().GetUserId() << "&group_id=" << groupId << "&file_id=" << fileId;

	request.SetBody(sStream.str());
	_engine->SendAsyncRequest(request, MakeRequestCallback(this, &GroupService::OnDeleteDocumentResponse, groupId, fileId));

	return UC_SUCCEEDED;
}

int32_t GroupService::GetDocumentInfo(int64_t groupId, int64_t fileId, const std::string &cloudFileInfo){

	// 	boost::unique_lock<boost::recursive_mutex> lock(_documentMutex);
	// 	const DocumentInfo* docInfo = GetDocumentInfoById(groupId, fileId);
	// 	if (NULL != docInfo){
	// 		triggerEvent(IGroupServiceEvent, OnDocInfoReceived(UC_SUCCEEDED, groupId, *docInfo));
	// 		return UC_SUCCEEDED;
	// 	}

	HTTPRequest     request;
	stringstream	sStream;
	string         	targetURL;

	targetURL = ((UCEngine*)_engine)->GetRequestURL(((ClientService*)_engine->QueryService(UCClientService))->GetLoginInfo().GetUccServerDomain(),
		GROUP_DOC_INFO_METHOD);

	sStream << targetURL << "?user_id=" << GetCurUser().GetUserId() <<
		"&session_id=" << ((ClientService*)_engine->QueryService(UCClientService))->GetLoginInfo().GetSessionId();
	if (fileId != 0) {
		sStream << "&file_id=" << fileId;
	}
	else if (!cloudFileInfo.empty()){
		sStream << "&cloud_file_info=" << StringConvert::URLEncode(cloudFileInfo);
	}
	else {
		return EMPTY_FILEID_CLOUDFILEINFO;
	}

	request.Init(sStream.str(), HTTP_METHOD_GET);
	_engine->SendAsyncRequest(request, MakeRequestCallback(this, &GroupService::OnGetDocumentInfoResponse, groupId, fileId));

	return UC_SUCCEEDED;
}

int32_t GroupService::SetGroupLogo(int64_t groupId, const std::string& logoURL){

	UC_LOG(ERROR) << "GroupService::SetGroupLogo " << " groupId = " << groupId << "  LogoUrl = " << logoURL;

	HTTPRequest     request;
	stringstream	sStream;
	string         	targetURL;

	targetURL = ((UCEngine*)_engine)->GetRequestURL(((ClientService*)_engine->QueryService(UCClientService))->GetLoginInfo().GetUccServerDomain(),
		GROUP_LOGO_SET_METHOD);

	request.Init(targetURL, HTTP_METHOD_POST);
	request.AddHeaderInfo("Content-Type", "application/x-www-form-urlencoded");

	sStream << "session_id=" << ((ClientService*)_engine->QueryService(UCClientService))->GetLoginInfo().GetSessionId() <<
		"&user_id=" << GetCurUser().GetUserId() << "&group_id=" << groupId << "&logo_url=" << StringConvert::URLEncode(logoURL.c_str());

	request.SetBody(sStream.str());
	_engine->SendAsyncRequest(request, MakeRequestCallback(this, &GroupService::OnSetGroupLogoResponse, groupId, logoURL));

	return UC_SUCCEEDED;
}

int32_t GroupService::SetTopChat(int64_t groupId, int32_t state){

	HTTPRequest     request;
	stringstream	sStream;
	string         	targetURL;

	targetURL = ((UCEngine*)_engine)->GetRequestURL(((ClientService*)_engine->QueryService(UCClientService))->GetLoginInfo().GetUccServerDomain(),
		GROUP_TOP_SET_METHOD);

	request.Init(targetURL, HTTP_METHOD_POST);
	request.AddHeaderInfo("Content-Type", "application/x-www-form-urlencoded");

	sStream << "session_id=" << ((ClientService*)_engine->QueryService(UCClientService))->GetLoginInfo().GetSessionId() <<
		"&user_id=" << GetCurUser().GetUserId() << "&group_id=" << groupId << "&is_top=" << state;

	request.SetBody(sStream.str());
	_engine->SendAsyncRequest(request, MakeRequestCallback(this, &GroupService::OnSetTotChatResponse, groupId, state));

	return UC_SUCCEEDED;
}

int32_t GroupService::SetDoNotDisturb(int64_t groupId, int32_t state){

	HTTPRequest     request;
	stringstream	sStream;
	string         	targetURL;

	targetURL = ((UCEngine*)_engine)->GetRequestURL(((ClientService*)_engine->QueryService(UCClientService))->GetLoginInfo().GetUccServerDomain(),
		GROUP_NODISTURB_SET_METHOD);

	request.Init(targetURL, HTTP_METHOD_POST);
	request.AddHeaderInfo("Content-Type", "application/x-www-form-urlencoded");

	sStream << "session_id=" << ((ClientService*)_engine->QueryService(UCClientService))->GetLoginInfo().GetSessionId() <<
		"&user_id=" << GetCurUser().GetUserId() << "&group_id=" << groupId << "&type=" << state;

	request.SetBody(sStream.str());
	_engine->SendAsyncRequest(request, MakeRequestCallback(this, &GroupService::OnSetDisturbResponse, groupId, state));

	return UC_SUCCEEDED;
}

int32_t GroupService::SetReminder(int64_t groupId, int32_t state, int32_t alertRule, int32_t alertTime){

	HTTPRequest     request;
	stringstream	sStream;
	string         	targetURL;

	targetURL = ((UCEngine*)_engine)->GetRequestURL(((ClientService*)_engine->QueryService(UCClientService))->GetLoginInfo().GetUccServerDomain(),
		GROUP_REMIND_SET_METHOD);
	request.Init(targetURL, HTTP_METHOD_POST);
	request.AddHeaderInfo("Content-Type", "application/x-www-form-urlencoded");

	sStream << "session_id=" << ((ClientService*)_engine->QueryService(UCClientService))->GetLoginInfo().GetSessionId() <<
		"&user_id=" << GetCurUser().GetUserId() << "&group_id=" << groupId << "&rule=" << alertRule <<
		"&remindtime=" << alertTime << "&type=" << state;
	request.SetBody(sStream.str());

	_engine->SendAsyncRequest(request, MakeRequestCallback(this, &GroupService::OnSetReminderResponse, groupId, state, alertRule, alertTime));

	return UC_SUCCEEDED;
}

int32_t GroupService::GetCommentList(int64_t groupId, int64_t fileId, int32_t count, int64_t startTime, int64_t endTime){

	HTTPRequest     request;
	stringstream	sStream;
	string         	targetURL;

	targetURL = ((UCEngine*)_engine)->GetRequestURL(((ClientService*)_engine->QueryService(UCClientService))->GetLoginInfo().GetUccServerDomain(),
		GROUP_COMMENT_LIST_METHOD);

	sStream << targetURL << "?session_id=" << ((ClientService*)_engine->QueryService(UCClientService))->GetLoginInfo().GetSessionId() <<
		"&user_id=" << GetCurUser().GetUserId() << "&file_id=" << fileId << "&count=" << count << "&start_time=" << startTime <<
		"&end_time=" << endTime;

	request.Init(sStream.str(), HTTP_METHOD_GET);
	_engine->SendAsyncRequest(request, MakeRequestCallback(this, &GroupService::OnGetCommentListResponse, groupId, fileId));

	return UC_SUCCEEDED;
}

int32_t GroupService::CreateComment(int64_t groupId, int64_t fileId, int32_t fileType, const std::string& fileTitle,
	const std::string content, int32_t& msgId){

	msgId = DBManager::GetInstance().GetMessageID();

	HTTPRequest     request;
	stringstream	sStream;
	string         	targetURL;

	targetURL = ((UCEngine*)_engine)->GetRequestURL(((ClientService*)_engine->QueryService(UCClientService))->GetLoginInfo().GetUccServerDomain(),
		GROUP_CREATE_COMMENT_METHOD);
	request.Init(targetURL, HTTP_METHOD_POST);
	request.AddHeaderInfo("Content-Type", "application/x-www-form-urlencoded");

	boost::property_tree::wptree jsonData;
	jsonData.put(L"content", StringConvert::FromUTF8Str(StringConvert::URLEncode(content)));
	jsonData.put(L"file_id", fileId);
	jsonData.put(L"content_type", fileType);
	jsonData.put(L"title", StringConvert::FromUTF8Str(fileTitle));
	jsonData.put(L"user_name", StringConvert::FromUTF8Str(GetCurUserName()));
	jsonData.put(L"to_user_id", GetCurUser().GetUserId());

	wstringstream sData;
	boost::property_tree::json_parser::write_json(sData, jsonData);

	sStream << "session_id=" << ((ClientService*)_engine->QueryService(UCClientService))->GetLoginInfo().GetSessionId() <<
		"&user_id=" << GetCurUser().GetUserId() << "&group_id=" << groupId <<
		"&data=" << StringConvert::URLEncode(StringConvert::ToUTF8Str(sData.str()));
	request.SetBody(sStream.str());

	_engine->SendAsyncRequest(request, MakeRequestCallback(this, &GroupService::OnCreateCommentResponse, groupId, fileId, content, msgId));

	return UC_SUCCEEDED;

}

int32_t GroupService::GetChatHistory(MessageChannel channel, ConversationType type, int64_t groupId, int64_t count, int64_t startTime, int64_t endTime){

	std::list<MessageStoreItem*> msgList;
	int64_t starttime_t = startTime;
	int64_t endtime_t = endTime;
	int32_t update_flag_msgid = 0;

	MSG_STATUS status = theStore.GetMsgStore()->GetChatHistory(groupId, type, count, startTime, endTime, update_flag_msgid, msgList);
	if (status != MSG_NEED_FROM_SERVER)
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
		triggerEvent(IGroupServiceEvent, OnChatHistoryReceived(UC_SUCCEEDED, channel, type, groupId, msgList.size(), min_timestamp));

		UC_LOG(INFO) << "group_service OnChatHistoryReceived from cache. result: " << UC_SUCCEEDED
			<< " type: " << type << " groupId: " << groupId << " message_count:" << msgList.size() << " min_timestamp: " << min_timestamp;

		//获取当前发送消息的未读用户个数;
		if (!seqList.empty()){
			GetUnreadUserCount(type, groupId, seqList);
		}

		if (MSG_FROM_DB_SUCCESS == status)
		{
			theStore.GetMsgStore()->ClearMessageList(msgList);
		}
		return UC_SUCCEEDED;
	}

	HTTPRequest     request;
	stringstream	sStream;
	string         	targetURL;

	targetURL = ((UCEngine*)_engine)->GetRequestURL(((ClientService*)_engine->QueryService(UCClientService))->GetLoginInfo().GetUccServerDomain(),
		CONTENT_BY_ID_METHOD);

	sStream << targetURL << "?from_user_id=" << GetCurUser().GetUserId() <<
		"&to_user_id=" << groupId << "&type=" << 1 << "&count=" << count << "&start_time=" << startTime << "&end_time=" << endTime <<
		"&session_id=" << ((ClientService*)_engine->QueryService(UCClientService))->GetLoginInfo().GetSessionId();
	request.Init(sStream.str(), HTTP_METHOD_GET);
	_engine->SendAsyncRequest(request, MakeRequestCallback(this, &GroupService::OnGetChatHistoryResponse, groupId, channel, type, count, starttime_t, endtime_t, startTime, endTime, update_flag_msgid));

	return UC_SUCCEEDED;
}

int32_t GroupService::GetSearchChatHistory(ConversationType type, int64_t relateId, int64_t count, int64_t startTime, int64_t endTime, int32_t reverse){

		HTTPRequest     request;
		stringstream	sStream;
		string         	targetURL;
		int32_t         relateType;

		targetURL = ((UCEngine*)_engine)->GetRequestURL(((ClientService*)_engine->QueryService(UCClientService))->GetLoginInfo().GetUccServerDomain(),
			CONTENT_BY_ID_METHOD);

		if (GROUP_CHAT == type || PROJECT_CHAT == type || CONFERENCE_CHAT == type || CLOUD_CHAT == type){
			relateType = 1;
		}else if (SINGLE_CALL == type){
			relateType = 0;
		}
		sStream << targetURL << "?from_user_id=" << GetCurUser().GetUserId() <<
			"&to_user_id=" << relateId << "&type=" << relateType << "&count=" << count << "&start_time=" << startTime << "&end_time=" << endTime <<
			"&session_id=" << ((ClientService*)_engine->QueryService(UCClientService))->GetLoginInfo().GetSessionId();
		if (reverse == 1){
			sStream << "&reverse=" << reverse;
		}
	

	request.Init(sStream.str(), HTTP_METHOD_GET);
	_engine->SendAsyncRequest(request, MakeRequestCallback(this, &GroupService::OnGeSearchChatHistoryResponse, relateId, type, count, startTime, endTime));

	return UC_SUCCEEDED;
}

int32_t GroupService::ResendMessage(int32_t msgId){

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
		_engine->SendAsyncRequest(request, MakeRequestCallback(this, &GroupService::OnSendMessageResponse, msgItem.conversation_type, 
			(int64_t)head.GetTo().GetUserId(), msgId, true));
		return UC_SUCCEEDED;
	}

	return UC_FAILED;
}

int32_t GroupService::ForwardMessage(UCID groupUid, int32_t msgId) {

// 	MessageInfo message;
// 	//TODO: marked by jeffrey, waiting for xuejian
// 	//bool result = theStore.GetMsgStore()->GetMessageById( msgId, messageInfo );
// 	bool result = false;
// 	if(DBError::DB_SUCCESS == result){
// 		HTTPRequest	request;
// 		MessageProcessor msgProcessor((UCEngine*)_engine);
// 		if (UC_SUCCEEDED == msgProcessor.CreateForwardMessage(GetCurUser(), groupUid, message, request)){
// 			_engine->SendAsyncRequest(request,
// 					MakeRequestCallback(this, &GroupService::OnSendMessageResponse, message.GetConversationType(), (int64_t)message.GetTo().GetUserId(), msgId));
// 			return UC_SUCCEEDED;
// 		}
// 	}
// 	UC_LOG(ERROR) << "get message failed from DB, errcode=" << result;
	return UC_FAILED;
}


int32_t GroupService::GetRevocationConversationType(ConversationType type){
	int32_t conversationType = -1;

	if (type == CONFERENCE_CHAT || type == PROJECT_CHAT || type == GROUP_CHAT || type == CLOUD_CHAT){
		conversationType = Revocation::GROUP_TYPE;
	}
	if (type == SINGLE_CHAT){
		conversationType = Revocation::SINGLE_TYPE;
	}
	if (type == ORG_CHAT){
		conversationType = Revocation::ORG_TYPE;
	}
	if (type == OPENAPI_CHAT){
		conversationType = Revocation::AppId_TYPE;
	}

	return conversationType;
}

int32_t GroupService::RevocationMessage(int64_t& touserId, ConversationType type, const int64_t& seqId, OUT int32_t& msgId){

	msgId = DBManager::GetInstance().GetMessageID();

	HTTPRequest     request;
	stringstream	sStream;
	string         	targetURL;

	targetURL = ((UCEngine*)_engine)->GetRequestURL(((ClientService*)_engine->QueryService(UCClientService))->GetLoginInfo().GetUccServerDomain(),
		MESSAGE_REVOCATION_METHOD);

	request.Init(targetURL, HTTP_METHOD_POST);
	request.AddHeaderInfo("Content-Type", "application/x-www-form-urlencoded");

	sStream << "session_id=" << ((ClientService*)_engine->QueryService(UCClientService))->GetLoginInfo().GetSessionId() <<
		"&user_id=" << GetCurUser().GetUserId() << "&to_user_id=" << touserId << "&type=" << GetRevocationConversationType(type) << "&seq=" << seqId;

	request.SetBody(sStream.str());
	_engine->SendAsyncRequest(request, MakeRequestCallback(this, &GroupService::OnRevocationMessageResponse, touserId, type, seqId, msgId));

	return UC_SUCCEEDED;
}

int32_t GroupService::SendTextMessage(ConversationType type, UCID groupUid, int8_t textType, const std::string& textFormat, const std::string& textData, const std::vector<UCID>& atUsers, int32_t& msgId) {

	MessageProcessor msgProcessor((UCEngine*)_engine);
	HTTPRequest	request;
	UCMsg *msg = NULL;
	BinMsg data;
	string body;

	msgId = msgProcessor.CreateTextMessage(type, GetCurUser(), groupUid, textType, textFormat, textData, "", "", 0, 0, 0, atUsers, msg);
	if (msg == NULL) {
		return UC_FAILED;
	}

	if (msgProcessor.EncodeMessageData(*msg, data, body) != UC_SUCCEEDED) {
		return UC_FAILED;
	}
	
	if (msgProcessor.SaveMessage(type, groupUid.GetUserId(), 0, 0, STATE_SENDING, msg, body) != UC_SUCCEEDED){
		UC_LOG(ERROR) << "save group message in cache failed. relate_id: " << groupUid.GetUserId();
		delete msg;
		msg = NULL;
		return UC_FAILED;
	}

	if (UC_SUCCEEDED == msgProcessor.CreateMessageRequest(*msg, request)) {
		_engine->SendAsyncRequest(request,MakeRequestCallback(this, &GroupService::OnSendMessageResponse, type, 
			(int64_t)groupUid.GetUserId(), msgId, true));
		delete msg;
		msg = NULL;
		return UC_SUCCEEDED;
	}

	delete msg;
	msg = NULL;
	return UC_FAILED;
}

int32_t GroupService::SendImageMessage(ConversationType type, UCID groupUid, const std::string& imageName, int32_t imageSize,
	const std::string& imageURL, const std::string& imageData, int32_t& msgId){
	MessageProcessor msgProcessor((UCEngine*)_engine);
	HTTPRequest	request;
	UCMsg *msg = NULL;
	BinMsg data;
	string body;

	msgId = msgProcessor.CreateImageMessage(type, GetCurUser(), groupUid, imageName, imageSize, imageURL, imageData, "", -1, 0, 0, 0, msg);
	if (msg == NULL) {
		return UC_FAILED;
	}
	// store
	if (msgProcessor.EncodeMessageData(*msg, data, body) != UC_SUCCEEDED) {
		return UC_FAILED;
	}

	if (msgProcessor.SaveMessage(type, groupUid.GetUserId(), 0, 0, STATE_SENDING, msg, body) != UC_SUCCEEDED){
		UC_LOG(ERROR) << "save group message in cache failed. relate_id: " << groupUid.GetUserId();
		delete msg;
		msg = NULL;
		return UC_FAILED;
	}

	if(UC_SUCCEEDED == msgProcessor.CreateMessageRequest(*msg, request)){
		_engine->SendAsyncRequest(request,
			MakeRequestCallback(this, &GroupService::OnSendMessageResponse, type, (int64_t)groupUid.GetUserId(), msgId, true));
		delete msg;
		msg = NULL;
		return UC_SUCCEEDED;
	}
	delete msg;
	msg = NULL;
	return UC_FAILED;
}

int32_t GroupService::SendAudioMessage(ConversationType type, UCID groupUid, const std::string& audioName, int32_t audioSize,
	const std::string& audioURL, int32_t audioDuration, int32_t& msgId){
	MessageProcessor msgProcessor((UCEngine*)_engine);
	HTTPRequest	request;
	UCMsg *msg = NULL;
	BinMsg data;
	string body;

	msgId = msgProcessor.CreateAudioMessage(type, GetCurUser(), groupUid, audioName, audioSize, audioURL, audioDuration, 0, 0, 0, msg);
	if (msg == NULL) {
		return UC_FAILED;
	}
	// store
	if (msgProcessor.EncodeMessageData(*msg, data, body) != UC_SUCCEEDED) {
		return UC_FAILED;
	}

	if (msgProcessor.SaveMessage(type, groupUid.GetUserId(), 0, 0, STATE_SENDING, msg, body) != UC_SUCCEEDED){
		UC_LOG(ERROR) << "save group message in cache failed. relate_id: " << groupUid.GetUserId();
		delete msg;
		msg = NULL;
		return UC_FAILED;
	}

	if(UC_SUCCEEDED == msgProcessor.CreateMessageRequest(*msg, request)){
		_engine->SendAsyncRequest(request,
			MakeRequestCallback(this, &GroupService::OnSendMessageResponse, type, (int64_t)groupUid.GetUserId(), msgId, true));
		delete msg;
		msg = NULL;
		return UC_SUCCEEDED;
	}
	delete msg;
	msg = NULL;
	return UC_FAILED;
}

int32_t GroupService::SendVideoMessage(ConversationType type, UCID groupUid, const std::string& videoName, int32_t videoSize,
	const std::string& videoURL, const std::string& videoData, int32_t videoDuration, int32_t& msgId) {
	MessageProcessor msgProcessor((UCEngine*)_engine);
	HTTPRequest	request;
	UCMsg *msg = NULL;
	BinMsg data;
	string body;

	msgId = msgProcessor.CreateVideoMessage(type, GetCurUser(), groupUid, videoName, videoSize, videoURL, videoData, videoDuration, 0, 0, 0, msg);
	if (msg == NULL) {
		return UC_FAILED;
	}
	// store
	if (msgProcessor.EncodeMessageData(*msg, data, body) != UC_SUCCEEDED) {
		return UC_FAILED;
	}

	if (msgProcessor.SaveMessage(type, groupUid.GetUserId(), 0, 0, STATE_SENDING, msg, body) != UC_SUCCEEDED){
		UC_LOG(ERROR) << "save group message in cache failed. relate_id: " << groupUid.GetUserId();
		delete msg;
		msg = NULL;
		return UC_FAILED;
	}

	if(UC_SUCCEEDED == msgProcessor.CreateMessageRequest(*msg, request)){
		_engine->SendAsyncRequest(request,
			MakeRequestCallback(this, &GroupService::OnSendMessageResponse, type, (int64_t)groupUid.GetUserId(), msgId, true));
		delete msg;
		msg = NULL;
		return UC_SUCCEEDED;
	}
	delete msg;
	msg = NULL;
	return UC_FAILED;
}


int32_t GroupService::SendReportMessage(ConversationType type, UCID groupUid, const std::string& reportTitle, const std::string& reportUrl, OUT int32_t& msgId){

	MessageProcessor msgProcessor((UCEngine*)_engine);
	HTTPRequest	request;
	UCMsg *msg = NULL;
	BinMsg data;
	string body;

	msgId = msgId = msgProcessor.CreateReportMessage(type, GetCurUser(), groupUid, reportTitle, reportUrl, 0, msg);
	if (msg == NULL) {
		UC_LOG(ERROR) << "create report message failed.";
		return UC_FAILED;
	}
	// store
	if (msgProcessor.EncodeMessageData(*msg, data, body) != UC_SUCCEEDED) {
		UC_LOG(ERROR) << "encode report message failed.";
		return UC_FAILED;
	}

	if (msgProcessor.SaveMessage(type, groupUid.GetUserId(), 0, 0, STATE_SENDING, msg, body) != UC_SUCCEEDED){
		UC_LOG(ERROR) << "save group message in cache failed. relate_id: " << groupUid.GetUserId();
		delete msg;
		msg = NULL;
		return UC_FAILED;
	}

	if (UC_SUCCEEDED == msgProcessor.CreateMessageRequest(*msg, request)){
		_engine->SendAsyncRequest(request,
			MakeRequestCallback(this, &GroupService::OnSendMessageResponse, type, (int64_t)groupUid.GetUserId(), msgId, false));
		delete msg;
		msg = NULL;
		return UC_SUCCEEDED;
	}
	delete msg;
	msg = NULL;
	return UC_FAILED;
}

int32_t GroupService::SendRecordVideoMessage(ConversationType type, UCID groupUid, const std::string& videoName, int32_t videoSize, const std::string& videoURL, 
	const std::string& videoThumb, const std::string& videoThumbURL, int32_t videoDuration, int32_t operatorId, const std::string& operatorName, const std::string& operatorAvatar, int32_t& msgId) {
	MessageProcessor msgProcessor((UCEngine*)_engine);
	HTTPRequest	request;
	UCMsg *msg = NULL;
	BinMsg data;
	string body;

	msgId = msgProcessor.CreateRecordVideoMessage(type, GetCurUser(), groupUid, videoName, videoSize, videoURL, videoThumb, videoThumbURL, videoDuration, operatorId, operatorName, operatorAvatar, 0, 0, 0, msg);
	if (msg == NULL) {
		return UC_FAILED;
	}
	// store
	if (msgProcessor.EncodeMessageData(*msg, data, body) != UC_SUCCEEDED) {
		return UC_FAILED;
	}

	if (msgProcessor.SaveMessage(type, groupUid.GetUserId(), 0, 0, STATE_SENDING, msg, body) != UC_SUCCEEDED){
		UC_LOG(ERROR) << "save group message in cache failed. relate_id: " << groupUid.GetUserId();
		delete msg;
		msg = NULL;
		return UC_FAILED;
	}

	if (UC_SUCCEEDED == msgProcessor.CreateMessageRequest(*msg, request)){
		_engine->SendAsyncRequest(request,
			MakeRequestCallback(this, &GroupService::OnSendMessageResponse, type, (int64_t)groupUid.GetUserId(), msgId, true));
		delete msg;
		msg = NULL;
		return UC_SUCCEEDED;
	}
	delete msg;
	msg = NULL;
	return UC_FAILED;
}


int32_t GroupService::SendCloudFileMessage(ConversationType type, CloudFileOperation operation, UCID groupUid, const std::string& fileName,
	const std::string& fileNameTo, int64_t fileSize, int8_t count, int8_t isDir, int8_t cloudType, const std::string& detailContent, bool isSave, OUT int32_t& msgId) {
	/*
	if (CLOUD_CHAT == type) {
		UC_LOG(ERROR) << "can not send cloud file to cloud group";
		return FORBID_SENDCLOUDFILE_TO_CLOUDGROUP;
	}
	*/
	MessageProcessor msgProcessor((UCEngine*)_engine);
	HTTPRequest	request;
	UCMsg *msg = NULL;
	BinMsg data;
	string body;

	//群聊或项目组上传文件
	if (type == PROJECT_CHAT || type ==  GROUP_CHAT) {
		count = 1;
		isDir = 0;
	}

	msgId = msgProcessor.CreateSendCloudFileMessage(type, operation, GetCurUser(), groupUid, GetCurUserName(), fileName, fileNameTo, fileSize,
		count, isDir, cloudType, detailContent, 0, 0, msg);

	if (msg == NULL) {
		UC_LOG(ERROR) << "send group cloud file message failed: " << msgId;
		return msgId;
	}

	if (msgProcessor.EncodeMessageData(*msg, data, body) != UC_SUCCEEDED) {
		UC_LOG(ERROR) << "encode group send cloud message failed.";
		return UC_FAILED;
	}

	if (isSave && msgProcessor.SaveMessage(type, groupUid.GetUserId(), 0, 0, STATE_SENDSUCCESS, msg, body) != UC_SUCCEEDED){
		UC_LOG(ERROR) << "save message in cache failed. relate_id: " << groupUid.GetUserId();
		delete msg;
		msg = NULL;
		return UC_FAILED;
	}

	// 后期可将BinMsg去掉，替换成string
	((UCEngine*)_engine)->SendQueuedRequest(type, (int64_t)groupUid.GetUserId(), msgId, std::string(data._msg, data._msgLen));

	delete msg;
	msg = NULL;

	return UC_SUCCEEDED;
}
int32_t GroupService::SendInviteCallMessage(ConversationType type, UCID groupUid, const std::string& confId,
	const std::string& confPwd, int32_t hostId, int32_t& msgId, UCID privateId) {
	int64_t msgTime = 0;
	HTTPRequest	request;
	MessageProcessor msgProcessor((UCEngine*)_engine);

	if (UC_SUCCEEDED == msgProcessor.CreateCallInvitationMessage(type, GetCurUser(), groupUid, confId, confPwd, hostId, request, msgId, msgTime, privateId)){
		UC_LOG(INFO) << "CreateCallInvitationMessage, type:" << type << ", cur user:" << GetCurUser().GetUserId()
			<< ", groupUid:" << groupUid.GetUserId() << ", confId:" << confId << ", confPwd:" << confPwd
			<< ", hostId:" << hostId << ", msgId:" << msgId << ", privateId:" << privateId.GetUserId();
		_engine->SendAsyncRequest(request,
			MakeRequestCallback(this, &GroupService::OnSendMessageResponse, type, (int64_t)groupUid.GetUserId(), msgId, false));
		return UC_SUCCEEDED;
	}
	return UC_FAILED;	
}

int32_t GroupService::SendInviteCallMessage(int32_t callType, ConversationType type, UCID groupUid, const std::string& confId,
	const std::string& confPwd, int32_t hostId, int32_t& msgId, UCID privateId, AudioService* service) {
	UC_LOG(INFO) << "SendInviteCallMessage, callType:" << callType << ", type:" << type << ", groupId:" << groupUid.GetUserId()
		<< ", confId:" << confId << ", confPwd:" << confPwd << ", hostId:" << hostId 
		<< ", msgId:" << msgId << ", privateId:" << privateId.GetUserId();
	int64_t msgTime = 0;
	HTTPRequest	request;
	MessageProcessor msgProcessor((UCEngine*)_engine);

	if (UC_SUCCEEDED == msgProcessor.CreateCallInvitationMessage(type, GetCurUser(), groupUid, confId, confPwd, hostId, request, msgId, msgTime, privateId)){
		UC_LOG(INFO) << "CreateCallInvitationMessage, type:" << type << ", cur user:" << GetCurUser().GetUserId()
			<< ", groupUid:" << groupUid.GetUserId() << ", confId:" << confId << ", confPwd:" << confPwd
			<< ", hostId:" << hostId << ", msgId:" << msgId << ", privateId:" << privateId.GetUserId();
		_engine->SendAsyncRequest(request,
			MakeRequestCallback(this, &GroupService::OnSendInviteCallMessageResponse, std::stoi(confId), callType, type, (int64_t)groupUid.GetUserId(), msgId, false, service));
		return UC_SUCCEEDED;
	}
	return UC_FAILED;
}

int32_t GroupService::SendAcceptCallMessage(ConversationType type, UCID groupUid, const std::string& confId, int32_t hostId, int32_t& msgId){
	int64_t msgTime = 0;
	HTTPRequest	request;
	MessageProcessor msgProcessor((UCEngine*)_engine);

	if (UC_SUCCEEDED == msgProcessor.CreateCallAgreeMentMessage(type, GetCurUser(), groupUid, confId, hostId, request, msgId, msgTime)){
		UC_LOG(INFO) << "CreateCallAgreeMentMessage, type:" << type << ", cur user:" << GetCurUser().GetUserId()
			<< ", groupUid:" << groupUid.GetUserId() << ", confId:" << confId
			<< ", hostId:" << hostId << ", msgId:" << msgId;
		_engine->SendAsyncRequest(request,
			MakeRequestCallback(this, &GroupService::OnSendMessageResponse, type, (int64_t)groupUid.GetUserId(), msgId, false));
		return UC_SUCCEEDED;
	}
	return UC_FAILED;
}

int32_t GroupService::SendRejectCallMessage(ConversationType type, UCID groupUid, const std::string& confId, 
	int32_t hostId, const std::string& reason, int32_t& msgId){
	int64_t msgTime = 0;
	HTTPRequest	request;
	MessageProcessor msgProcessor((UCEngine*)_engine);

	if (UC_SUCCEEDED == msgProcessor.CreateCallRejectMessage(type, GetCurUser(), groupUid, confId, hostId, reason, request, msgId, msgTime)){
		UC_LOG(INFO) << "CreateCallRejectMessage, type:" << type << ", cur user:" << GetCurUser().GetUserId()
			<< ", groupUid:" << groupUid.GetUserId() << ", confId:" << confId
			<< ", hostId:" << hostId << ", msgId:" << msgId;
		_engine->SendAsyncRequest(request,
			MakeRequestCallback(this, &GroupService::OnSendMessageResponse, type, (int64_t)groupUid.GetUserId(), msgId, false));
		return UC_SUCCEEDED;
	}
	return UC_FAILED;
}

int32_t GroupService::SendFinishCallMessage(ConversationType type, UCID groupUid, const std::string& confId, 
	int32_t hostId, const std::string& reason, int32_t& msgId){
	int64_t msgTime = 0;
	HTTPRequest	request;
	MessageProcessor msgProcessor((UCEngine*)_engine);

	if (UC_SUCCEEDED == msgProcessor.CreateCallFinishMessage(type, GetCurUser(), groupUid, confId, hostId, request, msgId, msgTime)){
		UC_LOG(INFO) << "CreateCallFinishMessage, type:" << type << ", cur user:" << GetCurUser().GetUserId()
			<< ", groupUid:" << groupUid.GetUserId() << ", confId:" << confId
			<< ", hostId:" << hostId << ", msgId:" << msgId;
		_engine->SendAsyncRequest(request,
			MakeRequestCallback(this, &GroupService::OnSendMessageResponse, type, (int64_t)groupUid.GetUserId(), msgId,false));
		return UC_SUCCEEDED;
	}
	return UC_FAILED;
}

int32_t GroupService::SendCancelCallMessage(ConversationType type, UCID groupUid, const string& phoneNum, int32_t& msgId){
	int64_t msgTime = 0;
	HTTPRequest	request;
	MessageProcessor msgProcessor((UCEngine*)_engine);

	if (UC_SUCCEEDED == msgProcessor.CreateCallCancelMessage(GROUP_CHAT, GetCurUser(), groupUid, phoneNum, request, msgId, msgTime)){
		UC_LOG(INFO) << "CreateCallCancelMessage, cur user:" << GetCurUser().GetUserId()
			<< ", groupUid:" << groupUid.GetUserId() << ", msgId:" << msgId;
		_engine->SendAsyncRequest(request,
			MakeRequestCallback(this, &GroupService::OnSendMessageResponse, type, (int64_t)groupUid.GetUserId(), msgId, false));
		return UC_SUCCEEDED;
	}
	return UC_FAILED;
}

// int32_t GroupService::SendNotRespondCallRecordMessage(ChatType chatType, UCID groupUid, int32_t& msgId){
// 
// 	HTTPRequest	request;
// 	MessageProcessor msgProcessor((UCEngine*)_engine);
// 
// 	if(msgProcessor.CreateNotRespondCallRecordMessage(chatType, GetCurUser(), groupUid, request, msgId)){
// 		_engine->SendAsyncRequest(&request,
// 			MakeRequestCallback(this, &GroupService::OnSendMessageResponse, groupUid.GetUserId(), msgId));
// 		return UC_SUCCEEDED;
// 	}
// 	return UC_FAILED;	
// }

// int32_t GroupService::SendRejectCallRecordMessage(ChatType chatType, UCID groupUid, int32_t& msgId){
// 
// 	HTTPRequest	request;
// 	MessageProcessor msgProcessor((UCEngine*)_engine);
// 
// 	if(msgProcessor.CreateRejectCallRecordMessage(chatType, GetCurUser(), groupUid, request, msgId)){
// 		_engine->SendAsyncRequest(request,
// 			MakeRequestCallback(this, &GroupService::OnSendMessageResponse, groupUid.GetUserId(), msgId));
// 		return UC_SUCCEEDED;
// 	}
// 	return UC_FAILED;
// }

int32_t GroupService::SendConnectedCallRecordMessage(ConversationType type, UCID groupUid, int32_t druation, const std::string& phoneNum, int32_t& msgId){

	int64_t msgTime = 0;
	HTTPRequest	request;
	MessageProcessor msgProcessor((UCEngine*)_engine);

	if (UC_SUCCEEDED == msgProcessor.CreateConnectedCallRecordMessage(type, GetCurUser(), groupUid, druation, phoneNum, request, msgId, msgTime)){
		UC_LOG(INFO) << "CreateConnectedCallRecordMessage, type:" << type << ", cur user:" << GetCurUser().GetUserId()
			<< ", groupUid:" << groupUid.GetUserId() << ", msgId:" << msgId;
		_engine->SendAsyncRequest(request,
			MakeRequestCallback(this, &GroupService::OnSendMessageResponse, type, (int64_t)groupUid.GetUserId(), msgId, false));
		return UC_SUCCEEDED;
	}
	return UC_FAILED;		
}

int32_t GroupService::SendMessageRead(ConversationType type, UCID groupUid, int32_t msgId, int64_t msgSeq){

	HTTPRequest	request;
	MessageProcessor msgProcessor((UCEngine*)_engine);

	if(UC_SUCCEEDED == msgProcessor.CreateMessageReadMessage(type, GetCurUser(), groupUid, msgId, msgSeq, request)) {
		_engine->SendAsyncRequest(request,
			MakeRequestCallback(this, &GroupService::OnSendMessageReadResponse, type, (int64_t)groupUid.GetUserId(), 0, msgId, msgSeq));
		return UC_SUCCEEDED;
	}
	return UC_FAILED;	
}

int32_t GroupService::SendMessagesRead(ConversationType type, int64_t groupId){

	HTTPRequest     request;
	stringstream	sStream;
	string         	targetURL;

	targetURL = ((UCEngine*)_engine)->GetRequestURL(((ClientService*)_engine->QueryService(UCClientService))->GetLoginInfo().GetUccServerDomain(), 
		MESSGAES_READ_METHOD);

	request.Init(targetURL, HTTP_METHOD_POST);
	request.AddHeaderInfo("Content-Type", "application/x-www-form-urlencoded");

	sStream  << "session_id=" << ((ClientService*)_engine->QueryService(UCClientService))->GetLoginInfo().GetSessionId() <<
		"&user_id=" << GetCurUser().GetUserId() << "&to_user_id=" << groupId << "&type=1";

	request.SetBody(sStream.str());
	_engine->SendAsyncRequest(request, MakeRequestCallback(this, &GroupService::OnSendMessageReadResponse, type, groupId, 1, 0, (int64_t)0));

	return UC_SUCCEEDED;
}

int32_t GroupService::SendMessagePlayed(ConversationType type, UCID groupUid, int32_t msgId, int64_t msgSeq){

	
	HTTPRequest	request;
	MessageProcessor msgProcessor((UCEngine*)_engine);

	if(UC_SUCCEEDED == msgProcessor.CreateMessagePlayedMessage(type, GetCurUser(), groupUid, msgId, msgSeq, request)) {
		_engine->SendAsyncRequest(request,
			MakeRequestCallback(this, &GroupService::OnSendMessagePlayedResponse, (int64_t)groupUid.GetUserId(), msgId, msgSeq));
		return UC_SUCCEEDED;
	}
	
	return UC_FAILED;
}

int32_t GroupService::GetUnreadUserCount(ConversationType type, int64_t groupId, std::list<int64_t> seq){

	std::list<MessageReadInfo> infolist;
	theStore.GetMessageReadStore()->GetMessageReadList(ConvKey(type, groupId), seq, infolist);
	if (!infolist.empty()){
		triggerEvent(IGroupServiceEvent, OnMessageUnreadUserCount(UC_SUCCEEDED, type, groupId, infolist));
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
		"&user_id=" << GetCurUser().GetUserId() << "&to_user_id=" << groupId << "&type=" << 1 <<
		"&data=" << StringConvert::URLEncode(StringConvert::ToUTF8Str(sData.str()));

	request.SetBody(sStream.str());
	_engine->SendAsyncRequest(request, MakeRequestCallback(this, &GroupService::OnGetUnreadUserCountResponse, type, groupId, seq));

	return UC_SUCCEEDED;
}


int32_t GroupService::GetUnreadUserList(ConversationType type, int64_t groupId, std::list<int64_t> seq){

	std::list<MessageReadUserInfo> infolist;
	theStore.GetMessageReadStore()->GetMessageReadList(ConvKey(type, groupId), seq, infolist);
	if (!infolist.empty()){
		triggerEvent(IGroupServiceEvent, OnMessageUnreadUserList(UC_SUCCEEDED, type, groupId, infolist));
	}

	if (seq.empty()){
		return UC_SUCCEEDED;
	}

	HTTPRequest     request;
	stringstream	sStream;
	string         	targetURL;

	targetURL = ((UCEngine*)_engine)->GetRequestURL(((ClientService*)_engine->QueryService(UCClientService))->GetLoginInfo().GetUccServerDomain(),
		MESSAGE_UNREADUSER_LIST_METHOD);

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
		"&user_id=" << GetCurUser().GetUserId() << "&to_user_id=" << groupId << "&type=" << 1 << 
		"&data=" << StringConvert::URLEncode(StringConvert::ToUTF8Str(sData.str()));

	request.SetBody(sStream.str());
	_engine->SendAsyncRequest(request, MakeRequestCallback(this, &GroupService::OnGetUnreadUserListResponse, type, groupId, seq));

	return UC_SUCCEEDED;
}

void GroupService::OnBatchUploadDocumentFired() {
	boost::unique_lock<boost::recursive_mutex> lock(_batchUploadOptsMutex);
	if (_batchUploadOpts.empty()) {
		Timer::GetInstance().Cancel(_batchUploadTimerId);
		_batchUploadTimerId = -1;

		return;
	}
	
	for (std::map<int64_t, std::list<DocumentUploadOperation>>::iterator iter = _batchUploadOpts.begin();
		iter != _batchUploadOpts.end(); ++iter) {
		
		//boost::property_tree::wptree jsonData;
		wstringstream sData;
		sData << L"[";
		std::list<DocumentUploadOperation>& optList = iter->second;
		for (std::list<DocumentUploadOperation>::iterator iterList = optList.begin();
			iterList != optList.end(); ++iterList) {
			if (iterList != optList.begin()) {
				sData << L", ";
			}

			boost::property_tree::wptree element;
			wstringstream elemStream;
			ConvertDocumentToJson(iter->first, iterList->operation, iterList->info, true, element);
			boost::property_tree::json_parser::write_json(elemStream, element);

			sData << elemStream.str();
			
		}
		sData << L"]";
		
		BatchUpoadDocument(iter->first, StringConvert::ToUTF8Str(sData.str()));
	}

	_batchUploadOpts.clear();
}

void GroupService::BatchUpoadDocument(int64_t groupId, const std::string& data) {
	HTTPRequest     request;
	stringstream	sStream;
	string         	targetURL;
	const LoginInfo &loginInfo = ((ClientService*)_engine->QueryService(UCClientService))->GetLoginInfo();

	targetURL = ((UCEngine*)_engine)->GetRequestURL(loginInfo.GetUccServerDomain(), GROUP_DOC_BATCHSAVE_METHOD);

	request.Init(targetURL, HTTP_METHOD_POST);
	request.AddHeaderInfo("Content-Type", "application/x-www-form-urlencoded");

	sStream << "session_id=" << loginInfo.GetSessionId() <<
		"&user_id=" << GetCurUser().GetUserId() << "&group_id=" << groupId
		<< "&data=" << StringConvert::URLEncode(data);

	request.SetBody(sStream.str());

	_engine->SendAsyncRequest(request, MakeRequestCallback(this, &GroupService::OnBatchUploadDocumentResponse, groupId));
}

void GroupService::ConvertDocumentToJson(int64_t groupId, int32_t operation, const DocumentInfo& info, bool isBatch,
	boost::property_tree::wptree& tree) {

	if (isBatch) {
		tree.put(L"operation", operation);
		tree.put(L"content_type", 0);
	}
	
	tree.put(L"group_id", groupId);
	tree.put(L"user_id", GetCurUser().GetUserId());
	tree.put(L"user_name", StringConvert::FromUTF8Str(GetCurUserName()));
	tree.put(L"file_url", StringConvert::FromUTF8Str(info.GetFileUrl()));
	tree.put(L"title", StringConvert::FromUTF8Str(info.GetFileName()));
	tree.put(L"format", StringConvert::FromUTF8Str(GetFileFormat(info.GetFileName())));
	tree.put(L"size", info.GetFileSize());
	tree.put(L"cloud_type", info.GetCloudType());
	tree.put(L"preview", StringConvert::FromUTF8Str(info.GetPreview()));

	if (info.GetCloudType() == CT_GoKuai) {
		tree.put(L"hash_code", StringConvert::FromUTF8Str(info.GetHashCode()));
		tree.put(L"mount_id", info.GetMountId());
		tree.put(L"detail_type", info.GetDetailType());
		tree.put(L"detail_content", StringConvert::FromUTF8Str(info.GetDetailContent()));
	}
}


int32_t GroupService::GetAtMessageList(int64_t groupId, int32_t page /*= 0*/, int32_t count /*= 100*/) {

	UC_LOG(INFO) << "GetAtMessageList enter, groupId = " << groupId;

	//从缓存中获取
	vector<AtMsgInfo> infos;
	if (GetAtMsgFromCache(groupId, infos)) {
		map<int64_t, int64_t> atMsgs;
		for (AtMsgInfo& info : infos) {
			atMsgs[info._seq] = info._sender;
		}
	
		UC_LOG(INFO) << "GetAtMessageList from cache success.";
		triggerEvent(IGroupServiceEvent, OnAtMessageListReceived(0, groupId, atMsgs));
		return UC_SUCCEEDED;
	}

	HTTPRequest     request;
	stringstream	sStream;
	string         	targetURL;
	page = page < 0 ? 0 : page;
	count = count < 500 ? 500 : count;
	targetURL = ((UCEngine*)_engine)->GetRequestURL(((ClientService*)_engine->QueryService(UCClientService))->GetLoginInfo().GetUccServerDomain(),
		MESSAGE_ATMESSAGE_LIST_METHOD);

	sStream << targetURL << "?session_id=" << ((ClientService*)_engine->QueryService(UCClientService))->GetLoginInfo().GetSessionId() <<
		"&user_id=" << GetCurUser().GetUserId() << "&to_user_id=" << groupId << "&type=1" << "&page=" << page << "&count=" << count;

	request.Init(sStream.str(), HTTP_METHOD_GET);
	_engine->SendAsyncRequest(request, MakeRequestCallback(this, &GroupService::OnGetAtMessageListResponse, groupId));

	return UC_SUCCEEDED;
}

int32_t GroupService::DeleteAtMessage(ConversationType type, int64_t groupId, const std::vector<int64_t>& seqList) {
	UC_LOG(INFO) << "DeleteAtMessage enter, ConversationType= "<< type << ", groupId = " << groupId;
	HTTPRequest     request;
	stringstream	sStream;
	string         	targetURL;

	targetURL = ((UCEngine*)_engine)->GetRequestURL(((ClientService*)_engine->QueryService(UCClientService))->GetLoginInfo().GetUccServerDomain(),
		MESSAGE_ATMESSAGE_DEL_METHOD);

	request.Init(targetURL, HTTP_METHOD_POST);
	request.AddHeaderInfo("Content-Type", "application/x-www-form-urlencoded");

	stringstream sData;
	sData << "[";
	for (std::vector<int64_t>::const_iterator iter = seqList.begin(); iter != seqList.end(); ++iter){
		if (iter != seqList.begin()){
			sData << ",";
		}
		sData << *iter;
	}
	sData << "]";

	sStream << "session_id=" << ((ClientService*)_engine->QueryService(UCClientService))->GetLoginInfo().GetSessionId() <<
		"&user_id=" << GetCurUser().GetUserId() << "&to_user_id=" << groupId << "&type=1" << "&data=" 
		<<sData.str();

	request.SetBody(sStream.str());
	_engine->SendAsyncRequest(request, MakeRequestCallback(this, &GroupService::OnDeleteAtMessageResponse, type, groupId, seqList));

	return UC_SUCCEEDED;
}

} /* namespace uc */
