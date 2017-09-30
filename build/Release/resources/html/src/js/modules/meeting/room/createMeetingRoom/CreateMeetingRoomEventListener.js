/**
 * User: hougang.chen
 * Date: 16-5-5
 * Time: 下午3:36
 */
(function (uc) {
	uc.modules.meeting.room.createMeetingRoom.CreateMeetingRoomEventListener = function(){

	};
	jQuery.extend(uc.modules.meeting.room.createMeetingRoom.CreateMeetingRoomEventListener.prototype,
		uc.interfaces.IContactEvent,
		uc.interfaces.IUIEvent,
		uc.interfaces.IMeetingRoomEvent,{
			/**IMeetingRoomEvent**/
			//会议室创建事件
			OnMeetingRoomCreated: function (errorCode, meetingRoom) {
				uc.modules.meeting.room.createMeetingRoom.CreateMeetingRoomManager.OnMeetingRoomCreated(errorCode, meetingRoom);
			},
			//会议室更新事件
			OnMeetingRoomUpdated: function (errorCode, meetingRoom) {
				uc.modules.meeting.room.createMeetingRoom.CreateMeetingRoomManager.OnMeetingRoomUpdated(errorCode, meetingRoom);

			}
		});
})(uc);