/**
 * Created by lei.zhou on 2015/7/31.
 */
(function (uc) {
    var HTML = '<li class="userDropList" qtips="'+uc_resource.Meeting.UserName+': {NAME} ;</br>'+uc_resource.Meeting.Department+'：{DEPARTMENT}">' +
         '<div class="info userName"></div>' +
         '<div class="info staff-post">' +
         '<span></span>' +
         '</div>' +
         '</li>';
    var initSearchBox = false, $target, $items, $list, options,
        userAvatar = {}, searchTime, page = 1, searchUser = [],
        dealUsers = [],
        selectedUser = {}, searchAllData = {}, searchedContent, searchIds = {}, timeoutId;

    function _inputEnd() {
        var _t = $items.find("li.userDropList-selected");
        if (_t.length == 1) {
            selectedUser = searchAllData["" + _t.attr("data-userId") + ""];
            options.selectedUser(selectedUser);
        }
        else {
            _t = $items.find("li");
            if (_t.length == 1) {
                selectedUser = searchAllData["" + _t.attr("data-userId") + ""];
                options.selectedUser(selectedUser);
            }
            else {
                var email = /^[a-z0-9]+([._\\-]*[a-z0-9])*@([a-z0-9]+[-a-z0-9]*[a-z0-9]+.){1,63}[a-z0-9]+$/g,
                    text = $target.val();
                text=text.replace(";","");
                if (email.test(text)) {
                    for(var i in searchAllData) {
                        if(searchAllData[i].email == text) {
                            selectedUser = searchAllData[i];
                            options.selectedUser(selectedUser);
                            dealUsers.push(selectedUser);
                            hideSearchBox();
                            return ;
                        }
                    }
                        selectedUser = {
                            account: text,
                            displayName: text
                        };
                        options.selectedUser(selectedUser);

                }
            }
        }
        $("#searchList").html("");
        hideSearchBox();
    }

    function _navigateList(isUpward) {
        var selected = $items.children(".userDropList-selected");
        if (selected.length > 0) {
            var $temp;
            if (isUpward) {
                $temp = selected.prev("li");
                if ($temp.length > 0) {
                    selected.removeClass("userDropList-selected");
                    $temp.addClass("userDropList-selected");
                }
            }
            else {
                $temp = selected.next("li");
                if ($temp.length > 0) {
                    selected.removeClass("userDropList-selected");
                    $temp.addClass("userDropList-selected");
                }
                else {
                    $list.find("a.more").click();
                }
            }
        }
        else {
            $items.children("li:first").addClass("userDropList-selected");
        }

        selected = $items.children(".userDropList-selected");
        var top = $items.scrollTop() + selected.position().top;
        $items.scrollTop(top);
    }

    function _searchContent(page, flag) {
        var text = searchedContent;

        //if (!flag) {
        //    uc.ISearchService.searchLocal(text, page, options.count * 2, [1], function (args) {
        //        if (args[0] == 0) {
        //            searchIds[args[1]] = true;
        //        }
        //    });
        //} else {
        //    uc.ISearchService.searchServer(text, page, options.count * 2, [1], function (args) {
        //        if (args[0] == 0) {
        //            searchIds[args[1]] = true;
        //        }
        //    });
        //}

        uc.ISearchService.searchServer(text, page, options.count * 2, [1], function (args) {
            if (args[0] == 0) {
                searchIds[args[1]] = true;
            }
        });
    }

    //not used
    function _dealSearch(searchId, contacts, type) {
        if (!searchIds[searchId]) {
            return;
        }

        if (!contacts || contacts.length <= options.count) {
            if (!type) {
                _searchContent(1, 1);
            } else {
                for (var i = 0; i < contacts.length; i++) {
                    searchAllData["" + contacts[i].userId + ""] = contacts[i];
                    searchUser.splice(searchUser.length, 0, contacts[i]);
                }
                _showUserList(type);
            }
        } else {
            for (var i = 0; i < contacts.length; i++) {
                searchAllData["" + contacts[i].userId + ""] = contacts[i];
                searchUser.splice(searchUser.length, 0, contacts[i]);
            }
            _showUserList(type);
        }

        if (contacts.length > 0) {
            $list.show(0);
        } else {
            hideSearchBox();
        }
    }

    function _showUserList(type) {
        $list.find("a.more").remove();
        for (var i = 0; i < searchUser.length; i++) {
            if (i + 1 > options.count) {
                break;
            }
            if($items.find("li[data-userid="+searchUser[i].userId+"]").length){
                continue;
            }
            var text = searchUser[i].displayName || searchUser[i].account;
            var value = searchUser[i].userId;
            var avatarNode = '<img src="../images/avatar_default.png" class="avatar round-image" />';
            if (searchUser[i].localAvatar) {
                avatarNode = uc.util.AvatarUtil.composeContactAvatar(searchUser[i].localAvatar, uc.constants.UI.DefaultLocalUrl.Conversation.Avatar);
            } else {
                if (searchUser[i].remoteAvatarUrl) {
                    userAvatar["" + value + ""] = searchUser[i].remoteAvatarUrl;
                }
            }
            var position = searchUser[i].departmentName + " " + searchUser[i].position;
            var html = HTML;
            html = html.replace(/{NAME}/i, text).replace(/{DEPARTMENT}/i, position);
            var $item = $(html);
            $item.prepend(avatarNode);
            $item.attr("data-userId", value).attr("data-userAccount", searchUser[i].account);
            $item.find(".userName").text(text);
            $item.find(".staff-post span").text(position);
            $item.appendTo($items);
        }
        searchUser.splice(0, i);
        if (searchUser.length) {
            $("<a href='javascript:void(0)' class='more'></a>").text(uc_resource.Meeting.MoreSearchCheck).attr("type", type).appendTo($items);
        }
        if ($(".userDropList").length < options.count) {
            $list.height($(".userDropList").length * 60);
        } else {
            $list.height(options.count * 60 + 30);
        }
        _loadUserAvatar(userAvatar);
    }

    function _loadUserAvatar(userAvatar) {
        for (var id in userAvatar) {
            uc.IUIService.downloadUserAvatar(id, userAvatar[id]);
        }
        userAvatar = {};
    }

    function _insertUserAvatar(avatarPath, userId) {
        if ($items && $items.length && searchAllData["" + userId + ""]) {
            searchAllData["" + userId + ""].localAvatar = avatarPath;
            $items.find("li[data-userId=" + userId + "]").find("img").attr("src", avatarPath).attr("onerror", "uc.util.ImageUtil.OnImageLoadError(this, '../images/avatar_default.png')");
        }
    }

    function _renderList() {
        var $list = $("<div class='searchdown-list searchBox'><div class='items' id='searchList'></div></div>");
        var width;
        if (options.width) {
            $list.width(options.width);
        }
        else {
            if ($target.width() > 250) {
                $list.width($target.width());
            }
            else {
                $list.width(250);
            }
        }

        var $items = $list.find("div.items");
        if (options.left === false) {
            $items.addClass("right");
        }
        $list.appendTo($("body"));

        return $list;
    }

    function hideSearchBox() {
        if (timeoutId) {
            clearTimeout(timeoutId);
        }
        searchedContent = "";
        $("div.searchBox").hide(100);
    }

    function _bindEvent() {
        var miniHeight = $list.height();
        //$list.css({ "top": 15 + "px", "left": 200 + "px" });
        var top = 0;
        var maxHeight = options.count * 60;

        $target.keyup(function (e) {
            var $this = $(this),
                content = $this.val(),
                position = $this.offset();

            if (!content || searchedContent == content) {
                if (!content) {
                    hideSearchBox();
                }

                return;
            }

            //回车/分号键
            if (e.which == 13 || e.which == 186) {
                _inputEnd();
                return;
            }
            var expend_len = $list.width()+position.left;
            var scrollwidth =15;
            var win_len = $(window).width()-scrollwidth;//scorll offset width  length
            if(expend_len >win_len)
            {
                position.left -= expend_len-win_len+scrollwidth ;
            }
            $list.css({ left: position.left + "px", top: (position.top + $target.height() + 5) + "px" });
            $list.children().empty();
                        
            searchedContent = content;

            clearTimeout(timeoutId);
            timeoutId = setTimeout(function () {
                $("#searchList").html("");
                searchUser = [];
                searchAllData = [];

                page = 1;
                    
                _searchContent(1, 0);
                
            }, 300);
        }).keydown(function (e) {
            //向上/向下键
            if (e.which == 38 || e.which == 40) {
                e.preventDefault();
                return;
            }
        });

        $("body").keyup(function (e) {
            if ($list.css("display") != "block") {
                return;
            }
            //回车键
            if (e.which == 13) {
                _inputEnd();
                return;
            }
            //向上/向下键
            if (e.which == 38 || e.which == 40) {
                _navigateList(e.which == 38);
                return;
            }
        });

        //$target.focus(function () {
        //    if ($(".dropdown-list .items").find("li").length) {
        //        $(".dropdown-list").show(100);
        //    }
        //});

        $list.on("click", "a.more", function (e) {
            e.stopPropagation();

            var type = parseInt($(this).attr("type"));
            if (searchUser.length > options.count) {
                _showUserList(type);
            } else if (searchUser.length == options.count) {
                _showUserList(type);
                page++;
                _searchContent(page, type);
            } else {
                if (!type) {
                    _showUserList(type);
                    _searchContent(1, 1);
                } else {
                    _showUserList(type);
                }
            }

        });

        $list.on("click", "li.userDropList", function () {
            searchedContent = "";

            selectedUser = searchAllData["" + $(this).attr("data-userId") + ""];
            $list.find("li.selected").removeClass("selected");
            $(this).addClass("selected");
            hideSearchBox();
            options.selectedUser(selectedUser);
        });

        $("body").click(function (e) {
            hideSearchBox();
        });
    }


    uc.modules.component.SearchBox = {
        init: function (target, option) {
            initSearchBox = true;
            userAvatar = {};
            page = 1;
            searchUser = [];
            selectedUser = {};
            searchAllData = {};
            if (!target) {
                return;
            }
            if (target.jquery) {
                $target = target;
            }
            else {
                $target = $("#" + target);
            }

            $target.attr("maxlength", "100");

            if (!option.count) {
                option.count = 5;
            }
            options = $.extend({}, option);

            //var $display = $("<span></span><a href='javascript:void(0)' class='dropdown-list-nav'>*</a>").appendTo($target);
            $list = _renderList();
            $items = $list.find("div.items");

            _bindEvent($list);
        },

        deal: function (searchId, contacts, type) {
            _dealSearch(searchId, contacts, type);
        },

        getSelectedValue: function () {
            return selectedUser;
        },

        updateAvatar: function (avatarPath, userId) {
            _insertUserAvatar(avatarPath, userId);
        }
    }
})(uc);


(function (uc) {
    uc.modules.component.SearchBox.SearchBoxEventListener = function () {

    };

    jQuery.extend(uc.modules.component.SearchBox.SearchBoxEventListener.prototype,
        uc.interfaces.ISearchEvent,
        uc.interfaces.IUIEvent, {
            OnLocalSearchResultReceived: function (errorCode, requestId, contacts, groups) {
                if (errorCode == 0) {
                    uc.modules.component.SearchBox.deal(requestId, contacts, 0);

                    //for (var i in dealUsers) {
                    //    if (dealUsers[i].email ) {
                    //
                    //    }
                    //}
                }
            },
            OnServerSearchResultReceived: function (errorCode, searchId, contacts, projects, groups, cloudGroups, meetings, messages) {
                if (errorCode == 0) {
                    uc.modules.component.SearchBox.deal(searchId, contacts, 1);
                }
            },
            OnUserAvatarDownloaded: function (userId, serverAvatarUrl, localAvatarUrl) {
                uc.modules.component.SearchBox.updateAvatar(localAvatarUrl, userId);
            }
        });
})(uc);
