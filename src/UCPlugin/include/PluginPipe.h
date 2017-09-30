#ifndef PLUGIN_PIPE_H
#define PLUGIN_PIPE_H

#include "Platform.h"
#include <string>

#ifdef PLATFORM_WIN
#include "windows.h"
#endif


class PluginPipe {

public:
	PluginPipe();
	~PluginPipe();

	int32_t start_as_server(const std::string& pipeName);
	int32_t start_as_client();

	int32_t send_message();
	int32_t read_message();

private:

#ifdef  PLATFORM_WIN
	HANDLE _pipeHandle;

#endif //  PLATFORM_WIN



};


#endif  //PLUGIN_PIPE_H;