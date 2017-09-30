/**
 * Created by senlin.ouyang on 2015/7/21.
 */
(function (uc) {
    var _this = null;
    uc.modules.conversation.ConversationManager = {
        sortedConversationList: [],
        topIndexConversationList: [],
        notifyDataChangedTimer: -1,
        notifyDataChangedDelay: 50,
        historyConversationLoadEnd: false,
        selectedConversationInfo: null,
        maxConversationUIIndex: 0, //conversation UI index in conversation list, if conversation is open from other module, t
        // he latest opened conversation index is the maximum
        noContactInfoUserIdArray: [],
        noGroupInfoGroupIdArray: [],
        noMeetingInfoArray: [],
        noOpenApiInfoArray: [],
        closeAppList: new uc.modules.openApi.OpenApiDataSet(),
        groupAvatarUserCount: 2,
        newMessageCount: 0,
        newOpenWindowTimeOffset: 1,
        moduleId: uc.constants.UI.ModuleId.Message,
        init: function () {
            uc.util.LogUtil.info('ConversationManager', 'InitConversationManager', "", null);
            uc.IUIService.openChatWindow(uc.constants.Chat.ChatType.None);

            _this = this;
            var loginUserInfo = uc.IClientService.getCurrentUCID();
            if (loginUserInfo) {
                this.loginUerId = loginUserInfo.userId;
            }
            this.conversationDataSet = new uc.modules.conversation.ConversationDataSet();
            this.conversationTimeLine = {
                startTimeStamp: Number.POSITIVE_INFINITY,
                endTimeStamp: Number.NEGATIVE_INFINITY
            };
            this.getLocalConversationList();
            this.initConversationList();
        },

        OnInitClient: function (clearData) {
            if (clearData) {
                this.syncConversationList();
            } else {
                this.init();
            }
        },

        initConversationList: function () {
            this.conversationListAdapter = new uc.modules.conversation.ConversationListAdapter(this);
            this.conversationListAdapter.init();
            this.conversationList = new uc.modules.conversation.ConversationList(this.conversationListAdapter);
            this.conversationList.init();
        },

        getLocalConversationList: function () {
            uc.IClientService.getLocalConversationList(uc.constants.Conversation.ConversationCountForSync);
        },

        conversationDataToConversationInfo: function (conversationData) {
            var conversationId = conversationData.relateID;
            var conversationType = conversationData.type;
            var conversationInfo = this.getConversation(conversationId, conversationType);
            var options = {
                conversationId: conversationId,
                conversationType: conversationType,
                conversation: conversationData.conversation,
                meetingId: conversationData.conferenceId, //meeting id
                eventId: conversationData.eventId, //event id
                accreditUserId: conversationData.authorizerId,
                newMsgCount: conversationData.newMsgCount,
                timestamp: conversationData.lastMsgTime,
                hidden: conversationData.hidden,
                atCounts: conversationData.atCounts,
                topIndex: conversationData.topIndex
            };
            if (conversationInfo) {
                conversationInfo.update(options);
            } else {
                conversationInfo = new uc.modules.conversation.ConversationInfo();
                conversationInfo.init(options);
                this.setConversationTitleAndAvatar(conversationInfo);
            }
            return conversationInfo;
        },
        setConversationTitleAndAvatar: function (conversationInfo) {
            var conversationId = conversationInfo.conversationId,
                conversationType = conversationInfo.conversationType;
            var title, contactInfo, userId, avatarIdArray = [], avatarUrlArray = [], meetingId = 0,
                meetingStartTime = 0, eventId = 0, isGNet = 0, isMeetingCancelled = 0, memberType, disturbStatus;
            switch (conversationType) {
                case uc.constants.Conversation.ConversationType.SingleChat:
                    contactInfo = uc.modules.cache.BasicContactInfoCache.getBasicContactInfo(Number(conversationId));
                    if (contactInfo) {
                        title = contactInfo.displayName ? contactInfo.displayName : '' + contactInfo.userId;
                        if (contactInfo.localAvatar) {
                            avatarUrlArray.push(contactInfo.localAvatar);
                        } else {
                            avatarUrlArray.push(uc.constants.UI.DefaultLocalUrl.Conversation.Avatar);
                            uc.IUIService.downloadUserAvatar(conversationId, contactInfo.remoteAvatarUrl);
                        }
                    } else {
                        title = '' + conversationId;
                        avatarUrlArray.push(uc.constants.UI.DefaultLocalUrl.Conversation.Avatar);
                        this.addNoContactInfoUserId(conversationId);
                    }
                    avatarIdArray.push(conversationId);
                    break;
                case uc.constants.Conversation.ConversationType.GroupChat:
                    var groupInfo = uc.IGroupService.getGroupInfo(conversationId);
                    if (groupInfo) {
                        title = groupInfo.name ? groupInfo.name : groupInfo.id;
                        avatarIdArray = uc.modules.group.GroupManager.getGroupTwoMembers(groupInfo.groupId, groupInfo);
                        memberType = groupInfo.memberType ? groupInfo.memberType : 0;
                        disturbStatus = groupInfo.disturbState;

                        $.each(avatarIdArray, function (key, item) {
                            if (item) {
                                var contact = uc.modules.cache.BasicContactInfoCache.getBasicContactInfo(Number(item));
                                if (contact) {
                                    if (contact.localAvatar) {
                                        avatarUrlArray.push(contact.localAvatar);
                                    } else {
                                        avatarUrlArray.push(uc.constants.UI.DefaultLocalUrl.Conversation.Avatar);
                                        uc.IUIService.downloadUserAvatar(item, contact.remoteAvatarUrl);
                                    }
                                } else {
                                    avatarUrlArray.push(uc.constants.UI.DefaultLocalUrl.Conversation.Avatar);
                                    _this.addNoContactInfoUserId(item);
                                }
                            }
                        });
//                        var adminId = groupInfo.adminId;
//                        if(adminId){
//                            contactInfo = uc.IContactService.getBasicContactInfo(adminId);
//                            if(contactInfo){
//                                if(contactInfo.localAvatar){
//                                    avatarUrlArray.push(contactInfo.localAvatar);
//                                }else{
//                                    avatarUrlArray.push(uc.constants.UI.DefaultLocalUrl.Conversation.Avatar);
//                                    uc.IUIService.downloadUserAvatar(adminId, contactInfo.remoteAvatarUrl);
//                                }
//                            }else{
//                                avatarUrlArray.push(uc.constants.UI.DefaultLocalUrl.Conversation.Avatar);
//                                this.addNoContactInfoUserId(adminId);
//                            }
//                            avatarIdArray.push(adminId);
//                        }
//                        var members = groupInfo.members;
//                        if(members && members.length){
//                            var userCount = this.groupAvatarUserCount - avatarIdArray.length;
//                            for(var i = 0, count = 0;  count < userCount && i < members.length; i++){
//                                userId = members[i].userId;
//                                if(!userId || avatarIdArray.indexOf(userId) != -1){
//                                    continue;
//                                }
//                                contactInfo = uc.IContactService.getBasicContactInfo(Number(userId));
//                                if(contactInfo){
//                                    if(contactInfo.localAvatar){
//                                        avatarUrlArray.push(contactInfo.localAvatar);
//                                    }else{
//                                        avatarUrlArray.push(uc.constants.UI.DefaultLocalUrl.Conversation.Avatar);
//                                        uc.IUIService.downloadUserAvatar(userId, contactInfo.remoteAvatarUrl);
//                                    }
//                                }else{
//                                    avatarUrlArray.push(uc.constants.UI.DefaultLocalUrl.Conversation.Avatar);
//                                    this.addNoContactInfoUserId(userId);
//                                }
//                                avatarIdArray.push(userId);
//                                count++;
//                            }
//                        }
//                        for(var j = avatarIdArray.length; j < this.groupAvatarUserCount; j++){
//                            avatarUrlArray.push(uc.constants.UI.DefaultLocalUrl.Conversation.Avatar);
//                        }
                    } else {
                        if (!conversationInfo.title) {
                            title = '' + conversationId;
                        }
                        this.addNoGroupInfoGroupId(conversationId);
                        for (var k = avatarIdArray.length; k < this.groupAvatarUserCount; k++) {
                            avatarUrlArray.push(uc.constants.UI.DefaultLocalUrl.Conversation.Avatar);
                        }
                    }
                    break;
                case uc.constants.Conversation.ConversationType.ProjectChat:
                    var projectInfo = uc.IGroupService.getGroupInfo(conversationId);
                    if (projectInfo) {
                        title = title ? title : projectInfo.name;
                        memberType = projectInfo.memberType ? projectInfo.memberType : 0;
                        disturbStatus = projectInfo.disturbState;

                        if (projectInfo.localAvatar) {
                            avatarUrlArray.push(projectInfo.localAvatar);
                        } else {
                            avatarUrlArray.push(uc.constants.UI.DefaultLocalUrl.Conversation.Project);
                            if (projectInfo.avatarIndex) {
                                uc.IUIService.downloadProjectAvatar(conversationId, projectInfo.avatarIndex);
                            }
                        }
                    } else {
                        if (!conversationInfo.title) {
                            title = '' + conversationId;
                        }
                        avatarUrlArray.push(uc.constants.UI.DefaultLocalUrl.Conversation.Project);
                        this.addNoGroupInfoGroupId(conversationId);
                    }
                    avatarIdArray.push(conversationId);
                    break;
                case uc.constants.Conversation.ConversationType.CloudChat:
                    var projectInfo = uc.IGroupService.getGroupInfo(conversationId);
                    if (projectInfo) {
                        title = title ? title : projectInfo.name;
                        memberType = projectInfo.memberType ? projectInfo.memberType : 0;

                        if (projectInfo.localAvatar) {
                            avatarUrlArray.push(projectInfo.localAvatar);
                        } else {
                            avatarUrlArray[0] = uc.constants.UI.DefaultLocalUrl.Conversation.CloudFile;
                            if (projectInfo.avatarIndex) {
                                if (('' + projectInfo.avatarIndex).indexOf('http') == 0) {
                                    avatarUrlArray[0] = projectInfo.avatarIndex;
                                }
                            }
                        }
                    } else {
                        if (!conversationInfo.title) {
                            title = '' + conversationId;
                        }
                        avatarUrlArray.push(uc.constants.UI.DefaultLocalUrl.Conversation.CloudFile);
                        this.addNoGroupInfoGroupId(conversationId);
                    }
                    avatarIdArray.push(conversationId);
                    break;
                case uc.constants.Conversation.ConversationType.MeetingMsg:
                    eventId = conversationInfo.eventId;
                    meetingId = conversationInfo.meetingId;
                    var meetingInfoArray = uc.IMeetingService.getMeetingInfoByIdSync(/* meetingId, */ eventId, 0, 0, conversationInfo.accreditUserId);
                    uc.util.LogUtil.info("ConversationManager", "setConversationTitleAndAvatar", "getMeetingInfoByIdSync",
                        {
                            meetingId: meetingId, eventId: eventId, accreditUserId: conversationInfo.accreditUserId,
                            meetingStartTime: 0, meetings: meetingInfoArray
                        });
                    if (meetingInfoArray && meetingInfoArray[0]) {
                        var meetingInfo = meetingInfoArray[0];
                        title = meetingInfo.title;
                        meetingStartTime = meetingInfo.startTime;
                        eventId = meetingInfo.eventId;
                        isGNet = meetingInfo.isGNet;
                        isMeetingCancelled = 0; //TODO use correct status
                    } else {
                        uc.util.LogUtil.info("ConversationManager", "setConversationTitleAndAvatar", "nogetMeetingInfoById",
                            {
                                conversationInfo: conversationInfo

                            });
                        if (!conversationInfo.title) {
                            title = '' + conversationId;
                        }
                        this.addNoMeetingInfoEventId(eventId, meetingId);
                    }
                    break;
                case uc.constants.Conversation.ConversationType.OpenApi:
                    var openApiInfo;
                    if(uc.modules.notice.NoticeManager.isNoticeConversation(conversationId)){
                      openApiInfo = uc.modules.notice.NoticeManager.getAppInfo();
                    }else{
                      openApiInfo = uc.modules.openApi.OpenApiManager.getOpenApiInfo(conversationId);
                    }
                    if (openApiInfo) {
                        title = openApiInfo.appTitle;
                        if (openApiInfo.appLogo) {//Api logo is not on FS server, can't download it via FS service
                            avatarUrlArray.push(openApiInfo.appLogo);
                        } else {
                            avatarUrlArray.push(uc.constants.UI.DefaultLocalUrl.Conversation.OpenApi);
                        }
                    } else {
                        title = uc_resource.OpenApi.OpenApiClosed + conversationId;
                        avatarUrlArray.push(uc.constants.UI.DefaultLocalUrl.Conversation.OpenApi);
                        this.noOpenApiInfoArray.push(conversationId);
                        conversationInfo.isCancelled = 1;
                        uc.modules.openApi.OpenApiManager.getAppsInfo([conversationId]);
                    }
                    avatarIdArray.push(conversationId);
                    break;
            }
            conversationInfo.title = title;
            conversationInfo.eventId = eventId;
            conversationInfo.meetingStartTime = meetingStartTime;
            conversationInfo.isGNet = isGNet;
            conversationInfo.isMeetingCancelled = isMeetingCancelled;
            conversationInfo.avatarIdArray = avatarIdArray;
            conversationInfo.avatarUrlArray = avatarUrlArray;
            conversationInfo.memberType = memberType ? memberType : 0;
            conversationInfo.disturbStatus = disturbStatus;
        },
        updateConversationTitle: function (conversationId, conversationType, title) {
            var conversationInfo = this.getConversation(conversationId, conversationType);
            if (conversationInfo) {
                conversationInfo.title = title;
                this.notifyDataSetChanged();
            }
        },
        updateProjectAvatar: function (conversationId, conversationType, serverAvatar) {
            uc.util.LogUtil.info('ConversationManager', 'updateProjectAvatar', 'conversationId = ' + conversationId + ', conversationType = ' + conversationType + '', null);
            var conversationInfo = this.getConversation(conversationId, conversationType);
            if (conversationInfo) {
                if (conversationType == uc.constants.Conversation.ConversationType.ProjectChat) {
                    conversationInfo.avatarUrlArray = [uc.constants.UI.DefaultLocalUrl.Conversation.Project];
                    uc.IUIService.downloadProjectAvatar(conversationId, serverAvatar);
                    this.notifyDataSetChanged();
                } else if (conversationType == uc.constants.Conversation.ConversationType.CloudChat) {
                    conversationInfo.avatarUrlArray = [serverAvatar];
                    //uc.IUIService.downloadProjectAvatar(conversationId, serverAvatar);
                    console.log('%c' + serverAvatar, 'color:red');
                    this.notifyDataSetChanged();
                }
            }
        },
        updateGroupAvatar: function (conversationId, conversationType, members) {
            uc.util.LogUtil.info('ConversationManager', 'updateProjectAvatar', 'conversationId = ' + conversationId + ', conversationType = ' + conversationType + '', null);
            var conversationInfo = this.getConversation(conversationId, conversationType);
            if (conversationInfo) {
                var needUpdateAvatar = false;
                for (var i = 0, len = members.length; i < len; i++) {
                    if (conversationInfo.avatarIdArray.indexOf(members[i].userId) != -1) {
                        needUpdateAvatar = true;
                        break;
                    }
                }
                if (needUpdateAvatar) {
                    this.setConversationTitleAndAvatar(conversationInfo);
                    this.notifyDataSetChanged();
                }
            }
        },
        setMeetingStatus: function (conversationId, conversationType, isCancelled) {
            var conversationInfo = this.getConversation(conversationId, conversationType);
            if (conversationInfo) {
                if (conversationType == uc.constants.Conversation.ConversationType.MeetingMsg) {
                    conversationInfo.isMeetingCancelled = isCancelled;
                    this.notifyDataSetChanged();
                }
            }
        },
        addNoContactInfoUserId: function (userId) {
            this.noContactInfoUserIdArray.push(userId);
        },
        addNoGroupInfoGroupId: function (groupId) {
            this.noGroupInfoGroupIdArray.push(groupId);
        },
        addNoMeetingInfoEventId: function (eventId, meetingId) {
            var data = {
                eventId: eventId,
                meetingId: meetingId
            };
            this.noMeetingInfoArray.push(data);
        },
        syncConversationList: function () {
            uc.util.LogUtil.info('ConversationManager', 'syncConversationList', 'get conversation list from server, count = ' + uc.constants.Conversation.ConversationCountForSync, null);
            this.isLoadingMore = false;
            uc.IClientService.syncConversationList(uc.constants.Conversation.ConversationCountForSync);
        },

        isHistoryConversationLoadEnd: function () {
            return this.historyConversationLoadEnd;
        },
        loadMoreHistoryConversation: function () {
            this.isLoadingMore = true;
            uc.util.LogUtil.info('ConversationManager', 'loadMoreHistoryConversation', 'get more conversation list from server, count = ' + uc.constants.Conversation.ConversationCountForSync
                + ', startTime = ' + this.conversationTimeLine.startTimeStamp, null);
            this.conversationList.showMoreHistoryConversationLoading();
            uc.IClientService.getHistoryConversationList(this.conversationTimeLine.startTimeStamp, uc.constants.Conversation.ConversationCountForSync);
        },
        sortConversation: function () {
            this.sortedConversationList = this.getAllNotTopConversations();//not top index conversation;
            this.sortedConversationList.sort(this.compareFunction);

            this.topIndexConversationList = this.getAllTopIndexConversations();//top index conversation;
            //this.topIndexConversationList.sort(this.compareTopIndexFunction);  //请不要删除
            this.topIndexConversationList.sort(this.compareFunction);
            this.sortedConversationList = this.topIndexConversationList.concat(this.sortedConversationList);
            uc.util.LogUtil.info('ConversationManager', 'sortConversation', 'conversationList', {conversationList: this.sortedConversationList});
            return this.sortedConversationList;
        },
        compareFunction: function (conversationInfo1, conversationInfo2) {
            if (conversationInfo1.isInCall && !conversationInfo2.isInCall) {
                return -1;
            } else if (!conversationInfo1.isInCall && conversationInfo2.isInCall) {
                return 1;
            } else {
                return Math.max(conversationInfo2.timestamp, conversationInfo2.fakeTime) - Math.max(conversationInfo1.timestamp, conversationInfo1.fakeTime);
            }
        },
        compareTopIndexFunction: function (conversationInfo1, conversationInfo2) {
            if (conversationInfo1.isInCall && !conversationInfo2.isInCall) {
                return -1;
            } else if (!conversationInfo1.isInCall && conversationInfo2.isInCall) {
                return 1;
            } else {
                return conversationInfo2.topIndex - conversationInfo1.topIndex;
            }
        },
        addConversation: function (conversationId, conversationType, conversationInfo) {
            if (!conversationInfo) {
                return;
            }
            uc.util.LogUtil.info('ConversationManager', 'addConversation', 'conversationInfo', {conversationInfo: conversationInfo});
            if (this.conversationDataSet.isConversationExist(conversationId, conversationType)) {
                this.updateConversation(conversationId, conversationType, conversationInfo);
            } else {
                this.conversationDataSet.addConversation(conversationId, conversationType, conversationInfo);
                this.notifyDataSetChanged();
            }
        },
        removeConversation: function (conversationId, conversationType) {
            if (this.conversationDataSet.isConversationExist(conversationId, conversationType)) {
                var conversationInfo = this.getConversation(conversationId, conversationType);
                this.decreaseNewMsgCount(conversationType, conversationInfo.newMsgCount ? conversationInfo.newMsgCount : 0);
                this.conversationDataSet.removeConversation(conversationId, conversationType);
                if (this.isChatWindowOpen(conversationId, conversationType)) {
                    this._updateSelectedConversation();
                }
                this.notifyDataSetChanged();
            }
        },
        setConversationOnTop: function (conversationType, conversationId, isTop) {
            uc.util.LogUtil.info('ConversationManager', 'setConversationOnTop', 'params:', {
                conversationType: conversationType,
                conversationId: conversationId,
                isTop: isTop
            });
            uc.IClientService.setConversationTop(conversationType, conversationId, isTop);
        },
        _updateSelectedConversation: function () {
            var index, conversationInfo;
            index = this.conversationList.getNextSelectedConversationIndex();
            if (index != -1) {
                conversationInfo = this.sortedConversationList[index];
                if (conversationInfo) {
                    var meetingData = {
                        meetingId: conversationInfo.meetingId,
                        eventId: conversationInfo.eventId,
                        startTime: conversationInfo.meetingStartTime,
                        isCancelled: conversationInfo.isMeetingCancelled
                    };
                    uc.IUIService.openChatWindow(this.conversationTypeToChatType(conversationInfo.conversationType), conversationInfo.conversationId, uc.constants.UI.OpenChatWindowSourceModule.Conversation, meetingData, 0, false);
                }
            }
        },

        updateConversation: function (conversationId, conversationType, conversationInfo) {
            if (!conversationInfo) {
                return;
            }
            if (!conversationInfo.title) {
                uc.util.LogUtil.info('ConversationManager', 'updateConversation', 'conversationId = ' + conversationId + ', conversationType = ' + conversationType
                    + ', conversationInfo = ' + JSON.stringify(conversationInfo) + ', message = ' + conversationInfo.message + ', timestamp = '
                    + conversationInfo.timestamp + ', newMsgCount = ' + conversationInfo.newMsgCount, null);
            }
            uc.util.LogUtil.info('ConversationManager', 'updateConversation', 'conversationId = ' + conversationId + ', conversationType = ' + conversationType
                + ', title = ' + conversationInfo.title + ', message = ' + conversationInfo.message + ', timestamp = '
                + conversationInfo.timestamp + ', newMsgCount = ' + conversationInfo.newMsgCount, null);
            if (this.conversationDataSet.isConversationExist(conversationId, conversationType)) {
                this.conversationDataSet.updateConversation(conversationId, conversationType, conversationInfo);
                this.notifyDataSetChanged();
            } else {
                this.addConversation(conversationId, conversationType, conversationInfo);
            }
        },
        getConversation: function (conversationId, conversationType) {
            return this.conversationDataSet.getConversation(conversationId, conversationType);
        },
        isConversationExist: function (conversationId, conversationType) {
            return this.conversationDataSet.isConversationExist(conversationId, conversationType);
        },
        notifyDataSetChanged: function () {
            if (!this.conversationListAdapter) {
                return;
            }
            clearTimeout(this.notifyDataChangedTimer);
            this.notifyDataChangedTimer = setTimeout(function () {
                _this._updateBubbleMessage();
                _this.conversationListAdapter.notifyDataSetChanged();
            }, this.notifyDataChangedDelay);
        },
        _updateBubbleMessage: function () {
            var sortedConversationList = _this.sortConversation();
            var conversationInfo, hasNewMsgConversations = [];
            for (var i = 0, len = sortedConversationList.length; i < len; i++) {
                conversationInfo = sortedConversationList[i];
                if (conversationInfo.newMsgCount) {
                    hasNewMsgConversations.push(conversationInfo);
                }
            }
            if (hasNewMsgConversations.length) {
                //group disturb state decide to bubble or not
                for (var k = 0; k < hasNewMsgConversations.length; k++) {
                    var groupDisturbState = hasNewMsgConversations[k].disturbStatus;
                    var currentUserStatus = uc.modules.main.Main.currentUserStatusValue;

                    if (groupDisturbState || currentUserStatus == uc.constants.UserStatusType.Busy) {
                        hasNewMsgConversations.splice(k);
                    }
                }
                if (hasNewMsgConversations.length) {
                    uc.modules.bubble.BubbleManager.updateBubbleMessageInfoList(hasNewMsgConversations);
                }
            }
        },

        getAllConversations: function () {
            return this.conversationDataSet.getAllConversations();
        },
        getAllNotTopConversations: function () {
            return this.conversationDataSet.getAllNotTopConversations();
        },
        getAllTopIndexConversations: function () {
            return this.conversationDataSet.getAllTopIndexConversations();
        },

        getConversationCount: function () {
            return this.conversationDataSet.getConversationCount();
        },

        getSortedConversationList: function (start, length) {
            var result = [];
            if (!this.sortedConversationList || this.sortedConversationList.length == 0) {
                return result;
            }
            if (start < 0 || start > this.sortedConversationList.length - 1) {
                return result;
            }
            if (length == -1) {
                length = this.sortedConversationList.length;
            }
            if (this.sortedConversationList.length - 1 >= start && start >= 0) {
                result = this.sortedConversationList.slice(start, start + length);
            }

            return result;
        },
        setConversationTimeLine: function (timeStamp) {
            timeStamp = parseInt(timeStamp);
            if (timeStamp < this.conversationTimeLine.startTimeStamp) {
                this.conversationTimeLine.startTimeStamp = timeStamp;
            }
            if (timeStamp > this.conversationTimeLine.endTimeStamp) {
                this.conversationTimeLine.endTimeStamp = timeStamp;
            }
        },
        clearConversationDraft: function (conversationId, conversationType) {
            return;
            uc.util.LogUtil.info('ConversationManager', 'clearConversationDraft', 'conversationInfo', {
                conversationId: conversationId,
                conversationType: conversationType
            });
            uc.modules.draft.DraftMessageManager.removeDraftMessage(conversationId, conversationType);
        },
        setTopIndexError: function (errorCode) {
            var msg = uc_resource.Main.SetTopFaild;
            switch (errorCode) {
                case 13002:
                    msg = uc_resource.Main.SetTopOverCount;
                    break;
            }

            uc.modules.component.PromptMessageBox.open({
                title: uc_resource.WindowTitle.Notification,
                message: msg,
                buttonMode: "ok",
                OkText: uc_resource.Common.OK
            }, function (result) {
                this.close();
            });
        },
        getLatestTopIndexConversation: function () {
            var conversation = null, conversations = this.conversationDataSet.getAllTopIndexConversations();
            if (conversations && conversations.length) {
                conversations = conversations.sort(this.compareFunction);
                conversation = conversations[0];
            }

            return conversation;
        },

        OnWindowResized: function () {
            this.conversationList.notifyDataSetChanged();
        },

        OnModuleChanged: function (moduleId) {
            uc.util.LogUtil.info('ConversationManager', 'OnModuleChanged', 'moduleId = ' + moduleId, null);
            if(this.moduleId != uc.constants.UI.ModuleId.Message && moduleId == uc.constants.UI.ModuleId.Message){
                uc.util.LogUtil.info('ConversationManager', 'OnModuleChanged', 'set selected conversation read.', null);
                this.setConversationMessagesRead(this.selectedConversationInfo);
                this.notifyDataSetChanged();
            }

            this.moduleId = moduleId;

            if(moduleId === uc.constants.UI.ModuleId.Message){ //当所选的conversation是广播站时 打开广播站列表
                var selectedConversationInfo = uc.modules.conversation.ConversationManager.selectedConversationInfo;
                if(selectedConversationInfo && uc.modules.notice.NoticeManager.isNoticeConversation(selectedConversationInfo.conversationId)){
                    uc.IUIService.openNoticeReceiverWindow();
                }
            }
        },
        OnOpenChatWindow: function (chatType, chatId, openChatWindowSourceModule, meetingData, siteId) {
            if (chatType == uc.constants.Chat.ChatType.None) {
                return;
            }
            var conversationType = this.chatTypeToConversationType(chatType);
            var conversationInfo = this.getConversation(chatId, conversationType);
            if (conversationInfo) {
                this.setConversationMessagesRead(conversationInfo);
                this.notifyDataSetChanged();
            }

            if (openChatWindowSourceModule == uc.constants.UI.OpenChatWindowSourceModule.Conversation &&
                this.selectedConversationInfo && this.selectedConversationInfo.conversationType == conversationType &&
                this.selectedConversationInfo.conversationId == chatId) {
                return;
            }
            if (!conversationInfo) {
                conversationInfo = this.createDefaultConversationInfo(chatId, chatType);
                if (meetingData) {
                    conversationInfo.updateData({
                        meetingId: meetingData.meetingId,
                        eventId: meetingData.eventId,
                        meetingStartTime: meetingData.startTime,
                        isMeetingCancelled: meetingData.isCancelled
                    });
                }
                this.setConversationTitleAndAvatar(conversationInfo);
            }
            if (openChatWindowSourceModule == uc.constants.UI.OpenChatWindowSourceModule.Other) {
                conversationInfo.isFakeTime = true;
                conversationInfo.fakeTime = this.conversationTimeLine.endTimeStamp + (this.newOpenWindowTimeOffset++);
            }
            this.addConversation(conversationInfo.conversationId, conversationInfo.conversationType, conversationInfo);
            this.setSelectedConversationInfo(conversationInfo);
            this.notifyDataSetChanged();
            if (openChatWindowSourceModule == uc.constants.UI.OpenChatWindowSourceModule.Other) {
                this.conversationList.scrollToTop();
            }
        },
        setConversationMessagesRead: function (conversationInfo) {
            if (conversationInfo && conversationInfo.newMsgCount) {
                var conversationType = conversationInfo.conversationType;
                var conversationId = conversationInfo.conversationId;
                switch (conversationType) {
                    case uc.constants.Conversation.ConversationType.GroupChat:
                    case uc.constants.Conversation.ConversationType.ProjectChat:
                    case uc.constants.Conversation.ConversationType.CloudChat:
                    case uc.constants.Conversation.ConversationType.MeetingMsg:
                        uc.IGroupService.sendMessagesReadById(conversationType, conversationInfo.conversationId);
                        break;
                    case uc.constants.Conversation.ConversationType.SingleChat:
                        uc.IIMService.sendMessagesReadById(conversationInfo.conversationId);
                        break;
                    case uc.constants.Conversation.ConversationType.OpenApi:
                        uc.IOpenApiService.sendMessagesReadById(conversationInfo.conversationId);
                        break;
                }
                this.decreaseNewMsgCount(conversationType, conversationInfo.newMsgCount, conversationId);
                conversationInfo.newMsgCount = 0;
                uc.modules.call.CallManager.clearNewMsgCount(conversationInfo.conversationId, conversationType);
            }
        },
        setNewMessageCount: function (newMsgCount) {
            this.newMessageCount = newMsgCount;
            uc.util.LogUtil.info('ConversationManager', 'setNewMessageCount', 'MessageCount = ' + newMsgCount, {newMessageCount: this.newMessageCount});
            uc.IUIEvent.OnNewMessageCountChanged(this.newMessageCount);
        },
        increaseNewMsgCount: function (conversationType, count, conversationId) {
            uc.util.LogUtil.info('ConversationManager', 'increaseNewMsgCount', 'conversationType = ' + conversationType, {
                count: count,
                newMessageCount: this.newMessageCount,
                conversationId: conversationId
            });
            if (this._needCalculateMessageCount(conversationType, conversationId)) {
                this.newMessageCount += count;
                uc.IUIEvent.OnNewMessageCountChanged(this.newMessageCount);
            }
        },
        decreaseNewMsgCount: function (conversationType, count, conversationId) {
            uc.util.LogUtil.info('ConversationManager', 'decreaseNewMsgCount', 'conversationType = ' + conversationType, {
                count: count,
                newMessageCount: this.newMessageCount,
                conversationId: conversationId
            });
            if (this._needCalculateMessageCount(conversationType, conversationId)) {
                this.newMessageCount -= count;
                uc.IUIEvent.OnNewMessageCountChanged(this.newMessageCount);
            }
        },
        createDefaultConversationInfo: function (chatId, chatType) {
            var conversationType = this.chatTypeToConversationType(chatType);
            var options = {
                conversationId: chatId,
                conversationType: conversationType
            };
            var conversationInfo = new uc.modules.conversation.ConversationInfo();
            conversationInfo.init(options);
            return conversationInfo;
        },
        setSelectedConversationInfo: function (conversationInfo) {
            if (!this.selectedConversationInfo) {
                conversationInfo.setSelected(true);
                this.selectedConversationInfo = conversationInfo;
            } else if (conversationInfo.conversationId != this.selectedConversationInfo.conversationId || conversationInfo.conversationType != this.selectedConversationInfo.conversationType) {
                this.selectedConversationInfo.setSelected(false);
                conversationInfo.setSelected(true);
                this.selectedConversationInfo = conversationInfo;
            }
        },
        reLoadMessage: function () {
            var _t = this;
            this.reLoad = true;
            $("#reLoadMessage").click(function () {
                $(this).parent().animate({height: '0', opacity: '1'}, "slow").hide();
                if (_t.isLocal) {
                    _t.getLocalConversationList();
                } else {
                    _t.syncConversationList();
                }
            });
        },
        loadConversationFailed: function () {
            if ($(".message .sub-menu-panel #conversationFailed").length) {
                $("#conversationFailed").animate({height: '100px', opacity: '1', lineHeight: '100px'}, "slow").show();
                $("#conversationFailed").animate({height: '20px', opacity: '1', lineHeight: '20px'}, "slow").show();
            } else {
                $(".message .sub-menu-panel").append("<div id='conversationFailed'><a>" + uc_resource.Message.ConversationListLoadFailed + "</a><a id='reLoadMessage'>" + uc_resource.Message.ReloadConversationList + "</a></div>");
                $("#conversationFailed").animate({height: '100px', opacity: '1', lineHeight: '100px'}, "slow").show();
                $("#conversationFailed").animate({height: '20px', opacity: '1', lineHeight: '20px'}, "slow").show();
            }
            if (!this.reLoad) {
                this.reLoadMessage();
            }
        },
        loadConversationSuccess: function () {
            if ($(".message .sub-menu-panel #conversationFailed").length) {
                $("#conversationFailed").animate({height: '0', opacity: '1'}, "slow").hide();
            }
        },
        OnConversationListReceived: function (errorCode, isLocal, conversations, totalUnread, groupAtCounts) {
            this.isLocal = isLocal;
            if (errorCode != uc.ErrorCode.Success) {
                this.loadConversationFailed();
                return;
            }
            this.loadConversationSuccess();
            if (isLocal) {
                this.syncConversationList();
                if (conversations && conversations.length == 1) {
                    this.conversationList.setConversationLoading();
                }
            }
            if (!isLocal && !this.isLoadingMore) {
                this.conversationDataSet.clear();
                this.conversationList.setConversationLoadEnd();
                this.conversationTimeLine.startTimeStamp = Number.POSITIVE_INFINITY;
                this.conversationTimeLine.endTimeStamp = Number.NEGATIVE_INFINITY;
                this.newOpenWindowTimeOffset = 1;
                this.newMessageCount = 0;
            }
            //if(!isLocal){
            //    this.conversationList.onMoreHistoryConversationLoadEnd();
            //}

            var conversationData, conversationInfo, firstConversation, selectedConversation;
            this.historyConversationLoadEnd = !isLocal && (conversations.length == 0);
            for (var i = 0, len = conversations.length; i < len; i++) {
                conversationData = conversations[i];
                if (!conversationData || conversationData.relateID == 0 || conversationData.relateID == this.loginUerId) {
                    continue;
                }
                //If meeting id and event id both are error, remove it,
                if (conversationData.type == uc.constants.Conversation.ConversationType.MeetingMsg && conversationData.eventId < 0 && conversationData.conferenceId < 0) {
                    continue;
                }
                conversationInfo = _this.conversationDataToConversationInfo(conversationData);
                if (!isLocal && !this.isLoadingMore && i == 0) {
                    firstConversation = conversationInfo;
                }
                if (!isLocal && !this.isLoadingMore && this.selectedConversationInfo //If is get latest conversation list from server, set the selected conversation
                    && this.selectedConversationInfo.conversationId == conversationInfo.conversationId
                    && this.selectedConversationInfo.conversationType == conversationInfo.conversationType) {
                    selectedConversation = conversationInfo;
                }
                if (conversationInfo.topIndex <= 0) {
                    this.setConversationTimeLine(conversationData.lastMsgTime);
                }
                _this.addConversation(conversationInfo.conversationId, conversationInfo.conversationType, conversationInfo);
            }
            if (!selectedConversation) {
                if (!isLocal && !this.isLoadingMore && this.selectedConversationInfo && firstConversation) {
                    selectedConversation = firstConversation;
                }
            }
            if (selectedConversation) {
                setTimeout(function () {
                    _this.selectedConversationInfo = null;
                    var meetingData = {
                        meetingId: selectedConversation.meetingId,
                        eventId: selectedConversation.eventId,
                        startTime: selectedConversation.meetingStartTime,
                        isCancelled: selectedConversation.isMeetingCancelled
                    };
                    uc.IUIService.openChatWindow(_this.conversationTypeToChatType(selectedConversation.conversationType),
                        selectedConversation.conversationId, uc.constants.UI.OpenChatWindowSourceModule.Conversation, meetingData, 0, false);
                    _this.conversationList.scrollToTop();
                }, 10);
            }
            this._getAllNewMsgCount();
            if(!this.isLocal){
                uc.IUIEvent.OnConversationDataUpdate();
            }
        },
        OnConversationListComplete: function (/*int*/conversationCount) {
            uc.util.LogUtil.info("ConversationManager", "OnConversationListComplete", "ConversationCount = " + conversationCount, null);
            this.conversationList.onMoreHistoryConversationLoadEnd();
            uc.util.LogUtil.info("ConversationManager", "OnConversationListComplete", "totalConversationCount = " + this.conversationDataSet.getConversationCount(), null);
            //If total count is less then requested, it means there is no conversation to list, will not load more
            if (conversationCount == uc.constants.Conversation.ConversationCountForSync && !this.isLocal) {
                if (this.conversationDataSet && this.conversationDataSet.getConversationCount() < this.conversationList.getConversationViewCountPerPage() + 1) {
                    this.loadMoreHistoryConversation();
                }
            }
            if (uc.modules.call.CallManager.inProgressCallInfo.isInProgress) {
                uc.IUIEvent.OnCallStarted(uc.modules.call.CallManager.inProgressCallInfo.chatId, uc.modules.call.CallManager.inProgressCallInfo.chatType);
            }
        },
        OnContactInfoReceived: function (contacts) {
            var contactInfo, userId;
            for (var i = 0, len = contacts.length; i < len; i++) {
                contactInfo = contacts[i];
                if (contactInfo) {
                    if (this.noContactInfoUserIdArray.indexOf(contactInfo.userId) != -1) {
                        userId = contactInfo.userId;
                        this.updateUserNameInMessageAndTitle(userId, contactInfo.displayName);
                        if (contactInfo.localAvatar) {
                            this.OnUserAvatarDownloaded(userId, contactInfo.localAvatar);
                        } else {
                            uc.IUIService.downloadUserAvatar(userId, contactInfo.remoteAvatarUrl);
                        }
                    }
                }
            }
        },
        updateUserNameInMessageAndTitle: function (userId, userName) {
            var allConversationInfo = this.getAllConversations();
            var title, message, conversationInfo;
            for (var i = 0, len = allConversationInfo.length; i < len; i++) {
                conversationInfo = allConversationInfo[i];
                title = conversationInfo.title;
                if (title && title.indexOf(userId) != -1) {
                    conversationInfo.title = title.replace('' + userId, userName);
                    this.updateConversation(conversationInfo.conversationId, conversationInfo.conversationType, conversationInfo);
                }
                if((conversationInfo.conversationId == userId)&&(userName != title)){
                    conversationInfo.title = title.replace(title, userName);
                    this.updateConversation(conversationInfo.conversationId, conversationInfo.conversationType, conversationInfo);
                }
                message = conversationInfo.message;
                if (message && message.indexOf(userId) != -1) {
                    //conversationInfo.message = message.replace('<span rel="'+ userId +'">' + userId + '</span>', userName);
                    conversationInfo.message = message.replace('' + userId, userName);
                    this.updateConversation(conversationInfo.conversationId, conversationInfo.conversationType, conversationInfo);
                }
            }
        },
        OnGroupInfoReceived: function (groupId, groupInfo) {
            if (groupInfo) {
                this.updateGroupNameInMessageAndTitle(groupInfo);
            }
        },
        updateGroupNameInMessageAndTitle: function (groupInfo) {
            if (this.noGroupInfoGroupIdArray.indexOf(groupInfo.id) == -1) {
                return;
            }
            var message;
            var conversationInfo = this.getConversation(groupInfo.id, uc.constants.Conversation.ConversationType.ProjectChat);
            if (!conversationInfo) {
                conversationInfo = this.getConversation(groupInfo.id, uc.constants.Conversation.ConversationType.GroupChat);
            }
            if (!conversationInfo) {
                conversationInfo = this.getConversation(groupInfo.id, uc.constants.Conversation.ConversationType.CloudChat);
            }
            if (conversationInfo) {
                message = conversationInfo.message;
                if (message && message.indexOf(groupInfo.id) != -1) {
                    //conversationInfo.message = message.replace('<span rel="'+ groupId +'">' + groupId + '</span>', groupName);
                    conversationInfo.message = message.replace('' + groupInfo.id, groupInfo.name);
                }
                this.setConversationTitleAndAvatar(conversationInfo);
                this.updateConversation(conversationInfo.conversationId, conversationInfo.conversationType, conversationInfo);
            }
        },
        OnUserAvatarDownloaded: function (userId, localAvatarUrl) {
            var allConversationInfo = this.getAllConversations();
            var avatarIdArray, avatarUrlArray, userIdIndex, conversationInfo, needRefreshConversationList = false;
            for (var i = 0, len = allConversationInfo.length; i < len; i++) {
                conversationInfo = allConversationInfo[i];
                if (conversationInfo.conversationType == uc.constants.Conversation.ConversationType.SingleChat ||
                    conversationInfo.conversationType == uc.constants.Conversation.ConversationType.GroupChat) {
                    avatarIdArray = conversationInfo.avatarIdArray;
                    avatarUrlArray = conversationInfo.avatarUrlArray;
                    userIdIndex = avatarIdArray.indexOf(userId);
                    if (userIdIndex != -1) {
                        needRefreshConversationList = true;
                        avatarUrlArray[userIdIndex] = localAvatarUrl;
                    }
                }
            }
            if (needRefreshConversationList) {
                this.notifyDataSetChanged();
            }
        },
        OnProjectAvatarDownloaded: function (groupId, localAvatarUrl) {
            var conversationInfo = this.getConversation(groupId, uc.constants.Conversation.ConversationType.ProjectChat);
            if (conversationInfo) {
                conversationInfo.avatarUrlArray[0] = localAvatarUrl;
                this.notifyDataSetChanged();
            }
        },
        OnMeetingInfoArrayReceived: function (meetingInfoArray) {
            var meetingInfo;
            var conversationInfo;
            for (var i = 0, meetingCount = meetingInfoArray.length; i < meetingCount; i++) {
                meetingInfo = meetingInfoArray[i];
                if (!this._isMeetingInfoFromConversation(meetingInfo.eventId, meetingInfo.conferenceId)) {
                    continue;
                }
                conversationInfo = this.getConversation(meetingInfo.groupId, uc.constants.Conversation.ConversationType.MeetingMsg);
                if (conversationInfo) {
                    conversationInfo.title = meetingInfo.title;
                    conversationInfo.meetingStartTime = meetingInfo.startTime;
                    conversationInfo.eventId = meetingInfo.eventId;
                    conversationInfo.isGNet = meetingInfo.isGNet;
                    //meetingInfo.isMeetingCancelled = meetingInfo.isOld;
                    this.updateConversation(conversationInfo.conversationId, conversationInfo.conversationType, conversationInfo);
                }
            }
        },
        _isMeetingInfoFromConversation: function (eventId, meetingId) {
            var noMeetingInfoData;
            var fromConversation = false;
            for (var k = 0, noMeetingInfoCount = this.noMeetingInfoArray.length; k < noMeetingInfoCount; k++) {
                noMeetingInfoData = this.noMeetingInfoArray[k];
                if (noMeetingInfoData.eventId == eventId || noMeetingInfoData.meetingId == meetingId) {
                    fromConversation = true;
                    break;
                }
            }
            return fromConversation;
        },
        isChatWindowOpen: function (conversationId, conversationType) {
            return this.moduleId == uc.constants.UI.ModuleId.Message && this.selectedConversationInfo
                && this.selectedConversationInfo.conversationId == conversationId
                && this.selectedConversationInfo.conversationType == conversationType;
        },
        /**
         *
         * @param messageData   chatId, chatType, taskId, msgId, contentType, content
         * @constructor
         */
        OnMessageSending: function (messageData) {
            var conversationType = this.chatTypeToConversationType(messageData.chatType);
            var conversationId = messageData.chatId;
            var conversationInfo = this.getConversation(conversationId, conversationType);
            if (!conversationInfo) {
                conversationInfo = this.createDefaultConversationInfo(messageData.chatId, conversationType);
                if (conversationType == uc.constants.Conversation.ConversationType.MeetingMsg) {
                    conversationInfo.meetingId = messageData.meetingId;
                    conversationInfo.eventId = messageData.eventId;
                    conversationInfo.meetingMsgType = uc.constants.Meeting.MsgType.chat;
                }

                this.setConversationTitleAndAvatar(conversationInfo);
            }
            if (conversationInfo) {
                var contentType = messageData.contentType;
                var msgId = messageData.msgId;
                var taskId = messageData.taskId;
                var sendingMessageData;
                switch (contentType) {
                    case uc.constants.Message.ContentType.Text:
                        //var textContent = uc.modules.emoji.EmojiConverter.unicodeToEmotionTag(messageData.content);
                        var textContent = uc.modules.emoji.EmojiConverter.unicodeToSmiley(messageData.content);
                        textContent = uc.util.EmotionUtil.codeToDescription(textContent);
                        if (conversationType != uc.constants.Conversation.ConversationType.SingleChat) {
                            textContent = uc.util.StringUtil.formatString(uc_resource.Message.TextMessageWithSender, [uc_resource.Message.You, textContent]);
                        }
                        conversationInfo.sendingMessageStack.unshift({
                            msgId: msgId,
                            content: textContent,
                            sendingStatus: uc.constants.Message.SendState.Sending
                        });
                        break;
                    case uc.constants.Message.ContentType.Image:
                        sendingMessageData = this._getSendMessageIndexAndData(conversationInfo.sendingMessageStack, 0, taskId);
                        if (conversationType == uc.constants.Conversation.ConversationType.SingleChat) {
                            textContent = uc_resource.Message.ImageMessage;
                        } else {
                            textContent = uc.util.StringUtil.formatString(uc_resource.Message.ImageMessageWithSender, [uc_resource.Message.You]);
                        }
                        if (sendingMessageData) {
                            sendingMessageData.sendingMessage.msgId = msgId;
                            sendingMessageData.sendingMessage.content = textContent;
                        } else {
                            conversationInfo.sendingMessageStack.unshift({
                                msgId: msgId,
                                content: textContent,
                                sendingStatus: uc.constants.Message.SendState.Sending
                            });
                        }
                        break;
                    case uc.constants.Message.ContentType.File:
                        sendingMessageData = this._getSendMessageIndexAndData(conversationInfo.sendingMessageStack, 0, taskId);
                        if (conversationType == uc.constants.Conversation.ConversationType.SingleChat) {
                            textContent = uc_resource.Message.FileMessage;
                        } else {
                            textContent = uc.util.StringUtil.formatString(uc_resource.Message.FileMessageWithSender, [uc_resource.Message.You]);
                        }
                        if (sendingMessageData) {
                            sendingMessageData.sendingMessage.msgId = msgId;
                            sendingMessageData.sendingMessage.content = textContent;
                        } else {
                            conversationInfo.sendingMessageStack.unshift({
                                msgId: msgId,
                                content: textContent,
                                sendingStatus: uc.constants.Message.SendState.Sending
                            });
                        }
                        break;
                    case uc.constants.Message.ContentType.Code:
                        if (conversationType == uc.constants.Conversation.ConversationType.SingleChat) {
                            textContent = uc_resource.Message.CodeMessage;
                        } else {
                            textContent = uc.util.StringUtil.formatString(uc_resource.Message.CodeMessageWithSender, [uc_resource.Message.You]);
                        }
                        conversationInfo.sendingMessageStack.unshift({
                            msgId: msgId,
                            content: textContent,
                            sendingStatus: uc.constants.Message.SendState.Sending
                        });
                        break;
                    case uc.constants.Message.ContentType.Video:
                        if (conversationType == uc.constants.Conversation.ConversationType.SingleChat) {
                            textContent = uc_resource.Message.MeetingVideoMessage;
                        } else {
                            textContent = uc.util.StringUtil.formatString(uc_resource.Message.MeetingVideoWithSender, [uc_resource.Message.You]);
                        }
                        conversationInfo.sendingMessageStack.unshift({
                            msgId: msgId,
                            content: textContent,
                            sendingStatus: uc.constants.Message.SendState.Sending
                        });
                        break;
                    case uc.constants.Message.ContentType.CloudFile:
                        if (conversationType == uc.constants.Conversation.ConversationType.SingleChat) {
                            textContent = uc_resource.Message.CloudFileMessage;
                        } else {
                            textContent = uc.util.StringUtil.formatString(uc_resource.Message.CloudFileMessageWithSender, [uc_resource.Message.You]);
                        }
                        conversationInfo.sendingMessageStack.unshift({
                            msgId: msgId,
                            content: textContent,
                            sendingStatus: uc.constants.Message.SendState.Sending
                        });
                        setTimeout(function () {
                            var ConversationManager = uc.modules.conversation.ConversationManager;
                            var sendingMessageData = ConversationManager._getSendMessageIndexAndData(conversationInfo.sendingMessageStack, msgId);
                            if (sendingMessageData) {
                                console.log(sendingMessageData);
                                conversationInfo.sendingMessageStack.splice(sendingMessageData.index, 1);
                                ConversationManager.updateConversation(conversationId, conversationType, conversationInfo);
                            }
                        }, 5000);
                        break;
                    case uc.constants.Message.ContentType.Comment:
                        var fileName = messageData.fileName;
                        textContent = uc.util.StringUtil.formatString(uc_resource.Message.CommentMessage, [uc_resource.Message.You, fileName]);
                        conversationInfo.sendingMessageStack.unshift({
                            msgId: msgId,
                            content: textContent,
                            sendingStatus: uc.constants.Message.SendState.Sending
                        });
                        var ConversationManager = uc.modules.conversation.ConversationManager;
                        var sendingMessageData = ConversationManager._getSendMessageIndexAndData(conversationInfo.sendingMessageStack, msgId);
                        if (sendingMessageData) {
                            conversationInfo.sendingMessageStack.splice(sendingMessageData.index, 1);
                            ConversationManager.updateConversation(conversationId, conversationType, conversationInfo);
                        }
                        break;
                    case uc.constants.Message.ContentType.MeetingReport:
                        sendingMessageData = this._getSendMessageIndexAndData(conversationInfo.sendingMessageStack, 0, taskId);
                        if (conversationType == uc.constants.Conversation.ConversationType.SingleChat) {
                            textContent = uc_resource.Message.MeetingReportMessage;
                        } else {
                            textContent = uc.util.StringUtil.formatString(uc_resource.Chat.MeetingReport, [uc_resource.Message.You]);
                        }
                        if (sendingMessageData) {
                            sendingMessageData.sendingMessage.msgId = msgId;
                            sendingMessageData.sendingMessage.content = textContent;
                        } else {
                            conversationInfo.sendingMessageStack.unshift({
                                msgId: msgId,
                                content: textContent,
                                sendingStatus: uc.constants.Message.SendState.Sending
                            });
                        }
                        break;
                }
                this.updateConversation(conversationId, conversationType, conversationInfo);
            } else {
                //Should not exist this case
            }
        },
        OnMessageFileUploading: function (chatId, chatType, taskId, contentType, messageData) {
            var conversationType = this.chatTypeToConversationType(chatType);
            var conversationInfo = this.getConversation(chatId, conversationType);
            if (!conversationInfo) {
                conversationInfo = this.createDefaultConversationInfo(chatId, chatType);
                if (conversationType == uc.constants.Conversation.ConversationType.MeetingMsg) {
                    conversationInfo.meetingId = messageData.meetingId;
                    conversationInfo.eventId = messageData.eventId;
                    conversationInfo.meetingMsgType = uc.constants.Meeting.MsgType.chat;
                }
                this.setConversationTitleAndAvatar(conversationInfo);
            }
            if (conversationInfo) {
                var content;
                switch (contentType) {
                    case uc.constants.Message.ContentType.Image:
                        if (conversationType == uc.constants.Conversation.ConversationType.SingleChat) {
                            content = uc_resource.Message.ImageMessage;
                        } else {
                            content = uc.util.StringUtil.formatString(uc_resource.Message.ImageMessageWithSender, [uc_resource.Message.You]);
                        }
                        break;
                    case uc.constants.Message.ContentType.File:
                        if (conversationType == uc.constants.Conversation.ConversationType.SingleChat) {
                            content = uc_resource.Message.FileMessage;
                        } else {
                            content = uc.util.StringUtil.formatString(uc_resource.Message.FileMessageWithSender, [uc_resource.Message.You]);
                        }
                        break;
                }
                conversationInfo.sendingMessageStack.unshift({
                    taskId: taskId,
                    msgId: 0,
                    content: content,
                    sendingStatus: uc.constants.Message.SendState.Sending
                });
                this.updateConversation(chatId, conversationType, conversationInfo);
            }
        },
        OnMessageFileUploadCancelled: function (chatId, chatType, taskId) {
            var conversationType = this.chatTypeToConversationType(chatType);
            var conversationInfo = this.getConversation(chatId, conversationType);
            if (conversationInfo) {
                var sendingMessageData = this._getSendMessageIndexAndData(conversationInfo.sendingMessageStack, 0, taskId);
                if (sendingMessageData) {
                    conversationInfo.sendingMessageStack.splice(sendingMessageData.index, 1);
                    this.updateConversation(chatId, conversationType, conversationInfo);
                }
            }
        },
        _getSendMessageIndexAndData: function (sendingMessageStack, msgId, taskId) {
            var sendingMessage, index = -1;
            for (var i = 0, len = sendingMessageStack.length; i < len; i++) {
                sendingMessage = sendingMessageStack[i];
                if (sendingMessage) {
                    if (msgId && sendingMessage.msgId == msgId) {
                        index = i;
                        break;
                    } else if (taskId && sendingMessage.taskId == taskId) {
                        index = i;
                        break;
                    }
                }
            }
            if (index == -1) {
                return null;
            }
            return {
                index: index,
                sendingMessage: sendingMessage
            };
        },
        chatTypeToConversationType: function (chatType) {
            var conversationType = uc.constants.Conversation.ConversationType.None;
            switch (chatType) {
                case uc.constants.Chat.ChatType.SingleChat:
                    conversationType = uc.constants.Conversation.ConversationType.SingleChat;
                    break;
                case uc.constants.Chat.ChatType.GroupChat:
                    conversationType = uc.constants.Conversation.ConversationType.GroupChat;
                    break;
                case uc.constants.Chat.ChatType.ProjectChat:
                    conversationType = uc.constants.Conversation.ConversationType.ProjectChat;
                    break;
                case uc.constants.Chat.ChatType.CloudChat:
                    conversationType = uc.constants.Conversation.ConversationType.CloudChat;
                    break;
                case uc.constants.Chat.ChatType.MeetingChat:
                    conversationType = uc.constants.Conversation.ConversationType.MeetingMsg;
                    break;
                case uc.constants.Chat.ChatType.OpenApiChat:
                    conversationType = uc.constants.Conversation.ConversationType.OpenApi;
                    break;
                case uc.constants.Chat.ChatType.Org:
                    conversationType = uc.constants.Conversation.ConversationType.OrgMsg;
                    break;
                case uc.constants.Chat.ChatType.Welcome:
                    conversationType = uc.constants.Conversation.ConversationType.WelcomeMsg;
                    break;
            }
            return conversationType;
        },
        conversationTypeToChatType: function (conversationType) {
            var chatType = uc.constants.Chat.ChatType.None;
            switch (conversationType) {
                case uc.constants.Conversation.ConversationType.SingleChat:
                    chatType = uc.constants.Chat.ChatType.SingleChat;
                    break;
                case uc.constants.Conversation.ConversationType.GroupChat:
                    chatType = uc.constants.Chat.ChatType.GroupChat;
                    break;
                case uc.constants.Conversation.ConversationType.ProjectChat:
                    chatType = uc.constants.Chat.ChatType.ProjectChat;
                    break;
                case uc.constants.Conversation.ConversationType.CloudChat:
                    chatType = uc.constants.Chat.ChatType.CloudChat;
                    break;
                case uc.constants.Conversation.ConversationType.MeetingMsg:
                    chatType = uc.constants.Chat.ChatType.MeetingChat;
                    break;
                case uc.constants.Conversation.ConversationType.OpenApi:
                    chatType = uc.constants.Chat.ChatType.OpenApiChat;
                    break;
                case uc.constants.Conversation.ConversationType.OrgMsg:
                    chatType = uc.constants.Chat.ChatType.Org;
                    break;
                case uc.constants.Conversation.ConversationType.WelcomeMsg:
                    chatType = uc.constants.Chat.ChatType.Welcome;
                    break;
            }
            return chatType;
        },
        groupTypeToChatType: function (groupType) {
            if (groupType == 3) {
                return uc.constants.Chat.ChatType.CloudChat;
            }
            return groupType;
        },
        OnMessageSent: function (errorCode, conversationId, conversationType, msgId, msgSeq, timestamp) {
            var conversationInfo = this.getConversation(conversationId, conversationType);
            if (conversationInfo) {
                var sendingMessageData = this._getSendMessageIndexAndData(conversationInfo.sendingMessageStack, msgId, 0);
                if (sendingMessageData) {
                    if (errorCode == uc.ErrorCode.Success) {
                        conversationInfo.sendingMessageStack.splice(sendingMessageData.index, 1);
                        conversationInfo.message = sendingMessageData.sendingMessage.content;
                        conversationInfo.msgId = msgId;
                        conversationInfo.msgSeq = msgSeq;
                        conversationInfo.fakeTime = 0;
                        conversationInfo.isFakeTime = false;
                        conversationInfo.timestamp = timestamp;
                        if (conversationInfo.topIndex <= 0) {
                            this.setConversationTimeLine(timestamp);
                        }
                        //this.clearConversationDraft(conversationId, conversationType);
                    } else {
                        sendingMessageData.sendingMessage.sendingStatus = uc.constants.Message.SendState.SendFail;
                    }
                    this.updateConversation(conversationId, conversationType, conversationInfo);
                }
            }
        },
        OnDocumentUploaded: function (errorCode, groupId, operatorId, document, msgId) {
            var conversationInfo = this.getConversation(groupId, uc.constants.Conversation.ConversationType.ProjectChat);
            if (!conversationInfo) {
                conversationInfo = this.getConversation(groupId, uc.constants.Conversation.ConversationType.GroupChat);
            }

            if (!conversationInfo) {
                conversationInfo = this.getConversation(groupId, uc.constants.Conversation.ConversationType.CloudChat);
            }

            if (!conversationInfo) {
                conversationInfo = this.getConversation(groupId, uc.constants.Conversation.ConversationType.MeetingMsg);
            }
            if (conversationInfo) {
                var sendingMessageData = this._getSendMessageIndexAndData(conversationInfo.sendingMessageStack, msgId, 0);
                if (sendingMessageData) {
                    if (errorCode == uc.ErrorCode.Success) {
                        conversationInfo.sendingMessageStack.splice(sendingMessageData.index, 1);
                        conversationInfo.message = uc.util.StringUtil.formatString(uc_resource.Message.UploadDocument, [uc_resource.Message.You, document.fileName]);
                        conversationInfo.fakeTime = 0;
                        conversationInfo.isFakeTime = false;
                        conversationInfo.timestamp = document ? document.createTime : conversationInfo.timestamp;
                        if (conversationInfo.topIndex <= 0) {
                            this.setConversationTimeLine(conversationInfo.timestamp);
                        }
                    } else {
                        sendingMessageData.sendingMessage.sendingStatus = uc.constants.Message.SendState.SendFail;
                    }
                    this.updateConversation(conversationInfo.conversationId, conversationInfo.conversationType, conversationInfo);
                }
            }
        },
        _getAllNewMsgCount: function () {
            var allConversationInfos = this.getAllConversations();
            var conversationInfo, newMessageCount = 0, conversationType, conversationId;
            for (var i = 0, len = allConversationInfos.length; i < len; i++) {
                conversationInfo = allConversationInfos[i];
                conversationType = conversationInfo.conversationType;
                conversationId = conversationInfo.conversationId;
                if (this._needCalculateMessageCount(conversationType, conversationId)) {
                    newMessageCount += (conversationInfo.newMsgCount ? conversationInfo.newMsgCount : 0);
                }
            }
            this.setNewMessageCount(newMessageCount);
        },
        _needCalculateMessageCount: function (conversationType, conversationId) {
            return ((conversationType == uc.constants.Conversation.ConversationType.GroupChat
            || conversationType == uc.constants.Conversation.ConversationType.ProjectChat
            || conversationType == uc.constants.Conversation.ConversationType.CloudChat
            || conversationType == uc.constants.Conversation.ConversationType.SingleChat
            || conversationType == uc.constants.Conversation.ConversationType.OpenApi
            || conversationType == uc.constants.Conversation.ConversationType.OrgMsg) &&
            !uc.modules.notice.NoticeManager.isNoticeConversation(conversationId));
        },
        OnMessageRead: function (conversationType, id, readType) {
            //currently, meeting chat message do not count
            if (conversationType == uc.constants.Conversation.ConversationType.MeetingMsg) {
                return;
            }
            var conversationInfo = this.getConversation(id, conversationType);

            if (conversationInfo) {
                if (conversationInfo.selected) { //TODO need check if this message read is from current login user, check userId, resId, will add it when SDK provide the channel
                    return;
                }

                if (conversationInfo.newMsgCount && conversationInfo.newMsgCount > 0) {
                    if (readType == uc.constants.Message.AckReadType.Conversation) {
                        this.decreaseNewMsgCount(conversationInfo.conversationType, conversationInfo.newMsgCount);
                        conversationInfo.newMsgCount = 0;
                    } else {
                        this.decreaseNewMsgCount(conversationInfo.conversationType, 1);
                        conversationInfo.newMsgCount--;
                    }
                } else {
                    conversationInfo.newMsgCount = 0;
                }
                this.notifyDataSetChanged();
            }
        },
        OnAppListReceived: function () {
            var appId, appInfo, conversationInfo;
            for (var i = 0, len = this.noOpenApiInfoArray.length; i < len; i++) {
                appId = this.noOpenApiInfoArray[i];
                appInfo = uc.modules.openApi.OpenApiManager.getOpenApiInfo(appId);
                if (appInfo) {
                    conversationInfo = this.getConversation(appId, uc.constants.Conversation.ConversationType.OpenApi);
                    if (conversationInfo) {
                        conversationInfo.title = appInfo.appTitle ? appInfo.appTitle : appId;
                        conversationInfo.avatarUrlArray = appInfo.appLogo ? [appInfo.appLogo] : [uc.constants.UI.DefaultLocalUrl.OpenApi];
                        conversationInfo.isCancelled = 0;
                        this.updateConversation(appId, uc.constants.Conversation.ConversationType.OpenApi, conversationInfo);
                        if (conversationInfo.selected) {
                            uc.modules.chat.OpenApiChat.updateAppInfo(appInfo);
                        }
                    }
                } else {
                    var appIds = [appId];
                    uc.modules.openApi.OpenApiManager.getAppsInfo(appIds);
                }
            }
        },
        OnGetAppInfoReceived: function (apps) {
            if (!apps.length) {
                return;
            }

            for (var i = 0; i < apps.length; i++) {
                var app = apps[i];
                uc.modules.openApi.OpenApiManager.cacheOpenApiInfo(app);
                var conversationInfo = this.getConversation(app.appId, uc.constants.Conversation.ConversationType.OpenApi);

                if(conversationInfo){
                      if (!app.status) {
                          conversationInfo.title = uc_resource.OpenApi.OpenApiClosed + app.appTitle;
                          conversationInfo.avatarUrlArray = app.appLogo ? [app.appLogo] : [uc.constants.UI.DefaultLocalUrl.OpenApi];
                          conversationInfo.isCancelled = app.status ? 0 : 1;
                          this.updateConversation(app.appId, uc.constants.Conversation.ConversationType.OpenApi, conversationInfo);
                      } else {
                          conversationInfo.title = app.appTitle ? app.appTitle : app.appId;
                          conversationInfo.avatarUrlArray = app.appLogo ? [app.appLogo] : [uc.constants.UI.DefaultLocalUrl.OpenApi];
                          conversationInfo.isCancelled = 0;
                          this.updateConversation(app.appId, uc.constants.Conversation.ConversationType.OpenApi, conversationInfo);
                      }

                      if (conversationInfo.selected) {
                          uc.modules.chat.OpenApiChat.updateAppInfo(app);
                      }
                }

                if (app.status == 0) {
                    this.closeAppList.addOpenApiInfo(app.appId, app);
                } else {
                    this.closeAppList.removeOpenApiInfo(app.appId);
                }
            }
        },
        OnAppStatusChangeReceived: function (channel, appId, status, extraData) {
            var conversationInfo = this.getConversation(appId, uc.constants.Conversation.ConversationType.OpenApi);
            if (conversationInfo) {
                var appIds = [appId];
                uc.modules.openApi.OpenApiManager.getAppsInfo(appIds);
            }
        },
        OnDraftMessageChanged: function (chatId, chatType, moduleId) {
            if (moduleId == uc.constants.UI.ModuleId.Message) {
                return;
            }
            this.notifyDataSetChanged();
        },
        OnWindowActiveChanged: function () {
            uc.util.LogUtil.info('ConversationEventListener', 'OnWindowActiveStatus', 'conversation:', this.selectedConversationInfo);
            if(this.moduleId != uc.constants.UI.ModuleId.Message){
                return;
            }
            this.setConversationMessagesRead(this.selectedConversationInfo);
            this.notifyDataSetChanged();

            uc.IUIService.triggerCrossWindowEvent(uc.constants.UI.CrossWindowEventName.OnMainWindowOnTop, {});
        },
        OnUserStatusReceived: function (userIds, status) {

        },
        /**
         * event for call started
         * @param chatId
         * @param chatType
         */
        OnCallStarted: function (chatId, chatType) {
            var conversationType = this.chatTypeToConversationType(chatType);
            var conversationInfo = this.getConversation(chatId, conversationType);
            if (!conversationInfo) {
                conversationInfo = this.createDefaultConversationInfo(chatId, conversationType);
            }
            if (!conversationInfo.isInCall) {
                conversationInfo.isInCall = true;
                this.setConversationTitleAndAvatar(conversationInfo);
                this.updateConversation(chatId, conversationType, conversationInfo);
            }
        },
        /**
         * event for call ended
         * @param chatId
         * @param chatType
         */
        OnCallEnded: function (chatId, chatType) {
            var conversationType = this.chatTypeToConversationType(chatType);
            var conversationInfo = this.getConversation(chatId, conversationType);
            if (conversationInfo) {
                conversationInfo.isInCall = false;
                this.updateConversation(chatId, conversationType, conversationInfo);
            }
        },
        OnConversationSetTopReceived: function (errorCode, message) {
            if (errorCode != uc.ErrorCode.Success || !message) {
                this.setTopIndexError(errorCode);
                return;
            }

            var conversation = this.getConversation(message.relateId, message.conversationType);
            var latestConversation = this.getLatestTopIndexConversation(), fakeTime = 0;

            if (conversation) {
                if (message.index > 0 && latestConversation) {
                    fakeTime = latestConversation.timestamp > latestConversation.fakeTime ? latestConversation.timestamp + 1 : latestConversation.fakeTime + 1;
                }

                conversation.fakeTime = fakeTime;
                conversation.topIndex = message.index;
                this.updateConversation(conversation.conversationId, conversation.conversationType, conversation)
            }
        },
        OnNoticeConversationStatusChanged:function(conversationId){
          var conversationInfo = this.getConversation(conversationId, uc.constants.Conversation.ConversationType.OpenApi);
          if(conversationInfo){
              this.notifyDataSetChanged();
          }
        }
    }
})(uc);