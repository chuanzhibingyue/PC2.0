#include "MessageStream.h"
#include "UCParserErrorCode.h"
#include "thrift/transport/TBufferTransports.h"
#include "boost/smart_ptr/shared_ptr.hpp"
#include "thrift/protocol/TBinaryProtocol.h"
#include "LogManager.h"

#ifdef WIN32
	#pragma comment(lib,"ws2_32.lib")
#endif

namespace uc {

using boost::shared_ptr;
using namespace apache::thrift::transport;
using namespace apache::thrift::protocol;

//serialize message to binary
int MsgStream::Serialize(UcMessageHead* head, UcMessageBody* body, BinMsg& dest) {
	uint8_t* headBuf = NULL;
	uint32_t headLen = 0;
	uint8_t* bodyBuf = NULL;
	uint32_t bodyLen = 0;

	try {
		//serialize body
		shared_ptr<TMemoryBuffer> bodyMem(new TMemoryBuffer);  
		shared_ptr<TBinaryProtocol> bodyBinProto(new TBinaryProtocol(bodyMem));

		body->write(bodyBinProto.get());
		bodyMem->getBuffer(&bodyBuf, &bodyLen);

		//body size
		head->length = bodyLen;
		
		//serialize head
		shared_ptr<TMemoryBuffer> headMem(new TMemoryBuffer);  
		shared_ptr<TBinaryProtocol> headBinProto(new TBinaryProtocol(headMem));

		head->write(headBinProto.get());
		headMem->getBuffer(&headBuf, &headLen);

		//construct the message
		dest._msgLen = headLen + bodyLen;
		dest._msg = new char[dest._msgLen];
		memcpy(dest._msg, headBuf, headLen);
		memcpy(dest._msg + headLen, bodyBuf, bodyLen);
	} catch (TProtocolException e) {
		delete []dest._msg;
		dest._msg = NULL;
		return ParserError::PARSER_SERIALIZE_ERROR;
	} catch (TTransportException e) {
		delete []dest._msg;
		dest._msg = NULL;
		return ParserError::PARSER_SERIALIZE_ERROR;
	} catch (...) {
		delete []dest._msg;
		dest._msg = NULL;
		return ParserError::PARSER_SERIALIZE_ERROR;
	}

	return ParserError::PARSER_SUCCESS;
}

int MsgStream::Serialize(UcMessageHead* head, char* body, int bodyLen, BinMsg& dest) {
	uint8_t* headBuf = NULL;
	uint32_t headLen = 0;

	try {
		//serialize head
		shared_ptr<TMemoryBuffer> headMem(new TMemoryBuffer);  
		shared_ptr<TBinaryProtocol> headBinProto(new TBinaryProtocol(headMem));

		head->write(headBinProto.get());
		headMem->getBuffer(&headBuf, &headLen);

		//construct the message
		dest._msgLen = headLen + bodyLen;
		dest._msg = new char[dest._msgLen];
		memcpy(dest._msg, headBuf, headLen);
		memcpy(dest._msg + headLen, body, bodyLen);
	} catch (TProtocolException e) {
		delete []dest._msg;
		dest._msg = NULL;
		return ParserError::PARSER_SERIALIZE_ERROR;
	} catch (TTransportException e) {
		delete []dest._msg;
		dest._msg = NULL;
		return ParserError::PARSER_SERIALIZE_ERROR;
	} catch (...) {
		delete []dest._msg;
		dest._msg = NULL;
		return ParserError::PARSER_SERIALIZE_ERROR;
	}

	return ParserError::PARSER_SUCCESS;
}

//deserialize message to uc type
int MsgStream::Deserialize(char* msg, int len, MsgRecord& dest) {
	if (msg == NULL || len <= 0) {
		return ParserError::PARSER_INPUT_NULL;
	}

	uint8_t* buf = (uint8_t *)(msg);
	uint32_t sz = len;
	try {
		//deserialize head 
		shared_ptr<TMemoryBuffer> headMem(new TMemoryBuffer());
		shared_ptr<TProtocol> headProtocol(new TBinaryProtocol(headMem));

		headMem->resetBuffer(buf, sz);
		dest.head->read(headProtocol.get());

		//deserialize body
		shared_ptr<TMemoryBuffer> bodyMem(new TMemoryBuffer());
		shared_ptr<TProtocol> bodyProtocol(new TBinaryProtocol(bodyMem));

		bodyMem->resetBuffer(buf+(sz-dest.head->length), dest.head->length);
		dest.body->read(bodyProtocol.get());
	}
	catch (std::exception &e) {
		UC_LOG(ERROR) << "Deserialize exception: " << e.what() << ", check head info, "
			<< "appid: " << dest.head->appid << ", protocoltype: " << dest.head->protocoltype 
			<< ", protocolid: " << dest.head->protocolid << ", fromUserID: " << dest.head->from.userID 
			<< ", seq: " << dest.head->seq << ", timestamp: " << dest.head->timestamp;
		return ParserError::PARSER_DESERIALIZE_ERROR;
	} 
	return ParserError::PARSER_SUCCESS;
}

//deserialize message to UcMessageBody
int MsgStream::Deserialize(char* msg, int len, UcMessageBody* body) {
	if (msg == NULL || len <= 0) {
		return ParserError::PARSER_INPUT_NULL;
	}

	uint8_t* buf = (uint8_t *)(msg);
	try {
		//deserialize body
		shared_ptr<TMemoryBuffer> bodyMem(new TMemoryBuffer());
		shared_ptr<TProtocol> bodyProtocol(new TBinaryProtocol(bodyMem));

		bodyMem->resetBuffer(buf, len);
		body->read(bodyProtocol.get());
	} catch (TProtocolException e) {
		return ParserError::PARSER_DESERIALIZE_ERROR;
	} catch (TTransportException e) {
		return ParserError::PARSER_DESERIALIZE_ERROR;
	} catch (...) {
		return ParserError::PARSER_DESERIALIZE_ERROR;
	}
	return ParserError::PARSER_SUCCESS;
}

} //namespace uc