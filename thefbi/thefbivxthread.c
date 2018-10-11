#include <errno.h>
#include <unistd.h>

#include "tinydebug.h"
#include "vxplugin.h"
#include "thefbivxthread.h"

Atom script_event_atom;
Display* display;

#define DRAWMODE_COPY 0
#define DRAWMODE_BLEND 1
#define DRAWMODE_ADD 2

#define ONEBYTE  1  
#define TWOBYTES  2
#define THREEBYTES  2
#define FOURBYTES  4

#define SOURCETYPE_RGBA               (FOURBYTES | 0x00)
#define SOURCETYPE_RGBA_PREMULTIPLIED (FOURBYTES | 0x08)
#define SOURCETYPE_INDEX8             (ONEBYTE   | 0x00)
#define SOURCETYPE_ALPHA              (ONEBYTE   | 0x08)
#define SOURCETYPE_RGB_               (FOURBYTES | 0x10)
#define SOURCETYPE_RGB                (THREEBYTES | 0x00)

void CopySpanRGBA(void* src, void* dst, int span) {
	 memcpy(dst,src,span*4);
}

void BlendSpanRGBA(void* src, void* dst, int span) {
	
	uint32_t* SrcWalk =  (uint32_t*) src;
	uint32_t* DstWalk =  (uint32_t*) dst;
	//todo: write a fast one in a bit
	int i;
	
	for (i= 0; i<span; i++) {
		uint32_t pixel = *SrcWalk++;
		uint32_t bg = *DstWalk;
		uint32_t Alpha = pixel >> 24;
		uint32_t IAlpha = 255-Alpha;
		uint32_t R = ((bg >> 16) & 0xff) *IAlpha;
		uint32_t G = ((bg >> 8) & 0xff) *IAlpha;
		uint32_t B = ((bg >> 0) & 0xff) *IAlpha;

		R += ((pixel >> 16) & 0xff) *Alpha;
		G += ((pixel >> 8) & 0xff) *Alpha;
		B += ((pixel >> 0) & 0xff) *Alpha;
		*DstWalk++ = ((R << 8)& 0xff0000)  | (G & 0x00ff00) | ((B >> 8)& 0x0000ff);
	}
}

void AddSpanPremultipliedRGBA(void* src, void* dst, int span) {
	 memcpy(dst,src,span*4);
}
void CopySpanPremultipliedRGBA(void* src, void* dst, int span) {
	 memcpy(dst,src,span*4);
}

void BlendSpanPremultipliedRGBA(void* src, void* dst, int span) {
	 memcpy(dst,src,span*4);
}

void AddSpanRGBA(void* src, void* dst, int span) {
	 memcpy(dst,src,span*4);
}

static void CopySpanData(VXThreadData* VXThread, void* src,void* dest, int span, int DrawMode, int SourceType) {
  debugf("src= $%x,  dest= $%x,  span= %i\n", src,dest,span);
  switch (SourceType) {
	  case SOURCETYPE_RGBA: {
		 switch (DrawMode) {
			 case DRAWMODE_COPY:  CopySpanRGBA(src,dest,span); break;
			 case DRAWMODE_BLEND: BlendSpanRGBA(src,dest,span); break;
			 case DRAWMODE_ADD:   AddSpanRGBA(src,dest,span); break;
		 } 
	  } break;
	  case SOURCETYPE_RGBA_PREMULTIPLIED: {
		 switch (DrawMode) {
			 case DRAWMODE_COPY:  CopySpanPremultipliedRGBA(src,dest,span); break;
			 case DRAWMODE_BLEND: BlendSpanPremultipliedRGBA(src,dest,span); break;
			 case DRAWMODE_ADD:   AddSpanPremultipliedRGBA(src,dest,span); break;
		 }
	  } break;
  }
}
static void dumpregs(struct vxproc *p)
{
	struct vxcpu *c = p->cpu;

	debugf("eax %08x  ecx %08x  edx %08x  ebx %08x\n",
		c->reg[EAX], c->reg[ECX], c->reg[EDX], c->reg[EBX]);
	debugf( "esp %08x  ebp %08x  esi %08x  edi %08x\n",
		c->reg[ESP], c->reg[EBP], c->reg[ESI], c->reg[EDI]);
	debugf( "eip %08x  eflags %08x\n",
		c->eip, c->eflags);

//	for (int i = 0; i < 8; i++) {
//		int32_t *val = r.xmm[i].i32;
//		debugf("xmm%d %08x%08x%08x%08x\n",
//			i, val[3], val[2], val[1], val[0]);
//	}
}

static void dosyscall(VXThreadData* VXThread)
{
	vxproc* proc = VXThread->VX;
	vxmmap* m = vxmem_map(proc->mem,0);
	PluginInfo* Info = VXThread->Info;
	
	switch (proc->cpu->reg[EAX]) {
		case 0x01:  { 
			debug_String("VX Thread Terminating by program request\n");	
			for(;;) sleep(1000);			
			break;
		}
		case 0x02: {
			debug_Line("Waiting for something to happen");
			int waitflags = proc->cpu->reg[EBX];
			do {
				sem_wait(&(VXThread->Communication->InputStateUpdated));
				//debugf("shtate chane flags: %x \n",VXThread->Info->StateChangeFlags);
			} while ( (VXThread->Info->StateChangeFlags & waitflags) == 0);  				
			proc->cpu->reg[EAX]=VXThread->Info->StateChangeFlags; 
			break;
		}
		case 0x03: {
			//debugf("Call Some Script window id: $%x\n",VXThread->Info->window);
			char* scriptcopy = VXThread->Communication->Buffer;
			
			strncpy(scriptcopy,(char*)m->base+proc->cpu->reg[EBX],sizeof(VXThread->Communication->Buffer));
			debug_Line(scriptcopy);
			XEvent ScriptEvent;
			ScriptEvent.type = ClientMessage;
			ScriptEvent.xclient.type = ClientMessage;
			ScriptEvent.xclient.message_type = script_event_atom;
			ScriptEvent.xclient.format=32;
			ScriptEvent.xclient.window=VXThread->Info->window;
			ScriptEvent.xclient.display=display;
			 
			XSendEvent(display,VXThread->Info->window,0,0,&ScriptEvent);
			XFlush(display);
			sem_wait(&(VXThread->Communication->InputStateUpdated));
			break;
		}			
		case 0x04: {
			debugf( "\nOutput_String(%x)\n",(char*)m->base+proc->cpu->reg[ECX]);
			debug_String((char*)m->base+proc->cpu->reg[ECX]);
			break;
		}
		case 0x05: {
			proc->cpu->reg[EAX]=VXThread->Info->MouseX;
			proc->cpu->reg[EBX]=VXThread->Info->MouseY;			
			break;
		}
		case 0x19: {
			//debugf("plot (%i,%i,%#x)\n",proc->cpu->reg[EBX],proc->cpu->reg[ECX],proc->cpu->reg[EDX] );
			unsigned long *pos = (unsigned long *) (VXThread->FrameBufferSurface);
			//debug_String("got surface");
			int width =Info->FrameBufferWidth;
			int height =Info->FrameBufferHeight;
			int X = proc->cpu->reg[EBX];
			int Y = proc->cpu->reg[ECX];
			if ( (X < 0) || (Y < 0 ) || (width <= X) || (height <= Y) ) break; 
			//debugf("\nSurface width %i\n",width);
			pos+= width * Y;
			pos+= X;
			//debug_String("write");
			*pos=proc->cpu->reg[EDX];
			//debug_String("written");
			VXThread->Communication->RedrawRequired = true;
			break;
		}
		case 0x1a: {
			//debugf("Span to FrameBuffer ...");
			//ebx = X  edi = Y  dl = drawmode dh = sourcetype
			//esi = span source start  ecx = span length
			debugf("SpanToFrameBuffer(%i,%i,$%x,%i,%i,%i)\n",
				proc->cpu->reg[EBX],
				proc->cpu->reg[EDI],
				proc->cpu->reg[ESI],
				proc->cpu->reg[ECX],
				proc->cpu->reg[EDX] & 0xff, (proc->cpu->reg[EDX] >> 8) & 0xff
				);
			int span = proc->cpu->reg[ECX];
			if (span <= 0) break;		
			
			//verify source range
			void* src = (char*)m->base+proc->cpu->reg[ESI];
			//void* src = validrange(proc->cpu->reg[ESI],span*4);
			if (src == NULL) break;
			
			//verify dest range
			int width =Info->FrameBufferWidth;
			int height =Info->FrameBufferHeight;
			int X = proc->cpu->reg[EBX];
			int Y = proc->cpu->reg[EDI];
			//debugf("X=%i    y=%i    width=%i   height=%i    span=%i   ",X,Y,width,height,span );

			if ( (X < 0) || (Y < 0 ) || (width <= (X+span)) || (height <= Y) ) break; 
			
			unsigned long *dest = (unsigned long *) (VXThread->FrameBufferSurface);
			dest+= width * Y;
			dest+= X;
			
			CopySpanData(VXThread,src,dest,span,proc->cpu->reg[EDX] & 0xff,(proc->cpu->reg[EDX] >> 8) & 0xff);
			VXThread->Communication->RedrawRequired = true;
			
			break;
		}
		case 0x1b: {
			debugf("Span to memory");
			//ecx = span length dl = drawmode dh = sourcetype
			//esi = span source start  edi=span dest Start

			int span = proc->cpu->reg[ECX];
			if (span <= 0) break;		

			void* src = (char*)m->base+proc->cpu->reg[ESI];
			//void* src = validrange(proc->cpu->reg[ESI],span*4);
			if (src == NULL) break;

			void* dest = (char*)m->base+proc->cpu->reg[EDI];
			//void* dest = validrange(proc->cpu->reg[EDI],span*4);
			if (dest == NULL) break;

			CopySpanData(VXThread, src,dest,span,proc->cpu->reg[EDX] & 0xff,(proc->cpu->reg[EDX] >> 8) & 0xff);
			break;
		}
		case 0xff: {
			dumpregs(proc);
		}
		break;
	}
}

void* ThefbiVXProcessHandler(VXThreadData* VXThread) {
	debug_String("\n**VX Thread Started**\n");

	display = XOpenDisplay(NULL);
	script_event_atom = XInternAtom(display,"Javascript_ClientMessage",false);
	
	debug_String("got script_event_atom*\n");
	
	//wait for first event before going
	sem_wait(&(VXThread->Communication->InputStateUpdated));
	
	vxproc* p = VXThread->VX;
	for (;;) {

		//debug_String("VX Run\n");
		int rc = vxproc_run(p);
		//debugf("VX Run'd  %#x\n", rc);
		//dumpregs(p);

		if (rc < 0){
			debug_String("\n!!!BAD!!!\n");
			dumpregs(p);
			debugf("vxproc_run: %s\n", strerror(errno));
			break;
			
		}
			
		if (rc == VXTRAP_SYSCALL) {
		  //debugf("syscall %#x\n",p->cpu->reg[EAX]); 
			dosyscall(VXThread);
			continue;
		}
		if (rc == VXTRAP_SOFT + 0x80) {
		  //debugf("int 0x80 syscall %#x\n",p->cpu->reg[EAX]); 
			dosyscall(VXThread);
			continue;
		}
		//dumpregs(p);
		debugf("BAD!! :vxproc_run trap %#x\n", rc);
		dumpregs(p);
		break;
	}

	XCloseDisplay(display);
	debug_String("VX Thread Terminating");

	return NULL;
}
