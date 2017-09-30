/**
 * Created by senlin.ouyang on 2015/7/17.
 */
(function (uc) {
    var _this = null;
    var startTime = 0, endTime = 0, flag = false;
    uc.modules.login.Login = {
        init: function () {
			uc.util.LogUtil.info("Login","Init","DOM ready, start to work!!", null);
            _this = this;
            _this.isLoging = false;
	        this.headListNode = $('#nav1');
            this.body = $('body');
            this.bindEvent();
            this.initUI();
        },
        initUI: function () {
            _this.accountInfos = [];
            uc.modules.component.TitleBarButton.init(uc.constants.UI.WindowCloseBehavior.Quit);
            uc.util.WindowUtil.setSize(465, 640);
            uc.util.WindowUtil.setOnTop(false);
            var accountsArray = uc.IClientService.getHistoryLoginAccounts();
            //uc.util.LogUtil.info('Login', 'getHistoryLoginAccounts', 'accountsArray:',accountsArray);
            var defaultAvatar = uc.constants.UI.DefaultLocalUrl.Avatar;

            if (accountsArray == null || accountsArray.length == 0) {
                return;
            }

            if (accountsArray.length == 1) { //单账号
                this.accountInfos = accountsArray;
                this.applyUserInfo(accountsArray[0]);
            }

            {//多账号
                this.accountInfos = accountsArray;
                this.initMultiAccountUI();
                var This = this;
                //点击选中账号
                $('#nav1').on("click", ".user",
                    function(e){
                        e.preventDefault();
                        e.stopPropagation();
                        var pos = Number($(this).attr("pos_num"));
                        This.applyUserInfo(accountsArray[pos]);
                        $('.avatar-node').show();
                        $('.multi-avatar').hide();
                    }
                );
                $('#nav1').on("click", ".nouser",
                    function(e){
                        e.preventDefault();
                        e.stopPropagation();
                        $('.avatar-node').show();
                        $('.multi-avatar').hide();
                        $(".avatar")[0].onerror="uc.util.ImageUtil.OnImageLoadError(this,\'' + defaultAvatar + '\')";
                        $('.avatar').attr("src",  "images/login/logo.png");
                        $("input[type=text]").val("");
                        $("input[type=password]").val("");
                        $("input[type=text]").focus();
                        $('.avatar').addClass("user");
                        $("input[name=rememberPwd]:checkbox").attr("checked",false);
                        $("input[name=autoLogin]:checkbox").attr("checked",false);
                    }
                );

                $('#nav1').on("mouseover", ".user", function(){
                        $('.user').removeClass("sel");
                        $(this).addClass("sel");
                        $('.close-avatar').hide();
                        $('.close-avatar',  $(this).parent()).show();
                    }
                );



            }

            //自动登录
            var data = uc.util.StorageUtil.getData("login-data");
            uc.util.LogUtil.info('Login', 'StorageUtil.getLoginData', 'data = ' + data,null);

            if (data == null) {
                this.applyUserInfo(accountsArray[0]);
                uc.util.LogUtil.info('Login', 'applyUserInfo', 'autoLogin = ' + accountsArray[0].autoLogin,null);

                if ((accountsArray[0].autoLogin && this.accountNode.val() != "" && this.passwordNode.val() != "")||
                    (accountsArray[0].autoLogin && this.corpAccount.val() != "" && this.personalAccount.val() != "" && this.corpPassword.val() != "")) {

                    _this.isLoging = true;
                    _this.loginBtn.click();

                    uc.util.StorageUtil.setData("login-data", {
                        firstLogin: false
                    });
                    return;
                }
            }
            if(accountsArray.length > 1) {
                $('.avatar-node').hide();
                $('.multi-avatar').show();
                _this.commonAccountArea.show();
                _this.corpAccountArea.hide();
                _this.corpAccountLoginBtn.show();
            }
            uc.util.StorageUtil.setData("login-data", {
                firstLogin: false
            });
        },

        initMultiAccountUI: function(){
            //$('.avatar-node').hide();
            //$('.multi-avatar').show();
            $('.user', $('.multi-avatar')).removeClass("sel");
            $('.user', $('.multi-avatar')).removeClass("user");
            $('.close-avatar').hide();
	        /*
            for (var j = 0; j < 8; j++) {
                $('#header'+ j)[0].onerror="uc.util.ImageUtil.OnImageLoadError(this,\'' + defaultAvatar + '\')";
                $('#header'+ j).attr("src", "images/login/avatar_default.png");
                $('#header'+ j).addClass("nouser");



            }*/

	        var _headListHtml = '';
	        for (var i = 0; i < this.accountInfos.length; i++) {
		        if (i <= 7) { //最多8个账号
			        _headListHtml+='<li><img title='+uc_resource.Login.DeleteAccount+'  class="close-avatar" src="images/login/close.png" /><img id="header'+i+'"  class="header nouser" src="images/login/avatar_default.png"/></li>'
		        }
	        }
	        if(_headListHtml == ""){
		        _headListHtml+='<li><img id="header'+i+'"  class="header nouser" src="images/login/avatar_default.png"/></li>'
	        }
	        this.headListNode.empty();
	        this.headListNode.append(_headListHtml);

            for (var i = 0; i < this.accountInfos.length; i++) {
	            if (i > 7) { //最多8个账号
                    uc.IClientService.deleteHistoryAccount(this.accountInfos[i].userAccount);
                    continue;
                }
                var userInfo = this.accountInfos[i];
                var header =  $('#header' + i);
                header.removeClass("nouser");
                header.addClass("user");
                header.attr("pos_num", i);
                if (userInfo.localAvatar != "") {
                    header.attr("src",  userInfo.localAvatar);
                }
            }
	        this.updateHeaderPosition();
	        var This = this;
	        //点击删除账号
	        $('.close-avatar', $('#nav1')).on("click",
		        function(){
			        var header = $('.user',  $(this).parent());
			        var pos = Number(header.attr("pos_num"));
			        uc.IClientService.deleteHistoryAccount(This.accountInfos[pos].userAccount);
			        This.accountInfos.splice(pos, 1);
			        This.initMultiAccountUI();
		        }
	        );
        },
	    updateHeaderPosition:function(){
			var accountInfosLen = this.accountInfos.length;
			var multi_avatar = $('.multi-avatar');
			if(accountInfosLen <= 4){
			  multi_avatar.css('top','74px');
			  if(accountInfosLen==0){
				  multi_avatar.css('padding-left',((180-45)+15)+'px');
			  }else{
				  multi_avatar.css('padding-left',((180-accountInfosLen*45)+15)+'px');
			  }
			}else{
			  multi_avatar.css('top','44px');
			  multi_avatar.css('padding-left','15px');
			}
	    },
        applyUserInfo: function(userInfo) {
            if (userInfo.localAvatar != "") {
                $(".avatar")[0].onerror="uc.util.ImageUtil.OnImageLoadError(this,\'' + defaultAvatar + '\')";
                $('.avatar').attr("src",  userInfo.localAvatar);
            } else {
                $(".avatar")[0].onerror="uc.util.ImageUtil.OnImageLoadError(this,\'' + defaultAvatar + '\')";
                $('.avatar').attr("src",  "images/login/logo.png");
            }
            $('.avatar').addClass("user");


            //企业用户登录
            var accountType = userInfo.accounttype;
            if(accountType == 2){
                this.commonAccountArea.hide();
                this.corpAccountArea.show();
                this.corpAccountLoginBtn.hide();

                var userAccount = userInfo.userAccount;
                var firstSplit = userAccount.split("@");
                this.personalAccount.val(firstSplit[0]);

                var secondSplit = firstSplit[1].split(".");
                this.corpAccount.val(secondSplit[0]);
            }else{
                $("input[type=text]").val(userInfo.userAccount);
            }

            if (userInfo.savePassword == 1){
                if (!$("input[name=rememberPwd]:checkbox").is(":checked")) {
                    $("input[name=rememberPwd]:checkbox").click();
                }
                $("input[type=password]").val(userInfo.password);
            } else {
                $("input[name=rememberPwd]:checkbox").attr("checked",false);
                $("input[type=password]").val("");
            }
            if (userInfo.autoLogin == 1) {
                if (!$("input[name=autoLogin]:checkbox").is(":checked")) {
                    $("input[name=autoLogin]:checkbox").click();
                }
            } else {
                $("input[name=autoLogin]:checkbox").attr("checked", false);
            }
        },

        bindEvent: function () {
            _this = this;
            this.accountNode = $('.login-container .input.account');
            this.passwordNode = $('.login-container .input.password');
            this.loginBtn = $('.login-container .login-btn');
            this.loginResult = $('.login-fail-notification .login-warning-content');
            this.loginWarning = $('.login-fail-notification');
            this.showDetail = $('.login-container .show-detail');
            this.rememberPwdChecked = $(".switch.one");
            this.autologinChecked = $(".switch.two");
            this.forgetPwd = $(".forgetPwd");
            this.corpAccountLoginBtn = $(".login-form .corporate-account-login");
            this.commonAccountArea = $('.login-container .common-account');
            this.corpAccountArea = $('.login-container .corporate-area');
            this.corpAccount = $('.login-container .corporate-area .input.corporate-account');
            this.personalAccount = $('.login-container .corporate-area .input.personal-account');
            this.corpPassword = $('.login-container .corporate-area .input.password');
            this.goBack = $('.login-container .corporate-area .back-to-login');


            document.onkeydown = function (e) {
                if (e.keyCode == 27) {
                    uc.util.WindowUtil.close();
                }

                var capsLock = uc.util.SystemUtil.isCapsLockPressed();
                if(capsLock && e.which != 20){  //当前是大写并且按下的键不是大小写切换
                    if ($('.login-container .input.password').is(":focus")) {
                        $(".check-key").show();
                        $(".check-key").html(uc_resource.ResetPassword.CapsLock);
                    }
                }else{
                    $(".check-key").hide();
                }
            };


            $('.avatar-node').on("click", ".user",
                function(e){
                    if( _this.isLoging) {
                        return;
                    }
                    e.preventDefault();
                    e.stopPropagation();
                    if(_this.accountInfos.length > 1){
                        $('.avatar-node').hide();
                        $('.multi-avatar').show();
                    }

                    _this.loginWarning.hide();
                    _this.commonAccountArea.show().find("input").val("");

                    _this.corpAccountArea.hide();
                    _this.corpAccountLoginBtn.show();
                }
            );

            $("input").on("click",function(){
                $('.login-fail-notification').hide();
            });

            this.passwordNode.on("focus", function(e){
                if(uc.util.SystemUtil.isCapsLockPressed()){
                    if( _this.corpAccountArea.is(":visible")){
                        $(".check-key").css("top","280px");
                    }
                    $(".check-key").show();
                    $('.login-fail-notification').hide();
                    $(".check-key").html(uc_resource.ResetPassword.CapsLock);
                }else{
                    $(".check-key").hide();
                }
            });

            this.passwordNode.on("blur", function(){
                $(".check-key").hide();
            });

            this.accountNode.on("input", function(){

                var account = $(this).val();
                _this.setAvatar(account);

            });

            this.personalAccount.on("input",function(){

                var perAccount = $(this).val();
                var corpAccount = _this.corpAccount.val();

                var account = perAccount + "@" + corpAccount +".com";
                _this.setAvatar(account);

            });

            //视高登录
            this.corpAccountLoginBtn.off("click");
            this.corpAccountLoginBtn.on("click",function(){
                if(_this.isLoging){
                   return ;
                }
                $(this).hide();
                _this.commonAccountArea.hide();
                _this.corpAccountArea.show();
                _this.corpAccount.focus();
                _this.loginWarning.hide();
                _this.loginBtn.removeClass("btn-length-change");
                _this.goBack.show();
                $('.multi-avatar').hide();
                $('.avatar-node').show().find("img")
                    .attr("src","images/login/logo.png")
                    .addClass("user");
                $(".check-key").text("");
                $("input").val("");
            });

            //键盘Enter键进行登录操作
            $("input").keydown(function (event){
                    uc.util.LogUtil.info('Login', 'Input KeyDown', "press enter here  to login");
                    if (event.keyCode == 13) {
                        _this.isLoging = true;
                        _this.loginBtn.click();
                    }
                }
            );

            this.showDetail.click(
                function () {
                    uc.util.LogUtil.info('Login', "showDetail click",'click here to show login setting');
                    $(".details").toggle();
                }
            );

            this.rememberPwdChecked.click(
                function(){
                    if( !($("input[name=rememberPwd]:checkbox").is(":checked")) ){
                        $("input[name=autoLogin]:checkbox").attr("checked",false);
                    }
                }
            );

           this.autologinChecked.click(
                function(){
                    if($("input[name=autoLogin]:checkbox").is(":checked")){
                        if (!$("input[name=rememberPwd]:checkbox").is(":checked")) {
                            $("input[name=rememberPwd]:checkbox").click();
                        }
                    }
                }
            );

            this.forgetPwd.off("click");
            this.forgetPwd.on("click", "",function(){

                if(_this.corpAccountArea.is(':visible')){
                    var accountType = 2;
                }else{
                    var accountType = 1;
                }

                var options = {
                    width: 580,
                    height: 420,
                    refreshIfExist: false,
                    centerScreen: true,
                    modal: true,
                    resizable: false,
                    isTop:false,
                    caption: uc.constants.UI.DragCaption.ForgetPassword
                };
                uc.util.WindowUtil.open('page/forgetPassword.html?accountType='+accountType,'forgetPassword',options);

            });

            this.goBack.click(function(){
                if(_this.accountInfos.length > 1){
                    $('.avatar-node').hide();
                    $('.multi-avatar').show();
                }
                _this.loginWarning.hide();
                _this.commonAccountArea.show();
                _this.accountNode.focus();
                $('.login-btn').html(uc_resource.Login.Login);
                _this.corpAccountArea.hide();
                _this.corpAccountLoginBtn.show();
            });

            this.loginBtn.click(function () {
                $(".details").toggle(false);
                if (_this.isLoging) {
                    _this.enableItems(true);
                } else {
                    $('.avatar-node').show();
                    $('.multi-avatar').hide();
                    _this.enableItems(false);
                    _this.login();
                }
            });

            uc.util.CaretUtil.setInputEditorCaretPosition(this.accountNode);
            uc.util.CaretUtil.setInputEditorCaretPosition(this.passwordNode);

            uc.IUIService.bindCrossWindowEvent(uc.constants.UI.CrossWindowEventName.OnPasswordReset,function () {
                    var options = {
                        width: 400,
                        height: 160,
                        refreshIfExist: false,
                        centerScreen: true,
                        modal: true,
                        resizable: false,
                        isTop:false,
                        caption: uc.constants.UI.DragCaption.ConfirmSetPassword
                    };
                    uc.util.WindowUtil.open('page/confirmSetPassword.html','confirmSetPassword',options);
                }
            );
            uc.IUIService.bindCrossWindowEvent(uc.constants.UI.CrossWindowEventName.OnRestartClient, function(){
                var options = {
                    title: 'bee',// 为了解决任务栏右键点击蜜蜂时不弹出主窗口的bug，把窗口名称“蜜蜂”改成“bee”，请勿回退
                    width: 420,
                    height: 600,
                    refreshIfExist: false,
                    centerScreen: true,
                    modal: false,
                    resizable: false,
                    minWidth: 420,
                    minHeight: 600,
                    hasTaskBarIcon: true,
                    behaviorIfSCClose: uc.constants.UI.WindowCloseBehavior.Quit,
                    isChild: false,
                    caption: uc.constants.UI.DragCaption.Login,
                    closeOtherWindow: true
                };
                uc.util.WindowUtil.open('index.html', 'login', options);
            });
            uc.IUIService.bindCrossWindowEvent(uc.constants.UI.CrossWindowEventName.OnProxyAuthenticateSet, function(){
                _this.loginBtn.click();
            });
        },

        setAvatar: function(account){
            for (var i = 0; i < _this.accountInfos.length; i++) {
                if (account == _this.accountInfos[i].userAccount) {
                    if (_this.accountInfos[i].localAvatar != "") {
                        $(".avatar")[0].onerror="uc.util.ImageUtil.OnImageLoadError(this,\'' + defaultAvatar + '\')";
                        $('.avatar').attr("src",  _this.accountInfos[i].localAvatar);
                    } else {
                        $(".avatar")[0].onerror="uc.util.ImageUtil.OnImageLoadError(this,\'' + defaultAvatar + '\')";
                        $('.avatar').attr("src",  "images/login/logo.png");
                    }
                    return
                }

                $(".avatar")[0].onerror="uc.util.ImageUtil.OnImageLoadError(this,\'' + defaultAvatar + '\')";
                $('.avatar').attr("src",  "images/login/logo.png");
            }
        },

        cleanLoginWarnings: function(){
            setTimeout(function(){
                if(_this.loginWarning.is(':visible')){
                    _this.loginWarning.fadeOut();
                }
            },1000);
         },

        enableItems: function (enable) {
            //此处展示动画效果,禁用输入框和按钮
            if (!enable) {
                _this.isLoging = true;
                $(".login-loading1").show();
                $(".login-loading2").show();
                if($('.back-to-login').is(':visible')){
                    $('.back-to-login').hide();
                    $('.login-btn').addClass("btn-length-change");
                }
                $('.login-btn').html(uc_resource.Login.Cancel);
                $('.login-btn').addClass("busy");
                $('.input').attr('disabled', "true");
                this.loginWarning.hide();
            } else { //取消登陆
                _this.isLoging = false;
                $(".login-loading1").hide();
                $(".login-loading2").hide();
                $('.login-btn').removeClass("busy");
                if(_this.corpAccountArea.is(':visible')){
                    $('.back-to-login').show();
                    $('.login-btn').removeClass("btn-length-change");
                }
                $('.login-btn').html(uc_resource.Login.Login);
                $('.input').removeAttr("disabled");
            }
        },
        login: function () {
            if(this.corpAccountArea.is(":visible")){
                var corAccount =  this.corpAccount.val();
                var personalAccount =  this.personalAccount.val();

                var account = personalAccount+"@"+corAccount+".com";
                var password = this.corpPassword.val();
                var accountType = 2;
            }else{
                var account = this.accountNode.val();
                var password = this.passwordNode.val();
                var accountType = 1;
            }

            if (account == "" || password == "") {
                this.loginResult.text(uc_resource.Login.NeitherEmpty);
                this.loginWarning.show();
                _this.cleanLoginWarnings();
                _this.enableItems(true);
                return;
            }
            account = account.replace(/(^\s*)|(\s*$)/g, "");

            uc.IClientService.login(account, password, accountType);
            startTime = new Date();
        },
        ucasLogin: function () {
            var options = {
                title: 'bee',// 为了解决任务栏右键点击蜜蜂时不弹出主窗口的bug，把窗口名称“蜜蜂”改成“bee”，请勿回退
                width: 860,
                height: 580,
                refreshIfExist: false,
                centerScreen: true,
                modal: false,
                resizable: true,
                minWidth: 860,
                minHeight: 580,
                hasTaskBarIcon: true,
                behaviorIfSCClose: uc.constants.UI.WindowCloseBehavior.Hide,
                caption: uc.constants.UI.DragCaption.Message,
                enableDrag: true,
                isChild: false
            };
            uc.util.WindowUtil.open('page/main.html', 'main', options);

            uc.util.WindowUtil.close();
        },
        resetPassword: function () {
            var options = {
                width: 546,
                height: 500,
                refreshIfExist: false,
                centerScreen: true,
                modal: true,
                resizable: false,
                isTop:false,
                caption: [{
                    left: 20,
                    top: 20,
                    width: 400,
                    height: 20
                }]
            };
            uc.util.WindowUtil.open('page/resetPassword.html','resetPassword',options);
        },
        OnLogin: function (errorCode, authType,pbxType, capability) {
            if (!_this.isLoging && errorCode == 0) {
                uc.IClientService.logout();
                return;
            }
            if (capability) {
                uc.util.StorageUtil.setData('capability', capability);
                var capabilityObj = JSON.parse(capability);
                if(capabilityObj.cloud_disk_type){
                    uc.util.StorageUtil.setData('cloudType',capabilityObj.cloud_disk_type);
                }
            }
            if (errorCode == 0) {
                if(this.corpAccountLoginBtn.is(':hidden')){
                    var perAccount = this.personalAccount.val();
                    var corpAccount = this.corpAccount.val();

                    var account = perAccount + "@" + corpAccount +".com";
                }else{
                    var account = this.accountNode.val();
                }

                var rememberPsw = $("input[name=rememberPwd]:checkbox").is(":checked");
                var autoLogin = $("input[name=autoLogin]:checkbox").is(":checked");
                uc.util.LogUtil.info('Login', 'OnLogin',"rememberPsw="+rememberPsw+",autoLogin="+autoLogin);

                uc.IClientService.rememberPassword(account, rememberPsw);
                uc.IClientService.setAutoLogin(account, autoLogin);

                endTime = new Date();
                startTime = new Date();
                uc.util.LogUtil.info('Login', 'OnLogin', 'Start to login UCAS', null);
                var timeout = endTime - startTime;
                uc.util.LogUtil.info('Login', 'OnLogin',"Login time ="+timeout,null);
                //uc.IClientService.loginUCAS();
                //uc.IUpdateService.checkUpdates();
            } else {
                _this.enableItems(true);
                if (errorCode == 10137 || errorCode == 20101) {
                    this.loginResult.text(uc_resource.Login.NeitherEmpty)
                }
                else if (errorCode == 10138) {
                    this.loginResult.text(uc_resource.Login.Wrong);
                }
                else if (errorCode == 10139) {
                    this.loginResult.text(uc_resource.Login.WrongType);
                }
                else if (errorCode == 10152) {
                    //this.loginResult.text('您必须更新您的客户端，否则将无法使用客户端');
                    this.body.hide();//隐藏登录窗口
                    //uc.util.StorageUtil.setData('updateForce', 1);
                    var options = {
                        title: uc_resource.Common.AppName,
                        width: 520,
                        height: 420,
                        refreshIfExist: false,
                        centerScreen: true,
                        modal: true,
                        resizable: false,
                        minWidth: 520,
                        minHeight: 420,
                        hasTaskBarIcon: true,
                        behaviorIfSCClose: uc.constants.UI.WindowCloseBehavior.Close,
                        isChild: false,
                        caption: uc.constants.UI.DragCaption.UpdateClient
                    };
                    uc.util.WindowUtil.open('page/update/update.html?type=1', 'update', options);
                }
                else if (errorCode == 10411) {
                    this.loginResult.text(uc_resource.Login.NotEmpty);
                }
                else if (errorCode == 10128 || errorCode == 10402 || errorCode == 20103) {
                    this.loginResult.text(uc_resource.Login.WrongInput);
                }
                else if (errorCode == 10436 || errorCode == 20104){
                    this.loginResult.text(uc_resource.Login.InvalidAccount);
                }
                else if (errorCode == 10437 || errorCode == 20105){
                    this.loginResult.text(uc_resource.Login.NeedURL);
                }
                else if (errorCode == 10435){
                    this.loginResult.text(uc_resource.Login.URLFailed);
                }
                else if (errorCode == 10141){
                    this.loginResult.text(uc_resource.Login.GetFailed);
                }
                else if (errorCode == 10415){
                    this.loginResult.text(uc_resource.Login.QueryError);
                }
                else if (errorCode == 10156 || errorCode == 10416
                    || errorCode == 10153 || errorCode == 10119 || errorCode == 10129){
                    this.loginResult.text(uc_resource.Login.DBFailed);
                }
                else if (errorCode == 10144){
                    this.loginResult.text(uc_resource.Login.Authority);
                }
                else if (errorCode == 10412){
                    this.loginResult.text(uc_resource.Login.IDFailed);
                }
                else if (errorCode == 10417 ){
                    this.loginResult.text(uc_resource.Login.CheckFailed);
                }
                else if (errorCode == 20102 ){
                    this.loginResult.text(uc_resource.Login.UserIDFailed);
                }
                else if (errorCode == 10140 ){
                    this.loginResult.text(uc_resource.Login.InfoNotExist);
                }
                else if(errorCode == 407){
                    this._openProxyAuthenticateWindow(authType);
                }
                else {
                    this.loginResult.text(uc_resource.Login.LoginFailed);
                }
                this.loginWarning.show();
                _this.cleanLoginWarnings();
            }
        },
       

        OnUCASLogin: function (errorCode) {
            if (!_this.isLoging) {
                uc.IClientService.logout();
                return;
            }
            if (errorCode == 0) {
                endTime = new Date();
                var timeout = endTime - startTime;
                uc.util.LogUtil.info("Login","OnUCASLogin","Login UCAS time ="+timeout,null);

                var allowChangePassword, daysRange, days;
                uc.IClientService.getPasswordRule(function(args) {
                    uc.util.LogUtil.info("Login", "OnLogin", "data", {
                        errCode: args[0],
                        daysRange: args[1],
                        memory: args[2],
                        complex: args[3],
                        allowChangePassword: args[4]
                    });
                    if (!args[0]) {
                        allowChangePassword = args[4];
                        daysRange = args[1];

                        if (allowChangePassword) {
                            uc.IClientService.getPwdExpiredDays(function (args) {
                                uc.util.LogUtil.info("Login", "OnLogin", "data", {errCode: args[0], days: args[1]});
                                if (!args[0]) {
                                    days = args[1];
                                    if (days > 0 && days < 4) {
                                        uc.modules.component.PromptMessageBox.open({
                                            title: uc_resource.WindowTitle.Notification,
                                            message: uc_resource.Login.Password + 30 * daysRange + uc_resource.Login.Due,
                                            buttonMode: "both",
                                            CancelText: uc_resource.Common.Cancel,
                                            OkText: uc_resource.Login.SetPassword
                                        }, function (result) {
                                            if (result === 0) {
                                                this.close();
                                                _this.resetPassword();

                                                uc.IUIService.bindCrossWindowEvent("Setting:ResetPassword", function (args) {
                                                    if (args.status === 0) {
                                                        uc.IClientService.logoutUCAS();
                                                        uc.IClientService.logout();
                                                        _this.enableItems(1);
                                                    }
                                                    else if (args.status === 1) {
                                                        $(".details").toggle(false);
                                                        $('.avatar-node').show();
                                                        $('.multi-avatar').hide();
                                                        _this.enableItems(false);
                                                        _this.passwordNode.val(args.password);
                                                        _this.login();
                                                    }
                                                });
                                            }
                                            else if (result === 1) {
                                                this.close();
                                                uc.modules.component.PromptMessageBox.open({
                                                    title: uc_resource.WindowTitle.Notification,
                                                    message: uc_resource.Login.SetTips,
                                                    buttonMode: "both",
                                                    CancelText: uc_resource.Common.Cancel,
                                                    OkText: uc_resource.Common.OK
                                                }, function (result) {
                                                    if (result === 0) {
                                                        _this.ucasLogin();
                                                        this.close();
                                                    }
                                                    else if (result === 1) {
                                                        this.close();
                                                        _this.resetPassword();

                                                        uc.IUIService.bindCrossWindowEvent("Setting:ResetPassword", function (args) {
                                                            if (args.status === 0) {
                                                                uc.IClientService.logoutUCAS();
                                                                uc.IClientService.logout();
                                                                _this.enableItems(1);
                                                            }
                                                            else if (args.status === 1) {
                                                                $(".details").toggle(false);
                                                                $('.avatar-node').show();
                                                                $('.multi-avatar').hide();
                                                                _this.enableItems(false);
                                                                _this.passwordNode.val(args.password);
                                                                _this.login();

                                                            }
                                                        });
                                                    }
                                                });
                                            }
                                        });
                                    }
                                    else if (days < 1) {
                                        uc.modules.component.PromptMessageBox.open({
                                            title: uc_resource.WindowTitle.Notification,
                                            message: uc_resource.Login.PasswordExpired,
                                            buttonMode: "ok",
                                            OkText: uc_resource.Login.SetPassword
                                        }, function (result) {
                                            this.close();
                                            _this.resetPassword();

                                            uc.IUIService.bindCrossWindowEvent("Setting:ResetPassword", function (args) {
                                                if (args.status === 0) {
                                                    uc.IClientService.logoutUCAS();
                                                    uc.IClientService.logout();
                                                    _this.enableItems(1);
                                                }
                                                else if (args.status === 1) {
                                                    $(".details").toggle(false);
                                                    $('.avatar-node').show();
                                                    $('.multi-avatar').hide();
                                                    _this.enableItems(false);
                                                    _this.passwordNode.val(args.password);
                                                    _this.login();
                                                }
                                            });
                                        });
                                    } else {
                                        _this.ucasLogin();
                                    }
                                }
                            });
                        } else {
                            _this.ucasLogin();
                        }
                    }
                });

            } else {
                _this.enableItems(true);
                this.loginResult.text(uc_resource.Login.UCASFailed);
            }
        },

        OnUCASLogout: function(errorCode){
            //When login UCAS, if network is download, SDK will trigger OnUCASLogout event, not OnUCASLogin event, so here
            //should handle OnUCASLogout event, to show login UCAS failed
            if(errorCode != uc.ErrorCode.Success){
                this.enableItems(true);
                this.loginResult.text(uc_resource.Login.UCASFailed);
            }
        },
        _openProxyAuthenticateWindow: function(authType){
            uc.util.StorageUtil.setData('ProxyAuthenticateAuthType', authType);
            var options = {
                width: 360,
                height: 250,
                refreshIfExist: false,
                centerScreen: true,
                modal: true,
                resizable: false,
                minWidth: 360,
                minHeight: 250,
                hasTaskBarIcon: true,
                caption: uc.constants.UI.DragCaption.ProxyAuthenticate
            };
            uc.util.WindowUtil.open('page/proxyAuthenticate.html', 'proxyAuthenticate', options);
        },
        OnCheckUpdates: function (errorCode, updateFlag, version, notes) {
            if (!_this.isLoging) {
                return;
            }

            if (errorCode == uc.ErrorCode.Success && updateFlag == uc.ErrorCode.NeedUpdate) {
                var options = {
                    title: uc_resource.Common.AppName,
                    width: 520,
                    height: 420,
                    refreshIfExist: false,
                    centerScreen: true,
                    modal: false,
                    resizable: false,
                    minWidth: 520,
                    minHeight: 420,
                    isTop: true,
                    hasTaskBarIcon: true,
                    behaviorIfSCClose: uc.constants.UI.WindowCloseBehavior.Close,
                    enableDrag: false,
                    isChild: false,
                    caption: uc.constants.UI.DragCaption.UpdateClient
                };
                uc.util.WindowUtil.open('page/update/update.html', 'update', options);
            }

            uc.IClientService.loginUCAS();
        }
    };
    jQuery(document).ready(function () {
        uc.init();
        uc.modules.login.Login.init();
    })
})(uc);