/**
 * User: hougang.chen
 * Date: 16-5-12
 * Time: 下午5:08
 */
(function (uc) {
	uc.interfaces.IMeetingRoomService = {
		//会议室创建
		createMeetingRoom: function (attendees, title, remarks, password, startTime, endTime, roomHostId, orgId, isSetPassword, defaultAttendsVisible, defaultAttendsJoin, callback) {
		},
		//会议室更新
		updateMeetingRoom: function (roomId, attendees, title, remarks, password, startTime, endTime, roomHostId, orgId, isSetPassword, defaultAttendsVisible, defaultAttendsJoin, callback) {
		},
		//会议室创建会议
		createMeetingInMeetingRoom: function (attendees, roomId, title, summary,startTime, duration, callback) {
		},
		//获取会议室列表
		getMeetingRoomList: function (orgId, callback) {
		},
		//检查会议室是否可用
		checkJoinMeetingRoom: function (roomId, callback) {
		},
		//检查会议室密码是否有效
		checkMeetingRoomPassword: function (roomId, password, callback) {
		},
		//删除会议室
		deleteMeetingRoom: function (roomId, callback) {
		},
		//检测当前用户是否有权限创建会议室
		checkRightsOfCreateRoom: function(callback){}
	}
})(uc);