/**
 * Created by zhen.xu on 2015/11/28.
 */
(function (uc) {
    uc.modules.video.VideoEventListener = function () {

    }

    jQuery.extend(uc.modules.video.VideoEventListener.prototype,
        uc.interfaces.IUIEvent, {
        OnPreviewVideo: function (jsonData) {
            console.log('On preview video... jsonData:', jsonData);
            uc.modules.video.VideoManager.openVideoPreviewWindow();
        }
    });
})(uc);