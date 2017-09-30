var ucClient;

if (!ucClient) {
	ucClient = {};
}

if (!ucClient.App) {
	ucClient.App = {};
};

if (!ucClient.Window) {
	ucClient.Window = {};
}

if (!ucClient.KeyBoard) {
    ucClient.KeyBoard = {};
}

if (!ucClient.Dialog) {
	ucClient.Dialog = {};
}

if (!ucClient.File) {
	ucClient.File = {};
}

if (!ucClient.Image) {
	ucClient.Image = {};
}

if (!ucClient.Font) {
    ucClient.Font = {};
}

(function() {
	ucClient.requestId = 0;
	
	ucClient.xmlHttpRequest = function() {
		ucClient.requestId += 1;
		var request = new XMLHttpRequest();
		var url = 'client://javascript/';
		if (arguments.length >= 2) {
			url += arguments[0] + '.' + arguments[1] + '.' + ucClient.requestId +'?';
			for (i = 2; i < arguments.length; i++) {
				url += arguments[i];
				if (i != arguments.length - 1) {
					url += '###';
				}
			}

			request.open('get', url, false);
        	request.send();
        	return request;
		}
		else {
			return undefined;
		}
	}
	
	native function JSCommond();
	// 应用程序操作
	
	/**
     * 退出应用程序
     */
	ucClient.App.quit = function() {
		return JSCommond('App', 'Quit');
	};

    /**
	 * 注册事件
	 * @param eventname 事件名称 
	 * @param callback 事件触发后的回调
	 */
	ucClient.addEventListener = function(eventname, callback) {
	    document.addEventListener(eventname, callback);
	};
	
	/**
     * 注销事件
     * @param eventname 事件名称 
     * @param callback 事件触发后的回调
     */
	ucClient.removeEventListener = function(eventname, callback) {
	    document.removeEventListener(eventname, callback);
	};

	/**
	 * 触发特定事件
	 * @param event event对象
	 */
	ucClient.dispatchEvent = function(event) {
		document.dispatchEvent(event);
	};

	/**
	 * 触发全局事件（包含本html注册和其他html注册的事件）
	 * @param event event对象
	 */
	ucClient.dispatchGlobalEvent = function(eventName, eventData) {
		return JSCommond('App', 'DispatchGlobalEvent', eventName, eventData);
	};

	// 窗口操作
	
	ucClient.Window.Status = {
	    MAXIMIZED_FROM_NORMAL: "WindowMaximizedFromNormal", 
	    RESTORED_FROM_MAXIMIZED: "WindowRestoredFromMaximized"
	};

	/**
     * 打开新窗口
     */
	ucClient.Window.open = function(url, name, features, afterBehavior) {
	    if (afterBehavior == undefined) {
	        afterBehavior = 0;
	    }
	    return JSCommond('Window', 'Open', url, name, features, afterBehavior);
	};
	
	ucClient.Window.setTop = function(isTop) {
		return JSCommond('Window', 'SetTop', isTop);
	};
	
	ucClient.Window.setTrayBalloon = function(icon, title, text, timeOut) {
	    return JSCommond('Window', 'SetTrayBalloon', icon, title, text, timeOut);
	};
	
	ucClient.Window.setCaptions = function(captions) {
		return JSCommond('Window', 'SetCaptions', captions);
	};

	ucClient.Window.navigate = function(url) {
		return JSCommond('Window', 'Navigate', url);
	};
	
	ucClient.Window.maximize = function() {
		return JSCommond('Window', 'Maximize');
	};

	ucClient.Window.fullScreen = function() {
	    return JSCommond('Window', 'FullScreen');
	};
	
	ucClient.Window.restore = function() {
		return JSCommond('Window', 'Restore');
	};
	
	ucClient.Window.minimize = function() {
		return JSCommond('Window', 'Minimize');
	};

	ucClient.Window.setCaretPosition = function(x, y, height) {
	    return JSCommond('Window', 'SetCaretPosition', x, y, height);
	};
	
	ucClient.Window.setSize = function(width, height) {
		return JSCommond('Window', 'SetSize', width, height);
	};
	
	ucClient.Window.centerWindow = function() {
		return JSCommond('Window', 'CenterWindow');
	};
	
	ucClient.Window.hide = function() {
		return JSCommond('Window', 'Hide');
	};
	
	ucClient.Window.hideAll = function() {
	    return JSCommond('Window', 'HideAll');
	};
	
	ucClient.Window.show = function() {
		return JSCommond('Window', 'Show');
	};
	
	ucClient.Window.close = function(name) {
	    if (name == undefined) {
	        name = "";
	    }
	    return JSCommond('Window', 'Close', name);
	};
	
	ucClient.Window.setTray = function(title, imageName, isflash, features) {
		if (features != undefined && features.menu != undefined) {
			ucClient.Window.menus = features.menu;
			ucClient.addEventListener('MenuClicked', function(e){
				var id = e.detail.id;
				for	(index = 0; index < ucClient.Window.menus.length; index++) {
					if (id == ucClient.Window.menus[index].id) {
						ucClient.Window.menus[index].click();
					}
				}
			});	
		}
		
		return JSCommond('Window', 'SetTray', title, imageName, isflash, features);
	};

	ucClient.Window.flashTray = function(isflash, interval) {
	    return JSCommond('Window', 'FlashTray', isflash, interval);
	};
	
	ucClient.Window.flashWindow = function(nCount, nTimeout) {
		return JSCommond('Window', 'FlashWindow', nCount, nTimeout);
	};

	ucClient.Window.disableDBClick = function() {
	    return JSCommond('Window', 'DisableDBClick');
	};

	ucClient.Window.enableDBClick = function() {
	    return JSCommond('Window', 'EnableDBClick');
	};
	
	ucClient.Window.move = function(x, y) {
	    return JSCommond('Window', 'Move', x, y);
	};

	ucClient.Window.setIconWithNumber = function(nCount) {
	    return JSCommond('Window', 'SetIconWithNumber', nCount);
	};
	
	ucClient.Window.getDroppedFiles = function(callback) {
	    JSCommond('Window', 'GetDroppedFiles', function(args){
	        args[1] =  (args[0] == 0) ? JSON.parse(args[1]) : [];
	        callback(args);
	    });
	};

	ucClient.Window.HOTKEY_EVENT = 'HotKey';
	ucClient.Window.hotkey = {MOD_ALT :  0x0001,  MOD_CONTROL: 0x0002, MOD_SHIFT: 0x0004, MOD_WIN: 0x0008};
	ucClient.Window.key = {A:  0x41, ZERO: 0x30, VK_F1: 0x70};
	ucClient.Window.registerHotKey = function(id, hotkey, key) {
	    return JSCommond('Window', 'RegisterHotKey', id, hotkey, key);
	};
	ucClient.Window.unRegisterHotKey = function(id) {
	    return JSCommond('Window', 'UnRegisterHotKey', id);
	};
	
	ucClient.Window.getWindowHwnd = function(name, callback) {
	    return JSCommond('Window', 'GetWindowHwnd', name, callback);
	};

	ucClient.Dialog.runOpenDialog = function(title, filters, isMulti, callback) {
		return JSCommond('Dialog', 'RunOpenDialog', title, filters, isMulti, callback);
	};
	
	ucClient.Dialog.runSaveDialog = function(title, fileName, acceptTypes, callback) {
		return JSCommond('Dialog', 'RunSaveDialog', title, fileName, acceptTypes, callback);
	};
	
	native function IsCapsLockPressed();
	ucClient.KeyBoard.isCapsLockPressed = function() {
	    return IsCapsLockPressed();
	};

	
	native function OpenFolder();
	ucClient.File.openFolder = function(filePath) {
	    return OpenFolder(filePath);
	};

	native function GetFileSize();
	ucClient.File.getFileSize = function(filePath) {
		return GetFileSize(filePath);
	};
	
	native function FileExist();
    ucClient.File.fileExist = function(filePath) {
	    return FileExist(filePath);
	};

    native function DeleteFile();
    ucClient.File.deleteFile = function(filePath) {
        return DeleteFile(filePath);
    };
    
	native function OpenFile();
	ucClient.File.openFile = function(filePath) {
		return OpenFile(filePath);
	};
	
    native function SaveFile();
	ucClient.File.saveFile = function(filePath, content) {
		return SaveFile(filePath, content);
	};

    native function CopyFile();
    ucClient.File.copyFile = function(srcPath, destPath) {
        return CopyFile(srcPath, destPath);
    };

	native function GetDirectoryContent();
	ucClient.File.getDirectoryContent = function(dir) {
		return GetDirectoryContent(dir);
	};
	
    native function CropImage();
    ucClient.Image.crop = function(imagePath, x, y, width, height, targetWidth, targetHeight, savePath) {
        return CropImage(imagePath, x, y, width, height, targetWidth, targetHeight, savePath);
    }

    native function GetSystemFonts();
    ucClient.Font.getSystemFonts = function() {
        return GetSystemFonts();
    }

})();
