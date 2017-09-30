/**
 * Created by senlin.ouyang on 2015/7/17.
 */
(function(uc){
    uc.modules.login.LoginEventListener = function(){

    };
    jQuery.extend(uc.modules.login.LoginEventListener.prototype,
        uc.interfaces.IClientEvent,
        uc.interfaces.IUpdateEvent,
        uc.interfaces.IPbxEvent,
        {
            OnLogin: function(errorCode, authType,pbxType, capability){
                uc.util.LogUtil.info('Login', 'OnLogin', 'errorCode = ' + errorCode+",authType"+authType+ ',capability='+ capability);
                uc.modules.login.Login.OnLogin(errorCode, authType,pbxType, capability);
            },
            OnLogout: function(errorCode){
                uc.util.LogUtil.info('Login', 'OnLogout', 'errorCode = ' + errorCode, null);
            },
            OnPasswordExpired: function(dueDays){
                uc.util.LogUtil.info('Login', 'OnPasswordExpired', 'dueDays = ' + dueDays, null);
            },
            OnUCASLogin: function(/*int*/errorCode){
                uc.util.LogUtil.info('Login', 'OnUCASLogin', 'errorCode = ' + errorCode, null);
                uc.modules.login.Login.OnUCASLogin(errorCode);
            },
            OnUCASLogout: function(/*int*/errorCode){
                uc.util.LogUtil.info('Login', 'OnUCASLogout', 'errorCode = ' + errorCode, null);
                uc.modules.login.Login.OnUCASLogout(errorCode);
            },
            OnCheckUpdates: function (errorCode, updateFlag, version, notes) {
                var _data = {
                    errorCode: errorCode,
                    updateFlag: updateFlag,
                    version: version,
                    notes: notes
                };

                uc.util.LogUtil.info('Login', 'OnCheckUpdates', 'SDK checkUpdates', _data);
                uc.util.StorageUtil.removeData('updateForce');
                uc.util.StorageUtil.setData('checkUpdates', JSON.stringify(_data));
                uc.modules.login.Login.OnCheckUpdates(errorCode, updateFlag, version, notes);
                //uc.IClientService.loginUCAS();
            },
            OnCloudLogin:function(errorCode){
                uc.util.LogUtil.info('Login', 'OnCloudLogin', 'errorCode = ' + errorCode, null);
                uc.util.StorageUtil.setData('CloudLoginResult',errorCode);
            },
            OnPbxLogin:function(errorCode){
                uc.util.LogUtil.info('Login', 'OnPbxLogin', 'errorCode = ' + errorCode, null);
                uc.util.StorageUtil.setData('OnPbxLoginResult',errorCode);
            },
            OnFileSendToBee:function(filePath){
                uc.util.LogUtil.info('Login', 'OnFileSendToBee', 'sendFilePath = ' + filePath, null);
                uc.util.StorageUtil.setData('OnFileSendToBee',filePath);
            }
    })
})(uc);