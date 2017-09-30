(function(uc){
    uc.modules.pbx.PbxDataSet = function(){
        this.PbxMap = new uc.util.HashMap();
    };
    jQuery.extend(uc.modules.pbx.PbxDataSet.prototype, {
        addPbx: function(pbxNum,pbxInfo){
            var key = this.toKey(pbxNum);
            if(this.PbxMap.isExist(key)){
                this.updatePbx(pbxNum, pbxInfo);
            }else{
                this.PbxMap.put(key, pbxInfo);
            }
        },
        removePbx: function(pbxNum){
            var key = this.toKey(pbxNum);
            this.PbxMap.remove(key);
        },
        updatePbx: function(pbxNum, pbxInfo){
            var key = this.toKey(pbxNum);
            if(this.PbxMap.isExist(key)){
                this.PbxMap.remove(key);
                this.PbxMap.put(key, pbxInfo);
            }else{
                this.addPbx(pbxNum, pbxInfo);
            }
        },
        getPbx: function(pbxNum){
            return this.PbxMap.get(this.toKey(pbxNum));
        },
        isPbxExist: function(pbxNum){
            return this.PbxMap.isExist(this.toKey(pbxNum));
        },
        getAllPbx: function(){
            return this.PbxMap.convertToArray();
        },
        getPbxCount: function(){
            return this.PbxMap.size();
        },
        clear: function(){
            this.PbxMap.clear();
        },
        toKey: function(pbxNum){
            return pbxNum + '';
        }
    })
})(uc);