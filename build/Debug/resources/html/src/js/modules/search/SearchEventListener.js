/**
 * Created by waouyang on 15/8/12.
 */
(function(uc){
    uc.modules.search.SearchEventListener = function(){

    };
    jQuery.extend(uc.modules.search.SearchEventListener.prototype,
        uc.interfaces.IUIEvent,
        uc.interfaces.IContactEvent,
        uc.interfaces.IGroupEvent,
        uc.interfaces.ISearchEvent, {
            OnInitClient: function(clearData){
                uc.modules.search.SearchManager.OnInitClient(clearData);
            },
            OnStartSearch: function(){
                uc.modules.search.SearchManager.OnStartSearch();
            },
            OnUserAvatarDownloaded: function(userId, serverAvatarUrl, localAvatarUrl){
                uc.modules.search.SearchManager.OnUserAvatarDownloaded(userId, serverAvatarUrl, localAvatarUrl);
            },
            OnProjectAvatarDownloaded: function(groupId, serverAvatarUrl, localAvatarUrl){
                uc.modules.search.SearchManager.OnProjectAvatarDownloaded(groupId, serverAvatarUrl, localAvatarUrl);
            },
            OnServerSearchResultReceived: function(errorCode, searchId, contacts, projects, groups, cloudGroups, meetings, messages,chatMessages,chatMessagesNum){
                uc.modules.search.SearchManager.OnServerSearchResultReceived(errorCode, searchId, contacts, projects, groups, cloudGroups, meetings, messages);
                if ($.fn.searchInput) {
                    if(chatMessagesNum>0)
                    {
                        $.fn.searchInput.OnServerSearchResultReceivedByPaging(errorCode, searchId, chatMessages,chatMessagesNum );
                    }
                    else {
                        $.fn.searchInput.OnServerSearchResultReceived(errorCode, searchId, contacts, projects, groups, cloudGroups, meetings, messages);
                    }
                }
            },
            OnLocalSearchResultReceived: function(errorCode, searchId, contacts, groups){
            },
            OnContactInfoReceived: function(errorCode, userIds, contacts){
                if(errorCode == uc.ErrorCode.Success){
                    uc.modules.search.SearchManager.OnContactInfoReceived(userIds, contacts);
                }
            },
            OnGroupInfoReceived: function(errorCode, groupId,mountId ,group){
                if(errorCode == uc.ErrorCode.Success){
                    uc.modules.search.SearchManager.OnGroupInfoReceived(groupId, group);
                }
            }
    })
})(uc);