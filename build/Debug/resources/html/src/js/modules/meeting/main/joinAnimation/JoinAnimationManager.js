/**
 * Created by zhen.xu on 16/5/26.
 */
(function (uc) {
    uc.modules.meeting.JoinAnimationManager = {
        OnInitClient: function (clearData) {
            uc.modules.meeting.JoinAnimation.init();
        },

        OnShowJoinAnimation: function () {
            uc.util.LogUtil.info('JoinAnimationManager', 'OnShowJoinAnimation', 'show join animation');
            uc.modules.meeting.JoinAnimation.showJoinAnimation();
        },

        OnCancelJoinMeeting: function () {
            uc.util.LogUtil.info('JoinAnimationManager', 'OnCancelJoinMeeting', 'cancel join meeting');
        },

        OnMeetingJoinFailed: function () {
            uc.modules.meeting.JoinAnimation.showJoinFailed();
        },

        OnMeetingJoined: function () {
            uc.modules.meeting.JoinAnimation.joined();
        },

        OnMeetingClosed: function () {
            uc.modules.meeting.JoinAnimation.closed();
        },

        OnMeetingReadyStatusReceived: function () {
            uc.modules.meeting.JoinAnimation.joinReady();
        }
    }
})(uc);