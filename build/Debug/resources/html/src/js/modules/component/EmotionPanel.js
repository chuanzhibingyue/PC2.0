/**
* Created by senlin.ouyang on 2015/7/20.
*/
(function (uc) {
    uc.modules.component.EmotionPanel = function (target, container) {
        if (!target) {
            return;
        }

        var $target, $container, me = this, $emotionPanel, $emotionContainer, emotions=[];
        if (target.jquery) {
            $target = target;
        }
        else {
            $target = $("#" + target);
        }

        if (container) {
            $container = container && container.jquery ? container : $("#" + container);
        }

        _init();
        _bindEvents();

        function _init() {
            emotions = [
                    { image: "1f604", title: "" },
                    { image: "1f60a", title: "" },
                    { image: "1f603", title: "" },
                    { image: "1f60d", title: "" },
                    { image: "263a", title: "" },
                    { image: "1f61a", title: "" },
                    { image: "1f633", title: "" },
                    { image: "1f632", title: "" },
                    { image: "1f613", title: "" },
                    { image: "1f601", title: "" },
                    { image: "1f61c", title: "" },
                    { image: "1f60c", title: "" },
                    { image: "1f62a", title: "" },
                    { image: "1f609", title: "" },
                    { image: "1f60f", title: "" },
                    { image: "1f61e", title: "" },
                    { image: "1f614", title: "" },
                    { image: "1f622", title: "" },
                    { image: "1f602", title: "" },
                    { image: "1f62d", title: "" },
                    { image: "1f631", title: "" },
                    { image: "1f621", title: "" },
                    { image: "1f616", title: "" },
                    { image: "1f637", title: "" },
                    { image: "1f612", title: "" },
                    { image: "1f61d", title: "" },
                    { image: "1f618", title: "" },
                    { image: "1f628", title: "" },
                    { image: "1f47f", title: "" },
                    { image: "1f625", title: "" },
                    { image: "1f630", title: "" },
                    { image: "1f623", title: "" },
                    { image: "1f620", title: "" },
                    { image: "1f64f", title: "" },
                    { image: "1f44d", title: "" },
                    { image: "1f44e", title: "" },
                    { image: "1f44c", title: "" },
                    { image: "270a", title: "" },
                    { image: "1f4aa", title: "" },
                    { image: "270c", title: "" },
                    { image: "1f44f", title: "" },
                    { image: "1f64c", title: "" },
                    { image: "261d", title: "" },
                    { image: "1f446", title: "" },
                    { image: "1f449", title: "" },
                    { image: "1f447", title: "" },
                    { image: "1f448", title: "" },
                    { image: "1f440", title: "" },
                    { image: "1f444", title: "" },
                    { image: "2764", title: "" },
                    { image: "1f494", title: "" },
                    { image: "26a1", title: "" },
                    { image: "1f319", title: "" },
                    { image: "2728", title: "" },
                    { image: "2600", title: "" },
                    { image: "1f31f", title: "" },
                    { image: "2601", title: "" },
                    { image: "1f4a6", title: "" },
                    { image: "1f4a8", title: "" },
                    { image: "2614", title: "" },
                    { image: "26c4", title: "" },
                    { image: "1f343", title: "" },
                    { image: "1f342", title: "" },
                    { image: "1f33b", title: "" },
                    { image: "1f33a", title: "" },
                    { image: "1f380", title: "" },
                    { image: "1f381", title: "" },
                    { image: "1f382", title: "" },
                    { image: "1f383", title: "" },
                    { image: "1f384", title: "" },
                    { image: "1f385", title: "" },
                    { image: "1f388", title: "" },
                    { image: "1f389", title: "" },
                    { image: "1f38e", title: "" },
                    { image: "1f49d", title: "" },
                    { image: "1f451", title: "" },
                    { image: "1f354", title: "" },
                    { image: "1f359", title: "" },
                    { image: "1f361", title: "" },
                    { image: "1f366", title: "" },
                    { image: "1f370", title: "" },
                    { image: "1f35a", title: "" },
                    { image: "1f35c", title: "" },
                    { image: "1f35e", title: "" },
                    { image: "1f37a", title: "" },
                    { image: "1f37b", title: "" },
                    { image: "1f4a1", title: "" },
                    { image: "1f4a3", title: "" },
                    { image: "1f4a4", title: "" },
                    { image: "1f4b0", title: "" },
                    { image: "1f4bc", title: "" },
                    { image: "1f4e2", title: "" },
                    { image: "1f3e5", title: "" },
                    { image: "1f48a", title: "" },
                    { image: "1f489", title: "" },
                    { image: "26fd", title: "" },
                    { image: "2708", title: "" },
                    { image: "1f684", title: "" },
                    { image: "1f695", title: "" },
                    { image: "1f697", title: "" },
                    { image: "1f6b2", title: "" },
                    { image: "1f6a2", title: "" },
                    { image: "1f6a5", title: "" },
                    { image: "26a0", title: "" },
                    { image: "1f454", title: "" },
                    { image: "1f3a5", title: "" },
                    { image: "1f3c0", title: "" },
                    { image: "1f3be", title: "" },
                    { image: "26bd", title: "" },
                    { image: "1f3c3", title: "" },
                    { image: "1f3c4", title: "" },
                    { image: "1f3bf", title: "" },
                    { image: "1f3c6", title: "" },
                    { image: "1f3ca", title: "" },
                    { image: "1f6a4", title: "" },
                    { image: "1f3af", title: "" },
                    { image: "1f3a4", title: "" },
                    { image: "26f3", title: "" },
                    { image: "1f3b1", title: "" },
                    { image: "1f300", title: "" },
                    { image: "260e", title: "" },
                    { image: "1f528", title: "" },
                    { image: "1f511", title: "" },
                    { image: "1f512", title: "" },
                    { image: "1f4f7", title: "" },
                    { image: "1f4fb", title: "" },
                    { image: "1f4f1", title: "" },
                    { image: "1f4e0", title: "" },
                    { image: "1f4bb", title: "" },
                    { image: "1f4bf", title: "" },
                    { image: "1f4dd", title: "" },
                    { image: "00a9", title: "" },
                    { image: "00ae", title: "" },
                    { image: "1f3a8", title: "" },
                    { image: "1f3b5", title: "" },
                    { image: "1f3b6", title: "" },
                    { image: "1f3e7", title: "" },
                    { image: "1f3ab", title: "" },
                    { image: "1f4e9", title: "" },
                    { image: "1f4eb", title: "" },
                    { image: "0031_20e3", title: "" },
                    { image: "0032_20e3", title: "" },
                    { image: "0033_20e3", title: "" },
                    { image: "0034_20e3", title: "" },
                    { image: "0035_20e3", title: "" },
                    { image: "0036_20e3", title: "" },
                    { image: "0037_20e3", title: "" },
                    { image: "0038_20e3", title: "" },
                    { image: "0039_20e3", title: "" },
                    { image: "0030_20e3", title: "" }];

            $emotionPanel = $("<div class='emotions-panel'/>");
            $emotionContainer = $("<div class='emotions-container'/>");
            $emotionContainer.appendTo($emotionPanel);
            var pages = Math.ceil(emotions.length / 50);
            var pageDomer = "";
            for (var p = 0; p < pages; p++){
                pageDomer =  pageDomer + ('<div class="emotion-page" pindex="' + p + '"></div>');
            }
            this.bottomHtml = '<div class="bottom">'
                +               '<div class="emotion-pages">'
                +                   pageDomer
                +               '</div>'
                +           '</div>';
            $emotionPanel.append(this.bottomHtml);
            if ($container) {
                $emotionPanel.appendTo($container);
            } else {
                $emotionPanel.insertAfter($target);
            }
            $(".emotion-pages", $container).css({width: pages*14});
            _showEmotions(0);
        }

        function _showEmotions(pageIndex) {
            var pages = Math.ceil(emotions.length/50);
            if (pageIndex >= pages) {
                pageIndex = 0;
            }
            $emotionContainer.empty();
            for (var i = pageIndex*50; i < pageIndex*50 + 50; i++) {
                var item = emotions[i];
                if (item == null) {
                    break;
                }
                $('<a href="javascript:void(0)"><img class="emotion-img" src="../images/emotions/' + item.image + '.emj"/></a>')
                   // .attr("title", item.title)
                    .attr("code", item.image)
                    .appendTo($emotionContainer);
            }
            $(".emotion-page", $emotionPanel).removeClass("sel");
            $(".emotion-page", $emotionPanel).each(
                function() {
                    if (Number($(this).attr("pindex")) == pageIndex) {
                        $(this).addClass("sel");
                        return true;
                    }
                }
            );
        }

        function _bindEvents() {
            $emotionPanel.on("click", "a", function (e) {
                if (me.onSelected) {
                    var code = $(e.target).attr("code");
                    if (code) {
                        me.onSelected(me, { emotion: code, event: e });
                    }
                }
            });

            $emotionPanel.on("dragstart", "a", function (e) {
                e.stopPropagation();
                return false;
            });

            $emotionPanel.on("click", ".emotion-img", function (e) {
                if (me.onSelected) {
                    var code = $(e.target).parent().attr("code");
                    if (code) {
                        me.onSelected(me, { emotion: code, event: e });
                    }
                }
            });

            $emotionPanel.on("click", ".emotion-page",
                function () {
                    var index = Number($(this).attr("pindex"));
                    _showEmotions(index);
            });
        }

        me._show = function () {
            var position = $target.position(),
                width = $emotionPanel.width(), height = $emotionPanel.height();

            $emotionPanel.css({ left: position.left - 150 + "px", top: (position.top - height - 15) + "px" }).show();
        }

        me._hide = function (e) {
            if ($(e.target).hasClass("title") ||
                $(e.target).hasClass("emotion-tab") ||
                $(e.target).hasClass("emotion-page") ||
                $(e.target).hasClass("emotion-pages") ||
                $(e.target).hasClass("bottom")) {
                return;
            }
            $emotionPanel.hide(0);
        }
    };

    jQuery.extend(uc.modules.component.EmotionPanel.prototype, {
        onSelected: function (sender, args) {
        },

        show: function () {
            this._show();
        },

        hide: function (e) {
            this._hide(e);
        }
    });
})(uc);