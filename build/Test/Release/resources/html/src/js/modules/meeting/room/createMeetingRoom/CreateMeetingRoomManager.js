/**
 * User: hougang.chen
 * Date: 16-5-5
 * Time: 下午3:33
 */
(function (uc) {
	uc.modules.meeting.room.createMeetingRoom.CreateMeetingRoomManager = {

		//会议室创建事件
		OnMeetingRoomCreated: function (errorCode, meetingRoom) {
			uc.modules.meeting.room.createMeetingRoom.CreateMeetingRoom.OnMeetingRoomCreated(errorCode, meetingRoom);
		},
		//会议室更新事件
		OnMeetingRoomUpdated: function (errorCode, meetingRoom) {
			uc.modules.meeting.room.createMeetingRoom.CreateMeetingRoom.OnMeetingRoomUpdated(errorCode, meetingRoom);
		}
	};
})(uc);

