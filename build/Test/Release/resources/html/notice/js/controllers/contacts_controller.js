'use strict';
appModule.controller('ContactsCtrl', ['$scope', '$rootScope','pages','utils','log',"notices", function($scope, $rootScope,pages,utils,log,notices) {

    $scope.hideContacts = function() {
        $scope.$destroy();
        pages.closeWindow();
    }

    $scope.selectedCount = 0;
    /**All the departments info array.**/
    $scope.departments = [];

    /**Selected departments;**/
    $scope.selectedNodes = [];

    $scope.windowName = "contactsgroup";
    pages.attachEventScope($scope.windowName+"Window",$scope);
    var originalSelected = JSON.parse(localStorage['notice_choose_contacts']) || [];
    var orgInfoCache = {};
    var allNodes = {};
    $scope.isNodeSelected = function(nd, isUser) {
        var id=nd.id;
        // nd.nodeCode = nd.node_code;
        if(isUser){
            id = 'user' + nd.id;
        }
        if (!allNodes[id]) {
            return checkSearchNodeSelected(nd,id);
        }
        return allNodes[id].isSelected;
    };
    $scope.fnCall = function(fnName, params, cb) {
        pages.fnCall($scope.windowName, fnName, params, function(data) {
            utils.applyScope($scope, function() {
                cb && cb(data);
            });
        });
    };
    $scope.rootorg = [];
    $scope.init = function(){
        var selectedAccounts = notices.retrieveSelectedAccount();
        var data = selectedAccounts.sectionIDs;
        log.logInfo('Initial the tree,the root ids:' + JSON.stringify(data));
        if (data) {
            originalSelected.forEach(function(dept){
                var sltNode = convertToSltNode(dept);
                sltNode.name = dept.text;
                sltNode.isSelected = 1;
                $scope.selectedNodes.push(sltNode);
            });
            data.forEach(function(val) {
                $scope.rootorg.push(val.id);
                var node = {
                    parentNode: null,
                    level: 1,
                    name: val.text,
                    id: val.id,
                    idChain: val.id,
                    parentId: 0,
                    isExpand: false,
                    childCount: 0,
                    countCheckChild: 0,
                    isSelected: 0,
                    nodeCode:""
                };
                allNodes[node.id] = node;
                node.children = [];
                $scope.departments.push(node);
                checkAreadySelected(node);
                // $scope.expandNode(node);
            });
            if($scope.departments.length == 1){
                $scope.expandNode($scope.departments[0]);
            }
        }
    }

    $scope.expandNode = function(node) {
        if (node.isExpand) {
            node.isExpand = false;
        } else {
            node.isExpand = true;
            if (!node.children || node.children.length == 0) {
                loadOrgInfo(node.id, function(data) {
                    node.children = formatUserData(data.staffs, node);
                    node.children = node.children.concat(formatDeparmentData(data.subDepartments, node));
                });
            }
        }
    };
    var orgInfoCbs = {};

    function loadOrgInfo(id, cb) {
        if (orgInfoCache[id]) {
            cb && cb(orgInfoCache[id]);
        } else {
            orgInfoCbs[id] = orgInfoCbs[id] || [];
            orgInfoCbs[id].push(cb);

            if (orgInfoCbs[id].length > 1) {
                //不用重新发送请求
                return;
            }
            notices.getOrgInfo(id,function(data) {
                orgInfoCache[id] = data;
                log.logInfo('load '+id+' org info:' + JSON.stringify(data));
                utils.applyScope($scope, function() {
                    if (orgInfoCbs[id]) {
                        orgInfoCbs[id].forEach(function(oneCb) {
                            oneCb && oneCb(data);
                        });
                        orgInfoCbs[id] = [];
                    }
                });
            });
        }
    }
    $scope.removeNode = function(e, index) {
        if (index >= 0) {
            var node = $scope.selectedNodes[index];
            if(allNodes[node.id] && allNodes[node.id] != node){
              allNodes[node.id].isSelected = node.isSelected;
              node = allNodes[node.id];
            }

            $scope.selectNode(null, node);
        }
    };
    $scope.searchSelectNode = function(isSelect, item,isUser) {
        var id = item.id;
        if(isUser){
            id = 'user' + item.id;
        }
        var node =null;
        node = allNodes[id];
        if(!node){
          node = $scope.selectedNodes.find(function(sd){
            return sd.id == id;
          });
        }
        if(!node && isSelect){
           node = {
             id:id,
             isSelected: isSelect,
             name:item.name,
             account: item.account,
             position:item.position,
             avatar:item.avatar,
             isPerson:id.toString().startsWith("user"),
             nodeCode:item.nodeCode
           };
           $scope.selectedNodes.push(node);
           return;
        }

        node.isSelected = isSelect ? 0 : 1;
        $scope.selectNode(null, node);
        item.isSelected = node.isSelected; //更新搜索结果节点状态
    };

    $scope.selectNode = function(e, node) {
        if(e){
           e.stopImmediatePropagation();
           e.preventDefault();
        }
        if (node.isSelected == 3 || node.isSelected == 4) {
            //3已选置灰态，4是未选置灰态，disable 状态，不做处理
            return;
        }
        if (node.isSelected === 0) {
            node.isSelected = 1;
            /**Check all the sub-departments./**/
            node.countCheckChild = node.childCount;
            selectSubNode(1, node.children);
            onParentNodeSelected(node);
            $scope.selectedNodes.push(node);
            onSubNodeSelected(node);
        } else if (node.isSelected == 1) {
            node.isSelected = 0;
            node.countCheckChild = 0;
            /** Unckeck all the sub-departments.**/
            selectSubNode(0, node.children);
            var index = $scope.selectedNodes.findIndex(function(val) {
                if (val.id == node.id) {
                    return true;
                }
            });
            if (index >= 0) {
                var removedNode =  $scope.selectedNodes[index];
                $scope.selectedNodes.splice(index, 1);
                if(removedNode.removedSelectedSubNodes && removedNode.removedSelectedSubNodes.length>0){
                  removedNode.removedSelectedSubNodes.forEach(function(rd){
                    var hasExisted =false;
                    for(var i=0; i<$scope.selectedNodes.length;i++){
                      if($scope.selectedNodes[i].id == rd.id){
                        hasExisted = true;
                      }
                    }
                    if(!hasExisted){
                      $scope.selectedNodes.push(rd);
                    }
                  });
                }
            }
        }
    };


    function selectSubNode(isSelected, nodes) {
        if (!nodes) {
            return;
        }
        nodes.forEach(function(val) {
            if (isSelected === 0) {
                if (val.isSelected == 3) {
                    val.isSelected = 1;
                    $scope.selectedNodes.push(val);
                } else if (val.isSelected == 4) {
                    val.isSelected = 0;
                }
            } else {
                if (val.isSelected === 0) {
                    val.isSelected = 4;
                } else if (val.isSelected == 1) {
                    val.isSelected = 3;
                }
            }
            selectSubNode(val.isSelected, val.children);
        });
    }



    $scope.submit = function() {
        var oSltNodes = [];
        $scope.selectedNodes.forEach(function(val) {
            var index = oSltNodes.findIndex(function(nd) {
                if (val.id == nd.id) {
                    return true;
                }
            });
            if (index < 0) {
                oSltNodes.push(convertToSltNode(val));
            }
        });
        // 暂时不限制部门数和发送人数
        // if (oSltNodes.length > 100) {
        //     $rootScope.$broadcast("onError", '最多只能选择100个部门或人员！');
        //     return;
        // }
        $scope.fnCall('onSelectSections', oSltNodes);
        $scope.hideContacts();
    };
    $scope.init();
    $scope.$on("$destroy",function(){
        pages.detachEventScope($scope.windowName+"Window");
    });

    function formatDeparmentData(dpts, parent) {
        if (!dpts) {
            return [];
        }
        var isSelected = 0;
        if (parent) {
            if (parent.isSelected == 1 || parent.isSelected == 3) {
                isSelected = 4;
            } else {
                isSelected = parent.isSelected;
            }
        }


        var nodesTmp = dpts.map(function(val) {
            var nd = {
                parentNode: parent,
                level: parent ? parent.level + 1 : 1,
                name: val.departmentName,
                id: val.id,
                idChain: parent ? parent.idChain + '-' + val.id : val.id,
                parentId: val.parentId,
                isExpand: false,
                isSelected: isSelected
            };
            checkAreadySelected(nd);
            if(!parent.nodeCode){
              parent.nodeCode ="";
            }
            nd.nodeCode=parent.nodeCode+"-"+parent.id;
            allNodes[nd.id] = nd;
            return nd;
        });
        if (parent) {
            parent.countCheckChild = parent.isSelected ? nodesTmp.length : 0;
            parent.childCount = nodesTmp.length;
        }
        return nodesTmp;
    }

    function formatUserData(staffs, parent) {
        if (!staffs) {
            return [];
        }
        var isSelected = 0;
        if (parent) {
            if (parent.isSelected == 1 || parent.isSelected == 3) {
                isSelected = 4;
            } else {
                isSelected = parent.isSelected;
            }
        }

        var nodesTmp = staffs.map(function(val) {
            var nd = {
                parentNode: parent,
                level: parent ? parent.level + 1 : 1,
                name: val.displayName,
                position: val.position,
                id: 'user' + val.userId,
                idChain: parent ? parent.idChain + '-' + val.id : val.id,
                parentId: val.parentId,
                isExpand: false,
                isPerson: true,
                account:val.account,
                avatar:val.remoteAvatarUrl ? val.remoteAvatarUrl : 'theme/images/icon_avatar_default_40.png',
                isSelected: isSelected
            };
            checkAreadySelected(nd);
            if(!parent.nodeCode){
              parent.nodeCode ="";
            }
            nd.nodeCode=parent.nodeCode+"-"+parent.id;
            allNodes[nd.id] = nd;
            return nd;
        });
        return nodesTmp;
    }

    function convertToSltNode(val){
      return {
          id: val.id,
          text: val.name,
          account: val.account,
          position:val.position,
          avatar:val.avatar,
          isPerson:val.id.toString().startsWith("user"),
          nodeCode:val.nodeCode
      };
    }

    //检查当前展开的节点是否存在于已选列表中
    function checkAreadySelected(nd){
      if(nd.isSelected === 0){
          $scope.selectedNodes.forEach(function(d){
            if(d.id == nd.id){
              nd.isSelected = 1;
            }
        });
      }
    }

    //当父节点选中后，将所有已选子节点从已选列表中移除,并将这些移除节点保存在父节点
    //保存的目的是处理一种场景：当父节点还未展开，但子节点在前一次选择时被选中，这时候未建立父子关系
    //一旦父节点从已选列表移除，将从父节点取出这些子节点放入移除节点.
    function onParentNodeSelected(nd){
      var tempNodes = [];
      nd.removedSelectedSubNodes = null;
      $scope.selectedNodes.forEach(function(sd){
         if(!!sd.nodeCode && sd.nodeCode.length>0){
           var nodeCodes = sd.nodeCode.split('-');
           if(nodeCodes.indexOf(nd.id.toString()) == -1){
             tempNodes.push(sd);
           }else{
             if(!nd.removedSelectedSubNodes){
               nd.removedSelectedSubNodes =[];
             }
             nd.removedSelectedSubNodes.push(sd);
           }
         }else{
           tempNodes.push(sd);
         }
      });
      $scope.selectedNodes = tempNodes;
    }

    //处理所有子节点选中后，父节点选中并disable所有子节点
    function onSubNodeSelected(nd){
      if(!nd.parentNode || nd.parentNode.isSelected){
        return;
      }
      var allSelected = true;
      for(var i=0;i<nd.parentNode.children.length;i++){
        if(nd.parentNode.children[i].isSelected === 0){
          allSelected = false;
          break;
        }
      }
      if(allSelected){
        $scope.selectNode(null,nd.parentNode);
      }
    }

    //根据已选节点来判断搜索出的节点的状态
    function checkSearchNodeSelected(nd,ndID){
      var isSelected = 0;
      var nodeCodes="";
      if(!!nd.nodeCode&&nd.nodeCode.length>0){
        nodeCodes = nd.nodeCode.split('-');
      }
      for(var i=0;i<$scope.selectedNodes.length;i++){

        if(ndID == $scope.selectedNodes[i].id){
          isSelected = 1;
          break;
        }
        if(nodeCodes.indexOf($scope.selectedNodes[i].id.toString()) > -1){
          isSelected = 4;
          break;
        }
      }

      return isSelected;
    }
}]);
