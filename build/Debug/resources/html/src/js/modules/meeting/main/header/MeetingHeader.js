/**
 * Created by waouyang on 16/4/26.
 */
(function(uc){
    var _this = null;
    uc.modules.meeting.MeetingHeader = {
        nodeHtml:   "<div class='accredit-list'>"+
                        "<div class='accredit-panel'>" +
                            "<a href='javascript:void(0)' class='accreditted'>" + uc_resource.Meeting.MyCalendar + "</a>" +
                            "<div class='accredit-users'>" +
                                "<a href='javascript:void(0)' class='accredit-user me accredit-user-selected' userId='0'>"+uc_resource.Meeting.MyCalendar+"</a>" +
                                "<a href='javascript:void(0)' class='accredit-setting'>"+uc_resource.Meeting.SetAccreditTitle+"</a>" +
                            "</div>" +
                        "</div>" +
                    "</div>",
        searchNodeHtml: '<div class="meeting-search-container"></div>',

        init: function(){
            _this = this;
            this.node = $(this.nodeHtml);
            this.panel = $('.accredit-panel', this.node);
            this.seletedNode = $('.accreditted', this.panel);
            this.userList = $('.accredit-users', this.panel);
            this.settingMenu = $('.accredit-setting', this.panel);
            this.node.appendTo(uc.ui.Meeting.MeetingTitle);

            this.shownAuthorizerUserId = 0;
            this.userIds = [];

            this.searchNode = $(this.searchNodeHtml).appendTo(uc.ui.Meeting.MeetingTitle);

            this.searchNode.searchInput({
                parent: uc.ui.Meeting.MeetingTitle,
                searchScope: $.extend({}, $.fn.searchInput.defaults.searchScope, {
                    'default': [uc.constants.Search.SearchScope.Meeting]
                }),
                defaultDisplayCount:9
            });

            this.bindEvent();
        },

        bindEvent: function(){
            this.seletedNode.click(function(e){
                e.stopPropagation();
                _this.userList.show();
            });
            this.userList.on('click', '.accredit-user', function(){
                _this._setSelectedAuthorizerUser($(this));
            });

            this.settingMenu.click(function(){
                _this._openMeetingAuthorizeWindow();
            });

            $("body").click(function () {
                _this.userList.hide();
            });
        },

        _setSelectedAuthorizerUser: function($userMenu){
            if ($userMenu.hasClass("accredit-user-selected")) {
                return;
            }
            
            $userMenu.parent().children(".accredit-user-selected").removeClass("accredit-user-selected");
            $userMenu.addClass("accredit-user-selected");
            this.seletedNode.text($userMenu.text());

            this.shownAuthorizerUserId = parseInt($userMenu.attr("userId"));
            uc.modules.meeting.MeetingListManager.onAuthorizerUserIdChanged(this.shownAuthorizerUserId);
        },
        
        setAuthorizerUser: function(authorizerId){
            this.userList.find(".accredit-user-selected").removeClass("accredit-user-selected");

            var $userMenu = this.userList.find(".accredit-user[userId='" + authorizerId + "']");
            $userMenu.addClass("accredit-user-selected");
            this.seletedNode.text($userMenu.text());

            this.shownAuthorizerUserId = parseInt($userMenu.attr("userId"));
        },

        _openMeetingAuthorizeWindow: function(){
            var authorizeFromMeUserList = uc.modules.meeting.MeetingHeaderManager.getAuthorizeFromMeUserList();

            uc.util.StorageUtil.setData('authorizeFromMeUserList', authorizeFromMeUserList);

            var options = {
                width: 400,
                height: 490,
                refreshIfExist: false,
                centerScreen: true,
                modal: true,
                resizable: false,
                caption: uc.constants.UI.DragCaption.AccreditMeeting
            };

            uc.util.WindowUtil.open("page/meetingAuthorize.html", "accreditMeeting", options);
        },

        listAuthorizeUser: function(userIds){
            this.userList.find('.accredit-user:not(.me)').remove();
            this.userIds = [];

            userIds.map(function(user){
                _this._addAuthorizeUser(user);
            });

            if (this.userIds.length > 0) {
                uc.IContactService.getBasicContactInfo(this.userIds);
            }
        },

        addAuthorizeUser: function(userId){
            this._addAuthorizeUser(userId);
        },

        _addAuthorizeUser: function(userId){
            var contactInfo = uc.IContactService.getBasicContactInfo(userId);
            var displayName = userId;
            if(contactInfo){
                displayName = contactInfo.displayName;
            }
            displayName = uc.util.StringUtil.formatString(uc_resource.Meeting.OthersCalendar, [displayName]);
            this.userList.prepend('<a href="javascript:void(0)" class="accredit-user" userId="'+ userId +'">' + displayName + '</a>');

            this.userIds.push(userId);
        },

        deleteAuthorizeUser: function(userId){
            this.userList.find(".accredit-user[userId='" + userId + "']").remove();
            if (this.shownAuthorizerUserId == userId) {
                this.userList.find(".accredit-user.me").click();
            }
        },

        _updateAuthorizeUser: function(user){
            var $user = this.userList.children("a[userId='" + user.userId + "']");
            if ($user.length > 0) {
                $user.text(uc.util.StringUtil.formatString(uc_resource.Meeting.OthersCalendar, [user.displayName]));
            }
        },

        updateAuthorizeUsers: function(contacts){
            if(contacts && contacts.length){
                contacts.map(function(contact){
                    if(_this.userIds.indexOf(contact.userId) != -1){
                        _this._updateAuthorizeUser(contact);
                    }
                })
            }
        }
    }
})(uc);