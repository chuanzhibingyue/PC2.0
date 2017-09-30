#include "PluginInstance.h"
#include <iostream>
#include <Windows.h>

#include "StringConvert.h"

PluginInstance::PluginInstance(PP_Instance pp_instance, PluginInterface* plugin_interface): 
								_instance(pp_instance), _interface(plugin_interface){
	

	_manager = new WindowManager(pp_instance, plugin_interface);
}

PluginInstance::~PluginInstance(){
	if (_manager){
		delete _manager;
		_manager = NULL;
	}

}

PP_Bool PluginInstance::OnCreated(uint32_t argc, const char* argn[], const char* argv[]){

	//embed标签设定参数;
	for (uint32_t i = 0; i < argc; ++i){

		if (argn[i] == "WindowType"){
			_config._windowType = (WindowType)StringConvert::Convert2Int32(argv[i]);
		}
	}

	_interface->GetInputInterface()->RequestInputEvents(_instance, PP_INPUTEVENT_CLASS_MOUSE);
	_interface->GetInputInterface()->RequestFilteringInputEvents(_instance, PP_INPUTEVENT_CLASS_MOUSE);

	return PP_TRUE;
}

void PluginInstance::OnDestory(){

}

void PluginInstance::OnViewChanged(PP_Resource view){

	//object标签设置区域大小和位置;
	struct PP_Rect rect;
	if (_interface->GetViewInterface()->GetRect(view, &rect) == PP_FALSE){
		return;
	}

	if (_config._windowRect.size.height != rect.size.height ||
		_config._windowRect.size.width != rect.size.width){
		_config._windowRect = rect;
	}

}

void PluginInstance::OnChangeFocus(PP_Bool has_focus){

}

PP_Bool PluginInstance::HandleDocumentLoad(PP_Resource url_loader){
	
	
	return PP_TRUE;
}

PP_Bool PluginInstance::HandleInputEvent(PP_Resource input_event){

	switch (_interface->GetInputInterface()->GetType(input_event)){
	case PP_INPUTEVENT_TYPE_MOUSEDOWN:
		break;
	default:
		break;
	}

	return PP_TRUE;
}

void PluginInstance::HandleMessage(struct PP_Var message){

	if (message.type == PP_VARTYPE_DICTIONARY){
		struct  PP_Var varKey = StringConvert::String2Var(_interface->GetVarInterface(), "command");
		if ("screenShare" == StringConvert::VarToString(_interface->GetVarInterface(), StringConvert::GetDictVar(_interface->GetDictionaryInterface(), message, varKey))){
			
		}
	}

	if (message.type == PP_VARTYPE_STRING){
		if ("screenShare" == StringConvert::VarToString(_interface->GetVarInterface(), message)){
	
		}
	}

}

