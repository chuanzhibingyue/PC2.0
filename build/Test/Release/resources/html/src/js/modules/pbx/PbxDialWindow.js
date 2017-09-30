(function(uc){
    var _this = null;
    uc.modules.pbx.PbxDialWindow = {
        init: function(){
            _this = this;
            this.code = "+86";
            uc.modules.component.TitleBarButton.init(uc.constants.UI.WindowCloseBehavior.Close);
            this.nodeHtml = $(".panel");
            this.bindEvent();
            this.showintlTelInput();
        },
        bindEvent:function(){
            var self=this;
            $(document).click(function(e){
                self.OnBodyClick(e);
            });
            this.nodeHtml.on("click",".pbx-tel-num a", function (e) {
                $(".pbx-dial-icon-search-del").show();
                $(".panel-input").addClass("panel-input-font-size").insertAtCaret($(this).text());
            });
            this.nodeHtml.on("keypress",".panel-input", function (e) {
                var code = (e.keyCode ? e.keyCode : e.which);
                if(code == 13){
                    $(".pbx-call a").trigger("click");
                    return false;
                }
                return (code >= 48 && code <= 57) || code == 42 || code == 35;
            });
            this.nodeHtml.on("input propertychange",".panel-input", function (e) {
                var text= $.trim($(".panel-input",self.nodeHtml).val());
                if(!text){
                    $(".pbx-dial-icon-search-del",self.nodeHtml).hide();
                    $(".panel-input").removeClass("panel-input-font-size");
                }else{
                    $(".pbx-dial-icon-search-del",self.nodeHtml).show();
                    $(".panel-input").addClass("panel-input-font-size");
                }
            });
            this.nodeHtml.on("click",'.pbx-dial-icon-search-del',function(){
                self.getDialFocus();
            });
            this.nodeHtml.on("click",".pbx-call a", function (e) {
                var phoneNumber = parseInt($.trim($(".panel-input").val()));
                if(!phoneNumber){
                    self.getDialFocus();
                    return false;
                }
                if($.inArray(phoneNumber,uc.constants.Pbx.PbxCallBackList) > -1){
                    self.showErrorTips();
                    return false;
                }
                if(!uc.util.StringUtil.IsTelephone(phoneNumber)){
                    self.showErrorTips();
                    return false;
                }
                if((phoneNumber.toString()).length >=15){
                    self.showErrorTips();
                    return false;
                }
                if((phoneNumber.toString()).length <= 6){
                    var code=(parseInt(phoneNumber));
                }else{
                    var code=self.code+(parseInt(phoneNumber));
                }
                uc.IUIService.triggerCrossWindowEvent(uc.constants.UI.CrossWindowEventName.PbxDialWindowSwitchMainWindow,{
                   "phoneNumber":code
                });
                uc.util.WindowUtil.close();
            });
            this.nodeHtml.on("click",".pbx-icon-down,.pbx-code", function (e) {
                $('.pbx-code-list-panel', self.nodeHtml).show();
            });
            this.nodeHtml.on("click",".pbx-code-list li", function (e) {
                var codeName=$(this).find(".codeName").attr("rel");
                self.code=$(this).find(".code").text();
                $('.pbx-code', self.nodeHtml).html(codeName+"&nbsp;");
                $('.pbx-icon-panel', self.nodeHtml).html("");
                $(this).find(".pbx-icon-panel").html("<span class='icon-call_singleSelected'></span>");
                $('.pbx-code-list-panel', self.nodeHtml).hide();
            });
        },
        showErrorTips:function(){
            $(".error-tips").fadeIn(1500);
            setTimeout(function(){
                $('.error-tips').slideUp();
            },2000);
            this.getDialFocus();
        },
        getDialFocus:function(){
            $(".panel-input").val("").removeClass("panel-input-font-size").get(0).focus();
            $(".pbx-dial-icon-search-del").hide();
},
        OnBodyClick:function(e){
            var target  = $(e.target);
            if(!target.closest(".pbx-code-list-panel").length
                && !target.closest(".pbx-code").length
                && !target.closest(".pbx-icon-down").length){
                    $(".pbx-code-list-panel").hide();
                }
            },
        showintlTelInput: function (code) {
            var selectedDialCode = "+86";
            if(code){
                selectedDialCode = code
            }
            var str='',codeHtml="",codeName="",code="",id="";
            var data=uc_resource.IntlTelInput.intlTeldata;
            for(var i= 0,len=data.length;i<len;i++){
                codeHtml="";
                codeName=data[i].name;
                code="+"+data[i].dialCode;
                id="code"+data[i].dialCode;
                if(codeName.length>=6){
                    codeName=codeName.substr(0,6)+"...";
                }
                if(selectedDialCode == code){
                    codeHtml="<span class='icon-call_singleSelected'></span>";
                }
                str+="<li id='"+id+"'>" +
                    "<span class='pbx-icon-panel'>"+codeHtml+"</span>" +
                    "<span class='codeName' rel="+data[i].name+">"+codeName+"</span>" +
                    "<span class='code'>"+code+"</span>" +
                    "<li>";
            }
            $('.pbx-code-list', this.nodeHtml).html(str);
        }
    };
    jQuery(document).ready(function () {
        uc.init();
        uc.modules.pbx.PbxDialWindow.init();
    })
})(uc);