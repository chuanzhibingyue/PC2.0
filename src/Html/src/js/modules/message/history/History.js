/**
 * Created by xujiansong on 16/3/16.
 */
(function (uc) {
    uc.modules.historymsg.History = {
        init: function () {
            this.firstGetHistory = true;
            this.historyDomStr = "";
            this.lastMsgSeq = 0;
            this.seqs = {};
            this.levelHistory = null;
            this.levelNextHistory = null;
            this.msgBegin = '<div class="message-body">';
            this.msgEnd = '</div></div></div>';

            this.nextResult_back();
            this.keywordback();

            this.msgText = '<div  class="message-info"></div>';
            this.msgImg = '<div class="message-img" ><img/></div>';
            this.msgfile = '<div class="message-file">'
                + '<div class="filepic"><img  src="" /><span class="filename-end"></span></div>'
                + '<div class="fileinfo"><div class="filename"></div><div class="filesize"></div></div>'
                + '<div class="fileop"><a></a><span></span></div> '
                + '</div>';

        },
        keywordback: function () {
            this.levelKeyWordHistory = null;
        },
        nextResult_back: function () {
            //NextlayerResult init fn
            this.firstGetHistory_next = true;
            this.historyDomStr_next = "";
            this.lastMsgSeq_next = 0;
            this.lastMsgSeq_max_next = 0;
            this.seqs_next = {};
            this.levelNextHistory = null;
            this.moveUporDown_next = 0;//0:up 1 down
        },
        getImgAutoHeight:function (obj,offsetH) {
            if(  this.rowHeight==null) {
                this.rowHeight = 0;
            }
            if(obj==null)
            {
                this.rowHeight +=offsetH;
            }
            else {
                var lastH = obj.scrollTop() + this.rowHeight;
                //obj.animate({scrollTop:lastH},150);
                if( obj.scrollTop()>0) {
                    obj.scrollTop(lastH);
                }
                this.rowHeight = 0;
            }
        },
        showhistoryList: function () {

        },
        getHeadDomStr: function (self, from, msgId, msgTime, fileUpdateStr) {
            var tmstr = uc.util.TimeUtil.getMessageTimeString(new Date(msgTime), uc.constants.UI.TimeFormat.Message.HourMinutes, uc.settings.meeting.timeZone.offset);
            var displayName = from.userId;
            if (from.displayName != null) {
                displayName = from.displayName;
            }
            else
            {
                var contactInfo = uc.modules.cache.BasicContactInfoCache.getBasicContactInfo(from.userId);
                if(contactInfo){
                    displayName = contactInfo.displayName;
                }
            }
            var fileUpdate = "";
            if (fileUpdateStr) {
                fileUpdate = fileUpdateStr;
            }
            var ismine = "me";
            var logoPath="";
            if (!self) {
                ismine = "";
                logoPath = "../images/avatar_default.png";
                var userAvatar ="";
                if (from.localAvatar != null && from.localAvatar != "") {
                    logoPath = from.localAvatar;// + "?" + new Date().getTime();
                }
                //如果是超级群，显示随机图像
                /*
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
                 }
                 */
            }
            else {
                if (uc.modules.chat.ChatManager.myUserInfo != null && uc.modules.chat.ChatManager.myUserInfo.displayName != null) {
                    displayName = uc.modules.chat.ChatManager.myUserInfo.displayName;
                }
            }

            return '<div class="row  ' + ismine + '" id=' + msgId + ' seq =' + msgTime + '>'
                + '<img class="row-user"   onerror="if(this.defaultlogo){return;} ;this.src=\'../images/avatar_default.png\';this.defaultlogo=true;"  rel="logo' + from.userId + '" src="' + logoPath + '" >'
                + '<div class="row-info" > '
                + '<div class="message-head">'
                + '<span class="user-name" rel="' + from.userId + '">' + displayName + '</span>'
                + '<span class="message-time" id="tm' + msgId + '">' + tmstr + '</span></div>';

        },
        getTextMsgDomStr: function (sendState, self, from, msgId, msgSeq, textType, textContent, msgTime, msgTime, textFormat) {
            var head = this.getHeadDomStr(self, from, msgId, msgSeq);
            textContent = uc.util.EmotionUtil.codeToEmotion(textContent);
            textContent = uc.modules.emoji.EmojiConverter.unicodeToSmiley(textContent);
            textContent = uc.modules.emoji.EmojiConverter.androidCodeToSmiley(textContent);
            textContent = textContent.replace(/\n/g, "<br>");
            textContent = uc.util.StringUtil.matchLink(textContent);
            try {
                var sizeStr = $('<div style="' + textFormat + '"/>').css("font-size");
                if (sizeStr != null) {
                    var value = Number(sizeStr.replace(/[^0-9]/ig, ""));
                    if (value > 0) {
                        textFormat += "; line-height: " + (value + 6) + "px;";
                    }
                }
                var tobj = $('<div style="' + textFormat + '"/>');
                var clr = tobj.css("color");
                if (clr != null) {
                    tobj.css("color", "");
                    var fmt = tobj.attr("style");
                    if (fmt != null) {
                        textFormat = fmt;
                    }
                }
            } catch (e) {
            }

            var mycss = "";
            if (self)  mycss = "change";
            this.msgText = '<div  class="message-info" style="' + textFormat + '" >' + textContent + '</div>';
            var shownextResult = "";
            if (this.levelKeyWordHistory != null && this.levelKeyWordHistory.attr("active") == 1) {
                shownextResult = '<div class="nextresult"><span>'+ uc_resource.Search.ViewPrevOrNext +'</span></div>';
                return head + '<div class="message-body ' + mycss + '">' + this.msgText + shownextResult + "</div></div></div>";
            }
            else
                return head + '<div class="message-body ' + mycss + '">' + this.msgText + this.msgEnd;
        },
        getImgMsgDomStr: function (sendState, self, from, msgId, msgSeq, fileName, fileSize, fileURL, thumbnails, msgTime) {
            var head = this.getHeadDomStr(self, from, msgId, msgSeq);
            var url = uc.modules.chat.ChatManager.getLoaclUrl(fileURL);
            imageName = uc.util.StringUtil.htmlEncode(fileName);
            if(url=="")
                url =fileURL;
            this.msgImg = '<div class="message-img" ><div class="bgImg"></div><img src="' + url + '"  style ="display:none"  data-url="' + thumbnails + '" size="' + fileSize + '" data-id="' + msgId + '" name="' + imageName + '" /></div>';
            return head + this.msgBegin + this.msgImg + this.msgEnd;
        },
        getFileMsgDomStr: function (conversationType, sendState, self, from, msgId, msgSeq, fileName, fileSize, fileURL, msgTime, fileId, username,localPath) {
            var head = this.getHeadDomStr(self, from, msgId, msgSeq);
            var size = uc.util.StringUtil.formatFileSize(fileSize);
            var cloudType = uc.util.StorageUtil.getData('cloudType');
            var cloudTokenResult = uc.util.StorageUtil.getData('CloudLoginResult');
            var url = uc.modules.chat.ChatManager.getLoaclUrl(fileURL);
            uc.modules.chat.ChatManager.FSFileInfo[msgId] = {
                "msgId": msgId,
                "fileName": fileName,
                "fileSize": fileSize,
                "fileURL": fileURL,
                "fileId": fileId
            };
            var fileName = uc.util.StringUtil.htmlEncode(fileName);
            var icon = uc.util.StringUtil.getFileIcon(fileName);
            var iconName = uc.util.StringUtil.getFileDisplayExt(fileName);
            var hide = "display:none";
            if (fileId) hide = "display:";

            var domSaveCloud = '', domReceive = '', domComment = "";
            if (cloudType == 1 && cloudTokenResult == 0) {
                domSaveCloud = '<a class="op-savecloud">' + uc_resource.MessageBox.saveCloud + '</a><span></span>';
            }
            if (url||localPath) {
                url = url?url:localPath;
                domReceive =
                    '<a  fileop="1"  class="op-receive"  op-status="open" local-path="' + url + '">' + uc_resource.MessageBox.open + '</a>' +
                    '<span></span>' +
                    '<a class="op-openfolder" local-path="' + url + '">' + uc_resource.MessageBox.openDir + '</a>';
            } else {
                domReceive =
                    '<a  fileop="1" class="op-saveAs">' + uc_resource.MessageBox.saveAs + '</a>' +
                    '<span></span>' +
                    '<a  fileop="1" class="op-receive">' + uc_resource.MessageBox.receive + '</a>';
            }
            if (conversationType == uc.constants.Chat.ChatType.ProjectChat || conversationType == uc.constants.Chat.ChatType.GroupChat) {
                domComment = '<a class="op-comment" style="' + hide + '"  file-id="' + fileId + '">' + uc_resource.MessageBox.comment + '</a>' +
                    '<span class="op-comment-span" style="' + hide + '" ></span>';
            }

            var menu = domSaveCloud + domComment + '<a class="op-forward">' + uc_resource.MessageBox.forward + '</a><span></span>' + domReceive+'<progress class="op-progress" style="display: none" value="0" max="100"></progress>';

            this.msgfile = '<div class="message-file">'
                + '<div class="filepic"><img  src=' + icon + ' /><span class="filename-end">' + iconName + '</span></div>'
                + '<div class="fileinfo"><div class="filename">' + fileName + '</div><div class="filesize">' + fileSize + '</div></div>'
                + '<div class="fileop file-list-panel-menu">' + menu + '</div> '
                + '</div>';
            return head + this.msgBegin + this.msgfile + this.msgEnd;
        },
        getAudioMsgDomStr: function (sendState, self, from, msgId, msgSeq, fileName, fileSize, fileURL, duration, msgTime) {
            var head = this.getHeadDomStr(self, from, msgId, msgTime);
        },
        getChatHistoryReceived: function (errorCode, conversationType, groupId, messages, msgCount, minTimestamp) {
            if (this.firstGetHistory) {
                //uc.modules.chat.ChatManager.appendUploadMessages();
            }
            if (errorCode == 0) {
                this.lastMsgSeq = minTimestamp;
            }
            if (msgCount < uc.constants.Message.OnceGetHistoryCount && errorCode == 0) {
                this.levelHistory.attr("stop", 1);
            }
            if (this.historyDomStr == "") {
                this.firstGetHistory = false;
                return;
            }
            uc.util.LogUtil.info("uc.modules.historymsg.History", "getChatHistoryReceived", "data=", {
                errorCode: errorCode,
                groupId: groupId
            });

            this.appendMessage(this.historyDomStr, this.levelHistory);
            if (this.firstGetHistory) {
            }
            this.firstGetHistory = false;
            this.historyDomStr = "";
        },
        getChatHistoryNextResultReceived: function (errorCode, conversationType, groupId, minTimestamp, msgCount, maxTimestamp) {
            if (this.firstGetHistory_next) {
                //uc.modules.chat.ChatManager.appendUploadMessages();
            }
            if (errorCode == 0) {
                this.lastMsgSeq_next = minTimestamp;
                this.lastMsgSeq_max_next = maxTimestamp;
            }
            if (msgCount < uc.constants.Message.OnceGetHistoryCount && errorCode == 0) {
                if (this.moveUporDown_next == 0)
                    this.levelNextHistory.attr("upstop", 1);
                else if (this.moveUporDown_next == 1)
                    this.levelNextHistory.attr("downstop", 1);
            }
            if (this.historyDomStr_next == "") {
                this.firstGetHistory_next = false;
                return;
            }
            uc.util.LogUtil.info("uc.modules.historymsg.History", "getChatHistoryNextResultReceived", "data=", {
                errorCode: errorCode,
                groupId: groupId
            });
            if (this.firstGetHistory_next) {
                //this.msgBox.scrollToEnd();
            }
            this.appendMessage(this.historyDomStr_next, this.levelNextHistory);
            this.firstGetHistory_next = false;
            this.historyDomStr_next = "";
        },
        procMessageDomStr: function (channel, domStr, msgId, msgSeq) {
            uc.util.LogUtil.info("uc.modules.historymsg.History", "procMessageDomStr", "msgSeq=", {
                msgSeq: msgSeq,
                channel: channel
            });
            if (this.seqs[msgSeq] != null) {
                uc.util.LogUtil.info("uc.modules.historymsg.History", "procMessageDomStr", "the msg seq has been existed!=", {msgSeq: msgSeq});
                return;
            }
            this.seqs[msgSeq] = true;

            //this.lastMsgSeq = msgSeq;
            if ($('#' + msgId, this.levelHistory).length > 0) {
                uc.util.LogUtil.info("uc.modules.historymsg.History", "procMessageDomStr", "the msg seq has been existed!=", {msgId: msgId});
                return;
            }
            if (uc.constants.Message.MessageChannel.History == channel) {
                this.historyDomStr = domStr + this.historyDomStr;
                return;
            }
        },
        procMessageKeyWordByDomStr: function (channel, domStr, msgCount) {
            if (uc.constants.Message.MessageChannel.History == channel) {
                this.appendMessage(domStr, this.levelKeyWordHistory);
                if (msgCount < uc.constants.Message.OnceGetHistoryCount) {
                    this.levelKeyWordHistory.attr("stop", 1);
                }
            }
        },
        procMessageNextResultDomStr: function (channel, domStr, msgId, msgSeq) {
            uc.util.LogUtil.info("uc.modules.historymsg.History", "procMessageNextResultDomStr", "msgSeq=", {
                msgSeq: msgSeq,
                channel: channel
            });
            if (this.seqs_next[msgSeq] != null) {
                uc.util.LogUtil.info("uc.modules.historymsg.History", "procMessageNextResultDomStr", "the msg seq has been existed!=", {msgSeq: msgSeq});
                return;
            }
            this.seqs_next[msgSeq] = true;
            if ($('#' + msgId, this.levelNextHistory).length > 0) {
                uc.util.LogUtil.info("uc.modules.historymsg.History", "procMessageNextResultDomStr", "the msg seq has been existed!=", {msgId: msgId});
                return;
            }
            if (uc.constants.Message.MessageChannel.Search == channel) {
                this.historyDomStr_next = domStr + this.historyDomStr_next;
                return;
            }
        },
        appendMessage: function (DOMStr, levelPage) {
            var objscroll = levelPage.find(".list");
            var needToBottom = false;
            if (objscroll.height() + 20 >= objscroll[0].scrollHeight)
                needToBottom = true;

            var prepH = objscroll[0].scrollHeight;
            if (this.moveUporDown_next == 0)
                objscroll.find(".rows").prepend(DOMStr);
            else if (this.moveUporDown_next == 1)
                objscroll.find(".rows").append(DOMStr);
            this.refreshSysyTimeDom(objscroll);
            this.bindEvent(levelPage);
            if (this.moveUporDown_next == 0) {
                if (needToBottom)
                    objscroll.scrollTop(objscroll[0].scrollHeight);
                else
                    objscroll.scrollTop(objscroll[0].scrollHeight - prepH);
            }
        },
        refreshSysyTimeDom: function (objscroll) {
            objscroll.find("[rel=time]").remove();
            var items = $(".row", objscroll.find(".rows"));
            var lastseq = 0;
            for (var i = 0; i < items.length; i++) {
                var seqAttr = $(items[i]).attr("seq");
                if (seqAttr == null) {
                    continue;
                }
                var seq = Number(seqAttr);
                if (lastseq == 0 || new Date(lastseq).toDateString() != new Date(seq).toDateString()) {
                    var dom = '<div class="row" rel="time"><div class="sys-row"><span class="sys-msg"  >' + uc.util.TimeUtil.getSysMessageTimeStr(seq) + '</span></div></div>';
                    $(items[i]).before(dom);
                    lastseq = seq;
                }
            }
        },
        bindEvent: function (objlevel) {
            var _this = this;
            objlevel.find(".row").off("hover");
            objlevel.find(".row").hover(function () {
                $(this).find(".nextresult").css("opacity","1");
            },function () {
                $(this).find(".nextresult").css("opacity","0");
            });
            objlevel.find(".nextresult").off("click");
            objlevel.find(".nextresult").click(function () {
                if (objlevel.fn != undefined) {
                    var frontorBack = 1;
                    var beginTime = 0;
                    var endTime = 0;
                    if (frontorBack == 1) {
                        beginTime = $(this).parents(".row").attr("seq");
                        endTime = 0
                    }
                    else if (frontorBack == 0) {
                        beginTime = 0;
                        endTime = $(this).parents(".row").attr("seq");
                    }
                    objlevel.fn(this, frontorBack, beginTime, endTime);
                }
            });

            objlevel.find(".message-img img").one("load",function () {
                var httpURL = $(this).attr("src");
                var url = uc.modules.chat.ChatManager.getLoaclUrl(httpURL);
                if(url!="")
                  $(this).attr("src",url);
                $(this).show();
                $(this).prev().hide();
                if(_this.moveUporDown_next == 0)
                {
                    var offsetH =$(this).parent().height()-$(this).prev().height();
                    _this.getImgAutoHeight(null,offsetH);
                    _this.getImgAutoHeight(objlevel.find(".list"));
                }
            });

            objlevel.find(".message-img img").off("click");
            objlevel.find(".message-img img").click(function () {
                _this.showPic(this);
            });

            //op op-comment
            objlevel.find(".op-comment").off("click");
            objlevel.find(".op-comment").click(function () {
                //close history query layer
                $(".content-panel-body .Hparent").click();

                var fileId = $(this).attr("file-id");
                var msgId = $(this).parents(".row").attr("id");
                if (msgId == null) {
                    return;
                }
                var options = _this.getChatObjectInfo();
                var cloudInfo = uc.modules.chat.ChatManager.cloudFileInfo[msgId];
                if (options.chatType == uc.constants.Chat.ChatType.CloudChat) {
                    options = $.extend(options, {"fileId": fileId});
                    uc.modules.cloud.CloudManager.gCheckFileExist(cloudInfo, options, "comment");
                } else {
                    uc.modules.comment.CommentManager.initChatComment(fileId, cloudInfo, options);
                }

            });

            //file forwad
            objlevel.find(".op-forward").off("click");
            objlevel.find(".op-forward").click(function () {
                var msgId = $(this).parents(".row").attr("id");
                var fsInfo = uc.modules.chat.ChatManager.FSFileInfo[msgId];
                var fileId = "";
                if (fsInfo)fileId = fsInfo.fileId;
                var self = _this.getChatObjectInfo();
                if (self.chatType == uc.constants.Chat.ChatType.GroupChat || self.chatType == uc.constants.Chat.ChatType.ProjectChat|| self.chatType == uc.constants.Chat.ChatType.MeetingChat) {
                    if (fileId && fileId != "0") {
                        uc.modules.chat.ChatManager.fileListTagName[fileId] = {
                            "type": "forward",
                            "msgId": msgId,
                            "fileId": fileId,
                            "groupId": self.chatObjId
                        };
                        if(self.moduleId == uc.constants.UI.ModuleId.Meeting){
                            uc.modules.chat.MeetingChat.showFileCommentByFileId(fileId);
                        }else {
                            _this.showFileCommentByFileId(fileId);
                        }
                    } else {
                        uc.modules.chat.ChatManager.fileIdNotExistPrompt();
                    }
                } else {
                    uc.modules.chat.MessageManager.forwardFileMessage(fsInfo.fileName, fsInfo.fileSize, fsInfo.fileURL);
                }
            });

            //file SaveAs
            objlevel.find("a[fileop='1']").off("click");
            objlevel.find("a[fileop='1']").click(function () {
                var classType = $(this).attr("class");
                var status=$(this).attr("op-status");
                if(classType=="op-saveAs")
                {
                    classType = "saveAs";
                }
                else if(classType=="op-receive")
                {
                    classType = "receive";
                }

                var msgId=$(this).parents(".row").attr("id");
                var fsInfo= uc.modules.chat.ChatManager.FSFileInfo[msgId];
                var fileId="";

                if(fsInfo)fileId=fsInfo.fileId;
                if((status!="open" && status!="uploading" && status!="downloading") &&
                    (self.chatType == uc.constants.Chat.ChatType.GroupChat || self.chatType == uc.constants.Chat.ChatType.ProjectChat ||
                    self.chatType == uc.constants.Chat.ChatType.MeetingChat)){
                    if(fileId && fileId!="0"){
                        uc.modules.chat.ChatManager.fileListTagName[fileId]={
                            "type":classType,
                            "msgId":msgId,
                            "fileId":fileId,
                            "groupId":self.currentUser
                        };
                        _this.showFileCommentByFileId(fileId);
                    }else{
                        uc.modules.chat.ChatManager.fileIdNotExistPrompt();
                    }
                }else {
                    _this.fileMsgOperation($(this),fsInfo,classType,status);
                }


            });

            //file openfolder other DomObj create
            objlevel.off("click",".op-openfolder");
            objlevel.on("click",".op-openfolder",function(){
                var domObj = $('.op-receive', $(this).parent());
                var path = domObj.attr('local-path');
                if (path == null || path == "") {
                    return;
                }
                if (uc.util.FileUtil.openFolder(path) != 0) {
                    uc.modules.component.PromptMessageBox.open({
                            title: uc_resource.Common.Notification,
                            message: uc_resource.Message.OpenFileFailed,
                            buttode: uc_resource.Common.OK,
                            OkText: uc_resource.Common.Known
                        },
                        function (result) {
                            this.close();
                        });
                }
            });

            //file op-savecloud
            objlevel.find(".op-savecloud").off("click");
            objlevel.find(".op-savecloud").click(function () {
                var msgId=$(this).parents(".row").attr("id");
                var fsInfo=uc.modules.chat.ChatManager.FSFileInfo[msgId];
                uc.util.LogUtil.info('MessageBox','op-savecloud','msgId = ' + msgId + '', "fsInfo="+JSON.stringify(fsInfo));
                var fileId=fsInfo.fileId;
                var fileUrl=$(this).parents("li").find(".op-receive").attr("server-path");
                if(fileUrl){
                    uc.modules.chat.ChatManager.FSFileInfo[msgId].fileURL=fileUrl;
                }
                if(self.chatType == uc.constants.Chat.ChatType.GroupChat || self.chatType == uc.constants.Chat.ChatType.ProjectChat||
                    self.chatType == uc.constants.Chat.ChatType.MeetingChat){
                    if(fileId && fileId!="0"){
                        uc.modules.chat.ChatManager.fileListTagName[fileId]={
                            "type":"fsSaveCloud",
                            "msgId":msgId,
                            "fileId":fileId,
                            "groupId":self.currentUser
                        };
                        //uc.modules.chat.MeetingChat.showFileCommentByFileId(fileId);
                        _this.showFileCommentByFileId(fileId);
                    }else{
                        uc.modules.chat.ChatManager.fileIdNotExistPrompt();
                    }
                }else{
                    _this.OnSaveFileToCloud(fsInfo.fileURL,fsInfo.fileName);
                }
            });
        },
        showPic: function (obj) {
            //单击图片查看大图; 
            var This = $(obj);
            if (This.attr('status') == "link") {
                var url = This.attr('data-url');
                if (url != "") {
                    uc.util.FileUtil.openFile(url);
                }
                return;
            }

            if (This.attr('status') == "networkissue") {
                This.attr("src", "../images/messagebox/placeImg.gif");
                This.css({cursor: "default"});
                var url = This.attr('data-url');
                var msgId = This.attr('data-id');
                var seq = This.attr('seq');
                var name = This.attr('name');
                uc.modules.chat.ChatManager.downloadImg(self.chatId, self.chatType, msgId, seq, url, name,
                    function () {
                        This.attr('status', 'downloading');
                    }
                );
                return;
            }

            if (!This.attr('src')) {
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

        },
        getChatObjectInfo: function () {
            var myUserInfo = uc.IContactService.getCurrentUserInfo();
            return {
                chatType: uc.modules.chat.ChatManager.currentChatType,
                chatObjId: uc.modules.chat.ChatManager.currentChatId,
                siteId: myUserInfo.siteId,
                resId: 100
            };
        },
        fileMsgOperation: function(self,fsInfo,type,status) {
            var url =fsInfo.fileURL;
            var msgId = fsInfo.msgId;
            var fname =fsInfo.fileName;
            var itemLi=self.parents(".row");
            if (status == null) {   //下载
                uc.modules.chat.ChatManager.downloadFile(self,msgId, url, fname,
                    function(){
                        uc.util.LogUtil.info('History','downloadFile','filename',fname);
                        itemLi.find(".op-progress").show();
                        self.text(uc_resource.MessageBox.cancel).attr('op-status','downloading');
                        self.siblings().hide();
                        self.siblings(".saveFailed").hide();
                    },type);
            } else if (status == "downloading") { //取消下载
                uc.modules.chat.ChatManager.cancelDownload(msgId);
                itemLi.find(".op-receive").text(uc_resource.MessageBox.receive).removeAttr("op-status");
                itemLi.find(".op-saveAs").text(uc_resource.MessageBox.saveAs).removeAttr("op-status");
                self.siblings().show();
                self.siblings(".saveFailed").hide();
                itemLi.find(".op-progress").hide();
            } else if (status == "open") {  //打开
                var path = self.attr('local-path');
                if (uc.util.FileUtil.openFile(path) != 0){
                    uc.modules.component.PromptMessageBox.open({title: uc_resource.Common.Prompt, message: uc_resource.Message.OpenFileFailed, buttonMode:"ok", OkText: uc_resource.Common.Known},
                        function(result){
                            this.close();
                        });
                }
            } else if (status == "uploading") { //取消上传
                uc.modules.chat.ChatManager.cancelUpload(msgId);
                itemLi.remove();
            }
        },
        showFileCommentByFileId: function(fileId){
            if (fileId == "" || fileId == "null" || fileId == 0) return;
            uc.util.LogUtil.info("History", "showFileCommentByFileId", "groupId:" + this.getChatObjectInfo().chatObjId + ",fileId:" + fileId+"uc.modules.historymsg.isMeetingChat:"+uc.modules.historymsg.isMeetingChat);
            if(uc.modules.historymsg.isMeetingChat)
            {
                uc.IMeetingService.getMinutesDetails(0, Number(fileId), function(args){ });
            }
            else {
                uc.IGroupService.getDocumentInfo(this.getChatObjectInfo().chatObjId, Number(fileId), function (args) { });
            }
        },
        OnSaveFileToCloud:function(url,fileName){
            if(!uc.modules.chat.ChatManager.checkCloudOpenStatus()){
                return;
            }
            if(!uc.modules.chat.ChatManager.checkCloudLoadStatus()){
                return;
            }
            var dataStr=JSON.stringify({"filename":fileName,"url":url});
            uc.util.LogUtil.info('History', 'op-savecloud', 'fileName = ' + fileName + ', cloudInfo = ' + dataStr + '', "url="+url);
            uc.modules.cloud.CloudManager.saveCloudFile(null,dataStr);
        }
    }
})(uc);