/**
 * Created by zhen.xu 2016-2-29
 */
(function(uc) {
    if (uc.modules.meeting.noticeInMeeting) {
        return;
    }
    var param = parseInt(window.location.href.split("?")[1].substring(3, 4));// 1 = logout ; 2 = quit

    function _init() {
        uc.modules.component.TitleBarButton.init(null, {
            events: {
                close: function () {
                    _close();
                }
            }
        });

        $(this).on("keydown", function(event) {
            if (event.keyCode == 27) {
                uc.util.WindowUtil.close();
            }
        });

        bindEvent();

        uc.util.WindowUtil.setOnTop(false);
    }

    function bindEvent () {
        $("#cancel").on("click", function () {
            _close();
        });

        $("#quit").on("click", function () {
            // to fix bug: 在任务栏右键菜单中点击“注销”之后，再登录进来时消息提醒弹框不显示的bug。 by xue.bai_2
            uc.util.StorageUtil.removeData('bubble');
            var clearItem=["CloudLoginResult"];
            for(var i=0;i<clearItem.length;i++){
                uc.util.StorageUtil.removeData(clearItem[i]);
            }
            if (param == 1) {
                uc.IMeetingService.killMeetingClient();
                uc.IClientService.logoutUCAS();
                var options = {
                    title: 'bee',// 为了解决任务栏右键点击蜜蜂时不弹出主窗口的bug，把窗口名称“蜜蜂”改成“bee”，请勿回退
                    width: 420,
                    height: 600,
                    refreshIfExist: false,
                    centerScreen: true,
                    modal: false,
                    resizable: false,
                    minWidth: 420,
                    minHeight: 600,
                    hasTaskBarIcon: true,
                    behaviorIfSCClose: uc.constants.UI.WindowCloseBehavior.Quit,
                    isChild: false,
                    caption: uc.constants.UI.DragCaption.Login,
                    closeOtherWindow: true
                };
                uc.util.WindowUtil.open('index.html', 'login', options);
                uc.IClientService.logout();
            } else {
                uc.IMeetingService.killMeetingClient();
                uc.IClientService.logoutUCAS();
                uc.IClientService.logout();
                uc.quit();
            }
        });
    }

    function _close() {
        uc.util.WindowUtil.close();
    }

    uc.modules.meeting.noticeInMeeting = {
        init: function () {
            _init();
        }
    };

}) (uc);

$().ready(function() {
    uc.init();
    uc.modules.meeting.noticeInMeeting.init();
});