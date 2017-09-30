/**
 * created by xue.bai_2 on 15/11/26
 */

(function (uc) {
	uc.modules.bubble.BubbleManager = {
        showPopupBubbleMessageTimer: -1,
        showPopupBubbleMessageDelay: 100,
        updateBubbleMessageListTimer: -1,
        updateBubbleMessageListDelay: 100,
        init: function(){
            setTimeout(function(){
                uc.modules.bubble.BubbleManager.createBubbleMessageWindow();
            }, 200);
        },
        showPopupBubbleMessage: function(conversationInfo){
            if(uc.IClientService.getWindowStatus() == uc.constants.UI.WindowActiveState.Topmost){
                return;
            }

		var currentUserStatus = uc.modules.main.Main.currentUserStatusValue;
		if(currentUserStatus == uc.constants.UserStatusType.Busy){
			return;
		}

            this._getSettings(function(settings){
                var bubbleSet = settings.bubbleSet;
                if(!bubbleSet){
                    return;
                }

                //flash system tray
                uc.util.WindowUtil.flashTray(true, 400);
                uc.modules.bubble.BubbleManager.updatePopupBubbleMessage(conversationInfo);
            })
        },

    	updateBubbleMessageInfoList: function(conversationInfoList){
			this._getSettings(function(settings){
				var bubbleSet = settings.bubbleSet;
				if(!bubbleSet){
					return;
				}

                //flash system tray
                uc.util.WindowUtil.flashTray(true, 400);

                uc.modules.bubble.BubbleManager.updateBubbleMessageList(conversationInfoList);
			});
    	},
        createBubbleMessageWindow: function(){
			var position = uc.IClientService.getTrayPos();
			var left = position.w > 300 ? position.x : position.x - (300 - position.w);
			var top = position.y - 340;

			var options = {
					left: left,
					top: top,
					width: 260,
	                height: 320,
	                refreshIfExist: false,
	                centerScreen: false,
	                modal: false,
	                resizable: false,
	                minWidth: 260,
	                minHeight: 110,
					isChild: false,
					hasTaskBarIcon: false,
					isTop: true
			};

            uc.util.WindowUtil.open('page/messageBubble.html', 'messageBubble', options);
		},

        updatePopupBubbleMessage: function(conversationInfo){
            clearTimeout(this.showPopupBubbleMessageTimer);

            this.showPopupBubbleMessageTimer = setTimeout(function(){
                uc.IUIService.triggerCrossWindowEvent(uc.constants.UI.CrossWindowEventName.ShowPopupBubbleMessage, {conversationInfo: conversationInfo});
            }, this.showPopupBubbleMessageDelay);
        },

		updateBubbleMessageList: function(conversationInfos){
			clearTimeout(this.updateBubbleMessageListTimer);

            this.updateBubbleMessageListTimer = setTimeout(function(){
                uc.IUIService.triggerCrossWindowEvent(uc.constants.UI.CrossWindowEventName.UpdateBubbleMessageList, {conversationInfoList: conversationInfos});
            }, this.updateBubbleMessageListDelay);

		},
		_getSettings: function(callback) {
			uc.IClientService.getConfigInfo("bee", "settings",
					function (args) {
						if (args[0] != 0) {
							uc.modules.setting.SettingManager.saveSetting();
							return;
						}
						var str = args[1];
						var obj = JSON.parse(str);
						if (obj != null) {
							callback && callback(obj);
						}
					}
			);
		}
	};
})(uc);