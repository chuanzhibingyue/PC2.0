/**
 * Created by waouyang on 15/10/10.
 */
(function(uc){
    uc.modules.cloud.CloudSetting = {
        init: function(){
            this.LoadingNode='<span>' +
                '<img src="../images/loading.gif" class="cloud-loading-img"/>'
                +uc_resource.Cloud.LoadCloud
            '</span>'
            this.panelTitle= $(".panelTitle");
            uc.modules.component.TitleBarButton.init();
            uc.modules.cloud.CloudSetting.initPage();
            uc.modules.cloud.CloudSetting.bindEvent();
        },
        bindEvent:function(){
            var self=this;
            $("body").on("click",'.cloudFailPanel .btn',function(){
                self.panelTitle.html(self.LoadingNode);
                uc.IUIService.triggerCrossWindowEvent("reLoginCloud",{});
            });
            uc.IUIService.bindCrossWindowEvent("reLoginCloudSuccess", function() {
                uc.modules.cloud.CloudSetting.initPage();
            });
        },
        initPage:function(){
            var cloudLoginResult=uc.util.StorageUtil.getData('CloudLoginResult');
            if(cloudLoginResult==0){
                uc.modules.cloud.CloudSetting.initCloud();
            }else if(cloudLoginResult==null){
                setTimeout(function(){
                    uc.modules.cloud.CloudSetting.initPage();
                },1000);
            }else if(cloudLoginResult==401141 ||
                cloudLoginResult==401142 ||
                cloudLoginResult==401143 ||
                cloudLoginResult==401144){
                var errorText="GK_ERROR_CODE_"+cloudLoginResult;
                $(".iframe").hide();
                $(".panelTitle").html(uc_resource.Cloud[errorText]).show();
            } else{
                uc.modules.cloud.CloudSetting.initFail();
            }
        },
        initCloud:function(){
            var path=uc.modules.cloud.CloudSetting.getRootPath()+"/gokuai/setting.html";
            var node ='<div class="panelTitle">' +
                '<span>' +
                '<img src="../images/loading.gif" class="cloud-loading-img"/>'+uc_resource.Cloud.LoadCloud+'</span>' +
                '</div>' +
                '<iframe class="iframe hide" src="'+path+'" frameborder="0"></iframe>' +
                '</div>';
            $(".panelContain").html(node);
        },
        OnGKCloudSettingLoadComplete:function(){
            uc.util.LogUtil.info('CloudSetting', 'CloudSetting', 'OnGKCloudSettingLoadComplete', null);
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
           return uc.util.StorageUtil.getData("cloudSetting");
        },
        getRootPath:function(){
            var href = location.href;
            var index = href.indexOf('/html');
            return href.substring(0, index + 6);
        }
    }
    jQuery(document).ready(function () {
        uc.init();
        uc.modules.cloud.CloudSetting.init();
    })
})(uc);