/**
 * Created by waouyang on 15/10/10.
 */
(function(uc){
    var isBindEvent=false;
    uc.modules.cloud.Cloud = {
        init: function(cloudData){
            uc.ui.Cloud.MainPanel.empty();
            if(cloudData){
                uc.util.StorageUtil.setData('cloudData',cloudData);
            }
            var path=uc.modules.cloud.Cloud.getRootPath()+"/gokuai/gokuai.html";
            this.node = $('<div class="cloud-container">' +
                '<div class="cloud-loading-font"><span><img src="../images/loading.gif" class="cloud-loading-img"/>'+uc_resource.Cloud.LoadCloud+'</span></div>' +
                '<iframe class="iframe hide" src="'+path+'" frameborder="0"></iframe>' +
                '</div>').appendTo(uc.ui.Cloud.MainPanel);
        },
        initLoading:function(){
            uc.ui.Cloud.MainPanel.empty();
            var path=uc.modules.cloud.Cloud.getRootPath()+"/gokuai/gokuai.html";
            this.node = $('<div class="cloud-container">' +
                '<div class="cloud-loading-font"><span><img src="../images/loading.gif" class="cloud-loading-img"/>'+uc_resource.Cloud.LoadCloud+'</span></div>' +
                '<iframe class="iframe hide" src="'+path+'" frameborder="0"></iframe>' +
                '</div>').appendTo(uc.ui.Cloud.MainPanel);
        },
        initFail:function(){
            uc.ui.Cloud.MainPanel.empty();
            this.node = $('<div class="cloud-container">' +
                '<div class="cloudFailPanel">'+
                '<img src="../images/chat/load-fail.png">' +
                '<div class="cloud_font">'+uc_resource.Cloud.LoadCloudFail+'</div>' +
                '<a class="btn">'+uc_resource.Cloud.RepeatLoadCloud+'</a>' +
                '</div>' +
                '</div>').appendTo(uc.ui.Cloud.MainPanel);
        },
        initFailText:function(cloudLoginResult){
            uc.ui.Cloud.MainPanel.empty();
            var errorText="GK_ERROR_CODE_"+cloudLoginResult;
            this.node = $('<div class="cloud-container">' +
                '<div class="cloud-loading-font">'+uc_resource.Cloud[errorText]+'</div>' +
                '</div>').appendTo(uc.ui.Cloud.MainPanel);
        },
        getRootPath:function(){
            var href = location.href;
            var index = href.indexOf('/html');
            return href.substring(0, index + 6);
        },
        displayCloudPage:function(){
            var cloudData=uc.util.StorageUtil.getData("cloudData");
            uc.util.LogUtil.info('Cloud', 'Cloud', 'displayCloudPage，cloudData = ' + cloudData, null);
            $(".cloud-container .iframe").removeClass().addClass("iframe");
            $(".cloud-container .cloud-loading-font").remove();
            if(cloudData){
                uc.modules.cloud.CloudManager.openCloudFileDir(cloudData);
                uc.util.StorageUtil.removeData("cloudData");
            }
            uc.modules.cloud.Cloud.changCloudFrameSize();

            if(!isBindEvent){
                this.bindEvent();
                isBindEvent = true;
            }
        },
        bindEvent: function(){
            var frame = uc.ui.Cloud.MainPanel.find("iframe");

            frame.contents().find("body").click(function(){
                $(".current-user-status-list").addClass("hide");
            });
        },
        changCloudFrameSize:function(){
            $(".cloud-container .iframe").css("height",$("#menu-panel").height());
        }
    }
})(uc);