(function(uc){
    uc.modules.pbx.FloatingPbxCallWindowEventListener = function(){

    };
    jQuery.extend(uc.modules.pbx.FloatingPbxCallWindowEventListener.prototype,
        uc.interfaces.IPbxEvent,
        uc.interfaces.IContactEvent,
        uc.interfaces.IIMEvent,{
            OnMakeCallStatus:function(errorCode,data){
                uc.util.LogUtil.info('FloatingPbxCallWindowEventListener', 'OnMakeCallStatus', 'errorCode = '+errorCode+",data="+JSON.stringify(data), null);
                if(errorCode==0){
                    if(data.callState==0)return;
                    uc.modules.pbx.FloatingPbxCallWindow.showCallStatus(data);
                }
            },
            OnPbxHangUpCallStatus:function(errorCode,data){
                uc.util.LogUtil.info('FloatingPbxCallWindowEventListener', 'OnPbxHangUpCallStatus', 'errorCode = ' + errorCode+",data="+JSON.stringify(data), null);
                uc.modules.pbx.FloatingPbxCallWindowToolBar.incomingCallClose(data);
            },
            OnMakeCallStatusReceived: function(errorCode,data){
                if(errorCode == 0){
                    uc.modules.pbx.FloatingPbxCallWindowToolBar._setHangupStatus();
                }else {
                    uc.modules.pbx.FloatingPbxCallWindowToolBar.incomingCallClose(data);
                }
            }

    })
})(uc);