/**
 * Created by senlin.ouyang on 2015/7/21.
 */
(function(uc){
    var _this,flag;
    uc.modules.setting.Setting = {
        init: function() {
            _this = this;
            this.hasNewVersion = uc.util.StorageUtil.getData('needUpdate') ?  1 : 0;
            this.bindEvent();
            this.initUI();
        },
        initUI: function() {
            this.defaultSettings = JSON.stringify(uc.settings);

            _this.getLoginInfo();
            _this.restSecurity();
            _this.restDays();
            _this.setCapability();
            _this.setCloudButton();
            _this.getClientVersion();
            _this.showNewVersion();

            uc.IClientService.getConfigInfo("bee", "settings", function(args) {
                uc.util.WindowUtil.show();

                var str = args[1];
                var obj = JSON.parse(str);

                if (args[0] != 0) {
                    _this.saveSetting();
                    return;
                }

                if (obj != null) {
                    uc.settings = $.extend(uc.settings,obj);
                    _this.initUIbySettingInfo();
                }
            });
 		uc.IClientService.getUserNotifyStatus();
        },
        bindEvent: function(){
            var _mySelf = this;
            var settingContent = $(".setting-content").children();
            var keyboardScreen = $(".keyboard-screen");
            var keyboardScreenInput = keyboardScreen.find(".input");
            var shortCutSetting = $(".shortcut-setting");
            var keyBoardOpen = $(".keyboard-open");
            var keyBoardOpenInput = keyBoardOpen.find(".input");
            var feedbackContentDom = $(".feedback-content");

            // 关闭窗口
            $('.icon-close').on("click", function(){
                uc.util.WindowUtil.close();
            });

            // 左侧菜单
            $(".setting-menu").on("click", "li", function(){
                var _self = $(this);
                var index = _self.index();

                _self.addClass("selected");
                _self.siblings().removeClass("selected");

                settingContent.addClass("hide");
                settingContent.eq(index).removeClass("hide");
            });

            // 退出登录
            $("#quit-login").on("click", function() {
                uc.IUIService.bindCrossWindowEvent("Setting:Logout-true", function(eventData){
                    if(eventData.isCallEnd){
                        uc.modules.component.PromptMessageBox.open(
                            {
                                title: uc_resource.WindowTitle.Notification,
                                message: uc_resource.Setting.LogoutTips,
                                buttonMode: "both",
                                status: "Red",
                                CancelText: uc_resource.Common.Cancel,
                                OkText: uc_resource.Common.OK
                            }, function(result) {
                                if (result === 0) {
                                    uc.IUIService.triggerCrossWindowEvent("Setting:Logout", {});
                                    this.close();
                                } else if (result === 1) {
                                    this.close();
                                }
                        });
                    }else{
                        uc.IUIService.triggerCrossWindowEvent("Setting:Logout", {});
                        uc.util.WindowUtil.close();
                    }
                });
                uc.IUIService.triggerCrossWindowEvent("Setting:Logout-CallEnd", {});
            });

            // 修改密码
            $(".reset-password").on("click", function(){
                var options = {
                    width: 546,
                    height: 500,
                    refreshIfExist: false,
                    centerScreen: true,
                    modal: true,
                    resizable: false,
                    isTop: false,
                    caption: [{
                        left: 20,
                        top: 20,
                        width: 400,
                        height: 20
                    }]
                };
                uc.util.WindowUtil.open('page/resetPassword.html', 'resetPassword', options);
 	 });

            //登录消息提醒设置
            $(".login-tips").on("click", "label", function(){
                var loginTips = $(".login-tips").find("input");

                var setLoginTips = loginTips.prop("checked");

                loginTips.prop("checked",!setLoginTips);

                var lastSetLoginTips = Number(!setLoginTips);

                uc.IClientService.setUpdateUserNotify(lastSetLoginTips);
            });

            // 自动启动
            $(".self-starting").on("click", "label", function(){
                var selfStart = $(".self-starting").find("input");
                uc.settings.autoRun = selfStart.prop("checked");

                if(uc.settings.autoRun){
                    uc.settings.autoRun = false;

                    selfStart.prop("checked", false);

                    uc.IClientService.disableAutoStart();
                }else{
                    uc.settings.autoRun = true;

                    selfStart.prop("checked", true);

                    uc.IClientService.enableAutoStart();
                }
                _this.saveSetting();
            });

            // 窗口最顶层
            $(".set-on-top").on("click", "label", function(){
                var selfOnTop = $(".set-on-top").find("input");
                uc.settings.topEst = selfOnTop.prop("checked");

                if(uc.settings.topEst){
                    selfOnTop.prop("checked", false);
                    uc.settings.topEst = false;
                }else{
                    selfOnTop.prop("checked", true);
                    uc.settings.topEst = true;
                }

                _this.saveSetting();
            });

            // 消息提示音
            $(".voice-prompts").on("click", "label", function(){
                    var voicePrompts = $(".voice-prompts").find("input");
                    uc.settings.msgPrompt.playPrompt = voicePrompts.prop("checked");

                    if(uc.settings.msgPrompt.playPrompt){
                        uc.settings.msgPrompt.playPrompt = false;
                        voicePrompts.prop("checked", false);
                    }else{
                        uc.settings.msgPrompt.playPrompt = true;
                        voicePrompts.prop("checked", true);
                    }

                    _this.saveSetting();
                }
            );

            // 新消息弹出气泡提示
            $(".bubble").on("click", "label", function(){
                    var bubble = $(".bubble").find("input");
                    uc.settings.bubbleSet = bubble.prop("checked");

                    if(uc.settings.bubbleSet){
                        bubble.prop("checked", false);
                        uc.settings.bubbleSet = false;
                    }else{
                        bubble.prop("checked", true);
                        uc.settings.bubbleSet = true;
                    }
                    _this.saveSetting();
                }
            );

            // 通用菜单中的恢复默认设置
            $(".general-setting").on("click", ".default", function(){
                var message = uc_resource.Setting.ConfirmDefault;

                uc.modules.component.PromptMessageBox.open(
                    {
                        title: uc_resource.Profile.Prompt,
                        message: message,
                        buttonMode: "both",
                        OkText: uc_resource.Common.OK,
                        CancelText: uc_resource.Common.Cancel,
                        status: "Red"
                    }, function (result) {
                        if (result == 0) {
                            var defaultSetting = JSON.parse(_this.defaultSettings);

                            uc.settings.autoRun = defaultSetting.autoRun;
                            uc.settings.topEst = defaultSetting.topEst;
                            uc.settings.msgPrompt = defaultSetting.msgPrompt;
                            uc.settings.bubbleSet = defaultSetting.bubbleSet;

                            uc.IClientService.enableAutoStart();
                            _this.saveSetting();

                            flag = true;
                            _this.isAutoStart();
                            _this._setOnTopBtn();
                            _this._voicePromptsBtn();
                            _this.selectVoice();
                            _this._bubbleSet();
                        }
                    this.close();
                });
            });

            // 打开截图快捷键设置页面
            shortCutSetting.on("click", ".screen", function(){
                var str = _this.getKeyStr(uc.settings.skScreenshot);
                var op = JSON.stringify(uc.settings.skScreenshot);

                keyboardScreenInput.text(str);
                keyboardScreenInput.attr("option", op);

                uc.IUIService.triggerCrossWindowEvent(
                    uc.constants.UI.CrossWindowEventName.OnSettingChanged,
                    {
                        action: "disableHotKey"
                    }
                );

                keyboardScreen.show();
            });

            // 取消设置截图快捷键
            keyboardScreen.on("click", ".key-cancel", function(){
                _mySelf.getSettings(function(settings){
                    uc.settings = settings;
                    _mySelf.saveSetting();
                });

                keyboardScreen.hide();
            });

            // 确定设置截图快捷键
            keyboardScreen.on("click", ".key-confirm", function(){ //设置截屏快捷键OK
                var op = keyboardScreenInput.attr("option");
                var obj = JSON.parse(op);
                var str = _this.getKeyStr(uc.settings.skScreenshot);
                var str1 = $("#set-mainPage").text();
                str = str.replace(/ /g, "");
                str1 = str1.replace(/ /g, "");


                if (obj.keyCode == null) {
                    keyboardScreenInput.text(uc_resource.Setting.IllegalShortcuts);
                    return;
                }

                uc.settings.skScreenshot.ctrlKey = obj.ctrlKey;
                uc.settings.skScreenshot.shiftKey = obj.shiftKey;
                uc.settings.skScreenshot.altKey = obj.altKey;
                uc.settings.skScreenshot.keyCode = obj.keyCode;

                if(str == str1){
                    uc.modules.component.PromptMessageBox.open({
                        title: uc_resource.WindowTitle.Notification,
                        message: uc_resource.Setting.ConflictOpen,
                        buttonMode: "ok",
                        OkText: uc_resource.Common.OK
                    }, function (result) {
                        if (result === 0) {
                            this.close();
                        }
                    });
                    return false;
                }

                _this.saveSetting();

                _this.shortCutSetting();
                keyboardScreen.fadeOut();
            });

            // 打开主页面快捷键设置页面
            shortCutSetting.on("click", ".main-interface", function(){
                var str = _this.getKeyStr(uc.settings.skOpenWindow);
                var op = JSON.stringify(uc.settings.skOpenWindow);

                keyBoardOpenInput.text(str);
                keyBoardOpenInput.attr("option", op);

                uc.IUIService.triggerCrossWindowEvent(
                    uc.constants.UI.CrossWindowEventName.OnSettingChanged,
                    {
                        action: "disableHotKey"
                    }
                );

                keyBoardOpen.show();
            });

            // 快捷键设置
            document.onkeydown = function(e) {
                if(e.keyCode == 27){
                    if(keyboardScreen.is(":visible")){
                        keyboardScreen.hide();
                        return;
                    }

                    if(keyboardScreen.is(":visible")){
                        keyBoardOpen.hide();
                        return;
                    }

                    if($("#modalWin").is(":visible")){
                        uc.modules.component.PromptMessageBox.close();
                        return;
                    }

                    uc.util.WindowUtil.close();
                    return;
                }

                if(e.keyCode == 13 && keyboardScreen.is(":visible")){
                    $(".key-confirm", keyboardScreen).click();
                }else if(e.keyCode == 13 && keyBoardOpen.is(":visible")){
                    $(".key-confirm", keyBoardOpen).click();
                }

                if(!keyboardScreen.is(":visible") && !keyBoardOpen.is(":visible")){
                    return;
                }

                var keyOption = {
                    ctrlKey: false,
                    altKey: false,
                    shiftKey: false,
                    keyCode: null
                };

                if(e.ctrlKey){
                    keyOption.ctrlKey = true;
                }
                if(e.altKey){
                    keyOption.altKey = true;
                }
                if(e.shiftKey){
                    keyOption.shiftKey = true;
                }
                if(e.which != 16 && e.which != 17 && e.which != 18){
                    keyOption.keyCode = e.which;
                }

                var str = _this.getKeyStr(keyOption);
                var op = JSON.stringify(keyOption);

                if(keyboardScreen.is(":visible")){
                    $('.input', keyboardScreen).text(str);
                    $('.input', keyboardScreen).attr("option", op);
                    return;
                }

                if(keyBoardOpen.is(":visible")){
                    $('.input', keyBoardOpen).text(str);
                    $('.input', keyBoardOpen).attr("option", op);
                }
            };

            // 设置打开窗口快捷键Cancel
            keyBoardOpen.on("click", ".key-cancel", function(){
                _mySelf.getSettings(function(settings){
                    uc.settings = settings;
                    _mySelf.saveSetting();
                });

                keyBoardOpen.hide();
            });

            // 设置打开窗口快捷键OK
            keyBoardOpen.on("click", ".key-confirm", function(){
                var op = keyBoardOpenInput.attr("option");
                var obj = JSON.parse(op);
                var str = _this.getKeyStr(uc.settings.skOpenWindow);
                var str2 = $("#set-screen").text();
                str = str.replace(/ /g, "");
                str2 = str2.replace(/ /g, "");

                if (obj.keyCode == null) {
                    keyBoardOpenInput.text(uc_resource.Setting.IllegalShortcuts);
                    return;
                }

                uc.settings.skOpenWindow.ctrlKey = obj.ctrlKey;
                uc.settings.skOpenWindow.shiftKey = obj.shiftKey;
                uc.settings.skOpenWindow.altKey = obj.altKey;
                uc.settings.skOpenWindow.keyCode = obj.keyCode;

                if(str == str2){
                    uc.modules.component.PromptMessageBox.open({
                        title: uc_resource.WindowTitle.Notification,
                        message: uc_resource.Setting.ConflictScreen,
                        buttonMode: "ok",
                        OkText: uc_resource.Common.OK
                    }, function (result) {
                        if (result === 0) {
                            this.close();
                        }
                    });
                    return false;
                }

                _this.saveSetting();

                _this.shortCutSetting();
                keyBoardOpen.fadeOut();
            });

            // 快捷键菜单中的恢复默认设置
            shortCutSetting.on("click", '.default', function(){
                var message = uc_resource.Setting.ConfirmDefault;

                uc.modules.component.PromptMessageBox.open(
                    {
                        title: uc_resource.Profile.Prompt,
                        message: message,
                        buttonMode: "both",
                        OkText: uc_resource.Common.OK,
                        CancelText: uc_resource.Common.Cancel,
                        status: "Red"
                    }, function (result) {
                        if (result == 0) {
                            var defaultSetting = JSON.parse(_this.defaultSettings);

                            uc.settings.skMsg = defaultSetting.skMsg;
                            uc.settings.skScreenshot.ctrlKey = defaultSetting.skScreenshot.ctrlKey;
                            uc.settings.skScreenshot.shiftKey = defaultSetting.skScreenshot.shiftKey;
                            uc.settings.skScreenshot.altKey = defaultSetting.skScreenshot.altKey;
                            uc.settings.skScreenshot.keyCode = defaultSetting.skScreenshot.keyCode;
                            uc.settings.skOpenWindow.ctrlKey = defaultSetting.skOpenWindow.ctrlKey;
                            uc.settings.skOpenWindow.shiftKey = defaultSetting.skOpenWindow.shiftKey;
                            uc.settings.skOpenWindow.altKey = defaultSetting.skOpenWindow.altKey;
                            uc.settings.skOpenWindow.keyCode = defaultSetting.skOpenWindow.keyCode;

                            _this.saveSetting();

                            _this.selectMessage();
                            _this.shortCutSetting();
                        }
                    this.close();
                });
            });

            // 功能菜单中的恢复默认设置
            $(".function-setting").on("click", '.default', function(){
                var message = uc_resource.Setting.ConfirmDefault;

                uc.modules.component.PromptMessageBox.open(
                    {
                        title: uc_resource.Profile.Prompt,
                        message: message,
                        buttonMode: "both",
                        OkText: uc_resource.Common.OK,
                        CancelText: uc_resource.Common.Cancel,
                        status: "Red"
                    }, function (result) {
                        if (result == 0) {
                            var defaultSetting = JSON.parse(_this.defaultSettings);

                            uc.settings.meeting.meetingDuration = defaultSetting.meeting.meetingDuration;

                            _this.saveSetting();

                            _this.selectMeetingTime();
                        }
                    this.close();
                });
            });

            uc.IUIService.bindCrossWindowEvent("CloudLoginSuccess", function() {
                $('.emptyCloud').show();
            });

            // 云盘设置
            $(".cloudSetting").on("click", function(){
                var options = {
                    width: 660,
                    height: 548,
                    refreshIfExist: false,
                    centerScreen: true,
                    modal: true,
                    resizable: false,
                    isTop:false,
                    caption: uc.constants.UI.DragCaption.CloudSetting
                };
                uc.util.WindowUtil.open('page/cloudSetting.html', 'cloudSetting', options);
            });

            // 音频检测
            $(".AVDetectionSetting").on("click", function(){
                var options = {
                    width: 460,
                    height: 675,
                    refreshIfExist: false,
                    centerScreen: true,
                    modal: true,
                    resizable: false,
                    isTop:false,
                    caption: uc.constants.UI.DragCaption.CloudSetting
                };
                uc.util.WindowUtil.open('page/AVdetection.html','AVdetectionWindow', options);
            });

            // 点击上传日志按钮
            $(".upload-log").on("click", "label", function(){
                var uploadLogInput = $(".upload-log").find("input");
                var checked = uploadLogInput.prop("checked");

                if(checked){
                    uploadLogInput.prop("checked", false);
                }else{
                    uploadLogInput.prop("checked", true);
                }
            });

            // 输入问题反馈
            feedbackContentDom.on("keyup", "textarea", function(){
                var inputNumDom = $(".input-number");
                var feedbackContent = $("#desc").val();
                var feedbackLen = feedbackContent.length;

                if(feedbackLen){
                    inputNumDom.find(".current-number").text(feedbackLen);
                    inputNumDom.removeClass("hide");
                }else{
                    inputNumDom.addClass("hide");
                }
            });

            // 提交问题反馈
            feedbackContentDom.on("click", ".submit", function(){
                var feedbackContent = $("#desc").val();
                var feedbackLen = feedbackContent.length;
                var isUploadLog = $(".upload-log").find("input").prop("checked");

                if(!feedbackLen){
                    return;
                }

                feedbackContentDom.append("<div id='setting-msg'><div class='status-msg on-submitting'><img src='../images/settings/loading_big.png' /><span>"+uc_resource.Setting.Submitting +"</span></div></div>");

                setTimeout(function(){
                    if(isUploadLog){
                        uc.util.LogUtil.info("Setting", "archiveLogFile", "ArchiveLogFile start here", null);

                        uc.IClientService.archiveLogFile();
                    }else{
                        uc.util.LogUtil.info("Setting", "setTimeout", "getClientVersion", null);

                        uc.IClientService.getClientVersion(function(args){
                            uc.util.LogUtil.info("Setting", "getClientVersion", "args:", args);

                            var version = args[1];

                            uc.util.LogUtil.info("Setting", "setTimeout", "uploadLog start here", null);
                            uc.IClientService.uploadLog("", feedbackContent, version);
                        });
                    }
                }, 1);
            });


            // 检查版本
            $(".check-version").on("click",function(){
                var options = {
                    width: 562,
                    height: 216,
                    refreshIfExist: false,
                    centerScreen: true,
                    modal: true,
                    resizable: false,
                    isTop:false,
                    caption: [{
                        left: 20,
                        top: 20,
                        width: 500,
                        height: 20
                    }]
                };
                uc.util.WindowUtil.open('page/checkForUpdates.html','checkForUpdates',options);
            });

            // 服务条款
            $(".service-terms").on("click",function(){
                var options = {
                    width: 562,
                    height: 687,
                    refreshIfExist: false,
                    centerScreen: true,
                    modal: true,
                    resizable: false,
                    isTop:false,
                    isChild:true,
                    caption: [{
                        left: 20,
                        top: 20,
                        width: 500,
                        height: 200
                    }]
                };
                uc.util.WindowUtil.open('page/termsOfService.html', 'termsOfService', options);
            });
        },
        getLoginInfo: function() {
            var user = uc.IContactService.getCurrentUserInfo();

            if(user){
                uc.util.LogUtil.info("Setting", "initUi", "getContactInfo", {contactInfo: user});

                $("#quit-name").text(user.displayName);
                $("#quit-account").text(user.account);

                if (user.localAvatar) {
                    $("#quit-avatar").attr("src", user.localAvatar);
                } else if (user.remoteAvatarUrl) {
                    uc.util.LogUtil.info("Setting", "initUi", "downloadUserAvatar",
                        {
                            userId: user.userId,
                            remoteAvatarUrl: user.remoteAvatarUrl
                        }
                    );

                    uc.IUIService.downloadUserAvatar(user.userId, user.remoteAvatarUrl);

                    $("#quit-avatar").attr("src", user.remoteAvatarUrl);
                } else {
                    $("#quit-avatar").attr("src", "../images/avatar_default.png");
                }
            }
        },
        restSecurity: function() {
            uc.IClientService.getPasswordRule(function(args){
                uc.util.LogUtil.info("Setting", "restSecurity", "data",
                    {
                        errCode: args[0],
                        days: args[1],
                        memory: args[2],
                        complex: args[3],
                        allowChangePassword: args[4]
                    }
                );

                if (! args[0]) {
                    var allowChangePassword = args[4];

                    if (allowChangePassword) {
                        $(".setting-menu").find(".safety").removeClass("hide");
                    }
                }
            });
        },
        restDays: function(){
            uc.IClientService.getPwdExpiredDays(function(args){
                uc.util.LogUtil.info("Setting", "password restDays", "data", {errCode:args[0], days:args[1]});

                var errorCode = args[0];
                var days = args[1];

                if(errorCode == 0){
                    if (days == 9999) {
                        var pwdSafe = $(".pwd-safe").find(".safety-setting");
                        pwdSafe.children("div:eq(0)").css("line-height", "32px");
                        pwdSafe.children('div:eq(1)').remove();
                    } else {
                        $("#rest-days").html(days);
                    }
                }
            });
        },
        setCapability: function() {
            if(uc.constants.Switch.OpenMultiVersion){
                var version = JSON.parse(uc.util.StorageUtil.getData("capability"));
                if (version) {
                    switch(version.im){
                        case "0":
                            break;
                    }

                    switch(version.cloud_disk_type){
                        case "1":
                            $(".function-setting").children(":eq(0)").hide();
                            break;
                    }
                }
            }
        },
        setCloudButton:function(){
            if(uc.util.StorageUtil.getData('cloudType') == 0 || uc.util.StorageUtil.getData('CloudLoginResult') != 0){
                $(".function-setting").children(":eq(1)").hide();
            }
        },
        getClientVersion: function(){
            uc.IClientService.getClientVersion(function(args){
                uc.util.LogUtil.info("Setting", "getClientVersion", "errorCode"+args[0]+"version"+args[1], null);

                var errorCode = args[0];
                var version = args[1];

                if(errorCode == 0){
                    $("#version").html(version);
                }else{
                    $("#version").html("");
                }
            });
        },
        showNewVersion: function () {
            if (this.hasNewVersion) {
                $(".about-bee").addClass('pop-tips');
                $('.check-version').addClass('pop-tips');
            } else {
                $(".about-bee").removeClass('pop-tips');
                $('.check-version').removeClass('pop-tips');
            }
        },
        saveSetting: function() {
            uc.IClientService.updateConfigInfo("bee", "settings", JSON.stringify(uc.settings), function(args){
                if (args[0] == 0) {
                    uc.IUIService.triggerCrossWindowEvent(
                        uc.constants.UI.CrossWindowEventName.OnSettingChanged,
                        {
                            action:"apply"
                        }
                    );
                }
            });
        },
        initUIbySettingInfo: function() {
            _this.isAutoStart();
            _this._setOnTopBtn();
            _this._voicePromptsBtn();
            _this.selectVoice();
            _this._bubbleSet();
            _this.selectMessage();
            _this.shortCutSetting();
            _this.selectMeetingTime();
        },
        isAutoStart:function(){
            var selfStartingInput = $(".self-starting").find('input');

            if(flag){
                selfStartingInput.prop("checked", uc.settings.autoRun);
            }else{
                uc.IClientService.isAutoStart(function(args){
                    var auto = args[0];

                    if(auto == 0){
                        uc.settings.autoRun = true;
                        selfStartingInput.prop("checked", true);
                    }else{
                        uc.settings.autoRun = false;
                        selfStartingInput.prop("checked", false);
                    }
                })
            }
        },
        _setOnTopBtn: function(){
            $(".set-on-top").find("input").prop("checked", uc.settings.topEst);
        },
        _voicePromptsBtn: function(){
            $(".voice-prompts").find("input").prop("checked", uc.settings.msgPrompt.playPrompt);
        },
        selectVoice:function(){
            new uc.modules.component.DropdownList($("div.select-voice"), {
                width: 110,
                items: uc_resource.Setting.SelectVoice,
                selectedByText: true,
                selectedValue: uc.settings.msgPrompt.name,
                onItemSelected: function (selectedData) {
                    var path = "media/" + selectedData.value;

                    uc.settings.msgPrompt.name = selectedData.value;

                    _this.saveSetting();

                    uc.util.AudioUtil.playSound(path, "msg-sound", function(args){
                        uc.util.LogUtil.info("Setting","selectVoice","path:"+path+",playSound:"+args[0],null);
                    });
                }
            });
        },
        _bubbleSet: function(){
            $(".bubble").find("input").prop("checked", uc.settings.bubbleSet);
        },
        selectMessage:function(){
             new uc.modules.component.DropdownList($("div.send-message"), {
                width: 110,
                items: uc_resource.Setting.SelectMessage,
                selectedByText: true,
                selectedValue: uc.settings.skMsg + "",
                 onItemSelected: function (selectedData) {
                    uc.settings.skMsg = selectedData.value;

                    _this.saveSetting();
                }
            });
        },
        shortCutSetting: function(){
            var shortCutSetting = $(".shortcut-setting");
            var str = _this.getKeyStr(uc.settings.skScreenshot);
            var str1 = _this.getKeyStr(uc.settings.skOpenWindow);
            str = str.replace(/ /g, "");
            str1 = str1.replace(/ /g, "");

            shortCutSetting.find(".screen").text(str);
            shortCutSetting.find(".main-interface").text(str1);
        },
        selectMeetingTime:function(){
            new uc.modules.component.DropdownList($("div.meeting-time"), {
                width: 110,
                items: uc_resource.Setting.SelectMeetingTime,
                selectedByText: true,
                selectedValue: uc.settings.meeting.meetingDuration + "",
                onItemSelected: function (selectedData) {
                    uc.settings.meeting.meetingDuration = selectedData.value;

                    _this.saveSetting();
                }
            });
         },
        setLoginTips:function(isNotify){
            var LoginTipsInput = $(".login-tips").find('input');
            if(isNotify == 1){
                LoginTipsInput.prop("checked", true);
            }else{
                LoginTipsInput.prop("checked", false);
                uc.util.LogUtil.info("Setting","setLoginTips","isNotify"+isNotify,null);
            }
        },
        getKeyStr: function(keyOption){
            var keyboardMap = {
                "48":"0",
                "49":"1",
                "50":"2",
                "51":"3",
                "52":"4",
                "53":"5",
                "54":"6",
                "55":"7",
                "56":"8",
                "57":"9",
                "65":"A",
                "66":"B",
                "67":"C",
                "68":"D",
                "69":"E",
                "70":"F",
                "71":"G",
                "72":"H",
                "73":"I",
                "74":"J",
                "75":"K",
                "76":"L",
                "77":"M",
                "78":"N",
                "79":"O",
                "80":"P",
                "81":"Q",
                "82":"R",
                "83":"S",
                "84":"T",
                "85":"U",
                "86":"V",
                "87":"W",
                "88":"X",
                "89":"Y",
                "90":"Z",
                "112":"F1",
                "113":"F2",
                "114":"F3",
                "115":"F4",
                "116":"F5",
                "117":"F6",
                "118":"F7",
                "119":"F8",
                "120":"F9",
                "121":"F10",
                "122":"F11",
                "123":"F12"
            };
            var keyStr = keyboardMap["" + keyOption.keyCode];
            var str = "";

            if (keyStr == null) {
                keyOption.keyCode = null;
                keyStr = "";
            }

            str +=  keyOption.ctrlKey ?  "Ctrl" : "";

            if (keyOption.altKey) {
                str += str.length > 0 ? " + Alt" : "Alt";
            }

            if (keyOption.shiftKey) {
                str += str.length > 0 ? " + Shift" : "Shift";
            }

            if (keyOption.keyCode != 16 && keyOption.keyCode != 17 && keyOption.keyCode != 18) {
                str += str.length > 0 ? " + " + keyStr : "" + keyStr;
            }

            return str;
        },
        OnArchiveLogFinished: function(filePath){
            if(filePath){
                uc.IFSService.uploadFile(filePath, function(args){
                    uc.util.LogUtil.info("Setting", "uploadFile", "filePath:" + filePath + " , details", args);

                    _this.taskId = args[1];
                    _this.logZipFilePath = filePath;
                });
            }else{
                // uc_resource.Feedback.FailedTip
                var feedbackContent = $(".feedback-content");

                feedbackContent.find("#setting-msg").remove();
                feedbackContent.append("<div id='setting-msg'></div><div class='status-msg failed'><img /><span>" + uc_resource.Feedback.SendFailed + "</span></div></div>");

                setTimeout(function(){
                    $(".feedback-content").find("#setting-msg").remove();
                }, 2000);
            }
        },
        OnFileUploading: function(errorCode, taskId, percent){

        },
        OnFileUploaded: function(errorCode, taskId, fileName, fileSize, operatorId, fid, operatorTime, downloadURL){
            if(taskId != this.taskId){
                return;
            }

            uc.util.FileUtil.deleteFile(this.logZipFilePath);

            if(errorCode == 0){
                var desc = $("#desc").val();
                var logURL =  downloadURL;

                uc.IClientService.getClientVersion(function(args){
                    uc.util.LogUtil.info("Setting","OnFileUploaded","getClientVersion",args);

                    var version = args[1];

                    uc.util.LogUtil.info("Setting","OnFileUploaded","uploadLog start here",null);
                    uc.IClientService.uploadLog(logURL, desc, version);
                });
            }else{
                // uc_resource.Feedback.NetworkProblem
                var feedbackContent = $(".feedback-content");

                feedbackContent.find("#setting-msg").remove();
                feedbackContent.append("<div id='setting-msg'><div class='status-msg failed'><img src='../images/settings/submit_failed.png' /><span>" + uc_resource.Feedback.SendFailed + "</span></div></div>");

                setTimeout(function(){
                    var statusMsg = $(".feedback-content").find("#setting-msg");
                    statusMsg.fadeOut();
                    statusMsg.remove();
                }, 2000);
            }
        },
        OnLogUploaded: function(errorCode){
            var feedbackContent = $(".feedback-content");
            feedbackContent.find("#setting-msg").remove();

            if (errorCode == 0) {
                $("#desc").val("");
                $(".input-number").addClass("hide");

                feedbackContent.append("<div id='setting-msg'><div class='status-msg success'><img src='../images/settings/submit_success.png' /><span>" + uc_resource.Feedback.Thanks + "</span></div></div>");

                setTimeout(function(){
                    var statusMsg = $(".feedback-content").find("#setting-msg");
                    statusMsg.fadeOut();
                    statusMsg.remove();
                }, 2000);
            } else {
                feedbackContent.append("<div id='setting-msg'><div class='status-msg failed'><img src='../images/settings/submit_failed.png' /><span>" + uc_resource.Feedback.SendFailed + "</span></div></div>");

                setTimeout(function(){
                    var statusMsg = $(".feedback-content").find("#setting-msg");
                    statusMsg.fadeOut();
                    statusMsg.remove();
                }, 2000);
            }
        },
        getSettings: function(callback) {
            uc.IClientService.getConfigInfo("bee", "settings", function (args){
                var str = args[1];
                var obj = JSON.parse(str);

                if (args[0] != 0) {
                    _this.saveSetting();
                    return;
                }

                if (obj != null) {
                    callback(obj);
                }
            });
        },
	 OnUpdateUserNotifySet:function(errorCode, userId){
            uc.util.LogUtil.info("Setting","OnUpdateUserNotifySet","errorCode"+errorCode+",userId"+userId,null);
            if(errorCode != 0){
                uc.modules.component.PromptMessageBox.open({
                    title: uc_resource.WindowTitle.Notification,
                    message: uc_resource.Setting.SetLoginTipsFailed,
                    buttonMode: "ok",
                    OkText: uc_resource.Common.Known
                }, function () {
                    this.close();
                    var loginTips = $(".login-tips").find("input");
                    var setLoginTips = loginTips.prop("checked");
                    loginTips.prop("checked",!setLoginTips);
                });
            }
        },
        OnGetUserNotifyStatus:function(errorCode,isNotify){
            uc.util.LogUtil.info("Setting","OnGetUserNotifyStatus","errorCode"+errorCode+",isNotify"+isNotify,null);
            if(errorCode == 0){
                _this.setLoginTips(isNotify);
            }
        }
        //selectTimeZone:function(){
        //    new uc.modules.component.DropdownList($("div.time-zone"), {
        //        width: 280,
        //        items: uc_resource.Meeting.CreateMeetingTimeZone,
        //        selectedByText: true,
        //        selectedValue: uc.settings.meeting.timeZone.name,
        //        saveData: function (selectedData) {
        //            uc.settings.meeting.timeZone.name = selectedData.text;
        //            uc.settings.meeting.timeZone.offset = selectedData.value;
        //            _this.saveSetting();
        //        }
        //    });
        //},
        //selectDate:function(){
        //    var timeData = uc.util.TimeUtil._unixTimeToTimeData(new Date().getTime(), 0);
        //    var items = [
        //        { text: timeData.year + "/" + timeData.month + "/" + timeData.day},
        //        { text: timeData.day + "/" + timeData.month + "/" + timeData.year},
        //        { text: timeData.month + "/" + timeData.day + "/" + timeData.year},
        //        {
        //          text: timeData.year + uc_resource.Meeting.Year  + timeData.month + uc_resource.Meeting.Month +
        //                  timeData.day + uc_resource.Meeting.Day
        //        }
        //    ];
        //    new uc.modules.component.DropdownList($("div.date"), {
        //        width: 140,
        //        items: items,
        //        selectedByText: true,
        //        selectedValue: uc.settings.meeting.dateFormat,
        //        saveData: function (selectedData) {
        //            uc.settings.meeting.dateFormat = selectedData.text;
        //            _this.saveSetting();
        //        }
        //
        //    });
        //},
        //selectTime:function(){
        //    new uc.modules.component.DropdownList($("div.time"), {
        //        width: 110,
        //        items: uc_resource.Setting.SelectTime,
        //        selectedByText: true,
        //        selectedValue: uc.settings.meeting.timeFormat,
        //        saveData: function (selectedData) {
        //            uc.settings.meeting.timeFormat = selectedData.value;
        //            _this.saveSetting();
        //        }
        //
        //    });
        //},
        //selectTimeInterval:function(){
        //    new uc.modules.component.DropdownList($("div.time-interval"), {
        //        width: 110,
        //        items: uc_resource.Setting.SelectTimeInterval,
        //        selectedByText: true,
        //        selectedValue: uc.settings.meeting.timeInterval,
        //        saveData: function (selectedData) {
        //            uc.settings.meeting.timeInterval = selectedData.value;
        //            _this.saveSetting();
        //        }
        //
        //    });
        //},
    };

    jQuery(document).ready(function () {
        uc.init();
        uc.modules.setting.Setting.init();
    })
})(uc);