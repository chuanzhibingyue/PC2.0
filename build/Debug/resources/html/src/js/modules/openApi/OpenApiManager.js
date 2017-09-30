/**
 * Created by zhen.xu on 2015/9/16.
 */
(function(uc){
    var _this = null;
    uc.modules.openApi.OpenApiManager = {
        openPreviewWindowAppIds: ['10488579'],
        presetApps: [],
        init: function(){
            _this = this;
            // if (this.loading) {
            //     return;
            // }
            // this.loading = false;
            this.appContainner = $('.open-api-container');
            this.startLoadingApps();
            this.openApiDataSet = new uc.modules.openApi.OpenApiDataSet();
            this.notShowApiDataSet = new uc.modules.openApi.OpenApiDataSet();
            this.getOpenApiList();
            uc.modules.openApi.OpenApi.init();
        },
        getOpenApiList: function(){
            //add presetApps
            for (var i = 0; i < this.presetApps.length; i++) {
                var openApiInfo = this.presetApps[i];
                if(openApiInfo && openApiInfo.isShow){
                    this.openApiDataSet.addOpenApiInfo(openApiInfo.appId, openApiInfo);
                }
            }
            uc.util.LogUtil.info('uc.modules.openApi.OpenApiManager', 'getOpenApiList', 'Get App list', this.presetApps);
            uc.IOpenApiService.getAppList();
        },
        OnInitClient: function(clearData){
            this.init();
        },
        OnAppListReceived: function(errorCode, apps){
            if(errorCode == uc.ErrorCode.Success){
                //this.openApiDataSet.clear();
                if(apps && apps.length){
                    var openApiInfo;
                    for(var i = 0, len = apps.length; i < len; i++){
                        openApiInfo = apps[i];
                        if(openApiInfo){
                            this.openApiDataSet.addOpenApiInfo(openApiInfo.appId, openApiInfo);
                        }
                    }
                }
            }
            uc.modules.openApi.OpenApi.OnAppListReceived(errorCode);
        },
        getOpenApiInfo: function(appId){
            var appInfo = this.openApiDataSet.getOpenApiInfo(appId);
            var notSHowAppInfo = this.notShowApiDataSet.getOpenApiInfo(appId);
            return appInfo ? appInfo : notSHowAppInfo;
        },
        getAppsInfo: function (appIdList) {
            uc.IOpenApiService.getAppInfo(appIdList);
        },
        getAllOpenApiInfo: function(){
            return this.openApiDataSet.getAllOpenApiInfo();
        },
        updateOpenApiInfo: function (app) {
            this.openApiDataSet.updateOpenApiInfo(app.appId, app);
        },
        cacheOpenApiInfo: function (app) {
            this.notShowApiDataSet.updateOpenApiInfo(app.appId, app);
        },
        getOpenApiInfoCount: function(){
            return this.openApiDataSet.getCount();
        },
        startLoadingApps: function () {
//            uc.ui.App.ContentPanelBody.append('<div class="open-api-loading"><div class="conversation-loading">' +
//                '<div class="conversation-loading-bar"></div>'+
//                '<div class="conversation-loading-bar"></div>'+
//                '<div class="conversation-loading-bar"></div>'+
//                '<div class="conversation-loading-bar"></div>'+
//                '<div class="conversation-loading-bar"></div>'+
//                '<div class="conversation-loading-bar"></div>'+
//                '<div class="conversation-loading-bar"></div>'+
//                '<div class="conversation-loading-bar"></div>'+
//                '<div class="conversation-loading-bar"></div>'+
//                '<div class="conversation-loading-bar"></div>'+
//                '</div></div>');

            uc.ui.App.ContentPanelBody.append('<div class="open-api-loading"><img src="../images/messagebox/loading-17.gif"/></div>');
            //this.loading = true;
        },
        stopLoadingApps: function () {
            uc.ui.App.ContentPanelBody.find('.open-api-loading').remove();
            //this.loading = false;
        },
        setPstAppStatus: function (appId, status) {
            var self = this;
            for (var i = 0; i < this.presetApps.length; i++) {
                var openApiInfo = this.presetApps[i];
                if(openApiInfo.appId == appId){
                    this.presetApps[i].isShow = status;
                    setTimeout(function () {self.init();}, 500);
                }
            }
        },
        isOpenPreviewWindow: function (appId) {
            return this.openPreviewWindowAppIds.indexOf(appId) >= 0 ? true : false;
        },
        OnAppStatusChangeReceived: function (channel, appId, status, extraData) {
            var self = this;
            var data = {
                channel: channel,
                appId: appId,
                status: status,
                extraData: extraData
            };
            uc.util.LogUtil.info('OpenApiManager', 'OnAppStatusChange', 'response data:', data);
            setTimeout(function () {self.init();}, 500);
        },
        OnAppUntreadedReceived: function (channel, appId, eventCount, extraData) {
            var self = _this, msgCount = $('.open-api-container').find('li[data-app-id="'+ appId +'"] .msg-count');
            uc.util.LogUtil.info('OpenApiManager', 'OnAppUntreadedReceived', 'response', {appId: appId, eventCount: eventCount, extraData: extraData});
            msgCount.removeClass('show-count show-point');
            if (eventCount) {
                msgCount.addClass('show-point');

                //msgCount.addClass('show-count');
                //if(eventCount > 10) {
                //    msgCount.addClass('double')
                //}
                //
                //if (eventCount >= 100) {
                //    eventCount = '99+';
                //}
                //msgCount.text(eventCount);
            }
        }
    }
})(uc);