/**
 * Created by zuo.dai on 2015/8/17.
 */
(function(uc){
    uc.modules.comment.Comment = function(){

    };
    jQuery.extend(uc.modules.comment.Comment.prototype, {
        init: function(parentNode){
            this.detailHtml =  '<div class="file-info" >'
                +                    '<img class="icon">'
                +                       '<span class="file-ext"/>'
                +                    '</img>'
                +                    '<div class="detail">'
                +                        '<div class="name"></div>'
                +                        '<div class="time"></div>'
                +                        '<div class="size"></div>'
                +                        '<div class="user"></div>'
                +                    '</div>'
                +               '</div>'
                +               '<div class="operation">'
                +                    '<div class="saveCloud">'+uc_resource.Comment.SaveCloud+'</div>'
                +                    '<progress class="progress" style="display:none"  value="0" max="100"></progress>'
                +                    '<div class="download">'+uc_resource.Comment.DownLoad+'</div>'
                +                    '<div class="forward">'+uc_resource.Comment.Forward+'</div>'
                //+                    '<div class="more">...</div>'
                +               '</div>'
                +               '<div class="no-comment" style="display: none;">'+uc_resource.Comment.NoComment+'</div>'
                +               '<div class="comment-box">'
                +               '<div class="line"></div>'
                +                   '<div class="more-msg-line">'
                +                       '<span class="more-msg-loading" style="display:none" ></span>'
                +                       '<span class="more-msg" style="display:none" >'+uc_resource.Comment.More+'</span>'
                +                    '</div>'
                +                    '<ul class= "comment-list">'
                +                    '</ul>'
                +               '</div>'
                +               '<div class="editor"><textarea class="text-input" placeholder="'+uc_resource.Comment.WriteComment+'"></textarea></div>'
                +               '<div class="tip">'+uc_resource.Comment.Sending+'</div>';
            this.node = $('<div class="item-list"></div>');
            this.node.append(this.detailHtml);
            parentNode.append("<div id='commentLoading'></div>");
            //this.node.append(this.menuHtml);
            this.node.appendTo(parentNode);
            this.parentNode = parentNode;
            this.commentList = $('.comment-list', this.node);
            this.bindEvent();
            this.setCloudIcon();
            this.chatType=uc.util.StorageUtil.getData('currentChatType');
            this.myUserInfo=uc.IContactService.getCurrentUserInfo();
        },
        bindEvent: function() {
            var This = this;
            this.itemList = this.node;
            this.moreMsgLine = $('.more-msg-line', this.itemList);
            this.moreMsg = $('.more-msg', this.itemList);
            this.moreMsgLoading = $('.more-msg-loading', this.itemList);

            this.itemList.on('click', '.download',
                function(){
                    var downloadBtn = $(this);
                    if(downloadBtn.text() ==uc_resource.Comment.Open) {
                        var path = downloadBtn.attr('local-path');
                        if (uc.util.FileUtil.openFile(path) != 0){
                            uc.modules.component.PromptMessageBox.open({title: uc_resource.Comment.Notification, message: uc_resource.Comment.OpenFail, buttonMode:"ok", OkText: uc_resource.Comment.Known},
                                function(result){
                                    this.close();
                                });
                        }
                        return;
                    }
                    var url = downloadBtn.attr("data-url");
                    var fileId =  downloadBtn.attr("file-id");
                    var item = $('#file' + fileId, This.itemList);
                    var fname = $('.file-info .detail .name', This.itemList).text();
                    var fileInfo = This.fileInfo;
                    if(fileInfo.cloudType==1){
                        if(!uc.modules.chat.ChatManager.checkCloudOpenStatus()){
                            return;
                        }
                        if(!uc.modules.chat.ChatManager.checkCloudLoadStatus()){
                            return;
                        }
                        var cloudData=fileInfo.detailContent;
                        if(This.chatType== uc.constants.Chat.ChatType.CloudChat){
                            uc.modules.cloud.CloudManager.gCheckFilePermission(cloudData,"comDownload",$(this));
                        }else{
                            var dataArr=JSON.parse(cloudData);
                            uc.modules.chat.ChatManager.downloadCloudDir(dataArr,function(data,savePath){
                                This.OnStartDownloadCloudFile(item,data,savePath);
                            });
                        }
                    }else{
                        uc.modules.chat.ChatManager.downloadFile(null, "file" + fileId, url, fname,
                            function(){
                                $('.progress', item).show();
                                downloadBtn.hide();
                            });
                    }
                });
            this.itemList.on('click', '.forward',
                function() {
                    var fileInfo = This.fileInfo;
                    if (fileInfo.cloudType==0 && fileInfo.contentType == 1) {
                        uc.modules.chat.MessageManager.forwardCodeMessage(fileInfo.codeTitle, fileInfo.langType, fileInfo.codeHead,
                            fileInfo.codeDesc, fileInfo.codeSize, fileInfo.codeURL, fileInfo.codeLine, fileInfo.createTime);
                    }else {
                        if(fileInfo.cloudType==1){
                            if(!uc.modules.chat.ChatManager.checkCloudOpenStatus()){
                                return;
                            }
                            if(!uc.modules.chat.ChatManager.checkCloudLoadStatus()){
                                return;
                            }
                            var cloudInfo=fileInfo.detailContent;
                            var cloudInfoArr=JSON.parse(cloudInfo);
                            if(This.chatType== uc.constants.Chat.ChatType.CloudChat){
                                uc.modules.cloud.CloudManager.gCheckFilePermission(cloudInfo,"comForward");
                            }else{
                                uc.modules.chat.MessageManager.forwardCloudFileMessage(fileInfo.fileName,fileInfo.fileSize,null,cloudInfo,cloudInfoArr);
                            }
                        }else if(fileInfo.cloudType==0){
                            uc.modules.chat.MessageManager.forwardFileMessage(fileInfo.fileName, fileInfo.fileSize, fileInfo.fileUrl);
                        }
                    }
                });

            this.moreMsg.click(function(){
                This.onMoreMsgLoading();
                This.getMoreComments();
            });

            this.itemList.on("keydown", ".text-input", function (e) {
                var skMsg = uc.settings.skMsg;
                switch (skMsg) {
                    case 0 :
                        if (e.which == 13) {
                            e.preventDefault();
                            This.sendComment();
                        }
                        break;
                    case 1 :
                        if (e.ctrlKey && e.which == 13) {
                            e.preventDefault();
                            This.sendComment();
                        }
                        break;
                }
            });
            this.itemList.on("click", ".tip", function (e) {
                This.sendComment();
                e.preventDefault();
            });
            this.commentList.on("click", ".header", function(){
                var userId = $(this).attr("rel");
                uc.IUIService.showPopupContactProfile(Number(userId.substring(4)));
            });
            this.commentList.on("click", ".user", function(){
                var userId = $(this).attr("rel");
                uc.IUIService.showPopupContactProfile(Number(userId));
            });
            this.itemList.on('click', '.saveCloud',function(){
                var fileInfo = This.fileInfo;
                if(fileInfo.cloudType==1){
                    if(!uc.modules.chat.ChatManager.checkCloudOpenStatus()){
                        return;
                    }
                    if(!uc.modules.chat.ChatManager.checkCloudLoadStatus()){
                        return;
                    }
                    var cloudInfo=fileInfo.detailContent;
                    uc.util.LogUtil.info('Comment', 'saveCloud','cloudInfo = ' +(cloudInfo) + '', null);
                    uc.modules.cloud.CloudManager.saveCloudFile(null,cloudInfo);
                }else if(fileInfo.cloudType==0 && fileInfo.contentType != 1){
                     var fileName=fileInfo.fileName;
                     var fileUrl=fileInfo.fileUrl;
                     var data=JSON.stringify({"filename":fileName,"url":fileUrl});
                     if(!fileUrl){
                         uc.util.LogUtil.info('Comment', 'saveCloud','data = ' +(data) + '', null);
                     }
                    uc.modules.cloud.CloudManager.saveCloudFile(null,data);
                }
            });
        },
        onBodyClick: function(e) {
            if ($(e.target).hasClass('more')) {
                return;
            }
            $('.menu', this.parentNode).hide();
        },
        onMoreMsgLoading: function(){
            this.moreMsgLoading.show();
            this.moreMsg.hide();
        },
        onMoreMsgLoadEnd: function(){
            this.moreMsgLoading.hide();
            this.moreMsg.show();
        },
        onLoadAllMsgEnd: function(){
            this.moreMsgLine.hide();
        },
        OnDownloadCloudFile:function(data,cloudData,that){
            var self=this;
            var data=JSON.parse(data);
            var fileInfo = this.fileInfo;
            if(data.hasOwnProperty("error")){
                var content= content=uc_resource.MessageBox.notPermission;
                if(data.error==40402){
                    content=uc_resource.MessageBox.notFileInfo;
                }
                console.log("OnDownloadCloudFile:"+data.error);
                uc.modules.component.PromptMessageBox.open({title: uc_resource.MessageBox.Notification, message: content, buttonMode:"ok",OkText:uc_resource.MessageBox.OK},
                    function(result){
                        this.close();
                    });
            }else{
                var dataArr=JSON.parse(cloudData);
                uc.modules.chat.ChatManager.downloadCloudDir(dataArr,function(data,savePath,cloudData){
                    self.OnStartDownloadCloudFile(that,data,savePath,cloudData);
                });
            }
        },
        OnStartDownloadCloudFile:function(that,data,savePath){
            var gkData=JSON.parse(data);
            var taskId=gkData.taskid;
            uc.modules.chat.ChatManager.cloudFileInfoDownloading[taskId] = {
                msgId:"",
                taskId:taskId,
                fileId:this.fileInfo.fileId
            };
            uc.modules.chat.ChatManager.fileDownLoadInfo[taskId]={
                fileSavePath:savePath,
                hash:savePath,
                fileId:this.fileInfo.fileId
            };
            var item = $('#file' + that.attr("file-id"));
            $('.progress', item).show();
            $('.download', item).hide();
        },
        OnForwardCloudFile:function(data,cloudData){
            var data=JSON.parse(data);
            var cloudFileInfoArr= JSON.parse(cloudData);
            var fileName=cloudFileInfoArr.filename;
            var fileSize=cloudFileInfoArr.filesize;
            if(data.hasOwnProperty("error")){
                var content= content=uc_resource.MessageBox.notForward;
                if(data.error==40402){
                    content=uc_resource.MessageBox.notFileInfo;
                }
                uc.modules.component.PromptMessageBox.open({title: uc_resource.MessageBox.Notification, message:content, buttonMode:"ok",OkText:uc_resource.MessageBox.OK},
                    function(result){
                        this.close();
                    });
            }else{
                uc.modules.chat.MessageManager.forwardCloudFileMessage(fileName,fileSize,null,cloudData,cloudFileInfoArr);
            }
        },
        OnGkFileDownloadCallBack:function(arg){
            if($(".filecomment").is(":hidden")){
                return;
            }
            if(arg){
                var percent=arg.detail.percent;
                var taskId=arg.detail.taskid;
                var hash=arg.detail.hash;
                var opObj=$("#"+hash);
                var item=opObj.parent();
                uc.util.LogUtil.info('Comment', 'gkTransfer = ' + JSON.stringify(arg.detail),null,null);
                if(arg.detail.hasOwnProperty("error")){
                    $('.progress', item).hide().attr("value", "0");
                    $('.download', item).show().text(uc_resource.Comment.download);
                }else{
                    if(uc.modules.chat.ChatManager.cloudFileInfoDownloading[taskId]){
                        item=$("#"+hash).parent();
                        if($('.progress', item).is(":hidden")){
                            $('.progress', item).show().attr("value",0);
                            $('.download', item).hide();
                        }
                    }
                    item.find(".progress").attr("value",percent);
                    if(percent==100){
                        if(uc.modules.chat.ChatManager.fileDownLoadInfo[taskId]){
                            var localPath=uc.modules.chat.ChatManager.fileDownLoadInfo[taskId].fileSavePath;
                            uc.modules.chat.ChatManager.downloadedFiles[arg.detail.hash]=localPath;
                        }
                        item.find(".progress").hide();
                        $('.progress', item).hide().attr("value", "0");
                        $('.download', item).show().text(uc_resource.Comment.Open).attr('local-path',localPath);
                        delete uc.modules.chat.ChatManager.cloudFileInfoDownloading[taskId];
                    }
                }
            }
        },
        setFileInfo: function(fileInfo, groupId) {
            if( this.fileInfo &&  this.fileInfo.fileId!=fileInfo.fileId){
                return;
            }
            this.groupId = groupId;
            this.fileInfo = fileInfo;
            var url=fileInfo.fileUrl;
            var username=fileInfo.username;
            $('.operation', this.itemList).attr('id', 'file' + fileInfo.fileId);
            if( fileInfo && fileInfo.cloudType==1 && fileInfo.hashCode){
                url=fileInfo.hashCode;
                $('.progress', this.itemList).attr('id',url);
            }
            var localUrl = uc.modules.chat.ChatManager.getLoaclUrl(url);
            $('.download', this.itemList).attr('data-url',fileInfo.fileUrl);
            $('.download', this.itemList).attr('file-id',fileInfo.fileId);
            if (localUrl == null || localUrl == ""){
                $('.download', this.itemList).text(uc_resource.Comment.DownLoad);
            } else {
                $('.download', this.itemList).text(uc_resource.Comment.Open);
                $('.download', this.itemList).attr('local-path',localUrl);
            }
            if( fileInfo && fileInfo.detailContent!="" && this.chatType== uc.constants.Chat.ChatType.CloudChat){
                uc.modules.cloud.CloudManager.gkGetFileInfo(fileInfo.detailContent,null);
                var cloudInfoObj=JSON.parse(fileInfo.detailContent);
                if(cloudInfoObj && cloudInfoObj.create_member_name){
                    username=cloudInfoObj.create_member_name;
                }else{
                    username=fileInfo.username;
                }
            }
            var iconSrc = uc.util.StringUtil.getFileIcon(fileInfo.fileName);
            var ext = uc.util.StringUtil.getFileDisplayExt(fileInfo.fileName);
            if (fileInfo && fileInfo.cloudType==0 && fileInfo.contentType == 1) {//代码片段
                $('.saveCloud', this.itemList).hide();
                iconSrc = "../images/sfileicons/code.png";
                ext = "code";
            }
            if(this.chatType== uc.constants.Chat.ChatType.CloudChat){
                $('.saveCloud', this.itemList).hide();
            }
            $('.file-info .icon', this.itemList).attr('src', iconSrc);
            $('.file-info .file-ext', this.itemList).text(ext);
            $('.file-info .detail .name', this.itemList).text(fileInfo.fileName);
            $('.file-info .detail .name', this.itemList).attr("title", fileInfo.fileName);

            var dateStr = "";
            var uploadTime=(''+fileInfo.uploadTime).length==10 ? (parseInt(fileInfo.uploadTime)*1000) : fileInfo.uploadTime;
            console.log(uploadTime);
            var date = new Date(uploadTime);
            var today = new Date();
            if (date.getYear() == today.getYear()){
                var month = (date.getMonth() + 1) < 10 ? '0' + (date.getMonth() + 1) : date.getMonth() + 1;
                var day = date.getDate() < 10 ? '0' + date.getDate() : date.getDate();
                dateStr = month + "/" + day;
            } else {
                try {
                    dateStr = date.toISOString().slice(0,10);
                } catch(e){
                    console.log(fileInfo.uploadTime);
                }
            }
            $('.file-info .detail .time', this.itemList).text(dateStr + " " +
                uc.util.TimeUtil.getMessageTimeString(new Date(fileInfo.uploadTime), uc.constants.UI.TimeFormat.Meeting.HourMinutes, uc.settings.meeting.timeZone.offset));
            $('.file-info .detail .size', this.itemList).text(uc.util.StringUtil.formatFileSize(fileInfo.fileSize));
            $('.file-info .detail .user', this.itemList).text(username);
            this.commentList.empty();
            this.lastMsgSeq = 0;
            this.getMoreComments();
            setTimeout(function () {
                $('.text-input').focus();
            }, 0);
        },
        OnGetFileInfo:function(data){
            var data=JSON.parse(data);
            if(data){
                this.fileInfo.fileName=data.filename;
                this.fileInfo.fileSize=data.filesize;
                var username=data.create_member_name;
                $('.file-info .detail .size', this.itemList).text(uc.util.StringUtil.formatFileSize(data.filesize));
                $('.file-info .detail .name', this.itemList).text(data.filename).attr("title",data.filename);
                if(username)$('.file-info .detail .user', this.itemList).text(username);
            }
        },
        getMoreComments: function() {
            this.onMoreMsgLoading();
            console.log("getMoreComments", this.fileInfo, this.lastMsgSeq);
            uc.IGroupService.getCommentList(this.groupId, this.fileInfo.fileId, 30, 0,  Number(this.lastMsgSeq),
                function(errorCode){
                    if (errorCode != 0) {
                        console.log("getMoreComments", this.groupId, errorCode);
                        this.onMoreMsgLoadEnd();
                    }
                }
            );
        },

        OnFileDownloading: function(itemId, percent){
            var item = $('#' + itemId, this.itemList);
            if (item.length <= 0) {
                return;
            }
            $('.progress', item).show();
            $('.progress', item).attr("value", percent);
            $('.download', item).hide();
        },
        OnFileDownloaded: function(errorCode, itemId, savePath){
            var item = $('#' + itemId, this.itemList);
            if (item.length <= 0) {
                return;
            }
            $('.progress', item).hide();
            $('.progress', item).attr("value", "0");
            $('.download', item).show();
            if (errorCode == 0) {
                $('.download', item).text(uc_resource.Comment.Open);
                $('.download', item).attr('local-path',savePath);
            }
        },

        OnCommentListReceived: function(errorCode, groupId, comments){
            if (comments.length == 0 && errorCode == 0 && $(".comment-item", this.itemList).length == 0){
                $(".no-comment", this.node).show();
            } else {
                $(".no-comment", this.node).hide();
            }

            if (groupId != this.groupId) {
                return;
            }

            this.onMoreMsgLoadEnd();
            if (errorCode != 0) {
                return;
            }
            if (comments.length < 30) {
                this.onLoadAllMsgEnd();
            }

            for (var i = 0; i < comments.length; i++) {
                this.addComment(true, comments[i].userId, comments[i].userName, comments[i].content, comments[i].createTime);
            }

            if (this.lastMsgSeq == 0) {
                var obj = $(".comment-box", this.node)
                obj.scrollTop(obj[0].scrollHeight);
            }

            if (comments.length > 1) {
                this.lastMsgSeq = comments[comments.length - 1].createTime;
            }
        },
        OnCommentCreated: function(errorCode, groupId, comment,msgId){
            if (groupId != this.groupId) {
                return;
            }
            if (this.fileInfo == null || comment.fileId != this.fileInfo.fileId) {
                return;
            }
            if (errorCode != 0) {
                this.showCommentFailView(msgId);
                return;
            }
           this.updateCommentView(comment,msgId);
        },
        showCommentFailView:function(msgId){
            $("#"+msgId).find(".send-fail").show();
        },
        showCommentView:function(comment,msgId){
            this.addComment(false, comment.userId, comment.userName, comment.content, comment.createTime,msgId);
            var obj = $(".comment-box", this.node);
            obj.scrollTop(obj[0].scrollHeight);
        },
        updateCommentView:function(comment,msgId){
            var msgseq=comment.createTime;
            var tmstr;
            if (new Date(msgseq).getYear() == new Date().getYear()) {
                tmstr = uc.util.TimeUtil.getMessageTimeString(new Date(msgseq), uc.constants.UI.TimeFormat.Message.MonthDay, uc.settings.meeting.timeZone.offset) + " "
                    + uc.util.TimeUtil.getMessageTimeString(new Date(msgseq), uc.constants.UI.TimeFormat.Message.HourMinutes, uc.settings.meeting.timeZone.offset);
            } else {
                tmstr = uc.util.TimeUtil.getMessageTimeString(new Date(msgseq), uc.constants.UI.TimeFormat.Message.YearMonthDay, uc.settings.meeting.timeZone.offset) + " "
                    + uc.util.TimeUtil.getMessageTimeString(new Date(msgseq), uc.constants.UI.TimeFormat.Message.HourMinutes, uc.settings.meeting.timeZone.offset);
            }
            $("#"+msgId).find(".timestamp").text(tmstr);
        },
        OnCommentCreateMessageReceived: function(fileId, userName, userId, content, createTime){
            if (this.fileInfo == null || fileId != this.fileInfo.fileId) {
                return;
            }
            if (userId ==  uc.modules.comment.CommentManager.myUserInfo.userId) {
                return;
            }
            this.addComment(false, userId, userName, content, createTime);
            var obj = $(".comment-box", this.node);
            obj.scrollTop(obj[0].scrollHeight);
        },
        addComment: function(isPrepend, userId, userName, content, msgseq,msgId) {
            $(".no-comment", this.node).hide();
            if(!msgId){
                msgId=msgseq;
            }
            var domStr = "";
            var text = uc.util.StringUtil.htmlEncode(content);
            var tmstr;
            if (new Date(msgseq).getYear() == new Date().getYear()) {
                tmstr = uc.util.TimeUtil.getMessageTimeString(new Date(msgseq), uc.constants.UI.TimeFormat.Message.MonthDay, uc.settings.meeting.timeZone.offset) + " "
                    + uc.util.TimeUtil.getMessageTimeString(new Date(msgseq), uc.constants.UI.TimeFormat.Message.HourMinutes, uc.settings.meeting.timeZone.offset);
            } else {
                tmstr = uc.util.TimeUtil.getMessageTimeString(new Date(msgseq), uc.constants.UI.TimeFormat.Message.YearMonthDay, uc.settings.meeting.timeZone.offset) + " "
                    + uc.util.TimeUtil.getMessageTimeString(new Date(msgseq), uc.constants.UI.TimeFormat.Message.HourMinutes, uc.settings.meeting.timeZone.offset);
            }
                var contactInfo = uc.IContactService.getContactInfo(userId);
                var localAvatar = null;
                if (contactInfo) {
                    localAvatar = contactInfo.localAvatar;
                    if (localAvatar == null || localAvatar == "") {
                        localAvatar = uc.modules.comment.CommentManager.getUserLogo(userId, contactInfo.remoteAvatarUrl);
                    }
                }
                var headerDom =  '<img class="header" rel="logo' + userId + '" src="../images/avatar_default.png"/>';
                if (localAvatar != null &&  localAvatar != ""){
                    //localAvatar = localAvatar + "?" + new Date().getTime();
                    headerDom =  '<img class="header" rel="logo' + userId + '"  src="' + localAvatar + '"/>';
                }
                domStr =            '<li class= "comment-item" id="'+msgId+'">'
                    +                   headerDom
                    +                    '<div class="context">'
                    +                    '<div class="left-text-info">'
                    +                        '<div class="user" rel="' + userId + '">' + userName + '</div>'
                    +                        '<div class="timestamp">' + tmstr + '</div>'
                    +                    '</div>'
                    +                    '<div class="content">' + text + '</div>'
                    +                    '</div>'
                    +                    '<div><img class="send-fail"  src="../images/messagebox/sendfail.png" title="'+uc_resource.Comment.CommentSendFail+'" style=" display: none; "></div>'
                    +                '</li>';
/*
            } else {
                domStr =            '<li class= "comment-item">'
                    +                    '<div class="right-text-info">'
                    +                        '<div class="user">' + userName + '</div>'
                    +                        '<div class="content">' + text + '</div>'
                    +                        '<div class="timestamp">' + tmstr + '</div>'
                    +                    '</div>'
                    +                '</li>';
            }
*/
            if (isPrepend) {
                this.commentList.prepend(domStr);
            } else {
                this.commentList.append(domStr);
            }
        },

        sendComment: function() {
            var This = this;
            var text = $(".text-input", this.itemList).val();
            text= $.trim(text);
            text = text.substring(0, 200);
            if (text != null && text != "") {
                uc.IGroupService.createComment(this.groupId, this.fileInfo.fileId,
                    0, this.fileInfo.fileName, text, function(args){
                        if (args[0] == 0) {
                            $(".text-input", This.itemList).val("");
                            var msgId=args[1];
                            This.showCommentView({
                                "userId":This.myUserInfo.userId,
                                "userName":This.myUserInfo.displayName,
                                "content":text,
                                "createTime": new Date().getTime()
                            },msgId);
                            uc.IUIEvent.OnMessageSending({
                                chatId: This.groupId,
                                chatType: This.chatType,
                                fileName:This.fileInfo.fileName,
                                msgId: args[1],
                                contentType: uc.constants.Message.ContentType.Comment
                            })
                        }
                    });
            }
        },
        updateUserLog: function(userId, localAvatarUrl){
            if ( this.commentList == null) {
                return;
            }
           // localAvatarUrl = localAvatarUrl + "?" + new Date().getTime();
            this.commentList.find("img[rel=logo" + userId + "]").attr("src", localAvatarUrl);
        },
        show: function(bshow) {
            var This = this;
            var w =  document.body.clientWidth;
            if (bshow) {
                $(".item-list",this.parentNode).hide();
                this.parentNode.show();
                this.parentNode.css({left:(w - 50) + "px", right:"0px", opacity:"0.3"});
                this.parentNode.animate( {left:"460px", opacity:"1"}, 200, "linear", function(){});
            } else {
                this.parentNode.animate( {left:w + "px", opacity:"0.3"}, 200,  "linear", function(){
                    This.parentNode.hide();
                });
            }
        },
        isShow: function() {
            return this.parentNode.is(":visible");
        },
        completeShow:function(){
            $("#commentLoading").hide();
            $(".item-list",this.parentNode).show();
            this.parentNode.show();
        },
        commentListShow:function(){
            $("#commentLoading").hide();
            $(".item-list",this.parentNode).show();
            var w =  document.body.clientWidth;
            this.parentNode.show();
            this.parentNode.css({left:(w - 50) + "px", right:"0px", opacity:"0.3"});
            this.parentNode.animate( {left:"460px", opacity:"1"}, 200, "linear", function(){});
        },
        setCloudIcon:function(){
            if(uc.modules.cloud.CloudManager.checkCloudIsLoginSuccess()==0 || uc.modules.cloud.CloudManager.checkCloudTokenIsSuccess()!=0){
                $('.saveCloud').hide();
            }
        }
    })
})(uc);