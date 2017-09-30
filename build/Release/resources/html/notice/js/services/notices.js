appModule.service('notices',['utils','native','log','$translate',function(utils,native,log,translate){
    var TEMP_NOTICE_KEY="temp_notice";
    var LOCAL_STORAGE_KEY = "saved_notices_";
    var SELECTED_ACCOUNT = "selected_account";

    return{
        saveTempNotice:function(n) {//缓存当前编辑草稿
            utils.saveObjTolocalStorage(TEMP_NOTICE_KEY,n);
        },
        getTempNotice:function() {//缓存当前编辑草稿
            return utils.getObjFromlocalStorage(TEMP_NOTICE_KEY);
        },
        deleteTempNotice:function() {
            utils.removeObjFromlocalStorage(TEMP_NOTICE_KEY);
        },
        saveNoticeDraft:function(notice) {//保存广播草稿
            var noticeDrafts = this.getNoticeDrafts();
            var isDuplicated = false;
            var currentId = notice.localId? notice.localId : Date.now();//获取草稿local id 如果没有local id 则将当前时间的毫秒值作为local id
            for (var i = 0; i < noticeDrafts.length; i++) {
                if (noticeDrafts[i].localId == notice.localId) {
                    noticeDrafts[i] = notice;
                    isDuplicated = true;
                    break;
                }
            }
            if (!isDuplicated) {
                notice.localId = currentId;
                noticeDrafts.unshift(notice);
            }
            var currentUser = utils.retrieveUser();
            var draftString = JSON.stringify(noticeDrafts);
            log.logInfo("save notice draft："+ notice.topic);
            native.saveLocalStore(LOCAL_STORAGE_KEY + currentUser.userId, draftString);
        },
        removeNoticeDraft:function(notice) {//删除广播草稿
            var noticeDrafts = this.getNoticeDrafts();
            var index = -1;
            for (var i = 0; i < noticeDrafts.length; i++) {
                if(!!notice.localId && notice.localId == noticeDrafts[i].localId){
                    index = i;
                    break;
                }
            }
            if (index > -1) {
                noticeDrafts.splice(index, 1);
                log.logInfo("removed notice draft："+JSON.stringify(notice));
            }
            var currentUser = utils.retrieveUser();
            native.saveLocalStore(LOCAL_STORAGE_KEY + currentUser.userId, JSON.stringify(noticeDrafts));
        },
        getNoticeDrafts:function() {//获取广播草稿列表
            var currentUser = utils.retrieveUser();
            var noticeDrafts = native.getLocalStore(LOCAL_STORAGE_KEY+currentUser.userId);
            if (!noticeDrafts) {
                return [];
            } else {
                return JSON.parse(noticeDrafts);
            }
        },
        getDrafts: function(userId, accountId) {
            var savedNotices = this.getNoticeDrafts();
            var notices = [];
            if (savedNotices.length > 0) {
                for (var i = 0; i < savedNotices.length; i++) {
                    if (savedNotices[i].creatorId == userId && savedNotices[i].accountId == accountId) {
                        notices.push(savedNotices[i]);
                    }
                }
            }
            return notices;
        },
        saveDraft: function(notice,callback) {
            if(!notice.author){
                var userInfo = native.getUserInfo();
                if(notice.creatorId == userInfo.userId){
                    notice.author = {
                        id: notice.creatorId,
                        name:userInfo.displayName
                    }
                }               
            }
            if(!notice.createTimestamp ){
                notice.createTimestamp = Date.now();
            }
            this.saveNoticeDraft(notice);
            if(callback){
                callback();
            }
        },
        retrieveLocalData:function(userId){
            log.logInfo("start get drafts from local db...");
            var key = LOCAL_STORAGE_KEY + userId;
            if(!localStorage[key]){
                var drafts = native.retrieveLocalData(key);
                localStorage[key] = drafts;
                var draftsArray=[];
                if(!!drafts && drafts.length>0){
                    draftsArray = JSON.parse(drafts);
                }
                log.logInfo("end get drafts from local db..."+ draftsArray.length);
            }
        },
        getDataFromLocalDB:function(key){
            return native.retrieveLocalData(key);
        },
        getCropperPath:function(){
            return native.getCropperPath();
        },
        getOrgInfo:function(param,callback){
            var id = param *1;
            
            native.getOrgInfo(id,callback);
        },
        notice:{
            create:function(){
                var nObj = {
                    id: "",
                    accountId: "",
                    sections: [],
                    topic: "",
                    cover: "",
                    content: "",
                    createTime: "",
                    creatorId: null,
                    placeHolder: "请输入正文...",
                    summary:""
                };

                nObj.isContentEmpty = function() {
                    if (this.content === "" || this.content == this.placeHolder) {
                        return true;
                    }

                    return false;
                };

                return nObj;
            },
            convertSectionsToPublishScope:function(sections){
                var publishScope = {};
                if (!sections || sections.length < 1) {
                    return publishScope;
                }
                publishScope.group_ids = [];
                publishScope.user_ids = [];
                $.each(sections, function(index, section) {
                    if((section.id+'').startsWith('user')){
                        publishScope.user_ids.push(section.id.replace('user',''));
                    }else{
                        publishScope.group_ids.push(section.id);
                    }
                });

                return publishScope;
            },
            convertGroupInfoToSections:function(data){
                var sections = [];
                var publishScope = data.publish_scope;
                if(!publishScope){
                    return sections;
                }
                if(!!publishScope.group_info && publishScope.group_info.length > 0){
                    $.each(publishScope.group_info,function(index,info){
                        sections.push({
                            id:info.org_id,
                            text:info.org_name
                        });
                    });
                }
                if(!!publishScope.user_info && publishScope.user_info.length > 0){
                    $.each(publishScope.user_info,function(index,info){
                        sections.push({
                            id:info.user_id,
                            text:info.display_name
                        });
                    });
                }

                return sections;
            },
            convertFromData:function(n,data) {
                n.id = data.id;
                n.accountId = data.board_id;
                n.sections = this.convertGroupInfoToSections(data);
                n.topic = data.title;
                if(!data.attachments){
                    data.attachments = [{
                        url: '',
                        fid: '',
                        size: '',
                        embeded:false
                    }];
                }
                n.cover = data.attachments[0];
                n.isEmbedCover = !!data.attachments[0] && data.attachments[0].embeded;
                n.content = data.content;
                n.createTime = utils.time2str(data.created_at/1000);
                n.createTimestamp = data.created_at;
                n.creatorId = data.user_id;
                if(data.read_status !== undefined){
                    n.readStatus = data.read_status;
                }
                n.readCount = data.read_count;
                n.unreadCount =data.unread_count;
                n.summary = data.description;
                if(data.user_info){
                    n.author = {
                        id:data.user_info.user_id,
                        name:data.user_info.display_name
                    }
                }
            },
            convertFromNewData:function(n,data){
                var customizedData = JSON.parse(data.customizedData);
                data.elements = JSON.parse(data.elements);
                var status = data.status;
                var colors = {
                    "red": "#f96868",
                    "yellow": "#e5c106",
                    "green": "#4bd963",
                    "gray": "#c3c3c3",
                    "blue": "#4e667a",
                    "lightblue": "#6fafe2",
                    "white":"#fff"
                };
                var titleBkColor,title;
                if(data.titleElements){
                    var titleElement = JSON.parse(data.titleElements);
                    for(var i in titleElement){
                        if (titleElement[i].status == status) {
                            titleBkColor = titleElement[i].color;
                            title = titleElement[i].title || customizedData.title;
                        }
                    }
                }else{
                    titleBkColor = data.color;
                }

                titleBkColor=colors[titleBkColor]?colors[titleBkColor]:colors.blue;
                if(data.titleStyle == "simple"){
                    n.titleBkColor = {
                        'background': '-webkit-gradient(linear,0 20%,0 21%,from(' + titleBkColor + '),to(#ffffff))'
                    };
                }else if(data.titleStyle = 'default'){
                    n.titleBkColor ={
                        'background':titleBkColor
                    };
                }
                var textContentStyle={};
                n.id = customizedData.bbs_id;
                n.accountId = customizedData.board_id;
                n.accountName = customizedData.board_name;
                n.accountAvatar = customizedData.avatar;
                n.topic = customizedData.title;
                n.createTime = utils.time2str(customizedData.created_at/1000);
                n.createTimestamp = customizedData.created_at;
                n.readStatus = customizedData.read_status;
                n.sections = this.convertGroupInfoToSections(customizedData);
                n.detailUrl = data.detailURL;
                if(customizedData.user_info){
                    n.author = {
                        id:customizedData.user_info.user_id,
                        name:customizedData.user_info.display_name
                    }
                }

                for (var i = 0; i < data.elements.length; i++) {
                    var el = data.elements[i];
                    if (el === null || (el.status !== null && status !== el.status)) {
                        continue;
                    }
                    if (el.type == "text") {
                        if(typeof el.content === "string"){
                            n.summary = el.content;
                        }else if(Array.isArray(el.content)){
                            //todo
                        }else if(typeof el.conent === "object"){
                            var content = el.content;
                            if(content.size == 1){
                                textContentStyle["font-size"] = "12px";
                            }else if(content.size ==3){
                                textContentStyle["font-size"] = "32px";
                                textContentStyle.padding = "10px 10px";
                            }

                            if(content.bold == 1){
                                textContentStyle["font-weight"] = "bold";
                            }
                            if (content.italic == 1) {
                                textContentStyle["font-style"] = "italic";
                            }
                            if (content.color !== null && content.color !== "") {
                                textContentStyle.color = content.color;
                            }

                            n.summary = el.content.text?el.content.text:"";
                        }
                    } else if (el.type == "image" && el.imageId) {
                        n.cover = {
                            url:el.imageId
                        };
                    } else if (el.type == "action") {
                        //todo
                    }
                }

                n.textStyle=textContentStyle;
            },
            convertToData:function(n) {
                var data = {
                    id: n.id,
                    board_id: n.accountId,
                    attachments: [{
                        url: n.cover.remoteUrl?n.cover.remoteUrl:(n.cover.url?n.cover.url:""),
                        fid: n.cover.fid?n.cover.fid:"",
                        size: n.cover.size?n.cover.size:0,
                        embeded: n.isEmbedCover?1:0
                    }],
                    content: n.content,
                    title: n.topic,
                    object_type: 'bbs',
                    publish_scope: this.convertSectionsToPublishScope(n.sections),
                    description:n.summary
                };
                if(!data.attachments[0].url){
                    data.attachments = [];
                }
                if(n.type){
                    data.type = n.type;
                    data.publish_scope = this.convertSectionsToPublishScope([n.previewer]);
                }
                return data;
            }
        },
        account:function(){
            var inner = {
                id: "",
                name: "",
                creatorId: null,
                createTime: null,
                modifiedTime: null,
                status:0,
                avatar:"",
                sectionIDs: []
            };

            inner.convertFromData = function(data) {
                this.id = data.id?data.id:data.board_id;
                this.name = data.board_name;
                this.preName = this.name;
                this.creatorId = data.user_id;
                this.createTime = utils.time2str(data.created_at);
                this.modifiedTime = utils.time2str(data.modified_at);
                this.avatar = data.avatar;
                this.status = data.status;
                var sectionIDs = [];
                if(data.group_info){
                    $.each(data.group_info, function(i, info) {
                        sectionIDs.push({
                            "id": info.org_id,
                            "text": info.org_name
                        });
                    });

                }
                this.sectionIDs = sectionIDs;
                var editors = [];
                if(data.editor_info){
                    $.each(data.editor_info,function(i,info){
                        editors.push({
                            "id":info.user_id,
                            "name":info.display_name
                        });
                    });
                }
                this.editors = editors;
            };

            return inner;
        },
        saveSelectedAccount:function(account){
            utils.saveObjTolocalStorage(SELECTED_ACCOUNT, account);
        },
        retrieveSelectedAccount:function(){
            return utils.getObjFromlocalStorage(SELECTED_ACCOUNT);
        },
        openAvatarWindow:function(callback){
            native.openAvatarChangesWindow("ChangeAccountAvatar",callback);
        },
        openCloudWindow:function(){
            native.triggerGlobalEvent("openCloudWindow");
        },
        onOpenCloudWindow:function(){
            log.logInfo("receive open cloud window event.");
            native.openCloudDialog({
                chatType: native.noticeChatType(),
                chatObjectId: "",
                siteId: null,
                resId: null,
                moduleId: null
            });
        },
        addCloudFileSelectedEvent:function(cb){
            native.cloudFileEvents.sendFromMount=function(eventData){
                var data = JSON.parse(eventData.detail.data.data),chatInfo=JSON.parse(eventData.detail.data.chatInfo);
                log.logInfo("add cloud file:"+eventData.detail.data.data);
                if(chatInfo.options.chatType === native.noticeChatType() && data){
                    data =data.list[0];
                    if(data.check_permission){
                        delete data.check_permission;
                    }
                    if(eventData.detail.data.fileIcon&&eventData.detail.data.fileIcon.length>0){
                        data.fileIcon = eventData.detail.data.fileIcon[0];
                    }
                    cb.success(data);
                }
            };
            native.cloudFileEvents.cancelSelectCloudFile = function(){
                cb.cancel();
            }
        },
        downloadCloudFile:function(url,cb){
            var data = url.substr(url.lastIndexOf("data=")+5);
            native.raiseCloudFileDownloadEvent(data);
            native.cloudFileEvents.noticeAlert = function(eventData){
                if(!!cb){
                    cb();
                }
            };
        },
        showReceiverNoticePreview:function(n){
            native.showPreview(n.accountName,n.detailUrl);
        },
        modifyImageURL:function(url,width){
            var appendString = "cov=1&tnw="+width+"&covtype=0";
            if(url.match(/ucfserver\/hddown\?[^?]+$/gi)){
                return url+"&"+appendString;
            }

            return url;
        },
        convertNoticeTime:function(source){
            if(!source){
                return "";
            }
            var curDate = new Date();
            var year,month,date,time,day;
            source = utils.time2str(source/1000);
            var parts = source.split(" ");
            if(parts.length <2){
                return source;
            }

            day = parts[0];
            time = parts[1];
            parts = day.split("/");
            if(parts.length <2){
                return source;
            }
            if(parts.length ==3){
                year = parseInt(parts[0]);
                month = parseInt(parts[1]);
                date = parseInt(parts[2]);
            }else{
                year = curDate.getFullYear();
                month = parseInt(parts[0]);
                date = parseInt(parts[1]);
            }

            if(year == curDate.getFullYear() && month == curDate.getMonth()+1 && date == curDate.getDate() - 1){
                return translate.instant("common.yesterday")+" "+time;
            }

            return source;
        },
        getConversationTitle:function(){
            return native.getConversationTitle();
        },
        minusNoticeUnreadCount:function(count){
            var curCount = native.getNoticeUnreadCount();
            curCount = curCount>count?curCount-count:0;
            native.setNoticeUnreadCount(curCount);
        },
        openUCAlert:function(msg,cb){
            native.openUCAlert(translate.instant("common.prompt"),msg,translate.instant("common.confirm"),cb);
        },
        getBasicUserInfo:function(userId){
            var info =  native.getBasicUserInfo(userId);
            info.avatar = info.localAvatar?info.localAvatar:info.remoteAvatarUrl;
            return info;
        },
        onLastAccountDeleted:function(){
            native.onDeleteLastAccount();
        }
    };
}]);
