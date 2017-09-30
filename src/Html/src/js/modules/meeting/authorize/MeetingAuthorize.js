/**
 * Created by waouyang on 16/4/26.
 */
(function (uc) {
    var _this = null;

    uc.modules.meeting.MeetingAuthorize = {
        init: function () {
            _this = this;

            this.authorizeUserId = 0;
            this._init();
            this.bindEvent();
            this._getAuthorizeUserList();
        },

        _init: function(){
            this.container = $('.popup-panel');
            this.userInput = $('.user_input');
            this.addUserNode = $('.content-add');
            this.userListNode = $('.content-del');
            this.userSettingBtn = $('#accreditSetting');
            this.avatarNode = $('.avatar', this.userListNode);
            this.displayNameNode = $('.name', this.userListNode);
            this.departmentNode = $('.department', this.userListNode);
            this.positionNode = $('.position', this.userListNode);

            this.btnNode = $('.popup-panel-result');
            this.saveBtn = $('.finish', this.btnNode);
            this.cancelBtn = $('.cancel', this.btnNode);

            uc.modules.component.SearchBox.init(this.userInput, {
                count: 5,
                width: 358,
                offsetLeft: 10,
                selectedUser: function(args){
                    _this.onUserSelected(args);
                }
            });

        },

        _getAuthorizeUserList: function () {
            var authorizeFromMeUserList = uc.util.StorageUtil.getData('authorizeFromMeUserList');
            uc.util.LogUtil.info("MeetingAuthorize", "_getAuthorizeUserList", "authorizeFromMeUserList = ", authorizeFromMeUserList);

            this._showAuthorizeUser(authorizeFromMeUserList);
        },

        bindEvent: function () {
            $(".accredit-meeting .popup-panel-head a").on("click", function() {
                uc.util.WindowUtil.close();
            });

            $(document.body).on("keydown", function(event) {
                if (event.keyCode == 27) {
                    uc.util.WindowUtil.close();
                }
            });

            this.userSettingBtn.click(function(){
                _this._changeAuthorizeUser($(this));
            });

            this.saveBtn.click(function(){
                _this._saveAuthorizeUser($(this));
            });

            this.cancelBtn.click(function(){
                uc.util.WindowUtil.close();
            });

            this.userInput.on("keyup", function(e) {
                if (e.which == 13 ) {
                    $(this).val("");
                }
            });

            this.userInput.on("click", function() {
                _this.userInput.css("width", "358px");
                _this.userInput.css("border", "1px solid #dadada");
                _this.userInput.css("background-color", "#ffffff");
            });
        },

        _saveAuthorizeUser: function($this){
            if ($this.hasClass("unavailable")) {
                return;
            }

            if (this.newAuthorizeUserId) {
                uc.IMeetingService.updateAccreditSetting(this.newAuthorizeUserId, 1, function (args) {
                    uc.util.LogUtil.info("MeetingAccreditation", "_modifyAccreditation", "_saveAuthorizeUser", {errorCode: args[0]});
                });
            }

            if (this.authorizeUserId) {
                uc.IMeetingService.updateAccreditSetting(this.authorizeUserId, 0, function (args) {
                    uc.util.LogUtil.info("MeetingAccreditation", "_modifyAccreditation", "_saveAuthorizeUser", {errorCode: args[0]});
                });
            }

            uc.util.WindowUtil.close();
        },

        _showAuthorizeUser: function(authorizeFromMeUserList){
            if(authorizeFromMeUserList && authorizeFromMeUserList.length >= 1){
                this.authorizeUserId = authorizeFromMeUserList[0];
                this.addUserNode.hide();
                this.userListNode.show();
                this.container.css("height", "181px");
                this.btnNode.hide();

                var contactInfo = uc.IContactService.getBasicContactInfo(this.authorizeUserId);
                this._showUserInfo(contactInfo);
            }else {
                this.authorizeUserId = 0;
                this.addUserNode.show();
                this.userListNode.hide();
                this.btnNode.show();
                this.saveBtn.addClass("unavailable");
            }
        },

        _changeAuthorizeUser: function($this){
            if ($this.text() == uc_resource.Meeting.CancelAccredit) {
                var userName = this.displayNameNode.text();
                var cancelAuthorizeMsg = uc.util.StringUtil.formatString(uc_resource.Meeting.CancelOthersAccredit, [userName]);

                uc.modules.component.PromptMessageBox.open({
                    title: uc_resource.WindowTitle.Notification,
                    message: cancelAuthorizeMsg,
                    buttonMode: "both",
                    CancelText: uc_resource.Common.Cancel,
                    OkText: uc_resource.Common.OK
                }, function (result) {
                    if (result == 0) {
                        uc.IMeetingService.updateAccreditSetting(_this.authorizeUserId, 0, function (args) {
                            uc.util.LogUtil.info("MeetingAuthorize", "cancelAuthorize", "cancelAuthorize result = ", {errorCode: args[0]});
                        });

                        _this.userListNode.hide();
                        _this.addUserNode.show();
                        _this.container.css("height", "253px");
                        _this.btnNode.show();
                        this.close();
                    } else if (result == 1) {
                        this.close();
                    }
                });
            } else if ($this.text() == uc_resource.Meeting.RepeatChose) {
                this.userListNode.hide();
                this.userInput.val("").focus();
                this.addUserNode.show();
                this.btnNode.show();

                this.saveBtn.addClass("unavailable");
            }
        },

        onUserSelected: function(userInfo){
            var userId = uc.IContactService.getCurrentUserInfo().userId;

            if (userInfo.userId > 0 && userInfo.userId != userId) {
                this.addUserNode.hide();
                this.userListNode.show();
                this.userSettingBtn.html(uc_resource.Meeting.RepeatChose);
                this.userSettingBtn.addClass("re-select");
                this.btnNode.show();
                this.saveBtn.removeClass("unavailable").addClass("available");

                this.newAuthorizeUserId = userInfo.userId;
                this._showUserInfo(userInfo);
            }
        },

        _showUserInfo: function(userInfo){
            if (userInfo.localAvatar) {
                this.avatarNode.attr("src", userInfo.localAvatar);
            } else if (userInfo.remoteAvatarUrl) {
                uc.IUIService.downloadUserAvatar(userInfo.userId, userInfo.remoteAvatarUrl);
                this.avatarNode.attr("src", "../images/avatar_default.png");
            } else {
                this.avatarNode.attr("src", "../images/avatar_default.png");
            }

            this.displayNameNode.text(userInfo.displayName);
            this.departmentNode.text(userInfo.departmentName);
            this.positionNode.text(userInfo.position);

            this._adjustUserListNode();
        },

        _adjustUserListNode: function(){
            var maxWidth = this.displayNameNode.width() - 17;
            var departmentWidth = this.departmentNode.width();
            var positionWidth = this.positionNode.width();

            var style = {
                'overflow': "hidden",
                'text-overflow': "ellipsis",
                'white-space': "nowrap",
                'display': "inline-block"
            };

            if (departmentWidth >= maxWidth) {
                this.departmentNode.css(style);
                this.departmentNode.css("width", maxWidth + "px");
                this.positionNode.hide();
            }else if (departmentWidth + positionWidth > maxWidth) {
                this.positionNode.css(style);
                this.positionNode.css("width", (maxWidth - departmentWidth) + "px");
            }
        },

        OnMeetingAccreditMessageReceived: function (channel, from, to, msgId, msgSeq, msgTime) {
            if(channel == uc.constants.Message.MessageChannel.UCAS){
                if(from.userId == uc.IContactService.getCurrentUserInfo().userId){
                    this._showAuthorizeUser([to.userId]);
                }
            }
        },

        OnMeetingAccreditCancelledMessageReceived: function (channel, from, to, msgId, msgSeq, msgTime) {
            if(channel == uc.constants.Message.MessageChannel.UCAS){
                if(channel == uc.constants.Message.MessageChannel.UCAS){
                    if(from.userId == uc.IContactService.getCurrentUserInfo().userId && to.userId == this.authorizeUserId){
                        this._showAuthorizeUser([]);
                    }
                }
            }
        }
    };

    jQuery(document).ready(function () {
        uc.init();
        uc.modules.meeting.MeetingAuthorize.init();
    });
})(uc);