/**
 * Created by waouyang on 15/9/24.
 */
(function(){
    uc.modules.setting.SettingManager = {
        loadAndApplySetting: function(){
            var _this = this;
            uc.IClientService.getConfigInfo("bee", "settings",
                function(args) {
                    var mode = 0;

                    uc.util.LogUtil.info("SettingManager","getConfigInfo","errorCode:"+args[0],{settingData:args[1]});

                    if(args[0] != 0){
                        _this.saveSetting();
                    }else{
                        var str = args[1];
                        var obj = JSON.parse(str);

                        if (obj != null) {
                            if(obj.meeting.timeZone.offset != parseInt(new Date().getTimezoneOffset() / 60) * (-1)){
                                var i = 0;

                                _this.saveSetting();

                                obj.meeting.timeZone.offset = parseInt(new Date().getTimezoneOffset() / 60) * (-1);

                                while(i < uc_resource.Meeting.CreateMeetingTimeZone.length){
                                    if(uc_resource.Meeting.CreateMeetingTimeZone[i].value == (-1) * parseInt(new Date().getTimezoneOffset() / 60)){
                                        obj.meeting.timeZone.name=uc_resource.Meeting.CreateMeetingTimeZone[i].text;
                                        break;
                                    }else{
                                        i++;
                                    }
                                }
                            }

                            if(obj.bubbleSet == undefined){
                                obj.bubbleSet = true;
                            }

                            uc.settings = $.extend(uc.settings,obj);
                            _this.saveSetting();
                        }
                    }
                    uc.IClientService.isAutoStart(function(args){
                        var auto = args[0];

                        if(auto == 0){
                            uc.IClientService.enableAutoStart();
                        }else {
                            uc.IClientService.disableAutoStart();
                        }
                    });

                    uc.util.WindowUtil.setOnTop(uc.settings.topEst);

                    mode |= uc.settings.skScreenshot.altKey ?  uc.constants.UI.HotKey.Alt : 0;
                    mode |= uc.settings.skScreenshot.shiftKey ?  uc.constants.UI.HotKey.Shift : 0;
                    mode |= uc.settings.skScreenshot.ctrlKey ?  uc.constants.UI.HotKey.Control : 0;
                    uc.util.SystemUtil.registerHotKey(0,mode,uc.settings.skScreenshot.keyCode);

                    mode = 0;
                    mode |= uc.settings.skOpenWindow.altKey ?  uc.constants.UI.HotKey.Alt : 0;
                    mode |= uc.settings.skOpenWindow.shiftKey ?  uc.constants.UI.HotKey.Shift : 0;
                    mode |= uc.settings.skOpenWindow.ctrlKey ?  uc.constants.UI.HotKey.Control : 0;
                    uc.util.SystemUtil.registerHotKey(1,mode,uc.settings.skOpenWindow.keyCode);

                    uc.modules.chat.ChatManager.updateInputBoxNotice();
                }
            );
        },
        saveSetting: function(){
            uc.IClientService.updateConfigInfo("bee", "settings", JSON.stringify(uc.settings));
        },
        getFontSettingFormat: function() {
            if (uc.settings.textMsgStyle == null) {
                return ;
            }

            var b = uc.settings.textMsgStyle.bold ? "bold;" : ";";
            var i =  uc.settings.textMsgStyle.italic ? "italic;" : ";";
            var u =  uc.settings.textMsgStyle.underline ? "underline;" : ";";

            return "font-size: " + uc.settings.textMsgStyle.fontSize + "px; font-weight: " + b + " font-style: " + i + " text-decoration: " + u;
        }
    }
})(uc);