/**
 * Created by senlin.ouyang on 2015/7/20.
 */
(function (uc) {
    uc.util.FileUtil = {
        getFileSize: function (filePath) {
            return ucClient.File.getFileSize(filePath);
        },
        openFile: function(filePath) {
            return ucClient.File.openFile(filePath);
        },
        openFolder: function(filePath) {
            return ucClient.File.openFolder(filePath);
        },
        getDirectoryFileList: function(dir){
            return ucClient.File.getDirectoryContent(dir);
        },
        saveFile: function(filePath, content){
            return ucClient.File.saveFile(filePath, content);
        },
        copyFile: function(srcPath, destPath){
            return ucClient.File.copyFile(srcPath, destPath);
        },
        deleteFile: function(filePath){
            ucClient.File.deleteFile(filePath);
        },
        fileExist: function(filePath){
            return ucClient.File.fileExist(filePath);
        },
        getDefaultFileSavePath:function(filename){
            return ucClient.ClientService.getDefaultFileSavePath(filename);
        }
    }

})(uc);