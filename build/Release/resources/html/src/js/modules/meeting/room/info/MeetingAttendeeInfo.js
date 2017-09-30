/**
 * User: hougang.chen
 * Date: 16-5-13
 * Time: 上午10:26
 */
(function (uc) {
	uc.modules.meeting.room.info.AttendeeInfo = function () {
		this.account = '';
		// 参会人类型：1 内部用户， 2 邮件用户， 3电话用户
		this.type = 0;
		// 参会人状态：0 未处理， 1 接受，  // 2 拒绝
		this.status = 0;
		//昵称
		this.name = '';

	};
	jQuery.extend(uc.modules.meeting.room.info.AttendeeInfo.prototype, {
		update: function (attendeeInfo) {
			if (!attendeeInfo) {
				return;
			}
			var self = this;
			$.each(this, function (key, prop) {
				if (attendeeInfo[key] != undefined) {
					self[key] = attendeeInfo[key];
				}
			});
		},

		reset: function () {
			this.account = '';
			this.type = 0;
			this.status = 0;
			this.name = '';
		}
	})
})(uc);