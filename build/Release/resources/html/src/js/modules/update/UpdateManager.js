/**
 * Created by zhen.xu on 2015/9/28.
 */
(function(uc){
    var _this = null;
    uc.modules.update.UpdateManager = {
        init: function(){
            _this = this;
            this.initUpdateNotes();
        },
        initUpdateNotes: function () {
            uc.util.LogUtil.info('uc.modules.update.UpdateManager', 'getUpdateNotes', 'start check update', '');
            uc.IUpdateService.checkUpdates();
        },
        OnUpdateNotesReceived: function (errorCode, updateFlag, version, notes, downloadUrl) {
            uc.util.LogUtil.info('UpdateManager', 'OnUpdateNotesReceived', 'Update Notes received', {errorCode: errorCode, updateFlag: updateFlag, downloadUrl: downloadUrl});
            if(errorCode == uc.ErrorCode.Success && updateFlag) {
                uc.modules.update.Update.initUpdateNotes(version, notes);
            } else {
                uc.modules.update.Update.checkUpdateFailed(downloadUrl);
            }
        },
        update: function () {
            setTimeout(function () {
                uc.IClientService.update();
            }, 300)
        }
    }
})(uc);
