/**
 * Created by wei.liu on 16-3-31.
 */
(function(uc){
    var _this = null;
    uc.modules.pbx.FloatingPbxCallWindowToolBar = {
        init: function(parentNode, callInfo){
            _this = this;
            this.nodeHtml = '<div class="floating-call-tool-bar">' +
                '<span class="call-btn-node">' +
                '<span class="call-toolbar-btn mute icon-call_tellCalm hide"></span>' +
                '<span class="call-toolbar-btn end-call icon-call_stopTell"></span>' +
                '<span class="call-toolbar-btn answer-call icon-call_reporterTell"></span>' +
                '</span>' +
                '</div>';
            this.node = $(this.nodeHtml).appendTo(parentNode);
            this.endCallBtn = $('.end-call', this.node);
            this.answerCallBtn = $('.answer-call', this.node);
            this.muteCallBtn = $('.mute', this.node);
            this.callInfo = callInfo;
            this._initButtons();
            this._bindEvent();
        },
        _initButtons: function(){
            if(this.callInfo.incoming == uc.constants.Pbx.CallIncomingType.CallOut){
                this.endCallBtn.addClass("disable");
            }
            this._showCallStatus();
        },
        _showCallStatus:function(){
            if(this.callInfo.incoming == uc.constants.Pbx.CallIncomingType.CallOut){
                _this.setCallHide();
                if(this.callInfo.isHangup){
                    this._setHangupStatus();
                }
            }
        },
        _setHangupStatus:function(){
            this.endCallBtn.removeClass("disable");
        },
        _setMuteStatus:function(){
            this.muteCallBtn.removeClass("hide");
        },
        _bindEvent: function(){
            this.answerCallBtn.click(function(){
                uc.IPbxService.answerCall(
                    _this.callInfo.callID,
                    0,
                    function(args){
                        var res = args[0];
                        uc.util.LogUtil.info('FloatingPbxCallWindowToolBar', 'answerCall', 'errorCode = ' +res+",callID="+_this.callInfo.callID+",callInfo="+JSON.stringify(_this.callInfo), null);
                        if(res == 0 ){
                            _this.setCallHide();
                            uc.modules.pbx.FloatingPbxCallWindow.showBtnStatus();
                        }else{
                            _this.showPbxAnswerCallFail();
                        }
                    });
            });
            this.endCallBtn.click(function(){
                if($(this).hasClass("disable")){
                    return;
                }
                if(_this.answerCallBtn.hasClass("hide")){
                    _this.hangupCall();
                }else{
                    _this.rejectCall();
                }
            });
            this.muteCallBtn.click(function(){
                if(_this.muteCallBtn.hasClass("muted")){
                    _this.unMuteCall();
                }else{
                    _this.muteCall();
                }
            });

            $(".icon-close").click(function(){
                _this.showPbxCloseWhenCalling($(this));
            });
        },
        muteCall:function(){
            uc.IPbxService.muteCall(_this.callInfo.callID,function(args){
                uc.util.LogUtil.info('FloatingPbxCallWindowToolBar','muteCall', 'errorCode = ' +args[0]+",callID="+_this.callInfo.callID+",callInfo="+JSON.stringify(_this.callInfo), null);
                if(args[0] == 0){
                    _this.callInfo.muteStatus = true;
                    _this.muteCallBtn.addClass("muted");
                }
            });
        },
        unMuteCall:function(){
            uc.IPbxService.unMuteCall(_this.callInfo.callID,function(args){
                uc.util.LogUtil.info('FloatingPbxCallWindowToolBar','muteCall', 'errorCode = ' +args[0]+",callID="+_this.callInfo.callID+",callInfo="+JSON.stringify(_this.callInfo), null);
                if(args[0] == 0){
                    _this.callInfo.muteStatus = false;
                    _this.muteCallBtn.removeClass("muted");
                }
            });
        },
        switchWindowSetMuteStatus:function(){
            if(this.callInfo.muteStatus){
                _this.muteCallBtn.addClass("muted");
            }else{
                _this.muteCallBtn.removeClass("muted");
            }
        },
        hangupCall:function(){
            uc.IPbxService.hangupCall(
                _this.callInfo.callID,
                function(args){
                    var res = args[0];
                    uc.util.LogUtil.info('FloatingPbxCallWindowToolBar', 'hangupCall', 'errorCode = ' +res+",callID="+_this.callInfo.callID+",callInfo="+JSON.stringify(_this.callInfo), null);
                    if(res == 0){
                        _this.close();
                    }else{
                        _this.showPbxHangupCallFail();
                    }
                });
        },
        rejectCall:function(){
            uc.IPbxService.rejectCall(
                _this.callInfo.callID,
                function(args){
                    var res = args[0];
                    uc.util.LogUtil.info('FloatingPbxCallWindowToolBar', 'rejectCall', 'errorCode = ' +res+",callID="+_this.callInfo.callID+",callInfo="+JSON.stringify(_this.callInfo), null);
                    _this.close();
                });
        },
        setCallHide:function(){
            this.answerCallBtn.addClass(" hide");
        },
        close:function(){
            uc.util.WindowUtil.close();
        },
        incomingCallClose:function(data){
            if(data.callID ==  _this.callInfo.callID){
                uc.util.WindowUtil.close();
            }
        },
        showPbxCloseWhenCalling: function(){
            uc.modules.component.PromptMessageBox.open({
                title: uc_resource.WindowTitle.Notification,
                message: uc_resource.Pbx.CloseWhenCalling,
                buttonMode: "both",
                OkText: uc_resource.Common.OK,
                CancelText: uc_resource.Common.Cancel
            }, function (result) {
                if(result == 0){
                    if(_this.answerCallBtn.hasClass("hide")){
                        _this.hangupCall();
                    }else{
                        _this.rejectCall();
                    }
                }else{
                    this.close();
                }
            });
        },
        showPbxHangupCallFail: function(){
            uc.modules.component.PromptMessageBox.open({
                title: uc_resource.WindowTitle.Notification,
                message: uc_resource.Pbx.hangupCallFailed,
                buttonMode: "ok",
                OkText: uc_resource.Common.OK
            }, function () {
                _this.close();
            });
        },
        showPbxAnswerCallFail: function(){
            uc.modules.component.PromptMessageBox.open({
                title: uc_resource.WindowTitle.Notification,
                message: uc_resource.Pbx.AnswerCallException,
                buttonMode: "ok",
                OkText: uc_resource.Common.OK
            }, function () {
                _this.close();
            });
        }
    }
})(uc);