/**
 * Created by waouyang on 15/12/7.
 */
(function(uc){
    uc.modules.component.EmbeddedCallWindowPanel = {
        init: function(parentNode){
            this.nodeHtml = '<div class="embedded-call-window-panel"></div>';
            this.callLoading = '<div class="call-speaking">' +
                '<div class="call-speaking-bar"></div>'+
                '<div class="call-speaking-bar"></div>'+
                '<div class="call-speaking-bar"></div>'+
                '</div>';
            this.node = $(this.nodeHtml).appendTo(parentNode);
            this._bindEvent();
        },
        initUI: function(callInfo){
            this.callInfo = callInfo;
            this._initUI();
        },
        _initUI: function(){
            this.node.empty();
            var audioChatNodeHtml;
            switch (this.callInfo.chatType){
                case uc.constants.Chat.ChatType.SingleChat:
                    audioChatNodeHtml = this._getP2PAudioChatNodeHtml();
                    break;
                case uc.constants.Chat.ChatType.GroupChat:
                case uc.constants.Chat.ChatType.ProjectChat:
                    audioChatNodeHtml = this._getGroupAudioChatNodeHtml();
                    break;
            }
            this.node.append(audioChatNodeHtml);
            this._showCallStatus();
        },

        updateCallInfo: function(callInfo){
            this.callInfo = callInfo;
            this._showCallStatus();
            if(this.callInfo.chatType != uc.constants.Chat.ChatType.SingleChat){
                this._updateCallUserList();
            }
        },

        _showCallStatus: function(){
            if(this.callInfo.chatType == uc.constants.Chat.ChatType.SingleChat){
                switch (this.callInfo.callStatus){
                    case uc.constants.Call.CallStatus.CONNECTING:
                        this.node.addClass('connecting');
                        if(!this.node.find(".p2p-audio-chat-connecting-ball").children().length){
                            this.node.find(".p2p-audio-chat-connecting-ball").append('<div/><div/><div/>');
                        }

                        break;
                    case uc.constants.Call.CallStatus.INVITING:
                        if(!this.node.find(".p2p-audio-chat-connecting-ball").children().length){
                            this.node.find(".p2p-audio-chat-connecting-ball").append('<div/><div/><div/>');
                        }

                        break;
                    case uc.constants.Call.CallStatus.INVITED:
                        if(!this.node.find(".p2p-audio-chat-connecting-ball").children().length){
                            this.node.find(".p2p-audio-chat-connecting-ball").append('<div/><div/><div/>' );
                        }
                        break;
                    case uc.constants.Call.CallStatus.CONNECTED:
                        this.node.removeClass('connecting');
                        this.node.find(".p2p-audio-chat-connecting-ball").remove();
                        break;
                }
            }
        },

        _bindEvent: function(){

        },
        _getP2PAudioChatNodeHtml: function(){
            var contactInfo = uc.IContactService.getBasicContactInfo(this.callInfo.chatId);
            var contactAvatar;
            if(contactInfo && contactInfo.localAvatar){
                contactAvatar = contactInfo.localAvatar;
            }else {
                contactAvatar = uc.constants.UI.DefaultLocalUrl.Avatar_100;
            }
            return '<div class="p2p-audio-chat">' +
                        '<div class="p2p-audio-chat-connecting-circle"></div>' +
                        '<div class="p2p-audio-chat-connecting-ball">' +

                        '</div>' +
                        '<div class="p2p-audio-chat-audio-icon"></div>' +
                        '<div class="p2p-audio-chat-user-avatar">' +
                            '<img src="' + contactAvatar + '" class="p2p-audio-chat-avatar-image">' +
                        '</div>' +
                    '</div>';
        },
        _getCallUserListHtml:function(callInfo){
            var userDom='',userName='',contactInfo,img='';
            var userData = this._updateUserListCallPanel(callInfo.userList);
            if(userData.length>=5){
              $(".embedded-call-window .user-list-up").show();
              $(".embedded-call-window .user-list-down").show();
            }
            for(var i=0;i<userData.length;i++){
                var userId = userData[i].userId;
                if(!callInfo.userList[userId].userInfo){
                    callInfo.userList[userId].userInfo={};
                }
                var userInfo=callInfo.userList[userId].userInfo;
//                if(userId == callInfo.loginUserId || callInfo.userList[userId].callStatus == uc.constants.Call.CallStatus.DISCONNECTED){
//                    continue;
//                }
                img='<img src="'+uc.constants.UI.DefaultLocalUrl.Avatar+'"/>';
                userName='<div class="nameDiv"><span class="userName no-name">'+userId+'</span></div>';
                if(userInfo.displayName){
                    userName='<div class="nameDiv"><span class="userName">'+userInfo.displayName+'</span></div>';
                    if(userInfo.localAvatar){
                        img='<img src="'+userInfo.localAvatar+'"/>';
                    }
                }else{
                    contactInfo = uc.IContactService.getBasicContactInfo(parseInt(userId));
                    if(contactInfo){

                        callInfo.userList[userId].userInfo.displayName=contactInfo.displayName;
                        userName='<div class="nameDiv"><span class="userName">'+userInfo.displayName+'</span></div>';
                        if(contactInfo.localAvatar){
                            callInfo.userList[userId].userInfo.localAvatar=contactInfo.localAvatar;
                            img='<img src="'+contactInfo.localAvatar+'"/>';
                        }
                        uc.modules.call.CallManager.updateCallInfo(callInfo);
                    }
                }
                var userCall='disable';
                var userStatus='<span class="userStatus">'+uc_resource.Call.WaitingAccepted+'</span>';
                if(callInfo.userList[userId].callStatus && callInfo.userList[userId].callStatus == uc.constants.Call.CallStatus.REJECTED){
                    userStatus='<span class="userStatus">'+uc_resource.Call.CallRejected+'</span>';
                }else if(callInfo.userList[userId].callStatus == uc.constants.Call.CallStatus.DISCONNECTED){
                    userStatus='<span class="userStatus">'+uc_resource.Call.CallUserStop+'</span>';
                }else{
                    switch(callInfo.userList[userId].muteStatus){
                        case uc.constants.Call.MuteStatus.MUTE:
                            userCall='';
                            if(callInfo.userList[userId].audioType == uc.constants.Call.AudioType.VOIP){
                                userStatus='<span class="userStatus icon-call_voipVideoMute videoSize"></span>';
                            }else if((callInfo.userList[userId].audioType == uc.constants.Call.AudioType.PHONE)){
                                userStatus='<span class="userStatus icon-call_phoneVideoMute videoSize"></span>';
                            }else{
                                userStatus='<span class="userStatus videoSize"></span>';
                            }
                            break;
                        case uc.constants.Call.MuteStatus.UN_MUTE:
                            userCall='';
                            if(callInfo.userList[userId].audioType == uc.constants.Call.AudioType.VOIP){
                                userStatus='<span class="userStatus icon-call_voipVideo videoSize"></span>';
                            }else if(callInfo.userList[userId].audioType == uc.constants.Call.AudioType.PHONE){
                                userStatus='<span class="userStatus icon-call_phoneVideo videoSize"></span>';
                            }else{
                                userStatus='<span class="userStatus videoSize"></span>';
                            }
                            break;
                    }

                }
                userDom+='<a class="callUserList '+userCall+'" data-userId="'+userId+'">'+img+userName+userStatus+'</a>';
            }
            return userDom;
        },

        _getGroupAudioChatNodeHtml: function(){
            var userDom=this._getCallUserListHtml(this.callInfo);
            return '<div class="group-audio-chat">' +
                    '<div class="group-audio-chat-list">' +
                userDom+
                '</div>'+
                '</div>';
        },
        addAudioClass:function(node,classType,text){
            node.removeClass("icon-call_phoneVideo").removeClass("icon-call_phoneVideoMute")
                .removeClass("icon-call_voipVideo").removeClass("icon-call_voipVideoMute").addClass(classType).text(text);
            if(text){
                node.removeClass("videoSize");
            }else{
                node.removeClass("videoSize").addClass("videoSize");
            }

        },
        updateUserListSpeakingStatus:function(userId,status){
            //this.callInfo=callInfo;
            var user=this.node.find(".callUserList[data-userId="+userId+"]").find(".userStatus");
            switch(status){
                case uc.constants.Call.AcceptorVideoStatus.Mute:
                    if(this.callInfo.userList[userId].audioType == uc.constants.Call.AudioType.VOIP){
                        this.addAudioClass(user,"icon-call_voipVideoMute",'');
                    }else if(this.callInfo.userList[userId].audioType == uc.constants.Call.AudioType.PHONE){
                        this.addAudioClass(user,"icon-call_phoneVideoMute",'');
                    }else{
                        this.addAudioClass(user,"",'');
                    }
                    user.parent().find(".call-speaking").remove();
                    user.parent().removeClass("disable");
                    break;
                case uc.constants.Call.AcceptorVideoStatus.Video:
                    if(this.callInfo.userList[userId].audioType == uc.constants.Call.AudioType.VOIP){
                        this.addAudioClass(user,"icon-call_voipVideo",'');
                    }else if(this.callInfo.userList[userId].audioType == uc.constants.Call.AudioType.PHONE){
                        this.addAudioClass(user,"icon-call_phoneVideo",'');
                    }else{
                        this.addAudioClass(user,"",'');
                    }
                    user.parent().find(".call-speaking").remove();
                    user.parent().removeClass("disable");
                    break;
                case uc.constants.Call.AcceptorVideoStatus.Speaking:
                    if(this.callInfo.userList[userId].audioType == uc.constants.Call.AudioType.VOIP){
                        this.addAudioClass(user,"icon-call_voipVideo",'');
                    }else if(this.callInfo.userList[userId].audioType == uc.constants.Call.AudioType.PHONE){
                        this.addAudioClass(user,"icon-call_phoneVideo",'');
                    }else{
                        this.addAudioClass(user,"",'');
                    }
                    if(!user.parent().find(".call-speaking").length){
                        user.parent().prepend(this.callLoading);
                    }
                    user.parent().removeClass("disable");
                    break;
                case uc.constants.Call.AcceptorVideoStatus.Rejected:
                    this.addAudioClass(user,"",uc_resource.Call.CallRejected);
                    user.parent().find(".call-speaking").remove();
                    user.parent().removeClass("disable").addClass("disable");
                break;
            }
        },
        updateContact:function(basicContactInfo){
            if(!this.node){
               return;
            }
            var user=this.node.find(".callUserList[data-userId="+basicContactInfo.userId+"]");
            user.find(".no-name").text(basicContactInfo.displayName);
            if(user.find(".no-name").length && basicContactInfo.localAvatar){
                user.find("img").attr("src",basicContactInfo.localAvatar);
            }
            user.find(".no-name").removeClass("no-name");
        },

        _updateCallUserList: function(){
            var userDom=this._getCallUserListHtml(this.callInfo);
            $(".group-audio-chat-list a").remove();
            $(".group-audio-chat-list").append(userDom);
        },

        handleLongTimeNoReceive:function(){
            var html='<div class="longTimeTip">'+uc_resource.Call.LongTimeNoHandle+'</div>'
            if(this.callInfo.chatType == uc.constants.Chat.ChatType.SingleChat){
                this.node.find("img").after(html);
            }
        },
        handleLongTimeReceive:function(){
            this.node.find(".longTimeTip").remove();
        },
        compareFunction:function(userData1,userData2){
            var loginUserId =  uc.IContactService.getCurrentUserInfo().userId;
            if(userData2.userId == loginUserId){
                return userData2.callStatus;
            }else{
                return userData2.callStatus-userData1.callStatus;
            }
        },

        _updateUserListCallPanel:function(userData){
            var userList=[];
            for(var i in userData){
                var user=userData[i];
                user.userId=i;
                userList.push(user);
            }
            userList.sort(this.compareFunction);
            return userList;
        }
    }
})(uc);