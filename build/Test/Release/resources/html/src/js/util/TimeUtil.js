/**
 * Created by senlin.ouyang on 2015/7/24.
 */
(function (uc) {
    Date.prototype.addDays = function (days) {
        var date = new Date();

        date.setTime(this.getTime() + days * 24 * 60 * 60 * 1000);
        return date;
    };

    uc.util.TimeUtil = {
        /**
         * get time string in message
         * @param  time
         * @param  timeFormat optional
         * @param  settingTimeZone
         */
        getMessageTimeString: function (time, timeFormat, settingTimeZone) {
            if (time == "" || ((time + "").length < 13) || (typeof time) == "string") {
                uc.util.LogUtil.info('TimeUtil', 'getMessageTimeString', 'time = ' + time + ', timeFormat = ' + timeFormat);
            }

            var timeData = this.UnixTimeToDisplayDateStringInfo(time, settingTimeZone, uc.constants.UI.TimeStampUnit.MilliSecond);
            var timeString;
            var timeFormatStr;
            if (timeFormat) {
                timeFormatStr = this._getMessageTimeFormat(timeFormat);
                switch (timeFormat) {
                    case uc.constants.UI.TimeFormat.Message.HourMinutes:
                        timeString = uc.util.StringUtil.formatString(timeFormatStr, [timeData.hour, timeData.minute]);
                        break;
                    case uc.constants.UI.TimeFormat.Message.HourMinutesSecond:
                        timeString = uc.util.StringUtil.formatString(timeFormatStr, [timeData.hour, timeData.minute, timeData.second]);
                        break;
                    case uc.constants.UI.TimeFormat.Message.MonthDay:
                        timeString = uc.util.StringUtil.formatString(timeFormatStr, [timeData.month, timeData.day]);
                        break;
                    case uc.constants.UI.TimeFormat.Message.YearMonthDayHourMinuteSecond:
                        timeString = uc.util.StringUtil.formatString(timeFormatStr, [timeData.year, timeData.month, timeData.day, timeData.hour, timeData.minute, timeData.second]);
                        break;
                    case uc.constants.UI.TimeFormat.Message.YearMonthDay:
                        timeString = uc.util.StringUtil.formatString(timeFormatStr, [timeData.year, timeData.month, timeData.day]);
                        break;
                }
            } else {
                var currentTime = new Date();
                var currentDay = currentTime.getDate();
                var currentMonth = currentTime.getMonth();
                var currentYear = currentTime.getFullYear();
                if (timeData.day == currentDay && timeData.month == currentMonth + 1) {
                    timeFormatStr = this._getMessageTimeFormat(uc.constants.UI.TimeFormat.Message.HourMinutes);
                    timeString = uc.util.StringUtil.formatString(timeFormatStr, [timeData.hour, timeData.minute]);
                } else if (timeData.year == currentYear) {
                    timeFormatStr = this._getMessageTimeFormat(uc.constants.UI.TimeFormat.Message.MonthDay);
                    timeString = uc.util.StringUtil.formatString(timeFormatStr, [timeData.month, timeData.day]);
                } else {
                    timeFormatStr = this._getMessageTimeFormat(uc.constants.UI.TimeFormat.Message.YearMonthDay);
                    timeString = uc.util.StringUtil.formatString(timeFormatStr, [timeData.year, timeData.month, timeData.day]);
                }
            }
            return timeString;
        },

        getMeetingStartTimeString: function (meetingStartTime, timeFormat, settingTimeZone) {
            var timeString;
            var timeData = this.UnixTimeToDisplayDateStringInfo(meetingStartTime, settingTimeZone, uc.constants.UI.TimeStampUnit.Second);
            var timeFormatStr;
            var type = 0;
            if (timeFormat) {
                timeFormatStr = this._getMeetingTimeFormat(timeFormat);
                switch (timeFormat) {
                    case uc.constants.UI.TimeFormat.Meeting.HourMinutes:
                        timeString = uc.util.StringUtil.formatString(timeFormatStr, [timeData.hour, timeData.minute]);
                        break;
                    case uc.constants.UI.TimeFormat.Meeting.MonthDay:
                        timeString = uc.util.StringUtil.formatString(timeFormatStr, [timeData.month, timeData.day]);
                        break;
                    case uc.constants.UI.TimeFormat.Meeting.YearMonthDayHourMinuteSecond:
                        timeString = uc.util.StringUtil.formatString(timeFormatStr, [timeData.year, timeData.month, timeData.day, timeData.hour, timeData.minute, timeData.second]);
                        break;
                    case uc.constants.UI.TimeFormat.Meeting.YearMonthDayHourMinutes:
                        timeString = uc.util.StringUtil.formatString(timeFormatStr, [timeData.year, timeData.month, timeData.day, timeData.hour, timeData.minute]);
                        break;
                    case uc.constants.UI.TimeFormat.Meeting.YearMonthDay:
                        timeString = uc.util.StringUtil.formatString(timeFormatStr, [timeData.year, timeData.month, timeData.day]);
                        break;
                    case uc.constants.UI.TimeFormat.Meeting.DayMonthYear:
                        timeString = uc.util.StringUtil.formatString(timeFormatStr, [timeData.day, timeData.month, timeData.year]);
                        break;
                    case uc.constants.UI.TimeFormat.Meeting.MonthDayYear:
                        timeString = uc.util.StringUtil.formatString(timeFormatStr, [timeData.month, timeData.day, timeData.year]);
                        break;
                    case uc.constants.UI.TimeFormat.Meeting.MonthDayChs:
                        timeString = uc.util.StringUtil.formatString(timeFormatStr, [timeData.month, timeData.day]);
                        break;
                    case uc.constants.UI.TimeFormat.Meeting.YearMonthDayChs:
                        timeString = uc.util.StringUtil.formatString(timeFormatStr, [timeData.year, timeData.month, timeData.day]);
                        break;
                }
            } else {
                var currentTime = this.UnixTimeToDisplayDateStringInfo(new Date(), settingTimeZone, uc.constants.UI.TimeStampUnit.MilliSecond);
                if (timeData.day == parseInt(currentTime.day) && timeData.month == parseInt(currentTime.month) && timeData.year == parseInt(currentTime.year)) {
                    timeFormat = this._getMeetingTimeFormat(uc.constants.UI.TimeFormat.Meeting.HourMinutes);
                    timeString = uc.util.StringUtil.formatString(timeFormat, [timeData.hour, timeData.minute]);
                } else if (timeData.day == parseInt(currentTime.day) + 1 && timeData.month == parseInt(currentTime.month) && timeData.year == parseInt(currentTime.year)) {
                    timeString = uc_resource.Meeting.Tomorrow;
                    type = 1;
                } else {
                    timeFormat = this._getMeetingTimeFormat(uc.constants.UI.TimeFormat.Meeting.HourMinutesChs);
                    timeString = uc.util.StringUtil.formatString(timeFormat, [timeData.month, timeData.day]);
                    type = 2;
                }
            }
            return {timeString: timeString, type: type};
        },

        _getMessageTimeFormat: function (timeFormat) {
            return uc.constants.UI.TimeFormat.Message.Format[timeFormat];
        },

        _getMeetingTimeFormat: function (timeFormat) {
            return uc.constants.UI.TimeFormat.Meeting.Format[timeFormat];
        },

        /**
         * @method  DateToString
         * @desc 将时间按照日期格式输出
         * @param  {object} date 日期对象
         * @param {int} dateFormat 日期格式
         * @returns {string} 格式化后的日期
         */
        DateToString: function (date, dateFormat) {
            var month = date.month < 10 ? '0' + date.month : date.month;
            var day = date.day < 10 ? '0' + date.day : date.day;
            var minute = date.minute < 10 ? '0' + date.minute : date.minute;

            if (date.day) {
                switch (parseInt(dateFormat)) {
                    case 1:
                        return date.year + "/" + month + "/" + day;
                        break;
                    case 2:
                        return day + "/" + month + "/" + date.year;
                        break;
                    case 3:
                        return month + "/" + day + "/" + date.year;
                        break;
                    case 4:
                        return month + uc_resource.Meeting.Month + day + uc_resource.Meeting.Day;
                        break;
                    case 5:
                        return date.hour + ":" + minute;
                        break;
                    case 6:
                        return date.year + "/" + month + "/" + day + " " + date.hour + ":" + minute;
                        break;
                    case 7:
                        return date.year + uc_resource.Meeting.Year + month + uc_resource.Meeting.Month + day + uc_resource.Meeting.Day + date.hour + ":" + minute;
                        break;
                    case 8:
                        return (date.year + "").substr(2, 2) + "/" + month + "/" + day;
                        break;
                    case 9:
                        return month + "/" + day;
                        break;
                    case 0:
                        //return date.year + "/" + month + "/" + day + "<div style='display:inline-block;width:20px;'></div>" + (date.hour < 10 ? "0" + date.hour : date.hour) + ":" + minute;
                        return date.year + "/" + month + "/" + day + "|" + (date.hour < 10 ? "0" + date.hour : date.hour) + ":" + minute + "|" + date.week;
                        break;
                    default:
                        return date.year + uc_resource.Meeting.Year + month + uc_resource.Meeting.Month + day + uc_resource.Meeting.Day;
                        break;
                }
            } else {
                switch (parseInt(dateFormat)) {
                    case 1:
                        return date.year + "/" + month;
                        break;
                    case 2:
                    case 3:
                        return month + "/" + date.year;
                        break;
                    case 4:
                    default:
                        return date.year + uc_resource.Meeting.Year + month + uc_resource.Meeting.Month;
                        break;
                }
            }
        },
        /**
         * @method  min2hour 分钟转换为小时
         * @param {int} min 分钟数，必选
         * @returns {string} 比如 1425 转换为23小时45分钟
         */
        min2hour: function (min, type, dateFormat) {
            var hour = parseInt(min / 60);
            var minutes = parseInt(min - hour * 60);
            if (hour == 0) {
                if (dateFormat) {
                    if (minutes < 10) {
                        minutes = '0' + minutes;
                    }
                    return '00:' + minutes;
                }
                return minutes + uc_resource.Meeting.Minutes;
            }
            if (hour != 0 && minutes == 0) {
                if (dateFormat) {
                    if (hour < 10) {
                        hour = '0' + hour;
                    }
                    return hour + ':00';
                } else {
                    return hour + uc_resource.Meeting.Hour;
                }
            }
            if (type) {
                if (minutes < 30) {
                    return hour + uc_resource.Meeting.Hour;
                }
                return hour + ".5" + uc_resource.Meeting.Hour;
            }
            if (dateFormat) {
                if (minutes < 10) {
                    minutes = '0' + minutes;
                }
                return hour + ":" + minutes;
            } else {
                return hour + uc_resource.Meeting.Hour + minutes + uc_resource.Meeting.Minutes;
            }
        },

        /**
         * 转换为00:05格式
         */
        getAudioTimeStr: function (seconds) {
            if (seconds >= 60) {
                return "00:59";
            }
            if (seconds > 9) {
                return "00:" + seconds;
            }
            return "00:0" + seconds;
        },

        getSysMessageTimeStr: function (seq) {
            var date = new Date(seq);
            var today = new Date();
            var yesterday = new Date();
            yesterday.setDate(today.getDate() - 1);
            if (date.toDateString() === today.toDateString()) {
                return uc_resource.Meeting.Today;
            } else if (date.toDateString() === yesterday.toDateString()) {
                return uc_resource.Meeting.Yesterday;
            } else {
                if (date.getYear() == today.getYear()) {
                    var month = (date.getMonth() + 1) < 10 ? '0' + (date.getMonth() + 1) : date.getMonth() + 1;
                    var day = date.getDate() < 10 ? '0' + date.getDate() : date.getDate();
                    return month + "/" + day;
                } else {
                    return date.toISOString().slice(0, 10);
                }
            }
        },

        getPbxMessageTimeStr: function (seq) {
            var date = new Date(seq);
            var today = new Date();
            var yesterday = new Date();
            yesterday.setDate(today.getDate() - 1);
            if (date.toDateString() === today.toDateString()) {
                return uc_resource.Meeting.Today;
            } else if (date.toDateString() === yesterday.toDateString()) {
                return uc_resource.Meeting.Yesterday;
            } else {
                if (date.getYear() == today.getYear()) {
                    var month = (date.getMonth() + 1) < 10 ? '0' + (date.getMonth() + 1) : date.getMonth() + 1;
                    var day = date.getDate() < 10 ? '0' + date.getDate() : date.getDate();
                    return month + uc_resource.Meeting.Month + day + uc_resource.Meeting.Day;
                } else {
                    return date.toISOString().slice(0, 10);
                }
            }
        },

        showMeetingTime: function (meetingDetail, settingTimeZone) {
            var startTime = uc.util.TimeUtil.UnixTimeToDisplayDateInfo(meetingDetail.startTime, settingTimeZone, uc.constants.UI.TimeStampUnit.Second);
            startTime = uc.util.TimeUtil.DateToString(startTime, 0).split("|");

            var endTime = uc.util.TimeUtil.UnixTimeToDisplayDateInfo(meetingDetail.startTime + meetingDetail.duration * 60, settingTimeZone, uc.constants.UI.TimeStampUnit.Second);
            endTime = uc.util.TimeUtil.DateToString(endTime, 0).split("|");

            var weekMap = {
                0: uc_resource.Meeting.ReSunday,
                1: uc_resource.Meeting.ReMonday,
                2: uc_resource.Meeting.ReTuesday,
                3: uc_resource.Meeting.ReWednesday,
                4: uc_resource.Meeting.ReThursday,
                5: uc_resource.Meeting.ReFriday,
                6: uc_resource.Meeting.ReSaturday
            };

            if (startTime[0] == endTime[0]) {
                return startTime[0] + "&nbsp;&nbsp;" + weekMap[startTime[2]] + "&nbsp;&nbsp;" + startTime[1] + "&nbsp;&nbsp;" + "~" + "&nbsp;&nbsp;" + endTime[1];
            } else {
                return startTime[0] + "&nbsp;&nbsp;" + weekMap[startTime[2]] + "&nbsp;&nbsp;" + startTime[1] + "&nbsp;&nbsp;" + "~" + "&nbsp;&nbsp;" + endTime[0] + "&nbsp;&nbsp;" + weekMap[endTime[2]] + "&nbsp;&nbsp;" + endTime[1];
            }
        },
        /**
         * 转换为00:00格式
         * 大于等于1h，显示00:00:00
         */
        getCallTimeStr: function (seconds) {
            var h = 0, i = 0, s = parseInt(seconds);
            if (s >= 60) {
                i = parseInt(s / 60);
                s = parseInt(s % 60);
                if (i >= 60) {
                    h = parseInt(i / 60);
                    i = parseInt(i % 60);
                }
            }
            // 补零
            var zero = function (v) {
                return (v >> 0) < 10 ? "0" + v : v;
            };
            if (seconds >= 3600) {
                return [zero(h), zero(i), zero(s)].join(":");
            }
            else {
                return [zero(i), zero(s)].join(":");
            }
        },

        ////////////////////////////////////////////////////////////////////////
        /////////////////////Time convert functions/////////////////////////////
        ////////////////////////////////////////////////////////////////////////
        /**
         * localTime:   本地时间，根据操作系统设置的时区来显示的时间
         * displayTime: 界面显示时间，根据设置里设置的时区显示的时间
         * UnixTime:    Unix时间，世界时格式的时间，单位为秒
         */

        /**
         * get local time zone, for example, in China returns 8, it's related to System time zone setting
         * @returns {number}
         */
        getLocalTimeZone: function () {
            var date = new Date();
            return 0 - date.getTimezoneOffset() / 60;
        },
        /**
         * Unix time convert to display time
         * The display time is shown according setting time zone, the time zone may be +8, or -8, or any
         * other time zone, it's not same as local time zone, for example, local time may be +8 for China
         * @param UnixTime              UTC Unix time, unit is second, seconds since 1970.01.01
         * @param settingTimeZone       time zone in setting, may be +8, -8, 0, or any time zone
         * @param unit                  second or milliseconds, see uc.constants.UI.TimeStampUnit
         */
        UnixTimeToDisplayTime: function (UnixTime, settingTimeZone, unit) {
            var localTimeZone = this.getLocalTimeZone();
            var multiple = unit == uc.constants.UI.TimeStampUnit.Second ? 1000 : 1;
            return new Date(UnixTime * multiple + (settingTimeZone - localTimeZone) * 60 * 60 * 1000);
        },

        /**
         * Unix time convert to display Date Info
         * @param UnixTime
         * @param settingTimeZone
         * @param unit
         * @returns {{year: number, month: number, day: (number|*), hour: number, minute: number, second: number, week: number}}
         */
        UnixTimeToDisplayDateInfo: function (UnixTime, settingTimeZone, unit) {
            var dateTime = this.UnixTimeToDisplayTime(UnixTime, settingTimeZone, unit);

            return {
                year: dateTime.getFullYear(),
                month: dateTime.getMonth() + 1,
                day: dateTime.getDate(),
                hour: dateTime.getHours(),
                minute: dateTime.getMinutes(),
                second: dateTime.getSeconds(),
                week: dateTime.getDay()
            };
        },

        /**
         * Unix time convert to display Date Info, data is in String format
         * @param UnixTime
         * @param settingTimeZone
         * @param unit
         * @returns {{year: number, month: *, day: *, hour: *, minute: *, second: *, week: number}}
         */
        UnixTimeToDisplayDateStringInfo: function (UnixTime, settingTimeZone, unit) {
            var date = this.UnixTimeToDisplayTime(UnixTime, settingTimeZone, unit);

            var year = date.getFullYear();
            var month = date.getMonth() + 1;
            var day = date.getDate();
            var hour = date.getHours();
            var minute = date.getMinutes();
            var second = date.getSeconds();
            var week = date.getDay();

            return {
                "year": year,
                "month": month < 10 ? '0' + month : month,
                "day": day < 10 ? '0' + day : day,
                "hour": hour < 10 ? '0' + hour : hour,
                "minute": minute < 10 ? '0' + minute : minute,
                "second": second < 10 ? '0' + second : second,
                "week": week
            };
        },

        /**
         * Unix time convert to local time
         * @param UnixTime
         * @param unit      second or milliseconds, see uc.constants.UI.TimeStampUnit
         * @constructor
         */
        UnixTimeToLocalTime: function (UnixTime, unit) {
            var multiple = unit == uc.constants.UI.TimeStampUnit.Second ? 1000 : 1;
            return new Date(UnixTime * multiple);
        },

        /**
         * get full time value info
         * @param date
         * @returns {{year: number, month: *, day: *, hour: *, minute: *, second: *, week: (*|number)}}
         */
        getFullTimeValueInfo: function (date) {
            var year = date.getFullYear();
            var month = date.getMonth() + 1;
            var day = date.getDate();
            var hour = date.getHours();
            var minute = date.getMinutes();
            var second = date.getSeconds();
            var week = date.getDay();
            return {
                "year": year,
                "month": month < 10 ? '0' + month : month,
                "day": day < 10 ? '0' + day : day,
                "hour": hour < 10 ? '0' + hour : hour,
                "minute": minute < 10 ? '0' + minute : minute,
                "second": second < 10 ? '0' + second : second,
                "week": week
            };
        },

        /**
         * local time convert to Unix time
         * @param localTime
         * @param unit      second or milliseconds, see uc.constants.UI.TimeStampUnit
         */
        localTimeToUnixTime: function (localTime, unit) {
            var multiple = unit == uc.constants.UI.TimeStampUnit.Second ? 1000 : 1;
            return parseInt(localTime.getTime() / multiple);
        },

        /**
         * display time convert to Unix time
         * @param displayTime
         * @param settingTimeZone
         * @param unit              second or milliseconds, see uc.constants.UI.TimeStampUnit
         */
        displayTimeToUnixTime: function (displayTime, settingTimeZone, unit) {
            var localTimeZone = this.getLocalTimeZone();
            var multiple = unit == uc.constants.UI.TimeStampUnit.Second ? 1000 : 1;
            return parseInt(displayTime.getTime() / multiple - (settingTimeZone - localTimeZone) * 60 * 60);
        },

        /**
         * @desc  将所选时区的日期 转换为 Unix时间戳（世界时）
         * @param {int} settingTimeZone
         * @param {int} year
         * @param {int} month
         * @param {int} day
         * @param {int} hour
         * @param {int} minute
         * @param {int} second
         * @param {int} unit    second or milliseconds, see uc.constants.UI.TimeStampUnit
         * @return {int} unix
         */
        displayDateToUnixTime: function (settingTimeZone, year, month, day, hour, minute, second, unit) {
            var displayTime = new Date(
                parseInt(year, 10),
                parseInt(month, 10) - 1,
                parseInt(day, 10),
                parseInt(hour, 10) || 0,
                parseInt(minute, 10) || 0,
                parseInt(second, 10) || 0);

            return this.displayTimeToUnixTime(displayTime, settingTimeZone, unit);
        },

        /**
         * convert display time to the begin of the day with Unix time
         * @param displayTime
         * @param settingTimeZone
         * @param unit              second or milliseconds, see uc.constants.UI.TimeStampUnit
         */
        displayTimeToDayBeginningUnixTime: function (displayTime, settingTimeZone, unit) {
            var date = new Date(displayTime.getFullYear(), displayTime.getMonth(), displayTime.getDate());

            return this.displayTimeToUnixTime(date, settingTimeZone, unit);
        },

        /**
         * convert display time to the end of the day with Unix time
         * @param displayTime
         * @param settingTimeZone
         * @param unit              second or milliseconds, see uc.constants.UI.TimeStampUnit
         */
        displayTimeToDayEndingUnixTime: function (displayTime, settingTimeZone, unit) {
            var date = new Date(displayTime.getFullYear(), displayTime.getMonth(), displayTime.getDate());
            var minus = unit == uc.constants.UI.TimeStampUnit.Second ? 1000 : 1;

            return this.displayTimeToUnixTime(new Date(date.getTime() + 24 * 60 * 60 * 1000 - minus), settingTimeZone, unit);
        },

        /**
         * convert display time to beginning of the day
         * @param displayTime
         * @returns {*}
         */
        displayTimeToDayBeginningDisplayTime: function (displayTime) {
            displayTime.setHours(0);
            displayTime.setMinutes(0);
            displayTime.setSeconds(0);
            displayTime.setMilliseconds(0);
            return displayTime;
        },

        /**
         * convert display time to ending of the day
         * @param displayTime
         */
        displayTimeToDayEndingDisplayTime: function (displayTime) {
            displayTime.setHours(23);
            displayTime.setMinutes(59);
            displayTime.setSeconds(59);
            displayTime.setMilliseconds(999);
            return displayTime;
        },

        /**
         * convert local to display time
         * @param localTime
         * @param settingTimeZone
         */
        localTimeToDisplayTime: function(localTime, settingTimeZone){
            var localTimeZone = this.getLocalTimeZone();
            return new Date(localTime.getTime() + (settingTimeZone - localTimeZone) * 60 * 60 * 1000);
        },

        /**
         * get current time, half hour or whole hour
         * @param settingTimeZone
         * @returns {Date}
         */
        getCurrentHalfOrWholeHourDisplayTime: function(settingTimeZone){
            var displayTime = this.localTimeToDisplayTime(new Date(), settingTimeZone);
            var now = displayTime.getTime();
            var milliSecond = now % (60 * 60 * 1000);
            var time =  milliSecond > 30 * 60 * 1000 ? now + 3600 * 1000 - milliSecond : now + 1800 * 1000 - milliSecond;
            return new Date(time);
        },

        /**
         * get current unix time, half hour or whole hour
         * @param settingTimeZone
         * @param unit
         * @returns {*}
         */
        getCurrentHalfOrWholeHourUnixTime: function (settingTimeZone, unit) {
            var currentTime = this.getCurrentHalfOrWholeHourDisplayTime(settingTimeZone);
            return this.displayTimeToUnixTime(currentTime, settingTimeZone, unit);
        }
    }
})(uc);