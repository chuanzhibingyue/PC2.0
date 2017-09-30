/**
 * Created by senlin.ouyang on 2015/7/21.
 */
(function(uc){
    uc.modules.contact.ContactListAdapter = function(){
        this.contactInfoArray = null;
        this.dataSetObserver = null;
    };
    jQuery.extend(uc.modules.contact.ContactListAdapter.prototype, {
        init: function(){
            this.contactInfoArray = uc.modules.contact.ContactManager.getSortedConversationList(0, -1);
        },
        setDataSetObserver: function(dataSetObserver){
            this.dataSetObserver = dataSetObserver;
        },

        notifyDataSetChanged: function(){
            this.contactInfoArray = uc.modules.contact.ContactManager.getSortedConversationList(0, -1);
            this.dataSetObserver.notifyDataSetChanged();
        },

        getCount: function(){
            return this.contactInfoArray.length;
        },

        getContactInfo: function(index){
            return this.contactInfoArray[index];
        }
    })
})(uc);