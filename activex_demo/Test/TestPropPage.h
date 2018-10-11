#pragma once

// TestPropPage.h : Declaration of the CTestPropPage property page class.


// CTestPropPage : See TestPropPage.cpp for implementation.

class CTestPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CTestPropPage)
	DECLARE_OLECREATE_EX(CTestPropPage)

// Constructor
public:
	CTestPropPage();

// Dialog Data
	enum { IDD = IDD_PROPPAGE_TEST };

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Message maps
protected:
	DECLARE_MESSAGE_MAP()
};

