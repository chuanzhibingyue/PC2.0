/**
 * Created by senlin.ouyang on 2015/7/17.
 */
(function(uc){
    uc.interfaces.IClientEvent = {
        /**
         * login result event when login to UCC Server
         * @param errorCode     error code of the login operation
         * @param authType      proxy authenticate type
         * @param pbxType       pbx type
         * @param capability
         * @constructor
         */
        OnLogin: function(/*int*/errorCode, authType, pbxType, capability){},
        /**
         * logout result event when logout UCC Server
         * @param errorCode error code of the logout operation
         * @constructor
         */
        OnLogout: function(/*int*/errorCode){},
        /**
         * login UCAS event
         * @param errorCode error code of login operation
         * @constructor
         */
        OnUCASLogin: function(/*int*/errorCode){},
        /**
         * logout UCAS event
         * @param errorCode error code of login operation
         * @constructor
         */
        OnUCASLogout: function(/*int*/errorCode){},
        /**
         * event for session expired
         * @constructor
         */
        OnSessionExpired: function(){},
        /**
         * forget password result event
         * @param errorCode
         * @constructor
         */
        OnPasswordReset: function(/*int*/errorCode){},
        /**
         * modify password event
         * @param errorCode
         */
        OnPasswordModified: function(/*String*/errorCode){},
        /**
         * password modified message
         * @constructor
         */
        OnPasswordModifiedMessageReceived: function(){},
        /**
         * password rule updated event
         * @param type new password rule 1）8~30位，不限制类型； 2）8~30位，数字或字母组合
         */
        OnPasswordRuleUpdatedMessageReceived: function(/*int*/type){},
        /**
         * password expire or about to expire event
         * @param dueDays   days to expired
         */
        OnPasswordExpiredMessageReceived: function(/*int*/dueDays){},

        /**
         * event for verify account
         * @param errorCode
         * @param mobile
         */
        OnAccountVerified: function(errorCode, verifyType, mobile, email, exprised){},
        /**
         * event for verify code
         * @param errorCode
         */
        OnCodeVerified: function(errorCode, exprised){},


        /**
         * conversation list received event
         * @param errorCode     error code
         * @param isLocal       is this conversation list from local data
         * @param conversations conversation array
         * @param totalUnread   total Unread message count
         */
        OnConversationListReceived: function(/*int*/errorCode, /*boolean*/isLocal, /*Array*/conversations, totalUnread, groupAtCounts){},

        /**
         * conversation list complete event
         * @param conversationCount total count of the conversations list from SDK
         */
        OnConversationListComplete: function(/*int*/conversationCount){},

        OnPlayingSoundNotification: function(id, percent, seconds){},
        OnNetworkStatusChanged: function(status){},
        OnUpdatesStatus:function(errorCode,updateState,version,notes){},
        OnLogUploaded:function(errorCode){},
        OnCodeResent:function(errorCode){},
        OnUpdatesProgress:function(checkProgress){},
        OnArchiveLogFinished:function(filePath){},
        OnMouseMoveInTray: function(){},
        OnMouseMoveOutTray: function(){},
        OnWindowActiveStatus: function () {},
	    OnCloudLogin:function(errorCode){},
        OnConversationSetTopReceived: function (errorCode, message) {},
        OnFileSendToBee:function(filePath){},
        OnUserLoginNotify:function(errorCode,message){},
        OnUpdateUserNotifySet:function(errorCode,userId){},
        OnGetUserNotifyStatus:function(errorCode,isNotify){}
    }
})(uc);