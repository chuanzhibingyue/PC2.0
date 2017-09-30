(function(uc){
    uc.modules.pbx.PbxList = function(pbxListAdapter){
        this.pbxListAdapter = pbxListAdapter;
        this.pbxListAdapter.setDataSetObserver(this);
        this.pbxViewArray = [];
     //   this.back=$('<div class="pbx-back">' +
          //  '<a class="pbx-back_icon"><i class="icon-call_video_left"></i></a>' +
      //      '<a class="pbx-back_font">'+uc_resource.Pbx.Tel+'</a></div>').appendTo(uc.ui.Pbx.MenuPanel);
        this.searchBox = $('<div class="pbx-search-container">' +
            '<div class="pbx-search-panel">' +
            '<span class="icon-search"></span>' +
            '<input type="text" class="search-input" placeholder="'+uc_resource.Pbx.search+'" spellcheck="false">' +
            '<span class="icon-search-del"></span></div></div>').appendTo(uc.ui.Pbx.MenuPanel);
        this.container = $('<div class="pbx-list-container"></div>').appendTo(uc.ui.Pbx.MenuPanel);
        this.header = $('<div class="pbx-list-header"></div>');
        this.pbxList = $('<ul class="sub-menu-panel-body list-with-avatar-and-info pbx-list"></ul>');
        this.footer = $('<div class="pbx-list-footer"></div>');
        this.loadingIcon = $('<div class="pbx-loading-more-icon"></div>');
        this.dial=$('<div class="pbx-dial-btn icon-dialpaddown"></div>').appendTo(uc.ui.Pbx.MenuPanel);
        this.container.append(this.header);
        this.container.append(this.pbxList);
        this.container.append(this.footer);
        this.container.append(this.loadingIcon);
        this.maxPbxViewCount = 20;
        this.pbxViewCount = 0;
        this.needUpdateView = true;
        this.isFirstMainLoaded = false;
        this.isFirstMainLoadedTimer = 0;
    };
    jQuery.extend( uc.modules.pbx.PbxList.prototype,{
        init: function(){
            uc.modules.component.PbxSearch.initUI(uc.ui.Pbx.MenuPanel);
            this.initPbxList();
            this.initPbxView();
            this.bindEvent();
            this.showPbxView();
            this.setDial();
        },
        setDial:function(){
            var capability = JSON.parse(uc.util.StorageUtil.getData("capability"));
            if(capability && capability.pbx != 1){
              $(".pbx-dial-btn").addClass("hide");
            }
        },
        scrollToTop: function(){
            this.container[0].scrollTop= 0;
        },
        initPbxList:function(){
            this.singleViewHeight = 70;
            this.beginPosition = 0;
            this.startIndex = 0;
            this.pbxListScrollTop = 0;
        },
        initPbxView:function(){
            this.containerHeight = $('#layout-container').height();
            this.pbxViewCountPerPage = Math.floor(this.containerHeight / this.singleViewHeight);
            this.maxPbxViewCount = this.pbxViewCountPerPage * 3;
            this.pbxCount = this.pbxListAdapter.getCount();
            this.pbxViewCount = this.pbxCount < this.maxPbxViewCount ? this.pbxCount : this.maxPbxViewCount;
            this.needReloadWhenScroll = this.pbxViewCount < this.pbxCount;
            this.totalHeight = this.pbxCount * this.singleViewHeight;
            var createdConversationViewCount = this.pbxViewArray.length;
            var pbxView;
            for(var i = 0; i < this.pbxViewCount - createdConversationViewCount; i++){
                pbxView = new uc.modules.pbx.PbxView();
                pbxView.init({parentNode: this.pbxList});
                this.pbxViewArray.push(pbxView);
            }
            this.viewHeight = this.pbxViewCount * this.singleViewHeight;
            this.endPosition = this.beginPosition + this.viewHeight;
            this.header.height(this.beginPosition);
            this.pbxList.height(this.viewHeight);
            this.footer.height(this.totalHeight - (this.beginPosition + this.viewHeight));
            this.calculateStartIndex(false);
        },
        calculateStartIndex: function(isScroll){
            if(this.needReloadWhenScroll){
                var scrollTop = this.pbxListScrollTop;
                if(isScroll){
                    scrollTop = this.container[0].scrollTop;
                }
                var beginIndex = Math.floor(scrollTop / this.singleViewHeight);
                var firstShownItemOffset = scrollTop % this.singleViewHeight;
                if(scrollTop <= this.beginPosition || scrollTop + this.containerHeight >= this.endPosition){
                    this.beginPosition = scrollTop - firstShownItemOffset - this.singleViewHeight * this.pbxViewCountPerPage;
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
                    var startIndex = beginIndex - this.pbxViewCountPerPage;
                    var endIndex = beginIndex + this.pbxViewCountPerPage;
                    if(startIndex <= 0){
                        startIndex = 0;
                    }
                    if(endIndex >= this.pbxCount - 1 || startIndex + this.pbxViewCount > this.pbxCount){
                        startIndex = this.pbxCount - this.pbxViewCount;
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
        showPbxView: function(){
            if(!this.needUpdateView){
                return;
            }
            var pbxView, pbxInfo;
            this.shownIndex = -1;
            for(var i = 0; i < this.pbxViewCount; i++){
                pbxView = this.pbxViewArray[i];
                pbxView.show();
                pbxInfo = this.pbxListAdapter.getPbxInfo(this.startIndex + i);
                pbxView.setPbxInfo(pbxInfo);
                if(pbxInfo.selected){
                    this.shownIndex = i;
                }
            }
            for(var j = this.pbxCount, len = this.pbxViewArray.length - 1; j < len; j++){
                this.pbxViewArray[j].hide();
            }
            clearTimeout(this.isFirstMainLoadedTimer);
            this.isFirstMainLoadedTimer = setTimeout(function () {
                this.isFirstMainLoaded = true;
            }, 800)
        },
        showPbxList:function(){
            this.initPbxView();
            this.needUpdateView = true;
            this.showPbxView();
        },
        notifyDataSetChanged: function(){
            this.showPbxList();
        },
        getNextSelectedPbxIndex: function () {
            var nextIndex = -1;
            if(this.shownIndex != -1){
                if(this.shownIndex < this.pbxViewCount - 1){
                    nextIndex = this.shownIndex + 1 + this.startIndex;
                }else if(this.shownIndex == this.pbxViewCount - 1){
                    nextIndex = this.shownIndex - 1 + this.startIndex;
                }
            }
            return nextIndex;
        },
        onScroll: function(){
            var scrollTop = this.container[0].scrollTop;
            if(this.containerHeight + scrollTop >= this.totalHeight){
                if(!uc.modules.pbx.PbxManager.historyCallLoadEnd){
                    uc.modules.pbx.PbxManager.getPbxList();
                }
            }
            if(this.needReloadWhenScroll){
                this.calculateStartIndex(true);
                this.showPbxView();
            }
            this.pbxListScrollTop = this.container.prop('scrollTop');
        },
        bindEvent:function(){
            var self=this;
            this.container.on('scroll', uc.util.EventUtil.hitch(this, 'onScroll'));
            $(".pbx .pbx-list-container,.pbx-back").click(function(e){
                $(".pbx-dialPanel").addClass("hide").removeAttr("style");
            });
            this.dial.click(function(){
                uc.modules.pbx.PbxCallManager.openDialWindow();
            });
            $(".pbx-search-panel",this.searchBox).click(function(){
                $(this).addClass('focus-line');
                $('.pbx .search-input').focus();
            });
        }
    })
})(uc);