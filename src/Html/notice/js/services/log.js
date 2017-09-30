appModule.service('log',['native',function(native){
  return{
    logInfo:function(content) {
        native.log(0, content, 5);
    },
    logWarning:function(content) {
        native.log(1, content, 5);
    },
    logError:function(content) {
        native.log(2, content, 5);
    }
  }
}]);
