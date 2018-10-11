
#include "tinydebug.h"

#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/shm.h>

#include "vxplugin.h"
#include "thefbivxthread.h"

int RunVX(void* Arg) {
	PluginInfo* Info = (PluginInfo*) Arg;
	
	VXThreadData VXThread;
	VXThread.Info = Info;
	
	debugf("VXProcess looking for communication segment\n");

	while (Info->Communication_Segment_ID == 0) usleep(100);	
	VXThread.Communication = shmat(Info->Communication_Segment_ID,NULL,0);
	debugf("VXProcess: Communication memory at %x\n",VXThread.Communication);
	while (Info->FrameBuffer_Segment_ID == 0) usleep(100);	
	VXThread.FrameBufferSurface = shmat(Info->FrameBuffer_Segment_ID,NULL,0);
	debugf("VXProcess: FrameBuffer memory at %x\n",VXThread.Communication);
	
	vx32_siginit();
	
	debug_String("Setting up VX32 instance\n");
	VXThread.VX = vxproc_alloc();
	VXThread.VX->allowfp = True;
	
	sem_wait(&(VXThread.Communication->DataDownloaded));
	
	debug_String("vxproc_loadelffile\n");
	VXThread.ElfFile = shmat(Info->ElfFile_Segment_ID,NULL,0);
	debugf("file ptr: $%x, length %i\n",VXThread.ElfFile, Info->ElfFileSize);
	//if (vxproc_loadelffile(VXThread.VX, "ThefbiTestExe", NULL, NULL) < 0) {
	  if (vxproc_loadelfmem(VXThread.VX,VXThread.ElfFile, Info->ElfFileSize, NULL, NULL) < 0) {
		debug_String("vxnproc_loadelffile Failed\n");
		return 0;
	}
	debug_String("elffile loaded\n");

	ThefbiVXProcessHandler(&VXThread);
	
	
	return 1;
}

int main(int argc, char** argv)
{
   if (argc != 2) return -1;
	
   int info_segment_id = atoi(argv[1]);

   if (info_segment_id == 0) return -1;
	
   void* Info = shmat(info_segment_id,NULL,SHM_RDONLY);
   if (Info == (void*) -1) {
	   debug_String("VXProcess: failed to get Info structure\n");
	   return -1;
   }
	   
   RunVX(Info);		
   return 0;
}
