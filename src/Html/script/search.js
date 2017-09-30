/**
 * Created by senlin.ouyang on 2015/7/15.
 */
(function(){
    var rlocalProtocol = /^(?:about|app|app-storage|.+-extension|file|res|widget):$/,
        rurl = /^([\w.+-]+:)(?:\/\/(?:[^\/?#]*@|)([^\/?#:]*)(?::(\d+)|)|)/,
        ajaxLocParts,
        ajaxLocation,
        isLocal;

    try {
        ajaxLocation = location.href;
    } catch( e ) {
        // Use the href attribute of an A element
        // since IE will modify it given document.location
        ajaxLocation = document.createElement( "a" );
        ajaxLocation.href = "";
        ajaxLocation = ajaxLocation.href;
    }

    ajaxLocParts = rurl.exec( ajaxLocation.toLowerCase() ) || [];
    isLocal = rlocalProtocol.test( ajaxLocParts[ 1 ] );

    var class2type = {};
    var toString = class2type.toString;
    var typeArray = "Boolean Number String Function Array Date RegExp Object Error".split(" ");
    for(var i = 0; i < typeArray.length; i++){
        class2type[ "[object " + typeArray[i] + "]" ] = typeArray[i].toLowerCase();
    }

    function type( obj ) {
        if ( obj == null ) {
            return obj + "";
        }
        return typeof obj === "object" || typeof obj === "function" ?
        class2type[ toString.call(obj) ] || "object" :
            typeof obj;
    }

    var isArray = Array.isArray || function(obj){
            return type(obj) === 'array';
        };

    function loadConfig(rootPath, htmlName){
        var configPath = rootPath + 'script/config/' + htmlName +  '_config.js';
        var xhr = getXHR();
        var loadConfigCallback = function(config){
            if(!config){
                return;
            }
            config = window['eval'].call(window, config);
            if(isArray(config)){
                loadScripts(rootPath, config);
            }
        };
        getScript(xhr,configPath, loadConfigCallback);
    }

    function loadScripts(rootPath,pathArray){
        var index = 0,
            path = pathArray[index];
        var callback = function(){
            path = pathArray[++index];
            loadSingleScript(rootPath, path, callback);
        };
        loadSingleScript(rootPath, path, callback);
    }

    function loadSingleScript(rootPath, path, callback){
        if(!path){
            return;
        }
        var script = document.createElement('script');
        script.src = rootPath + path;
        script.async = false;
        script.onload = script.onreadystatechange = function(){
            if ( !script.readyState || /loaded|complete/.test( script.readyState ) ) {
                // Handle memory leak in IE
                script.onload = script.onreadystatechange = null;
                callback && callback();
            }
        };
        document.getElementsByTagName('head')[0].appendChild(script);
    }

    function getScript(xhr, url, callback){
        xhr.open('GET', url, true);
        xhr.onreadystatechange = function(){
            if(xhr.readyState == 4){
                if(xhr.status == 200 || xhr.status == 0){
                    if(typeof xhr.responseText === "string"){
                        var script = xhr.responseText;
                        exeScript(script);
                        callback && callback(script);
                    }
                }
            }else{
                //callback && callback(null);
            }
        };
        xhr.send();
    }

    function exeScript(script){
        if(script){
            (window.execScript || function(script){
                window['eval'].call(window, script);
            })(script);
        }
    }

    function getRootPath(){
        var href = location.href;
        var index = href.indexOf('/html');
        return href.substring(0, index + 6);
    }

    function getXHR(){
        if(window.ActiveXObject != undefined){
            if(isLocal){
                return createActiveXHR();
            }else{
                return createStandardXHR();
            }
        }else{
            return createStandardXHR();
        }
    }
    function createStandardXHR() {
        try {
            return new window.XMLHttpRequest();
        } catch( e ) {}
    }


    function createActiveXHR() {
        try {
            return new window.ActiveXObject( "Microsoft.XMLHTTP" );
        } catch( e ) {}
    }


    function loadScript(htmlName){
        var rootPath = getRootPath();
        loadConfig(rootPath, htmlName);
    }

    loadScript('search');
})();