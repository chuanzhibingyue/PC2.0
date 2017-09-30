/**
 * Created by zhen.xu on 16/5/26.
 */
(function (uc) {
    var self = null, timer, timeoutTimer, endTimer;
    uc.modules.meeting.JoinAnimation = {
        init: function () {
            self = this;
            this.Client = {
                _w: 890,
                animation_wrap_w: 500,
                animation_w: 300
            };
            this.index = 0;
            this.endIndex = 40;
            this.endFylingIndex = 1;
            this.endFlyingTime = 17;
            this.timeoutTime = 60;
            this.isReady = false;
            this.node = $('.join-animation');
            this.bee = $('.bee img', this.node);
            this.btnCancel = $('.waiting-animation-cancel a', this.node);
            this.mainLayout = $('#layout-container');
            this.nodeWrap = $('.wrap', this.node);

            this.bindEvent();
        },

        bindEvent: function () {
            this.btnCancel.unbind().on('click', function () {
                self.cancelMeeting();
                self.node.addClass('close');
                self.clear();
            });
        },

        cancelMeeting: function () {
            if (this.isReady) {
                uc.IMeetingService.cancelJoinMeeting();//取消进入会议
            } else {
                uc.IMeetingService.killMeetingClient();//杀掉进会进程
            }

            uc.IUIService.cancelJoinMeeting();
        },

        showJoinAnimation: function () {
            uc.util.WindowUtil.restore();
            //uc.util.WindowUtil.center();
            this.node.show();
            this.beginFlying();
            this.hideMainLayoutWindow();
            this.checkTimeout();
            uc.util.WindowUtil.setWindowDBClickDisble();
        },

        hideMainLayoutWindow: function () {
            setTimeout(function () {
                self.mainLayout.addClass('show-join-animation');
            }, 600);
        },

        checkTimeout: function () {
            timeoutTimer = setInterval(function () {
                if (self.timeoutTime <= 0) {
                    uc.util.LogUtil.info('JoinAnimation', 'checkTimeout', 'data', 'join meeting time out');
                    self.clear();
                    uc.IUIService.cancelJoinMeeting();
                    //self.cancelMeeting();
                    return;
                }
                self.timeoutTime--;
            }, uc.constants.Timer.OneSecond);
        },

        beginFlying: function () {
            setTimeout(function () {
                timer = setInterval(function () {
                    if (self.index >= self.endIndex) {
                        self.index = 0;
                        return;
                    }
                    self.bee.attr('src', '../images/bee/xmf' + self.index + '.png').removeClass('hide');
                    self.index++;
                }, 60);
            }, 600);
        },

        endFlying: function () {
            endTimer = setInterval(function () {
                if (self.endFylingIndex >= self.endFlyingTime) {
                    self.endFylingIndex = 1;
                    clearTimeout(endTimer);
                    return;
                }
                self.bee.attr('src', '../images/bee/tc' + self.endFylingIndex + '.png');
                self.endFylingIndex++;
            }, 110);
        },

        endAnimation: function () {
            uc.util.WindowUtil.hide();
            this.clear();
        },

        getWindowPosition: function () {
            var windowPosition = uc.IClientService.getWindowPosition();
            var x = y = 0;
            if (windowPosition) {
                x = this.Client._w - this.Client.animation_wrap_w + (this.Client.animation_wrap_w - this.Client.animation_w) / 2 + windowPosition[0];
                y = windowPosition[1];
            }

            uc.util.LogUtil.info('JoinAnimation', 'getWindowPosition', 'data', {
                windowPosition: windowPosition,
                x: x,
                y: y
            });

            return {x: x, y: y};
        },

        showSlideJoining: function () {
            this.nodeWrap.addClass('slide-up');
            var position = this.getWindowPosition();
            //this.bee.attr('src', '../images/bee/tc1.png');
            // setTimeout(function () {
            //     //self.endFlying();
            // }, 500);
            setTimeout(function () {
                uc.IMeetingService.sendMeetingClientPosition(position.x, position.y);
                self.endAnimation();
            }, 1300);
        },

        joinReady: function () {
            this.isReady = true;
        },

        joined: function () {
            //收到云会议joined后，延时一秒后再收起，否则云会议正在加载会很卡
            setTimeout(function () {
                self.showSlideJoining();
            }, 1500);
        },

        showJoinFailed: function () {
            this.clear();
        },

        closed: function () {
            this.clear();
        },

        clear: function () {
            this.node.addClass('close');
            this.mainLayout.addClass('hide-join-animation');
            setTimeout(function () {
                self.isReady = false;
                self.index = 0;
                self.endFylingIndex = 1;
                self.timeoutTime = 60;
                self.node.hide();
                self.bee.addClass('hide');
                self.node.removeClass('close');
                self.nodeWrap.removeClass('slide-up');
                self.mainLayout.removeClass('show-join-animation hide-join-animation');
                clearInterval(timer);
                clearInterval(timeoutTimer);
                clearTimeout(endTimer);
                uc.util.WindowUtil.setWindowDBClickEnable();
            }, 400);
        }
    }
})(uc);