appModule.service('api', ['$http', '$q', '$rootScope','native','notices','log',"$translate", function($http, $q, $rootScope,native,notices,log,$translate) {
    //加载配置文件
    var SERVER_CONFIG = null;
    var user = {};
    var isSessionExpired = true;
    var successCode = 0;
    var getConfig = function() {
        var deferred = $q.defer();
        if (SERVER_CONFIG !== null) {
            deferred.resolve();
        } else {
            native.getUCCDomain(function(uccServerIP){
                if(!!uccServerIP){
                    SERVER_CONFIG = {
                      cover_address:uccServerIP+"/microblog/theme/default/images/bbs/"
                    };
                }
                deferred.resolve();
            });
        }
        return deferred.promise;
    };

    function generateRequest (controller,action,url,method,body){
      var req =  {
          controller:controller,
          action:action,
          url:$.extend({
              v:1
          },url),
          method:method
      };
      if(body){
        req.body = body;
      }
      return req;
    }

    function sendRequest(request,onSuccess,onError){
      var requestString = JSON.stringify(request);
      log.logInfo("send request:"+requestString);
      native.sendRequest(requestString,function(args){
        if(args[0] === 0){
          log.logInfo("send task_id:"+args[1]);
          native.addBBSEventHandler(args[1],function(response){
             log.logInfo("receive task_id:"+args[1]);
             log.logInfo("receive BBS response:"+JSON.stringify(response));
             if (response.code === successCode) {
                 onSuccess(response.data);
             } else {
                 log.logError("receive error code:"+response.code+"; error message:"+response.error);
                 var errorKey = "common.error." + response.code;
                 var translateMsg = $translate.instant(errorKey);
                 var errorMsg = translateMsg != errorKey ? translateMsg:response.error;
                 onError(errorMsg);
             }
             delete native.bbsCallback[args[1]];
          });
        }
      });
    }

    var api = {
        //获取当前用户id
        getCurrentUser: function() {
            return native.getUserInfo();
        },
        saveAccount: function(account,onSuccess,onError) {
            var request = generateRequest ("bbs/board","update.json",null,1,{
                id:account.id,
                avatar:account.avatar,
                board_name:account.name
            });
            sendRequest(request,onSuccess,onError);
        },
        notifyNewNoticeSaved: function(notice) {
            $rootScope.$broadcast("newNoticeSaved", notice);
        },
        notifyError: function(options) {
            var option = null;
            if(typeof options == "string"){
                option = {
                    msg:options,
                    isShowCancel:false
                }
            }else{
                option = options;
            }
            $rootScope.$broadcast("onError", option);
        },
        notifyCloseError:function(){
            $rootScope.$broadcast("onCloseError");
        },
        sendNotice: function(notice, onSuccess, onError) {
            log.logInfo("prepare to send notice:"+notice.title);
            var request = generateRequest ("bbs/bbs","create.json",null,1,notice);

            sendRequest(request,function(data){
                log.logInfo('success to send notice,topic:' + notice.title+"; id="+data.id);
                onSuccess(data);
            },function(data){
                log.logInfo('fail to send notice,topic:' + notice.title);
                onError(data);
            });
        },
        getNotices: function(accountId, criteria, onSuccess, onError) {
            var url = {
              count:20,
              board_id:accountId
            };
            if(criteria.keyword){
                url.keyword = criteria.keyword;
                if(!criteria.isNew && criteria.start){
                    url.start = criteria.start;
                }
            }else{
                if(criteria.startTime){
                    url.start_time = criteria.startTime;
                }
                if(criteria.editorId){
                    url.editor_id = criteria.editorId;
                }
                if(criteria.isNew){
                    url.since_id = criteria.lastNoticeId;
                }else{
                    url.max_id = criteria.lastNoticeId;
                }
            }


            var request = generateRequest ("bbs/bbs","list_by_editor.json",url,0);
            sendRequest(request,function(data){
                var list = [];
                if (Array.isArray(data) && data.length > 0) {
                    $.each(data, function(index, item) {
                        var n = notices.notice.create();
                        notices.notice.convertFromNewData(n, item);
                        list.push(n);
                    });
                }
                onSuccess(list);
            },onError);
        },
        getAccounts: function(onSuccess, onError) {
            var url = {
                count:Number.MAX_SAFE_INTEGER,
            };
            var request = generateRequest("bbs/board","lists.json", url,0);
            sendRequest(request,onSuccess,onError);
        },
        getReceiverAccounts:function(since, max, onSuccess,onError){
          var url = {
            "start_time":since,
            "end_time":max,
            "count":10
          };
          var request = generateRequest("bbs/board","new_lists.json",url,0);
          sendRequest(request,onSuccess,onError);
        },
        getReceiverNotices:function(accountId,since,max,onSuccess,onError){
            var url ={
              "since_id":since,
              "max_id":max,
              "count":10,
              "board_id":accountId
            };
            // var request = generateRequest("bbs","lists.json",url,0);
            var request = generateRequest("bbs/bbs","list_by_reader.json",url,0);
            sendRequest(request,function(data){
                var list = [];
                if (Array.isArray(data) && data.length > 0) {
                    for(var i=data.length-1; i>=0;i--){
                      var n = notices.notice.create();
                      notices.notice.convertFromNewData(n, data[i]);
                      list.push(n);
                    }
                }
                onSuccess(list);
            },onError);
        },
        deleteNotice:function(n,onSuccess,onError){
            var noticeId = n.id;
             if(!!noticeId){
                var url={
                  id:noticeId
                };
                var request = generateRequest ("bbs/bbs","destroy.json",url,0);
                sendRequest(request,onSuccess,onError);
             }else{
                //删除草稿
                notices.removeNoticeDraft(n);
                onSuccess();
             }
        },
        getDefaultCovers:function(onSuccess){
            var configPromise = getConfig();
            configPromise.then(function(){
                var coverAddress = SERVER_CONFIG.cover_address;
                onSuccess([{
                    url:coverAddress+"1.jpg"
                },{
                    url:coverAddress+"2.jpg"
                },{
                    url:coverAddress+"3.jpg"
                },{
                    url:coverAddress+"4.jpg"
                },{
                    url:coverAddress+"5.jpg"
                },{
                    url:coverAddress+"6.jpg"
                }]);
            });
        },
        search:function(action,key,orgIDs,onSuccess,onError){
          var request = generateRequest ("base/org",action+".json",{count:100,keyword:key,org_ids:orgIDs},0);
          if(this[action+"_taskID"]){
             delete native.bbsCallback[this[action+"_taskID"]];
          }
          var requestString = JSON.stringify(request);
          log.logInfo("send request:"+requestString);
          native.sendRequest(requestString,function(args){
            if(args[0] === 0){
                this[action+"_taskID"] = args[1];
                console.log("send request"+ args[1]);
                log.logInfo("send task_id:"+args[1]);
                native.addBBSEventHandler(args[1],function(response){
                    log.logInfo("receive task_id:"+args[1]);
                    log.logInfo("receive BBS response:"+JSON.stringify(response));
                    if (response.code === successCode) {
                        onSuccess(response.data);
                    } else {
                        onError(response.error);
                    }
                    delete native.bbsCallback[args[1]];
                });
            }
          },function(response){
            log.logInfo("fail to load search result:"+response.error);
          });
        },
        getOneNotice:function(id,onSuccess,onError){
          var request = generateRequest("bbs/bbs","info.json", {id:id},0);
          sendRequest(request,onSuccess,onError);
        },
        getAccountUnreadCount:function(accountIds,onSuccess,onError){
          var request = generateRequest("bbs/stat","board_unread.json", null,1,{board_ids:accountIds});
          sendRequest(request,onSuccess,onError);
        },
        getAccountUnreadInfo:function(accountId,onSuccess,onError){
          var request = generateRequest ("bbs/stat","total_unread.json",{board_id:accountId},0);
          sendRequest(request,onSuccess,onError);
        },
        getAvatarIcons:function(onSuccess,onError){
            var request = generateRequest ("bbs/pic","board_avatar.json",{},0);
            sendRequest(request,onSuccess,onError);
        },
        getNoticeCovers:function(onSuccess,onError){
            var request = generateRequest ("bbs/pic","bbs_thumb.json",{},0);
            sendRequest(request,onSuccess,onError);
        },
        removeNoticeCovers:function(id,onSuccess,onError){
            var request = generateRequest ("bbs/pic","destroy.json",{id:id},0);
            sendRequest(request,onSuccess,onError);
        },
        getAccountAllEditors:function(accountId,onSuccess,onError){
            var request = generateRequest ("bbs/bbs","editor_list_all.json",{board_id:accountId},0);
            sendRequest(request,onSuccess,onError);
        }
    };


    return api;
}]);
