/**
 * Created by waouyang on 15/8/18.
 */
(function (uc) {    
	$(document).keyup(function (e) {
        if (e.keyCode == 27) {        	
        	if($('#modalWin').is(":visible")){
        		uc.modules.component.PromptMessageBox.cancelDialog();             	
        	} else {
        		uc.modules.image.ImageCropper.cancelDialog();
        	}        	
        }
    });
    uc.modules.image.ImageCropper = {
    	active: false,
    	originalImagePath: "",
        init: function () {
           this.bindEvent();
        },
        cancelDialog: function() {
        	var key = uc.util.StorageUtil.getData('imageCropper');
    		uc.IUIService.triggerCrossWindowEvent(key, { okOrCancel: false});        		
    		uc.util.WindowUtil.close();
		},
        bindEvent: function() {
        	var _self = this;
        	var container = $('.avatar-body-container');
        	container.off("click");
        	
        	container.on("click", ".icon-close", function(){			    
        		//uc.IUIService.triggerCrossWindowEvent(key, { okOrCancel: false });
				// uc.util.WindowUtil.close();
					_self.cancelDialog();
			});
        	
        	container.on("click", ".btn-avatar-upload", function(){			    
        		uc.util.FileDialogUtil.openSelectFileDialog(uc_resource.Chat.ChooseImage, [{desc: uc_resource.Chat.OpenImageDesc, filter: uc_resource.Chat.OpenImageTips}], false, function (paths) {
                    if (paths && paths.length > 0) { 
                    	var filePath = paths[0];
                    	
                    	var size = uc.util.FileUtil.getFileSize(filePath);                    	
                		if(size > 5000000){                			
                			_self.popupMessage(uc_resource.Chat.OverSize);
                			return;
                		}
                		
                		uc.util.ImageUtil.getImageResolution(filePath, function(resolution){
                			var imageWidth = resolution.width;
                            var imageHeight = resolution.height;
                            if(imageWidth < 150 || imageHeight < 150){                            	
                    			_self.popupMessage(uc_resource.Chat.OverWidth);
                    			return;
                    		}
                			_self.loadImage(filePath);
                			$('.btn-ok').focus();
                        });
                    }
                });
			});
        	
        	container.on("click", ".btn-cancel", function(){	
        		_self.cancelDialog();
			});
        	
        	container.on("click", ".btn-ok", function(){			    
        		var key = uc.util.StorageUtil.getData('imageCropper');        		
        		
        		var avatarCropper = $('.avatar-cropper');
        		var avatarImage = avatarCropper.find('img');
        		if(avatarImage.length == 0 || !avatarImage.attr("src")) {
        			//alert("请选择图片。");
        			_self.popupMessage(uc_resource.Chat.ChooseTips);
        			return;
        		}
        		
        		var positionValue = $('.avatar-crop-info').val();
        		var positionInfo = JSON.parse(positionValue); 
        		
        		var croppedImagePath = uc.IClientService.getDataDirectory(uc.constants.UI.DirectoryType.Image) + '\\cropped\\' + _self.getCropImageFileName(_self.originalImagePath); 
        		   
        		//croppedImagePath = croppedImagePath.replace(/\\/g, "/");        		
        		var cropResult = _self.cropImage(_self.originalImagePath, positionInfo, croppedImagePath);
        		if(!cropResult){
        			return;
        		}
        		
				uc.IUIService.triggerCrossWindowEvent(key, { okOrCancel: true, croppedImagePath: croppedImagePath});
				uc.util.WindowUtil.close();
			});
        },
        loadImage: function(filePath){
        	var _self = this;
        	_self.originalImagePath = filePath;
        	//_self.uploadProjectAvatar(groupId, filePath);        	
        	var avatarCropper = $('.avatar-cropper');
        	var avatarPreview = $('.avatar-preview');
        	var image = _self.getAvatarImage(avatarCropper);
        	var previewImage = _self.getAvatarImage(avatarPreview);
        	image.attr("src", filePath);
        	previewImage.attr("src", filePath);
        	_self.previewCropImage(image, avatarPreview, filePath);
        },
        popupMessage: function(message){
        	setTimeout(function() {$('#modalWin .btn-ok').focus()}, 300);
        	
        	uc.modules.component.PromptMessageBox.open({title: uc_resource.Common.Notification, message: message ,buttonMode:"ok",OkText: uc_resource.Common.Known}, function(result){
               this.close();
               $('.btn-avatar-upload').focus();
            });
        },
        getCurrentDateTimeString: function() {
        	var d = new Date();
        	var currentDateTimeString = d.getFullYear() + "_" + (d.getMonth()+1) + "_" + d.getDate() + "_" +   
        	d.getHours() + "_" +  d.getMinutes() + "_" +  d.getSeconds() + "_" + d.getMilliseconds();
        	return currentDateTimeString;
        },
        getFileExtension: function(filePath){
        	var ext = filePath.split('.').pop();
        	return ext;
        },
        getCropImageFileName: function(originalImagePath) {
        	var _self = this;
        	var extension = _self.getFileExtension(originalImagePath);
        	var currentDateTimeString = _self.getCurrentDateTimeString();
        	var cropImageFileName = currentDateTimeString + "." +  extension;
        	return cropImageFileName;
        },
        cropImage: function(originalImagePath, positionInfo, croppedImagePath) {
        	var _self = this;
        	
        	var result = ucClient.Image.crop(                   // 成功时，返回0
        			originalImagePath,                           // 图像路径
        			Math.round(positionInfo.x < 0 ? 0 : positionInfo.x),                                              // 裁剪区域的左上角x坐标
        			Math.round(positionInfo.y < 0 ? 0 : positionInfo.y),                                              // 裁剪区域的左上角y坐标        			
        			Math.round(positionInfo.width),                                            // 裁剪区域宽度
        			Math.round(positionInfo.height),                                            // 裁剪区域的高度
        			Math.round(positionInfo.width),                                            // 目标宽度
        			Math.round(positionInfo.height),                                            // 目标高度
        		    croppedImagePath                          // 剪裁后图标的存储路径
        	);
        	if(result == 0){
        		return true;
        	} else {
        		_self.popupMessage(uc_resource.Chat.ScreenFaield);
        		uc.util.LogUtil.info('ImageCropper', 'cropImage', 'Exception:' + result, positionInfo);
        		return false;
        	}
        },
        previewCropImage: function(image, avatarPreview, filePath){
        	var _self = this;
        	if(_self.active){
        		image.cropper('destroy');
        	}
        	image.cropper({
        		aspectRatio: 1,
        		background: false,        		
        		preview: avatarPreview.selector,
                build: function (e) {
                  var $clone = $(this).clone();

                  $clone.css({
                    display: 'block',
                    width: '100%',
                    minWidth: 0,
                    minHeight: 0,
                    maxWidth: 'none',
                    maxHeight: 'none'
                  });

                  /*previews.css({
                    width: '100%',
                    overflow: 'hidden'
                  }).html($clone);*/
                },

                crop: function (e) {
                  var imageData = $(this).cropper('getImageData');
                  var previewAspectRatio = e.width / e.height;
                  var positionInfo = $(this).cropper('getData');
                  
                  $('.avatar-crop-info').val(JSON.stringify(positionInfo));

                  /*previews.each(function () {
                    var $preview = $(this);
                    var previewWidth = $preview.width();
                    var previewHeight = previewWidth / previewAspectRatio;
                    var imageScaledRatio = e.width / previewWidth;

                    $preview.height(previewHeight).find('img').css({
                      width: imageData.naturalWidth / imageScaledRatio,
                      height: imageData.naturalHeight / imageScaledRatio,
                      marginLeft: -e.x / imageScaledRatio,
                      marginTop: -e.y / imageScaledRatio
                    });
                  });*/
                }
              });
    		_self.active = true;
    		$('.btn-avatar-upload').text(uc_resource.Chat.ReUpload);
    		$('.upload-description').hide();
    		$('.avatar-cropper').removeClass("avatar-cropper-init");
        },
        getAvatarImage: function(avatarCropper) {
        	var image = avatarCropper.find('img');
        	if(image.length == 0){
        		image = $('<img class="avatar"/>');
        		avatarCropper.append(image);
        	}
        	return image;
        }
    };
    jQuery(document).ready(function () {
        uc.init();
        uc.modules.image.ImageCropper.init();
    })
})(uc);