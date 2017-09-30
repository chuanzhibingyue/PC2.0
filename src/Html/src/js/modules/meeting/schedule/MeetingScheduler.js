/**
 * Created by waouyang on 16/4/27.
 */
(function (uc) {
    var _this = null;
    uc.modules.meeting.MeetingScheduler = {
        init: function () {
            _this = this;
             /*options = {
                 shareUserId: userId,
                 meetingInfo: meetingInfo,
                 type: uc.constants.Meeting.MeetingCreateType.Default || uc.constants.Meeting.MeetingCreateType.RoomMeeting,
                 isCreate: true || false,
                 users: [userId],
                 startTime: startTime
                 duration: meeting duration, in minutes
                 roomId: meeting room id
             }*/

            this.options = this._getScheduleMeetingOptions();

            if(!this.options){
                uc.util.LogUtil.info('MeetingSchedule', 'Init', 'Schedule Meeting options is null');
            }

            this._initUI();
            this.bindEvent();

            this.activeDate = uc.util.StorageUtil.getData('activeDate') ? uc.util.StorageUtil.getData('activeDate') : 0;
            this.meetingAddress = {
                "address": {
                    "longitude": "",
                    "latitude": "",
                    "type": uc.constants.Meeting.AddressType.Company,    //1-外出    2-公司内
                    "name": "",
                    "location": ""
                }
            };
            this.longitude = "";
            this.latitude = "";
            this.attendees = {};
            this.attendeeIds = [];//all the attendee ids, including host, exclude mail account, used for get busy status
            this.busyTimeUsers = {};
            this.loadingTimer = -1;
            this.hostId = 0;
            this.hostName = '';
            this.timeInterval = 15;
            this.loginUserInfo = uc.IContactService.getCurrentUserInfo();// uc.IContactService.getBasicContactInfo(shareUserId);

            if (this.options.isCreate) {
                this._initScheduleMeetingUI();
            } else {
                this._initUpdateMeetingUI();
            }

            if (this.options.type == uc.constants.Meeting.MeetingCreateType.RoomMeeting) {
                this._initRoomMeetingUI();
            }
        },

        _getScheduleMeetingOptions: function(){
            var _uniqueName = uc.util.StorageUtil.getData("MeetingEditor:UniqueName");
            if (!_uniqueName) {
                return null;
            }
            this.tirggerSource = _uniqueName;
            var options = uc.util.StorageUtil.getData(_uniqueName);
            uc.util.LogUtil.info('MeetingSchedulerManager', 'init', 'data = ', options);

            return options;
        },

        _initUI: function () {
            uc.modules.component.TitleBarButton.init();
            this.windowTitle = $("div.window-bar span.title");
            this.meetingRowObj = $('.meeting-rows');
            this.meetingTileInput = $(".meeting-rows .subject");
            this.meetingTitleLengthNode = $(".meeting-rows .input-length");
            this.batchAddUserBtn = $(".meeting-rows .btnBatchUser");
            this.searchAttendeeInput = $("#selectedAttendee");
            this.attendeeList = $(".attendee-list");
            this.wholeDayObj = $("div .whole-days");
            this.cloudMettingObj = $("div .cloud-meeting");
            this.timezoneObj = $("div .timeZonelist");
            this.startTimeNode = $(".start-time");
            this.durationNode = $(".meeting-rows .duration");
            this.isRepeatsObj = $(".date-time-area .isRepeats");
            this.addressObj = $(".meeting-rows .address");
            this.summaryNode = $(".meeting-rows .marks");
            this.loadingNode = $(".loading-background");
            this.attendeeBusyNode = $('.busy-node');
            this.viewAttendeeBusyStatusBtn = $('.view-busy-status', this.attendeeBusyNode);
            this.submitBtn = $('#save');
            this.cancelBtn = $('#cancel');
            this.hostNode = $(".emcee");
            this.activeDiv = "activeDiv";
            this.ErrMsg =$('.ErrMsg',this.meetingRowObj);

            this.timezoneCtr = new uc.modules.component.DropdownList(this.timezoneObj, {
                width: 250,
                navigateButtons: true,
                items: uc_resource.Meeting.CreateMeetingTimeZone,
                selectedByText: true,
                selectedValue: uc.settings.meeting.timeZone.name
            });

            this.wholeDayCtr = new uc.modules.component.SwitchButton({
                container: this.wholeDayObj,
                onChanged: function (value) {

                }
            });
            this.cloudMeetingCtr = new uc.modules.component.SwitchButton({
                container: this.cloudMettingObj,
                disabled: !this._isCloudMeetingEnabled(),
                onDisabledBtnClicked: function () {
                    _this._onDisabledBtnClicked();
                },
                onChanged: function (value) {

                }
            });

            this.cloudMeetingCtr.setValue(this._isCloudMeetingEnabled());

            this.isRepeatsCtr = new uc.modules.component.DropdownList(this.isRepeatsObj, {
                width: 100,
                navigateButtons: true,
                items: uc_resource.Meeting.repeatCycleItem,
                selectedByText: true,
                selectedValue: uc_resource.Meeting.repeatCycleItem[0].value
            });

            this.startTimeDatePicker = new uc.modules.component.PickADate(this.startTimeNode, true, true, function (date) {
                    if (date != null) {
                        _this._onStartTimeDatePickerTimeChanged(date);
                    }
                }
            );

            this.durationCtr = new uc.modules.component.MeetingDurationPicker({
                parentNode: this.durationNode,
                onItemSelected: function (selectedItem) {
                    uc.util.LogUtil.info('MeetingSchedule', 'MeetingDurationPicker', 'SelectedItem = ', selectedItem);
                    _this._onDurationPickerValueChanged(selectedItem);
                }
            });

            uc.modules.component.SearchBox.init(this.searchAttendeeInput, {
                count: 4,
                selectedUser: function(user){
                    _this.onSearchedUserSelected(user);
                }
            });
        },

        _isCloudMeetingEnabled: function () {
            var capability = JSON.parse(uc.util.StorageUtil.getData("capability"));
            return capability && capability.cloud_meeting == "1";
        },

        _onDisabledBtnClicked: function () {
            uc.modules.component.PromptMessageBox.open({
                title: uc_resource.WindowTitle.Notification,
                message: uc_resource.Meeting.NoCloudMeetingPrivilege,
                buttonMode: "ok",
                OkText: uc_resource.Common.OK
            }, function () {
                this.close();
            });
        },

        _initScheduleMeetingUI: function () {
            if (this.options.shareUserId && this.loginUserInfo.userId != this.options.shareUserId) {
                this.hostId = this.options.shareUserId;
            }else {
                this.hostId = this.loginUserInfo.userId;
            }

            var hostContactInfo = uc.IContactService.getBasicContactInfo(this.hostId);
            if(hostContactInfo){
                this.hostName = hostContactInfo.displayName;
            }

            this.attendeeIds.push(this.hostId);
            this.meetingTileInput.val(uc.util.StringUtil.formatString(uc_resource.Meeting.OthersMeeting, [this.hostName]));
            this.submitBtn.removeClass('disabled');
            this._showTitleLength(uc.util.StringUtil.formatString(uc_resource.Meeting.OthersMeeting, [this.hostName]).length);
            
            // this.showPosition();
            this.windowTitle.text(uc_resource.WindowTitle.CreateMeeting);

            if (this.options.startTime) {
                var startTime = uc.util.TimeUtil.UnixTimeToDisplayDateInfo(this.options.startTime, uc.settings.meeting.timeZone.offset, uc.constants.UI.TimeStampUnit.Second);
                var startDate = new Date(startTime.year, startTime.month - 1, startTime.day, startTime.hour, startTime.minute, startTime.second);
                this.startTimeDatePicker.setDate(startDate, false);
            }

            var attendees = [];
            if(this.options.users && this.options.users.length){
                this.options.users.map(function (user) {
                    attendees.push({
                        account: user,
                        type: uc.constants.Meeting.AttendeeType.User,
                        displayName: ''
                    })
                });
            }

            this._initMeetingAttendees(attendees, false);
        },

        _initUpdateMeetingUI: function () {
            var meetingDetail = this.options.meetingInfo;
            if (!meetingDetail) {
                return;
            }
            
            this.hostId = meetingDetail.hostId;
            var hostContactInfo = uc.IContactService.getBasicContactInfo(this.hostId);
            if(hostContactInfo){
                this.hostName = hostContactInfo.displayName;
            }

            this.attendeeIds.push(this.hostId);
            
            this.meetingTitleLengthNode.text((meetingDetail.title || "").length);
            this.meetingTileInput.val(meetingDetail.title);

            if (meetingDetail.duration != null) {
                this.durationCtr.setValue(meetingDetail.duration);
            }

            this.cloudMeetingCtr.setValue(meetingDetail.isGNet === 1);
            this.summaryNode.val(meetingDetail.summary);
            //set btn status
            this.windowTitle.text(uc_resource.Meeting.UpdateMeeting);
            this.submitBtn.removeClass('disabled').text(uc_resource.Meeting.UpdateMeeting);

            if (!meetingDetail.timeZone) {
                meetingDetail.timeZone = uc.settings.meeting.timeZone.offset;
            }
            var startTime = uc.util.TimeUtil.UnixTimeToDisplayDateInfo(meetingDetail.startTime, meetingDetail.timeZone, uc.constants.UI.TimeStampUnit.Second),
                startDate = new Date(startTime.year, startTime.month - 1, startTime.day, startTime.hour, startTime.minute, startTime.second);
            this.startTimeDatePicker.setDate(startDate, false);
            try {
                var meetingAddress = JSON.parse(meetingDetail.extrasInfo);
                if (meetingAddress.address.type == uc.constants.Meeting.AddressType.Company) {

                }else if (meetingAddress.address.type == uc.constants.Meeting.AddressType.Out) {
                    _this.meetingAddress.address.type = uc.constants.Meeting.AddressType.Out;
                    _this.meetingAddress.address.longitude = meetingAddress.address.longitude;
                    _this.meetingAddress.address.latitude = meetingAddress.address.latitude;
                    _this.meetingAddress.address.name = meetingAddress.address.name;
                    _this.meetingAddress.address.location = meetingAddress.address.location;

                    _this.longitude = _this.meetingAddress.address.longitude;
                    _this.latitude = _this.meetingAddress.address.latitude;
                }
                _this.addressObj.val(meetingAddress.address.name).attr("addr", meetingAddress.address.name);
            }catch (e) {

            }

            this._initMeetingAttendees(meetingDetail.attendees, true);
        },

        _initRoomMeetingUI: function () {
            this.meetingTileInput.val(uc.util.StringUtil.formatString(uc_resource.Meeting.OthersMeeting, [this.hostName]));
            this.submitBtn.removeClass('disabled');
            this.addressObj.parent().hide();
            this.cloudMettingObj.parent().hide();
            this.cloudMeetingCtr.setValue(true);
        },

        _initMeetingAttendees: function(attendees, isFromMeeting){
            this._showHostNode();
            
            if(attendees && attendees.length){
                var user, userIds = [];
                attendees.map(function(attendee){
                    if(isFromMeeting){
                        user = {
                            account: attendee.account,
                            displayName: attendee.type == uc.constants.Meeting.AttendeeType.User ? attendee.name : attendee.account,
                            type: attendee.type,
                            status: attendee.status
                        }
                    }else {
                        user = attendee;
                    }
                    if(parseInt(user.account) == _this.hostId){
                        return;
                    }
                    _this.attendees[user.account] = user;
                    _this.appendUser(user.account, user.displayName);
                    if(user.type == uc.constants.Meeting.AttendeeType.User){
                        userIds.push(parseInt(user.account));
                    }

                    if(user.type == uc.constants.Meeting.AttendeeType.User){
                        _this.attendeeIds.push(parseInt(user.account));
                    }
                });

                uc.IContactService.getBasicContactInfo(userIds);
            }
            this._getUserBusyFreeStatus(this.attendeeIds);
        },

        _showHostNode: function(){
            this.hostNode.attr("key", this.hostId).text(this.hostName ? this.hostName : this.hostId);
        },

        _onStartTimeDatePickerTimeChanged: function(){
            this._getUserBusyFreeStatus(this.attendeeIds);
        },

        _onDurationPickerValueChanged: function(){
            this._getUserBusyFreeStatus(this.attendeeIds);
        },

        onSearchedUserSelected: function(user){
            _this.ErrMsg.hide();
            if (user.userId) {
                if (this.attendeeIds.indexOf(user.userId) == -1) {
                    this.attendees[user.userId] = {
                        account: user.userId,
                        userId: user.userId,
                        displayName: user.displayName,
                        type: uc.constants.Meeting.AttendeeType.User
                    };
                    this.attendeeIds.push(user.userId);
                    this.appendUser(user.userId, user.displayName);
                    this._getUserBusyFreeStatus([user.userId]);
                }
            } else {
                if(!uc.util.StringUtil.CheckEmail(user.account,false))
                {
                    //showMsg;
                    _this.ErrMsg.show();
                    return;
                }
                if (!this.attendees[user.account]) {
                    this.attendees[user.account] = {
                        account: user.account,
                        type: uc.constants.Meeting.AttendeeType.Mail,
                        displayName: user.displayName
                    };
                    this.appendUser(user.account, user.displayName);
                }
            }
            this.searchAttendeeInput.val("");
            this.searchAttendeeInput.focus();
        },

        bindEvent: function () {
            //select search Meeting subject
            this.meetingTileInput.on('input', function () {
                var keyword = $(this).val().trim();
                _this._onMeetingTitleInput(keyword);
            });

            this.addressObj.one("focus", function () {
                _this.showPosition(null, _this.addressObj.val());
            });

            this.searchAttendeeInput.focus(function () {
                var attendees = _this.meetingRowObj.find("span").size();
                if (attendees <= 10) {
                    return;
                }
                _this.meetingRowObj.parent().scrollTop(_this.meetingRowObj.parent()[0].scrollHeight);
            }).keydown(function (e) {
                    _this.ErrMsg.hide();
                    if (e.keyCode == uc.constants.UI.KeyCode.Enter) {
                        if($('.userDropList-selected').get(0)!=null)
                        {
                            _this.ErrMsg.hide();  //search result exist
                        }
                        else {
                            if ($(this).val().trim() != "" && !uc.util.StringUtil.CheckEmail($(this).val(), false)) {
                                _this.ErrMsg.show();
                            }
                            else {
                                _this.ErrMsg.hide();
                            }
                        }
                    }
                _this.meetingRowObj.parent().scrollTop(_this.meetingRowObj.parent()[0].scrollHeight);
            });

            this.batchAddUserBtn.on('click', function () {
                _this._onBatchAddUserBtnClicked();
            });

            this.attendeeList.on('click', '.delete-icon', function () {
                var userId = $(this).parent().attr('key');
                _this._onDeleteAttendeeBtnClicked(userId);
                $(this).parent().remove();
            });

            this.viewAttendeeBusyStatusBtn.click(function(){
                _this._onViewAttendeeBusyStatusBtnClicked();
            });

            //save and  close window
            this.submitBtn.click(function () {
                if ($(this).hasClass('disabled')) {
                    return;
                }
                $(this).addClass('disabled');
                _this.saveMeeting();
            });

            this.cancelBtn.click(function () {
                uc.util.WindowUtil.close();
            });

            var activeObj = this.meetingRowObj.find('.'+this.activeDiv);
            $(document).on('click','.meeting-duration-container',function () {
                setActive($(this));
                _this.ActiveObj = $(this);
                _this.durationActive =true;
            });
            activeObj.find('input').click(function () {
                setActive($(this));
            });
            var setActive =function (clickObj) {
                var cssName ='.'+_this.activeDiv;
                clickObj.parents(cssName).addClass("selects");
            };

            activeObj.find('input').blur(function () {
                activeObj.removeClass("selects");
            });

            // close  time duration activeDiv
            $("body").click(function (e) {
                var cssName ='.'+_this.activeDiv;
                if(_this.durationActive) {
                    _this.ActiveObj.parents(cssName).removeClass("selects");
                    _this.durationActive =false;
                }
            });
        },

        _onMeetingTitleInput: function(keyword){
            if (keyword == "") {
                this.submitBtn.addClass('disabled');
            }else {
                uc.ISearchService.searchServer(keyword, 0, 5, [4], function (args) {
                    if (args[0] == 0) {
                        uc.util.LogUtil.info('MeetingScheduler', 'search', 'search count:', args[1]);
                        _this.SearchId = args[1];
                    }
                });
                this.submitBtn.removeClass('disabled');
            }

            this._showTitleLength(this.meetingTileInput.val().length);
        },

        _showTitleLength: function (length) {
            this.meetingTitleLengthNode.text(length);
        },

        _onDeleteAttendeeBtnClicked: function(userId){
            var index = this.attendeeIds.indexOf(parseInt(userId));
            if(index != -1){
                this.attendeeIds.splice(index, 1);
            }
            delete this.busyTimeUsers[userId];
            delete _this.attendees[userId];
        },

        _onViewAttendeeBusyStatusBtnClicked: function(){
            var meetingTime = this._getMeetingStartAndEndTime();
            var duration = this.durationCtr.getValue().value;
            var eventId = this.options.isCreate ? 0 : this.options.meetingInfo.eventId;
            var shareUserId = this.options.shareUserId ? this.options.shareUserId : 0;
            var data = {
                eventId: eventId,
                shareUserId: shareUserId,
                hostId: this.hostId,
                attendees: this.attendees,
                meetingTime: {
                    startTime: meetingTime.startTime,
                    duration: duration
                }
            };
            this._openAttendeeBusyStatusWindow(data);
        },

        _openAttendeeBusyStatusWindow: function(data){
            var uniqueName = "MeetingAttendeeBusyStatus";
            uc.util.StorageUtil.setData("Calendar:UniqueName", uniqueName);
            uc.util.StorageUtil.setData(uniqueName, data);

            if(!this.crossWindowEventBound){
                uc.util.EventUtil.bindCrossWindowEvent(uniqueName, function(args){
                    uc.util.StorageUtil.removeData("Calendar:UniqueName");
                    uc.util.StorageUtil.removeData(uniqueName);

                    uc.util.LogUtil.info('MeetingScheduler', '_openAttendeeBusyStatusWindow', 'AttendeeBusyStatusResult = ', args);

                    _this._onAttendeeBusyStatusWindowClosed(args);
                });

                this.crossWindowEventBound = true;
            }

            var options = {
                width: 1000,
                height: 550,
                refreshIfExist: false,
                centerScreen: true,
                modal: true,
                resizable: false,
                caption: uc.constants.UI.DragCaption.MeetingAttendeeList
            };

            uc.util.WindowUtil.open("page/meetingAttendeeBusyStatus.html", "MeetingAttendeeBusyStatus", options);
        },

        _onAttendeeBusyStatusWindowClosed: function(attendeeData){
            if(!attendeeData){
                return;
            }
            var duration = attendeeData.meetingTime.duration,
                meetingStartTime = attendeeData.meetingTime.startTime,
                attendees = attendeeData.attendees;
            this.durationCtr.setValue(duration);
            var timezone = this.timezoneCtr.getValue().value;
            var displayTime = uc.util.TimeUtil.UnixTimeToDisplayTime(meetingStartTime, timezone, uc.constants.UI.TimeStampUnit.Second);
            this.startTimeDatePicker.setDate(displayTime);
            this.attendees = {};
            this.attendeeIds = [this.hostId];
            this.busyTimeUsers = {};

            this.hostNode.removeClass('busyUser');
            this.attendeeList.find('.attendees').remove();
            this.attendeeBusyNode.hide();

            this._initMeetingAttendees(attendees, false);
        },

        _onBatchAddUserBtnClicked: function(){
            uc.IUIService.openChooseContactWindow({
                key: "MeetingCreator",
                fromForward: true
            }, function (args) {
                if (args.okOrCancel == 0) {
                    return;
                }
                _this._onUserBatchAdded(args);
            });
        },

        _onUserBatchAdded: function(args){
            uc.util.LogUtil.info('MeetingScheduler', '_onUserBatchAdded', 'args', {args: args});
            var user, userIds = [];
            for (var i = 0; i < args.contacts.length; i++) {
                var userId = args.contacts[i].userId;
                var displayName = args.contacts[i].displayName;
                if (userId) {
                    if (this.attendeeIds.indexOf(userId) != -1) {
                        continue;
                    }
                    user = {
                        account: userId,
                        displayName: displayName,
                        type: uc.constants.Meeting.AttendeeType.User
                    };
                    this.attendees[userId] = user;
                    this.appendUser(userId, displayName);
                    this.attendeeIds.push(userId);
                    userIds.push(userId);
                } else {
                    user = {
                        account: args.contacts[i].account,
                        displayName: displayName,
                        type: uc.constants.Meeting.AttendeeType.Mail
                    };
                    this.attendees[args.contacts[i].account] = user;
                    this.appendUser(userId, displayName);
                }
            }
            if(userIds.length){
                this._getUserBusyFreeStatus(userIds);
            }
            this.searchAttendeeInput.focus();
        },

        appendUser: function (key, displayName) {
            this.attendeeList.find('.addUser').before("<span class='attendees' key='" + key + "'>" +
                                                            "<span class='name'>" + displayName + "</span>" +
                                                            "<span class='delete-icon icon-close'></span>" +
                                                        "</span>");
        },

        saveMeeting: function () {
            var title = this.meetingTileInput.val().trim();
            var timezone = this.timezoneCtr.getValue().value;
            if (this.meetingAddress.address.name != this.addressObj.val().trim()) {
                if (this.meetingAddress.address.longitude == this.longitude && this.meetingAddress.address.latitude == this.latitude) {
                    this.meetingAddress.address.type = uc.constants.Meeting.AddressType.Company;
                    this.meetingAddress.address.name = this.addressObj.val().trim();
                    this.meetingAddress.address.location = this.addressObj.val().trim();
                }
            }
            var location = JSON.stringify(this.meetingAddress);//this.addressObj.val().trim();
            var isCloudMeeting = this.cloudMeetingCtr.getValue() ? 1 : 0;
            var summary = this.summaryNode.val().trim();
            var meetingDuration = parseInt(this.durationCtr.getValue().value);
            var TempTime = this.startTimeDatePicker.getDate();
            if (!TempTime) {
                this.confirm(uc_resource.Meeting.InputTime, function () {
                    _this.submitBtn.removeClass('disabled');
                });
                return;
            }

            var meetingDateTime = uc.util.TimeUtil.displayDateToUnixTime(timezone, TempTime.getFullYear(), TempTime.getMonth() + 1,
                TempTime.getDate(), TempTime.getHours(), TempTime.getMinutes(), 0, uc.constants.UI.TimeStampUnit.Second);
            if (this.options.isCreate &&  TempTime < uc.util.TimeUtil.localTimeToDisplayTime(new Date(), timezone)) {
                this.confirm(uc_resource.Meeting.GNetStartTime, function (result) {
                    if (result == 0) {
                        _this.startTimeDatePicker.setDate(uc.util.TimeUtil.getCurrentHalfOrWholeHourDisplayTime(timezone));
                    }
                    _this.submitBtn.removeClass('disabled');
                }, true);
                return;
            }

            var persons = [];
            $.each(this.attendees, function (key, attendance) {
                if (parseInt(key) != _this.hostId) {
                    persons.push({
                        account: (attendance.userId || attendance.account) + "",    // 账号
                        type: attendance.type,                                      // 参会人类型：1 内部用户， 2 邮件用户， 3电话用户
                        status: 0,                                                  // 参会人状态：0 未处理， 1 接受，  // 2 拒绝
                        //startTime: 12432143142123,                                  // 入会时间
                        name: attendance.displayName                                // 参会人名称
                    });
                }
            });

            if (!this.meetingAddress.address.name && !isCloudMeeting) {
                this.confirm(uc_resource.Meeting.CreateMeetingIllegal + "</br><span style='color:red;margin-top:5px;display: block;'>" + uc_resource.Meeting.NoInputLocation + "</span>",
                    function (r) {
                    if (r == 0) {
                        _this._submitMeeting(title, persons, location, summary, timezone, meetingDateTime, meetingDuration, isCloudMeeting);
                    }
                    _this.submitBtn.removeClass('disabled');
                }, true);
            }else {
                this._submitMeeting(title, persons, location, summary, timezone, meetingDateTime, meetingDuration, isCloudMeeting);
            }
        },

        _submitMeeting: function(title, persons, location, summary, timeZone, meetingStartTime, meetingDuration, isCloudMeeting){
            var oldlocation  =this.meetingAddress.address.location;
            var formData;
            if (this.options.isCreate) {
                formData = {
                    persons: persons,
                    title: title,
                    location: oldlocation,
                    extrasInfo:location,
                    summary: summary,
                    timezone: timeZone,
                    startTime: meetingStartTime,
                    minutes: meetingDuration,
                    isCloudMeeting: isCloudMeeting,
                    displayName: this.hostName,
                    shareUserId: this.options.shareUserId
                };

	            if (this.options.type == uc.constants.Meeting.MeetingCreateType.RoomMeeting) {
		            formData.roomId = this.options.roomId;
		            this.submitCreateMeetingInMeetingRoom(formData);
	            }else{
		            this.submitCreateMeeting(formData);
	            }
	            uc.util.LogUtil.info("MeetingScheduler", "_submitMeeting", "createMeeting", formData);

            }else {
                formData = {
                    eventId: this.options.meetingInfo.eventId,
                    persons: persons,
                    title: title,
                    location: oldlocation,
                    extrasInfo:location,
                    summary: summary,
                    timezone: timeZone,
                    startTime: meetingStartTime,
                    minutes: meetingDuration,
                    isCloudMeeting: isCloudMeeting,
                    shareUserId: this.options.shareUserId
                };

                uc.util.LogUtil.info("MeetingScheduler", "_submitMeeting", "updateMeeting", formData);
                this.submitEditMeeting(formData);
            }
            this._showLoading();
        },

        submitCreateMeeting: function (formData) {
            uc.IMeetingService.createMeeting(
                formData.persons, formData.title, formData.location, formData.summary, formData.timezone + "", formData.displayName,
                formData.extrasInfo,formData.startTime, formData.minutes, formData.shareUserId, {
                    repeatType: '',
                    property: '',
                    value: ''
                }, formData.isCloudMeeting, 0, 0, 1, 1, function (args) {
                    uc.util.LogUtil.info("MeetingScheduler", "submitCreateMeeting", "createMeeting", {errorCode: args[0]});
                    if (args[0] == 0) {
                        uc.IUIService.triggerCrossWindowEvent(_this.tirggerSource, {success: true});
                        //uc.util.WindowUtil.close();
                    } else {
                        _this._hideLoading();
                    }
                    _this.submitBtn.removeClass('disabled');
                });
        },

        submitEditMeeting: function (formData) {
            uc.IMeetingService.updateMeeting(
                formData.eventId, 0/*_data.detail.startTime*/,
                formData.persons, formData.title, formData.location, formData.summary, formData.timezone + "",
                formData.extrasInfo,formData.startTime, formData.minutes, formData.shareUserId, {
                    repeatType: '',
                    property: '',
                    value: ''
                },
                formData.isCloudMeeting, 0, 0, 1, 1, function (args) {
                    uc.util.LogUtil.info("MeetingScheduler", "submitEditMeeting", "updateMeeting", {errorCode: args[0]});
                    if (args[0] == 0) {
                        //uc.util.WindowUtil.close();
                    } else {
                        _this._hideLoading();
                    }
                    _this.submitBtn.removeClass('disabled');
                });
        },

	    submitCreateMeetingInMeetingRoom: function (formDate) {
		    uc.IMeetingRoomService.createMeetingInMeetingRoom(
			    formDate.persons, formDate.roomId, formDate.title, formDate.summary,
			    formDate.startTime, formDate.minutes,function (args) {
				    uc.util.LogUtil.info("MeetingScheduler", "submitCreateMeetingRoomMeeting", "submitCreateMeetingInMeetingRoom", {errorCode: args[0]});
				    if (args[0] == 0) {

				    } else {
					    _this._hideLoading();
				    }
				    _this.submitBtn.removeClass('disabled');
			    });
	    },

        confirm: function (msg, fn, hasCancel) {
            var cancelText = "";
            var modeTxt = "ok";
            if (hasCancel) {
                modeTxt = "both";
                cancelText = uc_resource.Common.Cancel;
            }
            uc.modules.component.PromptMessageBox.open({
                title: uc_resource.WindowTitle.Notification,
                message: msg,
                OkText: uc_resource.Common.OK,
                buttonMode: modeTxt,
                CancelText: cancelText
            }, function (r) {
                if (fn != null) {
                    fn(r);
                }
                this.close();
            });
        },

        showPosition: function (local, addr) {
            var search = _this.addressObj.attr("id");
            var ac = new BMap.Autocomplete({"input": search, "location": local});    //建立一个自动完成的对象
            var myValue;
            ac.addEventListener("onconfirm", function (e) {    //鼠标点击下拉列表后的事件
                var _value = e.item.value;
                myValue = _value.province + _value.city + _value.district + _value.street + _value.business;
                _this.meetingAddress.address.name = myValue;
                _this.meetingAddress.address.location = myValue;
                setPlace();
            });

            function setPlace() {
                var myGeo = new BMap.Geocoder();
                myGeo.getPoint(myValue, function (point) {
                    if (point) {
                        _this.meetingAddress.address.longitude = point.lng;
                        _this.meetingAddress.address.latitude = point.lat;
                        _this.meetingAddress.address.type = uc.constants.Meeting.AddressType.Out;
                    }
                }, local);
            }

            setTimeout(function () {
                _this.addressObj.val(addr);
            }, 200);
        },

        showSearchlayer: function (obj, data, option) {
            var html = '<div  class="layer"><div class="LayerItem"></div></div>';
            var l = $(obj).offset().left + option.left;
            var t = $(obj).offset().top;
            var w = $(obj).outerWidth();
            if ($(".layer").get(0) == undefined) {
                $(obj).parent().append(html);
            }
            $(".layer").css({position: 'absolute', zindex: 1, top: t, left: l, width: w, height: option.height});

        },

        OnServerSearchResultReceived: function (errorCode, searchId, meetings) {
            uc.util.LogUtil.info('MeetingScheduler', 'OnServerSearchResultReceived', 'result', {
                errorCode: errorCode,
                meetings: meetings
            });
            if (errorCode == uc.ErrorCode.Success && _this.SearchId == searchId) {
                //_this.showSearchlayer(_this.meetingTileInput ,meetings,{height:200,left:-15});
                //this.setSearchData(meetings);
            }
        },

        _getMeetingStartAndEndTime: function () {
            var timeZone = parseInt(_this.timezoneCtr.getValue().value);
            var selectedTime = _this.startTimeDatePicker.getDate();
            var startTime = uc.util.TimeUtil.displayTimeToUnixTime(selectedTime, timeZone, uc.constants.UI.TimeStampUnit.Second);
            var endTime = startTime + this.durationCtr.getValue().value * 60;
            return {
                startTime: startTime,
                endTime: endTime
            }
        },

        _getUserBusyFreeStatus: function (userIds) {
            var queryTime = this._getMeetingStartAndEndTime();
            uc.util.LogUtil.info("Meeting Schedule", "_getUserBusyFreeStatus", "queryData = ", {
                userIds : userIds,
                startTime: queryTime.startTime,
                endTime: queryTime.endTime
            });

            uc.IMeetingService.getUserBusyFreeStatusInfo(userIds, queryTime.startTime, queryTime.endTime, this.options.shareUserId, function (args) {
                if (args[0] == 0) {
                    _this.busyFreeStatusRequestId = args[1];
                }else {
                    uc.util.LogUtil.info("Meeting Schedule", "getUserBusyFreeStatusInfo", "fail to get user busy free status", args);
                }
            });
        },

        OnUserBusyFreeStatusInfoReceived: function (schedules) {
            this.hostNode.removeClass('busyUser');
            this.attendeeList.find('.attendees').removeClass('busyUser');
            this.attendeeBusyNode.hide();
            if (!schedules || schedules.length == 0) {
                _this._checkUserBusyStatus();
                return;
            }
            var updatingMeetingEventId = this.options.isCreate ? 0 : this.options.meetingInfo.eventId;
            var meetingInfos, busyTime, busyTimes;
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
                    _this.busyTimeUsers[userId] = busyTimes;
                }

            });
            _this._checkUserBusyStatus();
        },

        _checkUserBusyStatus: function(){
            var time = this._getMeetingStartAndEndTime(),
                meetingStartTime = time.startTime,
                meetingEndTime = time.endTime;
            var busyTimes, attendeesBusy = false, singleBusy;
            for(var userId in this.busyTimeUsers){
                singleBusy = false;
                busyTimes = this.busyTimeUsers[userId];
                busyTimes.map(function(busyTime){
                    if((meetingStartTime < busyTime.startTime && meetingEndTime > busyTime.startTime)
                        || (meetingStartTime < busyTime.endTime && meetingEndTime > busyTime.startTime)){
                        singleBusy = true;
                        attendeesBusy = true;
                    }
                });
                var attendeeNode = this.attendeeList.find('span[key="' + userId + '"]');
                if(singleBusy){
                    attendeeNode.addClass("busyUser");

                }else {
                    attendeeNode.removeClass("busyUser");
                }
            }

            if(attendeesBusy){
                this.attendeeBusyNode.show();
            }else {
                this.attendeeBusyNode.hide();
            }
        },

        _showLoading: function () {
            this.loadingNode.show();
            clearTimeout(this.loadingTimer);
            this.loadingTimer = setTimeout(function () {
                _this.loadingNode.hide();
                clearTimeout(_this.loadingTimer);
            }, 10000);
        },

        _hideLoading: function () {
            this.loadingNode.hide();
            clearTimeout(this.loadingTimer);
        },

        _showErrorMsg: function(errorCode){
            var errorName = "ERROR_CODE_" + errorCode;
            var errorMeg;

            errorMeg = uc_resource.Meeting.ErrorMessage[errorName] || uc_resource.Meeting.ErrorMessage["ERROR_DEFAULT"];
	        if(errorMeg == uc_resource.Meeting.ErrorMessage["ERROR_DEFAULT"]){
		        uc.util.LogUtil.warn("uc.modules.meeting.MeetingScheduler ", "_showErrorMsg", "ERROR_DEFAULT=",errorCode);
	        }
            uc.modules.component.PromptMessageBox.open({
                title: uc_resource.WindowTitle.Notification,
                message: errorMeg,
                buttonMode: "ok",
                OkText: uc_resource.Common.OK
            }, function () {
                _this.submitBtn.removeClass('disabled');
                this.close();
            });
        },

        OnMeetingCreated: function (/*int*/errorCode, conferenceInfo) {
            if (errorCode == uc.ErrorCode.Success) {
                uc.util.WindowUtil.close();
            }else {
                this._hideLoading();
                this._showErrorMsg(errorCode);
            }
        },

        OnMeetingUpdated: function (/*int*/errorCode, eventId, startTime, conferenceInfo) {
            if (errorCode == uc.ErrorCode.Success) {
                uc.util.WindowUtil.close();
            }else {
                this._hideLoading();
	           this._showErrorMsg(errorCode);
            }
        },

        OnBasicContactInfoReceived: function(contacts){
            if(contacts && contacts.length){
                contacts.map(function(contact){
                    if(!_this.hostName && contact.userId == _this.hostId){
                        _this.hostName = contact.displayName;
                        _this.attendeeList.find('.emcee[key="' + contact.userId + '"] .name').text(contact.displayName);
                    }
                    if(_this.attendees[contact.userId]){
                        _this.attendees[contact.userId].displayName = contact.displayName;
                    }
                    _this.attendeeList.find('.attendees[key="' + contact.userId + '"] .name').text(contact.displayName);
                })
            }
        },

	    OnMeetingCreatedInMeetingRoom: function (errorCode, roomId, meeting) {
		    if(errorCode == uc.ErrorCode.Success){
			    uc.IUIService.triggerCrossWindowEvent(this.tirggerSource, {success: true});
			    uc.util.WindowUtil.close();
		    }else{
			    this._hideLoading();
			    this._showErrorMsg(errorCode);
		    }
	    }
    };

    jQuery(document).ready(function(){
        uc.init();
        uc.modules.meeting.MeetingScheduler.init();
    })
})(uc);

