/**
 * Created by waouyang on 15/11/18.
 */
(function(uc){
    uc.modules.draft.DraftMessageDataSet = function(){
        this.draftMessageMap = new uc.util.HashMap();
    };
    jQuery.extend(uc.modules.draft.DraftMessageDataSet.prototype,{
        addDraftMessage: function(id, type, message){
            var key = this.toKey(id, type);
            this.draftMessageMap.put(key, message);
        },
        removeDraftMessage: function(id, type){
            var key = this.toKey(id, type);
            if(this.draftMessageMap.isExist(key)){
                this.draftMessageMap.remove(key);
            }
        },
        updateDraftMessage: function(id, type, message){
            var key = this.toKey(id, type);
            this.draftMessageMap.put(key, message);
        },
        getDraftMessage: function(id, type){
            return this.draftMessageMap.get(this.toKey(id, type));
        },
        clear: function(){
            this.draftMessageMap.clear();
        },
        toKey: function(id, type){
            return id + "_" + type;
        },
        getAllDraftMessage: function () {
            return this.draftMessageMap.convertToArray();
        }
    })
})(uc);