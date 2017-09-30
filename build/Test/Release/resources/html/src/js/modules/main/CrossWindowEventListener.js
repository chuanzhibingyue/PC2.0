/**
 * Created by waouyang on 15/9/18.
 */
(function(uc){
    var CrossWindowEventData = {
        OnSettingChanged: {
            eventName: uc.constants.UI.CrossWindowEventName.OnSettingChanged,
            callback: function(eventData){
                if (eventData.action == "apply") {
                    uc.modules.setting.SettingManager.loadAndApplySetting();
                } else if (eventData.action == "disableHotKey") {
                    uc.util.SystemUtil.unRegisterHotKey( uc.constants.UI.HotKeyType.CaptureScreen);
                    uc.util.SystemUtil.unRegisterHotKey( uc.constants.UI.HotKeyType.ShowClient);
                }
            }
        },
        ModifyingPassword: {
            eventName: uc.constants.UI.CrossWindowEventName.ModifyingPassword,
            callback: function(){
                uc.modules.main.Main.ModifyingPassword();
            }
        },
        OnRestartClient: {
            eventName: uc.constants.UI.CrossWindowEventName.OnRestartClient,
            callback: function(){
                uc.modules.main.Main.logoutClient();
            }
        },
        OpenChatWindow: {
            eventName: uc.constants.UI.CrossWindowEventName.OpenChatWindow,
            callback: function(eventData){
	            if(eventData && !eventData.isFromCall){
		            uc.util.WindowUtil.restore();
	            }
            	uc.IUIService.openChatWindow(eventData.chatType, eventData.Id, uc.constants.UI.OpenChatWindowSourceModule.Other, null, null,true,eventData.isFromCall);
            }
        },
        OnProxyAuthenticateSet: {
        	eventName: uc.constants.UI.CrossWindowEventName.OnProxyAuthenticateSet,
            callback: function(){
            	uc.modules.main.Main.OnProxyAuthenticateSet();
            }
        },
        OnForwardMeetingVideo:{
            eventName: uc.constants.UI.CrossWindowEventName.OnForwardMeetingVideo,
            callback: function(eventData){
                var args=eventData.args;
                var videoName=eventData.videoName, videoSize=parseInt(eventData.videoSize),
                    videoURL=eventData.videoURL, videoData=eventData.videoData,
                    videoThumbUrl=eventData.thumbnailsURL,
                    videoDuration=parseInt(eventData.videoDuration),operatorName=eventData.operatorName,
                    operatorId=parseInt(eventData.operatorId),operatorAvatar=eventData.operatorAvatar;
                if (!args.okOrCancel) {
                    return;
                }
                var conversationType, toId;
                if (args.otherItem.groupChats.length > 0) {
                    conversationType = uc.constants.Chat.ChatType.GroupChat;
                    toId = args.otherItem.groupChats[0];
                } else if (args.otherItem.projects.length > 0) {
                    conversationType = uc.constants.Chat.ChatType.ProjectChat;
                    toId = args.otherItem.projects[0];
                } else if (args.contacts.length > 0) {
                    conversationType = uc.constants.Chat.ChatType.SingleChat;
                    toId = args.contacts[0].userId;
                } else {
                    return;
                }
                switch (conversationType) {
                    case uc.constants.Chat.ChatType.SingleChat:
                        uc.IIMService.sendImRecordVideoMessage(
                            {
                                userId: toId,
                                resId: 100,
                                siteId: uc.modules.chat.ChatManager.myUserInfo.siteId
                            }, videoName, videoSize, videoURL, videoData, videoThumbUrl,videoDuration,operatorId,operatorName,operatorAvatar,
                            function (args) {
                                console.log(args);
                                if (args[0] !== 0) {
                                    console.log(args, conversationType, toId, videoName, videoSize, videoURL);
                                    return;
                                }
                                uc.IUIEvent.OnMessageSending({
                                    chatId: toId,
                                    chatType: conversationType,
                                    taskId: 0,
                                    msgId: args[1],
                                    contentType: uc.constants.Message.ContentType.Video,
                                    meetingName:videoName
                                })
                            });
                        break;

                    case uc.constants.Chat.ChatType.GroupChat:
                    case uc.constants.Chat.ChatType.ProjectChat:
                        uc.IGroupService.sendGroupVideoMessage(
                            conversationType,
                            {
                                userId: toId,
                                resId: 100,
                                siteId: uc.modules.chat.ChatManager.myUserInfo.siteId
                            }, videoName, videoSize, videoURL, videoData, videoThumbUrl, videoDuration,operatorId,operatorName,operatorAvatar,
                            function (args) {
                                console.log(args);
                                if (args[0] !== 0) {
                                    console.log(args, conversationType, toId, videoName, videoSize, videoURL);
                                    return;
                                }
                                uc.IUIEvent.OnMessageSending({
                                    chatId: toId,
                                    chatType: conversationType,
                                    taskId: 0,
                                    msgId: args[1],
                                    contentType: uc.constants.Message.ContentType.Video,
                                    meetingName: videoName
                                })
                            });
                        break;
                }
            }
        },
        OnMeetingVideoMake:{
            eventName: uc.constants.UI.CrossWindowEventName.OnMeetingVideoMake,
            callback: function(eventData){
                var videoURL=eventData.videoURL,domain;

                uc.IClientService.getUccServerDomain(function(arg){
                    domain=arg[1];
                    var url=""+domain+"//"+"microblog/beeweb/theme/glaucous/qsplayer/meetingVideo.html"+"#"+videoURL;
                    window.open(url);
                });

            }
        },
        OnShareMeetingVideo:{
            eventName: uc.constants.UI.CrossWindowEventName.OnShareMeetingVideo,
            callback: function(eventData){
                var options = {
                    width: 540,
                    height: 380,
                    refreshIfExist: false,
                    centerScreen: true,
                    modal: true,
                    resizable: false,
                    caption: uc.constants.UI.DragCaption.EditMeetingMinutes
                };
                uc.util.StorageUtil.setData("Meeting-Video-Share:Data", eventData);
                uc.util.WindowUtil.open("page/shareMeetingVideo.html", "shareMeetingVideo", options);
            }
        },
        OpenSettingWindow: {
            eventName: uc.constants.UI.CrossWindowEventName.OpenSettingWindow,
            callback: function(){
                uc.IUIService.openSettingWindow();
            }
        },
        OpenChatWindowFromBubbleWindow: {
            eventName: uc.constants.UI.CrossWindowEventName.OpenChatWindowFromBubbleWindow,
            callback: function(eventData){
                uc.util.WindowUtil.show();
                uc.IUIService.openChatWindow(eventData.conversationType, eventData.conversationId, uc.constants.UI.OpenChatWindowSourceModule.Other, eventData.meetingData, null, true);
            }
        },
        SwitchFloatingCallWindowToEmbedded:{
            eventName: uc.constants.UI.CrossWindowEventName.SwitchFloatingCallWindowToEmbedded,
            callback: function(callInfo){
                uc.util.WindowUtil.show();
                uc.modules.call.CallManager.switchFloatingCallWindowToEmbedded(callInfo);
            }
        },
        RejectCallInvitation:{
            eventName: uc.constants.UI.CrossWindowEventName.RejectCallInvitation,
            callback: function(rejectedData){
                if(rejectedData.isClose){
                    uc.util.WindowUtil.show();
                    uc.modules.component.PromptMessageBox.open({ title: uc_resource.WindowTitle.Notification, message: uc_resource.Call.CloseFloatingTip, buttonMode: "both", OkText: uc_resource.Common.OK,CancelText: uc_resource.Common.Cancel }, function (result) {
                        if(!result){
                            uc.modules.call.CallManager.rejectCallInvitation(rejectedData.callInfo,rejectedData.reason);

                        }
                        this.close();
                    });
                }else{
                    uc.modules.call.CallManager.rejectCallInvitation(rejectedData.callInfo,rejectedData.reason);
                }
            }
        },
        CancelCall:{
            eventName: uc.constants.UI.CrossWindowEventName.CancelCall,
            callback: function(cancelData){
                if(cancelData.isClose){
                    uc.util.WindowUtil.show();
                    uc.modules.component.PromptMessageBox.open({ title: uc_resource.WindowTitle.Notification, message: uc_resource.Call.CloseFloatingTip, buttonMode: "both", OkText: uc_resource.Common.OK,CancelText: uc_resource.Common.Cancel }, function (result) {
                        if(!result){
                            uc.modules.call.CallManager.cancelCall(cancelData.callInfo);
                        }
                        this.close();
                    });
                }else{
                    uc.modules.call.CallManager.cancelCall(cancelData.callInfo);
                }
            }
        },
        LeaveCall:{
            eventName: uc.constants.UI.CrossWindowEventName.LeaveCall,
            callback: function(Data){
                if(Data.isClose){
                    uc.util.WindowUtil.show();
                    uc.modules.component.PromptMessageBox.open({ title: uc_resource.WindowTitle.Notification, message: uc_resource.Call.CloseFloatingTip, buttonMode: "both", OkText: uc_resource.Common.OK,CancelText: uc_resource.Common.Cancel }, function (result) {
                        if(!result){
                            uc.modules.call.CallManager.leaveCall(Data.callInfo);
                        }
                        this.close();

                    });
                }else{
                    uc.modules.call.CallManager.leaveCall(Data.callInfo);
                }
            }
        },
        LeaveCallSaveCallLogToPbx:{
            eventName: uc.constants.UI.CrossWindowEventName.LeaveCallSaveCallLogToPbx,
            callback: function(Data){
                uc.modules.call.CallManager.SaveCallLogToPbx(Data.callInfo);
            }
        },
        JoinCall:{
            eventName: uc.constants.UI.CrossWindowEventName.JoinCall,
            callback: function(callInfo){
                uc.modules.call.CallManager.joinCall(callInfo);
            }
        },
        IsMuteCall:{
            eventName: uc.constants.UI.CrossWindowEventName.IsMuteCall,
            callback: function(callInfo){
                uc.modules.call.CallManager.isMuteCall(callInfo);
            }
        },
        AddGroupMemberToCall:{
            eventName: uc.constants.UI.CrossWindowEventName.AddGroupMemberToCall,
            callback: function(eventData){
                var userStatus = [];
                for(var i = 0; i < eventData.userList.length; i++){
                    userStatus.push(eventData.userList[i].userId);
                }

                uc.util.LogUtil.info("crossWindowEventListener","AddGroupMemberToCall","data=",
                    {chatId: eventData.groupId,chatType:eventData.chatType, userList: userStatus});

	            var copyBack = '{"type":"' + eventData.chatType + '","userId":"' + eventData.groupId + '"}';
	            var userIds = userStatus;

	            uc.IContactService.getUserStatus(userIds,copyBack,function(arg){
                        var errorCode = arg[0];
                        if(errorCode == uc.ErrorCode.Success){
                            uc.modules.call.CallManager.changeCallUser = eventData.userList;
                            if(!uc.modules.call.CallManager.inProgressCallInfo.isInProgress){
                                uc.modules.call.CallManager.inProgressCallInfo.chatId = eventData.groupId;
                                uc.modules.call.CallManager.inProgressCallInfo.chatType = eventData.chatType;
                                uc.modules.call.CallManager.inProgressCallInfo.callMeetingId = eventData.meetingId+'';
                                uc.modules.call.CallManager.inProgressCallInfo.setIsInProgress(true);
                            }else{
                                if(uc.modules.call.CallManager.inProgressCallInfo.chatId == eventData.groupId){
                                }else{
                                    uc.modules.call.CallManager.invitedGroupMemberToCall={};
                                    uc.modules.call.CallManager.invitedGroupMemberToCall.chatId = eventData.groupId;
                                    uc.modules.call.CallManager.invitedGroupMemberToCall.chatType = eventData.chatType;
                                    uc.modules.call.CallManager.invitedGroupMemberToCall.callMeetingId = eventData.meetingId+'';
                                }
                            }
                        }
                    });
                }
        },
        minimizeWindow: {
            eventName: uc.constants.UI.CrossWindowEventName.MinimizeWindow,
            callback: function(){
                uc.IUIService.minimizeWindow();
            }
        },
        ReOpenCallWindowFormMessage:{
            eventName: uc.constants.UI.CrossWindowEventName.ReOpenCallWindowFormMessage,
            callback: function(eventData){
                uc.modules.call.CallManager.changeCallWindowFormMessage(eventData);
            }
        },
        ReOpenPbxChatDetail:{
            eventName: uc.constants.UI.CrossWindowEventName.ReOpenPbxChatDetail,
            callback: function(eventData){
                uc.modules.pbx.PbxCallManager.ReOpenPbxChatDetail(eventData);
            }
        },
	    startScreenShare:{
		    eventName: uc.constants.UI.CrossWindowEventName.startScreenShare,
		    callback: function(eventData){
			    uc.modules.call.CallManager.updateDesktopCallInfo(eventData);
			    uc.ISharingService.startDesktopShare(function(){});
		    }
	    },
	    stopScreenShare:{
		    eventName: uc.constants.UI.CrossWindowEventName.stopScreenShare,
		    callback: function(eventData){
			    uc.modules.call.CallManager.updateDesktopCallInfo(eventData);
			    uc.ISharingService.stopDesktopShare(function(){});
		    }
	    }
    };
    uc.modules.main.CrossWindowEventListener = {
        bindCrossWindowEvent: function(){
            var eventData, eventName, callback;

            for(var i in CrossWindowEventData){
                eventData = CrossWindowEventData[i];
                eventName = eventData.eventName;
                callback = eventData.callback;

                uc.IUIService.bindCrossWindowEvent(eventName, callback);
            }
        }
    }
})(uc);