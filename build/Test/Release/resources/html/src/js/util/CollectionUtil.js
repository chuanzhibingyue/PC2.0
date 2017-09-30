
(function (uc) {
    
    uc.util.CollectionUtil = {  
    	//Sort array by multiple properties:
    	//http://stackoverflow.com/questions/1129216/sort-array-of-objects-by-string-property-value-in-javascript
    	dynamicSortMultiple: function(){
    		var _self = this;
    		/*
    	     * save the arguments object as it will be overwritten
    	     * note that arguments object is an array-like object
    	     * consisting of the names of the properties to sort by
    	     */
    	    var props = arguments;
    	    return function (obj1, obj2) {
    	        var i = 0, result = 0, numberOfProperties = props.length;
    	        /* try getting a different result from 0 (equal)
    	         * as long as we have extra properties to compare
    	         */
    	        while(result === 0 && i < numberOfProperties) {
    	            result = _self.dynamicSort(props[i])(obj1, obj2);
    	            i++;
    	        }
    	        return result;
    	    }
        }, 
        dynamicSort: function(property) {
            var sortOrder = 1;
            if(property[0] === "-") {
                sortOrder = -1;
                property = property.substr(1);
            }
            return function (a,b) {
                var result = (a[property].toUpperCase() < b[property].toUpperCase()) ? -1 : (a[property].toUpperCase() > b[property].toUpperCase()) ? 1 : 0;
                return result * sortOrder;
            }
        },
        keySort: function (obj) {
        	return Object.keys(obj).sort().reduce(function (result, key) {
                result[key] = obj[key];
                return result;
            }, {});
        },
        findObjectByKey: function(array, key, value) {
            for (var i = 0; i < array.length; i++) {
                if (array[i][key] === value) {
                    return array[i];
                }
            }
            return null;
        },
        isArray: function(v){
        	return toString.apply(v) === '[object Array]';
        }
    }
})(uc);