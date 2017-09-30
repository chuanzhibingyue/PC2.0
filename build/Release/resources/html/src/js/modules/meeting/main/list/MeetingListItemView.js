/**
 * Created by waouyang on 16/4/26.
 */
(function(uc){
    var _this = null;

    uc.modules.meeting.MeetingListItemView = function(){
        this.nodeHtml = '<div class="meeting-list-item">' +
                            '<span class="meeting-list-item-title"></span>'+
                        '</div>';
        this.meetingInfo = null;
    };

    jQuery.extend(uc.modules.meeting.MeetingListItemView.prototype, {
        init: function(options){
            _this = this;

            this.node = $(this.nodeHtml).appendTo(options.parentNode);
            this.titleNode = $('.meeting-list-item-title', this.node);
            this.bindEvent();
        },

        bindEvent: function(){
            this.node.click(uc.util.EventUtil.hitch(this, 'onItemClicked'));
            this.node.contextmenu(uc.util.EventUtil.hitch(this, 'onItemRightClicked'));
            this.node.dblclick(uc.util.EventUtil.hitch(this, 'onItemDbClicked'));
        },

        onItemClicked: function(){
            this.setMeetingSelected();
            uc.IUIService.showMeetingDetail(this.meetingInfo, true);
        },

        onItemRightClicked: function(e){
            this.onItemClicked();
            uc.IUIService.showMeetingDetail(this.meetingInfo, true);

            uc.util.MenuUtil.showMeetingContextMenu(e, this.meetingInfo);
        },

        onItemDbClicked: function(e){
            e.stopPropagation();
            this.onItemClicked();

            //Edit meeting
            var authorizerId = uc.modules.meeting.MeetingListManager.getAuthorizerId();
            var userId = authorizerId === 0 ? uc.IContactService.getCurrentUserInfo().userId : authorizerId;

            if (this.meetingInfo.hostId == userId) {
                uc.IUIService.createMeeting("EditMeeting",{
                        meetingInfo: this.meetingInfo,
                        isCreate: false
                    }
                );
            }
        },

        setMeetingInfo: function(meetingInfo){
            this.meetingInfo = meetingInfo;
            this.showMeetingView();
        },

        showMeetingView: function(){
            this.node.css({
                top: this.meetingInfo.top,
                left: this.meetingInfo.left,
                width: this.meetingInfo.width,
                height: this.meetingInfo.height
            });

            this.titleNode.removeClass('min-height');
            if(this.meetingInfo.isMinHeight){
                this.titleNode.addClass('min-height');
                this.titleNode.css({
                    lineHeight: this.meetingInfo.height + 'px'
                })
            }

            this.titleNode.text(this.meetingInfo.title);
        },

        setMeetingSelected: function(){
            if(this.node.hasClass('active')){
                return;
            }
            this.node.addClass('active').siblings().removeClass('active');
        },

        removeActiveStatus: function(){
            this.node.removeClass('active');
        },

        show: function(){
            this.node.show();
        },

        hide: function(){
            this.node.hide();
        }
    })
})(uc);