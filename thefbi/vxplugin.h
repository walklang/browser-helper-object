/***************************************************************************
 *   Copyright (C) 2005 by Neil Graham   *
 *   lerc@screamingduck.com   *
 *                                                                         *
 ***************************************************************************/
#include "nspluginbase.h"
#include "imagetowindow.h" 
#include "pluginevents.h"
#include "vx32.h"
 
#include "thefbi_plugin_host_sharing.h"

#define MAXIMUM_FILE_SIZE (2 << 26)

#define PLUGIN_VERSION "0.1"
#define PLUGINDATA_NONELOADED 0
#define PLUGINDATA_PARTIALLYLOADED 1
#define PLUGINDATA_FULLYLOADED 2


typedef struct plugininstance_struct {
	NPP InstanceID;
	NPWindow* Window;
	NPObject* WindowObject;
	NPObject* DocumentObject;
	char URL[2048];

// stuff for bringing in a data stream
	int	DataMode;  // one of PLUGINDATA_NONELOADE, PLUGINDATA_PARTIALLYLOADED, PLUGINDATA_FULLYLOADED
	int	DataSize; //0 if unknown
	int	AllocatedDataSize;
	void* Data;

	int vx_pid;
	I2W_OutputHandle Output;
	//void* ProvidedOutputSurface;
	EventHandlerID EventHandler;
	
	
	PluginInfo* Info;
	PluginCommunication* Communication;
	void* ElfFile;
	int Info_Segment_ID;
	
	


#ifdef LINUX_PLUGIN

//	GdkWindow* GDKWindow;
//	GdkGC* GDKContext;
//	GC	XContext;
//	Display* XDisplay;
//	Widget	MotifWidget;
#endif

} PluginInstance;
