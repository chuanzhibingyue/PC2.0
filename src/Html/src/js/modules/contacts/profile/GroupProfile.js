(function (uc) {
	var uploadTaskMap = new uc.util.HashMap();
	var isGrouplistloading = false;
	var LOADCOUNT = 30;
	var CurrentGroupMembers=[];
	var loadIndex = 0;
	var currentIndex = 0;
	uc.modules.profile.GroupProfile = {
	    messageDisturbSwitcher: null,
        groupId: 0,
        init: function () {
            var self = this;
            var groupId = uc.util.StorageUtil.getData('popupGroupProfile');
            this.groupId = groupId;
            uc.util.LogUtil.info("GroupProfile", "init", "getGroupId=" + groupId, null);
            uc.modules.component.TitleBarButton.init();
            uc.IUIService.bindCrossWindowEvent("SendGroupId", function (data) {
                if (data.data && data.data.errorCode == 0) {
                    if (data.data.groupId == groupId) {
                        self.initUI(groupId);
                    }
                }
            });
            this.initUI(groupId);
        },
        initUI: function (groupId) {
            var groupInfo = uc.IGroupService.getGroupInfo(groupId);
            if (groupInfo) {
                this.loadGroupInfo(groupInfo);
                this.bindEvent();
            }
        },
        sortGroupmemberData: function(_groupInfo){
            var userInfo = uc.IContactService.getCurrentUserInfo();
            var _CurrentGroupMembers = [];
            loadIndex = 0;
            currentIndex = 0;
            var adminItem, currentUserItem;
            $.each(_groupInfo.members, function (key, item) {
                if (Number(item.userId) == _groupInfo.adminId) {
                    adminItem = item;
                } else if (Number(item.userId) == userInfo.userId) {
                    currentUserItem = item;
                } else {
                    _CurrentGroupMembers.push(item);
                }
            });
            if (currentUserItem) {
                _CurrentGroupMembers.unshift(currentUserItem);
            }
            if (adminItem) {
                _CurrentGroupMembers.unshift(adminItem);
            }
            return _CurrentGroupMembers;
        },
        combineGroupAvatarUrlArray: function (group) {
            var groupId = group.id;
            var twoMembers = uc.modules.group.GroupManager.getGroupTwoMembers(groupId, group);
            var groupAvatarUrlArray = [];

            $.each(twoMembers, function (key, item) {
                if (item) {
                    var contact = uc.IContactService.getBasicContactInfo(Number(item));
                    if (contact) {
                        groupAvatarUrlArray.push(contact.localAvatar);
                    }

                }
            });

            return groupAvatarUrlArray;
        },
        loadGroupInfo: function (group) {
            //sort ,unshift adminuser and loginuser
            CurrentGroupMembers = this.sortGroupmemberData(group);
            var _self = this;
            var mainPanel = $(".groupProfile .menu-panel-body-body");
            mainPanel.empty();

            var template = $('<div class="group-detail text-selectable"></div>');
            var userInfo = uc.IContactService.getCurrentUserInfo();
            if (group.adminId == userInfo.userId) {
                template.addClass("group-admin");
            }

            template.attr("groupId", group.id);
            template.attr("groupType", group.groupType);
            template.addClass("group-detail-" + group.id);

            var groupDetailUp = $('<div class="group-detail-up"></div>');
            var groupDetailDown = $('<div class="group-detail-down"></div>');
            template.append(groupDetailUp);
            template.append(groupDetailDown);

            var groupWrapper = $('<div class="profile-wrapper">'
                + '<div class="left-content">'
                + '<div class="group-name-container"><span class="group-name text-ellipsis" qtips-show-by="mouse" ></span><span class="group-count-container text-ellipsis">(<span class="group-count">0</span>)</span><span class="edit-group text-ellipsis"></span></div>'
                + '<div class="group-name-edit"><input maxlength="64" type="input" class="group-name-input"/></div>'
                + '<div class="group-creator-container"><span class="group-creator-titile">' + uc_resource.Profile.ProjectCreater + '</span><span class="group-creator"></span></div>'
                + '</div>'
                + '<div class="group-avatar"></div>'
                + '</div>');
            var editGroup = groupWrapper.find('.edit-group');

            if (group.groupType == uc.constants.GroupType.Project && group.adminId == userInfo.userId
                || group.groupType == uc.constants.GroupType.Group) {
                editGroup.show().css("display", "inline-block");
            }

            groupWrapper.find('.group-creator').addClass("group-creator-" + group.adminId);

            uc.modules.group.GroupManager.setGroupName(groupWrapper.find('.group-name'), group.name);

            groupWrapper.find('.group-name-input').val(group.name);
            groupWrapper.find('.group-name-edit').attr("groupId", group.id);
            groupWrapper.find('.group-count').text(CurrentGroupMembers.length);
            var contact = uc.IContactService.getBasicContactInfo(Number(group.adminId));
            if (contact) {
                groupWrapper.find('.group-creator').text(contact.displayName);
            } else {
                uc.util.LogUtil.info("GroupProfile", "getContactInfo", "getContactInfo failed");
            }
            var groupAvatar;
            if (group.groupType == uc.constants.GroupType.Project) {
                var defaultAvatar = uc.constants.UI.DefaultLocalUrl.Project;

                groupAvatar = $('<div class="project-group-avatar">'
                    + '     <img src="' + defaultAvatar + '" onerror="uc.util.ImageUtil.OnImageLoadError(this,\'' + defaultAvatar + '\')" class="project-avatar round-image"/>'
                    + '</div>')
                groupAvatar.find('.project-avatar').addClass("project-avatar-" + group.id);
                groupWrapper.find('.group-avatar').replaceWith(groupAvatar);

                if (group.adminId == userInfo.userId) {
                    var projectAvatarEdit = $('<div class="rounded-corners project-avatar-edit">'
                        + '</div>');
                    groupWrapper.append(projectAvatarEdit);
                }
                uc.modules.group.GroupManager.displayProjectAvatar(group, groupAvatar.find('.project-avatar'));

            } else {
                var groupAvatarUrlArray = this.combineGroupAvatarUrlArray(group);
                groupAvatar = uc.util.AvatarUtil.composeGroupAvatar(groupAvatarUrlArray, uc.constants.UI.DefaultLocalUrl.Avatar);
                groupWrapper.find('.group-avatar').replaceWith(groupAvatar);
            }

            groupDetailUp.append(groupWrapper);

            var actionsBar = $('<div class="actions-bar"></div>');
            actionsBar.attr("groupId", group.id);
            actionsBar.attr("groupType", group.groupType);
            groupDetailUp.append(actionsBar);

            /*var sendMessageFigure = $('<div class="send-content"><figure class="send-message">'
                + '<figcaption class="send-message-figcaption"></figcaption>'
                + '<figcaption>' + uc_resource.Profile.SendMessage + '</figcaption>'
                + '</figure>' +
                '<figure class="send-call">'
                + '<figcaption class="send-message-figcaption"></figcaption>'
                + '<figcaption>' + uc_resource.Profile.SendCall + '</figcaption>'
                + '</figure></div>');
            actionsBar.append(sendMessageFigure); */

	        var sendMessageFigure = $('<div class="send-content"><figure class="send-message">'
		        + '<figcaption class="send-message-figcaption"></figcaption>'
		        + '<figcaption>' + uc_resource.Profile.SendMessage + '</figcaption>'
		        + '</figure></div>');
	        actionsBar.append(sendMessageFigure);
	        //呼
	        var callFigure = $('<figure class="view-Groupcall">'
		        + '<figcaption class="view-GroupCall-figcaption" qtips="'+ uc_resource.Menu.Call +'"></figcaption>'
		        + '</figure>');
	        actionsBar.append(callFigure);

            var checkSchedule = $('<figure class="view-calendar hide">'
                + '<figcaption class="view-calendar-figcaption"></figcaption>'
                //+ '<figcaption>' + uc_resource.Profile.ViewCalendar + '</figcaption>'
                + '</figure>');
            actionsBar.append(checkSchedule);
            groupDetailUp.append(actionsBar);
            actionsBar.find(".view-calendar-figcaption").attr({"qtips":uc_resource.Meeting.CreateCalendar});
            var switchActionsBar = $('<div class="switch-actions-bar">'
                + '    <span class="switch-action switch-to-members">' + uc_resource.Profile.Members + '</span><span class="switch-action switch-to-settings">' + uc_resource.Profile.Setting + '</span>'
                + '</div>');
            groupDetailUp.append(switchActionsBar);

            var membersSelected = '<div class="switch-to-selected switch-to-members-selected"></div>';
            groupDetailUp.append(membersSelected);

            var settingsSelected = '<div class="switch-to-selected switch-to-settings-selected"></div>';
            groupDetailUp.append(settingsSelected);

            var membersPanel = $('<div class="group-members-panel"></div>');
            var addMembers = $('<div class="add-members">'
                + '        <div class="left"> '
                + '            <div class="add-member-icon">'
                + '                <img src="../images/contacts/add_member.png">'
                + '            </div>'
                + '        </div>'
                + '        <div class="right">'
                + '        </div>'
                + '        <div class="center add-member-text">' + uc_resource.Profile.AddNewMember + '</div>'
                + '    </div>');
            //if(group.adminId == userInfo.userId){
            membersPanel.append(addMembers);
            //}
            groupDetailDown.append(membersPanel);

            var contactList = $('<ul class="contact-list"></ul>');
            membersPanel.append(contactList);


            this.loadGroupMembersMainUI(contactList, this.gatGroupMemberData(), group.adminId);

            var settingPanel = $('<div class="group-settings"></div>');
            var chatSettings = $('<div class="chat-settings">'
                + ' <div class="chat-settings-title">' + uc_resource.Profile.ChatSetting + '</div>'
                + '    <ul class="chat-setting-list">'
                + '        <li class="chat-setting-item chat-setting-message-disturb">'
                + '         <div class="chat-setting-message-disturb-status"></div><div class="chat-setting-message-disturb-title">' + uc_resource.Profile.NoDisturbing + '</div>'
                + '     </li>'
                + '    </ul>'
                + '</div> ');
            var disturbStatus = chatSettings.find('.chat-setting-message-disturb-status');
            disturbStatus.attr("groupId", group.id);

            _self.messageDisturbSwitcher = new uc.modules.component.SwitchButton({
                container: disturbStatus,
                onChanged: function (value) {
                    var groupDetail = $('.group-detail:visible');
                    var groupId = Number(groupDetail.attr("groupId"));
                    var newStatus = value ? 1 : 0;
                    //Set status to group
                    uc.IGroupService.setDoNotDisturb(groupId, newStatus, function (result) {
                    });
                }
            });
            _self.messageDisturbSwitcher.setValue(group.disturbState == 1);
            settingPanel.append(chatSettings);
            var buttonPanel = $('<div class="button-panel">'
                + '     <button class="btn btn-exit-group">' + uc_resource.Profile.ExitGroup + '</button>'
                + '    </div>');

            // start------------------------modified by xue.bai_2 on 2016/04/15----------------------------
            var button = buttonPanel.find('.btn-exit-group');
            if (group.groupType == uc.constants.GroupType.Project) {
                button.text(uc_resource.Profile.ExitProject);
            }

            if (group.adminId == userInfo.userId) {
                button.addClass("btn-end-group").removeClass("btn-exit-group");
                button.text(uc_resource.Profile.EndProject);
                if(group.groupType == uc.constants.GroupType.Group){
                    button.text(uc_resource.Profile.EndGroup);
                }
            }
            // ------------------------modified by xue.bai_2 on 2016/04/15----------------------------end

            settingPanel.append(buttonPanel);

            groupDetailDown.append(settingPanel);

            template.attr("groupId", group.id);
            mainPanel.append(template);
            mainPanel.fadeIn('slow');

            //add by xiege.
            if (uc.constants.Switch.OpenMultiVersion) {
                var version = JSON.parse(uc.util.StorageUtil.getData("capability"));
                if (version) {
                    switch (version.im) {
                        case "0":
                            $(".send-message").addClass("hide");
                            break;
                    }
                    switch (version.calendar) {
                        case "0":
                            checkSchedule.find(".view-calendar-figcaption").addClass("hide");
                            break;
                    }
                    //switch (version.conf) {
                    //}
                    switch (version.call) {
                        case "0":
                            $(".send-call").addClass("hide");
                            $(".view-Groupcall").addClass("hide");
                    }
                    //switch (version.cloud_disk_type) {
                    //}
                    switch (version.app_center) {
                        case "0":
                            //socialFigure.addClass("hide");
                            break;
                    }
                    switch (version.app_moments) {
                        case "0":
                            //uc.ui.Contacts.ContentPanelBody.find(".attention").addClass("hide");
                            break;
                    }
                    //switch (version.scan) {
                    //}
                    //switch (version.remote_assistant) {
                    //}
                }
            }
        },
        bindEvent: function () {
            var _self = this;
            var mainPanel = $(".groupProfile .menu-panel-body-body");
            mainPanel.off("click");
            mainPanel.off("blur");

            mainPanel.on("click", ".group-detail .group-name-container .edit-group", function () {

                var groupNameContainer = $(this).parent('.group-name-container');
                var parent = groupNameContainer.parent('.left-content');
                var groupNameEdit = parent.find('.group-name-edit');
                groupNameContainer.hide();
                groupNameEdit.show();
                groupNameEdit.find('.group-name-input').focus();
            });

            mainPanel.on("keydown", ".group-name-input", function (event) {
                    if (event.keyCode == 13) {
                        $(".group-name-input").blur();
                    }
                }
            );


            mainPanel.on("blur", ".group-detail .group-name-input", function () {

                var groupNameNew = $(this).val();
                var groupNameEdit = $(this).parent('.group-name-edit');
                var groupId = Number(groupNameEdit.attr("groupId"));
                var parent = groupNameEdit.parent('.left-content');

                var groupNameContainer = parent.find('.group-name-container');
                var groupNameElement = groupNameContainer.find('.group-name');
                var groupNameBefore = groupNameElement.text();
                uc.modules.group.GroupManager.setGroupName(groupNameElement, groupNameNew);

                groupNameEdit.hide();
                groupNameContainer.show();

                //Call backend to save group name.
                if (groupNameNew != groupNameBefore) {
                    //Change in Group list UI
                    var groupItem = $('.group-item-' + groupId);
                    if (groupItem.length > 0) {
                        uc.modules.group.GroupManager.setGroupName(groupItem, groupNameNew);
                    }

                    uc.IGroupService.setGroupName(
                        groupId,
                        groupNameNew,
                        function (args) {
                            var errorCode = args[0];
                        }
                    );
                }
            });

            mainPanel.on("click", ".group-detail .project-avatar-edit", function () {
                uc.IUIService.openImageCropperWindow("ChangeGroupAvatar", function (args) {
                    if (args.okOrCancel) {
                        var croppedImagePath = args.croppedImagePath;

                        var groupDetail = $('.group-detail');
                        var groupId = Number(groupDetail.attr("groupId"));

                        uc.modules.group.GroupManager.uploadProjectAvatar(groupId, croppedImagePath);
                    }
                });
            });

            mainPanel.on("click", ".send-message figcaption", function () {
                var actionsBar = $(this).closest('.actions-bar');
                var groupId = actionsBar.attr("groupId");
                var groupType = actionsBar.attr("groupType");
                var chatType = uc.constants.Chat.ChatType.GroupChat;
                if (groupType == 1) {
                    chatType = uc.constants.Chat.ChatType.ProjectChat;
                }
                uc.IUIService.triggerCrossWindowEvent(uc.constants.UI.CrossWindowEventName.ReOpenCallWindowFormMessage, {chatType: chatType, Id: groupId});
                uc.IUIService.triggerCrossWindowEvent(uc.constants.UI.CrossWindowEventName.OpenChatWindow, {chatType: chatType, Id: groupId});

                uc.util.WindowUtil.close();
            });

            mainPanel.on("click", ".view-Groupcall figcaption", function() {
                var actionsBar = $(this).closest('.actions-bar');
                var groupId = actionsBar.attr("groupId");
                var groupType = actionsBar.attr("groupType");
                var chatType = uc.constants.Chat.ChatType.GroupChat;
                if(groupType == 1) {
                    chatType = uc.constants.Chat.ChatType.ProjectChat;
                }
                uc.IUIService.triggerCrossWindowEvent(uc.constants.UI.CrossWindowEventName.ReOpenCallWindowFormMessage, {chatType: chatType, Id: groupId});
                uc.IUIService.triggerCrossWindowEvent(uc.constants.UI.CrossWindowEventName.OpenChatWindow, {chatType: chatType, Id: groupId,isFromCall:true});
                uc.IUIService.bindCrossWindowEvent(uc.constants.UI.CrossWindowEventName.CloseGroupProfile, function(){
                    uc.util.WindowUtil.close();
                });

                // uc.util.WindowUtil.close();
            });
            
            mainPanel.on("click", ".view-calendar figcaption", function () {
                var actionsBar = $(this).closest('.actions-bar');
                var userIdArray = [];
                var userId = actionsBar.attr("userId");
                var currentUserInfo = uc.IContactService.getCurrentUserInfo();
                var currentUserId = currentUserInfo.userId;
                var groupId = Number(actionsBar.attr("groupId"));
                var group = uc.IGroupService.getGroupInfo(groupId);
                if(userId && currentUserId){
                    userIdArray.push(Number(userId));
                    userIdArray.push(Number(currentUserId));
                } else {
                    if(group != null){
                        var members = group.members;
                        $.each(members,function(key, member){
                            var userId = member.userId;
                            userIdArray.push(userId);
                        });
                    }
                }

                var param = {
                    shareUserId: uc.constants.Meeting.AuthorizeMySelf,
                    isCreate: true,
                    users: userIdArray
                };

                uc.IUIService.createMeeting("GroupProfileCreateMeeting", param);
            });

            mainPanel.on("click", ".group-detail .switch-actions-bar .switch-to-members", function () {
                var membersPanel = $('.group-detail .group-members-panel');
                var groupSettings = $(".group-detail .group-settings");

                if (membersPanel.is(":visible")) {
                    return;
                }

                $('.switch-to-members-selected').show();
                $('.switch-to-settings-selected').hide();

                groupSettings.hide();
                membersPanel.show();
            });

            mainPanel.on("click", ".group-detail .switch-actions-bar .switch-to-settings", function () {
                var membersPanel = $('.group-detail .group-members-panel');
                var groupSettings = $(".group-detail .group-settings");

                if (groupSettings.is(":visible")) {
                    return;
                }

                $('.switch-to-members-selected').hide();
                $('.switch-to-settings-selected').show();

                membersPanel.hide();
                groupSettings.show();
            });

            mainPanel.on("click", ".group-detail .add-members .add-member-icon, .group-detail .add-members .add-member-text", function () {
                var userIdArray = [];
                var groupId = Number(mainPanel.find(".group-detail").attr("groupId"));
                var group = uc.IGroupService.getGroupInfo(groupId);
                var groupCallId='';
                if(group != null){
                    groupCallId=(group.statusInfo !="" && group.statusInfo !="[]")? $.parseJSON(group.statusInfo).tempConferenceId:'';
                    var members = group.members;
                    $.each(members,function(key, member){
                        var userId = member.userId;
                        userIdArray.push(parseInt(userId));
                    });
                }

                var addCall=false;
                if(groupCallId){
                    if(members.length >= 9){
                        uc.modules.component.PromptMessageBox.open({
                            title: uc_resource.WindowTitle.Notification,
                            message: uc_resource.Call.GroupAddMemberFailed,
                            buttonMode: "ok",
                            OkText: uc_resource.Common.OK
                        }, function () {
                            this.close();
                        });
                        return;
                    }
                    uc.modules.component.PromptMessageBox.open({
                        title: uc_resource.WindowTitle.Notification,
                        message: uc_resource.Call.GroupHaveCall,
                        buttonMode: "both",
                        OkText: uc_resource.Common.OK,
                        CancelText: uc_resource.Common.Cancel
                    }, function (result) {
                        if(!result){
                            uc.IUIService.openChooseCallContactsWindow({
                                key:"chooseCallContacts_" + new Date().getTime(),
                                userList:userIdArray
                            }, function (args) {
                                var groupDetail = $('.group-detail');
                                var groupId = Number(groupDetail.attr("groupId"));
                                var groupType = Number(groupDetail.attr("groupType"));
                                var userStatus=[];
                                var callUser=[];
                                for(var i = 0 ;i < args.memberList.length; i++){
                                    userStatus.push(args.memberList[i].userId);
                                    for (var j = 0; j < userIdArray.length; j++){
                                        var flag=false;
                                        if(args.memberList[i].userId == userIdArray[j]){
                                            flag=true;
                                            break;
                                        }

                                    }
                                    if(!flag){
                                        callUser.push(args.memberList[i])
                                    }
                                }
                                if(!callUser.length) {
                                    uc.IUIService.triggerCrossWindowEvent(uc.constants.UI.CrossWindowEventName.chooseContactsCreateCall, {callStatus: uc.constants.UI.chooseCallContacts.createcall_fail,failedType:uc.constants.Call.CallFailedType.SelectedUser});
                                    return;
                                }

                                uc.IUIService.triggerCrossWindowEvent(uc.constants.UI.CrossWindowEventName.AddGroupMemberToCall, {userList:callUser,groupId:groupId,chatType:groupType,meetingId:groupCallId});
                            });

                        }
                        this.close();
                    });

                }else{
                    uc.IUIService.openChooseContactWindow({
                        key:"PopupAddGroupMembers",
                        selectedUserIdList: userIdArray,
                        memberType: group.memberType
                    }, function (args) {
                        if (args.okOrCancel) {
                            var contacts = args.contacts;
                            var groupDetail = $('.group-detail');
                            var groupId = Number(groupDetail.attr("groupId"));
                            var groupType = Number(groupDetail.attr("groupType"));

                        //Will rerender UI when success
                        uc.modules.profile.GroupProfile.addGroupMembers(groupId, groupType, contacts);
                        }
                    });
                }
            });

            var clickContactTimer = -1;
            mainPanel.on("click", ".contact-item", function () {
                clearTimeout(clickContactTimer);
                var userId = Number($(this).attr("userid"));
                clickContactTimer = setTimeout(function () {
                    uc.IUIService.showPopupContactProfile(userId);
                }, 300);
            });

            var currentUserInfo = uc.IContactService.getCurrentUserInfo();
            mainPanel.on("dblclick", ".contact-item", function () {
                clearTimeout(clickContactTimer);
                var userId = Number($(this).attr("userid"));

                if (currentUserInfo.userId != userId) {
                    uc.IUIService.triggerCrossWindowEvent(uc.constants.UI.CrossWindowEventName.ReOpenCallWindowFormMessage, {chatType: uc.constants.Chat.ChatType.SingleChat, Id: userId});
                    uc.IUIService.triggerCrossWindowEvent(uc.constants.UI.CrossWindowEventName.OpenChatWindow, {chatType: uc.constants.Chat.ChatType.SingleChat, Id: userId});
                    uc.util.WindowUtil.close();
                }
            });

            mainPanel.on("click", ".remove-contact", function (e) {
                var contactItem = $(this).parents('.contact-item');

                var groupDetail = $('.group-detail');
                var groupId = Number(groupDetail.attr("groupId"));

                var userId = Number(contactItem.attr("userId"));
                var userName = contactItem.find('.contact-display-name').text();
                var message = uc.util.StringUtil.formatString(uc_resource.Profile.RemoveMember, [userName]);

                var userIdArray = [];
                userIdArray.push(userId);

                uc.modules.component.PromptMessageBox.open({title: uc_resource.Profile.Prompt, message: message, buttonMode: "both", OkText: uc_resource.Common.OK, CancelText: uc_resource.Common.Cancel}, function (result) {
                    if (result == 0) {
                        uc.IGroupService.removeMember(
                            groupId,
                            userIdArray,
                            function (args) {
                                var errorCode = args[0];
                                if (errorCode == 0) {
                                    contactItem.remove();
                                }
                            }
                        );
                    }
                    this.close();
                });

                e.stopPropagation();
            });

            mainPanel.on("click", ".btn-exit-group", function () {
                var button = $(this);
	            var groupDetail = button.parents('.group-detail');
	            var groupId = Number(groupDetail.attr("groupId"));
	            var message;
	            //在此处判断当前玩家是否在GroupCall中，并且比较GroupCall的groupId，如果相同，则弹出在呼中提示，否则弹出正常退出群聊提示
	            var InProgressCallInfo = uc.util.StorageUtil.getData('InProgressCallInfo');
	            if(InProgressCallInfo.isInProgress){
		            var groupInfo = uc.IGroupService.getGroupInfo(InProgressCallInfo.chatId);
		            if(groupInfo.id == groupId){
			            message = uc_resource.Profile.ConfirmExitIfInGroupCall;
		            }else{
			            message = uc_resource.Profile.ConfirmExit;
		            }
	            }
	            else{
		            message = uc_resource.Profile.ConfirmExit;
	            }
	            uc.modules.component.PromptMessageBox.open({title: uc_resource.Profile.Prompt, message: message, buttonMode: "both", OkText: uc_resource.Common.OK, CancelText: uc_resource.Common.Cancel}, function (result) {
		            if (result == 0) {
			            uc.IGroupService.exitGroup(groupId, function (args) {
			            });
		            }
		            this.close();
		            uc.util.WindowUtil.close();
	            });
            });

            mainPanel.on("click", ".btn-end-group", function () {
	            var button = $(this);
	            var groupDetail = button.parents('.group-detail');
	            var groupId = Number(groupDetail.attr("groupId"));
	            var message;
	            //在此处判断当前玩家是否在GroupCall中，并且比较GroupCall的groupId，如果相同，则弹出在呼中提示，否则弹出正常退出群聊提示
	            var InProgressCallInfo = uc.util.StorageUtil.getData('InProgressCallInfo');
	            if(InProgressCallInfo.isInProgress){
		            var groupInfo = uc.IGroupService.getGroupInfo(InProgressCallInfo.chatId);
		            if(groupInfo.id == groupId){
			            message = uc_resource.Profile.ConfirmExitIfInGroupCall;
		            }else{
			            message = uc_resource.Profile.ConfirmEnd;
		            }
	            }
	            else{
		            message = uc_resource.Profile.ConfirmEnd;
	            }
                uc.modules.component.PromptMessageBox.open({title: uc_resource.Profile.Prompt, message: message, buttonMode: "both", OkText: uc_resource.Common.OK, CancelText: uc_resource.Common.Cancel}, function (result) {
                    if (result == 0) {
                        uc.IGroupService.endGroup(groupId, function (args) {
                        });
                    }
                    this.close();
                    uc.util.WindowUtil.close();
                });
            });

            // _self.messageDisturbSwitcher.onChange = function (sender, args) {
            //     var groupDetail = $('.group-detail:visible');
            //     var groupId = Number(groupDetail.attr("groupId"));
            //     var newStatus = args.value ? 1 : 0;
            //     //Set status to group
            //     uc.IGroupService.setDoNotDisturb(groupId, newStatus, function (result) {
            //     });
            // };

            $('.group-detail-down').on('scroll', function () {
                if (!isGrouplistloading  && currentIndex < CurrentGroupMembers.length) {
                    if ($(this).height() + $(this)[0].scrollTop >= $(this)[0].scrollHeight - 60) {
                        isGrouplistloading = true;
                        loadIndex++;
                        $(this).append('<div class="groupMoreListloading"></div>');
                        //load next
                        var contactList = $('.group-detail .contact-list');
                        _self.loadGroupMembersMainUI(contactList, _self.gatGroupMemberData());
                        $(this).find(".groupMoreListloading").remove();
                        isGrouplistloading = false;
                    }
                }
            })
        },
        gatGroupMemberData: function () {
            var members = [], adminInfo, currentUserInfo, gmembers;
            //limit LOADCOUNT
            currentIndex = loadIndex * LOADCOUNT;
            console.log(currentIndex, currentIndex + LOADCOUNT);
            gmembers = CurrentGroupMembers.slice(currentIndex, currentIndex + LOADCOUNT);
            $.each(gmembers, function (key, item) {
                var contact = uc.IContactService.getBasicContactInfo(Number(item.userId));
                if (contact) {
                    members.push(contact);
                }
            });
            return members;
        },

        addGroupMembers: function (groupId, groupType, contacts) {
            var _self = this;

            //var originalMembers = _self.getGroupMembers(groupId, groupType);
            var group = uc.IGroupService.getGroupInfo(groupId);
            if (group) {
                var originalMembers = group.members;
            }


            var members = [];
            $.each(contacts, function (key, contact) {
                //If contact not exist in original group members, then add
                if (uc.util.CollectionUtil.findObjectByKey(originalMembers, "userId", contact.userId) == null) {
                    var member = {};
                    member.userId = contact.userId;
                    member.username = contact.pinyin;
                    members.push(member);
                }
            });

            if (members.length == 0) {
                return;
            }

            //Call backend to save to db
            uc.IGroupService.addMember(
                groupId, members,
                function (args) {
                    var errorCode = args[0];
                    if (errorCode == 0) {
                        //1. Update Group detail page
                        var contactList = $('.group-detail .contact-list');
                        CurrentGroupMembers = _self.sortGroupmemberData(group);
                        _self.loadGroupMembersMainUI(contactList, _self.gatGroupMemberData());
                        _self.changeGroupMembersCount();
                    }
                }
            );
        },
        changeGroupMembersCount: function () {
            var memberCount = CurrentGroupMembers.length;
            $('.group-count').text(memberCount);
        },
        removeGroupMember: function (groupId, userId, message, contactItem) {
            var userIdArray = [],
                _self = this;

            userIdArray.push(userId);

            uc.modules.component.PromptMessageBox.open({title: uc_resource.Profile.Prompt, message: message, buttonMode: "both"}, function (result) {
                if (result == 0) {
                    uc.IGroupService.removeMember(
                        groupId,
                        userIdArray,
                        function (args) {
                            var errorCode = args[0];
                            if (errorCode == 0) {
                                contactItem.remove();
                                _self.changeGroupMembersCount();
                            }
                        }
                    );
                }
                this.close();
            });
        },
        loadGroupMembersMainUI: function (contactList, contacts, adminId) {
            $.each(contacts, function (key, item) {
                if (contactList.find(".contact-item-" + item.userId).length > 0) {
                    return true;
                }
                var template = uc.modules.contact.ContactManager._getContactTemplate();
                //If admin, hide remove icon by css
                if (adminId && item.userId == adminId) {
                    template.addClass("contact-item-admin");

                    //Add an icon for Creator
                    var creatorIcon = '<div class="group-creator-icon">'
                        + '    <img src="../images/contacts/group_creator.png">'
                        + '</div>'
                    template.find('.left').append(creatorIcon);
                }
                template.addClass("contact-item-" + item.userId);
                template.attr("userId", item.userId);
                template.find('.checkbox-parent').addClass("check-box-contact-" + item.userId);
                template.find('.contact-display-name').text(item.displayName);
                template.find('.contact-display-name').attr("userId", item.userId);
                template.find('.contact-department').text(item.departmentName);
                template.find('.contact-position').text(item.position);

                var avatar = template.find(".left .avatar");
                avatar.addClass("avatar-" + item.userId);
                if (item.localAvatar) {
                    avatar.attr("src", item.localAvatar);
                    avatar.addClass("avatar-loaded");
                } else {
                    avatar.addClass("avatar-unloaded");
                    if (item.remoteAvatarUrl) {

                    }
                }

                contactList.append(template);
            });
        }
    };
    jQuery(document).ready(function () {
        uc.init();
        uc.modules.profile.GroupProfile.init();
    });
})(uc);