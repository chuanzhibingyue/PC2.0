#ifndef PLUGIN_INTERFACE_H
#define PLUGIN_INTERFACE_H

#include "ppapi/c/pp_completion_callback.h"
#include "ppapi/c/pp_errors.h"
#include "ppapi/c/pp_module.h"
#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_rect.h"

#include "ppapi/c/ppb.h"
#include "ppapi/c/ppb_instance.h"
#include "ppapi/c/ppb_core.h"
#include "ppapi/c/ppb_var.h"
#include "ppapi/c/ppb_messaging.h"
#include "ppapi/c/ppb_graphics_2d.h"
#include "ppapi/c/ppb_graphics_3d.h"
#include "ppapi/c/ppb_input_event.h"
#include "ppapi/c/ppb_var_dictionary.h"
#include "ppapi/c/ppb_var_array.h"
#include "ppapi/c/ppb_view.h"
#include "ppapi/c/ppb_image_data.h"
#include "ppapi/c/ppb_file_io.h"


#include "ppapi/c/ppp.h"
#include "ppapi/c/ppp_instance.h"
#include "ppapi/c/ppp_messaging.h"
#include "ppapi/c/ppp_input_event.h"



class PluginInterface{

public:
	PluginInterface(){
		ppb_core_interface = NULL;
		ppb_graphics_2d_interface = NULL;
		ppb_graphics_3d_interface = NULL;
		ppb_image_interface = NULL;
		ppb_instance_interface = NULL;
		ppb_input_interface = NULL;
		ppb_messaging_interface = NULL;
		ppb_dictionary_interface = NULL;
		ppb_array_interface = NULL;
		ppb_var_interface = NULL;
		ppb_mouse_interface = NULL;
	}


	PPB_Instance* GetInstanceInterface() const{
		return ppb_instance_interface;
	}
	void SetInstanceInterface(PPB_Instance* ppb_instance){
		ppb_instance_interface = ppb_instance;
	}

	PPB_Core* GetCoreInterface() const{
		return ppb_core_interface;
	}
	void SetCoreInterface(PPB_Core* ppb_core){
		ppb_core_interface = ppb_core;
	}

	PPB_View* GetViewInterface() const{
		return ppb_view_interface;
	}
	void SetViewInterface(PPB_View* ppb_view){
		ppb_view_interface = ppb_view;
	}

	PPB_Graphics2D* GetGraphics2dInterface() const{
		return ppb_graphics_2d_interface;
	}
	void SetGraphics2dInterface(PPB_Graphics2D* ppb_graphics_2d){
		ppb_graphics_2d_interface = ppb_graphics_2d;
	}

	PPB_Graphics3D* GetGraphics3dInterface() const{
		return ppb_graphics_3d_interface;
	}
	void SetGraphics3dInterface(PPB_Graphics3D* ppb_graphics_3d){
		ppb_graphics_3d_interface = ppb_graphics_3d;
	}

	PPB_ImageData* GetImageInterface() const{
		return ppb_image_interface;
	}
	void SetImageInterface(PPB_ImageData* ppb_image){
		ppb_image_interface = ppb_image;
	}

	PPB_InputEvent* GetInputInterface() const{
		return ppb_input_interface;
	}
	void SetInputInterface(PPB_InputEvent* ppb_inputevent){
		ppb_input_interface = ppb_inputevent;
	}

	PPB_MouseInputEvent* GetMouseInterface() const{
		return ppb_mouse_interface;
	}
	void SetMouseInterface(PPB_MouseInputEvent* ppb_mouseinputevent){
		ppb_mouse_interface = ppb_mouseinputevent;
	}

	PPB_Var* GetVarInterface() const{
		return ppb_var_interface;
	}
	void SetVarInterface(PPB_Var* ppb_var){
		ppb_var_interface = ppb_var;
	}

	PPB_Messaging* GetMessageInterface() const{
		return ppb_messaging_interface;
	}
	void SetMessageInterface(PPB_Messaging* ppb_messaging){
		ppb_messaging_interface = ppb_messaging;
	}

	PPB_VarDictionary* GetDictionaryInterface() const{
		return ppb_dictionary_interface;
	}
	void SetDictionaryInterface(PPB_VarDictionary* ppb_dictionary){
		ppb_dictionary_interface = ppb_dictionary;
	}

	PPB_VarArray* GetArrayInterface() const{
		return ppb_array_interface;
	}
	void SetArrayInterface(PPB_VarArray* ppb_array){
		ppb_array_interface = ppb_array;
	}

private:

	PPB_Instance* ppb_instance_interface;
	PPB_Core* ppb_core_interface;
	PPB_View* ppb_view_interface;
	PPB_Graphics2D* ppb_graphics_2d_interface;
	PPB_Graphics3D* ppb_graphics_3d_interface;
	PPB_ImageData* ppb_image_interface;
	PPB_InputEvent *ppb_input_interface;
	PPB_MouseInputEvent *ppb_mouse_interface;
	PPB_Var* ppb_var_interface;
	PPB_Messaging* ppb_messaging_interface;
	PPB_VarDictionary *ppb_dictionary_interface;
	PPB_VarArray *ppb_array_interface;

};

#endif // PLUGIN_INTERFACE_H!