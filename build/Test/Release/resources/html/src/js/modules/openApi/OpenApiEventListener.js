/**
 * Created by zhen.xu on 2015/9/16.
 */
(function(uc){
    uc.modules.openApi.OpenApiEventListener = function(){

    };
    jQuery.extend(uc.modules.openApi.OpenApiEventListener.prototype,
        uc.interfaces.IUIEvent,
        uc.interfaces.IOpenApiEvent, {
            OnInitClient: function(clearData){
                uc.modules.openApi.OpenApiManager.OnInitClient(clearData);
            },
            OnAppListReceived: function(errorCode, apps){
                uc.util.LogUtil.info('OpenApiEventListener', 'OnAppListReceived', 'App list', apps);
                uc.modules.openApi.OpenApiManager.OnAppListReceived(errorCode, apps);
            },
            OnModuleChanged: function (moduleId) {
                if (moduleId == uc.constants.UI.ModuleId.App) {
                    //uc.modules.openApi.OpenApiManager.OnInitClient(false);
                }
            },
            OnAppStatusChangeReceived: function (channel, appId, status, extraData) {
                uc.modules.openApi.OpenApiManager.OnAppStatusChangeReceived(channel, appId, status, extraData);
            },
            OnOpenOAApproveWindow: function (option) {
                uc.util.StorageUtil.setData("OAApprove",  option);
                var openTime = new Date();

                var options = {
                    title: '',
                    width: 380,
                    height: 273,
                    refreshIfExist: true,
                    centerScreen: true,
                    modal: true,
                    resizable: false,
                    minWidth: 380,
                    minHeight: 273,
                    caption: uc.constants.UI.DragCaption.ApiOAApprove
                };

                uc.util.WindowUtil.open('page/apiOAApprove.html', 'OAApprove' + openTime, options);
            },
            OnAppUntreadedReceived: function (channel, appId, eventCount, extraData) {
                uc.modules.openApi.OpenApiManager.OnAppUntreadedReceived(channel, appId, eventCount, extraData);
            }
        })
})(uc);