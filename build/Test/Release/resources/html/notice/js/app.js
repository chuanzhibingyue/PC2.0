'use strict'
//创建angularjs app module
if(!window.appModule){
    window.appModule = angular.module('appModule', ['ngSanitize','ngAnimate','pascalprecht.translate']);
}
//记录Angular的错误
appModule.factory('$exceptionHandler', ['log',function(log) {
    return function(e, cause) {
     log.logError(e.stack);
     throw e;
   };
}]);

//隐藏鼠标右键菜单
document.oncontextmenu = function() {
    return false;
};

document.onkeydown = function(e){ //阻止CEF 默认回退键执行 返回 操作
    if(e.keyCode == 8 && e.target.tagName.toLowerCase() != "input"){
        e.preventDefault();
    }
};
var languagePromise = (function($){
  var defer = $.Deferred();
  $.getJSON("nls/zh-cn.json",function(data){
    defer.resolve(data);
  });
  return defer.promise();
})(jQuery);

//加载语言文件，引导angularjs
angular.element(document).ready(function(){
  languagePromise.then(function(data){
    window.appModule.config(['$translateProvider',function ($translateProvider) {
      $translateProvider.translations('zh', data);
       $translateProvider.preferredLanguage('zh');
    }]);
    angular.bootstrap(document, ['appModule']);
  });
});

ucClient.addEventListener("ClosePublishWindows",function(){
   if(window.top !== window){
       return;
   }
   ucClient.Window.close();
});