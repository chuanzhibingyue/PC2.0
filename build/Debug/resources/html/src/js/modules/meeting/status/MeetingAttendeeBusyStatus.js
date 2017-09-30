/**
 * Created by waouyang on 16/5/6.
 */
(function (uc) {
    var _this = null;
    uc.modules.meeting.MeetingAttendeeBusyStatus = {
        attendeeItemHtml: "<a class='attendee' href='javascript:void(0);'>"
                        +   "<span class='attendee-name'></span>"
                        + "</a>",
        init: function () {
            _this = this;

            this._init();
            this.bindEvent();
        },

        _init: function () {
            uc.modules.component.TitleBarButton.init();

            this.searchNode = $('.search-input');
            this.startTimeNode = $('.start-time');
            this.durationNode = $('.duration');
            this.timePointsContainer = $('.time-pointer-container');
            this.timePointsNode = $(".time-pointers", this.timePointsContainer);
            this.attendeeListContainer = $('.attendee-list-container');
            this.attendeeList = $('.attendee-list');
            this.verticalScrollBar = $('.vertical-scroll');
            this.verticalScrollBarContent = $('.content-length', this.verticalScrollBar);
            this.horizontalScrollBar = $('.horizontal-scroll');
            this.horizontalScrollBarContent = $('.content-length', this.horizontalScrollBar);
            this.meetingTimeLine = $('.meeting-timeline');
            this.attendeeWorksNode = $('.attendee-works');
            this.timeLines = $('.timelines');
            this.finishBtn = $('.finish');
            this.cancelBtn = $('.cancel');

            this.timeInterval = 15;
            this.scrollStep = 20;
            this.scrollLeft = 0;
            this.busyTimeCoordinate = {};
            this.pixelPerMinute = 1;
            this.userBusyTimes = {};
            this.userBusyTimeLine = [];//all user's busy time line, example: [10:00-11:00, 13:00-14:00]

            this.datePicker = new uc.modules.component.PickADate(this.startTimeNode, true, true, function (date) {
                    if (date != null) {
                        _this._onDatePickerTimeChanged(date);
                    }
                }
            );

            uc.modules.component.SearchBox.init(this.searchNode, {
                count: 4,
                selectedUser: function (user) {
                    _this.onContactSelected(user);
                }
            });

            this.durationPicker = new uc.modules.component.MeetingDurationPicker({
                parentNode: this.durationNode,
                onItemSelected: function(selectedItem){
                    _this._onMeetingDurationSelected(selectedItem);
                }
            });

            this.meetingTimeLine.draggable({
                axis: "x",
                grid: [15]
            }).resizable({
                minWidth: 15,
                handles: "e, w",
                grid: [15]
            });

            this._initBusyTimeCoordinate();
            this._renderTimePointers();


            //this._initRuler();

            this.attendeeData = this._getAttendeeData();
            if(!this.attendeeData){
                return;
            }
            this._showMeetingTimeLine(this.attendeeData.meetingTime);
            var meetingDisplayTime = uc.util.TimeUtil.UnixTimeToDisplayTime(this.attendeeData.meetingTime.startTime,
                uc.settings.meeting.timeZone.offset, uc.constants.UI.TimeStampUnit.Second);
            this.currentDay = uc.util.TimeUtil.displayTimeToDayBeginningUnixTime(meetingDisplayTime,
                uc.settings.meeting.timeZone.offset, uc.constants.UI.TimeStampUnit.Second);

            this.datePicker.setDate(meetingDisplayTime);
            this.durationPicker.setValue(this.attendeeData.meetingTime.duration);
            this._showAttendees();
        },

        bindEvent: function () {
            this.attendeeList.on('click', '.delete-attendee', function (e) {
                _this._deleteAttendee(parseInt($(this).parent().attr("userId")));
                e.stopPropagation();
            });
            this.verticalScrollBar.on('scroll', function () {
                _this._onVerticalScrollBarScrolled();
            });

            this.horizontalScrollBar.on('scroll', function () {
                _this._onHorizontalScrollBarScrolled();
            });

            this.finishBtn.click(function () {
                _this._save();
            });
            this.cancelBtn.click(function () {
                _this._cancel();
            });

            this.meetingTimeLine.on("drag", function (e, ui) {
                _this._onMeetingTimeLineDrag(e, ui);
            }).on("dragstop", function (e, ui) {
                _this._onMeetingTimeLineDragStop(e, ui);
            }).on("resize", function (e, ui) {
                _this._onMeetingTimeLineResize(e, ui);
            });
            this.attendeeListContainer.on("mousewheel", function (e, delta) {
                var $this = $(this);

                $this.scrollTop($this.scrollTop() - delta * _this.scrollStep);

                _this._onAttendeeListScrolled();
            });

            this.attendeeWorksNode.on("mousewheel", function (e, delta) {
                var $this = $(this);
                $this.scrollTop($this.scrollTop() - delta * _this.scrollStep);
                _this._onAttendeeTimeLineScrolled();

            });
        },

        _onMeetingDurationSelected: function(selectedItem){
            if(selectedItem){
                this.attendeeData.meetingTime.duration = selectedItem.value;
                var endTime = this.attendeeData.meetingTime.startTime + parseInt(selectedItem.value * 60);
                this.meetingTimeLine.attr('endTime', endTime);
                this.meetingTimeLine.width(parseInt(selectedItem.value / this.pixelPerMinute));
                _this._showMeetingStatus();
            }
        },

        _onMeetingTimeLineDrag: function(e, ui){
            if(ui.position.left + this.scrollLeft < 0){
                ui.position.left = -this.scrollLeft;
                return;
            }

            if(this.scrollLeft + ui.position.left + this.meetingTimeLine.width() > this.timeLines.width()){
                ui.position.left = this.timeLines.width() - this.meetingTimeLine.width() - this.scrollLeft;
                return;
            }
            var startTime = this._transferPositionToTime(ui.position.left);

            startTime = this.currentDay + startTime.hours * 60 * 60 + startTime.duration * 60;
            this.attendeeData.meetingTime.startTime = startTime;

            _this._showMeetingStatus();
        },

        _onMeetingTimeLineDragStop: function(e, ui){
            var startTime = this._transferPositionToTime(ui.position.left);

            startTime = this.currentDay + startTime.hours * 60 * 60 + startTime.duration * 60;
            var endTime = startTime + parseInt(this.meetingTimeLine.width() / this.pixelPerMinute * 60);

            this.meetingTimeLine.attr("startTime", startTime).attr("endTime", endTime);
            this.meetingTimeLine.attr('left', ui.position.left + this.scrollLeft);

            this.datePicker.setDate(new Date(startTime * 1000));
        },

        _onMeetingTimeLineResize: function(e, ui){
            if(ui.position.left + this.scrollLeft < 0){
                ui.position.left = -this.scrollLeft;
                return;
            }

            if(this.scrollLeft + ui.position.left + this.meetingTimeLine.width() > this.timeLines.width()){
                ui.position.left = this.timeLines.width() - this.meetingTimeLine.width() - this.scrollLeft;
                return;
            }

            var startTime = this._transferPositionToTime(ui.position.left);

            startTime = this.currentDay + startTime.hours * 60 * 60 + startTime.duration * 60;
            var endTime = startTime + parseInt(this.meetingTimeLine.width() / this.pixelPerMinute * 60);

            this.meetingTimeLine.attr("startTime", startTime).attr("endTime", endTime);
            this.meetingTimeLine.attr('left', ui.position.left + this.scrollLeft);

            this.attendeeData.meetingTime.startTime = startTime;
            this.attendeeData.meetingTime.duration = parseInt(ui.size.width / this.pixelPerMinute);

            this.durationPicker.setValue(this.attendeeData.meetingTime.duration);
            this.datePicker.setDate(new Date(startTime * 1000));

            this._showMeetingStatus();
        },

        _onDatePickerTimeChanged: function(time){
            var zeroTime = uc.util.TimeUtil.displayTimeToDayBeginningUnixTime(time, uc.settings.meeting.timeZone.offset, uc.constants.UI.TimeStampUnit.Second);
            if(zeroTime != this.currentDay){
                this.userBusyTimes = {};
                this.timeLines.find('.timeline-segment').remove();
                this.meetingTimeLine.removeClass("meeting-timeline-busy");
                this.currentDay = zeroTime;

                this._getUserBusyFreeStatus(zeroTime, this.userIds);
            }

            this.attendeeData.meetingTime.startTime = uc.util.TimeUtil.displayTimeToUnixTime(time, uc.settings.meeting.timeZone.offset, uc.constants.UI.TimeStampUnit.Second);
            this._showMeetingTimeLine(this.attendeeData.meetingTime);

        },

        _deleteAttendee: function (userId) {
            this.attendeeList.find('.attendee[userId="' + userId + '"]').remove();
            this.timeLines.find('.timeline[userId="' + userId + '"]').remove();
            this.verticalScrollBarContent.height(this.attendeeList.height());

            delete this.userBusyTimes[userId];
            var index = this.userIds.indexOf(userId);
            if(index != -1){
                this.userIds.splice(index, 1);
            }
            index = -1;
            var attendee;
            for(var i = 0, len = this.allAttendees.length; i < len; i++){
                attendee = this.allAttendees[i];
                if(parseInt(attendee.account) == userId){
                    index = i;
                    break;
                }
            }

            if(index != -1){
                this.allAttendees.splice(index ,1);
            }

            this._composeUserBusyTimeLine();
            this._showMeetingStatus();
        },

        _onVerticalScrollBarScrolled: function () {
            var scrollTop = this.verticalScrollBar.prop('scrollTop');
            this.attendeeListContainer.scrollTop(scrollTop);
            this.attendeeWorksNode.scrollTop(scrollTop);
        },

        _onAttendeeListScrolled: function(){
            var top = this.attendeeListContainer.scrollTop();
            this.attendeeListContainer.scrollTop(top);
            this.verticalScrollBar.scrollTop(top);
        },

        _onAttendeeTimeLineScrolled: function(){
            var top = this.attendeeWorksNode.scrollTop();
            this.attendeeListContainer.scrollTop(top);
            this.verticalScrollBar.scrollTop(top);
        },

        _onHorizontalScrollBarScrolled: function () {
            this.scrollLeft = this.horizontalScrollBar.prop('scrollLeft');
            if(this.scrollLeft + this.attendeeWorksNode.width() > this.horizontalScrollBar.prop('scrollWidth')){
                this.scrollLeft = this.horizontalScrollBar.prop('scrollWidth') - this.attendeeWorksNode.width();
            }
            this.timePointsContainer.scrollLeft(this.scrollLeft);
            this.attendeeWorksNode.scrollLeft(this.scrollLeft);
            var meetingTimeLineLeft = parseInt(this.meetingTimeLine.attr('left')) ;
            this.meetingTimeLine.css('left', (meetingTimeLineLeft - this.scrollLeft) + 'px');
        },

        _save: function () {
            this.attendeeData.attendees = this.allAttendees.slice(1);
            //uc.util.StorageUtil.setData('MeetingAttendeeBusyStatusResult', this.attendeeData);
            uc.IUIService.triggerCrossWindowEvent(this.uniqueName, this.attendeeData);
            uc.util.WindowUtil.close();
        },

        _cancel: function () {
            uc.util.WindowUtil.close();
        },

        _initBusyTimeCoordinate: function () {
            for (var i = 0; i < 24 * 60; i += this.timeInterval) {
                this.busyTimeCoordinate[i] = false;
                this.busyTimeCoordinate[i] = 0;
            }
        },

        _renderTimePointers: function () {
            var template = "<span class='time-pointer'><span class='hour'>{0}</span>:00</span>";

            var html = "";
            for (var i = 0; i <= 23; i++) {
                html += template.format((i < 10 ? "0" + i : i));
            }

            $(html).appendTo(this.timePointsNode);
            this.horizontalScrollBarContent.width(this.pixelPerMinute * 60 * 24);
        },

        _showMeetingTimeLine: function (meetingTime) {
            var width = meetingTime.duration * this.pixelPerMinute,
                startTime = uc.util.TimeUtil.UnixTimeToDisplayDateInfo(meetingTime.startTime, uc.settings.meeting.timeZone.offset, uc.constants.UI.TimeStampUnit.Second),
                endTime = uc.util.TimeUtil.UnixTimeToDisplayDateInfo(meetingTime.startTime + meetingTime.duration * 60, uc.settings.meeting.timeZone.offset, uc.constants.UI.TimeStampUnit.Second);

            var left = this._transferTimeToPosition(startTime.hour, startTime.minute);

            this.meetingTimeLine.attr("startTime", meetingTime.startTime).attr("endTime", meetingTime.startTime + meetingTime.duration * 60).attr('left', left);
            this.meetingTimeLine.width(width).css("left", (left - this.scrollLeft) + "px").show();
        },

        getAllAttendees: function (attendees, hostId) {
            var attendeeData = [];

            attendeeData.push({
                account: hostId,
                isHost: true,
                type: uc.constants.Meeting.AttendeeType.User
            });

            for(var account in attendees){
                attendeeData.push(attendees[account]);
            }

            return attendeeData;
        },

        _showAttendees: function () {
            this.allAttendees = this.getAllAttendees(this.attendeeData.attendees, this.attendeeData.hostId);
            //attendeeData.sort(this.userOrderCompareFunc);

            this.userIds = [];
            var noDisplayNameUserIds = [];
            var userId;
            if (this.allAttendees && this.allAttendees.length) {
                this.allAttendees.map(function (attendee) {
                    if(attendee.type == uc.constants.Meeting.AttendeeType.User){
                        userId = parseInt(attendee.account);
                        if (_this.userIds.indexOf(userId) != -1) {
                            return;
                        }
                        _this.userIds.push(userId);
                        if(!attendee.displayName){
                            noDisplayNameUserIds.push(userId);
                        }
                    }

                    _this._appendAttendee(attendee);
                });

                uc.IContactService.getBasicContactInfo(noDisplayNameUserIds);
            }

            this.verticalScrollBarContent.height(this.attendeeList.height());
            this.horizontalScrollBar.scrollLeft(420);

            var displayTime = uc.util.TimeUtil.UnixTimeToDisplayTime(this.attendeeData.meetingTime.startTime,
                                    uc.settings.meeting.timeZone.offset, uc.constants.UI.TimeStampUnit.Second);
            var unixStartTime = uc.util.TimeUtil.displayTimeToDayBeginningUnixTime(displayTime,
                                uc.settings.meeting.timeZone.offset, uc.constants.UI.TimeStampUnit.Second);
            this._getUserBusyFreeStatus(unixStartTime, this.userIds);
        },

        _appendAttendee: function(attendee){
            var attendeeItem = $(this.attendeeItemHtml);
            attendeeItem.attr('userId', attendee.account);
            if(attendee.displayName){
                attendeeItem.find('.attendee-name').text(attendee.displayName);
            }else {
                attendeeItem.find('.attendee-name').text(attendee.account);
            }

            if (attendee.isHost) {
                attendeeItem.addClass('host');
                attendeeItem.append('<span class="attendee-host">' + uc_resource.Meeting.HostName + '</span>');
            } else {
                attendeeItem.append('<span class="delete-attendee"></span>');
            }

            this.attendeeList.append(attendeeItem);
            this.timeLines.append('<div class="timeline" userId="' + attendee.account + '"></div>');
        },

        _getUserBusyFreeStatus: function (unixStartTime, userIds) {
            if (userIds.length == 0) {
                return;
            }
            var unixEndTime = unixStartTime + 24 * 60 * 60;
            var len = (parseInt(userIds.length / 6) == userIds.length / 6) ? userIds.length / 6 : parseInt(userIds.length / 6) + 1;

            for (var j = 0; j < len; j++) {
                var disUserIds = [];
                var maxLen = j * 6 + 6 > userIds.length ? userIds.length : j * 6 + 6;
                for (var i = j * 6; i < maxLen; i++) {
                    disUserIds.push(userIds[i]);
                }
                uc.util.LogUtil.info("Meeting Schedule", "getUserBusyFreeStatusInfo", "data=", {
                    users: disUserIds,
                    startTime: unixStartTime,
                    endTime: unixEndTime,
                    TIME_INTERVAL: this.timeInterval
                });
                uc.IMeetingService.getUserBusyFreeStatusInfo(disUserIds, unixStartTime, unixEndTime, this.attendeeData.shareUserId, function (args) {
                    if (args[0] != 0) {
                        uc.util.LogUtil.info("Meeting Schedule", "getUserBusyFreeStatusInfo", "fail to get user schedule", args);
                    }
                });
            }
        },

        _showAttendeeBusyFreeStatus: function (schedules) {
            if (!schedules || schedules.length == 0) {
                return;
            }
            var updatingMeetingEventId = this.attendeeData.eventId;
            var meetingInfos, busyTime, busyTimes, startTime, endTime;
            schedules.map(function (schedule) {
                for(var userId in schedule){
                    busyTimes = [];
                    meetingInfos = schedule[userId];
                    if(meetingInfos && meetingInfos.length){
                        meetingInfos.map(function (meetingInfo) {
                            for(var eventId in meetingInfo){
                                if(updatingMeetingEventId == 0 || updatingMeetingEventId != 0 && eventId != updatingMeetingEventId){
                                    busyTime = meetingInfo[eventId];
                                    busyTimes.push(busyTime);
                                }
                            }
                        });
                    }
                    _this.userBusyTimes[userId] = busyTimes;
                    busyTimes.map(function (time) {
                        startTime = uc.util.TimeUtil.UnixTimeToDisplayDateInfo(time.startTime, uc.settings.meeting.timeZone.offset, uc.constants.UI.TimeStampUnit.Second);
                        endTime = uc.util.TimeUtil.UnixTimeToDisplayDateInfo(time.endTime, uc.settings.meeting.timeZone.offset, uc.constants.UI.TimeStampUnit.Second);

                        _this._renderBusyArea(userId, startTime, endTime);
                    });
                }
            });

            this._composeUserBusyTimeLine();
            this._showMeetingStatus();
        },

        _renderBusyArea: function (userId, startTime, endTime) {
            var $timeline = this.timeLines.find('.timeline[userId="' + userId + '"]');

            var left = this._transferTimeToPosition(startTime.hour, startTime.minute),
                right;
            if (endTime.hour == 0 && endTime.minute == 0 || endTime.day > startTime.day) {
                right = this._transferTimeToPosition(24, endTime.minute);
            } else {
                right = this._transferTimeToPosition(endTime.hour, endTime.minute);
            }
            var width = right - left;
            $("<div class='timeline-segment'/>").css("left", left + "px").width(width).appendTo($timeline);
        },

        _getOverlapTimeLine: function (busyTime) {
            var index = -1;
            var timeLine;
            for (var i = 0, len = this.userBusyTimeLine.length; i < len; i++) {
                timeLine = this.userBusyTimeLine[i];
                if (busyTime.startTime >= timeLine.startTime && busyTime.startTime <= timeLine.endTime) {
                    index = i;
                    break;
                }
            }
            return index;
        },

        _composeUserBusyTimeLine: function () {
            var busyTimeArray = [];
            var userBusyTime;
            for (var userId in this.userBusyTimes) {
                userBusyTime = this.userBusyTimes[userId];
                if (userBusyTime && userBusyTime.length) {
                    userBusyTime.map(function (busyTime) {
                        busyTimeArray.push(busyTime);
                    })
                }
            }

            busyTimeArray.sort(this._busyTimeCompareFunc);

            this.userBusyTimeLine = [];
            var overlapIndex, overlapTimeLine;
            busyTimeArray.map(function (busyTime) {
                overlapIndex = _this._getOverlapTimeLine(busyTime);
                if (overlapIndex == -1) {
                    _this.userBusyTimeLine.push(busyTime);
                } else {
                    overlapTimeLine = _this.userBusyTimeLine[overlapIndex];
                    _this.userBusyTimeLine[overlapIndex] = {
                        startTime: Math.min(busyTime.startTime, overlapTimeLine.startTime),
                        endTime: Math.max(busyTime.endTime, overlapTimeLine.endTime)
                    }
                }
            });

            this.userBusyTimeLine.sort(this._busyTimeCompareFunc);
            this._mergeOverlapTimeLine();
        },

        _mergeOverlapTimeLine: function () {
            var index = 0;
            var timeLine, nextTimeLine;
            while (index < this.userBusyTimeLine.length - 1) {
                timeLine = this.userBusyTimeLine[index];
                nextTimeLine = this.userBusyTimeLine[index + 1];
                if (timeLine.endTime >= nextTimeLine.startTime) {
                    timeLine.endTime = nextTimeLine.endTime;
                    this.userBusyTimeLine.splice(index + 1, 1);
                } else {
                    index++;
                }
            }
        },

        _busyTimeCompareFunc: function (busyTime1, busyTime2) {
            return busyTime1.startTime - busyTime2.startTime;
        },

        _isMeetingOverlapBusyTime: function () {
            var startTime = this.attendeeData.meetingTime.startTime,
                endTime = startTime + this.attendeeData.meetingTime.duration * 60;
            var overlap = false;
            this.userBusyTimeLine.map(function(timeLine){
                if((startTime < timeLine.startTime && endTime > timeLine.startTime)
                || (startTime < timeLine.endTime && startTime >= timeLine.startTime)){
                    overlap = true;
                }
            });
            return overlap;
        },

        _showMeetingStatus: function () {
            if (this._isMeetingOverlapBusyTime()) {
                this.meetingTimeLine.addClass("meeting-timeline-busy");
            } else {
                this.meetingTimeLine.removeClass("meeting-timeline-busy");
            }
        },

        userOrderCompareFunc: function (attendee1, attendee2) {
            if (attendee1.isHost && !attendee2.isHost) {
                return -1;
            } else if (!attendee1.isHost && attendee2.isHost) {
                return 1;
            } else {
                return -1;
            }
        },

        //_initRuler: function () {
        //    var time = uc.util.TimeUtil.UnixTimeToDisplayTime(new Date().getTime() / 1000, uc.settings.meeting.timeZone.offset, uc.constants.UI.TimeStampUnit.Second);
        //    var left = this._transferTimeToPosition(time.hour, time.minute);
        //
        //    $("div.time-rule").css("left", left + "px").show();
        //    var $timeBox = $("div.rule-time-box").css("left", (left - 15) + "px");
        //
        //    $timeBox.find("span.hour").text(time.hour < 10 ? "0" + time.hour : time.hour);
        //    $timeBox.find("span.minute").text(time.minute < 10 ? ":0" + time.minute : ":" + time.minute);
        //    $timeBox.show(0);
        //},

        _transferTimeToPosition: function (hours, duration) {
            return hours * 60 * this.pixelPerMinute + duration * this.pixelPerMinute;
        },

        _transferPositionToTime: function (position) {
            position += this.scrollLeft;
            var hours = Math.floor(position / 60 / this.pixelPerMinute),
                duration = Math.round((position % 60) / this.pixelPerMinute);
            return {
                hours: hours,
                duration: duration
            };
        },

        _getAttendeeData: function () {
            this.uniqueName = uc.util.StorageUtil.getData("Calendar:UniqueName");

            //var data = {
            //    eventId: eventId,
            //    hostId: hostId,
            //    attendees: [{
            //        account: '',
            //        displayName: '',
            //        type: uc.constants.Meeting.AttendeeType.User || uc.constants.Meeting.AttendeeType.Mail,
            //        status: ''
            //    }],
            //    meetingTime: {
            //        startTime: startTime, //in second
            //        durations: duration   //in minute
            //    }
            //};

            return uc.util.StorageUtil.getData(this.uniqueName);
        },
        
        isUserInAttendees: function (user) {
            var attendee, inAttendees = false;
            var account = user.userId ? user.userId : user.account;
            for(var i = 0, len = this.allAttendees.length; i < len;i++){
                attendee = this.allAttendees[i];
                if(account == attendee.account){
                    inAttendees = true;
                    break;
                }
            }
            return inAttendees;
        },

        onContactSelected: function (user) {
            this.searchNode.val('');
            if(!user){
                return;
            }

            if(this.isUserInAttendees(user)){
                return;
            }
            var attendee;
            if (user.userId) {
                attendee = {
                    account: user.userId,
                    userId: user.userId,
                    displayName: user.displayName,
                    type: uc.constants.Meeting.AttendeeType.User
                };
                this.userIds.push(user.userId);
            } else {
                attendee = {
                    account: user.account,
                    type: uc.constants.Meeting.AttendeeType.Mail,
                    displayName: user.account
                };
            }

            this.allAttendees.push(attendee);

            this._appendAttendee({
                account: attendee.account,
                isHost: false,
                displayName: attendee.displayName
            });

            this.verticalScrollBarContent.height(this.attendeeList.height());

            var displayTime = uc.util.TimeUtil.UnixTimeToDisplayTime(this.attendeeData.meetingTime.startTime,
                                        uc.settings.meeting.timeZone.offset, uc.constants.UI.TimeStampUnit.Second);
            var unixStartTime = uc.util.TimeUtil.displayTimeToDayBeginningUnixTime(displayTime,
                                        uc.settings.meeting.timeZone.offset, uc.constants.UI.TimeStampUnit.Second);
            if(user.userId){
                this._getUserBusyFreeStatus(unixStartTime, [user.userId]);
            }
        },

        OnBasicContactInfoReceived: function (contacts) {
            if (contacts && contacts.length) {
                var attendeeItem;
                contacts.map(function (contact) {
                    if (_this.userIds.indexOf(contact.userId) != -1) {
                        attendeeItem = _this.attendeeList.find('.attendee[userId="' + contact.userId + '"]');
                        attendeeItem.find('.attendee-name').text(contact.displayName);
                    }
                })
            }
        },

        OnUserBusyFreeStatusInfoReceived: function (schedules) {
            this._showAttendeeBusyFreeStatus(schedules);
        }
    };

    jQuery(document).ready(function () {
        uc.init();
        uc.modules.meeting.MeetingAttendeeBusyStatus.init();
    });
})(uc);