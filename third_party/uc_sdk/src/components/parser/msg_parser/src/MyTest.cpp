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
		msg->get_head(head);

		cout << "head:" << endl;
		cout << "version:" << head.get_version() << endl;
		cout << "appid:" << head.get_appId() << endl;
		cout << "pri:" << (int)head.get_pri() << endl;
		cout << "protocoltype:" << (int)head.get_protocolType() << endl;
		cout << "protocolid:" << head.get_protocolId() << endl;
		cout << "id:" << head.get_id() << endl;
		cout << "seq:" << head.get_seq() << endl;
		cout << "timestamp:" << head.get_timeStamp() << endl;
		cout << "conversation:" << head.get_conversation() << endl;
		cout << "length:" << head.get_length() << endl;
		cout << "from:" << endl;
		cout << "resId:" << head.get_from().get_resId() << endl;
		cout << "siteId:" << head.get_from().get_siteId() << endl;
		cout << "userId:" << head.get_from().get_userId() << endl;
		cout << "to:" << endl;
		cout << "resId:" << head.get_from().get_resId() << endl;
		cout << "siteId:" << head.get_from().get_siteId() << endl;
		cout << "userId:" << head.get_from().get_userId() << endl;

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
		msg->get_head(head);

		cout << "head:" << endl;
		cout << "version:" << head.get_version() << endl;
		cout << "appid:" << head.get_appId() << endl;
		cout << "pri:" << (int)head.get_pri() << endl;
		cout << "protocoltype:" << (int)head.get_protocolType() << endl;
		cout << "protocolid:" << head.get_protocolId() << endl;
		cout << "id:" << head.get_id() << endl;
		cout << "seq:" << head.get_seq() << endl;
		cout << "timestamp:" << head.get_timeStamp() << endl;
		cout << "conversation:" << head.get_conversation() << endl;
		cout << "length:" << head.get_length() << endl;
		cout << "from:" << endl;
		cout << "resId:" << head.get_from().get_resId() << endl;
		cout << "siteId:" << head.get_from().get_siteId() << endl;
		cout << "userId:" << head.get_from().get_userId() << endl;
		cout << "to:" << endl;
		cout << "resId:" << head.get_from().get_resId() << endl;
		cout << "siteId:" << head.get_from().get_siteId() << endl;
		cout << "userId:" << head.get_from().get_userId() << endl;

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

static resp_func resp_tab[] = {MsgResp::presence_resp, MsgResp::ping_resp};
//////////////////////////////////////////////////////////////////
int test_presence()
{
	//head
	UCHead head;
	head.set_appId(AppId::AppPresence);
	head.set_pri(1);
	head.set_protocolType(PresenceMessageType::DefaultType);
	head.set_protocolId(1);
	head.set_id(0);
	head.set_seq(1);
	UCID id;
	id.set_resId(21);
	id.set_siteId(2);
	id.set_userId(1);
	head.set_from(id);
	head.set_to(id);
	head.set_conversation(12);
	head.set_length(4);

	MessagePresenceMsg* msg = new MessagePresenceMsg();
	//set head
	msg->set_head(&head);
	//set body
	msg->set_pri(0);
	msg->set_show("online");
	msg->set_status(PresenceType::Busy);
	msg->set_deviceType(DeviceType::PC);
	BinMsg dest;
	int ret = MsgParser::EncodeMessage(msg, &dest);

	/////////////////decode///////////////////
	//decode
	UCMsg* resp = NULL;
	MsgParser::DecodeMessage(&dest, resp);
	if (resp != NULL)
	{
		MsgType type = resp->get_msgType();
		ret = resp_tab[type](resp);
	}
	
	delete msg;
	msg = NULL;
	delete resp;
	resp = NULL;

	return 0;
}

////////////////////////////////////////////////
int test_ping()
{
	//head
	UCHead head;
	head.set_appId(AppId::AppHeartbeat);
	head.set_pri(1);
	head.set_protocolType(1);
	head.set_protocolId(1);
	head.set_id(0);
	head.set_seq(1);
	UCID id;
	id.set_resId(21);
	id.set_siteId(2);
	id.set_userId(1);
	head.set_from(id);
	head.set_to(id);
	head.set_conversation(12);
	head.set_length(4);

	MessagePingMsg* msg = new MessagePingMsg();
	//set head
	msg->set_head(&head);
	//set body
	msg->set_deviceType(DeviceType::PC);
	msg->set_interval(10);
	BinMsg dest;
	int ret = MsgParser::EncodeMessage(msg, &dest);

	/////////////////decode///////////////////
	//decode
	UCMsg* resp = NULL;
	MsgParser::DecodeMessage(&dest, resp);
	if (resp != NULL)
	{
		MsgType type = resp->get_msgType();
		ret = resp_tab[type](resp);
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
	UCHead head;
	head.set_appId(AppId::AppNotify);
	head.set_pri(1);
	head.set_protocolType(SystemProtoMessageType::SensitiveWordDelete);
	head.set_protocolId(1);
	head.set_id(0);
	head.set_seq(1);
	UCID id;
	id.set_resId(21);
	id.set_siteId(2);
	id.set_userId(1);
	head.set_from(id);
	head.set_to(id);
	head.set_conversation(12);
	head.set_length(0);

	NullMsg* msg = new NullMsg();
	//set head
	msg->set_head(&head);
	BinMsg dest;
	int ret = MsgParser::EncodeMessage(msg, &dest);

	/////////////////decode///////////////////
	//decode
	UCMsg* resp = NULL;
	MsgParser::DecodeMessage(&dest, resp);
	if (resp != NULL)
	{
		MsgType type = resp->get_msgType();
		ret = resp_tab[type](resp);
	}

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

	return 0;
}

