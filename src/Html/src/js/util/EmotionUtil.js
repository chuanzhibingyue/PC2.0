/**
 * Created by zuo.dai on 2015/8/5.
 */

(function (uc) {
    var _this = null;
    uc.util.EmotionUtil = {
        emotionData : {
            ":-)" : ["wx.emj", uc_resource.Emotion.Smile, "wx"],
            ":)" : ["wx.emj", uc_resource.Emotion.Smile, "wx"],
            ":-(" : ["bkx.emj", uc_resource.Emotion.Unhappy, "bkx"],
            ":(" : ["bkx.emj", uc_resource.Emotion.Unhappy, "bkx"],
            ":-@" : ["fn.emj", uc_resource.Emotion.Anger, "fn"],
            ";-)" : ["fy.emj", uc_resource.Emotion.Blink, "fy"],
            ";)" : ["fy.emj", uc_resource.Emotion.Blink, "fy"],
            ":^)" : ["wbzd.emj", uc_resource.Emotion.DontKnow, "wbzd"],
            ":-|" : ["mdkd.emj", uc_resource.Emotion.Stunned, "mdkd"],
            ":-D" : ["dx.emj", uc_resource.Emotion.Laugh, "dx"],
            "(H)" : ["hehe.emj", uc_resource.Emotion.Hoho, "hehe"],
            ":o)" : ["hl.emj", uc_resource.Emotion.Happy, "hl"],
            ":-*" : ["mimi.emj", uc_resource.Emotion.Secret, "mimi"],
            ":'(" : ["sx.emj", uc_resource.Emotion.Sad, "sx"],
            "(L)" : ["mxhx.emj", uc_resource.Emotion.FullJoy, "mxhx"],
            "+o(" : ["byy.emj", uc_resource.Emotion.Sickly, "byy"],
            "(Z)" : ["bns.emj", uc_resource.Emotion.CanNotSay, "bns"],
            ":-#" : ["bz.emj", uc_resource.Emotion.ShutUp, "bz"],
            ":-P" : ["tst.emj", uc_resource.Emotion.StickingTongue, "tst"],
            ":P" : ["tst.emj", uc_resource.Emotion.StickingTongue, "tst"],
            "*-)" : ["cs.emj", uc_resource.Emotion.Ponder, "cs"],
            "|-)" : ["xs.emj", uc_resource.Emotion.Sleepy, "xs"],
            ":-S" : ["yh.emj", uc_resource.Emotion.Doubt, "yh"],
            ":S" : ["yh.emj", uc_resource.Emotion.Doubt, "yh"]
        },
        escape_rx: function (text) {
            return text.replace(/[-[\]{}()*+?.,\\^$|#\s]/g, "\\$&");
        },
        _codeEmotionMap: {},
        _codeColonMap: {},
        _colonEmotionMap: {},
        _emotionCodeMap: {},
        _codeEmotionTagMap: {},
        init: function(){
            if(this.initialized){
                return;
            }
            _this = this;
            this.initialized = true;
            this.buildMap();
        },
        buildMap: function(){
            var codes = [], emotions = [], colons = [];
            for(var i in this.emotionData){
                codes.push(this.escape_rx(i));
                emotions.push(this.escape_rx(this.emotionData[i][0]));
                if(colons.indexOf(this.escape_rx(this.emotionData[i][2])) == -1){
                    colons.push(":" + this.escape_rx(this.emotionData[i][2]) + ":");
                }
                this._codeEmotionMap[i] = this.emotionData[i][0];
                this._codeEmotionTagMap[i] = this.emotionData[i][1];
                this._codeColonMap[i] = this.emotionData[i][2];
                this._emotionCodeMap[this.emotionData[i][0]] = i;
                this._colonEmotionMap[':' + this.emotionData[i][2] + ':'] = this.emotionData[i][0];
            }
            this._codeRegExp = new RegExp('(' + codes.join('|') + ')', 'g');
            this._emotionRegExp = new RegExp('(' + emotions.join('|') + ')', 'g');
            this._colonRegExp = new RegExp('(' + colons.join('|') + ')', 'g');
        },


        //替换文字消息中的表情符号为显示html标签
        codeToEmotion: function (input) {
            if (!input){
                return "";
            }
            if(!this.initialized){
                this.init();
            }
            return input.replace(this._codeRegExp, function(m){
                if(m){
                    return '<img class="emotion" src="../images/emotions/' + _this._codeEmotionMap[m] + '" alt=":' + _this._codeColonMap[m]  +':">';
                }else{
                    return "";
                }
            });
        },
        altToEmotionUnicode: function (emoji) {
            if (!emoji){
                return "";
            }
            if(!this.initialized){
                this.init();
            }
            var EmojiConfig = uc.modules.emoji.EmojiConfig;
            if (!EmojiConfig.rx_code_colons){
                EmojiConfig.init_unified();
            }
            var codeColonEmojiCodeMap = EmojiConfig.codeColonEmojiCodeMap;
            var smileyMap = EmojiConfig.smileyMap;    
            var _colonEmotionMap = this._colonEmotionMap;
            var _emotionCodeMap =  this._emotionCodeMap;
            
            if (codeColonEmojiCodeMap[emoji]) {
                return smileyMap[codeColonEmojiCodeMap[emoji] + '.emj'] || emoji;
            } else if (_colonEmotionMap[emoji]) {
                return _emotionCodeMap[_colonEmotionMap[emoji]] || emoji;
            }
            return emoji;
        },
        //替换html当中的表情为code
        htmlToEmotionUnicode: function (input) {
            if (!input){
                return "";
            }
            if(!this.initialized){
                this.init();
            }
            var EmojiConfig = uc.modules.emoji.EmojiConfig;
            if (!EmojiConfig.rx_code_colons){
                EmojiConfig.init_unified();
            }
            var codeColonEmojiCodeMap = EmojiConfig.codeColonEmojiCodeMap;
            var smileyMap = EmojiConfig.smileyMap;    
            var _colonEmotionMap = this._colonEmotionMap;
            var _emotionCodeMap =  this._emotionCodeMap;
            
            return input.replace(/<img\s.*?\salt="(.*?)".*?>/g, function (html, emoji) {
                if (codeColonEmojiCodeMap[emoji]) {
                    return smileyMap[codeColonEmojiCodeMap[emoji] + '.emj'] || html;
                } else if (_colonEmotionMap[emoji]) {
                    return _emotionCodeMap[_colonEmotionMap[emoji]] || html;
                }
                return html;
            })
        },
        altToEmotion: function (emoji) {
            if (!emoji){
                return "";
            }
            if(!this.initialized){
                this.init();
            }
            var EmojiConfig = uc.modules.emoji.EmojiConfig;
            if (!EmojiConfig.rx_code_colons){
                EmojiConfig.init_unified();
            }
            var codeColonEmojiCodeMap = EmojiConfig.codeColonEmojiCodeMap;   
            var _colonEmotionMap = this._colonEmotionMap;
            if (codeColonEmojiCodeMap[emoji]) {
                return emoji;
            } else if (_colonEmotionMap[emoji]) {
                return emoji;
            }
            return emoji;
        },
        htmlToEmotion: function (input) {
            if (!input){
                return "";
            }
            if(!this.initialized){
                this.init();
            }
            var EmojiConfig = uc.modules.emoji.EmojiConfig;
            if (!EmojiConfig.rx_code_colons){
                EmojiConfig.init_unified();
            }
            var codeColonEmojiCodeMap = EmojiConfig.codeColonEmojiCodeMap;   
            var _colonEmotionMap = this._colonEmotionMap;
            
            return input.replace(/<img\s.*?\salt="(.*?)".*?>/g, function (html, emoji) {
                if (codeColonEmojiCodeMap[emoji]) {
                    return emoji;
                } else if (_colonEmotionMap[emoji]) {
                    return emoji;
                }
                return html;
            })
        },
        colonToEmotion: function(input){
            if (!input){
                return "";
            }
            if(!this.initialized){
                this.init();
            }
            return input.replace(this._colonRegExp, function(m){
                if(m){
                    return '<img class="emotion" src="../images/emotions/' + _this._colonEmotionMap[m] + '" alt="' + m +'">';
                }else{
                    return "";
                }
            });
        },

        //根据表情图片名称获取表情符号文字
        emotionToCode: function (input) {
            if (!input){
                return "";
            }
            if(!this.initialized){
                this.init();
            }

            return input.replace(this._emotionRegExp, function(m){
                if(m){
                    return _this._emotionCodeMap[m];
                }else{
                    return "";
                }
            });
        },
        codeToDescription: function (input) {
            if (!input){
                return "";
            }
           if(!this.initialized){
               this.init();
           }

            return input.replace(this._codeRegExp, function(m){
                if(m){
                    return '[' + _this._codeEmotionTagMap[m] +']';
                }else{
                    return '';
                }
            });
        }
    }
})(uc);