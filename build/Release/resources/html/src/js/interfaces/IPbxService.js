(function(uc){
    uc.interfaces.IPbxService = {
        getMakeCall: function(userName,callNum,callType,value,callBack){},
        getCallRecordLst: function(count,time,callBack){},
        getCallDetailsLst : function(callNum,userID,time,count,callBack){},
        answerCall: function(callID,callType,callBack){},
        rejectCall : function(callID,callBack){},
        hangupCall: function(callID,callBack){},
        muteCall: function(callID,callBack){},
        unMuteCall: function(callID,callBack){},
        sendDTMF: function(callID,num,callBack){},
        getSearchCallRecord: function(keyWord,time,count,callBack){},
        checkPbxLogin: function(callBack){},
        refreshMic: function(callBack){},
        saveCallDetails: function(timestamp, taskid, number, userid, incoming, type, state, druation, callback){}
    }
})(uc);