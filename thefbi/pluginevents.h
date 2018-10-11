
typedef void* EventHandlerID;
typedef unsigned int EventResult;

typedef enum  {
	ekExpose, ekTimer,ekKeyDown,ekKeyUp,ekMouseDown,ekMouseUp, ekMouseMove, ekScriptRequest
} tEventKind;

typedef struct _PluginKeyEvent {
	tEventKind eventkind;
	int RawCode;
	int SybolicCode;
} tPluginKeyEvent;

typedef struct _PluginMouseButtonEvent {
	tEventKind eventkind;
	int Button;
	int PageX;
	int PageY;
	int ElementX;
	int ElementY;
	int ScreenX;
	int ScreenY;
	
} tPluginMouseButtonEvent;

typedef struct _PluginMouseMoveEvent {
	tEventKind eventkind;
	int PageX;
	int PageY;
	int ElementX;
	int ElementY;
	int ScreenX;
	int ScreenY;
} tPluginMouseMoveEvent;

typedef struct _PluginScriptEvent {
	tEventKind eventkind;	
} tPluginScriptEvent;


typedef union _TranslatedEvent {
	tEventKind eventkind;
	tPluginKeyEvent KeyEvent;
	tPluginMouseButtonEvent MouseButtonEvent;
	tPluginMouseMoveEvent MouseMoveEvent;
	tPluginScriptEvent ScriptEvent;
	//extra event fields go here
	long Timer_MillisecondsSinceLastEvent;
} tPluginTranslatedEventData, *pPluginTranslatedEventData;



typedef EventResult (*Plugin_Event_Handler_Function) (void*,tEventKind,pPluginTranslatedEventData);

#ifdef __cplusplus
extern "C" {
#endif

#ifdef LINUX_PLUGIN

#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/cursorfont.h>
#include <X11/Xutil.h>
#include <X11/Intrinsic.h>
#include <X11/IntrinsicP.h>
#include <X11/Core.h>
#include <X11/CoreP.h>

EventHandlerID InstallEventHandler(void* Instance,Plugin_Event_Handler_Function EventFunction,Window XWindow,Display* XDisplay);

#endif

#ifdef WIN32
#include <windows.h>
EventHandlerID InstallEventHandler(void* Instance,Plugin_Event_Handler_Function,HWND Window);
#endif

void RemoveEventHandler(EventHandlerID);

#ifdef __cplusplus
}
#endif

