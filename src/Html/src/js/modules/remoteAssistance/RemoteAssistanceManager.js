/**
 * Created by waouyang on 15/12/22.
 */
(function(uc){
    uc.modules.remoteAssistance.RemoteAssistanceManager = {
        remoteAssistanceStatus: uc.constants.RemoteAssistanceStatus.NONE,
        remoteAssistanceInfo: {},
        inProgressRemoteAssistanceUserId: 0,
        isHost: false,

        /** start remote assistance **/
        startRemoteAssistance: function(userId){
            var _this = this;
            this.isHost = true;
            uc.IRemoteAssistanceService.createRemoteAssistance(userId, function(args){
                var errorCode = args[0];
                if(errorCode != uc.ErrorCode.Success){
                    _this._showStartRemoteAssistanceFailed();
                }
            })
        },
        updateRemoteAssistanceStatus: function(userId, status){
            if(this.remoteAssistanceInfo[userId]){
                this.remoteAssistanceInfo[userId].status = status;
            }
        },
        getRemoteAssistanceChatInfo: function(userId){
            return this.remoteAssistanceInfo[userId];
        },
        isInRemoteAssistance: function(){
            return this.inProgressRemoteAssistanceUserId != 0;
        },
        isInvitedToRemoteAssistance: function(userId){
            var result = false;
            var singleRemoteAssistanceInfo = this.remoteAssistanceInfo[userId];
            if(singleRemoteAssistanceInfo){
                result = singleRemoteAssistanceInfo.status == uc.constants.RemoteAssistanceStatus.INVITED;
            }
            return result;
        },
        sendRemoteAssistanceInvitation: function(){
            var contactInfo = uc.IContactService.getContactInfo(this.inProgressRemoteAssistanceUserId);
            if(contactInfo){
                var jid = {
                    userId: this.inProgressRemoteAssistanceUserId,
                    siteId: contactInfo.siteId,
                    resId: 100
                };
                var remoteAssistanceInfo = this.remoteAssistanceInfo[this.inProgressRemoteAssistanceUserId];
                if(remoteAssistanceInfo){
                    var _this = this;
                    uc.IRemoteAssistanceService.sendRemoteAssistanceInvitation(jid, remoteAssistanceInfo.meetingId, remoteAssistanceInfo.attendeePassword, function(args){
                        if(args[0] != uc.ErrorCode.Success){
                            _this._showSendRemoteAssistanceInvitationFailed();
                        }
                    });
                }else {
                    this._showSendRemoteAssistanceInvitationFailed();
                }
            }else {
                this._showSendRemoteAssistanceInvitationFailed();
            }
        },
        /** Meeting Events**/
        OnMeetingStartUrlByConfIdReceived: function(/*int*/errorCode, startUrl, confId){
            if(confId == this.joiningRemoteAssistnceId){
                if(errorCode != uc.ErrorCode.Success){
                    this._showJoinRemoteAssistanceFailed(null, confId);
                }else {
                    this._startMeetingClient(startUrl);
                }
            }
        },
        /** Remote Assistance Events**/
        OnRemoteAssistanceCreated: function(errorCode, userId, meetingId, attendeePassword, meetingStartUrl){
            if(errorCode != uc.ErrorCode.Success){
                this._showStartRemoteAssistanceFailed();
            }else {
                this.remoteAssistanceInfo[userId] = {
                    userId: userId,
                    meetingId: meetingId,
                    attendeePassword: attendeePassword,
                    meetingStartUrl: meetingStartUrl,
                    status: uc.constants.RemoteAssistanceStatus.CREATED
                };
                this.joinRemoteAssistance(userId, meetingId, attendeePassword, meetingStartUrl, true);
            }
        },

        OnRemoteAssistanceInvitationReceived: function(from, meetingId, attendeePassword){
            var loginUserInfo = uc.IClientService.getCurrentUCID();
            var loginUserId = 0;
            if(loginUserInfo){
                loginUserId = loginUserInfo.userId;
            }
            if(from.userId == loginUserId){
                return;
            }
            this.isHost = false;
            this.remoteAssistanceInfo[from.userId] = {
                userId: from.userId,
                siteId: from.siteId,
                resId: from.resId,
                meetingId: meetingId,
                attendeePassword: attendeePassword,
                status: uc.constants.RemoteAssistanceStatus.INVITED
            }
        },

        OnRemoteAssistanceInvitationAccepted: function(from, meetingId){
            this.updateRemoteAssistanceStatus(from.userId, uc.constants.RemoteAssistanceStatus.ACCEPTED);
        },

        OnRemoteAssistanceInvitationRejected: function(from, meetingId){
            if(this.remoteAssistanceInfo[from.userId]){
                delete this.remoteAssistanceInfo[from.userId];
            }
        },

        OnRemoteAssistanceEnded: function(meetingId){
            var userId = this.getUserIdByMeetingId(meetingId);
            if(this.inProgressRemoteAssistanceUserId == userId){
                this._clearCurrentRemoteAssistanceData();
            }
        },
        OnRemoteAssistanceInvitationSent: function(errorCode, meetingId){
            var userId = this.getUserIdByMeetingId(meetingId);
            if(errorCode != uc.ErrorCode.Success && userId == this.inProgressRemoteAssistanceUserId){
                this._showSendRemoteAssistanceInvitationFailed();
            }
        },
        OnRemoteAssistanceAcceptMsgSent: function(errorCode, meetingId){

        },
        OnRemoteAssistanceRejectMsgSent: function(errorCode, meetingId){

        },
        _showStartRemoteAssistanceFailed: function(){
            var _this = this;
            if(this.isHost){
                uc.modules.component.PromptMessageBox.open({title: uc_resource.WindowTitle.Notification,
                    message: uc_resource.Message.StartRemoteAssistanceFailed ,
                    OkText: uc_resource.Common.ReStart,
                    CancelText: uc_resource.Common.Known,
                    buttonMode: "both"}, function (result){
                    if(result == 0){
                        _this.startRemoteAssistance(_this.inProgressRemoteAssistanceUserId);
                        this.close();
                    }else {
                        _this._clearCurrentRemoteAssistanceData();
                        this.close();
                    }
                });
            }else {
                uc.modules.component.PromptMessageBox.open({title: uc_resource.WindowTitle.Notification,
                    message: uc_resource.Message.JoinRemoteAssistanceFailed ,
                    OkText: uc_resource.Common.Known,
                    buttonMode: "ok"}, function (result){
                    if(result == 0){
                        _this._clearCurrentRemoteAssistanceData();
                        this.close();
                    }
                });
            }

        },
        _showJoinRemoteAssistanceFailed: function(meetingStartUrl, meetingId, password){
            var _this = this;
            if(this.isHost){
                uc.modules.component.PromptMessageBox.open({title: uc_resource.WindowTitle.Notification,
                    message: uc_resource.Message.StartRemoteAssistanceFailed ,
                    OkText: uc_resource.Common.ReStart,
                    CancelText: uc_resource.Common.Known,
                    buttonMode: "both"}, function (result){
                    if(result == 0){
                        _this._startMeetingClient(meetingStartUrl);
                        this.close();
                    }else {
                        _this._clearCurrentRemoteAssistanceData();
                        this.close();
                    }
                });
            }else {
                uc.modules.component.PromptMessageBox.open({title: uc_resource.WindowTitle.Notification,
                    message: uc_resource.Message.JoinRemoteAssistanceFailed ,
                    OkText: uc_resource.Common.ReJoin,
                    CancelText: uc_resource.Common.Known,
                    buttonMode: "both"}, function (result){
                    if(result == 0){
                        if(meetingStartUrl){
                            _this._startMeetingClient(meetingStartUrl);
                        }else {
                            if(meetingId && password){
                                _this._joinRemoteAssistance(_this.inProgressRemoteAssistanceUserId, meetingId, password);
                            }else {
                                var remoteAssistanceInfo = _this.getRemoteAssistanceChatInfo(_this.inProgressRemoteAssistanceUserId);
                                if(remoteAssistanceInfo){
                                    _this._joinRemoteAssistance(remoteAssistanceInfo.userId, remoteAssistanceInfo.meetingId, remoteAssistanceInfo.attendeePassword);
                                }
                            }
                        }

                        this.close();
                    }else {
                        _this._clearCurrentRemoteAssistanceData();
                        this.close();
                    }
                });
            }
        },
        _showSendRemoteAssistanceInvitationFailed: function(){
            var _this = this;
            uc.modules.component.PromptMessageBox.open({title: uc_resource.WindowTitle.Notification,
                message: uc_resource.Message.SendRemoteAssistanceInvitationFailed ,
                OkText: uc_resource.Common.ReSend,
                CancelText: uc_resource.Common.Known,
                buttonMode: "both"}, function (result){
                if(result == 0){
                    _this.sendRemoteAssistanceInvitation();
                    this.close();
                }else {
                    _this._clearCurrentRemoteAssistanceData();
                    this.close();
                }
            });
        },
        joinRemoteAssistance: function(userId, meetingId, password, meetingStartUrl, isHost){
            var _this = this;
            this.isHost = isHost;
            //If is in remote assistance, show confirm dialog
            if(this.inProgressRemoteAssistanceUserId != 0 && !this.isHost){
                uc.modules.component.PromptMessageBox.open({title: uc_resource.WindowTitle.Notification,
                    message: uc_resource.Message.InRemoteAssistanceForJoin ,
                    OkText: uc_resource.Common.OK,
                    CancelText: uc_resource.Common.Cancel,
                    buttonMode: "both"}, function (result){
                    if(result == 0){
                        _this._joinRemoteAssistance(userId, meetingId, password, meetingStartUrl);
                        this.close();
                    }else {
                        this.close();
                    }
                });
            }else {
                this._joinRemoteAssistance(userId, meetingId, password, meetingStartUrl);
            }
        },
        _joinRemoteAssistance: function(userId, meetingId, password, meetingStartUrl){
            if(!userId){
                this.getUserIdByMeetingId(meetingId);
            }
            //set in progress and remove the banner
            this.inProgressRemoteAssistanceUserId = userId;
            uc.modules.remoteAssistance.RemoteAssistanceBanner.removeRemoteAssistanceBanner();

            //set status to in progress
            this.updateRemoteAssistanceStatus(userId, uc.constants.RemoteAssistanceStatus.IN_ASSISTANCE);

            if(meetingStartUrl){
                this._startMeetingClient(meetingStartUrl);
            }else {
                var _this = this;
                _this.joiningRemoteAssistnceId = meetingId;
                var meetingStartUrl =   uc.IMeetingService.getMeetingStartURLSync(meetingId, password);
                if(!meetingStartUrl.startURL){
                    _this._showJoinRemoteAssistanceFailed(null, meetingId, password);
                }
                else{
                    this._startMeetingClient(meetingStartUrl.startURL);
                }
                //uc.IMeetingService.getMeetingStartURL(meetingId, password, function(args){
                //    var errorCode = args[0];
                //    if(errorCode != uc.ErrorCode.Success){
                //
                //    }
                //})
            }
        },
        getUserIdByMeetingId: function(meetingId){
            for(var userId in this.remoteAssistanceInfo){
                var singleRemoteAssistanceInfo = this.remoteAssistanceInfo[userId];
                if(singleRemoteAssistanceInfo && singleRemoteAssistanceInfo.meetingId == meetingId){
                    return parseInt(userId);
                }
            }
        },
        _startMeetingClient: function(meetingStartUrl){
            var _this = this;
            uc.IMeetingService.startMeetingClient(meetingStartUrl, function(args){
                var errorCode = args[0];
                if(errorCode != uc.ErrorCode.Success){
                    _this._showJoinRemoteAssistanceFailed(meetingStartUrl);
                }else {
                    if(_this.isHost){
                        _this.sendRemoteAssistanceInvitation();
                    }
                }
            });
        },
        _clearCurrentRemoteAssistanceData: function(){
            delete  this.remoteAssistanceInfo[this.inProgressRemoteAssistanceUserId];
            this.inProgressRemoteAssistanceUserId = 0;
            this.isHost = false;
        }
    }
})(uc);