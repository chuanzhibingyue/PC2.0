/**
 * Created by ge.xie on 2016/1/27.
 **/
(function(uc){
    uc.modules.chat.RtxBox = function (options) {
        this.chatType = options.chatType;
        this.chatId = options.chatId;
        this.lastMsgStamp = 0;
    };

    jQuery.extend(uc.modules.chat.RtxBox.prototype, {
        init: function(parentNode) {
            this.nodeHtml = '<div class="more-msg-line">'
                +               '<span class="more-msg-loading hide"></span>'
                +               '<span class="more-msg"" >'+ uc_resource.Message.ViewMore +'</span>'
                +           '</div>'
                +           '<ul class="external-chat-list">'
                +           '</ul>';

            this.nodeItemHtml = '<li class="message-item" seq="{{CHAT_SEQ}}"><div>'
                +                   '<div class="{{CHAT_POSITION}}">'
                //+                       '<img class="user-logo" src="../images/avatar_default.png" />'
                +                       '<img class="user-logo" src="{{USER_AVATAR}}" />'
                +                       '<div class="info">'
                +                           '<span class="user-name">{{USER_NAME}}</span>'
                +                           '<span class="message-time">{{CHAT_TIME}}</span>'
                +                       '</div>'
                +                   '</div>'
                +                   '<div class="{{CHAT_DIRECTION}}">'
                +                       '<div>{{CHAT_MSG}}</div>'
                +                   '</div>'
                +               '</div></li>';

            this.nodeSystemItemHtml = '<li class="sys-message-item" rel="sys-time-item">'
                +                         '<div class="sys-message-box">'
                +                             '<span class="sys-text">{{CHAT_DATE}}</span>'
                +                         '</div>'
                +                     '</li>';

            this.node = $('<div class="RtxBox"></div>');
            this.node.appendTo(parentNode);
            this.node.append(this.nodeHtml);
            this.moreMsgLine = $('.more-msg-line', this.node);
            this.moreMsgLoading = $('.more-msg-loading', this.node);
            this.moreMsg = $('.more-msg', this.node);
            this.chatList = $('.external-chat-list');
            //this.nodeItem = $(this.nodeItemHtml);
            this.lastMsgSeq = 0;
            this.currentUserId = uc.IContactService.getCurrentUserInfo().userId;
            this.bindEvent();

            this.getHistoryMessage();

            this.scrollToEnd();
        },

        getHistoryMessage: function() {
            uc.IIMService.getExternalMessageHistory(this.chatType, this.chatId, uc.constants.Message.OnceGetHistoryCount, 0, this.lastMsgSeq);
        },

        msgShowed: function(msgList) {

            var todayStamp = new Date().getTime();
            var yesterdayStamp = todayStamp - 3600 * 24 * 1000;

            for (var i = 0,length = msgList.length; i < length; i ++) {
                var chatMsgStr = "";
                var nodeItemHtml = this.nodeItemHtml;
                var nodeSystemItemHtml = "";

                nodeItemHtml = nodeItemHtml.replace("{{CHAT_SEQ}}", msgList[i].msgSeq);
                if (msgList[i].from.isSelf) {
                    nodeItemHtml = nodeItemHtml.replace("{{CHAT_POSITION}}", "right-item");
                    nodeItemHtml = nodeItemHtml.replace("{{CHAT_DIRECTION}}", "text-me-to");
                    nodeItemHtml = nodeItemHtml.replace("{{USER_AVATAR}}", "");
                } else {
                    nodeItemHtml = nodeItemHtml.replace("{{CHAT_POSITION}}", "left-item");
                    nodeItemHtml = nodeItemHtml.replace("{{CHAT_DIRECTION}}", "text-to-me");
                    if (msgList[i].from.localAvatar) {
                        nodeItemHtml = nodeItemHtml.replace("{{USER_AVATAR}}", msgList[i].from.localAvatar);
                    } else {
                        nodeItemHtml = nodeItemHtml.replace("{{USER_AVATAR}}", "../images/avatar_default.png");
                    }
                }

                var displayName = msgList[i].from.displayName ? msgList[i].from.displayName : msgList[i].from.userAccount;
                var textContent = msgList[i].textContent;
                textContent = uc.util.EmotionUtil.codeToEmotion(textContent);
                textContent = uc.modules.emoji.EmojiConverter.unicodeToSmiley(textContent);
                textContent = uc.modules.emoji.EmojiConverter.androidCodeToSmiley(textContent);
                textContent = uc.util.StringUtil.escapeHtml(textContent);
                nodeItemHtml = nodeItemHtml.replace("{{USER_NAME}}", displayName);
                nodeItemHtml = nodeItemHtml.replace("{{CHAT_TIME}}", uc.util.TimeUtil.getMeetingStartTimeString(msgList[i].msgTime / 1000, uc.constants.UI.TimeFormat.Meeting.HourMinutes, uc.settings.meeting.timeZone.offset).timeString);
                nodeItemHtml = nodeItemHtml.replace("{{CHAT_MSG}}", textContent);
                chatMsgStr += nodeItemHtml;

                if (this.lastMsgStamp !== 0 && new Date(this.lastMsgStamp).toDateString() !== new Date(Number(msgList[i].msgTime)).toDateString()) {
                    if (new Date(this.lastMsgStamp).toDateString() === new Date(todayStamp).toDateString()) {
                        nodeSystemItemHtml = this.nodeSystemItemHtml.replace("{{CHAT_DATE}}", uc_resource.Chat.Today);
                    } else if (new Date(this.lastMsgStamp).toDateString() === new Date(yesterdayStamp).toDateString()) {
                        nodeSystemItemHtml = this.nodeSystemItemHtml.replace("{{CHAT_DATE}}", uc_resource.Chat.Yesterday);
                    } else {
                        nodeSystemItemHtml = this.nodeSystemItemHtml.replace("{{CHAT_DATE}}", uc.util.TimeUtil.getSysMessageTimeStr(this.lastMsgStamp));
                    }
                }
                chatMsgStr += nodeSystemItemHtml;

                this.lastMsgStamp = Number(msgList[i].msgTime);
                this.node.find(".external-chat-list").prepend(chatMsgStr);
            }

            if (msgList.length < uc.constants.Message.OnceGetHistoryCount) {
                this.node.find(".external-chat-list").prepend(this.nodeSystemItemHtml.replace("{{CHAT_DATE}}", uc.util.TimeUtil.getSysMessageTimeStr(this.lastMsgStamp)));
            }

            this.onMoreMsgLoadEnd();
        },

        onMoreMsgLoading: function(){
            this.moreMsgLoading.removeClass("hide");
            this.moreMsg.addClass("hide");
        },

        onMoreMsgLoadEnd: function(){
            this.moreMsgLoading.addClass("hide");
            this.moreMsg.removeClass("hide");
        },

        onLoadAllMsgEnd: function(){
            this.moreMsgLine.addClass('hide');
        },

        scrollToEnd: function(){
            if (this.node) {
                this.node.scrollTop(this.node[0].scrollHeight);
            }
        },

        bindEvent: function() {
            var _this = this;
            this.node.on("scroll", function() {
                if (_this.moreMsg.is(":hidden")) {
                    return;
                }
                if (! _this.node.scrollTop()) {
                    _this.onMoreMsgLoading();
                    uc.IIMService.getExternalMessageHistory(_this.chatType, _this.chatId, uc.constants.Message.OnceGetHistoryCount, 0, _this.lastMsgSeq);
                }
            });

            this.node.on('click', '.more-msg', function () {
                _this.onMoreMsgLoading();
                _this.getHistoryMessage();
            });
        },


        /*************** OnExternalMessageReceived *****************/
        OnExternalMessageReceived: function (errorCode, conversationType, relateID, msgCount, minTimestamp, messages) {
            if (errorCode == uc.ErrorCode.Success) {
                this.lastMsgSeq = minTimestamp;
            }

            if (msgCount < uc.constants.Message.OnceGetHistoryCount && errorCode == uc.ErrorCode.Success) {
                this.onLoadAllMsgEnd();
            }
            this.msgShowed(messages);
        }
    });
})(uc);