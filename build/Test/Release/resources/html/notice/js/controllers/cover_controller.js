appModule.controller('CoverCtrl', ["api",  "$scope", "$timeout",'utils','pages','log',"$translate","notices", function(api,$scope, $timeout,utils,pages,log,translate,notices) {
    $scope.windowName = "cover";
    $scope.defaultImageWidth = 142;//封面图宽度
    $scope.timeoutPromise = null;
    $scope.init = function(){
        $scope.selectedCover=null;
        $scope.cropper = {};
        api.getNoticeCovers(function(data){
            if(!Array.isArray(data)){
                return;
            }
            utils.applyScope($scope,function(){
                $scope.defaultCoverCategories = $scope.replaceLocalImage(data);
                $scope.selectCategory(0);
            });

        },function(msg){
            log.logInfo("failed to load covers");
        });
    };

    $scope.replaceLocalImage = function(data){
      for(var i=0;i<data.length;i++){
          for(var j=0;j<data[i].lists.length;j++){
              var url = data[i].lists[j].url?data[i].lists[j].url:data[i].lists[j];
              var cropUrl = notices.modifyImageURL(url,$scope.defaultImageWidth);
              var localUrl = notices.getDataFromLocalDB(cropUrl);
              if(localUrl && utils.isFileExist(localUrl)){
                  if(data[i].lists[j].url){
                      data[i].lists[j] = {
                          url:localUrl,
                          remoteUrl:url
                      }
                  }else{
                      var remoteUrl = data[i].lists[j];
                      data[i].lists[j] = {
                          url:localUrl,
                          remoteUrl:url
                      };
                  }
              }else{
                  if(!data[i].lists[j].url){
                      data[i].lists[j] = {
                          url:cropUrl,
                          remoteUrl:url
                      };
                  }

              }
          }
      }
        return data;
    };


    $scope.selectCategory = function(index){
        $scope.selectedCategory = $scope.defaultCoverCategories[index];
        if($scope.timeoutPromise){
            $timeout.cancel($scope.timeoutPromise);
        }
        $scope.timeoutPromise = $timeout(function(){
            for(var i=0; i<$scope.selectedCategory.lists.length;i++){
                downloadCover($scope.selectedCategory.lists[i].url);
            }
        });
    };

    $scope.selectCover = function(index){
        if($scope.selectedCover== $scope.selectedCategory.lists[index]) {
            $scope.selectedCover = null;
            return;
        }
        $scope.selectedCover = $scope.selectedCategory.lists[index];
    };
    $scope.cancelUpload= function(){
        $scope.tempCover = "";
        $scope.toggleCoverOptionDialog();
    };

    $scope.saveCover = function() {
        var suffixIndex = $scope.tempCover.lastIndexOf(".");
        var suffix = "";
        if(suffixIndex > -1){
            suffix = $scope.tempCover.substr(suffixIndex);
        }
        var tempCroppedPath = ucClient.ClientService.getDocumentDataDirSync(3) + '/cropped/temp_notice_'+Date.now()+suffix;
        var path = $scope.cropper.crop(tempCroppedPath);
        log.logInfo("notice cover cropped path:"+path);
        $scope.toggleCoverOptionDialog();
        $scope.showLoading = true;
        $scope.startFileUpload(path,function(errorCode,data){
            $scope.showLoading = false;
            if(errorCode !== 0){
                api.notifyError(translate.instant("publish.alert.upload-image-failed"));
            } else{
                utils.deleteFile(path);
                $scope.defaultCoverCategories[$scope.defaultCoverCategories.length-1].lists.unshift(data);
                $scope.onClose();
            }
        });
    };

    $scope.onClose =function(){
        $scope.closeCover();
    };

    $scope.onSave = function() {
        if(!$scope.selectedCover){
            $scope.onClose();
        }else{
            $scope.selectNewCover($scope.selectedCover);
            $scope.onClose();
        }
    };
    $scope.removeCustomCover = function(index, cover,e){
        if(!cover.id){
            var option = {
                msg:translate.instant("publish.alert.remove-custom-cover"),
                onConfirm:function(){
            $scope.selectedCategory.lists.splice(index,1);
                },
                onCancel:null,
                isShowCancel:true
            };
            api.notifyError(option);
            return;
        }
        e.stopPropagation();
        var option = {
            msg:translate.instant("publish.alert.remove-custom-cover"),
            onConfirm:function(){
            api.removeNoticeCovers(cover.id,function(){
                utils.applyScope($scope,function(){
                    $scope.selectedCategory.lists.splice(index,1);
                });
            },function(msg){
                log.logInfo("delete custom cover failed:"+JSON.stringify(cover));
            });
            },
            onCancel:null,
            isShowCancel:true
        };
        api.notifyError(option);
    };
    $scope.toggleCoverOptionDialog = function() {
        $scope.showCoverOptions = !$scope.showCoverOptions;
    };
    $scope.showCoverOptionDialog = function() {
        $scope.showCoverOptions = true;
    };
    $scope.updateCover = function() {
        utils.openImageDialog(function(data) {
            if (data.errorCode == 0) {
                utils.applyScope($scope, function() {
                    $scope.tempCover = data.paths[0];
                    $scope.showCoverOptionDialog();
                });
            } else {
                api.notifyError(data.msg);
            }
        });
    };
    $scope.init();

    function downloadCover(url){
        if(url.startsWith("http")){
            pages.triggerCrossWindowEvent("OnRequestStoreCover",{"path":url});
        }
    }
}]);
