(function(uc){
    var beeCallStatus = null;
    uc.modules.pbx.PbxEventListener = function(){
    };
    jQuery.extend(uc.modules.pbx.PbxEventListener.prototype,
        uc.interfaces.IUIEvent,
        uc.interfaces.IContactEvent,
        uc.interfaces.ISearchEvent,
        uc.interfaces.IPbxEvent,
        uc.interfaces.IIMEvent, {
            OnPbxLogin:function(errorCode){
                uc.util.LogUtil.info('Pbx', 'OnPbxLogin', 'errorCode = ' + errorCode, null);
                uc.util.StorageUtil.setData('OnPbxLoginResult',errorCode);
            },
            OnUserAvatarDownloaded: function(userId, serverAvatarUrl, localAvatarUrl){
                uc.modules.pbx.PbxManager.OnUserAvatarDownloaded(userId, localAvatarUrl);
            },
            OnPbxHangUpCallStatus:function(errorCode,data){
                uc.util.LogUtil.info('Pbx', 'OnPbxHangUpCallStatus', 'errorCode = ' + errorCode+",data="+JSON.stringify(data), null);
                uc.modules.pbx.PbxManager.OnPbxHangUpCallStatus(errorCode,data);
            },
            OnOpenPbxCallChat:function(data){
                uc.modules.pbx.PbxManager.OnOpenPbxCallChat(data);
            },
            OnWindowResized: function(){
                uc.modules.pbx.PbxManager.OnWindowResized();
            },
            OnMinimizeWindow:function(){

            },
            OnPbxCallBee:function(userID){
                uc.modules.pbx.PbxCallManager.OnPbxCallBee(userID);
            },
            OnSaveBeeCallDetailsToPbx:function(beeData){
                uc.modules.pbx.PbxManager.SaveBeeCallDetailsToPbx(beeData);
            },
            OnSaveBeeCallStatusToPbx:function(data){
                if(data.userID){
                    data.incoming = beeCallStatus;
                    uc.modules.pbx.PbxManager.SaveBeeCallDetailsToPbx(data);
                    beeCallStatus = null;
                }else{
                    beeCallStatus = data.incoming;
                }
            },
            OnPbxCallFromContact:function(data){
                uc.IUIService.changeModule(uc.constants.UI.ModuleId.Pbx);
                uc.modules.pbx.PbxManager.prepareCallData(data);
            },
            OnPbxIncomingNotification:function(errorCode,data){
                uc.util.LogUtil.info('Pbx', 'OnPbxIncomingNotification', 'errorCode = '+errorCode+",data="+JSON.stringify(data), null);
                if(errorCode == 0){
                   uc.modules.pbx.PbxCallManager.OnPbxIncomingNotification(data);
                }
            },
            OnPbxCallListDetail:function(errorCode,data){
                uc.modules.pbx.PbxManager.OnPbxCallListDetail(errorCode,data);
            },
            OnModuleChanged: function(moduleId){
                uc.modules.pbx.PbxManager.OnModuleChanged(moduleId);
            },
            OnPbxListReceived: function(errorCode,data){
                uc.modules.pbx.PbxManager.OnPbxListReceived(errorCode,data);
            },
            OnMakeCallStatusReceived:function(errorCode,data){
                uc.modules.pbx.PbxCallManager.OnMakeCallStatusReceived(errorCode,data);
            },
            OnMakeCallStatus:function(errorCode,data){
                uc.util.LogUtil.info('Pbx', 'OnMakeCallStatus', 'errorCode = '+errorCode+",data="+JSON.stringify(data), null);
                if(errorCode==0){
                    if(data.callState == 0){
                        return;
                    }
                    uc.modules.pbx.PbxCallManager.showCallStatus(data);
                }
            },
            OnPbxGetSearchCall:function(errorCode,data){
                uc.modules.component.PbxSearch.OnPbxGetSearchCall(errorCode,data)
            },
            OnContactInfoReceived: function (errorCode, userIds, contacts) {
                uc.modules.pbx.PbxManager.OnPbxListUpdateCallList(errorCode, userIds, contacts);
            },
            OnServerSearchResultReceived: function(errorCode, searchId, contacts, projects, groupChats, cloudGroups, meetings, messages){
                uc.modules.pbx.PbxManager.OnServerSearchResultReceived(errorCode,searchId, contacts, projects, groupChats, cloudGroups, meetings, messages)
            }
    })
})(uc);