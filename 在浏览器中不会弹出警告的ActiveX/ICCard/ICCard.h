#if !defined(AFX_ICCARD_H__C34CEA8F_93F8_4BC3_9129_CEB30A9D1536__INCLUDED_)
#define AFX_ICCARD_H__C34CEA8F_93F8_4BC3_9129_CEB30A9D1536__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// ICCard.h : main header file for ICCARD.DLL

#if !defined( __AFXCTL_H__ )
	#error include 'afxctl.h' before including this file
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CICCardApp : See ICCard.cpp for implementation.
class CICCardApp : public COleControlModule
{
public:
	BOOL InitInstance();
	int ExitInstance();
};

extern const GUID CDECL _tlid;
extern const WORD _wVerMajor;
extern const WORD _wVerMinor;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ICCARD_H__C34CEA8F_93F8_4BC3_9129_CEB30A9D1536__INCLUDED)
