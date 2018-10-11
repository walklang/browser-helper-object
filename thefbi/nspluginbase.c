
#include "nspluginbase.h"
#include "tinydebug.h"
#include <string.h>
#include <stdio.h>

#ifdef WINDOWS_PLUGIN
#include <windows.h>
#endif

NPNetscapeFuncs BrowserFunctions;

#ifdef WINDOWS_PLUGIN

NPError  WINAPI NP_GetEntryPoints(NPPluginFuncs* PluginFuncs) {
	NPError result;
	debug_FunctionEnter("NP_GetEntryPoints");
	result = plugin_GetEntryPoints(PluginFuncs);
	debug_FunctionLeave("NP_GetEntryPoints");
	return result;
}

NPError WINAPI NP_Initialize(NPNetscapeFuncs* BrowserFuncs) {
	NPError result;
	debug_FunctionEnter("NP_Initialize");
	
	plugin_PreInit();
	result = plugin_ReadNetscapeFuncs(BrowserFuncs);
	debug_FunctionLeave("NP_Initialize");
	return result;
}


NPError  WINAPI NP_Shutdown() {
	NPError result;
	debug_FunctionEnter("NP_ShutDown");
	result = plugin_Shutdown();
	debug_FunctionLeave("NP_ShutDown");
	return result;
}


NPMIMEType WINAPI  NP_GetMIMEDescription(char** description) {
	NPMIMEType result;
	debug_FunctionEnter("NP_GetMIMEDescription");
	result = plugin_GetMIMEDescription();
	debug_FunctionLeave("NP_GetMIMEDescription");
	return result;
}

NPError  WINAPI NP_GetValue(void *future, NPPVariable aVariable, void *aValue){
	NPError result;
	debug_FunctionEnter("NP_GetValue");
	result = NPP_GetValue (NULL,aVariable,aValue);
	debug_FunctionLeave("NP_GetValue");
	return result;
}

#endif

#ifdef LINUX_PLUGIN

NPError NP_GetValue(void *future, NPPVariable aVariable, void *aValue){
	NPError result;
	debug_FunctionEnter("NP_GetValue");
	result = NPP_GetValue (NULL,aVariable,aValue);

	debug_FunctionLeave("NP_GetValue");
	return result;
}

NPError        NP_Initialize(NPNetscapeFuncs* BrowserFuncs , NPPluginFuncs* PluginFuncs) {
	NPError result;
	debug_FunctionEnter("NP_Initialize");

	plugin_PreInit();
	plugin_GetEntryPoints(PluginFuncs);
	result = plugin_ReadNetscapeFuncs(BrowserFuncs);
	debug_FunctionLeave("NP_Initialize");
	return result;
}

NPMIMEType    NP_GetMIMEDescription() {
	NPMIMEType result;
	debug_FunctionEnter("NP_GetMIMEDescription");
	result = plugin_GetMIMEDescription();
	debug_FunctionLeave("NP_GetMIMEDescription");
	return result;
}

#endif

#ifdef MAC_PLUGIN
DEFINE_API_C(NPError) main(NPNetscapeFuncs* BrowserFuncs, 
                           NPPluginFuncs* PluginFuncs,
                           NPP_ShutdownUPP* unloadUpp) {
	NPError result;
	result = plugin_PreInit();
	if (result != NPERR_NO_ERROR) then return result;
	
	result = plugin_GetEntryPoints(PluginFuncs);
	if (result != NPERR_NO_ERROR) then return result;

	result = plugin_ReadNetscapeFuncs(BrowserFuncs);
	if (result != NPERR_NO_ERROR) then return result;

	(*unloadUPP) = plugin_Shutdown

	return result;
}

#endif

NPError plugin_GetEntryPoints(NPPluginFuncs* PluginFuncs) {
	debug_FunctionEnter("plugin_GetEntryPoints");
    	if(PluginFuncs == NULL)   {
			debug_FunctionLeave("plugin_GetEntryPoints !!NPERR_INVALID_FUNCTABLE_ERROR!!");

		   return NPERR_INVALID_FUNCTABLE_ERROR;
	}

	PluginFuncs->version = (NP_VERSION_MAJOR << 8) | NP_VERSION_MINOR;

    PluginFuncs->newp = NPP_New;
    PluginFuncs->destroy = NPP_Destroy;
    PluginFuncs->setwindow = NPP_SetWindow;
    PluginFuncs->newstream = NPP_NewStream;
    PluginFuncs->destroystream = NPP_DestroyStream;
    PluginFuncs->asfile = NPP_StreamAsFile;
    PluginFuncs->writeready = NPP_WriteReady;
    PluginFuncs->write = NPP_Write;
    PluginFuncs->print = NPP_Print;
    PluginFuncs->event = NPP_HandleEvent;
    PluginFuncs->urlnotify = NPP_URLNotify;
    PluginFuncs->javaClass = NULL;  //NPP_GetJavaClass;
    PluginFuncs->getvalue = NPP_GetValue;
    PluginFuncs->setvalue = NULL;
	debug_FunctionLeave("plugin_GetEntryPoints");
	return NPERR_NO_ERROR;
}

NPError plugin_ReadNetscapeFuncs(NPNetscapeFuncs* BrowserFuncs) {
	NPError Result;
	size_t dataSize = sizeof(BrowserFunctions);
	debug_FunctionEnter("plugin_ReadNetscapeFuncs");

	memset(&BrowserFunctions,0,dataSize);

	if (BrowserFuncs == NULL) {
		debug_FunctionLeave("plugin_ReadNetscapeFuncs !!NPERR_INVALID_PARAM!!");
		return NPERR_INVALID_PARAM;
	}

	if( (BrowserFuncs->version >> 8) > NP_VERSION_MAJOR ){
		debug_FunctionLeave("plugin_ReadNetscapeFuncs !!NPERR_INCOMPATIBLE_VERSION_ERROR!!");
		 return NPERR_INCOMPATIBLE_VERSION_ERROR; 
	}

	dataSize = (BrowserFuncs->size < dataSize)?BrowserFuncs->size:dataSize;
	memcpy(&BrowserFunctions,BrowserFuncs,dataSize);
	BrowserFunctions.size = sizeof(BrowserFunctions);


	Result = plugin_ConfirmRequiredFunctions(&BrowserFunctions);

	debug_FunctionLeave("plugin_ReadNetscapeFuncs");

	return Result;
}

/*
NPError plugin_ConfirmRequiredFunctions(NPNetscapeFuncs* BrowserFuncs) {
	NPError error = NPERR_NO_ERROR;

	//error = NPERR_INVALID_FUNCTABLE_ERROR;
	return error;
}
*/

