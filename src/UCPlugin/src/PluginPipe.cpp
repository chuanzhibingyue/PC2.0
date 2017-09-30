#include "PluginPipe.h"
#include "StringConvert.h"

PluginPipe::PluginPipe(){

}

PluginPipe::~PluginPipe(){

}

int32_t PluginPipe::start_as_server(const std::string& pipeName){

	_pipeHandle = CreateNamedPipe(StringConvert::ToUTF8Str(pipeName).c_str(), PIPE_ACCESS_DUPLEX, 
								PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_NOWAIT, 1, 
								1024, 1024, 0, NULL);
	if (INVALID_HANDLE_VALUE == _pipeHandle){
		return -1;
	}
	


	return 0;
}


int32_t PluginPipe::start_as_client(){
	return 0;
}