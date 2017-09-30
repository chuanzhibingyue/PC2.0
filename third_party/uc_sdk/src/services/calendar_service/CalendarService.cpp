/*
 * CalendarService.cpp
 *
 *  Created on: Feb 9, 2015
 *      Author: ethan
 */

#define BOOST_SPIRIT_THREADSAFE

#include "CalendarService.h"
#include <sstream>
#include "client_service/ClientService.h"
#include "message_service/GroupService.h"
#include "contact_service/ContactService.h"
#include "CalendarRequestFactory.h"
#include "CalendarMsgProcessor.h"
#include "UCEngine.h"
#include "network/NetworkManager.h"
#include "HTTPRequestCallback.h"
#define BOOST_SPIRIT_THREADSAFE
#include "boost/property_tree/ptree.hpp"
#include "boost/property_tree/json_parser.hpp"
#include "common/StringConvert.h"
#include "LogManager.h"
#include "db/include/DBManager.h"
#include "boost/foreach.hpp"
#include "Macro.h"
#include "UCErrorCode.h"
#include "DBManager.h"
#include "CommonDef.h"
#include "message_service/MessageProcessor.h"
#include "ICalendarParser/icalendar.h"
#include "base/Timer.h"
#include "rapidjson/document.h"
#include "jsonhelper.h"
#include "HTTPResponseAssist.h"

#define KEY_CODE				L"code"
#define KEY_DATA				L"data"
#define KEY_MSG					L"msg"
#define KEY_CONFERENCEID		L"conferenceid"
#define KEY_EVENTID				L"eventid"
#define KEY_SUMMARYID			L"summaryId"
#define KEY_HOSTPASSWORD		L"pcode1"
#define KEY_ATTENDEEPASSWORD	L"pcode2"
#define KEY_BILLINGCODE			L"billingcode"
#define KEY_OWNER				L"owner"
#define KEY_OTHER				L"other"
#define KEY_ICALENDAR			L"icalendar"
#define KEY_PAGECOUNT			L"pagecount"
#define KEY_STATUS				L"status"
#define KEY_REQUEST_ID			L"request_id"
#define KEY_ID					L"id"
#define KEY_CREATOR				L"creator"
#define KEY_GROUPID				L"groupId"
#define KEY_STARTTIME			L"confStartTime"
#define KEY_CREATETIME			L"createTime"
#define KEY_UPDATETIME			L"updateTime"
#define KEY_LOCATION			L"confLocation"
#define KEY_SUMMARY				L"confSummary"
#define KEY_SUMMARYTITLE		L"confTitle"
#define KEY_SUMMARYTYPE			L"type"
#define KEY_EXTRASTINFO			L"extrasInfo"
#define KEY_VIDEONAME			L"videoName"
#define KEY_VIDEOURL			L"videoURL"
#define KEY_VIDEOSTREAMURL		L"videoStreamUrl"
#define KEY_VIDEOTHUMB			L"thumb"
#define KEY_VIDEOTHUMBNAIL		L"thumbnail"
#define KEY_OPERATORID			L"operatorId"
#define KEY_OPERATORNAME		L"operatorName"
#define KEY_OPERATORAVATAR		L"operatorAvatar"
#define KEY_RECORDSTARTTIME		L"recordingStarttime"
#define KEY_RECORDENDTIME		L"recordingEndtime"
#define KEY_VIDEOLENGTH			L"videoLength"
#define KEY_VIDEOSIZE			L"videoSize"

#define CACH_NUMMBER			24
#define TIMER_MEETING_ALARM_DELAY 10000		//会议提醒定时器延迟启动时间	
#define TIMER_MEETING_ALARM_ELAPSE 30000	//会议提醒定时器周期
#define DEF_ALARM_TIME	180					//会前提醒时间
namespace uc {

CalendarService::CalendarService(IUCEngine *engine) {
	_engine = engine;
	_engine->registerListener(this);
	_calendarRequestFactory = new CalendarRequestFactory(this);
	_calendarMsgProcessor = new CalendarMsgProcessor(this);
	_alarmId = -1;
	_alarmTime = DEF_ALARM_TIME;
	_bAlreadyCleanup = false;
}

CalendarService::~CalendarService() {
	_engine->removeListener(this);

	if (_calendarRequestFactory != NULL) {
		delete _calendarRequestFactory;
		_calendarRequestFactory = NULL;
	}

	if (_calendarMsgProcessor != NULL) {
		delete _calendarMsgProcessor;
		_calendarMsgProcessor = NULL;
	}

	_cacheMeetings.clear();
}

//-----------------------------------interface----------------------------------------------//

int32_t CalendarService::CreateMeeting(const std::vector<Attendee>& attendees, const std::string& title, const std::string& location,
	const std::string& summary, const std::string& timeZone, const std::string& hostName, const std::string& extrasInfo, int64_t startTime, int32_t duration,
	int32_t authorizerId, CycleRole& cycleRole, int32_t isGnet /*= 1*/, int32_t isCycle /*= 0*/, int32_t isWholeDay /*= 0*/,
	int32_t language /*= 1*/, int32_t emailLanguage /*= 1 */) {
	UC_LOG(INFO) << "CreateMeeting enter. startTime = " << startTime << ", authorizerId = " << authorizerId << ", isCycle = " << isCycle;
	
	if (authorizerId < 0 || authorizerId == GetUserId()) {
		authorizerId = 0;
	}

	Meeting* meeting = new Meeting;
	meeting->SetAttendees(attendees);
	meeting->SetTitle(StringConvert::URLEncode(title));
	meeting->SetExtrasInfo(StringConvert::URLEncode(extrasInfo));
	meeting->SetLocation(StringConvert::URLEncode(location));
	meeting->SetSummary(StringConvert::URLEncode(summary));
	meeting->SetTimeZone(timeZone);
	meeting->SetStartTime(startTime);
	meeting->SetDuration(duration);
	meeting->SetAuthorizerId(authorizerId);
	meeting->SetHostName(StringConvert::URLEncode(hostName));
	meeting->SetCycleRole(cycleRole);
	meeting->SetLanguage(language);
	meeting->SetEmailLanguage(emailLanguage);
	meeting->IsGNet(isGnet);
	meeting->IsCycle(isCycle);
	meeting->IsWholeDay(isWholeDay);
	if (authorizerId > 0) {
		meeting->SetHostId(authorizerId);
	} else {
		meeting->SetHostId(GetUserId());
	}

	HTTPRequest request;
	_calendarRequestFactory->MakeRequestOfCreateMeeting(meeting, request);
	_engine->SendAsyncRequest(request, MakeRequestCallback(this, &CalendarService::OnMeetingCreated, meeting));

	UC_LOG(INFO) << "CreateMeeting over. request URL = " << request.GetURL() << ", request body = " << request.GetBody();
	return UC_SUCCEEDED;
}

int32_t CalendarService::OnMeetingCreated( int32_t errorCode, HTTPResponse* response, Meeting* meeting ) {
	int32_t result = HTTPResponseAssist::GuardStatementForCallBack("OnMeetingCreated", errorCode, response);
	UC_LOG(INFO) << "OnMeetingCreated enter. Http request result = " << result;

	if (result != UC_SUCCEEDED) {
		triggerEvent(ICalendarServiceEvent, OnMeetingCreated(result, *meeting));
		SAFE_DELETE(meeting);
		return result;
	}

	//解析json
	int32_t code = -1;
	try {
		wstringstream jsonStream;
		boost::property_tree::wptree ptRoot;
		jsonStream.str(StringConvert::FromUTF8Str(response->GetContent()).c_str());
		boost::property_tree::json_parser::read_json(jsonStream, ptRoot);
		code = ptRoot.get<int32_t>(KEY_CODE, code);
		
		if (code != 0) {
			//server返回失败
			UC_LOG(ERROR) << "OnMeetingCreated error, server response error code = " << code;
			triggerEvent(ICalendarServiceEvent, OnMeetingCreated(code, *meeting));
			SAFE_DELETE(meeting);
			return code;
		}
		boost::property_tree::wptree ptData;
		boost::property_tree::wptree ptDef;
		ptData = ptRoot.get_child(KEY_DATA, ptDef);
		if (ptData.empty()) {
			//数据为空
			UC_LOG(ERROR) << "OnMeetingCreated error, server response data is null.";
			triggerEvent(ICalendarServiceEvent, OnMeetingCreated(code, *meeting));
			SAFE_DELETE(meeting);
			return code;
		}

		meeting->SetConferenceId(StringConvert::ToUTF8Str(ptData.get<std::wstring>((KEY_CONFERENCEID), L"")));
		meeting->SetEventId(ptData.get<int32_t>((KEY_EVENTID), 0));
		meeting->SetHostPassword(StringConvert::ToUTF8Str(ptData.get<std::wstring>((KEY_HOSTPASSWORD), L"")));
		meeting->SetAttendeePassword(StringConvert::ToUTF8Str(ptData.get<std::wstring>((KEY_ATTENDEEPASSWORD), L"")));
		meeting->SetBillingCode(StringConvert::ToUTF8Str(ptData.get<std::wstring>((KEY_BILLINGCODE), L"")));
		meeting->SetGroupId(ptData.get<int64_t>((KEY_GROUPID), 0));
	}
	catch(boost::property_tree::ptree_error & e) {
		UC_LOG(ERROR) << "encounter an exception when parse JSON from OnMeetingCreated , exception: " << e.what();
		triggerEvent(ICalendarServiceEvent, OnMeetingCreated(JSON_PARSE_ERROR, *meeting));
		SAFE_DELETE(meeting);
		return JSON_PARSE_ERROR;
	}

	//保存数据库
	int ret = DBManager::GetInstance().SaveMeeting(*meeting);
	if (0 != ret) {
		UC_LOG(ERROR) << "OnMeetingCreated save meeting to DB failed, error code = " << ret;
	}

	//回调上层
	triggerEvent(ICalendarServiceEvent, OnMeetingCreated(code, *meeting));
	
	SAFE_DELETE(meeting);
	UC_LOG(INFO) << "OnMeetingCreated success.";
	return code;
}

int32_t CalendarService::UpdateMeeting(int64_t eventId, int64_t oriTime, const std::vector<Attendee>& attendees,
	const std::string& title, const std::string& location, const std::string& summary,
	const std::string& timeZone, const std::string& extrasInfo, int64_t startTime, int32_t duration, int32_t authorizerId,
	CycleRole& cycleRole, int32_t isGnet /*= 1*/,
	int32_t isCycle /*= 0*/, int32_t isWholeDay /*= 0*/, int32_t language /*= 1*/, int32_t emailLanguage /*= 1 */)
{
	UC_LOG(INFO) << "UpdateMeeting enter. meeting event id = " << eventId << ", oriTime = " << oriTime << ", authorizerId = " << authorizerId << ", isCycle = " << isCycle;
	if (authorizerId < 0 || authorizerId == GetUserId()) {
		authorizerId = 0;
	}
	HTTPRequest request;
	Meeting* meeting = new Meeting;
	DBManager::GetInstance().GetMeeting(*meeting, eventId, oriTime, authorizerId);

	meeting->SetEventId(eventId);
	meeting->SetStartTime(startTime);
	meeting->SetAuthorizerId(authorizerId);
	meeting->SetAttendees(attendees);
	meeting->SetTitle(StringConvert::URLEncode(title));
	meeting->SetExtrasInfo(StringConvert::URLEncode(extrasInfo));
	meeting->SetLocation(StringConvert::URLEncode(location));
	meeting->SetSummary(StringConvert::URLEncode(summary));
	meeting->SetTimeZone(timeZone);
	meeting->SetDuration(duration);
	meeting->SetLanguage(language);
	meeting->SetEmailLanguage(emailLanguage);
	meeting->SetCycleRole(cycleRole);
	meeting->IsGNet(isGnet);
	meeting->IsCycle(isCycle);
	meeting->IsWholeDay(isWholeDay);

	_calendarRequestFactory->MakeRequestOfUpdateMeeting(oriTime, meeting, request);
	_engine->SendAsyncRequest(request, MakeRequestCallback(this, &CalendarService::OnMeetingUpdated, eventId, oriTime, authorizerId, meeting));

	UC_LOG(INFO) << "UpdateMeeting over. eventId = " << eventId << ", request URL = " << request.GetURL() << ", request body = " << request.GetBody();
	return UC_SUCCEEDED;
}

int32_t CalendarService::OnMeetingUpdated(int32_t errorCode, HTTPResponse* response, int64_t eventId, int64_t oriStart, int32_t authorizerId, Meeting* meeting) {
	int32_t result = HTTPResponseAssist::GuardStatementForCallBack("OnMeetingUpdated", errorCode, response);
	UC_LOG(INFO) << "OnMeetingUpdated enter. Http request result = " << result << ", eventId:" << eventId << ", oriStart:" << oriStart << ", authorizerId:"<< authorizerId;
	
	if (result != UC_SUCCEEDED) {
		triggerEvent(ICalendarServiceEvent, OnMeetingUpdated(result, eventId, oriStart, *meeting));
		SAFE_DELETE(meeting);
		return result;
	}

	//解析json
	int32_t code = -1;
	try {
		wstringstream jsonStream;
		boost::property_tree::wptree ptRoot;
		jsonStream.str(StringConvert::FromUTF8Str(response->GetContent()).c_str());
		boost::property_tree::json_parser::read_json(jsonStream, ptRoot);

		code = ptRoot.get<int32_t>(KEY_CODE, code);
		if (code != 0) {
			UC_LOG(ERROR) << "OnMeetingUpdated error, server response failed, error code = " << code << ", event id = " << meeting->GetEventId();
			triggerEvent(ICalendarServiceEvent, OnMeetingUpdated(code, eventId, oriStart, *meeting));
			SAFE_DELETE(meeting);
			return code;
		}
	}
	catch(boost::property_tree::ptree_error & e) {
		UC_LOG(ERROR) << "encounter an exception when parse JSON from OnMeetingUpdated , exception: " << e.what();
		triggerEvent(ICalendarServiceEvent, OnMeetingUpdated(JSON_PARSE_ERROR, eventId, oriStart, *meeting));
		SAFE_DELETE(meeting);
		return JSON_PARSE_ERROR;
	}

	//更新数据库
	int ret = DBManager::GetInstance().UpdateMeeting(*meeting, eventId, oriStart, authorizerId);
	if (DBError::DB_SUCCESS != ret) {
		UC_LOG(ERROR) << "OnMeetingUpdated error, UpdateMeeting to DB failed, error code = " << ret;
	}

	//回调上层
	triggerEvent(ICalendarServiceEvent, OnMeetingUpdated(code, eventId, oriStart, *meeting));

	SAFE_DELETE(meeting);
	UC_LOG(INFO) << "OnMeetingUpdated success.";
	return code;
}

int32_t CalendarService::CancelMeeting(int64_t eventId, int64_t startTime /*= 0*/, int32_t authorizerId /*= 0*/, int32_t language /*= 1*/, int32_t emailLanguage /*= 1 */) {
	UC_LOG(INFO) << "CancelMeeting enter. meeting event id = " << eventId;
	HTTPRequest request;
	_calendarRequestFactory->MakeRequestOfCancelMeeting(eventId, startTime, authorizerId, language, emailLanguage, request);
	_engine->SendAsyncRequest(request, MakeRequestCallback(this, &CalendarService::OnMeetingCanceled, eventId, startTime, authorizerId));
	UC_LOG(INFO) << "CancelMeeting over. event id = " << eventId << ", request URL = " << request.GetURL() << ", request body = " << request.GetBody();
	return UC_SUCCEEDED;
}

int32_t CalendarService::OnMeetingCanceled(int32_t errorCode, HTTPResponse* response, int64_t eventId, int64_t startTime, int32_t authorizerId) {
	int32_t result = HTTPResponseAssist::GuardStatementForCallBack("OnMeetingCanceled", errorCode, response);
	UC_LOG(INFO) << "OnMeetingCanceled enter. Http request result = " << result << ", eventId:" << eventId << ", startTime:" << startTime << ", authorizerId:" << authorizerId;
	if (result != UC_SUCCEEDED) {
		triggerEvent(ICalendarServiceEvent, OnMeetingCanceled(result, eventId, startTime, authorizerId));
		return result;
	}

	//解析json
	int32_t code = -1;
	try {
		wstringstream jsonStream;
		boost::property_tree::wptree ptRoot;
		jsonStream.str(StringConvert::FromUTF8Str(response->GetContent()).c_str());
		boost::property_tree::json_parser::read_json(jsonStream, ptRoot);

		code = ptRoot.get<int32_t>((KEY_CODE), code);
		if (code != 0) {
			UC_LOG(ERROR) << "OnMeetingCanceled error, server response failed, error code = " << code << ", meeting event id = " << eventId;
			triggerEvent(ICalendarServiceEvent, OnMeetingCanceled(code, eventId, startTime, authorizerId));
			return code;
		}
	}
	catch(boost::property_tree::ptree_error & e) {
		UC_LOG(ERROR) << "encounter an exception when parse JSON from OnMeetingCanceled , exception: " << e.what();
		triggerEvent(ICalendarServiceEvent, OnMeetingCanceled(JSON_PARSE_ERROR, eventId, startTime, authorizerId));
		return JSON_PARSE_ERROR;
	}

	//更新数据库
	int ret = DBManager::GetInstance().UpdateMeetingStatus(MeetingStatus::MS_CANCELED, eventId, startTime, authorizerId);
	if (ret != 0) {
		UC_LOG(ERROR) << "OnMeetingCanceled error, UpdateMeetingStatus to DB failed, error code = " << code << ", event id = " << eventId;
	}

	//回调上层
	triggerEvent(ICalendarServiceEvent, OnMeetingCanceled(code, eventId, startTime, authorizerId));
	
	UC_LOG(INFO) << "OnMeetingCanceled success.";
	return code;
}


int32_t CalendarService::ForwardMeeting(int64_t eventId, const std::vector<Attendee>& attendees, int64_t startTime /*= 0*/, int32_t authorizerId /*= 0*/, int32_t language /*= 1*/, int32_t emailLanguage /*= 1 */) {
	UC_LOG(INFO) << "ForwardMeeting enter. event id = " << eventId << ", authorizer id = " << authorizerId << ", startTime = " << startTime;
	if (authorizerId < 0 || authorizerId == GetUserId()) {
		authorizerId = 0;
	}
	HTTPRequest request;
	_calendarRequestFactory->MakeRequestOfForwardMeeting(eventId, attendees, authorizerId, startTime, language, emailLanguage, request);
	_engine->SendAsyncRequest(request, MakeRequestCallback(this, &CalendarService::OnMeetingForwarded, eventId, startTime, attendees, authorizerId));
	UC_LOG(INFO) << "ForwardMeeting over. request url = " << request.GetURL() << ", body = " << request.GetBody();
	return UC_SUCCEEDED;
}

int32_t CalendarService::OnMeetingForwarded(int32_t errorCode, HTTPResponse* response, int64_t eventId, int64_t startTime, std::vector<Attendee> attendees, int32_t authorizerId) {
	int32_t result = HTTPResponseAssist::GuardStatementForCallBack("OnMeetingForwarded", errorCode, response);
	UC_LOG(INFO) << "OnMeetingForwarded enter. Http request result = " << result << ", eventId = " << eventId << ", startTime = " << startTime << ", authorizerId = " << authorizerId;

	Meeting meeting;
	meeting.SetEventId(eventId);
	meeting.SetStartTime(startTime);

	if (result != UC_SUCCEEDED) {
		triggerEvent(ICalendarServiceEvent, OnMeetingForwarded(result, meeting));
		return result;
	}

	//解析json
	int32_t code = -1;
	try {
		wstringstream jsonStream;
		boost::property_tree::wptree ptRoot;
		jsonStream.str(StringConvert::FromUTF8Str(response->GetContent()).c_str());
		boost::property_tree::json_parser::read_json(jsonStream, ptRoot);
		code = ptRoot.get<int32_t>((KEY_CODE), code);
		if (code != 0) {
			UC_LOG(ERROR) << "OnMeetingForwarded error, server response failed, error code = " << code;
			triggerEvent(ICalendarServiceEvent, OnMeetingForwarded(code, meeting));
			return code;
		}
	}
	catch (boost::property_tree::ptree_error & e) {
		UC_LOG(ERROR) << "encounter an exception when parse JSON from OnMeetingForwarded , exception: " << e.what();
		triggerEvent(ICalendarServiceEvent, OnMeetingForwarded(JSON_PARSE_ERROR, meeting));
		return JSON_PARSE_ERROR;
	}

	//转发成功后，更新参会人信息
	if (DBError::DB_SUCCESS == DBManager::GetInstance().GetMeeting(meeting, eventId, startTime, authorizerId)){
		meeting.AddAttendees(attendees);
		//更新数据库参会人信息
		DBManager::GetInstance().AddAttendees(attendees, eventId, meeting.GetStartTime(), authorizerId);
	}
	else{
		if (FindInCacheMeeting(eventId, startTime, authorizerId, meeting)){
			meeting.AddAttendees(attendees);
			CacheMeeting(meeting);
		}
	}

	//回调上层
	triggerEvent(ICalendarServiceEvent, OnMeetingForwarded(code, meeting));

	UC_LOG(INFO) << "OnMeetingForwarded success.";
	return code;
}


int32_t CalendarService::AcceptMeetingInvitation(int64_t eventId, int64_t startTime /*= 0 */, int32_t authorizerId /*= 0 */)
{
	UC_LOG(INFO) << "AcceptMeetingInvitation enter. eventId = " << eventId << ", authorizerId = " << authorizerId << ", startTime = " << startTime;
	if (authorizerId < 0 || authorizerId == GetUserId()) {
		authorizerId = 0;
	}
	HTTPRequest request;
	_calendarRequestFactory->MakeRequestOfAccptMeetingInvitation(eventId, authorizerId, startTime, request);
	_engine->SendAsyncRequest(request, MakeRequestCallback(this, &CalendarService::OnMeetingInvitationAccepted, eventId, authorizerId, startTime));
	UC_LOG(INFO) << "AcceptMeetingInvitation over. request URL = " << request.GetURL() << ", request body = " << request.GetBody();
	return UC_SUCCEEDED;
}

int32_t CalendarService::OnMeetingInvitationAccepted(int32_t errorCode, HTTPResponse* response, int64_t eventId, int32_t authorizerId, int64_t startTime) {
	int32_t result = HTTPResponseAssist::GuardStatementForCallBack("OnMeetingInvitationAccepted", errorCode, response);
	UC_LOG(INFO) << "OnMeetingInvitationAccepted enter. Http request result = " << result << ", eventId:" << eventId << ", startTime:" << startTime <<", authorizerId:" << authorizerId ;
	if (result != UC_SUCCEEDED) {
		triggerEvent(ICalendarServiceEvent, OnMeetingInvitationAccepted(result, eventId, startTime, authorizerId));
		return result;
	}

	//解析json
	int32_t code = -1;
	try {
		wstringstream jsonStream;
		boost::property_tree::wptree ptRoot;
		jsonStream.str(StringConvert::FromUTF8Str(response->GetContent()).c_str());
		boost::property_tree::json_parser::read_json(jsonStream, ptRoot);
		code = ptRoot.get<int32_t>((KEY_CODE), code);
		if (code != 0) {
			UC_LOG(ERROR) << "OnMeetingInvitationAccepted error, server response failed, error code = " << code;
			triggerEvent(ICalendarServiceEvent, OnMeetingInvitationAccepted(code, eventId, startTime, authorizerId));
			return code;
		}
	}
	catch (boost::property_tree::ptree_error & e) {
		UC_LOG(ERROR) << "encounter an exception when parse JSON from OnMeetingInvitationAccepted , exception: " << e.what();
		triggerEvent(ICalendarServiceEvent, OnMeetingInvitationAccepted(JSON_PARSE_ERROR, eventId, startTime, authorizerId));
		return JSON_PARSE_ERROR;
	}

	//更新数据库参会人状态
	stringstream ss;
	ss << GetUserId();
	std::string sUserId = ss.str();
	DBManager::GetInstance().UpdateAttendeeStatus(uc::AttendeeStatus::AS_ACCEPTED, sUserId, eventId, startTime, authorizerId);
	UpdateCahcheMeetingStatus(uc::AttendeeStatus::AS_ACCEPTED, eventId, startTime, authorizerId);
	//回调上层
	triggerEvent(ICalendarServiceEvent, OnMeetingInvitationAccepted(code, eventId, startTime, authorizerId));
	UC_LOG(INFO) << "OnMeetingInvitationAccepted success.";
	return code;
}


int32_t CalendarService::DenyMeetingInvitation(int64_t eventId, int64_t startTime /*= 0 */, int32_t authorizerId /*= 0 */) {
	UC_LOG(INFO) << "DenyMeetingInvitation enter. event id = " << eventId << ", authorizer id = " << authorizerId << ", startTime = " << startTime;
	if (authorizerId < 0 || authorizerId == GetUserId()) {
		authorizerId = 0;
	}
	HTTPRequest request;
	_calendarRequestFactory->MakeRequestOfDenyMeetingInvitation(eventId, authorizerId, startTime, request);
	_engine->SendAsyncRequest(request, MakeRequestCallback(this, &CalendarService::OnMeetingInvitationDenied, eventId, authorizerId, startTime));
	UC_LOG(INFO) << "DenyMeetingInvitation over. request URL = " << request.GetURL() << ", request body = " << request.GetBody();
	return UC_SUCCEEDED;
}

int32_t CalendarService::OnMeetingInvitationDenied(int32_t errorCode, HTTPResponse* response, int64_t eventId, int32_t authorizerId, int64_t startTime) {
	int32_t result = HTTPResponseAssist::GuardStatementForCallBack("OnMeetingInvitationDenied", errorCode, response);
	UC_LOG(INFO) << "OnMeetingInvitationDenied enter. Http request result = " << result << ", eventId:" << eventId << ", startTime:" << startTime << ", authorizerId:" << authorizerId;

	if (result != UC_SUCCEEDED) {
		triggerEvent(ICalendarServiceEvent, OnMeetingInvitationDenied(result, eventId, startTime, authorizerId));
		return result;
	}

	//解析json
	int32_t code = -1;
	try {
		wstringstream jsonStream;
		boost::property_tree::wptree ptRoot;
		jsonStream.str(StringConvert::FromUTF8Str(response->GetContent()).c_str());
		boost::property_tree::json_parser::read_json(jsonStream, ptRoot);
		code = ptRoot.get<int32_t>((KEY_CODE), code);
		if (code != 0) {
			UC_LOG(ERROR) << "OnMeetingInvitationDenied error, server response failed, error code = " << code;
			triggerEvent(ICalendarServiceEvent, OnMeetingInvitationDenied(code, eventId, startTime, authorizerId));
			return code;
		}
	}
	catch (boost::property_tree::ptree_error & e) {
		UC_LOG(ERROR) << "encounter an exception when parse JSON from OnMeetingInvitationDenied , exception: " << e.what();
		triggerEvent(ICalendarServiceEvent, OnMeetingInvitationDenied(JSON_PARSE_ERROR, eventId, startTime, authorizerId));
		return JSON_PARSE_ERROR;
	}

	//更新数据库会议状态和参会人状态
	stringstream ss;
	ss << GetUserId();
	std::string sUserId = ss.str();
	DBManager::GetInstance().UpdateAttendeeStatus(AttendeeStatus::AS_DENIED, sUserId, eventId, startTime, authorizerId);
	UpdateCahcheMeetingStatus(AttendeeStatus::AS_DENIED, eventId, startTime, authorizerId);

	//回调上层
	triggerEvent(ICalendarServiceEvent, OnMeetingInvitationDenied(code, eventId, startTime, authorizerId));
	UC_LOG(INFO) << "OnMeetingInvitationDenied success.";
	return code;
}

int32_t CalendarService::GetMeetingStartURL(const std::string& confId, const std::string& password, std::string& startURL, int32_t& siteId) {
	UC_LOG(INFO) << "GetMeetingStartURL enter. confId = " << confId;
	Meeting meeting;
	if (!FindInCacheMeeting(confId, meeting)) {
		DBManager::GetInstance().GetMeeting(meeting, confId);
	}
	
	stringstream ss;
	string sDomain = GetDoMain();
	siteId = GetMeetingSiteId();
	
	ss << "tang:///app:6/skin:1/site:" << siteId << "/Version:0/wn:" << StringConvert::URLEncode(GetUserName())
		<< "/we:" << GetAccount() << "/userId:" << GetUserId() << "/wp:" << password 
		<< "/wucid:" << confId << "/ucDomain:" << sDomain << "/wfrom:uc/action:9/avatar:" << GetRemoteAvatar();
	startURL = ss.str();
	UC_LOG(INFO) << "GetMeetingStartURL construct tang URL over. URL = " << startURL;
	//triggerEvent(ICalendarServiceEvent, OnMeetingStartURLReceived(0, confId, startURL));

	//HTTPRequest request;
	//_calendarRequestFactory->MakeRequestOfGetMeetingStartURL(confId, password, request);
	//_engine->SendAsyncRequest(request, MakeRequestCallback(this, &CalendarService::OnMeetingStartURLReceived, confId));
	//UC_LOG(INFO) << "GetMeetingStartURL over. confId = " << confId << ", request URL = " << request.GetURL() << ", request body = " << request.GetBody();
	return UC_SUCCEEDED;
}


int32_t CalendarService::OnMeetingStartURLReceived( int32_t errorCode, HTTPResponse* response, std::string confId) {
	int32_t result = HTTPResponseAssist::GuardStatementForCallBack("OnMeetingStartURLReceived", errorCode, response);
	UC_LOG(INFO) << "OnMeetingStartURLReceived enter. Http request result = " << result << ", confId:" << confId;
	if (result != UC_SUCCEEDED) {
		triggerEvent(ICalendarServiceEvent, OnMeetingStartURLReceived(result, confId, ""));
		return result;
	}

	//解析json
	int32_t code = -1;
	std::string tangStartUrl = "";
	try {
		wstringstream jsonStream;
		boost::property_tree::wptree ptRoot;
		jsonStream.str(StringConvert::FromUTF8Str(response->GetContent()).c_str());
		boost::property_tree::json_parser::read_json(jsonStream, ptRoot);

		code = ptRoot.get<int32_t>((KEY_CODE), code);
		if (code != 0) {
			UC_LOG(ERROR) << "OnMeetingStartURLReceived error, server response failed, error code = " << code;
			triggerEvent(ICalendarServiceEvent, OnMeetingStartURLReceived(code, confId, tangStartUrl));
			return code;
		}

		tangStartUrl = StringConvert::ToUTF8Str(ptRoot.get<std::wstring>((KEY_DATA), L""));
	}
	catch(boost::property_tree::ptree_error &e) {
		UC_LOG(ERROR) << "encounter an exception when parse JSON from OnMeetingStartURLReceived , exception: " << e.what();
		triggerEvent(ICalendarServiceEvent, OnMeetingStartURLReceived(JSON_PARSE_ERROR, confId, tangStartUrl));
		return JSON_PARSE_ERROR;
	}

	//回调上层
	triggerEvent(ICalendarServiceEvent, OnMeetingStartURLReceived(code, confId, tangStartUrl));

	UC_LOG(INFO) << "OnMeetingStartURLReceived success.";
	return 0;
}


int32_t CalendarService::GetMeetingWebStartURL(const std::string& confId, const std::string& mail, const std::string& password, const std::string& displayName) {
	UC_LOG(INFO) << "GetMeetingWebStartURL enter. confId = " << confId;
	HTTPRequest request;
	_calendarRequestFactory->MakeRequestOfGetMeetingWebStartURL(confId, password, mail, displayName, request);
	_engine->SendAsyncRequest(request, MakeRequestCallback(this, &CalendarService::OnMeetingWebStartURLReceived, confId));
	UC_LOG(INFO) << "GetMeetingWebStartURL over. confId = " << confId << ", request URL = " << request.GetURL() << ", request body = " << request.GetBody();
	return UC_SUCCEEDED;
}


int32_t CalendarService::OnMeetingWebStartURLReceived(int32_t errorCode, HTTPResponse* response, std::string confId) {
	int32_t result = HTTPResponseAssist::GuardStatementForCallBack("OnMeetingWebStartURLReceived", errorCode, response);
	UC_LOG(INFO) << "OnMeetingWebStartURLReceived enter. Http request result = " << result << ", confId:" << confId;

	if (result != UC_SUCCEEDED) {
		triggerEvent(ICalendarServiceEvent, OnMeetingWebStartURLReceived(result, confId, ""));
		return result;
	}

	//解析json
	int32_t code = -1;
	std::string webStartUrl = "";
	try {
		wstringstream jsonStream;
		boost::property_tree::wptree ptRoot;
		jsonStream.str(StringConvert::FromUTF8Str(response->GetContent()).c_str());
		boost::property_tree::json_parser::read_json(jsonStream, ptRoot);
		code = ptRoot.get<int32_t>((KEY_CODE), code);
		if (code != 0) {
			UC_LOG(ERROR) << "OnMeetingWebStartURLReceived error, server response failed, error code = " << code;
			triggerEvent(ICalendarServiceEvent, OnMeetingWebStartURLReceived(code, confId, webStartUrl));
			return code;
		}

		webStartUrl = StringConvert::ToUTF8Str(ptRoot.get<std::wstring>((KEY_DATA), L""));
	}
	catch(boost::property_tree::ptree_error & e) {
		UC_LOG(ERROR) << "encounter an exception when parse JSON from OnMeetingWebStartURLReceived , exception: " << e.what();
		triggerEvent(ICalendarServiceEvent, OnMeetingWebStartURLReceived(JSON_PARSE_ERROR, confId, webStartUrl));
		return JSON_PARSE_ERROR;
	}
	
	//回调上层
	triggerEvent(ICalendarServiceEvent, OnMeetingWebStartURLReceived(code, confId, webStartUrl));

	UC_LOG(INFO) << "OnMeetingWebStartURLReceived success.";
	return code;
}

int32_t CalendarService::UpdateAccreditSetting( int32_t authorizedPersonId, int32_t status ) {
	UC_LOG(INFO) << "UpdateAccreditSetting enter. authorizer id = " << authorizedPersonId;
	HTTPRequest request;
	_calendarRequestFactory->MakeRequestOfUpdateAccreditSetting(authorizedPersonId, status, request);
	_engine->SendAsyncRequest(request, MakeRequestCallback(this, &CalendarService::OnAccreditSettingUpdated, authorizedPersonId, status));
	UC_LOG(INFO) << "UpdateAccreditSetting over. request URL = " << request.GetURL() << ", request body = " << request.GetBody();
	return UC_SUCCEEDED;
}

int32_t CalendarService::OnAccreditSettingUpdated( int32_t errorCode, HTTPResponse* response, int32_t authorizedPersonId, int32_t status) {
	int32_t result = HTTPResponseAssist::GuardStatementForCallBack("OnAccreditSettingUpdated", errorCode, response);
	UC_LOG(INFO) << "OnAccreditSettingUpdated enter. Http request result = " << result << ", authorizedPersonId = " << authorizedPersonId;

	if (result != UC_SUCCEEDED) {
		triggerEvent(ICalendarServiceEvent, OnAccreditSettingUpdated(result, authorizedPersonId, status));
		return result;
	}

	//解析json
	int32_t code = -1;
	try {
		wstringstream jsonStream;
		boost::property_tree::wptree ptRoot;
		jsonStream.str(StringConvert::FromUTF8Str(response->GetContent()).c_str());
		boost::property_tree::json_parser::read_json(jsonStream, ptRoot);
		code = ptRoot.get<int32_t>((KEY_CODE), code);
		if (code != 0) {
			UC_LOG(ERROR) << "OnAccreditSettingUpdated error, server response failed, error code = " << code;
			triggerEvent(ICalendarServiceEvent, OnAccreditSettingUpdated(code, authorizedPersonId, status));
			return code;
		}
	}
	catch(boost::property_tree::ptree_error & e) {
		UC_LOG(ERROR) << "encounter an exception when parse JSON from OnAccreditSettingUpdated , exception: " << e.what();
		triggerEvent(ICalendarServiceEvent, OnAccreditSettingUpdated(JSON_PARSE_ERROR, authorizedPersonId, status));
		return JSON_PARSE_ERROR;
	}

	Accrediting info;
	info.SetUserId(authorizedPersonId);
	info.SetStatus(status);
	//存数据库
	DBManager::GetInstance().SaveAccreditUser(info);

	//回调上层
	triggerEvent(ICalendarServiceEvent, OnAccreditSettingUpdated(code, authorizedPersonId, status));

	UC_LOG(INFO) << "OnAccreditSettingUpdated success.";
	return code;
}

int32_t CalendarService::GetAccreditUserList(vector<Accrediting>& accreditList) {
	UC_LOG(INFO) << "GetAccreditUserList enter.";

	//先从数据库取
// 	int32_t ret = DBManager::GetInstance().GetAccreditUserList(accreditList);
// 	if (0 == ret) {
// 		UC_LOG(INFO) << "GetAccreditUserList form DB success.";
// 		return UC_SUCCEEDED;
// 	}

	//从服务器取
	HTTPRequest request;
	_calendarRequestFactory->MakeRequestOfGetAccreditUserList( request );
	_engine->SendAsyncRequest(request, MakeRequestCallback(this, &CalendarService::OnAccreditUserListReceived));
	UC_LOG(INFO) << "GetAccreditUserList over. request URL = " << request.GetURL() << ", request body = " << request.GetBody();
	return UC_SUCCEEDED;
}

int32_t CalendarService::OnAccreditUserListReceived( int32_t errorCode, HTTPResponse* response ) {
	int32_t result = HTTPResponseAssist::GuardStatementForCallBack("OnAccreditUserListReceived", errorCode, response);
	UC_LOG(INFO) << "OnAccreditUserListReceived enter. Http request result = " << result;

	std::vector<Accrediting> accreditList;
	if (result != UC_SUCCEEDED) {
		triggerEvent(ICalendarServiceEvent, OnAccreditUserListReceived(result, accreditList));
		return result;
	}

	//解析json
	//{"code":0,"data":{"owner":"477828","other":["399610",]},"request_id":"1433924746.74448600.14638"}
	int32_t code = -1;
	try {
		wstringstream jsonStream;
		boost::property_tree::wptree ptRoot;
		jsonStream.str(StringConvert::FromUTF8Str(response->GetContent()).c_str());
		boost::property_tree::json_parser::read_json(jsonStream, ptRoot);
		code = ptRoot.get<int32_t>((KEY_CODE), code);
		if (code != 0) {
			UC_LOG(ERROR) << "OnAccreditUserListReceived error, server response failed, error code = " << code;
			triggerEvent(ICalendarServiceEvent, OnAccreditUserListReceived(code, accreditList));
			return code;
		}

		int32_t accreditId;
		Accrediting accredit;
		std::wstring otherUser;
		std::wstringstream stream;
		boost::property_tree::wptree ptChild = ptRoot.get_child((KEY_DATA));
		if (ptChild.empty()) {
			UC_LOG(ERROR) << "OnAccreditUserListReceived error, server response data is null";
			triggerEvent(ICalendarServiceEvent, OnAccreditUserListReceived(code, accreditList));
			return code;
		}
	
		//自己授权给他人
		std::wstring authorizedPersonId = ptChild.get<std::wstring>((KEY_OWNER), L"");
		if (!authorizedPersonId.empty()) {
			stream << authorizedPersonId;
			stream >> accreditId;
			accredit.SetUserId(accreditId);
			accredit.SetStatus(0);
			accreditList.push_back(accredit);
		}


		//其他人授权给自己的
		boost::property_tree::wptree ptOthers = ptChild.get_child(KEY_OTHER);
		if (ptOthers.empty()) {
			UC_LOG(ERROR) << "OnAccreditUserListReceived error, server response other is null";
			triggerEvent(ICalendarServiceEvent, OnAccreditUserListReceived(code, accreditList));
			return code;
		}
		BOOST_FOREACH(const boost::property_tree::wptree::value_type &vElement, ptOthers){
			boost::property_tree::wptree jsonValue = vElement.second;
			otherUser = jsonValue.get_value<std::wstring>();
			if (!otherUser.empty()) {
				stream.str(L"");
				stream.clear();
				stream << otherUser;
				stream >> accreditId;
				accredit.SetUserId(accreditId);
				accredit.SetStatus(1);
				accreditList.push_back(accredit);
			}
		}
	}
	catch(boost::property_tree::ptree_error & e) {
		UC_LOG(ERROR) << "encounter an exception when parse JSON from OnAccreditUserListReceived , exception: " << e.what();
		triggerEvent(ICalendarServiceEvent, OnAccreditUserListReceived(JSON_PARSE_ERROR, accreditList));
		return JSON_PARSE_ERROR;
	}

	//更新数据库
// 	for(unsigned int i=0; i<accreditList.size(); ++i) {
// 		DBManager::GetInstance().SaveAccreditUser(accreditList[i]);
// 	}
	
	//回调上层
	triggerEvent(ICalendarServiceEvent, OnAccreditUserListReceived(code, accreditList));
	
	UC_LOG(INFO) << "OnAccreditUserListReceived success.";
	return code;
}


int32_t CalendarService::GetMeetingList(int64_t startTime, int64_t endTime, int32_t limit, int32_t authorizerId, int64_t eventIdFilter, bool bfilterAsc, int32_t refreshflag, OUT int32_t& requestId, OUT vector<Meeting>& meetings)
{
	requestId = MakeRequestId();
	UC_LOG(INFO) << "GetMeetingList enter! request id = " << requestId << ", authorizerId = "<< authorizerId << ", startTime  = "<< startTime
		<< ", endTime = " << endTime << ", limit = " << limit << ", eventIdFilter = " << eventIdFilter << ", bfilterAsc = " << bfilterAsc << ", refreshflag = "<<refreshflag;

	//参数修正
	limit = limit < 0 ? 0 : limit; 
	int32_t type = -1;	//0 -- 具有startTime和endTime; 1 -- 只有startTime; 2 -- 只有endTime.
	if (startTime > 0 && endTime > 0) {
		if (startTime > endTime) {
			int64_t temp = startTime;
			startTime = endTime;
			endTime = temp;
		}
		type = 0;
	} else if (startTime > 0 && endTime <= 0) {
		type = 1;	//new
	}else if (startTime <= 0 && endTime >0) {
		type = 2;	//old
	}

	if (authorizerId < 0 || authorizerId == GetUserId()) {
		authorizerId = 0;
	}

	if (type == -1) {
		UC_LOG(ERROR) << "GetMeetingList input parameters error.";
		return UC_FAILED;
	}

	int64_t newStartTime = startTime;
	int64_t newEndTime = endTime;
	int32_t newLimit = limit;
	//刷新标记：默认0 -- 本地不够server凑; 1 -- 强制刷新,直接从服务器同步,忽略本地数据; 2 -- 只取本地数据.
	if (refreshflag == 0) {
		int32_t retDB = -1;
		bool bCompleted = false;
		//判断请求时间段是否已经同步过
		if (type == 0) {	
			//具有startTime和endTime
			if(IsTimeInSyncRange(authorizerId, startTime) && IsTimeInSyncRange(authorizerId, endTime)) {
				retDB = DBManager::GetInstance().GetMeetingList(meetings, startTime, endTime, limit, authorizerId, eventIdFilter, bfilterAsc);
				bCompleted = true;
			} else if (IsTimeInSyncRange(authorizerId, startTime)) {
				newStartTime = GetSyncRangEnd(authorizerId);
				retDB = DBManager::GetInstance().GetMeetingList(meetings, startTime, newStartTime, limit, authorizerId, eventIdFilter, bfilterAsc);
			} else if (IsTimeInSyncRange(authorizerId, endTime)) {
				newEndTime = GetSyncRangStart(authorizerId);
				retDB = DBManager::GetInstance().GetMeetingList(meetings, newEndTime, endTime, limit, authorizerId, eventIdFilter, bfilterAsc);
			}		
		} else if (type == 1) {	
			// 只有startTime
			if (IsTimeInSyncRange(authorizerId, startTime)) {
				newStartTime = GetSyncRangEnd(authorizerId);
				retDB = DBManager::GetInstance().GetMeetingList(meetings, startTime, newStartTime, 200, authorizerId, eventIdFilter, bfilterAsc);
			}
		} else if( type == 2 ) {
			// 只有endTime
			if (IsTimeInSyncRange(authorizerId, endTime)) {
				newEndTime = GetSyncRangStart(authorizerId);
				retDB = DBManager::GetInstance().GetMeetingList(meetings, newEndTime, endTime, 200, authorizerId, eventIdFilter, bfilterAsc);
			}
		}
		
		EraseNoDisplayedMeeting(meetings);
		UC_LOG(INFO) << "GetMeetingList, refreshFlag == 0, after qurey local db, number of meetings = " << meetings.size(); 
		//查库结果,且所有会议都不为old状态
		if (retDB == DBError::DB_SUCCESS && !haveOldMeeting(meetings)) {
			//按startTime升序排列
			SortMeetingList(meetings);
			if (type == 0) {
				//请求的起始时间和终止时间均在已同步范围内
				if (bCompleted) {
					UC_LOG(INFO) << "GetMeetingList from DB success. startTime = "<< startTime <<", endTime = " << endTime;
					return UC_SUCCEEDED;
				}

			} else {
				size_t tempLimit = (size_t)limit;
				// 只有起始时间或终止时间
				if (meetings.size() >= tempLimit) {
					//本地数据可以满足需求，直接返回
					vector<Meeting> vTemp;
					if (type == 1){
						//只有startTime取前limit条
						vTemp.assign(meetings.begin(), meetings.begin() + tempLimit);
					}else{
						//只有endTime取后limit条
						int32_t beg = meetings.size() - tempLimit;
						vTemp.assign(meetings.begin()+beg, meetings.end());
					}
					meetings.assign(vTemp.begin(), vTemp.end());					
					UC_LOG(INFO) << "GetMeetingList from DB success. startTime = "<< startTime <<", endTime = " << endTime;
					return UC_SUCCEEDED;
				}else{
					//本地数据不够，继续请求服务器
					newLimit = limit - meetings.size();
				}
			}
		}
		else {
			//本地数据不可用
			newStartTime = startTime;
			newEndTime = endTime;
			meetings.clear();
		}
	}
	else if (refreshflag == 2) {
		//仅从本地数据库找数据
		DBManager::GetInstance().GetMeetingList(meetings, startTime, endTime, limit, authorizerId, eventIdFilter, bfilterAsc);
		EraseNoDisplayedMeeting(meetings);
		UC_LOG(INFO) << "GetMeetingList, refreshFlag == 2, after qurey local db, number of meetings = " << meetings.size();
		return UC_SUCCEEDED;
	}
	
	HTTPRequest request;
	_calendarRequestFactory->MakeRequestOfGetMeetingList( authorizerId, newStartTime, newEndTime, newLimit, eventIdFilter, bfilterAsc, request);
	_engine->SendAsyncRequest(request, MakeRequestCallback(this, &CalendarService::OnMeetingListReceived, requestId, authorizerId, startTime, endTime, eventIdFilter, meetings));
	UC_LOG(INFO) << "GetMeetingList over. request URL = " << request.GetURL() << ", request body = " << request.GetBody();
	return UC_SUCCEEDED;
}

void CalendarService::CacheMeetingList() {
	static vector<Meeting> meetings;

	//as a tip to set requestid as 0 to avoid to callback to JS
	HTTPRequest request;
	int64_t startTime = GetTodayTime();
	_bAlreadyCleanup = false;
	
	_calendarRequestFactory->MakeRequestOfGetMeetingList(0, startTime, 0, CACH_NUMMBER, 0, true, request);
	((UCEngine*)_engine)->SendAsyncRequest(request, MakeRequestCallback(this, &CalendarService::OnMeetingListReceived, -1, 0, startTime, (int64_t)0, (int64_t)0, meetings));

	_calendarRequestFactory->MakeRequestOfGetMeetingList(0, 0, startTime, CACH_NUMMBER, 0, false, request);
	((UCEngine*)_engine)->SendAsyncRequest(request, MakeRequestCallback(this, &CalendarService::OnMeetingListReceived, -2, 0, (int64_t)0, startTime, (int64_t)0, meetings));
}

int32_t CalendarService::OnMeetingListReceived(int32_t errorCode, HTTPResponse* response, int32_t requestId, int32_t authorizerId, int64_t startTime, int64_t endTime, int64_t eventIdFilter, vector<Meeting> localMeetings) {
	int32_t result = HTTPResponseAssist::GuardStatementForCallBack("OnMeetingListReceived", errorCode, response);
	UC_LOG(INFO) << "OnMeetingListReceived enter. Http request result = " << result << ", SDK request id = " << requestId << ", startTime = " << startTime << ", endTime = " << endTime << ", authorizerId = " << authorizerId;
	authorizerId = authorizerId < 0 ? 0 : authorizerId;
	if (requestId < 0 && _bAlreadyCleanup == false) {
		UC_LOG(INFO) << "SDK cache meeting list received, clean up local DB.";
		_bAlreadyCleanup = true;
		
		// 同步范围重置
		_syncInfo.clear();

		// 清空数据库中所有会议数据
		DBManager::GetInstance().CleanupMeetings();
	}

	if (result != UC_SUCCEEDED) {
		triggerEvent_condition(ICalendarServiceEvent, requestId > 0, OnMeetingListReceived(result, requestId, startTime, endTime, eventIdFilter, authorizerId, localMeetings));
		return result;
	}

	//解析json
	int32_t code = -1;
	std::vector<Meeting> meetings;
	meetings.clear();
	rapidjson::Document document;
	if (!document.Parse<0>(response->GetContent().c_str()).HasParseError()) {
		code = jsonhelper::getIntValue(document, "code", -1);
		std::string sRequestId = jsonhelper::getStrValue(document, "request_id", "");
		if (0 == code) {
			rapidjson::Value &data = document["data"];
			std::string icalendar = jsonhelper::getStrValue(data, "icalendar", "");
			ParseICalendarStr(icalendar, meetings);
		}
		else {
			UC_LOG(ERROR) << "OnMeetingListReceived error, server response failed, error code = " << code << ", response request_id = " << sRequestId;
			triggerEvent_condition(ICalendarServiceEvent, requestId > 0, OnMeetingListReceived(code, requestId, startTime, endTime, eventIdFilter, authorizerId, localMeetings));
			return code;
		}
	}
	else{
		UC_LOG(ERROR) << "encounter an exception when parse JSON from OnMeetingListReceived , exception: " << document.GetParseError();
		triggerEvent_condition(ICalendarServiceEvent, requestId > 0, OnMeetingListReceived(JSON_PARSE_ERROR, requestId, startTime, endTime, eventIdFilter, authorizerId, localMeetings));
		return JSON_PARSE_ERROR;
	}

	int32_t size = meetings.size();
	int64_t RstartTime = startTime;
	int64_t RendTime = endTime;
	UC_LOG(INFO) << "OnMeetingListReceived, after parse response, number of meetings = " << size << ", request id = " << requestId;
	if (size > 0) {
		SetMeetingAuthorizerId(meetings, authorizerId);
	
		//计算本次同步范围 
		SortMeetingList(meetings);		
		int64_t st1 = meetings[0].GetStartTime();
		int64_t st2 = meetings[size - 1].GetStartTime();
		RstartTime = RstartTime <= 0 ? st1 : RstartTime;
		RendTime = RendTime <= 0 ? st2 : RendTime;

		//更新数据库
		UC_LOG(INFO) << "OnMeetingListReceived, Save meetings to DB.";
		int ret = DBManager::GetInstance().SaveMeetings(meetings);
		if (ret == DBError::DB_SUCCESS) {
			//保存成功再更新同步范围
			UpdateSyncRange(authorizerId, RstartTime, RendTime);			
		} else {
			UC_LOG(ERROR) << "OnMeetingListReceived save meetings failed.";
		}		
	} else if (startTime > 0 && endTime > 0) {
		UpdateSyncRange(authorizerId, startTime, endTime);
	}
	
	//回调上层
	meetings.insert(meetings.end(), localMeetings.begin(), localMeetings.end());
	SortMeetingList(meetings);
	UC_LOG(INFO) << "OnMeetingListReceived trigger event to CEF. SDK request id = " << requestId;
	triggerEvent_condition(ICalendarServiceEvent, requestId > 0, OnMeetingListReceived(code, requestId, startTime, endTime, eventIdFilter, authorizerId, meetings));
	UC_LOG(INFO) << "OnMeetingListReceived success.";
	return code;
}

int32_t CalendarService::GetBriefMeetingList(int64_t startTime, int64_t endTime, int32_t authorizerId, OUT int32_t& requestId, OUT vector<Meeting>& meetings) {
	UC_LOG(INFO) << "GetMeetingBriefList enter. startTime = " << startTime << ", endTime = " << endTime << ", authorizerId = " << authorizerId;
	if (authorizerId < 0 || authorizerId == GetUserId()) {
		authorizerId = 0;
	}
	requestId = MakeRequestId();
	HTTPRequest request;
	_calendarRequestFactory->MakeRequestOfGetBriefMeetingList(startTime, endTime, authorizerId, request);

	_engine->SendAsyncRequest(request, MakeRequestCallback(this, &CalendarService::OnBriefMeetingListReceived, requestId, authorizerId, startTime, endTime));

	UC_LOG(INFO) << "GetMeetingBriefList over. request URL = " << request.GetURL() << ", request body = " << request.GetBody();
	return UC_SUCCEEDED;
}

int32_t CalendarService::OnBriefMeetingListReceived(int32_t errorCode, HTTPResponse* response, int32_t requestId, int32_t authorizerId, int64_t startTime, int64_t endTime) {
	
	int32_t result = HTTPResponseAssist::GuardStatementForCallBack("OnBriefMeetingListReceived", errorCode, response);
	UC_LOG(INFO) << "OnBriefMeetingListReceived enter. Http request result = " << result << ", SDK request id = " << requestId << ", authorizerId = " << authorizerId << ", startTime = " << startTime << ", endTime = " << endTime;

	std::vector<Meeting> meetings;
	if (result != UC_SUCCEEDED) {
		triggerEvent(ICalendarServiceEvent, OnBriefMeetingListReceived(result, requestId, startTime, endTime, authorizerId, meetings));
		return result;
	}

	//解析json
	int32_t code = -1;
	try {
		wstringstream jsonStream;
		boost::property_tree::wptree ptRoot;
		jsonStream.str(StringConvert::FromUTF8Str(response->GetContent()).c_str());
		boost::property_tree::json_parser::read_json(jsonStream, ptRoot);
		code = ptRoot.get<int32_t>((KEY_CODE), code);
		if (code != 0) {
			std::wstring sRequestId = ptRoot.get<std::wstring>((KEY_REQUEST_ID), L"");
			UC_LOG(ERROR) << "OnBriefMeetingListReceived error, server response failed, error code = " << code << ", requestId = " << sRequestId;
			triggerEvent(ICalendarServiceEvent, OnBriefMeetingListReceived(code, requestId, startTime, endTime, authorizerId, meetings));
			return code;
		}

		boost::property_tree::wptree ptData = ptRoot.get_child(L"data");
		if (!ptData.empty()) {
			boost::property_tree::wptree ptInfo = ptData.get_child(L"eventInfo");
			if (!ptInfo.empty()) {
				BOOST_FOREACH(boost::property_tree::wptree::value_type v, ptInfo){
					Meeting m;
					m.SetEventId(v.second.get<int64_t>(L"eventId", 0));
					m.SetParentId(v.second.get<int64_t>(L"parentId", 0));
					m.SetStartTime(v.second.get<int64_t>(L"startTime", 0));
					m.SetOriTime(v.second.get<int64_t>(L"oriStart", 0));
					int64_t endTime = v.second.get<int64_t>(L"endTime", 0);
					int32_t duration = (endTime - m.GetStartTime()) / 60;
					m.SetDuration(duration);
					meetings.push_back(m);
				}
			}
		}

	}
	catch (boost::property_tree::ptree_error & e) {
		UC_LOG(ERROR) << "encounter an exception when parse JSON from OnBriefMeetingListReceived , exception: " << e.what();
		triggerEvent(ICalendarServiceEvent, OnBriefMeetingListReceived(JSON_PARSE_ERROR, requestId, startTime, endTime, authorizerId, meetings));
		return JSON_PARSE_ERROR;
	}

	SetMeetingAuthorizerId( meetings, authorizerId);

	//回调上层
	triggerEvent(ICalendarServiceEvent, OnBriefMeetingListReceived(code, requestId, startTime, endTime, authorizerId, meetings));

	UC_LOG(INFO) << "OnBriefMeetingListReceived success.";
	return code;
}

int32_t CalendarService::GetBriefChangedMeetingList(int64_t startTime, int32_t authorizerId /*= 0*/) {
	UC_LOG(INFO) << "GetBriefChangedMeetingList enter. startTime = " << startTime << ", authorizerId = " << authorizerId;
	if (authorizerId < 0 || authorizerId == GetUserId()) {
		authorizerId = 0;
	}
	HTTPRequest request;
	_calendarRequestFactory->MakeRequestOfGetBriefChangedMeetingList(startTime, authorizerId, request);

	_engine->SendAsyncRequest(request, MakeRequestCallback(this, &CalendarService::OnBriefChangedMeetingListReceived, authorizerId, startTime));

	UC_LOG(INFO) << "GetBriefChangedMeetingList over. request URL = " << request.GetURL() << ", request body = " << request.GetBody();
	return UC_SUCCEEDED;
}

int32_t CalendarService::OnBriefChangedMeetingListReceived(int32_t errorCode, HTTPResponse* response, int32_t authorizerId, int64_t startTime) {
	
	int32_t result = HTTPResponseAssist::GuardStatementForCallBack("OnBriefChangedMeetingListReceived", errorCode, response);
	UC_LOG(INFO) << "OnBriefChangedMeetingListReceived enter. Http request result = " << result << ", authorizerId = " << authorizerId << ", startTime = " << startTime;
	std::vector<Meeting> meetings;
	if (result != UC_SUCCEEDED) {
		triggerEvent(ICalendarServiceEvent, OnBriefChangedMeetingListReceived(result, startTime, authorizerId, meetings));
		return result;
	}

	//解析json
	int32_t code = -1;
	try {
		wstringstream jsonStream;
		boost::property_tree::wptree ptRoot;
		jsonStream.str(StringConvert::FromUTF8Str(response->GetContent()).c_str());
		boost::property_tree::json_parser::read_json(jsonStream, ptRoot);
		code = ptRoot.get<int32_t>((KEY_CODE), code);
		if (code != 0) {
			std::wstring sRequestId = ptRoot.get<std::wstring>((KEY_REQUEST_ID), L"");
			UC_LOG(ERROR) << "OnBriefChangedMeetingListReceived error, server response failed, error code = " << code << ", requestId = " << sRequestId;
			triggerEvent(ICalendarServiceEvent, OnBriefChangedMeetingListReceived(errorCode, startTime, authorizerId, meetings));
			return code;
		}

		boost::property_tree::wptree ptData = ptRoot.get_child(L"data");
		if (!ptData.empty()) {
			boost::property_tree::wptree ptInfo = ptData.get_child(L"eventInfo");
			if (!ptInfo.empty()) {
				BOOST_FOREACH(boost::property_tree::wptree::value_type v, ptInfo){
					Meeting m;
					m.SetEventId(v.second.get<int64_t>(L"eventId", 0));
					m.SetParentId(v.second.get<int64_t>(L"parentId", 0));
					m.SetStartTime(v.second.get<int64_t>(L"startTime", 0));
					m.SetOriTime(v.second.get<int64_t>(L"oriStart", 0));
					int64_t endTime = v.second.get<int64_t>(L"endTime", 0);
					int32_t duration = (endTime - m.GetStartTime()) / 60;
					m.SetDuration(duration);
					meetings.push_back(m);
				}
			}
		}
	}
	catch (boost::property_tree::ptree_error & e) {
		UC_LOG(ERROR) << "encounter an exception when parse JSON from OnBriefChangedMeetingListReceived , exception: " << e.what();
		triggerEvent(ICalendarServiceEvent, OnBriefChangedMeetingListReceived(JSON_PARSE_ERROR, startTime, authorizerId, meetings));
		return JSON_PARSE_ERROR;
	}

	//更新数据库
	SetMeetingAuthorizerId(meetings, authorizerId);

	//回调上层
	triggerEvent(ICalendarServiceEvent, OnBriefChangedMeetingListReceived(errorCode, startTime, authorizerId, meetings));

	UC_LOG(INFO) << "OnBriefChangedMeetingListReceived success.";
	return code;
}


int32_t CalendarService::GetMeetingInfoById( int64_t eventId, int64_t startTime, int64_t endTime, int32_t authorizerId, OUT vector<Meeting>& meetings ) {
	UC_LOG(INFO) << "GetMeetingInfoById enter. event id = " << eventId << ", start time = " << startTime << ", authorizer id = " << authorizerId;
	int32_t result = -1;
	if (authorizerId < 0 || authorizerId == GetUserId()) {
		authorizerId = 0;
	}
	
	//若eventId有效，优先使用eventId
	std::vector<int64_t> eventIds;	
	if (eventId > 0) {
		eventIds.push_back(eventId);
		result = DBManager::GetInstance().GetMeetingByIds(meetings, eventIds, startTime, endTime, authorizerId);
	}
	
	if (result == 0 && !meetings.empty()) {
		UC_LOG(INFO) << "DB::GetMeetingByIds from DB success.";
		return UC_SUCCEEDED;
	}

	Meeting meeting;
	result = DBManager::GetInstance().GetMeeting(meeting, eventId, startTime, authorizerId);

	if (result == DBError::DB_SUCCESS) {
		meetings.push_back(meeting);
		UC_LOG(INFO) << "DB::GetMeeting from DB success.";
		return UC_SUCCEEDED;
	} 

	if (true == FindInCacheMeeting(eventId, startTime, authorizerId, meeting)) {
		meetings.push_back(meeting);
		UC_LOG(INFO) << "FindInCacheMeeting return true.";
		return UC_SUCCEEDED;
	}

	HTTPRequest request;
	_calendarRequestFactory->MakeRequestOfGetMeetingInfoByIds(eventIds, startTime, endTime, authorizerId, request);
	
	_engine->SendAsyncRequest(request, MakeRequestCallback(this, &CalendarService::OnMeetingInfoReceivedById, eventId, startTime, authorizerId));
	
	UC_LOG(INFO) << "GetMeetingInfoById over. request URL = " << request.GetURL() << ", request body = " << request.GetBody();
	return UC_SUCCEEDED;
}

int32_t CalendarService::OnMeetingInfoReceivedById(int32_t errorCode, HTTPResponse* response, int64_t eventId, int64_t startTime, int32_t authorizerId) {
	
	int32_t result = HTTPResponseAssist::GuardStatementForCallBack("OnMeetingInfoReceivedById", errorCode, response);
	UC_LOG(INFO) << "OnMeetingInfoReceivedById enter. Http request result = " << result << "eventId:" << eventId << ", startTime =" << startTime<< ", authorizerId = " << authorizerId;
	std::vector<Meeting> meetings;
	if (result != UC_SUCCEEDED) {
		triggerEvent(ICalendarServiceEvent, OnMeetingInfoReceivedById(result, eventId, authorizerId, meetings));
		return result;
	}
	
	//解析json
	//{"code":"0","data":"BEGIN:VCALENDAR\r\nX-WR-CALNAME;VALUE=TEXT:\r\nPRODID:-\/\/WebCalendar-ics-v1.2.7\r\nVERSION:2.0\r\nMETHOD:PUBLISH\r\nBEGIN:VEVENT\r\nUID:-TESTCLOUD-QUANSHI-COM-WEBCALENDAR-1384668-0000133403\r\nLAST-MODIFIED:20150529T061801Z\r\nSUMMARY:%E8%B0%AD%E5%9B%BD%E8%81%94%E7%9A%84%E4%BC%9A%E8%AE%AE\r\nDESCRIPTION:\r\nLOCATION:\r\nCLASS:PUBLIC\r\nSTATUS:TENTATIVE\r\nATTENDEE;ROLE=REQ-PARTICIPANT;PARTSTAT=NEEDS-ACTION;CN=\"1189278\":MAILTO:your\r\n emailhere\r\nATTENDEE;ROLE=REQ-PARTICIPANT;PARTSTAT=NEEDS-ACTION;CN=\"1207238\":MAILTO:your\r\n emailhere\r\nATTENDEE;ROLE=REQ-PARTICIPANT;PARTSTAT=NEEDS-ACTION;CN=\"1228636\":MAILTO:your\r\n emailhere\r\nATTENDEE;ROLE=REQ-PARTICIPANT;PARTSTAT=NEEDS-ACTION;CN=\"1228638\":MAILTO:your\r\n emailhere\r\nATTENDEE;ROLE=REQ-PARTICIPANT;PARTSTAT=NEEDS-ACTION;CN=\"1228650\":MAILTO:your\r\n emailhere\r\nATTENDEE;ROLE=REQ-PARTICIPANT;PARTSTAT=NEEDS-ACTION;CN=\"1331346\":MAILTO:your\r\n emailhere\r\nATTENDEE;ROLE=REQ-PARTICIPANT;PARTSTAT=NEEDS-ACTION;CN=\"1382748\":MAILTO:your\r\n emailhere\r\nATTENDEE;ROLE=REQ-PARTICIPANT;PARTSTAT=NEEDS-ACTION;CN=\"1382752\":MAILTO:your\r\n emailhere\r\nATTENDEE;ROLE=CHAIR;PARTSTAT=ACCEPTED;CN=\"1382754\":MAILTO:youremailhere\r\nATTENDEE;ROLE=REQ-PARTICIPANT;PARTSTAT=NEEDS-ACTION;CN=\"1382758\":MAILTO:your\r\n emailhere\r\nATTENDEE;ROLE=REQ-PARTICIPANT;PARTSTAT=NEEDS-ACTION;CN=\"1382760\":MAILTO:your\r\n emailhere\r\nATTENDEE;ROLE=REQ-PARTICIPANT;PARTSTAT=NEEDS-ACTION;CN=\"1382762\":MAILTO:your\r\n emailhere\r\nATTENDEE;ROLE=REQ-PARTICIPANT;PARTSTAT=NEEDS-ACTION;CN=\"1382766\":MAILTO:your\r\n emailhere\r\nATTENDEE;ROLE=REQ-PARTICIPANT;PARTSTAT=NEEDS-ACTION;CN=\"1384650\":MAILTO:your\r\n emailhere\r\nATTENDEE;ROLE=REQ-PARTICIPANT;PARTSTAT=NEEDS-ACTION;CN=\"1384652\":MAILTO:your\r\n emailhere\r\nATTENDEE;ROLE=REQ-PARTICIPANT;PARTSTAT=NEEDS-ACTION;CN=\"1384660\":MAILTO:your\r\n emailhere\r\nATTENDEE;ROLE=REQ-PARTICIPANT;PARTSTAT=NEEDS-ACTION;CN=\"1384662\":MAILTO:your\r\n emailhere\r\nATTENDEE;ROLE=REQ-PARTICIPANT;PARTSTAT=NEEDS-ACTION;CN=\"1384670\":MAILTO:your\r\n emailhere\r\nATTENDEE;ROLE=REQ-PARTICIPANT;PARTSTAT=NEEDS-ACTION;CN=\"1384672\":MAILTO:your\r\n emailhere\r\nATTENDEE;ROLE=REQ-PARTICIPANT;PARTSTAT=DECLINED;CN=\"1767\":MAILTO:youremailhe\r\n re\r\nATTENDEE;ROLE=REQ-PARTICIPANT;PARTSTAT=ACCEPTED;CN=\"203178\":MAILTO:youremail\r\n here\r\nATTENDEE;ROLE=REQ-PARTICIPANT;PARTSTAT=NEEDS-ACTION;CN=\"209518\":MAILTO:youre\r\n mailhere\r\nATTENDEE;ROLE=REQ-PARTICIPANT;PARTSTAT=NEEDS-ACTION;CN=\"217705\":MAILTO:youre\r\n mailhere\r\nATTENDEE;ROLE=REQ-PARTICIPANT;PARTSTAT=NEEDS-ACTION;CN=\"366197\":MAILTO:youre\r\n mailhere\r\nATTENDEE;ROLE=REQ-PARTICIPANT;PARTSTAT=NEEDS-ACTION;CN=\"389548\":MAILTO:youre\r\n mailhere\r\nATTENDEE;ROLE=REQ-PARTICIPANT;PARTSTAT=NEEDS-ACTION;CN=\"390144\":MAILTO:youre\r\n mailhere\r\nATTENDEE;ROLE=REQ-PARTICIPANT;PARTSTAT=NEEDS-ACTION;CN=\"401604\":MAILTO:youre\r\n mailhere\r\nATTENDEE;ROLE=REQ-PARTICIPANT;PARTSTAT=NEEDS-ACTION;CN=\"476518\":MAILTO:youre\r\n mailhere\r\nDTSTART:20150529T063000Z\r\nDTSTAMP:20150611T034102Z\r\nDTEND:20150529T073000Z\r\nX-MOZILLA-ALARM-DEFAULT-LENGTH:15\r\nBEGIN:VALARM\r\nTRIGGER;VALUE=DATE-TIME:20150529T073000Z\r\nACTION:EMAIL\r\nDESCRIPTION:\r\nEND:VALARM\r\nX-WR-EVENTID:133403\r\nX-WR-CONFERENCEID:419379258\r\nX-WR-ISGNET:0\r\nX-WR-HOSTNAME:\u8c2d\u56fd\u8054\r\nX-WR-HOSTID:1382754\r\nX-WR-PCODE1:336705262947133688\r\nX-WR-PCODE2:759925193083650697\r\nX-WR-GROUP-ID:20001655\r\nX-WR-BIllINGCODE:60461685\r\nEND:VEVENT\r\nEND:VCALENDAR\r\n","webcal_requestid":"1433994062.33287500.13873","request_id":"1433994062.17795700.14539"}
	int32_t code = -1;
	try {
		wstringstream jsonStream;
		boost::property_tree::wptree ptRoot;
		jsonStream.str(StringConvert::FromUTF8Str(response->GetContent()).c_str());
		boost::property_tree::json_parser::read_json(jsonStream, ptRoot);
		code = ptRoot.get<int32_t>((KEY_CODE), code);
		if (code != 0) {
			std::wstring requestId = ptRoot.get<std::wstring>((KEY_REQUEST_ID), L"");
			UC_LOG(ERROR) << "OnMeetingInfoReceivedById error, server response failed, error code = " << code << ", requestId = " << requestId;
			triggerEvent(ICalendarServiceEvent, OnMeetingInfoReceivedById(code, eventId, authorizerId, meetings));
			return code;
		}
		//icalendar待解析
		std::wstring icalendar = ptRoot.get<std::wstring>((KEY_DATA), L"");
		ParseICalendarStr(StringConvert::ToUTF8Str(icalendar), meetings);
	}
	catch(boost::property_tree::ptree_error & e) {
		UC_LOG(ERROR) << "encounter an exception when parse JSON from OnMeetingInfoReceivedById , exception: " << e.what();
		triggerEvent(ICalendarServiceEvent, OnMeetingInfoReceivedById(JSON_PARSE_ERROR, eventId, authorizerId, meetings));
		return JSON_PARSE_ERROR;
	}

	//缓存该接口返回的数据
	if (meetings.size() >0) {
		SetMeetingAuthorizerId(meetings, authorizerId);
		CacheMeetings(meetings);
	}

	//回调上层
	triggerEvent(ICalendarServiceEvent, OnMeetingInfoReceivedById(code, eventId, authorizerId, meetings));
	
	UC_LOG(INFO) << "OnMeetingInfoReceivedById success.";
	return code;
}

int32_t CalendarService::GetMeetingInfoByIds(const std::vector<int64_t>& eventIds, int64_t startTime, int64_t endTime, int32_t authorizerId, int32_t& requestId, vector<Meeting>& meetings) {
	UC_LOG(INFO) << "GetMeetingInfoByIds enter.";
	requestId = MakeRequestId();
	if (authorizerId < 0 || authorizerId == GetUserId()) {
		authorizerId = 0;
	}
	if (eventIds.empty()) {
		return UC_FAILED;
	}

	int32_t ret = DBManager::GetInstance().GetMeetingByIds(meetings, eventIds, startTime, endTime, authorizerId);
	if (ret == 0) {
		//只有从数据库取到全部数据的情况下直接返回，否则从server请求数据
		if (eventIds.size() <= meetings.size()) {
			UC_LOG(INFO) << "GetMeetingInfoByIds from DB success.";
			return UC_SUCCEEDED;
		}
	}

	//全部从server重新请求
	meetings.clear();
	HTTPRequest request;
	_calendarRequestFactory->MakeRequestOfGetMeetingInfoByIds(eventIds, startTime, endTime, authorizerId, request);
	_engine->SendAsyncRequest(request, MakeRequestCallback(this, &CalendarService::OnMeetingInfoReceivedByIds, requestId, authorizerId));
	
	UC_LOG(INFO) << "GetMeetingInfoByIds over. request URL = " << request.GetURL() << ", request body = " << request.GetBody();
	return UC_SUCCEEDED;
}

int32_t CalendarService::OnMeetingInfoReceivedByIds( int32_t errorCode, HTTPResponse* response, int32_t requestId, int32_t authorizerId) {
	int32_t result = HTTPResponseAssist::GuardStatementForCallBack("OnMeetingInfoReceivedByIds", errorCode, response);
	UC_LOG(INFO) << "OnMeetingInfoReceivedByIds enter. Http request result = " << result << ", requestId = " << requestId << ", authorizerId = " << authorizerId;
	std::vector<Meeting> meetings;
	if (result != UC_SUCCEEDED) {
		triggerEvent(ICalendarServiceEvent, OnMeetingInfoReceivedByIds(result, requestId, authorizerId, meetings));
		return result;
	}

	//解析json
	int32_t code = -1;
	try {
		wstringstream jsonStream;
		boost::property_tree::wptree ptRoot;
		jsonStream.str(StringConvert::FromUTF8Str(response->GetContent()).c_str());
		boost::property_tree::json_parser::read_json(jsonStream, ptRoot);
		code = ptRoot.get<int32_t>((KEY_CODE), code);
		if (code != 0) {
			UC_LOG(ERROR) << "OnMeetingInfoReceivedByIds error, server response failed, error code = " << code;
			triggerEvent(ICalendarServiceEvent, OnMeetingInfoReceivedByIds(code, requestId, authorizerId, meetings));
			return code;
		}
		//icalendar待解析
		std::wstring icalendar = ptRoot.get<std::wstring>(KEY_DATA, L"");
		ParseICalendarStr(StringConvert::ToUTF8Str(icalendar), meetings);
	}
	catch(boost::property_tree::ptree_error & e) {
		UC_LOG(ERROR) << "encounter an exception when parse JSON from OnMeetingInfoReceivedByIds , exception: " << e.what();
		triggerEvent(ICalendarServiceEvent, OnMeetingInfoReceivedByIds(JSON_PARSE_ERROR, requestId, authorizerId, meetings));
		return JSON_PARSE_ERROR;
	}
	
	//缓存该接口返回的数据
	int32_t size = meetings.size();
	if (size > 0) {
		SetMeetingAuthorizerId(meetings, authorizerId);
		CacheMeetings(meetings);
	}

	//回调上层
	triggerEvent(ICalendarServiceEvent, OnMeetingInfoReceivedByIds(code, requestId, authorizerId, meetings));
	
	UC_LOG(INFO) << "OnMeetingInfoReceivedByIds success.";
	return code;
}

int32_t CalendarService::GetUserSchedule( const std::vector<int32_t>& userIdList, int64_t startTime, int64_t endTime, int32_t timeInterval, int64_t eventIdFilter, int32_t& requestId) {
	UC_LOG(INFO) << "GetUserSchedule enter.";
	timeInterval = timeInterval <= 0 ? 1 : timeInterval;
	requestId = MakeRequestId();
	HTTPRequest request;
	_calendarRequestFactory->MakeRequestOfGetUserSchedule(userIdList, startTime, endTime, timeInterval, eventIdFilter, request);
	_engine->SendAsyncRequest(request, MakeRequestCallback(this, &CalendarService::OnUserScheduleReceived, startTime, endTime, timeInterval, eventIdFilter, userIdList, requestId));
	UC_LOG(INFO) << "GetUserSchedule over. request URL = " << request.GetURL() << ", request body = " << request.GetBody();
	return UC_SUCCEEDED;
}

int32_t CalendarService::OnUserScheduleReceived( int32_t errorCode, HTTPResponse* response, int64_t startTime, int64_t endTime, int32_t timeInterval, int64_t eventIdFilter, std::vector<int32_t> userIdList, int32_t requestId ) {
	int32_t result = HTTPResponseAssist::GuardStatementForCallBack("OnUserScheduleReceived", errorCode, response);
	UC_LOG(INFO) << "OnUserScheduleReceived enter. Http request result = " << result << ", startTime = " << startTime << ", endTime = " << endTime;
	std::vector<UserSchedule> vUserSchedule;
	if (result != UC_SUCCEEDED) {
		triggerEvent(ICalendarServiceEvent, OnUserScheduleReceived(result, requestId, vUserSchedule));
		return result;
	}

	//解析json
	int32_t code = -1;
	try {
		wstringstream jsonStream;
		boost::property_tree::wptree ptRoot;
		jsonStream.str(StringConvert::FromUTF8Str(response->GetContent()).c_str());
		boost::property_tree::json_parser::read_json(jsonStream, ptRoot);
		code = ptRoot.get<int32_t>((KEY_CODE), code);
		if (code != 0) {
			std::wstring request = ptRoot.get<std::wstring>((KEY_REQUEST_ID), L"");
			UC_LOG(ERROR) << "OnUserScheduleReceived error, server response failed, error code = " << code << ", requestId = " << request;
			triggerEvent(ICalendarServiceEvent, OnUserScheduleReceived(code, requestId, vUserSchedule));
			return code;
		}

		boost::property_tree::wptree ptData;
		ptData = ptRoot.get_child(L"data");
		if (ptData.empty()) {
			UC_LOG(INFO) << "OnUserScheduleReceived server response data is null. ";
			triggerEvent(ICalendarServiceEvent, OnUserScheduleReceived(0, requestId, vUserSchedule));
			return 0;
		}

		UserSchedule schedule;
		wstringstream wsStream;
		for (unsigned int i=0; i<userIdList.size(); ++i) {
			schedule.SetUserId(userIdList[i]);
			wsStream.str(L"");
			wsStream.clear();
			wsStream << userIdList[i];
			std::wstring userId(wsStream.str());
			boost::property_tree::wptree pdef;
			boost::property_tree::wptree ptUser = ptData.get_child(userId, pdef);

			//该user data为空，代表请求时间内空闲
			if (ptUser.empty()) {
				//vUserSchedule.push_back(schedule);
				continue;
			}
		
			//server返回的时间间隔为1分钟
			std::vector<int32_t> times;
			int32_t endPoint = (int32_t)((endTime - startTime) / 60) + 1;
			for (int32_t startPoint = 0; startPoint<endPoint; startPoint++) {
				wsStream.str(L"");
				wsStream.clear();
				wsStream << startPoint;
				std::wstring time(wsStream.str());
				std::wstring value = ptUser.get<std::wstring>(time, L"");
				// A -- 有已接受会议， W--有待处理会议
				if (value.compare(L"A") == 0 || value.compare(L"W") == 0) {
					times.push_back(startPoint);		//保存忙碌状态时间点
				}
			}
			
			//将忙碌时间点合并成时间段
			vector<TimeSlot> timeSlots;
			MergeBusyTime(startTime, endTime, timeInterval, times, timeSlots);
			schedule.SetBusyTimes(timeSlots);
			vUserSchedule.push_back(schedule);
		}
	}
	catch(boost::property_tree::ptree_error & e) {
		UC_LOG(ERROR) << "encounter an exception when parse JSON from OnUserScheduleReceived , exception: " << e.what();
		triggerEvent(ICalendarServiceEvent, OnUserScheduleReceived(JSON_PARSE_ERROR, requestId, vUserSchedule));
		return JSON_PARSE_ERROR;
	}

	//回调上层
	triggerEvent(ICalendarServiceEvent, OnUserScheduleReceived(code, requestId, vUserSchedule));
	UC_LOG(INFO) << L"OnUserScheduleReceived success.";
	return code;
}


int32_t CalendarService::CreateMinutes(int64_t eventId, int64_t startTime, const std::string& title, const std::string& minutes) {
	UC_LOG(INFO) << "CreateMinutes eventId:" << eventId;
	HTTPRequest request;
	_calendarRequestFactory->MakeRequestOfCreateMinutes(eventId, startTime, title, minutes, request);
	_engine->SendAsyncRequest(request, MakeRequestCallback(this, &CalendarService::OnMinutesCreated, eventId, startTime, title));
	UC_LOG(INFO) << "CreateMinutes over. request URL = " << request.GetURL() << ", request body = " << request.GetBody();
	return UC_SUCCEEDED;
}


int32_t CalendarService::OnMinutesCreated(int32_t errorCode, HTTPResponse* response, int64_t eventId, int64_t startTime, std::string title) {
	int32_t result = HTTPResponseAssist::GuardStatementForCallBack("OnMinutesCreated", errorCode, response);
	UC_LOG(INFO) << "OnMinutesCreated enter. Http request result = " << result << ", eventId = " << eventId << ", startTime = " << startTime;
	int64_t minutesId = 0;
	if (result != UC_SUCCEEDED) {
		triggerEvent(ICalendarServiceEvent, OnMinutesCreated(result, eventId, startTime, minutesId, title));
		return result;
	}

	//解析json
	//{"code":0,"data":{"summaryId":1047,"conferenceId":425510740}}
	int32_t code = -1;
	try {
		wstringstream jsonStream;
		boost::property_tree::wptree ptRoot;
		jsonStream.str(StringConvert::FromUTF8Str(response->GetContent()).c_str());
		boost::property_tree::json_parser::read_json(jsonStream, ptRoot);
		code = ptRoot.get<int32_t>((KEY_CODE), code);
		if (code != 0) {
			UC_LOG(ERROR) << "OnMinutesCreated error, server response failed, error code = " << code ;
			triggerEvent(ICalendarServiceEvent, OnMinutesCreated(code, eventId, startTime, minutesId, title));
			return code;
		}

		boost::property_tree::wptree ptChild = ptRoot.get_child((KEY_DATA));
		if (ptChild.empty()) {
			UC_LOG(ERROR) << "OnMinutesCreated error, server response data is null";
			triggerEvent(ICalendarServiceEvent, OnMinutesCreated(code, eventId, startTime, minutesId, title));
			return code;
		}

		minutesId = ptChild.get<int64_t>((L"summaryId"), 0);
	}
	catch(boost::property_tree::ptree_error & e) {
		UC_LOG(ERROR) << "encounter an exception when parse JSON from OnMinutesCreated , exception: " << e.what();
		triggerEvent(ICalendarServiceEvent, OnMinutesCreated(JSON_PARSE_ERROR, eventId, startTime, minutesId, title));
		return JSON_PARSE_ERROR;
	}

	//回调上层
	triggerEvent(ICalendarServiceEvent, OnMinutesCreated(code, eventId, startTime, minutesId, title));
	UC_LOG(INFO) << "OnMinutesCreated success.";
	return code;
}

int32_t CalendarService::UpdateMinutes(int64_t eventId, int64_t minutesId, const std::string& title, const std::string& minutes) {
	UC_LOG(INFO) << "UpdateMinutes enter. minutesId = " << minutesId << ", eventId = " << eventId;
	HTTPRequest request;
	_calendarRequestFactory->MakeRequestOfUpdateMinutes(minutesId, title, minutes, request);
	_engine->SendAsyncRequest(request, MakeRequestCallback(this, &CalendarService::OnMinutesUpdated, eventId, minutesId, title));
	UC_LOG(INFO) << "UpdateMinutes over. request URL = " << request.GetURL() << ", request body = " << request.GetBody();
	return UC_SUCCEEDED;
}

int32_t CalendarService::OnMinutesUpdated(int32_t errorCode, HTTPResponse* response, int64_t eventId, int64_t minutesId, std::string title) {
	int32_t result = HTTPResponseAssist::GuardStatementForCallBack("OnMinutesUpdated", errorCode, response);
	UC_LOG(INFO) << "OnMinutesUpdated enter. Http request result = " << result << ", minutesId = " << minutesId << ", eventId = " << eventId;

	if (result != UC_SUCCEEDED) {
		triggerEvent(ICalendarServiceEvent, OnMinutesUpdated(result, eventId, minutesId, title));
		return result;
	}
	//解析json
	//{"code":0,"data":""}
	int32_t code = 0;
	try {
		wstringstream jsonStream;
		boost::property_tree::wptree ptRoot;
		jsonStream.str(StringConvert::FromUTF8Str(response->GetContent()).c_str());
		boost::property_tree::json_parser::read_json(jsonStream, ptRoot);
		code = ptRoot.get<int32_t>((KEY_CODE), code);
		if (code != 0) {
			UC_LOG(ERROR) << "OnMinutesUpdated error, server response failed, error code = " << code ;
		}
	}
	catch(boost::property_tree::ptree_error & e) {
		UC_LOG(ERROR) << "encounter an exception when parse JSON from OnMinutesUpdated , exception: " << e.what();
		triggerEvent(ICalendarServiceEvent, OnMinutesUpdated(JSON_PARSE_ERROR, eventId, minutesId, title));
		return JSON_PARSE_ERROR;
	}
	//回调上层
	triggerEvent(ICalendarServiceEvent, OnMinutesUpdated(code, eventId, minutesId, title));
	UC_LOG(INFO) << "OnMinutesUpdated success.";
	return code;
}


int32_t CalendarService::GetMinutesDetails(int64_t eventId, int64_t minutesId) {
	UC_LOG(INFO) << "GetMinutesDetails enter. minutesId = " << minutesId << ", eventId = " << eventId;
	HTTPRequest request;
	_calendarRequestFactory->MakeRequestOfGetMinutesDetails(minutesId, request);
	_engine->SendAsyncRequest(request, MakeRequestCallback(this, &CalendarService::OnMinutesDetailsReceived, eventId, minutesId));
	UC_LOG(INFO) << "GetMinutesDetails over. request URL = " << request.GetURL() << ", request body = " << request.GetBody();
	return UC_SUCCEEDED;
}

int32_t CalendarService::OnMinutesDetailsReceived(int32_t errorCode, HTTPResponse* response, int64_t eventId, int64_t minutesId) {
	int32_t result = HTTPResponseAssist::GuardStatementForCallBack("OnMinutesDetailsReceived", errorCode, response);
	UC_LOG(INFO) << "OnMinutesDetailsReceived enter. Http request result = " << result << ", minutesId = " << minutesId << ", eventId = " << eventId;
	Minutes details;
	if (result != UC_SUCCEEDED) {
		triggerEvent(ICalendarServiceEvent, OnMinutesDetailsReceived(result, eventId, minutesId, details));
		return result;
	}

	//解析json
	int32_t code = -1;
	try {
		wstringstream jsonStream;
		boost::property_tree::wptree ptRoot;
		jsonStream.str(StringConvert::FromUTF8Str(response->GetContent()).c_str());
		boost::property_tree::json_parser::read_json(jsonStream, ptRoot);
		code = ptRoot.get<int32_t>((KEY_CODE), code);
		if (code != 0) {
			UC_LOG(ERROR) << "OnMinutesDetailsReceived error, server response failed, error code = " << code ;
			triggerEvent(ICalendarServiceEvent, OnMinutesDetailsReceived(code, eventId, minutesId, details));
			return code;
		}
		boost::property_tree::wptree ptChild = ptRoot.get_child(KEY_DATA);
		if (ptChild.empty()) {
			UC_LOG(ERROR) << "OnMinutesDetailsReceived error, server response data is null ";
			triggerEvent(ICalendarServiceEvent, OnMinutesDetailsReceived(code, eventId, minutesId, details));
			return code;
		}

		details.SetCreatorId(ptChild.get<int32_t>((KEY_CREATOR), 0));
		details.SetStartTime(ptChild.get<int64_t>((KEY_STARTTIME), 0));
		details.SetCreateTime(ptChild.get<int32_t>((KEY_CREATETIME), 0));
		details.SetUpdateTime(ptChild.get<int32_t>((KEY_UPDATETIME), 0));
		details.SetContent(StringConvert::ToUTF8Str(ptChild.get<std::wstring>((KEY_SUMMARY), L"")));
		details.SetTitle(StringConvert::ToUTF8Str(ptChild.get<std::wstring>((KEY_SUMMARYTITLE), L"")));
	}
	catch(boost::property_tree::ptree_error & e) {
		UC_LOG(ERROR) << "encounter an exception when parse JSON from OnMinutesDetailsReceived , exception: " << e.what();
		triggerEvent(ICalendarServiceEvent, OnMinutesDetailsReceived(JSON_PARSE_ERROR, eventId, minutesId, details));
		return JSON_PARSE_ERROR;
	}
	//回调上层
	triggerEvent(ICalendarServiceEvent, OnMinutesDetailsReceived(code, eventId, minutesId, details));
	
	UC_LOG(INFO) << "OnMinutesDetailsReceived success.";
	return code;
}

int32_t CalendarService::GetMinutesList(int64_t eventId, int64_t startTime/* = 0*/) {
	UC_LOG(INFO) << "GetMinutesList enter. event id = " << eventId;
	HTTPRequest request;
	_calendarRequestFactory->MakeRequestOfGetMinutesList(eventId, startTime, request);
	_engine->SendAsyncRequest(request, MakeRequestCallback(this, &CalendarService::OnMinutesListReceived, eventId, startTime));

	UC_LOG(INFO) << "GetMinutesList over. request URL = " << request.GetURL() << ", request body = " << request.GetBody();
	return UC_SUCCEEDED;
}


int32_t CalendarService::OnMinutesListReceived( int32_t errorCode, HTTPResponse* response, int64_t eventId, int64_t startTime) {
	int32_t result = HTTPResponseAssist::GuardStatementForCallBack("OnMinutesListReceived", errorCode, response);
	UC_LOG(INFO) << "OnMinutesListReceived enter. Http request result = " << result << ", eventId = " << eventId << ", startTime = " << startTime;

	std::vector<Minutes> infos;
	if (result != UC_SUCCEEDED) {
		triggerEvent(ICalendarServiceEvent, OnMinutesListReceived(result, eventId, startTime, infos));
		return result;
	}

	//解析json
	int32_t code  = -1;
	try {
		wstringstream jsonStream;
		boost::property_tree::wptree ptRoot;
		jsonStream.str(StringConvert::FromUTF8Str(response->GetContent()).c_str());
		boost::property_tree::json_parser::read_json(jsonStream, ptRoot);
		code = ptRoot.get<int32_t>((KEY_CODE), code);
		if (code != 0) {
			UC_LOG(ERROR) << "OnMinutesListReceived error, server response failed, error code = " << code ;
			triggerEvent(ICalendarServiceEvent, OnMinutesListReceived(code, eventId, startTime, infos));
			return code;
		}

		boost::property_tree::wptree ptChild = ptRoot.get_child(KEY_DATA);
		if (ptChild.empty()) {
			UC_LOG(ERROR) << "OnMinutesListReceived error, server response data is null";
			triggerEvent(ICalendarServiceEvent, OnMinutesListReceived(code, eventId, startTime, infos));
			return code;
		}
		
		BOOST_FOREACH(const boost::property_tree::wptree::value_type &vElement, ptChild){
			Minutes info;
			info.SetEventId(eventId);
			boost::property_tree::wptree jsonValue = vElement.second;
			info.SetMinutesId(jsonValue.get<int64_t>((KEY_SUMMARYID), 0));
			info.SetConferenceId(StringConvert::ToUTF8Str(jsonValue.get<std::wstring>((L"conferenceId"), L"")));
			info.SetCreatorId(jsonValue.get<int32_t>((KEY_CREATOR), 0));
			info.SetCreateTime(jsonValue.get<int64_t>((KEY_CREATETIME), 0));
			info.SetUpdateTime(jsonValue.get<int64_t>((KEY_UPDATETIME), 0));
			info.SetTitle(StringConvert::ToUTF8Str(jsonValue.get<std::wstring>((KEY_SUMMARYTITLE), L"")));		
			info.SetType(jsonValue.get<int32_t>((KEY_SUMMARYTYPE), 0));
			
			int32_t ntype = info.GetType(); //0--会议纪要，1--会议录制，2--会议文档
			if (ntype > 0) {
				boost::property_tree::wptree ptChildExtras = jsonValue.get_child(KEY_EXTRASTINFO);
				if (ptChildExtras.empty()) {
					UC_LOG(ERROR) << "OnMinutesListReceived error, server response extras info is null. summary id = " << info.GetMinutesId();
				}
				else{
					if (ntype == 1) {
						MeetingRecording cd;
						cd.SetVideoURL(StringConvert::ToUTF8Str(ptChildExtras.get<std::wstring>((KEY_VIDEOSTREAMURL), L"")));
						cd.SetVideoName(info.GetTitle());
						cd.SetVideoThumb(StringConvert::ToUTF8Str(ptChildExtras.get<std::wstring>((KEY_VIDEOTHUMBNAIL), L"")));
						cd.SetVideoThumbURL(StringConvert::ToUTF8Str(ptChildExtras.get<std::wstring>((KEY_VIDEOTHUMB), L"")));
						cd.SetOperatorId(ptChildExtras.get<int32_t>((KEY_OPERATORID), 0));
						cd.SetOperatorName(StringConvert::ToUTF8Str(ptChildExtras.get<std::wstring>((KEY_OPERATORNAME), L"")));
						cd.SetOperatorAvatar(StringConvert::ToUTF8Str(ptChildExtras.get<std::wstring>((KEY_OPERATORAVATAR), L"")));
						cd.SetStartTime(ptChildExtras.get<int64_t>((KEY_RECORDSTARTTIME), 0));
						cd.SetEndTime(ptChildExtras.get<int64_t>((KEY_RECORDENDTIME), 0));
						cd.SetLength(ptChildExtras.get<int64_t>((KEY_VIDEOLENGTH), 0));
						cd.SetSize(ptChildExtras.get<int32_t>((KEY_VIDEOSIZE), 0));
						info.SetRecording(cd);
					}
					else if (ntype == 2) {
						DocumentInfo docInfo;
						docInfo.SetEventId(eventId);
						docInfo.SetFileId(info.GetMinutesId());
						docInfo.SetCreatorId(ptChildExtras.get<int32_t>(L"user_id"));
						docInfo.SetCreatorName(StringConvert::ToUTF8Str(ptChildExtras.get<wstring>(L"user_name", L"")));
						docInfo.SetFileName(StringConvert::ToUTF8Str(ptChildExtras.get<wstring>(L"title", L"")));
						docInfo.SetFileFormat(StringConvert::ToUTF8Str(ptChildExtras.get<wstring>(L"format", L"")));
						std::wstring url = ptChildExtras.get<wstring>(L"file_url", L"");
						docInfo.SetFileUrl(StringConvert::ToUTF8Str(url));
						docInfo.SetPreview(StringConvert::ToUTF8Str(ptChildExtras.get<wstring>(L"preview", L"")));
						docInfo.SetFileSize(ptChildExtras.get<int64_t>(L"size", 0));				
						docInfo.SetCloudType(DocumentCloudType(ptChildExtras.get<int32_t>(L"cloud_type", CT_FILE_TRANSFER)));

						if (docInfo.GetCloudType() != CT_FILE_TRANSFER) {
							docInfo.SetHashCode(StringConvert::ToUTF8Str(ptChildExtras.get<wstring>(L"hash_code", L"")));
							docInfo.SetMountId(ptChildExtras.get<int32_t>(L"mount_id", 0));
							docInfo.SetDetailType(ptChildExtras.get<int16_t>(L"‘detail_type", 0));
							docInfo.SetDetailContent(StringConvert::ToUTF8Str(ptChildExtras.get<wstring>(L"detail_content", L"")));
						}

						info.SetDocumentInfo(docInfo);
					}

				}
			}
			infos.push_back(info);
		}
	}
	catch(boost::property_tree::ptree_error & e) {
		UC_LOG(ERROR) << "encounter an exception when parse JSON from OnMinutesListReceived , exception: " << e.what();
		triggerEvent(ICalendarServiceEvent, OnMinutesListReceived(JSON_PARSE_ERROR, eventId, startTime, infos));
		return JSON_PARSE_ERROR;
	}

	//回调上层
	triggerEvent(ICalendarServiceEvent, OnMinutesListReceived(code, eventId, startTime, infos));

	UC_LOG(INFO) << "OnMinutesListReceived success.";
	return code;
}


int32_t CalendarService::DeleteMeetingRecording(int64_t eventId, const std::string& confId, int64_t startTime /*= 0*/) {
	UC_LOG(INFO) << "DeleteMeetingRecording enter. event id = " << eventId << ", startTime = " << startTime;
	HTTPRequest request;
	_calendarRequestFactory->MakeRequestOfDeleteMeetingRecording(confId, eventId, startTime, request);
	_engine->SendAsyncRequest(request, MakeRequestCallback(this, &CalendarService::OnMeetingRecordingDeleted, eventId, startTime));

	UC_LOG(INFO) << "DeleteMeetingRecording over. request URL = " << request.GetURL() << ", request body = " << request.GetBody();
	return UC_SUCCEEDED;
}

int32_t CalendarService::OnMeetingRecordingDeleted(int32_t errorCode, HTTPResponse* response, int64_t eventId, int64_t startTime) {
	int32_t result = HTTPResponseAssist::GuardStatementForCallBack("OnMeetingRecordingDeleted", errorCode, response);
	UC_LOG(INFO) << "OnMeetingRecordingDeleted enter. Http request result = " << result << ", eventId:" << eventId << ", startTime = " << startTime;

	if (result != UC_SUCCEEDED) {
		triggerEvent(ICalendarServiceEvent, OnMeetingRecordingDeleted(result, eventId, startTime));
		return result;
	}
	
	//解析json
	int32_t code = -1;
	try {
		wstringstream jsonStream;
		boost::property_tree::wptree ptRoot;
		jsonStream.str(StringConvert::FromUTF8Str(response->GetContent()).c_str());
		boost::property_tree::json_parser::read_json(jsonStream, ptRoot);
		code = ptRoot.get<int32_t>((KEY_CODE), code);
		if (code != 0) {
			UC_LOG(ERROR) << "OnMeetingRecordingDeleted error, server response failed, error code = " << code;
			triggerEvent(ICalendarServiceEvent, OnMeetingRecordingDeleted(code, eventId, startTime));
			return code;
		}
	}
	catch (boost::property_tree::ptree_error & e) {
		UC_LOG(ERROR) << "encounter an exception when parse JSON from OnMeetingRecordingDeleted , exception: " << e.what();
		triggerEvent(ICalendarServiceEvent, OnMeetingRecordingDeleted(JSON_PARSE_ERROR, eventId, startTime));
		return JSON_PARSE_ERROR;
	}

	//回调上层
	triggerEvent(ICalendarServiceEvent, OnMeetingRecordingDeleted(code, eventId, startTime));

	UC_LOG(INFO) << "OnMeetingRecordingDeleted success.";
	return code;
}


int32_t CalendarService::SetMeetingAlarm(int32_t alarmStatus, int32_t alarmTime /*= 900*/) {
	UC_LOG(INFO) << "SetMeetingAlarm enter, alarm status =" << alarmStatus << ", alarmTime = " << alarmTime;
	if (_alarmId < 0 && alarmStatus > 0) {
		Timer::GetInstance().Schedule(MakeCallback(this, &CalendarService::OnMeetingAlarmed), TIMER_MEETING_ALARM_DELAY, TIMER_MEETING_ALARM_ELAPSE, &_alarmId);
	}
	else if (_alarmId >= 0 && alarmStatus <= 0) {
		Timer::GetInstance().Cancel(_alarmId);
		_alarmId = -1;
	}

	_alarmTime = alarmTime < 60 ? 60 : alarmTime;
	return UC_SUCCEEDED;
}

void CalendarService::OnMeetingAlarmed() {
	int64_t currentUTC = GetCurrentUnixTime();
	int64_t endTime = currentUTC + _alarmTime + 30;
	vector<Meeting> meetings;
	DBManager::GetInstance().GetMeetingList(meetings, currentUTC, endTime, 200, 0, 0, true);
	std::vector<Meeting>::iterator it = meetings.begin();
	while (it != meetings.end()) {
		int64_t timeElapse = it->GetStartTime() - currentUTC;
		if (timeElapse < _alarmTime + 30 && timeElapse >= _alarmTime && it->GetStatus() == MeetingStatus::MS_NORMAL) {
			++it;
			continue;
		}
		it = meetings.erase(it);
	}

	if (!meetings.empty()) {
		UC_LOG(INFO) << "+++OnMeetingAlarmed triggerEvent to JS. meeting size = " << meetings.size() << ", the first meeting start time = " << meetings[0].GetStartTime();
		triggerEvent(ICalendarServiceEvent, OnMeetingAlarmed(meetings));
	}
}


int32_t CalendarService::UploadMeetingFile(int64_t eventId, int32_t operation, const DocumentInfo& info, int32_t& msgId) {
	UC_LOG(INFO) << "UploadMeetingFile enter. event id = " << eventId << ", operation = " << operation;
	msgId = DBManager::GetInstance().GetMessageID();
	HTTPRequest request;
	_calendarRequestFactory->MakeRequestOfUploadMeetingFile(eventId, operation, info, request);
	_engine->SendAsyncRequest(request, MakeRequestCallback(this, &CalendarService::OnMeetingFileUploaded, eventId, operation, msgId, info));

	UC_LOG(INFO) << "UploadMeetingFile over. request URL = " << request.GetURL() << ", request body = " << request.GetBody();
	return UC_SUCCEEDED;
}


int32_t CalendarService::OnMeetingFileUploaded(int32_t errorCode, HTTPResponse *response, int64_t eventId, int32_t operation, int32_t msgId, DocumentInfo info)
{
	int32_t result = HTTPResponseAssist::GuardStatementForCallBack("OnMeetingFileUploaded", errorCode, response);
	UC_LOG(INFO) << "OnMeetingFileUploaded enter. Http request result = " << result << ", eventId:" << eventId << ", operation = " << operation;

	info.SetMsgId(msgId);
	info.SetCreatorName(GetUserName());
	info.SetCreatorId(GetUserId());
	int32_t operatorId = GetUserId();
	if (result != UC_SUCCEEDED) {
		triggerEvent(ICalendarServiceEvent, OnMeetingFileUploaded(result, eventId, operation, operatorId, info));
		return result;
	}

	//解析json
	int32_t code = -1;
	try {
		wstringstream jsonStream;
		boost::property_tree::wptree ptRoot;
		jsonStream.str(StringConvert::FromUTF8Str(response->GetContent()).c_str());
		boost::property_tree::json_parser::read_json(jsonStream, ptRoot);
		code = ptRoot.get<int32_t>((KEY_CODE), code);
		if (code != 0) {
			UC_LOG(ERROR) << "OnMeetingFileUploaded error, server response failed, error code = " << code;
			triggerEvent(ICalendarServiceEvent, OnMeetingFileUploaded(code, eventId, operation, operatorId, info));
			return code;
		}

		boost::property_tree::wptree ptChild = ptRoot.get_child(KEY_DATA);
		if (ptChild.empty()) {
			UC_LOG(ERROR) << "OnMeetingFileUploaded error, server response data is null";
			triggerEvent(ICalendarServiceEvent, OnMeetingFileUploaded(code, eventId, operation, operatorId, info));
			return code;
		}
		info.SetFileId(ptChild.get<int64_t>((KEY_SUMMARYID), 0));
		info.SetMessageSeq(ptChild.get<int64_t>(L"seq"));
	}
	catch (boost::property_tree::ptree_error & e) {
		UC_LOG(ERROR) << "encounter an exception when parse JSON from OnMeetingFileUploaded , exception: " << e.what();
		triggerEvent(ICalendarServiceEvent, OnMeetingFileUploaded(JSON_PARSE_ERROR, eventId, operation, operatorId, info));
		return JSON_PARSE_ERROR;
	}

	//回调上层
	triggerEvent(ICalendarServiceEvent, OnMeetingFileUploaded(code, eventId, operation, operatorId, info));

	UC_LOG(INFO) << "OnMeetingFileUploaded success.";
	return code;
}


int32_t CalendarService::DeleteMeetingFile(int64_t minutesId) {
	UC_LOG(INFO) << "DeleteMeetingFile enter. minutesId = " << minutesId;
	HTTPRequest request;
	_calendarRequestFactory->MakeRequestOfDeleteMeetingFile(minutesId, request);
	_engine->SendAsyncRequest(request, MakeRequestCallback(this, &CalendarService::OnMeetingFileDeleted, minutesId));

	UC_LOG(INFO) << "UploadMeetingFile over. request URL = " << request.GetURL() << ", request body = " << request.GetBody();
	return UC_SUCCEEDED;
}


int32_t CalendarService::OnMeetingFileDeleted(int32_t errorCode, HTTPResponse *response, int64_t minutesId)
{
	int32_t result = HTTPResponseAssist::GuardStatementForCallBack("OnMeetingFileDeleted", errorCode, response);
	UC_LOG(INFO) << "OnMeetingFileDeleted enter. Http request result = " << result << ", minutesId:" << minutesId;

	int32_t operatorId = GetUserId();
	if (result != UC_SUCCEEDED) {
		triggerEvent(ICalendarServiceEvent, OnMeetingFileDeleted(result, minutesId, operatorId));
		return result;
	}

	//解析json
	int32_t code = -1;
	try {
		wstringstream jsonStream;
		boost::property_tree::wptree ptRoot;
		jsonStream.str(StringConvert::FromUTF8Str(response->GetContent()).c_str());
		boost::property_tree::json_parser::read_json(jsonStream, ptRoot);
		code = ptRoot.get<int32_t>((KEY_CODE), code);
		if (code != 0) {
			UC_LOG(ERROR) << "OnMeetingFileDeleted error, server response failed, error code = " << code;
			triggerEvent(ICalendarServiceEvent, OnMeetingFileDeleted(code, minutesId, operatorId));
			return code;
		}
	}
	catch (boost::property_tree::ptree_error & e) {
		UC_LOG(ERROR) << "encounter an exception when parse JSON from OnMeetingFileDeleted , exception: " << e.what();
		triggerEvent(ICalendarServiceEvent, OnMeetingFileDeleted(JSON_PARSE_ERROR, minutesId, operatorId));
		return JSON_PARSE_ERROR;
	}

	//回调上层
	triggerEvent(ICalendarServiceEvent, OnMeetingFileDeleted(code, minutesId, operatorId));

	UC_LOG(INFO) << "OnMeetingFileDeleted success.";
	return code;
}

int32_t CalendarService::ResendMessage(int32_t msgId){

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
		// TODO 这里需要修改 返回真实的数据
		_engine->SendAsyncRequest(request, MakeRequestCallback(this, &CalendarService::OnSendMessageResponse, head.GetTo().GetUserId(), 
			(int64_t)0, (int64_t)0, msgId, true));

		return UC_SUCCEEDED;
	}

	return UC_FAILED;
}

int32_t CalendarService::ForwardMessage(UCID groupUid, int32_t msgId){
	
// 	MessageInfo messageInfo;
// 	// TODO: marked by jeffrey, waiting for xuejian to change
// 	//bool result = theStore.GetMsgStore()->GetMessageById( msgId, messageInfo );
// 	bool result = false;
// 	if(result){
// 		HTTPRequest	request;
// 		MessageProcessor msgProcessor((UCEngine*)_engine);
// 		if (UC_SUCCEEDED == msgProcessor.CreateForwardMessage(GetCurUser(), groupUid, messageInfo, request)){
// 			_engine->SendAsyncRequest(request, MakeRequestCallback(this, &CalendarService::OnSendMessageResponse, messageInfo.GetTo().GetUserId(), (int64_t)0, (int64_t)0, msgId));
// 			return UC_SUCCEEDED;
// 		}
// 	}
// 	
	return UC_FAILED;
}

int32_t CalendarService::SendTextMessage(int64_t eventId, int64_t startTime, UCID groupUid, int8_t textType, const std::string& textFormat, const std::string& textData, int32_t& msgId){
	
	MessageProcessor msgProcessor((UCEngine*)_engine);
	HTTPRequest	request;
	UCMsg *msg = NULL;
	BinMsg data;
	string body;
	std::vector<UCID> atUsers;

	msgId = msgProcessor.CreateTextMessage(CONFERENCE_CHAT, GetCurUser(), groupUid, textType, textFormat, textData, "", "", GetConferenceIdByEventId(eventId, startTime), eventId, startTime, atUsers, msg);
	if (msg == NULL) {
		return UC_FAILED;
	}

	if (msgProcessor.EncodeMessageData(*msg, data, body) != UC_SUCCEEDED) {
		return UC_FAILED;
	}

	if (msgProcessor.SaveMessage(CONFERENCE_CHAT, groupUid.GetUserId(), 0, 0, STATE_SENDING, msg, body) != UC_SUCCEEDED){
		UC_LOG(ERROR) << "save group message in cache failed. relate_id: " << groupUid.GetUserId();
		delete msg;
		msg = NULL;
		return UC_FAILED;
	}

	if (UC_SUCCEEDED == msgProcessor.CreateMessageRequest(data, request)) {
		_engine->SendAsyncRequest(request, MakeRequestCallback(this, &CalendarService::OnSendMessageResponse, groupUid.GetUserId(),
			eventId, startTime, msgId, true));

		delete msg;
		msg = NULL;
		return UC_SUCCEEDED;
	}

	delete msg;
	msg = NULL;	
	return UC_FAILED;
}

int32_t CalendarService::SendImageMessage(int64_t eventId, int64_t startTime, UCID groupUid, const std::string& imageName, int32_t imageSize,
	const std::string& imageURL, const std::string& imageData, int32_t& msgId){

	MessageProcessor msgProcessor((UCEngine*)_engine);
	HTTPRequest	request;
	UCMsg *msg = NULL;
	BinMsg data;
	string body;

	msgId = msgProcessor.CreateImageMessage(CONFERENCE_CHAT, GetCurUser(), groupUid, imageName, imageSize, imageURL, imageData, "", -1, GetConferenceIdByEventId(eventId, startTime), eventId, startTime, msg);
	if (msg == NULL) {
		UC_LOG(ERROR) << "create image message failed. ";
		return UC_FAILED;
	}

	if (msgProcessor.EncodeMessageData(*msg, data, body) != UC_SUCCEEDED) {
		UC_LOG(ERROR) << "encode image message failed. ";
		return UC_FAILED;
	}

	if (msgProcessor.SaveMessage(CONFERENCE_CHAT, groupUid.GetUserId(), 0, 0, STATE_SENDING, msg, body) != UC_SUCCEEDED){
		UC_LOG(ERROR) << "save group message in cache failed. relate_id: " << groupUid.GetUserId();
		delete msg;
		msg = NULL;
		return UC_FAILED;
	}

	if (UC_SUCCEEDED == msgProcessor.CreateMessageRequest(data, request)) {
		_engine->SendAsyncRequest(request, MakeRequestCallback(this, &CalendarService::OnSendMessageResponse, groupUid.GetUserId(), 
			eventId, startTime, msgId, true));

		delete msg;
		msg = NULL;
		return UC_SUCCEEDED;
	}

	delete msg;
	msg = NULL;
	return UC_FAILED;
}
		
int32_t CalendarService::SendAudioMessage(int64_t eventId, int64_t startTime, UCID groupUid, const std::string& audioName, int32_t audioSize,
	const std::string& audioURL, int32_t audioDuration, int32_t& msgId){
	
	MessageProcessor msgProcessor((UCEngine*)_engine);
	HTTPRequest	request;
	UCMsg *msg = NULL;
	BinMsg data;
	string body;

	msgId = msgProcessor.CreateAudioMessage(CONFERENCE_CHAT, GetCurUser(), groupUid, audioName, audioSize, audioURL, audioDuration, GetConferenceIdByEventId(eventId, startTime), eventId, startTime, msg);
	if (msg == NULL) {
		UC_LOG(ERROR) << "create audio message failed. ";
		return UC_FAILED;
	}

	if (msgProcessor.EncodeMessageData(*msg, data, body) != UC_SUCCEEDED) {
		UC_LOG(ERROR) << "encode audio message failed. ";
		return UC_FAILED;
	}

	if (msgProcessor.SaveMessage(CONFERENCE_CHAT, groupUid.GetUserId(), 0, 0, STATE_SENDING, msg, body) != UC_SUCCEEDED){
		UC_LOG(ERROR) << "save group message in cache failed. relate_id: " << groupUid.GetUserId();
		delete msg;
		msg = NULL;
		return UC_FAILED;
	}

	if (UC_SUCCEEDED == msgProcessor.CreateMessageRequest(data, request)) {
		_engine->SendAsyncRequest(request, MakeRequestCallback(this, &CalendarService::OnSendMessageResponse, groupUid.GetUserId(), 
			eventId, startTime, msgId, true));

		delete msg;
		msg = NULL;
		return UC_SUCCEEDED;
	}

	delete msg;
	msg = NULL;
	return UC_FAILED;
}
	 
int32_t CalendarService::SendVideoMessage(int64_t eventId, int64_t startTime, UCID groupUid, const std::string& videoName, int32_t videoSize,
	const std::string& videoURL, const std::string& videoData, int32_t videoDuration, int32_t& msgId){
	
	MessageProcessor msgProcessor((UCEngine*)_engine);
	HTTPRequest	request;
	UCMsg *msg = NULL;
	BinMsg data;
	string body;

	msgId = msgProcessor.CreateVideoMessage(CONFERENCE_CHAT, GetCurUser(), groupUid, videoName, videoSize, videoURL, videoData, videoDuration, GetConferenceIdByEventId(eventId, startTime), eventId, startTime, msg);
	if (msg == NULL) {
		UC_LOG(ERROR) << "create video message failed. ";
		return UC_FAILED;
	}

	if (msgProcessor.EncodeMessageData(*msg, data, body) != UC_SUCCEEDED) {
		UC_LOG(ERROR) << "encode video message failed. ";
		return UC_FAILED;
	}

	if (msgProcessor.SaveMessage(CONFERENCE_CHAT, groupUid.GetUserId(), 0, 0, STATE_SENDING, msg, body) != UC_SUCCEEDED){
		UC_LOG(ERROR) << "save group message in cache failed. relate_id: " << groupUid.GetUserId();
		delete msg;
		msg = NULL;
		return UC_FAILED;
	}

	if (UC_SUCCEEDED == msgProcessor.CreateMessageRequest(data, request)) {
		_engine->SendAsyncRequest(request, MakeRequestCallback(this, &CalendarService::OnSendMessageResponse, groupUid.GetUserId(),
			eventId, startTime, msgId, true));

		delete msg;
		msg = NULL;
		return UC_SUCCEEDED;
	}

	delete msg;
	msg = NULL;
	return UC_FAILED;
}

int32_t CalendarService::OnSendMessageResponse(int32_t errorCode, HTTPResponse *response, int32_t groupId, int64_t eventId,
	int64_t startTime, int32_t msgId, bool saveUnread) {

	UC_LOG(INFO) << "OnSendMessageResponse UserId:" << groupId << " EventId:" << eventId << " StartTime:" << startTime << " MsgId:" << msgId
		<< " errorCode:" << errorCode << " response:" << (response != NULL ? response->GetContent() : "");

	int32_t result = UC_SUCCEEDED;
	int64_t msgSeq = 0;
	int64_t sendTime = 0;

	if (UC_SUCCEEDED == errorCode && NULL != response && 200 == response->GetStatusCode()) {
		wstringstream wsStream;
		wsStream.str(StringConvert::FromUTF8Str(response->GetContent()).c_str());
		boost::property_tree::wptree jsonRoot;
		boost::property_tree::json_parser::read_json(wsStream, jsonRoot);
		result = jsonRoot.get<int32_t>(L"code");

		if (UC_SUCCEEDED == result) {
			try {
				boost::property_tree::wptree jsonChild = jsonRoot.get_child(L"success_data");
				BOOST_FOREACH(boost::property_tree::wptree::value_type v, jsonChild){
					result = v.second.get<int32_t>(L"code", 0);
					msgId = v.second.get<int32_t>(L"msg_id", 0);
					msgSeq = v.second.get<int64_t>(L"seq", 0);
					sendTime = v.second.get<int64_t>(L"timestamp", 0);
				}

				jsonChild = jsonRoot.get_child(L"failed_data");
				BOOST_FOREACH(boost::property_tree::wptree::value_type v, jsonChild){
					result = v.second.get<int32_t>(L"code", 0);
					msgId = v.second.get<int32_t>(L"msg_id", 0);
					msgSeq = v.second.get<int64_t>(L"seq", 0);
					sendTime = v.second.get<int64_t>(L"timestamp", 0);
				}
			}
			catch(boost::property_tree::ptree_error & e) {
				UC_LOG(ERROR) << "encounter an exception when parse json from OnSendMessageResponse , exception: " << e.what();
				result = JSON_PARSE_ERROR;
			}
		}
	} else {
		result = UC_FAILED;
	}

	if (UC_SUCCEEDED == result) {
		theStore.GetMsgStore()->UpdateGroupMessageState(groupId, msgId, STATE_SENDSUCCESS, msgSeq);
	} else {
		theStore.GetMsgStore()->UpdateGroupMessageState(groupId, msgId, STATE_SENDFAIL, msgSeq);
	}

	triggerEvent(ICalendarServiceEvent, OnMessageSent(result, groupId, eventId, startTime, msgId, msgSeq, sendTime));

	if (saveUnread && UC_SUCCEEDED == result){	
		OnMessageUnreadUserNotify(groupId, msgSeq);
	}

	return result;
}

//-----------------------------------util----------------------------------------------//
std::string CalendarService::GetRequestUrl( const std::string &urlMethod ) {
	std::stringstream urlStream;
	ClientService* clientService = (ClientService*)_engine->QueryService(UCClientService);
	if(clientService != NULL) {
		urlStream << clientService->GetLoginInfo().GetUccServerDomain() << urlMethod;
	}

	return urlStream.str();
}


int32_t CalendarService::GetUserId() {
	ClientService* clientService = (ClientService*)_engine->QueryService(UCClientService);
	if(clientService != NULL) {
		return clientService->GetLoginUserId();
	}
	return 0;
}

UCID CalendarService::GetCurUser(){
	UCID userUid;
	ClientService* clientService = (ClientService*)_engine->QueryService(UCClientService);
	if(clientService != NULL) {
		userUid = clientService->GetLoginInfo().GetCurUser();
	}
	return userUid;
}

std::string CalendarService::GetAccount() {
	std::string account = "";
	ClientService* clientService = (ClientService*)_engine->QueryService(UCClientService);
	if(clientService != NULL) {
		account =  clientService->GetLoginInfo().GetAccount();
	}
	return account;
}


std::string CalendarService::GetDoMain() {
	std::string doMain = "";
	ClientService* clientService = (ClientService*)_engine->QueryService(UCClientService);
	if (clientService != NULL) {
		doMain = clientService->GetLoginInfo().GetUccServerDomain_Pure();
	}
	return doMain;
}

std::string CalendarService::GetRemoteAvatar() {
	std::string url = "";
	ContactService* contactService = (ContactService*)_engine->QueryService(UCContactService);
	if (contactService != NULL) {
		BasicContactInfo info;
		if (contactService->GetBasicContactInfo(GetUserId(), info)) {
			return info.GetRemoteAvatarUrl();
		}
	}
	return url;
}

std::string CalendarService::GetSessionId() {
	std::string sessionId = "";
	ClientService* clientService = (ClientService*)_engine->QueryService(UCClientService);
	if(clientService != NULL) {
		sessionId = clientService->GetLoginInfo().GetSessionId();
	}
	return sessionId;
}

bool CalendarService::IsCurUser(UCID userUid){

	if (userUid.GetUserId() == GetCurUser().GetUserId()
		&& userUid.GetResId() == GetCurUser().GetResId()
		&& userUid.GetSiteId() == GetCurUser().GetSiteId()){
			return true;
	}
	return false;
}

int32_t CalendarService::OnMessageReceived(MessageChannel channel, UCMsg &msg, const string &body, MsgSendState state) {
	int32_t type = msg.GetMsgType();
	bool isSave = false;
	bool saveUnread = false;
	UCHead head;
	msg.GetHead(head);
	//会议邀请
	if (type == UCMeetingConfInviteIQInvite) {
		_calendarMsgProcessor->ProcessMeetingInviteMsg(channel, (IQInviteContentMsg&)msg, state);
	} else if (type == UCMeetingConfInviteIQReceive) {
		_calendarMsgProcessor->ProcessMeetingAcceptMsg(channel, (IQReceiveContentMsg&)msg, state);
	} else if (type == UCMeetingConfInviteIQReject) {
		_calendarMsgProcessor->ProcessMeetingDenyMsg(channel, (IQRejectContentMsg&)msg, state);
	} else if (type == UCMeetingConfUpdateIQInvite) {
		_calendarMsgProcessor->ProcessMeetingUpdateMsg(channel, (IQInviteContentMsg&)msg, state);
	} else if (type == UCMeetingConfCancel) {
		_calendarMsgProcessor->ProcessMeetingCancelMsg(channel, (ConfCancelContentMsg&)msg, state);
	} else if (type == UCMeetingConfForwardIQInvite) {
		_calendarMsgProcessor->ProcessMeetingForwardMsg(channel, (IQInviteContentMsg&)msg, state);
	} else if (type == UCMeetingSummaryCreate) {
		_calendarMsgProcessor->ProcessMinutesCreateMsg(channel, (ConfSummaryContentMsg&)msg, state, isSave);
	} else if (type == UCMeetingSummaryUpdate) {
		_calendarMsgProcessor->ProcessMinutesUpdateMsg(channel, (ConfSummaryContentMsg&)msg, state, isSave);
	} else if (type == UCMeetingChatText) {
		_calendarMsgProcessor->ProcessMeetingChatTextMsg(channel, (ConfChatContentMsg&)msg, state, isSave, saveUnread);
	} else if (type == UCMeetingChatMedia) {
		_calendarMsgProcessor->ProcessMeetingChatMediaMsg(channel, (ConfChatContentMsg&)msg, state, isSave, saveUnread);
	}else if (type == UCMeetingChatReprot) {
		_calendarMsgProcessor->ProcessMeetingChatReportMsg(channel, (ReportContentMsg&)msg, state, isSave);
	}else if (type == UCCalendarConfShare) {
		_calendarMsgProcessor->ProcessAccreditMsg(channel, (ConfShareContentMsg&)msg,state);
	} else if (type == UCCalendarConfShareCancel) {
		_calendarMsgProcessor->ProcessAccreditCancelMsg(channel, (ConfShareContentMsg&)msg,state);
	} else if (type == UCMeetingConfAlertPartJoin) {
		_calendarMsgProcessor->ProcessMeetingAlerPartJoinMsg(channel, (ConfAlertContentMsg&)msg, state);
	} else if (type == UCMeetingConfAlertPartQuit) {
		_calendarMsgProcessor->ProcessMeetingAlerPartQuitMsg(channel, (ConfAlertContentMsg&)msg, state);
	} else if (type == UCRemoteAssistInvite) {
		_calendarMsgProcessor->ProcessRemoteAssistanceInviteMsg(channel, (RemoteAssistanceInviteContentMsg&)msg, state);
	} else if (type == UCRemoteAssistAccept) {
		_calendarMsgProcessor->ProcessRemoteAssistanceAcceptMsg(channel, (RemoteAssistanceAcceptContentMsg&)msg, state);
	} else if (type == UCRemoteAssistReject) {
		_calendarMsgProcessor->ProcessRemoteAssistanceRejectMsg(channel, (RemoteAssistanceRejectContentMsg&)msg, state);
	} else if (type == UCRemoteAssistEnd) {
		_calendarMsgProcessor->ProcessRemoteAssistanceEndMsg(channel, (RemoteAssistanceEndContentMsg&)msg, state, isSave);
	} else if (type == UCMeetingFileUpload) {
		_calendarMsgProcessor->ProcessMeetingDocumentUploadMsg(channel, (ConfUploadContentMsg&)msg, state, isSave, saveUnread);
	} else if (type == UCMeetingDelDoc) {
		 _calendarMsgProcessor->ProcessMeetingDocumentDeleteMsg(channel, (ConfDelDocContentMsg&)msg, state, isSave, saveUnread);
	}else if (type == UCMeetingRevocationMessage){
		_calendarMsgProcessor->ProcessMeetingRevocationMsg(channel, head, (ConfChatContentMsg&)msg, state,isSave);
	 }

	if (isSave && UCAS == channel){
		MessageProcessor msgProcessor((UCEngine*)_engine);

		int64_t relate_id = MessageProcessor::GetRelateId(CONFERENCE_CHAT, GetCurUser().GetUserId(), head.GetFrom().GetUserId(), head.GetTo().GetUserId());
		if (msgProcessor.SaveMessage(CONFERENCE_CHAT, relate_id/*head.GetTo().GetUserId()*/, 0, 0, STATE_SENDSUCCESS, &msg, body) != UC_SUCCEEDED)
		{
			UC_LOG(ERROR) << "CalendarService::OnMessageReceived save message failed." << " msgType = " << type;
		}

		if (saveUnread && (head.GetFrom().GetUserId() == GetCurUser().GetUserId())){
			OnMessageUnreadUserNotify(head.GetTo().GetUserId(), head.GetSeq());
		}
	}

	return 0;
}

void CalendarService::OnUCASDisconnected(int32_t errorCode) {
	UC_LOG(INFO) << "CalendarService OnUCASDisconnected enter. error code = " << errorCode;

	// 置数据库中所有会议数据为old状态
	DBManager::GetInstance().SetAllMeetingOld();

	//关闭会议提醒timer
	SetMeetingAlarm(0, DEF_ALARM_TIME);
}

void CalendarService::OnUCASConnected(int32_t errorCode) {
	UC_LOG(INFO) << "CalendarService OnUCASConnected enter.";
	if (errorCode != 0) {
		UC_LOG(ERROR) << "CalendarService OnUCASConnected errorCode = " << errorCode;
		return;
	}

	// 先缓存当天零时往后的30条数据
	CacheMeetingList();

	//开启会议提醒timer
	SetMeetingAlarm(1, DEF_ALARM_TIME);
}


int32_t CalendarService::MakeRequestId() {
	static int32_t temp = 1;
	return ((int32_t)time(NULL) + temp++);	
}

void CalendarService::ParseICalendarStr( const std::string& icalendar, vector<Meeting>& infos )
{
	ICalendar Calendar;
	Calendar.LoadICalendarStr(icalendar);
	VEvent *CurrentEvent;
	ICalendar::Query SearchQuery(&Calendar);
	Meeting info;
	list<VEvent*>::iterator it = Calendar.Events.begin();
	while (it != Calendar.Events.end()) {
		CurrentEvent = *it;
		info.SetConferenceId(CurrentEvent->ConferenceId);
		info.SetEventId(StringToInt64(CurrentEvent->EventId));
		info.SetGroupId(StringToInt64(CurrentEvent->GroupId));
		info.SetRoomId(CurrentEvent->RoomId);
		info.SetTitle(CurrentEvent->Summary);
		info.SetSummary(CurrentEvent->Description);
		info.SetStartTime(CurrentEvent->DtStart.ToUnixTime());
		unsigned long entTime = CurrentEvent->DtEnd.ToUnixTime();
		int32_t druation = entTime - info.GetStartTime();
		druation /= 60;
		info.SetDuration(druation);
		info.SetLocation(CurrentEvent->Location);
		info.SetExtrasInfo(CurrentEvent->ExtrasInfo);
		info.SetHostId(StringToInt32(CurrentEvent->HostId));
		info.SetHostName(CurrentEvent->HostName);
		info.SetHostPassword(CurrentEvent->PCode1);
		info.SetAttendeePassword(CurrentEvent->PCode2);
		info.SetBillingCode(CurrentEvent->BillingCode);
		info.IsGNet(StringToInt32(CurrentEvent->IsGnet));
		info.IsWholeDay(StringToInt32(CurrentEvent->IsWholeDay));
		info.SetUpdateFlag(StringToInt32(CurrentEvent->UpdateFlag));
		if (CurrentEvent->Status.compare("CANCELLED") == 0) {
			info.SetStatus(MeetingStatus::MS_CANCELED);
		}
		
		vector<Attendee> attendees;
		for(unsigned int i=0; i<CurrentEvent->Attendees.size(); ++i) {
			Attendee attendee;
			attendee.SetAccount(CurrentEvent->Attendees[i].Account);
			attendee.SetType((uc::AttendeeType)CurrentEvent->Attendees[i].Type);
			attendee.SetStatus((uc::AttendeeStatus)CurrentEvent->Attendees[i].Status);
			attendees.push_back(attendee);
		}
		info.SetAttendees(attendees);
		infos.push_back(info);
		it++;
	}
}

void CalendarService::ParseICalendarStr( const std::string& icalendar, Meeting& info )
{
	ICalendar Calendar;
	Calendar.LoadICalendarStr(icalendar);
	VEvent *CurrentEvent;
	list<VEvent*>::iterator it = Calendar.Events.begin();
	if (it != Calendar.Events.end()) {
		CurrentEvent = *it;

		info.SetConferenceId(CurrentEvent->ConferenceId);
		info.SetEventId(StringToInt64(CurrentEvent->EventId));
		info.SetGroupId(StringToInt64(CurrentEvent->GroupId));
		info.SetRoomId(CurrentEvent->RoomId);
		info.SetTitle(CurrentEvent->Summary);
		info.SetSummary(CurrentEvent->Description);
		info.SetStartTime(CurrentEvent->DtStart.ToUnixTime());
		unsigned long entTime = CurrentEvent->DtEnd.ToUnixTime();
		int32_t druation = entTime - info.GetStartTime();
		druation /= 60;
		info.SetDuration(druation);
		info.SetLocation(CurrentEvent->Location);
		info.SetExtrasInfo(CurrentEvent->ExtrasInfo);
		info.SetHostId(StringToInt32(CurrentEvent->HostId));
		info.SetHostName(CurrentEvent->HostName);
		info.SetHostPassword(CurrentEvent->PCode1);
		info.SetAttendeePassword(CurrentEvent->PCode2);
		info.SetBillingCode(CurrentEvent->BillingCode);
		info.IsGNet(StringToInt32(CurrentEvent->IsGnet));
		info.IsWholeDay(StringToInt32(CurrentEvent->IsWholeDay));
		info.SetUpdateFlag(StringToInt32(CurrentEvent->UpdateFlag));
		if (CurrentEvent->Status.compare("CANCELLED") == 0) {
			info.SetStatus(MeetingStatus::MS_CANCELED);
		}
		vector<Attendee> attendees;
		for(unsigned int i=0; i<CurrentEvent->Attendees.size(); ++i) {
			Attendee attendee;
			attendee.SetAccount(CurrentEvent->Attendees[i].Account);
			attendee.SetType((uc::AttendeeType)CurrentEvent->Attendees[i].Type);
			attendee.SetStatus((uc::AttendeeStatus)CurrentEvent->Attendees[i].Status);
			attendees.push_back(attendee);
		}
		info.SetAttendees(attendees);
	}
}

int64_t CalendarService::StringToInt64( const string& str ) {
	if (str.empty()) {
		return 0;
	}
	stringstream stream(str);
	int64_t n;
	stream>>n;
	return n;
}

int32_t CalendarService::StringToInt32( const string& str ) {
	if (str.empty()) {
		return 0;
	}
	stringstream stream(str);
	int32_t n;
	stream>>n;
	return n;
}

bool CalendarService::StringToBool(const string& str) {
	if (str.compare("true") == 0 || str.compare("TRUE") == 0)
	{
		return true;
	}
	else if (str.compare("false") == 0 || str.compare("FALSE") == 0)
	{
		return false;
	}
	else
	{
		int32_t iVal = StringToInt32(str);
		if (iVal == 0)
		{
			return false;
		}
	}

	return true;
}
bool CalendarService::ParseMeeting( const boost::property_tree::wptree &ptParse, Meeting *info ) {
	if( ptParse.empty() || info == NULL) {
		return false;
	}

	std::wstring content = ptParse.get<std::wstring>(L"content");
	if (content.empty()) {
		return false;
	}

	ParseICalendarStr(StringConvert::ToUTF8Str(content), *info);
	return true;
}

void CalendarService::UpdateSyncRange( int32_t userId, int64_t startTime, int64_t endTime ) {
	if (startTime <= 0 || endTime <= 0 || startTime >= endTime) {
		return;
	}

	std::map<int32_t, SyncRange>::iterator it = _syncInfo.find(userId);
	if (it != _syncInfo.end()) {
		int64_t oriStart = it->second.GetStartTime();
		int64_t oriEnd = it->second.GetEndTime();

		//防止跳空，保证数据连续性
		if (startTime < oriStart && endTime > oriEnd) {
			it->second.SetStartTime(startTime);
			it->second.SetEndTime(endTime);
		} else if (startTime < oriStart && endTime >= oriStart) {
			it->second.SetStartTime(startTime);
		} else if (endTime > oriEnd && startTime <= oriEnd) {
			it->second.SetEndTime(endTime);
		}
	} else {
		_syncInfo.insert(make_pair(userId, SyncRange(startTime, endTime)));
	}
}

bool CalendarService::IsTimeInSyncRange( int32_t userId, int64_t time ) {
	std::map<int32_t, SyncRange>::iterator it = _syncInfo.find(userId);
	if (it != _syncInfo.end()) {
		if (time >= it->second.GetStartTime() && time <= it->second.GetEndTime()) {
			return true;
		}
	}
	return false;
}

bool CalendarService::haveOldMeeting(const std::vector<Meeting>& meetings) {
	int32_t isOld = 0;
	for (const Meeting& meeting : meetings) {
		if (meeting.IsOld()) {
			UC_LOG(INFO) << "haveOldMeeting return true.";
			return true;
		}
	}

	return false;
}

int64_t CalendarService::GetSyncRangStart( int32_t userId ) {
	std::map<int32_t, SyncRange>::iterator it = _syncInfo.find(userId);
	if (it != _syncInfo.end()) {
		return it->second.GetStartTime();
	}

	return 0;
}

int64_t CalendarService::GetSyncRangEnd( int32_t userId ) {
	std::map<int32_t, SyncRange>::iterator it = _syncInfo.find(userId);
	if (it != _syncInfo.end()) {
		return it->second.GetEndTime();
	}

	return 0;
}

void CalendarService::SortMeetingList(std::vector<Meeting>& meetings) {
	std::sort(meetings.begin(), meetings.end(), [](Meeting& m1, Meeting& m2) {
		return m1.GetStartTime() < m2.GetStartTime();
	});
}

void CalendarService::EraseNoDisplayedMeeting(std::vector<Meeting>& meetings) {
	vector<Meeting>::iterator it = meetings.begin();
	while (it != meetings.end()) {
		MeetingStatus status = it->GetStatus();
		if (status != MeetingStatus::MS_NORMAL || IsDeniedMeeting(*it)) {
			it = meetings.erase(it);
		}
		else
			++it;
	}
}

void CalendarService::CacheMeetings(std::vector<Meeting>& meetings) {
	vector<Meeting>::iterator it = meetings.begin();
	vector<Meeting>::iterator it_cache = _cacheMeetings.begin();
	//以server为准
	while (it != meetings.end()) {
		while (it_cache != _cacheMeetings.end()){
			if (it->GetEventId() == it_cache->GetEventId() && it->GetStartTime() == it_cache->GetStartTime()
				&& it->GetAuthorizerId() == it_cache->GetAuthorizerId()) {
				it_cache = _cacheMeetings.erase(it_cache);
			}
			else
				it_cache++;
		}
		it++;
	}
	std::copy(meetings.begin(), meetings.end(), std::back_inserter(_cacheMeetings));
}

void CalendarService::SetMeetingAuthorizerId(std::vector<Meeting>& meetings, int32_t authorizerId) {
	for (Meeting& m : meetings) {
		m.SetAuthorizerId(authorizerId);
	}
}

bool CalendarService::FindInCacheMeeting(int64_t eventId, int64_t startTime, int32_t authorizerId, Meeting& meeting) {
	for (const Meeting& m : _cacheMeetings) {
		if (eventId == m.GetEventId() && authorizerId == m.GetAuthorizerId()) {
			if ((startTime > 0 && startTime == m.GetStartTime()) || startTime <= 0) {
				meeting = m;
				return true;
			}
		}
	}

	return false;
}

bool CalendarService::FindInCacheMeeting(const std::string& confId, Meeting& meeting){
	for (const Meeting& m : _cacheMeetings) {
		if (confId.compare(m.GetConferneceId()) == 0) {
			meeting = m;
			return true;
		}
	}

	return false;
}

int64_t CalendarService::GetTodayTime() {
	time_t now_time;
	now_time = time(NULL);
	struct tm *time = localtime(&now_time);
	time->tm_hour = 0;
	time->tm_min = 0;
	time->tm_sec = 0;
	return mktime(time);
}


void CalendarService::MergeBusyTime(int64_t startTime, int64_t endTime, int32_t timeInterval, std::vector<int32_t> times, std::vector<TimeSlot>& timeSlots) {
	int32_t beg = 0;

	//时间点为空,表示整个时间段为空闲状态
	if (times.empty()) {
		timeSlots.push_back(TimeSlot(0, 0));
		return;
	}

	int32_t size = times.size();
	int32_t point = times.at(0);
	int64_t timeBeg = startTime + point * 60;
	int64_t timeEnd = timeBeg;
	while (beg<size) {
		point = times.at(beg);
		//最后一个时间点
		if (beg+1 >= size) {
			timeEnd = timeEnd + (timeInterval * 60);
			timeSlots.push_back(TimeSlot(timeBeg, timeEnd));
			return;
		}

		//与下一个时间点是否连续（差值小于等于时间间隔，认为连续）
		int32_t nextPoint = times.at(beg + 1);
		int32_t dValu = nextPoint - point;
		if (dValu <= timeInterval && dValu > 0) {
			timeEnd += (dValu * 60);
		} else if (dValu > timeInterval) {
			//时间点中断，将之前一段连续时间保存
			timeEnd += (timeInterval * 60);
			timeSlots.push_back(TimeSlot(timeBeg, timeEnd));
			timeBeg = startTime + nextPoint * 60;
			timeEnd = timeBeg;
		} else {
			timeEnd = timeBeg;
		}
		++beg;
	}
}

int64_t CalendarService::diff_tm(struct tm *a, struct tm *b)
{
	return a->tm_sec - b->tm_sec
		+ 60LL * (a->tm_min - b->tm_min)
		+ 3600LL * (a->tm_hour - b->tm_hour)
		+ 86400LL * (a->tm_yday - b->tm_yday)
		+ (a->tm_year - 70) * 31536000LL
		- (a->tm_year - 69) / 4 * 86400LL
		+ (a->tm_year - 1) / 100 * 86400LL
		- (a->tm_year + 299) / 400 * 86400LL
		- (b->tm_year - 70) * 31536000LL
		+ (b->tm_year - 69) / 4 * 86400LL
		- (b->tm_year - 1) / 100 * 86400LL
		+ (b->tm_year + 299) / 400 * 86400LL;
}

int64_t CalendarService::GetCurrentUnixTime() {

	time_t now_time;
	now_time = time(NULL);
	struct tm *ptm = localtime(&now_time);
	time_t localTime = mktime(ptm);
	return localTime + diff_tm(ptm, gmtime(&localTime));
}

//判断是否是已经拒绝的会议（根据参会状态）
bool CalendarService::IsDeniedMeeting(const Meeting& meeting) {

	stringstream ssUserId;
	ssUserId << GetUserId();
	std::string sCurrentAccount = GetAccount();
	for (Attendee ad : meeting.GetAttendees()) {
		std::string account = ad.GetAccount();
		if (account.compare(ssUserId.str()) == 0 || account.compare(sCurrentAccount) == 0) {
			if (ad.GetStatus() == AttendeeStatus::AS_DENIED) {
				return true;
			}
		}

	}

	return false;
} 

//判断是否是已取消的会议（根据参会状态）
bool CalendarService::IsCanceledMeeting(const Meeting& meeting) {

	if (meeting.GetStatus() == MeetingStatus::MS_CANCELED) {
		return true;
	}

	return false;
}

int32_t CalendarService::GetGroupMemberList(int64_t groupId, int32_t& count, UserIdList& list){
	Meeting meeting;
	if( DBError::DB_SUCCESS != DBManager::GetInstance().GetMeeting(meeting, GetEventIdByGroupId(groupId))) {
		count = -1;
		return UC_FAILED;
	}

	if (GetCurUser().GetUserId() != meeting.GetHostId()){
		count ++;
		list.push_back(meeting.GetHostId());
	}

	std::vector<Attendee> attendees = meeting.GetAttendees();
	for (Attendee attendee : attendees){
		if (attendee.GetType() == AT_INTERNAL){
			int32_t userId = StringConvert::WStringToInt(StringConvert::FromUTF8Str(attendee.GetAccount()));
			if (GetCurUser().GetUserId() == userId){
				continue;
			}
			count ++;
			list.push_back(userId);
		}
	}

	return UC_SUCCEEDED;
}

int64_t CalendarService::GetEventIdByGroupId(int64_t groupId){
	
	std::vector<Meeting>::iterator it = std::find_if(_cacheMeetings.begin(),
		_cacheMeetings.end(), [&](Meeting info)->bool{ return info.GetGroupId() == groupId; });
	if (it != _cacheMeetings.end()){
		return it->GetEventId();
	}

	return DBManager::GetInstance().GetEventIdByGroupId(groupId);
}

int32_t CalendarService::OnMessageUnreadUserNotify(int64_t groupId, int64_t seq){

	int32_t count = 0;
	UserIdList userIdList;
	
	if (UC_SUCCEEDED != GetGroupMemberList(groupId, count, userIdList)){
		GroupService* groupService = (GroupService*)_engine->QueryService(UCGroupService);
		if (groupService){
			std::list<int64_t> seqList;
			seqList.push_back(seq);
			groupService->GetUnreadUserCount(CONFERENCE_CHAT, groupId, seqList);
		}
	}
	else{
		std::list<MessageReadInfo> list;
		MessageReadUserInfo info;
		info.SetMsgSeq(seq);
		info.SetConversationType(CONFERENCE_CHAT);
		info.SetRelateId(groupId);
		info.SetUnreadCount(count);
		if (count == MESSAGE_UNREAD_NONE){
			info.SetAllRead(true);
		}
		if (!userIdList.empty()){
			info.AddUser(userIdList);
		}
		list.push_back(info);
		theStore.GetMessageReadStore()->SaveMessageReadInfo(ConvKey(info.GetConversationType(), info.GetRelateId()), info);
		triggerEvent(ICalendarServiceEvent, OnMessageUnreadUserCount(UC_SUCCEEDED, groupId, list));
	}
	return UC_SUCCEEDED;
}

int32_t CalendarService::CreateRemoteAssistance(int32_t userId) {

	UC_LOG(INFO) << "CreateRemoteAssistance enter. userId = " << userId;

	HTTPRequest request;
	_calendarRequestFactory->MakeRequestOfCreateRemoteAssistance(userId, request);
	_engine->SendAsyncRequest(request, MakeRequestCallback(this, &CalendarService::OnRemotAssistanceCreated, userId));

	UC_LOG(INFO) << "CreateRemoteAssistance over. request URL = " << request.GetURL() << ", request body = " << request.GetBody();
	return UC_SUCCEEDED;
}

int32_t CalendarService::OnRemotAssistanceCreated(int32_t errorCode, HTTPResponse* response, int32_t userId) {

	int32_t result = HTTPResponseAssist::GuardStatementForCallBack("OnRemotAssistanceCreated", errorCode, response);
	UC_LOG(INFO) << "OnRemotAssistanceCreated enter. Http request result = " << result << ", to userId:" << userId;
	if (result != UC_SUCCEEDED) {
		triggerEvent(ICalendarServiceEvent, OnRemotAssistanceCreated(result, "", "", "", userId));
		return result;
	}

	//解析json
	int32_t code = -1;
	std::string confId = "";
	std::string passWd = "";
	std::string tangStartUrl = "";
	try {
		wstringstream jsonStream;
		boost::property_tree::wptree ptRoot;
		jsonStream.str(StringConvert::FromUTF8Str(response->GetContent()).c_str());
		boost::property_tree::json_parser::read_json(jsonStream, ptRoot);

		code = ptRoot.get<int32_t>((KEY_CODE), code);
		if (code != 0) {
			UC_LOG(ERROR) << "OnRemotAssistanceCreated error, server response failed, error code = " << code;
			triggerEvent(ICalendarServiceEvent, OnRemotAssistanceCreated(code, confId, passWd, tangStartUrl, userId));
			return code;
		}

		boost::property_tree::wptree ptData;
		boost::property_tree::wptree ptConfInfo;
		boost::property_tree::wptree ptDef;
		ptData = ptRoot.get_child(KEY_DATA, ptDef);
		if (ptData.empty()) {
			//数据为空
			UC_LOG(ERROR) << "OnRemotAssistanceCreated error, server response data is null.";
			triggerEvent(ICalendarServiceEvent, OnRemotAssistanceCreated(code, confId, passWd, tangStartUrl, userId));
			return code;
		}

		ptConfInfo = ptData.get_child(L"confInfo", ptDef);
		if (ptConfInfo.empty()) {
			//数据为空
			UC_LOG(ERROR) << "OnRemotAssistanceCreated error, server response confInfo is null.";
			triggerEvent(ICalendarServiceEvent, OnRemotAssistanceCreated(code, confId, passWd, tangStartUrl, userId));
			return code;
		}

		confId = StringConvert::ToUTF8Str(ptConfInfo.get<std::wstring>((L"conferenceId"), L""));
		passWd = StringConvert::ToUTF8Str(ptConfInfo.get<std::wstring>((KEY_ATTENDEEPASSWORD), L""));
		tangStartUrl = StringConvert::ToUTF8Str(ptData.get<std::wstring>(L"joinString", L""));
	}
	catch (boost::property_tree::ptree_error &e) {
		UC_LOG(ERROR) << "encounter an exception when parse JSON from OnRemotAssistanceCreated , exception: " << e.what();
		triggerEvent(ICalendarServiceEvent, OnRemotAssistanceCreated(JSON_PARSE_ERROR, confId, passWd, tangStartUrl, userId));
		return JSON_PARSE_ERROR;
	}

	//回调上层
	triggerEvent(ICalendarServiceEvent, OnRemotAssistanceCreated(code, confId, passWd, tangStartUrl, userId));

	UC_LOG(INFO) << "OnRemotAssistanceCreated success.";
	return 0;
}

int32_t CalendarService::SendRemoteAssistanceInviteMessage(UCID& uid, const std::string& confId, const std::string& confPwd, OUT int32_t& msgId) {
	
	UC_LOG(INFO) << "SendRemoteAssistanceInvitationMessage enter. toUserId = " << uid.GetUserId() << ", conferenceId = " << confId <<", confPwd = " << confPwd;
	int64_t msgTime = 0;
	HTTPRequest	request;
	MessageProcessor msgProcessor((UCEngine*)_engine);

	if (UC_SUCCEEDED == msgProcessor.CreateAssistInvitationMessage(SINGLE_CHAT, GetCurUser(), uid, confId, confPwd,  request, msgId, msgTime)){
		_engine->SendAsyncRequest(request, MakeRequestCallback(this, &CalendarService::OnSendRemoteAssistanceInviteMsgResponse, confId, msgId, msgTime));
		return UC_SUCCEEDED;
	}
	return UC_FAILED;
}

int32_t CalendarService::OnSendRemoteAssistanceInviteMsgResponse(int32_t errorCode, HTTPResponse *response, std::string conferenceId, int32_t msgId, int64_t msgTime) {
	int32_t result = HTTPResponseAssist::GuardStatementForCallBack("OnSendRemoteAssistanceInviteMsgResponse", errorCode, response);
	UC_LOG(INFO) << "OnSendRemoteAssistanceInviteMsgResponse enter. Http request result = " << result << ", MsgId:" << msgId << ", conferenceId:" << conferenceId;

	int64_t msgSeq = 0;
	int64_t sendTime = 0;

	if (UC_SUCCEEDED == result) {
		wstringstream wsStream;
		wsStream.str(StringConvert::FromUTF8Str(response->GetContent()).c_str());
		boost::property_tree::wptree jsonRoot;
		boost::property_tree::json_parser::read_json(wsStream, jsonRoot);
		result = jsonRoot.get<int32_t>(L"code");

		if (UC_SUCCEEDED == result) {
			try {
				boost::property_tree::wptree jsonChild = jsonRoot.get_child(L"success_data");
				BOOST_FOREACH(boost::property_tree::wptree::value_type v, jsonChild){
					result = v.second.get<int32_t>(L"code", 0);
					msgId = v.second.get<int32_t>(L"msg_id", 0);
					msgSeq = v.second.get<int64_t>(L"seq", 0);
					sendTime = v.second.get<int64_t>(L"timestamp", 0);
				}

				jsonChild = jsonRoot.get_child(L"failed_data");
				BOOST_FOREACH(boost::property_tree::wptree::value_type v, jsonChild){
					result = v.second.get<int32_t>(L"code", 0);
					msgId = v.second.get<int32_t>(L"msg_id", 0);
					msgSeq = v.second.get<int64_t>(L"seq", 0);
					sendTime = v.second.get<int64_t>(L"timestamp", 0);
				}
			}
			catch (boost::property_tree::ptree_error & e) {
				UC_LOG(ERROR) << "encounter an exception when parse JSON from OnSendRemoteAssistanceInviteMsgResponse , exception: " << e.what();
				result = JSON_PARSE_ERROR;
			}
		}
	}
	else {
		result = UC_FAILED;
	}


	triggerEvent(ICalendarServiceEvent, OnRemoteAssistanceInviteMsgSent(result, conferenceId, msgId, msgSeq, sendTime));

	return result;
}

int32_t CalendarService::SendRemoteAssistanceAcceptMessage(UCID& uid, const std::string& confId, OUT int32_t& msgId) {
	UC_LOG(INFO) << "SendRemoteAssistanceAcceptMessage enter. toUserId = " << uid.GetUserId() << ", conferenceId = " << confId;
	int64_t msgTime = 0;
	HTTPRequest	request;
	MessageProcessor msgProcessor((UCEngine*)_engine);

	if (UC_SUCCEEDED == msgProcessor.CreateAssistAcceptMessage(SINGLE_CHAT, GetCurUser(), uid, confId, request, msgId, msgTime)){
		_engine->SendAsyncRequest(request, MakeRequestCallback(this, &CalendarService::OnSendRemoteAssistanceAcceptMsgResponse, confId, msgId, msgTime));
		return UC_SUCCEEDED;
	}
	return UC_FAILED;
}


int32_t CalendarService::OnSendRemoteAssistanceAcceptMsgResponse(int32_t errorCode, HTTPResponse *response, std::string conferenceId, int32_t msgId, int64_t msgTime) {
	int32_t result = HTTPResponseAssist::GuardStatementForCallBack("OnSendRemoteAssistanceAcceptMsgResponse", errorCode, response);
	UC_LOG(INFO) << "OnSendRemoteAssistanceAcceptMsgResponse enter. Http request result = " << result << ", MsgId:" << msgId << ", conferenceId:" << conferenceId;
	int64_t msgSeq = 0;
	int64_t sendTime = 0;

	if (UC_SUCCEEDED == result) {
		wstringstream wsStream;
		wsStream.str(StringConvert::FromUTF8Str(response->GetContent()).c_str());
		boost::property_tree::wptree jsonRoot;
		boost::property_tree::json_parser::read_json(wsStream, jsonRoot);
		result = jsonRoot.get<int32_t>(L"code");

		if (UC_SUCCEEDED == result) {
			try {
				boost::property_tree::wptree jsonChild = jsonRoot.get_child(L"success_data");
				BOOST_FOREACH(boost::property_tree::wptree::value_type v, jsonChild){
					result = v.second.get<int32_t>(L"code", 0);
					msgId = v.second.get<int32_t>(L"msg_id", 0);
					msgSeq = v.second.get<int64_t>(L"seq", 0);
					sendTime = v.second.get<int64_t>(L"timestamp", 0);
				}

				jsonChild = jsonRoot.get_child(L"failed_data");
				BOOST_FOREACH(boost::property_tree::wptree::value_type v, jsonChild){
					result = v.second.get<int32_t>(L"code", 0);
					msgId = v.second.get<int32_t>(L"msg_id", 0);
					msgSeq = v.second.get<int64_t>(L"seq", 0);
					sendTime = v.second.get<int64_t>(L"timestamp", 0);
				}
			}
			catch (boost::property_tree::ptree_error & e) {
				UC_LOG(ERROR) << "encounter an exception when parse JSON from OnSendRemoteAssistanceAcceptMsgResponse , exception: " << e.what();
				result = JSON_PARSE_ERROR;
			}
		}
	}
	else {
		result = UC_FAILED;
	}


	triggerEvent(ICalendarServiceEvent, OnRemoteAssistanceInviteMsgSent(result, conferenceId, msgId, msgSeq, sendTime));

	return result;
}

int32_t CalendarService::SendRemoteAssistanceRejectMessage(UCID& uid, const std::string& confId, OUT int32_t& msgId) {
	UC_LOG(INFO) << "SendRemoteAssistanceRejectMessage enter. toUserId = " << uid.GetUserId() << ", conferenceId = " << confId;
	int64_t msgTime = 0;
	HTTPRequest	request;
	MessageProcessor msgProcessor((UCEngine*)_engine);

	if (UC_SUCCEEDED == msgProcessor.CreateAssistRejectMessage(SINGLE_CHAT, GetCurUser(), uid, confId, request, msgId, msgTime)){
		_engine->SendAsyncRequest(request, MakeRequestCallback(this, &CalendarService::OnSendRemoteAssistanceRejectMsgResponse, confId, msgId, msgTime));
		return UC_SUCCEEDED;
	}
	return UC_FAILED;
}


int32_t CalendarService::OnSendRemoteAssistanceRejectMsgResponse(int32_t errorCode, HTTPResponse *response, std::string conferenceId, int32_t msgId, int64_t msgTime) {
	int32_t result = HTTPResponseAssist::GuardStatementForCallBack("OnSendRemoteAssistanceRejectMsgResponse", errorCode, response);
	UC_LOG(INFO) << "OnSendRemoteAssistanceRejectMsgResponse enter. Http request result = " << result << ", MsgId:" << msgId << ", conferenceId:" << conferenceId;

	int64_t msgSeq = 0;
	int64_t sendTime = 0;

	if (UC_SUCCEEDED == result) {
		wstringstream wsStream;
		wsStream.str(StringConvert::FromUTF8Str(response->GetContent()).c_str());
		boost::property_tree::wptree jsonRoot;
		boost::property_tree::json_parser::read_json(wsStream, jsonRoot);
		result = jsonRoot.get<int32_t>(L"code");

		if (UC_SUCCEEDED == result) {
			try {
				boost::property_tree::wptree jsonChild = jsonRoot.get_child(L"success_data");
				BOOST_FOREACH(boost::property_tree::wptree::value_type v, jsonChild){
					result = v.second.get<int32_t>(L"code", 0);
					msgId = v.second.get<int32_t>(L"msg_id", 0);
					msgSeq = v.second.get<int64_t>(L"seq", 0);
					sendTime = v.second.get<int64_t>(L"timestamp", 0);
				}

				jsonChild = jsonRoot.get_child(L"failed_data");
				BOOST_FOREACH(boost::property_tree::wptree::value_type v, jsonChild){
					result = v.second.get<int32_t>(L"code", 0);
					msgId = v.second.get<int32_t>(L"msg_id", 0);
					msgSeq = v.second.get<int64_t>(L"seq", 0);
					sendTime = v.second.get<int64_t>(L"timestamp", 0);
				}
			}
			catch (boost::property_tree::ptree_error & e) {
				UC_LOG(ERROR) << "encounter an exception when parse JSON from OnSendRemoteAssistanceRejectMsgResponse , exception: " << e.what();
				result = JSON_PARSE_ERROR;
			}
		}
	}
	else {
		result = UC_FAILED;
	}


	triggerEvent(ICalendarServiceEvent, OnRemoteAssistanceRejectMsgSent(result, conferenceId, msgId, msgSeq, sendTime));

	return result;
}

bool CalendarService::IsNumber(const string& str) {
	for (char ch : str) {
		if (!(ch >= '0' && ch <= '9')) {
			return false;
		}
	}

	return true;
}

void CalendarService::CacheMeeting(const Meeting& meeting) {
	vector<Meeting>::iterator it = _cacheMeetings.begin();
	while ( it != _cacheMeetings.end() ) {
		if (it->GetEventId() == meeting.GetEventId() && it->GetStartTime() == meeting.GetStartTime()
			&& it->GetAuthorizerId() == meeting.GetAuthorizerId() ) {
				//缓存最后一条信息（最新）
				if (meeting.GetLastUpdateTime() > it->GetLastUpdateTime() && it->GetLastUpdateTime() > 0) {
					_cacheMeetings.erase(it);
					break;
				}
				else{
					return;
				}
		}
		it++;
	}

	_cacheMeetings.push_back(meeting);
}

int32_t CalendarService::GetConferenceIdByEventId(int64_t eventId, int64_t startTime /*= 0*/, int32_t authorizerId /*= 0*/) {
	for (const Meeting& m : _cacheMeetings) {
		if (eventId == m.GetEventId() && authorizerId == m.GetAuthorizerId()) {
			if ((startTime > 0 && startTime == m.GetStartTime()) || startTime <= 0) {
				return StringToInt32(m.GetConferneceId());
			}
		}
	}

	return DBManager::GetInstance().GetConferenceIdByEventId(eventId, startTime, authorizerId);
}

bool CalendarService::GetMeetingInfoByEventId(Meeting& meeting, int64_t eventId, int64_t startTime /*= 0*/, int32_t authorizerId /*= 0*/) {
	if (DBManager::GetInstance().GetMeeting(meeting, eventId, startTime, authorizerId) == DBError::DB_SUCCESS) {
		return true;
	}

	if (true == FindInCacheMeeting(eventId, startTime, authorizerId, meeting)) {
		return true;
	}

	return false;
}

std::string CalendarService::GetUserName() {
	std::string displayName = "";
	if (_engine){
		displayName = ((ClientService*)_engine->QueryService(UCClientService))->GetLoginInfo().GetDisplayName();
	}
	return 	displayName;
}

void CalendarService::UpdateCahcheMeetingStatus(uc::AttendeeStatus status, int64_t eventId, int64_t startTime /*= 0*/, int32_t authorizerId /*= 0*/) {
	stringstream ssUserId;
	if (authorizerId > 0 )
	{
		ssUserId << authorizerId;
	}
	else{
		ssUserId << GetUserId();
	}
	std::string sCurrentAccount = GetAccount();

	for (Meeting& m : _cacheMeetings) 
	{
		if (eventId == m.GetEventId() && authorizerId == m.GetAuthorizerId())
		{
			if ((startTime > 0 && startTime == m.GetStartTime()) || startTime <= 0)
			{
				vector<Attendee> atts = m.GetAttendees();
				for (Attendee& ad : atts)
				{
					if (ad.GetAccount().compare(ssUserId.str()) == 0 )
					{
						ad.SetStatus(status);
						m.SetAttendees(atts);
						return;
					}
				}
			}
		}
	}
}

int32_t CalendarService::GetMeetingSiteId() {
	int32_t siteId = 60001;
	ClientService* clientService = (ClientService*)_engine->QueryService(UCClientService);
	if (clientService != NULL) {
		if (clientService->GetLocalMeetingClientVersion() == 0) {
			siteId = 60000;
		}
	}

	return siteId;
}

int32_t CalendarService::RevocationMeetingMessage(int64_t groupId, int64_t eventId, int64_t seqId, int32_t& msgId){
	msgId = DBManager::GetInstance().GetMessageID();

	HTTPRequest     request;
	stringstream	sStream;
	string         	targetURL;

	targetURL = ((UCEngine*)_engine)->GetRequestURL(((ClientService*)_engine->QueryService(UCClientService))->GetLoginInfo().GetUccServerDomain(),
		MESSAGE_REVOCATION_METHOD);

	request.Init(targetURL, HTTP_METHOD_POST);
	request.AddHeaderInfo("Content-Type", "application/x-www-form-urlencoded");

	sStream << "session_id=" << ((ClientService*)_engine->QueryService(UCClientService))->GetLoginInfo().GetSessionId() <<
		"&user_id=" << GetCurUser().GetUserId() << "&to_user_id=" << groupId << "&type=" << 1 << "&seq=" << seqId;

	request.SetBody(sStream.str());

	_engine->SendAsyncRequest(request, MakeRequestCallback(this, &CalendarService::OnMeetingMessageRevocationResponse, eventId, groupId,seqId, msgId));

	return UC_SUCCEEDED;
}

int32_t CalendarService::OnMeetingMessageRevocationResponse(int32_t errorCode, HTTPResponse *response, int64_t eventId, int64_t groupId,int64_t seqId, int32_t msgId)
{
	UC_LOG(INFO) << "OnMeetingMessageRevocationResponse eventId:" << eventId << " SeqId:" << seqId <<" MsgId:" << msgId
		<< " errorCode:" << errorCode << " response:" << (response != NULL ? response->GetContent() : "");

	int32_t result = UC_FAILED;
	int64_t sendTime = seqId;//值相同

	if (UC_SUCCEEDED == errorCode && NULL != response && 200 == response->GetStatusCode()) {
		wstringstream wsStream;
		wsStream.str(StringConvert::FromUTF8Str(response->GetContent()).c_str());
		boost::property_tree::wptree jsonRoot;
		try{
			boost::property_tree::json_parser::read_json(wsStream, jsonRoot);
			result = jsonRoot.get<int32_t>(L"code");
		}
		catch (boost::property_tree::ptree_error & e) {
			UC_LOG(ERROR) << "encounter an exception when parse json from OnMeetingMessageRevocationResponse , exception: " << e.what();
			result = JSON_PARSE_ERROR;
		}
	}
	else{
		result = errorCode != UC_SUCCEEDED ? errorCode : (NULL != response && 200 != response->GetStatusCode() ? response->GetStatusCode() : UC_FAILED);
	}

	if (UC_SUCCEEDED == result)
	{
		theStore.GetMsgStore()->UpdateRevocationMsgFromDB(groupId, STATE_REVOCATION, CONFERENCE_CHAT, sendTime);
		theStore.GetMsgStore()->UpdateRevocationMessageState(groupId, sendTime, CONFERENCE_CHAT);
	}

	triggerEvent(ICalendarServiceEvent, OnMeetingMessageRevocation(result, seqId, msgId, sendTime, eventId));
	return result;
}

int32_t CalendarService::CreateMeetingRoom(const std::vector<Attendee>& attendees, const std::string& title, const std::string& remarks, const std::string& password, int64_t startTime, int64_t endTime, int64_t roomHostId, int64_t orgId, int32_t isSetPassword /*= 1*/, int32_t defaultAttendsVisible /*= 1*/, int32_t defaultAttendsJoin /*= 1*/) {
	UC_LOG(INFO) << "CreateMeetingRoom enter. orgId = " << orgId << ", roomHostId = " << roomHostId << ", title = " << title;

	HTTPRequest request;
	MeetingRoom room;
	room.SetAttendees(attendees);
	room.SetTitle(StringConvert::JsonEscape(title));
	room.SetRemarks(StringConvert::JsonEscape(remarks));
	room.SetRoomPassword(password);
	room.SetStartTime(startTime);
	room.SetEndTime(endTime);
	room.SetRoomHostId(roomHostId);
	room.SetOrgId(orgId);
	room.SetDefaultAttendeesJoin(defaultAttendsJoin);
	room.SetDefaultAttendeesVisible(defaultAttendsVisible);
	room.SetIsSetPassword(isSetPassword);
	_calendarRequestFactory->MakeRequestOfCreateMeetingRoom(room, request);
	_engine->SendAsyncRequest(request, MakeRequestCallback(this, &CalendarService::OnMeetingRoomCreated, room));

	UC_LOG(INFO) << "CreateMeetingRoom over. request URL = " << request.GetURL() << ", request body = " << request.GetBody();
	return UC_SUCCEEDED;

}

int32_t CalendarService::OnMeetingRoomCreated(int32_t errorCode, HTTPResponse* response, MeetingRoom room)
{
	int32_t result = HTTPResponseAssist::GuardStatementForCallBack("OnMeetingRoomCreated", errorCode, response);
	UC_LOG(INFO) << "OnMeetingRoomCreated enter. Http request result = " << result;

	if (result != UC_SUCCEEDED) {
		triggerEvent(ICalendarServiceEvent, OnMeetingRoomCreated(result, room));
		return result;
	}

	//解析json
	int32_t code = -1;
	try {
		wstringstream jsonStream;
		boost::property_tree::wptree ptRoot;
		jsonStream.str(StringConvert::FromUTF8Str(response->GetContent()).c_str());
		boost::property_tree::json_parser::read_json(jsonStream, ptRoot);
		code = ptRoot.get<int32_t>(KEY_CODE, code);

		if (code != 0) {
			//server返回失败
			UC_LOG(ERROR) << "OnMeetingRoomCreated error, server response error code = " << code;
			triggerEvent(ICalendarServiceEvent, OnMeetingRoomCreated(code, room));
			return code;
		}

		boost::property_tree::wptree ptData;
		boost::property_tree::wptree ptDef;
		ptData = ptRoot.get_child(KEY_DATA, ptDef);
		if (ptData.empty()) {
			//数据为空
			UC_LOG(ERROR) << "OnMeetingRoomCreated error, server response data is null.";
			triggerEvent(ICalendarServiceEvent, OnMeetingRoomCreated(code, room));
			return code;
		}

		room.SetConferenceId(StringConvert::ToUTF8Str(ptData.get<std::wstring>((L"conferenceId"), L"")));
		room.SetRoomId(StringConvert::ToUTF8Str(ptData.get<std::wstring>((L"roomId"), L"")));
		room.SetHostPassword(StringConvert::ToUTF8Str(ptData.get<std::wstring>((KEY_HOSTPASSWORD), L"")));
		room.SetAttendeePassword(StringConvert::ToUTF8Str(ptData.get<std::wstring>((KEY_ATTENDEEPASSWORD), L"")));
		room.SetBillingCode(StringConvert::ToUTF8Str(ptData.get<std::wstring>((L"billingCode"), L"")));
	}
	catch (boost::property_tree::ptree_error & e) {
		UC_LOG(ERROR) << "encounter an exception when parse JSON from OnMeetingRoomCreated , exception: " << e.what();
		triggerEvent(ICalendarServiceEvent, OnMeetingRoomCreated(JSON_PARSE_ERROR, room));
		return JSON_PARSE_ERROR;
	}

	//回调上层
	triggerEvent(ICalendarServiceEvent, OnMeetingRoomCreated(code, room));

	UC_LOG(INFO) << "OnMeetingRoomCreated success.";
	return code;
}

int32_t CalendarService::UpdateMeetingRoom(std::string& roomId, const std::vector<Attendee>& attendees, const std::string& title, const std::string& remarks, const std::string& password, int64_t startTime, int64_t endTime, int64_t roomHostId, int64_t orgId, int32_t isSetPassword /*= 1*/, int32_t defaultAttendsVisible /*= 1*/, int32_t defaultAttendsJoin /*= 1*/)
{
	UC_LOG(INFO) << "UpdateMeetingRoom enter. roomId = " << roomId << ", orgId = " << orgId << ",  roomHostId = " << roomHostId << ", title = " << title;

	HTTPRequest request;
	MeetingRoom room;
	room.SetRoomId(roomId);
	room.SetAttendees(attendees);
	room.SetTitle(StringConvert::JsonEscape(title));
	room.SetRemarks(StringConvert::JsonEscape(remarks));
	room.SetRoomPassword(password);
	room.SetStartTime(startTime);
	room.SetEndTime(endTime);
	room.SetRoomHostId(roomHostId);
	room.SetOrgId(orgId);
	room.SetDefaultAttendeesJoin(defaultAttendsJoin);
	room.SetDefaultAttendeesVisible(defaultAttendsVisible);
	room.SetIsSetPassword(isSetPassword);
	_calendarRequestFactory->MakeRequestOfUpdateMeetingRoom(room, request);
	_engine->SendAsyncRequest(request, MakeRequestCallback(this, &CalendarService::OnMeetingRoomUpdated, room));

	UC_LOG(INFO) << "UpdateMeetingRoom over. request URL = " << request.GetURL() << ", request body = " << request.GetBody();
	return UC_SUCCEEDED;

}


int32_t CalendarService::OnMeetingRoomUpdated(int32_t errorCode, HTTPResponse* response, MeetingRoom room)
{
	int32_t result = HTTPResponseAssist::GuardStatementForCallBack("OnMeetingRoomUpdated", errorCode, response);
	UC_LOG(INFO) << "OnMeetingRoomUpdated enter. Http request result = " << result;

	if (result != UC_SUCCEEDED) {
		triggerEvent(ICalendarServiceEvent, OnMeetingRoomUpdated(result, room));
		return result;
	}

	//解析json
	int32_t code = -1;
	try {
		wstringstream jsonStream;
		boost::property_tree::wptree ptRoot;
		jsonStream.str(StringConvert::FromUTF8Str(response->GetContent()).c_str());
		boost::property_tree::json_parser::read_json(jsonStream, ptRoot);
		code = ptRoot.get<int32_t>(KEY_CODE, code);

		if (code != 0) {
			//server返回失败
			UC_LOG(ERROR) << "OnMeetingRoomUpdated error, server response error code = " << code;
			triggerEvent(ICalendarServiceEvent, OnMeetingRoomUpdated(code, room));
			return code;
		}

		boost::property_tree::wptree ptData;
		boost::property_tree::wptree ptDef;
		ptData = ptRoot.get_child(KEY_DATA, ptDef);
		if (ptData.empty()) {
			//数据为空
			UC_LOG(ERROR) << "OnMeetingRoomUpdated error, server response data is null.";
			triggerEvent(ICalendarServiceEvent, OnMeetingRoomUpdated(code, room));
			return code;
		}

		room.SetConferenceId(StringConvert::ToUTF8Str(ptData.get<std::wstring>((L"conferenceId"), L"")));
		room.SetRoomId(StringConvert::ToUTF8Str(ptData.get<std::wstring>((L"roomId"), L"")));
		room.SetHostPassword(StringConvert::ToUTF8Str(ptData.get<std::wstring>((KEY_HOSTPASSWORD), L"")));
		room.SetAttendeePassword(StringConvert::ToUTF8Str(ptData.get<std::wstring>((KEY_ATTENDEEPASSWORD), L"")));
		room.SetBillingCode(StringConvert::ToUTF8Str(ptData.get<std::wstring>((L"billingCode"), L"")));
	}
	catch (boost::property_tree::ptree_error & e) {
		UC_LOG(ERROR) << "encounter an exception when parse JSON from OnMeetingRoomUpdated , exception: " << e.what();
		triggerEvent(ICalendarServiceEvent, OnMeetingRoomUpdated(JSON_PARSE_ERROR, room));
		return JSON_PARSE_ERROR;
	}

	//回调上层
	triggerEvent(ICalendarServiceEvent, OnMeetingRoomUpdated(code, room));

	UC_LOG(INFO) << "OnMeetingRoomUpdated success.";
	return code;
}

int32_t CalendarService::CreateMeetingInMeetingRoom(
	const std::vector<Attendee>& attendees,
	const std::string& roomId,
	const std::string& title,
	const std::string& summary,
	int64_t startTime,
	int32_t duration){
	UC_LOG(INFO) << "CreateMeetingInMeetingRoom enter. roomId = " << roomId << ",  startTime = " << startTime << ", title: " << title;

	HTTPRequest request;
	Meeting meeting;
	meeting.SetAttendees(attendees);
	meeting.SetTitle(StringConvert::URLEncode(title));
	meeting.SetSummary(StringConvert::URLEncode(summary));
	meeting.SetStartTime(startTime);
	meeting.SetDuration(duration);
	meeting.SetHostId(GetUserId());
	_calendarRequestFactory->MakeRequestOfCreateMeetingInMeetingRoom(meeting, roomId, request);
	_engine->SendAsyncRequest(request, MakeRequestCallback(this, &CalendarService::OnMeetingCreatedInMeetingRoom, roomId, meeting));

	UC_LOG(INFO) << "CreateMeetingInMeetingRoom over. request URL = " << request.GetURL() << ", request body = " << request.GetBody();
	return UC_SUCCEEDED;
}


int32_t CalendarService::OnMeetingCreatedInMeetingRoom(int32_t errorCode, HTTPResponse* response, std::string roomId, Meeting meeting)
{
	int32_t result = HTTPResponseAssist::GuardStatementForCallBack("OnMeetingCreatedInMeetingRoom", errorCode, response);
	UC_LOG(INFO) << "OnMeetingCreatedInMeetingRoom enter. Http request result = " << result;

	if (result != UC_SUCCEEDED) {
		triggerEvent(ICalendarServiceEvent, OnMeetingCreatedInMeetingRoom(result, roomId, meeting));
		return result;
	}

	//解析json
	int32_t code = -1;
	try {
		wstringstream jsonStream;
		boost::property_tree::wptree ptRoot;
		jsonStream.str(StringConvert::FromUTF8Str(response->GetContent()).c_str());
		boost::property_tree::json_parser::read_json(jsonStream, ptRoot);
		code = ptRoot.get<int32_t>(KEY_CODE, code);

		if (code != 0) {
			//server返回失败
			UC_LOG(ERROR) << "OnMeetingCreatedInMeetingRoom error, server response error code = " << code;
			triggerEvent(ICalendarServiceEvent, OnMeetingCreatedInMeetingRoom(code, roomId, meeting));
			return code;
		}

		boost::property_tree::wptree ptData;
		boost::property_tree::wptree ptDef;
		ptData = ptRoot.get_child(KEY_DATA, ptDef);
		if (ptData.empty()) {
			//数据为空
			UC_LOG(ERROR) << "OnMeetingCreatedInMeetingRoom error, server response data is null.";
			triggerEvent(ICalendarServiceEvent, OnMeetingCreatedInMeetingRoom(code, roomId, meeting));
			return code;
		}

		meeting.SetConferenceId(StringConvert::ToUTF8Str(ptData.get<std::wstring>((L"conferenceId"), L"")));
		meeting.SetEventId(ptData.get<int64_t>((KEY_EVENTID), 0));
		meeting.SetGroupId(ptData.get<int64_t>((KEY_GROUPID), 0));
		meeting.SetHostPassword(StringConvert::ToUTF8Str(ptData.get<std::wstring>((KEY_HOSTPASSWORD), L"")));
		meeting.SetAttendeePassword(StringConvert::ToUTF8Str(ptData.get<std::wstring>((KEY_ATTENDEEPASSWORD), L"")));
		meeting.SetBillingCode(StringConvert::ToUTF8Str(ptData.get<std::wstring>((KEY_BILLINGCODE), L"")));
	}
	catch (boost::property_tree::ptree_error & e) {
		UC_LOG(ERROR) << "encounter an exception when parse JSON from OnMeetingCreatedInMeetingRoom , exception: " << e.what();
		triggerEvent(ICalendarServiceEvent, OnMeetingCreatedInMeetingRoom(JSON_PARSE_ERROR, roomId, meeting));
		return JSON_PARSE_ERROR;
	}

	//回调上层
	triggerEvent(ICalendarServiceEvent, OnMeetingCreatedInMeetingRoom(code, roomId, meeting));

	UC_LOG(INFO) << "OnMeetingCreatedInMeetingRoom success.";
	return code;
}


int32_t CalendarService::GetMeetingRoomList(int64_t orgId)
{
	UC_LOG(INFO) << "GetMeetingRoomList enter. orgId = " << orgId;

	HTTPRequest request;
	_calendarRequestFactory->MakeRequestOfGetMeetingRoomList(orgId, request);
	_engine->SendAsyncRequest(request, MakeRequestCallback(this, &CalendarService::OnMeetingRoomListReceived, orgId));

	UC_LOG(INFO) << "GetMeetingRoomList over. request URL = " << request.GetURL() << ", request body = " << request.GetBody();
	return UC_SUCCEEDED;
}

int32_t CalendarService::OnMeetingRoomListReceived(int32_t errorCode, HTTPResponse* response, int64_t orgId)
{
	int32_t result = HTTPResponseAssist::GuardStatementForCallBack("OnMeetingRoomListReceived", errorCode, response);
	UC_LOG(INFO) << "OnMeetingRoomListReceived enter. Http request result = " << result;
	std::list<MeetingRoom> rooms;
	rooms.clear();
	if (result != UC_SUCCEEDED) {
		triggerEvent(ICalendarServiceEvent, OnMeetingRoomListReceived(result, orgId, rooms));
		return result;
	}

	//解析json
	int32_t code = -1;
	try {
		wstringstream jsonStream;
		boost::property_tree::wptree ptRoot;
		jsonStream.str(StringConvert::FromUTF8Str(response->GetContent()).c_str());
		boost::property_tree::json_parser::read_json(jsonStream, ptRoot);
		code = ptRoot.get<int32_t>(KEY_CODE, code);
		//int64_t timestamp = ptRoot.get<int64_t>(L"timestamp", 0);

		if (code != 0) {
			//server返回失败
			UC_LOG(ERROR) << "OnMeetingRoomListReceived error, server response error code = " << code;
			triggerEvent(ICalendarServiceEvent, OnMeetingRoomListReceived(code, orgId, rooms));
			return code;
		}

		boost::property_tree::wptree ptData;
		boost::property_tree::wptree ptDef;
		ptData = ptRoot.get_child(KEY_DATA, ptDef);
		if (ptData.empty()) {
			//数据为空
			UC_LOG(ERROR) << "OnMeetingRoomListReceived error, server response data is null.";
			triggerEvent(ICalendarServiceEvent, OnMeetingRoomListReceived(code, orgId, rooms));
			return code;
		}

		BOOST_FOREACH(boost::property_tree::wptree::value_type v, ptData){
			MeetingRoom room;
			boost::property_tree::wptree ptAttendees = v.second.get_child(L"attendLists");
			std::wstring account;
			std::vector<Attendee> vAtt;
			BOOST_FOREACH(const boost::property_tree::wptree::value_type &vElement, ptAttendees){
				boost::property_tree::wptree jsonValue = vElement.second;
				account = jsonValue.get_value<std::wstring>();
				if (!account.empty()) {
					Attendee ad;
					ad.SetAccount(StringConvert::ToUTF8Str(account));
					int pos = account.find('@');
					if (pos > 0){
						ad.SetType(AttendeeType::AT_MAIL);
					}
					vAtt.push_back(ad);
				}
			}
			room.SetAttendees(vAtt);
			room.SetRoomId(StringConvert::ToUTF8Str(v.second.get<std::wstring>((L"roomId"), L"")));
			room.SetTitle(StringConvert::ToUTF8Str(v.second.get<std::wstring>((L"roomTitle"), L"")));
			room.SetRemarks(StringConvert::ToUTF8Str(v.second.get<std::wstring>((L"remarks"), L"")));
			room.SetRoomPassword(StringConvert::ToUTF8Str(v.second.get<std::wstring>((L"roomPassword"), L"")));
			room.SetDefaultAttendeesVisible(v.second.get<int32_t>(L"defaultAttendsVisible", 0));
			room.SetDefaultAttendeesJoin(v.second.get<int32_t>(L"defaultAttendsJoin", 0));
			room.SetStartTime(v.second.get<int64_t>(L"startTime", 0));
			room.SetEndTime(v.second.get<int64_t>(L"endTime", 0));
			room.SetIsSetPassword(v.second.get<int32_t>(L"isSetPassword", 0));
			room.SetRoomHostId(v.second.get<int64_t>(L"roomHostId", 0));
			room.SetOrgId(v.second.get<int64_t>(L"orgId ", 0));
			room.SetConferenceId(StringConvert::ToUTF8Str(v.second.get<std::wstring>((L"conferenceId"), L"")));
			room.SetRoomId(StringConvert::ToUTF8Str(v.second.get<std::wstring>((L"roomId"), L"")));
			room.SetHostPassword(StringConvert::ToUTF8Str(v.second.get<std::wstring>((KEY_HOSTPASSWORD), L"")));
			room.SetAttendeePassword(StringConvert::ToUTF8Str(v.second.get<std::wstring>((KEY_ATTENDEEPASSWORD), L"")));
			room.SetBillingCode(StringConvert::ToUTF8Str(v.second.get<std::wstring>((L"billingCode"), L"")));
			rooms.push_back(room);
		}
	}
	catch (boost::property_tree::ptree_error & e) {
		UC_LOG(ERROR) << "encounter an exception when parse JSON from OnMeetingRoomListReceived , exception: " << e.what();
		triggerEvent(ICalendarServiceEvent, OnMeetingRoomListReceived(JSON_PARSE_ERROR, orgId, rooms));
		return JSON_PARSE_ERROR;
	}

	//回调上层
	triggerEvent(ICalendarServiceEvent, OnMeetingRoomListReceived(code, orgId, rooms));

	UC_LOG(INFO) << "OnMeetingRoomListReceived success.";
	return code;
}


int32_t CalendarService::CheckJoinMeetingRoom(const std::string& roomId)
{
	UC_LOG(INFO) << "CheckJoinMeetingRoom enter. roomId = " << roomId;

	HTTPRequest request;
	_calendarRequestFactory->MakeRequestOfCheckJoinMeetingRoom(roomId, request);
	_engine->SendAsyncRequest(request, MakeRequestCallback(this, &CalendarService::OnCheckJoinMeetingRoom, roomId));

	UC_LOG(INFO) << "CheckJoinMeetingRoom over. request URL = " << request.GetURL() << ", request body = " << request.GetBody();
	return UC_SUCCEEDED;
}


int32_t CalendarService::OnCheckJoinMeetingRoom(int32_t errorCode, HTTPResponse* response, std::string roomId)
{
	int32_t result = HTTPResponseAssist::GuardStatementForCallBack("OnCheckJoinMeetingRoom", errorCode, response);
	UC_LOG(INFO) << "OnCheckJoinMeetingRoom enter. Http request result = " << result;

	int32_t hasPassword = 0;
	int32_t isValid = 0;
	int32_t mayJoin = 0;
	if (result != UC_SUCCEEDED) {
		triggerEvent(ICalendarServiceEvent, OnCheckJoinMeetingRoom(result, roomId, hasPassword, isValid, mayJoin));
		return result;
	}

	//解析json
	int32_t code = -1;
	try {
		wstringstream jsonStream;
		boost::property_tree::wptree ptRoot;
		jsonStream.str(StringConvert::FromUTF8Str(response->GetContent()).c_str());
		boost::property_tree::json_parser::read_json(jsonStream, ptRoot);
		code = ptRoot.get<int32_t>(KEY_CODE, code);

		if (code != 0) {
			//server返回失败
			UC_LOG(ERROR) << "OnCheckJoinMeetingRoom error, server response error code = " << code;
			triggerEvent(ICalendarServiceEvent, OnCheckJoinMeetingRoom(code, roomId, hasPassword, isValid, mayJoin));
			return code;
		}

		boost::property_tree::wptree ptData;
		boost::property_tree::wptree ptDef;
		ptData = ptRoot.get_child(KEY_DATA, ptDef);
		if (ptData.empty()) {
			//数据为空
			UC_LOG(ERROR) << "OnCheckJoinMeetingRoom error, server response data is null.";
			triggerEvent(ICalendarServiceEvent, OnCheckJoinMeetingRoom(code, roomId, hasPassword, isValid, mayJoin));
			return code;
		}

		hasPassword = ptData.get<int32_t>(L"hasPassword", 0);
		isValid = ptData.get<int32_t>(L"isValid", 0);
		mayJoin = ptData.get<int32_t>(L"mayJoin", 0);
	}
	catch (boost::property_tree::ptree_error & e) {
		UC_LOG(ERROR) << "encounter an exception when parse JSON from OnCheckJoinMeetingRoom , exception: " << e.what();
		triggerEvent(ICalendarServiceEvent, OnCheckJoinMeetingRoom(JSON_PARSE_ERROR, roomId, hasPassword, isValid, mayJoin));
		return JSON_PARSE_ERROR;
	}

	//回调上层
	triggerEvent(ICalendarServiceEvent, OnCheckJoinMeetingRoom(code, roomId, hasPassword, isValid, mayJoin));

	UC_LOG(INFO) << "OnCheckJoinMeetingRoom success.";
	return code;
}


int32_t CalendarService::CheckMeetingRoomPassword(const std::string& roomId, const std::string& password)
{
	UC_LOG(INFO) << "CheckMeetingRommePassword enter. roomId = " << roomId;

	HTTPRequest request;
	_calendarRequestFactory->MakeRequestOfCheckMeetingRoomPassword(roomId, password, request);
	_engine->SendAsyncRequest(request, MakeRequestCallback(this, &CalendarService::OnCheckMeetingRoomPassword, roomId, password));

	UC_LOG(INFO) << "CheckMeetingRommePassword over. request URL = " << request.GetURL() << ", request body = " << request.GetBody();
	return UC_SUCCEEDED;
}


int32_t CalendarService::OnCheckMeetingRoomPassword(int32_t errorCode, HTTPResponse* response, std::string roomId, std::string password)
{
	int32_t result = HTTPResponseAssist::GuardStatementForCallBack("OnCheckMeetingRoomPassword", errorCode, response);
	UC_LOG(INFO) << "OnCheckMeetingRoomPassword enter. Http request result = " << result;
	int32_t value = 0;
	if (result != UC_SUCCEEDED) {
		triggerEvent(ICalendarServiceEvent, OnCheckMeetingRoomPassword(result, roomId, password, value));
		return result;
	}

	//解析json
	int32_t code = -1;
	try {
		wstringstream jsonStream;
		boost::property_tree::wptree ptRoot;
		jsonStream.str(StringConvert::FromUTF8Str(response->GetContent()).c_str());
		boost::property_tree::json_parser::read_json(jsonStream, ptRoot);
		code = ptRoot.get<int32_t>(KEY_CODE, code);

		if (code != 0) {
			//server返回失败
			UC_LOG(ERROR) << "OnCheckMeetingRoomPassword error, server response error code = " << code;
			triggerEvent(ICalendarServiceEvent, OnCheckMeetingRoomPassword(code, roomId, password, value));
			return code;
		}

		boost::property_tree::wptree ptData;
		boost::property_tree::wptree ptDef;

		value = ptRoot.get<int32_t>(KEY_DATA, value);
// 		ptData = ptRoot.get_child(KEY_DATA, ptDef);
// 		if (ptData.empty()) {
// 			//数据为空
// 			UC_LOG(ERROR) << "OnCheckMeetingRoomPassword error, server response data is null.";
// 			triggerEvent(ICalendarServiceEvent, OnCheckMeetingRoomPassword(code, roomId, password));
// 			return code;
// 		}

	}
	catch (boost::property_tree::ptree_error & e) {
		UC_LOG(ERROR) << "encounter an exception when parse JSON from OnCheckMeetingRoomPassword , exception: " << e.what();
		triggerEvent(ICalendarServiceEvent, OnCheckMeetingRoomPassword(JSON_PARSE_ERROR, roomId, password, value));
		return JSON_PARSE_ERROR;
	}

	//回调上层
	triggerEvent(ICalendarServiceEvent, OnCheckMeetingRoomPassword(code, roomId, password, value));

	UC_LOG(INFO) << "OnCheckMeetingRoomPassword success.";
	return code;
}

int32_t CalendarService::DeleteMeetingRoom(const std::string& roomId)
{
	UC_LOG(INFO) << "DeleteMeetingRoom enter. roomId = " << roomId;

	HTTPRequest request;
	_calendarRequestFactory->MakeRequestOfDeleteMeetingRoom(roomId, request);
	_engine->SendAsyncRequest(request, MakeRequestCallback(this, &CalendarService::OnMeetingRoomDeleted, roomId));

	UC_LOG(INFO) << "DeleteMeetingRoom over. request URL = " << request.GetURL() << ", request body = " << request.GetBody();
	return UC_SUCCEEDED;
}

int32_t CalendarService::OnMeetingRoomDeleted(int32_t errorCode, HTTPResponse* response, std::string roomId)
{
	int32_t result = HTTPResponseAssist::GuardStatementForCallBack("OnMeetingRoomDeleted", errorCode, response);
	UC_LOG(INFO) << "OnMeetingRoomDeleted enter. Http request result = " << result;

	if (result != UC_SUCCEEDED) {
		triggerEvent(ICalendarServiceEvent, OnMeetingRoomDeleted(result, roomId));
		return result;
	}

	//解析json
	int32_t code = -1;
	try {
		wstringstream jsonStream;
		boost::property_tree::wptree ptRoot;
		jsonStream.str(StringConvert::FromUTF8Str(response->GetContent()).c_str());
		boost::property_tree::json_parser::read_json(jsonStream, ptRoot);
		code = ptRoot.get<int32_t>(KEY_CODE, code);

		if (code != 0) {
			//server返回失败
			UC_LOG(ERROR) << "OnMeetingRoomDeleted error, server response error code = " << code;
			triggerEvent(ICalendarServiceEvent, OnMeetingRoomDeleted(code, roomId));
			return code;
		}

		boost::property_tree::wptree ptData;
		boost::property_tree::wptree ptDef;
		ptData = ptRoot.get_child(KEY_DATA, ptDef);
		if (ptData.empty()) {
			//数据为空
			UC_LOG(ERROR) << "OnMeetingRoomDeleted error, server response data is null.";
			triggerEvent(ICalendarServiceEvent, OnMeetingRoomDeleted(code, roomId));
			return code;
		}

	}
	catch (boost::property_tree::ptree_error & e) {
		UC_LOG(ERROR) << "encounter an exception when parse JSON from OnMeetingRoomDeleted , exception: " << e.what();
		triggerEvent(ICalendarServiceEvent, OnMeetingRoomDeleted(JSON_PARSE_ERROR, roomId));
		return JSON_PARSE_ERROR;
	}

	//回调上层
	triggerEvent(ICalendarServiceEvent, OnMeetingRoomDeleted(code, roomId));

	UC_LOG(INFO) << "OnMeetingRoomDeleted success.";
	return code;
}

int32_t CalendarService::CheckRightsOfCreateRoom() {
	UC_LOG(INFO) << "CheckRightsOfCreateRoom enter.";

	HTTPRequest request;
	_calendarRequestFactory->MakeRequestOfCheckRights(request);
	_engine->SendAsyncRequest(request, MakeRequestCallback(this, &CalendarService::OnRightsChecked));

	UC_LOG(INFO) << "CheckRightsOfCreateRoom over. request URL = " << request.GetURL() << ", request body = " << request.GetBody();
	return UC_SUCCEEDED;

}

int32_t CalendarService::OnRightsChecked(int32_t errorCode, HTTPResponse* response)
{
	int32_t result = HTTPResponseAssist::GuardStatementForCallBack("OnRightsChecked", errorCode, response);
	UC_LOG(INFO) << "OnRightsChecked enter. Http request result = " << result;
	//CALENDAR_SERVICE_RIGHTS_CHECKED_EVENT

	int32_t value = 0;
	if (result != UC_SUCCEEDED) {
		triggerEvent(ICalendarServiceEvent, OnRightsChecked(result));
		return result;
	}

	//解析json
	int32_t code = -1;
	try {
		wstringstream jsonStream;
		boost::property_tree::wptree ptRoot;
		jsonStream.str(StringConvert::FromUTF8Str(response->GetContent()).c_str());
		boost::property_tree::json_parser::read_json(jsonStream, ptRoot);
		code = ptRoot.get<int32_t>(KEY_CODE, code);

		if (code != 0) {
			//server返回失败
			UC_LOG(ERROR) << "OnRightsChecked error, server response error code = " << code;
			triggerEvent(ICalendarServiceEvent, OnRightsChecked(code));
			return code;
		}
	}
	catch (boost::property_tree::ptree_error & e) {
		UC_LOG(ERROR) << "encounter an exception when parse JSON from OnRightsChecked , exception: " << e.what();
		triggerEvent(ICalendarServiceEvent, OnRightsChecked(JSON_PARSE_ERROR));
		return JSON_PARSE_ERROR;
	}

	//回调上层
	triggerEvent(ICalendarServiceEvent, OnRightsChecked(code));

	UC_LOG(INFO) << "OnRightsChecked success.";
	return code;
}

int32_t CalendarService::ConvertAddressStr2Obj(const std::string& addressStr, Address& addressObj) {
	if (addressStr.empty()) {
		return UC_FAILED;
	}

	try {
		wstringstream jsonStream;
		boost::property_tree::wptree ptRoot;
		jsonStream.str(StringConvert::FromUTF8Str(addressStr.c_str()));
		boost::property_tree::json_parser::read_json(jsonStream, ptRoot);
		boost::property_tree::wptree ptAddress;
		boost::property_tree::wptree ptDef;
		ptAddress = ptRoot.get_child(L"address", ptDef);
		if (!ptAddress.empty())
		{
			addressObj.SetLongitude(StringConvert::ToUTF8Str(ptRoot.get<std::wstring>((L"longitude"), L"")));
			addressObj.SetLatitude(StringConvert::ToUTF8Str(ptRoot.get<std::wstring>((L"latitude"), L"")));
			addressObj.SetName(StringConvert::ToUTF8Str(ptRoot.get<std::wstring>((L"name"), L"")));
			addressObj.SetLocation(StringConvert::ToUTF8Str(ptRoot.get<std::wstring>((L"location"), L"")));
			addressObj.SetType(ptRoot.get<int8_t>((L"type"), 0));
		}
	}
	catch (boost::property_tree::ptree_error & e) {
		UC_LOG(ERROR) << "encounter an exception when parse JSON from Address , exception: " << e.what();
		return JSON_PARSE_ERROR;
	}

	return UC_SUCCEEDED;
}

void CalendarService::SetMeetingAddressValueFromExtrasInfo(Meeting& meeting)
{
	Address add;
    std::string extrasInfo = meeting.GetExtrasInfo();
    ConvertAddressStr2Obj(StringConvert::URLDecode(StringConvert::Replace(extrasInfo, " ", "")), add);
	meeting.SetAddress(add);
}

void CalendarService::SetMeetingAddressValueFromExtrasInfo(vector<Meeting>& meetings)
{
	for (Meeting& m : meetings) {
		SetMeetingAddressValueFromExtrasInfo(m);
	}
}

int32_t CalendarService::GetUserBusyFreeInfo(const std::vector<int32_t>& userIdList, int64_t startTime, int64_t endTime, int32_t shareUserId, OUT int32_t& requestId) {

	UC_LOG(INFO) << "GetUserBusyFreeInfo enter.";
	requestId = MakeRequestId();
	HTTPRequest request;
	_calendarRequestFactory->MakeRequestOfGetUserBusyFreeInfo(userIdList, startTime, endTime, shareUserId, request);
	_engine->SendAsyncRequest(request, MakeRequestCallback(this, &CalendarService::OnUserBusyFreeInfoReceived, startTime, endTime, userIdList, shareUserId, requestId));
	UC_LOG(INFO) << "GetUserBusyFreeInfo over. request URL = " << request.GetURL() << ", request body = " << request.GetBody();
	return UC_SUCCEEDED;
}

int32_t CalendarService::OnUserBusyFreeInfoReceived(int32_t errorCode, HTTPResponse* response, int64_t startTime, int64_t endTime, std::vector<int32_t> userIdList, int32_t shareUserId, int32_t requestId) {
	int32_t result = HTTPResponseAssist::GuardStatementForCallBack("OnUserBusyFreeInfoReceived", errorCode, response);
	UC_LOG(INFO) << "OnUserBusyFreeInfoReceived enter. Http request result = " << result << ", startTime = " << startTime << ", endTime = " << endTime;
	std::vector<UserBusyFreeInfo> vUserSchedule;
	if (result != UC_SUCCEEDED) {
		triggerEvent(ICalendarServiceEvent, OnUserBusyFreeInfoReceived(result, requestId, vUserSchedule));
		return result;
	}

	//解析json
	int32_t code = -1;
	try {
		wstringstream jsonStream;
		boost::property_tree::wptree ptRoot;
		jsonStream.str(StringConvert::FromUTF8Str(response->GetContent()).c_str());
		boost::property_tree::json_parser::read_json(jsonStream, ptRoot);
		code = ptRoot.get<int32_t>((KEY_CODE), code);
		if (code != 0) {
			std::wstring request = ptRoot.get<std::wstring>((KEY_REQUEST_ID), L"");
			UC_LOG(ERROR) << "OnUserBusyFreeInfoReceived error, server response failed, error code = " << code << ", requestId = " << request;
			triggerEvent(ICalendarServiceEvent, OnUserBusyFreeInfoReceived(code, requestId, vUserSchedule));
			return code;
		}

		boost::property_tree::wptree ptData;
		ptData = ptRoot.get_child(L"data");
		if (ptData.empty()) {
			UC_LOG(INFO) << "OnUserBusyFreeInfoReceived server response data is null. ";
			triggerEvent(ICalendarServiceEvent, OnUserBusyFreeInfoReceived(0, requestId, vUserSchedule));
			return 0;
		}

		UserBusyFreeInfo schedule;
		wstringstream wsStream;
		
		for (unsigned int i = 0; i < userIdList.size(); ++i) {
			UserBusyFreeInfo info;
			schedule.SetUserId(userIdList[i]);
			wsStream.str(L"");
			wsStream.clear();
			wsStream << userIdList[i];
			std::wstring userId(wsStream.str());
			boost::property_tree::wptree pdef;
			boost::property_tree::wptree ptUser = ptData.get_child(userId, pdef);

			//该user data为空，代表请求时间内空闲
			if (ptUser.empty()) {
				continue;
			}

			std::map<int64_t, TimeSlot> mapInfo;
			BOOST_FOREACH(boost::property_tree::wptree::value_type v, ptUser){			
				int64_t eventId = StringConvert::WStringToInt64(v.first);
				int64_t st = v.second.get<int64_t>(L"startTime", 0);
				int64_t et = v.second.get<int64_t>(L"endTime", 0);
				mapInfo[eventId] = TimeSlot(st, et);
			}
			
			schedule.SetInfo(mapInfo);
 			vUserSchedule.push_back(schedule);
		}
	}
	catch (boost::property_tree::ptree_error & e) {
		UC_LOG(ERROR) << "encounter an exception when parse JSON from OnUserBusyFreeInfoReceived , exception: " << e.what();
		triggerEvent(ICalendarServiceEvent, OnUserBusyFreeInfoReceived(JSON_PARSE_ERROR, requestId, vUserSchedule));
		return JSON_PARSE_ERROR;
	}

	//回调上层
	triggerEvent(ICalendarServiceEvent, OnUserBusyFreeInfoReceived(code, requestId, vUserSchedule));
	UC_LOG(INFO) << L"OnUserBusyFreeInfoReceived success.";
	return code;
}

}/* namespace uc */
