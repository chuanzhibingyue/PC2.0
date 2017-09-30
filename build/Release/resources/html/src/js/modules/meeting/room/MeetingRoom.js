/**
 * Created by waouyang on 16/5/4.
 */
(function (uc) {
	uc.modules.meeting.room.MeetingRoom = {
		init: function () {
			if (!this.initilized) {

				this.initilized = true;
				this.roomDataMap = new uc.modules.meeting.room.roomDataMap();

				this.orgRootNode = $('.meetingRoom');
				this.addMeetingRoomBtn = this.orgRootNode.find('.addMeetingRoom');

				this.currentOrgId = 0;

				this.bindEvents();
				var self = this;
				//初始化组织架构View
				uc.modules.meeting.room.MeetingRoomOrgList.init(this.roomDataMap, function (args) {
					self.updateDetails(args);
				}, function (args) {
					self.selectOrgItem(args);
				});
				//初始化组织会议室详情View
				uc.modules.meeting.room.MeetingRoomDetails.init(this.roomDataMap);
				this.updateDetails();

				//是否管理员，用于显示创建会议室按钮
				this.isAdmin = false;

				this.updateAddMeetingRoomBtnStatus();
				this.isGetRootOrgInfo = false;

			}
			//获取权限状态
			uc.IMeetingRoomService.checkRightsOfCreateRoom(function (args) {

			});
		},

		bindEvents: function () {
			var self = this;
			this.addMeetingRoomBtn.on('click', function () {
				var options = {key: "createMeetingRoom", type: uc.constants.Meeting.Room.CreateOrEditType.Create, orgId: self.currentOrgId};
				uc.IUIService.createMeetingRoom(options, function (data) {

				});
			});
		},

		OnOrgInfoReceived: function (errorCode, departmentData, taskId) {
			if (this.getOrgInfoTaskId == taskId) {
				uc.modules.meeting.room.MeetingRoomOrgList.renderOrgPanel(errorCode, departmentData);
			}
		},

		OnMeetingRoomListReceived: function (errorCode, orgId, lastGetTime, rooms) {
			uc.modules.meeting.room.MeetingRoomOrgList.OnMeetingRoomListReceived(errorCode, orgId, lastGetTime, rooms);
		},

		updateDetails: function (roomid) {

			uc.modules.meeting.room.MeetingRoomDetails.renderDetailPanel(roomid);
			this.updateAddMeetingRoomBtnStatus();
		},

		selectOrgItem: function (orgId) {
			this.currentOrgId = orgId;
			uc.modules.meeting.room.MeetingRoomDetails.renderDetailPanel(null);
			this.updateAddMeetingRoomBtnStatus();
		},

		clear: function () {

		},

		OnMeetingRoomCreated: function (meetingRoom) {
			uc.modules.meeting.room.MeetingRoomOrgList.OnMeetingRoomCreated(meetingRoom);
		},

		OnMeetingRoomUpdated: function (meetingRoom) {
			//更新会议列表，以及会议详情
			uc.modules.meeting.room.MeetingRoomOrgList.OnMeetingRoomUpdated(meetingRoom);
			uc.modules.meeting.room.MeetingRoomDetails.OnMeetingRoomUpdated(meetingRoom);
		},
		OnMeetingRoomDeleted: function (roomId) {
			uc.modules.meeting.room.MeetingRoomOrgList.OnMeetingRoomDeleted(roomId);
			this.updateDetails(null);
		},

		OnWaitJoinMeetingRoomMeeting: function () {
			uc.modules.meeting.room.MeetingRoomDetails.OnWaitJoinMeetingRoomMeeting();
		},

		OnMeetingRoomMeetingJoined: function () {
			uc.modules.meeting.room.MeetingRoomDetails.OnMeetingRoomMeetingJoined();
		},

		OnJoinMeetingRoomMeetingFailed: function (errorCode) {
			uc.modules.meeting.room.MeetingRoomDetails.OnMeetingRoomMeetingJoined(errorCode);
		},

		OnCheckJoinMeetingRoom: function (errorCode, roomId, hasPassword, isValid, mayJoin) {
			uc.modules.meeting.room.MeetingRoomDetails.OnCheckJoinMeetingRoom(errorCode, roomId, hasPassword, isValid, mayJoin);
		},

		OnCheckMeetingRoomPassword: function (errorCode, roomId, password, value) {
			uc.modules.meeting.room.MeetingRoomDetails.OnCheckMeetingRoomPassword(errorCode, roomId, password, value);
		},

		OnRightsChecked: function (errorCode) {
			this.isAdmin = errorCode == uc.ErrorCode.Success ? true : false;
			uc.modules.meeting.room.MeetingRoomDetails.setAdmin(this.isAdmin);
			uc.modules.meeting.room.MeetingRoomOrgList.setAdmin(this.isAdmin);
			this.updateAddMeetingRoomBtnStatus();
		   if(!this.isGetRootOrgInfo){
			   var self = this;
			   //获取组织架构Data
			   uc.IContactService.getOrgInfo(0, function (data) {
				   self.getOrgInfoTaskId = data[1];
			   });
			   this.isGetRootOrgInfo = true;
		   }

		},

		updateAddMeetingRoomBtnStatus: function () {
			if (this.isAdmin) {
				if (uc.modules.meeting.room.MeetingRoomDetails.isActive) {
					this.addMeetingRoomBtn.hide();
				} else {
					this.addMeetingRoomBtn.show();
				}
			} else {
				this.addMeetingRoomBtn.hide();
			}
		},
		OnBasicContactInfoReceived: function (basicContactInfos) {
			uc.modules.meeting.room.MeetingRoomDetails.OnBasicContactInfoReceived(basicContactInfos);
		}
	}
})(uc);