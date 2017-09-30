/**
 * Created by zhen.xu on 2016/5/13.
 */
(function (uc) {
    uc.modules.map.MapEventListener = function () {

    };

    jQuery.extend(uc.modules.map.MapEventListener.prototype,
        uc.interfaces.IUIEvent, {
        OnShowLocation: function () {
            var options = {
                title: uc_resource.WindowTitle.LocationView,
                width:  screen.availWidth * 0.7,
                height: screen.availHeight* 0.7,
                refreshIfExist: false,
                centerScreen: true,
                modal: false,
                resizable: false,
                hasTaskBarIcon: true,
                isChild: true,
                minWidth: screen.availWidth * 0.7,
                minHeight: screen.availHeight * 0.7,
                behaviorIfSCClose: uc.constants.UI.WindowCloseBehavior.Close,
                caption: uc.constants.UI.DragCaption.Map
            };
            uc.util.WindowUtil.open('page/map.html', 'map', options);
        }
    });
})(uc);