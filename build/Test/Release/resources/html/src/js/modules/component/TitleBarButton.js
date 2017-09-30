/**
 * Created by waouyang on 15/7/31.
 */
(function(uc){
    var _this = null, _options;
    uc.modules.component.TitleBarButton = {
        init: function(windowCloseBehavior, options){
            _this = this;
            _options = options;

            this.windowCloseBehavior = windowCloseBehavior ? windowCloseBehavior: uc.constants.UI.WindowCloseBehavior.Close;
            this.windowState = uc.constants.UI.WindowState.Normal;
            this.bindEvent();
        },
        bindEvent: function(){
            this.iconContainer = $('.tool-icon-container');
            this.minIcon = $('.icon-min', this.iconContainer);
            this.maxIcon = $('.icon-max', this.iconContainer);
            this.closeIcon = $('.icon-close', this.iconContainer);

            this.minIcon.click(function(){
                uc.util.WindowUtil.minimize();
                uc.IUIService.minimizeWindow();
            });
            this.maxIcon.click(function(){
                if(_this.windowState == uc.constants.UI.WindowState.Normal){
                    uc.util.WindowUtil.maximize();
                    $(this).addClass('icon-restore');
                }else{
                    uc.util.WindowUtil.restore();
                    $(this).removeClass('icon-restore');
                }
            });
            this.closeIcon.click(function () {
                switch (_this.windowCloseBehavior){
                    case uc.constants.UI.WindowCloseBehavior.Quit:
                        uc.quit();
                        break;
                    case uc.constants.UI.WindowCloseBehavior.Hide:
                        uc.util.WindowUtil.hide();
                        uc.IUIService.minimizeWindow();
                        break;
                    case uc.constants.UI.WindowCloseBehavior.Close:
                        if (_options && _options.events && _options.events.close && $.isFunction(_options.events.close)) {
                            _options.events.close();
                        }
                        uc.util.WindowUtil.close();
                        break;
                    case uc.constants.UI.WindowCloseBehavior.Minimize:
                        uc.util.WindowUtil.minimize();
                        uc.IUIService.minimizeWindow();
                        break;
                    case uc.constants.UI.WindowCloseBehavior.UnClose:
                        break;
                    default:
                        if (_options && _options.events && _options.events.close && $.isFunction(_options.events.close)) {
                            _options.events.close();
                        }
                        uc.util.WindowUtil.close();
                        break;
                }
            });
        },
        setWindowState: function(windowState){
            this.windowState = windowState;
            if(windowState == uc.constants.UI.WindowState.Maximized){
                $('#layout-container').removeClass('window-shadow').addClass('window-maximized');
                this.maxIcon.addClass('icon-restore');
            }else{
                $('#layout-container').removeClass('window-maximized').addClass('window-shadow');
                this.maxIcon.removeClass('icon-restore');
            }
        }
    }
})(uc);