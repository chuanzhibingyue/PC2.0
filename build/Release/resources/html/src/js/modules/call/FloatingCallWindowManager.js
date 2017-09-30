/**
 * Created by waouyang on 15/12/10.
 */
(function (uc) {
    var _this = null;
    uc.modules.call.FloatingCallWindowManager = {
        rejectCallMsgId: 0,
        callInfo: new uc.modules.call.CallInfo(),

        init: function () {
            _this = this;
            var callInfo = uc.util.StorageUtil.getData('floating_call_window_call_info');
	        var isEnd = uc.util.StorageUtil.getData('floating_call_window_close');
	        if(isEnd != null){
		        uc.util.StorageUtil.removeData('floating_call_window_close');
		        //如果在没有init完成时候，外部trigerWindowEvent(uc.constants.UI.CrossWindowEventName.CloseFloatingCallWindow，
		        // 直接关闭窗口
		        if(isEnd){
			       uc.util.WindowUtil.close();
			       uc.util.LogUtil.info("FloatingCallWindowManager","init_floating_call_window_close",isEnd);
		        }
		        return;
	        }
            this.callInfo.update(callInfo);
            this.bindCrossWindowEvent();
            uc.modules.call.FloatingCallWindow.init(this.callInfo);
        },

        bindCrossWindowEvent: function(){
            uc.IUIService.bindCrossWindowEvent(uc.constants.UI.CrossWindowEventName.UpdateInProgressCallInfo, function(callInfo){
                _this.updateInProgressCallInfo(callInfo);
            });


            uc.IUIService.bindCrossWindowEvent(uc.constants.UI.CrossWindowEventName.CloseFloatingCallWindow, function(eventData){
                if(eventData && eventData.isEnd){
                    uc.modules.call.FloatingCallWindow.callTurnToEnd();
                }else{
                    uc.modules.call.FloatingCallWindow.close();
                }
	            uc.util.StorageUtil.removeData('floating_call_window_close');
            });
        },

        updateInProgressCallInfo: function(callInfo){
            this.callInfo = callInfo;
            uc.modules.call.FloatingCallWindow.updateCallInfo(this.callInfo);
        },

        switchFloatingCallWindowToEmbedded: function (callInfo) {
            uc.IUIService.triggerCrossWindowEvent(uc.constants.UI.CrossWindowEventName.SwitchFloatingCallWindowToEmbedded, callInfo);
            uc.modules.call.FloatingCallWindow.close();
        },
        OnBasicContactInfoReceived: function (basicContactInfos) {
	        //解决消息接受到的时候，uc.modules.call.FloatingCallWindow还未初始化导致报错
	        if(!uc.modules.call.FloatingCallWindow.isInit){
		        var callInfo = uc.util.StorageUtil.getData('floating_call_window_call_info');
		        this.callInfo.update(callInfo);
		        uc.modules.call.FloatingCallWindow.init(this.callInfo);
	        }
            for (var i = 0, len = basicContactInfos.length; i < len; i++) {
                if (uc.modules.call.FloatingCallWindow.callInfo.chatType == uc.constants.Chat.ChatType.SingleChat) {
                    if (uc.modules.call.FloatingCallWindow.callInfo.chatId == basicContactInfos[i].userId) {
                        uc.modules.call.FloatingCallWindow.OnBasicContactInfoReceived(basicContactInfos[i]);
                        uc.modules.call.FloatingCallWindow.updateContact(basicContactInfos[i]);
                    }
                } else {
                    if (uc.modules.call.FloatingCallWindow.callInfo.userList) {
                        for (var userId in uc.modules.call.FloatingCallWindow.callInfo.userList) {
                            if (userId == basicContactInfos[i].userId) {
                                if(!uc.modules.call.FloatingCallWindow.callInfo.userList[userId]){
                                    uc.modules.call.FloatingCallWindow.callInfo.userList[userId]={};
                                    uc.modules.call.FloatingCallWindow.callInfo.userList[userId].userInfo={};
                                }
                                if(!uc.modules.call.FloatingCallWindow.callInfo.userList[userId].userInfo){
                                    uc.modules.call.FloatingCallWindow.callInfo.userList[userId].userInfo={};
                                }
                                uc.modules.call.FloatingCallWindow.callInfo.userList[userId].userInfo.displayName = basicContactInfos[i].displayName;
                                uc.modules.call.FloatingCallWindow.callInfo.userList[userId].userInfo.localAvatar = basicContactInfos[i].localAvatar;
                                uc.modules.call.FloatingCallWindow.OnBasicContactInfoReceived(basicContactInfos[i]);
                            }
                        }
                    }
                }
            }
        },

        OnGroupInfoReceived: function (group) {
            uc.modules.call.FloatingCallWindow.OnGroupInfoReceived(group);
        },

        OnCallSpeakingStatusChanged: function (meetingId, userId, chatType, isSpeaking) {
	        //解决消息接受到的时候，uc.modules.call.FloatingCallWindow还未初始化导致报错
	        if(!uc.modules.call.FloatingCallWindow.isInit){
		        var callInfo = uc.util.StorageUtil.getData('floating_call_window_call_info');
		        this.callInfo.update(callInfo);
		        uc.modules.call.FloatingCallWindow.init(this.callInfo);
	        }
            if (meetingId == uc.modules.call.FloatingCallWindow.callInfo.callMeetingId) {
                if (isSpeaking) {
                    if (userId == uc.modules.call.FloatingCallWindow.callInfo.chatId && chatType == uc.constants.Chat.ChatType.SingleChat) {
                        var status = isSpeaking ?  uc.constants.Call.AcceptorVideoStatus.Speaking : uc.constants.Call.MuteStatus.UN_MUTE;
                        uc.modules.call.FloatingCallWindow.setUserVideoStatus(status);
                    }
                }
            }
        },
        OnGroupNameUpdatedMessageReceived: function(conversationType, channel, from, to, operatorId, groupId, groupName, namePinYin, msgId, msgSeq, msgTime){
            if (channel != uc.constants.Message.MessageChannel.UCAS) {
                return;
            }
	        //解决消息接受到的时候，uc.modules.call.FloatingCallWindow还未初始化导致报错
	        if(!uc.modules.call.FloatingCallWindow.isInit){
		        var callInfo = uc.util.StorageUtil.getData('floating_call_window_call_info');
		        this.callInfo.update(callInfo);
		        uc.modules.call.FloatingCallWindow.init(this.callInfo);
	        }
            if (groupId == uc.modules.call.FloatingCallWindow.callInfo.chatId && conversationType == uc.modules.call.FloatingCallWindow.callInfo.chatType) {
                uc.modules.call.FloatingCallWindow.updateGroupName(groupName);
            }
        },
	    //监听桌面共享状态
	    onOpenScreenSharingWindow:function(conferenceId, userId){
		    uc.util.LogUtil.info('FloatingCallWindowManager', 'onOpenScreenSharingWindow', "callInfo:",this.callInfo);
		    if(this.callInfo.callMeetingId != conferenceId){
			    uc.util.LogUtil.info('FloatingCallWindowmanager', 'onOpenScreenSharingWindow', "conferenceId is differance:",conferenceId);
			    return;
		    }
		    //共享者
		    if(this.callInfo.desktopShareStatus == uc.constants.Call.DesktopShareStatus.OPEN){
			    //uc.util.StorageUtil.setData('desktopShareData',this.callInfo);
		    }else{
			    //共享接受者如果正在进行接受共享
			    if(this.callInfo.desktopShareAcceptStatus == uc.constants.Call.DesktopShareStatus.OPEN) {
				    //当前正在接受共享
				    return;
			    }
			    //uc.util.StorageUtil.setData('desktopShareAcceptData',this.callInfo);
			    this.callInfo.desktopShareAcceptStatus = uc.constants.Call.DesktopShareStatus.OPEN;
		    }
		    uc.modules.call.FloatingCallWindow.updateBottomBarStatus(this.callInfo);
		},
	    OnDesktopShareStoped:function(conferenceId, userId){
		    uc.util.LogUtil.info('FloatingCallWindowManager', 'OnDesktopShareStoped', "callInfo:",this.callInfo);
		    if(this.callInfo.callMeetingId != conferenceId){
			    uc.util.LogUtil.info('FloatingCallWindowmanager', 'OnDesktopShareStoped', "conferenceId is differance:",conferenceId);
			    return;
		    }
		    //共享者
		    if(this.callInfo.desktopShareStatus == uc.constants.Call.DesktopShareStatus.OPEN){
			    this.callInfo.desktopShareStatus = uc.constants.Call.DesktopShareStatus.CLOSE;
		    }else{
			    //发送stopDesktopView消息
			    if(this.callInfo.desktopShareAcceptStatus == uc.constants.Call.DesktopShareStatus.OPEN) {
				    this.callInfo.desktopShareAcceptStatus = uc.constants.Call.DesktopShareStatus.CLOSE;
			    }
		    }
		    uc.modules.call.FloatingCallWindow.updateBottomBarStatus(this.callInfo);
	    },
	    OnDesktopViewerShareStarted:function(conferenceId){

	    },
	    OnDesktopViewerShareStoped:function(conferenceId){

	    }
    }
})(uc);