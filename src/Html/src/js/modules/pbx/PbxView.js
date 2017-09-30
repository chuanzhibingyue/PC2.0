(function(uc){
    uc.modules.pbx.PbxView  = function(){
        this.pbxInfo = null;
        this.nodeHtml = '<div class="info"><div class="title"></div></div>' +
            '<div class="info message"></div>' +
            '<span class="msg-time"></span>'+
            '<span class="msg-num"></span>';
        this.node = $('<li class="list-item conversation-item"></li>');
    };
    jQuery.extend(uc.modules.pbx.PbxView.prototype, {
        init: function(options){
            this.node.append(this.nodeHtml);
            this.appendToParent(options.parentNode);
            this.avatarNode = $('.avatar-node', this.node);
            this.titleNode = $('.title', this.node);
            this.superGroupIcon = $('.icon-super-group', this.node);
            this.distrurbIcon = $('.icon-disturb',this.node);
            this.messageNode = $('.message', this.node);
            this.msgNumNode = $('.msg-num', this.node);
            this.msgTimeNode = $('.msg-time', this.node);
            this.msgInCall=  $('.avatar-call-node', this.node);
            this.bindEvent();
        },
        appendToParent: function(parentNode){
            this.node.appendTo(parentNode);
        },
        bindEvent: function(){
            this.node.click(uc.util.EventUtil.hitch(this, 'onPbxViewClicked'));
            this.node.contextmenu(uc.util.EventUtil.hitch(this, 'onPbxViewRightClicked'));
          //  this.node.hover(uc.util.EventUtil.hitch(this, 'onHoverIn'), uc.util.EventUtil.hitch(this, 'onHoverOut'));
        },
        onPbxViewClicked: function(){
            this.node.addClass('active').siblings().removeClass('active');
            uc.IUIEvent.OnOpenPbxCallChat(this.pbxInfo);
        },
        onPbxViewRightClicked: function (e) {
           this.onPbxViewClicked();
            if(this.pbxInfo && this.pbxInfo.userID != 0){
                var contactInfo = uc.IContactService.getContactInfo(Number(this.pbxInfo.userID));
                if(contactInfo) {
                    this.pbxInfo.phoneNumber = contactInfo.mobile;
                }
            }
            uc.util.MenuUtil.showPbxContextMenu(e,this.pbxInfo);
        },
        onHoverIn: function(){

        },
        onHoverOut: function(){

        },
        setPbxInfo: function(pbxInfo){
            if(pbxInfo){
                this.pbxInfo = pbxInfo;
                this.showPbxView();
            }
        },
        showPbxView: function(){
            if(this.pbxInfo.selected){
                this.node.addClass('active');
            }else{
                this.node.removeClass('active');
            }
            this.showTitle();
            this.showMessageTime();
            this.showMessage();
           // this.setAvatar();
          //  this.setInCall();
        },
        showTitle: function(){
            var count;
            if(this.pbxInfo && this.pbxInfo.userID && this.pbxInfo.userName){
                if(this.pbxInfo.count){
                    count = "("+this.pbxInfo.count +")";
                    this.titleNode.text(this.pbxInfo.userName+count);
                }else{
                    this.titleNode.text(this.pbxInfo.userName);
                }
            }else if(this.pbxInfo && this.pbxInfo.phoneNumber){
                if(this.pbxInfo.count){
                    count = "("+this.pbxInfo.count +")";
                    this.titleNode.text(this.pbxInfo.phoneNumber+count);
                }else{
                    this.titleNode.text(this.pbxInfo.phoneNumber);
                }
            }else{
                console.log(this.pbxInfo)
            }
        },
        showMessageTime: function(){
            try{
                if(this.pbxInfo.time){
                    this.msgTimeNode.text(uc.util.TimeUtil.getMessageTimeString((parseInt(this.pbxInfo.time)), null, uc.settings.meeting.timeZone.offset));
                }else{
                    this.msgTimeNode.text('');
                }
            }catch (e){
                console.log(this.pbxInfo.time)
            }
        },
        showMessage: function(){
            var isCallIn=this.pbxInfo.incoming==0 ? uc_resource.Pbx.callOut: uc_resource.Pbx.callIn;
            var status=this.getCallStatus(this.pbxInfo.state,this.pbxInfo.druation,this.pbxInfo.incoming);
            if((this.pbxInfo.incoming == 1) &&  (this.pbxInfo.state == 2 || this.pbxInfo.state == 0)){
                this.messageNode.addClass("fontRed");
            }else{
                this.messageNode.removeClass("fontRed");
            }
            this.messageNode.html(isCallIn+"&nbsp;&nbsp;"+status);
        },
        setAvatar: function(){
             var avatarNode = '<img src="../images/avatar_default.png" class="avatar round-image">';
            if(this.pbxInfo && this.pbxInfo.userID && this.pbxInfo.userName && this.pbxInfo.avatar){
                avatarNode = uc.util.AvatarUtil.composeContactAvatar(this.pbxInfo.avatar, uc.constants.UI.DefaultLocalUrl.Conversation.Avatar);
            }
            this.avatarNode.html(avatarNode);
        },
        setInCall:function(){
            if(this.pbxInfo.isInCall){
                this.node.addClass('active').siblings().removeClass('active');
              //  this.msgInCall.show();
            }else{
                this.msgInCall.hide();
            }
        },
        show: function(){
            this.node.show();
        },
        hide: function(){
            this.node.hide();
        },
        getCallDuration:function(){
            var duration="";
            if(this.pbxInfo.duration){
                 duration=uc.util.TimeUtil.getCallTimeStr(this.pbxInfo.duration);
                 duration=uc_resource.Pbx.duration+duration;
            }
            return duration;
        },
        getCallStatus:function(status,druation){
            var callStr="";
            switch (status){
                case uc.constants.Pbx.CallStatusType.CALL_STATE_EARLY:
                    callStr=uc_resource.Pbx.callOutUn;
                    break;
                case uc.constants.Pbx.CallStatusType.CALL_STATE_CONFIRMED:
                    if(druation){
                        callStr=uc_resource.Pbx.duration+"&nbsp;&nbsp;"+uc.util.TimeUtil.getCallTimeStr(druation);
                    }else{
                        callStr=uc_resource.Pbx.callOrhangupCal;
                    }
                    break;
                case uc.constants.Pbx.CallStatusType.CALL_STATE_CANCEL:
                    if(druation){
                        callStr= uc_resource.Pbx.duration+"&nbsp;&nbsp;"+uc.util.TimeUtil.getCallTimeStr(druation);
                    }else{
                        callStr=uc_resource.Pbx.callCancel;
                    }
                    break;
                case uc.constants.Pbx.CallStatusType.CALL_STATE_HANGUP:
                    if(druation){
                        callStr=uc_resource.Pbx.duration+"&nbsp;&nbsp;"+uc.util.TimeUtil.getCallTimeStr(druation);
                    }else{
                        callStr=uc_resource.Pbx.callRefuse;
                    }
                    break;
                case uc.constants.Pbx.CallStatusType.CALL_STATE_FAIL:
                    callStr=uc_resource.Pbx.callOutFail;
                    break;
                case uc.constants.Pbx.CallStatusType.CALL_STATE_NONE:
                    callStr=uc_resource.Pbx.callOutUn;
                    break;
                case uc.constants.Pbx.CallStatusType.CALL_STATE_Rejected:
                    callStr=uc_resource.Pbx.callRejected;
                    break;
            }
            return callStr;
        }
    })
})(uc);