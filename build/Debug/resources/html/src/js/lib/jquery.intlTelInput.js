/**
 * droplist
 * User: hougang.chen
 * Date: 15-12-31
 * Time: 下午1:21
 **/
(function ($) {
	$.fn.intlTelInput = function (_options) {
		var m_docThis = $(this);
		var isopen = false;
		var m_objdefaultVal = {
			inputWidth: 50,
			inputHeight: 20,
			listWidth: 160,
			listheight: 120,
			data: [],
			selectedDialCode: "",
			alignment: "1",//0左对齐，1水品居中对齐，2右对齐，3顶对齐，4，垂直居中对齐，5底对齐
			callback: null
		};
		var m_objoption = $.extend(m_objdefaultVal, _options);
		var _selectwrapper = $('<div class="jquery-intlTelInput"/>');

		m_docThis.append(_selectwrapper);
		_selectwrapper.prepend('<span>' + m_objoption.selectedDialCode + '</span><a></a>');
		var _list = $('<div class="listbox"/>');
		_selectwrapper.append(_list);
		var tempstr = '<ul class="select_inner">';
		for (var i = 0; i < m_objoption.data.length; i++) {
			var opttext = m_objoption.data[i].name;
			var optval = m_objoption.data[i].dialCode;
			if (m_objoption.selectedDialCode == m_objoption.data[i].name)
				tempstr += '<li id="' + optval + '"><span class="cname hide">' + opttext + '</span><span class="icon-call_singleSelected"/><span class="tel">' + opttext + '</span><span class="dialCode">+' + optval + '</span></li>'
			else
				tempstr += '<li id="' + optval + '"><span class="cname hide">' + opttext + '</span><span class="icon-call_singleSelected unselect"/><span class="tel">' + opttext + '</span><span class="dialCode">+' + optval + '</span></li>';
		}
		tempstr += '</ul>';
		_list.html(tempstr);

		//根据传入参数，更改样式
		_selectwrapper.children('span').width(m_objoption.inputWidth);
		_selectwrapper.children('span').height(m_objoption.inputHeight);

		_selectwrapper.children('.listbox').width(m_objoption.listWidth);
		_selectwrapper.children('.listbox').height(m_objoption.listheight);

		//更改对齐方式
		switch (m_objoption.alignment) {
			case "0":
				_selectwrapper.children('.listbox').css("left", 0);
				break;
			case "1":
				_selectwrapper.children('.listbox').css("left", 45+ "px");
				break;
			case "2":
				break;
			case "3":
				break;
			case "4":
				break;
			case "5":
				break;
		}
		//添加事件
		_selectwrapper.find('li').on('click', function () {
			var val = $(this).attr('id');
            $(this).parent().find("span[class='icon-call_singleSelected']").addClass("unselect");
            $(this).find("span.unselect").removeClass("unselect");
			_selectwrapper.children('span').text($(this).find('.cname').text());
			if (m_objoption.callback)
				m_objoption.callback(val);
		});

		$(document).click(function (e) {
			var target = $(e.target);
			if (!target.parent().hasClass("jquery-intlTelInput") && !target.hasClass("jquery-intlTelInput")) {
				$('.listbox').hide();
			}
		});
		_selectwrapper.on('click', function () {
			if (isopen)
				$(this).find('.listbox').hide();
			else
				$(this).find('.listbox').show();
			isopen = !isopen;
			e.stopPropagation();
		});
	}
})(jQuery);