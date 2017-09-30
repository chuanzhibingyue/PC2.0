/**
 * Created by waouyang on 15/12/7.
 */
(function(uc){
    uc.interfaces.ICallEvent = {
        /**
         * event for call created
         * @param errorCode     error code for the call create result
         * @param meetingId     meeting id created for the call
         * @param chatType
         * @param chatId        chat id for the call, user id for P2P call, group id for group call
         */
        OnCallCreated: function(errorCode, meetingId, chatId, chatType){},
        /**
         * event for self join call
         * @param errorCode     error code for the join call result
         * @param meetingId     meeting id of the call
         * @param chatType
         * @param chatId        chat id for the call, user id for P2P call, group id for group call
         */
        OnSelfJoinedCall: function(errorCode, meetingId, chatId, chatType){},
        /**
         * event for self left call
         * @param errorCode     error code for left call
         * @param meetingId     meeting id of the call
         * @param chatId
         * @param chatType
         */
        OnSelfLeftCall: function(errorCode, meetingId, chatId, chatType){},

        /**
         * event for user joined call
         * @param meetingId     meeting id of the call
         * @param userId        user id in UC
         * @param isMute        user's audio type
         * @param audioType     audio type
         * @param phoneNumber   phone number
         */
        OnUserJoinedCall: function(meetingId, userId, isMute, audioType, phoneNumber){},

        /**
         * event for user left call
         * @param meetingId     meeting id of the call
         * @param userId        user id who left call
         */
        OnUserLeftCall: function(meetingId, userId){},

        /**
         * event for user call property changed
         * @param meetingId     meeting id of the call
         * @param userId        user id who changed call property
         * @param isMute        mute status
         * @param audioType     audio type
         * @param phoneNumber   phone number
         * @param callPropertyChangedType   call property changed type 1: audio type changed, 1: mute status changes
         *                                  3: both audio type and mute status changed
         *                                  see uc.constants.Call.CallPropertyChangedType
         */
        OnUserCallPropertyChanged: function(meetingId, userId, isMute, audioType, phoneNumber, callPropertyChangedType){},

        /**
         * event fall call phone failed
         * @param errorCode
         * @param meetingId
         * @param phoneNumber
         */
        OnCallPhoneFailed: function(errorCode, meetingId, phoneNumber){},
        /**
         * event for call disconnected
         * @param meetingId     meeting id of the call
         */
        OnCallDisconnected: function(errorCode,meetingId){},
        /**
         * event for call reconnected
         * @param meetingId     meeting id of the call
         */
        OnCallReconnected: function(errorCode,meetingId){},
        /**
         * event for call quality changed
         * @param meetingId
         * @param userId
         * @param chatType
         * @param quality
         * @constructor
         */
        OnCallQualityChanged:function(meetingId,userId,chatType,quality){},
        /**
         *
         * @param meetingId
         * @param userId
         * @param chatType
         * @param isSpeaking
         * @constructor
         */
        OnCallSpeakingStatusChanged:function(meetingId,userId,chatType,isSpeaking){},
        /**
         * event for all user left
         * @param errorCode     status code: 0: success, 1: self leave, 2: host end meeting etc
         * @param meetingId
         * @constructor
         */
        OnCallAllLeft:function(errorCode,meetingId,callType){},

        OnUserCallStatusReceived:function(errorCode, chatId, chatType, callStatus){},
        OnInviteUsersToCall:function(errorCode,meetingId,chatType,userId){},
		OnGetHistoryPhoneNumReceived:function(errorCode,phoneNumbers){},
        OnGetSpeakerList:function(errorCode,speakers){},
        OnGetMicrophoneList:function(errorCode,microphone){},
        OnMikeDiagnosisStatus:function(mikeName,mikeLevel){},
        OnSpeakerDiagnosisStatus:function(speakerName,speakerLevel){},
        OnSpeakerDiagnosisStop:function(speakerName){},
	    OnAudioServiceReady:function(audioReadyStatus){},
	    OnAudioDeviceStatusChanged:function(audioDeviceStatusChanged){}
    }
})(uc);