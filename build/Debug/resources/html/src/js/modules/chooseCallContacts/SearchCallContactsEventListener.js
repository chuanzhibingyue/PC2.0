/**
 * Created by zhen.xu.
 * User: zhen.xu
 * Date: 15-12-15 下午5:09
 */
(function(uc){
    uc.modules.call.SearchCallContactsEventListener = function(){

    };
    jQuery.extend(uc.modules.call.SearchCallContactsEventListener.prototype,
        uc.interfaces.ISearchEvent, {
            OnServerSearchResultReceived: function(errorCode, searchId, contacts, projects, groupChats, meetings, messages){
                uc.modules.call.ChooseCallContactsManager.OnServerSearchResultReceived(errorCode, searchId, contacts);
            }
    })
})(uc);