
typedef struct vxthread_struct {
	PluginInfo* Info;
	vxproc* VX;	
        PluginCommunication* Communication;
	void* FrameBufferSurface;	
	void* ElfFile;
	
	//syscall machine state
	//not too sure about this, may come back to bite for multithreading
	void* IndexPalette;
} VXThreadData;

void* ThefbiVXProcessHandler(VXThreadData* VXThread);

void debugf(const char * format, ...);
