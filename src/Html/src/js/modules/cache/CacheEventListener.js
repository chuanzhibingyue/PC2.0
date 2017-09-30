/**
 * Created by waouyang on 16/1/20.
 */
(function(uc){
    uc.modules.cache.CacheEventListener = function(){

    };
    jQuery.extend(uc.modules.cache.CacheEventListener.prototype,
        uc.interfaces.IUIEvent,
        uc.interfaces.IContactEvent, {
            /**IUIEvent*/
            OnInitClient: function(clearData){
                uc.modules.cache.BasicContactInfoCache.OnInitClient(clearData);
            },
            /**IContactEvent*/
            OnUserAvatarDownloaded: function(userId, serverAvatarUrl, localAvatarUrl){
                uc.modules.cache.BasicContactInfoCache.OnUserAvatarDownloaded(userId, serverAvatarUrl, localAvatarUrl);
            },

            OnBasicContactInfoReceived: function(errorCode, userIds, contacts){
                if(errorCode == uc.ErrorCode.Success){
                    uc.modules.cache.BasicContactInfoCache.OnBasicContactInfoReceived(contacts);
                }
            },

            OnContactUpdated: function(contacts){
                uc.modules.cache.BasicContactInfoCache.OnContactUpdated(contacts);
            },

            OnCurrentUserInfoUpdated: function(errorCode, userInfo){
                if(errorCode == uc.ErrorCode.Success){
                    uc.modules.cache.BasicContactInfoCache.OnCurrentUserInfoUpdated(userInfo);
                }
            }
    })
})(uc);