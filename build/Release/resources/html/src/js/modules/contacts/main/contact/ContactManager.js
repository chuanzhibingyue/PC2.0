/**
 * Created by senlin.ouyang on 2015/7/20.
 */
(function (uc) {
    var uploadTaskMap = new uc.util.HashMap();
    var frequentContactTaskMap = new uc.util.HashMap();
    var contactDownloadingTaskMap = new uc.util.HashMap();
    //var isSelf;
    var mobile;
    var userName;
    var _this = null;
    var isMainWindow = location.href.indexOf('/resources/html/page/main.html') != -1;
    //var isMainWindow = location.href.indexOf('/resources/html/page/main.html') != -1 ? true : false;
    var rootPath = (function () {
        var href = location.href;
        var index = href.indexOf('/html');

        return href.substring(0, index + 6);
    })();

    uc.modules.contact.ContactManager = {
        batchCount: 20,
        poundKeyReplacer: "ZZ",
        latestClickedContactsPanel: null,
        latestClickedPanel: null,
        cachedContacts: new uc.modules.contact.ContactDataSet(),
        cachedContactsStatus: new uc.modules.contact.ContactStatusDataSet(),
        currentUserStatus: 0,
        getCachedContacts: function () {
            return this.cachedContacts.getAllContacts();
        },
        addOrUpdateContactsToCache: function (contacts) {
            var _self = this;

            $.each(contacts, function (key, contact) {
                _self.addOrUpdateContactToCache(contact.userId, contact);
            });
        },
        addOrUpdateContactToCache: function (userId, contactInfo) {
            var _self = this;
            var needDownloadAvatar = false;

            if (!contactInfo) {
                return;
            }

            if (this.cachedContacts.isContactExist(userId)) {
                var contact = _self.getCachedContact(userId);

                if (contactInfo.localAvatar == "" && contact.localAvatar != "") {
                    contactInfo.localAvatar = contact.localAvatar;
                    needDownloadAvatar = true;
                }

                this.cachedContacts.updateContact(userId, contactInfo);
            } else {
                this.cachedContacts.addContact(userId, contactInfo);
            }

            if ((contactInfo.localAvatar == "" || needDownloadAvatar) && contactInfo.remoteAvatarUrl != ""
                && ( contactInfo.remoteAvatarUrl.toLowerCase().indexOf("http") >= 0 || contactInfo.remoteAvatarUrl.toLowerCase().indexOf("https") >= 0 )) {

                var key = _self.getContactDownloadingKey(contactInfo.userId, contactInfo.remoteAvatarUrl);

                if (!contactDownloadingTaskMap[key]) {
                    contactDownloadingTaskMap[key] = {
                        userId: contactInfo.userId,
                        remoteAvatarUrl: contactInfo.remoteAvatarUrl
                    };

                    uc.IUIService.downloadUserAvatar(contactInfo.userId, contactInfo.remoteAvatarUrl);
                }
            }
        },
        getContactDownloadingKey: function (userId, remoteAvatarUrl) {
            return userId + '_' + remoteAvatarUrl;
        },
        getCachedContact: function (userId) {
            return this.cachedContacts.getContactInfo(userId);
        },
        isContactExist: function (userId) {
            return this.cachedContacts.isContactExist(userId);
        },
        init: function () {
            if (this.initilized) {
                // getLastClickedPanel，显示当前选中的Panel的contact的status
                var Panel = this.getLatestClickedPanel();
                if (!Panel) {
                    return;
                }

                var userIds = uc.modules.contact.ContactManager.cachedContactsStatus.getContacts(Panel);
                var copyBack = '{"panel": "' + Panel + '", "panelUl": "contact-list"}';

                if (userIds) {
                    uc.util.LogUtil.info('ContactManager', 'init', 'getUserStatus', {
                        userIds: userIds,
                        copyBack: copyBack
                    });
                    uc.IContactService.getUserStatus(userIds, copyBack, function (args) {
                        uc.util.LogUtil.info('ContactManager', 'getUserStatus', 'response', {args: args});
                    });
                }

                return;
            }

            this.initilized = true;
            _this = this;

            this._bindEvents();
            this.showContactsPanel(uc.ui.Contacts.MenuPanel);
            this.onPanelChange("panel-contacts");
            this.bindUserDetailEvents(uc.ui.Contacts.ContentPanelBody);
            this.contactPanel = $('.menu-panel-body.contacts .content-panel-body');
            uc.modules.group.GroupManager.clearCachedChatGroups();
            uc.modules.group.GroupManager.clearCachedProjectGroups();
        },
        getLatestClickedPanel: function () {
            return this.latestClickedPanel;
        },
        setLatestClickedPanel: function (contactsPanel) {
            this.latestClickedPanel = contactsPanel;
        },
        getLatestClickedContactsPanel: function () {
            return this.latestClickedContactsPanel;
        },
        setLatestClickedContactsPanel: function (contactsPanel) {
            this.latestClickedContactsPanel = contactsPanel;
        },
        OnModuleChanged: function (moduleId) {
            var _self = this;

            if (moduleId == uc.constants.UI.ModuleId.Contacts) {
                _self.init();
            }
        },
        contactInfoReceived: function (errorCode, userIds, contacts) {
            var _self = this;
            var parentPanel = _self.getLatestClickedContactsPanel();

            if (parentPanel == null) {
                return;
            }

            if (errorCode == 0) {
                var selectedFigure = parentPanel.find('.figure-selected');
                var panel = selectedFigure.attr('panel');

                if (panel == "panel-projects" || panel == "panel-groupChats") {

                } else {
                    var contact = contacts[0];
                    this.selectedContactJudge(contact);
                }
            } else {
                uc.util.LogUtil.info('ContactManager', 'contactInfoReceived', 'errorCode = ' + errorCode, null);
            }
        },
        _bindEvents: function () {
            var _self = this;

            uc.ui.Contacts.MainPanel.off("click");

            uc.ui.Contacts.MainPanel.on("click", ".sub-menu-panel .contact-item", function () {
                if ($(this).hasClass("contact-selected")) {
                    return;
                }

                $('.contact-item').removeClass('contact-selected');
                $(this).addClass('contact-selected');

                var contentPanel = uc.ui.Contacts.ContentPanelBody;
                contentPanel.empty();
                contentPanel.hide();

                var userId = $(this).find('.contact-display-name').attr("userId");
                if (uc.constants.Switch.OpenUserStatus) {
                    // do something
                    if (Number(userId) !== currentUserInfo.userId) {
                        uc.IContactService.subscribeUserStatus([Number(userId)]);
                    }
                }

                var contact = uc.IContactService.getContactInfo(Number(userId));
                if (contact) {
                    _self.loadContactDetail(contact);
                }
            });

            uc.ui.Contacts.MainPanel.on("dblclick", ".sub-menu-panel .contact-item", function () {
                var userId = $(this).find('.contact-display-name').attr("userId");

                if (currentUserInfo.userId != userId) {
                    uc.IUIService.openChatWindow(uc.constants.Chat.ChatType.SingleChat, userId, uc.constants.UI.OpenChatWindowSourceModule.Other, null, null, true);
                }
            });

            uc.ui.Contacts.MainPanel.on("click", ".sub-menu-panel .group-item", function () {
                if ($(this).hasClass("group-selected")) {
                    return;
                }

                $('.group-item').removeClass('group-selected');
                $(this).addClass('group-selected');

                var contentPanel = uc.ui.Contacts.ContentPanelBody;
                contentPanel.empty();

                var groupId = Number($(this).attr("groupId"));
                var groupType = $(this).attr("groupType");

                var group = {};
                group.groupId = groupId;
                group.memberCount = Number($(this).attr("membersCount"));
                group.name = $(this).find('.group-name').text();
                group.adminId = Number($(this).find('.group-name').attr("adminId"));

                var groupAvatarUrlArray = [];
                var groupAvatars = $(this).find('.group-avatar img');
                $.each(groupAvatars, function (key, groupAvatar) {
                    var groupAvatarUrl = $(groupAvatar).attr("src");
                    groupAvatarUrlArray.push(groupAvatarUrl);
                });
                group.groupAvatarUrlArray = groupAvatarUrlArray;


                var groupInfo = uc.IGroupService.getGroupInfo(groupId);
                if (groupInfo) {
                    group.disturbState = groupInfo.disturbState;
                    group.groupType = groupInfo.groupType;
                    group.memberType = groupInfo.memberType;
                }

                uc.modules.group.GroupManager.loadGroupDetailTemplate(group);

            });

            uc.ui.Contacts.MainPanel.on("dblclick", ".sub-menu-panel .group-item", function () {
                var groupId = Number($(this).attr("groupId"));
                var groupType = Number($(this).attr("groupType"));
                var chatType = uc.constants.Chat.ChatType.GroupChat;

                if (groupType == 1) {
                    chatType = uc.constants.Chat.ChatType.ProjectChat;
                }

                uc.IUIService.openChatWindow(chatType, groupId, uc.constants.UI.OpenChatWindowSourceModule.Other, null, null, true);
            });

            var clickColleagueTextTimer = -1;
            uc.ui.Contacts.MainPanel.on("click", ".content-panel .colleague-list .left-edge:visible, .content-panel .colleague-list .text:visible", function () {
                clearTimeout(clickColleagueTextTimer);

                var colleagueItem = $(this).parent('.colleague-item');
                var userId = Number(colleagueItem.attr("userid"));

                clickColleagueTextTimer = setTimeout(function () {
                    uc.IUIService.showPopupContactProfile(userId);
                }, 300);
            });

            var currentUserInfo = uc.IContactService.getCurrentUserInfo();
            uc.ui.Contacts.MainPanel.on("dblclick", ".content-panel .colleague-list .left-edge:visible, .content-panel .colleague-list .text:visible", function () {
                clearTimeout(clickColleagueTextTimer);

                var userId = Number($(this).attr("userid"));

                if (currentUserInfo.userId != userId) {
                    uc.IUIService.openChatWindow(uc.constants.Chat.ChatType.SingleChat, userId, uc.constants.UI.OpenChatWindowSourceModule.Other, null, null, true);
                }
            });

            var clickLeaderTimer = -1;
            uc.ui.Contacts.MainPanel.on("click", ".content-panel .leader-list .contact-item", function () {
                clearTimeout(clickLeaderTimer);

                var userId = Number($(this).attr("userid"));

                clickLeaderTimer = setTimeout(function () {
                    uc.IUIService.showPopupContactProfile(userId);
                }, 300);
            });

            uc.ui.Contacts.MainPanel.on("dblclick", ".content-panel .leader-list .contact-item", function () {
                clearTimeout(clickLeaderTimer);

                var userId = Number($(this).attr("userid"));

                if (currentUserInfo.userId != userId) {
                    uc.IUIService.openChatWindow(uc.constants.Chat.ChatType.SingleChat, userId, uc.constants.UI.OpenChatWindowSourceModule.Other, null, null, true);
                }
            });

            uc.ui.Contacts.MainPanel.on("click", ".send-message", function () {
                var actionsBar = $(this).parents('.actions-bar');
                var userId = actionsBar.attr("userId");

                if (userId) {
                    uc.IUIService.openChatWindow(uc.constants.Chat.ChatType.SingleChat, userId, uc.constants.UI.OpenChatWindowSourceModule.Other, null, null, true);
                } else {
                    var groupId = actionsBar.attr("groupId");
                    var groupType = actionsBar.attr("groupType");
                    var chatType = uc.constants.Chat.ChatType.GroupChat;

                    if (groupType == 1) {
                        chatType = uc.constants.Chat.ChatType.ProjectChat;
                    }

                    uc.IUIService.openChatWindow(chatType, groupId, uc.constants.UI.OpenChatWindowSourceModule.Other, null, null, true);
                }
            });

            uc.ui.Contacts.MainPanel.on("click", ".send-call", function () {
                var actionsBar = $(this).closest('.actions-bar');
                var userId = actionsBar.attr("userId");

                if (userId) {
                    if (_self.currentUserStatus == 4) {
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

                    uc.IUIService.triggerCrossWindowEvent(uc.constants.UI.CrossWindowEventName.OpenChatWindow, {
                        chatType: uc.constants.Chat.ChatType.SingleChat,
                        Id: userId,
                        isFromCall: true
                    });
                } else {
                    var groupId = actionsBar.attr("groupId");
                    var groupType = actionsBar.attr("groupType");
                    var chatType = uc.constants.Chat.ChatType.GroupChat;

                    if (groupType == 1) {
                        chatType = uc.constants.Chat.ChatType.ProjectChat;
                    }

                    uc.IUIService.triggerCrossWindowEvent(uc.constants.UI.CrossWindowEventName.OpenChatWindow, {
                        chatType: chatType,
                        Id: groupId,
                        isFromCall: true
                    });
                }
            });

            uc.ui.Contacts.MainPanel.on("click", ".view-calendar figcaption", function () {
                var actionsBar = $(this).parents('.actions-bar');
                var userIdArray = [];
                var userId = actionsBar.attr("userId");
                var currentUserInfo = uc.IContactService.getCurrentUserInfo();
                var currentUserId = currentUserInfo.userId;

                if (userId && currentUserId) {
                    userIdArray.push(Number(userId));
                    userIdArray.push(Number(currentUserId));
                } else {
                    var groupId = Number(actionsBar.attr("groupId"));
                    var group = uc.modules.group.GroupManager.findCachedGroupByGroupId(groupId);

                    if (group != null) {
                        var members = group.members;

                        $.each(members, function (key, member) {
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

                uc.IUIService.createMeeting("ContactManagerMeeting", param);
            });


            //siglecall and PBX
            uc.ui.Contacts.MainPanel.on("click", ".view-call figcaption", function (e) {
                var actionsBar = $(this).parents('.actions-bar');
                var userId = actionsBar.attr("userId");
                var phoneNum;
                if (_self.getPBXVisible()) {
                    phoneNum = uc.util.StringUtil.IsPBXTelNumber(mobile);
                }
                uc.util.MenuUtil.showStartCallContextMenu(e, phoneNum, _self.getCallVisible(),function (type, data) {
                    if (type == 0) {
                        uc.IUIService.triggerCrossWindowEvent(uc.constants.UI.CrossWindowEventName.OpenChatWindow, {
                            chatType: uc.constants.Chat.ChatType.SingleChat,
                            Id: userId,
                            isFromCall: true
                        });
                    } else {
                        //PBX,data就是点击的的电话号码
                        uc.IUIEvent.OnPbxCallFromContact({"userName": userName, "userID": userId, "Tel": data});
                    }
                });
            });

            //group call
            uc.ui.Contacts.MainPanel.on("click", ".view-Groupcall figcaption", function (e) {
                var actionsBar = $(this).parents('.actions-bar');
                var groupId = actionsBar.attr("groupId");
                var groupType = actionsBar.attr("groupType");
                var chatType = uc.constants.Chat.ChatType.GroupChat;

                if (groupType == 1) {
                    chatType = uc.constants.Chat.ChatType.ProjectChat;
                }

                uc.IUIService.triggerCrossWindowEvent(uc.constants.UI.CrossWindowEventName.OpenChatWindow, {
                    chatType: chatType,
                    Id: groupId,
                    isFromCall: true
                });
            });


            uc.ui.Contacts.MainPanel.on("click", ".social-figcaption", function () {
                var currentUserInfo = uc.IContactService.getCurrentUserInfo();
                var targetId = $(this).parents('.user-detail').attr('userid');

                if (currentUserInfo.userId == targetId) {
                    var _url = uc.util.StorageUtil.getData('currentHomeUrl');

                    window.open(_url, null, '_blank');
                } else {
                    uc.IMicroblogService.getUserhomePageURL(parseInt(currentUserInfo.userId), parseInt(targetId), function (args) {
                        var _url = args[1];

                        window.open(_url, null, '_blank');
                    });
                }
            });

            uc.ui.Contacts.MainPanel.on("click", ".contact-personal-data", function () {
                $(".actions-bar .contact-move").addClass('hide');
                $(".actions-bar .contact-slip-line").show();
                $(".menu-panel-body.contacts .colleagues-wrapper").addClass('hide');
                $(".menu-panel-body.contacts .profile-detail").show();
            });

            uc.ui.Contacts.MainPanel.on("click", ".contact-relationship-data", function () {
                $(".actions-bar .contact-slip-line").hide();
                $(".actions-bar .contact-move").removeClass('hide');
                $(".menu-panel-body.contacts .profile-detail").hide();
                $(".menu-panel-body.contacts .colleagues-wrapper").removeClass('hide');
            });

            //When click on colleagues, popup contact info.
            var clickColleagueTimer = -1;
            uc.ui.Contacts.MainPanel.on("click", ".colleague-list .contact-item", function () {
                clearTimeout(clickColleagueTimer);

                var userId = Number($(this).attr("userid"));

                clickColleagueTimer = setTimeout(function () {
                    uc.IUIService.showPopupContactProfile(userId);
                }, 300);
            });

            uc.ui.Contacts.MainPanel.on("dblclick", ".colleague-list .contact-item", function () {
                clearTimeout(clickColleagueTimer);

                var userId = Number($(this).attr("userid"));

                if (currentUserInfo.userId != userId) {
                    uc.IUIService.openChatWindow(uc.constants.Chat.ChatType.SingleChat, userId, uc.constants.UI.OpenChatWindowSourceModule.Other, null, null, true);
                }
            });

            uc.ui.Contacts.MainPanel.on("click", ".social", function () {
                var currentUserInfo = uc.IContactService.getCurrentUserInfo();
                var targetId = $(this).parents('.user-detail').attr('userid');

                if (currentUserInfo.userId == targetId) {
                    var _url = uc.modules.microblog.Microblog.getPageUrl();

                    window.open(_url, null, '_blank');
                } else {
                    uc.IMicroblogService.getUserhomePageURL(parseInt(currentUserInfo.userId), parseInt(targetId), function (args) {
                        var _url = args[1];

                        window.open(_url, null, '_blank');
                    });
                }
            });

            // When right click
            uc.ui.Contacts.MainPanel.off("contextmenu");
            uc.ui.Contacts.MainPanel.on("contextmenu", ".sub-menu-panel .contact-item", function (e) {
                var userId = Number($(this).attr("userid"));

                uc.util.MenuUtil.showContactContactContextMenu(e, userId);
            });

            uc.ui.Contacts.MainPanel.on("contextmenu", ".sub-menu-panel .group-item", function (e) {
                var groupId = Number($(this).attr("groupId"));
                var groupType = $(this).attr("groupType");

                if (groupType == 'project') {
                    uc.util.MenuUtil.showContactProjectContextMenu(e, groupId);
                }

                if (groupType == 'chat') {
                    uc.util.MenuUtil.showContactGroupContextMenu(e, groupId);
                }
            });
        },
        switchPanel: function (panel, isContact) {
            var selectedUserIdList = [];
            var _self = this;
            var parentPanel = _self.getLatestClickedContactsPanel();
            var userInfo = uc.IContactService.getCurrentUserInfo();
            var departmentId, ul;

            if (isContact) {
                uc.util.StorageUtil.removeData('chooseContacts');
            } else {
                var option = uc.util.StorageUtil.getData('chooseContacts');
                selectedUserIdList = option ? (option.selectedUserIdList ? option.selectedUserIdList : []) : [];
            }

            var _self = this;
            uc.IUIService.triggerCrossWindowEvent("contactAddEmail", {});
            //1. Refresh figures panel
            var parentPanel = _self.getLatestClickedContactsPanel();

            if (panel != "panel-email") {
                parentPanel.find('.panel-item').hide();
                parentPanel.find('.' + panel).show();
                parentPanel.removeClass("contacts-panel-main");
                _self._getFigurePanelItem(panel, parentPanel);
            }

            switch (panel) {
                case "panel-contacts":
                    parentPanel.addClass("contacts-panel-main");

                    //If is Popup Select Contacts page, need refresh Current User's department info
                    //OnOrgInfoReceived Event: uc.modules.contact.OrgManager.loadMyDepartmentPanel(parentPanel, department);
                    if (parentPanel.find('.my-departments-list').length > 0) {
                        var userInfo = uc.IContactService.getCurrentUserInfo();
                        var departmentId = userInfo.departmentId;
                        uc.IContactService.getOrgInfo(departmentId);
                    }

                    uc.IContactService.getContactList(0, 1000, function (args) {
                        var errorCode = args[0];

                        if (errorCode == 0) {
                            var contacts = args[1];
                            if (selectedUserIdList && selectedUserIdList.length > 0) {
                                for (var i = 0; i < selectedUserIdList.length; ++i) {
                                    var userId = selectedUserIdList[i].userId ? selectedUserIdList[i].userId : selectedUserIdList[i];

                                    for (var j = 0; j < contacts.length; j++) {
                                        if (contacts[j].userId == userId) {
                                            contacts.splice(j, 1);
                                            break;
                                        }
                                    }
                                }
                            }

                            // 如果是通讯录，则显示状态 ----added by xue.bai_2 on 2016/05/04--------------------
                            if (isContact) {
                                var userIds = [];
                                var copyBack = '{"panel": "panel-contacts", "panelUl": "contact-list"}';

                                for (var m = 0; m < contacts.length; m++) {
                                    userIds.push(contacts[m].userId);
                                }

                                uc.util.LogUtil.info('ContactManager', 'switchPanel', 'getUserStatus', {
                                    userIds: userIds,
                                    copyBack: copyBack
                                });
                                uc.IContactService.getUserStatus(userIds, copyBack, function (args) {
                                    uc.util.LogUtil.info('ContactManager', 'getUserStatus', 'response', {args: args});
                                });

                                // step1:存入常用联系人Cahce中
                                uc.modules.contact.ContactManager.cachedContactsStatus.addContacts("panel-contacts", userIds);
                            }
                            // -------------------------added by xue.bai_2 on 2016/05/04--------------------end

                            var panelContacts = parentPanel.find('.panel-contacts');
                            if (panelContacts.attr("loaded")) {
                                return;
                            }

                            panelContacts.attr("loaded", true);
                            var ul = panelContacts.find('.contact-list');
                            _self.loadFrequentContactsWithKeysPanel(ul, contacts, isContact);
                        }
                    });
                    break;
                case "panel-projects":
                    ul = parentPanel.find('.project-list');

                    _self.showLoading(ul);
                    uc.IGroupService.getGroupList(1, 0, 1000, 0, false);
                    break;
                case "panel-groupChats":
                    ul = parentPanel.find('.group-chat-list');

                    _self.showLoading(ul);
                    uc.IGroupService.getGroupList(4, 0, 1000, 0, false);
                    break;
                case "panel-my-department":
                    ul = parentPanel.find('.my-department-list');
                    departmentId = userInfo.departmentId;

                    _self.showLoading(ul);
                    uc.IContactService.getOrgInfo(departmentId);
                    break;
                case "panel-organization":
                    parentPanel.find('.org-parent').empty();

                    uc.modules.org.OrgManager.init(selectedUserIdList, isContact);
                    break;
                case "panel-email":
                    var options = {
                        title: '',
                        width: 400,
                        height: 204,
                        refreshIfExist: true,
                        centerScreen: true,
                        modal: true,
                        resizable: false,
                        minWidth: 400,
                        minHeight: 204,
                        caption: uc.constants.UI.DragCaption.ChooseContacts
                    };

                    uc.util.WindowUtil.open('page/contactAddEmail.html', 'contactAddEmail', options);
                    break;
            }
            this.onPanelChange(panel);
        },
        onPanelChange: function (panel) {
            if (!isMainWindow) return;

            var parentPanel = this.getLatestClickedContactsPanel();

            parentPanel.find('.contact-selected').removeClass('contact-selected');
            uc.ui.Contacts.ContentPanelBody.html(this.getInitPageHtml(panel));
        },
        getInitPageHtml: function (panel) {
            var map = {
                "panel-contacts": 'User',
                "panel-projects": 'Project',
                "panel-groupChats": 'Group',
                "panel-my-department": 'User',
                "panel-organization": 'User'
            };
            return ('<div class="init-panel">\
                        <div class="init-panel-none">\
                            <img src="' + rootPath + '/images/contacts/none' + (map[panel] || 'User') + '.png">\
                            <div class="init-panel-noneText">' + uc_resource.Main['ContactsMessage' + (map[panel] || 'User')] + '</div>\
                        </div>\
                    </div>');
        },
        showLoading: function (ul) {
            ul.empty();
            ul.append('<div class="in-loading">' + uc_resource.Profile.Loading + '</div>');
        },
        showContactList: function () {

        },
        hideContactList: function () {

        },
        bindUserDetailEvents: function (contentPanel) {
            var _self = this;
            contentPanel.off("click");
            contentPanel.off("blur");
            contentPanel.off("keydown");
            contentPanel.on("click", ".attentioned", function () {
                //Cancel Attentin
                _self.canelAttention();
            });
            contentPanel.on("click", ".add-attention", function () {
                //Add Attention
                _self.addAttention();
            });
            contentPanel.on("click", ".edit-user-signature", function () {
                var signatureView = contentPanel.find('.user-signature-view');
                var signature = signatureView.find('.user-signature').text();
                signatureView.hide();

                var signatureEdit = contentPanel.find('.user-signature-edit');
                var signatureInput = signatureEdit.find('.user-signature-input');
                signatureInput.val(signature);
                signatureEdit.show().css("display", "table-cell");
                signatureInput.focus();
            });
            contentPanel.on("blur", ".user-signature-input", function () {
                _self.editUserSignature(contentPanel);
            });
            contentPanel.on("keydown", ".user-signature-input", function (e) {
                var key = e.which;
                if (key == 13) {
                    e.preventDefault();
                    _self.editUserSignature(contentPanel);
                }
            });
            contentPanel.on("click", ".contact-avatar-edit", function () {

                uc.IUIService.openImageCropperWindow("ChangeContactAvatar", function (args) {
                    if (args.okOrCancel) {
                        var croppedImagePath = args.croppedImagePath;

                        var userDetail = $('.user-detail');
                        var userId = Number(userDetail.attr("userId"));
                        _self.uploadContactAvatar(userId, croppedImagePath);
                    }
                });
            });

        },
        editUserSignature: function (contentPanel) {

            var profileDetail = contentPanel.find('.profile-detail');
            var userId = profileDetail.attr("userId");

            var signatureView = profileDetail.find('.user-signature-view');
            var signatureEdit = profileDetail.find('.user-signature-edit');
            var userSignatureElement = signatureView.find('.user-signature');
            var signatureBefore = signatureView.find('.user-signature').text();
            var signatureNew = profileDetail.find('.user-signature-input').val();

            uc.modules.contact.ContactManager.setUserSignature(userSignatureElement, signatureNew);

            signatureEdit.hide();
            signatureView.show();

            //Call backend to save signature.
            if (signatureNew != signatureBefore) {
                //1. Save to backend
                var signature = {
                    "personal_sign": signatureNew.replace(uc.util.StringUtil.spaceReg, String.fromCharCode(160))
                };
                //Need change json to String
                uc.IContactService.updateUserInfo(JSON.stringify(signature));

                //2. Will receive OnCurrentUserInfoUpdated Event and update UI & Cache in uc.modules.contact.ContactManager.noticeContactUpdated

            }
        },
        getContactKey: function (taskId) {
            return "contact_" + taskId;
        },
        getFrequentContactKey: function (taskId) {
            return "frequentContact_" + taskId;
        },
        uploadContactAvatar: function (userId, filePath) {
            var _self = this;
            uc.IFSService.uploadFile(filePath, function (args) {
                var errorCode = args[0], taskId = args[1];
                if (errorCode === 0) {
                    uploadTaskMap.put(_self.getContactKey(taskId),
                        {
                            userId: userId,
                            localPath: filePath
                        });
                    _self.refreshContactAvatar(userId, filePath);
                }
            });
        },
        refreshContactAvatar: function (userId, localAvatarUrl) {
            var _self = this;
            //Update Contact list Avatar UI
            var contactAvatars = $('.avatar-' + userId);
            if (contactAvatars.length > 0) {
                for (var i = 0; i < contactAvatars.length; i++) {
                    var avatar = $(contactAvatars[i]);
                    avatar.attr("src", localAvatarUrl);
                }
            }
            //Update localAvatarUrl to Cached Contact
            var cachedContact = _self.getCachedContact(userId);
            if (cachedContact && localAvatarUrl != "" && localAvatarUrl != cachedContact.localAvatar) {
                cachedContact.localAvatar = localAvatarUrl;
                _self.addOrUpdateContactToCache(userId, cachedContact);
            }
        },
        noticeContactAvatarUploaded: function (taskId, remoteImageUrl) {
            var _self = this;
            var key = _self.getContactKey(taskId);
            var temp = uploadTaskMap.get(key);

            if (temp == null) {
                return;
            }

            var userId = temp.userId; //, localPath = temp.filePath;
            var cachedContact = _self.getCachedContact(userId);

            uploadTaskMap.remove(key);

            //Call interface to set Contact remote avatar
            //uc.IContactService.setContactLocalAvatarUrl(userId, remoteImageUrl, localPath);   
            var avatar = {
                "avatar": remoteImageUrl
            };

            //Need change json to String
            uc.IContactService.updateUserInfo(JSON.stringify(avatar));

            //Update localAvatarUrl to Cached Contact
            if (cachedContact && remoteImageUrl != "" && remoteImageUrl != cachedContact.remoteAvatarUrl) {
                cachedContact.remoteAvatarUrl = remoteImageUrl;
                _self.addOrUpdateContactToCache(userId, cachedContact);
            }
        },
        selectedContactJudge: function (contact) {
            var selectedContactElement = $('.contact-selected');
            if (selectedContactElement.is(":visible") && contact.userId == Number(selectedContactElement.attr("userid"))) {
                uc.modules.contact.ContactManager.loadContactDetail(contact);
            }
        },
        loadContactDetail: function (originalContact) {
            if (typeof(uc.ui.Contacts) == "undefined") {
                return;
            }

            var contentPanel = uc.ui.Contacts.ContentPanelBody;
            contentPanel.empty();
            contentPanel.hide();

            //Check whether is self
            var currentUser = uc.IContactService.getCurrentUserInfo();
            var currentUserId = currentUser.userId;
            var isSelf = currentUserId == originalContact.userId;
            var _self = this;
            var contact = originalContact;

            _self.addOrUpdateContactToCache(originalContact.userId, originalContact);

            var template = $('<div class="user-detail text-selectable"></div>');
            if (isSelf) {
                template.addClass("self");
            }
            template.attr("userId", originalContact.userId);

            var userDetailUp = $('<div class="user-detail-up"></div>');
            var userDetailDown = $('<div class="user-detail-down"></div>');
            template.append(userDetailUp);
            template.append(userDetailDown);

            var userDetailDownWrapper = $('<div class="user-detail-down-wrapper"></div>');
            userDetailDown.append(userDetailDownWrapper);

            var defaultAvatar = uc.constants.UI.DefaultLocalUrl.Avatar;

            var profileWrapper = $('<div class="profile-wrapper">'
                + '<div class="left-content left-none">'
                + '<div class="profile-display-name"><span class="person-name"></span></div>'
                + '<div class="user-status"><div></div><span></span></div>'
                + '<div class="profile-info">'
                + '<div class="profile-position"></div>'
                + '<div class="profile-department"></div>'
                + '</div>'
                + '</div>'
                + '<div class="profile-avatar">'
                + '    <img src="' + defaultAvatar + '" onerror="uc.util.ImageUtil.OnImageLoadError(this,\'' + defaultAvatar + '\')" class="avatar round-image left-position">'
                + '</div>'
                + '</div>');
            profileWrapper.find('.person-name').text(contact.displayName);
            profileWrapper.find('.profile-position').text(contact.position);
            profileWrapper.find('.profile-department').text(contact.departmentName);
            profileWrapper.find('.avatar').addClass("avatar-" + originalContact.userId);

            if (contact.localAvatar) {
                profileWrapper.find('.profile-avatar').find('img').attr("src", contact.localAvatar);
            } else if (contact.remoteAvatarUrl) {

            }

            if (isSelf) {
                var contactAvatarEdit = $('<div class="rounded-corners contact-avatar-edit contact-edit"></div>');

                profileWrapper.append(contactAvatarEdit);
            } else {
                var displayNameElement = profileWrapper.find('.profile-display-name');
                var attentionElement = $('<div class="attention"></div>');

                displayNameElement.append(attentionElement);

                uc.IMicroblogService.getRelation(originalContact.userId, function (args) {
                    var errorCode = args[0];

                    if (errorCode == 0) {
                        var attentionStatus = args[1];

                        _self.onAttentionChanged(originalContact.userId, attentionStatus);
                    }
                });
            }

            userDetailUp.append(profileWrapper);

            var actionsBar = $('<div class="actions-bar"></div>');
            actionsBar.attr("userId", contact.userId);
            userDetailUp.append(actionsBar);

            var socialFigure = $('<figure class="field hide">'
                + '<figcaption class="social-figcaption"></figcaption>'
                + '</figure>');
            actionsBar.append(socialFigure);

            var sendMessageFigure = $('<div class="send-content"><button class="send-message" >' + uc_resource.Profile.SendMessage + '</button></div>');
            actionsBar.append(sendMessageFigure);

            var callFigure = $('<figure class="view-call">'
                + '<figcaption class="view-call-figcaption" qtips="' + uc_resource.Menu.Call + '"></figcaption>'
                + '</figure>');
            actionsBar.append(callFigure);

            var calendarFigure = $('<figure class="view-calendar">'
                + '<figcaption class="view-calendar-figcaption"></figcaption>'
                + '</figure>');
            actionsBar.append(calendarFigure);

            var middleItem = $('<div class="contact-personal-data">' + uc_resource.Profile.PersonalData + '</div>'
                + '<div class="contact-relationship-data">' + uc_resource.Profile.Relationship + '</div>'
                + '<div class="contact-splitter"></div>'
                + '<div class="contact-slip-line"></div>'
                + '<div class="contact-move hide"></div>'
            );
            actionsBar.append(middleItem);
            actionsBar.find(".view-calendar-figcaption").attr({"qtips": uc_resource.Meeting.CreateCalendar});
            var upLoadMask = $('<div class="uploadMask"></div>');
            userDetailDownWrapper.append(upLoadMask);

            var detail = $('<div class="profile-detail">'
                + '<div class="field user-signature-field">'
                + '<div class="label">' + uc_resource.Profile.Signature + '</div>'
                + '<div class="value user-signature-view"><label class="user-signature text-ellipsis"></label><span class="edit-user-signature"></span></div>'
                + '<div class="value user-signature-edit"><input maxlength="255" type="input" class="user-signature-input"/></div>'
                + '</div>'
                + '</div>');
            userDetailDownWrapper.append(detail);

            detail.attr("userId", contact.userId);
            detail.find('.user-signature').addClass("user-signature-" + contact.userId);
            detail.find('.user-signature-input').addClass("user-signature-input-" + contact.userId);

            _self.setUserFieldValue(detail.find('.user-signature'), contact.personalSign, isSelf);
            _self.setUserSignature(detail.find('.user-signature'), contact.personalSign);

            mobile = contact.mobile;
            userName = contact.displayName;

            var colleagues = $('<div class="colleagues-wrapper hide" >'
                + '<div class="field leaders-field">'
                + '<div class="new-label">' + uc_resource.Profile.Leader + '</div>'
                + '<div class="value">'
                + '</div>'
                + '</div>'
                + '<ul class="leader-list">'
                + '</ul>'
                + '<div class="field colleagues-field">'
                + '<div class="new-label">' + uc_resource.Profile.Colleagues + '</div>'
                + '<div class="value">'
                + '</div>'
                + '</div>'
                + '<ul class="colleague-list">'
                + '</ul>'
                + '</div>');
            //detail.append(colleagues);
            userDetailDownWrapper.append(colleagues);

            contentPanel.append(template);
            contentPanel.fadeIn('slow');


            var userTags = contact.userTags;
            if (!userTags) {
                return;
            }

            var tags = eval('(' + userTags + ')');

            //先对tags按照order字段进行升序排序
            tags.sort(function (x, y) {
                return Number(x.tag_order) > Number(y.tag_order) ? 1 : -1;
            });

            //判断是否有code需要做本地化
            for (var k = 0; k < tags.length; k++) {
                if ((tags[k].tag_flag == 0) || (tags[k].tag_value == "")) {
                    continue;
                }

                var arr = tags[k].tag_code;

                if (arr) {
                    switch (arr) {
                        case "tag_u_display_name":
                            var tag_name = uc_resource.Profile.Personal[uc.constants.Profile.Me.DisplayNameIndex].tag_u_display_name;
                            break;
                        case "tag_u_mobile":
                            var tag_name = uc_resource.Profile.Personal[uc.constants.Profile.Me.PhoneIndex].tag_u_mobile;
                            break;
                        case "tag_u_email":
                            var tag_name = uc_resource.Profile.Personal[uc.constants.Profile.Me.EmailIndex].tag_u_email;
                            break;
                        default :
                            var tag_name = tags[k].tag_name;
                            break;
                    }
                } else {
                    var tag_name = tags[k].tag_name;
                }

                var tag_value = tags[k].tag_value;
                var tag_id = tags[k].tag_id;
                var tag_length = tags[k].tag_value_length;
                var existed_tag = detail.find(".edit-profile-" + tag_id);

                if (existed_tag.length) {
                    existed_tag.parents(".field").remove();
                }

                detail.find(".label").each(function () {
                    var self = $(this);

                    if (self.html() == tag_name) {
                        self.parent().remove();
                    }
                });

                var tagsShow = $('<div class="field">'
                    + '<div class="label">' + tag_name + '</div>'
                    + '<div class="value"><span class="text-ellipsis edit-value">' + tag_value + '</span>'
                    + '</div>'
                    + '</div>');
                detail.append(tagsShow);

                if ((tags[k].is_edit == 1) && isSelf) {
                    var editor = $('<span class="edit-profile"></span>');

                    editor.attr("tagName", tag_name);
                    editor.attr("tagId", tag_id);
                    editor.attr("tagValue", tag_value);
                    editor.attr("tagLength", tag_length);
                    editor.addClass("edit-profile-" + tag_id);
                    tagsShow.find(".value").append(editor);
                }

                $(".edit-profile").InputBox({callback: this.updateCustomTags});
            }


            // added by xue.bai_2 on 2016/05/10 to fix bug: 第一次点击contact-item时，详情页面状态不显示。
            if (uc.constants.Switch.OpenUserStatus) {
                if (!isSelf) {
                    uc.IContactService.subscribeUserStatus([originalContact.userId]);
                }
            }

            uc.IContactService.getColleagues(contact.userId);
            if (isSelf) {
                $(".send-message").addClass("hide");
                $(".send-call").addClass("hide");
                $(".view-calendar-figcaption").addClass("hide");
                $(".view-call-figcaption").addClass("hide");
                $(".user-detail-down").addClass("self-detail-down self-up");
            }

            // add by xiege.
            template.hasClass('self') ? template.find('.user-status').remove() : null;
            if (uc.constants.Switch.OpenMultiVersion) {
                var version = JSON.parse(uc.util.StorageUtil.getData("capability"));
                if (version) {
                    switch (version.im) {
                        case "0":
                            $(".send-message").addClass("hide");
                            break;
                    }
                    switch (version.app_center) {
                        case "0":
                            socialFigure.addClass("hide");
                            break;
                    }

                    switch (version.app_moments) {
                        case "0":
                            uc.ui.Contacts.ContentPanelBody.find(".attention").addClass("hide");
                            break;
                    }
                }

	            if (!isSelf) {
		            if(!this.getCalendarVisible()){
			            $(".view-calendar-figcaption").addClass("hide");
		            }
		            //判断呼或者pbx的权限，更改通话按钮显示状态
		            if(this.getCallVisible()) {
			            $(".view-call-figcaption").removeClass("hide");
		            }else{
			            if(!this.getPBXVisible()){
				            $(".view-call-figcaption").addClass("hide");
			            }
		            }
	            }
            }


            return template;

        },
        setUserFieldValue: function (element, value, isSelf) {
            var _self = this;

            element.text(value);

            //If user attribute is empty, and not current user, then hide this attribute
            if (_self.isBlank(value) && !isSelf) {
                element.parents('.field').hide();
            } else {
                element.parents('.field').show();
            }
        },
        isBlank: function (pString) {
            if (!pString || pString.length == 0) {
                return true;
            }

            return !/[^\s]+/.test(pString);
        },
        setUserSignature: function (element, value) {
            var text = uc.util.StringUtil.escapeHtml(value, true);

            element.html(text);
            element.attr("qtips", text);
        },
        addAttention: function () {
            var userDetail = $('.user-detail');

            if (userDetail.length == 0) {
                return;
            }

            var userId = Number(userDetail.attr("userId"));

            uc.IMicroblogService.addAttention(userId);
        },
        canelAttention: function () {
            var userDetail = $('.user-detail');

            if (userDetail.length == 0) {
                return;
            }

            var userId = Number(userDetail.attr("userId"));

            uc.IMicroblogService.cancelAttention(userId);
        },
        onAttentionChanged: function (userId, attentionStatus) {
            var userDetail = $('.user-detail');
            if (userDetail.length == 0) {
                return;
            }

            var currentPageUserId = Number(userDetail.attr("userId"));
            if (userId != currentPageUserId) {
                return;
            }

            var attentionElement = userDetail.find('.attention');
            attentionElement.removeClass("add-attention");
            attentionElement.removeClass("attentioned");
            attentionElement.removeClass("both-attentioned");
            attentionElement.empty();

            switch (attentionStatus) {
                case 2:
                case 4:
                    attentionElement.append('<img src="../images/contacts/add_attention.png" class="add-attention-icon"/><span class="attention-status">' + uc_resource.Profile.AddAttention + '</span>');
                    attentionElement.addClass("add-attention");
                    break;
                case 1:
                    attentionElement.append('<span class="attention-status">' + uc_resource.Profile.Attentioned + '</span>');
                    attentionElement.addClass("attentioned");
                    break;
                case 3:
                    attentionElement.append('<img src="../images/contacts/both_attentioned.png" class="both-attentioned-icon"/><span class="attention-status">' + uc_resource.Profile.CommonAttention + '</span>');
                    attentionElement.addClass("attentioned both-attentioned");
                    break;
            }
        },
        loadDepartmentMembersPanel: function (department) {
            var _self = this;
            var contacts = department.staffs;
            var ul = $('.panel-my-department .my-department-list');

            //For Organization Staff, need fetch localAvatar, remoteAvatar, do not use staff.localAvatar, remoteAvatar, they're empty.
            uc.modules.contact.ContactManager.addOrUpdateContactsToCache(contacts);

            _self.loadContactsPanel(ul, contacts);

            // start------------------------added by xue.bai_2 on 2016/05/11----------------------------
            var userIds = [];
            var copyBack = '{"panel": "panel-my-department", "panelUl": "my-department-list"}';

            for (var i = 0; i < contacts.length; i++) {
                userIds.push(contacts[i].userId);
            }

            uc.IContactService.getUserStatus(userIds, copyBack, function () {});

            uc.modules.contact.ContactManager.cachedContactsStatus.addContacts("panel-my-department", userIds);
            // ------------------------added by xue.bai_2 on 2016/05/11-------------------------------end

        },
        _getContactKeyTemplate: function () {
            var template = $('<li class="alphabetical-key-item">'
                + '<div class="fixed sort-key"></div>'
                + '<div class="fluid"><div class="line"></div></div>'
                + '</li>');
            return template;
        },
        loadFrequentContactsWithKeysPanel: function (ul, contacts, isContact) {
            var _self = this;

            var parentPanel = uc.modules.contact.ContactManager.getLatestClickedContactsPanel();
            if (parentPanel == null) {
                return;
            }

            var panelContacts = parentPanel.find('.panel-contacts');
            if (panelContacts.attr("loaded") == true) {
                return;
            }

            panelContacts.attr("loaded", true);

            ul.empty();

            //Sorted array, #(ZZ) last,
            var alphabeticalSortContacts = _self.getAlphabeticalSortArray(contacts, "pinyin", "displayName");

            //Cache Frequent Contacts for popup select Contacts
            uc.modules.chooseContacts.ChooseContactsManager.setCachedFrequentContacts(alphabeticalSortContacts);

            _self.loadMoreFrequentContacts(panelContacts, alphabeticalSortContacts);

            panelContacts.scroll(function () {
                _self.scrollOnFrequentContacts($(this), alphabeticalSortContacts, isContact);
            });
        },
        scrollOnFrequentContacts: function (panel, contacts, isContact) {
            var _self = this;
            var wayPoint = panel.find(".down-scroll-way-point");

            if (wayPoint.length == 0) {
                return;
            }

            if (_self.isBottom(wayPoint)) {
                _self.loadMoreFrequentContacts(panel, contacts, isContact);
            }
        },
        isBottom: function (wayPoint) {
            var end = wayPoint.offset().top;
            var viewEnd = $(window).scrollTop() + $(window).height();
            var distance = end - viewEnd;

            return distance < 0;
        },
        loadMoreFrequentContacts: function (parentPanel, contacts, isContact) {
            var _self = this;
            var ul = parentPanel.find('.contact-list');
            var wayPoint = parentPanel.find('.down-scroll-way-point');
            var moreItems = [];
            var currentItem = null;

            if (wayPoint.length == 0) {
                moreItems = _self.loadMoreItems(contacts, _self.batchCount, currentItem);
            } else {
                var lastContactItem = ul.find('.contact-item').last();
                var lastContactId = lastContactItem.attr("userId");

                currentItem = uc.util.CollectionUtil.findObjectByKey(contacts, "userId", Number(lastContactId));
                moreItems = _self.loadMoreItems(contacts, _self.batchCount, currentItem);
            }

            _self.loadMoreFrequentContactsUI(ul, moreItems);

            // 如果是通讯录，则显示状态 start----------- added by xue.bai_2 on 2016/05/04---------------------------------
            if (isContact) {
                var userIds = [];
                var copyBack = '{"panel": "panel-contacts", "panelUl": "contact-list"}';

                for (var m = 0; m < moreItems.length; m++) {
                    userIds.push(moreItems[m].userId);
                }

                uc.util.LogUtil.info('ContactManager', 'loadMoreFrequentContacts', 'getUserStatus', {
                    userIds: userIds,
                    copyBack: copyBack
                });
                uc.IContactService.getUserStatus(userIds, copyBack, function (args) {
                    uc.util.LogUtil.info('ContactManager', 'getUserStatus', 'response', {args: args});
                });
            }
            // ------------------------------------ added by xue.bai_2 on 2016/05/04 ------------------------------end


            //If current loaded data cannot fill full of this panel, load more data
            wayPoint = parentPanel.find(".down-scroll-way-point");
            if (wayPoint.length == 1) {
                if (_self.isBottom(wayPoint)) {
                    _self.loadMoreFrequentContacts(parentPanel, contacts, isContact);
                }
            }
        },
        loadContactStatusUI: function (status, copyBack) { // added by xue.bai_2 on 2016/05/04
            // 判断是否是通讯录模块返回的数据
            var copyBackObj = $.parseJSON(copyBack);
            var panel = copyBackObj.panel ? copyBackObj.panel : '';
            var panelUl = copyBackObj.panelUl ? copyBackObj.panelUl : '';
            if (!panel || !panelUl) {
                return;
            }

            // 判断通讯录是否可见
            var parentPanel = uc.modules.contact.ContactManager.getLatestClickedContactsPanel();
            if (parentPanel == null) {
                return;
            }

            // 获得通讯录模块当前选中的类别
            var selectedFigure = parentPanel.find('.figure-selected');
            var selectedPanel = selectedFigure.attr('panel');
            if (selectedFigure.length == 0) {
                selectedPanel = "panel-contacts";
            }

            // 如果返回的数据不是当前选中的类别
            if (panel != selectedPanel) {
                return;
            }

            // 渲染状态UI
            if (status.length > 0) {
                var panelContacts = parentPanel.find('.' + panel);
                var ul = panelContacts.find('.' + panelUl);

                $.each(status, function (index, item) {
                    var userId = item.userId;
                    var itemStatus = item.status;
                    var userStatus = ul.find('.contact-item-' + userId).find(".user-list-status");
                    var backgroundColor = '';

                    if (panel == "panel-organization") {
                        userStatus = panelContacts.find('.contact-item-' + userId).find(".user-list-status");
                    }

                    switch (itemStatus) {
                        case uc.constants.UserStatusType.Online:
                            backgroundColor = '#4cd964';
                            break;
                        case uc.constants.UserStatusType.Busy:
                            backgroundColor = '#f96868';
                            break;
                        case uc.constants.UserStatusType.Away:
                            backgroundColor = '#ffcc00';
                            break;
                        case uc.constants.UserStatusType.InMeeting:
                            backgroundColor = '#f96868';
                            break;
                        case uc.constants.UserStatusType.Offline:
                            backgroundColor = '#666666';
                            break;
                        case uc.constants.UserStatusType.InCall:
                            backgroundColor = '#f96868';
                            break;
                        default:
                            break;
                    }

                    userStatus.css("background-color", backgroundColor).removeClass("hide");
                });
            }
        },
        loadContactListStatusUI: function (errorCode, status) { // added by xue.bai_2 on 2016/05/23
            // 判断通讯录是否可见
            var parentPanel = uc.modules.contact.ContactManager.getLatestClickedContactsPanel();
            if (parentPanel == null) {
                return;
            }

            // 获得通讯录模块当前选中的类别
            var selectedFigure = parentPanel.find('.figure-selected');
            var selectedPanel = selectedFigure.attr('panel');
            if (selectedFigure.length == 0) {
                selectedPanel = "panel-contacts";
            }

            // 渲染状态UI
            if (status.length > 0) {
                var panelContacts = parentPanel.find('.' + selectedPanel);

                $.each(status, function (index, item) {
                    var userId = item.userId;
                    var itemStatus = item.status;
                    var userStatus = panelContacts.find('.contact-item-' + userId).find(".user-list-status");
                    var backgroundColor = '';

                    if (selectedPanel == "panel-organization") {
                        userStatus = panelContacts.find('.contact-item-' + userId).find(".user-list-status");
                    }

                    switch (itemStatus) {
                        case uc.constants.UserStatusType.Online:
                            backgroundColor = '#4cd964';
                            break;
                        case uc.constants.UserStatusType.Busy:
                            backgroundColor = '#f96868';
                            break;
                        case uc.constants.UserStatusType.Away:
                            backgroundColor = '#ffcc00';
                            break;
                        case uc.constants.UserStatusType.InMeeting:
                            backgroundColor = '#f96868';
                            break;
                        case uc.constants.UserStatusType.Offline:
                            backgroundColor = '#666666';
                            break;
                        case uc.constants.UserStatusType.InCall:
                            backgroundColor = '#f96868';
                            break;
                        default:
                            break;
                    }

                    userStatus.css("background-color", backgroundColor).removeClass("hide");
                });
            }
        },
        loadMoreFrequentContactsUI: function (ul, moreItems) {
            var _self = this;
            //[{key, [contacts]}, {key, [contacts]}], json format
            var alphabeticalSortContacts = _self.alphabeticalSortItems(moreItems, "pinyin", "displayName");

            $.each(alphabeticalSortContacts, function (key, sortedContacts) {
                _self.displayContactKey(ul, key, null);

                $.each(sortedContacts, function (k, contact) {
                    _self.addOrUpdateContactToCache(contact.userId, contact);

                    var item = _self.getCachedContact(contact.userId);

                    _self.loadContactItemUI(item, ul, null);
                });
            });

            var parentPanel = ul.parent('.panel-item');
            var wayPoint = parentPanel.find('.down-scroll-way-point');
            if (wayPoint.length == 0) {
                wayPoint = _self.getDownScrollWayPointTemplate();
                parentPanel.append(wayPoint);
            }

            if (moreItems.length < _self.batchCount) {
                parentPanel.unbind();
                wayPoint.remove();
            }
        },
        loadContactsPanel: function (ul, contacts) {
            ul.empty();

            var _self = this;
            var memberIdArray = [];

            $.each(contacts, function (k, contact) {
                var item = _self.getCachedContact(contact.userId);

                _self.loadContactItemUI(item, ul, null);
                memberIdArray.push(contact.userId);
            });

            uc.modules.contact.ContactManager.batchRequestMembers(memberIdArray);
        },
        loadContactItemUI: function (contact, ul, lastContactElement, currentUserInfo) {
            var item = contact;
            var _self = this;
            var template = _self.displayContactItemUI(item, currentUserInfo);

            if (ul.find(".contact-item-" + contact.userId).length > 0) {
                _self.noticeUserAvatarDownloaded(contact.userId, contact.remoteAvatarUrl, contact.localAvatar);
                return;
            }

            if (lastContactElement != null) {
                template.insertAfter(lastContactElement);
            } else {
                ul.append(template);
            }
        },

        displayContactItemUI: function (item, currentUserInfo) {
            var _self = this;
            var option = uc.util.StorageUtil.getData('chooseContacts');
            var template = _self._getContactTemplate();

            template.addClass("contact-item-" + item.userId);
            template.attr("userId", item.userId);
            template.find('.checkbox-parent').addClass("check-box-contact-" + item.userId);
            template.find('.contact-display-name').text(item.displayName);
            template.find('.contact-display-name').attr("userId", item.userId);
            template.find('.contact-department').text(item.departmentName);
            template.find('.contact-position').text(item.position);

            var avatar = template.find('.left .avatar');
            avatar.addClass("avatar-" + item.userId);
            if (currentUserInfo && item.userId == currentUserInfo.userId && option.key != uc.constants.Meeting.CreateMeetingName && !option.isMeetingForward) {
                template.find('[type="checkbox"]').attr('disabled', 'disabled');
            }

            if (item.localAvatar) {
                avatar.attr("src", item.localAvatar);
                avatar.addClass("avatar-loaded");
            } else {
                avatar.addClass("avatar-unloaded");

                if (item.remoteAvatarUrl) {
                }
            }
            return template;
        },
        batchRequestMembers: function (memberIdArray) {
            //When get Contacts batchly, will receive and process contacts in OnContactInfoReceived Event.
            uc.IContactService.getBasicContactInfo(memberIdArray);
        },
        noticeUserAvatarDownloaded: function (userId, serverAvatarUrl, localAvatarUrl) {
            var _self = this;

            if (localAvatarUrl == "") {
                return;
            }

            var contact = uc.modules.contact.ContactManager.getCachedContact(userId);

            if (contact) {
                contact.localAvatar = localAvatarUrl;
                uc.modules.contact.ContactManager.addOrUpdateContactToCache(userId, contact);
            } else {
                contact = uc.IContactService.getBasicContactInfo(userId);

                if (contact) {
                    contact.localAvatar = localAvatarUrl;
                    uc.modules.contact.ContactManager.addOrUpdateContactToCache(userId, contact);
                }
            }

            var key = _self.getContactDownloadingKey(userId, serverAvatarUrl);

            if (contactDownloadingTaskMap[key]) {
                contactDownloadingTaskMap.remove(key);
            }

            _self.changeUserAvatarUI(userId, localAvatarUrl);
        },
        changeUserAvatarUI: function (userId, localAvatarUrl) {
            var avatars = $('.avatar-' + userId);

            $.each(avatars, function (k, avatar) {
                $(avatar).attr("src", localAvatarUrl);
                $(avatar).removeClass("avatar-unloaded").addClass("avatar-loaded");
            });
        },
        getDownScrollWayPointTemplate: function () {
            var wayPoint = $('<div class="down-scroll-way-point">'
                + '	<img src="../images/contacts/loading.gif"/>'
                + '</div>');
            return wayPoint;
        },
        loadMoreItems: function (allSortedItems, batchCount, currentItem) {
            var currentIndex = -1;

            //Find currentIndex by currentItem, if currentIndex = -1, no exist
            if (currentItem != null) {
                currentIndex = $.inArray(currentItem, allSortedItems);
            }

            var totalCount = allSortedItems.length;
            var wantBeginIndex = currentIndex + 1;
            var loadBeginIndex = wantBeginIndex > totalCount ? totalCount : wantBeginIndex;
            var wantEndIndex = loadBeginIndex + batchCount;
            var loadEndIndex = wantEndIndex > totalCount ? totalCount : wantEndIndex;
            var moreItems = [];

            for (var i = loadBeginIndex; i < loadEndIndex; i++) {
                moreItems.push(allSortedItems[i]);
            }
            return moreItems;
        },
        sortItemsWithPoundKeyLast: function (items, key) {
            var poundKeyItems = items.filter(function (item) {
                return item[key] === "#";
            });

            var otherItems = items.filter(function (item) {
                return item[key] !== "#";
            });

            return $.merge(otherItems, poundKeyItems);
        },
        getAlphabeticalSortArray: function (array, firstAttribute, secondAttribute) {
            //Group: firstAttribute: pinyin, secondAttribute:name
            //Contact: firstAttribute: namePinYin, secondAttribute: displayName
            var _self = this;

            $.each(array, function (k, item) {
                if (item.hasOwnProperty(firstAttribute) && item[firstAttribute].length > 0) {
                    item.sortKey = item[firstAttribute].substr(0, 1);
                } else {
                    item.sortKey = "#";
                    item[firstAttribute] = "#";
                }
            });

            //1, sort by first word, and PinYin properties
            var sortedItems = array.sort(uc.util.CollectionUtil.dynamicSortMultiple("sortKey", firstAttribute, secondAttribute));

            return _self.sortItemsWithPoundKeyLast(sortedItems, "sortKey");
        },
        alphabeticalSortItems: function (groups, firstAttribute, secondAttribute) {
            //Group: firstAttribute: pinyin, secondAttribute:name
            //Contact: firstAttribute: namePinYin, secondAttribute: displayName
            var _self = this;
            var alphabeticalGroups = {};

            $.each(groups, function (k, group) {
                if (group.hasOwnProperty(firstAttribute) && group[firstAttribute].length > 0) {
                    group.sortKey = group[firstAttribute].substr(0, 1);
                } else {
                    group.sortKey = "#";
                    group[firstAttribute] = "#";
                }
                group.sortKey = group.sortKey == "#" ? _self.poundKeyReplacer : group.sortKey;
            });

            //1, sort by first word, and PinYin properties
            var sortedGroups = groups.sort(uc.util.CollectionUtil.dynamicSortMultiple("sortKey", firstAttribute, secondAttribute));

            //2, group by first Word
            $.each(sortedGroups, function (k, group) {
                var key = group.sortKey.toUpperCase();

                if (typeof(alphabeticalGroups[key]) == "undefined") {
                    alphabeticalGroups[key] = [];
                }
                alphabeticalGroups[key].push(group);
            });

            //3. Sort grouped data by key
            var alphabeticalSortGroups = uc.util.CollectionUtil.keySort(alphabeticalGroups);

            /*//4. Move # to last
             var poundGroups = [];
             if(alphabeticalSortGroups.hasOwnProperty("#")){
             poundGroups = alphabeticalSortGroups["#"];
             delete alphabeticalSortGroups["#"];
             alphabeticalSortGroups["#"] = poundGroups;
             }*/

            return alphabeticalSortGroups;
        },
        loadColleagues: function (userId, colleagues, leaders) {
            var _self = this;
            var profileDetail = $('.contacts .profile-detail');
            var currentUserId = profileDetail.attr("userId");

            if (currentUserId != userId) {
                return;
            }

            var colleaguesWrapper = $('.contacts .colleagues-wrapper');
            var leadersField = colleaguesWrapper.find('.leaders-field');
            var leadersUL = colleaguesWrapper.find('.leader-list');

            if (leaders.length == 0) {
                leadersField.hide();
                leadersUL.hide();
            } else {
                leadersField.show();
                leadersUL.show();
                _self.batchDisplayContacts(leadersUL, leaders);
            }

            var colleaguesField = colleaguesWrapper.find('.colleagues-field');
            var colleaguesUL = colleaguesWrapper.find('.colleague-list');

            if (colleagues.length == 0) {
                colleaguesField.hide();
                colleaguesUL.hide();
            } else {
                colleaguesField.show();
                colleaguesUL.show();
                _self.batchDisplayContacts(colleaguesUL, colleagues);
            }
        },
        batchDisplayContacts: function (ul, contacts) {
            var _self = this;

            _self.addOrUpdateContactsToCache(contacts);

            $.each(contacts, function (k, contact) {
                if (ul.find(".contact-item-" + contact.userId).length > 0) {
                    return true;
                }

                var item = _self.getCachedContact(contact.userId);
                var template = _self.displayContactItemUI(item);

                ul.append(template);
            });
        },
        _getContactTemplate: function () {
            var defaultAvatar = uc.constants.UI.DefaultLocalUrl.Avatar;
            var template = $('<li class="contact-item">'
                + '	<div class="contact-widget-container">'
                + '		<div class="left"><img src="../images/avatar_default.png" onerror="uc.util.ImageUtil.OnImageLoadError(this,\'' + defaultAvatar + '\')" class="avatar round-image"/>'
                + '<div class="user-list-status round-image hide"></div>'
                + '</div>'
                + '		<div class="right">'

                + '         <div class="checkbox-parent check-box-contact">'
                + '         	<input type="checkbox"/>'
                + '         	<label><span><img class="remove-contact" src="../images/contacts/remove.png"/></span></label>'
                + '         </div>'

                + '			'
                + '		</div>'
                + '		<div class="center">'
                + '			<div class="contact-display-name text-ellipsis"></div>'
                + '			<div class="contact-description text-ellipsis"><span class="contact-department"></span><span class="contact-position"></span></div>'
                + '		</div>'
                + '	</div>'
                + '</li>');
            return template;
        },
        _getColleaguesTemplate: function () {
            var template = $('<li class="colleague-item">'
                + '<span class="left-edge">'
                + '<img src="../images/contacts/avatar1.png" width="32px" height="32px"/>'
                + '</span>'
                + '<span class="text colleague"></span>'
                + '<span class="right-edge"></span>'
                + '</li>');
            return template;
        },
        _getFigurePanelItem: function (panelName, parentElement) {
            var panel;
            switch (panelName) {
                case "panel-contacts":
                    panel = new uc.modules.contact.ContactFiguresView();
                    panel.init({parentNode: parentElement});
                    break;
                default:
                    panel = new uc.modules.contact.ContactFigureView();
                    panel.init({parentNode: parentElement, panelName: panelName});
                    break;
            }
            return panel;
        },
        showContactsPanel: function (mainPanel) {
            var _self = this;

            mainPanel.empty();

            var searchBox = $('<div class="contact-search-container"></div>').appendTo(mainPanel);
            searchBox.searchInput({
                parent: mainPanel,
                searchScope: $.extend({}, $.fn.searchInput.defaults.searchScope, {
                    'default': [uc.constants.Search.SearchScope.Contact, uc.constants.Search.SearchScope.Project, uc.constants.Search.SearchScope.Group]
                })
            });

            var contactsPanel = $('<div id="contactPanel" class="contacts-panel"></div>');
            mainPanel.append(contactsPanel);

            _self.setLatestClickedContactsPanel(contactsPanel);

            var splitter = $('<div class="contacts-panel-splitter"><img src="../images/contacts/contacts_panel_splitter.png"/></div>');
            contactsPanel.append(splitter);

            var panelContacts = $('<div class="panel-contacts panel-item"></div>');
            var contactBreadCrumbs = $('<div class="contact-bread-crumbs bread-crumb">'
                + uc_resource.ChooseContacts.FrequentContacts
                + '</div>');
            panelContacts.append(contactBreadCrumbs);
            var contactsUL = $('<ul class="contact-list"></ul>');
            panelContacts.append(contactsUL);
            contactsPanel.append(panelContacts);

            //This is projects panel.
            var panelProjects = $('<div class="panel-projects panel-item"></div>');

            var projectsUL = $('<ul class="project-list group-list"></ul>');
            panelProjects.append(projectsUL);
            contactsPanel.append(panelProjects);

            //This is groupChats panel.
            var panelGroupChat = $('<div class="panel-groupChats panel-item"></div>');
            var groupChatsUL = $('<ul class="group-chat-list group-list"></ul>');
            panelGroupChat.append(groupChatsUL);
            contactsPanel.append(panelGroupChat);

            //This is my department panel.
            var panelMyDepartment = $('<div class="panel-my-department panel-item"></div>');
            var departmentUL = $('<ul class="my-department-list"></ul>');
            panelMyDepartment.append(departmentUL);
            contactsPanel.append(panelMyDepartment);

            //This is organization panel.
            var panelOrganization = $('<div class="panel-organization panel-item"></div>');
            var organizationUL = $('<div class="org-parent"></div>');
            panelOrganization.append(organizationUL);
            contactsPanel.append(panelOrganization);

            contactsPanel.find('.panel-item').hide();
            //contactsPanel.find('.panel-projects').fadeIn('slow');
            this.switchPanel("panel-contacts", true);
            this.setLatestClickedPanel("panel-contacts");
        },
        noticeContactUpdated: function (updatedContact) {
            var _self = this;
            var userId = updatedContact.userId;
            var cachedContact = _self.getCachedContact(userId);

            if (!cachedContact) {
                return;
            }

            var needUpdateCache = false;
            var needUpdateAvatar = false;
            var needUpdatePersonalSign = false;

            if (updatedContact.remoteAvatarUrl != cachedContact.remoteAvatarUrl) {
                //Clear localAvatar, re-download
                cachedContact.localAvatar = uc.constants.UI.DefaultLocalUrl.Avatar;
                cachedContact.remoteAvatarUrl = updatedContact.remoteAvatarUrl;
                needUpdateCache = true;
                needUpdateAvatar = true;
            }

            if (updatedContact.personalSign != cachedContact.personalSign) {
                cachedContact.personalSign = updatedContact.personalSign;
                needUpdateCache = true;
                needUpdatePersonalSign = true;
            }
            if (updatedContact.displayName != cachedContact.displayName) {
                cachedContact.displayName = updatedContact.displayName;
                needUpdateCache = true;
                $('.contact-display-name[userid=' + userId + ']').text(updatedContact.displayName);
            }
            if (updatedContact.departmentName != cachedContact.departmentName) {
                cachedContact.departmentName = updatedContact.departmentName;
                needUpdateCache = true;
                $('.contact-department[userid=' + userId + ']').text(updatedContact.departmentName);
            }
            if (updatedContact.position != cachedContact.position) {
                cachedContact.position = updatedContact.position;
                needUpdateCache = true;
                $('.contact-position[userid=' + userId + ']').text(updatedContact.position);
            }
            if (needUpdateCache) {
                _self.addOrUpdateContactToCache(userId, cachedContact);
            }

            if (needUpdatePersonalSign) {
                _self.changeUserPersonalSignatureUI(userId, updatedContact.personalSign);
            }

            if (needUpdateAvatar) {
                //re-download
                uc.IUIService.downloadUserAvatar(userId, updatedContact.remoteAvatarUrl);
            }
        },
        changeUserPersonalSignatureUI: function (userId, signature) {
            var _self = this;
            var signatures = $('.user-signature-' + userId);

            if (signatures.length > 0) {
                //Check whether is self
                var currentUser = uc.IContactService.getCurrentUserInfo();
                var currentUserId = currentUser.userId;
                var isSelf = currentUserId == userId;

                for (var i = 0; i < signatures.length; i++) {
                    var signatureElement = $(signatures[i]);

                    _self.setUserSignature(signatureElement, signature);
                    _self.setUserFieldValue(signatureElement, signature, isSelf);
                }
            }
            $('.user-signature-input-' + userId).each(function () {
                this.value = signature;
            });

        },
        noticeUserAvatarChanged: function (userId, remoteAvatarUrl) {

        },
        noticeFrequentContactReceived: function (contacts) {
            var _self = this;
            var contact = contacts[0];
            var userId = contact.userId;
            var key = _self.getFrequentContactKey(userId);
            var temp = frequentContactTaskMap.get(key);

            if (temp == null) {
                return;
            }

            frequentContactTaskMap.remove(key);
            _self.showNewFrequentContactItemUI(contact);
        },
        noticeFrequentContactRemoved: function (userId) {
            //Remove from cache
            var contacts = uc.modules.chooseContacts.ChooseContactsManager.getCachedFrequentContacts();

            if (uc.util.CollectionUtil.findObjectByKey(contacts, "userId", userId) != null) {
                var otherItems = contacts.filter(function (contact) {
                    return contact.userId !== userId;
                });

                //Cache Frequent Contacts for popup select Contacts
                uc.modules.chooseContacts.ChooseContactsManager.setCachedFrequentContacts(otherItems);
            }

            this.removeFrequentContactItemFromUI(userId);
        },
        removeFrequentContactItemFromUI: function (userId) {
            var contactItem = $('.contact-list .contact-item-' + userId);

            if (contactItem.length > 0) {
                //If contact is the only by first word
                var previousSibling = contactItem.prev();
                var nextSibling = contactItem.next();

                if (previousSibling.hasClass("alphabetical-key-item")
                    && (nextSibling.hasClass("alphabetical-key-item")
                    || !nextSibling.hasClass("alphabetical-key-item") && !nextSibling.hasClass("contact-item"))
                ) {
                    previousSibling.remove();
                }
                contactItem.remove();
            }
        },
        noticeFrequentContactAdded: function (userId) {
            var _self = this;
            var cachedContact = _self.getCachedContact(userId);
            var contact;

            if (cachedContact) {
                contact = cachedContact;
            } else {
                frequentContactTaskMap.put(_self.getFrequentContactKey(userId), {userId: userId});
                contact = uc.IContactService.getBasicContactInfo(userId);

                if (typeof(contact) == "undefined") {
                    //Wait for OnContactInfoReceived Event.
                }
            }

            if (contact) {
                _self.showNewFrequentContactItemUI(contact);
            }
        },
        showNewFrequentContactItemUI: function (contact) {
            var _self = this, keyItem, previousElement;

            var contacts = uc.modules.chooseContacts.ChooseContactsManager.getCachedFrequentContacts();
            var parentUL = $('.contact-list');

            var userId = contact.userId;

            _self.addOrUpdateContactToCache(userId, contact);

            if (uc.util.CollectionUtil.findObjectByKey(contacts, "userId", Number(userId)) == null) {
                contacts.push(contact);
            }

            //Sorted array, #(ZZ) last,
            var alphabeticalSortContacts = _self.getAlphabeticalSortArray(contacts, "pinyin", "displayName");

            //Cache Frequent Contacts for popup select Contacts
            uc.modules.chooseContacts.ChooseContactsManager.setCachedFrequentContacts(alphabeticalSortContacts);

            //2. Check whether need display on UI
            var currentIndex = $.inArray(contact, alphabeticalSortContacts);

            var originalKey = "#";
            var key = _self.poundKeyReplacer;
            if (contact.hasOwnProperty("pinyin") && contact["pinyin"].length > 0) {
                originalKey = contact["pinyin"].substr(0, 1);
            }
            key = originalKey.replace("#", _self.poundKeyReplacer);

            if (currentIndex == 0) {
                //prepend new group to parent
                var firstLiElement = parentUL.find("li:first");

                if (firstLiElement.length == 0) {
                    firstLiElement = null;
                }

                keyItem = parentUL.find('li[alphabetical-key="' + key + '"]');

                if (keyItem.length == 0) {
                    previousElement = _self.displayGroupKey(parentUL, key, firstLiElement, true);
                } else {
                    previousElement = keyItem;
                }
                _self.loadContactItemUI(contact, parentUL, previousElement);

            } else if (currentIndex > 0) {
                //Find previous group, insertAfter previous group
                //If nod found, no need to process; will process when scroll
                var previousContact = alphabeticalSortContacts[currentIndex - 1];
                var previousUserId = previousContact.userId;
                var previousContactElement = parentUL.find('.contact-item-' + previousUserId);

                if (previousContactElement.length > 0) {
                    //If sort key different from last item, then need add key
                    var lastKey = previousContact.sortKey;

                    if (originalKey == lastKey) {
                        //No need to insert key item;
                        _self.loadContactItemUI(contact, parentUL, previousContactElement);
                    } else {
                        keyItem = parentUL.find('li[alphabetical-key="' + key + '"]');

                        if (keyItem.length == 0) {
                            previousElement = _self.displayContactKey(parentUL, key, previousContactElement);
                        } else {
                            previousElement = keyItem;
                        }

                        _self.loadContactItemUI(contact, parentUL, previousElement);
                    }
                }
            }
        },
        displayContactKey: function (ul, key, previousElement, isInsertBefore) {
            var _self = this;
            //If key is #, need transform to ZZ, Jquey does not support alphabetical-key-item-#
            var keyItem = ul.find('li[alphabetical-key="' + key + '"]');

            if (keyItem.length == 0) {
                keyItem = _self._getContactKeyTemplate();
                keyItem.find('.sort-key').text(key.replace(_self.poundKeyReplacer, "#"));
                keyItem.attr("alphabetical-key", key);

                if (previousElement != null) {
                    if (isInsertBefore) {
                        keyItem.insertBefore(previousElement);
                    } else {
                        keyItem.insertAfter(previousElement);
                    }
                } else {
                    ul.append(keyItem);
                }

                return keyItem;
            }

            return null;
        },
        updateCustomTags: function (newtag, inputObj) {
            $(".uploadMask").show();
            uc.IContactService.updateCustomTags(newtag);
            inputObj.hide();
            inputObj.siblings().show();
        },

        OnUserTagsUpdated: function (errorCode, UpdatedTags) {
            if (errorCode == 0) {
                $(".uploadMask").hide();
            } else {
                //提示更新失败
                uc.modules.component.PromptMessageBox.open({
                    title: uc_resource.WindowTitle.Notification,
                    message: uc_resource.Profile.GetServerFailed,
                    buttonMode: "ok",
                    OkText: uc_resource.Common.OK
                }, function (result) {
                    if (result === 0) {
                        this.close();
                        $(".uploadMask").hide();
                    }
                });
            }

            if (!this.contactPanel) {
                return;
            }

            var tags = eval('(' + UpdatedTags + ')');
            var id = tags[0].tag_id;
            var value = tags[0].tag_value;

            this.contactPanel.find(".edit-profile-" + id).prev(".edit-value").text(value);
        },
        OnUserStatusSetted: function (errorCode, userId, status) {

        },
        OnUserStatusReceived: function (errorCode, status) {
            var statusDiv = $(".user-status");

            if (errorCode == 0) {
                for (var i in status) {
                    if (status[i].userId == Number($(".contact-selected").attr("userid"))) {
                        this.currentUserStatus = status[i].status;

                        statusDiv.find("div").removeClass();

                        switch (status[i].status) {
                            case 1:
                                statusDiv.find("div").addClass("status-online");
                                statusDiv.find("span").text(uc_resource.UserStatus.OnLine);
                                break;
                            case 2:
                                statusDiv.find("div").addClass("status-busy");
                                statusDiv.find("span").text(uc_resource.UserStatus.Busy);
                                break;
                            case 3:
                                break;
                            case 4:
                                statusDiv.find("div").addClass("status-incall");
                                statusDiv.find("span").text(uc_resource.UserStatus.InMeeting);
                                break;
                            case 5:
                                statusDiv.find("div").addClass("status-offline");
                                statusDiv.find("span").text(uc_resource.UserStatus.OffLine);
                                break;
                            case 6:
                                statusDiv.find("div").addClass("status-incall");
                                statusDiv.find("span").text(uc_resource.UserStatus.InCall);
                                break;
                        }
                    }
                    break;
                }
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
    }
})(uc);
