/**
 * Created by waouyang on 15/8/13.
 */
(function(uc){
    uc.modules.setting.SettingEventListener = function(){

    };
    jQuery.extend(uc.modules.setting.SettingEventListener.prototype,
        uc.interfaces.IFSEvent,
        uc.interfaces.IClientEvent,
        uc.interfaces.IUIEvent, {
            OnOpenSettingWindow: function(){
                uc.IUIService.bindCrossWindowEvent("Setting:Logout", function() {
                    uc.modules.main.Main.logoutClient();
                });
                uc.IUIService.bindCrossWindowEvent("Setting:Logout-CallEnd", function() {
                    if(uc.modules.call.CallManager.inProgressCallInfo.isInProgress || !uc.modules.pbx.PbxManager.DialIsClick){
                        uc.IUIService.triggerCrossWindowEvent("Setting:Logout-true", {isCallEnd:false});
                    }else{
                        uc.IUIService.triggerCrossWindowEvent("Setting:Logout-true", {isCallEnd:true});
                    }
                });
                var options = {
                    width: 580,
                    height: 440,
                    refreshIfExist: false,
                    centerScreen: true,
                    modal: true,
                    resizable: false,
                    isTop:false,
                    minWidth: 580,
                    minHeight: 440,
                    hasTaskBarIcon: false,
                    isShow: false,
                    caption: uc.constants.UI.DragCaption.Setting
                };
                uc.util.WindowUtil.open('page/setting.html', 'setting', options);
            }
    })
})(uc);