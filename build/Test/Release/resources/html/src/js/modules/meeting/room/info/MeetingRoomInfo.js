/**
 * User: hougang.chen
 * Date: 16-5-13
 * Time: 上午10:10
 */
(function (uc) {
	uc.modules.meeting.room.info.MeetingRoomInfo = function () {
		this.conferenceId = 0;
		this.roomId = 0;
		this.orgId = 0;
		this.isSetPassword = 1;
		this.hostPassword = '';
		this.attendeePassword = '';
		this.defaultAttendsVisible = 0;
		this.defaultAttendsJoin = 1;
		this.roomPassword = '';
		this.startTime = 0;
		this.endTime = 0;
		this.roomHostId = '';
		this.attendees = [];
		this.title = '';
		this.remarks = '';
		this.hostId = '';

	};
	jQuery.extend(uc.modules.meeting.room.info.MeetingRoomInfo.prototype, {
		update: function (roomInfo) {
			if (!roomInfo) {
				return;
			}
			var self = this;
			$.each(this, function (key, prop) {
				if (roomInfo[key] != undefined) {
					if (self[key] instanceof Array && roomInfo[key] instanceof Array) {
						for(var i=0;i<roomInfo[key].length;i++){
							self[key].push(roomInfo[key][i]);
						}
					} else {
						self[key] = roomInfo[key];
					}
				}
			});
			this.hostId = this.roomHostId;
		},

		reset: function () {
			this.conferenceId = 0;
			this.roomId = 0;
			this.orgId = 0;
			this.isSetPassword = 0;
			this.hostPassword = '';
			this.attendeePassword = '';
			this.defaultAttendsVisible = 0;
			this.defaultAttendsJoin = 0;
			this.roomPassword = '';
			this.startTime = 0;
			this.endTime = 0;
			this.roomHostId = '';
			this.attendees = [];
			this.title = '';
			this.remarks = '';
		}
	})
})(uc);