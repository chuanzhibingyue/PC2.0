/**
 * Created by senlin.ouyang on 2015/7/17.
 */
(function(uc){
    uc.util.EventUtil = {
        isString: function(obj){
            return !!arguments.length && obj != null && (obj instanceof String || typeof obj == 'string');
        },
        getObject: function(name, context, create){
            return this._getProperty(name.split('.'), context, create);
        },
        _getProperty: function(parts, context, create){
            var obj = context || window;
            for(var i = 0, p; obj && (p = parts[i]); i++){
                obj = p in obj ? obj = obj[p] : (create ? obj[p] = {} : undefined);
            }
            return obj;
        },
        hitch: function(context, method){
            if(arguments.length > 2){
                return null;
            }
            if(!method){
                method = context;
                context = null;
            }
            if(this.isString(method)){
                context = context || window;
                if(!context[method]){
                    return null;
                }
                return function(){
                    return context[method].apply(context, arguments || []);
                }
            }
            return !context ? method : function(){
                return method.apply(context, arguments || []);
            }
        },
        connect: function(obj, event, context, method, priority){
            var a = arguments, args = [], i = 0;
            args.push(this.isString(a[0]) ? null : a[i++], a[i++]);
            var a1 = a[i + 1];
            args.push((this.isString(a1) || jQuery.isFunction(a1) ? a[i++] : null), a[i++]);
            for(var l = a.length; i < l; i++){
                args.push(a[i]);
            }
            return uc.util.EventUtil._connect.apply(this, args);
        },
        _connect: function(obj, event, context, method, priority){
            var listener = this.hitch(context, method);
            var handler = this._addListener(obj, event, listener, priority);
            return [obj, event, handler];
        },
        _addListener: function(obj, event, listener, priority){
            obj = obj || window;
            var func = obj[event];
            if(!func || !func._listeners){
                var d = this._getDispatcher();
                d.target = func;
                d._listeners = {};
                func = obj[event] = d;
            }
            if(!func._listeners[priority]){
                func._listeners[priority] = [];
            }
            return func._listeners[priority].push(listener);
        },
        _getDispatcher: function(){
            return function(){
                var callee = arguments.callee, _listeners = callee._listeners, target = callee.target;
                var result = target && target.apply(this, arguments);
                var arg = arguments;
                var priorityArray = [];
                var listenerArray;
                var defferCall = function(){
                    for(var priority in _listeners){
                        priorityArray.push(priority);
                    }
                    priorityArray.sort().reverse();
                    for(var i = 0, priorityLen = priorityArray.length; i < priorityLen;i++){
                        listenerArray = _listeners[priorityArray[i]];
                        for(var j = 0, listenerLen = listenerArray.length; j < listenerLen; j++){
                            try{
                                listenerArray[j].apply(this, arg);
                            }catch (e){
                                uc.util.LogUtil.error('EventUtil', 'DispatchEvent', 'Dispatch event failed, error message = ' + e.message + ', call stack =', e.stack);
                            }
                        }
                    }
                };
                setTimeout(defferCall, 0);
                return result;
            }
        },
        disconnect: function(handler){
            uc.util.EventUtil._disconnect.apply(this, handler);
        },
        _disconnect: function(obj, event, handler){
            var func = obj[event];
            if(func && func._listeners && handler--){
                func._listeners.splice(handler,1);
            }
        },
        /**
         * trigger event from one window to another
         * @param eventName event name
         * @param eventData event data, can be JSON or other data format
         */
        triggerCrossWindowEvent: function(eventName, eventData){
            ucClient.dispatchGlobalEvent(eventName, eventData);
        },
        /**
         * bind cross window event
         * @param eventName event name
         * @param callback  callback for the event, result is callback(data), data is from dispatch event
         */
        bindCrossWindowEvent: function(eventName, callback){
            ucClient.addEventListener(eventName, function(e){
                callback && callback(e.detail.data);
            });
        },
        bindNativeEvent: function(eventName, callback){
            ucClient.addEventListener(eventName, function(data){
                callback && callback(data.detail);
            });
        }
    }
})(uc);