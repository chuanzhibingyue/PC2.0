/**
 * 呼选择联系人：已选择联系人列表
 * User: hougang.chen
 * Date: 2015/12/25 15:36
 */
(function (uc) {
    uc.modules.call.ChooseCallSelectContactsView = function () {
        var defaultAvatar = uc.constants.UI.DefaultLocalUrl.Avatar_40;
        this.nodeHtml = '<div class="left">' +
                            '<img src="'+ defaultAvatar +'" onerror="uc.util.ImageUtil.OnImageLoadError(this,\'' + defaultAvatar + '\')"  alt=""/>' +
                          '</div>' +
                          '<div class="center">' +
                               '<div class="name"></div>' +
                               '<div class="depart"></div>' +
                          '</div>' +
                          '<div class="right">' +
                              '<div class="checkbox-parent check-box-contact">' +
                              '<label><span></span></label>' +
                              '</div>' +
                          '</div>';
    }
    jQuery.extend(uc.modules.call.ChooseCallSelectContactsView.prototype, {
        init: function (data, parentNode, disabled) {
            this.data = data;
			//此处以this.data.userId为classname，主要是为了删除方便
            this.node = $('<li class="'+this.data.userId+'"></li>');
            this.node.append(this.nodeHtml);
            this.parentNode = parentNode;
            this.userAvatar = $('.left img', this.node);
            this.userName = $('.center .name', this.node);
            this.userDepart = $('.center .depart', this.node);
            this.disabled = disabled;
            this.initUI();
        },
        initUI: function () {
            this.setAvatar();
            this.setUserName();
            this.setUserDepart();
            this.node.appendTo(this.parentNode);
            if (this.disabled) {
                this.node.attr('disabled', 'disabled');
	            this.node.find('span').remove();
                return;
            }
            this.bindEvent();
        },
        setAvatar: function () {

            if (!this.data.localAvatar) {
                if (this.data.userId) {
                    this.userAvatar.attr('data-id', this.data.userId);
                    //uc.IUIService.downloadUserAvatar(this.data.userId, this.data.remoteAvatarUrl);
                }
                return;
            }

            this.userAvatar.attr('src', this.data.localAvatar);
        },
        setUserName: function () {
            this.userName.text(this.data.displayName);
	        this.node.attr("qtips",this.data.displayName);
        },
        setUserDepart: function () {
            this.userDepart.text(this.data.departmentName + '  ' + this.data.position);
        },
        bindEvent: function () {

            var self = this;
            $('span',this.node).on('click', function () {
				uc.modules.call.ChooseCallContacts.updateContacItemChecked(self.data.userId);
				self.destroy();
			});
        },
		destroy: function (){
			this.node.remove();
			this.data = null;
			this.node = null;
			this.nodeHtml = null;
			this.parentNode = null;
			this.userAvatar = null;
			this.userName = null;
			this.userDepart = null;
		},
		getUserId:function ()
		{
			return this.data.userId;
		}
    });
})(uc);
