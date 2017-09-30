/**
* Created by lei.zhou on 2015/7/31.
*/
(function (uc) {
    "use strict";

    uc.modules.component.TimeBox = function (container) {
        if (!container) {
            return;
        }

        var $container, me = this;
        if (container.jquery) {
            $container = container;
        }
        else {
            $container = $("#" + container);
        }

        var $box = $("<div class='time-box'><input type='text' class='time-hour' maxlength='2' /><span class='time-split'>:</span><input type='text' class='time-minute' maxlength='2' /></div>").appendTo($container);
        $(".time-hour", $box).keyup(function () {
            var $this = $(this), value = $this.val().replace(/\D/g, "");

            if (value == "") {
                $this.val(value);
                return;
            }

            var first = value.substr(0, 1);
            if (first != "0" && first != "1" && first != "2") {
                $this.val("0" + first);
                return;
            }

            if (value.length > 1) {
                var second = value.substr(1, 1);
                if (first == "2") {
                    if (second != "0" && second != "1" && second != "2" && second != "3") {
                        $this.val("23");
                    }
                    else {
                        $this.val(first + second);
                    }
                }
                else {
                    $this.val(value.substr(0, 2));
                }
            }
            else {
                $this.val(first);
            }
        }).blur(function () {
            var $this = $(this), value = $this.val().replace(/\D/g, "");

            if (value == "") {
                $this.val(value);
                return;
            }

            if (value.length == 1) {
                $this.val("0" + value);
            }
            else {
                $this.val(value.substr(0, 2));
            }
        }).on("drop", function () {
            return false;
        }).focus(function () {
            var $this = $(this);
            setTimeout(function () {
                $this.select();
            }, 100);
            return false;
        });

        $(".time-minute", $box).keyup(function () {
            var $this = $(this), value = $this.val().replace(/\D/g, "");

            if (value == "") {
                $this.val(value);
                return;
            }

            var first = value.substr(0, 1);
            if (first != "0" && first != "1" && first != "2" && first != "3" && first != "4" && first != "5") {
                $this.val("0" + first);
                return;
            }

            if (value.length > 1) {
                $this.val(value.substr(0, 2));
            }
            else {
                $this.val(first);
            }
        }).blur(function () {
            var $this = $(this), value = $this.val().replace(/\D/g, "");

            if (value == "") {
                $this.val(value);
                return;
            }

            if (value.length == 1) {
                $this.val("0" + value);
            }
            else {
                $this.val(value.substr(0, 2));
            }
        }).on("drop", function () {
            return false;
        }).focus(function (e) {
            var $this = $(this);
            setTimeout(function () {
                $this.select();
            }, 100);
            return false;
        });

        this.getValue = function () {
            var hours = $(".time-hour", $box).val(), minutes = $(".time-minute", $box).val();

            if (hours && minutes) {
                return hours + ":" + minutes;
            }
            else {
                return;
            }
        }

        this.setValue = function (time) {
            if (!time) {
                return;
            }

            var parts = time.split(":"), hours = format(parts[0]), minutes = format(parts[1]);
            
            $(".time-hour", $box).val(hours);
            $(".time-minute", $box).val(minutes);
        }

        function format(input) {
            if (!input) {
                return "00";
            }

            input = input.replace(/\D/g, "");

            if (input == "") {
                input = "00";
            }
            else if (input.length == 1) {
                input = "0" + input;
            }
            else {
                input = input.substr(0, 2);
            }

            return input;
        }
    }
})(uc);