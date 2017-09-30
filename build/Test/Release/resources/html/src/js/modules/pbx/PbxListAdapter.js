(function(uc){
    uc.modules.pbx.PbxListAdapter= function(pbxManager){
        this.pbxManager = pbxManager;
        this.pbxInfoArray = null;
        this.dataSetObserver = null;
    };
    jQuery.extend(uc.modules.pbx.PbxListAdapter.prototype, {
        init: function(){
            this.pbxInfoArray = this.pbxManager.getSortedPbxCallList(0, -1);
        },
        setDataSetObserver: function(dataSetObserver){
            this.dataSetObserver = dataSetObserver;
        },
        notifyDataSetChanged: function(){
            this.pbxInfoArray = this.pbxManager.getSortedPbxCallList(0, -1);
            this.dataSetObserver.notifyDataSetChanged();
        },
        getCount: function(){
            return this.pbxInfoArray.length;
        },
        getPbxInfo: function(index){
            return this.pbxInfoArray[index];
        }
    })
})(uc);