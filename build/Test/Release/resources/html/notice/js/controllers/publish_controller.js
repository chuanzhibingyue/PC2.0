appModule.controller('PublishCtrl', ["api", "$rootScope", "$scope", "$timeout", 'utils', 'notices', 'pages', 'log', '$translate', '$q', function (api, $rootScope, $scope, $timeout, utils, notices, pages, log, translate, $q) {
    $scope.windowName = "publish";
    $scope.summary = "";
    $scope.topicMaxLength = 30;
    $scope.cancelSaveTimeout = null;
    $scope.autoSaveInterval=60000*5;//每隔五分钟自动保存草稿
    $scope.autoSaveMsg = translate.instant("publish.alert.auto-save-draft-success");
    pages.attachEventScope($scope.windowName + "Window", $scope);
    $scope.setInputEditorCaretPosition = function ($event) {
        utils.setInputEditorCaretPosition($($event.currentTarget));
    };

    $scope.onInputCaretPositionKeyUp = function ($event) {
        utils.onInputCaretPositionKeyUp($($event.currentTarget));
    };
    $scope.isSectionShow = false;
    $scope.coverTaskId = "";
    $scope.currentNotice = notices.notice.create();
    $scope.currentNotice.creatorId = utils.getObjFromlocalStorage("notice_user").userId;
    $scope.selectedAccount = notices.retrieveSelectedAccount();
    $scope.initAccount = function () {
        if (!!$scope.selectedAccount) {
            $scope.currentNotice.accountId = $scope.selectedAccount.id;
            $scope.rootorg = [];
            $scope.rootSections = [];
            $scope.selectedAccount.sectionIDs.forEach(function (val) {
                $scope.rootSections.push({
                    text: val.text, //todo 需要获取当前部门的名称
                    id: val.id
                });
                $scope.rootorg.push(val.id);
            });
        }
        $scope.selectedSections = [];
    };
    $scope.initAccount();

    $scope.sectionEquals = function (oldV, newV) {
        if (oldV.sectionIDs.length != newV.sectionIDs.length) {
            return false;
        }
        var result = true, i = 0;
        for (; i < oldV.sectionIDs.length; i++) {
            var index = newV.sectionIDs.findIndex(function (v) {
                return v.id == oldV.sectionIDs[i].id;
            });
            if (index < 0) {
                result = false;
            }
        }
        return result;
    };

    $scope.updateAccount = function (ac) {
        if (ac.id == $scope.selectedAccount.id) {
            if ($scope.sectionEquals($scope.selectedAccount, ac)) {
                return;
            }

            $scope.selectedAccount = ac;
            pages.fnCall("contactsgroupWindow", "hideContacts", "", function () {
            });
            utils.applyScope($scope, function () {
                $scope.initAccount();
                api.notifyError(translate.instant("publish.alert.account-update"));
            });
        }
    };

    $scope.editAccount = function (ac) {
        if (ac.id == $scope.selectedAccount.id) {
            utils.applyScope($scope, function () {
                $scope.selectedAccount.avatar = ac.avatar;
                $scope.selectedAccount.name = ac.name;
            });
        }
    };

    $scope.onAccountInvalid = function (msg) {
        var option = {
            msg: msg,
            onConfirm: $scope.onClose,
            onCancel: $scope.onClose
        }
        api.notifyError(option);
    };
    $scope.bodyDown = function (e) {
        if (e.keyCode == 8 || e.keyCode == 46) {
            e.stopPropagation();
            $scope.deleteKeyDown(e);
        }
    };


    //自动保存草稿
    $scope.autoSave = function () {
        if($scope.cancelSaveTimeout){
            $timeout.cancel($scope.cancelSaveTimeout);
        }
        $scope.cancelSaveTimeout = $timeout(function () {
            if ($scope.isAvailableToSave()) {
                $scope.onSave($scope.autoSave);
            } else {
                $scope.autoSave();
            }
        }, $scope.autoSaveInterval);
    };

    //编辑草稿
    var currentNotice = notices.getTempNotice();
    if (!!currentNotice) {
        var creatorId = currentNotice.creatorId;
        if (creatorId) {
            $scope.publisher = notices.getBasicUserInfo(creatorId);
        } else {
            $scope.publisher = notices.getBasicUserInfo($scope.currentNotice.creatorId);
        }
        var emptyFn = $scope.currentNotice.isContentEmpty;
        var deferred = $q.defer();
        var promise = deferred.promise;

        if (currentNotice.id && !currentNotice.content) {
            $scope.showLoading = true;
            api.getOneNotice(currentNotice.id, function (data) {
                $scope.showLoading = false;
                utils.applyScope($scope, function () {
                    var notice = notices.notice.create();
                    notices.notice.convertFromData(notice, data);
                    currentNotice = $.extend(currentNotice, notice);
                    if (currentNotice.isEditDisabled) {
                        currentNotice.sections = []; // 转发不显示发布范围
                    }
                    deferred.resolve();
                });
            }, function (msg) {
                $scope.showLoading = false;
                log.logInfo("failed to load notice " + currentNotice.id + "'s detail:" + msg);
                deferred.resolve();
            });
        } else {
            deferred.resolve();
        }
        promise.then(function () {
            utils.applyScope($scope, function () {
                $scope.currentNotice = $.extend({}, currentNotice, {
                    isContentEmpty: emptyFn
                });
                $scope.currentNotice.isEmbedCover = $scope.currentNotice.isEmbedCover == 1;
                $scope.selectedSections = currentNotice.sections;
                $scope.showDefaultSection = !$scope.currentNotice.isEditDisabled;
                if (!$scope.currentNotice.isEditDisabled) {
                    $scope.autoSave();
                }
                if (!!currentNotice.cover) {
                    $scope.coverBackgroundStyle = {
                        "background": "url(" + currentNotice.cover.url + ") no-repeat",
                        "border": "none"
                    };
                }

                $scope.summary = currentNotice.summary;

                $timeout(function () {
                    $scope.$broadcast("ContentReady", $scope.currentNotice.content);
                });
            });
        });
        notices.deleteTempNotice();
    } else {
        $scope.showDefaultSection = true;
        $scope.autoSave();
        $scope.publisher = notices.getBasicUserInfo($scope.currentNotice.creatorId);
    }

    var stopWatch = $scope.$watch("currentNotice.cover", function (newV, oldV) {
        if (!!newV && !!newV.url) {
            if (!oldV || newV.url != oldV.url) {
                $scope.coverBackgroundStyle = {
                    "background": "url(" + newV.url + ") no-repeat",
                    "border": "none"
                };
            }
        }
        else {
            $scope.coverBackgroundStyle = "";
        }
    }, true);

    $scope.toggleCoverOptionDialog = function () {
        $scope.showCoverOptions = !$scope.showCoverOptions;
    };

    $scope.showCoverOptionDialog = function () {
        $scope.showCoverOptions = true;
    };

    //保存广播到草稿箱
    $scope.onSave = function (callback) {
        if ($scope.isAvailableToSave()) {
            if (!!callback) {
                $scope.currentNotice.sections = $scope.selectedSections;
                $scope.currentNotice.summary = $scope.summary;
            }
            if (!!callback || $scope.checkInputs(0)) {
                $scope.currentNotice.createTimestamp = Date.now();
                try{
                    notices.saveDraft($scope.currentNotice, function () {
                        if (!callback) {
                            // $scope.successMsg = translate.instant("publish.alert.save-draft-success");
                            $scope.showPublishSuccess(translate.instant("publish.alert.save-draft-success"));
                        } else { //自动保存
                            $scope.showPublishSuccess(translate.instant("publish.alert.auto-save-draft-success")+utils.getLocaleDateTime(new Date()),{"background-color": "#4cd964"});
                            callback();
                        }
                        pages.fnCall("notice", "saveDraft", "", function () {
                        });
                    });
                }catch(e){
                    log.logInfo(e.stack);
                    $scope.showPublishSuccess(translate.instant("publish.alert.save-draft-failed"),{"background-color":"#f96868"});
                }

            }
        }
    };


    $scope.onClose = function (saveDraft) {
        if(saveDraft){
            $scope.onSave(function(){});
        }
        $scope.currentNotice = null;
        $scope.$destroy();
        $timeout.cancel($scope.cancelSaveTimeout);
        pages.closeWindow();
    };

    $scope.closePage = function () {
        if (!$scope.currentNotice) {
            $scope.onClose();
        }

        if ($scope.isAvailableToSave()) {
            var option = {
                msg: translate.instant("publish.alert.close-window"),
                onConfirm: $scope.onClose,
                onCancel: null,
                isShowCancel: true
            };
            api.notifyError(option);
        } else {
            $scope.onClose();
        }
    };


    $scope.isAvailableToSendPreview = function () {
        return $scope.currentNotice && $scope.currentNotice.topic && !$scope.currentNotice.isContentEmpty() && $scope.currentNotice.cover;
    };

    $scope.isAvailableToSend = function () {
        return $scope.isAvailableToSendPreview() && $scope.selectedSections.length > 0;
    };

    $scope.isAvailableToSave = function () {
        return $scope.currentNotice && ($scope.currentNotice.topic
            || !$scope.currentNotice.isContentEmpty()
            || $scope.currentNotice.cover
            || $scope.summary
            || $scope.selectedSections.length > 0);
    };


    $scope.onSend = function () {
        if (!$scope.isAvailableToSend()) {
            return;
        }
        $scope.currentNotice.type = "";
        if ($scope.checkInputs(1)) {
            var option = {
                msg: translate.instant("publish.alert.send-confirm"),
                onConfirm: function () {
                    $scope.showLoading = true;
                    $scope.$broadcast("onLocalImageStartUpload");
                },
                onCancel: null,
                isShowCancel: true
            };
            api.notifyError(option);
        }
    };

    var offImageUploaded = $scope.$on("onLocalImagesUploaded", function (e, args) {
        if (args.errorCode !== 0) {
            log.logInfo("upload local image failed:" + args.errorMessage);
            $scope.showLoading = false;
            api.notifyError(translate.instant("publish.alert.upload-image-failed"));
            return;
        }
        $scope.currentNotice.content = args.data;
        var data = notices.notice.convertToData($scope.currentNotice);
        if (!data.id) {
            notices.saveNoticeDraft($scope.currentNotice);
        }
        delete data.id;//删除转发的公告id
        api.sendNotice(data, function (result) {
            if (result.type == "preview") {
                utils.applyScope($scope, function () {
                    $scope.isSendingPreview = false;
                    $scope.showLoading = false;
                    $scope.isShowPreviewDialog = false;
                    // $scope.successMsg = translate.instant("publish.alert.send-preview-success", {name: $scope.currentNotice.previewer.name});
                    $scope.showPublishSuccess(translate.instant("publish.alert.send-preview-success", {name: $scope.currentNotice.previewer.name}));
                });
                log.logInfo("success send preview:" + result.id);
                return;
            }
            notices.removeNoticeDraft($scope.currentNotice);
            pages.fnCall("notice", "sendNotice", result, function () {
            });
            $scope.onClose();
        }, function (msg) {
            pages.fnCall("notice", "saveDraft", "", function () {
            });
            $scope.showLoading = false;
            $scope.isSendingPreview = false;
            if (msg) {
                api.notifyError(msg);
            } else {
                api.notifyError(translate.instant("publish.alert.send-failed"));
            }
        });
    });
    $scope.checkTitleAndContent = function (noAlert) {
        if (!$scope.currentNotice.topic) {
            if (!noAlert) {
                api.notifyError(translate.instant("publish.alert.no-topic"));
            }
            return false;
        }
        if ($scope.currentNotice.isContentEmpty()) {
            if (!noAlert) {
                api.notifyError(translate.instant("publish.alert.no-content"));
            }
            return false;
        }
        return true;
    };
    $scope.convertDefaultSections = function (sections) {
        var tempSections = sections;
        if (tempSections[0] && tempSections.length > 0 && tempSections[0].type == 0) {
            tempSections = tempSections[0].sections;
        }
        return tempSections;
    };
    $scope.checkInputs = function (isSend) {
        var isVaild = true;
        $scope.currentNotice.sections = $scope.convertDefaultSections($scope.selectedSections);
        if (!isSend) {
            if (!$scope.currentNotice.topic && $scope.currentNotice.isContentEmpty()) {
                isVaild = $scope.checkTitleAndContent();
            }
        } else {
            isVaild = $scope.checkTitleAndContent();
            if (!isVaild) {
                return isVaild;
            }
            if ($scope.currentNotice.sections.length < 1 && isSend == 1) {
                api.notifyError(translate.instant("publish.alert.no-publish-scope"));
                return false;
            }
            if ((!$scope.currentNotice.cover || !$scope.currentNotice.cover.url)) {
                api.notifyError(translate.instant("publish.alert.no-cover"));
                return false;
            }
            if ($scope.currentNotice.topic.length > $scope.topicMaxLength) {
                api.notifyError(translate.instant("publish.alert.topic-overflow"));
                return false;
            }
            if ($scope.summaryLength > $scope.summaryMaxLength) {
                api.notifyError(translate.instant("publish.alert.summary-overflow"));
                return false;
            }
        }
        if (isVaild) {
            $scope.currentNotice.summary = $scope.summary;
            return true;
        }

        return isVaild;
    };

    $scope.updateCover = function () {
        $scope.isShowCoverPanel = true;
    };

    $scope.closeCover = function () {
        $scope.isShowCoverPanel = false;
    }

    //检查上传文件的尺寸
    $scope.checkImageSize = function (url, maxWidth, maxHeight, success, error) {
        var img = new Image();
        img.src = url;
        img.onload = function () {
            var width = this.width,
                height = this.height;
            if (width > maxWidth || height > maxHeight) {
                if (error) {
                    error();
                }
            } else {
                if (success) {
                    success();
                }
            }
        };
    };

    $scope.$on("$destroy", function () {
        stopWatch();
        stopWatchSummary();
        offImageUploaded();
        stopWatchSendToValue();
        pages.detachEventScope($scope.windowName + "Window");
        pages.removeWindowRestoreListener(onWindowRestored);
        pages.removeWindowMaxListener(onWindowMax);
        $timeout.cancel($scope.cancelSaveTimeout);
    });

    $scope.saveCover = function () {
        var tempCroppedPath = notices.getCropperPath();
        var path = $scope.cropper.crop(tempCroppedPath);
        //         $scope.currentNotice.cover = path;
        $scope.toggleCoverOptionDialog();
        startFileUpload(path);
    };

    $scope.cropper = {};

    $scope.startFileUpload = function (path, callback) {
        var promise = utils.uploadOneFile(path);
        promise.then(function (result) {
            if (result.code === 0) {
                utils.applyScope($scope, function () {
                    $scope.currentNotice.cover = result.data;
                    log.logInfo("upload notice cover:" + result.data.url);
                    callback(0, result.data);
                });
            } else {
                log.logInfo("fail to upload notice cover with error code:" + result.code);
                callback(-1, null);
            }
        });
    };

    $scope.selectNewCover = function (cover, callback) {
        if (cover && cover.url) {
            utils.applyScope($scope, function () {
                $scope.currentNotice.cover = cover;
            });

            if (callback) {
                callback();
            }
        }
    };

    $scope.showPublishSuccess = function (msg, style) {
        utils.applyScope($scope, function () {
            $scope.successMsg = msg;
            if(style){
                $scope.successStyle = style;
            }

            $scope.showSuccessMsg = true;
            $timeout(function () {
                $scope.showSuccessMsg = false;
            }, 2000);
        });

    };


    //-------------------- summary  -----------------------------//
    $scope.summaryMaxLength = 50;
    $scope.summaryLength = 0;
    var stopWatchSummary = $scope.$watch("summary", function (newV, oldV) {
        if (newV && newV.length > 0) {
            $scope.summaryLength = newV.length;
        } else {
            $scope.summaryLength = 0;
        }
    });

    //--------------------- window size control  ----------------//

    $scope.toggleMaximize = function () {
        $scope.isMinimized = false;
        ucClient.Window.setCaptions([{top: 5, left: 0, width: -90, height: 40}]);
        if ($scope.isMaximized) {
            $scope.isMaximized = false;
            pages.restoreWindow();
            return;
        }

        $scope.isMaximized = true;
        pages.maximizeWindow();
    };

    $scope.minimize = function () {
        $scope.isMinimized = true;
        ucClient.Window.setCaptions([{top: 5, left: 0, width: 100, height: 20}]);
    };

    function onWindowRestored() {
        utils.applyScope($scope, function () {
            $scope.isMaximized = false;

        });
    }

    function onWindowMax() {
        utils.applyScope($scope, function () {
            if ($scope.isMinimized) {
                $scope.isMinimized = false;
                ucClient.Window.setCaptions([{top: 5, left: 0, width: -90, height: 40}]);
            }
            $scope.isMaximized = true;
            // pages.restoreWindow();
            //pages.restoreWindow();
        });
    }

    pages.addWindowRestoreListener(onWindowRestored);
    pages.addWindowMaxListener(onWindowMax);
    //--------------------- show preview  ----------------//
    $scope.sendTo = {
        value: 0
    };
    $scope.showPreview = function () {
        notices.saveTempNotice($.extend({}, $scope.currentNotice, {
            accountName: $scope.selectedAccount.name,
            createTime: utils.time2str(Date.now() / 1000)
        }));
        var url = 'preview_mobile.html';
        pages.openModalDialog(url, 'previewMobile', screen.availWidth, screen.availHeight, $scope, {
            modal: true,
            left: 0,
            top: 0,
            centerScreen: false,
            enableDrag: false,
            isChild: true,
            resizable: false,
            removeBorder: true,
            caption: []
        });
    };
    $scope.showPreviewDialog = function () {
        if (!$scope.isAvailableToSendPreview()) {
            return;
        }
        $scope.previewReceiver = null;
        $scope.isShowPreviewDialog = true;
    };
    $scope.searchPreviewReceiver = function (isSelected, user) {
        if (user) {
            user.id = "user" + user.id;
            $scope.previewReceiver = user;
            $scope.sendTo.value = 1;
        }
    };

    $scope.sendPreview = function () {
        if (!$scope.previewReceiver && $scope.sendTo.value == 1) {
            return;
        }
        if ($scope.checkInputs(2)) {
            $scope.isSendingPreview = true;
            $scope.currentNotice.type = "preview";
            if ($scope.sendTo.value == 1) {
                $scope.currentNotice.previewer = $scope.previewReceiver;
            } else {
                $scope.currentNotice.previewer = {
                    id: "user" + $scope.publisher.userId,
                    name: $scope.publisher.displayName
                };
            }
            $scope.sendTo.value = 0;
            $scope.showLoading = true;

            $scope.$broadcast("onLocalImageStartUpload");
        }
    }

    var stopWatchSendToValue = $scope.$watch("sendTo.value", function (newV) {
        if (newV && newV == 0) {
            $scope.previewReceiver = null;
        }
    });
    //---------------------cloud ------------------//
    $scope.cloudFileCallback = function (data) {
        pages.setWindowToTop();
        $scope.$broadcast("onCloudFileSelected", data);
    };
    $scope.showCloudFileWindow = function () {
        notices.addCloudFileSelectedEvent({success: $scope.cloudFileCallback, cancel: pages.setWindowToTop});
        notices.openCloudWindow();
    };
}]);
