/**
 * User: hougang.chen
 * Date: 16-5-12
 * Time: 下午5:08
 */
(function (uc) {
	uc.interfaces.IMeetingRoomEvent = {
		//会议室创建事件
		OnMeetingRoomCreated: function (errorCode, meetingRoom) {
		},
		//会议室更新事件
		OnMeetingRoomUpdated: function (errorCode, meetingRoom) {
		},
		//会议室创建会议事件
		OnMeetingCreatedInMeetingRoom: function (errorCode, roomId, meeting) {
		},
		//会议室列表事件
		OnMeetingRoomListReceived: function (errorCode, orgId, lastGetTime, rooms) {
		},
		//检查会议室是否可用事件
		//hasPassword当前会议室是否使用密码
		//isValid字段可以判断会议室是否在有效期内，取值： 0 不在有效期 1在有效期内。
		//mayJoin： 1,可以入会 0不是默认参会人不能入会”，上层可以用这个错误码来区分不是默认参会人的入会失败场景。
		OnCheckJoinMeetingRoom: function (errorCode, roomId, hasPassword, isValid, mayJoin) {
		},
		//检查会议室密码是否有效事件
		OnCheckMeetingRoomPassword: function (errorCode, roomId, password, value) {
		},
		//删除会议室事件
		OnMeetingRoomDeleted: function (errorCode, roomId) {
		},
		//检测当前用户是否有权限创建会议
		OnRightsChecked: function (errorCode) {}
	}
})(uc);