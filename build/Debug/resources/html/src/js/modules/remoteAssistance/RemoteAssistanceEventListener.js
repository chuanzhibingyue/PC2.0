/**
 * Created by waouyang on 15/12/22.
 */
(function(uc){
    uc.modules.remoteAssistance.RemoteAssistanceEventListener = function(){

    };
    jQuery.extend(uc.modules.remoteAssistance.RemoteAssistanceEventListener.prototype,
        uc.interfaces.IMeetingEvent,
        uc.interfaces.IContactEvent,
        uc.interfaces.IRemoteAssistanceEvent, {
            /** Meeting Event **/
            OnMeetingStartUrlByConfIdReceived: function(/*int*/errorCode,startUrl,confId){
                uc.modules.remoteAssistance.RemoteAssistanceManager.OnMeetingStartUrlByConfIdReceived(errorCode,startUrl,confId);
            },
            /** Remote Assistance Event **/
            OnRemoteAssistanceCreated: function(errorCode, userId, meetingId, attendeePassword, meetingStartUrl){
                uc.modules.remoteAssistance.RemoteAssistanceManager.OnRemoteAssistanceCreated(errorCode, userId, meetingId, attendeePassword, meetingStartUrl);
            },

            OnRemoteAssistanceInvitationReceived: function(channel, from, meetingId, attendeePassword, msgId, msgSeq, timestamp){
                uc.modules.remoteAssistance.RemoteAssistanceManager.OnRemoteAssistanceInvitationReceived(from, meetingId, attendeePassword);
            },

            OnRemoteAssistanceInvitationAccepted: function(channel, from, meetingId, msgId, msgSeq, timestamp){
                uc.modules.remoteAssistance.RemoteAssistanceManager.OnRemoteAssistanceInvitationAccepted(from, meetingId);
            },

            OnRemoteAssistanceInvitationRejected: function(channel, from, meetingId, msgId, msgSeq, timestamp){
                uc.modules.remoteAssistance.RemoteAssistanceManager.OnRemoteAssistanceInvitationRejected(from, meetingId);
            },

            OnRemoteAssistanceEnded: function(channel, meetingId, msgId, msgSeq, timestamp){
                uc.modules.remoteAssistance.RemoteAssistanceManager.OnRemoteAssistanceEnded(meetingId);
            },
            OnRemoteAssistanceInvitationSent: function(errorCode, meetingId, msgId, msgSeq, timestamp){
                uc.modules.remoteAssistance.RemoteAssistanceManager.OnRemoteAssistanceInvitationSent(errorCode, meetingId);
            },
            OnRemoteAssistanceAcceptMsgSent: function(errorCode, meetingId, msgId, msgSeq, timestamp){
                uc.modules.remoteAssistance.RemoteAssistanceManager.OnRemoteAssistanceAcceptMsgSent(errorCode, meetingId);
            },
            OnRemoteAssistanceRejectMsgSent: function(errorCode, meetingId, msgId, msgSeq, timestamp){
                uc.modules.remoteAssistance.RemoteAssistanceManager.OnRemoteAssistanceRejectMsgSent(errorCode, meetingId);
            }
    });
})(uc);