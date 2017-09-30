#ifndef UCPARSERERRORCODE_H
#define UCPARSERERRORCODE_H

namespace uc {

//error code from 10000~15000
class ParserError {
public:
	enum {
		PARSER_SUCCESS					= 0,
		PARSER_INPUT_NULL				= 10000,
		PARSER_CREATE_DECODE_MSG_ERROR	= 10001,
		PARSER_SERIALIZE_ERROR			= 10002,
		PARSER_DESERIALIZE_ERROR		= 10003,
	};
};

} //namespace uc

#endif //UCPARSERERRORCODE_H