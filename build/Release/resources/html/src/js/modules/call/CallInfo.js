/**
 * Created by waouyang on 15/12/11.
 */
(function(uc){
    uc.modules.call.CallInfo = function() {
        this.chatId = 0;
        this.siteId = 0;
        this.chatType = uc.constants.Chat.ChatType.SingleChat;
        this.callStatus = uc.constants.Call.CallStatus.DISCONNECTED;
        this.muteStatus = uc.constants.Call.MuteStatus.UN_MUTE;
        this.videoStatus = uc.constants.Call.VideoStatus.CLOSE;
	    //共享者的状态
	    this.desktopShareStatus = uc.constants.Call.DesktopShareStatus.CLOSE;
	    //共享接接受人的状态
	    this.desktopShareAcceptStatus = uc.constants.Call.DesktopShareStatus.CLOSE;

        this.audioType = uc.constants.Call.AudioType.VOIP;
        this.phoneNumber = '';
        this.userList = {};
        this.callMeetingId = 0;
        this.hostId = 0;
        this.loginUserId = 0;
        this.isInProgress = false;
        this.callConnectedTime = 0;
        this.handleBtn='';
    };
    jQuery.extend(uc.modules.call.CallInfo.prototype, {
        update: function(callInfo){
            this.chatId = callInfo.chatId ? callInfo.chatId : this.chatId;
            this.chatType = callInfo.chatType != undefined && callInfo.chatType != null ? callInfo.chatType : this.chatType;
            this.callStatus = callInfo.callStatus != undefined && callInfo.callStatus != null ? callInfo.callStatus : this.callStatus;
            this.muteStatus = callInfo.muteStatus != undefined && callInfo.muteStatus != null ? callInfo.muteStatus : this.muteStatus;
            this.videoStatus = callInfo.videoStatus != undefined && callInfo.videoStatus != null ? callInfo.videoStatus : this.videoStatus;
            this.audioType = callInfo.audioType != undefined && callInfo.audioType != null ? callInfo.audioType : this.audioType;
            this.phoneNumber = callInfo.phoneNumber != undefined && callInfo.phoneNumber != null ? callInfo.phoneNumber : this.phoneNumber;
            this.userList = callInfo.userList != undefined && callInfo.userList != null ? callInfo.userList : this.userList;
            this.callMeetingId = callInfo.callMeetingId != undefined && callInfo.callMeetingId != null ? callInfo.callMeetingId : this.callMeetingId;
            this.siteId = callInfo.siteId != undefined && callInfo.siteId != null ? callInfo.siteId : this.siteId;
            this.hostId = callInfo.hostId != undefined && callInfo.hostId != null ? callInfo.hostId : this.hostId;
            this.loginUserId = callInfo.loginUserId != undefined && callInfo.loginUserId != null ? callInfo.loginUserId : this.loginUserId;
            this.setIsInProgress (callInfo.isInProgress != undefined && callInfo.isInProgress != null ? callInfo.isInProgress : this.isInProgress);
            this.callConnectedTime = callInfo.callConnectedTime != undefined && callInfo.isInProgress != null ? callInfo.callConnectedTime : this.callConnectedTime;
            this.handleBtn=callInfo.handleBtn != undefined && callInfo.handleBtn != null ? callInfo.handleBtn : this.handleBtn;
	        this.desktopShareStatus = callInfo.desktopShareStatus != undefined && callInfo.desktopShareStatus != null ? callInfo.desktopShareStatus : this.desktopShareStatus;
	        this.desktopShareAcceptStatus = callInfo.desktopShareAcceptStatus != undefined && callInfo.desktopShareAcceptStatus != null ? callInfo.desktopShareAcceptStatus : this.desktopShareAcceptStatus;
        },
        reset: function(){
            this.loginUserId = uc.IContactService.getCurrentUserInfo().userId;
            this.chatId = 0;
            this.chatType = uc.constants.Chat.ChatType.SingleChat;
            this.callStatus = uc.constants.Call.CallStatus.DISCONNECTED;
            this.muteStatus = uc.constants.Call.CallStatus.UN_MUTE;
            this.videoStatus = uc.constants.Call.VideoStatus.CLOSE;
            this.userList = {};
            this.callMeetingId = 0;
            this.audioType = uc.constants.Call.AudioType.VOIP;
            this.phoneNumber = '';
            this.siteId = 0;
            this.hostId = 0;
            this.setIsInProgress(false);
            this.callConnectedTime = 0;
            this.handleBtn='';
	        this.desktopShareStatus = uc.constants.Call.DesktopShareStatus.CLOSE;
	        this.desktopShareAcceptStatus = uc.constants.Call.DesktopShareStatus.CLOSE;

        },
	    setIsInProgress: function(_isInProgress){
		    this.isInProgress = _isInProgress;
		    if( uc.modules.call.CallManager &&  uc.modules.call.CallManager.inProgressCallInfo){
			    uc.util.StorageUtil.setData('InProgressCallInfo', uc.modules.call.CallManager.inProgressCallInfo);
		    }
	    }
    })
})(uc);