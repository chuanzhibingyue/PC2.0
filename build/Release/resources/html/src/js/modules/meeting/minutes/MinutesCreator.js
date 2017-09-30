(function (uc) {
    if (uc.modules.meeting.MinutesCreator) {
        return;
    }
    var isUEdit, _meeting, _summaryId,userInfo;

    function _init() {
        _summaryId = uc.util.StorageUtil.getData("Meeting-Minutes:summaryId");
        _meeting = uc.util.StorageUtil.getData("Meeting-Minutes:Data");


        uc.modules.component.TitleBarButton.init();
        $(document).keydown(function(e){
            if(e.which == 27){
                uc.util.WindowUtil.close();
            }
        });
        _initEditor();
        _bindEvent();
        _getMinutesDetail();

    }

    function _getMinutesDetail() {
        if (_summaryId) {
            uc.IMeetingService.getMinutesDetails(_meeting.eventId,_summaryId, function (arg) {

            });
        } else {
            _initUEdit();
            isUEdit.addListener('ready', function () {
                isUEdit.execCommand( 'fontsize', '16px' );
                var $iframe = $("#ueditor_0");
                $iframe.contents().find("body").keydown(function(e){
                    if(e.which == 27){
                        uc.util.WindowUtil.close();
                    }
                });

                uc.util.CaretUtil.setIframeEditorCaretPosition($iframe[0].contentWindow.document.body, $iframe[0]);
            });
        }
    }

    function _initEditor(userId) {
        userInfo = uc.IContactService.getCurrentUserInfo();
        if (userId && userId !== userInfo.userId) {
            userInfo = uc.IContactService.getBasicContactInfo(userId);
        }
        $(".minutes-title .minutes-userName").text(userInfo.displayName);
    }

    function _initUEdit() {
        isUEdit = new UE.ui.Editor(5);
        isUEdit.render("discussion");

    }

    function _setUEditContent(summary) {
        _initUEdit();
        isUEdit.addListener('ready', function () {
            isUEdit.setContent(summary);
            $("#ueditor_0").contents().find("body").keydown(function(e){
                if(e.which == 27){
                    uc.util.WindowUtil.close();
                }
            });
            var $iframe = $("#ueditor_0");
            uc.util.CaretUtil.setIframeEditorCaretPosition($iframe[0].contentWindow.document.body, $iframe[0]);
        });
    }

    function _bindEvent() {

        $(".create_minutes .release-minutes").click(function () {
            var summary = isUEdit.getContent();
            if (!_summaryId) {
                var location = _meeting.location || uc_resource.Meeting.DefaultCity;
                if (!summary) {
                    uc.modules.component.PromptMessageBox.open({ title: uc_resource.WindowTitle.Notification, message: uc_resource.Meeting.InputMeetingSummaryContext, buttonMode: "ok", OkText: uc_resource.Common.OK }, function (result) {
                        this.close();
                    });
                    return;
                }
                var attend = [];
                if (!_meeting.attendees.length) {
                    attend.push({
                        account: "" + _meeting.hostId + "",
                        type: 1,
                        status: 2,
                        startTime: 12432143142123,
                        name: _meeting.hostName
                    })
                } else {
                    attend = _meeting.attendees;
                }
                uc.IMeetingService.createMinutes(_meeting.eventId,
                    _meeting.startTime,
                    userInfo.displayName+uc_resource.Meeting.OnesMeetingSummary, summary, function (arg) {
                        console.log(arg);
                        if (arg[0] == 0) {
                            uc.util.WindowUtil.close();
                        } else {
                            uc.modules.component.PromptMessageBox.open({ title: uc_resource.WindowTitle.Notification, message: uc_resource.Meeting.SaveMeetingSummaryFailed, buttonMode: "ok", OkText: uc_resource.Common.OK }, function (result) {
                                this.close();
                            });
                        }
                    })
            } else {
                uc.IMeetingService.updateMinutes(_meeting.eventId, _meeting.minutesId, userInfo.displayName+uc_resource.Meeting.OnesMeetingSummary, summary, function (arg) {
                    console.log(arg);
                    if (arg[0] == 0) {
                        uc.util.WindowUtil.close();
                    } else {
                        uc.modules.component.PromptMessageBox.open({ title: uc_resource.WindowTitle.Notification, message: uc_resource.Meeting.SaveMeetingSummaryFailed, buttonMode: "ok", OkText: uc_resource.Common.OK }, function (result) {
                            this.close();
                        });
                    }
                })
            }

        });

        $(".create_minutes .cancel-minutes").click(function () {
            uc.util.WindowUtil.close();
        });

    }

    uc.interfaces.IMeetingEvent.OnMinutesDetailsReceived = function (errorCode,eventId,minutesId,creatorId,createTime,content,startTime) {
        if (errorCode == 0) {
            uc.modules.meeting.MinutesCreator.showSummary(errorCode,eventId,minutesId,creatorId,createTime,content,startTime);
        }
    };

    uc.modules.meeting.MinutesCreator = {

        init: function () {
            _init();
        },

        showSummary: function (errorCode,eventId,minutesId,creatorId,createTime,content,startTime) {
            if (_summaryId == minutesId) {
                _initEditor(creatorId);
                _setUEditContent(content);
                _meeting.eventId = eventId;
                _meeting.minutesId = minutesId;
                _meeting.creatorId = creatorId;
                _meeting.createTime = createTime;
                _meeting.startTime = startTime;
                _meeting.minutes = content
                //_meeting.location = location;
            }
        }
    }
})(uc);

jQuery(document).ready(function () {
    uc.init();
    uc.modules.meeting.MinutesCreator.init();
});