#include "imagetowindow.h"
#include "tinydebug.h"

#include <stdlib.h>

#ifdef WINDOWS_PLUGIN
#include <windows.h>
#endif

#ifdef OUTPUT_X
#include <sys/ipc.h>
#include <sys/shm.h>
#include <X11/extensions/XShm.h>

typedef struct _OutputData {
	int Width;
	int Height;
	void* Surface;
	int SurfaceSegmentID;
	int OwnSurface;
	int OwnImage; 
	int UsingSharedSurface;
	int UsingXSHM;
	XShmSegmentInfo SHMInfo;
	XImage* Image;
	GC	XContext;
	Display* XDisplay;
	Window XWindow;
} I2W_OutputData;

void I2W_InitImage(XImage* Image, int Width, int Height, void* Surface) {
		Image->width = Width;
		Image->height = Height;
		Image->format = ZPixmap;
		Image->bitmap_unit = 8;
		Image->bitmap_pad = 8;
		Image->byte_order = LSBFirst;
		Image->depth = 24;
		Image->bits_per_pixel = 32;
		Image->red_mask = 0xff0000;
		Image->green_mask = 0x00ff00;
		Image->blue_mask = 0x0000ff;
		
		Image->data = Surface;
	 
		XInitImage(Image);
		
	int x;
	int y;
	unsigned char* pos = (unsigned char*) Surface;

	for (y = 0; y < Height; y++)
	{
	for (x = 0; x < Width; x++)
		{
		*pos++ = x - x % 32;			/* Red. */
		*pos++ = (x / 32) * 4 + y - y % 32;	/* Green. */
		*pos++ = y - y % 32;			/* Blue. */
		*pos++ = 0xff;			// pad 
		}
	}
		
}

I2W_SetupSurface(I2W_OutputData* Output) {
/*	
	Output->OwnImage=true;
    Output->image=calloc(1,sizeof(XImage);
	
		Output->Image->width = Output->Width;
		Output->Image->height = Output->Height;
		Output->Image->format = ZPixmap;
		Output->Image->bitmap_unit = 8;
		Output->Image->bitmap_pad = 8;
		Output->Image->yte_order = LSBFirst;
		Output->Image->depth = 24;
		Output->Image->bits_per_pixel = 32;
		Output->Image->red_mask = 0xff0000;
		Output->Image->green_mask = 0x00ff00;
		Output->Image->blue_mask = 0x0000ff;
		
		Output->Image->data = Output->Surface;
	 
		XInitImage(Output->Image);
*/		
	int x;
	int y;
	unsigned char* pos = (unsigned char*) Output->Surface;

	for (y = 0; y < Output->Height; y++)
	{
	for (x = 0; x < Output->Width; x++)
		{
		*pos++ = x - x % 32;			/* Red. */
		*pos++ = (x / 32) * 4 + y - y % 32;	/* Green. */
		*pos++ = y - y % 32;			/* Blue. */
		*pos++ = 0xff;			// pad 
		}
	}

}

I2W_OutputHandle I2W_CreateOutput(Window XWindow,Display* XDisplay)
 {
	I2W_OutputData* Output = (I2W_OutputData*) calloc(sizeof(I2W_OutputData),1);
	debug_Line("creating FrameBuffer");	
	Output->XWindow = XWindow;
	Output->XDisplay = XDisplay;
	Output->XContext = XCreateGC(Output->XDisplay,Output->XWindow,0,NULL);

 	XWindowAttributes	WindowData;
	Status Xresult = XGetWindowAttributes(XDisplay,XWindow,&WindowData);
	
	Output->Width = WindowData.width;
	Output->Height = WindowData.height;
	
	
	Output->UsingSharedSurface = 1;
	int FrameBufferSize = Output->Width*Output->Height*4;
	debugf("FrameBuffer Size = %i\n",FrameBufferSize);	

	Output->SurfaceSegmentID=shmget(IPC_PRIVATE,FrameBufferSize,IPC_CREAT|0777);
    Output->Surface = shmat(Output->SurfaceSegmentID,NULL,0);

	debugf("FrameBuffer Surface = %x\n",Output->Surface);	

	Output->OwnSurface = 1;
	//Output->Surface = calloc(Output->Width*Output->Height*4,1);
   
	Output->SHMInfo.shmaddr = Output->Surface;
	Output->SHMInfo.readOnly = False;
	Output->SHMInfo.shmid = Output->SurfaceSegmentID;

	if (XShmQueryExtension(XDisplay)) {
		Visual* visual = XDefaultVisual(XDisplay,0);	
		Output->UsingXSHM = 1;
		Output->Image = XShmCreateImage(
			XDisplay,visual,DefaultDepth(XDisplay,0),ZPixmap,	    
			Output->Surface,&Output->SHMInfo,
			Output->Width,Output->Height);
	} else {
		Output->OwnImage = 1;
		Output->Image = calloc(sizeof(XImage),1);
		I2W_InitImage(Output->Image,Output->Width,Output->Height,Output->Surface);
	}
	
	debugf("Created Image = %x\n",Output->Image);
	XShmAttach(XDisplay,&Output->SHMInfo);
		
	//I2W_SetupSurface(Output);
	
	return (I2W_OutputHandle) Output;
}

int I2W_GetSharingID(I2W_OutputHandle Handle) {
	I2W_OutputData* OutputData = (I2W_OutputData*) Handle;
	 if (OutputData->UsingSharedSurface) {
		 return OutputData->SurfaceSegmentID;
	 } else return 0;
}

/*
I2W_OutputHandle I2W_CreateOutput_UsingProvidedSurface(Window XWindow,Display* XDisplay, void* Surface) {
	I2W_OutputData* Output = (I2W_OutputData*) calloc(sizeof(I2W_OutputData),1);

	Output->OwnSurface = 0;
	Output->XWindow = XWindow;
	Output->XDisplay = XDisplay;
	Output->Surface = Surface;

 	XWindowAttributes	WindowData;
	Status Xresult = XGetWindowAttributes(XDisplay,XWindow,&WindowData);
	
	Output->Width = WindowData.width;
	Output->Height = WindowData.height;
	
	I2W_SetupSurface(Output);
	
	return (I2W_OutputHandle) Output;
		
}
*/
/*
I2W_OutputHandle I2W_CreateOutput_UsingProvidedSHMID(Window XWindow,Display* XDisplay, int shmid) {
	Output->OwnSurface = 0;
	Output->UsingXSHM = 1;
	Output->XWindow = XWindow;
	Output->XDisplay = XDisplay;
	Output->Surface = Surface;

 	XWindowAttributes	WindowData;
	Status Xresult = XGetWindowAttributes(XDisplay,XWindow,&WindowData);
	
	Output->Width = WindowData.width;
	Output->Height = WindowData.height;

	Output->XContext = XCreateGC(Output->XDisplay,Output->XWindow,0,NULL);

    Ourput->SHMInfo.shmid = shmid;
    Ourput->SHMInfo.shmid = shmid;
    
	Output->Image.width = Output->Width;
	Output->Image.height = Output->Height;
	Output->Image.format = ZPixmap;
	Output->Image.bitmap_unit = 8;
	Output->Image.bitmap_pad = 8;
	Output->Image.byte_order = LSBFirst;
	Output->Image.depth = 24;
	Output->Image.bits_per_pixel = 32;
	Output->Image.red_mask = 0xff0000;
	Output->Image.green_mask = 0x00ff00;
	Output->Image.blue_mask = 0x0000ff;
		
	Output->Image.data = Output->Surface;

}
*/
void I2W_DrawSomething(I2W_OutputHandle Handle)
{
	I2W_OutputData* OutputData = (I2W_OutputData*) Handle;
 	int x;
	int y;
	unsigned long c = rand();
	for (y = 0; y < 16; y++)
	{
  	unsigned long *pos = (unsigned long *) OutputData->Surface;
  	pos+=OutputData->Width*y;
		for (x = 0; x < 16; x++)
			{
			  *pos++=c;
		}
	}
}

void I2W_FreeOutput(I2W_OutputHandle Handle) {
	I2W_OutputData* OutputData = (I2W_OutputData*) Handle;
	//XDestroyImage(OutputData->Image);
	if (OutputData->OwnImage) free(OutputData->Image);
	if (OutputData->OwnSurface) {
		if (OutputData->UsingSharedSurface) {
			shmdt(OutputData->Surface);
		} else {
			free(OutputData->Surface);
		}
	}
	free(OutputData);
}

void I2W_Draw(I2W_OutputHandle Handle) {
	I2W_OutputData* OutputData = (I2W_OutputData*) Handle;
	debug_Line("Draw ");
	if (OutputData->UsingXSHM) {
		debug_Line("SHM ");
		debugf("XShmPutImage($%x, $%x, $%x, $%x,0,0,0,0, %i, %i,False);\n",
			OutputData->XDisplay, OutputData->XWindow, OutputData->XContext, OutputData->Image, OutputData->Width, OutputData->Height);
			
		Status result = XShmPutImage(OutputData->XDisplay, OutputData->XWindow, OutputData->XContext, OutputData->Image,0,0,0,0, OutputData->Width, OutputData->Height,False);
		XSync(OutputData->XDisplay,False);
		
		debugf("status = %i\n",result);
	} else {
		debug_Line("Normal ");

		XPutImage(OutputData->XDisplay, OutputData->XWindow, OutputData->XContext, OutputData->Image,0,0,0,0, OutputData->Width, OutputData->Height);
	}
	debug_Line("Done Draw ");

}

#endif

#ifdef OUTPUT_GDK

#include <gdk/gdk.h>
#include <gdk/gdk.h>
typedef struct _OutputData {
	int Width;
	int Height;
	void* Surface;
	GdkWindow* GDKWindow;
	GdkGC* GDKContext;
	GC	XContext;
	Display* XDisplay;
	Window XWindow;
} I2W_OutputData;

I2W_OutputHandle I2W_CreateOutput(Window XWindow,Display* XDisplay)
 {
	I2W_OutputData* Output = (I2W_OutputData*) calloc(sizeof(I2W_OutputData),1);

	gdk_init (NULL,NULL);
  	gdk_rgb_init();

	Output->XWindow = XWindow;
	Output->XDisplay = XDisplay;

	XWindowAttributes	WindowData;

	Status Xresult = XGetWindowAttributes(XDisplay,XWindow,&WindowData);
	
	Output->Width = WindowData.width;
	Output->Height = WindowData.height;
	
	Output->GDKWindow = gdk_window_foreign_new((guint)(XWindow));
	Output->GDKContext = gdk_gc_new(Output->GDKWindow);

	Output->Surface = calloc(Output->Width*Output->Height*4,1);
	int x;
	int y;
	guchar* pos = (guchar*) Output->Surface;
	for (y = 0; y < Output->Height; y++)
	{
	for (x = 0; x < Output->Width; x++)
		{
		*pos++ = x - x % 32;			/* Red. */
		*pos++ = (x / 32) * 4 + y - y % 32;	/* Green. */
		*pos++ = y - y % 32;			/* Blue. */
		*pos++ = 0xff;			// pad 
		}
	}

	return (I2W_OutputHandle) Output;
}


void I2W_DrawSomething(I2W_OutputHandle Handle)
{
	I2W_OutputData* OutputData = (I2W_OutputData*) Handle;
 	int x;
	int y;
	unsigned long c = rand();
	for (y = 0; y < 16; y++)
	{
  	unsigned long *pos = (unsigned long *) OutputData->Surface;
  	pos+=OutputData->Width*y;
		for (x = 0; x < 16; x++)
			{
			  *pos++=c;
		}
	}
}

void I2W_FreeOutput(I2W_OutputHandle Handle) {
	I2W_OutputData* OutputData = (I2W_OutputData*) Handle;
	gdk_gc_unref(OutputData->GDKContext);
	gdk_window_unref(OutputData->GDKWindow);

	free(OutputData->Surface);
	free(OutputData);
}

void I2W_Draw(I2W_OutputHandle Handle) {
	I2W_OutputData* OutputData = (I2W_OutputData*) Handle;
	gdk_draw_rgb_32_image (OutputData->GDKWindow,OutputData->GDKContext,
		      0, 0, OutputData->Width, OutputData->Height,
		      GDK_RGB_DITHER_MAX, OutputData->Surface, OutputData->Width * 4);
}




#endif

#ifdef OUTPUT_WINDOWS

typedef struct _OutputData {
	int Width;
	int Height;
	void* Surface;
	HWND Window;
	BITMAPV4HEADER BitmapInfoHeader;
	HBITMAP DIB;
	
} I2W_OutputData;


I2W_OutputHandle I2W_CreateOutput(HWND Window){
	I2W_OutputData* Output = (I2W_OutputData*) calloc(sizeof(I2W_OutputData),1);

	RECT WindowRect;
	PBITMAPV4HEADER bmh;
	HDC DC;
	unsigned char* pos;
	int x;
	int y;
	
	

	GetClientRect(Window,&WindowRect);

	Output->Window = Window;
	Output->Width = WindowRect.right - WindowRect.left;
	Output->Height = WindowRect.bottom - WindowRect.top;

	DC = GetDC(Window);
	
	bmh=&Output->BitmapInfoHeader;
	bmh->bV4Size= sizeof(BITMAPV4HEADER);
	bmh->bV4RedMask =	0x000000ff;
	bmh->bV4GreenMask = 0x0000ff00;
	bmh->bV4BlueMask =	0x00ff0000;
	bmh->bV4Width = Output->Width ;
	bmh->bV4Height = -Output->Height;
	bmh->bV4Planes = 1;
	bmh->bV4BitCount =32;
	bmh->bV4V4Compression = BI_BITFIELDS;

	Output->DIB = CreateDIBSection(DC,&Output->BitmapInfoHeader,DIB_RGB_COLORS,&(Output->Surface),NULL,0);
	ReleaseDC(Window,DC);
	

	pos = (unsigned char*) Output->Surface;
	for (y = 0; y < Output->Height; y++)
	{
	for (x = 0; x < Output->Width; x++)
		{
		*pos++ = x - x % 32;			// Red. 
		*pos++ = (x / 32) * 4 + y - y % 32;	// Green. 
		*pos++ = y - y % 32;			// Blue. 
		*pos++ = 0xff;			// pad 
		}
	}

	return (I2W_OutputHandle) Output;
}

void I2W_FreeOutput(I2W_OutputHandle Handle) {
	
	I2W_OutputData* OutputData = (I2W_OutputData*) Handle;
	DeleteObject(OutputData->DIB);
	free(OutputData);
}

void I2W_Draw(I2W_OutputHandle Handle) {
	I2W_OutputData* OutputData = (I2W_OutputData*) Handle;
	HDC DC;
	HDC SourceDC;
	HBITMAP BitmapHold;
	
	PAINTSTRUCT ps;  //possibly set this up to do a begin/end paint
	//DC=BeginPaint(OutputData->Window, &ps );

	DC = GetDC(OutputData->Window);
	StretchDIBits(DC,
	0,0,OutputData->Width,OutputData->Height,
	0,0,OutputData->Width,OutputData->Height,
	OutputData->Surface,
	(LPBITMAPINFO)&(OutputData->BitmapInfoHeader),DIB_RGB_COLORS,SRCCOPY);

	//DC= BeginPaint(hwnd, &ps);
/*    SourceDC= CreateCompatibleDC(DC);
    BitmapHold = SelectObject(SourceDC, OutputData->DIB);

	BitBlt(DC,0,0,OutputData->Width,OutputData->Height,SourceDC,0,0,SRCCOPY);
	
	SelectObject(SourceDC, BitmapHold);
    
	DeleteDC(SourceDC);
*/
    //EndPaint(OutputData->Window, &ps);
	ReleaseDC(OutputData->Window,DC);
}

#endif

void* I2W_GetOutputSurface(I2W_OutputHandle Handle) {
	I2W_OutputData* OutputData = (I2W_OutputData*) Handle;
	if (OutputData == NULL) return NULL;
	return OutputData->Surface;
}

int I2W_GetOutputWidth(I2W_OutputHandle Handle) {
	I2W_OutputData* OutputData = (I2W_OutputData*) Handle;
	if (OutputData == NULL) return 0;
	return OutputData->Width;
}

int I2W_GetOutputHeight(I2W_OutputHandle Handle) {
	I2W_OutputData* OutputData = (I2W_OutputData*) Handle;
	if (OutputData == NULL) return 0;
	return OutputData->Height;
}
