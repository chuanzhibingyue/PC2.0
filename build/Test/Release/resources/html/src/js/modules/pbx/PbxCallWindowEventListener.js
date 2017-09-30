(function(uc){
    uc.modules.pbx.PbxCallWindowEventListener = function(){
    };
    jQuery.extend(uc.modules.pbx.PbxCallWindowEventListener.prototype,
        uc.interfaces.IUIEvent,
        uc.interfaces.IContactEvent,
        uc.interfaces.IPbxEvent,
        uc.interfaces.IIMEvent, {
            OnUserAvatarDownloaded: function(userId, serverAvatarUrl, localAvatarUrl){
                uc.modules.pbx.PbxManager.OnUserAvatarDownloaded(userId, localAvatarUrl);
            },
            OnPbxHangUpCallStatus:function(errorCode,data){
                uc.modules.pbx.PbxCallWindow.hangUpCallStatus(data);
            },
            OnMakeCallStatus:function(errorCode,data){
                if(errorCode==0){
                    if(data.callState == 0){
                        return;
                    }
                    uc.modules.pbx.PbxCallWindow.showCallStatus(data);
                }
            },
            OnContactInfoReceived: function (errorCode, userIds, contacts) {
           //   uc.modules.pbx.PbxManager.OnPbxListUpdateCallList(errorCode, userIds, contacts);
            }
    })
})(uc);