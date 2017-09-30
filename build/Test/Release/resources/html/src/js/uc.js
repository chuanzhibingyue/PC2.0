/**
 * Created by senlin.ouyang on 2015/7/15.
 */
(function(window){
    var disableContextMenu = function(){
        jQuery(document.body).contextmenu(function(){
            return false;
        });
    };
    window.uc = {
        interfaces: {},
        service: {},
        listener: {
            service: {},
            event: {}
        },
        constants: {},
        util: {},
        modules: {
            component: {},
            login: {},
            main: {},
            conversation: {},
            contact: {},
            chooseContacts: {},
            group: {},
            project: {},
            org: {},
            meeting: {
	            room:{
		            createMeetingRoom:{},
		            createMeeting:{},
		            info:{},
		            roomAttendee:{},
		            joinMeetingRoomMeeting:{}
	            }
            },
            search: {},
            setting: {},
            chat: {},
            app: {},
            profile: {},
            emoji:{},
            code:{},
            image: {},
            password: {},
            proxy: {},
            update: {},
            openApi: {},
            settingUpdate: {},
            serviceTerms: {},
            feedback: {},
            microblog: {},
            cloud: {},
            comment: {},
	        draft: {},
            video: {},
            bubble: {},
            remoteAssistance: {},
	        notice:{},
            cache: {},
            call: {},
			pbx:{},	    
	        AVdetection:{},
	        historymsg:{},
	        screenSharing:{},
            map: {}
        },
        ui: {},
        init: function(){
            uc.listener.manager && uc.listener.manager.init();
            disableContextMenu();
        },
        quit: function(){
            uc.IClientService.quit();
        },
        /**
         * get client status,
         * @returns  client status, status is defined in uc.constants.ClientStatus
         */
        getClientStatus: function(){
            return uc.modules.main.Main ? uc.modules.main.Main.getClientStatus() : uc.constants.ClientStatus.Connected;
        }
    };
    uc.uc = uc;
})(window);