#ifndef WINDOW_MANAGER_H
#define WINDOW_MANAGER_H

#include "Platform.h"
#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_resource.h"

#include "ppapi/c/ppb.h"
#include "ppapi/c/ppb_graphics_2d.h"

#include "PluginConfig.h"
#include "PluginInterface.h"

#ifdef PLATFORM_WIN
#include "Windows.h"
#endif

class WindowManager{

public:
	WindowManager(PP_Instance instance, PluginInterface* ppInterface);
	~WindowManager();


	int32_t Create(PluginConfig config);


private:
	int32_t CreateGraphics2D(struct PP_Rect rect);

private:
	PP_Instance _instance;
	PluginInterface* _interface;
	PP_Resource _graphice_2d;
	PP_Resource _graphice_3d;

#ifdef PLATFORM_WIN
	HWND _window_hand;
#endif

};


#endif //WINDOW_MANAGER_H
