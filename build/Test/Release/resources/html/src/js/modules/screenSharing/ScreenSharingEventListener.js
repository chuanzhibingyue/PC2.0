/**
 * Created with JetBrains WebStorm.
 * User: hougang.chen
 * Date: 16-4-7
 * Time: 上午10:53
 * To change this template use File | Settings | File Templates.
 */

/**
 * Created by waouyang on 15/12/7.
 */
(function(uc){
	uc.modules.screenSharing.ScreenSharingEventListener = function(){

	};
	jQuery.extend(uc.modules.screenSharing.ScreenSharingEventListener.prototype,
		uc.interfaces.IClientEvent,
		uc.interfaces.IUIEvent,
		uc.interfaces.ISharingEvent,{
			/**桌面共享**/
			//桌面共享停止事件
			OnDesktopShareStoped:function(conferenceId, userId){
			   uc.modules.screenSharing.ScreenSharingManager.OnDesktopShareStoped(conferenceId, userId);
			},
			//观看桌面共享开始事件
			OnDesktopViewerShareStarted:function(conferenceId){

			}
		})
})(uc);
