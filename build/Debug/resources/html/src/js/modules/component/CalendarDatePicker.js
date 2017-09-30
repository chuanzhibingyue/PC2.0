(function(uc) {
    var _this = null;
    uc.modules.component.CalendarDatePicker = function() {
        _this = this;

        this.options = {
            containerId: null,
            dateMode: 1, //1: one week; 2: one month; 3: two week
            timezone: -((new Date()).getTimezoneOffset()) / 60
        };

        this.nodeHtml = "<div class='calendar-box'>" +
                            "<div class='date-info'>" +
                                "<div>" +
                                    "<span class='today-info today-info-manu'>"+ this._formatDateTitle(this._getSystemToday()) + "</span>" +
                                    "<div class='nav'>" +
                                        "<a class='left'></a>" +
                                        "<span class='today-btn' href='javascript:void(0);'>" + uc_resource.Meeting.Today + "</span>" +
                                        "<a class='right'></a>" +
                                    "</div>" +
                                "</div>" +
                            "</div>" +
                            "<ul class='header clear-float'>" +
                                "<li>" + uc_resource.Meeting.NumMonday + "</li>" +
                                "<li>" + uc_resource.Meeting.NumTuesday + "</li>" +
                                "<li>" + uc_resource.Meeting.NumWednesday + "</li>" +
                                "<li>" + uc_resource.Meeting.NumThursday + "</li>" +
                                "<li>" + uc_resource.Meeting.NumFriday + "</li>" +
                                "<li>" + uc_resource.Meeting.NumSaturday + "</li>" +
                                "<li>" + uc_resource.Meeting.NumSunday + "</li>" +
                            "</ul>" +
                        "</div>";

        this.selectedDate = null;
    };

    jQuery.extend(uc.modules.component.CalendarDatePicker.prototype, {
        init: function(options){
            this.options = jQuery.extend(this.options, options);

            if (!this.options.containerId) {
                return;
            }

            this._init();
        },

        _init: function(){
            this._initUI(this.options.containerId);
            this._setToToday(true, false);

            this.bindEvent();
        },

        _initUI: function(containerId){
            if (containerId.jquery) {
                this.container = containerId;
            } else {
                this.container = $("#" + containerId);
            }

            this.calendarBox = $(this.nodeHtml).appendTo(this.container);
        },

        _renderDates: function(dates){
            var uls = this._getDateRenders(dates);

            uls.map(function(ul){
                _this.calendarBox.append(ul);
            })
        },

        _getDateRenders: function(dates){
            var $uls = [],
                $ul, $li, date, isToday;
            for (var i = 0; i < dates.length; i++) {
                if (i % 7 === 0) {
                    $ul = $("<ul class='days'/>");
                    $uls.push($ul);
                }
                isToday = false;
                date = dates[i];
                if(this._formatDate(date) == this._formatDate(this._getSystemToday())){
                    isToday = true;
                }
                $li = $("<li><a href='javascript:void(0);'></a></li>");
                var $a = $li.find("a").text(dates[i].getDate()).attr("date", this._formatDate(date));
                if(isToday){
                    $a.addClass('today');
                }
                if (!date.active) {
                    $a.addClass("inactive");
                }

                $li.appendTo($ul);
            }
            $ul.append("<br class='clear-float' />");

            return $uls;
        },

        bindEvent: function(){
            this.calendarBox.on("click", "div.nav a", function() {
                var isLeft = $(this).hasClass("left");
                _this._changeDateRange(isLeft);
            });

            this.calendarBox.on("click", ".today-btn", function() {
                _this._setToToday(false, true);
            });

            this.calendarBox.on("click", "ul.days a", function() {
                var isInactive = $(this).hasClass('inactive');
                if (isInactive) {
                    return;
                }

                _this.onDateItemClicked(parseInt($(this).attr('date')));
            });

            this.calendarBox.on("mouseover mouseout", ".nav a, .nav span", function(e) {
                if (e.type == "mouseover") {
                    _this.calendarBox.find(".nav a").css("border-color", "#ededed");
                    _this.calendarBox.find(".nav span").css("border-color", "#ededed");
                } else if (e.type == "mouseout") {
                    _this.calendarBox.find(".nav a").css("border-color", "#ffffff");
                    _this.calendarBox.find(".nav span").css("border-color", "#ffffff");
                }
            });
        },

        _changeDateRange: function(isLeft){
            var currentDateRange = this._getCurrentDateRange();
            var oneDate;
            switch (this.options.dateMode){
                case uc.constants.UI.DatePicker.DateMode.OneWeek:
                case uc.constants.UI.DatePicker.DateMode.OneMonth:
                    oneDate = isLeft ? currentDateRange.firstDate.addDays(-1) : currentDateRange.lastDate.addDays(1);
                    break;
                case uc.constants.UI.DatePicker.DateMode.TwoWeek:
                    oneDate = isLeft ? currentDateRange.firstDate.addDays(-14) : currentDateRange.lastDate.addDays(1);
                    break;
                default:
                    break;
            }
            this._showDatePicker(isLeft, oneDate, true);
        },

        _getCurrentDateRange: function(){
            var firstDate = this.calendarBox.find("a[date]:first").attr("date"),
                lastDate = this.calendarBox.find("a[date]:last").attr("date");

            return {
                firstDate: firstDate ? this._parseDate(firstDate) : this._getSystemToday(),
                lastDate: lastDate ? this._parseDate(lastDate) : this._getSystemToday()
            };
        },

        onDateItemClicked: function(date){
            this._setSelectedDate(date, true, true);
        },

        _setToToday: function(forceDateRangeChange, triggeredByClicked){
            this._navigateByDate(this._getSystemToday(), true, forceDateRangeChange, triggeredByClicked);
        },

        _setSelectedDate: function(date, triggerDateSelectedEvent, triggeredByClicked){
            if (!date) {
                return;
            }

            this.calendarBox.find("a.selected").removeClass("selected");
            this.calendarBox.find('a.today-selected').removeClass('today-selected');

            var temp = this._isDate(date) ? this._formatDate(date) : date;

            if(temp == this._formatDate(this._getSystemToday())){
                this.calendarBox.find("a[date='" + temp + "']").addClass("today-selected");
            }else {
                this.calendarBox.find("a[date='" + temp + "']").addClass("selected");
            }

            this.selectedDate = this._parseDate(temp);

            if (triggerDateSelectedEvent) {
                this._triggerDateSelectedEvent(this.selectedDate, triggeredByClicked);
            }

            this._setActiveDate(date);
        },

        _triggerDateSelectedEvent: function (selectedDate, triggeredByClicked) {
            this.options.onDateSelected && this.options.onDateSelected({
                selectedDate: selectedDate,
                triggeredByClicked: triggeredByClicked
            });
        },

        _getOneWeek: function(date){
            var day = date.getDay(),
                factor = day === 0 ? 7 : day;

            var weekDays = [];
            for (var i = 1; i <= 7; i++) {
                var temp = date.addDays(i - factor);
                temp.active = true;

                weekDays.push(temp);
            }

            return weekDays;
        },

        _getOneMonth: function(date){
            var firstDate = this._getStartDate(date),
                endDate = this._getEndDate(date),
                month = date.getMonth();

            var monthDays = [],
                temp = firstDate;
            while (temp <= endDate) {
                temp.active = month == temp.getMonth();
                monthDays.push(temp);

                temp = temp.addDays(1);
            }

            return monthDays;
        },

        _getTwoWeek: function(date){
            var day = date.getDay(),
                factor = day === 0 ? 7 : day;

            var weekDays = [];
            for (var i = 1; i <= 14; i++) {
                var temp = date.addDays(i - factor);
                temp.active = true;

                weekDays.push(temp);
            }

            return weekDays;
        },

        _getStartDate: function(date){
            var temp = new Date(date.getFullYear(), date.getMonth(), 1),
                day = temp.getDay();

            day = day === 0 ? 7 : day;

            return temp.addDays(1 - day);
        },

        _getEndDate: function(date){
            var year = date.getFullYear(),
                month = date.getMonth();
            year += (month + 1) / 12;
            month = (month + 1) % 12;

            var nextMonthDate = new Date(year, month, 1),
                temp = nextMonthDate.addDays(-1);

            var day = temp.getDay();
            if (day === 0) {
                return temp;
            } else {
                return temp.addDays(7 - day);
            }
        },

        _getDates: function(dateMode, date){
            var days;

            switch (dateMode){
                case uc.constants.UI.DatePicker.DateMode.OneWeek:
                    days = this._getOneWeek(date);
                    break;
                case uc.constants.UI.DatePicker.DateMode.OneMonth:
                    days = this._getOneMonth(date);
                    break;
                case uc.constants.UI.DatePicker.DateMode.TwoWeek:
                    days = this._getTwoWeek(date);
                    break;
                default:
                    break;
            }

            return days;
        },

        /**
         * format date style to "20160428"
         * @param date
         * @returns {*}
         * @private
         */
        _formatDate: function(date){
            return date.getFullYear() * 10000 + (date.getMonth() + 1) * 100 + date.getDate();
        },

        _getSystemToday: function(){
            var today = new Date();
            today.setHours(0, 0, 0, 0);

            return today;
        },

        _formatDateTitle: function (date) {
            var year = date.getFullYear(),
                month = date.getMonth() + 1,
                day = date.getDate();
            return year + uc_resource.Meeting.Year + month + uc_resource.Meeting.Month + "";
        },

        _parseDate: function(val){
            var temp = parseInt(val),
                year = Math.floor(temp / 10000),
                month = Math.floor((temp - year * 10000) / 100) - 1,
                day = temp - year * 10000 - (month + 1) * 100;

            return new Date(year, month, day, 0, 0, 0, 0);
        },

        _isDate: function(val){
            if (!val) {
                return false;
            }
            return Object.prototype.toString.call(val) === '[object Date]';
        },

        _navigateByDate: function (date, triggerDateSelectedEvent, forceDateRangeChange, triggeredByClicked) {
            var moveLeft = true,
                dateRangeChanged = true,
                currentDateRange = this._getCurrentDateRange();

            var firstDate = this._formatDate(currentDateRange.firstDate),
                lastDate = this._formatDate(currentDateRange.lastDate),
                dateString = this._formatDate(date);

            //if there is no date shown, show initial date
            if(firstDate == dateString && lastDate == dateString){
                moveLeft = false;
            }else if (firstDate <= dateString && dateString <= lastDate) {
                this._setSelectedDate(date, triggerDateSelectedEvent, triggeredByClicked);
                dateRangeChanged = false;
            } else if (currentDateRange.lastDate < date) {
                moveLeft = false;
            }

            if(dateRangeChanged || forceDateRangeChange){
                this._showDatePicker(moveLeft, date, false);
            }

            this._setSelectedDate(date, triggerDateSelectedEvent, triggeredByClicked);
        },

        _showDatePicker: function (moveLeft, oneDate, triggeredByNavigateClicked) {
            this.calendarBox.find("ul.days").remove();

            var dates = this._getDates(this.options.dateMode, oneDate);
            this._renderDates(dates);

            var $days = this.calendarBox.find("ul.days");
                // width = $days.parent().width();
            //if (moveLeft) {
            //    $days.css("left", -width);
            //} else {
            //    $days.css("left", width);
            //}

            $days.animate({left: "8px"}, 200);

            this.calendarBox.find("span.today-info").text(this._formatDateTitle(oneDate));

            this._triggerDateRangeChanged(dates[0], dates[dates.length - 1], triggeredByNavigateClicked);
        },

        _triggerDateRangeChanged: function (firstDate, lastDate, triggeredByNavigateClicked) {
            if (this.options.activeFirstDay && triggeredByNavigateClicked) {
                this.calendarBox.find("ul.days:first").find('li:first>a').trigger('click');
            }
            
            if (this.options.onDateRangeChanged) {
                this.options.onDateRangeChanged({
                    firstDate: firstDate,
                    lastDate: lastDate
                });
            }
        },

        _showMeetingMarks: function (hasMeetingDates) {
            var eventIdArray;
            for(var date in hasMeetingDates){
                eventIdArray = hasMeetingDates[date];

                if(eventIdArray && eventIdArray.length){
                    var isInactive = _this.calendarBox.find("a[date='" + date + "']").hasClass('inactive');
                    if (isInactive) {
                        return;
                    }
                    _this.calendarBox.find("a[date='" + date + "']").addClass("marked-date");
                }else {
                    _this.calendarBox.find("a[date='" + date + "']").removeClass("marked-date");
                }
            }
        },

        _setActiveDate: function(date){
            date = date ? (typeof date == 'number' ? date : this._formatDate(date)) : this._getSystemToday();
            uc.util.StorageUtil.setData('activeDate', date);
        },

        setDate: function(date, triggerDateSelectedEvent, forceDateRangeChange){
            if (date && this._isDate(date)) {
                this.selectedDate = date;
                this._navigateByDate(date, triggerDateSelectedEvent, forceDateRangeChange, false);
            }
        },

        showMeetingMarks: function(hasMeetingDates){
            this._showMeetingMarks(hasMeetingDates);
        },

        reset: function(){
            this._setToToday(true, false);
        }
    })
})(uc);