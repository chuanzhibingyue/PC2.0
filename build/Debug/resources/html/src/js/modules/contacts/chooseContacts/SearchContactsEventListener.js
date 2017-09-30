/**
 * Created by waouyang on 15/8/12.
 */
(function(uc){
    uc.modules.chooseContacts.SearchContactsEventListener = function(){

    };
    jQuery.extend(uc.modules.chooseContacts.SearchContactsEventListener.prototype,
        uc.interfaces.IUIEvent,
        uc.interfaces.ISearchEvent, {
            OnServerSearchResultReceived: function(errorCode, searchId, contacts, projects, groupChats, cloudGroups, meetings, messages){
                uc.modules.chooseContacts.SearchContactsManager.afterSearch();
                uc.modules.chooseContacts.SearchContactsManager.displaySearchResults(searchId, contacts, projects, groupChats);
            },
            OnLocalSearchResultReceived: function(errorCode, searchId, contacts, groups){
            }
    })
})(uc);