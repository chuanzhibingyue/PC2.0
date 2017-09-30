/**
 * Created by zhen.xu.
 * User: zhen.xu
 * Date: 15-12-15 上午10:00
 */
(function (uc) {
    uc.modules.call.ChooseCallContactsManager = {
        OnChooseCallContactsWindow: function (key) {
            uc.util.StorageUtil.setData("chooseCallContacts", key);
            var openTime = new Date();

            var options = {
                title: '',
                width: 601,
                height: 620,
                refreshIfExist: true,
                centerScreen: true,
                modal: true,
                resizable: false,
                minWidth: 601,
                minHeight: 620,
                caption: uc.constants.UI.DragCaption.ChooseCallContacts
            };

            uc.util.WindowUtil.open('page/chooseCallContacts.html', 'chooseCallContacts' + openTime, options);
        },
        OnContactInfoReceived: function(errorCode, userIds, contacts) {
            uc.modules.call.ChooseCallContacts.OnContactInfoReceived(errorCode, userIds, contacts);
        },
        OnGroupInfoReceived: function(errorCode, groupId, group) {
            uc.modules.call.ChooseCallContacts.OnGroupInfoReceived(errorCode, groupId, group);
        },
        OnServerSearchResultReceived: function (errorCode, searchId, contacts) {
            uc.modules.call.ChooseCallContacts.OnServerSearchResultReceived(errorCode, searchId, contacts);
        },
	    OnBasicContactInfoReceived: function (errorCode, userIds, contacts) {
		    if(errorCode == uc.ErrorCode.Success){
			    uc.modules.call.ChooseCallContacts.OnBasicContactInfoReceived(contacts);
		    }
	    }
    }
})(uc);