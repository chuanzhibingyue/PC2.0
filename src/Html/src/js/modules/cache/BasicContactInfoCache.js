/**
 * Created by waouyang on 16/1/20.
 */
(function(uc){
    uc.modules.cache.BasicContactInfoCache = {
        basicContactInfoMap : new uc.util.HashMap(),

        getBasicContactInfo: function(userId){
            var key = this.toKey(userId);
            var contactInfo = this.basicContactInfoMap.get(key);
            if(!contactInfo){
                contactInfo = uc.IContactService.getBasicContactInfo(userId);
                if(contactInfo){
                    this.basicContactInfoMap.put(key, contactInfo);
                }
            }
            return contactInfo;
        },

        getMultipleBasicContactInfo: function(userIdArray){
            var contactInfoArray = [], contactInfo, userId, noCacheUserIdArray = [];
            for(var i = 0, len = userIdArray.length; i < len; i++){
                userId = userIdArray[i];
                contactInfo = this.basicContactInfoMap.get(this.toKey(userId));
                if(contactInfo){
                    contactInfoArray.push(contactInfo);
                }else {
                    noCacheUserIdArray.push(userId);
                }
            }
            if(noCacheUserIdArray.length){
                uc.IContactService.getBasicContactInfo(noCacheUserIdArray);
            }
            return contactInfoArray;
        },

        OnInitClient: function(clearData){
            if(clearData){
                this.basicContactInfoMap.clear();
            }
        },

        OnUserAvatarDownloaded: function(userId, serverAvatarUrl, localAvatarUrl){
            var key = this.toKey(userId);
            var contactInfo = this.basicContactInfoMap.get(key);
            if(contactInfo){
                contactInfo.localAvatar = localAvatarUrl;
                this.basicContactInfoMap.put(key, contactInfo);
            }
        },

        OnBasicContactInfoReceived: function(contacts){
            var contactInfo;
            for(var i = 0, len = contacts.length; i < len; i++){
                contactInfo = contacts[i];
                if(contactInfo){
                    this.basicContactInfoMap.put(this.toKey(contactInfo.userId), contactInfo);
                }
            }
        },

        OnContactUpdated: function(contacts){
            var contactInfo;
            for(var i = 0, len = contacts.length; i < len; i++){
                contactInfo = contacts[i];
                if(contactInfo){
                    this._updateContactInfoToCache(contactInfo);
                }
            }
        },

        OnCurrentUserInfoUpdated: function(userInfo){
            if(userInfo){
                this._updateContactInfoToCache(userInfo);
            }
        },

        /**
         * update contact info to cache, only update, not add
         * @param contactInfo
         */
        _updateContactInfoToCache: function(contactInfo){
            var key = this.toKey(contactInfo.userId);
            var basicContactInfo = this.basicContactInfoMap.get(key);
            if(basicContactInfo){
                for(var prop in basicContactInfo){
                    basicContactInfo[prop] = contactInfo[prop];
                }
                this.basicContactInfoMap.put(key, basicContactInfo);
            }
        },

        toKey: function(userId){
            return userId + '';
        }
    }
})(uc);