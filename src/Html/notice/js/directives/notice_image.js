appModule.directive("noticeImage",["notices","pages","utils",function(notices,pages,utils){
    return{
        template: "<img/>",
        restrict: "E",
        replace:true,
        scope: {
            isrc:"=",
            defaultSrc:"@",
            size:"@",
            saveToLocal:"@"
        },
        link: function($scope,element){
          if(!$scope.size){
              $scope.size = 80;
          }
          element.on("error",function(){
              if($scope.defaultSrc){
                  this.src = $scope.defaultSrc;
              }
          });
          var stopWatch = $scope.$watch("isrc",function(newValue){
            var newSrc=$scope.defaultSrc;
            if(!!newValue ){
              if($scope.saveToLocal){
                  var localUrl = notices.getDataFromLocalDB(newValue);
                  if(localUrl && utils.isFileExist(localUrl)){
                      newSrc = localUrl;
                  }else{
                      newSrc = newValue;
                      if(newSrc.startsWith("http")){
                          pages.triggerCrossWindowEvent("OnRequestStoreCover",{"path":newSrc});
                      }
                  }
              }else{
              newSrc = notices.modifyImageURL(newValue,$scope.size);
              }
            }
            element.attr("src",newSrc);
          });
           $scope.$on("destroy",function(){
               stopWatch();
           });
        }
    };
}]);
