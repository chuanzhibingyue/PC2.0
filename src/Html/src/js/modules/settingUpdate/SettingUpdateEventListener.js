/**
 * Created by pan.li on 2015/9/17.
 */

(function(uc){
    uc.modules.settingUpdate.SettingUpdateEventListener = function(){

    };
    jQuery.extend(uc.modules.settingUpdate.SettingUpdateEventListener.prototype,
        uc.interfaces.IClientEvent,  {
            OnUpdatesStatus: function(errorCode,updateFlag,version,notes){
                uc.util.LogUtil.info("SettingUpdate", "OnUpdatesStatus", "errorCode"+errorCode+"updateFlag"+updateFlag+"version"+version, null);
                uc.modules.settingUpdate.SettingUpdate.OnUpdatesStatus(errorCode,updateFlag,version,notes);
            },
            OnUpdatesProgress:function(checkProgress){
                uc.util.LogUtil.info("SettingUpdate","progress show here",null);
                uc.modules.settingUpdate.SettingUpdate.OnUpdatesProgress(checkProgress);
            }
        })
})(uc);