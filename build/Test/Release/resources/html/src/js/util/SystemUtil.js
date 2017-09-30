/**
 * Created by waouyang on 15/8/27.
 */
(function(uc){
    uc.util.SystemUtil = {
        /**
         *catch screen
         */
        catchScreen: function (callback) {
            var path = uc.IClientService.getDataDirectory(uc.constants.UI.DirectoryType.Image);
            ucClient.CatchScreenService.startCatchScreen(path, function (args) {
                console.log(args);

                if (callback) {
                    callback({
                        errorCode: args[0],
                        state: args[1],
                        path: args[2]
                    });
                }
            });
        },
        /**
         * Clipboard Data
         */
        getClipboardData: function () {
            var data = ucClient.System.getClipboardData();

            if (!data) {
                return null;
            }
            else if (data.format === "CF_HDROP") {
                return {
                    type: uc.constants.UI.ClipboardDataType.File,
                    content: data.items && data.items.length > 0 ? data.items : ""
                };
            }
            else if (data.format === "CF_BITMAP") {
                return {
                    type: uc.constants.UI.ClipboardDataType.Image,
                    content: data.items && data.items.length > 0 ? data.items : ""
                };
            }
            else {
                return {
                    type: uc.constants.UI.ClipboardDataType.Text,
                    content: data.items && data.items.length > 0 ? data.items : ""
                };
            }
        },
        /**
         * flash window
         * @param times
         * @param delay
         */
        flashTaskBar: function(times, delay){
            ucClient.Window.flashWindow(times, delay);
        },

        /*
         * 大写键是否打开
         */
        isCapsLockPressed: function() {
            return ucClient.KeyBoard.isCapsLockPressed();
        },
        registerHotKey: function(id, hotkey, key){
            ucClient.Window.registerHotKey(id, hotkey, key);
        },
        unRegisterHotKey: function(id){
            ucClient.Window.unRegisterHotKey(id);
        },
        setClipboardImageData: function(imagePath) {
            ucClient.System.setClipboardData(imagePath);
        },

        /**
         * 任务栏显示计数
         */
        setIconWithNumber: function(count) {
            ucClient.Window.setIconWithNumber(count);
        },
        /**
         * set input Method caret position
         * @param left
         * @param top
         *
         */
        setInputMethodCaretPosition: function(left, top){
            ucClient.Window.setCaretPosition(parseInt(left), parseInt(top), 16);
        }
    }
})(uc);