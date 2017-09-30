#ifndef MESSAGEPARSER_H
#define MESSAGEPARSER_H

#include "UCParserDef.h"
#include "UCMessage.h"

namespace uc {

class MsgParser {
public:
	static int EncodeMessage(UCMsg* msg, BinMsg* dest);
	static int EncodeMessage(const UCHead& head, char* body, int bodyLen, BinMsg* dest);
	static int DecodeMessage(char* msg, int len, UCMsg*& dest);
	static int DecodeBody(const UCHead& head, char* body, int bodyLen, UCMsg*& dest);

private:
	static UCMsg* GetMsgInstance(const UcMessageHead *head);
};

} //namespace uc

#endif //MESSAGEPARSER_H