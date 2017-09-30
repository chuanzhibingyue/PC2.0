(function (uc) {
    uc.modules.component.HtmlEditor = function (container, moduleId) {
        if (!container) {
            return;
        }

        var $container, me = this, $iframe, iframeDoc, atId = null, $menu;
        if (container.jquery) {
            $container = container;
        }
        else {
            $container = $("#" + container);
        }

        _init();

        function _init() {
            $menu = $('<div class="edit-menu" style="display: none;">'
                +       '<div class="menu-item" id="e-cut">'+ uc_resource.Common.Cut +'</div>'
                +       '<div class="menu-item" id="e-copy">'+ uc_resource.Common.Copy +'</div>'
                +       '<div class="menu-item" id="e-paste">'+ uc_resource.Common.Paste +'</div>'
                +   '</div>').appendTo($container);

            var parentNode = moduleId == uc.constants.UI.ModuleId.Message ? uc.ui.Message.MainPanel : uc.ui.Meeting.MainPanel;
            $iframe = $('<iframe class="html-editor visible" hidefocus="true" frameborder="0"></iframe>').appendTo(parentNode);

            iframeDoc = $iframe[0].contentWindow.document;

            iframeDoc.open();
            iframeDoc.write(_getDocHtml());
            iframeDoc.close();

            $(window).resize(function(){
                _setIframePosition();
            });

            //iframeDoc.designMode = 'on';

            iframeDoc.onload = function(){
                iframeDoc.body.focus();
            };
            setTimeout(function () {
                if(iframeDoc && iframeDoc.body){
                    iframeDoc.body.focus();
                }
            }, 0);

            iframeDoc.onclick = function (e) {
                if (atId != null) {
                    atId = null;
                    me.onAt();
                }
                $iframe.click(e);
                $(document.body).trigger("click");
                me.ondblclick(e);
            };

            var _onAtKeyPress =function () {
                var id = "sp_" + new Date().getTime();
                iframeDoc.execCommand("insertHTML",false, "<span class='atSpan' id='" + id + "'>@</span>");
                var obj = $("#" + id, iframeDoc.body);
                var left = parseInt($iframe.offset().left + obj.offset().left);
                var top = parseInt($iframe.offset().top + obj.offset().top);
                atId = id;
                me.onAt(left, top, id);
            };

            iframeDoc.onkeydown = function (e) {
                var text = iframeDoc.body.innerHTML;
                if (text.length >= 5000 &&
                    e.which != 38 &&
                    e.which != 40 &&
                    e.which != 37 &&
                    e.which != 39 &&
                    e.which != 13 &&
                    e.which != 8 ||
                    e.which == 45
                ) {
                    e.preventDefault();
                    return;
                }

                if (e.shiftKey && (e.which == 50 /*|| e.which == 229*/)) { //@功能
                    _onAtKeyPress();
                    e.preventDefault();
                    return;
                }
                if (atId != null && (
                        e.which == 38 ||
                        e.which == 40 ||
                        e.which == 37 ||
                        e.which == 39 ||
                        e.which == 13 ||
                        e.which == 27
                    )
                ) { //@弹出菜单后搜索,(上下左右箭头和回车)
                    if (e.which == 38) { //上
                        me.onAtsearch(atId, "@up");
                        e.preventDefault();
                        return;
                    } else if (e.which == 40) { //下
                        me.onAtsearch(atId, "@down");
                        e.preventDefault();
                        return;
                    } else if (e.which == 13) { //回车
                        if (me.onAtsearch(atId, "@enter")) {
                            e.preventDefault();
                            return;
                        }
                    } else if (e.which == 27)  { //esc
                        atId = null;
                        me.onAt();
                        e.preventDefault();
                        return;
                    }
                }

                if (me.onkeydown) {
                    var re = me.onkeydown(e);
                    if (e.which == 13 && !re) {
                        e.preventDefault();
                        iframeDoc.execCommand("insertLineBreak", false, null);
                    }
                }
            };
            iframeDoc.onkeypress = function (e) {
                if (e.shiftKey && (e.which == 64)) { //@功能
                    _onAtKeyPress();
                    e.preventDefault();
                }
            };

            iframeDoc.onkeyup = function (e) {
                if (atId != null) { //@弹出菜单后搜索
                    var obj = $("#" + atId, iframeDoc.body);
                    if (obj.length <= 0) {
                        atId = null;
                        me.onAt();
                        return;
                    }

                    if (atId != null && (
                        e.which == 38 ||
                        e.which == 40 ||
                        e.which == 37 ||
                        e.which == 39 ||
                        e.which == 27)) {
                        return;
                    }

                    var text = obj.text();
                    var text = text.substr(1);
                    me.onAtsearch(atId, text);
                }

                if (e.which == 8 || e.which == 46 ) {
                    $('span', iframeDoc.body).each(function () {
                        var text = $.trim($(this).text());
                        var _atId = $(this).prev('.atSpan').attr('id');
                        atId = _atId;
                        if (!atId) {
                            me.onAt();
                        }

                        if (!text) {
                            $(this).remove();
                        }
                    });

                    var chartId = uc.modules.chat.ChatManager.currentChatId;
                    var chartType = uc.modules.chat.ChatManager.currentChatType;

                    if (!iframeDoc.body.innerHTML) {
                        $('.atMembers').hide();
                        uc.modules.draft.DraftMessageManager.removeDraftMessage(chartId, chartType);
                    }
                }
            };

            iframeDoc.oninput = function (e) {
                $('.atSpan', iframeDoc.body).each( //@删除不完整的人名
                    function() {
                        var name = $(this).attr("at-name");
                        var text = $(this).text();
                        if (name == null || name == "") {
                            return;
                        }
                        if (text.indexOf(name) < 0) {
                            $(this).remove();
                        }
                    }
                );
            };

            iframeDoc.onpaste = function (e) {
                if (!_tryClipboardImageOrFile()) {
                    if (!_setClipboardText(e.clipboardData)) {
                        return;
                    }
                }

                e.preventDefault();
            };

            iframeDoc.oncopy = function (e) {
                if (iframeDoc.getSelection().toString() != "") {
                    return;
                }
                var range= iframeDoc.getSelection().getRangeAt(0);
                var container = document.createElement('div');
                container.appendChild(range.cloneContents());
                var imgs  = $("img", $(container));
                if (imgs.length == 1) {
                    uc.util.SystemUtil.setClipboardImageData(imgs.attr("src"));
                    e.preventDefault();
                }
            };

            iframeDoc.ondragover = function (e) {
                e.preventDefault();
            };
            iframeDoc.ondrop = function (e) {
                ucClient.Window.getDroppedFiles(
                    function (args) {
                        if (args[0] != 0) {
                            return;
                        }
                        var files = args[1];
                        $.each(files,function(n,data){
                            if(data.attribute==0){
                                var path=[data.path];
                                me.handleClipboardFile(path);
                            }
                        });
                        //me.handleClipboardFile(files);
                        //for (var i = 0; i < files.length; i++) {
                        //    me.handleClipboardFile(files[i]);
                        //}
                    }
                );
                e.preventDefault();
            };

            iframeDoc.body.oninput = function(){
                me.onInputCallback();
            };

            iframeDoc.oncontextmenu=
                function(e) {
                    var range= iframeDoc.getSelection().getRangeAt(0);
                    var container = document.createElement('div');
                    container.appendChild(range.cloneContents());
                    if (container.innerHTML == "") {
                        $("#e-copy", $menu).addClass("disabled");
                        $("#e-cut", $menu).addClass("disabled");
                    } else {
                        $("#e-copy", $menu).removeClass("disabled");
                        $("#e-cut", $menu).removeClass("disabled");
                    }

                    $menu.show();
                    var top = $iframe.offset().top + e.pageY;
                    var left = $iframe.offset().left + e.pageX;
                    if (top + $menu.height() > $iframe.offset().top + $iframe.height() - 10) {
                        top = $iframe.offset().top + $iframe.height() - $menu.height() - 10;
                    }
                    if (left + $menu.width() > $iframe.offset().left + $iframe.width() - 10) {
                        left = $iframe.offset().left + $iframe.width() - $menu.width() - 10;
                    }
                    $menu.css({position: "fixed",'top': top, 'left': left});
                    e.preventDefault();
                };

            $menu.on("click", ".menu-item",
                function(e) {
                    if ($(this).hasClass("disabled")) {
                        return;
                    }
                    var id = $(this).attr("id");
                    if (id == "e-copy") {
                        iframeDoc.execCommand("copy");
                    } else if (id == "e-cut") {
                        iframeDoc.execCommand("cut");
                    } else if (id == "e-paste") {
                        iframeDoc.execCommand("paste");
                    }
                    $menu.hide();
                    e.preventDefault();
                    e.stopPropagation();
                    iframeDoc.body.focus();
                }
            );

            uc.util.CaretUtil.setIframeEditorCaretPosition(iframeDoc.body, $iframe[0]);

            $("body").click(function (e) {
                if ($menu != null) {
                    $menu.hide();
                }
            });

            //字体配置
            if (uc.settings.textMsgStyle != null) {
                $(iframeDoc.body).css({
                    "font-size":  "" + uc.settings.textMsgStyle.fontSize + "px",
                    "color":  "" + uc.settings.textMsgStyle.fontColor,
                    "font-weight":   uc.settings.textMsgStyle.bold ? "bold" : "",
                    "font-style": uc.settings.textMsgStyle.italic ? "italic" : "",
                    "text-decoration": uc.settings.textMsgStyle.underline ? "underline" : ""
                });
            }
        }

        function _getDocHtml() {
            var arr = [
		        '<html>',
		        '<head><meta charset="utf-8" /><title></title>',
		        '<style>',
		        'html {margin:0;padding:0;}',
		        'body {margin:0;padding:0;background-color: #f8f8f8;}',
		        '* { font-size: 15px; font-family: "微软雅黑", "helvetica"; word-wrap: break-word; color: #03111f; }',
		        'p {margin:0 0;}',
		        'img {max-width: 200px; max-height: 200px; cursor: pointer;}',
                '.emotion {width:20px;height:20px;}',
		        'table {border-collapse:collapse;}',
		        'img {border:0;}',
		        'noscript {display:none;}',
                'span {font-size: inherit;}',
		        '</style>'];

            arr.push('</head><body></body></html>');
            return arr.join('\n');
        }

        function _getContent(filterTags, exceptImg) {
           // var tags = _getFilterTags(filterTags, exceptImg);
            var content = iframeDoc.body.innerHTML;
            content = content.replace(/<br>/g, "\n");
            //content = uc.util.HtmlUtil.formatHtml(content, tags);
            //content = content.replace(/&nbsp;/g, " ");
            return content;
        }

        function _setContent(content, draftSource) {
            //var tags = _getFilterTags(filterTags, exceptImg);
            setTimeout(function () {
                if(draftSource == uc.constants.UI.DraftSource.DraftDataSet){
                    iframeDoc.body.focus();
                }

                iframeDoc.body.innerHTML = '';
                //var htmlContent = uc.util.HtmlUtil.formatHtml(content, tags);
                //htmlContent = htmlContent.replace(/\n/g, "<br/>");
                //iframeDoc.execCommand("insertHTML",false, htmlContent);
                iframeDoc.execCommand("insertHTML",false, content);
                iframeDoc.body.scrollTop = iframeDoc.body.scrollHeight;
            }, 100);
        }
/*
        function _getFilterTags(filterTags, exceptImg) {
            filterTags = filterTags === false ? false : true;
            exceptImg = exceptImg === false ? false : true;

            var tags = filterTags && exceptImg ? null : {};

            return tags;
        }
*/
        function _tryClipboardImageOrFile() {
            try{
                var data = uc.util.SystemUtil.getClipboardData();

                if (data.type == uc.constants.UI.ClipboardDataType.Image) {
                    //超级群，不允许发图片
                    if (moduleId == uc.constants.UI.ModuleId.Message && uc.modules.chat.ChatManager.groupInfo && uc.modules.chat.ChatManager.groupInfo.memberType) {
                        return false;
                    }
                    if (data.content) {
                        me.insertImage(data.content);
                    }

                    return true;
                }
                else if (data.type == uc.constants.UI.ClipboardDataType.File) {
                    if (data.content) {
                        me.handleClipboardFile(data.content);
                    }

                    return true;
                }
            } catch(e){

            }


            return false;
        }

        function _setClipboardText(clipboardData) {
            var data = clipboardData.getData("text");
            if (data == null || data.length <= 0) {
                return false;
            }

            var text = iframeDoc.body.innerHTML;
            var l = 5000 - text.length;
            if (l <= 0) {
                return true;
            }
            if (data.length > l) {
                data = data.substr(0, l);
            }

            data = data.replace(/&/g, "&amp;");
            data = data.replace(/</g, "&lt;");
            data = data.replace(/>/g, "&gt;");
            data = data.replace(/ /g, "&nbsp;");
            data = data.replace(/\'/g, "&#39;");
            data = data.replace(/\"/g, "&quot;");
            data = data.replace(/\r?\n/g, "<br>");
            data = uc.util.EmotionUtil.colonToEmotion(data);
            data = uc.modules.emoji.EmojiConverter.codeColonToSmiley(data);
            setTimeout(function(){
                iframeDoc.execCommand("insertHTML",false, data);
                iframeDoc.body.scrollTop = iframeDoc.body.scrollHeight;
            }, 1);
            return true;
        }

        function _setAtMember(id, userId, userName, draftId){
            atId = null;
            if (id == null || userId == null) {
                return;
            }
            var obj = $("#" + id, iframeDoc.body);
            obj.remove();
            //obj.text();
            //obj.attr("at-id", userId);
            setTimeout(function () {
                iframeDoc.body.focus();
                iframeDoc.execCommand("insertHTML",false, "<span class='atSpan' draftid='"+ draftId +"'  at-name='" + userName + "' at-id='" + userId + "'  id='" + id + "'>@" + userName + "</span><span>&nbsp;</span>");
            }, 2);
        }

        function _setIframePosition(){
            setTimeout(function(){

                // To solve the input problem when resized.
                // If switch the page (eg. message to contacts),the last page has add class "hide".
                // It will make this node's width and height equals zero.
                // So must remove class "hide" before resize, and add class "hide" when resized.
                var width = 0, height = 0, contentPosition;

                if ($(".menu-panel-body.message").hasClass("hide")) {
                    $(".menu-panel-body.message").removeClass("hide");
                    width = $container.width();
                    height = $container.height();
                    contentPosition = $container.offset();

                    $iframe.css('position', 'absolute');
                    $iframe.offset({
                        top: contentPosition.top,
                        left: contentPosition.left
                    });
                    $iframe.width(width);
                    $iframe.height(height);

                    $(".menu-panel-body.message").addClass("hide");
                } else {
                    contentPosition = $container.offset();

                    $iframe.css('position', 'absolute');
                    $iframe.offset({
                        top: contentPosition.top,
                        left: contentPosition.left
                    });
                    $iframe.width($container.width());
                    $iframe.height($container.height());
                }
                
                $iframe.removeClass('visible');
            }, 500);
        }

        this.insertImage = function (path, width, height, emotionCode) {
            if (width != null && height != null) {
                iframeDoc.execCommand("insertHTML", false, '<img width="' + width + '" height="' + height + '" src="' + path +'" alt=":' + emotionCode + ':">');
            } else {
                setTimeout(function() {
                    iframeDoc.execCommand("insertImage", false, path);
                }, 10);
            }
            setTimeout(function() {
                iframeDoc.body.scrollTop = iframeDoc.body.scrollHeight;
                iframeDoc.body.focus();
            }, 50);
        };

        this.getContent = function (filterTags, exceptImg) {
            return _getContent(filterTags, exceptImg);
        };

        this.setContent = function (content, draftSource) {
            _setContent(content, draftSource);
        };

        this.handleClipboardFile = function (filePath) {
        };

        this.clearContent = function () {
            iframeDoc.body.innerHTML = "";
        };

        this.clear = function () {
            $iframe.remove();
        };

        this.enable = function(bEnabled) {
            //iframeDoc.body.contenteditable = bEnabled ? 'true' : 'false';
            setTimeout(function () {
                $(iframeDoc).find('body').attr('contenteditable', bEnabled);
            }, 20);
        };
        this.onkeydown = function (e) {
        };

        this.ondblclick = function (e) {
        };

        this.onAt = function(x, y, id) {
        };
        this.onAtsearch = function(atId, text){
            return false;
        };

        this.setAtMember = function(atId, userId, userName, draftId) {
              _setAtMember(atId, userId, userName, draftId);
        };

        this.getHtmlContent = function(){
            return iframeDoc.body.innerHTML;
        };

        this.setOnInput = function(callback){
            this.onInputCallback = callback;
        };
        this.hide = function(){
            $iframe.hide();
        };
        this.setIframePosition = function(){
            _setIframePosition();
        };

        this.focusInput = function(){
            setTimeout(function(){
                if(iframeDoc && iframeDoc.body){
                    iframeDoc.body.focus();
                }
            }, 1000);
        };

        this.reset = function(){
            _setIframePosition();
            $iframe.show();

            this.focusInput();
        }
    }
})(uc);