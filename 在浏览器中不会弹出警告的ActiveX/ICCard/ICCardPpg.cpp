// ICCardPpg.cpp : Implementation of the CICCardPropPage property page class.

#include "stdafx.h"
#include "ICCard.h"
#include "ICCardPpg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CICCardPropPage, COlePropertyPage)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CICCardPropPage, COlePropertyPage)
	//{{AFX_MSG_MAP(CICCardPropPage)
	// NOTE - ClassWizard will add and remove message map entries
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CICCardPropPage, "ICCARD.ICCardPropPage.1",
	0x79ab5292, 0xcff9, 0x4f74, 0x99, 0x8f, 0xec, 0x15, 0x58, 0xe7, 0x8f, 0x66)


/////////////////////////////////////////////////////////////////////////////
// CICCardPropPage::CICCardPropPageFactory::UpdateRegistry -
// Adds or removes system registry entries for CICCardPropPage

BOOL CICCardPropPage::CICCardPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_ICCARD_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// CICCardPropPage::CICCardPropPage - Constructor

CICCardPropPage::CICCardPropPage() :
	COlePropertyPage(IDD, IDS_ICCARD_PPG_CAPTION)
{
	//{{AFX_DATA_INIT(CICCardPropPage)
	// NOTE: ClassWizard will add member initialization here
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA_INIT
}


/////////////////////////////////////////////////////////////////////////////
// CICCardPropPage::DoDataExchange - Moves data between page and properties

void CICCardPropPage::DoDataExchange(CDataExchange* pDX)
{
	//{{AFX_DATA_MAP(CICCardPropPage)
	// NOTE: ClassWizard will add DDP, DDX, and DDV calls here
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA_MAP
	DDP_PostProcessing(pDX);
}


/////////////////////////////////////////////////////////////////////////////
// CICCardPropPage message handlers
