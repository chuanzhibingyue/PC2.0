/**
 * Created by senlin.ouyang on 2015/7/20.
 */
(function(uc){
    uc.modules.contact.ContactFigureView = function(){
    	this.node = $('<div id="figuresPanel"></div>');     	
    };
    jQuery.extend(uc.modules.contact.ContactFigureView.prototype, {
    	init: function(options){
            this.node.append(this.getContactFigureViewHtml(options.panelName));
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
        getContactFigureViewHtml: function(panelName){
        	var parent = $('<div></div>');        	
        	var items = $('<div class="field"><div class="label return-contacts-panel"></div><div class="value figure-name"></div></div>');
        	parent.append(items); 
        	parent.find('.field').attr("panel", panelName);
        	parent.find('.field').addClass("figure-selected"); 
        	
        	switch(panelName){	    		
	    		case "panel-projects":        			
	    			items.find('.figure-name').text(uc_resource.ChooseContacts.Projects);	    			
	    			break;
	    		case "panel-groupChats":        			
	    			items.find('.figure-name').text(uc_resource.ChooseContacts.GroupChats);	    			
	    			break;
	    		case "panel-my-department":
	    			items.find('.figure-name').text(uc_resource.ChooseContacts.MyDept);	    			
	    			break;
	    		case "panel-organization":        			
	    			items.find('.figure-name').text(uc_resource.ChooseContacts.Org);	    			
	    			break;
	    	}
        	
			return parent.html();
        },
        bindEvents: function() {        	
        	var currentNode = this.node;
        	currentNode.find('.return-contacts-panel').on("click", function() {
                var popup = currentNode.parent('.popup-panel-left-body');                
                uc.modules.contact.ContactManager.setLatestClickedContactsPanel(currentNode.parent('.contacts-panel'));
                if(popup.length > 0) {
                	//When it's popup page
                	uc.modules.contact.ContactManager.switchPanel("panel-contacts");
                } else {
                	//When it's contacts main page
                	uc.modules.contact.ContactManager.switchPanel("panel-contacts", true);
					uc.modules.contact.ContactManager.setLatestClickedPanel("panel-contacts");
                }                
        	});
        }
    })
})(uc);