/**
 * Created by waouyang on 15/8/18.
 */
(function (uc) {
    var _this = null;
    uc.modules.image.ImagePreviewer = {
        init: function () {
            _this = this;
            this.timer = -1;
            this.bDrag = 0;
            this.bMove = 0;
            this._clickLeft = 0;//鼠标首次移动的坐标x
            this._clickTop = 0;//鼠标首次移动的坐标y
            this.scaleDefaultSize = 1;//默认的缩放比例
            this.maxScaleSize = 10;//最大缩放比例
            this.rotateDegree = 0;
            this.imageList = [];
            this.imagePath = '';
            this.imageResolution = {
                width: 0,
                height: 0
            };
            this.getImageData();
            this.bindEvent();
            this.setPreviousNextBtnStatus();
            this.previewImage();
            this.initScale();//初始化图片缩放
            this.initDrag();//初始化图片拖动
            this.setCloudIcon();
            this.bodyClickEventEnabled = false; //Add this param to handle case: double click chat window image, the second click will close this window
            setTimeout(function(){
                _this.bodyClickEventEnabled = true;
            }, 500);
        },
        getImageData: function () {
            var imageData = uc.util.StorageUtil.getData('image-preview-data');
            uc.util.LogUtil.info('ImagePreviewer', 'getImageData', 'getImageData', imageData);
            if (imageData) {
                this.imageList = imageData.imageList;
                this.imagePath = imageData.imagePath;
                this.imageResolution = imageData.imageResolution;
            }
        },
        bindEvent: function () {
            jQuery(document.body).click(function(e){
                _this.onBodyClicked(e);
            });
            jQuery(document.body).keyup(function(e){
                if(e.which == 27){
                    uc.util.WindowUtil.close();
                }
            });
            this.closeBtn = $('.icon-preview-close');
            this.imageContainer = $('.image-container');
            this.imageNode = $('img');
            this.txtScaleSize = $('.scale-size');
            this.controlContainer = $('.control-container');
            this.previousControlBtn = $('.previous', this.controlContainer);
            this.nextControlBtn = $('.next', this.controlContainer);
            this.downloadControlBtn = $('.download', this.controlContainer);
            this.rotateControlBtn = $('.rotate', this.controlContainer);
            this.cloudControlBtn = $('.cloud', this.controlContainer);
            this.closeBtn.click(function () {
                uc.util.WindowUtil.close();
            });
            this.previousControlBtn.click(function () {
                _this.previewPreviousImage();
            });
            this.nextControlBtn.click(function () {
                _this.previewNextImage();
            });
            this.downloadControlBtn.click(function () {
                _this.downloadImage();
            });
            this.rotateControlBtn.click(function () {
                _this.rotateImage();
            });
            this.cloudControlBtn.click(function(){
                _this.saveImgToCloud();
            });
            this.imageNode[0].onmouseover = function () {
                _this.bDrag = 1;
            };
            this.imageNode[0].onmouseout = function () {
                _this.bDrag = 0;
            }
        },
        onBodyClicked: function(e){
            if(!this.bodyClickEventEnabled){
                return;
            }
            if(e.target.classList.contains('img') || e.target.classList.contains('control-btn') || e.target.classList.contains('scale-size')){
                return;
            }
            uc.util.WindowUtil.close();
        },
        previewPreviousImage: function(){
            var index = this.imageList.indexOf(this.imagePath);
            if(index > 0){
                this.resetImgPosition();
                this.imagePath = this.imageList[--index];
                if(_this.imagePath){
                    _this.imageNode.attr('src', '');
                    setTimeout(function(){
                        _this.previewImage();
                    }, 50);
                }
            }
            this.setPreviousNextBtnStatus();
        },
        previewNextImage: function(){
            var index = this.imageList.indexOf(this.imagePath);
            if(index < this.imageList.length - 1){
                this.resetImgPosition();
                this.imagePath = this.imageList[++index];
                if(_this.imagePath){
                    _this.imageNode.attr('src', '');
                    setTimeout(function(){
                        _this.previewImage();
                    }, 50);
                }
            }
            this.setPreviousNextBtnStatus();
        },
        setPreviousNextBtnStatus: function(){
            var index = this.imageList.indexOf(this.imagePath);
            if(index == -1){
                return;
            }
            if(index == 0){
                this.previousControlBtn.addClass('disabled');
            }else{
                this.previousControlBtn.removeClass('disabled');
            }
            if(index == this.imageList.length - 1){
                this.nextControlBtn.addClass('disabled');
            }else{
                this.nextControlBtn.removeClass('disabled');
            }
        },
        _getCurrentImageName: function(){
            var index = this.imagePath.lastIndexOf('\\');
            if (index == -1) {
                index = this.imagePath.lastIndexOf('/');
            }
            var imageName = '';
            if(index != -1){
                imageName = this.imagePath.substring(index);
                if(imageName){
                    var _index = imageName.indexOf('_');
                    if(_index != -1){
                        imageName = imageName.substr(_index + 1);
                    }
                }
            }
            return imageName;
        },
        downloadImage: function(){
            var currentImageName = this._getCurrentImageName();
            uc.util.FileDialogUtil.openSaveFileDialog(uc_resource.Chat.SaveImage , currentImageName, [{desc: uc_resource.Chat.OpenFileDesc, filter: ''}] ,function(args){
                var savePath = args[0];
                if (!savePath) {
                    return;
                }
                uc.util.FileUtil.copyFile(_this.imagePath, savePath);
            });
        },
        previewImage: function () {
            if (this.imagePath) {
                uc.util.LogUtil.info('ImagePreviewer', 'PreviewImage', 'ImagePath = ' + this.imagePath, null);
                this.rotateDegree = 0;
                this.imageNode.removeClass('d90 d180 d270');
                this.setImageSize();
                this.imageNode.attr('src', this.imagePath);
                this.imageNode.hide();
                this.imageNode.load(function () {
                    _this.initDragPosition();
                    _this.imageNode.show();
                });
            }
        },
        rotateImage: function () {
            this.imageNode.css('webkitTransform', 'scale(1)');
            //this.imageNode.css('zoom', '1'); //请勿删除
            this.scaleDefaultSize = 1;
            this.imageNode.addClass('drag');
            this.rotateDegree = (this.rotateDegree + 90) % 360;
            var degreeClass = this.rotateDegree == 0 ? '' : 'd' + this.rotateDegree;
            this.imageNode.removeClass('d90 d180 d270').addClass(degreeClass);
            this.setImageSize();
            this.initDragPosition();
        },
        setImageSize: function () {
            if (this.rotateDegree == 0 || this.rotateDegree == 180) {
                this.imageNode.css('max-width', screen.availWidth * 0.8);
                this.imageNode.css('max-height', screen.availHeight * 0.8);
            } else {
                this.imageNode.css('max-width', screen.availHeight * 0.8);
                this.imageNode.css('max-height', screen.availWidth * 0.8);
            }
        },
        initScale: function () {
            document.onmousewheel = function (e) {
                var _msg = '';
                if (e.wheelDelta >= 1) {
                    if (parseFloat(_this.scaleDefaultSize) >= _this.maxScaleSize) {
                        _msg = uc_resource.Chat.Maximized;
                        _this.showScaleSize(_msg);
                        return;
                    }
                    if (_this.scaleDefaultSize >= 1) {
                        _this.scaleDefaultSize += 0.5;
                    } else {
                        _this.scaleDefaultSize += 0.1;
                    }
                } else {
                    if (parseFloat(_this.scaleDefaultSize) <=0.2 ) {
                        _msg = uc_resource.Chat.Minimize;
                        _this.showScaleSize(_msg);
                        return;
                    }
                    if (_this.scaleDefaultSize >= 1) {
                        _this.scaleDefaultSize -= 0.5;
                    } else {
                        _this.scaleDefaultSize -= 0.1;
                    }
                }
                _msg = (_this.scaleDefaultSize * 100).toFixed(0) + '%';
                _this.imageNode.removeClass().addClass('img');
                var rotateTransform = _this.rotateDegree == 0 ? '' : 'rotate(' + _this.rotateDegree + 'deg) ';
                _this.imageNode.css('webkitTransform', rotateTransform +'scale('+ _this.scaleDefaultSize.toFixed(1) +')');
                //_this.imageNode.css('zoom',  _this.scaleDefaultSize.toFixed(1)); 请勿删除
                _this.showScaleSize(_msg);
            };
        },
        showScaleSize: function (msg) {
            this.txtScaleSize.html(msg);
            if (this.txtScaleSize.is(':hidden')) {
                this.txtScaleSize.fadeIn(300);
            }
            clearTimeout(this.timer);
            this.timer = setTimeout(function () {
                _this.txtScaleSize.fadeOut(300);
            }, 1500);
        },
        initDrag: function () {
            window.document.onmousedown = this.mousedown;
            window.document.onmouseup = this.mouseup;
            window.document.onmousemove = this.mousemove;
            window.document.ondragstart = this.dragstart;
        },
        initDragPosition: function () {
            var _css = {
                'left': (window.screen.availWidth - this.imageNode.width()) / 2,
                'top': (window.screen.availHeight - this.imageNode.height()) / 2
            };
            _this.imageNode.css(_css);
        },
        mousedown: function () {
            if (_this.bDrag) {
                _this._clickLeft = window.event.x - parseInt(_this.imageNode.css('left'));
                _this._clickTop = window.event.y - parseInt(_this.imageNode.css('top'));
                _this.bMove = 1;
            }
        },
        mouseup: function () {
            _this.bMove = 0;
            _this.imageNode.removeClass('drag');
        },
        mousemove: function () {
            if ( _this.bMove ) {
                _this.imageNode.addClass('drag');
                if ( window.event.x <= 10 || window.event.y <= 10 || window.event.y >= (window.screen.availHeight -60 )  || window.event.x >= (window.screen.availWidth - 10) ){
                    return;
                }

                var _css = {
                    'left': window.event.x - _this._clickLeft,
                    'top': window.event.y - _this._clickTop,
                    'marginLeft': '',
                    'marginTop': ''
                };

                _this.imageNode.css(_css);
            }
        },
        dragstart: function () {
            window.event.returnValue = false;
        },
        resetImgPosition: function () {
            this.scaleDefaultSize = 1;
            this.imageNode.css('webkitTransform', 'scale(1)');
        },
        setCloudIcon:function(){
            if(uc.util.StorageUtil.getData('cloudType')==1 && uc.util.StorageUtil.getData('CloudLoginResult')==0){
                this.cloudControlBtn.removeClass().addClass("control-btn cloud");
            };
        },
        saveImgToCloud:function(){
            var fileName=this._getCurrentImageName();
            ucClient.dispatchGlobalEvent("saveLocalImagesToCloud",{"filename":fileName,"fullpath":this.imagePath});
        }
    };
    jQuery(document).ready(function () {
        uc.init();
        uc.modules.image.ImagePreviewer.init();
    })
})(uc);