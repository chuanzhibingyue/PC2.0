/**
 * Created by pan.li on 2015/8/26.
 */
(function(uc){
    uc.modules.password.ForgetPasswordEventListener = function(){

    };
    jQuery.extend(uc.modules.password.ForgetPasswordEventListener.prototype,
        uc.interfaces.IClientEvent,{
            OnPasswordReset: function(errorCode){
                uc.util.LogUtil.info("ForgetPassword","OnPasswordReset","errorCode="+errorCode,null);
                uc.modules.password.ForgetPassword.OnPasswordReset(errorCode);
            },
            OnAccountVerified: function(errorCode, verifyType, mobile, email, exprised){
                uc.util.LogUtil.info("ForgetPassword","OnAccountVerified","errorCode="+errorCode+",mobile="+mobile+",verifyType="+verifyType+",email="+email+",exprised="+exprised,null);
                uc.modules.password.ForgetPassword.OnAccountVerified(errorCode, verifyType, mobile, email, exprised);
            },
            OnCodeVerified: function(errorCode, exprised){
                uc.util.LogUtil.info("ForgetPassword","OnCodeVerified","errorCode="+errorCode+',exprised='+exprised,null);
                uc.modules.password.ForgetPassword.OnCodeVerified(errorCode,exprised);
            },
            OnCodeResent:function(errorCode, exprised){
                uc.util.LogUtil.info("ForgetPassword","OnCodeResent","errorCode="+errorCode+',exprised='+exprised,null);
                uc.modules.password.ForgetPassword.OnCodeResent(errorCode, exprised);
            }
        })
})(uc);