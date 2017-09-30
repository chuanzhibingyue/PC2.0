/**
 * Created by senlin.ouyang on 2015/7/20.
 */
(function(uc){
    uc.modules.main.MainEventListener = function(){
        this.moduleId = uc.constants.UI.ModuleId.Message;
        var loginUserInfo = uc.IContactService.getCurrentUserInfo();
        if(loginUserInfo){
            this.loginUerId = loginUserInfo.userId;
        }
        this.playingSoundId = 0;
    };
    jQuery.extend(uc.modules.main.MainEventListener.prototype,
        uc.interfaces.IUIEvent,
        uc.interfaces.IClientEvent,
        uc.interfaces.IIMEvent,
        uc.interfaces.IGroupEvent,
        uc.interfaces.IContactEvent,
        uc.interfaces.IMeetingEvent,
        uc.interfaces.IOpenApiEvent,
        uc.interfaces.IUpdateEvent,{
            OnModuleChanged: function(moduleId){
                uc.modules.main.Main.OnModuleChanged(moduleId);
            },
            OnUserAvatarDownloaded: function(userId, serverAvatarUrl, localAvatarUrl){
                uc.modules.main.Main.OnUserAvatarDownloaded(userId, serverAvatarUrl, localAvatarUrl);
            },
            OnLogin: function(errorCode, authType, pbxType, capability){
                uc.modules.main.Main.OnLogin(errorCode, authType, pbxType, capability);
            },
            OnLogout: function(errorCode){
                uc.modules.main.Main.OnLogout(errorCode);
            },
            OnUCASLogin: function(errorCode){
                uc.modules.main.Main.OnUCASLogin(errorCode);
            },
            OnUCASLogout: function(errorCode){
                uc.modules.main.Main.OnUCASLogout(errorCode);
            },
            OnSessionExpired: function(){
                uc.modules.main.Main.OnSessionExpired();
            },
            OnPasswordModified: function(errorCode){
                uc.modules.main.Main.OnPasswordModified(errorCode);
            },
            OnPasswordModifiedMessageReceived: function(){
                uc.modules.main.Main.OnPasswordModifiedMessageReceived();
            },
            OnPlayingSoundNotification: function(id, percent, seconds){
                this._onPlayingSoundNotification(id, percent, seconds);
            },
            OnNetworkStatusChanged: function(status){
                uc.modules.main.Main.OnNetworkStatusChanged(status);
            },
            OnNewMessageCountChanged: function(count){
                uc.modules.main.Main.OnNewMessageCountChanged(count);
            },
            /*** IM Event ***/
            OnTextMessageReceived: function(channel, sendState, from, to, msgId, msgSeq, textType, textData, sendTime, timestamp){
                this.OnMessageReceived(channel, from.userId);
            },
            OnImageMessageReceived: function(channel, sendState, from, to, msgId, msgSeq, imageName, imageSize, imageURL, imageData, timestamp){
                this.OnMessageReceived(channel, from.userId);
            },
            OnAudioMessageReceived: function(channel, sendState, from, to, msgId, msgSeq, audioName, audioSize, audioURL, audioDuration, timestamp){
                this.OnMessageReceived(channel, from.userId);
            },
            OnVideoMessageReceived: function(channel, sendState, from, to, msgId, msgSeq, videoName, videoSize, videoURL, videoThumb, duration, timestamp){
                this.OnMessageReceived(channel, from.userId);
            },
            OnFileReceived: function(channel, sendState, from, to, msgId, msgSeq,fileName, fileSize, fileURL, timestamp){
                this.OnMessageReceived(channel, from.userId);
            },
            OnCloudFileReceived: function(channel, sendState, from, to, msgId, msgSeq,fileName, fileSize, timestamp,detailContent){
                this.OnMessageReceived(channel, from.userId);
            },
            /*** Group Event ***/
            OnGroupTextMessageReceived: function(conversationType, channel, sendState, from, to, msgId, msgSeq, textType, textContent, msgTime, atUsers){
                this.OnMessageReceived(channel, from.userId, to.userId);
            },
            OnGroupImageMessageReceived: function(conversationType, channel, sendState, from, to, msgId, msgSeq, fileName, fileSize, fileURL, thumbnails, msgTime){
                this.OnMessageReceived(channel, from.userId, to.userId);
            },
            OnGroupAudioMessageReceived: function(conversationType, channel, sendState, from, to, msgId, msgSeq, fileName, fileSize, fileURL, duration, msgTime){
                this.OnMessageReceived(channel, from.userId, to.userId);
            },
            OnGroupVideoMessageReceived: function(conversationType, channel, sendState, from, to, msgId, msgSeq, fileName, fileSize, fileURL, thumbnails, duration, msgTime){
                this.OnMessageReceived(channel, from.userId, to.userId);
            },
            /*** Meeting Event ***/
            OnMeetingTextMessageReceived: function(channel, sendState, from, to, meetingId, eventId, meetingStartTime, textType, textContent, msgId, msgSeq, msgTime){
                this.OnMessageReceived(channel, from.userId);
            },
            OnMeetingImageMessageReceived: function(channel, sendState, from, to, meetingId, eventId, meetingStartTime, fileName, fileSize, fileURL, thumbnails, msgId, msgSeq, msgTime){
                this.OnMessageReceived(channel, from.userId);
            },
            OnMeetingAudioMessageReceived: function(channel, sendState, from, to, meetingId, eventId, meetingStartTime, fileName, fileSize, fileURL, duration, msgId, msgSeq,msgTime){
                this.OnMessageReceived(channel, from.userId);
            },
            OnMeetingVideoMessageReceived: function(channel, sendState, from, to, meetingId, eventId, meetingStartTime, fileName, fileSize, fileURL, thumbnails, duration, msgId, msgSeq, msgTime){
                this.OnMessageReceived(channel, from.userId);
            },
            OnMeetingInviteMessageReceived: function(channel, from, to, meetingId, eventId, meetingName, accreditUserId, hostId, isRecurrent, meetingStartTime, msgId, msgSeq, msgTime){
                this.OnMessageReceived(channel, from.userId);
            },
            OnMeetingForwardedMessageReceived: function(channel, from, to, meetingId, eventId, meetingName, accreditUserId, hostId, isRecurrent, meetingStartTime, msgId, msgSeq, msgTime){
                this.OnMessageReceived(channel, from.userId);
            },

            OnOpenApiTextMessageReceived: function(channel, sendState, from, to, textTitle, textContent, textType, detailContent, msgId, msgSeq, mstTime){
                this.OnMessageReceived(channel, from.userId == this.loginUerId ? to.userId : from.userId);
            },
            OnOpenApiImageMessageReceived: function(channel, sendState, from, to, imageId, imageType, imageData, msgId, msgSeq, msgTime){
                this.OnMessageReceived(channel, from.userId == this.loginUerId ? to.userId : from.userId);
            },
            OnOpenApiFileMessageReceived: function(channel, sendState, from, to, fileId, fileType, fileName, fileSize, msgId, msgSeq, mstTime){
                this.OnMessageReceived(channel, from.userId == this.loginUerId ? to.userId : from.userId);
            },
            OnOAMessageReceived: function(channel, from, to, title, titleColor, titleBkColor, titleElements, status, data, detailUrl, msgId, msgSeq,msgTime) {
                this.OnMessageReceived(channel, from.userId == this.loginUerId ? to.userId : from.userId);
            },

            OnMessageReceived: function(channel, fromUserId, toUserId){
                if(toUserId){
                    var groupInfo = uc.IGroupService.getGroupInfo(toUserId);
                    if(groupInfo){
                        var groupDisturbState = groupInfo.disturbState;
                        var groupType = groupInfo.groupType;
                        uc.util.LogUtil.info('MainEventListener','getGroupInfo','groupInfo disturbState',{groupType:groupType,groupDisturbState:groupDisturbState});
                        if(groupDisturbState){
                            return;
                        }
                    }
                }
                if(channel == uc.constants.Message.MessageChannel.UCAS && fromUserId != this.loginUerId){
                    uc.modules.main.Main.OnMessageReceived();
                    this.playMessageNotificationSound();
                }
            },
            playMessageNotificationSound: function(){
                if(this.playingSoundId || !uc.settings.msgPrompt.playPrompt){
                    return;
                }
                this.playingSoundId = 'msg-notification-sound';
                uc.util.AudioUtil.playMessageNotificationSound(this.playingSoundId);
            },
            _onPlayingSoundNotification: function(id, percent, seconds){
                if(percent == 100 && id == 'msg-notification-sound'){
                    this.playingSoundId = '';
                }
            },
            OnCheckUpdates : function (errorCode, updateFlag, version, notes) {
                uc.util.LogUtil.info('MainEventListener', 'OnCheckUpdates', 'main check update timer', {errorCode: errorCode, updateFlag: updateFlag});
                uc.modules.main.Main.OnCheckUpdates(errorCode, updateFlag, version, notes);
            },
            OnCloudLogin:function(errorCode){
                uc.util.LogUtil.info('Main', 'OnCloudLogin', 'errorCode = ' + errorCode, null);
                uc.util.StorageUtil.setData('CloudLoginResult',errorCode);
                uc.modules.cloud.CloudManager.OnPreloadCloud(errorCode);
            },
            OnUserStatusReceived: function(errorCode, status){
                uc.util.LogUtil.info('MainEventListener', 'OnUserStatusReceived', '', {errorCode: errorCode, status: status});
                if(errorCode == 0){
                    uc.modules.main.Main.OnUserStatusReceived(status);
                }
            },
            OnUserStatusSetted: function(errorCode, userId, status){
                uc.util.LogUtil.info('MainEventListener', 'OnUserStatusSetted', '', {errorCode: errorCode, userId: userId, status: status});
                if(errorCode == 0){
                    uc.modules.main.Main.renderCurrentUserStatus(userId, status);
                }
            },
            OnGetUserStatusResponse: function(errorCode, status, copyBack){
                uc.util.LogUtil.info('MainEventListener', 'OnGetUserStatusResponse', 'data=', {errorCode: errorCode, status: status, copyBack: copyBack});
                if(errorCode != 0 || !copyBack){
                    return ;
                }

                var copyBackObj = $.parseJSON(copyBack);
                var panel = copyBackObj.panel ? copyBackObj.panel : '';
                if(!panel || panel != "Main"){
                    return ;
                }

                uc.modules.main.Main.OnUserStatusReceived(status);
            },
	        OnFileSendToBee:function(filePath){
                uc.modules.main.Main.OnFileSendToBee(filePath);
            },
            OnUserLoginNotify:function(errorCode,message){
                uc.modules.main.Main.OnUserLoginNotify(errorCode,message);
            }
        })
})(uc);