appModule.service('utils', ['native','$q',function(native,$q){
  var NOTICE_USER="notice_user";
  var cursorLeft, cursorTop, position;
  function intwidth(h) {
      if (h < 10) {
          h = '0' + h;
      }
      return h;
  }

  return{
    time2str: function(timeLng) {
        var DT = new Date();
        if (timeLng !== undefined) {
            timeLng = parseInt(timeLng);
            var SERVER_TIME_ZONE = -8 * 60;
            var local_time_zone = DT.getTimezoneOffset();
            timeLng = timeLng + ((local_time_zone - SERVER_TIME_ZONE) * 60);
            DT.setTime(timeLng * 1000);
        }
        var y = DT.getYear(); //获取当前年份(2位)
        var Y = DT.getFullYear(); //获取完整的年份(4位,1970-????)
        var m = DT.getMonth() + 1; //获取当前月份(0-11,0代表1月)
        var d = DT.getDate(); //获取当前日(1-31)
        //myDate.getDay();         //获取当前星期X(0-6,0代表星期天)
        //myDate.getTime();        //获取当前时间(从1970.1.1开始的毫秒数)
        var h = DT.getHours(); //获取当前小时数(0-23)
        var i = DT.getMinutes(); //获取当前分钟数(0-59)
        var s = DT.getSeconds(); //获取当前秒数(0-59)
        //myDate.getMilliseconds();    //获取当前毫秒数(0-999)
        //myDate.toLocaleDateString();     //获取当前日期
        m = intwidth(m);
        d = intwidth(d);
        h = intwidth(h);
        i = intwidth(i);

        DT = new Date();
        var Y0 = DT.getFullYear();
        var m0 = DT.getMonth() + 1;
        var d0 = DT.getDate();

        if (Y0 == Y) {
            if (m == m0 && d == d0) {
                return h + ':' + i;
            } else {
                return m + '/' + d + ' ' + h + ':' + i;
            }
        } else {
            return Y + '/' + m + '/' + d + ' ' + h + ':' + i;
        }
    },
    applyScope:function($scope, func) {//预防angularjs 报 $apply already in progress 错误
        if ($scope.$$phase || $scope.$root && $scope.$root.$$phase) {
            func();
        } else {
            $scope.$apply(function() {
                func();
            });
        }
    },
    getObjFromlocalStorage:function(key) {//获取本地数据
        if (!localStorage[key]) {
            return null;
        }
        return JSON.parse(localStorage[key]);
    },
    saveObjTolocalStorage:function(key, value) {//将数据存储到本地
        localStorage[key] = JSON.stringify(value);
    },
    removeObjFromlocalStorage:function(key){ //删除存储数据
      localStorage.removeItem(key);
    },
    retrieveUser:function() {//从session storage 获取user
      return this.getObjFromlocalStorage(NOTICE_USER);
    },
    setInputEditorCaretPosition:function(inputNode){
        if(isNaN(cursorLeft) || isNaN(cursorTop)){
            cursorLeft = inputNode.offset().left + 5;
            cursorTop = inputNode.offset().top + 10;
        }
        native.setCaretPosition(cursorLeft + 10, cursorTop + 8);
    },
    onInputCaretPositionKeyUp:function(inputNode){
        position = inputNode.caret('position');
        cursorTop = inputNode.offset().top + position.top;
        cursorLeft = inputNode.offset().left + position.left;
    },
    uploadFile:function(path, callback){
      native.ucFileUpload(path,callback);
    },
    openImageDialog:function(callback,maxSize,selectMore){
      var sizeLimit = !!maxSize?maxSize:5000000;
      native.ucOpenImageDialog(callback,sizeLimit,selectMore);
    },
    backToApp:function(){
      native.backToApp();
    },
    getFileSuffix:function(fileName){
      if(!fileName || fileName.lastIndexOf('.') == -1){
        return "";
      }

      return fileName.substr(fileName.lastIndexOf('.')+1);
    },
    utf8ToBase64:function(str) {
      return window.btoa(encodeURIComponent(str));
    },
    uploadImages:function(urls){
      var deferred = $q.defer();
      if(urls && urls.length > 0){
        var tasks = [],count=0;
        native.bindGlobalEvent(ucClient.FileTransferService.FILETRANSFER_FILE_UPLOADED_EVENT,function(e){
          var index = tasks.findIndex(function(task){
            return task.taskId === e.detail.taskId;
          });
          if (index>-1) {
              if(e.detail.errorCode === 0){
                tasks[index].newURL = e.detail.downloadURL;
              }else{
                tasks[index].newURL = null;
              }
              count++;
              if(count===urls.length){
                deferred.resolve(tasks);
                  native.unbindGlobalEvent(ucClient.FileTransferService.FILETRANSFER_FILE_UPLOADED_EVENT,arguments.callee);
              }
          }
        });

        var callFileUpload = function(url){
          native.ucFileUpload(url,function(result){
            if (result[0] === 0) {
                tasks.push({
                  taskId:result[1],
                  originalURL:url
                });
            } else {
              tasks.push({
                taskId:-1,
                originalURL:url,
                newURL:null
              });
              count++;
              if(count===urls.length){
                deferred.resolve(tasks);
              }
            }
          });
        };
        urls.forEach(function(url){
          callFileUpload(url);
        });
      }else{
        deferred.resolve();
      }
      return deferred.promise;
    },
    deleteFile:function(path){
      native.deleteFile(path);
    },
    uploadOneFile:function(url){
      var deferred = $q.defer(),taskId=null;
      native.bindGlobalEvent(ucClient.FileTransferService.FILETRANSFER_FILE_UPLOADED_EVENT,function(e){
        if (e.detail.taskId == taskId) {
          if(e.detail.errorCode === 0){
            var data = e.detail;
            var fileInfo = {
                name: data.fileName,
                size: data.fileSize,
                url: data.downloadURL,
                thumbnailURL: data.convertDownloadURL,
                operatorId: data.operatorId,
                operatorTime: data.operatorTime,
                fid: data.fid
            };
            deferred.resolve({
              code: 0,
              data:fileInfo
            });
          }else{
            deferred.resolve({
              code:e.detail.errorCode
            });
          }
          native.unbindGlobalEvent(ucClient.FileTransferService.FILETRANSFER_FILE_UPLOADED_EVENT,arguments.callee);
        }
      });
      native.ucFileUpload(url,function(result){
        if (result[0] === 0) {
          taskId = result[1];
        }else{
          deferred.resolve({
              code:result[0]
          });
        }
      });

      return deferred.promise;
    },
    getFileSize:function(path){
       return native.getFileSize(path);
    },
    isFileExist:function(path){
        return native.fileExist(path);
    },
    getLocaleDateTime:function(d){
        return d.getFullYear()+"/"+(d.getMonth()+1)+"/" + d.getDate()+" "+d.getHours()+":"+d.getMinutes();
    }
  };
}]);
