#include "pluginevents.h"
#include "nspluginbase.h"
#include "tinydebug.h"

#include <sys/time.h>

#ifdef LINUX_PLUGIN


typedef struct _EventHandlerContext {
	void* Instance;
	Plugin_Event_Handler_Function EventFunction;
	GC	XContext;
	Window XWindow;
	Display* XDisplay;
	Widget	MotifWidget;
	XtIntervalId TimerId;
	struct timeval LastTimerCallbackTime;
} EventHandlerData, *pEventHandlerData;

Atom script_event_atom;

void TranslateXKeyEvent(XEvent* xevent, tPluginTranslatedEventData* translatedevent) {
   tPluginTranslatedEventData* t = translatedevent;
   t->KeyEvent.RawCode = xevent->xkey.keycode;
   t->KeyEvent.eventkind = (xevent->type == KeyPress) ? ekKeyDown : ekKeyUp;
}

void TranslateXMouseMotionEvent(XEvent* xevent, tPluginTranslatedEventData* translatedevent) {
   tPluginTranslatedEventData* t = translatedevent;
   t->MouseMoveEvent.ElementX = xevent->xmotion.x;
   t->MouseMoveEvent.ElementY = xevent->xmotion.y;
   t->MouseMoveEvent.ScreenX = xevent->xmotion.x_root;
   t->MouseMoveEvent.ScreenY = xevent->xmotion.y_root;   
   t->KeyEvent.eventkind = ekMouseMove;
}

void TranslateXMouseButtonEvent(XEvent* xevent, tPluginTranslatedEventData* translatedevent) {
   tPluginTranslatedEventData* t = translatedevent;
   t->MouseButtonEvent.ElementX = xevent->xbutton.x;
   t->MouseButtonEvent.ElementY = xevent->xbutton.y;
   t->MouseButtonEvent.ScreenX = xevent->xbutton.x_root;
   t->MouseButtonEvent.ScreenX = xevent->xbutton.y_root;   
   t->MouseButtonEvent.Button = xevent->xbutton.button;
   t->MouseButtonEvent.eventkind= (xevent->type == ButtonPress) ? ekMouseDown : ekMouseUp; 	 
}
 
static void LocalEventHandler(Widget w, XtPointer client_data, XEvent *event, Boolean *dispatch) {
	pEventHandlerData HandlerData;
	debug_FunctionEnter("LocalEventHandler");
	HandlerData = (pEventHandlerData) client_data;
	tPluginTranslatedEventData TranslatedEvent;

	if(HandlerData == NULL) {
		debug_FunctionLeave("LocalEventHandler !!HandlerData == NULL!!");
		return;
	}
	switch(event->type) {
	case Expose:
		{
			debug_Line("Expose");
			HandlerData->EventFunction(HandlerData->Instance,ekExpose,NULL);
		}
		debug_FunctionLeave("LocalEventHandler done Expose");
		return;
	case KeyPress:
		{
			debug_Line("KeyPress");
			TranslateXKeyEvent(event,&TranslatedEvent);
			HandlerData->EventFunction(HandlerData->Instance,ekKeyDown,&TranslatedEvent);
		}
		debug_FunctionLeave("LocalEventHandler done KeyPress");
		return;
	case KeyRelease:
		{
			debug_Line("KeyRelease");
			TranslateXKeyEvent(event,&TranslatedEvent);
			HandlerData->EventFunction(HandlerData->Instance,ekKeyUp,&TranslatedEvent);
		}
		debug_FunctionLeave("LocalEventHandler done KeyRelease");
		return;
	case ButtonPress:
		{
			debug_Line("ButtonPress");
			TranslateXMouseButtonEvent(event,&TranslatedEvent);
			HandlerData->EventFunction(HandlerData->Instance,ekKeyUp,&TranslatedEvent);
		}
		debug_FunctionLeave("LocalEventHandler done ButtonPress");
		return;
	case ButtonRelease:
		{
			debug_Line("ButtonRelease");
			TranslateXMouseButtonEvent(event,&TranslatedEvent);
			HandlerData->EventFunction(HandlerData->Instance,ekKeyUp,&TranslatedEvent);

		}
		debug_FunctionLeave("LocalEventHandler done ButtonRelease");
		return;
	case MotionNotify:
		{
			debug_Line("MotionNotify");
			TranslateXMouseMotionEvent(event,&TranslatedEvent);
			Window root_return,child_return;
			int root_x_return,root_y_return;
			int win_x_return,win_y_return;
			int mask_return;
			int res = XQueryPointer
						(event->xany.display,event->xany.window,
						 &root_return,&child_return,
						 &root_x_return,&root_y_return,
						 &win_x_return,&win_y_return,
						 &mask_return);
			if (res) {
				TranslatedEvent.MouseMoveEvent.ElementX=win_x_return;
				TranslatedEvent.MouseMoveEvent.ElementY=win_y_return;
				TranslatedEvent.MouseMoveEvent.ScreenX=root_x_return;
				TranslatedEvent.MouseMoveEvent.ScreenY=root_y_return;
			}			  
			HandlerData->EventFunction(HandlerData->Instance,ekMouseMove,&TranslatedEvent);

		}
		debug_FunctionLeave("LocalEventHandler done MotionNotify");
		return;
	case ClientMessage:
		{
			debugf("ClientMessage  message_type=%i",event->xclient.message_type);
			if (event->xclient.message_type == script_event_atom) {
				debug_Line("ClientMessage is a script event");
				debugf("Event function is $%x\n",HandlerData->EventFunction);
				debugf("Instance is $%x\n",HandlerData->Instance);
				HandlerData->EventFunction(HandlerData->Instance,ekScriptRequest,NULL);
			}
		}
		debug_FunctionLeave("LocalEventHandler done ClientMessage");
		return;

	}
	debug_FunctionLeave("LocalEventHandler fallthrough");
	return; 
}

 int GetMillisecondsElapsed(struct timeval *starttime, struct timeval *newtime){
        int elapsed;
        elapsed = 0;
		int carry = -1;
        if ( (newtime->tv_usec - starttime->tv_usec) > 0 ) {
                elapsed += (newtime->tv_usec -
                starttime->tv_usec)/1000 ;
        } else {
                elapsed += (1000000 + newtime->tv_usec -
                starttime->tv_usec)
                        /1000 ;
				carry = -1;
        }
        if ( (newtime->tv_sec+ carry  - starttime->tv_sec) > 0 ) {
                elapsed += 1000 * (newtime->tv_sec+carry -
                starttime->tv_sec) ;
        }      
        return(elapsed);

} 
static void LocalTimerCallback(XtPointer client_data, XtIntervalId *id)
{
	struct timeval Now;
	pEventHandlerData HandlerData;
	tPluginTranslatedEventData TimerEventData;
	int MilliSecondsElapsed;
	//debug_FunctionEnter("LocalTimerCallback");
	HandlerData = (pEventHandlerData) client_data;
	gettimeofday(&Now,NULL);
	MilliSecondsElapsed = GetMillisecondsElapsed(&HandlerData->LastTimerCallbackTime,&Now);
	HandlerData->LastTimerCallbackTime = Now;
	TimerEventData.Timer_MillisecondsSinceLastEvent = MilliSecondsElapsed;

	HandlerData->EventFunction(HandlerData->Instance,ekTimer,&TimerEventData);

	HandlerData->TimerId = XtAppAddTimeOut(XtWidgetToApplicationContext(HandlerData->MotifWidget),5,(XtTimerCallbackProc) LocalTimerCallback,(XtPointer) HandlerData);
	//debug_FunctionLeave("LocalTimerCallback");
	
}

EventHandlerID InstallEventHandler(void* Instance,Plugin_Event_Handler_Function EventFunction,Window XWindow,Display* XDisplay) {
	pEventHandlerData Result = calloc(sizeof(EventHandlerData),1);
	debug_FunctionEnter("InstallEventHandler");
	script_event_atom = XInternAtom(XDisplay,"Javascript_ClientMessage",false);
	
	Result->Instance = Instance;
	Result->EventFunction = EventFunction;
	Result->XWindow = XWindow;
	Result->XDisplay = XDisplay;
	Result->XContext = DefaultGC(Result->XDisplay, DefaultScreen(Result->XDisplay));
	Result->MotifWidget = XtWindowToWidget(Result->XDisplay,Result->XWindow);

	XtAddEventHandler(Result->MotifWidget, ExposureMask | KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask,
		True, (XtEventHandler) LocalEventHandler, (XtPointer)Result);

	Result->TimerId =XtAppAddTimeOut (XtWidgetToApplicationContext(Result->MotifWidget),5,(XtTimerCallbackProc) LocalTimerCallback,(XtPointer) Result);
	debug_FunctionLeave("InstallEventHandler");

	return (EventHandlerID) Result;
}

void RemoveEventHandler(EventHandlerID ID) {
	pEventHandlerData Handler = (pEventHandlerData)ID;
	debug_FunctionEnter("RemoveEventHandler");
	XtRemoveTimeOut(Handler->TimerId);
	debug_Line("Removed timer");

	//Removing the event handler callback  is being done automatically it appears.
	//XtRemoveEventHandler(Handler->MotifWidget, ExposureMask,
	//	True, (XtEventHandler) LocalEventHandler, (XtPointer)Handler);

	//debug_Line("Removed event callback");
	
	free(Handler);
	debug_FunctionLeave("RemoveEventHandler");

}

#endif

#ifdef WIN32
typedef struct _EventHandlerContext {
	void* Instance;
	WNDPROC    DefaultWindowProc;
	HWND	WindowHandle;
	Plugin_Event_Handler_Function EventFunction;

} EventHandlerData, *pEventHandlerData;

static const char* gEventHandlerCallName = "evtrans";

static LRESULT CALLBACK LocalEventHandler( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	pEventHandlerData HandlerData;
	debug_FunctionEnter("LocalEventHandler");

	HandlerData = (pEventHandlerData) GetProp(hWnd, gEventHandlerCallName);

	if(HandlerData == NULL) {
		debug_FunctionLeave("LocalEventHandler !!HandlerData == NULL!!");

		return DefWindowProc( hWnd, Msg, wParam, lParam);
	}
	switch(Msg) {

	case WM_ERASEBKGND:{debug_Line("WM_Paint");debug_FunctionLeave("LocalEventHandler"); return 1;}

	case WM_PAINT:
		{
			PAINTSTRUCT paintStruct;
			HDC hdc;
			debug_Line("WM_Paint");
			hdc = BeginPaint( hWnd, &paintStruct );
			EndPaint( hWnd, &paintStruct );
			HandlerData->EventFunction(HandlerData->Instance,0,NULL);
		}
		debug_FunctionLeave("LocalEventHandler donepaint");

		return 0;
	}
	debug_FunctionLeave("LocalEventHandler fallthrough");
	return HandlerData->DefaultWindowProc(hWnd, Msg, wParam, lParam);
}

EventHandlerID InstallEventHandler(void* Instance,Plugin_Event_Handler_Function EventFunction,HWND Window) {
	pEventHandlerData Result = calloc(sizeof(EventHandlerData),1);
	debug_FunctionEnter("InstallEventHandler");

	Result->Instance = Instance;
	Result->EventFunction = EventFunction;
	Result->WindowHandle = Window;
	Result->DefaultWindowProc = (WNDPROC)SetWindowLong(Window,GWL_WNDPROC,(LONG)LocalEventHandler);
	SetProp(Window, gEventHandlerCallName,(HANDLE) Result);

	debug_FunctionLeave("InstallEventHandler");

	return (EventHandlerID) Result;
}

void RemoveEventHandler(EventHandlerID ID) {
	pEventHandlerData Handler = (pEventHandlerData)ID;
	debug_FunctionEnter("RemoveEventHandler");

	SetWindowLong(Handler->WindowHandle, GWL_WNDPROC,(long) Handler->DefaultWindowProc);
	RemoveProp(Handler->WindowHandle,gEventHandlerCallName);
	free(Handler);
	debug_FunctionLeave("RemoveEventHandler");

}

#endif

