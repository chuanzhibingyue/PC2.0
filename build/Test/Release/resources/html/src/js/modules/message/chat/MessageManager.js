/**
* Created by lei.zhou on 2015/8/12.
*/
(function (uc) {
    if (uc.modules.chat.MessageManager) {
        return;
    }

    var uploadTaskMap = new uc.util.HashMap();

    uc.modules.chat.MessageManager = {
        sendText: function (chatInfo, messageInfo, atUsers, callback, textFormat) {
            uc.util.LogUtil.info('MessageManager', 'sendText', 'data', {chatInfo: chatInfo, messageInfo: messageInfo,atUsers:atUsers});
            textFormat = textFormat || uc.modules.setting.SettingManager.getFontSettingFormat();
            switch (chatInfo.chatType) {
                case uc.constants.Chat.ChatType.SingleChat:
                    uc.IIMService.sendTextMessage(
                        chatInfo,
                        messageInfo.contentType,
                        textFormat,
                        messageInfo.content,
                        function (args) {
                            if (callback) {
                                callback({
                                    errorCode: args[0],
                                    msgId: args[1],
                                    chatType: chatInfo.chatType,
                                    chatObjectId: chatInfo.userId,
                                    messageType: uc.constants.Message.ContentType.Text,
                                    contentType: messageInfo.contentType,
                                    content: messageInfo.content,
                                    textFormat: textFormat
                                });
                            }
                            if(args[0]!=uc.ErrorCode.Success){
                                uc.util.LogUtil.info("MessageManager","sendTextMessage","SingleChat",
                                    {chatInfo: chatInfo, messageInfo: messageInfo});
                                return;
                            }
                            uc.IUIEvent.OnMessageSending({
                                chatId: chatInfo.userId,
                                chatType: chatInfo.chatType,
                                msgId: args[1],
                                contentType: uc.constants.Message.ContentType.Text,
                                content: messageInfo.content
                            })
                        });
                    break;

                case uc.constants.Chat.ChatType.GroupChat:
                case uc.constants.Chat.ChatType.ProjectChat:
                case uc.constants.Chat.ChatType.CloudChat:
                    uc.IGroupService.sendTextMessage(
                        chatInfo.chatType,
                        chatInfo,
                        messageInfo.contentType,
                        textFormat,
                        messageInfo.content,
                        atUsers,
                        function (args) {
                            if (callback) {
                                callback({
                                    errorCode: args[0],
                                    msgId: args[1],
                                    chatType: chatInfo.chatType,
                                    chatObjectId: chatInfo.userId,
                                    messageType: uc.constants.Message.ContentType.Text,
                                    contentType: messageInfo.contentType,
                                    content: messageInfo.content,
                                    textFormat: textFormat
                                });
                            }
                            if(args[0]!=uc.ErrorCode.Success){
                                uc.util.LogUtil.info("MessageManager","sendTextMessage","ProjectChat",
                                    {errorCode:args[0], chatInfo: chatInfo, messageInfo: messageInfo,atUsers:atUsers});
                                return;
                            }
                            uc.IUIEvent.OnMessageSending({
                                chatId: chatInfo.userId,
                                chatType: chatInfo.chatType,
                                msgId: args[1],
                                contentType: uc.constants.Message.ContentType.Text,
                                content: messageInfo.content
                            });
                        });
                    break;

                case uc.constants.Chat.ChatType.MeetingChat:
                    uc.IMeetingService.sendTextMessage(
                        chatInfo.eventId,
                        /*chatInfo.meetingId,*/
                        chatInfo.startTime,
                        chatInfo,
                        messageInfo.contentType,
                        textFormat,
                        messageInfo.content,
                        function (args) {
                            if (callback) {
                                callback({
                                    errorCode: args[0],
                                    msgId: args[1],
                                    chatType: chatInfo.chatType,
                                    chatObjectId: chatInfo.userId,
                                    messageType: uc.constants.Message.ContentType.Text,
                                    contentType: messageInfo.contentType,
                                    content: messageInfo.content,
                                    textFormat: textFormat
                                });
                            }
                            if(args[0]!=uc.ErrorCode.Success){
                                uc.util.LogUtil.info("MessageManager","sendTextMessage","MeetingChat",
                                    {chatInfo: chatInfo, messageInfo: messageInfo});
                                return;
                            }
                            uc.IUIEvent.OnMessageSending({
                                chatId: chatInfo.userId,
                                chatType: chatInfo.chatType,
                                msgId: args[1],
                                contentType: uc.constants.Message.ContentType.Text,
                                content: messageInfo.content,
                                meetingId: chatInfo.meetingId,
                                eventId: chatInfo.eventId
                            })
                        });
                    break;

                case uc.constants.Chat.ChatType.OpenApiChat:
                    uc.IOpenApiService.sendTextMessage(
                        chatInfo,
                        "",
                        messageInfo.content,
                        messageInfo.contentType,
                        '',
                        function (args) {
                            if (callback) {
                                callback({
                                    errorCode: args[0],
                                    msgId: args[1],
                                    chatType: chatInfo.chatType,
                                    chatObjectId: chatInfo.userId,
                                    messageType: uc.constants.Message.ContentType.Text,
                                    contentType: messageInfo.contentType,
                                    content: messageInfo.content,
                                    textFormat: textFormat
                                });
                            }
                            if(args[0]!=uc.ErrorCode.Success){
                                uc.util.LogUtil.info("MessageManager","sendTextMessage","OpenApiChat",
                                    {chatInfo: chatInfo, messageInfo: messageInfo});
                                return;
                            }
                            uc.IUIEvent.OnMessageSending({
                                chatId: chatInfo.userId,
                                chatType: chatInfo.chatType,
                                msgId: args[1],
                                contentType: uc.constants.Message.ContentType.Text,
                                content: messageInfo.content
                            })
                        });
                    break;
                default:
                    break;
            }
        },

        sendImageMessage: function (chatInfo, fileInfo, taskId, localPath) {
            uc.util.LogUtil.info('MessageManager', 'sendImageMessage', 'data', {chatInfo: chatInfo, fileInfo: fileInfo});
            switch (chatInfo.chatType) {
                case uc.constants.Chat.ChatType.SingleChat:
                    uc.IIMService.sendImageMessage(
                        chatInfo,
                        fileInfo.name,
                        fileInfo.size,
                        fileInfo.url,
                        localPath,
                        function (args) {
                            if (args[0] !== 0) {
                                uc.util.LogUtil.info("MessageManager","sendImageMessage","SingleChat",
                                    {chatInfo: chatInfo, fileInfo: fileInfo});
                                uc.modules.chat.ChatManager.setUploadMsgFaild(taskId);
                                return;
                            }
                            uc.modules.chat.ChatManager.setUploadMsgId(taskId, args[1], true, localPath, fileInfo.name);
                            uc.IUIEvent.OnMessageSending({
                                chatId: chatInfo.userId,
                                chatType: chatInfo.chatType,
                                taskId: taskId,
                                msgId: args[1],
                                contentType: uc.constants.Message.ContentType.Image
                            })
                        });
                    break;

                case uc.constants.Chat.ChatType.GroupChat:
                case uc.constants.Chat.ChatType.ProjectChat:
                case uc.constants.Chat.ChatType.CloudChat:
                    uc.IGroupService.sendImageMessage(
                        chatInfo.chatType,
                        chatInfo,
                        fileInfo.name,
                        fileInfo.size,
                        fileInfo.url,
                        localPath,
                        function (args) {
                            if (args[0] !== 0) {
                                uc.util.LogUtil.info("MessageManager","sendImageMessage","ProjectChat",
                                    {chatInfo: chatInfo, fileInfo: fileInfo});
                                uc.modules.chat.ChatManager.setUploadMsgFaild(taskId);
                                return;
                            }
                            uc.modules.chat.ChatManager.setUploadMsgId(taskId, args[1], true, localPath, fileInfo.name);
                            uc.IUIEvent.OnMessageSending({
                                chatId: chatInfo.userId,
                                chatType: chatInfo.chatType,
                                taskId: taskId,
                                msgId: args[1],
                                contentType: uc.constants.Message.ContentType.Image
                            })
                        });
                    break;

                case uc.constants.Chat.ChatType.MeetingChat:
                    uc.IMeetingService.sendImageMessage(
                        chatInfo.eventId,
                        /* chatInfo.meetingId, */
                        chatInfo.startTime,
                        chatInfo,
                        fileInfo.name,
                        fileInfo.size,
                        fileInfo.url,
                        localPath,
                        function (args) {
                            if (args[0] !== 0) {
                                uc.util.LogUtil.info("MessageManager","sendImageMessage","MeetingChat",
                                    {chatInfo: chatInfo, fileInfo: fileInfo});
                                uc.modules.chat.ChatManager.setUploadMsgFaild(taskId);
                                return;
                            }
                            uc.modules.chat.ChatManager.setUploadMsgId(taskId, args[1], true, localPath, fileInfo.name);
                            uc.IUIEvent.OnMessageSending({
                                chatId: chatInfo.userId,
                                chatType: chatInfo.chatType,
                                taskId: taskId,
                                msgId: args[1],
                                contentType: uc.constants.Message.ContentType.Image,
                                meetingId: chatInfo.meetingId,
                                eventId: chatInfo.eventId
                            })
                        });
                    break;

                case uc.constants.Chat.ChatType.OpenApiChat:
                    uc.IOpenApiService.sendImageMessage(
                        chatInfo,
                        fileInfo.fid,
                        1,
                        localPath,
                        function (args) {
                            if (args[0] !== 0) {
                                uc.util.LogUtil.info("MessageManager","sendImageMessage","OpenApiChat",
                                    {chatInfo: chatInfo, fileInfo: fileInfo});
                                uc.modules.chat.ChatManager.setUploadMsgFaild(taskId);
                                return;
                            }
                            uc.modules.chat.ChatManager.setUploadMsgId(taskId, args[1], true, localPath, fileInfo.name);
                            uc.IUIEvent.OnMessageSending({
                                chatId: chatInfo.userId,
                                chatType: chatInfo.chatType,
                                taskId: taskId,
                                msgId: args[1],
                                contentType: uc.constants.Message.ContentType.Image
                            })
                        });
                    break;
                default:
                    break;
            }
        },

        sendFile: function (chatInfo, fileInfo, taskId) {
            uc.util.LogUtil.info('MessageManager', 'sendFile', 'data', {chatInfo: chatInfo, fileInfo: fileInfo});
            var id = 0;
            switch (chatInfo.chatType) {
                case uc.constants.Chat.ChatType.SingleChat:
                    uc.IIMService.sendFileMessage(
                        chatInfo,
                        1,
                        fileInfo.name,
                        fileInfo.size,
                        fileInfo.url,
                        id,
                        function (args) {
                            if (args[0] !== 0) {
                                uc.util.LogUtil.info("MessageManager","sendFileMessage","SingleChat",
                                    {chatInfo: chatInfo, fileInfo: fileInfo});
                                uc.modules.chat.ChatManager.setUploadMsgFaild(taskId);
                                return;
                            }
                            uc.modules.chat.ChatManager.setUploadMsgId(taskId, args[1], false, "", '');
                            uc.IUIEvent.OnMessageSending({
                                chatId: chatInfo.userId,
                                chatType: chatInfo.chatType,
                                taskId: taskId,
                                msgId: args[1],
                                contentType: uc.constants.Message.ContentType.File
                            })
                        });
                    break;

                case uc.constants.Chat.ChatType.GroupChat:
                case uc.constants.Chat.ChatType.ProjectChat:
                    uc.IGroupService.uploadDocument(
                        chatInfo.userId,
                        0,
                        fileInfo.name,
                        fileInfo.size,
                        fileInfo.url,
                        function (args) {
                            if (args[0] !== 0) {
                                uc.util.LogUtil.info("MessageManager","uploadDocument","ProjectChat",
                                    {chatInfo: chatInfo, fileInfo: fileInfo});
                                uc.modules.chat.ChatManager.setUploadMsgFaild(taskId);
                                return;
                            }
                            uc.modules.chat.ChatManager.setUploadMsgId(taskId, args[1], false, "", '');
	                        if(uc.modules.meeting.MeetingManager){
		                        uc.modules.meeting.MeetingManager.setUploadMsgId(taskId, args[1]);
	                        }
                            uc.IUIEvent.OnMessageSending({
                                chatId: chatInfo.userId,
                                chatType: chatInfo.chatType,
                                taskId: taskId,
                                msgId: args[1],
                                contentType: uc.constants.Message.ContentType.File
                            });
                        });
                    break;
	            case uc.constants.Chat.ChatType.MeetingChat:
		            uc.IMeetingService.uploadMeetingFile(
			            chatInfo.eventId,
			            0,
			            fileInfo.name,
			            fileInfo.size,
			            fileInfo.url,
                        fileInfo.operatorTime,
			            function (args) {
				            if (args[0] !== 0) {
					            uc.util.LogUtil.info("MessageManager","uploadMeetingFile","MeetingChat",
						            {chatInfo: chatInfo, fileInfo: fileInfo});
					            uc.modules.chat.ChatManager.setUploadMsgFaild(taskId);
					            return;
				            }
				            uc.modules.chat.ChatManager.setUploadMsgId(taskId, args[1], false, "", '');
				            if(uc.modules.meeting.MeetingFileManager){
                                uc.modules.meeting.MeetingFileManager.setUploadMsgId(taskId, args[1]);
				            }
				            uc.IUIEvent.OnMessageSending({
					            chatId: chatInfo.userId,
					            chatType: chatInfo.chatType,
					            taskId: taskId,
					            msgId: args[1],
					            contentType: uc.constants.Message.ContentType.File
				            });
			            });
		            break;
                case uc.constants.Chat.ChatType.OpenApiChat:
                    uc.IOpenApiService.sendFileMessage(
                        chatInfo,
                        fileInfo.fid,
                        1,
                        fileInfo.name,
                        fileInfo.size,
                        function (args) {
                            if (args[0] !== 0) {
                                uc.util.LogUtil.info("MessageManager","sendFileMessage","OpenApiChat",
                                    {chatInfo: chatInfo, fileInfo: fileInfo});
                                uc.modules.chat.ChatManager.setUploadMsgFaild(taskId);
                                return;
                            }
                            uc.modules.chat.ChatManager.setUploadMsgId(taskId, args[1], false, "", '');
                            uc.IUIEvent.OnMessageSending({
                                chatId: chatInfo.userId,
                                chatType: chatInfo.chatType,
                                taskId: taskId,
                                msgId: args[1],
                                contentType: uc.constants.Message.ContentType.File
                            });
                        });
                    break;
                default:
                    break;
            }
        },

        uploadFile: function (filePath, isImage, chatInfo, callback) {
            var me = this;

            uc.IFSService.uploadFile(filePath, function (args) {
                var errorCode = args[0], taskId = args[1];
                uc.util.LogUtil.info('MessageManager', 'uploadFile', 'filePath = ' + filePath + ",  errorCode = " + errorCode + ", taskId = " + taskId, null);
                if (errorCode == 60034) {
                    uc.modules.component.PromptMessageBox.open({title: uc_resource.Common.Error, message: uc_resource.Chat.ExceedLimit, buttonMode:"ok", OkText: uc_resource.Common.Known},
                        function(result){
                            this.close();
                        });
                    return;
                } else if (errorCode == 60035) {
                    uc.modules.component.PromptMessageBox.open({title: uc_resource.Common.Error, message: uc_resource.Chat.BeOccupied, buttonMode:"ok", OkText: uc_resource.Common.Known},
                        function(result){
                            this.close();
                        });
                    return;
                }  else if (errorCode == 60030) {
                    uc.modules.component.PromptMessageBox.open({title: uc_resource.Common.Error, message: uc_resource.Chat.ErrorCompulateMd5, buttonMode:"ok", OkText: uc_resource.Common.Known},
                        function(result){
                            this.close();
                        });
                    return;
                }

                if (errorCode === 0) {
                    uploadTaskMap.put(me.getTaskKey(taskId),
                        {
                            chatInfo: chatInfo,
                            isImage: isImage,
                            localPath: filePath
                        });
                }

                if (callback) {
                    callback({
                        errorCode: errorCode,
                        taskId: taskId,
                        chatType: chatInfo.chatType,
                        chatObjectId: chatInfo.userId
                    });
                }
            });
        },

        noticeFileUploaded: function (taskId, fileInfo) {
            var key = this.getTaskKey(taskId),
                temp = uploadTaskMap.get(key);
            if (temp == null) {
                return;
            }
            var isImage = temp.isImage, chatInfo = temp.chatInfo;

            uploadTaskMap.remove(key);

            if (isImage) {
                this.sendImageMessage(chatInfo, fileInfo, taskId, temp.localPath);
            }
            else {
                this.sendFile(chatInfo, fileInfo, taskId);
            }
        },

        getTaskKey: function (taskId) {
            return "task_" + taskId;
        },
        forwardTextMessage: function (content, textFormat) {
            uc.IUIService.openChooseContactWindow(
                {
                    key: "forwardMessage" + (new Date().getTime()),
                    allowMultipleSelect: true,
                    allowMultipleSelectConversations: uc.constants.chooseContacts.allowMultipleSelectConversations,
                    allowChooseOrg:false,
                    isForward: true
                }, function (args) {
                    if (!args.okOrCancel) {
                        return;
                    }

                    // start-------------------------added by xue.bai_2 on 2016/04/18-------------------------------------
                    if(args.otherItem.groupChats.length > 0){
                        var groupChats = args.otherItem.groupChats;
                        $.each(groupChats, function(index, groupChat){
                            uc.modules.chat.MessageManager.sendText({
                                chatType : uc.constants.Chat.ChatType.GroupChat,
                                resId: 100,
                                userId: groupChat,
                                siteId: uc.modules.chat.ChatManager.myUserInfo.siteId
                            }, {
                                contentType: 1,
                                content: content
                            }, [], function(){}, textFormat);
                        });
                    }

                    if(args.otherItem.projects.length > 0){
                        var projects = args.otherItem.projects;
                        $.each(projects, function(index, project){
                            uc.modules.chat.MessageManager.sendText({
                                chatType : uc.constants.Chat.ChatType.ProjectChat,
                                resId: 100,
                                userId: project,
                                siteId: uc.modules.chat.ChatManager.myUserInfo.siteId
                            }, {
                                contentType: 1,
                                content: content
                            }, [], function(){}, textFormat);
                        });
                    }

                    if(args.otherItem.contacts.length > 0){
                        var contacts = args.otherItem.contacts;
                        $.each(contacts, function(index, contact){
                            uc.modules.chat.MessageManager.sendText({
                                chatType : uc.constants.Chat.ChatType.SingleChat,
                                resId: 100,
                                userId: contact,
                                siteId: uc.modules.chat.ChatManager.myUserInfo.siteId
                            }, {
                                contentType: 1,
                                content: content
                            }, [], function(){}, textFormat);
                        });
                    }
                    // -------------------------added by xue.bai_2 on 2016/04/18--------------------------------end

                    // old code
                    //var conversationType, toId;
                    //if (args.otherItem.groupChats.length > 0) {
                    //    conversationType = uc.constants.Chat.ChatType.GroupChat;
                    //    toId = args.otherItem.groupChats[0];
                    //} else if (args.otherItem.projects.length > 0) {
                    //    conversationType = uc.constants.Chat.ChatType.ProjectChat;
                    //    toId = args.otherItem.projects[0];
                    //} else if (args.otherItem.contacts.length > 0) {
                    //    conversationType = uc.constants.Chat.ChatType.SingleChat;
                    //    toId = args.otherItem.contacts[0];
                    //} else {
                    //    return;
                    //}
                    //switch (conversationType) {
                    //    case uc.constants.Chat.ChatType.SingleChat:
                    //        uc.modules.chat.MessageManager.sendText({
                    //            chatType: conversationType,
                    //            resId: 100,
                    //            userId: toId,
                    //            siteId: uc.modules.chat.ChatManager.myUserInfo.siteId
                    //        }, {
                    //            contentType: 1,
                    //            content: content
                    //        }, [], function(){}, textFormat);
                    //        break;
                    //    case uc.constants.Chat.ChatType.GroupChat:
                    //    case uc.constants.Chat.ChatType.ProjectChat:
                    //        uc.modules.chat.MessageManager.sendText({
                    //            chatType: conversationType,
                    //            resId: 100,
                    //            userId: toId,
                    //            siteId: uc.modules.chat.ChatManager.myUserInfo.siteId
                    //        }, {
                    //            contentType: 1,
                    //            content: content
                    //        }, [], function(){}, textFormat);
                    //        break;
                    //}
                });
        },
        forwardImageMessage: function (imageName, imageSize, imageURL, imageData) {
            var imageSize = Number(imageSize);
            uc.IUIService.openChooseContactWindow(
                {
                    key: "forwardMessage" + (new Date().getTime()),
                    allowMultipleSelect: true,
                    allowMultipleSelectConversations: uc.constants.chooseContacts.allowMultipleSelectConversations,
                    allowChooseOrg:false,
                    isForward: true
                }, function (args) {
                    if (!args.okOrCancel) {
                        return;
                    }

                    // start-------------------------added by xue.bai_2 on 2016/04/18-------------------------------------
                    if(args.otherItem.groupChats.length > 0){
                        var groupChats = args.otherItem.groupChats;
                        $.each(groupChats, function(index, groupChat){

                            uc.IGroupService.sendImageMessage(
                                uc.constants.Chat.ChatType.GroupChat,
                                    {
                                        userId: groupChat,
                                        resId: 100,
                                        siteId: uc.modules.chat.ChatManager.myUserInfo.siteId
                                    },
                                    imageName,
                                    imageSize,
                                    imageURL,
                                    imageData,
                                    function (args) {
                                        if (args[0] !== 0) {
                                            return;
                                        }
                                        uc.IUIEvent.OnMessageSending({
                                            chatId: groupChat,
                                            chatType: uc.constants.Chat.ChatType.GroupChat,
                                            taskId: 0,
                                            msgId: args[1],
                                            contentType: uc.constants.Message.ContentType.Image
                                        })
                                });
                        });
                    }

                    if(args.otherItem.projects.length > 0){
                        var projects = args.otherItem.projects;
                        $.each(projects, function(index, project){
                            uc.IGroupService.sendImageMessage(
                                uc.constants.Chat.ChatType.ProjectChat,
                                {
                                    userId: project,
                                    resId: 100,
                                    siteId: uc.modules.chat.ChatManager.myUserInfo.siteId
                                },
                                imageName,
                                imageSize,
                                imageURL,
                                imageData,
                                function (args) {
                                    if (args[0] !== 0) {
                                        return;
                                    }
                                    uc.IUIEvent.OnMessageSending({
                                        chatId: project,
                                        chatType: uc.constants.Chat.ChatType.ProjectChat,
                                        taskId: 0,
                                        msgId: args[1],
                                        contentType: uc.constants.Message.ContentType.Image
                                    })
                                });
                        });
                    }

                    if(args.otherItem.contacts.length > 0){
                        var contacts = args.otherItem.contacts;
                        $.each(contacts, function(index, contact){
                            uc.IIMService.sendImageMessage(
                                {
                                    userId: contact,
                                    resId: 100,
                                    siteId: uc.modules.chat.ChatManager.myUserInfo.siteId
                                },
                                imageName,
                                imageSize,
                                imageURL,
                                imageData,
                                function (args) {
                                    if (args[0] !== 0) {
                                        return;
                                    }
                                    uc.IUIEvent.OnMessageSending({
                                        chatId: contact,
                                        chatType: uc.constants.Chat.ChatType.SingleChat,
                                        taskId: 0,
                                        msgId: args[1],
                                        contentType: uc.constants.Message.ContentType.Image
                                    })
                                });
                        });
                    }
                    // -------------------------added by xue.bai_2 on 2016/04/18--------------------------------end

                    // old code
                    //var conversationType, toId;
                    //if (args.otherItem.groupChats.length > 0) {
                    //    conversationType = uc.constants.Chat.ChatType.GroupChat;
                    //    toId = args.otherItem.groupChats[0];
                    //} else if (args.otherItem.projects.length > 0) {
                    //    conversationType = uc.constants.Chat.ChatType.ProjectChat;
                    //    toId = args.otherItem.projects[0];
                    //} else if (args.contacts.length > 0) {
                    //    conversationType = uc.constants.Chat.ChatType.SingleChat;
                    //    toId = args.contacts[0].userId;
                    //} else {
                    //    return;
                    //}
                    //switch (conversationType) {
                    //    case uc.constants.Chat.ChatType.SingleChat:
                    //        uc.IIMService.sendImageMessage(
                    //            {
                    //                userId: toId,
                    //                resId: 100,
                    //                siteId: uc.modules.chat.ChatManager.myUserInfo.siteId
                    //            },
                    //            imageName,
                    //            imageSize,
                    //            imageURL,
                    //            imageData,
                    //            function (args) {
                    //                if (args[0] !== 0) {
                    //                    return;
                    //                }
                    //                uc.IUIEvent.OnMessageSending({
                    //                    chatId: toId,
                    //                    chatType: conversationType,
                    //                    taskId: 0,
                    //                    msgId: args[1],
                    //                    contentType: uc.constants.Message.ContentType.Image
                    //                })
                    //            });
                    //        break;
                    //
                    //    case uc.constants.Chat.ChatType.GroupChat:
                    //    case uc.constants.Chat.ChatType.ProjectChat:
                    //        uc.IGroupService.sendImageMessage(
                    //            conversationType,
                    //            {
                    //                userId: toId,
                    //                resId: 100,
                    //                siteId: uc.modules.chat.ChatManager.myUserInfo.siteId
                    //            },
                    //            imageName,
                    //            imageSize,
                    //            imageURL,
                    //            imageData,
                    //            function (args) {
                    //                if (args[0] !== 0) {
                    //                    return;
                    //                }
                    //                uc.IUIEvent.OnMessageSending({
                    //                    chatId: toId,
                    //                    chatType: conversationType,
                    //                    taskId: 0,
                    //                    msgId: args[1],
                    //                    contentType: uc.constants.Message.ContentType.Image
                    //                })
                    //            });
                    //        break;
                    //}
                });
        },
        forwardFileMessage: function (fileName, fileSize, fileURL) {
            var fileSize = Number(fileSize);
            uc.IUIService.openChooseContactWindow(
                {
                    key: "forwardMessage" + (new Date().getTime()),
                    allowMultipleSelect: true,
                    allowMultipleSelectConversations: uc.constants.chooseContacts.allowMultipleSelectConversations,
                    allowChooseOrg:false
                }, function (args) {
                    console.log(args);
                    if (!args.okOrCancel) {
                        return;
                    }

                    // start-------------------------added by xue.bai_2 on 2016/04/18-------------------------------------
                    if(args.otherItem.groupChats.length > 0){
                        var groupChats = args.otherItem.groupChats;

                        $.each(groupChats, function(index, groupChat){
                            uc.IGroupService.uploadDocument(
                                groupChat, 0, fileName, fileSize, fileURL,
                                function (args) {
                                    console.log(args);
                                    if (args[0] !== 0) {
                                        console.log(args, uc.constants.Chat.ChatType.GroupChat, groupChat, fileName, fileSize, fileURL);
                                        return;
                                    }
                                    uc.IUIEvent.OnMessageSending({
                                        chatId: groupChat,
                                        chatType: uc.constants.Chat.ChatType.GroupChat,
                                        taskId: 0,
                                        msgId: args[1],
                                        contentType: uc.constants.Message.ContentType.File
                                    })
                                });
                        });
                    }

                    if(args.otherItem.projects.length > 0){
                        var projects = args.otherItem.projects;
                        $.each(projects, function(index, project){
                            uc.IGroupService.uploadDocument(
                                project, 0, fileName, fileSize, fileURL,
                                function (args) {
                                    console.log(args);
                                    if (args[0] !== 0) {
                                        console.log(args, uc.constants.Chat.ChatType.ProjectChat, project, fileName, fileSize, fileURL);
                                        return;
                                    }
                                    uc.IUIEvent.OnMessageSending({
                                        chatId: project,
                                        chatType: uc.constants.Chat.ChatType.ProjectChat,
                                        taskId: 0,
                                        msgId: args[1],
                                        contentType: uc.constants.Message.ContentType.File
                                    })
                                });
                        });
                    }

                    if(args.otherItem.contacts.length > 0){
                        var contacts = args.otherItem.contacts;
                        $.each(contacts, function(index, contact){
                            uc.IIMService.sendFileMessage(
                                {
                                    userId: contact,
                                    resId: 100,
                                    siteId: uc.modules.chat.ChatManager.myUserInfo.siteId
                                }, 0, fileName, fileSize, fileURL, 0,
                                function (args) {
                                    console.log(args);
                                    if (args[0] !== 0) {
                                        console.log(args, uc.constants.Chat.ChatType.SingleChat, toId, fileName, fileSize, fileURL);
                                        return;
                                    }
                                    uc.IUIEvent.OnMessageSending({
                                        chatId: contact,
                                        chatType: uc.constants.Chat.ChatType.SingleChat,
                                        taskId: 0,
                                        msgId: args[1],
                                        contentType: uc.constants.Message.ContentType.File
                                    })
                                });
                        });
                    }
                    // -------------------------added by xue.bai_2 on 2016/04/18--------------------------------end

                    // old code
                    //var conversationType, toId;
                    //if (args.otherItem.groupChats.length > 0) {
                    //    conversationType = uc.constants.Chat.ChatType.GroupChat;
                    //    toId = args.otherItem.groupChats[0];
                    //} else if (args.otherItem.projects.length > 0) {
                    //    conversationType = uc.constants.Chat.ChatType.ProjectChat;
                    //    toId = args.otherItem.projects[0];
                    //} else if (args.contacts.length > 0) {
                    //    conversationType = uc.constants.Chat.ChatType.SingleChat;
                    //    toId = args.contacts[0].userId;
                    //} else {
                    //    return;
                    //}
                    //switch (conversationType) {
                    //    case uc.constants.Chat.ChatType.SingleChat:
                    //        //uc.IUIEvent.OnMessageFileUploading(toId, conversationType, 0, uc.constants.Message.ContentType.File);
                    //        uc.IIMService.sendFileMessage(
                    //            {
                    //                userId: toId,
                    //                resId: 100,
                    //                siteId: uc.modules.chat.ChatManager.myUserInfo.siteId
                    //            }, 0, fileName, fileSize, fileURL, 0,
                    //            function (args) {
                    //                console.log(args);
                    //                if (args[0] !== 0) {
                    //                    console.log(args, conversationType, toId, fileName, fileSize, fileURL);
                    //                    return;
                    //                }
                    //                uc.IUIEvent.OnMessageSending({
                    //                    chatId: toId,
                    //                    chatType: conversationType,
                    //                    taskId: 0,
                    //                    msgId: args[1],
                    //                    contentType: uc.constants.Message.ContentType.File
                    //                })
                    //            });
                    //        break;
                    //
                    //    case uc.constants.Chat.ChatType.GroupChat:
                    //    case uc.constants.Chat.ChatType.ProjectChat:
		    	     //   case uc.constants.Chat.ChatType.MeetingChat:
                    //        console.log( fileName, fileSize, fileURL);
                    //        uc.IGroupService.uploadDocument(
                    //            toId, 0, fileName, fileSize, fileURL,
                    //            function (args) {
                    //                console.log(args);
                    //                if (args[0] !== 0) {
                    //                    console.log(args, conversationType, toId, fileName, fileSize, fileURL);
                    //                    return;
                    //                }
                    //                uc.IUIEvent.OnMessageSending({
                    //                    chatId: toId,
                    //                    chatType: conversationType,
                    //                    taskId: 0,
                    //                    msgId: args[1],
                    //                    contentType: uc.constants.Message.ContentType.File
                    //                })
                    //            });
                    //        break;
                    //}
                });
        },
        RightClickToSendFile:function(filePath){
            var paramObj = {filePath:filePath};
            uc.IUIService.openChooseContactWindow(
                {
                    key: "rightClickToSendFile" + (new Date().getTime()),
                    allowMultipleSelect: false,
                    allowChooseOrg:false
                }, function (args) {
                    console.log(args);
                    if (!args.okOrCancel) {
                        return;
                    }

                    if (args.otherItem.groupChats.length == 1) {
                        var groupChats = args.otherItem.groupChats;
                        uc.IUIService.openChatWindow(uc.constants.Chat.ChatType.GroupChat, groupChats[0], uc.constants.UI.OpenChatWindowSourceModule.Other, null, null, true, false, paramObj);
                    }

                    if (args.otherItem.projects.length > 0) {
                        var projects = args.otherItem.projects;
                        uc.IUIService.openChatWindow(uc.constants.Chat.ChatType.ProjectChat, projects[0], uc.constants.UI.OpenChatWindowSourceModule.Other, null, null, true, false, paramObj);
                    }

                    if (args.otherItem.contacts.length == 1) {
                        var contact = args.otherItem.contacts;
                        uc.IUIService.openChatWindow(uc.constants.Chat.ChatType.SingleChat, contact[0], uc.constants.UI.OpenChatWindowSourceModule.Other, null, null, true, false, paramObj);
                    }
                });
        },
        forwardCodeMessage: function (codeTitle, langType, codeHead, codeDesc, codeSize, codeURL, codeLine, createTime) {
            var codeSize = Number(codeSize);
            var codeLine = Number(codeLine);
            var createTime = Number(createTime);
            uc.IUIService.openChooseContactWindow(
                {
                    key: "forwardMessage" + (new Date().getTime()),
                    allowMultipleSelect: false,
                    allowChooseOrg:false
                }, function (args) {
                    console.log(args);
                    if (!args.okOrCancel) {
                        return;
                    }
                    var conversationType, toId;
                    if (args.otherItem.groupChats.length > 0) {
                        conversationType = uc.constants.Chat.ChatType.GroupChat;
                        toId = args.otherItem.groupChats[0];
                    } else if (args.otherItem.projects.length > 0) {
                        conversationType = uc.constants.Chat.ChatType.ProjectChat;
                        toId = args.otherItem.projects[0];
                    } else if (args.contacts.length > 0) {
                        conversationType = uc.constants.Chat.ChatType.SingleChat;
                        toId = args.contacts[0].userId;
                    } else {
                        return;
                    }
                    switch (conversationType) {
                        case uc.constants.Chat.ChatType.SingleChat:
                            console.log(toId, codeTitle, langType, codeHead, codeDesc, codeSize, codeURL, codeLine, createTime);
                            uc.IIMService.sendCodeMessage(
                                {
                                    userId: toId,
                                    resId: 100,
                                    siteId: uc.modules.chat.ChatManager.myUserInfo.siteId
                                }, 0, 0, codeTitle, langType, codeHead, codeDesc, codeSize, codeURL, codeLine, createTime,
                                function (args) {
                                    console.log(args);
                                    if (args[0] !== 0) {
                                        console.log(args, conversationType, toId, conversationType, toId, codeTitle, langType, codeHead, codeDesc, codeSize, codeURL, codeLine, createTime);
                                        return;
                                    }
                                    uc.IUIEvent.OnMessageSending({
                                        chatId: toId,
                                        chatType: conversationType,
                                        taskId: 0,
                                        msgId: args[1],
                                        contentType: uc.constants.Message.ContentType.Code
                                    })
                                });
                            break;

                        case uc.constants.Chat.ChatType.GroupChat:
                        case uc.constants.Chat.ChatType.ProjectChat:
                            console.log(toId, codeTitle, langType, codeHead, codeDesc, codeSize, codeURL, codeLine, createTime);
                            uc.IGroupService.uploadCodeSnippet(
                                toId, 0, codeTitle, langType, codeHead, codeDesc, codeSize, codeURL, codeLine,
                                function (args) {
                                    console.log(args);
                                    if (args[0] !== 0) {
                                        console.log(args, conversationType, toId, conversationType, toId, codeTitle, langType, codeHead, codeDesc, codeSize, codeURL, codeLine, createTime);
                                        return;
                                    }
                                    uc.IUIEvent.OnMessageSending({
                                        chatId: toId,
                                        chatType: conversationType,
                                        taskId: 0,
                                        msgId: args[1],
                                        contentType: uc.constants.Message.ContentType.Code
                                    })
                                });
                            break;
                    }
                });
        },
        sendCloudFileMessage:function(chatInfo,data){
            uc.util.LogUtil.info('MessageManager', 'sendCloudFileMessage', 'sendCloudFile', {chatInfo: chatInfo, data: data});
            var currentUserInfo = uc.IContactService.getCurrentUserInfo();
           // var cloudFileInfo=uc.modules.chat.ChatManager.cloudFileDataToString(data);
            var cloudFileInfo=JSON.stringify(data),mountId=0;
            if(data.hasOwnProperty("mount_id")){
                mountId=parseInt(data.mount_id);
            }else if(data.hasOwnProperty("mountId")){
                mountId=parseInt(data.mountId);
            }else if(data.hasOwnProperty("mountid")){
                mountId=parseInt(data.mountid);
            }
            switch (chatInfo.chatType){
                case uc.constants.Chat.ChatType.SingleChat:
                    uc.IIMService.sendCloudFileMessage(
                        { "userId": chatInfo.chatObjectId,
                          "siteId": chatInfo.siteId,
                          "resId": chatInfo.resId
                        },
                        data.filename,
                        parseInt(data.filesize),
                        uc.constants.Message.CloudType.GoKuai,
                        cloudFileInfo,
                        function (args) {
                            if (args[0] !== 0) {
                                console.log(args,data.filename, data.filesize+",chatInfo："+chatInfo);
                                return;
                            }
                            var _extentData= $.extend(data,{"msgId":args[1]});
                            uc.modules.chat.ChatManager.OnSendCloudFile(chatInfo,_extentData);
                            uc.IUIEvent.OnMessageSending({
                                chatId: chatInfo.chatObjectId,
                                chatType: chatInfo.chatType,
                                msgId: args[1],
                                contentType: uc.constants.Message.ContentType.CloudFile
                            })
                        });
                    break;
                case uc.constants.Chat.ChatType.ProjectChat:
                case uc.constants.Chat.ChatType.GroupChat:
                    uc.IGroupService.uploadCloudDocument(
                        chatInfo.chatObjectId,
                        parseInt(currentUserInfo.userId),
                        1,//isSendMessage,0-no ,1-send;
                        mountId,
                        data.filename,
                        parseInt(data.filesize),
                        uc.constants.Message.CloudType.GoKuai,
                        data.hash,
                        cloudFileInfo,
                        function (args) {
                            if (args[0] !== 0) {
                                console.log(args, {chatInfo: chatInfo, fileInfo: cloudFileInfo});
                                return;
                            }
                            var _extentData= $.extend(data,{"msgId":args[1]});
                            uc.modules.chat.ChatManager.OnSendCloudFile(chatInfo,_extentData);
                            uc.IUIEvent.OnMessageSending({
                                chatId: chatInfo.chatObjectId,
                                chatType: chatInfo.chatType,
                                msgId: args[1],
                                contentType: uc.constants.Message.ContentType.CloudFile
                            })
                        });
                    break;
                case uc.constants.Chat.ChatType.MeetingChat:
                    uc.IMeetingService.uploadCloudFile(
                        chatInfo.eventId,
                        parseInt(currentUserInfo.userId),
                        mountId,
                        data.filename,
                        parseInt(data.filesize),
                        uc.constants.Message.CloudType.GoKuai,
                        data.hash,
                        cloudFileInfo,
                        function (args) {
                            if (args[0] !== 0) {
                                console.log(args, {chatInfo: chatInfo, fileInfo: cloudFileInfo});
                                return;
                            }
                        var _extentData= $.extend(data,{"msgId":args[1]});
                        uc.modules.chat.ChatManager.OnSendCloudFile(chatInfo,_extentData);
                        uc.IUIEvent.OnMessageSending({
                            chatId: chatInfo.eventId,
                            chatType: chatInfo.chatType,
                            msgId: args[1],
                            contentType: uc.constants.Message.ContentType.CloudFile
                        })
                    });
                    break;
//                case uc.constants.Chat.ChatType.GroupChat:
//                    uc.IGroupService.uploadCloudDocument(
//                        chatInfo.chatObjectId,
//                        parseInt(currentUserInfo.userId),
//                        1,//isSendMessage,0-no ,1-send;
//                        parseInt(data.mountid),
//                        data.filename,
//                        parseInt(data.filesize),
//                        uc.constants.Message.CloudType.GoKuai,
//                        data.hash,
//                        cloudFileInfo,
//                        function (args) {
//                            console.log(args[0]+",@@@@@,cloudFileInfo:"+cloudFileInfo);
//                            if (args[0] !== 0) {
//                                console.log(args, {chatInfo: chatInfo, fileInfo: cloudFileInfo});
//                                return;
//                            }
//                            var _extentData= $.extend(data,{"msgId":args[1]});
//                            uc.modules.chat.ChatManager.OnSendCloudFile(chatInfo,_extentData);
//                            uc.IUIEvent.OnMessageSending({
//                                chatId: chatInfo.chatObjectId,
//                                chatType: chatInfo.chatType,
//                                msgId: args[1],
//                                contentType: uc.constants.Message.ContentType.CloudFile
//                            })
//                        });
//                    break;
//                    uc.IGroupService.sendCloudFileMessage(
//                        chatInfo.chatType,
//                        uc.constants.Group.GroupCloudType.CloudFileCreate,
//                        { "userId": chatInfo.chatObjectId,
//                          "siteId": chatInfo.siteId,
//                          "resId":  chatInfo.resId
//                        },
//                        data.filename,
//                        "",//fileUrl
//                        data.filesize,
//                        0,//count
//                        data.dir,//isDir
//                        uc.constants.Message.CloudType.GoKuai,
//                        cloudFileInfo,
//                        function (args) {
//                            console.log(args[0]+",@"+data.filename);
//                            if (args[0] !== 0) {
//                                console.log(args,data.filename, data.filesize+",chatInfo："+chatInfo);
//                                return;
//                            }
//                            var _extentData= $.extend(data,{"msgId":args[1]});
//                            uc.modules.chat.ChatManager.OnSendCloudFile(chatInfo,_extentData);
//                            uc.IUIEvent.OnMessageSending({
//                                chatId: chatInfo.chatObjectId,
//                                chatType: chatInfo.chatType,
//                                msgId: args[1],
//                                contentType: uc.constants.Message.ContentType.CloudFile
//                            })
//                        });
            }
        },
        forwardCloudFileMessage: function (fileName, fileSize, fileURL,content,dataArr) {
            var currentUserInfo = uc.IContactService.getCurrentUserInfo();
            var fileSize = Number(fileSize),mountId="";
            if(dataArr.hasOwnProperty("mount_id")){
                mountId=parseInt(dataArr.mount_id);
            }else if(dataArr.hasOwnProperty("mountId")){
                mountId=parseInt(dataArr.mountId);
            }else if(dataArr.hasOwnProperty("mountid")){
                mountId=parseInt(dataArr.mountid);
            }
            uc.IUIService.openChooseContactWindow(
                {
                    key: "forwardMessage" + (new Date().getTime()),
                    allowMultipleSelect: true,
                    allowMultipleSelectConversations: uc.constants.chooseContacts.allowMultipleSelectConversations,
                    allowChooseOrg:false
                }, function (args) {
                    console.log(args);
                    if (!args.okOrCancel) {
                        return;
                    }

                    if(args.otherItem.groupChats.length > 0){
                        var groupChats = args.otherItem.groupChats;
                        $.each(groupChats, function(index, groupChat){
                            uc.IGroupService.uploadCloudDocument(
                                groupChat,
                                parseInt(currentUserInfo.userId),
                                1,//isSendMessage,0-no ,1-send;
                                mountId,
                                fileName,
                                fileSize,
                                uc.constants.Message.CloudType.GoKuai,
                                dataArr.hash,
                                content,
                                function (args) {
                                    console.log(args[0]+",fileName:"+fileName);
                                    if (args[0] !== 0) {
                                        console.log(args, {chatInfo: groupChat, fileInfo: content});
                                        return;
                                    }
                                    //  var _extentData= $.extend(dataArr,{"msgId":args[1]});
                                    //  uc.modules.chat.ChatManager.OnSendCloudFile(dataArr,_extentData);
                                    uc.IUIEvent.OnMessageSending({
                                        chatId: groupChat,
                                        chatType: uc.constants.Chat.ChatType.GroupChat,
                                        msgId: args[1],
                                        contentType: uc.constants.Message.ContentType.CloudFile
                                    })
                                });
                        });
                    }

                    if(args.otherItem.projects.length > 0){
                        var projects = args.otherItem.projects;
                        $.each(projects, function(index, project){
                            uc.IGroupService.uploadCloudDocument(
                                project,
                                parseInt(currentUserInfo.userId),
                                1,//isSendMessage,0-no ,1-send;
                                mountId,
                                fileName,
                                fileSize,
                                uc.constants.Message.CloudType.GoKuai,
                                dataArr.hash,
                                content,
                                function (args) {
                                    console.log(args[0]+",fileName:"+fileName);
                                    if (args[0] !== 0) {
                                        console.log(args, {chatInfo: project, fileInfo: content});
                                        return;
                                    }
                                    //  var _extentData= $.extend(dataArr,{"msgId":args[1]});
                                    //  uc.modules.chat.ChatManager.OnSendCloudFile(dataArr,_extentData);
                                    uc.IUIEvent.OnMessageSending({
                                        chatId:project,
                                        chatType: uc.constants.Chat.ChatType.ProjectChat,
                                        msgId: args[1],
                                        contentType: uc.constants.Message.ContentType.CloudFile
                                    })
                                });
                        });
                    }

                    if(args.otherItem.contacts.length > 0){
                        var contacts = args.otherItem.contacts;
                        $.each(contacts, function(index, contact){
                            uc.IIMService.sendCloudFileMessage(
                                {   "userId": contact,
                                    "siteId": currentUserInfo.siteId,
                                    "resId": 100
                                },
                                fileName,
                                fileSize,
                                uc.constants.Message.CloudType.GoKuai,
                                content,
                                function (args) {
                                    console.log(args[0]+",fileName:"+fileName);
                                    if (args[0] !== 0) {
                                        console.log(args,fileName, fileSize+",chatInfo："+content);
                                        return;
                                    }
                                    //  var _extentData= $.extend(dataArr,{"msgId":args[1]});
                                    //  uc.modules.chat.ChatManager.OnSendCloudFile(dataArr,_extentData);
                                    uc.IUIEvent.OnMessageSending({
                                        chatId: contact,
                                        chatType:uc.constants.Chat.ChatType.SingleChat,
                                        msgId: args[1],
                                        contentType: uc.constants.Message.ContentType.CloudFile
                                    })
                                });
                        });
                    }
                    // -------------------------added by xue.bai_2 on 2016/04/18--------------------------------end

                    // old code
                    //var conversationType, toId;
                    //if (args.otherItem.groupChats.length > 0) {
                    //    conversationType = uc.constants.Chat.ChatType.GroupChat;
                    //    toId = args.otherItem.groupChats[0];
                    //} else if (args.otherItem.projects.length > 0) {
                    //    conversationType = uc.constants.Chat.ChatType.ProjectChat;
                    //    toId = args.otherItem.projects[0];
                    //} else if (args.contacts.length > 0) {
                    //    conversationType = uc.constants.Chat.ChatType.SingleChat;
                    //    toId = args.contacts[0].userId;
                    //} else {
                    //    return;
                    //}
                    //switch (conversationType) {
                    //    case uc.constants.Chat.ChatType.SingleChat:
                    //        uc.IIMService.sendCloudFileMessage(
                    //            { "userId": toId,
                    //               "siteId": currentUserInfo.siteId,
                    //               "resId": 100
                    //            },
                    //            fileName,
                    //            fileSize,
                    //            uc.constants.Message.CloudType.GoKuai,
                    //            content,
                    //            function (args) {
                    //                console.log(args[0]+",fileName:"+fileName);
                    //                if (args[0] !== 0) {
                    //                    console.log(args,fileName, fileSize+",chatInfo："+content);
                    //                    return;
                    //                }
                    //              //  var _extentData= $.extend(dataArr,{"msgId":args[1]});
                    //              //  uc.modules.chat.ChatManager.OnSendCloudFile(dataArr,_extentData);
                    //                uc.IUIEvent.OnMessageSending({
                    //                    chatId: toId,
                    //                    chatType:conversationType,
                    //                    msgId: args[1],
                    //                    contentType: uc.constants.Message.ContentType.CloudFile
                    //                })
                    //            });
                    //        break;
                    //    case uc.constants.Chat.ChatType.ProjectChat:
                    //    case uc.constants.Chat.ChatType.GroupChat:
						//case uc.constants.Chat.ChatType.MeetingChat:
                    //        uc.IGroupService.uploadCloudDocument(
                    //            toId,
                    //            parseInt(currentUserInfo.userId),
                    //            1,//isSendMessage,0-no ,1-send;
                    //            mountId,
                    //            fileName,
                    //            fileSize,
                    //            uc.constants.Message.CloudType.GoKuai,
                    //            dataArr.hash,
                    //            content,
                    //            function (args) {
                    //                console.log(args[0]+",fileName:"+fileName);
                    //                if (args[0] !== 0) {
                    //                    console.log(args, {chatInfo: toId, fileInfo: content});
                    //                    return;
                    //                }
                    //              //  var _extentData= $.extend(dataArr,{"msgId":args[1]});
                    //              //  uc.modules.chat.ChatManager.OnSendCloudFile(dataArr,_extentData);
                    //                uc.IUIEvent.OnMessageSending({
                    //                    chatId:toId,
                    //                    chatType: conversationType,
                    //                    msgId: args[1],
                    //                    contentType: uc.constants.Message.ContentType.CloudFile
                    //                })
                    //            });
                    //        break;
//                        case uc.constants.Chat.ChatType.GroupChat:
//                            uc.IGroupService.uploadCloudDocument(
//                                toId,
//                                parseInt(currentUserInfo.userId),
//                                1,//isSendMessage,0-no ,1-send;
//                                mountId,
//                                fileName,
//                                fileSize,
//                                uc.constants.Message.CloudType.GoKuai,
//                                dataArr.hash,
//                                content,
//                                function (args) {
//                                    console.log(args[0]+",fileName:"+fileName);
//                                    if (args[0] !== 0) {
//                                        console.log(args, {chatInfo: toId, fileInfo: content});
//                                        return;
//                                    }
//                                    //  var _extentData= $.extend(dataArr,{"msgId":args[1]});
//                                    //  uc.modules.chat.ChatManager.OnSendCloudFile(dataArr,_extentData);
//                                    uc.IUIEvent.OnMessageSending({
//                                        chatId:toId,
//                                        chatType: conversationType,
//                                        msgId: args[1],
//                                        contentType: uc.constants.Message.ContentType.CloudFile
//                                    })
//                                });
//                            break;
//                            uc.IGroupService.sendCloudFileMessage(
//                                conversationType,
//                                uc.constants.Group.GroupCloudType.CloudFileCreate,
//                                { "userId": toId,
//                                   "siteId":currentUserInfo.siteId,
//                                   "resId":  100
//                                },
//                                fileName,
//                                "",//fileUrl
//                                fileSize,
//                                0,//count
//                                0,//isDir
//                                uc.constants.Message.CloudType.GoKuai,
//                                content,
//                                function (args) {
//                                    console.log(args[0]+","+fileName);
//                                    if (args[0] !== 0) {
//                                        console.log(args,fileName, fileSize+",chatInfo："+content);
//                                        return;
//                                    }
//                                 //   var _extentData= $.extend(dataArr,{"msgId":args[1]});
//                                 //   uc.modules.chat.ChatManager.OnSendCloudFile(dataArr,_extentData);
//                                    uc.IUIEvent.OnMessageSending({
//                                        chatId:toId,
//                                        chatType: conversationType,
//                                        msgId: args[1],
//                                        contentType: uc.constants.Message.ContentType.CloudFile
//                                    })
//                                });
//                            break;
                    //}
                });
        },
        cloudChatGetFileId:function(fileSize,fileName,cloudFileInfoStr,options){
            var cloudInfoArr=JSON.parse(cloudFileInfoStr);
            var currentUserInfo = uc.IContactService.getCurrentUserInfo();
            uc.IGroupService.uploadCloudDocument(
                parseInt(options.chatObjId),
                parseInt(currentUserInfo.userId),
                0,//isSendMessage,0-no ,1-send;
                parseInt(cloudInfoArr.mount_id),
                fileName,
                parseInt(fileSize),
                uc.constants.Message.CloudType.GoKuai,
                cloudInfoArr.hash,
                cloudFileInfoStr,
                function (args) {
                    console.log(args[0]+",fileName:"+fileName+",cloudFileInfoStr:"+cloudFileInfoStr);
                    if (args[0] !== 0) {
                        console.log(args, {chatInfo:  options.chatObjId, fileInfo: cloudFileInfoStr});
                        return;
                    }
                });
        }
    }
})(uc);