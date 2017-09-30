(function (uc) {
    var _this=null;
    uc.modules.pbx.PbxCallManager = {
        onMakeCallInfo:{},
        currentCallIngId: 0,
        startCallPbx:function(callInfo){
            var self=this;
            if(!(callInfo.phoneNumber)){
                uc.util.LogUtil.info('PbxCallManager', 'startCallPbx', 'callInfo = '+JSON.stringify(callInfo), null);
                uc.modules.pbx.PbxCallManager.showPbxPhoneTelError();
                return ;
            }
            if(!uc.modules.pbx.PbxManager.DialIsClick){
                this.showCallIngPrompt();
                return;
            }
            if(!this.checkIsCall()){
                return;
            }
            var val = uc.util.StorageUtil.getData('OnPbxLoginResult');
            uc.modules.pbx.PbxManager.restoreDialStatus(false);
            if(val == 0){
                uc.modules.pbx.PbxCallManager.getMakeCall(callInfo);
            }else{
                this.restoreStatus();
                if(val == 5){
                    this.showStartToPbxCheckFailed();
                }else{
                    this.showStartCallFailed();
                }
            }
        },
        getMakeCall:function(callInfo){
            uc.IPbxService.getMakeCall(
                callInfo.userName,
                parseInt(callInfo.userID),
                callInfo.phoneNumber+"",
                callInfo.callType,
                callInfo.Type,
                function(args){
                    var res = args[0];
                    var timeStamp=args[1];
                    var callID=args[2];
                    uc.util.LogUtil.info('PbxCallManager', 'startCallPbx', 'errorCode = ' +res+",callBackData="+JSON.stringify(args), null);
                    if(res == 0 ){
                        uc.modules.pbx.PbxCallManager.currentCallIngId = timeStamp;
                        uc.modules.pbx.PbxCallManager.onMakeCallInfo[timeStamp]={
                            "callID":callID,
                            "callTime":0,
                            "timeStamp":timeStamp,
                            "incoming":uc.constants.Pbx.CallIncomingType.CallOut,
                            "userName":callInfo.userName,
                            "userID":callInfo.userID,
                            "phoneNumber":callInfo.phoneNumber
                        };
                    }
                    else{
                        uc.modules.pbx.PbxCallManager.restoreStatus();
                        uc.modules.pbx.PbxCallManager.showPbxCallFailNotification();
                    }
                });
        },
        OnPbxCallBee:function(userID){
            if(!uc.modules.pbx.PbxManager.DialIsClick){
                this.showCallIngPrompt();
                return;
            }
            uc.IUIService.triggerCrossWindowEvent(uc.constants.UI.CrossWindowEventName.ReOpenCallWindowFormMessage, {chatType: uc.constants.Chat.ChatType.SingleChat, Id: userID});
            uc.IUIService.triggerCrossWindowEvent(uc.constants.UI.CrossWindowEventName.OpenChatWindow, {chatType: uc.constants.Chat.ChatType.SingleChat, Id:userID, isFromCall:true});
        },
        ReOpenPbxChatDetail:function(data){
            uc.IUIEvent.OnPbxCallFromContact(data);
        },
        prepareCallData:function(data){
            uc.modules.pbx.PbxCallManager.startCallPbx({
                "userName":data.userName,
                "userID":data.userID,
                "phoneNumber":data.Tel,
                "callType":0,
                "Type":"ADEXEO2103"});
        },
        restoreStatus:function(){
            uc.modules.pbx.PbxManager.restoreDialStatus(true);
            uc.modules.pbx.PbxManager.restoreCallWindowStatus();
        },
        OnPbxHangUpCallStatus:function(data){
            //防止在通话过程，收到呼入的电话(呼入的电话在挂断之后也会有事件上来，造成状态不同步了)
           if(uc.modules.pbx.PbxCallManager.currentCallIngId == data.time){
               uc.modules.pbx.PbxManager.callWindowIsExist = false;
               uc.modules.pbx.PbxCallManager.currentCallIngId = 0;
               uc.modules.pbx.PbxCallManager.restoreStatus();
           }
        },
        OnMakeCallStatusReceived:function(errorCode,data){
            if(errorCode == 0){
                if($.isEmptyObject(this.onMakeCallInfo[data.timeStamp])){
                    this.onMakeCallInfo[data.timeStamp] = data;
                    this.onMakeCallInfo[data.timeStamp].errorCode = errorCode;
                }else{
                    this.onMakeCallInfo[data.timeStamp].callID = data.callID;
                    this.onMakeCallInfo[data.timeStamp].timeStamp = data.timeStamp;
                    this.onMakeCallInfo[data.timeStamp].micState = data.micState;
                    this.onMakeCallInfo[data.timeStamp].fromNumber = data.toNumber;
                    this.onMakeCallInfo[data.timeStamp].errorCode = errorCode;
                }
                uc.modules.pbx.PbxManager.OnPbxCallStarted(uc.modules.pbx.PbxCallManager.onMakeCallInfo[uc.modules.pbx.PbxCallManager.currentCallIngId]);
                uc.modules.pbx.PbxCallManager.openPbxCallWindow(uc.modules.pbx.PbxCallManager.onMakeCallInfo[uc.modules.pbx.PbxCallManager.currentCallIngId]);
            }else {
                data.number = data.toNumber;
                data.time = data.timeStamp;
                this.onMakeCallInfo[data.timeStamp].errorCode = errorCode;
                 if(data.micState == uc.constants.Pbx.MicStatus.MicStatusNotIn  ||
                    data.micState == uc.constants.Pbx.MicStatus.MicStatusNotOut ||
                    data.micState  == uc.constants.Pbx.MicStatus.MicStatusNotOutOrIn){
                     this.restoreStatus();
                     this.showPbxMicStatusNotification();
                 }else{
                     uc.modules.pbx.PbxManager.OnPbxUNCallStatus(data);
                     this.showPbxCallFailNotification();
                 }
            }
        },
        showCallStatus:function(data){
            if($.isEmptyObject(this.onMakeCallInfo[data.timeStamp])){
                this.onMakeCallInfo[data.timeStamp] = data;
            }else{
                this.onMakeCallInfo[data.timeStamp].callID = data.callID;
                this.onMakeCallInfo[data.timeStamp].callState = data.callState;
                this.onMakeCallInfo[data.timeStamp].resultCode = data.resultCode;
                this.onMakeCallInfo[data.timeStamp].callUUID = data.callUUID;
                this.onMakeCallInfo[data.timeStamp].timeStamp = data.timeStamp;
            }
            this.stopAllSound(data);
//          uc.modules.pbx.PbxEmbeddedCallWindow._showCallStatus(data);
        },
        stopAllSound:function(data){
            switch (data.callState){
                case  uc.constants.Pbx.CallStatus.CALL_STATE_CONFIRMED:
                case  uc.constants.Pbx.CallStatus.CALL_STATE_DISCONNECTED:
                    uc.util.AudioUtil.stopCallAllSound();
                    break;
            }
        },
        OnPbxIncomingNotification:function(data){
            //当前客户端正在呼叫，收到呼入电话需要拒绝
            if(!uc.modules.pbx.PbxManager.DialIsClick){
                //todo 挂断逻辑有问题，需要处理
                uc.modules.pbx.PbxCallManager.rejectCall(data.timeStamp,data.callID);
                return;
            }
            this.onMakeCallInfo[data.timeStamp] = {};
            uc.modules.pbx.PbxCallManager.currentCallIngId = data.timeStamp;
            this.onMakeCallInfo[data.timeStamp].callID = data.callID;
            this.onMakeCallInfo[data.timeStamp].callType = data.callType;
            this.onMakeCallInfo[data.timeStamp].fromNumber = data.fromNumber;
            this.onMakeCallInfo[data.timeStamp].timeStamp = data.timeStamp;
            this.onMakeCallInfo[data.timeStamp].incoming = uc.constants.Pbx.CallIncomingType.CallIn;
            this.onMakeCallInfo[data.timeStamp].muteStatus = false;
            uc.modules.pbx.PbxManager.restoreDialStatus(false);
            uc.modules.pbx.PbxManager.currentCallUserID=0;
            uc.modules.pbx.PbxCallManager.openFloatingCallWindow(this.onMakeCallInfo[data.timeStamp]);
          //  uc.modules.pbx.PbxManager.OnPbxIncomingNotification(this.onMakeCallInfo[data.timeStamp]);
            uc.util.AudioUtil.playCallSound(uc.constants.Call.callMediaType.ringVideo,true);
        },
        getMainCallDataToPbxFloating:function(data){
            var callData=this.onMakeCallInfo[data.callInfo];
            if(callData){
                uc.IUIService.triggerCrossWindowEvent(uc.constants.UI.CrossWindowEventName.PbxMainWindowGetCallDataToFloating,{
                    callInfo:callData
                });
            }
        },
        PbxFloatingWindowSwitchMainWindow:function(info){
            var data=this.onMakeCallInfo[info.timeStamp];
            if(data){
                this.onMakeCallInfo[info.timeStamp].callTime=info.callTime;
                this.onMakeCallInfo[info.timeStamp].muteStatus=info.muteStatus;
                uc.modules.pbx.PbxManager.callWindowIsExist=false;
                if(data.callState ==  uc.constants.Pbx.CallStatus.CALL_STATE_CONFIRMED){
                    data.isCallIng=true;
                    uc.modules.pbx.PbxEmbeddedCallWindow.init(data,data.timeStamp,data.callID);
                    uc.modules.pbx.PbxEmbeddedCallWindow.showCallTime(data,data.callTime);
                }else{
                    data.isCallIng=false;
                    uc.modules.pbx.PbxEmbeddedCallWindow.init(data,data.timeStamp,data.callID);
                }
            }
        },
        exitPbx:function(){
            if(uc.modules.pbx.PbxCallManager.currentCallIngId){
                var data=this.onMakeCallInfo[uc.modules.pbx.PbxCallManager.currentCallIngId];
                uc.util.LogUtil.info('PbxCallManager','exitPbx', 'callID = ' +data.callID+",callInfo="+JSON.stringify(data),null);
                if(data){
                    if(data.incoming == uc.constants.Pbx.CallIncomingType.CallIn){
                        if( data.callState == uc.constants.Pbx.CallStatus.CALL_STATE_CONFIRMED){
                            this.hangupCall(data.timeStamp,data.callID);
                        }else{
                            this.rejectCall(data.timeStamp,data.callID);
                        }
                    }else if (data.incoming == uc.constants.Pbx.CallIncomingType.CallOut){
                            this.hangupCall(data.timeStamp,data.callID);
                    }
                }
            }
        },
        hangupCall:function(timeStamp,callID,callBack){
            uc.IPbxService.hangupCall(callID,function(args){
                uc.util.LogUtil.info('PbxCallManager', 'hangupCall', 'errorCode = ' +args[0]+",callInfo="+JSON.stringify(uc.modules.pbx.PbxCallManager.onMakeCallInfo[timeStamp]), null);
                callBack && callBack();
            });
        },
        rejectCall:function(timeStamp,callID){
            uc.IPbxService.rejectCall(callID,function(args){
                uc.util.LogUtil.info('PbxCallManager', 'rejectCall', 'errorCode = ' +args[0]+",callInfo="+JSON.stringify(uc.modules.pbx.PbxCallManager.onMakeCallInfo[timeStamp]), null);
            });
        },
        muteCall:function(timeStamp,callID){
            uc.IPbxService.muteCall(callID,function(args){
                uc.util.LogUtil.info('PbxCallManager','muteCall', 'errorCode = ' +args[0]+",callInfo="+JSON.stringify(uc.modules.pbx.PbxCallManager.onMakeCallInfo[timeStamp]), null);
                if(args[0] == 0){
                    uc.modules.pbx.PbxCallManager.onMakeCallInfo[timeStamp].muteStatus = true;
                    uc.modules.pbx.PbxEmbeddedCallWindow.setMuteOpenStatus();
                }
            });
        },
        unMuteCall:function(timeStamp,callID){
            uc.IPbxService.unMuteCall(callID,function(args){
                uc.util.LogUtil.info('PbxCallManager','muteCall', 'errorCode = ' +args[0]+",callInfo="+JSON.stringify(uc.modules.pbx.PbxCallManager.onMakeCallInfo[timeStamp]), null);
                if(args[0] == 0){
                    uc.modules.pbx.PbxCallManager.onMakeCallInfo[timeStamp].muteStatus = false;
                    uc.modules.pbx.PbxEmbeddedCallWindow.setMuteCloseStatus();
                }
            });
        },
        sendDTMF:function(timeStamp,callID,num){
            uc.IPbxService.sendDTMF(callID,num,function(args){
                uc.util.LogUtil.info('PbxCallManager','sendDTMF', 'errorCode = ' +args[0]+",callInfo="+JSON.stringify(uc.modules.pbx.PbxCallManager.onMakeCallInfo[timeStamp])+",DMT="+num, null);
            });
        },
        showHangupCallFailed:function(){
            uc.modules.component.PromptMessageBox.open({
                title: uc_resource.WindowTitle.Notification,
                message: uc_resource.Pbx.hangupCallFailed,
                buttonMode: "ok",
                OkText: uc_resource.Common.OK
            }, function () {
                this.close();
            });
        },
        showStartCallFailed: function(){
            uc.modules.component.PromptMessageBox.open({
                title: uc_resource.WindowTitle.Notification,
                message: uc_resource.Pbx.StartInitPbx,
                buttonMode: "ok",
                OkText: uc_resource.Common.OK
            }, function () {
                this.close();
            });
        },
        showStartToPbxCheckFailed: function(){
            uc.modules.component.PromptMessageBox.open({
                title: uc_resource.WindowTitle.Notification,
                message: uc_resource.Pbx.PbxCheckFail,
                buttonMode: "ok",
                OkText: uc_resource.Common.OK
            }, function () {
                this.close();
            });
        },
        checkIsCall:function(){
            if(uc.modules.call.CallManager.inProgressCallInfo.isInProgress){
                this.showCallIngPrompt();
                return false;
            }
            return true;
        },
        showCallIngPrompt:function(){
            uc.modules.component.PromptMessageBox.open({
                title: uc_resource.WindowTitle.Notification,
                message: uc_resource.Call.HasCallNotCall,
                buttonMode: "ok",
                OkText: uc_resource.Common.OK
            }, function () {
                this.close();
            });
            return false;
        },
        showPbxLoginStatus: function(){
            //todo  后续根据状态码显示提示语
            uc.modules.component.PromptMessageBox.open({
                title: uc_resource.WindowTitle.Notification,
                message: uc_resource.Pbx.StartCallFailed,
                buttonMode: "ok",
                OkText: uc_resource.Common.OK
            }, function () {
                this.close();
            });
        },
        showPbxMicStatusNotification:function(){
            uc.modules.component.PromptMessageBox.open({
                title: uc_resource.WindowTitle.Notification,
                message: uc_resource.Pbx.unInstallVedio,
                buttonMode: "both",
                OkText: uc_resource.Common.AgainCheck,
                CancelText: uc_resource.Common.Ignore
            }, function (result) {
                if(result == 0){
                    uc.modules.pbx.PbxCallManager.openDeviceCheck();
                }
                this.close();
            });
        },
        showPbxCallFailNotification: function(){
            uc.modules.component.PromptMessageBox.open({
                title: uc_resource.WindowTitle.Notification,
                message: uc_resource.Pbx.StartCallFailed,
                buttonMode: "ok",
                OkText: uc_resource.Common.OK
            }, function () {
                this.close();
            });
        },
        showPbxPhoneTelError: function(){
            uc.modules.component.PromptMessageBox.open({
                title: uc_resource.WindowTitle.Notification,
                message: uc_resource.Pbx.PhoneTelError,
                buttonMode: "ok",
                OkText: uc_resource.Common.OK
            }, function () {
                this.close();
            });
        },
        openFloatingCallWindow: function (callInfo) {
            uc.util.StorageUtil.setData('floating_pbx_call_window_call_info', callInfo);
            var options = {
                title: uc_resource.Common.AppName,
                top: 20,
                left: screen.width - 360,
                width: 360,
                height: 260,
                refreshIfExist: false,
                centerScreen: false,
                modal: false,
                resizable: false,
                isTop: true,
                hasTaskBarIcon: false,
                behaviorIfSCClose: uc.constants.UI.WindowCloseBehavior.Close,
                enableDrag: false,
                isChild: false,
                caption: uc.constants.UI.DragCaption.FloatingCallWindow
            };
            uc.util.WindowUtil.open('page/floatingPbxCallWindow.html', 'floatingPbxCall', options);
        },
        openDialWindow: function () {
            var options = {
                title: uc_resource.WindowTitle.AppName,
                width: 260,
                height: 390,
                refreshIfExist: false,
                centerScreen: true,
                modal: false,
                resizable: false,
                isTop: true,
                hasTaskBarIcon: false,
                behaviorIfSCClose: uc.constants.UI.WindowCloseBehavior.Close,
                enableDrag: false,
                isChild: false,
                caption: uc.constants.UI.DragCaption.PbxDialWindow
            };
            uc.util.WindowUtil.open('page/pbxDialWindow.html', 'pbxDialWindow', options);
        },
        openPbxCallWindow: function(callInfo) {
            uc.util.StorageUtil.setData('pbx_call_window_call_info', callInfo);
            var options = {
                title: uc_resource.WindowTitle.AppName,
                width: 360,
                height: 390,
                refreshIfExist: false,
                centerScreen: true,
                modal: false,
                resizable: false,
                isTop: true,
                hasTaskBarIcon: false,
                behaviorIfSCClose: uc.constants.UI.WindowCloseBehavior.Close,
                enableDrag: false,
                isChild: false,
                caption: uc.constants.UI.DragCaption.PbxCallWindow
            };
            uc.util.WindowUtil.open('page/pbxCallWindow.html', 'pbxCallWindow', options);
        },
        openDeviceCheck:function(){
            var options = {
                width: 460,
                height: 675,
                refreshIfExist: false,
                centerScreen: true,
                modal: true,
                resizable: false,
                isTop:false,
                caption: uc.constants.UI.DragCaption.CloudSetting
            };
            uc.util.WindowUtil.open('page/AVdetection.html','AVdetectionWindow', options);
        }
    }
})(uc);