appModule.directive("noticeItemContextMenu",["$translate",function(translate){
    return{
        template: '<div class="noticeItemContext hide">'
                   + '<div class="btn edit" ng-click="hideMe();onEdit({notice:item,$event:$event})" translate="notice.edit" ng-show="!item.id"></div>'
                   + '<div class="btn delete" ng-click="hideMe();onDelete({notice:item,$event:$event})" translate="notice.delete"></div>'
                   + '</div>',
        restrict: "E",
        replace:true,
        scope:{
            item:"=",
            onEdit:"&",
            onDelete:"&"
        },
        link:function($scope,element){
            var offContextMenu = $scope.$on("onContextMenu",function(e,uiEvent){
                if($(uiEvent.target).closest(".noticeItem").length > 0){
                    var parent = $(uiEvent.target).closest(".noticeItem");
                    if(parent.find(element).length>0){
                        getPosition(element,uiEvent);
                        element.removeClass("hide");
                    }else{
                        element.addClass("hide");
                    }
                }else{
                    element.addClass("hide");
                }
            });

            var offDocumentClick = $scope.$on("onDocumentClick",function(e,uiEvent){
                if($(uiEvent.target).closest(".noticeItem").length > 0){
                    var parent = $(uiEvent.target).closest(".noticeItem");
                    if(parent.find(element).length==0){
                        element.addClass("hide");
                    }
                }else{
                    element.addClass("hide");
                }
            });

            $scope.hideMe = function(){
                element.addClass("hide");
                element.removeAttr("style");
            };

            $scope.$on("$destroy",function(){
                offContextMenu();
                offDocumentClick();
            });

            function getPosition(element,e){
                var totalWidth = window.innerWidth, totalHeight = window.innerHeight;
                var left, top, elementWidth = element.width(),elementHeight = element.height();
                if(totalWidth < e.x+elementWidth){
                    left = e.x - elementWidth;
                }else{
                    left = e.x;
                }

                if(totalHeight < e.y + elementHeight){
                    top = e.y - elementHeight;
                }else{
                    top = e.y;
                }
                element.css({
                    left:left,
                    top:top
                })
            }
        }
    };
}]);
