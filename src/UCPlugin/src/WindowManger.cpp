#include "WindowManager.h"

WindowManager::WindowManager(PP_Instance instance, PluginInterface* ppInterface){
	_instance = instance;
	_interface = ppInterface;
}

WindowManager::~WindowManager(){

}

int32_t WindowManager::Create(PluginConfig config){

	if (config._windowType ==  Window_Graphics2D){
		CreateGraphics2D(config._windowRect);
	}

	return 0;

}

int32_t WindowManager::CreateGraphics2D(struct PP_Rect rect){
	
	_graphice_2d = _interface->GetGraphics2dInterface()->Create(_instance, &rect.size, PP_FALSE);
	if (!_graphice_2d){
		return -1;
	}

	if (_interface->GetInstanceInterface()->BindGraphics(_instance, _graphice_2d)){
		_interface->GetCoreInterface()->ReleaseResource(_graphice_2d);
		return -1;
	}
	
	return 0;
}
