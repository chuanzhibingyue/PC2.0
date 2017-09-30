
(function (uc) {
    uc.modules.comment.CommentEventListener = function () {

    };
    jQuery.extend(uc.modules.comment.CommentEventListener.prototype,
        uc.interfaces.IClientEvent,
        uc.interfaces.IUIEvent,
        uc.interfaces.IIMEvent,
        uc.interfaces.IGroupEvent,{
            OnCommentListReceived: function(errorCode, groupId, comments){
                uc.modules.comment.CommentManager.OnCommentListReceived(errorCode, groupId, comments);
            },
            OnCommentCreated: function(errorCode, groupId, comment,msgId){
                uc.modules.comment.CommentManager.noticeCommentCreated(errorCode, groupId, comment,msgId);
            },
            OnCommentCreateMessageReceived:function(conversationType, channel, from, to, msgId, msgSeq, fileId, fileType, fileTitle, commentId, userName, userId, content, createTime, timestamp,cloudType){
                uc.modules.comment.CommentManager.noticeCommentCreateMessageReceived(conversationType, channel, from, to, msgId, msgSeq, fileId, fileType, fileTitle, commentId, userName, userId, content, createTime, timestamp,cloudType);
            },
            OnDocumentUploaded: function (errorCode, groupId, operatorId, document, msgId) {
                if(uc.modules.comment.CommentManager.fileComment){
                    if (errorCode == 0){
                        uc.modules.comment.CommentManager.showFileCommentPage(groupId,document,document.fileId);
                    } else{
                        uc.modules.component.PromptMessageBox.open({title: uc_resource.GroupChat.Notification, message:  uc_resource.Cloud.NotGroup, buttonMode:"ok", OkText:uc_resource.MessageBox.OK},
                            function(result){
                                this.close();
                            });
                    }
                }
            },
            OnDocInfoReceived:function(errorCode, groupId, document){
                if (errorCode == 10902) {
                  //  uc.modules.comment.CommentManager.hideFileCommentPage();
                   uc.modules.component.PromptMessageBox.open({title: uc_resource.GroupChat.Notification, message:  uc_resource.GroupChat.FileDelete, buttonMode:"ok", OkText:uc_resource.MessageBox.OK},
                        function(result){
                            this.close();
                        });
                }else{
                    if(uc.modules.comment.CommentManager.isFromChatComment){
                        uc.modules.comment.CommentManager.ChatCommentMessageSuccess(groupId,document,document.fileId);
                    }else{
                        uc.modules.comment.CommentManager.showFileCommentPage(groupId,document,document.fileId);
                    }
                }
            },
            OnGroupInfoReceived: function (errorCode, groupId,mountId, group){
                uc.modules.comment.CommentManager.getGroupInfo(errorCode,groupId,mountId,group);
                uc.modules.cloud.CloudManager.getGroupInfo(errorCode,groupId,mountId,group);
            },
            OnGkFileDownloadCallBack:function(data){
                uc.modules.comment.CommentManager.GkFileDownloaded(data);
            },
            OnGkGetFileInfo:function(data,cloudData,Type){
                uc.modules.comment.CommentManager.OnGetFileInfo(data);
            }
        });
})(uc);