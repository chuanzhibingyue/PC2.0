/**
 * Created by senlin.ouyang on 2015/7/21.
 */
(function(uc){
    uc.modules.ContactMainEventListener = function(){

    };
    jQuery.extend(uc.modules.ContactMainEventListener.prototype,
        uc.interfaces.IUIEvent, 
        uc.interfaces.IContactEvent, 
        uc.interfaces.IFSEvent, 
        uc.interfaces.IMicroblogEvent, {    		
	    	OnModuleChanged: function(moduleId){
	    		uc.modules.contact.ContactManager.OnModuleChanged(moduleId);	            
	        },
	        OnContactAdded: function(errorCode, userId){
	        	if(errorCode != 0){
	        		uc.util.LogUtil.info('ContactMainEventListener', 'OnContactAdded', 'Exception:' + errorCode, null);
    				return;
    			}
	        	//When Frequent Contact is added	        	
	        	uc.modules.contact.ContactManager.noticeFrequentContactAdded(userId);
	        },
	        OnContactDeleted: function(errorCode, userId){
	        	if(errorCode != 0){
	        		uc.util.LogUtil.info('ContactMainEventListener', 'OnContactDeleted', 'Exception:' + errorCode, null);
    				return;
    			}
	        	//When Frequent Contact is removed	        	
	        	uc.modules.contact.ContactManager.noticeFrequentContactRemoved(userId);
	        },
	        OnUserAvatarDownloaded: function(userId, serverAvatarUrl, localAvatarUrl) {	        	
	        	uc.modules.contact.ContactManager.noticeUserAvatarDownloaded(userId, serverAvatarUrl, localAvatarUrl);
	        	
	        	if(!$('.menu-panel-body.contacts .content-panel .content-panel-body').hasClass("hide")){	        		
	                uc.modules.group.GroupManager.postRefreshGroupsAvatar(userId, serverAvatarUrl, localAvatarUrl);
	        	}
	        	
	        	if(!$('.menu-panel-body.me .menu-panel-body-body ').hasClass("hide")){
	        		uc.modules.profile.ProfileManager.noticeUserAvatarDownloaded(userId, serverAvatarUrl, localAvatarUrl);
	        	}	        	                
            },
            OnContactInfoReceived: function(errorCode, userIds, contacts){
                if(errorCode != 0){
                    uc.util.LogUtil.info('ContactMainEventListener', 'OnContactInfoReceived', 'Exception:' + errorCode, {userIds:userIds, contacts:contacts});
                    return;
                }
                if(uc.modules.profile.PopupContactProfile && contacts && contacts.length == 1){
                    uc.modules.profile.PopupContactProfile.loadProfileDetail(contacts[0]);
                }

                if($('.contacts-panel').is(':hidden')) {
                    return;
                }
                uc.modules.contact.ContactManager.contactInfoReceived(errorCode, userIds, contacts);
            },
            OnBasicContactInfoReceived: function(errorCode, userIds, contacts){
    			if(errorCode != 0){
    				uc.util.LogUtil.info('ContactMainEventListener', 'OnBasicContactInfoReceived', 'Exception:' + errorCode, null);
    				return;
    			}

				if(!contacts || contacts.length < 1){
					return ;
				}
    			
    			//Add received Contacts info to cache    			
    			uc.modules.contact.ContactManager.addOrUpdateContactsToCache(contacts);    			
    			
    			uc.modules.group.GroupManager.loadGroupsCreatorsUI(contacts);
    			//Left Panel: Groups, Load Group Members as group-description 
    			uc.modules.group.GroupManager.loadGroupsMembersUI(contacts);
    			//Right Panel: Detail Group Page, Load Group Members 
    			uc.modules.group.GroupManager.loadGroupMembersUI(contacts);
    			if ($.fn.searchInput) {
                    $.fn.searchInput.OnContactInfoReceived(contacts);
                }
    			//Popup Right Panel: Load original selected Members 
    			if(uc.modules.chooseContacts.PopupChooseContacts) {
    				uc.modules.chooseContacts.PopupChooseContacts.loadSelectedMembersUI(contacts);
    			}    			
    			
    			uc.modules.contact.ContactManager.noticeFrequentContactReceived(contacts);

    			//var contact = contacts[0];
    			//uc.modules.contact.ContactManager.loadContactDetail(contact);
    			//if($('.contacts-panel').is(':hidden')) {
    			//	return;
    			//}
    			//uc.modules.contact.ContactManager.contactInfoReceived(errorCode, userIds, contacts);
    			
    			//uc.modules.chooseContacts.SearchContactsManager.displayDefaultSearchContacts(contacts);
    		},
    		OnCurrentUserInfoUpdated: function(errorCode, userInfo){
	            if(errorCode != 0){
	            	return;
	            }	       
	            var updatedContact = userInfo;
            	uc.modules.contact.ContactManager.noticeContactUpdated(updatedContact);
    			//uc.modules.contact.ContactManager.loadContactDetail(userInfo);
				if(userInfo.localAvatar){
					uc.modules.main.Main.setSelfAvatar(userInfo.localAvatar);
				}
            },
            OnContactUpdated: function (contacts) {
            	if(contacts.length == 0){
            		return;
            	}
            	//Change cached contact's remoteAvatarUrl, personalSign and UI 
            	var updatedContact = contacts[0];
            	uc.modules.contact.ContactManager.noticeContactUpdated(updatedContact);
            	uc.modules.contact.ContactManager.selectedContactJudge(contacts[0]);
				var userInfo = uc.IContactService.getCurrentUserInfo();
				if(updatedContact.userId == userInfo.userId){
					uc.util.LogUtil.info("ContactMainEventListener","OnContactUpdated","updatedContact is self");
					uc.modules.profile.Me.loadProfileDetail(updatedContact);
				}
            },
            OnFileUploaded: function (errorCode, taskId, fileName, fileSize, operatorId, fid, operatorTime, downloadURL, convertDownloadURL) {
    			if(errorCode != 0){    				
    				return;
    			}
    			uc.modules.contact.ContactManager.noticeContactAvatarUploaded(taskId, downloadURL);
    		},
    		OnColleagueInfoReceived: function(errorCode, userId, page, colleagueNumber, colleagues, leaders){    			
    			if($('.contacts').is(':hidden')) {
    				return;
    			}
    			uc.modules.contact.ContactManager.loadColleagues(userId, colleagues, leaders);
    		},
    		OnOrgInfoReceived: function(errorCode, department, myDepartment){
    			var _self = this;
				var currentUserInfo = uc.IContactService.getCurrentUserInfo();
    			if($('.contacts').is(':hidden')) {
    				return;
    			}
        		
        		var parentPanel = uc.modules.contact.ContactManager.getLatestClickedContactsPanel();
            	if(parentPanel == null){
            		return;
            	}
            	
        		var selectedFigure = parentPanel.find('.figure-selected');
            	var panel = selectedFigure.attr('panel');
            	//For Contacts Panel, there's no figure-selected
            	if(selectedFigure.length == 0){
            		panel = "panel-contacts";
            	}      
            	
            	if(errorCode != 0){
    				uc.util.LogUtil.info('ContactMainEventListener', 'OnOrgInfoReceived', 'Exception:' + errorCode, null);
    				var message = uc_resource.ChooseContacts.FailedToGetData;
    				if(panel == "panel-organization") {            		
    					var ul = $('.panel-organization .org-parent');
    					_self.showException(ul, message);    					
                	} else if(panel == "panel-contacts") {
                		$('.panel-contacts .my-departments-list .department-item').addClass("in-exception");
                		$('.panel-contacts .my-departments-list .department-item').text(message);     		
                	} else {            		
                		var ul = $('.panel-my-department .my-department-list');
    					_self.showException(ul, message);
                	}
    				return;
    			}
            	
        		if(panel == "panel-organization") {            		
            		uc.modules.org.OrgManager.loadOrganizationPanel(department, currentUserInfo);
            	} else if(panel == "panel-contacts") {
            		$('.panel-contacts .my-departments-list .department-item').removeClass("in-exception");
            		//If is Popup Select Contacts page, need refresh Current User's department info 
            		if(parentPanel.find('.my-departments-list').length > 0) {            			
            			uc.modules.org.OrgManager.loadMyDepartmentPanel(parentPanel, department);
            		}        		
            	} else {            		
            		//Get staff's department info from parent department
            		if(department.staffs.length > 0){
            			$.each(department.staffs,function(k,staff) {
                    		staff.departmentId =  department.id;     
                    		staff.departmentName =  department.departmentName;
                        });  
            		}            		
            		uc.modules.contact.ContactManager.loadDepartmentMembersPanel(department);            		
            	}        		                
            },
			OnGetUserStatusResponse: function(errorCode, status, copyBack){ // added by xue.bai_2 on 2016/05/04
				uc.util.LogUtil.info('ContactMainEventListener', 'OnGetUserStatusResponse', 'data=', {errorCode: errorCode, status: status, copyBack: copyBack});
				if(errorCode != 0 || !copyBack){
					return ;
				}

				uc.modules.contact.ContactManager.loadContactStatusUI(status, copyBack);
			},
            showException: function(ul, message) {     
            	console.log("showException");
            	ul.empty();
            	ul.append('<div class="in-exception in-loading">' + message + '</div>');
            },
            OnOrgAllMembersReceived: function(errorCode, orgId, userCount, memberIds,copyBack){
	            //解析copyback
	            if(copyBack && parseInt(copyBack) != 0){
		            var copyBackobj = $.parseJSON(copyBack);
		            var managerType =  copyBackobj.managerType;
		            //根据调用时传入的类型，区分执�?
		            switch(managerType){
			            case "uc.modules.chooseContacts.ChooseContactsManager":
				            uc.modules.chooseContacts.ChooseContactsManager.OnOrgAllMembersReceived(errorCode, orgId, userCount, memberIds);
				            break;
			            case "uc.modules.org.OrgManager":
				            var _ChooseContactsManager = uc.modules.chooseContacts.ChooseContactsManager;
				            if (errorCode == 400) {
					            _ChooseContactsManager.popupMessage(_ChooseContactsManager.getOrgAllMembersExceptionMessage);
					            return;
				            }
				            if (errorCode == 10465) {
					            var limitedCount = _ChooseContactsManager.getMembersLimit();
					            var message = uc.util.StringUtil.formatString(_ChooseContactsManager.getMembersLimitMessage(), [limitedCount]);
					            _ChooseContactsManager.popupMessage(message);
					            return;
				            }

				            if(memberIds) {
					            var currentUserInfo = uc.IContactService.getCurrentUserInfo();
					            memberIds.unshift(currentUserInfo.userId);
					            uc.IUIService.startChat(memberIds);
				            }
				            break;
		            }
	            }
            },
            OnAttentionAdd: function (errorCode, userId, relation) {
            	if(errorCode != 0){
            		uc.util.LogUtil.info('ContactMainEventListener', 'OnAttentionAdd', 'Exception:' + errorCode, null);
            		return;
            	}
            	uc.modules.contact.ContactManager.onAttentionChanged(userId, relation);
            },
            OnAttentionCancelled: function (errorCode, userId, relation) {
            	if(errorCode != 0){
            		uc.util.LogUtil.info('ContactMainEventListener', 'OnAttentionCancelled', 'Exception:' + errorCode, null);
            		return;
            	}
            	uc.modules.contact.ContactManager.onAttentionChanged(userId, relation);
            },
			OnUserTagsUpdated: function(errorCode,UpdatedTags){
				if(errorCode){
					uc.util.LogUtil.info('ContactMainEventListener', 'OnUserTagsUpdated', 'Exception:' + errorCode);
				}
				uc.modules.contact.ContactManager.OnUserTagsUpdated(errorCode,UpdatedTags);
			},
			OnUserStatusReceived: function(errorCode, status) {
				uc.modules.contact.ContactManager.OnUserStatusReceived(errorCode, status);
                uc.modules.contact.ContactManager.loadContactListStatusUI(errorCode, status);
			}
    });
})(uc);