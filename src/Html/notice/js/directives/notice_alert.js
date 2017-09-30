appModule.directive("noticeAlert",['utils','$translate',function(utils,translate){
    return{
        templateUrl: "templates/alert.html",
        restrict: "AE",
        scope: {
            isShow:"@",
            msg:"@"
        },
        link: function($scope){
            $scope.isShow = false;
            $scope.defaultOption={
                confirmTxt: translate.instant("common.confirm"),
                cancelTxt:  translate.instant("common.cancel"),
                msg:"",
                onConfirm:null,
                onCancel:null,
                confirmStyle:null,
                cancelStyle:null,
                isShowConfirm:true,
                isShowCancel:true
            };
            var offOnError = $scope.$on("onError",function(event,options){
                utils.applyScope($scope, function(){
                    $scope.option = $.extend({},$scope.defaultOption,options);
                    $scope.show();
                });
            });

            var offOnCloseError = $scope.$on("onCloseError",function(event){
                $scope.hide();
            });

            $scope.$on("$destroy",function(){
                offOnError();
                offOnCloseError();
            });

            $scope.show = function(){
                $scope.isShow = true;
            };

            $scope.hide = function(type,e){
                if(e){
                    e.stopPropagation();
                }
                if(type == 0 && $scope.option.onCancel){
                    $scope.option.onCancel();
                }else if(type == 1 &&$scope.option.onConfirm){
                    $scope.option.onConfirm();
                }

                // $scope.option.onCancel = $scope.option.onConfirm = null;
                $scope.isShow = false;
            }
        }
    }
}]);
