appModule.directive("rawHtml",["notices","api","pages","$translate",function(notices,api,pages,translate){
    return{
        restrict: "A",
        scope: {
            content:"=",
        },
        link: function($scope,element){
            var stopWatch = $scope.$watch("content",function(newV,oldV){
                 if(!!newV){
                    $(element).html(newV);
                    $("a[href^='http://gnet-uc-native.quanshi.com?act=cloudFileDownload']").on("click",function(){
                      notices.downloadCloudFile(this.href,function(){
                        pages.setWindowToTop();
                        api.notifyError(translate.instant("common.cloud-file-downloaded"));
                      });
                      return false;
                    });
                 }else{
                    $(element).empty();
                 }
            });

            $scope.$on("$destroy",function(){
                stopWatch();
            });
        }
    };
}]);
