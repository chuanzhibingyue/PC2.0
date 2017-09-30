(function (uc) {
    if (uc.modules.meeting.shareMeetingVideo) {
        return;
    }
    var isUEdit, _meeting, data,userInfo;

    function _init() {
        data = uc.util.StorageUtil.getData("Meeting-Video-Share:Data");

        uc.modules.component.TitleBarButton.init();
        $(document).keydown(function(e){
            if(e.which == 27){
                uc.util.WindowUtil.close();
            }
        });
        _bindEvent();

    }
    function _bindEvent() {

        $(".share-Meeting .release-share").click(function () {
            var summary = $(".share-Meeting .share_content").val();
            var videoName=data.meetingName, videoSize=parseInt(data.videoSize),
                videoURL=data.videoURL, videoData=data.videoData,
                videoThumbUrl=data.thumbnailsURL,
                videoDuration=parseInt(data.videoDuration);
            uc.IMeetingService.shareMeetingRecording(videoURL,videoThumbUrl, videoName,videoSize,videoDuration, summary,function(arg){
                if(arg[0]==0){
                    uc.util.WindowUtil.close();
                }
            })

        });

        $(".share-Meeting .cancel-share").click(function () {
            uc.util.WindowUtil.close();
        });
    }

    uc.modules.meeting.shareMeetingVideo = {

        init: function () {
            _init();
        }
    }
})(uc);

jQuery(document).ready(function () {
    uc.init();
    uc.modules.meeting.shareMeetingVideo.init();
});