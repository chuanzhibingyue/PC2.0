/**
 * Created by waouyang on 15/9/19.
 */
(function(uc){
    uc.modules.password.PasswordModified = {
        init: function () {
            this.cancel = $(".cancel");
            this.confirm = $(".confirm");

            this.initUI();
            this.bindEvent();
        },
        initUI: function(){

        },
        bindEvent: function(){
            document.onkeydown = function (e) {
                if (e.keyCode == 27) {
                    uc.util.WindowUtil.close();
                }
            };
            this.cancel.on("click",function() {
                uc.util.WindowUtil.close();
            });
            this.confirm.on("click",function() {
                uc.util.EventUtil.triggerCrossWindowEvent(uc.constants.UI.CrossWindowEventName.OnRestartClient, {});
            })
        }
    };
    jQuery(document).ready(function(){
        uc.modules.password.PasswordModified.init();
    });
})(uc);