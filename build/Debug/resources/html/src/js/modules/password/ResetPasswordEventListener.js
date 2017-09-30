/**
 * Created by pan.li on 2015/9/15.
 */
(function(uc){
    uc.modules.password.ResetPasswordEventListener = function(){

    };
    jQuery.extend(uc.modules.password.ResetPasswordEventListener.prototype,
        uc.interfaces.IClientEvent,{
            OnPasswordModified: function(errorCode){
                uc.util.LogUtil.info("ResetPassword","OnPasswordModified","errorCode="+errorCode,null);
                uc.modules.password.ResetPassword.OnPasswordModified(errorCode);
            }
        })
})(uc);