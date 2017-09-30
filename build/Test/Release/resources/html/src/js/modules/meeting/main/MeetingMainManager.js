/**
 * Created by waouyang on 16/4/27.
 */
(function (uc) {
    var _this = null;
    uc.modules.meeting.MeetingMainManager = {
        showingMeetingReminder: 0,
        moduleId: 0,
        meetingRoomMeetingInfo: new uc.modules.meeting.room.roomDataMap(),
        init: function () {
            _this = this;
        },

        OnInitClient: function (clearData) {
            this.init();
        },

        OnModuleChanged: function (moduleId, mountId, dataObj) {
            this.moduleId = moduleId;
        },

        OnMeetingReminderMessageReceived: function (meetings) {
            if (meetings && meetings.length) {
                meetings.map(function (meetingInfo) {
                    var position = {
                        left: (window.screen.availWidth - 370) / 2 + 20 * _this.showingMeetingReminder,
                        top: (window.screen.availHeight - 230) / 2 + 20 * _this.showingMeetingReminder
                    };
                    var key = "Calendar:MeetingReminder_" + meetingInfo.eventId;
                    uc.util.StorageUtil.setData(key, meetingInfo);

                    uc.IUIService.bindCrossWindowEvent(key, function (args) {
                        _this.showingMeetingReminder--;

                        if (args.isAttend) {
                            //uc.modules.meeting.MeetingDetailManager.joinMeeting(meetingInfo.conferenceId, meetingInfo.eventId, meetingInfo.startTime, meetingInfo.authorizerId);
                            uc.util.LogUtil.info('MeetingManager', 'joinMeeting', 'eventId', args.eventId);
                            var meetings = uc.IMeetingService.getMeetingInfoByIdSync(args.eventId, 0, 0, 0);
                            var userInfo = uc.IContactService.getCurrentUserInfo();
                            if (meetings && meetings.length) {
                                var meetingInfo = meetings[0];
                                uc.IUIService.joinMeeting(meetingInfo, true);
                            }
                        }
                    });

                    uc.util.WindowUtil.open("page/meetingReminder.html?" + meetingInfo.eventId, key, {
                        title: uc_resource.Meeting.MeetingReminderTitle,
                        left: position.left,
                        top: position.top,
                        width: 370,
                        height: 230,
                        refreshIfExist: false,
                        centerScreen: false,
                        modal: false,
                        resizable: false,
                        isChild: false,
                        isTop: true,
                        caption: uc.constants.UI.DragCaption.AlarmedMeeting
                    });
                    this.showingMeetingReminder++;
                })
            }
        },

        OnOpenMeetingAttendeeBusyStatusWindow: function (uniqueName, data) {
            uc.util.LogUtil.info("MeetingEventListener", "OnOpenMeetingAttendeeBusyStatusWindow", "data=", {
                uniqueName: uniqueName,
                data: data
            });
            var options = {
                width: 1000,
                height: 550,
                refreshIfExist: false,
                centerScreen: true,
                modal: true,
                resizable: false,
                caption: uc.constants.UI.DragCaption.MeetingAttendeeList
            };

            uc.util.WindowUtil.open("page/meetingAttendeeBusyStatus.html", "PopupCalendar", options);
        },

        OnJoinMeeting: function (meetingInfo, isFromMeetingModule, isCheckMeetingRoomMeeting) {
            uc.util.LogUtil.info('MeetingMainManager', 'OnJoinMeeting', 'join meeting', {meetingInfo: meetingInfo});
            var self = this;
            var isCheckMeetingRoomMeeting = isCheckMeetingRoomMeeting == null ? false : true;
            if (meetingInfo) {
                if (isCheckMeetingRoomMeeting) {
                    if (this.CheckIsMeetingRoomMeeting(meetingInfo)) {
                        this.meetingRoomMeetingInfo.addRoom(meetingInfo.roomId, [meetingInfo, isFromMeetingModule]);
                        uc.IMeetingRoomService.checkJoinMeetingRoom(meetingInfo.roomId, function (args) {
                            //uc.modules.meeting.MeetingDetailManager.OnWaitJoinMeeting();
                        });
                        return;
                    }
                }
                var password = 0, userInfo = uc.IContactService.getCurrentUserInfo();
                password = (meetingInfo.hostId == userInfo.userId ? meetingInfo.hostPassword : meetingInfo.attendeePassword);

                uc.IMeetingService.checkMeetingClient(function (arg) {
                    uc.util.LogUtil.info("MeetingMainManager", "OnJoinMeeting", "checkMeetingClient", {
                        errorCode: arg[0],
                        password: password
                    });

                    if (arg[0] == 0) {
                        var meetingStartUrl = uc.IMeetingService.getMeetingStartURLSync(meetingInfo.conferenceId, password);

                        uc.util.LogUtil.info("MeetingMainManager", "OnJoinMeeting", "getMeetingStartURLSync", {
                            eventId: meetingInfo.eventId,
                            password: password,
                            meetingStartUrl: meetingStartUrl
                        });

                        if (meetingStartUrl.startURL) {
                            self._startMeetingClient(meetingStartUrl, meetingInfo, isFromMeetingModule);
                        } else {
                            uc.modules.meeting.MeetingDetailManager.OnJoinMeetingFailed(null);
                        }
                    } else {
                        uc.util.LogUtil.info("MeetingMainManager", "OnJoinMeeting", "getMeetingWebStartUrl",
                            {
                                confId: meetingInfo.conferenceId,
                                mail: userInfo.email,
                                password: password,
                                displayName: userInfo.displayName
                            });
                        uc.IMeetingService.getMeetingWebStartUrl(
                            meetingInfo.conferenceId,
                            userInfo.email,
                            password,
                            userInfo.displayName, function (args) {
                                uc.util.LogUtil.info("MeetingMainManager", "OnJoinMeeting", "getMeetingWebStartUrl",
                                    {errorCode: args[0]});
                            });
                    }
                });
            }
        },

        _startMeetingClient: function (meetingStartUrl, meetingInfo, isFromMeetingModule) {
            var self = this, siteId = meetingStartUrl.siteId;
            uc.util.LogUtil.info("MeetingMainManager", "_startMeetingClient", "params", {
                meetingStartUrl: meetingStartUrl,
                meetingInfo: meetingInfo,
                isFromMeetingModule: isFromMeetingModule
            });

            uc.IMeetingService.startMeetingClient(meetingStartUrl.startURL, function (arg) {
                uc.util.LogUtil.info("MeetingMainManager", "_startMeetingClient", "startMeetingClient", {errorCode: arg[0]});
                if (arg[0] == 0) {

                    /*
                     *  如果开启了动画并且云会议版本大于等于60001，入会时需要开启入会动画，请不要删除注释
                     */
                    if (uc.constants.Meeting.useAnimation && siteId && siteId >= 60001) {
                        uc.IUIService.showJoinAnimation();
                        //self._showJoinMeetingAnimation();
                    }

                    if (isFromMeetingModule) {
                        uc.modules.meeting.MeetingDetailManager.OnWaitJoinMeeting();
                        uc.IUIEvent.OnWaitJoinMeetingRoomMeeting();
                    }
                    var start = setInterval(function () {
                        var isMeetingClient = uc.IMeetingService.getMeetingClientLaunchStatus();
                        if (isMeetingClient.MeetingClient.visible) {
                            uc.modules.meeting.MeetingDetailManager.OnMeetingJoined();
                            uc.IUIEvent.OnMeetingRoomMeetingJoined();
                            clearInterval(start);
                        }
                    }, uc.constants.Timer.OneAndAHalfSeconds);

                    var timeOutTimer = setTimeout(function () {
                        if (start) {
                            uc.modules.meeting.MeetingDetailManager.OnMeetingJoined();
                            uc.IUIEvent.OnMeetingRoomMeetingJoined();
                            clearInterval(start);
                        }
                        clearTimeout(timeOutTimer);
                    }, uc.constants.Timer.HalfMinutes);
                } else {
                    uc.modules.meeting.MeetingDetailManager.OnJoinMeetingFailed();
                    uc.IUIEvent.OnJoinMeetingRoomMeetingFailed(arg[0]);
                }
            });
        },

        OnMeetingWebStartUrlReceived: function (errorCode, startUrl, eventId) {
            if (errorCode == 0) {
                window.open(startUrl);
                uc.modules.meeting.MeetingDetailManager.OnMeetingJoined();
            } else {
                uc.modules.meeting.MeetingDetailManager.OnJoinMeetingFailed();
            }
        },

        _showJoinMeetingAnimation: function () {
            $('#layout-container').fadeOut(1000, function () {
                uc.util.WindowUtil.minimize();
                $('#layout-container').fadeIn();
            });

            uc.util.WindowUtil.open("page/joinMeeting.html", 'waitJoinMeeting', {
                title: uc_resource.Meeting.MeetingWiatJoinTitle,
                top: 250,
                left: screen.width - 220,
                width: 200,
                height: 200,
                refreshIfExist: false,
                centerScreen: false,
                modal: false,
                resizable: false,
                isChild: false,
                isTop: true,
                hasTaskBarIcon: false,
                caption: uc.constants.UI.DragCaption.JoinMeeting
            });
        },

        /**
         * 检测是否会议室会议，如果是，进行会议室入会相关检测
         * */
        CheckIsMeetingRoomMeeting: function (meetingInfo) {
            return meetingInfo && meetingInfo.roomId != undefined && meetingInfo.roomId != '' && meetingInfo.roomId != 0;

        },

        //检查会议室是否可用事件
        OnCheckJoinMeetingRoom: function (errorCode, roomId, hasPassword, isValid, mayJoin) {
            //[],0:meetingInfo;1:isFromMeetingModule
            var meetingInfo = this.meetingRoomMeetingInfo.getRoomInfo(roomId);
            if (!meetingInfo) {
                uc.util.LogUtil.warn("uc.modules.meeting.MeetingMainManager", "OnCheckJoinMeetingRoom", "error_meetingInfo_roomId", roomId);
                return;
            }

            uc.util.LogUtil.info("uc.modules.meeting.MeetingMainManager", "OnCheckJoinMeetingRoom", "meetingInfo", meetingInfo);

            if (errorCode == uc.ErrorCode.Success && isValid &&mayJoin) {
                //如果会议有效并且需要输入会议室密码
                if (hasPassword) {
                    //弹出会议室密码输入框
                    var options = {key: "joinMeetingRoomMeeting", roomInfo: meetingInfo[0]};
                    uc.IUIService.joinMeetingRoomMeeting(options, function (data) {
                        if (data.success != undefined && data.success) {
                            //密码验证通过,入会
                            uc.IUIService.joinMeeting(meetingInfo[0], meetingInfo[1]);
                        } else if (data.cancel != undefined && data.cancel) {
                            //取消，需要优化，应该统一使用事件
                            uc.modules.meeting.MeetingDetailManager.OnMeetingJoined();
                            uc.IUIEvent.OnMeetingRoomMeetingJoined();
                        }
                    });
                } else {
                    //不需要密码直接入会
                    uc.IUIService.joinMeeting(meetingInfo[0], meetingInfo[1]);
                }
            } else {

	            //isValid字段可以判断会议室是否在有效期内，取值： 0 不在有效期 1在有效期内。
	            //mayJoin： 1,可以入会 0不是默认参会人不能入会”，上层可以用这个错误码来区分不是默认参会人的入会失败场景

	            if(isValid == 0){
		            //会议无效，不再会议有效期呢
		            this.confirm(uc_resource.Meeting.meetingRoom.notAllowJoin_invalid, function () {
		            });
		            return;
	            }
	            if(mayJoin == 0){
		            //会议无效，不是默认参会人
		            this.confirm(uc_resource.Meeting.meetingRoom.notAllowJoin_IsNotDefaultAttendee, function () {
		            });
		            return;
	            }
            }
            //删除缓存的数据
	        this.meetingRoomMeetingInfo.deleteRoom(roomId);
        },

        confirm: function (msg, fn, iscannel) {
            var cannelText = "";
            var modeTxt = "ok"
            if (iscannel) {
                modeTxt = "both";
                cannelText = uc_resource.Common.Cancel;
            }
            uc.modules.component.PromptMessageBox.open({
                title: uc_resource.WindowTitle.Notification,
                message: msg,
                buttonMode: "ok",
                OkText: uc_resource.Common.OK,
                buttonMode: modeTxt,
                CancelText: cannelText
            }, function (r) {
                if (fn != null) {
                    fn(r);
                }
                this.close();
            });
        }
    }
})(uc);