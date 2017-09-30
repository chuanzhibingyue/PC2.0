/**
 * Created by zuo.dai on 2015/10/10.
 */
(function(uc){
    var _this = null;
    uc.modules.openApi.ApiNoticePreview = {
        init: function(){
            _this = this;
            _this.domPage = $(".apimsg-preview");
            _this.sizeIconContainer = $('.tool-icon-container');
            this.bindEvent();
            var sendObj = uc.util.StorageUtil.getData("ShowApiNoticePreview");
            $(".title").text(sendObj.noticeAccountName);
            var spliter = sendObj.url.indexOf("?")>-1?"&":"?";
		        sendObj.url = sendObj.url + spliter + 'uc_account=' + sendObj.account + '&uc_token=' + sendObj.token;


			// added by xue.bai_2 on 2016/02/24
            _this.modifyFrameContent(sendObj);
        },
        bindEvent: function(){
            uc.IUIService.bindCrossWindowEvent("noticeAlert", function (data) {
                uc.modules.component.PromptMessageBox.open({title: uc_resource.MessageBox.Notification, message: uc_resource.Cloud.DownloadingPrompt, buttonMode:"ok",OkText:uc_resource.MessageBox.OK},
                    function(result){
                        this.close();
                    });
                return false;
            });
            uc.util.EventUtil.bindNativeEvent(ucClient.Window.Status.MAXIMIZED_FROM_NORMAL,function(){
                _this.onWindowMaxed(_this.sizeIconContainer.find(".icon-max")[0]);
            });
            uc.util.EventUtil.bindNativeEvent(ucClient.Window.Status.RESTORED_FROM_MAXIMIZED,function(){
                _this.onWindowRestored(_this.sizeIconContainer.find(".icon-restore")[0]);
            });

            _this.sizeIconContainer.on('click',
                function(e){
                    target = e.target;
                    e.stopPropagation();
                    e.preventDefault();
                    if($(target).hasClass("icon-close")){
                      uc.util.WindowUtil.close();
                    }else if($(target).hasClass("icon-max")){
                        uc.util.WindowUtil.maximize();
                        _this.onWindowMaxed(target);
                    }else if($(target).hasClass("icon-min")){
                        _this.sizeIconContainer.find(".icon-max").removeClass("icon-max").addClass("icon-restore");
                        _this.domPage.removeClass("max");
                        _this.domPage.addClass("min");
                        uc.util.WindowUtil.setCaptions([{left: 5,top: 5,width: 80, height: 20}]);
                    }else if($(target).hasClass("icon-restore")){
                        uc.util.WindowUtil.restore();
                        _this.onWindowRestored(target);
                    }
                }
            );
		},
		modifyFrameContent: function(sendObj){
            window.previewImg = function(src, status){
                if(!src || status != "open") {
                    return;
                }

                uc.IUIService.previewImage(src);
            }

            $.get(sendObj.url, function(data){
                var frame = document.querySelector("#iframe");

                frame.setAttribute("uc_token", sendObj.token);
                frame.setAttribute("uc_account", sendObj.account);

                var imgAction = '<' + 'script>' +
                    //暂时移除图片预览（因为公告的图片并非都来自fs server）
                    // 'var imgs = document.querySelectorAll("img");' +
                    // 'for(var i=0,len=imgs.length;i<len;i++){' +
                    // 'var img = imgs[i];' +
                    // 'img.setAttribute("status", "open");' +
                    // 'img.setAttribute("style", "cursor:pointer");' +
                    // 'img.addEventListener("click", function(){top.window.previewImg(this.src,this.getAttribute("status"))}, false);' +
                    // '}' +
                    '</' + 'script>';

                var x = data.split('</body>');
                x[2] = '</body>' + x[1];
                x[1] = imgAction;

                var page = x.join("");
                frame.contentDocument.write(page);

                var iframe= $('.iframe');

                iframe.removeClass("hide");
                iframe.contents().find("a").click(function(){
                    var url=$(this).attr("href");
                    if(url.indexOf("cloudFileDownload")!=-1){
                        var strInfo=uc.util.StringUtil.getParameterByName("data",url);
                        uc.IUIService.triggerCrossWindowEvent("noticeCloudInfoDownload",{"data":strInfo});
                        return false;
                    }
                    return true;
                });
            });
        },
        onWindowMaxed:function(target){
            $(target).removeClass("icon-max");
            $(target).addClass("icon-restore");
            _this.domPage.removeClass("min");
            _this.domPage.addClass("max");
            uc.util.WindowUtil.setCaptions(uc.constants.UI.DragCaption.ApiNoticePreview);
        },
        onWindowRestored:function(target){
            $(target).removeClass("icon-restore");
            $(target).addClass("icon-max");
            _this.domPage.removeClass("min");
            _this.domPage.removeClass("max");
            uc.util.WindowUtil.setCaptions(uc.constants.UI.DragCaption.ApiNoticePreview);
        }
    };
    jQuery(document).ready(function(){
        uc.init();
        uc.modules.openApi.ApiNoticePreview.init();
    });
})(uc);