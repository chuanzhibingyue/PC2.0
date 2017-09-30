/**
 * Created by waouyang on 15/12/7.
 */
(function (uc) {
    uc.modules.call.CallManager = {
        callWindowType: uc.constants.Call.CallWindowType.NO_CALL_WINDOW,
        inProgressCallInfo: new uc.modules.call.CallInfo(),
        allCallInfo: [],
        msgCount: 0,
        rejectCallMsg: {},
        selectedConversationInfo: {},
        changeCallUser: [],
        moduleId: uc.constants.UI.ModuleId.Message,
	    callSoundNeedSaveCallStatus:0,

        initGroupCall: function (callInfo) {
            this.inProgressCallInfo.update(callInfo);
            var userList = [];
            var groupName = '';
            for (var userId in callInfo.userList) {
                userList.push(callInfo.userList[userId].userInfo);
            }
            var groupData = this.createGroupData(userList);
            userList = groupData.userList;
            groupName = groupData.groupName;
            uc.util.LogUtil.info("initGroupCall", "initGroupCall", "initGroupCall",
                {groupName: groupName, userList: userList});
            uc.ICallService.createCallByUserList(groupName, userList, function (args) {
                var errorCode = args[0];
                //var chatType=_this.inProgressCallInfo.chatType?
                if (errorCode == uc.ErrorCode.Success) {
                    //this.createGroupCall=args[1];
                } else {
                }
            });
        },


        startCall: function (callInfo, callWindowType, createCall, joinCall) {
            //update in progress call info
            if (!this.inProgressCallInfo.isInProgress && callInfo.isInProgress) {
                this.inProgressCallInfo.update(callInfo);
            }

            if (callInfo.isInProgress) {
                switch (callWindowType) {
                    case uc.constants.Call.CallWindowType.EMBEDDED_CALL_WINDOW:
                        this.initEmbeddedCallWindow(this.inProgressCallInfo);
                        this.callWindowType = uc.constants.Call.CallWindowType.EMBEDDED_CALL_WINDOW;
                        break;
                    case uc.constants.Call.CallWindowType.FLOATING_CALL_WINDOW:
                        this.initFloatingCallWindow(this.inProgressCallInfo);
                        this.callWindowType = uc.constants.Call.CallWindowType.FLOATING_CALL_WINDOW;
                        break;
                    default:
                        break;
                }

                if (createCall) {
                    this._createCall();
                }

                if (joinCall) {
                    this.joinCall();
                }
            }
        },


        /**
         * open embedded call window
         * @param callInfo
         */
        initEmbeddedCallWindow: function (callInfo) {
            uc.modules.call.EmbeddedCallWindow.init(callInfo);
            uc.modules.call.EmbeddedCallWindow.setMessageCount(this.msgCount);
        },
        /**
         * open floating call window
         * @param callInfo
         */
        initFloatingCallWindow: function (callInfo) {
            this.openFloatingCallWindow(callInfo);
        },

        _createCall: function () {
            var _this = this;
            uc.util.LogUtil.info('CallManager', '_createCall', 'createCallById=', {chatId:this.inProgressCallInfo.chatId,chatType:this.inProgressCallInfo.chatType});

            uc.ICallService.createCallById({userId:this.inProgressCallInfo.chatId,siteId:this.inProgressCallInfo.siteId,resId:100}, this.inProgressCallInfo.chatType, function (args) {
                uc.util.LogUtil.info('CallManager', '_createCall', 'callback=',args);

                var errorCode = args[0];

                if (errorCode == uc.ErrorCode.Success) {
                    uc.IUIEvent.OnCallStarted(_this.inProgressCallInfo.chatId, _this.inProgressCallInfo.chatType);
                } else {
                    if (_this.inProgressCallInfo.chatType != uc.constants.Chat.ChatType.SingleChat) {
                        uc.IUIService.triggerCrossWindowEvent(uc.constants.UI.CrossWindowEventName.chooseContactsCreateCall, {callStatus: uc.constants.UI.chooseCallContacts.createcall_fail,failedType:uc.constants.Call.CallFailedType.CreateFailed});
                    }
                    _this._setStartCallFailed();
                    _this.endInProgressCall();
                }
            });
        },

        _setStartCallFailed: function () {
            uc.modules.component.PromptMessageBox.open({
                title: uc_resource.WindowTitle.Notification,
                message: uc_resource.Call.StartCallFailed,
                buttonMode: "ok",
                OkText: uc_resource.Common.OK
            }, function () {
                this.close();
            });
        },

        initEmbeddedCallBanner: function (callInfo) {
            if (!callInfo.siteId) {
                if (callInfo.chatType == uc.constants.Chat.ChatType.SingleChat) {
                    var contactInfo = uc.IContactService.getContactInfo(callInfo.chatId);
                    if (contactInfo) {
                        callInfo.siteId = contactInfo.siteId;
                    } else {
                        callInfo.siteId = 0;
                    }
                } else {
                    var groupInfo = uc.IGroupService.getGroupInfo(callInfo.chatId);
                    if (groupInfo) {
                        callInfo.siteId = groupInfo.siteId;
                    } else {
                        callInfo.siteId = 0;
                    }
                }

            }
            if (callInfo.chatId == this.inProgressCallInfo.chatId && callInfo.chatType == this.inProgressCallInfo.chatType) {
                this.callWindowType = uc.constants.Call.CallWindowType.EMBEDDED_CALL_BANNER;
            }
            uc.modules.call.EmbeddedCallBanner.init(callInfo);
        },


        updateCallInfo: function (callInfo, windowType) {
            this.inProgressCallInfo.update(callInfo);
            if (windowType) {
                this.callWindowType = windowType;
            }
        },

        switchEmbeddedCallWindowToFloating: function () {
            this.closeWindow();
            this.openFloatingCallWindow(this.inProgressCallInfo);
            this.callWindowType = uc.constants.Call.CallWindowType.FLOATING_CALL_WINDOW;
        },

        switchFloatingCallWindowToEmbedded: function (callInfo) {
            switch (callInfo.callStatus) {
                case uc.constants.Call.CallStatus.INVITING:
                case uc.constants.Call.CallStatus.CONNECTING:
                case uc.constants.Call.CallStatus.CONNECTED:
                    this.switchFloatingCallWindowToEmbeddedCallWindow(callInfo);
                    break;
                case uc.constants.Call.CallStatus.INVITED:
                    this.switchFloatingCallWindowToEmbeddedCallBanner(callInfo);
                    break;
                default:
                    break;
            }
        },

        switchFloatingCallWindowToEmbeddedCallWindow: function (callInfo) {
            this.inProgressCallInfo.siteId=callInfo.siteId;
            uc.IUIService.openChatWindow(this.inProgressCallInfo.chatType, this.inProgressCallInfo.chatId, uc.constants.UI.OpenChatWindowSourceModule.Other, null, null, true);
            this.callWindowType = uc.constants.Call.CallWindowType.EMBEDDED_CALL_WINDOW;
            var _this = this;
            setTimeout(function () {
                _this.initEmbeddedCallWindow(_this.inProgressCallInfo);
                uc.IUIEvent.OnCallStarted(_this.inProgressCallInfo.chatId, _this.inProgressCallInfo.chatType);

            }, 500);

        },

        switchFloatingCallWindowToEmbeddedCallBanner: function (callInfo) {
            this.inProgressCallInfo.siteId=callInfo.siteId;
            uc.IUIService.openChatWindow(this.inProgressCallInfo.chatType, this.inProgressCallInfo.chatId, uc.constants.UI.OpenChatWindowSourceModule.Other, null, null, true);
            this.callWindowType = uc.constants.Call.CallWindowType.EMBEDDED_CALL_BANNER;
            var _this = this;
            setTimeout(function () {
                _this.initEmbeddedCallBanner(_this.inProgressCallInfo);
                uc.IUIEvent.OnCallStarted(_this.inProgressCallInfo.chatId, _this.inProgressCallInfo.chatType);

            }, 500);
        },

        switchEmbeddedCallBannerToFloating: function () {
            this.closeWindow();
            this.openFloatingCallWindow(this.inProgressCallInfo);
            this.callWindowType = uc.constants.Call.CallWindowType.FLOATING_CALL_WINDOW;
        },

        switchEmbeddedCallWindowToEmbeddedCallBanner: function () {
            this.closeWindow();
            if (this.inProgressCallInfo.chatId == this.selectedConversationInfo.conversationId
                && this.inProgressCallInfo.chatType == this.selectedConversationInfo.conversationType) {
                uc.modules.call.EmbeddedCallBanner.init(this.inProgressCallInfo);
                this.callWindowType = uc.constants.Call.CallWindowType.EMBEDDED_CALL_BANNER;
            }
        },

        switchEmbeddedCallBannerToEmbeddedCallWindow: function () {
            this.closeWindow();
            if (this.inProgressCallInfo.chatId == this.selectedConversationInfo.conversationId
                && this.inProgressCallInfo.chatType == this.selectedConversationInfo.conversationType) {
                this.msgCount = 0;
                uc.modules.call.EmbeddedCallWindow.init(this.inProgressCallInfo);
                this.callWindowType = uc.constants.Call.CallWindowType.EMBEDDED_CALL_WINDOW;
            }
        },

        closeWindow: function (isEnd) {
            switch (this.callWindowType) {
                case uc.constants.Call.CallWindowType.EMBEDDED_CALL_WINDOW:
	                this.closeEmbeddedCallWindow();
                    break;
                case uc.constants.Call.CallWindowType.EMBEDDED_CALL_BANNER:
	                this.closeEmbeddedCallBanner();
                    break;
                case uc.constants.Call.CallWindowType.FLOATING_CALL_WINDOW:
	                //会出现其他两种窗口还存在，但是callwindowtype已经改变的情况，在此统一清除
	                if(uc.modules.call.EmbeddedCallBanner && uc.modules.call.EmbeddedCallBanner.isActivity){
		                this.closeEmbeddedCallBanner();
	                }
	                if(uc.modules.call.EmbeddedCallWindow && uc.modules.call.EmbeddedCallWindow.isActivity){
		                this.closeEmbeddedCallWindow();
	                }
	                //防止trigger消息发出的时候，float窗口还未bind，添加loaclstore
	                var floating_call_window_isActivity =  uc.util.StorageUtil.getData('floating_call_window_isActivity');
	                //呼浮动窗口还是活动状态才存储
	                if(floating_call_window_isActivity) {
		                uc.util.StorageUtil.setData('floating_call_window_close', isEnd);
	                }
                    uc.IUIService.triggerCrossWindowEvent(uc.constants.UI.CrossWindowEventName.CloseFloatingCallWindow, {isEnd:isEnd});
                    break;
            }
        },
	    closeEmbeddedCallBanner:function(isEnd){
			    uc.modules.call.EmbeddedCallBanner.clearTimer();
			    uc.modules.call.EmbeddedCallWindow.clearTimer();
			    uc.modules.call.EmbeddedCallWindow.clearAllStatus();
			    if(isEnd){
				    uc.modules.call.EmbeddedCallBanner.callTurnToEnd();
			    }else{
				    uc.modules.call.EmbeddedCallBanner.close();
			    }
		},
	    closeEmbeddedCallWindow:function(isEnd){
			    uc.modules.call.EmbeddedCallWindow.clearTimer();
			    uc.modules.call.EmbeddedCallWindow.clearAllStatus();
			    uc.modules.call.EmbeddedCallBanner.clearTimer();
			    if(isEnd){
				    uc.modules.call.EmbeddedCallWindow.callTurnToEnd();
			    }else{
				    uc.modules.call.EmbeddedCallWindow.close();
			    }
	    },

        closeCallInfoBanner:function(callMeetingId){
            uc.modules.call.EmbeddedCallBanner.close(callMeetingId);
        },

        setBtnTurnDown:function(botton,callInfo){
            callInfo.handleBtn=botton;
            switch (this.callWindowType) {
                case uc.constants.Call.CallWindowType.EMBEDDED_CALL_WINDOW:
                    uc.modules.call.EmbeddedCallWindow.setBtnTurnDown(callInfo);
                    break;
                case uc.constants.Call.CallWindowType.EMBEDDED_CALL_BANNER:
                    uc.modules.call.EmbeddedCallBanner.setHandleDisableBtn(callInfo);
                    break;
                case uc.constants.Call.CallWindowType.FLOATING_CALL_WINDOW:
                    this._updateFloatingCallWindowCallInfo();
                    break;
            }
        },

        setBtnTurnUp:function(botton,callInfo){
            callInfo.handleBtn="success";
            switch (this.callWindowType) {
                case uc.constants.Call.CallWindowType.EMBEDDED_CALL_WINDOW:
                    uc.modules.call.EmbeddedCallWindow.setBtnTurnDown(callInfo);
                    break;
                case uc.constants.Call.CallWindowType.EMBEDDED_CALL_BANNER:
                    uc.modules.call.EmbeddedCallBanner.setHandleDisableBtn(callInfo);
                    break;
                case uc.constants.Call.CallWindowType.FLOATING_CALL_WINDOW:
                    this._updateFloatingCallWindowCallInfo();
                    break;
            }
        },
        handleLongTimeReceive:function(){
            switch (this.callWindowType) {
                case uc.constants.Call.CallWindowType.EMBEDDED_CALL_WINDOW:
                    uc.modules.call.EmbeddedCallWindow.handleLongTimeReceive();
                    break;
                case uc.constants.Call.CallWindowType.EMBEDDED_CALL_BANNER:

                    break;
                case uc.constants.Call.CallWindowType.FLOATING_CALL_WINDOW:

                    break;
            }
        },
        handleLongTimeNoReceive:function(){
            switch (this.callWindowType) {
                case uc.constants.Call.CallWindowType.EMBEDDED_CALL_WINDOW:
                    uc.modules.call.EmbeddedCallWindow.handleLongTimeNoReceive();
                    break;
                case uc.constants.Call.CallWindowType.EMBEDDED_CALL_BANNER:
                    uc.modules.component.PromptMessageBox.open({
                        title: uc_resource.WindowTitle.Notification,
                        message: uc_resource.Call.LongTimeNoHandle,
                        buttonMode: "ok",
                        OkText: uc_resource.Common.OK
                    }, function () {
                        this.close();
                    });
                    break;
                case uc.constants.Call.CallWindowType.FLOATING_CALL_WINDOW:
                    uc.modules.component.PromptMessageBox.open({
                        title: uc_resource.WindowTitle.Notification,
                        message: uc_resource.Call.LongTimeNoHandle,
                        buttonMode: "ok",
                        OkText: uc_resource.Common.OK
                    }, function () {
                        this.close();
                    });
                    break;
            }
        },
        updateGroupName:function(groupName){
            switch (this.callWindowType) {
                case uc.constants.Call.CallWindowType.EMBEDDED_CALL_WINDOW:
                    uc.modules.call.EmbeddedCallWindow.updateGroupName(groupName);
                    break;
                case uc.constants.Call.CallWindowType.EMBEDDED_CALL_BANNER:
                    break;
                case uc.constants.Call.CallWindowType.FLOATING_CALL_WINDOW:
                    break;
            }
        },
        onRejectCallInvitationSending: function (arg, reason, callInfo) {
            if (arg[0] == 0) {
                if (reason) {
                    var textMessageData = {
                        errorCode: arg[0],
                        msgId: arg[1],
                        chatType: callInfo.chatType,
                        chatObjectId: callInfo.chatId,
                        messageType: uc.constants.Message.ContentType.Text,
                        contentType: uc.constants.Message.TextMessageType.Text,
                        content: reason,
                        textFormat: uc.modules.setting.SettingManager.getFontSettingFormat()
                    };
                    uc.modules.chat.ChatManager.onRejectCallInvitationSending(callInfo.chatType, callInfo.chatId, textMessageData);

                    uc.IUIEvent.OnMessageSending({
                        chatId: callInfo.chatId,
                        chatType: callInfo.chatType,
                        msgId: arg[1],
                        contentType: uc.constants.Message.ContentType.Text,
                        content: reason
                    });
                }
                this.rejectCallMsg = {msgId:arg[1],callMeetingId:callInfo.callMeetingId};
                uc.util.LogUtil.info("CallManager", "rejectedCall", "rejectCallInvitation",
                    {rejectCallMsg:this.rejectCallMsg});
            } else {
                uc.util.LogUtil.info("CallManager", "rejectedCall", "rejectCallInvitation",
                    {chatInfo: callInfo, messageInfo: reason});
               this.setBtnTurnUp("endCall",callInfo);
            }
        },

        rejectCallInvitation: function (callInfo, reason) {
	        //清除所有callsound
	        uc.util.AudioUtil.stopCallAllSound();
            var _this = this;
            var jid = {
                userId: callInfo.chatId,
                siteId: callInfo.siteId,
                resId: 100
            };
            uc.util.LogUtil.info('CallManager', 'rejectCallInvitation', 'data=', {chatType:callInfo.chatType,jid:jid,callMeetingId:callInfo.callMeetingId,hostId:callInfo.hostId,reason:reason});
            this.setBtnTurnDown("endCall",callInfo);
            if (callInfo.chatType == uc.constants.Chat.ChatType.SingleChat) {
                uc.IIMService.rejectCallInvitation(jid, callInfo.callMeetingId, callInfo.hostId, reason, function (arg) {
                    _this.onRejectCallInvitationSending(arg, reason, callInfo);
                    uc.IUIEvent.OnSaveBeeCallDetailsToPbx({
                        "userID":callInfo.chatId,
                        "incoming":uc.constants.Pbx.CallIncomingType.CallIn,
                        "state":uc.constants.Pbx.CallStatusType.CALL_STATE_Rejected
                    });
                });
            } else {
                uc.IGroupService.rejectCallInvitation(callInfo.chatType, jid, callInfo.callMeetingId, callInfo.hostId, reason, function (arg) {
                    _this.onRejectCallInvitationSending(arg, reason, callInfo);
                });
            }
	        uc.ICallService.getLastAudioInvite(callInfo.chatId);
            //If call is in progress, after reject invitation is sent, end this call
//            if (callInfo.chatId == this.inProgressCalplaySoundlInfo.chatId && callInfo.chatType == this.inProgressCallInfo.chatType) {
//                this.endInProgressCall();
//            }
        },

        cancelCall: function (callInfo) {
	        //清除所有callsound
	        uc.util.AudioUtil.stopCallAllSound();
            if (callInfo && callInfo.siteId) {
                this.inProgressCallInfo.siteId = callInfo.siteId;
            }
            var jid = {
                userId: this.inProgressCallInfo.chatId,
                siteId: this.inProgressCallInfo.siteId,
                resId: 100
            };

            var _this = this;
            var telephoneNumber = this.inProgressCallInfo.audioType == uc.constants.Call.AudioType.VOIP ? '' : this.inProgressCallInfo.phoneNumber + "";

            var chatId = this.inProgressCallInfo.chatId, chatType = this.inProgressCallInfo.chatType;
            uc.util.LogUtil.info('CallManager', 'cancelCall', 'data=', {chatType:this.inProgressCallInfo.chatType,jid:jid,telephoneNumber:telephoneNumber});
            this.setBtnTurnDown("endCall",this.inProgressCallInfo);
            var _this=this;
            if (this.inProgressCallInfo.chatType == uc.constants.Chat.ChatType.SingleChat) {
                if (this.inProgressCallInfo.hostId == this.inProgressCallInfo.loginUserId && this.inProgressCallInfo.callStatus == uc.constants.Call.CallStatus.INVITING) {
                    uc.IIMService.cancelCall(jid, telephoneNumber, function (arg) {
                        if (arg[0] == uc.ErrorCode.Success) {
                            uc.IUIEvent.OnMessageSending({
                                chatId: chatId,
                                chatType: chatType,
                                msgId: arg[1],
                                contentType: uc.constants.Message.ContentType.Text,
                                content: uc.util.StringUtil.formatString(uc_resource.Message.SelfCancelCall, [_this.getMessageUserName(chatId)])
                            });
                        } else{
                            _this.setBtnTurnUp("endCall",_this.inProgressCallInfo);
                        }
                        _this.SaveCallLogToPbx(_this.inProgressCallInfo);
                    });
                }
            } else {
                uc.IGroupService.cancelCall(this.inProgressCallInfo.chatType,jid, telephoneNumber, function (arg) {
                    if (arg[0] == uc.ErrorCode.Success) {
                        uc.IUIEvent.OnMessageSending({
                            chatId: chatId,
                            chatType: chatType,
                            msgId: arg[1],
                            contentType: uc.constants.Message.ContentType.Text,
                            content: uc.util.StringUtil.formatString(uc_resource.Message.SelfCancelGroupCall, null)
                        });
                    }else{
                        _this.setBtnTurnUp("endCall",_this.inProgressCallInfo);
                    }
                });
            }

            this.leaveCall(null,true);

           // this.endInProgressCall();
        },
        sendCallDuration:function(){
            if(!this.inProgressCallInfo.callConnectedTime){
                return;
            }
            var jid = {
                userId: this.inProgressCallInfo.chatId,
                siteId: this.inProgressCallInfo.siteId,
                resId: 100
            };
            var _this = this;

            var telephoneNumber = this.inProgressCallInfo.audioType == uc.constants.Call.AudioType.VOIP ? '' : this.inProgressCallInfo.phoneNumber + "";
            uc.util.LogUtil.info('CallManager', 'sendCallDuration', 'data=', {chatType:this.inProgressCallInfo.chatType,jid:jid,connectedTime:this.inProgressCallInfo.callConnectedTime,telephoneNumber:telephoneNumber});
            var duration = parseInt((Date.now() - this.inProgressCallInfo.callConnectedTime) / 1000);
            var chatType=this.inProgressCallInfo.chatType;
            if(this.inProgressCallInfo.chatType == uc.constants.Chat.ChatType.SingleChat){
               uc.IIMService.sendCallDurationMessage(jid,duration,telephoneNumber,function(arg){
                   if (arg[0] == uc.ErrorCode.Success) {
                       uc.modules.chat.ChatManager.onCallDurationSending(uc.constants.Chat.ChatType.SingleChat,1,jid.userId,duration,arg[1],new Date().getTime());

                       uc.IUIEvent.OnMessageSending({
                           chatId: jid.userId,
                           chatType:  uc.constants.Chat.ChatType.SingleChat,
                           msgId: arg[1],
                           contentType: uc.constants.Message.ContentType.Text,
                           content: uc.util.StringUtil.formatString(uc_resource.Message.CallDuration, [uc.util.TimeUtil.getCallTimeStr(duration)])
                       });
                   }
               })
           } else{
                uc.IGroupService.sendCallDurationMessage(this.inProgressCallInfo.chatType,jid,duration,telephoneNumber,function(arg){
                    if (arg[0] == uc.ErrorCode.Success) {
                        uc.modules.chat.ChatManager.onCallDurationSending(chatType,1,jid.userId,duration,arg[1],new Date().getTime());

                        uc.IUIEvent.OnMessageSending({
                            chatId: jid.userId,
                            chatType: chatType,
                            msgId: arg[1],
                            contentType: uc.constants.Message.ContentType.Text,
                            content: uc_resource.Message.CallEndMessage//uc.util.StringUtil.formatString(uc_resource.Message.CallEndMessage, [uc.util.TimeUtil.getCallTimeStr(duration)])
                        });
                    }
                })
            }
        },
        leaveCall: function (callInfo,isSingleRejected) {
	        //清除所有callsound
	        uc.util.AudioUtil.stopCallAllSound();
            if (callInfo && callInfo.siteId) {
                this.inProgressCallInfo.siteId = callInfo.siteId;
            }
            if (this.inProgressCallInfo.callMeetingId) {
                uc.util.LogUtil.info('CallManager', 'leaveCall', 'data=', {callMeetingId:this.inProgressCallInfo.callMeetingId});
	            uc.IUIService.triggerCrossWindowEvent(uc.constants.UI.CrossWindowEventName.closeScreenShareWindow, {});
	            var _this=this;
                this.setBtnTurnDown("endCall",this.inProgressCallInfo);
                uc.ICallService.leaveCall(this.inProgressCallInfo.callMeetingId, function (arg) {
                    uc.util.LogUtil.info('CallManager', 'LeaveCall', 'errorCode = ', arg);
                    if(arg != uc.ErrorCode.Success){
	                    this.isLeave=false;
	                    _this.setBtnTurnUp("endCall",_this.inProgressCallInfo);
                    }
                });
//                var userLeave=false;
//                for(var userId in this.inProgressCallInfo.userList){
//                    if(this.inProgressCallInfo.userList[userId].callStatus==uc.constants.Call.CallStatus.CONNECTED){
//                        userLeave=true;
//                    }
//                }
//                if(!userLeave && !isSingleRejected){
//                    this.sendCallDuration();
//                }
               // this.endInProgressCall();
            }
        },

        joinCall: function (callInfo) {
            if (callInfo && callInfo.siteId) {
                this.inProgressCallInfo.siteId = callInfo.siteId;
            }

            var _this = this;

            this.setBtnTurnDown("joinCall",this.inProgressCallInfo);

            if (this.inProgressCallInfo.siteId) {
                var pId = {
                    userId: this.inProgressCallInfo.chatId,
                    siteId: this.inProgressCallInfo.siteId,
                    resId: 100
                };
                var meetingId = this.inProgressCallInfo.callMeetingId;
                uc.util.LogUtil.info('CallManager', 'acceptCallInvitation', 'data=', {pId:pId,hostId:_this.inProgressCallInfo.hostId,callMeetingId:meetingId});

                if (this.inProgressCallInfo.chatType == uc.constants.Chat.ChatType.SingleChat) {
                    uc.IIMService.acceptCallInvitation(pId, meetingId, _this.inProgressCallInfo.hostId, function (arg) {
                        if (arg[0] == 0) {
                            uc.util.LogUtil.info('CallManager', 'AcceptCallInvitation', 'P2P Call AcceptCallInvitation Success.', null);
                        } else {

                            _this.setBtnTurnUp("joinCall",_this.inProgressCallInfo);

                            uc.util.LogUtil.info('CallManager', 'AcceptCallInvitation', 'P2P Call AcceptCallInvitation Failed.', null);

                        }
                    });
                } else {
                    uc.IGroupService.acceptCallInvitation(this.inProgressCallInfo.chatType,pId, meetingId, _this.inProgressCallInfo.hostId, function (arg) {
                        if (arg[0] == 0) {
                            uc.util.LogUtil.info('CallManager', 'AcceptCallInvitation', 'Call Call AcceptCallInvitation Success.', null);
                        } else {

                            _this.setBtnTurnUp("joinCall",_this.inProgressCallInfo);

                            uc.util.LogUtil.info('CallManager', 'AcceptCallInvitation', 'Call Call AcceptCallInvitation Failed.', null);
                        }
                    });
                }
            }

            var id = this.inProgressCallInfo.chatType == uc.constants.Chat.ChatType.SingleChat ? 0 : this.inProgressCallInfo.chatId;
            uc.util.LogUtil.info('CallManager', 'Join Call', 'data=', {chatType:_this.inProgressCallInfo.chatType,callMeetingId:_this.inProgressCallInfo.callMeetingId,id:id});

            uc.ICallService.joinCall(_this.inProgressCallInfo.chatType, _this.inProgressCallInfo.callMeetingId, id, function (arg) {
                if (arg[0] == 0) {
                    uc.util.LogUtil.info('CallManager', 'Join Call', 'Call Join Call Success.', null);
                } else {
                    _this.setBtnTurnUp("joinCall",_this.inProgressCallInfo);

                    uc.util.LogUtil.info('CallManager', 'Join Call', 'Call Join Call Failed.', null);
                }
            });

	        uc.ICallService.getLastAudioInvite(this.inProgressCallInfo.chatId);
        },
        isMuteCall:function(callInfo){
            var _this=this;
            this.setBtnTurnDown("muteCall",this.inProgressCallInfo);
	        uc.util.LogUtil.info("callManager","isMuteCall","callInfo", callInfo);
            if(callInfo.muteStatus != uc.constants.Call.MuteStatus.MUTE){
                uc.ICallService.muteSelf(function(arg){
                    if(arg[0]==0){
                    }else{
                        _this.setBtnTurnUp("muteCall",_this.inProgressCallInfo);
                    }
                });

            }else {
                uc.ICallService.unMuteSelf(function(arg){
                    if(arg[0]==0){

                    }else{
                        _this.setBtnTurnUp("muteCall",_this.inProgressCallInfo);
                    }
                });

            }
        },
        /**
         * end current in progress call
         */
        endInProgressCall: function () {
            this.msgCount = 0;
            this.closeWindow(true);
            $(".tool-icon-container").removeClass("startCall");
            $(".tool-icon-container .tool-icon").removeClass("startCall");
            clearTimeout(this.createCallWaiting);
            clearTimeout(this.createCallEnd);
            uc.IUIEvent.OnCallEnded(this.inProgressCallInfo.chatId, this.inProgressCallInfo.chatType);
            //reset in progress call info, and reset call window type
            this.inProgressCallInfo.reset();
            this.callWindowType = uc.constants.Call.CallWindowType.NO_CALL_WINDOW;

        },

        changeToGroup: function (userId) {
            switch (this.callWindowType) {
                case uc.constants.Call.CallWindowType.EMBEDDED_CALL_WINDOW:
                    uc.modules.call.EmbeddedCallWindow.close(userId);
                    break;
                case uc.constants.Call.CallWindowType.EMBEDDED_CALL_BANNER:
                    // if(userId==this.inProgressCallInfo.loginUserId){
                    uc.modules.call.EmbeddedCallBanner.close();
                    // }
                    break;
                case uc.constants.Call.CallWindowType.FLOATING_CALL_WINDOW:
                    uc.util.WindowUtil.close('floatingCall');
                    break;
            }
            uc.IUIEvent.OnCallEnded(this.inProgressCallInfo.chatId, this.inProgressCallInfo.chatType);
        },

        startNewCall: function (userId, chatType) {
		    this.changeToGroup();
            uc.IUIService.openChatWindow(chatType, userId, uc.constants.UI.OpenChatWindowSourceModule.Other, null, null, true);

            this.inProgressCallInfo.chatId = userId;
            this.inProgressCallInfo.chatType = chatType;

            this.startCall(this.inProgressCallInfo, uc.constants.Call.CallWindowType.EMBEDDED_CALL_WINDOW);
            uc.IUIEvent.OnCallStarted(this.inProgressCallInfo.chatId, this.inProgressCallInfo.chatType);
        },

        updateInProgressCallInfo: function (callInfo) {
            if(callInfo){
                this.inProgressCallInfo = callInfo;
            }

            switch (this.callWindowType) {
                case uc.constants.Call.CallWindowType.EMBEDDED_CALL_WINDOW:
                    uc.modules.call.EmbeddedCallWindow.updateCallInfo(this.inProgressCallInfo);
                    break;
                case uc.constants.Call.CallWindowType.EMBEDDED_CALL_BANNER:
                    uc.modules.call.EmbeddedCallBanner.updateCallInfo(this.inProgressCallInfo);
                    break;
                case uc.constants.Call.CallWindowType.FLOATING_CALL_WINDOW:
                    this._updateFloatingCallWindowCallInfo();
                    break;
            }
        },

        _updateFloatingCallWindowCallInfo: function () {
            uc.IUIService.triggerCrossWindowEvent(uc.constants.UI.CrossWindowEventName.UpdateInProgressCallInfo, this.inProgressCallInfo);
        },

        onCallInvitationReceived: function (hostId, chatId, confId, chatType) {
            if (hostId == uc.IClientService.getCurrentUCID().userId) {
                return;
            }

	        this.callSoundNeedSaveCallStatus =  uc.constants.Call.CallStatus.INVITED;

            var callInfo = new uc.modules.call.CallInfo();
            callInfo.update({
                chatId: chatId,
                chatType: chatType,
                callMeetingId: confId,
                hostId: hostId,
                loginUserId: uc.IClientService.getCurrentUCID().userId,
                callStatus: uc.constants.Call.CallStatus.INVITED
            });

            if(confId==this.inProgressCallInfo.callMeetingId && this.inProgressCallInfo.isInProgress){
                if(chatId == this.inProgressCallInfo.chatId && chatType== this.inProgressCallInfo.chatType){
                    return;
                }
                this.startNewCall(chatId, chatType);
                return;
            }
            //In no in progress call, set the first invitation as in progress
            if (!this.inProgressCallInfo.isInProgress) {
                this.inProgressCallInfo = callInfo;
                this.inProgressCallInfo.setIsInProgress(true);
                uc.IUIEvent.OnCallStarted(this.inProgressCallInfo.chatId, this.inProgressCallInfo.chatType);
            }
            if (this.inProgressCallInfo.isInProgress) {
                var isHasCall=this._getCallInfo(chatId, chatType);
                if(isHasCall){
                    this._removeCallInfo(chatId, chatType);
                }
                this._addCallInfo(callInfo);

            }
             //is common chatId Call,but not common meetingId
            if(chatId==this.inProgressCallInfo.chatId && this.inProgressCallInfo.isInProgress
                && chatType==this.inProgressCallInfo.chatType && confId!=this.inProgressCallInfo.callMeetingId){
                this.setCallInfoToProgressCallInfo(this.inProgressCallInfo.callMeetingId,confId);
                return;
            }
            if (this.selectedConversationInfo
                && this.selectedConversationInfo.conversationId == chatId
                && this.selectedConversationInfo.conversationType == chatType
                && this.moduleId==uc.constants.UI.ModuleId.Message
                && uc.IClientService.getWindowStatus() == uc.constants.UI.WindowActiveState.Topmost) {

                this.initEmbeddedCallBanner(callInfo);
            } else if (callInfo.isInProgress) { //If is the first call invitation, show it in float call window
                this.initFloatingCallWindow(callInfo, false);
            }else if(this.selectedConversationInfo
                && this.selectedConversationInfo.conversationId == chatId
                && this.selectedConversationInfo.conversationType == chatType
                && this.moduleId!=uc.constants.UI.ModuleId.Message
                && !callInfo.isInProgress) {
                this.initEmbeddedCallBanner(callInfo);
            }

	        //·接听方未接听时，即使蜜蜂在后台，也需要有铃声ring
	        uc.util.AudioUtil.playCallSound(uc.constants.Call.callMediaType.ringVideo,true);
        },

        setCallInvitationRejected: function (userId, confId) {
	        uc.util.LogUtil.info("callManager", "setCallInvitationRejected", "this.inProgressCallInfo", this.inProgressCallInfo);
            if (this.inProgressCallInfo.callMeetingId == confId) {
                if (this.inProgressCallInfo.chatType == uc.constants.Chat.ChatType.SingleChat) {
                    uc.IUIEvent.OnSaveBeeCallDetailsToPbx({
                        "userID":userId,
                        "incoming":uc.constants.Pbx.CallIncomingType.CallOut,
                        "state":uc.constants.Pbx.CallStatusType.CALL_STATE_Rejected
                    });
                    if (userId == this.inProgressCallInfo.chatId) {
                        uc.modules.component.PromptMessageBox.open({
                            title: uc_resource.WindowTitle.Notification,
                            message: uc_resource.Call.RejectedYourCall,
                            buttonMode: "ok",
                            OkText: uc_resource.Common.OK
                        }, function () {
                            this.close();
                        });
                        if (userId != this.inProgressCallInfo.loginUserId
                            && (this.inProgressCallInfo.callStatus == uc.constants.Call.CallStatus.INVITING
                            || this.inProgressCallInfo.callStatus == uc.constants.Call.CallStatus.CONNECTED)) {
                            this.leaveCall(null,true);
                            //this.setCallInfoToProgressCallInfo();
                        }
                    }else{
                        this._removeCallInfoByMeetingId(confId);
                        this.endInProgressCall();
                        //this.setCallInfoToProgressCallInfo();
                    }

	                //接听方拒绝接听时，发起方需要有声音closeVideo提示
	                uc.util.AudioUtil.playCallSound(uc.constants.Call.callMediaType.closeVideo);
                    //this.endInProgressCall();
                } else {
                    //If other end reject call invitation, end current in progress call
                    if (userId == this.inProgressCallInfo.loginUserId) {
                        this._removeCallInfoByMeetingId(confId);
                        this.endInProgressCall();
                        this.setCallInfoToProgressCallInfo();
                    } else {
                        if (!this.inProgressCallInfo.userList[userId]) {
                            this.inProgressCallInfo.userList[userId] = {};
                        }
                        this.inProgressCallInfo.userList[userId].callStatus=uc.constants.Call.CallStatus.REJECTED;
                        if (this.callWindowType == uc.constants.Call.CallWindowType.EMBEDDED_CALL_WINDOW) {
                            this.updateInProgressCallInfo();
                        }
                    }
                }
            }
        },

        /** IUIEvent **/
        OnOpenChatWindow: function (chatType, chatId) {
            this.selectedConversationInfo.conversationId = chatId;
            this.selectedConversationInfo.conversationType = chatType;

            if (this.inProgressCallInfo.chatId == chatId && this.inProgressCallInfo.chatType == chatType
                || chatType == uc.constants.Conversation.ConversationType.None) {
                if(this.callWindowType != uc.constants.Call.CallWindowType.NO_CALL_WINDOW){
                   if(this.callWindowType != uc.constants.Call.CallWindowType.FLOATING_CALL_WINDOW){
                      this.msgCount=0;
                   }
                    return;
                }
                uc.modules.call.EmbeddedCallBanner.close();
                this.initEmbeddedCallBanner(this.inProgressCallInfo);
                return;
            }

            if (this.inProgressCallInfo.isInProgress) {
                switch (this.callWindowType) {
                    case uc.constants.Call.CallWindowType.EMBEDDED_CALL_WINDOW:
                        this.switchEmbeddedCallWindowToFloating();
                        break;
                    case uc.constants.Call.CallWindowType.EMBEDDED_CALL_BANNER:
                        this.switchEmbeddedCallBannerToFloating();
                        break;

                }
            }

            //show other call invitation
            var callInfo = this._getCallInfo(chatId, chatType);
            if (callInfo) {
                this.initEmbeddedCallBanner(callInfo);
            }
        },
        OnModuleChanged: function (moduleId) {
            this.moduleId = moduleId;

            if (moduleId == uc.constants.UI.ModuleId.Message || !this.inProgressCallInfo.isInProgress) {
                return;
            }
            if (this.callWindowType == uc.constants.Call.CallWindowType.EMBEDDED_CALL_WINDOW) {
                this.switchEmbeddedCallWindowToFloating();
            }
        },

        OnBasicContactInfoReceived: function (basicContactInfos) {
            for (var i = 0, len = basicContactInfos.length; i < len; i++) {
                if (this.inProgressCallInfo.chatType == uc.constants.Chat.ChatType.SingleChat) {
                    if (this.inProgressCallInfo.chatId == basicContactInfos[i].userId) {
                        if (basicContactInfos[i].siteId) {
                            this.inProgressCallInfo.siteId = basicContactInfos[i].siteId;
                        }
                        uc.modules.call.EmbeddedCallWindow.updateContact(basicContactInfos[i]);
                        uc.modules.call.EmbeddedCallBanner.updateContact(basicContactInfos[i]);
                        break;
                    }
                } else {
                    for (var userId in this.inProgressCallInfo.userList) {
                        if (userId == basicContactInfos[i].userId) {
                            if(!this.inProgressCallInfo.userList[userId]){
                                this.inProgressCallInfo.userList[userId]={};
                                this.inProgressCallInfo.userList[userId].userInfo={};
                            }
                            if(!this.inProgressCallInfo.userList[userId].userInfo){
                                this.inProgressCallInfo.userList[userId].userInfo={};
                            }
                            this.inProgressCallInfo.userList[userId].userInfo.displayName = basicContactInfos[i].displayName;
                            this.inProgressCallInfo.userList[userId].userInfo.localAvatar = basicContactInfos[i].localAvatar;
                            uc.modules.call.EmbeddedCallWindow.updateContact(basicContactInfos[i]);
                        }
                    }
                }

            }
        },

        OnGroupInfoReceived: function (group) {
            if (group && group.groupId == this.inProgressCallInfo.chatId) {
                this.inProgressCallInfo.siteId = group.siteId;
                for(var i=0;i<group.members.length;i++){
                    if(!this.inProgressCallInfo.userList[group.members[i].userId]){
                        this.inProgressCallInfo.userList[group.members[i].userId]={};
                    }
                }
                uc.modules.call.EmbeddedCallWindow.updateGroup(group);
                uc.modules.call.EmbeddedCallBanner.updateContact(group);
            }
        },

        _addCallInfo: function (callInfo) {
            this.allCallInfo.push(callInfo);
        },

        _getCallInfo: function (chatId, chatType) {
            var callInfo;
            for (var i = 0, len = this.allCallInfo.length; i < len; i++) {
                callInfo = this.allCallInfo[i];
                if (callInfo && callInfo.chatId == chatId && callInfo.chatType == chatType) {
                    return callInfo;
                }
            }
            return null;
        },

        _removeCallInfo: function (chatId, chatType) {
            var callInfo;
            for (var i = 0, len = this.allCallInfo.length; i < len; i++) {
                callInfo = this.allCallInfo[i];
                if (callInfo && callInfo.chatId == chatId && callInfo.chatType == chatType) {
                    delete this.allCallInfo[i];
                    break;
                }
            }
        },

        _removeCallInfoByMeetingId: function (MeetingId) {
            var callInfo;
            for (var i = 0, len = this.allCallInfo.length; i < len; i++) {
                callInfo = this.allCallInfo[i];
                if (callInfo && callInfo.callMeetingId == MeetingId) {
                    delete this.allCallInfo[i];
                    break;
                }
            }
        },

        setCallInfoToProgressCallInfo:function(meetingId,newMeetingId){
            var callInfo;
            for (var i = 0, len = this.allCallInfo.length; i < len; i++) {
                callInfo = this.allCallInfo[i];
                if (callInfo && callInfo.callMeetingId && callInfo.callMeetingId != meetingId ) {
                    if(newMeetingId && newMeetingId == callInfo.callMeetingId){
                        this.inProgressCallInfo=callInfo;
                    }else{
                        this.inProgressCallInfo=callInfo;
                    }
                    this.inProgressCallInfo.setIsInProgress(true);
                    //if (this.inProgressCallInfo.chatId == this.selectedConversationInfo.conversationId
                     //   && this.inProgressCallInfo.chatType == this.selectedConversationInfo.conversationType){
                    if(this.callWindowType == uc.constants.Call.CallWindowType.NO_CALL_WINDOW ||
                        this.callWindowType == uc.constants.Call.CallWindowType.EMBEDDED_CALL_BANNER){
	                    uc.modules.call.EmbeddedCallBanner.close();
	                    this.switchFloatingCallWindowToEmbeddedCallBanner(this.inProgressCallInfo);
                    }else{
                        uc.util.WindowUtil.close('floatingCall');
                        this.initFloatingCallWindow(this.inProgressCallInfo);
                    }

                  //  }
                    uc.IUIEvent.OnCallStarted(this.inProgressCallInfo.chatId, this.inProgressCallInfo.chatType);

                    break;
                }
            }

        },

        isInCall: function () {
            return this.inProgressCallInfo.callStatus == uc.constants.Call.CallStatus.INVITING
                || this.inProgressCallInfo.callStatus == uc.constants.Call.CallStatus.CONNECTING
                || this.inProgressCallInfo.callStatus == uc.constants.Call.CallStatus.CONNECTED;
        },

        openFloatingCallWindow: function (callInfo) {
            $(".tool-icon-container").removeClass("startCall");
            $(".tool-icon-container .tool-icon").removeClass("startCall");
            uc.util.StorageUtil.setData('floating_call_window_call_info', callInfo);
	        uc.util.StorageUtil.setData('floating_call_window_isActivity',true);
            var options = {
                title: uc_resource.Common.AppName,
                top: 20,
                left: screen.width - 360,
                width: 360,
                height: 260,
                refreshIfExist: false,
                centerScreen: false,
                modal: false,
                resizable: false,
                isTop: true,
                hasTaskBarIcon: false,
                behaviorIfSCClose: uc.constants.UI.WindowCloseBehavior.Close,
                enableDrag: false,
                isChild: false,
                caption: uc.constants.UI.DragCaption.FloatingCallWindow
            };
            uc.util.WindowUtil.open('page/floatingCallWindow.html', 'floatingCall', options);

            this.callWindowType = uc.constants.Call.CallWindowType.FLOATING_CALL_WINDOW;
        },

        //Call EVENT
        OnCallCreated: function (errorCode, meetingId, chatId, chatType) {
            if (errorCode == uc.ErrorCode.Success) {
                if(!this.inProgressCallInfo.userList[this.inProgressCallInfo.loginUserId]){
                    this.inProgressCallInfo.userList[this.inProgressCallInfo.loginUserId]={};
                }
                this.inProgressCallInfo.userList[this.inProgressCallInfo.loginUserId].callStatus=uc.constants.Call.CallStatus.CONNECTED;
                this.inProgressCallInfo.userList[this.inProgressCallInfo.loginUserId].muteStatus =uc.constants.Call.MuteStatus.UN_MUTE;
                this.inProgressCallInfo.userList[this.inProgressCallInfo.loginUserId].audioType = uc.constants.Call.AudioType.VOIP;
                this.inProgressCallInfo.userList[this.inProgressCallInfo.loginUserId].phoneNumber = '';
                if (chatType != uc.constants.Chat.ChatType.SingleChat) {
                    this.inProgressCallInfo.callStatus = uc.constants.Call.CallStatus.CONNECTED;
                    this.inProgressCallInfo.callConnectedTime = Date.now();
                    uc.IUIService.triggerCrossWindowEvent(uc.constants.UI.CrossWindowEventName.chooseContactsCreateCall, {callStatus: uc.constants.UI.chooseCallContacts.createCall_Ok});
                    uc.IUIService.triggerCrossWindowEvent(uc.constants.UI.CrossWindowEventName.CloseGroupProfile,{});
                }else{
                    clearTimeout(this.createCallWaiting);
                    var _this=this;
                    var callData = uc.util.StorageUtil.getData("CallManager-callInfo:inProgressCallInfo");
                    if(callData && meetingId == callData.callMeetingId && callData.callStatus == uc.constants.Call.CallStatus.CONNECTED){
                         return;
                    }
                    this.createCallWaiting=setTimeout(function(){
                        _this.handleLongTimeNoReceive();
                        clearTimeout(_this.createCallWaiting);
                    },20000);
                    clearTimeout(this.createCallEnd);
                    this.createCallEnd=setTimeout(function(){
                        _this.cancelCall(_this.inProgressCallInfo);
	                    //接听方长时间未接听，自动挂断，发起方需要有声音closeVideo提示
	                    uc.util.AudioUtil.playCallSound(uc.constants.Call.callMediaType.closeVideo);

                        clearTimeout(_this.createCallEnd);
                    },60000);
                }

                if (this.inProgressCallInfo.chatType == uc.constants.Chat.ChatType.SingleChat) {
                    this.inProgressCallInfo.callStatus = uc.constants.Call.CallStatus.INVITING;
                }

                this.inProgressCallInfo.callMeetingId = meetingId;
                this.inProgressCallInfo.muteStatus = uc.constants.Call.MuteStatus.UN_MUTE;

                if (!this.inProgressCallInfo.chatId) {
                    this.startNewCall(chatId, chatType);
                }

                this.updateInProgressCallInfo();
	            //•发起方等待对方接听时，发起方有等待铃声ring,
	            //在接收方没有回应，循环播放，100次
	            uc.util.AudioUtil.playCallSound(uc.constants.Call.callMediaType.ringVideo,true);
            } else {
                if(chatType == uc.constants.Chat.ChatType.SingleChat){
                    uc.modules.component.PromptMessageBox.open({
                        title: uc_resource.WindowTitle.Notification,
                        message:uc_resource.Call.CreateCallFailed,
                        buttonMode: "ok",
                        OkText: uc_resource.Common.OK
                    }, function () {
                        this.close();
                    });
                }else{
                    uc.IUIService.triggerCrossWindowEvent(uc.constants.UI.CrossWindowEventName.chooseContactsCreateCall, {callStatus: uc.constants.UI.chooseCallContacts.createcall_fail,failedType:uc.constants.Call.CallFailedType.CreateFailed});
                }
                this._removeCallInfo(chatId, chatType);
                this.endInProgressCall();
            }
        },

        OnSelfJoinedCall: function (errorCode, meetingId, chatId, chatType) {
	        //清除所有callsound
	        uc.util.AudioUtil.stopCallAllSound();
            if (this.inProgressCallInfo.callMeetingId == meetingId) {
                this.setBtnTurnUp("joinCall",this.inProgressCallInfo);
                if (errorCode == uc.ErrorCode.Success) {
                    //For group chat, self join call will set call connected, for single chat, onl other join call indicate call connected
                    if (this.inProgressCallInfo.chatType != uc.constants.Chat.ChatType.SingleChat
                        || (this.inProgressCallInfo.chatType == uc.constants.Chat.ChatType.SingleChat &&
                        this.inProgressCallInfo.hostId != this.inProgressCallInfo.loginUserId)) {
                        this.inProgressCallInfo.callStatus = uc.constants.Call.CallStatus.CONNECTED;
                        this.inProgressCallInfo.callConnectedTime = this.inProgressCallInfo.callConnectedTime ? this.inProgressCallInfo.callConnectedTime:Date.now();
                    }

                    if (!this.inProgressCallInfo.userList[this.inProgressCallInfo.loginUserId]) {
                        this.inProgressCallInfo.userList[this.inProgressCallInfo.loginUserId] = {};
                    }
                    this.inProgressCallInfo.userList[this.inProgressCallInfo.loginUserId].callStatus=uc.constants.Call.CallStatus.CONNECTED;
                    //this.inProgressCallInfo.callStatus = uc.constants.Call.CallStatus.CONNECTED;

                    this.updateInProgressCallInfo();

	                //·接听方点击接听时，接听方需要有声音videoRingIn提示接通
	                uc.util.AudioUtil.playCallSound(uc.constants.Call.callMediaType.ringInVideo);
                } else {
                    uc.modules.component.PromptMessageBox.open({
                        title: uc_resource.WindowTitle.Notification,
                        message:uc_resource.Call.SelfJoinCallFailed,
                        buttonMode: "ok",
                        OkText: uc_resource.Common.OK
                    }, function () {
                        this.close();
                    });
                    this._removeCallInfoByMeetingId(meetingId);
                    this.endInProgressCall();
                    this.setCallInfoToProgressCallInfo(meetingId);
                }
            }else{
                if (errorCode == uc.ErrorCode.Success){
                    uc.util.LogUtil.info('CallManager-OnSelfJoinedCall', 'leaveCall', 'data=', {callMeetingId:this.inProgressCallInfo.callMeetingId,meetingId:meetingId});
                    var _this=this;
                    uc.ICallService.leaveCall(meetingId, function (arg) {
                        uc.util.LogUtil.info('CallManager-OnSelfJoinedCall', 'LeaveCall', 'errorCode = ', arg[0]);
                    });
                }
            }
        },

        OnUserJoinedCall: function (meetingId, userId, isMute, audioType, phoneNumber) {
	        uc.util.LogUtil.info('CallManager', 'OnUserJoinedCall', 'inProgressCallInfo', this.inProgressCallInfo);
            if (this.inProgressCallInfo.callMeetingId == meetingId) {
                if (!this.inProgressCallInfo.userList[userId]) {
                    this.inProgressCallInfo.userList[userId] = {};
                    this.inProgressCallInfo.userList[userId].userInfo = {};
                }

                if(userId == this.inProgressCallInfo.loginUserId){
	                //group A呼BC（B不在线），B上线后，点击群聊呼，提示当前群聊正在呼中。确认加入，发送接受邀请消息，成功返回，需要通知chat和conversition更改conversationInfo.isInCall状态
	                uc.IUIEvent.OnCallStarted(this.inProgressCallInfo.chatId, this.inProgressCallInfo.chatType);

                    this.setBtnTurnUp("joinCall",this.inProgressCallInfo);
                    this.inProgressCallInfo.update({
                        muteStatus: isMute ? uc.constants.Call.MuteStatus.MUTE : uc.constants.Call.MuteStatus.UN_MUTE,
                        audioType: audioType,
                        phoneNumber: phoneNumber,
                        callStatus: this.inProgressCallInfo.chatType != uc.constants.Chat.ChatType.SingleChat ? uc.constants.Call.CallStatus.CONNECTED : this.inProgressCallInfo.callStatus
                    });
                        this.inProgressCallInfo.callConnectedTime = this.inProgressCallInfo.callConnectedTime ? this.inProgressCallInfo.callConnectedTime:Date.now();
                    //if(this.inProgressCallInfo.userList[this.inProgressCallInfo.loginUserId]){
                        this.inProgressCallInfo.userList[this.inProgressCallInfo.loginUserId].callStatus=uc.constants.Call.CallStatus.CONNECTED;
                        this.inProgressCallInfo.userList[this.inProgressCallInfo.loginUserId].muteStatus = isMute ? uc.constants.Call.MuteStatus.MUTE : uc.constants.Call.MuteStatus.UN_MUTE;
                        this.inProgressCallInfo.userList[this.inProgressCallInfo.loginUserId].audioType = audioType;
                        this.inProgressCallInfo.userList[this.inProgressCallInfo.loginUserId].phoneNumber = phoneNumber;
                  //  }
                }else {
                    this.inProgressCallInfo.userList[userId].callStatus=uc.constants.Call.CallStatus.CONNECTED;
                    this.inProgressCallInfo.userList[userId].muteStatus = isMute ? uc.constants.Call.MuteStatus.MUTE : uc.constants.Call.MuteStatus.UN_MUTE;
                    this.inProgressCallInfo.userList[userId].audioType = audioType;
                    this.inProgressCallInfo.userList[userId].phoneNumber = phoneNumber;
                }

                if (userId != this.inProgressCallInfo.loginUserId) {
                    //For single chat, other joined indicate connected
                    if (this.inProgressCallInfo.chatType == uc.constants.Chat.ChatType.SingleChat) {
                        clearTimeout(this.createCallWaiting);
                        this.handleLongTimeReceive();
                        clearTimeout(this.createCallEnd);
                        this.inProgressCallInfo.callStatus = uc.constants.Call.CallStatus.CONNECTED;
                        this.inProgressCallInfo.callConnectedTime = Date.now();
                    }
                } else {
                    //For group chat, self join call will set call connected
//                    if (this.inProgressCallInfo.chatType == uc.constants.Chat.ChatType.GroupChat
//                        || this.inProgressCallInfo.chatType == uc.constants.Chat.ChatType.ProjectChat) {
//
//                        this.inProgressCallInfo.callStatus = uc.constants.Call.CallStatus.CONNECTED;
//                        this.inProgressCallInfo.callConnectedTime = Date.now();
//                    }
                }

                this.updateInProgressCallInfo();

	            //接听方点击接听时，发起方需要有震动+声音videoRingIn提示接通
	            uc.util.AudioUtil.playCallSound(uc.constants.Call.callMediaType.ringInVideo);
            }
        },

        OnUserLeftCall: function (meetingId, userId) {
            if (this.inProgressCallInfo.callMeetingId == meetingId) {
                //For single chat,
                if (this.inProgressCallInfo.chatType == uc.constants.Chat.ChatType.SingleChat) {
                    if (userId != this.inProgressCallInfo.loginUserId && this.inProgressCallInfo.callStatus == uc.constants.Call.CallStatus.CONNECTED) {
                        this.inProgressCallInfo.userList[userId].callStatus=uc.constants.Call.CallStatus.DISCONNECTED;
                       // this.leaveCall();
                    } else if (userId == this.inProgressCallInfo.loginUserId) {
                        this._removeCallInfo(userId, this.inProgressCallInfo.chatType);
                    }

                } else {
                    if (userId == this.inProgressCallInfo.loginUserId) {

                        this.endInProgressCall();
                        this._removeCallInfo(userId, this.inProgressCallInfo.chatType);

                    } else {
                        if (this.inProgressCallInfo.userList[userId]) {
                            this.inProgressCallInfo.userList[userId].callStatus=uc.constants.Call.CallStatus.DISCONNECTED;
                            if(this.callWindowType == uc.constants.Call.CallWindowType.EMBEDDED_CALL_WINDOW){
                                this.updateInProgressCallInfo();
                            }
                        }
                    }
                }
            }
        },

        OnSelfLeftCall: function (errorCode, meetingId, chatId, chatType) {
            if (this.inProgressCallInfo.callMeetingId == meetingId) {
                this.setBtnTurnUp("endCall",this.inProgressCallInfo);
                if (errorCode == uc.ErrorCode.Success) {
                    this.getCurrentCallStatus(this.inProgressCallInfo,chatType);
                    uc.util.StorageUtil.removeData("CallManager-callInfo:inProgressCallInfo");
                    if(this.isLeave){
                         this.startCall(this.inProgressCallInfo, uc.constants.Call.CallWindowType.FLOATING_CALL_WINDOW, false, true);
                        this.isLeave=false;
                    }else{
                        this.endInProgressCall();
                        this._removeCallInfo(chatId, chatType);
                        this.setCallInfoToProgressCallInfo(meetingId);
                    }
	                //group一方退出通话时，退出方需要有声音videoRingIn提示
	                uc.util.AudioUtil.playCallSound(uc.constants.Call.callMediaType.ringInVideo);
                }else{
                    this.isLeave=false;
                    uc.modules.component.PromptMessageBox.open({
                        title: uc_resource.WindowTitle.Notification,
                        message:uc_resource.Call.leaveCallFailed,
                        buttonMode: "ok",
                        OkText: uc_resource.Common.OK
                    }, function () {
                        this.close();
                    });
                }
            }
        },

        OnCallAllLeft: function (errorCode, meetingId,callType) {
           // if(errorCode==uc.ErrorCode.Success){
            this._removeCallInfoByMeetingId(meetingId);
            if (meetingId == this.inProgressCallInfo.callMeetingId) {
                uc.util.StorageUtil.removeData("CallManager-callInfo:inProgressCallInfo");
	            uc.IUIService.triggerCrossWindowEvent(uc.constants.UI.CrossWindowEventName.closeScreenShareWindow, {});
                this.getCurrentCallStatus(this.inProgressCallInfo,callType);
                this.sendCallDuration();
                this.endInProgressCall();
                this.setCallInfoToProgressCallInfo(meetingId);
	            //group通话结束时，最后退出的人需要有声音videoRingIn提示
	            //p2p 通话结束时，发起方和接收方都应该有声音videoRingIn提示
	            if( this.callSoundNeedSaveCallStatus  == uc.constants.Call.CallStatus.INVITED)
	            {
		            uc.util.AudioUtil.playCallSound(uc.constants.Call.callMediaType.ringInVideo);
		            this.callSoundNeedSaveCallStatus =  0;
	            }
            } else{
	            //清除所有callsound
	            uc.util.AudioUtil.stopCallAllSound();

            }
        },
        SaveCallLogToPbx:function(callInfo){
            uc.IUIEvent.OnSaveBeeCallDetailsToPbx({
                "userID":callInfo.chatId,
                "incoming":uc.constants.Pbx.CallIncomingType.CallOut,
                "state":uc.constants.Pbx.CallStatusType.CALL_STATE_HANGUP
            });
        },
        getCurrentCallStatus:function(data,callType){
            if(callType == uc.constants.Chat.ChatType.SingleChat){
                var myUserInfo = uc.IContactService.getCurrentUserInfo();
                var incoming;
                if(data.hostId != myUserInfo.userId){
                    incoming = uc.constants.Pbx.CallIncomingType.CallIn;
                    uc.IUIEvent.OnSaveBeeCallStatusToPbx({"incoming":incoming});
                }else{
                    incoming = uc.constants.Pbx.CallIncomingType.CallOut;
                    uc.IUIEvent.OnSaveBeeCallStatusToPbx({"incoming":incoming});
                }
            }
        },
        OnUserCallPropertyChanged: function (meetingId, userId, isMute, audioType, phoneNumber, callPropertyChangedType) {
            if (this.inProgressCallInfo.callMeetingId == meetingId) {
                if (userId != this.inProgressCallInfo.loginUserId) {
                    if (!this.inProgressCallInfo.userList[userId]) {
                        this.inProgressCallInfo.userList[userId] = {};
                        this.inProgressCallInfo.userList[userId].userInfo = {};
                    }
                    this.inProgressCallInfo.userList[userId].callStatus=uc.constants.Call.CallStatus.CONNECTED;
                    this.inProgressCallInfo.userList[userId].muteStatus = isMute ? uc.constants.Call.MuteStatus.MUTE : uc.constants.Call.MuteStatus.UN_MUTE;
                    this.inProgressCallInfo.userList[userId].audioType = audioType;
                    this.inProgressCallInfo.userList[userId].phoneNumber = phoneNumber;

                }
                if (userId == this.inProgressCallInfo.loginUserId) {
	                //如果audioType有变化，说明执行了切换语音的操作，需要关闭calling界面
	                if(audioType == uc.constants.Call.AudioType.PHONE && this.inProgressCallInfo.audioType != audioType){
		                uc.modules.call.EmbeddedCallWindow.closeCallingpanel();
	                }
                    this.inProgressCallInfo.audioType = audioType;
                    this.inProgressCallInfo.phoneNumber = phoneNumber;
                    this.inProgressCallInfo.muteStatus = isMute ? uc.constants.Call.MuteStatus.MUTE : uc.constants.Call.MuteStatus.UN_MUTE;
                    this.inProgressCallInfo.userList[this.inProgressCallInfo.loginUserId].callStatus=uc.constants.Call.CallStatus.CONNECTED;
                    this.inProgressCallInfo.userList[this.inProgressCallInfo.loginUserId].muteStatus =isMute ? uc.constants.Call.MuteStatus.MUTE : uc.constants.Call.MuteStatus.UN_MUTE;
                    this.inProgressCallInfo.userList[this.inProgressCallInfo.loginUserId].audioType = audioType;
                    this.inProgressCallInfo.userList[this.inProgressCallInfo.loginUserId].phoneNumber = phoneNumber;
                    this.updateInProgressCallInfo();
                    this.setBtnTurnUp("muteCall",this.inProgressCallInfo);
                }else {
                    //only group call and Call Window, need update Call
                    if(this.callWindowType == uc.constants.Call.CallWindowType.EMBEDDED_CALL_WINDOW ||
		                this.callWindowType == uc.constants.Call.CallWindowType.FLOATING_CALL_WINDOW){
		                this.updateInProgressCallInfo();
	                }
                }
            }
        },

        increaseNewMsgCount: function(conversationId, conversationType){
            if (this.callWindowType == uc.constants.Call.CallWindowType.EMBEDDED_CALL_WINDOW
                    && this.inProgressCallInfo.chatId == conversationId
                    && this.inProgressCallInfo.chatType == conversationType) {
                this.msgCount++;
                uc.modules.call.EmbeddedCallWindow.setMessageCount(this.msgCount);
            }
        },

        clearNewMsgCount: function(conversationId, conversationType){
            if (this.callWindowType == uc.constants.Call.CallWindowType.EMBEDDED_CALL_WINDOW
                && this.inProgressCallInfo.chatId == conversationId
                && this.inProgressCallInfo.chatType == conversationType) {
                this.msgCount = 0;
                uc.modules.call.EmbeddedCallWindow.setMessageCount(this.msgCount);
            }
        },

        OnCallQualityChanged: function (meetingId, userId, chatType, quality) {
            if (meetingId == this.inProgressCallInfo.callMeetingId) {
                if (this.callWindowType == uc.constants.Call.CallWindowType.EMBEDDED_CALL_WINDOW) {
                    uc.modules.call.EmbeddedCallWindow.OnCallQualityChanged(quality);
                }
            }
        },

        OnCallSpeakingStatusChanged: function (meetingId, userId, chatType, isSpeaking) {
            if (chatType==uc.constants.Chat.ChatType.SingleChat) {
                return;
            }
            if (meetingId == this.inProgressCallInfo.callMeetingId) {
                if (this.callWindowType == uc.constants.Call.CallWindowType.EMBEDDED_CALL_WINDOW) {
                    uc.modules.call.EmbeddedCallWindow.OnCallSpeakingStatusChanged(userId, isSpeaking);
                }
            }
        },

        OnCallPhoneFailed: function (errorCode, meetingId, phoneNumber) {
            if (errorCode == uc.ErrorCode.Success) {
                if (meetingId == this.inProgressCallInfo.callMeetingId) {

                    uc.modules.component.PromptMessageBox.open({
                        title: uc_resource.WindowTitle.Notification,
                        message: uc_resource.Call.PhoneFailed,
                        buttonMode: "ok",
                        OkText: uc_resource.Common.OK
                    }, function () {
                        this.close();
                    });
                }
            }
        },

        OnMessageSent: function (errorCode, userId, msgId, msgSeq, timestamp) {
            if (errorCode == uc.ErrorCode.Success) {
                if (this.rejectCallMsg && msgId == this.rejectCallMsg.msgId) {
                    if(this.rejectCallMsg.callMeetingId == this.inProgressCallInfo.callMeetingId){
                        this._removeCallInfoByMeetingId(this.rejectCallMsg.callMeetingId);
                        this.endInProgressCall();
                        this.setCallInfoToProgressCallInfo(this.rejectCallMsg.callMeetingId);
                    }else{
                       this._removeCallInfoByMeetingId(this.rejectCallMsg.callMeetingId);
                       this.closeCallInfoBanner(this.rejectCallMsg.callMeetingId);
                    }
                }
            }
        },

        //P2P call
        OnP2PCallInvitationReceived: function (channel, userId,fromUserId, confId, confPwd, hostId, msgId, msgSeq, timestamp) {
            if (channel != uc.constants.Message.MessageChannel.UCAS) {
                return;
            }
            this.onCallInvitationReceived(hostId, userId, confId, uc.constants.Chat.ChatType.SingleChat);
        },

        OnP2PCallInvitationAccepted: function (channel, userId,fromUserId, confId, msgId, msgSeq, timestamp) {
            if (channel != uc.constants.Message.MessageChannel.UCAS) {
                return;
            }
	        //·接听方点击接听时，接听方需要有声音videoRingIn提示接通
	        uc.util.AudioUtil.playCallSound(uc.constants.Call.callMediaType.ringInVideo);
            //If other client accept the call, end this call in this client
            if (fromUserId == this.inProgressCallInfo.loginUserId && fromUserId != this.inProgressCallInfo.hostId && confId == this.inProgressCallInfo.callMeetingId) {
                this.endInProgressCall();
            }
	        this.callSoundNeedSaveCallStatus =  uc.constants.Call.CallStatus.INVITED;
        },

        OnP2PCallInvitationRejected: function (channel, userId,fromUserId, confId, hostId, reason, msgId, msgSeq, timestamp) {
            if (channel != uc.constants.Message.MessageChannel.UCAS) {
                return;
            }
            this.setCallInvitationRejected(fromUserId, confId);
        },

        OnP2PCallCancelled: function (channel, userId,fromUserId, msgId, msgSeq, timestamp) {
            if (channel != uc.constants.Message.MessageChannel.UCAS) {
                return;
            }
            var inComing = uc.constants.Pbx.CallIncomingType.CallIn;
            if (fromUserId == uc.IClientService.getCurrentUCID().userId) {
                inComing = uc.constants.Pbx.CallIncomingType.CallOut;
            }
            uc.IUIEvent.OnSaveBeeCallDetailsToPbx({
                "userID":userId,
                "incoming":inComing,
                "state":uc.constants.Pbx.CallStatusType.CALL_STATE_HANGUP,
                "time":timestamp
            });
	        uc.util.LogUtil.info("callmanager","OnP2PCallCancelled",'this.inProgressCallInfo',this.inProgressCallInfo);
	        //清除所有callsound
	        uc.util.AudioUtil.stopCallAllSound();
	        uc.IUIService.triggerCrossWindowEvent(uc.constants.UI.CrossWindowEventName.closeScreenShareWindow, {});
            if (fromUserId == this.inProgressCallInfo.chatId) {
                if (this.inProgressCallInfo.callStatus == uc.constants.Call.CallStatus.CONNECTED) {
                    this.leaveCall();
                } else {
                    this.endInProgressCall();
                    this.setCallInfoToProgressCallInfo(this.inProgressCallInfo.callMeetingId);
                }
            } else if (this.selectedConversationInfo && userId == this.selectedConversationInfo.conversationId
                && this.selectedConversationInfo.conversationType == uc.constants.Conversation.ConversationType.SingleChat) {
                uc.modules.call.EmbeddedCallBanner.close();
            }
            this._removeCallInfo(fromUserId, uc.constants.Chat.ChatType.SingleChat);

        },

        //Group
        OnGroupCallInvitationReceived: function (channel, conversationType, groupId, confId, confPwd, hostId, msgId, msgSeq, timestamp) {
            if (channel != uc.constants.Message.MessageChannel.UCAS) {
                return;
            }
            this.onCallInvitationReceived(hostId, groupId, confId, conversationType);
        },

        OnGroupCallInvitationAccepted: function (channel, conversationType, groupId, confId, userId,msgId, msgSeq, timestamp) {
            if (channel != uc.constants.Message.MessageChannel.UCAS) {
                return;
            }
	        //如果正在接听状态，并且接受到了其他人的接受消息，不相应videoRingIn
	        if(!(this.inProgressCallInfo.callStatus == uc.constants.Call.CallStatus.INVITED &&this.inProgressCallInfo.loginUserId != userId )){
		        //·接听方点击接听时，接听方需要有声音videoRingIn提示接通
		        uc.util.AudioUtil.playCallSound(uc.constants.Call.callMediaType.ringInVideo);
	        }
            //Other client accept the call, end this client's call
            if (groupId == this.inProgressCallInfo.chatId && conversationType == this.inProgressCallInfo.chatType && this.inProgressCallInfo.loginUserId == userId && confId == this.inProgressCallInfo.callMeetingId) {
                this.endInProgressCall();
            }
	        this.callSoundNeedSaveCallStatus =  uc.constants.Call.CallStatus.INVITED;
        },

        OnGroupCallInvitationRejected: function (channel, conversationType, groupId, confId, userId, msgId, msgSeq, timestamp) {
            if (channel != uc.constants.Message.MessageChannel.UCAS) {
                return;
            }
	        //如果正在接听状态，并且接受到了其他人的拒绝消息，应该继续playsound ring
	        if(!(this.inProgressCallInfo.callStatus == uc.constants.Call.CallStatus.INVITED &&this.inProgressCallInfo.loginUserId != userId )){
		        //清除所有callsound
		        uc.util.AudioUtil.stopCallAllSound();
	        }
            this.setCallInvitationRejected(userId, confId);
        },
        OnGroupNameUpdatedMessageReceived: function(conversationType, channel, from, to, operatorId, groupId, groupName, namePinYin, msgId, msgSeq, msgTime){
            if (channel != uc.constants.Message.MessageChannel.UCAS) {
                return;
            }
            if (groupId == this.inProgressCallInfo.chatId && conversationType == this.inProgressCallInfo.chatType) {
                this.updateGroupName(groupName);
            }
        },
        getMessageUserName: function (userId) {
            var userName;
            if (userId == this.loginUerId) {
                userName = uc_resource.Message.You;
            } else {
                var contactInfo = uc.IContactService.getBasicContactInfo(userId);
                if (contactInfo) {
                    userName = contactInfo.displayName;
                } else {
                    userName = '' + userId;
                }
            }
            return userName;
        },

        OnInviteUsersToCall: function (errorCode, meetingId, chatType, userId) {
            if (errorCode == uc.ErrorCode.Success) {
                uc.IUIService.triggerCrossWindowEvent(uc.constants.UI.CrossWindowEventName.chooseContactsCreateCall, {callStatus: uc.constants.UI.chooseCallContacts.createCall_Ok});
                if (meetingId == this.inProgressCallInfo.callMeetingId) {
                    uc.IUIService.triggerCrossWindowEvent(uc.constants.UI.CrossWindowEventName.CloseGroupProfile,{});
                    if (userId == this.inProgressCallInfo.chatId) {
                     
                        var groupInfo = uc.IGroupService.getGroupInfo(this.inProgressCallInfo.chatId);
                        for (var i = 0; i < groupInfo.members.length; i++) {
                            if (!this.inProgressCallInfo.userList[groupInfo.members[i].userId]) {
                                this.inProgressCallInfo.userList[groupInfo.members[i].userId] = {};
                                this.inProgressCallInfo.userList[groupInfo.members[i].userId].callStatus = uc.constants.Call.CallStatus.INVITING;
                            }
                        }
                        if (this.callWindowType == uc.constants.Call.CallWindowType.EMBEDDED_CALL_WINDOW) {
                            //uc.modules.call.EmbeddedCallWindow.addUserCall(this.inProgressCallInfo);
                            this.updateInProgressCallInfo();
                        }
                    } else {
                        this.startNewCall(userId, chatType);
                    }
                }
            }else{

                uc.IUIService.triggerCrossWindowEvent(uc.constants.UI.CrossWindowEventName.chooseContactsCreateCall, {callStatus: uc.constants.UI.chooseCallContacts.createcall_fail,failedType:uc.constants.Call.CallFailedType.InvitedCall});
            }
        },
        userStatus:function(callStatus){
            var userBusy = false,busyUserId=[];
            for (var i = 0; i < callStatus.length; i++) {
                var user = callStatus[i], busyUserList = [];
                if (user.status == 4 || user.status == 6) {
                    var displayName='';
                    for(var j=0;j<this.changeCallUser.length;j++){
                        if(user.userId==this.changeCallUser[j].userId){
                            displayName=this.changeCallUser[j].displayName;
                        }
                    }
                    userBusy = true;
                    busyUserId.push(user.userId);
                    var myUserInfo = uc.IContactService.getCurrentUserInfo();
                    if (user.userId == myUserInfo.userId) {
                        busyUserList.push(uc_resource.SingleChat.Me);
                    } else {
                        if(displayName){
                            busyUserList.push(displayName);
                        }else{
                            var contactInfo = uc.IContactService.getBasicContactInfo(user.userId);
                            if (contactInfo) {
                                busyUserList.push(contactInfo.displayName);
                            } else {
                                busyUserList.push('<span class="callBusy" data-userId="' + user.userId + '"></span>');
                            }
                        }

                    }
                }
            }
            return {userBusy:userBusy,busyUserList:busyUserList,busyUserId:busyUserId};
        },
        userCallStatusBusy:function(userBusy){
            var message = uc.util.StringUtil.formatString(uc_resource.Call.UserBusyDeleted, [userBusy.busyUserList.join(",")]);
            uc.IUIService.triggerCrossWindowEvent(uc.constants.UI.CrossWindowEventName.chooseContactsCreateCall, {callStatus: uc.constants.UI.chooseCallContacts.createcall_fail,failedType:uc.constants.Call.CallFailedType.BusyUserStatus,failedData:userBusy,message:message});
        },
        OnUserCallStatusReceived: function (errorCode, chatId, chatType, callStatus) {
	        //如果当前没有在呼中，并且当前在呼中但是confid为0（表明会议没有创建完成）
	        if(!uc.modules.call.CallManager.inProgressCallInfo.isInProgress || uc.modules.call.CallManager.inProgressCallInfo.isInProgress && uc.modules.call.CallManager.inProgressCallInfo.callMeetingId == 0){
		        return;
	        }
            if (errorCode == uc.ErrorCode.Success) {
                var _this = this;
                var groupName = '', userList = [] ,groupData;
                if (chatId == this.inProgressCallInfo.chatId && chatId) {
                    var userBusy=this.userStatus(callStatus);
                    if (userBusy.userBusy) {
                        this.userCallStatusBusy(userBusy);

                        if(this.inProgressCallInfo.callStatus == uc.constants.Call.CallStatus.DISCONNECTED){
                            _this.inProgressCallInfo.reset();
                        }
                        return;
                    }
                    var isGroup =  chatType != uc.constants.Chat.ChatType.SingleChat;
                    groupData = this.createGroupData(this.changeCallUser,isGroup,chatId);
                    groupName = groupData.groupName;
                    userList = groupData.userList;

                    uc.util.LogUtil.info("OnUserCallStatusReceived","inviteUsersToCall","data=",
                        {callMeetingId: _this.inProgressCallInfo.callMeetingId, userList: userList});
                    if (this.inProgressCallInfo.chatType == uc.constants.Chat.ChatType.SingleChat) {
                        uc.ICallService.inviteUsersToSingleCall(uc.constants.Chat.ChatType.GroupChat,
                            groupName, userList, this.inProgressCallInfo.callMeetingId, function (arg) {
                                if (arg[0] == 0) {
                                    for (var i = 0; i < _this.changeCallUser.length; i++) {
                                        if (!_this.inProgressCallInfo.userList[_this.changeCallUser[i].userId]) {
                                            _this.inProgressCallInfo.userList[_this.changeCallUser[i].userId] = {};
                                            _this.inProgressCallInfo.userList[_this.changeCallUser[i].userId].callStatus = uc.constants.Call.CallStatus.INVITING;
                                        }
                                        _this.inProgressCallInfo.userList[_this.changeCallUser[i].userId].userInfo = _this.changeCallUser[i];
                                    }
                                }
                            });
                    } else {
                        uc.ICallService.inviteUsersToGroupCall(_this.inProgressCallInfo.chatType,
                            userList, _this.inProgressCallInfo.chatId, _this.inProgressCallInfo.callMeetingId, function (arg) {
                                if (arg[0] == 0) {
                                    if(_this.inProgressCallInfo.callStatus == uc.constants.Call.CallStatus.DISCONNECTED){
                                        _this.inProgressCallInfo.reset();
                                        return;
                                    }
                                    for (var i = 0; i < _this.changeCallUser.length; i++) {
                                        if (!_this.inProgressCallInfo.userList[_this.changeCallUser[i].userId]) {
                                            _this.inProgressCallInfo.userList[_this.changeCallUser[i].userId] = {};
                                            _this.inProgressCallInfo.userList[_this.changeCallUser[i].userId].callStatus = uc.constants.Call.CallStatus.INVITING;
                                        }
                                        _this.inProgressCallInfo.userList[_this.changeCallUser[i].userId].userInfo = _this.changeCallUser[i];

                                    }
                                }
                            });
                    }
                }else{

                    if (chatId != this.inProgressCallInfo.chatId && uc.modules.call.CallManager.inProgressCallInfo.isInProgress){
                        userBusy=this.userStatus(callStatus);
                        if (userBusy) {
                            this.userCallStatusBusy(userBusy);
                            _this.changeCallUser=[];
                            _this.invitedGroupMemberToCall = null;
                            return;
                        }
                        groupData = this.createGroupData(this.changeCallUser);
                        userList = groupData.userList;
                        if(!this.invitedGroupMemberToCall){
                            return;
                        }
                        uc.ICallService.inviteUsersToGroupCall(this.invitedGroupMemberToCall.chatType,
                            userList, _this.invitedGroupMemberToCall.chatId, _this.invitedGroupMemberToCall.callMeetingId, function (arg) {
                                _this.changeCallUser=[];
                                _this.invitedGroupMemberToCall = null;
                            });
                    }
                }
            }else{
                uc.IUIService.triggerCrossWindowEvent(uc.constants.UI.CrossWindowEventName.chooseContactsCreateCall, {callStatus: uc.constants.UI.chooseCallContacts.createcall_fail,failedType:uc.constants.Call.CallFailedType.GetUserStatus});
            }
        },
        createGroupData: function (userData,isGroup,chatId) {
            var userList = [], count = 0;
            var currentUserInfo = uc.IContactService.getCurrentUserInfo();
            var groupName = currentUserInfo.displayName;

            if(!isGroup){
                var hasMe=false;
                for(var j=0;j<userData.length;j++){
                    if(userData[j].userId == currentUserInfo.userId){
                        hasMe=true;
                        break;
                    }
                }
                if(!hasMe){
                    userList.push({uid:{userId: parseInt(currentUserInfo.userId),siteId:currentUserInfo.siteId,resId:100}, userName: currentUserInfo.displayName});
                }
                var contactInfo = uc.IContactService.getContactInfo(this.inProgressCallInfo.chatId);
                if(contactInfo){
                    userList.push({uid:{userId: this.inProgressCallInfo.chatId,siteId:this.inProgressCallInfo.siteId,resId:100}, userName:contactInfo.displayName});
                }
            }
            for (var i = 0; i < userData.length; i++) {
                if(isGroup && userData[i].userId == currentUserInfo.userId){
                    if(this.inProgressCallInfo.chatId == chatId){
                        uc.modules.call.CallManager.startCall(this.inProgressCallInfo,uc.constants.Call.CallWindowType.EMBEDDED_CALL_WINDOW,false,true);
                    }
                    continue;
                }
                var user = userData[i];
                userList.push({uid:{userId: parseInt(user.userId),siteId:user.siteId,resId:100}, userName: user.displayName});
                if (count < 3) {
                    if(userData[i].userId == currentUserInfo.userId){
                        continue;
                    }
                    groupName += "、" + user.displayName;
                    count++;
                }
            }
            return {userList: userList, groupName: groupName}
        },
        OnGetHistoryPhoneNumReceived: function (errorCode, phoneNumbers) {
            uc.modules.call.EmbeddedCallWindow.OnGetHistoryPhoneNumReceived(errorCode, phoneNumbers);
        },
        OnGroupMemberAddedMessageReceived: function(conversationType, channel, from, to, operatorId, groupId,members){
            if (channel != uc.constants.Message.MessageChannel.UCAS) {
                return;
            }
            if(this.inProgressCallInfo.chatType == conversationType && this.inProgressCallInfo.chatId == groupId){
                for (var i = 0; i < members.length; i++) {
                    if (!this.inProgressCallInfo.userList[members[i].userId]) {
                        this.inProgressCallInfo.userList[members[i].userId] = {};
                        this.inProgressCallInfo.userList[members[i].userId].callStatus = uc.constants.Call.CallStatus.INVITING;
                    }
                    this.inProgressCallInfo.userList[members[i].userId].userInfo ={};

                }
                this.updateInProgressCallInfo();
            }
        },
        OnGroupCallDurationReceived: function(channel,conversationType, groupId, duration, msgId, msgSeq, timestamp){
            if (channel != uc.constants.Message.MessageChannel.UCAS) {
                return;
            }
	        uc.IUIService.triggerCrossWindowEvent(uc.constants.UI.CrossWindowEventName.closeScreenShareWindow, {});
            if(this.inProgressCallInfo.chatId == groupId && this.inProgressCallInfo.chatType==conversationType){
                if(this.inProgressCallInfo.callStatus != uc.constants.Call.CallStatus.CONNECTED){
                    this.setCallInfoToProgressCallInfo(this.inProgressCallInfo.callMeetingId);
                    this.endInProgressCall();
	                uc.util.AudioUtil.stopCallAllSound();
                }
            }else{
                if (this.selectedConversationInfo.conversationId == groupId && this.selectedConversationInfo.conversationType == conversationType){
                    uc.modules.call.EmbeddedCallBanner.close();
                }
            }
            this._removeCallInfo(groupId, conversationType);
        },
        OnCallDisconnected: function(errorCode,meetingId){
            if(this.inProgressCallInfo.callMeetingId == meetingId){
                clearTimeout(this.setReconnected);
                var _this=this;
                this.setReconnected=setTimeout(function(){
                    uc.ICallService.reconnectConference(function(arg){
                        uc.util.LogUtil.info('CallManager', 'OnCallDisconnected', 'data=', {arg:arg[0]});

                    });
                    clearTimeout(_this.setReconnected);
                },3000);
            }else{
                if(this.inProgressCallInfo.isInProgress){
                    uc.modules.component.PromptMessageBox.open({
                        title: uc_resource.WindowTitle.Notification,
                        message:uc_resource.Call.CreateCallFailed,
                        buttonMode: "ok",
                        OkText: uc_resource.Common.OK
                    }, function () {
                        this.close();
                    });
                    this._removeCallInfo(this.inProgressCallInfo.chatId, this.inProgressCallInfo.chatType);
                    this.endInProgressCall();
                }
            }
        },
        OnCallReconnected: function(errorCode,meetingId){
            uc.util.LogUtil.info('CallManager', 'OnCallReconnected', 'data=', {errorCode:errorCode,meetingId:meetingId});
            if (errorCode == uc.ErrorCode.Success){
                clearTimeout(this.setReconnected);
            }
        },
        OnInitClient: function(clearData){

            var callData = uc.util.StorageUtil.getData("CallManager-callInfo:inProgressCallInfo");
            if(callData && callData.isInProgress){
                this.inProgressCallInfo.update(callData);
                if(this.inProgressCallInfo.chatType != uc.constants.Chat.ChatType.SingleChat && this.inProgressCallInfo.callStatus == uc.constants.Call.CallStatus.CONNECTED){
                    this.isLeave=true;
                    this.leaveCall(this.inProgressCallInfo);
                   // this.startCall(this.inProgressCallInfo, uc.constants.Call.CallWindowType.FLOATING_CALL_WINDOW, false, true);
                }else{
                    this.startCall(this.inProgressCallInfo, uc.constants.Call.CallWindowType.FLOATING_CALL_WINDOW, false, false);
                }
            }


        },
        OnMinimizeWindow:function(){
            if (this.inProgressCallInfo.isInProgress) {
                switch (this.callWindowType) {
                    case uc.constants.Call.CallWindowType.EMBEDDED_CALL_WINDOW:
                        this.switchEmbeddedCallWindowToFloating();
                        break;
                    case uc.constants.Call.CallWindowType.EMBEDDED_CALL_BANNER:
                        this.switchEmbeddedCallBannerToFloating();
                        break;

                }
            }
        },
        changeCallWindowFormMessage:function(eventData){
            if (this.inProgressCallInfo.chatId == eventData.Id && this.inProgressCallInfo.chatType == eventData.chatType) {
                if(this.callWindowType == uc.constants.Call.CallWindowType.FLOATING_CALL_WINDOW){
                    return;
                }
                switch (this.callWindowType) {
                    case uc.constants.Call.CallWindowType.EMBEDDED_CALL_WINDOW:
                        this.switchEmbeddedCallWindowToFloating();
                        break;
                    case uc.constants.Call.CallWindowType.EMBEDDED_CALL_BANNER:
                        this.switchEmbeddedCallBannerToFloating();
                        break;
                    default:
                        break;
                }

            }
        },
	    //主动或被动退出群聊，如果当前在呼中调用
	    OnGroupExitedNeedLeaveGroupCall: function (errorCode, groupId, userId){
		    if(errorCode != uc.ErrorCode.Success){
			    uc.util.LogUtil.info('callManager', 'OnOnGroupExitedNeedLeaveGroupCall', 'Exception: ' + errorCode, null);
			    return;
		    }
		    return;
		    var InProgressCallInfo = uc.modules.call.CallManager.inProgressCallInfo;
		    if(InProgressCallInfo.isInProgress){
			    var groupInfo = uc.IGroupService.getGroupInfo(InProgressCallInfo.chatId);
			    if(groupInfo.id == groupId){
				   //退出群聊，同时也被退出群呼
				    if(InProgressCallInfo.callStatus == uc.constants.Call.CallStatus.INVITING){
					    this.cancelCall();
				    }else if(InProgressCallInfo.callStatus == uc.constants.Call.CallStatus.CONNECTING
					    || InProgressCallInfo.callStatus == uc.constants.Call.CallStatus.CONNECTED){
					    this.leaveCall();
				    }else if(InProgressCallInfo.callStatus==uc.constants.Call.CallStatus.INVITED){
					    this.rejectCallInvitation(InProgressCallInfo,'');
					    this.closeWindow(true);
				    }
			    }
		    }
	    },
	    //在退出登录的或者退出蜜蜂时，如果当前在呼中调用
	    exitCall:function(){
		    var InProgressCallInfo = uc.modules.call.CallManager.inProgressCallInfo;
		    if(InProgressCallInfo.isInProgress){
			    if(InProgressCallInfo.callStatus == uc.constants.Call.CallStatus.INVITING){
				    this.cancelCall();
			    }else if(InProgressCallInfo.callStatus == uc.constants.Call.CallStatus.CONNECTING
				    || InProgressCallInfo.callStatus == uc.constants.Call.CallStatus.CONNECTED){
				    this.leaveCall();
			    }else if(InProgressCallInfo.callStatus==uc.constants.Call.CallStatus.INVITED){
				    this.rejectCallInvitation(InProgressCallInfo,'');
			    }
		    }
		    this.closeWindow(true);
	    },
	    OnAudioServiceReady: function(audioReadyStatus,audioDeviceStatusChanged){
		    uc.util.LogUtil.info("callmanager", "OnAudioServiceReady", "audioReadyStatus", audioReadyStatus);
		    uc.util.LogUtil.info("callmanager", "OnAudioServiceReady", "audioDeviceStatusChanged", audioDeviceStatusChanged == undefined?'undefined':audioDeviceStatusChanged);
		    //已经准备好
		     if(audioReadyStatus == 1){
			     var flag;
			     //如果是状态变更，不用取数量
			     if(audioDeviceStatusChanged != undefined ){
				     flag = audioDeviceStatusChanged == 1 ? true:false;
			     }else
			     {
				     //获取扬声器和麦克风数量
				     var speakerNum = uc.ICallService.getSpeakerNum();
				     var phoneNum = uc.ICallService.getMicrophoneNum();
				     uc.util.LogUtil.info("callmanager", "OnAudioServiceReady", "speakerNum", speakerNum);
				     uc.util.LogUtil.info("callmanager", "OnAudioServiceReady", "phoneNum", phoneNum);
				     flag = speakerNum <= 0 || phoneNum<= 0;
			     }

			     if(flag){
				     // 弹出检测的提示
				     switch (this.callWindowType) {
					     case uc.constants.Call.CallWindowType.EMBEDDED_CALL_WINDOW:
						     uc.modules.call.EmbeddedCallWindow.IsShowCallNeedAVDetectionTip(true);
						     break;
					     case uc.constants.Call.CallWindowType.FLOATING_CALL_WINDOW:

						     break;
					     default:
						     break;
				     }
			     }
		     }
	    },
	    OnSharingError:function(statusCode){
		    uc.util.LogUtil.warn('callmanager', 'OnSharingError', "statusCode:",statusCode);
	    },
	    onOpenScreenSharingWindow:function(conferenceId, userId){
		    uc.util.LogUtil.info('callmanager', 'onOpenScreenSharingWindow', "inProgressCallInfo:",this.inProgressCallInfo);
		    if(this.inProgressCallInfo.callMeetingId != conferenceId){
			    uc.util.LogUtil.info('callmanager', 'onOpenScreenSharingWindow', "conferenceId is differance:",conferenceId);
			    return;
		    }
		    //共享者,打开顶部工具栏
		    if(this.inProgressCallInfo.desktopShareStatus == uc.constants.Call.DesktopShareStatus.OPEN){
			    //uc.util.StorageUtil.setData('desktopShareData',this.inProgressCallInfo);
			    var options = {
				    title: uc_resource.Common.AppName,
				    top: 0,
				    left: (screen.width-415)>>1,
				    width: 415,
				    height: 140,
				    refreshIfExist: false,
				    centerScreen: false,
				    modal: false,
				    resizable: false,
				    isTop: true,
				    hasTaskBarIcon: false,
				    behaviorIfSCClose: uc.constants.UI.WindowCloseBehavior.Minimize,
				    enableDrag: false,
				    isChild: false
			    };
			    uc.util.WindowUtil.open('page/screenSharing.html', 'screenSharing', options);
		    }else{
			    //共享接受者如果正在进行接受共享
			    if(this.inProgressCallInfo.desktopShareAcceptStatus == uc.constants.Call.DesktopShareStatus.OPEN) {
				    //当前正在接受共享
				    return;
			    }
			    //uc.util.StorageUtil.setData('desktopShareAcceptData',this.inProgressCallInfo);
			    //呼的共享按钮置灰，不可点击
			    //被共享者,startDesktopView
			    this.inProgressCallInfo.desktopShareAcceptStatus = uc.constants.Call.DesktopShareStatus.OPEN;
			    var contactInfo = uc.modules.cache.BasicContactInfoCache.getBasicContactInfo(userId);
			    var shareTitle = uc.util.StringUtil.formatString(uc_resource.Call.destopShareTitle, [contactInfo.displayName]);
			    uc.ISharingService.startDesktopView(shareTitle,function(arg){

			    });
		    }
		    if(uc.modules.call.EmbeddedCallWindow.isActivity){
			    uc.modules.call.EmbeddedCallWindow.updateToobarScreenBtnStatus(this.inProgressCallInfo);
		    }
	    },
	    OnDesktopShareStoped:function(conferenceId, userId){
		    uc.util.LogUtil.info('callmanager', 'OnDesktopShareStoped', "inProgressCallInfo:",this.inProgressCallInfo);
		    if(this.inProgressCallInfo.callMeetingId != conferenceId){
			    uc.util.LogUtil.info('callmanager', 'OnDesktopShareStoped', "conferenceId is differance:",conferenceId);
			    return;
		    }
		    //共享者，切换到主窗口呼界面，共享恢复缺省模式
		    if(this.inProgressCallInfo.desktopShareStatus == uc.constants.Call.DesktopShareStatus.OPEN){
			    this.inProgressCallInfo.desktopShareStatus = uc.constants.Call.DesktopShareStatus.CLOSE;
		    }else{
			    //发送stopDesktopView消息
			    if(this.inProgressCallInfo.desktopShareAcceptStatus == uc.constants.Call.DesktopShareStatus.OPEN) {
				    this.inProgressCallInfo.desktopShareAcceptStatus = uc.constants.Call.DesktopShareStatus.CLOSE;
				    //停止观看屏幕共享
				    uc.ISharingService.stopDesktopView();
			    }
		    }
		    if(uc.modules.call.EmbeddedCallWindow.isActivity){
			    uc.modules.call.EmbeddedCallWindow.updateToobarScreenBtnStatus(this.inProgressCallInfo);
		    }
	    },

	    OnDesktopViewerShareStarted:function(conferenceId){

	    },
	    OnDesktopViewerShareStoped:function(conferenceId){

	    },
	    updateDesktopCallInfo:function(callInfo){
		   if(callInfo != null){
			   this.inProgressCallInfo.desktopShareAcceptStatus =  callInfo && callInfo.desktopShareAcceptStatus!=undefined && callInfo.desktopShareAcceptStatus!=null?callInfo.desktopShareAcceptStatus:this.inProgressCallInfo.desktopShareAcceptStatus;
			   this.inProgressCallInfo.desktopShareStatus = callInfo && callInfo.desktopShareStatus!=undefined && callInfo.desktopShareStatus!=null?callInfo.desktopShareStatus:this.inProgressCallInfo.desktopShareStatus;
		   }
	    }
    }
})(uc);