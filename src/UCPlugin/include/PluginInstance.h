#ifndef PLUGIN_INSTANCE_H
#define PLUGIN_INSTANCE_H

#include "PluginInterface.h"
#include "PluginConfig.h"
#include "WindowManager.h"

class PluginInstance{

public:
	PluginInstance(PP_Instance pp_instance, PluginInterface* plugin_interface);
	~PluginInstance();

	//plugin instance interface event
	virtual PP_Bool OnCreated(uint32_t argc, const char* argn[], const char* argv[]);
	virtual void OnDestory();
	virtual void OnViewChanged(PP_Resource view);
	virtual void OnChangeFocus(PP_Bool has_focus);
	virtual PP_Bool HandleDocumentLoad(PP_Resource url_loader);
	
	//plugin input interface event
	virtual PP_Bool HandleInputEvent(PP_Resource input_event);

	//plugin message interface event
	virtual void HandleMessage(struct PP_Var message);

public:
	PP_Instance GetPluginInstance(){ return _instance; }
	PluginInterface* GetPluginInterface(){ return _interface; }

private:
	PP_Instance _instance;       //plugin instance, when html create a object/embed tag will create a instance
	PluginInterface* _interface; 

	PluginConfig _config;

	WindowManager* _manager;

};

#endif  //PLUGIN_INSTANCE_H