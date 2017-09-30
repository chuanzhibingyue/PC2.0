/**
 * Created by senlin.ouyang on 2015/7/21.
 */
(function(uc){
    uc.modules.conversation.ConversationDataSet = function(){
        this.conversationMap = new uc.util.HashMap();
        this.topConversationMap = new uc.util.HashMap();
    };
    jQuery.extend(uc.modules.conversation.ConversationDataSet.prototype, {
        addConversation: function(conversationId, conversationType, conversationInfo){
            var key = this.toKey(conversationId, conversationType);

            if (conversationInfo.topIndex && conversationInfo.topIndex > 0) {
                //top index conversation
                this.updateTopConversation(conversationId, conversationType, conversationInfo);
            } else {
                //not top conversation
                this.conversationMap.put(key, conversationInfo);
                this.topConversationMap.remove(key);
            }
        },
        removeConversation: function(conversationId, conversationType){
            var key = this.toKey(conversationId, conversationType);
            this.conversationMap.remove(key);
            this.topConversationMap.remove(key);
        },
        updateConversation: function(conversationId, conversationType, conversationInfo){
            var key = this.toKey(conversationId, conversationType);
            if(this.conversationMap.isExist(key)){
                if (conversationInfo.topIndex && conversationInfo.topIndex > 0) {
                    this.updateTopConversation(conversationId, conversationType, conversationInfo);
                } else {
                    this.conversationMap.remove(key);
                    this.topConversationMap.remove(key);
                    this.conversationMap.put(key, conversationInfo);
                }
            }else{
                if (conversationInfo.topIndex && conversationInfo.topIndex > 0) {
                    this.updateTopConversation(conversationId, conversationType, conversationInfo);
                } else {
                    this.addConversation(conversationId, conversationType, conversationInfo);
                }
            }
        },
        updateTopConversation: function(conversationId, conversationType, conversationInfo){
            var key = this.toKey(conversationId, conversationType);
            if(this.topConversationMap.isExist(key)){
                this.topConversationMap.remove(key);
                this.conversationMap.remove(key);
                this.topConversationMap.put(key, conversationInfo);
            }else{
                if(this.conversationMap.isExist(key)){
                    this.conversationMap.remove(key);
                }
                this.topConversationMap.put(key, conversationInfo);
            }
        },
        getConversation: function(conversationId, conversationType){
            var notTopConversation = this.conversationMap.get(this.toKey(conversationId, conversationType));
            var topIndexConversation = this.topConversationMap.get(this.toKey(conversationId, conversationType));
            return notTopConversation ? notTopConversation : topIndexConversation;
        },
        isConversationExist: function(conversationId, conversationType){
            var isNotTop = this.conversationMap.isExist(this.toKey(conversationId, conversationType));
            var isTopIndex = this.topConversationMap.isExist(this.toKey(conversationId, conversationType));
            return isNotTop ? isNotTop : isTopIndex;
        },
        getAllConversations: function(){
            return this.topConversationMap.convertToArray().concat(this.conversationMap.convertToArray());
        },
        getAllNotTopConversations: function(){
            return this.conversationMap.convertToArray();
        },
        getAllTopIndexConversations: function(){
            return this.topConversationMap.convertToArray();
        },
        getConversationCount: function(){
            return (this.conversationMap.size() + this.topConversationMap.size());
        },
        clear: function(){
            this.conversationMap.clear();
            this.topConversationMap.clear();
        },
        toKey: function(conversationId, conversationType){
            return conversationId + '|' + conversationType;
        }
    })
})(uc);