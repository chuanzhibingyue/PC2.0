/**
 * User: hougang.chen
 * Date: 16-5-5
 * Time: 下午2:05
 */
(function (uc) {
	uc.modules.meeting.room.MeetingRoomDetails = {
		init: function (roomDataMap) {

			this.detailRootNode = $('.meetingRoom');
			this.unSelectRoomTip = this.detailRootNode.find('.init-panel');
			this.detailsPanel = this.detailRootNode.find('.meetingRoomdetailspanel');

			this.roomDataMap = roomDataMap;
			this.isAdmin = false;
			this.userIds = [];
			this.bindEvents();

			//当前详情界面是否活动状态
			this.isActive = false;

			//当前是否有room正在入会
			this.isJoinMeeting = false;
			//当前正在进入的会议的roomid
			this.currentJoingMeetingRoomId = -1;

		},

		renderDetailPanel: function (roomid) {
			if (roomid == null) {
				this.unSelectRoomTip.show();
				if (this.detailsContent) {
					this.detailsContent.hide();
				}
				this.isActive = false;
			} else {
				this.isActive = true;
				this.currentRoomInfo = this.roomDataMap.getRoomInfo(roomid);
				if (!this.currentRoomInfo) {
					uc.util.LogUtil.warn('uc.modules.meeting.room.MeetingRoomDetails', 'renderDetailPanel', 'roomDataMap', this.roomDataMap);
					return false;
				}

				this.unSelectRoomTip.hide();

				if (!this.detailsContent) {

					this.detailsPanel.append($(this.createDetailsPanel()));
					this.detailsContent = this.detailRootNode.find('.detailsPanel');
				} else {
					this.detailsContent.show();
				}

				this.updateDetailsContent(this.currentRoomInfo);
			}
		},

		bindEvents: function () {
			var self = this;
			this.detailsPanel.on('click', 'a', function (e) {
				var action = $(this).attr('action');
				switch (action) {
					case "delete"://删除房间
						self.deleteMeetingRoom();
						break;
					case "edit": //编辑房间
						self.editMeetingRoom();
						break;
					case "create"://安排会议
						self.createMeeting();
						break;
					case "openAttendeeList"://打开默认参会者列表
						self.openAttendeeList();
						break;
					case "getPassword"://会议密码
						self.getPassword();
						break;
					case "join"://加入会议
						if (!($(this).hasClass('disabled'))) {
							self.joinMeeting();
						}
						break;
					case "back":
						self.removeAttendeesPanel();
						break;
					case "copyPassword": //复制密码
						self.copyPassword($(this));
						break;
				}
				e.stopPropagation();
			});

			uc.util.PageUtil.addBodyClick('meetingRoomdetailspanel', this.detailsPanel, function (e) {

				if ((!$(e.target).closest(".attendee").length) && (!$(e.target).closest(".password-area").length)) {
					self.removeAttendeesPanel();
					if (self.passwordArea && !self.passwordArea.hasClass('hide')) {
						self.passwordArea.addClass('hide');
					}
				}
			})
		},

		//鼠标事件回调函数
		deleteMeetingRoom: function () {
			var self = this;
			this.confirm(uc_resource.Meeting.meetingRoom.deleteMeetingRoomTip,function(r){
			   if(r==0){
				   uc.util.LogUtil.info("uc.modules.meeting.room.MeetingRoomDetails", "deleteMeetingRoom", " this.currentRoomInfo", self.currentRoomInfo);
				   uc.IMeetingRoomService.deleteMeetingRoom(self.currentRoomInfo.roomId, function (data) {

				   });
			   }
			},true);

		},

		editMeetingRoom: function () {
			var options = {key: "editMeetingRoom", type: uc.constants.Meeting.Room.CreateOrEditType.Edit, roomInfo: this.currentRoomInfo};
			uc.IUIService.createMeetingRoom(options, function (data) {
				//
			});
		},

		createMeeting: function () {

			var param = {
				shareUserId: uc.constants.Meeting.AuthorizeMySelf,
				type: uc.constants.Meeting.MeetingCreateType.RoomMeeting,
				roomId: this.currentRoomInfo.roomId,
				users: this.userIds
			};

			uc.IUIService.createMeeting("MeetingRoomCreateMeeting", param, function (args) {
				if (args.success) {
					uc.IUIService.changeModule(uc.constants.UI.ModuleId.Meeting);
				}
			});
		},

		openAttendeeList: function () {
			uc.modules.meeting.room.roomAttendee.MeetingRoomManager.showMeetingRoomAttendee(this.attendeesPanelListContent, this.currentRoomInfo);
			this.attendeesPanel.css({left: "850px", opacity: "0.3"});
			this.attendeesPanel.show();
			this.attendeesPanel.animate({"left": "70px", opacity: "1"}, 400, "linear", function () {
			});
		},

		getPassword: function () {
			this.passwordArea.toggleClass('hide');
		},

		copyPassword: function (_this) {
			var password = _this.prev()[0];
			password.setSelectionRange(0, password.value.length);
			password.focus();

			document.execCommand("copy");
			this.getPassword();
		},

		joinMeeting: function () {
			var self = this;
			//检测当前会议是否有效
			//如果有效，并且有密码，需要输入密码并且验证,验证通过，调用入会接口，如果没有密码，直接调用入会接口
			//如果无效，重置按钮状态
			uc.IUIService.joinMeeting(this.currentRoomInfo, false, true);

		},

		removeAttendeesPanel: function () {
			if (this.attendeesPanelListContent) {
				this.attendeesPanelListContent.find('*').off();
				this.attendeesPanelListContent.empty();
				this.attendeesPanel.hide();
				uc.modules.meeting.room.roomAttendee.MeetingRoomManager.hideMeetingRoomAttendee();
			}
		},
		createDetailsPanel: function () {
			var tempNodeStr = "";
			tempNodeStr += '<div class="detailsPanel"><div class="detailtitle"></div>' +
				'<div class="hostToolbar">' +
				'<a href="javascript:void(0)" action="delete" class="deleteMeetingRoom"><span class="icon"></span><span>' + uc_resource.Meeting.meetingRoom.deleteMeetingRoom + '</span></a>' +
				'<a href="javascript:void(0)" action="edit" class="editMeetingRoom"><span class="icon"></span><span>' + uc_resource.Meeting.meetingRoom.editMeetingRoom + '</span></a>' +
				'<a href="javascript:void(0)" action="create" class="createMeeting"><span class="icon"></span><span>' + uc_resource.Meeting.meetingRoom.createMeeting + '</span></a>' +
				'</div>' +
				'<div class="detailContent">' +
				'<div class="detailItem">' +
				'<div class="detailItemIcon host-icon"></div>' +
				'<div class="detailItemContent">' + uc_resource.Meeting.meetingRoom.host + '：<span class="hostName"></span></div>' +
				'</div>' +
				'<div class="detailItem">' +
				'<div class="detailItemIcon defaultAttendee-icon"></div>' +
				'<div class="detailItemContent">' + uc_resource.Meeting.meetingRoom.defaultAttendee + '：<span class="defaultAttendee"></span></div>' +
				'<a class="openAttendeeListBtn icon-call_video_ringht" action="openAttendeeList"></a>' +
				'</div>' +
				'<div class="detailItem">' +
				'<div class="detailItemIcon timeRange-icon"></div>' +
				'<div class="detailItemContent">' + uc_resource.Meeting.meetingRoom.timeRange + '：<span class="timeRange"></span></div>' +
				'</div>' +
				'<div class="detailItem">' +
				'<div class="detailItemIcon remark-icon"></div>' + uc_resource.Meeting.meetingRoom.remark +
				'：<pre class="remark text-selectable"></pre>' +
				'</div>' +
				'<div class="detailItem ">' +
				'<div class="detailItemIcon roomPSW-icon"></div>' +
				'<div class="detailItemContent roomPSWContent ">' + uc_resource.Meeting.meetingRoom.roomPSW + '</div>' +
				'</div>' +
				'</div>' +
				'<div class="detailFooter">' +
				'<div class="password-area text-selectable hide">' +
				'<p class="host-password">' + uc_resource.Meeting.HostPassword + '<input id="room-host-password" disabled="disabled" /><a action="copyPassword" class="copy-password-button">' + uc_resource.Meeting.CopyHostPassword + '</a></p>' +
				'<p class="attendee-password">' + uc_resource.Meeting.AttendeePassword + '<input id="room-attendee-password" disabled="disabled" /><a  action="copyPassword"  class="copy-password-button">' + uc_resource.Meeting.CopyAttendeePassword + '</a></p>' +
				'</div>' +
				'<a href="javascript:void(0)" action="getPassword" class="password-btn"><span class="icon-password"></span>&nbsp;<span>' + uc_resource.Meeting.MeetingPassword + '</span></a>' +
				'<a href="javascript:void(0);" action="join" class="join btn-uikit btn-dark"><img src="../images/loading.png"><span>' + uc_resource.Meeting.JoinMeeting + '</span></a>' +
				'</div>' +
				'<div class="attendeePanel">' +
				'<div class="title"><a action="back"><span class="icon-back" qtips="' + uc_resource.Common.Back + '"></span></a><span>' + uc_resource.Meeting.meetingRoom.defaultAttendee + '</span></div>' +
				'<div class="attendeeListContent"/>' +
				'</div></div>';

			return tempNodeStr;
		},

		updateDetailsContent: function (roomInfo) {

			if (this.title == undefined) {
				this.title = this.detailRootNode.find('.detailtitle');
				this.hostToolbar = this.detailRootNode.find('.hostToolbar');
				this.hostName = this.detailRootNode.find('.hostName');
				this.defaultAttendee = this.detailRootNode.find('.defaultAttendee');
				this.timeRange = this.detailRootNode.find('.timeRange');
				this.remark = this.detailRootNode.find('.remark');
				this.roomPSWContent = this.detailRootNode.find('.roomPSWContent');
				this.passwordArea = this.detailRootNode.find('.password-area');
				this.passwordArea.show();
				this.passwordBtn = this.detailRootNode.find('.password-btn');
				this.currentuserInfo = uc.IContactService.getCurrentUserInfo();
				this.btnJoin = this.detailRootNode.find('.join');
				this.attendeesPanel = this.detailRootNode.find('.attendeePanel');
				this.attendeesPanelListContent = this.detailRootNode.find('.attendeeListContent');
			}

			this.updateRoomDefaultAttendee();
			this.getAttendsBaseInfo(roomInfo);
			this.updateTitleAndRemark(roomInfo);
			this.updateRoomHost(roomInfo);
			this.updateTimeRange(roomInfo);
			this.updatePswContent(roomInfo);
			this.updateJoinBtnStatus();
		},

		getAttendsBaseInfo: function (roomInfo) {
			this.userIds = [];
			for (var i = 0, len = roomInfo.attendees.length; i < len; i++) {
				if (roomInfo.attendees[i].type == uc.constants.Meeting.AttendeeType.User) {
					this.userIds.push(parseInt(roomInfo.attendees[i].account));
				} else {
					this.userIds.push(roomInfo.attendees[i].account);
				}
			}
			uc.IContactService.getBasicContactInfo(this.userIds);
		},

		updateTitleAndRemark: function (roomInfo) {
			this.title.text(roomInfo.title);
			this.remark.text(roomInfo.remarks.replace(new RegExp(/\\n/ig),'\r\n'));
		},

		updateRoomHost: function (roomInfo) {

			var hostInfo = uc.modules.cache.BasicContactInfoCache.getBasicContactInfo(roomInfo.roomHostId);
			if (hostInfo) {
				this.hostName.text(hostInfo.displayName);
			} else {
				this.hostName.text(roomInfo.roomHostId);
			}

			this.updateAdminToolbar();
		},

		updateAdminToolbar: function () {
			if(!this.hostToolbar){
				return;
			}
			if (this.isAdmin) {
				this.hostToolbar.show();
				this.hostToolbar.find('a.deleteMeetingRoom').show();
				this.hostToolbar.find('a.editMeetingRoom').show();

				if (this.currentuserInfo.userId != this.currentRoomInfo.roomHostId) {
					this.hostToolbar.find('a.createMeeting').hide();
				} else {
					this.hostToolbar.find('a.createMeeting').show();
				}

			} else {
				//如果没有管理员权限，需要检测当前用户是否是主持人权限（可以安排会议），如果也不是主持人，隐藏当前工具栏
				if (this.currentuserInfo.userId != this.currentRoomInfo.roomHostId) {
					this.hostToolbar.hide();
				} else {
					this.hostToolbar.show();
					this.hostToolbar.find('a.createMeeting').show();
					this.hostToolbar.find('a.deleteMeetingRoom').hide();
					this.hostToolbar.find('a.editMeetingRoom').hide();
				}

			}
		},

		updateRoomDefaultAttendee: function (roomInfo) {
			if (!roomInfo) {
				this.defaultAttendee.text('');
				return false;
			}
			var attendeeStr = "";
			var name;
			var attendeeslen = roomInfo.attendees.length;
			if (attendeeslen == 0) {
				this.defaultAttendee.text('');
				return false;
			}
			for (var i = 0, len = attendeeslen; i < (len = len > 3 ? 3 : len); i++) {
				name = roomInfo.attendees[i].name;
				name = name == "" ? roomInfo.attendees[i].account : name;
				if (i == len - 1) {
					attendeeStr += name;
				} else {
					attendeeStr += name + ',';
				}
			}
			var attendeeFormatstr = uc.util.StringUtil.formatString(uc_resource.Meeting.meetingRoom.attendeesFormat, [attendeeStr, attendeeslen]);
			this.defaultAttendee.text(attendeeFormatstr);
		},

		updateTimeRange: function (roomInfo) {
			var timeZone = uc.settings.meeting.timeZone.offset;
			var startTimeStr = uc.util.TimeUtil.DateToString(uc.util.TimeUtil.UnixTimeToDisplayDateInfo(roomInfo.startTime, timeZone, uc.constants.UI.TimeStampUnit.Second));
			var endTimeStr = uc.util.TimeUtil.DateToString(uc.util.TimeUtil.UnixTimeToDisplayDateInfo(roomInfo.endTime, timeZone, uc.constants.UI.TimeStampUnit.Second));
			this.timeRange.text(startTimeStr + ' - ' + endTimeStr);
		},

		updatePswContent: function (roomInfo) {

			if (roomInfo.isSetPassword) {
				this.roomPSWContent.parent().show();
			} else {
				this.roomPSWContent.parent().hide();
			}

			$('#room-host-password').val(roomInfo.hostPassword);
			$('#room-attendee-password').val(roomInfo.attendeePassword);
			//如果是主持人和管理员，才可以看到的到主持人密码
			if (this.currentuserInfo.userId != this.currentRoomInfo.roomHostId && this.isAdmin) {
				this.detailRootNode.find('.host-password').show();
			} else{
				this.detailRootNode.find('.host-password').hide();
			}
		},

		updateJoinBtnStatus: function () {
			if (!this.isActive) {
				return;
			}
			//当前有会议室正在入会状态
			if (this.isJoinMeeting) {
				if (this.currentRoomInfo.roomId == this.currentJoingMeetingRoomId) {
					this.btnJoin.addClass('joining disabled');
					this.btnJoin.find('span').text(uc_resource.Meeting.JoiningMeeting);
				} else {
					this.btnJoin.addClass('disabled').removeClass('joining');
					this.btnJoin.find('span').text(uc_resource.Meeting.JoinMeeting);
				}
			} else {
				this.btnJoin.removeClass('disabled joining');
			}
		},

		resetJoinMeetingStatus: function () {
			this.isJoinMeeting = false;
			this.currentJoingMeetingRoomId = -1;
		},

		setWaitJoinMeetingStstua: function (roomId) {
			this.isJoinMeeting = true;
			this.currentJoingMeetingRoomId = roomId;
		},

		clearDetailsPanel: function (roomInfo) {

		},

		OnBasicContactInfoReceived: function (contacts) {

			if (!this.currentRoomInfo) {
				return;
			}
			var attendees = this.currentRoomInfo.attendees;
			if (contacts && contacts.length) {

				for (var i = 0; i < attendees.length; i++) {
					contacts.map(function (contact) {
						if (attendees[i].account == contact.userId || attendees[i].account == contact.account) {
							attendees[i].name = contact.displayName;
						}
					})
				}

			}

			this.updateRoomDefaultAttendee(this.currentRoomInfo);
		},

		OnMeetingRoomUpdated: function (meetingRoom) {

			var roomInfo = new uc.modules.meeting.room.info.MeetingRoomInfo();
			roomInfo.update(meetingRoom);

			this.renderDetailPanel(roomInfo.roomId);
		},

		OnWaitJoinMeetingRoomMeeting: function () {

			if (this.currentRoomInfo) {
				this.setWaitJoinMeetingStstua(this.currentRoomInfo.roomId);
			}

			if (this.isActive) {
				this.btnJoin.addClass('joining disabled');
				this.btnJoin.find('span').text(uc_resource.Meeting.JoiningMeeting);
			}

		},

		OnMeetingRoomMeetingJoined: function () {
			this.resetJoinMeetingStatus();

			if (this.isActive) {
				this.btnJoin.removeClass('joining disabled');
				this.btnJoin.find('span').text(uc_resource.Meeting.JoinMeeting);
			}
		},

		OnJoinMeetingRoomMeetingFailed: function (errorCode) {
			this.resetJoinMeetingStatus();

			if (this.isActive) {
				this.dealMeetingError(errorCode);
				this.OnMeetingRoomMeetingJoined();
			}
		},

		dealMeetingError: function (errorCode) {

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

		OnCheckJoinMeetingRoom: function (errorCode, roomId, hasPassword, isValid, mayJoin) {
			if (!this.isActive) {
				return;
			}
			if (errorCode == uc.ErrorCode.Success && isValid && mayJoin && this.currentRoomInfo && this.currentRoomInfo.roomId == roomId) {

			} else {
				//会议无效，更改入会按钮状态
				this.OnMeetingRoomMeetingJoined();
			}
		},

		OnCheckMeetingRoomPassword: function (errorCode, roomId, password, value) {

		},
		setAdmin: function (isAdmin) {
			this.isAdmin = isAdmin;
			this.updateAdminToolbar();
		},
		confirm: function (msg, fn, iscannel) {
			var cannelText = "";
			var modeTxt = "ok"
			if (iscannel) {
				modeTxt = "both";
				cannelText = uc_resource.Common.Cancel;
			}
			uc.modules.component.PromptMessageBox.open({ title: uc_resource.WindowTitle.Notification, message: msg, buttonMode: "ok", OkText: uc_resource.Common.OK, buttonMode: modeTxt, CancelText: cannelText}, function (r) {
				if (fn != null) {
					fn(r);
				}
				this.close();
			});
		}
	}
})(uc);