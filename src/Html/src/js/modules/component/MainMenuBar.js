/**
 * Created by waouyang on 15/7/30.
 */
(function(uc){
    var _this = null;
    uc.modules.component.MainMenuBar = {
        init: function(){
            _this = this;
            this.node = $('.main-menus');
            this.messageMenu = $('.menu-message', this.node);
            this.meetingMenu = $('.menu-meeting', this.node);
	        this.MeetingRoomMenu = $('.menu-meetingRoom', this.node);
            this.contactMenu = $('.menu-contact', this.node);
            this.appMenu = $('.menu-app', this.node);
            this.cloudMenu = $('.menu-cloud', this.node);
            this.noticeMenu = $('.menu-notice', this.node);
            this.pbxMenu = $('.menu-pbx', this.node);
            this.menuItems = $('.menu-item', this.node);
            this.setCapability();
            this.bindEvent();
            this.checkCloudType();
            this.setCallOrPbxVisible();
        },
        bindEvent: function(){
            this.messageMenu.click(function(){
                _this.OnMainMenuClicked(uc.constants.UI.ModuleId.Message);
            });
            this.meetingMenu.click(function(){
                _this.OnMainMenuClicked(uc.constants.UI.ModuleId.Meeting);
            });
            this.contactMenu.click(function(){
                _this.OnMainMenuClicked(uc.constants.UI.ModuleId.Contacts);
            });
            this.appMenu.click(function(){
                _this.OnMainMenuClicked(uc.constants.UI.ModuleId.App);
            });
            this.cloudMenu.click(function(){
                _this.OnMainMenuClicked(uc.constants.UI.ModuleId.Cloud);
            });
            this.noticeMenu.click(function(){
                _this.OnMainMenuClicked(uc.constants.UI.ModuleId.Notice);
            });
            this.pbxMenu.click(function(){
                _this.OnMainMenuClicked(uc.constants.UI.ModuleId.Pbx);
            });
	        this.MeetingRoomMenu.click(function(){
		        _this.OnMeetingRoomMenuClicked(uc.constants.UI.ModuleId.MeetingRoom);
	        });
        },
        OnMainMenuClicked: function(moduleId){
            uc.IUIService.changeModule(moduleId);
        },
	    OnMeetingRoomMenuClicked: function(moduleId){
		    uc.IUIService.changeModule(moduleId);
	    },
        OnModuleChanged: function(moduleId){
            this.menuItems.removeClass('selected');
            switch (moduleId){
                case uc.constants.UI.ModuleId.Message:
                    this.messageMenu.addClass('selected');
                    break;
                case uc.constants.UI.ModuleId.Meeting:
                    this.meetingMenu.addClass('selected');
                    break;
                case uc.constants.UI.ModuleId.Contacts:
                    this.contactMenu.addClass('selected');
                    break;
                case uc.constants.UI.ModuleId.App:
                    this.appMenu.addClass('selected');
                    break;
                case uc.constants.UI.ModuleId.Cloud:
                    this.cloudMenu.addClass('selected');
                    break;
                case uc.constants.UI.ModuleId.Notice:
                    this.noticeMenu.addClass('selected');
                    $(".tooltip",this.noticeMenu).remove();
                    break;
	            case uc.constants.UI.ModuleId.MeetingRoom:
		            this.MeetingRoomMenu.addClass('selected');
		            break;
                case uc.constants.UI.ModuleId.Pbx:
                    this.pbxMenu.addClass('selected');
                    break;
                default :
                    break;
            }
        },
        checkCloudType:function(){
          if(uc.util.StorageUtil.getData('cloudType')!=0){
              this.cloudMenu.removeClass('hide');
          }
        },
        setCallOrPbxVisible:function(){
            var capabilityObj = JSON.parse(uc.util.StorageUtil.getData("capability"));
            if((capabilityObj.call == 1) ||  (capabilityObj.pbx == 1) ){
                this.pbxMenu.removeClass('hide');
            }
        },
        setCapability:function() {

            // add by xiege
            if (uc.constants.Switch.OpenMultiVersion) {
                var version = JSON.parse(uc.util.StorageUtil.getData("capability"));
                if (version) {
	                this.checkVersionIM(version.im);
	                this.checkVersionCalendar(version.calendar);
	                this.checkVersionConf(version.conf);
	                this.checkVersionCloudDiskType(version.cloud_disk_type);
	                this.checkVersionAppCenterr(version.app_center);
	                this.checkVersionMeeting_room(version.meeting_room);
                }
            }
        },

	    checkVersionIM: function(VersionType){
		   if(VersionType == '0'){

		   }
	    },

	    checkVersionCalendar: function(VersionType){
		    if(VersionType == '0'){
			    this.meetingMenu.hide();
			    this.MeetingRoomMenu.hide();
		    }
	    },

	    checkVersionConf: function(VersionType){
		    if(VersionType == '0' || VersionType=='1'){
			    this.meetingMenu.removeClass("hide");
		    }
	    },

	    checkVersionCloudDiskType: function(VersionType){
		    if(VersionType != '0'){
			    this.cloudMenu.removeClass("hide");
		    }
	    },

	    checkVersionAppCenterr: function(VersionType){
		    if(VersionType != '0'){
			    this.appMenu.removeClass("hide");
		    }
	    },

	    checkVersionMeeting_room: function(VersionType){
	    		return;
		    if( VersionType=='1'){
			    this.MeetingRoomMenu.removeClass("hide");
		    }
	    }
    }
})(uc);