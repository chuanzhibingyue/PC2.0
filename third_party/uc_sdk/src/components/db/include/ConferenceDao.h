///////////////////////////////////////////////////////////
//  ConferenceDao.h
//  Implementation of the Class ConferenceDao
//  Created on:      22-五月-2015 10:36:00
//  Original author: Administrator
///////////////////////////////////////////////////////////

#if !defined(CONFERENCEDAO_H_)
#define CONFERENCEDAO_H_

#include "AppDao.h"
#include "CalendarData.h"

namespace uc {

const string SELECT_CONF = "SELECT EVENT_ID, PARENT_ID, START_TIME, ORI_TIME, AUTHORIZER_ID, "
		"DURATION, CONF_ID, CONF_TITLE, CONF_STATUS, CONF_SUMMARY, CONF_LOCATION, HOST_ID, HOST_NAME, "
		"HOST_PWD, ATTENDEE_PWD, BILLING_CODE, RELATE_GRPID, TIME_ZONE, IS_GNET, IS_WHOLEDAY, IS_CYCLE, "
		"REPEAT_TYPE, INTERVAL, MONTHNO, WEEKNO, DAYNO, WEEKDAYS, REPEAT_COUNT, REPEAT_ENDDATE, "
		"LANGUAGE, EMAIL_LANGUAGE, LAST_UPDATE_TIME, IS_OLD, CONF_EXTRASINFO, ROOM_ID FROM TBL_CONFERENCE";

class ConferenceDao : public AppDao {
public:
	ConferenceDao();
	virtual ~ConferenceDao();

	int SaveMeeting(const Meeting& data);
	int SaveMeetings(const vector<Meeting>& data);
	int GetMeetingByEventId(int64_t eventId, int64_t startTime, int32_t authorizerId, Meeting& data);
	int GetMeetingByConfId(const std::string& confId, Meeting& data);
	int UpdateMeetingStatus(uc::MeetingStatus status, int64_t eventId, int64_t startTime, int32_t authorizerId);
	int UpdateAttendeeStatus(uc::AttendeeStatus status, const std::string& account, int64_t eventId, int64_t startTime, int32_t authorizerId);
	int UpdateAttendees(int64_t eventId, int64_t startTime, int32_t authorizerId, const std::vector<Attendee>& attendees);
	
	int GetConferenceByParentId(int64_t parentId, int64_t oriTime, int32_t authorizerId, Meeting& data);
	int GetConferenceIdByEventId(int64_t eventId, int64_t startTime, int32_t authorizerId);
	int64_t GetEventIdByGroupId(int64_t groupId);
	int64_t GetEventIdByConferenceId(int32_t conferenceId, int32_t authorizerId);
	int GetConferenceByEventId(int64_t eventId, int64_t startTime, int64_t endTime, int32_t authorizerId, vector<Meeting>& data);
	int GetMeetingByIds(const vector<int64_t>& eventIds, int64_t startTime, int64_t endTime, int32_t authorizerId, vector<Meeting>& data);
	
	int GetMeetingList(int32_t authorizerId, int64_t startTime, int64_t endTime, int32_t limit, int64_t eventIdFilter, bool bFilterAsc, vector<Meeting>& data);
	int GetOldMeetingList(int32_t authorizerId, int64_t endTime, int32_t limit, int64_t eventIdFilter, vector<Meeting>& data);
	int GetNewMeetingList(int32_t authorizerId, int64_t startTime, int32_t limit, int64_t eventIdFilter, vector<Meeting>& data);
	
	int DeleteMeetings(const vector<Meeting>& data);
	int DeleteMeetingByEventId(int64_t eventId, int64_t startTime, int32_t authorizerId);
	int DeleteMeetingsWithCommonEventId(int64_t eventId, int64_t startTime, int64_t endTime, int32_t authorizerId = 0);
	int DeleteMeetingsWithTimeRange(int64_t startTime, int64_t endTime, int32_t limit = 200, int64_t eventIdFilter = 0, bool bfilterAsc = true, int32_t authorizerId = 0);
	
	int SaveAccreditUser(const Accrediting& data);
	int GetAccreditUserList(vector<Accrediting>& data);
	int SaveAttendees(int64_t eventId, int64_t startTime, int authorizerId, const std::vector<Attendee>& attendees);

	int InvalidateData();
	int Cleanup();

	//呼历史电话号码
	int SaveCallPhoneNum(const string& phones);
	int GetHistoryPhoneNum(list<string>& phoneNums);

private:
	int SaveMeetingInfo(const Meeting& data);
	void SetConference(CppSQLite3Query& query, Meeting& data);
	int DeleteAttendees(int64_t eventId, int64_t startTime = 0, int64_t authorizerId = 0);

	int AddAttendee(int64_t eventId, int64_t startTime, int authorizerId, const Attendee& attendee);
	int DeleteAttendee(int64_t eventId, int64_t startTime, int32_t authorizerId, const std::string& attendeeAccount);
	void SetAttendee(Meeting& data);


private:
	CppSQLite3DB* _db;

};

struct ConferenceId {
	int64_t _eventId;
	int64_t _startTime;
	int32_t _authorizerId;
	ConferenceId(): _eventId(0), _startTime(0), _authorizerId(0) {

	}
};

}
#endif // !defined(CONFERENCEDAO_H_)
