/**
 * Created by waouyang on 15/12/7.
 */
(function(uc){
    uc.modules.call.CallEventListener = function(){

    };
    jQuery.extend(uc.modules.call.CallEventListener.prototype,
        uc.interfaces.IUIEvent,
        uc.interfaces.ICallEvent,
        uc.interfaces.IContactEvent,
        uc.interfaces.IIMEvent,
        uc.interfaces.IGroupEvent,
	    uc.interfaces.ISharingEvent,{
            /** IUIEvent */
            OnInitClient: function(clearData){
                uc.modules.call.CallManager.OnInitClient(clearData);
            },
            OnOpenChatWindow: function(chatType, chatId, openChatWindowSourceModule, meetingData, siteId){
                uc.modules.call.CallManager.OnOpenChatWindow(chatType, chatId, openChatWindowSourceModule);
            },
            OnModuleChanged: function(moduleId, formCloudMountId){
                uc.modules.call.CallManager.OnModuleChanged(moduleId);
            },
            OnMinimizeWindow:function(){
                uc.modules.call.CallManager.OnMinimizeWindow();
            },
            /** ICallEvent */
            OnCallCreated: function(errorCode, meetingId, chatId, chatType){
                uc.modules.call.CallManager.OnCallCreated(errorCode, meetingId, chatId, chatType);
            },
            OnSelfJoinedCall: function(errorCode, meetingId, chatId, chatType){
                uc.modules.call.CallManager.OnSelfJoinedCall(errorCode, meetingId, chatId, chatType);
            },
            OnSelfLeftCall: function(errorCode, meetingId, chatId, chatType){
                uc.modules.call.CallManager.OnSelfLeftCall(errorCode, meetingId, chatId, chatType);
            },
            OnUserJoinedCall: function(meetingId, userId, isMute, audioType, phoneNumber){
                uc.modules.call.CallManager.OnUserJoinedCall(meetingId, userId, isMute, audioType, phoneNumber);
            },
            OnUserLeftCall: function(meetingId, userId){
                uc.modules.call.CallManager.OnUserLeftCall(meetingId,userId);
            },
            OnUserCallPropertyChanged: function(meetingId, userId, isMute, audioType, phoneNumber, callPropertyChangedType){
                uc.modules.call.CallManager.OnUserCallPropertyChanged(meetingId, userId, isMute, audioType, phoneNumber, callPropertyChangedType);
            },
            OnCallPhoneFailed: function(errorCode, meetingId, phoneNumber){
                uc.modules.call.CallManager.OnCallPhoneFailed(errorCode, meetingId, phoneNumber);
            },
            OnCallDisconnected: function(errorCode,meetingId){
                uc.modules.call.CallManager.OnCallDisconnected(errorCode,meetingId);
            },
            OnCallReconnected: function(errorCode,meetingId){
                uc.modules.call.CallManager.OnCallReconnected(errorCode,meetingId);

            },
            /** IContactEvent */
            OnBasicContactInfoReceived: function(errorCode, userIds, contacts){
                if(errorCode == uc.ErrorCode.Success && contacts && contacts.length){
                    uc.modules.call.CallManager.OnBasicContactInfoReceived(contacts);
                }
            },
            OnContactInfoReceived: function(errorCode, userIds, contacts){
                if(errorCode == uc.ErrorCode.Success && contacts && contacts.length){
                    uc.modules.call.CallManager.OnBasicContactInfoReceived(contacts);
                }
            },
            /** IIMEvent */
            OnP2PCallInvitationReceived: function(channel,userId,fromUserId, confId, confPwd, hostId, msgId, msgSeq, timestamp){
                uc.modules.call.CallManager.OnP2PCallInvitationReceived(channel,userId,fromUserId, confId, confPwd, hostId, msgId, msgSeq, timestamp);
            },
            OnP2PCallInvitationAccepted: function(channel,userId,fromUserId, confId, msgId, msgSeq, timestamp){
                 uc.modules.call.CallManager.OnP2PCallInvitationAccepted(channel,userId,fromUserId, confId, msgId, msgSeq, timestamp);
            },
            OnP2PCallInvitationRejected: function(channel, userId,fromUserId, confId, hostId, reason, msgId, msgSeq, timestamp){
                uc.modules.call.CallManager.OnP2PCallInvitationRejected(channel, userId,fromUserId, confId, hostId, reason, msgId, msgSeq, timestamp);
            },
            OnP2PCallStopped: function(channel, userId, confId, msgId, msgSeq, timestamp){

            },
            OnP2PCallCancelled: function(channel, userId,fromUserId, msgId, msgSeq, timestamp){
                uc.modules.call.CallManager.OnP2PCallCancelled(channel, userId,fromUserId, msgId, msgSeq, timestamp);
            },
            OnP2PCallNoResponse: function(channel, userId, msgId, msgSeq, timestamp){

            },
            OnP2PCallDurationReceived: function(channel, userId,fromUserId, duration, msgId, msgSeq, timestamp){

            },
            /** IGroupEvent */
            OnGroupInfoReceived: function(errorCode, groupId, mountId, group){
                if(errorCode == uc.ErrorCode.Success){
                    uc.modules.call.CallManager.OnGroupInfoReceived(group);
                }
            },
            OnGroupNameUpdatedMessageReceived: function(conversationType, channel, from, to, operatorId, groupId, groupName, namePinYin, msgId, msgSeq, msgTime){
                uc.modules.call.CallManager.OnGroupNameUpdatedMessageReceived(conversationType, channel, from, to, operatorId, groupId, groupName, namePinYin, msgId, msgSeq, msgTime);
            },

            OnGroupAvatarUpdatedMessageReceived: function(conversationType, channel, from, to, operatorId, groupId, groupAvatar, msgId, msgSeq, msgTime){
                //uc.modules.chat.ChatManager.noticeGroupAvatarUpdatedMessageReceived(conversationType, channel, from, to, operatorId, groupId, groupAvatar, msgId, msgSeq, msgTime);
            },
            OnGroupMemberAddedMessageReceived: function(conversationType, channel, from, to, operatorId, groupId, groupName, groupAvatar, members, msgId, msgSeq, msgTime){
                uc.modules.call.CallManager.OnGroupMemberAddedMessageReceived(conversationType, channel, from, to, operatorId, groupId,members);
            },
            OnGroupCallInvitationReceived: function(channel,conversationType, groupId, hostId, confId, confPwd, msgId, msgSeq, timestamp){
                uc.modules.call.CallManager.OnGroupCallInvitationReceived(channel,conversationType, groupId, confId, confPwd, hostId, msgId, msgSeq, timestamp);
            },
            OnGroupCallInvitationAccepted: function(channel,conversationType, groupId, confId,userId, msgId, msgSeq, timestamp){
                uc.modules.call.CallManager.OnGroupCallInvitationAccepted(channel,conversationType, groupId, confId,userId, msgId, msgSeq, timestamp);
            },
            OnGroupCallInvitationRejected: function(channel,conversationType, groupId, confId, reason,userId, msgId, msgSeq, timestamp){
                uc.modules.call.CallManager.OnGroupCallInvitationRejected(channel,conversationType, groupId, confId,userId, msgId, msgSeq, timestamp);
            },
            OnGroupCallStopped: function(conversationType, groupId, confId, msgId, msgSeq, timestamp){

            },
            OnGroupCallDurationReceived: function(channel,conversationType, groupId, duration, msgId, msgSeq, timestamp){
                uc.modules.call.CallManager.OnGroupCallDurationReceived(channel,conversationType, groupId, duration, msgId, msgSeq, timestamp);
            },
            OnGroupMessageSent:function(errorCode, conversationType, groupId, msgId, msgSeq, msgTime){
                uc.modules.call.CallManager.OnMessageSent(errorCode, groupId, msgId, msgSeq, msgTime);
            },
            OnCallQualityChanged: function (meetingId, userId, chatType, quality) {
                uc.modules.call.CallManager.OnCallQualityChanged(meetingId, userId, chatType, quality);
            },
            OnCallSpeakingStatusChanged: function (meetingId, userId, chatType, isSpeaking) {
                uc.modules.call.CallManager.OnCallSpeakingStatusChanged(meetingId, userId, chatType, isSpeaking);
            },
            OnCallAllLeft: function (errorCode, meetingId,callType) {
                uc.modules.call.CallManager.OnCallAllLeft(errorCode, meetingId,callType);
            },
            OnMessageSent: function (errorCode, userId, msgId, msgSeq, timestamp) {
                uc.modules.call.CallManager.OnMessageSent(errorCode, userId, msgId, msgSeq, timestamp);
            },
            OnInviteUsersToCall:function(errorCode,meetingId,chatType,userId){
                uc.modules.call.CallManager.OnInviteUsersToCall(errorCode,meetingId,chatType,userId);
            },
		    /*//废弃
            OnUserCallStatusReceived:function(errorCode, chatId, chatType, callStatus){
                uc.modules.call.CallManager.OnUserCallStatusReceived(errorCode, chatId, chatType, callStatus);
            }, */
		    OnGetUserStatusResponse:function(errorCode,status,copyBack){
			    if(copyBack && parseInt(copyBack) != 0){
				    var copyBackobj = $.parseJSON(copyBack);
				    if(!copyBackobj.userId){
					    return;
				    }
                    var chatId = parseInt(copyBackobj.userId) ? parseInt(copyBackobj.userId) : 0;
				    var chatType = parseInt(copyBackobj.type);
				    uc.modules.call.CallManager.OnUserCallStatusReceived(errorCode, chatId, chatType, status);
			    }
		    },
			OnGetHistoryPhoneNumReceived:function(errorCode,phoneNumbers){
				uc.modules.call.CallManager.OnGetHistoryPhoneNumReceived(errorCode,phoneNumbers);
			},
		    //如果在groupcall中，退出群聊的时候，也要退出groupcall
		    OnGroupExited: function(errorCode, groupId, userId){
			    uc.modules.call.CallManager.OnGroupExitedNeedLeaveGroupCall(errorCode, groupId);
		    },
		    GroupMemberKickedMessageReceived: function(conversationType, channel, from, to, operatorId, groupId, groupName, groupAvatar, members, msgId, msgSeq, msgTime) {
			    if (channel != uc.constants.Message.MessageChannel.UCAS) {
				    return;
			    }
			    if(members){
				    var loginuserId = uc.IClientService.getCurrentUCID().userId;
				    for( var i = 0;i < members.length; i++ ){
					      if( members[i] && members[i].userId == loginuserId ){
						      uc.modules.call.CallManager.OnGroupExitedNeedLeaveGroupCall(0, groupId);
						      break;
					      }
				    }
			    }
		    },
		    OnGroupEnded: function(errorCode, groupId, operatorId){
			    uc.modules.call.CallManager.OnGroupExitedNeedLeaveGroupCall(errorCode, groupId, operatorId);
		    },
		    //音频准备好事件
		    OnAudioServiceReady: function (audioReadyStatus){
			    //audioReadyStatus：1表示准备好
			    uc.modules.call.CallManager.OnAudioServiceReady(audioReadyStatus);
		    },
		    ////音频设备状态变更
		    OnAudioDeviceStatusChanged: function (audioDeviceStatusChanged){
			   //audioDeviceStatusChanged：1 有音频设备 0：无音频设备
			   // console.log("OnAudioDeviceStatusChanged:",audioDeviceStatusChanged);
			   uc.modules.call.CallManager.OnAudioServiceReady(1,audioDeviceStatusChanged);
		    },
		    /**桌面共享**/
		    //桌面共享error通知事件
		    OnSharingError: function(statusCode){
			    uc.modules.call.CallManager.OnSharingError(statusCode);
		    },
		    //桌面共享事件
		    OnDesktopShared: function(conferenceId, userId){
			    uc.modules.call.CallManager.onOpenScreenSharingWindow(conferenceId, userId);
		    },
		    //桌面共享停止事件
		    OnDesktopShareStoped:function(conferenceId, userId){
			    uc.modules.call.CallManager.OnDesktopShareStoped(conferenceId, userId);
		    },
		    //观看桌面共享开始事件
		    OnDesktopViewerShareStarted:function(conferenceId){
			    uc.modules.call.CallManager.OnDesktopViewerShareStarted(conferenceId);
		    },
		    //观看桌面共享结束事件
		    OnDesktopViewerShareStoped:function(conferenceId){
			    uc.modules.call.CallManager.OnDesktopViewerShareStoped(conferenceId);
		    }
	    })
})(uc);