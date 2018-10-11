#ifndef npfrm_h_
#define npfrm_h_

#include "mozilla-config.h"

#include "npapi.h"
#include "npfunctions.h"
#include "npruntime.h"
//#include "prtypes.h"

#ifdef XP_WIN
#include "windows.h"
#include "windowsx.h"
#endif

#ifdef XP_UNIX
#include <stdio.h>
#endif

#ifdef XP_MAC
#include <Carbon/Carbon.h>
#endif

#ifndef ARRAY_LENGTH
#define ARRAY_LENGTH(a) (sizeof(a)/sizeof(a[0]))
#endif

#ifndef HIBYTE
#define HIBYTE(i) (i >> 8)
#endif

#ifndef LOBYTE
#define LOBYTE(i) (i & 0xff)
#endif

// #ifndef SCRIPT
// #define SCRIPT
// #endif

#endif // npfrm_h_
