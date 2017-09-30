;(function ($, undefined) {
    var noop = function () {};
    var slice = Array.prototype.slice;
    var template = uc.util.template;
    
    var defaults = {
        boxHtml: '<div class="search-input-container">' + 
        				'<span class="icon-search"></span>' + 
                        '<input type="text" class="search-input" placeholder="搜索" spellcheck="false">' +
        				'<span class="icon-search-del" style="display: none;"></span>' +
        			'</div>',
        panelHtml: '<div class="jquery-search-result panel-search-result node-shadow" style="display:none;">' + 
                        '<div class="panel-search-warp">' +
                            '<div class="search-result default-search-result"></div>' +
                            '<div class="search-result contacts-search-result"></div>' +
                            '<div class="search-result projects-search-result"></div>' +
                            '<div class="search-result groups-search-result"></div>' +
                            '<div class="search-result cloudGroups-search-result"></div>' +
                            '<div class="search-result meetings-search-result"></div>' +
                            '<div class="search-result messages-search-result"></div>' +
                        '</div>' +
		        	'</div>',
        detailHtml: '<div class="jquery-search-result detail-result node-shadow" style="display:none;">' +
                            '<div class="bread-crumb" data-step="1">' +
                                '<span class="bread-crumb-return"><img src="../images/contacts/return.png" alt="返回"></span><span class="contact-bread-crumb bread-crumb-left">'+ uc_resource.Search.Message +'</span>' +
                            '</div>' +
                            '<div class="detail-search-warp">' +
                                '<div class="search-result default-detail"></div>' +
                                '<div class="search-result contacts-detail"></div>' +
                                '<div class="search-result projects-detail"></div>' +
                                '<div class="search-result groups-detail"></div>' +
                                '<div class="search-result cloudGroups-detail"></div>' +
                                '<div class="search-result meetings-detail"></div>' +
                                '<div class="search-result messages-detail"></div>' +
                            '</div>' +
                    '</div>',
        emptyResultHtml: '<div class="empty-result"></div>',
        searchScope: {
            'default': [uc.constants.Search.SearchScope.Contact,
                        uc.constants.Search.SearchScope.Project,
                        uc.constants.Search.SearchScope.Group,
                        uc.constants.Search.SearchScope.CloudGroup,
                        uc.constants.Search.SearchScope.Meeting],
            contacts: [uc.constants.Search.SearchScope.Contact],
            projects: [uc.constants.Search.SearchScope.Project],
            groups: [uc.constants.Search.SearchScope.Group],
            cloudGroups: [uc.constants.Search.SearchScope.CloudGroup],
            meetings: [uc.constants.Search.SearchScope.Meeting],
            messages: [uc.constants.Search.SearchScope.Message]
        },
        onClick: function ($li, event) {},
        defaultDisplayCount: 3,
        listDisplayCount: 20,
        parent: 'body',
        getTwoMembers: function (group) {
            var members = group.members;
            var adminId = group.adminId;
            var res = [adminId, 0];
            for (var i = 0, len = members.length; i < len; i++) {
                var userId = members[i].userId;
                if (userId != adminId) {
                    res[1] = userId;
                    break;
                }
            }
            return res;
        },
        setMessages: function (messages) {
            if (messages && messages.length) {
                for (var i = 0; i < messages.length; i++) {
                    searchRequestMap.messages.push(messages[i]);
                }
            }
        },
        getMessages: function (conversationType, conversationId) {
            uc.util.LogUtil.info('jquery.searchInput', 'getMessages', 'go to get function message', {messages: searchRequestMap.messages, conversationType: conversationType, conversationId: conversationId});
            if (conversationId && conversationType && searchRequestMap.messages) {
                for (var i = 0; i < searchRequestMap.messages.length; i++){
                    var msg = searchRequestMap.messages[i];
                    if (msg.conversationType == conversationType && msg.relateId == conversationId) {
                        uc.util.LogUtil.info('jquery.searchInput', 'getMessages', 'get message list', {messages: msg});
                        return msg;
                    }
                }
            }

            return null;
        },
        clearMessage: function () {
            searchRequestMap.messages = [];
        },
        excludeHost: function (messages) {
            uc.util.LogUtil.info("jquery.searchInput", "excludeHost", "messages", {message: messages});
            var hostInfo = uc.IContactService.getCurrentUserInfo(), msgList = [];
            if (messages && messages.length) {
                for (var i = 0; i < messages.length; i++) {
                    var msg = messages[i];
                    if (hostInfo.userId != msg.relateId) {
                        msgList.push(msg);
                    }
                }
            }

            return msgList;
        },
        excludeCancelMeeting: function (meetings) {
            uc.util.LogUtil.info("jquery.searchInput", "excludeCancelMeeting", "meetings", {meetings: meetings});
            var meetingList = [];
            if (meetings && meetings.length) {
                for (var i = 0; i < meetings.length; i++) {
                    var meeting = meetings[i];
                    if (meeting.status != uc.constants.Meeting.Operations.Cancel) {
                        meetingList.push(meeting);
                    }
                }
            }

            return meetingList;
        },
        template: {
            'messageDetail': template('<@var slice = Array.prototype.slice;var messageList = messages.message;\
                         $.each(messageList, function (index) { var message = messageList[index];@>\
                                <@if (messages.conversationType == uc.constants.Conversation.ConversationType.ProjectChat) { @>\
                                    <@var self = uc.IGroupService.getGroupInfo(messages.relateId); if (!self) { \
                                     uc.util.LogUtil.info("jquery.searchInput", "messageDetail", "self null", {messageId: messages.relateId});\
                                     return;}\
                                     var userId = message.from.userId; userInfo = uc.modules.cache.BasicContactInfoCache.getBasicContactInfo(userId); \
                                     var msgTime = uc.util.TimeUtil.getMessageTimeString(message.msgTime, null, uc.settings.meeting.timeZone.offset);@>\
                                     <li class="group-item search-group-messages-item group-item-<@=self.id@>"  msgTime="<@=message.msgTime@>"   data-id="<@=self.id@>"  data-type="<@=uc.constants.Search.SearchResultType.Message@>" data-site-id="<@=self.siteId@>" data-count="<@=message.count@>" data-index="<@=message.conversationType@>"  data-step="1">\
                                        <div class="group-widget-container">\
                                            <div class="left">\
                                                <img class="avatar round-image avatar-<@=userInfo.userId@> <@=userInfo.localAvatar?"avatar-loaded":"avatar-unloaded"@>" userid="<@=userInfo.userId@>" src="<@=userInfo.localAvatar?userInfo.localAvatar:"../images/avatar_default.png"@>" onerror="uc.util.ImageUtil.OnImageLoadError(self, uc.constants.UI.DefaultLocalUrl.Avatar)" >\
                                            </div>\
                                            <div class="center"><div class="contact-display-name text-ellipsis" ><@=userInfo.displayName@></div><div class="msgTime"><@=msgTime@></div>\
                                                <div class="contact-description text-ellipsis">\
                                                    <@if (message.count > 1) {\
                                                        var msgText = uc.util.StringUtil.formatString(uc_resource.Search.MessageCount, [message.count]);@>\
                                                        <span class="search-message-text"><@=msgText@></span>\
                                                    <@} else {\
                                                        var textContent = message.textContent;\
                                                        textContent = $.MatchKeyWord(textContent, searchCondition.keyword);\
                                                        textContent = uc.util.EmotionUtil.codeToEmotion(textContent);\
                                                        textContent = uc.modules.emoji.EmojiConverter.unicodeToSmiley(textContent);\
                                                        textContent = uc.modules.emoji.EmojiConverter.androidCodeToSmiley(textContent);@>\
                                                        <span class="search-message-text"><@=textContent@></span>\
                                                    <@}@>\
                                                </div>\
                                            </div>\
                                            <@if (message.count > 1) {@>\
                                                <div class="search-message-right icon-call_video_ringht"></div>\
                                            <@}@>\
                                        </div>\
                                    </li>\
                                <@}@>\
                                <@if (messages.conversationType == uc.constants.Conversation.ConversationType.CloudChat) { @>\
                                    <@var self = uc.IGroupService.getGroupInfo(messages.relateId); if (!self) { \
                                     uc.util.LogUtil.info("jquery.searchInput", "messageDetail", "self null", {messageId: messages.relateId});\
                                     return;} \
                                     var userId = message.from.userId; userInfo = uc.modules.cache.BasicContactInfoCache.getBasicContactInfo(userId); \
                                     var msgTime = uc.util.TimeUtil.getMessageTimeString(message.msgTime, null, uc.settings.meeting.timeZone.offset);@>\
                                     <li class="group-item search-group-messages-item group-item-<@=self.id@>"  msgTime="<@=message.msgTime@>"  data-id="<@=self.id@>"  data-type="<@=uc.constants.Search.SearchResultType.Message@>" data-site-id="<@=self.siteId@>" data-count="<@=message.count@>" data-index="<@=message.conversationType@>"  data-step="1">\
                                        <div class="group-widget-container">\
                                            <div class="left">\
                                                <div class="project-group-avatar">\
                                                   <img class="avatar round-image avatar-<@=userInfo.userId@> <@=userInfo.localAvatar?"avatar-loaded":"avatar-unloaded"@>" userid="<@=userInfo.userId@>" src="<@=userInfo.localAvatar?userInfo.localAvatar:"../images/avatar_default.png"@>" onerror="uc.util.ImageUtil.OnImageLoadError(self, uc.constants.UI.DefaultLocalUrl.Avatar)" >\
                                                </div>\
                                            </div>\
                                            <div class="center"><div class="contact-display-name text-ellipsis" ><@=self.name@></div>\
                                                <div class="contact-description text-ellipsis">\
                                                    <@if (message.count > 1) {\
                                                        var msgText = uc.util.StringUtil.formatString(uc_resource.Search.MessageCount, [message.count]);@>\
                                                        <span class="search-message-text"><@=msgText@></span>\
                                                    <@} else {\
                                                        var textContent = message.textContent;\
                                                        textContent = $.MatchKeyWord(textContent, searchCondition.keyword);\
                                                        textContent = uc.util.EmotionUtil.codeToEmotion(textContent);\
                                                        textContent = uc.modules.emoji.EmojiConverter.unicodeToSmiley(textContent);\
                                                        textContent = uc.modules.emoji.EmojiConverter.androidCodeToSmiley(textContent);@>\
                                                        <span class="search-message-text"><@=textContent@></span>\
                                                    <@}@>\
                                                </div>\
                                            </div>\
                                            <@if (message.count > 1) {@>\
                                                <div class="search-message-right icon-call_video_ringht"></div>\
                                            <@}@>\
                                        </div>\
                                    </li>\
                                <@}@>\
                                <@if (messages.conversationType == uc.constants.Conversation.ConversationType.GroupChat) {@>\
                                    <@var self = uc.IGroupService.getGroupInfo(messages.relateId); if (!self) { \
                                     uc.util.LogUtil.info("jquery.searchInput", "messageDetail", "self null", {messageId: messages.relateId});\
                                     return;} \
                                     var userId = message.from.userId; userInfo = uc.modules.cache.BasicContactInfoCache.getBasicContactInfo(userId); \
                                     var msgTime = uc.util.TimeUtil.getMessageTimeString(message.msgTime, null, uc.settings.meeting.timeZone.offset);@>\
                                     <li class="group-item search-group-messages-item group-item-<@=self.id@>"  msgTime="<@=message.msgTime@>"  data-id="<@=self.id@>"  data-type="<@=uc.constants.Search.SearchResultType.Message@>" data-site-id="<@=self.siteId@>" data-count="<@=message.count@>" data-index="<@=message.conversationType@>"  data-step="1">\
                                        <div class="group-widget-container">\
                                            <div class="left">\
                                               <img class="avatar round-image avatar-<@=userInfo.userId@> <@=userInfo.localAvatar?"avatar-loaded":"avatar-unloaded"@>" userid="<@=userInfo.userId@>" src="<@=userInfo.localAvatar?userInfo.localAvatar:"../images/avatar_default.png"@>" onerror="uc.util.ImageUtil.OnImageLoadError(self, uc.constants.UI.DefaultLocalUrl.Avatar)" >\
                                            </div>\
                                            <div class="center"><div class="contact-display-name text-ellipsis" ><@=userInfo.displayName@></div><div class="msgTime"><@=msgTime@></div>\
                                                <div class="contact-description text-ellipsis">\
                                                    <@if (message.count > 1) {\
                                                        var msgText = uc.util.StringUtil.formatString(uc_resource.Search.MessageCount, [message.count]);@>\
                                                        <span class="search-message-text"><@=msgText@></span>\
                                                    <@} else {\
                                                        var textContent = message.textContent;\
                                                        textContent = $.MatchKeyWord(textContent, searchCondition.keyword);\
                                                        textContent = uc.util.EmotionUtil.codeToEmotion(textContent);\
                                                        textContent = uc.modules.emoji.EmojiConverter.unicodeToSmiley(textContent);\
                                                        textContent = uc.modules.emoji.EmojiConverter.androidCodeToSmiley(textContent);@>\
                                                        <span class="search-message-text"><@=textContent@></span>\
                                                    <@}@>\
                                                </div>\
                                            </div>\
                                            <@if (message.count > 1) {@>\
                                                <div class="search-message-right icon-call_video_ringht"></div>\
                                            <@}@>\
                                        </div>\
                                    </li>\
                                <@}@>\
                                <@if (messages.conversationType == uc.constants.Conversation.ConversationType.SingleChat) {@>\
                                    <@ var self = uc.modules.cache.BasicContactInfoCache.getBasicContactInfo(messages.relateId);@>\
                                     <li class="contact-item contact-item-<@=self.userId@>"  data-id="<@=self.userId@>"  msgTime="<@=message.msgTime@>"  data-type="<@=uc.constants.Search.SearchResultType.Message@>" data-site-id="<@=self.siteId@>" data-count="<@=message.count@>" data-index="<@=message.conversationType@>"  data-step="2">\
                                        <div class="contact-widget-container">\
                                            <div class="left">\
                                                <img class="avatar round-image avatar-<@=self.userId@> <@=self.localAvatar?"avatar-loaded":"avatar-unloaded"@>" userid="<@=self.userId@>" src="<@=self.localAvatar?self.localAvatar:"../images/avatar_default.png"@>" onerror="uc.util.ImageUtil.OnImageLoadError(self, uc.constants.UI.DefaultLocalUrl.Avatar)" >\
                                            </div>\
                                            <div class="center"><div class="contact-display-name text-ellipsis" ><@=self.displayName@></div>\
                                                <div class="contact-description text-ellipsis">\
                                                    <@if (message.count > 1) {\
                                                        var msgText = uc.util.StringUtil.formatString(uc_resource.Search.MessageCount, [message.count]);@>\
                                                        <span class="search-message-text"><@=msgText@></span>\
                                                    <@} else { \
                                                        var textContent = message.textContent;\
                                                        textContent = $.MatchKeyWord(textContent, searchCondition.keyword);\
                                                        textContent = uc.util.EmotionUtil.codeToEmotion(textContent);\
                                                        textContent = uc.modules.emoji.EmojiConverter.unicodeToSmiley(textContent);\
                                                        textContent = uc.modules.emoji.EmojiConverter.androidCodeToSmiley(textContent);@>\
                                                        <span class="search-message-text"><@=textContent@></span>\
                                                    <@}@>\
                                                </div>\
                                            </div>\
                                            <@if (message.count > 1) {@>\
                                                <div class="search-message-right icon-call_video_ringht"></div>\
                                            <@}@>\
                                        </div>\
                                    </li>\
                                <@}@>\
                            <@})@>'),
            'messages': template('<@var slice = Array.prototype.slice;\
                         $.each(messages, function (index) { var message = messages[index];\
                            if (index < searchCondition.displayCount) { @>\
                                <@if (message.conversationType == uc.constants.Conversation.ConversationType.MeetingMsg){@>\
                                  <@var self = uc.IGroupService.getGroupInfo(message.relateId); if (!self) { \
                                     uc.util.LogUtil.info("jquery.searchInput", "messageDetail", "self null", {messageId: message.relateId});\
                                     return;} @>\
                                     <li class="group-item search-group-messages-item group-item-<@=self.id@>"  data-id="<@=self.id@>"  data-type="<@=uc.constants.Search.SearchResultType.Message@>" data-site-id="<@=self.siteId@>" data-count="<@=message.count@>" data-index="<@=message.conversationType@>"  data-step="1">\
                                        <div class="group-widget-container">\
                                            <div class="left">\
                                                <div class="project-group-avatar">\
                                                    <img src="<@=self.localAvatar?self.localAvatar:uc.constants.UI.DefaultLocalUrl.Project@>" onerror="uc.util.ImageUtil.OnImageLoadError(self,uc.constants.UI.DefaultLocalUrl.Project)" class="project-avatar round-image project-avatar-<@=self.id@>">\
                                                </div>\
                                            </div>\
                                            <div class="center"><div class="contact-display-name text-ellipsis" ><@=self.name@></div>\
                                                <div class="contact-description text-ellipsis">\
                                                    <@if (message.count > 1) {\
                                                        var msgText = uc.util.StringUtil.formatString(uc_resource.Search.MessageCount, [message.count]);@>\
                                                        <span class="search-message-text"><@=msgText@></span>\
                                                    <@} else { \
                                                        var textContent = message.message[0].textContent;\
                                                        textContent = $.MatchKeyWord(textContent, searchCondition.keyword);\
                                                        textContent = uc.util.EmotionUtil.codeToEmotion(textContent);\
                                                        textContent = uc.modules.emoji.EmojiConverter.unicodeToSmiley(textContent);\
                                                        textContent = uc.modules.emoji.EmojiConverter.androidCodeToSmiley(textContent);@>\
                                                        <span class="search-message-text"><@=textContent@></span>\
                                                    <@}@>\
                                                </div>\
                                            </div>\
                                            <@if (message.count > 1) {@>\
                                                <div class="search-message-right icon-call_video_ringht"></div>\
                                            <@}@>\
                                        </div>\
                                    </li>\
                                <@}@>\
                                <@if (message.conversationType == uc.constants.Conversation.ConversationType.ProjectChat) { @>\
                                   <@var self = uc.IGroupService.getGroupInfo(message.relateId); if (!self) { \
                                     uc.util.LogUtil.info("jquery.searchInput", "messageDetail", "self null", {messageId: message.relateId});\
                                     return;} @>\
                                     <li class="group-item search-group-messages-item group-item-<@=self.id@>"  data-id="<@=self.id@>"  data-type="<@=uc.constants.Search.SearchResultType.Message@>" data-site-id="<@=self.siteId@>" data-count="<@=message.count@>" data-index="<@=message.conversationType@>"  data-step="1">\
                                        <div class="group-widget-container">\
                                            <div class="left">\
                                                <div class="project-group-avatar">\
                                                    <img src="<@=self.localAvatar?self.localAvatar:uc.constants.UI.DefaultLocalUrl.Project@>" onerror="uc.util.ImageUtil.OnImageLoadError(self,uc.constants.UI.DefaultLocalUrl.Project)" class="project-avatar round-image project-avatar-<@=self.id@>">\
                                                </div>\
                                            </div>\
                                            <div class="center"><div class="contact-display-name text-ellipsis" ><@=self.name@></div>\
                                                <div class="contact-description text-ellipsis">\
                                                    <@if (message.count > 1) {\
                                                        var msgText = uc.util.StringUtil.formatString(uc_resource.Search.MessageCount, [message.count]);@>\
                                                        <span class="search-message-text"><@=msgText@></span>\
                                                    <@} else { \
                                                        var textContent = message.message[0].textContent;\
                                                        textContent = $.MatchKeyWord(textContent, searchCondition.keyword);\
                                                        textContent = uc.util.EmotionUtil.codeToEmotion(textContent);\
                                                        textContent = uc.modules.emoji.EmojiConverter.unicodeToSmiley(textContent);\
                                                        textContent = uc.modules.emoji.EmojiConverter.androidCodeToSmiley(textContent);@>\
                                                        <span class="search-message-text"><@=textContent@></span>\
                                                    <@}@>\
                                                </div>\
                                            </div>\
                                            <@if (message.count > 1) {@>\
                                                <div class="search-message-right icon-call_video_ringht"></div>\
                                            <@}@>\
                                        </div>\
                                    </li>\
                                <@}@>\
                                <@if (message.conversationType == uc.constants.Conversation.ConversationType.CloudChat) { @>\
                                   <@var self = uc.IGroupService.getGroupInfo(message.relateId); if (!self) { \
                                     uc.util.LogUtil.info("jquery.searchInput", "messageDetail", "self null", {messageId: message.relateId});\
                                     return;} @>\
                                     <li class="group-item search-group-messages-item group-item-<@=self.id@>"  data-id="<@=self.id@>"  data-type="<@=uc.constants.Search.SearchResultType.Message@>" data-site-id="<@=self.siteId@>" data-count="<@=message.count@>" data-index="<@=message.conversationType@>"  data-step="1">\
                                        <div class="group-widget-container">\
                                            <div class="left">\
                                                <div class="project-group-avatar">\
                                                    <img src="<@=self.localAvatar?self.localAvatar:uc.constants.UI.DefaultLocalUrl.CloudGroup@>" onerror="uc.util.ImageUtil.OnImageLoadError(self,uc.constants.UI.DefaultLocalUrl.CloudGroup)" class="project-avatar round-image project-avatar-<@=self.id@>">\
                                                </div>\
                                            </div>\
                                            <div class="center"><div class="contact-display-name text-ellipsis" ><@=self.name@></div>\
                                                <div class="contact-description text-ellipsis">\
                                                    <@if (message.count > 1) {\
                                                        var msgText = uc.util.StringUtil.formatString(uc_resource.Search.MessageCount, [message.count]);@>\
                                                        <span class="search-message-text"><@=msgText@></span>\
                                                    <@} else { \
                                                        var textContent = message.message[0].textContent;\
                                                        textContent = $.MatchKeyWord(textContent, searchCondition.keyword);\
                                                        textContent = uc.util.EmotionUtil.codeToEmotion(textContent);\
                                                        textContent = uc.modules.emoji.EmojiConverter.unicodeToSmiley(textContent);\
                                                        textContent = uc.modules.emoji.EmojiConverter.androidCodeToSmiley(textContent);@>\
                                                        <span class="search-message-text"><@=textContent@></span>\
                                                    <@}@>\
                                                </div>\
                                            </div>\
                                            <@if (message.count > 1) {@>\
                                                <div class="search-message-right icon-call_video_ringht"></div>\
                                            <@}@>\
                                        </div>\
                                    </li>\
                                <@}@>\
                                <@if (message.conversationType == uc.constants.Conversation.ConversationType.GroupChat) {@>\
                                     <@var self = uc.IGroupService.getGroupInfo(message.relateId); if (!self) { \
                                     uc.util.LogUtil.info("jquery.searchInput", "messageDetail", "self null", {messageId: message.relateId});\
                                     return;} @>\
                                     <li class="group-item search-group-messages-item group-item-<@=self.id@>"  data-id="<@=self.id@>"  data-type="<@=uc.constants.Search.SearchResultType.Message@>" data-site-id="<@=self.siteId@>" data-count="<@=message.count@>" data-index="<@=message.conversationType@>"  data-step="1">\
                                        <div class="group-widget-container">\
                                            <div class="left">\
                                                <div class="group-avatar avatar-loaded">\
                                                    <@ var tMembers = options.getTwoMembers(self); @>\
                                                    <img class="avatar-admin round-image avatar avatar-<@=tMembers[0]@> avatar-unloaded" userid="<@=tMembers[0]@>" src="<@=uc.constants.UI.DefaultLocalUrl.Avatar_32@>" onerror="uc.util.ImageUtil.OnImageLoadError(this,uc.constants.UI.DefaultLocalUrl.Avatar_32)" />\
                                                    <img class="avatar-attendee round-image avatar avatar-<@=tMembers[1]@> avatar-unloaded" userid="<@=tMembers[1]@>" src="<@=uc.constants.UI.DefaultLocalUrl.Avatar_32@>" onerror="uc.util.ImageUtil.OnImageLoadError(this,uc.constants.UI.DefaultLocalUrl.Avatar_32)" />\
                                                </div>\
                                            </div>\
                                            <div class="center"><div class="contact-display-name text-ellipsis" ><@=self.name@></div>\
                                                <div class="contact-description text-ellipsis">\
                                                    <@if (message.count > 1) {\
                                                        var msgText = uc.util.StringUtil.formatString(uc_resource.Search.MessageCount, [message.count]);@>\
                                                        <span class="search-message-text"><@=msgText@></span>\
                                                    <@} else {  \
                                                        var textContent = message.message[0].textContent;\
                                                        textContent = $.MatchKeyWord(textContent, searchCondition.keyword);\
                                                        textContent = uc.util.EmotionUtil.codeToEmotion(textContent);\
                                                        textContent = uc.modules.emoji.EmojiConverter.unicodeToSmiley(textContent);\
                                                        textContent = uc.modules.emoji.EmojiConverter.androidCodeToSmiley(textContent);@>\
                                                        <span class="search-message-text"><@=textContent@></span>\
                                                    <@}@>\
                                                </div>\
                                            </div>\
                                            <@if (message.count > 1) {@>\
                                                <div class="search-message-right icon-call_video_ringht"></div>\
                                            <@}@>\
                                        </div>\
                                    </li>\
                                <@}@>\
                                <@if (message.conversationType == uc.constants.Conversation.ConversationType.SingleChat) {@>\
                                    <@ var hostInfo = uc.IContactService.getCurrentUserInfo(); \
                                    var self = uc.modules.cache.BasicContactInfoCache.getBasicContactInfo(message.relateId);\
                                    if (self.userId == hostInfo.userId) { searchCondition.displayCount++; return true;}@>\
                                     <li class="contact-item contact-item-<@=self.userId@>"  data-id="<@=self.userId@>"  data-type="<@=uc.constants.Search.SearchResultType.Message@>" data-site-id="<@=self.siteId@>" data-count="<@=message.count@>" data-index="<@=message.conversationType@>"  data-step="1">\
                                        <div class="contact-widget-container">\
                                            <div class="left">\
                                                <img class="avatar round-image avatar-<@=self.userId@> <@=self.localAvatar?"avatar-loaded":"avatar-unloaded"@>" userid="<@=self.userId@>" src="<@=self.localAvatar?self.localAvatar:"../images/avatar_default.png"@>" onerror="uc.util.ImageUtil.OnImageLoadError(self, uc.constants.UI.DefaultLocalUrl.Avatar)" >\
                                            </div>\
                                            <div class="center"><div class="contact-display-name text-ellipsis" ><@=self.displayName@></div>\
                                                <div class="contact-description text-ellipsis">\
                                                    <@if (message.count > 1) {\
                                                        var msgText = uc.util.StringUtil.formatString(uc_resource.Search.MessageCount, [message.count]);@>\
                                                        <span class="search-message-text"><@=msgText@></span>\
                                                    <@} else { \
                                                        var textContent = message.message[0].textContent;\
                                                        textContent = $.MatchKeyWord(textContent, searchCondition.keyword);\
                                                        textContent = uc.util.EmotionUtil.codeToEmotion(textContent);\
                                                        textContent = uc.modules.emoji.EmojiConverter.unicodeToSmiley(textContent);\
                                                        textContent = uc.modules.emoji.EmojiConverter.androidCodeToSmiley(textContent);@>\
                                                        <span class="search-message-text"><@=textContent@></span>\
                                                    <@}@>\
                                                </div>\
                                            </div>\
                                            <@if (message.count > 1) {@>\
                                                <div class="search-message-right icon-call_video_ringht"></div>\
                                            <@}@>\
                                        </div>\
                                    </li>\
                                <@}@>\
                            <@}@>\
                        <@})@>\
                        <@ if (messages.length > searchCondition.displayCount && searchCondition.searchType != "default") {@>\
                            <div class="down-scroll-way-point jquery-search-loading" currentsearchpage="<@=searchCondition.searchPage@>">	<img src="../images/contacts/loading.gif"></div>\
                        <@}@>'),
            'meetings': template('<@var slice = Array.prototype.slice;\
                         $.each(meetings, function (index) {\
                            if (index < searchCondition.displayCount) { @>\
                            <li class="meeting-item meeting-item-<@=this.eventId@>" data-id="<@=this.eventId@>" data-type="<@=uc.constants.Search.SearchResultType.Meeting@>" data-site-id="<@=this.eventId@>" >\
                                <div class="meeting-widget-container">\
                                    <div class="left">\
                                        <@=uc.util.AvatarUtil.composeMeetingAvatar(this.startTime, this.isGNet, this.isOld)@>\
                                    </div>\
                                    <div class="center">\
                                        <div class="meeting-name-container">\
                                            <div class="meeting-name"><@=$.MatchKeyWord(uc.util.StringUtil.escapeHtml(this.title), searchCondition.keyword)@></div>\
                                        </div>\
                                        <div class="meeting-description text-ellipsis">主持人: <@=this.hostName@></div>\
                                    </div>\
                                </div>\
                            </li>\
                        <@}})@>\
                        <@if (meetings.length > searchCondition.displayCount && searchCondition.searchType != "default") {@>\
                            <div class="down-scroll-way-point jquery-search-loading" currentsearchpage="<@=searchCondition.searchPage@>">	<img src="../images/contacts/loading.gif"></div>\
                        <@}@>'),
            'groups': template('<@var slice = Array.prototype.slice;\
                        $.each(groups, function (index) {\
                            if (index < searchCondition.displayCount) {\
                            @>\
                            <li class="group-item group-item-<@=this.id@>" data-id="<@=this.id@>" data-type="<@=uc.constants.Search.SearchResultType.Group@>" data-site-id="<@=this.siteId@>" >\
                                <div class="group-widget-container">\
                                    <div class="left">\
                                        <div class="group-avatar avatar-loaded">\
                                           <@ var tMembers = options.getTwoMembers(this); @>\
                                            <img class="avatar-admin round-image avatar avatar-<@=tMembers[0]@> avatar-unloaded" userid="<@=tMembers[0]@>" src="<@=uc.constants.UI.DefaultLocalUrl.Avatar_32@>" onerror="uc.util.ImageUtil.OnImageLoadError(this,uc.constants.UI.DefaultLocalUrl.Avatar_32)" >\
                                            <img class="avatar-attendee round-image avatar avatar-<@=tMembers[1]@> avatar-unloaded" userid="<@=tMembers[1]@>" src="<@=uc.constants.UI.DefaultLocalUrl.Avatar_32@>" onerror="uc.util.ImageUtil.OnImageLoadError(this,uc.constants.UI.DefaultLocalUrl.Avatar_32)" >\
                                        </div>\
                                    </div>\
                                    <div class="center">\
                                        <div class="group-name-container"><span class="group-name" qtips="<@-uc.util.StringUtil.escapeHtml(this.name)@>" ><@=$.MatchKeyWord(uc.util.StringUtil.escapeHtml(this.name), searchCondition.keyword)@></span><span class="group-members-count-container">(<span class="group-members-count"><@=this.members.length@></span>)</span></div>\
                                        <div class="group-description text-ellipsis">\
                                            <div class="group-members" style="display:none">\
                                                <@$.each(slice.call(this.members, 0, 6), function () {@>\
                                                    <span class="group-member group-member-<@=this.userId@>"></span>\
                                                <@})@>\
                                            </div>\
                                        </div>\
                                    </div>\
                                </div>\
                            </li>\
                        <@}})@>\
                        <@if (groups.length > searchCondition.displayCount && searchCondition.searchType != "default") {@>\
                            <div class="down-scroll-way-point jquery-search-loading" currentsearchpage="<@=searchCondition.searchPage@>">	<img src="../images/contacts/loading.gif"></div>\
                        <@}@>'),
            //<div class="right"><div class="checkbox-parent check-box-group check-box-group-<@=this.id@>"><input type="checkbox"><label><span><img class="remove-group" src="../images/contacts/remove.png"></span></label></div></div>
            'projects': template('<@var slice = Array.prototype.slice;\
                        $.each(projects, function (index) {\
                            if (index < searchCondition.displayCount) { @>\
                            <li class="group-item group-item-<@=this.id@>" data-id="<@=this.id@>" data-type="<@=uc.constants.Search.SearchResultType.Project@>" data-site-id="<@=this.siteId@>" >\
                                <div class="group-widget-container">\
                                    <div class="left">\
                                        <div class="project-group-avatar">\
                                            <img src="<@=this.localAvatar?this.localAvatar:uc.constants.UI.DefaultLocalUrl.Project@>" onerror="uc.util.ImageUtil.OnImageLoadError(this,uc.constants.UI.DefaultLocalUrl.Project)" class="project-avatar round-image project-avatar-<@=this.id@>">\
                                        </div>\
                                    </div>\
                                    <div class="center">\
                                        <div class="group-name-container"><span class="group-name" qtips="<@-uc.util.StringUtil.escapeHtml(this.name)@>" ><@=$.MatchKeyWord(uc.util.StringUtil.escapeHtml(this.name), searchCondition.keyword)@></span><span class="group-members-count-container">(<span class="group-members-count"><@=this.members.length@></span>)</span></div>\
                                        <div class="group-description text-ellipsis">\
                                            <div class="group-members" style="display:none">\
                                                <@$.each(slice.call(this.members, 0, 6), function () {@>\
                                                    <span class="group-member group-member-<@=this.userId@>"></span>\
                                                <@})@>\
                                            </div>\
                                        </div>\
                                    </div>\
                                </div>\
                            </li>\
                        <@}})@>\
                        <@if (projects.length > searchCondition.displayCount && searchCondition.searchType != "default") {@>\
                            <div class="down-scroll-way-point jquery-search-loading" currentsearchpage="<@=searchCondition.searchPage@>">	<img src="../images/contacts/loading.gif"></div>\
                        <@}@>'),
            'cloudGroups': template('<@var slice = Array.prototype.slice;\
                        $.each(cloudGroups, function (index) {\
                            if (index < searchCondition.displayCount) { @>\
                            <li class="group-item group-item-<@=this.id@>" data-id="<@=this.id@>" data-type="<@=uc.constants.Search.SearchResultType.CloudGroup@>" data-site-id="<@=this.siteId@>" >\
                                <div class="group-widget-container">\
                                    <div class="left">\
                                        <div class="project-group-avatar">\
                                            <img src="<@=this.avatarIndex?this.avatarIndex:uc.constants.UI.DefaultLocalUrl.CloudGroup@>" onerror="uc.util.ImageUtil.OnImageLoadError(this,uc.constants.UI.DefaultLocalUrl.CloudGroup)" class="project-avatar round-image project-avatar-<@=this.id@>">\
                                        </div>\
                                    </div>\
                                    <div class="center">\
                                        <div class="group-name-container"><span class="group-name" qtips="<@-uc.util.StringUtil.escapeHtml(this.name)@>" ><@=$.MatchKeyWord(this.name, searchCondition.keyword)@></span><span class="group-members-count-container">(<span class="group-members-count"><@=this.memberCount@></span>)</span></div>\
                                        <div class="group-description text-ellipsis">\
                                            <div class="group-members" style="display:none">\
                                                <@$.each(slice.call(this.members, 0, 6), function () {@>\
                                                    <span class="group-member group-member-<@=this.userId@>"></span>\
                                                <@})@>\
                                            </div>\
                                        </div>\
                                    </div>\
                                </div>\
                            </li>\
                        <@}})@>\
                        <@if (cloudGroups.length > searchCondition.displayCount && searchCondition.searchType != "default") {@>\
                            <div class="down-scroll-way-point jquery-search-loading" currentsearchpage="<@=searchCondition.searchPage@>">	<img src="../images/contacts/loading.gif"></div>\
                        <@}@>'),
            //<div class="right"><div class="checkbox-parent check-box-group check-box-group-<@=this.id@>"><input type="checkbox"><label><span><img class="remove-group" src="../images/contacts/remove.png"></span></label></div></div>
            'contacts': template('<@$.each(contacts, function (index) {\
                                          if (index < searchCondition.displayCount) { @>\
                                        <li class="contact-item contact-item-<@=this.userId@>" data-id="<@=this.userId@>" data-type="<@=uc.constants.Search.SearchResultType.Contact@>" data-site-id="<@=this.siteId@>" >\
                                            <div class="contact-widget-container">\
                                                <div class="left">\
                                                    <img class="avatar round-image avatar-<@=this.userId@> <@=this.localAvatar?"avatar-loaded":"avatar-unloaded"@>" userid="<@=this.userId@>" src="<@=this.localAvatar?this.localAvatar:"../images/avatar_default.png"@>" onerror="uc.util.ImageUtil.OnImageLoadError(this, uc.constants.UI.DefaultLocalUrl.Avatar)" >\
                                                </div>\
                                                <div class="center"><div class="contact-display-name text-ellipsis" ><@=$.MatchKeyWord(this.displayName, searchCondition.keyword)@></div><div class="contact-description text-ellipsis"><span class="contact-department"><@=this.departmentName@></span><span class="contact-position"><@=this.position@></span></div></div>\
                                            </div>\
                                        </li>\
                                    <@}})@>\
                                    <@if (contacts.length > searchCondition.displayCount && searchCondition.searchType != "default") {@>\
                                        <div class="down-scroll-way-point jquery-search-loading" currentsearchpage="<@=searchCondition.searchPage@>">	<img src="../images/contacts/loading.gif"></div>\
                                    <@}@>'),
            //<div class="right"><div class="checkbox-parent check-box-contact check-box-contact-<@=this.userId@>"><input type="checkbox"><label><span><img class="remove-contact" src="../images/contacts/remove.png"></span></label></div></div>
            'defaultSearch': template('<@var slice = Array.prototype.slice;@>\
                            <@if (contacts && contacts.length > 0) {@>\
                                <div class="bread-crumb">\
                                    <span class="contact-bread-crumb bread-crumb-left">联系人</span>\
                                    <@if (contacts.length > searchCondition.displayCount ) {@><span class="bread-crumb-right view-more-search-result" list="contacts">更多联系人</span><@}@>\
                                </div>\
                                <ul class="contact-list">\
                                     <@=options.template.contacts({contacts: contacts, options: options, searchCondition: searchCondition})@>\
                                </ul>\
                            <@}@>\
                            <@if (projects && projects.length > 0) {@>\
                                <div class="bread-crumb">\
                                    <span class="project-bread-crumb bread-crumb-left">项目</span>\
                                    <@if (projects.length > searchCondition.displayCount ) {@><span class="bread-crumb-right view-more-search-result" list="projects" >更多项目</span><@}@>\
                                </div>\
                                <ul class="project-list group-list">\
                                    <@=options.template.projects({projects: projects, options: options, searchCondition: searchCondition})@>\
                                </ul>\
                            <@}@>\
                            <@if (groups && groups.length > 0) {@>\
		        				<div class="bread-crumb">\
                                    <span class="chat-bread-crumb bread-crumb-left">群聊</span>\
                                    <@if (groups.length > searchCondition.displayCount ) {@><span class="bread-crumb-right view-more-search-result" list="groups" >更多群聊</span><@}@>\
                                </div>\
		        				<ul class="group-chat-list group-list">\
                                    <@=options.template.groups({groups: groups, options: options, searchCondition: searchCondition})@>\
                                </ul>\
                            <@}@>\
                            \<@if (cloudGroups && cloudGroups.length > 0) {@>\
		        				<div class="bread-crumb">\
                                    <span class="chat-bread-crumb bread-crumb-left">文件库讨论组</span>\
                                    <@if (cloudGroups.length > searchCondition.displayCount ) {@><span class="bread-crumb-right view-more-search-result" list="cloudGroups" >更多文件库讨论组</span><@}@>\
                                </div>\
		        				<ul class="project-list group-list">\
                                    <@=options.template.cloudGroups({cloudGroups: cloudGroups, options: options, searchCondition: searchCondition})@>\
                                </ul>\
                            <@}@>\
                            <@if (meetings && meetings.length > 0) {@>\
		        				<div class="bread-crumb">\
                                    <span class="chat-bread-crumb bread-crumb-left">日程</span>\
                                    <@if (meetings.length > searchCondition.displayCount ) {@><span class="bread-crumb-right view-more-search-result" list="meetings" >更多日程</span><@}@>\
                                </div>\
		        				<ul class="group-chat-list group-list">\
                                    <@=options.template.meetings({meetings: meetings, options: options, searchCondition: searchCondition})@>\
                                </ul>\
                            <@}@>\
                            <@if (messages && messages.length > 0) {@>\
                                <div class="bread-crumb">\
                                    <span class="project-bread-crumb bread-crumb-left">'+uc_resource.Search.Message+'</span>\
                                    <@if (messages.length > searchCondition.displayCount ) {@><span class="bread-crumb-right view-more-search-result" list="messages" >'+uc_resource.Search.MoreMessage+'</span><@}@>\
                                </div>\
                                <ul class="project-list group-list">\
                                    <@=options.template.messages({messages: messages, options: options,\
                                     searchCondition: searchCondition})@>\
                                </ul>\
                            <@}@>'),
            'contactsSearch': template('<div class="bread-crumb">\
                                        <span class="bread-crumb-return"><img src="../images/contacts/return.png" alt="返回"></span><span class="contact-bread-crumb bread-crumb-left">联系人</span></div>\
                                    <ul class="contact-list jquery-search-list" >\
                                        <@=options.template.contacts({contacts: contacts, options: options, searchCondition: searchCondition})@>\
                                    </ul>'),
            'projectsSearch': template('<div class="bread-crumb">\
                                        <span class="bread-crumb-return"><img src="../images/contacts/return.png" alt="返回"></span><span class="contact-bread-crumb bread-crumb-left">项目</span></div>\
                                    <ul class="project-list group-list jquery-search-list" >\
                                        <@=options.template.projects({projects: projects, options: options, searchCondition: searchCondition})@>\
                                    </ul>'),
            'groupsSearch': template('<div class="bread-crumb">\
                                        <span class="bread-crumb-return"><img src="../images/contacts/return.png" alt="返回"></span><span class="contact-bread-crumb bread-crumb-left">群聊</span></div>\
                                    <ul class="group-list jquery-search-list" >\
                                        <@=options.template.groups({groups: groups, options: options, searchCondition: searchCondition})@>\
                                    </ul>'),
            'cloudGroupsSearch': template('<div class="bread-crumb">\
                                        <span class="bread-crumb-return"><img src="../images/contacts/return.png" alt="返回"></span><span class="contact-bread-crumb bread-crumb-left">文件库讨论组</span></div>\
                                    <ul class="group-list jquery-search-list" >\
                                        <@=options.template.cloudGroups({cloudGroups: cloudGroups, options: options, searchCondition: searchCondition})@>\
                                    </ul>'),
            'meetingsSearch': template('<div class="bread-crumb">\
                                        <span class="bread-crumb-return"><img src="../images/contacts/return.png" alt="返回"></span><span class="contact-bread-crumb bread-crumb-left">日程</span></div>\
                                    <ul class="meeting-list jquery-search-list" >\
                                        <@=options.template.meetings({meetings: meetings, options: options, searchCondition: searchCondition})@>\
                                    </ul>'),
            'messagesSearch': template('<div class="bread-crumb" data-step="2">\
                                        <span class="bread-crumb-return"><img src="../images/contacts/return.png" alt="返回"></span><span class="contact-bread-crumb bread-crumb-left">'+ uc_resource.Search.Message +'</span></div>\
                                    <ul class="meeting-list jquery-search-list" >\
                                        <@=options.template.messages({messages: messages, options: options,searchCondition: searchCondition})@>\
                                    </ul>')
        }
    };
    //<div class="down-scroll-way-point" currentsearchpage="`"><img src="../images/contacts/loading.gif"></div>
    var searchRequestMap = {messages: []};
    var getSearchRequestKey = function (id) {
        return "ContactsSearch_" + id;
    };
    var uuid = 1;
    var searchInputMap = {};
    function SearchInput($elm, options) {
        this.$box = $elm.addClass('jquery-search-box');
        this.requestMap = {};
        this.options = options;
        this.uuid = 'SearchInput_' + uuid++;
        this.init();
    }
    SearchInput.prototype = {
        init: function () {
            var self = this;
            var options = this.options;
            var $box = this.$box.html(options.boxHtml);
            $(options.panelHtml).appendTo(options.parent);
            $(options.detailHtml).appendTo(options.parent);
            this.$panel = $(options.parent).find('.panel-search-result');
            this.$DetailPanel = $(options.parent).find('.detail-result');
            this.$warp = this.$panel.find('.panel-search-warp');
            this.$MessagesWarp = this.$DetailPanel.find('.detail-search-warp');
            this.$ipt = $box.find(".search-input");
            this.$del = $box.find(".icon-search-del");
            this.searchKey = '';
            this.bindEvent();
            this.meetingDataMap = {};
            this.inSearchingTimer = -1;
            uc.util.CaretUtil.setInputEditorCaretPosition(this.$ipt);
        },
        bindEvent: function () {
            var $ipt = this.$ipt;
            var $del = this.$del;
            var self = this;
            var options = this.options;
            var timer;
            $ipt.on("input", function() {
                timer && clearTimeout(timer);
                timer = setTimeout(function(){
                    self.search($.trim($ipt.val()));
                }, 300);
                if (!$.trim($ipt.val())) {
                    $del.hide();
                }
            }).on("focus", function () {
                var isDetailShow = self.$DetailPanel.attr('data-show') * 1;
                if (isDetailShow && self.$ipt.val()) {
                    self.$DetailPanel.show();
                } else {
                    self.$panel.show();
                }
            }).on('keydown', function (e) {
                var isMeetingPanel = $('.jquery-search-result.detail-result:visible').length ? true : false;
                var $showPanel = isMeetingPanel ? $('.jquery-search-result.detail-result:visible') : self.panelShow();
                var $lis = $showPanel.find('li');
                var $active = $lis.filter('.active');
                var index = -1;
                var which = e.which;
                if ($active.length > 0) {
                    $.each($lis, function (i) {
                        if (this == $active[0]) {
                            index = i;
                            return true;
                        }
                    })
                }
                var len = $lis.length;
                if (which == 40) { //向下
                    if (index + 1 < len) {
                        $active.removeClass('active');
                        $active = $lis.eq(index + 1).addClass('active');
                        var scrollTop = self.$warp[0].scrollTop;
                        if ($active[0].offsetTop + 60 - scrollTop > 600) {  
                            self.$warp.scrollTop($active[0].offsetTop - 540);
                        }   
                    } else {
                        self.$warp.scrollTop($showPanel.height() - 600);
                    }
                } else if (which == 38) {
                    if (index - 1 >= 0) {
                        $active.removeClass('active');
                        $active = $lis.eq(index - 1).addClass('active');
                        var offsetTop = $active[0].offsetTop;
                        if (offsetTop  - self.$warp[0].scrollTop < 0) {  
                            self.$warp.scrollTop(offsetTop);
                        }
                    } else {
                        self.$warp.scrollTop(0);
                    }
                } else if (which == 13) {
                    $active.click();
                } else {
                    return;
                }
                return false;
            });
            $del.on("click", function() {
                $ipt.val("");
                $ipt.focus();
                $del.hide();
                self.hide(true);
                self.$DetailPanel.removeAttr('data-show');
            });
            var stopPropagation = false;
            this.$box.on('click', '.search-input-container', function (event) {
                stopPropagation = true;
                $(this).addClass('focus');
                $('.search-input').focus();
            });
            this.$panel.on('click', '.view-more-search-result', function (e) {
                e.stopPropagation();
                var $elm = $(this);
                var list = $elm.attr('list');
                var $panel = self.panelShow(list, true);
                if ($panel.find('.jquery-search-list').length == 0 || self.$DetailPanel.find('.default-search-result').length == 0) {
                    //加载中
                    self.search(self.searchKey, {
                        searchType: list,
                        searchScope: options.searchScope[list],
                        displayCount: options.listDisplayCount
                    })
                } else {
                    self.panelShow(list);
                }

                self.$ipt.focus();
            }).on('click', '.bread-crumb-return', function (e) {
                e.stopPropagation();
                self.panelShow('default');
                self.$ipt.focus();
            }).on('click', 'li', function (event) {
                event.stopPropagation();
                var $li = $(this);
                var res = options.onClick.call(self, $li);
                if (res !== false)
                    self.onClick($li, event);
            }).on('click', function (event) {
                stopPropagation = true;
                event.stopPropagation();
                self.$ipt.focus();
            });

            this.$DetailPanel.on('click', '.bread-crumb-return', function (e) {
                e.stopPropagation();
                self.$DetailPanel.hide().removeAttr('data-show');
                self.$panel.show();
                self.$ipt.focus();
            }).on('click', 'li', function (event) {
                event.stopPropagation();
                var $li = $(this);
                var res = options.onClick.call(self, $li);
                if (res !== false){
                    self.onClick($li, event);
                }
            }).on('click', function (event) {
                stopPropagation = true;
                event.stopPropagation();
                self.$ipt.focus();
            });

            $(document).on('click', function (e) {
                if (!stopPropagation) {
                    self.hide();
                }
                if ($(e.target).parent('.search-input-container').length == 0 && !$(e.target).hasClass('search-input-container') && !stopPropagation) {
                    $('.icon-search-del').hide();
                    $('.search-input-container').removeClass('focus');
                    $('.panel-search-result:visible').hide();
                    $('.jquery-search-result.detail-result:visible').hide();
                }

                stopPropagation = false;
            });
            
            var timer;
            this.$warp.on('scroll', function (){
                clearTimeout(timer);
                timer = setTimeout(self.onScroll(), 100);  
            });
            this.$MessagesWarp.on('scroll', function (){
                clearTimeout(timer);
                timer = setTimeout(self.onDetailScroll(this), 100);
            });
            //$(window).resize(function(){});
        },
        onClick: function ($li, e) {
            var self = this;
            var dataId = $li.attr('data-id');
            var dataType = $li.attr('data-type')*1;
            var siteId = $li.attr('data-site-id');
            var megCount = $li.attr('data-count')*1;
            var loginUserId = uc.IClientService.getCurrentUCID().userId;
            switch (dataType){
                case uc.constants.Search.SearchResultType.Contact:
                    if (uc.constants.Switch.OpenUserStatus) {
                        // do something
                        uc.IContactService.subscribeUserStatus([Number(dataId)]);
                    }
                    if (dataId == loginUserId) {
                        uc.IUIService.changeModule(uc.constants.UI.ModuleId.Me);
                    } else {
                        uc.IUIService.triggerCrossWindowEvent(uc.constants.UI.CrossWindowEventName.ReOpenCallWindowFormMessage, {chatType:uc.constants.Chat.ChatType.SingleChat, Id: dataId});
                        uc.IUIService.openChatWindow(uc.constants.Chat.ChatType.SingleChat, dataId, uc.constants.UI.OpenChatWindowSourceModule.Other, null, siteId, true);
                    }
                    break;
                case uc.constants.Search.SearchResultType.Project:
                    uc.IUIService.triggerCrossWindowEvent(uc.constants.UI.CrossWindowEventName.ReOpenCallWindowFormMessage, {chatType:uc.constants.Chat.ChatType.ProjectChat, Id: dataId});
                    uc.IUIService.openChatWindow(uc.constants.Chat.ChatType.ProjectChat, dataId, uc.constants.UI.OpenChatWindowSourceModule.Other, null, siteId, true);
                    break;
                case uc.constants.Search.SearchResultType.Group:
                    uc.IUIService.triggerCrossWindowEvent(uc.constants.UI.CrossWindowEventName.ReOpenCallWindowFormMessage, {chatType:uc.constants.Chat.ChatType.GroupChat, Id: dataId});
                    uc.IUIService.openChatWindow(uc.constants.Chat.ChatType.GroupChat, dataId, uc.constants.UI.OpenChatWindowSourceModule.Other, null, siteId, true);
                    break;
                case uc.constants.Search.SearchResultType.Meeting:
                    uc.IUIService.queryMeetingInCalendar(this.meetingDataMap[dataId]);
                    break;
                case uc.constants.Search.SearchResultType.Document:
                    break;
                case uc.constants.Search.SearchResultType.Link:
                    break;
                case uc.constants.Search.SearchResultType.Message:
                    var conversationType = $li.attr('data-index');
                    var conversationId = $li.attr('data-id');
                    var step = $li.attr('data-step');
                    var msgTime =  $li.attr('msgTime');
                    var message = self.options.getMessages(conversationType, conversationId);
                    if (megCount > 1) {
                        //request searchHistory BykeyWord
                        var _this =this;
                        _this.defaultMessage =message;
                        _this.filter ={};
                        _this.messageQueryStop =false;
                        if(this.filter.pageIndex==null) {
                            this.filter.pageIndex = 1;
                            this.$DetailPanel.attr({'data-step': step, 'data-show': 1}).find('.messages-detail').html('');
                        }
                        var pageSize =10;
                        var keyWord = self.searchKey;
                        this._searchMessageByKeyWrod(message.conversationType,message.relateId,keyWord,this.filter.pageIndex,pageSize);
                        e.stopPropagation();
                        message.message =[];
                        //self.detailPanelShow(message, step, self.searchKey);
                        return;
                    } else {
                        if(megCount==1) {
                            //only one data not group
                            msgTime = message.message[0].msgTime;
                        }
                        self.$panel.hide();
                        self.$DetailPanel.hide();
                        var meetingInfo =null;
                        if(message.conversationType==uc.constants.Conversation.ConversationType.MeetingMsg) {
                            var eventId = message.eventId;
                            uc.util.LogUtil.info('jquery.searchInput', 'searchShow-click', 'eventId', eventId);
                            var meetings = uc.IMeetingService.getMeetingInfoByIdSync(eventId, 0, 0, 0);
                            var userInfo = uc.IContactService.getCurrentUserInfo();
                            if (meetings && meetings.length) {
                                meetingInfo = meetings[0];
                            }
                            else
                            {
                                //eventId == 0 or meeting is null
                            }
                        }
                        uc.IUIService.openChatWindow(message.conversationType, dataId, uc.constants.UI.OpenChatWindowSourceModule.Other, meetingInfo, siteId, true, false, {
                            msgTime: msgTime,
                            keyword: self.searchKey
                        });
                    }
                    break;
                case uc.constants.Search.SearchResultType.CloudGroup:
                    uc.IUIService.openChatWindow(uc.constants.Chat.ChatType.CloudChat, dataId, uc.constants.UI.OpenChatWindowSourceModule.Other, null, siteId, true);
                    break;
            }
            $('.search-input-container').removeClass('focus');
            $('.search-input-container .search-input').val('');
            this.$del.hide();
            this.hide(true);
        },
        onScroll: function () {
            var $showPanel = this.panelShow();
            if ($showPanel.find('.jquery-search-loading').length == 0) return;
            if (this.$warp.height() + this.$warp.scrollTop() - $showPanel.height() > -40) {
                this.searchOnScroll($showPanel);
            }
        },
        onDetailScroll: function (scrollObj) {
            /*
            var $showPanel = this.$DetailPanel;
            if ($showPanel.find('.jquery-search-loading').length == 0) return;
            if (this.$warp.height() + this.$warp.scrollTop() - $showPanel.height() > -40) {
                this.searchOnScroll($showPanel);
            }
             */
            var top = scrollObj.scrollTop;
            if (this.messageQueryStop||$(scrollObj).attr("initload")!=undefined) {
                $(scrollObj).removeAttr("initload");
                return;
            }
            //this.onMoreMsgLoading();
            if(top == 0)
            {
                return;
            }
            if (top < 200) {
                //加载上一页
                this._searchMessageByKeyWrod(this.filter.conversationType, this.filter.relateId, this.filter.keyWord, this.filter.pageIndex, this.filter.pageSize);
            }
        },
        searchOnScroll: function ($showPanel) {
            var searchPage = $showPanel.find('.down-scroll-way-point').removeClass('jquery-search-loading').attr('currentsearchpage') * 1 + 1;
            this._search(this.searchKey, {
                searchType: this.showPanel,
                searchPage: searchPage
            });   
        },
        panelShow: function (list, noShow) {
            if (noShow)
                return this.$panel.find('.' + list + '-search-result');
            if (list === undefined || this.showPanel == list) {
                return this.$panel.find('.' + this.showPanel + '-search-result');
            }
            this.$panel.show();
            this.showPanel = list;
            var $res = this.$panel.find('.' + list + '-search-result');
            $res.show().siblings().hide();
            this.$warp.scrollTop(0);
            return $res;
        },
        detailPanelShow: function (message, step, keyword) {

            if (!message) {
                uc.util.LogUtil.info('jquery.searchInput', 'detailPanelShow', 'params null', {message: message, step: step, keyword: keyword});
                return;
            }
            var options = this.options;
            var searchCondition = {
                keyword:  keyword,
                searchPage: 1,
                searchType: "messageDetail"
            };
            var searchType = searchCondition.searchType, conversationInfo, displayName;
            switch (message.conversationType) {
                case 1:
                case 2:
                case 6:
                    conversationInfo = uc.IGroupService.getGroupInfo(message.relateId);
                    displayName = conversationInfo.name;
                    break;
                case 3:
                    conversationInfo = uc.modules.cache.BasicContactInfoCache.getBasicContactInfo(message.relateId);
                    displayName = conversationInfo.displayName;
                    break;
            }
            if (displayName) {
                this.$DetailPanel.find('.contact-bread-crumb').text(uc.util.StringUtil.escapeHtml(displayName));
            }
            this.$DetailPanel.find('.messages-detail').empty();
            this.$DetailPanel.attr({'data-step': step, 'data-show': 1}).find('.messages-detail').append(options.template[searchType]({
                messages: message,
                options: options,
                searchCondition: searchCondition
            }));

            this.$panel.hide();
            this._searchGroupAvatar();
            this.$DetailPanel.show();
            this.$ipt.focus();
        },
        hide: function (isClear) {
            var self = this;
            setTimeout(function () {
                self._hide(isClear);
            }, 20);
        },
        _hide: function (isClear) {
            this.$panel.hide();
            if (isClear) {
                this.clearSearchResults();
                this.$panel.hide();
                this.$DetailPanel.hide();
            }

            searchInputMap[this.uuid] = null;
            delete searchInputMap[this.uuid];
        },
        search: function (keyword, opt) {
            console.log('keyword:', keyword);
            if (!keyword) {
                this.hide(true);
                return;
            }
            searchInputMap[this.uuid] = this;
            var options = this.options;
       
            this.$del.show();
        	this._search(keyword, opt);
        },
        _search: function(keyword, opt){
            var self = this;
            var options = this.options;
            opt = opt || {searchType: 'default'};
            var searchCondition = {
                keyword: keyword,
                searchType: opt.searchType || 'default',
                displayCount: opt.searchType == 'default' ? options.defaultDisplayCount : options.listDisplayCount,
                searchPage: opt.searchPage || 1
            };
            //this.panelShow(searchCondition.searchType);
            if (searchCondition.searchPage == 1) {
                this.inSearching(searchCondition.searchPage);//搜索中
            }
            var dispalyCount =searchCondition.displayCount;
            if (searchCondition.searchType == 'default') {
                options.clearMessage();
                dispalyCount += 1;
            }

    		uc.ISearchService.searchServer(keyword, searchCondition.searchPage, dispalyCount , options.searchScope[searchCondition.searchType], function(args){
    			if (args[0] == 0){
    				var requestId = args[1];
    				self.requestMap[getSearchRequestKey(requestId)] = searchCondition;
    			}
    		});
        },
        _searchMessageByKeyWrod:function (currentChatType, currentChatId, keyword, pageIndex,pageSize) {
            var _this =this;
            searchInputMap[_this.uuid] = this;
            var options = _this.options;
            uc.ISearchService.searchHistoryMsgByKeyWord(currentChatType, currentChatId, keyword, pageIndex, pageSize, function (args) {
                var errorCode = args[0];
                var requestId = args[1];
                if (errorCode != 0) {
                    uc.util.LogUtil.info("jquery.searchInput", "_searchMessageByKeyWrod", "data=", {
                        groupId: currentChatId,
                        count: count,
                        page: pageIndex,
                        keyword: keyword
                    });
                }
                else {
                    _this.keyWordResultID = requestId;
                    _this.filter.conversationType = currentChatType;
                    _this.filter.relateId = currentChatId;
                    _this.filter.keyWord = keyword;
                    _this.filter.pageIndex = parseInt(pageIndex) + 1;
                    _this.filter.pageSize = pageSize;
                }
            });
        },
        _searchGroupUser: function (projects, groups, cloudGroups) {
            var res = [];
            var self = this;
            var ContactManager = uc.modules.contact.ContactManager;
            var cachedContactMap = {};
            $.each(projects, function () {
                $.each(slice.call(this.members, 0, 6), function () {
                    var cachedContact = ContactManager.getCachedContact(this.userId);
                    if (!cachedContact) {
                        res.push(this.userId);
                    } else {
                        cachedContactMap[this.userId] = cachedContact;
                    }
                })
            });
            $.each(groups, function () {
                $.each(slice.call(this.members, 0, 6), function () {
                    var cachedContact = ContactManager.getCachedContact(this.userId);
                    if (!cachedContact) {
                        res.push(this.userId);
                    } else {
                        cachedContactMap[this.userId] = cachedContact;
                    }
                })
            });
            $.each(cloudGroups, function () {
                $.each(slice.call(this.members, 0, 6), function () {
                    var cachedContact = ContactManager.getCachedContact(this.userId);
                    if (!cachedContact) {
                        res.push(this.userId);
                    } else {
                        cachedContactMap[this.userId] = cachedContact;
                    }
                })
            });
            res = $.unique(res);
            if (res.length > 0)
                ContactManager.batchRequestMembers(res);
            this._updataGroupName(cachedContactMap);
            this._updataGroupDescription();
        },
        _updataGroupName: function (users) {
            var $panel = this.$panel;
            $.each(users, function () {
                $panel.find('.group-member-' + this.userId).text(this.displayName);
            })
        },
        _updataGroupDescription: function () {
            this.$panel.find('.group-description').each(function () {
                var $member = $(this).find('.group-member');
                if ($member.length > 0) {
                    var text = [];
                    var flag = true;
                    $member.each(function () {
                        if (this.innerText) {
                            text.push(this.innerText);
                        } else {
                            flag = false;
                        }
                    })
                    if (flag)
                        this.innerHTML = "包含: " + text.join('、');  
                }
            })
        },
        searchResults: function (requestId, contacts, projects, groups, cloudGroups, meetings, messages) {//搜索完成

            this.$DetailPanel.removeAttr('data-show').hide();
            var options = this.options;
            meetings = options.excludeCancelMeeting(meetings);
            var key = getSearchRequestKey(requestId);
            var requestMap = this.requestMap;
            var searchCondition = requestMap[key];
            var listLength = 0;
            if (searchCondition == null) {
                return;
            }
            requestMap[key] = null;
            delete requestMap[key];

            $.each(slice.call(arguments, 1), function() {
                listLength += this.length;
            });

            var searchType = searchCondition.searchType;
            var $panel = this.panelShow(searchType, true); 
            clearTimeout(this.inSearchingTimer);

            if (!listLength) {
                $('.messages-search-result').hide();
                $('.jquery-search-result.detail-result:visible').hide();
                $panel.parents('.panel-search-result').show();
                $panel.show().html(defaults.emptyResultHtml);
                return;
            } else {
                $panel.html();
                $panel.parents('.panel-search-result').show();
            }
            
            if (searchCondition.searchPage == 1) {
                $panel.html(options.template[searchType + 'Search']({
                    contacts: contacts,
                    projects: projects,
                    groups: groups,
                    cloudGroups: cloudGroups,
                    meetings: meetings,
                    messages: options.excludeHost(messages),
                    options: options,
                    searchCondition: searchCondition
                }));
            } else {
                $panel.find('.down-scroll-way-point').remove();
                $panel.find('.jquery-search-list').append(options.template[searchType]({
                    contacts: contacts,
                    projects: projects,
                    groups: groups,
                    cloudGroups: cloudGroups,
                    meetings: meetings,
                    messages: options.excludeHost(messages),
                    options: options,
                    searchCondition: searchCondition
                }));
            }
            
            this.panelShow(searchType);
            if (messages && messages.length) {
                options.setMessages(options.excludeHost(messages));
            }
            if (this.searchKey != searchCondition.keyword) {
                this.clearSearchResults(true);
            }
            this.searchKey = searchCondition.keyword;
            
            var meetingDataMap = this.meetingDataMap;
            $.each(meetings, function () {
                meetingDataMap[this.eventId] = this;
            });
            this._searchGroupUser(projects, groups, cloudGroups);
            this._searchLoadUserAvatar();
            this._searchGroupAvatar();
            $panel.show();
        },
        _searchLoadUserAvatar: function () {
            var res = [];
            var cachedContactMap = {};
            this.$panel.find('.avatar-unloaded').each(function () {
                var userid = this.getAttribute('userid');
                var cachedContact = uc.modules.cache.BasicContactInfoCache.getBasicContactInfo(userid);
                if (!cachedContact) {
                    res.push(userid);
                } else {
                    if (cachedContact.localAvatar) {	        					
                        var $elm = $(this);
                        $elm.attr('src', cachedContact.localAvatar).removeClass('avatar-unloaded').addClass('avatar-loaded');
                    } else if (cachedContact.remoteAvatarUrl) {
                        uc.IUIService.downloadUserAvatar(userid, cachedContact.remoteAvatarUrl);
                    }
                }
            });
            //res = $.unique(res);
        },
        _searchGroupAvatar: function () {
            var res = [];
            var cachedContactMap = {};
            this.$DetailPanel.find('.avatar-unloaded').each(function () {
                var userid = this.getAttribute('userid');
                var cachedContact = uc.modules.cache.BasicContactInfoCache.getBasicContactInfo(userid);
                if (!cachedContact) {
                    res.push(userid);
                } else {
                    if (cachedContact.localAvatar) {
                        var $elm = $(this);
                        $elm.attr('src', cachedContact.localAvatar).removeClass('avatar-unloaded').addClass('avatar-loaded');
                    } else if (cachedContact.remoteAvatarUrl) {
                        uc.IUIService.downloadUserAvatar(userid, cachedContact.remoteAvatarUrl);
                    }
                }
            });
            //res = $.unique(res);
        },
        inSearching: function (searchType) {
            var self = this;
            clearTimeout(this.inSearchingTimer);
            this.inSearchingTimer = setTimeout(function () {
                //self.panelShow(searchType, true).html('<div style="font-size: 16px;height: 100px;line-height: 100px;text-align:center">正在搜索...</dvi>');
                //self.panelShow(searchType);
            }, 10);
        },
        clearSearchResults: function (notClearDefault) {
            if (notClearDefault) {
                this.$panel.find('.search-result.default-search-result').siblings().html('');
            } else {
                this.$panel.find('.search-result').html('');
            }
            this.meetingDataMap = {};
        },
        searchResults_byPaging: function (requestId,chatMessages,chatMessagesNum) {
            //分页搜索消息完成
            var keyword = this.searchKey;
            var options = this.options;
            if(chatMessages.length==0&&chatMessagesNum>0)
            {
                this.messageQueryStop =true;
            }
            var New_searchMsg = chatMessages.sort(function (a,b) {
                return  a.msgSeq -b.msgSeq;
            });
            var defaultMessage = this.defaultMessage;
            defaultMessage.message =[];
            New_searchMsg.map(function (item) {  defaultMessage.message.push(item); });

            var step ="1";
            var searchCondition = {
                keyword:  keyword,
                searchPage: 1,
                searchType: "messageDetail"
            };
            var searchType = searchCondition.searchType;
            var prepH =  this.$MessagesWarp[0].scrollHeight;
            switch (defaultMessage.conversationType) {
                case 1:
                case 2:
                case 6:
                    conversationInfo = uc.IGroupService.getGroupInfo(defaultMessage.relateId);
                    displayName = conversationInfo.name;
                    break;
                case 3:
                    conversationInfo = uc.modules.cache.BasicContactInfoCache.getBasicContactInfo(defaultMessage.relateId);
                    displayName = conversationInfo.displayName;
                    break;
            }
            if (displayName) {
                this.$DetailPanel.find('.contact-bread-crumb').text(uc.util.StringUtil.escapeHtml(displayName));
            }
           var detailObj = this.$DetailPanel.attr({'data-step': step, 'data-show': 1}).find('.messages-detail');
            detailObj.prepend(options.template[searchType]({
                messages: defaultMessage,
                options: options,
                searchCondition: searchCondition
            }));
            this.$panel.hide();
            this._searchGroupAvatar();
            this.$DetailPanel.show();
            this.$ipt.focus();
            if(this.filter.pageIndex==2) {
                this.$MessagesWarp.attr("initload", 1);
                this.$MessagesWarp.scrollTop(this.$MessagesWarp[0].scrollHeight);
            }
            if(this.filter.pageIndex>2)
            {
                this.$MessagesWarp.scrollTop(this.$MessagesWarp[0].scrollHeight-prepH);
            }

        }

    }
    
    $.fn.searchInput = function (options) {
        var args = slice.call(arguments, 1);
        var res;
        var flag = false;
        this.each(function () {
            var $elm = $(this);
            var obj = $elm.data('searchInput');
            if (typeof options == "string" ) {
                if (obj && obj[options]) {
                    var opt = obj[options];
                    if ($.isFunction(opt)) {
                        res = opt.apply(obj, args);
                        if (res !== undefined) {
                            flag = true;
                            return true;
                        }
                    } else {
                        if (args.length == 0) {
                            res = opt;
                            flag = true;
                            return true;
                        } else if (args.length == 1) {
                            obj[options] = args[0];
                        }
                    }
                }
            } else {
                $elm.data('searchInput', new SearchInput($elm, $.extend({}, defaults, options)));
            }
        })
        if (flag) {
            return res;
        } else {
            return this;
        }
    }
    
    $.fn.searchInput.defaults = defaults;
    $.fn.searchInput.OnContactInfoReceived = function (contacts) {
        for (key in searchInputMap) {
            searchInputMap[key]._updataGroupName(contacts);
            searchInputMap[key]._updataGroupDescription();
        }
    };
    $.fn.searchInput.OnServerSearchResultReceived = function (errorCode, requestId, contacts, projects, groups, cloudGroups, meetings, messages) {
        for (var key in searchInputMap) {
            searchInputMap[key].searchResults(requestId, contacts, projects, groups, cloudGroups, meetings, messages);
        }
    }
    $.fn.searchInput.OnServerSearchResultReceivedByPaging = function (errorCode, requestId,  chatMessages,chatMessagesNum) {
        for (var key in searchInputMap) {
            searchInputMap[key].searchResults_byPaging(requestId, chatMessages,chatMessagesNum);
        }
    }

})(jQuery);