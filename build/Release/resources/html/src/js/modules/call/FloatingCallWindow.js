/**
 * Created by waouyang on 15/12/7.
 */
(function(uc){
    var _this = null;
    uc.modules.call.FloatingCallWindow = {
	    isInit:false,
        titleNodeHtml: '<div class="title-node">' +
                            '<span class="avatar-node-container">' +
                                '<span class="avatar-node"></span>' +
                                '<span class="audio-icon"></span>' +
                            '</span>' +
                            '<div class="display-name"></div>' +
                            '<div class="call-status "><span></span></div>' +
                        '</div>',
        init: function(callInfo){
	        if(uc.modules.call.FloatingCallWindow.isInit){
		        return false;
	        }
	        uc.util.StorageUtil.setData('floating_call_window_isActivity',true);
            _this = this;
            this.callInfo = callInfo;
			this.callwindow= $('.floating-call-window');
            this.titleNodeContainer = $('.call-title-container');
            this.callPanelContainer = $('.call-panel-container');
            this.toolBarContainer = $('.call-tool-bar-container');

            this.titleNode = $(this.titleNodeHtml).appendTo(this.titleNodeContainer);
            this.avatarNode = $('.avatar-node', this.titleNode);
            this.audioIcon = $('.audio-icon', this.titleNode);
            this.displayNameNode = $('.display-name', this.titleNode);
            this.callStatusNode = $('.call-status', this.titleNode);
            this.callStatusTime = $('.call-status span', this.titleNode);

	        this.bottomBarToggle = $(".bottom-bar-toggle");
	        this.bottomBarToggleBtn = $(".bottom-bar-toggle-btn",this.callwindow);
	        this.screenSharebtn =  this.bottomBarToggle.find('ul li .ScreenShare');
	        this.updateWindowWH();

	        this._initUI();
            this._bindEvent();

            uc.modules.component.TitleBarButton.init(uc.constants.UI.WindowCloseBehavior.UnClose);
	        this.updateBottomBarStatus(callInfo);
        },

        _initUI: function(){
	        uc.modules.call.FloatingCallWindow.isInit = true;
            switch (this.callInfo.chatType){
                case uc.constants.Chat.ChatType.SingleChat:
                    this._showContactCallTitle();
                    break;
                case uc.constants.Chat.ChatType.ProjectChat:
                case uc.constants.Chat.ChatType.GroupChat:
                    this._showGroupCallTitle();
                    break;
            }
            if(this.callInfo.videoStatus == uc.constants.Call.VideoStatus.CLOSE){
                this.titleNode.addClass('no-video');
            }
            this._setCallStatus();
            uc.modules.component.FloatingCallWindowToolBar.init(this.toolBarContainer, this.callInfo);
        },

        _bindEvent: function(){
            var _this=this;
            this.avatarNode.dblclick(function(){
                uc.modules.call.FloatingCallWindowManager.switchFloatingCallWindowToEmbedded(_this.callInfo);
            });
            $('.icon-close', '.tool-icon-container').click(function(){
                if(_this.callInfo.hostId!=_this.callInfo.loginUserId && _this.callInfo.callStatus!=uc.constants.Call.CallStatus.CONNECTED){
                    var rejectedData={
                        callInfo:_this.callInfo,
                        reason:'',
                        isClose:true
                    };
                    uc.IUIService.triggerCrossWindowEvent(uc.constants.UI.CrossWindowEventName.RejectCallInvitation, rejectedData);

                }else if(_this.callInfo.callStatus==uc.constants.Call.CallStatus.CONNECTED || _this.callInfo.callStatus==uc.constants.Call.CallStatus.CONNECTING){
                    uc.IUIService.triggerCrossWindowEvent(uc.constants.UI.CrossWindowEventName.LeaveCall, {callInfo:_this.callInfo,isClose:true});
                }else if(_this.callInfo.callStatus==uc.constants.Call.CallStatus.INVITING){
                    uc.IUIService.triggerCrossWindowEvent(uc.constants.UI.CrossWindowEventName.CancelCall, {callInfo:_this.callInfo,isClose:true});
                }else if(_this.callInfo.callStatus==uc.constants.Call.CallStatus.INVITED){
                    uc.IUIService.triggerCrossWindowEvent(uc.constants.UI.CrossWindowEventName.RejectCallInvitation, {
                        callInfo:_this.callInfo,
                        reason: '',
                        isClose:true
                    });
                }else{
	                _this.close();
	                uc.util.LogUtil.error('FloatingCallWindow', 'click_close errstatus',_this.callInfo.callStatus );
                }

            });
	        this.bottomBarToggleBtn.on('click',function(e){
		        _this.bottomBarToggle.slideToggle('fast');
		        $(this).find('a').toggleClass('rorate90');
	        });

	        this.screenSharebtn.on('click',function(e){
		        if( _this.screenSharebtn.hasClass('disable')){
			        return false;
		        }
		        if(_this.callInfo.desktopShareStatus == uc.constants.Call.DesktopShareStatus.CLOSE){
			        _this.screenSharebtn.addClass('open');
			        _this.callInfo.desktopShareStatus = uc.constants.Call.DesktopShareStatus.OPEN;
			        uc.IUIService.triggerCrossWindowEvent(uc.constants.UI.CrossWindowEventName.startScreenShare,_this.callInfo);

		        }else {
			        _this.screenSharebtn.removeClass('open');
			        _this.callInfo.desktopShareStatus = uc.constants.Call.DesktopShareStatus.CLOSE;
			        uc.IUIService.triggerCrossWindowEvent(uc.constants.UI.CrossWindowEventName.stopScreenShare,_this.callInfo);
		        }
	        });
        },

        _setCallStatus: function(){
            switch (this.callInfo.callStatus){
                case uc.constants.Call.CallStatus.DISCONNECTED:
                    break;
                case uc.constants.Call.CallStatus.INVITING:
                    this.callStatusTime.text(uc_resource.Call.Inviting);
                    break;
                case uc.constants.Call.CallStatus.INVITED:
                    this.callStatusTime.text(uc_resource.Call.ForYouInviting);
                    break;
                case uc.constants.Call.CallStatus.CONNECTING:
                    this.callStatusTime.text(uc_resource.Call.Connecting);
                    break;
                case uc.constants.Call.CallStatus.CONNECTED:
                    this.setUserVideoStatus();
                    var _this = this;

                    clearInterval(this.countDurationTimer);
                    var duration = (Date.now() - this.callInfo.callConnectedTime) / 1000;
                    this.countDurationTimer = setInterval(function () {
                        duration = duration + 1;
                        var content = uc.util.TimeUtil.getCallTimeStr(duration);
                        _this.callStatusTime.text(content);
                    }, 1000);
                    _this.callStatusTime.text(uc.util.TimeUtil.getCallTimeStr(duration));
                    break;
                case uc.constants.Call.CallStatus.DISCONNECTING:
                    this.callStatusTime.text(uc_resource.Call.DisConnecting);
                    break;
            }
			this.updateWindowWH();


        },
        addAudioClass:function(classType){
            this.callStatusNode.removeClass("icon-call_phoneVideo").removeClass("icon-call_phoneVideoMute")
                .removeClass("icon-call_voipVideo").removeClass("icon-call_voipVideoMute").addClass(classType);
        },
        setUserVideoStatus:function(status){
          if(this.callInfo.chatType== uc.constants.Chat.ChatType.SingleChat){
              var video = status ? status : this.callInfo.userList[this.callInfo.chatId].muteStatus;
              switch  (video){
                  case  uc.constants.Call.MuteStatus.MUTE:
                      if(this.callInfo.userList[this.callInfo.chatId].audioType == uc.constants.Call.AudioType.VOIP){
                          this.addAudioClass("icon-call_voipVideoMute");
                      }else if(this.callInfo.userList[this.callInfo.chatId].audioType == uc.constants.Call.AudioType.PHONE){
                         this.addAudioClass("icon-call_phoneVideoMute");
                      }else{
                          this.addAudioClass("");
                      }
                      break;
                  case  uc.constants.Call.MuteStatus.UN_MUTE:
                  default :
                      if(this.callInfo.userList[this.callInfo.chatId].audioType == uc.constants.Call.AudioType.VOIP){
                          this.addAudioClass("icon-call_voipVideo");
                      }else if(this.callInfo.userList[this.callInfo.chatId].audioType == uc.constants.Call.AudioType.PHONE){
                          this.addAudioClass("icon-call_phoneVideo");
                      }else{
                          this.addAudioClass("");
                      }
                      break;
              }

          }
        },
        _setAudioIcon: function(){

        },
        _showContactCallTitle: function(){
            var contactInfo = uc.modules.cache.BasicContactInfoCache.getBasicContactInfo(this.callInfo.chatId);
            var contactAvatar;
            if(contactInfo){
                if(contactInfo.displayName){
                    this.displayNameNode.text(contactInfo.displayName);
                }else {
                    this.displayNameNode.text(this.callInfo.chatId);
                }
                this.callInfo.siteId=contactInfo.siteId;
                if(contactInfo.localAvatar){
                    contactAvatar = uc.util.AvatarUtil.composeContactAvatar(contactInfo.localAvatar, uc.constants.UI.DefaultLocalUrl.Avatar_40);
                }else {
                    contactAvatar = '<img src="' + uc.constants.UI.DefaultLocalUrl.Avatar_40 + '">';
                }
            }else {
                this.displayNameNode.attr("data-userId",this.callInfo.chatId).text(this.callInfo.chatId);
                contactAvatar = '<img src="' + uc.constants.UI.DefaultLocalUrl.Avatar_40 + '" data-userId="'+this.callInfo.chatId+'">';
            }
            this.avatarNode.append(contactAvatar);
        },
        _showGroupCallTitle: function(){
            var groupInfo = uc.IGroupService.getGroupInfo(this.callInfo.chatId);
            if(groupInfo){
                if(groupInfo.name){
                    this.displayNameNode.text(groupInfo.name);
                }else {
                    this.displayNameNode.text(this.callInfo.chatId);
                }
                this.callInfo.siteId=groupInfo.siteId;
            }else {
                this.displayNameNode.text(this.callInfo.chatId).addClass("no-groupInfo");
            }
            var groupAvatar = this._getGroupAvatar(groupInfo);
            this.avatarNode.append(groupAvatar);
        },
        _getGroupAvatar: function(groupInfo){
            var groupAvatarUrlArray = [];
            this.noAvatarUserIdArray = [];
            var groupAvatar;
            if(this.callInfo.chatType == uc.constants.Chat.ChatType.ProjectChat){
                groupAvatar = uc.util.AvatarUtil.composeProjectAvatar(null,uc.constants.UI.DefaultLocalUrl.Conversation.Project);
                return groupAvatar;
            }
            if(groupInfo){
                var members = groupInfo.members;
                for(var m=0;m<members.length;m++){
                    if(!this.callInfo.userList[members[m].userId]){
                        this.callInfo.userList[members[m].userId]={};
                        this.callInfo.userList[members[m].userId].userInfo={};
                    }
                }
                var adminId = groupInfo.adminId;
                if(adminId){
                    groupAvatarUrlArray.push(this._getContactAvatar(adminId));
                }
                var userId;
                for(var i = 0, len = members.length; i < len && groupAvatarUrlArray.length < uc.constants.UI.GroupAvatarUserCount; i++){
                    if(adminId == members[i].userId){
                        continue;
                    }
                    userId = members[i].userId;
                    groupAvatarUrlArray.push(this._getContactAvatar(userId));
                }
                for(var j = groupAvatarUrlArray.length; j < uc.constants.UI.GroupAvatarUserCount; j++){
                    groupAvatarUrlArray.push(uc.constants.UI.DefaultLocalUrl.Avatar_40);
                }
            }else {
                groupAvatarUrlArray.push(uc.constants.UI.DefaultLocalUrl.Avatar_40);
                groupAvatarUrlArray.push(uc.constants.UI.DefaultLocalUrl.Avatar_40);
            }
            groupAvatar = uc.util.AvatarUtil.composeGroupAvatar(groupAvatarUrlArray, uc.constants.UI.DefaultLocalUrl.Avatar_40);
            return groupAvatar;
        },
        _updateGroupAvatar:function(groupInfo){
            if( this.displayNameNode.hasClass("no-groupInfo")){
                this._getGroupAvatar(groupInfo);
            }
        },
        _getContactAvatar:function(userId) {
            var contactAvatar;
            if(this.callInfo.userList){
                for(var Id in this.callInfo.userList){
                    if(Id==userId && this.callInfo.userList[Id].userInfo.localAvatar){
                        contactAvatar = this.callInfo.userList[Id].userInfo.localAvatar;
                        return  contactAvatar;
                    }else if(Id==userId && !this.callInfo.userList[Id].userInfo.localAvatar){
                        var contactInfo = uc.IContactService.getBasicContactInfo(userId);
                        if(contactInfo){
                            if(contactInfo.localAvatar){
                                contactAvatar = contactInfo.localAvatar;
                                this.callInfo.userList[userId].userInfo.localAvatar=contactInfo.localAvatar;
                            }else {
                                contactAvatar = uc.constants.UI.DefaultLocalUrl.Avatar_40;
                            }
                            this.callInfo.userList[userId].userInfo.displayName=contactInfo.displayName;
                        }else {
                            contactAvatar = uc.constants.UI.DefaultLocalUrl.Avatar_40;
                            this.noAvatarUserIdArray.push(userId);
                        }
                        return contactAvatar;
                    }
                }
            }

        },
        OnBasicContactInfoReceived: function(basicContactInfos){
            this.titleNode.find('.display-name[data-userId='+basicContactInfos.userId+']').text(basicContactInfos.displayName);
            this.avatarNode.find('img[data-userId='+basicContactInfos.userId+']').attr("src",basicContactInfos.remoteAvatarUrl || basicContactInfos.localAvatar);
        },
        OnGroupInfoReceived: function(group){
            if(group.groupId==this.callInfo.chatId){
                this.callInfo.siteId=group.siteId;
                this.displayNameNode.text(group.name);
                this._updateGroupAvatar(group);
            }

        },
	    close: function(){
	        uc.util.StorageUtil.removeData('floating_call_window_close');
		    uc.util.StorageUtil.setData('floating_call_window_isActivity',false);
            clearInterval(this.countDurationTimer);
	        uc.modules.call.FloatingCallWindow.isInit = false;
            uc.util.WindowUtil.close();
        },
        updateCallInfo:function(callInfo){
            this.callInfo = callInfo;
            this._setCallStatus();
            uc.modules.component.FloatingCallWindowToolBar.updateCallInfo(this.callInfo);
        },
        updateContact:function(user){
            if(user.siteId){
                this.callInfo.siteId=user.siteId;
                uc.modules.component.FloatingCallWindowToolBar.setReply(user);
            }
        },
		updateWindowWH:function(){
            if (this.callInfo.callStatus == uc.constants.Call.CallStatus.INVITED && this.callInfo.hostId != this.callInfo.loginUserId) {
                this.callwindow.height('140px');
                this.toolBarContainer.height('55px');
	            this.toolBarContainer.css('bottom','0');
                this.titleNodeContainer.css('margin-top', '15px');
	            this.bottomBarToggle.hide();
	            this.bottomBarToggleBtn.hide();
            } else {
                this.callwindow.height('124px');
                this.toolBarContainer.height('70px');
	            this.toolBarContainer.css('top','30px');
                this.titleNodeContainer.css('margin-top', '30px');
	            this.bottomBarToggleBtn.show();
            }
		},
        callTurnToEnd:function(){
            this.callStatusTime.text(uc_resource.Call.CallTurnToEnd);
	        clearTimeout(this.countDurationTimer);
            var _this=this;
            var endTimer=setTimeout(function(){
                _this.close();
                clearTimeout(endTimer);
            },1000);
        },
        updateGroupName:function(groupName){
            this.displayNameNode.text(groupName);
        },
	    updateBottomBarStatus:function(callInfo){
		    this.screenSharebtn.parent().show();
		    if(this.bottomBarToggleBtn.is(':hidden') &&!(this.callInfo.callStatus == uc.constants.Call.CallStatus.INVITED && this.callInfo.hostId != this.callInfo.loginUserId)){
			    this.bottomBarToggleBtn.show();
			    this.bottomBarToggle.show();

		    }
		    if(callInfo.desktopShareStatus == uc.constants.Call.DesktopShareStatus.OPEN){
			    this.screenSharebtn.addClass('open');
			    this.screenSharebtn.text(uc_resource.Call.StopDesktopShare);
		    }else{
			    if(callInfo.desktopShareAcceptStatus == uc.constants.Call.DesktopShareStatus.OPEN) {
				    this.screenSharebtn.addClass('disable');
				    this.screenSharebtn.text(uc_resource.Call.StartDesktopShare);
				    this.screenSharebtn.parent().hide();
				    if(this.bottomBarToggle.find('ul li:hidden').length == this.bottomBarToggle.find('ul li').length){
					    this.bottomBarToggleBtn.hide();
					    this.bottomBarToggle.hide();
				    }
			    }else{
				    this.screenSharebtn.removeClass('open');
				    this.screenSharebtn.removeClass('disable');
				    this.screenSharebtn.text(uc_resource.Call.StartDesktopShare);
			    }
		    }
	    }
    };
    jQuery(document).ready(function () {
        uc.init();
        uc.modules.call.FloatingCallWindowManager.init();
    })
})(uc);