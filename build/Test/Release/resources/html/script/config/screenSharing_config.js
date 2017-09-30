/**
 * Created with JetBrains WebStorm.
 * User: hougang.chen
 * Date: 16-4-7
 * Time: 上午10:59
 * To change this template use File | Settings | File Templates.
*/
[
	'src/js/lib/jquery-2.1.4.js',
	'src/js/uc.js',
	/* Constants **/
	'src/js/constants/Constants.js',
	'src/js/constants/ErrorCode.js',
	/* Util **/
	'src/js/util/EventUtil.js',
	'src/js/util/WindowUtil.js',
	'src/js/util/PageUtil.js',
	'src/js/util/LogUtil.js',
	/* Interfaces **/
	'src/js/interfaces/IClientService.js',
	'src/js/interfaces/IClientEvent.js',
	'src/js/interfaces/IUIEvent.js',
	'src/js/interfaces/ICallService.js',
	'src/js/interfaces/ICallEvent.js',
	'src/js/interfaces/IUIService.js',
	'src/js/interfaces/ISharingService.js',
	'src/js/interfaces/ISharingEvent.js',
	'src/js/listener/event_config.js',
	'src/js/listener/service_config.js',
	'src/js/listener/native_event_config.js',
	'src/js/listener/native_service_config.js',

	'src/js/listener/manager.js',
	/* Components **/
	'src/js/service/UIService.js',
	'src/js/modules/component/TitleBarButton.js',
	'src/js/modules/component/PromptMessageBox.js',
	'src/js/util/TimeUtil.js',
	'src/js/util/StringUtil.js',
	/* ============ Modules ===============**/
	/* When js has document ready function, need position to end **/

	'src/js/modules/screenSharing/ScreenSharingEventListener.js',
	'src/js/modules/screenSharing/ScreenSharingManager.js',
	'src/js/modules/screenSharing/ScreenSharing.js'
]
