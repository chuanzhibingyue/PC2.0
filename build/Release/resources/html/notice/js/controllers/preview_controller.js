appModule.controller('previewCtrl',["$scope","notices","pages","api","log","utils","$translate","$sce",function($scope,notices,pages,api,log,utils,translate,$sce){
  $scope.notice = notices.getTempNotice();
  if(!!$scope.notice){
    notices.deleteTempNotice();
  }
  if($scope.notice.id){
    api.getOneNotice($scope.notice.id,function(data){
      utils.applyScope($scope,function(){
        $scope.notice =notices.notice.create();
        notices.notice.convertFromData($scope.notice,data);
        if($scope.notice.unreadCount == -1){
          $scope.notice.unreadCount = translate.instant("preview.sending");
        }
      });
    },function(msg){
      log.logInfo("failed to load notice "+$scope.notice.id+"'s detail:"+msg);
    });
  }

  $scope.plainHTML = function(html){
    return $sce.trustAsHtml(html);
  };

  $scope.onClose = function(){
    pages.closeWindow();
  };
  $scope.toggleMaximize = function(){
    $scope.isMinimized = false;
    if($scope.isMaximized){
      $scope.isMaximized = false;
      pages.restoreWindow();
      return;
    }

    $scope.isMaximized = true;
    pages.maximizeWindow();
  };
  $scope.minimize = function(){
    $scope.isMinimized = true;
    $scope.isMaximized = false;
  };
}]);
