/**
 * Created by zuo.dai on 2015/8/19.
 */
(function(uc,  undefined){
    uc.modules.code.send_data = {};
    uc.modules.code.CodeMsgHandler = function () {

    };
    jQuery.extend(uc.modules.code.CodeMsgHandler.prototype,
        uc.interfaces.IIMEvent,
        uc.interfaces.IFSEvent,
        uc.interfaces.IGroupEvent, {
            //FS
            OnFileUploading: function (errorCode, taskId, percent) {
            },
            OnFileUploaded: function (errorCode, taskId, fileName, fileSize, operatorId, fid, operatorTime, downloadURL, convertDownloadURL) {
                var data = uc.modules.code.send_data;
                if (data._currTaskId != taskId) {
                    return;
                }
                if (errorCode != 0) {
                    if (data._currObj.callback) {
                        data._currObj.callback(errorCode, -1, "");
                    }
                    return;
                }
                if (data._currObj.sendObj.chatType == uc.constants.Chat.ChatType.SingleChat) {
                    uc.IIMService.sendCodeMessage(
                            {
                                userId: data._currObj.sendObj.chatObjectId,
                                siteId: data._currObj.sendObj.siteId,
                                resId: data._currObj.sendObj.resId
                            },
                            0, 0, data._currObj.name, data._currObj.type, data._currObj.head, data._currObj.des, data._currObj.size,
                            downloadURL, data._currObj.lines, operatorTime,
                            function(args){
                                if (!data._currObj.callback) {
                                    return;
                                }
                                if (args[0] == 0) {
                                    data._currObj.callback(args[0], args[1], downloadURL);
                                } else {
                                    data._currObj.callback(args[0], -1, "");
                                }
                            }
                        );
                } else if (data._currObj.sendObj.chatType == uc.constants.Chat.ChatType.GroupChat||
                    data._currObj.sendObj.chatType == uc.constants.Chat.ChatType.ProjectChat){
                    console.log(data, downloadURL);
                    uc.IGroupService.uploadCodeSnippet(
                        data._currObj.sendObj.chatObjectId,
                        0, data._currObj.name, data._currObj.type, data._currObj.head, data._currObj.des, data._currObj.size,
                        downloadURL, data._currObj.lines,
                        function(args){
                            console.log(args);
                            if (!data._currObj.callback) {
                                return;
                            }
                            if (args[0] == 0) {
                                data._currObj.callback(args[0], args[1], downloadURL);
                            } else {
                                data._currObj.callback(args[0], -1, "");
                            }
                        }
                    );
                }

                return;
            }
        }
    );

    var _this = null;
    uc.modules.code.CodeEditor = {
        init: function(){
            _this = this;
            this.bindEvent();
            var time = new Date().getTime();
            $('.code-name').attr("value",uc_resource.Code.Unnaming + time);
        },
		_check: function ($ipt, val, unVal) {
			if (val == null || val == "" || (undefined !== unVal && val == unVal)) {
				$ipt.addClass('waring-ipt');
				return false;
			} else {
				$ipt.removeClass('waring-ipt');
				return val;
			}
		},
		checkSend: function () {
			var res = {};
			var $ipt = $('.selectformat');
			res.type = this._check($ipt, $ipt.attr("codeType"),uc_resource.Code.SelectLanguage);
			
			$ipt = $('.code-name');
			var name = this._check($ipt, $.trim($ipt.val()));
			if (name !== false) {
				if (/[\?"/\<>\*\\\|:]/.test(name)) {
					$ipt.addClass('waring-ipt');
					name = false;
					uc.modules.component.PromptMessageBox.open({title: uc_resource.WindowTitle.Notification, message: uc_resource.Code.CodeErrorPrompt , buttonMode: "ok"}, function (result){
						this.close();
					});			
				}
			}
			res.name = name !== false ?$ipt.val() : false;

			var code = this._check($('.middle-items'), $.trim($('.code-input').val()));
			res.code = code !== false ? $('.code-input').val() : false;
			
			return res;
		},
		sendCodeClick: function () {
			var res = this.checkSend();
			for (var key in res) {
				if (res[key] === false) return;
			}
			
			var type = res.type;
			var name = res.name;
			var code = res.code;
			
			var des = $('.des-input').val();
			if (des > 200){
				des = des.substring(0, 200);
			}
			this.setCode(type, code);
			var htmlContent = '<!DOCTYPE html><html>' +
                '<head lang="en">' +
                '<STYLE TYPE="text/css" MEDIA=screen>' +
                '.hljs {display: block;overflow-x: auto;padding: 0;background: white;color: black;-webkit-text-size-adjust: none;margin-top: 5px;}' +
                '.hljs-comment,.hljs-annotation,.diff .hljs-header,.hljs-chunk,.apache .hljs-cbracket {color: #008000;}' +
                '.hljs-keyword,.hljs-id,.hljs-built_in,.smalltalk .hljs-class,.hljs-winutils,.bash .hljs-variable,.tex .hljs-command,.hljs-request,.hljs-status,.nginx .hljs-title,.xml .hljs-tag,.xml .hljs-tag .hljs-value { color: #00f;}' +
                '.hljs-string,.hljs-title,.hljs-parent,.hljs-tag .hljs-value,.hljs-rule .hljs-value,.ruby .hljs-symbol,.ruby .hljs-symbol .hljs-string,.hljs-template_tag,.django .hljs-variable,.hljs-addition,.hljs-flow,.hljs-stream,.apache .hljs-tag,.hljs-date,.tex .hljs-formula,.coffeescript .hljs-attribute,' +
                '.hljs-name {color: #a31515;}.' +
                'ruby .hljs-string,.hljs-decorator,.hljs-filter .hljs-argument,.hljs-localvars,.hljs-array,.hljs-attr_selector,.hljs-pseudo,.hljs-pi,.hljs-doctype,.hljs-deletion,.hljs-envvar,.hljs-shebang,.hljs-preprocessor,.hljs-pragma,.userType,.apache .hljs-sqbracket,.nginx .hljs-built_in,.tex .hljs-special,.hljs-prompt {color: #2b91af;}' +
                '.hljs-doctag,.hljs-xmlDocTag { color: #808080;}' +
                '.hljs-type,.hljs-typename { font-weight: bold; }' +
                '.vhdl .hljs-string { color: #666666; }' +
                '.vhdl .hljs-literal { color: #a31515; }' +
                '.vhdl .hljs-attribute { color: #00b0e8; }' +
                '.xml .hljs-attribute { color: #f00; }' +
                'html, body, div, span, applet, object, iframe, h1, h2, h3, h4, h5, h6, p, blockquote, pre,' +
                ' a,abbr, acronym, address, big, cite, code, del, dfn, em, img, ins, kbd, q, s, samp, small,' +
                ' strike,strong, sub, sup, tt, var, b, u, i, center, dl, dt, dd, ol, ul, li, fieldset, form,' +
                ' label, legend,table, caption, tbody, tfoot, thead, tr, th, td, article, aside, canvas, details,' +
                ' embed, figure,figcaption, footer, header,  menu, nav, output, ruby, section, summary, time,' +
                ' mark, audio,' +
                ' video { margin: 0; padding: 0;border: 0;font: inherit;font-size: 100%;vertical-align: baseline;}' +
                'html { line-height: 1}' +
                'html, body { width: 100%;height: 100%;overflow: hidden;font-size: 12px;line-height: 1.3em;font-family: "Microsoft YaHei" !important; -webkit-font-smoothing: antialiased}' +
                'ul, ol, li{ list-style: none;	background: #f5f5f5;color: #555555;border-right: 1px solid #dadada;padding: 0 5px;}' +
                '::-webkit-scrollbar{width: 6px; height: 6px;}' +
                '::-webkit-scrollbar-track-piece{background-color: #ffffff;-webkit-border-radius: 6px;min-height:50px;}' +
                '::-webkit-scrollbar-thumb:vertical{height: 5px;background-color: #cccccc;-webkit-border-radius: 10px;}' +
                '::-webkit-scrollbar-thumb:horizontal{width: 5px;background-color: #cccccc;-webkit-border-radius: 10px;}' +
                '.round-image{ border-radius: 50%;}' +
                '.tbcode{margin-top:-18px;margin-bottom:-50px;}' +
                '#td-number{width: 10px;}' +
                '.pre-numbering {float: left;padding: 0; text-align: center; color: #AAA;font-size: 12px;}' +
                '.code-preview {overflow-y: auto;overflow-x: auto;width: 100%;height: 100%;-webkit-user-select:none;-moz-user-select:none;-ms-user-select:none;user-select:none;	}' +
                '#testcode{padding: 2px 10px;float: left;font-size: 12px;border: none;overflow: hidden; -webkit-user-select:text;-moz-user-select:text;-ms-user-select:text;user-select:text;}' +
                '</STYLE>' +
                '<meta charset="UTF-8">' +
                '<title></title>' +
                '</head><body>';
			htmlContent += $('.preview').html();
			htmlContent += "</body></html>";
			
			var path = uc.IClientService.getDataDirectory(uc.constants.UI.DirectoryType.File) + "\\" + name + ".html";
			
			
			if (uc.util.FileUtil.saveFile(path, htmlContent) != 0) {
				console.log("failed to save file ", path);
				return;
			}
			var size = uc.util.FileUtil.getFileSize(path);
			//uc.util.FileUtil.openFile(path);
			var sendObj = uc.util.StorageUtil.getData("ShowCodeEdit");
			var lines = $('#testcode').text().split('\n').length;
			var head =  code.substring(0, 200);
			this.sendCode(sendObj.options, name, head, des, type, lines, size, path,
				function (errcode, msgId, url) {
					if (errcode == 0) {
						var data = {
							msgId: msgId,
							type: type,
							name: encodeURI(name),
							size: size,
							lines: lines,
							head: encodeURI(head),
							des: encodeURI(des),
							path: encodeURI(path),
							url: encodeURI(url)
						};
						uc.IUIService.triggerCrossWindowEvent("SendCode",{
							data: JSON.stringify(data)
						});
						uc.util.WindowUtil.close();
					} else {//error

					}
				}
			);
		},
        bindEvent: function(){
			var self = this;
            $('.code-editor').on('click', function(e){
				var $target = $(e.target);
				if (!$target.hasClass('selectformat') && !$target.hasClass('code-type')) {
					$('.format-list').hide();
				}
				
				if($target.hasClass('type-item')) {
					$('.code-type').text($target.text());
					$('.selectformat').attr("codeType", $target.attr("codeType"));
					$('.code-type').addClass("initialize");
				}
				
				if (self.showInput) {
					$('.preview').hide();
					$('.code-input').show();
					$('.code-input').focus().select(0);
					self.showInput = false;
				} else if(!$target.hasClass('code-input')) {
					var code = $('.code-input').val();
					/*code = code.replace(/^\s+\n(\s*\S)/, '$1').replace(/\n\s*$/, '').replace(/^\s+$/, '');
					$('.code-input').val(code);*/
					var type = $('.selectformat').attr("codeType");
					if (code != "" && type!= null) {
						if (type != this.type || code != self.code) {
							self.setCode(type, code);
						}
						self.code = code;
						self.type = type;
						$('.preview').show();
						$('.code-input').hide();
					}
				}
				
				if ($target.hasClass('apply-btn')) {
					self.sendCodeClick();
				}
            });

            $('.selectformat').on('click',
                function(e){
                    $('.format-list').show();
                }
            );
            $('.code-preview').on('click',
                function(e){
                    _this.showInput = true;
                }
            );
            $('.cancel-btn').on('click',
                function(e){
                    uc.util.WindowUtil.close();
                }
            );
            $('.icon-close').on('click',
                function(e){
                    uc.util.WindowUtil.close();
                }
            );

            $(document).keyup(function(event){
                switch(event.keyCode) {
                    case 27:
                        uc.util.WindowUtil.close();
                }
            });
            uc.util.CaretUtil.setInputEditorCaretPosition($('.code-name'));
            uc.util.CaretUtil.setInputEditorCaretPosition($('.code-input'));
            uc.util.CaretUtil.setInputEditorCaretPosition($('.des-input'));
        },
        setCode: function (type, code) {
            var $numbering = $('.pre-numbering');
            $('#testcode').text(code);
            var lines = $('#testcode').text().split('\n').length;
            $numbering.empty();
            for(var i=1;i<=lines;i++){
                $numbering.append($('<li></li>').text(i));
            }
            $('#testcode').attr("class", type);
            $('pre code').each(function(i, block) {
                hljs.highlightBlock(block);
            });
        },

        //
        sendCode: function (sendObj, name, head, des, type, lines, size, filePath, callback) {
            uc.IFSService.uploadFile(filePath, function (args) {
                console.log(args);
                var errorCode = args[0]
                if (errorCode != 0) {
                    if (callback) {
                        callback(errorCode, -1);
                    }
                    return;
                }
                uc.modules.code.send_data._currTaskId = args[1];
                uc.modules.code.send_data._currObj = {
                    sendObj:sendObj,
                    name:name,
                    head:head,
                    des:des,
                    type:type,
                    lines:lines,
                    size:size,
                    filePath:filePath,
                    callback:callback
                };
            });
        }
    };
    jQuery(document).ready(function(){
        uc.init();
        uc.modules.code.CodeEditor.init();
    });
})(uc);