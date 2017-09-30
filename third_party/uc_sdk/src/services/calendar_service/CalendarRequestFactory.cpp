/*
*  CalendarRequestFactory.h
*
*  Created on: June 4, 2015
*      Author: changan wu
*/
#include <sstream>
#include "CalendarRequestFactory.h"
#include "CalendarService.h"
#include "LogManager.h"
#include "CommonDef.h"
#include "rapidjson/document.h"
#include "jsonhelper.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

//与服务器交互过程中request/response data Json串中的key
#define KEY_APPID			"appId"
#define KEY_USERID			"userId"
#define KEY_SESSIONID		"sessionId"
#define KEY_SHAREUSERID		"shareUserId"
#define KEY_CONFERENCEID	"conferenceId"
#define KEY_EVENTID			"eventId"
#define KEY_EVENTIDS		"eventIds"
#define KEY_ATTENDEES		"participantsLists"
#define KEY_USERNAME		"userName"
#define KEY_PASSWORD		"password"
#define KEY_TITLE			"title"
#define KEY_STARTTIME		"startTime"
#define KEY_ENDTIME			"endTime"
#define KEY_LOCATION		"location"
#define KEY_EXTRASINFO		"extrasInfo"
#define KEY_LANGUAGE		"language"
#define KEY_EMAILLANGUAGE	"email_language"
#define KEY_SUMMARY			"summary"
#define KEY_LENGTH			"length"
#define KEY_TIMEZONE		"timezone"
#define KEY_ISGNET			"isGnet"
#define KEY_REPEATCOUNT		"repeatCount"
#define KEY_REPEATENDDATE	"repeatEndDate"
#define KEY_GROUPID			"groupId"
#define KEY_ISCYCLE			"isCycle"
#define KEY_ISWHOLEDAY		"wholeType"
#define KEY_NAME			"name"
#define KEY_PWD				"pwd"			//参会密码
#define KEY_TEMPUSERID		"tempUserId"	//临时用户ID
#define KEY_TEMPCONFID		"tempConfId"	//临时会议ID
#define KEY_NEWNAME			"new_name"		//邮件地址入会页面输入的名称
#define KEY_FROM			"from"
#define KEY_STATUS			"status"
#define KEY_LIMIT			"limit"
#define KEY_PAGE			"page"
#define KEY_TYPE			"type"
#define KEY_USER			"user"
#define KEY_TIMEINTERVAL	"timeSpace"
#define KEY_USERLIST		"userList"
#define KEY_CREATOR			"creator"
#define KEY_ORISTART		"ori_start"
#define KEY_EVENTIDFILTER	"eventIdFilter"
#define KEY_FILTERASC		"filterAsc"
#define KEY_CONFERENCE_STARTTIME	"confStartTime"
#define KEY_CONFERENCE_LOCATION		"confLocation"
#define KEY_CONFERENCE_SUMMARY		"confSummary"
#define KEY_CONFERENCE_DECISION		"confDecision"
#define KEY_CONFERENCE_ATTENDEES	"confAttends"
#define KEY_SUMMARYID				"summaryId"
#define KEY_CONFTITLE				"confTitle"
#define KEY_OWNERID					"ownerId"
#define KEY_CYCLEROLE				"cycleRole"
#define KEY_REPEATTYPE				"repeatType"


//与服务器交互过程中request/response data Json串中的default value
#define DEF_VALUE_APPID			2
#define DEF_VALUE_CATEGORIES	1
#define DEF_VALUE_LANGUAGE		1
#define DEF_VALUE_EMAILLANGUAGE	1
#define DEF_VALUE_REPEATCOUNT	0
#define DEF_VALUE_REPEATENDDATE	0
#define DEF_VALUE_GROUPID		0
#define DEF_VALUE_ISCYCLE		false
#define DEF_VALUE_TEMPUSERID	0
#define DEF_VALUE_TEMPCONFID	0
#define DEF_VALUE_NEWNAME		""
#define DEF_VALUE_FROM			"uc"
#define DEF_VALUE_EVENTIDFILTER 0
#define DEF_VALUE_FILTERASC		true
#define DEF_VALUE_CONFTITLE		""
#define DEF_VALUE_CONFDECISION	"%20"

#define HTTP_HEAD_CONTENT_TYPE			"Content-Type"
#define HTTP_HEAD_CONTENT_TYPE_VALUE	"application/x-www-form-urlencoded"
using namespace rapidjson;
namespace uc {

void CalendarRequestFactory::MakeRequestOfCreateMeeting( Meeting *meeting, HTTPRequest &request ) {
			
		if (meeting == NULL)
			return;

		//构建request url
		std::string urlMethod(CONFERENCE_CREATE_METHOD);
		std::string url = _calendarService->GetRequestUrl(urlMethod);

		//构建request body
		Document doc;
		doc.SetObject();
		Document::AllocatorType& allocator = doc.GetAllocator();
		doc.AddMember(KEY_STARTTIME, meeting->GetStartTime(), allocator);
		doc.AddMember(KEY_LENGTH, meeting->GetDuration(), allocator);
		doc.AddMember(KEY_ISGNET, meeting->IsGNet(), allocator);
		doc.AddMember(KEY_ISCYCLE, meeting->IsCycle(), allocator);
		doc.AddMember(KEY_ISWHOLEDAY, meeting->IsWholeDay(), allocator);
		doc.AddMember(KEY_SHAREUSERID, meeting->GetAuthorizerId(), allocator);
		doc.AddMember(KEY_USERID, _calendarService->GetUserId(), allocator);
		doc.AddMember(KEY_LANGUAGE, meeting->GetLanguage(), allocator);
		doc.AddMember(KEY_EMAILLANGUAGE, meeting->GetEmailLanguage(), allocator);
		int32_t repeatCount = meeting->GetCycleRole().RepeatCount();
		if (repeatCount > 1) {
			doc.AddMember(KEY_REPEATCOUNT, repeatCount, allocator);
		}
		else {
			doc.AddMember(KEY_REPEATENDDATE, meeting->GetCycleRole().RepeatEndDate(), allocator);
		}
		
		Value vls(rapidjson::kStringType);
		const std::string &sTitle = meeting->GetTitle();
		vls.SetString(sTitle.c_str(), sTitle.length());
		doc.AddMember(KEY_TITLE, vls, allocator);	
		const std::string &sLocation = meeting->GetLocation();
		vls.SetString(sLocation.c_str(), sLocation.length());
		doc.AddMember(KEY_LOCATION, vls, allocator);
		const std::string &sExtrasInfo = meeting->GetExtrasInfo();
		vls.SetString(sExtrasInfo.c_str(), sExtrasInfo.length());
		doc.AddMember(KEY_EXTRASINFO, vls, allocator);
		const std::string &sSummary = meeting->GetSummary();
		vls.SetString(sSummary.c_str(), sSummary.length());
		doc.AddMember(KEY_SUMMARY, vls, allocator);
		const std::string &sTimeZone = meeting->GetTimeZone();
		vls.SetString(sTimeZone.c_str(), sTimeZone.length());
		doc.AddMember(KEY_TIMEZONE, vls, allocator);
		std::string sSessionId = _calendarService->GetSessionId();
		vls.SetString(sSessionId.c_str(), sSessionId.length());
		doc.AddMember(KEY_SESSIONID, vls, allocator);

		std::stringstream attendeesStream;
		std::vector<Attendee> vAttendee = meeting->GetAttendees();
		for (int i = vAttendee.size() - 1; i >= 0; i--) {
			attendeesStream << vAttendee.at(i).GetAccount();
			if (i > 0) {
				attendeesStream << ";";
			}
		}

		std::string sAttendees = attendeesStream.str();
		vls.SetString(sAttendees.c_str(), sAttendees.length());
		doc.AddMember(KEY_ATTENDEES, vls, allocator);

		if (meeting->IsCycle()) {
			Value vlobj(rapidjson::kObjectType);
			vlobj.SetObject();
			CycleRole role = meeting->GetCycleRole();
			std::string repeatType = role.RepeatType();
			vls.SetString(repeatType.c_str(), repeatType.length(), allocator);
			vlobj.AddMember(KEY_REPEATTYPE, vls, allocator);
			vlobj.AddMember("frequency", role.Interval(), allocator);

			if (repeatType.compare("weekly") == 0) {
				std::string weekdays = role.Weekdays();
				vls.SetString(weekdays.c_str(), weekdays.length(), allocator);
				vlobj.AddMember("weekno", vls, allocator);
			} else if (repeatType.compare("monthlyByDate") == 0) {
				vlobj.AddMember("dayno", role.DayNo(), allocator);
			} else if (repeatType.compare("monthlyBySetPos") == 0) {
				std::string weekdays = role.Weekdays();
				vls.SetString(weekdays.c_str(), weekdays.length(), allocator);
				vlobj.AddMember("weekno", vls, allocator);
				vlobj.AddMember("intervalWeek", role.WeekNo(), allocator);
			} else if (repeatType.compare("yearlyByDate") == 0) {
				vlobj.AddMember("monthno", role.MonthNo(), allocator);
				vlobj.AddMember("dayno", role.DayNo(), allocator);
			}
			else if (repeatType.compare("yearlyBySetPos") == 0) {
				vlobj.AddMember("monthno", role.MonthNo(), allocator);
				std::string weekdays = role.Weekdays();
				vls.SetString(weekdays.c_str(), weekdays.length(), allocator);
				vlobj.AddMember("weekno", vls, allocator);
				vlobj.AddMember("intervalWeek", role.WeekNo(), allocator);
			}
			doc.AddMember(KEY_CYCLEROLE, vlobj, allocator);
		}

		StringBuffer buffer(0,1024);
		Writer<StringBuffer> writer(buffer);
		doc.Accept(writer);
		std::string sbody = buffer.GetString();

		request.Init(url, HTTP_METHOD_POST);
		request.AddHeaderInfo(HTTP_HEAD_CONTENT_TYPE, HTTP_HEAD_CONTENT_TYPE);
		request.SetBody(sbody);
}


void CalendarRequestFactory::MakeRequestOfCreateRemoteAssistance(int32_t userId, HTTPRequest &request)
{
	//构建request url
	std::string urlMethod(CONTROL_CONFERENCE_CREATE_METHOD);
	std::string url = _calendarService->GetRequestUrl(urlMethod);

	//构建request body
	Document doc;
	doc.SetObject();
	int64_t startTime = _calendarService->GetCurrentUnixTime() + 120;
	int32_t duration = 60;
	std::string title = "RemoteAssistance";
	Document::AllocatorType& allocator = doc.GetAllocator();
	doc.AddMember(KEY_STARTTIME, startTime, allocator);
	doc.AddMember(KEY_LENGTH, duration, allocator);
	doc.AddMember(KEY_USERID, _calendarService->GetUserId(), allocator);

	Value vls(rapidjson::kStringType);
	vls.SetString(title.c_str(), title.length());
	doc.AddMember(KEY_TITLE, vls, allocator);

	std::string sSessionId = _calendarService->GetSessionId();
	vls.SetString(sSessionId.c_str(), sSessionId.length());
	doc.AddMember(KEY_SESSIONID, vls, allocator);

	Value vlObj(rapidjson::kObjectType);
	vlObj.SetObject();
	vlObj.AddMember("userId", userId, allocator);
	std::stringstream ss;
	ss << userId;
	std::string sEmail = ss.str();
	vls.SetString(sEmail.c_str(), sEmail.length());
	vlObj.AddMember("email", vls, allocator);
	std::string sNull = "";
	vls.SetString(sNull.c_str(), sNull.length());
	vlObj.AddMember("mobile", vls, allocator);
	vls.SetString(sNull.c_str(), sNull.length());
	vlObj.AddMember("name", vls, allocator);
	Value vlArry(rapidjson::kArrayType);
	vlArry.SetArray();
	vlArry.PushBack(vlObj, allocator);

	doc.AddMember("participantsLists", vlArry, allocator);

	StringBuffer buffer(0, 1024);
	Writer<StringBuffer> writer(buffer);
	doc.Accept(writer);
	std::string sbody = buffer.GetString();

	request.Init(url, HTTP_METHOD_POST);
	request.AddHeaderInfo(HTTP_HEAD_CONTENT_TYPE, HTTP_HEAD_CONTENT_TYPE);
	request.SetBody(sbody);
}


void CalendarRequestFactory::MakeRequestOfUpdateMeeting( int64_t oriTime, Meeting *meeting, HTTPRequest &request ) {
			
		//构建request url
		std::string urlMethod(CONFERENCE_UPDATE_METHOD);
		std::string url = _calendarService->GetRequestUrl(urlMethod);

		//构建request body
		Document doc;
		doc.SetObject();
		Document::AllocatorType& allocator = doc.GetAllocator();

		doc.AddMember(KEY_EVENTID, meeting->GetEventId(), allocator);
		doc.AddMember(KEY_STARTTIME, meeting->GetStartTime(), allocator);
		
		//周期会议单场时才需要ori_time
		if (oriTime > 0 && meeting->IsCycle()) {
			doc.AddMember(KEY_ORISTART, oriTime, allocator);
		}
		
		doc.AddMember(KEY_LENGTH, meeting->GetDuration(), allocator);
		doc.AddMember(KEY_ISGNET, meeting->IsGNet(), allocator);
		doc.AddMember(KEY_ISCYCLE, meeting->IsCycle(), allocator);
		doc.AddMember(KEY_ISWHOLEDAY, meeting->IsWholeDay(), allocator);
		doc.AddMember(KEY_SHAREUSERID, meeting->GetAuthorizerId(), allocator);
		doc.AddMember(KEY_USERID, _calendarService->GetUserId(), allocator);
		doc.AddMember(KEY_LANGUAGE, meeting->GetLanguage(), allocator);
		doc.AddMember(KEY_EMAILLANGUAGE, meeting->GetEmailLanguage(), allocator);
		int32_t repeatCount = meeting->GetCycleRole().RepeatCount();
		if (repeatCount > 1) {
			doc.AddMember(KEY_REPEATCOUNT, repeatCount, allocator);
		}
		else {
			doc.AddMember(KEY_REPEATENDDATE, meeting->GetCycleRole().RepeatEndDate(), allocator);
		}

		Value vls(rapidjson::kStringType);
		vls.SetString(meeting->GetTitle().c_str(), meeting->GetTitle().length());
		doc.AddMember(KEY_TITLE, vls, allocator);
		vls.SetString(meeting->GetLocation().c_str(), meeting->GetLocation().length());
		doc.AddMember(KEY_LOCATION, vls, allocator);
		const std::string &sExtrasInfo = meeting->GetExtrasInfo();
		vls.SetString(sExtrasInfo.c_str(), sExtrasInfo.length());
		doc.AddMember(KEY_EXTRASINFO, vls, allocator);
		vls.SetString(meeting->GetSummary().c_str(), meeting->GetSummary().length());
		doc.AddMember(KEY_SUMMARY, vls, allocator);
		vls.SetString(meeting->GetTimeZone().c_str(), meeting->GetTimeZone().length());
		doc.AddMember(KEY_TIMEZONE, vls, allocator);
		std::string sSessionId = _calendarService->GetSessionId();
		vls.SetString(sSessionId.c_str(), sSessionId.length());
		doc.AddMember(KEY_SESSIONID, vls, allocator);

		std::stringstream attendeesStream;
		std::vector<Attendee> vAttendee = meeting->GetAttendees();
		for (int i = vAttendee.size() - 1; i >= 0; i--) {
			attendeesStream << vAttendee.at(i).GetAccount();
			if (i > 0) {
				attendeesStream << ";";
			}
		}

		std::string sAttendees = attendeesStream.str();
		vls.SetString(sAttendees.c_str(), sAttendees.length());
		doc.AddMember(KEY_ATTENDEES, vls, allocator);

		//周期规则
		if (meeting->IsCycle()) {
			Value vlobj(rapidjson::kObjectType);
			vlobj.SetObject();
			CycleRole role = meeting->GetCycleRole();
			std::string repeatType = role.RepeatType();
			vls.SetString(repeatType.c_str(), repeatType.length(), allocator);
			vlobj.AddMember(KEY_REPEATTYPE, vls, allocator);
			vlobj.AddMember("frequency", role.Interval(), allocator);

			if (repeatType.compare("weekly") == 0) {
				std::string weekdays = role.Weekdays();
				vls.SetString(weekdays.c_str(), weekdays.length(), allocator);
				vlobj.AddMember("weekno", vls, allocator);
			}
			else if (repeatType.compare("monthlyByDate") == 0) {
				vlobj.AddMember("dayno", role.DayNo(), allocator);
			}
			else if (repeatType.compare("monthlyBySetPos") == 0) {
				std::string weekdays = role.Weekdays();
				vls.SetString(weekdays.c_str(), weekdays.length(), allocator);
				vlobj.AddMember("weekno", vls, allocator);
				vlobj.AddMember("intervalWeek", role.WeekNo(), allocator);
			}
			else if (repeatType.compare("yearlyByDate") == 0) {
				vlobj.AddMember("monthno", role.MonthNo(), allocator);
				vlobj.AddMember("dayno", role.DayNo(), allocator);
			}
			else if (repeatType.compare("yearlyBySetPos") == 0) {
				vlobj.AddMember("monthno", role.MonthNo(), allocator);
				std::string weekdays = role.Weekdays();
				vls.SetString(weekdays.c_str(), weekdays.length(), allocator);
				vlobj.AddMember("weekno", vls, allocator);
				vlobj.AddMember("intervalWeek", role.WeekNo(), allocator);
			}

			doc.AddMember(KEY_CYCLEROLE, vlobj, allocator);
		}

		StringBuffer buffer(0,1024);
		Writer<StringBuffer> writer(buffer);
		doc.Accept(writer);
		std::string sbody = buffer.GetString();

		request.Init(url, HTTP_METHOD_POST);
		request.AddHeaderInfo(HTTP_HEAD_CONTENT_TYPE, HTTP_HEAD_CONTENT_TYPE_VALUE);
		request.SetBody(sbody);
}

void CalendarRequestFactory::MakeRequestOfCancelMeeting(int64_t eventId, int64_t startTime, int32_t authorizerId, int32_t language, int32_t emailLanguage, HTTPRequest &request) {
	//构建request url
	//https://oncloud2.quanshi.com/uniform/rs/conferencepc3/cancelReservConference
	std::string urlMethod(CONFERENCE_CANCEL_METHOD);
	std::string url = _calendarService->GetRequestUrl(urlMethod);

	//构建request body
	//{"appId":2,"userId":1189278,"shareUserId":1189278,"sessionId":"da090c234d8bee0e284f5e0f49bc85878c24630c",
	//"conferenceId":917322706,"groupId":20002836,"isCycle":false,"language":"1","email_language":"1","eventId":134542}
	Document doc;
	doc.SetObject();
	Document::AllocatorType& allocator = doc.GetAllocator();
	doc.AddMember(KEY_EVENTID, eventId, allocator);
	doc.AddMember(KEY_USERID, _calendarService->GetUserId(), allocator);
	doc.AddMember(KEY_EMAILLANGUAGE, DEF_VALUE_EMAILLANGUAGE, allocator);
	doc.AddMember(KEY_SHAREUSERID, authorizerId, allocator);
	doc.AddMember(KEY_LANGUAGE, language, allocator);
	doc.AddMember(KEY_EMAILLANGUAGE, emailLanguage, allocator);
	if (startTime > 0) {
		doc.AddMember(KEY_ORISTART, startTime, allocator);
	}

	Value vls(rapidjson::kStringType);
	std::string s = _calendarService->GetSessionId();
	vls.SetString(s.c_str(), s.length());
	doc.AddMember(KEY_SESSIONID, vls, allocator);

	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);
	doc.Accept(writer);
	std::string sbody = buffer.GetString();

	request.Init(url, HTTP_METHOD_POST);
	request.AddHeaderInfo(HTTP_HEAD_CONTENT_TYPE, HTTP_HEAD_CONTENT_TYPE_VALUE);
	request.SetBody(sbody);
}

void CalendarRequestFactory::MakeRequestOfGetMeetingStartURL( const std::string& conferenceId, const std::string& password, HTTPRequest &request ) {
	//构建request url
	std::string urlMethod(CONFERENCE_GETSTARTURL_METHOD);
	std::string url = _calendarService->GetRequestUrl(urlMethod);

	//构建request body
	Document doc;
	doc.SetObject();
	Document::AllocatorType& allocator = doc.GetAllocator();

	doc.AddMember(KEY_TEMPUSERID, DEF_VALUE_TEMPUSERID, allocator);
	doc.AddMember(KEY_TEMPCONFID, DEF_VALUE_TEMPCONFID, allocator);

	Value vls(rapidjson::kStringType);
	std::string sUserId;
	std::stringstream ss;
	ss << _calendarService->GetUserId();
	ss >> sUserId;
	vls.SetString(sUserId.c_str(), sUserId.length());
	doc.AddMember(KEY_NAME, vls, allocator);	//参会人用户名，内部联系人传入userid,外部联系人传入email
	doc.AddMember(KEY_USERID, _calendarService->GetUserId(), allocator);

	vls.SetString(conferenceId.c_str(), conferenceId.length());
	doc.AddMember(KEY_CONFERENCEID, vls, allocator);

	vls.SetString(password.c_str(), password.length());
	doc.AddMember(KEY_PWD, vls, allocator);
	
	const std::string sFrom = DEF_VALUE_FROM;
	vls.SetString(sFrom.c_str(), sFrom.length());
	doc.AddMember(KEY_FROM, vls, allocator);

	const std::string sNewName = DEF_VALUE_NEWNAME;
	vls.SetString(sNewName.c_str(), sNewName.length());
	doc.AddMember(KEY_NEWNAME, vls, allocator);

	std::string sSessionId = _calendarService->GetSessionId();
	vls.SetString(sSessionId.c_str(), sSessionId.length());
	doc.AddMember(KEY_SESSIONID, vls, allocator);

	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);
	doc.Accept(writer);
	std::string sbody = buffer.GetString();

	request.Init(url, HTTP_METHOD_POST);
	request.AddHeaderInfo(HTTP_HEAD_CONTENT_TYPE, HTTP_HEAD_CONTENT_TYPE_VALUE);
	request.SetBody(sbody);
}


void CalendarRequestFactory::MakeRequestOfGetMeetingWebStartURL(const std::string& confId, const std::string& password, const std::string& mail, const std::string& displayName, HTTPRequest &request) {
	//构建request url
	std::string urlMethod(CONFERENCE_GETWEBSTARTURL_METHOD);
	std::string url = _calendarService->GetRequestUrl(urlMethod);

	//构建request body
	Document doc;
	doc.SetObject();
	Document::AllocatorType& allocator = doc.GetAllocator();
	doc.AddMember(KEY_TEMPUSERID, DEF_VALUE_TEMPUSERID, allocator);
	doc.AddMember(KEY_TEMPCONFID, DEF_VALUE_TEMPCONFID, allocator);

	Value vls(rapidjson::kStringType);
	vls.SetString(mail.c_str(), mail.length());
	doc.AddMember(KEY_NAME, vls, allocator);	//参会人用户名，内部联系人传入userid,外部联系人传入email

	vls.SetString(confId.c_str(), confId.length());
	doc.AddMember(KEY_CONFERENCEID, vls, allocator);

	doc.AddMember(KEY_USERID, _calendarService->GetUserId(), allocator);
	std::string sSessionId = _calendarService->GetSessionId();
	vls.SetString(sSessionId.c_str(), sSessionId.length());
	doc.AddMember(KEY_SESSIONID, vls, allocator);

	vls.SetString(password.c_str(), password.length());
	doc.AddMember(KEY_PWD, vls, allocator);

	const std::string sFrom = DEF_VALUE_FROM;
	vls.SetString(sFrom.c_str(), sFrom.length());
	doc.AddMember(KEY_FROM, vls, allocator);

	vls.SetString(displayName.c_str(), displayName.length());
	doc.AddMember(KEY_NEWNAME, vls, allocator);

	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);
	doc.Accept(writer);
	std::string sbody = buffer.GetString();

	request.Init(url, HTTP_METHOD_POST);
	request.AddHeaderInfo(HTTP_HEAD_CONTENT_TYPE, HTTP_HEAD_CONTENT_TYPE_VALUE);
	request.SetBody(sbody);
}

void CalendarRequestFactory::MakeRequestOfUpdateAccreditSetting( int32_t authorizedPersonId, int32_t status, HTTPRequest& request) {
	//构建request url
	std::string urlMethod(ACCREDITSETTING_UPDATE_METHOD);
	std::string url = _calendarService->GetRequestUrl(urlMethod);

	//构建request body
	Document doc;
	doc.SetObject();
	Document::AllocatorType& allocator = doc.GetAllocator();
	doc.AddMember(KEY_USERID, _calendarService->GetUserId(), allocator);
	doc.AddMember(KEY_SHAREUSERID, authorizedPersonId, allocator);
	doc.AddMember(KEY_STATUS, status, allocator);
	
	Value vls(rapidjson::kStringType);
	std::string sSessionId = _calendarService->GetSessionId();
	vls.SetString(sSessionId.c_str(), sSessionId.length());
	doc.AddMember(KEY_SESSIONID, vls, allocator);
	
	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);
	doc.Accept(writer);
	std::string sbody = buffer.GetString();

	request.Init(url, HTTP_METHOD_POST);
	request.AddHeaderInfo(HTTP_HEAD_CONTENT_TYPE, HTTP_HEAD_CONTENT_TYPE_VALUE);
	request.SetBody(sbody);
}

void CalendarRequestFactory::MakeRequestOfGetAccreditUserList( HTTPRequest& request ) {
	//构建request url
	std::string urlMethod(SHAREUSER_LIST_METHOD);
	std::string url = _calendarService->GetRequestUrl(urlMethod);

	//构建request body
	Document doc;
	doc.SetObject();
	Document::AllocatorType& allocator = doc.GetAllocator();
	doc.AddMember(KEY_USERID, _calendarService->GetUserId(), allocator);
	Value vls(rapidjson::kStringType);
	std::string sSessionId = _calendarService->GetSessionId();
	vls.SetString(sSessionId.c_str(), sSessionId.length());
	doc.AddMember(KEY_SESSIONID, vls, allocator);

	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);
	doc.Accept(writer);
	std::string sbody = buffer.GetString();

	request.Init(url, HTTP_METHOD_POST);
	request.AddHeaderInfo(HTTP_HEAD_CONTENT_TYPE, HTTP_HEAD_CONTENT_TYPE_VALUE);
	request.SetBody(sbody);
}

void CalendarRequestFactory::MakeRequestOfGetMeetingList(int32_t authorizerId, int64_t startTime, int64_t endTime, int32_t limit, int64_t eventIdFilter, bool bFilterAsc, HTTPRequest& request) {
	//构建request url
	std::string urlMethod(CONFERENCE_LIST_METHOD);
	std::string url = _calendarService->GetRequestUrl(urlMethod);

	//构建request body
	Document doc;
	doc.SetObject();
	Document::AllocatorType& allocator = doc.GetAllocator();
	doc.AddMember(KEY_USERID, _calendarService->GetUserId(), allocator);
	doc.AddMember(KEY_SHAREUSERID, authorizerId, allocator);
	if (startTime > 0){
		doc.AddMember(KEY_STARTTIME, startTime, allocator);
	}

	if (endTime > 0) {
		doc.AddMember(KEY_ENDTIME, endTime, allocator);
	}

	doc.AddMember(KEY_LIMIT, limit, allocator);
	doc.AddMember(KEY_EVENTIDFILTER, eventIdFilter, allocator);	
	doc.AddMember(KEY_FILTERASC, bFilterAsc, allocator);
	
	Value vls(rapidjson::kStringType);
	std::string sSessionId = _calendarService->GetSessionId();
	vls.SetString(sSessionId.c_str(), sSessionId.length());
	doc.AddMember(KEY_SESSIONID, vls, allocator);

	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);
	doc.Accept(writer);
	std::string sbody = buffer.GetString();

	request.Init(url, HTTP_METHOD_POST);
	request.AddHeaderInfo(HTTP_HEAD_CONTENT_TYPE, HTTP_HEAD_CONTENT_TYPE_VALUE);
	request.SetBody(sbody);
}

void CalendarRequestFactory::MakeRequestOfAccptMeetingInvitation( int64_t eventId, int32_t authorizerId, int64_t oriStart, HTTPRequest& request) {
	//构建request url
	std::string urlMethod(CONFERENCE_INVITATION_PROCESS_METHOD);
	std::string url = _calendarService->GetRequestUrl(urlMethod);

	//构建request body
	Document doc;
	doc.SetObject();
	Document::AllocatorType& allocator = doc.GetAllocator();
	doc.AddMember(KEY_USERID, _calendarService->GetUserId(), allocator);
	doc.AddMember(KEY_SHAREUSERID, authorizerId, allocator);
	doc.AddMember(KEY_STATUS, 1, allocator);
	doc.AddMember(KEY_EVENTID, eventId, allocator);
	if (oriStart > 0) {
		doc.AddMember(KEY_ORISTART, oriStart, allocator);						//接受拒绝循环会议单场时使用
	}


	Value vls(rapidjson::kStringType);
	std::string sSessionId = _calendarService->GetSessionId();
	vls.SetString(sSessionId.c_str(), sSessionId.length());
	doc.AddMember(KEY_SESSIONID, vls, allocator);

	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);
	doc.Accept(writer);
	std::string sbody = buffer.GetString();

	request.Init(url, HTTP_METHOD_POST);
	request.AddHeaderInfo(HTTP_HEAD_CONTENT_TYPE, HTTP_HEAD_CONTENT_TYPE_VALUE);
	request.SetBody(sbody);
}

void CalendarRequestFactory::MakeRequestOfDenyMeetingInvitation( int64_t eventId, int32_t authorizerId, int64_t oriStart, HTTPRequest& request) {
	//构建request url
	std::string urlMethod(CONFERENCE_INVITATION_PROCESS_METHOD);
	std::string url = _calendarService->GetRequestUrl(urlMethod);

	//构建request body
	Document doc;
	doc.SetObject();
	Document::AllocatorType& allocator = doc.GetAllocator();
	doc.AddMember(KEY_USERID, _calendarService->GetUserId(), allocator);
	doc.AddMember(KEY_SHAREUSERID, authorizerId, allocator);
	doc.AddMember(KEY_STATUS, 0, allocator);
	doc.AddMember(KEY_EVENTID, eventId, allocator);
	if (oriStart > 0) {
		doc.AddMember(KEY_ORISTART, oriStart, allocator);						//接受拒绝循环会议单场时使用
	}

	Value vls(rapidjson::kStringType);
	std::string sSessionId = _calendarService->GetSessionId();
	vls.SetString(sSessionId.c_str(), sSessionId.length());
	doc.AddMember(KEY_SESSIONID, vls, allocator);

	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);
	doc.Accept(writer);
	std::string sbody = buffer.GetString();

	request.Init(url, HTTP_METHOD_POST);
	request.AddHeaderInfo(HTTP_HEAD_CONTENT_TYPE, HTTP_HEAD_CONTENT_TYPE_VALUE);
	request.SetBody(sbody);
}

void CalendarRequestFactory::MakeRequestOfGetConferenceInfoById( int32_t conferenceId, int64_t startTime, int64_t endTime, int32_t authorizerId, HTTPRequest& request ) {
	//构建request url
	//https://oncloud2.quanshi.com/uniform/rs/conference/getConferenceInfoById

	std::string urlMethod(CONFERENCE_INFO_BYID_METHOD);
	std::string url = _calendarService->GetRequestUrl(urlMethod);

	//构建request body
	//{"userId":"1189278","sessionId":"71722dd6fe666b41a0c93e4914db16d9b33231fe","conferenceId":"419379258","startTime":"1432881000","endTime":""}
	Document doc;
	doc.SetObject();
	Document::AllocatorType& allocator = doc.GetAllocator();
	doc.AddMember(KEY_USERID, _calendarService->GetUserId(), allocator);
	doc.AddMember(KEY_SHAREUSERID, authorizerId, allocator);
	doc.AddMember(KEY_CONFERENCEID, conferenceId, allocator);
	if (startTime >0) {
		doc.AddMember(KEY_STARTTIME, startTime, allocator);
	}

	if (endTime > 0) {
		doc.AddMember(KEY_ENDTIME, endTime, allocator);
	}

	Value vls(rapidjson::kStringType);
	std::string sSessionId = _calendarService->GetSessionId();
	vls.SetString(sSessionId.c_str(), sSessionId.length());
	doc.AddMember(KEY_SESSIONID, vls, allocator);

	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);
	doc.Accept(writer);
	std::string sbody = buffer.GetString();

	request.Init(url, HTTP_METHOD_POST);
	request.AddHeaderInfo(HTTP_HEAD_CONTENT_TYPE, HTTP_HEAD_CONTENT_TYPE_VALUE);
	request.SetBody(sbody);
}

void CalendarRequestFactory::MakeRequestOfGetMeetingInfoByIds( const std::vector<int64_t>& eventIds, int64_t startTime, int64_t endTime, int32_t authorizerId, HTTPRequest& request ) {

	//构建request url
	//https://oncloud2.quanshi.com/uniform/rs/conference/getConferenceInfoByIds
	std::string urlMethod(CONFERENCE_INFO_BYIDS_METHOD);
	std::string url = _calendarService->GetRequestUrl(urlMethod);

	//构建request body
	//{"userId":"1189278","sessionId":"71722dd6fe666b41a0c93e4914db16d9b33231fe","conferenceId":"419379258","startTime":"1432881000","endTime":""}
	Document doc;
	doc.SetObject();
	Document::AllocatorType& allocator = doc.GetAllocator();
	doc.AddMember(KEY_USERID, _calendarService->GetUserId(), allocator);
	doc.AddMember(KEY_SHAREUSERID, authorizerId, allocator);
	if (startTime > 0) {
		doc.AddMember(KEY_STARTTIME, startTime, allocator);
	}
	if (endTime > 0) {
		doc.AddMember(KEY_ENDTIME, endTime, allocator);
	}

	Value vls(rapidjson::kStringType);

	if (!eventIds.empty()) {
		std::stringstream ss;
		for (int i = eventIds.size() - 1; i >= 0; i--) {
			ss << eventIds[i];
			if (i != 0) {
				ss << ";";
			}
		}

		vls.SetString(ss.str().c_str(), allocator);
		doc.AddMember(KEY_EVENTIDS, vls, allocator);
	}

	std::string sSessionId = _calendarService->GetSessionId();
	vls.SetString(sSessionId.c_str(), sSessionId.length());
	doc.AddMember(KEY_SESSIONID, vls, allocator);

	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);
	doc.Accept(writer);
	std::string sbody = buffer.GetString();

	request.Init(url, HTTP_METHOD_POST);
	request.AddHeaderInfo(HTTP_HEAD_CONTENT_TYPE, HTTP_HEAD_CONTENT_TYPE_VALUE);
	request.SetBody(sbody);
}

void CalendarRequestFactory::MakeRequestOfForwardMeeting(int64_t eventId, const std::vector<Attendee>& attendees, int32_t authorizerId, int64_t oriStart, int32_t language, int32_t emailLanguage, HTTPRequest& request) {

	//构建request url
	//https://oncloud2.quanshi.com/uniform/rs/conference/forwardConference
	std::string urlMethod(CONFERENCE_FORWARD_METHOD);
	std::string url = _calendarService->GetRequestUrl(urlMethod);

	//构建request body
	//{"appId":2,"sessionId":"0a039b75c63d09577756a4528a0bb7503ec699b4","userId":1189278,"shareUserId":1189278,"participantsLists":[{"type":1,"user":"477828","name":"王奕婷"}],"eventId":134972,"language":1,"conferenceId":425510740,"isCycle":false,"groupId":20003265}
	Document doc;
	doc.SetObject();
	Document::AllocatorType& allocator = doc.GetAllocator();
	doc.AddMember(KEY_USERID, _calendarService->GetUserId(), allocator);
	doc.AddMember(KEY_SHAREUSERID, authorizerId, allocator);
	//doc.AddMember(KEY_APPID, DEF_VALUE_APPID, allocator);
	doc.AddMember(KEY_EVENTID, eventId, allocator);
	doc.AddMember(KEY_LANGUAGE, language, allocator);
	doc.AddMember(KEY_EMAILLANGUAGE, emailLanguage, allocator);
	if (oriStart > 0) {
		doc.AddMember(KEY_ORISTART, oriStart, allocator);
	}
	
	Value vls(rapidjson::kStringType);
	std::string sSessionId = _calendarService->GetSessionId();
	vls.SetString(sSessionId.c_str(), sSessionId.length());
	doc.AddMember(KEY_SESSIONID, vls, allocator);

	Value vlarr(kArrayType);
	for (unsigned int  i = 0; i < attendees.size(); i++) {
		Value vlo(kObjectType);
		vlo.AddMember(KEY_TYPE, attendees[i].GetType(), allocator);
		
		Value vls(kStringType);
		const std::string& sAccount = attendees[i].GetAccount();
		vls.SetString(sAccount.c_str(), sAccount.length());
		vlo.AddMember(KEY_USER, vls, allocator);

		const std::string& sName = attendees[i].GetName();
		vls.SetString(sName.c_str(), sName.length());
		vlo.AddMember(KEY_NAME, vls, allocator);
		
		vlarr.PushBack(vlo, allocator);
	}
	doc.AddMember(KEY_ATTENDEES, vlarr, allocator);

	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);
	doc.Accept(writer);
	std::string sbody = buffer.GetString();

	request.Init(url, HTTP_METHOD_POST);
	request.AddHeaderInfo(HTTP_HEAD_CONTENT_TYPE, HTTP_HEAD_CONTENT_TYPE_VALUE);
	request.SetBody(sbody);
}

void CalendarRequestFactory::MakeRequestOfGetUserSchedule(const std::vector<int32_t>& userIdList, int64_t startTime, int64_t endTime, int32_t timeInterval, int64_t eventIdFilter, HTTPRequest& request) {

	//构建request url
	//https://oncloud2.quanshi.com/uniform/rs/conference/getUserBusyFreeStatus
	std::string urlMethod(BUSYFREE_STATUS_METHOD);
	std::string url = _calendarService->GetRequestUrl(urlMethod);

	//构建request body
	//{"userId":1189278,"sessionId":"71722dd6fe666b41a0c93e4914db16d9b33231fe","shareUserId":1189278,"startTime":1433952000,"endTime":1434038400,"timeSpace":5,"userList":"217705;477828;1189278;1382762"}
	Document doc;
	doc.SetObject();
	Document::AllocatorType& allocator = doc.GetAllocator();
	//doc.AddMember(KEY_APPID, DEF_VALUE_APPID, allocator);
	doc.AddMember(KEY_USERID, _calendarService->GetUserId(), allocator);
	doc.AddMember(KEY_STARTTIME, startTime, allocator);
	doc.AddMember(KEY_ENDTIME, endTime, allocator);
	doc.AddMember(KEY_TIMEINTERVAL, timeInterval, allocator);
	doc.AddMember(KEY_EVENTIDFILTER, eventIdFilter, allocator);	
	
	Value vls(rapidjson::kStringType);
	std::string sSessionId = _calendarService->GetSessionId();
	vls.SetString(sSessionId.c_str(), sSessionId.length());
	doc.AddMember(KEY_SESSIONID, vls, allocator);

	std::stringstream userListStream;
	for (int32_t id : userIdList) {
		userListStream << id << ";";
	}

	std::string sUserList = userListStream.str();
	sUserList = sUserList.substr(0, sUserList.length() - 1);
	vls.SetString(sUserList.c_str(), sUserList.length());
	doc.AddMember(KEY_USERLIST, vls, allocator);

	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);
	doc.Accept(writer);
	std::string sbody = buffer.GetString();

	request.Init(url, HTTP_METHOD_POST);
	request.AddHeaderInfo(HTTP_HEAD_CONTENT_TYPE, HTTP_HEAD_CONTENT_TYPE_VALUE);
	request.SetBody(sbody);
}

void CalendarRequestFactory::MakeRequestOfCreateMinutes(int64_t eventId, int64_t startTime, const std::string& title, const std::string& minutes, HTTPRequest& request) {
	//构建request url
	std::string urlMethod(CONFERENCE_SMMMARY_CREATE_METHOD);
	std::string url = _calendarService->GetRequestUrl(urlMethod);

	//构建request body
	Document doc;
	doc.SetObject();
	Document::AllocatorType& allocator = doc.GetAllocator();
	doc.AddMember(KEY_USERID, _calendarService->GetUserId(), allocator);
	doc.AddMember(KEY_CREATOR, _calendarService->GetUserId(), allocator);
	doc.AddMember(KEY_EVENTID, eventId, allocator);
	
	Value vls(rapidjson::kStringType);
	std::string sSummary = StringConvert::URLEncode(minutes);
	vls.SetString(sSummary.c_str(), sSummary.length(), allocator);
	doc.AddMember(KEY_CONFERENCE_SUMMARY, vls, allocator);
	std::string sSessionId = _calendarService->GetSessionId();
	vls.SetString(sSessionId.c_str(), sSessionId.length());
	doc.AddMember(KEY_SESSIONID, vls, allocator);
	std::string sTitle = StringConvert::URLEncode(title);
	vls.SetString(sTitle.c_str(), sTitle.length(), allocator);
	doc.AddMember(KEY_CONFTITLE, vls, allocator);

	if (startTime > 0) {
		doc.AddMember(KEY_CONFERENCE_STARTTIME, startTime, allocator);
	}
	
	StringBuffer buffer(0, 1024);
	Writer<StringBuffer> writer(buffer);
	doc.Accept(writer);
	std::string sbody = buffer.GetString();

	request.Init(url, HTTP_METHOD_POST);
	request.AddHeaderInfo(HTTP_HEAD_CONTENT_TYPE, HTTP_HEAD_CONTENT_TYPE_VALUE);
	request.SetBody(sbody);
}

void CalendarRequestFactory::MakeRequestOfUpdateMinutes(int64_t minutesId, const std::string& title, const std::string& minutes, HTTPRequest& request) {
	//构建request url
	std::string urlMethod(CONFERENCE_SMMMARY_UPDATE_METHOD);
	std::string url = _calendarService->GetRequestUrl(urlMethod);

	//构建request body
	Document doc;
	doc.SetObject();
	Document::AllocatorType& allocator = doc.GetAllocator();
	doc.AddMember(KEY_USERID, _calendarService->GetUserId(), allocator);
	doc.AddMember(KEY_SUMMARYID, minutesId, allocator);
	doc.AddMember(KEY_OWNERID, _calendarService->GetUserId(), allocator);
	
	Value vls(rapidjson::kStringType);
	std::string sSessionId = _calendarService->GetSessionId();
	vls.SetString(sSessionId.c_str(), sSessionId.length());
	doc.AddMember(KEY_SESSIONID, vls, allocator);
	std::string sSummary = StringConvert::URLEncode(minutes);
	vls.SetString(sSummary.c_str(), sSummary.length(), allocator);
	doc.AddMember(KEY_CONFERENCE_SUMMARY, vls, allocator);
	std::string sTitle = StringConvert::URLEncode(title);
	vls.SetString(sTitle.c_str(), sTitle.length(), allocator);
	doc.AddMember(KEY_CONFTITLE, vls, allocator);
	doc.AddMember(KEY_CREATOR, _calendarService->GetUserId(), allocator);		

	StringBuffer buffer(0,1024);
	Writer<StringBuffer> writer(buffer);
	doc.Accept(writer);
	std::string sbody = buffer.GetString();

	request.Init(url, HTTP_METHOD_POST);
	request.AddHeaderInfo(HTTP_HEAD_CONTENT_TYPE, HTTP_HEAD_CONTENT_TYPE_VALUE);
	request.SetBody(sbody);
}

void CalendarRequestFactory::MakeRequestOfGetMinutesDetails(int64_t minutesId, HTTPRequest& request) {
	//构建request url
	std::string urlMethod(CONFERENCE_SMMMARY_VIEW_METHOD);
	std::string url = _calendarService->GetRequestUrl(urlMethod);

	//构建request body
	Document doc;
	doc.SetObject();
	Document::AllocatorType& allocator = doc.GetAllocator();
	doc.AddMember(KEY_USERID, _calendarService->GetUserId(), allocator);
	doc.AddMember(KEY_SUMMARYID, minutesId, allocator);
	
	Value vls(rapidjson::kStringType);
	std::string sSessionId = _calendarService->GetSessionId();
	vls.SetString(sSessionId.c_str(), sSessionId.length());
	doc.AddMember(KEY_SESSIONID, vls, allocator);
	
	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);
	doc.Accept(writer);
	std::string sbody = buffer.GetString();

	request.Init(url, HTTP_METHOD_POST);
	request.AddHeaderInfo(HTTP_HEAD_CONTENT_TYPE, HTTP_HEAD_CONTENT_TYPE_VALUE);
	request.SetBody(sbody);
}

void CalendarRequestFactory::MakeRequestOfGetMinutesList(int64_t eventId, int64_t startTime, HTTPRequest& request ) {
	//构建request url
	std::string urlMethod(CONFERENCE_SUMMARYLIST_METHOD);
	std::string url = _calendarService->GetRequestUrl(urlMethod);

	//构建request body
	Document doc;
	doc.SetObject();
	Document::AllocatorType& allocator = doc.GetAllocator();
	doc.AddMember(KEY_USERID, _calendarService->GetUserId(), allocator);
	
	doc.AddMember(KEY_EVENTID, eventId, allocator);
	if (startTime > 0) {
		doc.AddMember(KEY_CONFERENCE_STARTTIME, startTime, allocator);
	}
	
	Value vls(rapidjson::kStringType);
	std::string sSessionId = _calendarService->GetSessionId();
	vls.SetString(sSessionId.c_str(), sSessionId.length());
	doc.AddMember(KEY_SESSIONID, vls, allocator);

	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);
	doc.Accept(writer);
	std::string sbody = buffer.GetString();

	request.Init(url, HTTP_METHOD_POST);
	request.AddHeaderInfo(HTTP_HEAD_CONTENT_TYPE, HTTP_HEAD_CONTENT_TYPE_VALUE);
	request.SetBody(sbody);
}

void CalendarRequestFactory::MakeRequestOfDeleteMeetingRecording(const std::string& confId, int64_t eventId, int64_t startTime, HTTPRequest& request) {
	//构建request url
	std::string urlMethod(CONFERENCE_RECORD_DELETE_METHOD);
	std::string url = _calendarService->GetRequestUrl(urlMethod);

	//构建request body
	Document doc;
	doc.SetObject();
	Document::AllocatorType& allocator = doc.GetAllocator();
	doc.AddMember(KEY_USERID, _calendarService->GetUserId(), allocator);
	Value vls(rapidjson::kStringType);
	vls.SetString(confId.c_str(), confId.length(), allocator);
	doc.AddMember(KEY_CONFERENCEID, vls, allocator);
	doc.AddMember(KEY_EVENTID, eventId, allocator);	
	std::string sSessionId = _calendarService->GetSessionId();
	vls.SetString(sSessionId.c_str(), sSessionId.length());
	doc.AddMember(KEY_SESSIONID, vls, allocator);

	if (startTime > 0) {
		doc.AddMember(KEY_CONFERENCE_STARTTIME, startTime, allocator);
	}

	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);
	doc.Accept(writer);
	std::string sbody = buffer.GetString();

	request.Init(url, HTTP_METHOD_POST);
	request.AddHeaderInfo(HTTP_HEAD_CONTENT_TYPE, HTTP_HEAD_CONTENT_TYPE_VALUE);
	request.SetBody(sbody);
}

void CalendarRequestFactory::MakeRequestOfGetBriefMeetingList(int64_t startTime, int64_t endTime, int32_t authorizerId, HTTPRequest& request) {
	
	//构建request url
	std::string urlMethod(CONFERENCE_BRIEF_LIST_METHOD);
	std::string url = _calendarService->GetRequestUrl(urlMethod);

	//构建request body
	Document doc;
	doc.SetObject();
	Document::AllocatorType& allocator = doc.GetAllocator();
	doc.AddMember(KEY_USERID, _calendarService->GetUserId(), allocator);
	Value vls(rapidjson::kStringType);
	std::string sSessionId = _calendarService->GetSessionId();
	vls.SetString(sSessionId.c_str(), sSessionId.length());
	doc.AddMember(KEY_SESSIONID, vls, allocator);

	if (startTime > 0) {
		doc.AddMember("startTime", startTime, allocator);
	}

	if (endTime > 0) {
		doc.AddMember("endTime", endTime, allocator);
	}
	doc.AddMember(KEY_SHAREUSERID, authorizerId, allocator);

	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);
	doc.Accept(writer);
	std::string sbody = buffer.GetString();

	request.Init(url, HTTP_METHOD_POST);
	request.AddHeaderInfo(HTTP_HEAD_CONTENT_TYPE, HTTP_HEAD_CONTENT_TYPE_VALUE);
	request.SetBody(sbody);
}

void CalendarRequestFactory::MakeRequestOfGetBriefChangedMeetingList(int64_t startTime, int32_t authorizerId, HTTPRequest& request) {
	//构建request url
	std::string urlMethod(CONFERENCE_BRIEF_CHANGE_LIST_METHOD);
	std::string url = _calendarService->GetRequestUrl(urlMethod);

	//构建request body
	Document doc;
	doc.SetObject();
	Document::AllocatorType& allocator = doc.GetAllocator();
	doc.AddMember(KEY_USERID, _calendarService->GetUserId(), allocator);
	Value vls(rapidjson::kStringType);
	std::string sSessionId = _calendarService->GetSessionId();
	vls.SetString(sSessionId.c_str(), sSessionId.length());
	doc.AddMember(KEY_SESSIONID, vls, allocator);

	if (startTime > 0) {
		doc.AddMember("startTime", startTime, allocator);
	}

	doc.AddMember(KEY_SHAREUSERID, authorizerId, allocator);

	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);
	doc.Accept(writer);
	std::string sbody = buffer.GetString();

	request.Init(url, HTTP_METHOD_POST);
	request.AddHeaderInfo(HTTP_HEAD_CONTENT_TYPE, HTTP_HEAD_CONTENT_TYPE_VALUE);
	request.SetBody(sbody);
}

void CalendarRequestFactory::MakeRequestOfUploadMeetingFile(int64_t eventId, int32_t operation, const DocumentInfo& info, HTTPRequest& request)
{
	//构建request url
	std::string urlMethod(CONFERENCE_UPLOAD_FILE_METHOD);
	std::string url = _calendarService->GetRequestUrl(urlMethod);

	//构建request body
	Document doc;
	doc.SetObject();
	Document::AllocatorType& allocator = doc.GetAllocator();
	doc.AddMember(KEY_USERID, _calendarService->GetUserId(), allocator);
	Value vls(rapidjson::kStringType);
	std::string sSessionId = _calendarService->GetSessionId();
	vls.SetString(sSessionId.c_str(), sSessionId.length());
	doc.AddMember(KEY_SESSIONID, vls, allocator);

	doc.AddMember(KEY_EVENTID, eventId, allocator);
	doc.AddMember("operation", operation, allocator);

	Value vlObj(rapidjson::kObjectType);
	vlObj.SetObject();

	vlObj.AddMember("user_id", _calendarService->GetUserId(), allocator);
	std::string sUserName = _calendarService->GetUserName();
	vls.SetString(sUserName.c_str(), sUserName.length());
	vlObj.AddMember("user_name", vls, allocator);
	vls.SetString(info.GetFileUrl().c_str(), info.GetFileUrl().length());
	vlObj.AddMember("file_url", vls, allocator);
	std::string fileName = info.GetFileName();
	vls.SetString(fileName.c_str(), fileName.length());
	vlObj.AddMember("title", vls, allocator);
	std::string fileFormat = "";
	std::size_t nPos = info.GetFileName().find_last_of(".");
	if (nPos != string::npos){
		fileFormat = fileName.substr(nPos + 1);
	}

	vls.SetString(fileFormat.c_str(), fileFormat.length());
	vlObj.AddMember("format", vls, allocator);
	vlObj.AddMember("size", info.GetFileSize(), allocator);
	vlObj.AddMember("cloud_type", info.GetCloudType(), allocator);
	vls.SetString(info.GetPreview().c_str(), info.GetPreview().length());
	vlObj.AddMember("preview", vls, allocator);

	if (info.GetCloudType() == CT_GoKuai) {
		vls.SetString(info.GetHashCode().c_str(), info.GetHashCode().length());
		vlObj.AddMember("hash_code", vls, allocator);
		vlObj.AddMember("mount_id", info.GetMountId(), allocator);
		vlObj.AddMember("detail_type", info.GetDetailType(), allocator);
		vls.SetString(info.GetDetailContent().c_str(), info.GetDetailContent().length());
		vlObj.AddMember("detail_content", vls, allocator);
	}
	
	doc.AddMember("data", vlObj, allocator);

	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);
	doc.Accept(writer);
	std::string sbody = buffer.GetString();

	request.Init(url, HTTP_METHOD_POST);
	request.AddHeaderInfo(HTTP_HEAD_CONTENT_TYPE, HTTP_HEAD_CONTENT_TYPE_VALUE);
	request.SetBody(sbody);
}

void CalendarRequestFactory::MakeRequestOfDeleteMeetingFile(int64_t minutesId, HTTPRequest& request)
{
	//构建request url
	std::string urlMethod(CONFERENCE_DELETE_FILE_METHOD);
	std::string url = _calendarService->GetRequestUrl(urlMethod);

	//构建request body
	Document doc;
	doc.SetObject();
	Document::AllocatorType& allocator = doc.GetAllocator();
	doc.AddMember(KEY_USERID, _calendarService->GetUserId(), allocator);
	Value vls(rapidjson::kStringType);
	std::string sSessionId = _calendarService->GetSessionId();
	vls.SetString(sSessionId.c_str(), sSessionId.length());
	doc.AddMember(KEY_SESSIONID, vls, allocator);

	doc.AddMember(KEY_SUMMARYID, minutesId, allocator);

	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);
	doc.Accept(writer);
	std::string sbody = buffer.GetString();

	request.Init(url, HTTP_METHOD_POST);
	request.AddHeaderInfo(HTTP_HEAD_CONTENT_TYPE, HTTP_HEAD_CONTENT_TYPE_VALUE);
	request.SetBody(sbody);
}

void CalendarRequestFactory::MakeRequestOfCreateMeetingRoom(const MeetingRoom& room, HTTPRequest &request)
{
	//构建request url
	std::string urlMethod(MEETING_ROOM_CREATE_METHOD);
	std::string url = _calendarService->GetRequestUrl(urlMethod);

	//构建request body
	Document doc;
	doc.SetObject();
	Document::AllocatorType& allocator = doc.GetAllocator();
	doc.AddMember(KEY_USERID, _calendarService->GetUserId(), allocator);
	doc.AddMember("defaultAttendsVisible", room.GetDefaultAttendeesVisible(), allocator);
	doc.AddMember("defaultAttendsJoin", room.GetDefaultAttendeesJoin(), allocator);
	doc.AddMember("isSetPassword", room.GetIsSetPassword(), allocator);
	doc.AddMember("roomHostId", room.GetRoomHostId(), allocator);
	doc.AddMember("orgId", room.GetOrgId(), allocator);
	doc.AddMember(KEY_STARTTIME, room.GetStartTime(), allocator);
	doc.AddMember(KEY_ENDTIME, room.GetEndTime(), allocator);

	Value vls(rapidjson::kStringType);
	const std::string &sTitle = room.GetTitle();
	vls.SetString(sTitle.c_str(), sTitle.length());
	doc.AddMember("roomTitle", vls, allocator);
	const std::string &remarks = room.GetRemarks();
	vls.SetString(remarks.c_str(), remarks.length());
	doc.AddMember("remarks", vls, allocator);
	const std::string &password = room.GetRoomPassword();
	vls.SetString(password.c_str(), password.length());
	doc.AddMember("roomPassword", vls, allocator);
	std::string sSessionId = _calendarService->GetSessionId();
	vls.SetString(sSessionId.c_str(), sSessionId.length());
	doc.AddMember(KEY_SESSIONID, vls, allocator);

	const std::vector<Attendee>& vAttendee = room.GetAttendees();
	Value vlarry(rapidjson::kArrayType);
	vlarry.SetArray();
	for (int i = vAttendee.size() - 1; i >= 0; i--) {
		vls.SetString(vAttendee.at(i).GetAccount().c_str(), vAttendee.at(i).GetAccount().length());
		vlarry.PushBack(vls, allocator);
	}

// 	std::stringstream attendeesStream;
// 	attendeesStream << "[";
// 	const std::vector<Attendee>& vAttendee = room.GetAttendees();
// 
// 	for (int i = vAttendee.size() - 1; i >= 0; i--) {
// 		attendeesStream << vAttendee.at(i).GetAccount();
// 		if (i > 0) {
// 			attendeesStream << ",";
// 		}
// 	}
// 	attendeesStream << "]";
// 	std::string sAttendees = attendeesStream.str();
// 	vls.SetString(sAttendees.c_str(), sAttendees.length());

	doc.AddMember("attendLists", vlarry, allocator);

	StringBuffer buffer(0, 1024);
	Writer<StringBuffer> writer(buffer);
	doc.Accept(writer);
	std::string sbody = buffer.GetString();

	request.Init(url, HTTP_METHOD_POST);
	request.AddHeaderInfo(HTTP_HEAD_CONTENT_TYPE, HTTP_HEAD_CONTENT_TYPE);
	request.SetBody(sbody);
}

void CalendarRequestFactory::MakeRequestOfUpdateMeetingRoom(const MeetingRoom& room, HTTPRequest &request)
{
	//构建request url
	std::string urlMethod(MEETING_ROOM_UPDATE_METHOD);
	std::string url = _calendarService->GetRequestUrl(urlMethod);

	//构建request body
	Document doc;
	doc.SetObject();
	Document::AllocatorType& allocator = doc.GetAllocator();
	doc.AddMember(KEY_USERID, _calendarService->GetUserId(), allocator);
	doc.AddMember("defaultAttendsVisible", room.GetDefaultAttendeesVisible(), allocator);
	doc.AddMember("defaultAttendsJoin", room.GetDefaultAttendeesJoin(), allocator);
	doc.AddMember("isSetPassword", room.GetIsSetPassword(), allocator);
	doc.AddMember("roomHostId", room.GetRoomHostId(), allocator);
	doc.AddMember("orgId", room.GetOrgId(), allocator);
	doc.AddMember(KEY_STARTTIME, room.GetStartTime(), allocator);
	doc.AddMember(KEY_ENDTIME, room.GetEndTime(), allocator);

	Value vls(rapidjson::kStringType);
	const std::string &roomId = room.GetRoomId();
	vls.SetString(roomId.c_str(), roomId.length());
	doc.AddMember("roomId", vls, allocator);

	const std::string &sTitle = room.GetTitle();
	vls.SetString(sTitle.c_str(), sTitle.length());
	doc.AddMember("roomTitle", vls, allocator);
	const std::string &remarks = room.GetRemarks();
	vls.SetString(remarks.c_str(), remarks.length());
	doc.AddMember("remarks", vls, allocator);
	const std::string &password = room.GetRoomPassword();
	vls.SetString(password.c_str(), password.length());
	doc.AddMember("roomPassword", vls, allocator);
	std::string sSessionId = _calendarService->GetSessionId();
	vls.SetString(sSessionId.c_str(), sSessionId.length());
	doc.AddMember(KEY_SESSIONID, vls, allocator);

// 	std::stringstream attendeesStream;
// 	attendeesStream << "[";
// 	const std::vector<Attendee>& vAttendee = room.GetAttendees();
// 
// 	for (int i = vAttendee.size() - 1; i >= 0; i--) {
// 		attendeesStream << vAttendee.at(i).GetAccount();
// 		if (i > 0) {
// 			attendeesStream << ",";
// 		}
// 	}
// 	attendeesStream << "]";
// 
// 	std::string sAttendees = attendeesStream.str();
// 	vls.SetString(sAttendees.c_str(), sAttendees.length());
	const std::vector<Attendee>& vAttendee = room.GetAttendees();
	Value vlarry(rapidjson::kArrayType);
	vlarry.SetArray();
	for (int i = vAttendee.size() - 1; i >= 0; i--) {
		vls.SetString(vAttendee.at(i).GetAccount().c_str(), vAttendee.at(i).GetAccount().length());
		vlarry.PushBack(vls, allocator);
	}
	doc.AddMember("attendLists", vlarry, allocator);

	StringBuffer buffer(0, 1024);
	Writer<StringBuffer> writer(buffer);
	doc.Accept(writer);
	std::string sbody = buffer.GetString();

	request.Init(url, HTTP_METHOD_POST);
	request.AddHeaderInfo(HTTP_HEAD_CONTENT_TYPE, HTTP_HEAD_CONTENT_TYPE);
	request.SetBody(sbody);
}

void CalendarRequestFactory::MakeRequestOfCreateMeetingInMeetingRoom(const Meeting& meeting, const std::string& roomId, HTTPRequest& request)
{
	//构建request url
	std::string urlMethod(MEETING_ROOM_CREATE_MEETING_METHOD);
	std::string url = _calendarService->GetRequestUrl(urlMethod);

	//构建request body
	Document doc;
	doc.SetObject();
	Document::AllocatorType& allocator = doc.GetAllocator();
	doc.AddMember(KEY_USERID, _calendarService->GetUserId(), allocator);
	doc.AddMember(KEY_STARTTIME, meeting.GetStartTime(), allocator);
	doc.AddMember(KEY_LENGTH, meeting.GetDuration(), allocator);

	Value vls(rapidjson::kStringType);
	const std::string &sTitle = meeting.GetTitle();
	vls.SetString(sTitle.c_str(), sTitle.length());
	doc.AddMember(KEY_TITLE, vls, allocator);
	const std::string &summary = meeting.GetSummary();
	vls.SetString(summary.c_str(), summary.length());
	doc.AddMember(KEY_SUMMARY, vls, allocator);
	vls.SetString(roomId.c_str(), roomId.length());
	doc.AddMember("roomId", vls, allocator);
	std::string sSessionId = _calendarService->GetSessionId();
	vls.SetString(sSessionId.c_str(), sSessionId.length());
	doc.AddMember(KEY_SESSIONID, vls, allocator);

// 	std::stringstream attendeesStream;
// 	attendeesStream << "[";
// 	const std::vector<Attendee>& vAttendee = meeting.GetAttendees();
// 
// 	for (int i = vAttendee.size() - 1; i >= 0; i--) {
// 		attendeesStream << vAttendee.at(i).GetAccount();
// 		if (i > 0) {
// 			attendeesStream << ",";
// 		}
// 	}
// 	attendeesStream << "]";
// 
// 	std::string sAttendees = attendeesStream.str();
// 	vls.SetString(sAttendees.c_str(), sAttendees.length());
	const std::vector<Attendee>& vAttendee = meeting.GetAttendees();
	Value vlarry(rapidjson::kArrayType);
	vlarry.SetArray();
	for (int i = vAttendee.size() - 1; i >= 0; i--) {
		vls.SetString(vAttendee.at(i).GetAccount().c_str(), vAttendee.at(i).GetAccount().length());
		vlarry.PushBack(vls, allocator);
	}
	doc.AddMember(KEY_ATTENDEES, vlarry, allocator);

	StringBuffer buffer(0, 1024);
	Writer<StringBuffer> writer(buffer);
	doc.Accept(writer);
	std::string sbody = buffer.GetString();

	request.Init(url, HTTP_METHOD_POST);
	request.AddHeaderInfo(HTTP_HEAD_CONTENT_TYPE, HTTP_HEAD_CONTENT_TYPE);
	request.SetBody(sbody);
}

void CalendarRequestFactory::MakeRequestOfGetMeetingRoomList(int64_t orgId, HTTPRequest& request)
{
	//构建request url
	std::string urlMethod(MEETING_ROOM_LIST_METHOD);
	std::string url = _calendarService->GetRequestUrl(urlMethod);

	//构建request body
	Document doc;
	doc.SetObject();
	Document::AllocatorType& allocator = doc.GetAllocator();
	doc.AddMember(KEY_USERID, _calendarService->GetUserId(), allocator);
	doc.AddMember("orgId", orgId, allocator);

	Value vls(rapidjson::kStringType);
	std::string sSessionId = _calendarService->GetSessionId();
	vls.SetString(sSessionId.c_str(), sSessionId.length());
	doc.AddMember(KEY_SESSIONID, vls, allocator);


	StringBuffer buffer(0, 1024);
	Writer<StringBuffer> writer(buffer);
	doc.Accept(writer);
	std::string sbody = buffer.GetString();

	request.Init(url, HTTP_METHOD_POST);
	request.AddHeaderInfo(HTTP_HEAD_CONTENT_TYPE, HTTP_HEAD_CONTENT_TYPE);
	request.SetBody(sbody);
}

void CalendarRequestFactory::MakeRequestOfCheckJoinMeetingRoom(const std::string &roomId, HTTPRequest& request)
{
	//构建request url
	std::string urlMethod(MEETING_ROOM_CHECK_METHOD);
	std::string url = _calendarService->GetRequestUrl(urlMethod);

	//构建request body
	Document doc;
	doc.SetObject();
	Document::AllocatorType& allocator = doc.GetAllocator();
	doc.AddMember(KEY_USERID, _calendarService->GetUserId(), allocator);

	Value vls(rapidjson::kStringType);
	std::string sSessionId = _calendarService->GetSessionId();
	vls.SetString(sSessionId.c_str(), sSessionId.length());
	doc.AddMember(KEY_SESSIONID, vls, allocator);
	vls.SetString(roomId.c_str(), roomId.length());
	doc.AddMember("roomId", vls, allocator);


	StringBuffer buffer(0, 1024);
	Writer<StringBuffer> writer(buffer);
	doc.Accept(writer);
	std::string sbody = buffer.GetString();

	request.Init(url, HTTP_METHOD_POST);
	request.AddHeaderInfo(HTTP_HEAD_CONTENT_TYPE, HTTP_HEAD_CONTENT_TYPE);
	request.SetBody(sbody);
}

void CalendarRequestFactory::MakeRequestOfCheckMeetingRoomPassword(const std::string &roomId, const std::string& password, HTTPRequest& request)
{
	//构建request url
	std::string urlMethod(MEETING_ROOM_CHECK_PASSWORD_METHOD);
	std::string url = _calendarService->GetRequestUrl(urlMethod);

	//构建request body
	Document doc;
	doc.SetObject();
	Document::AllocatorType& allocator = doc.GetAllocator();
	doc.AddMember(KEY_USERID, _calendarService->GetUserId(), allocator);

	Value vls(rapidjson::kStringType);
	std::string sSessionId = _calendarService->GetSessionId();
	vls.SetString(sSessionId.c_str(), sSessionId.length());
	doc.AddMember(KEY_SESSIONID, vls, allocator);
	vls.SetString(roomId.c_str(), roomId.length());
	doc.AddMember("roomId", vls, allocator);
	vls.SetString(password.c_str(), password.length());
	doc.AddMember("roomPassword", vls, allocator);


	StringBuffer buffer(0, 1024);
	Writer<StringBuffer> writer(buffer);
	doc.Accept(writer);
	std::string sbody = buffer.GetString();

	request.Init(url, HTTP_METHOD_POST);
	request.AddHeaderInfo(HTTP_HEAD_CONTENT_TYPE, HTTP_HEAD_CONTENT_TYPE);
	request.SetBody(sbody);
}

void CalendarRequestFactory::MakeRequestOfDeleteMeetingRoom(const std::string &roomId, HTTPRequest& request)
{
	//构建request url
	std::string urlMethod(MEETING_ROOM_DELETE_METHOD);
	std::string url = _calendarService->GetRequestUrl(urlMethod);

	//构建request body
	Document doc;
	doc.SetObject();
	Document::AllocatorType& allocator = doc.GetAllocator();
	doc.AddMember(KEY_USERID, _calendarService->GetUserId(), allocator);

	Value vls(rapidjson::kStringType);
	std::string sSessionId = _calendarService->GetSessionId();
	vls.SetString(sSessionId.c_str(), sSessionId.length());
	doc.AddMember(KEY_SESSIONID, vls, allocator);
	vls.SetString(roomId.c_str(), roomId.length());
	doc.AddMember("roomId", vls, allocator);


	StringBuffer buffer(0, 1024);
	Writer<StringBuffer> writer(buffer);
	doc.Accept(writer);
	std::string sbody = buffer.GetString();

	request.Init(url, HTTP_METHOD_POST);
	request.AddHeaderInfo(HTTP_HEAD_CONTENT_TYPE, HTTP_HEAD_CONTENT_TYPE);
	request.SetBody(sbody);
}

void CalendarRequestFactory::MakeRequestOfCheckRights(HTTPRequest& request)
{
	//构建request url
	std::string urlMethod(MEETING_ROOM_CHECK_RIGHTS_METHOD);
	std::string url = _calendarService->GetRequestUrl(urlMethod);

	//构建request body
	Document doc;
	doc.SetObject();
	Document::AllocatorType& allocator = doc.GetAllocator();
	doc.AddMember(KEY_USERID, _calendarService->GetUserId(), allocator);

	Value vls(rapidjson::kStringType);
	std::string sSessionId = _calendarService->GetSessionId();
	vls.SetString(sSessionId.c_str(), sSessionId.length());
	doc.AddMember(KEY_SESSIONID, vls, allocator);

	StringBuffer buffer(0, 1024);
	Writer<StringBuffer> writer(buffer);
	doc.Accept(writer);
	std::string sbody = buffer.GetString();

	request.Init(url, HTTP_METHOD_POST);
	request.AddHeaderInfo(HTTP_HEAD_CONTENT_TYPE, HTTP_HEAD_CONTENT_TYPE);
	request.SetBody(sbody);
}

void CalendarRequestFactory::MakeRequestOfGetUserBusyFreeInfo(const std::vector<int32_t>& userIdList, int64_t startTime, int64_t endTime, int32_t shareUserId, HTTPRequest& request)
{
	//构建request url
	///uniform/rs/conference/getUserBusyFreeInfo
	std::string urlMethod(BUSYFREE_INFO_METHOD);
	std::string url = _calendarService->GetRequestUrl(urlMethod);

	//构建request body
	//{"userId":1189278,"sessionId":"71722dd6fe666b41a0c93e4914db16d9b33231fe","shareUserId":1189278,"startTime":1433952000,"endTime":1434038400,"timeSpace":5,"userList":"217705;477828;1189278;1382762"}
	Document doc;
	doc.SetObject();
	Document::AllocatorType& allocator = doc.GetAllocator();
	doc.AddMember(KEY_USERID, _calendarService->GetUserId(), allocator);
	doc.AddMember(KEY_STARTTIME, startTime, allocator);
	doc.AddMember(KEY_ENDTIME, endTime, allocator);
	doc.AddMember(KEY_SHAREUSERID, shareUserId, allocator);

	Value vls(rapidjson::kStringType);
	std::string sSessionId = _calendarService->GetSessionId();
	vls.SetString(sSessionId.c_str(), sSessionId.length());
	doc.AddMember(KEY_SESSIONID, vls, allocator);

	std::stringstream userListStream;
	for (int32_t id : userIdList) {
		userListStream << id << ";";
	}

	std::string sUserList = userListStream.str();
	sUserList = sUserList.substr(0, sUserList.length() - 1);
	vls.SetString(sUserList.c_str(), sUserList.length());
	doc.AddMember(KEY_USERLIST, vls, allocator);

	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);
	doc.Accept(writer);
	std::string sbody = buffer.GetString();

	request.Init(url, HTTP_METHOD_POST);
	request.AddHeaderInfo(HTTP_HEAD_CONTENT_TYPE, HTTP_HEAD_CONTENT_TYPE_VALUE);
	request.SetBody(sbody);
}

}


