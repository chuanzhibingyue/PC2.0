/**
 * Created by zuo.dai on 2015/8/19.
 */
(function(uc){
    uc.modules.code.send_data = {};
    uc.modules.code.CodePreviewEventListener = function () {

    };
    jQuery.extend(uc.modules.code.CodePreviewEventListener.prototype,
        uc.interfaces.IIMEvent,
        uc.interfaces.IFSEvent,
        uc.interfaces.IGroupEvent, {
            //FS
            OnFileDownloaded: function(errorCode, taskId, savePath){
                console.log(errorCode, taskId, uc.modules.code.CodePreview.currTaskId);
                if (taskId != uc.modules.code.CodePreview.currTaskId) {
                    return;
                }
                uc.modules.code.CodePreview.OnCodeDownloaded(errorCode, savePath);

            }
        }
    );

    var _this = null;
    uc.modules.code.CodePreview = {
        init: function(){
            _this = this;
            this.bindEvent();

            //下载
            var obj = uc.util.StorageUtil.getData("ShowCodePreview");
            if (obj == null || obj.url == null) {
                return;
            }
            console.log(obj);
           // obj.name,obj. size,obj.type
            $('.name').text(obj.name);
            var ext = "code";
            $('.file-ext').text(ext);
            $('.type').text(obj.type);
            $('.size').text(uc.util.StringUtil.formatFileSize(Number(obj.size)));

            var savePath = uc.IClientService.getDataDirectory(uc.constants.UI.DirectoryType.File) + "\\preview_code_" + new Date().getTime() + ".html";
            console.log(savePath);
            uc.IFSService.downloadFile(obj.url, savePath,
                function (args) {
                    if (args[0] != 0 || args[1] == null) {
                        console.log(args);
                        return;
                    }
                    var taskId = args[1];
                    _this.currTaskId = taskId;
                });

        },
        bindEvent: function(){
            $(document).keyup(function(event){
                switch(event.keyCode) {
                    case 27:
                        uc.util.WindowUtil.close();
                }
            });
            $('.icon-close').on('click',
                function(e){
                    uc.util.WindowUtil.close();
                }
            );

            try {
                var iframeDoc = $('.code-frame')[0].contentWindow.document;
                iframeDoc.oncontextmenu=
                    function(e) {
                        console.log("test");
                        e.preventDefault();
                };
            }catch(ex){
                console.log(ex);
            }
        },
        OnCodeDownloaded: function(errorCode, path) {
            if (errorCode == 0) {
                $('.code-frame').attr("src", path);
                  try {
                      $('.code-frame')[0].onload = function() {
                          $('.code-frame')[0].contentWindow.document.oncontextmenu=
                              function(e) {
                                  console.log("test");
                                  e.preventDefault();
                              };
                      }
                  }catch(ex) {
                      console.log(ex);
                  }
            }
        }
    };
    jQuery(document).ready(function(){
        uc.init();
        uc.modules.code.CodePreview.init();
    })
})(uc);