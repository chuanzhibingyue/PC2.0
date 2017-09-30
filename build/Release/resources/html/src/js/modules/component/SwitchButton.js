/**
 * Created by lei.zhou on 2015/7/31.
 */
(function (uc) {
    "use strict";
    var _this = null;

    uc.modules.component.SwitchButton = function (options) {
        _this = this;
        var container = options.container;
        
        if (container.jquery) {
            this.container = container;
        }else {
            this.container = $("#" + container);
        }
        
        this.onChanged = options.onChanged;
        this.disabled = !!options.disabled;
        this.onDisabledBtnClicked = options.onDisabledBtnClicked;
        
        this.node = $("<div class='switch-bar switch-bar-left'>" +
                        "<span class='on'>" + uc_resource.Meeting.TurnOn + "</span>" +
                        "<span class='off'>" + uc_resource.Meeting.TurnOff + "</span>" +
                        "<a href='javascript:void(0)' class='switch'></a>" +
                    "</div>").appendTo(this.container);
        
        this.bindEvent();
    };
    
    jQuery.extend(uc.modules.component.SwitchButton.prototype, {
        bindEvent: function () {
            this.node.on("click", function () {
                _this._onNodeClicked();
            });
            
        },

        _onNodeClicked: function () {
            if(this.disabled){
                if(this.onDisabledBtnClicked){
                    this.onDisabledBtnClicked();
                }
                return;
            }

            if (this.node.hasClass("switch-bar-left")) {
                this.node.removeClass("switch-bar-left").addClass("switch-bar-right");
            }
            else {
                this.node.removeClass("switch-bar-right").addClass("switch-bar-left");
            }

            this.onValueChange(this.getValue());
        },
        
        getValue: function () {
            return this.node.hasClass("switch-bar-right");
        },
        
        setValue: function (on) {
            if (on) {
                this.node.addClass("switch-bar-right").removeClass("switch-bar-left");
            }else {
                this.node.addClass("switch-bar-left").removeClass("switch-bar-right");
            }

            this.onValueChange(this.getValue());
        },

        onValueChange: function (value) {
            if(this.onChanged){
                this.onChanged(value);
            }
        },
        
        disable: function () {
            this.disabled = true;
        },
        
        enable: function () {
            this.disabled = false;
        },
        
        switchButton: function (on) {
            if (on) {
                this.node.addClass("switch-bar-right").removeClass("switch-bar-left");
            }else {
                this.node.addClass("switch-bar-left").removeClass("switch-bar-right");
            }
        }
    })
})(uc);