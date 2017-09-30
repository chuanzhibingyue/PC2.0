/**
 * Created by zhen.xu.
 * User: zhen.xu
 * Date: 15-12-15 上午10:05
 */

(function (uc) {
	/*
	 $(document).keyup(function (e) {
	 if (e.keyCode == 27) {
	 uc.util.WindowUtil.close();
	 }
	 });*/
	uc.modules.call.ChooseCallContacts = {
		init: function () {
			this.btnOK = $('.footer .btn-ok');
			this.btnCancel = $('.footer .btn-cancel');
			this.containner = $('.contacts-wrap');
			this.currentContactsContainer = $('ul.contacts-list', this.containner);
			this.loading = $('.contacts-loading', this.containner);
			this.loadError = $('.contacts-load-failed', this.containner);
			this.btnRetry = $('a', this.loadError);
			this.searchContainer = $('.search-input-container');
			this.searchingContainer = $('.contacts-search-container');
			this.searchInput = $('.search-input', this.searchContainer);
			this.searchDelete = $('.icon-search-del', this.searchContainer);
			this.searching = $('.contacts-searching', this.searchingContainer);
			this.searchNoResult = $('.contacts-search-noresult', this.searchingContainer);
			this.searchContactsContainer = $('.contacts-search-list', this.searchingContainer);
			this.panelLoading = $('.chooseCallContacts .panelloading');
			this.chooseContacts = new uc.modules.call.ChooseCallContactsDataSet();
			this.options = uc.util.StorageUtil.getData('chooseCallContacts');//groupId ,userList
			uc.util.LogUtil.info('ChooseCallContacts', 'options', 'options=', this.options);
			this.myUserInfo = uc.IContactService.getCurrentUserInfo();
			this.selectedIds = this.options.userList || [];
			this.defaultSelectedCount = this.selectedIds.length;
			this.maxSelectCount = uc.constants.Call.MaxJoinCount;
			this.groupMemberList = null;
			this.contacts = null;
			this.isCreatingCall = false;
			//已选择列表
			this.selectContactsConatiner = $('.selectContainer');
			this.selectContactsConatinerList = $('ul', this.selectContactsConatiner);

			this.initContacts();
			this.bindEvent();
		},
		initContacts: function () {

			//select default select item
			if (this.myUserInfo && this.selectedIds.indexOf(this.myUserInfo.userId) < 0) {
				this.selectedIds.unshift(this.myUserInfo.userId);
			}
			this.defaultSelectedCount = this.selectedIds.length;

			//check is show group
			if (this.options.groupId) {
				var group = uc.IGroupService.getGroupInfo(parseInt(this.options.groupId));
				uc.util.LogUtil.info('ChooseCallContacts', 'initContacts', 'get group data:', group);
				if (group && group.members.length) {
					this.groupMemberList = group.members;
					var groupmemberlen = group.members.length;
					var userIds = [];
					for (var i = 0; i < groupmemberlen; i++) {
						var contactId = group.members[i].userId;
						userIds.push(contactId);
					}
					if (userIds.length != 0) {
						uc.IContactService.getBasicContactInfo(userIds);
					}
				}
				else {
					this.loadingError();
				}
			} else {
				this.getContactsInfo();
				//if selectContact is not empty，is reload
				if (!this.chooseContacts.getAllContacts().length) {
					if (this.selectedIds != 0) {
						uc.IContactService.getBasicContactInfo(this.selectedIds);
					}
				}
			}
		},
		startLoading: function () {
			this.loadError.addClass('hide');
			this.currentContactsContainer.addClass('hide');
			this.loading.removeClass('hide');
		},
		loadingFinish: function () {
			this.loading.addClass('hide');
			this.loadError.addClass('hide');
			this.currentContactsContainer.removeClass('hide');
		},
		loadingError: function () {
			this.loading.addClass('hide');
			this.loadError.removeClass('hide');
		},
		getContactsInfo: function () {
			var self = this;
			uc.IContactService.getContactList(0, 1000, function (args) {
				uc.util.LogUtil.info('ChooseCallContacts', 'getContactsInfo', 'result', args);
				var errorCode = args[0];
				if (errorCode == 0) {
					self.loadingFinish();
					var contacts = args[1];
					contacts = self.getAlphabeticalSortArray(contacts, "pinyin", "displayName");
					var selectedUserId = {};
					for (var j = 0; j < self.defaultSelectedCount; j++) {
						var _uid = self.selectedIds[j];
						selectedUserId[_uid] = _uid;
					}
					for (var i = 0; i < contacts.length; i++) {
						/*
						 if(i-1<0){
						 self.setContactView(contacts[i], self.currentContactsContainer, true, true,contacts[i].sortKey);
						 }else{
						 if(contacts[i].sortKey != contacts[i-1].sortKey){
						 self.setContactView(contacts[i], self.currentContactsContainer, true, true,contacts[i].sortKey);
						 }
						 }*/
						if (selectedUserId[contacts[i].userId]) {
							self.setContactView(contacts[i], self.currentContactsContainer, true, true);
						} else {
							self.setContactView(contacts[i], self.currentContactsContainer, false, false);
						}
					}
				} else {
					self.loadingError();
				}
			});
		},
		getAlphabeticalSortArray: function (array, firstAttribute, secondAttribute) {
			//Group: firstAttribute: pinyin, secondAttribute:name
			//Contact: firstAttribute: namePinYin, secondAttribute: displayName
			var _self = this;
			var alphabeticalGroups = {};

			$.each(array, function (k, item) {
				var key = "";
				if (item.hasOwnProperty(firstAttribute) && item[firstAttribute].length > 0) {
					item.sortKey = item[firstAttribute].substr(0, 1);
				} else {
					item.sortKey = "#";
					item[firstAttribute] = "#";
				}
			});

			//1, sort by first word, and PinYin properties
			var sortedItems = array.sort(uc.util.CollectionUtil.dynamicSortMultiple("sortKey", firstAttribute, secondAttribute));
			var poundKeyLastSortedArray = _self.sortItemsWithPoundKeyLast(sortedItems, "sortKey");
			return poundKeyLastSortedArray;
		},
		sortItemsWithPoundKeyLast: function (items, key) {
			var poundKeyItems = items.filter(function (item) {
				return item[key] === "#";
			});
			var otherItems = items.filter(function (item) {
				return item[key] !== "#";
			});
			var newItems = $.merge(otherItems, poundKeyItems);
			return newItems;
		},
		setContactView: function (contact, parentNode, bChecked, disabled, isTitle) {
			var contactView = new uc.modules.call.ChooseCallContactsView();
			contactView.init(contact, parentNode, bChecked, disabled, isTitle);
		},
		bindEvent: function () {
			var self = this;
			uc.IUIService.bindCrossWindowEvent(uc.constants.UI.CrossWindowEventName.chooseContactsCreateCall, function (_eventData) {
				if (_eventData) {
					if (_eventData.callStatus == uc.constants.UI.chooseCallContacts.createcall_fail) {
						if (_eventData.failedType) {
							switch (_eventData.failedType) {
								case uc.constants.Call.CallFailedType.CreateFailed:
									uc.modules.component.PromptMessageBox.open({
										title: uc_resource.WindowTitle.Notification,
										message: uc_resource.Call.CreateCallFailed,
										buttonMode: "ok",
										OkText: uc_resource.Common.OK
									}, function () {
										this.close();
									});
									break;
								case uc.constants.Call.CallFailedType.InvitedCall:
									uc.modules.component.PromptMessageBox.open({
										title: uc_resource.WindowTitle.Notification,
										message: uc_resource.Call.InvitedUserFailed,
										buttonMode: "ok",
										OkText: uc_resource.Common.OK
									}, function () {
										this.close();
									});
									break;
								case uc.constants.Call.CallFailedType.BusyUserStatus:
									uc.modules.component.PromptMessageBox.open({
										title: uc_resource.WindowTitle.Notification,
										message: _eventData.message,
										buttonMode: "ok",
										OkText: uc_resource.Common.OK
									}, function () {
										this.close();
									});

									var i = 0;
									for (i = 0; i < _eventData.failedData.busyUserId.length; i++) {
										if (self.myUserInfo.userId != _eventData.failedData.busyUserId[i]) {
											self.updateContacItemChecked(_eventData.failedData.busyUserId[i]);
											self.selectContactsConatinerList.find("." + _eventData.failedData.busyUserId[i] + "").remove();
										}
									}
									break;
								case uc.constants.Call.CallFailedType.GetUserStatus:
									uc.modules.component.PromptMessageBox.open({
										title: uc_resource.WindowTitle.Notification,
										message: uc_resource.Call.GetUserCallStatusFailed,
										buttonMode: "ok",
										OkText: uc_resource.Common.OK
									}, function () {
										this.close();
									});
									break;
								case uc.constants.Call.CallFailedType.SelectedUser:
									uc.modules.component.PromptMessageBox.open({
										title: uc_resource.WindowTitle.Notification,
										message: uc_resource.Call.SelectedInvitedUser,
										buttonMode: "ok",
										OkText: uc_resource.Common.OK
									}, function () {
										this.close();
									});
									break;
							}
						}
						self.hideCreateCallLoading();
					} else {
						self.closeWindow();
					}
				}
			});
			var _groupId = self.options.groupId;
			this.btnOK.on('click', function () {
				uc.util.LogUtil.info('ChooseCallContacts', 'bindEvent', 'btnOK click data:', self.chooseContacts.getAllContacts());
				if ($(this).hasClass('disabled')) {
					return;
				}
				self.contacts = self.chooseContacts.getAllContacts();
				if (self.options.groupId && self.groupMemberList) {
					if (self.groupMemberList.length == self.contacts.length) {
						self.groupMemberList.sort(self.comparefun("userId"));
						self.contacts.sort(self.comparefun("userId"));
						for (var j = 0; j < self.groupMemberList.length; j++) {
							if (self.groupMemberList[j].userId != self.contacts[j].userId) {
								_groupId = undefined;
								break;
							}
						}
					}
					else
						_groupId = undefined;
				}

				//contactlist delete myself
				for (var i = 0; i < self.contacts.length; i++) {
					if (self.contacts[i].userId == self.myUserInfo.userId) {
						self.contacts.splice(i, 1);
						break;
					}
				}
				uc.IUIService.triggerCrossWindowEvent(self.options.key, { okOrCancel: true, groupId: _groupId, memberList: self.contacts});
				self.showCreateCallLoading();
			});

			this.btnCancel.on('click', function () {
				uc.util.WindowUtil.close();
			});

			this.searchContainer.on('click', function () {
				$(this).addClass('focus');
				self.searchInput.focus();
			});

			this.searchInput.on('keyup', function () {
				var _text = $.trim($(this).val());
				if (_text) {
					self.searchDelete.show();
					self.search(_text);
				} else {
					self.searchDelete.hide();
					self.stopSearching();
				}
			});

			this.searchDelete.on('click', function () {
				self.searchInput.val('');
				self.stopSearching();
				$(this).hide();
			});

			this.btnRetry.on('click', function () {
				self.startLoading();
				setTimeout(function () {
					self.initContacts();
				}, 500);
			});
		},
		search: function (keyword) {
			this.startSearching();
			uc.ISearchService.searchServer(keyword, 0, 20, [1], function (args) {
				uc.util.LogUtil.info('ChooseCallContacts', 'search', 'call contacts search count:', args[1]);
			})
		},
		startSearching: function () {
			this.searchContactsContainer.empty();
			this.searchingContainer.removeClass('hide');
			this.searching.removeClass('hide');
			this.searchNoResult.addClass('hide');
		},
		finishSearching: function () {
			this.searching.addClass('hide');
			this.searchNoResult.addClass('hide');
			this.searchContactsContainer.removeClass('hide');
			this.searchContactsContainer.empty();
		},
		stopSearching: function () {
			this.searchingContainer.addClass('hide');
		},
		noSearchResult: function () {
			this.searching.addClass('hide');
			this.searchContactsContainer.addClass('hide');
			this.searchNoResult.removeClass('hide');
		},
		showSelectCount: function (_input, userId) {
			this.btnOK.addClass('disabled');
			var inCallCount = 0;
			var selectedCount = this.chooseContacts.getAllContacts().length;
			//选中的大于1，或者人数等于1但是userid不等于当前userid
			if (selectedCount > 1 || (selectedCount == 1 && !this.chooseContacts.isContactExist(this.myUserInfo.userId))) {
				this.btnOK.removeClass('disabled');
				/*
				 if(this.options.groupId){
				 inCallCount=selectedCount;
				 }else if(this.selectedIds.length){
				 inCallCount=selectedCount + this.selectedIds.length;
				 } */
				inCallCount = selectedCount;
				if (inCallCount > this.maxSelectCount) {
					this.removeChooseContact(userId);
					this.removeSelectContactItem(userId);
					_input.prop('checked', false);
					--inCallCount;
					--selectedCount;
					uc.modules.component.PromptMessageBox.open({title: uc_resource.WindowTitle.Notification, message: uc_resource.Call.ChooseMaxContactsTips, buttonMode: "ok", OkText: uc_resource.Common.OK},
						function (result) {
							this.close();
						});
				}
				/*
				 if (inCallCount == this.defaultSelectedCount) {
				 this.btnOK.addClass('disabled');
				 }*/
			} else {
				this.btnOK.addClass('disabled');
			}

			this.btnOK.find('span').text(selectedCount);
		},
		setSearchData: function (contacts) {
			if (contacts.length) {
				this.finishSearching();
				for (var i = 0; i < contacts.length; i++) {
					var disabled = (this.selectedIds.indexOf(contacts[i].userId) >= 0 ? true : false);
					var bChecked = this.chooseContacts.isContactExist(contacts[i].userId);
					this.setContactView(contacts[i], this.searchContactsContainer, bChecked, disabled)
				}
			} else {
				this.noSearchResult();
			}
		},
		addChooseContact: function (userId, contact) {
			this.chooseContacts.addContact(userId, contact);
		},
		removeChooseContact: function (userId) {
			this.chooseContacts.deleteContact(userId);
		},
		OnContactInfoReceived: function (errorCode, userIds, contacts) {

		},
		OnGroupInfoReceived: function (errorCode, groupId, group) {

		},
		OnBasicContactInfoReceived: function (contacts) {
			if (contacts && contacts.length && !this.isCreatingCall) {
				var len = contacts.length;
				var self = this;
				contacts.map(function (contact) {
					var contactId = contact.userId;
					var disabled = self.selectedIds.indexOf(contactId) >= 0 ? true : false;
					if(self.options.groupId){
						if (len <= self.maxSelectCount) {
							if (!disabled) {
								self.setContactView(contact, self.currentContactsContainer, true, disabled);
								self.addSelectContactItem(contact, disabled);
								self.addChooseContact(contactId, contact);
							} else {
								self.setContactView(contact, self.currentContactsContainer, false, disabled);
								self.addSelectContactItem(contact, disabled);
								self.addChooseContact(contactId, contact);
							}
						} else {
							self.setContactView(contact, self.currentContactsContainer, false, disabled);
							if (contactId == self.myUserInfo.userId) {
								self.addSelectContactItem(contact, disabled);
								self.addChooseContact(contactId, contact);
							}
						}
					}else{
						self.addSelectContactItem(contact, true);
						self.addChooseContact(contactId, contact);
					}
				})
			}
			this.loadingFinish();
			//update usercount
			var selectUserCount = this.chooseContacts.getAllContacts().length;
			this.btnOK.find('span').text(selectUserCount);
			this.btnOK.addClass('disabled');
			if (selectUserCount <= this.maxSelectCount) {
				if (selectUserCount > 1 || (selectUserCount == 1 && !this.chooseContacts.isContactExist(this.myUserInfo.userId))) {
					this.btnOK.removeClass('disabled');
				}
			}
		},

		OnServerSearchResultReceived: function (errorCode, searchId, contacts) {
			uc.util.LogUtil.info('ChooseCallContacts', 'OnServerSearchResultReceived', 'result', {errorCode: errorCode, contacts: contacts});
			if (errorCode == uc.ErrorCode.Success) {
				this.setSearchData(contacts);
			} else {
				this.stopSearching();
			}
		},
		createSelectContactItem: function (_contact, _parentNode, _disabled) {
			//已选择列表生成
			var selectContactView = new uc.modules.call.ChooseCallSelectContactsView();
			selectContactView.init(_contact, _parentNode, _disabled);
		},
		addSelectContactItem: function (_userdata, disable) {
			//add selectContactsItem to  selectContactslist
			var selectedCount = this.chooseContacts.getAllContacts().length;
			if (selectedCount > this.maxSelectCount) {
				return false;
			}
			this.createSelectContactItem(_userdata, this.selectContactsConatinerList, disable);
		},
		removeSelectContactItem: function (_userId) {
			//remove selectContactsItem from  selectContactslist
			this.selectContactsConatinerList.find("li[class='" + _userId + "']").remove();
		},
		updateContacItemChecked: function (_userId) {

			//右边已选列表，点击删除按钮的时候，清除搜索列表和当前联系人列表data和view
			this.removeChooseContact(_userId);
			$('input', this.currentContactsContainer.find("li[class='" + _userId + "']")).prop('checked', false);
			$('input', this.searchContactsContainer.find("li[class='" + _userId + "']")).prop('checked', false);
			var selectedCount = this.chooseContacts.getAllContacts().length;
			this.btnOK.addClass('disabled');
			if (selectedCount > 1) {
				this.btnOK.removeClass('disabled');
			} else if (selectedCount == 1) {
				if (!this.chooseContacts.isContactExist(this.myUserInfo.userId)) {
					this.btnOK.removeClass('disabled');
				}
			} else {
				this.btnOK.addClass('disabled');

			}
			this.btnOK.find('span').text(selectedCount);
		},
		showCreateCallLoading: function () {
			this.isCreatingCall = true;
			this.panelLoading.show();
		},
		hideCreateCallLoading: function () {
			this.isCreatingCall = false;
			this.panelLoading.hide();
		},
		closeWindow: function () {
			this.hideCreateCallLoading();
			uc.util.WindowUtil.close();
		},
		comparefun: function (propertyName) {
			return function (object1, object2) {
				var value1 = object1[propertyName];
				var value2 = object2[propertyName];
				if (value2 < value1) {
					return -1;
				} else if (value2 > value1) {
					return 1;
				}
				else {
					return 0;
				}
			}
		}
	};
	jQuery(document).ready(function () {
		uc.init();
		uc.modules.call.ChooseCallContacts.init();
	});
})(uc);