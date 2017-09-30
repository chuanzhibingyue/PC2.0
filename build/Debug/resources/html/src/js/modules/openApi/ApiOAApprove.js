/**
 * Created by zhen.xu.
 * User: zhen.xu
 * Date: 16-1-4 下午2:44
 */
(function (uc) {
    var self;
    uc.modules.openApi.OAApprove = {
        init: function(){
            self = this;
            this.options = uc.util.StorageUtil.getData("OAApprove");//suggestionTitle
            this.suggestionTitle = this.options.suggestionTitle ? this.options.suggestionTitle : uc_resource.OpenApi.ApproveTitle;
            this.textArea = $('.content textarea');
            this.count = $('.content .count>span');
            this.btnOk = $('.btn-OAApprove .btn-ok');
            this.windowTitle = $('.apimsg-preview .title');
            this.bindEvent();
            this.setTitle();
        },
        setTitle: function () {
            self.windowTitle.html(self.suggestionTitle);
        },
        bindEvent: function(){
            $('.icon-close, .btn-close').on('click', function(e){
                uc.IUIService.triggerCrossWindowEvent(self.options.key, { btnCancel: true});
                uc.util.WindowUtil.close();
            });

            self.btnOk.on('click', function(e){
                var text = self.textArea.val();
                var len = $.trim(text).length;
                if (self.options.noEmpty && !$.trim(text)) {
                    uc.modules.component.PromptMessageBox.open({title: uc_resource.Common.Prompt, message: uc_resource.OpenApi.ApproveNoEmpty, buttonMode:"ok", OkText: uc_resource.Common.Known},
                        function(result){
                            this.close();
                        });
                    return;
                }

                if (len > uc.constants.Api.OAApproveMaxLen) {
                    uc.modules.component.PromptMessageBox.open({title: uc_resource.Common.Prompt, message: uc_resource.OpenApi.ApproveOverMax, buttonMode:"ok", OkText: uc_resource.Common.Known},
                        function(result){
                            this.close();
                        });
                    return;
                }
                uc.IUIService.triggerCrossWindowEvent(self.options.key, { okOrCancel: true, data: text, params: self.options.params});
                uc.util.WindowUtil.close();
            });

            self.textArea.on('keyup', function () {
                var content = $.trim($(this).val());
                self.count.text(content.length);
                if (content.length > uc.constants.Api.OAApproveMaxLen) {
                    self.count.addClass('over');
                } else {
                    self.count.removeClass('over');
                }
            });
        }
    };

    jQuery(document).ready(function () {
        uc.init();
        uc.modules.openApi.OAApprove.init();
    });
})(uc);