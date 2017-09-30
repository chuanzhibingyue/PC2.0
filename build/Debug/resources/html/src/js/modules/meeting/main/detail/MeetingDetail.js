/**
 * Created by waouyang on 16/4/26.
 */
(function (uc) {
    var _this = null;
    uc.modules.meeting.MeetingDetail = {
        meetingInfo: null,
        isFromMeetingModule: true,

        init: function () {
            _this = this;

            this.nodeHtml =
                '<div class="today-panel hide"><div class="today-background"><div class="today-message"><span>' + uc_resource.Meeting.TodayNoMeeting + '</span>,' + uc_resource.Meeting.TodayIsRelax + '</div><div class="today-meeting">' + uc_resource.Meeting.CreateCalendar + '?</div><div class="today-meeting-bar"><a class="btn today-meeting-btn">' + uc_resource.Meeting.CreateCalendar + '</a></div></div></div>'
                + '<div class="meeting-panel hide">'
                + '    <div class="top"><h1 class="head text-selectable"></h1><div></div><span class="status"></span></div>'
                + '    <div class="attendee-status">'
                + '        <a action="accept" class="attendee-cancel">' + uc_resource.Meeting.Accept + '</a>'
                + '        <a action="reject" class="attendee-reject">' + uc_resource.Meeting.Reject + '</a>'
                + '    </div>'
                + '    <div class="functions-bar">'
                + '        <span class="time"></span>'
                + '        <a href="javascript:void(0);" class="function-btn detail">' + uc_resource.Chat.ViewDetail + '</a>'
                + '        <a href="javascript:void(0);" action="more" qtips="' + uc_resource.Meeting.MeetingMore + '" class="function-btn more icon-more"></a>'
                + '       <a href="javascript:void(0);" action="forward" qtips="' + uc_resource.Menu.ForwardMeeting + '" class="function-btn forward icon-forward"></a>'
                + '        <a href="javascript:void(0);" action="members" qtips="' + uc_resource.Meeting.Members + '" class="function-btn Members icon-meeting-attendee"></a>'
                + '        <a href="javascript:void(0);" action="file" qtips="' + uc_resource.Meeting.MeetingSummary + '" class="function-btn minutes icon-meeting-file"></a>'
                + '        <a href="javascript:void(0);" action="chat" qtips="' + uc_resource.Meeting.MeetingChat + '" class="function-btn icon-meeting-chat"></a>'
                + '        <div class="function-pop hide">'
                + '            <a href="javascript:void(0);" action="edit" class="function-pop-menu">' + uc_resource.Meeting.EditMeeting + '</a>'
                + '            <a href="javascript:void(0);" action="cancel" class="function-pop-menu">' + uc_resource.Meeting.CancelMeeting + '</a>'
                + '        </div>'
                + '    </div>'
                + '    <div class="footer-panel">'
                + '        <div class="password-area text-selectable"><p class="host-password">' + uc_resource.Meeting.HostPassword + '<input id="host-password" disabled="disabled" /><a class="copy-password-button">' + uc_resource.Meeting.CopyHostPassword + '</a></p><p class="attendee-password">' + uc_resource.Meeting.AttendeePassword + '<input id="attendee-password" disabled="disabled" /><a class="copy-password-button">' + uc_resource.Meeting.CopyAttendeePassword + '</a></p></div>'
                + '        <div class="icon-password password-btn"><span>&nbsp;' + uc_resource.Meeting.MeetingPassword + '</span></div>'
                + '        <a href="javascript:void(0);" action="join" class="join btn-uikit btn-dark"><img src="../images/loading.png"><span>' + uc_resource.Meeting.JoinMeeting + '</span></a>'
                + '    </div>'
                + '    <div class="sub-panel detail-panel"></div>'
                + '    <div class="sub-panel chat-panel">'
                + '        <div class="sub-panel-head">'
                + '            <div class="left"><i class="icon-back" qtips="' + uc_resource.Common.Back + '"></i><span>' + uc_resource.Meeting.MeetingChatTitle + '</span></div>'
                + '            <div class="right hide"><span class="add-attendee"><i class="icon-meeting-add-attendee"></i><span>' + uc_resource.Meeting.AddMember + '</span></span></div>'
                + '        </div> '
                + '    </div>'
                + '    <div class="sub-panel minutes-panel">'
                + '        <div class="sub-panel-head">'
                + '            <div class="left"><i class="icon-back" qtips="' + uc_resource.Common.Back + '"></i><span>' + uc_resource.Meeting.MeetingSummary + '</span></div>'
                + '            <div class="right"><span class="meeting-upload-file"><i class="icon-chattool-upload"></i><span>' + uc_resource.Meeting.MeetingUpload + '</span></span></div>'
                + '        </div> '
                + '        <div class="fileList-panel">'
                + '           <div class="fileList meetingPanel-fileList"></div>'
                + '        </div>'
                + '    </div>'
                + '    <div class="sub-panel members-panel">'
                + '        <div class="sub-panel-head">'
                + '            <div class="left"><i class="icon-back" qtips="' + uc_resource.Common.Back + '"></i><span>' + uc_resource.Meeting.AttendeeHeader + '</span></div>'
                + '            <div class="right hide"><span class="add-attendee"><i class="icon-meeting-add-attendee"></i><span>' + uc_resource.Meeting.AddMember + '</span></span></div>'
                + '        </div> '
                + '    </div>'
                + '</div>';

            this.meetingInfoNode =
                '<div class="meeting-detail">'
                + '    <div class="location-time">'
                + '        <div class="left">'
                + '            <div class="meeting-icon host-icon"></div>'
                + '            <div class="min-title-value meeting-host">' + uc_resource.Meeting.HostName + ': <span class="host-name"></span></div>'
                + '       </div>'
                + '        <div class="location-notice hide">' + uc_resource.Meeting.MeetingConflict + '</div>'
                + '    </div>'
                + '    <div class="location-time">'
                + '        <div class="meeting-icon time-icon"></div>'
                + '        <div class="min-title-value meeting-time">&nbsp;</div>'
                + '    </div>'
                + '    <div class="location-area hide">'
                + '        <div class="meeting-icon location-icon"></div>'
                + '        <div class="min-title-value  text-selectable"></div><div class="location-map">'+ uc_resource.Meeting.ViewOnMap +'</div>'
                + '    </div>'
                + '    <div class="remark-area hide">'
                + '        <div class="meeting-icon remark-icon"></div>'
                + '        <pre class="remark text-selectable"></pre>'
                + '    </div>'
                + '</div>';

            this.clear();
            this.initUI();
            this.bindEvent();
        },

        clear: function () {
            uc.ui.Meeting.ContentPanel.find('.today-panel, .meeting-panel').remove();
            //uc.ui.Meeting.ContentPanel.find('.today-panel, .meeting-panel').off();
        },

        initUI: function () {
            this.node = uc.ui.Meeting.ContentPanel.append(this.nodeHtml);
            this.noneNode = uc.ui.Meeting.ContentPanelBody;
            this.detailPanelNode = $('.meeting-panel', this.node);
            this.meetingDetailNode = $('.detail-panel', this.node);//meeting detail
            this.meetingDetailNode.append(this.meetingInfoNode);
            this.meetingTitle = $('.top .text-selectable', this.node);//meeting title
            this.meetingHost = $('.location-time', this.meetingDetailNode);//meeting host info
            this.meetingAddress = $('.location-area', this.meetingDetailNode);//meeting address
            this.meetingShowMap = $('.location-map', this.meetingDetailNode);//meeting address show map
            this.meetingMark = $('.remark-area', this.meetingDetailNode);//meeting mark
            this.passwordPop = $('.password-area', this.node);//meeting password
            this.btnPassword = $('.password-btn', this.node);//meeting password btn
            this.moreAction = $('.function-pop', this.node);//meeting more
            this.btnJoin = $('.join', this.node);
            this.btnChat = $('.icon-meeting-chat', this.node);
            this.btnFile = $('.icon-meeting-file', this.node);
            this.chatNode = $('.chat-panel', this.node);
            this.minutesNode = $('.minutes-panel', this.node);
            this.fileListNode = $('.meetingPanel-fileList', this.node);
            this.attendeeNode = $('.members-panel', this.node);
            this.statusNode = $('.status', this.node);
            this.attendeeStatusNode = $('.attendee-status', this.node);
        },

        bindEvent: function () {
            this.node.unbind().on('click', 'a[action]', function () {
                var $this = $(this);
                var action = $this.attr('action');

                switch (action) {
                    case 'members':
                        _this._showAttendees();
                        break;
                    case 'forward':
                        _this._forwardMeeting();
                        break;
                    case 'more':
                        _this._moreMeeting();
                        break;
                    case 'join':
                        _this._joinMeeting($this);
                        break;
                    case 'chat':
                        _this._meetingChat();
                        break;
                    case 'file':
                        _this._uploadMeetingFile();
                        break;
                    case 'accept':
                        _this._acceptMeeting();
                        break;
                    case 'reject':
                        _this._rejectMeeting();
                        break;
                    case 'edit':
                        _this._editMeeting();
                        break;
                    case 'cancel':
                        _this._cancelMeeting();
                        break;
                }
            });

            //view on map
            this.node.on('click', '.location-map', function () {
                var locationObj = _this.meetingInfo.extrasInfo;
                uc.IUIService.showLocation(locationObj);
            });

            //panel back
            this.node.on('click', 'i.icon-back', function (e) {
                var panel = $(this).parents('.sub-panel');
                panel.hide();

                //reset meeting chat
                if (panel.hasClass('chat-panel')) {
                    _this._hideMeetingChat();
                }

                //reset meeting file panel
                if (panel.hasClass('minutes-panel')) {
                    _this._hideMeetingFileList();
                }
            });

            //add attendee
            this.node.on('click', 'span.add-attendee', function (e) {
                _this._forwardMeeting();
            });

            //show meeting password
            this.node.on('click', '.password-btn', function (e) {
                _this.passwordPop.show();
                _this._hideMoreAction();
                e.stopPropagation();
            });

            //copy meeting password
            this.node.on('click', '.copy-password-button', function (e) {
                var password = $(this).prev()[0];
                password.setSelectionRange(0, password.value.length);
                password.focus();

                document.execCommand("copy");
                _this._hidePassword();
            });

            //upload file
            this.node.on('click', '.meeting-upload-file', function () {
                uc.modules.meeting.MeetingFileManager.uploadMeetingFile();
            });

            //body click
            uc.util.PageUtil.addBodyClick('meetingPanel', this.node, function (e) {
                _this._hideAllPanel(e);
            });
        },

        _hideAllPanel: function (e) {

            if ((!$(e.target).closest(".members-panel").length) &&
                (!$(e.target).closest(".icon-meeting-attendee").length) &&
                (!$(e.target).closest(".chat-panel").length) &&
                (!$(e.target).closest(".icon-meeting-chat").length) &&
                (!$(e.target).closest(".password-area").length) &&
                (!$(e.target).closest(".icon-more").length) &&
                (!$(e.target).closest(".minutes-panel").length) &&
                (!$(e.target).closest(".icon-meeting-file").length) &&
                (!$(e.target).closest(".more").length) &&
                (!$(e.target).closest(".record-more").length) &&
                (!$(e.target).closest(".mumberMessage").length) &&
                (!$(e.target).closest("#modalWin").length) &&
                (e.target.localName != "body")) {
                this._hideAttendees();
                this._hideMeetingFileList();
                this._hideMeetingChat();
                this._hidePassword();
                this._hideMoreAction();
                this._hidePop();
            }

            if (($(e.target).closest(".fileList").length) && (!$(e.target).closest(".more").length && !$(e.target).closest(".record-more").length)) {
                this._hidePop();
            }
        },

        _showAttendees: function () {
            uc.modules.meeting.MeetingAttendeeManager.showMeetingAttendee(this.attendeeNode, this.meetingInfo);
            this._showPanel(this.attendeeNode);
            this._hideMoreAction();
        },

        _hideAttendees: function () {
            this.attendeeNode.hide();
            uc.modules.meeting.MeetingAttendeeManager.hideMeetingAttendee();
        },

        _hideMeetingChat: function () {
            this.chatNode.hide();
            this.chatNode.children('div:not(".sub-panel-head")').remove();
            uc.modules.meeting.MeetingChatManager.hideMeetingChat();
        },

        _hideMeetingFileList: function () {
            this.minutesNode.hide();
            this.minutesNode.find('.meetingPanel-fileList').empty();
            uc.modules.meeting.MeetingFileManager.hideMeetingFileList();
        },

        _hidePassword: function () {
            this.passwordPop.hide(0);
        },

        _hideMoreAction: function () {
            this.moreAction.addClass('hide');
        },

        _hidePop: function () {
            this.node.find('.menu').hide();
        },

        _forwardMeeting: function () {
            var selectedUserList = [], self = this;
            var _shareUserId = this.getAuthorizerId();

            self._hideMoreAction();
            selectedUserList.push(this.meetingInfo.hostId);
            if ($.isArray(this.meetingInfo.attendees) && this.meetingInfo.attendees.length > 0) {
                $.each(this.meetingInfo.attendees, function (key, attendees) {
                    if (attendees.type == uc.constants.Meeting.AttendeeType.User) {
                        selectedUserList.push(parseInt(attendees.account));
                    } else {
                        selectedUserList.push(attendees.account);
                    }
                });
            }

            uc.util.StorageUtil.setData('ForwardUserIds:', JSON.stringify({data: selectedUserList}));

            uc.IUIService.openChooseContactWindow({
                key: "MeetingForward_" + new Date().getTime(),
                fromForward: true,
                selectedUserIdList: [],
                isMeetingForward: true
            }, function (args) {
                if (args.okOrCancel) {
                    var userIds = JSON.parse(uc.util.StorageUtil.getData('ForwardUserIds:')).data;
                    var persons = [], isRepeat = false;

                    $.each(args.contacts, function (key, attendees) {
                        var type = attendees.userId ? uc.constants.Meeting.AttendeeType.User : uc.constants.Meeting.AttendeeType.Mail;
                        if (userIds.indexOf(attendees.userId) >= 0 || attendees.userId == self.meetingInfo.hostId) {
                            isRepeat = true;
                            return true;
                        }
                        persons.push({
                            account: (attendees.userId || attendees.account) + "", // 账号
                            type: type,                                              // 参会人类型：1 内部  2 邮件  3 电话
                            status: 0,                                               // 参会人状态：0 接受  1 拒绝  2 未处理
                            name: attendees.displayName                             // 参会人名称
                        });
                    });

                    uc.util.LogUtil.info("MeetingDetail", "_forwardMeeting", "forwardMeeting", {
                        meetingInfo: self.meetingInfo,
                        persons: persons,
                        shareUserId: _shareUserId,
                        args: args
                    });

                    if (persons.length) {
                        uc.IMeetingService.forwardMeeting(self.meetingInfo.eventId, persons, 0, _shareUserId, 1, 1);
                    } else {
                        var msg = uc_resource.Meeting.AddForwardUser;

                        if (isRepeat) {
                            msg = uc_resource.Meeting.NotForwardRepeat;
                        }

                        uc.modules.component.PromptMessageBox.open({
                            title: uc_resource.WindowTitle.Notification,
                            message: msg,
                            buttonMode: "ok",
                            OkText: uc_resource.Common.OK
                        }, function (result) {
                            this.close();
                        });
                    }
                }
            });
        },

        _moreMeeting: function () {
            this.moreAction.toggleClass('hide');
        },

        _joinMeeting: function (self) {
            if (self.hasClass('joining')) {
                return;
            }
	        uc.IUIService.joinMeeting(this.meetingInfo, this.isFromMeetingModule, true);
        },

        showMeetingChat: function(){
            this._meetingChat();
        },

        _meetingChat: function () {
            uc.modules.meeting.MeetingChatManager.showMeetingChat(this.chatNode, this.meetingInfo);
            this._showPanel(this.chatNode);
            this._hideMoreAction();
        },

        _uploadMeetingFile: function () {
            uc.modules.meeting.MeetingFileManager.showMeetingFileList(this.minutesNode, this.fileListNode, this.meetingInfo);
            this._showPanel(this.minutesNode);
            this._hideMoreAction();
        },

        _acceptMeeting: function () {
            uc.IMeetingService.acceptMeetingInvitation(
                this.meetingInfo.eventId, 0,
                this.getAuthorizerId(),
                function(args) {
                    uc.util.LogUtil.info("MeetingDetail", "_setAttendeeStatus", "acceptMeeting", {errorCode:args[0]});
                    if (args[0] == uc.ErrorCode.Success) {
                        _this.meetingInfo = uc.IMeetingService.getMeetingInfoByIdSync(_this.meetingInfo.eventId, 0, 0, _this.getAuthorizerId())[0];
                        _this.showMeetingStatus();
                    }
                }
            );
        },

        _rejectMeeting: function () {
            uc.IMeetingService.rejectMeetingInvitation(
                this.meetingInfo.eventId, 0,
                this.getAuthorizerId(),
                function(args) {
                    uc.util.LogUtil.info("MeetingDetail", "_setAttendeeStatus ", "rejectMeeting", {errorCode:args[0]});
                    if (args[0] == uc.ErrorCode.Success) {
                        _this.meetingInfo = uc.IMeetingService.getMeetingInfoByIdSync(_this.meetingInfo.eventId, 0, 0, _this.getAuthorizerId())[0];
                        _this.showMeetingStatus();
                    }
                }
            );
        },

        _editMeeting: function () {
            var param = {
                shareUserId: this.getAuthorizerId(),
                meetingInfo: this.meetingInfo,
                type: uc.constants.Meeting.MeetingCreateType.Default,
                isCreate: false
            };

            uc.IUIService.createMeeting("EditMeeting", param);
        },

        _cancelMeeting: function () {
            uc.modules.component.PromptMessageBox.open({
                title: uc_resource.WindowTitle.Notification,
                message: uc_resource.Meeting.CanceledMeeting,
                CancelText: uc_resource.Common.Cancel,
                buttonMode: "both",
                OkText: uc_resource.Common.OK
            }, function (result) {
                if (result) {
                    this.close();
                } else {
                    this.close();
                    uc.IMeetingService.cancelMeeting(
                        _this.meetingInfo.eventId,
                        0,
                        _this.getAuthorizerId(),
                        1,
                        1,
                        function (args) {
                            uc.util.LogUtil.info("MeetingDetail", "_cancel", "meetingInfo", {
                                    errorCode: args[0],
                                    meetingInfo: _this.meetingInfo
                                });
                        });
                }
            });
        },

        _showPanel: function (panel) {
            panel.css({left: "850px", opacity: "0.3"});
            panel.show();
            panel.animate({"left": "70px", opacity: "1"}, 400, "linear", function () {});
        },

        _dealMeetingError: function (errorCode) {
            var errorName = "ERROR_CODE_" + errorCode;
            var errorMeg = uc_resource.Meeting.ErrorMessage[errorName] || uc_resource.Meeting.ErrorMessage["ERROR_DEFAULT"];

            uc.modules.component.PromptMessageBox.open({
                title: uc_resource.WindowTitle.Notification,
                message: errorMeg,
                buttonMode: "ok",
                OkText: uc_resource.Common.OK
            }, function (result) {
                this.close();
            });
        },

        updateMeetingInfo: function (meetingInfo) {
            if (this.meetingInfo && meetingInfo.eventId == this.meetingInfo.eventId) {
                this.meetingInfo = meetingInfo;
                uc.modules.meeting.MeetingAttendeeManager.updateMeeting(meetingInfo);
                this.refresh();
            }
        },

        cancelMeeting: function (eventId) {
            if (this.meetingInfo && this.meetingInfo.eventId == eventId) {
                this.showNoneMeeting();
            }
        },

        refresh: function () {
            if (this.meetingInfo) {
                this.showDetail();
            } else {
                this.showNoneMeeting();
            }
        },

        showDetail: function () {
            this.noneNode.addClass('hide');
            this.detailPanelNode.removeClass('hide');
            this.showMeetingTitle();
            this.showChat();
            this.showUploadFile();
            this.showMeetingHost();
            this.showMeetingAddress();
            this.showMeetingMark();
            this.showMeetingJoin();
            this.showMoreAction();
            this.showMeetingStatus();
            this.setMeetingPassword();
            this.showMeetingPasswordStatus();
            this.showIsGNet();
        },

        showNoneMeeting: function () {
            this.meetingInfo = null;
            this.noneNode.removeClass('hide');
            this.detailPanelNode.addClass('hide');
        },

        showMeetingTitle: function () {
            this.meetingTitle.text(this.meetingInfo.title).attr('qtips', this.meetingInfo.title);
        },

        showChat: function () {
            if (this.getAuthorizerId() != uc.constants.Meeting.AuthorizeMySelf) {
                this.btnChat.addClass('hide');
                this.btnFile.addClass('hide');
            } else {
                this.btnChat.removeClass('hide');
                this.btnFile.removeClass('hide');
            }
        },

        showUploadFile: function () {

        },

        showMeetingHost: function () {
            this.meetingHost.find('.host-name').text(this.meetingInfo.hostName);
            this.meetingHost.find('.meeting-time').html(uc.util.TimeUtil.showMeetingTime(this.meetingInfo, uc.settings.meeting.timeZone.offset));
        },

        showMeetingAddress: function () {
            var address = this.getMeetingAddress();
            if (address && address.location) {
                this.meetingAddress.removeClass('hide');
                this.meetingAddress.find('.text-selectable').text(address.location);

                if (address.type == uc.constants.Meeting.AddressType.Out) {
                    this.meetingShowMap.removeClass('hide');
                } else {
                    this.meetingShowMap.addClass('hide');
                }
            } else {
                this.meetingAddress.addClass('hide');
            }
        },

        showMeetingMark: function () {
            if (this.meetingInfo.summary) {
                this.meetingMark.removeClass('hide');
                this.meetingMark.find('.text-selectable').text(this.meetingInfo.summary);
            } else {
                this.meetingMark.addClass('hide');
            }
        },

        showMeetingJoin: function () {
            if (this.getAuthorizerId() != uc.constants.Meeting.AuthorizeMySelf || !this.meetingInfo.isGNet) {
                this.btnJoin.addClass('hide');
            } else {
                this.btnJoin.removeClass('hide');
            }
        },

        showMoreAction: function () {
            var currentUserInfo = uc.IContactService.getCurrentUserInfo();
            var authorizerId = this.getAuthorizerId();
            if (this.getAuthorizerId() == uc.constants.Meeting.AuthorizeMySelf) {
                this.showMoreActionBtn(currentUserInfo.userId);
            } else {
                this.showMoreActionBtn(authorizerId);
            }
        },

        showMeetingStatus: function () {
            var attendee = this.getCurrentAttendee();
            if (attendee.isHost) {
                this.showHostStatus();
            } else {
                this.showAttendeeStatus(attendee);
            }
        },

        showHostStatus: function () {
            this.statusNode.addClass('hide');
            this.attendeeStatusNode.addClass('hide');
        },

        showAttendeeStatus: function (attendee) {
            if (attendee) {
                switch (attendee.status) {
                    case uc.constants.Meeting.AttendeeStatus.Pending:
                        this.showPending();//未处理状态
                        break;
                    case uc.constants.Meeting.AttendeeStatus.Reject:
                        this.showReject();//拒绝状态
                        break;
                    case uc.constants.Meeting.AttendeeStatus.Accepted:
                        this.showAccepted();//接受状态
                        break;
                }
            }
        },

        showPending: function () {
            this.statusNode.addClass('hide');
            this.attendeeStatusNode.removeClass('hide');
        },

        showReject: function () {
            this.statusNode.text(uc_resource.Meeting.Rejected).removeClass('hide');
            this.attendeeStatusNode.addClass('hide');
        },

        showAccepted: function () {
            this.statusNode.text(uc_resource.Meeting.Accepted).removeClass('hide');
            this.attendeeStatusNode.addClass('hide');
        },

        setMeetingPassword: function () {
            this.passwordPop.find('#host-password').val(this.meetingInfo.hostPassword);
            this.passwordPop.find('#attendee-password').val(this.meetingInfo.attendeePassword);
        },

        showMeetingPasswordStatus: function () {
            var currentUserInfo = uc.IContactService.getCurrentUserInfo();
            var authorizerId = this.getAuthorizerId();
            var userId = authorizerId ? authorizerId : currentUserInfo.userId;
            if (userId == this.meetingInfo.hostId) {
                this.passwordPop.addClass('is-host');
            } else {
                this.passwordPop.removeClass('is-host');
            }
        },

        showIsGNet: function () {
            if (!this.meetingInfo.isGNet) {
                this.btnPassword.addClass('hide');
            }else {
                this.btnPassword.removeClass('hide');
            }
        },

        showMoreActionBtn: function (authorizerId) {
            if (this.meetingInfo.hostId != authorizerId) {
                this.node.find('a[action="more"]').addClass('hide');
            } else {
                this.node.find('a[action="more"]').removeClass('hide');
            }
        },

        showMeetingDetail: function (meetingInfo, isFromMeetingList) {
            this.meetingInfo = meetingInfo;
            this.isFromMeetingList = isFromMeetingList;
            this.refresh();
        },

        getCurrentAttendee: function () {
            var currentUserInfo = uc.IContactService.getCurrentUserInfo();
            var authorizerId = this.getAuthorizerId();
            var currentUserId = (authorizerId != uc.constants.Meeting.AuthorizeMySelf ? authorizerId :currentUserInfo.userId);

            if (authorizerId && this.meetingInfo.hostId == authorizerId) {
                return  {isHost: true};
            }

            if (this.meetingInfo) {
                for (var i = 0; i < this.meetingInfo.attendees.length; i++) {
                    var attendee = this.meetingInfo.attendees[i];
                    if (currentUserId == parseInt(attendee.account)) {
                        return attendee;
                    }
                }
            }

            return {isHost: true};
        },

        getMeetingAddress: function () {
            var address = {location: this.meetingInfo.location};

            /*
            *  这里要兼容旧地址数据，如果有拓展字段，认为是新版本，否则都取location
            */
            if (this.meetingInfo.extrasInfo) {
                var locationObj = JSON.parse(this.meetingInfo.extrasInfo);
                if (typeof locationObj == 'object') {
                    address = locationObj.address;
                }
            }

            return address;
        },

        getAuthorizerId: function () {
            return uc.modules.meeting.MeetingListManager.getAuthorizerId();
        },

        OnWaitJoinMeeting: function () {
            this.btnJoin.addClass('joining');
            this.btnJoin.find('span').text(uc_resource.Meeting.JoiningMeeting);
        },

        OnMeetingJoined: function () {
            this.btnJoin.removeClass('joining disabled');
            this.btnJoin.find('span').text(uc_resource.Meeting.JoinMeeting);
        },

        OnJoinMeetingFailed: function (errorCode) {
            this._dealMeetingError(errorCode);
            this.OnMeetingJoined();
        },

	    OnCheckJoinMeetingRoom: function (errorCode, roomId, hasPassword, isValid, mayJoin) {
		    if(!this.meetingInfo){
			    return;
		    }
		    if (errorCode == uc.ErrorCode.Success && isValid  && mayJoin && this.meetingInfo.roomId == roomId) {

		    } else {
			    //会议无效，更改入会按钮状态
			    this.OnMeetingJoined();
		    }
	    }
    }
})(uc);