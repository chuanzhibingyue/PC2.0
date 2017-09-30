/**
 * Created by waouyang on 15/12/22.
 */
(function(uc){
    var _this = null;
    uc.modules.remoteAssistance.RemoteAssistanceBanner = {
        nodeHtml:   '<div class="remote-assistance-banner">' +
                        '<span class="remote-assistance-icon"></span>'+
                        '<span class="remote-assistance-notification">'+
                            uc_resource.Message.YouAreInvitedToRemoteAssistance +
                        '</span>'+
                        '<span class="remote-assistance-join-node">' +
                            '<span class="remote-assistance-join-btn">' + uc_resource.Message.JoinRemoteAssistance + '</span>'+
                            '<span class="remote-assistance-join-indicator"></span>'+
                        '</span>'+
                    '</div>',
        init: function(remoteAssistanceInfo){
            _this = this;
            this.remoteAssistanceInfo = remoteAssistanceInfo;
            this.parentNode = $('.message-box-container', uc.ui.Message.ContentPanel);
            this.node = $(this.nodeHtml).appendTo(this.parentNode);
            this.joinNode = $('.remote-assistance-join-node', this.node);
            this._initUI(this.remoteAssistanceInfo.status);
            this.bindEvent();
        },
        _initUI: function(status){
            if(status == uc.constants.RemoteAssistanceStatus.INVITED){
                this.parentNode.addClass('has-remote-assistance-banner');
                uc.modules.chat.ChatManager.scrollMessageBoxToEnd();
            }else {
                this.parentNode.removeClass('has-remote-assistance-banner');
                uc.modules.chat.ChatManager.scrollMessageBoxToEnd();
            }
        },
        removeRemoteAssistanceBanner: function(){
            if(this.parentNode){
                this.parentNode.removeClass('has-remote-assistance-banner');
                this.node.remove();
            }
        },
        bindEvent: function(){
            this.joinNode.click(function(){
                uc.modules.remoteAssistance.RemoteAssistanceManager.joinRemoteAssistance(
                                    _this.remoteAssistanceInfo.userId,
                                    _this.remoteAssistanceInfo.meetingId,
                                    _this.remoteAssistanceInfo.attendeePassword,
                                    null,
                                    false);
            });
        }
    }
})(uc);