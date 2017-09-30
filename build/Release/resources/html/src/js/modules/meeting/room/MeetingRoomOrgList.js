/**
 * User: hougang.chen
 * Date: 16-5-5
 * Time: 下午2:04
 */
(function (uc) {
	uc.modules.meeting.room.MeetingRoomOrgList = {
		init: function (roomDataMap, clickRoomItemCallback, clickOrgItemCallback) {
			this.orgRootNode = $('.meetingRoom');
			this.orgListNode = this.orgRootNode.find('.meetingRoom-org-list');
			this.loadingGif = this.orgRootNode.find('.loading');
			this.loadError = this.orgRootNode.find('.loadError');

			this.roomDataMap = roomDataMap;
			this.clickRoomItemCallback = clickRoomItemCallback;
			this.clickOrgItemCallback = clickOrgItemCallback;

			this.currentuserInfo = uc.IContactService.getCurrentUserInfo();

			//当前增加的roomid,用来记录新增完成之后，增加select样式
			this.currentAddRoomId = -1;

			this.bindEvents();
		},

		bindEvents: function () {
			var self = this;
			this.orgListNode.on('click', '.child', function (e) {
				var target = $(this);

				self.orgListNode.find('.child').removeClass('selectedChild');
				target.addClass('selectedChild');

				var orgid = target.attr('orgid');
				if (orgid != undefined) {
					//点击了组织架构
					//收起
					if (self.orgListNode.find(".child[parentid=" + orgid + "]").length > 0) {
						//清空子组织所有子节点
						target.find('.open').removeClass('open').addClass('close');
						self.removeAllSubOrgByParentId(orgid);
					} else {//展开
						uc.IContactService.getOrgInfo(parseInt(orgid), function (data) {
							uc.modules.meeting.room.MeetingRoom.getOrgInfoTaskId = data[1];
						});
						target.find('.orgloading').removeClass('hide');
					}
					if ($.isFunction(self.clickOrgItemCallback)) {
						self.clickOrgItemCallback(orgid);
					}
				} else {
					//点击了会议室
					var roomId = target.attr('roomid');
					//向room发送roomid，刷新roomDetails
					if ($.isFunction(self.clickRoomItemCallback)) {
						self.clickRoomItemCallback(roomId);
					}
				}
			})
		},

		//组织架构数据返回
		renderOrgPanel: function (errorCode, departmentData) {

			this.loadingGif.hide();

			if (errorCode == uc.ErrorCode.Success) {
				var parentOrgId = departmentData.parentId;
				var orgId = departmentData.id;
				var OrgNode = null;
				if (parentOrgId == 0) {
					//render根组织
					OrgNode = this.createOrganizaitionNode(departmentData);
					this.orgListNode.find('*').off();
					this.orgListNode.empty();
					this.orgListNode.append(OrgNode);

					if ($.isFunction(this.clickOrgItemCallback)) {
						this.clickOrgItemCallback(orgId);
					}
				} else {
					//render选中组织的子组织
					var queryOrgNode = this.orgListNode.find(".child[orgid=" + orgId + "]");
					//更改当前组织图标状态
					queryOrgNode.find('.orgloading').addClass('hide');
					queryOrgNode.find('.close').removeClass('close').addClass('open');
					//创建子组织
					var depth = parseInt(queryOrgNode.attr("depth"));
					OrgNode = this.createOrganizaitionNode(departmentData, depth);
					queryOrgNode.after(OrgNode);

					//this.OnMeetingRoomListReceived(0, orgId);
				}

				uc.IMeetingRoomService.getMeetingRoomList(orgId, function (data) {
					uc.util.LogUtil.info("uc.modules.meeting.room.MeetingRoomOrgList", "getMeetingRoomList", " data", data);
				});

			} else {
			}
		},

		//组织架构id为orgId下所有会议室列表
		OnMeetingRoomListReceived: function (errorCode, orgId, lastGetTime, rooms) {
			if (errorCode == uc.ErrorCode.Success) {

				var queryOrgNode = this.orgListNode.find(".child[orgid=" + orgId + "]");
				var depth = parseInt(queryOrgNode.attr("depth"));
				var parentId = parseInt(orgId);
				var validRooms = [];
				if (rooms) {
					for (var i = 0, len = rooms.length; i < len; i++) {
						if (this.checkRoomIsVisible(rooms[i])) {
							var room = new uc.modules.meeting.room.info.MeetingRoomInfo();
							room.update(rooms[i]);
							this.roomDataMap.addRoom(room.roomId, room);
							validRooms.push(room);
						}
					}
				}
				queryOrgNode.after(this.createRoomNode(validRooms, parentId, depth + 1));

				if (this.currentAddRoomId != -1) {
					this.orgListNode.find('.child[roomid=' + this.currentAddRoomId + ']').trigger('click');
					this.currentAddRoomId = -1
				}
			}
		},

		//检测房间是否只对参会人可见
		checkRoomIsVisible: function (roomInfo) {
			if (roomInfo.defaultAttendsVisible == 0) {
				return true
			} else {
				//检测当前玩家是否在会议室参会人中
				return this.checkUserIsAttendee(roomInfo, this.currentuserInfo);
			}
			return false;
		},

		//检测房间是否只对参会人可进
		checkRoomIsJoin: function (roomInfo) {
			if (roomInfo.defaultAttendsJoin == 0) {
				return true
			} else {
				//检测当前玩家是否在会议室参会人中
				return this.checkUserIsAttendee(roomInfo, this.currentuserInfo);
			}
			return false;
		},

		checkUserIsAttendee: function (roomInfo, userInfo) {
			//如果当前用户具有管理员权限,return true
			if(this.isAdmin){
				return true;
			}
			//如果当前是主持人，return true
			if (roomInfo.roomHostId == userInfo.userId || roomInfo.roomHostId == userInfo.account) {
				return true;
			}
			for (var i = 0, len = roomInfo.attendees.length; i < len; i++) {
				if (roomInfo.attendees[i].account == userInfo.account || roomInfo.attendees[i].account == userInfo.userId) {
					return true;
				}
			}
			return false;
		},

		createOrganizaitionNode: function (_departmentData, _depth) {
			var nodeStr = "";
			var self = this;
			var depth = _depth == undefined ? 0 : _depth;
			//第一层
			if (depth == 0) {
				nodeStr += self.getOrganizaitionItemTemplate(_departmentData, depth, false);
			}
			depth++;
			//遍历下一层
			$.each(_departmentData.subDepartments, function (k, department) {
				nodeStr += self.getOrganizaitionItemTemplate(department, depth, true);
				//如果当前节点还有下一层，继续遍历
				if (department.subDepartments.length > 0) {
					self.createOrganizaitionNode(department, depth + 1);
				}
			});
			return $(nodeStr);
		},

		createRoomNode: function (roomDatas, parentid, _depth) {
			var nodeStr = "";
			var self = this;
			var depth = _depth == undefined ? 0 : _depth;
			nodeStr += self.getMeetingRoomItemTemplate(roomDatas, parentid, depth);
			return $(nodeStr);
		},

		getOrganizaitionItemTemplate: function (_department, _depth, isExpanded) {
			var iconElementClass = isExpanded ? "close" : "open";
			var isselect = !isExpanded ? 'selectedChild' : '';
			var nodeStr = '<div class="org-child child ' + isselect + '" parentid="' + _department.parentId + '" orgid="' + _department.id + '" depth="' + _depth + '" >'
				+ '<li class="organization-item item" style="padding-left:' + (_depth * 20) + 'px">'
				+ '<a class="' + iconElementClass + '"></a>'
				+ '<a class="orgloading hide"></a>'
				+ '<a class="organization-name">' + _department.departmentName + '</a>'
				+ '<div class="clear"/>'
				+ '</li>'
				+ '</div>';

			return nodeStr;
		},

		getMeetingRoomItemTemplate: function (roomDatas, parentid, _depth) {
			var nodeStr = "";
			var enable = ""
			for (var i = 0; i < roomDatas.length; i++) {
				//检测是否可以进入
				if (this.checkRoomIsJoin(roomDatas[i])) {
					enable = '';
				} else {
					enable = 'couldnotJoin'
				}
				nodeStr += '<div class="room-child child ' + enable + '" parentid="' + parentid + '" roomid="' + roomDatas[i].roomId + '" depth="' + _depth + '" >' +
					'<li class="room-item item" style="padding-left:' + (_depth * 20) + 'px">' +
					'<a class="roomList"></a>' +
					'<a class="roomName">' + roomDatas[i].title + '</a>' +
					'</li>' +
					'</div>';
				//test
				this.roomDataMap.addRoom(roomDatas[i].roomId, roomDatas[i]);
			}
			return nodeStr;
		},

		removeAllSubOrgByParentId: function (parentId) {
			var self = this;
			var operationNode = this.orgListNode.find(".child[parentid=" + parentId + "]");
			if (operationNode.length > 0) {
				$.each(operationNode, function (k, subNode) {
					var parentid = $(subNode).attr("orgid");

					//从dataMap里面移除数据
					var roomid = $(subNode).attr("roomid");
					if (roomid != undefined) {
						self.roomDataMap.deleteRoom(roomid);
					}

					$(subNode).find('*').off();
					$(subNode).remove();
					self.removeAllSubOrgByParentId(parentid);
				});
			}
		},

		OnMeetingRoomCreated: function (meetingRoom) {

			var roomInfo = new uc.modules.meeting.room.info.MeetingRoomInfo();
			roomInfo.update(meetingRoom);

			var orgId = roomInfo.orgId;
			var roomId = roomInfo.roomId;

			var orgNode = this.orgListNode.find(".child[orgid=" + orgId + "]");
			var depth = parseInt(orgNode.attr("depth"));
			//查询以当前orgid为父id的所有子元素
			var subchild = orgNode.find("a.open");
			if (subchild.length > 0) {
				//已经展开，直接新增
				if (this.roomDataMap.isExistRoom(roomId)) {
					uc.util.LogUtil.warn("uc.modules.meeting.room.MeetingRoomOrgList", "OnMeetingRoomCreated", " ID is not unique=", [roomInfo, this.roomDataMap]);
				} else {
					this.roomDataMap.addRoom(roomId, roomInfo);
				}
				orgNode.after(this.createRoomNode([roomInfo], orgId, depth + 1));

				this.orgListNode.find('.child[roomid=' + roomId + ']').trigger('click');
			} else {
				this.currentAddRoomId = roomId;
				//展开查询
				orgNode.trigger('click');
			}
		},

		OnMeetingRoomUpdated: function (meetingRoom) {

			var roomInfo = new uc.modules.meeting.room.info.MeetingRoomInfo();
			roomInfo.update(meetingRoom);

			var roomId = roomInfo.roomId;

			if (this.roomDataMap.isExistRoom(roomId)) {
				this.roomDataMap.updateRoom(roomId, roomInfo);
				var roomNode = this.orgListNode.find(".room-child[roomid=" + roomId + "]");
				//检测是否可见
				if (!this.checkRoomIsVisible(roomInfo)) {
					//删掉当前节点，并且通知meetingRoom刷新detail
					this.OnMeetingRoomDeleted(roomInfo.roomId);
					if ($.isFunction(this.clickRoomItemCallback)) {
						this.clickRoomItemCallback(null);
					}
					return;
				}
				//检测是否可以进入
				if (this.checkRoomIsJoin(roomInfo)) {
					roomNode.removeClass('couldnotJoin');
				} else {
					roomNode.addClass('couldnotJoin');
				}
				roomNode.find('.roomName').text(roomInfo.title);

			} else {
				uc.util.LogUtil.warn("uc.modules.meeting.room.MeetingRoomOrgList", "OnMeetingRoomUpdated", " roomId Does not exist=", [roomInfo, this.roomDataMap]);
			}
		},

		OnMeetingRoomDeleted: function (roomId) {
			if (this.roomDataMap.isExistRoom(roomId)) {
				var roomNode = this.orgListNode.find(".room-child[roomid=" + roomId + "]");
				var orgNode = this.orgListNode.find(".child[orgid=" + roomNode.attr('parentid') + "]");
				orgNode.addClass('selectedChild');

				roomNode.find('*').off();
				roomNode.remove();

				this.roomDataMap.deleteRoom(roomId);
			} else {
				uc.util.LogUtil.warn("uc.modules.meeting.room.MeetingRoomOrgList", "OnMeetingRoomDeleted", " roomId Does not exist=", [roomId, this.roomDataMap]);
			}
		},
		setAdmin: function(isAdmin){
			this.isAdmin = isAdmin;
		}
	}
})(uc);