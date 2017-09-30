(function(uc){
    uc.modules.chooseContacts.contactAddEmail = {
       init:function(){
           uc.modules.component.TitleBarButton.init();
          this.emailBody=$(".emailBody");
          this.bindEvent();
          this.count=0;
       },
       bindEvent:function(){
           var _t=this;

           this.emailBody.find("input").keyup(function(e){
               var inputVal=$(this).val();
               inputVal= $.trim(inputVal);
               var reg,current,currentVal;
               if(e.which == 186){
                   if(inputVal[inputVal.length-2]==";"){
                       var text=inputVal.replace(";;",";");
                       $(this).val(text);
                       return;
                   }
                   current=inputVal.lastIndexOf(";");//.indexOf(";");
                   if(!_t.count){
                       currentVal=inputVal.substr(0,current);
                   }else{
                      currentVal=inputVal.substring(_t.count+1,inputVal.length-2);
                   }
                   if(currentVal){
                       reg= _t.regEmail(currentVal);
                   }
               }
               if(e.which == 13){
                   if(inputVal[inputVal.length-1]==";"){
                       return;
                   }
                   current=inputVal.lastIndexOf(";");
                   if(!_t.count){
                      currentVal=inputVal.substr(0,inputVal.length-1);
                   }else{
                       currentVal=inputVal.substring(current+1,inputVal.length-1);
                   }
                   if(currentVal){
                       reg=_t.regEmail(currentVal);
                       if(reg){
                           $(this).val(inputVal+";");
                       }
                   }

               }
               if(e.which == 8 || e.which == 46){
                   _t.count=inputVal.lastIndexOf(";");
               }
               if(reg){
                   _t.count=inputVal.lastIndexOf(";");
               }
           });

           $(".email-save .cancel").click(function(){
               uc.util.WindowUtil.close();
           });
           $(".email-save .save").click(function(){
              var email=$(".emailBody").find("input").val();
               if(email.indexOf(",")>-1)
               {
                   email  = email.replace(/,/g,";");
               }

               email= email.split(";");
               var emailValue={},count=0;
               for(var i=0;i<email.length;i++){
                   if(email[i]){
                       if(uc.modules.chooseContacts.contactAddEmail.regEmail(email[i])){
                           emailValue[email[i]]= email[i];
                       }else{
                          count++;
                       }

                   }
               }
               if(!count){
                   uc.IUIService.triggerCrossWindowEvent("contactAddEmail",emailValue);
                   uc.util.WindowUtil.close();
               }
           })
       },
       regEmail:function(text){
           var email = /^[a-z0-9]+([._\\-]*[a-z0-9])*@([a-z0-9]+[-a-z0-9]*[a-z0-9]+.){1,63}[a-z0-9]+$/g;
           if (!email.test(text)){
               uc.modules.component.PromptMessageBox.open({ title: uc_resource.WindowTitle.Notification, message: uc_resource.ChooseContacts.RegInputEmail, buttonMode: "ok", OkText: uc_resource.Common.OK }, function (result) {
                   this.close();
               });
               return false;
           }
           return true;
       }
    };
    jQuery(document).ready(function(){
        uc.init();
        uc.modules.chooseContacts.contactAddEmail.init();
    });
})(uc);