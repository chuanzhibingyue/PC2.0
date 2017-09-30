(function(uc){
    uc.modules.cloud.SaveCloudFile = {
        init: function(){
            uc.modules.component.TitleBarButton.init();
            uc.modules.cloud.SaveCloudFile.initPage();
        },
        initPage:function(){
            var cloudLoginResult=uc.util.StorageUtil.getData('CloudLoginResult');
            if(cloudLoginResult==0){
                uc.modules.cloud.SaveCloudFile.initCloud();
            }else if(cloudLoginResult=="" ||cloudLoginResult==null)
                setTimeout(function(){
                    uc.modules.cloud.SaveCloudFile.initPage();
                },1000);
            else{
                uc.modules.cloud.SaveCloudFile.initFail();
            }
        },
        initCloud:function(){
            var data=uc.modules.cloud.SaveCloudFile.getCurrentOpenWindowInfo();
            console.log("cloudFileData:"+data);
            var cloudFileInfo= JSON.parse(data.options);
            var fileName=encodeURI(cloudFileInfo.filename);
            var path=uc.modules.cloud.SaveCloudFile.getRootPath()+"/gokuai/saveToMount.html?filename="+fileName+"";
           $(".iframe").attr("src",path);
        },
        OnGKSaveCloudFileLoadComplete:function(){
            uc.util.LogUtil.info('SaveCloudFile', 'SaveCloudFile', 'OnGKSaveCloudFileLoadComplete', null);
            $(".panelTitle").hide();
            $(".iframe").removeClass().addClass("iframe");
        },
        initFail:function(){
            $(".iframe").hide();
            $(".panelTitle").text(uc_resource.Cloud.LoadCloudFail).show();
        },
        getCurrentOpenWindowInfo:function(){
           return uc.util.StorageUtil.getData("saveCloudFile");
        },
        getRootPath:function(){
            var href = location.href;
            var index = href.indexOf('/html');
            return href.substring(0, index + 6);
        }
    }
    jQuery(document).ready(function () {
        uc.init();
        uc.modules.cloud.SaveCloudFile.init();
    })
})(uc);