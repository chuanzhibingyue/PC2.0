/**
 * Created by senlin.ouyang on 2015/7/21.
 */
(function(uc){
    var atList = [];
    uc.modules.chat.GroupChat = {
        init: function(groupId, chatType, siteId, paramObj){
            this.nodeHtml =    '<div class="line" />'
                +                   '<div class="title">'
                +                       '<div class="name" id="chatName"></div><div class="icon-super-group hide">'+ uc_resource.Common.SuperGroup +'</div><div class="icon-disturb hide"></div>'
                +                   '</div>'
                +                   '<div class="content">'
                +                       '<div class="chat selected"></div>'
                +                       '<div class="chat-title-btn">'
                    //+                       '<div class="chat-sel" id="chatsel"></div>'
                    //+                       '<div class="chat-file-sel" id="chatFilesel" style="display: none;"></div>'
                +                           '<div class="btn more"  qtips-w="40"  qtips="'+uc_resource.Tray.Setting+'" style="display: none"></div>'
                +                           '<div class="btn chat-file icon-chatfile" style="display: block;" qtips='+uc_resource.GroupChat.ChatFile+'></div>'
                +                           '<div class="btn call icon-chat_call_default" qtips="'+uc_resource.Menu.Call+'"  style="display: none;"></div>'
                +                           '<div class="btn calendar icon-createMetting" qtips="'+uc_resource.Menu.CreateMeeting+'"></div>'
                +                           '<div class="btn chat-external icon-external-chat hide" style="display: block;" qtips='+uc_resource.GroupChat.ExternalChat+'></div>'
                +        		            '<div class="btn cloud icon-cloudFile" qtips="'+uc_resource.Menu.Cloud+'"  style="display: none;"></div>'
                +                       '</div>'
                +                   '</div>'
                +                   '<div class="sign" id="chatSign" ></div>';
            this.headerNode = $('<div class="chat-header"></div>');
            this.headerNode.append(this.nodeHtml);
            this.paramObj = paramObj;
            this.headerNode.appendTo(uc.ui.Message.ContentPanelBody);
            this.chatName = $('#chatName', this.headerNode);
            this.superGroupIcon = $('.icon-super-group', this.headerNode);
            this.disturbIcon = $('.icon-disturb',this.headerNode);
            this.calendarAction = $('.calendar', this.headerNode);
            this.callAction = $('.call', this.headerNode);
            this.cloudAction = $('.cloud', this.headerNode);
            // this.updateChatName();
            //this.chatName.addClass('center');
            this.seqs = {};
            this.enable = true;

            this.groupId = groupId;
            this.chatType = chatType;
            this.node = $('<div class="group-chat message-box-container"></div>').appendTo(uc.ui.Message.ContentPanelBody);
            this.atNode = $('<div class="at-users"><span class="at-avatars"><img src="../images/avatar_default.png"></span><span class="at-tips"><span class="at-name"></span><span class="at-you">'+uc_resource.Chat.AtTips+'</span></span></div>').appendTo(this.node);
            this.msgBox = uc.modules.chat.ChatManager.msgBox;
            this.msgBox.init(this.node, this,chatType,groupId);
            this.historyDomStr = "";
            this.lastMsgSeq = 0;
            this.firstGetHistory = true;
            this.getMoreHistoryMsg();
            $('<div class="middle-line"></div>').appendTo(uc.ui.Message.ContentPanelBody);

            this.myUserInfo = uc.modules.chat.ChatManager.myUserInfo;
            siteId = siteId ? siteId : 1;
            if (this.myUserInfo != null) {//TODO should use group's siteId
                siteId = this.myUserInfo.siteId;
            }
            this.inputNode = $("<div class='chat-input'/>").appendTo(uc.ui.Message.ContentPanelBody);
            this.msgInputBox = uc.modules.chat.ChatManager.msgInputBox;
            this.msgInputBox.init(this.inputNode, {
                chatType: chatType,
                chatObjectId: this.groupId,
                siteId: siteId,
                resId: 100,
                moduleId: uc.constants.UI.ModuleId.Message
            });
            this.siteId=siteId;
            this.msgInputBox.enable(true);
            this.msgInputBox.disableAction("code");
            this.msgInputBox.disableAction("uploadMeetingDoc");
            this.msgBox.bindResize(uc.ui.Message.ContentPanelBody, this.msgInputBox);
            this.fileListNode = $('<div class="fileList" style="display: none"></div>').appendTo(uc.ui.Message.ContentPanelBody);
            this.fileList = uc.modules.chat.ChatManager.fileList;
            this.atNode = $('<div class="atMembers" style="display: none"></div>').appendTo(uc.ui.Message.ContentPanelBody);
            this.externalBox = '';
            this.groupInfo = uc.IGroupService.getGroupInfo(this.groupId);
            if (this.groupInfo && uc.IIMService.isShowExternalMessage(this.groupInfo.externalId) == 1 && this.groupInfo.externalId) {
                this.headerNode.find(".chat-external").removeClass("hide");

                this.externalChatNode = $('<div class="external-chat external-box-container"></div>').appendTo(uc.ui.Message.ContentPanelBody);
                this.externalBox = new uc.modules.chat.RtxBox({"chatType": this.chatType, "chatId": this.groupInfo.externalId});
                this.externalBox.init(this.externalChatNode);
            }

            if (this.groupInfo) {
                this._composeAtData(this.groupInfo.members);
            }
            //
            var This = this;
            $('.chat-file', this.headerNode).click(
                function(){
                    $(this).siblings('div').removeClass('selected')
                    $(this).siblings('.chat-external').removeClass('icon-external-chat-checked').addClass('icon-external-chat');
                    if ($('.chat-file', This.headerNode).hasClass("selected")) {
                        $('.chat-file', This.headerNode).removeClass("selected");
                        $('.chat-file', This.headerNode).removeClass("icon-chatfile-selected");
                        $('.chat-file', This.headerNode).addClass("icon-chatfile");
                        $('.filecomment').hide();
                        $('.fileList',uc.ui.Message.ContentPanelBody).hide();
                        $('.fileList',uc.ui.Message.ContentPanelBody).empty();
                    } else {
                        $('.chat-file', This.headerNode).addClass("selected");
                        $('.chat-file', This.headerNode).removeClass("icon-chatfile");
                        $('.chat-file', This.headerNode).addClass("icon-chatfile-selected");
                        //$('.chat', This.headerNode).removeClass("selected");
                        //$('.chat-sel', This.headerNode).hide();
                        //$('.chat-file-sel', This.headerNode).show();
                        //$('.group-chat', uc.ui.Message.ContentPanelBody).hide();
                        //$('.chat-input', uc.ui.Message.ContentPanelBody).hide();
                        $('.filecomment').hide();
                        $('.fileList',uc.ui.Message.ContentPanelBody).css({left:"860px", opacity:"0.3"});
                        $('.fileList', uc.ui.Message.ContentPanelBody).show();
                        $('.fileList',uc.ui.Message.ContentPanelBody).animate({"left": "460px", opacity:"1"}, 200, "linear", function() {});
                        This.fileList.init(This.groupId, This.fileListNode,This.chatType);
                    }
                });
            $('.chat-external', this.headerNode).click(
                function(){
                    if ($('.chat-external', This.headerNode).hasClass("selected")) {
                        $('.chat-external', This.headerNode).removeClass("selected");
                        $('.chat-external', This.headerNode).removeClass("icon-external-chat-checked");
                        $('.chat-external', This.headerNode).addClass("icon-external-chat");
                        $('.externalBox').hide();
                        //This.externalChatNode.empty();
                        This.externalBox.node.parent().hide();
                    } else {
                        $('.chat-external', This.headerNode).addClass("selected");
                        $('.chat-external', This.headerNode).removeClass("icon-external-chat");
                        $('.chat-external', This.headerNode).addClass("icon-external-chat-checked");
                        $('.externalBox').css({left:"860px", opacity:"0.3", "background-color":"#dddddd"});
                        $('.externalBox', uc.ui.Message.ContentPanelBody).show();
                        //$('.externalBox').animate({"left": "460px", opacity:"1"}, 200, "linear", function() {});
                        //This.externalBox.init(This.groupId, This.externalChatNode);
                        This.externalBox.node.parent().show();
                    }
                });

            this.atData = {};
            this.bindEvents();
            if (chatType == uc.constants.Chat.ChatType.CloudChat) {
                $('.chat-file', this.headerNode).hide();
                $('.chat', this.headerNode).hide();
                $('.chat-sel', this.headerNode).hide();
                $('.chat-file-sel', this.headerNode).hide();
                $('.name', this.headerNode).removeClass().addClass("cloudName");
                this.msgInputBox.disableAction("file");
                this.msgInputBox.disableAction("code");
                //this.msgInputBox.disableAction("history");
                this.msgInputBox.disableAction("cloud");
                this.msgInputBox.disableAction("uploadMeetingDoc");
                if(uc.modules.cloud.CloudManager.checkCloudIsLoginSuccess()==0){
                    this.msgInputBox.disableAction("cloud");
                    this.msgInputBox.disableAction("uploadMeetingDoc");
                    $('.cloud,.calendar', this.headerNode).hide();
                }else{
                    $('.cloud,.calendar', this.headerNode).show();
                }
            }else{
                $('.call', this.headerNode).show();
                if(uc.modules.cloud.CloudManager.checkCloudIsLoginSuccess()==0 ||
                    uc.modules.cloud.CloudManager.checkCloudTokenIsSuccess()!=0){
                    this.msgInputBox.disableAction("cloud");
                    this.msgInputBox.disableAction("uploadMeetingDoc");
                    $('.cloud', this.headerNode).hide();
                }
            }

            //add by xiege.
            if (uc.constants.Switch.OpenMultiVersion) {
	            if(!this.getCalendarVisible()){
		            this.headerNode.find(".calendar").addClass("hide");
	            }

	            if(!this.getCallVisible()){
		            this.headerNode.find(".call").addClass("hide");
	            }else{
		            this.headerNode.find(".call").removeClass("hide");
	            }
            }
        //    获取群聊，项目组，云盘组中@成员列表
            uc.IGroupService.getAtMessageList(this.groupId, 0, 100,function(args){
                if(args[0] != uc.ErrorCode.Success){
                    uc.util.LogUtil("GroupChat","getAtMessageList","getList Failed!",{groupId:this.groupId,errorCode:args[0]});
                }

            });
        },
        bindEvents: function() {
            var This = this;
            this.headerNode.on('click','.calendar',function() {
                $(this).siblings('div').removeClass('selected');
                $(this).siblings('.chat-external').removeClass('icon-external-chat-checked').addClass('icon-external-chat');
                var groupInfo = uc.IGroupService.getGroupInfo(This.groupId);
                var users=[];

                if (!groupInfo) {
                    return;
                }

                for(var i=0;i<groupInfo.members.length;i++){
                    users.push(groupInfo.members[i].userId);
                }

                var param = {
                    shareUserId: uc.constants.Meeting.AuthorizeMySelf,
                    isCreate: true,
                    users: users
                };

                uc.IUIService.createMeeting("ChatCalendarCreateMeeting", param);
            });
            this.headerNode.on('click','.name',function() {
                var groupInfo = uc.IGroupService.getGroupInfo(This.groupId);
                if(groupInfo && groupInfo.groupType==3){
                    uc.IUIEvent.OnCloudChatSet(Number(This.groupId));
                }else{
                    uc.IUIService.showPopupGroupProfile(Number(This.groupId));
                }
            });
            this.headerNode.on('click','.cloud',function() {
                $(this).siblings('div').removeClass('selected')
                $(this).siblings('.chat-external').removeClass('icon-external-chat-checked').addClass('icon-external-chat');
                var groupInfo = uc.IGroupService.getGroupInfo(This.groupId);
                if(groupInfo && groupInfo.mountId){
                    var cloudData=JSON.stringify({"mount_id":groupInfo.mountId});
                    uc.util.LogUtil.info('GroupChat', 'getGkMountId', 'cloudData = ' +cloudData, null);
                    uc.IUIService.changeModule(uc.constants.UI.ModuleId.Cloud,cloudData);
                }
            });

            this.atNode.on("click", ".atMember",
                function(e) {
                    if (This.atOption == null || This.msgInputBox == null) {
                        return;
                    }
                    var obj = $(".mName", $(this));
                    var name = obj.text();
                    var id = Number(obj.attr("rel"));
                    var draftId = This.groupId + '_' + This.chatType;
                    This.msgInputBox.setAtMember(This.atOption.id, id, name, draftId);
                    This.atOption = null;
                    This.atNode.hide();
                    e.preventDefault();
                }
            );

            uc.IUIService.bindCrossWindowEvent("CloudLoginSuccess", function (data) {
                This.OnCloudLoginReceived();
            });
            $(".at-users").click(
                function(){
                    var self = $(this);
                    var atMsg = [];
                    atMsg.push(parseInt(atList[0].atMsgSeq));
                    uc.modules.historymsg.HistoryManager.AtQueryControl(self,atList[0].atMsgSeq);
                    uc.IGroupService.deleteAtMessage(This.chatType, This.groupId, atMsg, function(args){
                        uc.util.LogUtil.info("GroupChat","deleteAtMessage","errorCode"+args[0],{conversationType:This.chatType, GroupId:This.groupId, Msg:atMsg});
                    });
                });
            this.headerNode.on('click','.call',function() {
                if(uc.modules.call.CallManager.inProgressCallInfo.isInProgress || !uc.modules.pbx.PbxManager.DialIsClick){
                    uc.IUIService.triggerCrossWindowEvent(uc.constants.UI.CrossWindowEventName.CloseGroupProfile,{});
                    uc.modules.component.PromptMessageBox.open({
                        title: uc_resource.WindowTitle.Notification,
                        message: uc_resource.Call.HasCallNotCall,
                        buttonMode: "ok",
                        OkText: uc_resource.Common.OK
                    }, function () {
                        this.close();
                    });
                    return;
                }
                //uc.ICallService.getUserCallStatus([This.myUserInfo.userId], This.myUserInfo.userId, uc.constants.Chat.ChatType.SingleChat,function(arg){
                var copyBack = '{"type":"'+uc.constants.Chat.ChatType.SingleChat+'","userId":"'+This.myUserInfo.userId+'"}';
                var userIds = [This.myUserInfo.userId];
                uc.IContactService.getUserStatus(userIds,copyBack,function(arg){
                    var errorCode = arg[0];
                    if(errorCode != uc.ErrorCode.Success){
                        This.showStartCallFailed();
                    }
                });
            });
        },
        setGroupCall:function(){
            var _this=this;
            var groupInfo =  uc.IGroupService.getGroupInfo(this.groupId);
            uc.util.LogUtil.info('setGroupCall', 'getGroupInfo', 'groupInfo=', groupInfo);
            var groupCallId=(groupInfo.statusInfo !="" && groupInfo.statusInfo !="[]") ? $.parseJSON(groupInfo.statusInfo).tempConferenceId:'';
            uc.IUIService.triggerCrossWindowEvent(uc.constants.UI.CrossWindowEventName.CloseGroupProfile,{});
            if(groupInfo && groupCallId){
                uc.modules.component.PromptMessageBox.open({
                    title: uc_resource.WindowTitle.Notification,
                    message:uc_resource.Call.GroupHasCall,
                    buttonMode: "both",
                    OkText: uc_resource.Common.OK,
                    CancelText: uc_resource.Common.Cancel
                }, function (result) {
                    if(!result){
                        var user={};
                        for(var i=0;i<groupInfo.members.length;i++){
                            user[groupInfo.members[i].userId]={};
                            user[groupInfo.members[i].userId].userInfo={};
                            user[groupInfo.members[i].userId].userInfo.userId=groupInfo.members[i].userId;
                            user[groupInfo.members[i].userId].callStatus=uc.constants.Call.CallStatus.INVITING;
                        }
                        uc.modules.call.CallManager.startCall({
                            loginUserId:_this.myUserInfo.userId,
                            chatId: _this.groupId,
                            siteId:_this.siteId,
                            callStatus: uc.constants.Call.CallStatus.CONNECTING,
                            chatType: _this.chatType,
                            videoStatus: uc.constants.Call.VideoStatus.CLOSE,
                            callMeetingId: groupCallId+'',
                            isInProgress: true
                        },uc.constants.Call.CallWindowType.EMBEDDED_CALL_WINDOW,false,true);
                    }
                    this.close();
                });
            }else{
                uc.IUIService.openChooseCallContactsWindow({
                    key:"chooseCallContacts_" + new Date().getTime(),
                    groupId:_this.groupId
                }, function (args) {
                    uc.util.LogUtil.info('GroupChat', 'ChooseCallContactcallback', 'data', args);
                    var groupId = 0,chatType = uc.constants.Chat.ChatType.GroupChat;
                    if(args.groupId){
                        groupId = args.groupId;
                        chatType = _this.chatType;
                    }
                    _this.callMemberList = args.memberList;
                    var userList=[];
                    userList.push(_this.myUserInfo.userId);
                    for(var i=0;i<args.memberList.length;i++){
                        userList.push(args.memberList[i].userId);
                    }
                    //  _this.startCall(groupId,chatType,uc.constants.Call.CallWindowType.EMBEDDED_CALL_WINDOW);
                    uc.util.LogUtil.info("GroupChat","setGroupCall","data=",
                        {chatId: groupId,chatType:chatType, userList: userList});
                    //废弃uc.ICallService.getUserCallStatus(userList,groupId,chatType,function(arg){
                    var copyBack = '{"type":"'+chatType+'","userId":"'+groupId+'"}';
                    var userIds = userList;
                    uc.IContactService.getUserStatus(userIds,copyBack,function(arg){
                        var errorCode = arg[0];
                        if(errorCode != uc.ErrorCode.Success){
                            _this.showStartCallFailed();
                        }
                    });
                });
            }
        },
        showStartCallFailed: function(){
            uc.modules.component.PromptMessageBox.open({
                title: uc_resource.WindowTitle.Notification,
                message: uc_resource.Message.StartCallFailed,
                buttonMode: "ok",
                OkText: uc_resource.Common.OK
            }, function () {
                this.close();
            });
        },
        _enableCall: function(){
            this.headerNode.find('.call').show();
        },
        _disableCall: function(){
            this.headerNode.find('.call').hide();
        },
        startCall:function(groupId,chatType,callWindowType){
            var user={};
            var userList=this.callMemberList;
            for(var i=0;i<userList.length;i++){
                user[userList[i].userId]={};
                user[userList[i].userId].userInfo={};
                user[userList[i].userId].userInfo=userList[i];
                user[userList[i].userId].callStatus=uc.constants.Call.CallStatus.INVITING;

            }
            user[this.myUserInfo.userId]={};
            user[this.myUserInfo.userId].userInfo={};
            user[this.myUserInfo.userId].userInfo=this.myUserInfo;
            user[this.myUserInfo.userId].callStatus=uc.constants.Call.CallStatus.INVITING;

            if(groupId){
                uc.modules.call.CallManager.startCall({
                    loginUserId:this.myUserInfo.userId,
                    chatId: groupId,
                    siteId:this.groupId == groupId?this.siteId:0,
                    hostId:this.myUserInfo.userId,
                    callStatus: uc.constants.Call.CallStatus.CONNECTING,
                    chatType: chatType,
                    videoStatus: uc.constants.Call.VideoStatus.CLOSE,
                    userList:user,
                    callMeetingId: 0,
                    isInProgress: true
                },callWindowType,true,false);
            }else{
                uc.modules.call.CallManager.initGroupCall({
                    loginUserId:this.myUserInfo.userId,
                    chatId: 0,
                    siteId:0,
                    hostId:this.myUserInfo.userId,
                    callStatus: uc.constants.Call.CallStatus.CONNECTING,
                    chatType: uc.constants.Chat.ChatType.GroupChat,
                    videoStatus: uc.constants.Call.VideoStatus.CLOSE,
                    userList:user,
                    callMeetingId: 0,
                    isInProgress: true
                });
            }


        },

        startCallFromProfile:function(){
            this.headerNode.find(".call").trigger("click");
        },
        enableInput: function(bEnable){
            this.enable = bEnable;
            this.msgInputBox.enable(bEnable);
            if (!bEnable) {
                $('.chat', this.headerNode).hide();
                $('.chat-file', this.headerNode).hide();
                $('.chat-sel', this.headerNode).hide();
                $('.chat-file-sel', this.headerNode).hide();
                $('.more', this.headerNode).hide();
                $('.add', this.headerNode).hide();
                $('.call', this.headerNode).hide();
                $('.calendar', this.headerNode).hide();
                $('.cloud', this.headerNode).hide();
                this.msgBox.unBindResize(uc.ui.Message.ContentPanelBody, this.msgInputBox);

            } else {
                $('.calendar', this.headerNode).show();
                $('.more', this.headerNode).show();

                if (this.chatType == uc.constants.Chat.ChatType.GroupChat) {
                    $('.chat-file', this.headerNode).show();
                    $('.call', this.headerNode).show();
                }else if (this.chatType == uc.constants.Chat.ChatType.ProjectChat) {
                    $('.chat', this.headerNode).show();
                    $('.chat-file', this.headerNode).show();
                    $('.chat-sel', this.headerNode).show();
                } else if (this.chatType == uc.constants.Chat.ChatType.CloudChat) {
                    $('.cloud,.calendar', this.headerNode).show();
                    $('.more', this.headerNode).hide();
                }
                this.msgBox.bindResize(uc.ui.Message.ContentPanelBody, this.msgInputBox);
            }
        },
        updateChatName: function(groupInfo) {
            var name = uc.util.StringUtil.escapeHtml(groupInfo.name);
            this.chatName.html(name).attr({"qtips": name,"qtips-show-by": "mouse"});
            var isSuperGroup = groupInfo.memberType ? 1 : 0;

            if (groupInfo.isUpdate){
                return;
            }
            if (isSuperGroup) {
                this.superGroupIcon.removeClass('hide');
                this.showSuperGroupAction();
            } else {
                this.superGroupIcon.addClass('hide');
            }
            this.showDisturbIcon(groupInfo);

        },
        showDisturbIcon:function(groupInfo){
            var isDisturb = groupInfo.disturbState ? 1:0;
            console.log("groupchat",isDisturb);
            if (isDisturb) {
                this.disturbIcon.removeClass('hide');
            } else {
                this.disturbIcon.addClass('hide');
            }
        },
        showSuperGroupAction: function () {
            this.calendarAction.addClass('hide');
            this.callAction.addClass('hide');
            //this.cloudAction.addClass('hide');
            this.msgInputBox.disableAction("image");
            this.msgInputBox.disableAction("file");
            this.msgInputBox.disableAction("code");
            this.msgInputBox.disableAction("screen-shot");
            this.msgInputBox.disableAction("screen-shot-setting");
        },
        updateGroupMember: function(members){
            var This = this;
            setTimeout(function () {
                This._composeAtData(members);
            }, 100);
            //$("#chatSign", This.headerNode).empty();
            //for (var j = 0; j < members.length; j++) {
            //    if (j == 0) {
            //        $("#chatSign", This.headerNode).append(uc_resource.GroupChat.Contain);
            //    }  else if (j > 3) {
            //        $("#chatSign", This.headerNode).append(" ...");
            //        break;
            //    } else {
            //        $("#chatSign", This.headerNode).append("、");
            //    }
            //    var uid = members[j].userId;
            //    var userName = "";
            //    var contactInfo = uc.modules.cache.BasicContactInfoCache.getBasicContactInfo(uid);
            //    if (contactInfo) {
            //        userName = contactInfo.displayName;
            //    }
            //    $("#chatSign", This.headerNode).append("<span rel='" + uid + "'>" + userName + "</span>");
            //}
        },
        _composeAtData: function(members){
            var id, userIdList = [];
            this.atData = {};
            for (var i = 0, len = members.length; i < len; i++) {
                id = members[i].userId;
                if (id == this.myUserInfo.userId) {
                    continue;
                }
                userIdList.push(id);
                this.atData['' + id] = {
                    userId: id,
                    displayName: id,
                    pinyin: '',
                    localAvatar: uc.constants.UI.DefaultLocalUrl.Avatar_40
                }
            }
            this.groupMemberIdList = userIdList;
            //var contactInfoArray = uc.modules.cache.BasicContactInfoCache.getMultipleBasicContactInfo(userIdList);
            //if(contactInfoArray.length){
            //    var contactInfo;
            //    for(var j = 0, contactLen = contactInfoArray.length; j < contactLen; j++){
            //        contactInfo = contactInfoArray[j];
            //        this.updateContact(contactInfo);
            //    }
            //}
        },
        _composeAtList: function(){
            var contactInfoArray = uc.modules.cache.BasicContactInfoCache.getMultipleBasicContactInfo(this.groupMemberIdList);
            if(contactInfoArray.length){
                var contactInfo;
                for(var j = 0, contactLen = contactInfoArray.length; j < contactLen; j++){
                    contactInfo = contactInfoArray[j];
                    this.updateContact(contactInfo, false);
                }
            }
            this.atNode.empty();
            var atData, userNodeList = [], userNode, userAvatar, isSuperGroup, userLogoColor, bgColor;
            if( this.groupInfo && this.groupInfo.memberType ){
                isSuperGroup = this.groupInfo.memberType;
            }
            for(var userId in this.atData){
                atData = this.atData[userId], userLogoColor = uc.modules.chat.ChatManager.superGroupMemberBgColor[userId];
                if (isSuperGroup) {
                    if (userLogoColor) {
                        bgColor = userLogoColor;
                    } else {
                        var randomNumber = parseInt(Math.random() * 10);
                        bgColor = randomNumber > 5 ? this.msgBox.superGroupBgColor[5] : this.msgBox.superGroupBgColor[randomNumber];
                        uc.modules.chat.ChatManager.superGroupMemberBgColor[userId] = bgColor;
                    }

                    var displayName = atData.displayName.toString();
                    userAvatar = '<div class="mHeader" style="background-color: '+ bgColor +'">'+ displayName.substring(0, 1) +'</div>';
                } else {
                    userAvatar = "<img src='" + atData.localAvatar + "' class='mHeader' rel=" + atData.userId + "/>";
                }
                userNode =  "<div class='atMember' data-id='" + userId + "'>" + userAvatar +
                    "<span class='mName' rel='" + atData.userId + "' pinyin='" + atData.pinyin + "'>" + atData.displayName + "</span>" +
                    "</div>";
                userNodeList.push(userNode);
            }
            this.atNode.append(userNodeList.join(''));

        },

        _updateAtMember: function(atData){
            var atMemberNode = $('.atMember[data-id="' + atData.userId + '"]');
            //如果是超级群，只显示姓氏
            var isSuperGroup = this.groupInfo.memberType;
            if (isSuperGroup) {
                atMemberNode.find('.mHeader').text(atData.displayName.substring(0, 1));
                atMemberNode.find('.mName').text(atData.displayName);
                return;
            }
            var nodeHtml = "<img src='" + atData.localAvatar + "' class='mHeader' rel=" + atData.userId + "/>" +
                "<span class='mName' rel='" + atData.userId + "' pinyin='" + atData.pinyin + "'>" + atData.displayName + "</span>";
            atMemberNode.html(nodeHtml);
        },

        updateContact: function(contact, refreshAtList) {
            if(contact && this.atData){
                var atData = this.atData['' + contact.userId];
                if(atData){
                    if(contact.displayName){
                        atData.displayName = contact.displayName;
                        //$("#chatSign", this.headerNode).find("span[rel=" + contact.userId + "]").text(contact.displayName);
                    }
                    if(contact.pinyin){
                        atData.pinyin = contact.pinyin;
                    }
                    if(contact.localAvatar){
                        atData.localAvatar = contact.localAvatar;
                    }

                    if(refreshAtList && this.atNode.is(':visible')){
                        this._updateAtMember(atData);
                    }
                }
            }
        },

        updateUserAvatar: function(userId, localAvatarUrl){
            var atData = this.atData ? this.atData['' + userId] : null;
            if(atData){
                atData.localAvatar = localAvatarUrl;
                if(this.atNode.is(':visible')){
                    this._updateAtMember(atData);
                }
            }
        },

        onAt: function(x, y, id) {
            if(!this.groupMemberIdList || this.groupMemberIdList.length == 0){
                return;
            }
            if (id == null) {
                this.atNode.hide();
                this.atOption = null;
                return;
            }
            this._composeAtList();
            $('.atMember', this.atNode).removeClass("sel");
            $('.atMember', this.atNode).show();
            var w = this.atNode.width();
            var h = this.atNode.height();
            if ( (x + w + 5) >= document.body.clientWidth) {
                x =  document.body.clientWidth - w - 5;
            }
            this.atNode.css({position: "fixed",'top':y - h,'left':x, 'z-index':2});
            $('.atMember', this.atNode).removeClass("sel");
            $('.atMember', this.atNode).show();
            this.atNode.show();
            this.atOption = {
                id : id,
                x : x,
                y : y
            }
        },
        onAtSearchDirect: function(up) {
            if (this.atOption == null || $('.atMember', this.atNode).length <= 0) {
                return;
            }
            var obj = $(".atMember.sel", this.atNode);
            if (obj.length <= 0) {
                var This = this;
                $('.atMember', this.atNode).each(
                    function() {
                        if (!$(this).is(":hidden") && ($(this).position().top + 50) >=  This.atNode.height()){
                            obj = $(this);
                            return false;
                        }
                    }
                );
            } else {
                if (up) {
                    while(obj != null && obj.length > 0){
                        obj = obj.prev();
                        if (!obj.is(":hidden")) {
                            break;
                        }
                    }
                } else {
                    while(obj != null && obj.length > 0){
                        obj = obj.next();
                        if (!obj.is(":hidden")) {
                            break;
                        }
                    }
                }
            }
            if (obj != null && obj.length > 0) {
                $('.atMember', this.atNode).removeClass("sel");
                obj.addClass("sel");
                var h =  $('.atMember', this.atNode).height() + 14;
                var showIndex = 0;
                $('.atMember', this.atNode).each(
                    function() {
                        if (obj.index() == $(this).index()) {
                            return false;
                        }
                        if (!$(this).is(":hidden")){
                            showIndex++;
                        }
                    }
                );
                var y = h*(showIndex + 1);
                y = y - this.atNode.height();
                this.atNode.scrollTop(y);
            }
        },
        onAtSearchEnter: function() {
            if (this.atOption == null || this.msgInputBox == null || $('.atMember', this.atNode).length <= 0) {
                this.atOption = null;
                this.atNode.hide();
                return false;
            }
            var selObj = $(".atMember.sel");
            if (selObj.length <= 0) {
                this.atOption = null;
                this.atNode.hide();
                return false;
            }
            if ($(selObj[0]).is(":hidden")){
                this.atOption = null;
                this.atNode.hide();
                return false;
            }
            var obj = $(".mName", selObj);
            var name = obj.text();
            var id = Number(obj.attr("rel"));
            var draftId = this.groupId + '_' + this.chatType;
            this.msgInputBox.setAtMember(this.atOption.id, id, name, draftId);
            this.atOption = null;
            this.atNode.hide();
            return true;
        },
        onAtsearch: function(id, text){
            if (this.atOption == null || $('.atMember', this.atNode).length <= 0) {
                return false;
            }
            if (text == "@up") {
                this.onAtSearchDirect(true);
                return false;
            } else if (text == "@down") {
                this.onAtSearchDirect(false);
                return false;
            }else if(text == "@enter") {
                return this.onAtSearchEnter();
            }
            this.atNode.show();
            $('.atMember', this.atNode).show();
            $('.atMember', this.atNode).removeClass("sel");
            if (text.length > 0) {
                $('.atMember', this.atNode).each(
                    function() {
                        var name = $(".mName", $(this)).text();
                        var pinyin = $('.mName', $(this)).attr('pinyin');
                        if(name.indexOf(text) < 0 && pinyin.toLowerCase().indexOf(text.toLowerCase()) < 0){
                            $(this).hide();
                        }
                    }
                );
            }
            var b = false;
            $('.atMember', this.atNode).each(
                function() {
                    if(!$(this).is(":hidden")) {
                        b = true;
                    }
                });
            if (!b) {
                this.atNode.hide();
                return false;
            }
            var h = this.atOption.y - this.atNode.height();
            this.atNode.css({position: "fixed",'top': h});

            return false;
        },
        onBodyClick: function(e){
            if (! ($(e.target).closest(".fileList").length ||
                $(e.target).closest(".filecomment").length) &&
                ! $(e.target).closest(".chat-file").length) {
                $(".chat-file").removeClass("selected");
                $('.chat-file').removeClass("icon-chatfile-selected");
                $('.chat-file').addClass("icon-chatfile");
                $(".fileList",uc.ui.Message.ContentPanelBody).hide();

                $(".fileList",uc.ui.Message.ContentPanelBody).empty();
            }
            if (this.externalBox && !$(e.target).closest(".external-chat").length && !$(e.target).hasClass('chat-external')) {
                $('.chat-external').removeClass('icon-external-chat-checked selected').addClass('icon-external-chat');
                this.externalBox.node.parent().hide();
            }

            if (this.atOption == null || this.msgInputBox == null) {
                return;
            }
            this.atOption = null;
            this.atNode.hide();
            this.msgInputBox.setAtMember(null);
        },

        getMoreHistoryMsg: function() {
            this.historyDomStr = "";
            var msgBox = this.msgBox;
            msgBox.onMoreMsgLoading();
            uc.util.LogUtil.info("GroupChat", "getMoreHistoryMsg","data=",{chatType:this.chatType,groupId:this.groupId, count:10, page:0, seq:Number(this.lastMsgSeq)});
            uc.IGroupService.getChatHistory(uc.constants.Message.MessageChannel.ChatHistory, this.chatType, this.groupId, uc.constants.Message.OnceGetHistoryCount, 0, Number(this.lastMsgSeq), function(errorCode){
                if (errorCode != 0) {
                    uc.util.LogUtil.info("GroupChat","getMoreHistoryMsg", "data=",{groupId:this.groupId, errorCode:errorCode});
                    msgBox.onMoreMsgLoadEnd();
                }
            });
        },

        onGroupChatHistoryReceived: function(errorCode, conversationType, groupId, messages, msgCount, minTimestamp){
            var historyLog = {
                errorCode: errorCode,
                conversationType: conversationType,
                groupId: groupId,
                messages: messages,
                msgCount: msgCount,
                minTimestamp: minTimestamp
            }
            uc.util.LogUtil.info("GroupChat","onGroupChatHistoryReceived","args", historyLog);

            if (this.firstGetHistory) {
                uc.modules.chat.ChatManager.appendUploadMessages();
            }

            if(this.paramObj && this.paramObj.filePath){
                this.msgInputBox.rightClickToSendFile(this.paramObj.filePath);
            }

            if (errorCode == 0) {
                this.lastMsgSeq = minTimestamp;
            }
            this.msgBox.onMoreMsgLoadEnd();
            if (msgCount < uc.constants.Message.OnceGetHistoryCount && errorCode == 0) {
                this.msgBox.onLoadAllMsgEnd();
            }
            if (this.historyDomStr == "") {
                this.firstGetHistory = false;
                return;
            }
            uc.util.LogUtil.info("GroupChat","onGroupChatHistoryReceived","data=",{errorCode:errorCode,groupId:groupId});
            this.msgBox.prependMessage(this.historyDomStr);
            if (this.firstGetHistory) {
                this.msgBox.scrollToEnd();
            }
            this.firstGetHistory = false;
            this.historyDomStr = "";
        },

        procMessageDomStr: function(channel, domStr, msgId, msgSeq) {
            uc.util.LogUtil.info("GroupChat","procMessageDomStr","msgSeq=",{msgSeq:msgSeq,channel:channel});
            if (this.seqs[msgSeq] != null){
                uc.util.LogUtil.info("GroupChat","procMessageDomStr","the msg seq has been existed!=",{msgSeq:msgSeq});
                return;
            }
            this.seqs[msgSeq] = true;

            //this.lastMsgSeq = msgSeq;
            if (this.msgBox.isMsgIdExist(msgId)) {
                uc.util.LogUtil.info("GroupChat","procMessageDomStr","the msg seq has been existed!=",{msgId:msgId});
                return;
            }
            if (uc.constants.Message.MessageChannel.ChatHistory == channel) {
                this.historyDomStr = domStr + this.historyDomStr;
                return;
            }
            this.msgBox.appendMessage(domStr);
        },
        //setConversationAtCounts:function(conversationType,to){
        //    if(atList&&atList.length == 0){
        //        var conversationInfo = uc.modules.conversation.ConversationManager.getConversation(to.userId, conversationType);
        //        conversationInfo.atCounts = 0;
        //        uc.modules.conversation.ConversationManager.updateConversation(to.userId, conversationType, conversationInfo)
        //    }
        //},
        onGroupTextMessageReceived: function(conversationType, channel, sendState, self, from, to, msgId, msgSeq, textType, textContent, msgTime, textFormat, atUsers){
            var domStr = this.msgBox.getTextMsgDomStr(sendState, self, from, msgId, msgSeq, textType, textContent, msgTime, msgTime, textFormat);
            this.procMessageDomStr(channel, domStr, msgId, msgSeq);
            //this.setConversationAtCounts(conversationType,to);
            if(atUsers && atUsers.length){
                    var userInfo = uc.IContactService.getCurrentUserInfo();
                    var currentUserId = userInfo.userId;
                    var atCounts = false;
                    var counts = 0;
                    for(var i = 0; i < atUsers.length; i++){
                        if(atUsers[i].userId == currentUserId) {
                            atCounts = true;
                            counts++;
                        }
                    }
                    var conversationInfo = uc.modules.conversation.ConversationManager.getConversation(to.userId, conversationType);
                    if(atCounts){
                        conversationInfo.atCounts -= counts;
                        if(conversationInfo.atCounts <= 0){
                            conversationInfo.atCounts = 0;
                        }
                        uc.modules.conversation.ConversationManager.updateConversation(to.userId, conversationType, conversationInfo)
                    }

                    var atMsg =[];
                    atMsg.push(parseInt(msgSeq));
                    uc.IGroupService.deleteAtMessage(conversationType, to.userId, atMsg, function(args){
                        uc.util.LogUtil.info("GroupChat","deleteAtMessage","window is opened,errorCode"+args[0],{conversationType:conversationType,userId:to.userId,atMsg:atMsg});
                    });
            }
        },
        onGroupImageMessageReceived: function(conversationType, channel, sendState, self, from, msgId, msgSeq, fileName, fileSize, fileURL, thumbnails, msgTime){
            var domStr = this.msgBox.getImgMsgDomStr(sendState, self, from, msgId, msgSeq, fileName, fileSize, fileURL, thumbnails, msgTime);
            this.procMessageDomStr(channel, domStr, msgId, msgSeq);
        },
        onGroupAudioMessageReceived: function(conversationType, channel, sendState, self, from, msgId, msgSeq, fileName, fileSize, fileURL, duration, msgTime){
            var domStr = this.msgBox.getAudioMsgDomStr(sendState, self, from, msgId, msgSeq, fileName, fileSize, fileURL, duration, msgTime);
            this.procMessageDomStr(channel, domStr, msgId, msgSeq);
        },
        onGroupVideoMessageReceived: function(conversationType, channel, sendState, self, from, msgId, msgSeq, fileName, fileSize, fileURL, thumbnails, duration, msgTime){

        },
        OnGroupReportMessageReceived: function (conversationType, channel, sendState, from, to, eventId, msgId, msgSeq, msgTime, reportTitle, reportUrl) {
            var domStr = this.msgBox.getMeetingReportMsgDomStr(eventId, to.siteId, msgId, msgSeq, msgTime, reportTitle, reportUrl);
            this.procMessageDomStr(channel, domStr, msgId, msgSeq);
        },
        onDocumentShared: function(errorCode, groupId, operatorId, document){

        },
        OnDocumentDeleteMessageReceived: function (conversationType, channel, from, to, operatorId, fileId, fileName, msgId, msgSeq, msgTime) {
            var message = this.getMessageUserName(operatorId) + uc_resource.GroupChat.Delete+'(' + fileName + ')';
            var domStr = this.msgBox.getSysMsgDomStr(message, msgSeq);
            this.procMessageDomStr(channel, domStr, msgId, msgSeq);
        },

        OnGroupCreatedMessageReceived: function(conversationType, channel, from, to, operatorId, groupId, groupName, groupAvatar, members, msgId, msgSeq, msgTime){
            var getRole=this.getGroupMessageMembersRole(operatorId,members);
            var message = this.getMessageUserName(operatorId) + uc_resource.GroupChat.Invitation;
            message += this.getGroupMessageMembersName(operatorId, members);
            message += uc_resource.GroupChat.Join;
            message += uc_resource.GroupChat[uc.constants.Conversation.type2ChartName[conversationType]||'GroupChat'];
            if(getRole==1){
                message=message+"、"+uc_resource.Message.setMemberAdmin;
            }
            var domStr = this.msgBox.getSysMsgDomStr(message, msgSeq);
            this.procMessageDomStr(channel, domStr, msgId, msgSeq);
            var groupInfo = uc.IGroupService.getGroupInfo(this.groupId);
            if (groupInfo != null) {
                this.updateGroupMember(groupInfo.members);
            }
        },
        OnGroupMemberExitedMessageReceived: function(conversationType, channel, from, to, operatorId, memberId, groupId, groupName, groupAvatar, msgId, msgSeq, msgTime){
            var message = this.getMessageUserName(memberId) + uc_resource.GroupChat.Exit;
            message += uc_resource.GroupChat[uc.constants.Conversation.type2ChartName[conversationType]||'GroupChat'];
            var domStr = this.msgBox.getSysMsgDomStr(message, msgSeq);
            this.procMessageDomStr(channel, domStr, msgId, msgSeq);
            if (memberId == this.myUserInfo.userId && channel == uc.constants.Message.MessageChannel.UCAS) {
                this.enableInput(false);
            }
            var groupInfo = uc.IGroupService.getGroupInfo(this.groupId);
            if (groupInfo != null && channel == uc.constants.Message.MessageChannel.UCAS) {
                this.updateGroupMember(groupInfo.members);
            }
            if (channel == uc.constants.Message.MessageChannel.UCAS) {
                this.updateChatName({name: groupName, isUpdate: true});
            }
        },
        OnGroupEndMessageReceived: function(conversationType, channel, from, to, operatorId, msgId, msgSeq, msgTime){
            var message = this.getMessageUserName(operatorId);
            message =  message + uc_resource.GroupChat.EndGroupChat + (uc_resource.GroupChat[uc.constants.Conversation.type2ChartName[conversationType]||'GroupChat']);
            var domStr = this.msgBox.getSysMsgDomStr(message, msgSeq);
            this.procMessageDomStr(channel, domStr, msgId, msgSeq);
            if (channel == uc.constants.Message.MessageChannel.UCAS) {
                this.enableInput(false);
            }
        },
        OnGroupMemberAddedMessageReceived: function(conversationType, channel, from, to, operatorId, groupId, groupName, groupAvatar, members, msgId, msgSeq, msgTime){
            var getRole=this.getGroupMessageMembersRole(from.userId,members);
            var message = this.getMessageUserName(operatorId) + uc_resource.GroupChat.Invitation;
            message += this.getGroupMessageMembersName(operatorId, members);
            message += uc_resource.GroupChat.Join;
            message += uc_resource.GroupChat[uc.constants.Conversation.type2ChartName[conversationType]||'GroupChat'];
            if(getRole==1){
                message=message+"、"+uc_resource.Message.setMemberAdmin;
            }
            var domStr = this.msgBox.getSysMsgDomStr(message, msgSeq);
            this.procMessageDomStr(channel, domStr, msgId, msgSeq);
            for(var i = 0, len = members.length; i < len; i++){
                var uid;
                if(typeof members[i] == 'object'){
                    uid = members[i].userId;
                }else{
                    uid = members[i];
                }
                if (uid == this.myUserInfo.userId  && channel == uc.constants.Message.MessageChannel.UCAS) {
                    this.enableInput(true);
                }
            }
            var groupInfo = uc.IGroupService.getGroupInfo(this.groupId);
            if (groupInfo != null && channel == uc.constants.Message.MessageChannel.UCAS) {
                this.updateGroupMember(groupInfo.members);
            }
            if (channel == uc.constants.Message.MessageChannel.UCAS) {
                this.updateChatName({name: groupName, isUpdate: true});
            }
        },
        GroupMemberKickedMessageReceived: function(conversationType, channel, from, to, operatorId, groupId, groupName, groupAvatar, members, msgId, msgSeq, msgTime){
            var userNames, message;
            var kickGroupName = uc_resource.GroupChat['Del' + (uc.constants.Conversation.type2ChartName[conversationType]||'GroupChat')];

            if(operatorId == this.myUserInfo.userId){
                message = uc_resource.GroupChat.IMe + this.getGroupMessageMembersName(operatorId, members) + kickGroupName;
            }else{
                var selfUserName=this.getMessageUserName(operatorId);
                if(members.indexOf(this.myUserInfo.userId) == -1){
                    userNames = this.getGroupMessageMembersName(operatorId, members);
                    message = userNames + uc_resource.GroupChat.By + selfUserName + kickGroupName;
                }else{
                    message = uc_resource.GroupChat.IBy + selfUserName + kickGroupName;
                }
            }
            var domStr = this.msgBox.getSysMsgDomStr(message, msgSeq);
            this.procMessageDomStr(channel, domStr, msgId, msgSeq);
            for(var i = 0, len = members.length; i < len; i++){
                var uid;
                if(typeof members[i] == 'object'){
                    uid = members[i].userId;
                }else{
                    uid = members[i];
                }
                if (uid == this.myUserInfo.userId  && channel == uc.constants.Message.MessageChannel.UCAS) {
                    this.enableInput(false);
                }
            }
            var groupInfo = uc.IGroupService.getGroupInfo(this.groupId);
            if (groupInfo != null && channel == uc.constants.Message.MessageChannel.UCAS) {
                this.updateGroupMember(groupInfo.members);
            }
            if (channel == uc.constants.Message.MessageChannel.UCAS) {
                this.updateChatName({name: groupName, isUpdate: true});
            }
        },
        OnGroupNameUpdatedMessageReceived: function(conversationType, channel, from, to, operatorId, groupId, groupName, namePinYin, msgId, msgSeq, msgTime){
            var message = this.getMessageUserName(operatorId) + uc_resource.GroupChat.ModifyI
                + (uc_resource.GroupChat[uc.constants.Conversation.type2ChartName[conversationType]||'GroupChat'])
                + uc_resource.GroupChat.GroupName + uc.util.StringUtil.escapeHtml(groupName);

            var domStr = this.msgBox.getSysMsgDomStr(message, msgSeq);
            this.procMessageDomStr(channel, domStr, msgId, msgSeq);

            if (channel == uc.constants.Message.MessageChannel.UCAS) {
                this.updateChatName({name: groupName, isUpdate: true});
            }
        },
        OnGroupAvatarUpdatedMessageReceived: function(conversationType, channel, from, to, operatorId, groupId, groupAvatar, msgId, msgSeq, msgTime){

            var message = this.getMessageUserName(operatorId) + uc_resource.GroupChat.UpdateI
                + (uc_resource.GroupChat[uc.constants.Conversation.type2ChartName[conversationType]||'GroupChat'])
                + uc_resource.GroupChat.Avatar;

            var domStr = this.msgBox.getSysMsgDomStr(message, msgSeq);
            this.procMessageDomStr(channel, domStr, msgId, msgSeq);
        },
        OnGroupDisturbSetMessageReceived: function(conversationType, channel, from, to, groupId, state, msgId, msgSeq, msgTime){
            var groupInfo = uc.IGroupService.getGroupInfo(groupId);
            this.showDisturbIcon(groupInfo);
        },
        OnGroupOnTopMessageReceived: function(conversationType, channel, from, to, operatorId, groupId, isTop, msgId, msgSeq, msgTime){

        },

        OnDocumentUploadMessageReceived: function(conversationType, channel, sendState, self, from, operatorId, fileId, fileName,
                                                  fileSize, downloadUrl, fileFormat, desc, msgId, msgSeq, msgTime, username, localPath){
//            if (this.chatType == uc.constants.Chat.ChatType.GroupChat) {
//                fileId = null;
//            }
            var domStr = this.msgBox.getFileMsgDomStr(sendState, self, from, msgId,msgSeq, fileName, fileSize, downloadUrl, msgTime, localPath ,fileId, username,conversationType);
            this.procMessageDomStr(channel, domStr, msgId, msgSeq);
        },
        OnDocumentShareMessageReceived: function(conversationType, channel, from, to, operatorId, fileId, fileName, fileSize, downloadUrl, fileFormat, desc, msgId, msgSeq, msgTime){

        },
        OnCodeSnippetUploadMessageReceived: function(conversationType, channel, sendState, self, from,
                                                     msgId, msgSeq, creatorId, creatorName, codeId, codeTitle, langType, codeHead, codeDesc, codeSize, codeUrl, codeLine, createTime, timestamp){

            //if (this.chatType == uc.constants.Chat.ChatType.GroupChat) {
            //    codeId = null;
            //}
            var domStr = this.msgBox.getCodeMsgDomStr(sendState, self, from,
                msgId, msgSeq, codeTitle, codeSize, codeUrl, langType, codeHead, codeLine, codeDesc, createTime, codeId, creatorName);
            this.procMessageDomStr(channel, domStr, msgId, msgSeq);
        },
        OnGroupNameUpdated: function (errorCode, channel, groupId, operatorId, groupName, pinyin) {
            if (channel == uc.constants.Message.MessageChannel.UCAS) {
                this.updateChatName({name: groupName, isUpdate: true});
            }
        },

//        OnCommentListReceived: function(errorCode, groupId, comments){
//            if ( this.fileComment != null){
//                this.fileComment.OnCommentListReceived(errorCode, groupId, comments);
//            }
//        },
//        OnCommentCreated: function(errorCode, groupId, comment){
//            if ( this.fileComment != null){
//                this.fileComment.OnCommentCreated(errorCode, groupId, comment);
//            }
//        },
        // added by xue.bai_2 on 2016/04/07
        OnRevocationMessageReceived: function(channel, conversationId, conversationType, from, to, msgId, msgSeq, msgTime, seqId){
            var message = uc.util.StringUtil.formatString(uc_resource.Message.RevocationMessage, [this.getMessageUserName(from.userId)]);
            var domStr = this.msgBox.getSysMsgDomStr(message, msgSeq);
            this.procMessageDomStr(channel, domStr, msgId, msgSeq);
            this.msgBox.chatList.find("li[seq='" + seqId + "']").remove();
        },
        OnCommentCreateMessageReceived:function(conversationType, channel, self, from, msgId,
                                                msgSeq, fileId, fileType, fileTitle, commentId, userName, userId, content, createTime, timestamp,cloudType){
            var domStr = this.msgBox.getCommentMsgDomStr(uc.constants.Message.SendState.SendOK, self, from, msgId,
                msgSeq, fileId, fileType, fileTitle, commentId, userName, userId, content, createTime, timestamp,cloudType);
            this.procMessageDomStr(channel, domStr, msgId, msgSeq);
            if ( this.fileComment != null && channel == uc.constants.Message.MessageChannel.UCAS){
                this.fileComment.OnCommentCreateMessageReceived(fileId, userName, userId, content, createTime);
            }
        },
        OnGroupSummaryVideoMessageReceived:function(channel, self,from, to, operatorId, summaryId, summaryName, conferenceId, eventId, meetingName, meetingStartTime, updateTime,type,videoName,videoURL,videoThumb,thumbnailsURL,length,size,operateName,operateAvatar,msgId, msgSeq, msgTime){
            var uid = from.userId == this.myUserInfo.userId ? to.userId : from.userId;
            var resource = from.userId == this.myUserInfo.userId ? uc_resource.Message.CancelAccreditMeetingByMe : uc_resource.Message.CancelAccreditMeetingToMe;
            var domStr = this.msgBox.getSummaryAudioMsgDomStr(self,from, to, operatorId, summaryId, summaryName, conferenceId, eventId, meetingName, meetingStartTime, updateTime,type,videoName,videoURL,videoThumb,thumbnailsURL,length,size,operateName,operateAvatar,msgId, msgSeq, msgTime);
            this.procMessageDomStr(channel, domStr, msgId, msgSeq);
        },
        OnGroupCloudFileMessageReceived: function(conversationType, channel, sendState, self, from, fileId, fileName,fileSize, downloadUrl, fileFormat, desc, msgId, msgSeq, msgTime, username,operation,detailContent){
            // if (this.chatType == uc.constants.Chat.ChatType.CloudChat) {
            //   fileId = null;
            //  }
            var msgContent="",cloudFileInfo="";
            uc.util.LogUtil.info('GroupChat', 'OnGroupCloudFileMessageReceived', 'detailContent = ' + detailContent + ', operation = ' + operation, null);
            if(conversationType== uc.constants.Conversation.ConversationType.GroupChat
                ||conversationType== uc.constants.Conversation.ConversationType.ProjectChat
                ||conversationType== uc.constants.Conversation.ConversationType.CloudChat){
                if(detailContent!=""){
                    cloudFileInfo= JSON.parse(detailContent);
                }
                switch (operation){
                    case uc.constants.Group.GroupCloudType.CloudFileCreate:
                        var domStr = this.msgBox.getCloudFileMsgDomStr(sendState, self, from, msgId,
                            msgSeq, fileName, fileSize, downloadUrl, msgTime, fileId, username,detailContent,conversationType);
                        break;
                    case uc.constants.Group.GroupCloudType.CloudFileUpdate:
                        var domStr = this.msgBox.getCloudFileMsgDomStr(sendState, self, from, msgId,
                            msgSeq, fileName, fileSize, downloadUrl, msgTime, fileId, username,detailContent,conversationType,operation);
                        break;
                    case uc.constants.Group.GroupCloudType.CloudFileDel:
                        msgContent=cloudFileInfo.dir ?uc_resource.GroupChat.deleteDir:uc_resource.GroupChat.deleteFile;
                        var message = uc.util.StringUtil.formatString(msgContent,[this.getMessageUserName(from.userId),cloudFileInfo.filename]);
                        var domStr = this.msgBox.getSysMsgDomStr(message, msgSeq,detailContent,operation,msgId);
                        break;
                    case uc.constants.Group.GroupCloudType.CloudFileMove:
                        msgContent=cloudFileInfo.dir ?uc_resource.GroupChat.moveDir:uc_resource.GroupChat.moveFile;
                        var message = uc.util.StringUtil.formatString(msgContent,[this.getMessageUserName(from.userId),cloudFileInfo.filename,cloudFileInfo.filename_to]);
                        var domStr = this.msgBox.getSysMsgDomStr(message, msgSeq,detailContent,operation,msgId);
                        break;
                    case uc.constants.Group.GroupCloudType.CloudFileRename:
                        msgContent=cloudFileInfo.dir ?uc_resource.GroupChat.renameDir:uc_resource.GroupChat.renameFile;
                        var message = uc.util.StringUtil.formatString(msgContent,[this.getMessageUserName(from.userId),cloudFileInfo.filename,cloudFileInfo.filename_to]);
                        var domStr = this.msgBox.getSysMsgDomStr(message, msgSeq,detailContent,operation,msgId);
                        break;
                    default:
                        break;
                }
                if(!domStr)return;
                this.procMessageDomStr(channel, domStr, msgId, msgSeq);
            }
        },
        OnGroupMemberRoleUpdatedMessageReceived:function(conversationType, channel, sendState, from, to, msgId, msgSeq, msgTime,roles){
            var getRole=this.getGroupMessageMembersRole(from.userId,roles);
            var getMemberUserName=this.getCloudMessageMembersName(from.userId,roles);
            var resource=getMemberUserName+uc_resource.Cloud.cancelAdmin;
            if(getRole==1){
                resource=getMemberUserName+uc_resource.Cloud.setAdmin;
            }
            var domStr = this.msgBox.getMemberRoleMsgDomStr(resource,msgSeq,msgId);
            if(!domStr)return;
            this.procMessageDomStr(channel, domStr, msgId, msgSeq);
        },

        getMessageUserName: function(userId){
            var userName;
            if(userId == this.myUserInfo.userId){
                userName = uc_resource.GroupChat.Me;
            }else{
                var contactInfo = uc.modules.cache.BasicContactInfoCache.getBasicContactInfo(userId);
                if(contactInfo){
                    userName = contactInfo.displayName;
                }else{
                    userName = '<span rel="' + userId + '">' + userId + '</span>' ;
                }
            }
            return userName;
        },
        getGroupMessageMembersName: function(operatorId, members){
            var userId, userNameArray = [], userName;
            for(var i = 0, len = members.length; i < len && i < uc.constants.UI.GroupInviteMsgUserMaxCount; i++){
                if(typeof members[i] == 'object'){
                    userId = members[i].userId;
                }else{
                    userId = members[i];
                }
                if(userId == operatorId){
                    continue;
                }
                userName = this.getMessageUserName(userId);
                userNameArray.push(userName);
            }
            var userNames = userNameArray.join('、');
            if(members.length > uc.constants.UI.GroupInviteMsgUserMaxCount){
                userNames += uc_resource.Common.Etc;
            }
            return userNames;
        },
        getCloudMessageMembersName: function(operatorId, members){
            var userId, userNameArray = [], userName;
            for(var i = 0, len = members.length; i < len; i++){
                if(typeof members[i] == 'object'){
                    userId = members[i].userId;
                }else{
                    userId = members[i];
                }
                userName = this.getMessageUserName(userId);
                userNameArray.push(userName);
            }
            return userNameArray.join('、');
        },
        getGroupMessageMembersRole: function(operatorId, members){
            var userId,role;
            for(var i = 0, len = members.length; i < len; i++){
                if(typeof members[i] == 'object'){
                    userId = members[i].userId;
                    if(userId == operatorId){
                        continue;
                    }
                    role= members[i].role;
                    break;
                }
            }
            return role;
        },
        showFileCommentByFileId: function(fileId){
            if(fileId=="" || fileId=="null" || fileId==0)return;
            uc.util.LogUtil.info("GroupChat","showFileCommentByFileId","groupId:"+this.groupId+",fileId:"+fileId);
            uc.IGroupService.getDocumentInfo(this.groupId, Number(fileId), function(args){
                $(".chat-file").addClass("selected");
            });
        },
        hideFileComment: function(){
            if ( uc.modules.chat.ChatManager.fileComment != null){
                uc.modules.chat.ChatManager.fileComment.show(false);
            }
        },
        resendMessage: function (msgId){
            var This = this;
            console.log("resendMessage", msgId);
            uc.IGroupService.resendMessage(msgId,
                function(args){
                    console.log(args);
                    if (args[0] != 0) {
                        This.msgBox.showSendFailFlag(msgId, true);
                    }
                });
        },
        OnGroupUnreadUserCountEvent:function(userId,info){
            if(userId==this.groupId){
                this.msgBox.setUnreadCountEvent(userId,info);
            }
        },
        OnGroupUnreadUserListEvent:function(userId,info){
            if(userId==this.groupId){
                this.msgBox.setUnreadUserList(userId,info);
            }
        },
        OnDocInfoReceived:function(errorCode, groupId, document) {
            var self=this;
            if (groupId != this.groupId || errorCode != 0) {
                return;
            }
            var documentInfo=uc.modules.chat.ChatManager.fileListTagName[document.fileId];
            if(documentInfo  && documentInfo.msgId){
                var msgId=documentInfo.msgId;
                var fsInfo=uc.modules.chat.ChatManager.FSFileInfo[msgId];
                var type=uc.modules.chat.ChatManager.fileListTagName[document.fileId].type;
                delete uc.modules.chat.ChatManager.fileListTagName[document.fileId];
                switch (type){
                    case "receive":
                        var T=$("#"+msgId).find(".op-receive");
                        this.msgBox.fileMsgOperation(T,fsInfo,"receive");
                        break;
                    case "saveAs":
                        var T=$("#"+msgId).find(".op-saveAs");
                        this.msgBox.fileMsgOperation(T,fsInfo,"saveAs");
                        break;
                    case "forward":
                        uc.modules.chat.MessageManager.forwardFileMessage(fsInfo.fileName,fsInfo.fileSize,fsInfo.fileURL);
                        break;
                    case "fsSaveCloud":
                        this.msgBox.OnSaveFileToCloud(fsInfo.fileURL,fsInfo.fileName);
                        break;
                    case "cloudFileDownload":
                        var cloudInfo=uc.modules.chat.ChatManager.cloudFileInfo[msgId];
                        if(cloudInfo){
                            var cloudData=JSON.parse(cloudInfo);
                            var TagType=documentInfo.TagType;
                            var that;
                            if(TagType=="receive"){
                                that=$("#"+msgId).find(".cloudReceive")
                            }else{
                                that=$("#"+msgId).find(".cloudDow")
                            }
                            uc.modules.chat.ChatManager.downloadCloudDir(cloudData,function(data,savePath){
                                self.msgBox.OnStartDownloadCloudFile(that,data,savePath);
                            },documentInfo.TagType);
                        }
                        break;
                    case "cloudFileForward":
                        var cloudInfo=uc.modules.chat.ChatManager.cloudFileInfo[msgId];
                        if(cloudInfo){
                            var cloudFileInfoArr= JSON.parse(cloudInfo);
                            var fileName=cloudFileInfoArr.filename;
                            var fileSize=cloudFileInfoArr.filesize;
                            var fileUrl="";
                            uc.modules.chat.MessageManager.forwardCloudFileMessage(fileName,fileSize,fileUrl,cloudInfo,cloudFileInfoArr);
                        }
                        break;
                    case "cloudFileSaveCloud":
                        var cloudInfo=uc.modules.chat.ChatManager.cloudFileInfo[msgId];
                        if(cloudInfo){
                            uc.modules.cloud.CloudManager.saveCloudFile(null,cloudInfo);
                        }
                        break;
                    case "downloadCode":
                        this.msgBox.downloadCode(msgId);
                        break;
                    case "forwardCode":
                        this.msgBox.forwardCode(msgId);
                        break;
                    case "previewCode":
                        this.msgBox.previewCode(msgId);
                        break;
                }
            }
        },
        OnCloudLoginReceived:function(){
            if( this.chatType == uc.constants.Conversation.ConversationType.GroupChat ||
                this.chatType == uc.constants.Conversation.ConversationType.ProjectChat ){
                $('.cloud',".chat-toolbar").show();
            }
        },
        clear: function(){
            this.nodeHtml = null;
            this.node = null;
            this.headerNode = null;
            this.msgBox.clear();
            this.msgInputBox.clear();
            uc.ui.Message.ContentPanelBody.find('*').off();
            uc.ui.Message.ContentPanelBody.empty();
        },
        OnExternalMessageReceived: function (errorCode, conversationType, relateID, msgCount, minTimestamp, messages) {
            if (msgCount < uc.constants.Message.OnceGetHistoryCount && errorCode == 0) {
                this.externalBox.onLoadAllMsgEnd();
            }

            if (this.externalBox) {
                this.externalBox.OnExternalMessageReceived(errorCode, conversationType, relateID, msgCount, minTimestamp, messages);
            }
        },
        OnCallEnded: function(){
            this._enableCall();
        },
        OnCallStarted: function(){
            this._disableCall();
        },

        OnGroupCallDurationReceived:function(channel, conversationType,groupId, duration, msgId, msgSeq, timestamp){
            var message =uc_resource.Message.CallEndMessage;
            var domStr = this.msgBox.getSysMsgDomStr(message, msgSeq);
            this.procMessageDomStr(channel, domStr, msgId, msgSeq);
        },
        OnGroupCallInvitationRejected: function(conversationType, channel, sendState, self, from, msgId, msgSeq, textType, textContent, msgTime, textFormat){
            var message;
            if (!textContent) {
                message = uc.util.StringUtil.formatString(uc_resource.Message.RejectedGroupCall, [this.getMessageUserName(from.userId)]);


                var domStr = this.msgBox.getSysMsgDomStr(message, msgSeq);
            } else {
                message = uc.util.EmotionUtil.codeToDescription(textContent);
                message = uc.modules.emoji.EmojiConverter.androidCodeToEmotionTag(message);
                var domStr = this.msgBox.getTextMsgDomStr(sendState, self, from, msgId, msgSeq, textType, message, msgTime, msgTime, textFormat);
            }
            this.procMessageDomStr(channel, domStr, msgId, msgSeq);
        },
        OnAtMessageListReceived:function(errorCode, groupId, atMsg){
            if(errorCode == 0){
                if(this.groupId != groupId){
                    return;
                }
                if(atMsg.length){
                    var atListReceive = [];
                    for(var i = 0; i < atMsg.length; i++){
                        var senderId = atMsg[i].sender;
                        var msgSeq= atMsg[i].seq;
                        var senderInfo = uc.modules.cache.BasicContactInfoCache.getBasicContactInfo(Number(senderId));
                        if(senderInfo){
                            var atAvatarUrl = senderInfo.localAvatar?senderInfo.localAvatar:senderInfo.remoteAvatarUrl;
                            var atName = senderInfo.displayName;
                        }
                        $(".at-users").show();
                        $(".at-avatars").find("img").attr("src",atAvatarUrl);
                        $(".at-tips").find(".at-name").text(atName);
                        atListReceive.unshift({
                            atAvatarUrl:atAvatarUrl,
                            atName:atName,
                            atMsgSeq:msgSeq,
                            groupId:groupId
                        });
                }
                    atList = atListReceive;
                    uc.util.LogUtil.info("GroupChat","OnAtMessageListReceived","show AtList",{groupId:groupId,atList:atList});
            }else{
                    uc.util.LogUtil.info("GroupChat","OnAtMessageListReceived","There is no At-Tips",{errorCode:errorCode,groupId:groupId});
                    var conversationInfo = uc.modules.conversation.ConversationManager.getConversation(this.groupId, this.chatType);
                    conversationInfo.atCounts = 0;
                    uc.modules.conversation.ConversationManager.updateConversation(this.groupId, this.chatType, conversationInfo)
                }

            }
        },
        OnAtMessageDeleted:function(errorCode, conversationType, groupId, atSeq){
            if(atList&&atList.length){
                if(groupId != atList[0].groupId){
                    return;
                }
            }
            var conversationInfo = uc.modules.conversation.ConversationManager.getConversation(this.groupId, this.chatType);
            if((errorCode == 0)&&atSeq&&atSeq.length){
                for(var m = 0;m < atList.length; m++){
                    for(var i = 0;i < atSeq.length; i++){

                        if(atSeq[i]==atList[m].atMsgSeq){
                            atList = atList.slice(m+1);
                        }
                    }
                }
                if(atList&&atList.length){
                    $(".at-users").show();
                    $(".at-avatars").find("img").attr("src",atList[0].atAvatarUrl);
                    $(".at-tips").find(".at-name").text(atList[0].atName);
                }else{
                    $(".at-users").hide();
                    conversationInfo.atCounts = 0;
                    uc.modules.conversation.ConversationManager.updateConversation(this.groupId, this.chatType, conversationInfo)
                }
            }else if((errorCode !=0)&&atSeq&&atSeq.length){
                if(atList.length){
                    $(".at-users").show();
                    $(".at-avatars").find("img").attr("src",atList[0].atAvatarUrl);
                    $(".at-tips").find(".at-name").text(atList[0].atName);
                }else{
                    $(".at-users").hide();
                    conversationInfo.atCounts = 0;
                    uc.modules.conversation.ConversationManager.updateConversation(this.groupId, this.chatType, conversationInfo)
                }
            }
        },
	    getCallVisible: function(){
		    return uc.modules.main.Main.getCallVisible();
	    },

	    getCalendarVisible: function () {
		    return uc.modules.main.Main.getCalendarVisible();
	    }
    }
})(uc);