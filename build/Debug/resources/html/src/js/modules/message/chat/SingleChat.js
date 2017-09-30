/**
 * Created by senlin.ouyang on 2015/7/21.
 */
(function(uc){
    uc.modules.chat.SingleChat = {
        init: function(userId, siteId, capability,paramObj){
            var isHide = capability ? (parseInt(capability.remote_assistant) ? '' : 'hide') : 'hide';
            this.nodeHtml =    '<div class="line" />'
            +                   '<div class="title">'
            +                       '<div class="name" id="chatName"></div>'
            +                       '<div class="sign" id="chatSign" style="display:none"></div>'
            +                   '</div>'
            +                   '<div class="content">'
            +                       '<div class="chat-title-btn">'
            +                           '<div class="chat-user-status"><div></div><span></span></div>'
            +                           '<div class="btn more" qtips="'+uc_resource.SingleChat.PersonalInformation+'" style="display: none"></div>'
            +                           '<div class="btn remote-assistance icon-remote-assistance '+ isHide +'" qtips="'+ uc_resource.Menu.RemoteAssistance +'"></div>'
	        +			                '<div class="chat-call-panel"><div class="call_btn_Delta"></div>'
	        +			                '<div class="btn call SingleChatCallBtn icon-chat_call_default" qtips="'+uc_resource.Menu.Call+'"></div></div>'
            +                           '<div class="btn calendar icon-createMetting" qtips="'+uc_resource.Menu.CreateMeeting+'"></div>'
            +                           '<div class="btn add icon-addcontacts" qtips="'+uc_resource.SingleChat.GroupChat+'"></div>'
            +                           '<div class="btn chat-external icon-external-chat hide" qtips="'+uc_resource.SingleChat.ExternalChat+'"></div>'
            +                       '</div>'
            +                   '</div>';
            this.headerNode = $('<div class="chat-header"></div>');
            this.headerNode.append(this.nodeHtml);

            this.headerNode.appendTo(uc.ui.Message.ContentPanelBody);
            this.paramObj = paramObj;
            this.chatName = $('#chatName', this.headerNode);
            this.chatSign = $('#chatSign', this.headerNode);
            this.remoteAssistanceNode = $('.remote-assistance', this.headerNode);
            this.userId = userId;
            this.seqs = {};
            this.chatType = uc.constants.Chat.ChatType.SingleChat;
            this.myUserInfo = uc.modules.chat.ChatManager.myUserInfo;
            this.userTelPhone = null;
            this.isGetcontactFullInfo = false;

            if (uc.constants.Switch.OpenUserStatus) {
                uc.IContactService.subscribeUserStatus([userId]);
            }
            var contactFullInfo = uc.IContactService.getContactInfo(userId);
            if(contactFullInfo && contactFullInfo.mobile){
                this.userTelPhone = contactFullInfo.mobile;
            }else{
                this.isGetcontactFullInfo = true;
                uc.IContactService.getContactInfo(userId);
            }
            var contactInfo = uc.modules.cache.BasicContactInfoCache.getBasicContactInfo(userId);
            siteId = siteId ? siteId : 1;
            if (contactInfo != null && contactInfo.displayName != null) {
                this.updateChatName(contactInfo.displayName, contactInfo.personalSign);
                siteId = contactInfo.siteId;
            } else {
                this.updateChatName(userId, null);
                if(!this.isGetcontactFullInfo){
                    uc.IContactService.getContactInfo(userId);
                }
            }
            this.node = $('<div class="single-chat message-box-container"></div>').appendTo(uc.ui.Message.ContentPanelBody);
            this.msgBox = uc.modules.chat.ChatManager.msgBox;
            this.msgBox.init(this.node, this, uc.constants.Chat.ChatType.SingleChat,userId);
            this.historyDomStr = "";
            this.lastMsgSeq = 0;
            this.firstGetHistory = true;
            this.externalBox = '';
            this.getMoreHistoryMsg();
            $('<div class="middle-line"></div>').appendTo(uc.ui.Message.ContentPanelBody);

            this.inputNode = $("<div class='chat-input'/>").appendTo(uc.ui.Message.ContentPanelBody);
            this.msgInputBox = uc.modules.chat.ChatManager.msgInputBox;
            this.msgInputBox.init(this.inputNode, {
            	chatType: uc.constants.Chat.ChatType.SingleChat,
            	chatObjectId: userId,
                siteId: siteId,
                resId: 100,
                moduleId: uc.constants.UI.ModuleId.Message
            });
            this.msgInputBox.enable(true);
            //this.msgInputBox.disableAction("history");
	        this.msgInputBox.disableAction("uploadMeetingDoc");
	        this.msgInputBox.disableAction("code");
            this.msgInputBox.showChatPrompt(true);
            this.msgBox.bindResize(uc.ui.Message.ContentPanelBody, this.msgInputBox);
            this.currentUserStatus;
            this.bindEvents();
			var userInfo = contactInfo;
			
            if (userInfo && uc.IIMService.isShowExternalMessage(userInfo.account) == 1) {
                this.headerNode.find(".chat-external").removeClass("hide");

                this.externalChatNode = $('<div class="external-chat external-box-container"></div>').appendTo(uc.ui.Message.ContentPanelBody);
                
                this.externalBox = new uc.modules.chat.RtxBox({"chatType": this.chatType, "chatId": userInfo.account});
                this.externalBox.init(this.externalChatNode);
            }

            if(uc.modules.cloud.CloudManager.checkCloudIsLoginSuccess()==0 ||
                uc.modules.cloud.CloudManager.checkCloudTokenIsSuccess()!=0){
                this.msgInputBox.disableAction("cloud");
	            this.msgInputBox.disableAction("uploadMeetingDoc");
            };

            //add by xiege.
            if (uc.constants.Switch.OpenMultiVersion) {
	            if(!this.getCalendarVisible()){
		            this.headerNode.find(".calendar").addClass("hide");
	            }
	            //判断呼或者pbx的权限，更改通话按钮显示状态
	            if(this.getCallVisible()) {
		            this.headerNode.find(".call").removeClass("hide");
		            this.headerNode.find(".call_btn_Delta").removeClass("hide");
	            }else{
		            if(!this.getPBXVisible()){
			            this.headerNode.find(".call").addClass("hide");
			            this.headerNode.find(".call_btn_Delta").addClass("hide");
		            }
	            }
            }
        },
        bindEvents: function() {
            var This = this;
            this.headerNode.on('click','.add',function() {
                $(this).siblings('div').removeClass('selected')
                $(this).siblings('.chat-external').removeClass('icon-external-chat-checked').addClass('icon-external-chat');
                uc.IUIService.startChat([This.myUserInfo.userId, This.userId]);
            });
            this.headerNode.on('click','.calendar',function() {
                $(this).siblings('div').removeClass('selected')
                $(this).siblings('.chat-external').removeClass('icon-external-chat-checked').addClass('icon-external-chat');
                var users= [];
                users.push(This.myUserInfo.userId);
                users.push(This.userId);

                var param = {
                    shareUserId: uc.constants.Meeting.AuthorizeMySelf,
                    isCreate: true,
                    users: users
                };

                uc.IUIService.createMeeting("SingleChatCreateMeeting", param);
            });
            this.headerNode.on('click','.name',function() {
                uc.IUIService.showPopupContactProfile(Number(This.userId));
            });

            this.headerNode.on('click','.chat-external',function() {
                if ($('.chat-external', This.headerNode).hasClass("selected")) {
                    $('.chat-external', This.headerNode).removeClass("selected");
                    $('.chat-external', This.headerNode).removeClass("icon-external-chat-checked");
                    $('.chat-external', This.headerNode).addClass("icon-external-chat");
                    $('.externalBox').hide();
                    //This.externalChatNode.empty();
                    This.externalBox.node.parent().hide();
                } else {
                    $('.chat-external', This.headerNode).addClass("selected");
                    $('.chat-external', This.headerNode).removeClass("icon-external-chat");
                    $('.chat-external', This.headerNode).addClass("icon-external-chat-checked");
                    $('.externalBox').css({left:"860px", opacity:"0.3", "background-color":"#dddddd"});
                    $('.externalBox', uc.ui.Message.ContentPanelBody).show();
                    This.externalBox.node.parent().show();
                }
            });

            this.remoteAssistanceNode.click(function(){
                This.startRemoteAssistance();
            });
            
            this.headerNode.on('click','.chat-call-panel',function(e) {
                var phoneNum;
                if(This.getPBXVisible()){
                    if(!This.userTelPhone){
                        var contactInfo = uc.IContactService.getContactInfo( This.userId);
                        if(contactInfo){
                            This.userTelPhone = contactInfo.mobile;
                            This.userName = contactInfo.displayName;
                        }
                    }
                    phoneNum=uc.util.StringUtil.IsPBXTelNumber(This.userTelPhone);
                }
	            uc.util.MenuUtil.showStartCallContextMenu(e,phoneNum,This.getCallVisible(),function(type,data){
		            if(type == 0){
			            This.startCall();
		            }else{
                        uc.IUIEvent.OnPbxCallFromContact({"userName":This.userName,"userID":This.userId,"Tel":data});
		            }
	            });


            }).on('click','.call_btn_Delta',function(){
	            This.headerNode.find(".call").trigger("click");
            });
        },

        startRemoteAssistance: function(){
            if(this.starting){
                return;
            }
            var _this = this;
            this.starting = true;
            setTimeout(function(){
                _this.starting = false;
            }, 1000);
            var isInRemoteAssistance = uc.modules.remoteAssistance.RemoteAssistanceManager.isInRemoteAssistance();
            if(isInRemoteAssistance){
                uc.modules.component.PromptMessageBox.open({title: uc_resource.WindowTitle.Notification,
                            message: uc_resource.Message.InRemoteAssistanceForStart ,
                            OkText: uc_resource.Common.OK,
                            CancelText: uc_resource.Common.Cancel,
                            buttonMode: "both"}, function (result){
                    if(result == 0){
                        uc.modules.remoteAssistance.RemoteAssistanceManager.startRemoteAssistance(_this.userId);
                        this.close();
                    }else {
                        this.close();
                    }
                    this.close();
                });
            }else {
                uc.modules.remoteAssistance.RemoteAssistanceManager.startRemoteAssistance(this.userId);
            }
        },
        startCall:function(){
	       if(!uc.modules.pbx.PbxManager.DialIsClick || uc.modules.call.CallManager.inProgressCallInfo.isInProgress){
                this.showCallIngPrompt();
                return;
            }
	        var copyBack = '{"type":"'+uc.constants.Chat.ChatType.SingleChat+'","userId":"'+this.userId+'"}';
	        var userIds = [this.myUserInfo.userId,this.userId];
	        uc.IContactService.getUserStatus(userIds,copyBack,function(arg){
		        var errorCode = arg[0];
		        if(errorCode != uc.ErrorCode.Success){
			        this.showStartCallFailed();
		        }
	        });
	        uc.util.LogUtil.info("SingleChat","ClickCallBtn","Start to get user call status.", copyBack);
        },
        showStartCallFailed: function(){
            uc.modules.component.PromptMessageBox.open({
                title: uc_resource.WindowTitle.Notification,
                message: uc_resource.Call.StartCallFailed,
                buttonMode: "ok",
                OkText: uc_resource.Common.OK
            }, function () {
                this.close();
            });
        },
        showCallIngPrompt:function(){
            uc.modules.component.PromptMessageBox.open({
                title: uc_resource.WindowTitle.Notification,
                message: uc_resource.Call.HasCallNotCall,
                buttonMode: "ok",
                OkText: uc_resource.Common.OK
            }, function () {
                this.close();
            });
            return false;
        },
        _disableCall: function(){
            this.headerNode.find('.call').addClass('chat_call_start');
	        this.headerNode.find('.call_btn_Delta').addClass('chat_call_start');
        },
        _enableCall: function(){
            this.headerNode.find('.call').removeClass('chat_call_start');
	        this.headerNode.find('.call_btn_Delta').removeClass('chat_call_start');
        },
        OnCallStarted: function(){
            this._disableCall();
        },
        OnCallEnded: function(){
            this._enableCall();
        },
        updateChatMobile:function(mobile){
            this.userTelPhone = mobile;
        },
        updateChatName: function(name, sign) {
            name= uc.util.StringUtil.escapeHtml(name);
            sign= uc.util.StringUtil.escapeHtml(sign);
            uc.util.LogUtil.info("SingleChat","updateChatName","chatName = "+ name + ", sign = " +sign,null);
            this.chatName.html(name).attr({"qtips":name,"qtips-show-by":"mouse"});
            if (sign != null && sign != "") {
                this.chatSign.html(sign).attr({"qtips":sign,"qtips-show-by":"mouse"}).show();
                this.chatName.removeClass('center');
            } else {
                this.chatName.addClass('center');
            }
            this.userName =name;
        },
        getMoreHistoryMsg: function() {
            this.historyDomStr = "";
            var msgBox = this.msgBox;
            msgBox.onMoreMsgLoading();
            uc.util.LogUtil.info("SingleChat", "getMoreHistoryMsg","data=",{groupId:this.userId, count:10, page:0, seq:Number(this.lastMsgSeq)});
            uc.IIMService.getChatHistory(uc.constants.Message.MessageChannel.ChatHistory, this.userId, uc.constants.Message.OnceGetHistoryCount, 0, Number(this.lastMsgSeq), function(errorCode){
                if (errorCode != 0) {
                    uc.util.LogUtil.info("SingleChat","getMoreHistoryMsg", "data=",{groupId:this.groupId, errorCode:errorCode});
                    msgBox.onMoreMsgLoadEnd();
                }
            });
        },
        onChatHistoryReceived: function(errorCode, userId, messages, msgCount, minTimestamp) {
            if (this.firstGetHistory) {
                uc.modules.chat.ChatManager.appendUploadMessages();
            }
            if(this.paramObj && this.paramObj.filePath){
                this.msgInputBox.rightClickToSendFile(this.paramObj.filePath);
            }
            if (errorCode == 0) {
                this.lastMsgSeq = minTimestamp;
            }
            this.msgBox.onMoreMsgLoadEnd();
            if (msgCount < uc.constants.Message.OnceGetHistoryCount && errorCode == 0) {
                this.msgBox.onLoadAllMsgEnd();
            }
            if (this.historyDomStr == "") {
                this.firstGetHistory = false;
                return;
            }
            uc.util.LogUtil.info("SingleChat","onChatHistoryReceived","data=",{errorCode:errorCode,userId:userId});
            this.msgBox.prependMessage(this.historyDomStr);
            if (this.firstGetHistory) {
                this.msgBox.scrollToEnd();
            }
            this.firstGetHistory = false;
            this.historyDomStr = "";
        },
        procMessageDomStr: function(channel, domStr, msgId, msgSeq) {
            uc.util.LogUtil.info("SingleChat","procMessageDomStr","msgSeq=",{msgSeq:msgSeq,channel:channel});
            if (this.seqs[msgSeq] != null){
                uc.util.LogUtil.info("SingleChat","procMessageDomStr","the msg seq has been existed!=",{msgSeq:msgSeq});
                return;
            }
            this.seqs[msgSeq] = true;

            //this.lastMsgSeq = msgSeq;
            if (this.msgBox.isMsgIdExist(msgId)) {
                uc.util.LogUtil.info("SingleChat","procMessageDomStr","the msg seq has been existed!=",{msgId:msgId});
                return;
            }
            if (uc.constants.Message.MessageChannel.ChatHistory == channel) {
                this.historyDomStr = domStr + this.historyDomStr;
                return;
            }
            this.msgBox.appendMessage(domStr);
        },

        onTextMessageReceived: function(channel, sendState, self, from, msgId, msgSeq, textType, textData, sendTime, timestamp, textFormat){
            var domStr = this.msgBox.getTextMsgDomStr(sendState, self, from, msgId, msgSeq, textType, textData, sendTime, timestamp, textFormat);
            this.procMessageDomStr(channel, domStr, msgId, msgSeq);
        },
        onImageMessageReceived: function(channel, sendState, self, from, msgId, msgSeq, imageName, imageSize, imageURL, imageData, timestamp){
            var domStr = this.msgBox.getImgMsgDomStr(sendState, self, from, msgId, msgSeq, imageName, imageSize, imageURL, imageData, timestamp);
            this.procMessageDomStr(channel, domStr, msgId, msgSeq);
        },
        onAudioMessageReceived: function(channel, sendState, self, from, msgId, msgSeq, audioName, audioSize, audioURL, audioDuration, timestamp){
            var domStr = this.msgBox.getAudioMsgDomStr(sendState, self, from, msgId, msgSeq, audioName, audioSize, audioURL, audioDuration, timestamp);
            this.procMessageDomStr(channel, domStr, msgId, msgSeq);
        },
        onVideoMessageReceived: function(channel, sendState, self, from, msgId, msgSeq, videoName, videoSize, videoURL, videoThumb, duration, timestamp){

        },
        OnIMReportMessageReceived: function(channel, sendState, from, to, eventId, msgId, msgSeq, msgTime, reportTitle, reportUrl) {
            var domStr = this.msgBox.getMeetingReportMsgDomStr(eventId, to.siteId, msgId, msgSeq, msgTime, reportTitle, reportUrl);
            this.procMessageDomStr(channel, domStr, msgId, msgSeq);
        },
        onFileReceived: function(channel, sendState, self, from, msgId, msgSeq,fileName, fileSize, fileURL, timestamp, localPath){
            var domStr = this.msgBox.getFileMsgDomStr(sendState, self, from, msgId, msgSeq,fileName, fileSize, fileURL, timestamp, localPath,null, "",uc.constants.Chat.ChatType.SingleChat);
            this.procMessageDomStr(channel, domStr, msgId, msgSeq);
        },
        onFileShared: function(self, from, msgId, msgSeq, fileName, fileSize, fileURL, fileId){

        },
        onFileDeleted: function(self, from, msgId, msgSeq, fileName, fileId){

        },
        OnCodeReceived: function(channel, sendState, self, from, msgId, msgSeq, creatorId, creatorName, codeId,
                                 codeTitle, langType, codeHead, codeDesc, codeSize, codeUrl, codeLine, createTime, timestamp){
            var domStr = this.msgBox.getCodeMsgDomStr(sendState, self, from,
                msgId, msgSeq, codeTitle, codeSize, codeUrl, langType, codeHead, codeLine, codeDesc, createTime, null, creatorName);
            this.procMessageDomStr(channel, domStr, msgId, msgSeq);
        },
        onCloudFileReceived: function(channel, sendState, self, from, msgId, msgSeq,fileName, fileSize, timestamp,detailContent){
            var domStr = this.msgBox.getCloudFileMsgDomStr(sendState, self, from, msgId, msgSeq,fileName, fileSize,null, timestamp, null, "",detailContent,uc.constants.Chat.ChatType.SingleChat);
            this.procMessageDomStr(channel, domStr, msgId, msgSeq);
        },
        OnMeetingAccreditMessageReceived: function(channel, from, to, msgId, msgSeq, msgTime){
            var uid = from.userId == this.myUserInfo.userId ? to.userId : from.userId;
            var resource = from.userId == this.myUserInfo.userId ? uc_resource.Message.AccreditMeetingByMe : uc_resource.Message.AccreditMeetingToMe;
            var message = uc.util.StringUtil.formatString(resource, [this.getMessageUserName(uid)]);
            var domStr = this.msgBox.getSysMsgDomStr(message, msgSeq);
            this.procMessageDomStr(channel, domStr, msgId, msgSeq);
        },
        OnMeetingAccreditCancelledMessageReceived: function(channel, from, to, msgId, msgSeq, msgTime){
            var uid = from.userId == this.myUserInfo.userId ? to.userId : from.userId;
            var resource = from.userId == this.myUserInfo.userId ? uc_resource.Message.CancelAccreditMeetingByMe : uc_resource.Message.CancelAccreditMeetingToMe;
            var message = uc.util.StringUtil.formatString(resource, [this.getMessageUserName(uid)]);
            var domStr = this.msgBox.getSysMsgDomStr(message, msgSeq);
            this.procMessageDomStr(channel, domStr, msgId, msgSeq);
        },
        OnIMSummaryVideoMessageReceived: function(channel,self, from, to, operatorId, summaryId, summaryName, conferenceId, eventId, meetingName, meetingStartTime, updateTime,type,videoName,videoURL,videoThumb,thumbnailsURL,length,size,operateName,operateAvatar,msgId, msgSeq, msgTime){
            var uid = from.userId == this.myUserInfo.userId ? to.userId : from.userId;
            var resource = from.userId == this.myUserInfo.userId ? uc_resource.Message.CancelAccreditMeetingByMe : uc_resource.Message.CancelAccreditMeetingToMe;
            var domStr = this.msgBox.getSummaryAudioMsgDomStr(self,from, to, operatorId, summaryId, summaryName, conferenceId, eventId, meetingName, meetingStartTime, updateTime,type,videoName,videoURL,videoThumb,thumbnailsURL,length,size,operateName,operateAvatar,msgId, msgSeq, msgTime);
            this.procMessageDomStr(channel, domStr, msgId, msgSeq);
        },
        getMessageUserName: function(userId){
            var userName;
            if(userId == this.myUserInfo.userId){
                userName = uc_resource.SingleChat.Me;
            }else{
                var contactInfo = uc.IContactService.getBasicContactInfo(userId);
                if(contactInfo){
                    userName = contactInfo.displayName;
                }else{
                    userName = '<span rel="' + userId + '">' + userId + '</span>' ;
                }
            }
            return userName;
        },
        resendMessage: function (msgId){
            var This = this;
            console.log("resendMessage", msgId);
            uc.IIMService.resendMessage(msgId,
                function(args){
                    console.log(args);
                    if (args[0] != 0) {
                        This.msgBox.showSendFailFlag(msgId, true);
                    }
                });
        } ,
        OnIMUnreadUserCountEvent:function(userId,info){
            if(userId==this.userId){
                this.msgBox.setUnreadCountEvent(userId,info);
            }
        },
        // added by xue.bai_2 on 2016/04/07
        OnRevocationMessageReceived: function(channel, conversationId, conversationType, from, to, msgId, msgSeq, msgTime, seqId){
            var message = uc.util.StringUtil.formatString(uc_resource.Message.RevocationMessage, [this.getMessageUserName(from.userId)]);
            var domStr = this.msgBox.getSysMsgDomStr(message, msgSeq);
            this.procMessageDomStr(channel, domStr, msgId, msgSeq);
            this.msgBox.chatList.find("li[seq='" + seqId + "']").remove();
        },
        OnRemoteAssistanceEnded: function(meetingId, msgId, msgSeq, timestamp){
            var message = uc_resource.Message.RemoteAssistanceEnded;
            var domStr = this.msgBox.getSysMsgDomStr(message, timestamp);
            this.procMessageDomStr(uc.constants.Message.MessageChannel.UCAS, domStr, msgId, msgSeq);
            uc.modules.remoteAssistance.RemoteAssistanceBanner.removeRemoteAssistanceBanner();
        },
        OnP2PCallInvitationReceived: function(channel,userId,fromUserId, confId, confPwd, hostId, msgId, msgSeq, timestamp){
            var message = uc.util.StringUtil.formatString(uc_resource.Message.CallInvitationReceived, [this.getMessageUserName(fromUserId)]);
            var domStr = this.msgBox.getSysMsgDomStr(message, msgSeq);
            this.procMessageDomStr(channel, domStr, msgId, msgSeq);
        },
        OnP2PCallInvitationAccepted: function(channel, userId,fromUserId, confId, msgId, msgSeq, timestamp){

        },
        OnP2PCallInvitationRejected: function(channel,self, userId,from, confId, hostId, reason, msgId, msgSeq, timestamp,sendState){
            var message;
            if (!reason) {
                if (from.userId == this.myUserInfo.userId) {
                    message = uc.util.StringUtil.formatString(uc_resource.Message.SelfRejectedCall, [this.getMessageUserName(userId)]);
                } else {
                    message = uc.util.StringUtil.formatString(uc_resource.Message.RejectedCall, [this.getMessageUserName(from.userId)]);
                }
                var domStr = this.msgBox.getSysMsgDomStr(message, msgSeq);
            } else {
                message = uc.util.EmotionUtil.codeToDescription(reason);
                message = uc.modules.emoji.EmojiConverter.androidCodeToEmotionTag(message);
                var textFormat=uc.modules.setting.SettingManager.getFontSettingFormat();

                var domStr = this.msgBox.getTextMsgDomStr(sendState, self, from, msgId, msgSeq, '', message, timestamp, timestamp, textFormat);
            }

            this.procMessageDomStr(channel, domStr, msgId, msgSeq);
        },
        OnP2PCallStopped: function(channel, userId, confId, msgId, msgSeq, timestamp){},
        OnP2PCallCancelled: function(channel, userId,fromUserId, msgId, msgSeq, timestamp){
            var message;
            if (fromUserId == this.myUserInfo.userId) {
                message = uc.util.StringUtil.formatString(uc_resource.Message.SelfCancelCall, [this.getMessageUserName(fromUserId)]);
            } else {
                message = uc.util.StringUtil.formatString(uc_resource.Message.CancelCall, [this.getMessageUserName(fromUserId)]);
            }
            var domStr = this.msgBox.getSysMsgDomStr(message, msgSeq);
            this.procMessageDomStr(channel, domStr, msgId, msgSeq);
        },
        OnP2PCallNoResponse: function(channel, userId, msgId, msgSeq, timestamp){},
        OnP2PCallDurationReceived: function(channel, self,form, duration, msgId, msgSeq, timestamp,sendState){
            var message = uc.util.StringUtil.formatString(uc_resource.Message.CallDuration, [uc.util.TimeUtil.getCallTimeStr(duration)]);
            var textFormat=uc.modules.setting.SettingManager.getFontSettingFormat();
            var domStr = this.msgBox.getDurationMsgDomStr(sendState, self, form, msgId, msgSeq, '', message, timestamp, timestamp, textFormat);
            this.procMessageDomStr(channel, domStr, msgId, msgSeq);
        },
        clear: function(){
            this.nodeHtml = null;
            this.node = null;
            this.headerNode = null;
            this.chatName = null;
            this.chatSign = null;

            this.msgBox.clear();
            this.msgInputBox.clear();


            uc.ui.Message.ContentPanelBody.find('*').off();
            uc.ui.Message.ContentPanelBody.empty();
        },
        OnUserStatusReceived: function(errorCode, status) {
            var $statusDiv = $(".chat-user-status");
            if (errorCode == 0) {
                for (var i = 0, len = status.length; i < len; i++) {
                    if (status[i].userId == this.userId) {
                        var currentStatus = uc.util.StorageUtil.getData("USER_STATUS:status");
                        if (currentStatus != status[i].status) {
                            uc.util.StorageUtil.setData("USER_STATUS:status", status[i].status);
                            uc.util.StorageUtil.setData("USER_STATUS:promptStatus", "1");
                        }

                        uc.util.StorageUtil.setData("USER_STATUS:status", status[i].status);
                        uc.util.StorageUtil.setData("USER_STATUS:conversation", "");
                        $statusDiv.find("div").removeClass();
                        this.currentUserStatus = status[i].status;
                        switch (status[i].status) {
                            case 1:
                                $statusDiv.find("div").addClass("status-online");
                                $statusDiv.find("span").text(uc_resource.UserStatus.OnLine);
                                break;
                            case 2:
                                $statusDiv.find("div").addClass("status-busy");
                                $statusDiv.find("span").text(uc_resource.UserStatus.Busy);
                                break;
                            case 3:
                                break;
                            case 4:
                                $statusDiv.find("div").addClass("status-incall");
                                $statusDiv.find("span").text(uc_resource.UserStatus.InMeeting);
                                break;
                            case 5:
                                $statusDiv.find("div").addClass("status-offline");
                                $statusDiv.find("span").text(uc_resource.UserStatus.OffLine);
                                break;
                            case 6:
                                $statusDiv.find("div").addClass("status-incall");
                                $statusDiv.find("span").text(uc_resource.UserStatus.InCall);
                                break;
                        }
                        break;
                    }
                }
            }
        },
        OnExternalMessageReceived: function (errorCode, conversationType, relateID, msgCount, minTimestamp, messages) {
            if (msgCount < uc.constants.Message.OnceGetHistoryCount && errorCode == 0) {
                this.externalBox.onLoadAllMsgEnd();
            }

            if (this.externalBox) {
                this.externalBox.OnExternalMessageReceived(errorCode, conversationType, relateID, msgCount, minTimestamp, messages);
            }
        },
        onBodyClick: function (e) {
            if (!$(e.target).closest(".external-chat").length && !$(e.target).closest('.fileList-panel')) {
                $(".chat-file").removeClass("selected");
                $('.chat-file').removeClass("icon-chatfile-selected");
                $('.chat-file').addClass("icon-chatfile");
                $(".fileList").hide();
            }

            if (this.externalBox && !$(e.target).closest(".external-chat").length && !$(e.target).hasClass('chat-external')) {
                $('.chat-external').removeClass('icon-external-chat-checked selected').addClass('icon-external-chat');
                this.externalBox.node.parent().hide();
            }
        },

	    getPBXVisible: function(){
		    return uc.modules.main.Main.getPBXVisible();
	    },

	    getCallVisible: function(){
		    return uc.modules.main.Main.getCallVisible();
	    },

	    getCalendarVisible: function () {
		    return uc.modules.main.Main.getCalendarVisible();
	    }
    };
})(uc);