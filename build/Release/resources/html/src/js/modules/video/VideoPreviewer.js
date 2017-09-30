/**
 * Created by zhen.xu on 2015/11/28.
 */
(function (uc) {
    uc.modules.video.VideoPreviewer = {
        init: function () {
            this.closeBtn = $('.icon-preview-close');
            this.node = $('video');
            this.bindEvent();
            this.setVideoCenter();
        },
        bindEvent: function () {
            jQuery(document.body).keyup(function(e){
                if(e.which == 27){
                    uc.util.WindowUtil.close();
                }
            });

            this.closeBtn.click(function () {
                uc.util.WindowUtil.close();
            });
        },
        setVideoCenter: function () {
            this.node.css('marginLeft', -(this.node.width()/2));
            this.node.css('marginTop', -(this.node.height()/2));
        }
    }

    jQuery(document).ready(function () {
        uc.init();
        uc.modules.video.VideoPreviewer.init();
    })
})(uc);