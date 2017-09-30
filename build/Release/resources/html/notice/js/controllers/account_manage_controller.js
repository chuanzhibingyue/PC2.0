appModule.controller('AccountManageCtrl', ["api",  "$scope", "notices", "$location",function(api,$scope,notices,$location){
    $scope.account =notices.retrieveSelectedAccount();

}]);
