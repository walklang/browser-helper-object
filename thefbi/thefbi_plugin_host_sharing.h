#include <semaphore.h>


typedef struct plugincommunication_struct {
	bool RedrawRequired;
	sem_t BrowserActionRequested;
	sem_t InputStateUpdated;	
	sem_t DataDownloaded;	
	char Buffer[65500];
} PluginCommunication;

#define PLUGIN_STATE_FLAG_FRAMEBUFFER 1
#define PLUGIN_STATE_FLAG_Somethng (1 << 1)
#define PLUGIN_STATE_FLAG_MOUSEDOWN (1 << 2)
#define PLUGIN_STATE_FLAG_MOUSEUP (1 << 3)
#define PLUGIN_STATE_FLAG_MOUSEPOSITION (1 << 4)
#define PLUGIN_STATE_FLAG_AGE (1 << 5)
#define PLUGIN_STATE_FLAG_ACTIVEAGE (1 << 6)
#define PLUGIN_STATE_FLAG_VISIBLEAGE (1 << 7)
#define PLUGIN_STATE_FLAG_KEYDOWN (1 << 8)
#define PLUGIN_STATE_FLAG_KEYUP (1 << 9)
#define PLUGIN_STATE_FLAG_KEYSTATE (1 << 10)

typedef struct plugininfo_struct {
	int FrameBufferWidth;
	int FrameBufferHeight;
	int MouseX;
	int MouseY;
	int Age;
	int ActiveAge;
	int VisibleAge;
	char Keymap_Literal[256];
	char Keymap_Symbolic[256];
	int StateChangeFlags;
	int StateChangeFlags_accumulator;
	int ElfFileSize;
	Window window; //this should only really be used for XSendEvent   
	int Communication_Segment_ID;
	int FrameBuffer_Segment_ID;
	int ElfFile_Segment_ID;
} PluginInfo;

