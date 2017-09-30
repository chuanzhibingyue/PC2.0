/**
 * Created by waouyang on 15/12/7.
 */
(function(uc){
    var _this = null;
    uc.modules.component.EmbeddedCallWindowToolBar = {
        init: function(parentNode){
            _this = this;
            this.nodeHtml = '<div class="embedded-call-tool-bar">' +
                                '<div class="call-btn-line">' +
                                    '<span class="text-chat-node">' +
                                        '<span class="call-toolbar-btn text-chat-btn icon-call_message" qtips="' + uc_resource.Call.TextChat + '"></span>' +
                                        '<span class="text-msg-num" style="display:none;"></span>' +
                                    '</span>' +
                                    '<span class="call-btn-node">' +
                                        '<span class="call-toolbar-btn mute icon-call_tellCalm disable" qtips="' + uc_resource.Call.Mute + '"></span>' +
                                        '<span class="call-toolbar-btn video icon-call_video disable" qtips="' + uc_resource.Call.OpenVideo + '"></span>' +
                                        '<span class="call-toolbar-btn desktop-share icon-call_shareScreen disable" qtips="' + uc_resource.Call.StartDesktopShare + '"></span>' +
                                        '<span class="call-toolbar-btn switch-call-channel voip icon-call_radioChange disable" qtips="' + uc_resource.Call.SwitchAudioChanel + '"></span>' +
                                        '<span class="call-toolbar-btn add icon-call_add_user disable" qtips="' + uc_resource.Call.Invite + '"></span>' +
                                    '</span>' +
                                    '<span class="full-screen-node hide">' +
                                        '<span class="call-toolbar-btn full-screen"></span>' +
                                    '</span>' +
                                '</div>' +
                                '<div class="end-call-line">' +
                                    '<span class="end-call-btn icon-call_stopTell"></span>' +
                                '</div>' +
                            '</div>';
            this.node = $(this.nodeHtml).appendTo(parentNode);
            this.textChatBtn = $('.text-chat-btn', this.node);
            this.textMsgNum = $('.text-msg-num', this.node);
            this.muteBtn = $('.mute', this.node);
            this.videoBtn = $('.video', this.node);
            this.desktopShareBtn = $('.desktop-share', this.node);
            this.switchCallChannelBtn = $('.switch-call-channel', this.node);
            this.addBtn = $('.add', this.node);
            this.fullScreenBtn = $('.full-screen', this.node);
            this.endCallBtn = $('.end-call-btn', this.node);
	        this.callToolbarbtn = $('.call-toolbar-btn', this.node);
			this.dialpadCalling = null;
	        this.dialPad = null;
            this.updateTextMsgNum(uc.modules.call.CallManager.msgCount);
	        this.$AVdetectionTips = null;
            this._bindEvent();
        },

        /**
         * init UI by EmbeddedCallWindow
         * @param callInfo
         */
        initUI: function(callInfo){
            this.callInfo = callInfo;
            this._showCallBtns();
        },

        updateCallInfo: function(callInfo){
            this.callInfo = callInfo;
            this._showCallBtns();
            this.updateTextMsgNum(uc.modules.call.CallManager.msgCount);
        },

        _showCallBtns: function(){
            switch (this.callInfo.callStatus){
                case uc.constants.Call.CallStatus.INVITING:
                    this.muteBtn.removeClass('disable');
                    this.switchCallChannelBtn.removeClass('disable');
                    this.addBtn.addClass('disable');
                    this.desktopShareBtn.addClass('disable');
                    this.endCallBtn.removeClass('disable');
                    break;
                case uc.constants.Call.CallStatus.CONNECTING:
                    this.muteBtn.addClass('disable');
                    this.switchCallChannelBtn.addClass('disable');
                    this.addBtn.addClass('disable');
                    this.desktopShareBtn.addClass('disable');
                    this.endCallBtn.removeClass('disable');
                    break;
                case uc.constants.Call.CallStatus.CONNECTED:
                    this.addBtn.removeClass('disable');
                    this.desktopShareBtn.removeClass('disable');
                    this.endCallBtn.removeClass('disable');
                    this.muteBtn.removeClass("disable");
                    this.switchCallChannelBtn.removeClass("disable");
                    break;
                case uc.constants.Call.CallStatus.DISCONNECTING:
                    this.muteBtn.addClass('disable');
                    this.switchCallChannelBtn.addClass('disable');
                    this.addBtn.addClass('disable');
                    this.desktopShareBtn.addClass('disable');
                    this.endCallBtn.addClass('disable');
                    break;
                case uc.constants.Call.CallStatus.DISCONNECTED:
                    break;
                default:
                    break;
            }

            if(this.callInfo.muteStatus){
                this.muteBtn.addClass("open").attr("qtips",uc_resource.Call.UnMute);
            }else{
                this.muteBtn.removeClass("open").attr("qtips",uc_resource.Call.Mute);
            }

            if(this.callInfo.audioType == uc.constants.Call.AudioType.PHONE){
                if(this.cancelSwitchPhone){
                    this.cancelSwitchPhone = false;
                    return;
                }
                _this.dialpadCallingclear();
            }

            this.setHandleDisableBtn(this.callInfo);
	        this.setScreenShareBtnStatus(this.callInfo);
        },
        setHandleDisableBtn:function(callInfo){
            switch(callInfo.handleBtn){
                case "endCall":
                    if(!this.endCallBtn.hasClass("disable")){
                        this.endCallBtn.addClass("disable");
                    }
                    break;
                case "muteCall":
                    if(!this.muteBtn.hasClass("disable")){
                        this.muteBtn.addClass("disable");
                    }
                    break;
                case 'success':
                    this.endCallBtn.removeClass("disable");
                    this.muteBtn.removeClass("disable");
                    break;
            }
        },
	    setScreenShareBtnStatus:function(callInfo){
		    if(callInfo.desktopShareStatus == uc.constants.Call.DesktopShareStatus.OPEN){
			    this.desktopShareBtn.addClass('open');
		    }else{
			    if(callInfo.desktopShareAcceptStatus == uc.constants.Call.DesktopShareStatus.OPEN) {
				    this.desktopShareBtn.addClass('disable');
				    this.desktopShareBtn.attr('qtips', uc_resource.Call.StopDesktopShare)
			    }else{
				    this.desktopShareBtn.removeClass('open');
				    if(callInfo.callStatus == uc.constants.Call.CallStatus.CONNECTED){
					    this.desktopShareBtn.removeClass('disable');
				    }
				    this.desktopShareBtn.attr('qtips', uc_resource.Call.StartDesktopShare)
			    }
		    }
	    },
        _bindEvent: function(){

            this.textChatBtn.click(function(){
                uc.modules.call.CallManager.switchEmbeddedCallWindowToEmbeddedCallBanner();
            });
            this.muteBtn.click(function(){
                _this._onMuteBtnClicked();
            });
            this.videoBtn.click(function(){
                _this._onVideoBtnClicked();
            });
            this.desktopShareBtn.click(function(){
                _this._onDesktopShareBtnClicked();
            });
            this.switchCallChannelBtn.click(function(e){
                if($(this).hasClass("disable")){
                    return;
                }
	            var _target =  this;
	            $(this).addClass('open');
				if(_this.dialPad && _this.dialPad.getIsActivity()) 
				{
					_this.dialPad.clear();
					return false;
				}
                var isVoip =  _this.callInfo.audioType == uc.constants.Call.AudioType.VOIP;
				var _options = {
					left:$(this).position().left + $(this).width()/2 -2,
					top:$(this).position().top-10,
					target:_this.node,
					isUseVOIP:isVoip,//true = 语音模式，flase，电话模式
					callBack:_this._onSwitchCallChannelBtnClicked,
					clearCallback:function(){
						uc.modules.call.CallManager.OnAudioServiceReady(1);
						$(_target).removeClass('open');
					}
				};
                _this.dialPad = new uc.modules.component.dialPad(_options);
	            uc.ICallService.getHistoryPhoneNum(function(arg){});
	            _this.clearAVdetectionTip();
				e.stopPropagation();
            });
            this.addBtn.click(function(){
                _this._onAddBtnClicked();
            });
            this.fullScreenBtn.click(function(){
                _this._onFullScreenBtnClicked();
            });
            this.endCallBtn.click(function(){
                _this._onEndCallBtnClicked();
            });

	        this.callToolbarbtn.mouseover(function(e){
		        if($(this).hasClass('open')){
			       $(this).removeClass('open');
			       $(this).addClass('openHover');
		        }
	        });

	        this.callToolbarbtn.mouseout(function(e){
		        if($(this).hasClass('openHover')){
			        $(this).removeClass('openHover');
			        $(this).addClass('open');
		        }
	        });

	        this.callToolbarbtn.mousedown(function(e){
		        if($(this).hasClass('openHover')){
			        $(this).removeClass('openHover');
			        $(this).addClass('openActive');
		        }
	        });

	        this.callToolbarbtn.mouseup(function(){
		        if($(this).hasClass('openActive')){
			        $(this).removeClass('openActive');
			        $(this).addClass('open');
		        }
	        });

	        $(window).resize(function (e) {
		        _this.updateAVdetectionTipPosition();
		        e.stopPropagation();
	        });

        },
        _onMuteBtnClicked: function(){
            if(this.muteBtn.hasClass("disable")){
                return;
            }
            uc.modules.call.CallManager.isMuteCall(this.callInfo);

        },
        _onVideoBtnClicked: function(){
            if(this.callInfo.videoStatus == uc.constants.Call.VideoStatus.CLOSE){
                this.videoBtn.addClass('open');
               // this.muteBtn.attr('qtips', uc_resource.Call.OpenVideo);
                this.callInfo.videoStatus = uc.constants.Call.VideoStatus.OPEN;
            }else {
                this.videoBtn.removeClass('open');
                //this.muteBtn.attr('qtips', uc_resource.Call.CloseVideo);
                this.callInfo.videoStatus = uc.constants.Call.VideoStatus.CLOSE;
            }
        },
        _onDesktopShareBtnClicked: function(){
	        if( this.desktopShareBtn.hasClass('disable')){
		        return false;
	        }
            if(this.callInfo.desktopShareStatus == uc.constants.Call.DesktopShareStatus.CLOSE){
                this.desktopShareBtn.addClass('open');
	            this.desktopShareBtn.attr('qtips', uc_resource.Call.StopDesktopShare);
                this.callInfo.desktopShareStatus = uc.constants.Call.DesktopShareStatus.OPEN;
	            uc.ISharingService.startDesktopShare(function(){});

            }else {
                this.desktopShareBtn.removeClass('open');
	            this.desktopShareBtn.attr('qtips', uc_resource.Call.StartDesktopShare);
                //this.callInfo.desktopShareStatus = uc.constants.Call.DesktopShareStatus.CLOSE;
	            uc.ISharingService.stopDesktopShare(function(){});
            }
        },
        _onSwitchCallChannelBtnClicked: function(_audioType,_telNumber,_dialCode){
			_this.dialPad = null;
            switch (_audioType){
                case uc.constants.Call.AudioType.VOIP:
                    _this._cancelSwitchPhone();
                    uc.ICallService.switchToVOIP(function(args){
                        if(args[0]==uc.ErrorCode.Success){
                          // _this.callInfo.audioType=uc.constants.Call.AudioType.VOIP;
                        }
                    });
                        break;
                case uc.constants.Call.AudioType.PHONE:
                    uc.ICallService.switchToPhone(_telNumber,function(args){
                        if(args[0]==uc.ErrorCode.Success){
							_this.dialpadCallingclear();
							_this.dialpadCalling= $('<div class="tellingPhone"></div>');
							_this.dialpadCalling.append('<p class="telling-number"><a>+'+(_dialCode == ""?86:"")+_telNumber+'</a></p><p class="telling-tip">'+uc_resource.Call.TellingPhone+'</p><p class="telling-cancel"><a>'+uc_resource.Common.Cancel+'</a></p>'); //
	                        _this.node.append(_this.dialpadCalling);
							var left = _this.switchCallChannelBtn.position().left -95;
							var top = _this.switchCallChannelBtn.position().top-256;
							$('.tellingPhone').css("position", "absolute");
							$('.tellingPhone').css("top", top);
							$('.tellingPhone').css("left",left);
							_this.dialpadCalling.find('.telling-cancel').bind("click",function(e){
                                _this._cancelSwitchPhone();
                                _this.dialpadCallingclear();
								e.stopPropagation();
							});
                        }
                    });
                    break;
            }
        },
        _onAddBtnClicked: function(){
	        if(this.addBtn.hasClass("disable")){
		        return;
	        }
            var _this=this;
             if(this.callInfo.chatType==uc.constants.Chat.ChatType.SingleChat){
                 uc.IUIService.openChooseCallContactsWindow({
                     key:"chooseCallContacts_" + new Date().getTime(),
                     userList:[this.callInfo.chatId]
                 }, function (args){
                     var userStatus=[];

                     for(var i = 0 ;i < args.memberList.length; i++){
                         if(args.memberList[i].userId != _this.callInfo.loginUserId && args.memberList[i].userId!=_this.callInfo.chatId){
                             userStatus.push(args.memberList[i].userId);
                         }
                     }
                     if(!userStatus.length) {
                         uc.IUIService.triggerCrossWindowEvent(uc.constants.UI.CrossWindowEventName.chooseContactsCreateCall, {callStatus: uc.constants.UI.chooseCallContacts.createcall_fail,failedType:uc.constants.Call.CallFailedType.SelectedUser});
                         return;
                     }
                     uc.modules.call.CallManager.changeCallUser = args.memberList;
                     uc.util.LogUtil.info("EmbeddedCallWindowToorBar","_onAddBtnClicked","data=",
                         {chatId: _this.callInfo.chatId,chatType:_this.callInfo.chatType, userList: userStatus});
	                 //废弃uc.ICallService.getUserCallStatus(userStatus,_this.callInfo.chatId,_this.callInfo.chatType,function(arg){
	                 var copyBack = '{"type":"'+_this.callInfo.chatType+'","userId":"'+_this.callInfo.chatId+'"}';
	                 var userIds = userStatus;
	                 uc.IContactService.getUserStatus(userIds,copyBack,function(arg){
                        var errorCode = arg[0];
                        if(errorCode != uc.ErrorCode.Success){
                           // This.showStartCallFailed();
                        }
                    });

                 })
             }else{
                 var userList=[];
                for(var userId in this.callInfo.userList){
                    userList.push(parseInt(userId));
                }
	             //如果当前列表已经有9个人，弹出不能在添加入会人员提示
	             if(userList.length >= uc.constants.Call.MaxJoinCount){
		             uc.modules.component.PromptMessageBox.open({title: uc_resource.WindowTitle.Notification, message: uc_resource.Call.ChooseMaxContactsTips, buttonMode: "ok", OkText: uc_resource.Common.OK},
			             function () {
				             this.close();
			             });
		             return;
	             }

                 uc.IUIService.openChooseCallContactsWindow({
                     key:"chooseCallContacts_" + new Date().getTime(),
                     userList:userList
                 }, function (args){
                     var userStatus=[];
                     var callUser=[];
                     for(var i = 0 ;i < args.memberList.length; i++){
                         for (var j = 0; j < userList.length; j++){
                             var flag=false;
                             if(args.memberList[i].userId == userList[j]){
                                 flag=true;
                                 break;
                             }

                         }
                         if(!flag){
                             userStatus.push(args.memberList[i].userId);
                             callUser.push(args.memberList[i])
                         }
                         //userList.push({userId:args.members[i].userId,userName:args.members[i].displayName});
                     }
                     if(!userStatus.length) {
                         uc.IUIService.triggerCrossWindowEvent(uc.constants.UI.CrossWindowEventName.chooseContactsCreateCall, {callStatus: uc.constants.UI.chooseCallContacts.createcall_fail,failedType:uc.constants.Call.CallFailedType.SelectedUser});
                         return;
                     }
                     uc.modules.call.CallManager.changeCallUser = callUser;
                     uc.util.LogUtil.info("EmbeddedCallWindowToorBar","_onAddBtnClicked","data=",
                         {chatId: _this.callInfo.chatId,chatType:_this.callInfo.chatType, userList: userStatus});
	                 //废弃
                     //uc.ICallService.getUserCallStatus(userStatus,_this.callInfo.chatId,_this.callInfo.chatType,function(arg){
	                 var copyBack = '{"type":"'+_this.callInfo.chatType+'","userId":"'+_this.callInfo.chatId+'"}';
	                 var userIds = userStatus;
	                 uc.IContactService.getUserStatus(userIds,copyBack,function(arg){
                         var errorCode = arg[0];
                         if(errorCode != uc.ErrorCode.Success){
                             // This.showStartCallFailed();
                         }
                     });
                 })
             }
        },
        _onFullScreenBtnClicked: function(){
            if(this.callInfo.fullScreenStatus == uc.constants.Call.FullScreenStatus.NORMAL){
                this.fullScreenBtn.addClass('open');
                this.callInfo.fullScreenStatus.fullScreenStatus = uc.constants.Call.FullScreenStatus.FULL_SCREEN;
            }else {
                this.fullScreenBtn.removeClass('open');
                this.callInfo.fullScreenStatus = uc.constants.Call.FullScreenStatus.NORMAL;
            }
        },
        _onEndCallBtnClicked: function(){
            if(this.endCallBtn.hasClass("disable")){
                return;
            }
            if(this.callInfo.callStatus == uc.constants.Call.CallStatus.INVITING){
                uc.modules.call.CallManager.cancelCall();
            }else if(this.callInfo.callStatus == uc.constants.Call.CallStatus.CONNECTING
                || this.callInfo.callStatus == uc.constants.Call.CallStatus.CONNECTED){
                uc.modules.call.CallManager.leaveCall();
            }

        },
        _cancelSwitchPhone:function(){
            var _this=this;
            uc.ICallService.hangupPhone(function(arg){
               if(arg[0] == uc.ErrorCode.Success){
                   _this.cancelSwitchPhone=true;
                   _this.dialpadCallingclear();
               }
            });

        },
        updateTextMsgNum: function(msgNum){
	        if(this.textMsgNum != null){
		        if(msgNum > 0){
			        this.textMsgNum.text(msgNum);
			        this.textMsgNum.show();
		        }else {
			        this.textMsgNum.hide();
		        }
	        }
        },

        show: function(){

        },
        hide: function(){

        },
        OnCallQualityChanged:function(quality){
            var internetTip,top,left,_this=this;
            if(this.callInfo.audioType==uc.constants.Call.AudioType.VOIP && quality>2){
                if(this.switchCallChannelBtn.find(".internetTip").length){
                    this.switchCallChannelBtn.find(".internetTip").text(uc_resource.Call.VoipToPhone);
                }else{
                    top=this.switchCallChannelBtn.offset().top+65+"px";
                    left=this.switchCallChannelBtn.offset().left-525+"px";
                    internetTip='<div class="internetTip" style="top:'+top+';left:'+left+'">'+uc_resource.Call.VoipToPhone+'</div>';
                    this.switchCallChannelBtn.append(internetTip);
                }
            }else if(this.callInfo.audioType==uc.constants.Call.AudioType.PHONE && quality<3){
                if(this.switchCallChannelBtn.find(".internetTip").length){
                    this.switchCallChannelBtn.find(".internetTip").text(uc_resource.Call.PhoneToVoip);
                }else{
                top=this.switchCallChannelBtn.offset().top+65+"px";
                left=this.switchCallChannelBtn.offset().left-525+"px";
                internetTip='<div class="internetTip" style="top:'+top+';left:'+left+'">'+uc_resource.Call.PhoneToVoip+'</div>';
                this.switchCallChannelBtn.append(internetTip);
                }
            }
			clearTimeout(tip);
            var tip=setTimeout(function(){
                _this.switchCallChannelBtn.find(".internetTip").remove();
                clearTimeout(tip);
            },2000);
        },
		dialpadCallingclear:function(){
			if(this.dialpadCalling)
			{
				this.dialpadCalling.find('.telling-cancel').unbind("click");
				this.dialpadCalling.remove();
				this.dialpadCalling = null;
			}
		},
        updateContact:function(){
            this.endCallBtn.removeClass("disable");
        },
	    IsShowCallNeedAVDetectionTip:function(isShow){
			if(isShow  &&  this.callInfo.audioType == uc.constants.Call.AudioType.VOIP){
				this.clearAVdetectionTip();
				this.$AVdetectionTips = $('<div class="AVdetectionTips"><p>'+uc_resource.Call.unInstallVedio+'<a class="reDetection" href="javascript:void(0)">'+uc_resource.Call.reDetection+'</a></p></div>');
				this.$AVdetectionTips.find('.reDetection').click(function(){
					var options = {
						width: 460,
						height: 675,
						refreshIfExist: false,
						centerScreen: true,
						modal: true,
						resizable: false,
						isTop:false,
						caption: uc.constants.UI.DragCaption.CloudSetting
					};
					uc.util.WindowUtil.open('page/AVdetection.html','AVdetectionWindow', options)
				});

				this.updateAVdetectionTipPosition();
			}else{
				this.clearAVdetectionTip();
			}
	    },
	    updateAVdetectionTipPosition:function(){
		  if(this.$AVdetectionTips){
			  var left = this.switchCallChannelBtn.position().left + this.switchCallChannelBtn.width()/2-145;
			  var top = this.switchCallChannelBtn.position().top-75;
			  this.$AVdetectionTips.css("position", "absolute");
			  this.$AVdetectionTips.css("top", top);
			  this.$AVdetectionTips.css("left", left);
			  this.node.append(this.$AVdetectionTips);
		  }
	    },
	    clearAVdetectionTip:function(){
		    if(this.$AVdetectionTips!=null){
			    this.$AVdetectionTips.find("*").off();
			    this.$AVdetectionTips.remove();
			    this.$AVdetectionTips = null;
		    }
	    }
    }
})(uc);