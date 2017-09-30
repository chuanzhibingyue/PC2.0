appModule.filter('coverImage',['notices',function(notices){
  return function(input){
    return notices.modifyImageURL(input,360);
  };
}]);
