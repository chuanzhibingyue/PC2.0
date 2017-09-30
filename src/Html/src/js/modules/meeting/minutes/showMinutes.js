
(function (uc) {
    if (uc.modules.meeting.showMinutes) {
        return;
    }
    var _meeting,_summaryId,isUEdit;

    function _init(){
        _summaryId = uc.util.StorageUtil.getData("Meeting-Minutes:summaryId");
        _meeting = uc.util.StorageUtil.getData("Meeting-Minutes:Data");

        uc.modules.component.TitleBarButton.init();

        _getShowMinutesDetail();

        $(document).keydown(function(e){
            if(e.which == 27){
                uc.util.WindowUtil.close();
            }
        })
    }

    function _initUEdit() {
        isUEdit = new UE.ui.Editor(0);
        isUEdit.render("discussion");
    }

    function _getShowMinutesDetail(){
        if(_summaryId){
            uc.IMeetingService.getMinutesDetails(_meeting.eventId,_summaryId,function(arg){
            });
        }
    }

    function _setSummary(data){
        _initUEdit();
        isUEdit.addListener('ready', function () {
            isUEdit.setContent(data);
            isUEdit.setDisabled('fullscreen');
            $("#ueditor_0").contents().find("body").keydown(function(e){
                if(e.which == 13){
                   return false;
                }
                if(e.which == 27){
                    uc.util.WindowUtil.close();
                }
            });
        });

       // $(".content-Minutes .content").html(data);
    }

    function _showEditors(userId){
        var userInfo =  uc.IContactService.getCurrentUserInfo();
        if(userId  && userId !== userInfo.userId){
            userInfo = uc.IContactService.getBasicContactInfo(userId);
        }
        if(userInfo){
            $(".show-Minutes .title-Minutes .userName").text(userInfo.displayName);
            $(".show-Minutes .title .userName").text(userInfo.displayName);
        }else{
            $(".show-Minutes .title-Minutes .userName").attr("creatorId",userId);
            $(".show-Minutes .title .userName").attr("creatorId",userId);
        }
    }

    uc.interfaces.IMeetingEvent.OnMinutesDetailsReceived=function(errorCode,eventId,minutesId,creatorId,createTime,content,startTime){
        if(errorCode == 0){
            uc.modules.meeting.showMinutes.showMinutes(errorCode,eventId,minutesId,creatorId,createTime,content,startTime);
        }
    };


    uc.interfaces.IContactEvent.OnContactInfoReceived=function(errorCode, userIds, contacts){
         if(errorCode == 0){
             for(var i = 0;i<contacts.length;i++){
                 $(".show-Minutes .title-Minutes").find(".userName[creatorId="+contacts[i].userId+"]").text(contacts[i].displayName);
                 $(".show-Minutes .title").find(".userName[creatorId="+contacts[i].userId+"]").text(contacts[i].displayName);
             }
         }
    };

    uc.modules.meeting.showMinutes = {

        init: function () {
            _init();
        },

        showMinutes:function(errorCode,eventId,minutesId,creatorId,createTime,content,startTime){
            if(_summaryId == minutesId){
                _showEditors(creatorId);
                _setSummary(content);
            }
        }
    }
})(uc);

jQuery(document).ready(function () {

    uc.init();

    uc.modules.meeting.showMinutes.init();
});