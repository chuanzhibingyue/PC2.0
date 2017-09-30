/**
 * Created by pan.li on 2015/9/15.
 */
(function(uc){
    var _this;
    uc.modules.password.ResetPassword = {

        init: function () {
            _this = this;
            this.bindEvent();
            this.initUI();
        },
        initUI: function () {
            //uc.modules.component.TitleBarButton.init();
            uc.IUIService.triggerCrossWindowEvent("Setting:ResetPassword", {status:-1});
        },
        bindEvent: function () {
            //ESC退出
            document.onkeydown = function (e) {
                if (e.keyCode == 27) {
                    uc.IUIService.triggerCrossWindowEvent("Setting:ResetPassword", {status:0});
                    uc.util.WindowUtil.close();
                }
                var capsLock = uc.util.SystemUtil.isCapsLockPressed();
                if(capsLock && e.which != 20){  //当前是大写并且按下的键不是大小写切换
                    if ($("#new-password").is(":focus")) {
                        $(".newPwd-warning").show();
                        $(".newPwd-warning").html(uc_resource.ResetPassword.CapsLock);
                    }else if($("#new-password-again").is(":focus")) {
                        $(".againPwd-warning").show();
                        $(".againPwd-warning").html(uc_resource.ResetPassword.CapsLock);
                    }
                }else{
                    $(".newPwd-warning").hide();
                    $(".againPwd-warning").hide();

                }
            };

            this.finish = $(".finished");
            this.oldPassWord = $("#old-password");
            this.newPassword = $("#new-password");
            this.newPasswordAgain = $("#new-password-again");
            this.cancel = $(".cancel");
            this.close = $(".tool-icon-container");
            this.verifyAccount = $(".verifyAccount");

            this.warning = $(".error-warning");


            this.oldPassWord.focus(function(){
                $(".error-warning").text("");
            });
            this.newPassword.focus(function(){
                $(".error-warning").text("");
                if(uc.util.SystemUtil.isCapsLockPressed()){
                    $(".newPwd-warning").show();
                    $('.error-warning').hide();
                    $(".againPwd-warning").hide();
                    $(".newPwd-warning").html(uc_resource.ResetPassword.CapsLock);
                }else{
                    $(".newPwd-warning").hide();
                }
            });
            this.newPasswordAgain.focus(function(){
                $(".error-warning").text("");
                if(uc.util.SystemUtil.isCapsLockPressed()){
                    $(".againPwd-warning").show();
                    $('.error-warning').hide();
                    $(".newPwd-warning").hide();
                    $(".againPwd-warning").html(uc_resource.ResetPassword.CapsLock);
                }else{
                    $(".againPwd-warning").hide();
                }
            });

            this.verifyAccount.focus(function () {
                $(".finished").css("background-color", "#3b4f61");
            });

            this.close.on("click", function() {
                uc.IUIService.triggerCrossWindowEvent("Setting:ResetPassword", {status:0});
                uc.util.WindowUtil.close();
            });
            this.cancel.on("click",function(){
                uc.IUIService.triggerCrossWindowEvent("Setting:ResetPassword", {status:0});
                uc.util.WindowUtil.close();
            });
            this.finish.on("click", function(){
                var oldPassword = _this.oldPassWord.val();
                var newPassword = _this.newPassword.val();
                var newPasswordAgain = _this.newPasswordAgain.val();
                var Password_length = newPassword.length;
                var PasswordAgain_length = newPasswordAgain.length;

                if(oldPassword =="" || newPassword == "" || newPasswordAgain ==""){
                    $(".error-warning").text(uc_resource.ResetPassword.NotEmpty);
                    uc.IUIService.triggerCrossWindowEvent("Setting:ResetPassword", {status:0});
                }else if($.trim(newPassword)==""||$.trim(newPasswordAgain)==""){
                    $(".error-warning").text(uc_resource.ResetPassword.PasswordTip);
                    uc.IUIService.triggerCrossWindowEvent("Setting:ResetPassword", {status:0});
                }else if((Password_length != PasswordAgain_length)||(newPassword != newPasswordAgain)){
                    uc.util.LogUtil.info("ResetPassword","Finished","newPassword_length:"+Password_length+",PasswordAgain_length:"+PasswordAgain_length);
                    $(".error-warning").text(uc_resource.ResetPassword.NotMatch);
                    uc.IUIService.triggerCrossWindowEvent("Setting:ResetPassword", {status:0});
                }else{
                    uc.IUIService.triggerCrossWindowEvent(uc.constants.UI.CrossWindowEventName.ModifyingPassword, {});
                    uc.util.LogUtil.info("ResetPassword","modifyPassword","SDK modifyPassword",null);
                    uc.IClientService.modifyPassword(oldPassword, newPassword);
                    uc.IUIService.triggerCrossWindowEvent("Setting:ResetPassword", {status:1, password:newPassword});
                }
            })
        },

        OnPasswordModified: function(errorCode){
            if(errorCode == 0){
                //uc.IClientService.logoutUCAS();
            }else if(errorCode == 10109){
                $(".error-warning").text(uc_resource.ResetPassword.SetFailed);
            }else if(errorCode == 10110){
                $(".error-warning").text(uc_resource.ResetPassword.WrongOldPassword);
            }else if(errorCode == 10130){
                uc.IClientService.getPasswordRule(function(args) {
                    uc.util.LogUtil.info("ResetPassword", "OnPasswordModified", "data", {
                        errCode: args[0],
                        daysRange: args[1],
                        memory: args[2],
                        complex: args[3],
                        allowChangePassword: args[4]
                    });
                    switch (args[2]) {
                        case 1:
                            $(".error-warning").text(uc_resource.ResetPassword.NotThreeTimes)
                            break;
                        case 2:
                            $(".error-warning").text(uc_resource.ResetPassword.NotFiveTimes)
                            break;
                        case 3:
                            $(".error-warning").text(uc_resource.ResetPassword.NotTenTimes)
                            break;
                        default :
                            $(".error-warning").text(uc_resource.ResetPassword.NoRepeat)
                            break;
                    }
                });


            }else if(errorCode == 10113){
                $(".error-warning").text(uc_resource.ResetPassword.NotNull);
            }else if(errorCode == 10146){
                $(".error-warning").text(uc_resource.ResetPassword.PasswordLength);
            }else if(errorCode == 10148){
                $(".error-warning").text(uc_resource.ResetPassword.PasswordComplexity);
            }else if(errorCode == 10149){
                $(".error-warning").text(uc_resource.ResetPassword.PasswordRule);
            }else{
                $(".error-warning").text(uc_resource.ResetPassword.SetFailed);
            }
        }

    };


    jQuery(document).ready(function () {
        uc.init();
        uc.modules.password.ResetPassword.init();
    })
})(uc);