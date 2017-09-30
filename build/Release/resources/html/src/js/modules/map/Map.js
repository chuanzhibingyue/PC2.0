/*
 * created by zhen.xu 2016-5-13
 */
(function (uc) {
    var self = null;
    uc.modules.map.Map = {
        init: function () {
            self = this;
            this.container = $('.map-container');
            this.geoData = uc.util.StorageUtil.getData('GeoLocationData');
            this.bindEvent();
            this.initMap();
        },

        initMap: function () {
            this.map = new BMap.Map("map");
            this.showLocation(JSON.parse(this.geoData));
        },

        bindEvent: function () {
            uc.modules.component.TitleBarButton.init();
            this.bindNativeEvent();
        },

        showLocation: function (locationObj) {
            var data = locationObj.address;
            this.map.addControl(new BMap.MapTypeControl());   //添加地图类型控件
            this.map.enableScrollWheelZoom(true);     //开启鼠标滚轮缩放
            this.map.centerAndZoom(new BMap.Point(data.longitude, data.latitude), 9);

            this.local = new BMap.LocalSearch(this.map, {
                renderOptions: {map: this.map}
            });
            this.local.search(data.location);
        },

        NativeEventData: {
            WindowMaximized: {
                eventName: ucClient.Window.Status.MAXIMIZED_FROM_NORMAL,
                callback: function(){
                    uc.modules.component.TitleBarButton.setWindowState(uc.constants.UI.WindowState.Maximized);
                    self.container.addClass('max');
                }
            },
            WindowRestored: {
                eventName: ucClient.Window.Status.RESTORED_FROM_MAXIMIZED,
                callback: function(){
                    uc.modules.component.TitleBarButton.setWindowState(uc.constants.UI.WindowState.Normal);
                    self.container.removeClass('max');
                }
            }
        },

        bindNativeEvent: function(){
            var eventData, eventName, callback;
            for(var i in this.NativeEventData){
                eventData = this.NativeEventData[i];
                eventName = eventData.eventName;
                callback = eventData.callback;
                uc.util.EventUtil.bindNativeEvent(eventName, callback);
            }
        }
    }

    jQuery(document).ready(function () {
        uc.init();
        uc.modules.map.Map.init();
    });
})(uc);