#pragma once

// TestCtrl.h : Declaration of the CTestCtrl ActiveX Control class.

// CTestCtrl : See TestCtrl.cpp for implementation.

class CTestCtrl : public COleControl
{
	DECLARE_DYNCREATE(CTestCtrl)

// Constructor
public:
	CTestCtrl();

// Overrides
public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();

// Implementation
protected:
	~CTestCtrl();

	DECLARE_OLECREATE_EX(CTestCtrl)    // Class factory and guid
	DECLARE_OLETYPELIB(CTestCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CTestCtrl)     // Property page IDs
	DECLARE_OLECTLTYPE(CTestCtrl)		// Type name and misc status

// Message maps
	DECLARE_MESSAGE_MAP()

// Dispatch maps
	DECLARE_DISPATCH_MAP()

	afx_msg void AboutBox();

// Event maps
	DECLARE_EVENT_MAP()

// Dispatch and event IDs
public:
	enum {
		eventidTestAlert = 1L
	};
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

protected:

	void TestAlert(LPCTSTR message)
	{
		FireEvent(eventidTestAlert, EVENT_PARAM(VTS_BSTR), message);
	}
public:
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void DoMessageBox(LPCSTR message);
};

