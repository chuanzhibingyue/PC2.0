(function (uc) {
    uc.modules.chooseContacts.ChooseContactsManager = {
        cachedFrequentContacts: [],
        cachedMyDepartments: [],
        cachedOrganizations: [],
        membersLimitMessage: uc_resource.ChooseContacts.MembersLimitMessage,
        getOrgAllMembersExceptionMessage: uc_resource.ChooseContacts.GetOrgAllMembersExceptionMessage,
        projectAlreadySelectedMessage: uc_resource.ChooseContacts.ProjectAlreadySelectedMessage,
        chatAlreadySelectedMessage: uc_resource.ChooseContacts.ChatAlreadySelectedMessage,
        groupNotExistsMessage: uc_resource.ChooseContacts.GroupNotExistsMessage,
        promptTitle: uc_resource.Profile.Prompt,
        isSuperGroup: false,
        isForwardMessage: false,
        clearCache: function () {
            this.clearCachedMyDepartments();
            this.clearCachedOrganizations();
            this.isSuperGroup = false;
            this.isForwardMessage = false;
        },
        getCachedFrequentContacts: function () {
            return this.FrequentContacts;
        },
        setCachedFrequentContacts: function (contacts) {
            this.FrequentContacts = contacts;
        },
        clearCachedFrequentContacts: function () {
            this.FrequentContacts = [];
        },
        getCachedMyDepartments: function () {
            return this.cachedMyDepartments;
        },
        setCachedMyDepartments: function (departments) {
            this.cachedMyDepartments = departments;
        },
        addMyDepartmentToCache: function (department) {
            if (uc.util.CollectionUtil.findObjectByKey(this.cachedMyDepartments, "id", department.id) == null) {
                this.cachedMyDepartments.push(department);
            }
        },
        clearCachedMyDepartments: function () {
            this.cachedMyDepartments = [];
        },
        addOrganizationToCache: function (organization) {
            if (uc.util.CollectionUtil.findObjectByKey(this.cachedOrganizations, "orgId", organization.orgId) == null) {
                this.cachedOrganizations.push(organization);
            }
        },
        getCachedOrganizations: function () {
            return this.cachedOrganizations;
        },
        setCachedOrganizations: function (organizations) {
            this.cachedOrganizations = organizations;
        },
        clearCachedOrganizations: function () {
            this.cachedOrganizations = [];
        },
        getMembersLimit: function () {
            var membersLimit = this.isSuperGroup ? uc.IClientService.getMaxGroupMemberLimit() : uc.IClientService.getMinGroupMemberLimit();
            return parseInt(membersLimit);
        },
        getMembersLimitMessage: function () {
            return this.membersLimitMessage;
        },
        init: function () {
            this.initLeftPanel();
            this.bindEvents();
        },
        initLeftPanel: function () {
            var parentElement = $('.popup-panel-left-body');
            var panel = new uc.modules.contact.ContactFiguresView();
            panel.init({parentNode: parentElement});
            uc.modules.contact.ContactManager.setLatestClickedContactsPanel(parentElement);
            uc.modules.contact.ContactManager.switchPanel("panel-contacts");
        },

        bindEvents: function () {
            var _self = this;
            var leftHeader = $('.popup-panel-header-left');
            leftHeader.off("input");
            leftHeader.off("click");

            var timer;
            leftHeader.on("input", ".search-input", function () {
                //Add a delay when user inputs to reduce search count
                //http://stackoverflow.com/questions/14471889/jquery-keyup-delay
                timer && clearTimeout(timer);
                timer = setTimeout(function () {
                    var keyword = leftHeader.find('.search-input').val().trim();
                    uc.modules.chooseContacts.SearchContactsManager.onSearchInput(keyword);
                }, 300);
            });
            leftHeader.on("click", ".icon-search-del", function () {
                leftHeader.find('.search-input').val("");
                var keyword = "";
                uc.modules.chooseContacts.SearchContactsManager.onSearchInput(keyword);
            });

            var parentElement = $('.popup-panel-left-body');
            parentElement.off("click");

            //Process my department choose
            parentElement.on("click", ".my-department-item", function () {

                var checkbox = $(this).find("input[type='checkbox']");
                if (checkbox.is(":checked")) {
                    checkbox.prop("checked", false);
                    uc.modules.chooseContacts.PopupChooseContacts.unCheckLeftMyDept($(this));
                } else {
                    if (!uc.modules.chooseContacts.PopupChooseContacts.allowMultipleSelect($(this))) {
                        return;
                    }
                    var departmentItem = $(this).find('.department-item');
                    var orgId = departmentItem.attr("orgId");
                    var myDepartmentInfo = uc.util.CollectionUtil.findObjectByKey(_self.getCachedMyDepartments(), "id", Number(orgId));
                    var staffs = myDepartmentInfo.staffs;

                    //1. Check Members limit count
                    var contactIdArray = [];
                    $.each(staffs, function (k, contact) {
                        contactIdArray.push(contact.userId);
                    });

                    if (!_self.checkMembersLimit(contactIdArray)) {
                        return;
                    }

                    checkbox.prop("checked", true);
                    uc.modules.chooseContacts.PopupChooseContacts.checkLeftMyDept($(this));
                }
            });

            //Process contact choose
            parentElement.on("click", ".contact-list .contact-item", function () {
                var checkbox = $(this).find("input[type='checkbox']");
                if (checkbox.is(':disabled')) {
                    return;
                }
                if (checkbox.is(":checked")) {
                    checkbox.prop("checked", false);
                    uc.modules.chooseContacts.PopupChooseContacts.unCheckLeftContact($(this));

                } else {
                    if (!uc.modules.chooseContacts.PopupChooseContacts.allowMultipleSelect()) {
                        return;
                    }
                    //1. Find chosen Contact id
                    var userId = $(this).attr("userId");

                    //2. Check distinct (Contact + already chosen members) count
                    var contactIdArray = [];
                    contactIdArray.push(Number(userId));
                    if (!_self.checkMembersLimit(contactIdArray)) {
                        return;
                    }

                    //Change contact Id Array to Contact Objects, process in OnContactInfoReceived
                    _self.processContactsDetail(contactIdArray);

                    checkbox.prop("checked", true);
                    uc.modules.chooseContacts.PopupChooseContacts.checkLeftContact($(this));
                }
            });

            //Process group choose
            parentElement.on("click", ".group-item", function () {
                var checkbox = $(this).find("input[type='checkbox']");
                var groupType = $(this).attr("groupType");

                if (checkbox.is(":checked")) {
                    checkbox.prop("checked", false);
                    if (groupType == uc.modules.group.GroupManager.GROUP_PROJECT) {
                        uc.modules.chooseContacts.PopupChooseContacts.unCheckLeftProject($(this));
                    } else if (groupType == uc.modules.group.GroupManager.GROUP_CHAT) {
                        uc.modules.chooseContacts.PopupChooseContacts.unCheckLeftGroup($(this));
                    }
                } else {
                    if (!uc.modules.chooseContacts.PopupChooseContacts.allowMultipleSelect()) {
                        return;
                    }

                    //1. Find chosen Group id
                    var groupId = $(this).attr("groupId");

                    var group = null;
                    var flag = true;
                    var rightSelected = uc.modules.chooseContacts.PopupChooseContacts.selectedContactsResult();
                    if (groupType == uc.modules.group.GroupManager.GROUP_PROJECT) {
                        var rightSelectedProjects = rightSelected.projects;
                        $.each(rightSelectedProjects, function (i, rightSelectedProject) {
                            if (groupId == rightSelectedProject) {
                                flag = false;
                            }
                        });

                        if (flag) {
                            group = uc.modules.group.GroupManager.getCachedProjectGroup(Number(groupId));
                        }
                    } else if (groupType == uc.modules.group.GroupManager.GROUP_CHAT) {
                        var rightSelectedGroups = rightSelected.groupChats;
                        $.each(rightSelectedGroups, function (i, rightSelectedGroup) {
                            if (groupId == rightSelectedGroup) {
                                flag = false;
                            }
                        });

                        if (flag) {
                            group = uc.modules.group.GroupManager.getCachedChatGroup(Number(groupId));
                        }
                    }
                    if (!flag) {
                        var message = "";
                        if (groupType == uc.modules.group.GroupManager.GROUP_PROJECT) {
                            message = _self.projectAlreadySelectedMessage;
                        } else {
                            message = _self.chatAlreadySelectedMessage;
                        }
                        _self.popupMessage(message);
                        return;
                    }

                    if (!group) {
                        group = uc.IGroupService.getGroupInfo(Number(groupId));
                        if (group) {
                            uc.modules.group.GroupManager.addOrUpdateProjectGroupToCache(Number(groupId), group);
                        } else {
                            uc.util.LogUtil.info('ChooseContactsManager', 'Select Group', "Cannot find selected Group, Group Id: " + groupId, null);
                            _self.popupMessage(_self.groupNotExistsMessage);
                            return;
                        }
                    }

                    var members = group.members;

                    var contactIdArray = [];
                    $.each(members, function (k, member) {
                        contactIdArray.push(member.userId);
                    });

                    //2. Check whether Group members count,  distinct (Group members + already chosen members) count exceed limited count
                    if (!_self.checkMembersLimit(contactIdArray)) {
                        return;
                    }

                    //Change contact Id Array to Contact Objects, process in OnContactInfoReceived
                    _self.processContactsDetail(contactIdArray);

                    checkbox.prop("checked", true);
                    if (groupType == uc.modules.group.GroupManager.GROUP_PROJECT) {
                        uc.modules.chooseContacts.PopupChooseContacts.checkLeftProject($(this));
                    } else if (groupType == uc.modules.group.GroupManager.GROUP_CHAT) {
                        uc.modules.chooseContacts.PopupChooseContacts.checkLeftGroup($(this));
                    }
                }
            });

            //Special for Organizations and Staff
            parentElement.on("click", ".org-child", function () {
                var organizationItem = $(this).find('.item');
                //_self.singleClickOrganization(organizationItem);
                _self.multiClickElement(organizationItem, function () {
                    _self.singleClickOrganization(organizationItem)
                }, function () {
                    uc.modules.org.OrgManager.doubleClickOrganization(organizationItem)
                });
            });
            parentElement.on("click", ".staff-child", function () {
                var checkbox = $(this).find("input[type='checkbox']");
                if (checkbox.attr("disabled") == "disabled") {
                    return;
                }
                if (checkbox.is(":checked")) {
                    checkbox.prop("checked", false);
                    uc.modules.chooseContacts.PopupChooseContacts.unCheckOrgContact($(this));
                } else {
                    if (!uc.modules.chooseContacts.PopupChooseContacts.allowMultipleSelect()) {
                        return;
                    }
                    //1. Find chosen Contact id
                    var userId = $(this).find('.contact-item').attr("userId");

                    //2. Check distinct (Contact + already chosen members) count
                    var contactIdArray = [];
                    contactIdArray.push(Number(userId));
                    if (!_self.checkMembersLimit(contactIdArray)) {
                        return;
                    }

                    //Change contact Id Array to Contact Objects, process in OnContactInfoReceived
                    _self.processContactsDetail(contactIdArray);

                    checkbox.prop("checked", true);
                    uc.modules.chooseContacts.PopupChooseContacts.checkOrgContact($(this));
                }
            });
            uc.util.CaretUtil.setInputEditorCaretPosition($('.search-input'));
        },
        singleClickOrganization: function (organizationItem) {
            var _self = this;
            var checkbox = organizationItem.find("input[type='checkbox']");
            if (checkbox.attr("disabled") == "disabled") {
                return;
            }

            var orgId = organizationItem.attr("orgId");
            if (checkbox.is(":checked")) {

                var parentElement = organizationItem.parents('.org-parent');

                var newStatus = false;
                checkbox.prop("checked", newStatus);

                //Process sub organizations and staff
                var children = parentElement.find('.org-parent-' + orgId);
                $.each(children, function (k, child) {
                    var childItem = $(this);
                    if (childItem.attr('disabled') == 'disabled') {
                        return true;
                    }
                    var childCheckBox = childItem.find("input[type='checkbox']");
                    childCheckBox.prop("checked", newStatus);
                    childCheckBox.removeAttr('disabled');
                });

                //Need process data here, to do...
                uc.modules.chooseContacts.PopupChooseContacts.unCheckOrg(orgId);
            } else {
                if (!uc.modules.chooseContacts.PopupChooseContacts.allowMultipleSelect(organizationItem.parent())) {
                    return;
                }
                //Get Organzation's All Members(Including Sub Organizations)
	            var copyBack = '{"managerType":"uc.modules.chooseContacts.ChooseContactsManager"}';
                uc.IContactService.getOrgAllMembers(Number(orgId), _self.getMembersLimit(),copyBack);
                //Will process in uc.modules.chooseContacts.ChooseContactsManager.OnOrgAllMembersReceived(errorCode, orgId, userCount, memberIds) when event return.
            }
        },
        multiClickElement: function (el, onsingle, ondouble) {
            if (!el.attr("data-dblclick")) {
                el.attr("data-dblclick", 1);
                setTimeout(function () {
                    if (el.attr("data-dblclick") == 1) {
                        onsingle();
                    }
                    el.removeAttr("data-dblclick");
                }, 300);
            } else {
                el.removeAttr("data-dblclick");
                ondouble();
            }
        },
        unCheckMyDepartment: function (departmentId) {
            var className = ".check-box-my-department-" + departmentId;
            this.unCheckItem(className);
        },
        unCheckContact: function (contactId) {
            var className = ".check-box-contact-" + contactId;
            this.unCheckItem(className);
        },
        unCheckGroup: function (groupId) {
            var className = ".check-box-group-" + groupId;
            this.unCheckItem(className);
        },
        unCheckItem: function (className) {
            var parentElement = $('.popup .contacts-panel');
            var checkboxParents = parentElement.find(className);
            $.each(checkboxParents, function (k, checkboxParent) {
                var checkbox = $(checkboxParent).find("input[type='checkbox']");
                if (!checkbox.attr("disabled")) {
                    checkbox.prop("checked", false);
                }
            });
        },
        unCheckOrganization: function (organizationId) {
            var parentElement = $('.popup .contacts-panel');
            var className = ".check-box-organization-" + organizationId;
            var checkboxParent = parentElement.find(className);

            var checkbox = checkboxParent.find("input[type='checkbox']");
            if (checkbox.attr("disabled") == "disabled") {
                return;
            }
            //Process self
            checkbox.prop("checked", false);

            //Process sub organizations and staff
            var children = parentElement.find('.org-parent-' + organizationId);
            $.each(children, function (k, child) {
                var childItem = $(this);
                var childCheckBox = childItem.find("input[type='checkbox']");
                childCheckBox.prop("checked", false);
                childCheckBox.removeAttr('disabled');
            });
        },
        OnOpenChooseContactWindow: function (key) {
            uc.util.StorageUtil.setData("chooseContacts", key);
            var openTime = new Date();

            var options = {
                title: '',
                width: 600,
                height: 580,
                refreshIfExist: true,
                centerScreen: true,
                modal: true,
                resizable: false,
                minWidth: 600,
                minHeight: 580,
                caption: uc.constants.UI.DragCaption.ChooseContacts
            };

            uc.util.WindowUtil.open('page/chooseContacts.html', 'chooseContacts' + openTime, options);
        },
        OnOrgAllMembersReceived: function (errorCode, orgId, userCount, memberIds) {
            var organizationPanel = $('.popup .panel-organization');
            if (organizationPanel.length == 0) {
                return;
            }

            var _self = this;
            if (errorCode == 400) {
                _self.popupMessage(_self.getOrgAllMembersExceptionMessage);
                return;
            }
            if (errorCode == 10465) {
                var limitedCount = _self.getMembersLimit();
                var message = uc.util.StringUtil.formatString(_self.getMembersLimitMessage(), [limitedCount]);
                _self.popupMessage(message);
                return;
            }
	        var parentElement = $('.popup-panel-left-body');

            var item = parentElement.find('.org-item-' + orgId);
            var checkbox = item.find("input[type='checkbox']");

            //Check distinct (Organization all members + already chosen members) count
            var contactIdArray = [];
            $.each(memberIds, function (k, contactId) {
                contactIdArray.push(contactId);
            });

            if (!_self.checkMembersLimit(contactIdArray)) {
                return;
            }

            //Add Organization(OrgId, Members) to Cache
            var organization = {};
            organization.orgId = orgId;
            organization.memberIds = memberIds;
            _self.addOrganizationToCache(organization);

            _self.processContactsDetail(memberIds);

            //Process data here, to do...
            uc.modules.chooseContacts.PopupChooseContacts.checkOrg(item);

            var newStatus = true;
            checkbox.prop("checked", newStatus);

            //Process sub organizations and staff
            var children = parentElement.find('.org-parent-' + orgId);
            $.each(children, function (k, child) {
                var childItem = $(this);
                var childCheckBox = childItem.find("input[type='checkbox']");
                childCheckBox.prop("checked", newStatus);
                childCheckBox.attr('disabled', 'disabled');
            });
        },
        popupMessage: function (message) {
            var dialog = uc.modules.component.PromptMessageBox.open({
                title: this.promptTitle,
                message: message,
                buttonMode: "ok"
            }, function (result) {
                //alert("RESULT FROM CALLBACK: " + result);
                this.close();
            });
        },
        processContactsDetail: function (contactIdArray) {
            var _self = this;
            //Find detailed Contact Info, process in OnContactInfoReceived
            var toFindContactIdArray = [];
            $.each(contactIdArray, function (k, contactId) {
                if (!uc.modules.contact.ContactManager.getCachedContact(contactId)) {
                    toFindContactIdArray.push(contactId);
                }
            });
            if (toFindContactIdArray.length > 0) {
                uc.modules.contact.ContactManager.batchRequestMembers(toFindContactIdArray);
            }
        },
        checkMembersLimit: function (contactIdArray) {

            if (this.isForwardMessage) {
                return true;
            }

            var pass = false;
            var _self = this;

            //1. Check contactIdArray count
            var membersCount = contactIdArray.length;

            var limitedCount = _self.getMembersLimit();
            if (membersCount > limitedCount) {
                var message = uc.util.StringUtil.formatString(_self.getMembersLimitMessage(), [limitedCount]);
                _self.popupMessage(message);
                return pass;
            }

            //2. Check distinct (contactIdArray + already chosen members) count
            var chosenResult = uc.modules.chooseContacts.PopupChooseContacts.selectedContactsResult();
            var chosenContactIdArray = chosenResult.allContacts;
            var combinedContactIdArray = $.merge(contactIdArray, chosenContactIdArray); //contactIdArray.concat(chosenContactIdArray);
            var distinctContactIdArray = $.unique(combinedContactIdArray);
            membersCount = distinctContactIdArray.length;

            if (membersCount > limitedCount) {
                var message = uc.util.StringUtil.formatString(_self.getMembersLimitMessage(), [limitedCount]);
                _self.popupMessage(message);
                return pass;
            }

            return true;
        }
    }
})(uc);