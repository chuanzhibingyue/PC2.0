/**
 * Created by zhen.xu on 2015/9/18.
 */
(function (uc) {
    uc.interfaces.IMicroblogService = {
        addAttention: function (userId) {},
        cancelAttention: function (userId) {},
        getHomePageURL: function (userId, callback) {},
        getNewMsgCount: function () {},
        getRelation: function (userId, callback) {},
        getRelationSync: function(userId){},
        /**
         * get user's microblog home page, for not current user.
         * @param currentUserId
         * @param userId	the user you want to visit his or her home page
         * @param callback 	callback(args): args[0]: errorCode, args[1]: url of user's home page
         */
        getUserhomePageURL: function(currentUserId, userId, callback){}
    };
})(uc);