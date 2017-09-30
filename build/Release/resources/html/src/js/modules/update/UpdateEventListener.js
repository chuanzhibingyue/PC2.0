/**
 * Created by zhen.xu on 2015/9/15.
 */
(function(uc){
    uc.modules.update.UpdateEventListener = function(){

    };
    jQuery.extend(uc.modules.update.UpdateEventListener.prototype,
        uc.interfaces.IUIEvent,
        uc.interfaces.IUpdateEvent,  {
            OnInitClient: function(clearData){
                uc.modules.update.Update.init();
            },
            OnCheckUpdates: function (errorCode, updateFlag, version, notes, downloadUrl) {
                uc.util.LogUtil.info('UpdateEventListener', 'OnCheckUpdates', 'getUpdateResult', {
                        errorCode: errorCode, updateFlag: updateFlag, version: version, notes:notes, downloadUrl: downloadUrl
                });

                uc.modules.update.UpdateManager.OnUpdateNotesReceived(errorCode, updateFlag, version, notes, downloadUrl);
            }
    })
})(uc);