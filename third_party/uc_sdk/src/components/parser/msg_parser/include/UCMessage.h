#ifndef UCMESSAGE_H
#define UCMESSAGE_H

#include "UCParserDef.h"
#include <sstream>
namespace uc {

const int MSG_VERSION = 0x0100; //version

/************************************
	UC Message define
************************************/
class UCID {
public:
	UCID()
		:_userId(-1), _siteId(-1), _resId(-1) {
	}

	UCID(int32_t userId, int32_t siteId, int32_t resId) {
		_userId = userId;
		_siteId = siteId;
		_resId = resId;
	}

	UCID& operator=(const UCID &source) {
			_userId = source.GetUserId();
			_siteId = source.GetSiteId();
			_resId = source.GetResId();
			return *this;
		}

	~UCID(){}

	int32_t GetResId() const {
		return _resId;
	}

	void SetResId(int32_t resId) {
		_resId = resId;
	}

	int32_t GetSiteId() const {
		return _siteId;
	}

	void SetSiteId(int32_t siteId) {
		_siteId = siteId;
	}

	int32_t GetUserId() const {
		return _userId;
	}

	void SetUserId(int32_t userId) {
		_userId = userId;
	}
private:
	int32_t _userId;
	int32_t _siteId;
	int32_t _resId;
};

//message head define
class UCHead {
public:
	UCHead()
		:_version(0), _appId(0), _pri(0), 
		_protocoltype(0), _protocolId(0), 
		_id(0), _seq(0),_timestamp(0), _from(), 
		_to(), _conversation(0), _length(0),
		_private(), _controlPri(-1), _channelPri(-1) {
	}

	~UCHead(){}

	int16_t GetAppId() const {
		return _appId;
	}

	void SetAppId(int16_t appId) {
		_appId = appId;
	}

	int32_t GetConversation() const {
		return _conversation;
	}

	void SetConversation(int32_t conversation) {
		_conversation = conversation;
	}

	const UCID& GetFrom() const {
		return _from;
	}

	void SetFrom(const UCID& from) {
		_from = from;
	}

	int32_t GetId() const {
		return _id;
	}

	void SetId(int32_t id) {
		_id = id;
	}

	int32_t GetLength() const {
		return _length;
	}

	void SetLength(int32_t length) {
		_length = length;
	}

	int8_t GetPri() const {
		return _pri;
	}

	void SetPri(int8_t pri) {
		_pri = pri;
	}

	int16_t GetProtocolId() const {
		return _protocolId;
	}

	void SetProtocolId(int16_t protocolId) {
		_protocolId = protocolId;
	}

	int8_t GetProtocoltype() const {
		return _protocoltype;
	}

	void SetProtocoltype(int8_t protocoltype) {
		_protocoltype = protocoltype;
	}

	int64_t GetSeq() const {
		return _seq;
	}

	void SetSeq(int64_t seq) {
		_seq = seq;
	}

	int64_t GetTimestamp() const {
		return _timestamp;
	}

	void SetTimestamp(int64_t timestamp) {
		_timestamp = timestamp;
	}

	const UCID& GetTo() const {
		return _to;
	}

	void SetTo(const UCID& to) {
		_to = to;
	}

	int16_t GetVersion() const {
		return _version;
	}

	void SetVersion(int16_t version) {
		_version = version;
	}

	const UCID& GetPrivate() const {
		return _private; 
	}

	void SetPrivate(const UCID& toPrivate) { 
		_private = toPrivate; 
	}

	int8_t GetControlPri() const { 
		return _controlPri;
	}

	void SetControlPri(int8_t controlPri) {
		_controlPri = controlPri; 
	}

	int16_t GetChannelPri() const { 
		return _channelPri; 
	}

	void SetChannelPri(int16_t channelPri) { 
		_channelPri = channelPri; 
	}

	const std::vector<UCID>& GetGroupAtUsers() const {
		return _groupAtUsers;
	}

	void SetGroupAtUsers(const std::vector<UCID>& _users) {
		_groupAtUsers = _users;
	}

	std::string GetGroupAtUsers_STR() {
		std::stringstream ss("");
		if (_groupAtUsers.empty())
			return ss.str();

		ss << "[";
		for (int i = _groupAtUsers.size() - 1; i >= 0; i--) {
			UCID ucid = _groupAtUsers.at(i);
			ss << "{" << "userId:" << ucid.GetUserId() << "," << "siteId:"
				<< ucid.GetSiteId() << "," << "resId:" << ucid.GetResId() << "}";
			if (i > 0)
				ss << ",";
		}

		ss << "] ";
		return ss.str();
	}

	void SetGroupAtUsers_STR(const std::string& str) {
		std::string strTemp = str;
		std::vector<UCID> users;
		int pos = strTemp.find_first_of('}');
		while (pos > 0 && strTemp.length() >1) {
			std::string obj = strTemp.substr(0, pos + 1);
			pos = obj.find("userId:");
			std::string userId = obj.substr( pos + 7, obj.find(',')-pos-7);
			pos = obj.find("siteId:");
			std::string siteId = obj.substr(pos + 7, obj.find(',', pos+7) - pos-7);
			pos = obj.find("resId:");
			std::string resId = obj.substr( pos + 6, obj.find('}', pos+6) - pos-6);
			UCID ucid;
			int32_t i = 0;
			std::stringstream ss(userId);
			ss >> i;
			ucid.SetUserId(i);
			ss.str("");
			ss.clear();
			ss<<siteId;
			ss >> i;
			ucid.SetSiteId(i);
			ss.str("");
			ss.clear();
			ss<<resId;
			ss >> i;
			ucid.SetResId(i);
			_groupAtUsers.push_back(ucid);
			pos = strTemp.find_first_of('}');
			strTemp = strTemp.substr( pos + 2, strTemp.length() - pos - 2);
			pos = strTemp.find_first_of('}');
		}
	}

private:
	int16_t _version;
	int16_t _appId;
	int8_t  _pri;
	int8_t  _protocoltype;
	int16_t _protocolId;
	int32_t _id;
	int64_t _seq;
	int64_t _timestamp;
	UCID    _from;
	UCID    _to;
	int32_t _conversation;
	int32_t _length;
	UCID _private;
	int8_t _controlPri;
	int16_t _channelPri;
	std::vector<UCID> _groupAtUsers;
};

//message define
class UCMsg {
public:
	UCMsg();
	virtual ~UCMsg();

public:
	int Encode(BinMsg& dest);
	int Decode(const MsgRecord& data);

public:
	const MsgType GetMsgType() const;
	void SetHead(const UCHead& ucHead);
	void GetHead(UCHead& ucHead);
	int GetBodyLen();

private:
	virtual void EncodeBody() = 0;
	virtual void DecodeBody(UcMessageBody *body) = 0;

protected:
	MsgType _msgType;
	UcMessageHead* _head;
	UcMessageBody* _body;
};

}//namespace uc

#endif //UCMESSAGE_H
