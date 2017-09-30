appModule.directive("noticeEditor",["api","utils","selection","$translate",function(api,utils,selection,translate){
    return{
        templateUrl: "templates/editor.html",
        restrict: "AE",
        scope:{
            content: "=",
            placeHolder:"=",
            openCloud:"&",
            editDisabled:"="
        },
        link: function($scope,element){
            $scope.setInputEditorCaretPosition = function($event){
                utils.setInputEditorCaretPosition($($event.currentTarget));
            };

            $scope.onInputCaretPositionKeyUp = function($event){
                utils.onInputCaretPositionKeyUp($($event.currentTarget));
            };

            var offContentReady = $scope.$on("ContentReady",function(e,content){
                $("#content",element).html(content);
            });

            selection.setCursorEnd($("#content",element)[0]);

            $("a.btn",element).on("mousedown",function(){
                $(this).addClass("enter");
            });
            $("a.btn",element).on("mouseup",function(){
                $(this).removeClass("enter");
            });

            $("#content",element).on("input",function(){
                if($scope.isBlockInputing){
                    return;
                }
                if($(event.currentTarget).find("[style*='font-size: x-small']").length>0 || $(event.currentTarget).find("font[size=1]").length>0){
                    return ; //忽略设置字体大小的中间态，字体改变后再次触发。
                }
                $scope.isBlockInputing = true;
                setTimeout(function(){
                    $scope.updateContent(function(){});
                    $scope.isBlockInputing = false;
                },200);

            });

            $('#content',element).on('paste', function(e) {
                var text = (e.originalEvent || e).clipboardData.getData('text/html');
                var p = text.lastIndexOf('</html>');
                if (p < 1) {
                    text = (e.originalEvent || e).clipboardData.getData('text');
                    if(text.length>0){
                        e.preventDefault();
                        document.execCommand("insertText",false,text);
                    }
                    return;
                }

                e.preventDefault();
                text = text.substr(0, p+7);
                text = removeUseless(text);
                var clip = $('<COPYDATA>' + text + '</COPYDATA>');
                if (clip.find("meta[content='Excel.Sheet']").length > 0 || clip.find("meta[content='Word.Document']").length > 0) {
                    clip.find('LINK').remove();
                    var style = clip.find('style').html();
                    style=filterStyle(style);
                    clip.find('style').html(style);
                    $(this).append('<style>'+style +'</style>');
                }
                var cpElement = $("<div>"+text.replace(/[\r\n]+/gi,"").match(/<body[\s\S]*<\/body>/gi)[0]+"</div>");
                filterElementStyle(cpElement);
                var noLargeImage = convertLocalImage(cpElement);
                convertSepcialTag(cpElement);
                document.execCommand('insertHtml', false, cpElement[0].outerHTML);
                addBlinkAttribute();
                if(!noLargeImage){
                    api.notifyError(translate.instant("publish.alert.insert-large-image"));
                }
            });

            $('#content',element).on('mouseup keyup',function(){
                checkSelectedStyle();
            });




            $('.ta-toolbar .btn', element).click(function(e) {
                $("#content",element).focus();

                var $self = $(this);
                var sel = document.getSelection();
                var rng = null;

                if($(this).data('role') == "img"){
                    utils.openImageDialog(function(data){
                        if(data.errorCode === 0){
                            if(data.paths && data.paths.length > 0){
                                data.paths.forEach(function(path){
                                    document.execCommand("insertHtml", false, "<p><img src='"+path+"' type='local'/></p>");
                                });
                            }
                        }else{
                            api.notifyError(translate.instant("publish.alert.insert-large-image"));
                        }
                    },1024*1024,true); //大图会导致CEF 无响应，暂时设置1M
                }else if($(this).data('role') == "hr"){

                    if(sel.type !== "Caret"){
                        return;
                    }
                    var blockquote = getBlockquote(sel);
                    if(blockquote){
                        return;
                    }
                    document.execCommand("styleWithCSS", true, null);
                    document.execCommand('insertHtml',false,'<hr/>');
                    rng = sel.getRangeAt(0);
                    var start = rng.startContainer;
                    if(start.nodeType == 1 && !start.childNodes[rng.startOffset]){
                        var tmp = document.createElement('br');
                        rng.insertNode(tmp);
                        rng.setStartBefore(tmp);
                        rng.collapse(true);
                        sel.removeAllRanges();
                        sel.addRange(rng);
                    }else{
                        if(start.nodeType == 1){
                            rng.setStartBefore(start.childNodes[rng.startOffset]);
                            rng.collapse(true);
                            sel.removeAllRanges();
                            sel.addRange(rng);
                        }
                    }
                }else if($(this).data('role') == "blockquote"){
                    if(sel.type !== "Caret" ){
                        return;
                    }
                    var sameTag = getBlockquote(sel);
                    if(sameTag){
                        return;
                    }
                    document.execCommand('insertHtml',false,'<blockquote><br/></blockquote>');
                }else if($(this).data('role') == "rollback"){
                    $scope.onBack();
                }else if($(this).data('role') == "cloud"){
                    $scope.addCloudFile();
                }else{
                    document.execCommand("styleWithCSS", true, null);
                    document.execCommand($self.data('role'), false, null);
                }
                checkSelectedStyle();
            });

            $scope.fonts = [12,14,16,18,24,36];
            $scope.colors = ['#03111f','#777777','#ff2600','#ff9200','#00f900','#0333ff'];
            $scope.font = {
                fontSize: $scope.fonts[2],
                color:$scope.colors[1]
            };

            //超链接
            $scope.toggleLinkOptions = function(){
                event.stopPropagation();



                $scope.isShowLinkOptions = !$scope.isShowLinkOptions;
                if($scope.isShowLinkOptions){
                    var aTag = getCurrentLink();
                    if(aTag){
                        $scope.selectedAnchor = aTag;
                        $scope.linkURL = getHref(aTag);
                    }else{
                        $scope.selectedAnchor = null;
                        $scope.linkURL = "";
                        $scope.tempSelection = selection.saveSelection();
                    }
                }
            };
            $scope.onLinkInputKeyup = function($event){
                if($event.keyCode === 13){
                    $scope.addLink();
                }
            };

            $scope.addLink = function(){
                event.stopPropagation();
                if($scope.linkURL){
                    if(!$scope.linkURL.startsWith("http")){
                        $scope.linkURL = "http://"+$scope.linkURL;
                    }
                    if($scope.selectedAnchor){
                        var text = $($scope.selectedAnchor).text();
                        var url = $scope.selectedAnchor.href;
                        if(url.endsWith("/")){
                            url = url.substr(0,url.length-1);
                        }
                        if(text == url){
                            $($scope.selectedAnchor).text($scope.linkURL);
                        }
                        $scope.selectedAnchor.href = $scope.linkURL;
                    }else{
                        selection.restoreSelection($scope.tempSelection);
                        document.execCommand("createlink", false, $scope.linkURL);
                        addBlinkAttribute();
                        selection.clearSelection();
                    }
                }
                $scope.linkURL = "";
                $scope.toggleLinkOptions();
            };

            function getCurrentLink(){
                var anchorTag = selection.getSelectionParent(window.getSelection(),"a");
                if(anchorTag!=null && !$(anchorTag).hasClass("cloudFile")){
                    return anchorTag;
                }
                return null;
            }

            function getHref(anchor){
                var text = $(anchor).text();
                var url = anchor.href;
                if(url.endsWith("/")){
                    url = url.substr(0,url.length-1);
                }
                return url;
            }

            //字体名
            $scope.toggleFontFamilyOptions = function(){
                event.stopPropagation();
                $scope.isShowFontFamilyOptions = !$scope.isShowFontFamilyOptions;
            };

            $scope.updateFontFamily = function(font){
                event.stopPropagation();
                $scope.toggleFontFamilyOptions();
                $scope.font.fontFamily = font;
                document.execCommand("fontname", false, font.value);
            };

            //字体颜色
            $scope.toggleFontColorOptions = function(){
                event.stopPropagation();
                $scope.isShowFontColorOptions = !$scope.isShowFontColorOptions;
                if($scope.isShowFontColorOptions){
                    $scope.tempSelection = selection.saveSelection();
                }
            };

            $scope.updateFontColor = function(color){
                event.stopPropagation();
                $scope.toggleFontColorOptions();
                $scope.font.color = color;
                selection.restoreSelection($scope.tempSelection);
                document.execCommand("forecolor", false, color.replace("#",""));
            };

            //字体大小
            $scope.toggleFontSizeOptions = function(value){
                event.stopPropagation();
                if(value !== undefined){
                    $scope.isShowFontSizeOptions = value;
                }else{
                    $scope.isShowFontSizeOptions = !$scope.isShowFontSizeOptions;
                }
                if($scope.isShowFontSizeOptions){
                    $scope.tempSelection = selection.saveSelection();
                }
            };

            $scope.updateFontSize = function(delta){
                $("#content",element).focus();
                $scope.font.fontSize =delta;
                $scope.font.selectedFontSize = delta;
                $scope.$broadcast("fontSizeChanged",$scope.font.selectedFontSize);
            };

            var stopWatch = $scope.$on("fontSizeChanged", function(e,newV){
                if(!!newV){
                    // if($("#content",element).html()){
                    selection.restoreSelection($scope.tempSelection);
                    var r = window.getSelection();
                    if(r.type === "Caret"){
                        var sel = document.getSelection();
                        var range = sel.getRangeAt(0);
                        var span = document.createElement('span');
                        span.style.fontSize=newV+"px";
                        span.innerHTML = "\u200B"; //插入占位符
                        range.insertNode(span);
                        var tRange = document.createRange();
                        tRange.selectNodeContents(span);
                        tRange.setStart(span,0);
                        tRange.setEnd(span,1);
                        tRange.collapse(false);
                        sel.removeAllRanges();
                        sel.addRange(tRange);
                        return;
                    }
                    document.execCommand("styleWithCSS", true, null);
                    document.execCommand("fontSize", false, '1');
                    $scope.updateContent(function(){
                        resetFont(newV);
                    });
                }
                // }
            });

            //云盘
            $scope.addCloudFile= function(){
                $("#content",element).focus();
                var r = window.getSelection();
                if(r.type !== "Caret"){
                    return;
                }
                $scope.tempSelection = selection.saveSelection();
                $scope.openCloud();
            };

            var stopWatchContent = $scope.$watch("content",function(newV,oldV){
                if(!newV){
                    $("#content",element).empty();
                }
            });

            var stopWatchPlaceholder= $scope.$watch("placeHolder",function(newV){
                if(newV !== undefined){
                    $("#content",element).attr("placeholder",newV);
                }
            });

            var stopWatchEditDisabled= $scope.$watch("editDisabled",function(newV){
                if(!!newV){
                    $("#content",element).removeAttr("contenteditable");
                    $(".disableShadow",element).css("display","block");
                }
            });

            var offImageUpload = $scope.$on("onLocalImageStartUpload",function(){
                var urls = [];
                $("#content img[type='local']", element).each(function(){
                    urls.push($(this).attr("src"));
                });
                if(urls.length>0){
                    var promise = utils.uploadImages(urls);
                    promise.then(function(result){
                        var message="",code=0;
                        result.forEach(function(n){
                            if(!n.newURL){
                                message += n.originalURL+",";
                            }else{
                                $("#content img[type='local']", element).each(function(){
                                    if($(this).attr("src") === n.originalURL){
                                        $(this).attr("src",n.newURL);
                                        $(this).removeAttr("type");
                                    }
                                });
                            }
                        });
                        if(message.length>0){
                            code = -1;
                        }
                        $scope.$emit("onLocalImagesUploaded",{
                            "errorCode":code,
                            "errorMessage":message,
                            "data":$("#content",element).html()
                        });
                    });
                }else{
                    $scope.$emit("onLocalImagesUploaded",{
                        "errorCode":0,
                        "errorMessage":"",
                        "data":$("#content",element).html()
                    });
                }
            });

            //处理云盘返回的文件JSON对象
            var offClouldFileSelected = $scope.$on("onCloudFileSelected",function(event,args){
                if(!args){
                    return;
                }
                if(!$scope.tempSelection){
                    return;
                }
                var iconURL = args.fileIcon;
                var size = args.filesize;
                var division = 1024,unit = "KB";
                if(size> division*division){
                   division *= division;
                    unit = "MB";
                }
                size = (size/division).toFixed(2)+unit;
                delete args.fileIcon;

                drawCloudFile(iconURL,args.filename,translate.instant("publish.editor.size")+size,function(url){
                    var base64data = utils.utf8ToBase64(JSON.stringify(args));
                    var href = "http://gnet-uc-native.quanshi.com?act=cloudFileDownload&data="+base64data;
                    var anchor = "<a href='"+href+"' target='_blank' class='cloudFile'>"
                                +"<img src='"+url+"' />"
                                +"</a>";
                    selection.restoreSelection($scope.tempSelection);
                    document.execCommand('insertHtml', false, anchor);
                    selection.clearSelection();
                });

            });

            function drawCloudFile(imageURL, fileName,fileSize,callback){
                var canvas = document.createElement("canvas");
                canvas.width= "300";
                canvas.height = "96";
                var ctx = canvas.getContext("2d");
                var image = new Image();
                image.onload = function(){
                    ctx.drawImage(image,20,25,40,40);
                    ctx.font="12px 微软雅黑";
                    ctx.fillText(adjustFillText(ctx,fileName,200),80,35);
                    ctx.fillText(fileSize,80,60);
                    callback(canvas.toDataURL());
                };
                image.src= imageURL;
            }

            function adjustFillText(ctx,text,limit){
                var surfix = "...";
                var newText=text;
                while(ctx.measureText(newText + surfix) > limit){
                    newText = newText.splice(newText.length-1,0);
                }
                if(newText < text){
                    newText+=surfix;
                }
                return newText;
            }

            $scope.$on("$destroy",function(){
                stopWatch();
                stopWatchContent();
                stopWatchPlaceholder();
                offImageUpload();
                stopWatchEditDisabled();
                offContentReady();
            });

            //更新model
            $scope.updateContent = function(func){
                if(typeof func === "function"){
                    func();
                }
                // console.log($("#content",element).html());
                if($scope.previousContent !== null){
                    $scope.pushContentToHistory($scope.previousContent);
                }

                $scope.previousContent = $("#content",element).html();
                utils.applyScope($scope,function(){
                    $scope.content = $("#content",element).html();

                });
            };

            function resetFont(fontSize){
                $("#content font[size=1]", element).removeAttr("size").css("font-size", fontSize + "px");
                $("#content [style*='font-size: x-small']", element).removeAttr("size").css("font-size", fontSize + "px");
            }

            function addBlinkAttribute(){
                $("#content",element).find("a:not([target])").attr("target","_blank");
                $scope.updateContent();
            }


            //---------------------- 处理选中样式  -----------------------//

            function checkSelectedStyle(){
                var selection = document.getSelection();
                if(selection.anchorNode){
                    var element = null;
                    if(selection.anchorNode.nodeType == 1 && selection.anchorNode == selection.focusNode ){
                        element = selection.anchorNode;
                    }else{
                        if(selection.anchorNode.parentElement == selection.focusNode.parentElement){
                            element = selection.focusNode.parentElement;
                        }
                    }
                    if(element){
                    var toolbarStyle = getElementStyle(element);
                    setToolbarStatus(toolbarStyle);
                    }
                }
            }

            function componentToHex(c) {
                var hex = c.toString(16);
                return hex.length == 1 ? "0" + hex : hex;
            }

            function rgbToHex(r, g, b) {
                return "#" + componentToHex(r) + componentToHex(g) + componentToHex(b);
            }

            function initStyle(){
                return {
                    "bold":false,
                    "italic":false,
                    "underline":false,
                    "align-left":false,
                    "align-right":false,
                    "align-center":false,
                    "color":$scope.colors[0],
                    "font":$scope.fonts[2],
                    "ol":false,
                    "ul":false
                };
            }

            //获取选择节点的toolbar样式
            function getElementStyle(element){
                var styles=initStyle();
                var computedStyles = window.getComputedStyle(element,null);
                if(computedStyles.fontWeight === "bold"){
                    styles.bold = true;
                }
                if(computedStyles.fontStyle === "italic"){
                    styles.italic = true;
                }
                if(computedStyles.textDecoration === "underline"){
                    styles.underline = true;
                }
                if(computedStyles.textAlign ==="left"){
                    styles["align-left"] = true;
                }
                if(computedStyles.textAlign ==="center"){
                    styles["align-center"] = true;
                }
                if(computedStyles.textAlign ==="right"){
                    styles["align-right"] = true;
                }

                styles.font= parseFloat(computedStyles.fontSize);

                var rgbs = computedStyles.color.match(/(\d{1,3})/gi);
                styles.color = rgbToHex(parseInt(rgbs[0]),parseInt(rgbs[1]),parseInt(rgbs[2]));

                styles.fontFamily = computedStyles.fontFamily;
                if($(element).closest('ol').length>0){
                    styles.ol = true;
                }
                if($(element).closest('ul').length>0){
                    styles.ul = true;
                }

                return styles;
            }

            //合并样式
            function mergeElementStyle(from,to){
                var style=initStyle();
                if(from.bold == to.bold){
                    style.bold = from.bold;
                }
                if(from.italic == to.italic){
                    style.italic = from.italic;
                }
                if(from.underline == to.underline){
                    style.underline = from.underline;
                }
                if(from.underline == to.underline){
                    style.underline = from.underline;
                }
                if(from["align-left"] == to["align-left"]){
                    style["align-left"] = from["align-left"];
                }
                if(from["align-center"] == to["align-center"]){
                    style["align-center"] = from["align-center"];
                }
                if(from["align-right"] == to["align-right"]){
                    style["align-right"] = from["align-right"];
                }
                if(from.font == to.font){
                    style.font = from.font;
                }
                if(from.color == to.color){
                    style.color = from.color;
                }
                if(from.fontFamily == to.fontFamily){
                    style.fontFamily = from.fontFamily;
                }
                return style;
            }

            //设置toolbar状态
            function setToolbarStatus(style){
                if(style.bold){
                    $(".ta-toolbar .btn[data-role='bold']", element).addClass("selected");
                }else{
                    $(".ta-toolbar .btn[data-role='bold']", element).removeClass("selected");
                }
                if(style.italic){
                    $(".ta-toolbar .btn[data-role='italic']", element).addClass("selected");
                }else{
                    $(".ta-toolbar .btn[data-role='italic']", element).removeClass("selected");
                }
                if(style.underline){
                    $(".ta-toolbar .btn[data-role='underline']", element).addClass("selected");
                }else{
                    $(".ta-toolbar .btn[data-role='underline']", element).removeClass("selected");
                }
                if(style["align-left"]){
                    $(".ta-toolbar .btn[data-role='justifyLeft']", element).addClass("selected");
                }else{
                    $(".ta-toolbar .btn[data-role='justifyLeft']", element).removeClass("selected");
                }
                if(style["align-center"]){
                    $(".ta-toolbar .btn[data-role='justifyCenter']", element).addClass("selected");
                }else{
                    $(".ta-toolbar .btn[data-role='justifyCenter']", element).removeClass("selected");
                }
                if(style["align-right"]){
                    $(".ta-toolbar .btn[data-role='justifyRight']", element).addClass("selected");
                }else{
                    $(".ta-toolbar .btn[data-role='justifyRight']", element).removeClass("selected");
                }
                if(style["ul"]){
                    $(".ta-toolbar .btn[data-role='insertUnorderedList']", element).addClass("selected");
                }else{
                    $(".ta-toolbar .btn[data-role='insertUnorderedList']", element).removeClass("selected");
                }
                if(style["ol"]){
                    $(".ta-toolbar .btn[data-role='insertOrderedList']", element).addClass("selected");
                }else{
                    $(".ta-toolbar .btn[data-role='insertOrderedList']", element).removeClass("selected");
                }
                utils.applyScope($scope,function(){
                    $scope.font.color = style.color;
                    $scope.font.fontSize = style.font>=10?style.font:16;
                });
            }

            //----------------------处理引用  ------------------------//
            //获取引用节点
            function getBlockquote(selection){
                return getSelectionParentTag(selection,"blockquote");
            }

            function getSelectionParentTag(selection,tagName){
                if(!selection || !selection.focusNode){
                    return null;
                }
                var target = null;
                if(selection.focusNode.nodeType==1 && selection.focusNode.tagName.toLowerCase() === tagName){
                    target = selection.focusNode;
                }else{
                    target = $(selection.focusNode.parentElement).closest(tagName)[0];
                }

                return target;
            }

            $('#content',element).on('keydown',function(e){
                //阻止默认的回退和恢复快捷键
                if(e.keyCode === 89 && e.ctrlKey){
                    e.preventDefault();
                    e.stopPropagation();
                    return false;
                }
                if(e.keyCode === 90 && e.ctrlKey){
                    e.preventDefault();
                    e.stopPropagation();
                    $scope.onBack();
                    return false;
                }
                var selection = window.getSelection();

                //处理引用换行的问题

                var li = getSelectionParentTag(selection,"li");
                if(li){
                    return true;
                }
                var blockquote = getBlockquote(selection);
                if(!blockquote){
                    return true;
                }
                if(e.keyCode === 13){ // 换行时 br 替换 div
                    if($scope.currentBlockquote == blockquote && $scope.currentKeyEnter){
                        $scope.currentKeyEnter = false;
                        $scope.curblqHeight = 0;
                        var brNode = document.createElement("br");
                        var brTags = $(blockquote).find("br");
                        var i= brTags.length-1
                        if(brTags[i]){
                            brTags[i].remove();
                        }

                        blockquote.parentElement.insertBefore(brNode, blockquote.nextSibling);
                        // brNode = document.createElement("br");
                        // blockquote.parentElement.insertBefore(brNode, blockquote.nextSibling);
                        selection.removeAllRanges();

                        var rng = document.createRange();
                        rng.selectNode(brNode);
                        rng.collapse(true);
                        selection.addRange(rng);
                        $scope.updateContent(function(){});
                        return false;
                    }
                    if(checkCursorEnd(blockquote)){
                        $scope.currentKeyEnter = true;
                    }
                    $scope.currentBlockquote = blockquote;
                    e.preventDefault();
                    var range = selection.getRangeAt(0),
                    br = document.createElement("br"),
                    textNode = document.createTextNode("\u00a0");
                    range.deleteContents();
                    range.insertNode(br);
                    range.collapse(false);
                    range.insertNode(textNode);
                    range.selectNodeContents(textNode);
                    // range.setStart(textNode,0);
                    range.collapse(true);
                    // range.extractContents();
                    selection.removeAllRanges();
                    selection.addRange(range);

                    return false;
                }
                $scope.currentKeyEnter = false;
                // if(e.keyCode === 8){
                //     $scope.curblqHeight = $(blockquote).height();
                //     if($(blockquote).text().length ===0){
                //       $(blockquote).remove();
                //       $scope.updateContent(function(){});
                //       return false;
                //     }
                //     return true;
                // }
            });

            //处理引用结束的问题
            //$('#content',element).on('keyup',function(e){//引用删除最后一行后引用结束
            //  var selection = window.getSelection();
            //  var blockquote = getBlockquote(selection);
            //  if(!blockquote){
            //    return true;
            //  }
            //  if(e.keyCode === 13){
            //    if($scope.currentBlockquote == blockquote && $scope.currentKeyEnter){
            //        $scope.currentKeyEnter = false;
            //        $scope.curblqHeight = 0;
            //        var brNode = document.createElement("br");
            //        var brTags = $(blockquote).find("br");
            //        for(var i= brTags.length-1,j=0; i>=0&&j<2;i--,j++){
            //            if(brTags[i]){
            //                brTags[i].remove();
            //            }
            //        }
            //
            //        blockquote.parentElement.insertBefore(brNode, blockquote.nextSibling);
            //        // brNode = document.createElement("br");
            //        // blockquote.parentElement.insertBefore(brNode, blockquote.nextSibling);
            //        selection.removeAllRanges();
            //
            //        var rng = document.createRange();
            //        rng.selectNode(brNode);
            //        rng.collapse(true);
            //        selection.addRange(rng);
            //        $scope.updateContent(function(){});
            //        return false;
            //    }
            //
            //    $scope.currentBlockquote = blockquote;
            //
            //
            //    return;
            //  }
            //  $scope.currentBlockquote = null;
            //
            //  return true;
            //});

            function checkCursorEnd(element){
                var range = window.getSelection().getRangeAt(0);
                var post_range = document.createRange();
                post_range.selectNodeContents(element);
                post_range.setStart(range.endContainer, range.endOffset);
                var next_text = post_range.cloneContents();
                return next_text.textContent.trim().length === 0;
            }
            //---------------- 处理回退操作-----------------------//
            $scope.history = [];
            $scope.previousContent = null;
            $scope.isBlockInputing = false;
            $scope.onBack = function(){
                var content = $scope.history.pop();
                if(content !== undefined){
                    $("#content",element).html(content);
                    $scope.content = content;
                    selection.setCursorEnd($("#content",element)[0]);
                }
                if($scope.history.length === 0){
                    $scope.previousContent = $("#content",element).html();
                }
            };
            $scope.pushContentToHistory = function(content){
                var length = $scope.history.length;
                if(length >= 5){
                    $scope.history.shift();
                }

                if(content!== undefined && content!= $scope.history[$scope.history.length-1]){
                    $scope.history.push(content);
                }
            };
            $scope.pushContentToHistory($("#content",element).html());

            //-------------------过滤粘贴的html的样式-------------------//
            //过滤style
            function filterStyle(style){
                var result = "";
                if(style){
                    result = style.replace('<!--', '').replace('-->', '').replace(/:\.\d+pt/g, ':1px'); //将非整形转换为整形
                    result = result.replace(/<!--\[if[\s\S]+<xml>[\s\S]+<\/xml><!\[endif\]-->/gi,""); //删除word 注释
                    result = result.replace(/<script>[\s\S]*<\/script>/gi,"");//删除javascript
                    result = result.replace(/mso(-\w+)+:[^;]+;/gi,""); //删除office 样式
                    result = result.replace(/text-(align|justify){1}:[\w-]+;/gi,""); //移除对齐样式
                    result = result.replace(/(margin|padding){1}[-\w]*:[^;]+;/gi,""); //移除边距
                    result = result.replace(/font-(family|kerning|stretch|style|variant|variant-ligatures|weight)+:[^;]+;/gi,""); //移除文字样式(保留字号)
                    result = result.replace(/color:[^;]+;/gi,""); //移除文字颜色
                    //  result = result.replace(/(background-)?color:[^;]+;/gi,""); //移除背景颜色样式
                    result = result.replace(/([;{\s]+)(left|right|top|bottom){1}:[^;]+;/gi,"$1");//移除定位
                }

                return result;
            }

            function filterInlineStyle(style){
                var result = "";
                if(style){
                    result = result.replace(/mso(-\w+)+:[^;]+/gi,""); //删除office 样式
                    result = result.replace(/text-(align|justify){1}:[\w-]+/gi,""); //移除对齐样式
                    result = result.replace(/(margin|padding){1}[-\w]*:[^;]+/gi,""); //移除边距
                    result = result.replace(/font-(family|kerning|stretch|style|variant|variant-ligatures|weight)+:[^;]+/gi,""); //移除文字样式(保留字号)
                    result = result.replace(/color:[^;]+/gi,""); //移除文字颜色
                    //  result = result.replace(/(background-)?color:[^;]+;/gi,""); //移除背景颜色样式
                    result = result.replace(/(left|right|top|bottom){1}:[^;]+/gi,"");//移除定位
                }
                return result;
            }

            //删除注释 javascript
            function removeUseless(text){
                var result = "";
                result = text.replace(/<!--\[if[\s\S]+<xml>[\s\S]+<\/xml><!\[endif\]-->/gi,""); //删除word 注释
                result = result.replace(/[\r\n]+/gi," ");//移除换行
                result = result.replace(/<script>[\s\S]*<\/script>/gi,"");//删除javascript
                result = result.replace("<!--StartFragment-->",""); //删除startfragment 注释
                result = result.replace("<!--EndFragment-->",""); //删除endfragment 注释

                return result;
            }

            //将加粗 斜体标签转为span
            function convertSepcialTag($ele){
                $ele.find("b,i").replaceWith(function(){
                    return $('<span/>', {
                        html: this.innerHTML
                    });
                });
            }

            //过滤节点样式
            function filterElementStyle($ele){
                var styleText = $ele.attr("style");
                $ele.attr("style",filterInlineStyle(styleText));
                $ele.children().each(function(){
                    filterElementStyle($(this));
                });
            }

            //给本地图片加标记 待发布时上传到FS
            function convertLocalImage($ele){
                var result = true;
                $ele.find('img').each(function(){
                    var src = this.src;
                    if(src && src.startsWith('file:///')){
                        src = src.substr(8);
                        if(utils.getFileSize(src) < 1024*1024){
                            $(this).attr("src",src);
                            $(this).attr('type','local');
                        }else{
                            $(this).remove();
                            result =false;
                        }
                    }
                });
                return result;
            }
        }
    };
}]);
