#ifdef TINYDEBUG

#include <stdio.h>
#include <stdarg.h>
#include <errno.h>

int debugindentlevel;
void debug_Indent() {
	debugindentlevel +=1;
}

void debug_Outdent(){
	debugindentlevel -=1;
	if (debugindentlevel <0 ) debugindentlevel = 0;
}

void debug_OutputString(char* s) {
	FILE* outfile = fopen ("slog.txt","a");
	if (outfile == NULL) return;
	fputs(s,outfile);
	fclose(outfile);
}

void debug_OutputEnter(char* s) {
	int i;
	FILE* outfile = fopen ("slog.txt","a");
	if (outfile == NULL) return;
	for (i=0;i<debugindentlevel;i++) fputs("    ",outfile);
	fputs("Entering ",outfile);
	fputs(s,outfile);
	fputs("\n",outfile);
	fclose(outfile);
	debug_Indent();
}

void debug_OutputLeave(char* s) {
	int i;
	FILE* outfile;
	debug_Outdent();
	outfile = fopen ("slog.txt","a");
	if (outfile == NULL) return;
	for (i=0;i<debugindentlevel;i++) fputs("    ",outfile);
	fputs("Leaving ",outfile);
	fputs(s,outfile);
	fputs("\n",outfile);
	fclose(outfile);
}

void debug_OutputLine(char* s) {
	int i;
	FILE* outfile = fopen ("slog.txt","a");
	if (outfile == NULL) return;
	for (i=0;i<debugindentlevel;i++) fputs("    ",outfile);
	fputs(s,outfile);
	fputs("\n",outfile);
	fclose(outfile);
}

void debugf(const char * format, ...) {
	va_list args;
	va_start(args,format);
	char buffer [65536]; //why not indead
	vsprintf(buffer,format,args);
	va_end(args);
	debug_OutputString(buffer);
}
#else
void debugf(const char * format, ...) {/*nothing*/}
#endif


