/**
 * Created by waouyang on 15/12/7.
 */
(function(uc){
    var _this = null;

    uc.modules.call.EmbeddedCallWindow = {
        init: function(callInfo){
            this.callInfo = callInfo;

            if(!this.initialized){
                _this = this;
                this.nodeHtml = '<div class="embedded-call-window">' +
                                    '<div class="embedded-call-title-container">' +
                                        '<div class="embedded-call-title"></div>' +
                                        '<div class="embedded-call-status"><span></span></div>' +
                                    '</div>' +
                                    '<div class="embedded-call-panel-container"><div class="user-list-up"></div><div class="user-list-down"></div></div>' +
                                    '<div class="embedded-call-tool-bar-container"></div>' +
                                '</div>'+
                                '<div class="callTurnEnd"></div>' ;
                this.node = $(this.nodeHtml).appendTo(uc.ui.Message.MainPanel);
                this.callTitle = $('.embedded-call-title', this.node);
                this.callStatusNode = $('.embedded-call-status', this.node);
                this.callStatusTime= $('.embedded-call-status span', this.node);
                this.callPanelContainer = $('.embedded-call-panel-container', this.node);
                this.callToolBarContainer = $('.embedded-call-tool-bar-container', this.node);
                this.callTurnEnd=$('.callTurnEnd');
                uc.modules.component.EmbeddedCallWindowPanel.init(this.callPanelContainer);
                uc.modules.component.EmbeddedCallWindowToolBar.init(this.callToolBarContainer);
                this.initialized = true;
            }
	        this.isActivity = true;
            this.clearAllStatus();
            this.callStatusNode.attr("data-userId",this.callInfo.chatId);
            this._initUI();
            this._showCallStatus();

            setTimeout(function(){
                _this.node.removeClass('show').removeClass('close').addClass('show');
            }, 10);
        },

        _initUI: function(){
            this.handleWindowBtn();
            switch (this.callInfo.chatType){
                case uc.constants.Chat.ChatType.SingleChat:
                    var contactInfo = uc.IContactService.getContactInfo(this.callInfo.chatId);
                    if(contactInfo){
                        this.callTitle.text(contactInfo.displayName);
                        this.callInfo.siteId=contactInfo.siteId;
                        uc.modules.call.CallManager.updateCallInfo(this.callInfo);
                    }else {
                        this.callTitle.text(this.callInfo.chatId);
                    }
                    break;
                case uc.constants.Chat.ChatType.ProjectChat:
                case uc.constants.Chat.ChatType.GroupChat:
                    var groupInfo = uc.IGroupService.getGroupInfo(this.callInfo.chatId);
                    if(groupInfo){
                        this.callTitle.text(groupInfo.name);
                        this.callInfo.siteId=groupInfo.siteId;
                        for(var i=0;i<groupInfo.members.length;i++){
                            if(!_this.callInfo.userList[groupInfo.members[i].userId]){
                                _this.callInfo.userList[groupInfo.members[i].userId]={};
                                _this.callInfo.userList[groupInfo.members[i].userId].callStatus= uc.constants.Call.CallStatus.INVITING;
                            }
                        }
                        uc.modules.call.CallManager.updateCallInfo(this.callInfo);
                    }else {
                        this.callTitle.text(this.callInfo.chatId);
                    }
                    break;
            }
            uc.modules.component.EmbeddedCallWindowPanel.initUI(this.callInfo);
            uc.modules.component.EmbeddedCallWindowToolBar.initUI(this.callInfo);
        },
        handleWindowBtn:function(){
          $(".tool-icon-container").removeClass("startCall");
          $(".tool-icon-container .tool-icon").removeClass("startCall");
          $(".tool-icon-container").addClass("startCall");
          $(".tool-icon-container .tool-icon").addClass("startCall");
        },
        updateCallInfo: function(callInfo){
            this.callInfo = callInfo;
            this._showCallStatus();
        },
	    updateToobarScreenBtnStatus:function(callInfo){
		    this.callInfo = callInfo;
		    uc.modules.component.EmbeddedCallWindowToolBar.updateCallInfo(this.callInfo);
	    },
        _showCallStatus: function(){
            switch (this.callInfo.callStatus){
                case uc.constants.Call.CallStatus.INVITING:
                    this.callStatusTime.text(uc_resource.Call.Inviting);
                    break;
                case uc.constants.Call.CallStatus.CONNECTING:
                    this.callStatusTime.text(uc_resource.Call.Connecting);
                    break;
                case uc.constants.Call.CallStatus.CONNECTED:
                    clearInterval(this.countDurationTimer);
                    var duration = (Date.now() - this.callInfo.callConnectedTime) / 1000;
                    this.countDurationTimer = setInterval(function () {
                        duration = duration + 1;
                        var content = uc.util.TimeUtil.getCallTimeStr(duration);
                        _this.callStatusTime.text(content);
                    }, 1000);
                    if(this.callInfo.chatType==uc.constants.Chat.ChatType.SingleChat){
                        switch(this.callInfo.userList[this.callInfo.chatId].muteStatus){
                            case uc.constants.Call.MuteStatus.MUTE:
                                if(this.callInfo.userList[this.callInfo.chatId].audioType == uc.constants.Call.AudioType.VOIP){
                                    this.addAudioClass("icon-call_voipVideoMute");
                                }else if(this.callInfo.userList[this.callInfo.chatId].audioType == uc.constants.Call.AudioType.PHONE){
                                    this.addAudioClass("icon-call_phoneVideoMute");
                                }else{
                                    this.addAudioClass("");
                                }
                                break;
                            case uc.constants.Call.MuteStatus.UN_MUTE:
                                if(this.callInfo.userList[this.callInfo.chatId].audioType == uc.constants.Call.AudioType.VOIP){
                                    this.addAudioClass("icon-call_voipVideo");
                                }else if(this.callInfo.userList[this.callInfo.chatId].audioType == uc.constants.Call.AudioType.PHONE){
                                    this.addAudioClass("icon-call_phoneVideo");
                                }else{
                                    this.addAudioClass("");
                                }
                                break;
                        }
                    }
                    this.callStatusTime.text(uc.util.TimeUtil.getCallTimeStr(duration));
                    break;
                case uc.constants.Call.CallStatus.DISCONNECTED:
                    this.callStatusTime.text(uc_resource.Call.DisConnecting);
                    break;
            }
            uc.modules.component.EmbeddedCallWindowPanel.updateCallInfo(this.callInfo);
            uc.modules.component.EmbeddedCallWindowToolBar.updateCallInfo(this.callInfo);
        },
        addAudioClass:function(classType){
            this.callStatusNode.removeClass("icon-call_phoneVideo").removeClass("icon-call_phoneVideoMute")
                .removeClass("icon-call_voipVideo").removeClass("icon-call_voipVideoMute").addClass(classType);
        },
        updateContact: function(basicContactInfo){
            if(this.callInfo && this.callInfo.chatType==uc.constants.Chat.ChatType.SingleChat){
                if(basicContactInfo.siteId){
                    uc.modules.component.EmbeddedCallWindowToolBar.updateContact();
                }
                this.callTitle.text(basicContactInfo.displayName);
            }else{
                uc.modules.component.EmbeddedCallWindowPanel.updateContact(basicContactInfo);
            }
        },

        updateGroup:function(groupInfo){
            this.callTitle.text(groupInfo.name);
            uc.modules.component.EmbeddedCallWindowPanel.updateCallInfo(this.callInfo);
            uc.modules.component.EmbeddedCallWindowToolBar.updateContact();
        },

        close: function(userId){
          //  if(this.callInfo.chatType==uc.constants.Chat.ChatType.SingleChat){
                this.node.removeClass('show').addClass('close');
                $(".embedded-call-window .user-list-up").hide();
                $(".embedded-call-window .user-list-down").hide();
                this.node.find(".p2p-audio-chat-connecting-ball").remove();
	            this.isActivity = false;
           // }else{

         //   }
        },

        clearTimer:function(){
            clearInterval(this.countDurationTimer);
        },

        setMessageCount:function(msgCount){
            uc.modules.component.EmbeddedCallWindowToolBar.updateTextMsgNum(msgCount);
        },

        OnCallQualityChanged:function(quality){
            if(!this.initialized){
                return;
            }

            uc.modules.component.EmbeddedCallWindowToolBar.OnCallQualityChanged(quality);
        },

        OnCallSpeakingStatusChanged:function(userId,isSpeaking){
            if(!this.initialized){
                return;
            }

            if(this.callInfo.chatType==uc.constants.Chat.ChatType.SingleChat){
                if(isSpeaking){
                    this.node.find(".embedded-call-status[data-userId="+userId+"]").addClass("icon-call_voipVideo").removeClass("icon-call_tellCalm").removeClass("icon-call_reporterTell");
                }else{
                    this.node.find(".embedded-call-status[data-userId="+userId+"]").removeClass("icon-call_voipVideo").removeClass("icon-call_tellCalm").addClass("icon-call_reporterTell");
                }
            }else{
                var status=isSpeaking?uc.constants.Call.AcceptorVideoStatus.Speaking:uc.constants.Call.AcceptorVideoStatus.Video;
                uc.modules.component.EmbeddedCallWindowPanel.updateUserListSpeakingStatus(userId,status);
            }

        },

        clearAllStatus:function(){
            this.initialized && this.node.find(".embedded-call-status").removeClass("icon-call_voipVideo").removeClass("icon-call_tellCalm").removeClass("icon-call_reporterTell");
        },

	    OnGetHistoryPhoneNumReceived:function(errorCode,phoneNumbers){
		    if(errorCode == uc.ErrorCode.Success){
			    uc.modules.component.EmbeddedCallWindowToolBar.dialPad.updateCallList(phoneNumbers);
		    }
	    },

        setBtnTurnDown:function(callInfo){
            uc.modules.component.EmbeddedCallWindowToolBar.setHandleDisableBtn(callInfo);
        },

        callTurnToEnd:function(){
            this.callStatusTime.text(uc_resource.Call.CallTurnToEnd);
            this.callTurnEnd.show();
            var _this=this;
	        this.isActivity = false;
            clearTimeout(endTimer);
            var endTimer=setTimeout(function(){
                _this.close();
                _this.callTurnEnd.hide();
                clearTimeout(endTimer);
            },1000);
        },

        updateUserListPanel:function(userList){
            uc.modules.component.EmbeddedCallWindowPanel.updateUserListPanel(userList);
        },

        handleLongTimeNoReceive:function(){
            uc.modules.component.EmbeddedCallWindowPanel.handleLongTimeNoReceive();

        },
        handleLongTimeReceive:function(){
            uc.modules.component.EmbeddedCallWindowPanel.handleLongTimeReceive();

        },
        updateGroupName:function(groupName){
            this.callTitle.text(groupName);
        },
	    IsShowCallNeedAVDetectionTip:function(_isShow){
		    if(!this.initialized){
			    return;
		    }
		    uc.modules.component.EmbeddedCallWindowToolBar.IsShowCallNeedAVDetectionTip(_isShow);
	    },
	    closeCallingpanel:function(){
		    uc.modules.component.EmbeddedCallWindowToolBar.dialpadCallingclear();
	    }

    }
})(uc);