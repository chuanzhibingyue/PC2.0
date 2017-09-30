/**
 * Created by senlin.ouyang on 2015/7/15.
 */
(function(uc){
    uc.util.HashMap = function (){
        this.key = [];
        this.value = {};
    };

    jQuery.extend(uc.util.HashMap.prototype,{
        _indexOf : function (key){
            //to find the index is slow, avoid to do that if key is not exist.
            if (this.isExist(key)){
                for (var i in this.key) {
                    if (this.key[i] == key){
                        return i;
                    }
                }
            }
            return -1;
        },
        put : function (keyWord, value, /*int ?*/idx){
            if (keyWord == null){
                return;
            }				//can't hava null key

            var keys = this.key;
            var values = this.value;
            var objIdx = this._indexOf(keyWord);
            if (objIdx < 0){                            //can't hava duplicate keys
                if (idx != undefined){
                    keys.splice(idx, 0, keyWord);   //insert the new element into a specified position
                } else {
                    keys.push(keyWord);
                }
            }
            values[keyWord] = value;				//if it's a new item just add it, otherwise update the old value
        },
        get : function (keyWord){ //support get value by it's index and keyword
            if (typeof(keyWord) == "number"){
                return this.value[this.key[keyWord]];
            } else {
                return this.value[keyWord];
            }
        },
        indexOf : function (keyWord){
            return this._indexOf(keyWord);
        },
        isExist : function (keyWord){
            return this.value[keyWord] != undefined;
        },
        remove : function (keyWord){//support remove value by it's index and keyword
            var keys = this.key;
            var values = this.value;
            var obj = null;
            if (typeof(keyWord) == "number"){
                obj = values[keys[keyWord]];
                delete values[keys[keyWord]];
                keys.splice(keyWord, 1);
            } else {
                var objIdx = this._indexOf(keyWord);
                obj = values[keyWord];
                delete values[keyWord];
                if (objIdx >= 0){
                    keys.splice(objIdx, 1);
                }
            }
            return obj;
        },
        resetKey : function (oldKey, newKey){
            var idx = this._indexOf(oldKey);
            if (idx < 0){
                return;
            }
            this.key[idx] = newKey;
            this.value[newKey] = this.value[oldKey];
            delete this.value[oldKey]
        },
        clear : function (){
            this.key = [];
            this.value = {};
        },
        size : function (){
            return this.key.length;
        },
        isEmpty : function (){
            return this.key.length == 0;
        },
        convertToArray : function (){
            var valueArr = [];
            for (var i = 0; i < this.size(); i++) {
                valueArr.push(this.get(i));
            }
            return valueArr;
        }
    });
})(uc);