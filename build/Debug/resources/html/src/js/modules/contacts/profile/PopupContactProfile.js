/**
 * Created by waouyang on 15/8/4.
 */
(function(uc){
	var uploadTaskMap = new uc.util.HashMap();
	var _this = null;
	var mobile;
	var userName;
	$(document).keyup(function (e) {
		if (e.keyCode == 27) {
			uc.util.WindowUtil.close();      	
        }
    });
	
	uc.modules.profile.PopupContactProfile = {
        currentUserStatus:0,
		init: function(){
			this.shownUserId = uc.util.StorageUtil.getData('popup_contact_user_id');
			uc.util.LogUtil.info("PopupContactProfile","init","shownUserId=" + this.shownUserId, null);
			this.initUI();
			this.getContactInfo(this.shownUserId);
			this.mainPanel = $('.menu-panel-body.popup .menu-panel-body-body ');
			this._bindEvents();
		},
		initUI: function(){
			uc.modules.component.TitleBarButton.init();
			uc.ui = {
				Popup: {
					ModuleId: uc.constants.UI.ModuleId.Popup,
	                MainPanel: $('#layoutContainer .menu-panel-body.popup'),
	                ContentPanelTitle: $('#layoutContainer .menu-panel-body.popup .menu-panel-body-title'),
	                ContentPanelBody: $('#layoutContainer .menu-panel-body.popup .menu-panel-body-body ')
				}	
			};
		},
		_bindEvents: function(){
            var _this=this;
			uc.ui.Popup.ContentPanelBody.off("click");
			uc.ui.Popup.ContentPanelBody.on("click", ".contact-avatar-edit", function(){
				uc.IUIService.openImageCropperWindow("ChangePopupMyAvatar", function (args) {        			
                    if (args.okOrCancel) {
                    	var croppedImagePath = args.croppedImagePath;
                    	
                    	var userDetail = $('.user-detail');
    	        		var userId = Number(userDetail.attr("userId"));    	        		
    	        		userDetail.find(".avatar-" + userId).attr("src", croppedImagePath);
    	        		
    	        		uc.modules.contact.ContactManager.uploadContactAvatar(userId, croppedImagePath);
                    }
        		}); 
			});
			
			uc.ui.Popup.ContentPanelBody.on("click", ".attentioned", function(){   
				uc.modules.contact.ContactManager.canelAttention();
        	});
			uc.ui.Popup.ContentPanelBody.on("click", ".add-attention", function(){   
				uc.modules.contact.ContactManager.addAttention();
        	});

			var clickContactTimer = -1;
			uc.ui.Popup.ContentPanelBody.on("click", ".contact-item", function(){
				clearTimeout(clickContactTimer);
				var userId = Number($(this).attr("userid"));
				clickContactTimer = setTimeout(function(){
					uc.IUIService.showPopupContactProfile(userId);
				}, 300);
			});

			var currentUserInfo = uc.IContactService.getCurrentUserInfo();
			uc.ui.Popup.ContentPanelBody.on("dblclick", ".contact-item", function(){
				clearTimeout(clickContactTimer);
				var userId = Number($(this).attr("userid"));
				if(currentUserInfo.userId != userId){
                    uc.IUIService.triggerCrossWindowEvent(uc.constants.UI.CrossWindowEventName.ReOpenCallWindowFormMessage, {chatType: uc.constants.Chat.ChatType.SingleChat, Id: userId});
                    uc.IUIService.triggerCrossWindowEvent(uc.constants.UI.CrossWindowEventName.OpenChatWindow, {chatType: uc.constants.Chat.ChatType.SingleChat, Id: userId});
					uc.util.WindowUtil.close();
				}
			});

            uc.ui.Popup.ContentPanelBody.on("click", ".personal-data", function(){
                $(".move").addClass('hide');
                $(".slip-line").show();
                $(".colleagues-wrapper").addClass('hide');
                $(".profile-detail").show();


            });
            uc.ui.Popup.ContentPanelBody.on("click", ".relationship-data", function(){
                $(".slip-line").hide();
                $(".move").removeClass('hide');
                $(".profile-detail").hide();
                $(".colleagues-wrapper").removeClass('hide');

            });
			uc.ui.Popup.ContentPanelBody.on("click", ".send-message", function() {
        		var actionsBar = $(this).closest('.actions-bar');
        		var userId = actionsBar.attr("userId");
                uc.IUIService.triggerCrossWindowEvent(uc.constants.UI.CrossWindowEventName.ReOpenCallWindowFormMessage, {chatType: uc.constants.Chat.ChatType.SingleChat, Id: userId});
                uc.IUIService.triggerCrossWindowEvent(uc.constants.UI.CrossWindowEventName.OpenChatWindow, {chatType: uc.constants.Chat.ChatType.SingleChat, Id: userId});
        		uc.util.WindowUtil.close();
        	});
            uc.ui.Popup.ContentPanelBody.on("click", ".view-call figcaption ", function(e) {
	            var actionsBar = $(this).parents('.actions-bar');
	            var userId = actionsBar.attr("userId");
                var phoneNum;
                if(_this.getPBXVisible()){
                    phoneNum=uc.util.StringUtil.IsPBXTelNumber(mobile);
                }
	            uc.util.MenuUtil.showStartCallContextMenu(e, phoneNum,_this.getCallVisible(),function(type,data){
		            if(type == 0){
			            uc.IUIService.triggerCrossWindowEvent(uc.constants.UI.CrossWindowEventName.ReOpenCallWindowFormMessage, {chatType: uc.constants.Chat.ChatType.SingleChat, Id: userId});
			            uc.IUIService.triggerCrossWindowEvent(uc.constants.UI.CrossWindowEventName.OpenChatWindow, {chatType: uc.constants.Chat.ChatType.SingleChat, Id: userId, isFromCall:true});
		            }else{
                        uc.IUIService.triggerCrossWindowEvent(uc.constants.UI.CrossWindowEventName.ReOpenPbxChatDetail, {"userName":userName,"userID":userId,"Tel":data});
		            }
		            uc.util.WindowUtil.close();
	            });
            });
            uc.ui.Popup.ContentPanelBody.on("click", ".social-figcaption", function(){
                var currentUserInfo = uc.IContactService.getCurrentUserInfo();
                var targetId = $(this).parents('.user-detail').attr('userid');
                if (currentUserInfo.userId == targetId) {
                    var _url = uc.util.StorageUtil.getData('currentHomeUrl');
                    window.open(_url, null, '_blank');
                } else {
                    uc.IMicroblogService.getUserhomePageURL(parseInt(currentUserInfo.userId), parseInt(targetId), function (args) {
                        var _url = args[1];
                        window.open(_url, null, '_blank');
                    });
                }
            });
			
			uc.ui.Popup.ContentPanelBody.on("click", ".view-calendar figcaption", function() {
				var actionsBar = $(this).parents('.actions-bar');
        		var userId = Number(actionsBar.attr("userId"));
				var currentUserInfo = uc.IContactService.getCurrentUserInfo();
				var currentUserId = currentUserInfo.userId;
        		var userIdArray = [];
        		userIdArray.push(userId);
				userIdArray.push(currentUserId);
				if(userId && currentUserId){
					userIdArray.push(Number(userId));
					userIdArray.push(Number(currentUserId));
				} else {
					var groupId = Number(actionsBar.attr("groupId"));
					var group = uc.modules.group.GroupManager.findCachedGroupByGroupId(groupId);

					if(group != null){
						var members = group.members;
						$.each(members,function(key, member){
							var userId = member.userId;
							userIdArray.push(userId);
						});
					}
				}

				var param = {
					shareUserId: uc.constants.Meeting.AuthorizeMySelf,
					isCreate: true,
					users: userIdArray
				};

				uc.IUIService.createMeeting("PopupChatCreateMeeting", param);
        	});

            uc.ui.Popup.ContentPanelBody.on("click", ".icon-close", function() {
                if (uc.constants.Switch.OpenUserStatus) {
                    // do something
                    uc.IContactService.unsubscribeUserStatus([this.shownUserId]);
                }
            });
		},

		getContactInfo: function(userId){
			var contact = uc.IContactService.getContactInfo(Number(userId));
			if(contact){
				this.loadProfileDetail(contact);
			}

		},
		loadProfileDetail: function(contact){
			if(contact && this.shownUserId == contact.userId){
				var currentUserInfo = uc.IContactService.getCurrentUserInfo();
				if(currentUserInfo){
					var mainPanel = uc.ui.Popup.ContentPanelBody;
					mainPanel.empty();
				}
	        	
	        	var template = $('<div class="user-detail contact text-selectable"></div>');
	        	template.attr("userId", contact.userId);
	        	
	        	var _isSelf = contact.userId == currentUserInfo.userId;
	        	if(_isSelf){
	        		template.addClass("self");
	        	}
	        	
	        	var defaultAvatar = uc.constants.UI.DefaultLocalUrl.Avatar;
	        	
	        	var profileWrapper = $('<div class="profile-wrapper">'
					    + '<div class="left-content">'
					        + '<div class="profile-display-name"></div>'
							+ '<div class="user-status"><div></div><span></span></div>'
					        + '<div class="profile-info">'
                                + '<div class="profile-position"></div>'
							    + '<div class="profile-department"></div>'
                            + '</div>'
					    + '</div>'     
					    + '<div class="profile-avatar">'
					    + '    <img src="' + defaultAvatar + '" onerror="uc.util.ImageUtil.OnImageLoadError(this,\'' + defaultAvatar + '\')" width="100px" height="100px" class="avatar round-image">'
					    + '</div>'    
					+ '</div>');
	        	
	        	profileWrapper.find('.profile-display-name').text(contact.displayName);
	        	profileWrapper.find('.profile-position').text(contact.position);
	        	profileWrapper.find('.profile-department').text(contact.departmentName);
	        	
	        	var avatar = profileWrapper.find(".profile-avatar img");
				avatar.addClass("avatar-" + contact.userId);
				if(contact.localAvatar){
					avatar.attr("src", contact.localAvatar);
		            avatar.addClass("avatar-loaded");
				}else{
					avatar.addClass("avatar-unloaded");
					if(contact.remoteAvatarUrl){
						uc.IUIService.downloadUserAvatar(contact.userId, contact.remoteAvatarUrl);
					}
				}
				
				if(_isSelf){
	        		var contactAvatarEdit = $('<div class="rounded-corners contact-avatar-edit">'    					
	    					+ '</div>');
	            	profileWrapper.append(contactAvatarEdit);
	        	} else {
	        		var displayNameElement = profileWrapper.find('.profile-display-name');        		
	        		var attentionElement = $('<div class="attention"></div>');        		      		
	        		displayNameElement.append(attentionElement);
	        		uc.IMicroblogService.getRelation(contact.userId, function(args) {
	        			var errorCode = args[0];
	        			if(errorCode == 0){
	        				var attentionStatus = args[1];  
	        				uc.modules.contact.ContactManager.onAttentionChanged(contact.userId, attentionStatus);
	        			}         			
	        		});
	        	}
	        	
	        	template.append(profileWrapper);
	        	
	        	var actionsBar = $('<div class="actions-bar"></div>');
	        	actionsBar.attr("userId", contact.userId);
	        	template.append(actionsBar);



				//鍛?
                if(this.getPBXVisible() ||  this.getCallVisible()){
                    var callFigure = $('<figure class="view-call">'
                        + '<figcaption class="view-call-figcaption" qtips="'+ uc_resource.Menu.Call +'"></figcaption>'
                        + '</figure>');
                    actionsBar.append(callFigure);
                }
	        	var checkSchedule = $('<figure class="view-calendar hide">'
						+ '<figcaption class="meeting-figcaption"></figcaption>'
						+ '</figure>');

	        	actionsBar.append(checkSchedule);
				actionsBar.find(".meeting-figcaption").attr({"qtips":uc_resource.Meeting.CreateCalendar});

				var social = $('<figure class="field hide">'
					+ '<figcaption class="social-figcaption"></figcaption>'
					+ '</figure>');
				actionsBar.append(social);

				var middleItem = $('<div class="personal-data">'+ uc_resource.Profile.PersonalData +'</div>'
					+'<div class="relationship-data">'+ uc_resource.Profile.Relationship +'</div>'
					+'<div class="user-up-splitter"></div>'
					+'<div class="slip-line"></div>'
					+'<div class="move hide"></div>'
				);
				actionsBar.append(middleItem);

               /* var sendMessage = $('<div class="send-content"><button class="send-message" >'+ uc_resource.Profile.SendMessage +'</button>' +
                    '<button class="send-call" >'+ uc_resource.Profile.SendCall +'</button></div>');
                actionsBar.append(sendMessage);*/
				var sendMessage = $('<div class="send-content"><button class="send-message" >'+ uc_resource.Profile.SendMessage +'</button></div>');
				actionsBar.append(sendMessage);
	        	
	        	var detail = $('<div class="profile-detail">'
	    	            + '<div class="field">'
	    	                + '<div class="label">' + uc_resource.Profile.Signature + '</div>'
	    	                + '<div class="value profile-signature text-ellipsis"></div>'
	    	        + '</div>');
	        	detail.attr("userId", contact.userId);
                uc.modules.contact.ContactManager.setUserSignature(detail.find('.profile-signature'), contact.personalSign);
	        	detail.find('.profile-account').text(contact.account);
	        	detail.find('.profile-mobile').text(contact.mobile);
	        	detail.find('.profile-email').text(contact.email);
	        	detail.find('.profile-work-phone').text(contact.workPhone);
	        	detail.find('.profile-employee-id').text(contact.userId);
			mobile = contact.mobile;
			userName = contact.displayName;
	        	if(contact.personalSign.length == 0){
	        		detail.find('.profile-signature').parent().hide();
	        	}
	        	
	        	if(contact.workPhone.length == 0){
	        		detail.find('.profile-work-phone').parent().hide();
	        	}

				var colleagues = $('<div class="colleagues-wrapper hide" userId="'+ contact.userId +'">'
                    +'<div class="field leaders-field">'
						+ '<div class="label">' + uc_resource.Profile.Leader + '</div>'
						+ '<div class="value">'
						+ '</div>'
					+ '</div>'		
					+ '<ul class="leader-list">'						
					+ '</ul>'
					+ '<div class="field colleagues-field">'
						+ '<div class="label">' + uc_resource.Profile.Colleagues + '</div>'
						+ '<div class="value">'
						+ '</div>'
					+ '</div>'
					+ '<ul class="colleague-list">'							
					+ '</ul>'
                    +'</div>');
	        	
	        	var magic = $('<div class="magic-lamp"></div>');
	        	if(contact.userId == currentUserInfo.userId){
	        		var contactAvatarEdit = $('<div class="rounded-corners contact-avatar-edit">'    					
	    					+ '</div>');
	    			profileWrapper.append(contactAvatarEdit);
	        		
	        		actionsBar.css("display", "none");
	        		magic.css("top", "140px");
	        	}else{
	        		if(!contact.workPhone){
	            		detail.find('.profile-work-phone').hide();
	            	}
	        	}

				// add by xiege
				template.hasClass('self') ? template.find('.user-status').remove(): null;
				if (uc.constants.Switch.OpenMultiVersion) {
					var version = JSON.parse(uc.util.StorageUtil.getData("capability"));
					if (version) {
						switch (version.im) {
							case "0":
								sendMessage.find(".send-message").addClass("hide");
								break;
						}
						switch (version.calendar) {
							case "0":
								checkSchedule.addClass("hide");
								break;
						}
						/*switch (version.conf) {
						 }*/
						 switch (version.call) {
							 case "0":
								 sendMessage.find(".send-call").addClass("hide");
								 $(".view-call").addClass("hide");
						 }
						 /*switch (version.cloud_disk_type) {
						 }*/
						switch (version.app_center) {
							case "0":
								social.addClass("hide");
								break;
						}
						switch (version.app_moments) {
							case "0":
								attentionElement && attentionElement.addClass("hide");
								break;
						}
						/*switch (version.scan) {
						 }
						 switch (version.remote_assistant) {
						 }*/
					}

					if (!_isSelf) {
						if(!this.getCalendarVisible()){
							checkSchedule.addClass("hide");
						}
						//判断呼或者pbx的权限，更改通话按钮显示状态
						if(this.getCallVisible()) {
							$(".view-call").removeClass("hide");
						}else{
							if(!this.getPBXVisible()){
								$(".view-call").addClass("hide");
							}
						}
					}
				}
                if (uc.constants.Switch.OpenUserStatus) {
                    // do something
                    uc.IContactService.subscribeUserStatus([contact.userId]);
                }

	        	template.append(magic);
	        	magic.append(detail);
                magic.append(colleagues);

				var userTags = contact.userTags;
				if(!userTags){
					return;
				}

				var tags = eval('(' + userTags + ')');
				//先对tags按照order字段进行升序排序
				tags.sort(function(x, y){
					return  Number(x.tag_order) > Number(y.tag_order) ? 1:-1;
				});

				//判断是否有code需要做本地化
				for(var k = 0; k < tags.length; k++ ){
					if((tags[k].tag_flag == 0)||(tags[k].tag_value == "")){
						continue;
					}
					var arr = tags[k].tag_code;
					if(arr){
						switch(arr){
							case "tag_u_display_name":
								var tag_name = uc_resource.Profile.Personal[uc.constants.Profile.Me.DisplayNameIndex].tag_u_display_name;
								break;
							case "tag_u_mobile":
								var tag_name = uc_resource.Profile.Personal[uc.constants.Profile.Me.PhoneIndex].tag_u_mobile;
								break;
							case "tag_u_email":
								var tag_name = uc_resource.Profile.Personal[uc.constants.Profile.Me.EmailIndex].tag_u_email;
								break;
							default :
								var tag_name = tags[k].tag_name;
								break;
						}
					}else{
						var tag_name = tags[k].tag_name;
					}
					var tag_value = tags[k].tag_value;
					var tag_id = tags[k].tag_id;
					var tag_length = tags[k].tag_value_length;

					detail.find(".label").each(function(){
						var self = $(this);
						if(self.html() == tag_name){
							self.parent().remove();
						}
					});

					var tagsShow = $('<div class="field">'
						+ '<div class="label">' + tag_name + '</div>'
						+ '<div class="value"><span class="text-ellipsis edit-value">' + tag_value + '</span>'
						+ '</div>'
						+ '</div>');
					detail.append(tagsShow);

				}



				uc.IContactService.getColleagues(contact.userId);
	        	mainPanel.append(template);
	        	mainPanel.fadeIn('slow');        	
			
			}
		},
        OnUserStatusReceived: function(errorCode, status) {
            $statusDiv = $(".contact .user-status");
            if (errorCode == 0) {
				for (var i in status) {
				    if (status[i].userId == this.shownUserId && this.shownUserId != uc.IContactService.getCurrentUserInfo().userId) {
                        $statusDiv.find("div").removeClass();
                        this.currentUserStatus = status[i].status;
                        switch (status[i].status) {
                            case 1:
                                $statusDiv.find("div").addClass("status-online");
                                $statusDiv.find("span").text(uc_resource.UserStatus.OnLine);
                                break;
                            case 2:
				$statusDiv.find("div").addClass("status-busy");
				$statusDiv.find("span").text(uc_resource.UserStatus.Busy);
                                break;
                            case 3:
                                break;
                            case 4:
                                $statusDiv.find("div").addClass("status-incall");
                                $statusDiv.find("span").text(uc_resource.UserStatus.InMeeting);
                                break;
                            case 5:
                                $statusDiv.find("div").addClass("status-offline");
                                $statusDiv.find("span").text(uc_resource.UserStatus.OffLine);
                                break;
                            case 6:
                                $statusDiv.find("div").addClass("status-incall");
                                $statusDiv.find("span").text(uc_resource.UserStatus.InCall);
                                break;
                        }
					}
                    break;
				}
			}
        },

		getPBXVisible: function(){
			var version=JSON.parse(uc.util.StorageUtil.getData("capability"));
            return version?version.pbx && version.pbx==1:false;
		},

		getCallVisible: function(){
			var version = JSON.parse(uc.util.StorageUtil.getData("capability"));
			return version?version.call && version.call==1:false;
		},

		getCalendarVisible: function () {
			var version = JSON.parse(uc.util.StorageUtil.getData("capability"));
			return version?version.calendar && version.calendar==1:false;
		}

	};
	jQuery(document).ready(function(){
        uc.init();
	    uc.modules.profile.PopupContactProfile.init();
    });
})(uc);