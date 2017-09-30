/**
 * Created by waouyang on 15/9/1.
 */
(function(){
    var class2type = {};
    
    function getRootPath(){
        var href = location.href;
        var index = href.indexOf('/html');
        return href.substring(0, index + 6);
    }
    function getXHR() {
        try {
            return new window.XMLHttpRequest();
        } catch( e ) {}
    }

    function exeScript(script, overwrite){
        if(script){
            (window.execScript || function(script){
                window['eval'].call(window, script);
            })(script);

            if(overwrite){
                extendObject(true, uc_resource, uc_resource_extention);
            }
        }
    }
    
    function getScript(xhr, url, overwrite){
        xhr.open('GET', url, false);
        xhr.onreadystatechange = function(){
            if(xhr.readyState == 4){
                if(xhr.status == 200 || xhr.status == 0){
                    if(typeof xhr.responseText === "string"){
                        var script = xhr.responseText;
                        exeScript(script, overwrite);
                    }
                }
            }else{}
        };
        xhr.send();
    }
    function loadResource(){
        var rootPath = getRootPath();
        var configPath = rootPath + 'nls/resource.js';
        var xhr = getXHR();
        getScript(xhr,configPath, false);
        configPath = rootPath + 'nls/customize/resource_customize.js';
        getScript(xhr,configPath, true);
    }

    function _getProperty(parts, context, create){
        var obj = context || window;
        for(var i = 0, p; obj && (p = parts[i]); i++){
            obj = p in obj ? obj = obj[p] : (create ? obj[p] = {} : undefined);
        }
        return obj;
    }
    function getObject (name, context, create){
        return _getProperty(name.split('.'), context, create);
    }

    function replaceToken(token){
        token = token.substring(2, token.length - 2);
        return getObject(token, uc_resource, false);
    }

    function translate(){
        var bodyHtml = document.body.innerHTML;
        var reg = new RegExp("\{\{.*\}\}", "g");
        document.body.innerHTML = bodyHtml.replace(reg, function(token){
            return replaceToken(token)
        })
    }

    document.addEventListener('DOMContentLoaded', function () {
        document.removeEventListener('DOMContentLoaded', arguments.callee, false);
        translate();
    }, false);

    function initClass2TypeMap() {
        "Boolean Number String Function Array Date RegExp Object Error".split(" ").map(function (name) {
            class2type["[object " + name + "]"] = name.toLowerCase();
        })
    }

    initClass2TypeMap();

    function type(obj) {
        if ( obj == null ) {
            return obj + "";
        }
        // Support: Android<4.0, iOS<6 (functionish RegExp)
        return typeof obj === "object" || typeof obj === "function" ?  class2type[ class2type.toString.call(obj) ] || "object" : typeof obj;
    }

    function isFunction( obj ) {
        return type(obj) === "function";
    }

    function isPlainObject( obj ) {
        // Not plain objects:
        // - Any object or value whose internal [[Class]] property is not "[object Object]"
        // - DOM nodes
        // - window
        if (obj == null || type( obj ) !== "object" || obj.nodeType || obj === obj.window ) {
            return false;
        }

        if ( obj.constructor && !class2type.hasOwnProperty.call( obj.constructor.prototype, "isPrototypeOf" ) ) {
            return false;
        }

        // If the function hasn't returned already, we're confident that
        // |obj| is a plain object, created by {} or constructed with new Object
        return true;
    }

    function extendObject() {
        var options, name, src, copy, copyIsArray, clone,
            target = arguments[0] || {},
            i = 1,
            length = arguments.length,
            deep = false;

        // Handle a deep copy situation
        if ( typeof target === "boolean" ) {
            deep = target;

            // Skip the boolean and the target
            target = arguments[ i ] || {};
            i++;
        }

        // Handle case when target is a string or something (possible in deep copy)
        if ( typeof target !== "object" && !isFunction(target) ) {
            target = {};
        }

        // Extend jQuery itself if only one argument is passed
        if ( i === length ) {
            target = this;
            i--;
        }

        for ( ; i < length; i++ ) {
            // Only deal with non-null/undefined values
            if ( (options = arguments[ i ]) != null ) {
                // Extend the base object
                for ( name in options ) {
                    src = target[ name ];
                    copy = options[ name ];

                    // Prevent never-ending loop
                    if ( target === copy ) {
                        continue;
                    }

                    // Recurse if we're merging plain objects or arrays
                    if ( deep && copy && ( isPlainObject(copy) || (copyIsArray = Array.isArray(copy)) ) ) {
                        if ( copyIsArray ) {
                            copyIsArray = false;
                            clone = src && Array.isArray(src) ? src : [];

                        } else {
                            clone = src && isPlainObject(src) ? src : {};
                        }

                        // Never move original objects, clone them
                        target[ name ] = extendObject( deep, clone, copy );

                        // Don't bring in undefined values
                    } else if ( copy !== undefined ) {
                        target[ name ] = copy;
                    }
                }
            }
        }

        // Return the modified object
        return target;
    }

    loadResource();

})();
