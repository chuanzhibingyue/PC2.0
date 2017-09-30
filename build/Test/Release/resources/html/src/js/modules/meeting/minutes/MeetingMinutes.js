
(function (uc) {
    var _meeting;
    uc.modules.meeting.MeetingMinutes = function (moduleId) {
        var rootPath = (function(){
            var href = location.href;
            var index = href.indexOf('/html');
            return href.substring(0, index + 6);
        })();
        var MINUTES_TEMPLATE = "<div class='meeting-minutes'>"
            +"<div class='noMinutes-tip'>"
            +"<img src='" + rootPath + "/images/meeting/noMeetingMinutes.png'>"
            +" <div class='init-panel-noneText'>" + uc_resource.Main.MeetingMinutesMessage + "</div>"
            +"<div class='no-minutes'></div>"
            +"<div class='create-minutes'>" + uc_resource.Meeting.EditMeetingMinutes + "</div>"
            +"</div>"
            +"<div class='minutes-content'>"
            +"     <div class='border-minutes'></div>"
            + "    <div class='add-minutes add-icon'>"+uc_resource.Meeting.AddMeetingSummary+""
            + "    </div>"
            + "     <div class='minutes-list'></div>"
            + "    </div>"
            +"</div>";
        var $minutesPanel, $minutesRequest, _userInfo,timeZone= 8,_meeting;

        function _init($parent) {
            _userInfo = uc.IContactService.getCurrentUserInfo();

            $parent.html("");
            $minutesPanel = $(MINUTES_TEMPLATE);
            _bindEvent();

            $parent.append($minutesPanel);
        }

        function _bindEvent() {
            $minutesPanel.on("click", ".minutes-name", function () {
                if($(this).parent().hasClass("minutes-video")){
                    var videoURL=$(this).parent().attr("videoURL");
                    uc.IUIService.triggerCrossWindowEvent(uc.constants.UI.CrossWindowEventName.OnMeetingVideoMake, {uniqueName: 'messageVideoMake',
                        videoURL:videoURL});
                    return;
                }
                var options = {
                    width: 540,
                    height: 530,
                    refreshIfExist: false,
                    centerScreen: true,
                    modal: true,
                    resizable: false,
                    caption: uc.constants.UI.DragCaption.EditMeetingMinutes
                };

                var summaryId = parseInt($(this).parent().attr("summaryId"));
                uc.util.StorageUtil.setData("Meeting-Minutes:summaryId", summaryId);
                uc.util.StorageUtil.setData("Meeting-Minutes:Data", { eventId: _meeting.eventId });
                if ($(this).parent().find(".minutes-delete").length) {
                    uc.util.WindowUtil.open("page/editMinutes.html", "editMinutes", options);

                } else {
                    uc.util.WindowUtil.open("page/showMinutes.html", "showMinutes", options);
                }
            });

            $minutesPanel.on("click", ".add-minutes", function () {
                var options = {
                    width: 540,
                    height: 530,
                    refreshIfExist: false,
                    centerScreen: true,
                    modal: true,
                    resizable: false,
                    caption: uc.constants.UI.DragCaption.EditMeetingMinutes
                };
                uc.util.StorageUtil.setData("Meeting-Minutes:summaryId", '');
                uc.util.StorageUtil.setData("Meeting-Minutes:Data", _meeting);
                uc.util.WindowUtil.open("page/editMinutes.html", "editMinutes", options);
            });
            $minutesPanel.on("click", ".create-minutes", function () {
                $minutesPanel.find(".add-minutes").trigger("click");
            });

            $minutesPanel.on("click", ".minutes-forward", function () {
                var videoName=$(this).parent().attr("videoName");
                var videoSize=$(this).parent().attr("videoSize");
                var videoURL=$(this).parent().attr("videoURL");
                var videoData=$(this).parent().attr("videoThumb");
                var videoDuration=$(this).parent().attr("videoDuration");
                var operatorName=$(this).parent().attr("operatorName");
                var operatorId=$(this).parent().attr("operatorId");
                var operatorAvatar=$(this).parent().attr("operatorAvatar");
                var thumbnailsURL=$(this).parent().attr("thumbnailsURL");
                uc.IUIService.openChooseContactWindow(
                    {
                        key: "forwardMessage" + (new Date().getTime()),
                        allowMultipleSelect: false,
                        allowChooseOrg:false
                    }, function (args) {
                        console.log(args);
                        uc.IUIService.triggerCrossWindowEvent(uc.constants.UI.CrossWindowEventName.OnForwardMeetingVideo, {uniqueName: 'messageChat', args:args,
                            videoName:videoName,videoSize:videoSize,videoURL:videoURL,videoData:videoData,thumbnailsURL:thumbnailsURL,
                            videoDuration:videoDuration,operatorName:operatorName,operatorId:operatorId,operatorAvatar:operatorAvatar,from:'minutes'});
                    });
            });

            $minutesPanel.on("click", ".minutes-video-delete", function () {
                uc.IMeetingService.deleteConferenceRecording(
                        _meeting.eventId,
                        _meeting.conferenceId,
                        _meeting.startTime,function (args) {
                        if(args[0]==0){
                            uc.util.LogUtil.info("MeetingMinutes", "minutes-delete", "deleteConferenceRecording",
                                {
                                    eventId: _meeting.eventId,
                                    confId: _meeting.conferenceId,
                                    startTime: _meeting.startTime
                                });
                        }
                    })
            });
            $minutesPanel.on("click", ".minutes-share", function () {
                var videoName=$(this).parent().attr("videoName");
                var videoSize=$(this).parent().attr("videoSize");
                var videoURL=$(this).parent().attr("videoURL");
                var videoData=$(this).parent().attr("videoThumb");
                var videoDuration=$(this).parent().attr("videoDuration");
                var operatorName=$(this).parent().attr("operatorName");
                var operatorId=$(this).parent().attr("operatorId");
                var operatorAvatar=$(this).parent().attr("operatorAvatar");
                var thumbnailsURL=$(this).parent().attr("thumbnailsURL");
                var meetingName= _meeting.title;
                uc.IUIService.triggerCrossWindowEvent(uc.constants.UI.CrossWindowEventName.OnShareMeetingVideo, {uniqueName: 'shareMeetingVideo',
                    videoURL:videoURL,meetingName:meetingName,videoSize:videoSize,thumbnailsURL:thumbnailsURL,videoDuration:videoDuration});
            });
        }

        function _show(meeting) {
            _meeting = meeting;

            _getMinutesList(_meeting.eventId, _meeting.startTime);
        }

        function _getMinutesList(eventId, startTime) {
            return;//meeting minute 已关闭，此代码暂留
            uc.IMeetingService.getMinutesList(eventId, startTime, function (arg) {
            });
        }

        function _renderMeetingMinute(meetingMinute, unKnow) {
            var timeStr = "", time, start, end, html = '', createTime;
            if (meetingMinute.updateTime) {
                if (meetingMinute.updateTime == meetingMinute.createTime) {
                    timeStr = uc_resource.Meeting.CreateSummaryDate;
                    start = uc.util.TimeUtil.UnixTimeToDisplayTime(parseInt(meetingMinute.createTime),
                            uc.settings.meeting.timeZone.offset, uc.constants.UI.TimeStampUnit.Second);
                    time = uc.util.TimeUtil.DateToString(start, 7);
                    createTime = meetingMinute.createTime;
                } else {
                    timeStr = uc_resource.Meeting.UpdateSummaryDate;
                    end = uc.util.TimeUtil.UnixTimeToDisplayTime(parseInt(meetingMinute.updateTime), uc.settings.meeting.timeZone.offset,
                        uc.constants.UI.TimeStampUnit.Second);
                    time = uc.util.TimeUtil.DateToString(end, 7);
                    createTime = meetingMinute.updateTime;
                }
            } else {
                timeStr = uc_resource.Meeting.CreateSummaryDate;
                start = uc.util.TimeUtil.UnixTimeToDisplayTime(parseInt(meetingMinute.createTime), uc.settings.meeting.timeZone.offset,
                    uc.constants.UI.TimeStampUnit.Second);
                time = uc.util.TimeUtil.DateToString(start, 7);
                createTime = meetingMinute.createTime;
            }
           if(!meetingMinute.type){
               if (meetingMinute.creatorId == _userInfo.userId || !meetingMinute.creatorId) {
                   html += "<div class='minutes-view minutes-person' summaryId='" + meetingMinute.minutesId + "'>" +
                       "<a class='minutes-name' create_id='" + meetingMinute.creatorId + "'><span class='userName'>" + _userInfo.displayName + "</span>"+uc_resource.Meeting.OthersAddSummary+"</a>" +
                       "<span class='minutes-time' createTime='" + createTime + "'>" + timeStr + " " + time + "</span>" +
                       "<a class='minutes-delete'></a>" +
                       "</div>"
               } else {
                   html += "<div class='minutes-view minutes-person' summaryId='" + meetingMinute.minutesId + "'>" +
                       "<a class='minutes-name' create_id='" + meetingMinute.creatorId + "'><span class='userName'></span>"+uc_resource.Meeting.OthersAddSummary+"</a>" +
                       "<span class='minutes-time' createTime='" + createTime + "'>" + timeStr + " " + time + "</span>" +
                       "</div>";
                   unKnow.push(meetingMinute.creatorId);
               }
           }else{
               if (meetingMinute.creatorId == _userInfo.userId) {
                   html += "<div class='minutes-view minutes-video icon-minutes-video' summaryId='" + meetingMinute.minutesId + "' videoName='" + meetingMinute.videoName + "'" +
                       "videoSize='" + meetingMinute.size + "' videoURL='" + meetingMinute.videoUrl + "' videoURL='" + meetingMinute.videoURL + "'" +
                       "videoThumb='" + meetingMinute.videoThumb + "' videoDuration='" + meetingMinute.length + "' operatorId='"+meetingMinute.operatorId+"'" +
                   "operatorName='"+meetingMinute.operatorName+"' operatorAvatar='"+meetingMinute.operatorAvatar+"' thumbnailsURL='"+meetingMinute.videoThumbUrl+"'>" +
                       "<a class='minutes-name' create_id='" + meetingMinute.operatorId + "'><span class='userName'>" + _meeting.title + "</span>"+uc_resource.Meeting.MeetingVideo+"</a>" +
                       "<span class='minutes-time' createTime='" + createTime + "'>" + timeStr + " " + time + "</span>" +
                       "<a class='minutes-forward'></a>" +
                       "<a class='minutes-share'></a>" +
                      // "<a class='minutes-video-delete'></a>" +
                       "</div>"
               } else {
                   html += "<div class='minutes-view minutes-video icon-minutes-video' summaryId='" + meetingMinute.minutesId + "' videoName='" + meetingMinute.videoName + "'" +
                       "videoSize='" + meetingMinute.size + "' videoURL='" + meetingMinute.videoUrl + "' videoURL='" + meetingMinute.videoURL + "'" +
                       "videoThumb='" + meetingMinute.videoThumb + "' videoDuration='" + meetingMinute.length + "' thumbnailsURL='"+meetingMinute.videoThumbUrl+"'>" +
                       "<a class='minutes-name' create_id='" + meetingMinute.operatorId + "'><span class='userName'>" + _meeting.title + "</span>"+uc_resource.Meeting.MeetingVideo+"</a>" +
                       "<span class='minutes-time' createTime='" + createTime + "'>" + timeStr + " " + time + "</span>" +
                       "<a class='minutes-forward'></a>" +
                       "<a class='minutes-share'></a>" +
                       "</div>";
               }
           }
            return html;
        }

        function _bindMeetingMinutes(meetingMinutes) {
            $minutesPanel.find(".minutes-list").html("");
            if(meetingMinutes.length>=2){
                meetingMinutes.sort(function (meetingMinutes1, meetingMinutes2) {
                    if(meetingMinutes1.updateTime && meetingMinutes2.updateTime){
                        return meetingMinutes1.updateTime - meetingMinutes2.updateTime;
                    }else if(meetingMinutes1.updateTime && !meetingMinutes2.updateTime){
                        return meetingMinutes1.updateTime - meetingMinutes2.createTime;
                    }else if(!meetingMinutes1.updateTime && meetingMinutes2.updateTime){
                        return meetingMinutes1.createTime - meetingMinutes2.updateTime;
                    }else{
                        return meetingMinutes1.createTime - meetingMinutes2.createTime;
                    }
                });
            }

            var html = '', unKnow = [];
            for (var i = 0; i < meetingMinutes.length; i++) {
                html += _renderMeetingMinute(meetingMinutes[i], unKnow);
            }
            if(html){
                $minutesPanel.find(".minutes-content").show();
                $minutesPanel.find(".minutes-list").append(html);
            }else{
                $minutesPanel.find(".noMinutes-tip").show();
            }
            if (unKnow && unKnow.length) {
                uc.IContactService.getBasicContactInfo(unKnow);
            }
        }

        function _removeMeetingMinutes(){
            if($minutesPanel.find(".minutes-video").length){
                $minutesPanel.find(".minutes-video").remove();
            }
            if($minutesPanel.find(".minutes-view").length){
                $minutesPanel.find(".minutes-content").show();
                $minutesPanel.find(".minutes-list").append(html);
            }else{
                $minutesPanel.find(".minutes-content").hide();
                $minutesPanel.find(".noMinutes-tip").show();
            }
        }

        function _setEditorNames(editors) {
            for (var i = 0; i < editors.length; i++) {
                $minutesPanel.find(".minutes-name[create_id=" + editors[i].userId + "]").find("span.userName").text(editors[i].displayName);
            }
        }

        function _addMeetingMinute(meetingMinute) {
            if ($minutesPanel.find(".minutes-view[summaryId=" + meetingMinute.minutesId + "]").length) {
                return;
            }

            var unKnow = [], html = _renderMeetingMinute(meetingMinute, unKnow);

            var $minutesViews = $minutesPanel.find(".minutes-view");
            if ($minutesViews.length) {
                var hadMinutes=false;
                $minutesViews.each(function () {
                    if (parseInt($(this).find(".minutes-time").attr("createTime")) >= meetingMinute.createTime) {
                        $(this).before(html);
                        hadMinutes=true;
                        return false;
                    }
                });
                if(!hadMinutes){
                    $minutesPanel.find(".noMinutes-tip").hide();
                    $minutesPanel.find(".minutes-view:last").after(html);
                }
            } else {
                $minutesPanel.find(".noMinutes-tip").hide();
                $minutesPanel.find(".minutes-content").show();
                $minutesPanel.find(".minutes-list").append(html);
            }

            if (unKnow && unKnow.length) {
                uc.IContactService.getBasicContactInfo(unKnow);
            }
        }

        function _updateMeetingMinute(meetingMinute) {
            $minutesPanel.find(".minutes-view[summaryId=" + meetingMinute.minutesId + "]").remove();
            _addMeetingMinute(meetingMinute);
        }





        this.init = function ($container, detail) {
            _init($container, detail);
        };

        this.show = function (meeting) {
           // if (_meeting && _meeting.eventId == meeting.eventId) {
                _show(meeting);
           // }
        };

        this.bindMeetingMinutes = function (eventId,meetingMinutes) {
            if(_meeting && _meeting.eventId==eventId){
                _bindMeetingMinutes(meetingMinutes);
            }
        };

        this.showEditors = function (editors) {
            _setEditorNames(editors);
        };

        //TODO: not sure to be used
        //this.show = function (data) {
        //    _meeting = data;
        //    $minutesPanel = $(MINUTES_TEMPLATE);
        //    _getMinutesList(data.conferenceId);
        //};

        this.addMeetingMinute = function (meetingMinute) {
            if (meetingMinute && meetingMinute.eventId == _meeting.eventId){
                _addMeetingMinute(meetingMinute);
            }
        };

        this.updateMeetingMinute = function (meetingMinute) {
            if (meetingMinute && meetingMinute.eventId == _meeting.eventId){
                _updateMeetingMinute(meetingMinute);
            }
        };

        this.removeMeetingMinutes=function(){
            _removeMeetingMinutes();
        }
    };
})(uc);


