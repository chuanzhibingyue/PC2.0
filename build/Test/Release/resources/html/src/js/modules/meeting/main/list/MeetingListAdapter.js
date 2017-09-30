/**
 * Created by waouyang on 16/4/26.
 */
(function(uc){
    uc.modules.meeting.MeetingListAdapter = function(meetingListManager){
        this.meetingListManager = meetingListManager;
        this.meetingInfoList = null;
        this.dataSetObserver = null;
    };

    jQuery.extend(uc.modules.meeting.MeetingListAdapter.prototype, {

        init: function(){
            this.meetingInfoList = this.meetingListManager.getSortedMeetingList();
        },

        setDataSetObserver: function(dataSetObserver){
            this.dataSetObserver = dataSetObserver;
        },

        notifyDataSetChanged: function(){
            this.meetingInfoList = this.meetingListManager.getSortedMeetingList();
            this.dataSetObserver.notifyDataSetChanged();
        },

        getCount: function(){
            return this.meetingInfoList.length;
        },

        getMeetingInfo: function(index){
            return this.meetingInfoList[index];
        }
    })
})(uc);