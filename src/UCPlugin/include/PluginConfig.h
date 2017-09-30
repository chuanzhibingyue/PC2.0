#ifndef PLUGIN_CONFIG_H
#define PLUGIN_CONFIG_H

#include "PluginDefs.h"

struct PluginConfig{
	PluginConfig(){
		_windowType = Window_None;
	}

	WindowType _windowType;
	struct PP_Rect _windowRect;
};

#endif  //PLUGIN_CONFIG_H