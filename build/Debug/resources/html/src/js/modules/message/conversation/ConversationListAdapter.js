/**
 * Created by senlin.ouyang on 2015/7/21.
 */
(function(uc){
    uc.modules.conversation.ConversationListAdapter = function(conversationManager){
        this.conversationManager = conversationManager;
        this.conversationInfoArray = null;
        this.dataSetObserver = null;
    };
    jQuery.extend(uc.modules.conversation.ConversationListAdapter.prototype, {
        init: function(){
            this.conversationInfoArray = this.conversationManager.getSortedConversationList(0, -1);
        },
        setDataSetObserver: function(dataSetObserver){
            this.dataSetObserver = dataSetObserver;
        },
        notifyDataSetChanged: function(){
            this.conversationInfoArray = this.conversationManager.getSortedConversationList(0, -1);
            this.dataSetObserver.notifyDataSetChanged();
        },

        getCount: function(){
            return this.conversationInfoArray.length;
        },

        getConversationInfo: function(index){
            return this.conversationInfoArray[index];
        }
    })
})(uc);