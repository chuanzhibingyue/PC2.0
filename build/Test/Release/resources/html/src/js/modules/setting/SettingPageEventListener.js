/**
 * Created by pan.li on 2016/5/23.
 */

(function(uc){
    uc.modules.setting.SettingPageEventListener = function(){

    };
    jQuery.extend(uc.modules.setting.SettingPageEventListener.prototype,
        uc.interfaces.IFSEvent,
        uc.interfaces.IClientEvent,{
            OnFileUploading: function (errorCode, taskId, percent) {
                uc.modules.setting.Setting.OnFileUploading(errorCode, taskId, percent);
            },
            OnFileUploaded: function (errorCode, taskId, fileName, fileSize, operatorId, fid, operatorTime, downloadURL, convertDownloadURL){
                var _data = {
                    errorCode: errorCode,
                    taskId: taskId,
                    fileName: fileName,
                    fileSize: fileSize,
                    operatorId: operatorId,
                    fid: fid,
                    operatorTime: operatorTime,
                    downloadURL: downloadURL,
                    convertDownloadURL: convertDownloadURL
                };

                uc.util.LogUtil.info('Setting', 'OnFileUploaded', 'FS uploadLog', _data);
                uc.modules.setting.Setting.OnFileUploaded(errorCode, taskId, fileName, fileSize, operatorId, fid, operatorTime, downloadURL, convertDownloadURL);
            },
            OnLogUploaded:function(errorCode){
                uc.modules.setting.Setting.OnLogUploaded(errorCode);
            },
            OnArchiveLogFinished:function(filePath){
                uc.modules.setting.Setting.OnArchiveLogFinished(filePath);
            },
            OnGetUserNotifyStatus:function(errorCode,isNotify){
                uc.modules.setting.Setting.OnGetUserNotifyStatus(errorCode, isNotify);
            },
            OnUpdateUserNotifySet:function(errorCode, userId){
                uc.modules.setting.Setting.OnUpdateUserNotifySet(errorCode, userId);
            }
        })
})(uc);