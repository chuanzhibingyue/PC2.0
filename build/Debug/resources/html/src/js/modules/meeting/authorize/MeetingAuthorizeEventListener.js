/**
 * Created by waouyang on 16/4/27.
 */
(function(uc){
    uc.modules.meeting.MeetingAuthorizeEventListener = function(){

    };

    jQuery.extend(uc.modules.meeting.MeetingAuthorizeEventListener.prototype,
        uc.interfaces.IMeetingEvent, {
            OnMeetingAccreditMessageReceived: function (channel, from, to, msgId, msgSeq, msgTime) {
                uc.modules.meeting.MeetingAuthorize.OnMeetingAccreditMessageReceived(channel, from, to, msgId, msgSeq, msgTime);
            },
            OnMeetingAccreditCancelledMessageReceived: function (channel, from, to, msgId, msgSeq, msgTime) {
                uc.modules.meeting.MeetingAuthorize.OnMeetingAccreditCancelledMessageReceived(channel, from, to, msgId, msgSeq, msgTime);
            }
        })
})(uc);