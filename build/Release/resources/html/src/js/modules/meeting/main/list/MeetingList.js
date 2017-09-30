/**
 * Created by waouyang on 16/4/26.
 */
(function(uc){
    var _this = null;
    uc.modules.meeting.MeetingList = function(meetingListAdapter){
        this.nodeHtml = "<div class='meeting-list-container'>" +
                        "   <div class='schedule-time-too-early-warning'>"+ uc_resource.Meeting.ScheduleTimeTooEarly +"</div>" +
                        "   <div class='calendar-panel'></div>" +
                        "   <div class='list-panel'>" +
                        "       <div class='meeting-list'>" +
                        "           <div class='current-time-indicator'></div>"+
                        "       </div>" +
                        "   </div>" +
                        "   <div class='add-meeting'>"+
                        "       <i class='addMeeting icon-createMetting' qtips='"+ uc_resource.Meeting.NewMeeting +"'></i>" +
                        "   </div>"+
                        "</div>";

        this.meetingListAdapter = meetingListAdapter;
        this.meetingListAdapter.setDataSetObserver(this);
        this.itemViewArray = [];
        this.timeZone = uc.settings.meeting.timeZone.offset;
        this.gridHeight = 40;
        this.defaultItemViewWidth = 234;
        this.defaultItemViewLeft = 60;
        this.isFromMeetingList = true;
        this.clearWarningTimer = -1;
    };

    jQuery.extend(uc.modules.meeting.MeetingList.prototype, {
        init: function(){
            _this = this;

            this._initUI();
            this.bindEvent();
            this._setCapability();
        },

        _initUI: function(){
            this.container = $(this.nodeHtml).appendTo(uc.ui.Meeting.MenuPanel);
            this.calendarNode = $('.calendar-panel', this.container);
            this.scheduleTimeTooEarlyWarningNode = $('.schedule-time-too-early-warning', this.container);
            this.meetingListPanel = $('.list-panel', this.controller);
            this.meetingList = $('.meeting-list', this.container);
            this.addMeetingNode = $('.add-meeting', this.container);
            this.currentTimeIndicator = $('.current-time-indicator', this.container);

            this.calendarDatePicker = new uc.modules.component.CalendarDatePicker();
            this.calendarDatePicker.init({
                containerId: this.calendarNode,
                timezone: this.timeZone,
                dateMode: uc.constants.UI.DatePicker.DateMode.TwoWeek,
                activeFirstDay: true,
                onDateSelected: function(args){
                    _this._onDateSelected(args);
                },
                onDateRangeChanged: function(args){
                    _this._onDateRangeChanged(args);
                }
            });

            this._initMeetingListGrid();
            this._initMeetingListItemView();
            this._showMeetingListItemView();
        },

        _setCapability: function(){
            if (uc.constants.Switch.OpenMultiVersion) {
                var version = JSON.parse(uc.util.StorageUtil.getData("capability"));
                if (version) {
                    if(version.conf == "0"){
                        this.addMeetingNode.addClass('hide');
                    }
                }
            }
        },

        bindEvent: function(){
            this.addMeetingNode.click(function(){
                _this._scheduleMeeting();
            });

            this.meetingList.on('dblclick', '.meeting-list-grid-item', function(){
                var hour = parseInt($(this).attr('hour'));
                _this._onMeetingListGridItemDblClicked(hour, $(this));
            });

            this.meetingList.on('contextmenu', '.meeting-list-grid-item', function(e){
                var hour = parseInt($(this).attr('hour'));
                _this._onMeetingListGridItemRightClicked(e, hour);
            });

            this.meetingList.on('click', '.meeting-list-grid-time', function (e) {
                e.stopPropagation();
            });

            this.meetingList.on('dblclick', '.meeting-list-grid-time', function (e) {
                e.stopPropagation();
            });
        },

        _initMeetingListGrid: function(){
            var gridNodeStr, timeStr;
            for(var i = 0; i < 24; i++){
                timeStr = i < 10 ? '0' + i : i ;
                gridNodeStr = '<div class="meeting-list-grid-item" hour="' + i + '"><span class="meeting-list-grid-time"><span class="hour">' + timeStr + '</span>:00</span></div>';
                this.meetingList.append(gridNodeStr);
            }
        },

        _initMeetingListItemView: function(){
            this.meetingCount = this.meetingListAdapter.getCount();
            var viewCount = this.itemViewArray.length;
            var itemView;
            if(viewCount < this.meetingCount){
                for(var i = 0; i < this.meetingCount - viewCount; i++){
                    itemView = new uc.modules.meeting.MeetingListItemView();
                    itemView.init({parentNode: this.meetingList});
                    this.itemViewArray.push(itemView);
                }
            }
        },

        _showMeetingListItemView: function(){
            this._calcMeetingPosition();
            var i, viewCount, itemView, meetingInfo, selectedMeetingTime = 0;

            for(i = this.meetingCount, viewCount = this.itemViewArray.length; i < viewCount; i++){
                this.itemViewArray[i].hide();
            }

            for(i = 0; i < this.meetingCount; i++){
                itemView = this.itemViewArray[i];
                itemView.show();
                itemView.removeActiveStatus();
                meetingInfo = this.meetingListAdapter.getMeetingInfo(i);
                itemView.setMeetingInfo(meetingInfo);

                if(this.toShowMeetingInfo && meetingInfo.eventId == this.toShowMeetingInfo.eventId){
                    itemView.setMeetingSelected();
                    selectedMeetingTime = meetingInfo.startTime;
                }
            }
            
            this._scrollMeetingList(selectedMeetingTime);
        },

        _calcMeetingPosition: function(){
            if(this.meetingCount == 0){
                return;
            }
            var meetingInfo, startTime, duration, tempEndPosition = 0, lastMeetingEndPosition = 0, overlapMeetingIndex = [];
            for(var i = 0; i < this.meetingCount; i++){
                meetingInfo = this.meetingListAdapter.getMeetingInfo(i);
                startTime = this._getMeetingStartTimeMinutes(meetingInfo.startTime);
                duration = meetingInfo.duration;

                meetingInfo.top = startTime * this.gridHeight / 60;
                meetingInfo.height = duration * this.gridHeight / 60 - 1;

                meetingInfo.height = meetingInfo.height < 20 ? 20 : meetingInfo.height;
                meetingInfo.isMinHeight = meetingInfo.height < this.gridHeight;

                if(this.meetingCount == 1){
                    meetingInfo.left = this.defaultItemViewLeft;
                    meetingInfo.width = this.defaultItemViewWidth - 3;
                    if(meetingInfo.top + meetingInfo.height > this.gridHeight * 24){
                        meetingInfo.height = this.gridHeight * 24 - meetingInfo.top;
                    }

                }else {
                    if(i == 0){
                        overlapMeetingIndex.push(i);
                    } else {
                        if(meetingInfo.top < lastMeetingEndPosition){
                            overlapMeetingIndex.push(i);
                        }else {
                            this._calcLeftAndWidth(overlapMeetingIndex);

                            overlapMeetingIndex = [i];
                        }
                    }
                    tempEndPosition = meetingInfo.top + meetingInfo.height;
                    if(tempEndPosition > lastMeetingEndPosition){
                        lastMeetingEndPosition = tempEndPosition;
                    }
                    if(lastMeetingEndPosition > this.gridHeight * 24){
                        lastMeetingEndPosition = this.gridHeight * 24;
                        meetingInfo.height = lastMeetingEndPosition - meetingInfo.top;
                    }
                }

                if(i == this.meetingCount - 1){
                    this._calcLeftAndWidth(overlapMeetingIndex);
                }
            }
        },

        _calcLeftAndWidth: function(overlapMeetingIndex){
            var itemViewWidth,tempMeetingInfo;
                itemViewWidth = this.defaultItemViewWidth / overlapMeetingIndex.length;
            for(var j = 0, len = overlapMeetingIndex.length; j < len; j++){
                tempMeetingInfo = _this.meetingListAdapter.getMeetingInfo(overlapMeetingIndex[j]);
                tempMeetingInfo.left = itemViewWidth * j + this.defaultItemViewLeft;
                tempMeetingInfo.width = itemViewWidth - 3;
            }
        },

        _getMeetingStartTimeMinutes: function(startTime){
            var displayTime = uc.util.TimeUtil.UnixTimeToDisplayTime(startTime, uc.settings.meeting.timeZone.offset, uc.constants.UI.TimeStampUnit.Second);
            var startTimeMinutes;
            if(displayTime.getFullYear() == this.selectedDate.getFullYear()
                && displayTime.getMonth() == this.selectedDate.getMonth()
                && displayTime.getDate() == this.selectedDate.getDate()){
                startTimeMinutes = displayTime.getHours() * 60 + displayTime.getMinutes();
            }else {
                startTimeMinutes = 0;
            }
            return startTimeMinutes;
        },

        _scheduleMeeting: function(){
            var scheduleTime = uc.util.TimeUtil.getCurrentHalfOrWholeHourDisplayTime(uc.settings.meeting.timeZone.offset);
            scheduleTime.setFullYear(this.selectedDate.getFullYear(), this.selectedDate.getMonth(), this.selectedDate.getDate());
            var param = {
                shareUserId: uc.IUIService.getAuthorizerId(),
                type: uc.constants.Meeting.MeetingCreateType.Default,
                isCreate: true,
                startTime: uc.util.TimeUtil.displayTimeToUnixTime(scheduleTime,
                    uc.settings.meeting.timeZone.offset, uc.constants.UI.TimeStampUnit.Second)
            };

            uc.IUIService.createMeeting("CreateMeeting", param, function (args) {
                if (args.success) {
                    uc.IUIService.changeModule(uc.constants.UI.ModuleId.Meeting);
                }
            });
        },
        
        _showScheduleTimeTooEarlyWarning: function () {
            clearTimeout(this.clearWarningTimer);
            this.scheduleTimeTooEarlyWarningNode.addClass('slide');
            this.clearWarningTimer = setTimeout(function () {
                _this.scheduleTimeTooEarlyWarningNode.removeClass('slide');
            }, uc.constants.Timer.OneAndAHalfSeconds);
        },

        _onMeetingListGridItemDblClicked: function(selectedHour, target){
            var selectedTime = this.selectedDate.getTime() + selectedHour * 60 * 60 * 1000;
            var nextHourTime = selectedTime + 60 * 60 * 1000;

            var currentTime = new Date();
            if(nextHourTime < currentTime){
                this._showScheduleTimeTooEarlyWarning();
                return;
            }

            selectedTime = selectedTime > currentTime ? selectedTime : currentTime;

            var idleTimeInterval = this._getIdleTimeInterval(selectedTime, nextHourTime);
            var timeInterval = idleTimeInterval.endTime - idleTimeInterval.startTime;
            if(timeInterval > 0){

                target.addClass('fadeOut');//创建会议时，列表给个回馈

                var meetingDuration = timeInterval / 1000 / 60;
                meetingDuration = meetingDuration < 30 ? 30 : meetingDuration;

                var meetingStartTime = idleTimeInterval.startTime / 1000;

                uc.IUIService.createMeeting("CreateMeeting", {
                        startTime: meetingStartTime,
                        duration: meetingDuration,
                        isCreate: true
                    }, function (args) {
                        if (args.success) {
                            //uc.IUIService.changeModule(uc.constants.UI.ModuleId.Meeting);
                        }
                    }
                );

                this.clearFadeOut();
            }
        },

        clearFadeOut: function () {
            var self = this;
            setTimeout(function () {
                self.meetingList.find('.fadeOut').removeClass('fadeOut');
            }, uc.constants.Timer.OneAndAHalfSeconds);
        },

        _onMeetingListGridItemRightClicked: function(e, selectedHour){
            var selectedTime = this.selectedDate.getTime() + selectedHour * 60 * 60 * 1000;
            var nextHourTime = selectedTime + 60 * 60 * 1000;

            var currentTime = new Date();
            if(nextHourTime < currentTime){
                this._showScheduleTimeTooEarlyWarning();
                return;
            }

            selectedTime = selectedTime > currentTime ? selectedTime : currentTime;

            var idleTimeInterval = this._getIdleTimeInterval(selectedTime, nextHourTime);
            var timeInterval = idleTimeInterval.endTime - idleTimeInterval.startTime;
            if(timeInterval > 0){
                if(idleTimeInterval.startTime != selectedTime && idleTimeInterval.startTime <= selectedHour + 30 * 60 * 1000){
                    idleTimeInterval.startTime = selectedHour + 30 * 60 * 1000;
                }else {
                //
                }
                var meetingDuration = timeInterval / 1000 / 60;
                meetingDuration = meetingDuration < 30 ? 30 : meetingDuration;

                var meetingStartTime = idleTimeInterval.startTime / 1000;

                uc.util.MenuUtil.showMeetingListContextMenu(e, {
                    startTime: meetingStartTime,
                    duration: meetingDuration
                });
            }
        },

        _getIdleTimeInterval: function(startTime, endTime){
            var meetingCount = this.meetingListAdapter.getCount();
            var idleStartTime = startTime,
                idleEndTime = endTime;
            var meetingInfo, meetingStartTime, meetingEndTime;
            for(var i = 0; i < meetingCount; i++){
                meetingInfo = this.meetingListAdapter.getMeetingInfo(i);
                meetingStartTime = meetingInfo.startTime * 1000;
                meetingEndTime = meetingStartTime + meetingInfo.duration * 60 * 1000;

                if(meetingStartTime <= idleStartTime && meetingEndTime >= idleStartTime){
                    idleStartTime = meetingEndTime;
                }else if(meetingStartTime <= idleEndTime && meetingEndTime >= idleEndTime){
                    idleEndTime = meetingStartTime;
                }else  if(meetingStartTime >= idleStartTime && meetingEndTime <= idleEndTime){
                    idleEndTime = meetingStartTime;
                }
            }

            //console.log('========== idleStartTime = ', idleStartTime);
            //console.log('========== idleEndTime = ', idleEndTime);
            //console.log('========== duration = ', idleEndTime - idleStartTime);

            return {
                startTime: idleStartTime,
                endTime: idleEndTime
            }
        },

        clearToShowMeetingInfo: function(){
            this.toShowMeetingInfo = null;
        },

        _onDateSelected: function(args){
            uc.util.LogUtil.info('MeetingList', 'onDateSelected', 'selectedDate = ', args.selectedDate);
            
            this.selectedDate = args.selectedDate;
            uc.modules.meeting.MeetingListManager.onDateSelected(this.selectedDate);
            
            this._setCurrentTimeIndicator();
            this._scrollMeetingList();
            this._getMeetingList(this.selectedDate);
            if(this.isFromMeetingList || args.triggeredByClicked){
                this.clearToShowMeetingInfo();
            }

            if(args.triggeredByClicked){ //click date in calendar date picker, show no meeting selected
                uc.IUIService.showMeetingDetail(null);
            }
        },

        _onDateRangeChanged: function(args){
            uc.util.LogUtil.info('MeetingList', 'onDateRangeChanged', 'dateRange = ', args);
            uc.modules.meeting.MeetingListManager.onDateRangeChanged(args.firstDate, args.lastDate);
        },

        _getMeetingList: function(meetingTime){
            uc.modules.meeting.MeetingListManager.getOneDayMeetingList(meetingTime);
        },

        _isShownToday: function () {
            var currentTime = uc.util.TimeUtil.localTimeToDisplayTime(new Date(), uc.settings.meeting.timeZone.offset);
            return (currentTime.getFullYear() == this.selectedDate.getFullYear()
                && currentTime.getMonth() == this.selectedDate.getMonth()
                && currentTime.getDate() == this.selectedDate.getDate());
        },

        _setCurrentTimeIndicator: function(){
            if(this._isShownToday()){
                var hour = this._getTodayCurrentHour();
                this.currentTimeIndicator.css({
                    top: (hour + 0.5) * this.gridHeight - 8
                }).show();
            }else {
                this.currentTimeIndicator.hide();
            }
        },

        _getTodayCurrentHour: function () {
            var currentTime = uc.util.TimeUtil.localTimeToDisplayTime(new Date(), uc.settings.meeting.timeZone.offset);
            return currentTime.getHours();
        },

        _scrollMeetingList: function(selectedMeetingTime){
            var scrollTop = 0, startTime, startHour;
            //scroll to selected meeting time
            if(selectedMeetingTime){
                startTime = uc.util.TimeUtil.UnixTimeToDisplayTime(selectedMeetingTime,
                        uc.settings.meeting.timeZone.offset, uc.constants.UI.TimeStampUnit.Second);
                startHour = startTime.getHours();
                scrollTop = parseInt(this.gridHeight * startHour);
            }else if(this._isShownToday()){ //scroll to today's current time
                var hour = this._getTodayCurrentHour();
                scrollTop = hour * this.gridHeight;
            }else {
                if(this.meetingListAdapter.getCount() == 0){ //No meeting, scroll to 7
                    scrollTop = this.gridHeight * 7;
                }else {//scroll to first meeting
                    var firstMeeting = this.meetingListAdapter.getMeetingInfo(0);
                    startTime = uc.util.TimeUtil.UnixTimeToDisplayTime(firstMeeting.startTime,
                            uc.settings.meeting.timeZone.offset, uc.constants.UI.TimeStampUnit.Second);
                    if(startTime < this.selectedDate){
                        startTime = this.selectedDate;
                    }
                    startHour = startTime.getHours();
                    scrollTop = parseInt(this.gridHeight * startHour);
                }
            }
            setTimeout(function(){
                _this.meetingListPanel.scrollTop(scrollTop);
            }, 100);
        },

        onAuthorizerUserIdChanged: function(){
            this.clearToShowMeetingInfo();
            this.calendarDatePicker.setDate(new Date(), true, true);
        },

        OnModuleChanged: function (moduleId, mountId, dataObj) {
            if(moduleId == uc.constants.UI.ModuleId.Meeting){
                if(this.meetingModuleClicked){
                    this._setCurrentTimeIndicator();
                    return;
                }
                this.meetingModuleClicked = true;
                this._scrollMeetingList();
            }
        },

        OnDateChanged:function(){
            this.calendarDatePicker.reset();
        },

        OnShowMeetingDetail: function (meetingInfo, isFromMeetingList) {
            if(meetingInfo){
                var meetingStartTime = meetingInfo.startTime;
                this.toShowMeetingInfo = meetingInfo;
                this.isFromMeetingList = isFromMeetingList;

                var displayTime = uc.util.TimeUtil.UnixTimeToDisplayTime(meetingStartTime, uc.settings.meeting.timeZone.offset, uc.constants.UI.TimeStampUnit.Second);
                this.calendarDatePicker.setDate(displayTime, !isFromMeetingList, false);
            }
        },

        showMeetingMarks: function (hasMeetingDates) {
            this.calendarDatePicker.showMeetingMarks(hasMeetingDates);
        },

        showMeetingList: function(){
            this._initMeetingListItemView();
            this._showMeetingListItemView();
        },

        notifyDataSetChanged: function(){
            this.showMeetingList();
        },

        clear: function(){
            this.calendarDatePicker.reset();
        }
    })
})(uc);