/**
 * Created by senlin.ouyang on 2015/7/17.
 */
(function(uc){
    uc.interfaces.IContactService = {
        /**
         * sync contact list after login
         */
        syncContactList: function(){},

        /**
         * get basic contact info from SDK, for single user, if contact info is in SDK, will return BasicContactInfo,
         * otherwise will return it via OnContactInfoReceived, for user array, will return via OnContactInfoReceived event
         * @param userId
         */
        getBasicContactInfo: function(userId){},

        /**
         *  get contact info from SDK, for single user, if contact info is in SDK, will return ContactInfo, otherwise will
         *  return it via OnContactInfoReceived, for user array, will return via OnContactInfoReceived event
         * @param userId or user id array
         */
        getContactInfo: function(userId){},
        /**
         * get contact list from SDK, only for my contacts
         * @param start     start index
         * @param count     count of contact info, if is 0, get all the contacts
         * @param callback  callback(args): args[0]: errorCode, args[1]: contacts array
         *                   contact: userId, siteId, cardVersion, status, displayName, pinyin, sex, account, email, mobile
         *                           workPhone, localAvatar, remoteAvatarUrl, personalSign, departmentId, departmentName,
         *                           position, superiorId, superiorName, colleagueNum, type, tags
         */
        getContactList: function(start, count, callback){},
        /**
         * set contact's local avatar url after avatar is downloaded from server
         * @param userId            userId
         * @param serverAvatarUrl   server avatar url
         * @param localAvatarUrl    local avatar url
         */
        setContactLocalAvatarUrl: function(userId, serverAvatarUrl, localAvatarUrl){},
        /**
         * add a contact
         * @param userId    user id to add
         */
        addContact: function(userId){},
        /**
         * delete a contact
         * @param userId    user id to delete
         */
        deleteContact: function(userId){},
        /**
         * search local contact
         * @param type      contact type
         * @param keyword   keyword to search
         * @param callback  callback(args): args[0]: errorCode, args[1]: contacts array
         */
        searchLocalContact: function(type, keyword, callback){},
        /**
         * update user tags
         * @param tagsArray tags array: for example [{id: 1, name: 'study'}, {id: 3, name:'play game'}]
         */
        updateCurrentUserTags: function(tagsArray){},
        /**
         * get current login user info
         */
        getCurrentUserInfo: function(){},        
        //ucClient.ContactService.updateUserInfo('{"personal_sign":"tttttt"}');
        updateUserInfo: function(/*string*/userJsonString){},
        /**
         * get colleagues, result will be passed by OnColleagueReceived
         * @param userId    userId
         */
        getColleagues: function(userId){},
        /**
         * get organization info for given org id
         * @param orgId org id to get
         */
        getOrgInfo: function(orgId){},
        /**
         * get organization & Sub organizations Members for given org id; if exceed limitedCount, exception.
         * @param orgId org id to get
         * @param limitedCount  if exceed limitedCount, exception.
         */
        getOrgAllMembers: function(orgId, limitedCount,copyBack){},
        updateCustomTags:function(){},
        /*
         * set or subscribe or unsubscribe users status.
         */
        setUserStatus:function(/*int*/userId, /*int*/status, /*string*/promptMessage){},
        subscribeUserStatus:function(/*array[int]*/userIds){},
        unsubscribeUserStatus:function(/*array[int]*/userIds){},
        unsubscribeAllUserStauts:function(){},
	    getUserStatus:function(userIds,copyback,callback){},
        getCurrentUserFullInfo:function(){}
    }
})(uc);