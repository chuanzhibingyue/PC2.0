/**
 * Created by senlin.ouyang on 2015/7/21.
 **/
(function(uc){
    uc.modules.chat.MeetingChat = {
        init: function(chatId, meetingData){
            this.data = uc.IMeetingService.getMeetingInfoByIdSync(meetingData.eventId, meetingData.startTime, 0, 0);

            if (this.data && this.data.length) {
                this.meetingInfo = this.data[0];
            }
            this.meetingChatId = meetingData.eventId;
            uc.util.LogUtil.info('MeetingChat', 'MeetingChatInit', 'Init', {chatId: chatId, meetingInfo: this.meetingInfo, meetingData: meetingData});
            this.nodeHtml = "<div class='meeting-panel chat-max'>"
                    + "    <div class='top'><h1 class='head text-selectable'>"+ this.meetingInfo.title +"</h1><div>"
                    + "    <div class='attendee-status'>"
                    + "        <a class='attendee-cancel hide'>" + uc_resource.Meeting.Accept + "</a>"
                    + "        <a class='attendee-reject hide'> " + uc_resource.Meeting.Reject + " </a>"
                    + "    </div>"
                    + "    </div><span class='status hide'></span></div>"
                    + "    <div class='functions-bar'>"
                    //+ "        <a href='javascript:void(0);' action='detail' class='detail function-menu-selected'></a>"
                    + "        <span class='time'>"+ uc.util.TimeUtil.showMeetingTime(this.meetingInfo, uc.settings.meeting.timeZone.offset) +"</span>"
                    + "        <a href='javascript:void(0);' class='function-btn reAccept hide'>"+ uc_resource.Chat.ReAccept +"</a>"
                    + "        <a href='javascript:void(0);' class='function-btn detail'>"+ uc_resource.Chat.ViewDetail +"</a>"
                    //+ "        <a href='javascript:void(0);' action='join' qtips='" + uc_resource.Meeting.JoinMeeting + "' class='function-btn join'>" + uc_resource.Meeting.JoinMeeting + "</a>"
                    + "        <div class='function-pop hide'>"
                    + "            <a href='javascript:void(0);' action='edit' class='function-pop-menu'>" + uc_resource.Meeting.EditMeeting + "</a>"
                    + "            <a href='javascript:void(0);' action='cancel' class='function-pop-menu'>" + uc_resource.Meeting.CancelMeeting + "</a>"
                    + "        </div>"
                    + "    </div>"
                    + "</div>";
            this.historyDomStr = "";
            this.lastMsgSeq = 0;
            this.firstGetHistory = true;
            this.meetingChatId = this.meetingInfo.eventId;//2016-1-22之后統一廢除 meetingId，改為使用eventId
            this.groupId = chatId;
            this.seqs = {};
            this.headerNode = $('<div class="chat-header chat-max"></div>');
            this.headerNode.append(this.nodeHtml);
            this.headerNode.appendTo(uc.ui.Message.ContentPanelTitle);

            this.middleLine = $('<div class="middle-line"></div>').appendTo(uc.ui.Message.ContentPanelBody);
            this.myUserInfo = uc.modules.chat.ChatManager.myUserInfo;
            var siteId = 1;
            if (this.myUserInfo != null) {
                siteId = this.myUserInfo.siteId;
            }
            this.inputNode = $("<div class='chat-input'/>").appendTo(uc.ui.Message.ContentPanelBody);

            this.node = $('<div class="meeting-chat message-box-container"></div>').appendTo(uc.ui.Message.ContentPanelBody);
            this.msgBox = uc.modules.chat.ChatManager.msgBox;
            this.msgBox.init(this.node, this, uc.constants.Chat.ChatType.MeetingChat, chatId);
            this.msgInputBox = uc.modules.chat.ChatManager.msgInputBox;
            this.msgInputBox.init(this.inputNode, {
                chatType: uc.constants.Chat.ChatType.MeetingChat,
                chatObjectId: this.groupId,
                siteId: siteId,
                resId: 100,
                eventId: meetingData.eventId,
                startTime: meetingData.startTime,
                meetingId: meetingData.meetingChatId,
                moduleId: uc.constants.UI.ModuleId.Message
            });
            this.msgInputBox.onBeforeSend = this.onBeforeSend;
            this.msgInputBox.setUserData(this.msgBox);

            this.getMoreHistoryMsg();

            this.msgInputBox.enable(!meetingData.isCancelled);
            this.msgInputBox.disableAction("file");
            this.msgInputBox.disableAction("code");
            //this.msgInputBox.disableAction("history");
            this.msgInputBox.unDisableAction("uploadMeetingDoc");
            this.msgInputBox.disableAction("cloud");


            //this.msgBox.setChatIdAndType(this.groupId, uc.constants.Chat.ChatType.MeetingChat);
            this.msgBox.bindResize(this.node.parent('.chat-max'), this.msgInputBox);
            this.msgInputBox.updateInputBoxPosition();
            this.bindEvent();
            this.setAction(this.meetingInfo.status);
        },
        bindEvent: function () {
            var self = this;
            this.headerNode.unbind().on("click", "a.detail", function () {
                uc.util.LogUtil.info('MeetingChat', 'bindEvent', 'detail click', {eventId: self.meetingInfo.eventId, meetingData: self.meetingInfo});
                uc.IUIService.showMeetingDetail(self.meetingInfo, false);
            }).on("click", ".attendee-cancel, .reAccept", function () {
                uc.IMeetingService.acceptMeetingInvitation(
                    self.meetingInfo.eventId, 0,
                    0,
                    function(args) {
                        uc.util.LogUtil.info("MeetingChat", "bindEvent", "acceptMeetingInvitation", {errorCode:args[0]});
                    }
                );
            }).on("click", ".attendee-reject", function () {
                uc.IMeetingService.rejectMeetingInvitation(
                    self.meetingInfo.eventId, 0,
                    0,
                    function(args) {
                        uc.util.LogUtil.info("MeetingChat", "bindEvent", "rejectMeetingInvitation", {errorCode:args[0]});
                    }
                );
            });
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
        OnRevocationMessageReceived: function(channel, conversationId, conversationType, from, to, msgId, msgSeq, msgTime, seqId, eventId){
            if (this.meetingChatId != eventId) {
                return;
            }
            var message = uc.util.StringUtil.formatString(uc_resource.Message.RevocationMessage, [this.getMessageUserName(from.userId)]);
            var domStr = this.msgBox.getSysMsgDomStr(message, msgSeq);
            this.procMessageDomStr(channel, domStr, msgId, msgSeq);
            this.msgBox.chatList.find("li[seq='" + seqId + "']").remove();
        },
        getMoreHistoryMsg: function() {
            this.historyDomStr = "";
            var msgBox = this.msgBox;
            msgBox.onMoreMsgLoading();
            uc.util.LogUtil.info("MeetingChat", "getMoreHistoryMsg","data=",{groupId:this.groupId, count:10, page:0, seq:Number(this.lastMsgSeq)});
            uc.IGroupService.getChatHistory(uc.constants.Message.MessageChannel.ChatHistory, uc.constants.Chat.ChatType.MeetingChat,this.groupId, uc.constants.Message.OnceGetHistoryCount, 0, Number(this.lastMsgSeq), function(errorCode){
                if (errorCode != 0) {
                    uc.util.LogUtil.info("MeetingChat","getMoreHistoryMsg", "data=",{groupId:this.groupId, errorCode:errorCode});
                    msgBox.onMoreMsgLoadEnd();
                }
            });
        },
        showFileCommentByFileId: function(fileId){
            if(fileId=="" || fileId=="null" || fileId==0)return;
            var eventId = 0;
            uc.IMeetingService.getMinutesDetails(eventId, Number(fileId), function(args){
                $(".chat-file").addClass("selected");
            });
        },

        onBeforeSend: function (sender, args) {
            var msgBox = sender.getUserData();
            console.log(msgBox);
            if (msgBox != null) {
                uc.modules.chat.ChatManager.procBeforeSend(sender, args, msgBox, msgBox.chatType, args.chatObjectId);
            }
        },
        OnMinutesDetailsReceived:function(errorCode,eventId,minutesId){
            uc.util.LogUtil.info("MeetingChat","OnMinutesDetailsReceived","minutesId:"+minutesId+",errorCode:"+errorCode);
            if(errorCode != 0){
                uc.modules.component.PromptMessageBox.open({title: uc_resource.MessageBox.Notification, message: uc_resource.MessageBox.fileIdNotExist, buttonMode:"ok",OkText:uc_resource.MessageBox.OK},
                    function(result){
                        this.close();
                    });
                return;
            }
            var documentInfo=uc.modules.chat.ChatManager.fileListTagName[minutesId];
            uc.util.LogUtil.info("MeetingChat","OnMinutesDetailsReceived",'fileListTagName',{documentInfo:documentInfo});
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
            if (msgCount < uc.constants.Message.OnceGetHistoryCount && errorCode == 0) {
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

        setAction: function(attendeeStatus) {
            var self = this;
            var meetingStatus = this.meetingInfo.status;
            var myStatus = null;
            var currentUserId = uc.IContactService.getCurrentUserInfo().userId;

            // if meeting has canceled
            if (meetingStatus == 1) {
                this.showStatus(uc.constants.Meeting.AttendeeStatus.Cancel);
                return;
            }

            if (currentUserId != this.meetingInfo.hostId) {
                for (var i = 0, length = this.meetingInfo.attendees.length; i < length; i ++) {
                    if (Number(this.meetingInfo.attendees[i].account) == currentUserId) {
                        myStatus = this.meetingInfo.attendees[i].status;
                        break;
                    }
                }

                var status = attendeeStatus || myStatus;
                this.showStatus(status);
            }
        },

        // 0 pending
        // 1 accept
        // 2 reject
        // 3 cancel
        showStatus: function(status) {
            switch (status) {
                case uc.constants.Meeting.AttendeeStatus.Pending:
                    this.headerNode.find(".attendee-status .attendee-cancel").removeClass("hide");
                    this.headerNode.find(".attendee-status .attendee-reject").removeClass("hide");
                    this.headerNode.find(".top .status").addClass("hide");
                    this.headerNode.find(".detail").removeClass("hide");
                    this.headerNode.find(".reAccept").addClass("hide");
                    this.enableInput(true);
                    break;
                case uc.constants.Meeting.AttendeeStatus.Accepted:
                    this.headerNode.find(".attendee-status .attendee-cancel").addClass("hide");
                    this.headerNode.find(".attendee-status .attendee-reject").addClass("hide");
                    this.headerNode.find(".detail").removeClass("hide");
                    this.headerNode.find(".reAccept").addClass("hide");
                    this.headerNode.find(".top .status").removeClass("hide");
                    this.headerNode.find(".top .status").text(uc_resource.Meeting.Accepted);
                    this.enableInput(true);
                    break;
                case uc.constants.Meeting.AttendeeStatus.Reject:
                    this.headerNode.find(".attendee-status .attendee-cancel").addClass("hide");
                    this.headerNode.find(".attendee-status .attendee-reject").addClass("hide");
                    this.headerNode.find(".detail").addClass("hide");
                    this.headerNode.find(".reAccept").removeClass("hide");
                    this.headerNode.find(".top .status").removeClass("hide");
                    this.headerNode.find(".top .status").text(uc_resource.Meeting.Rejected);
                    break;
                case uc.constants.Meeting.AttendeeStatus.Cancel:
                    this.headerNode.find(".attendee-status .attendee-cancel").addClass("hide");
                    this.headerNode.find(".attendee-status .attendee-reject").addClass("hide");
                    this.headerNode.find(".detail").addClass("hide");
                    this.headerNode.find(".reAccept").addClass("hide");
                    this.headerNode.find(".top .status").removeClass("hide");
                    this.headerNode.find(".top .status").text(uc_resource.Meeting.Canceled);
                    this.msgInputBox.enable(false);
                    this.middleLine.css('cursor', 'default');
                    this.enableInput(false);
                    break;
            }
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
            //return;
            if (this.meetingChatId != eventId) {
                return;
            }

            this.data = uc.IMeetingService.getMeetingInfoByIdSync(eventId, 0, 0, accreditUserId);
            if (this.data && this.data.length) {
                this.meetingInfo = this.data[0];
            }

            this.setAction(uc.constants.Meeting.AttendeeStatus.Pending);

            var message;
            if(hostId == this.myUserInfo.userId){
                if(isRecurrent){
                    message = uc_resource.Meeting.cycleMeeting;
                }else{
                    message = uc_resource.Meeting.CreateRecurrentMeetingByMe;
                }
            }else{
                var userName = this.getMessageUserName(hostId);
                if(isRecurrent){
                    message = userName + uc_resource.Meeting.invitedToCycleMeeting;
                }else{
                    message = userName +uc_resource.Meeting.invitedToMeeting;
                }
            }
            //var domStr = this.msgBox.getSysMsgDomStr(message, msgSeq);
            //this.procMessageDomStr(channel, domStr, msgId, msgSeq);
        },
        OnMeetingInvitationAcceptedMessageReceived: function(channel, from, to, meetingId, eventId, meetingName, accreditUserId, hostId, isRecurrent,  meetingStartTime, msgId, msgSeq, msgTime){
            return;
            if (this.meetingChatId != eventId) {
                return;
            }
            var message;
            var userName = this.getMessageUserName(hostId);
            if(isRecurrent){
                message = userName + uc_resource.Meeting.acceptCycleMeeting;
            }else if(from.userId == this.myUserInfo.userId){
                message = uc_resource.Meeting.acceptMeeting;
            }else{
                message = userName + uc_resource.Meeting.meetingAccepted;
            }
            var domStr = this.msgBox.getSysMsgDomStr(message, msgSeq);
            this.procMessageDomStr(channel, domStr, msgId, msgSeq);
        },
        OnMeetingInvitationRejectedMessageReceived: function(channel, from, to, meetingId, eventId, meetingName, accreditUserId, hostId, isRecurrent, meetingStartTime, msgId, msgSeq, msgTime){
            return;
            if (this.meetingChatId != eventId) {
                return;
            }
            var message;
            var userName = this.getMessageUserName(hostId);
            if(from.userId == this.myUserInfo.userId){
                message = uc_resource.Meeting.refuseMeeting;
            }else{
                message = userName + uc_resource.Meeting.meetingRefused;
            }
            var domStr = this.msgBox.getSysMsgDomStr(message, msgSeq);
            this.procMessageDomStr(channel, domStr, msgId, msgSeq);
        },
        OnMeetingUpdateMessageReceived: function(channel, from, to, meetingId, eventId, meetingName, accreditUserId, hostId, isRecurrent, meetingStartTime, updateFlag, msgId, msgSeq, msgTime){
            if (this.meetingChatId != eventId) {
                return;
            }
            var message;
            if(hostId == this.myUserInfo.userId || from.userId == this.myUserInfo.userId){
                message = uc_resource.Meeting.updateMeetingByYou;
            }else{
                var userName = this.getMessageUserName(hostId);
                message = userName + uc_resource.Meeting.updateMeeting;
            }
            var domStr = this.msgBox.getSysMsgDomStr(message, msgSeq);
            //this.procMessageDomStr(channel, domStr, msgId, msgSeq);
            this.updateMeetingInfo(eventId, meetingStartTime);
        },
        OnMeetingForwardedMessageReceived: function(channel, from, to, meetingId, eventId, meetingName, accreditUserId, hostId, isRecurrent, meetingStartTime, msgId, msgSeq, msgTime){
            //return;
            if (this.meetingChatId != eventId) {
                return;
            }
            this.data = uc.IMeetingService.getMeetingInfoByIdSync(eventId, 0, 0, accreditUserId);
            if (this.data && this.data.length) {
                this.meetingInfo = this.data[0];
            }

            this.setAction(uc.constants.Meeting.AttendeeStatus.Pending);
            return;
            var message;
            if(from.userId == this.myUserInfo.userId){
                message = uc.util.StringUtil.formatString(uc_resource.Meeting.forwardMeetingByYou,[this.getMessageUserName(hostId)]);
            }else if(hostId == this.myUserInfo.userId){
                message = this.getMessageUserName(accreditUserId) + uc_resource.Meeting.forwardYourMeeting;
            }else{
                message = this.getMessageUserName(accreditUserId) + uc_resource.Meeting.forwardMeetingToYou;
            }
            //var domStr = this.msgBox.getSysMsgDomStr(message, msgSeq);
            //this.procMessageDomStr(channel, domStr, msgId, msgSeq);
        },
        OnMeetingCancelledMessageReceived: function(channel, from, to, meetingId, eventId, groupId, meetingName, accreditUserId, hostId, isRecurrent,meetingStartTime, msgId, msgSeq, msgTime){
            if (this.meetingChatId != eventId ||  accreditUserId!=0) {
                return;
            }
            //var message = this.getMessageUserName(from.userId) + uc_resource.Message.CancelMeeting;
            var message = uc.util.StringUtil.formatString(uc_resource.Message.CancelMeeting,[this.getMessageUserName(from.userId)]);
            var domStr = this.msgBox.getSysMsgDomStr(message, msgSeq);
            this.procMessageDomStr(channel, domStr, msgId, msgSeq);
            this.showStatus(uc.constants.Meeting.AttendeeStatus.Cancel);
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

        OnMeetingAcceptMessageReceived: function(channel, from, to, meetingId, eventId, meetingName, accreditUserId, hostId, isRecurrent, meetingStartTime, msgId, msgSeq, msgTime) {
            if (this.meetingChatId != eventId) {
                return;
            }

            this.data = uc.IMeetingService.getMeetingInfoByIdSync(eventId, 0, 0, accreditUserId);
            if (this.data && this.data.length) {
                this.meetingInfo = this.data[0];
            }
            uc.util.LogUtil.info('MeetingChat', 'OnMeetingAcceptMessageReceived', '', {meetingData: this.meetingInfo});

            this.setAction();
        },
        OnMeetingRejectMessageReceived: function(channel, from, to, meetingId, eventId, meetingName, accreditUserId, hostId, isRecurrent, meetingStartTime,updateFlag, msgId, msgSeq, msgTime) {
            if (this.meetingChatId != eventId) {
                return;
            }

            this.data = uc.IMeetingService.getMeetingInfoByIdSync(eventId, 0, 0, accreditUserId);
            if (this.data && this.data.length) {
                this.meetingInfo = this.data[0];
            }
            uc.util.LogUtil.info('MeetingChat', 'OnMeetingRejectMessageReceived', '', {meetingData: this.meetingInfo});

            this.setAction();
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
                var contactInfo = uc.modules.cache.BasicContactInfoCache.getBasicContactInfo(userId);
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
        updateMeetingInfo: function (meetingId, startTime) {

            var meetingInfo = uc.IMeetingService.getMeetingInfoByIdSync(meetingId, startTime, 0, 0);
            this.meetingInfo = meetingInfo[0];
            this.headerNode.find('.text-selectable').text(this.meetingInfo.title);
            this.headerNode.find('.time').html(uc.util.TimeUtil.showMeetingTime(this.meetingInfo, uc.settings.meeting.timeZone.offset));
        },
        clear: function(){
            this.nodeHtml = null;
            this.node = null;
            this.headerNode = null;
            this.chatName = null;
            this.chatSign = null;
            uc.modules.chat.ChatManager.meetingChat = null;
	        if(this.msgBox != null){
                this.msgBox.clear();
                this.msgBox = null;
            }
	        if(this.msgInputBox != null){
                this.msgInputBox.clear();
                this.msgInputBox = null;
            }

            uc.ui.Message.ContentPanelBody.find('*').off();
            uc.ui.Message.ContentPanelBody.empty();
            uc.ui.Message.ContentPanelTitle.empty();
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
	    },
        OnMeetingInfoReceivedById: function (errorCode,eventId, authorizerId, meetingInfoArray) {
            if (this.meetingChatId != eventId) {
                return;
            }

            for (var i = 0; i < meetingInfoArray.length; i++) {
                if (this.meetingChatId == meetingInfoArray[i].eventId) {
                    this.meetingInfo = meetingInfoArray[i];
                    this.setAction();
                }
            }
        }
    };
})(uc);