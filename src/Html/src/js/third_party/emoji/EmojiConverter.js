/**
 * Created by senlin.ouyang on 2015/4/20.
 */
(function (uc) {
    uc.modules.emoji.EmojiConverter = {
        init: function () {
            uc.modules.emoji.EmojiConfig.init_unified();
        },
        smileyToUnicode: function(input){
            if (!input){
                return "";
            }

            if (!uc.modules.emoji.EmojiConfig.rx_smileys){
                uc.modules.emoji.EmojiConfig.init_unified();
            }
            return input.replace(uc.modules.emoji.EmojiConfig.rx_smileys, function(m){
                var val = uc.modules.emoji.EmojiConfig.smileyMap[m];
                if(val){
                    return val;
                }else{
                    return "";
                }
            });
        },
        unicodeToEmotionTag: function(input){
            if (!input){
                return "";
            }

            if (!uc.modules.emoji.EmojiConfig.rx_codes){
                uc.modules.emoji.EmojiConfig.init_unified();
            }
            return input.replace(uc.modules.emoji.EmojiConfig.rx_codes, function(m){
                return uc_resource.Emotion.EmotionTag;
            });
        },
        unicodeToSmiley: function (input) {
            if (!input){
                return "";
            }

            if (!uc.modules.emoji.EmojiConfig.rx_codes){
                uc.modules.emoji.EmojiConfig.init_unified();
            }
            return input.replace(uc.modules.emoji.EmojiConfig.rx_codes, function (m) {
                var emojiCode = uc.modules.emoji.EmojiConfig.unicodeEmojiCodeMap[m];
                if(emojiCode){
                    return uc.modules.emoji.EmojiConverter.createEmojiIcon(emojiCode);
                }else{
                    return "";
                }
            });
        },
        codeColonToSmiley: function (input) {
            if (!input){
                return "";
            }

            if (!uc.modules.emoji.EmojiConfig.rx_code_colons){
                uc.modules.emoji.EmojiConfig.init_unified();
            }

            return input.replace(uc.modules.emoji.EmojiConfig.rx_code_colons, function (m) {
                var emojiCode = uc.modules.emoji.EmojiConfig.codeColonEmojiCodeMap[m];
                if (emojiCode) {
                    return uc.modules.emoji.EmojiConverter.createEmojiIcon(emojiCode);
                } else {
                    return "";
                }
            });
        },
        androidCodeToSmiley: function(input){
            if (!input){
                return "";
            }

            if (!uc.modules.emoji.EmojiConfig.rx_android_colons){
                uc.modules.emoji.EmojiConfig.init_unified();
            }

            return input.replace(uc.modules.emoji.EmojiConfig.rx_android_colons, function (m) {
                var emojiCode = uc.modules.emoji.EmojiConfig.androidColonEmojiCodeMap[m];
                if (emojiCode) {
                    return uc.modules.emoji.EmojiConverter.createEmojiIcon(emojiCode);
                } else {
                    return "";
                }
            });
        },
        androidCodeToEmotionTag: function(input){
            if (!input){
                return "";
            }

            if (!uc.modules.emoji.EmojiConfig.rx_android_colons){
                uc.modules.emoji.EmojiConfig.init_unified();
            }

            return input.replace(uc.modules.emoji.EmojiConfig.rx_android_colons, function(m){
                return uc_resource.Emotion.EmotionTag;
            });
        },
        createEmojiIcon: function(emojiCode){
            return '<img width="' + 20 + '" height="' + 20 + '" src="' + "../images/emotions/" + emojiCode + ".emj" + '" alt=":' + emojiCode + ':">';
        }
    }
})(uc);