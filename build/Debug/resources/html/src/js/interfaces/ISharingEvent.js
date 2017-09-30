/**
 * Created with JetBrains WebStorm.
 * User: hougang.chen
 * Date: 16-4-7
 * Time: 下午3:42
 * To change this template use File | Settings | File Templates.
 */
(function(uc){
	uc.interfaces.ISharingEvent =
	{
		//桌面共享error通知事件
		OnSharingError: function(statusCode){},
		//桌面共享事件
		OnDesktopShared: function(conferenceId, userId){},
		//桌面共享停止事件
		OnDesktopShareStoped:function(conferenceId, userId){},
		//观看桌面共享开始事件
		OnDesktopViewerShareStarted:function(conferenceId){},
		//观看桌面共享结束事件
		OnDesktopViewerShareStoped:function(conferenceId){}
	}
})(uc);