/**
 * User: hougang.chen
 * Date: 16-5-13
 * Time: 下午2:49
 */
(function (uc) {
	uc.modules.meeting.room.roomDataMap = function () {
		this.roomDataMap = new uc.util.HashMap();
	};
	jQuery.extend(uc.modules.meeting.room.roomDataMap.prototype, {

		addRoom: function (roomId, roomInfo) {
			roomId = this.toKey(roomId);
			if (!this.roomDataMap.isExist(roomId)) {
				this.roomDataMap.put(roomId, roomInfo);
			} else {
				this.updateRoom(roomId, roomInfo);
			}
		},

		deleteRoom: function (roomId) {
			this.roomDataMap.remove(this.toKey(roomId));
		},

		updateRoom: function (roomId, roomInfo) {
			roomId = this.toKey(roomId);
			if (this.roomDataMap.isExist(roomId)) {
				this.deleteRoom(roomId);
				this.roomDataMap.put(roomId, roomInfo);
			} else {
				this.roomDataMap(roomId, roomInfo);
			}
		},

		getRoomInfo: function (roomId) {
			return this.roomDataMap.get(this.toKey(roomId));
		},

		isExistRoom: function (roomId) {
			return this.roomDataMap.isExist(this.toKey(roomId));
		},

		getAllRoom: function () {
			return this.roomDataMap.convertToArray();
		},

		size: function () {
			return this.roomDataMap.size();
		},

		clear: function () {
			this.roomDataMap.clear();
		},

		toKey: function (roomId) {
			return '' + roomId;
		}
	})
})(uc);