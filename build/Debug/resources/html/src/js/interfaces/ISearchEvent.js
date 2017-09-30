/**
 * Created by waouyang on 15/8/17.
 */
(function(uc){
    uc.interfaces.ISearchEvent = {
        /**
         * search result event from server
         * @param errorCode
         * @param searchId
         * @param contacts
         * @param projects
         * @param groups
         * @param meetings
         * @param messages
         */
        OnServerSearchResultReceived: function(errorCode, searchId, contacts, projects, groups, cloudGroups, meetings, messages,chatMessages,chatMsgNum){},
        /**
         * search result event from local
         * @param errorCode
         * @param searchId
         * @param contacts
         * @param groups
         */
        OnLocalSearchResultReceived: function(errorCode, searchId, contacts, groups){},
        /**
        show searchHistroy  NextResult 
        */
        OnSearchNextResultReceived:function(errorCode, conversationType,groupId, msgCount,minmsgtime,maxmsgTime){}
    }
})(uc);