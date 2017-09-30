/**
 * Created by senlin.ouyang on 2015/7/20.
 */
(function(uc){
	uc.modules.group.GroupDataSet = function(){
        this.groupMap = new uc.util.HashMap();
    };
    jQuery.extend(uc.modules.group.GroupDataSet.prototype, {
    	addGroup: function(groupId, groupInfo){
            groupId = this.toKey(groupId);
            if(!this.groupMap.isExist(groupId)){
                this.groupMap.put(groupId, groupInfo);
            }else{
                this.updateGroup(groupId, groupInfo);
            }
        },
        removeGroup: function(groupId){
            this.groupMap.remove(this.toKey(groupId));
        },
        updateGroup: function(groupId, groupInfo){
            groupId = this.toKey(groupId);
            if(this.groupMap.isExist(groupId)){
                this.groupMap.remove(groupId);
                this.groupMap.put(groupId, groupInfo);
            }else{
                this.addGroup(groupId, groupInfo);
            }
        },
        getGroupInfo: function(groupId){
            return this.groupMap.get(this.toKey(groupId));
        },
        isGroupExist: function(groupId){
            return this.groupMap.isExist(this.toKey(groupId));
        },
        getAllGroups: function(){
            return this.groupMap.convertToArray();
        },
        getGroupsCount: function(){
            return this.groupMap.size();
        },
        clear: function(){
            this.groupMap.clear();
        },
        toKey: function(groupId){
            return '' + groupId;
        }
    })
})(uc);