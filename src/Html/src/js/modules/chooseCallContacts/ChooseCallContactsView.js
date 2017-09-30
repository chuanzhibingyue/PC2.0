/**
 * Created by zhen.xu.
 * User: zhen.xu
 * Date: 15-12-15 上午10:08
 */
(function (uc) {
    uc.modules.call.ChooseCallContactsView = function () {
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
                              '<input type="checkbox">' +
                              '<label><span></span></label>' +
                              '</div>' +
                          '</div>';
    }
    jQuery.extend(uc.modules.call.ChooseCallContactsView.prototype, {
        init: function (data, parentNode, bChecked, disabled,isTitle) {
            this.parentNode = parentNode;
            if(isTitle){
                this.nodeTitle=$('<div class="left-title">'+isTitle+'<span></span></div>');
                this.nodeTitle.appendTo(this.parentNode);
                return;
            }
            this.data = data;
            //此处以this.data.userId为classname，主要是为了删除方便
            this.node = $('<li class="'+this.data.userId+'"></li>');
            this.node.append(this.nodeHtml);
            this.userAvatar = $('.left img', this.node);
            this.userName = $('.center .name', this.node);
            this.userDepart = $('.center .depart', this.node);
            this.bChecked = bChecked;
            this.disabled = disabled;
            this.initUI();
        },
        initUI: function () {
            this.setAvatar();
            this.setUserName();
            this.setUserDepart();
            this.node.appendTo(this.parentNode);
            if (this.disabled || this.bChecked) {
                this.node.find('input').prop('checked', true);
                if(!this.disabled){
                    uc.modules.call.ChooseCallContacts.addChooseContact(this.data.userId, this.data);
                }
            }

            if (this.disabled) {
                this.node.find('input').attr('disabled', 'disabled');
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
            this.node.on('click', function () {
                var _input = $(this).find('input');
                if (_input.is(':checked')) {
                    _input.prop('checked', false);
					//delete from data
                    uc.modules.call.ChooseCallContacts.removeChooseContact(self.data.userId);
					//remove from view
					uc.modules.call.ChooseCallContacts.removeSelectContactItem(self.data.userId);
                } else {
                    _input.prop('checked', true);
					//add to data
                    uc.modules.call.ChooseCallContacts.addChooseContact(self.data.userId, self.data);
					//add to view
					uc.modules.call.ChooseCallContacts.addSelectContactItem(self.data,false);
                }
                uc.modules.call.ChooseCallContacts.showSelectCount(_input, self.data.userId);
            });
        },
        deleteSelectedUser:function(userData){
            for(var i=0;i<userData.length;i++){
                var _input=this.parentNode.find("."+userData[i].userId+"").find('input.checked');
                _input.prop('checked', false);
                uc.modules.call.ChooseCallContacts.removeChooseContact(userData[i].userId);
                //remove from view
                uc.modules.call.ChooseCallContacts.removeSelectContactItem(userData[i].userId);
            }
        },
        getChooseCount: function () {
            var _count = this.parentNode.find('input:checked').length;
            uc.modules.call.ChooseCallContacts.showSelectCount(_count);
            console.log('count', _count);
        }
    });
})(uc);
