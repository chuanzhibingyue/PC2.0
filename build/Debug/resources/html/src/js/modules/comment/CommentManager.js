(function (uc) {
    var rootPath = (function(){
        var href = location.href;
        var index = href.indexOf('/html');
        return href.substring(0, index + 6);
    })();
    
    uc.modules.comment.CommentManager = {
        init: function (){
            this.fileId="";
            this.mountId="";
            this.isFromChatComment=false;
            this.tempData={};
            if (this.myUserInfo == null) {
                this.myUserInfo = uc.IContactService.getCurrentUserInfo();
            }
        },
        getUserLogo: function (userId, logoUrl) {
            uc.IUIService.downloadUserAvatar(userId, logoUrl);
        },
        initCommentPage:function(){
            this.isFromChatComment=false;
            this.init();
            this.fileCommentNode=$(".filecomment");
            this.fileCommentNode.empty();
            if(this.fileComment==null){
                this.fileComment = new uc.modules.comment.Comment;
                uc.modules.chat.ChatManager.fileComment=this.fileComment;
            }
            this.fileComment.init(this.fileCommentNode);
            $("#commentLoading").show();
        },
        initChatComment:function(fileId,cloudInfo,options){
            this.initCommentPage();
            this.fileComment.show(true);
            this.fileId=fileId;
            var self=this;
            if(fileId && fileId!=0){
                this.showFileCommentByFileId(options.chatObjId,fileId);
            }else if(cloudInfo!=""){
                uc.util.StorageUtil.setData('isOpenComment',true);
                var cloudFileInfoArr= JSON.parse(cloudInfo);
                uc.modules.chat.MessageManager.cloudChatGetFileId(cloudFileInfoArr.filesize,cloudFileInfoArr.filename,cloudInfo,options);
            }
        },
        initChatListComment:function(fileInfo,groupId){
            this.initCommentPage();
            this.fileComment.setFileInfo(fileInfo,groupId);
            this.fileComment.commentListShow();
        },
        initChatCommentMessage:function(fileId,options){
            this.initCommentPage();
            this.isFromChatComment=true;
            this.fileComment.show(true);
            this.fileId=fileId;
            var self=this;
            if(fileId && fileId!=0){
                this.showFileCommentByFileId(options.chatObjId,fileId);
            }
        },
        ChatCommentMessageSuccess:function(groupId,document,fileId){
            var cloudData=document.detailContent;
            var self=this;
            if(!cloudData)return;
            gkClient.gCheckFileExist(cloudData,function(data){
                uc.util.LogUtil.info('CommentManager', 'ChatCommentMessageSuccess', 'gCheckFileExist = ' + data, null);
                var data=JSON.parse(data);
                if(data && data.error==0){
                    self.showFileCommentPage(groupId,document,fileId);
                }else{
                    //self.hideFileCommentPage();
                    var content= content=uc_resource.MessageBox.notPermission;
                    if(data.error==40402){
                        content=uc_resource.MessageBox.notFileInfo;
                    }
                    uc.modules.component.PromptMessageBox.open({title: uc_resource.MessageBox.Notification, message: content, buttonMode:"ok",OkText:uc_resource.MessageBox.OK},
                        function(result){
                            this.close();
                        });
                }
            });
        },
        showFileCommentPage:function(groupId,document,fileId){
             if(this.fileComment != null && fileId==this.fileId){
                 this.fileComment.setFileInfo(document,groupId);
                 this.fileComment.completeShow();
             }else{
                  if(uc.util.StorageUtil.getData('isOpenComment')){
                      this.fileComment.setFileInfo(document,groupId,true);
                      this.fileComment.completeShow();
                  }
             }
        },
        hideFileCommentPage:function(){
            if(this.fileComment != null){
                this.fileComment.show(false);
                this.fileComment=null;
            }
        },
        initCloudComment:function(data){
            this.initCommentPage();
            this.fileComment.show(true);
            uc.util.StorageUtil.setData('isOpenComment',true);
            var cloudFileArr=JSON.parse(data);
            var groupInfo = uc.IGroupService.getGroupInfoByMountId(cloudFileArr.mount_id);
            if(groupInfo){
                var options={"chatObjId":groupInfo.id};
                uc.modules.chat.MessageManager.cloudChatGetFileId(cloudFileArr.filesize,cloudFileArr.filename,data,options);
            }else{
                this.mountId=cloudFileArr.mount_id;
                this.tempData[this.mountId]=cloudFileArr;
            }
        },
        getGroupInfo:function(errorCode,groupId,mountId,group){
            if(mountId==this.mountId){
                if(errorCode==0){
                    var options={"chatObjId":groupId};
                    var cloudData=this.tempData[mountId];
                    var cloudDStr=JSON.stringify(cloudData);
                    if(cloudData){
                        uc.modules.chat.MessageManager.cloudChatGetFileId(cloudData.filesize,cloudData.filename,cloudDStr,options);
                    }
                }else if(errorCode==10372){
                   // this.hideFileCommentPage();
                    uc.modules.component.PromptMessageBox.open({title: uc_resource.Cloud.Notification, message: uc_resource.Cloud.NotComment, buttonMode:"ok",OkText:uc_resource.MessageBox.OK},
                        function(result){
                            this.close();
                        });
                    return false;
                }
            }
        },
        noticeCommentCreated:function(errorCode, groupId, comment,msgId){
            if(this.fileComment != null){
                this.fileComment.OnCommentCreated(errorCode, groupId, comment,msgId);
            }
        },
        OnCommentListReceived: function(errorCode, groupId, comments){
            if(this.fileComment){
                this.fileComment.OnCommentListReceived(errorCode, groupId, comments);
            }
        },
        OnGetFileInfo:function(data){
            if(this.fileComment){
                this.fileComment.OnGetFileInfo(data);
            }
        },
        noticeCommentCreateMessageReceived:function(conversationType, channel, from, to, msgId, msgSeq, fileId, fileType,
           fileTitle, commentId, userName, userId, content, createTime, timestamp,cloudType){
            if (this.fileComment != null && channel == uc.constants.Message.MessageChannel.UCAS){
                this.fileComment.OnCommentCreateMessageReceived(fileId, userName, userId, content, createTime);
            }
        },
        showFileCommentByFileId: function(groupId,fileId){
            if(fileId=="" || fileId=="null" || fileId==0)return;
            uc.IGroupService.getDocumentInfo(groupId, Number(fileId), function(args){
            });
        },
        noticeCheckGkFilePermission:function(data,cloudInfo,type,that){
            if(this.fileComment){
                switch(type) {
                    case 'comDownload':
                        this.fileComment.OnDownloadCloudFile(data,cloudInfo,that);
                        break;
                    case 'comForward':
                        this.fileComment.OnForwardCloudFile(data,cloudInfo,that);
                        break;
                    default:
                        break;
                }
            }
        },
        GkFileDownloaded:function(data){
            if(this.fileComment){
                this.fileComment.OnGkFileDownloadCallBack(data);
            }
        },
        fileListSaveCloud:function(dataStr){
            if(!uc.modules.chat.ChatManager.checkCloudOpenStatus()){
                return;
            }
            if(!uc.modules.chat.ChatManager.checkCloudLoadStatus()){
                return;
            }
            uc.modules.cloud.CloudManager.saveCloudFile("",dataStr);
        },
        checkCloudOpenStatus:function(){
            if(uc.modules.cloud.CloudManager.checkCloudIsLoginSuccess()==0){
                uc.modules.component.PromptMessageBox.open({title: uc_resource.MessageBox.Notification, message: uc_resource.MessageBox.cloudLoadLimit, buttonMode:"ok",OkText:uc_resource.MessageBox.OK},
                    function(result){
                        this.close();
                    });
                return false;
            }
            return true;
        },
        checkCloudLoadStatus:function(){
            if(uc.util.StorageUtil.getData('CloudLoginResult')!=0){
                uc.modules.component.PromptMessageBox.open({title: uc_resource.MessageBox.Notification, message: uc_resource.MessageBox.cloudLoadFail, buttonMode:"ok",OkText:uc_resource.MessageBox.OK},
                    function(result){
                        this.close();
                    });
                return false;
            }
            return true;
        },
        noticeCloudFileDownloaded:function(data){
            if(this.msgBox){
                this.msgBox.OnChatCloudFileDownloaded(data);
            }
            if(this.fileList){
                this.fileList.OnGkFileDownloadCallBack(data);
            }
            if(this.fileComment){
                this.fileComment.OnGkFileDownloadCallBack(data);
            }
        }
    };
})(uc);