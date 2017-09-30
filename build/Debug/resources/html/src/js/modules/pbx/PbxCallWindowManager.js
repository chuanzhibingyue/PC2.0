(function (uc) {
    var _this=null;
    uc.modules.pbx.PbxCallWindowManager = {
        hangupCall:function(callID,callInfo){
            uc.IPbxService.hangupCall(callID,function(args){
                uc.util.LogUtil.info('PbxCallWindowManager', 'hangupCall', 'errorCode = ' +args[0]+",callInfo = "+JSON.stringify(callInfo), null);
                uc.modules.pbx.PbxCallWindow.close();
            });
        },
        rejectCall:function(callID){
            uc.IPbxService.rejectCall(callID,function(args){
                uc.util.LogUtil.info('PbxCallWindowManager', 'rejectCall', 'errorCode = ' +args[0], null);
            });
        },
        muteCall:function(callID,callBack){
            uc.IPbxService.muteCall(callID,function(args){
                uc.util.LogUtil.info('PbxCallWindowManager','muteCall', 'errorCode = ' +args[0], null);
                if(args[0] == 0){
                    callBack && callBack();
                }
            });
        },
        unMuteCall:function(callID,callBack){
            uc.IPbxService.unMuteCall(callID,function(args){
                uc.util.LogUtil.info('PbxCallWindowManager','unMuteCall', 'errorCode = ' +args[0], null);
                if(args[0] == 0){
                    callBack && callBack();
                }
            });
        },
        sendDTMF:function(callID,num){
            uc.IPbxService.sendDTMF(callID,num,function(args){
                uc.util.LogUtil.info('PbxCallWindowManager','sendDTMF', 'errorCode = ' +args[0]+",DMT="+num, null);
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
                buttonMode: "ok",
                OkText: uc_resource.Common.OK
            }, function () {
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
        }

    }
})(uc);