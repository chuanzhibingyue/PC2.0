/**
 * created by xue.bai_2 on 15/11/26
 */
(function (uc) {
    uc.modules.bubble.BubbleEventListener = function () {

    };

    jQuery.extend(uc.modules.bubble.BubbleEventListener.prototype,
        uc.interfaces.IClientEvent,
        uc.interfaces.IContactEvent,
        uc.interfaces.IGroupEvent, {
            OnMouseMoveInTray: function () {
                uc.modules.bubble.BubbleView.mouseInTray();
            },
            OnMouseMoveOutTray: function () {
                uc.modules.bubble.BubbleView.mouseOutIcon();
            },
            OnBasicContactInfoReceived: function(errorCode, userIds, contacts){
                if(errorCode == uc.ErrorCode.Success){
                    uc.modules.bubble.BubbleView.OnBasicContactInfoReceived(contacts);
                }
            },
            OnGroupInfoReceived: function(errorCode, groupId, mountId, group){
                if(errorCode == uc.ErrorCode.Success){
                    uc.modules.bubble.BubbleView.OnGroupInfoReceived(groupId, mountId, group);
                }
            }
        });
})(uc);
