appModule.directive("noticeItem",["$translate",function(translate){
    return{
        templateUrl: "templates/notice_item.html",
        restrict: "E",
        scope:{
          convertTime:"&",
          n:"=",
          hideOptions:"@",
          showPreview:"&",
          showPublishScope:"&",
          showPublish:"&",
          forward:"&",
          deleteNotice:"&",
          needShowPublishScope:"&"
        },
        link:function($scope){
          $scope.checkTopic = function(topic){
            if(!topic){
              return translate.instant("notice.no-topic");
            }

            return topic;
          };
          $scope.checkSummary = function(summary){
            if(!summary){
              return translate.instant("notice.no-summary");
            }
            return summary;
          }
        }
    };
}]);
