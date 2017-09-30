/**
 * Created by senlin.ouyang on 2015/7/23.
 */
(function(uc){
    var newMsgCount = 0;
    var isChatWindowOpen = false;
    uc.modules.conversation.ConversationEventListener = function(){
        var loginUserInfo = uc.IClientService.getCurrentUCID();
        if(loginUserInfo){
            this.loginUserId = loginUserInfo.userId;
        }
    };
    jQuery.extend(uc.modules.conversation.ConversationEventListener.prototype,
        uc.interfaces.IUIEvent,
        uc.interfaces.IClientEvent,
        uc.interfaces.IContactEvent,
        uc.interfaces.IMeetingEvent,
        uc.interfaces.IGroupEvent,
        uc.interfaces.IIMEvent,
        uc.interfaces.IOpenApiEvent,
        uc.interfaces.IRemoteAssistanceEvent, {
            /** IUIEvent **/
            OnInitClient: function(clearData){
                uc.modules.conversation.ConversationManager.OnInitClient(clearData);
            },
            OnWindowResized: function(){
                uc.modules.conversation.ConversationManager.OnWindowResized();
            },
            OnModuleChanged: function (moduleId) {
                uc.modules.conversation.ConversationManager.OnModuleChanged(moduleId);
            },
            OnOpenChatWindow: function(chatType, chatId, openChatWindowSourceModule, meetingData, siteId){
                uc.modules.conversation.ConversationManager.OnOpenChatWindow(chatType, chatId, openChatWindowSourceModule, meetingData, siteId);
            },
            OnUserAvatarDownloaded: function(userId, serverAvatarUrl, localAvatarUrl){
                uc.modules.conversation.ConversationManager.OnUserAvatarDownloaded(userId, localAvatarUrl);
            },
            OnProjectAvatarDownloaded: function(groupId, serverAvatarUrl, localAvatarUrl){
                uc.modules.conversation.ConversationManager.OnProjectAvatarDownloaded(groupId, localAvatarUrl);
            },
            OnMessageSending: function(messageData){
                uc.modules.conversation.ConversationManager.OnMessageSending(messageData);
            },
            OnMessageFileUploading: function(chatId, chatType, taskId, contentType,messageData){
                uc.modules.conversation.ConversationManager.OnMessageFileUploading(chatId, chatType, taskId, contentType,messageData);
            },
            OnMessageFileUploadCancelled: function(chatId, chatType, taskId){
                uc.modules.conversation.ConversationManager.OnMessageFileUploadCancelled(chatId, chatType, taskId);
            },
            OnDraftMessageChanged: function(chatId, chatType, moduleId){
                uc.modules.conversation.ConversationManager.OnDraftMessageChanged(chatId, chatType, moduleId);
            },
            /**
             * event for call started
             * @param chatId
             * @param chatType
             */
            OnCallStarted: function(chatId, chatType){
                uc.modules.conversation.ConversationManager.OnCallStarted(chatId, chatType);
            },
            /**
             * event for call ended
             * @param chatId
             * @param chatType
             */
            OnCallEnded: function(chatId, chatType){
                uc.modules.conversation.ConversationManager.OnCallEnded(chatId, chatType);
            },
            /** IClientEvent **/
            OnConversationListReceived: function(/*int*/errorCode, /*boolean*/isLocal, /*Array*/conversations, totalUnread, groupAtCounts){
                uc.modules.conversation.ConversationManager.OnConversationListReceived(errorCode, isLocal, conversations, totalUnread, groupAtCounts);
            },
            OnConversationListComplete: function(/*int*/conversationCount){
                uc.modules.conversation.ConversationManager.OnConversationListComplete(conversationCount);
            },
            /** IContactEvent **/
            OnContactInfoReceived: function(errorCode, userIds, contacts){
                if(errorCode == uc.ErrorCode.Success && contacts && contacts.length){
                    uc.modules.conversation.ConversationManager.OnContactInfoReceived(contacts);
                }
            },
            OnContactUpdated: function(contacts){
                var userId = contacts[0].userId;
                var displayName = contacts[0].displayName;
                uc.modules.conversation.ConversationManager.updateUserNameInMessageAndTitle(userId, displayName)
            },
            OnBasicContactInfoReceived: function(errorCode, userIds, contacts){
                if(errorCode == uc.ErrorCode.Success && contacts && contacts.length){
                    uc.modules.conversation.ConversationManager.OnContactInfoReceived(contacts);
                }
            },
            OnDepartmentNameChanged: function(operatorId, departmentId, oldDepartmentName, newDepartmentName, description, timestamp){
                var conversationId = 0;
                var conversationType = uc.constants.Conversation.ConversationType.OrgMsg;
                var message =  uc.util.StringUtil.formatString(uc_resource.Message.DepartmentNameChanged, [newDepartmentName]);
                this.OnMessageReceived(0, conversationId, conversationType, message, timestamp, uc.constants.Message.MessageChannel.UCAS, timestamp, null, uc_resource.Message.DepartmentMessageName);
            },
            OnDepartmentTransferred: function(operatorId, userId, userName, departmentId, oldDepartmentName, departmentName, description, timestamp){
                var conversationId = 0;
                var conversationType = uc.constants.Conversation.ConversationType.OrgMsg;
                var message =  uc.util.StringUtil.formatString(uc_resource.Message.DepartmentTransferred, [departmentName]);
                this.OnMessageReceived(0, conversationId, conversationType, message, timestamp, uc.constants.Message.MessageChannel.UCAS, timestamp, null, uc_resource.Message.DepartmentMessageName);
            },
            OnPositionChanged: function(operatorId, userId, userName, newPosition, oldPosition, departmentName, description, timestamp){
                var conversationId = 0;
                var conversationType = uc.constants.Conversation.ConversationType.OrgMsg;
                var message =  uc.util.StringUtil.formatString(uc_resource.Message.PositionChanged, [newPosition]);
                this.OnMessageReceived(0, conversationId, conversationType, message, timestamp, uc.constants.Message.MessageChannel.UCAS, timestamp, null, uc_resource.Message.DepartmentMessageName);
            },
            OnEmployeeJoined: function(operatorId, userId, departmentId, userName, position, description, timestamp){
                var conversationId = 0;
                var conversationType = uc.constants.Conversation.ConversationType.OrgMsg;
                var message =  uc.util.StringUtil.formatString(uc_resource.Message.EmployeeJoined, [userName]);
                this.OnMessageReceived(0, conversationId, conversationType, message, timestamp, uc.constants.Message.MessageChannel.UCAS, timestamp, null, uc_resource.Message.DepartmentMessageName);
            },
            OnEmployeeLeft: function(operatorId, userId, userName, departmentName, description, timestamp){
                var conversationId = 0;
                var conversationType = uc.constants.Conversation.ConversationType.OrgMsg;
                var message =  uc.util.StringUtil.formatString(uc_resource.Message.EmployeeLeft, [userName]);
                this.OnMessageReceived(0, conversationId, conversationType, message, timestamp, uc.constants.Message.MessageChannel.UCAS, timestamp, null, uc_resource.Message.DepartmentMessageName);
            },
            OnEmployeeRightsChanged: function(operatorId, userId, userName, departmentId, departmentName, description, timestamp){
                var conversationId = 0;
                var conversationType = uc.constants.Conversation.ConversationType.OrgMsg;
                var message =  uc.util.StringUtil.formatString(uc_resource.Message.EmployeeRightsChanged, [departmentName]);
                this.OnMessageReceived(0, conversationId, conversationType, message, timestamp, uc.constants.Message.MessageChannel.UCAS, timestamp, null, uc_resource.Message.DepartmentMessageName);
            },
            OnJoinCompany: function(operatorId, companyId,companyName, userId, userName, description, timestamp){
                var conversationId = 0;
                var conversationType = uc.constants.Conversation.ConversationType.OrgMsg;
                var message =  uc.util.StringUtil.formatString(uc_resource.Message.JoinCompany, [companyName]);
                this.OnMessageReceived(0, conversationId, conversationType, message, timestamp, uc.constants.Message.MessageChannel.UCAS, timestamp, null, uc_resource.Message.DepartmentMessageName);
            },
            OnDepartmentParentChanged: function(operatorId, departmentId, departmentName, oldDepartmentName, newDepartmentName, description, timestamp){
                var conversationId = 0;
                var conversationType = uc.constants.Conversation.ConversationType.OrgMsg;
                var message =  uc.util.StringUtil.formatString(uc_resource.Message.DepartmentParentChanged, [oldDepartmentName, newDepartmentName]);
                this.OnMessageReceived(0, conversationId, conversationType, message, timestamp, uc.constants.Message.MessageChannel.UCAS, timestamp, null, uc_resource.Message.DepartmentMessageName);
            },

            OnDepartmentDeleted: function(operatorId, departmentId, departmentName, parentDepartmentName, description, timestamp){
                var conversationId = 0;
                var conversationType = uc.constants.Conversation.ConversationType.OrgMsg;
                var message =  uc.util.StringUtil.formatString(uc_resource.Message.DepartmentDeleted, [parentDepartmentName, departmentName]);
                this.OnMessageReceived(0, conversationId, conversationType, message, timestamp, uc.constants.Message.MessageChannel.UCAS, timestamp, null, uc_resource.Message.DepartmentMessageName);
            },
            OnEmployeeJoinConfirmed: function(operatorId, departmentId, departmentName, userName, userId, description, timestamp){
                var conversationId = 0;
                var conversationType = uc.constants.Conversation.ConversationType.OrgMsg;
                var message =  uc.util.StringUtil.formatString(uc_resource.Message.EmployeeJoinConfirmed, [userName]);
                this.OnMessageReceived(0, conversationId, conversationType, message, timestamp, uc.constants.Message.MessageChannel.UCAS, timestamp, null, uc_resource.Message.DepartmentMessageName);
            },
            OnEmployeeLeaveConfirmed: function(operatorId, departmentId,departmentName, userName, userId, description, timestamp){
                var conversationId = 0;
                var conversationType = uc.constants.Conversation.ConversationType.OrgMsg;
                var message =  uc.util.StringUtil.formatString(uc_resource.Message.EmployeeLeaveConfirmed, [userName]);
                this.OnMessageReceived(0, conversationId, conversationType, message, timestamp, uc.constants.Message.MessageChannel.UCAS, timestamp, null, uc_resource.Message.DepartmentMessageName);
            },
            OnDepartmentTransferConfirmed: function(operatorId, userId,userName, departmentName, description, timestamp){
                var conversationId = 0;
                var conversationType = uc.constants.Conversation.ConversationType.OrgMsg;
                var message =  uc.util.StringUtil.formatString(uc_resource.Message.DepartmentTransferConfirmed, [userName]);
                this.OnMessageReceived(0, conversationId, conversationType, message, timestamp, uc.constants.Message.MessageChannel.UCAS, timestamp, null, uc_resource.Message.DepartmentMessageName);
            },
            /** IIMEvent**/
            OnMessageSent: function(errorCode, userId, msgId, msgSeq, timestamp){
                uc.modules.conversation.ConversationManager.OnMessageSent(errorCode, userId, uc.constants.Conversation.ConversationType.SingleChat, msgId, msgSeq, timestamp);
            },
            OnTextMessageReceived: function(channel, sendState, from, to, msgId, msgSeq, textType, message, sendTime, timestamp){
                var conversationId = from.userId == this.loginUserId ? to.userId : from.userId;
                var conversationType = uc.constants.Conversation.ConversationType.SingleChat;
                var ackData = from.userId == this.loginUserId ? null : {to: from, msgId: msgId, msgSeq: msgSeq};
                message = uc.util.EmotionUtil.codeToDescription(message);
                //message = uc.modules.emoji.EmojiConverter.unicodeToEmotionTag(message);
                message = uc.modules.emoji.EmojiConverter.androidCodeToEmotionTag(message);

                this.OnMessageReceived(from.userId, conversationId, conversationType, message, timestamp, channel, msgId, ackData);
            },
            OnImageMessageReceived: function(channel, sendState, from, to, msgId, msgSeq, imageName, imageSize, imageURL, imageData, timestamp){
                var conversationId = from.userId == this.loginUserId ? to.userId : from.userId;
                var conversationType = uc.constants.Conversation.ConversationType.SingleChat;
                var ackData = from.userId == this.loginUserId ? null : {to: from, msgId: msgId, msgSeq: msgSeq};

                this.OnMessageReceived(from.userId, conversationId, conversationType,uc_resource.Message.ImageMessage, timestamp, channel, msgId, ackData);
            },
            OnAudioMessageReceived: function(channel, sendState, from, to, msgId, msgSeq, audioName, audioSize, audioURL, audioDuration, timestamp){
                var conversationId = from.userId == this.loginUserId ? to.userId : from.userId;
                var conversationType = uc.constants.Conversation.ConversationType.SingleChat;
                var ackData = from.userId == this.loginUserId ? null : {to: from, msgId: msgId, msgSeq: msgSeq};

                this.OnMessageReceived(from.userId, conversationId, conversationType, uc_resource.Message.AudioMessage, timestamp, channel, msgId, ackData);
            },
            OnVideoMessageReceived: function(channel, sendState, from, to, msgId, msgSeq, videoName, videoSize, videoURL, videoThumb, duration, timestamp){
                var conversationId = from.userId == this.loginUserId ? to.userId : from.userId;
                var conversationType = uc.constants.Conversation.ConversationType.SingleChat;
                var ackData = from.userId == this.loginUserId ? null : {to: from, msgId: msgId, msgSeq: msgSeq};

                this.OnMessageReceived(from.userId, conversationId, conversationType, uc_resource.Message.VideoMessage, timestamp, channel, msgId, ackData);
            },
            OnIMReportMessageReceived: function(channel, sendState, from, to, eventId, msgId, msgSeq, msgTime, reportTitle, reportUrl) {
                var conversationId = from.userId == this.loginUserId ? to.userId : from.userId;
                var conversationType = uc.constants.Conversation.ConversationType.SingleChat;
                var ackData = from.userId == this.loginUserId ? null : {to: from, msgId: msgId, msgSeq: msgSeq};
                var message = uc_resource.Message.MeetingReportMessage;//uc.util.EmotionUtil.codeToDescription(reportTitle);
                //message = "[" + uc_resource.Chat.MeetingReport + "] " + uc.modules.emoji.EmojiConverter.androidCodeToEmotionTag(reportTitle);
                this.OnMessageReceived(from.userId, conversationId, conversationType, message, msgTime, channel, msgId, ackData);
            },
            OnFileReceived: function(channel, sendState, from, to, msgId, msgSeq,fileName, fileSize, fileURL, timestamp,localPath){
                var conversationId = from.userId == this.loginUserId ? to.userId : from.userId;
                var conversationType = uc.constants.Conversation.ConversationType.SingleChat;
                var ackData = from.userId == this.loginUserId ? null : {to: from, msgId: msgId, msgSeq: msgSeq};

                this.OnMessageReceived(from.userId, conversationId, conversationType, uc_resource.Message.FileMessage, timestamp, channel, msgId, ackData);
            },
            OnCloudFileReceived: function(channel, sendState, from, to, msgId, msgSeq,fileName, fileSize, timestamp,detailContent){
                var conversationId = from.userId == this.loginUserId ? to.userId : from.userId;
                var conversationType = uc.constants.Conversation.ConversationType.SingleChat;
                var ackData = from.userId == this.loginUserId ? null : {to: from, msgId: msgId, msgSeq: msgSeq};
                this.OnMessageReceived(from.userId, conversationId, conversationType, uc_resource.Message.CloudFileMessage, timestamp, channel, msgId, ackData);
            },
            OnCodeReceived: function(channel, sendState, from, to, msgId, msgSeq, creatorId, creatorName, codeId, codeTitle, langType, codeHead, codeDesc, codeSize, codeUrl, codeLine, createTime, timestamp){
                var conversationId = from.userId == this.loginUserId ? to.userId : from.userId;
                var conversationType = uc.constants.Conversation.ConversationType.SingleChat;
                var ackData = from.userId == this.loginUserId ? null : {to: from, msgId: msgId, msgSeq: msgSeq};

                this.OnMessageReceived(from.userId, conversationId, conversationType, uc_resource.Message.CodeMessage, timestamp, channel, msgId, ackData);
            },
            OnFileShared: function(jid, msgId, msgSeq, fileName, fileSize, fileURL, fileId){},
            OnFileDeleted: function(jid, msgId, msgSeq, fileName, fileId){},
            OnMessageRead: function(errorCode, userId, readType, msgId, msgSeq){
                if(errorCode == uc.ErrorCode.Success){
                    this._OnMessageRead(uc.constants.Conversation.ConversationType.SingleChat, userId, readType);
                }
            },
            OnMessageMediaPlayed: function(errorCode, userId, msgId, msgSeq){},

            /**P2P call event**/
            OnP2PCallInvitationReceived: function (channel,userId,fromUserId, confId, confPwd, hostId, msgId, msgSeq, timestamp) {
                if(fromUserId == this.loginUserId || hostId == this.loginUserId || channel != uc.constants.Message.MessageChannel.UCAS){
                    return;
                }
                var conversationId = fromUserId == this.loginUserId ? hostId : fromUserId;
                var conversationType = uc.constants.Conversation.ConversationType.SingleChat;

                var ackData = fromUserId == this.loginUserId ? null : {to: hostId, msgId: msgId, msgSeq: msgSeq};
                var message = uc.util.StringUtil.formatString(uc_resource.Message.CallInvitationReceived, [this.getMessageUserName(hostId)]);

                this.OnMessageReceived(hostId, conversationId, conversationType, message, timestamp, channel, msgId, ackData);
            },
            OnP2PCallInvitationAccepted: function (channel,userId,fromUserId, confId, msgId, msgSeq, timestamp) {
            },
            OnP2PCallInvitationRejected: function (channel, userId, fromUserId,confId, hostId, reason, msgId, msgSeq, timestamp) {
                if(fromUserId == this.loginUserId || channel != uc.constants.Message.MessageChannel.UCAS){
                    return;
                }
                var conversationId = userId;
                var conversationType = uc.constants.Conversation.ConversationType.SingleChat;

                var ackData = fromUserId == this.loginUserId ? null : {to: this.loginUserId, msgId: msgId, msgSeq: msgSeq};
                var message;
                if (!reason) {
                    if (fromUserId == this.loginUserId) {
                        message = uc.util.StringUtil.formatString(uc_resource.Message.SelfRejectedCall, [this.getMessageUserName(fromUserId)]);
                    } else {
                        message = uc.util.StringUtil.formatString(uc_resource.Message.RejectedCall, [this.getMessageUserName(fromUserId)]);
                    }
                } else {
                    message = uc.util.EmotionUtil.codeToDescription(reason);
                    message = uc.modules.emoji.EmojiConverter.androidCodeToEmotionTag(message);
                }

                this.OnMessageReceived(fromUserId, conversationId, conversationType, message, timestamp, channel, msgId, ackData);
            },
            OnP2PCallStopped: function (channel, userId, confId, msgId, msgSeq, timestamp) {
            },
            OnP2PCallCancelled: function (channel, userId, fromUserId,msgId, msgSeq, timestamp) {
                if(fromUserId == this.loginUserId || channel != uc.constants.Message.MessageChannel.UCAS){
                    return;
                }
                var conversationId = userId;
                var conversationType = uc.constants.Conversation.ConversationType.SingleChat;

                var ackData = fromUserId == this.loginUserId ? null : {to: this.loginUserId, msgId: msgId, msgSeq: msgSeq};
                var message;
                if (fromUserId == this.loginUserId) {
                    message = uc.util.StringUtil.formatString(uc_resource.Message.SelfCancelCall, [this.getMessageUserName(fromUserId)]);
                } else {
                    message = uc.util.StringUtil.formatString(uc_resource.Message.CancelCall, [this.getMessageUserName(fromUserId)]);
                }

                this.OnMessageReceived(fromUserId, conversationId, conversationType, message, timestamp, channel, msgId, ackData);
            },
            OnP2PCallNoResponse: function (channel, userId, msgId, msgSeq, timestamp) {
            },
            OnP2PCallDurationReceived: function (channel, userId,fromUserId, duration, msgId, msgSeq, timestamp) {
                var conversationId = userId;
                var conversationType = uc.constants.Conversation.ConversationType.SingleChat;
                var ackData = fromUserId == this.loginUserId ? null : {to: this.loginUserId, msgId: msgId, msgSeq: msgSeq};
                var message;
                message = uc.util.StringUtil.formatString(uc_resource.Message.CallDuration, [uc.util.TimeUtil.getCallTimeStr(duration)]);


                this.OnMessageReceived(fromUserId, conversationId, conversationType, message, timestamp, channel, msgId, ackData);
            },

            /** IGroupEvent**/
            OnGroupMessageSent: function(errorCode, conversationType, groupId, msgId, msgSeq, timestamp){
                uc.modules.conversation.ConversationManager.OnMessageSent(errorCode, groupId, conversationType, msgId, msgSeq, timestamp);
            },
            OnGroupTextMessageReceived: function(conversationType, channel, sendState, from, to, msgId, msgSeq, textType, textContent, timestamp, atUsers){
                var conversationId = to.userId;
                var ackData = {to: to, msgId: msgId, msgSeq: msgSeq};
                var message = textContent;
                message = uc.util.EmotionUtil.codeToDescription(message);
                //message = uc.modules.emoji.EmojiConverter.unicodeToEmotionTag(message);
                message = uc.modules.emoji.EmojiConverter.androidCodeToEmotionTag(message);
                message = uc.util.StringUtil.formatString(uc_resource.Message.TextMessageWithSender, [this.getMessageUserName(from.userId), message]);

                this.OnMessageReceived(from.userId, conversationId, conversationType, message, timestamp, channel, msgId, ackData);
            },
            OnGroupImageMessageReceived: function(conversationType, channel, sendState, from, to, msgId, msgSeq, fileName, fileSize, fileURL, thumbnails, timestamp){
                var conversationId = to.userId;
                var ackData = {to: to, msgId: msgId, msgSeq: msgSeq};
                var message = uc.util.StringUtil.formatString(uc_resource.Message.ImageMessageWithSender, [this.getMessageUserName(from.userId)]);

                this.OnMessageReceived(from.userId, conversationId, conversationType, message, timestamp, channel, msgId, ackData);
            },
            OnGroupAudioMessageReceived: function(conversationType, channel, sendState, from, to, msgId, msgSeq, fileName, fileSize, fileURL, duration, timestamp){
                var conversationId = to.userId;
                var ackData = {to: to, msgId: msgId, msgSeq: msgSeq};
                var message = uc.util.StringUtil.formatString(uc_resource.Message.AudioMessageWithSender, [this.getMessageUserName(from.userId)]);

                this.OnMessageReceived(from.userId, conversationId, conversationType, message, timestamp, channel, msgId, ackData);
            },
            OnGroupVideoMessageReceived: function(conversationType, channel, sendState, from, to, msgId, msgSeq, fileName, fileSize, fileURL, thumbnails, duration, timestamp){
                var conversationId = to.userId;
                var ackData = {to: to, msgId: msgId, msgSeq: msgSeq};
                var message = uc.util.StringUtil.formatString(uc_resource.Message.VideoMessageWithSender, [this.getMessageUserName(from.userId)]);

                this.OnMessageReceived(from.userId, conversationId, conversationType, message, timestamp, channel, msgId, ackData);
            },
            OnGroupReportMessageReceived: function (conversationType, channel, sendState, from, to, eventId, msgId, msgSeq, msgTime, reportTitle, reportUrl) {
                var conversationId = to.userId;
                var ackData = {to: to, msgId: msgId, msgSeq: msgSeq};
                var message = uc.util.StringUtil.formatString(uc_resource.Chat.MeetingReport, [this.getMessageUserName(from.userId)]);//uc.util.EmotionUtil.codeToDescription(reportTitle);
                //message = uc.modules.emoji.EmojiConverter.androidCodeToEmotionTag(reportTitle);
                //message = "[" + uc_resource.Chat.MeetingReport + "] " + uc.util.StringUtil.formatString(uc_resource.Message.TextMessageWithSender, [this.getMessageUserName(from.userId), reportTitle]);

                this.OnMessageReceived(from.userId, conversationId, conversationType, message, msgTime, channel, msgId, ackData);
            },
            OnGroupMessageRead: function(errorCode, conversationType, groupId, readType, msgId, msgSeq){
                if(errorCode == uc.ErrorCode.Success){
                    this._OnMessageRead(conversationType, groupId, readType);
                }
            },
            OnGroupCloudFileMessageReceived: function(conversationType, channel, sendState, from, to, fileId, fileName, fileSize, downloadUrl, fileFormat, desc, msgId, msgSeq, timestamp, username,operation,detailContent){
                uc.util.LogUtil.info('Conversation', 'OnGroupCloudFileMessageReceived', 'detailContent = ' + detailContent + ', operation = ' + operation, null);
                if(!detailContent)return;
                var conversationId = to.userId;
                var ackData = {to: to, msgId: msgId, msgSeq: msgSeq},msgContent="",cloudFileInfo="";
                if(detailContent!=""){
                    cloudFileInfo= JSON.parse(detailContent);
                }
                switch (operation){
                    case uc.constants.Group.GroupCloudType.CloudFileCreate:
                        msgContent=cloudFileInfo.dir ? uc_resource.Message.UpdateCloudDir:uc_resource.Message.UpdateCloudFile;
                        break;
                    case uc.constants.Group.GroupCloudType.CloudFileUpdate:
                        msgContent=cloudFileInfo.dir ? uc_resource.Message.updateDir:uc_resource.Message.updateFile;
                        break;
                    case  uc.constants.Group.GroupCloudType.CloudFileDel:
                        msgContent=cloudFileInfo.dir ?uc_resource.Message.deleteDir:uc_resource.Message.deleteFile;
                        break;
                    case uc.constants.Group.GroupCloudType.CloudFileMove:
                        msgContent=cloudFileInfo.dir ?uc_resource.Message.moveDir:uc_resource.Message.moveFile;
                        break;
                    case uc.constants.Group.GroupCloudType.CloudFileRename:
                        msgContent=cloudFileInfo.dir ?uc_resource.Message.renameDir:uc_resource.Message.renameFile;
                        break;
                }
                var message = uc.util.StringUtil.formatString(msgContent,[this.getMessageUserName(from.userId), uc.util.StringUtil.escapeHtml(fileName)]);
                this.OnMessageReceived(from.userId, conversationId, conversationType, message, timestamp, channel, msgId, ackData);
            },
            OnGroupMessagePlayed: function(errorCode, userId, msgId, msgSeq){},
            OnGroupCallInvitationReceived: function(conversationType, groupId, hostId, confId, confPwd, msgId, msgSeq, timestamp){},
            OnGroupCallInvitationAccepted: function(conversationType, groupId, confId, msgId, msgSeq, timestamp){},
            OnGroupCallInvitationRejected: function(channel,conversationType, groupId, confId, reason,userId, msgId, msgSeq, timestamp){
                if(userId == this.loginUserId || channel != uc.constants.Message.MessageChannel.UCAS){
                    return;
                }
                var conversationId = groupId;
                var conversationType = conversationType;

                var ackData = userId == this.loginUserId ? null : {to: this.loginUserId, msgId: msgId, msgSeq: msgSeq};
                var message;
                if (!reason) {
                    if (userId == this.loginUserId) {
                        message = uc.util.StringUtil.formatString(uc_resource.Message.SelfRejectedFromGroupCall,null);
                    } else {
                        message = uc.util.StringUtil.formatString(uc_resource.Message.RejectedGroupCall, [this.getMessageUserName(userId)]);
                    }
                } else {
                    message = uc.util.EmotionUtil.codeToDescription(reason);
                    message = uc.modules.emoji.EmojiConverter.androidCodeToEmotionTag(message);
                }

                this.OnMessageReceived(userId, conversationId, conversationType, message, timestamp, channel, msgId, ackData);
            },
            // added by xue.bai_2 on 2016/04/07
            OnRevocationMessageReceived: function(channel, conversation, conversationType, from, to, msgId, msgSeq, msgTime){
                var conversationId = from.userId == this.loginUserId ? to.userId : from.userId;
                var message = uc.util.StringUtil.formatString(uc_resource.Message.RevocationMessage, [this.getMessageUserName(from.userId)]);
                var ackData = from.userId == this.loginUserId ? null : {to: this.loginUserId, msgId: msgId, msgSeq: msgSeq};

                if(conversationType == uc.constants.Conversation.ConversationType.SingleChat){
                    conversationId = from.userId == this.loginUserId ? to.userId : from.userId;
                }else{
                    conversationId = to.userId;
                }

                this.OnMessageReceived(from.userId, conversationId, conversationType, message, msgTime, channel, msgId, ackData);
            },
            OnMeetingMessageRevocationReceived: function(channel, conversation, conversationType, from, to, msgId, msgSeq, msgTime, seqId, eventId){
                var conversationId = from.userId == this.loginUserId ? to.userId : from.userId;
                var message = uc.util.StringUtil.formatString(uc_resource.Message.RevocationMessage, [this.getMessageUserName(from.userId)]);
                var ackData = from.userId == this.loginUserId ? null : {to: this.loginUserId, msgId: msgId, msgSeq: msgSeq};

                if(conversationType == uc.constants.Conversation.ConversationType.SingleChat){
                    conversationId = from.userId == this.loginUserId ? to.userId : from.userId;
                }else{
                    conversationId = to.userId;
                }

                this.OnMessageReceived(from.userId, conversationId, conversationType, message, msgTime, channel, msgId, ackData, '', eventId);
            },
            OnGroupCallStopped: function(conversationType, groupId, confId, msgId, msgSeq, timestamp){},
            OnGroupCallDurationReceived: function(channel,conversationType, groupId, duration, msgId, msgSeq, timestamp){
                var conversationId = groupId;
                var conversationType = conversationType;

                var ackData = groupId == this.loginUserId ? null : {to: this.loginUserId, msgId: msgId, msgSeq: msgSeq};
                var message;
                message = uc_resource.Message.CallEndMessage;//uc.util.StringUtil.formatString(uc_resource.Message.CallEndMessage, [uc.util.TimeUtil.getCallTimeStr(duration)]);


                this.OnMessageReceived(groupId, conversationId, conversationType, message, timestamp, channel, msgId, ackData);
            },

            OnGroupCreated: function(errorCode, operatorId, group){},
            OnGroupCreatedMessageReceived: function(conversationType, channel, from, to, operatorId, groupId, groupName, groupAvatar, members, msgId, msgSeq, timestamp){
                var conversationId = groupId;
                var ackData = {to: to, msgId: msgId, msgSeq: msgSeq};
                var resource = uc_resource.Message['InviteMemberTo' + (uc.constants.Conversation.type2Name[conversationType]||'Group')];
                var selfUserName=this.getMessageUserName(operatorId);
                var memberNameObj=this.getGroupMessageMembersName(operatorId, members);
                var getRole=this.getGroupMessageMembersRole(operatorId,members);
                var message = uc.util.StringUtil.formatString(resource, [selfUserName,memberNameObj]);
                if(getRole==1){
                    message=message+uc_resource.Message.setMemberAdmin;
                }

                this.OnMessageReceived(operatorId, conversationId, conversationType, message, timestamp, channel, msgId, ackData, groupName);
            },
            OnGroupExited: function(errorCode, groupId, userId){},
            OnGroupMemberExitedMessageReceived: function(conversationType, channel, from, to, operatorId, memberId, groupId, groupName, groupAvatar, msgId, msgSeq, timestamp){
                var conversationId = groupId;
                var ackData = {to: to, msgId: msgId, msgSeq: msgSeq};
                var resource = uc_resource.Message['MemberExit' + (uc.constants.Conversation.type2Name[conversationType]||'Group')];
                var message = uc.util.StringUtil.formatString(resource, [this.getMessageUserName(operatorId)]);

                this.OnMessageReceived(memberId, conversationId, conversationType, message, timestamp, channel, msgId, ackData, groupName);
                if(channel == uc.constants.Message.MessageChannel.UCAS){
                    uc.modules.conversation.ConversationManager.updateGroupAvatar(conversationId, conversationType, [{userId: memberId}]);
                }
            },
            OnGroupEnded: function(errorCode, groupId, operatorId){},
            OnGroupEndMessageReceived: function(conversationType, channel, from, to, operatorId, msgId, msgSeq, timestamp){
                var conversationId = to.userId;
                var ackData = {to: to, msgId: msgId, msgSeq: msgSeq};
                var resource = uc_resource.Message['End' + (uc.constants.Conversation.type2Name[conversationType]||'Group')];
                var message = uc.util.StringUtil.formatString(resource, [this.getMessageUserName(operatorId)]);

                this.OnMessageReceived(operatorId, conversationId, conversationType, message, timestamp, channel, msgId, ackData);
            },
            OnGroupMemberAdded: function(errorCode, channel, groupId, operatorId, memberIds){},
            OnGroupMemberAddedMessageReceived: function(conversationType, channel, from, to, operatorId, groupId, groupName, groupAvatar, members, msgId, msgSeq, timestamp ){
                var conversationId = groupId;
                var ackData = {to: to, msgId: msgId, msgSeq: msgSeq};
                var resource = uc_resource.Message['InviteMemberTo' + (uc.constants.Conversation.type2Name[conversationType]||'Group')];
                var getRole=this.getGroupMessageMembersRole(from.userId,members);
                var getUserName=this.getMessageUserName(operatorId);
                var getMemberName=this.getGroupMessageMembersName(operatorId, members);
                var getRole=this.getGroupMessageMembersRole(operatorId,members);
                var Title=[getUserName,getMemberName];
                var message = uc.util.StringUtil.formatString(resource,Title);
                if(getRole==1){
                    message=message+"、"+uc_resource.Message.setMemberAdmin;
                }

                this.OnMessageReceived(operatorId, conversationId, conversationType, message, timestamp, channel, msgId, ackData, groupName);
            },
            OnGroupMemberKicked: function(errorCode, channel, groupId, operatorId, memberIds){},
            GroupMemberKickedMessageReceived: function(conversationType, channel, from, to, operatorId, groupId, groupName, groupAvatar, members, msgId, msgSeq, timestamp){
                var conversationId = groupId;
                var ackData = {to: to, msgId: msgId, msgSeq: msgSeq};
                var resource, message;
                if(members.indexOf(this.loginUserId) == -1){
                    resource = uc_resource.Message['KickMemberFrom' + (uc.constants.Conversation.type2Name[conversationType]||'Group')];
                    message = uc.util.StringUtil.formatString(resource, [this.getMessageUserName(operatorId), this.getGroupMessageMembersName(operatorId, members)]);
                }else{
                    resource = uc_resource.Message['BeKickedFrom' + (uc.constants.Conversation.type2Name[conversationType]||'Group')];
                    message = uc.util.StringUtil.formatString(resource, [this.getMessageUserName(operatorId)]);
                }

                this.OnMessageReceived(operatorId, conversationId, conversationType, message, timestamp, channel, msgId, ackData, groupName);
                if(channel == uc.constants.Message.MessageChannel.UCAS){
                    uc.modules.conversation.ConversationManager.updateGroupAvatar(conversationId, conversationType, members);
                }
            },
            OnGroupNameUpdated: function(errorCode, channel, groupId, operatorId, groupName, pinyin){},
            OnGroupNameUpdatedMessageReceived: function(conversationType, channel, from, to, operatorId, groupId, groupName, namePinYin, msgId, msgSeq, timestamp){
                var conversationId = groupId;
                var ackData = {to: to, msgId: msgId, msgSeq: msgSeq};

                var resource = uc_resource.Message['Update' + (uc.constants.Conversation.type2Name[conversationType]||'Group') + 'Name'];
                var message = uc.util.StringUtil.formatString(resource, [this.getMessageUserName(operatorId), uc.util.StringUtil.escapeHtml(groupName)]);

                this.OnMessageReceived(operatorId, conversationId, conversationType, message, timestamp, channel, msgId, ackData, groupName);
            },
            OnGroupAvatarUpdated: function(errorCode, channel, groupId, logoURL){},
            OnGroupAvatarUpdatedMessageReceived: function(conversationType, channel, from, to, operatorId, groupId, groupAvatar, msgId, msgSeq, timestamp){
                uc.util.LogUtil.info('ConversationEventListener', 'OnGroupAvatarUpdatedMessageReceived', 'groupId = ' + groupId + ', groupAvatar = ' + groupAvatar, null);
                var conversationId = groupId;
                var ackData = {to: to, msgId: msgId, msgSeq: msgSeq};
                var resource = uc_resource.Message['Update' + (uc.constants.Conversation.type2Name[conversationType]||'Group') + 'Avatar'];
                var message = uc.util.StringUtil.formatString(resource, [this.getMessageUserName(operatorId)]);

                this.OnMessageReceived(operatorId, conversationId, conversationType, message, timestamp, channel, msgId, ackData);
                if(channel == uc.constants.Message.MessageChannel.UCAS){
                    uc.modules.conversation.ConversationManager.updateProjectAvatar(conversationId, conversationType, groupAvatar);
                }
            },
            OnDisturbSet: function(errorCode, groupId, isDisturb){},
            OnGroupDisturbSetMessageReceived: function(conversationType, channel, from, to, groupId, state, msgId, msgSeq, timestamp){
                //if(channel == uc.constants.Message.MessageChannel.MessageList){
                //    uc.modules.conversation.ConversationManager.removeConversation(groupId, conversationType);
                //}
                uc.util.LogUtil.info('ConversationEventListener', 'OnGroupDisturbSetMessageReceived', 'groupId = ' + groupId, null);
                var conversationId = groupId;
                var ackData = {to: to, msgId: msgId, msgSeq: msgSeq};
                var resource = conversationType == uc.constants.Conversation.ConversationType.GroupChat ? uc_resource.Message.UpdateGroupDisturbState : uc_resource.Message.UpdateProjectDisturbState;
                var message = uc.util.StringUtil.formatString(resource, [this.getMessageUserName(from.userId)]);

                var conversationInfo = uc.modules.conversation.ConversationManager.getConversation(conversationId, conversationType);
                if(conversationInfo){
                    conversationInfo.disturbStatus = state;
                    uc.modules.conversation.ConversationManager.updateConversation(conversationId, conversationType, conversationInfo);
                }

                this.OnMessageReceived(from.userId, conversationId, conversationType, message, timestamp, channel, msgId, ackData);
            },
            OnTopChatSet: function(errorCode, groupId, isTop){},
            OnGroupOnTopMessageReceived: function(conversationType, channel, from, to, operatorId, groupId, isTop, msgId, msgSeq, timestamp){
                uc.util.LogUtil.info('ConversationEventListener', 'OnGroupOnTopMessageReceived', 'groupId = ' + groupId, null);
                var conversationId = groupId;
                var ackData = {to: to, msgId: msgId, msgSeq: msgSeq};
                var resource = conversationType == uc.constants.Conversation.ConversationType.GroupChat ? uc_resource.Message.UpdateGroupOnTopState : uc_resource.Message.UpdateProjectOnTopState;
                var message = uc.util.StringUtil.formatString(resource, [this.getMessageUserName(from.userId)]);

                this.OnMessageReceived(from.userId, conversationId, conversationType, message, timestamp, channel, msgId, ackData);
            },
            OnGroupReminderSet: function(errorCode, groupId, isAlert, remindRule, remindTime){},
            OnGroupReminderSetMessageReceived: function(conversationType, channel, from, to, groupId, state, rule, time, msgId, msgSeq, timestamp){
                uc.util.LogUtil.info('ConversationEventListener', 'OnGroupReminderSetMessageReceived', 'groupId = ' + groupId, null);
                var conversationId = groupId;
                var ackData = {to: to, msgId: msgId, msgSeq: msgSeq};
                var resource = conversationType == uc.constants.Conversation.ConversationType.GroupChat ? uc_resource.Message.UpdateGroupReminderState : uc_resource.Message.UpdateProjectReminderState;
                var message = uc.util.StringUtil.formatString(resource, [this.getMessageUserName(from.userId)]);

                this.OnMessageReceived(from.userId, conversationId, conversationType, message, timestamp, channel, msgId, ackData);
            },
            OnGroupListReceived: function(errorCode, groupType, groups){},
            OnGroupInfoReceived: function(errorCode, groupId,mountId,group){
                if(errorCode == uc.ErrorCode.Success){
                    uc.modules.conversation.ConversationManager.OnGroupInfoReceived(groupId, group);
                }
            },
            OnGroupMemberRoleUpdatedMessageReceived:function(conversationType, channel, sendState, from, to, msgId, msgSeq, timestamp,roles){
                var conversationId = to.userId;
                var ackData = {to: to, msgId: msgId, msgSeq: msgSeq};
                var getRole=this.getGroupMessageMembersRole(from.userId,roles);
                var getMemberUserName=this.getCloudMessageMembersName(from.userId,roles);
                var message=getMemberUserName+uc_resource.Cloud.cancelAdmin;
                if(getRole==1){
                    message=getMemberUserName+uc_resource.Cloud.setAdmin;
                }
                this.OnMessageReceived(from.userId, conversationId, conversationType, message, timestamp, channel, msgId, ackData);
            },
            OnDocListReceived: function(errorCode, groupId, documents){},
            OnDocInfoReceived: function(errorCode, groupId, document){},
            OnDocumentUploaded: function(errorCode, groupId, operatorId, document, msgId){
                uc.modules.conversation.ConversationManager.OnDocumentUploaded(errorCode, groupId, operatorId, document, msgId);
            },
            OnMeetingFileUploaded: function(errorCode, eventId, operatorId, document, msgId){
                var meetings = uc.IMeetingService.getMeetingInfoByIdSync(eventId, 0, 0, 0);
                if (meetings && meetings.length) {
                    uc.modules.conversation.ConversationManager.OnDocumentUploaded(errorCode, meetings[0].groupId, operatorId, document, msgId);
                }
            },
            OnDocumentUploadMessageReceived: function(conversationType, channel, sendState, from, to, operatorId, fileId, fileName, fileSize, downloadUrl, fileFormat, desc, msgId, msgSeq, timestamp){
                var conversationId = to.userId;
                var ackData = {to: to, msgId: msgId, msgSeq: msgSeq};
                var message = uc.util.StringUtil.formatString(uc_resource.Message.UploadDocument, [this.getMessageUserName(operatorId), fileName]);

                this.OnMessageReceived(operatorId, conversationId, conversationType, message, timestamp, channel, msgId, ackData);
            },
            OnDocumentShared: function(errorCode, groupId, operatorId, document){},
            OnDocumentShareMessageReceived: function(conversationType, channel, from, to, operatorId, fileId, fileName, fileSize, downloadUrl, fileFormat, desc, msgId, msgSeq, timestamp){
                var conversationId = to.userId;
                var ackData = {to: to, msgId: msgId, msgSeq: msgSeq};
                var message = uc.util.StringUtil.formatString(uc_resource.Message.ForwardDocument, [this.getMessageUserName(operatorId), fileName]);

                this.OnMessageReceived(operatorId, conversationId, conversationType, message, timestamp, channel, msgId, ackData);
            },
            OnDocumentDeleted: function(errorCode, groupId, operatorId, docName, fileId){},
            OnDocumentDeleteMessageReceived: function(conversationType, channel, from, to, operatorId, fileId, fileName, msgId, msgSeq, timestamp){
                var conversationId = to.userId;
                var ackData = {to: to, msgId: msgId, msgSeq: msgSeq};
                var message = uc.util.StringUtil.formatString(uc_resource.Message.DeleteDocument, [this.getMessageUserName(operatorId), fileName]);

                this.OnMessageReceived(operatorId, conversationId, conversationType, message, timestamp, channel, msgId, ackData);
            },
            OnCommentCreateMessageReceived:function(conversationType, channel, from, to, msgId, msgSeq, fileId, fileType, fileTitle, commentId, userName, userId, content, createTime, timestamp){
                var conversationId = to.userId;
                var ackData = {to: to, msgId: msgId, msgSeq: msgSeq};
                var message = uc.util.StringUtil.formatString(uc_resource.Message.CommentMessage, [this.getMessageUserName(userId), fileTitle]);

                this.OnMessageReceived(userId, conversationId, conversationType, message, timestamp, channel, msgId, ackData);
            },
            OnCodeSnippetUploaded:function(errorCode, groupId, operatorId, document, msgId){
                uc.modules.conversation.ConversationManager.OnDocumentUploaded(errorCode, groupId, operatorId, document, msgId);
            },
            OnCodeSnippetUploadMessageReceived: function(conversationType, channel, sendState, from, to, msgId, msgSeq, creatorId, creatorName, codeId, codeTitle,
                                                         langType, codeHead, codeDesc, codeSize, codeUrl, codeLine, createTime, timestamp){
                var conversationId = to.userId;
                var ackData = {to: to, msgId: msgId, msgSeq: msgSeq};
                var message = uc.util.StringUtil.formatString(uc_resource.Message.CodeMessageWithSender, [this.getMessageUserName(creatorId)]);

                this.OnMessageReceived(creatorId, conversationId, conversationType, message, timestamp, channel, msgId, ackData);
            },
            /** IMeetingEvent **/
            OnMeetingCreated: function(/*int*/errorCode,meetingInfo){},
            OnMeetingUpdated: function(/*int*/errorCode,eventId,startTime,meetingInfo){},
            OnMeetingCanceled: function(/*int*/errorCode,eventId,authorizerId,startTime){

            },
            OnMeetingDocumentUploadMessageReceived:function(eventId,conversationType, channel, sendState, from, to, operatorId, fileId, fileName, fileSize, downloadUrl, fileFormat, desc, msgId, msgSeq, timestamp,localPath){
                var conversationId = to.userId;
                var ackData = {to: to, msgId: msgId, msgSeq: msgSeq};
                // evetnId = 20297;
                var message = uc.util.StringUtil.formatString(uc_resource.Message.UploadDocument, [this.getMessageUserName(operatorId), fileName]);
                //this.OnMessageReceived(from.userId, conversationId, conversationType, message, timestamp, channel, msgId, ackData, meetingName,  eventId, 0, meetingStartTime, uc.constants.Meeting.MsgType.Text);
                this.OnMessageReceived(from.userId, conversationId, conversationType, message, timestamp, channel, msgId, ackData, '',  eventId, 0);
            },

            OnMeetingFileDeleteMessageReceived: function(eventId,conversationType, channel, from, to, operatorId, fileId, fileName, msgId, msgSeq, timestamp){
                var conversationId = to.userId;
                var ackData = {to: to, msgId: msgId, msgSeq: msgSeq};
                var message = uc.util.StringUtil.formatString(uc_resource.Message.DeleteDocument, [this.getMessageUserName(operatorId), fileName]);

                this.OnMessageReceived(operatorId, conversationId, conversationType, message, timestamp, channel, msgId, ackData);
            },
            //OnMeetingStartUrlReceived: function(/*int*/errorCode,startUrl,eventId){},
            OnMeetingWebStartUrlReceived:function(errorCode,startUrl,eventId){},
            OnMeetingListReceived: function(errorCode,requestId,startTime,endTime,eventIdFilter,authorizerId,meetings){},
            OnMeetingInvitationAccepted:function(errorCode, eventId, startTime, authorizerId){},
            OnMeetingInvitationRejected:function(errorCode, eventId, startTime, authorizerId){
                //var conversationId = meetingInfo.groupId;
                //var conversationType = uc.constants.Conversation.ConversationType.MeetingMsg;
                //uc.modules.conversation.ConversationManager.removeConversation(conversationId, conversationType);
            },
            OnAccreditUserListReceived:function(errorCode,accreditUsers){},
            OnAccreditSettingUpdated:function(errorCode,shareUserId,status){},
            OnMeetingInfoReceivedById:function(errorCode,eventId, authorizerId, meetingInfoArray){
                if(errorCode == uc.ErrorCode.Success && meetingInfoArray && meetingInfoArray.length){
                    uc.modules.conversation.ConversationManager.OnMeetingInfoArrayReceived(meetingInfoArray);
                }
            },
            OnMeetingInfoReceivedByIds:function(errorCode,requestId,meetingInfoArray){
                if(errorCode == uc.ErrorCode.Success && meetingInfoArray && meetingInfoArray.length){
                    uc.modules.conversation.ConversationManager.OnMeetingInfoArrayReceived(meetingInfoArray);
                }
            },
            OnMeetingForwarded:function(errorCode,meetingInfo){},
            OnUserScheduleListReceived:function(errorCode,requestId,schedules){},
            OnMinutesCreated:function(errorCode,eventId,startTime,minutesId,minutesTitle){},
            OnMinutesUpdated:function(errorCode,eventId,minutesId,minutesTitle){},
            OnMinutesDetailsReceived:function(errorCode,eventId,minutesId,creatorId,createTime,content,startTime){},
            OnMinutesListReceived:function(errorCode,eventId,summaryIds){},
            OnMeetingMessageSent: function(errorCode, groupId, eventId, meetingStartTime, msgId, msgSeq, timestamp){
                uc.modules.conversation.ConversationManager.OnMessageSent(errorCode, groupId, uc.constants.Conversation.ConversationType.MeetingMsg, msgId, msgSeq, timestamp);
            },
            OnMeetingChatHistoryReceived: function(errorCode, groupId){},
            OnMeetingTextMessageReceived: function(channel, sendState, from, to, meetingId, eventId, meetingStartTime, textType, textContent, msgId, msgSeq, timestamp){
                var conversationId = to.userId;
                var conversationType = uc.constants.Conversation.ConversationType.MeetingMsg;
                var ackData = {to: to, msgId: msgId, msgSeq: msgSeq};
                var message = textContent;
                message = uc.util.EmotionUtil.codeToDescription(message);
                //message = uc.modules.emoji.EmojiConverter.unicodeToEmotionTag(message);
                message = uc.modules.emoji.EmojiConverter.androidCodeToEmotionTag(message);
                message = uc.util.StringUtil.formatString(uc_resource.Message.TextMessageWithSender, [this.getMessageUserName(from.userId), message]);

                this.OnMessageReceived(from.userId, conversationId, conversationType, message, timestamp, channel, msgId, ackData, '',  eventId, 0, meetingStartTime, uc.constants.Meeting.MsgType.Text);
            },
            OnMeetingImageMessageReceived: function(channel, sendState, from, to, meetingId, eventId, meetingStartTime, fileName, fileSize, fileURL, thumbnails, msgId, msgSeq, timestamp){
                var conversationId = to.userId;
                var conversationType = uc.constants.Conversation.ConversationType.MeetingMsg;
                var ackData = {to: to, msgId: msgId, msgSeq: msgSeq};
                var message = uc.util.StringUtil.formatString(uc_resource.Message.ImageMessageWithSender, [this.getMessageUserName(from.userId)]);

                this.OnMessageReceived(from.userId, conversationId, conversationType,message, timestamp, channel, msgId, ackData, '', eventId, 0, meetingStartTime, uc.constants.Meeting.MsgType.Image);
            },
            OnMeetingAudioMessageReceived: function(channel, sendState, from, to, meetingId, eventId, meetingStartTime, fileName, fileSize, fileURL, duration, msgId, msgSeq,timestamp){
                var conversationId = to.userId;
                var conversationType = uc.constants.Conversation.ConversationType.MeetingMsg;
                var ackData = {to: to, msgId: msgId, msgSeq: msgSeq};
                var message = uc.util.StringUtil.formatString(uc_resource.Message.AudioMessageWithSender, [this.getMessageUserName(from.userId)]);

                this.OnMessageReceived(from.userId, conversationId, conversationType, message, timestamp, channel, msgId, ackData, '', eventId, 0, meetingStartTime, uc.constants.Meeting.MsgType.Audio);
            },
            OnMeetingVideoMessageReceived: function(channel, sendState, from, to, meetingId, eventId, meetingStartTime, fileName, fileSize, fileURL, thumbnails, duration, msgId, msgSeq, timestamp){
                var conversationId = to.userId;
                var conversationType = uc.constants.Conversation.ConversationType.MeetingMsg;
                var ackData = {to: to, msgId: msgId, msgSeq: msgSeq};
                var message = uc.util.StringUtil.formatString(uc_resource.Message.MeetingVideoMessageWithSender, [this.getMessageUserName(from.userId)]);

                this.OnMessageReceived(from.userId, conversationId, conversationType, message, timestamp, channel, msgId, ackData, '', eventId, 0, meetingStartTime, uc.constants.Meeting.MsgType.Video);
            },
            OnMeetingReportMessageReceived: function (channel, sendState, from, to, eventId, msgId, msgSeq, msgTime, reportTitle, reportUrl) {
                var conversationId = to.userId;
                var conversationType = uc.constants.Conversation.ConversationType.MeetingMsg;
                var ackData = {to: to, msgId: msgId, msgSeq: msgSeq};
                var message = uc_resource.Message.MeetingReportMessage;//uc.util.EmotionUtil.codeToDescription(reportTitle);
                //message =  uc_resource.Chat.MeetingReportMessage + uc.modules.emoji.EmojiConverter.androidCodeToEmotionTag(reportTitle);

                this.OnMessageReceived(from.userId, conversationId, conversationType, message, msgTime, channel, msgId, ackData, '', eventId, 0, 0, uc.constants.Meeting.MsgType.Text);
            },
            OnMeetingInviteMessageReceived: function(channel, from, to, meetingId, eventId, meetingName, accreditUserId, hostId, isRecurrent, meetingStartTime, msgId, msgSeq, timestamp){
                var conversationId = to.userId;
                if(this.loginUserId == to.userId){
                    conversationId = from.userId;
                }
                if(conversationId == 0){
                    return;
                }
                var conversationType = uc.constants.Conversation.ConversationType.MeetingMsg;
                if(accreditUserId != 0){//别人授权给我的会议，不需要显示
                    if(channel == uc.constants.Message.MessageChannel.MessageList){
                        uc.modules.conversation.ConversationManager.removeConversation(conversationId, conversationType);
                    }
                    return;
                }
                var message;
                if(hostId == this.loginUserId){
                    if(isRecurrent){
                        message = uc_resource.Message.CreateRecurrentMeetingByMe;
                    }else{
                        message = uc_resource.Message.CreateMeetingByMe;
                    }
                    if(channel == uc.constants.Message.MessageChannel.MessageList){
                        uc.modules.conversation.ConversationManager.removeConversation(conversationId, conversationType);
                    }
                    return; //If meeting is created by Me, do not show it
                }else{
                    if(isRecurrent){
                        message = uc.util.StringUtil.formatString(uc_resource.Message.BeInvitedToRecurrentMeeting, [this.getMessageUserName(hostId)]);
                    }else{
                        message = uc.util.StringUtil.formatString(uc_resource.Message.BeInvitedToMeeting, [this.getMessageUserName(hostId)]);
                    }
                }
                var ackData = {to: to, msgId: msgId, msgSeq: msgSeq};

                this.OnMessageReceived(from.userId, conversationId, conversationType, message, timestamp, channel, msgId, ackData, meetingName, eventId, accreditUserId, meetingStartTime, uc.constants.Meeting.MsgType.Invitation);
                if(this.needUpdateMeetingStatus(conversationId, conversationType, timestamp)){
                    uc.modules.conversation.ConversationManager.setMeetingStatus(conversationId, conversationType, 0);
                }
            },
            OnMeetingAcceptMessageReceived: function(channel, from, to, meetingId, eventId, meetingName, accreditUserId, hostId, isRecurrent, meetingStartTime, msgId, msgSeq, msgTime){

                var conversationId = to.userId;
                var conversationType = uc.constants.Conversation.ConversationType.MeetingMsg;
                var msg;

                // server暂不支持多次接受会议，对此处逻辑替换。
                //if(isRecurrent){
                //    message = uc.util.StringUtil.formatString(uc_resource.Message.RecurrentMeetingInvitationAccepted, [this.getMessageUserName(from.userId)]);
                //}else if(accreditUserId != 0){
                //    message = uc_resource.Message.AcceptMeetingInvitation;
                //    if(channel == uc.constants.Message.MessageChannel.MessageList){
                //        uc.modules.conversation.ConversationManager.removeConversation(conversationId, conversationType);
                //    }
                //    //return; //If meeting is accepted by Me, do not show it
                //}else if (from.userId == this.loginUerId) {
                //    message = uc.util.StringUtil.formatString(uc_resource.Message.AcceptMeetingInvitation, [this.getMessageUserName(hostId)]);
                //}else{
                //    message = uc.util.StringUtil.formatString(uc_resource.Message.MeetingInvitationAccepted, [this.getMessageUserName(from.userId)]);
                //}
                if(accreditUserId != 0){//别人授权给我的会议，不需要显示
                    if(channel == uc.constants.Message.MessageChannel.MessageList){
                        uc.modules.conversation.ConversationManager.removeConversation(conversationId, conversationType);
                    }
                    return;
                }


                //屏蔽收到与会者执行接受操作消息，如果上一条conversationInfo.message == “”，删除当前conversationInfo
                var conversationInfo = uc.modules.conversation.ConversationManager.getConversation(conversationId, conversationType);
                if(conversationInfo && conversationInfo.timestamp <= msgTime) {
                    if(conversationInfo.message == "" && channel == uc.constants.Message.MessageChannel.MessageList){
                        uc.modules.conversation.ConversationManager.removeConversation(conversationId, conversationType);
                    }
                }
                return ;

                if(isRecurrent){
                    msg = uc.util.StringUtil.formatString(uc_resource.Message.RecurrentMeetingInvitationAccepted, [this.getMessageUserName(from.userId)]);
                }else if(from.userId == this.loginUserId){
                    msg = uc_resource.Message.AcceptMeetingInvitation;
                    if(channel == uc.constants.Message.MessageChannel.MessageList){
                        uc.modules.conversation.ConversationManager.removeConversation(conversationId, conversationType);
                    }
                    return; //If meeting is accepted by Me, do not show it
                }else{
                    msg = uc.util.StringUtil.formatString(uc_resource.Message.MeetingInvitationAccepted, [this.getMessageUserName(from.userId)]);
                }
                var ackData = {to: to, msgId: msgId, msgSeq: msgSeq};

                this.OnMessageReceived(from.userId, conversationId, conversationType, msg, msgTime, channel, msgId, ackData, meetingName, eventId, accreditUserId, meetingStartTime, uc.constants.Meeting.MsgType.InvitationAccepted);
            },
            OnMeetingRejectMessageReceived: function(channel, from, to, meetingId, eventId, meetingName, accreditUserId, hostId, isRecurrent, meetingStartTime, msgId, msgSeq, msgTime){
                var conversationId = to.userId;
                var conversationType = uc.constants.Conversation.ConversationType.MeetingMsg;
                if(accreditUserId !=0){
                    return;
                }

                //屏蔽收到与会者执行接受操作消息，如果上一条conversationInfo.message == “”，删除当前conversationInfo
                var conversationInfo = uc.modules.conversation.ConversationManager.getConversation(conversationId, conversationType);
                if(conversationInfo && conversationInfo.timestamp <= msgTime) {
                    if(conversationInfo.message == "" && channel == uc.constants.Message.MessageChannel.MessageList){
                        //uc.modules.conversation.ConversationManager.removeConversation(conversationId,
                        // conversationType);
                    }
                }
                //return ;

                var msg;
                if(from.userId == this.loginUserId){
                    msg = uc.util.StringUtil.formatString(uc_resource.Message.RejectMeetingInvitation, [this.getMessageUserName(hostId)]);
                    if(channel == uc.constants.Message.MessageChannel.MessageList){
                        uc.modules.conversation.ConversationManager.removeConversation(conversationId, conversationType);
                    }
                    //return; //If meeting is rejected by Me, do not show it
                }else{
                    msg = uc.util.StringUtil.formatString(uc_resource.Message.MeetingInvitationRejected, [this.getMessageUserName(from.userId)]);
                }
                var ackData = {to: to, msgId: msgId, msgSeq: msgSeq};

                this.OnMessageReceived(from.userId, conversationId, conversationType, msg, msgTime, channel, msgId, ackData, meetingName, eventId, accreditUserId, meetingStartTime, uc.constants.Meeting.MsgType.InvitationRejected);
            },
            OnMeetingUpdateMessageReceived: function(channel, from, to, meetingId, eventId, meetingName, accreditUserId, hostId, isRecurrent, meetingStartTime, updateFlag, msgId, msgSeq, msgTime){
                if (channel != uc.constants.Message.MessageChannel.UCAS) {
                    //return;
                }

                var conversationId = to.userId, conversationInfo;
                var conversationType = uc.constants.Conversation.ConversationType.MeetingMsg;

                var meetingInfoArray = uc.IMeetingService.getMeetingInfoByIdSync(/* meetingId, */ eventId, 0, 0, accreditUserId);
                if(meetingInfoArray && meetingInfoArray.length){
                    conversationInfo = uc.modules.conversation.ConversationManager.getConversation(conversationId, conversationType);
                    if(conversationInfo && conversationInfo.timestamp <= msgTime) {
                        conversationInfo.title = meetingName;
                        conversationInfo.meetingStartTime = meetingStartTime;
                        conversationInfo.isGNet = meetingInfoArray[0].isGNet;
                        uc.modules.conversation.ConversationManager.updateConversation(conversationId, conversationType, conversationInfo);
                    }
                }
                var msg;
                if(hostId == this.loginUserId || from.userId == this.loginUserId || accreditUserId != 0
                    || (updateFlag != uc.constants.Meeting.UpdateFlag.MeetingInfoChanged && updateFlag != uc.constants.Meeting.UpdateFlag.AttendeeAndMeetingInfoChanged)){
                    msg = uc_resource.Message.MeetingUpdatedByMe;
                    if(channel == uc.constants.Message.MessageChannel.MessageList){
                        uc.modules.conversation.ConversationManager.removeConversation(conversationId, conversationType);
                    }

                    return;//If meeting is created by Me/update by Me/or accredit meeting, do not show it
                }else{
                    msg = uc.util.StringUtil.formatString(uc_resource.Message.MeetingUpdatedByOther, [this.getMessageUserName(hostId)]);
                }
                var ackData = {to: to, msgId: msgId, msgSeq: msgSeq};

                this.OnMessageReceived(from.userId, conversationId, conversationType, msg, msgTime, channel, msgId, ackData, meetingName, eventId, accreditUserId, meetingStartTime, uc.constants.Meeting.MsgType.Updated);
            },
            OnMeetingForwardMessageReceived: function(channel, from, to, meetingId, eventId, meetingName, accreditUserId, hostId, isRecurrent, meetingStartTime, msgId, msgSeq, msgTime){
                var conversationId = to.userId;
                var conversationType = uc.constants.Conversation.ConversationType.MeetingMsg;
                var msg;
                if(from.userId == this.loginUserId){
                    msg = uc.util.StringUtil.formatString(uc_resource.Message.MeetingForwardedByMe, [this.getMessageUserName(hostId)]);
                    if(channel == uc.constants.Message.MessageChannel.MessageList){
                        uc.modules.conversation.ConversationManager.removeConversation(conversationId, conversationType);
                    }
                    return; //If meeting is forwarded by Me, do not show it
                }else if(hostId == this.loginUserId){
                    msg = uc.util.StringUtil.formatString(uc_resource.Message.ForwardMyMeeting, [this.getMessageUserName(from.userId)]);
                    if(channel == uc.constants.Message.MessageChannel.MessageList){
                        uc.modules.conversation.ConversationManager.removeConversation(conversationId, conversationType);
                    }
                    return; //If meeting is created by Me, do not show it
                }else if(accreditUserId != 0){
                    return;
                } else{
                    msg = uc.util.StringUtil.formatString(uc_resource.Message.ForwardMeetingToMe, [this.getMessageUserName(from.userId)]);
                }
                var ackData = {to: to, msgId: msgId, msgSeq: msgSeq};

                this.OnMessageReceived(from.userId, conversationId, conversationType, msg, msgTime, channel, msgId, ackData, meetingName, eventId, accreditUserId, meetingStartTime, uc.constants.Meeting.MsgType.Forwarded);
                if(this.needUpdateMeetingStatus(conversationId, conversationType, msgTime)){
                    uc.modules.conversation.ConversationManager.setMeetingStatus(conversationId, conversationType, 0);
                }
            },
            OnMeetingCancelMessageReceived: function(channel, from, to, meetingId, eventId, meetingName, groupId, accreditUserId, hostId, isRecurrent, meetingStartTime, msgId, msgSeq, msgTime){
                var conversationId = groupId;
                if(to.userId != groupId){//If to is not group, it should be send from accredit user or to accredit user
                    conversationId = this.loginUserId == from.userId ? to.userId : from.userId;
                }
                var conversationType = uc.constants.Conversation.ConversationType.MeetingMsg;
                if(hostId == this.loginUserId || from.userId == this.loginUserId || accreditUserId != 0){
                    if(channel == uc.constants.Message.MessageChannel.MessageList){
                        uc.modules.conversation.ConversationManager.removeConversation(conversationId, conversationType);
                        return;//If meeting is cancelled by Me, do not show it
                    }
                    if(this.needUpdateMeetingStatus(conversationId, conversationType, msgTime)){
                        uc.modules.conversation.ConversationManager.setMeetingStatus(conversationId, conversationType, 1);
                    }
                    return;
                }
                var ackData = {to: to, msgId: msgId, msgSeq: msgSeq};
                var msg = uc.util.StringUtil.formatString(uc_resource.Message.CancelMeeting, [this.getMessageUserName(from.userId)]);

                this.OnMessageReceived(from.userId, conversationId, conversationType, msg, msgTime, channel, msgId, ackData, meetingName, eventId,accreditUserId, meetingStartTime, uc.constants.Meeting.MsgType.Cancelled);
                if(this.needUpdateMeetingStatus(conversationId, conversationType, msgTime)){
                    uc.modules.conversation.ConversationManager.setMeetingStatus(conversationId, conversationType, 1);
                }
            },
            OnMinutesCreateMessageReceived: function(channel, from, to, operatorId, minutesId, minutesTitle, meetingId,eventId, meetingName, meetingStartTime, updateTime, msgId, msgSeq, timestamp){
                var conversationId = to.userId;
                var conversationType = uc.constants.Conversation.ConversationType.MeetingMsg;
                var ackData = {to: to, msgId: msgId, msgSeq: msgSeq};
                var message = uc.util.StringUtil.formatString(uc_resource.Message.CreateMeetingSummary, [this.getMessageUserName(from.userId)]);

                this.OnMessageReceived(from.userId, conversationId, conversationType, message, timestamp, channel, msgId, ackData, meetingName, meetingId, eventId, 0, meetingStartTime, uc.constants.Meeting.MsgType.SummaryCreated);
            },
            OnMinutesUpdateMessageReceived: function(channel, from, to, operatorId,  minutesId, minutesTitle,meetingId, eventId, meetingName, meetingStartTime, updateTime, msgId, msgSeq, timestamp){
                var conversationId = to.userId;
                var conversationType = uc.constants.Conversation.ConversationType.MeetingMsg;
                var ackData = {to: to, msgId: msgId, msgSeq: msgSeq};
                var message = uc.util.StringUtil.formatString(uc_resource.Message.updateMinutes, [this.getMessageUserName(from.userId)]);

                this.OnMessageReceived(from.userId, conversationId, conversationType, message, timestamp, channel, msgId, ackData,  meetingName, meetingId,eventId, 0, meetingStartTime, uc.constants.Meeting.MsgType.SummaryUpdated);
            },
            OnMeetingSummaryVideoMessageReceived: function(channel, from, to, operatorId, summaryId, summaryName, conferenceId, eventId, meetingName, meetingStartTime, updateTime,type,videoName,videoURL,videoThumb,thumbnailsURL,length,size,operateName,operateAvatar,msgId, msgSeq, timestamp){
                var conversationId = to.userId;
                var conversationType = uc.constants.Conversation.ConversationType.MeetingMsg;
                var ackData = {to: to, msgId: msgId, msgSeq: msgSeq};
                meetingName=decodeURI(meetingName);
                var message = uc.util.StringUtil.formatString(uc_resource.Message.meetingVideoSingleChat, null);

                this.OnMessageReceived(from.userId, conversationId, conversationType, message, timestamp, channel, msgId, ackData,  meetingName, conferenceId, eventId, 0, meetingStartTime, uc.constants.Meeting.MsgType.SummaryCreated);
            },
            OnIMSummaryVideoMessageReceived:function(channel, from, to, operatorId, summaryId, summaryName, conferenceId, eventId, meetingName, meetingStartTime, updateTime,type,videoName,videoURL,videoThumb,thumbnailsURL,length,size,operateName,operateAvatar,msgId, msgSeq, msgTime){
                var conversationId = from.userId == this.loginUserId ? to.userId : from.userId;
                var conversationType = uc.constants.Conversation.ConversationType.SingleChat;
                var ackData = {to: to, msgId: msgId, msgSeq: msgSeq};
                var message = uc.util.StringUtil.formatString(uc_resource.Message.MeetingVideo, null);

                this.OnMessageReceived(from.userId, conversationId, conversationType, message, msgTime, channel, msgId, ackData);
            },
            OnGroupSummaryVideoMessageReceived:function(conversationType,channel, from, to, operatorId, summaryId, summaryName, conferenceId, eventId, meetingName, meetingStartTime, updateTime,type,videoName,videoURL,videoThumb,thumbnailsURL,length,size,operateName,operateAvatar,msgId, msgSeq, msgTime){
                var conversationId = to.userId;
                //var conversationType = uc.constants.Conversation.ConversationType.ProjectChat;
                var ackData = {to: to, msgId: msgId, msgSeq: msgSeq};
                var message = uc.util.StringUtil.formatString(uc_resource.Message.MeetingVideoWithSender, [this.getMessageUserName(from.userId)]);

                this.OnMessageReceived(from.userId, conversationId, conversationType, message, msgTime, channel, msgId, ackData);
            },
            OnMeetingAccreditMessageReceived: function(channel, from, to, msgId, msgSeq, timestamp){
                var conversationId = from.userId == this.loginUserId ? to.userId : from.userId;
                var conversationType = uc.constants.Conversation.ConversationType.SingleChat;
                var ackData = from.userId == this.loginUserId ? null : from;
                var resource = from.userId == this.loginUserId ? uc_resource.Message.AccreditMeetingByMe : uc_resource.Message.AccreditMeetingToMe;
                var message = uc.util.StringUtil.formatString(resource, [this.getMessageUserName(conversationId)]);

                this.OnMessageReceived(from.userId, conversationId, conversationType, message, timestamp, channel, msgId, ackData);
            },
            OnMeetingAccreditCancelledMessageReceived: function(channel, from, to, msgId, msgSeq, timestamp){
                var conversationId = from.userId == this.loginUserId ? to.userId : from.userId;
                var conversationType = uc.constants.Conversation.ConversationType.SingleChat;
                var ackData = from.userId == this.loginUserId ? null : from;
                var resource = from.userId == this.loginUserId ? uc_resource.Message.CancelAccreditMeetingByMe : uc_resource.Message.CancelAccreditMeetingToMe;
                var message = uc.util.StringUtil.formatString(resource, [this.getMessageUserName(conversationId)]);

                this.OnMessageReceived(from.userId, conversationId, conversationType, message, timestamp, channel, msgId, ackData);
            },
            OnMeetingPartJoinMessageReceived: function(channel, from, to, msgId, msgSeq, timestamp){
                var conversationId = from.userId == this.loginUserId ? to.userId : from.userId;
                var conversationType = uc.constants.Conversation.ConversationType.MeetingMsg;
                var ackData = {to: to, msgId: msgId, msgSeq: msgSeq};
                var message = uc_resource.Message.JoinedInMeeting;

                this.OnMessageReceived(from.userId, conversationId, conversationType, message, timestamp, channel, msgId, ackData);
            },
            OnMeetingPartCancelMessageReceived: function(channel, from, to, msgId, msgSeq, timestamp){
                var conversationId = from.userId == this.loginUserId ? to.userId : from.userId;
                var conversationType = uc.constants.Conversation.ConversationType.MeetingMsg;
                var ackData = {to: to, msgId: msgId, msgSeq: msgSeq};
                var message = uc_resource.Message.QuitMeeting;

                this.OnMessageReceived(from.userId, conversationId, conversationType, message, timestamp, channel, msgId, ackData);
            },

            /************* IOpenApiEvent **************/
            OnAppListReceived: function(errorCode, apps){
                if(errorCode == uc.ErrorCode.Success){
                    uc.modules.conversation.ConversationManager.OnAppListReceived();
                }
            },
            OnGetAppInfoReceived: function(errorCode, apps) {
                uc.util.LogUtil.info('ConversitionEventListener', 'OnGetAppInfoReceived', 'GetAppsInfo', apps);
                if(errorCode == uc.ErrorCode.Success){
                    uc.modules.conversation.ConversationManager.OnGetAppInfoReceived(apps);
                }
            },
            OnAppStatusChangeReceived: function (channel, appId, status, extraData) {
                if (status == uc.constants.Api.AppChange.Modify) {
                    uc.modules.conversation.ConversationManager.OnAppStatusChangeReceived(channel, appId, status, extraData);
                }
            },
            OnChatHistoryReceived: function(errorCode, appId, msgCount, minTimestamp){},
            OnOpenApiTextMessageReceived: function(channel, sendState, from, to, textTitle, textContent, textType, detailContent, msgId, msgSeq, timestamp){
                var conversationId = from.userId == this.loginUserId ? to.userId : from.userId;
                var conversationType = uc.constants.Conversation.ConversationType.OpenApi;
                if (from) {
                    from.msgId = msgId;
                    from.msgSeq = msgSeq;
                }

                this.OnMessageReceived(from.userId, conversationId, conversationType, textContent, timestamp, channel, msgId, from);
            },
            OnOpenApiImageMessageReceived: function(channel, sendState, from, to, imageId, imageType, imageData, msgId, msgSeq, timestamp){
                var conversationId = from.userId == this.loginUserId ? to.userId : from.userId;
                var conversationType = uc.constants.Conversation.ConversationType.OpenApi;
                if (from) {
                    from.msgId = msgId;
                    from.msgSeq = msgSeq;
                }

                this.OnMessageReceived(from.userId, conversationId, conversationType, uc_resource.Message.ImageMessage, timestamp, channel, msgId, from);
            },
            OnOpenApiFileMessageReceived: function(channel, sendState, from, to, fileId, fileType, fileName, fileSize, msgId, msgSeq, timestamp){
                var conversationId = from.userId == this.loginUserId ? to.userId : from.userId;
                var conversationType = uc.constants.Conversation.ConversationType.OpenApi;
                if (from) {
                    from.msgId = msgId;
                    from.msgSeq = msgSeq;
                }

                this.OnMessageReceived(from.userId, conversationId, conversationType, uc_resource.Message.FileMessage, timestamp, channel, msgId, from);
            },
            OnOpenApiMessageSent:function(errorCode, appId, msgId, msgSeq, timestamp){
                uc.modules.conversation.ConversationManager.OnMessageSent(errorCode, appId, uc.constants.Conversation.ConversationType.OpenApi, msgId, msgSeq, timestamp);
            },
            OnOAMessageStatusReceived: function(errorCode, status) {},
            OnOAMessageReceived: function(channel, from, to, title, titleColor, titleBkColor, titleElements, status, data, detailUrl, msgId, msgSeq,timestamp) {
                var conversationId = from.userId == this.loginUserId ? to.userId : from.userId;
                var message = null;
                if(uc.modules.notice.NoticeManager.isNoticeConversation(conversationId)){
                   //将非UCAS消息替换为server端返回的最后一条未被删除的消息
                   if(channel != uc.constants.Message.MessageChannel.UCAS){
                     var lastNoticeInfo = uc.modules.notice.NoticeManager.getLastNoticeInfo();
                     if(lastNoticeInfo && lastNoticeInfo.title !== undefined){
                         title = lastNoticeInfo.title;
                         timestamp= parseInt(lastNoticeInfo.created_at);
                         channel = uc.constants.Message.MessageChannel.Notice;
                     }
                   }
                   message =  title;
                }else{
                   message = uc.util.StringUtil.formatString(uc_resource.Message.OpenApiOAMessage, [title, '']);//TODO get message content
                }
                var conversationType = uc.constants.Conversation.ConversationType.OpenApi;
                if (from) {
                    from.msgId = msgId;
                    from.msgSeq = msgSeq;
                }

                this.OnMessageReceived(from.userId, conversationId, conversationType, message, timestamp, channel, msgId, from);
            },
            OnOAMessageStatusChanged: function(channel, from, to, status, msgId, msgSeq, timestamp) {},
            OnPushMessageAck: function(channel, errorCode, from, to, conversation, msgId, msgSeq, timestamp){},

            /************* IRemoteAssistanceEvent **************/
            OnRemoteAssistanceCreated: function(errorCode, userId, meetingId, attendeePassword, meetingStartUrl){

            },

            OnRemoteAssistanceInvitationReceived: function(channel, from, meetingId, attendeePassword, msgId, msgSeq, timestamp){
                var conversationId = from.userId;
                if(conversationId == this.loginUserId){
                    return;
                }
                var conversationType = uc.constants.Conversation.ConversationType.SingleChat;
                var message = uc.util.StringUtil.formatString(uc_resource.Message.RemoteAssistanceInvited, [this.getMessageUserName(from.userId)]);

                this.OnMessageReceived(from.userId, conversationId, conversationType, message, timestamp, channel, msgId);
            },

            OnRemoteAssistanceInvitationAccepted: function(channel, from, meetingId, msgId, msgSeq, timestamp){
                var conversationId = from.userId;
                if(conversationId == this.loginUserId){
                    return;
                }
                var conversationType = uc.constants.Conversation.ConversationType.SingleChat;
                var message = uc.util.StringUtil.formatString(uc_resource.Message.RemoteAssistanceAccepted, [this.getMessageUserName(from.userId)]);

                this.OnMessageReceived(from.userId, conversationId, conversationType, message, timestamp, channel, msgId);
            },

            OnRemoteAssistanceInvitationRejected: function(channel, from, meetingId, msgId, msgSeq, timestamp){
                var conversationId = from.userId;
                if(conversationId == this.loginUserId){
                    return;
                }
                var conversationType = uc.constants.Conversation.ConversationType.SingleChat;
                var message = uc.util.StringUtil.formatString(uc_resource.Message.RemoteAssistanceRejected, [this.getMessageUserName(from.userId)]);

                this.OnMessageReceived(from.userId, conversationId, conversationType, message, timestamp, channel, msgId);
            },

            OnRemoteAssistanceEnded: function(channel, meetingId, msgId, msgSeq, timestamp){
                var userId = uc.modules.remoteAssistance.RemoteAssistanceManager.getUserIdByMeetingId(meetingId);
                if(userId <= 0 || !userId){
                    return;
                }
                var conversationId = userId;
                var conversationType = uc.constants.Conversation.ConversationType.SingleChat;
                var message = uc.util.StringUtil.formatString(uc_resource.Message.RemoteAssistanceEnded, [this.getMessageUserName(userId)]);

                this.OnMessageReceived(userId, conversationId, conversationType, message, timestamp, channel, msgId);
            },

            /** local function **/
            isChatWindowOpen: function(conversationId, conversationType){
                return uc.modules.conversation.ConversationManager.isChatWindowOpen(conversationId, conversationType);
            },

            needUpdateMeetingStatus: function(conversationId, conversationType, timestamp){
                var conversationInfo = uc.modules.conversation.ConversationManager.getConversation(conversationId, conversationType);
                return !(conversationInfo && conversationInfo.timestamp > timestamp);
            },

            /**
             *
             * @param from              from user id
             * @param conversationId    conversation id
             * @param conversationType  conversation type
             * @param message           message content
             * @param timestamp         timestamp of the message
             * @param msgChannel        msg channel
             * @param msgId             msg id
             * @param ackData           data need for sending ack read, format: {to: xx, msgId: xx, msgSeq: xx}
             * @param title             title of the conversation
             * @param meetingId         meeting id if message message
             * @param eventId           event if of the meeting
             * @param accreditUserId    accredit user id
             * @param meetingStartTime  meeting start time
             */
            OnMessageReceived: function(from, conversationId, conversationType, message, timestamp, msgChannel, msgId, ackData, title, eventId, accreditUserId, meetingStartTime, meetingMsgType){
                if(conversationId == 0){
                    return;
                }

                var conversationInfo = uc.modules.conversation.ConversationManager.getConversation(conversationId, conversationType);
                if(!conversationInfo){
                    //If message is from chat history, do not create conversation
                    if(msgChannel == uc.constants.Message.MessageChannel.ChatHistory){
                        return;
                    }
                    newMsgCount = msgChannel == uc.constants.Message.MessageChannel.UCAS ? (from == this.loginUserId ? 0 : 1) : 0;
                    conversationInfo = new uc.modules.conversation.ConversationInfo();
                    var userTopIndexInfo = uc.IClientService.getConversationInfoSync(conversationType, conversationId);
                    uc.util.LogUtil.info('ConversationEventListener', 'OnMessageReceived', 'userTopIndexInfo', {userTopIndexInfo: userTopIndexInfo});
                    var options = {
                        conversationId : conversationId,
                        conversationType: conversationType,
                        message:  uc.modules.emoji.EmojiConverter.unicodeToSmiley(uc.util.StringUtil.escapeHtml(message)),
                        title: title,
                        meetingId: eventId,
                        eventId : eventId,
                        authorizerId: accreditUserId,
                        meetingStartTime: meetingStartTime,
                        msgId: msgId,
                        timestamp: timestamp,
                        newMsgCount: newMsgCount,
                        meetingMsgType: meetingMsgType,
                        topIndex: userTopIndexInfo ? userTopIndexInfo.topIndex : 0
                    };

                    conversationInfo.init(options);
                    if(newMsgCount && !uc.modules.notice.NoticeManager.isNoticeConversation(conversationInfo.conversationId)){
                        this.increaseNewMsgCount(conversationId, conversationType, newMsgCount);
                    }
                    uc.modules.conversation.ConversationManager.setConversationTitleAndAvatar(conversationInfo);
                    uc.modules.conversation.ConversationManager.addConversation(conversationId, conversationType, conversationInfo);
                }else{
                    isChatWindowOpen = this.isChatWindowOpen(conversationId, conversationType);
                    if(isChatWindowOpen && ackData && msgChannel == uc.constants.Message.MessageChannel.UCAS && from != this.loginUserId && uc.IClientService.getWindowStatus() == uc.constants.UI.WindowActiveState.Topmost ){
                        this.setMessageRead(conversationType, ackData);
                    }

                    newMsgCount = msgChannel == uc.constants.Message.MessageChannel.UCAS ? (from == this.loginUserId ? 0 : (isChatWindowOpen && uc.IClientService.getWindowStatus() == uc.constants.UI.WindowActiveState.Topmost ? 0 : 1)) : 0;

                    if(conversationInfo.timestamp <= timestamp || newMsgCount || msgChannel == uc.constants.Message.MessageChannel.Notice){
                        var updateData = {
                            message :  uc.modules.emoji.EmojiConverter.unicodeToSmiley(uc.util.StringUtil.escapeApiHtml(message)),
                            title: title,
                            meetingId : eventId,
                            eventId : eventId,
                            authorizerId: accreditUserId,
                            msgId: msgId,
                            timestamp: timestamp,
                            fakeTime: msgChannel != uc.constants.Message.MessageChannel.ChatHistory ? 0 : conversationInfo.fakeTime,
                            isFakeTime: msgChannel != uc.constants.Message.MessageChannel.ChatHistory ? false : conversationInfo.isFakeTime,
                            newMsgCount: newMsgCount,
                            meetingMsgType: meetingMsgType
                        };
                        if(msgChannel == uc.constants.Message.MessageChannel.UCAS ){
                            updateData.meetingStartTime = meetingStartTime;
                        }
                        conversationInfo.updateData(updateData);

                        if(newMsgCount&& !uc.modules.notice.NoticeManager.isNoticeConversation(conversationInfo.conversationId)){
                            this.increaseNewMsgCount(conversationId, conversationType, newMsgCount);
                        }
                        uc.modules.conversation.ConversationManager.updateConversation(conversationId, conversationType, conversationInfo);

                    }
                }

                //Only message from UCAS and MessageList, need to update conversation time line
                if(msgChannel == uc.constants.Message.MessageChannel.UCAS || msgChannel == uc.constants.Message.MessageChannel.MessageList){
                    if (conversationInfo.topIndex <= 0) {
                        uc.modules.conversation.ConversationManager.setConversationTimeLine(timestamp);
                    }
                }

                if(msgChannel == uc.constants.Message.MessageChannel.UCAS){
                    uc.modules.call.CallManager.increaseNewMsgCount(conversationId, conversationType);
                }

                if(newMsgCount){
                    //group disturb state decide to bubble or not
                    if(conversationInfo){
                        var groupDisturbState = conversationInfo.disturbStatus;
                        var currentUserStatus = uc.modules.main.Main.currentUserStatusValue;

                        if(groupDisturbState || currentUserStatus == uc.constants.UserStatusType.Busy){
                            return;
                        }
                        uc.modules.bubble.BubbleManager.showPopupBubbleMessage(conversationInfo);
                    }
                }

            },
            setMessageRead: function(conversationType, ackData){
                switch (conversationType){
                    case uc.constants.Conversation.ConversationType.SingleChat:
                        uc.IIMService.sendMessageRead(ackData.to, ackData.msgId, ackData.msgSeq, function(a){console.log(a)});
                        break;
                    case uc.constants.Conversation.ConversationType.ProjectChat:
                    case uc.constants.Conversation.ConversationType.GroupChat:
                    case uc.constants.Conversation.ConversationType.CloudChat:
                    case uc.constants.Conversation.ConversationType.MeetingMsg:
                        uc.IGroupService.sendMessageRead(conversationType, ackData.to, ackData.msgId, ackData.msgSeq, function(a){console.log(a)});
                        break;
                    case uc.constants.Conversation.ConversationType.OpenApi:
                        uc.IOpenApiService.sendMessageRead(ackData, ackData.msgId, ackData.msgSeq, function(a){console.log(a)});
                        break;
                }
            },
            increaseNewMsgCount: function(conversationId, conversationType, count){
                uc.modules.conversation.ConversationManager.increaseNewMsgCount(conversationType, count, conversationId);
            },
            _OnMessageRead: function(conversationType, id, readType){
                uc.modules.conversation.ConversationManager.OnMessageRead(conversationType, id, readType);
            },
            getMessageUserName: function(userId){
                var userName;
                if(userId == this.loginUserId){
                    userName = uc_resource.Message.You;
                }else{
                    //var contactInfo = uc.IContactService.getBasicContactInfo(userId);
                    var contactInfo = uc.modules.cache.BasicContactInfoCache.getBasicContactInfo(userId);
                    if(contactInfo){
                        userName = contactInfo.displayName;
                    }else{
                        userName = '' + userId;
                        uc.modules.conversation.ConversationManager.addNoContactInfoUserId(userId);
                    }
                }
                return userName;
            },
            getGroupMessageMembersName: function(operatorId, members){
                var userId, userNameArray = [], userName;
                for(var i = 0, len = members.length; i < len && i < uc.constants.UI.GroupInviteMsgUserMaxCount; i++){
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
            getCloudMessageMembersName: function(operatorId, members){
                var userId, userNameArray = [], userName;
                for(var i = 0, len = members.length; i < len; i++){
                    if(typeof members[i] == 'object'){
                        userId = members[i].userId;
                    }else{
                        userId = members[i];
                    }
                    userName = this.getMessageUserName(userId);
                    userNameArray.push(userName);
                }
                return userNameArray.join('、');
            },
            getGroupMessageMembersRole: function(operatorId, members){
                var userId,role;
                for(var i = 0, len = members.length; i < len; i++){
                    if(typeof members[i] == 'object'){
                        userId = members[i].userId;
                        if(userId == operatorId){
                            continue;
                        }
                        role= members[i].role;
                        break;
                    }
                }
                return role;
            },
            OnWindowActiveStatus: function () {
                uc.modules.conversation.ConversationManager.OnWindowActiveChanged();
            },
            OnUserStatusSetted: function(errorCode, userId, status) {
            },
            OnUserStatusReceived: function(userIds, status) {
                uc.modules.conversation.ConversationManager.OnUserStatusReceived(userIds, status);
            },
            OnNoticeConversationStatusChanged:function(conversationId){
              uc.modules.conversation.ConversationManager.OnNoticeConversationStatusChanged(conversationId);
            },
            OnConversationSetTopReceived: function (errorCode, message) {
                uc.modules.conversation.ConversationManager.OnConversationSetTopReceived(errorCode, message);
            },
            OnNoticeLastMessageReceived:function(conversationId,title,timestamp){
                var conversationType = uc.constants.Conversation.ConversationType.OpenApi;
                this.OnMessageReceived(conversationId, conversationId, conversationType, title, timestamp, uc.constants.Message.MessageChannel.Notice);
            }
        })
})(uc);