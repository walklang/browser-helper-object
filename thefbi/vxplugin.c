/***************************************************************************
 *   Copyright (C) 2005 by Neil Graham   *
 *   lerc@screamingduck.com   *
 *                                                                         *
 ****************simple***********************************************************/

#include "tinydebug.h"

#include <string.h>
#include <stdlib.h>
#include "vxplugin.h"

#include "lzma/LzmaDec.h"
#include "lzma/Types.h"

#ifdef LINUX_PLUGIN

#include <unistd.h>
#include <signal.h>
#include <sched.h>
#include <sys/ipc.h>
#include <sys/shm.h>

//not liking this solution
#define __USE_GNU
#include <dlfcn.h>

#endif

 NPMIMEType plugin_GetMIMEDescription() {
	debug_FunctionEnter("plugin_GetMIMEDescription");
	
    char *result;
    char MimeTypes[4000];
	memset(MimeTypes,0,sizeof(MimeTypes));
    strcat(MimeTypes, "application/x-thefbi:tfb:Thefbi media;");

    result = strdup(MimeTypes);
    debugf("     I said '%s'\n",result);
    debug_FunctionLeave("plugin_GetMIMEDescription");
    return result;
}

void InformStateUpdate(PluginInstance* InstanceData, int StateFlags) {
	int res;
	int semaphorevalue;
	InstanceData->Info->StateChangeFlags_accumulator |= StateFlags; 
	//debug_Line("semaphore check");
	res = sem_getvalue(&(InstanceData->Communication->InputStateUpdated),&semaphorevalue);
	if (res) {
		debug_Line("sem_getvalue InputStateUpdated failed");
		return;
	}
	if (semaphorevalue <=0) {  //someone is looking to see
		//debug_Line("semaphore post");
		InstanceData->Info->StateChangeFlags = InstanceData->Info->StateChangeFlags_accumulator;
		InstanceData->Info->StateChangeFlags_accumulator = 0;	
	
		sem_post(&(InstanceData->Communication->InputStateUpdated));
	}
}

EventResult plugin_EventHandler (void* Instance,tEventKind Kind,pPluginTranslatedEventData EventData) {
		//debug_FunctionEnter("plugin_EventHandler");
		PluginInstance* InstanceData = (PluginInstance*) Instance;
		switch(Kind) {
			case ekExpose:
				debug_Line("Expose");
				I2W_Draw(InstanceData->Output);
				debug_Line("Frame rendered");
				break;
			case ekTimer:
				if (InstanceData->Communication->RedrawRequired) {
					InstanceData->Communication->RedrawRequired = false;
					debug_Line("RedrawRequested");
					I2W_Draw(InstanceData->Output);
				}
				InstanceData->Info->Age++;
				InstanceData->Info->ActiveAge++;
				InstanceData->Info->VisibleAge++;
				InformStateUpdate(InstanceData,PLUGIN_STATE_FLAG_AGE | PLUGIN_STATE_FLAG_ACTIVEAGE | PLUGIN_STATE_FLAG_VISIBLEAGE) ;
				break;
			case ekMouseMove:
				InstanceData->Info->MouseX = EventData->MouseMoveEvent.ElementX;	
				InstanceData->Info->MouseY = EventData->MouseMoveEvent.ElementY;	
				InformStateUpdate(InstanceData,PLUGIN_STATE_FLAG_MOUSEPOSITION);
				break;
				
			case ekScriptRequest:
			{
			   NPVariant ScriptResult;
			   NPString Script;
			   Script.utf8characters = InstanceData->Communication->Buffer;
			   Script.utf8length = strlen(InstanceData->Communication->Buffer);
			   debug_Line("Handling Script event")
			   debug_Line(InstanceData->Communication->Buffer);
			   BrowserFunctions.evaluate(InstanceData->InstanceID,InstanceData->DocumentObject,&Script,&ScriptResult);
			   debug_Line("Done Evaluate")
			   BrowserFunctions.releasevariantvalue(&ScriptResult);
   			   debug_Line("Freed Variant")

			   break;	
		    } 
			default:
				;
		}

		//debug_FunctionLeave("plugin_EventHandler");
		return 1;
}

NPError plugin_PreInit(){
	debug_FunctionLeave("plugin_PreInit");
	return NPERR_NO_ERROR;
}

NPError plugin_Shutdown() {
	debug_FunctionEnter("plugin_Shutdown");

	debug_FunctionLeave("plugin_PreInit");

	return NPERR_NO_ERROR;
}

NPError plugin_ConfirmRequiredFunctions(NPNetscapeFuncs* BrowserFuncs) {
	debug_FunctionEnter("plugin_ConfirmRequiredFunctions");

	NPError error = NPERR_NO_ERROR;

	//error = NPERR_INVALID_FUNCTABLE_ERROR;

	debug_FunctionLeave("plugin_ConfirmRequiredFunctions");
	return error;
}

NPError NPP_GetValue(NPP instance, NPPVariable variable, void *value){
	debug_FunctionEnter("plugin_GetValue");

	NPError result =  NPERR_NO_ERROR;
    // some sites use this description to figure out what formats can be played. So we have to make sure the 
    // description matches the features

    if (variable == NPPVpluginNameString) {
		debug_String("       PluginName :");
	*((const char **) value) = "Thefbi " PLUGIN_VERSION;

       debugf("I said '%s'\n",*((const char **) value));

    }

    if (variable == NPPVpluginDescriptionString) {
		debug_String("       PluginDescription :");

	*((const char **) value) =
	    "<a href=\"http://reddit.com/\">thefbi</a> "
	    PLUGIN_VERSION
	    "<br><br>x86 code in a browser";

       debugf("I said '%s'\n",*((const char **) value));

    }

    if (variable == NPPVpluginNeedsXEmbed) {
		debug_Line("NeedsXEmbed :");
	*((PRBool *) value) = PR_FALSE;

    }

    if ((variable != NPPVpluginNameString)
	&& (variable != NPPVpluginDescriptionString)
	&& (variable != NPPVpluginNeedsXEmbed)) {
	result = NPERR_GENERIC_ERROR;
				debug_Line("Value not provided:");
    }
	
	debug_FunctionLeave("plugin_GetValue");

    return result;

}

int RunVX(PluginInstance* InstanceData) {
	
	Dl_info LinkerInfo;
	dladdr(&RunVX,&LinkerInfo);
	debug_String(LinkerInfo.dli_fname);
	debug_String(" <-- Me?  \n");
	char segbuf[256];
        debug_String("converting....");
	sprintf(segbuf,"%i",InstanceData->Info_Segment_ID);
	
	debug_String("converted Info_Segment_ID to ");
	debug_String(segbuf);
	debug_Line("..!\n");
	int pid = fork();
	if (pid == 0) {
		char* args[] = {"thefbi_sandbox_host",segbuf,(char*) 0};
		char* fullpath = calloc(strlen(args[0])+strlen(LinkerInfo.dli_fname)+10,1);
		strncpy(fullpath,LinkerInfo.dli_fname,65536);
		char* walk = fullpath;
		char* filename = fullpath;
		while (*walk != 0) {
			if (*walk++ == '/') filename=walk; 
		}
		debug_Line(filename);
		
		strcpy(filename,args[0]);
		args[0] = fullpath;
		debug_String(args[0]);
		debug_String(" ");
		debug_String(args[1]);
		debug_String("\n");
		
		execv(args[0],args);
		debug_Line("Should have not reached this poing after execvp!");
		sleep(100);
		exit(-1);
	} else  {
		return pid;
	}
	
	
	return 1;
}

void ClearUpPreviousWindow(PluginInstance* InstanceData) {
	debug_FunctionEnter("ClearUpPreviousWindow");
	if (InstanceData->Window == NULL) {
		debug_FunctionLeave("ClearUpPreviousWindow Nothing to do");
		return;//nothing to do
	}
	RemoveEventHandler(InstanceData->EventHandler);
	
	if (InstanceData->Output != NULL){
		I2W_FreeOutput(InstanceData->Output);
			InstanceData->Output = NULL;	
	}
/*
	if (InstanceData->ProvidedOutputSurface !=NULL) {
		shmdt(InstanceData->ProvidedOutputSurface);
		InstanceData->ProvidedOutputSurface=NULL;		
	}
*/
	InstanceData->Window=NULL;
	debug_FunctionLeave("ClearUpPreviousWindow");
}

void ClearUpSharedMemory(PluginInstance* InstanceData) {
	debug_FunctionEnter("ClearUpSharedMemory");

	if (InstanceData->Communication !=NULL) {
		shmdt(InstanceData->Communication);
		InstanceData->Communication=NULL;		
	}

	if (InstanceData->ElfFile !=NULL) {
		shmdt(InstanceData->ElfFile);
			InstanceData->ElfFile=NULL;		
	}

	if (InstanceData->Info !=NULL) {

		shmdt(InstanceData->Info);
		InstanceData->Info=NULL;		
	}


	debug_FunctionLeave("ClearUpSharedMemory");
}

void ClearUpSemaphores(PluginInstance* InstanceData) {
	debug_FunctionEnter("ClearUpSemaphores");

	if (InstanceData->Communication !=NULL) {
		sem_destroy(&(InstanceData->Communication->BrowserActionRequested)); 
		sem_destroy(&(InstanceData->Communication->InputStateUpdated)); 
		sem_destroy(&(InstanceData->Communication->DataDownloaded)); 
	}
	debug_FunctionLeave("ClearUpSharedMemory");
}

void KillVXProcess(PluginInstance* InstanceData) {
	debug_FunctionEnter("KillVxProcess");
	if (InstanceData->vx_pid != 0) {
		kill(InstanceData->vx_pid,SIGKILL);
		InstanceData->vx_pid=0;
	}	
	debug_FunctionLeave("KillVxProcess");
	
}

void FreeInstance(PluginInstance* InstanceData) {
	debug_FunctionEnter("FreeInstance");
	KillVXProcess(InstanceData);
	ClearUpPreviousWindow(InstanceData);
	ClearUpSemaphores(InstanceData);
	ClearUpSharedMemory(InstanceData);
	
	if (InstanceData->WindowObject != NULL) {
		BrowserFunctions.releaseobject(InstanceData->WindowObject);
	}
	if (InstanceData->DocumentObject != NULL) {
		BrowserFunctions.releaseobject(InstanceData->DocumentObject);
	}
	free(InstanceData);
	debug_FunctionLeave("FreeInstance");	
}

#define checkproperty(S) \
 NPIdentifier id_##S = BrowserFunctions.getstringidentifier(#S);\
 if (BrowserFunctions.hasproperty(InstanceData->InstanceID,Object,id_##S)) debugf("Object has property %s\n",#S);
 
  
void LogObject(PluginInstance* InstanceData, NPObject* Object) {
	checkproperty(name);
	checkproperty(document);
	checkproperty(window);
	checkproperty(chicken);
}
 

/* Once-per-instance initialization */
NPError NPP_New(NPMIMEType pluginType,NPP instance,uint16 mode,
		int16 argc,char* argn[],char* argv[],NPSavedData* saved) {
	debug_FunctionEnter("NPP_New");

	
	PluginInstance* InstanceData;
	
	
	if (instance == NULL) {
		debug_FunctionLeave("NPP_New !!NPERR_INVALID_INSTANCE_ERROR!!" );

		return NPERR_INVALID_INSTANCE_ERROR;
	}
	
	instance->pdata = calloc(sizeof(PluginInstance),1);
	InstanceData = (PluginInstance*)instance->pdata;

	if (InstanceData == NULL) {
		debug_FunctionLeave("NPP_New !!NPERR_OUT_OF_MEMORY_ERROR!!" );

		return NPERR_OUT_OF_MEMORY_ERROR;
	}
	InstanceData->InstanceID = instance;

	//  failures beyond this point goto a common exit point to clean up and then free the InstanceData
	
	debug_Line("get the Window object");
	NPError getWindowResult = BrowserFunctions.getvalue(instance,NPNVWindowNPObject,&(InstanceData->WindowObject));
	if (getWindowResult != NPERR_NO_ERROR) goto failure;

	debug_Line("get the Document object");
	NPIdentifier DocumentIdentifier = BrowserFunctions.getstringidentifier("document");
	NPVariant DocumentVariant;

	if (BrowserFunctions.hasproperty(InstanceData->InstanceID,InstanceData->WindowObject,DocumentIdentifier)) debugf("Object has property document\n");
	bool getDocumentResult = BrowserFunctions.getproperty(InstanceData->InstanceID,InstanceData->WindowObject,DocumentIdentifier,&DocumentVariant);
	if (!getDocumentResult) goto failure;
	InstanceData->DocumentObject = BrowserFunctions.retainobject(NPVARIANT_TO_OBJECT(DocumentVariant));
	BrowserFunctions.releasevariantvalue(&DocumentVariant);

	//LogObject(InstanceData,InstanceData->WindowObject);
	
	debug_Line("allocate shared memory zones");

	
	InstanceData->Info_Segment_ID=shmget(IPC_PRIVATE,sizeof(PluginInfo),IPC_CREAT|511);
	if (InstanceData->Info_Segment_ID == 0) goto failure;
	
	InstanceData->Info = shmat(InstanceData->Info_Segment_ID,NULL,0);
	if (InstanceData->Info == NULL) goto failure;
	
	InstanceData->Info->Communication_Segment_ID=shmget(IPC_PRIVATE,sizeof(PluginCommunication),IPC_CREAT|511);
	if (InstanceData->Info->Communication_Segment_ID == 0) goto failure;
	
	InstanceData->Communication = shmat(InstanceData->Info->Communication_Segment_ID,NULL,0);
	if (InstanceData->Communication == NULL) goto failure;

    if(sem_init(&(InstanceData->Communication->BrowserActionRequested),1,0)) goto failure; 
    if(sem_init(&(InstanceData->Communication->InputStateUpdated),1,0)) goto failure; 
    if(sem_init(&(InstanceData->Communication->DataDownloaded),1,0)) goto failure; 
	
	debug_Line("launching VXThread");
	
	InstanceData->vx_pid=RunVX(InstanceData);
	
	if (InstanceData->vx_pid == 0) { 
		debug_Line("failed to launch VXThread");
		goto failure;
	}

	debug_FunctionLeave("NPP_New");

	return NPERR_NO_ERROR;
	
failure:
	debug_Line("NPP_New failure point: attempting to clean up");
	FreeInstance(InstanceData);
	instance->pdata = NULL;
	debug_FunctionLeave("NPP_New  !! Failure!!");
	return NPERR_GENERIC_ERROR;
}



NPError NPP_Destroy(NPP instance, NPSavedData** save)
{
	debug_FunctionEnter("NPP_Destroy");
	PluginInstance* InstanceData;

	if (instance == NULL) {
		debug_FunctionLeave("NPP_Destroy !!NPERR_INVALID_INSTANCE_ERROR!!");
		return NPERR_INVALID_INSTANCE_ERROR;
	}
	InstanceData = (PluginInstance*)instance->pdata;
	if(InstanceData == NULL){
		debug_FunctionLeave("NPP_Destroy !!NPERR_INVALID_INSTANCE_ERROR!!");
		return NPERR_INVALID_INSTANCE_ERROR;
	}

	FreeInstance(InstanceData);
	instance->pdata = NULL;
	
	debug_FunctionLeave("NPP_Destroy");
	return NPERR_NO_ERROR;
}

/* Browser is providing us with a window */
NPError NPP_SetWindow(NPP instance, NPWindow* window)
{
	debug_FunctionEnter("NPP_SetWindow");


	NPError result = NPERR_NO_ERROR;
	PluginInstance* InstanceData;
	
	if (instance == NULL) {
		debug_FunctionLeave("NPP_SetWindow !!NPERR_INVALID_INSTANCE_ERROR!!");
		return NPERR_INVALID_INSTANCE_ERROR;
	}

	InstanceData = (PluginInstance*) instance->pdata;
	
	if( ( window == NULL) || ( window->window == NULL) ) {
	//No window left do any cleanup here
		debug_Line("There's no window anymore");
	        ClearUpPreviousWindow(InstanceData);
	} else {
		debug_Line("D");
		
		if (InstanceData->Window != window) {
		//The window is a different one from last time
			debug_Line("There's a new window");
			ClearUpPreviousWindow(InstanceData);
			debug_Line("Old window cleaned up");
			InstanceData->Window = window;

#ifdef WIN32			
			InstanceData->EventHandler = InstallEventHandler(InstanceData,plugin_EventHandler,(HWND)InstanceData->Window->window);
			InstanceData->Output = I2W_CreateOutput((HWND)(InstanceData->Window->window));
#endif

#ifdef LINUX_PLUGIN				
				NPSetWindowCallbackStruct *ws;
				ws = (NPSetWindowCallbackStruct *)window->ws_info;
				XWindowAttributes	WindowData;
				Status Xresult = XGetWindowAttributes(ws->display,(Window)InstanceData->Window->window,&WindowData);


				InstanceData->Output = I2W_CreateOutput((Window)InstanceData->Window->window,ws->display);
				InstanceData->Info->FrameBuffer_Segment_ID = I2W_GetSharingID(InstanceData->Output);

			    InstanceData->Info->window = (Window)InstanceData->Window->window;
				int FrameBufferSize = WindowData.width*WindowData.height*4;
				InstanceData->Info->FrameBufferWidth=WindowData.width;
				InstanceData->Info->FrameBufferHeight=WindowData.height;
				

				InstanceData->EventHandler = InstallEventHandler(InstanceData,plugin_EventHandler,(Window)InstanceData->Window->window,ws->display);
				
				

#endif

		}
 		debug_Line("Finding OutputBuffer");
		int w = I2W_GetOutputWidth(InstanceData->Output);
		int h = I2W_GetOutputHeight(InstanceData->Output);
		void* s = I2W_GetOutputSurface(InstanceData->Output);
		debug_Line("Attaching OutputBuffer");

		//do any plain stuff here.  Maybe a redraw/invalidate
	}
	debug_FunctionLeave("NPP_SetWindow");
	return result;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++
 * NPP_NewStream:
 * Notifies an instance of a new data stream and returns an error value. 
 * 
 * NPP_NewStream notifies the instance denoted by instance of the creation of
 * a new stream specifed by stream. The NPStream* pointer is valid until the
 * stream is destroyed. The MIME type of the stream is provided by the
 * parameter type. 
 +++++++++++++++++++++++++++++++++++++++++++++++++*/
NPError  NPP_NewStream(NPP instance,NPMIMEType type,NPStream *stream, 
	      NPBool seekable,uint16 *stype) {
	debug_FunctionEnter("NPP_NewStream");

	PluginInstance* InstanceData;

	if(instance==NULL) {
		debug_FunctionLeave("NPP_NewStream !!NPERR_INVALID_INSTANCE_ERROR!!");
		return NPERR_INVALID_INSTANCE_ERROR;
	}
	InstanceData = (PluginInstance*) instance->pdata;
	if(InstanceData == NULL) {
		debug_FunctionLeave("NPP_NewStream !!NNPERR_GENERIC_ERROR!!");
		return NPERR_GENERIC_ERROR;
	}
	/* save the URL for later */
	strncpy(InstanceData->URL,stream->url,2048);

	*stype = NP_NORMAL;  // deliver data via NPP_Write 

	//free any previous URL stream data here before beginning anew	
	
	InstanceData->DataMode=PLUGINDATA_NONELOADED;

	InstanceData->DataSize=stream->end;
	if(stream->end>100000000 || stream->end<0) InstanceData->DataSize=0;
	debug_FunctionLeave("NPP_NewStream");
	return NPERR_NO_ERROR;
}

int32 NPP_WriteReady(NPP instance, NPStream *stream)
{
	debug_FunctionEnter("NPP_WriteReady");
	/* Number of bytes ready to accept in NPP_Write() */
	/* We can handle any amount, so just return some really big number. */

	debug_FunctionLeave("NPP_WriteReady");
	return (int32)0X0FFFFFFF;
}
/*+++++++++++++++++++++++++++++++++++++++++++++++++
 * NPP_Write:
 * Delivers data from a stream and returns the number of bytes written. 
 * 
 * NPP_Write is called after a call to NPP_NewStream in which the plug-in
 * requested a normal-mode stream, in which the data in the stream is delivered
 * progressively over a series of calls to NPP_WriteReady and NPP_Write. The
 * function delivers a buffer buf of len bytes of data from the stream identified
 * by stream to the instance. The parameter offset is the logical position of
 * buf from the beginning of the data in the stream. 
 * 
 * The function returns the number of bytes written (consumed by the instance).
 * A negative return value causes an error on the stream, which will
 * subsequently be destroyed via a call to NPP_DestroyStream. 
 * 
 * Note that a plug-in must consume at least as many bytes as it indicated in the
 * preceeding NPP_WriteReady call. All data consumed must be either processed
 * immediately or copied to memory allocated by the plug-in: the buf parameter
 * is not persistent. 
 +++++++++++++++++++++++++++++++++++++++++++++++++*/
int32 NPP_Write(NPP instance, NPStream *stream, int32 offset, int32 len, void *buffer)
{
	
	debug_FunctionEnter("NPP_Write");
	PluginInstance* InstanceData;
	int NewDataSize = offset+len;
	char* Walk;

	if (instance == NULL){
		debug_FunctionLeave("NPP_Write !!instance == NULL!!");
		return -1;
	}
	InstanceData = (PluginInstance*) instance->pdata;
	if (InstanceData == NULL){
		debug_FunctionLeave("NPP_Write !!instance->pdata == NULL!!");
		 return -1;
	}

	
	if ( (NewDataSize) >= InstanceData->AllocatedDataSize) {
		InstanceData->AllocatedDataSize = NewDataSize+4096;
		InstanceData->Data=realloc(InstanceData->Data,InstanceData->AllocatedDataSize);
	}

	Walk = (char*)InstanceData->Data;
	Walk+=offset;
	memcpy(Walk,buffer,len);
	InstanceData->DataSize = offset+len;	 
	InstanceData->DataMode = PLUGINDATA_PARTIALLYLOADED;
	debug_FunctionLeave("NPP_Write");
	return len; /* The number of bytes accepted -- we always accept them all. */
}

static void *lzmaAlloc(void *p, size_t size) { p = p; debugf("lzma_malloc(%i)\n",size); return malloc(size); }

static void lzmaFree(void *p, void *address) { p = p; free(address); }


static ISzAlloc g_Alloc = { lzmaAlloc, lzmaFree };
void DecompressElf(PluginInstance* InstanceData) {
	char* walk = InstanceData->Data;
	unsigned char* LZMAProps = walk+8;
	unsigned char* LZMAData = LZMAProps + LZMA_PROPS_SIZE + 8;
	ISzAlloc Alloc = { lzmaAlloc, lzmaFree };	

	CLzmaDec state;
	LzmaDec_Construct(&state);

	LzmaDec_Init(&state);
	LzmaDec_Allocate(&state,LZMAProps,LZMA_PROPS_SIZE,&Alloc);
	
	ELzmaStatus status;

	walk+= 13;
	SizeT DecompressedDataSize = *((SizeT*)walk);

	if (DecompressedDataSize > MAXIMUM_FILE_SIZE) return;
	SizeT CompressedDataSize = InstanceData->DataSize-8-LZMA_PROPS_SIZE;
	InstanceData->Info->ElfFileSize = DecompressedDataSize;
	debugf("Expected Decompressed Size: %i\n",InstanceData->Info->ElfFileSize);
	InstanceData->Info->ElfFile_Segment_ID=shmget(IPC_PRIVATE,InstanceData->Info->ElfFileSize,IPC_CREAT|511);
	InstanceData->ElfFile = shmat(InstanceData->Info->ElfFile_Segment_ID,NULL,0);
	
	int res = LzmaDec_DecodeToBuf(&state,
 /*dest*/   InstanceData->ElfFile,&DecompressedDataSize,
 /*source*/ LZMAData,&CompressedDataSize,
            LZMA_FINISH_END,&status); 

	LzmaDec_Free(&state, &Alloc);

	debugf("Decompressed Size: %i   Result:%i    Status: %i\n ",DecompressedDataSize,res,status);
		   
	if (res == SZ_OK) {
		//wake up the VX process to run the elf
		sem_post(&(InstanceData->Communication->DataDownloaded));
	}	

}

NPError NPP_DestroyStream(NPP instance, NPStream *stream, NPError reason)
{
	debug_FunctionEnter("NPP_DestroyStream");
	PluginInstance* InstanceData;

	if (instance == NULL) {
		debug_FunctionLeave("NPP_DestroyStream !!NPERR_INVALID_INSTANCE_ERROR!!");
		return NPERR_INVALID_INSTANCE_ERROR;
	}
	InstanceData = (PluginInstance*) instance->pdata;
	if (InstanceData == NULL){
		debug_FunctionLeave("NPP_DestroyStream !!NPERR_INVALID_INSTANCE_ERROR!!");
		 return NPERR_INVALID_INSTANCE_ERROR;
	}

	if(reason==NPRES_DONE) {
		debug_Line("Plugin data arrived.");
		InstanceData->DataMode=PLUGINDATA_FULLYLOADED;
		DecompressElf(InstanceData);
	}
	debug_FunctionLeave("NPP_DestroyStream");
	return NPERR_NO_ERROR;
}

/* This should not get called, since we aren't using the NP_ASFILE mode */
void NPP_StreamAsFile(NPP instance, NPStream *stream, const char* fname)
{
	debug_FunctionEnter("NPP_StreamasFile");

	debug_FunctionLeave("NPP_StreamasFile");
	return;
}

int16 NPP_HandleEvent(NPP instance, void* event)
{
	debug_FunctionEnter("NPP_HandleEvent");
	debug_FunctionLeave("NPP_HandleEvent");
	return 0;
}

void NPP_URLNotify(NPP instance, const char* url, NPReason reason, void* notifyData)
{
	debug_FunctionEnter("NPP_URLNotify");
	debug_FunctionLeave("NPP_URLNotify");
	return;
}

/* Print embedded plug-in (via the browser's Print command) */
void NPP_Print(NPP instance, NPPrint* printInfo)
{
	debug_FunctionEnter("NPP_Print");
	PluginInstance* InstanceData;
	if (instance == NULL){
		debug_FunctionLeave("NPP_Print !!Instance==NULL!!");
		return;
	}
	InstanceData = (PluginInstance*) instance->pdata;
	if (InstanceData == NULL){
		debug_FunctionLeave("NPP_Print !!Instance->pdata==NULL!!");		
		return;
	}

	if(printInfo == NULL) {
		/* Though it's not documented, if a Netscape user tries to print a full-page
		 * plugin, the browser will call NPP_Print with printInfo set to NULL.
		 * At this point you should print your plug-in in whatever manner you
		 * choose. */

		/* PrintFullPage();  */    // not implemented 
		debug_FunctionLeave("NPP_Print ??printinfo==NULL??");		
		return;
	}
	debug_FunctionLeave("NPP_Print");

	// this isn't implemented either
}
