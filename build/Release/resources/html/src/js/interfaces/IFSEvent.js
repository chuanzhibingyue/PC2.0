/**
 * Created by waouyang on 15/8/7.
 */
(function(uc){
    uc.interfaces.IFSEvent = {
        /**
         * event for downloading file
         * @param errorCode error code of file downloading event
         * @param taskId    task id of this download
         * @param percent   percent of downloading
         * @constructor
         */
        OnFileDownloading: function(errorCode, taskId, percent){},
        /**
         * event for file downloaded
         * @param errorCode error code of the download
         * @param taskId    task id of the download
         * @param savePath  local file path
         * @constructor
         */
        OnFileDownloaded: function(errorCode, taskId, savePath){},
        /**
         * event for uploading file
         * @param errorCode error code of the uploading
         * @param taskId    task id of the uploading
         * @param percent   current percentage of the uploading
         * @constructor
         */
        OnFileUploading: function(errorCode, taskId, percent){},
        /**
         * event for file uploaded
         * @param errorCode             error code of the uploading
         * @param taskId                task id
         * @param fileName              local file name
         * @param fileSize              file size
         * @param operatorId            operator id
         * @param fid                   file id
         * @param operatorTime          operation time
         * @param downloadURL           download url
         * @param convertDownloadURL    converted download url
         * @constructor
         */
        OnFileUploaded: function(errorCode, taskId, fileName, fileSize, operatorId, fid, operatorTime, downloadURL, convertDownloadURL){}
    }
})(uc);