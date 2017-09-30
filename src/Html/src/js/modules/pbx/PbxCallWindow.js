(function(uc){
    var _this = null;
    uc.modules.pbx.PbxCallWindow = {
        init: function(){
            _this = this;
            this.callInfo = uc.util.StorageUtil.getData('pbx_call_window_call_info');
            this.title = $('.title');
            this.status = $('.status');
            this.avatar = $('.avatar-out');
            this.muteBtn = $('.muteBtn');
            this.callBtn = $('.callBtn');
            this.dialBtn = $('.dialBtn');
            this.hideContent = $('.content-status');
            this.avatarAnimation = $('.avatar-animation');
            this.dmzBody = $('.dmz-body');
            this.dmzhide = $('.dmz-none');
            this.dmzhangUp = $('.dmz-hangUp');
            this.dmzInput = $('.panel-input');
            this.dmzNum = $('.pbx-tel-num a');
            this.dmzDelIcon = $('.pbx-dial-icon-search-del');
            this.closeIcon =  $(".icon-close");
            this.timeOutTips =  $(".timeOutTips");
            uc.modules.component.TitleBarButton.init(uc.constants.UI.WindowCloseBehavior.UnClose);
            this.bindEvent();
            this.initUI();
        },
        initUI:function(){
            this.setTitle();
            this.setStatus();
            this.setAvatar();
            this.startTimeShowTips();
        },
        bindEvent:function(){
            this.muteBtn.click(function(){
                _this.setMuteStatus();
            });
            this.callBtn.click(function(){
                _this.hangupCall();
            });
            this.dmzhangUp.click(function(){
                _this.dmzBody.hide();
                _this.hideContent.show();
                _this.hangupCall();
            });
            this.dialBtn.click(function(){
                _this.hideContent.hide();
                _this.dmzDelIcon.hide();
                _this.dmzBody.show();
                _this.dmzInput.val("").focus();
            });
            this.dmzhide.click(function(){
                _this.hideContent.show();
                _this.dmzBody.hide();
            });
            this.dmzNum.click(function(){
                var num=$(this).text();
                _this.dmzDelIcon.show();
                _this.dmzInput.insertAtCaret(num);
                uc.modules.pbx.PbxCallWindowManager.sendDTMF(_this.callInfo.callID,num);
            });
            this.dmzBody.on("input propertychange",_this.dmzInput,function(){
                var text= $.trim(_this.dmzInput.val());
                if(!text){
                    _this.dmzDelIcon.hide();
                }else{
                    _this.dmzDelIcon.show();
                }
            });
            this.dmzDelIcon.click(function(){
                _this.dmzInput.val("").focus();
                _this.dmzDelIcon.hide();
            });
            this.dmzInput.keypress(function(e){
                var code = (e.keyCode ? e.keyCode : e.which);
                if(uc.constants.Pbx.PbxKeyCode[code]){
                    uc.modules.pbx.PbxCallWindowManager.sendDTMF(_this.callInfo.callID,uc.constants.Pbx.PbxKeyCode[code]);
                }
                return (code >= 48 && code <= 57) || code == 42 || code == 35;
            });
            this.closeIcon.click(function(){
                _this.showPbxCloseWhenCallWindow();
            });
            uc.IUIService.triggerCrossWindowEvent(uc.constants.UI.CrossWindowEventName.PbxFloatingGetCallData,{
                callInfo:_this.callInfo.timeStamp
            });
            uc.IUIService.bindCrossWindowEvent(uc.constants.UI.CrossWindowEventName.PbxMainWindowGetCallDataToFloating, function(data){
                if(data.callInfo.timeStamp == _this.callInfo.timeStamp){
                    if(data.callInfo.callState == uc.constants.Pbx.CallStatus.CALL_STATE_DISCONNECTED){
                        _this.close();
                    }
                }
            });
        },
        setTitle:function(){
            if(this.callInfo.userID){
                var contactInfo = uc.IContactService.getBasicContactInfo(parseInt(this.callInfo.userID));
                if(contactInfo){
                    this.title.text(contactInfo.displayName);
                }else{
                    this.title.text(this.callInfo.phoneNumber);
                }
            }else{
                this.title.text(this.callInfo.phoneNumber);
            }
        },
        setStatus:function(){
            this.status.text(uc_resource.Pbx.Inviting);
        },
        setAvatar:function(){
            var contactAvatar,imgHtml,avatarDefaultCss;
            if(this.callInfo.userID){
                var contactInfo = uc.IContactService.getBasicContactInfo(parseInt(this.callInfo.userID));
                if(contactInfo){
                    if(contactInfo && contactInfo.localAvatar){
                        contactAvatar = contactInfo.localAvatar;
                    }else {
                        contactAvatar = uc.constants.UI.DefaultLocalUrl.Avatar_100;
                    }
                    avatarDefaultCss ="avatar round-image";
                }
            }else{
                avatarDefaultCss ="avatar round-image action";
                contactAvatar = uc.constants.UI.DefaultLocalUrl.PbxAvatar_100;
            }
            imgHtml ='<img src="'+contactAvatar+'" class="'+avatarDefaultCss+'">';
            this.avatar.html(imgHtml);
        },
        setMuteStatus:function(){
            if(!this.callInfo.isCalling){
                if(this.muteBtn.hasClass("mute-call-btn")){
                    this.muteBtn.removeClass("mute-call-btn").addClass("mute-call-btn-select").attr("qtips",uc_resource.Pbx.UnMute);
                }else{
                    this.muteBtn.removeClass("mute-call-btn-select").addClass("mute-call-btn").attr("qtips",uc_resource.Pbx.Mute);
                }
                return;
            }
            if(this.muteBtn.hasClass("disable")){
                return;
            }
            this.setMute();
        },
        setMute:function(){
            if(this.muteBtn.hasClass("mute-call-btn-select")){
                uc.modules.pbx.PbxCallWindowManager.unMuteCall(this.callInfo.callID,function(){
                    _this.muteBtn.removeClass("mute-call-btn-select").addClass("mute-call-btn").attr("qtips",uc_resource.Pbx.Mute);
                });
            }else{
                uc.modules.pbx.PbxCallWindowManager.muteCall(this.callInfo.callID,function(){
                    _this.muteBtn.removeClass("mute-call-btn").addClass("mute-call-btn-select").attr("qtips",uc_resource.Pbx.UnMute);
                });
            }
        },
        startTimeShowTips:function(){
           this.showTipsTime=setTimeout(function(){
               _this.timeOutTips.text(uc_resource.Pbx.timeOutTips);
           },20000);
        },
        callSuccessSetMute:function(){
            if(this.muteBtn.hasClass("mute-call-btn-select")){
                uc.modules.pbx.PbxCallWindowManager.muteCall(this.callInfo.callID,function(){
                    _this.muteBtn.removeClass("mute-call-btn").addClass("mute-call-btn-select").attr("qtips",uc_resource.Pbx.UnMute);
                });
            }
        },
        showCallStatus:function(data){
            switch (data.callState){
                case uc.constants.Pbx.CallStatus.CALL_STATE_INCOMING:
                    this.status.text(uc_resource.Pbx.Inviting);
                    break;
                case uc.constants.Pbx.CallStatus.CALL_STATE_EARLY:
                case uc.constants.Pbx.CallStatus.CALL_STATE_CONNECTING:
                    this.status.text(uc_resource.Pbx.Inviting);
                    break;
                case uc.constants.Pbx.CallStatus.CALL_STATE_CONFIRMED:
                    this.status.text(uc_resource.Pbx.Connecting);
                    this.callSuccessSetMute();
                    this.showCallTime();
                    break;
                case uc.constants.Pbx.CallStatus.CALL_STATE_DISCONNECTED:
                    break;
            }
        },
        showCallTime:function(){
            this.avatar.find("img").removeClass("action");
            this.avatarAnimation.remove();
            this.dialBtn.removeClass("hide");
            this.timeOutTips.addClass("hide");
            this.callInfo.isCalling = true;
            var duration = (Date.now() - Date.now()) / 1000;
            this.clearDurationTimer();
            this.countDurationTimer = setInterval(function () {
                duration = duration + 1;
                var content = uc.util.TimeUtil.getCallTimeStr(duration);
                _this.status.text(content);
            }, 1000);
        },
        clearDurationTimer:function(){
            clearInterval(this.countDurationTimer);
            clearTimeout(this.showTipsTime);
        },
        hideStatus:function(){
            if(this.muteBtn.hasClass("mute-call-btn-select")){
                this.muteBtn.addClass("disable-opacity");
            }else{
                this.muteBtn.addClass("disable");
            }
            this.avatar.addClass("disable-opacity");
            this.callBtn.addClass("disable");
            this.dialBtn.addClass("disable");
            this.status.text(uc_resource.Pbx.CallTurnToEnd);
        },
        close:function(){
            this.clearDurationTimer();
            this.avatar.find("img").removeClass("action");
            this.avatarAnimation.remove();
            this.hideStatus();
            setTimeout(function(){
                uc.util.WindowUtil.close();
            },1000);
        },
        hangUpCallStatus:function(data){
            if(data.time == this.callInfo.timeStamp){
                _this.close();
            }
        },
        hangupCall:function(){
            uc.modules.pbx.PbxCallWindowManager.hangupCall(this.callInfo.callID,this.callInfo);
        },
        showPbxCloseWhenCallWindow: function(){
            uc.modules.component.PromptMessageBox.open({
                title: uc_resource.WindowTitle.Notification,
                message: uc_resource.Pbx.CloseWhenCallWindow,
                buttonMode: "both",
                OkText: uc_resource.Common.OK,
                CancelText: uc_resource.Common.Cancel
            }, function (result) {
                if(result == 0){
                    _this.hangupCall();
                }
                this.close();
            });
        }
    };
    jQuery(document).ready(function () {
        uc.init();
        uc.modules.pbx.PbxCallWindow.init();
    })
})(uc);