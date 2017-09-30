/**
 * Created by zhen.xu on 2015/11/4.
 */
(function (uc) {
    "use strict";

    var _this = null;
    var selectEnd = 4, middleIndex = 2;
    uc.modules.component.TimeChooseBox = function (container,options) {
        if (!container) {
            return;
        }
	    _this = this;
	    this._default = {
		    onkeypress:null,
		    onclick:null,
		    onselect:null,
		    self:null
	    };
        this.$el = document.getElementById(container);
        this.$el.onkeypress = this.keyPress;
        this.$el.onclick = this.click;
        this.$el.onselect = this.select;
        this.$el.onkeydown = this.keydown;
        this.$el.onkeyup = this.keyUp;
        this.$el.onpaste = this.preventDefault;
        this.$el.ondragstart = this.preventDefault;
        this.$el.onblur = this.blur;
        this.$el.$options = $.extend({},  this._default, options);
        //this.setTime();
        return this.$el;
    };

    $.extend(uc.modules.component.TimeChooseBox.prototype, {
        setTime: function () {
            var today = new Date();
            var hours = today.getHours(), minutes = today.getMinutes();
            if (minutes > 30) {
                minutes = "00";
                if (hours == 23) {
                    hours = 0;
                } else {
                    hours += 1;
                }
            } else {
                minutes = "30";
            }
            hours = ((hours + 100) + "").substr(1, 2);
            this.$el.value = (hours + ":" + minutes);
        },

        preventDefault: function (e) {
            return e.preventDefault();
        },

        keyPress: function (e) {
            if (e.target.selectionStart > selectEnd || e.target.selectionStart == middleIndex || e.keyCode < 48 || e.keyCode > 57) {
                return e.preventDefault();
            }
            _this.proccessKeypress(e, e.keyCode);
        },

        proccessKeypress: function (e, keyCode) {
            var number = parseInt(String.fromCharCode(keyCode));
            var selectEnd = e.target.selectionEnd;
            var selectStart = e.target.selectionStart;
            if (selectStart == 0 && selectEnd == 0) {
                var _hour = e.target.value.substring(0, e.target.value.indexOf(':'));
                number > 2 ? e.preventDefault() | e.target.setSelectionRange(0, e.target.value.indexOf(':')) | e.target.setRangeText('0' + number) | e.target.setSelectionRange(e.target.value.indexOf(':') + 1, e.target.value.length) : _hour.length >= 2 ? e.preventDefault() : true;
            } else if (selectStart == 0 && selectEnd == 1) {
                return number <= 2 ? true : e.preventDefault() | e.target.setSelectionRange(0, e.target.value.indexOf(':')) | e.target.setRangeText('0' + number);
            } else if (selectStart >= 1 && selectEnd <= 2) {
                var _one = e.target.value.substring(0, 1);
                return  _one >= 2 ? number <= 4 ? (number <= 3 ? true : e.preventDefault()) : e.preventDefault() : true;
            } else if (selectStart == 0 && selectEnd == 2) {
                return  number > 2 ? e.preventDefault() | e.target.setSelectionRange(0, e.target.value.indexOf(':')) | e.target.setRangeText('0' + number) | e.target.setSelectionRange(e.target.value.indexOf(':') + 1, e.target.value.length) : true;
            } else if (selectStart == 3) {
                return number <= 5 ? true : e.target.setRangeText('0' + number) | e.target.setSelectionRange(3, 5) | e.preventDefault();
            } else {
                var _len = e.target.value.substring(e.target.value.indexOf(':') + 1, e.target.value.length + 1);
                if (_len.length > 2) {
                    return e.preventDefault();
                }
            }
        },

        keydown: function (e) {
            if (e.target.selectionStart == e.target.value.indexOf(':') + 1 && e.keyCode == 8 && e.target.selectionEnd == e.target.value.indexOf(':') + 1) {
                e.target.setSelectionRange(0, e.target.value.indexOf(':'));
                e.preventDefault();
            }
	        if(this.$options.onkeypress)
		        this.$options.onkeypress( this.$options.self);
        },

        keyUp: function (e) {
            var number = String.fromCharCode(e.keyCode);
            var _munites = e.target.value.substring(e.target.value.indexOf(':') + 1, e.target.value.length + 1);
            var hasSplit = e.target.value.indexOf(':') >= 0 ? true : false;
            if (e.target.selectionStart == middleIndex && e.keyCode != 37 && e.keyCode != 39 && e.keyCode != 8) {

                if (hasSplit) {
                    e.target.setSelectionRange(3, 5);
                    return;
                }
                e.target.setRangeText(':');
                e.target.selectionStart = 3;
                return;
            }else{
	            if(e.target.value.indexOf(':')==-1){
		            e.target.setRangeText(':');
	            }
            }

            if (e.keyCode == 39 && e.target.selectionStart > e.target.value.indexOf(':')) {
                var _preNum = parseInt(e.target.value.substring(0, e.target.value.indexOf(':')));
                var _nextNum = e.target.value.substring(e.target.value.indexOf(':'), e.target.value.length + 1);
                _preNum < 10 ? e.target.value = '0' + _preNum + _nextNum : true;
                e.target.selectionStart = e.target.value.indexOf(':') + 1;
            }

            if (e.keyCode == 37 && e.target.selectionStart < e.target.value.indexOf(':') + 1) {
                var _preNum = e.target.value.substring(0, e.target.value.indexOf(':') + 1);
                var _nextNum = parseInt(e.target.value.substring(e.target.value.indexOf(':') + 1, e.target.value.length + 1));
                _nextNum < 6 ? e.target.value = _preNum + _nextNum + '0' : true;
                e.target.selectionEnd = e.target.value.indexOf(':');
            }
        },

        click: function (e) {
            e.target.selectionStart <= middleIndex ?
                e.target.setSelectionRange(0, e.target.value.indexOf(':') >= 0 ?
                    e.target.value.indexOf(':') : 1) : e.target.setSelectionRange(e.target.value.indexOf(':') + 1, 5);
            if (e.target.selectionStart == middleIndex) {
                //e.target.setSelectionRange(0, 2);
                return;
            }
            if(this.$options.onclick)
                this.$options.onclick(e.target.value,e.target.selectionStart,e.target.selectionEnd,this.$options.self);

        },

        select: function (e) {
            this.selectStart = e.target.selectionStart;
            if (e.target.selectionStart == middleIndex) {
                return e.preventDefault();
            }
        },

        blur: function (e) {
            var _value = $(this).val();
            var _middle = e.target.value.indexOf(':');
            var _preNum = _middle >= 0 ? _value.substring(0, _middle) : _value.substring(0, 2);
            var _preNext = _middle >= 0 ? _value.substring(_middle + 1, _value.length + 1) : '00';
            $(this).val((_preNum ? _preNum.length <= 1 ? '0' + _preNum : _preNum : '00') + ':' + (_preNext ? _preNext.length <= 1 ? '0' + _preNext: _preNext : '00'));
        }
    });

})(uc);