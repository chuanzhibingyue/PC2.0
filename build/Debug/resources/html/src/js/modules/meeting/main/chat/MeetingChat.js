/**
 * Created by waouyang on 16/4/26.
 */
(function(uc){
    uc.modules.meeting.MeetingChat = function(){
        this.bEnableInput = true;
        this.moduleId = uc.constants.UI.ModuleId.Meeting;
    };

    jQuery.extend(uc.modules.meeting.MeetingChat.prototype, {
        init: function(groupId, meetingChatId, eventId, startTime, parentNode){
            console.log(groupId, meetingChatId, eventId, startTime, parentNode);

            this.msgBox = new uc.modules.component.MessageBox();
            this.msgInputBox = new uc.modules.component.MessageInputBox();
            this.msgInputBox.onBeforeSend = this.onBeforeSend;
            this.msgInputBox.setUserData(this.msgBox);

            this.meetingChatId = eventId;//2016-1-22之后統一廢除 meetingId，改為使用eventId
            this.groupId = groupId;
            this.seqs = {};

            this.node = $('<div class="meeting-chat meeting-detail-chat message-box-container"></div>').appendTo(parentNode);
            this.msgBox.init(this.node, this,uc.constants.Chat.ChatType.MeetingChat,groupId);
            this.historyDomStr = "";
            this.lastMsgSeq = 0;
            this.firstGetHistory = true;
            this.getMoreHistoryMsg();
            $('<div class="middle-line"></div>').appendTo(parentNode);
            this.myUserInfo = uc.modules.chat.ChatManager.myUserInfo;
            var siteId = 1;
            if (this.myUserInfo != null) {
                siteId = this.myUserInfo.siteId;
            }
            this.inputNode = $("<div class='chat-input'/>").appendTo(parentNode);
            this.msgInputBox.init(this.inputNode, {
                chatType: uc.constants.Chat.ChatType.MeetingChat,
                chatObjectId: this.groupId,
                siteId: siteId,
                resId: 100,
                eventId: eventId,
                startTime:startTime,
                meetingId: meetingChatId,
                moduleId: this.moduleId
            });
            this.msgInputBox.enable(this.bEnableInput);
            this.msgInputBox.disableAction("file");
            this.msgInputBox.disableAction("code");
            this.msgInputBox.disableAction("history");
            this.msgInputBox.disableAction("cloud");

            this.msgBox.setChatIdAndType(groupId, uc.constants.Chat.ChatType.MeetingChat);
            this.msgBox.bindResize(this.node.parent('.chat-panel'), this.msgInputBox);
            this.msgInputBox.updateInputBoxPosition();

            this.parentNode = parentNode;
        },
        getMsgBox: function() {
            return this.msgBox;
        },
        getMsgInputBox: function() {
            return this.msgInputBox;
        },
        enableInput: function(bEnable) {
            this.bEnableInput = bEnable;
            this.msgInputBox.enable(bEnable);
        },
        getMoreHistoryMsg: function() {
            this.historyDomStr = "";
            var msgBox = this.msgBox;
            msgBox.onMoreMsgLoading();
            uc.util.LogUtil.info("MeetingChat", "getMoreHistoryMsg","data=",{groupId:this.groupId, count:10, page:0, seq:Number(this.lastMsgSeq)});
            uc.IGroupService.getChatHistory(uc.constants.Message.MessageChannel.ChatHistory, uc.constants.Chat.ChatType.MeetingChat,this.groupId, 10, 0, Number(this.lastMsgSeq), function(errorCode){
                if (errorCode != 0) {
                    uc.util.LogUtil.info("MeetingChat","getMoreHistoryMsg", "data=",{groupId:this.groupId, errorCode:errorCode});
                    msgBox.onMoreMsgLoadEnd();
                }
            });
        },

        onBeforeSend: function (sender, args) {
            var msgBox = sender.getUserData();
            console.log(msgBox);
            if (msgBox != null) {
                uc.modules.chat.ChatManager.procBeforeSend(sender, args, msgBox, msgBox.chatType, args.chatObjectId);
            }

            if(uc.modules.chat.ChatManager.meetingChat) {
                var msgBox = uc.modules.chat.MeetingChat.getMsgBox();
                uc.modules.chat.ChatManager.procBeforeSend(sender, args, msgBox, msgBox.chatType, args.chatObjectId);
            }
        },

        OnMeetingMessageSent: function(errorCode, groupId, eventId, meetingStartTime, msgId, msgSeq, msgTime){
            if (this.groupId != groupId) {
                return;
            }
            if (this.msgBox == null) {
                return;
            }
            if (errorCode == 0) {
                var tstr = uc.util.TimeUtil.getMessageTimeString(new Date(msgSeq), null, uc.settings.meeting.timeZone.offset);
                this.msgBox.updateMsgTime(msgId, tstr);
                this.msgBox.showSendFailFlag(msgId, false);
                this.msgBox.updateMessageSeq(msgId,msgSeq);
            } else {
                this.msgBox.showSendFailFlag(msgId, true);
            }
        },
        OnMeetingChatHistoryReceived: function(errorCode, groupId, msgCount, minTimestamp){
            if (this.groupId != groupId) {
                return;
            }

            if (this.firstGetHistory) {
                uc.modules.chat.ChatManager.appendUploadMessages();
            }

            if (errorCode == 0) {
                this.lastMsgSeq = minTimestamp;
            }
            this.msgBox.onMoreMsgLoadEnd();
            if (msgCount < 10 && errorCode == 0) {
                this.msgBox.onLoadAllMsgEnd();
            }
            if (this.historyDomStr == "") {
                this.firstGetHistory = false;
                return;
            }
            uc.util.LogUtil.info("MeetingChat","OnMeetingChatHistoryReceived","data=",{errorCode:errorCode,groupId:groupId});
            this.msgBox.prependMessage(this.historyDomStr);
            if (this.firstGetHistory) {
                this.msgBox.scrollToEnd();
            }
            this.firstGetHistory = false;
            this.historyDomStr = "";
        },
        OnMinutesDetailsReceived:function(errorCode,eventId,minutesId){
            uc.util.LogUtil.info("MeetingDetailChat","OnMinutesDetailsReceived","minutesId:"+minutesId+",errorCode:"+errorCode);
            if(errorCode != 0){
                uc.modules.component.PromptMessageBox.open({title: uc_resource.MessageBox.Notification, message: uc_resource.MessageBox.fileIdNotExist, buttonMode:"ok",OkText:uc_resource.MessageBox.OK},
                    function(result){
                        this.close();
                    });
                return;
            }
            var documentInfo=uc.modules.chat.ChatManager.fileListTagName[minutesId];
            uc.util.LogUtil.info("MeetingDetailChat","OnMinutesDetailsReceived",'fileListTagName',{documentInfo:documentInfo});
            if(documentInfo  && documentInfo.msgId){
                var msgId=documentInfo.msgId;
                var fsInfo=uc.modules.chat.ChatManager.FSFileInfo[msgId];
                var type=uc.modules.chat.ChatManager.fileListTagName[minutesId].type;
                delete uc.modules.chat.ChatManager.fileListTagName[minutesId];
                switch (type){
                    case "receive":
                        var T=$("#"+msgId).find(".op-receive");
                        this.msgBox.fileMsgOperation(T,fsInfo,"receive");
                        break;
                    case "saveAs":
                        var T=$("#"+msgId).find(".op-saveAs");
                        this.msgBox.fileMsgOperation(T,fsInfo,"saveAs");
                        break;
                    case "forward":
                        uc.modules.chat.MessageManager.forwardFileMessage(fsInfo.fileName,fsInfo.fileSize,fsInfo.fileURL);
                        break;
                    case "fsSaveCloud":
                        this.msgBox.OnSaveFileToCloud(fsInfo.fileURL,fsInfo.fileName);
                        break;
                    case "cloudFileDownload":
                        var cloudInfo=uc.modules.chat.ChatManager.cloudFileInfo[msgId];
                        if(cloudInfo){
                            var cloudData=JSON.parse(cloudInfo);
                            var TagType=documentInfo.TagType;
                            var that;
                            if(TagType=="receive"){
                                that=$("#"+msgId).find(".cloudReceive")
                            }else{
                                that=$("#"+msgId).find(".cloudDow")
                            }
                            uc.modules.chat.ChatManager.downloadCloudDir(cloudData,function(data,savePath){
                                self.msgBox.OnStartDownloadCloudFile(that,data,savePath);
                            },documentInfo.TagType);
                        }
                        break;
                    case "cloudFileForward":
                        var cloudInfo=uc.modules.chat.ChatManager.cloudFileInfo[msgId];
                        if(cloudInfo){
                            var cloudFileInfoArr= JSON.parse(cloudInfo);
                            var fileName=cloudFileInfoArr.filename;
                            var fileSize=cloudFileInfoArr.filesize;
                            var fileUrl="";
                            uc.modules.chat.MessageManager.forwardCloudFileMessage(fileName,fileSize,fileUrl,cloudInfo,cloudFileInfoArr);
                        }
                        break;
                    case "cloudFileSaveCloud":
                        var cloudInfo=uc.modules.chat.ChatManager.cloudFileInfo[msgId];
                        if(cloudInfo){
                            uc.modules.cloud.CloudManager.saveCloudFile(null,cloudInfo);
                        }
                        break;
                }
            }
        },
        procMessageDomStr: function(channel, domStr, msgId, msgSeq) {
            uc.util.LogUtil.info("MeetingChat","procMessageDomStr","msgSeq=",{msgSeq:msgSeq,channel:channel});
            if (this.seqs[msgSeq] != null){
                uc.util.LogUtil.info("MeetingChat","procMessageDomStr","the msg seq has been existed!=",{msgSeq:msgSeq});
                return;
            }
            this.seqs[msgSeq] = true;

            this.lastMsgSeq = msgSeq;
            if (this.msgBox.isMsgIdExist(msgId)) {
                uc.util.LogUtil.info("MeetingChat","procMessageDomStr","the msg seq has been existed!=",{msgId:msgId});
                return;
            }
            if (uc.constants.Message.MessageChannel.ChatHistory == channel) {
                this.historyDomStr = domStr + this.historyDomStr;
                return;
            }
            this.msgBox.appendMessage(domStr);
        },
        OnMeetingTextMessageReceived: function(channel, sendState, self, from, meetingId, eventId, meetingStartTime, textType, textContent, msgId, msgSeq, msgTime, textFormat){
            if (this.meetingChatId != eventId) {
                return;
            }
            var domStr = this.msgBox.getTextMsgDomStr(sendState, self, from, msgId, msgSeq, textType, textContent, msgTime, msgTime, textFormat);
            this.procMessageDomStr(channel, domStr, msgId, msgSeq);
        },
        OnMeetingImageMessageReceived: function(channel, sendState,  self, from, meetingId, eventId, meetingStartTime, fileName, fileSize, fileURL, thumbnails, msgId, msgSeq, msgTime){
            if (this.meetingChatId != eventId) {
                return;
            }
            var domStr = this.msgBox.getImgMsgDomStr(sendState, self, from, msgId, msgSeq, fileName, fileSize, fileURL, thumbnails, msgTime);
            this.procMessageDomStr(channel, domStr, msgId, msgSeq);
        },
        OnMeetingAudioMessageReceived: function(channel, sendState,  self, from, meetingId, eventId, meetingStartTime, fileName, fileSize, fileURL, duration, msgId, msgSeq,msgTime){
            if (this.meetingChatId != eventId) {
                return;
            }
            var domStr = this.msgBox.getAudioMsgDomStr(sendState, self, from, msgId, msgSeq, fileName, fileSize, fileURL, duration, msgTime);
            this.procMessageDomStr(channel, domStr, msgId, msgSeq);
        },
        OnMeetingVideoMessageReceived: function(channel, sendState,  self, from, meetingId, eventId, meetingStartTime, fileName, fileSize, fileURL, thumbnails, duration, msgId, msgSeq, msgTime){
            if (this.meetingChatId != eventId) {
                return;
            }
        },
        OnMeetingReportMessageReceived: function (channel, sendState, from, to, eventId, msgId, msgSeq, msgTime, reportTitle, reportUrl) {
            if (this.meetingChatId != eventId) {
                return;
            }

            var domStr = this.msgBox.getMeetingReportMsgDomStr(eventId, to.siteId, msgId, msgSeq, msgTime, reportTitle, reportUrl);
            this.procMessageDomStr(channel, domStr, msgId, msgSeq);
        },
        OnMeetingInviteMessageReceived: function(channel, from, to, meetingId, eventId, meetingName, accreditUserId, hostId, isRecurrent, meetingStartTime, msgId, msgSeq, msgTime){
            return;
            /*
             if (this.meetingChatId != meetingId) {
             return;
             }
             var message;
             if(hostId == this.myUserInfo.userId){
             if(isRecurrent){
             message = '您安排了一场周期会议';
             }else{
             message = '您安排了一场会议';
             }
             }else{
             var userName = this.getMessageUserName(hostId);
             if(isRecurrent){
             message = userName + '邀请您加入周期会议';
             }else{
             message = userName +'邀请您加入会议';
             }
             }
             var domStr = this.msgBox.getSysMsgDomStr(message, msgSeq);
             this.procMessageDomStr(channel, domStr, msgId, msgSeq);
             */
        },
        OnMeetingInvitationAcceptedMessageReceived: function(channel, from, to, meetingId, eventId, meetingName, accreditUserId, hostId, isRecurrent,  meetingStartTime, msgId, msgSeq, msgTime){
            return;
            /*
             if (this.meetingChatId != meetingId) {
             return;
             }
             var message;
             var userName = this.getMessageUserName(hostId);
             if(isRecurrent){
             message = userName + '接受了您的周期约会邀请';
             }else if(from.userId == this.myUserInfo.userId){
             message = '您接受了会议邀请';
             }else{
             message = userName + '接受了您的会议邀请';
             }
             var domStr = this.msgBox.getSysMsgDomStr(message, msgSeq);
             this.procMessageDomStr(channel, domStr, msgId, msgSeq);
             */
        },
        OnMeetingInvitationRejectedMessageReceived: function(channel, from, to, meetingId, eventId, meetingName, accreditUserId, hostId, isRecurrent, meetingStartTime, msgId, msgSeq, msgTime){
            return;
            /*
             if (this.meetingChatId != meetingId) {
             return;
             }
             var message;
             var userName = this.getMessageUserName(hostId);
             if(from.userId == this.myUserInfo.userId){
             message = '您拒绝了会议邀请';
             }else{
             message = userName + '拒绝了您的会议邀请';
             }
             var domStr = this.msgBox.getSysMsgDomStr(message, msgSeq);
             this.procMessageDomStr(channel, domStr, msgId, msgSeq);
             */
        },
        OnMeetingUpdateMessageReceived: function(channel, from, to, meetingId, eventId, meetingName, accreditUserId, hostId, isRecurrent, meetingStartTime, updateFlag, msgId, msgSeq, msgTime){
            return;
            /*
             if (this.meetingChatId != meetingId) {
             return;
             }
             var message;
             if(hostId == this.myUserInfo.userId || from.userId == this.myUserInfo.userId){
             message = '您变更了会议信息';
             }else{
             var userName = this.getMessageUserName(hostId);
             message = userName + '变更了会议信息';
             }
             var domStr = this.msgBox.getSysMsgDomStr(message, msgSeq);
             this.procMessageDomStr(channel, domStr, msgId, msgSeq);
             */
        },
        OnMeetingForwardedMessageReceived: function(channel, from, to, meetingId, eventId, meetingName, accreditUserId, hostId, isRecurrent, meetingStartTime, msgId, msgSeq, msgTime){
            return;
            /*
             if (this.meetingChatId != meetingId) {
             return;
             }
             var message;
             if(from.userId == this.myUserInfo.userId){
             message = '您转发了' + this.getMessageUserName(hostId) + '的会议邀请';
             }else if(hostId == this.myUserInfo.userId){
             message = this.getMessageUserName(accreditUserId) + '转发了您的会议';
             }else{
             message = this.getMessageUserName(accreditUserId) + '转发了会议邀请给您';
             }
             var domStr = this.msgBox.getSysMsgDomStr(message, msgSeq);
             this.procMessageDomStr(channel, domStr, msgId, msgSeq);
             */
        },
        OnMeetingCancelledMessageReceived: function(channel, from, to, meetingId, eventId, groupId, meetingName, accreditUserId, hostId, isRecurrent,meetingStartTime, msgId, msgSeq, msgTime){
            if (this.meetingChatId != eventId ||  accreditUserId!=0) {
                return;
            }
            //var message = this.getMessageUserName(from.userId) + uc_resource.Message.CancelMeeting;
            var message = uc.util.StringUtil.formatString(uc_resource.Message.CancelMeeting,[this.getMessageUserName(from.userId)]);
            var domStr = this.msgBox.getSysMsgDomStr(message, msgSeq);
            this.procMessageDomStr(channel, domStr, msgId, msgSeq);
        },
        OnMinutesCreateMessageReceived: function(channel, from, to, operatorId,minutesId, minutesTitle,meetingId,eventId, meetingName, meetingStartTime, updateTime, msgId, msgSeq, msgTime){
            if (this.meetingChatId != eventId) {
                return;
            }
            //var message = this.getMessageUserName(from.userId) + uc_resource.Message.CreateMeetingSummary;
            var message = uc.util.StringUtil.formatString(uc_resource.Message.CreateMeetingSummary,[this.getMessageUserName(from.userId)]);
            var domStr = this.msgBox.getSysMsgDomStr(message, msgSeq);
            this.procMessageDomStr(channel, domStr, msgId, msgSeq);
        },
        OnMinutesUpdateMessageReceived: function(channel, from, to, operatorId,  minutesId, minutesTitle, meetingId,eventId, meetingName, meetingStartTime, updateTime, msgId, msgSeq, msgTime){
            if (this.meetingChatId != eventId) {
                return;
            }
            //var message = this.getMessageUserName(from.userId) + uc_resource.Message.updateMinutes;
            var message = uc.util.StringUtil.formatString(uc_resource.Message.updateMinutes,[this.getMessageUserName(from.userId)]);
            var domStr = this.msgBox.getSysMsgDomStr(message, msgSeq);
            this.procMessageDomStr(channel, domStr, msgId, msgSeq);
        },
        OnMeetingSummaryVideoMessageReceived:function(channel, self,from, to, operatorId, summaryId, summaryName, conferenceId, eventId, meetingName, meetingStartTime, updateTime,type,videoName,videoURL,videoThumb,thumbnailsURL,length,size,operateName,operateAvatar,msgId, msgSeq, msgTime){
            if (this.meetingChatId != eventId) {
                return;
            }
            var domStr = this.msgBox.getSummaryAudioMsgDomStr(self,from, to, operatorId, summaryId, summaryName, conferenceId, eventId, meetingName, meetingStartTime, updateTime,type,videoName,videoURL,videoThumb,thumbnailsURL,length,size,operateName,operateAvatar,msgId, msgSeq, msgTime);
            this.procMessageDomStr(channel, domStr, msgId, msgSeq);
        },
        getMessageUserName: function(userId){
            var userName;
            if(userId == this.myUserInfo.userId){
                userName = uc_resource.Message.You;
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
        getGroupMessageMembersName: function(operatorId, members){
            var userId, userNameArray = [], userName;
            for(var i = 0, len = members.length; i < len; i++){
                if(typeof members[i] == 'object'){
                    userId = members[i].userId;
                }else{
                    userId = members[i];
                }
                if(userId == operatorId){
                    continue;
                }
                userName = this.getMessageUserName(userId);
                userNameArray.push(userName);
            }
            return userNameArray.join('、');
        },
        resendMessage: function (msgId){
            var This = this;
            console.log("resendMessage", msgId);
            uc.IMeetingService.resendMessage(msgId,
                function(args){
                    console.log(args);
                    if (args[0] != 0) {
                        This.msgBox.showSendFailFlag(msgId, true);
                    }
                });
        },
        OnMeetingUnreadUserCountEvent:function(userId,info){
            if(userId==this.groupId){
                this.msgBox.setUnreadCountEvent(userId,info);
            }
        },
        OnMeetingUnreadUserListEvent:function(userId,info){
            if(userId==this.groupId){
                this.msgBox.setUnreadUserList(userId,info);
            }
        },
        clear: function(){
            if(this.msgBox != null){
                this.msgBox.clear();
            }

            if(this.msgInputBox && this.moduleId){
                this.msgInputBox.clear();
                this.msgInputBox.clearHtml();
            }
            this.parentNode  = null;
            this.clearMeetingUploadTip();
            //uc.modules.chat.ChatManager.meetingChats = [];
        },
        //meetingpanel点击上传会议资料按钮，打开聊天抽屉，并且显示tip，“在这里上传资料”
        showMeetingUploadTip: function (){
            var This = this;
            this.clearMeetingUploadTip();
            this.MeetingUploadTip = $("<div class='chat-Meeting-UploadTip'>"+uc_resource.Meeting.MeetingUploadTip+"</div>");
            this.parentNode.append(this.MeetingUploadTip);
            //_UpLoadFileTips.append("<div class='chat-Meeting-UploadTip-range'/>");
            //动画显示
            this.MeetingUploadTip.fadeIn(100).delay(2000).fadeOut(100,function(){
                This.clearMeetingUploadTip();
            });
        },
        clearMeetingUploadTip: function(){
            if(this.MeetingUploadTip){
                this.MeetingUploadTip.find("*").off();
                this.MeetingUploadTip.remove();
                this.MeetingUploadTip = null;
            }

        },
        OnMeetingDocumentUploadMessageReceived: function(eventId,conversationType, channel, sendState, self, from, operatorId, fileId, fileName,
                                                         fileSize, downloadUrl, fileFormat, desc, msgId, msgSeq, msgTime, cloudType, localPath){
            if (this.meetingChatId != eventId) {
                return;
            }
            var domStr = this.msgBox.getFileMsgDomStr(sendState, self, from, msgId,msgSeq, fileName, fileSize, downloadUrl, msgTime, localPath, fileId, '',conversationType);
            this.procMessageDomStr(channel, domStr, msgId, msgSeq);
        },
        OnDocumentDeleteMessageReceived: function (eventId,conversationType, channel, from, to, operatorId, fileId, fileName, msgId, msgSeq, msgTime) {
            if (this.meetingChatId != eventId) {
                return;
            }
            var message = this.getMessageUserName(operatorId) + uc_resource.GroupChat.Delete+'(' + fileName + ')';
            var domStr = this.msgBox.getSysMsgDomStr(message, msgSeq);
            this.procMessageDomStr(channel, domStr, msgId, msgSeq);
        }
    })
})(uc);