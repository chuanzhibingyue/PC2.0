/**
 * Created by waouyang on 15/12/22.
 */
(function(uc){
    uc.interfaces.IRemoteAssistanceService = {
        /**
         * create a remote assistance
         * @param userId    userId
         * @param callback  callback function when create the remote assistance, format:
         *                  function(args){ var errorCode = args[0] }
         */
        createRemoteAssistance: function(userId, callback){},

        /**
         * send remote assistance invitation
         * @param jid
         * @param meetingId
         * @param conferencePassword
         * @param callback
         */
        sendRemoteAssistanceInvitation: function(jid, meetingId, conferencePassword, callback){},

        /**
         * accept remote assistance invitation
         * @param jid
         * @param meetingId
         * @param callback
         */
        acceptRemoteAssistanceInvitation: function(jid, meetingId, callback){},

        /**
         * reject remote assistance invitation
         * @param jid
         * @param meetingId
         * @param callback
         */
        rejectRemoteAssistanceInvitation: function(jid, meetingId, callback){}
    }
})(uc);