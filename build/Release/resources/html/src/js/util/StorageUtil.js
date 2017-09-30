/**
 * Created by waouyang on 15/8/14.
 */
(function(uc){
    uc.util.StorageUtil = {
        setData: function(key, value){
            localStorage.setItem(key, JSON.stringify(value));
        },
        getData: function(key){
	        if(localStorage.getItem(key) == undefined){
		        return null;
	        }
            return JSON.parse(localStorage.getItem(key));
        },
        removeData: function(key){
            localStorage.removeItem(key);
        },
        clear: function(){
            localStorage.clear();
        }
    }
})(uc);