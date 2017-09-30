;(function ($, undefined) {
    var noop = function () {};
    var slice = Array.prototype.slice;
    var template = uc.util.template;
    
    var defaults = {
        menus: [],
        arrow: 't',
        hoverClass: 'selected'
    };
    
    var $popMenu;
    var lastPopMenuObj;
    var tmp = template('<@$.each(menus, function () {@>\
        <div class="start-item" action="<@=this.action@>"><@=this.title@></div>\
    <@})@>')
    var stopPropagation = false;
    
    function PopMenu($elm, options) {
        this.$elm = $elm;
        this.options = options;
        this.isShow = false;
        this.init();
    }
    PopMenu.prototype = {
        actions: {
            'start-chat': function () {
                uc.modules.component.ExtraMenuBar.onStartChatMenuClicked();
            },
            'create-project': function () {
                uc.modules.component.ExtraMenuBar.onCreateProjectMenuClicked();
            },
            'create-meeting': function () {
                uc.modules.component.ExtraMenuBar.onCreateMeetingMenuClicked();
            }
        },
        init: function () {
            this.bindEvent();
        },
        bindEvent: function () {
            var options = this.options;
            var self = this;
            if (!$popMenu) {
                $popMenu = $('<div class="start-container node-shadow hide" ></div>').appendTo('body');
                $popMenu.on('click', '.start-item', function () {
                    var action = this.getAttribute('action');
                    if (self.actions[action]) {
                        self.actions[action](this);
                        lastPopMenuObj.hide();
                    }
                })
                $(document).on('click', function () {
                    if (!stopPropagation) {
                        if (lastPopMenuObj) {
                            lastPopMenuObj.hide();
                        }
                    }
                    stopPropagation = false;
                })
            }
            this.$elm.on('click', function () {
                self.toggle();
                stopPropagation = true;
            })
        },
        toggle: function () {
            if (this.isShow) {
                this.hide();
            } else {
                this.show();
            }
        },
        show: function () {
            if (lastPopMenuObj !== this) {
                if (lastPopMenuObj) {
                    lastPopMenuObj.hide();
                }
                $popMenu.removeClass('arrow-t arrow-l arrow-r arrow-b');
                $popMenu.addClass('arrow-' + this.options.arrow);
                this.pos();
                $popMenu.html(tmp(this.options));
            }
            $popMenu.removeClass('hide');
            this.$elm.addClass(this.options.hoverClass);
            this.isShow = true;
            lastPopMenuObj = this;
        }, 
        hide: function () {
            this.isShow = false;
            $popMenu.addClass('hide');
            this.$elm.removeClass(this.options.hoverClass);
            lastPopMenuObj = null;
        },
        pos: function () {
            var $elm = this.$elm;
            var offset = this.$elm.offset();
            if (this.options.arrow == 't') {
                $popMenu.css({
                    top: offset.top + $elm.outerHeight() + 10,
                    left: offset.left + ($elm.outerWidth()/2) - 71.5
                });
            }
            
        }
    }
    
    $.fn.popMenu = function (options) {
        var args = slice.call(arguments, 1);
        var res;
        var flag = false;
        this.each(function () {
            var $elm = $(this);
            var obj = $elm.data('popMenu');
            if (typeof options == "string" ) {
                if (obj && obj[options]) {
                    var opt = obj[options];
                    if ($.isFunction(opt)) {
                        res = opt.apply(obj, args);
                        if (res !== undefined) {
                            flag = true;
                            return true;
                        }
                    } else {
                        if (args.length == 0) {
                            res = opt;
                            flag = true;
                            return true;
                        } else if (args.length == 1) {
                            obj[options] = args[0];
                        }
                    }
                }
            } else {
                $elm.data('popMenu', new PopMenu($elm, $.extend({}, defaults, options)));
            }
        })
        if (flag) {
            return res;
        } else {
            return this;
        }
    }
    
    $.fn.popMenu.defaults = defaults;
})(jQuery);