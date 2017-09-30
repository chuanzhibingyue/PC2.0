appModule.filter('accountName',['$translate',function($translate){
    return function(input){
       return input?input : $translate.instant("account.default-name");
    };
}]);
