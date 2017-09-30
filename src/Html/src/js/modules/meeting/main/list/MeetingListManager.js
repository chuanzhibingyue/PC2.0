/**
 * Created by waouyang on 16/4/26.
 */
(function(uc){
    var _this = null;
    uc.modules.meeting.MeetingListManager = {
        authorizerId: 0, //current authorizer id
        selectedDate: 0, //current selected date in calendar date picker
        calendarFirstDate: 0, //first date in Calendar Picker
        calendarLastDate: 0,  //last date in Calendar Picker
        hasMeetingDays: {},//dates
        sortedMeetingList: [],
        meetingDataSet: null,
        meetingList: null,
        notifyDataChangedTimer: -1,
        notifyDataChangedDelay: 50,

        init: function(clearData){
            _this = this;
            var currentTime = uc.util.TimeUtil.localTimeToDisplayTime(new Date(), uc.settings.meeting.timeZone.offset);
            this.selectedDate = uc.util.TimeUtil.displayTimeToDayBeginningDisplayTime(currentTime);

            if(!clearData){
                this.meetingDataSet = new uc.modules.meeting.MeetingDataSet();
                this.initMeetingList();
            }else {
                this.meetingDataSet.clear();
                this.meetingList.clear();
                uc.IUIService.showMeetingDetail(null, false);
            }
        },

        initMeetingList: function(){
            this.meetingListAdapter = new uc.modules.meeting.MeetingListAdapter(this);
            this.meetingListAdapter.init();
            this.meetingList = new uc.modules.meeting.MeetingList(this.meetingListAdapter);
            this.meetingList.init();
        },

        /**
         * get one day's meeting list, for example 2016.04.28 00:00:00
         * @param time
         */
        getOneDayMeetingList: function(time){
            var startTime = uc.util.TimeUtil.displayTimeToDayBeginningUnixTime(time, uc.settings.meeting.timeZone.offset, uc.constants.UI.TimeStampUnit.Second);
            var endTime = startTime + 86400;

            uc.util.LogUtil.info('MeetingListManager', 'getOneDayMeetingList', 'queryData = ', {
                startTime: startTime,
                endTime: endTime,
                authorizerId: this.authorizerId
            });
            var meetings = uc.IMeetingService.getMeetingListSync(startTime, endTime, 200, this.authorizerId, 0, true, 0);
            uc.util.LogUtil.info('MeetingListManager', 'getOneDayMeetingList', 'meetings = ', meetings);
            this.updateMeetingList(meetings);
        },

        sortMeetingList: function(){
            var allMeetingInfo = this.meetingDataSet.getAllMeetingInfo();
            this.sortedMeetingList = allMeetingInfo.sort(this.compareFunction);
        },

        compareFunction: function(meetingInfo1, meetingInfo2){
            return meetingInfo1.startTime - meetingInfo2.startTime;
        },

        getSortedMeetingList: function(){
            return this.sortedMeetingList;
        },

        addMeetingInfo: function(meetingInfo){
            if(!meetingInfo){
                return;
            }
            if(meetingInfo.authorizerId == this.authorizerId){
                if(this.meetingDataSet.isMeetingInfoExist(meetingInfo.eventId)){
                    this.updateMeetingInfo(meetingInfo);
                }else {
                    this.meetingDataSet.addMeetingInfo(meetingInfo);
                    this.notifyDataSetChanged();
                }
            }
        },

        updateMeetingInfo: function(meetingInfo){
            if(!meetingInfo){
                return;
            }
            if(meetingInfo.authorizerId == this.authorizerId){
                if(this.meetingDataSet.isMeetingInfoExist(meetingInfo.eventId)){
                    this.meetingDataSet.updateMeetingInfo(meetingInfo);
                    this.notifyDataSetChanged();
                }else {
                    this.addMeetingInfo(meetingInfo);
                }
            }
        },

        removeMeetingInfo: function(eventId){
            if(this.meetingDataSet.isMeetingInfoExist(eventId)){
                this.meetingDataSet.removeMeetingInfo(eventId);
                this.notifyDataSetChanged();
            }
        },

        updateMeetingList: function(meetings){
            this.meetingDataSet.clear();
            if(meetings && meetings.length){
                meetings.map(function(meetingInfo){
                    if(_this._isMeetingInSelectedDate(meetingInfo.startTime, meetingInfo.duration)){
                        _this.addMeetingInfo(meetingInfo);
                    }
                })
            }
            this.notifyDataSetChanged();
        },

        notifyDataSetChanged: function(){
            if(!this.meetingListAdapter){
                return;
            }
            clearTimeout(this.notifyDataChangedTimer);
            this.notifyDataChangedTimer = setTimeout(function(){
                _this.sortMeetingList();
                _this.meetingListAdapter.notifyDataSetChanged();
            }, this.notifyDataChangedDelay);
        },

        _isMeetingInSelectedDate: function(meetingTime, duration){
            var selectedEndTime = this.selectedDate.addDays(1);
            var meetingStartTime = new Date(meetingTime * 1000);
            var meetingEndTime = new Date(meetingTime * 1000 + duration * 60 * 1000);
            return meetingStartTime >= this.selectedDate && meetingStartTime < selectedEndTime
                || meetingStartTime < this.selectedDate && meetingEndTime > this.selectedDate;
        },

        getAuthorizerId: function(){
            return this.authorizerId;
        },

        getBriefMeetingList: function(){
            var starTime = uc.util.TimeUtil.displayTimeToDayBeginningUnixTime(this.calendarFirstDate, uc.settings.meeting.timeZone.offset, uc.constants.UI.TimeStampUnit.Second);
            var endTime = uc.util.TimeUtil.displayTimeToDayEndingUnixTime(this.calendarLastDate, uc.settings.meeting.timeZone.offset, uc.constants.UI.TimeStampUnit.Second);

            uc.util.LogUtil.info('MeetingListManager', 'getBriefMeetingList', 'queryData = ', {
                startTime: starTime,
                endTime: endTime
            });

            uc.IMeetingService.getBriefMeetingList(starTime, endTime, this.authorizerId,function(args){
            });
        },

        onDateSelected: function(selectedDate){
            uc.util.LogUtil.info('MeetingListManager', 'onDateSelected', 'selectedDate = ', selectedDate);

            this.selectedDate = selectedDate;
        },

        onDateRangeChanged: function(firstDate, lastDate){
            uc.util.LogUtil.info('MeetingListManager', 'onDateRangeChanged', 'dateRange = ', {
                firstDate: firstDate,
                lastDate: lastDate
            });

            this.calendarFirstDate = firstDate;
            this.calendarLastDate = lastDate;

            this.getBriefMeetingList();

            this.hasMeetingDays = {};
            var tempDate = firstDate;

            while (tempDate <= lastDate){
                this.hasMeetingDays[this._formatDate(tempDate, true)] = [];
                tempDate = tempDate.addDays(1);
            }

            this.showMeetingMarks();
        },

        showMeetingMarks: function(){
            this.meetingList.showMeetingMarks(this.hasMeetingDays);
        },

        updateHasMeetingDays: function(meetings){
            if(meetings && meetings.length){
                var date;
                meetings.map(function(meeting){
                    date = _this._formatDate(meeting.startTime, false);
                    if(_this.hasMeetingDays[date]){
                        _this.hasMeetingDays[date].push(meeting.eventId);
                    }
                })
            }
            this.showMeetingMarks();
        },

        addMeetingToMeetingDays: function(meetingInfo){
            var date = this._formatDate(meetingInfo.startTime, false);
            if(this.hasMeetingDays[date] && this.hasMeetingDays[date].indexOf(meetingInfo.eventId) == -1){
                this.hasMeetingDays[date].push(meetingInfo.eventId);
                this.showMeetingMarks();
            }
        },

        removeMeetingFromMeetingDays: function(eventId){
            var eventIdArray, index;
            for(var date in this.hasMeetingDays){
                eventIdArray = this.hasMeetingDays[date];
                if(eventIdArray && (index = eventIdArray.indexOf(eventId)) != -1){
                    eventIdArray.splice(index, 1);
                    this.showMeetingMarks();
                }
            }
        },

        /**
         * format date style to "20160428"
         * @param date
         * @param isDisplayTime
         * @returns {*}
         * @private
         */
        _formatDate: function(date, isDisplayTime){
            if(!isDisplayTime){
                date = uc.util.TimeUtil.UnixTimeToDisplayTime(date, uc.settings.meeting.timeZone.offset, uc.constants.UI.TimeStampUnit.Second);
            }

            return '' + (date.getFullYear() * 10000 + (date.getMonth() + 1) * 100 + date.getDate());
        },

        onAuthorizerUserIdChanged: function(authorizerId){
            uc.util.LogUtil.info('MeetingListManager', 'onAuthorizerUserIdChanged', 'authorizerId = ' + authorizerId);
            this.authorizerId = authorizerId;
            this.meetingList.onAuthorizerUserIdChanged();
            uc.IUIService.showMeetingDetail(null);
        },

        OnInitClient: function (clearData) {
            uc.util.LogUtil.info('MeetingListManager', 'OnInitClient', 'clearData = ' + clearData);
            this.init(clearData);
        },

        OnModuleChanged: function (moduleId, mountId, dataObj) {
            this.meetingList.OnModuleChanged(moduleId, mountId, dataObj);
        },

        OnCreateMeeting: function (uniqueName, data) {
            uc.util.StorageUtil.setData(uniqueName, $.extend({
                shareUserId: this.authorizerId
            }, data));

            var options = {
                width: 860/2,
                height: 580,
                refreshIfExist: false,
                centerScreen: true,
                modal: false,
                isChild: true,
                resizable: false,
                caption: uc.constants.UI.DragCaption.CreateMeeting
            };

            uc.util.WindowUtil.open("page/meetingScheduler.html", "MeetingEdit", options);
        },

        OnDateChanged:function(){
            this.meetingList.OnDateChanged();
        },

        OnShowMeetingDetail: function (meetingInfo, isFromMeetingList) {
            if(!meetingInfo){
                return;
            }
            if(!isFromMeetingList){
                uc.IUIService.changeModule(uc.constants.UI.ModuleId.Meeting, '');
            }

            if(meetingInfo.authorizerId != this.authorizerId){
                uc.modules.meeting.MeetingHeaderManager.switchAuthorizeUser(meetingInfo.authorizerId);
                this.authorizerId = meetingInfo.authorizerId;
            }

            this.meetingList.OnShowMeetingDetail(meetingInfo, isFromMeetingList);
        },

        OnMeetingCreated: function(errorCode, meetingInfo){
            if(errorCode == uc.ErrorCode.Success && meetingInfo && this.authorizerId == meetingInfo.authorizerId){
                if(this._isMeetingInSelectedDate(meetingInfo.startTime)){
                    this.addMeetingInfo(meetingInfo);
                }

                this.addMeetingToMeetingDays(meetingInfo);

                //show meeting detail
                uc.IUIService.showMeetingDetail(meetingInfo, false);
            }
        },

        OnMeetingUpdated: function (errorCode, eventId, startTime, meetingInfo) {
            if (errorCode == uc.ErrorCode.Success && meetingInfo && this.authorizerId == meetingInfo.authorizerId) {
                if(this._isMeetingInSelectedDate(meetingInfo.startTime)){
                    this.updateMeetingInfo(meetingInfo);
                    this.addMeetingToMeetingDays(meetingInfo);
                }else {
                    _this.removeMeetingInfo(meetingInfo.eventId);
                    _this.removeMeetingFromMeetingDays(meetingInfo.eventId);
                    _this.addMeetingToMeetingDays(meetingInfo);
                }

                //show meeting detail
                uc.IUIService.showMeetingDetail(meetingInfo, false);
            }
        },
        OnMeetingCanceled: function (/*int*/errorCode, eventId, startTime, authorizerId) {
            if (errorCode == uc.ErrorCode.Success && this.authorizerId == authorizerId) {
                this.removeMeetingInfo(eventId);
                this.removeMeetingFromMeetingDays(eventId);
                uc.IUIService.showMeetingDetail(null);//取消会议后，恢复meetingDetail， 有可能需要选中下一场会议，待定
            }
        },

        OnMeetingInvitationRejected: function (errorCode, eventId, startTime, authorizerId) {
            if (errorCode == uc.ErrorCode.Success && this.authorizerId == authorizerId) {
                this.removeMeetingInfo(eventId);
                this.removeMeetingFromMeetingDays(eventId);
            }
        },

        OnMeetingInvitationAccepted: function (errorCode, eventId, startTime, authorizerId) {
            if (errorCode == uc.ErrorCode.Success && this.authorizerId == authorizerId && this._isMeetingInSelectedDate(startTime)) {
                var meetingInfos = uc.IMeetingService.getMeetingInfoByIdSync(eventId, startTime, 0, authorizerId);
                if(meetingInfos && meetingInfos.length){
                    meetingInfos.map(function(meetingInfo){
                        _this.updateMeetingInfo(meetingInfo);
                    })
                }
            }
        },

        OnMeetingForwarded: function (errorCode, meetingInfo) {
            if (errorCode == uc.ErrorCode.Success && meetingInfo && this.authorizerId == meetingInfo.authorizerId
                && this._isMeetingInSelectedDate(meetingInfo.startTime)) {
                this.updateMeetingInfo(meetingInfo);
            }
        },

        OnMeetingListReceived: function(errorCode,requestId,startTime,endTime,eventIdFilter,authorizerId,meetings){
            uc.util.LogUtil.info('MeetingListManager', 'onDateRangeChanged', 'param = ', {
                errorCode: errorCode,
                requestId: requestId,
                startTime: startTime,
                endTime: endTime,
                authorizerId: authorizerId,
                meetings: meetings
            });

            if(errorCode == uc.ErrorCode.Success && this.authorizerId == authorizerId){
                this.updateMeetingList(meetings);
            }
        },

        OnMeetingInviteMessageReceived: function (channel, from, to, meetingId, eventId, meetingName, accreditUserId, hostId, isRecurrent, meetingStartTime, msgId, msgSeq, msgTime) {
            if (channel !== uc.constants.Message.MessageChannel.UCAS) {
                return;
            }

            var meetings = uc.IMeetingService.getMeetingInfoByIdSync(eventId, meetingStartTime, 0, accreditUserId);

            if (meetings && meetings.length > 0) {
                meetings.map(function(meetingInfo){
                    if(_this._isMeetingInSelectedDate(meetingInfo.startTime)){
                        _this.addMeetingInfo(meetingInfo);
                    }
                    _this.addMeetingToMeetingDays(meetingInfo);
                })
            }
        },

        OnMeetingAcceptMessageReceived: function (channel, from, to, meetingId, eventId, meetingName, accreditUserId, hostId, isRecurrent, meetingStartTime, msgId, msgSeq, msgTime) {
            if (channel !== uc.constants.Message.MessageChannel.UCAS) {
                return;
            }
            var meetings = uc.IMeetingService.getMeetingInfoByIdSync(eventId, meetingStartTime, 0, accreditUserId);

            if (meetings && meetings.length > 0) {
                meetings.map(function(meetingInfo){
                    if(_this._isMeetingInSelectedDate(meetingInfo.startTime)){
                        _this.updateMeetingInfo(meetingInfo);
                    }
                    _this.addMeetingToMeetingDays(meetingInfo);
                })
            }
        },

        OnMeetingRejectMessageReceived: function (channel, from, to, meetingId, eventId, meetingName, accreditUserId, hostId, isRecurrent, meetingStartTime, msgId, msgSeq, msgTime) {
            if (channel !== uc.constants.Message.MessageChannel.UCAS) {
                return;
            }
            var meetings = uc.IMeetingService.getMeetingInfoByIdSync(eventId, meetingStartTime, 0, accreditUserId);

            if (meetings && meetings.length > 0) {
                meetings.map(function(meetingInfo){
                    if(from.userId == uc.IContactService.getCurrentUserInfo().userId){
                        if(_this._isMeetingInSelectedDate(meetingInfo.startTime)){
                            _this.removeMeetingInfo(meetingInfo.eventId);
                        }
                    }else {
                        if(_this._isMeetingInSelectedDate(meetingInfo.startTime)){
                            _this.updateMeetingInfo(meetingInfo);
                        }
                    }
                })
            }
        },

        OnMeetingUpdateMessageReceived: function (channel, from, to, meetingId, eventId, meetingName, accreditUserId, hostId, isRecurrent, meetingStartTime, updateFlag, msgId, msgSeq, msgTime) {
            if (channel !== uc.constants.Message.MessageChannel.UCAS) {
                return;
            }
            var meetings = uc.IMeetingService.getMeetingInfoByIdSync(eventId, meetingStartTime, 0, accreditUserId);

            if (meetings && meetings.length > 0) {
                meetings.map(function(meetingInfo){
                    if(_this._isMeetingInSelectedDate(meetingInfo.startTime)){
                        _this.updateMeetingInfo(meetingInfo);
                        _this.addMeetingToMeetingDays(meetingInfo);
                    }else {
                        _this.removeMeetingInfo(meetingInfo.eventId);
                        _this.removeMeetingFromMeetingDays(meetingInfo.eventId);
                        _this.addMeetingToMeetingDays(meetingInfo);
                    }
                })
            }

            // this.getBriefMeetingList()
        },

        OnMeetingForwardMessageReceived: function (channel, from, to, meetingId, eventId, meetingName, accreditUserId, hostId, isRecurrent, meetingStartTime, msgId, msgSeq, msgTime) {
            if (channel !== uc.constants.Message.MessageChannel.UCAS) {
                return;
            }
            var meetings = uc.IMeetingService.getMeetingInfoByIdSync(eventId, meetingStartTime, 0, accreditUserId);

            if (meetings && meetings.length > 0) {
                meetings.map(function(meetingInfo){
                    if(_this._isMeetingInSelectedDate(meetingInfo.startTime)){
                        _this.updateMeetingInfo(meetingInfo);
                    }
                    _this.addMeetingToMeetingDays(meetingInfo);
                })
            }
        },

        OnMeetingCancelMessageReceived: function (channel, from, to, meetingId, eventId, meetingName, accreditUserId, hostId, isRecurrent, meetingStartTime, msgId, msgSeq, msgTime) {
            if (channel !== uc.constants.Message.MessageChannel.UCAS) {
                return;
            }

            this.removeMeetingInfo(eventId);

            this.removeMeetingFromMeetingDays(eventId);
        },

        OnQueryMeetingInCalendar: function (meeting) {
            uc.util.LogUtil.info('MeetingListManager', 'onDateRangeChanged', 'meeting = ', meeting);
            uc.IUIService.changeModule(uc.constants.UI.ModuleId.Meeting);

            if (meeting.authorizerId != 0 && !uc.modules.meeting.MeetingHeaderManager.isAuthorizeToMeUserExist(meeting.authorizerId)) {
                uc.modules.component.PromptMessageBox.open({
                    title: uc_resource.WindowTitle.Notification,
                    message: uc_resource.Meeting.NoRightCheckMeeting,
                    buttonMode: "ok",
                    OkText: uc_resource.Common.OK
                }, function (result) {
                    this.close();
                });
                return;
            }
            var meetings = uc.IMeetingService.getMeetingInfoByIdSync(meeting.eventId, 0, 0, 0);
            if(meetings && meetings.length){
                uc.IUIService.showMeetingDetail(meetings[0], false);
            }else {
                uc.IUIService.showMeetingDetail(meeting, false);
            }
        },

        OnBriefMeetingListReceived: function (errorCode, requestId, startTime, endTime, authorizerId, meetings) {
            if (errorCode == uc.ErrorCode.Success && this.authorizerId == authorizerId) {
                this.updateHasMeetingDays(meetings);
            }
        },

        OnBriefChangedMeetingListReceived: function (errorCode, actionTime, authorizerId, meetings) {
            if (errorCode == uc.ErrorCode.Success) {
                this.meetingList.OnBriefChangedMeetingListReceived(errorCode, actionTime, authorizerId, meetings);
            }
        },

        OnMeetingInfoReceivedById: function (errorCode, eventId, authorizerId, meetings) {
            if (errorCode == uc.ErrorCode.Success) {
                if(meetings && meetings.length){
                    meetings.map(function(meetingInfo){
                        if(_this.meetingDataSet.isMeetingInfoExist(meetingInfo.eventId)){
                            _this.updateMeetingInfo(meetingInfo);
                        }
                    })
                }
            }
        },

        OnMeetingInfoReceivedByIds: function (errorCode, requestId, authorizerId, meetings) {
            if (errorCode == uc.ErrorCode.Success) {
                if(meetings && meetings.length){
                    meetings.map(function(meetingInfo){
                        if(_this.meetingDataSet.isMeetingInfoExist(meetingInfo.eventId)){
                            _this.updateMeetingInfo(meetingInfo);
                        }
                    })
                }
            }
        }
    };
})(uc);