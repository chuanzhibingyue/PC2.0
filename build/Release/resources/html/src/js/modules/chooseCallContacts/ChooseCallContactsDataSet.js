/**
 * Created by senlin.ouyang on 2015/7/20.
 */
(function(uc){
    uc.modules.call.ChooseCallContactsDataSet = function(){
        this.contactMap = new uc.util.HashMap();
    };
    jQuery.extend(uc.modules.call.ChooseCallContactsDataSet.prototype, {
    	addContact: function(userId, contactInfo){
            userId = this.toKey(userId);
            if(!this.contactMap.isExist(userId)){
                this.contactMap.put(userId, contactInfo);
            }else{
                this.updateContact(userId, contactInfo);
            }
        },
        deleteContact: function(userId){
            this.contactMap.remove(this.toKey(userId));
        },
        updateContact: function(userId, contactInfo){
            userId = this.toKey(userId);
            if(this.contactMap.isExist(userId)){
                this.contactMap.remove(userId);
                this.contactMap.put(userId, contactInfo);
            }else{
                this.addContact(userId, contactInfo);
            }
        },
        getContactInfo: function(userId){
            return this.contactMap.get(this.toKey(userId));
        },
        isContactExist: function(userId){
            return this.contactMap.isExist(this.toKey(userId));
        },
        getAllContacts: function(){
            return this.contactMap.convertToArray();
        },
        getContactsCount: function(){
            return this.contactMap.size();
        },
        clear: function(){
            this.contactMap.clear();
        },
        toKey: function(userId){
            return '' + userId;
        }
    })
})(uc);