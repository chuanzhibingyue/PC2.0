/**
 * Created by zuo.dai on 2015/10/10.
 */
(function(uc){
    var _this = null;
    uc.modules.openApi.ApiMsgPreview = {
        init: function(){
            _this = this;
            this.bindEvent();
            var sendObj = uc.util.StorageUtil.getData("ShowApiMsgPreview");
            $(".content").text(sendObj.content);
        },
        bindEvent: function(){
            $('.icon-close').on('click',
                function(){
                    uc.util.WindowUtil.close();
                }
            );
        }
    };
    jQuery(document).ready(function(){
        uc.init();
        uc.modules.openApi.ApiMsgPreview.init();
    });
})(uc);