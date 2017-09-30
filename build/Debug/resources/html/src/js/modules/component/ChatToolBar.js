/**
* Created by senlin.ouyang on 2015/7/21.
*/
(function (uc) {
    uc.modules.component.ChatToolBar = function (container) {
        if (!container) {
            return;
        }

        var $container, me = this, $toolbar, $emotionPanel, $screenShotSetting, $textPanel;
        if (container.jquery) {
            $container = container;
        }
        else {
            $container = $("#" + container);
        }

        _init();
        _bindEvent();


        function _init() {
            $toolbar = $("<div class='chat-toolbar'/>");

            $("<a href='javascript:void(0)' action='textFormat' class='textFormat' qtips-w='40' qtips='"+ uc_resource.Chat.Font +"'>textFormat</a>").appendTo($toolbar);
            $("<a href='javascript:void(0)' action='emotions' class='emotions' qtips-w='40' qtips="+ uc_resource.Chat.Emotion +">emotions</a>").appendTo($toolbar);
            $("<a href='javascript:void(0)' action='image' class='image' qtips-w='40'  qtips="+ uc_resource.Chat.Image +">image</a>").appendTo($toolbar);
            $("<a href='javascript:void(0)' action='file' class='file' qtips-w='40'  qtips="+ uc_resource.Chat.File +">file</a>").appendTo($toolbar);
            $("<a href='javascript:void(0)' action='code' class='code' qtips="+ uc_resource.Chat.Code +">code</a>").appendTo($toolbar);
            $("<a href='javascript:void(0)' action='screenshot' class='screen-shot' qtips-w='40'  qtips="+ uc_resource.Chat.ScreenShot +">screen shot</a>").appendTo($toolbar);
            $("<a href='javascript:void(0)' action='screenshotsetting' class='screen-shot-setting'>screen shot</a>").appendTo($toolbar);
            $("<a href='javascript:void(0)'  onclick ='uc.modules.historymsg.HistoryManager.createHistoryPage(this)' action='history' class='history' qtips="+ uc_resource.Chat.MessageRecord +">history</a>").appendTo($toolbar);
            $("<a href='javascript:void(0)' action='cloud' class='cloud' qtips="+ uc_resource.Chat.Cloud +">cloud</a>").appendTo($toolbar);
	        $("<a href='javascript:void(0)' action='uploadMeetingDoc' class='uploadMeetingDoc' qtips="+ uc_resource.Chat.uploadMeetingDoc +">uploadMeetingDoc</a>").appendTo($toolbar);
            $toolbar.appendTo($container);

            $screenShotSetting = $("<div class='screen-shot-settings'>"
            + "<div class='setting-menu shoot-key'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+ uc_resource.Chat.ScreenTips +"</div>"
            + "<div class='setting-menu hide-window'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+ uc_resource.Chat.ScreenHide +"</div>"
            + "</div>");

            $screenShotSetting.insertAfter($('.screen-shot-setting', $toolbar));

            $textPanel = $("<div class='text-format-panel' style='display:none;'>"
                + "<div class='font-size'>14pt</div><div class='font-sizes' style='display: none'><div class='fs'>24</div><div class='fs'>20</div><div class='fs'>18</div><div class='fs'>16</div><div class='fs'>14</div><div class='fs'>12</div></div>"
                + "<div class='font-color' style='display: none'><div class='sel-color'></div></div> <div class='colors' style='display: none'></div>"
                + "<div class='font-bold'>B</div>"
                + "<div class='font-italic'>I</div>"
                + "<div class='font-underline'>U</div>"
                + "</div>").appendTo($toolbar);

            var fontColors = ["#3b4f61", "#000000", "#ff0000", "#00ff00", "#0000ff", "#ffcc00", "#ffff00", "#00ffff", "#ff00ff"];
            for (var i = 0; i < fontColors.length; i++) {
                $(".colors", $textPanel).append("<div class='color' style='background-color: " + fontColors[i] + "' clr=" + fontColors[i] + "></div>");
            }
        }

        function _showScreenShotSetting($target) {
            var position = $target.position();
            $screenShotSetting.css({ left: (position.left - 5) + "px", top: (position.top + $target.height()) + "px" });

            //TODO: get globe settings and show menu
            if (uc.settings.skScreenshot.hideWindow) {
                $('.hide-window', $screenShotSetting).html("<em class='it-right'></em>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+ uc_resource.Chat.ScreenHide);
                $('.hide-window', $screenShotSetting).attr("checked",  true);
            } else {
                $('.hide-window', $screenShotSetting).html("&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+ uc_resource.Chat.ScreenHide);
                $('.hide-window', $screenShotSetting).attr("checked", false);
            }
            var getKeyStr =  function(keyOption){
                var keyboradMap = {
                    "48":"0",
                    "49":"1",
                    "50":"2",
                    "51":"3",
                    "52":"4",
                    "53":"5",
                    "54":"6",
                    "55":"7",
                    "56":"8",
                    "57":"9",
                    "65":"A",
                    "66":"B",
                    "67":"C",
                    "68":"D",
                    "69":"E",
                    "70":"F",
                    "71":"G",
                    "72":"H",
                    "73":"I",
                    "74":"J",
                    "75":"K",
                    "76":"L",
                    "77":"M",
                    "78":"N",
                    "79":"O",
                    "80":"P",
                    "81":"Q",
                    "82":"R",
                    "83":"S",
                    "84":"T",
                    "85":"U",
                    "86":"V",
                    "87":"W",
                    "88":"X",
                    "89":"Y",
                    "90":"Z",
                    "112":"F1",
                    "113":"F2",
                    "114":"F3",
                    "115":"F4",
                    "116":"F5",
                    "117":"F6",
                    "118":"F7",
                    "119":"F8",
                    "120":"F9",
                    "121":"F10",
                    "122":"F11",
                    "123":"F12"
                };
                var keyStr = keyboradMap["" + keyOption.keyCode];
                if (keyStr == null) {
                    keyOption.keyCode = null;
                    keyStr = "";
                }
                var str = "";
                str +=  keyOption.ctrlKey ?  "Ctrl" : "";
                if (keyOption.altKey) {
                    str += str.length > 0 ? " + Alt" : "Alt";
                }
                if (keyOption.shiftKey) {
                    str += str.length > 0 ? " + Shift" : "Shift";
                }
                if (keyOption.keyCode != 16 && keyOption.keyCode != 17 && keyOption.keyCode != 18) {
                    str += str.length > 0 ? " + " + keyStr : "" + keyStr;
                }
                return str;
            };
            var keyOption = uc.settings.skScreenshot;
            var str = getKeyStr(keyOption);
            str = str.replace(/ /g, "");
            $('.shoot-key', $screenShotSetting).html("&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+ uc_resource.Setting.ScreenShots + str);
            $screenShotSetting.show();
        }

        function _isHideWindow() {
            return uc.settings.skScreenshot.hideWindow;
        }

        function _applyFontStyle () {
            uc.modules.setting.SettingManager.saveSetting();

            $(".html-editor").each(
                function() {
                    $(this.contentWindow.document.body).css({
                        "font-size":  "" + uc.settings.textMsgStyle.fontSize + "px",
//                        "color":  "" + uc.settings.textMsgStyle.fontColor,
                        "font-weight":   uc.settings.textMsgStyle.bold ? "bold" : "",
                        "font-style": uc.settings.textMsgStyle.italic ? "italic" : "",
                        "text-decoration": uc.settings.textMsgStyle.underline ? "underline" : ""
                    });
                }
            );
        }

        function _bindEvent() {
            $textPanel.on("click",
                function (e) {
                    e.stopPropagation();
                }
            );

            $textPanel.on("click", ".font-size",
                function () {
                    $(".fs", $(".font-sizes", $textPanel)).each(
                        function() {
                            var size = Number($(this).text());
                            if (uc.settings.textMsgStyle != null && uc.settings.textMsgStyle.fontSize != null) {
                                if (uc.settings.textMsgStyle.fontSize == size) {
                                    $(this).addClass("sel");
                                    return;
                                }
                            }
                            $(this).removeClass("sel");
                        }
                    );
                    $(".font-sizes", $textPanel).show();
                }
            );
            $textPanel.on("click", ".fs",
                function() {
                    $(".font-sizes", $textPanel).hide();
                    var size = Number($(this).text());
                    if (size <= 0) {
                        return;
                    }
                    if (uc.settings.textMsgStyle == null) {
                        uc.settings.textMsgStyle = {};
                    }
                    uc.settings.textMsgStyle.fontSize = size;
                    $(".font-size", $textPanel).text("" + size + "pt");
                    _applyFontStyle();
                }
            );

            $textPanel.on("click", ".font-color",
                function () {
                    $(".colors", $textPanel).show();
                }
            );
            $textPanel.on("click", ".color",
                function() {
                    $(".colors", $textPanel).hide();
                    var color = $(this).attr("clr");
                    if (color == null || color == "") {
                        return;
                    }
                    if (uc.settings.textMsgStyle == null) {
                        uc.settings.textMsgStyle = {};
                    }
                    uc.settings.textMsgStyle.fontColor = color;
                    $(".sel-color", $textPanel).css({"background-color": color});
                    _applyFontStyle();
                }
            );

            $textPanel.on("click", ".font-bold",
                function () {
                    var bold = false;
                    if ($(this).hasClass("sel")) {
                        $(this).removeClass("sel");
                    } else {
                        $(this).addClass("sel");
                        bold = true;
                    }
                    if (uc.settings.textMsgStyle == null) {
                        uc.settings.textMsgStyle = {};
                    }
                    uc.settings.textMsgStyle.bold = bold;
                    _applyFontStyle();
                }
            );
            $textPanel.on("click", ".font-italic",
                function () {
                    var italic = false;
                    if ($(this).hasClass("sel")) {
                        $(this).removeClass("sel");
                    } else {
                        $(this).addClass("sel");
                        italic = true;
                    }
                    if (uc.settings.textMsgStyle == null) {
                        uc.settings.textMsgStyle = {};
                    }
                    uc.settings.textMsgStyle.italic = italic;
                    _applyFontStyle();
                }
            );
            $textPanel.on("click", ".font-underline",
                function () {
                    var underline = false;
                    if ($(this).hasClass("sel")) {
                        $(this).removeClass("sel");
                    } else {
                        $(this).addClass("sel");
                        underline = true;
                    }
                    if (uc.settings.textMsgStyle == null) {
                        uc.settings.textMsgStyle = {};
                    }
                    uc.settings.textMsgStyle.underline = underline;
                    _applyFontStyle();
                }
            );

            $toolbar.on("dragstart", "a", function (e) {
                e.stopPropagation();
                return false;
            });

            $toolbar.on("click", "a[action]", function (e) {
                var $target = $(e.target), action = $target.attr("action");
                $(document.body).trigger("click");
                if (action === "emotions") {
                    if (!$emotionPanel) {
                        $emotionPanel = new uc.modules.component.EmotionPanel($target, $container);

                        $emotionPanel.onSelected = function (sender, args) {
                            me.onSelectedEmotion(me, args);
                        }
                    }

                    $emotionPanel.show();
                    e.stopPropagation();
                }
                else if (action === "textFormat") {
                    if (uc.settings.textMsgStyle != null && uc.settings.textMsgStyle.fontSize != null) {
                        $(".font-size", $textPanel).text("" + uc.settings.textMsgStyle.fontSize + "pt");
                    }
                    if (uc.settings.textMsgStyle != null && uc.settings.textMsgStyle.fontColor != null) {
//                        $(".sel-color", $textPanel).css({"background-color": uc.settings.textMsgStyle.fontColor});
                    }
                    if (uc.settings.textMsgStyle != null && uc.settings.textMsgStyle.bold) {
                        $(".font-bold", $textPanel).addClass("sel");
                    }
                    if (uc.settings.textMsgStyle != null && uc.settings.textMsgStyle.italic) {
                        $(".font-italic", $textPanel).addClass("sel");
                    }
                    if (uc.settings.textMsgStyle != null && uc.settings.textMsgStyle.underline) {
                        $(".font-underline", $textPanel).addClass("sel");
                    }

                    $textPanel.show();
                    e.stopPropagation();
                }
                else if (action === "image") {
                    uc.util.FileDialogUtil.openSelectFileDialog(uc_resource.Chat.OpenImage, [{desc: uc_resource.Chat.OpenImageDesc, filter: uc_resource.Chat.OpenImageTips}], false, function (paths) {
                        if (paths && paths.length > 0) { 
                        	me.onUploadImage(me, {
                                path: paths[0]
                            });
                        }
                    });
                }
                else if (action === "file") {
                    //old code
                    //uc.util.FileDialogUtil.openSelectFileDialog(uc_resource.Chat.OpenFile, [{desc: uc_resource.Chat.OpenFileDesc, filter: '*.*'}], false, function (paths) {
                    //    if (paths && paths.length > 0) {
                    //        me.onUploadFile(me, {
                    //            path: paths[0]
                    //        });
                    //    }
                    //});

                    // added by xue.bai_2 on 2016/01/12
                    uc.util.FileDialogUtil.openSelectFileDialog(uc_resource.Chat.OpenFile, [{desc: uc_resource.Chat.OpenFileDesc, filter: '*.*'}], true, function (paths) {
                        if (paths && paths.length > 0) {
                            $.each(paths, function(index, path){
                                me.onUploadFile(me, {
                                    path: path
                                });
                            });
                        }
                    });
                }
                else if (action === "code") {
                    me.onCodeEdit(me, {});
                }
                else if(action=="cloud"){
                    me.onCloudFile(me,{});
                }
                else if (action === "screenshot") {
                    if (_isHideWindow()) {
                        uc.util.WindowUtil.minimize();
                        uc.IUIService.minimizeWindow();
                    }

                    uc.util.SystemUtil.catchScreen(function (args) {
                        if (args.errorCode == 0 && args.state != 1) {
                            me.onScreenShot(me, {
                                path: args.path
                            });
                        }

                        if (_isHideWindow()) {
                            uc.util.WindowUtil.restore();
                        }
                    });
                }
                else if (action === "screenshotsetting") {
                    _showScreenShotSetting($target);
                    e.stopPropagation();
                }else if(action === "uploadMeetingDoc"){
	                uc.util.FileDialogUtil.openSelectFileDialog(uc_resource.Chat.OpenFile, [{desc: uc_resource.Chat.OpenFileDesc, filter: '*.*'}], true, function (paths) {
		                if (paths && paths.length > 0) {
			                $.each(paths, function(index, path){
				                me.onUploadFile(me, {
					                path: path
				                });
			                });
		                }
	                });
                }
            });

            $('.shoot-key', $container).on("click", function(){
                $screenShotSetting.hide();
                setTimeout(
                    function() {
                        if (_isHideWindow()) {
                            //uc.util.WindowUtil.hide();
                            uc.util.WindowUtil.minimize();
                            uc.IUIService.minimizeWindow();
                        }

                        uc.util.SystemUtil.catchScreen(function (args) {
                            if (args.errorCode == 0 && args.state != 1) {
                                me.onScreenShot(me, {
                                    path: args.path
                                });
                            }
                            uc.util.WindowUtil.show();
                        });
                    },50
                )

            });

            $('.hide-window', $container).on("click", function(){
                var checked = $(this).attr("checked");
                if (!checked) {
                    $(this).html("&nbsp;&nbsp;&nbsp;"+ uc_resource.Chat.ScreenHide);
                    $(this).attr("checked",  true);
                    uc.settings.skScreenshot.hideWindow = true;
                } else {
                    $(this).html("&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"+ uc_resource.Chat.ScreenHide);
                    $(this).attr("checked", false);
                    uc.settings.skScreenshot.hideWindow = false;
                }
                uc.modules.setting.SettingManager.saveSetting();
                $screenShotSetting.hide();
            });

            $("body").click(function (e) {
                if ($emotionPanel) {
                    $emotionPanel.hide(e);
                }

                if ($screenShotSetting) {
                    $screenShotSetting.hide();
                }

                if ($textPanel) {
                    $(".font-sizes", $textPanel).hide();
                    $(".colors", $textPanel).hide();
                    $textPanel.hide();
                }
            });
        }

        function _disableAction(action) {
        	//if(action!="history")
            $("."+ action, $toolbar).hide();
        }
	    function _unDisableAction(action) {
		    $("."+ action, $toolbar).show();
	    }
        this._resetChaToolBar=function(){
            $(".file,.code,.history,.cloud", $toolbar).show();
        }
        this._disableAction = function(action) {
	        _disableAction(action);
        }
	    this._unDisableAction = function (action){
		    _unDisableAction(action);
	    }
    };


    jQuery.extend(uc.modules.component.ChatToolBar.prototype, {
        onSelectedEmotion: function (sender, args) {
        },

        onUploadImage: function (sender, args) {
        },

        onUploadFile: function (sender, args) {
        },

        onUploadCode: function (sender, args) {
        },

        onScreenShot: function (sender, args) {
        },

        onCodeEdit: function(sender, args) {
        },
        onCloudFile:function(sender,args){
        },
        disableAction: function (action) {
            this._disableAction(action);
        },
	    unDisableAction:function(action){
		    this._unDisableAction(action);
	    },
        resetChaToolBar:function(){
            this._resetChaToolBar();
        }
    })
})(uc);