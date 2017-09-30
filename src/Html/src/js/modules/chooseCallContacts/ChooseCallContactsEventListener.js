/**
 * Created by zhen.xu.
 * User: zhen.xu
 * Date: 15-12-14 下午5:09
 */
(function (uc) {
    uc.modules.call.ChooseCallContactsEventListener = function () {

    }

    jQuery.extend(uc.modules.call.ChooseCallContactsEventListener.prototype,
        uc.interfaces.IUIEvent,
        uc.interfaces.IContactEvent,
        uc.interfaces.IGroupEvent,{
            OnOpenChooseCallContactsWindow: function (key) {
                uc.util.LogUtil.info('ChooseCallContactsEventListener', 'OnChooseCallContactsWindow', 'key=', key);
                uc.modules.call.ChooseCallContactsManager.OnChooseCallContactsWindow(key);
            },
            OnContactInfoReceived: function(errorCode, userIds, contacts) {
                uc.modules.call.ChooseCallContactsManager.OnContactInfoReceived(errorCode, userIds, contacts);
            },
            OnGroupInfoReceived: function(errorCode, groupId, group) {
                uc.modules.call.ChooseCallContactsManager.OnGroupInfoReceived(errorCode, groupId, group);
            },
		    OnBasicContactInfoReceived: function (errorCode, userIds, contacts) {
			    uc.modules.call.ChooseCallContactsManager.OnBasicContactInfoReceived(errorCode, userIds, contacts);
		    }

    });
})(uc);