(function(uc){
	
	var uploadTaskMap = new uc.util.HashMap();
	
	uc.modules.group.GroupManager = {
			GROUP_PROJECT: "project",
			GROUP_CHAT: "chat",
			batchCount: 20,
	    	poundKeyReplacer: "ZZ",	 
	    	messageDisturbSwitcher: null,
	    	cachedProjectGroups: new uc.modules.group.GroupDataSet(),
	    	cachedChatGroups: new uc.modules.group.GroupDataSet(),	    	
	        getCachedProjectGroups: function () {
	    		return this.cachedProjectGroups.getAllGroups();
	    	},
	    	clearCachedProjectGroups: function() {
	    		this.cachedProjectGroups = new uc.modules.group.GroupDataSet();
	    	},
	    	clearCachedChatGroups: function() {
	    		this.cachedChatGroups = new uc.modules.group.GroupDataSet();
	    	},
	    	addOrUpdateProjectGroupsToCache: function(groups) {
	    		var _self = this;
	    		 $.each(groups,function(key, group){
	    			 _self.addOrUpdateProjectGroupToCache(group.id, group);
	             }); 
	        }, 
	        addOrUpdateProjectGroupToCache: function(groupId, groupInfo) {	        	
	    		var _self = this;	    		
	            if (!groupInfo) {
	                return;
	            }	            
	            if (this.cachedProjectGroups.isGroupExist(groupId)) {	            	
	            	var group = _self.findCachedGroupByGroupId(groupId);

	            	if(groupInfo.localAvatar == ""  && group.localAvatar != "") {
	            		groupInfo.localAvatar = group.localAvatar;
	            	}
	            	this.cachedProjectGroups.updateGroup(groupId, groupInfo);
	            } else {	            	
	                this.cachedProjectGroups.addGroup(groupId, groupInfo);	                
	            }
	        }, 
	        removeProjectGroup: function (groupId) {
	            if (this.cachedProjectGroups.isGroupExist(groupId)) {
	                this.cachedProjectGroups.removeGroup(groupId);	                
	            }
	        },
	        addOrUpdateGroupToCache: function(groupId, group) {
	        	var _self = this;
	        	if(group.groupType == uc.constants.GroupType.Project){
        			_self.addOrUpdateProjectGroupToCache(groupId, group);
        		} else {
        			_self.addOrUpdateChatGroupToCache(groupId, group);
        		}
	        },
	        getCachedProjectGroup: function (groupId) {
	            return this.cachedProjectGroups.getGroupInfo(groupId);
	        },
	        isProjectGroupExist: function (groupId) {
	            return this.cachedProjectGroups.isProjectGroupExist(groupId);
	        },
	        getCachedChatGroups: function () {
	    		return this.cachedChatGroups.getAllGroups();
	    	},
	    	addOrUpdateChatGroupsToCache: function(groups) {
	    		var _self = this;
	    		 $.each(groups,function(key, group){
	    			 _self.addOrUpdateChatGroupToCache(group.id, group);
	             }); 
	        }, 
	        removeChatGroup: function (groupId) {
	            if (this.cachedChatGroups.isGroupExist(groupId)) {
	                this.cachedChatGroups.removeGroup(groupId);	                
	            }
	        },
	        addOrUpdateChatGroupToCache: function(groupId, group) {
	            if (!group) {
	                return;
	            }
	            if (this.cachedChatGroups.isGroupExist(groupId)) {
	            	this.cachedChatGroups.updateGroup(groupId, group);
	            } else {
	                this.cachedChatGroups.addGroup(groupId, group);	                
	            }
	        }, 
	        getCachedChatGroup: function (groupId) {
	            return this.cachedChatGroups.getGroupInfo(groupId);
	        },
	        isChatGroupExist: function (groupId) {
	            return this.cachedChatGroups.isChatGroupExist(groupId);
	        },
	        batchRefreshChatGroupsAvatar: function() {        		
        		var _self = this;        		
        		var groupItems = $('.group-chat-list .group-item');
        		for(var i = 0; i < groupItems.length; i++){
        			var groupItem = $(groupItems[i]);
        			var groupAvatar = groupItem.find('.group-avatar');
	        		if(groupAvatar.hasClass("avatar-loaded")){
	        			continue;
	        		}
	        		var groupId = Number(groupItem.attr("groupId"));
	        		var group = _self.findCachedGroupByGroupId(groupId);
	        		if(group){
	        			_self.refreshChatGroupAvatar(groupId, group);
	        		}	        			        		
        		}        			        	 
	        },
	        postRefreshGroupsAvatar: function(userId, serverAvatarUrl, localAvatarUrl){
	        	var _self = this;	        	
	        	
	        	//Set localAvatar back to cache
        		var currentContact = uc.modules.contact.ContactManager.getCachedContact(userId);
        		if(currentContact != null && currentContact.localAvatar != localAvatarUrl) {
        			currentContact.localAvatar = localAvatarUrl;
        			uc.modules.contact.ContactManager.addOrUpdateContactToCache(userId, currentContact);
        		}	  
        		
        		if($('.group-chat-list:visible').length == 0){
	        		return;
        		}
        		
        		_self.batchRefreshChatGroupsAvatar();
	        	      	
	        },
			loadProjectsPanel: function(groups) {	        	
	        	var _self = this;
	        	var parentPanel = uc.modules.contact.ContactManager.getLatestClickedContactsPanel();
	        	if(parentPanel == null){
	        		return;
	        	}
	        	//var panelProjects = parentPanel.find('.panel-projects');
	        	var panelProjects = parentPanel.find('.panel-groupChats');
	        	if(panelProjects.attr("loaded") == true) {
	        		return;
	        	}
	        	panelProjects.attr("loaded", true);        	
	        	
	        	var ul = panelProjects.find('.group-list');
	        	ul.empty();	        	
	        	
	        	//Sorted array, #(ZZ) last, 
	        	var alphabeticalSortGroupArray = uc.modules.contact.ContactManager.getAlphabeticalSortArray(groups, "pinyin", "name");        	
	        	_self.addOrUpdateProjectGroupsToCache(alphabeticalSortGroupArray);	        	
	        	
	        	_self.loadMoreGroups(panelProjects, uc.modules.group.GroupManager.GROUP_PROJECT);
				
				panelProjects.scroll(function() {
					_self.scrollOnGroups($(this), uc.modules.group.GroupManager.GROUP_PROJECT);	  
				});
	        }, 
	        loadGroupChatPanel: function(groups) {	        	
	        	var _self = this;
	        	var parentPanel = uc.modules.contact.ContactManager.getLatestClickedContactsPanel();
	        	if(parentPanel == null){
	        		return;
	        	}
	        	
	        	var panelGroupChats = parentPanel.find('.panel-groupChats');
	        	if(panelGroupChats.attr("loaded") == true) {
	        		return;
	        	}
	        	panelGroupChats.attr("loaded", true); 
	        	
	        	var ul = panelGroupChats.find('.group-list');
	        	ul.empty();
	        	
	        	//Sorted array, #(ZZ) last, 
	        	var alphabeticalSortGroupArray = uc.modules.contact.ContactManager.getAlphabeticalSortArray(groups, "pinyin", "name");        	

	        	_self.addOrUpdateChatGroupsToCache(alphabeticalSortGroupArray);
	        	
	        	_self.loadMoreGroups(panelGroupChats, uc.modules.group.GroupManager.GROUP_CHAT);
				
	        	panelGroupChats.scroll(function() {
					_self.scrollOnGroups($(this), uc.modules.group.GroupManager.GROUP_CHAT);	  
				});	        	
	        },
			scrollOnGroups: function(panel, groupType) {
	        	var _self = this;
	        	var wayPoint = panel.find(".down-scroll-way-point");
	        	if(wayPoint.length == 0){
	        		return;
	        	}
				var end = wayPoint.offset().top; 
				var viewEnd = $(window).scrollTop() + $(window).height(); 
				var distance = end - viewEnd;
				if (distance <= -70) {
					//Load more data...				  
					_self.loadMoreGroups(panel, groupType);
				}	
	        },
	        loadMoreGroups: function(parentPanel, groupType) {
	        	_self = this;   
	        	
	        	var ul = parentPanel.find('.group-list');        	
	        	var wayPoint = parentPanel.find('.down-scroll-way-point');
				var moreItems = [];
				var currentItem = null;
				
				var groups;
				
				if(groupType == uc.modules.group.GroupManager.GROUP_PROJECT){
	        		groups = _self.getCachedProjectGroups();	        		
	        	}else{
	        		groups = _self.getCachedChatGroups();	        		
	        	}
				
	        	if(wayPoint.length == 0){
	        		moreItems = uc.modules.contact.ContactManager.loadMoreItems(groups, _self.batchCount, currentItem);
	        	} else {
					var lastGroupItem = ul.find('.group-item').last();
					var lastGroupId = lastGroupItem.attr("groupId"); 
					//currentItem = uc.util.CollectionUtil.findObjectByKey(groups, "id", Number(lastGroupId));
					if(groupType == uc.modules.group.GroupManager.GROUP_PROJECT){		        		
						currentItem = _self.getCachedProjectGroup(Number(lastGroupId));
		        	}else{		        		
		        		currentItem = _self.getCachedChatGroup(Number(lastGroupId));
		        	}
	        		moreItems = uc.modules.contact.ContactManager.loadMoreItems(groups, _self.batchCount, currentItem);
	        	}			
				_self.loadMoreGroupsUI(ul, moreItems, groupType);
				
				//If current loaded data cannot fill full of this panel, load more data
				wayPoint = parentPanel.find(".down-scroll-way-point");
				if(wayPoint.length == 1){
					var end = wayPoint.offset().top; 
					var viewEnd = $(window).scrollTop() + $(window).height(); 
					var distance = end - viewEnd;
					if(distance < 0){
						_self.loadMoreGroups(parentPanel, groupType);
					}
				}			
	        },	        
			loadMoreGroupsUI: function(ul, moreItems, groupType) {
				var _self = this;
				//[{key, [groups]}, {key, [groups]}], json format	        	      	
	        	var alphabeticalSortGroups = uc.modules.contact.ContactManager.alphabeticalSortItems(moreItems, "pinyin", "name");
	        	var memberIdArray = [];
	            $.each(alphabeticalSortGroups,function(key,groups){
					
	            	_self.displayGroupKey(ul, key, null);
	            	
	            	$.each(groups,function(index, group){	            		
	            		var currentGroupMemberIdArray = _self.displayGroupItem(group, ul, null);
	            		
	            		var cachedContacts = [];
	            		var unCachedMembers = [];
	            		for(var i = 0; i < currentGroupMemberIdArray.length; i++){
	            			var userId = currentGroupMemberIdArray[i];
	            			var cachedContact = uc.modules.contact.ContactManager.getCachedContact(userId);
			        		if(cachedContact != null){
			        			cachedContacts.push(cachedContact);
			        		} else {
			        			//Need fetch contact from server
			        			unCachedMembers.push(userId);
			        		}
	            		}	
	            		if(cachedContacts.length > 0){
	            			_self.loadGroupsMembersUI(cachedContacts);
	            		}	            		
	                	memberIdArray = $.merge(memberIdArray, unCachedMembers);
	                	memberIdArray = $.unique(memberIdArray);
	                });            	
	            });	           
				var parentPanel = ul.parent('.panel-item');
	            var wayPoint = parentPanel.find('.down-scroll-way-point');
				if(wayPoint.length == 0){
					wayPoint = uc.modules.contact.ContactManager.getDownScrollWayPointTemplate();				
					parentPanel.append(wayPoint);
				}
	            
				if(moreItems.length < _self.batchCount){
					//Unbind project panel scroll event
					parentPanel.unbind();
					wayPoint.remove();
				}
				
				if(memberIdArray.length > 0){	
					uc.modules.contact.ContactManager.batchRequestMembers(memberIdArray);
				}
			},
			displayGroupKey: function(ul, key, previousElement, isInsertBefore) {
				var _self = this;
				//If key is #, need transform to ZZ, Jquey does not support alphabetical-key-item-#
            	var keyItem = ul.find('li[alphabetical-key="'+ key +'"]');
            	if(keyItem.length == 0){
            		keyItem = _self._getGroupKeyTemplate();
            		keyItem.find('.sort-key').text(key.replace(_self.poundKeyReplacer, "#"));
            		keyItem.attr("alphabetical-key", key);
            		if(previousElement != null){
            			if(isInsertBefore){
            				keyItem.insertBefore(previousElement);
            			} else {
            				keyItem.insertAfter(previousElement);
            			}            			
            		} else {
            			ul.append(keyItem);
            		}                	
                	return keyItem;
            	}
            	return null;
			},
			displayGroupItem: function(group, ul, lastGroupElement){
				var _self = this;
				var currentGroupMemberIdArray = [];				
				if(ul.find(".group-item-" + group.id).length > 0){					
					return currentGroupMemberIdArray;
				}
				
				var groupType = uc.modules.group.GroupManager.GROUP_PROJECT;
				if(group.groupType == uc.constants.Conversation.ConversationType.GroupChat){
					groupType = uc.modules.group.GroupManager.GROUP_CHAT;
				}
				var template = _self._getGroupTemplate(groupType);
				
				var x = template.clone();                	
            	x.addClass("group-item-" + group.id);
            	x.attr("groupId", group.id);
            	x.attr("groupType", groupType);
            	x.attr("membersCount", group.members.length);
            	                	
            	x.find('.checkbox-parent').addClass("check-box-group-" + group.id);
                
                uc.modules.group.GroupManager.setGroupName(x.find('.group-name'), group.name);
                
            	x.find('.group-name').attr("groupId", group.id);
            	x.find('.group-name').attr("adminId", group.adminId);
            	x.find('.group-members-count').text(group.members.length);
            	
            	//If project, add project-avatar-groupid
            	if(groupType == uc.modules.group.GroupManager.GROUP_PROJECT){
            		var projectAvatarImage = x.find('.project-avatar');
            		projectAvatarImage.addClass("project-avatar-" + group.id);
            		_self.displayProjectAvatar(group, projectAvatarImage);
            	}            	
            	
            	//Group members spans                	
            	var groupMembersDiv = x.find('.group-members');            	
            	for(var i = 0; i < group.members.length; i++){
            		var member = group.members[i];
            		//For performance, only show first 6 members names
            		if(currentGroupMemberIdArray.length < 6){
            			currentGroupMemberIdArray.push(member.userId);
            			var groupMemberTemplate = $('<span class="group-member"></span>');
                		var groupMemberSpan = groupMemberTemplate.addClass("group-member-" + member.userId);
                		groupMembersDiv.append(groupMemberSpan);
            		} else {
            			break;
            		}            		
            	}            	
            	if(lastGroupElement != null){
            		x.insertAfter(lastGroupElement);
            	} else{
            		ul.append(x);
            	}
            	
            	return currentGroupMemberIdArray;
			},
			displayProjectAvatar: function(group, projectAvatarImage){				
				if(group.localAvatar != ""){					
					projectAvatarImage.attr("src", group.localAvatar);
				} else if(group.localAvatar == "" && group.avatarIndex != ""
	            	&& ( group.avatarIndex.toLowerCase().indexOf("http") >=0 || group.avatarIndex.toLowerCase().indexOf("https") >=0 ) ){
	            	uc.IUIService.downloadProjectAvatar(group.id, group.avatarIndex);
				}
			},
			_getGroupKeyTemplate: function() {
	        	var template = $('<li class="alphabetical-key-item">'  
			        		 + '<div class="fixed sort-key"></div>'
			        		 + '<div class="fluid"><div class="line"></div></div>'
	        				 + '</li>');
	        	return template;
	        },
	        _getGroupTemplate: function(groupType) {
	        	var template = $('<li class="group-item">'
						+ '	<div class="group-widget-container">'
						+ '		<div class="left"></div>'
						+ '		<div class="right">'					
						
						+ '         <div class="checkbox-parent check-box-group">'
						+ '         	<input type="checkbox"/>'
						+ '         	<label><span><img class="remove-group" src="../images/contacts/remove.png"/></span></label>'
						+ '         </div>'
						
						+ '			'
						+ '		</div>'
						+ '		<div class="center">'        					
						+ '			<div class="group-name-container"><span class="group-name"></span><span class="group-members-count-container">(<span class="group-members-count">10</span>)</span></div>'	
						+ '			<div class="group-description text-ellipsis"></div>'
						+ '			<div class="group-members" style="display:none"></div>'
		        		+ '		</div>'
		        		+ '	</div>'
		        		+ '</li>');
	        	if(groupType == uc.modules.group.GroupManager.GROUP_PROJECT){
	        		var defaultAvatar = uc.constants.UI.DefaultLocalUrl.Project;
	        		
	        		var groupAvatar = $('<div class="project-group-avatar">'
	        						+ ' 	<img src="' + defaultAvatar + '" onerror="uc.util.ImageUtil.OnImageLoadError(this,\'' + defaultAvatar + '\')" class="project-avatar round-image"></div>');	        		
	        		template.find('.left').append(groupAvatar);
	        	}else{		        		
	        		var groupAvatarUrlArray = [];
		        	groupAvatarUrlArray.push(uc.constants.UI.DefaultLocalUrl.Avatar);
		        	groupAvatarUrlArray.push(uc.constants.UI.DefaultLocalUrl.Avatar);
		        	var groupAvatar = uc.util.AvatarUtil.composeGroupAvatar(groupAvatarUrlArray, uc.constants.UI.DefaultLocalUrl.Avatar);
		        	var groupAvatarElement = $(groupAvatar);
		        	//groupAvatarElement.find('img').addClass("avatar");
		        	template.find('.left').append(groupAvatarElement);
	        	}
	        	
	        	return template;
	        },
	        loadGroupsCreatorsUI: function(contacts) {		        	
	        	$.each(contacts,function(k,contact){
	        		var userId = contact.userId;
	        		var creators = $('.group-creator-' + userId);	        		
	        		$.each(creators,function(i,creator){
	        			$(creator).text(contact.displayName);
	        		});
	        	});
	        },
	        loadGroupsMembersUI: function(contacts) {	        		   
	        	//Load Left Panel Group Members' Names & Chat Group Avatar
	        	var _self = this;
	        	var parentPanel = uc.modules.contact.ContactManager.getLatestClickedContactsPanel();
	        	if(parentPanel == null){
	        		return;
	        	}
	        	
	        	uc.modules.contact.ContactManager.addOrUpdateContactsToCache(contacts);
	        	
	        	//1. Process Group Members' Names
	        	$.each(contacts,function(k,contact){
	        		var userId = contact.userId;
	        		parentPanel.find('.group-member-' + userId).text(contact.displayName);	        		        		
	        	});
	        	
	        	var groupItems = parentPanel.find('.group-item');	        	
	        	
	        	$.each(groupItems,function(i,groupItem){	        		
	        		var groupDescription = $(groupItem).find('.group-description');
		        		var members = $(groupItem).find('.group-member');
		        		if(members.length > 0){
		        			var memberNameArray = [];
		            		$.each(members,function(j, member){
		            			var memberName = $(member).text();
		            			if(memberName){
		            				memberNameArray.push(memberName);
		            			}
		                	});
		            		var memberNames = uc_resource.Profile.Include + memberNameArray.join(uc_resource.Profile.Comma);
		            		groupDescription.text(memberNames);
		        		}
	        	});     
	        	
	        	//2. Process Chat Group Avatar
	        	if($('.group-chat-list:visible').length > 0){
	        		_self.batchRefreshChatGroupsAvatar();
        		}
	        },
	        loadGroupDetailTemplate: function(groupInfo) {
	        	var _self = this;
	        	var contentPanel = uc.ui.Contacts.ContentPanelBody;
	            contentPanel.empty();
	    		contentPanel.hide();
	    			    		
	    		var groupId = groupInfo.id ? groupInfo.id : groupInfo.groupId;	    		
	    		var group = _self.findCachedGroupByGroupId(groupId);
	    		
	    		var groupAvatarUrlArray = groupInfo.groupAvatarUrlArray;
	    		
	    		var creatorIdArray = [];
	    		if(group.adminId > 0){
	    			creatorIdArray.push(group.adminId);
	    			uc.IContactService.getBasicContactInfo(creatorIdArray);
	    		}	    		
	    		
	        	var template = $('<div class="group-detail text-selectable"></div>');
	        	var userInfo = uc.IContactService.getCurrentUserInfo();
	        	if(group.adminId == userInfo.userId){
	        		template.addClass("group-admin");
	        	}
	        	template.attr("groupId", groupId);
	        	template.attr("groupType", group.groupType);
	        	template.addClass("group-detail-" + groupId);
	        	
	        	var groupDetailUp = $('<div class="group-detail-up"></div>');
	        	var groupDetailDown = $('<div class="group-detail-down"></div>');
	        	template.append(groupDetailUp);
	        	template.append(groupDetailDown);
	        	
	        	var groupDetailDownWrapper = $('<div class="group-detail-down-wrapper"></div>');
	        	groupDetailDown.append(groupDetailDownWrapper);
	        	
	        	var groupWrapper = $('<div class="profile-wrapper">'
									    + '<div class="left-content left-none">'
									        + '<div class="group-name-container"><span class="group-name text-ellipsis"></span><span class="group-count-container text-ellipsis">(<span class="group-count">0</span>)</span><span class="edit-group text-ellipsis"></span></div>'
									        + '<div class="group-name-edit"><input maxlength="64" type="input" class="group-name-input"/></div>'
									        + '<div class="group-creator-container"><span class="group-creator-titile">' + uc_resource.Profile.ProjectCreater +'</span><span class="group-creator"></span></div>'											
									    + '</div>'     
									    + '<div class="group-avatar"></div>'									       
									+ '</div>');
	        	var editGroup = groupWrapper.find('.edit-group');
	        	
	        	if(group.groupType == uc.constants.GroupType.Project && group.adminId == userInfo.userId 
	        			|| group.groupType == uc.constants.GroupType.Group){
	        		editGroup.show().css("display", "inline-block");
	        	}
	        	
	        	groupWrapper.find('.group-creator').addClass("group-creator-" + group.adminId);	  
	        	var groupNameElement = groupWrapper.find('.group-name');
	        	_self.setGroupName(groupNameElement, group.name);
	        	
	        	groupWrapper.find('.group-name-input').val(group.name);
	        	groupWrapper.find('.group-name-edit').attr("groupId", groupId);
	        	var membersCount = group.members.length;
	        	groupWrapper.find('.group-count').text(membersCount);	  

	        	var groupAvatar;
	        	if(group.groupType == uc.constants.GroupType.Project){
	        		var defaultAvatar = uc.constants.UI.DefaultLocalUrl.Project;
	        		
	        		groupAvatar = $('<div class="project-group-avatar">'
	        						+ ' 	<img src="' + defaultAvatar + '" onerror="uc.util.ImageUtil.OnImageLoadError(this,\'' + defaultAvatar + '\')" class="project-avatar round-image"/>'	        						
	        						+ '</div>')
	        		groupAvatar.find('.project-avatar').addClass("project-avatar-" + groupId);
	        		groupWrapper.find('.group-avatar').replaceWith(groupAvatar);	        		
	        		
	        		if(group.adminId == userInfo.userId){
		        		var projectAvatarEdit = $('<div class="rounded-corners project-avatar-edit">'	        									
	        									+ '</div>');
						groupAvatar.append(projectAvatarEdit);

		        	}	        		
		        	_self.displayProjectAvatar(group, groupAvatar.find('.project-avatar'));
			        											
	        	}else{		        	
	        		var twoMembers = _self.getGroupTwoMembers(groupId, group);
	        		var groupAdminId = twoMembers[0];
	        		var groupMemberId = twoMembers[1];  
	        		
	        		groupAvatar = uc.util.AvatarUtil.composeGroupAvatar(groupAvatarUrlArray, uc.constants.UI.DefaultLocalUrl.Avatar);
	        		var groupAvatarElement = $(groupAvatar);
		        	groupAvatarElement.find('img').addClass("avatar");		        	
		        	groupAvatarElement.find('.avatar-admin').addClass("avatar-" + groupAdminId);
		        	groupAvatarElement.find('.avatar-attendee').addClass("avatar-" + groupMemberId);
		        	groupWrapper.find('.group-avatar').replaceWith(groupAvatarElement);
	        	}
				
	        	groupDetailUp.append(groupWrapper);	  
	        	
	        	var actionsBar = $('<div class="actions-bar"></div>');
	        	groupDetailUp.append(actionsBar);
	        	actionsBar.attr("groupId", groupId);
	        	actionsBar.attr("groupType", group.groupType);

		        var sendMessageFigure = $('<div class="send-content"><figure class="send-message">'
			        + '<figcaption class="send-message-figcaption"></figcaption>'
			        + '<figcaption>' + uc_resource.Profile.SendMessage + '</figcaption>'
			        + '</figure></div>');
		        actionsBar.append(sendMessageFigure);

		        //呼
		        var callFigure = $('<figure class="view-Groupcall">'
			        + '<figcaption class="view-GroupCall-figcaption" qtips="'+ uc_resource.Menu.Call +'"></figcaption>'
			        + '</figure>');
		        actionsBar.append(callFigure);

				var _class = groupInfo.memberType ?  'view-calendar-figcaption hide' : 'view-calendar-figcaption';
	        	var calendarFigure = $('<figure class="view-calendar">'
						+ '<figcaption class="'+ _class +'"></figcaption>'
						+ '</figure>');

	        	actionsBar.append(calendarFigure);
	        	
	        	groupDetailUp.append(actionsBar);
				actionsBar.find(".view-calendar-figcaption").attr({"qtips":uc_resource.Meeting.CreateCalendar});
	        	var splitter = $('<div class="group-up-splitter"></div>');
	        	groupDetailUp.append(splitter);
	        	
	        	var switchActionsBar = $('<div class="switch-actions-bar">'
	        							+'	<span class="switch-action switch-to-members">' + uc_resource.Profile.Members + '</span><span class="switch-action switch-to-settings">' + uc_resource.Profile.Setting + '</span>'
	        						    +'</div>');
	        	groupDetailUp.append(switchActionsBar);	  	        	
	        	
	        	var membersSelected = '<div class="switch-to-selected switch-to-members-selected"></div>';
	        	groupDetailUp.append(membersSelected);
	        	
	        	var settingsSelected = '<div class="switch-to-selected switch-to-settings-selected"></div>';
	        	groupDetailUp.append(settingsSelected);
	        	
	        	var membersPanel = $('<div class="group-members-panel"></div>');
	        	var addMembers = $('<div class="add-members">'
	        					+ '		<div class="left"> '
	        					+ '			<div class="add-member-icon">'
								+ '				<img src="../images/contacts/add_member.png">'
								+ '			</div>'
								+ '		</div>'
								+ '		<div class="right">'							
								+ '		</div>'
								+ '		<div class="center add-member-text">' + uc_resource.Profile.AddNewMember + '</div>'
								+ '	</div>');
	        		membersPanel.append(addMembers);
	        	groupDetailDownWrapper.append(membersPanel);
	        	
	        	var contactList = $('<ul class="contact-list"></ul>');
	        	membersPanel.append(contactList);
	        	
	        	var members = group.members; //_self.getGroupMembers(groupId, group.groupType);
	        	_self.loadGroupMembersMainUI(contactList, members, group.adminId);	        	
	        	
	        	var settingPanel = $('<div class="group-settings"></div>');
	        	var chatSettings = $('<div class="chat-settings">'
        							+ ' <div class="chat-settings-title">' + uc_resource.Profile.ChatSetting + '</div>'
        							+ '	<ul class="chat-setting-list">'
        							+ '		<li class="chat-setting-item chat-setting-message-disturb">'
        							+ ' 		<div class="chat-setting-message-disturb-status"></div><div class="chat-setting-message-disturb-title">'+uc_resource.Profile.NoDisturbing+'</div>'
        							+ ' 	</li>'
        							+ '	</ul>'
        							+ '</div> ');
	        	var disturbStatus = chatSettings.find('.chat-setting-message-disturb-status');
	        	disturbStatus.attr("groupId", groupId);
	        	
	        	_self.messageDisturbSwitcher = new uc.modules.component.SwitchButton({
                    container: disturbStatus,
                    onChanged: function (value) {
                        var groupDetail = $('.group-detail:visible');
                        //var groupId = Number(groupDetail.attr("groupId"));
                        var newStatus = value ? 1 : 0;
                        //Set status to group
                        uc.IGroupService.setDoNotDisturb(groupInfo.groupId, newStatus, function(result) {
                        });
                    }
                });
	        	_self.messageDisturbSwitcher.setValue(group.disturbState == 1);
	        	
	        	settingPanel.append(chatSettings);
	        	
	        	var buttonPanel = $('<div class="button-panel">'
	        					+ ' 	<button class="btn btn-exit-group">' + uc_resource.Profile.ExitGroup + '</button>'
	        					+ '	</div>');
				// start------------------------modified by xue.bai_2 on 2016/04/15----------------------------
				var button = buttonPanel.find('.btn-exit-group');
				if (group.groupType == uc.constants.GroupType.Project) {
					button.text(uc_resource.Profile.ExitProject);
				}

				if (group.adminId == userInfo.userId) {
					button.addClass("btn-end-group").removeClass("btn-exit-group");
					button.text(uc_resource.Profile.EndProject);
					if(group.groupType == uc.constants.GroupType.Group){
						button.text(uc_resource.Profile.EndGroup);
					}
				}
				// ------------------------modified by xue.bai_2 on 2016/04/15----------------------------end

	        	settingPanel.append(buttonPanel);
	        	
	        	groupDetailDownWrapper.append(settingPanel);
	        	
	        	template.attr("groupId", groupId);
	        	contentPanel.append(template);
	        	contentPanel.fadeIn('slow');
	        	
	        	var contactsResult = _self.getGroupCachedAndUnCachedMembers(members);
	        	
	        	var cachedContacts = contactsResult.cachedContacts;
	        	var unCachedMembers = contactsResult.unCachedMembers;
	        	
	        	//Fetch Contacts from cache
	        	_self.loadGroupMembersUI(cachedContacts);
	        	//Fetch Contacts from backend, process in loadGroupMembersUI when receive OnContactInfoReceived event
	        	uc.modules.contact.ContactManager.batchRequestMembers(unCachedMembers);

				//add by xiege.
				if (uc.constants.Switch.OpenMultiVersion) {
					var version = JSON.parse(uc.util.StorageUtil.getData("capability"));
					if (version) {
						switch (version.im) {
							case "0":
								$(".send-message").addClass("hide");
								break;
						}
						switch (version.calendar) {
							case "0":
								calendarFigure.find(".view-calendar-figcaption").addClass("hide");
								break;
						}
						switch (version.call) {
							case "0":
								$(".send-call").addClass("hide");
								$(".view-Groupcall").addClass("hide");
						}
						switch (version.app_moments) {
							case "0":
								uc.ui.Contacts.ContentPanelBody.find(".attention").addClass("hide");
								break;
						}
					}
				}

	        	_self.bindEvent();
	        },	
	        setGroupName: function(groupNameElement, newGroupName) {
                var text = uc.util.StringUtil.escapeHtml(newGroupName);
	        	groupNameElement.html(text);
	        	groupNameElement.attr("qtips", text);
	        },
	        getGroupMembers: function(groupId, groupType) {
	        	var _self = this;
	        	var group = _self.findCachedGroupByGroupId(groupId);	        	
	        	var members = [];
	        	if(group != null){
	        		members = group.members;
	        	}
	        	//member:{joinTime: long, userId: long}
	        	return members;
	        },
	        getGroupCachedAndUnCachedMembers: function(members) {
	        	var cachedContacts = [];
	        	var unCachedMembers = [];
	        	$.each(members,function(key, member){ 
	        		var userId = member.userId;
	        		var cachedContact = uc.modules.contact.ContactManager.getCachedContact(userId);
	        		if(cachedContact != null){
	        			cachedContacts.push(cachedContact);
	        		} else {
	        			//Need fetch contact from server
	        			unCachedMembers.push(userId);
	        		}
	        	});
	        	var result = {};
	        	result.cachedContacts = cachedContacts;
	        	result.unCachedMembers = unCachedMembers;
	        	return result;
	        },
	        loadGroupMembersMainUI: function(contactList, contacts, adminId) {	        	
        		if(contacts.length == 0){
        			return;
        		}
	        	var sortedContacts = [];
	        	
	        	//Sort Group Members UI by Creator, Current User, others.
	        	var adminInfo = uc.util.CollectionUtil.findObjectByKey(contacts, "userId", adminId);
	        	if(adminInfo != null){
	        		sortedContacts.push(adminInfo);
	        	}
	        	
	        	var currentUser = uc.IContactService.getCurrentUserInfo();
        		var currentUserId = currentUser.userId;
        		if(currentUserId != adminId) {
        			var currentUserInfo = uc.util.CollectionUtil.findObjectByKey(contacts, "userId", currentUserId);
    	        	if(currentUserInfo != null){
    	        		sortedContacts.push(currentUserInfo);
    	        	}
        		}
        		
        		for(var i = 0; i < contacts.length; i++){
        			var contact = contacts[i];
        			if(contact == null) {
        				continue;
        			}
        			var userId = contact.userId;
        			if(userId == adminId || userId == currentUserId) {
        				continue;
        			}
        			sortedContacts.push(contact);
        		}        		
	        	$.each(sortedContacts,function(key, item){ 	        		
	        		if(contactList.find(".contact-item-" + item.userId).length > 0){
	        			return true;
	        		}
	        		var template = uc.modules.contact.ContactManager._getContactTemplate();
	        		//If admin, hide remove icon by css
	        		if(adminId && item.userId == adminId){
	        			template.addClass("contact-item-admin");
	        			//Add an icon for Creator
	        			var creatorIcon = '<div class="group-creator-icon">'
	        							+ '	<img src="../images/contacts/group_creator.png">'
	        							+ '</div>'		
	        			template.find('.left').append(creatorIcon);
	        		}
            		template.addClass("contact-item-" + item.userId);
            		template.attr("userId", item.userId);            		
            		contactList.append(template);
	        	});	        	
	        },
	        loadGroupMembersUI: function(contacts) {
	        	var contactList = $('.group-detail .contact-list');	
	        	if(contactList.length == 0){
	        		return;
	        	}	        	
	        	
	        	$.each(contacts,function(key, item){ 
	        		var contactElements = contactList.find(".contact-item-" + item.userId);
	        		$.each(contactElements,function(index, contactElement){
	        			var template = $(contactElement);
	            		template.find('.contact-display-name').text(item.displayName);
	            		template.find('.contact-display-name').attr("userId", item.userId);
	            		template.find('.contact-department').text(item.departmentName);
	            		template.find('.contact-position').text(item.position);
	            		if(!item.departmentName){
	            			template.find('.contact-department').hide();
	            		}
	                	
	            		var avatar = template.find('.left .avatar');
	                	avatar.addClass("avatar-" + item.userId);
	                	if(item.localAvatar){
	                		avatar.attr("src", item.localAvatar);
	                		avatar.addClass("avatar-loaded");
	            		} else {
	            			avatar.addClass("avatar-unloaded");
	            			if(item.remoteAvatarUrl){
	            			}
	            		} 
	        		})	        		           		
	        	});	        	
	        },	        
	        bindEvent: function() {
	        	var _self = this;
				uc.ui.Contacts.MainPanel.find('.content-panel').off();
	        	$('.content-panel').on("click", ".group-detail .group-name-container .edit-group", function() {
	        		var groupNameContainer = $(this).parent('.group-name-container');
	        		var parent = groupNameContainer.parent('.left-content');
	        		var groupNameEdit = parent.find('.group-name-edit');
	        		
	        		var groupName = groupNameContainer.find('.group-name').text();
	        		var input = groupNameEdit.find('.group-name-input');
	        		input.val(groupName);
	        		
	        		groupNameContainer.hide();
	        		groupNameEdit.show();
	        		
	        		input.focus();
	        	});
	        	
	        	$('.content-panel').on("blur", ".group-detail .group-name-input", function() {
	        		_self.editGroupName();
	        	});
	        	
	        	$('.content-panel').on("keydown", ".group-detail .group-name-input", function(e) {        		
	        		 var key = e.which;	        		 
	                 if (key == 13) {
	                     e.preventDefault();
	                     _self.editGroupName();
	                 }        		       			        		
	        	});
	        	
	        	$('.content-panel').on("click", ".group-detail .project-avatar-edit", function() {
	        		uc.IUIService.openImageCropperWindow("ChangeGroupAvatar", function (args) {	        			
	                    if (args.okOrCancel) {
	                    	var croppedImagePath = args.croppedImagePath;
	                    	
	                    	var groupDetail = $('.group-detail');
	    	        		var groupId = Number(groupDetail.attr("groupId"));
	    	        		
	                    	_self.uploadProjectAvatar(groupId, croppedImagePath);
	                    }
	        		});	        		
	        	});
	        	
	        	$('.content-panel').on("click", ".group-detail .switch-actions-bar .switch-to-members", function() {	        		
	        		var membersPanel = $('.group-detail .group-members-panel');
	        		var groupSettings = $(".group-detail .group-settings");	        		
	        	
	        		if(membersPanel.is(":visible")){
	        			return;
	        		}
	        
	        		$('.switch-to-members-selected').show();
	        		$('.switch-to-settings-selected').hide();
	        		
	        		groupSettings.hide();
	        		membersPanel.show();
	        	});
	        	
	        	$('.content-panel').on("click", ".group-detail .switch-actions-bar .switch-to-settings", function() {	        		
	        		var membersPanel = $('.group-detail .group-members-panel');
	        		var groupSettings = $(".group-detail .group-settings");
	        		
	        		if(groupSettings.is(":visible")){
	        			return;
	        		}	        		
	        		
	        		$('.switch-to-members-selected').hide();
	        		$('.switch-to-settings-selected').show();
	        		
	        		membersPanel.hide();
	        		groupSettings.show();
	        	});
	        	
	        	$('.content-panel').on("click", ".btn-exit-group", function() {
	        		var button = $(this);
	        		var groupDetail = button.parents('.group-detail');
	        		var groupId = Number(groupDetail.attr("groupId"));
			        var message;
			        //在此处判断当前玩家是否在GroupCall中，并且比较GroupCall的groupId，如果相同，则弹出在呼中提示，否则弹出正常退出群聊提示
			        var InProgressCallInfo = uc.modules.call.CallManager.inProgressCallInfo;
			        if(InProgressCallInfo.isInProgress){
				        var groupInfo = uc.IGroupService.getGroupInfo(InProgressCallInfo.chatId);
				        if(groupInfo.id == groupId){
					        message = uc_resource.Profile.ConfirmExitIfInGroupCall;
				        }else{
					        message = uc_resource.Profile.ConfirmExit;
				        }
			        }
			        else{
				        message = uc_resource.Profile.ConfirmExit;
			        }
	        		uc.modules.component.PromptMessageBox.open({title: uc_resource.Profile.Prompt, message: message ,buttonMode:"both", OkText: uc_resource.Common.OK, CancelText: uc_resource.Common.Cancel}, function(result){
	                    if(result == 0) {
	        				uc.IGroupService.exitGroup( 
	        						groupId,	        						
	        					    function(args){
	        							var errorCode = args[0];
	        	        		        if(errorCode == 0){
	        	        		        	_self.onCurrentUserExitGroup(groupId);
	        	        		        	
	        	        		        	var contentPanel = button.parents('.content-panel-body');
        	        		        		if(contentPanel.length > 0){
        	        		        			contentPanel.empty();
        	        		        		}
	        	        		        }	        	        		        
	        						}
	        				);
	        			}
	        			this.close();         		
	                });
	        		
	        	});	
	        	
	        	$('.content-panel').on("click", ".btn-end-group", function() {
	        		var button = $(this);
	        		var groupDetail = button.parents('.group-detail');
	        		var groupId = Number(groupDetail.attr("groupId"));
			        var message;
			        //在此处判断当前玩家是否在GroupCall中，并且比较GroupCall的groupId，如果相同，则弹出在呼中提示，否则弹出正常退出群聊提示
			        var InProgressCallInfo = uc.modules.call.CallManager.inProgressCallInfo;
			        if(InProgressCallInfo.isInProgress){
				        var groupInfo = uc.IGroupService.getGroupInfo(InProgressCallInfo.chatId);
				        if(groupInfo.id == groupId){
					        message = uc_resource.Profile.ConfirmExitIfInGroupCall;
				        }else{
					        message = uc_resource.Profile.ConfirmEnd;
				        }
			        }
			        else{
				        message = uc_resource.Profile.ConfirmEnd;
			        }
	        		uc.modules.component.PromptMessageBox.open({title: uc_resource.Profile.Prompt, message: message ,buttonMode:"both", OkText: uc_resource.Common.OK, CancelText: uc_resource.Common.Cancel}, function(result){
	                    if(result == 0) {
	        				uc.IGroupService.endGroup( 
	        						groupId,	        						
	        					    function(args){
	        							var errorCode = args[0];
	        	        		        if(errorCode == 0){
	        	        		        	_self.onCurrentUserExitGroup(groupId);
	        	        		        	
	        	        		        	var contentPanel = button.parents('.content-panel-body');
        	        		        		if(contentPanel.length > 0){
        	        		        			contentPanel.empty();
        	        		        		}
	        	        		        }	        	        		        
	        						}
	        				);
	        			}
	        			this.close();         		
	                });
	        		
	        	});	
	        	
	        	$('.content-panel').on("click", ".group-detail .add-members .add-member-icon, .group-detail .add-members .add-member-text", function() {
	        		var userIdArray = [];
					var groupId = Number($('.content-panel').find(".group-detail").attr("groupId"));
					var group = uc.IGroupService.getGroupInfo(groupId);
                    var groupCallId='';
		        	if(group != null){
                        groupCallId=(group.statusInfo !="" && group.statusInfo !="[]")? $.parseJSON(group.statusInfo).tempConferenceId:'';
		        		var members = group.members;
		        		$.each(members,function(key, member){
			        		var userId = member.userId;
			        		userIdArray.push(parseInt(userId));
			        	});
		        	}

                    if(groupCallId){
                        if(members.length >= 9){
                            uc.modules.component.PromptMessageBox.open({
                                title: uc_resource.WindowTitle.Notification,
                                message: uc_resource.Call.GroupAddMemberFailed,
                                buttonMode: "ok",
                                OkText: uc_resource.Common.OK
                            }, function () {
                                this.close();
                            });
                            return;
                        }
                        uc.modules.component.PromptMessageBox.open({
                            title: uc_resource.WindowTitle.Notification,
                            message: uc_resource.Call.GroupHaveCall,
                            buttonMode: "both",
                            OkText: uc_resource.Common.OK,
                            CancelText: uc_resource.Common.Cancel
                        }, function (result) {
                            if(!result){
                                uc.IUIService.openChooseCallContactsWindow({
                                    key:"chooseCallContacts_" + new Date().getTime(),
                                    userList:userIdArray
                                }, function (args) {
                                    var groupDetail = $('.group-detail');
                                    var groupId = Number(groupDetail.attr("groupId"));
                                    var groupType = Number(groupDetail.attr("groupType"));
                                    var userStatus=[];
                                    var callUser=[];
                                    for(var i = 0 ;i < args.memberList.length; i++){
                                        userStatus.push(args.memberList[i].userId);
                                        for (var j = 0; j < userIdArray.length; j++){
                                            var flag=false;
                                            if(args.memberList[i].userId == userIdArray[j]){
                                                flag=true;
                                                break;
                                            }

                                        }
                                        if(!flag){
                                            callUser.push(args.memberList[i])
                                        }
                                    }
                                    if(!callUser.length) {
                                        uc.IUIService.triggerCrossWindowEvent(uc.constants.UI.CrossWindowEventName.chooseContactsCreateCall, {callStatus: uc.constants.UI.chooseCallContacts.createcall_fail,failedType:uc.constants.Call.CallFailedType.SelectedUser});
                                        return;
                                    }
                                    uc.IUIService.triggerCrossWindowEvent(uc.constants.UI.CrossWindowEventName.AddGroupMemberToCall, {userList:callUser,groupId:groupId,chatType:groupType,meetingId:groupCallId});
                                });

                            }
                            this.close();
                        });

                    }else{
                        uc.IUIService.openChooseContactWindow({
                            key:"AddGroupMembers", selectedUserIdList: userIdArray
                        }, function (args) {
                            if (args.okOrCancel) {
                                var contacts = args.contacts;

                                var groupDetail = $('.group-detail');
                                var groupId = Number(groupDetail.attr("groupId"));
                                var groupType = Number(groupDetail.attr("groupType"));

                                 _self.addGroupMembers(groupId, groupType, contacts);
                                //Will rerender UI when success
                            }
                        });
                    }
		        	

	        	});
	        	
	        	//When click on colleagues, popup contact info.
				var clickContactTimer = -1;
	        	$('.content-panel').on("click", ".group-detail .contact-item", function(){
					clearTimeout(clickContactTimer);
					var userId = Number($(this).attr("userid"));
					clickContactTimer = setTimeout(function(){
						uc.IUIService.showPopupContactProfile(userId);
					}, 300);
				});

				var currentUserInfo = uc.IContactService.getCurrentUserInfo();
				$('.content-panel').on("dblclick", ".group-detail .contact-item", function(){
					clearTimeout(clickContactTimer);
					var userId = Number($(this).attr("userid"));
					if(currentUserInfo.userId != userId){
						uc.IUIService.openChatWindow(uc.constants.Chat.ChatType.SingleChat, userId, uc.constants.UI.OpenChatWindowSourceModule.Other, null, null, true);
					}
				});
	        	
	        	$('.content-panel').on("click", ".group-detail .contact-item .remove-contact", function(e) {	        		
	        		var contactItem = $(this).parents('.contact-item');
	        		var groupDetail = $('.group-detail');
    	        	var groupId = Number(groupDetail.attr("groupId"));
    	        	
	        		var userId = Number(contactItem.attr("userId"));
	        		var userName = contactItem.find('.contact-display-name').text();
	        		var message = uc.util.StringUtil.formatString(uc_resource.Profile.RemoveMember, [userName]);
	        		
    	        	_self.removeGroupMember(groupId, userId, message, contactItem);    	        	
    	        	e.stopPropagation();
	        	});
	        },
            bindCallUserStatus:function(){
                uc.IUIService.bindCrossWindowEvent(uc.constants.UI.CrossWindowEventName.chooseContactsCreateCall, function (_eventData) {
                    if (_eventData) {
                        if (_eventData.callStatus == uc.constants.UI.chooseCallContacts.createcall_fail) {
                            if(_eventData.failedType){
                                switch(_eventData.failedType){
                                    case uc.constants.Call.CallFailedType.CreateFailed:
                                        uc.modules.component.PromptMessageBox.open({
                                            title: uc_resource.WindowTitle.Notification,
                                            message:uc_resource.Call.CreateCallFailed,
                                            buttonMode: "ok",
                                            OkText: uc_resource.Common.OK
                                        }, function () {
                                            this.close();
                                        });
                                        break;
                                    case uc.constants.Call.CallFailedType.InvitedCall:
                                        uc.modules.component.PromptMessageBox.open({
                                            title: uc_resource.WindowTitle.Notification,
                                            message:uc_resource.Call.InvitedUserFailed,
                                            buttonMode: "ok",
                                            OkText: uc_resource.Common.OK
                                        }, function () {
                                            this.close();
                                        });
                                        break;
                                    case uc.constants.Call.CallFailedType.BusyUserStatus:
                                        uc.modules.component.PromptMessageBox.open({
                                            title: uc_resource.WindowTitle.Notification,
                                            message: _eventData.message,
                                            buttonMode: "ok",
                                            OkText: uc_resource.Common.OK
                                        }, function () {

                                            this.close();
                                        });
                                        break;
                                    case uc.constants.Call.CallFailedType.GetUserStatus:
                                        uc.modules.component.PromptMessageBox.open({
                                            title: uc_resource.WindowTitle.Notification,
                                            message:uc_resource.Call.GetUserCallStatusFailed,
                                            buttonMode: "ok",
                                            OkText: uc_resource.Common.OK
                                        }, function () {
                                            this.close();
                                        });
                                        break;
                                }
                            }
                        }
                    }
                });
            },
            invitedUserCallFailed:function(){
                uc.modules.component.PromptMessageBox.open({
                    title: uc_resource.WindowTitle.Notification,
                    message: uc_resource.Call.ChooseMaxContactsTips,
                    buttonMode: "ok",
                    OkText: uc_resource.Common.OK
                }, function () {
                    this.close();
                });
            },
	        onGroupDisturbSet: function(errorCode, groupId, newDisturbState) {
				if(errorCode!=0){
					uc.util.LogUtil.info("GroupManager","onGroupDisturbSet","set disturb failed,errorCode="+errorCode+',groupId='+groupId);
					uc.modules.component.PromptMessageBox.open({
						title: uc_resource.WindowTitle.Notification,
						message: uc_resource.Message.DisturbSetFailed,
						buttonMode: "ok",
						OkText: uc_resource.Common.OK
					}, function (result) {
						this.close();
					});

				}
	        	var _self = this;        	

        		//Update disturbState back to cache
        		var cachedGroup = _self.findCachedGroupByGroupId(groupId);
        		if(cachedGroup && cachedGroup.disturbState != newDisturbState) {
        			cachedGroup.disturbState = newDisturbState;
        			var groupType = cachedGroup.groupType;
        			if((groupType == uc.constants.GroupType.Project)||(groupType == uc.constants.GroupType.Group)){
    	        		//Project
    		        	_self.addOrUpdateProjectGroupToCache(groupId, cachedGroup);
    	        	} else {
    	        		//Chat
    	        		_self.addOrUpdateChatGroupToCache(groupId, cachedGroup);
    	        	}        			
        		}
        		
	        	var groupDetail = $('.group-detail-' + groupId);
	        	if(groupDetail.length == 0){
	        		return;
	        	}	
        		if(_self.messageDisturbSwitcher){
					var orginal = _self.messageDisturbSwitcher.getValue();
					var now = newDisturbState == 1;
					if(orginal != now) {
						_self.messageDisturbSwitcher.setValue(now);
					}
				}
	        },
			noticeGroupDisturbSetMessageReceived:function(conversationType, channel, from, to, groupId, state, msgId, msgSeq, msgTime){
				if (this == uc.modules.group.GroupManager) {
					return;
				}
				var _self = this;
				var groupInfo = uc.IGroupService.getGroupInfo(groupId);
				var groupDetail = $('.group-detail:visible');
				var visibleGroupId = Number(groupDetail.attr("groupId"));
				if(visibleGroupId == groupId){
					_self.messageDisturbSwitcher.switchButton(groupInfo.disturbState == 1);
				}
			},
	        editGroupName: function() {
	        	var contentPanel = $('.content-panel');
	        	var input = contentPanel.find('.group-detail .group-name-input');
	        	var _self = this;
	        	var groupNameNew = input.val();
        		var groupNameEdit = input.parent('.group-name-edit');
        		var groupId = Number(groupNameEdit.attr("groupId"));
        		var parent = groupNameEdit.parent('.left-content');
        		
        		var groupNameContainer = parent.find('.group-name-container');
        		var groupNameElement = groupNameContainer.find('.group-name');
        		var groupNameBefore = groupNameElement.text();	        	
	        	_self.setGroupName(groupNameElement, groupNameNew);
        		
        		groupNameEdit.hide();
        		groupNameContainer.show();
        		//Call backend to save group name.
        		if(groupNameNew != groupNameBefore) {
        			//Will change Group list UI when receive OnGroupNameUpdated event,uc.modules.group.GroupManager.noticeGroupNameUpdated
        			uc.IGroupService.setGroupName( 
        					groupId,
        					groupNameNew,
	        			    function(args){
	        			        var errorCode = args[0];
	        			        uc.util.LogUtil.info('GroupManager', 'IGroupService.setGroupName', 'Exception:' + errorCode, null);   
	        			    }
	        			);
        		}
	        },
	        noticeGroupNameUpdated: function(groupId, groupName, pinyin){
	        	var _self = this;
	        	var cachedGroup = _self.findCachedGroupByGroupId(groupId);	        	
		        if(cachedGroup) {
		        	cachedGroup.name = groupName;
		        	cachedGroup.pinyin = pinyin;
		        	_self.removeGroupItemFromUI(groupId);
		        	_self.showNewGroupItemUI(cachedGroup);
		        	
		        	var groupDetails = $('.group-detail-' + groupId);
		        	if(groupDetails.length > 0){
		        		for(var i = 0; i < groupDetails.length; i++){
		        			var groupDetail = $(groupDetails[i]);
		        			var groupNameElement = groupDetail.find('.group-name');
		        			_self.setGroupName(groupNameElement, groupName);
		        		}
		        	}
		        }		        
	        },
	        removeGroupItemFromUI: function(groupId){
	        	var groupItem = $('.group-item-' + groupId);
        		if(groupItem.length > 0){ 
        			//If group is the only by first word
        			var previousSibling = groupItem.prev();
        			var nextSibling = groupItem.next();        			
        			if(previousSibling.hasClass("alphabetical-key-item")
        				&& (nextSibling.hasClass("alphabetical-key-item") 
        						|| !nextSibling.hasClass("alphabetical-key-item") && !nextSibling.hasClass("group-item"))
        						) {
        				previousSibling.remove();
        			}
        			groupItem.remove();
        		}
	        },
	        getProjectKey: function (taskId) {
	            return "project_" + taskId;
	        },
	        uploadProjectAvatar: function(groupId, filePath) {
	        	var _self = this;	        	
	        	uc.IFSService.uploadFile(filePath, function (args) {	        		
	                var errorCode = args[0], taskId = args[1];
	                if (errorCode === 0) {
	                	uploadTaskMap.put(_self.getProjectKey(taskId),
	                            {
	                                groupId: groupId,	                                
	                                localPath: filePath
	                            });	                	
	                	_self.refreshProjectGroupAvatar(groupId, filePath);
	                	uc.IGroupService.updateGroupAvatar(groupId, filePath, function(){});
	                }	                
	            });
	        },
	        noticeProjectAvatarUploaded: function (taskId, remoteImageUrl) {	        	
	        	var _self = this;
	            var key = _self.getProjectKey(taskId);
	            var temp = uploadTaskMap.get(key);	            
	            if (temp == null) {
	                return;
	            }
	            var groupId = temp.groupId, localPath = temp.filePath;

	            uploadTaskMap.remove(key);

	            //Call interface to set Project Logo
            	uc.IGroupService.setGroupLogo(groupId, remoteImageUrl, function(result){            		
            		var errorCode = result[0];
            		if(errorCode == 0){            			
            			//Update remoteAvatarUrl to Cached Group
        	        	var cachedGroup = _self.findCachedGroupByGroupId(groupId);
        	        	if(cachedGroup){
        	        		cachedGroup.avatarIndex = remoteImageUrl;
        	        		_self.addOrUpdateProjectGroupToCache(groupId, cachedGroup);	        		
        	        	}
            		}            		
            	});
	        },
	        removeGroupMember: function(groupId, userId, message, contactItem) {
	        	var userIdArray = [];
	        	userIdArray.push(userId);	        	
	        	
        		uc.modules.component.PromptMessageBox.open({title: uc_resource.Profile.Prompt, message: message ,buttonMode:"both", OkText: uc_resource.Common.OK, CancelText: uc_resource.Common.Cancel}, function(result){
                    if(result == 0) {
        				uc.IGroupService.removeMember( 
        						groupId,
        						userIdArray,
        					    function(args){
        							var errorCode = args[0];              	        		        
        	        		        if(errorCode == 0){
        	        		        	contactItem.remove();	        	        		        	
        	        		        }	        	        		        
        						}
        				);
        			}
        			this.close();         		
                });
	        },
	        addGroupMembers: function(groupId, groupType, contacts) {
	        	var _self = this;
	        	
	        	//Cache Contacts:
	        	uc.modules.contact.ContactManager.addOrUpdateContactsToCache(contacts);	        	

	        	var group = _self.findCachedGroupByGroupId(groupId);
	        	var originalMembers = group.members;
	        	var members = [];
	        	$.each(contacts,function(key, contact){ 	
	        		//If contact not exist in original group members, then add
	        		if(uc.util.CollectionUtil.findObjectByKey(originalMembers, "userId", contact.userId) == null){
	        			var member = {};
	        			member.userId = contact.userId;
	        			member.username = contact.pinyin;
	        			members.push(member);	        			
	        		}
	        	});	        	
	        		        	
	        	if(members.length == 0){
	        		return;
	        	}	        
	        	
	        	//Call backend to save to db
	        	uc.IGroupService.addMember( 
	        			groupId, members,        
	        		    function(args){                 
	        		        var errorCode = args[0];
	        		        if(errorCode == 0){
	        		        	//1. Update Group detail page
	        		        	var contactList = $('.group-detail .contact-list');	        	        	
		        	        	_self.loadGroupMembersMainUI(contactList, contacts, group.adminId);	
		        	        	_self.loadGroupMembersUI(contacts);
	        		        }
	        		    }
        		);
	        },
            popupMessage: function(message){
                uc.modules.component.PromptMessageBox.open({title: uc_resource.Common.Prompt, message: message ,buttonMode:"ok",OkText:uc_resource.Common.OK}, function(result){
                    this.close();
                });
            },
	        onGroupMembersAdded: function(groupId, memberIds,errorCode){
	        	var _self = this;	        
                var errorMsg;
                switch (errorCode) {
                    case 10449:
                        errorMsg = uc_resource.GroupChat.GetGroupListFailed;
                        break;
                    case 10362:
                        errorMsg = uc_resource.GroupChat.MemberCountLimited;
                        break;
                    case 10363:
                        errorMsg = uc_resource.GroupChat.GroupCountLimited;
                        break;
                    case 10369:
                        errorMsg = uc_resource.GroupChat.GroupCreateLimited;
                        break;
                }

                if (errorMsg) {
                    _self.popupMessage(errorMsg);
                    return;
                }

                if (uc.modules.profile.GroupProfile && groupId != uc.modules.profile.GroupProfile.groupId) {
                    return;
                }
	        	
	        	//When receive OnGroupMemberAdded Event: memberIds [id1, id2..]
	        	//When receive OnGroupMemberAddedMessageReceived Event, memberIds [{id: id1, joinTime: 11111}, {id: id2, joinTime: 222222}]
	        	//Process Group Members Cache.
	        	var group = _self.findCachedGroupByGroupId(groupId);	        	
	        	if(group){	        		
	        		var originalMembers = group.members;
	        		
	        		var newMembers = [];	   
	        		var currentTime = new Date().getTime();
	        		$.each(memberIds,function(key, member){
	        			var memberId = member.userId ? member.userId: member;
		        		//If contact not exist in original group members, then add to cache
		        		if(uc.util.CollectionUtil.findObjectByKey(originalMembers, "userId", memberId) == null){
		        			var newMember = {};
		        			newMember.userId = memberId;
		        			newMember.joinTime = member.joinTime ?  member.joinTime : currentTime;
		        			newMembers.push(newMember);	        			
		        		}
		        	});
	        		
	        		group.members = $.merge(originalMembers, newMembers);	        		
	        		_self.addOrUpdateGroupToCache(groupId, group);
	        		
	        		var contactList = $('.group-detail .contact-list');	      
	        		var cachedContacts = [];
	        		var toRequestContactIdList = [];
	        		for(var i = 0; i < newMembers.length; i++){
	        			var member = newMembers[i];
	        			var userId = member.userId;
	        			var cachedContact = uc.modules.contact.ContactManager.getCachedContact(userId);
	        			if(cachedContact){
	        				cachedContacts.push(cachedContact);
	        			} else {
	        				toRequestContactIdList.push(userId);
	        			}
	        		}
    	        	_self.loadGroupMembersMainUI(contactList, newMembers, group.adminId);
    	        	_self.loadGroupMembersUI(cachedContacts);    	        	
    	        	if(toRequestContactIdList.length > 0) {
    	        		uc.modules.contact.ContactManager.batchRequestMembers(toRequestContactIdList);
    	        	}
    	        	
    	        	setTimeout(_self.handleGroupMembersChanged(groupId, group.members), 500);
					
	        	}
	        },
	        onGroupMemberRemoved: function(groupId, memberIds){
				uc.util.LogUtil.info('GroupManager', 'onGroupMemberRemoved', 'params', {groupId: groupId, memberIds: memberIds});
	        	var _self = this;
	        	
	        	//memberIds: [{userId: 1296684}]
	        	//Process Group Members Cache.
	        	var group = _self.findCachedGroupByGroupId(groupId);
	        	if(!group){
	        		group = uc.IGroupService.getGroupInfo(groupId);
	        	}

				if (!group) {
					return;
				}
        		var members = group.members;
        		
        		var currentUser = uc.IContactService.getCurrentUserInfo();
        		var currentUserId = currentUser.userId;
        		
        		//1. If current User in memberIds
        		var currentUserMemberIds = memberIds.filter(function(member){
              		  return member.userId === currentUserId;
    			});
        		if(currentUserMemberIds.length > 0){
        			_self.onCurrentUserExitGroup(groupId);
        		}
        		
        		//2. Members exclude current User
        		var otherMemberIds = memberIds.filter(function(member){
              		  return member.userId !== currentUserId;
    			});
        		if (otherMemberIds.length > 0){
        			//Remove member from original members
	        		for(var i = 0; i < otherMemberIds.length; i++) {
	        			var member = otherMemberIds[i];			        			
	        			var memberId = member.userId;		        			
	        			
	        			members = members.filter(function(item){
	              		  return item.userId !== memberId;
	        			});		        			
	        			
	        			var contactItems = $('.group-detail .contact-item-' + memberId);		        			
	        			$.each(contactItems,function(index, item){
	        				var contactItem = $(item);
	        				contactItem.remove();
	        			});
		        	};
	        		
	        		group.members = members;
	        		
	        		_self.addOrUpdateGroupToCache(groupId, group);
	        		
	        		_self.handleGroupMembersChanged(groupId, group.members);
        		}
	        },
	        onCurrentUserExitGroup: function(groupId) {	 
	        	var _self = this;
	        	//Process cache
        		var group = _self.findCachedGroupByGroupId(groupId);
        		if(group){
        			if(group.groupType == uc.constants.GroupType.Project){
	        			_self.removeProjectGroup(groupId);
	        		} else {
	        			_self.removeChatGroup(groupId);
	        		}
        		}
	        	
        		_self.removeGroupItemFromUI(groupId);
	        	var groupDetails = $('.group-detail-' + groupId);
	        	if(groupDetails.length > 0){
	        		for(var i = 0; i < groupDetails.length; i++){
	        			var groupDetail = $(groupDetails[i]);
	        			var parent = groupDetail.parent();
                        if (parent.hasClass('content-panel-body')) {
                            if(group.groupType == uc.constants.GroupType.Project){
                                parent.html(uc.modules.contact.ContactManager.getInitPageHtml("panel-projects"));
                            } else {
                                parent.html(uc.modules.contact.ContactManager.getInitPageHtml("panel-groupChats"));
                            }
                        } else {
                            parent.empty();
                        }
	        		}
	        	}
	        },
	        findCachedGroupByGroupId: function(groupId) {
	        	var _self = this;
	        	var group = _self.getCachedProjectGroup(groupId);
	        	if(group != null){
	        		return group;
	        	}
	        	group = _self.getCachedChatGroup(groupId);
	        	return group;
	        },	        
	        handleGroupMembersChanged: function(groupId, members){
	        	var _self = this;
	        	var totalMembersCount = members.length;
	        	
	        	//2. Update Group list cache & UI
                if (uc.modules.profile.GroupProfile && groupId == uc.modules.profile.GroupProfile.groupId){
                    $('.group-detail .group-count').text(totalMembersCount);
                }

	        	var groupItems = $('.group-item-' + groupId);
	        	
	        	var group = _self.findCachedGroupByGroupId(groupId);
	        	
	        	var groupContacts = [];
        		var cachedContacts = uc.modules.contact.ContactManager.getCachedContacts();
        		for(var i = 0; i < members.length; i++){
        			var userId = members[i].userId;
        			var contact = uc.modules.contact.ContactManager.getCachedContact(userId);
        			if(contact){
        				groupContacts.push(contact);
        			}	        			
        		}
	        	
	        	$.each(groupItems,function(key, item){
	        		var groupItem = $(item);	        		
	        		groupItem.attr("membersCount", totalMembersCount);
	        		groupItem.find('.group-members-count').text(totalMembersCount);
	        		
	        		//Refresh group-description
	        		groupItem.find('.group-members').empty();
	        		var memberNames = _self.getGroupFirst6MemberNames(groupContacts);
	        		groupItem.find('.group-description').text(memberNames);
	        		
	        		//Clear Avatar
	        		var groupAvatar = groupItem.find('.group-avatar');
	        		groupAvatar.removeClass("avatar-loaded");	
	        		
	        		$('.group-detail .group-avatar').removeClass("avatar-loaded");
	        		
	        		//Refresh group avatar
	        		_self.refreshChatGroupAvatar(groupId, group);	        		
	        	});		        		        	
	        },
	        getGroupTwoMembers: function(groupId, group) {
	        	var twoMembers = [];
	        	var admin = group.members.filter(function(item){
	          		  return item.userId == group.adminId;
	          	});
	        	var otherMembers = group.members.filter(function(item){
	          		  return item.userId !== group.adminId;
	          	});
	        	
	        	if(admin.length > 0){
	        		// group.adminId exist
	        		twoMembers.push(group.adminId);
	        		if(otherMembers.length > 0){
	            		twoMembers.push(otherMembers[0].userId);
	            	}
	        	}else{
	        		// group.adminId not exist
	        		if(otherMembers.length >= 2){
	        			twoMembers.push(otherMembers[0].userId, otherMembers[1].userId);
	            	}else if(otherMembers.length == 1){
	            		twoMembers.push(otherMembers[0].userId);
	            	}
	        	}          	
            	return twoMembers;            	
	        },
	        getGroupFirst6MemberNames: function(groupContacts) {
	        	var memberNameArray = [];	        	
	        	
	        	var maxLength = groupContacts.length > 6 ? 6 : groupContacts.length;
	        	for(var i = 0; i < maxLength; i++){
	        		var contact = groupContacts[i];
	        		var memberName = contact.displayName;
        			if(memberName){
        				memberNameArray.push(memberName);
        			}
	        	}
	        	var memberNames = uc_resource.Profile.Include + memberNameArray.join(uc_resource.Profile.Comma);    		
        		return memberNames;
	        },	       
	        refreshProjectGroupAvatar: function(groupId, localAvatarUrl) {
	        	var _self = this;
	        	//Update project Avatar UI
	        	var projectAvatars = $('.project-avatar-' + groupId);
	        	if(projectAvatars.length > 0){
	        		for(var i = 0; i < projectAvatars.length; i++){
		        		var avatar = $(projectAvatars[i]);
		        		avatar.attr("src", localAvatarUrl);
		        	} 
	        	}
	        	//Update localAvatarUrl to Cached Group
	        	var cachedGroup = _self.findCachedGroupByGroupId(groupId);
	        	if(cachedGroup && localAvatarUrl != "" && localAvatarUrl != cachedGroup.localAvatar){
	        		cachedGroup.localAvatar = localAvatarUrl;
	        		_self.addOrUpdateProjectGroupToCache(groupId, cachedGroup);	        		
	        	}
	        },
	        refreshChatGroupAvatar: function(groupId, group) {
	        	var _self = this;
        		
        		var twoMembers = _self.getGroupTwoMembers(groupId, group);
        		var groupAdminId = twoMembers[0];
        		var groupMemberId = twoMembers[1];
        		
        		//Get how many members have avatars
        		var count = 0;	     
        		var groupAvatarUrlArray = [];
        		for(var j = 0; j < twoMembers.length; j++){
        			var memberId = twoMembers[j];
        			var contactBefore = uc.modules.contact.ContactManager.getCachedContact(memberId);
        			
        			if(contactBefore != null){
        				if(contactBefore.localAvatar){	        					
        					groupAvatarUrlArray.push(contactBefore.localAvatar);
        					count++;
        				} else if (contactBefore.remoteAvatarUrl) {
        					//Call download avatar
            				uc.IUIService.downloadUserAvatar(memberId, contactBefore.remoteAvatarUrl);
        				}	        				
        			} 
        			if (memberId <= 0){
        				//Some old group does not have group admin
        				groupAvatarUrlArray.push(uc.constants.UI.DefaultLocalUrl.Avatar);
    					count++;
        			}
        		}	     
        		if(groupAvatarUrlArray.length < 2){
        			groupAvatarUrlArray.push(uc.constants.UI.DefaultLocalUrl.Avatar);
        		}        		  		
        		//If two members have localAvatarUrl, then call uc.util.AvatarUtil.composeGroupAvatar to show group avatar
        		var groupNewAvatarHtml = uc.util.AvatarUtil.composeGroupAvatar(groupAvatarUrlArray, uc.constants.UI.DefaultLocalUrl.Avatar);
    			var groupNewAvatar = $(groupNewAvatarHtml);	        			
    			groupNewAvatar.addClass("avatar-loaded");    			
    			groupNewAvatar.find('img').addClass("avatar");
    			groupNewAvatar.find('.avatar-admin').addClass("avatar-" + groupAdminId);
    			groupNewAvatar.find('.avatar-attendee').addClass("avatar-" + groupMemberId);
    			
    			var groupItem = $('.group-item-' + groupId);
        		if(groupItem.length > 0) {
        			var groupAvatar = groupItem.find('.group-avatar');
        			groupAvatar.html(groupNewAvatar.html());
        			groupAvatar.addClass("avatar-loaded");        				
        		}    		
        		var groupDetail = $('.group-detail');
        		if(groupDetail.length > 0 && Number(groupDetail.attr("groupId")) == groupId) {
        			var groupAvatar = groupDetail.find('.group-avatar');
        			if(!groupAvatar.hasClass("avatar-loaded")){
        				groupAvatar.html(groupNewAvatar.html());
        				groupAvatar.addClass("avatar-loaded"); 
        			}
        		}				
	        },
	        noticeGroupCreated: function(group,errorCode){
	        	var _self = this;
			var errorMsg;
			switch (errorCode) {
				case 10449:
					errorMsg = uc_resource.GroupChat.GetGroupListFailed;
					break;
				case 10362:
					errorMsg = uc_resource.GroupChat.MemberCountLimited;
					break;
				case 10363:
					errorMsg = uc_resource.GroupChat.GroupCountLimited;
					break;
				case 10369:
					errorMsg = uc_resource.GroupChat.GroupCreateLimited;
					break;
			}

			if (errorMsg) {
				_self.popupMessage(errorMsg);
				return;
			}
	        	var groupId = group.id;	        	
	        	var cachedGroup = _self.findCachedGroupByGroupId(groupId);
	        	if(cachedGroup){	        		
	        		return;
	        	}    
	        	_self.showNewGroupItemUI(group);
	        },
	        showNewGroupItemUI: function(group) {
	        	var _self = this;
	        	var groupType = group.groupType;
	        	var groups;
	        	var alphabeticalSortGroupArray;	  
	        	var parentUL;
	        	
	        	var groupId = group.id;
	        	
	        	//1. Insert into Cache, Sort Cache again
	        	if(groupType == uc.constants.GroupType.Project){
	        		//Project
		        	_self.addOrUpdateProjectGroupToCache(groupId, group);		        	
		        	groups = _self.getCachedProjectGroups();		        	
		        	parentUL = $('.contacts-panel .project-list');		        	
	        	} else {
	        		//Chat
	        		_self.addOrUpdateChatGroupToCache(groupId, group);	        		
	        		groups = _self.getCachedChatGroups();	        		
		        	parentUL = $('.contacts-panel .group-chat-list');
	        	}
	        	//Sorted array, #(ZZ) last, 
	        	alphabeticalSortGroupArray = uc.modules.contact.ContactManager.getAlphabeticalSortArray(groups, "pinyin", "name"); 	
	        	        	
	        	//2. Check whether need display on UI
	        	var currentIndex = $.inArray(group, alphabeticalSortGroupArray);	        	        	
	        	
	        	var originalKey = "#";
	        	var key = _self.poundKeyReplacer;
	        	if(group.hasOwnProperty("pinyin") && group["pinyin"].length > 0){
	        		originalKey = group["pinyin"].substr(0,1);
	        	}
	        	key = originalKey.replace("#", _self.poundKeyReplacer);
	        	
	        	var currentGroupMemberIdArray = [];
	        	if(currentIndex == 0){
	        		//prepend new group to parent
	        		//parentUL.append("<div>" + group.name  + "</div>");
	        		var firstLiElement = parentUL.find("li:first");
	        		if(firstLiElement.length == 0){
	        			firstLiElement = null;
	        		}
    	        	var keyItem = parentUL.find('li[alphabetical-key="'+ key +'"]');
    	        	var previousElement;
    	        	if(keyItem.length == 0){
    	        		previousElement =  _self.displayGroupKey(parentUL, key, firstLiElement, true);       	        		
    	        	} else {
    	        		previousElement = keyItem;        	        		
    	        	}    	        	
	        		currentGroupMemberIdArray = _self.displayGroupItem(group, parentUL, previousElement);
	        	} else if(currentIndex > 0){
	        		//Find previous group, insertAfter previous group
	        		//If nod found, no need to process; will process when scroll
	        		var previousGroup = alphabeticalSortGroupArray[currentIndex - 1];
	        		var previousGroupId = previousGroup.id;
	        		var previousGroupElement = parentUL.find('.group-item-'+ previousGroupId);	        		
	        		if(previousGroupElement.length > 0) {		        			
	        			//If sort key different from last item, then need add key
	        			var lastKey = previousGroup.sortKey;
	        			if(originalKey == lastKey){
	        				//No need to insert key item;
	        				currentGroupMemberIdArray = _self.displayGroupItem(group, parentUL, previousGroupElement);	        					
	        			} else {
	        	        	var keyItem = parentUL.find('li[alphabetical-key="'+ key +'"]');
	        	        	var previousElement;
	        	        	if(keyItem.length == 0){
	        	        		previousElement =  _self.displayGroupKey(parentUL, key, previousGroupElement);        	        		
	        	        	} else {
	        	        		previousElement = keyItem;        	        		
	        	        	}
	        	        	currentGroupMemberIdArray = _self.displayGroupItem(group, parentUL, previousElement);
	        			} 	        			
	        		}	        		
	        	}
	        	uc.modules.contact.ContactManager.batchRequestMembers(currentGroupMemberIdArray);	        	
	        }
    }
})(uc);