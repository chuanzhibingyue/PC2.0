/**
 * Created by senlin.ouyang on 2015/7/23.
 */
(function (uc) {
    uc.modules.profile.ProfileEventListener = function () {

    };
    jQuery.extend(uc.modules.profile.ProfileEventListener.prototype,
        uc.interfaces.IUIEvent, 
        uc.interfaces.IContactEvent,
		uc.interfaces.IGroupEvent,{
            OnModuleChanged: function (moduleId) {
                uc.modules.profile.ProfileManager.OnModuleChanged(moduleId);
            },
            OnColleagueInfoReceived: function(errorCode, userId, page, colleagueNumber, colleagues, leaders){ 
            	uc.util.LogUtil.info('ProfileEventListener', 'OnColleagueInfoReceived', 'Exception:' + errorCode, null);

            	var profileDetail = $('#layout-container .me .menu-panel-body-body .profile-detail');            	
            	if(profileDetail.length > 0){            		
            		uc.modules.profile.ProfileManager.loadColleagues(profileDetail, userId, colleagues, leaders);
            	}
                var myColleagues = $('#layout-container .me .menu-panel-body-body .my-colleagues-wrapper');
                if(myColleagues.length > 0){
                    uc.modules.profile.ProfileManager.loadColleagues(myColleagues, userId, colleagues, leaders);
                }

            	var popupProfileDetail = $('#layoutContainer .menu-panel-body.popup .menu-panel-body-body .profile-detail');
                if(popupProfileDetail.length > 0){
            		uc.modules.profile.ProfileManager.loadColleagues(popupProfileDetail, userId, colleagues, leaders);
            	}
                var popupColleagues = $('#layoutContainer .menu-panel-body.popup .menu-panel-body-body .colleagues-wrapper');
                if(popupColleagues.length > 0){
                    uc.modules.profile.ProfileManager.loadColleagues(popupColleagues, userId, colleagues, leaders);
                }
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
            	uc.util.LogUtil.info('ProfileEventListener', 'OnShowPopupContactProfile', 'userId=' + userId, null);
                uc.modules.profile.ProfileManager.OnShowPopupContactProfile(userId);
            },
            OnShowPopupGroupProfile: function(groupId){
            	uc.util.LogUtil.info('ProfileEventListener', 'OnShowPopupGroupProfile', 'groupId=' + groupId, null);
            	uc.modules.profile.ProfileManager.OnShowPopupGroupProfile(groupId);
            },
            OnCurrentUserInfoUpdated: function(errorCode, userInfo){
            	uc.util.LogUtil.info('ProfileEventListener', 'OnCurrentUserInfoUpdated', 'Exception:' + errorCode, null);
            	uc.modules.contact.ContactManager.noticeContactUpdated(userInfo);
            },
            OnGroupInfoReceived: function (errorCode, groupId, group) {
                uc.util.LogUtil.info('ProfileEventListener', 'OnGroupInfoReceived', 'Exception:' + errorCode +',groupId:'+ groupId+',group'+group, null);
                uc.modules.profile.ProfileManager.noticeGroupInfoReceived(errorCode, groupId, group);
            },
            OnUserTagsUpdated: function(errorCode,UpdatedTags){
                if(errorCode){
                    uc.util.LogUtil.info('ProfileEventListener', 'OnUserTagsUpdated', 'Exception:' + errorCode);
                }
                uc.modules.profile.Me.OnUserTagsUpdated(errorCode,UpdatedTags);
            },
            OnUserStatusSetted: function(errorCode, userId, status) {
            },
            OnUserStatusReceived: function(errorCode, status) {
                uc.modules.profile.ProfileManager.OnUserStatusReceived(errorCode, status);
            },
            OnGroupDisturbSetMessageReceived: function(conversationType, channel, from, to, groupId, state, msgId, msgSeq, msgTime){
                if(channel != uc.constants.Message.MessageChannel.UCAS) {
                    return;
                }
                uc.modules.profile.ProfileManager.noticeGroupDisturbSetMessageReceived(conversationType, channel, from, to, groupId, state, msgId, msgSeq, msgTime);
            },
            OnDisturbSet: function(errorCode, groupId, isDisturb){
                uc.modules.profile.ProfileManager.onGroupDisturbSet(errorCode,groupId, isDisturb);
            },
            OnContactInfoReceived: function(errorCode, userId, contact) {
                uc.modules.profile.Me.OnContactInfoReceived(errorCode, userId, contact);
            }
        });
})(uc);