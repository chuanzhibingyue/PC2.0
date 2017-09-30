(function(uc){
    uc.modules.cloud.gkEventListener = {
        OnGkEventListener:function(type,data){
            switch (type){
                case  "sendFromMount":
                    var chatInfo= uc.modules.cloud.SelectCloudFile.getCurrentOpenWindowInfo();
                    var fileIcon = [];
                    for(var i=0; i<data.list.length;i++){
                        fileIcon.push(uc.util.StringUtil.getFileIcon(data.list[i].filename));
                    }
                    uc.IUIService.triggerCrossWindowEvent("sendFromMount",{
                        data: JSON.stringify(data),
                        chatInfo:JSON.stringify(chatInfo),
                        fileIcon:fileIcon
                    });
                    uc.util.WindowUtil.close();
                    break;
                case  "saveToMount":
                    var info= uc.modules.cloud.SaveCloudFile.getCurrentOpenWindowInfo();
                    console.log("saveToMount:"+info.options);
                    top.window.gkClientCallback("saveToMount",info.options);
                    break;
                case "createlibSuccess":
                    uc.IUIService.triggerCrossWindowEvent("createCloudLibSuccess",data);
                    uc.util.WindowUtil.close();
                    break;
                case  "cancel":
                    if(uc.modules.cloud.SelectCloudFile){
                        var cloudFileInfo=uc.modules.cloud.SelectCloudFile.getCurrentOpenWindowInfo();
                        if( cloudFileInfo && cloudFileInfo.options.chatType == 16){
                            uc.IUIService.triggerCrossWindowEvent("cancelSelectCloudFile",{});
                        }
                    }
                    uc.util.WindowUtil.close();
                    break;
            }
        }

    }
    jQuery(document).ready(function () {
       uc.init();
    })
})(uc);