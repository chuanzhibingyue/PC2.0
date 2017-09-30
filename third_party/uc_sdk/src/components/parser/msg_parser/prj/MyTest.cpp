// test2.cpp : Defines the entry point for the console application.
//

#include <string>
#include <iostream>
#include <list>
#include <map>
#include "MessageParser.h"
#include "UCMessageType.h"

using namespace std;
using namespace uc;

/////////////////////////test//////////////////////////
typedef int (*resp_func)(UCMsg* msg);
class MsgResp
{
public:
	static int presence_resp(UCMsg* msg)
	{
		UCHead head;
		msg->GetHead(head);

		cout << "head:" << endl;
		cout << "version:" << head.GetVersion() << endl;
		cout << "appid:" << head.GetAppId() << endl;
		cout << "pri:" << (int)head.GetPri() << endl;
		cout << "protocoltype:" << (int)head.GetProtocoltype() << endl;
		cout << "protocolid:" << head.GetProtocolId() << endl;
		cout << "id:" << head.GetId() << endl;
		cout << "seq:" << head.GetSeq() << endl;
		cout << "timestamp:" << head.GetTimestamp() << endl;
		cout << "conversation:" << head.GetConversation() << endl;
		cout << "length:" << head.GetLength() << endl;
		cout << "from:" << endl;
		cout << "resId:" << head.GetFrom().GetResId() << endl;
		cout << "siteId:" << head.GetFrom().GetSiteId() << endl;
		cout << "userId:" << head.GetFrom().GetUserId() << endl;
		cout << "to:" << endl;
		cout << "resId:" << head.GetFrom().GetResId() << endl;
		cout << "siteId:" << head.GetFrom().GetSiteId() << endl;
		cout << "userId:" << head.GetFrom().GetUserId() << endl;

		MessagePresenceMsg* presence = dynamic_cast<MessagePresenceMsg*>(msg);
		int pri = presence->get_pri();
		string show = presence->get_show();
		int status = presence->get_status();
		int type = presence->get_deviceType();

		cout << "body:" << endl;
		cout << "pri:" << pri << endl;
		cout << "show:" << show << endl;
		cout << "status:" << status << endl;
		cout << "type:" << type << endl;

		assert(pri == 0);
		assert(show == "online");
		assert(status == PresenceType::Busy);
		assert(type == DeviceType::PC);

		return 0;
	}

	static int ping_resp(UCMsg* msg)
	{
		//head
		UCHead head;
		msg->GetHead(head);

		cout << "head:" << endl;
		cout << "version:" << head.GetVersion() << endl;
		cout << "appid:" << head.GetAppId() << endl;
		cout << "pri:" << (int)head.GetPri() << endl;
		cout << "protocoltype:" << (int)head.GetProtocoltype() << endl;
		cout << "protocolid:" << head.GetProtocolId() << endl;
		cout << "id:" << head.GetId() << endl;
		cout << "seq:" << head.GetSeq() << endl;
		cout << "timestamp:" << head.GetTimestamp() << endl;
		cout << "conversation:" << head.GetConversation() << endl;
		cout << "length:" << head.GetLength() << endl;
		cout << "from:" << endl;
		cout << "resId:" << head.GetFrom().GetResId() << endl;
		cout << "siteId:" << head.GetFrom().GetSiteId() << endl;
		cout << "userId:" << head.GetFrom().GetUserId() << endl;
		cout << "to:" << endl;
		cout << "resId:" << head.GetFrom().GetResId() << endl;
		cout << "siteId:" << head.GetFrom().GetSiteId() << endl;
		cout << "userId:" << head.GetFrom().GetUserId() << endl;

		//body
		MessagePingMsg* presence = dynamic_cast<MessagePingMsg*>(msg);
		cout << "body:" << endl;
		cout << "interval:" << presence->get_interval() << endl;
		cout << "devicetype:" << presence->get_deviceType() << endl;
		assert(presence->get_interval() == 10);
		assert(presence->get_deviceType() == DeviceType::PC);

		return 0;
	}
};

//////////////////////////////////////////////////////////////////
int test_presence()
{
	UCHead head;
	head.SetAppId(AppId::AppPresence);
	head.SetPri(1);
	head.SetProtocoltype(PresenceMessageType::DefaultType);
	head.SetProtocolId(1);
	head.SetId(0);
	head.SetSeq(1);
	UCID id;
	id.SetResId(21);
	id.SetSiteId(2);
	id.SetUserId(1);
	head.SetFrom(id);
	head.SetTo(id);
	head.SetConversation(12);
	head.SetLength(4);

	MessagePresenceMsg* msg = new MessagePresenceMsg();
	//set head
	msg->SetHead(&head);
	//set body
	msg->set_pri(0);
	msg->set_show("online");
	msg->set_status(PresenceType::Busy);
	msg->set_deviceType(DeviceType::PC);
	BinMsg dest;
	int ret = MsgParser::EncodeMessage(msg, &dest);
	assert(ret == 0);

	/////////////////decode///////////////////
	//decode
	UCMsg* resp = NULL;
	ret = MsgParser::DecodeMessage(dest._msg, dest._msgLen, resp);
	assert(ret == 0);
	if (resp != NULL)
	{
		MsgType type = resp->GetMsgType();
		ret = MsgResp::presence_resp(resp);
	}

	//////////////////decode body//////////////////
	UCMsg* bodyMsg = NULL;
	int bodyLen = resp->GetBodyLen();
	char* body = new char[bodyLen];
	memset(body, 0, bodyLen);
	memcpy(body, dest._msg + (dest._msgLen - bodyLen), bodyLen);
	ret = MsgParser::DecodeBody(head, body, bodyLen, bodyMsg);
	assert(ret == 0);
	ret = MsgResp::presence_resp(bodyMsg);
	assert(ret == 0);

	delete msg;
	msg = NULL;
	delete resp;
	resp = NULL;
	delete bodyMsg;
	bodyMsg = NULL;
	delete []body;
	body = NULL;


	return 0;
}

////////////////////////////////////////////////
int test_ping()
{
	//head
	UCHead head;
	head.SetAppId(AppId::AppHeartbeat);
	head.SetPri(1);
	head.SetProtocoltype(1);
	head.SetProtocolId(1);
	head.SetId(0);
	head.SetSeq(1);
	UCID id;
	id.SetResId(21);
	id.SetSiteId(2);
	id.SetUserId(1);
	head.SetFrom(id);
	head.SetTo(id);
	head.SetConversation(12);
	head.SetLength(4);

	MessagePingMsg* msg = new MessagePingMsg();
	//set head
	msg->SetHead(&head);
	//set body
	msg->set_deviceType(DeviceType::PC);
	msg->set_interval(10);
	BinMsg dest;
	int ret = MsgParser::EncodeMessage(msg, &dest);
	assert(ret == 0);

	/////////////////decode///////////////////
	//decode
	UCMsg* resp = NULL;
	MsgParser::DecodeMessage(dest._msg, dest._msgLen, resp);
	if (resp != NULL)
	{
		MsgType type = resp->GetMsgType();
		MsgResp::ping_resp(resp);
	}

	delete msg;
	msg = NULL;
	delete resp;
	resp = NULL;

	return ret;
}


//////////////////////////////////////////////
int test_ack_sent()
{

	return 0;
}

////////////////////////////////////////////////
int test_null()
{
	//head
// 	UCHead head;
// 	head.SetAppId(AppId::AppNotify);
// 	head.SetPri(1);
// 	head.SetProtocoltype(SystemProtoMessageType::SensitiveWordDelete);
// 	head.SetProtocolId(1);
// 	head.SetId(0);
// 	head.SetSeq(1);
// 	UCID id;
// 	id.SetResId(21);
// 	id.SetSiteId(2);
// 	id.SetUserId(1);
// 	head.SetFrom(id);
// 	head.SetTo(id);
// 	head.SetConversation(12);
// 	head.SetLength(0);
// 
// 	NullMsg* msg = new NullMsg();
// 	//set head
// 	msg->set_head(&head);
// 	BinMsg dest;
// 	int ret = MsgParser::EncodeMessage(msg, &dest);
// 
// 	/////////////////decode///////////////////
// 	//decode
// 	UCMsg* resp = NULL;
// 	MsgParser::DecodeMessage(dest._msg, dest._msgLen, resp);
// 	if (resp != NULL)
// 	{
// 		MsgType type = resp->get_msgType();
// 		ret = resp_tab[type](resp);
// 	}

	return 0;
}

int test_presence_body() {
	//head
	UCHead head;
	head.SetAppId(AppId::AppPresence);
	head.SetPri(1);
	head.SetProtocoltype(PresenceMessageType::DefaultType);
	head.SetProtocolId(1);
	head.SetId(0);
	head.SetSeq(1);
	UCID id;
	id.SetResId(21);
	id.SetSiteId(2);
	id.SetUserId(1);
	head.SetFrom(id);
	head.SetTo(id);
	head.SetConversation(12);
	head.SetLength(4);
	 
	MessagePresenceMsg* msg = new MessagePresenceMsg();
	msg->SetHead(&head);
	msg->set_pri(0);
	msg->set_show("online");
	msg->set_status(PresenceType::Busy);
	msg->set_deviceType(DeviceType::PC);
	BinMsg dest;
	int ret = MsgParser::EncodeMessage(msg, &dest);
	assert(ret == 0);

	char* body = new char[msg->GetBodyLen()];
	memset(body, 0, msg->GetBodyLen());
	memcpy(body, dest._msg + (dest._msgLen - msg->GetBodyLen()), msg->GetBodyLen());
	UCMsg* result = NULL;
	ret = MsgParser::DecodeBody(head, body, msg->GetBodyLen(), result);
	assert(ret == 0);

	MessagePresenceMsg* presence = dynamic_cast<MessagePresenceMsg*>(result);
	int pri = presence->get_pri();
	string show = presence->get_show();
	int status = presence->get_status();
	int type = presence->get_deviceType();
	cout << "body:" << endl;
	cout << "pri:" << pri << endl;
	cout << "show:" << show << endl;
	cout << "status:" << status << endl;
	cout << "type:" << type << endl;


	delete msg;
	msg = NULL;
	delete [] body;
	body = NULL;
	delete presence;
	presence = NULL;

	return 0;
}


int main()
{
 	int ret = 0;
	ret = test_presence();
	assert(ret == 0);

	ret = test_ping();
	assert(ret == 0);

	//ret = test_null();
	//assert(ret == 0);

	ret = test_presence_body();

	return 0;
}

