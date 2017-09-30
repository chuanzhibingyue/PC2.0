/**
 * Created with JetBrains WebStorm.
 * User: hougang.chen
 * Date: 16-2-24
 * Time: 下午7:52
 * To change this template use File | Settings | File Templates.
 */
(function (uc) {
	var defaults = {
		dimension: 22
	};

	uc.modules.component.circleLoader = function (_options) {
		if (!_options.target || !_options.callBack) {
			return;
		}
		if (!_options.target.jquery) {
			this.$target = $("#" + _options.target);
		} else {
			this.$target = _options.target;
		}
		this.options = $.extend({}, defaults, _options);
		this.init();
	}
	jQuery.extend(uc.modules.component.circleLoader.prototype, {
		init: function () {
			this.createsvg();
			this.OnProgress(0);
		},
		createsvg: function () {
			var half = this.options.dimension / 2;
			var quarter = this.options.dimension / 4;
			var area = Math.PI * 2 * quarter;
			var svg =
				'<svg xmlns:svg="http://www.w3.org/2000/svg"' +
					'xmlns="http://www.w3.org/2000/svg"' +
					'>' +
					'<circle r="' + half +
					'" cx="' + half +
					'" cy="' + half +
					'"/>' +
					'<circle r="' + (quarter + 0.5) + // +0.5 to debug non-webkit based browsers
					'" cx="' + half +
					'" cy="' + half + '"' +
					'style="stroke-width:' + half + 'px;' +
					'stroke-dasharray:' + '0px' + ' ' + area + ';' +
					'"/>' +
					'</svg>';
			this.$target.prepend(svg);
		},
		clear: function () {
			this.$target = null;
			this.options = null;
		},
		OnProgress: function (_progress) {
			var area = 2 * Math.PI * ((this.options.dimension / 4) + 0.4); // +0.4 to debug non webkit browsers
			var strokeEndValue = _progress * area / 100;
			var $target = this.$target.find('svg circle:nth-child(2)');
			$target.css('stroke-dasharray', strokeEndValue + 'px' + ' ' + area + 'px');
			if (_progress >= 99) {
				this.options.callBack(this.options.taskId);
				this.clear();
			}
		}
	});
})(uc);


