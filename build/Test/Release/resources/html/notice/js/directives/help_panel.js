appModule.directive("helpPanel",["$translate","utils",function(translate,utils){
    return{
        templateUrl: "templates/help_panel.html",
        restrict: "E",
        link:function($scope,$element){
            var offDocumentClick = $scope.$on("onDocumentClick",function(e,uiEvent){
                if($(uiEvent.target).closest(".help_panel").length == 0){
                    utils.applyScope($scope,function(){
                        $scope.isShowPanel = false;
                    });
                }
            });

            $scope.sections = [{
                    name:translate.instant("common.help.getting-started.name"),
                    list:[{
                        name:translate.instant("common.help.getting-started.list.what-is.title"),
                        detail:translate.instant("common.help.getting-started.list.what-is.detail")
                    },{
                        name:translate.instant("common.help.getting-started.list.how-to-create-message.title"),
                        detail:translate.instant("common.help.getting-started.list.how-to-create-message.detail")
                    },{
                        name:translate.instant("common.help.getting-started.list.how-to-get-account.title"),
                        detail:translate.instant("common.help.getting-started.list.how-to-get-account.detail")
                    },{
                        name:translate.instant("common.help.getting-started.list.why-several-editors.title"),
                        detail:translate.instant("common.help.getting-started.list.why-several-editors.detail")
                    },{
                        name:translate.instant("common.help.getting-started.list.how-to-receive-message.title"),
                        detail:translate.instant("common.help.getting-started.list.how-to-receive-message.detail")
                    }]
                },{
                name:translate.instant("common.help.sending-message.name"),
                list:[{
                    name:translate.instant("common.help.sending-message.list.can-i-change-account.title"),
                    detail:translate.instant("common.help.sending-message.list.can-i-change-account.detail")
                },{
                    name:translate.instant("common.help.sending-message.list.can-i-modify-account.title"),
                    detail:translate.instant("common.help.sending-message.list.can-i-modify-account.detail")
                },{
                    name:translate.instant("common.help.sending-message.list.select-account-scope.title"),
                    detail:translate.instant("common.help.sending-message.list.select-account-scope.detail")
                },{
                    name:translate.instant("common.help.sending-message.list.batch-select-scope.title"),
                    detail:translate.instant("common.help.sending-message.list.batch-select-scope.detail")
                },{
                    name:translate.instant("common.help.sending-message.list.filter-select-scope.title"),
                    detail:translate.instant("common.help.sending-message.list.filter-select-scope.detail")
                },{
                    name:translate.instant("common.help.sending-message.list.quit-reference.title"),
                    detail:translate.instant("common.help.sending-message.list.quit-reference.detail")
                },{
                    name:translate.instant("common.help.sending-message.list.modify-hyper-link.title"),
                    detail:translate.instant("common.help.sending-message.list.modify-hyper-link.detail")
                },{
                    name:translate.instant("common.help.sending-message.list.modify-attachment.title"),
                    detail:translate.instant("common.help.sending-message.list.modify-attachment.detail")
                },{
                    name:translate.instant("common.help.sending-message.list.save-cover.title"),
                    detail:translate.instant("common.help.sending-message.list.save-cover.detail")
                }]
            }, {
                name: translate.instant("common.help.message-center.name"),
                list: [{
                    name: translate.instant("common.help.message-center.list.what-is.title"),
                    detail: translate.instant("common.help.message-center.list.what-is.detail")
                },{
                    name: translate.instant("common.help.message-center.list.how-to-search.title"),
                    detail: translate.instant("common.help.message-center.list.how-to-search.detail")
                },{
                    name: translate.instant("common.help.message-center.list.search-for.title"),
                    detail: translate.instant("common.help.message-center.list.search-for.detail")
                },{
                    name: translate.instant("common.help.message-center.list.search-criteria.title"),
                    detail: translate.instant("common.help.message-center.list.search-criteria.detail")
                },{
                    name: translate.instant("common.help.message-center.list.how-to-see-detail.title"),
                    detail: translate.instant("common.help.message-center.list.how-to-see-detail.detail")
                }]
            },{
                name: translate.instant("common.help.setting.name"),
                list: [{
                    name: translate.instant("common.help.setting.list.update-avatar.title"),
                    detail: translate.instant("common.help.setting.list.update-avatar.detail")
                },{
                    name: translate.instant("common.help.setting.list.update-name.title"),
                    detail: translate.instant("common.help.setting.list.update-name.detail")
                },{
                    name: translate.instant("common.help.setting.list.editor-change.title"),
                    detail: translate.instant("common.help.setting.list.editor-change.detail")
                }]
            }
            ];

            $scope.toggleHelpPanel = function(e){
                e.stopPropagation();
                $scope.isShowPanel = !$scope.isShowPanel;

            };

            $scope.changeSection = function(section){
                $scope.selectedSection = section;
                $scope.isShowList = true;
            };

            $scope.changeSectionItem = function(item){
                $scope.selectedItem = item;
                $scope.isShowList = false;
            };

            $scope.changeSection($scope.sections[0]);

            $scope.$on("$destroy",function(){
                offDocumentClick();
            });
        }
    };
}]);
