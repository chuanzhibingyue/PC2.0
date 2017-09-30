/**
 * Created by waouyang on 15/8/5.
 */
(function (uc) {
    String.prototype.format = function (args) {
        var result = this;
        if (arguments.length > 0) {
            if (arguments.length == 1 && typeof (args) == "object") {
                for (var key in args) {
                    if (args[key] != undefined) {
                        var reg = new RegExp("({" + key + "})", "g");
                        result = result.replace(reg, args[key]);
                    }
                }
            }
            else {
                for (var i = 0; i < arguments.length; i++) {
                    if (arguments[i] != undefined) {
                        var reg = new RegExp("({)" + i + "(})", "g");
                        result = result.replace(reg, arguments[i]);
                    }
                }
            }
        }
        return result;
    };

    uc.util.StringUtil = {
        /**
         * replace params of a string, for example convert '%1 has left the session at %2'
         * to 'XXX has left the session at 2009/12/28 09:18:20', srcString is '%1 has left the session at %2'
         * and params = ['XXX','2009/12/28 09:18:20']
         * @param  srcString source String
         * @param  params params used to replace
         */
        formatString: function (/*String*/srcString, /*Array*/params) {
            var destString = srcString;
            if (params) {
                var reg = new RegExp("{([1-" + params.length + "])}", "ig");
                destString = srcString.replace(reg, function (match, index) {
                    return params[index - 1];
                });
            }
            return destString;
        },

        /**
         * set email link
         * @param {Object} mailAddress
         */
        addMailTo: function (mailAddress) {
            return '<a href="mailto:' + mailAddress + '">' + this.setFontStyle(mailAddress, 'underline').fontcolor("#0c65a5") + '</a>';
        },

        addLink: function (url) {
            return '<a target="_blank" href="' + url + '">' + url + '</a>';
        },
        /**
         * match email & url from message
         * @param {Object} message
         */
        matchLink: function (message) {
            var alreadyDecodeUrl = [];

            message = message.replace(/\$/g, "uc_bee_speical_ch_");
            message = message.replace(/<br>/g, ' <br> ');

            //email
            var emailExp = /\w+([-+.]\w+)*@\w+([-.]\w+)*\.\w+([-.]\w+)*/g;
            message = message.replace(emailExp, function (mailStr) {
                return '<a href="mailto:' + mailStr + '">' + mailStr + '</a>';
            });

            var urlExp = /((((http|https|ftp|ftps|sftp):[\\\/]{2})([^\s\\\/]+(:[^\s\\\/]+)?@)?)(([\w\d\-]+\.){1,3}[\w\d\-]+)([:\d]+)?[\\\/]?[^\s]+([\s]|$))|(?!((http|https|ftp|ftps|sftp):[\\\/]{2}))(www\.)(([\w\d\-]+\.){1,3}[\w\d\-]+)([:\d]+)?[\\\/]?[^\s]+([\s]|$)/gmi;
            message = message.replace(urlExp, function (urlString) {
                return '<a target="_blank" href="' + urlString.trim() + '">' + urlString.trim() + '</a>';
            });

            message = message.replace(/uc_bee_speical_ch_/g, "$");
            return message;
        },

        setFontStyle: function (url, fontStyle) {
            var result = '';
            if (fontStyle == 'underline') {
                result = '<font style="text-decoration:' + fontStyle + ';">' + url + '</font>';
            } else {
                result = '<font style="font-style:' + fontStyle + ';">' + url + '</font>';
            }
            return result;
        },

        formatFileSize: function (size) {
            if (typeof size == 'number') {
                if (size >= 0 && size < 1024) {
                    return size + 'B';
                } else if (size >= 1024 && size < 1048576) {
                    return (size / 1024).toFixed(2) + 'KB';
                }
                else if (size >= 1048576 && size < 1073741824) {
                    return (size / 1048576).toFixed(2) + 'MB';
                }
                else if (size >= 1073741824 && size < 1099511627776) {
                    return (size / 1073741824).toFixed(2) + 'GB';
                }
            } else if (typeof size == 'string') {
                if (/\d+B/i.test(size)) {
                    return parseInt(size);
                } else if (/\d+KB/i.test(size)) {
                    return parseInt(size) * 1024;
                } else if (/\d+MB/i.test(size)) {
                    return parseInt(size) * 1048576;
                } else if (/\d+GB/i.test(size)) {
                    return parseInt(size) * 1073741824;
                }
            }
        },

        fileExtMap: {
            "jpg": ["pcx", "emf", "gif", "bmp", "tga", "jpg", "tif", "jpeg", "png", "rle"],
            "doc": ["doc", "docx"],
            "xls": ["xls", "xlsx"],
            "pdf": ["pdf"],
            "file": ["file"],
            "ppt": ["ppt", "pptx"],
            "txt": ["txt", "ini", "log", "chm", "xml", "html", "htm", "js", "cpp", "h", "c", "java", "php", "go", "py", "bat", "sh", "css", "exe", "msi", "dmp"],
            "rar": ["rar", "gz", "zip", "gzip", "tar", "cab", "jar", "7z", "iso", "ace", "arj", "bz2", "uue", "z", "apk"],
            "flv": ["aiff", "avi", "mov", "mpeg", "mpg", "qt", "ram", "viv", "ra", "rm", "rmvb", "dat", "asf", "wmv", "swf", "vob"]
        },
        fileSpecialExtMap: {
            "doc": "word",
            "docx": "word",
            "xls": "excel",
            "xlsx": "excel"
        },
        getFileDisplayExt: function (fileName) {
            var index = fileName.lastIndexOf(".");
            if (index < 0) {
                return "";
            }
            var ext = fileName.substring(index + 1, fileName.length);
            ext = ext.toLocaleLowerCase();
            if (uc.util.StringUtil.fileSpecialExtMap[ext] != null) {
                return uc.util.StringUtil.fileSpecialExtMap[ext].toLocaleUpperCase();
            }
            return ext.toLocaleUpperCase();
        },
        getFileIcon: function (fileName, bigIcon) {
            var str = "../images/fileicons/";
            if (!bigIcon) {
                str = "../images/sfileicons/";
            }
            var index = fileName.lastIndexOf(".");
            if (index < 0) {
                return str + "unknow.png";
            }
            var ext = fileName.substring(index + 1, fileName.length);
            ext = ext.toLocaleLowerCase();
            for (var key in uc.util.StringUtil.fileExtMap) {
                var exts = uc.util.StringUtil.fileExtMap[key];
                for (var i = 0; i < exts.length; i++) {
                    if (exts[i] == ext) {
                        return str + key + ".png";
                    }
                }
            }
            return str + "unknow.png";
        },
        htmlEncode: function (value) {
            return $('<div/>').text(value).html();
        },
        htmlDecode: function (value) {
            return $('<div/>').html(value).text();
        },
        spaceReg: new RegExp("[ " + String.fromCharCode(160) + "]", "g"),
        escapeHtml: function (input, isSpace) {
            if (!input) {
                return '';
            }
            input = '' + input;
            if (isSpace) {
                return input.replace(/&/gm, "&amp;").replace(/</gm, "&lt;").replace(/>/gm, "&gt;").replace(this.spaceReg, "&nbsp;");
            } else {
                return input.replace(/&/gm, "&amp;").replace(/</gm, "&lt;").replace(/>/gm, "&gt;");
            }
        },
        escapeApiHtml: function (input) {
            if (!input) {
                return '';
            }

            return input.replace(/</gm, "&lt;").replace(/>/gm, "&gt;");
        },
        getParameterByName: function (splitStr, strUrl) {
            splitStr = splitStr.replace(/[\[]/, "\\\[").replace(/[\]]/, "\\\]");
            var regexS = "[\\?&]" + splitStr + "=([^&#]*)";
            var regex = new RegExp(regexS);
            var results = regex.exec(strUrl);
            if (results == null) {
                return "";
            } else {
                return decodeURIComponent(results[1].replace(/\+/g, " "));
            }
        },
        IsTelephone: function (telnumber) {
            var isMobile = /^(?:13\d|15\d|18\d|17\d)\d{5}(\d{3}|\*{3})$/;
            var isPhone = /^((0\d{2,3}))?(\d{7,8})?((\d{0,}))$/;//不需要分机号(-(\d{3,}))
            return isMobile.test(telnumber) || isPhone.test(telnumber);
        },
        trim: function (str, is_global) {
            var result;
            result = str.replace(/(^\s+)|(\s+$)/g, "");
            if (is_global.toLowerCase() == "g") {
                result = result.replace(/\s/g, "");
            }
            return result;
        },
        IsPBXTelNumber: function (/*String*/telNumber) {
            if (!telNumber) {
                return null;
            }
            var tempsz = telNumber.split('/');
            var returnSz = [];
            for (var i = 0; i < tempsz.length; i++) {
                if (this.IsTelephone(tempsz[i])) {
                    returnSz.push(tempsz[i]);
                }
            }
            return returnSz;
        },
        //验证邮箱
        CheckEmail: function (value,FailedMsgBox) {
            if (!/^\w+([-+.]\w+)*@\w+([-.]\w+)*\.\w+([-.]\w+)*$/.test(value)) {
                if (FailedMsgBox == null) {
                    uc.modules.component.PromptMessageBox.open({
                            title: uc_resource.Common.Notification,
                            message: uc_resource.Profile.EmailCheck,
                            buttonMode: "ok",
                            OkText: uc_resource.Common.Known
                        },
                        function (result) {
                            this.close();
                            $("input").focus();
                        });
                }
                return false;
            }
            return true;
        },
        //验证手机号码
        MobileCheck: function (value) {
            var regu = /^[1][0-9][0-9]{9}$/;
            var re = new RegExp(regu);
            if (re.test(value)) {
                return true;
            } else {
                uc.modules.component.PromptMessageBox.open({
                        title: uc_resource.Common.Notification,
                        message: uc_resource.Profile.PhoneCheck,
                        buttonMode: "ok",
                        OkText: uc_resource.Common.Known
                    },
                    function (result) {
                        this.close();
                        $("input").focus();
                    });

                return false;
            }
        },
        isMobileNumber: function (tel) {
            var re = /^1\d{10}$/;
            if (re.test(tel)) {
                return true;
            }
            return false;
        },
        replaceAtPosition: function (srcStr, startPos, endPos, replaceStr) {
            return srcStr.substring(0, startPos) + replaceStr + srcStr.substring(endPos);
        },
        textSearchHighLight:function(text,keyWord,cls){
            var content = text;
            var clStr =  cls ? "class='"+cls+"'" : "style='color:#f00'";
            var array = keyWord.split(",");
            for(var i=0;i<array.length;i++){
                if(content.indexOf(array[i])>-1){
                    var key = new RegExp(array[i],"gi");
                    content = content.replace(key,("<span "+clStr+">" + array[i] + "</span>"));
                }
            }
            return content;
        }
    }
    ;
    (function () {
        var templateSettings = {
            evaluate: /<@([\s\S]+?)@>/g,
            interpolate: /<@=([\s\S]+?)@>/g,
            escape: /<@-([\s\S]+?)@>/g
        };
        var noMatch = /(.)^/;
        var escapes = {
            "'": "'",
            '\\': '\\',
            '\r': 'r',
            '\n': 'n',
            '\u2028': 'u2028',
            '\u2029': 'u2029'
        };
        var escapeRegExp = /\\|'|\r|\n|\u2028|\u2029/g;
        var escapeChar = function (match) {
            return '\\' + escapes[match];
        };

        var escapeMap = {
            '&': '&amp;',
            '<': '&lt;',
            '>': '&gt;',
            '"': '&quot;',
            "'": '&#x27;',
            '`': '&#x60;'
        };

        var createEscaper = function (map) {
            var escaper = function (match) {
                return map[match];
            };
            var source = '(?:' + Object.keys(map).join('|') + ')';
            var testRegexp = RegExp(source);
            var replaceRegexp = RegExp(source, 'g');
            return function (string) {
                string = string == null ? '' : '' + string;
                return testRegexp.test(string) ? string.replace(replaceRegexp, escaper) : string;
            };
        };
        var _ = {
            escape: createEscaper(escapeMap)
        };
        uc.util.template = function (text, settings, oldSettings) {
            if (!settings && oldSettings) settings = oldSettings;
            settings = $.extend({}, templateSettings, settings);

            var matcher = RegExp([
                    (settings.escape || noMatch).source,
                    (settings.interpolate || noMatch).source,
                    (settings.evaluate || noMatch).source
                ].join('|') + '|$', 'g');

            var index = 0;
            var source = "__p+='";
            text.replace(matcher, function (match, escape, interpolate, evaluate, offset) {
                source += text.slice(index, offset).replace(escapeRegExp, escapeChar);
                index = offset + match.length;

                if (escape) {
                    source += "'+\n((__t=(" + escape + "))==null?'':_.escape(__t))+\n'";
                } else if (interpolate) {
                    source += "'+\n((__t=(" + interpolate + "))==null?'':__t)+\n'";
                } else if (evaluate) {
                    source += "';\n" + evaluate + "\n__p+='";
                }

                return match;
            });
            source += "';\n";

            if (!settings.variable) source = 'with(obj||{}){\n' + source + '}\n';

            source = "var __t,__p='',__j=Array.prototype.join," +
                "print=function(){__p+=__j.call(arguments,'');};\n" +
                source + 'return __p;\n';

            var render;
            try {
                render = new Function(settings.variable || 'obj', '_', source);
            } catch (e) {
                e.source = source;
                throw e;
            }

            var template = function (data) {
                return render.call(this, data, _);
            };

            var argument = settings.variable || 'obj';
            template.source = 'function(' + argument + '){\n' + source + '}';

            return template;
        };
    })()
})(uc);