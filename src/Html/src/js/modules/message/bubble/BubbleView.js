/**
 * created by xue.bai_2 on 15/11/26
 */
(function(uc){
    var _this = null;
	uc.modules.bubble.BubbleView = {
		singleBubbleHideTimer: -1,
        singleBubbleHideDelay: 5000,
		bubbleListHideTimer: -1,
        bubbleListHideDelay: 1000,
        conversationInfoList: [],
        singleConversationInfo: null,

		init: function(){
            _this = this;

            this.initUI();
            this.bindUIEvents();

            this.bindCrossWindowEvent();

		},

        bindCrossWindowEvent: function(){
            //show single popup bubble message
            uc.IUIService.bindCrossWindowEvent(uc.constants.UI.CrossWindowEventName.ShowPopupBubbleMessage, function(args){
                uc.util.LogUtil.info('BubbleView', 'ShowPopupBubbleMessage', 'args = ' , args);
                _this.singleConversationInfo = args.conversationInfo;
                _this.setWindowPosition();
                _this._showPopupBubbleMessage();
            });
            //update bubble message list
            uc.IUIService.bindCrossWindowEvent(uc.constants.UI.CrossWindowEventName.UpdateBubbleMessageList, function(args){
                uc.util.LogUtil.info('BubbleView', 'UpdateBubbleMessageList', 'args = ' , args);
                _this.conversationInfoList = args.conversationInfoList;
                _this._updateBubbleMessageList();
            });
            //main window on top
            uc.IUIService.bindCrossWindowEvent(uc.constants.UI.CrossWindowEventName.OnMainWindowOnTop, function(){
                uc.util.LogUtil.info('BubbleView', 'OnMainWindowOnTop', '', null);
                _this.singleBubbleContainer.addClass("hide");
                _this.bubbleListContainer.addClass("hide");

                uc.util.WindowUtil.flashTray(false, 400);
            });
        },

        _showPopupBubbleMessage: function(){
            if(!this.singleConversationInfo){
                return;
            }
            //If mouse is hover in Bubble list, do not show pop up single bubble message
            if(this.hoverInBubbleList){
                return;
            }
            var singleLi = this.singleBubbleContainer.find("li");
            _this.updateBubble(singleLi, this.singleConversationInfo);
            
            this.singleBubbleContainer.removeClass("hide");
            this.bubbleListContainer.addClass('hide');
            
            clearTimeout(this.singleBubbleHideTimer);
            this.singleBubbleHideTimer = setTimeout(function(){
                _this.singleBubbleContainer.addClass("hide");
            }, this.singleBubbleHideDelay);
        },
        _updateBubbleMessageList: function(){
            this.bubbleConversationList.find('*').off();
            this.bubbleConversationList.empty();

            this.conversationInfoList.sort(this.compareFunction);

            var conversationInfo, bubbleItem;
            for(var i = 0, len = this.conversationInfoList.length; i < len; i++){
                conversationInfo = this.conversationInfoList[i];

                if(!conversationInfo){
                    continue;
                }

                bubbleItem = this._getBubbleTemplate(conversationInfo);
                this.bubbleConversationList.append(bubbleItem);
            }

            // Calculate the total number of new message in bubble list.
            this.calculateTotalMsgNum();

            // Calculate the position of bubble list.
            this.calculatePosition();
        },
        compareFunction: function (conversationInfo1, conversationInfo2) {
            return conversationInfo2.timestamp - conversationInfo1.timestamp;
        },

		initUI: function(){
            this.mainPanel = $('.bubble');
            this.singleBubbleContainer = $(".single-bubble", this.mainPanel);
            this.deleteIcon = $('.delete', this.singleBubbleContainer);
            this.bubbleListContainer = $('.bubble-list', this.mainPanel);
            this.bubbleConversationList = $('.conversation-list', this.bubbleListContainer);
            this.ignoreNode = $('.ignore');
            this.settingNode = $('.setting');
		},

        bindUIEvents: function(){
            this.singleBubbleContainer.hover(function(){
                clearTimeout(_this.singleBubbleHideTimer);
                _this.singleBubbleContainer.removeClass("hide");
                _this.bubbleListContainer.addClass('hide');
            }, function(){
                _this.singleBubbleHideTimer = setTimeout(function(){
                    _this.singleBubbleContainer.addClass("hide");
                }, _this.singleBubbleHideDelay);
            });

            this.bubbleListContainer.hover(function(){
                clearTimeout(_this.singleBubbleHideTimer);
                clearTimeout(_this.bubbleListHideTimer);

                _this.singleBubbleContainer.addClass('hide');
                _this.bubbleListContainer.removeClass("hide");
                _this.hoverInBubbleList = true;
            }, function(){
                _this.singleBubbleHideTimer = setTimeout(function(){
                    _this.bubbleListContainer.addClass("hide");
                    _this.hoverInBubbleList = false;
                }, _this.bubbleListHideDelay);
            });

			//点击单个消息气泡，打开主面板
            this.singleBubbleContainer.on('click', 'li', function(){
				_this.singleBubbleContainer.addClass("hide");
				_this.bubbleListContainer.addClass("hide");

                if(!_this.singleConversationInfo){
                    return;
                }
                var meetingData = {
                    meetingId: _this.singleConversationInfo.meetingId,
                    eventId: _this.singleConversationInfo.eventId,
                    startTime: _this.singleConversationInfo.meetingStartTime,
                    isCancelled: _this.singleConversationInfo.isMeetingCancelled
                };
				uc.IUIService.triggerCrossWindowEvent(uc.constants.UI.CrossWindowEventName.OpenChatWindowFromBubbleWindow,
                        {conversationId: _this.singleConversationInfo.conversationId,
                            conversationType: _this.singleConversationInfo.conversationType,
                            meetingData: meetingData});

                uc.util.WindowUtil.flashTray(false, 400);
			});
			
			//点击单个气泡上的删除按钮，清空并隐藏单个消息气泡
			this.deleteIcon.on('click', function(e){
				_this.singleBubbleContainer.addClass("hide");
				e.stopPropagation();
			});

			// 点击消息列表弹框上的单条消息，打开主面板，判断消息列表弹框上还有没有消息，没有，则停止托盘图标闪烁
			this.bubbleListContainer.on('click', 'li', function(){
				var conversationId = Number($(this).attr("conversationId"));
				var conversationType = Number($(this).attr("conversationType"));

				_this.bubbleListContainer.addClass("hide");

                var conversationInfo = _this._getClickedConversation(conversationId, conversationType);
                if(conversationInfo){
                    var meetingData = {
                        meetingId: conversationInfo.meetingId,
                        eventId: conversationInfo.eventId,
                        startTime: conversationInfo.meetingStartTime,
                        isCancelled: conversationInfo.isMeetingCancelled
                    };
                    uc.IUIService.triggerCrossWindowEvent(uc.constants.UI.CrossWindowEventName.OpenChatWindowFromBubbleWindow,
                        {conversationId: conversationInfo.conversationId, conversationType: conversationInfo.conversationType, meetingData: meetingData});
                }


                uc.util.WindowUtil.flashTray(false, 400);
			});
			
			this.ignoreNode.click(function(){
                _this.conversationInfoList = [];
				_this.calculatePosition();
				_this.bubbleListContainer.addClass("hide");

                uc.util.WindowUtil.flashTray(false, 400);
			});

			this.settingNode.click(function(){
				uc.IUIService.triggerCrossWindowEvent(uc.constants.UI.CrossWindowEventName.OpenSettingWindow, {});

				_this.singleBubbleContainer.addClass("hide");
				_this.bubbleListContainer.addClass("hide");
			});
		},

        _getClickedConversation: function(conversationId, conversationType){
            var conversationInfo;
            for(var i = 0, len = this.conversationInfoList.length; i < len; i++){
                conversationInfo = this.conversationInfoList[i];
                if(conversationInfo.conversationId == conversationId && conversationInfo.conversationType == conversationType){
                    return conversationInfo;
                }
            }
            return null;
        },

		_getBubbleTemplate: function(conversationInfo){
			var template = $('<li class="list-item conversation-item">' +
									'<span class="avatar-node">' + 
									'</span>' +
									'<span class="info title text-ellipsis"></span>' + 
									'<span class="msg-num"></span>' + 
							'</li>');
			template = _this.updateBubble(template, conversationInfo);

			return template;
		},
		calculateTotalMsgNum: function(){
            var conversationInfo;
            var msgTotalNum = 0;
            for(var i = 0, len = this.conversationInfoList.length; i < len; i++){
                conversationInfo = this.conversationInfoList[i];
                if(conversationInfo.conversationType == uc.constants.Conversation.ConversationType.MeetingMsg){
                    continue;
                }
                msgTotalNum = msgTotalNum + (conversationInfo.newMsgCount ? conversationInfo.newMsgCount : 0);
            }

			this.bubbleListContainer.find(".message-total-num").text('(' + msgTotalNum + ')');
		},
		calculatePosition: function(){
			this.bubbleListContainer.css("top", "200px");

			if(this.conversationInfoList.length > 0){
				this.ignoreNode.text(uc_resource.Common.Ignore);
				this.bubbleConversationList.parent().css("height","50px");

				if(this.conversationInfoList.length > 1){
					this.ignoreNode.text(uc_resource.Common.IgnoreAll);

					if(this.conversationInfoList.length > 5){
                        this.bubbleListContainer.css({"position":"absolute", "top":"0px", "left":"15px", "right":"2px"});
                        this.bubbleConversationList.parent().css({"position":"relative","bottom": "0px","height": "250px", "margin-right":"3px", "overflow-x":"hidden", "overflow-y":"scroll","background-color":"white"});
						this.settingNode.css("margin-right", "14px");
					}else{
                        this.bubbleListContainer.css("top", 200 - (this.conversationInfoList.length - 1) * 50 + "px");
                        this.bubbleConversationList.parent().css("height", this.conversationInfoList.length * 50 + "px");
					}
				}
			}
		},
		updateBubble: function(template, conversationInfo){
			template.attr("conversationId", conversationInfo.conversationId);
			template.attr("conversationType", conversationInfo.conversationType);

            var imageNode;
            switch (conversationInfo.conversationType){
                case uc.constants.Conversation.ConversationType.MeetingMsg:
                    imageNode = uc.util.AvatarUtil.composeMeetingAvatar(conversationInfo.meetingStartTime, conversationInfo.isGNet, conversationInfo.isMeetingCancelled);
                    break;
                case uc.constants.Conversation.ConversationType.ProjectChat:
                    imageNode = uc.util.AvatarUtil.composeProjectAvatar(conversationInfo.avatarUrlArray[0], uc.constants.UI.DefaultLocalUrl.Project_40);
                    break;
                case uc.constants.Conversation.ConversationType.GroupChat:
                    imageNode = uc.util.AvatarUtil.composeGroupAvatar(conversationInfo.avatarUrlArray, uc.constants.UI.DefaultLocalUrl.Avatar_40);
                    break;
                case uc.constants.Conversation.ConversationType.SingleChat:
                    imageNode = uc.util.AvatarUtil.composeContactAvatar(conversationInfo.avatarUrlArray[0], uc.constants.UI.DefaultLocalUrl.Avatar_40);
                    break;
                case uc.constants.Conversation.ConversationType.OpenApi:
                    imageNode = uc.util.AvatarUtil.composeLocalImageAvatar(uc.constants.UI.DefaultLocalUrl.Conversation.OpenApi, uc.constants.UI.DefaultLocalUrl.Conversation.OpenApi);
                    break;
                case uc.constants.Conversation.ConversationType.OrgMsg:
                    imageNode = uc.util.AvatarUtil.composeLocalImageAvatar(uc.constants.UI.DefaultLocalUrl.Conversation.Org, uc.constants.UI.DefaultLocalUrl.Conversation.Org);
                    break;
                case uc.constants.Conversation.ConversationType.CloudChat:
                    imageNode = uc.util.AvatarUtil.composeProjectAvatar(conversationInfo.avatarUrlArray, uc.constants.UI.DefaultLocalUrl.Cloud_Avatar_40);
                    break;
            }
            template.find(".avatar-node").empty();
            template.find(".avatar-node").append(imageNode);

            template.find(".title").removeClass('cancelled');
			template.find(".title").text(conversationInfo.title);
            if(conversationInfo.conversationType == uc.constants.Conversation.ConversationType.MeetingMsg){
                if(conversationInfo.isMeetingCancelled){
                    template.find(".title").addClass('cancelled');
                }
            }
            this.setNewMsgNum(template.find(".msg-num"), conversationInfo);

			return template;
		},

        setNewMsgNum: function(msgNumNode, conversationInfo){
            var msgNumClass = '';
            var newMsgCount = conversationInfo.newMsgCount;
            if(newMsgCount < 0){
                newMsgCount = 0;
            }
            switch (conversationInfo.conversationType){
                case uc.constants.Conversation.ConversationType.SingleChat:
                case uc.constants.Conversation.ConversationType.GroupChat:
                case uc.constants.Conversation.ConversationType.ProjectChat:
                case uc.constants.Conversation.ConversationType.WelcomeMsg:
                case uc.constants.Conversation.ConversationType.OpenApi:
                case uc.constants.Conversation.ConversationType.CloudChat:
                   if(!uc.modules.notice.NoticeManager.isNoticeConversation(conversationInfo.conversationId)){
                    if(newMsgCount == 0){
                        msgNumClass = 'hide';
                    }else if(newMsgCount < 10){
                        msgNumClass = 'single';
                        msgNumNode.text(newMsgCount);
                    }else if(newMsgCount < 100){
                        msgNumClass = 'ten';
                        msgNumNode.text(newMsgCount);
                    }else{
                        msgNumClass = 'hundred';
                        msgNumNode.text('99+');
                    }
                    break;
                  }
                case uc.constants.Conversation.ConversationType.MeetingMsg:
                    if(!newMsgCount){
                        msgNumClass = 'hide';
                    }else{
                        msgNumClass = 'meeting';
                    }
                    msgNumNode.text('');
                    break;
                default :
                    break;
            }
            msgNumNode.removeClass().addClass('msg-num').addClass(msgNumClass);
        },

		mouseOutIcon: function(){
            uc.util.LogUtil.info('BubbleView', 'mouseOutIcon', 'mouseOutIcon ' , null);
            this.bubbleListHideTimer = setTimeout(function(){
                if(_this.hoverInBubbleList){
                    return;
                }
				_this.bubbleListContainer.addClass("hide");
			}, this.bubbleListHideDelay);
		},
		mouseInTray: function(){
            uc.util.LogUtil.info('BubbleView', 'mouseInTray', 'mouseInTray' , null);
            clearTimeout(this.bubbleListHideTimer);

            if(this.conversationInfoList && this.conversationInfoList.length > 0){
                this.setWindowPosition();
                this.singleBubbleContainer.addClass('hide');
                this.bubbleListContainer.removeClass('hide');

                uc.util.WindowUtil.setOnTop(true);
            }
		},
        OnBasicContactInfoReceived: function(contacts){
            if(!this.singleConversationInfo || this.singleConversationInfo.conversationType != uc.constants.Conversation.ConversationType.SingleChat){
                return;
            }
            var contactInfo;
            for(var i = 0, len = contacts.length; i < len; i++){
                contactInfo = contacts[i];
                if(contactInfo.userId == this.singleConversationInfo.conversationId && !this.singleBubbleContainer.hasClass('hide')){
                    this.singleConversationInfo.title = contactInfo.displayName ? contactInfo.displayName : this.singleConversationInfo.conversationId;
                        this._showPopupBubbleMessage();
                    break;
                }
            }
        },
        OnGroupInfoReceived: function(groupId, mountId, group){
            if(!this.singleConversationInfo || (this.singleConversationInfo.conversationType != uc.constants.Conversation.ConversationType.ProjectChat
                && this.singleConversationInfo.conversationType != uc.constants.Conversation.ConversationType.GroupChat
                && this.singleConversationInfo.conversationType != uc.constants.Conversation.ConversationType.CloudChat)){
                return;
            }
            if(this.singleConversationInfo.conversationId == groupId){
                this.singleConversationInfo.title = group.name ? group.name : groupId;
                this.singleBubbleContainer.find(".title").text(this.singleConversationInfo.title);
            }
        },
        setWindowPosition: function(){
            var position = uc.IClientService.getTrayPos();
            var left = position.w > 300 ? position.x : position.x - (300 - position.w);
            var top = position.y - 340;
            uc.util.WindowUtil.setPosition(left, top);
        }
	};
	jQuery(document).ready(function(){
        uc.init();
	    uc.modules.bubble.BubbleView.init();
    });
})(uc);