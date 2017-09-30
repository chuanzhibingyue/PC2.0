/**
 * Created by wanglei on 15/10/10.
 */
(function (uc) {
    uc.modules.notice.NoticeEventListener = function () {

    };
    jQuery.extend(uc.modules.notice.NoticeEventListener.prototype,
        uc.interfaces.IClientEvent,
        uc.interfaces.IContactEvent,
        uc.interfaces.IGroupEvent,
        uc.interfaces.IFSEvent,
        uc.interfaces.IBBSEvent,
        uc.interfaces.IOpenApiEvent,
        uc.interfaces.IUIEvent, {
            OnInitClient: function (clearData) {
                uc.modules.notice.NoticeManager.loadAppInfo();
                uc.modules.notice.NoticeManager.checkUserAuthorization();
                uc.IUIService.bindCrossWindowEvent("OnRequestStoreCover", function (url) {
                    var path = url.path;
                    var fidIndex = path.indexOf("fid=");
                    if (fidIndex > -1) {
                        var fid = path.substr(fidIndex + 5).replace(/&[\w=]+/g, '');
                        uc.modules.notice.NoticeManager.downloadFile(fid, path, "", function (data) {
                            uc.util.LogUtil.info("NoticeEventListener", "OnRequestStoreCover", "save " + path + "to " + data.savedPath, "");
                            ucClient.ClientService.updateValueByKey("notice", path, data.savedPath);
                        });
                    }
                });
            },
            OnConversationDataUpdate: function () {
                uc.modules.notice.NoticeManager.getUnreadMessage();
            },
            OnWindowResized: function () {
                uc.modules.notice.Notice.changNoticeFrameSize();
            },
            OnFileUploading: function (errorCode, taskId, percent) {
                //todo
            },
            OnFileUploaded: function (errorCode, taskId, fileName, fileSize, operatorId, fid, operatorTime, downloadURL, convertDownloadURL) {
                var fileInfo = {
                    name: fileName,
                    size: fileSize,
                    url: downloadURL,
                    thumbnailURL: convertDownloadURL,
                    operatorId: operatorId,
                    operatorTime: operatorTime,
                    fid: fid
                };

                if (errorCode === 0) {
                    uc.modules.notice.NoticeManager.noticeFileUploaded(taskId, fileInfo);
                }
            },
            OnFileDownloaded: function (errorCode, taskId, savePath) {
                var downloadQueue = uc.modules.notice.NoticeManager.downloadQueue;
                if (downloadQueue && downloadQueue[taskId]) {
                    if (downloadQueue[taskId].path) {
                        uc.util.FileUtil.copyFile(savePath, downloadQueue[taskId].path);
                    }

                    if (downloadQueue[taskId].callback) {
                        downloadQueue[taskId].callback({
                            id: downloadQueue[taskId].id,
                            savedPath: savePath,
                            targetPath: downloadQueue[taskId].path
                        });
                    }
                    delete downloadQueue[taskId];
                }
            },
            OnOrgInfoReceived: function (errorCode, department, myDepartment) {
                uc.modules.notice.NoticeManager.noticeOrgInfoReceived(errorCode, department, myDepartment);
            },
            OnOpenPresetApp: function (appId) {
                if (uc.modules.notice.NoticeManager.isNoticeConversation(appId)) {
                    uc.modules.notice.NoticeManager.init();
                }
            },
            OnModuleChanged: function (moduleId) {
                if (moduleId == uc.constants.UI.ModuleId.Notice) {
                    uc.modules.notice.NoticeManager.init();
                } else {
                    $(".tooltip", uc.modules.component.MainMenuBar.noticeMenu).addClass("hide");
                }
            },
            BBSServiceOnResponse: function (errorCode, requestID, response) {
                uc.util.LogUtil.info("NoticeEventListener", "BBSServiceOnResponse", "BBSServiceOnResponse", {
                    "errorCode": errorCode,
                    "requestID": requestID,
                    "response": response
                });
                if (errorCode === 0) {
                    uc.modules.notice.NoticeManager.onBBSResultReceived(requestID, response);
                }
            },
            onOpenNoticeReceiverWindow: function () {
                uc.modules.notice.NoticeManager.openNoticeWindow();
            },
            OnOAMessageReceived: function (channel, from, to, title, titleColor, titleBkColor, titleElements, status, data, detailUrl, msgId, msgSeq, msgTime, detailAuth, oaExternalData, titleStyle, customizedType, customizedData) {
                //只接收ucas 不接收其他的chanel事件
                if (customizedType == "application/json" && channel === uc.constants.Message.MessageChannel.UCAS && uc.modules.notice.NoticeManager.isNoticeConversation(from.userId)) {
                    uc.util.LogUtil.info("NoticeEventListener", "OnOAMessageReceived", "OnOAMessageReceived", {"OA message data:": arguments});
                    uc.modules.notice.NoticeManager.notifyNewNoticeReceived(title, titleColor, titleBkColor, titleElements, status, data, detailUrl, msgId, msgSeq, msgTime, detailAuth, oaExternalData, titleStyle, customizedType, customizedData);
                }
            },
            OnOACustomizedMessageReceived: function (channel, from, data1, data2, data3, data4) {
                if (uc.modules.notice.NoticeManager.isNoticeConversation(from.userId) && channel === uc.constants.Message.MessageChannel.UCAS) {
                    uc.util.LogUtil.info("NoticeEventListener", "OnOACustomizedMessageReceived", "OnOACustomizedMessageReceived", {"read data:": data1});
                    uc.modules.notice.NoticeManager.notifyNoticeMsgReceived(data1);
                }
            },
            OnGetAppInfoReceived: function (errorCode, apps) {
                uc.util.LogUtil.info("NoticeEventListener", "OnGetAppInfoReceived", "OnGetAppInfoReceived", {"received data:": apps});
                if (errorCode === 0 && apps.length > 0) {
                    for (var i = 0; i < apps.length; i++) {
                        var app = apps[i];
                        if (uc.modules.notice.NoticeManager.isNoticeConversation(app.appId)) {
                            uc.modules.notice.NoticeManager.setAppInfo(app);
                        }
                    }
                }
            },
            OnOpenChatWindow: function (chatType, chatId, openChatWindowSourceModule, meetingData, siteId) {
                if (!uc.modules.notice.NoticeManager.isNoticeConversation(chatId)) { //当从别的地方打开聊天窗口时，如果不是广播，就隐藏广播界面
                    //避免与module changed冲突
                    setTimeout(function () {
                        uc.modules.notice.NoticeManager.hidePanel();
                    }, 50);
                }
            }
        });
})(uc);
