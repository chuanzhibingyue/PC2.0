appModule.service('native',function(){
  var UC = window.top.uc;
  var inputting = false;
  function getErrorObject() {
      try {
          throw Error('');
      } catch (err) {
          return err;
      }
  };
  var nativeService = {
    getUCCDomain:function(callback){//获取ucc domain server 地址
      ucClient.ClientService.getUccServerDomain(function(data){
         if(data[0] == 0){
            callback(data[1].replace(':443','').replace('https://','http://'));
         }else{
            callback(null);
         }
      });
    },
    getUserInfo:function () {//获取当前用户信息
        var currentUser = ucClient.ContactService.getCurrentUserInfoSync();
        localStorage["notice_user"] = JSON.stringify(currentUser);
        return currentUser;
    },
    getSessionInfo:function(callback) {//获取session 信息
        ucClient.ClientService.getSessionId(function(data) {
            localStorage["notice_sessionId"] = data[1];
            callback(data);
        });
    },
    ucFileUpload:function(path, callback) {//调用一期接口上传文件
        ucClient.FileTransferService.uploadFile(path, callback);
    },
    ucOpenFileDialog:function(title, types, callback,multiple) {//调用一期接口打开文件对话框
        var selectMore = multiple?true:false;
        ucClient.Dialog.runOpenDialog(title, types, selectMore, callback);
    },
    ucOpenImageDialog:function(callback,sizeLimit,selectMore) {//上传图片
        this.ucOpenFileDialog("打开图片", [{
            desc: '图片(*.png; *.jpg; *.jpeg; *.gif)',
            filter: '*.png; *.jpg; *.jpeg; *.gif'
        }], function(paths) {
            if (paths && paths.length > 0) {
                for(var i=0; i < paths.length;i++){
                  if(ucClient.File.getFileSize(paths[0]) > sizeLimit){
                    callback({
                        errorCode: -1,
                        msg: "文件尺寸超出范围"
                    });
                    return;
                  }
                }
                callback({
                    errorCode: 0,
                    paths: paths
                });
            }
        },selectMore);
    },
    saveLocalStore:function(key,value){//将数据持久化到native db
        ucClient.ClientService.updateValueByKey("notice",key,value);
        localStorage[key] = value;
    },
    getLocalStore:function(key){
    //    return ucClient.ClientService.getValueByKeySync("notice",key);
        return localStorage[key];
    },
    retrieveLocalData:function(key){//临时方法 解决蜜蜂每次启动清空localstorage的问题
        return ucClient.ClientService.getValueByKeySync("notice",key);
    },
    getCropperPath:function() {//获取切图临时存储的路径
        return ucClient.ClientService.getDocumentDataDirSync(3) + '/cropped/temp_notice';
    },
    bindGlobalEvent:function(eventName, callback) {//绑定全局事件
        if (!!top) {
            top.ucClient.addEventListener(eventName, callback);
        }
    },
    triggerGlobalEvent:function(eventName, param) {//触发全局事件
        if (!param) {
            param = {};
        }
        if (!!top) {
            top.ucClient.dispatchGlobalEvent(eventName, param);
        }
    },
    unbindGlobalEvent:function(eventName, callback) {//取消绑定全局事件
        if (!!top) {
            top.ucClient.removeEventListener(eventName, callback);
        }
    },
    unbindDefaultEvent:function(callFn,callbackFun){
         this.unbindGlobalEvent("subWindowFn",callFn);
         this.unbindGlobalEvent("subWindowCallback",callbackFun);
    },
    closeWindow:function() {  //关闭当前窗口
        ucClient.Window.close();
    },
    maximizeWindow:function(){//最大化窗口
      ucClient.Window.maximize();
    },
    restoreWindow:function(){//恢复当前窗口大小
      ucClient.Window.restore();
    },
    minimizeWindow:function(){//最小化当前窗口
      ucClient.Window.minimize();
    },
    log:function(logLevel, content, caller){//日志
      caller = caller || 4;
      var err = getErrorObject();
      var caller_line = err.stack.split("\n")[caller];
      var index = caller_line.indexOf("at ");
      var content = caller_line.slice(index + 2, caller_line.length) + ']]' + content;
      ucClient.Log.writeLog('NOTICELOG', 0, logLevel, content);
    },
    openWindow:function(url,eventName,options){//打开新窗口
      ucClient.Window.open(url, eventName,options,options.closeOtherWindow ? 1 : 0);
    },
    setCaretPosition:function(left, top){//设置光标位置(修复输入法位置错误)
        if(inputting){
            return;
        }
        setTimeout(function(){
            inputting = false;
        }, 300);
        ucClient.Window.setCaretPosition(parseInt(left), parseInt(top), 16);
        inputting = true;
    },
    getOrgInfo:function(id,callback){
        if(!this.getOrgInfoCallbacks){
            this.getOrgInfoCallbacks ={};
        }
        this.getOrgInfoCallbacks[id] = callback;
        var that = this;
        that.log(0, "start get org info "+ id,5);
        this.bindGlobalEvent(ucClient.ContactService.ORG_ADDRESS_BOOK_RECEIVED_EVENT,function(e){
            if(e.detail && e.detail.errorCode == 0){
                var deptId = e.detail.department.id;
                if(that.getOrgInfoCallbacks[deptId]){
                    that.getOrgInfoCallbacks[deptId](e.detail.department);
                    delete that.getOrgInfoCallbacks[deptId];
                    that.log(0, "end get org info "+ deptId,5);
                    that.unbindGlobalEvent(ucClient.ContactService.ORG_ADDRESS_BOOK_RECEIVED_EVENT,arguments.callee);
                }
            }
        });
        ucClient.ContactService.getOrgAddressBook(id);
    },
    backToApp:function(){
      UC.ui.Notice.MainPanel.addClass("hide");
    },
    openCloudDialog:function(opts){
      UC.modules.cloud.CloudManager.selectCloudFile(opts);
    },
    noticeChatType:function(){
      return 16; //UC.constants.Chat.ChatType.notice;
    },
    setTop:function(isSetTop){
      ucClient.Window.setTop(isSetTop);
    },
    raiseCloudFileDownloadEvent:function(data){
       this.triggerGlobalEvent("noticeCloudInfoDownload",{"data":data});
    },
    openAvatarChangesWindow:function(eventName,callback){
      var that = this;
      that.avatarCallback = callback;
      UC.IUIService.openImageCropperWindow(eventName, function (args) {
        if (args.okOrCancel) {
          var croppedImagePath = args.croppedImagePath;
          that.log(0,"crop account image:"+croppedImagePath,5);
          var taskId = "";
          that.bindGlobalEvent(ucClient.FileTransferService.FILETRANSFER_FILE_UPLOADED_EVENT,function(e){
            if (e.detail.errorCode === 0) {
                var data = e.detail;
                if(data.taskId == taskId){
                  that.unbindGlobalEvent(ucClient.FileTransferService.FILETRANSFER_FILE_UPLOADED_EVENT,arguments.callee);
                  that.avatarCallback(data.downloadURL);
                }
            }
          });
          that.ucFileUpload(croppedImagePath,function(data){
              if(data[0]===0){
                taskId = data[1];
              }
          });
      }else{
            that.avatarCallback(null);
      }
    });
  },
  sendRequest:function(request,callback){
    ucClient.BBSService.BBSRequest(request,callback);
  },
  addBBSEventHandler:function(taskId,callback){
    if(!this.bbsCallback){
      this.bbsCallback = {};
    }
    this.bbsCallback[taskId] = callback;
  },
  hideNoticePanel:function(){
    UC.modules.notice.NoticeManager.hidePanel();
  },
  showPreview:function(accountName,url){
    try {
        UC.IClientService.getSessionId(function (token){
            var currentUserInfo = UC.IContactService.getCurrentUserInfo();
            UC.util.StorageUtil.setData("ShowApiNoticePreview", {
                url: url,
                token: token[1],
                account: currentUserInfo.account,
                noticeAccountName:accountName
            });
            var options = {
                width: 750,
                height: 650,
                refreshIfExist: false,
                centerScreen: true,
                modal: true,
                resizable: true,
                minWidth: 750,
                minHeight: 650,
                caption:UC.constants.UI.DragCaption.ApiNoticePreview,
                removeBorder:true
            };
            UC.util.WindowUtil.open('page/apiNoticePreview.html', 'ApiNoticePreview', options);
        });
    } catch (e) {
        uc.util.LogUtil.info('ChatManager', 'getSessionId', "" + e.message, null);
    }
  },
  deleteFile:function(fileName){
    ucClient.File.deleteFile(fileName);
  },
  getConversationTitle:function(){
    return UC.modules.notice.Notice.appInfo.appTitle;
  },
  getNoticeUnreadCount:function(){
      return UC.modules.notice.NoticeManager.getNoticeUnreadCount();
  },
  setNoticeUnreadCount:function(count){
      UC.modules.notice.NoticeManager.setNoticeUnreadCount();
  },
  goToConversation:function(){
      if(UC.modules.main.Main.moduleId ==UC.constants.UI.ModuleId.Notice){
      UC.IUIService.changeModule(UC.constants.UI.ModuleId.Message);
      }
  },
  openUCAlert:function(title,msg,confirmText,callback){
      UC.modules.component.PromptMessageBox.open({title: title, message: msg, buttonMode:"ok",OkText:confirmText,status:'Light-Blue'},
          function(result){
              callback(result)
              this.close();
      });
  },
  getFileSize:function(path){
      return ucClient.File.getFileSize(path);
  },
  getBasicUserInfo:function(userId){
      return ucClient.ContactService.getBasicContactInfoSync(userId);
  },
  fileExist:function(path){
      return ucClient.File.fileExist(path);
  },
  bindWindowRestoreEvent:function(handler){
      if(this.windowRestoreEventHandler){
          this.unbindWindowRestoreEvent(this.windowRestoreEventHandler);
      }
      this.windowRestoreEventHandler = handler;
      this.bindGlobalEvent(ucClient.Window.Status.RESTORED_FROM_MAXIMIZED,handler);
  },
  unbindWindowRestoreEvent:function(handler){
      this.unbindGlobalEvent(ucClient.Window.Status.RESTORED_FROM_MAXIMIZED,handler);
  },
  bindWindowMaxListener:function(handler){
      if(this.windowMaxEventHandler){
          this.unbindWindowMaxEvent(this.windowMaxEventHandler);
      }
      this.windowMaxEventHandler = handler;
      this.bindGlobalEvent(ucClient.Window.Status.MAXIMIZED_FROM_NORMAL,handler);
  },
  unbindWindowMaxEvent:function(handler){
      this.unbindGlobalEvent(ucClient.Window.Status.MAXIMIZED_FROM_NORMAL,handler);
  },
  onDeleteLastAccount:function(){
      UC.modules.notice.NoticeManager.onDeleteLastAccount();
  }    
  };
  nativeService.cloudFileEvents= {
    sendFromMount:null,
    noticeAlert:null,
    cancelSelectCloudFile:null
  };

  var cloudFileEventHandler = function(eventData){
    if(nativeService.cloudFileEvents[eventData.type]){
        nativeService.cloudFileEvents[eventData.type](eventData);
        //cancel 和 sendFromMount 同一时间触发一次 执行完后需要同时释放
        if(eventData.type == nativeService.cloudFileEvents.sendFromMount || eventData.type == nativeService.cloudFileEvents.cancelSelectCloudFile){
            nativeService.cloudFileEvents.sendFromMount = null;
            nativeService.cloudFileEvents.cancelSelectCloudFile = null;
        }else{
            nativeService.cloudFileEvents[eventData.type] = null;
        }
    }
  };
  for(var evt in nativeService.cloudFileEvents){
      nativeService.bindGlobalEvent(evt,cloudFileEventHandler);
  }

  nativeService.bindGlobalEvent(ucClient.BBSService.BBS_SERVICE_MAIN_EVENT,function(eventData){
      if(eventData.detail && eventData.detail.errorCode === 0 && eventData.detail.requestID){
        // console.log("receive request"+ eventData.detail.requestID);
        if(nativeService.bbsCallback && nativeService.bbsCallback[eventData.detail.requestID]){
          nativeService.bbsCallback[eventData.detail.requestID](eventData.detail.response);
        }
      }
  });

  return nativeService;
});
