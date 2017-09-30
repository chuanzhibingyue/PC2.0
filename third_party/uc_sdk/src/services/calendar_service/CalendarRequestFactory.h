/*
*  CalendarRequestFactory.h
*
*  Created on: June 4, 2015
*      Author: changan wu
*/

#ifndef CALENDARREQUESTFACTORY_H_
#define CALENDARREQUESTFACTORY_H_

#include "HTTPRequest.h"
#include "CalendarData.h"
#include "DocumentInfo.h"
#include <vector>

namespace uc {
class CalendarService;

//日程相关request生成器
class CalendarRequestFactory {
public:
	CalendarRequestFactory(CalendarService* service) {
		_calendarService = service;
	}

	/**
	* 构建创建会议的request
	* @param
	* @param [out] request			构建的request对象
	* @return
	*/
	void MakeRequestOfCreateMeeting(
		Meeting *meeting,
		HTTPRequest &request
		);

	void MakeRequestOfCreateRemoteAssistance(
		int32_t userId,
		HTTPRequest &request
		);

	/**
	* 构建更新会议的request
	* @param
	* @param [out] request			构建的request对象
	* @return
	*/
	void MakeRequestOfUpdateMeeting(
		int64_t oriTime,
		Meeting *meeting,
		HTTPRequest &request
		);

	/**
	* 构建取消会议的request
	* @param
	* @param [out] request			构建的request对象
	* @return
	*/
	void MakeRequestOfCancelMeeting(int64_t eventId, int64_t startTime, int32_t authorizerId, int32_t language, int32_t emailLanguage,  HTTPRequest &request);

	/**
	* 构建获取会议吊起串的request
	* @param
	* @param [out] request			构建的request对象
	* @return
	*/
	void MakeRequestOfGetMeetingStartURL(const std::string& conferenceId, const std::string& password, HTTPRequest &request);

	/**
	* 构建获取会议Web入会吊起串的request
	* @param
	* @param [out] request			构建的request对象
	* @return
	*/
	void MakeRequestOfGetMeetingWebStartURL(const std::string& confId, const std::string& password, const std::string& mail, const std::string& displayName, HTTPRequest &request);

	/**
	* 构建获取会议Web入会吊起串的request
	* @param authorizedPersonId		被授权人ID
	* @param 状态					授权1，取消0
	* @param [out] request			构建的request对象
	* @return
	*/
	void MakeRequestOfUpdateAccreditSetting(int32_t authorizedPersonId, int32_t status, HTTPRequest& request);

	/**
	* 构建获取共享和被共享用户列表的request
	* @param [out] request			构建的request对象
	* @return
	*/
	void MakeRequestOfGetAccreditUserList(HTTPRequest& request);

	/**
	* 构建获取会议列表的request
	* @param authorizedPersonId		授权人ID
	* @param [out] request			构建的request对象
	* @return
	*/
	void MakeRequestOfGetMeetingList(int32_t authorizerId, int64_t startTime, int64_t endTime, int32_t limit, int64_t eventIdFilter, bool bFilterAsc, HTTPRequest& request);
	void MakeRequestOfAccptMeetingInvitation(int64_t eventId, int32_t authorizerId, int64_t oriStart, HTTPRequest& request);
	void MakeRequestOfDenyMeetingInvitation(int64_t eventId, int32_t authorizerId,int64_t oriStart, HTTPRequest& request);
	void MakeRequestOfGetConferenceInfoById(int32_t conferenceId, int64_t startTime, int64_t endTime, int32_t authorizerId, HTTPRequest& request);
	void MakeRequestOfGetMeetingInfoByIds(const std::vector<int64_t>& eventIds, int64_t startTime, int64_t endTime, int32_t authorizerId, HTTPRequest& request);
	void MakeRequestOfForwardMeeting(int64_t eventId, const std::vector<Attendee>& attendees, int32_t authorizerId, int64_t oriStart, int32_t language, int32_t emailLanguage, HTTPRequest& request);
	void MakeRequestOfGetUserSchedule(const std::vector<int32_t>& userIdList, int64_t startTime, int64_t endTime, int32_t timeInterval, int64_t eventIdFilter, HTTPRequest& request);
	void MakeRequestOfCreateMinutes(int64_t eventId, int64_t startTime, const std::string& title, const std::string& minutes, HTTPRequest& request);
	void MakeRequestOfUpdateMinutes(int64_t minutesId, const std::string& title, const std::string& minutes, HTTPRequest& request);
	void MakeRequestOfGetMinutesDetails(int64_t minutesId, HTTPRequest& request);
	void MakeRequestOfGetMinutesList(int64_t eventId, int64_t startTime, HTTPRequest& request);
	void MakeRequestOfDeleteMeetingRecording(const std::string& confId, int64_t eventId, int64_t startTime, HTTPRequest& request);
	void MakeRequestOfGetBriefMeetingList(int64_t startTime, int64_t endTime, int32_t authorizerId, HTTPRequest& request);
	void MakeRequestOfGetBriefChangedMeetingList(int64_t startTime, int32_t authorizerId, HTTPRequest& request);
	void MakeRequestOfUploadMeetingFile(int64_t eventId, int32_t operation, const DocumentInfo& info, HTTPRequest& request);
	void MakeRequestOfDeleteMeetingFile(int64_t minutesId, HTTPRequest& request);
	void MakeRequestOfCreateMeetingRoom(const MeetingRoom& room,HTTPRequest &request);
	void MakeRequestOfUpdateMeetingRoom(const MeetingRoom& room, HTTPRequest &request);
	void MakeRequestOfCreateMeetingInMeetingRoom(const Meeting& meeting, const std::string& roomId, HTTPRequest& request);
	void MakeRequestOfGetMeetingRoomList(int64_t orgId, HTTPRequest& request);
	void MakeRequestOfCheckJoinMeetingRoom(const std::string &roomId, HTTPRequest& request);
	void MakeRequestOfCheckMeetingRoomPassword(const std::string &roomId, const std::string& password, HTTPRequest& request);
	void MakeRequestOfDeleteMeetingRoom(const std::string &roomId, HTTPRequest& request);
	void MakeRequestOfCheckRights(HTTPRequest& request);
	void MakeRequestOfGetUserBusyFreeInfo(const std::vector<int32_t>& userIdList, int64_t startTime, int64_t endTime, int32_t shareUserId, HTTPRequest& request);
private:
	CalendarService * _calendarService;				//日程服务对象
};
} /*namespace uc*/

#endif /*CALENDARREQUESTFACTORY_H_*/
