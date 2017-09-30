///////////////////////////////////////////////////////////
//  ConferenceDao.cpp
//  Implementation of the Class ConferenceDao
//  Created on:      22-五月-2015 10:36:00
//  Original author: Administrator
///////////////////////////////////////////////////////////

#include "ConferenceDao.h"
#include "LogManager.h"
#include "UCTime.h"

#include <list>
#include <limits>

namespace uc {

using namespace std;
#undef max
ConferenceDao::ConferenceDao() {
	_db = SQLiteManager::GetInstance().GetUserDB();
}

ConferenceDao::~ConferenceDao() {
	SQLiteManager::GetInstance().ReleaseUserDB(_db);
}

int ConferenceDao::SaveMeeting(const Meeting& data) {
	ExecDML("BEGIN TRANSACTION;", _db);
	int ret = SaveMeetingInfo(data);
	ExecDML("COMMIT TRANSACTION;", _db);
	return ret;
}

int ConferenceDao::SaveMeetingInfo(const Meeting& data) {
	//会议基本信息
	CppSQLite3Statement stmt = _db->compileStatement(
		"INSERT OR REPLACE INTO TBL_CONFERENCE(EVENT_ID, PARENT_ID, "
		"START_TIME, ORI_TIME, AUTHORIZER_ID, DURATION, CONF_ID, CONF_TITLE, CONF_STATUS, CONF_SUMMARY, "
		"CONF_LOCATION, HOST_ID, HOST_NAME, HOST_PWD, ATTENDEE_PWD, BILLING_CODE, RELATE_GRPID, "
		"TIME_ZONE, IS_GNET, IS_WHOLEDAY, IS_CYCLE, REPEAT_TYPE, INTERVAL, MONTHNO, WEEKNO, DAYNO, WEEKDAYS, REPEAT_COUNT, REPEAT_ENDDATE, LANGUAGE, EMAIL_LANGUAGE, "
		"LAST_UPDATE_TIME, IS_OLD, CONF_EXTRASINFO, ROOM_ID) "
		"VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);");
	int i = 1;
	stmt.bind(i++, data.GetEventId());
	stmt.bind(i++, data.GetParentId());
	stmt.bind(i++, data.GetStartTime());
	stmt.bind(i++, data.GetOriTime());
	stmt.bind(i++, data.GetAuthorizerId());
	stmt.bind(i++, data.GetDuration());
	stmt.bind(i++, data.GetConferneceId().c_str());
	stmt.bind(i++, data.GetTitle().c_str());
	stmt.bind(i++, data.GetStatus());
	stmt.bind(i++, data.GetSummary().c_str());
	stmt.bind(i++, data.GetLocation().c_str());
	stmt.bind(i++, data.GetHostId());
	stmt.bind(i++, data.GetHostName().c_str());
	stmt.bind(i++, data.GetHostPassword().c_str());
	stmt.bind(i++, data.GetAttendeePassword().c_str());
	stmt.bind(i++, data.GetBillingCode().c_str());
	stmt.bind(i++, data.GetGroupId());
	stmt.bind(i++, data.GetTimeZone().c_str());
	stmt.bind(i++, data.IsGNet());
	stmt.bind(i++, data.IsWholeDay());
	stmt.bind(i++, data.IsCycle());
	stmt.bind(i++, data.GetCycleRole().RepeatType().c_str());
	stmt.bind(i++, data.GetCycleRole().Interval());
	stmt.bind(i++, data.GetCycleRole().MonthNo());
	stmt.bind(i++, data.GetCycleRole().WeekNo());
	stmt.bind(i++, data.GetCycleRole().DayNo());
	stmt.bind(i++, data.GetCycleRole().Weekdays().c_str());
	stmt.bind(i++, data.GetCycleRole().RepeatCount());
	stmt.bind(i++, data.GetCycleRole().RepeatEndDate());
	stmt.bind(i++, data.GetEmailLanguage());
	stmt.bind(i++, data.GetLanguage());
	stmt.bind(i++, data.GetLastUpdateTime());
	stmt.bind(i++, data.IsOld());
	stmt.bind(i++, data.GetExtrasInfo().c_str());
	stmt.bind(i++, data.GetRoomId().c_str());

	int ret = ExecDML(stmt);
	stmt.reset();
	if (ret != 0) {
		UC_LOG(ERROR) << "SaveMeetingInfo return error:" << ret
			<< "; conf_id:" << data.GetConferneceId()
			<< ", event_id:" << data.GetEventId()
			<< ", parent_id:" << data.GetParentId()
			<< ", conf_name:" << data.GetTitle()
			<< ", start_time:" << data.GetStartTime()
			<< ", ori_time:" << data.GetOriTime()
			<< ", status:" << data.GetStatus()
			<< ", authorizer_id:" << data.GetAuthorizerId()
			<< ", duration:" << data.GetDuration()
			<< ", time_zone:" << data.GetTimeZone()
			<< ", conf_address:" << data.GetLocation()
			<< ", conf_desc:" << data.GetSummary()
			<< ", host_name:" << data.GetHostName()
			<< ", host_id:" << data.GetHostId()
			<< ", host_pwd:" << data.GetHostPassword()
			<< ", conf_pwd:" << data.GetAttendeePassword()
			<< ", relate_grpid:" << data.GetGroupId()
			<< ", is_gnet:" << data.IsGNet()
			<< ", is_cycle:" << data.IsCycle()
			<< ", email_language:" << data.GetEmailLanguage()
			<< ", language:" << data.GetLanguage()
			<< ", billing_code:" << data.GetBillingCode()
			<< ", is_Old:" << data.IsOld()
			<< ", extrasInfo:" << data.GetExtrasInfo()
			<< ", room_id:" << data.GetRoomId();
		return DBError::DB_UPDATE_ERROR;
	}

	//会议参会人列表信息
	DeleteAttendees(data.GetEventId(), data.GetStartTime(), data.GetAuthorizerId());
	ret = SaveAttendees(data.GetEventId(), data.GetStartTime(), data.GetAuthorizerId(), data.GetAttendees());
	if (ret != 0) {
		UC_LOG(ERROR) << "SaveAttendee return error:" << ret << ", eventId = " << data.GetEventId();
		return DBError::DB_INSERT_ERROR;
	}
	return DBError::DB_SUCCESS;
}

int ConferenceDao::SaveAttendees(int64_t eventId, int64_t startTime, int authorizerId, const std::vector<Attendee>& attendees) {
	int ret = DBError::DB_SUCCESS;
	for (std::vector<Attendee>::const_iterator itor = attendees.begin();
		itor != attendees.end(); ++itor) {
		ret = AddAttendee(eventId, startTime, authorizerId, *itor);
		if (ret != 0) {
			break;
		}
	}
	return ret;
}

int ConferenceDao::AddAttendee(int64_t eventId, int64_t startTime, int authorizerId, const Attendee& attendee) {
	CppSQLite3Statement stmt = _db->compileStatement(
			"INSERT OR REPLACE INTO TBL_CONF_MEMBER_RELATION(EVENT_ID, START_TIME, AUTHORIZER_ID, USER_ACCOUNT, USER_NAME, USER_TYPE, USER_STATUS) "
			"VALUES(?, ?, ?, ?, ?, ?, ?);");
		int i = 1;
		stmt.bind(i++, eventId);
		stmt.bind(i++, startTime);
		stmt.bind(i++, authorizerId);
		stmt.bind(i++, attendee.GetAccount().c_str());
		stmt.bind(i++, attendee.GetName().c_str());
		stmt.bind(i++, attendee.GetType());
		stmt.bind(i++, attendee.GetStatus());

	int ret = ExecDML(stmt); 
	stmt.reset();
	if (ret != 0) {
		UC_LOG(ERROR) << "AddAttendee return error:" << ret
			<< ", event_id:" << eventId
			<< ", start_time:" << startTime
			<< ", authorizer_Id:" << authorizerId
			<< ", user_account:" << attendee.GetAccount()
			<< ", user_name:" << attendee.GetName()
			<< ", user_type:" << attendee.GetType()
			<< ", user_state:" << attendee.GetStatus();
		return DBError::DB_INSERT_ERROR;
	}
	return DBError::DB_SUCCESS;
}

int ConferenceDao::DeleteAttendee(int64_t eventId, int64_t startTime, int32_t authorizerId, const std::string& attendeeAccount) {
	CppSQLite3Buffer sql;
	if (startTime > 0) {
		sql.format("DELETE FROM TBL_CONF_MEMBER_RELATION WHERE EVENT_ID = %lld AND START_TIME = %lld AND AUTHORIZER_ID = %ld AND USER_ACCOUNT = %s;", eventId, startTime, authorizerId, attendeeAccount.c_str());
	}
	else {
		sql.format("DELETE FROM TBL_CONF_MEMBER_RELATION WHERE EVENT_ID = %lld AND AUTHORIZER_ID = %ld AND USER_ACCOUNT = %s;", eventId, authorizerId, attendeeAccount.c_str());
	}
	
	int ret = ExecDML((const char*)sql, _db);
	if (ret != 0) {
		UC_LOG(ERROR) << "DeleteAttendee return error:" << ret << ", sql:" << (const char*)sql;
		ret = DBError::DB_DELETE_ERROR;
	}
	return ret;
}

int ConferenceDao::DeleteAttendees(int64_t eventId, int64_t startTime/* = 0*/, int64_t authorizerId/* = 0*/) {
	CppSQLite3Buffer sql;
	if (startTime > 0) {
		sql.format("DELETE FROM TBL_CONF_MEMBER_RELATION WHERE EVENT_ID = %lld AND START_TIME = %lld AND AUTHORIZER_ID = %ld;", eventId, startTime, authorizerId);
	}
	else {
		sql.format("DELETE FROM TBL_CONF_MEMBER_RELATION WHERE EVENT_ID = %lld AND AUTHORIZER_ID = %ld;", eventId, authorizerId);
	}
	
	int ret = ExecDML((const char*)sql, _db);
	if (ret != 0) {
		UC_LOG(ERROR) << "DeleteAttendees return error:" << ret << ", sql:" << (const char*)sql;
		ret = DBError::DB_DELETE_ERROR;
	}
	return ret;
}

int ConferenceDao::SaveMeetings(const vector<Meeting>& data) {
	int ret = DBError::DB_SUCCESS;
	ExecDML("BEGIN TRANSACTION;", _db);
	for (vector<Meeting>::const_iterator itor = data.begin();
		itor != data.end(); ++itor) {
		ret = SaveMeetingInfo(*itor);
		if (ret != 0) {
			break;
		}
	}
	ExecDML("COMMIT TRANSACTION;", _db);
	return ret;
}


int ConferenceDao::UpdateAttendees(int64_t eventId, int64_t startTime, int32_t authorizerId, const std::vector<Attendee>& attendees) {
	DeleteAttendees(eventId, startTime, authorizerId);
	return SaveAttendees(eventId, startTime, authorizerId, attendees);
}

int ConferenceDao::UpdateMeetingStatus(uc::MeetingStatus status, int64_t eventId, int64_t startTime, int32_t authorizerId) {
	CppSQLite3Statement stmt;
	if (startTime > 0) {
		stmt = _db->compileStatement(
			"UPDATE TBL_CONFERENCE SET CONF_STATUS = ? WHERE EVENT_ID = ? AND START_TIME = ? AND AUTHORIZER_ID = ?;");
		int i = 1;
		stmt.bind(i++, status);
		stmt.bind(i++, eventId);
		stmt.bind(i++, startTime);
		stmt.bind(i++, authorizerId);
	}
	else {
		stmt = _db->compileStatement(
			"UPDATE TBL_CONFERENCE SET CONF_STATUS = ? WHERE EVENT_ID = ? AND AUTHORIZER_ID = ?;");
		int i = 1;
		stmt.bind(i++, status);
		stmt.bind(i++, eventId);
		stmt.bind(i++, authorizerId);
	}

	int ret = ExecDML(stmt);
	if (ret != 0) {
		UC_LOG(ERROR) << "UpdateMeetingStatus return error:" << ret
			<< "; status:" << status
			<< ", eventId:" << eventId
			<< ", startTime:" << startTime
			<< ", authorizerId:" << authorizerId;
		ret = DBError::DB_UPDATE_ERROR;
	}
	return ret;
}

int ConferenceDao::UpdateAttendeeStatus(uc::AttendeeStatus status, const std::string& account, int64_t eventId, int64_t startTime, int32_t authorizerId) {
	CppSQLite3Statement stmt;
	if (startTime > 0) {
		stmt = _db->compileStatement(
			"UPDATE TBL_CONF_MEMBER_RELATION SET USER_STATUS = ? WHERE EVENT_ID = ? AND START_TIME = ? AND AUTHORIZER_ID = ? AND USER_ACCOUNT = ?;");
		int i = 1;
		stmt.bind(i++, status);
		stmt.bind(i++, eventId);
		stmt.bind(i++, startTime);
		stmt.bind(i++, authorizerId);
		stmt.bind(i++, account.c_str());
	}
	else {
		stmt = _db->compileStatement(
			"UPDATE TBL_CONF_MEMBER_RELATION SET USER_STATUS = ? WHERE EVENT_ID = ? AND AUTHORIZER_ID = ? AND USER_ACCOUNT = ?;");
		int i = 1;
		stmt.bind(i++, status);
		stmt.bind(i++, eventId);
		stmt.bind(i++, authorizerId);
		stmt.bind(i++, account.c_str());
	}

	int ret = ExecDML(stmt);
	if (ret != 0) {
		UC_LOG(ERROR) << "UpdateMeetingStatus return error:" << ret
			<< "; status:" << status
			<< ", eventId:" << eventId
			<< ", startTime:" << startTime
			<< ", authorizerId:" << authorizerId
			<< ", account:" << account;
		ret = DBError::DB_UPDATE_ERROR;
	}
	return ret;
}

int ConferenceDao::GetMeetingByEventId(int64_t eventId, int64_t startTime, int32_t authorizerId, Meeting& data) {
	
	UC_LOG(ERROR) << "ConferenceDao::GetMeetingByEventId enter! event_id:" << eventId << ", startTime:" << startTime << ", authorizerId:" << authorizerId;
	CppSQLite3Statement stmt;
	if (startTime > 0) {
		string sql = SELECT_CONF + " WHERE EVENT_ID = ? AND START_TIME = ? AND AUTHORIZER_ID = ?;";
		stmt = _db->compileStatement(sql.c_str());
		int i = 1;
		stmt.bind(i++, eventId);
		stmt.bind(i++, startTime);
		stmt.bind(i++, authorizerId);
	}
	else {
		string sql = SELECT_CONF + " WHERE EVENT_ID = ? AND AUTHORIZER_ID = ?;";
		stmt = _db->compileStatement(sql.c_str());
		int i = 1;
		stmt.bind(i++, eventId);
		stmt.bind(i++, authorizerId);
	}

	CppSQLite3Query query;
	int ret = Query(stmt, query);
	if (ret != 0) {
		UC_LOG(ERROR) << "GetMeeting return error:" << ret 
			<< ", event_id:" << eventId
			<< ", start_time:" << startTime
			<< ", authorizerId:" << authorizerId;
		return DBError::DB_QUERY_ERROR;
	}

	if (!query.eof()) {
		SetConference(query, data);
	} else {
		UC_LOG(ERROR) << "GetMeetingByEventId return db empty error! event_id:" << eventId << ", start_time:" << startTime << ", authorizerId:" << authorizerId;
		return DBError::DB_QUERY_EMPTY;
	}
	return DBError::DB_SUCCESS;
}

int ConferenceDao::GetMeetingByConfId(const std::string& confId, Meeting& data) {

	UC_LOG(ERROR) << "ConferenceDao::GetMeetingByConfId enter! confId:" << confId;
	CppSQLite3Statement stmt;
	string sql = SELECT_CONF + " WHERE CONF_ID = ?;";
	stmt = _db->compileStatement(sql.c_str());
	int i = 1;
	stmt.bind(i++, confId.c_str());

	CppSQLite3Query query;
	int ret = Query(stmt, query);
	if (ret != 0) {
		UC_LOG(ERROR) << "GetMeetingByConfId return error:" << ret
			<< ", confId:" << confId;
		return DBError::DB_QUERY_ERROR;
	}

	if (!query.eof()) {
		SetConference(query, data);
	}
	else {
		UC_LOG(ERROR) << "GetMeetingByConfId return db empty error! confId:" << confId;
		return DBError::DB_QUERY_EMPTY;
	}
	return DBError::DB_SUCCESS;
}

int ConferenceDao::GetConferenceByParentId(int64_t parentId, int64_t oriTime, int32_t authorizerId, Meeting& data) {
	string sql = SELECT_CONF + " WHERE PARENT_ID = ? AND ORI_TIME = ? AND AUTHORIZER_ID = ?;";
	CppSQLite3Statement stmt = _db->compileStatement(sql.c_str());
	int i = 1;
	stmt.bind(i++, parentId);
	stmt.bind(i++, oriTime);
	stmt.bind(i++, authorizerId);
	CppSQLite3Query query;
	int ret = Query(stmt, query);
	if (ret != 0) {
		UC_LOG(ERROR) << "GetConferenceByParentId return error:" << ret
			<< ", parentId:" << parentId
			<< ", oriTime:" << oriTime
			<< ", authorizerId:" << authorizerId;
		return DBError::DB_QUERY_ERROR;
	}

	if (!query.eof()) {
		SetConference(query, data);
	}
	else {
		UC_LOG(ERROR) << "GetConference return db empty error! parentId:" << parentId << ", oriTime:" << oriTime;
		return DBError::DB_QUERY_EMPTY;
	}
	return DBError::DB_SUCCESS;
}


void ConferenceDao::SetConference(CppSQLite3Query& query, Meeting& data) {
	int i = 0;
	data.SetEventId(StringConvert::Convert2Int64(query.fieldValue(i++)));
	data.SetParentId(StringConvert::Convert2Int64(query.fieldValue(i++)));
	data.SetStartTime(StringConvert::Convert2Int64(query.fieldValue(i++)));
	data.SetOriTime(StringConvert::Convert2Int64(query.fieldValue(i++)));
	data.SetAuthorizerId(atoi(query.fieldValue(i++)));
	data.SetDuration(atoi(query.fieldValue(i++)));	
	data.SetConferenceId(query.fieldValue(i++));
	data.SetTitle(query.fieldValue(i++));
	data.SetStatus((MeetingStatus)atoi(query.fieldValue(i++)));
	data.SetSummary(query.fieldValue(i++));
	data.SetLocation(query.fieldValue(i++));
	data.SetHostId(atoi(query.fieldValue(i++)));
	data.SetHostName(query.fieldValue(i++));
	data.SetHostPassword(query.fieldValue(i++));
	data.SetAttendeePassword(query.fieldValue(i++));
	data.SetBillingCode(query.fieldValue(i++));
	data.SetGroupId(StringConvert::Convert2Int64(query.fieldValue(i++)));
	data.SetTimeZone(query.fieldValue(i++));
	data.IsGNet(atoi(query.fieldValue(i++)));
	data.IsWholeDay(atoi(query.fieldValue(i++)));
	data.IsCycle(atoi(query.fieldValue(i++)));
	CycleRole role;
	role.RepeatType(query.fieldValue(i++));
	role.Interval(atoi(query.fieldValue(i++)));
	role.MonthNo(atoi(query.fieldValue(i++)));
	role.WeekNo(atoi(query.fieldValue(i++)));
	role.DayNo(atoi(query.fieldValue(i++)));
	role.Weekdays(query.fieldValue(i++));
	role.RepeatCount(atoi(query.fieldValue(i++)));
	role.RepeatEndDate(StringConvert::Convert2Int64(query.fieldValue(i++)));
	data.SetCycleRole(role);
	data.SetLanguage(atoi(query.fieldValue(i++)));
	data.SetEmailLanguage(atoi(query.fieldValue(i++)));
	data.SetLastUpdateTime(StringConvert::Convert2Int64(query.fieldValue(i++)));
	data.IsOld(atoi(query.fieldValue(i++)));
	data.SetExtrasInfo(query.fieldValue(i++));
	data.SetRoomId(query.fieldValue(i++));
	SetAttendee(data);
}

void ConferenceDao::SetAttendee(Meeting& data) {
	CppSQLite3Statement stmt = _db->compileStatement(
		"SELECT USER_ACCOUNT, USER_NAME, USER_TYPE, USER_STATUS "
		"FROM TBL_CONF_MEMBER_RELATION WHERE EVENT_ID = ? AND START_TIME = ? AND AUTHORIZER_ID = ?;");

	int i = 1;
	stmt.bind(i++, data.GetEventId());
	stmt.bind(i++, data.GetStartTime());
	stmt.bind(i++, data.GetAuthorizerId());
	CppSQLite3Query query;
	int ret = Query(stmt, query);
	if (ret != 0) {
		UC_LOG(ERROR) << "Get attendee return error:" << ret 
			<< ", event_id:" << data.GetEventId()
			<< ", authorizer_id:" << data.GetAuthorizerId()
			<< ", start_time:" << data.GetStartTime();
		return;
	}

	vector<Attendee> confUsers;
	Attendee user;
	while (!query.eof()) {
		int i = 0;
		user.SetAccount(query.fieldValue(i++));
		user.SetName(query.fieldValue(i++));
		user.SetType((uc::AttendeeType)atoi(query.fieldValue(i++)));
		user.SetStatus((uc::AttendeeStatus)atoi(query.fieldValue(i++)));
		confUsers.push_back(user);
		query.nextRow();
	}
	data.SetAttendees(confUsers);
}

int ConferenceDao::GetConferenceByEventId(int64_t eventId, int64_t startTime, int64_t endTime, int32_t authorizerId, vector<Meeting>& data) {
	if (endTime <= 0){
		endTime = std::numeric_limits<int64_t>::max();
	}
	string sql = SELECT_CONF + "  WHERE EVENT_ID = ? AND START_TIME >= ? AND START_TIME <= ? AND AUTHORIZER_ID = ?;";
	CppSQLite3Statement stmt = _db->compileStatement(sql.c_str());
	int i = 1;
	stmt.bind(i++, eventId);
	stmt.bind(i++, startTime);
	stmt.bind(i++, endTime);
	stmt.bind(i++, authorizerId);
	CppSQLite3Query query;
	int ret = Query(stmt, query);
	if (ret != 0) {
		UC_LOG(ERROR) << "GetConference return error:" << ret
			<< "; eventId:" << eventId
			<< ", startTime:" << startTime
			<< ", endTime:" << endTime
			<< ", authorizerId:" << authorizerId;
		return DBError::DB_QUERY_ERROR;
	}

	if (query.eof()) {
		UC_LOG(ERROR) << "GetConference return db empty!" << "; eventId:" << eventId
			<< ", startTime:" << startTime << ", endTime:" << endTime << ", authorizerId:" << authorizerId;
		return DBError::DB_QUERY_EMPTY;
	}

	Meeting info;
	while (!query.eof()) {
		SetConference(query, info);
		data.push_back(info);
		query.nextRow();
	}
	return DBError::DB_SUCCESS;
}
int ConferenceDao::GetMeetingByIds(const vector<int64_t>& eventIds, int64_t startTime, 
					int64_t endTime, int32_t authorizerId, vector<Meeting>& data) {
	if (eventIds.empty()) {
		UC_LOG(ERROR) << "GetConferenceByIds input eventIds is empty!";
		return DBError::DB_INPUT_ERROR;
	}

	if (endTime <= 0){
		endTime = std::numeric_limits<int64_t>::max();
	}

	stringstream events;
	for (vector<int64_t>::const_iterator itor = eventIds.begin();
		itor != eventIds.end(); ++itor) {
			events << *itor << ",";
	}
	string eventListList(events.str());
	eventListList.erase(eventListList.length() - 1);

	CppSQLite3Buffer sql;
	sql.format("%s WHERE "
		"EVENT_ID IN (%s) AND AUTHORIZER_ID = %d AND START_TIME >= %lld AND START_TIME <= %lld;", 
		SELECT_CONF.c_str(), eventListList.c_str(), authorizerId, startTime, endTime);
	CppSQLite3Query query;
	int ret = Query((const char*)sql, _db, query);
	if (ret != 0) {
		UC_LOG(ERROR) << "GetConferenceByIds query return error:" << ret << ", sql:" << (const char*)sql;
		return DBError::DB_QUERY_ERROR;
	}

	Meeting info;
	while(!query.eof()) {
		SetConference(query, info);
		data.push_back(info);
		query.nextRow();
	}
	return DBError::DB_SUCCESS;
}

int ConferenceDao::GetMeetingList(int32_t authorizerId, int64_t startTime, int64_t endTime, int32_t limit, int64_t eventIdFilter, bool bFilterAsc, vector<Meeting>& data) {
	startTime = startTime < 0 ? 0 : startTime;
	endTime = endTime <= 0 ? std::numeric_limits<int64_t>::max() : endTime;
	limit = limit < 0 ? 0 : limit;
	CppSQLite3Statement stmt;
	if (eventIdFilter > 0) {
		string sql;
		if (bFilterAsc) {
			sql = SELECT_CONF + " WHERE AUTHORIZER_ID = ? AND START_TIME >= ? AND START_TIME <= ? AND EVENT_ID <= ? ORDER BY START_TIME ASC LIMIT 0, ?;";
		} else {
			sql = SELECT_CONF + " WHERE AUTHORIZER_ID = ? AND START_TIME >= ? AND START_TIME <= ? AND EVENT_ID >= ? ORDER BY START_TIME DESC LIMIT 0, ?;";
		}

		stmt = _db->compileStatement(sql.c_str());
		int i = 1;
		stmt.bind(i++, authorizerId);
		stmt.bind(i++, startTime);
		stmt.bind(i++, endTime);
		stmt.bind(i++, eventIdFilter);
		stmt.bind(i++, limit);
	}
	else {
		string sql;
		if (bFilterAsc) {
			sql = SELECT_CONF + " WHERE AUTHORIZER_ID = ? AND (START_TIME >= ? OR (START_TIME+DURATION*60) > ?) AND START_TIME <= ? ORDER BY START_TIME ASC LIMIT 0, ?;";
			int i = 1;
			stmt = _db->compileStatement(sql.c_str());
			stmt.bind(i++, authorizerId);
			stmt.bind(i++, startTime);
			stmt.bind(i++, startTime);
			stmt.bind(i++, endTime);
			stmt.bind(i++, limit);
		}
		else {
			sql = SELECT_CONF + " WHERE AUTHORIZER_ID = ? AND START_TIME >= ? AND START_TIME <= ? ORDER BY START_TIME DESC LIMIT 0, ?;";
			stmt = _db->compileStatement(sql.c_str());
			int i = 1;
			stmt.bind(i++, authorizerId);
			stmt.bind(i++, startTime);
			stmt.bind(i++, endTime);
			stmt.bind(i++, limit);
		}
	}

	CppSQLite3Query query;
	int ret = Query(stmt, query);
	if (ret != 0) {
		UC_LOG(ERROR) << "GetConference return error:" << ret
			<< "; authorizerId:" << authorizerId
			<< ", start_time:" << startTime
			<< ", end_time:" << endTime
			<< ", limit:" << limit
			<< ", eventFilter:" << eventIdFilter
			<< ", bFilterAsc:" << bFilterAsc;
		return DBError::DB_QUERY_ERROR;
	}

	if (query.eof()) {
		UC_LOG(ERROR) << "GetConference return db empty!" << "authorizerId:" << authorizerId 
			<< ", start_time:" << startTime << ", end_time:" << endTime << ", limit:" << limit
			<< ", eventFilter:" << eventIdFilter << ", bFilterAsc:" << bFilterAsc;;
		return DBError::DB_QUERY_EMPTY;
	}

	Meeting info;
	while(!query.eof()) {
		SetConference(query, info);
		data.push_back(info);
		query.nextRow();
	}
	return DBError::DB_SUCCESS;
}

int ConferenceDao::GetOldMeetingList(int32_t authorizerId, int64_t endTime, int32_t limit, int64_t eventIdFilter, vector<Meeting>& data) {
	CppSQLite3Statement stmt;
	if (eventIdFilter > 0) {
		string sql = SELECT_CONF + " WHERE AUTHORIZER_ID = ? AND START_TIME <= ? AND EVENT_ID >= ? ORDER BY START_TIME DESC LIMIT 0, ?;";
		stmt = _db->compileStatement(sql.c_str());
		int i = 1;
		stmt.bind(i++, authorizerId);
		stmt.bind(i++, endTime);
		stmt.bind(i++, eventIdFilter);
		stmt.bind(i++, limit);
	} else {
		string sql = SELECT_CONF + " WHERE AUTHORIZER_ID = ? AND START_TIME <= ? ORDER BY START_TIME DESC LIMIT 0, ?;";
		stmt = _db->compileStatement(sql.c_str());
		int i = 1;
		stmt.bind(i++, authorizerId);
		stmt.bind(i++, endTime);
		stmt.bind(i++, limit);
	}

	CppSQLite3Query query;
	int ret = Query(stmt, query);
	if (ret != 0) {
		UC_LOG(ERROR) << "GetOldMeetingList return error:" << ret
			<< "; authorizerId:" << authorizerId
			<< ", end_time:" << endTime
			<< ", limit:" << limit
			<< ", eventIdFilter:" << eventIdFilter;
		return DBError::DB_QUERY_ERROR;
	}

	if (query.eof()) {
		UC_LOG(ERROR) << "GetOldMeetingList return db empty!" << "authorizerId:" << authorizerId 
			<< ", end_time:" << endTime << ", limit:" << limit << ", eventIdFilter:" << eventIdFilter;
		return DBError::DB_QUERY_EMPTY;
	}

	Meeting info;
	while(!query.eof()) {
		SetConference(query, info);
		data.push_back(info);
		query.nextRow();
	}
	return DBError::DB_SUCCESS;
}

int ConferenceDao::GetNewMeetingList(int32_t authorizerId, int64_t startTime, int32_t limit, int64_t eventIdFilter, vector<Meeting>& data) {
	
	CppSQLite3Statement stmt;
	if (eventIdFilter > 0) {
		string sql = SELECT_CONF + " WHERE AUTHORIZER_ID = ? AND START_TIME >= ? AND EVENT_ID <= ? ORDER BY START_TIME ASC LIMIT 0, ?;";
		stmt = _db->compileStatement(sql.c_str());
		int i = 1;
		stmt.bind(i++, authorizerId);
		stmt.bind(i++, startTime);
		stmt.bind(i++, eventIdFilter);
		stmt.bind(i++, limit);
	}
	else {
		string sql = SELECT_CONF + " WHERE AUTHORIZER_ID = ? AND START_TIME >= ? ORDER BY START_TIME ASC LIMIT 0, ?;";
		stmt = _db->compileStatement(sql.c_str());
		int i = 1;
		stmt.bind(i++, authorizerId);
		stmt.bind(i++, startTime);
		stmt.bind(i++, limit);
	}

	CppSQLite3Query query;
	int ret = Query(stmt, query);
	if (ret != 0) {
		UC_LOG(ERROR) << "GetNewConferenceList return error:" << ret
			<< "; authorizerId:" << authorizerId
			<< ", startTime:" << startTime
			<< ", limit:" << limit
			<< ", eventIdFilter:" << eventIdFilter;
		return DBError::DB_QUERY_ERROR;
	}

	if (query.eof()) {
		UC_LOG(ERROR) << "GetNewConferenceList return db empty!" << "authorizerId:" << authorizerId 
			<< ", startTime:" << startTime << ", limit:" << limit << ", eventIdFilter:" << eventIdFilter;
		return DBError::DB_QUERY_EMPTY;
	}

	Meeting info;
	while(!query.eof()) {
		SetConference(query, info);
		data.push_back(info);
		query.nextRow();
	}
	return DBError::DB_SUCCESS;
}


int ConferenceDao::DeleteMeetings(const vector<Meeting>& data) {
	int ret = DBError::DB_SUCCESS;
	ExecDML("BEGIN TRANSACTION;", _db);
	for (vector<Meeting>::const_iterator itor = data.begin();
		itor != data.end(); ++itor) {
		ret = DeleteMeetingByEventId(itor->GetEventId(), itor->GetStartTime(), itor->GetAuthorizerId());
	}
	ExecDML("COMMIT TRANSACTION;", _db);
	return ret;
}

int ConferenceDao::DeleteMeetingByEventId(int64_t eventId, int64_t oriStart, int32_t authorizerId) {
	UC_LOG(ERROR) << "ConferenceDao::DeleteMeetingByEventId enter! event_id:" << eventId << ", oriStart:" << oriStart << ", authorizerId:" << authorizerId;
	authorizerId = authorizerId < 0 ? 0 : authorizerId;
	CppSQLite3Buffer sql;
	if (oriStart > 0) {
		sql.format("DELETE FROM TBL_CONFERENCE WHERE EVENT_ID = %lld AND START_TIME = %lld AND AUTHORIZER_ID = %d;"
			"DELETE FROM TBL_CONF_MEMBER_RELATION WHERE EVENT_ID = %lld AND START_TIME = %lld AND AUTHORIZER_ID = %d;",
			eventId, oriStart, authorizerId, eventId, oriStart, authorizerId);
	}
	else {
		sql.format("DELETE FROM TBL_CONFERENCE WHERE EVENT_ID = %lld AND AUTHORIZER_ID = %d;"
			"DELETE FROM TBL_CONF_MEMBER_RELATION WHERE EVENT_ID = %lld AND AUTHORIZER_ID = %d;",
			eventId, authorizerId, eventId, authorizerId);
	}


	int ret = ExecDML((const char*)sql, _db);
	if (ret != 0) {
		UC_LOG(ERROR) << "DeleteMeetingByEventId return error:" << ret << ", sql:" << (const char*)sql;
		ret = DBError::DB_DELETE_ERROR;
	}
	return ret;
}

int ConferenceDao::DeleteMeetingsWithCommonEventId(int64_t eventId, int64_t startTime, int64_t endTime, int32_t authorizerId/* = 0*/) {
	endTime = endTime <= 0 ? std::numeric_limits<int64_t>::max() : endTime;
	startTime = startTime < 0 ? 0 : startTime;
	authorizerId = authorizerId < 0 ? 0 : authorizerId;
	
	CppSQLite3Buffer sql;
	sql.format("DELETE FROM TBL_CONFERENCE WHERE EVENT_ID = %lld AND START_TIME >= %lld AND START_TIME <= %lld AND AUTHORIZER_ID = %d ;"
		"DELETE FROM TBL_CONF_MEMBER_RELATION WHERE EVENT_ID = %lld AND START_TIME >= %lld AND START_TIME <= %lld AND AUTHORIZER_ID = %d ;",
		eventId, startTime, endTime, authorizerId, eventId, startTime, endTime, authorizerId);
	
	ExecDML("BEGIN TRANSACTION;", _db);
	int ret = ExecDML((const char*)sql, _db);
	ExecDML("COMMIT TRANSACTION;", _db);
	if (ret != 0) {
		UC_LOG(ERROR) << "DeleteConferenceList return error:" << ret << ", sql:" << (const char*)sql;
		return DBError::DB_DELETE_ERROR;
	}
	return DBError::DB_SUCCESS;
}

int ConferenceDao::DeleteMeetingsWithTimeRange(int64_t startTime, int64_t endTime, int32_t limit/* = 200*/, int64_t eventIdFilter /*= 0*/, bool bfilterAsc/* = true*/, int32_t authorizerId/* = 0*/) {
	if (endTime <= 0){
		endTime = std::numeric_limits<int64_t>::max();
	}

	list<ConferenceId> confList;
	if (eventIdFilter <= 0)	{
		CppSQLite3Statement stmt = _db->compileStatement(
			"SELECT EVENT_ID, START_TIME, AUTHORIZER_ID FROM TBL_CONFERENCE WHERE AUTHORIZER_ID = ? AND START_TIME >= ? "
			"AND START_TIME <= ? ORDER BY START_TIME DESC LIMIT ?;");
		int i = 1;
		stmt.bind(i++, authorizerId);
		stmt.bind(i++, startTime);
		stmt.bind(i++, endTime);
		stmt.bind(i++, limit);
		CppSQLite3Query query;
		int ret = Query(stmt, query);
		if (ret != 0) {
			UC_LOG(ERROR) << "DeleteMeetingsWithTimeRange return error:" << ret;
			return DBError::DB_QUERY_ERROR;
		}

		ConferenceId info;
		while (!query.eof()) {
			int i = 0;
			info._eventId = StringConvert::Convert2Int64(query.fieldValue(i++));
			info._startTime = StringConvert::Convert2Int64(query.fieldValue(i++));
			info._authorizerId = atoi(query.fieldValue(i++));
			confList.push_back(info);
			query.nextRow();
		}

		ExecDML("BEGIN TRANSACTION;", _db);
		for (list<ConferenceId>::iterator itor = confList.begin();
			itor != confList.end(); ++itor) {
			DeleteMeetingByEventId(itor->_eventId, itor->_startTime, itor->_authorizerId);
		}
		ExecDML("COMMIT TRANSACTION;", _db);
	}
	else {
		if (bfilterAsc)
		{
			CppSQLite3Statement stmt = _db->compileStatement(
				"SELECT EVENT_ID, START_TIME, AUTHORIZER_ID FROM TBL_CONFERENCE WHERE AUTHORIZER_ID = ? AND START_TIME >= ? "
				"AND START_TIME <= ? AND EVENT_ID <= ? ORDER BY EVENT_ID DESC LIMIT ?;");
			int i = 1;
			stmt.bind(i++, authorizerId);
			stmt.bind(i++, startTime);
			stmt.bind(i++, endTime);
			stmt.bind(i++, eventIdFilter);
			stmt.bind(i++, limit);
			CppSQLite3Query query;
			int ret = Query(stmt, query);
			if (ret != 0) {
				UC_LOG(ERROR) << "DeleteMeetingsWithTimeRange return error:" << ret;
				return DBError::DB_QUERY_ERROR;
			}

			ConferenceId info;
			while (!query.eof()) {
				int i = 0;
				info._eventId = StringConvert::Convert2Int64(query.fieldValue(i++));
				info._startTime = StringConvert::Convert2Int64(query.fieldValue(i++));
				info._authorizerId = atoi(query.fieldValue(i++));
				confList.push_back(info);
				query.nextRow();
			}

			ExecDML("BEGIN TRANSACTION;", _db);
			for (list<ConferenceId>::iterator itor = confList.begin();
				itor != confList.end(); ++itor) {
				DeleteMeetingByEventId(itor->_eventId, itor->_startTime, itor->_authorizerId);
			}
			ExecDML("COMMIT TRANSACTION;", _db);
		}
		else
		{
			CppSQLite3Statement stmt = _db->compileStatement(
				"SELECT EVENT_ID, START_TIME, AUTHORIZER_ID FROM TBL_CONFERENCE WHERE AUTHORIZER_ID = ? AND START_TIME >= ? "
				"AND START_TIME <= ? AND EVENT_ID >= ? ORDER BY EVENT_ID DESC LIMIT ?;");
			int i = 1;
			stmt.bind(i++, authorizerId);
			stmt.bind(i++, startTime);
			stmt.bind(i++, endTime);
			stmt.bind(i++, eventIdFilter);
			stmt.bind(i++, limit);
			CppSQLite3Query query;
			int ret = Query(stmt, query);
			if (ret != 0) {
				UC_LOG(ERROR) << "DeleteMeetingsWithTimeRange return error:" << ret;
				return DBError::DB_QUERY_ERROR;
			}

			ConferenceId info;
			while (!query.eof()) {
				int i = 0;
				info._eventId = StringConvert::Convert2Int64(query.fieldValue(i++));
				info._startTime = StringConvert::Convert2Int64(query.fieldValue(i++));
				info._authorizerId = atoi(query.fieldValue(i++));
				confList.push_back(info);
				query.nextRow();
			}

			ExecDML("BEGIN TRANSACTION;", _db);
			for (list<ConferenceId>::iterator itor = confList.begin();
				itor != confList.end(); ++itor) {
				DeleteMeetingByEventId(itor->_eventId, itor->_startTime, itor->_authorizerId);
			}
			ExecDML("COMMIT TRANSACTION;", _db);
		}
		
	}

	return DBError::DB_SUCCESS;
}

int ConferenceDao::SaveAccreditUser(const Accrediting& data) {
	CppSQLite3Statement stmt = _db->compileStatement(
		"INSERT OR REPLACE INTO TBL_CONF_SHARE_USER(TYPE, SHARE_USERID) VALUES(?, ?);");
	int i = 1;
	stmt.bind(i++, data.GetStatus());
	stmt.bind(i++, data.GetUserId());
	int ret = ExecDML(stmt); 
	stmt.reset();
	if (ret != 0) {
		UC_LOG(ERROR) << "SaveAccreditUser return error:" << ret
			<< "; type:" << data.GetStatus()
			<< ", share_userid:" << data.GetUserId();
		return DBError::DB_INSERT_ERROR;
	}
	return DBError::DB_SUCCESS;
}

int ConferenceDao::GetAccreditUserList(vector<Accrediting>& data) {
	CppSQLite3Statement stmt = _db->compileStatement(
		"SELECT * FROM TBL_CONF_SHARE_USER;");
	CppSQLite3Query query;
	int ret = Query(stmt, query);
	if (ret != 0) {
		UC_LOG(ERROR) << "GetAccreditUserList return error:" << ret;
		return DBError::DB_QUERY_ERROR;
	}

	if (query.eof()) {
		UC_LOG(ERROR) << "GetAccreditUserList return db empty!";
		return DBError::DB_QUERY_EMPTY;
	}

	Accrediting info;
	while(!query.eof()) {
		int i = 0;
		info.SetStatus(atoi(query.fieldValue(i++)));
		info.SetUserId(atoi(query.fieldValue(i++)));
		data.push_back(info);
		query.nextRow();
	}
	return DBError::DB_SUCCESS;
}

int ConferenceDao::InvalidateData() {
	string sql = "UPDATE TBL_CONFERENCE SET IS_OLD = 1 WHERE IS_OLD = 0;";
	int ret = ExecDML(sql, _db);
	if (ret != 0) {
		UC_LOG(ERROR) << "InvalidateData return error:" << ret;
		return DBError::DB_UPDATE_ERROR;
	}
	return DBError::DB_SUCCESS;
}

int ConferenceDao::Cleanup() {
	std::string sql = "DELETE FROM TBL_CONFERENCE;DELETE FROM TBL_CONF_MEMBER_RELATION;";
	int ret = ExecDML(sql, _db);
	if (ret != 0) {
		UC_LOG(ERROR) << "ConferenceDao::Cleanup() return error:" << ret;
		return DBError::DB_DELETE_ERROR;
	}
	return DBError::DB_SUCCESS;
}

int ConferenceDao::GetConferenceIdByEventId(int64_t eventId, int64_t startTime, int32_t authorizerId) {

	CppSQLite3Statement stmt;
	if (startTime > 0) {
		string sql = "SELECT CONF_ID FROM TBL_CONFERENCE WHERE EVENT_ID = ? AND START_TIME = ? AND AUTHORIZER_ID = ?;";
		stmt = _db->compileStatement(sql.c_str());
		int i = 1;
		stmt.bind(i++, eventId);
		stmt.bind(i++, startTime);
		stmt.bind(i++, authorizerId);
	} else {
		string sql = "SELECT CONF_ID FROM TBL_CONFERENCE WHERE EVENT_ID = ? AND AUTHORIZER_ID = ?;";
		stmt = _db->compileStatement(sql.c_str());
		int i = 1;
		stmt.bind(i++, eventId);
		stmt.bind(i++, authorizerId);
	}
	
	CppSQLite3Query query;
	int ret = Query(stmt, query);
	if (ret != 0) {
		UC_LOG(ERROR) << "GetConferenceIdByEventId return error:" << ret
			<< "; eventId:" << eventId
			<< ", startTime:" << startTime
			<< ", authorizerId:" << authorizerId;
		return -1;
	}

	if (query.eof()) {
		UC_LOG(ERROR) << "GetConferenceIdByEventId return db empty!" << "; eventId:" << eventId << ", authorizerId:" << authorizerId;
		return -1;
	}

	return atoi(query.fieldValue(0));
}

int64_t ConferenceDao::GetEventIdByGroupId(int64_t groupId){

	string sql = SELECT_CONF + "  WHERE RELATE_GRPID = ?;";
	CppSQLite3Statement stmt = _db->compileStatement(sql.c_str());
	int i = 1;
	stmt.bind(i++, groupId);
	CppSQLite3Query query;
	int ret = Query(stmt, query);
	if (ret != 0) {
		UC_LOG(ERROR) << "GetConferenceIdByEventId return error:" << ret
			<< "; groupId:" << groupId;
		return -1;
	}

	if (query.eof()) {
		UC_LOG(ERROR) << "GetConferenceIdByEventId return db empty!" << "; groupId:" << groupId ;
		return -1;
	}

	return atoi(query.fieldValue(1));
}

int64_t ConferenceDao::GetEventIdByConferenceId(int32_t conferenceId, int32_t authorizerId) {
	string sql = SELECT_CONF + "  WHERE CONF_ID = ? AND AUTHORIZER_ID = ?;";
	CppSQLite3Statement stmt = _db->compileStatement(sql.c_str());
	int i = 1;
	stmt.bind(i++, conferenceId);
	stmt.bind(i++, authorizerId);
	CppSQLite3Query query;
	int ret = Query(stmt, query);
	if (ret != 0) {
		UC_LOG(ERROR) << "GetConferenceIdByEventId return error:" << ret
			<< "; conferenceId:" << conferenceId
			<< ", authorizerId:" << authorizerId;
		return -1;
	}

	if (query.eof()) {
		UC_LOG(ERROR) << "GetEventIdByConferenceId return db empty!" << "; conferenceId:" << conferenceId << ", authorizerId:" << authorizerId;
		return -1;
	}

	Meeting info;
	return atoll(query.fieldValue(1));
}

//////////////////////////////////////////////////////
int ConferenceDao::SaveCallPhoneNum(const string& phoneNum) {
	CppSQLite3Statement stmt = _db->compileStatement(
		"INSERT OR REPLACE INTO TBL_CALL_PHONENUM(PHONE_NUMBER, CALL_TIME) VALUES(?, ?);");
	int i = 1;
	stmt.bind(i++, phoneNum.c_str());
	stmt.bind(i++, Time::GetSysTime());
	int ret = ExecDML(stmt);
	stmt.reset();
	if (ret != 0) {
		UC_LOG(ERROR) << "SavePhoneNum return error:" << ret
			<< "; phoneNum:" << phoneNum;
		return DBError::DB_INSERT_ERROR;
	}
	return DBError::DB_SUCCESS;
}

int ConferenceDao::GetHistoryPhoneNum(list<string>& phoneNums) {
	CppSQLite3Statement stmt = _db->compileStatement(
		"SELECT PHONE_NUMBER FROM TBL_CALL_PHONENUM ORDER BY CALL_TIME DESC LIMIT 0, 10;");
	CppSQLite3Query query;
	int ret = Query(stmt, query);
	if (ret != 0) {
		UC_LOG(ERROR) << "GetConfigValue return error:" << ret;
		return DBError::DB_QUERY_ERROR;
	}

	if (query.eof()) {
		UC_LOG(ERROR) << "GetConfigValue return db empty error!";
		return DBError::DB_QUERY_EMPTY;
	}

	string value;
	while (!query.eof()) {
		int i = 0;
		value = query.fieldValue(i++);
		phoneNums.push_back(value);
		query.nextRow();
	}
	return DBError::DB_SUCCESS;
}

} //namespace uc
