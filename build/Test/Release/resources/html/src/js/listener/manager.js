/**
 * Created by waouyang on 15/1/21.
 */
(function(uc){
    var _this = null;
    uc.listener.manager = {
        _service: {},
        init: function(){
            _this = this;
            this._createEvent();
            this.bindNativeService();
            this._createService();
            this._createListener();
        },
        _createEvent: function(){
            for(var eventType in uc.interfaces){
                if(!/^I[\w\d]+Event$/.test(eventType)){
                    continue;
                }
                uc[eventType] = {};
                for(var event in uc.interfaces[eventType]) {
                    uc[eventType][event] = uc.interfaces[eventType][event];
                    if(eventType == 'IUIEvent'){
                        continue;
                    }
                    this._bindNativeEvent(eventType, event);
                }
            }
        },

        bindNativeService: function(){
            for(var serviceType in uc.interfaces){
                if(!/^I[\w\d]+Service$/.test(serviceType)){
                    continue;
                }
                if(serviceType == 'IUIService'){
                    continue;
                }
                uc[serviceType] = {};
                for(var method in uc.interfaces[serviceType]){
                    uc[serviceType][method] = function(){};
                    this._bindNativeService(serviceType, method);
                }
            }
        },

        _createService: function(){
            for(var i = 0; i < uc.listener.service.config.length; i++){
                var config = uc.listener.service.config[i];
                var clazz = config.clazz;
                var service = uc.util.EventUtil.getObject(clazz, uc, false);
                if(service){
                    service = new service();
                }else{
                    continue;
                }
                var interfaceTypes = config.type, interfaceType;
                for(var j = 0; j < interfaceTypes.length; j++){
                    interfaceType = interfaceTypes[j];
                    this._registerService(service, interfaceType);
                }
            }
        },
        _registerService: function(context, interfaceType){
            if(!uc.interfaces[interfaceType]){
                return;
            }
            if(this._service[interfaceType]){
                return;
            }
            this._service[interfaceType] = context;
            uc[interfaceType] = {};
            for(var method in uc.interfaces[interfaceType]){
                uc[interfaceType][method] = uc.util.EventUtil.hitch(context, method);
            }
        },

        _createListener: function(){
            for(var i = 0; i < uc.listener.event.config.length; i++){
                var config = uc.listener.event.config[i];
                var priority = config.priority;
                var clazz = config.clazz;
                var listener = uc.util.EventUtil.getObject(clazz, uc, false);
                if(listener){
                    listener = new listener();
                }else{
                    continue;
                }
                var interfaceTypes = config.type, interfaceType;
                for(var j = 0; j < interfaceTypes.length; j++){
                    interfaceType = interfaceTypes[j];
                    for(var event in uc[interfaceType]){
                        //console.log('manager interfaceType = ' + interfaceType + ', event = ' + event + ', listener = ', listener);
                        uc.util.EventUtil.connect(uc[interfaceType], event, listener, event, priority);
                    }
                }
            }
        },
        addListener: function(eventType, event, context, method){
            uc.util.EventUtil.connect(uc[eventType], event, context, method);
        },

        removeListener: function(handler){
            uc.util.EventUtil.disconnect(handler);
        },
        _bindNativeEvent: function(eventType, event){
            //TODO need native to change event register
            ucClient.addEventListener(uc.listener.nativeEventConfig[eventType][event].nativeEvent, function(data){
                //console.log('[ ' + eventType + ' ] [' + event + '] Data = ', data.detail);
                if(event != 'OnFileDownloading' && event != "OnCallSpeakingStatusChanged"){
                    uc.util.LogUtil.info(eventType, event, 'Data = ', data.detail);
                }

                if (uc.listener.nativeEventConfig[eventType][event].nativeEvent == ucClient.GroupService.GROUP_CREATED_EVENT){
                    if (data.detail.group.id === 0) return;
                    data.detail.group.groupType = uc.modules.conversation.ConversationManager.groupTypeToChatType(data.detail.group.groupType);
                }
                uc[eventType][event].apply(uc[eventType], _this.getEventParamData(data, uc.listener.nativeEventConfig[eventType][event].param));
            });
        },
        getEventParamData: function(data, paramArray){
            var result = [];
            for(var i = 0, len = paramArray.length; i < len; i++){
                result.push(data.detail[paramArray[i]]);
            }
            return result;
        },
        _bindNativeService: function(serviceType, method){
            uc[serviceType][method] = uc.util.EventUtil.hitch(uc.listener.nativeServiceConfig[serviceType][method].context, uc.listener.nativeServiceConfig[serviceType][method].method);
        }
    }
})(uc);