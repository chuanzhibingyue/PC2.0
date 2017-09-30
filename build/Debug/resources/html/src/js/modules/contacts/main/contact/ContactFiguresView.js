/**
 * Created by senlin.ouyang on 2015/7/20.
 */
(function(uc){
    uc.modules.contact.ContactFiguresView = function(){
    	this.node = $('<div id="figuresPanel"></div>');     	
    };
    jQuery.extend(uc.modules.contact.ContactFiguresView.prototype, {
    	init: function(options){
            this.node.append(this.getContactFiguresViewHtml());
            this.prependToParent(options.parentNode);
            this.bindEvents();
        },
        prependToParent: function(parentNode){
        	var figuresPanel = parentNode.find('#figuresPanel');
        	if(figuresPanel.length > 0){
        		figuresPanel.replaceWith(this.node);
        	} else {
        		this.node.prependTo(parentNode);
        	}        	
        },
        getContactFiguresViewHtml: function(){
        	var parent = $('<div></div>');
        	var projectFigure = $('<figure class="figure-projects hide" panel="panel-projects">'
					+ '<figcaption><img src="../images/contacts/project.png"></figcaption>'
					+ '<figcaption>' + uc_resource.ChooseContacts.Projects + '</figcaption>'
					+ '<i class="icon-call_video_ringht"></i>'
					+ '</figure>');
        	parent.append(projectFigure);
			
			var groupChatFigure = $('<figure class="figure-groupChats" panel="panel-groupChats">'
					+ '<figcaption><img src="../images/contacts/group.png"></figcaption>'
					+ '<figcaption>' + uc_resource.ChooseContacts.GroupChats + '</figcaption>'
					+ '<i class="icon-call_video_ringht"></i>'
					+ '</figure>');
			parent.append(groupChatFigure);        	
			
			var colleaguesFigure = $('<figure class="figure-my-department" class="figure-my-department" panel="panel-my-department">'
					+ '<figcaption><img src="../images/contacts/mydepart.png"></figcaption>'
					+ '<figcaption>' + uc_resource.ChooseContacts.MyDept + '</figcaption>'
					+ '<i class="icon-call_video_ringht"></i>'
					+ '</figure>');
			parent.append(colleaguesFigure);
			
			var organizationFigure = $('<figure class="figure-organization" panel="panel-organization">'
					+ '<figcaption><img src="../images/contacts/org.png"></figcaption>'
					+ '<figcaption>' + uc_resource.ChooseContacts.Org + '</figcaption>'
					+ '<i class="icon-call_video_ringht"></i>'
					+ '</figure>');
			parent.append(organizationFigure);

            var emailFigure=$('<figure class="figure-email hide" panel="panel-email">'
                + '<figcaption><img src="../images/contacts/email_contact.png"></figcaption>'
                + '<figcaption>' + uc_resource.ChooseContacts.Email + '</figcaption>'
                + '</figure>');
            parent.append(emailFigure);
			return parent.html();
        },
        bindEvents: function() {        	
        	var currentNode = this.node;
        	currentNode.find('figure').on("click", function() { 
        		var figure = $(this);        		
        		currentNode.find('figure').removeClass('figure-selected');
        		
        		uc.modules.contact.ContactManager.setLatestClickedContactsPanel(currentNode.parent('.contacts-panel'));  
        		        		        		                   
                var panel = figure.attr('panel');   
                figure.addClass('figure-selected');
                
                var popup = currentNode.parent('.popup-panel-left-body');                
                if(popup.length > 0) {
                	//When it's popup page
                	uc.modules.contact.ContactManager.switchPanel(panel);
                } else {
                	//When it's contacts main page
                	uc.modules.contact.ContactManager.switchPanel(panel, true);
					uc.modules.contact.ContactManager.setLatestClickedPanel(panel);
                }                
        	});  
        	/*uc.ui.Contacts.MainPanel.on("click", "#contactPanel .return-contacts-panel", function() { 
        		_self.switchPanel("panel-contacts");
        	});*/
        }
    })
})(uc);