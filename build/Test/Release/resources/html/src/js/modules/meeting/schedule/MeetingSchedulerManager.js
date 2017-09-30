/**
 * Created by waouyang on 16/4/26.
 */
(function(uc){
    var _this = null;
    uc.modules.meeting.MeetingSchedulerManager = {
        OnMeetingCreated: function (/*int*/errorCode, conferenceInfo) {
            uc.util.LogUtil.info("MeetingSchedulerManager", "OnMeetingCreated", "data=", {
                errorCode: errorCode,
                meetings: conferenceInfo
            });
            uc.modules.meeting.MeetingScheduler.OnMeetingCreated(errorCode, conferenceInfo);
        },

        OnMeetingUpdated: function (/*int*/errorCode, eventId, startTime, conferenceInfo) {
            uc.util.LogUtil.info("MeetingSchedulerManager", "OnMeetingUpdated", "data=", {
                errorCode: errorCode,
                meetings: conferenceInfo
            });
            uc.modules.meeting.MeetingScheduler.OnMeetingUpdated(errorCode, eventId, startTime, conferenceInfo);
        },

        OnServerSearchResultReceived: function (errorCode, searchId, meetings) {
            uc.util.LogUtil.info("MeetingSchedulerManager", "OnServerSearchResultReceived", "data=", {
                errorCode: errorCode,
                searchId: searchId,
                meetings: meetings
            });
            uc.modules.meeting.MeetingScheduler.OnServerSearchResultReceived(errorCode, searchId, meetings);
        },

        OnUserBusyFreeStatusInfoReceived: function (schedules) {
            uc.util.LogUtil.info("MeetingSchedulerManager", "OnUserBusyFreeStatusInfoReceived", "data=", {
                schedules: schedules
            });
            uc.modules.meeting.MeetingScheduler.OnUserBusyFreeStatusInfoReceived(schedules);
        },

        OnBasicContactInfoReceived: function(errorCode, userIds, contacts){
            uc.util.LogUtil.info("MeetingSchedulerManager", "OnBasicContactInfoReceived", "data=", {
                errorCode: errorCode,
                contacts: contacts
            });
            if (errorCode == uc.ErrorCode.Success) {
                uc.modules.meeting.MeetingScheduler.OnBasicContactInfoReceived(contacts);
            }
        },
	    OnMeetingCreatedInMeetingRoom: function (errorCode, roomId, meeting) {
		    uc.modules.meeting.MeetingScheduler.OnMeetingCreatedInMeetingRoom(errorCode, roomId, meeting);
	    }
    };
})(uc);