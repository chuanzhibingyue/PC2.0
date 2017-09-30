/**
 * Created by zuo.dai on 2015/8/14.
 */
(function(uc){
	var rootPath = (function () {
		var href = location.href;
		var index = href.indexOf('/html');
		return href.substring(0, index + 6);
	})();

	function _getFileName(path) {
		var formatPath = path.replace(/\//g, "\\");
		var parts = formatPath.split("\\");
		if (parts.length > 0) {
			return parts[parts.length - 1];
		}
		else {
			return path;
		}
	}

	function _getFileSize(path) {
		return uc.util.FileUtil.getFileSize(path);
	}

    uc.modules.chat.FileList = function(){
		this.meetingOption = null;//会议相关参数
		this.isActivity = false;//文件列表是激活状态
    };
    jQuery.extend(uc.modules.chat.FileList.prototype, {
		init: function (groupId, parentNode, chatType, isMeeting, meetingOption) {
            this.menuHtml =    '<div class="menu">'
                +                '<div class="items">'
                +                   '<div class="item"><div class="menu-item" id="comment" style="display: none;">'+uc_resource.Chat.Discuss+'</div></div>'
                +                   '<div class="item"><div class="menu-item" style="display: none;" id="collection">'+uc_resource.Chat.Collect+'</div></div>'
                +                   '<div class="item"><div class="menu-item"  id="resend" style="display: none;">'+uc_resource.Chat.Transmit+'</div></div>'
                +                   '<div class="item"><div class="menu-item"  style="display: none;" id="delete">'+uc_resource.Chat.Delete+'</div></div>'
                +                   '<div class="item"><div class="menu-item"  style="display:none;" id="saveCloud">'+uc_resource.MessageBox.saveCloud+'</div></div>'
                +                   '<div class="item"><div class="menu-item"  style="display:none;" id="cloudPreview">'+uc_resource.Chat.Preview+'</div></div>'
                +                   '<div class="item"><div class="menu-item"  style="display: none;" id="preview">'+uc_resource.Chat.Preview+'</div></div>'
                +                   '<div class="item"><div class="menu-item children" style="display: none;" id="share">'+uc_resource.Chat.Share
                +                       '<div class="arrow">'
                +                           '<div class="second-menu">'
                +                               '<div class="item"><div class="menu-item" id="dongtai">'+uc_resource.Chat.News+'</div></div>'
                +                               '<div class="item"><div class="menu-item" id="anli">'+uc_resource.Chat.Case+'</div></div>'
                +                            '</div>'
                +                       '</div>'
                +                   '</div></div>'
                +                 '</div>'
              //  +                 '<div class="menu-bottom"></div>'
                +               '</div>';
            this.node = $('<div class="item-list"></div>');
            this.node.append(this.menuHtml);
            this.node.appendTo(parentNode);
            parentNode.append('<div class="no-files" style="display: none">'+uc_resource.Chat.NoFiles+'</div>'
                +               '<div class="load-fail" style="display: none">'
                +                    '<div class="fail-img"/>'
                +                    '<div class="tips">'+uc_resource.Chat.LoadingFailed+'</div>'
                +                    '<div class="re-load">'+uc_resource.Chat.Reloading+'</div>'
                +               '</div>'
                +               '<div class="bottom-margin"/>'
                +               '<div class="more-file-line">'
                +                   '<span class="more-file-loading" style="display:none"/>'
                +               '</div>');
			//status
			this.isLoadedItemByLoadingcache = false;//缓存的正在loading的列表是否已经初始化
			if (!this.uploadingItemMap)
				this.uploadingItemMap = new uc.util.HashMap();
			if (!this.reUploadItemMap) {
				this.reUploadItemMap = new uc.util.HashMap();
            }
            this.isLastFileType = false;
            this.parentNode = parentNode;
            this.pageCount = 20;
            this.page = 1;
            this.groupId = groupId;
            this.lastDate = "";
            this.recvFileEnd = false;
            this.files = {};
            this.currMenuFileID = null;
            this.chatType=chatType;
			this.isMeeting = isMeeting;
			this.meetingOption = meetingOption;

			if (this.isMeeting) {
				//如果是上传会议资料并且no-file状态，需要更换no-file图标，增加上传会议资料button，
				var _nofiles = "<p>" + uc_resource.Meeting.unuploadMeetingData + "</p><img src='" + rootPath + "/images/meeting/today_no_meeting.png'/><p><a class='upload-MeetingDocument-button btn-uikit btn-dark' href='javascript:void(0);'>" + uc_resource.Meeting.uploadMeetingData + "</a></p>";
				$('.no-files', this.parentNode).html(_nofiles);
				$('.no-files', this.parentNode).addClass("no-files-metting");
				//会议资料需要在底部添加上传会议资料bar
				if (!$('.upload-MeetingDocuments').length) {
					this.parentNode.after("<div class='upload-MeetingDocuments hide'><a class='upload-MeetingDocument-button' href='javascript:void(0);'>" + uc_resource.Meeting.uploadMeetingData + "</a></div>");
				}
                uc.IMeetingService.getMinutesList(meetingOption.eventId, meetingOption.startTime);
                //IGroupService.getDocumentList(this.groupId, this.page, this.pageCount);
			} else {
                uc.IGroupService.getDocumentList(this.groupId, this.page, this.pageCount);
            }
            this.bindEvent();
			this.onMoreFileLoading();
			this.isActivity = true;
        },
        bindEvent: function() {
            var This = this;
            this.moreFileLoading = $('.more-file-loading', this.parentNode);
            $('.bottom-margin', this.parentNode).hide();

            this.itemList =  this.node;
            this.node.unbind();
            this.parentNode.unbind();
            this.itemList.on('click', '.menu-item',
                function(){
                    if (This.currMenuFileID == null) {
                        return;
                    }
                    var fileInfo = This.files[This.currMenuFileID];
                    if ($(this).attr('id') == "comment") {
                        $(".fileList").hide();
                        This.showFileComment(fileInfo);
                        return false;
                    } else if($(this).attr('id') == "resend"){
                        if (fileInfo.cloudType==0 && fileInfo.contentType == 1) {
                            uc.modules.chat.MessageManager.forwardCodeMessage(fileInfo.codeTitle, fileInfo.langType, fileInfo.codeHead,
                                fileInfo.codeDesc, fileInfo.codeSize, fileInfo.codeUrl, fileInfo.codeLine, fileInfo.createTime);
                        }else {
                            if(fileInfo.cloudType==1){
                                var dataArr=JSON.parse(fileInfo.detailContent);
                                uc.modules.chat.MessageManager.forwardCloudFileMessage(fileInfo.fileName, fileInfo.fileSize, fileInfo.fileUrl,fileInfo.detailContent,dataArr);
                            }else if(fileInfo.cloudType==0){
                                if(fileInfo.type == uc.constants.Message.FileListType.Record && This.isMeeting) {
                                    uc.IUIService.openChooseContactWindow(
                                        {
                                            key: "forwardMessage" + (new Date().getTime()),
                                            allowMultipleSelect: false,
                                            allowChooseOrg:false
                                        }, function (args) {
                                            var title = uc.util.StringUtil.formatString(uc_resource.Message.MeetingVideoWithMeetingName, [fileInfo.operatorName]);
                                            var videoDuration = fileInfo.videoEndTime - fileInfo.videoStartTime;
                                            uc.IUIService.triggerCrossWindowEvent(uc.constants.UI.CrossWindowEventName.OnForwardMeetingVideo, {uniqueName: 'messageChat', args:args,
                                                videoName: title,videoSize: fileInfo.size,videoURL: fileInfo.videoURL,videoData: fileInfo.videoThumb,thumbnailsURL: fileInfo.videoThumbURL,
                                                videoDuration: videoDuration,operatorName: fileInfo.operatorName,operatorId: fileInfo.operatorId,operatorAvatar: fileInfo.operatorAvatar,from:'minutes'});
                                        });
                                } else {
                                    uc.modules.chat.MessageManager.forwardFileMessage(fileInfo.fileName, fileInfo.fileSize, fileInfo.fileUrl);
                                }
                            }

                        }
                    } else if($(this).attr('id') == "delete"){
                        uc.modules.component.PromptMessageBox.open({title: uc_resource.Common.Prompt, message: uc_resource.Chat.DeleteFile, buttonMode:"both", OkText: uc_resource.Common.OK, CancelText: uc_resource.Common.Cancel},
                            function(result){
                                this.close();
                                if (result == 0) {
	                                if(This.isMeeting){
		                                uc.IMeetingService.deleteMeetingFile(fileInfo.fileId,function(args){
			                                if (args[0] == 0) {
				                                var item = $('#file' + fileInfo.fileId, This.itemList);
				                                item.remove();
			                                }
		                                });
	                                }else{
		                                uc.IGroupService.deleteDocument(This.groupId, fileInfo.fileId,
			                                function(args) {
				                                if (args[0] == 0) {
					                                var item = $('#file' + fileInfo.fileId, This.itemList);
					                                item.remove();
				                                }
			                                }
		                                );
	                                }

                                }
                            });
                    } else if($(this).attr('id') == "preview"){
                        console.log(fileInfo, fileInfo.codeURL, fileInfo.codeTitle, fileInfo.codeSize, fileInfo.langType);
                        uc.modules.chat.ChatManager.showCodePreview(fileInfo.codeUrl, fileInfo.codeTitle, fileInfo.codeSize, fileInfo.langType);
                    }else if($(this).attr('id') == "saveCloud"){
                        if(fileInfo.cloudType==1){
                            uc.modules.chat.ChatManager.fileListSaveCloud(fileInfo.detailContent);
                        }else if(fileInfo.cloudType==0){
                            var dataStr=JSON.stringify({"filename":fileInfo.fileName,"url":fileInfo.fileUrl});
                            uc.modules.chat.ChatManager.fileListSaveCloud(dataStr);
                        }

                    }else if($(this).attr('id') == "cloudPreview"){
                        var cloudData=fileInfo.detailContent;
                        uc.IUIEvent.OnCloudFilePreviewer(cloudData);
                    } else if ($(this).attr('id') == 'share') {
                        if(fileInfo.type == uc.constants.Message.FileListType.Record && This.isMeeting){
                            var title = uc.util.StringUtil.formatString(uc_resource.Message.MeetingVideoWithMeetingName, [fileInfo.operatorName]);
                            var videoDuration = fileInfo.videoEndTime - fileInfo.videoStartTime;
                            uc.IUIService.triggerCrossWindowEvent(uc.constants.UI.CrossWindowEventName.OnShareMeetingVideo, {uniqueName: 'shareMeetingVideo',
                                videoURL: fileInfo.videoURL,meetingName: title,videoSize: fileInfo.size,thumbnailsURL: fileInfo.videoThumbURL,videoDuration: videoDuration});
                        } else {
                            //other share
                        }

                    }

                    $(this).parents('.menu').hide();
                });
            this.itemList.on('click', '.download',
                function(){
                    var downloadBtn = $(this);
                    if(downloadBtn.text() == "打开") {
                        var path = downloadBtn.attr('local-path');
                        if (uc.util.FileUtil.openFile(path) != 0){
                            uc.modules.component.PromptMessageBox.open({title: uc_resource.Common.Prompt, message: uc_resource.Chat.OpenFailed, buttonMode:"ok", OkText: uc_resource.Common.Known},
                                function(result){
                                    this.close();
                                });
                        }
                        return;
                    }
                    var url = downloadBtn.attr("data-url");
                    var fileId =  downloadBtn.attr("file-id");
                    var item = $('#file' + fileId, This.itemList);
                    var fname = $('.name', item).text();
                    var fileInfo = This.files[fileId];
                    if(fileInfo.cloudType==1){
                        if(!uc.modules.chat.ChatManager.checkCloudOpenStatus()){
                            return;
                        }
                        if(!uc.modules.chat.ChatManager.checkCloudLoadStatus()){
                            return;
                        }
                        var dataArr=JSON.parse(fileInfo.detailContent);
                        uc.modules.chat.ChatManager.downloadCloudDir(dataArr,function(data,savePath){
                            var data=JSON.parse(data);
                            var taskId=data.taskid;
                            var gkItem="fileGkDown"+taskId;
                            uc.modules.chat.ChatManager.cloudFileInfoDownloading[taskId] = {
                                msgId:"",
                                taskId:taskId,
                                fileId:fileId
                            };
                            uc.modules.chat.ChatManager.fileDownLoadInfo[taskId]={
                                fileSavePath:savePath,
                                hash:savePath,
                                msgId:""
                            };
                            $('.progress', item).show().attr("id",gkItem);
                            downloadBtn.hide();
                        });

                    }else if(fileInfo.cloudType==0){
                        uc.modules.chat.ChatManager.downloadFile(null,"file" + fileId, url, fname,
                            function(){
                                $('.progress', item).show();
                                downloadBtn.hide();
                            });
                    }
                });

            this.parentNode.scroll(function(){
                $('.menu', This.parentNode).hide();
                if (This.recvFileEnd) {
                    return;
                }
                var viewH =$(this).height(),
                    contentH =$(this).get(0).scrollHeight,
                    scrollTop =$(this).scrollTop();
                if(scrollTop/(contentH -viewH)>=0.95){ //到达底部100px时,加载新内容
                    This.onMoreFileLoading();
                    console.log(This.groupId, This.page + 1, This.pageCount);
                    This.page = This.page + 1;
                    uc.IGroupService.getDocumentList(This.groupId, This.page, This.pageCount);
                }
            });

//            //UI事件
//            this.parentNode.on('click',
//                function(e){
//                    if ($(e.target).hasClass('menu-item')) {
//                        return;
//                    }
////                    if (uc.modules.chat.ChatManager.fileComment.isShow()){
////                        uc.modules.chat.ChatManager.fileComment.show(false);
////                    }
//                });
            this.itemList.on('click', '.more',
                function(){
                    This.currMenuFileID = $(this).attr("file-id");
                    var fileInfo = This.files[This.currMenuFileID];
                    var menuObj = $('.menu',  This.parentNode);
                    var currentUserId = uc.IContactService.getCurrentUserInfo().userId;
                    $('#saveCloud,#cloudPreview,#preview,#comment,#resend', menuObj).hide();
                    var cloudType=uc.util.StorageUtil.getData('cloudType');
                    var cloudTokenResult=uc.util.StorageUtil.getData('CloudLoginResult');
                    if (fileInfo.creatorId ==  currentUserId){
                        $('#delete', menuObj).show();
                    } else {
                        $('#delete', menuObj).hide();
                    }
                    if (fileInfo.cloudType==0 && fileInfo.contentType ==  1){
                        $('#preview', menuObj).show();
                    } else {
                        $('#preview', menuObj).hide();
                    }
                    //如果是云盘文件
                    if(fileInfo.cloudType==1){
                        if(cloudType==1 && cloudTokenResult==0){
                            $('#saveCloud,#resend', menuObj).show();
                        }else{
                            $('#saveCloud,#resend', menuObj).hide();
                        }
						//会议不显示评论
						if(!This.isMeeting){
                            //$('#share', menuObj).hide();
                            $('#comment', menuObj).show();
                        }
                    }
                    //FS文件
                    else if(fileInfo.cloudType==0){
                        if(cloudType==1 && cloudTokenResult==0 &&  fileInfo.contentType == 0){
                            $('#saveCloud', menuObj).show();
                        }else{
                            $('#saveCloud', menuObj).hide();
                        }
						//会议不显示评论
						if(!This.isMeeting) {
                            $('#comment', menuObj).show();
                        } else {
                            $('#share', menuObj).hide();
                        }
						$('#resend', menuObj).show();
                    }
                    var left = $(this).offset().left;
                    var top = $(this).offset().top;
                    menuObj.show();
                    $(".item-list .menu em,.item-list .menu span").remove();
                    $(".item-list .menu a,.item-list .menu b").remove();
                    var winHeight = $(window).height();
                    var m=$(".item-list .menu").height();
                    if (top + 160 > winHeight) {
                        menuObj.addClass('down-arrow');
                        $(".item-list .menu").append("<em></em><span></span>");
                        menuObj.css({position: "fixed",'top':top - m-10,'left':left - 70, 'z-index':2});
                        return;
                    }
                    $(".item-list .menu").append("<a></a><b></b>");
                    menuObj.removeClass('down-arrow');
                    menuObj.css({position: "fixed",'top':top + 30,'left':left - 70, 'z-index':2});
                });

            this.itemList.on('click', '.record-more', function(){
                This.currMenuFileID = $(this).attr("file-id");
                var fileInfo = This.files[This.currMenuFileID];
                var menuObj = $('.menu',  This.parentNode);
                $('#saveCloud,#cloudPreview,#preview,#comment,#resend,#delete', menuObj).hide();
                var cloudType=uc.util.StorageUtil.getData('cloudType');
                var cloudTokenResult=uc.util.StorageUtil.getData('CloudLoginResult');
                $('#resend', menuObj).show();
                $('#share', menuObj).show();
                var left = $(this).offset().left;
                var top = $(this).offset().top;
                menuObj.show();
                $(".item-list .menu em,.item-list .menu span").remove();
                $(".item-list .menu a,.item-list .menu b").remove();
                var winHeight = $(window).height();
                var m=$(".item-list .menu").height();
                if (top + 160 > winHeight) {
                    menuObj.addClass('down-arrow');
                    $(".item-list .menu").append("<em></em><span></span>");
                    menuObj.css({position: "fixed",'top':top - m-10,'left':left - 70, 'z-index':2});
                    return;
                }
                $(".item-list .menu").append("<a></a><b></b>");
                menuObj.removeClass('down-arrow');
                menuObj.css({position: "fixed",'top':top + 30,'left':left - 70, 'z-index':2});
            });

            this.itemList.on('click', '.view-minutes', function() {
                This.currMenuFileID = $(this).attr("file-id");
                var eventId = $(this).attr("event-id");
                var options = {
                    width: 540,
                    height: 530,
                    refreshIfExist: false,
                    centerScreen: true,
                    modal: true,
                    resizable: false,
                    caption: uc.constants.UI.DragCaption.EditMeetingMinutes
                };

                uc.util.StorageUtil.setData("Meeting-Minutes:summaryId", parseInt(This.currMenuFileID));
                uc.util.StorageUtil.setData("Meeting-Minutes:Data", { eventId: parseInt(eventId)});
                uc.util.WindowUtil.open("page/editMinutes.html", "editMinutes", options);
            });

            this.itemList.on('mouseover', '.file-info',
                function() {
                    if($('.menu',  This.parentNode).is(":visible")){
                        return;
                    }
                    $('.operation', This.node).hide();
                    $('.operation', $(this)).show();
                });
            this.itemList.on('mouseout', '.file-info',
                function() {
                    if($('.menu',  This.parentNode).is(":visible")){
                        return;
                    }
                    $('.operation', $(this)).hide();
                });

            this.parentNode.on('click', ".re-load",
                function() {
                    $('.load-fail', This.parentNode).hide();
                    uc.IGroupService.getDocumentList(This.groupId, This.page, This.pageCount);
		    });

            //会议上传文件
            this.parentNode.on('click', '.upload-MeetingDocument-button', function () {
                This.openUploadWindow();
            });

            this.itemList.on('click', '.file-info .loading-operation a',
                function (e) {
                    var _this = $(this);
                    var action = _this.attr('action');
                    var taskId = _this.attr('taskid');
                    console.log(_this.attr('action'));
                    switch (action) {
                        case "reload"://重新上传
                            //重新上传
                            if (This.reUploadItemMap) {
                                var path = This.reUploadItemMap.get(taskId + "");
                                if (path)
                                    This.sendFile(path, This.meetingOption);
                            }
                            break;
                        case "cancel"://取消上传
                            //send  cancelUploadFile message to fs server
                            uc.IFSService.cancelUploadFile(parseInt(taskId));
                            This.checkFileItem(_this);
                            This.deleteFileItemByTaskId(taskId);
                            break;
                        case "delete"://没有上传成功，删除当前文件
                            //删除dom
                            //删除当前节点}
                            This.deleteFileItemByTaskId(taskId);
                            break;
                    }
                    e.stopPropagation();
                });

            this.itemList.on('click', '.view-record', function () {
                var url = $(this).attr('href');
                uc.IClientService.getUccServerDomain(function(arg){
                    var domain=arg[1];
                    var _url=""+domain+"//"+"microblog/beeweb/theme/glaucous/qsplayer/meetingVideo.html"+"#"+url;
                    window.open(_url);
                });
            });
        },
        OnGkFileDownloadCallBack:function(arg){
            if(!$(".fileList").is(":visible")){
                return;
            }
            if(arg){
                var percent=arg.detail.percent;
                var taskId=arg.detail.taskid;
                var hash=arg.detail.hash;
                var opObj=$("#fileGkDown"+taskId);
                var item=opObj.parents("li");
                if(arg.detail.hasOwnProperty("error")){
                    uc.util.LogUtil.info('FileList', 'gkTransfer = ' + JSON.stringify(arg.detail),null,null);
                    $('.progress', item).hide().attr("value", "0");
                    $('.download', item).show().text(uc_resource.MessageBox.download);
                }else{
                    if(uc.modules.chat.ChatManager.cloudFileInfoDownloading[taskId]){
                        var fileId=uc.modules.chat.ChatManager.cloudFileInfoDownloading[taskId].fileId;
                        if(!$("#fileGkDown"+taskId).length){
                            item=$("#"+fileId+hash);
                            $('.progress', item).attr("id","fileGkDown"+taskId).show();
                            $('.download', item).hide();
                        }
                    }
                    item.find(".progress").attr("value",percent);
                    if(percent==100){
                        if(uc.modules.chat.ChatManager.fileDownLoadInfo[taskId]){
                            var localPath=uc.modules.chat.ChatManager.fileDownLoadInfo[taskId].fileSavePath;
                            uc.modules.chat.ChatManager.downloadedFiles[arg.detail.hash]=localPath;
                        }
                        item.find(".progress").hide();
                        $('.progress', item).hide().attr("value", "0");
                        $('.download', item).show().text(uc_resource.MessageBox.open).attr('local-path',localPath);
                        delete uc.modules.chat.ChatManager.cloudFileInfoDownloading[taskId];
                    }
                }
            }

        },
        onBodyClick: function(e) {
            if ($(e.target).hasClass('more') || $(e.target).hasClass('record-more')) {
                return;
            }
            $('.menu', this.parentNode).hide();
        },

        OnFileDownloading: function(itemId, percent){
            var item = $('#' + itemId, this.itemList);
            if (item.length <= 0) {
                return;
            }
            $('.progress', item).show();
            $('.progress', item).attr("value", percent);
            $('.download', item).hide();
        },
        OnFileDownloaded: function(errorCode, itemId, savePath){
            var item = $('#' + itemId, this.itemList);
            if (item.length <= 0) {
                return;
            }
            $('.progress', item).hide();
            $('.progress', item).attr("value", "0");
            $('.download', item).show();
            if (errorCode == 0) {
                $('.download', item).text(uc_resource.Chat.Open);
                $('.download', item).attr('local-path',savePath);
            }
        },
        onDocListReceived: function(errorCode, documents){
            if (documents.length <= 0 && $(".file-item", this.node).length <= 0){
                if (errorCode == 0){
					//有缓存正loading的文件，不显示no-files
					if (this.uploadingItemMap == null || this.uploadingItemMap.isEmpty())
						$('.no-files', this.parentNode).show();
					this.recvFileEnd = true;
				} else {
					$('.load-fail', this.parentNode).show();
				}
				$('.upload-MeetingDocuments').hide();
				//此处设置bottom为0 是和会议资料上传显示有关（此状态下css设置bottom为40px）
				this.parentNode.css("bottom", 0);
			} else {
				if (this.isMeeting) {
					//this.parentNode.css("bottom", "40px");
					//$('.upload-MeetingDocuments').show();
				}
                $('.load-fail', this.parentNode).hide();
			}
			this.onMoreFileLoadEnd();
            $('.bottom-margin', this.parentNode).show();

            for (var i = 0; i < documents.length; i++) {
                var docObj = documents[i];
                this.files[docObj.fileId + ''] = docObj;
                var date = new Date(docObj.uploadTime).toISOString().slice(0,10);
                if (this.lastDate != date) {
                    this.lastDate = date
                    this.addDateItem(docObj.uploadTime);
                }
                this.addFileItem(docObj.fileName, docObj.fileSize, docObj.uploadTime,
                    docObj.commentCount, docObj.fileUrl, docObj.fileId, docObj.username, docObj.contentType,docObj.cloudType,docObj.hashCode);
            }
			//添加正在上传的文件
			this.showLoadingItemByLoadingcache();
        },
        onMoreFileLoading: function(){
			if (this.moreFileLoading != null) {
                this.moreFileLoading.show();
            }
        },
        onMoreFileLoadEnd: function(){
            if (this.moreFileLoading != null) {
                this.moreFileLoading.hide();
            }
        },
        addFileItem: function(fileName, fileSize, timestamp, commentCount, fileUrl, fileId, username, type,cloudType,hashCode) {
            var downloadDomStr = '<div class="download" file-id="' + fileId + '" data-url="' + fileUrl + '">'+uc_resource.Chat.Download+'</div>';
            if(cloudType==1 && hashCode!=""){
                fileUrl=hashCode;
            }
            var localUrl = uc.modules.chat.ChatManager.getLoaclUrl(fileUrl);
            if (localUrl != null && localUrl != ""){
                downloadDomStr = '<div class="download" file-id="' + fileId + '" local-path="' + localUrl + '"  data-url="' + fileUrl + '">'+uc_resource.Chat.Open+'</div>';
            }
            var marginItem = this.isLastFileType ? '<div class="file-margin"/>' : '';
            var ext = uc.util.StringUtil.getFileDisplayExt(fileName);
            var iconSrc =  uc.util.StringUtil.getFileIcon(fileName);
            if (cloudType==0  && type == 1) {//代码片段
                iconSrc = "../images/sfileicons/code.png";
                ext = "code";
            }

            var commentStr="";
            if(!this.isMeeting){
                if (commentCount <= 0) {
                    commentStr = '<div class="comment nocomment"></div>';
                } else {
                    var str = commentCount > 99 ? "99+" : "" + commentCount;
                    commentStr = '<div class="comment">' + str + '</div>'
                }
            }
            var domStr =
                '<li class="file-item" id="file' + fileId + '">' + marginItem
                    + '<div class="file-info" id="' + (fileId + hashCode) + '">'
                    + '<img class="icon" src="' + iconSrc + '">'
                    + '<span class="file-ext">' + ext + '</span>'
                    + '</img>'
                    + '<div class="detail">'
                    + '<div class="name" title="' + fileName + '">' + fileName + '</div>'
                    + '<div class="time">' + uc.util.TimeUtil.getMessageTimeString(new Date(timestamp), uc.constants.UI.TimeFormat.Message.HourMinutes, uc.settings.meeting.timeZone.offset) + '</div>'
                    + '<div class="size">' + uc.util.StringUtil.formatFileSize(fileSize) + '</div>'
                    + '<div class="creator">' + username + '</div>'
                    + commentStr
                    + '</div>'
                    + '<div class="operation" style="display: none;">'
                    + '<progress class="progress" style="display:none"  value="0" max="100"/>'
                    + downloadDomStr
                    + '<div class="more" file-id="' + fileId + '" commnet-count="' + commentCount + '">•••</div>'
                    + '</div>'
                    + '</div>'
                    + '</li>';
            this.node.append(domStr);
            this.isLastFileType = true;
        },
        addRecordItem: function(docObj) {
            var downloadDomStr = '<span class="view-record" href="'+ docObj.videoURL +'" target="_blank">'+uc_resource.Chat.Open+'</span>';
            var marginItem = this.isLastFileType ? '<div class="file-margin"/>' : '';
            var ext = uc.util.StringUtil.getFileDisplayExt(docObj.fileName);
            var title = uc.util.StringUtil.formatString(uc_resource.Message.MeetingVideoWithMeetingName, [docObj.operatorName]);
            var time = parseInt(docObj.updateTime.toString().substring(0, 10)) * 1000;
            var domStr =
                '<li class="file-item" id="file' + docObj.minutesId + '">' + marginItem
                + '<div class="file-info" id="' + docObj.minutesId + '">'
                    + '<img class="icon" src="'+ docObj.videoThumbURL +'">'
                    + '<span class="file-ext">' + ext + '</span>'
                    + '</img>'
                    + '<div class="detail">'
                        + '<div class="name" title="' + docObj.operatorName + '">' + title + '</div>'
                        + '<div class="time">' + uc.util.TimeUtil.getMessageTimeString(new Date(time), uc.constants.UI.TimeFormat.Message.HourMinutes, uc.settings.meeting.timeZone.offset) + '</div>'
                        + '<div class="size">' + uc.util.StringUtil.formatFileSize(docObj.size) + '</div>'
                    + '</div>'
                    + '<div class="operation" style="display: none;">'
                    +  downloadDomStr
                            + '<div class="record-more" file-id="' + docObj.minutesId + '">•••</div>'
                    + '</div>'
                + '</div>'
                + '</li>';
            this.node.append(domStr);
            this.isLastFileType = true;
        },
        addMinutesItem: function(docObj, eventId) {
            var marginItem = this.isLastFileType ? '<div class="file-margin"/>' : '';
            var ext = uc.util.StringUtil.getFileDisplayExt(docObj.title);
            var downloadDomStr = '<div class="view-minutes" file-id="' + docObj.minutesId + '" event-id="'+ eventId +'" >'+uc_resource.MessageBox.view+'</div>';
            var domStr =
                '<li class="file-item" id="file' + docObj.minutesId + '">' + marginItem
                + '<div class="file-info" id="' + (docObj.minutesId) + '">'
                + '<img class="icon" src="../images/sfileicons/txt.png">'
                + '<span class="file-ext">' + ext + '</span>'
                + '</img>'
                + '<div class="detail">'
                + '<div class="name" title="' + docObj.title + '">' + docObj.title + '</div>'
                + '<div class="time">' + uc.util.TimeUtil.getMessageTimeString(new Date(docObj.updateTime * 1000), uc.constants.UI.TimeFormat.Message.HourMinutes, uc.settings.meeting.timeZone.offset) + '</div>'
                + '<div class="size hide">' + uc.util.StringUtil.formatFileSize(docObj.fileSize) + '</div>'
                + '</div>'
                + '<div class="operation" style="display: none;">'
                +  downloadDomStr
                + '&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'
                + '</div>'
                + '</div>'
                + '</li>';
            this.node.append(domStr);
            this.isLastFileType = true;
        },
        addDateItem: function (timestamp) {
            var dateClass = "";
            var dateStr = "";
            var date = new Date(timestamp);
            var today = new Date();
            var yesterday = new Date();
            yesterday.setDate(today.getDate() - 1);
            if (date.toDateString() === today.toDateString()) {
                dateClass = "date today";
                dateStr = uc_resource.Chat.Today;
            } else if (date.toDateString() === yesterday.toDateString()) {
                dateClass = "date yesterday";
                dateStr = uc_resource.Chat.Yesterday;
            } else {
                dateClass = "date";
                if (date.getYear() == today.getYear()) {
                    var month = (date.getMonth() + 1) < 10 ? '0' + (date.getMonth() + 1) : date.getMonth() + 1;
                    var day = date.getDate() < 10 ? '0' + date.getDate() : date.getDate();
                    dateStr = month + "/" + day;
                } else {
                    dateStr = date.toISOString().slice(0, 10);
                }
            }
            var domStr =
                '<li class="file-item">'
                    + '<div class="date-info">'
                    + '<div class="date-line"/>'
                    + '<div class="' + dateClass + '">' + dateStr + '</div>'
                    + '</div>'
                    + '</li>';
            this.node.append(domStr);
            this.isLastFileType = false;
        },

        showFileComment: function(fileInfo){
            uc.util.StorageUtil.setData('currentChatType',this.chatType);
            uc.modules.comment.CommentManager.initChatListComment(fileInfo,this.groupId);
		},
		openUploadWindow: function () {
            $('.meetingPanel-fileList .menu').hide();
			var This = this;
			setTimeout(function () {
                uc.util.FileDialogUtil.openSelectFileDialog(uc_resource.Chat.OpenFile, [
                    {desc: uc_resource.Chat.OpenFileDesc, filter: '*.*'}
                ], true, function (paths) {
                    if (paths && paths.length > 0) {
                        $.each(paths, function (index, path) {
                            This.sendFile(path, This.meetingOption);
                        });
                    }
                });
            }, 200)
		},
		sendFile: function (filePath, MeetingOptions) {
			var This = this;
			var chatInfo = {};
			chatInfo.userId = MeetingOptions.userId,
			chatInfo.siteId = MeetingOptions.siteId,
			chatInfo.resId = MeetingOptions.resId
			chatInfo.chatType = MeetingOptions.chatType;
			chatInfo.eventId = MeetingOptions.eventId;
			chatInfo.startTime = MeetingOptions.startTime;

			uc.modules.chat.MessageManager.uploadFile(filePath, false, chatInfo, function (args) {
				var fileMessageData = {
					errorCode: args.errorCode,
					taskId: args.taskId,
					chatType: args.chatType,
					chatObjectId: args.chatObjectId,
					messageType: uc.constants.Message.ContentType.File,
					filePath: filePath,
					fileName: _getFileName(filePath),
					fileSize: _getFileSize(filePath)
				};
				//缓存重新上传路径
				var key = args.taskId + "";
				if (!This.reUploadItemMap)
					This.reUploadItemMap = new uc.util.HashMap();
				//reload 对应key没有数据，表明不是reload，需要新增item
				if (!This.reUploadItemMap.isExist(key)) {
					//在文件列表list之前插入item
					This.addLoadingFileItem(fileMessageData);
				}
				This.reUploadItemMap.put(key, filePath);


				if (args.errorCode != uc.ErrorCode.Success) {
					//showtips
					This.updateLoadErrorStatus(args.taskId, true);
					return false;
				}

				uc.IUIEvent.OnMessageFileUploading(args.chatObjectId, args.chatType, args.taskId, uc.constants.Message.ContentType.File, chatInfo);
			});
		},
		addLoadingFileItem: function (fileMessageData) {
			var This = this;
			var dom = $(this.getLoadingitem(fileMessageData));
			//如果是今天首次上传，显示今天标志
			if (!$(".file-item .today", this.node).length) {
				var timelineStr =
					'<li class="file-item">'
						+ '<div class="date-info">'
						+ '<div class="date-line"/>'
						+ '<div class="date today">' + uc_resource.Chat.Today + '</div>'
						+ '</div>'
						+ '</li>';
				this.node.prepend(dom);
				this.node.prepend(timelineStr);
			} else {
				//今天已经上传文件，在时间线后面插入
				$(".file-item", this.node).first().after(dom);
			}
			this.updateItemMargin();

			//删除no-file，显示底部上传会议资料
			$('.no-files', this.parentNode).hide();
			//this.parentNode.css("bottom", "40px");
			$('.upload-MeetingDocuments').show();
			var _circleLoader = new uc.modules.component.circleLoader({target: $('.circleLoader', dom), taskid: fileMessageData.taskId, callBack: function (taskId) {

			}});
			if (!this.uploadingItemMap) {
				this.uploadingItemMap = new uc.util.HashMap();
			}
			var key = fileMessageData.taskId + "";
			if (this.uploadingItemMap.isExist(key)) {
				this.uploadingItemMap.get(key).circleLoader.clear();
				this.uploadingItemMap.get(key).circleLoader = null;
				this.uploadingItemMap.get(key).fileMessageData = null;
			}
			this.uploadingItemMap.put(key, {circleLoader: _circleLoader, fileMessageData: fileMessageData});
		},
		fileListUploadProgress: function (taskId, percent, errorCode) {
			if (errorCode != uc.ErrorCode.Success) {
				this.updateLoadErrorStatus(taskId, true);
				uc.IFSService.cancelUploadFile(parseInt(taskId));
				return false;
			}
			if (this.uploadingItemMap != null && this.uploadingItemMap.isExist(taskId + "")) {
				var _loader = this.uploadingItemMap.get(taskId + "").circleLoader;
				_loader.OnProgress(percent);
			} else {
                var fileInfo = uc.modules.chat.ChatManager.fileUpLoadInfo[taskId];
                if (fileInfo) {
                    this.addLoadingFileItem(fileInfo.args);
                }
            }
		},
		fileListUploadComplete: function (eventId, operatorId, document, msgId) {
			var key = msgId + "";
			this.clearFileItem(key);
			if (!this.isActivity) return false;

            var user = uc.IContactService.getBasicContactInfo(parseInt(operatorId));
			this.files[document.fileId] = document;
			var fileName, fileSize, timestamp, commentCount, fileUrl, fileId, username, type, cloudType, hashCode;
			fileName = document.fileName;
			fileSize = document.fileSize;
			timestamp = document.uploadTime;
			commentCount = document.commentCount;
			fileUrl = document.fileUrl;
			fileId = document.fileId;
			username = document.username ? document.username : user.displayName;
			type = document.contentType;
			cloudType = document.cloudType;
			hashCode = document.hashCode;
			var _domstr = this.getLoadCompleteItem(fileName, fileSize, timestamp, commentCount, fileUrl, fileId, username, type, cloudType, hashCode);
			//增加新的节点;
			var tempNode = $("#" + key, this.node);
			tempNode.find('*').off();
			tempNode.empty();
			tempNode.attr("id", "file" + fileId);//.html(_domstr);
			$("#file" + fileId, this.node).html(_domstr);
			this.updateItemMargin();

		},
        fileListUploadMessageReceived: function (eventId,conversationType, channel, sendState, from, to, operatorId, fileId, fileName, fileSize, downloadUrl, fileFormat, desc, msgId, msgSeq, timestamp) {
            var key = fileId + "";

            if (this.files && this.files[fileId]){
                this.files[fileId].creatorId = operatorId;
                this.files[fileId].fileSize = fileSize;
                this.files[fileId].fileUrl = downloadUrl;
                this.files[fileId].fileName = fileName;
                this.files[fileId].type = uc.constants.Message.FileListType.Doc;
            }
			//更新节点;
			var tempNode = $("#file" + key, this.node);
            tempNode.find('.time').text(uc.util.TimeUtil.getMessageTimeString(new Date(timestamp), uc.constants.UI.TimeFormat.Message.HourMinutes, uc.settings.meeting.timeZone.offset));
            tempNode.find('.size').text(uc.util.StringUtil.formatFileSize(fileSize));
            this.updateItemMargin();
		},
		//fs上传，需要taskid作为key，上传文成，发送上传成功消息后，需要以msgid为key，此处做一个转换
		setUploadMsgId: function (taskId, msgId) {
			$("#file" + taskId, this.node).attr("id", msgId);
			var key = taskId + "";
			if (this.uploadingItemMap && this.uploadingItemMap.isExist(key)) {
				this.uploadingItemMap.put(msgId + "", this.uploadingItemMap.get(key));
				this.uploadingItemMap.remove(key);
			}
		},
		clear: function () {
			/*
			 if (this.uploadingItemMap != null) {
			 this.uploadingItemMap.clear();
			 this.uploadingItemMap = null;
			 }*/
			this.isActivity = false;
			this.isLoadedItemByLoadingcache = false;
			this.files = null;
			if (this.reUploadItemMap != null) {
				this.reUploadItemMap.clear();
				this.reUploadItemMap = null;
			}
		},
		getLoadingitem: function (fileMessageData) {
			var marginItem = '<div class="file-margin"/>';
			var fileName = fileMessageData.fileName;
			var fileSize = fileMessageData.fileSize;
			var ext = uc.util.StringUtil.getFileDisplayExt(fileName);
			var iconSrc = uc.util.StringUtil.getFileIcon(fileName);
			var domStr =
				'<li class="file-item" id="file' + fileMessageData.taskId + '">'
					+ '<div class="file-info" >'
					+ '<img class="icon" src="' + iconSrc + '">'
					+ '<span class="file-ext">' + ext + '</span>'
					+ '</img>'
					+ '<div class="loading-mask">'
					+ '<div class="circleLoader loading-right-element"/>'
					+ '<img class="loading-mask-img" src="' + rootPath + '/images/sfileicons/file_mask.png"/>'
					+ '<img class=" load-error-element loading-error hide" src="' + rootPath + '/images/sfileicons/file_loading_error.png"/>'
					+ '<span class=" load-error-element loading-tip hide">' + uc_resource.Chat.FailUploadFile + '</span>'
					+ '</div>'
					+ '<div class="detail">'
					+ '<div class="name" title="' + fileName + '">' + fileName + '</div>'
					+ '<div class="size">' + uc.util.StringUtil.formatFileSize(fileSize) + '</div>'
					+ '</div>'
					+ '<div class="loading-operation loading-right-element">'
					+ '<a href="javascript:void(0)"  taskId="' + fileMessageData.taskId + '" class="cancel icon-close" action="cancel" qtips="' + uc_resource.Chat.FailUploadCancel + '"></a>'
					+ '</div>'
					+ '<div class="loading-operation load-error-element hide">'
					+ '<a href="javascript:void(0)" taskId="' + fileMessageData.taskId + '" class="reload icon-refresh" action="reload" qtips="' + uc_resource.Chat.Reloading + '"></a>'
					+ '<a href="javascript:void(0)" taskId="' + fileMessageData.taskId + '" class="delete icon-close" action="delete" qtips="' + uc_resource.Chat.FailUploadDelete + '"></a>'
					+ '</div>'
					+ '</div>'
					+ '</li>';
			return domStr;
		},
		getLoadCompleteItem: function (fileName, fileSize, timestamp, commentCount, fileUrl, fileId, username, type, cloudType, hashCode) {
			var downloadDomStr = '<div class="download" file-id="' + fileId + '" data-url="' + fileUrl + '">下载</div>';
			if (cloudType == 1 && hashCode != "") {
				fileUrl = hashCode;
			}
			var localUrl = uc.modules.chat.ChatManager.getLoaclUrl(fileUrl);
			if (localUrl != null && localUrl != "") {
				downloadDomStr = '<div class="download" file-id="' + fileId + '" local-path="' + localUrl + '"  data-url="' + fileUrl + '">打开</div>';
			}
			var ext = uc.util.StringUtil.getFileDisplayExt(fileName);
			var iconSrc = uc.util.StringUtil.getFileIcon(fileName);
			if (cloudType == 0 && type == 1) {//代码片段
				iconSrc = "../images/sfileicons/code.png";
				ext = "code";
			}

			var commentStr;
			if (commentCount <= 0) {
				commentStr = '<div class="comment nocomment"></div>';
			} else {
				var str = commentCount > 99 ? "99+" : "" + commentCount;
				commentStr = '<div class="comment">' + str + '</div>'
			}

            if(this.isMeeting) {
                commentStr = '';
            }

			var domStr = '<div class="file-info" id="' + (fileId + hashCode) + '">'
				+ '<img class="icon" src="' + iconSrc + '">'
				+ '<span class="file-ext">' + ext + '</span>'
				+ '</img>'
				+ '<div class="detail">'
				+ '<div class="name" title="' + fileName + '">' + fileName + '</div>'
				+ '<div class="time">' + uc.util.TimeUtil.getMessageTimeString(new Date(timestamp), uc.constants.UI.TimeFormat.Message.HourMinutes, uc.settings.meeting.timeZone.offset) + '</div>'
				+ '<div class="size">' + uc.util.StringUtil.formatFileSize(fileSize) + '</div>'
				+ '<div class="creator">' + username + '</div>'
				+ commentStr
				+ '</div>'
				+ '<div class="operation" style="display: none;">'
				+ '<progress class="progress" style="display:none"  value="0" max="100"/>'
				+ downloadDomStr
				+ '<div class="more" file-id="' + fileId + '" commnet-count="' + commentCount + '">•••</div>'
				+ '</div>'
				+ '</div>';
			this.isLastFileType = true;
			return domStr;
		},
		updateItemMargin: function () {
			var marginItem = '<div class="file-margin"/>';
			var item;
			var fileItemList = $(".file-item", this.node);
			//文件列表中只有事件时间线显示，没有文件
			if (fileItemList.length == 1) {
				fileItemList.remove();
				//显示no-file
				$('.no-files', this.parentNode).show();
				$('.upload-MeetingDocuments').hide();
				this.parentNode.css("bottom", 0);
				return;
			}
			$.each(fileItemList, function (index, item) {
				if (index >= 1) {
					if ($(item).find(".file-margin").length > 0) {
						if (index == 1) {
							$(item).find(".file-margin").remove();
						}
					} else {
						if (index == 1) {
							return true;
						}
						$(item).find(".file-info").before(marginItem);
					}
				}
			});
		},

		showLoadingItemByLoadingcache: function () {
			if (this.uploadingItemMap != null && !this.isLoadedItemByLoadingcache) {
				//循环显示所有正在上传中的item
				var szkey = this.uploadingItemMap.key;
				for (var i = 0; i < szkey.length; i++) {
					this.addLoadingFileItem(this.uploadingItemMap.get(szkey[i]).fileMessageData);
				}
				this.isLoadedItemByLoadingcache = true;
			}
		},
        clearUploadingItemMap: function (key) {
            if (key != null && this.uploadingItemMap != null) {
                var key = key + '';
                if (this.uploadingItemMap.isExist(key)) {
                    if (this.uploadingItemMap.get(key).circleLoader) {
                        this.uploadingItemMap.get(key).circleLoader.clear();
                        this.uploadingItemMap.get(key).circleLoader = null;
                        this.uploadingItemMap.get(key).fileMessageData = null;
                    }
                    this.uploadingItemMap.remove(key);
                }
                if (this.uploadingItemMap.isEmpty()) {
                    this.uploadingItemMap = null;
                }
            } else {
                if (this.uploadingItemMap) {
                    this.uploadingItemMap = null;
                }
            }
        },
		deleteFileItemByTaskId: function (taskId) {
			var key = taskId + "";
			this.clearFileItem(taskId);
			var tempNode = $("#file" + key, this.node);
			tempNode.find('*').off();
			tempNode.remove();
			this.updateItemMargin();
		},
        checkFileItem: function (target) {
            var parentNode = target.parents('.file-item');
            var preElement = parentNode.prev('.file-item[id]');
            var nextElement = parentNode.next('.file-item[id]');
            if (!preElement.length && !nextElement.length) {
                parentNode.prev('.file-item').remove();
            }
        },
		clearFileItem: function (key) {
			if (key != null && this.uploadingItemMap != null) {
				if (this.uploadingItemMap.isExist(key)) {
					this.uploadingItemMap.get(key).circleLoader.clear();
					this.uploadingItemMap.get(key).circleLoader = null;
					this.uploadingItemMap.get(key).fileMessageData = null;
					this.uploadingItemMap.remove(key);
				}
				if (this.uploadingItemMap.isEmpty()) {
					this.uploadingItemMap.clear();
					this.uploadingItemMap = null;
				}
			} else {
				if (this.uploadingItemMap) {
					this.uploadingItemMap.clear();
					this.uploadingItemMap = null;
				}
			}
		},
		updateLoadErrorStatus: function (taskId, isError) {
			if (!this.isMeeting) return;
			var _node = $('#file' + taskId, this.itemList);
			if (isError) {
				_node.find(".load-error-element").removeClass('hide');
				_node.find(".load-error-element").show();
				//隐藏正常loading
				_node.find(".loading-right-element").hide();
			} else {
				_node.find(".load-error-element").hide();
				_node.find(".loading-right-element").show();
			}
		},
        OnMinutesListReceived: function (errorCode, eventId, meetingMinutes) {
            if (meetingMinutes.length <= 0 && $(".file-item", this.node).length <= 0){
                if (errorCode == 0){
                    //有缓存正loading的文件，不显示no-files
                    if (this.uploadingItemMap == null || this.uploadingItemMap.isEmpty())
                        $('.no-files', this.parentNode).show();
                    this.recvFileEnd = true;
                } else {
                    $('.load-fail', this.parentNode).show();
                }
                $('.upload-MeetingDocuments').hide();
                //此处设置bottom为0 是和会议资料上传显示有关（此状态下css设置bottom为40px）
                this.parentNode.css("bottom", 0);
            } else {
                if (this.isMeeting) {
                    //this.parentNode.css("bottom", "40px");
                    //$('.upload-MeetingDocuments').show();
                }
            }
            this.onMoreFileLoadEnd();
            $('.bottom-margin', this.parentNode).show();

            for (var i = 0; i < meetingMinutes.length; i++) {
                var docObj = meetingMinutes[i];
                this.files[docObj.minutesId] = docObj;
                var date = new Date(docObj.updateTime * 1000).toISOString().slice(0,10);
                if (this.lastDate != date) {
                    this.lastDate = date
                    this.addDateItem(docObj.updateTime * 1000);
                }
                if (docObj.type == uc.constants.Message.FileListType.Minutes) {
                    this.addMinutesItem(docObj, eventId);
                } else if (docObj.type == uc.constants.Message.FileListType.Record) {
                    this.addRecordItem(docObj);
                } else if (docObj.type == uc.constants.Message.FileListType.Doc) {
                    this.addFileItem(docObj.fileName, docObj.fileSize, docObj.updateTime * 1000,
                        docObj.commentCount, docObj.fileUrl, docObj.minutesId, docObj.username, docObj.contentType,docObj.cloudType,docObj.hashCode);
                }
            }
            //添加正在上传的文件
            this.showLoadingItemByLoadingcache();
        }
    })
})(uc);