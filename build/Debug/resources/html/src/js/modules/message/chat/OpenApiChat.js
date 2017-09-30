/**
 * Created by senlin.ouyang on 2015/7/21.
 */
(function(uc){
    uc.modules.chat.OAMsgStatus = {
    };
    uc.modules.chat.OAMsgOpStuts = {
    };
    uc.modules.chat.OpenApiChat = {
        init: function(appId, appInfo){
            this.nodeHtml =    '<div class="line" />'
            +                   '<div class="title">'
            +                       '<div class="name" id="chatName"></div>'
            +                   '</div>'
            +                   '<div class="content">'
            +                       '<div class="filter hide"><div class="condition">'+ uc_resource.OpenApi.OpenApiFilterAll
            +                           '</div><div class="filter-list"><div class="active" condition="All">'+ uc_resource.OpenApi.OpenApiFilterAll +'</div>'
            +                                   '<div condition="Untreated">'+ uc_resource.OpenApi.OpenApiFilterPending +'</div>'
            +                                   '<div condition="Treated">'+ uc_resource.OpenApi.OpenApiFilterPended +'</div>'
            +                                   '<div condition="Other">'+ uc_resource.OpenApi.OpenApiFilterOther +'</div>'
            +                           '</div>'
            +                       '</div>'
            +                   '</div>'
            +                   '<div class="sign" id="chatSign" style="display:none"></div>';
            this.headerNode = $('<div class="chat-header"></div>');
            this.headerNode.append(this.nodeHtml);
            uc.ui.Message.ContentPanelBody.empty();
            this.headerNode.appendTo(uc.ui.Message.ContentPanelBody);
            this.chatName = $('#chatName', this.headerNode);
            this.chatSign = $('#chatSign', this.headerNode);
            this.condition = $('.condition', this.headerNode);
            this.filterCondition = 0;
            this.filterList = $('.filter-list', this.headerNode);
            this.appId = appId;
            this.appInfo = appInfo ? appInfo : uc.IOpenApiService.getAppInfo([appId]);
            this.chatType = uc.constants.Chat.ChatType.OpenApiChat;
            this.appOAMsgs = {};
            this.seqs = {};
            this.myUserInfo = uc.modules.chat.ChatManager.myUserInfo;

            this.myUserInfo = uc.modules.chat.ChatManager.myUserInfo;
            this.siteId = this.myUserInfo.siteId;
            this.updateChatName("" + appId);

            this.node = $('<div class="openapi-chat message-box-container"></div>').appendTo(uc.ui.Message.ContentPanelBody);
            this.msgBox = uc.modules.chat.ChatManager.msgBox;
            this.msgBox.init(this.node, this);
            this.historyDomStr = "";
            this.lastMsgSeq = 0;
            this.firstGetHistory = true;
            this.getMoreHistoryMsg();
            this.middleLine = $('<div class="middle-line"></div>').appendTo(uc.ui.Message.ContentPanelBody);

            this.inputNode = $("<div class='chat-input'/>").appendTo(uc.ui.Message.ContentPanelBody);
            this.msgInputBox = null;

            this.bSend = (appInfo ? appInfo.enableSendMessage ? true : false : false);
            if (this.bSend && this.appInfo.status) {
                this.msgInputBox = uc.modules.chat.ChatManager.msgInputBox;
                this.setMsgInputEnable();
            } else {
                this.setMsgInputDisEnable();
            }

            this.msgBox.bindResize(uc.ui.Message.ContentPanelBody, this.msgInputBox);

            if(this.appInfo){
                this.updateChatName(this.appInfo.appTitle, this.appInfo.appDesc);
            }
            this.bindEvent();

        },
        bindEvent: function () {
            var self = this;

            this.condition.on('click', function () {
                $(this).parent().toggleClass('open');
            });

            this.filterList.on('click', 'div', function () {
                var condition = $(this).attr('condition');
                self.filterCondition = uc.constants.Api.filter[condition] || 0;
                $(this).siblings('div').removeClass('active');
                $(this).addClass('active').parents('.filter').removeClass('open');
                self.condition.text($(this).text());
                self.msgBox.OnOAMessageFilterReceived(self.filterCondition);
            });
        },
        updateAppInfo: function(appInfo) {
            this.appInfo = appInfo;
            this.updateChatName(appInfo.appTitle, this.appInfo.appDesc);
            if (this.appInfo.enableSendMessage && this.appInfo.status) {
                this.setMsgInputEnable();
            } else {
                this.setMsgInputDisEnable();
            }
        },
        setMsgInputEnable: function () {
            this.msgInputBox.init(this.inputNode, {
                chatType: uc.constants.Chat.ChatType.OpenApiChat,
                chatObjectId: this.appId,
                siteId: this.siteId,
                resId: 100,
                moduleId: uc.constants.UI.ModuleId.Message
            });
            this.msgInputBox.enable(true);
            this.msgInputBox.disableAction("code");
            this.msgInputBox.disableAction("history");
	        this.msgInputBox.disableAction("cloud");
	        this.msgInputBox.disableAction("uploadMeetingDoc");
            this.msgBox.setNormal();
            this.middleLine.show();
            this.inputNode.show();
        },
        setMsgInputDisEnable: function () {
            if (this.msgInputBox) {
                this.msgInputBox.enable(false);
            }
            this.middleLine.hide();
            this.msgBox.setMax();
            //this.inputNode.empty().hide();  // old code

            // added by xue.bai_2 on 2016/03/07 for fixing bug:
            // 0003007: ��PC-2.1.267.73953��
            // �ͻ���ͣ����Ӧ�����촰�ڣ�ucadmin�رա�Ӧ����Ϣ�Ƿ���䡿���ٴ򿪣�Ӧ�����촰����ʾ�쳣��
            this.inputNode.hide();
        },
        updateChatName: function(name, sign) {
            /*name = ("" + name) || "";
            this.chatName.html(uc.util.StringUtil.escapeHtml(name, true));*/
            
            name= uc.util.StringUtil.escapeHtml(name);
            sign= uc.util.StringUtil.escapeHtml(sign);
            this.chatName.html(name).attr({"qtips":name,"qtips-show-by":"mouse"});

            if (sign != null && sign != "") {
                this.chatSign.text(sign);
                this.chatSign.show();
                this.chatName.removeClass('center');
            } else {
                this.chatName.addClass('center');
            }
        },
        getMoreHistoryMsg: function() {
            this.historyDomStr = "";
            var msgBox = this.msgBox;
            msgBox.onMoreMsgLoading();
            console.log("getChatHistory", this.appId, 10, 0, Number(this.lastMsgSeq));
            uc.IOpenApiService.getChatHistory(this.appId, uc.constants.Message.OnceGetHistoryCount, 0, Number(this.lastMsgSeq), function(errorCode){
                if (errorCode != 0) {
                    console.log(errorCode);
                    msgBox.onMoreMsgLoadEnd();
                }
            });
        },
        onChatHistoryReceived: function(errorCode, appId, msgCount, minTimestamp) {
            if (this.firstGetHistory) {
                uc.modules.chat.ChatManager.appendUploadMessages();
            }

            if (errorCode == 0) {
                this.lastMsgSeq = minTimestamp;
            }
            this.msgBox.onMoreMsgLoadEnd();
            if (msgCount < uc.constants.Message.OnceGetHistoryCount && errorCode == 0) {
                this.msgBox.onLoadAllMsgEnd();
            }
            if (this.historyDomStr == "") {
                this.firstGetHistory = false;
                this.msgBox.OnOAMessageFilterReceived(this.filterCondition);
                return;
            }
            this.msgBox.prependMessage(this.historyDomStr);
            if (this.firstGetHistory) {
                this.msgBox.scrollToEnd();
            }
            this.firstGetHistory = false;
            this.historyDomStr = "";
            this.updateAllOpStatus();
            this.msgBox.OnOAMessageFilterReceived(this.filterCondition);
        },
        procMessageDomStr: function(channel, domStr, msgId, msgSeq) {
            if (this.seqs[msgSeq] != null){
                console.log("the msg seq has been existed!", msgSeq);
                return;
            }
            this.seqs[msgSeq] = true;

            //this.lastMsgSeq = msgSeq;
            if (this.msgBox.isMsgIdExist(msgId)) {
                console.log("the msgId has been existed!", msgId);
                return;
            }
            if (uc.constants.Message.MessageChannel.ChatHistory == channel) {
                this.historyDomStr = domStr + this.historyDomStr;
                return;
            }
            this.msgBox.appendMessage(domStr);
            this.updateAllOpStatus();
        },
        // added by xue.bai_2 on 2016/04/07
        OnRevocationMessageReceived: function(conversationId, conversationType, from, to, msgId, msgSeq, msgTime){
            var message = uc.util.StringUtil.formatString(uc_resource.Message.RevocationMessage, [this.getMessageUserName(from.userId)]);
            var domStr = this.msgBox.getSysMsgDomStr(message, msgTime);
            this.procMessageDomStr(uc.constants.Message.MessageChannel.UCAS, domStr, msgId, msgSeq);
        },
        getMessageUserName: function(userId){
            var userName;
            if(userId == this.myUserInfo.userId){
                userName = uc_resource.SingleChat.Me;
            }else{
                var contactInfo = uc.IContactService.getBasicContactInfo(userId);
                if(contactInfo){
                    userName = contactInfo.displayName;
                }else{
                    userName = '<span rel="' + userId + '">' + userId + '</span>' ;
                }
            }
            return userName;
        },
        isSendByMyself: function (from) {
            if (from.userId != this.myUserInfo.userId && this.appInfo != null){
                from.displayName = this.appInfo.appTitle;
                from.localAvatar = this.appInfo.appLogo;
            }
            return from.userId == this.myUserInfo.userId;
        },
        OnOpenApiTextMessageReceived: function(channel, sendState, from, to, textTitle, textContent, textType, detailContent, msgId, msgSeq, mstTime, detailAuth){
            var self = this.isSendByMyself(from);
            if (!self && (textTitle != "" || detailContent != "")) {
                var str = this.msgBox.getApiTextMsgDomStr(sendState, self, from, msgId, msgSeq, textTitle, textContent, textType, detailContent, msgSeq, detailAuth);
                this.procMessageDomStr(channel, str, msgId, msgSeq);
                return;
            }
            var domStr = this.msgBox.getTextMsgDomStr(sendState, self, from, msgId, msgSeq, textType, textContent, msgSeq);
            this.procMessageDomStr(channel, domStr, msgId, msgSeq);
        },
        OnOpenApiImageMessageReceived: function(channel, sendState, from, to, imageId, imageType, imageData, msgId, msgSeq, msgTime){
            var domStr = "";
            if (imageType == 2) {
                domStr = this.msgBox.getApiImgMsgDomStr(sendState, this.isSendByMyself(from), from, msgId, msgSeq, "", 0, imageId, imageData, msgSeq);
            } else {
                if (imageType == 1){
                    imageId = this.getUrlByFid(imageId);
                }
                domStr = this.msgBox.getImgMsgDomStr(sendState, this.isSendByMyself(from), from, msgId, msgSeq, "", 0, imageId, imageData, msgSeq);
            }
            this.procMessageDomStr(channel, domStr, msgId, msgSeq);
        },
        OnOpenApiFileMessageReceived: function(channel, sendState, from, to, fileId, fileType, fileName, fileSize, msgId, msgSeq, mstTime){
            var domStr = "";
            if (fileType != 0 && fileType != 1) {
                domStr = this.msgBox.getApiFileMsgDomStr(sendState, this.isSendByMyself(from), from, msgId, msgSeq,fileName, fileSize, fileId, msgSeq, null, "");
            } else {
                if (fileType == 1){
                    fileId = this.getUrlByFid(fileId);
                }
                domStr = this.msgBox.getFileMsgDomStr(sendState, this.isSendByMyself(from), from, msgId, msgSeq,fileName, fileSize, fileId, msgSeq, null, "");
            }
            this.procMessageDomStr(channel, domStr, msgId, msgSeq);
        },
        OnOAMessageReceived: function(channel, from, to, title, titleColor, titleBkColor, titleElements, titleStyle, status, data, detailUrl, msgId, msgSeq,msgTime, detailAuth, oaExternalData) {
            var s = this.getApiStatus(this.appId, msgSeq);
            if (s != null) {
                status = s;
            }
            this.appOAMsgs[String(msgSeq)] = {
                msgId: msgId,
                title: title,
                titleStyle: titleStyle,
                titleColor: titleColor,
                titleBkColor: titleBkColor,
                titleElements: titleElements,
                status: status,
                data: data,
                detailUrl: detailUrl,
                detailAuth: detailAuth,
                oaExternalData: oaExternalData
            };
            var domStr = this.msgBox.getApiOAMsgDomStr(msgId, msgSeq, title, titleColor, titleBkColor, titleElements, titleStyle, status, data, detailUrl, detailAuth, oaExternalData);
            this.procMessageDomStr(channel, domStr, msgId, msgSeq);

            if (s == null) {
                var appId = this.appId;
                var info = [{
                    appId: Number(appId),
                    msgSeq: Number(msgSeq)
                }];
                uc.util.LogUtil.info('OpenApiChat', 'getOAMessageStatus', JSON.stringify(info), null);
                uc.IOpenApiService.getOAMessageStatus(
                    info,
                    function(args){
                        uc.util.LogUtil.info('OpenApiChat', 'OnGetOAMessageStatus', JSON.stringify(args), null);
                    }
                );
            }
        },
        OnOAMessageStatusReceived: function(errorCode, appId, msgSeq, status) {
            if (errorCode != 0 ) {
                return;
            }
            for (var i = 0; i < status.length; i++){
                var stObj = status[i];
                if (stObj == null || stObj.appId == null || stObj.msgSeq == null || stObj.status == null || stObj.status == -1){
                    continue;
                }
                this.setApiStatus(stObj.appId, stObj.msgSeq, stObj.status);
                this.removeApiOpStatus(stObj.appId, stObj.msgSeq);
                if (this.msgBox != null && stObj.appId == this.appId){
                    this.msgBox.updateApiActionStatus(stObj.msgSeq, stObj.status, this.filterCondition);
                }
            }
        },
        OnOAMessageStatusChanged: function(appId, msgSeq, status, from, extraData) {
            if (appId == null) {
                appId = from.userId;
            }
            uc.util.LogUtil.info('OpenApiChat', 'OnOAMessageStatusChanged', "appId: " + appId + ", msgSeq: "
                + msgSeq + ", status: " + status, null);
            this.setApiStatus(appId, msgSeq, status);
            this.removeApiOpStatus(appId, msgSeq);
            if (this.msgBox != null && this.appId == appId){
                this.msgBox.updateApiActionStatus(msgSeq, status, this.filterCondition, extraData);
            }
        },
        OnPushMessageAck: function(channel, errorCode, from, to, conversation, msgId, msgSeq, msgTime){

        },
        resendMessage: function (msgId){
            var This = this;
            uc.IOpenApiService.resendMessage(msgId,
                function(args){
                    if (args[0] != 0) {
                        This.msgBox.showSendFailFlag(msgId, true);
                    }
                });
        },
        sendApiRequest: function (msgSeq, url, method, body, index){
            var contentType = {"Content-Type": "application/json"};
            var This = this, params = [], method = method || 1, body = body ? JSON.stringify(body) : '';
            if (method == uc.constants.Http.Post) {
                params.push(contentType);
            }
            this.setApiOpStatus(this.appId, msgSeq, "busy");
            uc.util.LogUtil.info('OpenApiChat', 'sendRequest', url, params);
            uc.IOpenApiService.sendRequest(method, url, body, params, function(args){
                uc.util.LogUtil.info('OpenApiChat', 'OnResponse', 'args', args);
                var status = null, approveSuggestion = '';
                if (args[0] == 0) {
                    try {
                        var obj = JSON.parse(args[1]);
                        if (obj != null && Object.prototype.toString.call(obj) == "[object Object]"){
                            status = obj["status"];
                            approveSuggestion = obj.externalData;
                        }
                    }catch(e){
                        uc.util.LogUtil.info('OpenApiChat', 'OnResponse', "JSON parse fail! ", null);
                    }
                }
                if (status != null) {
                    This.removeApiOpStatus(This.appId, msgSeq);
                    This.setApiStatus(This.appId, msgSeq, status)
                } else {
                    This.setApiOpStatus(This.appId, msgSeq, "fail");
                }
                This.msgBox.updateApiActionStatus(msgSeq, status, This.filterCondition, approveSuggestion, index);
            });
        },
        setApiStatus: function (appId, msgSeq, status){
            if (appId == null || msgSeq == null) {
                return;
            }
            if (uc.modules.chat.OAMsgStatus[String(appId)] == null) {
                uc.modules.chat.OAMsgStatus[String(appId)] = {};
            }
            uc.modules.chat.OAMsgStatus[String(appId)][String(msgSeq)] = status;
        },
        getApiStatus: function (appId, msgSeq) {
            if (appId == null || msgSeq == null) {
                return null;
            }
            if (uc.modules.chat.OAMsgStatus[String(appId)] == null) {
                return null;
            }
            return uc.modules.chat.OAMsgStatus[String(appId)][String(msgSeq)];
        },
        setApiOpStatus: function (appId, msgSeq, status){
            if (appId == null || msgSeq == null) {
                return;
            }
            if (uc.modules.chat.OAMsgOpStuts[String(appId)] == null) {
                uc.modules.chat.OAMsgOpStuts[String(appId)] = {};
            }
            uc.modules.chat.OAMsgOpStuts[String(appId)][String(msgSeq)] = status;
        },
        removeApiOpStatus: function (appId, msgSeq) {
            if (appId == null || msgSeq == null) {
                return;
            }
            if (uc.modules.chat.OAMsgOpStuts[String(appId)] == null) {
                return;
            }
            delete uc.modules.chat.OAMsgOpStuts[String(appId)][String(msgSeq)];
        },
        getApiOpStatus: function (appId, msgSeq) {
            if (appId == null || msgSeq == null) {
                return null;
            }
            if (uc.modules.chat.OAMsgOpStuts[String(appId)] == null) {
                return null;
            }
            return uc.modules.chat.OAMsgOpStuts[String(appId)][String(msgSeq)];
        },
        getApiOpStatusBySeq: function (msgSeq) {
            return this.getApiOpStatus(this.appId, msgSeq);
        },
        updateAllOpStatus: function() {
            var appOpstatus = uc.modules.chat.OAMsgOpStuts[String(this.appId)];
            if (this.appId == null || appOpstatus == null) {
                return;
            }
            for (var key in appOpstatus) {
                var opstatus = appOpstatus[key];
                if (opstatus == null) {
                    continue;
                }
                this.msgBox.updateApiActionOpStatus(key, opstatus);
            }
            this.msgBox.OnOAMessageFilterReceived(this.filterCondition);
        },
        getOAMsgObj: function(msgSeq) {
            return this.appOAMsgs[String(msgSeq)];
        },
        getUrlByFid: function(url){
            try{
                //return "http://192.168.35.145/msgtest/file/get?id=" + url;
                return  uc.IFSService.getDownloadUrlFromFid(url);
            }catch(e){
            }
            return url;
        },
        getImageUrl: function(imgType, url){
            var tp = imgType.toLowerCase();
            if (tp == "fileid"){
               return  this.getUrlByFid(url);
            }
            return url;
        },
        clear: function(){
            if(this.msgBox != null){
                this.msgBox.clear();
                this.msgBox = null;
            }
            
            if(this.msgInputBox != null){
                this.msgInputBox.clear();
                this.msgInputBox = null;
            }

            uc.ui.Message.ContentPanelBody.find('*').off();
            uc.ui.Message.ContentPanelBody.empty();
        }
    };
})(uc);