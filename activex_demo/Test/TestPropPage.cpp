// TestPropPage.cpp : Implementation of the CTestPropPage property page class.

#include "stdafx.h"
#include "Test.h"
#include "TestPropPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CTestPropPage, COlePropertyPage)



// Message map

BEGIN_MESSAGE_MAP(CTestPropPage, COlePropertyPage)
END_MESSAGE_MAP()



// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CTestPropPage, "TEST.TestPropPage.1",
	0x5b1f6fe2, 0x448a, 0x46ef, 0x87, 0x92, 0xcc, 0x5b, 0x27, 0x6d, 0xaf, 0x53)



// CTestPropPage::CTestPropPageFactory::UpdateRegistry -
// Adds or removes system registry entries for CTestPropPage

BOOL CTestPropPage::CTestPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_TEST_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}



// CTestPropPage::CTestPropPage - Constructor

CTestPropPage::CTestPropPage() :
	COlePropertyPage(IDD, IDS_TEST_PPG_CAPTION)
{
}



// CTestPropPage::DoDataExchange - Moves data between page and properties

void CTestPropPage::DoDataExchange(CDataExchange* pDX)
{
	DDP_PostProcessing(pDX);
}



// CTestPropPage message handlers
