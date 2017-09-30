#include "PluginInterface.h"
#include "PluginInstance.h"
#include "StringConvert.h"
#include <Windows.h>

typedef struct PluginInstanceInfo {
	PluginInstance *plugin_instance;
	PluginInstanceInfo* next;
}PluginInstanceInfo;

PluginInstanceInfo *plugin_instances = NULL;
PluginInterface plugin_interfaces;


PluginInstanceInfo* FindInstance(PP_Instance instance){
	PluginInstanceInfo* cur = plugin_instances;
	while (cur){
		if (cur->plugin_instance->GetPluginInstance() != instance){
			cur = cur->next;
		}
		return cur;
	}
	return NULL;
}

/* PPP_Instance implementation -----------------------------------------------*/
PP_Bool Instance_DidCreate(PP_Instance instance, uint32_t argc, const char* argn[], const char* argv[]){

	PluginInstanceInfo* info = (PluginInstanceInfo*)malloc(sizeof(PluginInstanceInfo));
	info->plugin_instance = new PluginInstance(instance, &plugin_interfaces);
	info->next = plugin_instances;
	plugin_instances = info;

	return info->plugin_instance->OnCreated(argc, argn, argv);
}

void Instance_DidDestroy(PP_Instance instance){

	PluginInstanceInfo** prev_ptr = &plugin_instances;
	PluginInstanceInfo* cur = plugin_instances;
	while (cur) {
		if (instance == cur->plugin_instance->GetPluginInstance()) {
			*prev_ptr = cur->next;
			cur->plugin_instance->OnDestory();
			delete cur->plugin_instance;
			free(cur);
			break;
		}
		prev_ptr = &cur->next;
		cur = cur->next;
	}
}


void Instance_DidChangeView(PP_Instance instance, PP_Resource view){
	PluginInstanceInfo* info = FindInstance(instance);
	if (info){
		info->plugin_instance->OnViewChanged(view);
	}
}

void Instance_DidChangeFocus(PP_Instance instance, PP_Bool has_focus){
	PluginInstanceInfo* info = FindInstance(instance);
	if (info){
		info->plugin_instance->OnChangeFocus(has_focus);
	}
}

PP_Bool Instance_HandleDocumentLoad(PP_Instance instance, PP_Resource url_loader){
	PluginInstanceInfo* info = FindInstance(instance);
	if (info){
		info->plugin_instance->HandleDocumentLoad(url_loader);
	}
	return PP_TRUE;
}


PP_Bool InputEvent_HandleInputEvent(PP_Instance instance, PP_Resource input_event){
	
	PluginInstanceInfo* info = FindInstance(instance);
	if (info){
		info->plugin_instance->HandleDocumentLoad(input_event);
	}
	return PP_TRUE;
}

void Plugin_HandleMessage(PP_Instance instance, struct PP_Var message)
{
	PluginInstanceInfo *info = FindInstance(instance);
	if (info){
		info->plugin_instance->HandleMessage(message);
	}
	
}

/* Global entrypoints --------------------------------------------------------*/

static PPP_Instance instance_interface = {
	&Instance_DidCreate,
	&Instance_DidDestroy,
	&Instance_DidChangeView,
	&Instance_DidChangeFocus,
	&Instance_HandleDocumentLoad
};


static PPP_InputEvent input_interface = {
	&InputEvent_HandleInputEvent
};

static PPP_Messaging message_interface = {
	&Plugin_HandleMessage
};


PP_EXPORT int32_t PPP_InitializeModule(PP_Module module, PPB_GetInterface get_browser_interface){
	OutputDebugString(L"PPP_InitializeModule 1");
	plugin_interfaces.SetCoreInterface((PPB_Core*)get_browser_interface(PPB_CORE_INTERFACE));
	plugin_interfaces.SetInstanceInterface((PPB_Instance*)get_browser_interface(PPB_INSTANCE_INTERFACE));
	plugin_interfaces.SetImageInterface((PPB_ImageData*)get_browser_interface(PPB_IMAGEDATA_INTERFACE));
	plugin_interfaces.SetGraphics2dInterface((PPB_Graphics2D*)get_browser_interface(PPB_GRAPHICS_2D_INTERFACE));
	plugin_interfaces.SetGraphics3dInterface((PPB_Graphics3D*)get_browser_interface(PPB_GRAPHICS_3D_INTERFACE));
	plugin_interfaces.SetViewInterface((PPB_View*)get_browser_interface(PPB_VIEW_INTERFACE));
	plugin_interfaces.SetInputInterface((PPB_InputEvent*)get_browser_interface(PPB_INPUT_EVENT_INTERFACE));
	plugin_interfaces.SetMouseInterface((PPB_MouseInputEvent*)get_browser_interface(PPB_MOUSE_INPUT_EVENT_INTERFACE));
	plugin_interfaces.SetVarInterface((PPB_Var*)get_browser_interface(PPB_VAR_INTERFACE));
	plugin_interfaces.SetMessageInterface((PPB_Messaging*)get_browser_interface(PPB_MESSAGING_INTERFACE));
	plugin_interfaces.SetDictionaryInterface((PPB_VarDictionary*)get_browser_interface(PPB_VAR_DICTIONARY_INTERFACE));
	plugin_interfaces.SetArrayInterface((PPB_VarArray*)get_browser_interface(PPB_VAR_ARRAY_INTERFACE));
	OutputDebugString(L"PPP_InitializeModule 2");
	if (plugin_interfaces.GetCoreInterface() == NULL || !plugin_interfaces.GetInstanceInterface() || !plugin_interfaces.GetImageInterface() ||
		!plugin_interfaces.GetGraphics2dInterface() || !plugin_interfaces.GetViewInterface() || !plugin_interfaces.GetVarInterface() ||
		!plugin_interfaces.GetInputInterface() || !plugin_interfaces.GetMouseInterface() || !plugin_interfaces.GetDictionaryInterface() ||
		!plugin_interfaces.GetMessageInterface() || !plugin_interfaces.GetArrayInterface()){ 

		return -1;
	}

	return PP_OK;
}

PP_EXPORT void PPP_ShutdownModule(){

}

PP_EXPORT const void* PPP_GetInterface(const char* interface_name){

	if (strcmp(interface_name, PPP_INSTANCE_INTERFACE) == 0){
		return &instance_interface;
	}

	if (strcmp(interface_name, PPP_MESSAGING_INTERFACE) == 0){
		return &message_interface;
	}

	if (strcmp(interface_name, PPP_INPUT_EVENT_INTERFACE) == 0){
		return &input_interface;
	}

	return NULL;
}