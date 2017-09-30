/**
 * Created by waouyang on 15/8/12.
 */
(function(uc){
    uc.util.FileDialogUtil = {
        /**
         * open select file dialog
         * @param title         dialog title
         * @param acceptTypes   accepted file types, mime type or suffix example:
         *                       [{
         *                          desc: 'image',
         *                          filter: '*.png, *.jpg'
         *                       }, {
         *                          desc: 'text',
         *                          filter: '*.txt, *.log'
         *                       }]
         * @param multipleSelected single select or multiple select
         * @param callback      callback of the selection, callback(args)
         *                       args[0] is file path, args[1] is the second path
         *                       [] if selection is cancelled by user
         */
        openSelectFileDialog: function(title, acceptTypes, multipleSelected, callback){
            ucClient.Dialog.runOpenDialog(title, acceptTypes, multipleSelected, callback);
        },
        /**
         * open save file dialog
         * @param title         title of the dialog
         * @param fileName      default file name to save
         * @param acceptTypes   accepted file types, mime type or suffix example:
         *                       [{
         *                          desc: 'image',
         *                          filter: '*.png, *.jpg'
         *                       }, {
         *                          desc: 'text',
         *                          filter: '*.txt, *.log'
         *                       }]
         * @param callback      callback of the save operation, callback(args)
         *                       args[0] is saved file path
         */
        openSaveFileDialog: function(title, fileName, acceptTypes, callback){
            ucClient.Dialog.runSaveDialog(title, fileName, acceptTypes, callback);
        }
    }
})(uc);