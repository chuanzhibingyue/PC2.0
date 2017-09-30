/**
 * Created by waouyang on 15/9/11.
 */
(function(uc){
    var _this = null;
    uc.modules.proxy.ProxyAuthenticate = {
        init: function(){
            _this = this;
            this.authType = uc.util.StorageUtil.getData('ProxyAuthenticateAuthType');
            uc.modules.component.TitleBarButton.init();
            this.bindEvent();
            setTimeout(function(){
                _this.accountInput.focus();
            }, 200);
        },
        bindEvent: function(){
            this.accountInput = $('.input.account');
            this.passwordInput = $('.input.password');
            this.authResultNode = $('.auth-result');
            this.okBtn = $('.btn.ok');
            this.cancelBtn = $('.btn.cancel');
            this.cancelBtn.click(function(){
                uc.util.WindowUtil.close();
            });
            this.okBtn.click(function(){
                _this.setProxyAuthenticateInfo();
            });
            jQuery(document.body).keypress(function(e){
                _this.OnBodyPressed(e);
            });
        },
        setProxyAuthenticateInfo: function(){
            var account = this.accountInput.val().trim();
            var password = this.passwordInput.val().trim();
            if(!account && !password){
                this.authResultNode.text(uc_resource.Proxy.NeedAccountPass);
                return;
            }
            uc.IClientService.setProxyAuthInfo(parseInt(this.authType), account, password, function(args){
                uc.IUIService.triggerCrossWindowEvent(uc.constants.UI.CrossWindowEventName.OnProxyAuthenticateSet, {});
                uc.util.WindowUtil.close();
            })
        },
        OnBodyPressed: function(e){
            if(e.which == 13){
                if($(e.target).hasClass('account')){
                    this.passwordInput.focus();
                }else if($(e.target).hasClass('password')){
                    this.setProxyAuthenticateInfo();
                }else if($(e.target).hasClass('ok')){
                    this.setProxyAuthenticateInfo();
                }
            }
        }
    };
    jQuery(document).ready(function(){
        uc.init();
        uc.modules.proxy.ProxyAuthenticate.init();
    });
})(uc);