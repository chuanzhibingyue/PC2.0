/**
 * Created with JetBrains WebStorm.
 * User: hougang.chen
 * Date: 16-4-8
 * Time: 下午5:06
 * To change this template use File | Settings | File Templates.
 */
/**
 * Created with JetBrains WebStorm.
 * User: hougang.chen
 * Date: 16-4-7
 * Time: 上午10:51
 * To change this template use File | Settings | File Templates.
 */

(function (uc) {
	uc.modules.screenSharing.screenSharingAccept = {
		init: function () {
			//uc.util.WindowUtil.maximize();
			this.$ScreenSharingAcceptPanel = $('.ScreenSharingAcceptPanel');
			this.screenShareViewObj = $('#screenShareViewObj');
			this.IsFullScreen = false;
			this.bindEvent();

			//获取创建窗口的窗口句柄，调用startDesktopView: function(hwnd,callback){},发送给sdk
			var This = this;
			uc.util.WindowUtil.getWindowHwnd('screenSharingAccept',function(arg){
				uc.util.LogUtil.info('uc.modules.screenSharing.screenSharingAccept','getWindowHwnd name is screenSharingAccept',arg);
				if(arg[0] == 0){
					This.postMeaasge("screenShare",arg[1]);
				}
			});
		},
		bindEvent:function(){
			var This = this;
			this.$ScreenSharingAcceptPanel.on('click','.statusbar .WindowOpertionToolbar .minSize',function(e){
				uc.util.WindowUtil.minimize();
			}).on('click','.statusbar .WindowOpertionToolbar .fullScreen',function(e){
					/*if(This.IsFullScreen){
						uc.util.WindowUtil.restore();
					}else{
						uc.util.WindowUtil.fullScreen();
					} */
					//uc.util.WindowUtil.maximize();
					This.IsFullScreen = !This.IsFullScreen;
			});
			//测试代码
			this.screenShareViewObj[0].addEventListener('message', this.handleMessage, true);
		},
		handleMessage:function(message_event){
			console.log("message_event:"+message_event.data);
			var data =  $.parseJSON(message_event.data);
			if(data != null && data.errorcode == 0){
				if(data.operation == "createScreenShareWindow"){
					uc.ISharingService.startDesktopView(parseInt(data.data),function(arg){

					})
				}
			}
		},
		postMeaasge:function(command,data){
			this.screenShareViewObj[0].postMessage({command:command,data:data+''});
		}
	};
	jQuery(document).ready(function () {
		uc.init();
		uc.modules.screenSharing.screenSharingAccept.init();
	});
})(uc);
