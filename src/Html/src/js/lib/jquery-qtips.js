(function ($) {
    $(function () {
        var timmer = '', _this;
        $('body').on('mouseenter', '[qtips]', function (e) {
            _this = $(this);
            timmer = setTimeout(function () {
                var _qt_text = _this.attr('qtips');//获取tips 文字
                var _qt_w = _this.attr('qtips-w');//获取tips 固定宽度
                var _show_by = _this.attr('qtips-show-by');//获取tips 显示方式: mouse  随鼠标指针位置显示
                var _text = $('<div class="text"></div>');
                var _dir = $('<div class="qtips-dir"></div>');
                var _wrap = $('<div class="qtips-wrap"></div>');

                var _offset = _this.offset();//获取offset
                var _w = _this.outerWidth();//获取outerWidth
                var _dom_w = _qt_w ? _qt_w : 'auto';
                _wrap.css({'width':_dom_w, 'height': 'auto'});

                _text.html(_qt_text);
                _dir.appendTo(_wrap);
                _text.appendTo(_wrap);

                $('body').append(_wrap);
                var _wrap_w = _wrap.outerWidth();//新生成的宽度
                var _wrap_h =  0;
                var _left = 0;
                var _top = _offset.top - 25;

                if (_qt_text.length >= 8) {
                    _wrap.addClass('long');
                    _wrap_h = _wrap.outerHeight();//新生成的宽度
                    _top = _offset.top - _wrap_h - 5;
                }

                _left = (_w - _wrap_w) >= 0 ? (_offset.left + (_w - _wrap_w) / 2) : (_offset.left - (_wrap_w - _w) / 2 );
                var padding_left =_offset.left+_wrap_w -$(window).width();
                if(padding_left>0)
                {
                    //当tips在屏幕最右侧且超出屏幕时 需要 减去伸出屏幕外的距离 padding_left 再恢复上次的差值( (_wrap_w - _w) / 2)
                    _left -=padding_left-(_wrap_w - _w)/2;
                }
                _wrap.css({'left': _left, 'top': _top});

                if (_show_by) {
                    _wrap.css({'left': e.pageX + 10, 'top': e.pageY}).addClass('qtips-show-by');
                }
                _wrap.fadeIn(200);
            }, 500);
        }).on('mouseleave', '[qtips]', function () {
            if (timmer) {
                clearTimeout(timmer);
            }
            $('.qtips-wrap').fadeOut(200, function () {
                $(this).remove();
            });
        }).on('DOMNodeRemoved', '[qtips]', function() {
            if (timmer) {
                clearTimeout(timmer);
            }
            $('.qtips-wrap').fadeOut(200, function () {
                $(this).remove();
            });
        }).on('click', '[qtips]', function () {
            if (timmer) {
                clearTimeout(timmer);
            }
            $('.qtips-wrap').fadeOut(200, function () {
                $(this).remove();
            });
        });
    });
})($);
