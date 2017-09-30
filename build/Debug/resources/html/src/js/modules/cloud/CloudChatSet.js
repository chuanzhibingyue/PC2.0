(function(uc){
    uc.modules.cloud.CloudChatSet = {
        init: function(){
            this.groupInfo="";
            this.bar=$(".switch-bar");
            this.getCurrentChatInfo();
            uc.modules.component.TitleBarButton.init();
            uc.modules.cloud.CloudChatSet.initPage();
            uc.modules.cloud.CloudChatSet.bindEvent();
        },
        bindEvent:function(){
            var self=this;
            this.bar.click(function(){
                self.setValue($(this));
                var groupId=self.groupInfo.id;
                var newStatus=self.getValue();
                uc.IGroupService.setDoNotDisturb(groupId,newStatus,function(result){});
            });
        },
        setValue:function(that){
            var $this =that;
            if ($this.hasClass("switch-bar-left")) {
                $this.removeClass("switch-bar-left").addClass("switch-bar-right");
            }
            else {
                $this.removeClass("switch-bar-right").addClass("switch-bar-left");
            }
        },
        getValue:function () {
            if (this.bar.hasClass("switch-bar-right")) {
                return 1;
            }
            else {
                return 0;
            }
        },
        initPage:function(){
            if(this.groupInfo){
                if(this.groupInfo.disturbState==1){
                    this.bar.addClass("switch-bar-right").removeClass("switch-bar-left");
                }else{
                    this.bar.addClass("switch-bar-left").removeClass("switch-bar-right");
                }
            }
        },
        getCurrentChatInfo:function(){
           var groupId=uc.util.StorageUtil.getData("cloudChatSetGroupId");
            if(groupId){
                var groupInfo = uc.IGroupService.getGroupInfo(groupId);
                if(groupInfo){
                    this.groupInfo=groupInfo;
                }
            }
        }
    }
    jQuery(document).ready(function () {
        uc.init();
        uc.modules.cloud.CloudChatSet.init();
    })
})(uc);