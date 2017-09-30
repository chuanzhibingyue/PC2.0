(function (uc) {
    var _this = null;
	var isEnd;
    uc.modules.pbx.FloatingPbxCallWindowManager = {
        rejectCallMsgId: 0,
        init: function () {
            _this = this;
            _this = this;
            var callInfo = uc.util.StorageUtil.getData('floating_pbx_call_window_call_info');
            isEnd = uc.util.StorageUtil.getData('floating_pbx_call_window_close');
            if(isEnd != null){
                console.log("uc.constants.UI.CrossWindowEventName.CloseFloatingCallWindow,localStorage",isEnd);
                uc.util.StorageUtil.removeData('floating_pbx_call_window_close');
                if(isEnd){
                    uc.util.WindowUtil.close();
                }
                isEnd = null;
                return;
            }
            uc.modules.pbx.FloatingPbxCallWindow.init(callInfo);
        }
    }
})(uc);