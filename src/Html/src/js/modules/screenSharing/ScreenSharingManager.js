/**
 * Created with JetBrains WebStorm.
 * User: hougang.chen
 * Date: 16-4-7
 * Time: 上午10:53
 * To change this template use File | Settings | File Templates.
 */
(function (uc) {
	uc.modules.screenSharing.ScreenSharingManager = {
		init:function(){

		},
		OnDesktopShareStoped:function(conferenceId, userId){
			uc.util.WindowUtil.close();
		},
		OnDesktopViewerShareStarted:function(conferenceId){
		   //插件区域显示共享内容
		}
	}
})(uc);


