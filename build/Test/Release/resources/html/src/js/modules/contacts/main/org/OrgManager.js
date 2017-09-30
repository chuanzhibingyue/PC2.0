
(function(uc){
    uc.modules.org.OrgManager = {
		selectedUserIdList: [],
        init: function(selectedUserIdList, isContact){
			if(!this.initilized){
				this.initilized = true;
				this.isContact = isContact;
				this.selectedUserIdList = selectedUserIdList;
	        	this._bindEvents();
			}			
        	uc.IContactService.getOrgInfo(0);
        },      
        _bindEvents: function() {
        	var _self = this;
	        var orgparent = $('.org-parent');

        	//Expand organization item, need call back-end, when OnOrgInfoReceived event, call loadOrganizationPanel
	        orgparent.on("click", ".arrow-right", function(e) {
        		var clickedItem = $(this);
        		var orgElement = clickedItem.parents('.organization-item');

        		_self.expandOrganization(orgElement);
                e.stopPropagation();
        	});

        	//When item is expanded, after clicked, collapse it
	        orgparent.on("click", ".arrow-down", function(e) {
        		var clickedItem = $(this);
        		var orgElement = clickedItem.parents('.organization-item');

        		_self.collapseOrganization(orgElement);
                e.stopPropagation();
        	});

        	$('#layout-container .org-parent').on("click", ".organization-item", function() {
        		var orgElement = $(this);

        		_self.itemClickOrganization(orgElement);
        	});

	        orgparent.on("click", ".staff-item", function() {
        		var userId = $(this).attr("userId");        		
        		var contact = uc.IContactService.getBasicContactInfo(Number(userId));

        		if(typeof(contact) != "undefined") {        			
        			var template = uc.modules.contact.ContactManager.loadContactDetail(contact);
        		}
        	});

			$('.panel-organization').scroll(function () {
				var domLeft = 243;

				$(this).find('.right').css('left', domLeft + $(this).scrollLeft());
				$(this).find('.staff-child .right').css('left', domLeft + 10 + $(this).scrollLeft());
			});

	        //rightClick
	        orgparent.on("contextmenu", ".organization-item", function(e) {
		        var orgId = $(this).attr('orgid');
		        var _uc = uc;

		        uc.util.MenuUtil.showContactCreateGroupCallContextMenu(e, function(){
			        //Get Organzation's All Members(Including Sub Organizations)
			        var copyBack = '{"managerType":"uc.modules.org.OrgManager"}';
			        _uc.IContactService.getOrgAllMembers(parseInt(orgId),parseInt(_uc.IClientService.getMinGroupMemberLimit()),copyBack);
		        });
	        });
        },
	    itemClickOrganization: function(orgElement) {
        	var _self = this;
    		var arrowIcon = orgElement.find('.left i');

    		if(arrowIcon.hasClass("arrow-right")) {    			
    			_self.expandOrganization(orgElement);
    		} else {    			
    			_self.collapseOrganization(orgElement);
    		}    		
        },
        expandOrganization: function(orgElement){
    		var currentLevel = orgElement.attr("level");
    		var orgId = orgElement.attr("orgId");
            var i = $('<i class="arrow-down"></i>');
            var arrowIcon = orgElement.find('.arrow-right');

    		if(orgElement.attr("loaded")) {
    			var rootParent = orgElement.parents('.org-parent');
    			var children = rootParent.find('.org-parent-' + orgId);

    			$.each(children,function(k,child){//遍历数组
    				var childElement = $(child);
    				var childLevel = $(this).attr("level");
    				var parentOrgId  = childElement.attr("parentOrgId");
    				var parentElement  = rootParent.find('.org-item-' + parentOrgId);

    				if((childLevel - currentLevel == 1) || (!childElement.hasClass("collapsed") && parentElement.is(":visible")) ) {
    					childElement.parent().show();
    					childElement.removeClass("collapsed");
    				}
    			});
    		} else {    			       			
    			uc.IContactService.getOrgInfo(Number(orgId));
    		}        		

    		arrowIcon.replaceWith(i);
        },
        collapseOrganization: function(orgElement){    		
    		var currentLevel = orgElement.attr("level");
    		var orgId  = orgElement.attr("orgId");
    		var children = $('.org-parent .org-parent-' + orgId);
            var i = $('<i class="arrow-right"></i>');
            var arrowIcon = orgElement.find('.arrow-down');
    		
    		$.each(children,function(k,child){
    			var childElement = $(child);
    			var childLevel = $(this).attr("level");

    			if(childLevel - currentLevel == 1) {
    				childElement.addClass("collapsed");
    			}

    			childElement.parent().hide();
    		});

    		arrowIcon.replaceWith(i);
        },
        loadMyDepartmentPanel: function(parentPanel, department) {
        	//Now for a user, only belongs to one organization; later, may belong to multiple organizations.
        	var departmentItem = parentPanel.find('.my-departments-list .my-department-item .department-item:first');
        	departmentItem.text(department.departmentName);
        	departmentItem.attr("orgId", department.id);
        	var myDepartment = departmentItem.parent('.department-widget-container').parent('.my-department-item');
        	myDepartment.addClass("my-department-item-" + department.id);
        	myDepartment.find('.checkbox-parent').addClass("check-box-my-department-" + department.id);
        	uc.modules.chooseContacts.ChooseContactsManager.addMyDepartmentToCache(department);        	
        },
        loadOrganizationPanel: function(department, currentUserInfo) {
        	var _self = this;
        	var parentOrgId = department.parentId;
        	var id = department.id;        	
        	var parentPanel = uc.modules.contact.ContactManager.getLatestClickedContactsPanel();
            var lastElement;

        	if(parentPanel == null){
        		return;
        	}
        	        	
        	if(parentOrgId == 0){
        		//This is root organization        		 
        		var parentOrgElement = parentPanel.find('.panel-organization .org-parent');         		
        		if(parentOrgElement.children().length > 0) {
        			return;
        		}

        		//Append root organization
        		var rootOrganization = _self._getOrganizaitionItemView(department, true, 0);
        		parentOrgElement.append(rootOrganization);
        		rootOrganization.find('.org-item:first').attr("loaded", true);
        		
                lastElement = rootOrganization;

        		if(department.staffs.length > 0){
        			lastElement = _self._getStaffListView(rootOrganization, department.staffs, id, 0, currentUserInfo);
        		}
        		
        		//Append sub organizations
        		_self._getOrganizaitionListView(lastElement, department.subDepartments);
        		 
        	} else {
        		//Find clicked organization
        		var clickedOrgElement = parentPanel.find('.panel-organization .org-item-' + id);
        		if(clickedOrgElement.attr("loaded")) {
        			return;
        		}

        		clickedOrgElement.attr("loaded", true);
        		var level = clickedOrgElement.attr("level");
        		lastElement = clickedOrgElement.parent('.org-child');

        		//Append members
        		if(department.staffs.length > 0){
        			lastElement = _self._getStaffListView(lastElement, department.staffs, id, level, currentUserInfo);
        		}

        		//Append sub organizations
        		_self._getOrganizaitionListView(lastElement, department.subDepartments);        		 
        	}        	
        	_self.paddingRows();
        },      
        paddingRows: function() {
        	var parentOrgElement = $('.org-parent');
        	var rows = parentOrgElement.find('li:visible');        	
        	var maxLevel = 0;
        	for(var i = 0; i < rows.length; i++){
        		var row = $(rows[i]);
        		var level = Number(row.attr("level"));
        		if(level > maxLevel) {
        			maxLevel = level;
        		}
        	}        	
        	for(var i = 0; i < rows.length; i++){
        		var row = $(rows[i]);
        		var level = Number(row.attr("level"));
        		var difference = maxLevel - level;
        		if(row.hasClass("contact-item")){
        			difference = maxLevel - level + 1;
        		}
        		var paddingRight = difference * 20;        		
        		row.css("padding-right", paddingRight + "px");
        	}
        },
        _getOrganizaitionListView: function(lastElement, departments) {
        	var _self = this;        	
        	$.each(departments,function(k,department) {
            	var departmentView = _self._getOrganizaitionItemView(department, false);            	
            	departmentView.insertAfter(lastElement);
            	lastElement = departmentView;
            });
        	return lastElement;
        },        
        _getOrganizaitionItemView: function(department, isExpanded, level) {
        	var _self = this;
        	
        	var parentOrgId = department.parentId;
        	var parentLevel = -1;
        	var parentElement = $('.org-item-' + parentOrgId);
        	var parentClassnameList = [];
        	if(parentElement.length > 0) {
        		parentLevel = parentElement.attr("level");
        		var parentClassnames = parentElement.attr("class");
        		parentClassnameList = parentClassnames.split(" ");        		       		
        	}
        	var level = Number(parentLevel) + 1;
        	var id = department.id;   
        	
        	var orgChildElement = $('<div class="org-child"></div>');        	
        	var orgElement = _self._getOrganizaitionTemplate(isExpanded);
        	orgElement.addClass("org-item-" + id);
        	orgElement.find('.checkbox-parent').addClass("check-box-organization-" + id);
        	
        	//var orgElement = $('<div class="org-item item org-item-' + id + '"></div'); 
        	orgElement.attr("orgId", id); 
        	orgElement.attr("parentOrgId", parentOrgId); 
        	orgElement.attr("level", level); 
        	//orgElement.addClass("item-" + level);
        	var paddingLeft = level * 20;
        	orgElement.css("padding-left", paddingLeft + "px");
        	//orgElement.css("padding-right", "-" + paddingLeft + "px");
        	orgElement.addClass("org-parent-" + parentOrgId);
        	$.each(parentClassnameList, function(k, classname){
        		if(classname != " " && classname.indexOf("org-parent-") >=0){
        			orgElement.addClass(classname);
        		}
    		});
        	
        	orgElement.find('.organization-name').text(department.departmentName);
        	
        	//If parent Organization is checked, then sub Organizations and Staff should be checked and disabled
        	var parentCheckbox = parentElement.find("input[type='checkbox']");
    		if(parentCheckbox.is(":checked")){
    			var checkbox = orgElement.find("input[type='checkbox']");
    			checkbox.prop("checked", true);
    			checkbox.attr('disabled', 'disabled');
    		}

			if ($('.panel-organization').scrollLeft() > 0 ){
				orgElement.find('.right').css('left', 243 + $('.panel-organization').scrollLeft());
			}
        	
        	orgChildElement.append(orgElement);
        	
        	return orgChildElement;
        },
        _getOrganizaitionTemplate: function(isExpanded) {   
        	var iconElementClass = isExpanded ? "arrow-down" : "arrow-right";
        	var template = $('<li class="organization-item org-item item">'
        					+ '<div class="organization-widget-container">'
        					+ '		<div class="left"><i class="' + iconElementClass + '"></i></div>'
        					+ '		<div class="right">'
        					
        					+ '         <div class="checkbox-parent check-box-organization">'
        					+ '         	<input type="checkbox"/>'
        					+ '         	<label><span></span></label>'
        					+ '         </div>'
        					
        					+ '			<img class="remove-organization" src="../images/contacts/remove.png"/>'
        					+ '		</div>'
        					+ '		<div class="center">'        					
        					+ '			<div class="organization-name"></div>'
			        		+ '		</div>'
			        		+ '	</div>'
			        		+ '</li>');
			if ($('.panel-organization').scrollLeft > 0 ){
				template.find('.right').css('left', 243 + 10 + $('.panel-organization').scrollLeft());
			}
        	return template;
        },
        _getStaffListView: function(lastElement, staffs, parentOrgId, parentOrgLevel, currentUserInfo) {
        	var _self = this;        	
        	
        	var memberIdArray = [];
        	//For Organization Staff, need fetch localAvatar, remoteAvatar, do not use staff.localAvatar, remoteAvatar, they're empty.
        	uc.modules.contact.ContactManager.addOrUpdateContactsToCache(staffs);

			if(this.selectedUserIdList && this.selectedUserIdList.length > 0){
				for(var i = 0; i < this.selectedUserIdList.length; ++i){
					var userId = this.selectedUserIdList[i].userId ? this.selectedUserIdList[i].userId : this.selectedUserIdList[i];

					for(var j=0; j<staffs.length;j++){
						if(staffs[j].userId == userId){
							staffs.splice(j, 1);
							break;
						}
					}
				}
			}

        	for (var i = 0; i < staffs.length; i++){
        		var staff = staffs[i];
        		if($('.org-parent .contact-item-' + staff.userId).length > 0){
        			continue;
        		}
        		memberIdArray.push(staff.userId);            	
        		var contact = uc.modules.contact.ContactManager.getCachedContact(staff.userId);
        		
            	var staffView = _self._getStaffItemView(contact, parentOrgId, parentOrgLevel, currentUserInfo);
            	staffView.insertAfter(lastElement);
            	lastElement = staffView;
        	}
        	
        	uc.modules.contact.ContactManager.batchRequestMembers(memberIdArray);

			// start------------------------added by xue.bai_2 on 2016/05/11----------------------------
			if(this.isContact){
				var copyBack = '{"panel": "panel-organization", "panelUl": "staff-child"}';
				uc.IContactService.getUserStatus(memberIdArray, copyBack, function(){});
			}

			var userIds = uc.modules.contact.ContactManager.cachedContactsStatus.getContacts("panel-organization");
			userIds = $.unique(userIds ? $.merge(userIds, memberIdArray) : memberIdArray);

			uc.modules.contact.ContactManager.cachedContactsStatus.addContacts("panel-organization", userIds);
			// ------------------------added by xue.bai_2 on 2016/05/11-------------------------------end

        	return lastElement;
        },
        _getStaffItemView: function(contact, parentOrgId, parentOrgLevel, currentUserInfo) {
			var option = uc.util.StorageUtil.getData('chooseContacts');
        	var staff = uc.modules.contact.ContactManager.getCachedContact(contact.userId);
        	var parentElement = $('.org-item-' + parentOrgId);        	
        	var parentClassnameList = [];
            var level = Number(parentOrgLevel) + 1;
            var staffChildElement = $('<div class="staff-child"></div>');
            var staffElement = uc.modules.contact.ContactManager._getContactTemplate();
            var avatar = staffElement.find('.left .avatar');
            var paddingLeft = level * 20;

        	if(parentElement.length > 0) {        		
        		var parentClassnames = parentElement.attr("class");        		
        		parentClassnameList = parentClassnames.split(" ");
        	}

        	staffElement.addClass("contact-item-" + staff.userId);
        	staffElement.attr("userId", staff.userId);
        	staffElement.find('.checkbox-parent').addClass("check-box-contact-" + staff.userId);
        	staffElement.find('.contact-display-name').text(staff.displayName);
        	staffElement.find('.contact-display-name').attr("userId", staff.userId);
        	staffElement.find('.contact-department').text(parentElement.find('.organization-name').text());
        	staffElement.find('.contact-position').text(staff.position);
        	staffElement.find('.text-ellipsis').removeClass("text-ellipsis");

			if (currentUserInfo && contact.userId == currentUserInfo.userId && (option && option.key != uc.constants.Meeting.CreateMeetingName && !option.isMeetingForward)) {
				staffElement.attr('disabled', 'disabled');
				staffElement.find('[type="checkbox"]').attr('disabled', 'disabled');
			}

        	avatar.addClass("avatar-" + staff.userId);

        	if(staff.localAvatar){
        		avatar.attr("src", staff.localAvatar);
        		avatar.addClass("avatar-loaded");
    		} else {
    			avatar.addClass("avatar-unloaded");

    			if(staff.remoteAvatarUrl){
    			}
    		}

        	staffElement.css("padding-left", paddingLeft + "px");
        	staffElement.addClass("org-parent-" + parentOrgId);
        	staffElement.attr("userId", staff.userId); 
        	staffElement.attr("parentOrgId", parentOrgId);
        	staffElement.attr("level", level);
        	
        	$.each(parentClassnameList, function(k, classname){
        		if(classname != " " && classname.indexOf("org-parent-") >=0){
        			staffElement.addClass(classname);
        		}
    		}); 
        	
        	//If parent Organization is checked, then sub Organizations and Staff should be checked and disabled
        	var parentCheckbox = parentElement.find("input[type='checkbox']");
    		if(parentCheckbox.is(":checked")){    			
    			var checkbox = staffElement.find("input[type='checkbox']");
    			checkbox.prop("checked", true);
    			checkbox.attr('disabled', 'disabled');
    		}

			if ($('.panel-organization').scrollLeft() > 0 ){
				staffElement.find('.right').css('left', 243 + 10 + $('.panel-organization').scrollLeft());
			}
    		
        	staffChildElement.append(staffElement);
        	
        	return staffChildElement;        	
        }
    }
})(uc);