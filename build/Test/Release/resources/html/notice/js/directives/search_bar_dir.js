appModule.directive('searchBar', ['$timeout', 'utils', 'api', function ($timeout, utils, api) {
    return {
        templateUrl: 'templates/search_bar.html',
        restrict: "E",
        replace: true,
        scope: {
            searchType: "=",
            selectNode: "=",
            isNodeSelected: "=",
            rootorg: "=",
            placeholder: "@",
            inputFocus: "&",
            inputBlur: "&",
            isSingleSelect:"@"
        },
        link: function ($scope, element, attrs) {
            $scope.setInputEditorCaretPosition = function ($event) {
                utils.setInputEditorCaretPosition($($event.currentTarget));
            };

            $("#searchInput", element).attr("placeholder", $scope.placeholder);
            $("#searchInput", element).on("focus", function (e) {
                if ($scope.inputFocus) {
                    $scope.inputFocus();
                }
            }).on("blur", function (e) {
                if ($scope.inputBlur) {
                    $scope.inputBlur();
                }
            });

            $scope.onInputCaretPositionKeyUp = function ($event) {
                utils.onInputCaretPositionKeyUp($($event.currentTarget));
                if($event.keyCode == 38){
                    $scope.setSearchItemSelected("up");
                }

                if($event.keyCode == 40){
                    $scope.setSearchItemSelected("down");
                }
            };

            $scope.setSearchItemSelected = function (orientation) {
                var index = -1;
                if(!$scope.isSingleSelect){
                    return;
                }
                if ($scope.contactList.length == 0 && $scope.orgList.length == 0) {
                    return;
                }

                if($scope.selectedResult){
                    if($scope.selectedResult.isUser){
                        index = $scope.contactList.findIndex(function(user){
                            return user.id == $scope.selectedResult.id;
                        });
                    }else{
                        index = $scope.orgList.findIndex(function(org){
                            return org.id == $scope.selectedResult.id;
                        });
                    }

                }
                if (orientation == "up") { //按向上箭头
                    if(index == -1){
                        return;
                    }
                    if($scope.selectedResult.isUser){
                        if(index==0){//当前选择的是第一个搜索到的用户
                            return;
                        }
                        $scope.selectedResult = $scope.contactList[index-1];
                    }else{
                        if(index ==0){//当前选择的是第一个搜索到的组织
                            if($scope.contactList.length>0){
                                $scope.selectedResult = $scope.contactList[$scope.contactList.length-1]; //将选择项设置为当前搜索到用户的最后一个
                            }
                            return;
                        }
                        $scope.selectedResult = $scope.orgList[index-1];
                    }
                } else if(orientation == "down"){
                    if(index == -1){
                        if($scope.contactList.length > 0){
                            $scope.selectedResult = $scope.contactList[0];
                            return;
                        }
                        if($scope.orgList.length > 0){
                            $scope.selectedResult = $scope.orgList[0];
                            return;
                        }
                    }else{
                        if(!$scope.selectedResult.isUser){
                            if(index == $scope.orgList.length-1){  //当前选择的是组织最后一个
                                return;
                            }
                            $scope.selectedResult = $scope.orgList[index+1];
                        }else{
                            if(index == $scope.contactList.length-1){ //当前选择的是用户最后一个
                                if($scope.orgList.length>0){
                                    $scope.selectedResult = $scope.orgList[0];
                                }
                                return;
                            }
                            $scope.selectedResult = $scope.contactList[index+1];
                        }
                    }
                }
            };

            $scope.onSearchInputKeyPress = function($event){
                if($event.keyCode == 13 && $scope.selectedResult){
                    $scope.onContactClick($scope.selectedResult,$scope.selectedResult.isUser);
                    $scope.deleteKeyWord($event);
                }
            };

            $scope.keyWord = '';
            $scope.isShowRlt = false;

            function isSelected(node, isUser) {
                if (!$scope.isNodeSelected) {
                    return 0;
                } else {
                    if (!node.nodeCode && node.node_code) {
                        node.nodeCode = node.node_code;
                    }
                    return $scope.isNodeSelected(node, isUser);
                }
            }

            $scope.deleteKeyWord = function (e) {
                if (e) {
                    e.preventDefault();
                    e.stopImmediatePropagation();
                }

                canceller && canceller.resolve();
                canceller2 && canceller2.resolve();

                $scope.keyWord = '';
                $scope.isSearchInputActive = false;
                $scope.isShowRlt = false;
            };
            document.addEventListener('click', function (e) {
                if (e.target.closest('.header .searchbar-dir') || e.target.closest('.header .searchbar') || e.target.closest('.list_header')) {
                    return;
                }
                $scope.$apply(function () {
                    $scope.deleteKeyWord();
                });

            });

            $scope.showSearchRlt = function (e) {
                //   e.stopPropagation();
                e.stopImmediatePropagation();
                if (!$scope.isSearchInputActive) {
                    e.currentTarget.getElementsByTagName('input')[0].focus();
                    $scope.isSearchInputActive = true;
                    return;
                }
            }
            $scope.isSearchInputActive = false;

            $scope.onContactClick = function (item, isUser) {
                if (item.isSelected > 1) {
                    return;
                }
                item.isSelected = !item.isSelected;
                if ($scope.selectNode) {
                    $scope.selectNode(item.isSelected, item, isUser);
                    $scope.updateSearchResultStatus();
                }
            };

            //当搜索结果被点击后，更新所有搜索结果状态
            $scope.updateSearchResultStatus = function () {
                $scope.contactList.forEach(function (user) {
                    user.isSelected = isSelected(user, true);
                });
                $scope.orgList.forEach(function (org) {
                    org.isSelected = isSelected(org);
                });
            };

            $scope.showDetail = function (id) {
                $scope.deleteKeyWord();
                show_info(id, 'timenews');
            };

            var backupData = {};
            $scope.isSinglePage = false;
            $scope.backToRlt = function () {
                $scope.isSinglePage = false;
                $scope.contactList = backupData.contacts;
                $scope.timenewsList = backupData.timenews;
                $scope.orgList = backupData.orgList;
            }

            $scope.showSinglePage = function (type) {
                $scope.isSinglePage = true;
                backupData.contacts = $scope.contactList;
                backupData.timenews = $scope.timenewsList;
                backupData.orgList = $scope.orgList;
                clearAllRlt();
                $scope.search(type);
            }
            var canceller;
            var canceller2;
            var searchTimeOut;
            $scope.contactList = [];
            $scope.orgList = [];
            $scope.search = function (type) {
                $scope.selectedResult = null;
                $("#searchInput", element).removeAttr("placeholder");
                if (type && type == 'contact') {
                    $scope.searchUser(100);

                } else if (type && type == 'orgs') {
                    $scope.searchOrg(100);
                } else {
                    $scope.isSinglePage = false;
                    searchTimeOut && $timeout.cancel(searchTimeOut);
                    if (!$scope.keyWord) {
                        $scope.isShowRlt = false;
                        return;
                    }
                    searchTimeOut = $timeout(function () {
                        if ($scope.searchType == 1) {
                            $scope.searchUser(6);
                        } else if ($scope.searchType == 2) {
                            $scope.searchOrg(6);
                        } else {
                            $scope.searchOrg(6);
                            $scope.searchUser(6);
                        }
                    }, 500);
                }
            };

            $scope.searchOrg = function (num) {
                api.search("search_org", $scope.keyWord, $scope.rootorg, function (data) {
                    utils.applyScope($scope, function () {
                        $scope.isShowRlt = true;
                        parseOrgData(num, data);
                    });
                }, function () {
                });
            };

            $scope.searchUser = function (num) {
                api.search("search_user", $scope.keyWord, $scope.rootorg, function (data) {
                    utils.applyScope($scope, function () {
                        $scope.isShowRlt = true;
                        parseOrgUserData(num, data);
                    });
                }, function () {
                });
            };

            function parseOrgData(count, data) {
                $scope.orgList = [];
                var org = null;
                for (var i = 0; i < data.length; i++) {
                    if (i == count) {
                        return;
                    }
                    org = data[i];
                    var orginzation = {
                        id: org.org_id,
                        name: org.org_name,
                        nodeCode: org.node_code
                    };
                    orginzation.isSelected = isSelected(orginzation);
                    $scope.orgList.push(orginzation);
                }

            }

            function parseOrgUserData(count, data) {
                $scope.contactList = [];
                var user = null;
                for (var i = 0; i < data.length; i++) {
                    if (i == count) {
                        return;
                    }
                    user = data[i];
                    var usr = {
                        id: user.user_id,
                        name: user.display_name,
                        pinyin: user.pinyin,
                        avatar: user.avatar || 'theme/images/icon_avatar_default_40.png',
                        department: user.org_name,
                        position: user.position,
                        desc: user.org_name + ' ' + user.position,
                        nodeCode: user.node_code,
                        isUser:true
                    };
                    usr.isSelected = isSelected(usr, true);
                    $scope.contactList.push(usr);
                }
            }

            function clearAllRlt() {
                $scope.contactList = [];
                $scope.timenewsList = [];
                $scope.orgList = [];
            }

            function parseData(type, data) {
                clearAllRlt();
                if (!data.data || data.data.code != 0) {
                    return;
                }
                var groups = data.data.data;
                var contactIndex = -1;
                if (!type || type == 'contact') {
                    contactIndex = groups.findIndex(function (val) {
                        if (val.groupValue && val.groupValue == 1) {
                            return true;
                        }
                    });
                }
                if (contactIndex >= 0) {
                    var docs = groups[contactIndex].doclist.docs;
                    docs = docs || [];
                    docs.forEach(function (val) {

                        var user = ucClient.ContactService.getContactInfoSync(val.user_id * 1);
                        if (!user) {
                            return;
                        }
                        var usr = {
                            id: user.userId,
                            name: user.displayName,
                            pinyin: user.pinyin,
                            avatar: user.localAvatar || user.remoteAvatarUrl || 'theme/images/icon_avatar_default_40.png',
                            department: user.departmentName,
                            position: user.position,
                            desc: user.departmentName + ' ' + user.position,
                            nodeCode: user.node_code
                        };
                        usr.isSelected = isSelected(usr, true);
                        $scope.contactList.push(usr);
                    });
                }
            };
        }
    };
}]);
