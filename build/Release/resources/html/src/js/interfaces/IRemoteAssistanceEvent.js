/**
 * Created by waouyang on 15/12/22.
 */
(function(uc){
    uc.interfaces.IRemoteAssistanceEvent = {
        /**
         * event for remote assistance created
         * @param errorCode
         * @param userId
         * @param meetingId
         * @param attendeePassword
         * @param meetingStartUrl
         */
        OnRemoteAssistanceCreated: function(errorCode, userId, meetingId, attendeePassword, meetingStartUrl){},

        /**
         * event for remote assistance invitation received
         * @param channel
         * @param from
         * @param meetingId
         * @param attendeePassword
         * @param msgId
         * @param msgSeq
         * @param timestamp
         */
        OnRemoteAssistanceInvitationReceived: function(channel, from, meetingId, attendeePassword, msgId, msgSeq, timestamp){},

        /**
         * event for remote assistance invitation accepted
         * @param channel
         * @param from
         * @param meetingId
         * @param msgId
         * @param msgSeq
         * @param timestamp
         */
        OnRemoteAssistanceInvitationAccepted: function(channel, from, meetingId, msgId, msgSeq, timestamp){},

        /**
         * event for remote assistance invitation rejected
         * @param channel
         * @param from
         * @param meetingId
         * @param msgId
         * @param msgSeq
         * @param timestamp
         */
        OnRemoteAssistanceInvitationRejected: function(channel, from, meetingId, msgId, msgSeq, timestamp){},

        /**
         * event for remote assistance ended
         * @param channel
         * @param meetingId
         * @param msgId
         * @param msgSeq
         * @param timestamp
         */
        OnRemoteAssistanceEnded: function(channel, meetingId, msgId, msgSeq, timestamp){},

        /**
         * event for remote assistance invitation sent
         * @param errorCode
         * @param meetingId
         * @param msgId
         * @param msgSeq
         * @param timestamp
         * @constructor
         */
        OnRemoteAssistanceInvitationSent: function(errorCode, meetingId, msgId, msgSeq, timestamp){},

        /**
         * event for accept remote assistance message sent
         * @param errorCode
         * @param meetingId
         * @param msgId
         * @param msgSeq
         * @param timestamp
         * @constructor
         */
        OnRemoteAssistanceAcceptMsgSent: function(errorCode, meetingId, msgId, msgSeq, timestamp){},

        /**
         * event for reject remote assistance message sent
         * @param errorCode
         * @param meetingId
         * @param msgId
         * @param msgSeq
         * @param timestamp
         * @constructor
         */
        OnRemoteAssistanceRejectMsgSent: function(errorCode, meetingId, msgId, msgSeq, timestamp){}
    }
})(uc);