/**
 * Created by waouyang on 16/4/27.
 */
(function (uc) {
    uc.modules.meeting.MeetingReminderEventListener = function () {

    };

    jQuery.extend(uc.modules.meeting.MeetingReminderEventListener.prototype,
        uc.interfaces.IMeetingEvent, {

            OnMeetingCancelledMessageReceived: function (channel, from, to, meetingId, eventId, groupId, meetingName, accreditUserId, hostId, isRecurrent, meetingStartTime, msgId, msgSeq, msgTime) {
                uc.modules.meeting.MeetingReminder.OnMeetingCanceled(channel, eventId);
            }
        })
})(uc);