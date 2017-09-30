/**
 * Created by waouyang on 16/5/6.
 */
(function(uc){
    uc.modules.meeting.MeetingAttendeeBusyStatusEventListener = function(){

    };

    jQuery.extend(uc.modules.meeting.MeetingAttendeeBusyStatusEventListener.prototype,
        uc.interfaces.IMeetingEvent,
        uc.interfaces.IContactEvent, {
            OnBasicContactInfoReceived: function (errorCode, userIds, contacts) {
                if (errorCode == uc.ErrorCode.Success) {
                    uc.modules.meeting.MeetingAttendeeBusyStatus.OnBasicContactInfoReceived(contacts);
                }
            },
            
            OnUserBusyFreeStatusInfoReceived: function (errorCode, requestId, schedules) {
                uc.util.LogUtil.info("MeetingSchedulerEventListener", "OnBasicContactInfoReceived", "data=", {
                    errorCode: errorCode,
                    requestId: requestId,
                    schedules: schedules
                });
                if (errorCode == uc.ErrorCode.Success) {
                    uc.modules.meeting.MeetingAttendeeBusyStatus.OnUserBusyFreeStatusInfoReceived(schedules);
                }
            }
    })
})(uc);