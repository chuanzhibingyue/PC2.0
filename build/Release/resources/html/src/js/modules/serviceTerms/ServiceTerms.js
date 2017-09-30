/**
 * Created by pan.li on 2015/9/17.
 */
(function (uc) {
    var _this;
    uc.modules.serviceTerms.ServiceTerms = {
        init: function () {
            _this = this;
            this.close = $(".icon-close.tool-icon");
            this.version = $(".version-num");

            this.initUI();
            this.bindEvent();
        },
        initUI: function () {
            _this.getClientVersion();
        },

        bindEvent: function () {
            document.onkeydown = function (e) {
                if (e.keyCode == 27) {
                    uc.util.WindowUtil.close();
                }
            };
            this.close.on("click", function () {
                uc.util.WindowUtil.close();
            })
        },
        getClientVersion: function () {
            uc.IClientService.getClientVersion(function (args) {
                uc.util.LogUtil.info("ServiceTerms", "getClientVersion", "version detail:", args);
                var errorCode = args[0];
                var version = args[1];
                if (errorCode == 0) {
                    _this.version.html(version);
                } else {
                    _this.version.html("");
                }
            });
        }
    };
    jQuery(document).ready(function () {
        uc.init();
        uc.modules.serviceTerms.ServiceTerms.init();
    })
})(uc);