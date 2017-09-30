/**
 * Created by senlin.ouyang on 2015/7/20.
 */
(function(uc){
    uc.modules.profile.ProfileManager = {
        init: function(){
			if(this.initilized){
				return;
			}

			this.initilized = true;
        },
        OnModuleChanged: function(moduleId){
            if (moduleId == uc.constants.UI.ModuleId.Me) {
            	this.init();
				uc.modules.profile.Me.init();
            }
        },
        loadColleagues: function(profileDetail, userId, colleagues, leaders){
        	var currentUserId = profileDetail.attr("userId");

        	if(currentUserId != userId) {        		
        		return;
        	}
        	
        	var memberIdArray = [];
        	
        	var leadersField = profileDetail.find('.leaders-field');
        	var leaderUl = profileDetail.find('.leader-list');
        	
        	if(leaders.length == 0) {
    			leadersField.hide();
    			leaderUl.hide();
        	} else {
        		leadersField.show();
    			leaderUl.show();
    			
    			uc.modules.contact.ContactManager.batchDisplayContacts(leaderUl, leaders);        		
        	}
        	
        	var colleaguesField = profileDetail.find('.colleagues-field');
        	var colleagueUl = profileDetail.find('.colleague-list');
        	
        	if(colleagues.length == 0) {
    			colleaguesField.hide();
    			colleagueUl.hide();
        	} else {
        		colleaguesField.show();
    			colleagueUl.show();
    			
    			uc.modules.contact.ContactManager.batchDisplayContacts(colleagueUl, colleagues);        		        		
        	}
        	uc.modules.contact.ContactManager.batchRequestMembers(memberIdArray);
        },
        noticeUserAvatarDownloaded: function(userId, serverAvatarUrl, localAvatarUrl) {    
        	if(localAvatarUrl == "") {
        		return;
        	}        	
        	var contact = uc.modules.contact.ContactManager.getCachedContact(userId);
			if(contact){
				contact.localAvatar = localAvatarUrl;
				uc.modules.contact.ContactManager.addOrUpdateContactToCache(userId, contact);
			}
        	var avatars = $('.avatar-' + userId);        	
        	$.each(avatars,function(k,avatar){
        		$(avatar).attr("src", localAvatarUrl);
        	});       	
        },
        /**
         * event for show contact profile
         * @param userId    userId of contact to show profile
         */
        OnShowContactProfile: function (userId) {

        },
        /**
         * event for show pop up contact profile
         * @param userId
         * @constructor
         */
        OnShowPopupContactProfile: function (userId) {        	
			uc.util.StorageUtil.setData('popup_contact_user_id', userId);
			
			var options = {
					width: 440,
	                height: 560,
	                refreshIfExist: false,
	                centerScreen: true,
	                modal: true,
	                resizable: false,
	                minWidth: 440,
	                minHeight: 560,
	                caption: uc.constants.UI.DragCaption.Profile
			};
			
            uc.util.WindowUtil.open('page/profile.html', 'profile'+ userId, options);
        },
        OnShowPopupGroupProfile: function (groupId) {
			var groupInfo = uc.IGroupService.getGroupInfo(groupId);			
			if(!groupInfo || groupInfo.joinState == 1 || groupInfo.status == 1){
				return ;
			}
			
			uc.util.StorageUtil.setData('popupGroupProfile', groupId);
	        uc.util.StorageUtil.setData('InProgressCallInfo', uc.modules.call.CallManager.inProgressCallInfo);
			
			var options = {
					width: 440,
	                height: 560,
	                refreshIfExist: false,
	                centerScreen: true,
	                modal: true,
	                resizable: false,
	                minWidth: 440,
	                minHeight: 560,
	                caption: uc.constants.UI.DragCaption.GroupProfile
			};
			
            uc.util.WindowUtil.open('page/groupProfile.html', 'groupProfile' + groupId, options);
        },


		noticeGroupInfoReceived: function (errorCode, groupId, group) {
			uc.IUIService.triggerCrossWindowEvent("SendGroupId",{
				data:{"errorCode":errorCode,"groupId": groupId,"groupInfo":group}
			});
		},
		OnUserStatusSetted: function(errorCode, userId, status) {
		},
		OnUserStatusReceived: function(errorCode, status) {
			if (uc.modules.profile.PopupContactProfile) {
				uc.modules.profile.PopupContactProfile.OnUserStatusReceived(errorCode, status);
			}
			//uc.modules.profile.Me.OnUserStatusReceived(errorCode, status);
		},
		onGroupDisturbSet: function(errorCode, groupId, newDisturbState) {
			if(errorCode!=0){
				uc.util.LogUtil.info("ProfileManager","onGroupDisturbSet","set disturb failed,errorCode="+errorCode+',groupId='+groupId);
				uc.modules.component.PromptMessageBox.open({
					title: uc_resource.WindowTitle.Notification,
					message: uc_resource.Message.DisturbSetFailed,
					buttonMode: "ok",
					OkText: uc_resource.Common.OK
				}, function (result) {
					this.close();
				});
			}
		},
		//OnContactInfoReceived:function(errorCode, userIds, contacts){
		//	if(errorCode == 0) {
		//		uc.modules.profile.PopupContactProfile.loadProfileDetail(contacts);
		//	}else{
		//		uc.util.LogUtil.info("ProfileManager","OnContactInfoReceived","OnContactInfoReceived failed,errorCode="+errorCode+',userIds='+userIds);
		//	}
		//},
		noticeGroupDisturbSetMessageReceived:function(conversationType, channel, from, to, groupId, state, msgId, msgSeq, msgTime){
			if(this == uc.modules.profile.GroupProfile){
				var groupInfo = uc.IGroupService.getGroupInfo(groupId);
				var groupDetail = $('.group-detail:visible');
				var visibleGroupId = Number(groupDetail.attr("groupId"));
				if(visibleGroupId == groupId){
					uc.modules.profile.GroupProfile.messageDisturbSwitcher.switchButton(groupInfo.disturbState == 1);
				}
			}
		}
    }
})(uc);