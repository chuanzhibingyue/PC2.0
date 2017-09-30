/**
 * Created by senlin.ouyang on 2015/7/21.
 */
(function(uc){
    uc.modules.conversation.ConversationList = function(conversationListAdapter){
        this.conversationListAdapter = conversationListAdapter;
        this.conversationListAdapter.setDataSetObserver(this);
        this.conversationViewArray = [];
        this.searchBox = $('<div class="conversation-search-container"></div>').appendTo(uc.ui.Message.MenuPanel);
        this.container = $('<div class="conversation-list-container"></div>').appendTo(uc.ui.Message.MenuPanel);
        this.header = $('<div class="conversation-list-header"></div>');
        this.conversationList = $('<ul class="sub-menu-panel-body list-with-avatar-and-info conversation-list"></ul>');
        this.footer = $('<div class="conversation-list-footer"></div>');
        this.loadingIcon = $('<div class="conversation-loading-more-icon"></div>');
        this.container.append(this.header);
        this.container.append(this.conversationList);
        this.container.append(this.footer);
        this.container.append(this.loadingIcon);
        this.maxConversationViewCount = 20;
        this.conversationViewCount = 0;
        this.needUpdateView = true;
        this.isFirstMainLoaded = false;
        this.isFirstMainLoadedTimer = 0;
    };
    jQuery.extend(uc.modules.conversation.ConversationList.prototype, {
        init: function(){
            this.searchBox.searchInput({
                parent: uc.ui.Message.MenuPanel
            });
//            $('<div class="search-extra-menu icon-start">').appendTo(this.searchBox).popMenu({
//                //{title: uc_resource.Main.CreateProject, action: 'create-project'}
//                menus: [{title: uc_resource.Main.StartChat, action: 'start-chat'}, {title: uc_resource.Main.CreateMeeting, action: 'create-meeting'}]
//            });
            this.initConversationList();
            this.initConversationView();
            this.bindEvent();
            this.showConversationView();
        },
        initConversationList: function(){
            this.singleViewHeight = 70;
            this.beginPosition = 0;
            this.startIndex = 0;
            this.conversationListScrollTop = 0;
        },
        initConversationView: function(){
            //this.containerHeight = this.container.height();
            //TODO container height is not full height, need to check
            this.containerHeight = $('#layout-container').height() - 6; //there is 3px margin-top and margin-bottom, user main layout to avoid container height is zero bug
            this.conversationViewCountPerPage = Math.floor(this.containerHeight / this.singleViewHeight);
            this.maxConversationViewCount = this.conversationViewCountPerPage * 3;
            this.conversationCount = this.conversationListAdapter.getCount();
            this.conversationViewCount = this.conversationCount < this.maxConversationViewCount ? this.conversationCount : this.maxConversationViewCount;
            this.needReloadWhenScroll = this.conversationViewCount < this.conversationCount;
            //this.conversationViewCount = this.conversationCount;
            this.totalHeight = this.conversationCount * this.singleViewHeight;
            var createdConversationViewCount = this.conversationViewArray.length;
            var conversationView;
            for(var i = 0; i < this.conversationViewCount - createdConversationViewCount; i++){
                conversationView = new uc.modules.conversation.ConversationView();
                conversationView.init({parentNode: this.conversationList});
                this.conversationViewArray.push(conversationView);
            }
            this.viewHeight = this.conversationViewCount * this.singleViewHeight;
            this.endPosition = this.beginPosition + this.viewHeight;
            this.header.height(this.beginPosition);
            this.conversationList.height(this.viewHeight);
            this.footer.height(this.totalHeight - (this.beginPosition + this.viewHeight));
            this.calculateStartIndex(false);
        },
        showConversationView: function(){
            if(!this.needUpdateView){
                return;
            }
            //uc.util.LogUtil.info('ConversationList', 'ShowConversationView', 'Start to show conversation list -> startIndex = ' + this.startIndex, null);
            //uc.util.LogUtil.info('ConversationList', 'ShowConversationView', 'conversationCount = ' + this.conversationCount, null);
            //uc.util.LogUtil.info('ConversationList', 'ShowConversationView', 'conversationViewCount = ' + this.conversationViewCount, null);
            //uc.util.LogUtil.info('ConversationList', 'ShowConversationView', 'containerHeight = ' + this.containerHeight, null);
            //uc.util.LogUtil.info('ConversationList', 'ShowConversationView', 'headerListHeight = ' + this.header.height(), null);
            //uc.util.LogUtil.info('ConversationList', 'ShowConversationView', 'conversationListHeight = ' + this.viewHeight, null);
            //uc.util.LogUtil.info('ConversationList', 'ShowConversationView', 'footerHeight = ' + this.footer.height(), null);
            var conversationView, conversationInfo;
            this.shownIndex = -1;
            for(var i = 0; i < this.conversationViewCount; i++){
                conversationView = this.conversationViewArray[i];
                conversationView.show();
                conversationInfo = this.conversationListAdapter.getConversationInfo(this.startIndex + i);
                conversationView.setConversationInfo(conversationInfo);
                if(conversationInfo.selected){
                    this.shownIndex = i;
                }
            }
            for(var j = this.conversationCount, len = this.conversationViewArray.length - 1; j < len; j++){
                this.conversationViewArray[j].hide();
            }
            clearTimeout(this.isFirstMainLoadedTimer);
            this.isFirstMainLoadedTimer = setTimeout(function () {
                uc.modules.conversation.ConversationList.isFirstMainLoaded = true;
                uc.IUIService.startDownloadUserAvatar();
            }, 800)
        },

        bindEvent: function(){
            this.container.on('scroll', uc.util.EventUtil.hitch(this, 'onScroll'));
        },

        onScroll: function(){
            var scrollTop = this.container[0].scrollTop;
            if(this.containerHeight + scrollTop >= this.totalHeight){
                if(!uc.modules.conversation.ConversationManager.isHistoryConversationLoadEnd()){
                    uc.modules.conversation.ConversationManager.loadMoreHistoryConversation();
                }
            }
            if(this.needReloadWhenScroll){
                this.calculateStartIndex(true);
                this.showConversationView();
            }
            this.conversationListScrollTop = this.container.prop('scrollTop');
        },
        showMoreHistoryConversationLoading: function(){
            clearTimeout(this.hideLoadingIconTimer);
            this.loadingIcon.show();
        },
        onMoreHistoryConversationLoadEnd: function(){
            var _this = this;
            this.hideLoadingIconTimer = setTimeout(function(){
                _this.loadingIcon.hide();
            }, 10);
        },
        calculateStartIndex: function(isScroll){
            if(this.needReloadWhenScroll){
                var scrollTop = this.conversationListScrollTop;
                if(isScroll){
                    scrollTop = this.container[0].scrollTop;
                }
                var beginIndex = Math.floor(scrollTop / this.singleViewHeight);
                var firstShownItemOffset = scrollTop % this.singleViewHeight;
                if(scrollTop <= this.beginPosition || scrollTop + this.containerHeight >= this.endPosition){
                    this.beginPosition = scrollTop - firstShownItemOffset - this.singleViewHeight * this.conversationViewCountPerPage;
                    if(this.beginPosition < 0){
                        this.beginPosition = 0;
                    }
                    this.endPosition = this.beginPosition + this.viewHeight;
                    if(this.endPosition > this.totalHeight){
                        this.endPosition = this.totalHeight;
                        this.beginPosition = this.totalHeight - this.viewHeight;
                    }
                    this.header.height(this.beginPosition);
                    this.footer.height(this.totalHeight - this.endPosition);
                    var startIndex = beginIndex - this.conversationViewCountPerPage;
                    var endIndex = beginIndex + this.conversationViewCountPerPage;
                    if(startIndex <= 0){
                        startIndex = 0;
                    }
                    if(endIndex >= this.conversationCount - 1 || startIndex + this.conversationViewCount > this.conversationCount){
                        startIndex = this.conversationCount - this.conversationViewCount;
                    }
                    if(this.startIndex == startIndex && this.startIndex != 0){
                        this.needUpdateView = false;
                    }else{
                        this.needUpdateView = true;
                        this.startIndex = startIndex;
                    }
                }else{
                    this.needUpdateView = false;
                }
            }else{
                this.beginPosition = 0;
                this.endPosition = this.beginPosition + this.viewHeight;
                this.header.height(this.beginPosition);
                this.footer.height(this.totalHeight - this.endPosition);
                this.startIndex = 0;
                this.needUpdateView = false;
            }
        },
        showConversationList: function(){
            this.initConversationView();
            this.needUpdateView = true;
            this.showConversationView();
        },
        notifyDataSetChanged: function(){
            this.showConversationList();
        },
        clear: function(){

        },
        scrollToTop: function(){
            this.container[0].scrollTop= 0;
        },
        setConversationLoading: function(){
            this.conversationLoadingContainer = $('<div class="conversation-loading-container"></div>').appendTo(this.container);
            this.conversationLoadingContainer.append('<div class="conversation-loading">' +
                                                        '<div class="conversation-loading-bar"></div>'+
                                                        '<div class="conversation-loading-bar"></div>'+
                                                        '<div class="conversation-loading-bar"></div>'+
                                                        '<div class="conversation-loading-bar"></div>'+
                                                        '<div class="conversation-loading-bar"></div>'+
                                                        '<div class="conversation-loading-bar"></div>'+
                                                        '<div class="conversation-loading-bar"></div>'+
                                                        '<div class="conversation-loading-bar"></div>'+
                                                        '<div class="conversation-loading-bar"></div>'+
                                                        '<div class="conversation-loading-bar"></div>'+
                                                    '</div>');
        },
        setConversationLoadEnd: function(){
            this.conversationLoadingContainer && this.conversationLoadingContainer.remove();
        },
        getNextSelectedConversationIndex: function () {
            var nextIndex = -1;
            if(this.shownIndex != -1){
                if(this.shownIndex < this.conversationViewCount - 1){
                    nextIndex = this.shownIndex + 1 + this.startIndex;
                }else if(this.shownIndex == this.conversationViewCount - 1){
                    nextIndex = this.shownIndex - 1 + this.startIndex;
                }
            }
            return nextIndex;
        },
        getConversationViewCountPerPage: function(){
            return this.conversationViewCountPerPage;
        }
    })
})(uc);