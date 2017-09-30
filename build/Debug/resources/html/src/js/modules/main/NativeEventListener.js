/**
 * Created by waouyang on 15/9/19.
 */
(function(uc){
    var NativeEventData = {
        HotKey: {
            eventName: ucClient.Window.HOTKEY_EVENT,
            callback: function(data){
                switch (data.id){
                    case uc.constants.UI.HotKeyType.CaptureScreen:
                        var hit = false;
                        $('.screen-shot', '.message-input-box').each(
                            function(){
                                if($(this).is(':visible')){
                                    hit = true;
                                    $(this).click();
                                    return true;
                                }
                            }
                        );
                        if (!hit){
                            if (uc.settings.skScreenshot.hideWindow) {
                                uc.util.WindowUtil.minimize();
                                uc.IUIService.minimizeWindow();
                            }
                            uc.util.SystemUtil.catchScreen(function () {
                                if(uc.IClientService.getWindowStatus() == uc.constants.UI.WindowActiveState.Normal ){
                                    uc.util.WindowUtil.show();
                                }else if(uc.IClientService.getWindowStatus() == uc.constants.UI.WindowActiveState.Minimized){
                                    uc.util.WindowUtil.minimize();
                                    uc.IUIService.minimizeWindow();
                                }
                            });
                        }
                        break;
                    case uc.constants.UI.HotKeyType.ShowClient:
                        uc.util.WindowUtil.show();
                        break;
                }
            }
        },
        WindowMaximized: {
            eventName: ucClient.Window.Status.MAXIMIZED_FROM_NORMAL,
            callback: function(){
                uc.modules.main.Main.setWindowState(uc.constants.UI.WindowState.Maximized);
            }
        },
        WindowRestored: {
            eventName: ucClient.Window.Status.RESTORED_FROM_MAXIMIZED,
            callback: function(){
                uc.modules.main.Main.setWindowState(uc.constants.UI.WindowState.Normal);
            }
        }
    };
    uc.modules.main.NativeEventListener = {
        bindNativeEvent: function(){
            var eventData, eventName, callback;

            for(var i in NativeEventData){
                eventData = NativeEventData[i];
                eventName = eventData.eventName;
                callback = eventData.callback;

                uc.util.EventUtil.bindNativeEvent(eventName, callback);
            }
        }
    }
})(uc);