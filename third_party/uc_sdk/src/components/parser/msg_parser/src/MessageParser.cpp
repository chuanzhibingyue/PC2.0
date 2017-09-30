#include "MessageParser.h"
#include "MessageStream.h"
#include "AppMessageType.h"
#include "UCParserErrorCode.h"
#include "LogManager.h"

namespace uc {

int MsgParser::EncodeMessage(UCMsg* msg, BinMsg* dest) {
	if (msg == NULL || dest == NULL) {
		UC_LOG(ERROR) << "EncodeMessage input is NULL!";
		return ParserError::PARSER_INPUT_NULL;
	}
	return msg->Encode(*dest);
}

int MsgParser::EncodeMessage(const UCHead& head, char* body, int bodyLen, BinMsg* dest) {
	UcMessageHead* ucMsgHead = new UcMessageHead();
	ucMsgHead->version = head.GetVersion();
	ucMsgHead->appid = head.GetAppId();
	ucMsgHead->pri = head.GetPri();
	ucMsgHead->protocoltype = head.GetProtocoltype();
	ucMsgHead->protocolid = head.GetProtocolId();
	ucMsgHead->id = head.GetId();
	ucMsgHead->seq = head.GetSeq();
	ucMsgHead->timestamp = head.GetTimestamp();
	ucMsgHead->from.userID = head.GetFrom().GetUserId();
	ucMsgHead->from.siteID = head.GetFrom().GetSiteId();
	ucMsgHead->from.resID = head.GetFrom().GetResId();
	ucMsgHead->to.userID = head.GetTo().GetUserId();
	ucMsgHead->to.siteID = head.GetTo().GetSiteId();
	ucMsgHead->to.resID = head.GetTo().GetResId();
	ucMsgHead->conversation = head.GetConversation();
	ucMsgHead->length = head.GetLength();
	ucMsgHead->toPrivate.userID = head.GetPrivate().GetUserId();
	ucMsgHead->toPrivate.siteID = head.GetPrivate().GetSiteId();
	ucMsgHead->toPrivate.resID = head.GetPrivate().GetResId();
	std::vector<UCID> ucids = head.GetGroupAtUsers();
	if (!ucids.empty()) {
		JID jid;
		vector<JID> jids;
		for (const UCID& ucid : ucids) {
			jid.__set_resID(ucid.GetResId());
			jid.__set_siteID(ucid.GetSiteId());
			jid.__set_userID(ucid.GetUserId());
			jids.push_back(jid);
		}
		ucMsgHead->groupAtUsers = jids;
	}

	ucMsgHead->length = bodyLen;
	int ret = MsgStream::Serialize(ucMsgHead, body, bodyLen, *dest);
	if (ret != 0) {
		UC_LOG(ERROR) << "Serialize return error:" << ret;
	}
	delete ucMsgHead;
	ucMsgHead = NULL;
	return ret;
}

int MsgParser::DecodeMessage(char* msg, int len, UCMsg*& dest) {
	MsgRecord data;
	data.head = new UcMessageHead();
	data.body = new UcMessageBody();
	int ret = MsgStream::Deserialize(msg, len, data);
	if (ret != 0) {
		UC_LOG(ERROR) << "Deserialize return error:" << ret << ", len:" << len;
		return ret;
	}

	dest = GetMsgInstance(data.head);
	if (dest == NULL) {
		UC_LOG(ERROR) << "GetMsgInstance return NULL, appid:" << data.head->appid
			<< ", protocoltype:" << data.head->protocoltype
			<< ", protocolid:" << data.head->protocolid;
		return ParserError::PARSER_CREATE_DECODE_MSG_ERROR;
	}
	return dest->Decode(data);
}

int MsgParser::DecodeBody(const UCHead& head, char* body, int bodyLen, UCMsg*& dest) {
	MsgRecord data;
	data.head = new UcMessageHead();
	data.head->version = head.GetVersion();
	data.head->appid = head.GetAppId();
	data.head->pri = head.GetPri();
	data.head->protocoltype = head.GetProtocoltype();
	data.head->protocolid = head.GetProtocolId();
	data.head->id = head.GetId();
	data.head->seq = head.GetSeq();
	data.head->timestamp = head.GetTimestamp();
	data.head->from.userID = head.GetFrom().GetUserId();
	data.head->from.siteID = head.GetFrom().GetSiteId();
	data.head->from.resID = head.GetFrom().GetResId();
	data.head->to.userID = head.GetTo().GetUserId();
	data.head->to.siteID = head.GetTo().GetSiteId();
	data.head->to.resID = head.GetTo().GetResId();
	data.head->conversation = head.GetConversation();
	data.head->length = head.GetLength();
	data.head->toPrivate.userID = head.GetPrivate().GetUserId();
	data.head->toPrivate.siteID = head.GetPrivate().GetSiteId();
	data.head->toPrivate.resID = head.GetPrivate().GetResId();
	data.head->length = bodyLen;

	data.body = new UcMessageBody();
	int ret = MsgStream::Deserialize(body, bodyLen, data.body);
	if (ret != 0) {
		UC_LOG(ERROR) << "Deserialize return error:" << ret << ", bodyLen:" << bodyLen;
		return ret;
	}

	dest = GetMsgInstance(data.head);
	if (dest == NULL) {
		UC_LOG(ERROR) << "GetMsgInstance return NULL, appid:" << data.head->appid
			<< ", protocoltype:" << data.head->protocoltype
			<< ", protocolid:" << data.head->protocolid;
		return ParserError::PARSER_CREATE_DECODE_MSG_ERROR;
	}
	return dest->Decode(data);
}

UCMsg* MsgParser::GetMsgInstance(const UcMessageHead *head) {
	UCMsg* msg = AppMsgType::Instance().GetMsg(head);
	if (msg == NULL) {
		UC_LOG(ERROR) << "GetMsgInstance return NULL!";
	}
	return msg;
}

} //namespace uc