/**
 * Created by waouyang on 15/9/6.
 */
(function(uc){
    var Menu = {
        data: {
            zIndex: 2000,
            index: 2000,
            template: '<div class="menu node-shadow"><ul></ul></div>',
            config: {
                menuItemNode: '<li class="menu-item {menuStatus}"><span class="menu-label">{menuLabel}</span></li>',
                menuItemNodeWithIcon: '<li class="menu-item {menuStatus}"><span class="menu-icon">{menuIcon}</span><span class="menu-label">{menuLabel}</span></li>',
                menuParentNode: '<li class="menu-parent"><span class="menu-label">{menuLabel}</span></li>',
                menuParentNodeWithIcon: '<li class="menu-parent"><span class="menu-icon">{menuIcon}</span><span class="menu-label">{menuLabel}</span></li>',
                menuSplit: '<li class="menu-split"></li>',
                list: '.menu ul',
                top: 2,
                left: 5,
                space: 'space',
                more: 'more'
            },
            list: [],
            gradient: {
                menu: 150,
                close: 1000
            },
            bodyClickEventBound: false
        },

        init: function (dom, event, menuOptions, type) {
            this.bind(dom, event, menuOptions, type);
        },

        bind: function (dom, event, menuOptions, type) {
            var _this = this;
            dom.on(event, function(e){
                _this.menu(e, menuOptions, event, type, dom);
                return false;
            });
        },
        bindBodyClickEvent: function(){
            if(this.data.bodyClickEventBound){
                return;
            }
            var _this = this;
            $(document.body).click(function(e){
                if(!$(e.target).hasClass('bee-menu') && !$.contains($('.bee-menu'), e.target)){
                    _this.close();
                }
            });
            this.data.bodyClickEventBound = true;
        },

        load: function (dom, event, menuOptions, type) {
            this.init($(dom), event, menuOptions, type);
        },

        close: function (index) {
            var _this = this;
            if (typeof index == 'undefined') {
                index = 0;
            }

            var fn = function () {
                for (var i = index; i < _this.data.list.length; i++) {
                    $(_this.data.list[i]).remove();
                    _this.data.zIndex--;
                }
                _this.data.list.splice(index, _this.data.list.length);
            };
            fn();
        },

        closeMore: function (type) {
            var index = type.menuContainer.css('z-index') - this.data.index;
            this.close(index + 1, this.data.gradient.close);
        },

        bindFn: function (ev,label) {
            this.method = function (e) {
                ev(label);
                e.stopPropagation();
                e.preventDefault();
                Menu.close();
            }
        },

        event: function (menuContainer, obj, ev,label) {
            var _this = this;
            var type = {menuContainer: menuContainer, list: obj};
            if (typeof ev == 'function') {
                var fn = new this.bindFn(ev,label);
                obj.bind('click', fn.method.bind(fn));
                obj.bind('mouseover', function () {
                    _this.closeMore(type);
                });
            } else {
                this.load(obj, 'mouseover', ev, type);
            }
        },

        getList: function (menuContainer, menuOptions) {
            var menuItemNode;
            var menuItems = menuOptions.menuItems;
            var hasIcon = menuOptions.hasIcon;
            var menuItemNodeStr = hasIcon ? this.config('menuItemNodeWithIcon') : this.config('menuItemNode');
            var menuParentNodeStr = hasIcon ? this.config('menuParentNodeWithIcon') : this.config('menuParentNode');
            var list = this.config('list');
            var listNode = $(list, menuContainer);
            hasIcon && listNode.addClass('with-icon');
            var menuItem, menuType, menuLabel, menuIcon, menuDisabled, callback, subMenuOptions;
            var iconNode, statusNode;
            for(var i = 0, len = menuItems.length; i < len; i++){
                menuItem = menuItems[i];
                menuType = menuItem.type;
                switch (menuType){
                    case uc.constants.UI.MenuItemType.MenuItem:
                        menuIcon = menuItem.icon;
                        menuLabel = menuItem.label;
                        menuDisabled = menuItem.disabled;
                        callback = menuItem.callback;
                        statusNode = menuDisabled ? 'disabled hide' : '';
                        if(hasIcon){
                            if(menuIcon){
                                iconNode = '<img src="' + menuIcon + '">';
                            }else{
                                iconNode = '';
                            }
                            menuItemNode = $(menuItemNodeStr.replace(/\{menuStatus}/ig, statusNode).replace(/\{menuIcon}/ig, iconNode).replace(/\{menuLabel}/ig, menuLabel)).appendTo(listNode);
                        }else{
                            menuItemNode = $(menuItemNodeStr.replace(/\{menuStatus}/ig, statusNode).replace(/\{menuLabel}/ig, menuLabel)).appendTo(listNode);
                        }
                        this.event(menuContainer, menuItemNode, callback,menuLabel);
                        break;
                    case uc.constants.UI.MenuItemType.Split:
                        $(this.config('menuSplit')).appendTo(listNode);
                        break;
                    case uc.constants.UI.MenuItemType.MenuParent:
                        menuType = menuItem.type;
                        menuIcon = menuItem.icon;
                        menuLabel = menuItem.label;
                        subMenuOptions = menuItem.subMenuOptions;
                        if(hasIcon){
                            if(menuIcon){
                                iconNode = '<img src="' + menuIcon + '">';
                            }else{
                                iconNode = '';
                            }
                            menuItemNode = $(menuParentNodeStr.replace(/\{menuIcon}/ig, iconNode).replace(/\{menuLabel}/ig, menuLabel)).appendTo(listNode);
                        }else{
                            menuItemNode = $(menuParentNodeStr.replace(/\{menuLabel}/ig, menuLabel)).appendTo(listNode);
                        }
                        this.event(menuContainer, menuItemNode, subMenuOptions);
                        break;
                }
            }
        },

        getBody: function (type) {
            if (typeof type == 'undefined') {
                this.close();
            } else {
                this.closeMore(type);
            }
            var body = this.data.template;
            var menuContainer = jQuery('<div class="bee-menu"></div>').appendTo(document.body);
            menuContainer.css({
                position: 'absolute',
                display: 'none'
            });
            menuContainer.html(body);
            return menuContainer;
        },

        position: function (e, menuContainer, type) {
            var t = e.pageY;
            var l = e.pageX;
            if (typeof type == 'object') {
                l = $(type.menuContainer).offset().left + $(type.menuContainer).width() + parseInt(this.config('left'));
                t = $(type.list).offset().top - parseInt(this.config('top'));
            }
            var lt = this.overflow(menuContainer, l, t, type);
            menuContainer.css({
                top: lt.top + 'px',
                left: lt.left + 'px'
            });
        },

        config: function (key) {
            return this.data.config[key];
        },

        overflow: function (menuContainer, l, t, type) {
            var wWidth = jQuery(window).width();
            var wHeight = jQuery(window).height();
            if (menuContainer.width() + 2 > wWidth - l) {
                l -= menuContainer.width();
                if (typeof type == 'object') {
                    l -= $(type.menuContainer).width() + this.config('left') * 2;
                }
            }

            if (menuContainer.height() + 2 > wHeight - t) {
                t -= menuContainer.height();
                if (typeof type == 'object') {
                    t += $(type.list).height() + parseInt(this.config('top'));
                }
            }
            return {left: l, top: t};
        },

        menu: function (e, menuOptions, event, type) {
            this.bindBodyClickEvent();
            var menuContainer = this.getBody(type);
            this.getList(menuContainer, menuOptions);
            menuContainer.css('z-index', this.data.zIndex);
            menuContainer.fadeIn(this.data.gradient.menu);
            this.data.list.push(menuContainer);
            this.position(e, menuContainer, type);
            this.data.zIndex++;
        }
    };
    uc.modules.component.Menu = {
        showMenu: function(e, menuOptions){
            Menu.menu(e, menuOptions);
        }
    }
})(uc);