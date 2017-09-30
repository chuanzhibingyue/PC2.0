appModule.service('pages', ['native',function(native){
  var scopes = {};
  var cbs = {};
  var functionId = 0;

  function windowFnHandler(e){
      var data = e.detail.data;
      scopes[data.windowName] && scopes[data.windowName][data.fnName] && scopes[data.windowName][data.fnName](data.params, function(rlt) {
          native.triggerGlobalEvent('subWindowCallback', {
              fnName: data.fnName,
              data: rlt,
              windowName: data.windowName,
              fnId: data.fnId
          });
      });
  }

  function windowFnCallbackHandler(e){
      var data = e.detail.data;
      cbs[data.fnId] && cbs[data.fnId](data.data);
      delete cbs[data.fnId];
  }

  native.bindGlobalEvent('subWindowFn', windowFnHandler);

  native.bindGlobalEvent('subWindowCallback', windowFnCallbackHandler);

  return{
    openModalDialog:function(url, eventName, width, height, scope,cOptions) {  //打开模态对话框
        var option = !cOptions? {}:cOptions;
        option.caption = option.caption ||[{
            left: 20,
            top: 5,
            width: -90,
            height: 40
        },{
          left: 0,
          top: 0,
          width: 20,
          height: -20
        }];
        var options = {
            width: width,
            height: height,
            refreshIfExist: false,
            centerScreen: true,
            modal: true,
            resizable: true,
            minWidth: width,
            minHeight: height
        };
        var WindowShadowWidth = {
            Top: 5,
            Left: 15,
            Right: 15,
            Bottom: 15
        };
        if (!option.removeBorder) {
            options.width = options.width + WindowShadowWidth.Left + WindowShadowWidth.Right;
            options.height = options.height + WindowShadowWidth.Top + WindowShadowWidth.Bottom;
            options.minWidth = options.minWidth ? options.minWidth + WindowShadowWidth.Left + WindowShadowWidth.Right : options.width;
            options.minHeight = options.minHeight ? options.minHeight + WindowShadowWidth.Top + WindowShadowWidth.Bottom : options.height;
            options.borderSpace={
                leftBorder: WindowShadowWidth.Left,
                topBorder: WindowShadowWidth.Top,
                rightBorder: WindowShadowWidth.Right,
                bottomBorder: WindowShadowWidth.Bottom
            };
        }
        var rootPath = location.pathname.match(/notice\/(.*)/gi)[0];
        var currentPage = rootPath.substr(rootPath.lastIndexOf("/")+1);
        url = rootPath.replace(currentPage,url);
        native.openWindow(url, eventName, $.extend({
            title: eventName,
            left: 0,
            top: 0,
            width: 0,
            height: 0,
            refreshIfExist: false,
            centerScreen: true,
            modal: true,
            resizable: true,
            minWidth: 200,
            minHeight: 200,
            isTop: false,
            enableDrag: false,
            hasTaskBarIcon: true,
            behaviorIfSCClose: 0,
            isChild: false,
            isShow: true,
        }, options,option));
        if(scope){
            scopes[eventName] = scope;
            scope.$on('$destroy', function() {
                delete scopes[eventName];
            });
        }
    },
    fnCall:function(windowName, fnName, params, cb) {
        functionId++;
        native.triggerGlobalEvent('subWindowFn', {
            windowName: windowName,
            fnName: fnName,
            params: params,
            fnId: windowName + functionId
        });
        if (!!cb) {
            cbs[windowName + functionId] = cb;
        }
    },
    closeWindow:function(){
      native.unbindDefaultEvent(windowFnHandler,windowFnCallbackHandler);
      native.closeWindow();
    },
    maximizeWindow:function(){//最大化窗口
      native.maximizeWindow();
    },
    restoreWindow:function(){//恢复当前窗口大小
      native.restoreWindow();
    },
    minimizeWindow:function(){//最小化当前窗口
      native.minimizeWindow();
    },
    setWindowToTop:function(){
      native.setTop(true);
      native.setTop(false);
    },
    backToMain:function(){
      native.hideNoticePanel();
    },
    backToConversation:function(){
      native.goToConversation();
    },
    attachEventScope:function(windowName,scope){
      scopes[windowName] = scope;
    },
    detachEventScope:function(windowName){
      delete scopes[windowName];
    },
    bindGlobalEvent:function(name,handler){
        native.bindGlobalEvent(name,handler);
    },
    unbindGlobalEvent:function(name,handler){
        native.unbindGlobalEvent(name,handler);
    },
    triggerCrossWindowEvent:function(name,param){
        native.triggerGlobalEvent(name,param);
    },
    addWindowRestoreListener:function(handler){
        native.bindWindowRestoreEvent(handler);
    },
    removeWindowRestoreListener:function(handler){
        native.unbindWindowRestoreEvent(handler);
    },
    addWindowMaxListener:function(handler) {
        native.bindWindowMaxListener(handler);
    },
    removeWindowMaxListener:function(handler){
        native.unbindWindowMaxEvent(handler);
    }
  };
}]);
