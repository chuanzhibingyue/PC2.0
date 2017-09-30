(function (uc) {
    var capability = JSON.parse(uc.util.StorageUtil.getData("capability"));
    var keywordTextCls ="keywordPbxText";
	uc.modules.component.PbxSearch ={
        time:0,
        keyWord:"",
        page:1,
        callList:{},
        callUserIdList:{},
        contactList:{},
        needScroll:true,
        repeatClick:false,
		initUI: function (option) {
            this.searchContent=
                $('<div class="pbx-search-content node-shadow">' +
                    '<div class="panel-search-warp">' +
                    '<div class="search-result default-search-tel">' +
                    '<span class="callIcon icon-call_phoneVideo"></span>' +
                    '<span class="tel"></span>' +
                    '</div>' +
                    '<div class="search-result pbx-default-search-result"></div>' +
                    '<div class="search-result pbx-contact-search-result"></div>' +
                    '</div>' +
                    '</div>');
            this.telNode='<div class="pbx-search-tel-list node-shadow">' +
                '<div class="pbx-search-call-bee hide">'+uc_resource.Pbx.callBee+'</div>' +
                '<div class="pbx-search-call-pbx1"></div>' +
                '<div class="pbx-search-call-pbx2"></div>' +
                '</div>'
            option.append(this.searchContent);
            option.append(this.telNode);
            this.bindEvent();
            var loginUserInfo = uc.IClientService.getCurrentUCID();
            if(loginUserInfo){
                this.loginUerId = loginUserInfo.userId;
            }
		},
        bindEvent:function(){
            var self=this;
            var timer;
            $(".pbx .search-input").on("input",function(){
                var inputVal= $.trim($(this).val());
                if(!inputVal){
                    $(".pbx .pbx-search-content").hide();
                }
                timer && clearTimeout(timer);
                timer= setTimeout(function(){
                    self.startSearchPbx(inputVal);
                },300);
            });
            $(".pbx .search-input").on("focus",function(){
                $(".pbx .pbx-dialPanel").addClass("hide").removeAttr("style");
                $(".pbx .pbx-search-tel-list").hide();
                var inputVal= $.trim($(this).val());
                if(inputVal){
                    $(".pbx .pbx-search-content").show();
                }
            });
            $(".pbx .search-input").on("keypress",function(){
                var inputVal= $.trim($(this).val());
                if(!inputVal){
                    $(".pbx .pbx-search-content").hide();
                }
            });
            $(".pbx").on("click",".more-contact",function(){
                if(uc.modules.component.PbxSearch.repeatClick){
                    return;
                }
                uc.modules.component.PbxSearch.repeatClick=true;
                uc.modules.component.PbxSearch.searchContactInfo();
            });
            $(".pbx .pbx-search-content").scroll(function(){
                if(!uc.modules.component.PbxSearch.needScroll){
                    return;
                }
                var viewH =$(this).height(),
                    contentH =$(this).get(0).scrollHeight,
                    scrollTop =$(this).scrollTop();
                if(scrollTop/(contentH -viewH)>=0.95){
                    uc.modules.component.PbxSearch.page++;
                    self.searchContactInfo();
                }
            });
            $(".pbx").on("click",".default-search-tel",function(){
                if(capability && capability.pbx == 1){
                    var tel = $(this).find(".tel").text();
                    if (tel) {
                        if (tel.length > 15) {
                            self.showPbxPhoneTelError();
                            return;
                        }
                        if($.inArray((tel*1),uc.constants.Pbx.PbxCallBackList) > -1){
                            self.ForbidPhoneTel();
                            return false;
                        }
                        uc.modules.pbx.PbxManager.dialPadCallback(tel);
                        $(".pbx .pbx-search-content").hide();
                    }
                }else{
                    uc.modules.component.PbxSearch.showPbxCallLimits();
                }

            });
            $(".pbx").on("click",".pbx-default-search-result li",function(){
                var rel=$(this).attr("rel");
                $(".pbx .pbx-search-tel-list").hide();
                if(capability && capability.pbx == 1){
                    var data=uc.modules.component.PbxSearch.callList[rel];
                    if(data){
                        if(data.userID){
                            var callInfo={"userName":data.name,
                                "userID":data.userID,
                                "phoneNumber":data.number,
                                "callType":0,
                                "time":new Date().getTime(),
                                "Type":"ADEXEO2103"
                            };
                            uc.modules.pbx.PbxCallManager.startCallPbx(callInfo);
                        }else{
                            uc.modules.pbx.PbxManager.dialPadCallback(data.number);
                        }
                    }
                }else{
                    uc.modules.component.PbxSearch.showPbxCallLimits();
                }
                $(".pbx .pbx-search-content").hide();
            });
            $(".pbx").on("click",".pbx-contact-search-result li",function(e){
                var rel=$(this).attr("rel");
                $(".pbx .pbx-search-tel-list").hide();
                if(capability && capability.call == 1){
                    $(".pbx-search-call-bee").removeClass("hide");
                }
                $(this).addClass("active").siblings().removeClass('active');
                   var data=uc.modules.component.PbxSearch.contactList[rel];
                    if(data){
                        $(".pbx .pbx-search-tel-list").attr("rel",data.userId);
                    }
                    if(capability && capability.pbx == 1){
                        uc.modules.component.PbxSearch.getTel(data);
                    }else{
                        $(".pbx .pbx-search-call-pbx1").hide();
                        $(".pbx .pbx-search-call-pbx2").hide();
                    }
                       var self=$(this).find(".contact-tel");
                       var hTel=$(".pbx .pbx-search-tel-list").height();
                       var wTop= self.offset().top+23;
                       var winHeight = $(window).height();
                        if((winHeight-wTop-60) < hTel){
                            wTop=wTop-hTel-23;
                        }
                       var _left = self.offset().left;
                       var _top  = self.offset().top;
                       $(".pbx .pbx-search-tel-list").show().css({
                           "left": _left-120,
                           "top":	wTop,
                           "z-index": 12
                       });
            });
            $(".pbx").on("click",".pbx-search-tel-list div",function(){
                var rel=$(this).parent().attr("rel");
                var index=$(this).index();
                var data=uc.modules.component.PbxSearch.contactList[rel];
                if(data){
                    var callInfo={"userName":data.displayName,
                        "userID":data.userId,
                        "phoneNumber":$(this).text(),
                        "callType":0,
                        "Type":"ADEXEO2103"
                    };
                    if(index==0){
                        uc.IUIEvent.OnPbxCallBee(data.userId);
                    }else{
                        uc.modules.pbx.PbxCallManager.startCallPbx(callInfo);
                    }
                }
                $(".pbx .pbx-search-content").hide();
                $(".pbx .pbx-search-tel-list").hide();
            });
        },
        getTel:function(data){
            if(data.mobile.indexOf("/") >-1){
                var sp=data.mobile.split("/");
                $(".pbx .pbx-search-call-pbx1").show().text(sp[0]);
                $(".pbx .pbx-search-call-pbx2").show().text(sp[1]);
            }else{
                if(Number(data.mobile)){
                    $(".pbx .pbx-search-call-pbx1").show().text(data.mobile);
                }else{
                    $(".pbx .pbx-search-call-pbx1").hide();
                }
                $(".pbx .pbx-search-call-pbx2").hide();
            }
        },
        startSearchPbx:function(keyWord){
            if(!keyWord){
                return false;
            }
            uc.modules.component.PbxSearch.needScroll=true;
            uc.modules.component.PbxSearch.page=1;
            uc.modules.component.PbxSearch.keyWord=keyWord;
            uc.modules.component.PbxSearch.callList={};
            uc.modules.component.PbxSearch.callUserIdList={};
            uc.modules.component.PbxSearch.contactList={};
            $(".pbx .pbx-search-content").show();
            $(".pbx-default-search-result,.pbx-contact-search-result",this.searchContent).html("");
                if(!isNaN(keyWord)){
                    $(".default-search-tel",this.searchContent).show();
                    $(".default-search-tel .tel",this.searchContent).html(keyWord);
                }else{
                    $(".default-search-tel",this.searchContent).hide();
                }
            uc.IPbxService.getSearchCallRecord(keyWord,uc.modules.component.PbxSearch.time,uc.constants.Pbx.GetSearchCallCount,function(args){
                var res = args[0];
                uc.util.LogUtil.info('PbxSearch', 'startSearchPbx', 'errorCode = '+res, null);
            });
        },
        searchContactInfo:function(){
            uc.ISearchService.searchServer(uc.modules.component.PbxSearch.keyWord,uc.modules.component.PbxSearch.page,uc.constants.Pbx.GetSearchCallCount, [1], function (args) {
                uc.util.LogUtil.info('PbxSearch', 'search', 'call contacts search count:', args[1]);
            })
        },
        OnPbxGetSearchCall:function(errorCode,data){
            if(!data.length){
                var moreNode ="<div class='pbx-search-contact-title'>" +
                    "<div class='bread-crumb-contact'><span class='bread-crumb-center more-contact'>"+uc_resource.Pbx.moreContact+"</span></div></div>";
                $(".pbx-default-search-result",this.searchContent).append(moreNode);
               return;
            }
            var li="";
            var comData="";
            var contact="<div class='bread-crumb'><span class='bread-crumb-right'>"+uc_resource.Pbx.recentlyCall+"</span></div>";
            var len= 0;
                for(var i = 0;i <  data.length;i++){
                    if(len >= 5){
                        break;
                    }
                   uc.modules.component.PbxSearch.callList[data[i].number]=data[i];
                    if(data[i] && data[i].userID){
                        if(uc.modules.component.PbxSearch.callUserIdList[data[i].userID]){
                            continue;
                        }else{
                            uc.modules.component.PbxSearch.callUserIdList[data[i].userID]=data[i];
                        }
                    }
                   comData=this.getPbxContactUserInfo(data[i]);
                   li+= "<li class='contact-item' rel="+data[i].number+">" +
                        "<div class='contact-widget-container'>" +
                        "<div class='left'></div>" +
                        "<div class='center'>" +
                        "<div class='pbx-contact-display-name text-ellipsis'>"+uc.util.StringUtil.textSearchHighLight(comData.name,uc.modules.component.PbxSearch.keyWord,keywordTextCls)+"</div>" +
                        "<div class='pbx-contact-description text-ellipsis'>" +
                        "<span class='pbx-contact-department'>"+comData.departmentName+"</span>" +
                        "<span class='pbx-contact-position'>"+comData.position+"</span></div>" +
                        "</div>" +
                        "</div>" +
                        "</li>";
                    len++;
                }
            var moreNode ="<div class='pbx-search-contact-title'><div class='bread-crumb-contact'><span class='bread-crumb-center more-contact'>"+uc_resource.Pbx.moreContact+"</span></div></div>";
            contact=contact+"<ul>"+li+"</ul>"+moreNode;
            $(".pbx-default-search-result",this.searchContent).html(contact);
        },
        OnServerSearchResultReceived:function(errorCode, searchId, contacts, projects, groupChats, cloudGroups, meetings, messages){
          uc.util.LogUtil.info('PbxSearch', 'OnServerSearchResultReceived', 'errorCode='+errorCode,null);
          if(errorCode == 0 && contacts.length){
              var li="";
              var avatarNode="";
              var userID;
              for(var i= 0,len=contacts.length;i<len;i++){
                  userID=contacts[i].userId;
                  if(this.loginUerId == userID){
                      continue;
                  }
                  avatarNode=this.getPbxContactUserInfo({"userID":userID});
                  uc.modules.component.PbxSearch.contactList[userID]=contacts[i];
                  li+= "<li class='contact-item' rel="+userID+">" +
                      "<div class='contact-widget-container'>" +
                      "<div class='left'></div>" +
                      "<div class='center'>" +
                          "<div class='pbx-contact-display-name text-ellipsis'>"+uc.util.StringUtil.textSearchHighLight(contacts[i].displayName,uc.modules.component.PbxSearch.keyWord,keywordTextCls)+"</div>" +
                          "<div class='pbx-contact-description text-ellipsis'>" +
                              "<span class='pbx-contact-department'>"+contacts[i].departmentName+"</span>" +
                              "<span class='pbx-contact-position'>"+contacts[i].position+"</span>" +
                              "<span class='contact-tel'></span>" +
                           "</div>" +
                      "</div>" +
                      "</div>" +
                      "</li>";
              }
              var node="<ul>"+li+"</ul>";
              $(".pbx-search-contact-title",this.searchContent).html(this.getContactPanel());
              $(".pbx-contact-search-result",this.searchContent).append(node);
          }
          if(contacts.length < uc.constants.Pbx.GetSearchCallCount){
              uc.modules.component.PbxSearch.needScroll=false;
          }
          if(!contacts.length &&  uc.modules.component.PbxSearch.page == 1){
              $(".pbx-search-contact-title",this.searchContent).html("<div class='bread-crumb'><span class='bread-crumb-right'>"+uc_resource.Pbx.contact+"</span></div></div><div class='empty-result'></div>");
          }
            uc.modules.component.PbxSearch.repeatClick=false;
        },
        getContactPanel:function(){
          return "<div class='bread-crumb'><span class='bread-crumb-right'>"+uc_resource.Pbx.contact+"</span></div>";
        },
        getPbxContactUserInfo:function(data){
            var contactInfo,avatarNode,name,departmentName="",position="";
            if(data && data.userID){
                contactInfo = uc.modules.cache.BasicContactInfoCache.getBasicContactInfo(Number(data.userID));
                if(contactInfo){
                     avatarNode = uc.util.AvatarUtil.composeContactAvatar(contactInfo.localAvatar, uc.constants.UI.DefaultLocalUrl.Avatar_80);
                     name=contactInfo.displayName;
                    departmentName=contactInfo.departmentName;
                    position=contactInfo.position;
                }else{
                    avatarNode = uc.util.AvatarUtil.composeContactAvatar("", uc.constants.UI.DefaultLocalUrl.Avatar_80);
                    name=data.number;
                    //todo 联系人为空需要向服务器获取
                }
            }else{
                avatarNode = uc.util.AvatarUtil.composeContactAvatar("", uc.constants.UI.DefaultLocalUrl.Avatar_80);
                name=data.number;
            }

            return {
                avatar:avatarNode,
                name:name,
                departmentName:departmentName,
                position:position
            }
        },
        showPbxPhoneTelError: function(){
            uc.modules.component.PromptMessageBox.open({
                title: uc_resource.WindowTitle.Notification,
                message: uc_resource.Pbx.PhoneTelError,
                buttonMode: "ok",
                OkText: uc_resource.Common.OK
            }, function () {
                this.close();
            });
        },
        ForbidPhoneTel: function(){
            uc.modules.component.PromptMessageBox.open({
                title: uc_resource.WindowTitle.Notification,
                message: uc_resource.Pbx.ForbidPhoneTel,
                buttonMode: "ok",
                OkText: uc_resource.Common.OK
            }, function () {
                this.close();
            });
        },
        showPbxCallLimits: function(){
            uc.modules.component.PromptMessageBox.open({
                title: uc_resource.WindowTitle.Notification,
                message: uc_resource.Pbx.PbxCallLimits,
                buttonMode: "ok",
                OkText: uc_resource.Common.OK
            }, function () {
                this.close();
            });
        }
	}
})(uc);