/**
 * Created with JetBrains WebStorm.
 * User: hougang.chen
 * Date: 16-4-7
 * Time: 上午10:51
 * To change this template use File | Settings | File Templates.
 */

(function (uc) {
	uc.modules.screenSharing.ScreenSharing = {
		init: function () {
			this.$ScreenSharingPanel = $('.ScreenSharingPanel');
			this.$operationBox = this.$ScreenSharingPanel.find('.operationBox');
			this.$toolbar = this.$operationBox.find('.comment_Toolbar .toolbar');
			this.$toggleBtn = this.$ScreenSharingPanel.find('.toggle_btn');
			this.$thumbtack_Icon = this.$ScreenSharingPanel.find('.thumbtack_Icon');
			this.$toggleBtn.hide();
			//this.updatToolbarPosition();
			this.focusTimer;
			this.IsHumbtack = false;
			this.bindEvent();
			this.toggleTimer();
		},
		bindEvent:function(){
			var This = this;
			this.$ScreenSharingPanel.on('click','.operationBox .comment_Toolbar .commentToggle_btn',function(e){
				This.updatToolbarPosition();
			}).on('click','.operationBox .endShare_btn',function(e){
				//关闭共享
				uc.ISharingService.stopDesktopShare();
				uc.util.WindowUtil.close();
			}).on('mouseover',function(){
				//This.$operationBox.slideToggle('fast');
				//$(this).toggle();
				clearTimeout(This.focusTimer);
			}).on('mouseout',function(e){
				if(!This.IsHumbtack){ //如果固定住，不计时
					This.toggleTimer();
				}
			}).on('click','.operationBox .thumbtack_Icon',function(){
				//点击固定图标
				This.IsHumbtack = !This.IsHumbtack;
				This.$thumbtack_Icon.toggleClass('thumbtack_Icon_lock');

			}).on('mouseover','.toggle_btn',function(e){
					This.$operationBox.slideToggle('fast');
					$(this).toggle();
					//clearTimeout(This.focusTimer);
					e.stopPropagation();
				});
			uc.IUIService.bindCrossWindowEvent(uc.constants.UI.CrossWindowEventName.closeScreenShareWindow, function(){
				uc.util.WindowUtil.close();
			});
		},
		//是否置顶计时
		toggleTimer:function(){
			var This = this;
			this.focusTimer = setTimeout(function(){
				This.$operationBox.slideToggle('fast',function(){
					This.$toggleBtn.toggle();
				});
				clearTimeout(This.focusTimer);
			},5000);
		},
		updatToolbarPosition:function(){
			//批注切换
			var isHidden= this.$toolbar.is(":hidden");//是否隐藏
			if(isHidden){
				this.$toolbar.show();
				//this.$operationBox.css('left',10+"px");
			}else{
				this.$toolbar.hide();
				//this.$operationBox.css('left',100+"px");
			}
		}
	};
	jQuery(document).ready(function () {
		uc.init();
		uc.modules.screenSharing.ScreenSharing.init();
	});
})(uc);
