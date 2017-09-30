(function(uc){
	
	uc.modules.chooseContacts.ChooseContactsEventListener = function(){
		
	};
	
	jQuery.extend(uc.modules.chooseContacts.ChooseContactsEventListener.prototype, 
		uc.interfaces.IUIEvent, {
			OnOpenChooseContactWindow: function(key){
				uc.util.LogUtil.info('ChooseContactsEventListener', 'OnOpenChooseContactWindow', 'key=', key);
				uc.modules.chooseContacts.ChooseContactsManager.OnOpenChooseContactWindow(key);
			}
	});
})(uc);