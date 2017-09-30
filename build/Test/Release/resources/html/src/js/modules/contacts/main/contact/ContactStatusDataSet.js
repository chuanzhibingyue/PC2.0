/**
 * Created by xue.bai_2 on 2016/5/11.
 */
(function(uc){
    uc.modules.contact.ContactStatusDataSet = function(){
        this.contactStatusMap = new uc.util.HashMap();
    };
    jQuery.extend(uc.modules.contact.ContactStatusDataSet.prototype, {
        addContacts: function(panelId, userIdsArr){
            panelId = this.toKey(panelId);
            if(this.contactStatusMap.isExist(panelId)){
                this.contactStatusMap.remove(panelId);
            }
            this.contactStatusMap.put(panelId, userIdsArr);
        },
        getContacts: function(panelId){
            return this.contactStatusMap.get(this.toKey(panelId));
        },
        toKey: function(panelId){
            return '' + panelId;
        }
    })
})(uc);