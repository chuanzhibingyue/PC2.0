/**
 * Created by pan.li on 2015/9/17.
 */
(function (uc) {
    uc.modules.settingUpdate.SettingUpdate = {
        init: function () {
            this.cancel = $(".cancel");//取消升级检测
            this.update = $(".update-now");//立即更新
            this.version = $(".version-num");//检查更新的版本号
            this.confirm = $(".confirm");//没有新版本需要更新
            this.close = $(".icon-close");//没有新版本需要更新

            this.bindEvent();
        },
        bindEvent: function(){
            uc.IClientService.checkUpdates();

            document.onkeydown = function (e) {
                if (e.keyCode == 27) {
                    uc.util.WindowUtil.close();
                }
            };

            this.cancel.on("click",function(){
                uc.util.WindowUtil.close();
            });

            this.close.on("click",function(){
                uc.util.WindowUtil.close();
            });

            this.update.on("click",function(){
                uc.util.WindowUtil.hideAll();
                setTimeout(function () {
                    uc.IClientService.update();
                }, 300)
            });

            this.confirm.on("click",function(){
                uc.util.WindowUtil.close();
            });
        },
        OnUpdatesProgress:function(checkProgress){
            $("#bar").css("width","0%");
            //进度条的速度，越小越快
            //var speed = 100;
            //var bar = setInterval(function(){
                var nowWidth = parseInt(checkProgress);
                //宽度要不能大于进度条的总宽度
                if(nowWidth <= 100){
                    var barWidth = (nowWidth)+"%";
                    $("#bar").css("width",barWidth);
                }
                //else{
                //    //进度条读满后，停止
                //    clearInterval(bar);
                //}
            //},speed);
        },
        OnUpdatesStatus: function(errorCode,updateFlag,version,notes){
            if((errorCode == 0)&&(updateFlag == 1)){
                $(".loading").hide();
                $(".new-version").show();
                this.version.html(version);
            }else if((errorCode == 0)&&(updateFlag == 0)){
                $(".loading").hide();
                $(".new-version").hide();
                $(".version").show();
            }else if(errorCode != 0){
                $(".loading").hide();
                $(".checkup_failed").show();
            }
        }
    };
    jQuery(document).ready(function () {
        uc.init();
        uc.modules.settingUpdate.SettingUpdate.init();
    })
})(uc);