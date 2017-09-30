/**
 * Created with JetBrains WebStorm.
 * User: hougang.chen
 * Date: 16-4-7
 * Time: 下午3:30
 * To change this template use File | Settings | File Templates.
 */
(function(uc){
	uc.interfaces.ISharingService = {
		startDesktopShare: function(callback){},
		stopDesktopShare: function(callback){},
		startDesktopView: function(shareTitle,callback){},
		stopDesktopView: function(callback){}
	}
})(uc);