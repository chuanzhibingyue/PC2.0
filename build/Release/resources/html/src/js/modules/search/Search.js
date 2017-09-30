/**
 * Created by senlin.ouyang on 2015/7/21.
 */
(function(uc){
    var _this = null;
    var SearchStatus = {
        SearchEnd: 0,
        Searching: 1,
        SearchSuccess: 2
    };
    uc.modules.search.Search = {
        init: function(){
            _this = this;
            this.searchTimer = -1;
            this.loginUserId = uc.IClientService.getCurrentUCID().userId;
            this.searchStatus = SearchStatus.SearchEnd;
            this.nodeHtml = '<div class="search-input-container">' +
                                '<span class="icon-search"></span>'+
                                '<input type="text" class="search-input" placeholder="' + uc_resource.Search.InputPlaceHolder + '" spellcheck="false">'+
                                '<span class="icon-search-del"></span>'+
                            '</div>' +
                            '<div class="searching"></div>' +
                            '<div class="search-output-container">' +
                                '<div class="search-category-container">' +
                                    '<span class="search-category selected" data-category="' + uc.constants.Search.SearchCategory.All + '">' + uc_resource.Search.All + '</span>'+
                                    '<span class="search-category" data-category="' + uc.constants.Search.SearchCategory.Contact + '">' + uc_resource.Search.Contact + '</span>'+
                                    '<span class="search-category" data-category="' + uc.constants.Search.SearchCategory.Meeting + '">' + uc_resource.Search.Meeting + '</span>'+
                                    '<span class="search-category" data-category="' + uc.constants.Search.SearchCategory.Document + '">' + uc_resource.Search.Document + '</span>'+
                                    '<span class="search-category" data-category="' + uc.constants.Search.SearchCategory.Link + '">' + uc_resource.Search.Link + '</span>'+
                                    '<span class="search-category" data-category="' + uc.constants.Search.SearchCategory.Message + '" style="display:none;">' + uc_resource.Search.Message + '</span>'+
                                '</div>'+
                                '<div class="search-result-container">' +
                                    '<div class="search-result"></div>'+
                                '</div>'+
                            '</div>';
            this.node = $('<div class="search"></div>').append(this.nodeHtml).appendTo(uc.ui.Search.MainPanel);
            this.bindEvent();
            this.noAvatarUserIdArray = [];
            this.noAvatarProjectIdArray = [];
            this.noContactInfoUserIdArray = [];
            this.noGroupInfoGroupIdArray = [];
            this.noMeetingInfoArray = [];
        },
        bindEvent: function(){
            this.searchInput = $('.search-input', this.node);
            this.searchDelIcon = $('.icon-search-del', this.node);
            this.searchOutputContainer = $('.search-output-container', this.node);
            this.searchResultNode = $('.search-result', this.node);
            this.searchCategory = $('.search-category', this.node);
            uc.util.PageUtil.addClickBodyHideNode(this.node[0], null, uc.util.EventUtil.hitch(this, 'hideSearchView'));
            this.searchInput.on('input', function(){
                _this.onInput();
            });
            this.searchDelIcon.click(function(){
                _this.onSearchDelIconClicked();
            });
            this.searchCategory.click(function(e){
                _this.onSearchCategoryClicked(e.target);
            });
            this.searchResultNode.on('click', 'li', function(e){
                setTimeout(function () {
                    _this.onSearchResultItemClicked(e.target);
                }, 200);
            });
            this.searchResultNode.on('click', '.search-result-more', function(e){
                _this.onSearchResultMoreClicked(e.target);
            });
            this.node.on('keyup', function(e){
                _this.onKeyup(e);
            })
        },
        search: function(){
            if(this.searchTimer){
                clearTimeout(this.searchTimer);
            }
            this.searchTimer = setTimeout(function(){
                uc.modules.search.SearchManager.searchServer(_this.searchInput.val().trim());
            }, uc.constants.Search.SearchTimerDelay);
        },
        onInput: function(){
            var inputValue = this.searchInput.val();
            if(inputValue.trim()){
                this.searchDelIcon.show();
                this.search();
                this.OnSearching();
            }else{
                this.searchDelIcon.hide();
                this.hideSearchResult();
                this.searchStatus = SearchStatus.SearchEnd;
            }
        },
        onSearchDelIconClicked: function(){
            this.searchInput.val('').focus();
            this.searchDelIcon.hide();
            this.hideSearchResult();
        },
        onSearchCategoryClicked: function(target){
            var category = $(target).attr('data-category');
            this._switchCategory(category, true);
        },
        _switchCategory: function(category, showResult){
            category = parseInt(category);
            this.searchCategory.removeClass('selected');
            $('.search-category[data-category="' + category + '"]', this.searchOutputContainer).addClass('selected');
            if(showResult){
                this.showSearchResult(category);
            }
        },
        onSearchResultItemClicked: function(target){
            var $li = $(target).closest('li');
            if($li){
                var dataId = $li.attr('data-id');
                var dataType = $li.attr('data-type');
                var siteId = parseInt($li.attr('data-site-id'));
                var needHideSearchView = true;
                switch (parseInt(dataType)){
                    case uc.constants.Search.SearchResultType.Contact:
                        if(dataId == this.loginUserId){
                            uc.IUIService.changeModule(uc.constants.UI.ModuleId.Me);
                        }else{
                            uc.IUIService.triggerCrossWindowEvent(uc.constants.UI.CrossWindowEventName.ReOpenCallWindowFormMessage, {chatType: uc.constants.Chat.ChatType.SingleChat, Id: dataId});
                            uc.IUIService.openChatWindow(uc.constants.Chat.ChatType.SingleChat, dataId, uc.constants.UI.OpenChatWindowSourceModule.Other, null, siteId, true);
                        }
                        break;
                    case uc.constants.Search.SearchResultType.Project:
                        uc.IUIService.triggerCrossWindowEvent(uc.constants.UI.CrossWindowEventName.ReOpenCallWindowFormMessage, {chatType: uc.constants.Chat.ChatType.ProjectChat, Id: dataId});
                        uc.IUIService.openChatWindow(uc.constants.Chat.ChatType.ProjectChat, dataId, uc.constants.UI.OpenChatWindowSourceModule.Other, null, siteId, true);
                        break;
                    case uc.constants.Search.SearchResultType.Group:
                        uc.IUIService.triggerCrossWindowEvent(uc.constants.UI.CrossWindowEventName.ReOpenCallWindowFormMessage, {chatType: uc.constants.Chat.ChatType.GroupChat, Id: dataId});
                        uc.IUIService.openChatWindow(uc.constants.Chat.ChatType.GroupChat, dataId, uc.constants.UI.OpenChatWindowSourceModule.Other, null, siteId, true);
                        break;
                    case uc.constants.Search.SearchResultType.Meeting:
                        var meetingData = this._getMeetingData(dataId);
                        uc.IUIService.queryMeetingInCalendar(meetingData);
                        break;
                    case uc.constants.Search.SearchResultType.Document:
                        break;
                    case uc.constants.Search.SearchResultType.Link:
                        break;
                    case uc.constants.Search.SearchResultType.Message:
                        if($li.hasClass('parent')){
                            $li.toggleClass('expanded');
                            needHideSearchView = false;
                        }else if($li.hasClass('child')){
                            var conversationType = $li.attr('data-conversation-type');
                            uc.IUIService.triggerCrossWindowEvent(uc.constants.UI.CrossWindowEventName.ReOpenCallWindowFormMessage, {chatType:Number(conversationType), Id: dataId});
                            uc.IUIService.openChatWindow(Number(conversationType), dataId, uc.constants.UI.OpenChatWindowSourceModule.Other, null, siteId, true);
                        }
                        break;
                }
                if(needHideSearchView){
                    this.hideSearchView();
                }

            }
        },
        _getMeetingData:function(eventId){
            eventId = Number(eventId);
            var meetingData;
            for(var i = 0, len = this.searchResult.meeting.length; i < len; i++){
                meetingData = this.searchResult.meeting[i];
                if(meetingData.eventId == eventId){
                    return meetingData;
                }
            }
        },
        onSearchResultMoreClicked: function(target){
            var searchCategory = $(target).attr('data-category');
            this._switchCategory(searchCategory,  true);
        },
        onKeyup: function(e){
            if(e.which == 27){
                this.hideSearchView();
            }
        },
        showSearchView: function(){
            uc.ui.Search.MainPanel.removeClass('hide');
            this.focusInput();
        },
        hideSearchView: function(){
            uc.ui.Search.MainPanel.addClass('hide');
            this.searchStatus = SearchStatus.SearchEnd;
        },
        focusInput: function(){
            this.searchInput.focus();
        },
        showSearchResultNode: function(){
            this.searchOutputContainer.show();
        },
        hideSearchResult: function(){
            this.searchOutputContainer.hide();
        },
        showSearchError: function(){
            this.searchResultNode.removeClass('searching').removeClass('no-result').addClass('error');
            this.searchResultNode.text(uc_resource.Search.NoResult);
        },
        showSearching: function(){
            this.searchResultNode.removeClass('error').removeClass('no-result').addClass('searching');
            this.searchResultNode.text(uc_resource.Search.Searching);
            this._switchCategory(uc.constants.Search.SearchCategory.All, false);
        },
        showSearchSuccess: function(){
            this.searchResultNode.removeClass('error').removeClass('searching').removeClass('no-result');
        },
        showNoResult: function(){
            this.searchResultNode.removeClass('searching').removeClass('error').addClass('no-result');
            this.searchResultNode.text(uc_resource.Search.NoResult);
        },

        OnSearching: function(){
            this.searchStatus = SearchStatus.Searching;
            this.searchResult = {
                contact: [],
                project: [],
                group: [],
                meeting: [],
                document: [],
                link: [],
                message: []
            };
            this.showSearchResultNode();
            this.showSearching();
        },
        OnServerSearchResultReceived: function(contacts, projects, groups, cloudGroups, meetings, messages){
            if(this.searchStatus != SearchStatus.Searching){
                return;
            }
            this.searchStatus = SearchStatus.SearchEnd;
            this.searchResult.contact = contacts ? contacts : [];
            this.searchResult.project = projects ? projects : [];
            this.searchResult.group =  groups ? groups: [];
            this.searchResult.meeting = meetings ? meetings : [];
            this.searchResult.message = messages ? messages : [];
            this.searchResult.document = [];
            this.searchResult.link = [];
            this.noAvatarUserIdArray = [];
            this.noAvatarProjectIdArray = [];
            this.noContactInfoUserIdArray = [];
            this.noGroupInfoGroupIdArray = [];
            this.noMeetingInfoArray = [];
            this._splitMessageResult();
            this.showSearchResult(uc.constants.Search.SearchCategory.All);
        },
        _splitMessageResult: function(){
            if(this.searchResult.message.length == 0){
                return;
            }
            this.searchResult.message.sort(this.messageCompareFunction);
            this.searchResult.message.conversation = {};
            var conversationArray;
            var messageData;
            var conversationType, conversationId;
            for(var i = 0, len = this.searchResult.message.length; i < len; i++){
                messageData = this.searchResult.message[i];
                conversationType = messageData.conversationType;
                switch (conversationType){
                    case uc.constants.Conversation.ConversationType.SingleChat:
                        conversationId = messageData.from.userId == this.loginUserId ? messageData.to.userId : messageData.from.userId;
                        break;
                    case uc.constants.Conversation.ConversationType.ProjectChat:
                    case uc.constants.Conversation.ConversationType.GroupChat:
                    case uc.constants.Conversation.ConversationType.MeetingMsg:
                        conversationId = messageData.to.userId;
                        break;
                }
                conversationArray = this.searchResult.message.conversation[conversationId + '-' + conversationType];
                if(conversationArray){
                    conversationArray.push(messageData);
                }else{
                    this.searchResult.message.conversation[conversationId + '-' + conversationType] = conversationArray = [messageData];
                }
            }
        },
        OnSearchError: function(){
            if(this.searchStatus == SearchStatus.SearchEnd){
                return;
            }
            this.searchStatus = SearchStatus.SearchEnd;
            this.showSearchError();
        },
        showSearchResult: function(category){
            this.searchResultNode.empty();
            switch (category){
                case uc.constants.Search.SearchCategory.All:
                    this.showAll();
                    break;
                case uc.constants.Search.SearchCategory.Contact:
                    this.showContact();
                    break;
                case uc.constants.Search.SearchCategory.Meeting:
                    this.showMeeting();
                    break;
                case uc.constants.Search.SearchCategory.Document:
                    this.showDocument();
                    break;
                case uc.constants.Search.SearchCategory.Link:
                    this.showLink();
                    break;
                case uc.constants.Search.SearchCategory.Message:
                    this.showMessage();
                    break;
                default :
                    break;
            }
        },
        showAll: function(){
            if(this.searchResult.contact.length == 0 && this.searchResult.group.length == 0
                && this.searchResult.meeting.length == 0 && this.searchResult.document.length == 0
                && this.searchResult.link.length == 0 && this.searchResult.message.length == 0
                && this.searchResult.project.length == 0){
                this.showNoResult();
                return;
            }
            this.showSearchSuccess();
            this._showContactInAll();
            this._showCalendarInAll();
            this._showDocumentInAll();
            this._showLinkInAll();
            this._showMessageInAll();
        },
        _showContactInAll: function(){
            if(this.searchResult.contact.length == 0 && this.searchResult.group.length == 0 && this.searchResult.project.length == 0){
                return;
            }
            var nodeArray = [], nodeCount = 0;
            var titleNode = '<div class="search-result-title-container">' +
                                '<span class="search-result-title">' + uc_resource.Search.Contact + '</span>' +
                                '<div class="search-result-title-split"></div>' +
                            '</div>';
            if(this.searchResult.contact.length){
                var contactData, contactNode;
                for(var i = 0, contactCount = this.searchResult.contact.length; i < contactCount && i < uc.constants.Search.SearchResultCountPerCategoryInAll; i++){
                    contactData = this.searchResult.contact[i];
                    contactNode = this._getContactNode(contactData);
                    nodeArray.push(contactNode);
                    nodeCount++;
                }
            }
            if(this.searchResult.project.length && nodeCount < uc.constants.Search.SearchResultCountPerCategoryInAll){
                var projectData, projectNode;
                for(var j = 0, projectCount = this.searchResult.project.length; j < uc.constants.Search.SearchResultCountPerCategoryInAll - nodeCount && j < projectCount; j++){
                    projectData = this.searchResult.project[j];
                    projectNode = this._getProjectNode(projectData);
                    nodeArray.push(projectNode);
                }
            }
            if(this.searchResult.group.length && nodeCount < uc.constants.Search.SearchResultCountPerCategoryInAll){
                var groupData, groupNode;
                for(var k = 0, groupCount = this.searchResult.group.length; k < uc.constants.Search.SearchResultCountPerCategoryInAll - nodeCount && k < groupCount; k++){
                    groupData = this.searchResult.group[k];
                    groupNode = this._getGroupNode(groupData);
                    nodeArray.push(groupNode);
                }
            }
            if(this.searchResult.contact.length + this.searchResult.project.length + this.searchResult.group.length > uc.constants.Search.SearchResultCountPerCategoryInAll){
                this.searchResultNode.append(titleNode);
                this.searchResultNode.append('<ul>' + nodeArray.join('')+ '</ul>');
                this.searchResultNode.append('<div class="search-result-more" data-category="' + uc.constants.Search.SearchCategory.Contact + '">' + uc_resource.Search.ViewMoreContact+ '</div>');
            }else{
                this.searchResultNode.append(titleNode);
                this.searchResultNode.append('<ul>' + nodeArray.join('')+ '</ul>');
            }
        },
        _showCalendarInAll: function(){
            if(this.searchResult.meeting.length == 0){
                return;
            }
            var nodeArray = [];
            var meetingData, meetingNode;
            for(var i = 0, len = this.searchResult.meeting.length; i < len && i < uc.constants.Search.SearchResultCountPerCategoryInAll; i++){
                meetingData = this.searchResult.meeting[i];
                meetingNode = this._getMeetingNode(meetingData);
                nodeArray.push(meetingNode);
            }
            var titleNode = '<div class="search-result-title-container">' +
                                '<span class="search-result-title">' + uc_resource.Search.Meeting + '</span>' +
                                '<div class="search-result-title-split"></div>' +
                            '</div>';
            if(this.searchResult.meeting.length  > uc.constants.Search.SearchResultCountPerCategoryInAll){
                this.searchResultNode.append(titleNode);
                this.searchResultNode.append('<ul>' + nodeArray.join('')+ '</ul>');
                this.searchResultNode.append('<div class="search-result-more"  data-category="' + uc.constants.Search.SearchCategory.Meeting + '">' + uc_resource.Search.ViewMoreMeeting+ '</div>');
            }else{
                this.searchResultNode.append(titleNode);
                this.searchResultNode.append('<ul>' + nodeArray.join('')+ '</ul>');
            }
        },
        _showDocumentInAll: function(){
            if(this.searchResult.document.length == 0){
                return;
            }
            var nodeArray = [];
            var documentData, documentNode;
            for(var i = 0, len = this.searchResult.document.length; i < len && i < uc.constants.Search.SearchResultCountPerCategoryInAll; i++){
                documentData = this.searchResult.document[i];
                documentNode = this._getDocumentNode(documentData);
                nodeArray.push(documentNode);
            }
            var titleNode = '<div class="search-result-title-container">' +
                                '<span class="search-result-title">' + uc_resource.Search.Document + '</span>' +
                                '<div class="search-result-title-split"></div>' +
                            '</div>';
            if(this.searchResult.document.length >  uc.constants.Search.SearchResultCountPerCategoryInAll){
                this.searchResultNode.append(titleNode);
                this.searchResultNode.append('<ul>' + nodeArray.join('')+ '</ul>');
                this.searchResultNode.append('<div class="search-result-more"  data-category="' + uc.constants.Search.SearchCategory.Document + '">' + uc_resource.Search.ViewMoreDocument+ '</div>');
            }else{
                this.searchResultNode.append(titleNode);
                this.searchResultNode.append('<ul>' + nodeArray.join('')+ '</ul>');
            }
        },
        _showLinkInAll: function(){
            if(this.searchResult.link.length == 0){
                return;
            }
            var nodeArray = [];
            var linkData, linkNode;
            for(var i = 0, len = this.searchResult.link.length; i < len && i < uc.constants.Search.SearchResultCountPerCategoryInAll; i++){
                linkData = this.searchResult.link[i];
                linkNode = this._getLinkNode(linkData);
                nodeArray.push(linkNode);
            }
            var titleNode = '<div class="search-result-title-container">' +
                                '<span class="search-result-title">' + uc_resource.Search.Link + '</span>' +
                                '<div class="search-result-title-split"></div>' +
                            '</div>';
            if(this.searchResult.link.length > uc.constants.Search.SearchResultCountPerCategoryInAll){
                this.searchResultNode.append(titleNode);
                this.searchResultNode.append('<ul>' + nodeArray.join('')+ '</ul>');
                this.searchResultNode.append('<div class="search-result-more"  data-category="' + uc.constants.Search.SearchCategory.Link + '">' + uc_resource.Search.ViewMoreLink+ '</div>');
            }else{
                this.searchResultNode.append(titleNode);
                this.searchResultNode.append('<ul>' + nodeArray.join('')+ '</ul>');
            }
        },
        _showMessageInAll: function(){
            if(this.searchResult.message.length == 0){
                return;
            }
            var nodeArray = [];
            var messageNode, messageArray, count = 1, hasMore = false;
            for(var conversation in this.searchResult.message.conversation){
                if(count > uc.constants.Search.SearchResultCountPerCategoryInAll){
                    hasMore = true;
                    break;
                }
                messageArray = this.searchResult.message.conversation[conversation];
                messageNode = this._getMessageNode(conversation, messageArray);
                nodeArray.push(messageNode);
                count++;
            }
            var titleNode = '<div class="search-result-title-container">' +
                                '<span class="search-result-title">' + uc_resource.Search.Message + '</span>' +
                                '<div class="search-result-title-split"></div>' +
                            '</div>';
            if(hasMore){
                this.searchResultNode.append(titleNode);
                this.searchResultNode.append('<ul>' + nodeArray.join('')+ '</ul>');
                this.searchResultNode.append('<div class="search-result-more" data-category="' + uc.constants.Search.SearchCategory.Message + '">' + uc_resource.Search.ViewMoreMessage+ '</div>');
            }else{
                this.searchResultNode.append(titleNode);
                this.searchResultNode.append('<ul>' + nodeArray.join('')+ '</ul>');
            }
        },
        showContact: function(){
            if(this.searchResult.contact.length == 0 && this.searchResult.group.length == 0 && this.searchResult.project.length == 0){
                this.showNoResult();
                return;
            }
            this.showSearchSuccess();
            this.searchResultNode.empty();
            var nodeArray = [];
            var contactData, projectData, groupData;
            var contactNode, projectNode,groupNode;
            for(var i = 0, contactCount = this.searchResult.contact.length; i < contactCount; i++){
                contactData = this.searchResult.contact[i];
                contactNode = this._getContactNode(contactData);
                nodeArray.push(contactNode);
            }
            for(var j = 0, projectCount = this.searchResult.project.length; j < projectCount; j++){
                projectData = this.searchResult.project[j];
                projectNode = this._getProjectNode(projectData);
                nodeArray.push(projectNode);
            }
            for(var k = 0, groupCount = this.searchResult.group.length; k < groupCount; k++){
                groupData = this.searchResult.group[k];
                groupNode = this._getGroupNode(groupData);
                nodeArray.push(groupNode);
            }
            this.searchResultNode.append('<ul>' + nodeArray.join('')+ '</ul>');
        },
        showMeeting: function(){
            if(this.searchResult.meeting.length == 0){
                this.showNoResult();
                return;
            }
            this.showSearchSuccess();
            this.searchResultNode.empty();
            var nodeArray = [];
            var meetingData, meetingNode;
            for(var i = 0, len = this.searchResult.meeting.length; i < len; i++){
                meetingData = this.searchResult.meeting[i];
                meetingNode = this._getMeetingNode(meetingData);
                nodeArray.push(meetingNode);
            }
            this.searchResultNode.append('<ul>' + nodeArray.join('')+ '</ul>');
        },
        showDocument: function(){
            if(this.searchResult.document.length == 0){
                this.showNoResult();
                return;
            }
            this.showSearchSuccess();
            this.searchResultNode.empty();
        },
        showLink: function(){
            if(this.searchResult.link.length == 0){
                this.showNoResult();
                return;
            }
            this.showSearchSuccess();
            this.searchResultNode.empty();
        },
        showMessage: function(){
            if(this.searchResult.message.length == 0){
                this.showNoResult();
                return;
            }
            this.showSearchSuccess();
            this.searchResultNode.empty();
            var nodeArray = [];
            var messageNode, messageArray;
            for(var conversation in this.searchResult.message.conversation){
                messageArray = this.searchResult.message.conversation[conversation];
                messageNode = this._getMessageNode(conversation, messageArray);
                nodeArray.push(messageNode);
            }
            this.searchResultNode.append('<ul>' + nodeArray.join('')+ '</ul>');
        },
        _getContactNode: function(contactData){
            if(!contactData){
                return '';
            }
            var userId = contactData.userId;
            var titleAvatarData = this._getContactTitleAndAvatarNode(contactData.userId, contactData);
            var resultType = uc.constants.Search.SearchResultType.Contact;
            return '<li class="search-result-contact" data-id="' + userId + '" data-type="' + resultType + '" data-site-id="' + contactData.siteId + '">' +
                        '<span class="search-result-contact-avatar-node">'+ titleAvatarData.avatarNode+  '</span>' +
                        '<div class="search-result-contact-name">' + uc.util.StringUtil.escapeHtml(titleAvatarData.title) + '</div>' +
                        '<div class="search-result-contact-position-node">' +
                            '<span class="search-result-contact-department">' + uc.util.StringUtil.escapeHtml(contactData.departmentName) + '</span>' +
                            '<span class="search-result-contact-position">' + uc.util.StringUtil.escapeHtml(contactData.position) + '</span>' +
                        '</div>' +
                    '</li>';
        },
        _getProjectNode: function(projectData){
            if(!projectData){
                return '';
            }
            var projectId = projectData.id;
            var titleAvatarData = this._getProjectTitleAndAvatar(projectId, projectData);
            var resultType = uc.constants.Search.SearchResultType.Project;
            return '<li class="search-result-group" data-id="' + projectId + '" data-type="' + resultType + '" data-site-id="' + projectData.siteId + '">' +
                        '<span class="search-result-group-avatar-node">' + titleAvatarData.avatarNode +  '</span>' +
                        '<div class="search-result-group-name">' + uc.util.StringUtil.escapeHtml(titleAvatarData.title) + '</div>' +
                        '<div class="search-result-group-host-name"></div>' +
                    '</li>';
        },
        _getGroupNode: function(groupData){
            if(!groupData){
                return '';
            }
            var groupId = groupData.id;
            var titleAvatarNode = this._getGroupTitleAndAvatar(groupId, groupData);
            var resultType = uc.constants.Search.SearchResultType.Group;
            return '<li class="search-result-group" data-id="' + groupId + '" data-type="' + resultType + '" data-site-id="' + groupData.siteId + '">' +
                        '<span class="search-result-group-avatar-node">' + titleAvatarNode.avatarNode +  '</span>' +
                        '<div class="search-result-group-name">' + uc.util.StringUtil.escapeHtml(titleAvatarNode.title) + '</div>' +
                        '<div class="search-result-group-host-name"></div>' +
                    '</li>';
        },

        _getMeetingNode: function(meetingData){
            if(!meetingData){
                return '';
            }
            var meetingId = meetingData.conferenceId;
            var hostName = meetingData.hostName;
            var eventId = meetingData.eventId;
            var titleAvatarNode = this._getMeetingTitleAndAvatar(meetingId, eventId, meetingData);
            var resultType = uc.constants.Search.SearchResultType.Meeting;
            return '<li class="search-result-meeting" data-id="' + eventId + '" data-type="' + resultType + '">' +
                        '<span class="search-result-meeting-avatar-node">' + titleAvatarNode.avatarNode +  '</span>' +
                        '<div class="search-result-meeting-name">' + uc.util.StringUtil.escapeHtml(titleAvatarNode.title) + '</div>' +
                        '<div class="search-result-meeting-host-name">' + uc.util.StringUtil.formatString(uc_resource.Search.MeetingHostName, [hostName]) + '</div>' +
                    '</li>';
        },
        _getDocumentNode: function(documentData){
            if(!documentData){
                return '';
            }
            var resultType = uc.constants.Search.SearchResultType.Document;
        },
        _getLinkNode: function(linkData){

        },
        _getMessageNode: function(conversation, messageArray){
            var conversationData = conversation.split('-');
            var conversationId = Number(conversationData[0]),
                conversationType = Number(conversationData[1]);
            var message, titleAvatarData;
            var messageNode, childMessageNode, messageNodeArray = [];
            switch (conversationType){
                case uc.constants.Conversation.ConversationType.SingleChat:
                    titleAvatarData = this._getContactTitleAndAvatarNode(conversationId);
                    break;
                case uc.constants.Conversation.ConversationType.ProjectChat:
                    titleAvatarData = this._getProjectTitleAndAvatar(conversationId);
                    break;
                case uc.constants.Conversation.ConversationType.GroupChat:
                    titleAvatarData = this._getGroupTitleAndAvatar(conversationId);
                    break;
                case uc.constants.Conversation.ConversationType.MeetingMsg:
                    //TODO no meeting
                    titleAvatarData = this._getMeetingTitleAndAvatar(0, 0);
                    break;
            }
            if(messageArray && messageArray.length > 1){
                message = uc.util.StringUtil.formatString(uc_resource.Search.MessageCount, [messageArray.length]);
                for(var i = 0, len = messageArray.length; i < len; i++){
                    childMessageNode = this._getChildMessageNode(titleAvatarData.title, titleAvatarData.avatarNode, messageArray[i], conversationId, conversationType);
                    messageNodeArray.push(childMessageNode);
                }
                messageNode = this._getParentMessageNode(titleAvatarData.title, titleAvatarData.avatarNode, message, conversationId, messageNodeArray.join(''));

            }else{
                messageNode = this._getChildMessageNode(titleAvatarData.title, titleAvatarData.avatarNode, messageArray[0], conversationId, conversationType);
            }
            return messageNode;
        },
        _getParentMessageNode: function(title, avatarNode, message, conversationId, childNodes){
            var resultType = uc.constants.Search.SearchResultType.Message;
            return '<li class="search-result-message parent" data-id="' + conversationId + '" data-type="' + resultType +'">' +
                        '<span class="search-result-message-avatar-node">' + avatarNode +  '</span>' +
                        '<div class="search-result-message-name">' + uc.util.StringUtil.escapeHtml(title) + '</div>' +
                        '<div class="search-result-message-message">' + uc.util.StringUtil.escapeHtml(message) + '</div>' +
                        '<span class="search-result-message-more"></span>' +
                        '<ul class="search-result-message-child-container">' + childNodes+ '</ul>' +
                    '</li>';
        },
        _getChildMessageNode: function(title, avatarNode, messageData, conversationId, conversationType, siteId){
            var msgSeq = messageData.msgSeq;
            var timestamp = messageData.msgTime;
            var message = messageData.textContent;
            var resultType = uc.constants.Search.SearchResultType.Message;
            var timeStr = uc.util.TimeUtil.getMessageTimeString(timestamp, null, uc.settings.meeting.timeZone.offset);
            return '<li class="search-result-message child" data-id="' + conversationId + '" data-type="' + resultType +
                                    '" data-conversation-type="' + conversationType + '" + data-msg-seq="'+ msgSeq + '" data-site-id="' + siteId + '">' +
                        '<span class="search-result-message-avatar-node">' + avatarNode +  '</span>' +
                        '<div class="search-result-message-name">' + uc.util.StringUtil.escapeHtml(title) + '</div>' +
                        '<div class="search-result-message-message">' + uc.util.StringUtil.escapeHtml(message) + '</div>' +
                        '<span class="search-result-message-message-time">' + timeStr + '</span>' +
                    '</li>';
        },
        _getContactTitleAndAvatarNode: function(userId, contactInfo){
            var title, avatarNode;
            if(!contactInfo){
                contactInfo = uc.IContactService.getBasicContactInfo(userId);
            }
            if(contactInfo){
                title = contactInfo.displayName;
                if(contactInfo.localAvatar){
                    avatarNode = '<img src="' + contactInfo.localAvatar + '" draggable="false" class="round-image" onerror="uc.util.ImageUtil.OnImageLoadError(this,\'' + uc.constants.UI.DefaultLocalUrl.Avatar + '\')" >';
                }else{
                    if(contactInfo.remoteAvatarUrl){
                        this.noAvatarUserIdArray.push(userId);
                        uc.IUIService.downloadUserAvatar(contactInfo.userId, contactInfo.remoteAvatarUrl);
                    }
                    avatarNode = '<img src="' + uc.constants.UI.DefaultLocalUrl.Avatar + '" draggable="false" class="round-image" rel="' +
                                userId + '_'+ uc.constants.UI.AvatarType.User + '" onerror="uc.util.ImageUtil.OnImageLoadError(this,\'' + uc.constants.UI.DefaultLocalUrl.Avatar + '\')" >';
                }
            }else{
                title = '<span rel="' + userId + '">' + userId + '</span>';
                avatarNode = '<img src="' + uc.constants.UI.DefaultLocalUrl.Avatar + '" draggable="false" class="round-image" rel="' +
                                userId + '_'+ uc.constants.UI.AvatarType.User + '" onerror="uc.util.ImageUtil.OnImageLoadError(this,\'' + uc.constants.UI.DefaultLocalUrl.Avatar + '\')" >';
                this.noContactInfoUserIdArray.push(userId);
            }
            return {
                title: title,
                avatarNode: avatarNode
            }
        },
        _getProjectTitleAndAvatar: function(projectId, projectInfo){
            var title, avatarNode;
            if(!projectInfo){
                projectInfo = uc.IGroupService.getGroupInfo(projectId);
            }
            if(projectInfo){
                title = projectInfo.name;
                if(projectInfo.localAvatar){
                    avatarNode = '<img class="round-image" draggable="false" src="' + projectInfo.localAvatar + '" onerror="uc.util.ImageUtil.OnImageLoadError(this,\'' + uc.constants.UI.DefaultLocalUrl.Project + '\')" >';
                }else{
                    if(projectInfo.avatarIndex){
                        this.noAvatarProjectIdArray.push(projectId);
                        uc.IUIService.downloadProjectAvatar(projectId, projectInfo.avatarIndex);
                    }
                    avatarNode = '<img class="round-image" draggable="false" src="' + uc.constants.UI.DefaultLocalUrl.Project + '" rel="' +
                                projectId + '_' + uc.constants.UI.AvatarType.Project + '" onerror="uc.util.ImageUtil.OnImageLoadError(this,\'' + uc.constants.UI.DefaultLocalUrl.Project + '\')" >';
                }
            }else{
                title = '<span rel="' + projectId + '">' + projectId + '</span>';
                avatarNode = '<img src="' + uc.constants.UI.DefaultLocalUrl.Project + '" draggable="false" class="round-image" rel="' +
                            projectId + '_'+ uc.constants.UI.AvatarType.Project + '" onerror="uc.util.ImageUtil.OnImageLoadError(this,\'' + uc.constants.UI.DefaultLocalUrl.Project + '\')" >';
                this.noGroupInfoGroupIdArray.push(projectId);
            }
            return {
                title: title,
                avatarNode: avatarNode
            }
        },
        _getGroupTitleAndAvatar: function(groupId, groupInfo){
            if(!groupInfo){
                groupInfo = uc.IGroupService.getGroupInfo(groupId);
            }
            var title, avatarNode, avatarArray = [], userIdArray = [];
            if(groupInfo){
                title = groupInfo.name;
                var adminId = groupInfo.adminId;
                var contactInfo, userId;
                if(adminId){
                    contactInfo = uc.IContactService.getBasicContactInfo(adminId);
                    if(contactInfo.localAvatar){
                        avatarArray.push(contactInfo.localAvatar);
                    }else{
                        if(contactInfo.remoteAvatarUrl){
                            this.noAvatarUserIdArray.push(adminId);
                            uc.IUIService.downloadUserAvatar(adminId, contactInfo.remoteAvatarUrl);
                        }
                        avatarArray.push(uc.constants.UI.DefaultLocalUrl.Avatar);
                    }
                    userIdArray.push(adminId);
                }
                for(var i = avatarArray.length, len = groupInfo.members.length, index = 0; i < len && i < uc.constants.UI.GroupAvatarUserCount; index++){
                    userId = groupInfo.members[index].userId;
                    if(!userId || userIdArray.indexOf(userId) != -1){
                        continue;
                    }
                    contactInfo = uc.IContactService.getBasicContactInfo(userId);
                    if(contactInfo){
                        if(contactInfo.localAvatar){
                            avatarArray.push(contactInfo.localAvatar);
                        }else{
                            if(contactInfo.remoteAvatarUrl){
                                this.noAvatarUserIdArray.push(userId);
                                uc.IUIService.downloadUserAvatar(userId, contactInfo.remoteAvatarUrl);
                            }
                            avatarArray.push(uc.constants.UI.DefaultLocalUrl.Avatar);
                        }
                    }else{
                        avatarArray.push(uc.constants.UI.DefaultLocalUrl.Avatar);
                        this.noContactInfoUserIdArray.push(userId);
                    }

                    userIdArray.push(userId);
                    i++;
                }
                for(var j = avatarArray.length; j < uc.constants.UI.GroupAvatarUserCount; j++){
                    avatarArray.push(uc.constants.UI.DefaultLocalUrl.Avatar);
                    userIdArray.push(0);
                }
            }else{
                title =  '<span rel="' + groupId + '">' + groupId + '</span>';
                for(var k = 0; k < uc.constants.UI.GroupAvatarUserCount; k++){
                    userIdArray.push(0);
                    avatarArray.push(uc.constants.UI.DefaultLocalUrl.Avatar);
                }
                this.noGroupInfoGroupIdArray.push(groupId);
            }
            avatarNode = this.composeGroupAvatar(userIdArray, avatarArray, uc.constants.UI.DefaultLocalUrl.Avatar);
            return {
                title: title,
                avatarNode: avatarNode
            }
        },
        _getMeetingTitleAndAvatar: function(meetingId, eventId, meetingInfo){
            if(!meetingInfo){
                meetingInfo = uc.IMeetingService.getMeetingInfoByIdSync(/* meetingId, */ eventId, 0, 0, 0);
            }
            var title, avatarNode;
            if(meetingInfo){
                var startTime = meetingInfo.startTime;
                title = meetingInfo.title;
                var isGNet = meetingInfo.isGNet;
                var isMeetingCancelled = meetingInfo.isOld;//TODO get meeting status
                avatarNode = uc.util.AvatarUtil.composeMeetingAvatar(startTime, isGNet, isMeetingCancelled);
            }else{
                title =  '<span rel="' + meetingId + '">' + meetingId + '</span>';
                avatarNode = uc.util.AvatarUtil.composeMeetingAvatar(0, 0, false);
                this.noMeetingInfoArray.push(meetingId);
            }
            return {
                title: title,
                avatarNode: avatarNode
            }
        },
        composeGroupAvatar: function (userIdArray, localAvatarUrlArray, defaultAvatar) {
            var str = '';
            var len = localAvatarUrlArray.length;
            if (len == 2) {
                str =   '<div class="group-avatar">' +
                '<img src="' + localAvatarUrlArray[0]  + '" onerror="uc.util.ImageUtil.OnImageLoadError(this,\'' + defaultAvatar + '\')" class="avatar-admin round-image" draggable="false" rel="' + userIdArray[0] + '_' + uc.constants.UI.AvatarType.User + '">' +
                '<img src="' + localAvatarUrlArray[1]  + '" onerror="uc.util.ImageUtil.OnImageLoadError(this,\'' + defaultAvatar + '\')" class="avatar-attendee round-image" draggable="false" rel="' + userIdArray[0] + '_' + uc.constants.UI.AvatarType.User + '">' +
                '</div>'
            }  else if (len == 1) {
                str =   '<img src="' + localAvatarUrlArray[0]  + '" onerror="uc.util.ImageUtil.OnImageLoadError(this,\'' + defaultAvatar + '\')" draggable="false" class="avatar-attendee round-image">';
            } else {
                str =   '<img src="' + defaultAvatar + '" class="avatar-attendee round-image" draggable="false">';
            }
            return str;
        },
        OnUserAvatarDownloaded: function(userId, serverAvatarUrl, localAvatarUrl){
            if(this.noAvatarUserIdArray.indexOf(userId) != -1){
                var rel = userId + "_" + uc.constants.UI.AvatarType.User;
                $('img[rel="' + rel + '"]', this.node).attr('src', localAvatarUrl);
            }
        },
        OnProjectAvatarDownloaded: function(groupId, serverAvatarUrl, localAvatarUrl){
            if(this.noAvatarProjectIdArray.indexOf(groupId) != -1){
                var rel = groupId + "_" + uc.constants.UI.AvatarType.Project;
                $('img[rel="' + rel + '"]', this.node).attr('src', localAvatarUrl);
            }
        },
        OnContactInfoReceived: function(userIds, contacts){
            if(contacts && contacts.length){
                var contactInfo;
                for(var i = 0, len = contacts.length; i <  len; i++){
                    contactInfo = contacts[i];
                    if(!contactInfo){
                        continue;
                    }
                    if(this.noContactInfoUserIdArray.indexOf(contactInfo.userId) != -1){
                        if(contactInfo.displayName){
                            $('span[rel="' + contactInfo.userId + '"]', this.node).replaceWith(contactInfo.displayName);
                        }
                        if(!contactInfo.localAvatar && contactInfo.remoteAvatarUrl){
                            uc.IUIService.downloadUserAvatar(contactInfo.userId, contactInfo.remoteAvatarUrl);
                        }
                    }
                }
            }
        },
        OnGroupInfoReceived: function(groupId, group){

        },
        messageCompareFunction: function(messageData1, messageData2) {
            return messageData2.msgTime - messageData1.msgTime;
        }
    }
})(uc);