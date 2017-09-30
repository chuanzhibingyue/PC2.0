/**
 * Created by wanglei on 15/10/10.
 */
(function (uc) {
    uc.modules.notice.NoticeManager = {
        init: function () {
            uc.modules.notice.Notice.init();
        },
        noticeFileUploaded: function (taskId, fileInfo) {
            uc.modules.notice.Notice.broadCast("FileUploaded", [taskId, fileInfo]);
        },
        noticeOrgInfoReceived: function () {
            uc.modules.notice.Notice.broadCast("NoticeOrgInfoReceived", arguments);
        },
        //验证当前用户是否有权限发布广播
        checkUserAuthorization: function () {
            var userId = uc.IContactService.getCurrentUserInfo().userId;
            var request = {
                controller: "user",
                action: "login.json",
                url: {
                    v: 1
                },
                method: 1,
                body: {
                    user_id: userId
                }
            };
            var self = this;
            uc.util.LogUtil.info("NoticeManager", "checkUserAuthorization", "", request);
            uc.IBBSService.BBSRequest(JSON.stringify(request), function (args) {
                if (args[0] === 0) {
                    self.authorizationTaskID = args[1];
                }
            });
        },
        getUnreadMessage: function () {
            var request = {
                controller: "bbs/stat",
                action: "total_unread.json",
                url: {
                    v: 1
                },
                method: 0
            };
            var self = this;
            uc.util.LogUtil.info("NoticeManager", "getUnreadMessage", "", request);
            uc.IBBSService.BBSRequest(JSON.stringify(request), function (args) {
                if (args[0] === 0) {
                    self.unReadMessageTaskID = args[1];
                }
            });
        },
        backToApp: function () {
            uc.IUIService.changeModule(uc.constants.UI.ModuleId.App);
        },
        onBBSResultReceived: function (taskID, response) {
            if (taskID === this.authorizationTaskID) {
                var result = response;
                uc.util.LogUtil.info("NoticeManager", "onAuthorizationResultReceived", "", result);
                if (result.code == 0 && !!result.data && !!result.data.permissions.bbs && result.data.permissions.bbs.editor == 1) {
                    uc.modules.component.MainMenuBar.noticeMenu.removeClass("hide");
                    uc.modules.notice.Notice.isEditor = true;
                    if (!uc.modules.notice.Notice.inited) {
                        uc.modules.notice.Notice.init(true);
                    }

                    if (!!result.data.stat && !!result.data.stat.bbs) {
                        var hideToolTip = ucClient.ClientService.getValueByKeySync("notice", "hide_tool_tip"); //从本地获取tooltip是否已关闭状态
                        if (hideToolTip === "1") {
                            return;
                        }
                        if (!result.data.stat.bbs.create) {
                            function removeTooltip(tag) {
                                $(tag).remove();
                                ucClient.ClientService.updateValueByKey("notice", "hide_tool_tip", "1"); //将tooltip已关闭状态记录到本地，下次将不再显示
                            }

                            $(".tooltip", uc.modules.component.MainMenuBar.noticeMenu).removeClass("hide").on("click", function (e) {
                                if ($(e.target).hasClass("icon-close")) {
                                    e.preventDefault();
                                    e.stopPropagation();
                                    removeTooltip(this);
                                }
                            }).fadeOut(10000, function () {
                                removeTooltip(this);
                            });
                        }
                    }
                } else {
                    uc.modules.component.MainMenuBar.noticeMenu.addClass("hide");
                    if (uc.modules.notice.Notice.isEditor) {
                        uc.modules.notice.Notice.isEditor = false;
                        uc.modules.notice.Notice.broadCast("OnLostPublishRight");
                        uc.IUIService.triggerCrossWindowEvent("ClosePublishWindows", {});
                    }
                }
            } else if (taskID === this.unReadMessageTaskID) {
                if (response.code === 0 && !!response.data) {
                    var data = response.data;
                    uc.modules.notice.Notice.unreadCount = data.total_unread_count ? data.total_unread_count : 0;
                    uc.util.LogUtil.info("NoticeManager", "receive unread count:", data.total_unread_count, "");
                    uc.modules.notice.Notice.lastUnreadInfo = data.last_bbs_info;
                    if (data.last_bbs_info && data.last_bbs_info.title !== undefined) {
                        uc.IUIEvent.OnNoticeLastMessageReceived(uc.modules.notice.Notice.appInfo.appId, data.last_bbs_info.title, parseInt(data.last_bbs_info.created_at));
                    }
                }
            }
        },
        openNoticeWindow: function () {
            uc.modules.notice.Notice.showReceiverView();
        },
        notifyNewNoticeReceived: function (title, titleColor, titleBkColor, titleElements, status, data, detailUrl, msgId, msgSeq, msgTime, detailAuth, oaExternalData, titleStyle, customizedType, customizedData) {
            this.getUnreadMessage();
            uc.util.LogUtil.info("NoticeManager", "receive unread count:", uc.modules.notice.Notice.unreadCount, "");
            var receiveNotice = this.convertReceiveNoticeData(title, titleColor, titleBkColor, titleElements, status, data, detailUrl, msgId, msgSeq, msgTime, detailAuth, oaExternalData, titleStyle, customizedType, customizedData);
            uc.modules.notice.Notice.broadCast("NewNoticeReceived", receiveNotice);
        },
        convertReceiveNoticeData: function (title, titleColor, titleBkColor, titleElements, status, data, detailUrl, msgId, msgSeq, msgTime, detailAuth, oaExternalData, titleStyle, customizedType, customizedData) {
            return {
                "title": title,
                "color": titleColor,
                "status": status,
                "elements": data,
                "detailURL": detailUrl,
                "detailAuth": detailAuth,
                "customizedType": customizedType,
                "customizedData": customizedData,
                "titleElements": titleElements,
                "titleStyle": titleStyle,
                "read_status": 0
            };
        },
        notifyNoticeMsgReceived: function (data) {
            uc.util.LogUtil.info("NoticeManager", "notifyNoticeMsgReceived", data);
            data = JSON.parse(data);
            if (data.action == "ackread") {
                this.getUnreadMessage();
                uc.modules.notice.Notice.broadCast("NoticeReaded", data);
                uc.IUIEvent.OnNoticeConversationStatusChanged(uc.modules.notice.Notice.appInfo.appId);
            } else if (data.action == "delete") {
                uc.modules.notice.Notice.broadCast("NoticeDeleted", data);
                this.getUnreadMessage();
            } else {
                if (data.action == "board_update" || data.action == "board_create" || data.action == "board_destroy") {
                    if (data.action == "board_destroy") {
                        this.getUnreadMessage();
                    } else {
                        this.checkUserAuthorization();
                    }
                }
                uc.modules.notice.Notice.broadCast(data.action, data);
            }
        },
        hidePanel: function () {
            uc.modules.notice.Notice.hideView();
        },
        isNoticeConversation: function (conversationId) {
            return conversationId == uc.modules.notice.Notice.appInfo.appId;
        },
        getNoticeUnreadCount: function () {
            uc.util.LogUtil.info("NoticeManager", "getUnreadCount", uc.modules.notice.Notice.unreadCount);
            return uc.modules.notice.Notice.unreadCount;
        },
        setNoticeUnreadCount: function (count) {
            uc.util.LogUtil.info("NoticeManager", "setUnreadCount", count);
            uc.modules.notice.Notice.unreadCount = count;
        },
        getLastNoticeInfo: function () {
            return uc.modules.notice.Notice.lastUnreadInfo;
        },
        getAppInfo: function () {
            return uc.modules.notice.Notice.appInfo;
        },
        setAppInfo: function (app) {
            uc.modules.notice.Notice.appInfo = app;
            uc.modules.notice.Notice.setMenuTips();
        },
        loadAppInfo: function () {
            var appId = uc.modules.notice.Notice.appInfo.appId;
            uc.IOpenApiService.getAppInfo([appId]);
        },
        downloadFile: function (id, url, targetPath, callback) {
            var savePath = ucClient.ClientService.getDocumentDataDirSync(0) + "/notice/" + id;
            if (!this.downloadQueue) {
                this.downloadQueue = [];
            }
            var that = this;
            uc.IFSService.downloadFile(url, savePath, function (args) {
                if (args[0] != 0 || args[1] == null) {
                    uc.modules.component.PromptMessageBox.open({
                            title: uc_resource.MessageBox.Notification,
                            message: uc_resource.OpenApi.UnSupportDownload,
                            buttonMode: "ok",
                            OkText: uc_resource.MessageBox.OK
                        },
                        function (result) {
                        }
                    );
                    callback();
                    return;
                }

                var taskId = args[1];
                that.downloadQueue[taskId] = {
                    id: id,
                    path: targetPath,
                    callback: callback
                };
            });
        },
        onDeleteLastAccount: function () {
            uc.modules.component.MainMenuBar.noticeMenu.addClass("hide");
            if (uc.modules.notice.Notice.isEditor) {
                uc.modules.notice.Notice.isEditor = false;
                uc.IUIService.triggerCrossWindowEvent("ClosePublishWindows", {});
            }
        }
    };
})(uc);
