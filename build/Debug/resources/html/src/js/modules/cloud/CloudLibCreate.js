
(function(uc){
    uc.modules.cloud.CloudLibCreate = {
        init: function(){
            this.LoadingNode='<span>' +
                '<img src="../images/loading.gif" class="cloud-loading-img"/>'
                +uc_resource.Cloud.LoadCloud
            '</span>';
            this.panelTitle= $(".panelTitle");
            uc.modules.component.TitleBarButton.init();
            uc.modules.cloud.CloudLibCreate.initPage();
            uc.modules.cloud.CloudLibCreate.bindEvent();
        },
        bindEvent:function(){
            var self=this;
            $("body").on("click",'.cloudFailPanel .btn',function(){
                self.panelTitle.html(self.LoadingNode);
                uc.IUIService.triggerCrossWindowEvent("reLoginCloud",{});
            });
            uc.IUIService.bindCrossWindowEvent("reLoginCloudSuccess", function() {
                uc.modules.cloud.CloudLibCreate.initPage();
            });
        },
        initPage:function(){
            var value=uc.util.StorageUtil.getData('CloudLoginResult');
            switch (value){
                case 0:
                    uc.modules.cloud.CloudLibCreate.initCloud();
                    break;
                case "":
                case null:
                    setTimeout(function(){
                        uc.modules.cloud.CloudLibCreate.initPage();
                    },1000);
                    break;
                case 401141:
                case 401142:
                case 401143:
                case 401144:
                    $(".iframe").hide();
                    var errorText="GK_ERROR_CODE_"+value;
                    $(".panelTitle").html(uc_resource.Cloud[errorText]).show();
                    break;
                default :
                    uc.modules.cloud.CloudLibCreate.initFail();
                    break;
            }
        },
        initCloud:function(){
            var path=uc.modules.cloud.CloudLibCreate.getRootPath()+"/gokuai/createLib.html";
            $(".iframe").attr("src",path);
          //  $(".panelTitle").hide();
        },
        OnGKCloudLibCreateLoadComplete:function(){
            uc.util.LogUtil.info('CloudLibCreate', 'CloudLibCreate', 'OnGKCloudLibCreateLoadComplete', null);
            $(".panelTitle").hide();
            $(".iframe").removeClass().addClass("iframe");
        },
        initFail:function(){
            $(".iframe").hide();
            var node = '<div class="cloudFailPanel">'+
                '<img src="../images/chat/load-fail.png" width="120" height="70">' +
                '<div class="cloud_font">'+uc_resource.Cloud.LoadCloudFail+'</div>' +
                '<a class="btn">'+uc_resource.Cloud.RepeatLoadCloud+'</a>' +
                '</div>';
            $(".panelTitle").html(node).show();
        },
        getRootPath:function(){
            var href = location.href;
            var index = href.indexOf('/html');
            return href.substring(0, index + 6);
        }
    }
    jQuery(document).ready(function () {
        uc.init();
        uc.modules.cloud.CloudLibCreate.init();
    })
})(uc);