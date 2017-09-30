/**
 * Created by senlin.ouyang on 2015/7/20.
 */
(function(uc){
	uc.modules.group.GroupEventListener = function(){

    };
    jQuery.extend(uc.modules.group.GroupEventListener.prototype,
        uc.interfaces.IUIEvent,         
        uc.interfaces.IGroupEvent,
        uc.interfaces.IFSEvent, {
    		OnGroupListReceived: function(errorCode, groupType, groups){
    			if(errorCode == 10101){    				
    				uc.util.LogUtil.info('GroupEventListener', 'OnGroupListReceived', 'Session time out.', null);
    				return;
    			}
    			switch(groupType){        			
	    			case uc.constants.Contact.SearchType.Project:
	    				uc.modules.group.GroupManager.clearCachedProjectGroups();
	    				uc.modules.group.GroupManager.loadProjectsPanel(groups);
	    				break;
	    			case uc.constants.Contact.SearchType.Group:
	    				uc.modules.group.GroupManager.clearCachedChatGroups();	    				
	    				uc.modules.group.GroupManager.loadGroupChatPanel(groups);
	    				break;
					case uc.constants.Contact.SearchType.ProjectGroup:
						uc.modules.group.GroupManager.clearCachedChatGroups();
						uc.modules.group.GroupManager.clearCachedProjectGroups();
						uc.modules.group.GroupManager.loadProjectsPanel(groups);
						uc.modules.group.GroupManager.loadGroupChatPanel(groups);
						break;
	    		}    			
    		},
    		OnGroupInfoReceived: function(errorCode, groupId,mountId, group){
    			//uc.modules.group.GroupManager.getGroupDetailTemplate();
    			
    			//When Group creator a new Group, Group Member will receive this Event    			
    			this.createGroupUI(errorCode, group);
    		},
    		OnDisturbSet: function(errorCode, groupId, isDisturb){
    			uc.modules.group.GroupManager.onGroupDisturbSet(errorCode,groupId, isDisturb);
    		},
    		OnGroupNameUpdated: function (errorCode, groupId, operatorId, groupName, pinyin) {
    			//uc.modules.group.GroupManager.noticeGroupNameUpdated(groupId, groupName, pinyin);
    			//For group creator, Will be processed in OnGroupNameUpdatedMessageReceived
            },
            OnGroupNameUpdatedMessageReceived: function(conversationType, channel, from, to, operatorId, groupId, groupName, groupNamePinYin, msgId, msgSeq, msgTime){            	
            	//Channel: UCAS = 1, LIST = 2, HISTORY = 3, UCCSERVICE = 4; Now have UCAS and HISTORY, only need process UCAS (Instant message)
            	if(channel != uc.constants.Message.MessageChannel.UCAS) {
            		return;
            	}
            	if(conversationType == uc.constants.GroupType.Project || conversationType == uc.constants.GroupType.Group) {    				
    				uc.modules.group.GroupManager.noticeGroupNameUpdated(groupId, groupName, groupNamePinYin);
    			}
            },
    		OnGroupMemberAdded: function(errorCode, groupId, operatorId, memberIds){
    			if(errorCode != 0){    				
    				uc.util.LogUtil.info('GroupEventListener', 'OnGroupMemberAdded', 'Exception: ' + errorCode, null);
    				return;
    			}
    			uc.modules.group.GroupManager.onGroupMembersAdded(groupId, memberIds,errorCode);
    		},
    		GroupMemberKickedMessageReceived: function(conversationType, channel, from, to, operatorId, groupId, groupName, groupAvatar, members, msgId, msgSeq, msgTime) {
    			//Channel: UCAS = 1, LIST = 2, HISTORY = 3, UCCSERVICE = 4; Now have UCAS and HISTORY, only need process UCAS (Instant message)
            	if(channel != uc.constants.Message.MessageChannel.UCAS) {
            		return;
            	}
    			//When Group member is removed from Group, will receive this event.
    			//members: [id1, id2]
    			this.groupMemberRemoved(groupId, members);
    		},
    		OnGroupMemberExitedMessageReceived: function(conversationType, channel, from, to, operatorId, memberId, groupId, groupName, groupAvatar, msgId, msgSeq, msgTime){
    			//Channel: UCAS = 1, LIST = 2, HISTORY = 3, UCCSERVICE = 4; Now have UCAS and HISTORY, only need process UCAS (Instant message)
            	if(channel != uc.constants.Message.MessageChannel.UCAS) {
            		return;
            	}
    			//When A Group member exists from Group, will receive this event.
    			var memberIds = [];
    			memberIds.push(memberId);
    			this.groupMemberRemoved(groupId, memberIds);
    		},
    		OnGroupMemberKicked: function(errorCode, groupId, operatorId, memberIds){
    			if(errorCode != 0){    				
    				uc.util.LogUtil.info('GroupEventListener', 'OnGroupMemberKicked', 'Exception: ' + errorCode, null);
    				return;
    			}
    			this.groupMemberRemoved(groupId, memberIds);
    		},
    		groupMemberRemoved: function(groupId, memberIds) {
                if (uc.modules.profile.GroupProfile && groupId != uc.modules.profile.GroupProfile.groupId) {
                    return;
                }
    			var currentUserInfo = uc.IContactService.getCurrentUserInfo();
				var _isCurrentUserRemoved = false;
    			
    			//memberIds: [id1, id2] -> members: [{userId: id1}, {userId: id2}]; 
    			var members = [];
    			for(var i = 0; i < memberIds.length; i++){
    				var member = {};
    				if(memberIds[i].hasOwnProperty("userId")) {
    					member.userId = memberIds[i].userId;
    				} else {
    					member.userId = memberIds[i];
    				} 
    				
    				if(member.userId == currentUserInfo.userId){
    					_isCurrentUserRemoved = true;
    				}
    				members.push(member);
    			}    			
    			uc.modules.group.GroupManager.onGroupMemberRemoved(groupId, members);
    			
    			// popupGroupProfile
    			if(uc.modules.profile.GroupProfile && groupId){
    				var groupInfo = uc.IGroupService.getGroupInfo(groupId);
    				uc.modules.profile.GroupProfile.loadGroupInfo(groupInfo);
    				
    				if(_isCurrentUserRemoved){// current user removed
    					var message = uc_resource.Profile.YouBeenRemovedGroup;
    					if(groupInfo.groupType == uc.constants.GroupType.Project){
    						message = uc_resource.Profile.YouBeenRemovedProject;
    					}
    					
    					uc.modules.component.PromptMessageBox.open({title: uc_resource.Profile.Prompt, message: message, buttonMode:"ok", OkText: uc_resource.Common.OK}, function(result){
    	        			this.close(); 
    	        			uc.util.WindowUtil.close();
    	                });
    		        	e.stopPropagation();
    				}
    			}
    		},
    		OnGroupEnded: function(errorCode, groupId, operatorId){
    			if(errorCode != 0){
    				uc.util.LogUtil.info('GroupEventListener', 'OnGroupEnded', 'Exception: ' + errorCode, null);
    				return;
    			}
    			this.currentUserEndGroup(groupId);
    		},
    		OnGroupEndMessageReceived: function(conversationType, channel, from, to, operatorId, msgId, msgSeq, msgTime, conversation){
    			//Channel: UCAS = 1, LIST = 2, HISTORY = 3, UCCSERVICE = 4; Now have UCAS and HISTORY, only need process UCAS (Instant message)
            	if(channel != uc.constants.Message.MessageChannel.UCAS) {
            		return;
            	}
    			if(conversationType == uc.constants.GroupType.Project || conversationType == uc.constants.GroupType.Group) {
    				var groupId = to.userId;        			
        			this.currentUserEndGroup(groupId);
    			}                
            },            
            currentUserEndGroup: function(groupId) {
            	var currentUser = uc.IContactService.getCurrentUserInfo();
        		var currentUserId = currentUser.userId;
    			var memberIds = [];
    			memberIds.push(currentUserId);    			
    			this.groupMemberRemoved(groupId, memberIds);
            },
    		OnGroupExited: function(errorCode, groupId, userId){
    			if(errorCode != 0){
    				uc.util.LogUtil.info('GroupEventListener', 'OnGroupExited', 'Exception: ' + errorCode, null);
    				return;
    			}
    			var memberIds = [];
    			memberIds.push(userId);    			
    			this.groupMemberRemoved(groupId, memberIds);
    		},    	
    		//When a group created, Creator will receive OnGroupCreated event; Members will receive OnGroupMemberAddedMessageReceived
    		OnGroupCreated: function (errorCode, operatorId, group) {
    			this.createGroupUI(errorCode, group);
            },
            //When others create a group and invite current user, will receive this message
            OnGroupCreatedMessageReceived: function(conversationType, channel, from, to, operatorId, groupId, groupName, groupAvatar, members, msgId, msgSeq, msgTime){
            	//Channel: UCAS = 1, LIST = 2, HISTORY = 3, UCCSERVICE = 4; Now have UCAS and HISTORY, only need process UCAS (Instant message)
            	if(channel != uc.constants.Message.MessageChannel.UCAS) {
            		return;
            	}
            	var groupType = conversationType;
                if(groupType != uc.constants.GroupType.Project && groupType != uc.constants.GroupType.Group) {
             		return;
             	} 
            	var group = uc.IGroupService.getGroupInfo(groupId);
            	if(group){            		
            		group.groupType = groupType;
            		uc.modules.group.GroupManager.noticeGroupCreated(group);
            		//group.members: only include new members: members [{userId: id1}, {userId: id2}]
            		//uc.modules.group.GroupManager.onGroupMembersAdded(groupId, members);
            	} else {
            		//Put to map, when receive GroupInfo event, call uc.modules.group.GroupManager.noticeGroupCreated(group);
            	}
            },
            createGroupUI: function(errorCode, group) {
            	if(errorCode != 0){    				
    				uc.util.LogUtil.info('GroupEventListener', 'OnGroupCreated', 'Exception: ' + errorCode, null);
    				return;
    			}
    			if(group.groupType != uc.constants.GroupType.Project && group.groupType != uc.constants.GroupType.Group) {
            		return;
            	}
    			uc.modules.group.GroupManager.noticeGroupCreated(group,errorCode);
            },
            OnGroupMemberAddedMessageReceived: function(conversationType, channel, from, to, operatorId, groupId, groupName, groupAvatar, members, msgId, msgSeq, timestamp ){
            	//Channel: UCAS = 1, LIST = 2, HISTORY = 3, UCCSERVICE = 4; Now have UCAS and HISTORY, only need process UCAS (Instant message)
            	if(channel != uc.constants.Message.MessageChannel.UCAS) {
            		return;
            	}
            	var groupType = conversationType;
                if(groupType != uc.constants.GroupType.Project && groupType != uc.constants.GroupType.Group) {
            		return;
            	}

                if (uc.modules.profile.GroupProfile && groupId != uc.modules.profile.GroupProfile.groupId) {
                    return;
                }

            	var group = uc.IGroupService.getGroupInfo(groupId);            	
            	if(group){
            		uc.modules.group.GroupManager.noticeGroupCreated(group);
            		//group.members: only include new members: members [{userId: id1}, {userId: id2}]
            		uc.modules.group.GroupManager.onGroupMembersAdded(groupId, members);
            		
            		if(uc.modules.profile.GroupProfile && groupId){
        				uc.modules.profile.GroupProfile.initUI(groupId);
        			}
            	} else {
            		//Put to map, when receive GroupInfo event, call uc.modules.group.GroupManager.noticeGroupCreated(group);
            	}
            	
            },
    		OnFileUploaded: function (errorCode, taskId, fileName, fileSize, operatorId, fid, operatorTime, downloadURL, convertDownloadURL) {
    			if(errorCode != 0){    				
    				return;
    			}
    			uc.modules.group.GroupManager.noticeProjectAvatarUploaded(taskId, downloadURL);
    		},
    		OnProjectAvatarDownloaded: function(groupId, serverAvatarUrl, localAvatarUrl){
    			uc.modules.group.GroupManager.refreshProjectGroupAvatar(groupId, localAvatarUrl);
            },
    		OnGroupAvatarUpdated: function(errorCode,groupId, logoURL){
    			if(errorCode != 0){
    				uc.util.LogUtil.info('GroupEventListener', 'OnGroupAvatarUpdated', 'Session time out.', null);
    				return;
    			}
    			if(logoURL != ""
	            	&& ( logoURL.toLowerCase().indexOf("http") >=0 || logoURL.toLowerCase().indexOf("https") >=0 ) ){
	            	uc.IUIService.downloadProjectAvatar(groupId, logoURL);
				}
    		},
    		OnGroupAvatarUpdatedMessageReceived: function(conversationType, channel, from, to, operatorId, groupId, groupAvatar, msgId, msgSeq, msgTime){
    			//Channel: UCAS = 1, LIST = 2, HISTORY = 3, UCCSERVICE = 4; Now have UCAS and HISTORY, only need process UCAS (Instant message)
            	if(channel != uc.constants.Message.MessageChannel.UCAS) {
            		return;
            	}
    		},
			OnGroupDisturbSetMessageReceived: function(conversationType, channel, from, to, groupId, state, msgId, msgSeq, msgTime){
				if(channel != uc.constants.Message.MessageChannel.UCAS) {
					return;
				}
				//console.log();
				uc.modules.group.GroupManager.noticeGroupDisturbSetMessageReceived(conversationType, channel, from, to, groupId, state, msgId, msgSeq, msgTime);
			}
    });
})(uc);