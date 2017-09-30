/**
* Created by lei.zhou on 2015/7/31.
*/
(function (uc) {
    "use strict";

    uc.modules.component.DropdownList = function (target, options) {
        if (!target) {
            return;
        }

        var maxShownItemCount = 6;
        var singleItemCount = 25;

        var $target, me = this, selectedData;
        if (target.jquery) {
            $target = target;
        }
        else {
            $target = $("#" + target);
        }

        if (!options.items || !$.isArray(options.items)) {
            return;
        }
        options = $.extend({}, options);


        if ($target.find(".dropdown-list-nav").length != 0) {
            $target.html("");
        }
        var $display = $("<span></span><a href='javascript:void(0)' class='dropdown-list-nav'>*</a>").appendTo($target);

        if (options.selectedValue) {
            _bindSelectedData(options.selectedValue);
        }

        $target.click(function (e) {
            var dropdown = $target.attr("dropdown");
            if (dropdown) {
                return;
            }

            $(".dropdown-list").remove();

            var $list = _renderList(), miniHeight = $list.height(), initPosition = $target.position();
            $list.css({ "top": (initPosition.top - miniHeight / 2) + "px", "left": initPosition.left + "px" });
            if (options.css != undefined) {
                $list.css(options.css);
            }

            var maxHeight;
            if (options.items.length > maxShownItemCount) {
                maxHeight = miniHeight + singleItemCount * maxShownItemCount;
            }
            else {
                maxHeight = options.items.length * singleItemCount + miniHeight;
            }

            $list.show();

            $list.animate({ top: (options.items.length > maxShownItemCount ? initPosition.top - miniHeight / 2 : parseInt($list.css("top"))) + "px",
                            height: maxHeight + "px" }, 100, function () {
                if (selectedData) {
                    var $a = options.selectedByText
                        ? $list.find("a[text='" + selectedData.text + "']")
                        : $list.find("a[val='" + selectedData.value + "']");

                    var $item = $list.find("div.items");

                    $item.css("overflow-y", "auto");
                    if ($a.length > 0) {
                        _setSelected($a);
                        var position = $a.position();

                        if (options.items.length > maxHeight) {
                            if (position.top > 50) {
                                $item.scrollTop(position.top - 50);
                            }
                            else {
                                $item.scrollTop(position.top);
                            }
                        }
                    }
                }
            });
        }).hover(function () {
            $target.find("a.dropdown-list-nav").show(0);
        }, function () {
            $target.find("a.dropdown-list-nav").hide(0);
        });



        $("body").click(function (e) {

            if ($(e.target).is($target) || $(e.target).parent().is($target)) {
                return;
            }

            var $tag = $target.next();
            if ($tag.hasClass("dropdown-list")) {
                $tag.hide(0);
                $tag.remove();
            }
        });

        function _bindSelectedData(value) {
            if (value) {
                var hasValue = false;
                $.each(options.items, function (i, item) {
                    if ((options.selectedByText && value == item.text) || value == item.value) {
                        selectedData = { text: item.text, value: item.value || item.text };
                        _displaySelectedItem(item.text);

                        hasValue = true;
                        return false;
                    }
                });

                if(hasValue){
                    return;
                }
                var displayValue = options.unit ? value + options.unit : value;
                selectedData = { text: displayValue, value: value };
                _displaySelectedItem(displayValue);
            }
        }

        function _renderList() {
            var $list = $("<div class='dropdown-list'><div class='items'></div></div>");
            if (options.width != undefined) {
                $list.width(options.width);
            }

            var $items = $list.find("div.items");
            if (options.left === false) {
                $items.addClass("right");
            }

            if (options.items.length > maxShownItemCount) {
                $("<a href='javascript:void(0)' class='prev'>^</a>").insertBefore($items);
                $("<a href='javascript:void(0)' class='next'>V</a>").insertAfter($items);
                $list.css({ "padding": "15px 3px 15px 0px", "height": "30px"});
                if (options.css != undefined) {
                    $list.css(options.css);
                }
            }

            $.each(options.items, function (i, item) {
                var value = item.value || item.text;

                $("<a href='javascript:void(0)'></a>")
                    .text(item.text)
                    .attr("val", value).attr("text", item.text).attr("title", item.text)
                    .appendTo($items);
            });
            
            _bindEvent($list);

            $list.insertAfter($target);
            

            return $list;
        }

        function _bindEvent($list) {
            $list.on("click", "a.prev", function (e) {
                e.stopPropagation();

                var $item = $list.find("div.items");
                $item.scrollTop($item.scrollTop() - 25);
            }).on("click", "a.next", function (e) {
                e.stopPropagation();

                var $item = $list.find("div.items");
                $item.scrollTop($item.scrollTop() + 25);
            }).on("click", "div.items a", function (e) {
                e.stopPropagation();

                $list.find("a.selected").removeClass("selected").find("i").remove();
                _setSelected($(this));

                if (selectedData) {
                    $target.find("span").text(selectedData.text).val(selectedData.text).attr("title", selectedData.text);
                }
                if (options.onItemSelected && options.onItemSelected.constructor == Function) {
                    options.onItemSelected(selectedData);
                }

                $list.remove();
                $('.setting-content').focus();
            });


            $("div.items", $list).on("mousewheel", function (e) {
                var $this = $(this);

                if ($this.scrollTop() == 0) {
                    $this.prev().hide(0);
                    $this.parent().css("padding-top", "3px");

                }
                else if ($this[0].scrollHeight - $this.height() == $this.scrollTop()) {
                    $this.next().hide(0);
                    $this.parent().css("padding-bottom", "3px");
                    //e.preventDefault();
                }
                else {
                    $this.prev().show(0);
                    $this.next().show(0);

                    $this.parent().css("padding-top", "15px");
                    $this.parent().css("padding-bottom", "15px");
                }
                e.stopPropagation();
            });
        }

        function _setSelected($item) {
            selectedData = { text: $item.text(), value: $item.attr("val") };
            $item.addClass("selected").append("<i>*</i>");
        }

        function _displaySelectedItem(text) {
            $($display[0]).text(text);
        }


        this.getValue = function () {
            return selectedData;
        };

        this.setValue = function (value) {
            if (value != undefined) {
                _bindSelectedData(value);
            }
        };
    }
})(uc);