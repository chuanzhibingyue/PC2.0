/**
 * Created by senlin.ouyang on 2015/7/23.
 */
(function(uc){
    uc.modules.conversation.ConversationView = function(){
        this.conversationInfo = null;
        this.nodeHtml ='<span class="avatar-node"></span>' +
                        '<div class="avatar-call-node icon-call_phoneVideo"></div>'+
                        '<div class="info"><div class="title"></div><div class="icon-super-group hide">'+ uc_resource.Common.SuperGroup +'</div><div class="icon-disturb hide"></div></div>' +
                        '<div class="info message"></div>' +
                        '<span class="msg-time"></span>'+
                        '<span class="msg-num"></span>';
        this.node = $('<li class="list-item conversation-item"></li>');
    };
    jQuery.extend(uc.modules.conversation.ConversationView.prototype, {
        init: function(options){
            this.node.append(this.nodeHtml);
            this.appendToParent(options.parentNode);
            this.avatarNode = $('.avatar-node', this.node);
            this.titleNode = $('.title', this.node);
            this.superGroupIcon = $('.icon-super-group', this.node);
            this.distrurbIcon = $('.icon-disturb',this.node);
            this.messageNode = $('.message', this.node);
            this.msgNumNode = $('.msg-num', this.node);
            this.msgTimeNode = $('.msg-time', this.node);
            this.msgInCall=  $('.avatar-call-node', this.node);
            this.bindEvent();
        },
        appendToParent: function(parentNode){
            this.node.appendTo(parentNode);
        },
        bindEvent: function(){
            this.node.click(uc.util.EventUtil.hitch(this, 'onConversationViewClicked'));
            this.node.contextmenu(uc.util.EventUtil.hitch(this, 'onConversationViewRightClicked'));
            //this.node.hover(uc.util.EventUtil.hitch(this, 'onHoverIn'), uc.util.EventUtil.hitch(this, 'onHoverOut'));
        },
        onConversationViewClicked: function(){
            uc.util.StorageUtil.setData("USER_STATUS:promptStatus", "1");
            this.node.addClass('active').siblings().removeClass('active');

            if (this.conversationInfo.conversationType == uc.constants.Conversation.ConversationType.OpenApi
                && uc.modules.notice.NoticeManager.isNoticeConversation(this.conversationInfo.conversationId)) {
                uc.IUIService.openChatWindow(uc.constants.Chat.ChatType.OpenApiChat, this.conversationInfo.conversationId, uc.constants.UI.OpenChatWindowSourceModule.Conversation, null, null, false);
                return;
            } else {
                uc.modules.notice.NoticeManager.hidePanel();
            }
            if (this.conversationInfo.selected) {
                return;
            }
            switch (this.conversationInfo.conversationType){
                case uc.constants.Conversation.ConversationType.SingleChat:
/*                    if (uc.constants.Switch.OpenUserStatus) {
                        uc.IContactService.subscribeUserStatus([this.conversationInfo.conversationId]);
                    }*/
                    uc.IUIService.openChatWindow(uc.constants.Chat.ChatType.SingleChat, this.conversationInfo.conversationId, uc.constants.UI.OpenChatWindowSourceModule.Conversation, null, null, false);
                    break;
                case uc.constants.Conversation.ConversationType.GroupChat:
                    uc.IUIService.openChatWindow(uc.constants.Chat.ChatType.GroupChat, this.conversationInfo.conversationId, uc.constants.UI.OpenChatWindowSourceModule.Conversation, null, null, false);
                    break;
                case uc.constants.Conversation.ConversationType.ProjectChat:
                    uc.IUIService.openChatWindow(uc.constants.Chat.ChatType.ProjectChat, this.conversationInfo.conversationId, uc.constants.UI.OpenChatWindowSourceModule.Conversation, null, null, false);
                    break;
                case uc.constants.Conversation.ConversationType.CloudChat:
                    uc.IUIService.openChatWindow(uc.constants.Chat.ChatType.CloudChat, this.conversationInfo.conversationId, uc.constants.UI.OpenChatWindowSourceModule.Conversation, null, null, false);
                    break;
                case uc.constants.Conversation.ConversationType.MeetingMsg:
                    var meetingData = {
                        meetingId: this.conversationInfo.meetingId,
                        eventId: this.conversationInfo.eventId,
                        startTime: this.conversationInfo.meetingStartTime,
                        isCancelled: this.conversationInfo.isMeetingCancelled,
                        meetingMsgType: this.conversationInfo.meetingMsgType || uc.constants.Meeting.MsgType.detail
                    };
                    uc.IUIService.openChatWindow(uc.constants.Chat.ChatType.MeetingChat, this.conversationInfo.conversationId, uc.constants.UI.OpenChatWindowSourceModule.Conversation, meetingData, null, false);
                    break;
                case uc.constants.Conversation.ConversationType.OpenApi:
                    var openApiData = {
                        isCancelled: this.conversationInfo.isCancelled
                    };
                    uc.IUIService.openChatWindow(uc.constants.Chat.ChatType.OpenApiChat, this.conversationInfo.conversationId, uc.constants.UI.OpenChatWindowSourceModule.Conversation, openApiData, null, false);
                    break;
                case uc.constants.Conversation.ConversationType.WelcomeMsg:
                    break;
                case uc.constants.Conversation.ConversationType.OrgMsg:
                    break;
            }
        },
        onConversationViewRightClicked: function (e) {
            if(!uc.modules.notice.NoticeManager.isNoticeConversation(this.conversationInfo.conversationId)){
              this.onConversationViewClicked();
            }
            switch (this.conversationInfo.conversationType){
                case uc.constants.Conversation.ConversationType.SingleChat:
                    uc.util.MenuUtil.showConversationContactContextMenu(e, this.conversationInfo.conversationId, this.conversationInfo);
                    break;
                case uc.constants.Conversation.ConversationType.GroupChat:
                    uc.util.MenuUtil.showConversationGroupContextMenu(e, this.conversationInfo.conversationId, this.conversationInfo);
                    break;
                case uc.constants.Conversation.ConversationType.ProjectChat:
                    uc.util.MenuUtil.showConversationProjectContextMenu(e, this.conversationInfo.conversationId, this.conversationInfo);
                    break;
                case uc.constants.Conversation.ConversationType.CloudChat:
                    uc.util.MenuUtil.showConversationCloudContextMenu(e, this.conversationInfo.conversationId, this.conversationInfo);
                    break;
                case uc.constants.Conversation.ConversationType.MeetingMsg:
                    var meetingData = {
                        groupId: this.conversationInfo.conversationId,
                        meetingId: this.conversationInfo.meetingId,
                        eventId: this.conversationInfo.eventId,
                        isGNet: this.conversationInfo.isGNet,
                        startTime: this.conversationInfo.meetingStartTime,
                        isCancelled: this.conversationInfo.isMeetingCancelled,
                        accreditUserId: this.conversationInfo.accreditUserId
                    };
                    uc.util.MenuUtil.showConversationMeetingContextMenu(e, meetingData, this.conversationInfo);
                    break;
                case uc.constants.Conversation.ConversationType.WelcomeMsg:
                case uc.constants.Conversation.ConversationType.OrgMsg:
                case uc.constants.Conversation.ConversationType.OpenApi:
                    uc.util.MenuUtil.showDeleteConversationContextMenu(e, this.conversationInfo.conversationId, this.conversationInfo.conversationType, this.conversationInfo);
                    break;
            }
        },
        onHoverIn: function(){

        },
        onHoverOut: function(){

        },
        setConversationInfo: function(conversationInfo){
            this.conversationInfo = conversationInfo;
            this.showConversationView();
        },
        showConversationView: function(){
            if(this.conversationInfo.selected){
                this.node.addClass('active');
            }else{
                this.node.removeClass('active');
            }
            this.showTitle();
            this.showMessageTime();
            this.showMessage();
            this.setNewMsgNum();
            this.setAvatar();
            this.setInCall();
            this.showSuperGroupIcon();
            this.showDisturbIcon();
            this.setTopIndex();
        },
        showTitle: function(){
            if(this.conversationInfo.conversationType == uc.constants.Conversation.ConversationType.MeetingMsg && this.conversationInfo.isMeetingCancelled ||
                this.conversationInfo.conversationType == uc.constants.Conversation.ConversationType.OpenApi && this.conversationInfo.isCancelled ){
                this.titleNode.addClass('cancelled');
            }else{
                this.titleNode.removeClass('cancelled');
            }

            //this.titleNode.html(uc.util.StringUtil.escapeHtml(this.conversationInfo.title, true));
            this.titleNode.text(this.conversationInfo.title);
        },
        showMessageTime: function(){
            if(this.conversationInfo.timestamp){
                this.msgTimeNode.text(uc.util.TimeUtil.getMessageTimeString(this.conversationInfo.timestamp, null, uc.settings.meeting.timeZone.offset));
            }else{
                this.msgTimeNode.text('');
            }
        },
        showMessage: function(){
            if(this.conversationInfo.selected){
                if(this.conversationInfo.sendingMessageStack.length > 0){
                    var sendingMessage = this.conversationInfo.sendingMessageStack[0];
                    if(this.conversationInfo.msgId > sendingMessage.msgId){
                        this.messageNode.html(this.conversationInfo.message);
                    }else{
                        if(sendingMessage.sendingStatus == uc.constants.Message.SendState.SendOK ||
                            sendingMessage.sendingStatus == uc.constants.Message.SendState.Sending){
                            this.messageNode.html(sendingMessage.content);
                        }else if(sendingMessage.sendingStatus == uc.constants.Message.SendState.SendFail){
                            this.messageNode.html('<span class="fail">' + uc_resource.Message.SendFailed + '</span>'+ sendingMessage.content);
                            this.conversationInfo.sendingMessageStack.splice(0, 1);
                        }else{
                            this.messageNode.html(this.conversationInfo.message);
                        }
                    }
                }else{
                    this.messageNode.html(this.conversationInfo.message);
                }
            }else{
                var draft = uc.modules.draft.DraftMessageManager.getDraftMessage(this.conversationInfo.conversationId, this.conversationInfo.conversationType);
                var atCounts = this.conversationInfo.atCounts;
                if(atCounts){
                    uc.util.LogUtil.info("ConversationView","showMessage","atCounts"+atCounts,{conversationId:this.conversationInfo.conversationId,conversationType:this.conversationInfo.conversationType});
                }

                if(draft && draft.draft){
                    if(atCounts){
                        this.messageNode.html('<span class="at-you">'+uc_resource.Message.AtTips+'</span>'+ draft.draft);
                    }else{
                        this.messageNode.html('<span class="draft">' + uc_resource.Message.Draft + '</span>'+ draft.draft);
                    }
                }else{
                    if(this.conversationInfo.sendingMessageStack.length > 0){
                        var sendingMessageData = this.conversationInfo.sendingMessageStack[0];
                        if(this.conversationInfo.msgId > sendingMessageData.msgId){
                            if(atCounts){
                                this.messageNode.html('<span class="at-you">'+uc_resource.Message.AtTips+'</span>'+this.conversationInfo.message);
                            }else{
                                this.messageNode.html(this.conversationInfo.message);
                            }
                        }else{
                            if(sendingMessageData.sendingStatus == uc.constants.Message.SendState.SendOK ||
                                sendingMessageData.sendingStatus == uc.constants.Message.SendState.Sending){
                                if(atCounts){
                                    this.messageNode.html('<span class="at-you">'+uc_resource.Message.AtTips+'</span>'+sendingMessageData.content);
                                }else{
                                    this.messageNode.html(sendingMessageData.content);
                                }
                            }else{
                                this.messageNode.html('<span class="fail">' + uc_resource.Message.SendFailed + '</span>'+ sendingMessageData.content);
                            }
                        }
                    }else{
                        if(atCounts){
                            this.messageNode.html('<span class="at-you">'+uc_resource.Message.AtTips+'</span>'+this.conversationInfo.message);
                        }else{
                            this.messageNode.html(this.conversationInfo.message);
                        }
                    }
                }
            }
        },
        setNewMsgNum: function(){
            var msgNumClass = '';
            var newMsgCount = this.conversationInfo.newMsgCount;
             if(newMsgCount < 0){
                 newMsgCount = 0;
             }
            switch (this.conversationInfo.conversationType){
                case uc.constants.Conversation.ConversationType.SingleChat:
                case uc.constants.Conversation.ConversationType.GroupChat:
                case uc.constants.Conversation.ConversationType.CloudChat:
                case uc.constants.Conversation.ConversationType.ProjectChat:
                case uc.constants.Conversation.ConversationType.WelcomeMsg:
                case uc.constants.Conversation.ConversationType.OpenApi:
                   if(!uc.modules.notice.NoticeManager.isNoticeConversation(this.conversationInfo.conversationId)){
                     if(newMsgCount == 0){
                         msgNumClass = 'hide';
                     }else if(newMsgCount < 10){
                         msgNumClass = 'single';
                         this.msgNumNode.text(newMsgCount);
                     }else if(newMsgCount < 100){
                         msgNumClass = 'ten';
                         this.msgNumNode.text(newMsgCount);
                     }else{
                         msgNumClass = 'hundred';
                         this.msgNumNode.text('99+');
                     }
                     break;
                   }else{
                     newMsgCount = uc.modules.notice.NoticeManager.getNoticeUnreadCount();
                     this.conversationInfo.newMsgCount = newMsgCount;
                     msgNumClass = this.showDot(newMsgCount);
                     break;
                   }
                case uc.constants.Conversation.ConversationType.MeetingMsg:
                    msgNumClass = this.showDot(newMsgCount);
                    break;
                default :
                    break;
            }
            this.msgNumNode.removeClass().addClass('msg-num').addClass(msgNumClass);
        },
        setAvatar: function(){
            var avatarNode = '<img src="../images/avatar_default.png" class="avatar round-image">';
            switch (this.conversationInfo.conversationType){
                case uc.constants.Conversation.ConversationType.SingleChat:
                    if(this.conversationInfo.avatarUrlArray[0]){
                        avatarNode = uc.util.AvatarUtil.composeContactAvatar(this.conversationInfo.avatarUrlArray[0], uc.constants.UI.DefaultLocalUrl.Conversation.Avatar);
                    }
                    break;
                case uc.constants.Conversation.ConversationType.GroupChat:
                    avatarNode = uc.util.AvatarUtil.composeGroupAvatar(this.conversationInfo.avatarUrlArray, uc.constants.UI.DefaultLocalUrl.Conversation.Avatar);
                    break;
                case uc.constants.Conversation.ConversationType.ProjectChat:
                    avatarNode = uc.util.AvatarUtil.composeProjectAvatar(this.conversationInfo.avatarUrlArray[0], uc.constants.UI.DefaultLocalUrl.Project);
                    break;
                case uc.constants.Conversation.ConversationType.CloudChat:
                    if (!this.conversationInfo.avatarUrlArray[0] || this.conversationInfo.avatarUrlArray[0] == '""') {
                        this.conversationInfo.avatarUrlArray[0] = uc.constants.UI.DefaultLocalUrl.Conversation.CloudFile;
                    }
                    avatarNode = uc.util.AvatarUtil.composeProjectAvatar(this.conversationInfo.avatarUrlArray[0], uc.constants.UI.DefaultLocalUrl.Conversation.CloudFile);
                    break;
                case uc.constants.Conversation.ConversationType.MeetingMsg:
                    avatarNode = uc.util.AvatarUtil.composeMeetingAvatar(this.conversationInfo.meetingStartTime, this.conversationInfo.isGNet, this.conversationInfo.isMeetingCancelled);
                    break;
                case uc.constants.Conversation.ConversationType.OpenApi:
                    avatarNode = uc.util.AvatarUtil.composeLocalImageAvatar(this.conversationInfo.avatarUrlArray[0], uc.constants.UI.DefaultLocalUrl.Conversation.OpenApi, this.conversationInfo.isCancelled);
                    break;
                case uc.constants.Conversation.ConversationType.WelcomeMsg:
                    break;
                case uc.constants.Conversation.ConversationType.OrgMsg:
                    break;
            }
            this.avatarNode.html(avatarNode);
        },
        setInCall:function(){
            if(this.conversationInfo.isInCall){
                this.msgInCall.show();
            }else{
                this.msgInCall.hide();
            }
        },
        showSuperGroupIcon: function () {
            var conversationType = this.conversationInfo;
            var isSuper = this.conversationInfo.memberType;
            if (isSuper) {
                this.superGroupIcon.removeClass('hide');
            } else {
                this.superGroupIcon.addClass('hide');
            }
        },
        setTopIndex: function () {
            var conversationType = this.conversationInfo;
            var isTop = this.conversationInfo.topIndex;
            if (isTop > 0) {
                this.node.addClass('item-top-index');
            } else {
                this.node.removeClass('item-top-index');
            }
        },
        showDisturbIcon:function(){
            switch (this.conversationInfo.conversationType){
                case uc.constants.Conversation.ConversationType.SingleChat:
                case uc.constants.Conversation.ConversationType.CloudChat:
                case uc.constants.Conversation.ConversationType.WelcomeMsg:
                case uc.constants.Conversation.ConversationType.OpenApi:
                case uc.constants.Conversation.ConversationType.MeetingMsg:
                    this.distrurbIcon.addClass('hide');
                    break;
                case uc.constants.Conversation.ConversationType.GroupChat:
                case uc.constants.Conversation.ConversationType.ProjectChat:
                    var isDisturb = this.conversationInfo.disturbStatus;
                    if (isDisturb) {
                        this.distrurbIcon.removeClass('hide');
                    } else {
                        this.distrurbIcon.addClass('hide');
                    }
                    break;
                default :
                    break;
            }
        },
        show: function(){
            this.node.show();
        },
        hide: function(){
            this.node.hide();
        },
        showDot:function(newMsgCount){
            var msgNumClass=null;
            if(!newMsgCount){
                msgNumClass = 'hide';
            }else{
                msgNumClass = 'meeting';
            }
            this.msgNumNode.text('');
            return msgNumClass;
        }
    })
})(uc);