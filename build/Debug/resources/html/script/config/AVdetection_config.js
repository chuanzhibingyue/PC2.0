/**
 * Created by senlin.ouyang on 2015/7/15.
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
	'src/js/modules/AVdetection/AVdetectionEventListener.js',
	'src/js/modules/AVdetection/AVdetectionManager.js',
	'src/js/modules/AVdetection/AVdetectionWindow.js'
]