/**
 * Created by waouyang on 15/8/7.
 */
(function(uc){
    uc.interfaces.IFSService = {
        /**
         * download a from from FS server or other server
         * @param downloadUrl   download url
         * @param savePath      local save path
         * @param callback      callback of the call, return value is array
         *                       array[0] is errorCode, array[1] is taskId
         */
        downloadFile: function(downloadUrl, savePath, callback){},
        /**
         * upload a file to Fs server
         * @param filePath  local file path
         * @param callback  callback of the call, return value is array
         *                   array[0] is errorCode, array[1] is taskId
         */
        uploadFile: function(filePath, callback){},
        /**
         * cancel download file
         * @param taskId    taskId
         */
        cancelDownloadFile: function(taskId){},
        /**
         * cancel upload file
         * @param taskId
         */
        cancelUploadFile: function(taskId){},
        /**
         * check if file is downloading
         * @param taskId
         */
        isFileDownloading: function(taskId){},
        /**
         * check if file is uploading
         * @param taskId
         */
        isFileUploading: function(taskId){},

        /**
         * get file local path by file url
         * @param fileURL
         */
        getLocalFilePath: function(fileURL){},
        /**
         * save local url for the file's server url
         * @param serverUrl
         * @param localUrl
         */
        updateFileLocalPath: function(serverUrl, localUrl){},

        getDownloadUrlFromFid: function(fid, callback){},

        resumeTask: function(taskId, callback){}
    }
})(uc);