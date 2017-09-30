/**
 * Created with JetBrains WebStorm.
 * User: hougang.chen
 * Date: 15-12-23
 * Time: 下午2:54
 * To change this template use File | Settings | File Templates.
 * @param   _options
 *_options.target 拨号键盘父节点
 *_options.callBack 拨号键盘确定回调函数，返回两个参数：_audioType:int,_telNumber:String
 *_options.left 绝对定位左 x
 *_options.top  绝对定位上 y
 *_options.isUseVOIP /true = 语音模式，flase，电话模式
 */

(function (uc) {
	uc.modules.component.dialPad = function (_options) {

		if (!_options.target || (!_options.callBack && typeof _options.callBack == 'function' )) {
			return;
		}
		this.x = _options.left - 128 ;
		this.y = _options.top - 370;

		this.$target = _options.target;
		this.$callback = _options.callBack;
		this.$isUseVOIP = _options.isUseVOIP;
		this.clearCallback = _options.clearCallback;
		this.$dialPadDoc;
		this.$callListDoc;
		this.$calllist = [];
		this.$activity = false;
		if (!_options.target.jquery) {
			this.$target = $("#" + _options.target);
		}
		this.initUI();
		this.bindEvent();
		this.showintlTelInput();
	}
	jQuery.extend(uc.modules.component.dialPad.prototype, {
		initUI: function () {
			//call list
			this.$callListDoc = $("<div class='calllistpanel'/>");
			var listBox = $("<div class='listBox'>");
			var calllist = $("<div class='calllist'><ul></ul></div>");
			var footer = $("<div class='footer'><span class='icon-dialpaddown'/><span class='inputteltip'>输入电话号码</span></div>");
			listBox.append(calllist);
			listBox.append(footer);

			this.$callListDoc.append(listBox);

			//dialPad
			var cl = $("<div class='cl'></div>");
			this.$dialPadDoc = $("<div class='dialPad child-window-shadow'/>");
			var title = $("<div class='diaPad_title'/>");
			title.append($("<div class='diaPad_zoneSelect'/>"));
			title.append($("<div class='dialPad_inputBox'>" +
				"<input type='text' placeholder='"+uc_resource.Call.InputTelephoneNumber+"' class='input account telNumber' value='' spellcheck='false' autofocus tabindex='1'>" +
				"<input type='text' placeholder='"+uc_resource.Call.InputExtensionNumber+"' class='input account extensionNumber' value='' spellcheck='false' tabindex='2'>" +
				"</div>"));
			title.append(cl);
			title.append($("<div class='displadInputdel'></div>"));
			var main = $("<div class='dialPad_main'>");
			var mainList = $("<ul/>");
			var tempstr = "<li><a class='phoneNumber' href='javascript:void(0)'>1</a></li>"
				+ "<li><a class='phoneNumber' href='javascript:void(0)'>2</a></li>"
				+ "<li><a class='phoneNumber' href='javascript:void(0)'>3</a></li>"
				+ "<li><a class='phoneNumber' href='javascript:void(0)'>4</a></li>"
				+ "<li><a class='phoneNumber' href='javascript:void(0)'>5</a></li>"
				+ "<li><a class='phoneNumber' href='javascript:void(0)'>6</a></li>"
				+ "<li><a class='phoneNumber' href='javascript:void(0)'>7</a></li>"
				+ "<li><a class='phoneNumber' href='javascript:void(0)'>8</a></li>"
				+ "<li><a class='phoneNumber' href='javascript:void(0)'>9</a></li>"
				+ "<li><a class='phoneNumber' href='javascript:void(0)' style='font-size:24px;'>*</a></li>"
				+ "<li><a class='phoneNumber' href='javascript:void(0)'>0</a></li>"
				+ "<li><a class='phoneNumber' href='javascript:void(0)'>#</a></li>"
				//btn
				+ "<li class='cancelbtn'><a href='javascript:void(0)'><span class='icon-dialpadup'/></a></li>"
				+ "<li class='hidenumber'><a href='javascript:void(0)'></a></li>"
				+ "<li class='okbtn'><a  href='javascript:void(0)'>"+uc_resource.Call.DialPadTrue+"</a></li>"
				+ "<li class='okbtnhide'><a  href='javascript:void(0)'>"+uc_resource.Call.DialPadTrue+"</a></li>";
			mainList.append(tempstr);
			main.append(mainList);
			this.$dialPadDoc.append(title);
			this.$dialPadDoc.append(cl);
			this.$dialPadDoc.append(main);
			this.$dialPadDoc.append(cl);
			//telnumber error
			this.$dialPadDoc.append('<div class="dialPadInput_Error"><span>'+uc_resource.Call.InputTelNumberError+'</span></div>');
			//add to target doc
			this.$target.append(this.$callListDoc);
			this.$target.append(this.$dialPadDoc);
			this.$dialPadDoc.hide();
			//set position 
			this.$callListDoc.css("position", "absolute");
			this.$callListDoc.css("top", this.y);
			this.$callListDoc.css("left", this.x);

			this.$dialPadDoc.css("position", "absolute");
			this.$dialPadDoc.css("top", this.y);
			this.$dialPadDoc.css("left", this.x);

			this.$activity = true;
			this.$okbtn = $('.dialPad .dialPad_main .okbtn');
			this.$okbtnhide = $('.dialPad .dialPad_main .okbtnhide');
			this.$cancelbtn = $('.dialPad .dialPad_main .cancelbtn');
			this.$telInput = $(".dialPad .dialPad_inputBox .telNumber");
			this.$currentFoucsInput = this.$telInput;
			this.$extensionInput = $(".dialPad .dialPad_inputBox .extensionNumber");
			this.$delIcon = $(".diaPad_title .displadInputdel");
			this.$footer = $('.calllistpanel .listBox .footer');
			this.$inputErrorTips = $('.dialPad .dialPadInput_Error');
			this.$inputErrorTips.hide();
			this.$intlTel = "";


		},
		clear: function () {
			//移除创建的节点以及子节点
			if (this.$dialPadDoc) {
				this.$dialPadDoc.find('*').off();
				this.$dialPadDoc.remove();
			}

			if (this.$callListDoc){
				this.$callListDoc.remove();
			}

			//释放创建的对象
			this.$target = null;
			this.$callback = null;
			this.$dialPadDoc = null;
			this.$callListDoc = null;
			this.$activity = false;

			this.$okbtn = null;
			this.$okbtnhide = null;
			this.$cancelbtn = null;
			this.$telInput = null;
			this.$extensionInput = null;
			this.$delIcon = null;
			this.$footer = null;
			if(this.clearCallback){
				this.clearCallback();
				this.clearCallback = null;
			}
			this.$inputErrorTips = null;
		},
		bindEvent: function () {
			$(window).resize(function () {
				_self.clear();
				e.stopPropagation();
			});
			uc.util.PageUtil.addClickBodyHideNode(this.$dialPadDoc[0], null, uc.util.EventUtil.hitch(this, 'clear'));
			//点击拨号列表底部，切换到拨号键盘
			var _self = this;
			this.$footer.on({
				click: function (e) {
					_self.$callListDoc.hide();
					_self.$dialPadDoc.show();
                    _self.$telInput.focus();
					e.stopPropagation();
				}
			});
			//点击返回按钮，切换到拨号列表
			this.$cancelbtn.on({
				click: function (e) {
					//clear();
					_self.$callListDoc.show();
					_self.$dialPadDoc.hide();
					e.stopPropagation();
				}
			});
			//点击确认按钮，关闭当前窗口，并且将获取到的输入框值传给外部
			this.$okbtn.on({
				click: function (e) {
					//去掉所有空格
					var _value = uc.util.StringUtil.trim(_self.$telInput.val(),'g');
					var ExtensionValue = uc.util.StringUtil.trim(_self.$extensionInput.val(),'g');
					var tempPhoneNumber =  ExtensionValue!="" ?_value+''+ExtensionValue:_value;
					var _isPhoneNumber = uc.util.StringUtil.IsTelephone(tempPhoneNumber);
					if(!_isPhoneNumber){
						_self.showInputErrorTips();
						_self.$telInput.focus();//输入框获得光标
						return _isPhoneNumber;
					}
					// 调用外部传递的回调函数
					if (_self.$callback) {
						var _telNumber = _self.$intlTel + ExtensionValue!="" ?_value+'-'+ExtensionValue:_value;
						var _audioType = uc.constants.Call.AudioType.PHONE;
						_self.$callback(_audioType, _telNumber, _self.$intlTel);
					}
					_self.clear();
					e.stopPropagation();
				}
			});
			//获取拨号键盘所有数字对象，绑定点击事件
			$('.dialPad .dialPad_main .phoneNumber').bind('click', function (e) {
				var input = _self.$currentFoucsInput;
				if(input == '86'){
					input = '';
				}
				input.val(input.val() + $(this).text());
				_self.checkOkBtnStatus();
				e.stopPropagation();
			});
			//输入验证，input只能输入数字和*#
			this.$telInput.css("ime-mode", "disabled");
			this.$telInput.bind("keypress", function (e) {
				var code = (e.keyCode ? e.keyCode : e.which);
				//0-9,*和#键
				return (code >= 48 && code <= 57) || code == 42 || code == 35;
			});

			this.$extensionInput.css("ime-mode", "disabled");
			this.$extensionInput.bind("keypress", function (e) {
				var code = (e.keyCode ? e.keyCode : e.which);
				//0-9,*和#键
				return (code >= 48 && code <= 57) || code == 42 || code == 35;
			});

			//设置确定按钮的状态
			this.$telInput.bind("keyup", function (e) {
				_self.checkOkBtnStatus();
				e.stopPropagation();
			});

			this.$delIcon.on('click', function () {
				_self.$telInput.val('');
				_self.$extensionInput.val('');
				_self.checkOkBtnStatus();
				$(this).hide();
			});

			this.$telInput.focus(function(){
				_self.$currentFoucsInput = $(this);
			});

			this.$extensionInput.focus(function(){
				_self.$currentFoucsInput = $(this);
			});

		},
		checkOkBtnStatus: function () {
			var inputval = this.$telInput.val();
			var extensionNumber = this.$extensionInput.val();
			if (inputval == "" && extensionNumber == "") {
				this.$okbtn.hide();
				this.$okbtnhide.show();
				this.$delIcon.hide();
			} else {
				this.$okbtn.show();
				this.$okbtnhide.hide();
				this.$delIcon.show();
			}
		},
		showintlTelInput: function () {
			var _self = this;
			var options = {
				data: uc_resource.IntlTelInput.intlTeldata,
				selectedDialCode: "中国",
				callback: function (_intlTel) {
					_self.$intlTel = _intlTel;
				}
			};
			_self.$intlTel = "";
			$('.diaPad_zoneSelect').intlTelInput(options);
		},
		getIsActivity: function () {
			return this.$activity;
		},
		updateCallList:function (_calllist) {
			if(!_calllist) return;
			for(var j=0;j<_calllist.length;j++){
				this.$calllist.push([_calllist[j],j == 0 && !this.$isUseVOIP?1:0]);
			}
			if (this.$calllist.length) {
				this.$calllist.unshift([uc.constants.Call.AudioType.VOIP,this.$isUseVOIP?1:0])
			}else{
				this.$calllist.unshift([uc.constants.Call.AudioType.VOIP,1])
			}
			//create calllist
			var tempDoc = $('.calllistpanel .listBox .calllist ul');
			var tempstr = "";
			for (var i = 0; i < this.$calllist.length; i++) {

				if (this.$calllist[i][0] == uc.constants.Call.AudioType.VOIP) {
					if (this.$calllist[i][1] == 0)
						tempstr+="<li class='netvoice'><span class='icon-call_voipVideo'></span><span class='tel'>"+uc_resource.Call.VoipCall+"</span><span class='icon-call_singleSelected hide'></span></li>";
					else
						tempstr+="<li class='netvoice'><span class='icon-call_voipVideo'></span><span class='tel'>"+uc_resource.Call.VoipCall+"</span><span class='icon-call_singleSelected'></span></li>";
				} else {
					if (this.$calllist[i][1] == 0)
						tempstr+="<li class='phonevoice'><span class='icon-call_reporterTell'></span><span class='tel'>" + this.$calllist[i][0] + "</span><span class='icon-call_singleSelected hide'></span></li>";
					else
						tempstr+="<li class='phonevoice'> <span class='icon-call_reporterTell'></span><span class='tel'>" + this.$calllist[i][0] + "</span><span class='icon-call_singleSelected'></span></li>";
				}
			}
			tempDoc.append(tempstr);

			//点击拨号列表，使用网络语音
			$('.calllistpanel .listBox .calllist .netvoice').on({
				click: function (e) {
					// 调用外部传递的回调函数
					if (_self.$callback) {
						var _telNumber = "";
						var _audioType = uc.constants.Call.AudioType.VOIP;
						_self.$callback(_audioType, _telNumber, "");
					}
					e.stopPropagation();
					_self.clear();
				}
			});
			var _self = this;
			//点击拨号列表，使用电话
			$('.calllistpanel .listBox .calllist .phonevoice .tel').on({
				click: function (e) {
					// 调用外部传递的回调函数
					if (_self.$callback) {
						var _telNumber = $(this).text();
						var _audioType = uc.constants.Call.AudioType.PHONE;
						var _dialCode = "";
						_self.$callback(_audioType, _telNumber, _dialCode);
					}
					e.stopPropagation();
					_self.clear();
				}
			});
		},
		showInputErrorTips:function(){
			if(this.$inputErrorTips){
				this.$inputErrorTips.slideToggle('fast').delay(1000).fadeOut("fast");
			}
		}
	})
})(uc);
