appModule.controller('ReceiverCtrl', ['$scope', 'api', '$timeout', 'utils', 'notices', 'pages', '$translate', 'log', function ($scope, api, $timeout, utils, notices, pages, translate, log) {
    $scope.noticeContainerId = "notices_container";
    $scope.showNotices = false;
    $scope.isLoadingAccounts = true;
    $scope.ConversationTitle = notices.getConversationTitle();
    api.getReceiverAccounts(0, 0, function (data) {
        utils.applyScope($scope, function () {
            $scope.accounts = data;
            $scope.isLoadingAccounts = false;
        });
    }, function (msg) {
        log.logInfo("get receiver accounts failed. user_id:" + api.getCurrentUser().userId + "; msg:" + msg);
        api.notifyError(msg);
        utils.applyScope($scope, function () {
            $scope.isLoadingAccounts = false;
        });
    });

    $scope.backToMain = function () {
        $scope.showNotices = false;
    };

    $scope.getNotices = function (callback) {
        if ($scope.isLoadingNotices) {
            return;
        }
        utils.applyScope($scope,function(){
            $scope.isLoadingNotices = true;
            var accountId = $scope.selectedAccount.board_id;
            var notices = $scope.selectedAccount.notices;
            var max = 0, since = 0;
            if (notices && notices.length > 0) {
                max = notices[0].id;
            }
            api.getReceiverNotices(accountId, since, max, function (data) {
                utils.applyScope($scope, function () {
                    if (data.length > 0) {
                        if (notices) {
                            $scope.selectedAccount.notices = data.concat($scope.selectedAccount.notices);
                        } else {
                            $scope.selectedAccount.notices = data;
                        }
                        $timeout(function () {
                            $scope.$broadcast("onNoticesInit", data[data.length - 1].id);
                        });
                    }
                    $scope.isLoadingNotices = false;
                });
                if (callback) {
                    callback(data);
                }
            }, function (msg) {
                $scope.isLoadingNotices = false;
                log.logInfo("get receiver notices failed. user_id:" + api.getCurrentUser().userId + "; account_id=" + accountId + "; msg:" + msg);
                // api.notifyError(msg);
                utils.applyScope($scope, function () {
                    $scope.isLoadingNotices = false;
                });
                if (callback) {
                    callback(null);
                }
            });
        });
    };

    $scope.updateSelectedAccount = function (account) {
        $scope.selectedAccount = account;
        $scope.showNotices = true;
        if (!$scope.selectedAccount.notices || $scope.selectedAccount.notices.length === 0) {
            $scope.getNotices();
        } else {
            $timeout(function () {
                $scope.$broadcast("scrollToBottom", $scope.noticeContainerId);
            });
        }
    };

    $scope.time2str = function (t) {
        return utils.time2str(t / 1000);
    };

    $scope.onScrollDown = function () {
        utils.applyScope($scope, function () {
            if ($scope.isLoadingAccounts) {
                return;
            }
            $scope.isLoadingAccounts = true;
            var max = $scope.accounts[$scope.accounts.length - 1].created_at;
            api.getReceiverAccounts(0, max, function (data) {
                utils.applyScope($scope, function () {
                    $scope.accounts = $scope.accounts.concat(data);
                    $scope.isLoadingAccounts = false;
                });
            }, function (msg) {
                log.logInfo("get receiver accounts failed. user_id:" + api.getCurrentUser().userId + "; msg:" + msg);
                api.notifyError(msg);
                utils.applyScope($scope, function () {
                    $scope.isLoadingAccounts = false;
                })
            });
        });
    };

    $scope.onScrollUp = function (callback) {
        $scope.getNotices(callback);
    };

    $scope.showPreview = function (n) {
        notices.showReceiverNoticePreview(n);
    };

    $scope.convertTime = function (source) {//将日期转化成视觉要求
        return notices.convertNoticeTime(source);
    };

    $scope.dateFilter = function (n, index, arrays) {
        var year, month, date, dateStr;
        var nowDate = new Date();
        var curDate = new Date();
        var yesterday = new Date();
        curDate.setTime(n.createTimestamp);
        yesterday.setDate(nowDate.getDate() - 1);
        if (index > 0) {
            var nextDate = new Date();
            nextDate.setTime(arrays[index - 1].createTimestamp);
            if (curDate.getFullYear() != nextDate.getFullYear()) {
                year = curDate.getFullYear();
                month = curDate.getMonth() >= 9 ? curDate.getMonth() + 1 : "0" + (curDate.getMonth() + 1);
                date = curDate.getDate() > 9 ? curDate.getDate() : "0" + curDate.getDate();
                dateStr = year + "/" + month + "/" + date;
            } else if (curDate.getMonth() != nextDate.getMonth() || curDate.getDate() != nextDate.getDate()) {
                month = curDate.getMonth() >= 9 ? curDate.getMonth() + 1 : "0" + (curDate.getMonth() + 1);
                date = curDate.getDate() > 9 ? curDate.getDate() : "0" + curDate.getDate();
                dateStr = month + "/" + date;
            }
        } else {
            year = curDate.getFullYear() == nowDate.getFullYear() ? "" : curDate.getFullYear();
            month = curDate.getMonth() >= 9 ? curDate.getMonth() + 1 : "0" + (curDate.getMonth() + 1);
            date = curDate.getDate() > 9 ? curDate.getDate() : "0" + curDate.getDate();
            if (year) {
                dateStr = year + "/" + month + "/" + date;
            } else {
                dateStr = month + "/" + date;
            }
        }

        if (dateStr) {
            if (curDate.toDateString() == nowDate.toDateString()) {
                n.dateSpliter = translate.instant("common.today");
            } else if (curDate.toDateString() == yesterday.toDateString()) {
                n.dateSpliter = translate.instant("common.yesterday");
            } else {
                n.dateSpliter = dateStr;
            }
        } else {
            n.dateSpliter = null;
        }

        return true;
    };

    $scope.convertBadgeNumber = function (num) {
        if (num > 99) {
            return 99;
        }
        return num;
    };

    var offNoticeReceived = $scope.$on("NewNoticeReceived", function (e, data) {
        log.logInfo("receive new notice received event:" + JSON.stringify(data));
        var n = notices.notice.create();
        notices.notice.convertFromNewData(n, data);
        n.readStatus = 0;
        var index = $scope.accounts.findIndex(function (ac) {
            return ac.board_id == n.accountId;
        });
        log.logInfo("account index: " + index);
        utils.applyScope($scope, function () {
            if (index < 0) {
                var ac = {
                    name: n.accountName,
                    avatar: n.accountAvatar,
                    title: n.topic,
                    board_id: n.accountId,
                    created_at: n.createTimestamp,
                    board_name: n.accountName
                };
                api.getAccountUnreadCount([n.accountId], function (data) {
                    ac.unread_count = data[0].unread_count;
                    utils.applyScope($scope, function () {
                        log.logInfo("un-shift new account : " + ac.name + ";id: " + ac.board_id);
                        $scope.accounts.unshift(ac);
                    });
                }, function (msg) {
                    log.logInfo("fail to load account unread count:" + msg);
                });
            } else {
                if($scope.accounts[index].bbs_id < n.id){
                $scope.accounts[index].name = n.accountName;
                $scope.accounts[index].avatar = n.accountAvatar;
                $scope.accounts[index].title = n.topic;
                $scope.accounts[index].created_at = n.createTimestamp;
                $scope.accounts[index].unread_count += 1;
                if (!$scope.accounts[index].notices) {
                    $scope.accounts[index].notices = [];
                }
                $scope.accounts[index].notices.push(n);

                var account = $scope.accounts[index];
                $scope.accounts.splice(index, 1);
                $scope.accounts.unshift(account);
                if (account === $scope.selectedAccount) {
                    $timeout(function () {
                        $scope.$broadcast("scrollToBottom", $scope.noticeContainerId);
                    });
                }
            }
            }
        });
    });

    var offNoticeReaded = $scope.$on("NoticeReaded", function (e, data) {
        log.logInfo("receive notice readed event:" + JSON.stringify(data));
        var index = $scope.accounts.findIndex(function (ac) {
            return ac.board_id == data.board_id;
        });
        if (index < 0) {
            return;
        } else {
            utils.applyScope($scope, function () {
                if($scope.accounts[index].unread_count > 0){
                $scope.accounts[index].unread_count -= 1;
                }
                if ($scope.accounts[index].notices && $scope.accounts[index].notices.length > 0) {
                    var nIndex = $scope.accounts[index].notices.findIndex(function (n) {
                        return n.id == data.bbs_id;
                    });
                    if (nIndex > -1 && $scope.accounts[index].notices) {
                        $scope.accounts[index].notices[nIndex].readStatus = 1;
                    }
                }
            });
        }
    });

    var offNoticeDeleted = $scope.$on("NoticeDeleted", function (e, data) {
        log.logInfo("receive notice deleted event:" + JSON.stringify(data));
        var index = $scope.accounts.findIndex(function (ac) {
            return ac.board_id == data.board_id;
        });
        if (index < 0) {
            return;
        } else {
            utils.applyScope($scope, function () {
                api.getAccountUnreadInfo($scope.accounts[index].board_id, function (data) {
                    var boardData = data.board_data;
                    utils.applyScope($scope, function () {
                        if (!boardData || !boardData.last_bbs_info || boardData.last_bbs_info.length == 0) {
                            if($scope.selectedAccount == $scope.accounts[index]){
                                $scope.selectedAccount=null;
                                $scope.backToMain();
                            }
                            $scope.accounts.splice(index,1);
                        }else{
                            $scope.accounts[index].title = boardData.last_bbs_info.title;
                            $scope.accounts[index].created_at = boardData.last_bbs_info.created_at;
                            $scope.accounts[index].unread_count = boardData.unread_count;
                        }
                    });


                }, function (msg) {
                    log.logInfo("fail to load account unread info:" + msg);
                });
                if ($scope.accounts[index].notices && $scope.accounts[index].notices.length > 0) {
                    var nIndex = $scope.accounts[index].notices.findIndex(function (n) {
                        return n.id == data.bbs_id;
                    });
                    if (nIndex > -1) {
                        if ($scope.accounts[index].notices[nIndex].readStatus == 0) {
                            $scope.accounts[index].unread_count -= 1;
                            $scope.accounts[index].notices[nIndex].readStatus = 1;
                        }
                    }
                }
            });
        }
    });
    var offScopeBackToAccount = $scope.$on("BackToAccount", function () {
        utils.applyScope($scope, function () {
            $scope.backToMain();
        });
    });

    var offBoardDelete = $scope.$on("board_destroy",function(e,data){
        log.logInfo("receive account delete event:" + JSON.stringify(data));
        var index = $scope.accounts.findIndex(function (ac) {
            return ac.board_id == data.board_id;
        });
        if(index> -1){
            var name = $scope.accounts[index].board_name;
            utils.applyScope($scope,function(){
                if($scope.selectedAccount == $scope.accounts[index]){
                    $scope.selectedAccount=null;
                    $scope.backToMain();
                }
                $scope.accounts.splice(index,1);
            });
            // notices.openUCAlert(translate.instant("common.account-delete-alert",{name:name}),function(){
            // });
        }

        //处理弹窗
        var selectedAccount = notices.retrieveSelectedAccount();
        if(selectedAccount && selectedAccount.id == data.board_id){
            pages.fnCall("publishWindow", "onAccountInvalid", translate.instant("common.account-delete-alert", {name: selectedAccount.name}), function () {
            });
        }
    });

    var offBoardEdit = $scope.$on("board_edit",function(e,data){
        log.logInfo("receive account edit event:" + JSON.stringify(data));
        var index = $scope.accounts.findIndex(function (ac) {
            return ac.board_id == data.board_id;
        });
        if(index> -1){
            utils.applyScope($scope,function(){
                $scope.accounts[index].board_name=data.board_name;
                $scope.accounts[index].avatar=data.avatar;
            });
        }
    });

    var offBoardUpdate = $scope.$on("board_update",function(e,data){
        //处理弹窗
        var selectedAccount = notices.retrieveSelectedAccount();
        if(selectedAccount && selectedAccount.id == data.board_id){
            var currentUser = utils.retrieveUser();
            var uIndex = data.editor_info.findIndex(function(user){
                return user.user_id == currentUser.userId;
            });
            if(uIndex < 0){
                pages.fnCall("publishWindow", "onAccountInvalid", translate.instant("common.no-account-right-alert", {name: selectedAccount.name}), function () {
                });
            }else{
                var ac = notices.account();
                ac.convertFromData(data);
                notices.saveSelectedAccount(ac);
                pages.fnCall("publishWindow", "updateAccount", ac, function () {
                });
            }
        }
    });

    $scope.$on("$destroy", function () {
        offNoticeReceived();
        offNoticeReaded();
        offNoticeDeleted();
        offScopeBackToAccount();
        offBoardDelete();
        offBoardEdit();
        offBoardUpdate();
    });
}]);
