/**
 * Created by xujiansong on 16/3/16.
 */
(function (uc) {
    var rootPath = (function () {
        var href = location.href;
        var index = href.indexOf('/html');
        return href.substring(0, index + 6);
    })();
    $.MatchKeyWord =function (content,key) {
        if(key=="")
        {
          return content;
        }
        var reg = new RegExp(key, "gi");
        var newResult ="";
        var tempResult =content;
        if(content.match(reg)!=null) {
            for (var i = 0; i < content.match(reg).length; i++) {
                var tempKey = content.match(reg)[i];
                var head = tempResult.split(tempKey)[0];
                newResult += head + "<span class='keywordText'>" + tempKey + "</span>";
                tempResult = tempResult.replace(head + tempKey, "");
            }
            content = newResult + tempResult;
        }
        return content;
    }
    uc.modules.historymsg.Page = "rootHistory";
    uc.modules.historymsg.parents = "content-panel-body";
    uc.modules.historymsg.oclick = true;
    uc.modules.historymsg.scope = null;
    uc.modules.historymsg.topHistoryDomStr = "";
    uc.modules.historymsg.lastHistoryDomStr = "";
    uc.modules.historymsg.AtUserHistoryActive = false;
    uc.modules.historymsg.isMeetingChat = false;//when currentHistory  is not MeetingChat
    uc.modules.historymsg.$$ = function (s) {
        if (uc.modules.historymsg.scope != null)
            return $(s, uc.modules.historymsg.scope);
        else
            return $(uc.modules.historymsg.Page);
    }

    uc.modules.historymsg.HistoryManager = {

        /* create query history window page  */
        /* param ={level:0 }*/
        createHistoryPage: function (createObj, param,extend) {
            var _this = this;
            this.rootBody = $(createObj).parents("." + uc.modules.historymsg.parents);
            var initWidth = this.rootBody.width();
            var parentLay = "Hparent";
            if (param == undefined)
                param = {"level": 0};
            if(extend==undefined)
                extend =false;
            var searchTilte = "init_headtitle";
            var bglay = "<div class='" + parentLay + "'></div>";
            if (param.level == 0) {
                this.historyLay = true;
                this.currentChatId = uc.modules.chat.ChatManager.currentChatId;
                this.currentChatType = uc.modules.chat.ChatManager.currentChatType;
                this.meetingChatId = uc.modules.chat.MeetingChat.meetingChatId;
                this.lastMsgSeq = 0;
                this.myUserInfo = uc.IContactService.getCurrentUserInfo();
                this.userId = this.myUserInfo.userId;
                this.rootBody.find("." + uc.modules.historymsg.Page).remove();
            }
            else {
                searchTilte = "headtitle";
                bglay = "";
            }
            var titleHtml = "<div class='" + searchTilte + " titlebar'><a class='back'></a><div class='title'></div></div>";
            var searchHtml = "<div class='searchbar'><span class='history-timepicker'></span><div class='searchbarIO'><input /><span class='delinput' onclick='javascript:$(this).hide();$(this).prevAll(\"input\").val(\"\")'></span><span class='btn'>搜索</span></div></div>";
            var pageHtml = bglay + "<div id='level" + param.level + "'  class='" + uc.modules.historymsg.Page + "' style ='-webkit-transform:translate(" + initWidth + "px,0px );'>" + titleHtml + "<div class='list messageBox'>" +
                    '<div class="more-msg-line">'
                +        '<span class="more-msg-loading" style="display:none" ></span>'
                +         '<span class="more-msg" style="display:none" >'+ uc_resource.Message.ViewMore +'</span>'
                +   '</div>'
                + "<div class='history-list'><div class='rows'></div></div></div>" + searchHtml + "</div>";
            this.rootBody.append(pageHtml);
            if(extend)
            {
                this.PageStatus.extend(param.level);
            }
            else
            {
                this.PageStatus(param.level);
            }
            var times = 300;
            setTimeout(function () {
                uc.modules.historymsg.scope.css("-webkit-animation-timing-function", "ease-in-out").css("-webkit-transform", "translate(0px ,0px ) scale(1) translateZ(0px)").css("transition-duration", "" + (times / 1000) + "s").css("-webkit-transition-duration", "" + (times / 1000) + "s");
                _this.rootBody.find("." + parentLay).click(function () {
                    $(this).remove();
                    uc.modules.historymsg.scope.remove();
                    _this.historyLay = false;
                    _this.IsStopOtherScroll(false);
                });
                uc.modules.historymsg.$$(".btn").off("click");
                uc.modules.historymsg.$$(".btn").click(function () {
                    if (_this.RequestHistoryListByKeyWord(this, true))  _this.skip(this, false);
                });
                uc.modules.historymsg.$$(".back").click(function () {
                    var closePage = $(this).parents("." + uc.modules.historymsg.Page);
                    closePage.css("-webkit-animation-timing-function", "ease-in-out").css("-webkit-transform", "translate(" + initWidth + "px,0px) scale(1) translateZ(0)").css("transition-duration", "" + (times / 1000) + "s").css("-webkit-transition-duration", "" + (times / 1000) + "s");
                    setTimeout(function () {
                        closePage.remove();
                        var id = closePage.attr("id").replace("level", "");
                        _this.PageResume(id);
                        if (id == 1)
                            uc.modules.historymsg.History.keywordback();//kill this obj
                        else if (id == 2)
                            uc.modules.historymsg.History.nextResult_back();//kill this obj
                    }, times);
                });
                _this.rootBody.find(".del").remove();
                _this.Scope(".searchbarIO input").off("input");
                _this.Scope(".searchbarIO input").on("input",function () {
                    if($(this).val().trim())
                        _this.Scope(".searchbarIO>.delinput").show().css("display","inline");
                    else
                        _this.Scope(".searchbarIO>.delinput").hide();
                });
                if(extend)
                {
                    _this.bindEvent.extend(param.level);
                }
                else
                {
                    _this.bindEvent(param.level);
                }
            }, times);
        },
        Scope:function (select) {
           var obj = this.rootBody.find("div[active='1']");
           if(select!=undefined)
           {
               return  $(select,obj);
           }
            else
           {
               return obj;
           }
        },
        skip: function (btnobj, isLast) {
            setTimeout(function () {
                uc.modules.historymsg.oclick = true;
            }, 500);
            if (!uc.modules.historymsg.oclick)
                return;
            uc.modules.historymsg.oclick = false;
            var opPage = $(btnobj).parents("." + uc.modules.historymsg.Page);
            opPage.removeAttr("stop");
            var param = {"level": 1};
            if (opPage.attr("id") == "level1") {
                if (isLast) {
                    param.level = 2;
                }
                else {
                    // repaly  query keyword
                    this.onMoreMsgLoading();
                    opPage.find(".rows").html(""); opPage.find(".titlebar>.title").text("");
                    return;
                }
            }
            else if (opPage.attr("id") == "level2") {
                if (!isLast) {
                    //remove layer 1 and layer 2 and Create layer1
                    $(btnobj).parents("." + uc.modules.historymsg.parents).find("#level1").attr("id", "del").addClass("del", "true");
                    $(btnobj).parents("." + uc.modules.historymsg.parents).find("#level2").attr("id", "del").addClass("del", "true");
                }
                else {
                    return;
                }
            }
            this.createHistoryPage(opPage, param);
        },
        PageStatus: function (id) {
            var _this = this;
            uc.modules.historymsg.AtUserHistoryActive = false;
            uc.modules.historymsg.isMeetingChat =false;
            uc.modules.historymsg.scope = this.rootBody.find("." + uc.modules.historymsg.Page);
            uc.modules.historymsg.scope.attr("active", "0");
            var cpage = $("#level" + id);
            cpage.attr("active", "1");

            _this.moreMsgLine = $('.more-msg-line',cpage);
            _this.moreMsg = $('.more-msg', cpage);
            _this.moreMsgLoading = $('.more-msg-loading', cpage);
            _this.moreMsg.click(function(){
                if(id==0) {
                    _this.moreMsg.attr("goOn",$(this).offset().top);
                    _this.onMoreMsgLoading();
                    _this.RequestInitHistoryList();
                }
            });

            _this.onMoreMsgLoading();
            // timepicker begin
            _this.rootBody.find(".del").find(".searchbar>.history-timepicker input").attr("id","del");
            var selDateText =cpage.find(".searchbar>.history-timepicker");
            var selDateTextID ='history-picker_'+id;
            var historyPicker = new uc.modules.component.DatePicker(selDateText, selDateTextID, function (date) {
                if(date>=new Date())
                {
                    $('#'+selDateTextID).val(new Date().format('yyyy/mm/dd'));
                    return;
                }
                var beginTime =  1000 * uc.util.TimeUtil.displayTimeToUnixTime(date, parseInt(new Date().getTimezoneOffset()/60)*(-1),uc.constants.UI.TimeStampUnit.Second);
                var param ={ level :2};
                _this.forntOrBack = 1;
                _this.beginTime = beginTime;
                _this.endTime = 0;
                _this.keyWord ="";//cannel keyWord when select timepicker
                _this.createHistoryPage(selDateText,param,false);
            });
            // timepicker End

            if (id == 0) {
                uc.modules.historymsg.History.init();
                uc.modules.historymsg.History.levelHistory = cpage;
                this.RequestInitHistoryList();    //request query history
                cpage.find(".searchbar>.history-timepicker input").removeAttr("disabled").removeClass("onlyread");
            }
            else if (id == 1) {
                uc.modules.historymsg.History.levelKeyWordHistory = cpage;
                uc.modules.historymsg.History.levelKeyWordHistory.fn = function (btn, frontorBack, beginTime, endTime) {
                    _this.forntOrBack = frontorBack;
                    _this.beginTime = beginTime;
                    _this.endTime = endTime;
                    _this.skip(btn, true);
                };//open window
                uc.modules.historymsg.scope.attr("lastMsgSeq", "0");
                var prepV = cpage.prev().find(".searchbarIO input");
                cpage.find(".searchbarIO input").val(prepV.val());
                cpage.find(".searchbarIO>.delinput").show();
                prepV.nextAll(".delinput").click();//clear input
                cpage.find(".searchbar>.history-timepicker input").attr("disabled","disabled").css({background:'#ededed',color:'#666'});//selectDate
            }
            else if (id == 2) {
                uc.modules.historymsg.History.levelNextHistory = cpage;
                uc.modules.historymsg.History.moveUporDown_next = 1;
                _this.requestTwo = _this.beginTime;
                var prepV = cpage.prev().find(".searchbar>.history-timepicker input");
                cpage.find(".searchbar>.history-timepicker input").val(prepV.val());
                _this.RequestHistoryByNexeResult(_this.forntOrBack, _this.beginTime, _this.endTime);//request back Nexthistory
                cpage.find(".searchbar>.history-timepicker input").attr("disabled","disabled").css({background:'#ededed',color:'#666'});//selectDate
            }
        },
        PageResume: function (id) {
            id = parseInt(id) - 1;
            $("#level" + id).attr("active", "1");
        },
        IsStopOtherScroll: function (isStop) {
            /*
            var otherScroll = $("." + uc.modules.historymsg.parents + " .more-msg");
            if (isStop) {
                if (!otherScroll.is(":hidden")) {
                    otherScroll.attr("show", "1").hide();
                }
            }
            else {
                if (otherScroll.attr("show") == "1") {
                    otherScroll.removeAttr("show");
                    otherScroll.show();
                }
            }
            */
            if(isStop)
            {
                uc.modules.chat.ChatManager.chatMsg =false;
            }
            else
            {
                uc.modules.chat.ChatManager.chatMsg =true;
            }
        },
        bindEvent: function (id) {
            var _this = this;
            var cpage = $("#level" + id);
            var scope = this.rootBody.find("div[active='1']");
            _this.lastTop = 0;
            var setSrcoll = function () {
                if (scope != null && scope.get(0) != undefined) {
                    scope.find(".list").scroll(function () {
                        _this.IsStopOtherScroll(true);	//stop other scroll
                        var top = this.scrollTop;
                        if (scope.attr("stop") == 1)
                            return;
                        _this.onMoreMsgLoading();
                        if (id == 1 && top < 200) {
                            if (top != 0) {
                                _this.RequestHistoryListByKeyWord(this, false);//request ();//request
                            }
                        }
                        else if (id == 0 && top < 200)
                            _this.RequestInitHistoryList(); 	//request
                        else if (id == 2) {
                            var move = this.scrollTop - _this.lastTop;
                            var objHeight = this.scrollHeight - $(this).height() - 200;
                            _this.lastTop = top;
                            var frontorBack = 1;
                            var beginTime = 0;
                            var endTime = 0;
                            if (scope.attr("downstop") == 1 && scope.attr("upstop") == 1) {
                                //queryNextResult end
                                return;
                            }
                            if (move < 0) {
                                if (top < 200) {
                                    //scope.find(".title").text("加载上一页");
                                    if (scope.attr("upstop") == 1)
                                        return;
                                    frontorBack = 0;
                                    endTime = uc.modules.historymsg.History.lastMsgSeq_next;
                                    var domMin = scope.find(".row").not("[rel='time']").first().attr("seq");
                                    if (domMin != endTime) endTime = domMin;
                                    beginTime = 0
                                    uc.modules.historymsg.History.moveUporDown_next = 0;
                                    _this.RequestHistoryByNexeResult(frontorBack, beginTime, endTime);// move up request
                                }
                                else
                                    scope.find(".title").text("");

                            }
                            else {
                                if (top > objHeight) {
                                    if (scope.attr("downstop") == 1)
                                        return;
                                    //scope.find(".title").text("加载下一页");
                                    frontorBack = 1;
                                    endTime = 0;
                                    beginTime = uc.modules.historymsg.History.lastMsgSeq_max_next;
                                    var domMax = scope.find(".row").not("[rel='time']").last().attr("seq");
                                    if (domMax != beginTime) beginTime = domMax;
                                    uc.modules.historymsg.History.moveUporDown_next = 1;
                                    _this.RequestHistoryByNexeResult(frontorBack, beginTime, endTime);// move down request
                                }
                                else
                                    scope.find(".title").text("");
                            }
                        }
                    });
                }
            }
            if (id == 0) {
                cpage.find(".titlebar").show();
                cpage.find(".titlebar>.title").text("消息记录");
            }
            else if (id == 1) {
                cpage.find(".titlebar").show();
            }
            else if (id == 2) {
                cpage.find(".titlebar").show();
            }
            setSrcoll();

            $('.searchbarIO>input').on('keydown', function (e) {
                if (e.keyCode == uc.constants.UI.KeyCode.Enter) {
                    scope.find(".btn").click();
                    return false;
                }
            });
            //设置输入法跟随
            uc.util.CaretUtil.setInputEditorCaretPosition($(".searchbarIO>input"));
        },
        onMoreMsgLoading: function(){
            this.moreMsgLoading.show();
            this.moreMsg.hide();
        },
        onMoreMsgLoadEnd: function(){
            //this.moreMsgLoading.hide();
            //this.moreMsg.show();
            this.Scope(".more-msg-loading").hide();
            this.Scope(".more-msg").show();
        },
        onLoadAllMsgEnd: function(){
            //this.moreMsgLine.hide();
            //this.moreMsg.hide();
            this.Scope(".more-msg-line").hide();
            this.Scope(".more-msg").hide();
        },
        RequestInitHistoryList: function () {
            if (this.currentChatType == uc.constants.Chat.ChatType.SingleChat) {
                uc.util.LogUtil.info("uc.modules.historymsg.History", "RequestInitHistoryList", "data=", {
                    groupId: this.userId,
                    count: 10,
                    page: 0,
                    seq: Number(this.lastMsgSeq)
                });
                uc.IIMService.getChatHistory(uc.constants.Message.MessageChannel.History, this.currentChatId, uc.constants.Message.OnceGetHistoryCount, 0, Number(this.lastMsgSeq), function (errorCode) {
                    if (errorCode != 0) {
                        uc.util.LogUtil.info("SingleChat", "getMoreHistoryMsg", "data=", {
                            groupId: this.currentChatId,
                            errorCode: errorCode
                        });
                    }
                });
            }
            else if(this.currentChatType == uc.constants.Chat.ChatType.GroupChat||this.currentChatType == uc.constants.Chat.ChatType.ProjectChat||this.currentChatType == uc.constants.Chat.ChatType.CloudChat) {
                uc.util.LogUtil.info("uc.modules.historymsg.History", "RequestInitHistoryList", "data=", {
                    chatType: this.chatType,
                    groupId: this.groupId,
                    count: 10,
                    page: 0,
                    seq: Number(this.lastMsgSeq)
                });
                uc.IGroupService.getChatHistory(uc.constants.Message.MessageChannel.History, this.currentChatType, this.currentChatId, uc.constants.Message.OnceGetHistoryCount, 0, Number(this.lastMsgSeq), function (errorCode) {
                    if (errorCode != 0) {
                        uc.util.LogUtil.info("History", "RequestInitHistoryList", "data=", {
                            groupId: this.currentChatId,
                            errorCode: errorCode
                        });
                        //msgBox.onMoreMsgLoadEnd();
                    }
                });
            }
            else if(this.currentChatType == uc.constants.Chat.ChatType.MeetingChat)
            {
                uc.IGroupService.getChatHistory(uc.constants.Message.MessageChannel.History, this.currentChatType,this.currentChatId, uc.constants.Message.OnceGetHistoryCount, 0, Number(this.lastMsgSeq), function(errorCode){
                    if (errorCode != 0) {
                        uc.util.LogUtil.info("uc.modules.historymsg.HistoryManager","RequestInitHistoryList", "data=",{groupId:this.currentChatId, errorCode:errorCode});
                    }
                });
            }
        },
        getChatHistoryReceived: function (errorCode, conversationType, groupId, messages, msgCount, minTimestamp) {
            if (this.historyLay) {
                if (groupId != this.currentChatId) {
                    return;
                }
                if (errorCode == 0)
                    this.lastMsgSeq = minTimestamp;
                if (conversationType == null) conversationType = this.currentChatType;
                this.onMoreMsgLoadEnd();
                if (conversationType == uc.constants.Chat.ChatType.MeetingChat) {
                    uc.modules.historymsg.History.getChatHistoryReceived(errorCode, conversationType, groupId, messages, msgCount, minTimestamp);
                }
                else {
                    if (this.currentChatType && this.currentChatType != uc.constants.Chat.ChatType.GroupChat && this.currentChatType != uc.constants.Chat.ChatType.SingleChat && this.currentChatType != uc.constants.Chat.ChatType.ProjectChat && this.currentChatType != uc.constants.Chat.ChatType.CloudChat) {
                        return;
                    }
                    uc.modules.historymsg.History.getChatHistoryReceived(errorCode, conversationType, groupId, messages, msgCount, minTimestamp);
                }
                if(this.Scope().attr("stop")==1)
                {
                    this.onLoadAllMsgEnd();
                }
                // add callback fun
                var moreMsg = this.Scope(".more-msg");
                if(moreMsg.attr("goOn")!=null)
                {
                    if( moreMsg.offset().top>= parseInt(moreMsg.attr("goOn"))){
                        moreMsg.click();//serach some msg(txt file img ) don't include other msg ;so continue query next result
                    }
                }
            }
        },
        getTextMessageReceived: function (conversationType, channel, sendState, from, to, msgId, msgSeq, textType, textContent, sendTime, textFormat,atUsers) {
            if (uc.constants.Message.MessageChannel.History == channel || uc.constants.Message.MessageChannel.Search == channel) {
                if (conversationType == null) conversationType = this.currentChatType;
                if (!this.isCurrentMsg(conversationType, from, to))
                    return;
                var self = this.isSendByMyself(from);
                if (uc.constants.Message.MessageChannel.Search == channel) {
                    var searchkeyword = this.keyWord;
                    if(uc.modules.historymsg.AtUserHistoryActive) // this operator object is AtUserHistory
                    {
                        if(parseInt(this.beginTime) !=msgSeq)
                        {
                            searchkeyword ="";
                        }
                    }
                    textContent = $.MatchKeyWord(textContent,searchkeyword);
                }
                var domStr = uc.modules.historymsg.History.getTextMsgDomStr(sendState, self, from, msgId, msgSeq, textType, textContent, sendTime, sendTime, textFormat);
                if (uc.constants.Message.MessageChannel.Search == channel)
                    uc.modules.historymsg.History.procMessageNextResultDomStr(channel, domStr, msgId, msgSeq);
                else
                    uc.modules.historymsg.History.procMessageDomStr(channel, domStr, msgId, msgSeq);
            }
        },
        getImageMessageReceived: function (conversationType, channel, sendState, from, to, msgId, msgSeq, fileName, fileSize, fileURL, thumbnails, msgTime) {
            if (uc.constants.Message.MessageChannel.History == channel||uc.constants.Message.MessageChannel.Search == channel) {
                if (conversationType == null) conversationType = this.currentChatType;
                if (!this.isCurrentMsg(conversationType, from, to))
                    return;
                var self = this.isSendByMyself(from);
                var domStr = uc.modules.historymsg.History.getImgMsgDomStr(sendState, self, from, msgId, msgSeq, fileName, fileSize, fileURL, thumbnails, msgTime);
                if (uc.constants.Message.MessageChannel.Search == channel)
                {
                    uc.modules.historymsg.History.procMessageNextResultDomStr(channel, domStr, msgId, msgSeq);
                }
                else
                {
                    uc.modules.historymsg.History.procMessageDomStr(channel, domStr, msgId, msgSeq);
                }
            }
        },
        getFiledMessageReceived: function (conversationType, channel, sendState, from, to, operatorId, fileId, fileName, fileSize, downloadUrl, fileFormat, desc, msgId, msgSeq, msgTime, username,localPath) {
            if (uc.constants.Message.MessageChannel.History == channel||uc.constants.Message.MessageChannel.Search == channel) {
                if (conversationType == null) conversationType = this.currentChatType;
                if (!this.isCurrentMsg(conversationType, from, to))
                    return;
                var self = this.isSendByMyself(from);
                var domStr = uc.modules.historymsg.History.getFileMsgDomStr(conversationType, sendState, self, from, msgId, msgSeq, fileName, fileSize, downloadUrl, msgTime, fileId, username,localPath);
                if (uc.constants.Message.MessageChannel.Search == channel)
                {
                    uc.modules.historymsg.History.procMessageNextResultDomStr(channel, domStr, msgId, msgSeq);
                }
                else
                {
                    uc.modules.historymsg.History.procMessageDomStr(channel, domStr, msgId, msgSeq);
                }
            }
        },

        RequestHistoryListByKeyWord: function (btnObj, isreQuery) {
            var _this = this;
            var oppage = $(btnObj).parents("." + uc.modules.historymsg.Page);
            var keyword = oppage.find(".searchbarIO input").val();
            if (keyword.trim() != "") {
                if (_this.pageIndex == undefined)
                    _this.pageIndex = 1;
                if (isreQuery) //reQuery
                    _this.pageIndex = 1;

                uc.ISearchService.searchHistoryMsgByKeyWord(_this.currentChatType, _this.currentChatId, keyword, _this.pageIndex, 10, function (args) {
                    var errorCode = args[0];
                    var requestId = args[1];
                    if (errorCode != 0) {
                        uc.util.LogUtil.info("uc.modules.historymsg.History", "RequestHistoryListByKeyWord", "data=", {
                            groupId: this.currentChatId,
                            count: 10,
                            page: this.pageIndex,
                            keyword: keyword
                        });
                    }
                    else {
                        _this.pageIndex = parseInt(_this.pageIndex) + 1;
                        _this.keyWord = keyword;
                        _this.keyWordResultID = requestId;
                    }
                });
                return true;
            }
            else {
                return false;
            }
        },
        GetHistoryListByKeyWord: function (errorCode, requestId, searchMsg, allCount) {
            /*    return more  by keyword and pagesize   */
            var _this = this;
            if (errorCode != 0)
                return;
            if (_this.keyWordResultID == requestId) {
                if (_this.rootBody.find("div[active='1']").get(0) != undefined)
                    _this.rootBody.find("div[active='1']").find(".titlebar .title").text("搜索到" + allCount + "条消息记录");
                var channel = uc.constants.Message.MessageChannel.History;
                var keyWordResultHtml = "";
                var New_searchMsg = searchMsg.sort(function (a,b) {
                   return  a.msgSeq -b.msgSeq;
                });
                for (var i in New_searchMsg) {
                    var row = searchMsg[i];
                    var conversationType = row.conversationType;
                    var sendState = row.sendState;
                    var from = row.from;
                    var to = row.to;
                    var msgId = row.msgId;
                    var msgSeq = row.msgSeq;
                    var textType = row.textType;
                    var textContent = row.textContent;
                    var sendTime = row.msgSeq;
                    var msgTime = row.msgTime;
                    var textFormat = row.textFormat;
                    keyWordResultHtml += _this.getTextMessageByKeyWordReceived(conversationType, channel, sendState, from, to, msgId, msgSeq, textType, textContent, sendTime, msgTime, textFormat);
                }
                uc.modules.historymsg.History.procMessageKeyWordByDomStr(channel, keyWordResultHtml, searchMsg.length);
                if(_this.Scope().attr("stop")==1)
                {
                    _this.onLoadAllMsgEnd();
                }
                else
                {
                    _this.moreMsgLine.show();
                    _this.onMoreMsgLoadEnd();
                }

            }
        },
        getTextMessageByKeyWordReceived: function (conversationType, channel, sendState, from, to, msgId, msgSeq, textType, textContent, sendTime, msgTime, textFormat) {
            if (uc.constants.Message.MessageChannel.History == channel) {
                var self = this.isSendByMyself(from);
                var searchkeyword = this.keyWord;
                textContent = $.MatchKeyWord(textContent,searchkeyword);
                var domStr = uc.modules.historymsg.History.getTextMsgDomStr(sendState, self, from, msgId, msgSeq, textType, textContent, sendTime, msgTime, textFormat);
                return domStr;
            }
        },
        RequestHistoryByNexeResult: function (forntOrBack, beginTime, endTime) {
            var _this = this;
            var beginTime = parseInt(beginTime);
            var endTime = parseInt(endTime);
            var count = 10;
            uc.util.LogUtil.info("uc.modules.historymsg.History", "RequestHistoryByNexeResult", "data=", {
                currentChatType: _this.currentChatType,
                groupId: _this.currentChatId,
                count: count,
                beginTime: beginTime,
                endTime: endTime,
                forntOrBack: forntOrBack
            });
            uc.ISearchService.searchHistoryByNexeResult(_this.currentChatType, _this.currentChatId, count, beginTime, endTime, forntOrBack, function (args) {
                var errorCode = args[0];
                if (errorCode != 0) {
                    uc.util.LogUtil.info("uc.modules.historymsg.History", "RequestHistoryByNexeResult", "data=", {
                        currentChatType: _this.currentChatType,
                        groupId: _this.currentChatId,
                        count: count,
                        beginTime: beginTime,
                        endTime: endTime,
                        forntOrBack: forntOrBack
                    });
                }
                else {

                }
            });
        },
        GetHistoryByNexeResult: function (errorCode, conversationType, groupId, msgCount, minmsgtime, maxmsgTime) {
            if (this.currentChatType && this.currentChatType != uc.constants.Chat.ChatType.GroupChat && this.currentChatType != uc.constants.Chat.ChatType.SingleChat && this.currentChatType != uc.constants.Chat.ChatType.ProjectChat && this.currentChatType != uc.constants.Chat.ChatType.CloudChat) {
                return;
            }
            if (groupId != this.currentChatId) {
                return;
            }
            if (errorCode == 0) {
                this.lastMsgSeq_front = minmsgtime;//front;
                this.lastMsgSeq_back = maxmsgTime;//back
            }
            if (conversationType == null) conversationType = this.currentChatType;
            uc.modules.historymsg.History.getChatHistoryNextResultReceived(errorCode, conversationType, groupId, minmsgtime, msgCount, maxmsgTime);
            //init request two
            if (this.requestTwo != undefined) {
                uc.modules.historymsg.History.moveUporDown_next = 0;
                this.RequestHistoryByNexeResult(0, 0, this.requestTwo);//request front Nexthistory
                this.requestTwo = null;
            }
            if(this.Scope().attr("upstop")==1||this.Scope().attr("downstop")==1)
            {
                this.onLoadAllMsgEnd();
            }
        },
        isCurrentMsg: function (chatType, from, to) {
            if (this.currentChatId != from.userId && this.currentChatId != to.userId) {
                return false;
            }
            if (this.currentChatType != chatType && this.moduleId != uc.constants.UI.ModuleId.Meeting) {
                return false;
            }
            return true;
        },
        isSendByMyself: function (from) {
            if (from.userId != this.myUserInfo.userId) {
                var contactInfo = uc.modules.cache.BasicContactInfoCache.getBasicContactInfo(from.userId);
                if (contactInfo) {
                    from.displayName = contactInfo.displayName;
                    from.localAvatar = contactInfo.localAvatar;
                    from.remoteAvatarUrl = contactInfo.remoteAvatarUrl;
                    if (from.localAvatar == null || from.localAvatar == "") {
                        //如果是超级群，不下载图像
                        if (this.currentChatType == uc.constants.Chat.ChatType.GroupChat || this.currentChatType == uc.constants.Chat.ChatType.ProjectChat && (this.groupInfo && this.groupInfo.memberType)) {
                            return;
                        }
                        if (from.remoteAvatarUrl != "") {
                            try {
                                from.localAvatar = this.getUserLogo(from.userId, from.remoteAvatarUrl);
                            }
                            catch (e) {
                                from.localAvatar = "../images/icon_avatar_default_40.png";
                            }
                        }
                        else
                            from.localAvatar = "../images/icon_avatar_default_40.png";
                    }
                }
            }
            return from.userId == this.myUserInfo.userId;
        },
        /*At query history control Main*/
        AtQueryControl:function (createObj,beginTime,keyword) {
            if(beginTime==undefined)
            {
                return;
            }
            var _this = this;
            _this.PageStatus.extend =function (id) {
                uc.modules.historymsg.scope = _this.rootBody.find("." + uc.modules.historymsg.Page);
                uc.modules.historymsg.scope.attr("active", "0");
                var cpage = $("#level" + id);
                cpage.attr("active", "1");
                cpage.find(".list").height(cpage.height());
                cpage.find(".titlebar").hide();
                cpage.find(".searchbar").hide();

                _this.moreMsgLine = $('.more-msg-line', cpage);
                _this.moreMsg = $('.more-msg', cpage);
                _this.moreMsgLoading = $('.more-msg-loading', cpage);
                _this.onMoreMsgLoading();
                if (id == 0) {
                    uc.modules.historymsg.History.init();
                    uc.modules.historymsg.History.levelNextHistory = cpage;
                    uc.modules.historymsg.History.moveUporDown_next = 1;
                    _this.beginTime =beginTime;
                    _this.endTime ="0";
                    _this.forntOrBack =1;
                    _this.requestTwo = _this.beginTime;
                    _this.RequestHistoryByNexeResult(_this.forntOrBack, _this.beginTime, _this.endTime);//request back Nexthistory
                    cpage.find('.searchbarIO').hide();
                    if(keyword!=undefined)
                        _this.keyWord=keyword;
                    else
                        _this.keyWord= _this.myUserInfo.displayName;
                }
            };
            _this.bindEvent.extend =function (id) {
                var cpage = $("#level" + id);
                var scope = _this.rootBody.find("div[active='1']");
                _this.lastTop = 0;
                var setSrcoll = function () {
                    if (scope != null && scope.get(0) != undefined) {
                        scope.find(".list").scroll(function () {
                            _this.IsStopOtherScroll(true);	//stop other scroll
                            var top = this.scrollTop;
                            if (scope.attr("stop") == 1)
                                return;
                            _this.onMoreMsgLoading();
                             if (id == 0)
                             {
                                  if(top == 0)
                                  {
                                      return;
                                  }
                                var move = this.scrollTop - _this.lastTop;
                                var objHeight = this.scrollHeight - $(this).height() - 200;
                                _this.lastTop = top;
                                var frontorBack = 1;
                                var beginTime = 0;
                                var endTime = 0;
                                if (scope.attr("downstop") == 1 && scope.attr("upstop") == 1) {
                                    //queryNextResult end
                                    return;
                                }
                                if (move < 0) {
                                    if (top < 200) {
                                        //scope.find(".title").text("加载上一页");
                                        if (scope.attr("upstop") == 1)
                                            return;
                                        frontorBack = 0;
                                        endTime = uc.modules.historymsg.History.lastMsgSeq_next;
                                        var domMin = scope.find(".row").not("[rel='time']").first().attr("seq");
                                        if (domMin != endTime) endTime = domMin;
                                        beginTime = 0
                                        uc.modules.historymsg.History.moveUporDown_next = 0;
                                        _this.RequestHistoryByNexeResult(frontorBack, beginTime, endTime);// move up request
                                    }
                                    else
                                        scope.find(".title").text("");
                                }
                                else {
                                    if (top > objHeight) {
                                        if (scope.attr("downstop") == 1)
                                            return;
                                        //scope.find(".title").text("加载下一页");
                                        frontorBack = 1;
                                        endTime = 0;
                                        beginTime = uc.modules.historymsg.History.lastMsgSeq_max_next;
                                        var domMax = scope.find(".row").not("[rel='time']").last().attr("seq");
                                        if (domMax != beginTime) beginTime = domMax;
                                        uc.modules.historymsg.History.moveUporDown_next = 1;
                                        _this.RequestHistoryByNexeResult(frontorBack, beginTime, endTime);// move down request
                                    }
                                    else
                                        scope.find(".title").text("");
                                }
                            }
                        });
                    }
                }
                setSrcoll();
            };
            var param = {"level": 0};
            _this.createHistoryPage(createObj,param,true);
            uc.modules.historymsg.AtUserHistoryActive = true;
        },
        /*General Query History Message  for keyword history control  Main*/
        GeneralQueryMessageBykeyWord:function (msgTime,keyword) {
            var _this = this;
            var createObj = $("." + uc.modules.historymsg.parents).find("div").first();
            _this.AtQueryControl(createObj,msgTime,keyword);
        },
        isQueryMeetingChat:function (eventId) {
           if(this.meetingChatId==eventId)
           {
               uc.modules.historymsg.isMeetingChat =true;
               return true;
           }
           else
           {
               return false;
           }
        }
    };
})(uc);