/**
 * Created by waouyang on 16/5/4.
 */

(function (uc) {
	uc.modules.meeting.room.MeetingRoomEventListener = function () {
		this.moduleId = -1;
	};
	jQuery.extend(uc.modules.meeting.room.MeetingRoomEventListener.prototype,
		uc.interfaces.IContactEvent,
		uc.interfaces.IUIEvent,
		uc.interfaces.IMeetingEvent,
		uc.interfaces.IMeetingRoomEvent, {
			OnInitClient: function (clearData) {

			},
			OnModuleChanged: function (moduleId) {
				this.moduleId = moduleId;
				if (this.isCurrentModule()) {
					uc.modules.meeting.room.MeetingRoomManager.OnModuleChanged();
				}
			},
			OnBasicContactInfoReceived: function (errorCode, userIds, contacts) {
				if (errorCode == uc.ErrorCode.Success && contacts && this.isCurrentModule()) {

					uc.modules.meeting.room.MeetingRoomManager.OnBasicContactInfoReceived(contacts);
					uc.modules.meeting.room.roomAttendee.MeetingRoomManager.OnBasicContactInfoReceived(errorCode, userIds, contacts);
				}
			},
			OnUserAvatarDownloaded: function (userId, serverAvatarUrl, localAvatarUrl) {
				if (this.isCurrentModule()) {
					uc.modules.meeting.room.roomAttendee.MeetingRoomManager.OnBasicContactInfoReceived(userId, serverAvatarUrl, localAvatarUrl);
				}
			},
			/**IContactEvent*/
			OnOrgInfoReceived: function (errorCode, department, myDepartment, taskId) {
				if (this.isCurrentModule()) {
					uc.modules.meeting.room.MeetingRoomManager.OnOrgInfoReceived(errorCode, department, myDepartment, taskId);
				}
			},
			isCurrentModule: function () {
				return  this.moduleId == uc.constants.UI.ModuleId.MeetingRoom;
			},
			/**IUIEvent*/
			OnCreateMeetingRoom: function (data) {
				uc.modules.meeting.room.MeetingRoomManager.OnCreateMeetingRoom(data);
			},
			OnWaitJoinMeetingRoomMeeting: function () {
				uc.modules.meeting.room.MeetingRoomManager.OnWaitJoinMeetingRoomMeeting();
			},

			OnMeetingRoomMeetingJoined: function () {
				uc.modules.meeting.room.MeetingRoomManager.OnMeetingRoomMeetingJoined();
			},

			OnJoinMeetingRoomMeetingFailed: function (errorCode) {
				uc.modules.meeting.room.MeetingRoomManager.OnJoinMeetingRoomMeetingFailed(errorCode);
			},
			OnJoinMeetingRoomMeeting: function (data) {
				uc.modules.meeting.room.MeetingRoomManager.OnJoinMeetingRoomMeeting(data);
			},
			/**IMeetingRoomEvent**/
			//会议室创建事件
			OnMeetingRoomCreated: function (errorCode, meetingRoom) {
				if (errorCode == uc.ErrorCode.Success) {
					uc.modules.meeting.room.MeetingRoomManager.OnMeetingRoomCreated(meetingRoom);
				}
			},
			//会议室更新事件
			OnMeetingRoomUpdated: function (errorCode, meetingRoom) {
				if (errorCode == uc.ErrorCode.Success) {
					uc.modules.meeting.room.MeetingRoomManager.OnMeetingRoomUpdated(meetingRoom);
				}
			},
			//会议室创建会议事件
			OnMeetingCreatedInMeetingRoom: function (errorCode, roomId, meeting) {

			},
			//会议室列表事件
			OnMeetingRoomListReceived: function (errorCode, orgId, lastGetTime, rooms) {
				uc.modules.meeting.room.MeetingRoomManager.OnMeetingRoomListReceived(errorCode, orgId, lastGetTime, rooms);
			},
			//检查会议室是否可用事件
			OnCheckJoinMeetingRoom: function (errorCode, roomId, hasPassword, isValid, mayJoin) {
				if(uc.modules.main.Main.moduleId == uc.constants.UI.ModuleId.MeetingRoom){
					uc.modules.meeting.room.MeetingRoomManager.OnCheckJoinMeetingRoom(errorCode, roomId, hasPassword, isValid, mayJoin);
				}
			},
			//检查会议室密码是否有效事件
			OnCheckMeetingRoomPassword: function (errorCode, roomId, password,value) {
				uc.modules.meeting.room.MeetingRoomManager.OnCheckMeetingRoomPassword(errorCode, roomId, password,value);
			},
			//删除会议室事件
			OnMeetingRoomDeleted: function (errorCode, roomId) {
				if (errorCode == uc.ErrorCode.Success) {
					uc.modules.meeting.room.MeetingRoomManager.OnMeetingRoomDeleted(roomId);
				}
			},
			OnRightsChecked: function (errorCode){
				uc.modules.meeting.room.MeetingRoomManager.OnRightsChecked(errorCode);
			}
		});
})(uc);