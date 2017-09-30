appModule.directive("noticeSections", ['$timeout',function($timeout){
    return{
        templateUrl: "templates/notice_sections.html",
        restrict: "E",
        scope: {
            sections:"=",
            cover:"="
        },
        link: function($scope,element){
            var prefix = "发送给：";
            var showMoreMsg = "详细信息";
            var hideMoreMsg = "详细信息";
            var stopWatch = $scope.$watch("sections",function(newV,oldV){
                 if(!!newV && Array.isArray(newV)){
                   var sections = newV;
                    var rawTxt = prefix;
                    $.each(sections,function(i,v){
                      if(i<sections.length-1){
                        rawTxt+=v.text+"，";
                      }else{
                        rawTxt+=v.text;
                      }
                    });

                    $scope.rawText = rawTxt;
                    $scope.menuText = showMoreMsg;
                    $timeout(function(){
                      if(needShowMore()){
                        $timeout(function(){
                          $(element).find(".displayText").removeClass('force-hide');
                          var fullText = $(element).find(".fullText");
                          var position = $(element).find(".menuBtn").position();
                          var menuWidth = $(element).find(".menuBtn").width();
                          var menuHeight = $(element).find(".menuBtn").height();
                          fullText.css('top',position.top+menuHeight+5);
                          if(!$scope.cover.url){
                            fullText.addClass("no-cover");
                          }else{
                            fullText.removeClass("no-cover");
                          }
                        });
                      }else{
                        $(element).find(".displayText").addClass('force-hide');
                      }
                      $scope.showDisplayInfo = false;
                    });
                 }
            });

            function needShowMore(){
              var rawDiv = $(element).find(".rawText").removeClass("hide");
              rawDiv.removeClass("visible");
              if(rawDiv[0].clientHeight == rawDiv[0].scrollHeight){
                  rawDiv.addClass("visible");
                  return false;
              }
              rawDiv.addClass("hide");
              return true;
            }

            $scope.toggleDisplayInfo = function(){
              $scope.showDisplayInfo = !$scope.showDisplayInfo;
              $scope.menuText = $scope.showDisplayInfo?hideMoreMsg : showMoreMsg;
            };

            $scope.$on("$destroy",function(){
                stopWatch();
            });
        }
    }
}]);
