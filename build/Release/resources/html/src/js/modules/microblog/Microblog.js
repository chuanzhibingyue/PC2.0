/**
 * Created by zhen.xu on 2015/9/18.
 */

(function (uc) {
    var _this = null, timer;
    uc.modules.microblog.Microblog = {
        init: function () {
            return;//第一版不需要此功能
            _this = this;
            this.loginUserInfo = uc.IContactService.getCurrentUserInfo();
            this.homePageUrl = '';
            this.getHomePageURL(this.loginUserInfo.userId, this.setPageUrl);
            this.timerSpeed = 120000;
            this.getNewMsgCount();
            this.startGetMsgTimer();
        },
        getNewMsgCount: function () {
            uc.util.LogUtil.info('MicroBlog', 'GetNewMsgCount', 'Get new message count', null);
            uc.IMicroblogService.getNewMsgCount();
        },
        getHomePageURL: function (userId, callback) {
            uc.IMicroblogService.getHomePageURL(userId, callback);
        },
        getPageUrl: function () {
            return _this.homePageUrl;
        },
        setPageUrl: function (datas) {
            _this.homePageUrl = datas[1];
            uc.util.StorageUtil.setData('currentHomeUrl', _this.homePageUrl);
        },
        OnNewMsgCountGet: function (errorCode, countAll, relationCount, timenewsCount, pollCount, caseCount) {
            if (errorCode == 0) {
                if (countAll >= 1) {
                    uc.modules.component.ExtraMenuBar.showMicroBlogMsg();
                } else {
                    uc.modules.component.ExtraMenuBar.hideMicroBlogMsg();
                }
            }
        },
        OnOpenMyLinkPage: function () {
            var _url = this.getPageUrl();
            this.getNewMsgCount();
            window.open(_url, null, '_blank');
        },
        startGetMsgTimer: function () {
            this.stopGetMsgTimer();
            timer = setInterval(function () {
                _this.getNewMsgCount();
            }, _this.timerSpeed);
        },
        stopGetMsgTimer: function () {
            clearInterval(timer);
        }
    }
})(uc);