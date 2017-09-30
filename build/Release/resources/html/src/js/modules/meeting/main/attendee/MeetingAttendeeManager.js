/**
 * Created by waouyang on 16/4/27.
 */
(function(uc){
    uc.modules.meeting.MeetingAttendeeManager = {
        init: function(parentNode){
            uc.modules.meeting.MeetingAttendee.init(parentNode);
            this.attendeeListInitialized = true;
        },

        showMeetingAttendee: function(parentNode, meetingInfo){
            if(!this.attendeeListInitialized){
                this.init(parentNode);
            }

            var meeting = uc.IMeetingService.getMeetingInfoByIdSync(meetingInfo.eventId, meetingInfo.meetingStartTime, 0, uc.modules.meeting.MeetingListManager.authorizerId);

            uc.modules.meeting.MeetingAttendee.showAttendees(meeting[0]);
            this.attendeeListShown = true;
        },

        updateMeeting: function (meetingInfo) {
            if (this.attendeeListShown && meetingInfo) {
                uc.modules.meeting.MeetingAttendee.updateMeetingInfo(meetingInfo);
            }
        },

        OnBasicContactInfoReceived: function (errorCode, userIds, contacts) {
            if(errorCode == uc.ErrorCode.Success && this.attendeeListShown){
                uc.modules.meeting.MeetingAttendee.OnBasicContactInfoReceived(contacts);
            }
        },
        OnUserAvatarDownloaded: function (userId, serverAvatarUrl, localAvatarUrl) {
            if(this.attendeeListShown){
                uc.modules.meeting.MeetingAttendee.OnUserAvatarDownloaded(userId, serverAvatarUrl, localAvatarUrl);
            }
        },

        hideMeetingAttendee: function(){
            this.attendeeListShown = false;
        }
    }
})(uc);