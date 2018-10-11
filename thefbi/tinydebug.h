
#ifdef __cplusplus
extern "C" {
#endif

#ifdef TINYDEBUG
void debug_OutputString(const char* s);
void debug_OutputLine(const char* s);
void debug_OutputEnter(const char* s);
void debug_OutputLeave(const char* s);

#define debug_Line(s) debug_OutputLine(s);
#define debug_String(s) debug_OutputString(s);
#define debug_FunctionEnter(s) debug_OutputEnter(s);
#define debug_FunctionLeave(s) debug_OutputLeave(s);

#else
#define debug_Line(s) /*debug_line(s)*/
#define debug_String(s) /*debug_String(s)*/
#define debug_FunctionEnter(s) /*debug_FunctionEnter(s)*/
#define debug_FunctionLeave(s) /*debug_FunctionLeave(s)*/
#endif

void debugf(const char * format, ...);

#ifdef __cplusplus
}  /* end extern "C" */
#endif
