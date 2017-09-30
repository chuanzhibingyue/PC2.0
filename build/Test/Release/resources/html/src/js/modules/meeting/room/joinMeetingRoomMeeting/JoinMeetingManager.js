/**
 * User: hougang.chen
 * Date: 16-5-17
 * Time: 下午4:59
 */

(function (uc) {
	uc.modules.meeting.room.joinMeetingRoomMeeting.JoinMeetingManager = {
		OnCheckMeetingRoomPassword: function (errorCode, roomId, password, value) {
			uc.modules.meeting.room.joinMeetingRoomMeeting.JoinMeeting.OnCheckMeetingRoomPassword(errorCode, roomId, password, value);
		}
	};
})(uc);
