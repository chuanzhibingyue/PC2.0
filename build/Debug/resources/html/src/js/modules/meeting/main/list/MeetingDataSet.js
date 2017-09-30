/**
 * Created by waouyang on 16/4/26.
 */
(function (uc) {
    uc.modules.meeting.MeetingDataSet = function () {
        this.meetingInfoMap = new uc.util.HashMap();
    };

    jQuery.extend(uc.modules.meeting.MeetingDataSet.prototype, {

        addMeetingInfo: function (meetingInfo) {
            if (this.isMeetingInfoExist(meetingInfo.eventId)) {
                this.updateMeetingInfo(meetingInfo);
            } else {
                this.meetingInfoMap.put(this.toKey(meetingInfo.eventId), meetingInfo);
            }
        },

        removeMeetingInfo: function (eventId) {
            this.meetingInfoMap.remove(this.toKey(eventId));
        },

        updateMeetingInfo: function (meetingInfo) {
            if (this.isMeetingInfoExist(meetingInfo.eventId)) {
                this.meetingInfoMap.remove(this.toKey(meetingInfo.eventId));
                this.meetingInfoMap.put(this.toKey(meetingInfo.eventId), meetingInfo);
            } else {
                this.addMeetingInfo(meetingInfo);
            }
        },

        isMeetingInfoExist: function (eventId) {
            return this.meetingInfoMap.isExist(this.toKey(eventId));
        },

        getMeetingInfo: function (eventId) {
            return this.meetingInfoMap.get(this.toKey(eventId));
        },

        getCount: function () {
            return this.meetingInfoMap.size();
        },

        getAllMeetingInfo: function () {
            return this.meetingInfoMap.convertToArray();
        },

        clear: function () {
            this.meetingInfoMap.clear();
        },

        toKey: function (eventId) {
            return '' + eventId;
        }
    })
})(uc);