/**
 * Created by waouyang on 15/8/27.
 */
(function(uc){
    uc.util.AudioUtil = {
        playSound: function(filePath, id, callback){
            ucClient.System.playSound(filePath, id, false,callback);
        },
        stopPlay: function(id){
            ucClient.System.stopSound(id);
        },
        playMessageNotificationSound: function(id){
            if(!uc.settings.msgPrompt.playPrompt){
                return;
            }
            var fullPath = location.href;
            var htmlIndex = fullPath.indexOf('html');
            var soundPath = fullPath.substring(8, htmlIndex + 5) + "media\/" + uc.settings.msgPrompt.name;
            ucClient.System.playSound(soundPath, id, false,function(){});
        },
	    playCallSound: function (type,loop){
		    this.stopCallAllSound();
			var _loop = loop != undefined ? loop: false;
			var fullPath = location.href;
		    var htmlIndex = fullPath.indexOf('html');
		    var soundPath = fullPath.substring(8, htmlIndex + 5) + "media\/callmedia\/" + type+".amr";
            soundPath = decodeURI(soundPath);
		    ucClient.System.playSound(soundPath, type,_loop,function(){});
		   // uc.util.LogUtil.info("uc.util.AudioUtil", "playCallSound", "type", type);
	    },
	    stopCallSound: function(type){
		    ucClient.System.stopSound(type);
	    },
	    stopCallAllSound: function(){
		    var _type;
		    var _obj = uc.constants.Call.callMediaType;
		    for(_type in _obj){
			    ucClient.System.stopSound(_obj[_type]);
			   // uc.util.LogUtil.info("uc.util.AudioUtil", "stopCallAllSound", "type", _obj[_type]);
		    }
	    }
    }
})(uc);