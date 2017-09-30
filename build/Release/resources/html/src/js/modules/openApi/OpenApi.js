/**
 * Created by zhen.xu on 2015-9-16
 */
(function(uc){
    var _this = null;
    uc.modules.openApi.OpenApi = {
        init: function () {
            _this = this;
            this.loginUserInfo = uc.IContactService.getCurrentUserInfo();
            _this.token = '';
            this.containerWrap = $('<div class="open-api-container-wrap"></div>');//最外层容器
            this.prevPageContainer = $('<div class="open-api-container-prev disabled"></div>');//上一页容器
            this.nextPageContainer = $('<div class="open-api-container-next"></div>');//下一页容器
            this.appsContainer = $('<div class="open-api-container-apps"></div>');//应用父容器
            this.appsWrap = $('<div class="open-api-apps-wrap"></div>');//应用列表包装层
            this.pageNumberWrap = $('<div class="page-num"></div>');//页数显示
            this.appShowCount = 9;//每页显示app个数

            this.container = $('<ul class="open-api-container"></ul>');
            this.container.parent().removeClass('no-app');
            this.currentPage = 1;
            this.totalPage = 0;
            this.apps = [];
            this.containerDefaultW = 700;
            this.initToken();
        },
        initUI: function () {
            this.clearWrap();
            this.containerWrap.empty();
            this.prevPageContainer.addClass('disabled').empty().appendTo(this.containerWrap);
            this.appsContainer.empty().appendTo(this.containerWrap);
            this.nextPageContainer.empty().appendTo(this.containerWrap);
            this.pageNumberWrap.empty().appendTo(this.containerWrap);
            this.appsWrap.attr('style', '').empty().appendTo(this.appsContainer);
            this.containerWrap.appendTo(uc.ui.App.ContentPanelBody);//创建外层容器
            this.bindEvent();
        },
        bindEvent: function () {
            //上一页
            _this.prevPageContainer.on('click', function (e) {
                _this.prevPage();
            });

            //下一页
            _this.nextPageContainer.on('click', function () {
                _this.nextPage();
            });

            //页码
            _this.pageNumberWrap.on('click', 'div', function (e) {
                e.stopPropagation();
                var _index = parseInt($(this).index()) + 1;
                _this.changePage(_index);
            });
        },
        initToken: function () {
            uc.IClientService.getSessionId(function (token){
                _this.token = token[1];
            });
        },
        OnAppListReceived: function (errorCode) {
            if(errorCode != uc.ErrorCode.Success || uc.modules.openApi.OpenApiManager.getOpenApiInfoCount() == 0){
                this.clearWrap();
                uc.ui.App.ContentPanelBody.addClass('no-app');
                uc.modules.openApi.OpenApiManager.stopLoadingApps();
                return;
            }

            this.initUI();
            var apps = uc.modules.openApi.OpenApiManager.getAllOpenApiInfo();
            var openApiView;
            if (errorCode == 0) {
                var i = 1, j = 0;
                //var appCount = parseInt((apps.length / this.appShowCount == 0 ? apps.length / this.appShowCount : apps.length / this.appShowCount + 1));
                var appCount = parseInt((apps.length / this.appShowCount <= 1 ? Math.ceil(apps.length / this.appShowCount) : (apps.length % this.appShowCount == 0 ? apps.length / this.appShowCount : apps.length / this.appShowCount + 1)));
                uc.util.LogUtil.info('OpenApi', 'OnApplistReceived', 'appCount', {appCount: appCount});
                _this.totalPage = appCount;
                this.initPage(appCount);
                this.setContainerWidth(appCount);
                for (; i < appCount + 1; i++ ) {
                    var _parent = $('<ul class="open-api-container"></ul>');
                    var _appsWrap = $('<div class="app-wrap"></div>');
                    for (j; j < this.appShowCount * i; j++) {
                        openApiView = new uc.modules.openApi.OpenApiView();
                        openApiView.init({parentNode: _parent, item: apps[j], account: this.loginUserInfo.account, token: _this.token});
                    }
                    _parent.appendTo(_appsWrap);
                    _appsWrap.appendTo(this.appsWrap);
                }
                uc.modules.openApi.OpenApiManager.stopLoadingApps();
            }
        },
        initPage: function (appCount) {
            for(var i = 0; i < appCount; i++) {
                $('<div></div>').appendTo(this.pageNumberWrap);
            }

            if (appCount <= uc.ErrorCode.OnlyOneApp) {
                this.prevPageContainer.css('visibility', 'hidden');
                this.nextPageContainer.css('visibility', 'hidden');
            }

            if (appCount >= uc.ErrorCode.OnlyOneApp){
                $('.no-app').removeClass('no-app');
            }

            this.pageNumberWrap.find('div:first').addClass('active');
        },
        setContainerWidth: function (appCount) {
            this.appsWrap.css('width', (appCount * this.containerDefaultW ) + 30)
        },
        prevPage:function () {
            if (_this.currentPage == 1) {
                return;
            }
            _this.prevPageContainer.removeClass('disabled');
            _this.changePage(_this.currentPage - 1);
        },
        nextPage: function () {
            if (_this.currentPage == _this.totalPage) {
                return;
            }

            _this.nextPageContainer.removeClass('disabled');
            _this.changePage(_this.currentPage + 1);
        },
        changePage: function (currentPage) {
            var _width = (currentPage - 1) * this.containerDefaultW;
            $('.open-api-apps-wrap').css('transform', 'translateX(-740px)');
            this.appsWrap.css('transform', 'translateX(-'+ _width +'px)');
            this.onPageChange(currentPage);
        },
        onPageChange: function (currentPage) {
            _this.currentPage = currentPage;
            _this.pageNumberWrap.find('div').removeClass('active');
            _this.pageNumberWrap.find('div:eq('+ (currentPage - 1) +')').addClass('active');
            this.containerWrap.find('div.disabled').removeClass('disabled');
            if (currentPage == 1) {
                _this.prevPageContainer.addClass('disabled');
                return;
            }

            if (currentPage == _this.totalPage) {
                _this.nextPageContainer.addClass('disabled');
                return;
            }
        },
        getAppAuth: function (url) {
            var _join = (url.indexOf('?') >= 0 ? '&' : '?');
            return _join + 'uc_account=' + this.loginUserInfo.account +'&uc_token=' + this.token;
        },
        clearWrap: function () {
            this.containerWrap.unbind();
            uc.ui.App.ContentPanelBody.find('.open-api-container-wrap').remove();
        }
    };
})(uc);