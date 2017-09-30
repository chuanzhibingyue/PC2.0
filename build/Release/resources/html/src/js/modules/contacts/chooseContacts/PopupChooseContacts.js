/**
 * Created by waouyang on 15/8/4.
 */
(function(uc){
	
	$(document).keyup(function (e) {
		if (e.keyCode == 27) {        	
        	if($('#modalWin').is(":visible")){
        		uc.modules.component.PromptMessageBox.cancelDialog();             	
        	} else {
        		uc.modules.chooseContacts.PopupChooseContacts.cancelSelect();
        	}        	
        }
    });
	
	uc.modules.chooseContacts.PopupChooseContacts = {		
		init: function(){
            this.isSuperGroup = false;
			this.initUI();
			this.bindEvent();
			this.initRightPanel();
		},
		initUI: function(){
			uc.modules.component.TitleBarButton.init();
			uc.modules.chooseContacts.ChooseContactsManager.init();			
		},
		initRightPanel: function() {			
			var option = uc.util.StorageUtil.getData('chooseContacts');
		    var selectedUserIdList = option.selectedUserIdList;

            uc.modules.chooseContacts.ChooseContactsManager.isSuperGroup = option.memberType ? option.memberType : false;
            uc.modules.chooseContacts.ChooseContactsManager.isForwardMessage = option.isForward ? option.isForward : false;
		    //selectedUserIdList support: selectedUserIdList: [1296370, 1449932, 1739, 1358016, 1358986];  selectedUserIdList: [{userId: 1296370}, {userId: 1449932}];
		    if(selectedUserIdList && selectedUserIdList.length > 0){
		    	var contactList = $('.popup .popup-panel-right-content .contact-list');	 
		    	     
        		var cachedContacts = [];
        		var toRequestContactIdList = [];
        		var memberList = [];
				var outUser = [];
        		for(var i = 0; i < selectedUserIdList.length; i++){
        			var user = selectedUserIdList[i];
        			var userId;
        			if(user.userId){
        				userId = user.userId;
        			} else {
						if(typeof user == "string"){
							outUser.push(user);
						}else{
							userId = user;
						}
        			}
        			var member = {};
        			member.userId = userId;        			
        			var cachedContact = uc.modules.contact.ContactManager.getCachedContact(userId);
        			if(cachedContact){
        				cachedContacts.push(cachedContact);
        			} else {
        				toRequestContactIdList.push(userId);
        			}
        			memberList.push(member);
        		}        		
        		uc.modules.group.GroupManager.loadGroupMembersMainUI(contactList, memberList, false);
				contactList.find(".right").remove();
				contactList.find(".contact-item").attr("enable", false);

				if(outUser.length > 0){
					$.each(outUser, function(index, email){
						if(email){
							var defaultAvatar = uc.constants.UI.DefaultLocalUrl.Avatar;
							var template = '<li class="contact-item" enable="false" email="'+email+'">'
									+ '	<div class="contact-widget-container">'
									+ '		<div class="left"><img src="../images/contacts/contact-email-avatar.png" onerror="uc.util.ImageUtil.OnImageLoadError(this,\'' + defaultAvatar + '\')" class="avatar round-image"/></div>'
									+ '		<div class="center">'
									+ '			<div class="contact-display-name text-ellipsis contact-email">'+email+'</div>'
									+ '		</div>'
									+ '	</div>'
									+ '</li>';
							if($(".popup-panel-right-content").find("ul.email-list").length){
								$(".popup-panel-right-content").find("ul.email-list").append(template);
							}else{
								var ul='<ul class="email-list">'+template+'</ul>';
								$(".popup-panel-right-content").append(ul);
							}
						}
					});
				}
        		
	        	if(toRequestContactIdList.length > 0) {
	        		uc.modules.contact.ContactManager.batchRequestMembers(toRequestContactIdList);
	        	}	        	
		    }
            if(option.fromForward){
                this.dealEmailUser();
            }
		    $(".popup-panel-right-content").trigger('selectedChange');
		},
        dealEmailUser:function(){
            this.addEmailUser={};
            var _t=this;
            $(".popup #figuresPanel .figure-email").removeClass('hide');
            $('.popup-panel-left-body').addClass('email');
            $(".popup #figuresPanel figure").addClass("add_email");
            uc.IUIService.bindCrossWindowEvent("contactAddEmail", function (eventData) {
               var emailValue=false;
                for(var email in eventData){
                    emailValue=true;
                   if(email){
                       var defaultAvatar = uc.constants.UI.DefaultLocalUrl.Avatar;
                       var template = '<li class="contact-item" email="'+email+'">'
                           + '	<div class="contact-widget-container">'
                           + '		<div class="left"><img src="../images/contacts/contact-email-avatar.png" onerror="uc.util.ImageUtil.OnImageLoadError(this,\'' + defaultAvatar + '\')" class="avatar round-image"/></div>'
                           + '		<div class="right">'
                           + '         <div class="checkbox-parent check-box-contact">'
                           + '         	<label><span><img class="remove-contact" src="../images/contacts/remove.png"/></span></label>'
                           + '         </div>'
                           + '			'
                           + '		</div>'
                           + '		<div class="center">'
                           + '			<div class="contact-display-name text-ellipsis contact-email">'+email+'</div>'
                           + '		</div>'
                           + '	</div>'
                           + '</li>';
                       if($(".popup-panel-right-content").find("ul.email-list").length){
                           $(".popup-panel-right-content").find("ul.email-list").append(template);
                       }else{
                           var ul='<ul class="email-list">'+template+'</ul>';
                           $(".popup-panel-right-content").append(ul);
                       }
                       _t.addEmailUser[email]={userId:'',displayName:email,account:email};
                       $(".popup-panel-right-content").trigger('selectedChange');
                   }
               }
               if(!emailValue) {
                    $(".popup #figuresPanel .figure-email").removeClass('hide');
				    $('.popup-panel-left-body').addClass('email');
                    $(".popup #figuresPanel figure").addClass("add_email");
                }

            });
        },
		loadSelectedMembersUI: function(contacts) {        	
        	var _self = this;
        	var contactList = $('.popup .popup-panel-right-content .contact-list');        	
        	$.each(contacts,function(k,contact){
        		var userId = contact.userId;
        		var template = contactList.find('.contact-item-' + userId);
        		
        		template.find('.contact-display-name').text(contact.displayName);
        		template.find('.contact-display-name').attr("userId", contact.userId);
        		template.find('.contact-department').text(contact.departmentName);
        		template.find('.contact-position').text(contact.position);
        		if(!contact.departmentName){
        			template.find('.contact-department').hide();
        		}
            	
        		var avatar = template.find('.left .avatar');
            	avatar.addClass("avatar-" + contact.userId);
            	if(contact.localAvatar){
            		avatar.attr("src", contact.localAvatar);
            		avatar.addClass("avatar-loaded");
        		} else {
        			avatar.addClass("avatar-unloaded");
        			//Need call uc.IUIService.downloadUsloadGroupsMembersUIerAvatar(userId, remoteAvatarUrl) to load avatar to localAvatar
        			//Receive event in uc.IUIEvent.OnUserAvatarDownloaded(userId, serverAvatarUrl, localAvatar)
        			if(contact.remoteAvatarUrl){
        				//uc.IUIService.downloadUserAvatar(item.userId, item.remoteAvatarUrl);
        			}
        		}
            	
        	});
		},
		cancelSelect: function() {
			uc.modules.chooseContacts.ChooseContactsManager.clearCache();

		    //lei.zhou add
		    var option = uc.util.StorageUtil.getData('chooseContacts');		    
		    uc.IUIService.triggerCrossWindowEvent(option.key, { okOrCancel: false });

			uc.util.WindowUtil.close();
		},
		bindEvent: function(){
			var _self = this;
			var mainPanel = $("#layoutContainer .popup-panel .popup-panel-body");
			var rightContentPanel = mainPanel.find(".popup-panel-right .popup-panel-right-content");
			var rightFootPanel = mainPanel.find(".popup-panel-right .popup-panel-right-footer");

			rightFootPanel.off("click");
			rightFootPanel.on("click", ".btn-cancel", function(){
				_self.cancelSelect();
			});
			
			rightFootPanel.on("click", ".btn-complete", function(){
				var chooseResult = uc.modules.chooseContacts.PopupChooseContacts.selectedContactsResult();
				
				var allContacts = chooseResult.allContacts;	
				
				var contacts = [];
				$.each(allContacts, function(i, userId){
                    if(typeof userId!="object"){
                        var cachedContact = uc.modules.contact.ContactManager.getCachedContact(userId);
                        if(cachedContact) {
                            contacts.push(cachedContact);
                        }else{
                        	uc.util.LogUtil.info("PopupChooseContacts","bindEvent","userId=" + userId, {});
                            var contactInfo = uc.IContactService.getBasicContactInfo(Number(userId));
                            uc.util.LogUtil.info("getContactInfo","bindEvent","contactInfo=", contactInfo);
                            if(contactInfo) {
                            	contacts.push(contactInfo);
                                uc.modules.contact.ContactManager.addOrUpdateContactToCache(userId, contactInfo);
                            }                            
                        }
                    }else{
                        contacts.push(allContacts[i]);
                    }

				});				
				
				var option = uc.util.StorageUtil.getData('chooseContacts');
			    //lei.zhou add
				uc.IUIService.triggerCrossWindowEvent(option.key, { okOrCancel: true, contacts: contacts, otherItem:  chooseResult});
				
				uc.modules.chooseContacts.ChooseContactsManager.clearCache();
				
				uc.util.WindowUtil.close();
			});
			
			// show the selected contacts number in real time
			rightContentPanel.bind('selectedChange', function(){
				var contactResult = uc.modules.chooseContacts.PopupChooseContacts.selectedContactsResult();
				var allContacts = contactResult.allContacts;
				var buttonText = uc.util.StringUtil.formatString(uc_resource.ChooseContacts.SelectedContactsCountButton, [allContacts.length]);
				rightFootPanel.find(".btn").eq(1).text(buttonText);
				var rightBody = $('.popup-panel-right-body');
				
				if(allContacts.length > 0){
					rightFootPanel.find(".btn").eq(1).removeClass("disable").addClass("btn-complete");
					rightBody.addClass("memmbers-selected").removeClass("no-memmbers-selected");
				}else{
					rightFootPanel.find(".btn").eq(1).removeClass("btn-complete").addClass("disable");
					rightBody.addClass("no-memmbers-selected").removeClass("memmbers-selected");
				}
		      });
			
			// remove right contact
			rightContentPanel.off("click");
			rightContentPanel.on("click", ".contact-item", function(){
				if($(this).attr("enable")){
					return false;
				}
				
                if($(this).attr("email")){
                    var email=$(this).attr("email");
                    delete _self.addEmailUser[email];
                }else{
                    var userId = $(this).attr("userid");
                    uc.modules.chooseContacts.ChooseContactsManager.unCheckContact(userId);
                }
				$(this).remove();
				$(".popup-panel-right-content").trigger('selectedChange');
			});
			
			// remove right project or group
			rightContentPanel.find(".project-list, .group-chat-list").on("click", ".group-item", function(){
				var groupId = $(this).attr("groupid");
				
				$(this).remove();
				uc.modules.chooseContacts.ChooseContactsManager.unCheckGroup(groupId);
				
				$(".popup-panel-right-content").trigger('selectedChange');
			});
			
			// remove right my department
			rightContentPanel.on("click", ".my-department-item", function(){
				var orgId = $(this).find(".department-item").attr("orgid");
				
				$(this).remove();
				uc.modules.chooseContacts.ChooseContactsManager.unCheckMyDepartment(orgId);
				
				$(".popup-panel-right-content").trigger('selectedChange');
			});
			
			//remove orgnization
			rightContentPanel.on("click", ".org-child", function(){
				var orgId = $(this).find(".organization-name").attr("orgid");
				
				$(this).parent().find(".org-parent-" + orgId).remove();
				$(this).remove();
				uc.modules.chooseContacts.ChooseContactsManager.unCheckOrganization(orgId);
				
				$(".popup-panel-right-content").trigger('selectedChange');
			});
			
		},
		checkLeftMyDept: function(classObj){
			var mainPanel = $("#layoutContainer .popup-panel .popup-panel-body");
			var rightContentPanel = mainPanel.find(".popup-panel-right .popup-panel-right-content");
			var rightMyDeptContactLabel = rightContentPanel.find(".my-departments-list");
			
			if(classObj.find(".checkbox-parent span .remove-group").length < 1){
				classObj.find(".checkbox-parent span").append('<img class="remove-group" src="../images/contacts/remove.png" />');
			}
			
			classObj.clone(true).removeClass('hover').appendTo(rightMyDeptContactLabel);
			rightMyDeptContactLabel.find("input").remove();
			
			$(".popup-panel-right-content").trigger('selectedChange');
		},
		unCheckLeftMyDept: function(classObj){
			var orgId = classObj.find(".right").next("div").attr("orgid");
			
			var mainPanel = $("#layoutContainer .popup-panel .popup-panel-body");
			var rightContentPanel = mainPanel.find(".popup-panel-right .popup-panel-right-content");
			var rightMyDeptContactLabel = rightContentPanel.find(".my-departments-list");
			var rightContent = rightMyDeptContactLabel.find(".department-item");
			
			if(orgId == rightContent.attr("orgid")){
				rightContent.parent().parent().remove();
			}
			
			$(".popup-panel-right-content").trigger('selectedChange');
		},
		checkLeftContact: function(classObj){
			var userId = classObj.find(".right").next("div").find(".contact-display-name").attr("userid");
			
			var mainPanel = $("#layoutContainer .popup-panel .popup-panel-body");
			var rightContentPanel = mainPanel.find(".popup-panel-right .popup-panel-right-content");
			var rightActiveContactLabel = rightContentPanel.find(".contact-list");
			
			rightActiveContactLabel.find(".contact-item").each(function(){
				var rightContent = $(this).find(".right").next("div").find(".contact-display-name");
				
				if(userId == rightContent.attr("userid")){
					$(this).remove();
				}
			});
			
			classObj.clone(true).removeClass('hover').appendTo(rightActiveContactLabel);
			rightActiveContactLabel.find("input").remove();
			
			$(".popup-panel-right-content").trigger('selectedChange');
		},
		unCheckLeftContact: function(classObj){
			var userId = classObj.find(".right").next("div").find(".contact-display-name").attr("userid");
			
			var mainPanel = $("#layoutContainer .popup-panel .popup-panel-body");
			var rightContentPanel = mainPanel.find(".popup-panel-right .popup-panel-right-content");
			var rightActiveContactLabel = rightContentPanel.find(".contact-list");
			
			rightActiveContactLabel.find(".contact-item").each(function(){
				var rightContent = $(this).find(".right").next("div").find(".contact-display-name");
				
				if(userId == rightContent.attr("userid")){
					$(this).remove();
				}
			});
			
			$(".popup-panel-right-content").trigger('selectedChange');
		},
		checkLeftGroup: function(classObj){
			var mainPanel = $("#layoutContainer .popup-panel .popup-panel-body");
			var rightContentPanel = mainPanel.find(".popup-panel-right .popup-panel-right-content");
			var rightGroupChat = rightContentPanel.find(".group-chat-list");
			
			classObj.clone(true).removeClass('hover').appendTo(rightGroupChat);
			rightGroupChat.find("input").remove();
			rightGroupChat.css("padding", "0px");
			rightGroupChat.find(".group-name-container").css("width", "150px");
			
			$(".popup-panel-right-content").trigger('selectedChange');
		},
		unCheckLeftGroup: function(classObj){
			var groupId = classObj.find(".right").next("div").find(".group-name").attr("groupid");
			
			var mainPanel = $("#layoutContainer .popup-panel .popup-panel-body");
			var rightContentPanel = mainPanel.find(".popup-panel-right .popup-panel-right-content");
			var rightGroupChat = rightContentPanel.find(".group-chat-list");
			
			rightGroupChat.find(".group-item").each(function(){
				var rightContent = $(this).find(".right").next("div").find(".group-name");
				
				if(groupId == rightContent.attr("groupid")){
					$(this).remove();
				}
			});
			
			$(".popup-panel-right-content").trigger('selectedChange');
		},
		checkLeftProject: function(classObj){
			var mainPanel = $("#layoutContainer .popup-panel .popup-panel-body");
			var rightContentPanel = mainPanel.find(".popup-panel-right .popup-panel-right-content");
			var rightProject = rightContentPanel.find(".project-list");
			
			classObj.clone(true).removeClass('hover').appendTo(rightProject);
			rightProject.find("input").remove();
			rightProject.css("padding", "0px");
			rightProject.find(".group-name-container").css("width", "150px");
			
			$(".popup-panel-right-content").trigger('selectedChange');
		},
		unCheckLeftProject: function(classObj){
			var groupId = classObj.find(".right").next("div").find(".group-name").attr("groupid");
						
			var mainPanel = $("#layoutContainer .popup-panel .popup-panel-body");
			var rightContentPanel = mainPanel.find(".popup-panel-right .popup-panel-right-content");
			var rightProject = rightContentPanel.find(".project-list");
			
			rightProject.find(".group-item").each(function(){
				var rightContent = $(this).find(".right").next("div").find(".group-name");
				
				if(groupId == rightContent.attr("groupid")){
					$(this).remove();
				}
			});
			
			$(".popup-panel-right-content").trigger('selectedChange');
		},
		checkOrgContact: function(classObj){
			var userId = classObj.find(".right").next("div").find(".contact-display-name").attr("userid");
						
			var mainPanel = $("#layoutContainer .popup-panel .popup-panel-body");
			var rightContentPanel = mainPanel.find(".popup-panel-right .popup-panel-right-content");
			var rightActiveContactLabel = rightContentPanel.find(".contact-list");
			
			rightActiveContactLabel.find(".contact-item").each(function(){
				var rightContent = $(this).find(".right").next("div").find(".contact-display-name");
				if(userId == rightContent.attr("userid")){
					$(this).remove();
				}
			});
			
			classObj.children().clone(true).appendTo(rightActiveContactLabel);
			rightActiveContactLabel.find("input").remove();
			rightActiveContactLabel.find(".contact-item").css("padding","0px");
			
			$(".popup-panel-right-content").trigger('selectedChange');
		},
		unCheckOrgContact: function(classObj){
			var userId = classObj.find(".right").next("div").find(".contact-display-name").attr("userid");
						
			var mainPanel = $("#layoutContainer .popup-panel .popup-panel-body");
			var rightContentPanel = mainPanel.find(".popup-panel-right .popup-panel-right-content");
			var rightActiveContactLabel = rightContentPanel.find(".contact-list");
			
			rightActiveContactLabel.find(".contact-item").each(function(){
				var rightContent = $(this).find(".right").next("div").find(".contact-display-name");
				if(userId == rightContent.attr("userid")){
					$(this).remove();
				}
			});
			
			$(".popup-panel-right-content").trigger('selectedChange');
		},
		checkOrg: function(classObj){
			var orgId = classObj.attr("orgid");
			var orgName = classObj.find(".organization-name").text();
			var parentId = classObj.attr("parentorgid");
			var orgClass = classObj.attr("class");
			
			var mainPanel = $("#layoutContainer .popup-panel .popup-panel-body");
			var rightContentPanel = mainPanel.find(".popup-panel-right .popup-panel-right-content");
			var rightOrgLabel = rightContentPanel.find(".org-parent");
			
			var rightOrg = '<li class="org-child ' + orgClass +'" style="cursor:pointer;">' +
								'<div class="organization-widget-container">' + 
									'<div class="left" style="width: 35px;">' +
										'<div class="rounded-corners avatar-my-department" >' +
											'<img src="../images/contacts/avatar_organization.png">' +
										'</div>' +
									'</div>' +
									'<div class="right">' + 
										'<div class="checkbox-parent check-box-organization>' +         	
											'<input type="checkbox">' +         	
											'<label>' +
												'<span><img class="remove-contact" src="../images/contacts/remove.png"></span>' +
											'</label>' +       
										'</div>' +
									'</div>' +
									'<div class="center text-ellipsis organization-name" orgid="' + orgId +'">'+ orgName +'</div>' +
								'</div>' + 
							'</li>';
			
			rightOrgLabel.append(rightOrg);
			
			$(".popup-panel-right-content").trigger('selectedChange');
		},
		unCheckOrg: function(orgId){
			var mainPanel = $("#layoutContainer .popup-panel .popup-panel-body");
			var rightContentPanel = mainPanel.find(".popup-panel-right .popup-panel-right-content");
			var rightOrgLabel = rightContentPanel.find(".org-parent");
			
			rightOrgLabel.find(".org-child").each(function(){
				if($(this).find(".organization-name").attr("orgid") == orgId){
					$(this).remove();
				}
			});
			
			rightContentPanel.find(".org-parent-" + orgId).remove();
			
			$(".popup-panel-right-content").trigger('selectedChange');
		},
		allowMultipleSelect: function(domObject){
			 var option = uc.util.StorageUtil.getData('chooseContacts');
			 
			 var allowMultipleSelect = (option.allowMultipleSelect == undefined) ? true: option.allowMultipleSelect;
			 var allowMultipleSelectConversations = option.allowMultipleSelectConversations ? option.allowMultipleSelectConversations : 0;
			 var allowChooseOrg = (option.allowChooseOrg == undefined) ? true: option.allowChooseOrg;
			 
			 var totalSelected = $(".popup-panel-right-content").find("li");			 
			 
			 if(allowMultipleSelect == false && totalSelected.length >= 1){
				 $($(".popup-panel-left-body").find("li")).find("input[type='checkbox']").prop("checked", false);
				 totalSelected.remove();
			 }

			 if(allowMultipleSelect && allowMultipleSelectConversations && totalSelected.length > 0){
				 if(totalSelected.length >= allowMultipleSelectConversations){
					 var message = uc.util.StringUtil.formatString(uc_resource.ChooseContacts.allowMultipleSelectNumber, [allowMultipleSelectConversations]);
					 uc.modules.component.PromptMessageBox.open({title: uc_resource.ChooseContacts.Tips, message: message, buttonMode:"both", OkText: uc_resource.Common.OK, CancelText: uc_resource.Common.Cancel}, function(result){
						 this.close();
					 });
					 return false;
				 }
			 }
			 
			 if(!allowChooseOrg){
				 if(domObject != null && (domObject.hasClass("org-child") || domObject.hasClass("my-department-item"))){
					 uc.modules.component.PromptMessageBox.open({title: uc_resource.ChooseContacts.Tips, message: uc_resource.ChooseContacts.CannotChooseDepart, buttonMode:"ok", OkText: uc_resource.Common.Known}, function(result){
			        		this.close();
			         });
					 return false;
				 }
			 }
			 
			 return true;
		},
		getChoseProjects: function(){
			var projects = new Array();
			
			var mainPanel = $("#layoutContainer .popup-panel .popup-panel-body");
			var rightContentPanel = mainPanel.find(".popup-panel-right .popup-panel-right-content");
			var rightProject = rightContentPanel.find(".project-list");
			
			rightProject.find(".group-item").each(function(){
				var groupId = $(this).find(".right").next("div").find(".group-name").attr("groupid");
				
				projects.push(Number(groupId));
			});
			
			return projects;
		},
		getChoseGroupChats: function(){
			var groupChats = new Array();
			
			var mainPanel = $("#layoutContainer .popup-panel .popup-panel-body");
			var rightContentPanel = mainPanel.find(".popup-panel-right .popup-panel-right-content");
			var rightProject = rightContentPanel.find(".group-chat-list");
			
			rightProject.find(".group-item").each(function(){
				var groupId = $(this).find(".right").next("div").find(".group-name").attr("groupid");
				
				groupChats.push(Number(groupId));
			});
			
			return groupChats;
		},
		getChoseOragnizations: function(){
			var organizations = new Array();
			
			var mainPanel = $("#layoutContainer .popup-panel .popup-panel-body");
			var rightContentPanel = mainPanel.find(".popup-panel-right .popup-panel-right-content");
			var rightOrgnizationLabel = rightContentPanel.find(".org-child");
			
			rightOrgnizationLabel.each(function(){
				var orgId = $(this).find(".organization-name").attr("orgid");
				
				organizations.push(Number(orgId));
			});
			
			return organizations;
		},
		getChoseMyDepartments: function(){
			var myDepartments = new Array();
			
			var mainPanel = $("#layoutContainer .popup-panel .popup-panel-body");
			var rightContentPanel = mainPanel.find(".popup-panel-right .popup-panel-right-content");
			var rightMyDeptContactLabel = rightContentPanel.find(".my-departments-list");
			
			rightMyDeptContactLabel.find(".my-department-item").each(function(){
				var myDeptId = $(this).find(".department-item").attr("orgid");
				
				myDepartments.push(Number(myDeptId));
			});
			
			return myDepartments;
		},
		getChoseContacts: function(){
			var contacts = new Array();
			
			var mainPanel = $("#layoutContainer .popup-panel .popup-panel-body");
			var rightContentPanel = mainPanel.find(".popup-panel-right .popup-panel-right-content");
			var rightActiveContactLabel = rightContentPanel.find(".contact-list");
			
			rightActiveContactLabel.find(".contact-item").each(function(){
				var userId = $(this).attr("userid");
				
				contacts.push(Number(userId));
			});
						
			return contacts;
		},
		getChoseAllContacts: function(projects, groupChats, organizations, myDepartments, contacts,email){
			var allContacts = new Array();
			
			// project userIds
			if(projects.length > 0){
				$.each(projects, function(i, project){					
					var group = uc.modules.group.GroupManager.getCachedProjectGroup(Number(project));					
					if(group && group != null){						
						$.each(group.members, function(j, member){							
							allContacts.push(member.userId);
						});
					}
				});
			}
			
			// group userIds
			if(groupChats.length > 0){
				$.each(groupChats, function(k, groupChat){					
					var group = uc.modules.group.GroupManager.getCachedChatGroup(Number(groupChat));					
					if(group && group != null){						
						$.each(group.members, function(l, member){							
							allContacts.push(member.userId);
						});
					}
				});
			}
			
			// organization userIds
			if(organizations.length > 0){
				$.each(organizations, function(m, organization){
					var cachedOrg = uc.modules.chooseContacts.ChooseContactsManager.getCachedOrganizations();
					var orgInfo = uc.util.CollectionUtil.findObjectByKey(cachedOrg, "orgId", organization);
					var staffs = orgInfo.memberIds;
					
					$.each(staffs, function(n, staff){						
						allContacts.push(staff);
					});
				});
			}
			
			// myDept userIds
			if(myDepartments.length > 0){
				$.each(myDepartments, function(x, myDepartment){
					var cachedMyDept = uc.modules.chooseContacts.ChooseContactsManager.getCachedMyDepartments();
					var myDepartmentInfo = uc.util.CollectionUtil.findObjectByKey(cachedMyDept, "id", Number(myDepartment));
					var staffs = myDepartmentInfo.staffs; 
					
					$.each(staffs, function(y, staff){						
						allContacts.push(staff.userId);
					});
				});
			}
			
			// contact userIds
			if(contacts.length > 0){
				$.each(contacts, function(z, contact){					
					allContacts.push(contact);
				});
			}

            if(email){
                $.each(email, function(z, user){
                    allContacts.push(user);
                });
            }
			// distict userIds
			allContacts = $.unique(allContacts);
			
			return allContacts;
		},
		getEmailUser: function(){
			var emailUsers = new Array();

			var mainPanel = $("#layoutContainer .popup-panel .popup-panel-body");
			var rightContentPanel = mainPanel.find(".popup-panel-right .popup-panel-right-content");
			var rightActiveContactLabel = rightContentPanel.find(".email-list");
			var rightActiveContactLabel = rightContentPanel.find(".email-list");

			rightActiveContactLabel.find(".contact-item").each(function(){
				var email = $(this).attr("email");
				emailUsers.push({userId:'',displayName:email,account:email});
			});

			return emailUsers;
		},
		selectedContactsResult: function(){
			var _self = this;
			var result_json = {};
			var projects = _self.getChoseProjects();
			var groupChats = _self.getChoseGroupChats();
			var organizations = _self.getChoseOragnizations();
			var myDepartments = _self.getChoseMyDepartments();
			var contacts = _self.getChoseContacts();
            //var addEmail= _self.addEmailUser || {};
			var addEmail= _self.getEmailUser();
            var allContacts = _self.getChoseAllContacts(projects, groupChats, organizations, myDepartments, contacts, addEmail);
			result_json = {
					"projects":	projects,
					"groupChats": groupChats,
					"organizations": organizations,
					"myDepartments": myDepartments,
					"contacts": contacts,
					"allContacts": allContacts,
                    "addEmail":addEmail
			};		
			
			return result_json;
		}
	};
	jQuery(document).ready(function(){        
		uc.init();
	    uc.modules.chooseContacts.PopupChooseContacts.init();
    });
})(uc);