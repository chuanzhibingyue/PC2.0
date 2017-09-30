/**
 * Created by waouyang on 15/10/10.
 */
(function(uc){
    uc.modules.cloud.CloudEventListener = function(){

    };
    jQuery.extend(uc.modules.cloud.CloudEventListener.prototype,
        uc.interfaces.IClientEvent,
        uc.interfaces.IContactEvent,
        uc.interfaces.IGroupEvent,
        uc.interfaces.IUIEvent,{
            OnModuleChanged: function(moduleId,mountId){
                uc.modules.cloud.CloudManager.OnModuleChanged(moduleId,mountId);
            },
            OnWindowResized: function(){
                uc.modules.cloud.Cloud.changCloudFrameSize();
            },
            //contact events
            OnBasicContactInfoReceived : function (errorCode, userIds, contacts) {
                if(errorCode == uc.ErrorCode.Success && contacts && contacts.length){
                    uc.modules.cloud.CloudManager.OnContactInfoReceived(contacts);
                }
            },
            OnUserAvatarDownloaded: function(userId, serverAvatarUrl, localAvatarUrl){
                uc.modules.cloud.CloudManager.OnUserAvatarDownloaded(userId, serverAvatarUrl, localAvatarUrl);
            },
            OnGkClientConnectionStatus:function(errorCode,mountIdData){
                uc.modules.cloud.CloudManager.OnGkClientConnectionStatus(errorCode,mountIdData);
            },
            OnCreateCloud:function(){
                uc.modules.cloud.CloudManager.cloudLibCreate();
            },
            OnCloudFilePreviewer:function(data){
                uc.modules.cloud.CloudManager.OnCloudFilePreviewer(data);
            },
            OnCloudChatSet:function(groupId){
                uc.modules.cloud.CloudManager.OnCloudChatSet(groupId);
            }
        });
})(uc);