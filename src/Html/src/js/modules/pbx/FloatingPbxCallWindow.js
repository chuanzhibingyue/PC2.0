(function(uc){
    var _this = null;
    uc.modules.pbx.FloatingPbxCallWindow = {
	    isInit:false,
	    callStart:false,
        titleNodeHtml: '<div class="title-node">' +
                            '<span class="avatar-node-container">' +
                                '<span class="avatar-node"></span>' +
                                '<span class="audio-icon"></span>' +
                            '</span>' +
                            '<div class="display-name"></div>' +
                            '<div class="call-status "><span></span></div>' +
                        '</div>',
        init: function(callInfo){
            _this = this;
            this.callInfo = callInfo;
			this.callwindow= $('.floating-call-window');
            this.titleNodeContainer = $('.call-title-container');
            this.callPanelContainer = $('.call-panel-container');
            this.toolBarContainer = $('.call-tool-bar-container');
            this.titleNode = $(this.titleNodeHtml).appendTo(this.titleNodeContainer);
            this.avatarNode = $('.avatar-node', this.titleNode);
            this.displayNameNode = $('.display-name', this.titleNode);
            this.callStatusNode = $('.call-status', this.titleNode);
            this.callStatusTime = $('.call-status span', this.titleNode);
            this._initUI();
           // this._bindEvent();
            uc.modules.component.TitleBarButton.init(uc.constants.UI.WindowCloseBehavior.UnClose);
        },
        _initUI:function(){
            clearInterval(this.countDurationTimer);
            uc.modules.pbx.FloatingPbxCallWindowToolBar.init(this.toolBarContainer, this.callInfo);
            this.initPageStatus();
        },
        initPageStatus:function(){
            if(this.callInfo.incoming == uc.constants.Pbx.CallIncomingType.CallOut){
                //set title
                if(this.callInfo && this.callInfo.userID){
                    this.displayNameNode.text(this.callInfo.userName);
                    this.setAvatar(this.getAvatar(this.callInfo.userID));
                }else{
                    this.displayNameNode.text(this.callInfo.phoneNumber || this.callInfo.fromNumber);
                    this.setAvatar(uc.constants.UI.DefaultLocalUrl.PbxAvatar_40);
                }
                //set time
                if(this.callInfo.callTime && this.callInfo.callTime!=0){
                    this.showCallTime(this.callInfo.callTime);
                }else{
                    this.callStatusTime.text(uc_resource.Pbx.Calling);
                }

            }
            //呼入电话
            else if(this.callInfo.incoming == uc.constants.Pbx.CallIncomingType.CallIn){
                this.displayNameNode.text(this.callInfo.fromNumber);
                this.setAvatar(uc.constants.UI.DefaultLocalUrl.PbxAvatar_40);
                if(this.callInfo.callState ==  uc.constants.Pbx.CallStatus.CALL_STATE_CONFIRMED){
                    uc.modules.pbx.FloatingPbxCallWindowToolBar.setCallHide();
                }else{
                    this.callStatusTime.text(uc_resource.Pbx.ForYouInviting);
                    this.updateWindowWH();
                }
            }
        },
        setAvatar:function(path){
            var contactAvatar = '<img src="' + path+ '" class="avatar round-image">';
            this.avatarNode.append(contactAvatar);
        },
        getAvatar:function(userID){
            var contactInfo = uc.IContactService.getBasicContactInfo(parseInt(userID));
            var contactAvatar;
            if(contactInfo && contactInfo.localAvatar){
                contactAvatar = contactInfo.localAvatar;
            }else {
                contactAvatar = uc.constants.UI.DefaultLocalUrl.Avatar_40;
            }
            return contactAvatar;
        },
        _bindEvent:function(){
            var self = this;
            uc.IUIService.triggerCrossWindowEvent(uc.constants.UI.CrossWindowEventName.PbxFloatingGetCallData,{
                info:_this.callInfo.timeStamp
            });
            uc.IUIService.bindCrossWindowEvent(uc.constants.UI.CrossWindowEventName.PbxMainWindowGetCallDataToFloating, function(data){
                if(data.callInfo.timeStamp == _this.callInfo.timeStamp){
                    if(data.callInfo.callState == uc.constants.Pbx.CallStatus.CALL_STATE_DISCONNECTED){
                        uc.modules.pbx.FloatingPbxCallWindowToolBar.close();
                    }else if(data.callInfo.callState == uc.constants.Pbx.CallStatus.CALL_STATE_CONFIRMED){
                        if(!self.callStart){
                            self.showCallTime(data.callInfo.callTime);
                        }
                    }
                    if(data.callInfo.errorCode && data.callInfo.errorCode != 0){
                        uc.modules.pbx.FloatingPbxCallWindowToolBar.close();
                    }
                    if(data.callInfo.isHangup){
                        uc.modules.pbx.FloatingPbxCallWindowToolBar._setHangupStatus();
                    }
                    uc.util.LogUtil.info('PbxMainWindowGetCallDataToFloating', 'Pbx', 'callID = ' +data.callInfo.callID+",callID="+_this.callInfo.callID+",callInfo="+JSON.stringify(data.callInfo), null);
                }
            });
            this.avatarNode.click(function(){
                if(_this.callInfo && _this.callInfo.incoming == uc.constants.Pbx.CallIncomingType.CallIn){
                    if(_this.callInfo.callState == uc.constants.Pbx.CallStatus.CALL_STATE_CONFIRMED){
                        _this.switchFloatingWindow();
                    }
                }else{
                        _this.switchFloatingWindow();
                }
            });
        },
        switchFloatingWindow:function(){
            uc.IUIService.triggerCrossWindowEvent(uc.constants.UI.CrossWindowEventName.PbxFloatingWindowSwitchMainWindow,_this.callInfo);
            uc.modules.pbx.FloatingPbxCallWindowToolBar.close();
        },
        updateWindowWH:function(){
            this.toolBarContainer.height('60px');
            this.callwindow.height('140px');
        },
        setCallInfo:function(data){
             if(data.timeStamp == _this.callInfo.timeStamp){
                 this.callInfo.callID=data.callID;
                 this.callInfo.callState=data.callState;
             }
        },
        showCallStatus: function(data){
            this.setCallInfo(data);
            switch (data.callState){
                case uc.constants.Pbx.CallStatus.CALL_STATE_INCOMING:
                    this.callStatusTime.text(uc_resource.Pbx.Inviting);
                    break;
                case uc.constants.Pbx.CallStatus.CALL_STATE_EARLY:
                case uc.constants.Pbx.CallStatus.CALL_STATE_CONNECTING:
                    this.callStatusTime.text(uc_resource.Pbx.Calling);
                    break;
                case uc.constants.Pbx.CallStatus.CALL_STATE_CONFIRMED:
                    this.callStatusTime.text(uc_resource.Pbx.Connecting);
                    this.showCallTime();
                    break;
                case uc.constants.Pbx.CallStatus.CALL_STATE_DISCONNECTED:
                    uc.modules.pbx.FloatingPbxCallWindowToolBar.incomingCallClose(data);
                    break;
            }
        },
        showCallTime:function(callTime){
            var time = 0;
            if(callTime && callTime != 0){
                time = callTime;
            }
            this.callStart=true;
            clearInterval(this.countDurationTimer);
            uc.modules.pbx.FloatingPbxCallWindowToolBar._setMuteStatus();
            uc.modules.pbx.FloatingPbxCallWindowToolBar._setHangupStatus();
            uc.modules.pbx.FloatingPbxCallWindowToolBar.switchWindowSetMuteStatus();
            var duration = (Date.now() - Date.now()) / 1000;
            this.countDurationTimer = setInterval(function () {
                duration = duration + 1 + time;
                var content = uc.util.TimeUtil.getCallTimeStr(duration);
                _this.callStatusTime.text(content);
                _this.callInfo.callTime = duration;
                time = 0;
            }, 1000);
        },
        showBtnStatus:function(){
            this.callwindow.height('100px');
            this.toolBarContainer.height('70px');
            this.titleNodeContainer.css('margin-top', '30px');
        }
    };
    jQuery(document).ready(function () {
        uc.init();
        uc.modules.pbx.FloatingPbxCallWindowManager.init();
    })
})(uc);