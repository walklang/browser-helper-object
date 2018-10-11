#if !defined(AFX_ICCARDCTL_H__DB9521C5_89CE_4BDF_9F8B_6971787BCDAA__INCLUDED_)
#define AFX_ICCARDCTL_H__DB9521C5_89CE_4BDF_9F8B_6971787BCDAA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// ICCardCtl.h : Declaration of the CICCardCtrl ActiveX Control class.

/////////////////////////////////////////////////////////////////////////////
// CICCardCtrl : See ICCardCtl.cpp for implementation.
// 默认端口号
#define DEFAULT_COM_PORT 0x01

class CICCardCtrl : public COleControl
{
	DECLARE_DYNCREATE(CICCardCtrl)

// Constructor
public:
	CICCardCtrl();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CICCardCtrl)
	public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();
	virtual void OnEnabledChanged();
	virtual void OnSetClientSite();
	//}}AFX_VIRTUAL

// Implementation
protected:
	~CICCardCtrl();

	DECLARE_OLECREATE_EX(CICCardCtrl)    // Class factory and guid
	DECLARE_OLETYPELIB(CICCardCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CICCardCtrl)     // Property page IDs
	DECLARE_OLECTLTYPE(CICCardCtrl)		// Type name and misc status

// Message maps
	//{{AFX_MSG(CICCardCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg LRESULT OnReadEvent(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// Dispatch maps
	//{{AFX_DISPATCH(CICCardCtrl)
	short m_comPort;
	afx_msg void OnComPortChanged();
	afx_msg BSTR ReadICCard();
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()

	afx_msg void AboutBox();

// Event maps
	//{{AFX_EVENT(CICCardCtrl)
	//}}AFX_EVENT
	DECLARE_EVENT_MAP()

// Dispatch and event IDs
public:
	enum {
	//{{AFX_DISP_ID(CICCardCtrl)
	dispidComPort = 1L,
	dispidReadICCard = 2L,
	//}}AFX_DISP_ID
	};
protected:
	char  g_pFile[MAX_PATH]; // 释放DLL的文件名 
	BOOL LoadDll();          // 加载动态链接库 
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ICCARDCTL_H__DB9521C5_89CE_4BDF_9F8B_6971787BCDAA__INCLUDED)
