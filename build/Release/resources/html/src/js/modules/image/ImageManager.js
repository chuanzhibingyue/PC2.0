/**
 * Created by waouyang on 15/8/18.
 */
(function(uc){
    var avatarDownloadStatus = {};
    var avatarDownloadTaskData = {};
    var chatImageDownloadTaskData = {};
    var AvatarDownloadStatus = {
        Downloading: 0,
        Downloaded: 1,
        Failed: 2
    };
    var MaxDownloadTryTime = 3;
    uc.modules.image.ImageManager = {
        downloadUserAvatar: function(userId, avatarUrl){
            uc.util.LogUtil.info('ImageManager', 'downloadUserAvatar', 'userId = ' + userId + ', avatarUrl = ' + avatarUrl, null);
            this._downloadAvatar(userId, uc.constants.UI.AvatarType.User, avatarUrl);
        },
        downloadProjectAvatar: function(groupId, avatarUrl){
            uc.util.LogUtil.info('ImageManager', 'downloadProjectAvatar', 'projectId = ' + groupId + ', avatarUrl = ' + avatarUrl, null);
            this._downloadAvatar(groupId, uc.constants.UI.AvatarType.Project, avatarUrl);
        },
        _downloadAvatar: function(id, type, serverUrl){
            if(serverUrl.indexOf('http') == -1 && serverUrl.indexOf('ftp') == -1){
                return;
            }
            var key = this._getKey(id, type, serverUrl);
            if(avatarDownloadStatus[key]){
                if(avatarDownloadStatus[key]['downloadStatus'] == AvatarDownloadStatus.Downloading ||
                    avatarDownloadStatus[key]['downloadStatus'] == AvatarDownloadStatus.Downloaded){
                    return;
                }else if(avatarDownloadStatus[key]['downloadStatus'] == AvatarDownloadStatus.Failed && avatarDownloadStatus[key]['tryTime'] == MaxDownloadTryTime){
                    return;
                }
            }else{
                avatarDownloadStatus[key] = {
                    tryTime : 0
                };
            }

            var savePath = this._getImageDirectory() + '\\Avatar\\' + id + '_' + type + '_' + (new Date().getTime());
            uc.util.LogUtil.info('ImageManager', '_downloadAvatar', 'id = ' + id + ', localAvatarUrl  = ' + savePath, null);
            uc.util.AvatarUtil.downloadAvatar(serverUrl, savePath, function(args){
                var errorCode, taskId;
                if(args && args.length >= 2){
                    errorCode = args[0];
                    if(errorCode == uc.ErrorCode.Success){
                        taskId = args[1];
                        avatarDownloadStatus[key]['downloadStatus'] = AvatarDownloadStatus.Downloading;
                        avatarDownloadStatus[key]['tryTime'] = ++avatarDownloadStatus[key]['tryTime'];
                        avatarDownloadTaskData[taskId] = {
                            id: id,
                            type: type,
                            serverUrl: serverUrl
                        }
                    }
                }
            })
        },
        _getKey: function(id, type, serverUrl){
            return id + '_' + type + '_' + serverUrl;
        },
        _getImageDirectory: function(){
            if(this.imageDirectory){
                return this.imageDirectory;
            }
            return this.imageDirectory = uc.IClientService.getDataDirectory(uc.constants.UI.DirectoryType.Image);
        },
        downloadChatImage: function(chatId, chatType, imageServerPath, msgId){
            var savePath = this._getImageDirectory() + '\\' + chatId + '_' + chatType + '\\' + msgId;
            uc.IFSService.downloadFile(imageServerPath, savePath, function(args){
                var errorCode, taskId;
                if(args && args.length >= 2){
                    errorCode = args[0];
                    if(errorCode == uc.ErrorCode.Success){
                        taskId = args[1];
                        chatImageDownloadTaskData[taskId] = {
                            id: chatId,
                            type: chatType,
                            serverUrl: imageServerPath,
                            msgId: msgId
                        }
                    }
                }
            });
        },
        OnFileDownloaded: function(errorCode, taskId, savePath){
            var id, type, serverUrl, msgId, key;
            if(errorCode == uc.ErrorCode.Success){
                if(avatarDownloadTaskData[taskId]){
                    id = avatarDownloadTaskData[taskId]['id'];
                    type = avatarDownloadTaskData[taskId]['type'];
                    serverUrl = avatarDownloadTaskData[taskId]['serverUrl'];
                    key = this._getKey(id, type, serverUrl);
                    avatarDownloadStatus[key]['downloadStatus'] = AvatarDownloadStatus.Downloaded;
                    if(type == uc.constants.UI.AvatarType.User){
                        uc.IUIEvent.OnUserAvatarDownloaded(id, serverUrl, savePath);
                        uc.util.LogUtil.info('ImageManager', 'OnUserAvatarDownloaded', 'userId = ' + id + ', localPath = ' + savePath + ', serverUrl = ' + serverUrl, null);
                        uc.IContactService.setContactLocalAvatarUrl(id, serverUrl, savePath);
                    }else{
                        uc.IUIEvent.OnProjectAvatarDownloaded(id, serverUrl, savePath);
                        uc.util.LogUtil.info('ImageManager', 'OnProjectAvatarDownloaded', 'groupId = ' + id + ', localPath = ' + savePath + ', serverUrl = ' + serverUrl, null);
                        uc.IGroupService.updateGroupAvatar(id, savePath, function(){});
                    }
                }else if(chatImageDownloadTaskData[taskId]){
                    id = chatImageDownloadTaskData[taskId]['id'];
                    type = chatImageDownloadTaskData[taskId]['type'];
                    serverUrl = chatImageDownloadTaskData[taskId]['serverUrl'];
                    msgId = chatImageDownloadTaskData[taskId]['msgId'];
                    uc.IUIEvent.OnChatImageDownloaded(id, type, serverUrl, savePath, msgId);
                    uc.IFSService.updateFileLocalPath(serverUrl, savePath);
                }
            }else{
                if(avatarDownloadTaskData[taskId]){
                    id = avatarDownloadTaskData[taskId]['id'];
                    type = avatarDownloadTaskData[taskId]['type'];
                    serverUrl = avatarDownloadTaskData[taskId]['serverUrl'];
                    key = this._getKey(id, type, serverUrl);
                    avatarDownloadStatus[key]['downloadStatus'] = AvatarDownloadStatus.Failed;
                    avatarDownloadStatus[key]['tryTime'] = ++avatarDownloadStatus[key]['tryTime'];
                }
            }
        },
        OnPreviewChatImage: function(chatId, chatType, msgSeq, imageName){
            var imageFolder = this._getImageDirectory() + '\\' + chatId + '_' + chatType;
            var imageNameList = uc.util.FileUtil.getDirectoryFileList(imageFolder);
            var imageList = [];
            if(imageNameList && imageNameList.length){
                var imageFileName;
                for(var i = 0, len = imageNameList.length; i < len; i++){
                    imageFileName = imageNameList[i];
                    if(imageFileName){
                        imageList.push(imageFolder + '\\' + imageFileName);
                    }
                }
            }
            var imagePath = imageFolder + '\\' + msgSeq + '_' + imageName;
            uc.util.StorageUtil.setData('image-preview-data',{
                imageList: imageList,
                imagePath: imagePath
            });
            uc.util.LogUtil.info('ImageManager', 'OnPreviewChatImage', 'setImageData', {
                imageList: imageList,
                imagePath: imagePath
            });
            this.openImagePreviewerWindow();
        },
        OnPreviewImage: function(imagePath){
            uc.util.StorageUtil.setData('image-preview-data',{
                imageList: [imagePath],
                imagePath: imagePath
            });
            this.openImagePreviewerWindow();
        },
        openImagePreviewerWindow: function(){
            var options = {
                title: uc_resource.WindowTitle.ImagePreview,
                width:  screen.availWidth,
                height: screen.availHeight,
                left: 0,
                top: 0,
                refreshIfExist: false,
                centerScreen: false,
                modal: true,
                resizable: false,
                minWidth: screen.availWidth,
                minHeight: screen.availHeight,
                behaviorIfSCClose: uc.constants.UI.WindowCloseBehavior.Close,
                borderSpace: {
                    leftBorder: 0,
                    topBorder: 0,
                    rightBorder: 0,
                    bottomBorder: 0
                },
                removeBorder: true
            };
            uc.util.WindowUtil.open('page/imagePreviewer.html', 'imagePreviewer', options);
        },
        OnOpenImageCropperWindow: function(key){
        	uc.util.StorageUtil.setData("imageCropper", key);
            var options = {
            	width: 580,
	            height: 500,
                refreshIfExist: false,
                centerScreen: true,
                modal: true,
                resizable: true,
                minWidth: 580,
                minHeight: 500,
                caption: uc.constants.UI.DragCaption.ImageCropper
            };
            uc.util.WindowUtil.open('page/imageCropper.html', 'imageCropper', options);
        }
    }
})(uc);