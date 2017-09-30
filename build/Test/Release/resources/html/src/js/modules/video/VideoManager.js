/**
 * Created by zhen.xu on 2015/11/28.
 */
(function (uc) {
    uc.modules.video.VideoManager = {
        openVideoPreviewWindow: function () {
            var options = {
                title: uc_resource.WindowTitle.VideoPreview,
                width:  screen.availWidth,
                height: screen.availHeight,
                left: 0,
                top: 0,
                refreshIfExist: false,
                centerScreen: false,
                modal: true,
                resizable: false,
                minWidth: screen.availWidth,
                minHeight: screen.availHeight,
                behaviorIfSCClose: uc.constants.UI.WindowCloseBehavior.Close,
                borderSpace: {
                    leftBorder: 0,
                    topBorder: 0,
                    rightBorder: 0,
                    bottomBorder: 0
                },
                removeBorder: true
            };
            uc.util.WindowUtil.open('page/videoPreview.html', 'videoPreviewer', options);
        }
    };
})(uc);