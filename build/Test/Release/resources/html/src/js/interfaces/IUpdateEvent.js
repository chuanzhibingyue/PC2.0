/**
 * Created by zhen.xu on 2015/9/15
 */
(function(uc){
    uc.interfaces.IUpdateEvent = {
        OnCheckUpdates : function (errorCode, updateFlag, version, notes, downloadUrl) {}
    }
})(uc);