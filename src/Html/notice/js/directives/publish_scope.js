appModule.directive("publishScope", ['$timeout','utils','pages','$translate',"log",function($timeout,utils,pages,translate,log) {
    return {
        templateUrl: 'templates/publish_scope.html',
        restrict: "E",
        replace: true,
        scope: {
            deleteKeyDown: "=",
            selectedSections: "=",
            rootSections: "=",
            showDefaultSection:"=",
            convertDefaultSections:"=",
            rootorg:"="
        },
        link: function($scope, element, attrs) {
            $scope.sectionMousehover = function(isHover, e) {
                if (!isHover) {
                    e.currentTarget.className = e.currentTarget.className.replace('selectedSection_last_hover', '').replace('selectedSection_hover', '');
                    return;
                }

                e.currentTarget.className = e.currentTarget.className + ' selectedSection_hover';
            };
            $(element).on("keyup","#searchInput",function(e){
                var top = $(this).offset().top;
                $(".searchrlt",element).css("top", top+30);
            });
            $scope.setSearchInputFocus = function(e){
                if($(e.target).closest(".selectedSection").length == 0){
                var inputBox = $("#searchInput", element)[0];
                if(inputBox){
                    setTimeout(function(){
                        inputBox.focus();
                    });
                    }
                }
            };
            $scope.isSelectAll = false;
            $scope.selectionText = translate.instant("publish.sections.all");
            $scope.sections = [];
            //$scope.selectedSections = [];

            $scope.sections.push({
                type: "0",
                sections: $scope.rootSections,
                text: translate.instant("publish.sections.all"),
                selected: false
            });

            $scope.sections.push({
                type: "1",
                text: translate.instant("publish.sections.custom"),
                selected: false
            });

            //--------------------- 处理搜索框获取焦点和失去焦点的样式  --------------------//
            $scope.onSearchInputFocus = function(){
                $(".sectionSelection",element).addClass("focus");
            };
            $scope.onSearchInputBlur = function(){
                $(".sectionSelection",element).removeClass("focus");
            };
            //-------------------------------------------------------------------------//
            $scope.onSelectSections = function(sections) {
                utils.applyScope($scope, function() {
                    startSelectedItem = 0;
                    $scope.selectedSections = sections;
                });
            };

            $scope.selectAll = function(){
                if(!$scope.isSelectAll){
                    $scope.isSelectAll = true;
                    $scope.selectionText = translate.instant("publish.sections.cancel");
                    $scope.onSelectedSectionChanged($scope.sections[0]);

                }else{
                    $scope.isSelectAll = false;
                    $scope.selectionText = translate.instant("publish.sections.all");
                    $scope.selectedSections = [];
                }
            };

            $scope.isNodeSelected = function(nd, isUser) {
                var id=nd.id;
                // nd.nodeCode = nd.node_code;
                if(isUser){
                    id = 'user' + nd.id;
                }
                var index = $scope.selectedSections.findIndex(function(n){
                    return n.id == id;
                });
                if (index < 0) {
                    return checkSearchNodeSelected(nd,id);
                }
                return 4;
            };

            //根据已选节点来判断搜索出的节点的状态
            function checkSearchNodeSelected(nd,ndID){
                var isSelected = 0;
                var nodeCodes="";
                if(!!nd.nodeCode&&nd.nodeCode.length>0){
                    nodeCodes = nd.nodeCode.split('-');
                }
                for(var i=0;i<$scope.selectedSections.length;i++){

                    if(ndID == $scope.selectedSections[i].id){
                        isSelected = 4;
                        break;
                    }
                    if(nodeCodes.indexOf($scope.selectedSections[i].id.toString()) > -1){
                        isSelected = 4;
                        break;
                    }
                }

                return isSelected;
            }

            $scope.addSelection = function(isSelected,item,isUser){
                if(!isSelected){
                    log.logInfo("search result is selected:"+JSON.stringify(item));
                    return;
                }
                if(!isUser){
                    var newArray = [];
                    $scope.selectedSections.forEach(function(section){
                        if(section.nodeCode){
                            var nodeCodes = section.nodeCode.split('-');
                            if(nodeCodes.indexOf(item.id.toString())> -1){
                                return;
                            }
                        }
                        newArray.push(section);
                    });
                    $scope.selectedSections = newArray;
                }
                $scope.selectedSections.push({
                    id: isUser?"user"+item.id: item.id,
                    text: item.name,
                    account: item.account,
                    position:item.position,
                    avatar:item.avatar,
                    isPerson:isUser,
                    nodeCode:item.nodeCode
                });
            };

            $scope.batchAddSelection = function(){
                if($scope.isSelectAll){
                    return;
                }
                $scope.onSelectedSectionChanged($scope.sections[1]);
            };

            $scope.getRootOrg = function(params, cb) {
                cb && cb($scope.rootSections);
            };

            $scope.isMouseOverSectionOptions = false;
            $scope.toggleSection = function(e) {
                $scope.isSectionShow = !$scope.isSectionShow;
            };
            $scope.hideSection = function() {
                if (!$scope.isMouseOverSectionOptions) {
                    $scope.isSectionShow = false;
                }
            };

            $scope.onSelectedSectionChanged = function(section) {
                $scope.toggleSection();
                //显示发布范围弹窗
                if (section && section.type == 1) {
                    var tempSections = $scope.selectedSections;
                    if(tempSections[0] && tempSections.length> 0 && tempSections[0].type == 0){
                        tempSections = [];
                    }
                    localStorage['notice_choose_contacts'] = JSON.stringify(tempSections);
                    pages.openModalDialog('contacts.html', 'contactsgroup', 600, 620, $scope, {
                      modal:true,
                      caption:[{
                          left: 300,
                          top: 5,
                          width: 260,
                          height: 40
                      }]
                    });
                } else {
                     $scope.selectedSections = [].concat(section.sections);
                    //$scope.selectedSections = [section];
                }
            };

            $scope.deleteKeyDown = function(e) {
                if(element.find(e.target).length == 0){
                    return;
                }
                var deleteSections = [];
                $scope.selectedSections.forEach(function(st, index) {
                    if (st.isSelected) {
                        deleteSections.push({
                            id: st.id
                        });
                    }
                });
                deleteSections.forEach(function(val) {
                    $scope.removeSelectedSection(val);
                });
            }

            $scope.onSelectOrgItem = function(e, section) {
                e && e.stopImmediatePropagation();
                e && e.preventDefault();
                if (!section) {
                    if (e.currentTarget == document.activeElement) {
                        return;
                    }
                    startSelectedItem = 0;
                    $scope.selectedSections.forEach(function(st, index) {
                        st.isSelected = false;
                    });
                    return;
                }
                if (e.shiftKey) {
                    var range = $scope.selectedSections.indexOf(section) - startSelectedItem;
                    $scope.selectedSections.forEach(function(st, index) {
                        if (((index - startSelectedItem) >= 0 && (index - startSelectedItem) <= range) ||
                            ((index - startSelectedItem) <= 0 && (index - startSelectedItem) >= range)) {
                            st.isSelected = true;
                        } else {
                            st.isSelected = false;
                        }
                    });
                } else if (e.ctrlKey) {
                    section.isSelected = !section.isSelected;
                } else {
                    startSelectedItem = $scope.selectedSections.indexOf(section);
                    var countSelected = 0;
                    $scope.selectedSections.forEach(function(st, index) {
                        if (st == section) {
                            return;
                        }
                        st.isSelected && countSelected++;
                        st.isSelected = false;
                    });
                    if (!section.isSelected || (section.isSelected && countSelected > 0)) {
                        section.isSelected = true;
                    } else {
                        section.isSelected = false;
                    }
                }

            }

            //删除选中部门
            $scope.removeSelectedSection = function(section, e) {
                e && e.stopImmediatePropagation();
                e && e.preventDefault();
                var i = 0;
                for (i = $scope.selectedSections.length - 1; i >= 0; i--) {
                    if ($scope.selectedSections[i].id == section.id) {
                        $scope.selectedSections.splice(i, 1);
                        return;
                    }
                }
            };

            var startSelectedItem = 0;

            var stopWatchRootSection = $scope.$watch("rootSections",function(newV,oldV){
                if(newV && newV != oldV){
                    $scope.sections[0].sections = newV;
                    //$scope.selectedSections = [];
                }
            });

            var stopWatchSelectedSection = $scope.$watch("selectedSections",function(newV,oldV){
                if(newV && newV.length> 0){
                    $scope.allSections = $scope.sections[0].sections;
                    for(var i=0;i<$scope.allSections.length;i++){
                        var index = newV.findIndex(function(section){
                            return section.id == $scope.allSections[i].id;
                        });
                        if(index < 0){
                            $scope.isSelectAll = false;
                            $scope.selectionText = translate.instant("publish.sections.all");
                            return;
                        }
                    }
                    $scope.isSelectAll = true;
                }else{
                    $scope.isSelectAll = false;
                }
                if($scope.isSelectAll){
                    $scope.selectionText = translate.instant("publish.sections.cancel");
                }else{
                    $scope.selectionText = translate.instant("publish.sections.all");
                }
            },true);

            $scope.$on("$destroy",function(){
                stopWatchRootSection();
                stopWatchSelectedSection();
            });
        }
    }
}]);
