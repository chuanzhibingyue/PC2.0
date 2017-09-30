/**
 * Created by waouyang on 15/9/24.
 */
(function(uc){
    uc.modules.openApi.OpenApiDataSet = function(){
        this.openApiMap = new uc.util.HashMap();
    };
    jQuery.extend(uc.modules.openApi.OpenApiDataSet.prototype, {
        addOpenApiInfo: function(appId, appInfo){
            if(this.isOpenApiInfoExist(appId)){
                this.updateOpenApiInfo(appId, appInfo);
            }else{
                this.openApiMap.put(this.toKey(appId), appInfo);
            }
        },
        removeOpenApiInfo: function(appId){
            this.openApiMap.remove(this.toKey(appId));
        },
        updateOpenApiInfo: function(appId, appInfo){
            if(this.isOpenApiInfoExist(appId)){
                this.openApiMap.remove(this.toKey(appId));
                this.openApiMap.put(this.toKey(appId), appInfo);
            }else{
                this.addOpenApiInfo(appId, appInfo);
            }
        },
        isOpenApiInfoExist: function(appId){
            return this.openApiMap.isExist(this.toKey(appId));
        },
        getOpenApiInfo: function(appId, appInfo){
            return this.openApiMap.get(this.toKey(appId), appInfo);
        },
        getCount: function(){
            return this.openApiMap.size();
        },
        getAllOpenApiInfo: function(){
            return this.openApiMap.convertToArray();
        },
        clear: function(){
            this.openApiMap.clear();
        },
        toKey: function(appId){
            return '' + appId;
        }
    })
})(uc);