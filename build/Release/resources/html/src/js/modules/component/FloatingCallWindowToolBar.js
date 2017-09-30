/**
 * Created by waouyang on 15/12/7.
 */
(function(uc){
    var _this = null;
    uc.modules.component.FloatingCallWindowToolBar = {
        init: function(parentNode, callInfo){
            _this = this;
            this.nodeHtml = '<div class="floating-call-tool-bar">' +
                                '<span class="call-toolbar-btn quick-reply">' +uc_resource.Call.CallReply+'</span>' +
                                '<div class="replyMessage">' +
                                '<li class="sorry-tell">'+uc_resource.Call.SorryTell+'</li>'+
                                '<li class="come-soon">'+uc_resource.Call.ComeSoon+'</li>'+
                                '<li class="call-you">'+uc_resource.Call.CallYou+'</li>'+
                                '<li class="meeting-after">'+uc_resource.Call.MeetingAfterCallYou+'</li>'+
                                '<li class="self-send-message">'+uc_resource.Call.SelfSendMessage+'</li>'+
                                '</div>'+
                                '<span class="call-btn-node">' +
                                    '<span class="call-toolbar-btn mute icon-call_tellCalm"></span>' +
                                    '<span class="call-toolbar-btn end-call icon-call_stopTell"></span>' +
                                    '<span class="call-toolbar-btn answer-call icon-call_reporterTell"></span>' +
                                    '<span class="call-toolbar-btn video icon-call_video"></span>' +
                                '</span>' +
                            '</div>';
            this.node = $(this.nodeHtml).appendTo(parentNode);
            this.quickReplyBtn = $('.quick-reply', this.node);
            this.callBtnNode = $('.call-btn-node', this.node);
            this.muteBtn = $('.mute', this.node);
            this.endCallBtn = $('.end-call', this.node);
            this.answerCallBtn = $('.answer-call', this.node);
            this.videoBtn = $('.video', this.node);
            this.replyMessage=$('.replyMessage', this.node);
            this.selfSendMessage=$('.self-send-message', this.node);
	        this.callToolbarbtn = $('.call-toolbar-btn', this.node);
            this.callInfo = callInfo;
            this._initButtons();
            this._bindEvent();
        },
        _initButtons: function(){
            if(this.callInfo.videoStatus == uc.constants.Call.VideoStatus.OPEN){
                this.callBtnNode.addClass('video-open');
            }
            this._showCallStatus();
        },
        _bindEvent: function(){
			$(document).click(function(e){
                var target = $(e.target);
                if(!target.parent().hasClass("replyMessage") && !target.hasClass("replyMessage")){
                    _this.replyMessage.hide();
                }
            });
            this.quickReplyBtn.click(function(e){
                _this._onQuickReplyBtnClicked();
				e.stopPropagation();
            });
            this.muteBtn.click(function(){
                _this._onMuteBtnClicked();
            });
            this.endCallBtn.click(function(){
                _this._onEndCallBtnClicked();
            });
            this.answerCallBtn.click(function(){
                _this._onAnswerCallBtnClicked();
            });
            this.videoBtn.click(function(){
                _this._onVideoBtnClicked();
            });
            this.selfSendMessage.click(function(){
                var rejectedData = {
                    callInfo: _this.callInfo,
                    reason: ''
                };
                uc.IUIService.triggerCrossWindowEvent(uc.constants.UI.CrossWindowEventName.RejectCallInvitation, rejectedData);

                uc.IUIService.triggerCrossWindowEvent(uc.constants.UI.CrossWindowEventName.OpenChatWindow,
                                {chatType: _this.callInfo.chatType, Id: _this.callInfo.chatId});
                _this._closeFloatingCallWindow();
            });
            this.replyMessage.find("li").click(function(){
				_this.replyMessage.hide();
	            if(_this.quickReplyBtn.hasClass("disable") || $(this).hasClass("self-send-message")){
		            return;
	            }
                var rejectedData={
                    callInfo:_this.callInfo,
                    reason:$(this).text()
                };
                uc.IUIService.triggerCrossWindowEvent(uc.constants.UI.CrossWindowEventName.RejectCallInvitation, rejectedData);
                _this._closeFloatingCallWindow();
            });
	        this.callToolbarbtn.mouseover(function(e){
		        if($(this).hasClass('muted')){
			        $(this).removeClass('muted');
			        $(this).addClass('openHover');
		        }
	        });

	        this.callToolbarbtn.mouseout(function(e){
		        if($(this).hasClass('openHover')){
			        $(this).removeClass('openHover');
			        $(this).addClass('muted');
		        }
	        });

	        this.callToolbarbtn.mousedown(function(e){
		        if($(this).hasClass('openHover')){
			        $(this).removeClass('openHover');
			        $(this).addClass('openActive');
		        }
	        });

	        this.callToolbarbtn.mouseup(function(){
		        if($(this).hasClass('openActive')){
			        $(this).removeClass('openActive');
			        $(this).addClass('muted');
		        }
	        });
        },
        _onQuickReplyBtnClicked: function(){	
			this.replyMessage.show();
        },
        _onMuteBtnClicked: function(){
            var _t=this;
            if(this.muteBtn.hasClass("disable")){
                return;
            }
            uc.IUIService.triggerCrossWindowEvent(uc.constants.UI.CrossWindowEventName.IsMuteCall, this.callInfo);
        },
        _onEndCallBtnClicked: function(){
            if(this.endCallBtn.hasClass("disable")){
                return;
            }
            this.endCallBtn.addClass("disable");
            this.answerCallBtn.addClass("disable");
            var _this=this;
            if (this.callInfo.callStatus == uc.constants.Call.CallStatus.INVITED) {
                var rejectedData = {
                    callInfo: _this.callInfo,
                    reason: ''
                };
                uc.IUIService.triggerCrossWindowEvent(uc.constants.UI.CrossWindowEventName.RejectCallInvitation, rejectedData);
            } else if(this.callInfo.callStatus == uc.constants.Call.CallStatus.INVITING){
                uc.IUIService.triggerCrossWindowEvent(uc.constants.UI.CrossWindowEventName.CancelCall, {callInfo:this.callInfo});
            }else {
                if(!this.callInfo.callConnectedTime){
                    uc.IUIService.triggerCrossWindowEvent(uc.constants.UI.CrossWindowEventName.LeaveCallSaveCallLogToPbx, {callInfo:this.callInfo});
                }
                uc.IUIService.triggerCrossWindowEvent(uc.constants.UI.CrossWindowEventName.LeaveCall, {callInfo:this.callInfo});
            }
            //this._closeFloatingCallWindow();
        },
        _onAnswerCallBtnClicked: function(){
            if(this.answerCallBtn.hasClass("disable")){
                return;
            }
            this.endCallBtn.addClass("disable");
            this.answerCallBtn.addClass("disable");
            uc.IUIService.triggerCrossWindowEvent(uc.constants.UI.CrossWindowEventName.JoinCall, this.callInfo);
        },
        _onVideoBtnClicked: function(){

        },
        _closeFloatingCallWindow: function(){
	        uc.modules.call.FloatingCallWindow.close();
        },
        updateCallInfo: function(callInfo){
            this.callInfo = callInfo;
            this._showCallStatus();
        },

        _showCallStatus: function(){
            switch (this.callInfo.callStatus){
                case uc.constants.Call.CallStatus.INVITING:
                case uc.constants.Call.CallStatus.CONNECTING:
                    this.quickReplyBtn.hide();
                    this.answerCallBtn.hide();
                    this.videoBtn.hide();
                    this.endCallBtn.show();
                    this.muteBtn.show();
                    break;
                case uc.constants.Call.CallStatus.INVITED:
                    this.quickReplyBtn.show();
                    this.answerCallBtn.show();
                    this.endCallBtn.show();
                    this.muteBtn.hide();
                    this.videoBtn.hide();
                    break;
                case uc.constants.Call.CallStatus.CONNECTED:
                    this.endCallBtn.show();
                    this.muteBtn.show();
                    this.quickReplyBtn.hide();
                    this.answerCallBtn.hide();
                    this.videoBtn.hide();
                    break;
                case uc.constants.Call.CallStatus.DISCONNECTING:
                    this.quickReplyBtn.hide();
                    this.answerCallBtn.hide();
                    this.videoBtn.hide();
                    break;
                case uc.constants.Call.CallStatus.DISCONNECTED:
                    break;
            }

            if(this.callInfo.muteStatus == uc.constants.Call.MuteStatus.MUTE){
                this.muteBtn.addClass('muted');
            }else {
                this.muteBtn.removeClass('muted');
            }

            this.setHandleDisableBtn();
        },

        setHandleDisableBtn:function(){
            switch(this.callInfo.handleBtn){
                case "joinCall":
                    if(!this.answerCallBtn.hasClass("disable")){
                        this.answerCallBtn.addClass("disable");
                    }
                    if(!this.endCallBtn.hasClass("disable")){
                        this.endCallBtn.addClass("disable");
                    }
                    break;
                case "endCall":
                    if(!this.endCallBtn.hasClass("disable")){
                        this.endCallBtn.addClass("disable");
                    }
                    if(!this.answerCallBtn.hasClass("disable")){
                        this.answerCallBtn.addClass("disable");
                    }
                    break;
                case "muteCall":
                    if(!this.muteBtn.hasClass("disable")){
                        this.muteBtn.addClass("disable");
                    }
                    break;
                case 'success':
                    this.answerCallBtn.removeClass("disable");
                    this.endCallBtn.removeClass("disable");
                    this.muteBtn.removeClass("disable");
                    break;

            }
        },

        setReply:function(user){
            this.callInfo.siteId=user.siteId;
            this.quickReplyBtn.removeClass("disable");
            this.endCallBtn.removeClass("disable");
            this.answerCallBtn.removeClass("disable");
        },
		clearReplyMessage:function(){

		}
    }
})(uc);