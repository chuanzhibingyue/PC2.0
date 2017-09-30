appModule.directive("cropper", ["log",function(log) {
    return {
        template: '<img class="cropperImg" ng-src="{{imgSrc}}" style="display:none;"/>',
        restrict: "E",
        replace: true,
        scope: {
            imgSrc: "=",
            cropper: "="
        },
        link: function($scope, element, attrs) {

            $scope.isVerticalImg = false;
            $scope.cropper = $scope.cropper || {};
            var cropper;
            element.on('load', function(e) {
                log.logInfo("after loaded...");
                adjustParentArea(element);
                cropper && cropper.cropper('destroy');
                cropper = element.cropper({
                    zoomable: false,
                    background: false,
                    aspectRatio: 16/9,
                    minContainerWidth:element.parent().width(),
                    minContainerHeight:element.parent().height()
                });
                log.logInfo("after cropper...");
            });

            function adjustParentArea(el){
              var rate = el[0].naturalWidth/el[0].naturalHeight;
              el.parent().css('margin',0);
              if(rate > 1){
                el.parent().width(280);
                el.parent().height(280/rate);
                el.parent().css('margin-top',140-140/rate);
              }else{
                el.parent().width(280*rate);
                el.parent().height(280);
                el.parent().css('margin-left',140-140*rate);
              }
            }

            $scope.cropper.crop = function(croppedImagePath) {
                if (!cropper) {
                    return -1;
                }
                var positionInfo = cropper.cropper('getData');
                var rlt = ucClient.Image.crop( // 成功时，返回0
                    $scope.imgSrc, // 图像路径
                    Math.round(positionInfo.x < 0 ? 0 : positionInfo.x), // 裁剪区域的左上角x坐标
                    Math.round(positionInfo.y < 0 ? 0 : positionInfo.y), // 裁剪区域的左上角y坐标
                    Math.round(positionInfo.width), // 裁剪区域宽度
                    Math.round(positionInfo.height), // 裁剪区域的高度
                    Math.round(positionInfo.width), // 目标宽度
                    Math.round(positionInfo.height), // 目标高度
                    croppedImagePath // 剪裁后图标的存储路径
                );
                if (rlt == 0) {
                    return croppedImagePath;
                } else {
                    return -1;
                }
            }
        }
    }
}]);
