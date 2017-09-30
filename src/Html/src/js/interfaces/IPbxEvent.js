(function(uc){
    uc.interfaces.IPbxEvent = {
        OnPbxLogin:function(errorCode){},
        OnMakeCallStatus:function(errorCode,data){},
        OnMakeCallStatusReceived:function(errorCode,data){},
        OnPbxListReceived:function(errorCode,data){},
        OnPbxHangUpCallStatus:function(errorCode,data){},
        OnPbxCallListDetail:function(errorCode,data){},
        OnPbxIncomingNotification:function(errorCode,data){},
        OnPbxGetSearchCall:function(errorCode,data){}
    }
})(uc);