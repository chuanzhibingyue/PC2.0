/**
 * Created by senlin.ouyang on 2015/7/20.
 */
(function(uc){
    uc.modules.component.MessageBox = function(){
        this.superGroupBgColor = {
            0: '#989BD0',
            1: '#70AEE2',
            2: '#3AC0B8',
            3: '#Ed88B7',
            4: '#F08681',
            5: '#F09B50'
        };
    };

    jQuery.extend(uc.modules.component.MessageBox.prototype, {
        init: function(parentNode, parent, chatType, userId){
            this.nodeHtml = '<div class="more-msg-line">'
                +               '<span class="more-msg-loading" style="display:none" ></span>'
                +               '<span class="more-msg" style="display:none" >'+ uc_resource.Message.ViewMore +'</span>'
                +           '</div>'
                +           '<ul class="chat-list">'
                +           '</ul>'
                + 			'<ul class="api-menu">'
                + 			'</ul>'
                +           '<ul class="left-menu" style="display:none" >'
                +               '<li class="resend-menu">'+ uc_resource.Message.Resend +'</li>'
                +           '</ul>'
                +           '<ul class="right-menu" style="display:none" >'
                +               '<li class="copy-menu">'+ uc_resource.Common.Copy +'</li>'
                +               '<li class="save-menu">'+ uc_resource.Common.SaveAs +'</li>'
                +               '<li class="send-menu">'+ uc_resource.Common.Forward +'</li>'
                +               '<li class="backout-menu">'+ uc_resource.Common.BackOut +'</li>'
                +               '<li class="saveCloud-menu">'+uc_resource.MessageBox.saveCloud+'</li>'
                +           '</ul>';

            this.unRead='<div class="unReadNumber"></div>';
            this.node = $('<div class="messageBox"></div>');
            this.faildTip = $('<div class="messageBox-failTip">'+uc_resource.Message.sendMessageFialed+'</div>');
            this.parentNode = parentNode;
            this.parent = parent;
            this.chatType = chatType;
            this.currentUser = userId;

            this.node.append(this.nodeHtml);
            this.node.appendTo(parentNode);

            this.bindEvent();
        },
        setMax: function(){
            this.node.parent().addClass('max');
        },
        setNormal: function(){
            this.node.parent().removeClass('max');
        },
        showRightMenu: function (e, type, elm, showBackoutMenu) {
            var $elm = $(elm);
            var node = this.node;
            var left = e.clientX;
            var top = e.clientY;

            if (document.body.clientWidth - left < 120) {
                left = document.body.clientWidth - 120;
            }

            if(showBackoutMenu){
                var seq = parseInt($elm.parents(".message-item").attr('seq'));

                $(".backout-menu", node).attr("seq", seq).show();
            }else{
                $(".backout-menu", node).hide();
            }

            if (type == 'img') {
                $('.saveCloud-menu', node).attr("name", $elm.attr('name')).attr("path", $elm.attr('data-url')).show();
                $('.save-menu', node).attr("name", $elm.attr('name')).attr("path", $elm.attr('src')).show();
                $('.send-menu', node).attr("target", 'img').attr("size", $elm.attr('size')).attr("url", $elm.attr('data-url')).show();
                $('.copy-menu', node).hide();
            } else if(type == 'file'){
                $('.save-menu,.saveCloud-menu', node).hide();
                $('.send-menu', node).hide();
                $('.copy-menu', node).hide();
            }else{
                $('.save-menu,.saveCloud-menu', node).hide();

                if (document.getSelection().toString() == "") {//未选择内容可以转发当前条信息
                    $('.send-menu', node).attr("target", 'text').show();
                } else {
                    $('.send-menu', node).attr("target", '').hide();
                    $(".backout-menu", node).hide();
                }
                $('.copy-menu', node).show();
            }

            if(uc.modules.cloud.CloudManager.checkCloudIsLoginSuccess()==0 ||
                uc.modules.cloud.CloudManager.checkCloudTokenIsSuccess()!=0 ){
                $('.saveCloud-menu', node).hide();
            }

            $('.right-menu', node).css({position: "fixed",'top':top,'left':left, 'z-index':2}).show();

            this.showRightMenuTarget = elm;
        },
        hideRightMenu: function(){
            this.showRightMenuTarget = null;

            $('.right-menu', this.node).hide();
        },
        getChatObjectInfo: function(){
            var chatType = this.parent.chatType;
            var siteId = this.parent.siteId;
            var chatObjId;

            if(chatType == uc.constants.Chat.ChatType.SingleChat){
                chatObjId = this.parent.userId;
            }else if(chatType == uc.constants.Chat.ChatType.OpenApiChat){ // added by xue.bai_2 on 2016/04/11
                chatObjId = this.parent.appInfo.appId;
            }else{
                chatObjId = this.parent.groupId;
            }

            return {
                chatType: chatType,
                chatObjId: chatObjId,
                siteId: siteId,
                resId: 100
            };
        },
        getChatObjectInfos: function(){
            var chatType = this.chatType;
            var siteId = this.siteId;
            var chatObjId;

            if(chatType == uc.constants.Chat.ChatType.SingleChat){
                chatObjId = this.userId;
            }else if(chatType == uc.constants.Chat.ChatType.OpenApiChat){
                chatObjId = this.appInfo.appId;
            }else{
                chatObjId = this.groupId;
            }

            return {
                chatType: chatType,
                chatObjId: chatObjId,
                siteId: siteId,
                resId: 100
            };
        },
        bindEvent: function(){
            var self = this;
            this.moreMsgLine = $('.more-msg-line', self.parentNode);
            this.moreMsg = $('.more-msg', self.parentNode);
            this.moreMsgLoading = $('.more-msg-loading', self.parentNode);
            this.chatList = $('.chat-list', self.parentNode);
            this.leftMenu = $('.left-menu', this.node);
            this.rightMenu = $('.right-menu', this.node);
            var clickLogoTimer = -1;
            var currentUserInfo = uc.IContactService.getCurrentUserInfo();

            this.moreMsg.click(function(){
                self.onMoreMsgLoading();
                self.parent.getMoreHistoryMsg();
            });

            $(".content-panel").on("click",".read-unread-staff",function(){
                var userId = parseInt($(this).attr("data-userId"));

                uc.IUIService.showPopupContactProfile(userId);
            });

            this.chatList.mousewheel(function(){
                if(self.chatList.find(".unReadNumber").length){
                    $(".mumberMessage").remove();
                }
            });

            this.node.on('contextmenu', function(e){
                e.preventDefault();
            });

            this.node.scroll(function(){
                $('.left-menu', self.node).hide();
                $('.api-menu', self.node).hide();
                $(".mumberMessage").remove();
                self.hideRightMenu();

                if (self.moreMsg.is(":hidden")) {
                    return;
                }

                var top = self.node[0].scrollTop;
                if (top < 200) {
                    self.onMoreMsgLoading();
                    self.parent.getMoreHistoryMsg();
                }
            });

            this.chatList.on("click", '.unReadNumber', function(){
                if(self.chatType==uc.constants.Chat.ChatType.SingleChat || $(this).hasClass("successRead")){
                    return;
                }

                var _t = $(this);
                var seqId = parseInt($(this).parents("li.message-item").attr("seq"));

                $(".mumberMessage").remove();

                uc.IGroupService.getUnreadUserList(self.chatType, self.currentUser, [seqId], function(arg){
                    if(arg[0] == 0){
                        _t.parents("li.message-item").siblings().removeClass("getUserlist");
                        _t.parents("li.message-item").addClass("getUserlist");
                    }
                });

            });

            //点击发送失败图标响应事件
            this.chatList.on('click', '.send-fail', function(e){
                var This = $(this);
                var left = This.offset().left;
                var top = This.offset().top;
                var msgId = Number(This.attr('msgId'));
                var msgObj = $('#' + msgId, self.chatList);

                if (This.attr("state") != "sendfail") {
                    return;
                }

                if(msgObj.length <= 0) {
                    return;
                }

                if ( $('.file-me-to', msgObj).length > 0 ||
                    $('.file-to-me', msgObj).length > 0 ||
                    $('.code-me-to', msgObj).length > 0 ||
                    $('.code-to-me', msgObj).length > 0) {
                    return;
                }

                $('.resend-menu', self.node).attr("msgId", msgId);
                $('.left-menu', self.node).css({position: "fixed", 'top': top, 'left': left - 80, 'z-index': 2});
                $('.left-menu', self.node).show();

                e.stopPropagation();
            });

            this.leftMenu.on('click', '.resend-menu', function(){
                var msgId = Number($(this).attr('msgId'));
                self.showSendingFlag(msgId);
                $('.left-menu', self.node).hide();
                setTimeout(function() {
                        self.parent.resendMessage(msgId);
                    }, 1000);
            });

            this.chatList.on('click', '.user-logo', function(){
                clearTimeout(clickLogoTimer);

                var userId = $(this).attr("rel");
                var uid = Number(userId.substring(4));

                if(self.appInfo != null &&
                    self.appInfo.pcUrl != null &&
                    self.appInfo.pcUrl != "" &&
                    uid == self.appInfo.appId){

                    var account =  uc.modules.chat.ChatManager.myUserInfo.account;
                    var token = uc.modules.chat.ChatManager.token;

                    uc.util.FileUtil.openFile(self.appInfo.pcUrl + '?uc_account=' + account +'&uc_token=' + token);
                    return;
                }

                clickLogoTimer = setTimeout(function(){
                    uc.IUIService.showPopupContactProfile(uid);
                }, 300);
            });


            this.chatList.on('dblclick', '.user-logo', function(){
                clearTimeout(clickLogoTimer);

                var userId = $(this).attr("rel");
                var uid = Number(userId.substring(4));

                if (self.appInfo != null &&
                    self.appInfo.pcUrl != null &&
                    self.appInfo.pcUrl != "" &&
                    uid == self.appInfo.appId) {

                    var account =  uc.modules.chat.ChatManager.myUserInfo.account;
                    var token = uc.modules.chat.ChatManager.token;

                    uc.util.FileUtil.openFile(self.appInfo.pcUrl + '?uc_account=' + account + '&uc_token=' + token);
                    return;
                }

                if(currentUserInfo && currentUserInfo.userId != uid){
                    uc.IUIService.openChatWindow(uc.constants.Chat.ChatType.SingleChat, uid, uc.constants.UI.OpenChatWindowSourceModule.Other, null, null, true);
                }
            });


            this.chatList.on('click', '.user-name', function() {
                var userId = $(this).attr("rel");
                var uid = Number(userId);

                if (self.appInfo != null &&
                    self.appInfo.pcUrl != null &&
                    self.appInfo.pcUrl != "" &&
                    uid == self.appInfo.appId) {

                    var account =  uc.modules.chat.ChatManager.myUserInfo.account;
                    var token = uc.modules.chat.ChatManager.token;

                    uc.util.FileUtil.openFile(self.appInfo.pcUrl + '?uc_account=' + account + '&uc_token=' + token);
                    return;
                }

                uc.IUIService.showPopupContactProfile(uid);
            });

            //单击图片查看大图;
            this.chatList.on('click', 'img', function(){
                var This = $(this);
                var url = '';

                if (This.attr('status') == "link") {
                    url = This.attr('data-url');
                    if (url != "") {
                        uc.util.FileUtil.openFile(url);
                    }
                    return;
                }

                if (This.attr('status') == "networkissue") {
                    This.attr("src", "../images/messagebox/placeImg.gif");
                    This.css({cursor:"default"});

                    url = This.attr('data-url');
                    var msgId = This.attr('data-id');
                    var seq = This.attr('seq');
                    var name = This.attr('name');

                    uc.modules.chat.ChatManager.downloadImg(self.chatId, self.chatType, msgId, seq, url, name,
                        function() {
                            This.attr('status', 'downloading');
                        }
                    );
                    return;
                }

                if(!This.attr('src') || This.attr('status') != "open") {
                    return;
                }

                if ($(this).hasClass("send-fail")) {
                    return;
                }

                var path = uc.IClientService.getDataDirectory(uc.constants.UI.DirectoryType.Image) + '\\' + self.chatId
                    + '_' + self.chatType + '\\' + This.attr("seq") + '_' + This.attr('name');

                if (uc.util.FileUtil.fileExist(path)) {
                    uc.IUIService.previewChatImage(self.chatId, self.chatType, This.attr("seq"), This.attr('name'));
                } else {
                    uc.IUIService.previewImage(This.attr("src"));
                }
            });

            this.chatList.on('contextmenu', 'img', function(e){
                var This = $(this);
                var status = This.attr('status');
                var seq = This.attr("seq");

                if(!This.attr('src')) {
                    return;
                }

                if (status != "open") {
                    return;
                }

                if (seq == null) {
                    return;
                }

                self.showRightMenu(e, 'img', this);
                e.preventDefault();
            });

            // |================= modified and added by xue.bai_2 on 2016/04/06 =================
            this.chatList.on('contextmenu', '.text-to-me', function(e){
                self.showRightMenu(e, 'text', this);
                e.preventDefault();
            });

            this.chatList.on('contextmenu', '.text-me-to', function(e){
                var showBackoutMenu = true;
                var options = self.getChatObjectInfos();
                var conversationType = options.chatType;
                var unReadNumber = $(this).find(".unReadNumber").text();

                if(conversationType == uc.constants.Conversation.ConversationType.OpenApi){
                    showBackoutMenu = false;
                }else{
                    if(unReadNumber == ''){
                        showBackoutMenu = false;
                    }
                }

                self.showRightMenu(e, 'text', this, showBackoutMenu);
                e.preventDefault();
            });

            this.chatList.on('contextmenu', '.img-me-to img', function(e){
                var This = $(this);
                var status = This.attr('status');
                var seq = This.attr("seq");

                if(!This.attr('src')) {
                    return;
                }

                if (status != "open") {
                    return;
                }

                if (seq == null) {
                    return;
                }

                var unReadNumber = This.parent().next().text();
                var showBackoutMenu = true;
                var options = self.getChatObjectInfos();
                var conversationType = options.chatType;

                if(conversationType == uc.constants.Conversation.ConversationType.OpenApi){
                    showBackoutMenu = false;
                }else{
                    if(unReadNumber == ''){
                        showBackoutMenu = false;
                    }
                }

                self.showRightMenu(e, 'img', this, showBackoutMenu);
                e.preventDefault();
            });

            this.chatList.on('contextmenu', '.file-list-me-to', function(e){
                if(!$(this).find(".op-progress").attr("style")){
                    return;
                }

                var options = self.getChatObjectInfos();
                var conversationType = options.chatType;
                if(conversationType == uc.constants.Conversation.ConversationType.OpenApi){
                    return;
                }

                self.showRightMenu(e, 'file', this, true);
                e.preventDefault();
            });
            // ================= modified and added by xue.bai_2 on 2016/04/06 =================|
            
            var elm2MsgBoxText = function (elm) {
                var childs = elm.childNodes;
                var res = "";
                var node;

                for (var i = 0, len = childs.length; i < len; i++) {
                    node = childs[i];

                    if($(node).hasClass("unReadNumber")){
                        continue;
                    }

                    var reChilds = node.childNodes;

                    for(var j = 0, length = reChilds.length; j < length; j++){
                        var reNode = reChilds[j];

                        if (reNode.nodeType == 1 && reNode.nodeName) {
                            if (reNode.nodeName == 'A') {
                                res += reNode.innerText;
                            } else if (reNode.nodeName == 'IMG') {
                                res += uc.util.EmotionUtil.altToEmotionUnicode(reNode.getAttribute('alt'));
                            } else if (reNode.nodeName == 'BR') {
                                res += '\n';
                            } else {
                                res += $(reNode).text();
                            }
                        } else if (reNode.nodeType == 3) {
                            res += reNode.nodeValue;
                        }
                    }

                    if(length){
                        continue;
                    }

                    if (node.nodeType == 1 && node.nodeName) {
                        if (node.nodeName == 'A') {
                            res += node.innerText;
                        } else if (node.nodeName == 'IMG') {
                            res += uc.util.EmotionUtil.altToEmotion(node.getAttribute('alt'));
                        } else if (node.nodeName == 'BR') {
                            res += '\n';
                        } else {
                            res += $(node).text();
                        }
                    } else if (node.nodeType == 3) {
                        res += node.nodeValue;
                    }
                }
                return res;
            };

            var elm2MsgText = function (elm) {
                var childs = elm.childNodes;
                var res = "";
                var node;

                for (var i = 0, len = childs.length; i<len; i++) {
                    node = childs[i];

                    if($(node).hasClass("unReadNumber")){
                        continue;
                    }

                    var reChilds = node.childNodes;

                    for(var j= 0, length = reChilds.length; j < length; j++){
                        var reNode = reChilds[j];

                        if (reNode.nodeType == 1 && reNode.nodeName) {
                            if (reNode.nodeName == 'A') {
                                res += reNode.innerText;
                            } else if (reNode.nodeName == 'IMG') {
                                res += uc.util.EmotionUtil.altToEmotionUnicode(reNode.getAttribute('alt'));
                            } else if (reNode.nodeName == 'BR') {
                                res += '\n';
                            } else {
                                res += $(reNode).text();
                            }
                        } else if (reNode.nodeType == 3) {
                            res += reNode.nodeValue;
                        }
                    }

                    if(length){
                        continue;
                    }

                    if (node.nodeType == 1 && node.nodeName) {
                        if (node.nodeName == 'A') {
                            res += node.innerText;
                        } else if (node.nodeName == 'IMG') {
                            res += uc.util.EmotionUtil.altToEmotionUnicode(node.getAttribute('alt'));
                        } else if (node.nodeName == 'BR') {
                            res += '\n';
                        } else {
                            res += $(node).text();
                        }
                    } else if (node.nodeType == 3) {
                        res += node.nodeValue;
                    }
                }
                return res;
            };

            this.rightMenu.on('click', '.copy-menu', function(){
                if (document.getSelection().toString() == "") {//复制当前消息
                    var ipt = document.querySelector('#messageBoxHideIpt');

                    ipt.value = elm2MsgBoxText(self.showRightMenuTarget);
                    ipt.value = "";
                    ipt.select();

                    document.execCommand("copy");
                } else {//复制选中消息
                    document.execCommand("copy");
                }

                self.hideRightMenu();
            });

            this.rightMenu.on('click', '.save-menu', function(){
                self.hideRightMenu();

                var path = $(this).attr('path');
                var fileName = $(this).attr('name');

                uc.util.FileDialogUtil.openSaveFileDialog(uc_resource.Chat.SaveFiles, fileName, [{ desc: uc_resource.Chat.AllFiles, filter: '*.*'}],
                    function (args) {
                        var savePath = args[0];

                        if (savePath == null || savePath == "") {
                            return;
                        }

                        uc.util.FileUtil.copyFile(path, savePath);
                    });
            });

            this.rightMenu.on('click', '.saveCloud-menu', function(){
                self.hideRightMenu();

                var url = $(this).attr('path');
                var fileName = $(this).attr('name');

                self.OnSaveFileToCloud(url,fileName);
            });

            this.rightMenu.on('click', '.send-menu', function(){
                var $elm = $(this);
                var target = $elm.attr('target');

                if (target == 'text') {
                    if (document.getSelection().toString() == "") {//复制当前消息
                        var style = self.showRightMenuTarget.style;
                        var content = uc.util.StringUtil.escapeHtml(elm2MsgText(self.showRightMenuTarget));
                        var textFormat = 'font-size: ' + style.fontSize + '; font-weight: ' + style.fontWeight + '; font-style: ' + style.fontStyle + '; text-decoration: ' + style.textDecoration + ';';

                        uc.modules.chat.MessageManager.forwardTextMessage(content, textFormat);
                    }
                } else {
                    var $saveMenu = $('.save-menu', $('.right-menu', self.node));
                    var path = $saveMenu.attr('path');
                    var name = $saveMenu.attr('name');
                    var size = $elm.attr('size');
                    var url = $elm.attr('url');

                    if (url == null || path == null) {
                        return;
                    }

                    uc.modules.chat.MessageManager.forwardImageMessage(name, size, url, path);
                }
                self.hideRightMenu();
            });

            // added by xue.bai_2 on 2016/04/06
            this.rightMenu.on('click', '.backout-menu', function(){
                var options = self.getChatObjectInfo(),
                    toUserId = options.chatObjId,
                    conversationType = options.chatType,
                    $elm = $(this),
                    seq = parseInt($elm.attr("seq"));

                self.hideRightMenu();
                if(self.chatType == uc.constants.Chat.ChatType.MeetingChat){
                    uc.util.LogUtil.info("IMeetingService", "revocationMeetingMessage", "request", {groupId: toUserId, eventId: self.parent.meetingChatId, seq: seq});
                    uc.IMeetingService.revocationMeetingMessage(toUserId, self.parent.meetingChatId, seq, function(args){
                        uc.util.LogUtil.info("IMeetingService", "revocationMeetingMessage", "response", args);
                        var err = args[0];

                        if(err != 0){
                            uc.modules.component.PromptMessageBox.open(
                                {
                                    title:uc_resource.MessageBox.Notification,
                                    message: uc_resource.MessageBox.BackOutMessageFailed,
                                    buttonMode:"ok",
                                    OkText: uc_resource.MessageBox.Known
                                },
                                function(){
                                    this.close();
                            });
                        }
                    });
                    return;
                }

                uc.util.LogUtil.info("IGroupService", "revocationMessage", "request", {toUserId: toUserId, type: conversationType, seq: seq});
                uc.IGroupService.revocationMessage(toUserId, conversationType, seq, function(args){
                    uc.util.LogUtil.info("IGroupService", "revocationMessage", "response", args);
                    var errorCode = args[0];

                    if(errorCode != 0){
                        uc.modules.component.PromptMessageBox.open(
                            {
                                title:uc_resource.MessageBox.Notification,
                                message: uc_resource.MessageBox.BackOutMessageFailed,
                                buttonMode:"ok",
                                OkText: uc_resource.MessageBox.Known
                            },
                            function(){
                                this.close();
                        });
                    }
                });
            });

            //file receive;
            this.chatList.on('click', '.op-receive, .op-saveAs', function(){
                var classType = "receive";
                var msgId = $(this).parents("li").attr("id");
                var fsInfo = uc.modules.chat.ChatManager.FSFileInfo[msgId];
                var fileId = "";
                var status = $(this).attr("op-status");

                if($(this).attr("class") == 'op-saveAs'){
                    classType = "saveAs";
                }

                if(fsInfo){
                    fileId = fsInfo.fileId;
                }

                if((status!="open" && status!="uploading" && status!="downloading") &&
                    (self.chatType == uc.constants.Chat.ChatType.GroupChat || self.chatType == uc.constants.Chat.ChatType.ProjectChat ||
                    self.chatType == uc.constants.Chat.ChatType.MeetingChat)){

                    if(fileId && fileId!="0"){
                        uc.modules.chat.ChatManager.fileListTagName[fileId] = {
                            "type":classType,
                            "msgId":msgId,
                            "fileId":fileId,
                            "groupId":self.currentUser
                        };

                        if(self.parent.moduleId == uc.constants.UI.ModuleId.Meeting){
                            uc.modules.chat.MeetingChat.showFileCommentByFileId(fileId);
                        }else{
                            self.parent.showFileCommentByFileId(fileId);
                        }
                    }else{
                        uc.modules.chat.ChatManager.fileIdNotExistPrompt();
                    }
                }else {
                    self.fileMsgOperation($(this),fsInfo,classType);
                }
            });

            // cloud file Previewer
            this.chatList.on('click', '.cloudFilePreviewer', function(){
                if(!uc.modules.chat.ChatManager.checkCloudOpenStatus()){
                    return;
                }

                if(!uc.modules.chat.ChatManager.checkCloudLoadStatus()){
                    return;
                }

                var msgId = $(this).parents("li").attr("id");
                var data = uc.modules.chat.ChatManager.cloudFileInfo[msgId];

                if(data){
                    data= $.extend(JSON.parse(data), {"check_type":"preview"});
                    data = JSON.stringify(data);
                    uc.util.LogUtil.info('MessageBox', 'MessageBox', 'cloudFilePreviewer = ' + data , null);

                    if(self.chatType==uc.constants.Chat.ChatType.CloudChat){
                        uc.modules.cloud.CloudManager.gCheckFilePermission(data, "cloudFilePreviewer", $(this));
                    }else{
                        uc.IUIEvent.OnCloudFilePreviewer(data);
                    }
                }
            });

            //cloud file download;
            this.chatList.on('click', '.cloudDow, .cloudReceive', function(){
                var T = $(this);
                var type = T.attr("type");
                var localPath = $(this).attr("local-path");
                var that = $(this).parents("li");
                var msgId = that.attr("id");
                var status = $(this).attr("status");
                var options = self.getChatObjectInfo();

                if (msgId == null) {
                    return;
                }

                var cloudInfo = uc.modules.chat.ChatManager.cloudFileInfo[msgId];
                if(!cloudInfo){
                    uc.util.LogUtil.info('MessageBox', 'MessageBox', 'cloudFileDownload = ' + cloudInfo, null);
                    return;
                }

                var cloudData = JSON.parse(cloudInfo);

                if(status && status == "cancel"){
                    var taskId = that.find(".op-progress").attr("id");
                    var tagName = type == "receive" ? uc_resource.MessageBox.receive : uc_resource.MessageBox.saveAs;

                    if(taskId){
                        taskId = taskId.replace("gkDown","");
                        cloudData = $.extend(cloudData,{"taskid": taskId});
                        cloudInfo = JSON.stringify(cloudData);
                    }

                    uc.modules.cloud.CloudManager.gkRemoveDownload(cloudInfo);
                    self.deleteCloudFileInfoDownloading(taskId);
                    that.find(".op-progress").attr("value", 0);
                    $(this).text(tagName).removeAttr("status").siblings().show();
                    that.find(".op-progress,.cloudOpenDir,.cloudEndLine").hide();
                    return;
                }

                if (localPath != null && localPath != ""){
                    if (uc.util.FileUtil.openFile(localPath) != 0){
                        uc.modules.component.PromptMessageBox.open(
                            {
                                title:uc_resource.MessageBox.Notification,
                                message: uc_resource.MessageBox.openFileFail,
                                buttonMode:"ok",
                                OkText: uc_resource.MessageBox.Known
                            },
                            function(){
                                this.close();
                            });
                    }
                    return;
                }


                if(!uc.modules.chat.ChatManager.checkCloudOpenStatus()){
                    return;
                }

                if(!uc.modules.chat.ChatManager.checkCloudLoadStatus()){
                    return;
                }

                if(options.chatType == uc.constants.Chat.ChatType.CloudChat){
                    uc.modules.cloud.CloudManager.gCheckFilePermission(cloudInfo, "download", T);
                }else if(self.chatType == uc.constants.Chat.ChatType.GroupChat || self.chatType == uc.constants.Chat.ChatType.ProjectChat){
                    var fileId = that.find(".cloudComment").attr("file-id");

                    if(fileId && fileId!="0"){
                        uc.modules.chat.ChatManager.fileListTagName[fileId] = {
                            "type": "cloudFileDownload",
                            "msgId": msgId,
                            "fileId": fileId,
                            "TagType": type,
                            "groupId": self.currentUser
                        };

                        self.parent.showFileCommentByFileId(fileId);
                    }else{
                        uc.modules.chat.ChatManager.fileIdNotExistPrompt();
                    }
                }else{
                    uc.modules.chat.ChatManager.downloadCloudDir(cloudData, function(data, savePath){
                        self.OnStartDownloadCloudFile(T, data, savePath);
                    }, type);
                }
            });

            //打开云盘文件夹
            this.chatList.on('click', '.cloudOpenDir, .cloudOpenDir-show', function(){
                var localPath = $(this).attr("local-path");

                if (localPath == null && localPath == "") {
                    return;
                }

                if (uc.util.FileUtil.openFolder(localPath) != 0){
                    uc.modules.component.PromptMessageBox.open(
                        {
                            title:uc_resource.MessageBox.Notification,
                            message:uc_resource.MessageBox.openFileFail,
                            buttode:"ok",
                            OkText:uc_resource.MessageBox.Known
                        },
                        function(){
                            this.close();
                        });
                }
            });

            //打开文件夹
            this.chatList.on('click', '.op-openfolder', function(){
                var domObj = $('.op-receive', $(this).parent());
                var path = domObj.attr('local-path');

                if (path == null || path == "") {
                    return;
                }

                if (uc.util.FileUtil.openFolder(path) != 0){
                    uc.modules.component.PromptMessageBox.open(
                        {
                            title: uc_resource.Common.Notification,
                            message: uc_resource.Message.OpenFileFailed,
                            buttode: uc_resource.Common.OK,
                            OkText: uc_resource.Common.Known
                        },
                        function(){
                            this.close();
                        });
                }
            });

            //文件转发;
            this.chatList.on('click', '.op-forward', function(){
                var msgId = $(this).parents("li").attr("id");
                var fsInfo = uc.modules.chat.ChatManager.FSFileInfo[msgId];
                var fileId = "";

                if(fsInfo){
                    fileId=fsInfo.fileId;
                }

                if(self.chatType == uc.constants.Chat.ChatType.GroupChat ||
                    self.chatType == uc.constants.Chat.ChatType.ProjectChat ||
                    self.chatType == uc.constants.Chat.ChatType.MeetingChat){

                    if(fileId && fileId!="0"){
                        uc.modules.chat.ChatManager.fileListTagName[fileId] = {
                            "type": "forward",
                            "msgId": msgId,
                            "fileId": fileId,
                            "groupId": self.currentUser
                        };

                        if(self.parent.moduleId == uc.constants.UI.ModuleId.Meeting){
                            uc.modules.chat.MeetingChat.showFileCommentByFileId(fileId);
                        }else {
                            self.parent.showFileCommentByFileId(fileId);
                        }
                    }else{
                        uc.modules.chat.ChatManager.fileIdNotExistPrompt();
                    }
                }else{
                    uc.modules.chat.MessageManager.forwardFileMessage(fsInfo.fileName, fsInfo.fileSize, fsInfo.fileURL);
                }
            });

            //文件评论;
            this.chatList.on('click', '.op-comment, .cloudComment', function(){
                var fileId = $(this).attr("file-id");
                var msgId = $(this).parents("li").attr("id");

                if (msgId == null) {
                    return;
                }

                var cloudInfo = uc.modules.chat.ChatManager.cloudFileInfo[msgId];
                var options = self.getChatObjectInfo();

                uc.util.StorageUtil.setData('currentChatType',options.chatType);

                if(options.chatType == uc.constants.Chat.ChatType.CloudChat){
                    options = $.extend(options, {"fileId": fileId});

                    uc.modules.cloud.CloudManager.gCheckFileExist(cloudInfo, options, "comment");
                }else{
                    uc.modules.comment.CommentManager.initChatComment(fileId, cloudInfo, options);
                }
                return false;
            });

            //云库发出的消息转发
            this.chatList.on('click', '.cloudForward', function(){
                var msgId = $(this).parents("li").attr("id");

                if (msgId == null) {
                    return;
                }

                var cloudInfo = uc.modules.chat.ChatManager.cloudFileInfo[msgId];
                var options = self.getChatObjectInfo();
                var cloudFileInfoArr = JSON.parse(cloudInfo);
                var fileName = cloudFileInfoArr.filename;
                var fileSize = cloudFileInfoArr.filesize;
                var fileUrl = "";

                if(!uc.modules.chat.ChatManager.checkCloudOpenStatus()){
                    return;
                }

                if(!uc.modules.chat.ChatManager.checkCloudLoadStatus()){
                    return;
                }

                uc.util.LogUtil.info('MessageBox', 'forward', 'fileName = ' + fileName + ', cloudInfo = ' + cloudInfo + '', null);
                if(options.chatType == uc.constants.Chat.ChatType.CloudChat){
                    uc.modules.cloud.CloudManager.gCheckFilePermission(cloudInfo,"forward");
                }else if(self.chatType == uc.constants.Chat.ChatType.GroupChat ||
                    self.chatType == uc.constants.Chat.ChatType.ProjectChat){
                    var fileId=$(this).parents("li").find(".cloudComment").attr("file-id");

                    if(fileId && fileId!="0"){
                        uc.modules.chat.ChatManager.fileListTagName[fileId] = {
                            "type": "cloudFileForward",
                            "msgId": msgId,
                            "fileId": fileId,
                            "groupId": self.currentUser
                        };
                        self.parent.showFileCommentByFileId(fileId);
                    }else{
                        uc.modules.chat.ChatManager.fileIdNotExistPrompt();
                    }
                }else{
                    uc.modules.chat.MessageManager.forwardCloudFileMessage(fileName,fileSize,fileUrl,cloudInfo,cloudFileInfoArr);
                }
            });

            //fs文件保存云盘文件
            this.chatList.on('click', '.op-savecloud', function(){
                var msgId = $(this).parents("li").attr("id");
                var fsInfo = uc.modules.chat.ChatManager.FSFileInfo[msgId];
                var fileId = fsInfo.fileId;
                var fileUrl = $(this).parents("li").find(".op-receive").attr("server-path");
                uc.util.LogUtil.info('MessageBox', 'op-savecloud', 'msgId = ' + msgId + '', "fsInfo=" + JSON.stringify(fsInfo));

                if(fileUrl){
                    uc.modules.chat.ChatManager.FSFileInfo[msgId].fileURL = fileUrl;
                }

                if(self.chatType == uc.constants.Chat.ChatType.GroupChat ||
                    self.chatType == uc.constants.Chat.ChatType.ProjectChat||
                    self.chatType == uc.constants.Chat.ChatType.MeetingChat){

                    if(fileId && fileId != "0"){
                        uc.modules.chat.ChatManager.fileListTagName[fileId] = {
                            "type": "fsSaveCloud",
                            "msgId": msgId,
                            "fileId": fileId,
                            "groupId": self.currentUser
                        };

                        if(self.parent.moduleId == uc.constants.UI.ModuleId.Meeting){
                            uc.modules.chat.MeetingChat.showFileCommentByFileId(fileId);
                        }else{
                            self.parent.showFileCommentByFileId(fileId);
                        }
                    }else{
                        uc.modules.chat.ChatManager.fileIdNotExistPrompt();
                    }
                }else{
                    self.OnSaveFileToCloud(fsInfo.fileURL,fsInfo.fileName);
                }
            });

            //保存云盘文件
            this.chatList.on('click', '.cloudSave', function(){
                if(!uc.modules.chat.ChatManager.checkCloudOpenStatus()){
                    return;
                }

                if(!uc.modules.chat.ChatManager.checkCloudLoadStatus()){
                    return;
                }

                var msgId = $(this).parents("li").attr("id");
                if (msgId == null) {
                    return;
                }

                var cloudInfo = uc.modules.chat.ChatManager.cloudFileInfo[msgId];
                var options = self.getChatObjectInfo();

                uc.util.LogUtil.info('MessageBox', 'op-savecloud', 'cloudInfo = ' + JSON.stringify(cloudInfo) + '', null);
                if(self.chatType == uc.constants.Chat.ChatType.GroupChat ||
                    self.chatType == uc.constants.Chat.ChatType.ProjectChat){
                    var fileId=$(this).parents("li").find(".cloudComment").attr("file-id");

                    if(fileId && fileId != "0"){
                        uc.modules.chat.ChatManager.fileListTagName[fileId] = {
                            "type": "cloudFileSaveCloud",
                            "msgId": msgId,
                            "fileId": fileId,
                            "groupId": self.currentUser
                        };
                        self.parent.showFileCommentByFileId(fileId);
                    }else{
                        uc.modules.chat.ChatManager.fileIdNotExistPrompt();
                    }
                }else{
                    uc.modules.cloud.CloudManager.saveCloudFile(options,cloudInfo);
                }
            });

            //查看云库文件详情
            this.chatList.on('click','.ViewCloudFile', function(){
                var msgId = $(this).parents("li").attr("id");

                if (msgId == null) {
                    return;
                }

                self.OnOpenCloudDir(msgId);
            });

            //查看云库文件详情
            this.chatList.on('click','.sys-text-view', function(){
                var msgId = $(this).parents("li").attr("id");

                if (msgId == null) {
                    return;
                }

                self.OnOpenCloudDir(msgId);
            });

            this.chatList.on('click', '.sys_comment', function() {
                var fileId = $(this).attr("fileId");

                if (fileId != null) {
                    var options = self.getChatObjectInfo();

                    uc.util.StorageUtil.setData('currentChatType', options.chatType);

                    if(options.chatType == uc.constants.Chat.ChatType.CloudChat){
                        uc.modules.comment.CommentManager.initChatCommentMessage(fileId,options);
                    }else{
                       uc.modules.comment.CommentManager.initChatComment(fileId,null,options);
                    }
                }
                return false;
            });

            //代码下载,打开
            this.chatList.on('click', '.operation .download', function() {
                var type = self.parent.chatType;
                var that = $(this).parents("li");
                var msgId = that.attr("id");

                if( type == uc.constants.Chat.ChatType.GroupChat || type == uc.constants.Chat.ChatType.ProjectChat){
                    var fileId = that.find(".comment").attr("file-id");

                    if (fileId == null || fileId == "") {
                        return;
                    }

                    uc.modules.chat.ChatManager.fileListTagName[fileId] = {
                        "type": "downloadCode",
                        "msgId": msgId
                    };

                    self.parent.showFileCommentByFileId(fileId);
                }else{
                    self.downloadCode(msgId);
                }
            });

            //代码评论;
            this.chatList.on('click', '.operation .comment', function(){
                var fileId = $(this).attr("file-id");

                if (fileId == null || fileId == "") {
                    return;
                }

                var options = self.getChatObjectInfo();
                uc.modules.comment.CommentManager.initChatComment(fileId, null, options);
            });

            //代码转发;
            this.chatList.on('click', '.operation .forward', function(){
                var type = self.parent.chatType;
                var that = $(this).parents("li");
                var msgId = that.attr("id");

                if( type == uc.constants.Chat.ChatType.GroupChat || type == uc.constants.Chat.ChatType.ProjectChat){
                    var fileId = that.find(".comment").attr("file-id");

                    if (fileId == null || fileId == "") {
                        return;
                    }

                    uc.modules.chat.ChatManager.fileListTagName[fileId] = {
                        "type": "forwardCode",
                        "msgId": msgId
                    };

                    self.parent.showFileCommentByFileId(fileId);
                }else{
                    self.forwardCode(msgId);
                }
            });

            //代码预览
            this.chatList.on('click', '.operation .preview', function(){
                var type = self.parent.chatType;
                var that = $(this).parents("li");
                var msgId = that.attr("id");

                if(type == uc.constants.Chat.ChatType.GroupChat || type == uc.constants.Chat.ChatType.ProjectChat){
                    var fileId = that.find(".comment").attr("file-id");

                    if (fileId == null || fileId == "") {
                        return;
                    }

                    uc.modules.chat.ChatManager.fileListTagName[fileId] = {
                        "type": "previewCode",
                        "msgId": msgId
                    };

                    self.parent.showFileCommentByFileId(fileId);
                }else {
                    self.previewCode(msgId);
                }
            });

            //播放音频
            this.chatList.on('click', '.play-back', function(){
                var msgId = $(this).parents('li.message-item').attr("id");

                if($('.play-btn', $(this)).hasClass("playing")){
                    uc.util.AudioUtil.stopPlay(msgId);
                    return;
                }

                var url = $(this).attr("data-url");
                var msgSeq = $(this).attr("seq");

                uc.modules.chat.ChatManager.playSound(msgId, msgSeq, url);
                $('.play-btn', $(this)).addClass("playing");
            });

            this.node.on('click', function(e) {
                if ($(e.target).hasClass('op-comment') || $(e.target).hasClass('comment')) {
                    return;
                }

                if (self.parent.hideFileComment != null) {
                    self.parent.hideFileComment();
                }
            });

            //API 文字消息详情点击事件
            this.node.on('click', '.detail', function() {
                var tp = $(this).attr("tp");
                var ct =  $(this).attr("ct");
                var auth = $(this).attr("detailauth");
                var authUrl = auth == '1' ? uc.modules.openApi.OpenApi.getAppAuth(ct) : '';

                if (tp == "1" && ct != "") {    //url
                    var reg = /^(http|https|ftp):\/\//;
                    var url = ct + authUrl;

                    if (!reg.test(url)) {
                        url = 'http://' + url;
                    }

                    uc.util.FileUtil.openFile(url);
                } else {    //text
                    uc.modules.chat.ChatManager.showApiMsgPreview(ct);
                }
            });

            this.chatList.on('click', '.api-file-me-to', function(){
                var url = $(this).attr("data-url");

                if (url != null) {
                    uc.util.FileUtil.openFile(url);
                }
            });

            this.chatList.on('click', '.api-file-to-me', function(){
                var url = $(this).attr("data-url");

                if (url != null) {
                    uc.util.FileUtil.openFile(url);
                }
            });

            this.chatList.on('click', '.oadetail', function(){
                var parent = $(this).parents('.message-item');
                var url = $(this).attr("url");
                var auth = parent.attr("detailauth");
                var appId = parent.attr("data-id");
                var isOpenPreviewWindow = uc.modules.openApi.OpenApiManager.isOpenPreviewWindow(appId);

                if (url != null && !isOpenPreviewWindow) {
                    uc.util.FileUtil.openFile(self.getApiRequestUrl(url, auth));
                }

                //open notice
                if (isOpenPreviewWindow) {
                    uc.modules.chat.ChatManager.showApiNoticePreview(url);
                }
            });

            this.chatList.on('click', '.oaBtn.normal', function(){
                var parent = $(this).parents('.message-item');
                var msgSeq = $(this).attr("msgSeq");
                var url = $(this).attr("url");
                var dataName = $(this).attr("data-name");
                var needSuggestion = parseInt($(this).attr("data-need-suggestion"));
                var suggestionTitle = $(this).attr("data-suggestion-title");
                var needSignature = parseInt($(this).attr("data-need-signature"));
                var noEmpty = parseInt($(this).attr("data-not-empty"));
                var index = parseInt($(this).prev().attr("data-button-num"));
                var comment = parent.find('.comment');
                var auth = parent.attr("detailauth");

                if (parent.length <= 0) {
                    return;
                }

                if(needSignature){
                    uc.modules.component.PromptMessageBox.open({
                        title: uc_resource.WindowTitle.Notification,
                        message: uc_resource.OpenApi.NotSupportSignature,
                        buttonMode: "ok",
                        OkText: uc_resource.Common.Known
                    }, function () {
                        this.close();
                    });
                    return;
                }

                if (needSuggestion) {
                    var params = {msgSeq: msgSeq, url: url, auth: auth, dataName: dataName};
                    uc.IUIService.openOAApproveWindow({key:"OAApprove", noEmpty: noEmpty, suggestionTitle: suggestionTitle, params: params}, function (args) {
                        var  context = {text: {}};
                        if (args.btnCancel) {
                            return;
                        }

                        if (args.data && args.params.dataName) {
                            context.text[args.params.dataName] = args.data;
                        }

                        $(parent).find(".opstatus[data-button-num="+ index + "]").removeClass("fail");
                        $(parent).find(".opstatus[data-button-num="+ index + "]").removeClass("busy");
                        $(parent).find(".oaBtn[data-button-num="+ index + "]").removeClass("normal");
                        $(parent).find(".oamore").removeClass("normal");

                        self.parent.sendApiRequest(args.params.msgSeq, self.getApiRequestUrl(args.params.url, args.params.auth), 2, context, index);//args.data
                    });
                    return;
                }

                $(parent).find(".opstatus[data-button-num="+ index + "]").removeClass("fail");
                $(parent).find(".opstatus[data-button-num="+ index + "]").removeClass("busy");
                $(parent).find(".oaBtn[data-button-num="+ index + "]").removeClass("normal");
                $(parent).find(".oamore").removeClass("normal");

                self.parent.sendApiRequest(msgSeq, self.getApiRequestUrl(url, auth), 2, '', index);
            });

            this.chatList.on('click', '.oaitem .comment', function () {
                $(this).removeClass('error');
            });

            this.node.on('click', '.api-menuitem', function(){
                var msgId = $(this).attr("msgId");

                if (msgId == null) {
                    return;
                }

                var msgItem = $("#" + msgId, self.node);
                var msgSeq = $(".oaBtn", msgItem).attr("msgSeq");
                var url = $(this).attr("url");
                var auth = $(this).attr("auth");
                var suggestionTitle = $(this).attr("data-suggestion-title");
                var needSuggestion = parseInt($(this).attr("data-need-suggestion"));
                var needSignature = parseInt($(this).attr("data-need-signature"));
                var noEmpty = parseInt($(this).attr("data-not-empty"));
                var dataName = $(this).attr("data-name");

                if (msgItem.length <= 0 || msgSeq == null || url == null) {
                    return;
                }

                if(needSignature){
                    uc.modules.component.PromptMessageBox.open({
                        title: uc_resource.WindowTitle.Notification,
                        message: uc_resource.OpenApi.NotSupportSignature,
                        buttonMode: "ok",
                        OkText: uc_resource.Common.Known
                    }, function () {
                        this.close();
                    });
                    return;
                }

                if (needSuggestion) {
                    var params = {msgSeq: msgSeq, url: url, auth: auth, dataName: dataName};
                    uc.IUIService.openOAApproveWindow({key:"OAApprove", noEmpty: noEmpty, suggestionTitle: suggestionTitle, params: params}, function (args) {
                        var  context = {text: {}};
                        if (args.btnCancel) {
                            return;
                        }

                        if (args.data && args.params.dataName) {
                            context.text[args.params.dataName] = args.data;
                        }

                        $(".opstatus", msgItem).removeClass("fail");
                        $(".opstatus", msgItem).addClass("busy");
                        $(".oaBtn", msgItem).removeClass("normal");
                        $(".oamore", msgItem).removeClass("normal");

                        self.parent.sendApiRequest(args.params.msgSeq, self.getApiRequestUrl(args.params.url, args.params.auth), 2, context);//args.data
                    });
                    return;
                }

                $(".opstatus", msgItem).removeClass("fail");
                $(".opstatus", msgItem).addClass("busy");
                $(".oaBtn", msgItem).removeClass("normal");
                $(".oamore", msgItem).removeClass("normal");
                self.parent.sendApiRequest(msgSeq, self.getApiRequestUrl(url, auth));
            });

            this.chatList.on('click', '.oamore.normal', function(e){
                var parent = $(this).parent();

                if (parent == null) {
                    return;
                }

                var msgId = $(".oaBtn", parent).attr("msgId");
                var menuObj = $(".api-menu", self.node);
                var menustr = $(".opstatus", parent).attr("menuitem");
                var menuItems = JSON.parse(decodeURIComponent(menustr));
                var suggestionTitle = $(this).attr("data-suggestion-title");
                var needSuggestion = parseInt($(this).attr("data-need-suggestion"));
                var needSignature = parseInt($(this).attr("data-need-signature"));
                var noEmpty = parseInt($(this).attr("data-not-empty"));
                var dataName = $(this).attr("data-name");
                var left = $(this).offset().left;
                var top = $(this).offset().top;

                for (var i = 0; i < menuItems.length; i++) {
                    var item = menuItems[i];

                    if (item == null) {
                        continue;
                    }



                    menuObj.append('<li class="api-menuitem" msgId="' + msgId + '" url="' + item.url  + '" auth="'+ item.urlauth +'" data-need-suggestion="' + needSuggestion + '" data-not-empty="' + noEmpty +'" data-name="'+ dataName +'" data-suggestion-title="'+ suggestionTitle + '" data-need-signature="' + needSignature + '">' + item.title + '</li>');
                }

                menuObj.empty();
                menuObj.show();
                menuObj.css({position: "fixed",'top':top + 27,'left':left - 55, 'z-index':2});
                e.preventDefault();
                e.stopPropagation();
            });

            this.node.on('click', '.meetingVideoForward', function(){
                var videoName = $(this).parents(".meetingVideo").attr("videoName");
                var videoSize = $(this).parents(".meetingVideo").attr("videoSize");
                var videoURL = $(this).parents(".meetingVideo").attr("videoURL");
                var videoData = $(this).parents(".meetingVideo").attr("videoThumb");
                var videoDuration = $(this).parents(".meetingVideo").attr("videoDuration");
                var operatorName = $(this).parents(".meetingVideo").attr("operatorName");
                var operatorId = $(this).parents(".meetingVideo").attr("operatorId");
                var operatorAvatar = $(this).parents(".meetingVideo").attr("operatorAvatar");
                var thumbnailsURL = $(this).parents(".meetingVideo").attr("thumbnailsURL");

                uc.IUIService.openChooseContactWindow(
                    {
                        key: "forwardMessage" + (new Date().getTime()),
                        allowMultipleSelect: false,
                        allowChooseOrg:false
                    }, function (args) {
                        uc.IUIService.triggerCrossWindowEvent(uc.constants.UI.CrossWindowEventName.OnForwardMeetingVideo,
                            {
                                uniqueName: 'messageChat',
                                args: args,
                                videoName: videoName,
                                videoSize: videoSize,
                                videoURL: videoURL,
                                videoData: videoData,
                                thumbnailsURL: thumbnailsURL,
                                videoDuration: videoDuration,
                                operatorName: operatorName,
                                operatorId: operatorId,
                                operatorAvatar: operatorAvatar
                            });
                    });
            });

            this.chatList.on("click", '.meetingVideoImage', function(){
                var videoURL = $(this).parents(".meetingVideo").attr("videoURL");

                uc.IUIService.triggerCrossWindowEvent(uc.constants.UI.CrossWindowEventName.OnMeetingVideoMake,
                    {
                        uniqueName: 'messageVideoMake',
                        videoURL: videoURL
                    });

            });

            this.chatList.on("click", '.meetingVideoCheck', function(){
                $(this).prev().trigger('click');
            });

            this.chatList.on("click", '.meetingVideoShare', function(){
                var videoName = $(this).parents(".meetingVideo").attr("videoName");
                var videoSize = $(this).parents(".meetingVideo").attr("videoSize");
                var videoURL = $(this).parents(".meetingVideo").attr("videoURL");
                var videoDuration = $(this).parents(".meetingVideo").attr("videoDuration");
                var operatorName = $(this).parents(".meetingVideo").attr("operatorName");
                var operatorId = $(this).parents(".meetingVideo").attr("operatorId");
                var operatorAvatar = $(this).parents(".meetingVideo").attr("operatorAvatar");
                var thumbnailsURL = $(this).parents(".meetingVideo").attr("thumbnailsURL");

                uc.IUIService.triggerCrossWindowEvent(uc.constants.UI.CrossWindowEventName.OnShareMeetingVideo,
                    {
                        uniqueName: 'shareMeetingVideo',
                        videoURL: videoURL,
                        meetingName: videoName,
                        videoSize: videoSize,
                        thumbnailsURL: thumbnailsURL,
                        videoDuration: videoDuration
                    });
            });

            this.chatList.on('click', '.meeting-report-link .icon-forward', function () {
                var meetingReport = $(this).closest(".meeting-report");
                var reportTitle = meetingReport.find("p").text();
                var reportUrl = meetingReport.find("a[content]").attr("content");
                var siteId = Number(meetingReport.attr("site-id"));

                uc.IUIService.openChooseContactWindow(
                    {
                        key: "MeetingReportForward",
                        allowMultipleSelect: true,
                        allowMultipleSelectConversations: uc.constants.chooseContacts.allowMultipleSelectConversations,
                        allowChooseOrg: false,
                        isForward: true
                    }, function (args) {
                        if (args.okOrCancel) {
                            var contacts = args.otherItem.contacts;
                            var groupChats = args.otherItem.groupChats;
                            var projectChats = args.otherItem.projects;

                            for (var i = 0, length = contacts.length; i < length; i ++) {
                                var contact = contacts[i];
                                uc.IIMService.sendReportMessage({
                                    userId: contact,           // 用户id
                                    siteId: siteId,            // 站点id
                                    resId: 0                   // 资源id
                                }, reportTitle, reportUrl, function (args) {
                                    uc.IUIEvent.OnMessageSending(
                                        {
                                            chatId: contact,
                                            chatType: uc.constants.Conversation.ConversationType.SingleChat,
                                            taskId: 0,
                                            msgId: args[1],
                                            contentType: uc.constants.Message.ContentType.MeetingReport
                                    });
                                });


                            }

                            for (var j = 0, len = groupChats.length; j < len; j ++) {
                                var groupId = groupChats[j];
                                uc.IGroupService.sendReportMessage(uc.constants.Chat.ChatType.GroupChat, {
                                    userId: groupId,
                                    siteId: siteId,
                                    resId: 0
                                }, reportTitle, reportUrl, function (args) {
                                    uc.IUIEvent.OnMessageSending({
                                        chatId: groupId,
                                        chatType: uc.constants.Conversation.ConversationType.GroupChat,
                                        taskId: 0,
                                        msgId: args[1],
                                        contentType: uc.constants.Message.ContentType.MeetingReport
                                    });
                                });
                            }

                            for (var k = 0; k < projectChats.length; k++) {
                                var projectId = projectChats[k];
                                uc.IGroupService.sendReportMessage(uc.constants.Chat.ChatType.ProjectChat, {
                                    userId: projectId,
                                    siteId: siteId,
                                    resId: 0
                                }, reportTitle, reportUrl, function (args) {
                                    uc.IUIEvent.OnMessageSending({
                                        chatId: projectId,
                                        chatType: uc.constants.Conversation.ConversationType.ProjectChat,
                                        taskId: 0,
                                        msgId: args[1],
                                        contentType: uc.constants.Message.ContentType.MeetingReport
                                    });
                                });
                            }
                        }
                    }
                );
            }).on('click', '.meeting-report-link [content]', function () {
                uc.util.FileUtil.openFile($(this).attr("content"));
            });
        },
        OnSaveFileToCloud: function(url, fileName){
            if(!uc.modules.chat.ChatManager.checkCloudOpenStatus()){
                return;
            }
            if(!uc.modules.chat.ChatManager.checkCloudLoadStatus()){
                return;
            }
            var dataStr=JSON.stringify({"filename":fileName,"url":url});
            uc.util.LogUtil.info('MessageBox', 'op-savecloud', 'fileName = ' + fileName + ', cloudInfo = ' + dataStr + '', "url="+url);
            uc.modules.cloud.CloudManager.saveCloudFile(null,dataStr);
        },
        OnOpenCloudDir:function(msgId){
            if(!uc.modules.chat.ChatManager.checkCloudOpenStatus()){
                return;
            }
            if(!uc.modules.chat.ChatManager.checkCloudLoadStatus()){
                return;
            }
            var cloudInfo=uc.modules.chat.ChatManager.cloudFileInfo[msgId];
            if(cloudInfo){
                var cloudInfoMerge= $.extend(JSON.parse(cloudInfo),{"check_type":"preview"});
                var data=JSON.stringify(cloudInfoMerge);
                uc.modules.cloud.CloudManager.gCheckFilePermission(data,"cloudView");
            }else{
                uc.util.LogUtil.info('MessageBox', 'OnOpenCloudDir', 'cloudInfo = ' + cloudInfo + ', preview=', "msgId="+msgId);
            }
        },
        OnCommentCloudFile:function(data,cloudData,option){
            var dataObj=JSON.parse(data);
            console.log("OnCommentCloudFile:"+dataObj.error);
            if(dataObj && dataObj.error==0){
                uc.modules.comment.CommentManager.initChatComment(option.fileId,cloudData,option);
            }else{
                var content=uc_resource.MessageBox.notPermission;
                if(dataObj.error==40402){
                    content=uc_resource.MessageBox.notFileInfo;
                }
                uc.modules.component.PromptMessageBox.open(
                    {
                        title: uc_resource.MessageBox.Notification,
                        message: content,
                        buttonMode:"ok",
                        OkText:uc_resource.MessageBox.OK
                    },
                    function(){
                        this.close();
                });
            }
        },
        OnCloudFilePreviewer:function(data, cloudData){
            var dataObj=JSON.parse(data);
            if(data.hasOwnProperty("error")){
                var content=uc_resource.MessageBox.notPermission;
                if(dataObj.error==40402){
                    content=uc_resource.MessageBox.notFileInfo;
                }
                uc.modules.component.PromptMessageBox.open({title: uc_resource.MessageBox.Notification, message: content, buttonMode:"ok",OkText:uc_resource.MessageBox.OK},
                    function(){
                        this.close();
                    });
            }else{
                uc.IUIEvent.OnCloudFilePreviewer(cloudData);
            }
        },
        OnDownloadCloudFile:function(data,cloudData,that){
            var self=this;
            var dataObj=JSON.parse(data);
            var cloudDataObj=JSON.parse(cloudData);
            if(dataObj.hasOwnProperty("error")){
                var content=uc_resource.MessageBox.notPermission;
                if(dataObj.error==40402){
                    content=uc_resource.MessageBox.notFileInfo;
                    if(cloudDataObj.dir==1){
                        content=uc_resource.MessageBox.notFileInfoDir;
                    }
                }
                console.log("OnDownloadCloudFile:"+dataObj.error);
                uc.modules.component.PromptMessageBox.open({title: uc_resource.MessageBox.Notification, message: content, buttonMode:"ok",OkText:uc_resource.MessageBox.OK},
                    function(){
                        this.close();
                    });
            }else{
                var type=that.attr("type");
                uc.modules.chat.ChatManager.downloadCloudDir(cloudDataObj,function(data,savePath){
                    self.OnStartDownloadCloudFile(that,data,savePath);
                },type);
            }
        },
        OnStartDownloadCloudFile:function(that,data,savePath){
            var gkData=JSON.parse(data);
            var taskId=gkData.taskid;
            var gkItem="gkDown"+taskId;
            var msgId = that.parents("li").attr("id");
            var fileId=that.parents("li").attr("fileId");
            that.attr("status","cancel");
            uc.util.LogUtil.info('MessageBox', 'OnStartDownloadCloudFile msgId= ' + msgId,'fileId='+fileId,'gkFileData='+data);
            uc.modules.chat.ChatManager.cloudFileInfoDownloading[taskId] = {
                msgId:msgId,
                taskId:taskId,
                gkItem:gkItem,
                fileId:fileId
            };
            uc.modules.chat.ChatManager.fileDownLoadInfo[taskId]={
                fileSavePath:savePath,
                hash:savePath,
                msgId:msgId
            };
            that.siblings().hide();
            that.text(uc_resource.MessageBox.cancel);
            that.parents("li").find(".op-progress").show().attr("id",gkItem);
        },
        OnGoToCloud:function(data,cloudData){
            var dataObj=JSON.parse(data);
            var cloudDataObj=JSON.parse(cloudData);
            if(dataObj.hasOwnProperty("error")){
                var content= uc_resource.MessageBox.notPermission;
                if(dataObj.error==40402){
                    content=uc_resource.MessageBox.notFileInfo;
                    if(cloudDataObj.dir==1){
                        content=uc_resource.MessageBox.notFileInfoDir;
                    }
                }
                uc.modules.component.PromptMessageBox.open({title: uc_resource.MessageBox.Notification, message: content, buttonMode:"ok",OkText:uc_resource.MessageBox.OK},
                    function(){
                        this.close();
                    });
            }else{
                uc.IUIService.changeModule(uc.constants.UI.ModuleId.Cloud,cloudData);
            }
        },
        forwardCloudFile:function(dataInfo,cloudInfo){
            var data=JSON.parse(dataInfo);
            var cloudFileInfoArr= JSON.parse(cloudInfo);
            var fileName=cloudFileInfoArr.filename;
            var fileSize=cloudFileInfoArr.filesize;
            if(data.hasOwnProperty("error")){
                var content=uc_resource.MessageBox.notForward;
                if(data.error==40402){
                    content=uc_resource.MessageBox.notFileInfo;
                    if(cloudFileInfoArr.dir==1){
                        content=uc_resource.MessageBox.notFileInfoDir;
                    }
                }
                uc.modules.component.PromptMessageBox.open({title: uc_resource.MessageBox.Notification, message:content, buttonMode:"ok",OkText:uc_resource.MessageBox.OK},
                    function(){
                        this.close();
                    });
            }else{
                uc.modules.chat.MessageManager.forwardCloudFileMessage(fileName, fileSize,null,dataInfo,cloudFileInfoArr);
            }
        },
        cloudFileDownloadFailPrompt:function(){
            uc.modules.component.PromptMessageBox.open({title: uc_resource.MessageBox.Notification, message: uc_resource.MessageBox.cloudLoadFail, buttonMode:"ok",OkText:uc_resource.MessageBox.OK},
                function(){
                    this.close();
                });
        },
        OnChatCloudFileDownloaded:function(arg){
            if(arg){
                var taskId=arg.detail.taskid;
                var hash=arg.detail.hash;
                if(uc.modules.chat.ChatManager.fileDownLoadInfo[taskId]){
                    var localPath=uc.modules.chat.ChatManager.fileDownLoadInfo[taskId].fileSavePath;
                    var msgId=uc.modules.chat.ChatManager.fileDownLoadInfo[taskId].msgId;
                }
                var parentObj=$("#"+msgId);
                var opObj=$("#gkDown"+taskId,parentObj);
                var percent=arg.detail.percent;
                uc.util.LogUtil.info('MessageBox', 'gkTransfer = ' + JSON.stringify(arg.detail),"localPath:"+localPath,"msgId:"+msgId);
                if(arg.detail.hasOwnProperty("error")){
                    this.deleteCloudFileInfoDownloading(taskId);
                    parentObj.find(".cloudDow").text(uc_resource.MessageBox.saveAs).removeAttr("status").show();
                    parentObj.find(".cloudReceive").text(uc_resource.MessageBox.receive).removeAttr("status").show();
                    parentObj.find(".cloudReceive").siblings().show();
                    parentObj.find(".op-progress,.cloudOpenDir,.cloudEndLine").hide();
                    opObj.attr("value",0);
                    if(arg.detail.error==15){
                        uc.modules.component.PromptMessageBox.open(
                            {title: uc_resource.MessageBox.Notification,
                             message: uc_resource.MessageBox.LocalSpaceNotEnough,
                             buttonMode:"ok",OkText:uc_resource.MessageBox.OK},
                            function(){
                                this.close();
                            });
                    }
                }else{
                    if(uc.modules.chat.ChatManager.cloudFileInfoDownloading[taskId]){
                        var id=uc.modules.chat.ChatManager.cloudFileInfoDownloading[taskId].gkItem;
                        if(!$("#"+id).length){
                            var that=parentObj.find(".cloudDow");
                            that.siblings().hide();
                            that.text(uc_resource.MessageBox.cancel).attr("status","cancel");
                            parentObj.find(".op-progress").show().attr("id",id);
                        }
                    }
                    if(percent==100){
                        uc.modules.chat.ChatManager.downloadedFiles[hash]=localPath;
                        this.deleteCloudFileInfoDownloading(taskId);
                        parentObj.find(".cloudDow").text(uc_resource.MessageBox.open).attr("local-path",localPath).removeAttr("status");
                        parentObj.find(".cloudOpenDir").attr("local-path",localPath);
                        parentObj.find(".cloudPanelRight a,.cloudPanelRight span").show();
                        parentObj.find(".cloudPanelRight .cloudReceive").hide();
                        parentObj.find(".cloudPanelRight .dir-line").hide();
                        parentObj.find(".cloudPanelRight .cloudEndLine").hide();
                        parentObj.find(".op-progress").hide().attr("value",0);
                    }else{
                        parentObj.find(".op-progress").attr("value",percent);
                    }
                }
            }
        },
        deleteCloudFileInfoDownloading:function(taskId){
            if(uc.modules.chat.ChatManager.cloudFileInfoDownloading[taskId]){
                delete uc.modules.chat.ChatManager.cloudFileInfoDownloading[taskId];
            }
        },
        forwardCode:function(msgId){
            var that=$("#"+msgId).find(".forward");
            var codeHead = that.attr("text");
            var codeLine = that.attr("lines");
            var codeDesc= decodeURIComponent(that.attr("des"));
            var obj = $('.download', that.parent());
            var langType = obj.attr("data-type");
            var codeTitle = obj.attr("data-fname");
            var codeURL = obj.attr("data-url");
            var createTime = $('.comment', that.parent()).attr("file-tm");
            var codeSize = $('.comment', that.parent()).attr("file-size");
            uc.modules.chat.MessageManager.forwardCodeMessage(codeTitle, langType, codeHead,
                codeDesc, codeSize, codeURL, codeLine, createTime);
        },
        previewCode:function(msgId) {
            var that=$("#"+msgId).find(".preview");
            var url = that.attr("data-url");
            var obj = $('.download', that.parent());
            var fileName = obj.attr("data-fname");
            var type = obj.attr("data-type");
            var fileSize = $('.comment', that.parent()).attr("file-size");
            uc.modules.chat.ChatManager.showCodePreview(url, fileName, fileSize, type);
        },
        downloadCode: function(msgId){
            var that=$("#"+msgId).find(".download");
            var localPath = that.attr("local-path");
            if (localPath != null && localPath != "") {
                uc.util.FileUtil.openFile(localPath);
                return;
            }
            var url = that.attr("data-url");
            uc.modules.chat.ChatManager.dowloadCode(msgId, url);
        },
        bindResize: function (parent) {
            var self = this;
            var middleLine =  $('.middle-line', parent);
            middleLine.on("mousedown",
                function() {
                    var inputShadow = $(".input-shadow", $(".message-input-box", parent));
                    var htmlEditor = $('.html-editor:visible');
                    document.onmousemove = function(e) {
                        var iT =  document.body.clientHeight - e.pageY;
                        iT < 130 && (iT = 130);
                        iT > 400 && (iT = 400);

                        $(".message-box-container", parent).css({'bottom': iT  + "px"});
                        middleLine.css({'bottom': iT - 5 + "px"});
                        $(".chat-input", parent).css({'height': iT - 5  + "px"});
                        self.parent.msgInputBox.updateInputBoxPosition();
                        return false;
                    };
                    document.onmouseup = function() {
                        self.parent.msgInputBox.updateInputBoxPosition();
                        htmlEditor.show();
                        inputShadow.hide();
                        document.onmousemove = null;
                        document.onmouseup = null;
                        middleLine.releaseCapture &&  middleLine.releaseCapture();
                        self.parent.msgInputBox.focusInput();
                    };
                    htmlEditor.hide();
                    inputShadow.html(self.parent.msgInputBox.getHtmlContent());
                    inputShadow.show();
                    this.setCapture && this.setCapture();
                    return false;
                }
            );
        },
        unBindResize: function (parent) {
            var middleLine =  $('.middle-line', parent);
            middleLine.css({'cursor':'default'});
            middleLine.unbind();
            this.moreMsgLine.addClass('hide');
        },
        getApiRequestUrl: function(url, auth) {
            if (auth == '0') {
                return url;
            }
            var account =  uc.modules.chat.ChatManager.myUserInfo.account;
            var token = uc.modules.chat.ChatManager.token;
            var _join = (url.indexOf("?") >= 0 ? '&' : '?');
            return url + _join + 'uc_account=' + account +'&uc_token=' + token;
        },
        updateApiActionStatus: function(seq, status, filterCondition, approveSuggestion, index) {
            if( this.chatList != null && seq != null && status != null) {
                var msgItem = this.chatList.find("li[seq=" + seq + "]");
                var oaMsgObj = this.parent.getOAMsgObj(seq);
                if (approveSuggestion) {
                    oaMsgObj.oaExternalData = approveSuggestion;
                }
                if (msgItem.length > 0 && oaMsgObj != null) {
                    var domStr = this.getApiOAMsgDomStr(oaMsgObj.msgId, seq, oaMsgObj.title, oaMsgObj.titleColor,
                        oaMsgObj.titleBkColor, oaMsgObj.titleElements, oaMsgObj.titleStyle, status, oaMsgObj.data, oaMsgObj.detailUrl, oaMsgObj.detailAuth, oaMsgObj.oaExternalData);
                    msgItem.before(domStr);
                    msgItem.remove();
                }
            }
            var opStatus = this.parent.getApiOpStatusBySeq(seq);
            if (opStatus != null) {
                this.updateApiActionOpStatus(seq, opStatus, index);
            }
        },
        updateApiActionOpStatus: function(seq, opStatus, index) {
            if( this.chatList == null || seq == null || opStatus == null) {
                return;
            }
            var msgItem = this.chatList.find("li[seq=" + seq + "]");
            if (msgItem.length <= 0) {
                return;
            }

            if(index == undefined || index == null){
                $(".opstatus", msgItem).attr("title", "");
                $(".opstatus", msgItem).removeClass("busy");
                $(".opstatus", msgItem).removeClass("fail");
                $(".oaBtn", msgItem).addClass("normal");
                $(".oamore", msgItem).addClass("normal");
                if (opStatus == "busy"){
                    $(".opstatus", msgItem).addClass("busy");
                    $(".oaBtn", msgItem).removeClass("normal");
                    $(".oamore", msgItem).removeClass("normal");
                } else if (opStatus == "fail") {
                    $(".opstatus", msgItem).attr("title", uc_resource.Common.RequestFailed);
                    $(".opstatus", msgItem).addClass("fail");
                }
            }else{
                $(msgItem).find(".opstatus[data-button-num="+ index + "]").attr("title", "");
                $(msgItem).find(".opstatus[data-button-num="+ index + "]").removeClass("busy");
                $(msgItem).find(".opstatus[data-button-num="+ index + "]").removeClass("fail");
                $(msgItem).find(".oaBtn[data-button-num="+ index + "]").addClass("normal");
                $(msgItem).find(".oamore").eq(index).addClass("normal");
                if (opStatus == "busy"){
                    $(msgItem).find(".opstatus[data-button-num="+ index + "]").addClass("busy");
                    $(msgItem).find(".oaBtn[data-button-num="+ index + "]").removeClass("normal");
                    $(msgItem).find(".oamore").removeClass("normal");
                } else if (opStatus == "fail") {
                    $(msgItem).find(".opstatus[data-button-num="+ index + "]").attr("title", uc_resource.Common.RequestFailed);
                    $(msgItem).find(".opstatus[data-button-num="+ index + "]").addClass("fail");
                }
            }
        },

        onBodyClick: function() {
            $('.left-menu', this.node).hide();
            $('.api-menu', this.node).hide();
            this.hideRightMenu();
        },

        setImagSeq: function(msgId, msgSeq) {
            var msgObj = $('#' + msgId, this.chatList);
            if(msgObj.length <= 0) {
                return;
            }
            var imgObj = $(".msg-img", msgObj);
            if (imgObj.length <= 0) {
                return;
            }
            imgObj.attr("seq", msgSeq);
        },

        setChatIdAndType: function(chatId, chatType){
            this.chatId = chatId;
            this.chatType = chatType;
        },

        setApiInfo: function(appInfo) {
            this.appInfo = appInfo;
        },

        replaceMsgId: function(oldMsgId, msgId) {
            var fmsgObj = $('#' + oldMsgId, this.chatList);

            uc.modules.chat.ChatManager.FSFileInfo[msgId].fileURL = fmsgObj.find(".op-receive").attr("server-path");

            fmsgObj.attr("id", msgId);
            $(".send-fail", fmsgObj).attr("id", 'sf' + msgId);
            $(".send-fail", fmsgObj).attr("msgId", msgId);
            $(".message-time", fmsgObj).attr("id", 'tm' + msgId);
        },
        OnFileUploading: function(msgId, percent){
            var fmsgObj = $('#' + msgId, this.chatList);
            var progressObj =  $(".op-progress", fmsgObj);
            if (progressObj.length <= 0) {
                return;
            }
            var opObj = $(".op-receive", fmsgObj);
            progressObj.show();
            progressObj.attr("value", percent);
            fmsgObj.find(".file-list-panel-menu span,.file-list-panel-menu a").hide();
            opObj.text(uc_resource.MessageBox.cancel).show().attr('op-status', "uploading");
        },
        OnFileUploaded: function(errorCode, msgId, filePath, downloadURL){
            var fmsgObj = $('#' + msgId, this.chatList);
            var imgObj = $(".msg-img", fmsgObj);
            if (imgObj.length > 0) {
                var needToBottom = false;
                if (this.node != null && this.node[0] != null &&
                    this.node[0].scrollTop + this.node.height() + 20 >= this.node[0].scrollHeight) {
                    needToBottom = true;
                }
                imgObj.attr("status", "open");
                imgObj.attr("src", filePath);
                imgObj.attr("data-url", downloadURL);
                imgObj.css({cursor:"pointer"});
                if (needToBottom) {
                    var This = this;
                    setTimeout(
                        function() {
                            This.scrollToEnd();
                        }, 100
                    )
                }
            } else {
                var progressObj =  $(".op-progress", fmsgObj);
                var opObj = $(".op-receive", fmsgObj);
                if (errorCode != 0) {
                    return;
                }
                opObj.text(uc_resource.MessageBox.open).attr('op-status', "open");
                opObj.attr('local-path', filePath);
                opObj.attr('server-path', downloadURL);
                progressObj.hide().attr("value", 0);
                fmsgObj.find(".file-list-panel-menu span,.file-list-panel-menu a").show();
                fmsgObj.find(".op-saveAs").hide();
                fmsgObj.find(".op-comment,.op-comment-span").hide();
                fmsgObj.find(".op-saveAs").next().hide();
				if(!fmsgObj.find('.op-openfolder').length)
					fmsgObj.find(".op-receive").after('<span></span><a class="op-openfolder">'+uc_resource.MessageBox.openDir+'</a>');
            }
        },
        onFailedTip:function(taskId){
            this.node.find(".message-item[id=upload_"+taskId+"]").append(this.faildTip);
            $(".messageBox-failTip").show();
        },
        onFileIdUpdate: function(msgId, fileId, uploadTime, url){
            var fmsgObj = $('#' + msgId, this.chatList);
            var obj = $(".op-comment", fmsgObj);
            obj.attr("file-id", "" + fileId);
            obj.attr("file-tm", "" + uploadTime);
            $(".op-saveAs", fmsgObj).attr("data-url", url);
            obj.show();
            $(".op-comment-span,.cloudComment,.cloudComment-span", fmsgObj).show();
            $(".cloudComment", fmsgObj).attr("file-id",fileId);
            $(".line-2", fmsgObj).show();
            var codeObj = $(".comment", fmsgObj);
            codeObj.attr("file-id", "" + fileId);
            codeObj.attr("file-tm", "" + uploadTime);
            codeObj.show();
            $(".line-3", fmsgObj).show();
            if(uc.modules.chat.ChatManager.FSFileInfo[msgId]){
                uc.modules.chat.ChatManager.FSFileInfo[msgId].fileId=fileId;
            }
        },

        OnFileDownloading: function(msgId, percent,fromDomObj){
            var fmsgObj = $('#' + msgId, this.chatList);
            var progressObj =  $(".op-progress", fmsgObj);
            if(fromDomObj!=null)
            {
                fmsgObj = $('#' + msgId, fromDomObj);
                progressObj =  $(".op-progress", fmsgObj);
            }
            if (progressObj.length <= 0) {
                return;
            }
            var opObj = $(".op-receive", fmsgObj);
            fmsgObj.find(".file-list-panel-menu span,.file-list-panel-menu a").hide();
            opObj.text(uc_resource.MessageBox.cancel).show().attr('op-status', "downloading");
            progressObj.show().attr("value", percent);
        },
        OnFileDownloaded: function(errorCode, msgId, savePath,fromDomObj){
            var fmsgObj = $('#' + msgId, this.chatList);
            var imgObj = $(".msg-img", fmsgObj);
            var codeObj = $(".operation .download", fmsgObj);
            var audioObj = $(".play-back", fmsgObj);
            if(fromDomObj!=null)
            {
                 fmsgObj = $('#' + msgId, fromDomObj);
                if(fmsgObj.attr("seq")!=null)
                {
                    var seq = fmsgObj.attr("seq");
                    fmsgObj = $("div[seq='"+seq+"']", fromDomObj);
                }
                 imgObj = $(".msg-img", fmsgObj);
                 codeObj = $(".operation .download", fmsgObj);
                 audioObj = $(".play-back", fmsgObj);
            }

            if(errorCode != 0){
                uc.util.LogUtil.info("MessageBox","OnFileDownloaded","errorCode",errorCode);
                if(errorCode == 60031){
                    uc.modules.component.PromptMessageBox.open({title: uc_resource.Common.Prompt, message: uc_resource.Message.UnableToDownload, buttonMode:"ok", OkText: uc_resource.Common.Known},
                        function(){
                            uc.modules.chat.ChatManager.cancelDownload(msgId);
                            $(".op-receive",fmsgObj).attr("op-status","downloading");
                            this.close();
                        });
                    return;
                }
                $(".saveFailed",fmsgObj).show();
                $(".op-progress", fmsgObj).hide();
                if($(".op-receive",fmsgObj).attr("op-status") == "downloading"){
                    $(".op-receive",fmsgObj).text(uc_resource.Chat.ReDownload).removeAttr('op-status');
                }
                if($(".op-saveAs",fmsgObj).attr("op-status") == "downloading"){
                    $(".op-saveAs",fmsgObj).text(uc_resource.Chat.ReDownload).removeAttr('op-status');
                }
            }
            if (imgObj.length > 0) {
                if (errorCode != 0) {
                    imgObj.attr("src", "../images/messagebox/placeImgError.gif");
                    imgObj.attr("status", "networkissue");
                    imgObj.css({cursor:"pointer"});
                    return;
                }
                var needToBottom = false;
                if (this.node != null && this.node[0] != null &&
                    this.node[0].scrollTop + this.node.height() + 20 >= this.node[0].scrollHeight) {
                    needToBottom = true;
                }
                imgObj.attr("status", "open");
                imgObj.attr("src", savePath);
                imgObj.css({cursor:"pointer"});
                if (needToBottom) {
                    var This = this;
                    setTimeout(
                        function() {
                            This.scrollToEnd();
                        }, 100
                    )
                }
            } else if(codeObj.length > 0) {
                codeObj.text(uc_resource.MessageBox.open);
                codeObj.attr('local-path', savePath);
            }
            else if (audioObj.length > 0) {
                //播放
                uc.modules.chat.ChatManager.playLocalSound(savePath, msgId);
            }
            else {
                var progressObj =  $(".op-progress", fmsgObj);
                var opObj = $(".op-receive", fmsgObj);
                if (errorCode != 0) {
                    return;
                }
                opObj.text(uc_resource.MessageBox.open).attr('op-status', "open");
                opObj.attr('local-path', savePath);
                progressObj.hide().attr("value", 0);
                fmsgObj.find(".file-list-panel-menu span,.file-list-panel-menu a").show();
                $(".saveFailed",fmsgObj).hide();
                fmsgObj.find(".op-saveAs").hide();
                fmsgObj.find(".op-saveAs").next().hide();
				if(!fmsgObj.find('.op-openfolder').length)
					fmsgObj.find(".op-receive").after('<span></span><a class="op-openfolder">'+uc_resource.MessageBox.openDir+'</a>');
            }
        },
        OnChatImageDownloaded: function(localUrl, msgId){
            var fmsgObj = $('#' + msgId, this.chatList);
            var imgObj = $(".msg-img", fmsgObj);
            if (imgObj.length > 0) {
                var needToBottom = false;
                if (this.node != null && this.node[0] != null &&
                    this.node[0].scrollTop + this.node.height() + 20 >= this.node[0].scrollHeight) {
                    needToBottom = true;
                }
                imgObj.attr("status", "open");
                imgObj.attr("src", localUrl);
                if (needToBottom) {
                    var This = this;
                    setTimeout(
                        function() {
                            This.scrollToEnd();
                        }, 100
                    )
                }
            }
        },
        fileMsgOperation: function(self,fsInfo,type) {
            var url =fsInfo.fileURL;
            var msgId = fsInfo.msgId;
            var fname =fsInfo.fileName;
            var status =self.attr('op-status');
            var itemLi=self.parents("li");
            if (status == null) {   //下载
                uc.modules.chat.ChatManager.downloadFile(self,msgId, url, fname,
                    function(){
                        uc.util.LogUtil.info('MessageBox','downloadFile','filename',fname);
                        itemLi.find(".op-progress").show();
                        self.text(uc_resource.MessageBox.cancel).attr('op-status','downloading');
                        self.siblings().hide();
                        self.siblings(".saveFailed").hide();
                    },type);
            } else if (status == "downloading") { //取消下载
                uc.modules.chat.ChatManager.cancelDownload(msgId);
                itemLi.find(".op-receive").text(uc_resource.MessageBox.receive).removeAttr("op-status");
                itemLi.find(".op-saveAs").text(uc_resource.MessageBox.saveAs).removeAttr("op-status");
                itemLi.find(".op-progress").hide();
                self.siblings().show();
                self.siblings(".saveFailed").hide();
            } else if (status == "open") {  //打开
                var path = self.attr('local-path');
                if ((path == null || path == "")||(uc.util.FileUtil.openFile(path) != 0)){
                    uc.modules.component.PromptMessageBox.open({title: uc_resource.Common.Prompt, message: uc_resource.Message.OpenFileFailed, buttonMode:"ok", OkText: uc_resource.Common.Known},
                        function(){
                            this.close();
                        });
                }
            } else if (status == "uploading") { //取消上传
                uc.modules.chat.ChatManager.cancelUpload(msgId);
                itemLi.remove();
            }
         },
        downloadImg: function() {
            var _this = this;
            $(".msg-img", this.chatList).each(function() {
                var imgObj = $(this);
                var status = imgObj.attr('status');
                if (status == null) {   //下载
                    var url = imgObj.attr('data-url');
                    var msgId = imgObj.attr('data-id');
                    var seq = imgObj.attr('seq');
                    var name = imgObj.attr('name');
                    uc.modules.chat.ChatManager.downloadImg(_this.chatId, _this.chatType, msgId, seq, url, name,
                        function() {
                            imgObj.attr('status', 'downloading');
                        }
                    );
                } else if (status == "link") {
                    $(this).css({cursor:"pointer"});
                }
            });
        },
        onMoreMsgLoading: function(){
            this.moreMsgLoading.show();
            this.moreMsg.hide();
            uc.util.LogUtil.info("MessageBox","onMoreMsgLoading","jquery element", {moreMsgLoading: this.moreMsgLoading, moreMsg: this.moreMsg});
        },
        onMoreMsgLoadEnd: function(){
            this.moreMsgLoading.hide();
            this.moreMsg.show();
            uc.util.LogUtil.info("MessageBox","onMoreMsgLoadEnd","jquery element", {moreMsgLoading: this.moreMsgLoading, moreMsg: this.moreMsg});
        },
        onLoadAllMsgEnd: function(){
            this.moreMsgLine.hide();
            uc.util.LogUtil.info("MessageBox","onLoadAllMsgEnd","jquery element", {moreMsgLoading: this.moreMsgLoading, moreMsg: this.moreMsg});
        },
        clearMessage : function(){
            this.chatList.empty();
        },
        appendMessage : function(DOMStr){
            var  needToBottom = false;
            if (this.node != null && this.node[0] != null &&
                this.node[0].scrollTop + this.node.height() + 500 >= this.node[0].scrollHeight) {
                needToBottom = true;
            }
            this.chatList.append(DOMStr);
            this.refreshSysyTimeDom();
            this.downloadImg();
            if (needToBottom) {
                this.scrollToEnd();
            }
        },
        prependMessage : function(DOMStr){
            if (!this.node) {
                return;
            }
            var h = this.node[0].scrollHeight;
            this.chatList.prepend(DOMStr);
            this.refreshSysyTimeDom();
            this.downloadImg();
            this.node.scrollTop(this.node[0].scrollHeight - h);
        },
        scrollToEnd: function(){
            var This = this;
            if (this.node != null && this.node[0] != null) {
                this.node.scrollTop(this.node[0].scrollHeight);
                setTimeout(
                    function() {
                        This.node.scrollTop(This.node[0].scrollHeight);
                    }, 100
                )
            }
        },

        updateUserLog: function(uid, logo){
            if( this.chatList != null) {
                //如果是超级群，显示随机图像
                if ((this.chatType == uc.constants.Chat.ChatType.GroupChat ||
                    this.chatType == uc.constants.Chat.ChatType.ProjectChat || this.chatType == uc.constants.Chat.ChatType.CloudChat) && uc.modules.chat.ChatManager.groupInfo && uc.modules.chat.ChatManager.groupInfo.memberType) {
                    return;
                }
                this.chatList.find("img[rel=logo" + uid + "]").attr("src", logo);
            }
        },
        updateUserName: function(uid, name){
            if( this.chatList != null) {
                //如果是超级群，显示姓氏
                if ((this.chatType == uc.constants.Chat.ChatType.GroupChat ||
                    this.chatType == uc.constants.Chat.ChatType.ProjectChat) && uc.modules.chat.ChatManager.groupInfo && uc.modules.chat.ChatManager.groupInfo.memberType) {
                }
                this.chatList.find("span[rel=" + uid + "]").text(name);
            }
        },
        updateMsgTime: function(msgId, msgTime) {
            $('#tm' + msgId, this.chatList).text(msgTime);
        },
        isMsgIdExist: function(msgId){
            return $('#' + msgId, this.chatList).length > 0;
        },
        showSendFailFlag: function(msgId, bshow) {
            if (bshow) {
                $('#sf' + msgId, this.chatList).attr("src", "../images/messagebox/sendfail.png");
                $('#sf' + msgId, this.chatList).show();
                $('#sf' + msgId, this.chatList).attr("state", "sendfail");
            } else {
                $('#sf' + msgId, this.chatList).hide();
                $('#sf' + msgId, this.chatList).attr("state", "sendok");
            }
        },
        updateMessageSeq:function(msgId,seq){
            this.chatList.find(".message-item[id="+msgId+"]").attr("seq",seq);
        },
        showSendingFlag: function(msgId) {
            $('#sf' + msgId, this.chatList).attr("src", "../images/messagebox/sending.gif");
            $('#sf' + msgId, this.chatList).attr("state", "sending");
            $('#sf' + msgId, this.chatList).show();
        },
        refreshSysyTimeDom: function() {
            this.chatList.find("li[rel=sys-time-item]").remove();
            var items = $("li", this.chatList);
            var lastseq = 0;
            for (var i = 0; i < items.length; i++){
                var seqAttr = $(items[i]).attr("seq");
                if (seqAttr == null) {
                    continue;
                }
                var seq = Number(seqAttr);
                if (lastseq == 0 || new Date(lastseq).toDateString() != new Date(seq).toDateString()) {
                    var dom = this.getSysTimeMsgDomStr(seq);
                    $(items[i]).before(dom);
                    lastseq = seq;
                }
            }
        },
        OnSoundPlaying: function(msgId, percent, seconds){
	        if(!msgId){
		        return;
	        }
            var obj = $('#' + msgId, this.chatList);
            if (obj.length <= 0) {
                return;
            }
            seconds = parseInt(Number($('.audio-time', obj).attr("senconds")));
            var waveHtml, tmstr;
            if (percent >= 100) {
                $('.play-btn', obj).removeClass("playing");
                waveHtml = this.getAudioHtml(seconds, 0);
                tmstr = uc.util.TimeUtil.getAudioTimeStr(seconds);
                $('.audio-time', obj).text(tmstr);
                $('.wave', obj).empty();
                $('.wave', obj).html(waveHtml);
                return;
            }
            $('.play-btn', obj).addClass("playing");
            var s = parseInt(percent*seconds/100);
            waveHtml = this.getAudioHtml(seconds, percent);
            tmstr = uc.util.TimeUtil.getAudioTimeStr(s);
            $('.audio-time', obj).text(tmstr);
            $('.wave', obj).empty();
            $('.wave', obj).html(waveHtml);
        },

        getAudioHtml: function (seconds, precent){
            var waveHeights = [4,8,12,16,20,24,20,16,12,8,4,4,4,8,4,8,4,12,8,16,12,8,8,4,4,4,8,16,12,4,4,8,16,4,4,4,4,8,4,8,4,16,8,16,12,8,8,4,4,4,8,16,12,4,4,12,16,4,4,4,4,8,8,16,20,16,8,4,4,4,8,16,12,4,4,12,16,4,4,4,4,8,8,16,20,16,8,4,4,4,8,16,12,4,4,12,16,4,4,4,4,8,8,16,20,16,8,4,4,4];
            var num = 26;
            if (seconds > 1 && seconds <= 10) {
                num = (seconds - 1)*10/4 + 26;
            } else if (seconds > 10 && seconds <= 20) {
                num = (seconds - 11) + 49;
            } else if (seconds > 20&& seconds <= 60) {
                num = (seconds - 20)/2 + 58;
            } else if (seconds > 60) {
                num = 70;
            }
            if (num > 70) {
                num = 70;
            }
            if (precent > 100 || precent < 0) {
                precent = 0;
            }
            var pNum = precent*num/100;
            var html = "";
            for (var i = 0; i < num; i++){
                var preattr = "";
                if (i < pNum) {
                    preattr = "class='precent'";
                }
                var top = 23 - waveHeights[i]/2;
                html += "<span " + preattr + " style='height:" + waveHeights[i] + "px; margin-top:" + top + "px'></span>";
            }
            return html;
        },
        getUnsendFlagDomStr : function(sendState, self, msgId) {
            if (!self) {
                return "";
            }
            if (sendState == uc.constants.Message.SendState.SendFail) {
                return  '<img class="send-fail" msgId="' + msgId +'" id="sf' + msgId + '" state="sendfail"  src="../images/messagebox/sendfail.png"/>';
            } else if(sendState == uc.constants.Message.SendState.Sending){
                return '<img class="send-fail" msgId="' + msgId +'" id="sf' + msgId + '" src="../images/messagebox/sending.gif"/>'
            } else {
                return '<img class="send-fail" msgId="' + msgId +'" id="sf' + msgId + '" style="display:none" src="../images/messagebox/sendfail.png"/>'
            }
        },
        getSysMsgDomStr: function(content, seq,detailContent,operation,msgId){
            var tag="",img="";
            if(operation==uc.constants.Group.GroupCloudType.CloudFileMove){
                uc.modules.chat.ChatManager.cloudFileInfo[msgId]=detailContent;
                tag='<span class="sys-text-view">'+uc_resource.MessageBox.viewCloud+'</span>';
            }
            if(operation==uc.constants.Group.GroupCloudType.CloudFileDel
                ||operation==uc.constants.Group.GroupCloudType.CloudFileMove
                ||operation==uc.constants.Group.GroupCloudType.CloudFileRename){
                img='<img src="../images/message/cloud_small.png" class="cloud_small"/>';
            }
            return          '<li class="sys-message-item" seq="' + seq + '" id="'+msgId+'">'
                +               '<div class="sys-message-box">'
                +                   '<span class="sys-text">'+img+ content + '</span>'
                +                   tag
                +               '</div>'
                +           '</li>';
        },
        getSysTimeMsgDomStr: function(seq){
            var content = uc.util.TimeUtil.getSysMessageTimeStr(seq);
            return          '<li class="sys-message-item" rel="sys-time-item" >'
                +               '<div class="sys-message-box">'
                +                   '<span class="sys-text">' + content + '</span>'
                +               '</div>'
                +           '</li>';
        },
        getMsgExtDomStr: function(self, from, msgId, msgTime,fileUpdateStr) {
            var tmstr = uc.util.TimeUtil.getMessageTimeString(new Date(msgTime), uc.constants.UI.TimeFormat.Message.HourMinutes, uc.settings.meeting.timeZone.offset);
            var displayName = from.userId;
            if (from.displayName != null) {
                displayName =  from.displayName;
            }
            var fileUpdate="";
            if(fileUpdateStr){
                fileUpdate=fileUpdateStr;
            }
            if(!self) {
                var logoPath = "../images/avatar_default.png", userAvatar;
                if (from.localAvatar != null && from.localAvatar != "") {
                    logoPath = from.localAvatar;// + "?" + new Date().getTime();
                }

                //如果是超级群，显示随机图像
                if ((this.chatType == uc.constants.Chat.ChatType.GroupChat ||
                    this.chatType == uc.constants.Chat.ChatType.ProjectChat || this.chatType == uc.constants.Chat.ChatType.CloudChat) && uc.modules.chat.ChatManager.groupInfo && uc.modules.chat.ChatManager.groupInfo.memberType) {
                    var bgColor,userLogoColor = uc.modules.chat.ChatManager.superGroupMemberBgColor[from.userId];
                    if (userLogoColor) {
                        bgColor = userLogoColor;
                    } else {
                        var randomNumber = parseInt(Math.random() * 10);
                        bgColor = randomNumber > 5 ? this.superGroupBgColor[5] : this.superGroupBgColor[randomNumber];
                        uc.modules.chat.ChatManager.superGroupMemberBgColor[from.userId] = bgColor;
                    }
                    userAvatar = '<div class="user-logo" style="background-color: '+ bgColor +'"  rel="logo' + from.userId + '" >'+ displayName.substring(0, 1) +'</div>';
                } else {
                    userAvatar =  '<img class="user-logo" onerror="if(this.defaultlogo){return;}this.src=\'../images/avatar_default.png\';this.defaultlogo=true;" rel="logo' + from.userId + '" src="' + logoPath + '" />'
                }

                return      '<div class="left-item">'
                +               userAvatar
                +               '<div class = "info">'
                +                   '<span class="user-name" rel="' + from.userId + '">' + displayName + '</span>'
                +                   fileUpdate
                +                   '<span class="message-time" id="tm' + msgId + '">' + tmstr + '</span>'
                +               '</div>'
                +           '</div>';
            }
            if (uc.modules.chat.ChatManager.myUserInfo != null && uc.modules.chat.ChatManager.myUserInfo.displayName != null) {
                var myUserInfo = uc.IContactService.getCurrentUserInfo();
                displayName = myUserInfo.displayName;
            }
            return          '<div class="right-item">'
                +               '<div class = "info">'
                +                   '<span class="unread-info" style="display:none">2人未读</span>'
                +                   '<span class="user-name" rel="' + from.userId + '">' + displayName + '</span>'
                +                    fileUpdate
                +                   '<span class="message-time" id="tm' + msgId + '">' + tmstr + '</span>'
                +               '</div>'
                +           '</div>';
        },
        getTextMsgDomStr: function(sendState, self, from, msgId, msgSeq, textType, textContent, sendTime, msgTime, textFormat){
            textContent = uc.util.EmotionUtil.codeToEmotion(textContent);
            textContent = uc.modules.emoji.EmojiConverter.unicodeToSmiley(textContent);
            textContent = uc.modules.emoji.EmojiConverter.androidCodeToSmiley(textContent);
            textContent = textContent.replace(/\n/g, "<br>");
            textContent = uc.util.StringUtil.matchLink(textContent);

            try{
                var sizeStr =  $('<div style="' + textFormat + '"/>').css("font-size");
                if (sizeStr != null) {
                    var value = Number(sizeStr.replace(/[^0-9]/ig,""));
                    if (value > 0) {
                        textFormat += "; line-height: " + (value + 6) + "px;";
                    }
                }
                var tobj = $('<div style="' + textFormat + '"/>');
                var clr =  tobj.css("color");
                if (clr != null) {
                    tobj.css("color", "");
                    var fmt = tobj.attr("style");
                    if (fmt != null) {
                        textFormat = fmt;
                    }
                }
            } catch(e){}

            var cls = self ? "text-me-to" : "text-to-me";
            var unread=self ? this.unRead:"";
            return          '<li class="message-item" id="'+ msgId +'"  seq="' + msgSeq + '">'

                +'<div>'
                +               this.getMsgExtDomStr(self, from, msgId, msgSeq)
                +               '<div class="' + cls + '" style="' + textFormat + '"><div>' + textContent + '</div>'
                +unread

                +'</div>'
                +               this.getUnsendFlagDomStr(sendState, self, msgId)

                +'</div>'

                +           '</li>';
        },
        getImgMsgDomStr: function(sendState, self, from, msgId, msgSeq, imageName, imageSize, imageURL){
            var url = uc.modules.chat.ChatManager.getLoaclUrl(imageURL);
            imageName = uc.util.StringUtil.htmlEncode(imageName);
            var imgDomStr = '<img class="msg-img" size="' + imageSize + '" data-id="'+ msgId +'" name="' + imageName + '" seq="' + msgSeq + '" data-url="' + imageURL +'" src="../images/messagebox/placeImg.gif"/>';
            if (url != null &&  url != ""){
                imgDomStr = '<img class="msg-img" size="' + imageSize + '" status="open" data-id="'+ msgId +'" style="cursor:pointer;"  name="' + imageName + '" seq="' + msgSeq + '" data-url="' + imageURL +'" src="' + url + '"/>';
            }
            var cls = self ? "img-me-to" : "img-to-me";
            var unread = self ? this.unRead : "";
            return          '<li class="message-item" id="'+ msgId +'"  seq="' + msgSeq + '">'
                +               '<div class="' + cls + '">'
                +               '<div>'
                +  imgDomStr
                +              '</div>'
                +               unread
                +               '</div>'

                +               this.getUnsendFlagDomStr(sendState, self, msgId)
                +               this.getMsgExtDomStr(self, from, msgId, msgSeq)
                +           '</li>';
        },
        getAudioMsgDomStr: function(sendState, self, from, msgId, msgSeq, audioName, audioSize, audioURL, audioDuration){
            var waveHtml = this.getAudioHtml(audioDuration, 0);
            var tmstr = uc.util.TimeUtil.getAudioTimeStr(audioDuration);
            var cls = self ? "audio-me-to" : "audio-to-me";
            //var unread=self ? this.unRead:"";
            return          '<li class="message-item" id="'+ msgId +'"  seq="' + msgSeq + '">'
                +               '<div class="' + cls + '">'
                +                  '<div>'
                +                  '<div class="play-back" seq="' + msgSeq  + '" data-url="' + audioURL + '">'
                +                       '<div class="play-btn"></div>'
                +                  '</div>'
                +                  '<div class="audio-time" senconds="' + audioDuration + '">' + tmstr + '</div>'
                +                  '<div class="wave">'
                +                       waveHtml
                +                  '</div>'
                +                  '</div>'
                +               '</div>'
                +               this.getUnsendFlagDomStr(sendState, self, msgId)
                +               this.getMsgExtDomStr(self, from, msgId, msgSeq)
                +           '</li>';
        },
        getFileMsgDomStr: function(sendState, self, from, msgId, msgSeq, fileName, fileSize, fileURL, timestamp, localPath, fileId, username,conversationType){
            var fileNameEnocde = uc.util.StringUtil.htmlEncode(fileName);
            var size = uc.util.StringUtil.formatFileSize(fileSize);
            var cloudType=uc.util.StorageUtil.getData('cloudType');
            var cloudTokenResult=uc.util.StorageUtil.getData('CloudLoginResult');
            var url = uc.modules.chat.ChatManager.getLoaclUrl(fileURL);
            uc.modules.chat.ChatManager.FSFileInfo[msgId]={
                "msgId":msgId,
                "fileName":fileNameEnocde,
                "fileSize":fileSize,
                "fileURL":fileURL,
                "fileId":fileId
            };
            var hide="display:none";
            if(fileId)hide="display:";
            var domSaveCloud='',domReceive='',domComment="";
            if(cloudType==1 && cloudTokenResult==0 ){
                domSaveCloud='<a class="op-savecloud">'+uc_resource.MessageBox.saveCloud+'</a><span></span>';
            }
            if((localPath!='')&&self){
                domReceive=
                    '<a class="op-receive"  op-status="open" local-path="'+localPath+'">'+uc_resource.MessageBox.open+'</a>' +
                    '<span></span>' +
                    '<a class="op-openfolder" local-path="'+localPath+'">'+uc_resource.MessageBox.openDir+'</a>';
            }else{
                domReceive=
                    '<a class="op-saveAs">'+uc_resource.MessageBox.saveAs+'</a>' +
                    '<span></span>' +
                    '<a class="op-receive">'+uc_resource.MessageBox.receive+'</a>';
            }
            if(conversationType==uc.constants.Chat.ChatType.ProjectChat
                || conversationType==uc.constants.Chat.ChatType.GroupChat){
                domComment=
                    '<a class="op-comment" style="'+hide+'"  file-id="'+fileId+'">'+uc_resource.MessageBox.comment+'</a>' +
                    '<span class="op-comment-span" style="'+hide+'" ></span>';
            }
            var menu=
                domSaveCloud    +
                domComment      +
                '<a class="op-forward">'+uc_resource.MessageBox.forward+'</a>' +
                '<span></span>' +
                domReceive;
            var cls = self ? "file-list-me-to" : "file-list-to-me";
            return      '<li class="message-item" id="'+ msgId +'"  seq="' + msgSeq + '">'
                +             '<div class="'+cls+'">'
                +                   '<div class="file-list-panel-avatar">'
                +                       '<img src="'+ uc.util.StringUtil.getFileIcon(fileNameEnocde) + '">'
                +                        '<span class="file-list-panel-filename">'
                +                    uc.util.StringUtil.getFileDisplayExt(fileNameEnocde) +'</span>'
                +                   '</div>'
                +                   '<div class="file-list-panel-tag">'
                +                   '<div class="file-list-panel-title">'
                +                    '<div class="name">'+fileNameEnocde+'</div>'
                +                    '<div class="fileSize">'+size+'</div>'
                +                   '</div>'
                +                   '<div class="file-list-panel-menu">'
                +                   '<div class="file-list-panel-menu-list">'+menu+'</div>'
                +                   '<progress class="op-progress" style="display: none" value="0" max="100"></progress>'
                +                   '</div>'
                +                    '</div>'
                +               '</div>'
                +               this.getUnsendFlagDomStr(sendState, self, msgId)
                +               this.getMsgExtDomStr(self, from, msgId, msgSeq)
                +       '</li>';
        },
        getCodeMsgDomStr: function(sendState, self, from, msgId, msgSeq, fileName, fileSize, fileURL, type, text, lines, des, timestamp, fileId, username){
            var fileNameEnocde = uc.util.StringUtil.htmlEncode(fileName);
            var textEnocde = uc.util.StringUtil.htmlEncode(text);
            var size = uc.util.StringUtil.formatFileSize(fileSize);
            var show_type =  "code";
            var commentDomstr =  '<div class="line-3" style="display: none"/><span class="comment" file-tm="' + msgSeq + '" file-size="' + fileSize + '" style="display: none;">'+ uc_resource.Chat.Discuss +'</span>';
            if (fileId != null) {
                commentDomstr = '<div class="line-3"/><span class="comment" file-id="' + fileId + '" file-tm="' + msgSeq + '" file-size="' + fileSize + '">'+ uc_resource.Chat.Discuss +'</span>';
            }
            var url = uc.modules.chat.ChatManager.getLoaclUrl(fileURL);
            var domStr = '<span class="download" data-type="' + type + '" data-fname="'+ fileNameEnocde +'" username="'+ username + '" data-url="' + fileURL +'">'+ uc_resource.Chat.SaveAs +'</span>';
            if (url != null &&  url != ""){
                domStr = '<span class="download" data-type="' + type + '" data-fname="'+ fileNameEnocde +'" username="'+ username + '" local-path="' + url + '" data-url="' + fileURL +'">'+ uc_resource.Chat.Open +'</span>'
            }
            var desDom =  '<div class="description" style="display: none"/>';
            if (des != null && des != ""){
                desDom =  '<div class="description">'+ uc_resource.Chat.CodeDesc +'<br/>' +  uc.util.StringUtil.htmlEncode(des) + '</div>';
            }
            desDom = desDom.replace(/\n/g,"<br/>");
            var cls = self ? "code-me-to" : "code-to-me";
            return  '<li class="message-item" id="'+ msgId +'" seq="' + msgSeq + '">'
                +  '<div class="' + cls + '">'
                +     '<div>'
                +       '<img src="../images/fileicons/code.png"/>'
                +       '<div class="type">' + show_type + '</div>'
                +       '<div class="content">'
                +           '<div class="title">' + fileNameEnocde + '</div>'
                +           '<div class="middle">'
                +               '<div class="lan-type">' +  type + '</div>'
                +               '<div class="size">' + size +  '</div>'
                +           '</div>'
                +           '<div class="operation">'
                +               domStr
                +               '<div class="line-1" />'
                +               '<span class="forward" text="' + textEnocde + '" lines="' + lines + '" des="' + encodeURIComponent(des) +' ">'+ uc_resource.Common.Forward +'</span>'
                +               '<div class="line-2" />'
                +               '<span class="preview" data-url="' + fileURL + '">'+ uc_resource.Chat.Preview +'</span>'
                +               commentDomstr
                +           '</div>'
                +       '</div>'
                +       desDom
                +     '</div>'
                +   '</div> '
                +   this.getUnsendFlagDomStr(sendState, self, msgId)
                +   this.getMsgExtDomStr(self, from, msgId, msgSeq)
                + '</li>';
        },
        getCloudFileMsgDomStr: function(sendState, self, from, msgId, msgSeq, fileName, fileSize, fileURL, timestamp, fileId, username,detailContent,conversationType,operation){
            var cloudFileInfo;
            try{
                cloudFileInfo = JSON.parse(detailContent);
            }catch (e){
            }
            var orgName = fileName;
            var fileNameEnocde = uc.util.StringUtil.htmlEncode(fileName);
            var size = uc.util.StringUtil.formatFileSize(fileSize);
            var fileExt= uc.util.StringUtil.getFileDisplayExt(fileName);
            var cloudType=uc.util.StorageUtil.getData('cloudType');
            var cloudTokenResult=uc.util.StorageUtil.getData('CloudLoginResult');
            var hash=cloudFileInfo.hash;
            var url = uc.modules.chat.ChatManager.getLoaclUrl(hash);
            uc.modules.chat.ChatManager.cloudFileInfo[msgId]=detailContent;
            if(conversationType==uc.constants.Chat.ChatType.ProjectChat
                || conversationType==uc.constants.Chat.ChatType.GroupChat){
                uc.modules.chat.ChatManager.FSFileInfo[msgId]={
                    "msgId":msgId,
                    "fileName":orgName,
                    "fileSize":fileSize,
                    "fileURL":fileURL,
                    "fileId":fileId
                };
            }
            var fileSizeStyle="size";
            var hide="display:none";
            if(fileId)hide="display:";
            var domItem, domDirItem;
            if(url){
             domItem='<a class="cloudDow" local-path="'+url+'">'+uc_resource.MessageBox.open+'</a>' +
                 '<span></span>' +
                 '<a class="cloudOpenDir-show" local-path="'+url+'">'+uc_resource.MessageBox.openDir+'</a>';
             domDirItem='<a class="cloudOpenDir-show" local-path="'+url+'">'+uc_resource.MessageBox.open+'</a>';
            }else{
                domItem=
                    '<a class="cloudDow" type="saveAs">'+uc_resource.MessageBox.saveAs+'</a>' +
                        '<span></span>' +
                        '<a class="cloudReceive" type="receive">'+uc_resource.MessageBox.receive+'</a>' +
                        '<span class="cloudEndLine"></span>' +
                        '<a class="cloudOpenDir">'+uc_resource.MessageBox.openDir+'</a>';
                domDirItem=
                    '<a class="cloudDow" type="saveAs">'+uc_resource.MessageBox.saveAs+'</a>' +
                        '<span class="dir-line"></span>' +
                        '<a class="cloudReceive" type="receive">'+uc_resource.MessageBox.receive+'</a>';
            }
            var saveCloudStr="";
            if(cloudType==1 && cloudTokenResult==0){
               saveCloudStr='<a class="cloudSave">'+uc_resource.MessageBox.saveCloud+'</a><span></span>';
            }
            var item;
            //单人聊天和群聊天
            if(conversationType==uc.constants.Chat.ChatType.SingleChat){
                item= saveCloudStr+'<a class="cloudForward">'+uc_resource.MessageBox.forward+'</a><span></span>'+domItem;
            }
            //云库讨论组
            if(conversationType==uc.constants.Chat.ChatType.CloudChat){
                item=
                    '<a class="cloudFilePreviewer">'+uc_resource.MessageBox.cloudFilePreviewer+'</a><span></span><a class="cloudComment" file-id="'+fileId+'">'+uc_resource.MessageBox.comment+'</a><span></span>' +
                        '<a class="cloudForward">'+uc_resource.MessageBox.forward+'</a><span></span>'+domItem;
            }
            //项目组
            if(conversationType==uc.constants.Chat.ChatType.ProjectChat || conversationType==uc.constants.Chat.ChatType.GroupChat || conversationType==uc.constants.Chat.ChatType.MeetingChat){
                item=
                    saveCloudStr +
                        '<a class="cloudComment" style="'+hide+'" file-id="'+fileId+'">'+uc_resource.MessageBox.comment+'</a><span class="cloudComment-span" style="'+hide+'"></span>' +
                        '<a class="cloudForward">'+uc_resource.MessageBox.forward+'</a><span></span>'+domItem;
            }
            var img='<img src="'+ uc.util.StringUtil.getFileIcon(fileNameEnocde) + '">';
            if(cloudFileInfo.dir==1){
                item='<a class="ViewCloudFile">'+uc_resource.MessageBox.view+'</a>' +
                    '<span></span>' +
                    domDirItem;
                fileSizeStyle="size hide";
                fileExt="";
                img='<img id="fileClass" src="'+ uc.util.StringUtil.getFileIcon("file.file") + '">';
            }
            if(operation==uc.constants.Group.GroupCloudType.CloudFileUpdate){
                var updateFileName=cloudFileInfo.dir ? uc_resource.MessageBox.updateFileDir :uc_resource.MessageBox.updateFile;
                var fileUpdateStr='<span class="updateFile" >'+updateFileName+'</span>';
            }
            var cls = self ? "file-me-to-cloud" : "file-to-me-cloud";
            return          '<li class="message-item" id="'+ msgId +'"  seq="' + msgSeq + '"  fileId="'+fileId+'">'
                +               '<div class="' + cls + '">'
                +                   img+'<div class="file-ext">' + fileExt+ '</div>'
                +                   '<span class="name">' + fileNameEnocde + '</span>'
                +                   '<span class="'+fileSizeStyle+'">' + size + '</span>'
                +               '<div class="cloudPanelRight">'
                +               item
                +               '</div>'
                +               '<progress class="op-progress" style="display:none" value="0" max="100"></progress>'
                +               '</div>'
                +               this.getUnsendFlagDomStr(sendState, self, msgId)
                +               this.getMsgExtDomStr(self, from, msgId, msgSeq,fileUpdateStr)
                +           '</li>';
        },
        getCommentMsgDomStr: function(sendState, self, from, msgId, msgSeq, fileId, fileType, fileTitle, commentId, userName, userId, content, createTime, timestamp,cloudType) {
            var icon;
            if(cloudType == 0 && fileType==1){
                icon = "../images/fileicons/code.png";
            }else{
               icon = uc.util.StringUtil.getFileIcon(fileTitle);
            }
            content = uc.util.StringUtil.htmlEncode(content);
            var cls = self ? "comment-me-to" : "comment-to-me";
            return          '<li class="message-item" id="'+ msgId +'"  seq="' + msgSeq + '">'
                +               '<div class="' + cls + '">'
                +                   '<img class="file-icon" src="'+ icon + '"/>'
                +                   '<span class="name">' + fileTitle + '</span>'
                +                   '<div class="sys_comment" fileId="' + fileId + '">&gt;</div>'
                +                   '<img class="flag" src="../images/messagebox/comment.png"/>'
                +                    '<div class="content">' + content + '</div>'
                +               '</div>'
                +               this.getUnsendFlagDomStr(sendState, self, msgId)
                +               this.getMsgExtDomStr(self, from, msgId, msgSeq)
                +           '</li>';
        },
        getApiTextMsgDomStr: function(sendState, self, from, msgId, msgSeq, textTitle, textContent, detailType, detailContent, sendTime, detailAuth){
            textContent = uc.util.EmotionUtil.codeToEmotion(textContent);
            textContent = uc.modules.emoji.EmojiConverter.unicodeToSmiley(textContent);
            textContent = uc.modules.emoji.EmojiConverter.androidCodeToSmiley(textContent);
            textContent = uc.util.StringUtil.escapeHtml(textContent);
            textContent = textContent.replace(/\n/g, "<br>");
            textContent = uc.util.StringUtil.matchLink(textContent);
            var detailDom = "";
            if (detailContent != "") {
                detailDom =  '<div class="detail" tp="' + detailType + '" detailauth="'+ detailAuth +'" ct="' + detailContent + '">&gt;</div>';
            }
            return          '<li class="message-item" id="'+ msgId +'"  seq="' + msgSeq + '">'
                +               this.getMsgExtDomStr(self, from, msgId, msgSeq)
                +               '<div class="api-text-to-me">'
                +                   '<div class="top">'
                +                       '<div class="title">' + uc.util.StringUtil.htmlEncode(textTitle) + '</div>'
                +                       detailDom
                +                   '</div>'
                +                   '<div class="content">' + textContent + '</div>'
                +               '</div>'
                +               this.getUnsendFlagDomStr(sendState, self, msgId)
                +           '</li>';
        },

        getApiOAMsgDomStr: function(msgId, msgSeq, title, titleColor, titleBkColor, titleElements, titleStyle, status, data, detailUrl, detailAuth, oaExternalData) {
            var self = this, oaData = oaExternalData, opinion = '';
            var getTitleBkColor =
                function(titleBkColor) {
                    if (Object.prototype.toString.call(titleBkColor) == "[object String]") {
                        var titleBkColorLower = titleBkColor.toLowerCase(), titleElementObj = {};
                        if (titleElements) {
                            titleElementObj = JSON.parse(titleElements);
                            for(var i in titleElementObj){
                                if (titleElementObj[i].status == status) {
                                    titleBkColorLower = titleElementObj[i].color;
                                    title = titleElementObj[i].title || uc_resource.OpenApi.OpenApiDefaultTitle;
                                }
                            }
                        }
                        var colors = {
                            "red": "#f96868",
                            "yellow": "#e5c106",
                            "green": "#4bd963",
                            "gray": "#c3c3c3",
                            "blue": "#4e667a",
                            "lightblue": "#6fafe2",
                            "white": "#ffffff"
                        };
                        if (colors[titleBkColorLower] != null) {
                            return colors[titleBkColorLower];
                        }
                    }
                    return colors.blue;
                } ;

            //辅助拼装elements dom字符串的函数
            var getTextContentDomStr =
                function(content) {
                    if (content == null || content.text == "") {
                        return "";
                    }
                    var fontClass = "";
                    if (content.size == 1) {
                        fontClass = "smallfont";
                    } else if (content.size == 3) {
                        fontClass = "bigfont";
                    }
                    var style = "";
                    if (content.bold == 1) {
                        style = style + "font-weight:bold;";
                    }
                    if (content.italic == 1) {
                        style = style + "font-style:italic;";
                    }
                    if (content.color != null && content.color != "") {
                        style = style + 'color:' + content.color + ';';
                    }
                    var textContent = content.text ? content.text : '';
                    textContent = textContent.replace(/\n/g, "uc_special_ch_enter");
                    textContent = uc.util.StringUtil.htmlEncode(textContent);
                    textContent = textContent.replace(/uc_special_ch_enter/g, "<br>");
                    var text = "", option = '';
                    if (content.url != null && content.url != "") {
                        var url = content.url;
                        if (content.urlauth == 1) {
                            url = self.getApiRequestUrl(url)
                        }
                        text = '<a target="_blank" href="' + url + '">' + textContent + '</a>';
                    } else {
                        text =  textContent;
                    }

                    if (content.name) {
                        if (oaData) {
                            content.name = content.name.replace(/\n/g, "");
                            var _oaData = (typeof(oaData) == 'object' ? oaData : JSON.parse(oaData));
                            if (_oaData.text) {
                                var _text = (_oaData.text[content.name] ? _oaData.text[content.name] : '');
                                option = '<span>'+ _text +'</span><br />';
                            }
                        } else {
                            option = '<span data-name="'+ content.name +'"></span><br />';
                        }
                    }
                    return '<span class="' + fontClass + '" style="' + style +  '">' + text + '</span>' + option;
                };
            var getTextElementsDomStr =
                function(el) {
                    var domstr = "";
                    if (Object.prototype.toString.call(el.content) == "[object String]") {
                        domstr = '<div class="oaitem ">' + uc.util.StringUtil.htmlEncode(el.content) + '</div>';
                    } else if (Object.prototype.toString.call(el.content) == "[object Array]") {
                        var els = "";
                        for (var i = 0; i < el.content.length; i++) {
                            els = els + getTextContentDomStr(el.content[i]);
                        }
                        domstr = '<div class="oaitem ">' + els + '</div>';
                    } else if (Object.prototype.toString.call(el.content) == "[object Object]") {
                        var elDom = getTextContentDomStr(el.content);
                        domstr = '<div class="oaitem ">' + elDom + '</div>';
                    }
                    return domstr;
                };
            var getImageElementsDomStr =
                function(el) {
                    var url = '';

                    // 手写签名
                    if(el.name){
                        if (oaData) {
                            var _oaData = (typeof(oaData) == 'object' ? oaData : JSON.parse(oaData));
                            if(el.name && _oaData.image){
                                url = self.parent.getImageUrl(el.imageType, _oaData.image[el.name]);
                                return  '<div class="oaImageFrame"><img class="oaiimageItem" src="' + url + '" status="open" style="cursor:pointer;"></div>';
                            }
                        }

                        return '';
                    }else{
                        // 其他图片
                        url = self.parent.getImageUrl(el.imageType, el.imageId);

                        if (url == el.imageId){
                            return  '<div class="oaImageFrame"><img class="oaiimageItem" src="' + url + '"></div>';
                        }
                        var localUrl = uc.modules.chat.ChatManager.getLoaclUrl(url);
                        if (localUrl != null &&  localUrl != ""){
                            return '<div class="oaImageFrame"><img class="msg-img" status="open" style="cursor:pointer;"  data-id="'+ msgId +'" name="" seq="'
                                + msgSeq + '" data-url="' + url +'" src="' + localUrl + '"></div>';
                        }
                        return '<div class="oaImageFrame"><img class="msg-img" data-id="'+ msgId +'" name="" seq="'
                            + msgSeq + '" data-url="' + url +'" src="../images/messagebox/placeImg.gif"></div>';
                    }
                };
            var getImageElementStyle = function(domStr, el){
                var $originalDom = $('<div></div>'), width, height, imgMode = el.mode, imgAlign = el.align;

                if(!domStr){
                    return '';
                }

                $originalDom.append(domStr);

                var $dom = $originalDom.find(".oaImageFrame"), $img = $dom.find("img"), url = $img.attr("src");

                var fitWidth = function($dom, $img){
                    $dom.addClass("fitWidthModeFrame");
                    $img.addClass("fitWidthModeImg");
                };

                var fitHeight = function($dom, $img, imgAlign, width, height){
                    if(imgAlign.toLowerCase() == "left"){
                        $dom.addClass("fitHeightModeLeftFrame");
                        $img.addClass("fitHeightModeLeftImg");
                    }else if(imgAlign.toLowerCase() == "right"){
                        $dom.addClass("fitHeightModeRightFrame");
                        $img.addClass("fitHeightModeRightImg");
                    }else{
                        $dom.addClass("fitHeightModeCenterFrame");
                        $img.addClass("fitHeightModeCenterImg");

                        if(width/height > 9/16){
                            $img.css("margin-left", "-180px");
                        }else{
                            $img.css("margin-left", (320 - width/height*630)/2 + 'px');
                        }
                    }
                };

                // 获取原图的宽和高
                var image = new Image();
                image.src = url;
                if(image.complete){
                    width = image.width;
                    height = image.height;
                }else{
                    image.onload = function(){
                        width = image.width;
                        height = image.height;
                    };
                }

                if(!width || !height){
                    $dom.addClass("notCutFrame");
                    $img.addClass("notCutImg");
                }else{
                    imgAlign = imgAlign ? imgAlign : "center";
                    if(imgMode == "original"){
                        if(imgAlign.toLowerCase() == "left"){
                            $dom.addClass("originalModeLeftFrame");
                            $img.addClass("originalModeLeftImg");
                        }else if(imgAlign.toLowerCase() == "right"){
                            $dom.addClass("originalModeRightFrame");
                            $img.addClass("originalModeRightImg");
                        }else{
                            $dom.addClass("originalModeCenterFrame");
                            $img.addClass("originalModeCenterImg");
                            $img.css("margin-left", -width/2 + 'px');
                        }
                    }else if(imgMode == "fitWidth"){
                        fitWidth($dom, $img);
                    }else if(imgMode == "fitHeight"){
                        fitHeight($dom, $img, imgAlign, width, height);
                    }else{
                        if(width/height > 9/16){
                            fitWidth($dom, $img);
                        }else{
                            fitHeight($dom, $img, imgAlign, width, height);
                        }
                    }
                }
                return $originalDom.html();
            };
            var getActionElementsDomStr =
                function(el, index) {
                    //needSuggestion: 整形， 1：需要意见， 0：不需要意见; 默认不需要。
                    //noEmptySuggestion: 整形， 1: 意见内容不能为空， 0: 意见内容可为空：默认0.

                    if (el.buttons == null || el.buttons.length <= 0) {
                        return "";
                    }

                    var oaBtnsDomstr = "", comment = '<div class="oaitem"><input class="comment hide" placeholder="'+ uc_resource.OpenApi.Comment +'" /></div>';
                    var menuItems = [];
                    for (var i = 0; i < el.buttons.length; i++){
                        var btn = el.buttons[i];
                        var isNeedSuggestion = btn.needSuggestion ? '1' : '0';
                        var suggestionTitle = btn.suggestionTitle || uc_resource.OpenApi.ApproveTitle;
                        var isNeedSignature = btn.needSignature ? '1' : '0';
                        var signatureName = btn.signatureName;
                        var isNoEmpty = ((isNeedSuggestion == 1) ? btn.noEmptySuggestion : 0);
                        if (i >= 2) {
                            if (i == 2) {
                                oaBtnsDomstr = oaBtnsDomstr + '<button class="oamore normal" data-need-suggestion="'+ isNeedSuggestion +'" data-not-empty="'+ isNoEmpty +'" data-name="' + opinion + '" data-suggestion-title="'+ suggestionTitle + '" data-need-signature="' + isNeedSignature + '" style="display: block;">.<br>.<br>.</button>';
                            }
                            menuItems.push(btn);
                            continue;
                        }
                        oaBtnsDomstr = oaBtnsDomstr + '<button class="oaBtn normal" data-button-num="' + index + '" data-name="'+ opinion +'" data-need-suggestion="'+ isNeedSuggestion +'" data-not-empty="'+ isNoEmpty +'" data-suggestion-title="'+ suggestionTitle +'" data-need-signature="' + isNeedSignature +'" data-signature-name="' + signatureName +'"  msgSeq="' + msgSeq + '" msgId="'+ msgId +'" url="' + btn.url  + '">' + btn.title + '</button>';
                    }
                    var menuStr = JSON.stringify(menuItems);
                    return comment + '<div class="oaopation">'
                        +                '<div class="layout">'
                        +                         '<div class="opstatus" data-button-num="' + index + '" menuitem="' + encodeURIComponent(menuStr) + '"></div>'
                        +                         oaBtnsDomstr
                        +                     '</div>'
                        +                '</div>';
                };

            var oaitemDomstr =  "";
            var obj;
            var elements = [];
            try {
                obj = JSON.parse(data);
                elements = obj;
            } catch(e) {
                console.log(e);
            }

            for(var j = 0; j < elements.length; j++){
                var element = elements[j];

                if(element == null) {
                    continue;
                }

                if(element.type == "text" && element.content){
                    var eLcontents = element.content;

                    for(var t = 0; t < eLcontents.length; t++){
                        var eLcontent = eLcontents[t];

                        if(eLcontent.name){
                            opinion = eLcontent.name;
                        }
                    }

                }
            }

            for (var i = 0; i < elements.length; i++) {
                var el = elements[i];

                if (el == null || (el.status != null && status != el.status)) {
                    continue;
                }
                if (el.type == "text") {
                    oaitemDomstr += getTextElementsDomStr(el);
                } else if (el.type == "image") {
                    oaitemDomstr += getImageElementStyle(getImageElementsDomStr(el), el);
                } else if (el.type == "action") {
                    oaitemDomstr += getActionElementsDomStr(el, i);
                }
            }

            var detailDom = '<div class="oabottom"></div>';
            if (detailUrl != ""){
                detailDom = '<div class="oadetail" url="' + detailUrl + '">'+ uc_resource.Chat.ViewDetail +'</div>';
            }
            var titleCss = '';
            var titleMode = titleStyle;
            if(titleMode == uc.constants.Api.titleStyleMode.Default){
                var titleBkColors = getTitleBkColor(titleColor);
                var titleFontColor = titleBkColors == "#ffffff" ? "" : "#ffffff";
                titleCss = 'background:' + titleBkColors + '; color:' + titleFontColor + ';';
            }else if(titleMode == uc.constants.Api.titleStyleMode.simple){
                titleCss = 'background:-webkit-gradient(linear,0 20%,0 21%,from(' + getTitleBkColor(titleColor) + '),to(#ffffff));';
            }
            return          '<li class="message-item" id="'+ msgId +'" data-status="'+ status +'"  seq="' + msgSeq + '" detailauth="'+ detailAuth +'" data-id="'+ self.parent.appId +'">'
                +                '<div class="oa-message" >'
                +                   '<div class="oatitle" style="' + titleCss + '">'
                +                       '<div class="oatitletext">'+ uc.util.StringUtil.htmlEncode(title ) +'</div>'
                +                       '<div class="oatime"  id="tm' + msgSeq +'">' + uc.util.TimeUtil.getMessageTimeString(new Date(parseInt(msgSeq)), uc.constants.UI.TimeFormat.Message.HourMinutes, uc.settings.meeting.timeZone.offset) + '</div>'
                +                   '</div>'
                +                   oaitemDomstr
                +                   detailDom
                +                '</div>'
                +             '</li>';
        },
        getApiImgMsgDomStr: function(sendState, self, from, msgId, msgSeq, imageName, imageSize, imageURL, imageData, timestamp){
            imageName = uc.util.StringUtil.htmlEncode(imageName);
            var imgDomStr = '<img class="msg-img" status="link" data-id="'+ msgId +'" name="' + imageName + '" seq="' + msgSeq + '" data-url="' + imageURL +'" src="' + imageURL +'"/>';
            var cls = self ? "img-me-to" : "img-to-me";
            return          '<li class="message-item" id="'+ msgId +'"  seq="' + msgSeq + '">'
                +               '<div class="' + cls + '">'
                +               '<div>'
                +  imgDomStr
                +               '</div>'
                +              '</div>'
                +               this.getUnsendFlagDomStr(sendState, self, msgId)
                +               this.getMsgExtDomStr(self, from, msgId, msgSeq)
                +           '</li>';
        },
        getApiFileMsgDomStr: function(sendState, self, from, msgId, msgSeq, fileName, fileSize, fileURL){
            var fileNameEnocde = uc.util.StringUtil.htmlEncode(fileName);
            var cls = self ? "api-file-me-to" : "api-file-to-me";
            return          '<li class="message-item" id="'+ msgId +'"  seq="' + msgSeq + '">'
                +               '<div class="' + cls + '" data-url="' + fileURL + '">'
                +                  '<div>'
                +                   '<img src="'+ uc.util.StringUtil.getFileIcon(fileNameEnocde) + '"><div class="file-ext">' + uc.util.StringUtil.getFileDisplayExt(fileNameEnocde) + '</div>'
                +                   '<div class="content">'
                +                       '<span class="name">' + fileNameEnocde + '</span>'
                +                   '</div>'
                +                  '</div>'
                +               '</div>'
                +               this.getUnsendFlagDomStr(sendState, self, msgId)
                +               this.getMsgExtDomStr(self, from, msgId, msgSeq)
                +           '</li>';
        },
        getSummaryAudioMsgDomStr:function(self,from, to, operatorId, summaryId, summaryName, conferenceId, eventId, meetingName, meetingStartTime, updateTime,type,videoName,videoURL,videoThumb,thumbnailsURL,length,size,operateName,operateAvatar,msgId, msgSeq){
            meetingName=decodeURI(meetingName);
            var message = uc.util.StringUtil.formatString(uc_resource.Message.MeetingVideoWithMeetingName, [operateName]);
            var meetingVideoLength=uc.util.TimeUtil.min2hour(length / 60,false,true);
            var imgDomStr = '<div class="meetingVideo" videoName="' + (videoName||meetingName) + '"'+
                'videoSize="' + size + '" videoURL="' + videoURL + '" videoURL="' + videoURL + '"' +
                'videoThumb="' + videoThumb + '" videoDuration="'+ length +'" operatorId="'+operatorId+'"' +
                'operatorName="'+operateName+'" operatorAvatar="'+operateAvatar+'" thumbnailsURL="'+thumbnailsURL+'">' +
                '<img class="meetingVideoImage" onerror="this.src=\'../images/sfileicons/record.png\'" src="'+thumbnailsURL+'"/>' +
                '<span class="meetingVideoCheck" ></span>' +
                '<span class="meetingVideoTitle"> '+message+'</span>' +
                '<span class="meetingVideoTimeLength">'+meetingVideoLength+'</span>' +
                '<div class="meetingVideoButton">' +
                '<a class="meetingVideoForward">'+uc_resource.Menu.ForwardMeeting+'</a>' +
                '<a class="meetingVideoShare">'+uc_resource.Message.ShareMeetingVideo+'</a>' +
                ' </div>' +
                '</div>';
            var cls = self ? "img-me-to" : "img-to-me";
            return          '<li class="message-item" id="'+ msgId +'"  seq="' + msgSeq + '">'
                +               '<div class="' + cls + '">'
                +  imgDomStr
                +               '</div>'
                +               this.getMsgExtDomStr(self, from, msgId, msgSeq)
                +           '</li>';
        },
        getMeetingReportMsgDomStr: function(eventId, siteId, msgId, msgSeq, msgTime, reportTitle, reportUrl) {
            var tmstr = uc.util.TimeUtil.getMessageTimeString(new Date(msgTime), uc.constants.UI.TimeFormat.Message.HourMinutes, uc.settings.meeting.timeZone.offset);

            return '<li class="message-item" id="'+ msgId +'" seq="'+ msgSeq +'">'
                     + '<span class="message-time" id="tm' + msgId + '">' + tmstr + '</span>'
                     + '<div class="meeting-report" site-id="' + siteId + '">'
                     +   '<p class="text-selectable">' + reportTitle + '</p>'
                     +   '<div class="separator"></div>'
                     +   '<div class="meeting-report-link">'
                     +       '<a class="icon-forward"></a>'
                     +       '<a content="'+ reportUrl +'">' + uc_resource.Chat.ViewMeetingReport +'</a>'
                     +   '</div>'
                     + '</div>'
                     + '</li>';
        },
        setUnreadCountEvent:function(userId,info){
            if(userId==this.currentUser){
                var message;
                switch(this.chatType){
                    case uc.constants.Chat.ChatType.SingleChat:
                        for(var i=0;i<info.length;i++){
                            message=!info[i].allRead?uc_resource.Message.SingleChatMessageUnread:uc_resource.Message.MessageRead;
                            if(message==uc_resource.Message.SingleChatMessageUnread){
                                this.chatList.find("li.message-item[seq="+info[i].msgSeq+"]").find(".unReadNumber").text(message).show().addClass("singleRead").removeClass("singleReadColor");
                            }else{
                                this.chatList.find("li.message-item[seq="+info[i].msgSeq+"]").find(".unReadNumber").text(message).show().addClass("singleReadColor").addClass("singleRead");
                            }
                        }
                        break;
                    case uc.constants.Chat.ChatType.MeetingChat:
                    case uc.constants.Chat.ChatType.ProjectChat:
                    case uc.constants.Chat.ChatType.GroupChat:
                    case uc.constants.Chat.ChatType.CloudChat:
                        for(var j=0;j<info.length;j++){
                            if(info[j].allRead){
                                message=uc_resource.Message.MessageRead;
                                this.chatList.find("li.message-item[seq="+info[j].msgSeq+"]").find(".unReadNumber").text(message).show().addClass("successRead");
                                if($(".mumberMessage[data-seq="+info[j].msgSeq+"]").length){
                                    $(".mumberMessage").remove();
                                }
                            }else{
                                if(info[j].unReadCount<0){
                                    this.chatList.find("li.message-item[seq="+info[j].msgSeq+"]").find(".unReadNumber").hide();
                                    if($(".mumberMessage[data-seq="+info[j].msgSeq+"]").length){
                                        $(".mumberMessage").remove();
                                        //this.chatList.find("li.message-item[seq="+info[j].msgSeq+"]").find(".unReadNumber").trigger("click");
                                    }
                                }else{
                                    message = uc.util.StringUtil.formatString(uc_resource.Message.MultiChatUnread, [info[j].unReadCount]);
                                    this.chatList.find("li.message-item[seq="+info[j].msgSeq+"]").find(".unReadNumber").text(message).show().removeClass("successRead");
                                    if($(".mumberMessage[data-seq="+info[j].msgSeq+"]").length){
                                        $(".mumberMessage").remove();
                                        //this.chatList.find("li.message-item[seq="+info[j].msgSeq+"]").find(".unReadNumber").trigger("click");
                                    }
                                }

                            }
                        }
                        break;
                }
            }

        },
        setUnreadUserListDiv: function(t, users, seq){
            var maxLeft = parseInt(t.offset().left);
            var maxTop = parseInt(t.offset().top);
            var unreadWidth = t.width();
            var number = users.length;
            var overflow = false;
            var staff = '';
            var width;
            var height;
            var left, mumber, tagLeft, tagff;
            var avatarNode = '<img src="../images/avatar_default.png" class="avatar round-image" />';

            if(maxLeft < 0){
                maxLeft = (-1) * maxLeft;
            }

            for(var i = 0; i < number; i++){
                var contactInfo = uc.modules.cache.BasicContactInfoCache.getBasicContactInfo(users[i]);
                var userId = users[i];

                //如果是超级群，显示随机图像
                var isSuperGroup = (this.chatType == uc.constants.Chat.ChatType.GroupChat ||
                    this.chatType == uc.constants.Chat.ChatType.ProjectChat ||
                    this.chatType == uc.constants.Chat.ChatType.CloudChat) &&
                    uc.modules.chat.ChatManager.groupInfo &&
                    uc.modules.chat.ChatManager.groupInfo.memberType;
                var bgColor, userLogoColor;

                if (isSuperGroup) {
                    userLogoColor = uc.modules.chat.ChatManager.superGroupMemberBgColor[userId];
                    if (userLogoColor) {
                        bgColor = userLogoColor;
                    } else {
                        var randomNumber = parseInt(Math.random() * 10);
                        bgColor = randomNumber > 5 ? this.superGroupBgColor[5] : this.superGroupBgColor[randomNumber];
                        uc.modules.chat.ChatManager.superGroupMemberBgColor[users[i]] = bgColor;
                    }
                }

                if(!contactInfo){
                    avatarNode = '<img src="../images/avatar_default.png" class="avatar round-image" rel="logo' + userId + '"/>';
                    if (isSuperGroup) {
                        avatarNode = '<div class="avatar round-image" style="background-color: '+ bgColor +'">'+ userId.toString().substring(0, 1) +'</div>';
                    }
                    staff+='<div class="read-unread-staff" data-userId="'+ userId +'">'
                        + avatarNode
                        + '<span class="userName" rel="'+ userId +'">'+ userId +'</span>'
                        +'</div>';
                }else{
                    if (isSuperGroup) {
                        avatarNode = '<div class="avatar round-image" style="background-color: '+ bgColor +'">'+ contactInfo.displayName.substring(0, 1) +'</div>';
                    } else {
                        if(contactInfo.localAvatar){
                            avatarNode = '<img src="'+contactInfo.localAvatar+'" class="avatar round-image" />';
                        }else{
                            avatarNode = '<img src="../images/avatar_default.png" class="avatar round-image" rel="logo' + userId + '"/>';
                            uc.IUIService.downloadUserAvatar(userId, contactInfo.remoteAvatarUrl);
                        }
                    }

                    staff += '<div class="read-unread-staff" data-userId="'+ userId +'">'
                                + avatarNode
                                + '<span class="userName">'+contactInfo.displayName+'</span>'
                                +'</div>';
                }
            }

            if(number <= 6){
                width = number * 39 + number * 20;
                height = 20 + 75;
            }else if(number < 18 || number == 18){
                width = 370;
                var num = parseInt(number/6);
                var or = number%6;
                height = or > 0 ? ( num + 1 ) * 90 : num * 90;
            }else{
                width = 370;
                height = 263;
                overflow = true;
            }

            var center = width / 2;
            var tagTop = maxTop < height ? ( -7 ) + "px" : 0;
            overflow = overflow ? "auto" : "hidden";
            maxTop = maxTop < height ? maxTop + 25 : maxTop - height - 15;


            if(maxLeft - 340 < width){
                if(this.chatType == uc.constants.Chat.ChatType.MeetingChat && uc.modules.chat.ChatManager.moduleId != uc.constants.UI.ModuleId.Message){
                    tagLeft= $('.content-panel .chat-panel')[0].clientWidth-
                        $('.content-panel .chat-panel').find("li.getUserlist")[0].clientWidth+ $('.content-panel .chat-panel').find("li.getUserlist").find('.unReadNumber')[0].offsetLeft+170/2;//center-maxLeft-10+"px";
                    left=380+"px";
                }else{
                    left = 380 + "px";
                    tagLeft = t[0].offsetLeft + 70 / 2 - 20;
                }

                tagff = tagLeft - 5 + "px";
                tagLeft = tagLeft + "px";
                mumber = '<div class="mumberMessage" data-seq="' + seq + '" style="left:' + left + ';width:' + width + 'px;height:' + height + 'px;' +
                    'top:' + maxTop + 'px;">';

                if(tagTop){
                    mumber += '<div class="tagMessageTop" style="left:' + tagLeft + ';top:' + tagTop + '"></div>';
                    mumber += '<div class="tagMessageff" style="left:' + tagff + ';top:0px"></div>';
                }else{
                    mumber += '<div class="tagMessage" style="left:' + tagLeft + '"></div>';
                    mumber += '<div class="tagMessageff" style="left:' + tagff + '"></div>';
                }

                mumber += '<div class="users" style="left:' + left + ';width:' + width + 'px;height:' + height + 'px;overflow-y:' + overflow + ';' +
                    'top:' + maxTop + 'px;">' +
                    staff
                    + '</div>'
                    + '</div>';
            }else{
                tagLeft = center;
                var rightLength = t.parents("li.message-item")[0].clientWidth - t[0].offsetLeft - unreadWidth / 2 - center - 20;

                if(rightLength < 0){
                    tagLeft = tagLeft + ( -1 ) * rightLength - 10;
                    center = center - unreadWidth / 2 - 10 + ( -1 ) * rightLength;

                    if(this.chatType == uc.constants.Chat.ChatType.MeetingChat && uc.modules.chat.ChatManager.moduleId != uc.constants.UI.ModuleId.Message){
                        left=400+t.parents(".chat-panel")[0].clientWidth-width+"px";
                    }else{
                        left = 360 + t.parents("li.message-item")[0].clientWidth - width + "px";
                    }
                }else{
                    center = center - unreadWidth / 2 - 10;
                    left = maxLeft - 70 / 2 - center + 20 + "px";
                }

                tagff = tagLeft - 5 + "px";
                tagLeft = tagLeft + "px";
                mumber = '<div class="mumberMessage" data-seq="' + seq + '" style="left:' + left + ';width:' + width + 'px;height:' + height + 'px;' +
                    'top:' + maxTop + 'px;">';

                if(tagTop){
                    mumber += '<div class="tagMessageTop" style="left:' + tagLeft + ';top:' + tagTop + '"></div>';
                    mumber += '<div class="tagMessageff" style="left:' + tagff + ';top:0px"></div>';
                }else{
                    mumber += '<div class="tagMessage" style="left:' + tagLeft + '"></div>';
                    mumber += '<div class="tagMessageff" style="left:' + tagff + '"></div>';
                }

                mumber += '<div class="users" style="left:' + left + ';width:' + width + 'px;height:' + height + 'px;overflow-y:' + overflow + ';' +
                    'top:' + maxTop + 'px;">' +
                    staff
                    + '</div>'
                    + '</div>';
            }

            if(this.chatType == uc.constants.Chat.ChatType.MeetingChat && uc.modules.chat.ChatManager.moduleId != uc.constants.UI.ModuleId.Message){
                $('.meeting-detail-chat').parents('.content-panel').append(mumber);
                return;
            }

            this.parentNode.parent().append(mumber);
        },
        setUnreadUserList:function(userId,info){
            if(userId==this.currentUser){
				this.setUnreadCountEvent(userId,info);
                for(var i=0;i<info.length;i++){
                    var t=this.chatList.find("li.message-item[seq="+info[i].msgSeq+"]");
                    if(this.chatType==uc.constants.Chat.ChatType.MeetingChat && uc.modules.chat.ChatManager.moduleId != uc.constants.UI.ModuleId.Message){
                         t= $('.meeting-detail-chat .chat-list').find("li.message-item[seq="+info[i].msgSeq+"]");
                    }
                    if(t.hasClass("getUserlist")){
                        t= t.find(".unReadNumber");
                        this.setUnreadUserListDiv(t,info[i].unReadUser, info[i].msgSeq);
                    }
                }
            }
        },
        getMemberRoleMsgDomStr: function(content,seq,msgId){
            return  '<li class="sys-message-item" seq="' + seq + '" id="' + msgId + '">'
                +       '<div class="sys-message-box">'
                +           '<span class="sys-text">'+ content + '</span>'
                +       '</div>'
                +   '</li>';
        },
        getDurationMsgDomStr: function(sendState, self, from, msgId, msgSeq, textType, textContent, sendTime, msgTime, textFormat){
            textContent = uc.util.EmotionUtil.codeToEmotion(textContent);
            textContent = uc.modules.emoji.EmojiConverter.unicodeToSmiley(textContent);
            textContent = uc.modules.emoji.EmojiConverter.androidCodeToSmiley(textContent);
            textContent = textContent.replace(/\n/g, "<br>");
            textContent = uc.util.StringUtil.matchLink(textContent);

            try{
                var sizeStr = $('<div style="' + textFormat + '"/>').css("font-size");

                if (sizeStr != null) {
                    var value = Number(sizeStr.replace(/[^0-9]/ig, ""));

                    if (value > 0) {
                        textFormat += "; line-height: " + (value + 6) + "px;";
                    }
                }

                var tobj = $('<div style="' + textFormat + '"/>');
                var clr =  tobj.css("color");

                if (clr != null) {
                    var fmt = tobj.attr("style");

                    tobj.css("color", "");

                    if (fmt != null) {
                        textFormat = fmt;
                    }
                }
            } catch(e){}

            var cls = self ? "text-me-to" : "text-to-me";

            return '<li class="message-item" id="'+ msgId +'"  seq="' + msgSeq + '">'
                +       '<div>'
                +           this.getMsgExtDomStr(self, from, msgId, msgSeq)
                +           '<div class="' + cls + '" style="' + textFormat + '">'
                +               '<div>' + textContent + '</div>'
                +           '</div>'
                +           this.getUnsendFlagDomStr(sendState, self, msgId)
                +       '</div>'
                +   '</li>';
        },
        OnOAMessageFilterReceived: function (condition) {
            return; // 挂起
            this.chatList.children('li').not('[rel]').addClass('hide');

            switch (condition) {
                case 0: // all oa message
                    this.chatList.children('li').removeClass('hide');
                    break;
                case 1: // untreated oa message
                    $('.chat-list').children('li[data-status]').filter(function () {
                        var status = parseInt($(this).attr('data-status'));
                        return (status >= 11 && status <= 20) ? true : false;
                    }).removeClass('hide');
                    break;
                case 2: // treated oa message
                    $('.chat-list').children('li[data-status]').filter(function () {
                        var status = $(this).attr('data-status');
                        return (status >= 1 && status <= 10) ? true : false;
                    }).removeClass('hide');
                    break;
                case 3: // other oa message
                    this.chatList.children('li').not('[data-status]').removeClass('hide');
                    break;
            }
        },
        clear: function(){
            if (this.node) {
                this.node.remove();
                this.node = null;
            }
        }
    })
})(uc);