appModule.controller('RootCtrl', ['$rootScope','notices','api','pages','$translate','log',function($rootScope,notices,api,pages,translate,log){
  //用于客户端广播回调
  window.broadCast = function(type, params) {
      //处理删除的公告板的提示
      if(type == "board_destroy"){
        api.getAccounts(function(data){
          if(data.length > 0){
            notices.openUCAlert(translate.instant("common.account-delete-alert", {name: params.board_name?params.board_name:translate.instant("common.no-name-account")}),function(){});
          }else{
            notices.onLastAccountDeleted();
            notices.openUCAlert(translate.instant("common.account-delete-alert", {name: params.board_name?params.board_name:translate.instant("common.no-name-account")}),handleNoRightAlert);
          }
          $rootScope.$broadcast(type,params);
        },function(msg){
          log.logInfo("get accounts error:"+msg);
        });
        return;
      }
      if(type == "board_update"){
        api.getAccounts(function(data){
          if(data.length > 0){
            var index = data.findIndex(function(ac){
              return ac.id == params.board_id;
            });
            if(index < 0){
              notices.openUCAlert(translate.instant("common.no-account-right-alert", {name:params.board_name?params.board_name:translate.instant("common.no-name-account")}),function(){});
            }
            $rootScope.$broadcast(type,params);
          }
        });
        return;
      }
      $rootScope.$broadcast(type,params);
  };

  //广播鼠标右键菜单
  document.oncontextmenu = function(e) {
    $rootScope.$broadcast("onContextMenu",e);
    return false;
  };

  document.onclick = function(e){
    $rootScope.$broadcast("onDocumentClick",e);
    return false;
  }

  notices.retrieveLocalData(api.getCurrentUser().userId);

  pages.bindGlobalEvent("openCloudWindow",notices.onOpenCloudWindow);

  var offContextChange = $rootScope.$on('changeContext',function(e,param){
    var path = location.hash.replace("#","");
    if(path === param){
      $rootScope.$broadcast("Init");
      return;
    }
    // location.href=location.href.replace(location.hash,"#")+param;
    location.hash = "#"+param;
  });

  var offLostRight = $rootScope.$on("OnLostPublishRight",function(e){
    notices.openUCAlert(translate.instant("common.no-right-alert"),handleNoRightAlert);
  });

  $rootScope.$on("$destroy",function(){
    offContextChange();
    offLostRight();
  });

  function handleNoRightAlert(){
    api.notifyCloseError();
    pages.backToConversation();
  }
}]);
