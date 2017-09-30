#include "UCMessage.h"
#include "MessageStream.h"
#include "LogManager.h"
#include "UCTime.h"

namespace uc {

UCMsg::UCMsg() { 
	_msgType = UCInvalidTMsgype;
	_head = new UcMessageHead(); 
	_body = new UcMessageBody();

	_head->__set_version(MSG_VERSION);
	_head->__set_timestamp(Time::GetSysTime());
}

UCMsg::~UCMsg() {
	if (_head != NULL) {
		delete _head;
		_head = NULL;
	}

	if (_body != NULL) {
		delete _body;
		_body = NULL;
	}
}

int UCMsg::Encode(BinMsg& dest) {
	EncodeBody();
	return MsgStream::Serialize(_head, _body, dest);
}

int UCMsg::Decode(const MsgRecord& data) {
	swap(*this->_head, *data.head);
	DecodeBody(data.body);
	return 0;
}

const MsgType UCMsg::GetMsgType() const {
	return _msgType;
}

void UCMsg::SetHead(const UCHead& ucHead) {

	_head->__set_version(ucHead.GetVersion());
	_head->__set_appid(ucHead.GetAppId());
	_head->__set_pri(ucHead.GetPri());
	_head->__set_protocoltype(ucHead.GetProtocoltype());
	_head->__set_protocolid(ucHead.GetProtocolId());
	_head->__set_id(ucHead.GetId());
	_head->__set_seq(ucHead.GetSeq());
	_head->__set_timestamp(ucHead.GetTimestamp());
	JID from;
	from.__set_resID(ucHead.GetFrom().GetResId());
	from.__set_siteID(ucHead.GetFrom().GetSiteId());
	from.__set_userID(ucHead.GetFrom().GetUserId());
	_head->__set_from(from);
	JID to;
	to.__set_resID(ucHead.GetTo().GetResId());
	to.__set_siteID(ucHead.GetTo().GetSiteId());
	to.__set_userID(ucHead.GetTo().GetUserId());
	_head->__set_to(to);
	_head->__set_conversation(ucHead.GetConversation());
	_head->__set_length(ucHead.GetLength());
	if (ucHead.GetPrivate().GetResId() != -1) {
		JID toPrivate;
		toPrivate.__set_resID(ucHead.GetPrivate().GetResId());
		toPrivate.__set_siteID(ucHead.GetPrivate().GetSiteId());
		toPrivate.__set_userID(ucHead.GetPrivate().GetUserId());
		_head->__set_toPrivate(toPrivate);
	}
	if (ucHead.GetControlPri() != -1) {
		_head->__set_controlPri(ucHead.GetControlPri());
	}
	if (ucHead.GetChannelPri() != -1) {
		_head->__set_channelPri(ucHead.GetChannelPri());
	}
	std::vector<UCID> ucids = ucHead.GetGroupAtUsers();
	if (!ucids.empty()) {
		vector<JID> atUsers;
		for (const UCID& ucid : ucids) {
			JID atUser;
			atUser.__set_resID(ucid.GetResId());
			atUser.__set_siteID(ucid.GetSiteId());
			atUser.__set_userID(ucid.GetUserId());
			atUsers.push_back(atUser);
		}
		_head->__set_groupAtUsers(atUsers);	
	}
	return;
}

void UCMsg::GetHead(UCHead& ucHead) {
	ucHead.SetVersion(_head->version);
	ucHead.SetAppId(_head->appid);
	ucHead.SetPri(_head->pri);
	ucHead.SetProtocoltype(_head->protocoltype);
	ucHead.SetProtocolId(_head->protocolid);
	ucHead.SetId(_head->id);
	ucHead.SetSeq(_head->seq);
	ucHead.SetTimestamp(_head->timestamp);
	UCID from;
	from.SetResId(_head->from.resID);
	from.SetSiteId(_head->from.siteID);
	from.SetUserId(_head->from.userID);
	ucHead.SetFrom(from);
	UCID to;
	to.SetResId(_head->to.resID);
	to.SetSiteId(_head->to.siteID);
	to.SetUserId(_head->to.userID);
	ucHead.SetTo(to);
	ucHead.SetConversation(_head->conversation);
	ucHead.SetLength(_head->length);
	if (_head->__isset.toPrivate) {
		UCID toPrivate;
		toPrivate.SetResId(_head->toPrivate.resID);
		toPrivate.SetSiteId(_head->toPrivate.siteID);
		toPrivate.SetUserId(_head->toPrivate.userID);
		ucHead.SetPrivate(toPrivate);
	}
	if (_head->__isset.controlPri) {
		ucHead.SetControlPri(_head->controlPri);
	}
	if (_head->__isset.channelPri) {
		ucHead.SetChannelPri(_head->channelPri);
	}
	if (_head->__isset.groupAtUsers) {
		vector<UCID> atUsers;
		for (const JID& jid : _head->groupAtUsers) {
			UCID atUser;
			atUser.SetResId(jid.resID);
			atUser.SetSiteId(jid.siteID);
			atUser.SetUserId(jid.userID);
			atUsers.push_back(atUser);
		}
		ucHead.SetGroupAtUsers(atUsers);
	}
	return;
}

int UCMsg::GetBodyLen() {
	return _head->length;
}

} //namespace uc
