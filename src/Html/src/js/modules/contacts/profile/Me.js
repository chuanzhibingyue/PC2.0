/**
 * Created by waouyang on 15/8/4.
 */
(function(uc){
	var uploadTaskMap = new uc.util.HashMap();
	var _this = null;
	uc.modules.profile.Me = {
		init: function(){
			if(this.initilized){
				return;
			}
			_this = this;
			this.initilized = true;
			this._bindEvents();
			this.mainPanel = $('.menu-panel-body.me .menu-panel-body-body ');
		},
		_bindEvents: function(){
			var currentUserInfo = this.getCurrentUserInfo();
			uc.ui.Me.ContentPanelBody.off("click");
			uc.ui.Me.ContentPanelBody.off("blur");

			uc.ui.Me.ContentPanelBody.on("click", ".contact-avatar-edit", function(){
				uc.IUIService.openImageCropperWindow("ChangeMyAvatar", function (args) {
					if (args.okOrCancel) {
						var croppedImagePath = args.croppedImagePath;

//						var userDetail = $('.user-detail');
//						var userId = Number(userDetail.attr("userId"));
						var userId = currentUserInfo.userId;
						uc.modules.contact.ContactManager.uploadContactAvatar(userId, croppedImagePath);
					}
				});
			});

			uc.ui.Me.ContentPanelBody.on("click", ".personal-data", function(){
				$(".user-detail.self .move").addClass('hide');
				$(".user-detail.self .slip-line").show();
				$(".magic-lamp .my-colleagues-wrapper").addClass('hide');
				$(".magic-lamp .profile-detail").show();


			});
			uc.ui.Me.ContentPanelBody.on("click", ".relationship-data", function(){
				$(".user-detail.self .slip-line").hide();
				$(".user-detail.self .move").removeClass('hide');
				$(".magic-lamp .profile-detail").hide();
				$(".magic-lamp .my-colleagues-wrapper").removeClass('hide');

			});

			var clickContactTimer = -1;

			uc.ui.Me.ContentPanelBody.on("click", ".contact-item", function(){
				clearTimeout(clickContactTimer);
				var userId = Number($(this).attr("userid"));
				clickContactTimer = setTimeout(function(){
					 uc.IUIService.showPopupContactProfile(userId);
				 }, 300);
			});

			uc.ui.Me.ContentPanelBody.on("dblclick", ".contact-item", function(){
				clearTimeout(clickContactTimer);
				var userId = Number($(this).attr("userid"));

				if(currentUserInfo && currentUserInfo.userId != userId){
                    uc.IUIService.triggerCrossWindowEvent(uc.constants.UI.CrossWindowEventName.ReOpenCallWindowFormMessage, {chatType: uc.constants.Chat.ChatType.SingleChat, Id: userId});
                    uc.IUIService.openChatWindow(uc.constants.Chat.ChatType.SingleChat, userId, uc.constants.UI.OpenChatWindowSourceModule.Other, null, null, true);
				}
			});

            uc.ui.Me.ContentPanelBody.on('click', '.social', function () {
                var _url = uc.modules.microblog.Microblog.getPageUrl();
                window.open(_url, null, '_blank');
            });

		},
		getCurrentUserInfo: function(){
			var userInfo = uc.IContactService.getCurrentUserFullInfo();
			var userTags = userInfo.userTags;
			if((userTags)&&(userTags.length)){
				this.loadProfileDetail(userInfo);
			}
			return userInfo;
		},
		updateUserInfo:function(newEdit,inputObj){
			uc.IContactService.updateUserInfo(JSON.stringify(newEdit));
			$(".profile-detail .profile-signature").text(newEdit.personal_sign);
			inputObj.hide();
			inputObj.siblings().show();
		},
		loadProfileDetail: function(originalContact){

			uc.modules.contact.ContactManager.addOrUpdateContactToCache(originalContact.userId, originalContact);
			var userInfo = uc.modules.contact.ContactManager.getCachedContact(originalContact.userId);

			var mainPanel = $('.menu-panel-body.me .menu-panel-body-body ');
			mainPanel.empty();

			var template = $('<div class="user-detail self text-selectable"></div>');
			template.attr("userId", userInfo.userId);

			var userDetailDownWrapper = $('<div class="user-detail-down-wrapper"></div>');
			template.append(userDetailDownWrapper);

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
				+'		<div class="rounded-corners contact-avatar-edit"></div>'
				+ '</div>'
				+ '</div>');

			profileWrapper.find('.profile-display-name').text(userInfo.displayName);
			profileWrapper.find('.profile-position').text(userInfo.position);
			profileWrapper.find('.profile-department').text(userInfo.departmentName);
			profileWrapper.find('.avatar').addClass("avatar-" + userInfo.userId);
			profileWrapper.find('.user-status').hide();
	
			if(userInfo.localAvatar){
				profileWrapper.find(".avatar-" + userInfo.userId).attr("src", userInfo.localAvatar);
			}



			template.append(profileWrapper);
			var middleItem = $('<div class="personal-data">'+ uc_resource.Profile.PersonalData +'</div>'
								+'<div class="relationship-data">'+ uc_resource.Profile.Relationship +'</div>'
								+'<div class="user-up-splitter"></div>'
								+'<div class="slip-line"></div>'
								+'<div class="move hide"></div>'
			);
			profileWrapper.append(middleItem);

			var social = $('<div class="social hide">'
				+ '<div class="field">'
				+ '<div class="label"><img src="../images/contacts/social-new.png"></div>'
				+ '</div>'
				+ '</div>');
			userDetailDownWrapper.append(social);

			var upLoadMask = $('<div class="uploadMask"></div>');
			userDetailDownWrapper.append(upLoadMask);

			var detail = $('<div class="profile-detail">'
				+ '<div class="field">'
				+ '<div class="label">' + uc_resource.Profile.Signature + '</div>'
				+ '<div class="value"><span class="profile-signature user-signature text-ellipsis"></span><span class="edit-profile-signature"></span></div>'
				+ '</div>'
				+ '</div>');

			detail.attr("userId", userInfo.userId);
			detail.find('.profile-signature').addClass("user-signature-" + userInfo.userId);
			detail.find('.edit-profile-signature').attr("tagName","personal_sign");
			detail.find('.profile-employee-id').text(userInfo.userId);
			userDetailDownWrapper.append(detail);

			var colleagues = $('<div class="my-colleagues-wrapper hide" userId="'+ userInfo.userId +'">'
				+'<div class="field leaders-field">'
				+ '<div class="label">' + uc_resource.Profile.Leader + '</div>'
				+ '<div class="value">'
				+ '</div>'
				+ '</div>'
				+ '<ul class="leader-list no-margin">'
				+ '</ul>'
				+ '<div class="field colleagues-field">'
				+ '<div class="label">' + uc_resource.Profile.Colleagues + '</div>'
				+ '<div class="value">'
				+ '</div>'
				+ '</div>'
				+ '<ul class="colleague-list no-margin" >'
				+ '</ul>'
				+'</div>');
			//detail.append(colleagues);
			userDetailDownWrapper.append(colleagues);


			var magic = $('<div class="magic-lamp"></div>');
			template.append(magic);
			magic.append(userDetailDownWrapper);

			uc.IContactService.getColleagues(userInfo.userId);

			mainPanel.append(template);
			mainPanel.fadeIn('slow');

			$(".edit-profile-signature").InputBox({type:1,callback:this.updateUserInfo});
			uc.modules.contact.ContactManager.setUserSignature(detail.find('.profile-signature'), userInfo.personalSign);

			//此处为标签
			var userTags = userInfo.userTags;
			if(!userTags){
				return;
			}

			var tags = eval('(' + userTags + ')');
			//先对tags按照order字段进行升序排序
			tags.sort(function(x, y){
				return Number(x.tag_order) > Number(y.tag_order) ? 1:-1;
			});

			//判断是否有code需要做本地化

			for(var k = 0; k < tags.length; k++ ){
				if((tags[k].tag_flag == 0)||((tags[k].is_edit == 0)&&(tags[k].tag_value == ""))){
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
				if (tags[k].is_edit == 1) {
					var editor = $('<span class="edit-profile"></span>');
					editor.attr("tagName",tag_name);
					editor.attr("tagId",tag_id);
					editor.attr("tagValue",tag_value);
					editor.attr("tagLength",tag_length);
					editor.addClass("edit-profile-"+tag_id);
					tagsShow.find(".value").append(editor);
				}
				$(".edit-profile").InputBox({callback:this.updateCustomTags});
			}



			// add by xiege
			if (uc.constants.Switch.OpenMultiVersion) {
				var version = JSON.parse(uc.util.StorageUtil.getData("capability"));
				if (version) {
					/*switch (version.im) {
					}
					switch (version.calendar) {
					}
					switch (version.conf) {
					}
					switch (version.call) {
					}
					switch (version.cloud_disk_type) {
					}
					switch (version.app_center) {
					}
					switch (version.app_moments) {
					}
					switch (version.scan) {
					}
					switch (version.remote_assistant) {
					}*/
				}
			}
            if (uc.constants.Switch.OpenUserStatus) {
                // do something
            }
		},
		updateCustomTags:function(newtag,inputObj) {
			$(".uploadMask").show();
			uc.IContactService.updateCustomTags(newtag);
			inputObj.hide();
			inputObj.siblings().show();
		},
		OnUserTagsUpdated:function(errorCode,UpdatedTags) {
			if(errorCode == 0){
				$(".uploadMask").hide();
			}else{
				uc.modules.component.PromptMessageBox.open({
					title: uc_resource.WindowTitle.Notification,
					message: uc_resource.Profile.GetServerFailed,
					buttonMode: "ok",
					OkText: uc_resource.Common.OK
				}, function (result) {
					if (result === 0) {
						this.close();
						$(".uploadMask").hide();
					}
				});
			}
			if(!this.mainPanel){
				return;
			}
			var tags = eval('(' + UpdatedTags + ')');
			var id = tags[0].tag_id;
			var value = tags[0].tag_value;
			this.mainPanel.find(".edit-profile-"+id).prev(".edit-value").html(value);
		},
		OnContactInfoReceived:function(errorCode, userId, contact){
			if(errorCode == 0){
				var currentUserInfo = this.getCurrentUserInfo();
				if(userId == currentUserInfo.userId){
					uc.util.LogUtil.info("Me","OnContactInfoReceived","errorCode"+errorCode,{userId:userId,contact:contact});
					this.loadProfileDetail(contact[0]);
				}
				return;
			}else{
				uc.modules.component.PromptMessageBox.open({
					title: uc_resource.WindowTitle.Notification,
					message: uc_resource.Profile.GetServerFailed,
					buttonMode: "ok",
					OkText: uc_resource.Common.OK
				}, function (result) {
					if (result === 0) {
						this.close();
						$(".uploadMask").hide();
					}
				});
			}

		}
        /*OnUserStatusReceived: function(userIds, status) {
            $statusDiv = $(".user-status");
			if (errorCode == 0) {
				for (var i in status) {
					if (status[i].userId == this.shownUserId) {
						switch (status[i].status) {
							case 1:
								$statusDiv.find("div").addClass("status-online");
								$statusDiv.find("span").text(uc_resource.UserStatus.OnLine);
								break;
							case 2:
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

        }*/
	};
})(uc);