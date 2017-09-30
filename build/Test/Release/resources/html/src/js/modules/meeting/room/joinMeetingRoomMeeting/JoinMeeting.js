/**
 * User: hougang.chen
 * Date: 16-5-17
 * Time: 下午5:00
 */
(function (uc) {
	uc.modules.meeting.room.joinMeetingRoomMeeting.JoinMeeting = {
		init: function () {

			this.roomData = uc.util.StorageUtil.getData("joinMeetingRoomMeeting");
			this.roomInfo = this.roomData.roomInfo;
			if (this.roomData == null) {
				uc.util.LogUtil.warn('uc.modules.meeting.room.joinMeetingRoomMeeting.JoinMeeting', 'init', 'joinMeetingRoomMeeting', this.roomData);
				return;
			}
			this.baseNode = $('.meetingRoom_JoinMeeting');
			this.tirggerSource = this.roomData.key;
			this.closeBtn =  this.baseNode.find('.cancel');

			this.joinBtn = this.baseNode.find('.joinMeeting');
			this.PSWInput = this.baseNode.find('.password');
			this.pswErrorTips = this.baseNode.find('.pswErrorTips');
			this.pswErrorTips.hide();

			this.PSWInput.focus();
			this.bindEvents();
		},

		bindEvents: function () {
			var self = this;
			this.closeBtn.on('click', function () {
				uc.IUIService.triggerCrossWindowEvent(self.tirggerSource, {cancel: true});
				uc.util.WindowUtil.close();
			});

			//入会按钮点击事件
			this.joinBtn.on('click', function () {
				if ($(this).hasClass('disabled')) {
					return false;
				}
				$(this).addClass('disabled');
				var psw = self.PSWInput.val().trim();
				if (psw.length <= 0) {
					return false;
				}
				uc.IMeetingRoomService.checkMeetingRoomPassword(self.roomInfo.roomId, psw, function (args) {
					if (args[0] != uc.ErrorCode.Success) {
						self.joinBtn.removeClass('disabled');
					}
				});
			});

			//密码框输入事件
			this.PSWInput.on('input', function (e) {
				if ($(this).val().trim() == "") {
					self.joinBtn.addClass('disabled');
				} else {
					self.joinBtn.removeClass('disabled');
				}
			});
		},

		OnCheckMeetingRoomPassword: function (errorCode, roomId, password, value) {
			if (errorCode == uc.ErrorCode.Success && this.roomInfo.roomId == roomId, value) {
				//value, 为0代表密码无效 ，1 为有效
				if (value == 1) {
					//向主窗口发送密码检查通过，可以入会
					uc.IUIService.triggerCrossWindowEvent(this.tirggerSource, {success: true});
					uc.util.WindowUtil.close();
				} else {
					this.joinBtn.removeClass('disabled');
					this.showPswErrorTips();
					this.PSWInput.focus();
				}
			} else {
				this.joinBtn.removeClass('disabled');
				this.showPswErrorTips();
				this.PSWInput.focus();
			}
		},
		showPswErrorTips: function () {
			if(this.pswErrorTips){
				this.pswErrorTips.slideToggle('fast').delay(1000).fadeOut("fast");
			}
		}
	}
})(uc);

jQuery(document).ready(function () {
	uc.init();
	uc.modules.meeting.room.joinMeetingRoomMeeting.JoinMeeting.init();
});
