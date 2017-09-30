/**
 * Created by waouyang on 15/12/11.
 */
(function(uc){
    var _this = null;
    uc.modules.call.EmbeddedCallBanner = {
        nodeHtml:   '<div class="embedded-call-banner">' +
                        '<span class="call-status"></span>' +
                        '<div class="call-btn-node">' +
                            '<span class="call-toolbar-btn back-to-call-window icon-call_changeWindow"></span>' +
                            '<span class="call-tell-status"></span>' +
                             '<span class="call-toolbar-btn calm-call icon-call_tellCalm"></span>' +
                            '<span class="call-toolbar-btn answer-call icon-call_reporterTell"></span>' +
                            '<span class="call-toolbar-btn end-call icon-call_stopTell"></span>' +
                            '<span class="call-toolbar-btn quick-reply">'+uc_resource.Call.CallReply+'</span>' +
                                '<div class="replyMessage">' +
                                '<li class="sorry-tell">'+uc_resource.Call.SorryTell+'</li>'+
                                '<li class="come-soon">'+uc_resource.Call.ComeSoon+'</li>'+
                                '<li class="call-you">'+uc_resource.Call.CallYou+'</li>'+
                                '<li class="meeting-after">'+uc_resource.Call.MeetingAfterCallYou+'</li>'+
                                '</div>'+
                        '</div>' +
                    '</div>',
        init: function(callInfo){
            _this = this;
            this.parentNode = $('.message-box-container', uc.ui.Message.ContentPanel);
            this.node = $(this.nodeHtml).appendTo(this.parentNode);
            this.callStatusNode = $('.call-status', this.node);
            this.backToCallWindowBtn = $('.back-to-call-window', this.node);
            this.endCallBtn = $('.end-call', this.node);
            this.answerCallBtn = $('.answer-call', this.node);
            this.quickReplyBtn = $('.quick-reply', this.node);
            this.callStatus = $('.call-tell-status', this.node);
            this.calmCall = $('.calm-call', this.node);
            this.replyMessage = $('.replyMessage', this.node);
	        this.callToolbarbtn = $('.call-toolbar-btn', this.node);
            this.callInfo = callInfo;
            this._initUI();
            this._showCallStatus();
            this._bindEvent();
            this.initialized = true;
	        this.isActivity = true;
        },

        _initUI: function(){
            this.parentNode.addClass('has-floating-call-tool-bar');
            $(".tool-icon-container").removeClass("startCall");
            $(".tool-icon-container .tool-icon").removeClass("startCall");
            uc.modules.chat.ChatManager.scrollMessageBoxToEnd();
        },

        updateCallInfo: function(callInfo){
            this.callInfo = callInfo;
            this._showCallStatus();
        },

        _showCallStatus: function(){
            switch (this.callInfo.callStatus){
                case uc.constants.Call.CallStatus.INVITED:
                    if(this.callInfo.chatType == uc.constants.Chat.ChatType.SingleChat){
                        this.callStatus.text(uc_resource.Call.SingleChatCallInvited);
                    }else {
                        this.callStatus.text(uc_resource.Call.GroupChatCallInvited);
                    }

                    this.backToCallWindowBtn.hide();
                    this.answerCallBtn.show();
                    this.endCallBtn.show();
                    this.calmCall.hide();
                    this.quickReplyBtn.show();

                    if(!this.callInfo.isInProgress){
                        this.answerCallBtn.hide();
                    }
                    break;
                case uc.constants.Call.CallStatus.INVITING:
                    this.callStatus.text(uc_resource.Call.Inviting);
                    this.backToCallWindowBtn.show();
                    this.answerCallBtn.hide();
                    this.endCallBtn.show();
                    this.calmCall.show();
                    this.quickReplyBtn.hide();
                    break;
                case uc.constants.Call.CallStatus.CONNECTING:
                    this.callStatus.text(uc_resource.Call.Connecting);
                    this.backToCallWindowBtn.show();
                    this.answerCallBtn.hide();
                    this.endCallBtn.show();
                    this.calmCall.hide();
                    this.quickReplyBtn.hide();
                    break;
                case uc.constants.Call.CallStatus.CONNECTED:
                    var _this = this;
                    clearInterval(this.countDurationTimer);
                    var duration = (Date.now() - this.callInfo.callConnectedTime) / 1000;
                    this.countDurationTimer = setInterval(function () {
                        duration = duration + 1;
                        var content = uc.util.StringUtil.formatString(uc_resource.Call.CallTelling, [uc.util.TimeUtil.getCallTimeStr(duration)]);
                        _this.callStatus.text(content);
                    }, 1000);

                    this.backToCallWindowBtn.show();
                    this.answerCallBtn.hide();
                    this.endCallBtn.show();
                    this.calmCall.show();
                    this.quickReplyBtn.hide();
                    break;
                case uc.constants.Call.CallStatus.DISCONNECTING:
                    break;
                case uc.constants.Call.CallStatus.DISCONNECTED:
                    break;
            }

            if (this.callInfo.muteStatus == uc.constants.Call.MuteStatus.MUTE) {
                this.calmCall.addClass("open");
            } else {
                this.calmCall.removeClass("open");
            }

            this.setHandleDisableBtn(this.callInfo);
        },
        setHandleDisableBtn:function(callInfo){
            switch(callInfo.handleBtn){
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
                    if(!this.calmCall.hasClass("disable")){
                        this.calmCall.addClass("disable");
                    }
                    break;
                case 'success':
                    this.answerCallBtn.removeClass("disable");
                    this.endCallBtn.removeClass("disable");
                    this.calmCall.removeClass("disable");
                    break;

            }
        },
        _bindEvent: function(){
            this.backToCallWindowBtn.click(function(){
                _this._onBackToTextCallWindowClicked();
            });
            this.endCallBtn.click(function(){
                _this._onEndCallBtnClicked();
            });
            this.answerCallBtn.click(function(){
                _this._onAnswerCallBtnClicked();
            });
            this.quickReplyBtn.click(function(){
                _this._onQuickReplyBtnClicked();
            });
            this.calmCall.click(function(){
                if($(this).hasClass("disable")){
                    return;
                }
                uc.modules.call.CallManager.isMuteCall(_this.callInfo);

            });
            this.replyMessage.find("li").click(function(){
                if(_this.quickReplyBtn.hasClass("disable")){
                  return;
                }
                uc.modules.call.CallManager.rejectCallInvitation(_this.callInfo, $(this).text());
            });

	        this.callToolbarbtn.mouseover(function(e){
		        if($(this).hasClass('open')){
			        $(this).removeClass('open');
			        $(this).addClass('openHover');
		        }
	        });

	        this.callToolbarbtn.mouseout(function(e){
		        if($(this).hasClass('openHover')){
			        $(this).removeClass('openHover');
			        $(this).addClass('open');
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
			        $(this).addClass('open');
		        }
	        });
        },
        _onQuickReplyBtnClicked: function(){
            this.replyMessage.show();
        },
        _onEndCallBtnClicked: function(){
            if(this.endCallBtn.hasClass("disable")){
                return;
            }
            switch (this.callInfo.callStatus){
                case uc.constants.Call.CallStatus.INVITED:
                    uc.modules.call.CallManager.rejectCallInvitation(this.callInfo,"");
                    break;
                case uc.constants.Call.CallStatus.CONNECTED:
                case uc.constants.Call.CallStatus.CONNECTING:
                    uc.modules.call.CallManager.leaveCall();
                    break;
                case uc.constants.Call.CallStatus.INVITING:
                    uc.modules.call.CallManager.cancelCall();
                    break;
            }
        },
        _onAnswerCallBtnClicked: function(){
            if(this.answerCallBtn.hasClass("disable")){
                return;
            }
            uc.modules.call.CallManager.joinCall();
        },
        _onBackToTextCallWindowClicked: function(){
            uc.modules.call.CallManager.switchEmbeddedCallBannerToEmbeddedCallWindow();
        },
        close: function(meetingId){
            if(this.initialized){
                if((meetingId && meetingId == this.callInfo.callMeetingId) || !meetingId){
                    this.parentNode.removeClass('has-floating-call-tool-bar');
                    this.node.remove();
	                this.isActivity = false;
                }

            }
        },
        clearTimer:function(){
            if(this.initialized){
                clearInterval(this.countDurationTimer);
            }
        },
        updateContact:function(basicContactInfos){
            if(this.initialized){
                if(basicContactInfos.siteId){
                    this.quickReplyBtn.removeClass("disable");
                    this.endCallBtn.show().removeClass("disable");
                    this.answerCallBtn.show().removeClass("disable");
                }
            }
        },
        callTurnToEnd:function(){
            this.callStatus.text(uc_resource.Call.CallTurnToEnd);
            var _this=this;
            clearTimeout(endTimer);
	        this.isActivity = false;
            var endTimer=setTimeout(function(){
                _this.close();
                clearTimeout(endTimer);
            },1000);
        }

    }
})(uc);