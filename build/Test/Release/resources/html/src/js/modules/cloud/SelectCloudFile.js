/**
 * Created by waouyang on 15/10/10.
 */
(function(uc){
    uc.modules.cloud.SelectCloudFile = {
        init: function(){
            this.LoadingNode='<span>' +
                '<img src="../images/loading.gif" class="cloud-loading-img"/>'
                +uc_resource.Cloud.LoadCloud
                '</span>'
            this.panelTitle= $(".panelTitle");
            uc.modules.component.TitleBarButton.init();
            uc.modules.cloud.SelectCloudFile.initPage();
            uc.modules.cloud.SelectCloudFile.bindEvent();
        },
        bindEvent:function(){
            var self=this;
            $("body").on("click",'.cloudFailPanel .btn',function(){
                self.panelTitle.html(self.LoadingNode);
                uc.IUIService.triggerCrossWindowEvent("reLoginCloud",{});
            });
            uc.IUIService.bindCrossWindowEvent("reLoginCloudSuccess", function() {
                uc.modules.cloud.SelectCloudFile.initPage();
            });
            $(".icon-close").off("click")
            $("body").on("click",".icon-close",function(e){
                var cloudFileInfo=self.getCurrentOpenWindowInfo();
                if( cloudFileInfo && cloudFileInfo.options.chatType == 16){
                    uc.IUIService.triggerCrossWindowEvent("cancelSelectCloudFile",{});
                }
                uc.util.WindowUtil.close();
            });
        },
        initPage:function(){
            var cloudLoginResult=uc.util.StorageUtil.getData('CloudLoginResult');
            if(cloudLoginResult==0){
                uc.modules.cloud.SelectCloudFile.initCloud();
            }else if(cloudLoginResult=="" ||cloudLoginResult==null){
                setTimeout(function(){
                    uc.modules.cloud.SelectCloudFile.initPage();
                },1000);
            }else if(cloudLoginResult==401141 ||
                cloudLoginResult==401142 ||
                cloudLoginResult==401143 ||
                cloudLoginResult==401144){
                var errorText="GK_ERROR_CODE_"+cloudLoginResult;
                $(".iframe").hide();
                $(".panelTitle").html(uc_resource.Cloud[errorText]).show();
            }else{
                uc.modules.cloud.SelectCloudFile.initFail();
            }
        },
        initCloud:function(){
            var path=uc.modules.cloud.SelectCloudFile.getRootPath()+"/gokuai/sendFromMount.html";
            var node ='<div class="panelTitle">' +
                '<span>' +
                '<img src="../images/loading.gif" class="cloud-loading-img"/>'+uc_resource.Cloud.LoadCloud+'</span>' +
                '</div>' +
                '<iframe class="iframe hide" src="'+path+'" frameborder="0"></iframe>' +
                '</div>';

            $(".panelContain").html(node);
        },
        OnGKSelectCloudFileLoadComplete:function(){
            uc.util.LogUtil.info('SelectCloudFile', 'SelectCloudFile', 'OnGKSelectCloudFileLoadComplete', null);
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
           return uc.util.StorageUtil.getData("selectCloudFile");
        },
        getRootPath:function(){
            var href = location.href;
            var index = href.indexOf('/html');
            return href.substring(0, index + 6);
        }
    }
    jQuery(document).ready(function () {
        uc.init();
        uc.modules.cloud.SelectCloudFile.init();
    })
})(uc);