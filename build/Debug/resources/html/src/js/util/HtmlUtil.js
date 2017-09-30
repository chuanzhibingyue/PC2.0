/**
 * Created by lei.zhou on 2015/8/6.
 */
/*****************************************************************************
html tags and attributes
format html tags objects
{
    font: ['id', 'class', 'color', 'size', 'face', '.background-color'],
    span: [
        'id', 'class', '.color', '.background-color', '.font-size', '.font-family', '.background',
        '.font-weight', '.font-style', '.text-decoration', '.vertical-align', '.line-height'
    ],
    div: [
        'id', 'class', 'align', '.border', '.margin', '.padding', '.text-align', '.color',
        '.background-color', '.font-size', '.font-family', '.font-weight', '.background',
        '.font-style', '.text-decoration', '.vertical-align', '.margin-left'
    ],
    table: [
        'id', 'class', 'border', 'cellspacing', 'cellpadding', 'width', 'height', 'align', 'bordercolor',
        '.padding', '.margin', '.border', 'bgcolor', '.text-align', '.color', '.background-color',
        '.font-size', '.font-family', '.font-weight', '.font-style', '.text-decoration', '.background',
        '.width', '.height', '.border-collapse'
    ],
    'td,th': [
        'id', 'class', 'align', 'valign', 'width', 'height', 'colspan', 'rowspan', 'bgcolor',
        '.text-align', '.color', '.background-color', '.font-size', '.font-family', '.font-weight',
        '.font-style', '.text-decoration', '.vertical-align', '.background', '.border'
    ],
    a: ['id', 'class', 'href', 'target', 'name'],
    embed: ['id', 'class', 'src', 'width', 'height', 'type', 'loop', 'autostart', 'quality', '.width', '.height', 'align', 'allowscriptaccess'],
    img: ['id', 'class', 'src', 'width', 'height', 'border', 'alt', 'title', 'align', '.width', '.height', '.border'],
    'p,ol,ul,li,blockquote,h1,h2,h3,h4,h5,h6': [
        'id', 'class', 'align', '.text-align', '.color', '.background-color', '.font-size', '.font-family', '.background',
        '.font-weight', '.font-style', '.text-decoration', '.vertical-align', '.text-indent', '.margin-left'
    ],
    pre: ['id', 'class'],
    hr: ['id', 'class', '.page-break-after'],
    'br,tbody,tr,strong,b,sub,sup,em,i,u,strike,s,del': ['id', 'class'],
    iframe: ['id', 'class', 'src', 'frameborder', 'width', 'height', '.width', '.height']
}
*****************************************************************************/
(function (uc) {

    var FILL_ATTR_MAP = toMap('checked,compact,declare,defer,disabled,ismap,multiple,nohref,noresize,noshade,nowrap,readonly,selected'),
    SINGLE_TAG_MAP = toMap('area,base,basefont,br,col,frame,hr,img,input,isindex,link,meta,param,embed'),
    INLINE_TAG_MAP = toMap('a,abbr,acronym,b,basefont,bdo,big,br,button,cite,code,del,dfn,em,font,i,img,input,ins,kbd,label,map,q,s,samp,select,small,span,strike,strong,sub,sup,textarea,tt,u,var'),
    PRE_TAG_MAP = toMap('pre,style,script'),
    BLOCK_TAG_MAP = toMap('address,applet,blockquote,body,center,dd,dir,div,dl,dt,fieldset,form,frameset,h1,h2,h3,h4,h5,h6,head,hr,html,iframe,ins,isindex,li,map,menu,meta,noframes,noscript,object,ol,p,pre,script,style,table,tbody,td,tfoot,th,thead,title,tr,ul');

    var re = /(\s*)<(\/)?([\w\-:]+)((?:\s+|(?:\s+[\w\-:]+)|(?:\s+[\w\-:]+=[^\s"'<>]+)|(?:\s+[\w\-:"]+="[^"]*")|(?:\s+[\w\-:"]+='[^']*'))*)(\/)?>(\s*)/g;

    function foreach(obj, fn) {
        if (isArray(obj)) {
            for (var i = 0, len = obj.length; i < len; i++) {
                if (fn.call(obj[i], i, obj[i]) === false) {
                    break;
                }
            }
        } else {
            for (var key in obj) {
                if (obj.hasOwnProperty(key)) {
                    if (fn.call(obj[key], key, obj[key]) === false) {
                        break;
                    }
                }
            }
        }
    }

    function isArray(val) {
        if (!val) {
            return false;
        }
        return Object.prototype.toString.call(val) === '[object Array]';
    }

    function formatValue(val, defaultVal) {
        return val === undefined ? defaultVal : val;
    }

    function toMap(val, delimiter) {
        delimiter = delimiter === undefined ? ',' : delimiter;
        var map = {}, arr = isArray(val) ? val : val.split(delimiter), match;

        foreach(arr, function (key, val) {
            if ((match = /^(\d+)\.\.(\d+)$/.exec(val))) {
                for (var i = parseInt(match[1], 10); i <= parseInt(match[2], 10); i++) {
                    map[i.toString()] = true;
                }
            } else {
                map[val] = true;
            }
        });

        return map;
    }

    function trim(str) {
        return str.replace(/(?:^[ \t\n\r]+)|(?:[ \t\n\r]+$)/g, '');
    }

    function getCssList(css) {
        var list = {},
            reg = /\s*([\w\-]+)\s*:([^;]*)(;|$)/g,
            match;
        while ((match = reg.exec(css))) {
            var key = _trim(match[1].toLowerCase()),
                val = _trim(_toHex(match[2]));
            list[key] = val;
        }
        return list;
    }

    function inArray(val, arr) {
        for (var i = 0, len = arr.length; i < len; i++) {
            if (val === arr[i]) {
                return i;
            }
        }
        return -1;
    }

    function getAttrList(tag) {
        var list = {},
            reg = /\s+(?:([\w\-:]+)|(?:([\w\-:]+)=([^\s"'<>]+))|(?:([\w\-:"]+)="([^"]*)")|(?:([\w\-:"]+)='([^']*)'))(?=(?:\s|\/|>)+)/g,
            match;
        while ((match = reg.exec(tag))) {
            var key = (match[1] || match[2] || match[4] || match[6]).toLowerCase(),
                val = (match[2] ? match[3] : (match[4] ? match[5] : match[7])) || '';
            list[key] = val;
        }
        return list;
    }

    function formatUrl(url, mode, host, pathname) {
        mode = formatValue(mode, '').toLowerCase();
        if (url.substr(0, 5) != 'data:') {
            url = url.replace(/([^:])\/\//g, '$1/');
        }
        if (inArray(mode, ['absolute', 'relative', 'domain']) < 0) {
            return url;
        }
        host = host || location.protocol + '//' + location.host;
        if (pathname === undefined) {
            var m = location.pathname.match(/^(\/.*)\//);
            pathname = m ? m[1] : '';
        }
        var match;
        if ((match = /^(\w+:\/\/[^\/]*)/.exec(url))) {
            if (match[1] !== host) {
                return url;
            }
        } else if (/^\w+:/.test(url)) {
            return url;
        }
        if (/^\//.test(url)) {
            url = host + getRealPath(url.substr(1));
        } else if (!/^\w+:\/\//.test(url)) {
            url = host + getRealPath(pathname + '/' + url);
        }

        if (mode === 'relative') {
            url = getRelativePath(host + pathname, 0).substr(2);
        } else if (mode === 'absolute') {
            if (url.substr(0, host.length) === host) {
                url = url.substr(host.length);
            }
        }
        return url;

        function getRelativePath(path, depth) {
            if (url.substr(0, path.length) === path) {
                var arr = [];
                for (var i = 0; i < depth; i++) {
                    arr.push('..');
                }
                var prefix = '.';
                if (arr.length > 0) {
                    prefix += '/' + arr.join('/');
                }
                if (pathname == '/') {
                    prefix += '/';
                }
                return prefix + url.substr(path.length);
            } else {
                if ((match = /^(.*)\//.exec(path))) {
                    return getRelativePath(match[1], ++depth);
                }
            }
        }

        function getRealPath(path) {
            var parts = path.split('/'), paths = [];
            for (var i = 0, len = parts.length; i < len; i++) {
                var part = parts[i];
                if (part == '..') {
                    if (paths.length > 0) {
                        paths.pop();
                    }
                } else if (part !== '' && part != '.') {
                    paths.push(part);
                }
            }
            return '/' + paths.join('/');
        }
    }

    function getHtmlTagMap(htmlTags) {
        var htmlTagMap = {};
        foreach(htmlTags, function (key, val) {
            var arr = key.split(',');
            for (var i = 0, len = arr.length; i < len; i++) {
                htmlTagMap[arr[i]] = toMap(val);
            }
        });

        return htmlTagMap;
    }


    uc.util.HtmlUtil = {
        formatHtml: function (html, htmlTags) {
            if (html == null) {
                html = '';
            }

            html = html.replace(/(<(?:pre|pre\s[^>]*)>)([\s\S]*?)(<\/pre>)/ig, function ($0, $1, $2, $3) {
                return $1 + $2.replace(/<(?:br|br\s[^>]*)>/ig, '\n') + $3;
            });
            html = html.replace(/<(?:br|br\s[^>]*)\s*\/?>\s*<\/p>/ig, '</p>');
            html = html.replace(/(<(?:p|p\s[^>]*)>)\s*(<\/p>)/ig, '$1<br />$2');
            html = html.replace(/\u200B/g, '');
            html = html.replace(/\u00A9/g, '&copy;');
            html = html.replace(/\u00AE/g, '&reg;');
            html = html.replace(/<[^>]+/g, function ($0) {
                return $0.replace(/\s+/g, ' ');
            });

            var urlType = urlType || '',
                wellFormatted = formatValue(wellFormatted, true),
                indentChar = formatValue(indentChar, '\t'),
                fontSizeList = 'xx-small,x-small,small,medium,large,x-large,xx-large'.split(',');

            htmlTags = htmlTags || {
                img: ['id', 'class', 'src', 'width', 'height', 'border', 'alt', 'title', 'align', '.width', '.height', '.border']
            };
            var htmlTagMap = getHtmlTagMap(htmlTags);

            if (!htmlTagMap.script) {
                html = html.replace(/(<(?:script|script\s[^>]*)>)([\s\S]*?)(<\/script>)/ig, '');
            }
            if (!htmlTagMap.style) {
                html = html.replace(/(<(?:style|style\s[^>]*)>)([\s\S]*?)(<\/style>)/ig, '');
            }

            var tagStack = [];
            html = html.replace(re, function ($0, $1, $2, $3, $4, $5, $6) {
                var full = $0,
                    startNewline = $1 || '',
                    startSlash = $2 || '',
                    tagName = $3.toLowerCase(),
                    attr = $4 || '',
                    endSlash = $5 ? ' ' + $5 : '',
                    endNewline = $6 || '';

                if (htmlTags && !htmlTagMap[tagName]) {
                    return '';
                }
                if (endSlash === '' && SINGLE_TAG_MAP[tagName]) {
                    endSlash = ' /';
                }
                if (INLINE_TAG_MAP[tagName]) {
                    if (startNewline) {
                        startNewline = ' ';
                    }
                    if (endNewline) {
                        endNewline = ' ';
                    }
                }
                if (PRE_TAG_MAP[tagName]) {
                    if (startSlash) {
                        endNewline = '\n';
                    } else {
                        startNewline = '\n';
                    }
                }
                if (wellFormatted && tagName == 'br') {
                    endNewline = '\n';
                }
                if (BLOCK_TAG_MAP[tagName] && !PRE_TAG_MAP[tagName]) {
                    if (wellFormatted) {
                        if (startSlash && tagStack.length > 0 && tagStack[tagStack.length - 1] === tagName) {
                            tagStack.pop();
                        } else {
                            tagStack.push(tagName);
                        }
                        startNewline = '\n';
                        endNewline = '\n';
                        for (var i = 0, len = startSlash ? tagStack.length : tagStack.length - 1; i < len; i++) {
                            startNewline += indentChar;
                            if (!startSlash) {
                                endNewline += indentChar;
                            }
                        }
                        if (endSlash) {
                            tagStack.pop();
                        } else if (!startSlash) {
                            endNewline += indentChar;
                        }
                    } else {
                        startNewline = endNewline = '';
                    }
                }
                if (attr !== '') {
                    var attrMap = getAttrList(full);
                    if (tagName === 'font') {
                        var fontStyleMap = {}, fontStyle = '';
                        foreach(attrMap, function (key, val) {
                            if (key === 'color') {
                                fontStyleMap.color = val;
                                delete attrMap[key];
                            }
                            if (key === 'size') {
                                fontStyleMap['font-size'] = fontSizeList[parseInt(val, 10) - 1] || '';
                                delete attrMap[key];
                            }
                            if (key === 'face') {
                                fontStyleMap['font-family'] = val;
                                delete attrMap[key];
                            }
                            if (key === 'style') {
                                fontStyle = val;
                            }
                        });
                        if (fontStyle && !/;$/.test(fontStyle)) {
                            fontStyle += ';';
                        }
                        foreach(fontStyleMap, function (key, val) {
                            if (val === '') {
                                return;
                            }
                            if (/\s/.test(val)) {
                                val = "'" + val + "'";
                            }
                            fontStyle += key + ':' + val + ';';
                        });
                        attrMap.style = fontStyle;
                    }
                    foreach(attrMap, function (key, val) {
                        if (FILL_ATTR_MAP[key]) {
                            attrMap[key] = key;
                        }
                        if (inArray(key, ['src', 'href']) >= 0) {
                            attrMap[key] = formatUrl(val, urlType);
                        }
                        if (htmlTags && key !== 'style' && !htmlTagMap[tagName]['*'] && !htmlTagMap[tagName][key] ||
                            tagName === 'body' && key === 'contenteditable') {
                            delete attrMap[key];
                        }
                        if (key === 'style' && val !== '') {
                            var styleMap = getCssList(val);
                            foreach(styleMap, function (k, v) {
                                if (htmlTags && !htmlTagMap[tagName].style && !htmlTagMap[tagName]['.' + k]) {
                                    delete styleMap[k];
                                }
                            });
                            var style = '';
                            foreach(styleMap, function (k, v) {
                                style += k + ':' + v + ';';
                            });
                            attrMap.style = style;
                        }
                    });
                    attr = '';
                    foreach(attrMap, function (key, val) {
                        if (key === 'style' && val === '') {
                            return;
                        }
                        val = val.replace(/"/g, '&quot;');
                        attr += ' ' + key + '="' + val + '"';
                    });
                }
                if (tagName === 'font') {
                    tagName = 'span';
                }
                return startNewline + '<' + startSlash + tagName + attr + endSlash + '>' + endNewline;
            });

            return trim(html);
        },

        transformImageUrl: function (html, transferCall) {
            var re = /(\s*)<(\/)?([\w\-:]+)((?:\s+|(?:\s+[\w\-:]+)|(?:\s+[\w\-:]+=[^\s"'<>]+)|(?:\s+[\w\-:"]+="[^"]*")|(?:\s+[\w\-:"]+='[^']*'))*)(\/)?>(\s*)/g,
                tagStack = [];

            var htmlTags = {
                img: ['id', 'class', 'src', 'width', 'height', 'border', 'alt', 'title', 'align', '.width', '.height', '.border']
            };
            var htmlTagMap = getHtmlTagMap(htmlTags);

            html = html.replace(re, function ($0, $1, $2, $3, $4, $5, $6) {
                var full = $0,
                    startNewline = $1 || '',
                    startSlash = $2 || '',
                    tagName = $3.toLowerCase(),
                    attr = $4 || '',
                    endSlash = $5 ? ' ' + $5 : '',
                    endNewline = $6 || '';

                if (htmlTags && !htmlTagMap[tagName]) {
                    return startNewline + '<' + startSlash + tagName + attr + endSlash + '>' + endNewline;
                }

                if (attr !== '') {
                    var attrMap = getAttrList(full);

                    foreach(attrMap, function (key, val) {
                        if (FILL_ATTR_MAP[key]) {
                            attrMap[key] = key;
                        }

                        if (inArray(key, ['src', 'href']) >= 0) {
                            if (transferCall) {
                                var newUrl = transferCall(val);
                                attrMap[key] = newUrl || val;
                            }
                        }

                        if (htmlTags && key !== 'style' && !htmlTagMap[tagName]['*'] && !htmlTagMap[tagName][key] ||
                            tagName === 'body' && key === 'contenteditable') {
                            delete attrMap[key];
                        }
                        if (key === 'style' && val !== '') {
                            var styleMap = getCssList(val);
                            foreach(styleMap, function (k, v) {
                                if (htmlTags && !htmlTagMap[tagName].style && !htmlTagMap[tagName]['.' + k]) {
                                    delete styleMap[k];
                                }
                            });
                            var style = '';
                            foreach(styleMap, function (k, v) {
                                style += k + ':' + v + ';';
                            });
                            attrMap.style = style;
                        }
                    });
                    attr = '';
                    foreach(attrMap, function (key, val) {
                        if (key === 'style' && val === '') {
                            return;
                        }
                        val = val.replace(/"/g, '&quot;');
                        attr += ' ' + key + '="' + val + '"';
                    });
                }
                return startNewline + '<' + startSlash + tagName + attr + endSlash + '>' + endNewline;
            });

            return trim(html);
        },

        replaceImage: function (html, replaceConditionCall) {
            var re = /(\s*)<(\/)?([\w\-:]+)((?:\s+|(?:\s+[\w\-:]+)|(?:\s+[\w\-:]+=[^\s"'<>]+)|(?:\s+[\w\-:"]+="[^"]*")|(?:\s+[\w\-:"]+='[^']*'))*)(\/)?>(\s*)/g,
                tagStack = [];

            var htmlTags = {
                img: ['id', 'class', 'src', 'width', 'height', 'border', 'alt', 'title', 'align', '.width', '.height', '.border']
            };
            var htmlTagMap = getHtmlTagMap(htmlTags);

            html = html.replace(re, function ($0, $1, $2, $3, $4, $5, $6) {
                var full = $0,
                    startNewline = $1 || '',
                    startSlash = $2 || '',
                    tagName = $3.toLowerCase(),
                    attr = $4 || '',
                    endSlash = $5 ? ' ' + $5 : '',
                    endNewline = $6 || '';

                if (htmlTags && !htmlTagMap[tagName]) {
                    return startNewline + '<' + startSlash + tagName + attr + endSlash + '>' + endNewline;
                }

                var imagePath;
                if (attr !== '') {
                    var attrMap = getAttrList(full);

                    foreach(attrMap, function (key, val) {
                        if (FILL_ATTR_MAP[key]) {
                            attrMap[key] = key;
                        }

                        if (inArray(key, ['src', 'href']) >= 0) {
                            imagePath = attrMap[key];
                        }
                    });
                }

                var replace = false;
                if (replaceConditionCall) {
                    replace = replaceConditionCall(imagePath);
                }

                if (replace === false) {
                    return "";
                }
                else if (replace === true) {
                    return startNewline + '<' + startSlash + tagName + attr + endSlash + '>' + endNewline; //ʵ��ͼ�Ļ���
                }
                else {
                    return startNewline + replace + endNewline;
                }
            });

            return trim(html);
        },
        getUrlArgs: function () {
            var args = {};
            var match;
            var search = decodeURIComponent(location.search.substring(1));
            var reg = /(?:([^&amp;]+)=([^&amp;]+))/g;
            while ((match = reg.exec(search)) !== null) {
                args[match[1]] = match[2];
            }
            return args;
        }
    };
})(uc);