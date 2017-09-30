/**
 * Created by waouyang on 16/4/27.
 */
(function (uc) {
    var _this = null;

    uc.modules.meeting.MeetingReminder = {
        canceledMeetingIdArray: [],
        meetingInfo: null,

        init: function () {
            _this = this;
            uc.modules.component.TitleBarButton.init(null, {
                events: {
                    close: this.closeMeetingReminder
                }
            });
            this._initUI();
            this.bindEvent();

            uc.util.WindowUtil.setOnTop();
        },

        _initUI: function () {
            var values = window.location.href.split("?");
            if (values.length == 2) {
                var eventId = values[1];
                this.key = this._getMeetingKey(eventId);
                this.meetingInfo = uc.util.StorageUtil.getData(this.key);

                if (this.meetingInfo) {
                    $("span.meeting-name").text(this.meetingInfo.title);
                    if (this.meetingInfo.isGNet) {
                        $("#joinMeeting").removeClass("hide");
                    }
                }
            }
        },

        bindEvent: function () {
            $(document.body).on("keydown", function (event) {
                if (event.keyCode == 27) {
                    uc.util.WindowUtil.close();
                }
            });

            $("#confirm").on("click", function () {
                _this.closeMeetingReminder();
            });

            $("#joinMeeting").on("click", function () {
                _this._joinMeeting();
            });
        },

        _joinMeeting: function () {
            if (this.canceledMeetingIdArray.indexOf(this.meetingInfo.eventId) == -1) {
                if (this.meetingInfo.isGNet == 1) {
                    _this.closeMeetingReminder(true);
                }
            } else {
                uc.modules.component.PromptMessageBox.open({
                    title: uc_resource.WindowTitle.Notification,
                    message: uc_resource.Meeting.MeetingReminderCanceled,
                    buttonMode: "ok",
                    OkText: uc_resource.Common.OK
                }, function (result) {
                    uc.util.WindowUtil.close();
                });
            }
        },

        closeMeetingReminder: function (isAttend) {
            uc.util.LogUtil.info("MeetingReminder", "closeMeetingReminder", "triggerCrossWindowEvent", {
                key: this.key,
                eventId: this.meetingInfo.eventId,
                isGNet: this.meetingInfo.isGNet
            });
            uc.IUIService.triggerCrossWindowEvent(this.key, {eventId: this.meetingInfo.eventId, isAttend: !!isAttend});
            uc.util.WindowUtil.close();
        },

        OnMeetingCanceled: function (channel, eventId) {
            if (channel !== uc.constants.Message.MessageChannel.UCAS) {
                return;
            }
            this.canceledMeetingIdArray.push(eventId);
        },

        _getMeetingKey: function (eventId) {
            return "Calendar:MeetingReminder_" + eventId;
        }
    };

    jQuery(document).ready(function () {
        uc.init();
        uc.modules.meeting.MeetingReminder.init();
    });
})(uc);