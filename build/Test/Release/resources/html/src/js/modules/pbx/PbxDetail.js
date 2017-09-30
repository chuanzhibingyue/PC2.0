(function(uc){
    uc.modules.pbx.PbxDetail = function(){
        this.PbxNotMessage='<div class="init-panel">' +
            '<div class="init-panel-none">' +
            '<img src="../images/chat/noneChat.png">' +
            '<div class="init-panel-noneText">' + uc_resource.Pbx.NotPbxMessage + '</div></div></div>';
        this.nodeHtml ='<div class="pbx-detail-head">' +
                '<div class="pbx-avatar"></div>' +
                '<div class="pbx-title"></div>' +
            '</div>'+
            '<div class="pbx-tel"></div>' +
            '<div class="call-title">' +
                '<span class="pbx-call-left-line"><span class="pbx-call-title-text">'+uc_resource.Pbx.callRecorder+'<span></span>' +
            '</div>';
        this.node = $('<div class="pbx-detail hide"></div>');
        this.nodeList=$('<div class="pbx-call-history-panel"><div class="pbx-call-history"><ul class="pbx-call-history-list"></ul></div>');
        this.callId = 0;
        this.pbxInfo={};
        this.getCallHistoryTime=0;
        this.needScroll=true;
    };
    jQuery.extend(uc.modules.pbx.PbxDetail.prototype, {
        init: function(options){
            this.node.append(this.nodeHtml);
            uc.ui.Pbx.ContentPanelBody.append(this.PbxNotMessage);
            this.node.appendTo(uc.ui.Pbx.ContentPanelBody);
            this.nodeList.appendTo(uc.ui.Pbx.ContentPanelBody);
            this.pbxHideNode = $(".pbx .pbx-detail");
            this.avatarNode = $('.pbx-avatar', this.node);
            this.titleNode = $('.pbx-title', this.node);
            this.telNode = $('.pbx-tel', this.node);
            this.callHistoryNode = $('.pbx-call-history-list', this.nodeList);
            this.myUserInfo = uc.modules.chat.ChatManager.myUserInfo;
            if (this.myUserInfo == null) {
                this.myUserInfo = uc.IContactService.getCurrentUserInfo();
            }
            this.bindEvent();
        },
        loadPbxDetail:function(data,type){
            if(!this.initilized){
                $(".pbx .init-panel").remove();
                $(".pbx .content-panel-body").addClass("pbx-content-flex");
                this.pbxHideNode.removeClass("hide");
            }
            this.initilized = true;
            this.pbxInfo=data;
            this.callId = 0;
            this.callHistoryNode.html("");
            this.lastDate=0;
            this.getCallHistoryTime=0;
            this.needScroll=true;
            this.setAvatar();
            this.setTel();
            this.getCallHistory();
        },
        setAvatar: function(){
            var avatarNode = '<img src="../images/avatar_default.png" class="avatar round-image">';
            if(this.pbxInfo && this.pbxInfo.userID && this.pbxInfo.userName && this.pbxInfo.avatar){
                avatarNode = uc.util.AvatarUtil.composeContactAvatar(this.pbxInfo.avatar, uc.constants.UI.DefaultLocalUrl.Avatar_80);
            }
            this.avatarNode.html(avatarNode);
        },
        updateDetailAvatar:function(userId,localAvatarUrl){
            if(this.pbxInfo && this.pbxInfo.userID == userId){
                this.pbxInfo.avatar=localAvatarUrl;
                this.setAvatar();
            }
        },
        setTel:function(){
            var str = "";
            var callLabel = "";
            var nodeHtml= "";
            var capability = JSON.parse(uc.util.StorageUtil.getData("capability"));
            if(this.pbxInfo && this.pbxInfo.userID != 0){
                if(capability && capability.pbx == 1){
                    var contactInfo = uc.IContactService.getContactInfo(Number(this.pbxInfo.userID));
                    if(contactInfo) {
                        var phoneNumber = contactInfo.mobile;
                        if(phoneNumber && phoneNumber.indexOf("/") >-1){
                            var sp=phoneNumber.split("/");
                            for(var i= 0,len = sp.length;i < len;i++){
                                str +="<div><a class='call-tel' tel="+sp[i]+">"+sp[i]+"</a><span class='icon-call_phoneVideo call-icon'></span></div>";
                            }
                        }else if(phoneNumber && phoneNumber !=""){
                            str="<div><a class='call-tel' tel="+phoneNumber+">"+phoneNumber+"</a><span class='icon-call_phoneVideo call-icon'></span></div>";
                        }
                    }else if(this.pbxInfo.phoneNumber && this.pbxInfo.phoneNumber != ""){
                        str="<div><a class='call-tel' tel="+this.pbxInfo.phoneNumber+">"+this.pbxInfo.phoneNumber+"</a><span class='icon-call_phoneVideo call-icon'></span></div>";
                    }
                }
                if(capability && capability.call == 1){
                    callLabel = "<div><a class='call-bee'>"+uc_resource.Pbx.callBee+"</a><span class='icon-call_phoneVideo  call-icon-bee'></span></div>";
                }
                nodeHtml = callLabel +str;
                this.titleNode.text(this.pbxInfo.userName?this.pbxInfo.userName:this.pbxInfo.phoneNumber);
            }else{
                if(capability && capability.pbx == 1){
                    var tel = this.pbxInfo.phoneNumber+"";
                    if(tel.length > 15){
                        tel = this.pbxInfo.phoneNumber.substring(0,15)+"...";
                    }
                    nodeHtml ="<div><a class='call-tel' tel="+this.pbxInfo.phoneNumber+">"+tel+"</a>" +
                        "<span class='icon-call_phoneVideo call-icon'></span></div>";
                }
                this.titleNode.text(this.pbxInfo.phoneNumber);
            }
            this.telNode.html(nodeHtml);
        },
        getCallHistory:function(){
            var self = this;
            uc.IPbxService.getCallDetailsLst(this.pbxInfo.phoneNumber+"",parseInt(this.pbxInfo.userID),this.getCallHistoryTime,uc.constants.Pbx.GetCallDetailCount,function(args){
                uc.util.LogUtil.info('PbxDetail','getCallHistory', 'errorCode = ' + args[0]+",getCallHistoryTime="+self.getCallHistoryTime, null);
            });
        },
        OnPbxCallListDetail:function(data){
            var node="";
            var time="";
            var status="";
            var dataInfo="";
            var formatTime="";
            var isCallIn="";
            for(var i= 0,len=data.length;i<len;i++){
                dataInfo=data[i];
                time=dataInfo.time;
                var date = new Date(time).toISOString().slice(0,10);
                if (this.lastDate != date) {
                    this.lastDate = date;
                    node+= "<li class='li-title-data'>"+uc.util.TimeUtil.getPbxMessageTimeStr(time)+"<span class='round'><span></li>"
                }
                formatTime = uc.util.TimeUtil.getMessageTimeString((parseInt(time)),uc.constants.UI.TimeFormat.Message.HourMinutes, uc.settings.meeting.timeZone.offset);
                isCallIn=dataInfo.incoming==0 ? uc_resource.Pbx.callOut: uc_resource.Pbx.callIn;
                status=this.getCallStatus(dataInfo.state,dataInfo.druation);
                if((dataInfo.incoming == 1) &&  (dataInfo.state == 2 || dataInfo.state == 0)){
                    node+= "<li class='li-title '>"+formatTime+"&nbsp;&nbsp;&nbsp;<span class='fontRed'>"+isCallIn+"&nbsp;&nbsp;&nbsp;"+status+"</span></li>";
                }else{
                    node+= "<li class='li-title'>"+formatTime+"&nbsp;&nbsp;&nbsp;"+isCallIn+"&nbsp;&nbsp;&nbsp;"+status+"</li>";
                }
            }
            if(data.length<uc.constants.Pbx.GetCallDetailCount){
                this.needScroll=false;
            }
            this.getCallHistoryTime=data[data.length-1].time;
            this.callHistoryNode.append(node);
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
        },
        bindEvent:function(){
            var self=this;
            $(".pbx .pbx-call-history-panel").scroll(function(){
                if(!self.needScroll){
                    return ;
                }
                var viewH =$(this).height(),
                    contentH =$(this).get(0).scrollHeight,
                    scrollTop =$(this).scrollTop();
                if(scrollTop/(contentH -viewH)>=0.95){
                    self.getCallHistory();
                }
            });
            this.telNode.on("click",".call-tel",function(){
                var tel=$(this).attr("tel");
                self.prepareCallInfo(tel);
                return false;
            });
            this.telNode.on("click",".call-icon",function(){
                var tel=$(this).siblings("a").text();
                self.prepareCallInfo(tel);
                return false;
            });
            this.telNode.on('click','.call-bee,.call-icon-bee',function() {
                uc.IUIEvent.OnPbxCallBee(self.pbxInfo.userID);
            });
        },
        prepareCallInfo:function(phoneNumber){
            var callInfo={"userName":this.pbxInfo.userName,
                "userID":this.pbxInfo.userID,
                "phoneNumber":phoneNumber,
                "callType":0,
                "Type":"ADEXEO2103"
            };
            uc.modules.pbx.PbxCallManager.startCallPbx(callInfo);
        }
    })
})(uc);