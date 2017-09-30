/**
 * Created by senlin.ouyang on 2015/7/23.
 */
(function(uc){
    uc.util.AvatarUtil = {
        /**
         * @desc Download avatar from server, via FS
         * @param avatarUrl
         * @param savePath
         * @param callback
         */
        downloadAvatar:function(avatarUrl, savePath, callback){
            if(!avatarUrl){
                return;
            }
            uc.IFSService.downloadFile(avatarUrl, savePath, callback);
        },
        composeGroupAvatar: function (localAvatarUrlArray, defaultAvatar) {
            var str = '';
            var len = localAvatarUrlArray.length;
            if (len == 2) {
                str =   '<div class="group-avatar">' +
                            '<img src="' + localAvatarUrlArray[1] +  '" onerror="uc.util.ImageUtil.OnImageLoadError(this,\'' + defaultAvatar + '\')" draggable="false" class="avatar-attendee round-image">' +
                            '<img src="' + localAvatarUrlArray[0] +  '" onerror="uc.util.ImageUtil.OnImageLoadError(this,\'' + defaultAvatar + '\')" draggable="false" class="avatar-admin round-image">' +
                        '</div>';
            }  else if (len == 1) {
                str =   '<div class="group-avatar">' +
                			'<img src="' + defaultAvatar +  '" onerror="uc.util.ImageUtil.OnImageLoadError(this,\'' + defaultAvatar + '\')" draggable="false" class="avatar-attendee round-image">' + 
                      '<img src="' + localAvatarUrlArray[0] +  '" onerror="uc.util.ImageUtil.OnImageLoadError(this,\'' + defaultAvatar + '\')" draggable="false" class="avatar-admin round-image">' +
                		'</div>';
            } else {
                str =   '<div class="group-avatar">' +	
                			'<img src="' + defaultAvatar +  '" class="avatar-attendee round-image">' + 
                		'</div>';
            }
            return str;
        },
        composeMeetingAvatar: function(meetingStartTime, isGNet, isMeetingCancelled){
            var iconClass = '';
            if(isGNet){
                iconClass += ' cloud';
            }
            if(isMeetingCancelled){
                iconClass += ' cancelled';
            }
            var timeStr,time;
            if(meetingStartTime == 0){
                timeStr = '00:00';
            }else{
                time= uc.util.TimeUtil.getMeetingStartTimeString(meetingStartTime, null, uc.settings.meeting.timeZone.offset);
                timeStr=time.timeString;
            }
            if(iconClass.indexOf('cloud') != -1){
                if(time){
                    if(time.type==1){
                        timeStr = '<span class="cloud-time cloud-day">' + timeStr + '</span>';
                    }else if(time.type==2){
                        timeStr = '<span class="cloud-time cloud-month">' + timeStr + '</span>';
                    }else{
                        timeStr = '<span class="cloud-time">' + timeStr + '</span>';
                    }
                }else{
                    timeStr = '<span class="cloud-time">' + timeStr + '</span>';
                }
            }
            return '<div class="meeting-icon' + iconClass + '">' + timeStr + '</div>';
            //if(meetingStartTime == 0){
            //    return '<div class="meeting-icon' + iconClass + '">00:00</div>';
            //}else{
            //    var timeStr = uc.util.TimeUtil.getMeetingStartTimeString(meetingStartTime, null, uc.settings.meeting.timeZone.offset);
            //    return '<div class="meeting-icon' + iconClass + '">' + timeStr + '</div>';
            //}
        },
        composeContactAvatar: function(localAvatarUrl, defaultAvatar){
            return  '<img src="' + localAvatarUrl + '" onerror="uc.util.ImageUtil.OnImageLoadError(this,\'' + defaultAvatar + '\')" draggable="false" class="avatar round-image">';
        },
        composeProjectAvatar: function(localProjectUrl, defaultAvatar){
            return  '<img src="' + localProjectUrl + '" onerror="uc.util.ImageUtil.OnImageLoadError(this,\'' + defaultAvatar + '\')" draggable="false" class="avatar round-image">';
        },
        composeLocalImageAvatar: function(localAvatarUrl, defaultAvatar, isCacelled){
            var isCancel = isCacelled ? 'cancelled' : '';
            return  '<img src="' + localAvatarUrl + '" onerror="uc.util.ImageUtil.OnImageLoadError(this,\'' + defaultAvatar + '\')" draggable="false" class="avatar round-image '+ isCancel +'">';
        }
    }
})(uc);