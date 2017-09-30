/**
 * Created by zhen.xu on 2015/9/16.
 */
(function(uc){
    uc.modules.openApi.OpenApiView = function(){
        this.nodeHtml ='<div class="head"><img class="open-app-logo" />' +
            '<span class="open-app-name"></span>' +
            '<span class="open-app-intro"></span></div>' +
            '<div><a class="open-app-sendMsg" href="javascript:void(0);">'+ uc_resource.OpenApi.OpenApiSendMsg +'</a><span class="split">|</span>' +
            '<a class="open-app-open">'+ uc_resource.OpenApi.OpenApp +'</a></div><span class="msg-count"></span>';
        this.node = $('<li class="open-api-item"></li>');
        this.undefinedApp = $('<li class="undefinedApp"></li>');
    };

    jQuery.extend(uc.modules.openApi.OpenApiView.prototype, {
        init: function(obj){
            this.node.append(this.nodeHtml);
            this.item = obj.item;
            if ( !this.item ) {
                this.undefinedApp.appendTo(obj.parentNode);
                return;
            }
            this.appendParent(obj.parentNode);
            this.token = obj.token;
            this.account = obj.account;
            this.appLogo = $('.open-app-logo', this.node);
            this.appName = $('.open-app-name', this.node);
            this.appIntro = $('.open-app-intro', this.node);
            this.appsendMsg = $('.open-app-sendMsg', this.node);
            this.split = $('.split', this.node);
            this.appOpen = $('.open-app-open', this.node);
            this.headNode = $('.head', this.node);
            this.msgCount = $('.msg-count', this.node);


            this.setApp(this.item);
            this.setAppOpenLink(this.item);
            this.bindEvent();
        },
        appendParent: function (parent) {
            this.node.appendTo(parent);
        },
        setApp: function (item) {
	    if(item.type){
                this.appOpen.attr('rel', item.type);
            }
            this.node.attr('data-app-id', item.appId);
            this.appLogo.attr('src', item.appLogo);
            this.appName.text(item.appTitle);
            this.setAppDesc(item.appDesc);
            this.setSendMsg(item.enableSendMessage);
            this.getAppUntreated(item.eventUrl);
        },
        setAppDesc: function (desc) {
            if (!desc) {
                this.headNode.addClass('no-desc');
                return;
            }
            this.appIntro.text(desc);
        },
        setSendMsg: function (enableSendMessage) {
            if (!enableSendMessage) {
                this.appsendMsg.hide();
                this.split.hide();
            }
        },
        setAppOpenLink: function (item) {
            if (!item.pcUrl) {
                this.appOpen.attr('href', '###');
                return;
            }
            var _join = (item.pcUrl.indexOf('?') >= 0 ? '&' : '?');
            this.appOpen.attr('href', item.pcUrl+ _join + 'uc_account=' + this.account +'&uc_token=' + this.token);
        },
        getAppUntreated: function (url) {
            var self = this;
            if (url && url != 'null') {
                var url = (url + uc.modules.openApi.OpenApi.getAppAuth(url));
                uc.IOpenApiService.sendRequest(1, url, '', [], function(args){
                    uc.util.LogUtil.info('OpenApiView', 'getAppUntreated', 'args', args);
                    if (args[0] == 0) {
                        self.msgCount.removeClass('show-count show-point');
                        self.msgCount.text('');
                        try {
                            var data = JSON.parse(args[1]).data;
                            if (data.displayCount) {
                                self.msgCount.addClass('show-count');
                                if(data.eventCount > 10) {
                                    self.msgCount.addClass('double')
                                }

                                if (data.eventCount >= 100) {
                                    data.eventCount = '99+';
                                }
                                //显示数量
                                self.msgCount.text(data.eventCount);
                            } else {
                                if(data.eventCount > 0){
                                    //显示红点
                                    self.msgCount.addClass('show-point');
                                }

                            }
                        }catch(e){
                            uc.util.LogUtil.info('OpenApiView', 'OnResponse', "JSON parse fail! ", null);
                        }
                    }
                });
            }
        },
        bindEvent: function () {
            var _this = this;

            this.node.on('click', '.open-app-open', function () {
                var href = $(this).attr('href');
                if (href == '###') {
                    uc.IUIService.openPresetApp(_this.item.appId);
                }
            });

            this.node.on('click', '.open-app-sendMsg', function () {
                uc.util.LogUtil.info('openApi', 'OpenApiView', 'sendMessage', {appId: _this.item.appId})
                uc.IUIService.openChatWindow(uc.constants.Conversation.ConversationType.OpenApi, _this.item.appId, uc.constants.UI.OpenChatWindowSourceModule.Other, null, null, true);
            });
        }
    })
})(uc);