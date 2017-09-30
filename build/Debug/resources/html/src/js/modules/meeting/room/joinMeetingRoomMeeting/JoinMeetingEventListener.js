/**
 * User: hougang.chen
 * Date: 16-5-17
 * Time: 下午4:59
 */
(function (uc) {
	uc.modules.meeting.room.joinMeetingRoomMeeting.JoinMeetingEventListener = function () {

	};
	jQuery.extend(uc.modules.meeting.room.joinMeetingRoomMeeting.JoinMeetingEventListener.prototype,
		uc.interfaces.IUIEvent,
		uc.interfaces.IMeetingRoomEvent, {
			//检查会议室密码是否有效事件
			OnCheckMeetingRoomPassword: function (errorCode, roomId, password, value) {
				uc.modules.meeting.room.joinMeetingRoomMeeting.JoinMeetingManager.OnCheckMeetingRoomPassword(errorCode, roomId, password, value);
			}
		});
})(uc);