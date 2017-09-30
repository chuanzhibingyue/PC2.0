/**
 * Created by waouyang on 15/8/12.
 */
(function(uc){
    var _this = null;
    uc.modules.component.ExtraMenuBar = {
        init: function(){
            _this = this;
            this.cloudExtraMenu = $('.extra-options .icon-cloud');
            this.linkExtraMenu = $('.extra-options .icon-link');
            this.startExtraMenu = $('.extra-options .icon-start');
            this.searchExtraMenu = $('.extra-options .icon-search .hide');
            this.settingExtraMenu = $('.extra-options .icon-settings');
            this.startContainer = $('.start-container');
            this.startChatMenu = $('.start-chat', this.startContainer);
            this.createProjectMenu = $('.create-project', this.startContainer);
            this.createMeetingMenu = $('.create-meeting', this.startContainer);
            this.createCloudMenu = $('.create-cloud', this.startContainer);
            this.settingContainer = $('.setting-container');
            this.forumMenu = $('.forum', this.settingContainer);
            this.settingMenu = $('.setting', this.settingContainer);
            this.microMsg = $('.new-weibo-msg', this.linkExtraMenu);
            this.bindEvent();
            this.setCloudCreateIcon();
            this.setCapability();
        },
        bindEvent: function(){
            uc.util.PageUtil.addClickBodyHideNode(this.startExtraMenu[0], uc.util.EventUtil.hitch(this, 'showStartContainer'),
                uc.util.EventUtil.hitch(this, 'hideStartContainer'));
//            uc.util.PageUtil.addClickBodyHideNode(this.settingExtraMenu[0], uc.util.EventUtil.hitch(this, 'showSettingContainer'),
//                uc.util.EventUtil.hitch(this, 'hideSettingContainer'));

            this.cloudExtraMenu.click(function(){
                uc.IUIService.changeModule(uc.constants.UI.ModuleId.Cloud);
            });
            this.linkExtraMenu.click(function(){
                uc.IUIService.openMyLinkPage();
            });
            //this.startExtraMenu.click(function(){
            //    _this.showStartContainer();
            //});
            this.searchExtraMenu.click(function(){
                uc.IUIService.startSearch();
            });
            this.settingExtraMenu.click(function(e){
                _this.onSettingMenuClicked();
                //_this.showSettingContainer();
            });
            this.startChatMenu.click(function(){
                _this.onStartChatMenuClicked();
            });
            this.createProjectMenu.click(function(){
                _this.onCreateProjectMenuClicked();
            });
            this.createMeetingMenu.click(function(){
                _this.onCreateMeetingMenuClicked();
            });
            this.createCloudMenu.click(function(){
                _this.onCreateCloudMenuClicked();
            });
            this.forumMenu.click(function(){
                _this.onForumMenuClicked();
            });
            this.settingMenu.click(function(){
                _this.onSettingMenuClicked();
            });
        },
        showStartContainer: function(){
            this.setCloudCreateIcon();
            this.startContainer.removeClass('hide');
            this.startContainer.css({'bottom':(75 - this.startContainer.height()/2) + 'px'});
            this.startContainer.find('a').css({'top':this.startContainer.height()/2 + 'px'});
        },
        hideStartContainer: function(){
            this.startContainer.addClass('hide');
        },
        showSettingContainer: function(){
            this.settingContainer.removeClass('hide');
        },
        hideSettingContainer: function(){
            this.settingContainer.addClass('hide');
        },
        onStartChatMenuClicked: function(){
            this.hideStartContainer();
            uc.IUIService.startChat();
        },
        onCreateProjectMenuClicked: function(){
            this.hideStartContainer();
            uc.IUIService.createProject();
        },
        onCreateMeetingMenuClicked: function(){
            this.hideStartContainer();
            var param = {
                type: uc.constants.Meeting.MeetingCreateType.Default,
                isCreate: true
            };

            uc.IUIService.createMeeting("ExtraMenuCreateMeeting", param);
        },
        onCreateCloudMenuClicked: function(){
            this.hideStartContainer();
            uc.IUIEvent.OnCreateCloud();
        },
        onForumMenuClicked: function(){
            this.hideSettingContainer();
        },
        onSettingMenuClicked: function(){
            //this.hideSettingContainer();
            uc.IUIService.openSettingWindow();
        },
        showMicroBlogMsg: function () {
            this.microMsg.show();
        },
        hideMicroBlogMsg: function () {
            this.microMsg.hide();
        },
        showNewVersion: function () {
            this.settingExtraMenu.addClass('pop-tips');
        },
        hideNewVersion: function () {
            this.settingExtraMenu.removeClass('pop-tips');
        },
        setCloudCreateIcon:function(){
            if(uc.util.StorageUtil.getData('cloudType')==0 || uc.util.StorageUtil.getData('CloudLoginResult')!=0){
                this.createCloudMenu.hide();
                this.startContainer.find("a").removeClass().addClass("leftIconMin");
            }else{
                this.createCloudMenu.show();
                this.startContainer.find("a").removeClass().addClass("leftIconMax");
            }
        },
        setCapability: function() {
            // add by xiege
            if (uc.constants.Switch.OpenMultiVersion) {
                var version = JSON.parse(uc.util.StorageUtil.getData("capability"));
                if (version) {
                    switch (version.im) {
                        case "0":
                            //this.startChatMenu.addClass("hide");
                            break;
                    }
                    //switch (version.calendar) {
                    //}
                    switch (version.conf) {
                        case "0":
                            this.createMeetingMenu.addClass("hide");
                            this.startExtraMenu.attr('qtips', uc_resource.Extra.Create_2);
                            //this.createMeetingMenu.remove();
                            break;
                    }
                    switch (version.call) {
                    }
                    switch (version.cloud_disk_type) {
                    }
                    switch (version.app_center) {
                    }
                    switch (version.app_moments) {
                    }
                    switch (version.scan) {
                    }
                    switch (version.remote_assistant) {
                    }
                }
            }
        }
    }
})(uc);