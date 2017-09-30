(function(uc){
    var isBindEvent=false;
    uc.modules.cloud.CloudManager = {
        init: function(mountIdData){
            this.mountId="";
            if(this.initilized){
                uc.modules.cloud.CloudManager.openCloudFileDir(mountIdData);
                return;
            }
            uc.modules.cloud.Cloud.init(mountIdData);
            this.initilized = true;

        },
        bindEvent:function(){
            $("body").on("click",'.cloud-container .btn',function(){
                uc.modules.cloud.Cloud.initLoading();
                uc.modules.cloud.CloudManager.getGkClientReLogin();
                setTimeout(function(){
                    uc.modules.cloud.CloudManager.getGkClientStatus();
                },500);
            });
        },
        createLibSuccess:function(data){
            var mountId=data.mountId;
            var cloudData=JSON.stringify({"mount_id":mountId,"create":true});
            uc.util.LogUtil.info('CloudManager', 'createLibSuccess', 'cloudData = ' +cloudData, null);
            uc.IUIService.changeModule(uc.constants.UI.ModuleId.Cloud,cloudData);
        },
        OnModuleChanged: function(moduleId,mountIdData){
            if(!isBindEvent){
                this.bindEvent();
                isBindEvent=true;
            }
            if(moduleId == uc.constants.UI.ModuleId.Cloud){
                var value=uc.util.StorageUtil.getData('CloudLoginResult');
                switch (value){
                    case 0:
                        uc.modules.cloud.CloudManager.init(mountIdData);
                        break;
                    case 401141:
                    case 401142:
                    case 401143:
                    case 401144:
                        uc.modules.cloud.Cloud.initFailText(value);
                        break;
                    case "":
                    case null:
                        uc.modules.cloud.CloudManager.getGkClientStatus(mountIdData);
                        break;
                    default :
                        uc.modules.cloud.Cloud.initFail();
                        break;
                }
            }
        },
        OnGkClientConnectionStatus:function(errorCode,mountIdData){
            if(errorCode==0){
                uc.IUIService.triggerCrossWindowEvent("CloudLoginSuccess",{});
                uc.modules.cloud.CloudManager.setCloudLoginResult(errorCode);
                uc.modules.cloud.CloudManager.init(mountIdData);
                return;
            }
            uc.modules.cloud.Cloud.initLoading();
            var cloudLoginResult=uc.util.StorageUtil.getData('CloudLoginResult');
            if(cloudLoginResult==401141 ||
                cloudLoginResult==401142 ||
                cloudLoginResult==401143 ||
                cloudLoginResult==401144){
                uc.modules.cloud.CloudManager.setCloudLoginResult(cloudLoginResult);
                uc.modules.cloud.Cloud.initFailText(cloudLoginResult);
                return;
            }
            if(errorCode==1 || errorCode==2){
                setTimeout(function(){
                    uc.modules.cloud.CloudManager.getGkClientStatus(mountIdData);
                },1000);
            }else{
                uc.modules.cloud.Cloud.initFail();
            }
        },
        setCloudLoginResult:function(errorCode){
            uc.util.StorageUtil.setData('CloudLoginResult',errorCode);
        },
        reLoginCloudSuccess:function(errorCode){
            uc.IUIService.triggerCrossWindowEvent("reLoginCloudSuccess",{});
        },
        OnPreloadCloud:function(errorCode){
            this.reLoginCloudSuccess(errorCode);
            if(errorCode==0){
                uc.IUIService.triggerCrossWindowEvent("CloudLoginSuccess",{});
                var cloudLoginResult=uc.util.StorageUtil.getData('CloudLoginResult');
                if(cloudLoginResult==0){
                    uc.modules.cloud.CloudManager.init();
                }
            }
        },
        OnUserAvatarDownloaded:function(userId, serverAvatarUrl, localAvatarUrl){
            if(top.window.gkClientCallback){
                var contact=[{"userId":userId,"serverAvatarUrl":serverAvatarUrl,"localAvatarUrl":localAvatarUrl}];
                uc.IUIService.triggerCrossWindowEvent("OnCloudPreviewUserInfo",{"contact":contact});
            }
        },
        OnCloudGetContactInfo:function(userIds){
           uc.util.LogUtil.info('CloudManager', 'userIds', 'OnCloudGetContactInfo userIds = ' + JSON.stringify(userIds), null);
            if(userIds.length){
                uc.IContactService.getBasicContactInfo(userIds);
            }
        },
        getPreviewerUserId:function(data){
            var userIds=data["userId"];
            uc.modules.cloud.CloudManager.OnCloudGetContactInfo(userIds);
        },
        OnContactInfoReceived: function(contacts){
            var contactInfo, userId,serverUrl,AvatarDownloadComplete=[];
            for(var i = 0, len = contacts.length; i < len; i++){
                contactInfo = contacts[i];
                if(contactInfo){
                    userId = contactInfo.userId;
                    serverUrl = contactInfo.remoteAvatarUrl;
                    if(contactInfo.localAvatar){
                        AvatarDownloadComplete.push({"userId":userId,"serverAvatarUrl":serverUrl,"localAvatarUrl":contactInfo.localAvatar});
                       // uc.IUIEvent.OnUserAvatarDownloaded(userId, serverUrl, contactInfo.localAvatar);
                    }else{
                        uc.IUIService.downloadUserAvatar(userId, contactInfo.remoteAvatarUrl);
                    }
                }
            }
            //头像下载完成批量传给GK
            if(AvatarDownloadComplete.length){
                if(top.window.gkClientCallback){
                    uc.IUIService.triggerCrossWindowEvent("OnCloudPreviewUserInfo",{"contact":AvatarDownloadComplete});
                }
            }
        },
        OnCloudOpenChat:function(mountId){
            uc.util.LogUtil.info("CloudManager", "OnCloudOpenChat", "mountId :", mountId);
            if(!mountId){
                return;
            }
            this.mountId=mountId;
            var groupInfo = uc.IGroupService.getGroupInfoByMountId(mountId);
            if(groupInfo && groupInfo.mountId){
                uc.util.LogUtil.info("CloudManager-OnCloudOpenChat", "getGroupInfo:"+JSON.stringify(groupInfo)+",get groupInfo errorCode :","mountId:"+mountId+",ToMountId:"+this.mountId);
                uc.IUIService.openChatWindow(uc.constants.Chat.ChatType.CloudChat,groupInfo.id,uc.constants.UI.OpenChatWindowSourceModule.Other, null, null, true);
            }else{
                uc.util.LogUtil.info("CloudManager", "OnCloudOpenChat", "get groupInfo fail,mountId :", mountId);
            }
        },
        getGroupInfo:function(errorCode,groupId,mountId,group){
            uc.util.LogUtil.info("CloudManager-getGroupInfo", "getGroupInfo"+JSON.stringify(group)+",get groupInfo errorCode :", errorCode+",mountId:"+mountId+",ToMountId:"+this.mountId);
            if(this.mountId && (mountId==this.mountId)){
                if(errorCode==0 && mountId!=0){
                    uc.IUIService.openChatWindow(uc.constants.Chat.ChatType.CloudChat,groupId,uc.constants.UI.OpenChatWindowSourceModule.Other, null, null, true);
                }else{
                    uc.modules.component.PromptMessageBox.open({title: uc_resource.MessageBox.Notification, message: uc_resource.MessageBox.notRelationGroup, buttonMode:"ok",OkText:uc_resource.MessageBox.OK},
                        function(result){
                            this.close();
                        });
                    return false;
                }
            }
        },
        OnCloudHideComment:function(){
            uc.modules.component.ExtraMenuBar.hideStartContainer();
            uc.modules.comment.CommentManager.hideFileCommentPage();
        },
        OnCloudOpenComment:function(data){
            uc.util.StorageUtil.setData('currentChatType',uc.constants.Chat.ChatType.CloudChat);
            uc.modules.comment.CommentManager.initCloudComment(data);
        },
        OnCloudFileInfo:function(data){
         var dataArr=JSON.parse(data);
            var mountId="";
            if(dataArr.hasOwnProperty("mount_id")){
                mountId=parseInt(dataArr.mount_id);
            }else if(dataArr.hasOwnProperty("mountId")){
                mountId=parseInt(dataArr.mountId);
            }else if(dataArr.hasOwnProperty("mountid")){
                mountId=parseInt(dataArr.mountid);
            }
            dataArr.mount_id=mountId;
         uc.modules.chat.MessageManager.forwardCloudFileMessage(dataArr.filename, dataArr.filesize,"",data,dataArr);
        },
        checkCloudIsLoginSuccess:function(){
            return uc.util.StorageUtil.getData('cloudType');
        },
        checkCloudTokenIsSuccess:function(){
            return uc.util.StorageUtil.getData('CloudLoginResult');
        },
        openCloudFileDir:function(cloudData){
            uc.util.LogUtil.info('CloudManager', 'CloudManager', 'openCloudFileDir = ' + cloudData, null);
            if(cloudData){
                gkClient.gOpenMount(cloudData);
            }
        },
        OnGKLoadComplete:function(){
            uc.modules.cloud.Cloud.displayCloudPage();
        },
        gCheckFilePermission:function(cloudInfo,type,that){
            uc.util.LogUtil.info('CloudManager_Start', 'CloudManager_Start', 'gCheckFilePermission = ' + cloudInfo);
            gkClient.gCheckFilePermission(cloudInfo,function(data){
                uc.util.LogUtil.info('CloudManager', 'CloudManager', 'gCheckFilePermission = ' + data, null);
                uc.IUIEvent.OnCheckGkFilePermission(data,cloudInfo,type,that);
            });
        },
        gCheckFileExist:function(cloudData,option,Type){
            gkClient.gCheckFileExist(cloudData,function(data){
                uc.util.LogUtil.info('CloudManager', 'CloudManager cloudData='+cloudData, 'gCheckFileExist = ' + data, null);
                uc.IUIEvent.OnCheckGkFileExist(data,cloudData,option,Type);
            });
        },
        gkRemoveDownload:function(data){
            uc.util.LogUtil.info('CloudManager', 'gkRemoveDownload', 'gkRemoveDownload = ' + data, null);
            gkClient.gRemoveDownload(data,function(arg){});
        },
        gkGetFileInfo:function(cloudData,Type){
            gkClient.gGetFileInfoByHash(cloudData,function(data){
                uc.util.LogUtil.info('CloudManager', 'gkGetFileInfo', 'gkGetFileInfo = ' + data, null);
                uc.IUIEvent.OnGkGetFileInfo(data,cloudData,Type);
            });
        },
        getGkClientStatus:function(mountIdData){
            uc.IClientService.getGkClientConnection(function(arg){
                uc.util.LogUtil.info('Main', 'getGkClientStatus', 'errorCode = ' + arg[1], null);
                uc.IUIEvent.OnGkClientConnectionStatus(arg[1],mountIdData);
            });
        },
        getGkClientReLogin:function(){
            uc.IClientService.getGkClientReLogin();
        },
        clearLocalStorage:function(){
            var clearItem=["CloudLoginResult"];
            for(var i=0;i<clearItem.length;i++){
                uc.util.StorageUtil.removeData(clearItem[i]);
            }
        },
        noticeCloudInfoDownload:function(data){
            if(!uc.modules.chat.ChatManager.checkCloudOpenStatus()){
                return;
            }
            if(!uc.modules.chat.ChatManager.checkCloudLoadStatus()){
                return;
            }
            var self=this;
            var CloudInfoStr=decodeURIComponent(window.atob(data.data));
            var CloudInfoObj= $.parseJSON(CloudInfoStr);
            uc.util.LogUtil.info('Main', 'noticeCloudInfoDownload', 'cloudInfo = ' + CloudInfoStr, null);
            uc.util.FileDialogUtil.openSaveFileDialog(
                uc_resource.MessageBox.SaveFile,
                CloudInfoObj.filename,
                [{ desc: uc_resource.MessageBox.AllFile, filter: '*.*'}],
                function(args){
                    var savePath = args[0];
                    if (savePath == null || savePath == "") {
                        return;
                    }
                    var info= $.extend(CloudInfoObj,{"fullpath":savePath});
                    self.downloadCloudFile(JSON.stringify(info));
                    uc.IUIService.triggerCrossWindowEvent("noticeAlert",{});
            });
        },
        downloadCloudFile:function(data,callBack){
            gkClient.gSaveFile(data,function(t){
                callBack && callBack(t);
            });
        },
        OnCloudFilePreviewer:function(data){
            uc.util.StorageUtil.setData("CloudFilePreviewer", {
                options: data
            });
            var cloudInfo=JSON.parse(data);
            var options = {
                width: 730,
                height: 600,
                refreshIfExist: false,
                centerScreen: true,
                modal: false,
                isChild: false,
                resizable: true,
                hasTaskBarIcon:true,
                title:cloudInfo.filename,
                caption:uc.constants.UI.DragCaption.CloudFilePreviewer
            };
            uc.util.WindowUtil.open('page/cloudFilePreviewer.html', 'CloudFilePreviewer'+cloudInfo.hash, options);
        },
        cloudLibCreate:function(data){
            var options = {
                width: 420,
                height: 150,
                refreshIfExist: false,
                centerScreen: true,
                modal: true,
                resizable: false,
                caption:uc.constants.UI.DragCaption.CloudLibCreate
            };
            uc.util.WindowUtil.open('page/cloudLibCreate.html', 'cloudLibCreate', options);
        },
        selectCloudFile: function (op) {
            uc.util.StorageUtil.setData("selectCloudFile", {
                options: op
            });
            var options = {
                width: 700,
                height: 530,
                refreshIfExist: false,
                centerScreen: true,
                modal: true,
                resizable: false,
                caption:uc.constants.UI.DragCaption.selectCloudFile
            };
            uc.util.WindowUtil.open('page/selectCloudFile.html', 'selectCloudFile', options);
        },
        saveCloudFile: function (op,cloudFileInfo) {
            uc.util.StorageUtil.setData("saveCloudFile", {
                options: cloudFileInfo
            });
            var options = {
                width: 700,
                height: 530,
                refreshIfExist: false,
                centerScreen: true,
                modal: true,
                resizable: false,
                caption:uc.constants.UI.DragCaption.saveCloudFile
            };
            uc.util.WindowUtil.open('page/saveCloudFile.html', 'saveCloudFile', options);
        },
        OnCloudChatSet: function (groupId) {
            var userInfo = uc.IContactService.getCurrentUserInfo();
            var groupInfo = uc.IGroupService.getGroupInfo(groupId);
            if(!groupInfo || groupInfo.joinState == 1 || groupInfo.status == 1){
                return ;
            }
            uc.util.StorageUtil.setData('cloudChatSetGroupId', groupId);
            var options = {
                width: 395,
                height: 150,
                refreshIfExist: false,
                centerScreen: true,
                modal: true,
                resizable: false,
                minWidth: 395,
                minHeight: 180,
                caption: uc.constants.UI.DragCaption.CloudChatSet
            };
            uc.util.WindowUtil.open('page/cloudChatSet.html', 'cloudChatSet' + groupId, options);
        }
    }
})(uc);