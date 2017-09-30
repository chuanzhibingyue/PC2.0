/**
 * Created by waouyang on 15/8/18.
 */
(function(uc){
    uc.modules.image.ImageEventListener = function(){};
    jQuery.extend(uc.modules.image.ImageEventListener.prototype,
        uc.interfaces.IUIEvent,
        uc.interfaces.IFSEvent, {
            /**
             * event for preview image when user click image in Message Box
             * @param chatId    chat id of the image
             * @param chatType  chat type of the image
             * @param msgId     image id
             * @param imageName   image name
             */
            OnPreviewChatImage: function(chatId, chatType, msgId, imageName){
                uc.modules.image.ImageManager.OnPreviewChatImage(chatId, chatType, msgId, imageName);
            },
            OnPreviewImage: function(imagePath){
                uc.modules.image.ImageManager.OnPreviewImage(imagePath);
            },
            OnFileDownloaded: function(errorCode, taskId, savePath){
                uc.modules.image.ImageManager.OnFileDownloaded(errorCode, taskId, savePath);
            },
            OnOpenImageCropperWindow: function(key){
            	uc.modules.image.ImageManager.OnOpenImageCropperWindow(key);
			}
    });
})(uc);