#include "MicroblogService.h" 
#define BOOST_SPIRIT_THREADSAFE
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include "CommonDef.h"
#include "UCEngine.h"
#include "client_service/ClientService.h"
#include "HTTPResponseAssist.h"
#include "UCErrorCode.h"
#include <sstream>
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
using namespace rapidjson;

namespace uc {

MicroblogService::MicroblogService(IUCEngine *engine) {
	_engine = engine;
	_engine->registerListener(this);
}

MicroblogService::~MicroblogService() {
	_engine->removeListener(this);
}

std::string MicroblogService::GetHomePageURL(int32_t userId) {
	std::string			url = "";
	ClientService		*clientService = (ClientService*)_engine->QueryService(UCClientService);
	std::stringstream	sStream;
	const LoginInfo		&loginInfo = clientService->GetLoginInfo();

	if (loginInfo.GetClientType() == UCCLIENT_WIN) {
		url = UCEngine::GetRequestURL(clientService->GetLoginInfo().GetUccServerDomain(), MICROBLOG_HOMEPAGE_PC_METHOD);
		sStream << url << "?session_id=" << loginInfo.GetSessionId() << "&user_id=" << userId;
	}
	else {
		url = UCEngine::GetRequestURL(clientService->GetLoginInfo().GetUccServerDomain(), MICROBLOG_HOMEPAGE_METHOD);
		
		boost::property_tree::wptree ptRoot;
		wstringstream wsStream;
		wstring json = L"";

		ptRoot.put(L"url", L"personal.user.htm");
		ptRoot.put(L"act", L"personal");
		ptRoot.put(L"title", L"homepage");
		ptRoot.put(L"personal_user_id", userId);

		boost::property_tree::write_json(wsStream, ptRoot);
		json = wsStream.str();

		sStream << url << StringConvert::ToUTF8Str(json);
	}

	return sStream.str();
}

std::string MicroblogService::GetUserHomePageURL(int32_t selfId, int32_t userId)
{
	std::string			url = "";
	ClientService		*clientService = (ClientService*)_engine->QueryService(UCClientService);
	std::stringstream	sStream;
	const LoginInfo		&loginInfo = clientService->GetLoginInfo();

	if (loginInfo.GetClientType() == UCCLIENT_WIN) {
		url = UCEngine::GetRequestURL(clientService->GetLoginInfo().GetUccServerDomain(), MICROBLOG_HOMEPAGE_PC_METHOD);
		sStream << url << "?session_id=" << loginInfo.GetSessionId() << "&user_id=" << selfId << "&return=u/home?id=" << userId;
	}
	else {
		// 其他版本的暂未实现
	}

	return sStream.str();
}

void MicroblogService::GetNewMsgCount() {

	HTTPRequest			request;
	std::string			url = "";
	ClientService		*clientService = (ClientService*)_engine->QueryService(UCClientService);
	std::stringstream	sStream;
	const LoginInfo		&loginInfo = clientService->GetLoginInfo();

	url = UCEngine::GetRequestURL(clientService->GetLoginInfo().GetUccServerDomain(), MICROBLOG_NEWMSGCOUNT_METHOD);

	sStream << url << "?session_id=" << loginInfo.GetSessionId() << "&user_id=" << loginInfo.GetUserId()
		<< "&customer_code=" << loginInfo.GetCustomerCode();

	request.Init(sStream.str(), HTTP_METHOD_GET);
	_engine->SendAsyncRequest(request, MakeRequestCallback(this, &MicroblogService::OnGetNewMsgCountComplete));
}

void MicroblogService::AddAttention(int32_t userId) {
	HTTPRequest		request;
	string			url;
	string 			bodyData;
	stringstream	sStream;
	ClientService	*clientService = (ClientService*)_engine->QueryService(UCClientService);
	const LoginInfo	&loginInfo = clientService->GetLoginInfo();

	url = UCEngine::GetRequestURL(loginInfo.GetUccServerDomain(), MICROBLOG_ADDATTENTION_METHOD);
	request.Init(url, HTTP_METHOD_POST);

	sStream << "session_id=" << loginInfo.GetSessionId() << "&customer_code=" << loginInfo.GetCustomerCode()
		<< "&user_id=" << loginInfo.GetUserId() << "&oid=" << userId;

	request.AddHeaderInfo("Content-Type", "application/x-www-form-urlencoded; charset=UTF-8");
	request.SetBody(sStream.str());

	_engine->SendAsyncRequest(request, MakeRequestCallback(this, &MicroblogService::OnAddAttentionComplete, userId));
}

void MicroblogService::CancelAttention(int32_t userId) {
	HTTPRequest		request;
	string			url;
	string 			bodyData;
	stringstream	sStream;
	ClientService	*clientService = (ClientService*)_engine->QueryService(UCClientService);
	const LoginInfo	&loginInfo = clientService->GetLoginInfo();

	url = UCEngine::GetRequestURL(loginInfo.GetUccServerDomain(), MICROBLOG_CANCELATTENTION_METHOD);
	request.Init(url, HTTP_METHOD_POST);

	sStream << "session_id=" << loginInfo.GetSessionId() << "&customer_code=" << loginInfo.GetCustomerCode()
		<< "&user_id=" << loginInfo.GetUserId() << "&oid=" << userId;

	request.AddHeaderInfo("Content-Type", "application/x-www-form-urlencoded; charset=UTF-8");
	request.SetBody(sStream.str());

	_engine->SendAsyncRequest(request, MakeRequestCallback(this, &MicroblogService::OnCancelAttentionComplete, userId));
}


void MicroblogService::ShareMeetingRecording(std::string urlVideo, std::string urlThumb, std::string fileName, int64_t fileSize, int64_t voiceTime, std::string comments)
{
	ClientService *clientService = (ClientService*)_engine->QueryService(UCClientService);
	const LoginInfo	&loginInfo = clientService->GetLoginInfo();
	int32_t selfID = loginInfo.GetUserId();
	int32_t scopeType = 0;
	int32_t attatchType = 5;
	int32_t hardwaretype = 0;
	HTTPRequest request;

	std::string encodeUrlVideo = StringConvert::URLEncode(urlVideo);
	std::string encodeUrlThumb = StringConvert::URLEncode(urlThumb);

	Document doc;
	//user_id
	doc.SetObject();
	Document::AllocatorType& allocator = doc.GetAllocator();
	Value vls(kStringType);
	//vls.SetString(selfID.c_str(), selfID.length());
	doc.AddMember("user_id", selfID, allocator);

	//session_id	
// 	std::string sSessionId = loginInfo.GetSessionId();
// 	vls.SetString(sSessionId.c_str(), sSessionId.length());
// 	doc.AddMember("session_id", vls, allocator);

	//timenews_content
	vls.SetString(comments.c_str(), comments.length());
	doc.AddMember("timenews_content", vls, allocator);
	
	//sharescope: { "type":4,"scope_id":[3741,3744] })
	Value vlObjScope;
	vlObjScope.SetObject();
	stringstream ss;
	ss << scopeType;
	string sScopeType = ss.str();
	vls.SetString(sScopeType.c_str(), sScopeType.length());
	vlObjScope.AddMember("type", vls, allocator);
	
	Value vlArrScopeIds;
	vlArrScopeIds.SetArray();
	vlArrScopeIds.Clear();
	vlObjScope.AddMember("scope_id", vlArrScopeIds, allocator);
	doc.AddMember("sharescope", vlObjScope, allocator);
	
	
	//contact_ids
	Value vlArryContractIds;
	vlArryContractIds.SetArray();
	vlArryContractIds.Clear();
	doc.AddMember("contact_ids", vlArryContractIds, allocator);
	
	//"attachments":[{"type":5, "url" : "http:\/\/oncloud2.quanshi.com\/microblog\/common\/getm3u8\/1334916\/70804", "endMark" : "", "fileName" : "PC客户端新视觉", "fileSize" : "", "voiceTime" : "45678", "thumburl" : "http:\/\/oncloud.quanshi.com\/\/ucfserver\/\/hddown?fid=MTM1OTEwNC84LzU1NDk5ZWVjMmQ4YzQyYWJiNDhlOTIzMzk0YzcyYmY0LnBuZ15eXnRhbmdoZGZzXl5eMGM3MzNkNDBkZWMzOTc2YTllM2I0Y2Y3YzVlM2NjMjheXl50YW5naGRmc15eXjE5Mjk3$&u=1359104"}],
	Value vlArryAttachs;
	vlArryAttachs.SetArray();
	Value vlObj;
	vlObj.SetObject();

	vlObj.AddMember("type", attatchType, allocator);
	vls.SetString(encodeUrlVideo.c_str(), encodeUrlVideo.length());
	vlObj.AddMember("url", vls, allocator);
	vls.SetString(fileName.c_str(), fileName.length());
	vlObj.AddMember("fileName", vls, allocator);
	
	ss.str("");
	ss.clear();
	ss << fileSize;
	string sFileSize = ss.str();
	vls.SetString(sFileSize.c_str(), sFileSize.length());
	vlObj.AddMember("fileSize", vls, allocator);

	ss.str("");
	ss.clear();
	ss << voiceTime;
	string sVoiceTime = ss.str();
	vls.SetString(sVoiceTime.c_str(), sVoiceTime.length());
	vlObj.AddMember("voiceTime", vls, allocator);

	vls.SetString(encodeUrlThumb.c_str(), encodeUrlThumb.length());
	vlObj.AddMember("thumburl", vls, allocator);
	vlArryAttachs.PushBack(vlObj, allocator);
	doc.AddMember("attachments", vlArryAttachs, allocator);

	//hardwaretype
	doc.AddMember("hardwaretype", hardwaretype, allocator);
	

	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);
	doc.Accept(writer);
	std::string sbody = "data=";
	std::string buf = buffer.GetString();
	sbody += buf;

	/////////////////
	std::string flag = ":443";
	string doMain = loginInfo.GetUccServerDomain();
	doMain.replace(doMain.find(flag), flag.size(),  "");
	string url = UCEngine::GetRequestURL(doMain, MICROBLOG_SHARE_METHOD);
	stringstream urlstream;
	urlstream << url << "?session_id=" << loginInfo.GetSessionId() << "&user_id=" << selfID;
	request.Init(urlstream.str(), HTTP_METHOD_POST);	
	request.SetBody(sbody);

	_engine->SendAsyncRequest(request, MakeRequestCallback(this, &MicroblogService::OnMeetingRecordingShared));

}

int32_t MicroblogService::OnAddAttentionComplete(int32_t errorCode, HTTPResponse *response, int32_t userId) {
	int32_t result = HTTPResponseAssist::GuardStatementForCallBack("OnGetNewMsgCountComplete", errorCode, response);

	if (result != UC_SUCCEEDED) {
		triggerEvent(IMicroblogServiceEvent, OnAttentionAdd(result, userId, 0));
		return -1;
	}

	int32_t code = JSON_PARSE_ERROR;

	wstringstream jsonStream;
	boost::property_tree::wptree ptRoot;
	jsonStream.str(StringConvert::FromUTF8Str(response->GetContent()).c_str());
	try{
		boost::property_tree::json_parser::read_json(jsonStream, ptRoot);
		code = ptRoot.get<int32_t>(L"code");
	}
	catch (boost::property_tree::ptree_error & e)
	{
		UC_LOG(ERROR) << "encounter an exception when parse json in logout process, exception: " << e.what();
	}

	
	if (code != 0) {
		triggerEvent(IMicroblogServiceEvent, OnAttentionAdd(code, userId, 0));
		return code;
	}

	int32_t relation = 0;

	boost::property_tree::wptree ptData;
	try {
		ptData = ptRoot.get_child(L"data");
		relation = ptData.get<int32_t>(L"relation", 0);
	}
	catch (boost::property_tree::ptree_error & e)
	{
		UC_LOG(ERROR) << "encounter an exception when parse json in logout process, exception: " << e.what();
		triggerEvent(IMicroblogServiceEvent, OnAttentionAdd(JSON_PARSE_ERROR, userId, relation));
		return JSON_PARSE_ERROR;
	}

	_mapRelation[userId] = relation;
	triggerEvent(IMicroblogServiceEvent, OnAttentionAdd(code, userId, relation));
	return code;
}

int32_t MicroblogService::OnCancelAttentionComplete(int32_t errorCode, HTTPResponse *response, int32_t userId) {
	int32_t result = HTTPResponseAssist::GuardStatementForCallBack("OnGetNewMsgCountComplete", errorCode, response);
	if (result != UC_SUCCEEDED) {
		triggerEvent(IMicroblogServiceEvent, OnAttentionCancelled(result, userId, 0));
		return -1;
	}

	int32_t code = JSON_PARSE_ERROR;

	wstringstream jsonStream;
	boost::property_tree::wptree ptRoot;
	jsonStream.str(StringConvert::FromUTF8Str(response->GetContent()).c_str());
	try{
		boost::property_tree::json_parser::read_json(jsonStream, ptRoot);
		code = ptRoot.get<int32_t>(L"code");
	}
	catch (boost::property_tree::ptree_error & e)
	{
		UC_LOG(ERROR) << "encounter an exception when parse json in logout process, exception: " << e.what();
	}
	
	if (code != 0) {
		triggerEvent(IMicroblogServiceEvent, OnAttentionCancelled(code, userId, 0));
		return code;
	}

	int32_t relation = 0;

	boost::property_tree::wptree ptData;
	try{
		ptData = ptRoot.get_child(L"data");
		relation = ptData.get<int32_t>(L"relation", 0);
	}
	catch (boost::property_tree::ptree_error & e)
	{
		UC_LOG(ERROR) << "encounter an exception when parse json in logout process, exception: " << e.what();
		triggerEvent(IMicroblogServiceEvent, OnAttentionCancelled(JSON_PARSE_ERROR, userId, 0));
		return JSON_PARSE_ERROR;
	}

	_mapRelation[userId] = relation;

	triggerEvent(IMicroblogServiceEvent, OnAttentionCancelled(code, userId, relation));
	return code;
}

int32_t MicroblogService::OnGetNewMsgCountComplete(int32_t errorCode, HTTPResponse *response) {
	int32_t result = HTTPResponseAssist::GuardStatementForCallBack("OnGetNewMsgCountComplete", errorCode, response);

	if (result != UC_SUCCEEDED) {
		triggerEvent(IMicroblogServiceEvent, OnNewMsgCountGet(result, 0, 0, 0, 0, 0));
		return -1;
	}

	int32_t code = UC_FAILED;

	wstringstream jsonStream;
	boost::property_tree::wptree ptRoot;
	jsonStream.str(StringConvert::FromUTF8Str(response->GetContent()).c_str());
	try{
		boost::property_tree::json_parser::read_json(jsonStream, ptRoot);
		code = ptRoot.get<int32_t>(L"code");
	}
	catch (boost::property_tree::ptree_error & e)
	{
		UC_LOG(ERROR) << "encounter an exception when parse json in logout process, exception: " << e.what();
	}
	
	if (code != 0) {
		triggerEvent(IMicroblogServiceEvent, OnNewMsgCountGet(code, 0, 0, 0, 0, 0));
		return code;
	}

	int32_t countAll = 0;
	int32_t relationCount = 0;
	int32_t timenewsCount = 0;
	int32_t pollCount = 0;
	int32_t caseCount = 0;

	boost::property_tree::wptree ptData;
	try{
		ptData = ptRoot.get_child(L"data");

		countAll = ptData.get<int32_t>(L"count_all", 0);
		relationCount = ptData.get<int32_t>(L"relation", 0);
		timenewsCount = ptData.get<int32_t>(L"timenews", 0);
		pollCount = ptData.get<int32_t>(L"poll", 0);
		caseCount = ptData.get<int32_t>(L"case", 0);
	}
	catch (boost::property_tree::ptree_error & e)
	{
		UC_LOG(ERROR) << "encounter an exception when parse json in logout process, exception: " << e.what();
		code = JSON_PARSE_ERROR;
	}
	triggerEvent(IMicroblogServiceEvent, OnNewMsgCountGet(code, countAll, relationCount, timenewsCount, pollCount, caseCount));
	return code;
}

void MicroblogService::GetAttentionList(int32_t index, int32_t offset) {
	HTTPRequest			request;
	std::string			url = "";
	ClientService		*clientService = (ClientService*)_engine->QueryService(UCClientService);
	std::stringstream	sStream;
	const LoginInfo		&loginInfo = clientService->GetLoginInfo();
	int64_t				timestamp = 0;

	url = UCEngine::GetRequestURL(clientService->GetLoginInfo().GetUccServerDomain(), MICROBLOG_GETATTENTIONLIST_METHOD);

	sStream << url << loginInfo.GetUserId() << "/" << index << "/" << offset << "/" << timestamp
		<< "?session_id=" << loginInfo.GetSessionId() << "&user_id=" << loginInfo.GetUserId()
		<< "&customer_code=" << loginInfo.GetCustomerCode();

	request.Init(sStream.str(), HTTP_METHOD_GET);
	_engine->SendAsyncRequest(request, MakeRequestCallback(this, &MicroblogService::OnGetAttentionListComplete, index, offset));
}

int32_t MicroblogService::OnGetAttentionListComplete(int32_t errorCode, HTTPResponse *response, int32_t index, int32_t offset) {
	int32_t result = HTTPResponseAssist::GuardStatementForCallBack("OnGetNewMsgCountComplete", errorCode, response);

	if (result != UC_SUCCEEDED) {
		return -1;
	}

	int32_t code = UC_FAILED;

	wstringstream jsonStream;
	boost::property_tree::wptree ptRoot;
	jsonStream.str(StringConvert::FromUTF8Str(response->GetContent()).c_str());
	try{
		boost::property_tree::json_parser::read_json(jsonStream, ptRoot);
		code = ptRoot.get<int32_t>(L"code");
	}
	catch (boost::property_tree::ptree_error & e)
	{
		UC_LOG(ERROR) << "encounter an exception when parse json in logout process, exception: " << e.what();
	}
	
	if (code != 0) {
		return code;
	}

	int32_t attentionNum = 0;

	boost::property_tree::wptree ptData;
	boost::property_tree::wptree ptList;
	boost::property_tree::wptree ptAttention;

	try{
		ptData = ptRoot.get_child(L"data");

		attentionNum = ptData.get<int32_t>(L"attentionNum", 0);
		ptList = ptData.get_child(L"attentionList");
	}
	catch (boost::property_tree::ptree_error & e)
	{
		UC_LOG(ERROR) << "encounter an exception when parse json in logout process, exception: " << e.what();
		return JSON_PARSE_ERROR;
	}

	int32_t userId = 0;
	int32_t relation = 0;
	int32_t total = 0;

	BOOST_FOREACH(boost::property_tree::wptree::value_type &v, ptList) {
		ptAttention = v.second;
		try{
			userId = ptAttention.get<int32_t>(L"user_id", 0);
			relation = ptAttention.get<int32_t>(L"person_relation", 0);
		}
		catch (boost::property_tree::ptree_error & e)
		{
			UC_LOG(ERROR) << "encounter an exception when parse json in logout process, exception: " << e.what();
			return JSON_PARSE_ERROR;
		}

		_mapRelation.insert(pair<int32_t, int32_t>(userId, relation));
		total++;
	}

	if (total == offset) {
		index++;
		GetAttentionList(index, offset);
	}

	return code;
}

int32_t MicroblogService::GetRelation(int32_t userId) {
	int32_t relation = 4;

	std::map<int32_t, int32_t>::const_iterator itor = _mapRelation.find(userId);
	if (itor != _mapRelation.end()) {
		relation = itor->second;
	}

	return relation;
}

// 登陆成功时从server端获取关注列表
void MicroblogService::OnLoginSucceeded() {
	_mapRelation.clear();

	// 每次取100
	GetAttentionList(0, 100);
}

// 清空登陆列表
void MicroblogService::OnLogoutSucceeded() {
	_mapRelation.clear();
}

int32_t MicroblogService::OnMeetingRecordingShared(int32_t errorCode, HTTPResponse *response)
{
	int32_t result = HTTPResponseAssist::GuardStatementForCallBack("OnMeetingRecordingShared", errorCode, response);

	if (result != UC_SUCCEEDED) {
		triggerEvent(IMicroblogServiceEvent, OnMeetingRecordingShared(result, 0));
		return -1;
	}

	//解析json
	int32_t code = -1;
	int64_t resourceId = 0;
	try {
		wstringstream jsonStream;
		boost::property_tree::wptree ptRoot;
		jsonStream.str(StringConvert::FromUTF8Str(response->GetContent()).c_str());
		boost::property_tree::json_parser::read_json(jsonStream, ptRoot);
		code = ptRoot.get<int32_t>(L"code", code);

		if (code != 0) {
			//server返回失败
			triggerEvent(IMicroblogServiceEvent, OnMeetingRecordingShared(code, resourceId));
			return code;
		}
		boost::property_tree::wptree ptData;
		boost::property_tree::wptree ptDef;
		ptData = ptRoot.get_child(L"data", ptDef);
		if (ptData.empty()) {
			//数据为空
			UC_LOG(ERROR) << "OnConferenceCreated error, server response data is null.";
			triggerEvent(IMicroblogServiceEvent, OnMeetingRecordingShared(code, resourceId));
			return code;
		}

		resourceId = ptData.get<int64_t>(L"resourceID", 0);
	}
	catch (boost::property_tree::ptree_error & e) {
		UC_LOG(ERROR) << "encounter an exception when parse json from OnAccreditUserListReceived , exception: " << e.what();
		triggerEvent(IMicroblogServiceEvent, OnMeetingRecordingShared(JSON_PARSE_ERROR, resourceId));
		return JSON_PARSE_ERROR;
	}

	triggerEvent(IMicroblogServiceEvent, OnMeetingRecordingShared(code, resourceId));
	return UC_SUCCEEDED;
}

}
