/**
 * Created by waouyang on 16/4/26.
 */
(function (uc) {
    var _this = null;
    uc.modules.meeting.MeetingHeaderManager = {
        authorizeToMeUserIds: [],
        authorizeFromMeUserIds: [],

        init: function () {
            _this = this;
            uc.util.LogUtil.info("MeetingListManager", "initMeetingList", "getAccreditUserList", {});

            uc.IMeetingService.getAccreditUserList(function (args) {
                uc.util.LogUtil.info("MeetingListManager", "initMeetingList", "getAccreditUserList result errorCode = ", args[0]);
            });
        },

        OnInitClient: function (clearData) {
            if (clearData) {

            } else {
                uc.modules.meeting.MeetingHeader.init();
            }
            this.init();
        },

        _addAuthorizeToMeUser:function(userId){
            if(this.authorizeToMeUserIds.indexOf(userId) == -1){
                this.authorizeToMeUserIds.push(userId);
            }
        },

        _removeAuthorizeToMeUser: function(userId){
            var index = this.authorizeToMeUserIds.indexOf(userId);
            if(index != -1){
                this.authorizeToMeUserIds.splice(index ,1);
            }
        },

        isAuthorizeToMeUserExist: function(userId){
            return this.authorizeToMeUserIds.indexOf(userId) != -1;
        },

        _addAuthorizeFromMeUser: function(userId){
            if(this.authorizeFromMeUserIds.indexOf(userId) == -1){
                this.authorizeFromMeUserIds.push(userId);
            }
        },

        _removeAuthorizeFromMeUser: function(userId){
            var index = this.authorizeFromMeUserIds.indexOf(userId);
            if(index != -1){
                this.authorizeFromMeUserIds.splice(index ,1);
            }
        },

        switchAuthorizeUser: function(authorizerId){
            uc.modules.meeting.MeetingHeader.setAuthorizerUser(authorizerId);
        },

        getAuthorizeFromMeUserList: function(){
            return this.authorizeFromMeUserIds;
        },

        OnAccreditUserListReceived: function (errorCode, authorizeUsers) {
            uc.util.LogUtil.info("MeetingHeaderManager", "OnAccreditUserListReceived", "authorizeUsers = ", authorizeUsers);
            if (errorCode == uc.ErrorCode.Success && authorizeUsers) {
                this.authorizeToMeUserIds = [];
                this.authorizeFromMeUserIds = [];

                authorizeUsers.map(function(authorizeUser){
                    if(authorizeUser.status == uc.constants.Meeting.AuthorizeType.AuthorizeToMe){
                        _this._addAuthorizeToMeUser(authorizeUser.accreditUserId);
                    }else {
                        _this._addAuthorizeFromMeUser(authorizeUser.accreditUserId);
                    }
                });

                uc.modules.meeting.MeetingHeader.listAuthorizeUser(this.authorizeToMeUserIds);

            }
        },

        OnMeetingAccreditMessageReceived: function (channel, from, to, msgId, msgSeq, msgTime) {
            if (channel !== uc.constants.Message.MessageChannel.UCAS) {
                return;
            }
            uc.util.LogUtil.info("MeetingHeaderManager", "OnMeetingAccreditMessageReceived", "data=", {
                channel: channel,
                from: from,
                to: to,
                msgId: msgId,
                msgSeq: msgSeq,
                msgTime: msgTime
            });

            var currentUserInfo = uc.IContactService.getCurrentUserInfo();

            if (from && from.userId && from.userId != currentUserInfo.userId) {
                uc.modules.meeting.MeetingHeader.addAuthorizeUser(from.userId);
                this._addAuthorizeToMeUser(from.userId);
            }else if(from.userId == currentUserInfo.userId){
                this._addAuthorizeFromMeUser(to.userId);
            }
        },

        OnMeetingAccreditCancelledMessageReceived: function (channel, from, to, msgId, msgSeq, msgTime) {
            if (channel !== uc.constants.Message.MessageChannel.UCAS) {
                return;
            }
            uc.util.LogUtil.info("MeetingManager", "OnMeetingAccreditCancelledMessageReceived", "data=", {
                channel: channel,
                from: from,
                to: to,
                msgId: msgId,
                msgSeq: msgSeq,
                msgTime: msgTime
            });

            var currentUserInfo = uc.IContactService.getCurrentUserInfo();
            if (from && from.userId && from.userId != currentUserInfo.userId) {
                uc.modules.meeting.MeetingHeader.deleteAuthorizeUser(from.userId);
                this._removeAuthorizeToMeUser(from.userId);
            }else if(from.userId == currentUserInfo.userId){
                this._removeAuthorizeFromMeUser(to.userId);
            }
        },

        OnBasicContactInfoReceived: function (errorCode, userIds, contacts) {
            if(errorCode == uc.ErrorCode.Success){
                uc.modules.meeting.MeetingHeader.updateAuthorizeUsers(contacts);
            }
        }
    }
})(uc);