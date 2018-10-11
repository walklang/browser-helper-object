#if !defined(AFX_ICCARDPPG_H__1F3829AC_0A99_4442_8D4B_7F52B3A1EA12__INCLUDED_)
#define AFX_ICCARDPPG_H__1F3829AC_0A99_4442_8D4B_7F52B3A1EA12__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// ICCardPpg.h : Declaration of the CICCardPropPage property page class.

////////////////////////////////////////////////////////////////////////////
// CICCardPropPage : See ICCardPpg.cpp.cpp for implementation.

class CICCardPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CICCardPropPage)
	DECLARE_OLECREATE_EX(CICCardPropPage)

// Constructor
public:
	CICCardPropPage();

// Dialog Data
	//{{AFX_DATA(CICCardPropPage)
	enum { IDD = IDD_PROPPAGE_ICCARD };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Message maps
protected:
	//{{AFX_MSG(CICCardPropPage)
		// NOTE - ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ICCARDPPG_H__1F3829AC_0A99_4442_8D4B_7F52B3A1EA12__INCLUDED)
