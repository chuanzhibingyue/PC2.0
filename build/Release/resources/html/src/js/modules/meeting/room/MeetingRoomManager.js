/**
 * Created by waouyang on 16/5/4.
 */
(function (uc) {
	uc.modules.meeting.room.MeetingRoomManager = {
		OnModuleChanged: function () {
			this.init();
		},
		init: function () {
			uc.modules.meeting.room.MeetingRoom.init();
		},
		/**
		 * 组织架构数据返回
		 * */
		OnOrgInfoReceived: function (errorCode, department, myDepartment,taskId) {
			uc.modules.meeting.room.MeetingRoom.OnOrgInfoReceived(errorCode, department,taskId);
		},
		/**创建会议室UI事件*/
		OnCreateMeetingRoom: function (data) {
			var options = {
				width: 420,
				height: 635,
				refreshIfExist: false,
				centerScreen: true,
				modal: true,
				resizable: false,
				isTop: false,
				caption: uc.constants.UI.DragCaption.CloudSetting
			};
			uc.util.WindowUtil.open('page/meetingRoom_createMeetingRoom.html', 'meetingRoom_createMeeting', options);
		},
		/**加入会议室会议，输入密码*/
		OnJoinMeetingRoomMeeting:  function (data) {
			var options = {
				width: 390,
				height: 190,
				refreshIfExist: false,
				centerScreen: true,
				modal: true,
				resizable: false,
				isTop: false,
				enableDrag:false
			};
			uc.util.WindowUtil.open('page/meetingRoom_JoinMeeting.html', 'meetingRoom_JoinMeeting', options);
		},

		OnMeetingRoomListReceived: function (errorCode, orgId, lastGetTime, rooms) {
			uc.modules.meeting.room.MeetingRoom.OnMeetingRoomListReceived(errorCode, orgId, lastGetTime, rooms);
		},

		OnBasicContactInfoReceived: function (basicContactInfos) {
			uc.modules.meeting.room.MeetingRoom.OnBasicContactInfoReceived(basicContactInfos);
		},
		OnMeetingRoomCreated: function (meetingRoom) {
			uc.modules.meeting.room.MeetingRoom.OnMeetingRoomCreated(meetingRoom);
		},
		OnMeetingRoomUpdated: function (meetingRoom) {
			uc.modules.meeting.room.MeetingRoom.OnMeetingRoomUpdated(meetingRoom);
		},
		OnMeetingRoomDeleted: function (roomId) {
			uc.modules.meeting.room.MeetingRoom.OnMeetingRoomDeleted(roomId);
		},

		OnWaitJoinMeetingRoomMeeting: function () {
			uc.modules.meeting.room.MeetingRoom.OnWaitJoinMeetingRoomMeeting();
		},

		OnMeetingRoomMeetingJoined: function () {
			uc.modules.meeting.room.MeetingRoom.OnMeetingRoomMeetingJoined();
		},

		OnJoinMeetingRoomMeetingFailed: function (errorCode) {
			uc.modules.meeting.room.MeetingRoom.OnMeetingRoomMeetingJoined(errorCode);
		},

		OnCheckJoinMeetingRoom: function ( errorCode, roomId, hasPassword, isValid, mayJoin ) {
			uc.modules.meeting.room.MeetingRoom.OnCheckJoinMeetingRoom(errorCode, roomId, hasPassword, isValid, mayJoin);
		},

		OnCheckMeetingRoomPassword: function (errorCode, roomId, password,value) {
			uc.modules.meeting.room.MeetingRoom.OnCheckMeetingRoomPassword(errorCode, roomId, password,value);
		},
		OnRightsChecked: function (errorCode) {
			uc.modules.meeting.room.MeetingRoom.OnRightsChecked(errorCode);
		}
	};
})(uc);
