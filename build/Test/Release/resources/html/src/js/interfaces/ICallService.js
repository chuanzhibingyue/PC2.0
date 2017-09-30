/**
 * Created by waouyang on 15/12/7.
 */
(function(uc){
    uc.interfaces.ICallService = {
        /**
         * create a call from id
         * @param chatId        user id for P2P call, group id for group call
         * @param callType      type of the call, P2P: 0, Group: 1, see uc.constants.Call.CallType
         * @param callback      callback function when create the call, format:
         *                      function(args){ var errorCode = args[0] }
         */
        createCallById: function(chatId, callType, callback){},

        /**
         * create a group call by user list
         * @param groupName     group name for the group
         * @param userList      user list in the group, userId + user name pinyin, format:
         *                      [{userId: 123, userName: 'pinyin'}, {userId: 456, userName: 'pinyin'}]
         * @param callback      callback function when create the call, format:
         *                      function(args){ var errorCode = args[0] }
         */
        createCallByUserList: function(groupName, userList, callback){},

        /**
         * join a call
         * @param chatType      type of the call, same as chat type
         * @param meetingId     meeting id to join
         * @param chatId        group id for group call, 0 for P2P call
         * @param callback      callback function when join a call, format:
         *                      function(args){ var errorCode = args[0] }
         */
        joinCall: function(chatType, meetingId, chatId, callback){},

        /**
         * leave a call
         * @param meetingId     meeting id to leave
         * @param callback      callback function when leave a call, format:
         *                      function(args){ var errorCode = args[0] }
         */
        leaveCall: function(meetingId, callback){},

        /**
         * switch VOIP to phone
         * @param phoneNumber   phone number
         * @param callback      callback function when switch VOIP to phone, format:
         *                      function(args){ var errorCode = args[0] }
         */
        switchToPhone: function(phoneNumber, callback){},
        /**
         * switch phone to VOIP
         * @param callback      callback function when switch VOIP to phone, format:
         *                      function(args){ var errorCode = args[0] }
         */
        switchToVOIP: function(callback){},

        /**
         * mute call
         */
        muteSelf: function(){},

        /**
         * un-mute call
         */
        unMuteSelf: function(){},
        /**
         * get user's call status
         * @param userList
         * @param id            userId for single chat, groupId for group chat
         * @param type          chatType
         * @param callback
         */
        getUserCallStatus:function(userList, id, type, callback){},
        inviteUsersToSingleCall:function(chatType,groupName,userList,meetingId,callback){},
        inviteUsersToGroupCall:function(chatType,userList,groupId,meetingId,callback){},
		getHistoryPhoneNum:function(callback){},
        getSpeakerList:function(callback){},
        getMicrophoneList:function(callback){},
        startSpeakerDiagnosis:function(Audio,callback){},
        startMikeDiagnosis:function(Audio,callback){},
        stopSpeakerDiagnosis:function(callback){},
        stopMikeDiagnosis:function(callback){},
        reconnectConference:function(){},
        hangupPhone:function(callback){},
		getSpeakerNum:function(){},
		getMicrophoneNum:function(){},
		getLastAudioInvite:function(userid,callback){},
		getCameraList:function(camera, hwnd ,callback){},
		cameraDiagnosis:function(callback){},
		stopCameraDiagnosis:function(callback){}
    }
})(uc);