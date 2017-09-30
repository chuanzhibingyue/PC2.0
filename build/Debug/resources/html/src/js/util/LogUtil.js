/**
 * Created by waouyang on 15/8/20.
 */
(function(uc){
    uc.util.LogUtil = {
        /**
         * print info log, out format is: [ module ][ method ] message
         * @param module    module of the log, for example Login
         * @param method    method of the log, for example login
         * @param message   log message, for example 'userId = xxxx'
         * @param data      data object if needs to print it
         */
        info: function(module, method, message, data){
            this._printLog(module, method, message, data, uc.constants.UI.LogLevel.Info);
        },
        warn: function(module, method, message, data){
            this._printLog(module, method, message, data, uc.constants.UI.LogLevel.Warning);
        },
        error: function(module, method, message, data) {
            this._printLog(module, method, message, data, uc.constants.UI.LogLevel.Error);
        },
        _printLog: function(module, method, message, data, logLevel){
            var log = '[ ' + module + ' ][ ' + method + ' ] ' + message;
            /**DebugBegin**/
                var date = new Date();
            var consoleLog = '[' + uc.util.TimeUtil.getMessageTimeString(date.getTime(), uc.constants.UI.TimeFormat.Message.YearMonthDayHourMinuteSecond, uc.util.TimeUtil.getLocalTimeZone()) + ']' + log;
            switch (logLevel){
                case uc.constants.UI.LogLevel.Info:
                    if(data){
                        console.info(consoleLog, data);
                    }else{
                        console.info(consoleLog);
                    }
                    break;
                case uc.constants.UI.LogLevel.Warning:
                    if(data){
                        console.warn(consoleLog, data);
                    }else{
                        console.warn(consoleLog);
                    }
                    break;
                case uc.constants.UI.LogLevel.Error:
                    if(data){
                        console.error(consoleLog, data);
                    }else{
                        console.error(consoleLog);
                    }
                    break;
            }
            /**DebugEnd**/
            if(data){
                log += JSON.stringify(data);
            }
            ucClient.Log.writeLog(this._getWindowName() + '[HTML]', 1, logLevel, log);
        },
        _getWindowName: function(){
            if(this.windowNameRetrived){
                return this.windowName;
            }
            var index = location.pathname.lastIndexOf('/');
            if(index != -1){
                this.windowName = '[' +  location.pathname.substr(index + 1) + ']';
            }else{
                this.windowName = "";
            }
            this.windowNameRetrived = true;
            return this.windowName;
        }
    }
})(uc);