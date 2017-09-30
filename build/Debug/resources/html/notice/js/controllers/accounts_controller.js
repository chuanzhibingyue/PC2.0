appModule.controller('AccountsCtrl', ["api",  "$scope","notices","$location",'utils','$translate','log','pages','$timeout',function(api,$scope,notices,$location,utils,$translate,log,pages,$timeout){
  $scope.init = function(){
    $scope.accounts = [];
    $scope.accountNameMaxLength = 15;
    $scope.isShowActivationTips = false;
    $scope.accountSettingKey = "account_for_setting";
    $scope.hasInactivedAccount = false;
    $scope.isLoadingAccounts = true;
    api.notifyCloseError();
    var account = utils.getObjFromlocalStorage($scope.accountSettingKey);
    if(account){
      $scope.showManagement(account);
      utils.removeObjFromlocalStorage($scope.accountSettingKey);
      $scope.isLoadingAccounts = false;
    }
    api.getAccounts(function(data) {
      utils.applyScope($scope,function(){
          $scope.isLoadingAccounts = false;
          var i = 0,ac;
          $scope.accounts = [];
          for (; i < data.length; i++) {
            ac =notices.account();
            ac.convertFromData(data[i]);
            ac.placeholder = ac.status===0?$translate.instant('input-placeholder'):ac.name;
            if(ac.status == 0 && !$scope.hasInactivedAccount){
              $scope.hasInactivedAccount = true;
              ac.isShowActivationTips = true;
              ac.activateTip = $translate.instant("account.guide.activate-all");
            }
            $scope.accounts.push(ac);
          }
          $scope.sortAccounts();
      });
    }, function(msg) {
      log.logInfo("get accounts error:"+msg);
    });

    if($location.path() == "/manage"){
      $scope.module="setting";
      api.getAvatarIcons(function(data){
        utils.applyScope($scope,function(){
          $scope.icons = data;
        });
      },function(msg){
        log.logInfo("get accounts avatar icons error:"+msg);
      });
    }else{
      $scope.module="account";
    }
  };
  $scope.onTopClick = function(e){
    if(!$(e.target).closest(".tips ")[0] && !$(e.target).closest(".avartarDiv")[0]){
      $scope.accounts.forEach(function(ac){
        if(ac.isShowActivationTips){
          ac.isShowActivationTips = false;
        }
      });
    }
  };
  $scope.toggleManagementPanel = function(value){
    if(value != undefined){
      $scope.isShowManagement = value;
      return;
    }

    $scope.isShowManagement = !$scope.isShowManagement;
  };
  $scope.showPublish = function(n, e) {
    e.preventDefault();
    e.stopPropagation();
    if(n.status == 0){
      $scope.manage(e,n);
      $location.path("/manage");
      return;
    }
    var url = 'publish.html';
    if(!!n){
      notices.saveSelectedAccount(n);
    }
    pages.fnCall("publishWindow", "onClose", {saveDraft:true}, function() {
    });
    $timeout(function () {
      pages.openModalDialog(url, 'publish', 887, 730, null,{modal:false,behaviorIfSCClose:3});
    },50);
  };
  $scope.hideActivationTips = function(){
    $scope.isShowActivationTips = false;
  };
  $scope.enter = function(ac){
    if(ac.status != 0){
      notices.saveSelectedAccount(ac);
      $location.path("/notice");
    }
  };
  $scope.manage = function($event,ac){
    $event.stopPropagation();
    utils.saveObjTolocalStorage($scope.accountSettingKey,ac);
    return true;
  };
  $scope.showManagement = function(account){
    $scope.account =  account;
    $scope.temp_avatar = account.avatar;
    $scope.temp_name = account.name;
    $scope.manageSetting =  account.name? account.name : $translate.instant("common.setting");
    $scope.showname = !account.name;
    $scope.toggleManagementPanel(true);
  };


  $scope.sortAccounts = function(){
    if(!$scope.accounts || $scope.accounts.length == 0){
      return;
    }
    for(var i=0; i< $scope.accounts.length;i++){
      for(var j=i+1;j<$scope.accounts.length;j++){
        if($scope.accounts[i].status != 0){
          if($scope.accounts[j].status == 0 || (parseInt($scope.accounts[j].modified_at) < parseInt($scope.accounts[i].modified_at))){
            var t = $scope.accounts[i];
            $scope.accounts[i]= $scope.accounts[j];
            $scope.accounts[j]= t;
          }
        }else{
          if($scope.accounts[j].status == 0 && (parseInt($scope.accounts[j].modified_at) < parseInt($scope.accounts[i].modified_at))){
            var t = $scope.accounts[i];
            $scope.accounts[i]= $scope.accounts[j];
            $scope.accounts[j]= t;
          }
        }
      }
    }
  };

  $scope.updateAvatar = function(newUrl,isForce){
      if(newUrl){
        $scope.temp_avatar = newUrl;
        return;
      }
      if($scope.customizedURL && !isForce){
        $scope.temp_avatar = $scope.customizedURL;
        return;
      }
      $scope.showLoading = true;
      notices.openAvatarWindow(function(url){
        utils.applyScope($scope,function(){
          if(url){
            $scope.temp_avatar = url;
            $scope.customizedURL = url;
            $scope.customizeStyle = {
              "background":"url("+url+") no-repeat top left",
              "background-size": "100%"
            }
          }
          $scope.showLoading = false;
        });
      });
  };

  $scope.removeCustomizedIcon = function(){
    if($scope.customizedURL == $scope.temp_avatar){
      $scope.cancel("avatar");
    }
    $scope.customizedURL = null;
    $scope.customizeStyle = null;
  };

  $scope.showPublishScope = function($event,n){
      $event.stopPropagation();
      if(n.sectionIDs && n.sectionIDs.length > 0){
        $scope.scopes = n.sectionIDs;
        $scope.isShowScopesPanel = true;
      }
  };

  $scope.hidePublishScope = function($event){
    $event.stopPropagation();
    if($($event.target).hasClass("close") || $($event.target).hasClass("publishScopeDiv")){
      $scope.closePublishPanel();
    }
  };

  $scope.closePublishPanel = function(){
    $scope.isShowScopesPanel = false;
    $scope.scopes = null;
  };

  $scope.checkSubmitValue = function(key,value){
    if(key == "name" && value.length > $scope.accountNameMaxLength ){
      return $translate.instant("account.errors.name-too-long", {maxLength:$scope.accountNameMaxLength} );
    }
    return null;
  };

  $scope.submit = function(key){
    var tempKey = "temp_"+key;
    if(!$scope[tempKey])
    {
      return;
    }
    var errorMsg = $scope.checkSubmitValue(key,$scope[tempKey]);
    if(errorMsg){
      api.notifyError(errorMsg);
      return;
    }
    var ac = null, acc=null,index=-1;
    for(var i=0;i<$scope.accounts.length;i++) {
      var ac = $scope.accounts[i];
      if (ac.id == $scope.account.id) {
        acc = $.extend({},ac);
        acc[key] = $scope[tempKey];
        index= i;
        break;
      }
    }
    if(acc){
      api.saveAccount(acc,function(){
        utils.applyScope($scope,function(){
          ac[key] = $scope[tempKey];
          $scope.account = ac;
          $scope["show"+key] = false;
        });
      },function(msg){
        if(index>-1){
          utils.applyScope($scope,function(){
            $scope[tempKey] =$scope.accounts[index][key];
          });
        }
        api.notifyError(msg);
        log.logInfo("failed to save account:"+msg);
        return;
      });
    }
  };

  $scope.cancel = function(key){
    var tempKey = "temp_"+key;

    $scope[tempKey] = $scope.account[key];
    $scope["show" + key] = false;
  };

  $scope.closeManagement = function(){
    $scope.account = null;
    $scope.tempAvatar = null;
    $scope.tempName = null;
    $scope.toggleManagementPanel(false);
  };

  $scope.init();

  $scope.insertNewAccount = function(ac){
    var i = 0;
    for(; i< $scope.accounts.length;i++){
      if($scope.accounts[i].status != 0){
        break;
      }
    }
    if(ac.status == 0){
      for(i=0; i<$scope.accounts.length;i++){
        $scope.accounts[i].isShowActivationTips = false;
      }
      $scope.hasInactivedAccount = true;
      ac.isShowActivationTips = true;
      ac.activateTip = $translate.instant("account.guide.activate-all");
      $scope.accounts.splice(0,0,ac);
    }else{
      $scope.accounts.splice(i,0,ac);
    }

    return;
  };

  var offInit = $scope.$on("Init",function(){
    $scope.init();
  });

  var offAccountCreate = $scope.$on("board_create",function(e,data){
    log.logInfo("receive account create event:" + JSON.stringify(data));
    var jsonData = data;
    var index =  $scope.accounts.findIndex(function (ac) {
      return ac.accountId == jsonData.board_id;
    });
    if(index < 0){
      utils.applyScope($scope,function(){
        var ac =notices.account();
        ac.convertFromData(jsonData);
        $scope.insertNewAccount(ac);
      });
    }
  });

  var offBoardEdit = $scope.$on("board_edit",function(e,data){
    log.logInfo("receive account edit event:" + JSON.stringify(data));
    var index = $scope.accounts.findIndex(function (ac) {
      return ac.id == data.board_id;
    });

    if(index> -1){
      utils.applyScope($scope,function(){
        $scope.accounts[index].name=data.board_name;
        $scope.accounts[index].avatar=data.avatar;
        var currentUser = utils.retrieveUser();
        if($scope.account &&$scope.accounts[index].id == $scope.account.id&&data.editor_user.user_id != currentUser.userId){
          $scope.temp_avatar = data.avatar;
          $scope.temp_name = data.board_name;
          if($scope.accounts[index].status == 0 && data.status==1){
            $scope.accounts[index].isShowActivationTips = false;
          }
          api.notifyError($translate.instant("common.account-edit",{name:data.editor_user.display_name}));
        }
        var storeAccount = notices.retrieveSelectedAccount();
        if(storeAccount && storeAccount.id == data.board_id){
          var ac = notices.account();
          ac.convertFromData(data);
          notices.saveSelectedAccount(ac);
          pages.fnCall("publishWindow", "editAccount", ac, function () {
          });
        }
      });
    }
  });

  var offBoardUpdate = $scope.$on("board_update",function(e,data){
    log.logInfo("receive account update event:" + JSON.stringify(data));
    var index = $scope.accounts.findIndex(function (ac) {
      return ac.id == data.board_id;
    });

    utils.applyScope($scope,function(){
      if(index> -1){
          var currentUser = utils.retrieveUser();
          var uIndex = data.editor_info.findIndex(function(user){
            return user.user_id == currentUser.userId;
          });
          if(uIndex < 0){
            var name = $scope.accounts[index].name;
            if(!name){
              name = $translate.instant("common.no-name-account");
            }
            if($scope.account &&$scope.accounts[index].id == $scope.account.id){　//如果删除的是当前正在设置的账号，
              name= $translate.instant("common.current-account");
              $scope.closeManagement();
            }
            $scope.accounts.splice(index,1);

            var storeAccount = notices.retrieveSelectedAccount();
            if(storeAccount && storeAccount.id == data.board_id){
              pages.fnCall("publishWindow", "onAccountInvalid", $translate.instant("common.no-account-right-alert", {name: storeAccount.name}), function () {
              });
            }
            return;
          }
          var ac =notices.account();
          ac.convertFromData(data);
          $scope.accounts[index] = ac;
          if($scope.account &&$scope.accounts[index].id == $scope.account.id){
            $scope.account = ac;
          }
          var storeAccount = notices.retrieveSelectedAccount();
          if(storeAccount && storeAccount.id == data.board_id){
            notices.saveSelectedAccount(ac);
            pages.fnCall("publishWindow", "updateAccount", ac, function () {
            });
          }
      }else{
        var ac =notices.account();
        ac.convertFromData(data);
        $scope.insertNewAccount(ac);
      }
    });

  });

  var offBoardDelete = $scope.$on("board_destroy",function(e,data){
    log.logInfo("receive account delete event:" + JSON.stringify(data));
    var index = $scope.accounts.findIndex(function (ac) {
      return ac.id == data.board_id;
    });
    var storeAccount = notices.retrieveSelectedAccount();
    if(storeAccount && storeAccount.id == data.board_id){
      pages.fnCall("publishWindow", "onAccountInvalid", $translate.instant("common.account-delete-alert", {name: storeAccount.name}), function () {
      });
    }
    utils.applyScope($scope, function () {
      if(index> -1) {
        var name = $scope.accounts[index].name;
        if(!name){
          name = $translate.instant("common.no-name-account");
        }
        if($scope.account &&$scope.accounts[index].id == $scope.account.id){　//如果删除的是当前正在设置的账号，
          name= $translate.instant("common.current-account");
          $scope.closeManagement();
        }

        $scope.accounts.splice(index, 1);
        // if ($scope.accounts.length > 0) {//只有最后一个的时候不需要提示，会有取消编辑权限的提示
        //   notices.openUCAlert($translate.instant("common.account-delete-alert", {name: name}), function () {
        //   });
        // }
      }
    });
  });
  $scope.$on("$destroy",function(){
    offInit();
    offAccountCreate();
    offBoardEdit();
    offBoardUpdate();
    offBoardDelete();
  });
}]);
