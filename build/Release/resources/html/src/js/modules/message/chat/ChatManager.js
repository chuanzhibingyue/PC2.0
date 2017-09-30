/*
 * Created by waouyang on 15/8/3.
 */
(function (uc) {
    var rootPath = (function(){
        var href = location.href;
        var index = href.indexOf('/html');
        return href.substring(0, index + 6);
    })();

    uc.modules.chat.ChatManager = {
        fileDownLoadInfo: {},
        fileUpLoadInfo: {},
        cloudFileInfoDownloading: {},
        cloudFileInfo: {},
        downloadedFiles: {},
        meetingChat: null,
        meetDetailChat: null,
        uploadCopyImgs: {},
        fileListTagName:{},
        FSFileInfo: {},
        moduleId:uc.constants.UI.ModuleId.Message,
        inited:false,
        superGroupMemberBgColor: {},
        chatMsg:true,
        init: function () {
        },
        newMeetingChat: function(moduleId) {
            if (this.myUserInfo == null) {
                this.myUserInfo = uc.IContactService.getCurrentUserInfo();
            }
            var meetingChat = new uc.modules.chat.MeetingDetailChat(moduleId);
            this.meetDetailChat = meetingChat;
            return meetingChat;
        },
        setModuleId:function(moduleId){
            this.moduleId = moduleId;
            if (uc.modules.conversation.ConversationManager.selectedConversationInfo) {
                this.currentChatType = uc.modules.conversation.ConversationManager.selectedConversationInfo.conversationType;
            }
        },

        initComponents: function() {
            if (this.inited) {
                return;
            }
            this.inited = true;
            this.OnFSTasksStatusCheckLoop();
            if (this.msgBox == null) {
                this.msgBox = new uc.modules.component.MessageBox();
            }
            if (this.fileList == null) {
                this.fileList = new uc.modules.chat.FileList;
            }
//            if (this.fileComment == null) {
//                this.fileComment = new uc.modules.comment.Comment;
//            }
            if (this.msgInputBox == null) {
                this.msgInputBox = new uc.modules.component.MessageInputBox();
                this.msgInputBox.onBeforeSend = this.onBeforeSend;
                this.msgInputBox.onAt = this.onAt;
                this.msgInputBox.onAtsearch = this.onAtsearch;
            }

            //bind cross window event
            uc.IUIService.bindCrossWindowEvent("SendCode", function (eventData) {
                uc.modules.chat.ChatManager.OnSendCode(eventData);
            });
            uc.IUIService.bindCrossWindowEvent("sendFromMount", function (eventData) {
                var obj = JSON.parse(eventData.data);
                var chatInfo = JSON.parse(eventData.chatInfo);
                for(var i in obj.list){
                    uc.modules.chat.MessageManager.sendCloudFileMessage(chatInfo.options,obj.list[i]);
                }
            });
            $(document.body).click(
                function (e) {
                    var This =  uc.modules.chat.ChatManager;
                    if (This.fileList != null) {
                        This.fileList.onBodyClick(e);
                    }
                    if (This.fileComment != null) {
                        This.fileComment.onBodyClick(e);
                    }

                    if (This.msgBox != null) {
                        This.msgBox.onBodyClick(e);
                    }
                    if (This.meetDetailChat) {
                        var msgBox = This.meetDetailChat.getMsgBox();
                        if (msgBox != null) {
                            msgBox.onBodyClick(e);
                        }
                    }
                    if (This.currentChatType == uc.constants.Chat.ChatType.GroupChat) {
                        uc.modules.chat.GroupChat.onBodyClick(e);
                    } else if  (This.currentChatType == uc.constants.Chat.ChatType.ProjectChat){
                        uc.modules.chat.ProjectChat.onBodyClick(e);
                    } else if  (This.currentChatType == uc.constants.Chat.ChatType.SingleChat){
                        uc.modules.chat.SingleChat.onBodyClick(e);
                    }
                }
            );
        },

        _showNoChatContent: function(){
            this.currentChatType = uc.constants.Chat.ChatType.None;
            uc.ui.Message.ContentPanelBody.find('*').off();
            uc.ui.Message.ContentPanelBody.empty();
            uc.ui.Message.ContentPanelBody.append('<div class="init-panel">\
                                                    <div class="init-panel-none">\
                                                        <img src="' + rootPath + '/images/chat/noneChat.png">\
                                                        <div class="init-panel-noneText">' + uc_resource.Main.ChatMessage + '</div>\
                                                    </div>\
                                                </div>');
        },

        _clear: function(){
            this.chatMsg =true;
            if(this.currentChatType == uc.constants.Chat.ChatType.None){
                uc.ui.Message.ContentPanelBody.empty();
            }
            switch (this.currentChatType) {
                case uc.constants.Chat.ChatType.SingleChat:
                    uc.modules.chat.SingleChat.clear();
                    break;
                case uc.constants.Chat.ChatType.GroupChat:
                    uc.modules.chat.GroupChat.clear();
                    break;
                case uc.constants.Chat.ChatType.ProjectChat:
                    uc.modules.chat.ProjectChat.clear();
                    break;
                case uc.constants.Chat.ChatType.CloudChat:
                    uc.modules.chat.CloudChat.clear();
                    break;
                case uc.constants.Chat.ChatType.MeetingChat:
                    uc.modules.chat.MeetingChat.clear();
                    break;
                case uc.constants.Chat.ChatType.OpenApiChat:
                    uc.modules.chat.OpenApiChat.clear();
                    break;
            }
        },

        OnOpenChatWindow: function (chatType, chatId, openChatWindowSourceModule, meetingData, siteId, isFromCall, paramObj) {
            if( uc.modules.notice.NoticeManager.isNoticeConversation(chatId)){
              uc.IUIService.openNoticeReceiverWindow();
              this._clear();
              return;
            }
            if (chatType == uc.constants.Chat.ChatType.None) {
                uc.util.LogUtil.info('ChatManager', 'clearChatWidnow', "" + (!!chatId?chatId:""), null);
                this._showNoChatContent();
                return;
            }
            //if (this.currentChatType == chatType &&
            //    this.currentChatId == chatId) {
            //    return;
            //}
            if (chatId <= 0) {
                return;
            }

            this._clear();

            try {
                uc.IClientService.getSessionId(function (token){
                    uc.modules.chat.ChatManager.token = token[1];
                });
            } catch (e) {
                uc.util.LogUtil.info('ChatManager', 'getSessionId', "" + e.message, null);
            }

            if (this.myUserInfo == null) {
                this.myUserInfo = uc.IContactService.getCurrentUserInfo();
            }
            this.initComponents();
            this.currentChatType = chatType;
            this.currentChatId = chatId;
            this.groupInfo = null;
            if(this.msgBox){
                this.msgBox.setApiInfo(null);
            }

            var groupInfo;
            switch (chatType) {
                case uc.constants.Chat.ChatType.SingleChat:
                    var capability = uc.util.StorageUtil.getData('capability');
                    if (capability) {
                        capability = JSON.parse(capability);
                    }
                    uc.modules.chat.SingleChat.init(chatId, siteId, capability, paramObj);
                    if(isFromCall){
                        uc.modules.chat.SingleChat.startCall();
                    }
                    if (paramObj && paramObj.msgTime && paramObj.keyword) {
                        uc.modules.historymsg.HistoryManager.GeneralQueryMessageBykeyWord(paramObj.msgTime, paramObj.keyword);
                    }
                    break;
                case uc.constants.Chat.ChatType.GroupChat:
                    uc.modules.chat.GroupChat.init(chatId, uc.constants.Chat.ChatType.GroupChat, siteId, paramObj);
                    groupInfo = uc.IGroupService.getGroupInfo(chatId);
                    if (groupInfo != null) {
                        this.groupInfo = groupInfo;
                        uc.modules.chat.GroupChat.updateChatName(groupInfo);
                        uc.modules.chat.GroupChat.updateGroupMember(groupInfo.members);
                        if (groupInfo.joinState == 1 || groupInfo.status == 1) {
                            uc.modules.chat.GroupChat.enableInput(false);
                        }
                    }
                    if(isFromCall){
                        uc.modules.chat.GroupChat.startCallFromProfile();
                    }

                    if (paramObj && paramObj.msgTime && paramObj.keyword) {
                        uc.modules.historymsg.HistoryManager.GeneralQueryMessageBykeyWord(paramObj.msgTime, paramObj.keyword);
                    }
                    break;
                case uc.constants.Chat.ChatType.ProjectChat:
                    this.groupInfo = groupInfo;
                    uc.modules.chat.ProjectChat.init(chatId, uc.constants.Chat.ChatType.ProjectChat, siteId, paramObj);
                    groupInfo = uc.IGroupService.getGroupInfo(chatId);
                    if (groupInfo != null) {
                        this.groupInfo = groupInfo;
                        uc.modules.chat.ProjectChat.updateChatName(groupInfo);
                        uc.modules.chat.ProjectChat.updateGroupMember(groupInfo.members);
                        if (groupInfo.joinState == 1 || groupInfo.status == 1) {
                            uc.modules.chat.ProjectChat.enableInput(false);
                        }
                    }
                    if(isFromCall){
                        uc.modules.chat.GroupChat.startCallFromProfile();
                    }

                    if (paramObj && paramObj.msgTime && paramObj.keyword) {
                        uc.modules.historymsg.HistoryManager.GeneralQueryMessageBykeyWord(paramObj.msgTime, paramObj.keyword);
                    }
                    break;
                case uc.constants.Chat.ChatType.CloudChat:
                    uc.modules.chat.CloudChat.init(chatId, uc.constants.Chat.ChatType.CloudChat, siteId);
                    groupInfo = uc.IGroupService.getGroupInfo(chatId);
                    if (groupInfo != null) {
                        this.groupInfo = groupInfo;
                        uc.modules.chat.CloudChat.updateChatName(groupInfo);
                        uc.modules.chat.CloudChat.updateGroupMember(groupInfo.members);
                        if (groupInfo.joinState == 1 || groupInfo.status == 1) {
                            uc.modules.chat.CloudChat.enableInput(false);
                        }
                    }

                    if (paramObj && paramObj.msgTime && paramObj.keyword) {
                        uc.modules.historymsg.HistoryManager.GeneralQueryMessageBykeyWord(paramObj.msgTime, paramObj.keyword);
                    }
                    break;
                case uc.constants.Chat.ChatType.MeetingChat:
//                    if (this.meetingPanel) {
//                        this.meetingPanel = uc.modules.meeting.MeetingManager.createMeetingPanel(uc.ui.Message.ContentPanelBody, uc.constants.UI.ModuleId.Message);
//                    }
                    //this.meetingPanel = uc.modules.meeting.MeetingManager.createMeetingPanel(uc.ui.Message.ContentPanelBody, uc.constants.UI.ModuleId.Message);
                    //uc.modules.meeting.MeetingManager.showMeetingPanel(meetingData.meetingId, meetingData.eventId, meetingData.startTime, meetingData.isCancelled == 1,meetingData.meetingMsgType);
                    uc.modules.chat.MeetingChat.init(chatId, meetingData);
                    this.meetingChat = uc.modules.chat.MeetingChat;
                    break;

                case uc.constants.Chat.ChatType.OpenApiChat:
                    var appInfo = uc.modules.openApi.OpenApiManager.getOpenApiInfo(chatId);
                    if (!appInfo) {
                        appInfo = uc.modules.conversation.ConversationManager.closeAppList.getOpenApiInfo(chatId)
                    }
                    uc.util.LogUtil.info('chat', 'ChatManager', 'appInfo', appInfo);
                    if(this.msgBox){
                        this.msgBox.setApiInfo(appInfo);
                    }

                    uc.modules.chat.OpenApiChat.init(chatId, appInfo);
                    break;
            }
            if(this.msgBox && this.moduleId == uc.constants.UI.ModuleId.Message){
                this.msgBox.setChatIdAndType(this.currentChatId, this.currentChatType);
            }
            if(this.currentChatType == uc.constants.Chat.ChatType.SingleChat
                && uc.modules.remoteAssistance.RemoteAssistanceManager.isInvitedToRemoteAssistance(this.currentChatId)){
                var info = uc.modules.remoteAssistance.RemoteAssistanceManager.getRemoteAssistanceChatInfo(this.currentChatId);
                uc.modules.remoteAssistance.RemoteAssistanceBanner.init(info);
            }
        },

        isSendByMyself: function (from) {
            if (from.userId != this.myUserInfo.userId) {
                var contactInfo = uc.modules.cache.BasicContactInfoCache.getBasicContactInfo(from.userId);
                if (contactInfo) {
                    from.displayName = contactInfo.displayName;
                    from.localAvatar = contactInfo.localAvatar;
                    from.remoteAvatarUrl = contactInfo.remoteAvatarUrl;
                    if (from.localAvatar == null || from.localAvatar == "") {
                        //如果是超级群，不下载图像
                        if (this.currentChatType == uc.constants.Chat.ChatType.GroupChat || this.currentChatType == uc.constants.Chat.ChatType.ProjectChat && (this.groupInfo && this.groupInfo.memberType)) {
                            return;
                        }
                        from.localAvatar = this.getUserLogo(from.userId, from.remoteAvatarUrl);
                    }
                }
            }
            return from.userId == this.myUserInfo.userId;
        },

        procBeforeSend: function(sender, args, msgBox, chatType, chatId) {
            var chatManager = uc.modules.chat.ChatManager;
            var dom, taskId, msgId, info;

            var currentUserInfo = uc.IContactService.getCurrentUserInfo();
            var from = {
                userId: chatManager.myUserInfo.userId,
                displayName: currentUserInfo.displayName
            };
            switch (args.messageType) {
                case uc.constants.Message.ContentType.Text:
                {
                    dom = msgBox.getTextMsgDomStr(uc.constants.Message.SendState.Sending, true, from,
                        args.msgId, new Date().getTime(), null, args.content, new Date().getTime(), new Date().getTime(), args.textFormat);
                    msgBox.appendMessage(dom);
                    msgBox.scrollToEnd();
                } break;
                case uc.constants.Message.ContentType.Image:
                {
                    taskId = args.taskId;
                    msgId = "upload_" + taskId;
                    info = {
                        msgId: msgId,
                        chatType: chatManager.currentChatType,
                        chatId: chatManager.currentChatId,
                        percent: 0,
                        filePath: args.filePath,
                        args: args
                    };
                    if (chatType != null && chatId != null) {
                        info.chatType = chatType;
                        info.chatId = chatId;
                    }
                    chatManager.fileUpLoadInfo["" + taskId] = info;
                    dom = msgBox.getImgMsgDomStr(uc.constants.Message.SendState.SendOK, true, from,
                        msgId, new Date().getTime(), args.fileName, args.fileSize, "", "", new Date().getTime());
                    msgBox.appendMessage(dom);
                    msgBox.scrollToEnd();
                } break;
                case uc.constants.Message.ContentType.File:
                {
                    taskId = args.taskId;
                    msgId = "upload_" + taskId;
                    info = {
                        msgId: msgId,
                        chatType: chatManager.currentChatType,
                        chatId: chatManager.currentChatId,
                        percent: 0,
                        filePath: args.filePath,
                        args: args
                    };
                    if (chatType != null && chatId != null) {
                        info.chatType = chatType;
                        info.chatId = chatId;
                    }
                    chatManager.fileUpLoadInfo["" + taskId] = info;
                    dom = msgBox.getFileMsgDomStr(uc.constants.Message.SendState.SendOK, true, from,
                        msgId, new Date().getTime(), args.fileName, args.fileSize, "", new Date().getTime(), null, this.myUserInfo.displayName, chatManager.currentChatType);
                    msgBox.appendMessage(dom);
                    msgBox.scrollToEnd();

                    if (sender != null) {
                        chatManager.noticeFileUploading(0, taskId, 0);
                    }
                } break;
                default:
                    break;
            }
        },

        onBeforeSend: function (sender, args) {
            if (uc.modules.chat.ChatManager.msgBox != null) {
                uc.modules.chat.ChatManager.procBeforeSend(sender, args, uc.modules.chat.ChatManager.msgBox);
            }
        },
        onMeetingDetailBeforeSend: function (sender, args, chatType, chatId) {
            uc.modules.chat.ChatManager.procBeforeSend(sender, args, this.meetDetailChat.getMsgBox(), chatType, chatId);
        },
        onAt: function(x, y, id) {
            if (uc.modules.chat.ChatManager.currentChatType == uc.constants.Chat.ChatType.GroupChat) {
                uc.modules.chat.GroupChat.onAt(x, y, id);
            } else if  (uc.modules.chat.ChatManager.currentChatType == uc.constants.Chat.ChatType.ProjectChat){
                uc.modules.chat.ProjectChat.onAt(x, y, id);
            }else if  (uc.modules.chat.ChatManager.currentChatType == uc.constants.Chat.ChatType.CloudChat){
                uc.modules.chat.CloudChat.onAt(x, y, id);
            }
        },
        onAtsearch: function(id, text) {
            if (uc.modules.chat.ChatManager.currentChatType == uc.constants.Chat.ChatType.GroupChat) {
                return uc.modules.chat.GroupChat.onAtsearch(id, text);
            } else if  (uc.modules.chat.ChatManager.currentChatType == uc.constants.Chat.ChatType.ProjectChat){
                return uc.modules.chat.ProjectChat.onAtsearch(id, text);
            }else if  (uc.modules.chat.ChatManager.currentChatType == uc.constants.Chat.ChatType.CloudChat){
                return uc.modules.chat.CloudChat.onAtsearch(id, text);
            }
        },
        appendUploadMessages: function () {
            for (var key in this.fileUpLoadInfo) {
                var info = this.fileUpLoadInfo[key];
                if (info.chatType == this.currentChatType &&
                    info.chatId == this.currentChatId && this.moduleId != uc.constants.UI.ModuleId.Meeting) {
                    this.onBeforeSend(null, info.args);
                    this.noticeFileUploading(0, info.args.taskId, info.percent);
                }

                if (this.meetDetailChat && info.chatType == uc.constants.Chat.ChatType.MeetingChat &&
                    info.chatId == this.meetDetailChat.groupId && this.moduleId == uc.constants.UI.ModuleId.Meeting) {
                    this.onMeetingDetailBeforeSend(null, info.args, uc.constants.Chat.ChatType.MeetingChat, this.meetDetailChat.groupId);
                    //this.noticeFileUploading(0, info.args.taskId, info.percent);
                }
            }
        },

        OnMessageSent: function (errorCode, msgId, msgSeq) {
            if (this.msgBox == null) {
                return;
            }
            if (errorCode == 0) {
                var tstr = uc.util.TimeUtil.getMessageTimeString(new Date(msgSeq), null, uc.settings.meeting.timeZone.offset);
                this.msgBox.updateMsgTime(msgId, tstr);
                this.msgBox.showSendFailFlag(msgId, false);
                this.msgBox.updateMessageSeq(msgId,msgSeq);
                return;
            }

            this.msgBox.showSendFailFlag(msgId, true);
        },
        noticeMessageSent: function (errorCode, userId, msgId, msgSeq) {
            if (this.msgBox != null) {
                this.OnImageMessageSend(errorCode, msgId, msgSeq, this.msgBox);
            }
            if (this.currentChatType != uc.constants.Chat.ChatType.SingleChat &&
                this.currentChatId != userId) {
                return;
            }
            this.OnMessageSent(errorCode, msgId, msgSeq);
        },
        noticeGroupMessageSent: function (errorCode, conversationType, groupId, msgId, msgSeq) {
            if (this.msgBox != null) {
                this.OnImageMessageSend(errorCode, msgId, msgSeq, this.msgBox);
            }
            if ((this.currentChatType != uc.constants.Chat.ChatType.GroupChat && this.currentChatType != uc.constants.Chat.ChatType.ProjectChat && this.currentChatType != uc.constants.Chat.ChatType.CloudChat)
                || groupId != this.currentChatId) {
                return;
            }
            this.OnMessageSent(errorCode, msgId, msgSeq);
        },
        OnOpenApiMessageSent:function(errorCode, appId, msgId, msgSeq, msgTime){
            if (this.msgBox != null) {
                this.OnImageMessageSend(errorCode, msgId, msgSeq, this.msgBox);
            }
            if (this.currentChatType != this.currentChatType == uc.constants.Chat.ChatType.OpenApiChat  || appId != this.currentChatId) {
                return;
            }
            this.OnMessageSent(errorCode, msgId, msgSeq);
        },
        noticeDocumentUploaded: function(errorCode, groupId, operatorId, document, msgId){//group
            if(this.currentChatType != uc.constants.Chat.ChatType.MeetingChat){
                if (this.currentChatType == uc.constants.Chat.ChatType.SingleChat || groupId != this.currentChatId) {
                    return;
                }
            }
            this.OnMessageSent(errorCode, msgId, document.messageSeq);
            //if (errorCode == 0 && this.msgBox != null && this.currentChatType == uc.constants.Chat.ChatType.ProjectChat) {    讨论组展示聊天文件
            if (errorCode == 0 && this.msgBox != null){
                this.msgBox.onFileIdUpdate(msgId, document.fileId, document.uploadTime, document.fileUrl,this.currentChatType);
            }
        },
        noticeMeetingDocumentUploaded: function(errorCode, eventId, operatorId, document, msgId){//group
            if(this.currentChatType == uc.constants.Chat.ChatType.MeetingChat){
                this.OnMessageSent(errorCode, msgId, document.messageSeq);
                //if (errorCode == 0 && this.msgBox != null && this.currentChatType == uc.constants.Chat.ChatType.ProjectChat) {    讨论组展示聊天文件
                if (errorCode == 0 && this.msgBox != null){
                    this.msgBox.onFileIdUpdate(msgId, document.fileId, document.messageSeq, document.fileUrl,this.currentChatType);
                }
            }

            // 给MeetingChat更新seq
            if(this.meetDetailChat){
                this.meetDetailChat.noticeMeetingDocumentUploaded(errorCode, eventId, operatorId, document, msgId);
            }

        },
        noticeCodeSnippetUploaded: function(errorCode, groupId, operatorId, document, msgId) {
            if (this.currentChatType == uc.constants.Chat.ChatType.SingleChat || groupId != this.currentChatId) {
                return;
            }
            this.OnMessageSent(errorCode, msgId, document.uploadTime);
            //if (errorCode == 0 && this.msgBox != null && this.currentChatType == uc.constants.Chat.ChatType.ProjectChat) {
            if (errorCode == 0 && this.msgBox != null){
                this.msgBox.onFileIdUpdate(msgId, document.fileId, document.uploadTime, document.fileUrl);
            }
        },

        notifyContactInfoReceived: function (errorCode, userIds, contacts) {
            if (errorCode == 0) {
                this.updateChatContactInfo(contacts);
            }
        },
        notifyContactUpdated: function (contacts) {
            this.updateChatContactInfo(contacts);
        },
        notifyBasicContactInfoReceived: function (errorCode, userIds, contacts) {
            if (errorCode == 0) {
                this.updateChatContactInfo(contacts);
            }
        },

        isCurrentMsg: function (chatType, from, to) {
            if (this.currentChatId != from.userId && this.currentChatId != to.userId) {
                return false;
            }
            if (this.currentChatType != chatType && this.moduleId != uc.constants.UI.ModuleId.Meeting) {
                return false;
            }
            return true;
        },
        noticeChatHistoryReceived: function (errorCode, userId, messages, msgCount, minTimestamp) {
            if (this.currentChatType != uc.constants.Chat.ChatType.SingleChat &&
                this.currentChatId != userId) {
                return;
            }
            uc.modules.chat.SingleChat.onChatHistoryReceived(errorCode, userId, messages, msgCount, minTimestamp);
        },
        noticeTextMessageReceived: function (channel, sendState, from, to, msgId, msgSeq, textType, textData, sendTime, timestamp, textFormat) {
            if (!this.isCurrentMsg(uc.constants.Chat.ChatType.SingleChat, from, to)) {
                return;
            }
            uc.modules.chat.SingleChat.onTextMessageReceived(channel, sendState, this.isSendByMyself(from), from, msgId, msgSeq, textType, textData, sendTime, timestamp, textFormat);
        },
        noticeImageMessageReceived: function (channel, sendState, from, to, msgId, msgSeq, imageName, imageSize, imageURL, imageData, timestamp) {
            if (!this.isCurrentMsg(uc.constants.Chat.ChatType.SingleChat, from, to)) {
                return;
            }
            uc.modules.chat.SingleChat.onImageMessageReceived(channel, sendState, this.isSendByMyself(from), from, msgId, msgSeq, imageName, imageSize, imageURL, imageData, timestamp);
        },
        noticeAudioMessageReceived: function (channel, sendState, from, to, msgId, msgSeq, audioName, audioSize, audioURL, audioDuration, timestamp) {
            if (!this.isCurrentMsg(uc.constants.Chat.ChatType.SingleChat, from, to)) {
                return;
            }
            uc.modules.chat.SingleChat.onAudioMessageReceived(channel, sendState, this.isSendByMyself(from), from, msgId, msgSeq, audioName, audioSize, audioURL, audioDuration, timestamp);
        },
        noticeVideoMessageReceived: function (channel, sendState, from, to, msgId, msgSeq, videoName, videoSize, videoURL, videoThumb, duration, timestamp) {
            if (!this.isCurrentMsg(uc.constants.Chat.ChatType.SingleChat, from, to)) {
                return;
            }
            uc.modules.chat.SingleChat.onVideoMessageReceived(channel, sendState, this.isSendByMyself(from), from, msgId, msgSeq, videoName, videoSize, videoURL, videoThumb, duration, timestamp);
        },
        noticeIMReportMessageReceived: function (channel, sendState, from, to, eventId, msgId, msgSeq, msgTime, reportTitle, reportUrl) {
            if (!this.isCurrentMsg(uc.constants.Chat.ChatType.SingleChat, from, to)) {
                return;
            }
            uc.modules.chat.SingleChat.OnIMReportMessageReceived(channel, sendState, from, to, eventId, msgId, msgSeq, msgTime, reportTitle, reportUrl);
        },
        noticeFileReceived: function (channel, sendState, from, to, msgId, msgSeq, fileName, fileSize, fileURL, timestamp,localPath) {
            if (!this.isCurrentMsg(uc.constants.Chat.ChatType.SingleChat, from, to)) {
                return;
            }
            uc.modules.chat.SingleChat.onFileReceived(channel, sendState, this.isSendByMyself(from), from, msgId, msgSeq, fileName, fileSize, fileURL, timestamp,localPath);
        },
        noticeFileShared: function (from, to, msgId, msgSeq, fileName, fileSize, fileURL, fileId) {
            if (!this.isCurrentMsg(uc.constants.Chat.ChatType.SingleChat, from, to)) {
                return;
            }
            uc.modules.chat.SingleChat.onFileShared(this.isSendByMyself(from), from, msgId, msgSeq, fileName, fileSize, fileURL, fileId);
        },
        noticeOnFileDeleted: function (from, to, msgId, msgSeq, fileName, fileId) {
            if (!this.isCurrentMsg(uc.constants.Chat.ChatType.SingleChat, from, to)) {
                return;
            }
            uc.modules.chat.SingleChat.onFileDeleted(this.isSendByMyself(from), from, msgId, msgSeq, fileName, fileId);
        },
        noticeCodeReceived: function(channel, sendState, from, to, msgId, msgSeq, creatorId, creatorName,
                                     codeId, codeTitle, langType, codeHead, codeDesc, codeSize, codeUrl, codeLine, createTime, timestamp){
            if (!this.isCurrentMsg(uc.constants.Chat.ChatType.SingleChat, from, to)) {
                return;
            }
            uc.modules.chat.SingleChat.OnCodeReceived(channel, sendState, this.isSendByMyself(from), from, msgId, msgSeq, creatorId,
                creatorName, codeId, codeTitle, langType, codeHead, codeDesc, codeSize, codeUrl, codeLine, createTime, timestamp);
        },
        noticeCloudFileReceived: function (channel, sendState, from, to, msgId, msgSeq, fileName, fileSize, timestamp,detailContent) {
            if (!this.isCurrentMsg(uc.constants.Chat.ChatType.SingleChat, from, to)) {
                return;
            }
            uc.modules.chat.SingleChat.onCloudFileReceived(channel, sendState, this.isSendByMyself(from), from, msgId, msgSeq, fileName, fileSize, timestamp,detailContent);
        },

        OnP2PCallInvitationReceived: function(channel, userId,fromUserId, confId, confPwd, hostId, msgId, msgSeq, timestamp){
            if(this.currentChatId == fromUserId && this.currentChatType == uc.constants.Chat.ChatType.SingleChat){
                uc.modules.chat.SingleChat.OnP2PCallInvitationReceived(channel, userId,fromUserId, confId, confPwd, hostId, msgId, msgSeq, timestamp);
            }
        },
        OnP2PCallInvitationAccepted: function(channel, userId,fromUserId, confId, msgId, msgSeq, timestamp){
            if(this.currentChatId == fromUserId && this.currentChatType == uc.constants.Chat.ChatType.SingleChat){
                uc.modules.chat.SingleChat.OnP2PCallInvitationAccepted(channel, userId,fromUserId, confId, msgId, msgSeq, timestamp);
            }
        },
        OnP2PCallInvitationRejected: function(channel, userId,fromUserId, confId, hostId, reason, msgId, msgSeq, timestamp,sendState){
            if(this.currentChatId == userId && this.currentChatType == uc.constants.Chat.ChatType.SingleChat){
                var form={userId:fromUserId};
                uc.modules.chat.SingleChat.OnP2PCallInvitationRejected(channel,this.isSendByMyself(form),userId, form, confId, hostId, reason, msgId, msgSeq, timestamp,sendState);
            }
        },
        OnP2PCallStopped: function(channel, userId, confId, msgId, msgSeq, timestamp){
            if(this.currentChatId == userId && this.currentChatType == uc.constants.Chat.ChatType.SingleChat){
                uc.modules.chat.SingleChat.OnP2PCallStopped(channel, userId, confId, msgId, msgSeq, timestamp);
            }
        },
        OnP2PCallCancelled: function(channel, userId,fromUserId, msgId, msgSeq, timestamp){
            if(this.currentChatId == fromUserId && this.currentChatType == uc.constants.Chat.ChatType.SingleChat){
                uc.modules.chat.SingleChat.OnP2PCallCancelled(channel, userId,fromUserId, msgId, msgSeq, timestamp);
            }
        },
        OnP2PCallNoResponse: function(channel, userId, msgId, msgSeq, timestamp){
            if(this.currentChatId == fromUserId && this.currentChatType == uc.constants.Chat.ChatType.SingleChat){
                uc.modules.chat.SingleChat.OnP2PCallNoResponse(channel, userId, msgId, msgSeq, timestamp);
            }
        },
        OnP2PCallDurationReceived: function(channel, userId,fromUserId, duration, msgId, msgSeq, timestamp,sendState){
            if(this.currentChatId == userId && this.currentChatType == uc.constants.Chat.ChatType.SingleChat){
                var form={userId:fromUserId};
                uc.modules.chat.SingleChat.OnP2PCallDurationReceived(channel, this.isSendByMyself(form),form, duration, msgId, msgSeq, timestamp,sendState);
            }
            if (channel == uc.constants.Message.MessageChannel.UCAS){
                uc.IUIEvent.OnSaveBeeCallStatusToPbx({
                    "userID":userId,
                    "state":uc.constants.Pbx.CallStatusType.CALL_STATE_CONFIRMED,
                    "druation":duration,
                    "time":timestamp
                });
            }
        },


        //Group
        noticeGroupChatHistoryReceived: function (errorCode, conversationType, groupId, messages, msgCount, minTimestamp) {
            if(!this.chatMsg)
            {
                return;//differ  from others. Search histroy EventListener
            }
            if(conversationType==uc.constants.Chat.ChatType.MeetingChat){
                this.OnMeetingChatHistoryReceived(errorCode, groupId, msgCount, minTimestamp);
            }else{
                if (this.currentChatType && this.currentChatType != uc.constants.Chat.ChatType.GroupChat && this.currentChatType != uc.constants.Chat.ChatType.ProjectChat && this.currentChatType != uc.constants.Chat.ChatType.CloudChat){
                    return;
                }

                if( groupId != this.currentChatId){
                    return;
                }
                uc.modules.chat.GroupChat.onGroupChatHistoryReceived(errorCode, conversationType, groupId, messages, msgCount, minTimestamp);
            }
        },

        noticeGroupTextMessageReceived: function (conversationType, channel, sendState, from, to, msgId, msgSeq, textType, textContent, msgTime, textFormat,atUsers) {
            if(atUsers && atUsers.length){
                var userInfo = uc.IContactService.getCurrentUserInfo();
                var currentUserId = userInfo.userId;
                var atCounts = false;
                var counts = 0;
                for(var i = 0; i < atUsers.length; i++){
                    if(atUsers[i].userId == currentUserId) {
                        atCounts = true;
                        counts++;
                    }
                }
                var conversationInfo = uc.modules.conversation.ConversationManager.getConversation(to.userId, conversationType);
                if(atCounts){
                    conversationInfo.atCounts += counts;
                    uc.modules.conversation.ConversationManager.updateConversation(to.userId, conversationType, conversationInfo);
                }
            }
            if (this.isCurrentMsg(conversationType, from, to)) {
                uc.modules.chat.GroupChat.onGroupTextMessageReceived(conversationType, channel, sendState, this.isSendByMyself(from), from, to, msgId, msgSeq, textType, textContent, msgTime, textFormat, atUsers);
            }
        },
        noticeGroupImageMessageReceived: function (conversationType, channel, sendState, from, to, msgId, msgSeq, fileName, fileSize, fileURL, thumbnails, msgTime) {
            if (this.isCurrentMsg(conversationType, from, to)) {
                uc.modules.chat.GroupChat.onGroupImageMessageReceived(conversationType, channel, sendState, this.isSendByMyself(from), from,
                    msgId, msgSeq, fileName, fileSize, fileURL, thumbnails, msgTime);
            }
        },
        noticeGroupAudioMessageReceived: function (conversationType, channel, sendState, from, to, msgId, msgSeq, fileName, fileSize, fileURL, duration, msgTime) {
            if (this.isCurrentMsg(conversationType, from, to)) {
                uc.modules.chat.GroupChat.onGroupAudioMessageReceived(conversationType, channel, sendState, this.isSendByMyself(from), from,
                    msgId, msgSeq, fileName, fileSize, fileURL, duration, msgTime);
            }
        },
        noticeGroupVideoMessageReceived: function (conversationType, channel, sendState, from, to, msgId, msgSeq, fileName, fileSize, fileURL, thumbnails, duration, msgTime) {
            if (this.isCurrentMsg(conversationType, from, to)) {
                uc.modules.chat.GroupChat.onGroupVideoMessageReceived(conversationType, channel, sendState, this.isSendByMyself(from), from,
                    msgId, msgSeq, fileName, fileSize, fileURL, thumbnails, duration, msgTime);
            }
        },
        noticeGroupReportMessageReceived: function (conversationType, channel, sendState, from, to, eventId, msgId, msgSeq, msgTime, reportTitle, reportUrl) {
            if (!this.isCurrentMsg(conversationType, from, to)) {
                return;
            }
            uc.modules.chat.GroupChat.OnGroupReportMessageReceived(conversationType, channel, sendState, from, to, eventId, msgId, msgSeq, msgTime, reportTitle, reportUrl);
        },
        noticeGroupCreatedMessageReceived: function (conversationType, channel, from, to, operatorId, groupId, groupName, groupAvatar, members, msgId, msgSeq, msgTime) {
            if (this.isCurrentMsg(conversationType, from, to)) {
                uc.modules.chat.GroupChat.OnGroupCreatedMessageReceived(conversationType, channel, from, to, operatorId, groupId, groupName, groupAvatar, members, msgId, msgSeq, msgTime);
            }
        },
        noticeGroupMemberExitedMessageReceived: function (conversationType, channel, from, to, operatorId, memberId, groupId, groupName, groupAvatar, msgId, msgSeq, msgTime) {
            if (this.isCurrentMsg(conversationType, from, to)) {
                uc.modules.chat.GroupChat.OnGroupMemberExitedMessageReceived(conversationType, channel, from, to, operatorId, memberId, groupId, groupName, groupAvatar, msgId, msgSeq, msgTime);
            }
        },
        noticeGroupEndMessageReceived: function (conversationType, channel, from, to, operatorId, msgId, msgSeq, msgTime) {
            if (this.isCurrentMsg(conversationType, from, to)) {
                uc.modules.chat.GroupChat.OnGroupEndMessageReceived(conversationType, channel, from, to, operatorId, msgId, msgSeq, msgTime);
            }
        },
        noticeGroupMemberAddedMessageReceived: function (conversationType, channel, from, to, operatorId, groupId, groupName, groupAvatar, members, msgId, msgSeq, msgTime) {
            if (this.isCurrentMsg(conversationType, from, to)) {
                uc.modules.chat.GroupChat.OnGroupMemberAddedMessageReceived(conversationType, channel, from, to, operatorId, groupId, groupName, groupAvatar, members, msgId, msgSeq, msgTime);
            }
        },
        noticeGroupMemberKickedMessageReceived: function (conversationType, channel, from, to, operatorId, groupId, groupName, groupAvatar, members, msgId, msgSeq, msgTime) {
            if (this.isCurrentMsg(conversationType, from, to)) {
                uc.modules.chat.GroupChat.GroupMemberKickedMessageReceived(conversationType, channel, from, to, operatorId, groupId, groupName, groupAvatar, members, msgId, msgSeq, msgTime);
            }
        },
        noticeGroupNameUpdatedMessageReceived: function (conversationType, channel, from, to, operatorId, groupId, groupName, namePinYin, msgId, msgSeq, msgTime) {
            if (this.isCurrentMsg(conversationType, from, to)) {
                uc.modules.chat.GroupChat.OnGroupNameUpdatedMessageReceived(conversationType, channel, from, to, operatorId, groupId, groupName, namePinYin, msgId, msgSeq, msgTime);
            }
        },
        noticeGroupAvatarUpdatedMessageReceived: function (conversationType, channel, from, to, operatorId, groupId, groupAvatar, msgId, msgSeq, msgTime) {
            if (this.isCurrentMsg(conversationType, from, to)) {
                uc.modules.chat.GroupChat.OnGroupAvatarUpdatedMessageReceived(conversationType, channel, from, to, operatorId, groupId, groupAvatar, msgId, msgSeq, msgTime);
            }
        },
        noticeGroupDisturbSetMessageReceived: function (conversationType, channel, from, to, groupId, state, msgId, msgSeq, msgTime) {
            if (this.isCurrentMsg(conversationType, from, to)) {
                uc.modules.chat.GroupChat.OnGroupDisturbSetMessageReceived(conversationType, channel, from, to, groupId, state, msgId, msgSeq, msgTime);
            }
        },
        noticeGroupOnTopMessageReceived: function (conversationType, channel, from, to, operatorId, groupId, isTop, msgId, msgSeq, msgTime) {
            if (this.isCurrentMsg(conversationType, from, to)) {
                uc.modules.chat.GroupChat.OnGroupOnTopMessageReceived(conversationType, channel, from, to, operatorId, groupId, isTop, msgId, msgSeq, msgTime);
            }
        },
        noticeDocumentUploadMessageReceived: function (conversationType, channel, sendState, from, to, operatorId, fileId, fileName, fileSize, downloadUrl,
                                                       fileFormat, desc, msgId, msgSeq, msgTime, username, localPath) {
            if (this.isCurrentMsg(conversationType, from, to)) {
                uc.modules.chat.GroupChat.OnDocumentUploadMessageReceived(conversationType, channel, sendState, this.isSendByMyself(from), from,
                    operatorId, fileId, fileName, fileSize, downloadUrl, fileFormat, desc, msgId, msgSeq, msgTime, username, localPath);
            }
        },
        noticeCodeSnippetUploadMessageReceived:function(conversationType, channel, sendState, from, to, msgId, msgSeq, creatorId, creatorName, codeId, codeTitle,
                                                        langType, codeHead, codeDesc, codeSize, codeUrl, codeLine, createTime, timestamp){
            if (this.isCurrentMsg(conversationType, from, to)) {
                uc.modules.chat.GroupChat.OnCodeSnippetUploadMessageReceived(conversationType, channel, sendState, this.isSendByMyself(from), from,
                    msgId, msgSeq, creatorId, creatorName, codeId, codeTitle, langType, codeHead, codeDesc, codeSize, codeUrl, codeLine, createTime, timestamp);
            }
        },

        noticeDocumentShareMessageReceived: function (conversationType, channel, from, to, operatorId, fileId, fileName, fileSize, downloadUrl, fileFormat, desc, msgId, msgSeq, msgTime, username) {
            if (this.isCurrentMsg(conversationType, from, to)) {
                uc.modules.chat.GroupChat.OnDocumentUploadMessageReceived(conversationType, channel, uc.constants.Message.SendState.SendOK, this.isSendByMyself(from), from,
                    operatorId, fileId, fileName, fileSize, downloadUrl, fileFormat, desc, msgId, msgSeq, msgTime, username,null);
            }
        },
        noticeDocumentDeleteMessageReceived: function (conversationType, channel, from, to, operatorId, fileId, fileName, msgId, msgSeq, msgTime) {
            if (this.isCurrentMsg(conversationType, from, to)) {
                uc.modules.chat.GroupChat.OnDocumentDeleteMessageReceived(conversationType, channel, from, to, operatorId, fileId, fileName, msgId, msgSeq, msgTime);
            }
        },

        noticeGroupCreated: function (errorCode, operatorId, group) {
            uc.IUIService.openChatWindow(group.groupType, group.id, uc.constants.UI.OpenChatWindowSourceModule.Other, null, null, true);
        },
        noticeGroupExited: function (errorCode, channel, groupId, userId) {

        },
        noticeGroupEnded: function (errorCode, channel, groupId, operatorId) {

        },
        noticeMemberAdded: function (errorCode, channel, groupId, operatorId, memberIds) {

        },
        noticeMemberRemoved: function (errorCode, channel, groupId, operatorId, memberIds) {

        },
        noticeGroupNameUpdated: function (errorCode, channel, groupId, operatorId, groupName, pinyin) {
            if ((this.currentChatType == uc.constants.Chat.ChatType.GroupChat
                || this.currentChatType == uc.constants.Chat.ChatType.ProjectChat || this.currentChatType == uc.constants.Chat.ChatType.CloudChat) &&
                groupId == this.currentChatId){
                uc.modules.chat.GroupChat.OnGroupNameUpdated(errorCode, channel, groupId, operatorId, groupName, pinyin);
            }
        },
        noticeGroupLogoUpdated: function (errorCode, channel, groupId, logoURL) {

        },
        noticeGroupInfoReceived: function (errorCode, groupId, group) {
            if (this.currentChatType == uc.constants.Chat.ChatType.SingleChat || groupId != this.currentChatId) {
                return;
            }
            if (group == null || group.name == null) {
                return;
            }

            this.groupInfo = group;
            if (this.currentChatType == uc.constants.Chat.ChatType.GroupChat) {
                uc.modules.chat.GroupChat.updateChatName(group);
                uc.modules.chat.GroupChat.updateGroupMember(group.members);
                if (group.joinState == 1 || group.status == 1) {
                    uc.modules.chat.GroupChat.enableInput(false);
                }
            } else if (this.currentChatType == uc.constants.Chat.ChatType.ProjectChat || this.currentChatType == uc.constants.Chat.ChatType.CloudChat) {
                uc.modules.chat.ProjectChat.updateChatName(group);
                uc.modules.chat.ProjectChat.updateGroupMember(group.members);
                if (group.joinState == 1 || group.status == 1) {
                    uc.modules.chat.ProjectChat.enableInput(false);
                }
            }
        },
        noticeDocListReceived: function (errorCode, groupId, documents) {
            if (this.currentChatType == uc.constants.Chat.ChatType.SingleChat || this.currentChatType == uc.constants.Chat.ChatType.MeetingChat || groupId != this.currentChatId) {
                return;
            }
            if (this.fileList != null) {
                this.fileList.onDocListReceived(errorCode, documents);
            }
        },
        noticeDocumentShared: function (errorCode, groupId, operatorId, document) {
            if (groupId == this.currentChatId) {
                uc.modules.chat.GroupChat.onDocumentShared(errorCode, groupId, operatorId, document);
            }
        },
//        noticeCommentListReceived: function(errorCode, groupId, comments){
//            uc.modules.chat.GroupChat.OnCommentListReceived(errorCode, groupId, comments);
//        },
//        noticeCommentCreated: function(errorCode, groupId, comment){
//            uc.modules.chat.GroupChat.OnCommentCreated(errorCode, groupId, comment);
//        },
        noticeCommentCreateMessageReceived:function(conversationType, channel, from, to, msgId, msgSeq, fileId, fileType,
                                                    fileTitle, commentId, userName, userId, content, createTime, timestamp,cloudType){
            if (this.isCurrentMsg(conversationType, from, to)) {
                uc.modules.chat.GroupChat.OnCommentCreateMessageReceived(conversationType, channel, this.isSendByMyself(from), from, msgId, msgSeq, fileId, fileType,
                    fileTitle, commentId, userName, userId, content, createTime, timestamp,cloudType);
            }
        },
        OnGroupCallDurationReceived:function(channel, conversationType,groupId, duration, msgId, msgSeq, timestamp){
            if(this.currentChatId == groupId && this.currentChatType == conversationType){
                uc.modules.chat.GroupChat.OnGroupCallDurationReceived(channel, conversationType,groupId, duration, msgId, msgSeq, timestamp);
            }
        },
        noticeDocInfoReceived:function(errorCode, groupId, document){
            uc.modules.chat.GroupChat.OnDocInfoReceived(errorCode, groupId, document);
        },
        noticeGroupCloudFileMessageReceived: function (conversationType, channel, sendState, from, to, fileId, fileName, fileSize, downloadUrl, fileFormat, desc, msgId, msgSeq, msgTime, username,operation,detailContent) {
            if (this.isCurrentMsg(conversationType, from, to)) {
                uc.modules.chat.GroupChat.OnGroupCloudFileMessageReceived(conversationType, channel, sendState, this.isSendByMyself(from), from, fileId, fileName, fileSize, downloadUrl, fileFormat, desc, msgId, msgSeq, msgTime, username,operation,detailContent);
            }
        },
        noticeGroupMemberRoleUpdatedMessageReceived:function(conversationType, channel, sendState, from, to, msgId, msgSeq, msgTime,roles){
            if (this.isCurrentMsg(conversationType, from, to)) {
                uc.modules.chat.GroupChat.OnGroupMemberRoleUpdatedMessageReceived(conversationType, channel, sendState, from, to, msgId, msgSeq, msgTime,roles);
            }
        },
        // added by xue.bai_2 on 2016/04/07
        OnRevocationMessage: function(errorCode, groupId, msgId, msgSeq, msgTime){
            if(errorCode != 0){
                if(errorCode == 13500){
                    uc.modules.component.PromptMessageBox.open(
                        {
                            title:uc_resource.MessageBox.Notification,
                            message: uc_resource.MessageBox.BackOutMessageTimeout,
                            buttonMode:"ok",
                            OkText: uc_resource.MessageBox.Known
                        },
                        function(){
                            this.close();
                        });
                }else if(errorCode == 13501){
                    uc.modules.component.PromptMessageBox.open(
                        {
                            title:uc_resource.MessageBox.Notification,
                            message: uc_resource.MessageBox.BackOutMessageRead,
                            buttonMode:"ok",
                            OkText: uc_resource.MessageBox.Known
                        },
                        function(){
                            this.close();
                        });
                }else{
                    uc.modules.component.PromptMessageBox.open(
                        {
                            title:uc_resource.MessageBox.Notification,
                            message: uc_resource.MessageBox.BackOutMessageFailed,
                            buttonMode:"ok",
                            OkText: uc_resource.MessageBox.Known
                        },
                        function(){
                            this.close();
                        });
                }
            }else{
                if(this.msgBox) {
                    this.msgBox.chatList.find("li[seq='" + msgSeq + "']").remove();
                }

                if(this.meetDetailChat && this.meetDetailChat.msgBox){
                    this.meetDetailChat.msgBox.chatList.find("li[seq='" + msgSeq + "']").remove();
                }
            }
        },
        OnRevocationMessageReceived: function(channel, conversationId, conversationType, from, to, msgId, msgSeq, msgTime, seqId){
            if (this.isCurrentMsg(conversationType, from, to)) {
                switch(conversationType){
                    case uc.constants.Conversation.ConversationType.SingleChat:
                        uc.modules.chat.SingleChat.OnRevocationMessageReceived(channel, conversationId, conversationType, from, to, msgId, msgSeq, msgTime, seqId);
                        break;
                    case uc.constants.Conversation.ConversationType.ProjectChat:
                    case uc.constants.Conversation.ConversationType.GroupChat:
                    case uc.constants.Conversation.ConversationType.CloudChat:
                        uc.modules.chat.GroupChat.OnRevocationMessageReceived(channel, conversationId, conversationType, from, to, msgId, msgSeq, msgTime, seqId);
                        break;
                    case uc.constants.Conversation.ConversationType.MeetingMsg:
                        if(this.meetingChat){
                            this.meetingChat.OnRevocationMessageReceived(channel, conversationId, conversationType, from, to, msgId, msgSeq, msgTime, seqId);
                        }

                        if(this.meetDetailChat){
                            this.meetDetailChat.OnRevocationMessageReceived(channel, conversationId, conversationType, from, to, msgId, msgSeq, msgTime, seqId);
                        }
                        break;
                    default:
                        break;
                }
            }
        },
        OnMeetingMessageRevocationReceived: function(channel, conversationId, conversationType, from, to, msgId, msgSeq, msgTime, seqId, eventId){
	        if(this.meetingChat){
	            this.meetingChat.OnRevocationMessageReceived(channel, conversationId, conversationType, from, to, msgId, msgSeq, msgTime, seqId, eventId);
	        }

	        if(this.meetDetailChat){
	            this.meetDetailChat.OnRevocationMessageReceived(channel, conversationId, conversationType, from, to, msgId, msgSeq, msgTime, seqId, eventId);
	        }
        },
        OnGroupSummaryVideoMessageReceived:function(conversationType,channel, from, to, operatorId, summaryId, summaryName, conferenceId, eventId, meetingName, meetingStartTime, updateTime,type,videoName,videoURL,videoThumb,thumbnailsURL,length,size,operateName,operateAvatar,msgId, msgSeq, msgTime){
            if (this.isCurrentMsg(conversationType, from, to)) {
                uc.modules.chat.GroupChat.OnGroupSummaryVideoMessageReceived(channel,this.isSendByMyself(from), from, to, operatorId, summaryId, summaryName, conferenceId, eventId, meetingName, meetingStartTime, updateTime,type,videoName,videoURL,videoThumb,thumbnailsURL,length,size,operateName,operateAvatar,msgId, msgSeq, msgTime);
            }
        },
        OnGroupUnreadUserCountEvent:function(type,userId,info){
            if(type==uc.constants.Chat.ChatType.MeetingChat){
                this.OnMeetingUnreadUserCountEvent(userId,info);
            }else{
                uc.modules.chat.GroupChat.OnGroupUnreadUserCountEvent(userId,info);
            }
        },
        OnGroupUnreadUserListEvent:function(type,userId,info){
            if(type==uc.constants.Chat.ChatType.MeetingChat){
                this.OnMeetingUnreadUserListEvent(userId,info);
            }else{
                uc.modules.chat.GroupChat.OnGroupUnreadUserListEvent(userId,info);
            }
        },
        OnGroupCallInvitationRejected: function(channel,conversationType, groupId, confId, reason,userId, msgId, msgSeq, timestamp,sendState){
            var from={userId:userId};
            var to={userId:groupId};
            if (this.isCurrentMsg(conversationType, from, to)) {
                var textFormat=uc.modules.setting.SettingManager.getFontSettingFormat();
                uc.modules.chat.GroupChat.OnGroupCallInvitationRejected(conversationType, channel, sendState, this.isSendByMyself(from), from, msgId, msgSeq, '', reason, timestamp, textFormat);
            }
        },
        OnAtMessageListReceived:function(errorCode, groupId, atMsg){
            uc.util.LogUtil.info("ChatManager","OnAtMessageListReceived","errorCode"+errorCode,{groupId:groupId,atMsg:atMsg});
            //if(errorCode == 0){
            //    var atCounts = atMsg.length;
            //    var conversationInfo = uc.modules.conversation.ConversationManager.getConversation(groupId, conversationType);
            //    conversationInfo.atCounts = atCounts;
            //    uc.modules.conversation.ConversationManager.updateConversation(groupId, conversationType, conversationInfo)
            //}
            uc.modules.chat.GroupChat.OnAtMessageListReceived(errorCode, groupId, atMsg);

        },
        OnAtMessageDeleted:function(errorCode, conversationType, groupId, atSeq){
            if(atSeq&&atSeq.length){
                var conversationInfo = uc.modules.conversation.ConversationManager.getConversation(groupId, conversationType);
                conversationInfo.atCounts-=atSeq.length;
                if(conversationInfo.atCounts > 0){
                    uc.modules.conversation.ConversationManager.updateConversation(groupId, conversationType, conversationInfo)
                }else{
                    uc.util.LogUtil.info("ChatManager","OnAtMessageDeleted","atCounts"+conversationInfo.atCounts,null);
                    conversationInfo.atCounts = 0;
                    uc.modules.conversation.ConversationManager.updateConversation(groupId, conversationType, conversationInfo)
                }
            }
            uc.modules.chat.GroupChat.OnAtMessageDeleted(errorCode, conversationType, groupId, atSeq);
        },
        //Meeting
        OnMeetingMessageSent: function(errorCode, groupId, eventId, meetingStartTime, msgId, msgSeq, msgTime){
            if (this.meetingChat){
                this.OnImageMessageSend(errorCode, msgId, msgSeq, uc.modules.chat.MeetingChat.getMsgBox());
                uc.modules.chat.MeetingChat.OnMeetingMessageSent(errorCode, groupId, eventId, meetingStartTime, msgId, msgSeq, msgTime);
            }
            if (this.meetDetailChat) {
                this.OnImageMessageSend(errorCode, msgId, msgSeq, this.meetDetailChat.getMsgBox());
                this.meetDetailChat.OnMeetingMessageSent(errorCode, groupId, eventId, meetingStartTime, msgId, msgSeq, msgTime);
            }
        },
        OnMeetingChatHistoryReceived: function(errorCode, groupId, msgCount, minTimestamp){
            if (this.meetingChat){
                uc.modules.chat.MeetingChat.OnMeetingChatHistoryReceived(errorCode, groupId, msgCount, minTimestamp);
            }
            if (this.meetDetailChat) {
                this.meetDetailChat.OnMeetingChatHistoryReceived(errorCode, groupId, msgCount, minTimestamp);
            }
        },
        OnMeetingTextMessageReceived: function(channel, sendState, from, to, meetingId, eventId, meetingStartTime, textType, textContent, msgId, msgSeq, msgTime, textFormat){
            if (this.meetingChat){
                uc.modules.chat.MeetingChat.OnMeetingTextMessageReceived(channel, sendState, this.isSendByMyself(from), from, meetingId, eventId, meetingStartTime, textType, textContent, msgId, msgSeq, msgTime, textFormat);
            }

            if (this.meetDetailChat) {
                this.meetDetailChat.OnMeetingTextMessageReceived(channel, sendState, this.isSendByMyself(from), from, meetingId, eventId, meetingStartTime, textType, textContent, msgId, msgSeq, msgTime, textFormat);
            }
            // if(channel==uc.constants.Message.MessageChannel.UCAS && from!=this.myUserInfo.userId){
            //    uc.modules.meeting.MeetingManager.showMeetingPanelMessage(eventId);
            // }
        },
        OnMeetingImageMessageReceived: function(channel, sendState, from, to, meetingId, eventId, meetingStartTime, fileName, fileSize, fileURL, thumbnails, msgId, msgSeq, msgTime){
            if (this.meetingChat){
                uc.modules.chat.MeetingChat.OnMeetingImageMessageReceived(channel, sendState, this.isSendByMyself(from), from, meetingId, eventId, meetingStartTime, fileName, fileSize, fileURL, thumbnails, msgId, msgSeq, msgTime);
            }

            if (this.meetDetailChat) {
                this.meetDetailChat.OnMeetingImageMessageReceived(channel, sendState, this.isSendByMyself(from), from, meetingId, eventId, meetingStartTime, fileName, fileSize, fileURL, thumbnails, msgId, msgSeq, msgTime);
            }
        },
        OnMeetingAudioMessageReceived: function(channel, sendState, from, to, meetingId, eventId, meetingStartTime, fileName, fileSize, fileURL, duration, msgId, msgSeq,msgTime){
            if (this.meetingChat){
                uc.modules.chat.MeetingChat.OnMeetingAudioMessageReceived(channel, sendState, this.isSendByMyself(from), from, meetingId, eventId, meetingStartTime, fileName, fileSize, fileURL, duration, msgId, msgSeq,msgTime);
            }

            if (this.meetDetailChat) {
                this.meetDetailChat.OnMeetingAudioMessageReceived(channel, sendState, this.isSendByMyself(from), from, meetingId, eventId, meetingStartTime, fileName, fileSize, fileURL, duration, msgId, msgSeq,msgTime);
            }
        },
        OnMeetingVideoMessageReceived: function(channel, sendState, from, to, meetingId, eventId, meetingStartTime, fileName, fileSize, fileURL, thumbnails, duration, msgId, msgSeq, msgTime){
            if (this.meetingChat){
                uc.modules.chat.MeetingChat.OnMeetingVideoMessageReceived(channel, sendState, this.isSendByMyself(from), from, meetingId, eventId, meetingStartTime, fileName, fileSize, fileURL, thumbnails, duration, msgId, msgSeq, msgTime);
            }

            if (this.meetDetailChat) {
                this.meetDetailChat.OnMeetingVideoMessageReceived(channel, sendState, this.isSendByMyself(from), from, meetingId, eventId, meetingStartTime, fileName, fileSize, fileURL, thumbnails, duration, msgId, msgSeq, msgTime);
            }
        },
        OnMeetingReportMessageReceived: function (channel, sendState, from, to, eventId, msgId, msgSeq, msgTime, reportTitle, reportUrl) {
            if (this.meetingChat) {
                uc.modules.chat.MeetingChat.OnMeetingReportMessageReceived(channel, sendState, from, to, eventId, msgId, msgSeq, msgTime, reportTitle, reportUrl);
            }

            if (this.meetDetailChat) {
                this.meetDetailChat.OnMeetingReportMessageReceived(channel, sendState, from, to, eventId, msgId, msgSeq, msgTime, reportTitle, reportUrl);
            }
        },
        OnMeetingInviteMessageReceived: function(channel, from, to, meetingId, eventId, meetingName, accreditUserId, hostId, isRecurrent, meetingStartTime, msgId, msgSeq, msgTime){
            if (this.moduleId == uc.constants.UI.ModuleId.Message || this.meetingChat){
                uc.modules.chat.MeetingChat.OnMeetingInviteMessageReceived(channel, from, to, meetingId, eventId, meetingName, accreditUserId, hostId, isRecurrent, meetingStartTime, msgId, msgSeq, msgTime);
            }

            if (this.meetDetailChat) {
                this.meetDetailChat.OnMeetingInviteMessageReceived(channel, from, to, meetingId, eventId, meetingName, accreditUserId, hostId, isRecurrent, meetingStartTime, msgId, msgSeq, msgTime);
            }
        },
        OnMeetingInvitationAcceptedMessageReceived: function(channel, from, to, meetingId, eventId, meetingName, accreditUserId, hostId, isRecurrent, meetingStartTime, msgId, msgSeq, msgTime){
            if (this.meetingChat){
                uc.modules.chat.MeetingChat.OnMeetingInvitationAcceptedMessageReceived(channel, from, to, meetingId, eventId, meetingName, accreditUserId, hostId, isRecurrent, meetingStartTime, msgId, msgSeq, msgTime);
            }

            if (this.meetDetailChat) {
                this.meetDetailChat.OnMeetingInvitationAcceptedMessageReceived(channel, from, to, meetingId, eventId, meetingName, accreditUserId, hostId, isRecurrent, meetingStartTime, msgId, msgSeq, msgTime);
            }
        },
        OnMeetingInvitationRejectedMessageReceived: function(channel, from, to, meetingId, eventId, meetingName, accreditUserId, hostId, isRecurrent, meetingStartTime, msgId, msgSeq, msgTime){
            if (this.meetingChat){
                uc.modules.chat.MeetingChat.OnMeetingInvitationRejectedMessageReceived(channel, from, to, meetingId, eventId, meetingName, accreditUserId, hostId, isRecurrent, meetingStartTime, msgId, msgSeq, msgTime);
            }

            if (this.meetDetailChat) {
                this.meetDetailChat.OnMeetingInvitationRejectedMessageReceived(channel, from, to, meetingId, eventId, meetingName, accreditUserId, hostId, isRecurrent, meetingStartTime, msgId, msgSeq, msgTime);
            }
        },
        OnMeetingUpdateMessageReceived: function(channel, from, to, meetingId, eventId, meetingName, accreditUserId, hostId, isRecurrent, meetingStartTime, updateFlag, msgId, msgSeq, msgTime){
            if (channel != uc.constants.Message.MessageChannel.UCAS) {
                return;
            }
            if (this.meetingChat){
                uc.modules.chat.MeetingChat.OnMeetingUpdateMessageReceived(channel, from, to, meetingId, eventId, meetingName, accreditUserId, hostId, isRecurrent, meetingStartTime, updateFlag, msgId, msgSeq, msgTime);
            }

            if (this.meetDetailChat) {
                this.meetDetailChat.OnMeetingUpdateMessageReceived(channel, from, to, meetingId, eventId, meetingName, accreditUserId, hostId, isRecurrent, meetingStartTime, updateFlag, msgId, msgSeq, msgTime);
            }
        },
        OnMeetingForwardMessageReceived: function(channel, from, to, meetingId, eventId, meetingName, accreditUserId, hostId, isRecurrent, meetingStartTime, msgId, msgSeq, msgTime){
            if (this.meetingChat){
                uc.modules.chat.MeetingChat.OnMeetingForwardedMessageReceived(channel, from, to, meetingId, eventId, meetingName, accreditUserId, hostId, isRecurrent, meetingStartTime, msgId, msgSeq, msgTime);
            }

            if (this.meetDetailChat) {
                this.meetDetailChat.OnMeetingForwardedMessageReceived(channel, from, to, meetingId, eventId, meetingName, accreditUserId, hostId, isRecurrent, meetingStartTime, msgId, msgSeq, msgTime);
            }
        },
        OnMeetingCancelledMessageReceived: function(channel, from, to, meetingId, eventId, groupId, meetingName, accreditUserId, hostId, isRecurrent,meetingStartTime, msgId, msgSeq, msgTime){
            if (this.meetingChat){
                uc.modules.chat.MeetingChat.OnMeetingCancelledMessageReceived(channel, from, to, meetingId, eventId, groupId, meetingName, accreditUserId, hostId, isRecurrent,meetingStartTime, msgId, msgSeq, msgTime);
            }

            if (this.meetDetailChat) {
                this.meetDetailChat.OnMeetingCancelledMessageReceived(channel, from, to, meetingId, eventId, groupId, meetingName, accreditUserId, hostId, isRecurrent,meetingStartTime, msgId, msgSeq, msgTime);
            }
        },
        OnMinutesCreateMessageReceived: function(channel, from, to, operatorId, minutesId, minutesTitle, meetingId,eventId, meetingName, meetingStartTime, updateTime, msgId, msgSeq, msgTime){
            if (this.meetingChat){
                uc.modules.chat.MeetingChat.OnMinutesCreateMessageReceived(channel, from, to, operatorId, minutesId, minutesTitle,meetingId,eventId, meetingName, meetingStartTime, updateTime, msgId, msgSeq, msgTime);
            }

            if (this.meetDetailChat) {
                this.meetDetailChat.OnMinutesCreateMessageReceived(channel, from, to, operatorId, minutesId, minutesTitle,meetingId,eventId, meetingName, meetingStartTime, updateTime, msgId, msgSeq, msgTime);
            }
        },
        OnMinutesUpdateMessageReceived: function(channel, from, to, operatorId,  minutesId, minutesTitle, state,eventId, meetingName, meetingStartTime, updateTime, msgId, msgSeq, msgTime){
            if (this.meetingChat){
                uc.modules.chat.MeetingChat.OnMinutesUpdateMessageReceived(channel, from, to, operatorId,  minutesId, minutesTitle, state,eventId, meetingName, meetingStartTime, updateTime, msgId, msgSeq, msgTime);
            }

            if (this.meetDetailChat) {
                this.meetDetailChat.OnMinutesUpdateMessageReceived(channel, from, to, operatorId,  minutesId, minutesTitle, state,eventId, meetingName, meetingStartTime, updateTime, msgId, msgSeq, msgTime);
            }
        },
        OnMeetingInfoReceivedById: function (errorCode,eventId, authorizerId, meetingInfoArray) {
            if (this.meetingChat){
                uc.modules.chat.MeetingChat.OnMeetingInfoReceivedById(errorCode,eventId, authorizerId, meetingInfoArray);
            }
        },
        OnMeetingAcceptMessageReceived: function(channel, from, to, meetingId, eventId, meetingName, accreditUserId, hostId, isRecurrent, meetingStartTime, msgId, msgSeq, msgTime) {
            if (this.meetingChat){
                uc.modules.chat.MeetingChat.OnMeetingAcceptMessageReceived(channel, from, to, meetingId, eventId, meetingName, accreditUserId, hostId, isRecurrent, meetingStartTime, msgId, msgSeq, msgTime);
            }
        },
        OnMeetingRejectMessageReceived: function(channel, from, to, meetingId, eventId, meetingName, accreditUserId, hostId, isRecurrent, meetingStartTime,updateFlag, msgId, msgSeq, msgTime) {
            if (this.meetingChat){
                uc.modules.chat.MeetingChat.OnMeetingRejectMessageReceived(channel, from, to, meetingId, eventId, meetingName, accreditUserId, hostId, isRecurrent, meetingStartTime,updateFlag, msgId, msgSeq, msgTime);
            }
        },
        OnMeetingSummaryVideoMessageReceived:function(channel, from, to, operatorId, summaryId, summaryName, conferenceId, eventId, meetingName, meetingStartTime, updateTime,type,videoName,videoURL,videoThumb,thumbnailsURL,length,size,operateName,operateAvatar,msgId, msgSeq, msgTime){
            if (this.meetingChat){
                uc.modules.chat.MeetingChat.OnMeetingSummaryVideoMessageReceived(channel, this.isSendByMyself(from),from, to, operatorId, summaryId, summaryName, conferenceId, eventId, meetingName, meetingStartTime, updateTime,type,videoName,videoURL,videoThumb,thumbnailsURL,length,size,operateName,operateAvatar,msgId, msgSeq, msgTime);
            }

            if (this.meetDetailChat) {
                this.meetDetailChat.OnMeetingSummaryVideoMessageReceived(channel, this.isSendByMyself(from),from, to, operatorId, summaryId, summaryName, conferenceId, eventId, meetingName, meetingStartTime, updateTime,type,videoName,videoURL,videoThumb,thumbnailsURL,length,size,operateName,operateAvatar,msgId, msgSeq, msgTime);
            }
        },
        OnMeetingUnreadUserCountEvent:function(userId,info){
            if (this.meetingChat){
                uc.modules.chat.MeetingChat.OnMeetingUnreadUserCountEvent(userId,info);
            }

            if (this.meetDetailChat) {
                this.meetDetailChat.OnMeetingUnreadUserCountEvent(userId,info);
            }
        },
        OnMeetingUnreadUserListEvent:function(userId,info){
            if (this.meetingChat && this.moduleId == uc.constants.UI.ModuleId.Message){
                uc.modules.chat.MeetingChat.OnMeetingUnreadUserListEvent(userId,info);
            }

            if (this.meetDetailChat) {
                if(this.meetDetailChat.moduleId == this.moduleId){
                    this.meetDetailChat.OnMeetingUnreadUserListEvent(userId,info);
                }
            }
        },
        OnMinutesDetailsReceived:function(errorCode,eventId,minutesId){
            if (this.meetingChat && this.moduleId == uc.constants.UI.ModuleId.Message){
                uc.modules.chat.MeetingChat.OnMinutesDetailsReceived(errorCode,eventId,minutesId);
            }

            if (this.meetDetailChat) {
                if(this.meetDetailChat.moduleId==this.moduleId){
                    this.meetDetailChat.OnMinutesDetailsReceived(errorCode,eventId,minutesId);
                }
            }
        },
        //授权消息走P2P
        OnMeetingAccreditMessageReceived: function(channel, from, to, msgId, msgSeq, msgTime){
            if (!this.isCurrentMsg(uc.constants.Chat.ChatType.SingleChat, from, to)) {
                return;
            }
            uc.modules.chat.SingleChat.OnMeetingAccreditMessageReceived(channel, from, to, msgId, msgSeq, msgTime);
        },
        OnMeetingAccreditCancelledMessageReceived: function(channel, from, to, msgId, msgSeq, msgTime){
            if (!this.isCurrentMsg(uc.constants.Chat.ChatType.SingleChat, from, to)) {
                return;
            }
            uc.modules.chat.SingleChat.OnMeetingAccreditCancelledMessageReceived(channel, from, to, msgId, msgSeq, msgTime);
        },
        OnIMSummaryVideoMessageReceived:function(channel, from, to, operatorId, summaryId, summaryName, conferenceId, eventId, meetingName, meetingStartTime, updateTime,type,videoName,videoURL,videoThumb,thumbnailsURL,length,size,operateName,operateAvatar,msgId, msgSeq, msgTime){
            if (!this.isCurrentMsg(uc.constants.Chat.ChatType.SingleChat, from, to)) {
                return;
            }
            uc.modules.chat.SingleChat.OnIMSummaryVideoMessageReceived(channel,this.isSendByMyself(from), from, to, operatorId, summaryId, summaryName, conferenceId, eventId, meetingName, meetingStartTime, updateTime,type,videoName,videoURL,videoThumb,thumbnailsURL,length,size,operateName,operateAvatar,msgId, msgSeq, msgTime);
        },
        OnIMUnreadUserCountEvent:function(userId,info){
            uc.modules.chat.SingleChat.OnIMUnreadUserCountEvent(userId,info);
        },
        //OpenApi
        OnApiChatHistoryReceived: function(errorCode, appId, msgCount, minTimestamp){
            if (this.currentChatType != uc.constants.Chat.ChatType.OpenApiChat &&
                this.currentChatId != appId) {
                return;
            }
            uc.modules.chat.OpenApiChat.onChatHistoryReceived(errorCode, appId, msgCount, minTimestamp);
        },
        OnOpenApiTextMessageReceived: function(channel, sendState, from, to, textTitle, textContent, textType, detailContent, msgId, msgSeq, mstTime, detailAuth){
            if (!this.isCurrentMsg(uc.constants.Chat.ChatType.OpenApiChat, from, to)) {
                return;
            }
            uc.modules.chat.OpenApiChat.OnOpenApiTextMessageReceived(channel, sendState, from, to, textTitle, textContent, textType, detailContent, msgId, msgSeq, mstTime, detailAuth);
        },
        OnOpenApiImageMessageReceived: function(channel, sendState, from, to, imageId, imageType, imageData, msgId, msgSeq, msgTime){
            if (!this.isCurrentMsg(uc.constants.Chat.ChatType.OpenApiChat, from, to)) {
                return;
            }
            uc.modules.chat.OpenApiChat.OnOpenApiImageMessageReceived(channel, sendState, from, to, imageId, imageType, imageData, msgId, msgSeq, msgTime);
        },
        OnOpenApiFileMessageReceived: function(channel, sendState, from, to, fileId, fileType, fileName, fileSize, msgId, msgSeq, mstTime){
            if (!this.isCurrentMsg(uc.constants.Chat.ChatType.OpenApiChat, from, to)) {
                return;
            }
            uc.modules.chat.OpenApiChat.OnOpenApiFileMessageReceived(channel, sendState, from, to, fileId, fileType, fileName, fileSize, msgId, msgSeq, mstTime);
        },
        OnOAMessageReceived: function(channel, from, to, title, titleColor, titleBkColor, titleElements, titleStyle, status, data, detailUrl, msgId, msgSeq,msgTime, detailAuth, oaExternalData) {
            if (!this.isCurrentMsg(uc.constants.Chat.ChatType.OpenApiChat, from, to)) {
                return;
            }
            uc.modules.chat.OpenApiChat.OnOAMessageReceived(channel, from, to, title, titleColor, titleBkColor, titleElements, titleStyle, status, data, detailUrl, msgId, msgSeq,msgTime, detailAuth, oaExternalData);
        },
        OnOAMessageStatusChanged: function(appId, msgSeq, status, from, extraData) {
            uc.modules.chat.OpenApiChat.OnOAMessageStatusChanged(appId, msgSeq, status, from, extraData);
        },
        OnPushMessageAck: function(channel, errorCode, from, to, conversation, msgId, msgSeq, msgTime){
            uc.modules.chat.OpenApiChat.OnPushMessageAck(channel, errorCode, from, to, conversation, msgId, msgSeq, msgTime);
        },
        OnOAMessageStatusReceived: function(errorCode, appId, msgSeq, status) {
            uc.modules.chat.OpenApiChat.OnOAMessageStatusReceived(errorCode, appId, msgSeq, status);
        },


        //FS
        noticeFileDownloading: function (errorCode, taskId, percent,fromDomObj) {
            if (this.moduleId == uc.constants.UI.ModuleId.Meeting) {
                return;
            }

            //var _meetingFileListIsActivity = uc.modules.meeting.MeetingManager.getMeetingPanelFileListIsActivity();
            ////如果是在会议资料列表下载文件
            //if (this.moduleId == uc.constants.UI.ModuleId.Meeting && _meetingFileListIsActivity) {
            //    return;
            //}

            if (errorCode != 0) {
                return;
            }
            if (this.fileDownLoadInfo["" + taskId] == null) {
                return;
            }
            var msgId = this.fileDownLoadInfo["" + taskId].msgId;
            if (this.msgBox != null) {
                this.msgBox.OnFileDownloading(msgId, percent,fromDomObj);
            }
            if (this.meetDetailChat) {
                var msgBox = this.meetDetailChat.getMsgBox();
                if (msgBox != null) {
                    msgBox.OnFileDownloading(msgId, percent);
                }
            }
            if (this.fileList != null) {
                this.fileList.OnFileDownloading(msgId, percent);
            }
            if (this.fileComment != null) {
                this.fileComment.OnFileDownloading(msgId, percent);
            }
        },
        getLoaclUrl: function(sUrl) {
            if (sUrl == null ||  sUrl == "") {
                return "";
            }
            try {
                var url = this.downloadedFiles[sUrl];
                if (url == null) {
                    return "";
                }
                if (!uc.util.FileUtil.fileExist(url)) {
                    uc.util.LogUtil.info("ChatManager","getLoaclUrl","File doesn't exist in native and localpath is"+url,null);
                    delete  this.downloadedFiles[sUrl];
                    return "";
                }
                return url;
            } catch(e){
            }
            return "";
        },
        noticeFileDownloaded: function(errorCode, taskId, savePath,fromDomObj) {
            //var _meetingFileListIsActivity = uc.modules.meeting.MeetingManager.getMeetingPanelFileListIsActivity();
            ////如果是在会议资料列表下载文件,return
            //if (this.moduleId == uc.constants.UI.ModuleId.Meeting && _meetingFileListIsActivity) {
            //    return;
            //}
            if(fromDomObj!=null)
            {
                if (this.fileDownLoadInfo_Copy == null) {
                    return;
                }
                this.fileDownLoadInfo =  JSON.parse(this.fileDownLoadInfo_Copy);
            }
            if (this.fileDownLoadInfo["" + taskId] == null) {
                return;
            }
            uc.util.LogUtil.info('ChatManager', 'noticeFileDownloaded', 'upload taskId = ' + taskId + ", errorCode = " + errorCode, null);
            var msgId = this.fileDownLoadInfo["" + taskId].msgId;
            if (errorCode == 0) {
                var sUrl = this.fileDownLoadInfo["" + taskId].sUrl;
                this.downloadedFiles[sUrl] = savePath;
            }
            if (this.msgBox != null) {
                this.msgBox.OnFileDownloaded(errorCode, msgId, savePath,fromDomObj);
            }
            if (this.meetDetailChat) {
                var msgBox = this.meetDetailChat.getMsgBox();
                if (msgBox != null) {
                    msgBox.OnFileDownloaded(errorCode, msgId, savePath);
                }
            }
            if (this.fileList != null) {
                this.fileList.OnFileDownloaded(errorCode, msgId, savePath);
            }
            if (this.fileComment != null) {
                this.fileComment.OnFileDownloaded(errorCode, msgId, savePath);
            }

            this.fileDownLoadInfo["" + taskId].errorCode = errorCode;
            if(fromDomObj==null)
            {
                this.fileDownLoadInfo_Copy = JSON.stringify(this.fileDownLoadInfo);
            }
            var isFile = this.fileDownLoadInfo["" + taskId].isFile;
            if (errorCode == 0 || errorCode == 22 || errorCode == 404) {
                delete this.fileDownLoadInfo["" + taskId];
                if(fromDomObj!=null)
                {
                    delete this.fileDownLoadInfo_Copy ;
                }
            }
            if (errorCode != 0) {
                if (isFile) {
                    uc.modules.component.PromptMessageBox.open({title: uc_resource.Common.Prompt, message: uc_resource.Chat.RetryDownload, buttonMode:"ok", OkText: uc_resource.Common.Known},
                        function(){
                            this.close();
                        });
                }
                this.cancelDownload(msgId);
            }
        },
        noticeFileUploading: function (errorCode, taskId, percent) {
            if (errorCode != 0) {
                return;
            }
            var uploadInfo = this.fileUpLoadInfo["" + taskId];
            if (uploadInfo == null) {
                return;
            }
            this.fileUpLoadInfo["" + taskId].percent = percent;
            //if (uploadInfo.chatType != this.currentChatType || uploadInfo.chatId != this.currentChatId) {
            //    return;
            ///}
            var msgId = uploadInfo.msgId;
            if (this.msgBox != null) {
                this.msgBox.OnFileUploading(msgId, percent);
            }

            if (this.meetDetailChat) {
                var msgBox = this.meetDetailChat.getMsgBox();
                if (msgBox != null) {
                    msgBox.OnFileUploading(msgId, percent);
                }
            }
        },
        noticeFileUploaded: function (errorCode, taskId, fileName, fileSize, operatorId, fid, operatorTime, downloadURL, convertDownloadURL) {
            var uploadInfo = this.fileUpLoadInfo["" + taskId];
            if (uploadInfo == null) {
                return;
            }
            uc.util.LogUtil.info('ChatManager', 'noticeFileUploaded', 'upload taskId = ' + taskId + ", errorCode = " + errorCode, null);
            this.fileUpLoadInfo["" + taskId].errorCode = errorCode;
            if (errorCode == 0) {
                this.downloadedFiles[downloadURL] = uploadInfo.filePath;
                //if (uploadInfo.chatType != this.currentChatType || uploadInfo.chatId != this.currentChatId) {
                //    return;
                //}
                if (this.msgBox != null) {
                    this.msgBox.OnFileUploaded(errorCode, uploadInfo.msgId, uploadInfo.filePath, downloadURL);
                }
                if (this.meetDetailChat) {
                    var msgBox = this.meetDetailChat.getMsgBox();
                    if (msgBox != null) {
                        msgBox.OnFileUploaded(errorCode, uploadInfo.msgId, uploadInfo.filePath, downloadURL);
                    }
                }
            }else{
                this.setUploadMsgFaild(taskId);
                this.cancelUpload(uploadInfo.msgId);
            }
        },
        setUploadMsgFaild:function(taskId){
            if (this.msgBox != null) {
                this.msgBox.onFailedTip(taskId);
            }
            if (this.meetDetailChat) {
                var msgBox = this.meetDetailChat.getMsgBox();
                if (msgBox != null) {
                    msgBox.onFailedTip(taskId);
                }
            }
        },
        setUploadMsgId: function (taskId, msgId, isImage, localPath, imageName) {
            var uploadInfo = this.fileUpLoadInfo["" + taskId];
            if (uploadInfo == null) {
                return;
            }
            uc.util.LogUtil.info('ChatManager', 'setUploadMsgId', 'upload taskId = ' + taskId, null);
            delete this.fileUpLoadInfo["" + taskId];
            if (isImage) {
                this.uploadCopyImgs[msgId] = {
                    path: localPath,
                    chatType: uploadInfo.chatType,
                    chatId: uploadInfo.chatId,
                    imageName: imageName
                };
            }
            //if (uploadInfo.chatType != this.currentChatType || uploadInfo.chatId != this.currentChatId) {
            //    return;
            //}
            this.FSFileInfo[msgId]={
                "msgId":msgId,
                "fileName":uploadInfo.args.fileName,
                "fileSize":uploadInfo.args.fileSize,
                "fileId":""
            };
            if (this.msgBox != null) {
                this.msgBox.replaceMsgId(uploadInfo.msgId, msgId);
            }
            if (this.meetDetailChat) {
                var msgBox = this.meetDetailChat.getMsgBox();
                if (msgBox != null) {
                    msgBox.replaceMsgId(uploadInfo.msgId, msgId);
                }
            }
        },
        downloadImg: function (chatId, chatType, msgId, seq, url, imageName, callback) {
            if (url == "") {
                for (var key in this.fileUpLoadInfo) {
                    var info = this.fileUpLoadInfo[key];
                    if (info.msgId == msgId) {
                        return;
                    }
                }
            }

            if(imageName.length > 100){
                var chatImgName = imageName.substring(imageName.length-100,imageName.length);
            }else{
                var chatImgName = imageName;
            }

            var savePath = uc.IClientService.getDataDirectory(uc.constants.UI.DirectoryType.Image) + '\\' + chatId + '_' + chatType + '\\' + seq + '_' + chatImgName;
            uc.IFSService.downloadFile(url, savePath,
                function (args) {
                    uc.util.LogUtil.info('ChatManager', 'downloadImg', 'errorCode = ' + args[0] + ", url = " + url + ", savePath = " + savePath, null);
                    if (args[0] != 0 || args[1] == null) {
                        if (uc.modules.chat.ChatManager.msgBox != null) {
                            uc.modules.chat.ChatManager.msgBox.OnFileDownloaded(-1, msgId, "");
                        }
                        if (uc.modules.chat.ChatManager.meetDetailChat) {
                            var msgBox = uc.modules.chat.ChatManager.meetDetailChat.getMsgBox();
                            if (msgBox != null) {
                                msgBox.OnFileDownloaded(-1, msgId, "");
                            }
                        }
                        return;
                    }
                    var taskId = args[1];
                    uc.modules.chat.ChatManager.fileDownLoadInfo["" + taskId] = {
                        msgId: msgId,
                        sUrl: url
                    };
                    if (callback != null) {
                        callback();
                    }
                });
        },
        downloadFile: function (self, msgId, url, fname, callback,type) {
            uc.modules.chat.ChatManager.openSaveFileDialog(self,msgId,type,fname,function(savePath){
                if (savePath == null || savePath == "") {
                    return;
                }
                uc.IFSService.downloadFile(url, savePath,
                    function (args) {
                        uc.util.LogUtil.info('ChatManager', 'downloadFile', 'errorCode = ' + args[0] + ", url = " + url + ", savePath = " + savePath, null);
                        if (args[0] != 0 || args[1] == null) {
                            return;
                        }
                        var taskId = args[1];
                        uc.modules.chat.ChatManager.fileDownLoadInfo["" + taskId] = {
                            msgId: msgId,
                            sUrl: url,
                            isFile: true
                        };
                        if (callback != null) {
                            callback();
                        }
                    });
            });
        },
        openSaveFileDialog:function(self,msgId,type,fname,callBack){
            if(type=="receive"){
                var Obj = uc.util.FileUtil.getDefaultFileSavePath(fname);
                var fileObj = JSON.parse(Obj);
                var savePath =  fileObj[0];
                var fileName =  fileObj[1];
                self.parents("li[id="+msgId+"]").find(".name").text(fileName);
                uc.util.LogUtil.info('ChatManager','openSaveFileDialog','receiveFile savePath',savePath);
                callBack&&callBack(savePath);
            }else{
                uc.util.FileDialogUtil.openSaveFileDialog(uc_resource.MessageBox.SaveFile, fname, [{ desc: uc_resource.MessageBox.AllFile, filter: '*.*'}],
                    function (args) {
                        var savePath=args[0];
                        callBack&&callBack(savePath);
                    });
            }
        },
        dowloadCode: function (msgId, url) {
            uc.util.FileDialogUtil.openSaveFileDialog(uc_resource.Chat.SaveFiles, "", [{ desc: uc_resource.Chat.AllFiles, filter: '*.*'}],
                function (args) {
                    var savePath = args[0];
                    if (savePath == null || savePath == "") {
                        return;
                    }
                    savePath += ".html";
                    uc.IFSService.downloadFile(url, savePath,
                        function (args) {
                            uc.util.LogUtil.info('ChatManager', 'dowloadCode', 'errorCode = ' + args[0] + ", url = " + url + ", savePath = " + savePath, null);
                            if (args[0] != 0 || args[1] == null) {
                                return;
                            }
                            var taskId = args[1];
                            uc.modules.chat.ChatManager.fileDownLoadInfo["" + taskId] = {
                                msgId: msgId,
                                sUrl: url
                            };
                        });
                });
        },
        dowloadAudio: function (msgId, msgSeq, url) {
            var savePath = uc.IClientService.getDataDirectory(uc.constants.UI.DirectoryType.Audio) + '\\' + this.currentChatId + '_' + this.currentChatType + '\\' + msgSeq;
            uc.IFSService.downloadFile(url, savePath,
                function (args) {
                    uc.util.LogUtil.info('ChatManager', 'dowloadAudio', 'errorCode = ' + args[0] + ", url = " + url + ", savePath = " + savePath, null);
                    if (args[0] != 0 || args[1] == null) {
                        return;
                    }
                    var taskId = args[1];
                    uc.modules.chat.ChatManager.fileDownLoadInfo["" + taskId] = {
                        msgId: msgId,
                        sUrl: url
                    };
                });
        },
        cancelDownload: function (msgId) {
            var taskId = null;
            for (var id in this.fileDownLoadInfo) {
                var tempId = this.fileDownLoadInfo["" + id].msgId;
                if (tempId == msgId) {
                    taskId = id;
                }
            }
            if (taskId == null) {
                return;
            }
            uc.util.LogUtil.info('ChatManager', 'cancelDownload', 'taskId = ' + taskId, null);
            uc.IFSService.cancelDownloadFile(Number(taskId));
            delete this.fileDownLoadInfo["" + taskId];
        },
        cancelUpload: function (msgId) {
            for (var key in this.fileUpLoadInfo) {
                var info = this.fileUpLoadInfo[key];
                if (info.msgId = msgId) {
                    var taskId = info.args.taskId;
                    uc.util.LogUtil.info('ChatManager', 'cancelUploadFile', 'taskId = ' + taskId, null);
                    uc.IFSService.cancelUploadFile(taskId);
                    uc.IUIEvent.OnMessageFileUploadCancelled(this.currentChatId, this.currentChatType, taskId);
                    delete this.fileUpLoadInfo["" + taskId];
                    return;
                }
            }
        },
        OnFSTasksStatusCheckLoop : function() {
            setTimeout(uc.modules.chat.ChatManager.OnFSTasksStatusCheckLoop, 5000);
            if (uc.getClientStatus() != uc.constants.ClientStatus.Connected) {
                return;
            }
            for (var key in uc.modules.chat.ChatManager.fileUpLoadInfo) {
                if (uc.modules.chat.ChatManager.fileUpLoadInfo[key].errorCode == null) {
                    continue;
                }
                if (uc.modules.chat.ChatManager.fileUpLoadInfo[key].errorCode != 0) {
                    if (uc.modules.chat.ChatManager.fileUpLoadInfo[key].retryTimes != null &&
                        uc.modules.chat.ChatManager.fileUpLoadInfo[key].retryTimes> 10) {
                        delete uc.modules.chat.ChatManager.fileUpLoadInfo[key];
                        continue;
                    }
                    uc.modules.chat.ChatManager.fileUpLoadInfo[key].errorCode = 0;
                    if (uc.modules.chat.ChatManager.fileUpLoadInfo[key].retryTimes == null) {
                        uc.modules.chat.ChatManager.fileUpLoadInfo[key].retryTimes = 1;
                    } else {
                        uc.modules.chat.ChatManager.fileUpLoadInfo[key].retryTimes++;
                    }
                    uc.util.LogUtil.info('ChatManager', 'resumeTask', 'upload taskId = ' + key, null);
                    uc.IFSService.resumeTask(Number(key));
                }
            }
            for (var key in uc.modules.chat.ChatManager.fileDownLoadInfo) {
                if (uc.modules.chat.ChatManager.fileDownLoadInfo[key].errorCode == null) {
                    continue;
                }
                if (uc.modules.chat.ChatManager.fileDownLoadInfo[key].errorCode != 0) {
                    if (uc.modules.chat.ChatManager.fileDownLoadInfo[key].retryTimes != null &&
                        uc.modules.chat.ChatManager.fileDownLoadInfo[key].retryTimes> 10) {
                        delete uc.modules.chat.ChatManager.fileDownLoadInfo[key];
                        continue;
                    }
                    uc.modules.chat.ChatManager.fileDownLoadInfo[key].errorCode = 0;
                    if (uc.modules.chat.ChatManager.fileDownLoadInfo[key].retryTimes == null) {
                        uc.modules.chat.ChatManager.fileDownLoadInfo[key].retryTimes = 1;
                    } else {
                        uc.modules.chat.ChatManager.fileDownLoadInfo[key].retryTimes++;
                    }
                    uc.util.LogUtil.info('ChatManager', 'resumeTask', 'download taskId = ' + key, null);
                    uc.IFSService.resumeTask(Number(key));
                }
            }
        },
        getUserLogo: function (userId, logoUrl) {
            uc.IUIService.downloadUserAvatar(userId, logoUrl);
        },

        NoticeUserAvatarDownloaded: function (userId, serverAvatarUrl, localAvatarUrl){
            if (this.msgBox != null) {
                this.msgBox.updateUserLog(userId, localAvatarUrl);
            }
            if (this.meetDetailChat) {
                var msgBox = this.meetDetailChat.getMsgBox();
                if (msgBox != null) {
                    msgBox.updateUserLog(userId, localAvatarUrl);
                }
            }
            if (this.fileComment != null) {
                this.fileComment.updateUserLog(userId, localAvatarUrl);
            }

            uc.modules.chat.GroupChat.updateUserAvatar(userId, localAvatarUrl);
        },
        OnChatImageDownloaded: function(chatId, chatType, serverUrl, localUrl, msgSeq){
            if(this.currentChatId == chatId && this.currentChatType == chatType){
                if(this.msgBox){
                    this.msgBox.OnChatImageDownloaded(localUrl, msgSeq);
                }
            }
        },
        playSound: function (msgId, msgSeq, url){
            var localUrl = this.getLoaclUrl(url);
            if (localUrl != null && localUrl != "") {
                this.playLocalSound(localUrl, msgId);
                return;
            }
            this.dowloadAudio(msgId, msgSeq, url);
        },
        playLocalSound: function(path, msgId) {
            if (this.lastSoundId != null) {
                uc.util.AudioUtil.stopPlay(this.lastSoundId);
            }
            this.lastSoundId = msgId;
            uc.util.AudioUtil.playSound(path, msgId,
                function(args){
                });
        },
        OnPlayingSoundNotification: function(id, percent, seconds) {
            //console.log(id, percent, seconds);
            if (this.msgBox != null) {
                this.msgBox.OnSoundPlaying(id, percent, seconds);
            }
            if (this.meetDetailChat) {
                var msgBox = this.meetDetailChat.getMsgBox();
                if (msgBox != null) {
                    msgBox.OnSoundPlaying(id, percent, seconds);
                }
            }
        },

        updateChatContactInfo: function (contacts) {
            var i, msgBox;
            for (var t = 0; t < contacts.length; t++) {
                var contact = contacts[t];
                if (this.msgBox != null) {
                    this.msgBox.updateUserName(contact.userId, contact.displayName);
                }
                if (this.meetDetailChat) {
                    msgBox = this.meetDetailChat.getMsgBox();
                    if (msgBox != null) {
                        msgBox.updateUserName(contact.userId, contact.displayName);
                    }
                }
                if (contact.localAvatar) {
                    if (this.msgBox != null) {
                        this.msgBox.updateUserLog(contact.userId, contact.localAvatar);
                    }
                    if (this.meetDetailChat) {
                        msgBox = this.meetDetailChat.getMsgBox();
                        if (msgBox != null) {
                            msgBox.updateUserLog(contact.userId, contact.localAvatar);
                        }
                    }
                } else if (contact.remoteAvatarUrl) { //down load user logo
                    this.getUserLogo(contact.userId, contact.remoteAvatarUrl);
                }

                //单人聊天，更新标题为对方DisplayName
                if (this.currentChatType == uc.constants.Chat.ChatType.SingleChat &&
                    this.currentChatId == contact.userId) {
                    uc.modules.chat.SingleChat.updateChatName(contact.displayName, contact.personalSign);
                    uc.modules.chat.SingleChat.updateChatMobile(contact.mobile);
                }

                uc.modules.chat.GroupChat.updateContact(contact, true);
            }
        },
        setDraft: function (chatId, chatType, moduleId) {
            return;
            var draft = uc.modules.draft.DraftMessageManager.getDraftMessage(this.currentChatId, this.currentChatType);
            if ( !draft || !draft.content && !draft.draft) {
                return;
            }

            var msgInputBox;
            if (this.meetDetailChat) {
                msgInputBox = this.meetDetailChat.getMsgInputBox();
                if(msgInputBox){
                    msgInputBox.setDraft(chatId, chatType, moduleId);
                }
            }
        },
        startChat: function (userIds) {
            userIds = userIds == null ? [] : userIds;
            uc.IUIService.openChooseContactWindow(
                {
                    key:"StartChat",
                    selectedUserIdList: userIds
                }, function (args) {
                    if (args.okOrCancel) {
                        if (!args.contacts && args.contacts.length == 0) {
                            return;
                        }

                        if (args.contacts.length == 1) {
                            uc.IUIService.openChatWindow(uc.constants.Chat.ChatType.SingleChat, args.contacts[0].userId, uc.constants.UI.OpenChatWindowSourceModule.Other, null, null, true);
                        }
                        else {
                            var currentUserInfo = uc.IContactService.getCurrentUserInfo();
                            var count = 0, groupName = currentUserInfo.displayName,
                                persons = [{ userId: currentUserInfo.userId, username: currentUserInfo.pinyin}];
                            for (var i = 0; i < args.contacts.length; i++) {
                                if (count < 3 && args.contacts[i].userId != currentUserInfo.userId) {
                                    count++;
                                    groupName += "、" + args.contacts[i].displayName;
                                }

                                persons.push({
                                    userId: args.contacts[i].userId,
                                    username: args.contacts[i].pinyin
                                });
                            }
                            uc.IGroupService.createGroup(uc.constants.GroupType.Group, groupName, persons, 1, 0, function (args) {
                                console.log("Create Group Callback:" + args[0]);
                            });
                        }
                    }
                });
        },
        createProject: function () {

            var options = {
                width: 400,
                height: 200,
                refreshIfExist: false,
                centerScreen: true,
                modal: false,
                resizable: false,
                minWidth: 400,
                minHeight: 200,
                caption: uc.constants.UI.DragCaption.CreateProject
            };

            uc.IUIService.bindCrossWindowEvent("CreateProjectName", function(arg){
                uc.util.StorageUtil.setData("projectName", arg.name);

                uc.IUIService.openChooseContactWindow({
                    key:"CreateProject"
                }, function (args) {
                    if (args.okOrCancel) {

                        var currentUserInfo = uc.IContactService.getCurrentUserInfo();

                        var persons = [{ userId: currentUserInfo.userId, username: currentUserInfo.pinyin}];
                        for (var i = 0; i < args.contacts.length; i++) {
                            persons.push({
                                userId: args.contacts[i].userId,
                                username: args.contacts[i].pinyin
                            });
                        }

                        var name = uc.util.StorageUtil.getData("projectName");

                        uc.IGroupService.createGroup(uc.constants.Chat.ChatType.ProjectChat, name, persons, 1, 0, function (args) {
                            console.log("Create Project Callback:" + args[0]);
                        });
                    }
                });
            });

            uc.util.WindowUtil.open('page/createProject.html', 'createProject', options);
        },
        OnCallStarted: function(chatId, chatType){
            if(this.currentChatId == chatId && this.currentChatType == chatType){
                switch (chatType){
                    case uc.constants.Chat.ChatType.SingleChat:
                        uc.modules.chat.SingleChat.OnCallStarted();
                        break;
                    case uc.constants.Chat.ChatType.ProjectChat:
                    case uc.constants.Chat.ChatType.GroupChat:
                        uc.modules.chat.GroupChat.OnCallStarted();
                        break;
                }
            }
        },
        OnCallEnded: function(chatId, chatType){
            if(this.currentChatId == chatId && this.currentChatType == chatType){
                switch (chatType){
                    case uc.constants.Chat.ChatType.SingleChat:
                        uc.modules.chat.SingleChat.OnCallEnded();
                        break;
                    case uc.constants.Chat.ChatType.ProjectChat:
                    case uc.constants.Chat.ChatType.GroupChat:
                        uc.modules.chat.GroupChat.OnCallEnded();
                        break;
                }
            }
        },

        OnImageMessageSend: function(errorCode, msgId, msgSeq, msgBox) {
            if (errorCode != 0) {
                return;
            }
            var obj = this.uploadCopyImgs[msgId];
            if (obj != null && obj.path != "") {
                if (msgBox != null) {
                    msgBox.setImagSeq(msgId, msgSeq);
                }
                var savePath = uc.IClientService.getDataDirectory(uc.constants.UI.DirectoryType.Image) + '\\' + obj.chatId + '_' + obj.chatType + '\\' + msgSeq + '_' + obj.imageName;
                if (!uc.util.FileUtil.fileExist(savePath)) {
                    uc.util.FileUtil.copyFile(obj.path, savePath);
                }
                setTimeout(function(){
                    delete uc.modules.chat.ChatManager.uploadCopyImgs[msgId];
                }, 1);
            }
        },

        showCodePreview: function(url, fileName, fileSize, type) {
            uc.util.StorageUtil.setData("ShowCodePreview", {
                url: url,
                name: fileName,
                size: fileSize,
                type: type
            });
            var options = {
                width: 660,
                height: 640,
                refreshIfExist: false,
                centerScreen: true,
                modal: true,
                resizable: false,
                minWidth: 520,
                minHeight: 520,
                caption: uc.constants.UI.DragCaption.CodePreview
            };
            uc.util.WindowUtil.open('page/codePreview.html', 'codePreview', options);
        },
        showCodeEdit: function (op) {
            uc.util.StorageUtil.setData("ShowCodeEdit", {
                options: op
            });
            var options = {
                width: 660,
                height: 640,
                refreshIfExist: false,
                centerScreen: true,
                modal: true,
                resizable: false,
                minWidth: 520,
                minHeight: 520,
                caption:uc.constants.UI.DragCaption.CodeEditor
            };
            uc.util.WindowUtil.open('page/codeEditor.html', 'codeEditor', options);
        },
        showApiMsgPreview: function (content) {
            uc.util.StorageUtil.setData("ShowApiMsgPreview", {
                content: content
            });
            var options = {
                width: 530,
                height: 550,
                refreshIfExist: false,
                centerScreen: true,
                modal: true,
                resizable: false,
                minWidth: 530,
                minHeight: 550,
                caption:uc.constants.UI.DragCaption.ApiMsgPreview
            };
            uc.util.WindowUtil.open('page/apiMsgPreview.html', 'ApiMsgPreview', options);
        },
        showApiNoticePreview: function (url) {
            var currentUserInfo = uc.IContactService.getCurrentUserInfo();
            uc.util.StorageUtil.setData("ShowApiNoticePreview", {
                url: url,
                token: this.token,
                account: currentUserInfo.account
            });
            var options = {
                width: 530,
                height: 550,
                refreshIfExist: false,
                centerScreen: true,
                modal: true,
                resizable: false,
                minWidth: 520,
                minHeight: 520,
                caption:uc.constants.UI.DragCaption.ApiNoticePreview
            };
            uc.util.WindowUtil.open('page/apiNoticePreview.html', 'ApiNoticePreview', options);
        },
        OnSendCode: function(eventData){
            if (this.currentChatType != uc.constants.Chat.ChatType.SingleChat &&
                this.currentChatType != uc.constants.Chat.ChatType.GroupChat &&
                this.currentChatType != uc.constants.Chat.ChatType.ProjectChat &&
                this.currentChatType != uc.constants.Chat.ChatType.CloudChat) {
                return;
            }
            var obj = JSON.parse(eventData.data);
            var msgId =  obj.msgId,
                type = obj.type,
                name  = decodeURI(obj.name),
                size = obj.size,
                lines = obj.lines,
                head = decodeURI(obj.head),
                des = decodeURI(obj.des),
                //path = decodeURI(obj.path),
                url= decodeURI(obj.url);
            if (this.msgBox != null) {
                var from = {
                    userId: this.myUserInfo.userId,
                    displayName: this.myUserInfo.displayName
                };
                var domStr = this.msgBox.getCodeMsgDomStr(uc.constants.Message.SendState.SendOK, true, from,
                    msgId, new Date().getTime(), name, size, url, type, head, lines, des, new Date().getTime(), null, this.myUserInfo.displayName);
                this.msgBox.appendMessage(domStr);
                this.msgBox.scrollToEnd();
                uc.IUIEvent.OnMessageSending({
                    chatId: this.currentChatId,
                    chatType: this.currentChatType,
                    msgId:msgId,
                    contentType: uc.constants.Message.ContentType.Code
                })
            }
        },
        updateInputBoxNotice: function() {
            if (this.msgInputBox != null) {
                this.msgInputBox.updateNotice();
            }
            if (this.meetDetailChat) {
                var box = this.meetDetailChat.getMsgInputBox();
                if (box != null) {
                    box.updateNotice();
                }
            }
        },
        OnDraftMessageChanged: function(chatId, chatType, moduleId){
            if(chatId == this.currentChatId && chatType == this.currentChatType){
                this.setDraft(chatId, chatType, moduleId);
            }
        },
        cloudFileDataToString:function(data){
            var cloudFileInfo={
                "mount_id":data.mountid,
                "hash":data.hash,
                "filehash":data.filehash,
                "filename":data.filename,
                "filesize":data.filesize
            };
            return JSON.stringify(cloudFileInfo);
        },
        OnSendCloudFile: function(chatInfo,obj){
            if (this.currentChatType != uc.constants.Chat.ChatType.SingleChat &&
                this.currentChatType != uc.constants.Chat.ChatType.GroupChat &&
                this.currentChatType != uc.constants.Chat.ChatType.ProjectChat&&
                this.currentChatType != uc.constants.Chat.ChatType.MeetingChat
            ) {
                return;
            }
            //  var cloudFileInfo=uc.modules.chat.ChatManager.cloudFileDataToString(obj);
            var cloudFileInfo=JSON.stringify(obj);
            var msgId =  obj.msgId,
                type = obj.type,
                name  = decodeURI(obj.filename),
                size = obj.filesize
            var _msgbox = this.msgBox;
            if(!_msgbox){
                if(this.meetDetailChat!=null){
                    _msgbox =  this.meetDetailChat.msgBox;
                }
            }
            if(_msgbox) {
                var from = {
                    userId: this.myUserInfo.userId,
                    displayName: this.myUserInfo.displayName
                };
                var domStr = _msgbox.getCloudFileMsgDomStr(uc.constants.Message.SendState.SendOK, true, from,
                    msgId, new Date().getTime(), name, size, "", new Date().getTime(), null,this.myUserInfo.displayName,cloudFileInfo,this.currentChatType);
                _msgbox.appendMessage(domStr);
                _msgbox.scrollToEnd();

                if(this.meetDetailChat != null){
                    if(this.meetDetailChat.msgBox != null){
                        this.meetDetailChat.msgBox.appendMessage(domStr);
                        this.meetDetailChat.msgBox.scrollToEnd();
                    }
                }
                uc.IUIEvent.OnMessageSending({
                    chatId: this.currentChatId,
                    chatType: this.currentChatType,
                    msgId:msgId,
                    contentType: uc.constants.Message.ContentType.CloudFile
                })
            }

        },
        downloadCloudDir:function(data,callback,type){
            if(!data.filename){
                uc.util.LogUtil.info('ChatMessage', 'downloadCloudDir', 'data = ' + JSON.stringify(data) + ', filename = ' + data.filename + '', null);
            }
            if(type=="receive"){
                var Obj= uc.util.FileUtil.getDefaultFileSavePath(data.filename);
                var fileObj = JSON.parse(Obj);
                var savePath =  fileObj[0];
                var info= $.extend(data,{"fullpath":savePath});
                uc.modules.cloud.CloudManager.downloadCloudFile(JSON.stringify(info),function(gkFileData){
                    callback&&callback(gkFileData,savePath);
                });
                return;
            }
            uc.util.FileDialogUtil.openSaveFileDialog(uc_resource.MessageBox.SaveFile, data.filename, [{ desc: uc_resource.MessageBox.AllFile, filter: '*.*'}],function(args){
                var savePath = args[0];
                if (savePath == null || savePath == "") {
                    return;
                }
                var info= $.extend(data,{"fullpath":savePath});
                uc.modules.cloud.CloudManager.downloadCloudFile(JSON.stringify(info),function(gkFileData){
                    callback&&callback(gkFileData,savePath);
                });
            });
        },
        fileListSaveCloud:function(dataStr){
            console.log("fileListSaveCloud:"+dataStr);
            if(!uc.modules.chat.ChatManager.checkCloudOpenStatus()){
                return;
            }
            if(!uc.modules.chat.ChatManager.checkCloudLoadStatus()){
                return;
            }
            uc.modules.cloud.CloudManager.saveCloudFile("",dataStr);
        },
        checkCloudOpenStatus:function(){
            if(uc.modules.cloud.CloudManager.checkCloudIsLoginSuccess()==0){
                uc.modules.component.PromptMessageBox.open({title: uc_resource.MessageBox.Notification, message: uc_resource.MessageBox.cloudLoadLimit, buttonMode:"ok",OkText:uc_resource.MessageBox.OK},
                    function(result){
                        this.close();
                    });
                return false;
            }
            return true;
        },
        checkCloudLoadStatus:function(){
            var value=uc.util.StorageUtil.getData('CloudLoginResult');
            if(value==null){
                uc.modules.component.PromptMessageBox.open({title: uc_resource.MessageBox.Notification, message: uc_resource.MessageBox.cloudLoadLoading, buttonMode:"ok",OkText:uc_resource.MessageBox.OK},
                    function(result){
                        this.close();
                    });
                return false;
            }else if(value==401141 || value==401142 || value==401143 || value==401144){
                uc.modules.component.PromptMessageBox.open({title: uc_resource.MessageBox.Notification, message: uc_resource.MessageBox.cloudLoadLimit, buttonMode:"ok",OkText:uc_resource.MessageBox.OK},
                    function(result){
                        this.close();
                    });
                return false;
            }else if(value!=0){
                uc.modules.component.PromptMessageBox.open({title: uc_resource.MessageBox.Notification, message: uc_resource.MessageBox.cloudLoadFail, buttonMode:"ok",OkText:uc_resource.MessageBox.OK},
                    function(result){
                        this.close();
                    });
                return false;
            }
            return true;
        },
        fileIdNotExistPrompt:function(){
            uc.modules.component.PromptMessageBox.open({title: uc_resource.MessageBox.Notification, message: uc_resource.MessageBox.fileIdNotExist, buttonMode:"ok",OkText:uc_resource.MessageBox.OK},
                function(result){
                    this.close();
                });
            return false;
        },
        noticeCloudFileDownloaded:function(data){
            if(this.msgBox){
                this.msgBox.OnChatCloudFileDownloaded(data);
            }
            if(this.fileList){
                this.fileList.OnGkFileDownloadCallBack(data);
            }
        },
        noticeCheckGkFilePermission:function(data,cloudInfo,type,that){
            if(this.msgBox){
                switch(type) {
                    case 'forward':
                        this.msgBox.forwardCloudFile(data,cloudInfo);
                        break;
                    case 'delete':
                        break;
                    case 'download':
                        this.msgBox.OnDownloadCloudFile(data,cloudInfo,that);
                        break;
                    case "cloudFilePreviewer":
                        this.msgBox.OnCloudFilePreviewer(data,cloudInfo,that);
                        break;
                    case "cloudView":
                        this.msgBox.OnGoToCloud(data,cloudInfo);
                        break;
                    default:
                        break;
                }
            }
        },
        noticeCheckGkFileExist:function(data,cloudData,option,Type){
            if(this.msgBox){
                if(Type=="cloudView"){
                    this.msgBox.OnGoToCloud(data,cloudData);
                }else if(Type=="comment"){
                    this.msgBox.OnCommentCloudFile(data,cloudData,option);
                }
            }
        },
        OnCloudLoginReceived:function(errorCode){
            uc.modules.chat.GroupChat.OnCloudLoginReceived(errorCode);
        },
        /************* IRemoteAssistanceEvent **************/
        OnRemoteAssistanceInvitationReceived: function(from, meetingId, attendeePassword, msgId, msgSeq, timestamp){
            if(this.currentChatId == from.userId && this.currentChatType == uc.constants.Chat.ChatType.SingleChat){
                uc.modules.remoteAssistance.RemoteAssistanceBanner.init({
                    userId: from.userId,
                    meetingId: meetingId,
                    attendeePassword: attendeePassword,
                    status: uc.constants.RemoteAssistanceStatus.INVITED
                });
            }
        },

        OnRemoteAssistanceEnded: function(channel, meetingId, msgId, msgSeq, timestamp){
            var userId = uc.modules.remoteAssistance.RemoteAssistanceManager.getUserIdByMeetingId(meetingId);
            if(this.currentChatId == userId && this.currentChatType == uc.constants.Chat.ChatType.SingleChat){
                uc.modules.chat.SingleChat.OnRemoteAssistanceEnded(meetingId, msgId, msgSeq, timestamp);
            }
        },
        scrollMessageBoxToEnd: function(){
            if(this.msgBox){
                this.msgBox.scrollToEnd();
            }
        },
        OnUserCallStatusReceived: function (errorCode, chatId, chatType, callStatus) {
            if(uc.modules.call.CallManager.inProgressCallInfo.isInProgress){
                return;
            }
            var This = this;
            if (errorCode == uc.ErrorCode.Success) {
                var user, userBusy = false, length = callStatus.length, busyUserList = [],busyUserId=[],selfStatus = -1,currentUserStatus =-1,currentUserStatusName;
                for (var i = 0; i < length; i++) {
                    user = callStatus[i];
                    if (user.status == 4 || user.status == 6) {
                        userBusy = true;
                        busyUserId.push(user.userId);
                        if (user.userId == this.myUserInfo.userId) {
                            selfStatus = user.status;
                            if(chatType != uc.constants.Chat.ChatType.SingleChat ) {
                                if(busyUserList.length == 0) {
                                    userBusy = false;
                                }
                            }
                            // busyUserList.push(uc_resource.SingleChat.Me);
                        } else {
                            var contactInfo = uc.IContactService.getBasicContactInfo(user.userId);
                            if (contactInfo) {
                                busyUserList.push(contactInfo.displayName);
                                currentUserStatusName = contactInfo.displayName;
                            } else {
                                busyUserList.push('<span class="callBusy" data-userId="' + user.userId + '"></span>');
                                currentUserStatusName = '<span class="callBusy" data-userId="' + user.userId + '"></span>';
                            }
                            currentUserStatus = user.status;
                        }
                    }
                }

                if (userBusy) {
                    if(chatType != uc.constants.Chat.ChatType.SingleChat){
                        var message = uc.util.StringUtil.formatString(uc_resource.Call.UserBusyCall, [busyUserList.join(",")]);
                        var userData={userBusy:userBusy,busyUserList:busyUserList,busyUserId:busyUserId};
                        uc.IUIService.triggerCrossWindowEvent(uc.constants.UI.CrossWindowEventName.chooseContactsCreateCall, {callStatus: uc.constants.UI.chooseCallContacts.createcall_fail,failedType:uc.constants.Call.CallFailedType.BusyUserStatus,failedData:userData,message:message});
                        return;
                    }else{

                        if(selfStatus == 6 || selfStatus == 4){
                            uc.modules.component.PromptMessageBox.open({
                                title: uc_resource.WindowTitle.Notification,
                                message: selfStatus == 6 ? uc_resource.Call.MaybeHasCallNotCall : uc_resource.Call.MaybeSelfInMeeting,
                                buttonMode: "both",
                                OkText: uc_resource.Common.OK,
                                CancelText:uc_resource.Common.Cancel
                            }, function (flag) {
                                this.close();
                                if(flag == 1) {//cancel
                                    return;
                                }else{
                                    //如果自己在通话中状态，弹出提示是否继续呼叫，再次确认检测其他人状态
                                    if(currentUserStatus == 6){
                                        var message = uc.util.StringUtil.formatString(uc_resource.Call.UserBusyCall, [currentUserStatusName]);
                                        uc.modules.component.PromptMessageBox.open({
                                            title: uc_resource.WindowTitle.Notification,
                                            message: message,
                                            buttonMode: "ok",
                                            OkText: uc_resource.Common.OK
                                        }, function () {
                                            this.close();
                                        });
                                        return;
                                    }/*else if(selfStatus == 4){
                                        uc.modules.component.PromptMessageBox.open({
                                            title: uc_resource.WindowTitle.Notification,
                                            message: uc_resource.Call.SelfInMeeting,
                                            buttonMode: "ok",
                                            OkText: uc_resource.Common.OK
                                        }, function () {
                                            this.close();
                                        });
                                        return;
                                    }*/else if(currentUserStatus == 4){
                                        uc.modules.component.PromptMessageBox.open({
                                            title: uc_resource.WindowTitle.Notification,
                                            message: uc_resource.Call.UserInMeeting,
                                            buttonMode: "ok",
                                            OkText: uc_resource.Common.OK
                                        }, function () {
                                            this.close();
                                        });
                                        return;
                                    }
                                    This.runStartCall(chatId,chatType);
                                }
                            });
                            return;
                        }else if(currentUserStatus == 6){
                            var message = uc.util.StringUtil.formatString(uc_resource.Call.UserBusyCall, [currentUserStatusName]);
                            uc.modules.component.PromptMessageBox.open({
                                title: uc_resource.WindowTitle.Notification,
                                message: message,
                                buttonMode: "ok",
                                OkText: uc_resource.Common.OK
                            }, function () {
                                this.close();
                            });
                            return;
                        }/*else if(selfStatus == 4){
                            uc.modules.component.PromptMessageBox.open({
                                title: uc_resource.WindowTitle.Notification,
                                message: uc_resource.Call.SelfInMeeting,
                                buttonMode: "ok",
                                OkText: uc_resource.Common.OK
                            }, function () {
                                this.close();
                            });
                            return;
                        }*/else if(currentUserStatus == 4){
                            uc.modules.component.PromptMessageBox.open({
                                title: uc_resource.WindowTitle.Notification,
                                message: uc_resource.Call.UserInMeeting,
                                buttonMode: "ok",
                                OkText: uc_resource.Common.OK
                            }, function () {
                                this.close();
                            });
                            return;
                        }
                    }
                }
                this.runStartCall(chatId,chatType);
            }else {
                switch (chatType){
                    case uc.constants.Chat.ChatType.SingleChat:
                        uc.modules.chat.SingleChat.showStartCallFailed();
                        break;
                    case uc.constants.Chat.ChatType.GroupChat:
                    case uc.constants.Chat.ChatType.ProjectChat:
                        uc.modules.chat.GroupChat.showStartCallFailed();
                        break;
                    default:
                        break;
                }
            }
        },
        runStartCall:function(chatId,chatType){
            if(chatId==this.myUserInfo.userId && chatType==uc.constants.Chat.ChatType.SingleChat){
                uc.modules.chat.GroupChat.setGroupCall();

            }else{
                var callWindowType;
                if (this.moduleId == uc.constants.UI.ModuleId.Message
                    && this.currentChatType == chatType
                    && this.currentChatId == chatId) {
                    callWindowType = uc.constants.Call.CallWindowType.EMBEDDED_CALL_WINDOW;
                } else {
                    callWindowType = uc.constants.Call.CallWindowType.FLOATING_CALL_WINDOW;
                }
                switch (chatType){
                    case uc.constants.Chat.ChatType.SingleChat:
                        uc.modules.call.CallManager.startCall({
                            loginUserId: this.myUserInfo.userId,
                            chatId: chatId,
                            hostId: this.myUserInfo.userId,
                            callStatus: uc.constants.Call.CallStatus.CONNECTING,
                            chatType: uc.constants.Chat.ChatType.SingleChat,
                            videoStatus: uc.constants.Call.VideoStatus.CLOSE,
                            callMeetingId: 0,
                            isInProgress: true
                        }, callWindowType, true);
                        break;
                    case uc.constants.Chat.ChatType.GroupChat:
                    case uc.constants.Chat.ChatType.ProjectChat:
                        uc.modules.chat.GroupChat.startCall(chatId, chatType,callWindowType);
                        break;
                    default:
                        break;
                }
            }
        },
        onRejectCallInvitationSending:function(chatType,userId,message){
            if (this.msgBox != null) {
                if (this.currentChatType == chatType &&
                    this.currentChatId == userId) {
                    this.onBeforeSend(null,message);
                }
            }
        },

        onCallDurationSending:function(chatType,channel, userId, duration, msgId, msgSeq, timestamp){
            if(chatType == uc.constants.Chat.ChatType.SingleChat){
	            if (this.myUserInfo == null) {
		            this.myUserInfo = uc.IContactService.getCurrentUserInfo();
	            }
                this.OnP2PCallDurationReceived(channel, userId,this.myUserInfo.userId, duration, msgId, msgSeq, timestamp);
            }else{
                this.OnGroupCallDurationReceived(channel, chatType,userId, duration, msgId, msgSeq, timestamp);
            }
        },
        /********************UserStatusEvent*******************************/
        OnUserStatusSetted: function(errorCode, userId, status) {
        },
        OnUserStatusReceived: function(errorCode, status) {
            uc.modules.chat.SingleChat.OnUserStatusReceived(errorCode, status);
        },
        OnExternalMessageReceived: function(errorCode, conversationType, relateID, msgCount, minTimestamp, messages){
            if (this.currentChatType == uc.constants.Chat.ChatType.SingleChat && this.currentChatId) {
                uc.modules.chat.SingleChat.OnExternalMessageReceived(errorCode, conversationType, relateID, msgCount, minTimestamp, messages);
            }

            if (this.currentChatType == uc.constants.Chat.ChatType.GroupChat && this.currentChatId) {
                uc.modules.chat.GroupChat.OnExternalMessageReceived(errorCode, conversationType, relateID, msgCount, minTimestamp, messages);
            }
        },
        OnModuleChanged: function () {
            if(this.meetingChat) {
                //this.meetingChat.getMoreHistoryMsg();
            }
        },
        noticeMeetingDocumentUploadMessageReceived: function (eventId,conversationType, channel, sendState, from, to, operatorId, fileId, fileName, fileSize, downloadUrl, fileFormat, desc, msgId, msgSeq, timestamp, cloudType, localPath) {

            if (this.meetingChat){
                uc.modules.chat.MeetingChat.OnMeetingDocumentUploadMessageReceived(eventId,conversationType, channel, sendState,  this.isSendByMyself(from), from, operatorId, fileId, fileName,
                    fileSize, downloadUrl, fileFormat, desc, msgId, msgSeq, timestamp, cloudType, localPath);
            }
            if (this.meetDetailChat) {
                this.meetDetailChat.OnMeetingDocumentUploadMessageReceived(eventId,conversationType, channel, sendState,  this.isSendByMyself(from), from, operatorId, fileId, fileName,
                    fileSize, downloadUrl, fileFormat, desc, msgId, msgSeq, timestamp, cloudType, localPath);
                // this.meetDetailChat.OnMeetingDocumentUploadMessageReceived(channel, sendState, this.isSendByMyself(from), from, eventId, meetingStartTime, fileName, fileSize, downloadUrl, fileFormat, msgId, msgSeq, timestamp);
            }
        } ,
        noticeMeetingDocumentDeleteMessageReceived: function (eventId,conversationType, channel, from, to, operatorId, fileId, fileName, msgId, msgSeq, msgTime) {
            if (this.meetingChat){
                uc.modules.chat.MeetingChat.OnDocumentDeleteMessageReceived(eventId,conversationType, channel, from, to, operatorId, fileId, fileName, msgId, msgSeq, msgTime);
            }
            if (this.meetDetailChat) {
                this.meetDetailChat.OnDocumentDeleteMessageReceived(eventId,conversationType, channel, from, to, operatorId, fileId, fileName, msgId, msgSeq, msgTime);
            }
        },
        isMyMsgChannel:function(channel)
        {
            if(channel ==uc.constants.Message.MessageChannel.UCAS ||
                channel==uc.constants.Message.MessageChannel.MessageList||
                channel==uc.constants.Message.MessageChannel.ChatHistory)
                return true;
            else
                return false;
        }

//        OnSelfJoinedCall:function(errorCode, meetingId, callType, chatId){
//            if(errorCode==uc.ErrorCode.Success){
//                switch(callType){
//                    case uc.constants.Chat.ChatType.SingleChat:
//                        if (!this.isCurrentMsg(uc.constants.Chat.ChatType.SingleChat, chatId,this.currentChatId)) {
//                            uc.modules.chat.SingleChat.OnSelfJoinedCallToPop(errorCode, meetingId, callType, chatId);
//                            break;
//                        }
//                        uc.modules.chat.SingleChat.OnSelfJoinedCall(errorCode, meetingId, callType, chatId);
//                        break;
//                    case uc.constants.Chat.ChatType.ProjectChat:
//                        uc.modules.chat.ProjectChat.OnSelfJoinedCall();
//                        break;
//                    case uc.constants.Chat.ChatType.GroupChat:
//                        uc.modules.chat.GroupChat.OnSelfJoinedCall();
//                        break;
//                }
//            }
//
//            if (!this.isCurrentMsg(uc.constants.Chat.ChatType.SingleChat, userId,this.currentChatId)) {
//                uc.modules.chat.SingleChat.OnP2PCallInvitationReceivedToPop(userId, confId, confPwd, hostId, msgId, msgSeq, timestamp);
//                return;
//            }
//            uc.modules.chat.SingleChat.OnP2PCallInvitationReceived(channel,userId, confId, confPwd, hostId, msgId, msgSeq, timestamp);
//        },
//        OnSelfLeftCall:function(errorCode, meetingId, , chatId, chatType){
//            if (!this.isCurrentMsg(uc.constants.Chat.ChatType.SingleChat, userId,this.currentChatId)) {
//                uc.modules.chat.SingleChat.OnP2PCallInvitationReceivedToPop(userId, confId, confPwd, hostId, msgId, msgSeq, timestamp);
//                return;
//            }
//            uc.modules.chat.SingleChat.OnP2PCallInvitationReceived(channel, userId, confId, confPwd, hostId, msgId, msgSeq, timestamp);
//        }
    };
})(uc);