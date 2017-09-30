#include "CalendarServiceMockClient.h"

void CalendarServiceMockClient::Init( IUCEngine *ucEngine ) {
	_calendarService = (ICalendarService*)ucEngine->QueryService(UCCalendarService);
	_calendarService->registerListener(this);
}

void CalendarServiceMockClient::UnInit() {
	_calendarService->removeListener(this);
}

void CalendarServiceMockClient::CreateConference(
	const std::vector<Attendee>& attendees,
	const std::string& title,
	const std::string& location,
	const std::string& summary,
	const std::string& timeZone,
	int64_t startTime,
	int32_t duration,
	int32_t isGnet,
	int32_t authorizerId) {

		_calendarService->CreateConference(attendees,title,location,summary,timeZone,startTime,duration,isGnet,authorizerId);
}

void CalendarServiceMockClient::OnConferenceCreated( int32_t errorCode, ConferenceInfo& info ) {
	NotifyResult(errorCode);
	cout << "OnChatHistoryReceived result: " << errorCode << endl;
}

void CalendarServiceMockClient::OnConferenceUpdated( int32_t errorCode, int64_t eventId, int64_t startTime, ConferenceInfo& info )
{

}

void CalendarServiceMockClient::OnConferenceCanceled( int32_t errorCode, int32_t conferenceId, int64_t eventId,  int64_t startTime, int32_t authorizerId )
{

}

void CalendarServiceMockClient::OnConferenceStartUrlReceived( int32_t errorCode, int64_t eventId, const std::string& startUrl )
{

}

void CalendarServiceMockClient::OnConferenceWebStartUrlReceived( int32_t errorCode, int64_t eventId, const std::string& startUrl )
{

}

void CalendarServiceMockClient::OnAccreditSettingUpdated( int32_t errorCode/*=-1*/, int32_t authorizedPersonId /*= 0*/, int32_t status /*= 1*/ )
{

}

void CalendarServiceMockClient::OnAccreditUserListReceived( int32_t errorCode, std::vector<AccreditInfo>& accreditUserList )
{

}

void CalendarServiceMockClient::OnConferenceListReceived( int32_t errorCode, int32_t requestId, int64_t startTime, int64_t endTime, int32_t page, std::vector<ConferenceInfo>& infos )
{

}

void CalendarServiceMockClient::OnConferenceInfoReceivedById( int32_t errorCode, int32_t conferenceId, std::vector<ConferenceInfo>& infos )
{

}

void CalendarServiceMockClient::OnConferenceInfoReceivedByIds( int32_t errorCode, int32_t requesetId, std::vector<ConferenceInfo>& infos )
{

}

void CalendarServiceMockClient::OnConferenceInvitationAccepted( int32_t errorCode, ConferenceInfo& info )
{

}

void CalendarServiceMockClient::OnConferenceInvitationDenied( int32_t errorCode, ConferenceInfo& info )
{

}

void CalendarServiceMockClient::OnConferenceForwarded( int32_t errorCode, ConferenceInfo& info )
{

}

void CalendarServiceMockClient::OnUserScheduleListReceived( int32_t errorCode, int32_t requestId, std::vector<UserSchedule>& schedules )
{

}

void CalendarServiceMockClient::OnConferenceSummaryCreated( int32_t errorCode, int32_t conferenceId, int32_t summaryId, int32_t creatorId, const std::string& conferenceName, const std::string& summaryName, int64_t startTime, int64_t createTime )
{

}

void CalendarServiceMockClient::OnConferenceSummaryUpdated( int32_t errorCode, int32_t conferenceId, int32_t summaryId, int32_t operatorId, const std::string& conferenceName, const std::string& summaryName, int64_t startTime, int64_t updateTime )
{

}

void CalendarServiceMockClient::OnConferenceSummaryInfoReceived( int32_t errorCode /*= -1*/, int32_t conferenceId, int32_t summaryId, int32_t creatorId, int64_t groupId, int32_t createTime, const std::string& summary /*= ""*/, int32_t startTime, const std::string& location /*= "" */ )
{

}

void CalendarServiceMockClient::OnSummaryListReceivedByConferenceId( int32_t errorCode, int32_t conferenceId, std::vector<int32_t> summaryIds )
{

}

