appModule.directive("scrollContainer",function(){
    return{
        template: "<div class='scrollable' ng-transclude></div>",
        restrict: "E",
        transclude: true,
        link: function($scope,element,attrs){
            element.attr("id",$scope[attrs.id]);
            var showMoreDiv = $(".showMoreDiv",element);
            if(showMoreDiv.length > 0){
              showMoreDiv.on("click",function(){
                if($scope.onScrollUp){
                  $scope.onScrollUp(function(result){
                      if(!result || result.length == 0){
                          showMoreDiv.addClass("force-hide");
                      }
                  });
                }
              });
            }


            var upToUpdate = attrs.upToUpdate;
            $(element).on("scroll",function(){
              if(upToUpdate === "true"){
                if($scope.onScrollUp  && this.scrollTop === 0){
                  $scope.onScrollUp();
                }
                return;
              }
              if(this.clientHeight+this.scrollTop == this.scrollHeight){
                  if($scope.onScrollDown){
                      $scope.onScrollDown();
                  }
              }
            });

            var stopWatchScrollToTop = $scope.$on("scrollToTop",function(e,param){
                if(param == element.attr("id")){
                    $(element).scrollTop(0);
                }
            });

            var offNoticesInit = $scope.$on("onNoticesInit",function(e,id){
                if(upToUpdate === "true"){
                  if(element[0].scrollHeight > element[0].clientHeight){
                    var target = $("#"+id, element);
                    $(element).scrollTop(target.position().top + target.height());
                    showMoreDiv.addClass("force-hide");
                  }else{
                    showMoreDiv.removeClass("force-hide");
                  }
                }
            });

            var offScrollToBottom = $scope.$on("scrollToBottom",function(e,param){
                if(param == element.attr("id")){
                    if(element[0].scrollHeight > element[0].clientHeight){
                      $(element).scrollTop(element[0].scrollHeight);
                      showMoreDiv.addClass("force-hide");
                    }else{
                      showMoreDiv.removeClass("force-hide");
                    }
                }
            });

            $scope.$on("$destroy",function(){
                stopWatchScrollToTop();
                offScrollToBottom();
                offNoticesInit();
            });
        }
    };
});