/**
 * Created by waouyang on 15/12/10.
 */
(function(uc){
    uc.modules.call.FloatingCallWindowEventListener = function(){

    };
    jQuery.extend(uc.modules.call.FloatingCallWindowEventListener.prototype,
        uc.interfaces.ICallEvent,
        uc.interfaces.IContactEvent,
        uc.interfaces.IIMEvent,
        uc.interfaces.IGroupEvent,
	    uc.interfaces.ISharingEvent,{
            /** ICallEvent */
            OnCallDisconnected: function(meetingId){

            },
            OnCallReconnected: function(meetingId){

            },
            /** IContactEvent */
            OnBasicContactInfoReceived: function(errorCode, userIds, contacts){
                if(errorCode == uc.ErrorCode.Success && contacts && contacts.length){
                    uc.modules.call.FloatingCallWindowManager.OnBasicContactInfoReceived(contacts);
                }
            },
            OnContactInfoReceived: function(errorCode, userIds, contacts){
                if(errorCode == uc.ErrorCode.Success && contacts && contacts.length){
                    uc.modules.call.FloatingCallWindowManager.OnBasicContactInfoReceived(contacts);
                }
            },
            /** IGroupEvent */
            OnGroupInfoReceived: function(errorCode, groupId, mountId, group){
                if(errorCode == uc.ErrorCode.Success){
                    uc.modules.call.FloatingCallWindowManager.OnGroupInfoReceived(group);
                }
            },
            OnGroupNameUpdatedMessageReceived: function(conversationType, channel, from, to, operatorId, groupId, groupName, namePinYin, msgId, msgSeq, msgTime){
                uc.modules.call.FloatingCallWindowManager.OnGroupNameUpdatedMessageReceived(conversationType, channel, from, to, operatorId, groupId, groupName, namePinYin, msgId, msgSeq, msgTime);
            },
            OnGroupCallInvitationReceived: function(conversationType, groupId, hostId, confId, confPwd, msgId, msgSeq, timestamp){

            },
            OnGroupCallInvitationAccepted: function(conversationType, groupId, confId, msgId, msgSeq, timestamp){

            },
            OnGroupCallInvitationRejected: function(conversationType, groupId, confId, reason, msgId, msgSeq, timestamp){

            },
            OnGroupCallStopped: function(conversationType, groupId, confId, msgId, msgSeq, timestamp){

            },
            OnGroupCallDurationReceived: function(conversationType, groupId, duration, msgId, msgSeq, timestamp){

            },
            OnUserCallPropertyChanged: function(meetingId, userId, isMute, audioType, phoneNumber, callPropertyChangedType){
               // uc.modules.call.FloatingCallWindowManager.OnUserCallPropertyChanged(meetingId, userId, isMute, audioType, phoneNumber, callPropertyChangedType);
            },
            OnCallSpeakingStatusChanged:function(meetingId,userId,chatType,isSpeaking){
                uc.modules.call.FloatingCallWindowManager.OnCallSpeakingStatusChanged(meetingId,userId,chatType,isSpeaking);
            },
		    /**桌面共享**/
		    //桌面共享事件
		    OnDesktopShared: function(conferenceId, userId){
			    uc.modules.call.FloatingCallWindowManager.onOpenScreenSharingWindow(conferenceId, userId);
		    },
		    //桌面共享停止事件
		    OnDesktopShareStoped:function(conferenceId, userId){
			    uc.modules.call.FloatingCallWindowManager.OnDesktopShareStoped(conferenceId, userId);
		    },
		    //观看桌面共享开始事件
		    OnDesktopViewerShareStarted:function(conferenceId){
			    uc.modules.call.FloatingCallWindowManager.OnDesktopViewerShareStarted(conferenceId);
		    },
		    //观看桌面共享结束事件
		    OnDesktopViewerShareStoped:function(conferenceId){
			    uc.modules.call.FloatingCallWindowManager.OnDesktopViewerShareStoped(conferenceId);
		    }
    })
})(uc);