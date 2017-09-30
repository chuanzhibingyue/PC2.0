/**
 * Created by pan.li on 2015/9/17.
 */
(function (uc) {
    uc.modules.password.ConfirmSetPassword = {
        init: function () {
            this.close = $(".icon-close");
            this.cancel = $(".cancel");
            this.confirm = $(".confirm");

            this.bindEvent();
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
                uc.util.WindowUtil.close();
            })
        }
    };
    jQuery(document).ready(function () {
        uc.init();
        uc.modules.password.ConfirmSetPassword.init();
    })
})(uc);