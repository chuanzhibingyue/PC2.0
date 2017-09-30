/**
 * Created by waouyang on 16/4/27.
 */
(function (uc) {
    uc.modules.meeting.MeetingSchedulerEventListener = function () {

    };

    jQuery.extend(uc.modules.meeting.MeetingSchedulerEventListener.prototype,
        uc.interfaces.IMeetingEvent,
        uc.interfaces.IContactEvent,
        uc.interfaces.ISearchEvent,
	    uc.interfaces.IMeetingRoomEvent,{
            OnMeetingCreated: function (/*int*/errorCode, conferenceInfo) {
                uc.util.LogUtil.info("MeetingSchedulerEventListener", "OnMeetingCreated", "data=", {
                    errorCode: errorCode,
                    meetings: conferenceInfo
                });
                uc.modules.meeting.MeetingSchedulerManager.OnMeetingCreated(errorCode, conferenceInfo);
            },

            OnMeetingUpdated: function (/*int*/errorCode, eventId, startTime, conferenceInfo) {
                uc.util.LogUtil.info("MeetingSchedulerEventListener", "OnMeetingUpdated", "data=", {
                    errorCode: errorCode,
                    eventId: eventId,
                    startTime: startTime,
                    meetings: conferenceInfo
                });

                uc.modules.meeting.MeetingSchedulerManager.OnMeetingUpdated(errorCode, eventId, startTime, conferenceInfo);
            },

            OnServerSearchResultReceived: function (errorCode, searchId, contacts, projects, groupChats, cloudGroups, meetings, messages) {
                uc.util.LogUtil.info("MeetingSchedulerEventListener", "OnServerSearchResultReceived", "data=", {
                    errorCode: errorCode,
                    searchId: searchId,
                    contacts: contacts,
                    meetings: meetings
                });
                uc.modules.meeting.MeetingSchedulerManager.OnServerSearchResultReceived(errorCode, searchId, meetings);
            },

            OnBasicContactInfoReceived: function(errorCode, userIds, contacts){
                uc.util.LogUtil.info("MeetingSchedulerEventListener", "OnBasicContactInfoReceived", "contact =", contacts);
                uc.modules.meeting.MeetingSchedulerManager.OnBasicContactInfoReceived(errorCode, userIds, contacts);
            },
		    //会议室创建会议事件
		    OnMeetingCreatedInMeetingRoom: function (errorCode, roomId, meeting) {
			    uc.modules.meeting.MeetingSchedulerManager.OnMeetingCreatedInMeetingRoom(errorCode, roomId, meeting);
		    },
            
            OnUserBusyFreeStatusInfoReceived: function (errorCode, requestId, schedules) {
                uc.util.LogUtil.info("MeetingSchedulerEventListener", "OnBasicContactInfoReceived", "data=", {
                    errorCode: errorCode,
                    requestId: requestId,
                    schedules: schedules
                });
                if (errorCode == uc.ErrorCode.Success) {
                    uc.modules.meeting.MeetingSchedulerManager.OnUserBusyFreeStatusInfoReceived(schedules);
                }
            }
        })
})(uc);