/**
 * Created by waouyang on 15/10/10.
 */
(function(uc){
    var self = null;
    uc.modules.cloud.CloudFilePreviewer = {
        init: function(){
            self = this;
            this.LoadingNode='<span>' +
                '<img src="../images/loading.gif" class="cloud-loading-img"/>'
                +uc_resource.Cloud.LoadCloud
            '</span>'
            this.panelTitle= $(".panelTitle");
            this.container = $('.panel');
            uc.modules.component.TitleBarButton.init();
            uc.modules.cloud.CloudFilePreviewer.initPage();
            uc.modules.cloud.CloudFilePreviewer.bindEvent();
        },
        bindEvent:function(){
            var self=this;
            $("body").on("click",'.cloudFailPanel .btn',function(){
                self.panelTitle.html(self.LoadingNode);
                uc.IUIService.triggerCrossWindowEvent("reLoginCloud",{});
            });
            uc.IUIService.bindCrossWindowEvent("reLoginCloudSuccess", function() {
                uc.modules.cloud.CloudFilePreviewer.initPage();
            });
            uc.IUIService.bindCrossWindowEvent("OnCloudPreviewUserInfo", function (data) {
                top.window.gkClientCallback("user_avatar_downloaded",data);
            });
            this.bindNativeEvent();
        },
        NativeEventData: {
            WindowMaximized: {
                eventName: ucClient.Window.Status.MAXIMIZED_FROM_NORMAL,
                callback: function(){
                    uc.modules.component.TitleBarButton.setWindowState(uc.constants.UI.WindowState.Maximized);
                    self.container.addClass('max');
                }
            },
            WindowRestored: {
                eventName: ucClient.Window.Status.RESTORED_FROM_MAXIMIZED,
                callback: function(){
                    uc.modules.component.TitleBarButton.setWindowState(uc.constants.UI.WindowState.Normal);
                    self.container.removeClass('max');
                }
            }
        },

        bindNativeEvent: function(){
            var eventData, eventName, callback;
            for(var i in this.NativeEventData){
                eventData = this.NativeEventData[i];
                eventName = eventData.eventName;
                callback = eventData.callback;
                uc.util.EventUtil.bindNativeEvent(eventName, callback);
            }
        },
        initPage:function(){
            var cloudLoginResult=uc.util.StorageUtil.getData('CloudLoginResult');
            if(cloudLoginResult==0){
                uc.modules.cloud.CloudFilePreviewer.initCloud();
            }else if(cloudLoginResult=="" ||cloudLoginResult==null){
                setTimeout(function(){
                    uc.modules.cloud.CloudFilePreviewer.initPage();
                },1000);
            }else{
                uc.modules.cloud.CloudFilePreviewer.initFail();
            }
        },
        initCloud:function(){
            var cloudFileInfo=  this.getCurrentOpenWindowInfo();
            uc.util.LogUtil.info('CloudFilePreviewer', 'initCloud', 'cloudFileInfo='+cloudFileInfo.options, null);
            var dataStr=encodeURIComponent(cloudFileInfo.options);
            var path=uc.modules.cloud.CloudFilePreviewer.getRootPath()+"/gokuai/preview.html#?selected_file="+dataStr+"&from=qs";
            $(".iframe").attr("src",path).removeClass().addClass("iframe");
            $(".panelTitle").hide();
        },
        OnGKCloudFilePreviewerLoadComplete:function(){
            uc.util.LogUtil.info('CloudFilePreviewer', 'CloudFilePreviewer', 'OnGKCloudFilePreviewerLoadComplete', null);
            $(".panelTitle").hide();
            $(".iframe").removeClass().addClass("iframe");
        },
        initFail:function(){
            $(".iframe").hide();
            var node = '<div class="cloudFailPanel">'+
                '<img src="../images/chat/load-fail.png">' +
                '<div class="cloud_font">'+uc_resource.Cloud.LoadCloudFail+'</div>' +
                '<a class="btn">'+uc_resource.Cloud.RepeatLoadCloud+'</a>' +
                '</div>';
            $(".panelTitle").html(node).show();
        },
        getCurrentOpenWindowInfo:function(){
           return uc.util.StorageUtil.getData("CloudFilePreviewer");
        },
        getRootPath:function(){
            var href = location.href;
            var index = href.indexOf('/html');
            return href.substring(0, index + 6);
        }
    }
    jQuery(document).ready(function () {
        uc.init();
        uc.modules.cloud.CloudFilePreviewer.init();
    })
})(uc);