
(function(uc){	
	
	var searchRequestMap = new uc.util.HashMap();
	
    uc.modules.chooseContacts.SearchContactsManager = {
    	defaultDisplayCount: 3,
    	defaultSearch: "defaultSearch",
    	singleSearch: "singleSearch",
    	cachedKeyword: "",
        searchPage: 1,
    	getSearchRequestKey: function (requestId) {
            return "ContactsSearch_" + requestId;
        }, 
        getDefaultDisplayCount: function() {
        	return this.defaultDisplayCount;
        },
        searchDefault: function(keyword){
        	var _self = this;
        	_self.cachedKeyword = keyword;
        	var searchScope = [uc.constants.Search.SearchScope.Contact, uc.constants.Search.SearchScope.Project,
                               uc.constants.Search.SearchScope.Group];
        	var searchCondition = {
					keyword: keyword,
					searchType: _self.defaultSearch,
					needClear: true
				};
        	_self.clearSearchResults();
        	_self.inSearching();
        	
        	_self.search(keyword, uc.constants.Search.SearchPage, _self.defaultDisplayCount + 1, searchScope, searchCondition);
        	_self.bindEvents();
        },
        onSearchInput: function(keyword){
            var inputValue = keyword;
            var searchDelIcon = $('.icon-search-del');
            if(inputValue){
            	searchDelIcon.show();
                this.searchDefault(inputValue);
                this.inSearching();
            }else{
            	searchDelIcon.hide();
                this.hideSearchResult();
                //uc.modules.contact.ContactManager.switchPanel("panel-contacts");
            }
        },
        hideSearchResult: function() {
        	//$('.panel-search-result').hide();
        	$('.panel-search-result-parent').hide();
            this.searchPage = 1;
        },
        inSearching: function() {
        	//$('.panel-item').hide();
        	//$('.panel-search-result').show();
        	$('.panel-search-result-parent').show();
        	$('.search-result').hide();
        	$('.in-searching').show();
            $('.no-chooseContacts-result').hide();
        },
        afterSearch: function() {
        	$('.in-searching').hide();
        },
        searchByType: function(keyword, searchPage, searchCount, searchScope, needClear){
        	var _self = this;
        	var searchCondition = {
					keyword: keyword,
					searchType: _self.singleSearch,
					needClear: needClear
				};
        	if(needClear){
        		_self.clearSearchResults();
            	_self.inSearching();
        	}
        	_self.search(keyword, searchPage, searchCount, searchScope, searchCondition);
        },
        bindEvents: function() {
        	var _self = this;
        	$('.popup-panel-left').off("click", ".view-more-contacts");
        	$('.popup-panel-left').off("click", ".view-more-projects");
        	$('.popup-panel-left').off("click", ".view-more-chats");
        	$('.popup-panel-left').off("click", ".bread-crumb-return");
        	$('.popup-panel-left').off("keydown", ".search-input");

        	$('.popup-panel-left').on("click", ".view-more-contacts", function() {        		
        		$('.panel-search-result .more-search-result .contacts-result').show();
            	$('.panel-search-result .more-search-result .projects-result').hide();
            	$('.panel-search-result .more-search-result .chats-result').hide();
            	
        		var keyword = _self.cachedKeyword;
        		var searchScope = [uc.constants.Search.SearchScope.Contact];
                _self.searchPage = 1;
        		_self.searchByType(keyword, uc.constants.Search.SearchPage, uc.constants.Search.SearchCountPerPage + 1, searchScope, false);
        		
        		$('.panel-search-result').unbind('scroll');
            	$('.panel-search-result').scroll(function() {
            		setTimeout(_self.scrollOnContacts(), 100);            		   				  
    			});
        	});
        	$('.popup-panel-left').on("click", ".view-more-projects", function() {        		        		
        		$('.panel-search-result .more-search-result .contacts-result').hide();
            	$('.panel-search-result .more-search-result .projects-result').show();
            	$('.panel-search-result .more-search-result .chats-result').hide();
            	
        		var keyword = _self.cachedKeyword;
        		var searchScope = [uc.constants.Search.SearchScope.Project];
                _self.searchPage = 1;
        		_self.searchByType(keyword, uc.constants.Search.SearchPage, uc.constants.Search.SearchCountPerPage + 1, searchScope, false);
        		$('.panel-search-result').unbind('scroll');
            	$('.panel-search-result').scroll(function() {
            		setTimeout(_self.scrollOnProjects(), 100);            							  
    			});
        	});
        	$('.popup-panel-left').on("click", ".view-more-chats", function() {        		
        		$('.panel-search-result .more-search-result .contacts-result').hide();
            	$('.panel-search-result .more-search-result .projects-result').hide();
            	$('.panel-search-result .more-search-result .chats-result').show();
            	
        		var keyword = _self.cachedKeyword;
        		var searchScope = [uc.constants.Search.SearchScope.Group];
                _self.searchPage = 1;
        		_self.searchByType(keyword, uc.constants.Search.SearchPage, uc.constants.Search.SearchCountPerPage + 1, searchScope, false);
        		$('.panel-search-result').unbind('scroll');
            	$('.panel-search-result').scroll(function() {
            		setTimeout(_self.scrollOnGroupChats(), 100);            		 					  
    			});
        	});
        	$('.popup-panel-left').on("click", ".bread-crumb-return", function() {        		
        		$('.default-search-result').show();
            	$('.more-search-result').hide();
                _self.searchPage = 1;
        	});

            $('.popup-panel-left').on('keydown', '.search-input', function (e) {
                var keyCode = e.keyCode;
                var index = -1;
                var $parent = $('.panel-search-result>div').not(':hidden');
                var $li = $parent.find('li');
                var li_len = $li.length;
                var $hoverElement = $parent.find('li.hover');
                if (keyCode == 13) {
                    $hoverElement.click();
                    return;
                }

                if ($hoverElement.length > 0) {
                    $li.each(function (i) {
                        if($(this).hasClass('hover')) {
                            index = i;
                            $li.removeClass('hover');
                            return true;
                        }
                    });
                }

                switch (keyCode) {
                    case 38:
                        if (index - 1 <= 0) {
                            $li.eq(0).addClass('hover');
                        } else {
                            $li.eq(index - 1).addClass('hover');
                        }
                        e.preventDefault();
                        break;
                    case 40:
                         if (index + 1 < li_len) {
                             $li.eq(index + 1).addClass('hover');
                         } else {
                             $li.eq(li_len - 1).addClass('hover');
                         }
                        e.preventDefault();
                        break;
                }
            });
        },        
    	search: function(keyword, searchPage, searchCount, searchScope, searchCondition){
    		var _self = this;    		
    		//uc.constants.Search.SearchCountPerPage
            uc.util.LogUtil.info('SearchContactsManager', 'search', 'params', {
                keyword: keyword,
                searchPage: searchPage,
                searchCount: searchCount,
                searchScope: searchScope,
                searchCondition: searchCondition
            });
    		uc.ISearchService.searchServer(keyword, searchPage, searchCount, searchScope, function(args){
    			var errorCode = args[0];
    			if(errorCode == 0){
    				var requestId = args[1];
    				searchRequestMap.put(_self.getSearchRequestKey(requestId), searchCondition);    				  				
    			}
    		});
            _self.searchPage++;
        },
		displaySearchResults: function(requestId, contacts, projectGroups, chatGroups) {
			var _self = this;
			var currentUserInfo = uc.IContactService.getCurrentUserInfo();
			
			var key = _self.getSearchRequestKey(requestId);
            var temp = searchRequestMap.get(key);            
            if (temp == null) {
                return;
            }
            searchRequestMap.remove(key);
            var keyword = temp.keyword, searchType = temp.searchType, needClear = temp.needClear;
            if(needClear){
            	_self.clearSearchResults();
            }

            if (contacts.length == uc.ErrorCode.NoResult && projectGroups.length == uc.ErrorCode.NoResult && chatGroups.length == uc.ErrorCode.NoResult) {
                if (!$('.panel-search-result-parent ul').find('li').length) {
                    $('.panel-search-result').show()
                    $('.no-chooseContacts-result').show();
                }
                return;
            }
			
            //Search on default panel        	
        	//$('.panel-item').hide();
        	$('.panel-search-result').show();
            if(searchType == _self.defaultSearch){   
            	$('.default-search-result').show();
            	$('.more-search-result').hide();
            	_self.displayDefaultSearchContacts(contacts, currentUserInfo);
            	_self.displayDefaultSearchProjectGroups(projectGroups);
            	_self.displayDefaultSearchChatGroups(chatGroups);
            } else {
            	$('.default-search-result').hide();
            	$('.more-search-result').show();
            	_self.displaySingleSearchContacts(contacts);
            	_self.displaySingleSearchProjectGroups(projectGroups);
            	_self.displaySingleSearchChatGroups(chatGroups);
            }        	
        },
        clearSearchResults: function() {
        	$('.search-result').hide();
        	var ul = $('.panel-search-result .contact-list');
        	ul.empty();
        	ul = $('.panel-search-result .project-list');
        	ul.empty();
        	ul = $('.panel-search-result .group-chat-list');
        	ul.empty();        	
        },
        displayDefaultSearchContacts: function(contacts, currentUserInfo){
        	var _self = this;
        	var ul = $('.panel-search-result .default-search-result .contact-list');
        	var displayCount;
        	
        	displayCount = contacts.length > _self.defaultDisplayCount ? _self.defaultDisplayCount : contacts.length;
    		if(displayCount > 0){
        		$('.contacts-result').show();
        		for(var i = 0; i < displayCount; i++){
            		uc.modules.contact.ContactManager.loadContactItemUI(contacts[i], ul, null, currentUserInfo);
            	}
            	//Display more link
            	if(contacts.length > uc.modules.chooseContacts.SearchContactsManager.defaultDisplayCount){
            		$('.view-more-contacts').show();
            	} else {
            		$('.view-more-contacts').hide();
            	}
        	}
        },
        displaySingleSearchContacts: function(contacts) {
        	var _self = this;
        	var ul = $('.panel-search-result .more-search-result .contact-list');
        	var displayCount;
        	var needViewMore = false;
    		if(contacts.length == (uc.constants.Search.SearchCountPerPage + 1)) {
    			//Need scroll to show more search result
    			displayCount = uc.constants.Search.SearchCountPerPage;
    			needViewMore = true;
    		} else {
    			//End of search result, do not need to load more
    			displayCount = contacts.length;
    		}
    		if(displayCount > 0){
				var parentNode = $('.contacts-result:eq(1)');
        		$('.contacts-result').show();
        		var currentPage = 0;
        		var wayPoint = $(".down-scroll-way-point", parentNode);
            	if(wayPoint.length > 0){
            		if(wayPoint.attr("currentSearchPage")){
            			currentPage = Number(wayPoint.attr("currentSearchPage"));
            		}                		  
            		wayPoint.remove();
            	}
            	
        		for(var i = 0; i < displayCount; i++){
            		uc.modules.contact.ContactManager.loadContactItemUI(contacts[i], ul);
            	}
            	//Display more link
            	if(needViewMore){
            		wayPoint = _self.getDownScrollWayPointTemplate();
            		wayPoint.attr("currentSearchPage", currentPage + 1);
                    ul.append(wayPoint);
            	}                	               	
        	}
        },
        scrollOnContacts: function() {        	    	
        	var _self = this;
        	var wayPoint = $(".contact-list .down-scroll-way-point");
        	if(wayPoint.length == 0){
        		return;
        	}
        	var currentSearchPage = Number(wayPoint.attr("currentSearchPage"));
        	var nextSearchPage = currentSearchPage + 1;			
			if (_self.isBottom(wayPoint)) {
				//Load more data...				  
				_self.searchMoreContacts(nextSearchPage);
			}        	
        }, 
        scrollOnProjects: function() {        	  	
        	var _self = this;
        	var wayPoint = $(".project-list .down-scroll-way-point");
        	if(wayPoint.length == 0){
        		return;
        	}
        	var currentSearchPage = Number(wayPoint.attr("currentSearchPage"));
        	var nextSearchPage = currentSearchPage + 1;			
			if (_self.isBottom(wayPoint)) {
				//Load more data...				  
				_self.searchMoreProjects(nextSearchPage);
			}        	
        },
        scrollOnGroupChats: function() {        	     	
        	var _self = this;
        	var wayPoint = $(".group-chat-list .down-scroll-way-point");
        	if(wayPoint.length == 0){
        		return;
        	}
        	var currentSearchPage = Number(wayPoint.attr("currentSearchPage"));
        	var nextSearchPage = currentSearchPage + 1;			
			if (_self.isBottom(wayPoint)) {
				//Load more data...				  
				_self.searchMoreGroupChats(nextSearchPage);
			}        	
        },
        isBottom: function(wayPoint) {
        	var end = wayPoint.offset().top; 
			var viewEnd = $(window).scrollTop() + $(window).height(); 
			var distance = end - viewEnd; 			
			if (distance < 10) {
				//Load more data...				  
				return true;
			}
			return false;
        },
        searchMoreContacts: function(searchPage) {
        	var _self = this;
        	var keyword = _self.cachedKeyword;        	
    		var searchScope = [uc.constants.Search.SearchScope.Contact];    		
    		_self.searchMore(keyword, searchPage, searchScope);
        },
        searchMoreProjects: function(searchPage) {
        	var _self = this;
        	var keyword = _self.cachedKeyword;        	
    		var searchScope = [uc.constants.Search.SearchScope.Project];    		
    		_self.searchMore(keyword, searchPage, searchScope);
        },
        searchMoreGroupChats: function(searchPage) {
        	var _self = this;
        	var keyword = _self.cachedKeyword;        	
    		var searchScope = [uc.constants.Search.SearchScope.Group];    		
    		_self.searchMore(keyword, searchPage, searchScope);
        },
        searchMore: function(keyword, searchPage, searchScope){ 
        	var _self = this;
    		_self.searchByType(keyword, _self.searchPage, uc.constants.Search.SearchCountPerPage + 1, searchScope, false);
        },        
        getDownScrollWayPointTemplate: function() {
        	var wayPoint = $('<div class="down-scroll-way-point">'  				
		    				+ '	<img src="../images/contacts/loading.gif"/>'
		    				+ '</div>');
        	return wayPoint;
        },
        displayDefaultSearchProjectGroups: function(projectGroups) {
        	var ul = $('.panel-search-result .default-search-result .project-list');
        	this.displayDefaultSearchGroups(projectGroups, ul, uc.constants.GroupType.Project);        	        	
        },
        displayDefaultSearchChatGroups: function(chatGroups) {
        	var ul = $('.panel-search-result .default-search-result .group-chat-list');
        	this.displayDefaultSearchGroups(chatGroups, ul, uc.constants.GroupType.Group);        	
        },
        displayDefaultSearchGroups: function(groups, ul, groupType){
        	var _self = this;        	
        	var displayCount = groups.length > _self.defaultDisplayCount ? _self.defaultDisplayCount : groups.length; 
        	if(displayCount > 0){
        		var memberIdArray = [];
        		if(groupType == uc.constants.GroupType.Project){
        			uc.modules.group.GroupManager.addOrUpdateProjectGroupsToCache(groups);
        		}else{
        			uc.modules.group.GroupManager.addOrUpdateChatGroupsToCache(groups);
        		}
        		
        		
        		if(groupType == uc.constants.GroupType.Project){
        			$('.projects-result').show();
        		} else {
        			$('.chats-result').show();
        		}
        		for(var i = 0; i < displayCount; i++){
            		var currentGroupMemberIdArray = uc.modules.group.GroupManager.displayGroupItem(groups[i], ul, null);
            		memberIdArray = $.merge(memberIdArray, currentGroupMemberIdArray);
                	memberIdArray = $.unique(memberIdArray);
            	}
        		uc.modules.contact.ContactManager.batchRequestMembers(memberIdArray);
            	//Display more link
            	if(groups.length > _self.defaultDisplayCount){
            		if(groupType == uc.constants.GroupType.Project){            			
            			$('.view-more-projects').show();
            		} else {
            			$('.view-more-chats').show();
            		}        		
            	} else {
            		if(groupType == uc.constants.GroupType.Project){            			
            			$('.view-more-projects').hide();
            		} else {
            			$('.view-more-chats').hide();
            		}
            	}
        	}        	
        },
        displaySingleSearchProjectGroups: function(projectGroups) {        	
        	var ul = $('.panel-search-result .more-search-result .project-list');
        	this.displaySingleSearchGroups(projectGroups, ul, uc.constants.GroupType.Project);        	        	
        },
        displaySingleSearchChatGroups: function(chatGroups) {        	
        	var ul = $('.panel-search-result .more-search-result .group-chat-list');
        	this.displaySingleSearchGroups(chatGroups, ul, uc.constants.GroupType.Group);        	
        },
        displaySingleSearchGroups:function(groups, ul, groupType){        	
        	var _self = this;        	
        	var displayCount;
        	var needViewMore = false;
    		if(groups.length == (uc.constants.Search.SearchCountPerPage + 1)) {
    			//Need scroll to show more search result
    			displayCount = uc.constants.Search.SearchCountPerPage;
    			needViewMore = true;
    		} else {
    			//End of search result, do not need to load more
    			displayCount = groups.length;
    		}
    		if(displayCount > 0){
    			var memberIdArray = [];
    			uc.modules.group.GroupManager.addOrUpdateProjectGroupsToCache(groups);
				var parentNode = null;
    			if(groupType == uc.constants.GroupType.Project){
        			$('.projects-result').show();
					parentNode = $('.projects-result:eq(1)');
        		} else {
        			$('.chats-result').show();
					parentNode = $('.chats-result:eq(1)');
        		}
        		var currentPage = 0;
        		var wayPoint = $(".down-scroll-way-point", parentNode);
            	if(wayPoint.length > 0){
            		if(wayPoint.attr("currentSearchPage")){
            			currentPage = Number(wayPoint.attr("currentSearchPage"));
            		}                		  
            		wayPoint.remove();
            	}
            	
            	for(var i = 0; i < displayCount; i++){
            		var currentGroupMemberIdArray = uc.modules.group.GroupManager.displayGroupItem(groups[i], ul, null);
            		memberIdArray = $.merge(memberIdArray, currentGroupMemberIdArray);
                	memberIdArray = $.unique(memberIdArray);
            	}
            	uc.modules.contact.ContactManager.batchRequestMembers(memberIdArray);
            	//Display more link
            	if(needViewMore){
            		wayPoint = _self.getDownScrollWayPointTemplate();
            		wayPoint.attr("currentSearchPage", _self.searchPage);
            		ul.append(wayPoint);
            	}                	               	
        	} else {
                $(".group-chat-list .down-scroll-way-point").remove();
                $(".project-list .down-scroll-way-point").remove();
            }
        }
    }
})(uc);