/**
 * Created by pan.li on 2015/8/26.
 */

(function (uc) {
    var _this, maxtime, resend = 0,timer;
    var time = Date.parse(new Date());
    var timestamp = time.toString();
    var verifyType = 0;
    uc.modules.password.ForgetPassword = {
        init: function () {
            _this = this;
            this.bindEvent();
            this.initUI();
        },
        initUI: function () {
            uc.modules.component.TitleBarButton.init();
            _this.getCaptcha();
        },
        bindEvent: function () {
            this.description = $(".description");
            this.messageSend = $(".message-send");
            this.warningArea = $(".warning-area");
            this.generalDescription = $(".general-description");
            this.againDescription = $(".again-description");

            this.findByPhone = $(".findByPhone");
            this.findByEmail = $(".findByEmail");

            this.verifyAccount = $('.input.verifyAccount');
            this.verifyCode = $('.input.verifyCode');
            this.messageCode = $(".input.messageVerifyCode");
            this.newPassword = $(".newPassword");
            this.newPasswordAgain = $(".newPasswordAgain");
            this.corpAccount = $('.input.corpAccount');
            this.personalAccount = $('.input.personalAccount');


            this.cancelBtn = $(".button-area  .cancel-btn");
            this.refresh = $(".refreshBtn");
            this.sendVerifyMessage = $(".button-area  .send-message-btn");
            this.resendBtn = $(".button-area .resend");
            this.commitMessageBtn = $(".button-area .commit-message-code");
            this.commitPwdBtn = $(".button-area .commit-password-btn");

            document.onkeydown = function (e) {
                var capsLock = uc.util.SystemUtil.isCapsLockPressed();
                if(capsLock && e.which != 20) {  //当前是大写并且按下的键不是大小写切换
                    if ($(".set-password.input.newPassword").is(":focus")) {
                        $(".password-description").show();
                        $(".password-description").html(uc_resource.ResetPassword.CapsLock);
                    } else if ($(".set-password.input.newPasswordAgain").is(":focus")) {
                        $(".again-description").show();
                        $(".again-description").html(uc_resource.ResetPassword.CapsLock);
                    }
                }
                else{
                    $(".password-description").html("");
                    $(".again-description").html("");

                }
            };

            var accountType = this.getParam();
            if(Number(accountType) == 2){
                $('.corporate-account-area').removeClass("hide");
                $('.common-account-area').addClass("hide");
                $('.verify-code-area').addClass("corporate");
            }



            $("input").on("click",function(){
                _this.warningArea.hide();
            });


            this.messageCode.focus(function () {
                $(".commit-message-code").css("background-color", "#3b4f61");
            });

            this.newPassword.focus(function () {
                $(".commit-password-btn").css("background-color", "#3b4f61");
                if(uc.util.SystemUtil.isCapsLockPressed()){
                    $(".password-description").show();
                    $(".password-description").html(uc_resource.ResetPassword.CapsLock);
                }else{
                    $(".password-description").html("");
                }
            });
            this.newPasswordAgain.focus(function () {
                $(".commit-password-btn").css("background-color", "#3b4f61");
                if(uc.util.SystemUtil.isCapsLockPressed()){
                    $(".again-description").show();
                    $(".again-description").html(uc_resource.ResetPassword.CapsLock);
                }else{
                    $(".again-description").html("");
                }
            });

            this.findByPhone.on("click",function(){
                verifyType = 0;
                $("input").val("");
                $(".findByPhone").css("color","#ffffff");
                $(".findByEmail").css("color","#A3AFBB");
                $(".email-selected").hide();
                $(".phone-selected").show();
                $(".verify-account-area").show();
                $(".message-verify-area").hide();
                $(".modify-password-area").hide();
                _this.description.html(uc_resource.ForgetPassword.PhoneTips);
                uc.modules.password.ForgetPassword.initUI();
                clearInterval(timer);
                _this.counting = false;
                $(".countNumber").html("");
                _this.sendVerifyMessage.removeAttr("disabled").css("background-color","#3b4f61");

            });

            this.findByEmail.on("click",function(){
                verifyType = 1;
                $("input").val("");
                $(".findByPhone").css("color","#A3AFBB");
                $(".findByEmail").css("color","#ffffff");
                $(".email-selected").show();
                $(".phone-selected").hide();
                $(".verify-account-area").show();
                $(".message-verify-area").hide();
                $(".modify-password-area").hide();
                _this.description.html(uc_resource.ForgetPassword.EmailTips);
                uc.modules.password.ForgetPassword.initUI();
                clearInterval(timer);
                _this.counting = false;
                $(".countNumber").html("");
                _this.sendVerifyMessage.removeAttr("disabled").css("background-color","#3b4f61");
            });

            this.cancelBtn.on("click", function () {
                uc.util.WindowUtil.close();
            });

            this.refresh.on("click", function () {
                _this.getCaptcha();
            });

            this.sendVerifyMessage.on("click", function () {

                if($('.corporate-account-area').is(':visible')){
                    var corpAccount = _this.corpAccount.val();
                    var personalAccount = _this.personalAccount.val();
                    var account = personalAccount+"@"+corpAccount+".com";
                }else{
                    var account = _this.verifyAccount.val();
                }

                var code = _this.verifyCode.val();
                if ((account == '') || (code == '')) {
                    _this.showGeneralTips(uc_resource.ForgetPassword.NeitherEmpty);
                } else if(($.trim(account) == '')||($.trim(account) == '')){
                    _this.showGeneralTips(uc_resource.ForgetPassword.NeitherNull);
                }else{
                    uc.util.LogUtil.info("ForgetPassword","verifyAccount","SDK verifyAccount");
                    uc.IClientService.verifyAccount(verifyType,account, code, timestamp);
                }
            });


            this.commitMessageBtn.on("click", function () {
                var account = _this.verifyAccount.val();
                var mobileCode = _this.messageCode.val();
                if (mobileCode == '') {
                    _this.showGeneralTips(uc_resource.ForgetPassword.NeedMessage);
                } else if (account == '') {
                    _this.showGeneralTips(uc_resource.ForgetPassword.Redo);
                } else {
                    var resend = 0;
                    uc.util.LogUtil.info("ForgetPassword","verifyCode","SDK verifyCode");
                    uc.IClientService.verifyCode(verifyType,account, mobileCode, resend);
                }
            });

            this.resendBtn.on("click", function () {
                var account = _this.verifyAccount.val();
                var mobileCode = _this.messageCode.val();
                $(".resend").attr('disabled', "true");
                var resend = 1;
                uc.util.LogUtil.info("ForgetPassword","verifyCode","SDK resendPassword");
                uc.IClientService.verifyCode(verifyType, account, mobileCode, resend);
            });

            this.commitPwdBtn.on('click', function () {
                var password = $(".newPassword").val();
                var passwordAgain = $(".newPasswordAgain").val();
                var account = _this.verifyAccount.val();
                var mobileCode = _this.messageCode.val();
                if (password == "") {
                    _this.showGeneralTips(uc_resource.ResetPassword.NewPasswordPlaceHolder);
                } else if ($.trim(password) == "" || $.trim(passwordAgain) == "") {
                    _this.showGeneralTips(uc_resource.ResetPassword.PasswordTip);
                } else if (passwordAgain == "") {
                    _this.showGeneralTips(uc_resource.ResetPassword.PasswordAgainPlaceHolder);
                } else if (password == passwordAgain) {
                    uc.util.LogUtil.info("ForgetPassword","resetPassword","SDK resetPassword");
                    uc.IClientService.resetPassword(account, password, mobileCode);
                } else {
                    _this.showGeneralTips(uc_resource.ResetPassword.NotMatch);
                }
            });



        },
        showGeneralTips:function(warningTips){
            _this.warningArea.show();
            _this.generalDescription.html(warningTips);
            setTimeout(function(){
                _this.warningArea.fadeOut();
            },1000);
        },
        getParam:function(){
            var url = location.href;
            return url.indexOf('?') >= 0 ? url.substring(url.indexOf('accountType=') + 12, url.length) : 1;
        },
        getCaptcha: function () {
            time = Date.parse(new Date());
            timestamp = time.toString();
            uc.IClientService.getCaptchaURL(timestamp, function (args) {
                if (args[0] == 0) {
                    $(".verifyCodeShow").attr("src", args[1]);
                    uc.util.LogUtil.info("ForgetPassword","getCaptcha succeed","args"+args,null);
                } else {
                    uc.util.LogUtil.info("ForgetPassword","getCaptcha failed","args"+args,null);
                }
            })
        },
        resentCount: function (num) {
            if (_this.counting) {
                return;
            }
            if (num == 0) {
                $(".resend").html(uc_resource.ForgetPassword.Resend);
                $(".resend").removeAttr("disabled");
                return false;
            } else {
                $(".resend").attr('disabled', "true");
                $(".resend").html(num + uc_resource.ForgetPassword.Resend);
            }
            num = num - 1;
            setTimeout(function () {
                _this.resentCount(num)
            }, 1000);
        },
        CountDown: function () {
                timer = setInterval(function () {
                    var minutes = Math.floor(maxtime / 60);
                    var seconds = Math.floor(maxtime % 60);
                    var msg = minutes + ":" + seconds;
                    $(".resend").html(msg + uc_resource.ForgetPassword.Resend);
                    $(".countNumber").html(msg);
                    $(".resend").attr('disabled', "true");
                    $(".send-message-btn").attr('disabled', "true");
                    --maxtime;
                    //setTimeout(uc.modules.password.ForgetPassword.CountDown, 1000);
                }, 1000);
                if(maxtime <= 0){
                    _this.counting = false;
                    $(".resend").html(uc_resource.ForgetPassword.Resend);
                    //$(".countNumber").html(uc_resource.ForgetPassword.SendMessage);
                    $(".countNumber").html("");
                    $(".resend").removeAttr("disabled");
                    $(".send-message-btn").removeAttr("disabled");
                }
        },
        count: function () {
            if (!_this.counting) {
                _this.counting = true;
                setTimeout(uc.modules.password.ForgetPassword.CountDown, 100);
            }
        },
        //verifyType：0表示手机号找回密码，1表示邮箱找回密码
        OnAccountVerified: function (errorCode, verifyType, mobile, email, exprised) {
            $(".resend").removeAttr("disabled");
                if(errorCode == 0) {
                    if(verifyType){
                            _this.messageSend.html(uc_resource.ForgetPassword.EmailTipOne + email + uc_resource.ForgetPassword.EmailTipTwo);
                    }else{
                        _this.messageSend.html(uc_resource.ForgetPassword.SendTipOne + mobile + uc_resource.ForgetPassword.SendTipTwo);
                    }

                    $(".verify-account-area").hide();
                    $(".message-verify-area").show();
                    _this.resentCount(59);
                    $(".resend").attr('disabled', "true");
                    return;
                } else if(errorCode == 20113 || errorCode == 10158){
                    uc.modules.component.PromptMessageBox.open({
                            title: uc_resource.WindowTitle.Notification,
                            message: uc_resource.ForgetPassword.Forbidden,
                            buttonMode: "ok",
                            OkText: uc_resource.Common.OK
                        }, function (result) {
                            if (result === 0) {
                                this.close();
                            }
                    });
                }else if(errorCode == 20139 || errorCode == 10155){
                    uc.modules.component.PromptMessageBox.open({
                        title: uc_resource.WindowTitle.Notification,
                        message: uc_resource.ForgetPassword.LdapAccount,
                        buttonMode: "ok",
                        OkText: uc_resource.Common.OK
                    }, function (result) {
                        if (result === 0) {
                            this.close();
                        }
                    });
                } else if (errorCode == 20103) {
                    _this.showGeneralTips(uc_resource.ForgetPassword.WrongAccount);
                } else if (errorCode == 20110 || errorCode == 10425) {
                    _this.showGeneralTips(uc_resource.ForgetPassword.UserNotExist);
                } else if (errorCode == 20108 || errorCode == 10424) {
                    _this.showGeneralTips(uc_resource.ForgetPassword.WrongCode);
                } else if (errorCode == 20801) {
                    _this.showGeneralTips(uc_resource.ForgetPassword.ServiceException);
                } else if (errorCode == 20109 || errorCode == 10144) {
                     _this.showGeneralTips(uc_resource.ForgetPassword.FiveTimes);
                } else if (errorCode == 20136) {
                     _this.showGeneralTips(uc_resource.ForgetPassword.Hours);
                    _this.sendVerifyMessage.attr('disabled', "true").css("background-color","#adadad");
                    if(exprised){
                        maxtime = exprised;
                        _this.count();
                    }
                } else if (errorCode == 20125) {
                     _this.showGeneralTips(uc_resource.ForgetPassword.NumberNotExist);
                    // _this.sendVerifyMessage.addClass("disable");
                }else if(errorCode == 10158){
                     _this.showGeneralTips(uc_resource.ForgetPassword.FunctionForbidden);
                } else if(errorCode == 10155){
                     _this.showGeneralTips(uc_resource.ForgetPassword.ContactIT);
                } else if(errorCode == 20144){
                     _this.showGeneralTips(uc_resource.ForgetPassword.EmailNotExits);
                }else if(errorCode == 20145){
                     _this.showGeneralTips(uc_resource.ForgetPassword.EmailIsNotVerified);
                } else if(errorCode == 20142){
                     _this.showGeneralTips(uc_resource.ForgetPassword.MessageSendFailed);
                }else if(errorCode == 20143){
                     _this.showGeneralTips(uc_resource.ForgetPassword.EmailSendFailed);
                }else if(errorCode == 20146){
                     _this.showGeneralTips(uc_resource.ForgetPassword.PhoneIsNotVerified);
                }else {
                     _this.showGeneralTips(uc_resource.ForgetPassword.Failed);
                }
                _this.getCaptcha();
        },
        OnCodeVerified: function (errorCode, exprised) {
                if (errorCode == 0) {
                    $(".message-verify-area").hide();
                    $(".modify-password-area").show();
                    $(".description").hide();

                    //获取密码规则
                    uc.IClientService.getPasswordRule(
                        function(args){
                            var rule = uc_resource.ForgetPassword.PasswordRulesOne;
                            if (args[3] == 2) {
                                rule = uc_resource.ForgetPassword.PasswordRulesTwo;
                            } else if (args[3] == 3){
                                rule = uc_resource.ForgetPassword.PasswordRulesThree;
                            }
                            _this.showGeneralTips(rule);
                        }
                    );
                } else if (errorCode == 10427 || errorCode == 20114) {
                    _this.showGeneralTips(uc_resource.ForgetPassword.EnterAgain);
                } else if (errorCode == 10163 || errorCode == 20136) {
                    _this.showGeneralTips(uc_resource.ForgetPassword.Hours);
                    if(exprised){
                        maxtime = exprised;
                        _this.count();
                    }
                } else if (errorCode == 10164 || errorCode == 20137) {
                    _this.showGeneralTips(uc_resource.ForgetPassword.CaptchaFailed);
                } else if (errorCode == 10165 || errorCode == 20138) {
                    _this.showGeneralTips(uc_resource.ForgetPassword.GetAgain);
                } else if (errorCode == 20801) {
                    _this.showGeneralTips(uc_resource.ForgetPassword.ServiceException);
                }

        },
        OnCodeResent:function(errorCode,exprised){
            if(errorCode == 0){
                _this.resentCount(59);
                $(".resend").attr('disabled', "true");
                return;
            }else if (errorCode == 20801) {
                _this.showGeneralTips(uc_resource.ForgetPassword.ServiceException);
            } else if (errorCode == 20109 || errorCode == 10144) {
                _this.showGeneralTips(uc_resource.ForgetPassword.FiveTimes);
            } else if(errorCode == 10158){
                _this.showGeneralTips(uc_resource.ForgetPassword.FunctionForbidden);
            } else if(errorCode == 10155){
                _this.showGeneralTips(uc_resource.ForgetPassword.ContactIT);
            } else if (errorCode == 10427 || errorCode == 20114) {
                _this.showGeneralTips(uc_resource.ForgetPassword.EnterAgain);
            } else if (errorCode == 10163 || errorCode == 20136) {
                _this.showGeneralTips(uc_resource.ForgetPassword.Hours);
                if(exprised){
                    maxtime = exprised;
                    _this.count();
                }
            } else if (errorCode == 10164 || errorCode == 20137) {
                _this.showGeneralTips(uc_resource.ForgetPassword.CaptchaFailed);
            } else if (errorCode == 10165 || errorCode == 20138) {
                _this.showGeneralTips(uc_resource.ForgetPassword.GetAgain);
            }
        },
        OnPasswordReset: function (errorCode) {
                if (errorCode == 0) {
                    uc.IUIService.triggerCrossWindowEvent(uc.constants.UI.CrossWindowEventName.OnPasswordReset, {});
                    uc.util.WindowUtil.close();
                } else if (errorCode == 20120 || errorCode == 10130) {
                    _this.showGeneralTips(uc_resource.ForgetPassword.NotTheSame);
                } else if (errorCode == 10155) {
                    _this.showGeneralTips(uc_resource.ForgetPassword.ContactIT);
                } else if (errorCode == 10158) {
                    _this.showGeneralTips(uc_resource.ForgetPassword.Forbidden);
                } else if (errorCode == 10428 || errorCode == 20121) {
                    _this.showGeneralTips(uc_resource.ForgetPassword.SetFailed);
                } else if (errorCode == 10146 || errorCode == 20126) {
                    _this.showGeneralTips(uc_resource.ForgetPassword.PasswordRulesOne);
                } else if (errorCode == 10148 || errorCode == 20129) {
                    _this.showGeneralTips(uc_resource.ForgetPassword.PasswordRulesTwo);
                } else if (errorCode == 10149 || errorCode == 20130) {
                    _this.showGeneralTips(uc_resource.ForgetPassword.PasswordRulesThree);
                } else if (errorCode == 20801) {
                    _this.showGeneralTips(uc_resource.ForgetPassword.ServiceException);
                } else {
                    _this.showGeneralTips(uc_resource.ForgetPassword.SetFailed);
                }
        }

    };

    jQuery(document).ready(function () {
        uc.init();
        uc.modules.password.ForgetPassword.init();
    })
})(uc);