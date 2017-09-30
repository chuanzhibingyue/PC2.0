/**
 * Created by waouyang on 15/8/15.
 */
(function(uc){
    var _this = null;
    var startTime, endTime;
    uc.modules.search.SearchManager = {
        init: function(){
            _this = this;
            if(this.initialized){
                return;
            }
            uc.modules.search.Search.init();
            this.searchId = 0;
            this.initialized = true;
        },
        OnInitClient: function(clearData){
            this.init();
        },
        OnStartSearch: function(){
            uc.modules.search.Search.showSearchView();
        },
        OnServerSearchResultReceived: function(errorCode, searchId, contacts, projects, groups, cloudGroups, meetings, messages){
            if(searchId == this.searchId){
                endTime = new Date();
                if(errorCode == uc.ErrorCode.Success){
                    uc.modules.search.Search.OnServerSearchResultReceived(contacts, projects, groups, cloudGroups, meetings, messages);
                }else{
                    uc.modules.search.Search.OnSearchError();
                }
            }
        },
        OnUserAvatarDownloaded: function(userId, serverAvatarUrl, localAvatarUrl){
            uc.modules.search.Search.OnUserAvatarDownloaded(userId, serverAvatarUrl, localAvatarUrl);
        },
        OnProjectAvatarDownloaded: function(groupId, serverAvatarUrl, localAvatarUrl){
            uc.modules.search.Search.OnProjectAvatarDownloaded(groupId, serverAvatarUrl, localAvatarUrl);
        },
        OnContactInfoReceived: function(userIds, contacts){
            uc.modules.search.Search.OnContactInfoReceived(userIds, contacts);
        },
        OnGroupInfoReceived: function(groupId, group){
            uc.modules.search.Search.OnGroupInfoReceived(groupId, group);
        },
        searchServer: function(keyword){
            var searchScope = [uc.constants.Search.SearchScope.Contact, uc.constants.Search.SearchScope.Project,
                                uc.constants.Search.SearchScope.Meeting,
                                uc.constants.Search.SearchScope.Group];
            uc.ISearchService.searchServer(keyword, uc.constants.Search.SearchPage, uc.constants.Search.SearchCountPerPage, searchScope, function(args){
                var errorCode = args[0];
                var searchId = args[1];
                if(errorCode == uc.ErrorCode.Success){
                    _this.searchId = searchId;
                }else{
                    uc.modules.search.Search.OnSearchError();
                }
                startTime = new Date();
            })
        }
    }
})(uc);