/**
 * User: hougang.chen
 * Date: 16-5-17
 * Time: 上午10:09
 */
/**
 * Created by waouyang on 16/5/4.
 */
(function (uc) {
	uc.modules.meeting.room.roomAttendee.MeetingRoomManager = {

		showMeetingRoomAttendee: function (parentNode, roomInfo) {
			uc.modules.meeting.room.roomAttendee.MeetingRoomAttendee.init(parentNode);
			uc.modules.meeting.room.roomAttendee.MeetingRoomAttendee.showAttendees(roomInfo);
			this.attendeeListShown = true;
		},

		OnBasicContactInfoReceived: function (errorCode, userIds, contacts) {
			if (errorCode == uc.ErrorCode.Success && this.attendeeListShown) {
				uc.modules.meeting.room.roomAttendee.MeetingRoomAttendee.OnBasicContactInfoReceived(contacts);
			}
		},

		OnUserAvatarDownloaded: function (userId, serverAvatarUrl, localAvatarUrl) {
			if (this.attendeeListShown) {
				uc.modules.meeting.room.roomAttendee.MeetingRoomAttendee.OnUserAvatarDownloaded(userId, serverAvatarUrl, localAvatarUrl);
			}
		},

		hideMeetingRoomAttendee: function () {
			this.attendeeListShown = false;
		}
	};
})(uc);
