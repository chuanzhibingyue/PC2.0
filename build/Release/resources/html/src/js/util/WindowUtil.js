/**
 * Created by senlin.ouyang on 2015/7/20.
 */
(function (uc) {
    uc.util.WindowUtil = {
        /**
         * open a window, create it if not exist, otherwise bring it to front
         * @param url       html url to show, path to /html/
         * @param name      window name
         * @param options   window options {
        *   title              	title of the window, show on system task bar
        *   left              	left position on screen
        *   top               	top position on screen
        *   width             	width of the window
        *   height            	height of the window
        *   refreshIfExist    	if window is exist, whether to refresh to reload the url
        *   centerScreen    	whether shown in screen center, if true, left and top will not be used
        *   modal           	whether window is modal or not
        *   resizable        	whether window is resizable
        *   minWidth          	minWidth of the window
        *   minHeight         	minHeight of the window
        *   isTop:true,         whether window is on top
        *   enableDrag          whether drag and drop is enabled
        *   hasTaskBarIcon      whether to show app icon for this window
        *   behaviorIfSCClose   behavior when close window on System task bar
        *   isChild             whether window is child window of another window
        *   isShow              whether show the window when open
        *   borderSpace         border width, for drag
        *   caption      	  	draggable caption of the window, in array each caption has four properties top, left, width, height
        * }
         *                           if width is less than 0, it stands for distance to right border, if height is less than 0, it stands for
         *                           distance to bottom border, for example:
         *                           {
        *                              top: 0,
        *                              left: 0,
        *                              width: -20,
        *                              height: -20
        *                           }
         *  removeBorder        whether to remove border for shadow, if true, do not add border to show window shadow
         */
        open: function (url, name, options) {
            if (!options.removeBorder) {
                options.width = options.width + uc.constants.UI.WindowShadowWidth.Left + uc.constants.UI.WindowShadowWidth.Right;
                options.height = options.height + uc.constants.UI.WindowShadowWidth.Top + uc.constants.UI.WindowShadowWidth.Bottom;
                options.minWidth = options.minWidth ? options.minWidth + uc.constants.UI.WindowShadowWidth.Left + uc.constants.UI.WindowShadowWidth.Right : options.width;
                options.minHeight = options.minHeight ? options.minHeight + uc.constants.UI.WindowShadowWidth.Top + uc.constants.UI.WindowShadowWidth.Bottom : options.height;
            }
            ucClient.Window.open(url, name, $.extend({
                title: uc_resource.Common.AppName,
                left: 0,
                top: 0,
                width: 0,
                height: 0,
                refreshIfExist: false,
                centerScreen: true,
                modal: false,
                resizable: false,
                minWidth: 200,
                minHeight: 200,
                isTop: false,
                enableDrag: false,
                hasTaskBarIcon: true,
                behaviorIfSCClose: uc.constants.UI.WindowCloseBehavior.Close,
                isChild: true,
                isShow: true,
                borderSpace: {
                    leftBorder: uc.constants.UI.WindowShadowWidth.Left,
                    topBorder: uc.constants.UI.WindowShadowWidth.Top,
                    rightBorder: uc.constants.UI.WindowShadowWidth.Right,
                    bottomBorder: uc.constants.UI.WindowShadowWidth.Bottom
                },
                isLayered: false
            }, options), options.closeOtherWindow ? 1 : 0);
        },
        /**
         * set window draggable area
         * @param captionArray      draggable caption of the window, in array each caption has four properties top, left, width, height
         *                           if width is less than 0, it stands for distance to right border, if height is less than 0, it stands for
         *                           distance to bottom border, for example:
         *                           {
        *                              top: 0,
        *                              left: 0,
        *                              width: -20,
        *                              height: -20
        *                           }
         */
        setCaptions: function (captionArray) {
            ucClient.Window.setCaptions(captionArray)
        },
        /**
         * set window on top
         * @param isTop
         */
        setOnTop: function (isTop) {
            ucClient.Window.setTop(isTop);
        },
        /**
         * set system tray
         * @param title     title for system tray
         * @param icon      icon image
         * @param isFlash   whether flash the icon
         * @param menu      menu when click system tray, format is :
         *                  {
        *                      menu: [
        *                          {
        *                              id: 200,
        *                              label: 'open window',
        *                              icon: 'a.png',
        *                              click: function(){}
        *                          },
        *                          {
        *                              id: 201,
        *                              label: ''    "" indicate line
        *                          },
        *                          {
        *                              id: 202,
        *                              label: 'logout',
        *                              click: function(){}
        *                          }
        *                      ]
        *                  }
         */
        setSystemTray: function (title, icon, isFlash, menu) {
            ucClient.Window.setTray(title, icon, isFlash, menu);
        },
        /**
         * navigate to another url in current window
         * @param url   url to navigate, path to /html/
         */
        navigate: function (url) {
            ucClient.Window.navigate(url);
        },
        /**
         * minimize current window
         */
        minimize: function () {
            ucClient.Window.minimize();
        },
        /**
         * miximize current window
         */
        maximize: function () {
            ucClient.Window.maximize();
        },
        /**
         * restore window
         */
        restore: function () {
            ucClient.Window.restore();
        },
        /**
         * hide current window
         */
        hide: function () {
            ucClient.Window.hide();
        },
        /**
         * hide all window
         */
        hideAll: function () {
            ucClient.Window.hideAll();
        },
        /**
         * center current window
         */
        center: function () {
            ucClient.Window.centerWindow();
        },
        /**
         * set current window size
         */
        setSize: function (width, height) {
            ucClient.Window.setSize(width, height);
        },

        /**
         * set window position
         * @param left
         * @param top
         */
        setPosition: function (left, top) {
            ucClient.Window.move(left, top);
        },
        /**
         * show current window
         */
        show: function () {
            ucClient.Window.show();
        },
        /**
         * close current window
         */
        close: function (url) {
            ucClient.Window.close(url);
        },
        /**
         * set system tray flash
         */
        flashTray: function (isflash, interval) {
            ucClient.Window.flashTray(isflash, interval);
        },
        getWindowHwnd: function (name, callback) {
            ucClient.Window.getWindowHwnd(name, callback);
        },
        fullScreen: function () {
            ucClient.Window.fullScreen();
        },
        setWindowDBClickDisble: function () {
            ucClient.Window.disableDBClick();
        },
        setWindowDBClickEnable: function () {
            ucClient.Window.enableDBClick();
        }
}
})(uc);