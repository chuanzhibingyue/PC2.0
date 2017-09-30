#ifndef CALENDAR_SERVICE_MOCK_CLIENT_H_
#define CALENDAR_SERVICE_MOCK_CLIENT_H_

#include "ICalendarService.h"
#include "UCMockBase.h"
using namespace uc;

class CalendarServiceMockClient : public ICalendarServiceEvent, public UCMockBase
{
public:
	void Init(IUCEngine *ucEngine);
	void UnInit();

	//interface
	void CreateConference(
		const std::vector<Attendee>& attendees,
		const std::string& title,
		const std::string& location,
		const std::string& summary,
		const std::string& timeZone,
		int64_t startTime,
		int32_t duration,
		int32_t isGnet,
		int32_t authorizerId);

	//event
	virtual void OnConferenceCreated( int32_t errorCode, ConferenceInfo& info);

	virtual void OnConferenceUpdated(int32_t errorCode, int64_t eventId, int64_t startTime, ConferenceInfo& info);

	virtual void OnConferenceCanceled(int32_t errorCode, int32_t conferenceId, int64_t eventId,  int64_t startTime, int32_t authorizerId = 0);

	virtual void OnConferenceStartUrlReceived(int32_t errorCode, int64_t eventId, const std::string& startUrl);

	virtual void OnConferenceWebStartUrlReceived(int32_t errorCode, int64_t eventId, const std::string& startUrl);

	virtual void OnAccreditSettingUpdated(int32_t errorCode=-1, int32_t authorizedPersonId = 0, int32_t status = 1);

	virtual void OnAccreditUserListReceived(int32_t errorCode, std::vector<AccreditInfo>& accreditUserList);

	virtual void OnConferenceListReceived(int32_t errorCode, int32_t requestId, int64_t startTime, int64_t endTime, int32_t page, std::vector<ConferenceInfo>& infos);

	virtual void OnConferenceInfoReceivedById(int32_t errorCode,  int32_t conferenceId, std::vector<ConferenceInfo>& infos);

	virtual void OnConferenceInfoReceivedByIds(int32_t errorCode, int32_t requesetId, std::vector<ConferenceInfo>& infos);

	virtual void OnConferenceInvitationAccepted(int32_t errorCode, ConferenceInfo& info);

	virtual void OnConferenceInvitationDenied(int32_t errorCode, ConferenceInfo& info);

	virtual void OnConferenceForwarded(int32_t errorCode, ConferenceInfo& info);

	virtual void OnUserScheduleListReceived(int32_t errorCode, int32_t requestId, std::vector<UserSchedule>& schedules);

	virtual void OnConferenceSummaryCreated(
											int32_t errorCode,
											int32_t conferenceId,
											int32_t summaryId,
											int32_t creatorId,
											const std::string& conferenceName,
											const std::string& summaryName,
											int64_t startTime,
											int64_t createTime
											);

	virtual void OnConferenceSummaryUpdated(
											int32_t errorCode,
											int32_t conferenceId,
											int32_t summaryId,
											int32_t operatorId,
											const std::string& conferenceName,
											const std::string& summaryName,
											int64_t startTime,
											int64_t updateTime
											);

	virtual void OnConferenceSummaryInfoReceived(
												int32_t errorCode = -1,
												int32_t conferenceId = 0,
												int32_t summaryId = 0,
												int32_t creatorId = 0,
												int64_t groupId = 0,
												int32_t createTime = 0,
												const std::string& summary = "",
												int32_t startTime = 0,
												const std::string& location = ""
												);

	virtual void OnSummaryListReceivedByConferenceId(
													int32_t errorCode,
													int32_t conferenceId,
													std::vector<int32_t> summaryIds
													);
private:
	ICalendarService* _calendarService;
};

#endif