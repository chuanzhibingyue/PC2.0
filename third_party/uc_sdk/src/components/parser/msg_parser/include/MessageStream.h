#ifndef MESSAGESTREAM_H
#define MESSAGESTREAM_H

#include "UCParserDef.h"

namespace uc {

class MsgStream {
public:

	//serialize message before sending
	static int Serialize(UcMessageHead* head, UcMessageBody* body, BinMsg& dest);

	//serialize message
	static int Serialize(UcMessageHead* head, char* body, int bodyLen, BinMsg& dest);

	//deserialize message after received
	static int Deserialize(char* msg, int len, MsgRecord& dest);

	//deserialize message body
	static int Deserialize(char* msg, int len, UcMessageBody* body);
};

} //namespace uc

#endif //MESSAGESTREAM_H