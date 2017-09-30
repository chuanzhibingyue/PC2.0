/**
 * Created by senlin.ouyang on 2015/7/20.
 */
(function (uc) {
    uc.modules.component.MessageInputBox = function () {
        var $container, options, me = this, $box, $toolbar, htmlEditor, disableActions={}, enabled=true;

        function _initUI() {
            $box = $("<div class='message-input-box'>"
                +   "<div class='notice-menu' style='display: none'>"
                +       "<div class='menu-item' id='enter'>"+ uc_resource.Chat.SendByEnter +"</div>"
                +       "<div class='menu-item' id='altenter'>" + uc_resource.Chat.SendByAltEnter + "</div>"
                +   "</div>"
                +   "<div class='msg-notice-box' style='display:none;'/>"
                + "<div class='disable-mask' style='display:none;'/>"
            + "<div class='toolbar'></div>"
            + "<div class='content'><div class='input-shadow'></div></div>"
            + "<div class='failTip'>"+uc_resource.Message.sendMessageFialed+"</div>"
            //+ "<div class='notice'>"+ uc_resource.Chat.SendByEnter +"</div>"
            + "<div class='send-wrap'><div class='btn-uikit btn-gray btn-sending'>"+ uc_resource.Chat.Sending +"</div><div class='send-setting'></div></div>"
            + "</div>").appendTo($container);

            $toolbar = new uc.modules.component.ChatToolBar($box.find("div.toolbar"));
            htmlEditor = new uc.modules.component.HtmlEditor($box.find("div.content"), options.moduleId);

        }


        function _bindEvent() {
            htmlEditor.setOnInput(function(){
                setTimeout(function () {
                    _saveDraft();
                }, 50);
            });
            htmlEditor.onAt = function(x, y, id){
                me.onAt(x, y, id);
            };

            htmlEditor.onAtsearch = function(atId, text){
                return me.onAtsearch(atId, text);
            };

            htmlEditor.onkeydown = function (e) {
                $(".msg-notice-box", $box).hide();
                if (e.which != 13) {
                    return true;
                }
                if (uc.settings.skMsg == 0) {
                    if (!(e.ctrlKey||e.altKey|| e.shiftKey)) {
                        e.preventDefault();
                        _sendText();
                        return true;
                    }
                } else{
                    if (e.ctrlKey) {
                        e.preventDefault();
                        _sendText();
                        return true;
                    }
                }
                return false;
            };

            htmlEditor.ondblclick = function (e) {
                var $img = $(e.target);
                if ($img) {
                    var link = $img.attr("src");
                    if (link == null) {
                        return;
                    }
                    link = link.replace(/\//g, "\\");
                    var fileName = link.substr(link.lastIndexOf("\\") + 1);

                    var emotion = uc.modules.emoji.EmojiConverter.smileyToUnicode(fileName);
                    if (emotion == fileName) {
                        uc.IUIService.previewImage(link);
                    }
                }
            };

            htmlEditor.handleClipboardFile = function (filePaths) {
                if (filePaths == "" || filePaths.length == null || filePaths.length <= 0) {
                    return;
                }
                $.each(filePaths, function (i, filePath) {
                    _sendFile(filePath);
                });
            };

            $toolbar.onSelectedEmotion = function (sender, args) {
                htmlEditor.insertImage("../images/emotions/" + args.emotion + ".emj", 20, 20, args.emotion);
                $(".html-editor",$box).focus();
            };

            $toolbar.onUploadImage = function (sender, args) {
                htmlEditor.insertImage(args.path);
                $(".html-editor",$box).focus();
            };

            $toolbar.onUploadFile = function (sender, args) {
                _sendFile(args.path);
            };

            $toolbar.onScreenShot = function (sender, args) {
                htmlEditor.insertImage(args.path);
                $(".html-editor",$box).focus();
            };

            $toolbar.onCodeEdit = function (sender, args) {
                uc.modules.chat.ChatManager.showCodeEdit(options);
            };
            $toolbar.onCloudFile = function (sender, args) {
                uc.modules.cloud.CloudManager.selectCloudFile(options);
            };
            $box.on('click', '.send-setting', function(e) {
                //$(document.body).trigger("click");
                $('.notice-menu', $box).fadeToggle(200);
                e.preventDefault();
                e.stopPropagation();
            });
            $box.on('click', '.btn-sending', function(e) {
                $(document.body).trigger("click");
                _sendText();
                e.preventDefault();
                e.stopPropagation();
            });
            $("body").click(function () {
                $('.notice-menu', $box).hide();
                $(".msg-notice-box", $box).hide();
            });
            $box.on('click', '.menu-item',
                function() {
                    var id = $(this).attr("id");
                    if (id == "enter") {
                        uc.settings.skMsg = 0;
                    } else {
                        uc.settings.skMsg = 1;
                    }
                    uc.modules.setting.SettingManager.saveSetting();
                    _updateNotice();
                });
        }

        function _updateNotice() {
            uc.util.LogUtil.info('MessageInputBox', 'UpdateNotice', 'uc.settings.skMsg:', uc.settings.skMsg);
            $('.notice-menu>div', $box).removeClass('active');
            $('.notice-menu>div:eq('+ uc.settings.skMsg +')', $box).addClass('active');
        }

        function _sendText() {
            $(".msg-notice-box", $box).hide();
            if (options.chatType == uc.constants.Chat.ChatType.SingleChat) {

                var promptStatus = uc.util.StorageUtil.getData("USER_STATUS:promptStatus");

                if (promptStatus == "1") {
                    var status = uc.util.StorageUtil.getData("USER_STATUS:status");
                    var $chatPanel = $(".single-chat");
                    $chatPanel.find(".status-msg").remove();
                    if (status == uc.constants.UserStatusType.InCall) {
                        $chatPanel.append("<div class='status-msg'><img /><span>"+uc_resource.UserStatus.InCallMessage+"</span></div>");
                    } else if (status == uc.constants.UserStatusType.InMeeting) {
                        $chatPanel.append("<div class='status-msg'><img /><span>"+uc_resource.UserStatus.InMeetingMessage+"</span></div>");
                    } else if (status == uc.constants.UserStatusType.Offline) {
                        $chatPanel.append("<div class='status-msg'><img /><span>"+uc_resource.UserStatus.OffLineMessage+"</span></div>");
                    }else if (status == uc.constants.UserStatusType.Busy) {
                        $chatPanel.append("<div class='status-msg'><img /><span>"+uc_resource.UserStatus.BusyMessage+"</span></div>");
                    }

                    uc.util.StorageUtil.setData("USER_STATUS:promptStatus", "0");
                }
            }

            if (!enabled) {
                return;
            }
            var content = _getContent();
            /*
            if (content.content && content.content.length > 5000) {
                uc.modules.component.PromptMessageBox.open({title: "提示", message: "您发送的文字消息过长，请调整至5千字以内再发送", buttonMode:"ok", OkText: "知道了"},
                    function(result){
                        this.close();
                });
                return;
            }
            */
            var contentType = uc.constants.Message.TextMessageType.Text;//content.isText
                    //? uc.constants.Message.TextMessageType.Text
                    //: uc.constants.Message.TextMessageType.MIME;

            var chatInfo = _getChatObjectInfo();
            chatInfo.chatType = options.chatType;
            chatInfo.eventId = options.eventId;
            chatInfo.meetingId = options.meetingId;
            chatInfo.startTime = options.startTime;

            //atUsers
            var atUsers = [];
            $("iframe").contents().find("body").find(".atSpan[at-id]").each(function(){
                    atUsers.push({
                        userId : parseInt($(this).attr('at-id')),
                        siteId :0,
                        resId :0
                    })

                });

            var sendTextByContent = function(ct){
                ct = ct.replace(/<[^>]+>/g,"");
                var testStr = ct.replace(/(^\s*)|(\s*$)/g, '');
                if (testStr == ""){
                    return;
                }
                var messageEntity = {
                    contentType: contentType,
                    content: ct
                };


                var textFormat = uc.modules.setting.SettingManager.getFontSettingFormat();
                uc.modules.chat.MessageManager.sendText(chatInfo, messageEntity, atUsers, function (args) {
                    if(args.errorCode!=uc.ErrorCode.Success){

                        $box.find(".failTip").show();
                        if(failTip){
                            clearTimeout(failTip);
                        }
                        var failTip=setTimeout(function(){
                            $box.find(".failTip").hide();
                            clearTimeout(failTip);
                        },1000);
                        return;
                    }
                    var textMessageData = {
                        errorCode: args.errorCode,
                        msgId: args.msgId,
                        chatType: args.chatType,
                        chatObjectId: args.chatObjectId,
                        messageType: args.messageType,
                        contentType: args.contentType,
                        content: args.content,
                        textFormat: args.textFormat
                    };
                    me.onBeforeSend(me, textMessageData);
                },textFormat);
            };

            if (content.content!=undefined) {
                if (content.images.length == 0){
                    var str = content.content;
                    str = str.replace(/<[^>]+>/g,"");
                    str = str.replace(/(^\s*)|(\s*$)/g, '');
                    if (str.length <= 0) {
                        $(".msg-notice-box", $box).text(uc_resource.Chat.CannotSendBlank);
                        $(".msg-notice-box", $box).show();
                        htmlEditor.clearContent();
                        setTimeout(function(){$(".msg-notice-box", $box).hide(); },1500);
                        return;
                    }
                }
                var texts = content.content.split("uc_special_ch_sep");
                for (var i = 0; i < texts.length; i ++) {
                    sendTextByContent(texts[i]);
                }
            }

            var sendImageByPath = function(path) {
                if (!uc.util.FileUtil.fileExist(path)) {
                    var fileName = path.substr(path.lastIndexOf("\\") + 1);
                    if (fileName == "") {
                        fileName = uc_resource.Chat.Image;
                    }
                    if (fileName > 100){
                        fileName = fileName.substr(0, 100);
                        fileName += "...";
                    }
                    uc.modules.component.PromptMessageBox.open({title: uc_resource.Common.Notification, message: fileName + uc_resource.Chat.Nonentity, buttonMode: "ok", OkText: uc_resource.Common.OK},
                        function(){
                            this.close();
                        });
                    return;
                }
                uc.modules.chat.MessageManager.uploadFile(path, true, chatInfo, function (args) {
                    if(args.errorCode!=uc.ErrorCode.Success){
                        $box.find(".failTip").show();
                        if(failTip){
                            clearTimeout(failTip);
                        }
                        var failTip=setTimeout(function(){
                            $box.find(".failTip").hide();
                            clearTimeout(failTip);
                        },1000);
                        return;
                    }
                    var imageMessageData = {
                        errorCode: args.errorCode,
                        taskId: args.taskId,
                        chatType: args.chatType,
                        chatObjectId: args.chatObjectId,
                        messageType: uc.constants.Message.ContentType.Image,
                        filePath: path,
                        fileName: _getFileName(path),
                        fileSize: _getFileSize(path)
                    };
                    me.onBeforeSend(me, imageMessageData);
                    uc.IUIEvent.OnMessageFileUploading(args.chatObjectId, args.chatType, args.taskId, uc.constants.Message.ContentType.Image,chatInfo);
                });
            };

            if (content.images.length > 0) {
                for (var i = 0; i < content.images.length; i++) {
                    var path = content.images[i];
                    sendImageByPath(path);
                }
            }

            htmlEditor.clearContent();
            //remove draft
            uc.modules.draft.DraftMessageManager.removeDraftMessage(options.chatObjectId, options.chatType);
        }

        function _sendFile(filePath) {
	        //上传会议资料，跳过disableActions["file"]验证
		    if (!((!disableActions["file"] || !disableActions["uploadMeetingDoc"]) && enabled)) {
			   return;
		    }
            var chatInfo = _getChatObjectInfo();
            chatInfo.chatType = options.chatType;
            chatInfo.eventId = options.eventId;
            chatInfo.startTime = options.startTime;

            uc.modules.chat.MessageManager.uploadFile(filePath, false, chatInfo, function (args) {
                if(args.errorCode!=uc.ErrorCode.Success){
                    $box.find(".failTip").show();
                    if(failTip){
                        clearTimeout(failTip);
                    }
                    var failTip=setTimeout(function(){
                        $box.find(".failTip").hide();
                        clearTimeout(failTip);
                    },1000);
                    return;
                }
                var fileMessageData = {
                    errorCode: args.errorCode,
                    taskId: args.taskId,
                    chatType: args.chatType,
                    chatObjectId: args.chatObjectId,
                    messageType: uc.constants.Message.ContentType.File,
                    filePath: filePath,
                    fileName: _getFileName(filePath),
                    fileSize: _getFileSize(filePath)
                };

                me.onBeforeSend(me, fileMessageData);
                uc.IUIEvent.OnMessageFileUploading(args.chatObjectId, args.chatType, args.taskId, uc.constants.Message.ContentType.File,chatInfo);
            });
        }

        function _getChatObjectInfo() {
            return {
                userId: options.chatObjectId,
                siteId: options.siteId,
                resId: options.resId
            };
        }

        function _getContent() {
            var content = htmlEditor.getContent();
            var isText = true, images = [];
            content = uc.util.HtmlUtil.replaceImage(content, function (link) {
                isText = false;
                link = link.replace(/\//g, "\\");
                var fileName = link.substr(link.lastIndexOf("\\") + 1);

                var emotion = uc.modules.emoji.EmojiConverter.smileyToUnicode(fileName);

                if (emotion != fileName) {
                    return emotion;
                }else {
                    var oldEmotion = uc.util.EmotionUtil.emotionToCode(fileName);
                    if(oldEmotion != fileName){
                        return oldEmotion;
                    }
                    link =  uc.util.StringUtil.htmlDecode(link);
                    images.push(link);

                    return "uc_special_ch_sep";
                    //return false;
                }
            });
            content = content.replace(/&nbsp;/g, " ");
            return { isText: isText, content: content, images: images };
        }

        function _getDraft() {
            try {
                var content = htmlEditor.getContent();

                var draft = uc.util.HtmlUtil.replaceImage(content, function (link) {
                    var fileName = link.substr(link.lastIndexOf("/") + 1);

                    var emotion = uc.modules.emoji.EmojiConverter.smileyToUnicode(fileName);
                    if (emotion != fileName) {
                        return true;
                    }else {
                        return uc_resource.Message.ImageMessage;
                    }
                });
                return { draft: draft, content: content.replace(/\n/g, "<br>") };
            }
            catch (e) {
                return { draft: "", content: "" };
            }
        }

        function _setDraft(draftSource) {
            me.isFromEvent = draftSource == uc.constants.UI.DraftSource.Event;
            var id = options.chatObjectId, type = options.chatType;
            var draft = uc.modules.draft.DraftMessageManager.getDraftMessage(id, type);
            if (draft) {
                htmlEditor.setContent(draft.content || "", draftSource);
            }
            if(draftSource == uc.constants.UI.DraftSource.Event){
                setTimeout(function(){
                    me.isFromEvent = false;
                }, 200)
            }
        }

        function _saveDraft(){
            if (uc.modules.chat.ChatManager.moduleId && uc.modules.chat.ChatManager.currentChatType == uc.constants.Chat.ChatType.MeetingChat) {
                return;
            }
            var draft = _getDraft();
            if(draft  && draft.draft){
                uc.modules.draft.DraftMessageManager.addDraftMessage(options.chatObjectId, options.chatType, draft);
                if(!me.isFromEvent){
                    uc.IUIEvent.OnDraftMessageChanged(options.chatObjectId, options.chatType, options.moduleId);
                }
            } else {
                uc.modules.draft.DraftMessageManager.removeDraftMessage(options.chatObjectId, options.chatType);
            }
        }

        function _getFileName(path) {
            var formatPath = path.replace(/\//g, "\\");
            var parts = formatPath.split("\\");
            if (parts.length > 0) {
                return parts[parts.length - 1];
            }
            else {
                return path;
            }
        }

        function _getFileSize(path) {
            return uc.util.FileUtil.getFileSize(path);
        }

        function _disableAction(action){
            disableActions[action] = true;
            $toolbar.disableAction(action);
        }

	    function _unDisableAction(action){
		    disableActions[action] = false;
		    $toolbar.unDisableAction(action);
	    }

        function _enable(bEnable){
            enabled = bEnable;
            htmlEditor.enable(bEnable);
            if (enabled) {
                $('.disable-mask', $box).hide();
                $('.send-wrap', $box).removeClass('hide');
                $('.screen-shot', $box).show();
                $('.image', $box).show();
                $('.screen-shot-setting', $box).show();
            } else {
                $('.disable-mask', $box).show();
                $('.send-wrap', $box).addClass('hide');
            }
        }

        function _reset() {
            $box.appendTo($container);
            htmlEditor.reset();

            _updateNotice();
            _setDraft(uc.constants.UI.DraftSource.DraftDataSet);
        }

        this._init = function (container, opts) {
            if (!container) {
                return;
            }

            if (!opts
                || opts.chatType == undefined || !opts.chatObjectId == undefined
                || !opts.siteId == undefined || !opts.resId == undefined) {
                return;
            }

            options = opts;


            if (container.jquery) {
                $container = container;
            }
            else {
                $container = $("#" + container);
            }
           disableActions={};
            if(!this.initialized){
                _initUI();
                _bindEvent();
                this.initialized = true;
            }

            _reset();
            $toolbar.resetChaToolBar();
        };

        this._setDraft = function (chatId, chatType, moduleId) {
            if(moduleId == options.moduleId){
                return;
            }
            _setDraft(uc.constants.UI.DraftSource.Event);
        };

        this._disableAction = function (action) {
            _disableAction(action);
        };

	    this._unDisableAction = function (action) {
		    _unDisableAction(action);
	    };

        this._enable = function (bEnable) {
            _enable(bEnable);
        };

        this._setAtMember = function (atId, userId, userName, draftId){
            htmlEditor.setAtMember(atId, userId, userName, draftId);
        };

        this._updateNotice = function() {
            _updateNotice();
        };
        this._updateIframePosition = function(){
            if(htmlEditor){
                htmlEditor.setIframePosition();
            }
        };

        this._getHtmlContent = function(){
            return htmlEditor.getHtmlContent();
        };

        this._focusInput = function(){
            htmlEditor && htmlEditor.focusInput();
        };

        this._clear = function(){
            var chartId = uc.modules.chat.ChatManager.currentChatId;
            var chartType = uc.modules.chat.ChatManager.currentChatType;
            uc.util.LogUtil.info('MessageInputBox', '_clear', 'chatInfo', {chartId: chartId, chartType: chartType, initialized: this.initialized});
            if(this.initialized){
                $box.detach();
                $box.removeAttr('style');
                $box.find("div.content").removeAttr('style');
                if(htmlEditor){
                    htmlEditor.setIframePosition();
                    htmlEditor.clearContent();
                    htmlEditor.hide();
                }
            }
        };

        this._clearHtml = function () {
            if (htmlEditor) {
                htmlEditor.clear();
            }
        };

        this._showChatPrompt = function(flag) {
            if (flag === false) {
                return;
            } else {
                $(".single-chat").append("<div class='prompt hide'><img src=''><span></span></div>");
            }
        }
        this._sendFile =function(filePath){
            _sendFile(filePath);
        }
    };

    jQuery.extend(uc.modules.component.MessageInputBox.prototype, {
        init: function (container, options) {
            this._init(container, options);
        },
        setDraft: function (chatId, chatType, moduleId) {
            this._setDraft(chatId, chatType, moduleId);
        },
        onBeforeSend: function (sender, args) {
        },
        setUserData: function(data) {
            this.userData = data;
        },
        getUserData:function () {
            return this.userData;
        },
        disableAction: function (action) {
            this._disableAction(action);
        },
	    unDisableAction: function (action) {
		    this._unDisableAction(action);
	    },
        enable: function(bEnable) {
            this._enable(bEnable);
        },
        onAt: function(x, y, id){
        },
        onAtsearch: function(atId, text) {
            return false;
        },
        setAtMember: function(atId, userId, userName, draftId){
            this._setAtMember(atId, userId, userName, draftId);
        },
        updateNotice: function() {
            this._updateNotice();
        },
        updateInputBoxPosition: function(){
            this._updateIframePosition();
        },

        getHtmlContent: function(){
            return this._getHtmlContent();
        },

        focusInput: function(){
            this._focusInput();
        },
        clear: function(){
            this._clear();
        },
        clearHtml: function(){
            this._clearHtml();
        },
        showChatPrompt: function(flag) {
            this._showChatPrompt(flag);
        },
        rightClickToSendFile:function(filePath){
            this._sendFile(filePath);
        }
})
})(uc);