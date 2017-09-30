/**
 * Created by waouyang on 15/9/3.
 */
(function (uc) {
    uc.util.MenuUtil = {
        showConversationContactContextMenu: function (e, userId, conversationInfo) {
            var isTopIndex = conversationInfo.topIndex > 0 ? true : false;
            var menuItems = [{
                label: uc_resource.Menu.ViewProfile,
                type: uc.constants.UI.MenuItemType.MenuItem,
                disabled: false,
                callback: function () {
                    uc.IUIService.showPopupContactProfile(userId);
                }
            }, {
                label: uc_resource.Menu.ViewCalendar,
                type: uc.constants.UI.MenuItemType.MenuItem,
                disabled: false,
                callback: function () {
                    var users = [];
                    users.push(userId);
                    var param = {
                        shareUserId: uc.constants.Meeting.AuthorizeMySelf,
                        isCreate: true,
                        users: users
                    };

                    uc.IUIService.createMeeting("ConversationContextMenuCreateMeeting", param);
                }
            }, {
                label: uc_resource.Menu.SetConversationOnTop,
                type: uc.constants.UI.MenuItemType.MenuItem,
                disabled: isTopIndex,
                callback: function () {
                    uc.modules.conversation.ConversationManager.setConversationOnTop(conversationInfo.conversationType, userId, 1);
                }
            },{
                label: uc_resource.Menu.CancelConversationOnTop,
                type: uc.constants.UI.MenuItemType.MenuItem,
                disabled: !isTopIndex,
                callback: function () {
                    uc.modules.conversation.ConversationManager.setConversationOnTop(conversationInfo.conversationType, userId, 0);
                }
            }, {
                type: uc.constants.UI.MenuItemType.Split
            }, {
                label: uc_resource.Menu.DeleteConversation,
                type: uc.constants.UI.MenuItemType.MenuItem,
                disabled: false,
                callback: function () {
                    uc.modules.conversation.ConversationManager.removeConversation(userId, uc.constants.Conversation.ConversationType.SingleChat);
                }
            }];

            this.setCapability(menuItems);

            var menuOptions = {
                hasIcon: false,
                menuItems: menuItems
            };
            uc.modules.component.Menu.showMenu(e, menuOptions);
            e.stopPropagation();
            e.preventDefault();
        },
        showConversationProjectContextMenu: function (e, groupId, conversationInfo) {
            var isTopIndex = conversationInfo.topIndex > 0;
            var menuItems = [];
            if (this._isInGroup(groupId)) {
                menuItems = [{
                    label: uc_resource.Menu.ProjectSetting,
                    type: uc.constants.UI.MenuItemType.MenuItem,
                    disabled: false,
                    callback: function () {
                        uc.IUIService.showPopupGroupProfile(groupId);
                    }
                }, {
                    label: uc_resource.Menu.ViewCalendar,
                    type: uc.constants.UI.MenuItemType.MenuItem,
                    disabled: false,
                    callback: function () {
                        var groupInfo = uc.IGroupService.getGroupInfo(groupId);
                        var users = [];
                        for (var i = 0; i < groupInfo.members.length; i++) {
                            users.push(groupInfo.members[i].userId);
                        }

                        var param = {
                            shareUserId: uc.constants.Meeting.AuthorizeMySelf,
                            isCreate: true,
                            users: users
                        };

                        uc.IUIService.createMeeting("ConversationProjectContextMenuCreateMeeting", param);
                    }
                }, {
                    label: uc_resource.Menu.SetConversationOnTop,
                    type: uc.constants.UI.MenuItemType.MenuItem,
                    disabled: isTopIndex,
                    callback: function () {
                        uc.modules.conversation.ConversationManager.setConversationOnTop(conversationInfo.conversationType, groupId, 1);
                    }
                },{
                    label: uc_resource.Menu.CancelConversationOnTop,
                    type: uc.constants.UI.MenuItemType.MenuItem,
                    disabled: !isTopIndex,
                    callback: function () {
                        uc.modules.conversation.ConversationManager.setConversationOnTop(conversationInfo.conversationType, groupId, 0);
                    }
                }, {
                    type: uc.constants.UI.MenuItemType.Split
                }];
            }
            menuItems.push({
                label: uc_resource.Menu.DeleteConversation,
                type: uc.constants.UI.MenuItemType.MenuItem,
                disabled: false,
                callback: function () {
                    uc.modules.conversation.ConversationManager.removeConversation(groupId, uc.constants.Conversation.ConversationType.ProjectChat);
                }
            });

            this.setCapability(menuItems);

            var menuOptions = {
                hasIcon: false,
                menuItems: menuItems
            };
            uc.modules.component.Menu.showMenu(e, menuOptions);
            e.stopPropagation();
            e.preventDefault();
        },
        showConversationGroupContextMenu: function (e, groupId, conversationInfo) {
            var isTopIndex = conversationInfo.topIndex > 0;
            var menuItems = [];
            if (this._isInGroup(groupId)) {
                menuItems = [{
                    label: uc_resource.Menu.GroupSetting,
                    type: uc.constants.UI.MenuItemType.MenuItem,
                    disabled: false,
                    callback: function () {
                        uc.IUIService.showPopupGroupProfile(groupId);
                    }
                }, {
                    label: uc_resource.Menu.ViewCalendar,
                    type: uc.constants.UI.MenuItemType.MenuItem,
                    disabled: false,
                    callback: function () {
                        var groupInfo = uc.IGroupService.getGroupInfo(groupId);
                        var users = [];
                        for (var i = 0; i < groupInfo.members.length; i++) {
                            users.push(groupInfo.members[i].userId);
                        }
                        var param = {
                            shareUserId: uc.constants.Meeting.AuthorizeMySelf,
                            isCreate: true,
                            users: users
                        };

                        uc.IUIService.createMeeting("ConversationGroupContextMenuCreateMeeting", param);
                    }
                }, {
                    label: uc_resource.Menu.SetConversationOnTop,
                    type: uc.constants.UI.MenuItemType.MenuItem,
                    disabled: isTopIndex,
                    callback: function () {
                        uc.modules.conversation.ConversationManager.setConversationOnTop(conversationInfo.conversationType, groupId, 1);
                    }
                },{
                    label: uc_resource.Menu.CancelConversationOnTop,
                    type: uc.constants.UI.MenuItemType.MenuItem,
                    disabled: !isTopIndex,
                    callback: function () {
                        uc.modules.conversation.ConversationManager.setConversationOnTop(conversationInfo.conversationType, groupId, 0);
                    }
                }, {
                    type: uc.constants.UI.MenuItemType.Split
                }];
            }
            menuItems.push({
                label: uc_resource.Menu.DeleteConversation,
                type: uc.constants.UI.MenuItemType.MenuItem,
                disabled: false,
                callback: function () {
                    uc.modules.conversation.ConversationManager.removeConversation(groupId, uc.constants.Conversation.ConversationType.GroupChat);
                }
            });

            this.setCapability(menuItems);

            var menuOptions = {
                hasIcon: false,
                menuItems: menuItems
            };
            uc.modules.component.Menu.showMenu(e, menuOptions);
            e.stopPropagation();
            e.preventDefault();
        },
        showConversationCloudContextMenu: function (e, groupId, conversationInfo) {
            var isTopIndex = conversationInfo.topIndex > 0;
            var menuItems = [];
            if (this._isInGroup(groupId)) {
                menuItems = [{
                    label: uc_resource.Menu.OpenCloud,
                    type: uc.constants.UI.MenuItemType.MenuItem,
                    disabled: false,
                    callback: function () {
                        var groupInfo = uc.IGroupService.getGroupInfo(groupId);
                        if (groupInfo) {
                            var cloudData = JSON.stringify({"mount_id": groupInfo.mountId});
                            uc.util.LogUtil.info('MenuUtil', 'getGkMountId', 'cloudData = ' + cloudData, null);
                            uc.IUIService.changeModule(uc.constants.UI.ModuleId.Cloud, cloudData);
                        }
                    }
                }, {
                    label: uc_resource.Menu.ViewCalendar,
                    type: uc.constants.UI.MenuItemType.MenuItem,
                    disabled: false,
                    callback: function () {
                        var groupInfo = uc.IGroupService.getGroupInfo(groupId);
                        var users = [];
                        for (var i = 0; i < groupInfo.members.length; i++) {
                            users.push(groupInfo.members[i].userId);
                        }

                        var param = {
                            shareUserId: uc.constants.Meeting.AuthorizeMySelf,
                            isCreate: true,
                            users: users
                        };

                        uc.IUIService.createMeeting("ConversationCloudContextMenuCreateMeeting", param);
                    }
                }, {
                    label: uc_resource.Menu.SetConversationOnTop,
                    type: uc.constants.UI.MenuItemType.MenuItem,
                    disabled: isTopIndex,
                    callback: function () {
                        uc.modules.conversation.ConversationManager.setConversationOnTop(conversationInfo.conversationType, groupId, 1);
                    }
                },{
                    label: uc_resource.Menu.CancelConversationOnTop,
                    type: uc.constants.UI.MenuItemType.MenuItem,
                    disabled: !isTopIndex,
                    callback: function () {
                        uc.modules.conversation.ConversationManager.setConversationOnTop(conversationInfo.conversationType, groupId, 0);
                    }
                }, {
                    type: uc.constants.UI.MenuItemType.Split
                }];
            }
            menuItems.push({
                label: uc_resource.Menu.DeleteConversation,
                type: uc.constants.UI.MenuItemType.MenuItem,
                disabled: false,
                callback: function () {
                    uc.modules.conversation.ConversationManager.removeConversation(groupId, uc.constants.Conversation.ConversationType.CloudChat);
                }
            });

            this.setCapability(menuItems);

            var menuOptions = {
                hasIcon: false,
                menuItems: menuItems
            };
            uc.modules.component.Menu.showMenu(e, menuOptions);
            e.stopPropagation();
            e.preventDefault();
        },
        showPbxContextMenu:function(e,info){
            var menuItems = [];
            if(info.userID && info.userID != 0){
                if(this.getCallVisible()){
                    menuItems = [{
                        label: uc_resource.Pbx.callBee,
                        type: uc.constants.UI.MenuItemType.MenuItem,
                        disabled: false,
                        callback: function(){
                            uc.IUIEvent.OnPbxCallBee(info.userID);
                        }
                    }];
                }
                if(this.getPBXVisible()){
                    if(info.phoneNumber && info.phoneNumber.indexOf("/") >-1){
                        var sp=info.phoneNumber.split("/");
                        for(var i= 0,len = sp.length;i < len;i++){
                            menuItems.push({
                                label: sp[i],
                                type: uc.constants.UI.MenuItemType.MenuItem,
                                disabled: false,
                                callback: function(){
                                    uc.IUIEvent.OnPbxCallFromContact({"userName":info.userName,"userID":info.userID,"Tel":sp[i]});
                                }
                            });
                        }
                    }else if(info.phoneNumber && info.phoneNumber != ""){
                        menuItems.push({
                            label: info.phoneNumber,
                            type: uc.constants.UI.MenuItemType.MenuItem,
                            disabled: false,
                            callback: function(){
                                uc.IUIEvent.OnPbxCallFromContact({"userName":info.userName,"userID":info.userID,"Tel":info.phoneNumber});
                            }
                        });
                    }
                }
            }else{
                if((this.getPBXVisible() && info.phoneNumber)){
                    menuItems.push({
                        label: info.phoneNumber,
                        type: uc.constants.UI.MenuItemType.MenuItem,
                        disabled: false,
                        callback: function(){
                            uc.IUIEvent.OnPbxCallFromContact({"userName":info.userName,"userID":info.userID,"Tel":info.phoneNumber});
                        }
                    });
                }
            }
            var pbxNum=info.phoneNumber;
            if(info.userID){
                pbxNum=info.userID;
            }
                menuItems.push({
                    label: uc_resource.Menu.DeletePbx,
                    type: uc.constants.UI.MenuItemType.MenuItem,
                    disabled: false,
                    callback: function(){
                        uc.modules.pbx.PbxManager.removePbx(pbxNum);
                    }
                });
            this.setCapability(menuItems);

            var menuOptions = {
                hasIcon: false,
                menuItems: menuItems
            };
            uc.modules.component.Menu.showMenu(e, menuOptions);
            e.stopPropagation();
            e.preventDefault();
        },
        showConversationMeetingContextMenu: function (e, meetingData, conversationInfo) {
            var isTopIndex = conversationInfo.topIndex > 0;
            var _this = this;
            var menuItems = [];
            var _meetingDetail = uc.IMeetingService.getMeetingInfoByIdSync(meetingData.eventId, 0, 0, meetingData.authorizerId)[0];
            if (!meetingData.isCancelled) {
                if (meetingData.isGNet) {
                    menuItems.push({
                        label: uc_resource.Menu.JoinMeeting,
                        type: uc.constants.UI.MenuItemType.MenuItem,
                        disabled: false,
                        callback: function () {
                            uc.IUIService.joinMeeting(_meetingDetail, false, true);
                        }
                    });
                }
                menuItems.push({
                    label: uc_resource.Menu.ForwardMeeting,
                    type: uc.constants.UI.MenuItemType.MenuItem,
                    disabled: false,
                    callback: function () {
                        _this._forwardMeeting(_meetingDetail);
                    }
                }, {
                    label: uc_resource.Menu.SetConversationOnTop,
                    type: uc.constants.UI.MenuItemType.MenuItem,
                    disabled: isTopIndex,
                    callback: function () {
                        uc.modules.conversation.ConversationManager.setConversationOnTop(uc.constants.Conversation.ConversationType.MeetingMsg, meetingData.groupId, 1);
                    }
                },{
                    label: uc_resource.Menu.CancelConversationOnTop,
                    type: uc.constants.UI.MenuItemType.MenuItem,
                    disabled: !isTopIndex,
                    callback: function () {
                        uc.modules.conversation.ConversationManager.setConversationOnTop(uc.constants.Conversation.ConversationType.MeetingMsg, meetingData.groupId, 0);
                    }
                }, {
                    type: uc.constants.UI.MenuItemType.Split
                });
            }
            menuItems.push({
                label: uc_resource.Menu.DeleteConversation,
                type: uc.constants.UI.MenuItemType.MenuItem,
                disabled: false,
                callback: function () {
                    uc.modules.conversation.ConversationManager.removeConversation(meetingData.groupId, uc.constants.Conversation.ConversationType.MeetingMsg);
                }
            });

            this.setCapability(menuItems);

            var menuOptions = {
                hasIcon: false,
                menuItems: menuItems
            };
            uc.modules.component.Menu.showMenu(e, menuOptions);
            e.stopPropagation();
            e.preventDefault();
        },
        showDeleteConversationContextMenu: function (e, conversationId, conversationType, conversationInfo) {
            var isTopIndex = conversationInfo.topIndex > 0;
            var menuItems = [{
                label: uc_resource.Menu.SetConversationOnTop,
                type: uc.constants.UI.MenuItemType.MenuItem,
                disabled: isTopIndex,
                callback: function () {
                    uc.modules.conversation.ConversationManager.setConversationOnTop(conversationType, conversationId, 1);
                }
            },{
                label: uc_resource.Menu.CancelConversationOnTop,
                type: uc.constants.UI.MenuItemType.MenuItem,
                disabled: !isTopIndex,
                callback: function () {
                    uc.modules.conversation.ConversationManager.setConversationOnTop(conversationType, conversationId, 0);
                }
            },{
                label: uc_resource.Menu.DeleteConversation,
                type: uc.constants.UI.MenuItemType.MenuItem,
                disabled: uc.modules.notice.NoticeManager.isNoticeConversation(conversationId),
                callback: function () {
                    uc.modules.conversation.ConversationManager.removeConversation(conversationId, conversationType);
                }
            }];

            this.setCapability(menuItems);

            var menuOptions = {
                hasIcon: false,
                menuItems: menuItems
            };
            uc.modules.component.Menu.showMenu(e, menuOptions);
            e.stopPropagation();
            e.preventDefault();
        },
        _isInGroup: function (groupId) {
            var groupInfo = uc.IGroupService.getGroupInfo(groupId);
            if (groupInfo) {
                return !(groupInfo.joinState == uc.constants.Group.JoinStatus.Exited || groupInfo.status == uc.constants.Group.GroupStatus.Ended)
            }
            return false;
        },
        _forwardMeeting: function (meetingData) {
            var selectedUserList = [];
            selectedUserList.push(meetingData.hostId);
            if ($.isArray(meetingData.attendees) && meetingData.attendees.length > 0) {
                $.each(meetingData.attendees, function (key, attendees) {
                    if (attendees.type == uc.constants.Meeting.AttendeeType.User) {
                        selectedUserList.push(parseInt(attendees.account));
                    } else {
                        selectedUserList.push(attendees.account);
                    }
                });
            }

            uc.util.StorageUtil.setData('ForwardUserIds:', JSON.stringify({data: selectedUserList}));
            uc.IUIService.openChooseContactWindow({
                key: "ConversationMeeting",
                isMeetingForward: true
            }, function (args) {
                if (args.okOrCancel) {
                    var userIds = JSON.parse(uc.util.StorageUtil.getData('ForwardUserIds:')).data;
                    var persons = [], isRepeat = false;
                    $.each(args.contacts, function (key, attendees) {
                        var type = attendees.userId ? uc.constants.Meeting.AttendeeType.User : uc.constants.Meeting.AttendeeType.Mail;
                        if (userIds.indexOf(attendees.userId) >= 0 || attendees.userId == meetingData.hostId) {
                            isRepeat = true;
                            return true;
                        }
                        persons.push({
                            account: (attendees.userId || attendees.account) + "",                                        // 账号
                            type: type,                           // 参会人类型：1 内部用户 ?2 邮件用户 ?3 电话用户
                            status: 0,                                                              // 参会人状态：0 接受 ?1 拒绝 ?2 未处 ?
                            //startTime: 12432143142123,                                              // 入会时间
                            name: attendees.displayName                                            // 参会人名�?
                        });
                    });

                    if (persons.length) {
                        uc.util.LogUtil.info("MenuUtil", "_forwardMeeting", "forwardMeeting",
                            {
                                eventId: meetingData.eventId, persons: persons,
                                shareUserId: meetingData.authorizerId
                            });
                        uc.IMeetingService.forwardMeeting(meetingData.eventId, persons, 0, meetingData.authorizerId, 1, 1);
                    } else {
                        var msg = uc_resource.Meeting.AddForwardUser;
                        if (isRepeat) {
                            msg = uc_resource.Meeting.NotForwardRepeat;
                        }
                        uc.modules.component.PromptMessageBox.open({
                            title: uc_resource.WindowTitle.Notification,
                            message: msg,
                            buttonMode: "ok",
                            OkText: uc_resource.Common.OK
                        }, function (result) {
                            this.close();
                        });
                    }
                }
            });
        },
        setCapability: function (menuItems) {

            // add by xiege
            if (uc.constants.Switch.OpenMultiVersion) {
                var version = JSON.parse(uc.util.StorageUtil.getData("capability"));
                if (version) {
                    //switch (version.im) {
                    //}
                    switch (version.calendar) {
                        case "0":
                            for (menuItem in menuItems) {
                                if (menuItems[menuItem].label == uc_resource.Menu.ViewCalendar) {
                                    menuItems.splice(menuItem, 1);
                                    break;
                                }
                            }
                            break;
                    }
                    /*switch (version.conf) {
                     }
                     switch (version.call) {
                     }
                     switch (version.cloud_disk_type) {
                     }
                     switch (version.app_center) {
                     }
                     switch (version.app_moments) {
                     }
                     switch (version.scan) {
                     }
                     switch (version.remote_assistant) {
                     }*/
                }
            }
        },
        showContactContactContextMenu: function (e, userId) {
            var menuItems = [{
                label: uc_resource.Menu.ViewProfile,
                type: uc.constants.UI.MenuItemType.MenuItem,
                disabled: false,
                callback: function () {
                    uc.IUIService.showPopupContactProfile(userId);
                }
            }, {
                label: uc_resource.Menu.ViewCalendar,
                type: uc.constants.UI.MenuItemType.MenuItem,
                disabled: false,
                callback: function () {
                    var users = [];
                    users.push(userId);

                    var param = {
                        shareUserId: uc.constants.Meeting.AuthorizeMySelf,
                        isCreate: true,
                        users: users
                    };

                    uc.IUIService.createMeeting("ContactContextMenuCreateMeeting", param);
                }
            }];

            var currentUserInfo = uc.IContactService.getCurrentUserInfo();
            if (currentUserInfo.userId == userId) {
//            	menuItems.push({
// 	        		label: uc_resource.Menu.ViewMicroblog,
// 	                type: uc.constants.UI.MenuItemType.MenuItem,
// 	                disabled: false,
// 	                callback: function(){
// 	                	var _url = uc.modules.microblog.Microblog.getPageUrl();
// 	                	window.open(_url, null, '_blank');
// 	                }
// 	            });
            } else {
//            	menuItems.push({
// 	        		label: uc_resource.Menu.ViewMicroblog,
// 	                type: uc.constants.UI.MenuItemType.MenuItem,
// 	                disabled: false,
// 	                callback: function(){
// 	                	uc.IMicroblogService.getUserhomePageURL(currentUserInfo.userId, userId, function(args){
// 	                		var _url = args[1];
// 	 	                	window.open(_url, null, '_blank');
// 	                	});
// 	                }
// 	            });

                var attentionStatus = uc.IMicroblogService.getRelationSync(userId);
                if (attentionStatus == 1) {
                    menuItems.push({
                        label: uc_resource.Menu.CancelAttention,
                        type: uc.constants.UI.MenuItemType.MenuItem,
                        disabled: false,
                        callback: function () {
                            uc.IMicroblogService.cancelAttention(userId);
                        }
                    });
                } else if (attentionStatus == 3) {
                    menuItems.push({
                        label: uc_resource.Menu.CommonAttention,
                        type: uc.constants.UI.MenuItemType.MenuItem,
                        disabled: false,
                        callback: function () {
                            uc.IMicroblogService.cancelAttention(userId);
                        }
                    });
                } else {
                    menuItems.push({
                        label: uc_resource.Menu.PayAttention,
                        type: uc.constants.UI.MenuItemType.MenuItem,
                        disabled: false,
                        callback: function () {
                            uc.IMicroblogService.addAttention(userId);
                        }
                    });
                }
            }

            this.setCapability(menuItems);

            var menuOptions = {
                hasIcon: false,
                menuItems: menuItems
            };
            uc.modules.component.Menu.showMenu(e, menuOptions);
            e.stopPropagation();
            e.preventDefault();
        },
	    showContactCreateGroupCallContextMenu: function (e, _callback) {
		    var menuItems = [{
			    label: uc_resource.Menu.createGroupChat,
			    type: uc.constants.UI.MenuItemType.MenuItem,
			    disabled: false,
			    callback: _callback
		    }];
		    this.setCapability(menuItems);
		    var menuOptions = {
			    hasIcon: false,
			    menuItems: menuItems
		    };
		    if(uc.modules.component.Menu){
			    uc.modules.component.Menu.showMenu(e, menuOptions);
		    }
		    e.stopPropagation();
		    e.preventDefault();
	    },
        showContactProjectContextMenu: function (e, groupId) {
            var menuItems = [];
            if (this._isInGroup(groupId)) {
                var groupInfo = uc.IGroupService.getGroupInfo(groupId);

                menuItems = [{
                    label: uc_resource.Menu.ProjectSetting,
                    type: uc.constants.UI.MenuItemType.MenuItem,
                    disabled: false,
                    callback: function () {
                        uc.IUIService.showPopupGroupProfile(groupId);
                    }
                }, {
                    label: uc_resource.Menu.ViewCalendar,
                    type: uc.constants.UI.MenuItemType.MenuItem,
                    disabled: false,
                    callback: function () {
                        var users = [];
                        for (var i = 0; i < groupInfo.members.length; i++) {
                            users.push(groupInfo.members[i].userId);
                        }

                        var param = {
                            shareUserId: uc.constants.Meeting.AuthorizeMySelf,
                            isCreate: true,
                            users: users
                        };

                        uc.IUIService.createMeeting("ContactProjectContextMenuCreateMeeting", param);
                    }
                }];

                var currentUserInfo = uc.IContactService.getCurrentUserInfo();
                if (currentUserInfo.userId == groupInfo.adminId) {
                    menuItems.push({
                        label: uc_resource.Menu.EndProject,
                        type: uc.constants.UI.MenuItemType.MenuItem,
                        disabled: false,
                        callback: function () {
                            var message = uc_resource.Profile.ConfirmEnd;
                            uc.modules.component.PromptMessageBox.open({
                                title: uc_resource.Common.Prompt,
                                message: message,
                                buttonMode: "both",
                                OkText: uc_resource.Common.OK,
                                CancelText: uc_resource.Common.Cancel
                            }, function (result) {
                                if (result == 0) {
                                    uc.IGroupService.endGroup(groupId, function (args) {
                                    });
                                }
                                this.close();
                            });
                        }
                    });
                } else {
                    menuItems.push({
                        label: uc_resource.Menu.QuitProject,
                        type: uc.constants.UI.MenuItemType.MenuItem,
                        disabled: false,
                        callback: function () {
                            var message = uc_resource.Profile.ConfirmExit;
                            uc.modules.component.PromptMessageBox.open({
                                title: uc_resource.Common.Prompt,
                                message: message,
                                buttonMode: "both",
                                OkText: uc_resource.Common.OK,
                                CancelText: uc_resource.Common.Cancel
                            }, function (result) {
                                if (result == 0) {
                                    uc.IGroupService.exitGroup(groupId, function (args) {
                                    });
                                }
                                this.close();
                            });
                        }
                    });
                }
            }

            this.setCapability(menuItems);

            var menuOptions = {
                hasIcon: false,
                menuItems: menuItems
            };
            uc.modules.component.Menu.showMenu(e, menuOptions);
            e.stopPropagation();
            e.preventDefault();
        },
        showContactGroupContextMenu: function (e, groupId) {
            var menuItems = [];
            if (this._isInGroup(groupId)) {
                menuItems = [{
                    label: uc_resource.Menu.GroupSetting,
                    type: uc.constants.UI.MenuItemType.MenuItem,
                    disabled: false,
                    callback: function () {
                        uc.IUIService.showPopupGroupProfile(groupId);
                    }
                }, {
                    label: uc_resource.Menu.ViewCalendar,
                    type: uc.constants.UI.MenuItemType.MenuItem,
                    disabled: false,
                    callback: function () {
                        var groupInfo = uc.IGroupService.getGroupInfo(groupId);
                        var users = [];
                        for (var i = 0; i < groupInfo.members.length; i++) {
                            users.push(groupInfo.members[i].userId);
                        }

                        var param = {
                            shareUserId: uc.constants.Meeting.AuthorizeMySelf,
                            isCreate: true,
                            users: users
                        };

                        uc.IUIService.createMeeting("ContactGroupContextMenuCreateMeeting", param);
                    }
                }, {
                    label: uc_resource.Menu.QuitGroup,
                    type: uc.constants.UI.MenuItemType.MenuItem,
                    disabled: false,
                    callback: function () {
                        var message = uc_resource.Profile.ConfirmExit;
                        uc.modules.component.PromptMessageBox.open({
                            title: uc_resource.Common.Prompt,
                            message: message,
                            buttonMode: "both",
                            OkText: uc_resource.Common.OK,
                            CancelText: uc_resource.Common.Cancel
                        }, function (result) {
                            if (result == 0) {
                                uc.IGroupService.exitGroup(groupId, function (args) {
                                });
                            }
                            this.close();
                        });
                    }
                }];
            }

            this.setCapability(menuItems);

            var menuOptions = {
                hasIcon: false,
                menuItems: menuItems
            };
            uc.modules.component.Menu.showMenu(e, menuOptions);
            e.stopPropagation();
            e.preventDefault();
        },
        showMeetingContextMenu: function (e, meetingInfo) {
            var authorizerId = uc.modules.meeting.MeetingListManager.getAuthorizerId();
            var instanceId = "_" + new Date().getTime();

            var menuItems = [];

            var joinItem = {
                label: uc_resource.Menu.JoinMeeting,
                type: uc.constants.UI.MenuItemType.MenuItem,
                disabled: false,
                callback: function () {
                    uc.IUIService.joinMeeting(meetingInfo, true, true);
                }
            };


            //******************************************************************
            //   isGnet  |  hostId==currentId | shareUserId  |  join | forward
            //     0     |          0         |      0       =>   0  |    1
            //     0     |          0         |      1       =>   0  |    1
            //     0     |          1         |      0       =>   0  |    1
            //     0     |          1         |      1       =>   0  |    1
            //     1     |          0         |      0       =>   1  |    1
            //     1     |          0         |      1       =>   0  |    1
            //     1     |          1         |      0       =>   1  |    1
            //     1     |          1         |      1       =>   0  |    1
            //*******************************************************************
            if (meetingInfo.isGNet && !authorizerId) {
                menuItems.push(joinItem);
            }

            var chatItem = {
                label: uc_resource.Menu.ViewMeetingChat,
                type: uc.constants.UI.MenuItemType.MenuItem,
                disabled: false,
                callback: function(){
                    uc.modules.meeting.MeetingDetailManager.showMeetingChat(meetingInfo);
                }
            };

            menuItems.push(chatItem);

            var forwardItem = {
                label: uc_resource.Menu.ForwardMeeting,
                type: uc.constants.UI.MenuItemType.MenuItem,
                disabled: false,
                callback: function () {
                    var selectedUserList = [];
                    selectedUserList.push(meetingInfo.hostId);
                    if ($.isArray(meetingInfo.attendees) && meetingInfo.attendees.length > 0) {
                        $.each(meetingInfo.attendees, function (key, attendees) {
                            if (attendees.type == uc.constants.Meeting.AttendeeType.User) {
                                selectedUserList.push(parseInt(attendees.account));
                            } else {
                                selectedUserList.push(attendees.account);
                            }
                        });
                    }

                    uc.util.StorageUtil.setData('ForwardUserIds:', JSON.stringify({data: selectedUserList}));

                    uc.IUIService.openChooseContactWindow({
                        key: "MeetingForward" + instanceId,
                        fromForward: true,
                        selectedUserIdList: [],
                        isMeetingForward: true
                    }, function (args) {
                        if (args.okOrCancel) {
                            if (args.contacts.length > 0) {
                                $("span.empty-mark").remove();
                            }

                            var userIds = JSON.parse(uc.util.StorageUtil.getData('ForwardUserIds:')).data;
                            var persons = [], isRepeat = false;
                            $.each(args.contacts, function (key, attendees) {
                                var type = attendees.userId ? uc.constants.Meeting.AttendeeType.User : uc.constants.Meeting.AttendeeType.Mail;
                                if (userIds.indexOf(attendees.userId) >= 0 || attendees.userId == meetingInfo.hostId) {
                                    isRepeat = true;
                                    return true;
                                }
                                persons.push({
                                    account: (attendees.userId || attendees.account) + "",                                        // 账号
                                    type: type,                           // 参会人类型：1 内部用户 ?2 邮件用户�?3 电话用户
                                    status: 0,                                                              // 参会人状态：0 接受 ?1 拒绝 ?2 未处 ?
                                    //startTime: 12432143142123,                                              // 入会时间
                                    name: attendees.displayName                                            // 参会人名 ?
                                });
                            });

                            if (persons.length) {
                                uc.util.LogUtil.info("MeetingPanel", "_forwardMeeting", "forwardMeeting", {
                                        eventId: meetingInfo.eventId, persons: persons,
                                        shareUserId: authorizerId, startTime: meetingInfo.startTime
                                    });
                                uc.IMeetingService.forwardMeeting(meetingInfo.eventId, persons, 0, authorizerId, 1, 1);
                            } else {
                                var msg = uc_resource.Meeting.AddForwardUser;
                                if (isRepeat) {
                                    msg = uc_resource.Meeting.NotForwardRepeat;
                                }
                                uc.modules.component.PromptMessageBox.open({
                                    title: uc_resource.WindowTitle.Notification,
                                    message: msg,
                                    buttonMode: "ok",
                                    OkText: uc_resource.Common.OK
                                }, function (result) {
                                    this.close();
                                });
                            }
                        }
                    });
                }
            };

            menuItems.push(forwardItem);

            var loginUserInfo = uc.IContactService.getCurrentUserInfo();
            if(loginUserInfo.userId == meetingInfo.hostId || meetingInfo.authorizerId == meetingInfo.hostId){
                var editItem = {
                    label: uc_resource.Menu.EditMeeting,
                    type: uc.constants.UI.MenuItemType.MenuItem,
                    disabled: false,
                    callback: function(){
                        var param = {
                            meetingInfo: meetingInfo,
                            isCreate: false
                        };

                        uc.IUIService.createMeeting("ConversationContextEditMeeting", param);
                    }
                };

                var cancelItem = {
                    label: uc_resource.Menu.CancelMeeting,
                    type: uc.constants.UI.MenuItemType.MenuItem,
                    disabled: false,
                    callback: function(){
                        uc.modules.component.PromptMessageBox.open({
                            title: uc_resource.WindowTitle.Notification,
                            message: uc_resource.Meeting.CanceledMeeting,
                            CancelText: uc_resource.Common.Cancel,
                            buttonMode: "both",
                            OkText: uc_resource.Common.OK
                        }, function (result) {
                            if (result) {
                                this.close();
                            } else {
                                this.close();
                                uc.IMeetingService.cancelMeeting(meetingInfo.eventId, 0, authorizerId, 1, 1,function (args) {
                                    uc.util.LogUtil.info("MenuUtil", "showMeetingContextMenu", "cancelMeeting", {
                                        errorCode: args[0],
                                        eventId: meetingInfo.eventId
                                    });
                                });
                            }
                        });
                    }
                };

                menuItems.push(editItem);
                menuItems.push(cancelItem);
            }

            var menuOptions = {
                hasIcon: false,
                menuItems: menuItems
            };
            uc.modules.component.Menu.showMenu(e, menuOptions);
            e.stopPropagation();
            e.preventDefault();
        },
        showStartCallContextMenu: function(e, telphone,callAuthority,_callback){
		    var menuItems = [];
	        if(callAuthority){
		        menuItems.push({
			        label: uc_resource.Menu.callbee,
			        type: uc.constants.UI.MenuItemType.MenuItem,
			        disabled: false,
			        callback: function(){
				        _callback(0);
			        }
		        });
	        }
		    if(telphone != null){
			    var telNumber;
			    for( var i = 0; i < telphone.length; i++ ){
				    telNumber = telphone[i];
				    menuItems.push({
					    label:telNumber,
					    type: uc.constants.UI.MenuItemType.MenuItem,
					    disabled: false,
					    callback: function(telNumber){
						    //PBX
						    _callback(1,telNumber);
					    }
				    });
			    }
		    }
		    this.setCapability(menuItems);
		    var menuOptions = {
			    hasIcon: false,
			    menuItems: menuItems
		    };
		    uc.modules.component.Menu.showMenu(e, menuOptions);
		    e.stopPropagation();
		    e.preventDefault();
	    },

        showMeetingListContextMenu: function(e, options){
            var menuItems = [];
            var newItem = {
                label: uc_resource.Meeting.CreateCalendar,
                type: uc.constants.UI.MenuItemType.MenuItem,
                disabled: false,
                callback: function(){
                    var param = {
                        startTime: options.startTime,
                        type: uc.constants.Meeting.MeetingCreateType.Default,
                        isCreate: true
                    };

                    uc.IUIService.createMeeting("CreateMeeting", param);
                }
            };

            menuItems.push(newItem);
            var menuOptions = {
                hasIcon: false,
                menuItems: menuItems
            };
            uc.modules.component.Menu.showMenu(e, menuOptions);
            e.stopPropagation();
            e.preventDefault();
        },
        getPBXVisible: function(){
            return uc.modules.main.Main.getPBXVisible();
        },

        getCallVisible: function(){
            return uc.modules.main.Main.getCallVisible();
        }
}
})(uc);