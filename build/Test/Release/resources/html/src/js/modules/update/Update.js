/**
 * Created by zhen.xu on 2015/9/15
 */
(function (uc) {
    var _this = null;
    uc.modules.update.Update = {
        init: function () {
            _this = this;
            this.body = $('body');
            this.btnIgnore = $('#btnIgnore');//忽略
            this.updateTitle = $('.logo .title');//更新提示的标题
            this.btnUpdate = $('#btnUpdate');//立即更新
            this.btnDownSelf = $('#btnDownSelf');//立即更新
            this.btnSkip = $('#btnSkip');//以后再说
            this.btnRetry = $('#btnRetry');//重试
            this.btnUpdate2 = $('#btnUpdate2');//提示框的立即更新
            this.btnForget = $("#btnForget");//放弃更新
            this.iptUpdateNotes = $('textarea.tips');//更新说明
            this.forceUpdate = this.getParam();//是否是强制更新
            uc.util.LogUtil.info('Update', 'InitUI', 'getUrlParam', this.forceUpdate);

            this.iconContainer = $('.tool-icon-container');
            this.minIcon = $('.icon-min', this.iconContainer);
            this.closeIcon = $('.icon-close', this.iconContainer);
            this.initUI();
            this.bindEvent();
        },
        initUI: function () {
            //var _param = uc.util.StorageUtil.getData('updateForce');
            if (this.forceUpdate) {
                this.initForceUpdate();
                this.getUpdateNotes();
            } else {
                this.initUpdate();
            }
//            var version = uc.StorageUtil.getData('version');
//            var msg = uc.StorageUtil.getData('msg');
//            $('#version').text(version);
//            $('#msg').text(msg);
            //uc.modules.component.TitleBarButton.init(uc.constants.UI.WindowCloseBehavior.Quit);
        },
        getUpdateNotes: function () {
            uc.modules.update.UpdateManager.init();
        },
        initUpdateNotes: function (version, notes) {
            this.setUpdateNotes(notes);
        },
        setUpdateNotes: function (notes) {
            this.iptUpdateNotes.html(this.handleUpdateNotes(notes)).removeClass('center');
            this.btnUpdate.removeClass('disabled').html(uc_resource.Update.Update);
            this.btnDownSelf.addClass('hide');
        },
        handleUpdateNotes: function (notes) {
            var _notes = '';
            if (notes) {
                for (var i = 0; i < notes.length; i++) {
                    _notes += notes[i] + '\r\n';
                }
            }
            return _notes;
        },
        /*  加载强制更新界面  */
        initForceUpdate: function () {
            this.closeIcon.click(function () {
                $('.update-welcome').hide();
                $('.update-main').addClass('tips');
                $('.update-force').show();

                $(this).unbind().click(function () {
                    uc.quit();
                });
            });

            this.setForceUpdateTitle();
        },
        setForceUpdateTitle: function () {
            _this.updateTitle.html(uc_resource.Update.Title);
        },
        setUpdateTitle: function () {
            _this.updateTitle.html(uc_resource.Update.NewVersionTitle);
        },
        /*  加载正常更新界面  */
        initUpdate: function () {
            $('#btnIgnore').css('display', 'inline-block');
            this.closeIcon.click(function () {
                _this.close();
            });

            this.setUpdateTitle();
            var _notes = uc.util.StorageUtil.getData('checkUpdates');
            this.setUpdateNotes(JSON.parse(_notes).notes);
        },
        close: function () {
            uc.util.WindowUtil.close();
        },
        getParam: function () {
            var url = location.href;
            return url.indexOf('?') >= 0 ? url.substring(url.indexOf('type=') + 5, url.length) : 0;
        },
        bindEvent: function () {
            //最小化
            this.minIcon.click(function(){
                uc.util.WindowUtil.minimize();
                uc.IUIService.minimizeWindow();
            });

            //忽略
            this.btnIgnore.on("click", function(){
                _this.close();
            });

            //立即更新
            this.btnUpdate.on("click", function(){
                if ($(this).hasClass('disabled')) {
                    return;
                }
                $('.update-main .icon-close').hide();//隐藏关闭按钮
                $('.update-main .update-welcome').hide();//隐藏欢迎页
                uc.util.WindowUtil.hideAll();
                uc.modules.update.UpdateManager.update();
            });

            //以后再说
            this.btnSkip.on('click', function () {
                _this.btnIgnore.trigger('click');
            });

            //重试
            this.btnRetry.on('click', function () {
                $('.update-failed').hide();
                _this.btnUpdate.trigger('click');
            });

            //弹出框的立即更新
            this.btnUpdate2.on('click', function () {
                _this.btnUpdate.trigger('click');
            });

            //强制更新-->放弃更新
            this.btnForget.on("click", function(){
                uc.quit();
            });

            uc.IUIService.bindCrossWindowEvent('OnClientUpdate', function(){
                uc.util.WindowUtil.hide();
                uc.IUIService.minimizeWindow();
            });
        },
        checkUpdateFailed: function (downloadUrl) {
            if (!this.forceUpdate) {
                return;
            }
            this.closeIcon.unbind().click(function () {
                uc.quit();
            });

            this.btnUpdate.unbind().click(function () {
                uc.quit();
            });

            this.btnUpdate.removeClass('disabled').html(uc_resource.Update.UpdateFail);
            this.showUpdateFailedResult(downloadUrl);
        },
        showUpdateFailedResult: function (downloadUrl) {
            if (!downloadUrl) {
                this.iptUpdateNotes.html(uc_resource.SettingUpdate.UpdateFailed).addClass('center');
                return;
            }
            this.iptUpdateNotes.html(uc_resource.Update.UpdateFailTips).addClass('center');
            this.btnDownSelf.attr('href', downloadUrl).removeClass('hide');
        }
    };
    jQuery(document).ready(function () {
        uc.init();
        uc.modules.update.Update.init();
    })
})(uc);