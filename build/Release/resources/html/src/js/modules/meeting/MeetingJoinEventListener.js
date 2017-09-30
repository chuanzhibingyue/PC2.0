
/**
 * Created by senlin.ouyang on 2015/7/23.
 */
(function (uc) {
    var timer;
    uc.modules.meeting.MeetingJoinEventListener = function () {

    };
    jQuery.extend(uc.modules.meeting.MeetingJoinEventListener.prototype,
        uc.interfaces.IUIEvent,
        uc.interfaces.IMeetingEvent,
        {
            OnMeetingJoined: function () {
                clearInterval(timer);
                uc.util.WindowUtil.close();
            },

            OnMeetingJoinFailed: function () {
                clearInterval(timer);
                uc.util.WindowUtil.close();
            }
        });

    jQuery(document).ready(function(){
        uc.init();
        var closeTime = 30;
        timer =  setInterval(function () {
            if (closeTime == 0) {
                clearInterval(timer);
                uc.util.WindowUtil.close();
                return;
            }
            closeTime--;
            console.log(closeTime);
        }, 1000);
    });
})(uc);
