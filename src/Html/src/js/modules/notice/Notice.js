/**
 * Created by wanglei on 15/10/10.
 */
(function (uc) {
    uc.modules.notice.Notice = {
        init: function (isFirst) {
            if ($("iframe", uc.ui.Notice.MainPanel).length === 0) {
                uc.ui.Notice.MainPanel.empty();
                var path = uc.modules.notice.Notice.getRootPath() + "/notice/index.html";
                this.node = $('<div class="notice-container">' +
                    '<iframe class="iframe" src="' + path + '" frameborder="0"></iframe>' +
                    '</div>'
                ).appendTo(uc.ui.Notice.MainPanel);
                this.inited = true;

            } else {
                if (isFirst) { //首次不需要向iframe广播
                    return;
                }
                this.broadCast('changeContext', '/');
            }
            this.currentType = 1;//发布者类型为1
            if (!isFirst) {
                uc.ui.Notice.MainPanel.removeClass("hide");
            }
            uc.ui.Notice.MainPanel.removeClass("receiver");
        },
        showReceiverView: function () {
            if(uc.modules.main.Main.moduleId != uc.constants.UI.ModuleId.Message){ //因为事件的触发是异步的，可能导致在打开时当前module不是消息
                return;
            }
            uc.ui.Notice.MainPanel.addClass("receiver");
            var caption = uc.constants.UI.DragCaption.Message;
            var left = caption[0].left;
            caption[0].left = 410;
            uc.util.WindowUtil.setCaptions(caption);
            caption[0].left = left;
            if ($("iframe", uc.ui.Notice.MainPanel).length === 0) { //第一次加载接收端时 iframe未初始化
                uc.ui.Notice.MainPanel.empty();
                var path = uc.modules.notice.Notice.getRootPath() + "/notice/index.html#/receiver";
                this.node = $('<div class="notice-container">' +
                    '<iframe class="iframe" src="' + path + '" frameborder="0"></iframe>' +
                    '</div>'
                ).appendTo(uc.ui.Notice.MainPanel);
                uc.ui.Notice.MainPanel.removeClass("hide");
                uc.util.LogUtil.info("Notice", "showReceiverView", "init receiver page", "");
                return;
            }

            if (this.currentType === 0) {
                uc.ui.Notice.MainPanel.removeClass("hide");
                this.broadCast('BackToAccount');
                uc.util.LogUtil.info("Notice", "showReceiverView", "show receiver page", "");
                return;
            }
            this.broadCast('changeContext', '/receiver');
            uc.util.LogUtil.info("Notice", "showReceiverView", "switch to receiver page", "");
            this.currentType = 0;//接收者类型为0
            uc.ui.Notice.MainPanel.removeClass("hide");
        },
        initFail: function () {
            this.init();
        },
        getRootPath: function () {
            var href = location.href;
            var index = href.indexOf('/html');
            return href.substring(0, index + 6);
        },
        changNoticeFrameSize: function () {
            $(".noitce-container .iframe").css("height", $("#menu-panel").height());
        },
        broadCast: function (type, params) {
            if (!!this.node) {
                var noticeFrame = this.node.find("iframe");
                if (noticeFrame.length > 0) {
                    var noticeWindow = noticeFrame[0].contentWindow;
                    if (typeof noticeWindow.broadCast === "function") {
                        this.node.find("iframe")[0].contentWindow.broadCast(type, params);
                    }
                }
            }
        },
        hideView: function () {
            uc.util.LogUtil.info("Notice", "hideReceiverView", "hide receiver page", "");
            if (!uc.ui.Notice.MainPanel.hasClass("hide")) {
                uc.util.WindowUtil.setCaptions(uc.constants.UI.DragCaption.Message);
                uc.ui.Notice.MainPanel.addClass("hide");
            }
        },
        setMenuTips: function () {
            uc.modules.component.MainMenuBar.noticeMenu.attr("qtips", this.appInfo.appTitle);
        },
        unreadCount: 0,
        appInfo: {
            appId: 10488557,
            appDesc: '',
            appLogo: '../images/notice/notice_avatar.png',
            appTitle: 'i广播',
            isShow: false,
            status: 1
        }
    };
})(uc);
