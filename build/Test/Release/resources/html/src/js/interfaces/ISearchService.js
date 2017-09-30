/**
 * Created by waouyang on 15/8/17.
 */
(function(uc){
    uc.interfaces.ISearchService = {
        /**
         * search result from server
         * @param keyword   keyword to search
         * @param page      page count to search
         * @param count     result count per page
         * @param scope     scope array to search, for example [1, 2, 4]
         * @param callback  callback of the call,
         *                   args[0] errorCode, args[1] search id
         */
        searchServer: function(keyword, page, count, scope, callback){},
        /**
         * search data from local
         * @param keyword   keyword to search
         * @param page      page count to search
         * @param count     result count per page
         * @param scope     scope array to search, for example [1, 2, 4]
         * @param callback  callback of the call, args[0] errorCode, args[1] search id
         */
        searchLocal: function(keyword, page, count, scope, callback){},
         /**
         * search result from server
         * @param chatType  conversationType currentChatType
         * @param chatId    currentChatId  
         * @param keyword   search keyword
         * @param pageIndex  search  pageIndex
         * @param pageSize    search pageSize to search, for example [1, 2, 3]
         * @param callback  callback of the call,
         * args[0] errorCode, args[1] search  id for result
         */
        searchHistoryMsgByKeyWord:function(chatType,chatId,keyword,pageIndex,pageSize,callback){},
        /* request History NextResult */ 
		    searchHistoryByNexeResult:function(chatType,chatId,count,beginTime,endTime,frontOrback,callback){}
    }
})(uc);