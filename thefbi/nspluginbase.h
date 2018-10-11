/***************************************************************************
 *   Copyright (C) 2005 by Neil Graham   *
 *   lerc@screamingduck.com   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Library General Public License as       *
 *   published by the Free Software Foundation; either version 2 of the    *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this program; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifdef WIN32
	// windows version uses a cobbled together npapi header
	#include "npapi.h"
#else
	#include <npapi.h>
	#include <npupp.h>
#endif
#ifdef __cplusplus
extern "C" {
#endif


NPError plugin_GetEntryPoints(NPPluginFuncs* PluginFuncs);
NPError plugin_ReadNetscapeFuncs(NPNetscapeFuncs* BrowserFuncs);

NPMIMEType  plugin_GetMIMEDescription();
NPError plugin_PreInit();
NPError  plugin_Shutdown();
NPError plugin_ConfirmRequiredFunctions(NPNetscapeFuncs* BrowserFuncs);

NPError NPP_New(NPMIMEType pluginType,NPP instance,uint16 mode,
		int16 argc,char* argn[],char* argv[],NPSavedData* saved);
NPError NPP_Destroy(NPP instance, NPSavedData** save);
NPError NPP_GetValue(NPP instance, NPPVariable variable, void *value);
jref NPP_GetJavaClass(void);
NPError NPP_SetWindow(NPP instance, NPWindow* window);
NPError  NPP_NewStream(NPP instance,NPMIMEType type,NPStream *stream, 
	      NPBool seekable,uint16 *stype);
int32 NPP_WriteReady(NPP instance, NPStream *stream);
int32 NPP_Write(NPP instance, NPStream *stream, int32 offset, int32 len, void *buffer);
NPError NPP_DestroyStream(NPP instance, NPStream *stream, NPError reason);
void NPP_StreamAsFile(NPP instance, NPStream *stream, const char* fname);
void NPP_Print(NPP instance, NPPrint* printInfo);
void NPP_URLNotify(NPP instance, const char* url, NPReason reason, void* notifyData);
int16 NPP_HandleEvent(NPP instance, void* event);

extern NPNetscapeFuncs BrowserFunctions;


#ifdef __cplusplus
}  /* end extern "C" */
#endif
