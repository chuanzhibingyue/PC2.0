/**
 * Created by yang.sun_1 on 2015/11/18.
 */
appModule.controller('NoticeCtrl', ['$scope', 'api', '$timeout', 'utils', 'notices', 'pages', '$location', '$translate', 'log', function ($scope, api, $timeout, utils, notices, pages, $location, translate, log) {
    //返回到应用
    $scope.backToMain = function () {
        $location.url("/");
    };

    $scope.setInputEditorCaretPosition = function ($event) {
        utils.setInputEditorCaretPosition($($event.currentTarget));
    };

    $scope.onInputCaretPositionKeyUp = function ($event) {
        utils.onInputCaretPositionKeyUp($($event.currentTarget));
    };

    $scope.openImageDialog = function (param, cb) {
        utils.openImageDialog(function (data) {
            if (!!cb) {
                cb(data);
            }
        });
    };

    $scope.fileUploadedListeners = [];

    $scope.onFileUploaded = function (args) {
        for (var i = $scope.fileUploadedListeners.length - 1; i >= 0; i--) {
            if ($scope.fileUploadedListeners[i].taskId == args[0]) {
                $scope.fileUploadedListeners[i].callback({
                    errorCode: 0,
                    data: args[1]
                });
                $scope.fileUploadedListeners.splice(i, 1);
                return;
            }
        }
    };

    $scope.saveDraft = function () {
        $scope.onDraftUpdate();
        if (!$scope.historyNoticesSelected) {
            utils.applyScope($scope, function () {
                $scope.selectedNotices = $scope.draftNotices;
            });
        }
    };

    $scope.deleteNotice = function (n, e) {
        if (e) {
            e.stopPropagation();
        }
        var option = {
            msg: translate.instant("notice.delete-confirm"),
            onConfirm: function () {
                if($scope.selectedNotice && ((n.id && n.id == $scope.selectedNotice.id) || n.localId == $scope.selectedNotice.localId)){
                    $scope.closeDetail();
                }
                api.deleteNotice(n, function () {
                    utils.applyScope($scope, function () {
                        var list = !!n.id ? $scope.historyNotices : $scope.draftNotices;
                        for (var i = list.length - 1; i > -1; i--) {
                            if (list[i] == n) {
                                list.splice(i, 1);
                                // if (!!n.id) {
                                //     $scope.cacheNotices[$scope.selectedAccount.id] = list;
                                // }
                                return;
                            }
                        }
                    });
                }, function (msg) {
                    if (msg) {
                        api.notifyError(msg);
                    }
                });
            },
            onCancel: null,
            isShowCancel: true
        };

        api.notifyError(option);

    };

    $scope.sendNotice = function (param) {

        utils.applyScope($scope, function () {
            $scope.onDraftUpdate();
        });

        var newNotice = notices.notice.create();
        //新创建的广播如果没有创建时间，将当前时间赋值为创建时间
        if (!param.created_at) {
            param.created_at = param.modified_at || Date.now();
        }
        notices.notice.convertFromData(newNotice, param);
        newNotice.creatorId = $scope.currentUser.userId;
        if ($scope.historyNoticesSelected && $scope.selectedAccount.id == newNotice.accountId) {//当前选择的是已发送列表并且当前选择的账号是此广播的账号
            utils.applyScope($scope, function () {
                $scope.historyNotices.unshift(newNotice);
                // $scope.cacheNotices[$scope.selectedAccount.id] = $scope.historyNotices;
                $scope.$broadcast("scrollToTop", $scope.historyNoticesId);
                // $scope.showDetail($scope.historyNotices[0]);
            });
        } else {//当前选择的是草稿箱
            // $scope.cacheNotices[newNotice.accountId].unshift(newNotice);
            utils.applyScope($scope, function () {
                $scope.selectedNotices = $scope.draftNotices;
                //设置新的选择项
                // if($scope.draftNotices.length > 0){
                //     $scope.showDetail($scope.draftNotices[0]);
                // }else{
                //     $scope.showDetail(null);
                // }
            });
        }

        // $scope.showPubishSuccess();
    };

    $scope.userManage = function (e, account) {
        e.preventDefault();
        e.stopPropagation();
        account.isEdit = true;
        var el = e.currentTarget.parentElement.getElementsByTagName('input')[0];
        setTimeout(function () {
            el.focus();
        }, 0);

    };

    $scope.showPublishSuccess = function () {

        $scope.showSuccessMsg = true;
        $timeout(function () {
            $scope.showSuccessMsg = false;
        }, 2000);
    };

    $scope.showPublish = function (n, e) {
        e.preventDefault();
        e.stopPropagation();
        if (!$scope.selectedAccount || n.id) {
            return;
        }

        notices.saveSelectedAccount($scope.selectedAccount);
        var url = 'publish.html';
        if (!!n) {
            notices.saveTempNotice(n);
        }
        pages.fnCall("publishWindow", "onClose", {saveDraft:true}, function () {
        });
        $timeout(function () {
            pages.openModalDialog(url, 'publish', 887, 730, $scope, {modal: false, behaviorIfSCClose: 3});
            $scope.closeDetail();
        }, 50);
    };

    $scope.forward = function (n, e) {
        $scope.showPublish($.extend({isEditDisabled: true}, n, {sections: []}), e);
    };

    $scope.isUnableToSend = function (n) {
        var result = !n || n.id || !n.topic || !n.content || !n.cover || n.sections.length == 0;
        if (!result) {
            $scope.$broadcast("ContentReady", n.content);
        }
        return result;
    };

    $scope.publishNotice = function (n, e) {
        e.stopPropagation();
        if($scope.historyNoticesSelected){
            return;
        }
        var option = {
            msg: translate.instant("publish.alert.send-confirm"),
            onConfirm: function () {
                $scope.showLoadingNotice = true;
                $scope.$broadcast("onLocalImageStartUpload");
            },
            onCancel: null,
            isShowCancel: true
        };
        api.notifyError(option);
    };

    var offImageUploaded = $scope.$on("onLocalImagesUploaded", function (e, args) {
        if (args.errorCode !== 0) {
            log.logInfo("upload local image failed:" + args.errorMessage);
            $scope.showLoadingNotice = false;
            api.notifyError(translate.instant("publish.alert.upload-image-failed"));
            return;
        }
        $scope.selectedNotice.content = args.data;
        var data = notices.notice.convertToData($scope.selectedNotice);
        if (!data.id) {
            notices.saveNoticeDraft($scope.selectedNotice);
        }
        delete data.id;//删除转发的公告id
        api.sendNotice(data, function (result) {
            notices.removeNoticeDraft($scope.selectedNotice);
            $scope.closeDetail();
            $scope.sendNotice(result);
        }, function (msg) {
            pages.fnCall("notice", "saveDraft", "", function () {
            });
            $scope.showLoadingNotice = false;
            if (msg) {
                api.notifyError(msg);
            } else {
                api.notifyError(translate.instant("publish.alert.send-failed"));
            }
        });
    });

    //-------------------------广播列表----------------------------------//
    $scope.ITEM_PER_PAGE = 20;
    $scope.historyNoticesSelected = true;
    $scope.historyNotices = [];
    $scope.historyNoticesId = "histories";
    $scope.draftNoticesId = "drafts";
    // $scope.cacheNotices = {};
    $scope.isLoadingNotice = false;

    $scope.onDraftUpdate = function () {
        if (!!$scope.selectedAccount) {
            $scope.draftNotices = notices.getDrafts($scope.currentUser.userId, $scope.selectedAccount.id);
        }
    };

    $scope.getNotices = function (isNew) {
        if (!$scope.isLoadingNotice) {
            utils.applyScope($scope, function () {
                $scope.isLoadingNotice = true;
            });
            var lastNoticeId = $scope.historyNotices.length > 0 ? (!!isNew ? $scope.historyNotices[0].id : $scope.historyNotices[$scope.historyNotices.length - 1].id) : 0;
            var accountId = $scope.selectedAccount.id;
            var criteria = {
                lastNoticeId: lastNoticeId,
                isNew: isNew
            };
            if ($scope.selectedAccount.searchText) {
                criteria.keyword = $scope.selectedAccount.searchText;
                if (!isNew) {
                    criteria.start = $scope.historyNotices.length + 1;
                }
            } else {
                if ($scope.selectedAccount.timeRange && $scope.selectedAccount.timeRange.value) {
                    var curTimestamp = Date.now();
                    criteria.startTime = curTimestamp - $scope.selectedAccount.timeRange.value;
                }
                if ($scope.selectedAccount.selectedEditor && $scope.selectedAccount.selectedEditor.id != -1) {
                    criteria.editorId = $scope.selectedAccount.selectedEditor.id;
                }
            }


            api.getNotices(accountId, criteria, function (result) {
                //                setTimeout(function(){
                utils.applyScope($scope, function () {
                    if (!!isNew) {
                        $scope.historyNotices = result;
                    } else {
                        $scope.historyNotices = $scope.historyNotices.concat(result);
                    }
                    $scope.selectedNotices = $scope.historyNotices;
                    // $scope.cacheNotices[accountId] = $scope.historyNotices;
                    $scope.isLoadingNotice = false;
                });
                //                },5000);
            }, function (msg) {
                //                api.notifyError(msg); 没必要弹出对话框告诉用户没有数据了
                utils.applyScope($scope, function () {
                    $scope.isLoadingNotice = false;
                });
            });
        }
    };

    // $scope.readCache = function () {
    //     var accountId = $scope.selectedAccount.id;
    //     $scope.historyNotices = $scope.cacheNotices[accountId] ? $scope.cacheNotices[accountId] : [];
    // };

    $scope.toggleNoticeType = function (type) {
        $scope.toggleCategories();
        if (type !== undefined) {
            $scope.historyNoticesSelected = type;
        } else {
            $scope.historyNoticesSelected = !$scope.historyNoticesSelected;
        }

        //设置当前列表第一个广播为选择状态。
        if ($scope.historyNoticesSelected) {
            $scope.selectedCategory = translate.instant("notice.sent-message");
            $scope.selectedNotices = $scope.historyNotices;
        } else {
            $scope.selectedNotices = $scope.draftNotices;
            $scope.selectedCategory = translate.instant("notice.drafts");
        }
    };

    $scope.showDetail = function (n, e) {
        if ($scope.selectedNotice == n) {
            $scope.closeDetail();
            return;
        }
        $scope.selectedNotice = n;
        $scope.isShowDetail = true;
        if (n.id) {
            api.getOneNotice(n.id, function (data) {
                utils.applyScope($scope, function () {
                    var notice = notices.notice.create();
                    notices.notice.convertFromData(notice, data);
                    if ($scope.selectedNotice.id == notice.id) {
                        $scope.selectedNotice.unreadCount = notice.unreadCount;
                        $scope.selectedNotice.readCount = notice.readCount;
                    }
                    $scope.selectedNotices.forEach(function (n) {
                        if (n.id == notice.id) {
                            n.unreadCount = notice.unreadCount;
                            n.readCount = notice.readCount;
                        }
                    });
                });
            }, function (msg) {
                log.logInfo("failed to load notice " + $scope.notice.id + "'s detail:" + msg);
            })
        }
    };

    $scope.closeDetail = function () {
        $scope.isShowDetail = false;
        $scope.selectedNotice = null;
    }

    //下拉刷新
    $scope.onScrollDown = function () {
        if ($scope.historyNoticesSelected) {
            $scope.getNotices();
        }
    };

    $scope.showPublishScope = function (n) {
        if (n.sections && n.sections.length > 0) {
            $scope.scopes = n.sections;
        } else {
            $scope.scopes = [];
        }
        $scope.isShowScopesPanel = true;
    };

    $scope.hidePublishScope = function () {
        $scope.isShowScopesPanel = false;
        $scope.scopes = null;
    };

    $scope.needShowPublishScope = function (n) {
        if (n.sections.length != $scope.selectedAccount.sectionIDs.length) {
            return true;
        }

        //查找公告的发布范围与账号是否一致
        for (var i = 0; i < n.sections.length; i++) {
            var isFind = false;
            for (var j = 0; j < $scope.selectedAccount.sectionIDs.length; j++) {
                if ($scope.selectedAccount.sectionIDs[j].id === n.sections[i].id) {
                    isFind = true;
                    break;
                }
            }
            if (!isFind) {
                return true;
            }
        }
        return false;
    };


    //-------------------------------------------------------------------//

    //--------------------------------- 账号管理-------------------------------//

    $scope.accounts = [];
    $scope.isShowOptions = false;

    $scope.toggleAccountsOptions = function () {
        $scope.isShowOptions = !$scope.isShowOptions;
    };

    $scope.updateSelectedAccount = function (e, account) {
        if (e) {
            e.preventDefault();
            e.stopPropagation();
        }

        $scope.clearCriteria($scope.selectedAccount);

        $scope.selectedAccount = account;
        $scope.clearCache(); //todo 移除cache机制，因为切换帐号的时候不保留筛选结果
        $scope.selectedAccount.allEditors = [$scope.defaultEditor];
        $scope.selectedAccount.selectedEditor = account.allEditors[0];
        $scope.getAccountsEditors($scope.selectedAccount);
        $scope.toggleAccountsOptions();
    };


    $scope.toggleAccountsSelection = function (e) {
        $scope.showAccountsSelection = !$scope.showAccountsSelection;
    };

    var stopSelectedAccountWatch = $scope.$watch("selectedAccount", function (newV, oldV) {
        if (!!newV) {
            //$scope.showDetail(null);
            // $scope.readCache();
            $scope.historyNoticesSelected = true;
            // if (!$scope.cacheNotices[newV.id] || $scope.cacheNotices[newV.id].length < $scope.ITEM_PER_PAGE) {
            $scope.getNotices(true);
            // } else {
            //     $scope.selectedNotices = $scope.historyNotices;
            // }

            $scope.onDraftUpdate();
        } else {
            $scope.selectedNotices = null;
        }
    });

    var stopWatchSelectedNotices = $scope.$watch("selectedNotices", function (newV, oldV) {
        if ($scope.accountsLoaded) {
            if (!$scope.selectedAccount) {
                $scope.showEmpty = true;
                return;
            }

            if (newV && newV.length == 0 && $scope.historyNoticesSelected) {
                $scope.showEmpty = true;
                if(($scope.selectedAccount.timeRange && $scope.selectedAccount.timeRange.value) ||  ($scope.selectedAccount.selectedEditor && $scope.selectedAccount.selectedEditor.id != -1)){
                    $scope.isNoFilterResult = true;
                }else{
                    $scope.isNoFilterResult = false;
                }
                if($scope.selectedAccount.searchText){
                    $scope.isNoSearchResult = true;
                }else{
                    $scope.isNoSearchResult = false;
                }
            } else {
                $scope.showEmpty = false;
            }
        }
    });

    $scope.toggleCategories = function () {
        // if(event){
        //   event.stopPropagation();
        // }
        $scope.isShowCategories = !$scope.isShowCategories;
    };

    $scope.onInit = function () {
        $scope.successMsg = translate.instant("notice.publish-success");
        $scope.module = "notice";
        //获取用户信息并存入localstorage
        $scope.currentUser = api.getCurrentUser();
        $scope.selectedAccount = null;
        $scope.historyNotices = [];
        $scope.accounts = [];
        // $scope.cacheNotices = {};
        pages.attachEventScope($scope.module, $scope);
        //获取账号
        api.getAccounts(function (data) {
            utils.applyScope($scope, function () {
                var i = 0, ac;
                for (; i < data.length; i++) {
                    ac = notices.account();
                    ac.convertFromData(data[i]);
                    if (ac.status == 0) {
                        continue;
                    }
                    $scope.accounts.push(ac);
                }
                $scope.accountsLoaded = true;
                $scope.selectedNotices = null;
                if ($scope.accounts.length > 0) {
                    $scope.updateSelectedAccount(null, $scope.accounts[0]);
                    //获取最新草稿箱
                    $scope.onDraftUpdate();
                    $scope.historyNoticesSelected = true;
                    $scope.selectedCategory = translate.instant("notice.sent-message");
                    // $scope.cacheNotices[$scope.selectedAccount.id] = [];
                    notices.retrieveLocalData($scope.currentUser.userId);
                    $scope.updateAccountsArray();
                }
            });
        }, function (msg) {
            log.logInfo("get accounts error:" + msg);
        });


    };
    $scope.updateAccountsArray = function () {
        $scope.accountsArray = [];
        var j = 0, k = 0;
        for (; j < $scope.accounts.length; j++) {
            if (j > 0 && j % 6 == 0) {
                k++;
            }
            if (!$scope.accountsArray[k]) {
                $scope.accountsArray[k] = [];
            }

            $scope.accountsArray[k].push($scope.accounts[j]);
        }
        var gridWidth = 200 * (k + 1);
        $scope.accountGridStyle = {
            'width': gridWidth < 600 ? gridWidth : 600 //600: 最多显示三行 多了出现滚动条
        }
    };
    $scope.onInit();
    $scope.$on("Init", function () {
        $scope.onInit();
    });
    $scope.onTopClick = function (e) {
        if (!$(e.target).closest(".noticeItem ")[0] && !$(e.target).closest(".detailPanel")[0]) {
            $scope.closeDetail();
        }
        if (!$(e.target).closest(".accountsGrid")[0] && !$(e.target).closest(".accountsSelect")[0]) {
            $scope.showAccountsSelection = false;
        }
        if (!$(e.target).closest(".timeRanges")[0] && !$(e.target).closest(".selectedTimeRange")[0]) {
            $scope.showAccountTimeRange = false;
        }
        if (!$(e.target).closest(".accountEditors")[0] && !$(e.target).closest(".selectedEditor")[0]) {
            $scope.showSelectedAccountEditors = false;
        }
    };
    $scope.openModalDialog = function (params) {
        pages.openModalDialog(params.url, params.eventName, params.width, params.height, $scope);
    };

    $scope.convertTime = function (source) {//将日期转化成视觉要求
        return notices.convertNoticeTime(source);
    };
    //--------------------- show preview  ----------------//
    $scope.showPreview = function (n, e) {
        if (e) {
            e.stopPropagation();
        }
        if (!n.createTime) {
            n = $.extend({}, n, {createTime: utils.time2str(Date.now() / 1000)});
        }
        notices.saveTempNotice(n);
        var url = 'preview.html';
        pages.openModalDialog(url, 'preview', 730, 720, $scope, {
            modal: false,
            caption: [{
                left: 0,
                top: 5,
                width: 90,
                height: 20
            }, {
                left: 150,
                top: 5,
                width: -60,
                height: 40
            }],
            resizable: false
        });
    };

    //-------------------------------------------------------------------------//

    //------------------------消息列表搜索 过滤  排序---------------------------//
    $scope.toggleAccountTimeRange = function (e) {
        // if (e) {
        //     e.stopPropagation();
        // }
        $scope.showAccountTimeRange = !$scope.showAccountTimeRange;
    };

    $scope.timeRanges = [{
        key: translate.instant("notice.time-range.week"),
        value: 1000 * 60 * 60 * 24 * 7 //最近一周
    }, {
        key: translate.instant("notice.time-range.month"),
        value: 1000 * 60 * 60 * 24 * 30 //最近30天
    }, {
        key: translate.instant("notice.time-range.all"),
        value: null //最近一周
    }];

    $scope.getAccountTimeRange = function (account) {
        return account ? (account.timeRange ? account.timeRange.key : $scope.timeRanges[$scope.timeRanges.length - 1].key) : $scope.timeRanges[$scope.timeRanges.length - 1].key;
    };

    $scope.onTimeRangeChanged = function (range, e) {
        if (e) {
            e.stopPropagation();
        }
        $scope.toggleAccountTimeRange();
        if ($scope.selectedAccount.timeRange != range) {
            $scope.selectedAccount.timeRange = range;
            $scope.clearCache();
            $scope.clearSearchText($scope.selectedAccount);
            $scope.getNotices(true);
        }
    };

    $scope.clearCache = function () {
        // var accountId = $scope.selectedAccount;
        // $scope.cacheNotices[accountId] = [];
        $scope.historyNotices = [];
    };

    $scope.defaultEditor = {
        id: -1,
        name: translate.instant("common.all")
    };

    //获取账号的所有编辑（包括已取消权限的）
    $scope.getAccountsEditors = function (account) {

        api.getAccountAllEditors(account.id, function (result) {
            utils.applyScope($scope, function () {
                result.forEach(function (data) {
                    account.allEditors.push({
                        id: data.user_id,
                        name: data.display_name
                    });
                });
            });
        }, function (msg) {
            log.logInfo("get account " + account.id + " all editor error " + msg);
        });

    };

    $scope.toggleSelectedAccountEditor = function (e) {
        // if (e) {
        //     e.stopPropagation();
        // }
        $scope.showSelectedAccountEditors = !$scope.showSelectedAccountEditors;
    };

    $scope.onSelectedEditorChanged = function (editor, e) {
        if (e) {
            e.stopPropagation();
        }
        $scope.toggleSelectedAccountEditor();
        if ($scope.selectedAccount.selectedEditor != editor) {
            $scope.selectedAccount.selectedEditor = editor;
            $scope.clearCache();
            $scope.clearSearchText($scope.selectedAccount);
            $scope.getNotices(true);
        }
    };

    $scope.clearCriteria = function (account) {
        if (account) {
            if (account.timeRange) {
                delete account.timeRange;
            }
            account.selectedEditor = $scope.defaultEditor;
            if (account.searchText) {
                account.searchText = "";
            }
        }
    };

    $scope.clearSearchText = function (account) {
        if (account.searchText) {
            account.searchText = "";
        }
    };

    $scope.onSearchNotice = function () {
        var searchText = $scope.selectedAccount.searchText;
        $scope.clearCriteria($scope.selectedAccount);
        $scope.selectedAccount.searchText = searchText;
        $scope.clearCache();
        $scope.historyNoticesSelected = true;
        $scope.getNotices(true);
    };

    $scope.onSearchTextKeyUp = function (e) {
        if (e.keyCode == 13) {
            $scope.onSearchNotice();
        }
    };

    $scope.updateSearchFocusStatus = function(value){
        $scope.searchFocused = value;
    };

    $scope.removeSearchText = function(){
        $scope.selectedAccount.searchText = "";
        $scope.onSearchNotice();
    };

    //-------------------------------公告板修改 删除 更新事件 -------------------//
    var offBoardEdit = $scope.$on("board_edit", function (e, data) {
        log.logInfo("notice controller receive account edit event:" + JSON.stringify(data));
        var index = $scope.accounts.findIndex(function (ac) {
            return ac.id == data.board_id;
        });
        var ac = notices.account();
        ac.convertFromData(data);
        if (index > -1) {
            $scope.accounts[index].avatar = ac.avatar;
            $scope.accounts[index].name = ac.name;
            if ($scope.selectedAccount && $scope.selectedAccount.id == data.board_id) {
                $scope.selectedAccount = $scope.accounts[index];
            }
            var storeAccount = notices.retrieveSelectedAccount();
            if (storeAccount && storeAccount.id == data.board_id) {
                notices.saveSelectedAccount($scope.accounts[index]);
                pages.fnCall("publishWindow", "editAccount", $scope.accounts[index], function () {
                });
            }
        } else {
            $scope.accounts.unshift(ac);
            $scope.updateAccountsArray();
        }
    });

    var offBoardUpdate = $scope.$on("board_update", function (e, data) {
        log.logInfo("notice controller receive account update event:" + JSON.stringify(data));
        utils.applyScope($scope, function () {
            var currentUser = utils.retrieveUser();
            var uIndex = data.editor_info.findIndex(function (user) {
                return user.user_id == currentUser.userId;
            });
            if (uIndex < 0) {
                $scope.removeAccounts(data, "common.no-account-right-alert");
            } else {
                var ac = notices.account();
                ac.convertFromData(data);
                var index = $scope.accounts.findIndex(function (ac) {
                    return ac.id == data.board_id;
                });
                if (index > -1) {
                    $scope.accounts[index].sectionIDs = ac.sectionIDs;
                    $scope.accounts[index].editors = ac.editors;
                    if ($scope.selectedAccount && $scope.selectedAccount.id == data.board_id) {
                        $scope.selectedAccount = $scope.accounts[index];
                    }
                } else {
                    if (ac.status != 0) {
                        $scope.accounts.unshift(ac);
                        $scope.updateAccountsArray();
                    }
                }
                var storeAccount = notices.retrieveSelectedAccount();
                if (storeAccount && storeAccount.id == data.board_id) {
                    notices.saveSelectedAccount(ac);
                    pages.fnCall("publishWindow", "updateAccount", ac, function () {
                    });
                }
            }
        });
    });

    var offBoardDelete = $scope.$on("board_destroy", function (e, data) {
        log.logInfo("notice controller receive account delete event:" + JSON.stringify(data));
        $scope.removeAccounts(data, "common.account-delete-alert");
    });

    $scope.removeAccounts = function (data, message) {
        var index = $scope.accounts.findIndex(function (ac) {
            return ac.id == data.board_id;
        });
        if (index < 0) {
            return;
        }

        api.getAccounts(function (d) {
            if (d.length > 0) {
                utils.applyScope($scope, function () {
                    if ($scope.selectedAccount && $scope.selectedAccount.id == data.board_id) {
                        // var name = $scope.selectedAccount.name;
                        var newIndex;
                        if ($scope.accounts.length == 1) {
                            $scope.updateSelectedAccount(null, null);
                        } else {
                            if (index > -1) {
                                if (index == $scope.accounts.length - 1) {
                                    newIndex = 0;
                                } else {
                                    newIndex = index + 1;
                                }
                            }
                            $scope.updateSelectedAccount(null, $scope.accounts[newIndex]);
                        }

                        $scope.closeDetail();
                        // notices.openUCAlert(translate.instant(message, {name: name}), function () {
                        // });
                    }
                    var storeAccount = notices.retrieveSelectedAccount();
                    if (storeAccount.id == data.board_id) {
                        pages.fnCall("publishWindow", "onAccountInvalid", translate.instant(message, {name: storeAccount.name}), function () {
                        });
                    }

                    $scope.accounts.splice(index, 1);
                    $scope.updateAccountsArray();

                });
            }
        });

    };

    $scope.$on("$destroy", function () {
        stopWatchSelectedNotices();
        stopSelectedAccountWatch();
        offBoardEdit();
        offBoardUpdate();
        offBoardDelete();
        offImageUploaded();
        pages.detachEventScope($scope.module);
    });
}]);
