/**
 * User: hougang.chen
 * Date: 16-5-5
 * Time: 下午3:31
 */
(function (uc) {
	uc.modules.meeting.room.createMeetingRoom.CreateMeetingRoom = {
		init: function () {

			this.currentuserInfo = uc.IContactService.getCurrentUserInfo();
			if (!this.currentuserInfo) {
				uc.util.LogUtil.warn('uc.modules.meeting.room.createMeetingRoom', 'init', 'currentuserInfo', this.currentuserInfo);
				return;
			}

			this.roomData = uc.util.StorageUtil.getData("meetingRoom_createOrEditMeeting");
			if (this.roomData == null) {
				uc.util.LogUtil.warn('uc.modules.meeting.room.createMeetingRoom', 'init', 'meetingRoom_createOrEditMeeting', this.roomData);
				return;
			}
			this.type = this.roomData.type;
			this.roomInfo = this.roomData.roomInfo;


			this.baseNode = $('.meetingRoom-createMeetingRoom');

			this.closeBtn = $('.tool-icon-container .icon-close');

			this.roomNameInput = this.baseNode.find(".roomNameInput");
			this.roomNameCount = this.baseNode.find(".roomNameCount");

			this.startTime = this.baseNode.find(".startTimeBox");
			this.endTime = this.baseNode.find(".endTimeBox");
			this.startTimePickdate = null;
			this.endTimePickdate = null;

			this.roomHostInput = this.baseNode.find(".roomHostInput");
			this.roomHostContent = this.baseNode.find(".roomHostContent");

			this.defaultAttendeeInput = this.baseNode.find(".defaultAttendeeInput");
			this.defaultAttendeeContent = this.baseNode.find(".defaultAttendeeContent");

			this.checkBox_defaultAttendsVisible = this.baseNode.find(".defaultAttendsVisible").find('input[type="checkbox"]');
			this.checkBox_defaultAttendsJoin = this.baseNode.find(".defaultAttendsJoin").find('input[type="checkbox"]');
			this.checkBox_isSetPassword = this.baseNode.find(".isSetPassword").find('input[type="checkbox"]');

			this.passwordInput = this.baseNode.find(".passwordInput");

			this.roomRemarkInput = this.baseNode.find(".roomRemark textarea");

			this.cancelBtn = $('#createorEditMeetingRoomCancel');
			this.oKBtn = $('#createorEditMeetingSave');

			this.currentFocusSearchInput = null;

			this.attendeeData = new uc.modules.meeting.room.createMeetingRoom.AttendeeMap();

			this.roomHostUserInfo = null;

			this.initUI();
			this.bindEvents();
		},

		initUI: function () {
			this.initTitle();
			this.initRoomnameInputCount();
			this.initRoomExpiryDate();
			this.initRoomHost();
			this.initDefaultAttendee();
			this.initCheckBox();
			this.initOkBtnStatus();
		},

		bindEvents: function () {
			this.bindWindCloseEvent();
			this.bindroomNameInputEvent();
			this.bindFocusEvent();
			this.bindGlobalEvent();
		},

		initTitle: function () {
			var title = uc_resource.Meeting.meetingRoom.createOrEditMeetingRoomPanle.createRoom;

			switch (this.type) {
				case uc.constants.Meeting.Room.CreateOrEditType.Create:
					title = uc_resource.Meeting.meetingRoom.createOrEditMeetingRoomPanle.createRoom;
					break;
				case uc.constants.Meeting.Room.CreateOrEditType.Edit:
					title = uc_resource.Meeting.meetingRoom.createOrEditMeetingRoomPanle.editRoom;
					break;
			}

			this.baseNode.find('.windowTitle').text(title);
		},

		initRoomExpiryDate: function () {
			this.startTimePickdate = new uc.modules.component.PickADate(this.startTime, false, false, function (date) {
					//set endTimeValue
					if (date != null) {

					}
				}
			);

			this.endTimePickdate = new uc.modules.component.PickADate(this.endTime, false, false, function (date) {
					//set endTimeValue
					if (date != null) {

					}
				}
			);

			//编辑会议室，直接赋值
			if (this.type == uc.constants.Meeting.Room.CreateOrEditType.Edit) {

				this.startTimePickdate.setDate(new Date(this.roomInfo.startTime * 1000));
				this.endTimePickdate.setDate(new Date(this.roomInfo.endTime * 1000));
			} else {
				this.startTimePickdate.setDate(this.getStartTime());
				this.endTimePickdate.setDate(this.getEndTime());
			}
		},

		initRoomnameInputCount: function () {
			this.roomNameInput.attr('maxlength', uc.constants.Meeting.MeetingRoom.RoomNameInputMaxLength);

			//编辑会议室，直接赋值
			if (this.type == uc.constants.Meeting.Room.CreateOrEditType.Edit) {
				this.roomNameInput.val(this.roomInfo.title);
				this.roomRemarkInput.val(this.roomInfo.remarks);
			}

			this.updateRoomnameInputCount();
		},

		initRoomHost: function () {
			var self = this;
			//编辑会议室，直接赋值
			if (this.type == uc.constants.Meeting.Room.CreateOrEditType.Edit) {
				var hostInfo = uc.modules.cache.BasicContactInfoCache.getBasicContactInfo(this.roomInfo.roomHostId);
				if (hostInfo) {
					this.roomHostUserInfo = hostInfo;
					this.addUserItem(this.roomHostInput, this.roomHostUserInfo.userId || this.roomHostUserInfo.account, this.roomHostUserInfo.displayName, true, false);
				}
			} else {
				if (this.roomHostUserInfo == null) {
					this.roomHostUserInfo = this.currentuserInfo;
				}
				//显示默认主持人
				this.addUserItem(this.roomHostInput, this.roomHostUserInfo.userId || this.roomHostUserInfo.account, this.roomHostUserInfo.displayName, true, false);
			}
			uc.modules.component.SearchBox.init(this.roomHostInput, {
				count: 4,
				selectedUser: function (args) {
					self.searchUserCallBack(args);
				}
			});

		},

		initDefaultAttendee: function () {
			var self = this;
			//编辑会议室，直接赋值
			if (this.type == uc.constants.Meeting.Room.CreateOrEditType.Edit) {
				for (var i = 0; i < this.roomInfo.attendees.length; i++) {
					var key = this.roomInfo.attendees[i].userId || this.roomInfo.attendees[i].account;
					var displayName = this.roomInfo.attendees[i].name;
					if (!this.attendeeData.isExistAttendee(key)) {
						this.attendeeData.addAttendee(key, this.roomInfo.attendees[i]);
						//此处需要优化
						this.addUserItem(this.defaultAttendeeInput, key, displayName, true, true);
					}
				}
			}

			uc.modules.component.SearchBox.init(this.defaultAttendeeInput, {
				count: 4,
				selectedUser: function (args) {
					self.searchUserCallBack(args);
				}
			});
		},

		initCheckBox: function () {
			//编辑会议室，直接赋值
			if (this.type == uc.constants.Meeting.Room.CreateOrEditType.Edit) {
				if (this.roomInfo.isSetPassword == 1) {
					this.checkBox_isSetPassword.prop("checked", true);
					this.passwordInput.toggleClass('hide');
					this.passwordInput.val(this.roomInfo.roomPassword);
				}
				if (this.roomInfo.defaultAttendsVisible == 1) {
					this.checkBox_defaultAttendsVisible.prop("checked", true);
				}

				if (this.roomInfo.defaultAttendsJoin == 1) {
					this.checkBox_defaultAttendsJoin.prop("checked", true);
				}

			}
		},

		initOkBtnStatus: function () {
			switch (this.type) {
				case uc.constants.Meeting.Room.CreateOrEditType.Create:
					this.oKBtn.addClass('disabled');
					break;
				case uc.constants.Meeting.Room.CreateOrEditType.Edit:
					this.oKBtn.removeClass('disabled');
					break;
			}
		},

		searchUserCallBack: function (user) {
			var key =  user.userId || user.account;

			if (this.roomHostInput == this.currentFocusSearchInput) {
				this.roomHostInput.val('');
				this.roomHostInput.focus();
				this.roomHostContent.addClass('focus');

				//如果默认参会者列表有此玩家，删除默认列表
				if(this.attendeeData.isExistAttendee(key)){
					this.baseNode.find('.userItemDelete[userid='+ key +']').trigger('click');
				}

				this.roomHostUserInfo = user;
				this.roomHostInput.parent().find('.userItem').remove();

				this.addUserItem(this.roomHostInput, this.roomHostUserInfo.userId || this.roomHostUserInfo.account, this.roomHostUserInfo.displayName, true, false);

			} else if (this.defaultAttendeeInput == this.currentFocusSearchInput) {
				this.defaultAttendeeInput.val('');
				this.defaultAttendeeInput.focus();
				this.defaultAttendeeContent.addClass('focus');

				//set type
				if (user.userId) {
					user.type = uc.constants.Meeting.AttendeeType.User;
				} else {
					user.type = uc.constants.Meeting.AttendeeType.Mail;
				}
				//如果默认参会人列表没有，并且不是主持人
				if (!this.attendeeData.isExistAttendee(key) && key != (this.roomHostUserInfo.userId || this.roomHostUserInfo.account)) {
					var attaendee = new uc.modules.meeting.room.info.AttendeeInfo();
					var userInfo = user;
					var type = userInfo.userId ? (userInfo.type ? userInfo.type : uc.constants.Meeting.AttendeeType.User) : uc.constants.Meeting.AttendeeType.Mail;

					attaendee.account = (userInfo.userId || userInfo.account) + "";
					attaendee.name = userInfo.name || userInfo.displayName;
					attaendee.status = 0;
					attaendee.type = type;

					this.attendeeData.addAttendee(key, attaendee);
					this.addUserItem(this.defaultAttendeeInput, key, user.displayName, true, true);
				}
			}
		},

		updateRoomnameInputCount: function () {
			this.roomNameCount.text(this.roomNameInput.val().length + '/' + uc.constants.Meeting.MeetingRoom.RoomNameInputMaxLength);
		},

		addUserItem: function (target, userId, disPlayname, isBgGray, isColudDelete) {
			var bgColor = isBgGray ? "bgGray" : "bgRed";
			var _item;
			var inc;
			if (!isColudDelete) {
				_item = '<span style="background:#e9e9e9;color:#03111f" class="userItem ' + bgColor + '">' + disPlayname + '&nbsp;</span>'
				inc = 0;
			} else {
				_item = '<span class="userItem ' + bgColor + '">' + disPlayname + '&nbsp;<i userid="' + userId + '" class="icon-close hide userItemDelete"></i></span>'
				inc = 10;
			}

			_item = $(_item);
			target.before(_item);
			_item.width((_item.width() + inc) + "px");

			return _item;
		},

		bindWindCloseEvent: function () {
			this.closeBtn.on('click', function () {
				uc.util.WindowUtil.close();
			});
		},

		bindroomNameInputEvent: function () {
			var self = this;
			this.roomNameInput.on('input', function (e) {
				if ($(this).val().trim() == "") {
					self.oKBtn.addClass('disabled');
				} else {
					self.oKBtn.removeClass('disabled');
				}
				self.updateRoomnameInputCount();
			});
		},

		bindFocusEvent: function () {
			var self = this;
			this.roomHostInput.blur(function () {
				$(this).val('');
				self.roomHostContent.removeClass('focus');
			}).focus(function () {
					self.currentFocusSearchInput = self.roomHostInput;
					self.roomHostContent.addClass('focus');
				});

			this.defaultAttendeeInput.blur(function () {
				$(this).val('');
				self.defaultAttendeeContent.removeClass('focus');
			}).focus(function () {
					self.currentFocusSearchInput = self.defaultAttendeeInput;
					self.defaultAttendeeContent.addClass('focus');
				});
		},

		bindGlobalEvent: function () {
			var self = this;
			this.baseNode.on('click', 'a',function (e) {
				var action = $(this).attr('action');
				switch (action) {
					case "roomDefaultAttendeeBatchAdd"://批量添加
						self.selectBatchUserWindow(function (args) {
							if (args.okOrCancel == 0) {
								return;
							}
							self.selectBatchUserCallback(args);
							self.defaultAttendeeInput.focus();
						});
						break;
				}
			}).on('mouseover', '.userItem',function (e) {
					$(this).find('i').removeClass('hide');
				}).on('mouseout', '.userItem',function (e) {
					$(this).find('i').addClass('hide');
				}).on('click', '.userItemDelete',function (e) {
					var _parent = $(this).parent();
					var key = $(this).attr('userid');

					self.attendeeData.deleteAttendee(key);

					_parent.find('*').off();
					_parent.remove();
				}).on('click', 'label', function (e) {
					var checkBox = $(this).parent().find('input[type="checkbox"]');
					var type = $(this).attr('type');
					if (checkBox.prop("checked")) {
						checkBox.prop("checked", false);
					} else {
						checkBox.prop("checked", true);
					}
					if (type == 3) {
						self.passwordInput.toggleClass('hide');
					}
				});
			this.cancelBtn.on('click', function () {
				uc.util.WindowUtil.close();
				uc.util.StorageUtil.removeData("meetingRoom_createOrEditMeeting");
			});
			this.oKBtn.on('click', function () {
				if ($(this).hasClass('disabled')) {
					return false;
				}
				$(this).addClass('disabled');
				self.createOrEditMeetting();
			});
		},

		createOrEditMeetting: function () {

			var attendees = this.getAttendees();
			var title = this.roomNameInput.val();
			var remarks = this.roomRemarkInput.val();
			var password = this.passwordInput.val();

			var startTime = this.getExpiryData(this.startTimePickdate);
			var endTime = this.getExpiryData(this.endTimePickdate);

			var roomHostId = this.roomHostUserInfo.userId;
			var orgId = 0;
			var isSetPassword = this.checkBox_isSetPassword.prop("checked") ? 1 : 0;
			var defaultAttendsVisible = this.checkBox_defaultAttendsVisible.prop("checked") ? 1 : 0;
			var defaultAttendsJoin = this.checkBox_defaultAttendsJoin.prop("checked") ? 1 : 0;

			/**time Check*/
			if (!this.checkExpiryDate(startTime, endTime)) {
				return false;
			}
			/**AttendeeData check*/
			if (!this.checkAttendeeData()) {
				return false;
			}
			/**PSW check*/
			if (!this.CheckPSW(isSetPassword)) {
				return false;
			}

			switch (this.type) {
				case uc.constants.Meeting.Room.CreateOrEditType.Create:
					orgId = parseInt(this.roomData.orgId);
					uc.IMeetingRoomService.createMeetingRoom(attendees, title, remarks, password, startTime, endTime, roomHostId, orgId, isSetPassword, defaultAttendsVisible, defaultAttendsJoin, function (errorCode) {
						if (errorCode[0] != uc.ErrorCode.Success) {
							uc.util.LogUtil.warn("uc.modules.meeting.room.createMeetingRoom.CreateMeetingRoom", "createOrEditMeetting", "createCallBack", errorCode[0]);
						}
					});
					break;
				case uc.constants.Meeting.Room.CreateOrEditType.Edit:
					orgId = parseInt(this.roomInfo.orgId);
					uc.IMeetingRoomService.updateMeetingRoom(this.roomInfo.roomId, attendees, title, remarks, password, startTime, endTime, roomHostId, orgId, isSetPassword, defaultAttendsVisible, defaultAttendsJoin, function (errorCode) {
						if (errorCode[0] != uc.ErrorCode.Success) {
							uc.util.LogUtil.warn("uc.modules.meeting.room.createMeetingRoom.CreateMeetingRoom", "createOrEditMeetting", "updateCallBack", errorCode[0]);
						}
					});
					break;
			}
		},

		getAttendees: function () {
			//var attendees = [];
			var allAttendees = this.attendeeData.getAllAttendee();
			/*for (var i = 0, len = allAttendees.length; i < len; i++) {
				var attaendee = new uc.modules.meeting.room.info.AttendeeInfo();
				var userInfo = allAttendees[i];
				var type = userInfo.userId ? (userInfo.type ? userInfo.type : uc.constants.Meeting.AttendeeType.User) : uc.constants.Meeting.AttendeeType.Mail;

				attaendee.account = (userInfo.userId || userInfo.account) + "";
				attaendee.name = userInfo.name || userInfo.displayName;
				attaendee.status = 0;
				attaendee.type = type;

				attendees.push(attaendee);
			}*/

			return allAttendees;
		},

		CheckPSW: function (isSetPassword) {
			var self = this;
			if (isSetPassword) {
				if (this.passwordInput.val().trim() == '') {

					this.confirm(uc_resource.Meeting.meetingRoom.needInputPSW, function () {
						self.oKBtn.removeClass('disabled');
					});
					return false
				}
			}
			return true;
		},

		checkAttendeeData: function () {
			var self = this;
			if (!this.attendeeData.size()) {
				this.confirm(uc_resource.Meeting.meetingRoom.attendeeDataEmpty, function (r) {
					if (r == 0) {
						self.selectBatchUserWindow(function (args) {
							if (args.okOrCancel == 0) {
								return;
							}
							self.selectBatchUserCallback(args);
							self.defaultAttendeeInput.focus();
						});
					}
					self.oKBtn.removeClass('disabled');
				}, true);
				return false
			}
			return true;
		},

		checkExpiryDate: function (startTime, endTime) {
			uc.util.LogUtil.info('uc.modules.meeting.room.createMeetingRoom', 'checkExpiryDate', 'startTime', new Date(startTime * 1000));
			uc.util.LogUtil.info('uc.modules.meeting.room.createMeetingRoom', 'checkExpiryDate', 'endTime', new Date(endTime * 1000));
			var self = this;
			if (startTime == null || endTime == null) {
				this.confirm(uc_resource.Meeting.meetingRoom.time.needInputTime, function () {
					self.oKBtn.removeClass('disabled');
				});
				return false;
			} else {
				var currentday = new Date();
				currentday.setHours(0);
				currentday.setMinutes(0);
				currentday.setSeconds(0);

				var currentTime = parseInt(currentday.getTime() / 1000);

				if (startTime < currentTime) {
					this.confirm(uc_resource.Meeting.meetingRoom.time.startTimeNeedLaterCurrentTime, function (r) {
						if (r == 0) {
							self.startTimePickdate.setDate(self.getStartTime());
						}
						self.oKBtn.removeClass('disabled');
					}, true);
					return false;
				}
				if (endTime < currentTime) {
					this.confirm(uc_resource.Meeting.meetingRoom.time.endTimeNeedLaterCurrentTime, function (r) {
						if (r == 0) {
							self.endTimePickdate.setDate(self.getEndTime());
						}
						self.oKBtn.removeClass('disabled');
					}, true);
					return false;
				}

				if (endTime < startTime) {
					this.confirm(uc_resource.Meeting.meetingRoom.time.endTimeNeedLaterStartTime, function () {
						self.oKBtn.removeClass('disabled');
					});
					return false;
				}
				if (endTime == startTime) {
					this.confirm(uc_resource.Meeting.meetingRoom.time.endTimeUnEqualStartTime, function () {
						self.oKBtn.removeClass('disabled');
					});
					return false;
				}
			}
			return true;
		},

		getStartTime: function () {

			var today = new Date();
			var hours = today.getHours(), minutes = today.getMinutes();
			if (hours == 23 && minutes > 30) {
				//today = _getDateFromToday(1);
				today.setDate(today.getDate() + 1);
			}
			if (minutes > 30) {
				minutes = "00";
				if (hours == 23) {
					hours = 0;
				} else {
					hours += 1;
				}
			} else {
				minutes = "30";
			}
			hours = ((hours + 100) + "").substr(1, 2);
			today = today;
			today.setHours(hours);
			today.setMinutes(minutes);
			return today;
		},

		getEndTime: function () {
			var today = this.getStartTime();
			var hours = today.getHours(), minutes = today.getMinutes();
			if (hours == 23) {
				today.setDate(today.getDate() + 1);
			}
			today.setHours(23);
			today.setMinutes(59);
			return today;
		},

		getExpiryData: function (timbox) {
			var TempTime = timbox.getDate();
			var timeZone = uc.settings.meeting.timeZone.offset;
			if (TempTime) {
				return uc.util.TimeUtil.displayDateToUnixTime(timeZone, TempTime.getFullYear(), TempTime.getMonth() + 1,
					TempTime.getDate(), TempTime.getHours(), TempTime.getMinutes(), 0, uc.constants.UI.TimeStampUnit.Second);
			} else {
				return null;
			}
		},

		selectBatchUserCallback: function (args) {
			for (var i = 0; i < args.contacts.length; i++) {
				var key = args.contacts[i].userId || args.contacts[i].account;
				var displayName = args.contacts[i].displayName;
				if (!this.attendeeData.isExistAttendee(key) && key != (this.roomHostUserInfo.userId || this.roomHostUserInfo.account)) {

					var attaendee = new uc.modules.meeting.room.info.AttendeeInfo();
					var userInfo = args.contacts[i];
					var type = userInfo.userId ? (userInfo.type ? userInfo.type : uc.constants.Meeting.AttendeeType.User) : uc.constants.Meeting.AttendeeType.Mail;

					attaendee.account = (userInfo.userId || userInfo.account) + "";
					attaendee.name = userInfo.name || userInfo.displayName;
					attaendee.status = 0;
					attaendee.type = type;
					this.attendeeData.addAttendee(key, attaendee);
					//此处需要优化
					this.addUserItem(this.defaultAttendeeInput, key, displayName, true, true);
				}
			}

		},

		selectBatchUserWindow: function (fn) {
			uc.IUIService.openChooseContactWindow({
				key: "MeetingRoomCreator",
				fromForward: true
			}, function (args) {
				fn(args);
			});
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
		},

		OnMeetingRoomCreated: function (errorCode, meetingRoom) {
			var self = this;
			if (errorCode == uc.ErrorCode.Success) {
				uc.util.WindowUtil.close();
				uc.IUIService.triggerCrossWindowEvent(self.roomData.key, {success: true});
			} else {
				self._showErrorMsg(errorCode);
			}
		},

		OnMeetingRoomUpdated: function (errorCode, meetingRoom) {
			var self = this;
			if (errorCode == uc.ErrorCode.Success) {
				uc.util.WindowUtil.close();
				uc.IUIService.triggerCrossWindowEvent(self.roomData.key, {success: true});
			} else {
				self._showErrorMsg(errorCode);
			}
		},
		_showErrorMsg: function(errorCode){
			var errorName = "ERROR_CODE_" + errorCode;
			var errorMeg;
			var self = this;
			errorMeg = uc_resource.Meeting.ErrorMessage[errorName] || uc_resource.Meeting.ErrorMessage["ERROR_DEFAULT"];
			if(errorMeg == uc_resource.Meeting.ErrorMessage["ERROR_DEFAULT"]){
				uc.util.LogUtil.warn("uc.modules.meeting.room.createMeetingRoom.CreateMeetingRoom ", "_showErrorMsg", "ERROR_DEFAULT=",errorCode);
			}
			uc.modules.component.PromptMessageBox.open({
				title: uc_resource.WindowTitle.Notification,
				message: errorMeg,
				buttonMode: "ok",
				OkText: uc_resource.Common.OK
			}, function () {
				self.oKBtn.removeClass('disabled');
				this.close();
			});
		}
	};

	jQuery(document).ready(function () {
		uc.init();
		uc.modules.meeting.room.createMeetingRoom.CreateMeetingRoom.init();
	});
})(uc);
