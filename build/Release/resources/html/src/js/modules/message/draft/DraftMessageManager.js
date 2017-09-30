/**
 * Created by waouyang on 15/11/18.
 */
(function(uc){
    uc.modules.draft.DraftMessageManager = {
        draftMessageDataSet: new uc.modules.draft.DraftMessageDataSet(),
        /**
         * add draft message to data set
         * @param id        id of the chat, for single chat, it's userId, for group and meeting chat, it's group id
         * @param type      type of the chat, see uc.constants.Conversation.ConversationType
         * @param message   draft message to save
         */
        addDraftMessage: function(id, type, message){
            uc.modules.draft.DraftMessageManager.draftMessageDataSet.addDraftMessage(id, type, message);
        },
        /**
         * remove draft message to data set
         * @param id        id of the chat, for single chat, it's userId, for group and meeting chat, it's group id
         * @param type      type of the chat, see uc.constants.Conversation.ConversationType
         */
        removeDraftMessage: function(id, type){
            uc.modules.draft.DraftMessageManager.draftMessageDataSet.removeDraftMessage(id, type);
        },
        /**
         * update draft message to data set
         * @param id        id of the chat, for single chat, it's userId, for group and meeting chat, it's group id
         * @param type      type of the chat, see uc.constants.Conversation.ConversationType
         * @param message   draft message to save
         */
        updateDraftMessage: function(id, type, message){
            uc.modules.draft.DraftMessageManager.draftMessageDataSet.updateDraftMessage(id, type, message);
        },
        /**
         * get draft message
         * @param id        id of the chat, for single chat, it's userId, for group and meeting chat, it's group id
         * @param type      type of the chat, see uc.constants.Conversation.ConversationType
         */
        getDraftMessage: function(id, type){
            return uc.modules.draft.DraftMessageManager.draftMessageDataSet.getDraftMessage(id, type);
        },

        clear: function(){
            uc.modules.draft.DraftMessageManager.draftMessageDataSet.clear();
        },

        getAllDraftMessage: function(){
            uc.modules.draft.DraftMessageManager.draftMessageDataSet.getAllDraftMessage();
        }
    }
})(uc);