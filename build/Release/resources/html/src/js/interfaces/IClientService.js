/**
 * Created by senlin.ouyang on 2015/7/17.
 */
(function(uc){
    uc.interfaces.IClientService = {
        /**
         * login UCC Server
         * @param account login account
         * @param password login password
         */
        login: function(/*String*/account, /*String*/password, accounttype){},
        /**
         * logout UCC Server
         */
        logout: function(){},

        reLogin: function(){},

        /**
         * login UCAS after login UCC Server
         */
        loginUCAS: function(){},
        /**
         * logout UCAS
         */
        logoutUCAS: function(){},
        /**
         *
         */
        quit: function(){},
        /**
         * get local conversation list after login UCC Server, before get conversation list from server
         * @param count  conversation count to get from local
         */
        getLocalConversationList: function(/*int*/count){},
        /**
         * get conversation list from server
         * @param count conversation count to get from server
         */
        syncConversationList: function(/*int*/count){},
        /**
         * get conversation list when pull down the conversation list
         * @param startTime
         * @param count
         */
        getHistoryConversationList: function(/*int*/startTime, /*int*/count){},
        /**
         * get data directory, if not exist, will create it
         * @param type  directory type, audio, video, image, file etc
         */
        getDataDirectory: function(type){},
        /**
         * get history login account info
         * @return login account list
         */
        getHistoryLoginAccounts: function(){},
        /**
         *delete history login account info
         */
        deleteHistoryAccount:function(/*String*/account){},
        /**
         *remember password
         * 0 is close,1 is open(default is 0)
         */
        rememberPassword:function(/*String*/account,/*boolean*/checked){},
        /**
         *auto login
         * 0 is close,1 is open(default is 1)
         */
        setAutoLogin:function(/*String*/account,/*boolean*/checked){},

        /**
         * reset password
         * @param oldPassword old password to verify
         * @param newPassword new password to set
         */
        modifyPassword: function(/*String*/oldPassword, /*String*/newPassword){},
        getCaptchaURL: function(hashStr, callback){},
        verifyAccount: function (verifyType,account, captcha, hashStr){},
        verifyCode: function (verifyType,account, mobileCode, resend){},
        resetPassword: function (account, newPwd, mobileCode){},
        getCurrentUCID: function(){},
        setProxyAuthInfo: function(authType, authAccount, authPassword, callback){},
        updateClient: function(){},
        getConfigInfo: function(app, name, callback){},
        getConfigInfos: function(app, callback){},
        updateConfigInfo: function(app, name, value, callback){},
        updateConfigInfos: function(app, values, callback){},
        getPwdExpiredDays: function(callback){},
        enableAutoStart: function(){},
        disableAutoStart: function(){},
        checkUpdates: function(callback){},
        update: function(){},
        archiveLogFile: function(){},
        uploadLog: function(logURL, desc, version){},
        getClientVersion: function(){},
        getPasswordRule: function(callback){},
        getSessionId: function(callback){},
        isAutoStart:function(callback){},
        getGkClientConnection:function(callback){},
        getGkClientReLogin:function(){},
        getWindowStatus: function(){},
        getTrayPos: function(){},
	    getUccServerDomain:function(callback){},
        getDefaultFileSavePath:function(){},
        getMaxGroupMemberLimit:function(){},
        getMinGroupMemberLimit:function(){},
	    getWindowHwnd:function(wname){},
        setConversationTop: function (conversationType, conversationId, isTop) {},
        getConversationInfoSync: function (conversationType, conversationId) {},
        setUpdateUserNotify:function(isNotify){},
        getUserNotifyStatus:function(){},
        /**
         * getWindowPosition
         * @param  return array [topLeft.x, topLeft.y,  bottomRight.x, bottomRight.y]
         */
        getWindowPosition: function() {}
    }
})(uc);