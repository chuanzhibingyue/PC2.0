/**
 * Created by pan.li on 2016/1/4.
 */
(function ($) {
    'use strict';
    $.fn.InputBox = function (options) {
        $(this).each(function () {
                var dom = $(this);
                var input = $('<input class="inputBox" autofocus spellcheck="false"/>');

                var tagName = dom.attr("tagName");
                var tagId = dom.attr("tagId");
                var tagValue = dom.attr("tagValue");
                var tagLength = dom.attr("tagLength");

                input.attr("maxlength", tagLength ? tagLength : uc.constants.Profile.InputBox.SetMaxLength);

                dom.prev().attr("maxwidth", "150px");

                //点击编辑按钮显示文本输入框，带入用户标签内容
                var oldInput;

                dom.unbind().bind('click', function () {
                    oldInput = dom.prev('span').text();
                    dom.hide();
                    dom.siblings().hide();
                    dom.parent().append(input);
                    input.show().focus().val(oldInput);
                });

                //修改后点击enter或鼠标失焦点保存内容
                input.blur(function(){
                    var newInput  = uc.util.StringUtil.escapeHtml($(this).val());

                    if (tagId == uc.constants.Profile.InputBox.EmailTagId) {

                        if (!uc.util.StringUtil.CheckEmail(newInput)) {
                            return;
                        }

                    } else if (tagId == uc.constants.Profile.InputBox.PhoneTagId) {

                        if (!uc.util.StringUtil.MobileCheck(newInput)) {
                            return;
                        }

                    }
                    if (newInput != oldInput) {
                        if (options.type == 1) {
                            var newEdit = {};
                            newEdit[tagName] = newInput.replace(uc.util.StringUtil.spaceReg, String.fromCharCode(160));
                            options.callback(newEdit, $(this));
                        } else {
                            var newTags = {};
                            newTags["tag_id"] = tagId.replace(uc.util.StringUtil.spaceReg, String.fromCharCode(160));
                            newTags["tag_name"] = tagName.replace(uc.util.StringUtil.spaceReg, String.fromCharCode(160));
                            newTags["tag_value"] = newInput.replace(uc.util.StringUtil.spaceReg, String.fromCharCode(160));

                            var list = [];
                            list.push(newTags);
                            var tag = JSON.stringify(list);
                            options.callback(tag, $(this));
                        }
                    }
                    $(this).hide();
                    $(this).siblings().show();

                });
                input.on("keydown", function (event) {
                    if (event.keyCode == uc.constants.Profile.InputBox.EnterKeyCode) {
                        input.blur();
                    }
                })
            }
        );

    }
})(jQuery);