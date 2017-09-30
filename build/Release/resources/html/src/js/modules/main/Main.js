/**
 * Created by senlin.ouyang on 2015/7/20.
 */
(function(uc){
    var _this = null;
    uc.modules.main.Main = {
        init: function () {
            _this = this;
	        this.moduleId =  uc.constants.UI.ModuleId.Message;
            this.loginUserInfo = uc.IContactService.getCurrentUserFullInfo();
            this.reloginTimer = -1;
            this.removeConnectedStatusTimer = -1;
            this.maxReloginTime = 3;
            this.currentUserStatusValue = null;


            this.initSystemTray();

            this.initLoginStatus();
            this.initSetting();
            this.initUI();
            this.bindUIEvent();
            this.bindCrossWindowEvent();
            this.bindNativeEvent();
            this.initModules(false);
            uc.IUIService.triggerCrossWindowEvent(uc.constants.UI.CrossWindowEventName.OnSettingChanged,{data:""});
            this.checkDate();
            this.bindCloudEvent();
            this.checkPbxLogin();
            this.checkSendFile();
            uc.modules.pbx.PbxManager.init();
        },
        checkDate: function () {
            var self = this;
            //uc.IUpdateService.checkUpdates();//初始加载时，从storage中获取更新信息
            var updateFlag = uc.util.StorageUtil.getData('checkUpdates') ? JSON.parse(uc.util.StorageUtil.getData('checkUpdates')) : {errorCode: 0, updateFlag: 0};
            this.OnCheckUpdates(updateFlag.errorCode, updateFlag.updateFlag);

            this.loginDateStr = uc.util.TimeUtil.getMessageTimeString(new Date(), uc.constants.UI.TimeFormat.Message.YearMonthDay, uc.settings.meeting.timeZone.offset);
            setInterval(function () {
                uc.IUpdateService.checkUpdates();
                var nowDateStr = uc.util.TimeUtil.getMessageTimeString(new Date(), uc.constants.UI.TimeFormat.Message.YearMonthDay, uc.settings.meeting.timeZone.offset);
                //console.log("%ccheckDate  setInterval", 'color:red', self.loginDateStr, nowDateStr, new Date().toString());
                    uc.IClientService.getConfigInfo("bee", "settings",
                        function (args) {
                            if (args[0] != 0) {
                                uc.modules.setting.SettingManager.saveSetting();
                                return;
                            }
                            var str = args[1];
                            var obj = JSON.parse(str);
                            if (obj != null) {
                                if(obj.meeting.timeZone.offset!=parseInt(new Date().getTimezoneOffset()/60)*(-1)){
                                    obj.meeting.timeZone.offset=parseInt(new Date().getTimezoneOffset()/60)*(-1);
                                    var i=0;
                                    while(i<uc_resource.Meeting.CreateMeetingTimeZone.length){
                                        if(uc_resource.Meeting.CreateMeetingTimeZone[i].value==(-1)*parseInt(new Date().getTimezoneOffset()/60)){
                                            obj.meeting.timeZone.name=uc_resource.Meeting.CreateMeetingTimeZone[i].text;
                                            break;
                                        }else{
                                            i++;
                                        }
                                    }
                                    uc.settings = $.extend(uc.settings,obj)
                                    uc.modules.setting.SettingManager.saveSetting();
                                    uc.IUIEvent.OnDateChanged();
                                }
                            }
                        });
                if (self.loginDateStr != nowDateStr) {
                    self.loginDateStr = nowDateStr;
                    uc.IUIEvent.OnDateChanged();
                }
            }, 30 * 60 * 1000);
        },
        initSetting: function(){
            uc.modules.setting.SettingManager.loadAndApplySetting();
        },
        initLoginStatus: function(){
            this.networkStatus = uc.constants.NetworkStatus.OK;
            this.UCASStatus = uc.constants.UCASStatus.Connected;
            this.sessionStatus = uc.constants.SessionStatus.OK;
            this.clientStatus = uc.constants.ClientStatus.Connected;
            this.reloginTimeCount = 0;
        },
        initUI: function () {
            this.filecomment=$(".filecomment");
            this.profileImageNode = $('#menu-panel .profile img');
            this.currentUserStatus = $('#menu-panel .current-user-status');
            this.userStatusList = $("#menu-panel .current-user-status-list");
            this.newMsgCountNode = $('.main-menus .menu-message .new-msg-node');
            this.clientStatusContainer = $('.client-status-container');
            this.clientStatusTextNode = $('.client-status-text', this.clientStatusContainer);
            this.reConnectNode = $('.re-connect', this.clientStatusContainer);

            this.initProfile();
            this.initBubbleMessageWindow();
            uc.modules.component.MainMenuBar.init();
            uc.modules.component.TitleBarButton.init(uc.constants.UI.WindowCloseBehavior.Hide);
            uc.modules.component.ExtraMenuBar.init();
            uc.modules.emoji.EmojiConverter.init();
            uc.ui = {
                Message: {
                    ModuleId: uc.constants.UI.ModuleId.Message,
                    MainPanel: $('.menu-panel-body.message'),
                    MenuPanel: $('.menu-panel-body.message .sub-menu-panel'),
                    ContentPanel: $('.menu-panel-body.message  .content-panel'),
                    ContentPanelTitle: $('.menu-panel-body.message  .content-panel .content-panel-title'),
                    ContentPanelBody: $('.menu-panel-body.message  .content-panel .content-panel-body')
                },
                Contacts: {
                    ModuleId: uc.constants.UI.ModuleId.Contacts,
                    MainPanel: $('.menu-panel-body.contacts'),
                    MenuPanel: $('.menu-panel-body.contacts .sub-menu-panel'),
                    ContentPanelTitle: $('.menu-panel-body.contacts  .content-panel .content-panel-title'),
                    ContentPanelBody: $('.menu-panel-body.contacts .content-panel .content-panel-body')
                },
                Meeting: {
                    ModuleId: uc.constants.UI.ModuleId.Meeting,
                    MainPanel: $('.menu-panel-body.meeting'),
                    MeetingTitle:$('.menu-panel-body.meeting .meeting-common-title'),
                    MenuPanel: $('.menu-panel-body.meeting .sub-menu-panel'),
                    ContentPanel: $('.menu-panel-body.meeting  .content-panel'),
                    ContentPanelTitle: $('.menu-panel-body.meeting  .content-panel .content-panel-title'),
                    ContentPanelBody: $('.menu-panel-body.meeting .content-panel .content-panel-body')
                },
	            MeetingRoom: {
		            ModuleId: uc.constants.UI.ModuleId.MeetingRoom,
		            MainPanel: $('.menu-panel-body.meetingRoom'),
		            MenuPanel: $('.menu-panel-body.meetingRoom .sub-menu-panel'),
		            ContentPanelTitle: $('.menu-panel-body.meetingRoom  .content-panel .content-panel-title'),
		            ContentPanelBody: $('.menu-panel-body.meetingRoom .content-panel .content-panel-body')
	            },
                Link: {
                    ModuleId: uc.constants.UI.ModuleId.Link,
                    MainPanel: $('.menu-panel-body.link'),
                    ContentPanelTitle: $('.menu-panel-body.link .menu-panel-body-title'),
                    ContentPanelBody: $('.menu-panel-body.link .menu-panel-body-body')
                },
                App: {
                    ModuleId: uc.constants.UI.ModuleId.App,
                    MainPanel: $('.menu-panel-body.app'),
                    ContentPanelTitle: $('.menu-panel-body.app .menu-panel-body-title'),
                    ContentPanelBody: $('.menu-panel-body.app .menu-panel-body-body')
                },
                Search: {
                    ModuleId: uc.constants.UI.ModuleId.Search,
                    MainPanel: $('.search-container')
                },
                Me: {
                    ModuleId: uc.constants.UI.ModuleId.Me,
                    MainPanel: $('.menu-panel-body.me'),
                    ContentPanelTitle: $('.menu-panel-body.me .menu-panel-body-title'),
                    ContentPanelBody: $('.menu-panel-body.me .menu-panel-body-body ')
                },
		        Cloud: {
                    ModuleId: uc.constants.UI.ModuleId.Cloud,
                    MainPanel: $('.menu-panel-body.cloud')
                },
                Notice: {
                    MainPanel: $('.menu-panel-body.notice')
                },
                Pbx:{
                    ModuleId: uc.constants.UI.ModuleId.Pbx,
                    MainPanel: $('.menu-panel-body.pbx'),
                    MenuPanel: $('.menu-panel-body.pbx .sub-menu-panel'),
                    ContentPanelTitle: $('.menu-panel-body.pbx  .content-panel .content-panel-title'),
                    ContentPanelBody: $('.menu-panel-body.pbx .content-panel .content-panel-body')
                }
            };

            // added by xue.bai_2 on 2016/05/10
            var userIds = [this.loginUserInfo.userId];
            var copyBack = '{"panel": "Main"}';

            uc.util.LogUtil.info('Main', 'OnUCASLogin', 'getUserStatus', {userId: userIds, copyBack: copyBack});
            uc.IContactService.getUserStatus(userIds, copyBack, function(args){
                uc.util.LogUtil.info('Main', 'getUserStatus', 'response', {args: args});
            });
        },
        initModules: function(clearData){
            uc.IUIService.initClient(clearData);
        },
        bindCrossWindowEvent: function(){
            uc.modules.main.CrossWindowEventListener.bindCrossWindowEvent();
        },
        bindNativeEvent: function(){
            uc.modules.main.NativeEventListener.bindNativeEvent();
        },
        initSystemTray: function () {
            var fullPath = location.href;
            var htmlIndex = fullPath.indexOf('html');
            var iconPath = fullPath.substring(8, htmlIndex + 5) + "images\/bee.ico";
            iconPath = decodeURI(iconPath);
            var menu = {
                menu: [
                    {
                        id: 200,
                        label: uc_resource.Tray.Setting,
                        click: function () {
                            uc.IUIService.openSettingWindow();
                        }
                    },
                    {
                        id: 201,
                        label: uc_resource.Tray.Logout,
                        click: function () {
							_this.logoutClient();
							return;
                            uc.IMeetingService.isInMeeting(function (args) {
                                var inMeeting = args[1];
                                if (inMeeting) {
                                    uc.util.WindowUtil.open("page/noticeInMeeting.html?op=1", 'isInMeeting', {
                                        title:uc_resource.Meeting.MeetingReminderTitle,
                                        width: 370,
                                        height: 230,
                                        refreshIfExist: false,
                                        centerScreen: true,
                                        modal: false,
                                        resizable: false,
                                        isChild: false,
                                        isTop:true,
                                        caption: uc.constants.UI.DragCaption.AlarmedMeeting
                                    });
                                    return;
                                }
                                _this.logoutClient();
                            });
                        }
                    },
                    {
                        id: 202,
                        label: uc_resource.Tray.Quit,
                        click: function () {
							_this.quitClient();
							return;
                            uc.IMeetingService.isInMeeting(function (args) {
                                var inMeeting = args[1];
                                if (inMeeting) {
                                    uc.util.WindowUtil.open("page/noticeInMeeting.html?op=2", 'isInMeeting', {
                                        title:uc_resource.Meeting.MeetingReminderTitle,
                                        width: 370,
                                        height: 230,
                                        refreshIfExist: false,
                                        centerScreen: true,
                                        modal: false,
                                        resizable: false,
                                        isChild: false,
                                        isTop:true,
                                        caption: uc.constants.UI.DragCaption.AlarmedMeeting
                                    });
                                    return;
                                }
                                _this.quitClient();
                            });
                        }
                    }
                ]
            };
            uc.util.WindowUtil.setSystemTray(uc_resource.Common.AppName, iconPath, false, menu);
        },
        initProfile: function () {
            if (this.loginUserInfo) {
                var avatarUrl = this.loginUserInfo.localAvatar;
                if (avatarUrl) {
                    this.setSelfAvatar(avatarUrl);
                }else if(this.loginUserInfo.remoteAvatarUrl){
                    uc.IUIService.downloadUserAvatar(this.loginUserInfo.userId, this.loginUserInfo.remoteAvatarUrl);
                }
            }
        },
        initBubbleMessageWindow: function(){
            uc.modules.bubble.BubbleManager.init();
        },
        setSelfAvatar: function(localAvatarUrl){
            if(!localAvatarUrl){
                return;
            }
            uc.util.ImageUtil.OnImageLoadError(this.profileImageNode[0], uc.constants.UI.DefaultLocalUrl.Avatar_60);
            this.profileImageNode.attr('src', localAvatarUrl);
        },
        bindUIEvent: function () {
            $('body').on("dragstart", "img[class!='msg-img']", function (e) {
                e.stopPropagation();
                return false;
            });

            jQuery(window).resize(function () {
                uc.IUIEvent.OnWindowResized();
            });

            this.profileImageNode.click(function () {
                uc.IUIService.changeModule(uc.constants.UI.ModuleId.Me);
            });

            // start-----------------------added by xue.bai_2 on 2016/05/09-----------------------
            this.currentUserStatus.on("click", ".status-online", function(){
                _this.userStatusList.find(".status-inCall, .status-inMeeting").addClass("hide");
                _this.userStatusList.find(".status-list i").addClass("hide");
                _this.userStatusList.find(".status-online i").removeClass("hide");
                _this.userStatusList.removeClass("hide");
            });

            this.currentUserStatus.on("click", ".status-busy", function(){
                _this.userStatusList.find(".status-inCall, .status-inMeeting").addClass("hide");
                _this.userStatusList.find(".status-list i").addClass("hide");
                _this.userStatusList.find(".status-busy i").removeClass("hide");
                _this.userStatusList.removeClass("hide");
            });

            this.currentUserStatus.on("click", ".status-inCall", function(){
                _this.userStatusList.find(".status-online, .status-busy").addClass("disabled");
                _this.userStatusList.find(".status-inMeeting").addClass("hide");
                _this.userStatusList.find(".status-list i").addClass("hide");
                _this.userStatusList.find(".status-inCall i").removeClass("hide");
                _this.userStatusList.removeClass("hide");
            });

            this.currentUserStatus.on("click", ".status-inMeeting", function(){
                _this.userStatusList.find(".status-online, .status-busy").addClass("disabled");
                _this.userStatusList.find(".status-inCall").addClass("hide");
                _this.userStatusList.find(".status-list i").addClass("hide");
                _this.userStatusList.find(".status-inMeeting i").removeClass("hide");
                _this.userStatusList.removeClass("hide");
            });

            _this.userStatusList.on("click", ".status-list", function(e){
                var _self = $(this);
                var userId = _this.loginUserInfo.userId;
                var status = parseInt(_self.attr("data-value"));
                var show = _self.text();

                if(_self.hasClass("disabled")){
                    return false;
                }else{
                    if(_self.find("i").hasClass("hide")){// 被点击的item之前未选中
                        _this.userStatusList.find("i").addClass("hide");
                        _self.find("i").removeClass("hide");

                        uc.util.LogUtil.info('Main', 'bindUIEvent', 'setUserStatus', {userId: userId, status: status, show: show});
                        uc.IContactService.setUserStatus(userId, status, show);
                    }

                    _this.userStatusList.addClass("hide");
                }
            });
            // -----------------------added by xue.bai_2 on 2016/05/09-------------------------end

            document.ondragover= function(e) {
                e.preventDefault();
            };

            document.ondrop = function(e) {
                e.preventDefault();
            };

            this.reConnectNode.click(function(){
                _this.reLogin(false, true);
            });

            addEventListener("gkTransfer", function(arg){
                uc.IUIEvent.OnGkFileDownloadCallBack(arg);
            });

            $(document).click(function(e){
                _this.OnBodyClick(e);
            });
        },
        OnModuleChanged: function (moduleId) {
	        this.moduleId = moduleId;
            uc.modules.component.MainMenuBar.OnModuleChanged(moduleId);
            this.showModuleView(moduleId);
        },
        OnUserAvatarDownloaded: function(userId, serverAvatarUrl, localAvatarUrl){
            if(userId == this.loginUserInfo.userId){
                this.setSelfAvatar(localAvatarUrl);
            }
        },
        showModuleView: function (moduleId) {
            for (var module in uc.ui) {
                if (uc.ui[module].ModuleId == moduleId) {
                    uc.ui[module].MainPanel.removeClass('hide');
                } else {
                    uc.ui[module].MainPanel.addClass('hide');
                }
            }
            this.setDragCaption(moduleId);
        },
        setDragCaption: function (moduleId) {
            var dragCaption = [];
            switch (moduleId) {
                case uc.constants.UI.ModuleId.Message:
                    dragCaption = uc.constants.UI.DragCaption.Message;
                    break;
                case uc.constants.UI.ModuleId.Meeting:
                    dragCaption = uc.constants.UI.DragCaption.Meeting;
                    break;
                case uc.constants.UI.ModuleId.Contacts:
                    dragCaption = uc.constants.UI.DragCaption.Contacts;
                    break;
                case uc.constants.UI.ModuleId.App:
                    dragCaption = uc.constants.UI.DragCaption.App;
                    break;
                case uc.constants.UI.ModuleId.Me:
                    dragCaption = uc.constants.UI.DragCaption.Me;
                    break;
                case uc.constants.UI.ModuleId.Cloud:
                    dragCaption = uc.constants.UI.DragCaption.Cloud;
                    break;
                case uc.constants.UI.ModuleId.Notice:
                    dragCaption = uc.constants.UI.DragCaption.Notice;
                    break;
                case uc.constants.UI.ModuleId.Pbx:
                    dragCaption = uc.constants.UI.DragCaption.Pbx;
                    break;
            }
            if (dragCaption.length) {
                uc.util.WindowUtil.setCaptions(dragCaption);
            }
        },

        OnLogin: function(/*int*/errorCode, authType, pbxType, capability){
            uc.util.LogUtil.info('Main', 'OnLogin', 'errorCode = ' + errorCode+",authType="+authType+ ',capability='+ capability);
            if (capability) {
                uc.util.StorageUtil.setData('capability', capability);
                var capabilityObj = JSON.parse(capability);
                if(capabilityObj.cloud_disk_type){
                    uc.util.StorageUtil.setData('cloudType',capabilityObj.cloud_disk_type);
                }
            }
            if(errorCode == uc.ErrorCode.Success){
                uc.util.LogUtil.info('Main', 'OnLogin', 'Start to loginUCAS ', null);
                uc.IClientService.loginUCAS();
            }else{
            	if(errorCode == 407){
                	this._openProxyAuthenticateWindow(authType);
                }else if(errorCode == 10128 || errorCode == 10402 || errorCode == 20103) {
                	//If password changed in the back-end, to login page
                	this.logoutClient();
                } else if (errorCode == 10152) {
                    var options = {
                        title: uc_resource.Common.AppName,
                        width: 520,
                        height: 420,
                        refreshIfExist: false,
                        centerScreen: true,
                        modal: true,
                        resizable: false,
                        minWidth: 520,
                        minHeight: 420,
                        hasTaskBarIcon: true,
                        behaviorIfSCClose: uc.constants.UI.WindowCloseBehavior.Close,
                        isChild: false,
                        caption: uc.constants.UI.DragCaption.UpdateClient
                    };
                    uc.util.WindowUtil.open('page/update/update.html?type=1', 'update', options);
                }
                else {
                	this.reLogin(false);
                }
            }
        },
        _openProxyAuthenticateWindow: function(authType){
            uc.util.StorageUtil.setData('ProxyAuthenticateAuthType', authType);
            var options = {
                width: 360,
                height: 250,
                refreshIfExist: false,
                centerScreen: true,
                modal: true,
                resizable: false,
                minWidth: 360,
                minHeight: 250,
                hasTaskBarIcon: true,
                caption: uc.constants.UI.DragCaption.ProxyAuthenticate
            };
            uc.util.WindowUtil.open('page/proxyAuthenticate.html', 'proxyAuthenticate', options);
        },
        OnProxyAuthenticateSet: function(){
        	this.reLogin(false, true);
        },
        OnLogout: function (errorCode) {
        },
        OnUCASLogin: function(errorCode){
            uc.util.LogUtil.info('Main', 'OnUCASLogin', 'errorCode = ' + errorCode, null);
            if(errorCode == uc.ErrorCode.Success){
                this.initLoginStatus();
                this.setClientStatus(uc.constants.ClientStatus.Connected);
                this.initModules(true);

                // added by xue.bai_2 on 2016/05/10
                var userIds = [this.loginUserInfo.userId];
                var copyBack = '{"panel": "Main"}';

                uc.util.LogUtil.info('Main', 'OnUCASLogin', 'getUserStatus', {userId: userIds, copyBack: copyBack});
                uc.IContactService.getUserStatus(userIds, copyBack, function(args){
                    uc.util.LogUtil.info('Main', 'getUserStatus', 'response', {args: args});
                });
            }else{
            	this.setDisconnected();
            	this.reLogin(false);
            }
        },
        OnNetworkStatusChanged: function(status){
            uc.util.LogUtil.info('Main', 'OnNetworkStatusChanged', 'status = ' + status, null);
            this.networkStatus = status;
            if(this.networkStatus == uc.constants.NetworkStatus.Error){
                this.currentUserStatus.find("div").remove();
                this.setClientStatus(uc.constants.ClientStatus.Disconnected);
                clearTimeout(this.reloginTimer);
            }else{
                this.reLogin(true);
            }
        },
        OnUCASLogout: function (/*int*/errorCode) {
            uc.util.LogUtil.info('Main', 'OnUCASLogout', 'errorCode = ' + errorCode, null);
            if(errorCode == uc.ErrorCode.Success){
                return;
            }
            if(this.modifyingPassword && this.passwordModified){
                return;
            }
            this.setDisconnected();
            this.reLogin(false);
        },
        OnSessionExpired: function(){
            uc.util.LogUtil.info('Main', 'OnSessionExpired', 'Session is expired, need reLogin', null);
            this.sessionStatus = uc.constants.SessionStatus.Expired;
            this.UCASStatus = uc.constants.UCASStatus.Disconnected;
            this.setDisconnected();
            this.reLogin(false);
        },
        setDisconnected: function(){
        	if(this.reloginTimeCount >= this.maxReloginTime){
                this.setClientStatus(uc.constants.ClientStatus.ReConnectFailed);
            }else{
                this.setClientStatus(uc.constants.ClientStatus.Disconnected);
            }
        },
        OnPasswordModified: function(/*String*/errorCode){
            uc.util.LogUtil.info('Main', 'OnPasswordModified', 'errorCode = ' + errorCode, null);
            if(errorCode == uc.ErrorCode.Success){
                this.passwordModified = true;
                _this._openPasswordResetWindow();
            }
        },
        OnPasswordModifiedMessageReceived: function(){
            uc.util.LogUtil.info('Main', 'OnPasswordModifiedMessageReceived', '', null);
            if(this.modifyingPassword){
                return;
            }
            this._openPasswordModifiedWindow();
        },

        OnNewMessageCountChanged: function(count){
            var nodeClass = '';
            if(count < 0){
                count = 0;
            }
            if(count == 0){
                nodeClass = 'no-msg';
            }else if(count < 10){
                nodeClass = 'single';
                this.newMsgCountNode.text(count);
            }else if(count < 100){
                nodeClass = 'ten';
                this.newMsgCountNode.text(count);
            }else {
                nodeClass = 'hundred';
                this.newMsgCountNode.text('99+');
            }
            this.newMsgCountNode.removeClass().addClass('new-msg-node').addClass(nodeClass);
        },
        OnMessageReceived: function () {
            uc.util.SystemUtil.flashTaskBar(1, 0);
        },
        ModifyingPassword: function(){
            this.modifyingPassword = true;
        },
        _getReLoginTimerDelay: function(){
            if(this.reloginTimeCount == 0){
                return 5 * 1000;
            }
            var delay = 30 * 1000;
            if(this.reloginTimeCount >= this.maxReloginTime){
                delay = -1;
            }else{
                delay = Math.ceil(delay * Math.random()) + (Math.pow(2, this.reloginTimeCount) - 1) * delay;//delay
            }
            return delay;
        },
        saveCallInfoData:function(){
            if(uc.modules.call.CallManager.inProgressCallInfo.isInProgress){
                uc.util.StorageUtil.setData("CallManager-callInfo:inProgressCallInfo", uc.modules.call.CallManager.inProgressCallInfo);
                uc.modules.call.CallManager.closeWindow();
            }
        },
        reLogin: function(isFromNetworkChanged, isManual){
            if(this.networkStatus == uc.constants.NetworkStatus.Error){
                return;
            }
            var delay, addCount = true;
            if(isFromNetworkChanged || isManual){//Network connected, manual, set the delay is 2 second
                delay = 2 * 1000;
                addCount = false;
            }else {
                delay = this._getReLoginTimerDelay();
            }
            if(delay == -1){
                return;
            }
            if(this.reloginTimer){
                clearTimeout(this.reloginTimer);
            }
            this.clientStatus = uc.constants.ClientStatus.Connecting;
            this.setClientStatus(this.clientStatus);
            this.saveCallInfoData();
            this.reloginTimer = setTimeout(function(){
                if(_this.sessionStatus == uc.constants.SessionStatus.Expired){//If session is expired, need login
                    uc.util.LogUtil.info('Main', 'ReLogin', 'ReLogin UCC Server', null);
                    uc.IClientService.logoutUCAS();
                    uc.IClientService.reLogin();
                }else{
                    uc.util.LogUtil.info('Main', 'ReLogin', 'ReLogin UCAS Server', null);
                    uc.IClientService.logoutUCAS();//TODO need SDK call this method
                    uc.IClientService.loginUCAS();
                }

                if(addCount){
                    _this.reloginTimeCount++;
                }
            }, delay);
        },

        logoutClient: function(){
	        var This = this;
            if(uc.modules.call.CallManager.inProgressCallInfo.isInProgress || !uc.modules.pbx.PbxManager.DialIsClick){
                uc.util.WindowUtil.show();
                uc.modules.component.PromptMessageBox.open({
                    title: uc_resource.WindowTitle.Notification,
                    message: uc_resource.Call.logoutWhenCalling,
                    buttonMode: "both",
                    OkText: uc_resource.Common.OK,
	                CancelText: uc_resource.Common.Cancel
                }, function (flag) {
	                if(flag == 0){ //ok
                        if(!uc.modules.pbx.PbxManager.DialIsClick){
                            uc.modules.pbx.PbxCallManager.exitPbx();
                        }else{
                            uc.modules.call.CallManager.exitCall();
                        }
		                uc.IClientService.logoutUCAS();
		                This.restartClient();
		                uc.modules.cloud.CloudManager.clearLocalStorage();
	                }else{

	                }
                    this.close();
                });
                return;
            }
            uc.IClientService.logoutUCAS();
            this.restartClient();
            uc.modules.cloud.CloudManager.clearLocalStorage();
        },
        quitClient: function(){
            if(uc.modules.call.CallManager.inProgressCallInfo.isInProgress || !uc.modules.pbx.PbxManager.DialIsClick){
	            uc.modules.component.PromptMessageBox.open({
		            title: uc_resource.WindowTitle.Notification,
		            message: uc_resource.Call.quitWhenCalling,
		            buttonMode: "both",
		            OkText: uc_resource.Common.OK,
		            CancelText: uc_resource.Common.Cancel
	            }, function (flag) {
		            if(flag == 0){ //ok
                        if(!uc.modules.pbx.PbxManager.DialIsClick){
                            uc.modules.pbx.PbxCallManager.exitPbx();
                        }else{
                            uc.modules.call.CallManager.exitCall();
                        }
			            uc.IClientService.logoutUCAS();
			            uc.IClientService.logout();
			            uc.quit();
		            }
		            this.close();
	            });
                return;
            }
            uc.IClientService.logoutUCAS();
            uc.IClientService.logout();
            uc.quit();
        },
        restartClient: function(){
            var options = {
                title: 'bee',// 为了解决任务栏右键点击蜜蜂时不弹出主窗口的bug，把窗口名称“蜜蜂”改成“bee”，请勿回退
                width: 420,
                height: 600,
                refreshIfExist: false,
                centerScreen: true,
                modal: false,
                resizable: false,
                minWidth: 420,
                minHeight: 600,
                hasTaskBarIcon: true,
                behaviorIfSCClose: uc.constants.UI.WindowCloseBehavior.Quit,
                isChild: false,
                caption: uc.constants.UI.DragCaption.Login,
                closeOtherWindow: true
            };
            uc.util.WindowUtil.open('index.html', 'login', options);
            uc.IClientService.logout();

            // to fix bug: 在任务栏右键菜单中点击“注销”之后，再登录进来时消息提醒弹框不显示的bug。 by xue.bai_2
            uc.util.StorageUtil.removeData('bubble');
        },
        _openPasswordResetWindow: function(){
            var options = {
                width: 400,
                height: 160,
                refreshIfExist: false,
                centerScreen: true,
                modal: true,
                resizable: false,
                isTop:true,
                caption: uc.constants.UI.DragCaption.ConfirmSetPassword
            };
            uc.util.WindowUtil.open('page/confirmSetPassword.html','confirmSetPassword',options);
        },
        _openPasswordModifiedWindow: function(){
            var options = {
                width: 400,
                height: 160,
                refreshIfExist: false,
                centerScreen: true,
                modal: true,
                resizable: false,
                isTop:false,
                caption: uc.constants.UI.DragCaption.PasswordModified
            };
            uc.util.WindowUtil.open('page/passwordModified.html','passwordModified',options);
        },
        setClientStatus: function(status){
            clearTimeout(this.removeConnectedStatusTimer);
            switch (status){
                case uc.constants.ClientStatus.Disconnected:
                    this.clientStatusTextNode.text(uc_resource.ClientStatus.Disconnected);
                    this.clientStatusContainer.removeClass('connecting').removeClass('connected').removeClass('reconnect-failed').addClass('disconnect');
                    break;
                case uc.constants.ClientStatus.Connecting:
                    this.clientStatusTextNode.text(uc_resource.ClientStatus.Connecting);
                    this.clientStatusContainer.removeClass('disconnect').removeClass('connected').removeClass('reconnect-failed').addClass('connecting');
                    break;
                case uc.constants.ClientStatus.Connected:
                    this.clientStatusTextNode.text(uc_resource.ClientStatus.Connected);
                    this.clientStatusContainer.removeClass('disconnect').removeClass('connecting').removeClass('reconnect-failed').addClass('connected');
                    this.removeConnectedStatusTimer = setTimeout(function(){
                        _this.clientStatusContainer.removeClass('connecting').removeClass('disconnect').removeClass('reconnect-failed').removeClass('connected');
                    }, 2000);
                    break;
                case uc.constants.ClientStatus.ReConnectFailed:
                    this.clientStatusTextNode.text(uc_resource.ClientStatus.ReConnectFailed);
                    this.clientStatusContainer.removeClass('disconnect').removeClass('connecting').removeClass('connected').addClass('reconnect-failed');
            }
            this.clientStatus = status;
        },
        getClientStatus: function() {
            return this.clientStatus;
        },
        setWindowState: function(windowState){
            uc.modules.component.TitleBarButton.setWindowState(windowState);
        },
        OnCheckUpdates: function (errorCode, updateFlag, version, notes) {
            if (errorCode == uc.ErrorCode.Success && updateFlag == uc.ErrorCode.NeedUpdate) {
                uc.util.StorageUtil.setData('needUpdate', 1);
                uc.modules.component.ExtraMenuBar.showNewVersion();
            } else {
                uc.modules.component.ExtraMenuBar.hideNewVersion();
                uc.util.StorageUtil.removeData('needUpdate');
            }
        },
        bindCloudEvent:function(){
            uc.IUIService.bindCrossWindowEvent("createCloudLibSuccess", function (data) {
                uc.modules.cloud.CloudManager.createLibSuccess(data);
            });
            uc.IUIService.bindCrossWindowEvent("reLoginCloud", function (data) {
                uc.modules.cloud.CloudManager.getGkClientReLogin();
            });
            uc.IUIService.bindCrossWindowEvent("getCloudPreviewUserInfo", function (data) {
                uc.modules.cloud.CloudManager.getPreviewerUserId(data);
            });
            uc.IUIService.bindCrossWindowEvent("saveLocalImagesToCloud", function (data) {
                uc.modules.cloud.CloudManager.saveCloudFile(null,JSON.stringify(data));
            });
	        uc.IUIService.bindCrossWindowEvent("sendFromMount", function (eventData) {
		        uc.IUIEvent.OnCloudFileSelectCallBack(eventData);
	        });
	    uc.IUIService.bindCrossWindowEvent("noticeCloudInfoDownload", function (data) {
                uc.modules.cloud.CloudManager.noticeCloudInfoDownload(data);
            });
        },
        OnBodyClick:function(e){
            var target  = $(e.target);
            if(!target.closest(".filecomment").length
                && !target.closest("#comment").length
                && !target.closest(".op-comment").length
                && !target.closest(".sys_comment").length
                && !target.closest(".operation .comment").length
                && !target.closest(".cloudComment").length
                && !target.closest(".pbx .add").length
                && !target.closest(".pbx .call-dmz-dial").length
                && !target.closest(".pbx .pbx-code-list-panel").length
                && !target.closest(".pbx .pbx-code").length
                && !target.closest(".pbx .pbx-icon").length
                && !target.closest(".pbx .pbx-search-panel").length
                && !target.closest(".pbx .pbx-search-content").length
                && !target.closest(".pbx .pbx-search-tel-list").length
                && !target.closest(".pbx .contact-search-result").length
                && !target.closest(".pbx .pbx-dial-btn").length
                && !target.closest(".pbx .pbx-dialPanel").length
                && !target.closest(".chat-header .chat-tel-list").length
                && !target.closest(".chat-header .call").length
                && !target.closest(".unReadNumber").length
                && !target.closest(".mumberMessage").length
                && !target.closest(".current-user-status").length
                && !target.hasClass("internetTip")
                && !target.hasClass("tellingPhone")
                && !target.parent().hasClass("replyMessage")
                && !target.hasClass("quick-reply")
                && !target.hasClass("replyMessage")){
                $(".replyMessage").hide();
                $(".current-user-status-list").addClass("hide");
                $(".pbx .call-dmz-dial").hide();
                $(".pbx .pbx-code-list-panel").hide();
                $('.pbx .icon-search-del').hide();
                $('.pbx .pbx-search-panel').removeClass('focus-line');
                $('.pbx .pbx-dialPanel').addClass('hide');
                $('.pbx .pbx-search-content').hide();
                $('.pbx .pbx-search-tel-list').hide();
                $('.chat-header .chat-tel-list').hide();
                $(".internetTip").remove();
                $(".tellingPhone").remove();
                $(".mumberMessage").remove();
                this.filecomment.hide();
                uc.util.StorageUtil.setData('isOpenComment',false);
                uc.util.StorageUtil.setData('currentChatType',uc.constants.Chat.ChatType.None);
                uc.modules.comment.CommentManager.fileComment=null;
            }
        },
        checkPbxLogin:function(){
            var capability = JSON.parse(uc.util.StorageUtil.getData("capability"));
            var val = uc.util.StorageUtil.getData('OnPbxLoginResult');
            if(capability && capability.pbx == 1){
                if(val != 0){
                    uc.IPbxService.checkPbxLogin(function(args){
                        var errorCode=args[0],res=args[1];
                        uc.util.LogUtil.info('Main', 'checkPbxLogin', 'errorCode='+errorCode+",res="+res, null);
                        if(errorCode == 0){
                            if(res == 0){
                                uc.util.StorageUtil.setData('OnPbxLoginResult',res);
                            }else if(res == 10 || res == 13  ||  res == 14){
                                _this.reConnectPbxLogin();
                            }else{
                                uc.util.StorageUtil.setData('OnPbxLoginResult',res);
                            }
                        }
                    });
                }
            }
        },
        checkSendFile:function(){
            var filePath = uc.util.StorageUtil.getData("OnFileSendToBee");
            if(filePath){
                uc.modules.chat.MessageManager.RightClickToSendFile(filePath);
                uc.util.StorageUtil.removeData("OnFileSendToBee");
            }
        },
        reConnectPbxLogin:function(){
            setTimeout(function(){
                _this.checkPbxLogin();
            },1000);
        },
        OnUserStatusReceived: function(status){
            if(status && status.length > 0){
                var len = status.length;

                for(var i = 0; i < len; i++){
                    var userStatus = status[i].status;
                    var userId = status[i].userId;

                    this.renderCurrentUserStatus(userId, userStatus);

                    if(this.currentUserStatusValue){
                        break;
                    }
                }
            }
        },
        renderCurrentUserStatus: function(userId, userStatus){
            var statusStyle = "";

            if(this.loginUserInfo.userId == userId){
                switch(userStatus){
                    case uc.constants.UserStatusType.Online:
                        statusStyle = 'status-online';
                        break;
                    case uc.constants.UserStatusType.Busy:
                        statusStyle = 'status-busy';
                        break;
                    case uc.constants.UserStatusType.Away:
                        statusStyle = 'status-away';
                        break;
                    case uc.constants.UserStatusType.InMeeting:
                        statusStyle = 'status-inMeeting';
                        break;
                    case uc.constants.UserStatusType.Offline:
                        statusStyle = 'status-offline';
                        break;
                    case uc.constants.UserStatusType.InCall:
                        statusStyle = 'status-inCall';
                        break;
                    default:
                        break;
                }

                this.userStatusList.addClass("hide");
                this.userStatusList.find("li").removeClass("hide");
                this.userStatusList.find("li").removeClass("disabled");
                this.userStatusList.find("i").addClass("hide");

                if(statusStyle){
                    var statusDom = '<div class="' + statusStyle + '"></div>';

                    this.currentUserStatusValue = userStatus;
                    this.currentUserStatus.find("div").remove();
                    this.currentUserStatus.append(statusDom);
                }
            }
        },
	    OnFileSendToBee:function(filePath){
            uc.modules.chat.MessageManager.RightClickToSendFile(filePath);
        },
        OnUserLoginNotify:function(errorCode,message){
            uc.util.LogUtil.info("Main","OnUserLoginNotify","errorCode:"+errorCode,{message:message});
            if(errorCode == 0){
                var tmstr = uc.util.TimeUtil.getMessageTimeString(new Date(), uc.constants.UI.TimeFormat.Message.HourMinutes, uc.settings.meeting.timeZone.offset);
                uc.modules.component.PromptMessageBox.open({
                    title: uc_resource.WindowTitle.Notification,
                    message: uc.util.StringUtil.formatString(uc_resource.Login.LoginTips,[tmstr]),
                    buttonMode: "ok",
                    OkText: uc_resource.Common.OK
                }, function () {
                    this.close();
                });
            }
        },

	    getPBXVisible: function(){
            var version = JSON.parse(uc.util.StorageUtil.getData("capability"));
            return version ? version.pbx && version.pbx==1:false;
	    },

	    getCallVisible: function(){
		    var version = JSON.parse(uc.util.StorageUtil.getData("capability"));
		    return version?version.call && version.call==1:false;
	    },

	    getCalendarVisible: function () {
		    var version = JSON.parse(uc.util.StorageUtil.getData("capability"));
		    return version?version.calendar && version.calendar==1:false;
	    }
    };
    jQuery(document).ready(function(){
        uc.init();
        uc.modules.main.Main.init();
    });
})(uc);