/**
 * User: hougang.chen
 * Date: 16-5-12
 * Time: 上午10:50
 */

(function (uc) {
	uc.modules.meeting.room.createMeetingRoom.AttendeeMap = function () {
		this.AttendeeMap = new uc.util.HashMap();
	};
	jQuery.extend(uc.modules.meeting.room.createMeetingRoom.AttendeeMap.prototype, {

		addAttendee: function (userId, userInfo) {
			userId = this.toKey(userId);
			if (!this.AttendeeMap.isExist(userId)) {
				this.AttendeeMap.put(userId, userInfo);
			} else {
				this.updateAttendee(userId, userInfo);
			}
		},

		deleteAttendee: function (userId) {
			this.AttendeeMap.remove(this.toKey(userId));
		},

		updateAttendee: function (userId, userInfo) {
			userId = this.toKey(userId);
			if (this.AttendeeMap.isExist(userId)) {
				this.deleteAttendee(userId);
				this.AttendeeMap.put(userId, userInfo);
			} else {
				this.addAttendee(userId, userInfo);
			}
		},

		getAttendeeUserInfo: function (userId) {
			return this.AttendeeMap.get(this.toKey(userId));
		},

		isExistAttendee: function (userId) {
			return this.AttendeeMap.isExist(this.toKey(userId));
		},

		getAllAttendee: function () {
			return this.AttendeeMap.convertToArray();
		},

		size: function () {
			return this.AttendeeMap.size();
		},

		clear: function () {
			this.AttendeeMap.clear();
		},

		toKey: function (userId) {
			return '' + userId;
		}
	})
})(uc);
