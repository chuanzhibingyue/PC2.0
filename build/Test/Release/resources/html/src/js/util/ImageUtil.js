/**
 * Created by waouyang on 15/8/18.
 */
(function(uc){
    uc.util.ImageUtil = {
        getImageResolution: function(imagePath, callback){
            var image = new Image();
            image.onload = function(){
                callback && callback({
                    width: image.naturalWidth,
                    height: image.naturalHeight
                });
            };
            image.src = imagePath;
        },
        OnImageLoadError: function(node, defaultImage){
            node.onerror = null;
            node.src = defaultImage;
        },
        createThumbnail: function(filePath){
            return ucClient.System.createThumbnailsSync(filePath);
        }
    }
})(uc);