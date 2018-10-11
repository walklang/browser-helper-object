// TestCtrl.cpp : Implementation of the CTestCtrl ActiveX Control class.

#include "stdafx.h"
#include "Test.h"
#include "TestCtrl.h"
#include "TestPropPage.h"

#include "afxinet.h"
#include "TestFunction.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CTestCtrl, COleControl)



// Message map

BEGIN_MESSAGE_MAP(CTestCtrl, COleControl)
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
	ON_WM_CREATE()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()



// Dispatch map

BEGIN_DISPATCH_MAP(CTestCtrl, COleControl)
	DISP_FUNCTION_ID(CTestCtrl, "AboutBox", DISPID_ABOUTBOX, AboutBox, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CTestCtrl, "DoMessageBox", DoMessageBox, VT_EMPTY, VTS_BSTR)	//加入这行代码
END_DISPATCH_MAP()



// Event map

BEGIN_EVENT_MAP(CTestCtrl, COleControl)		
	EVENT_CUSTOM_ID("TestAlert", eventidTestAlert, TestAlert, VTS_BSTR)
END_EVENT_MAP()



// Property pages

// TODO: Add more property pages as needed.  Remember to increase the count!
BEGIN_PROPPAGEIDS(CTestCtrl, 1)
	PROPPAGEID(CTestPropPage::guid)
END_PROPPAGEIDS(CTestCtrl)



// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CTestCtrl, "TEST.TestCtrl.1",
	0x1a4b594a, 0xa204, 0x4064, 0x9b, 0x94, 0x2e, 0xcf, 0x1, 0x8b, 0x18, 0xed)



// Type library ID and version

IMPLEMENT_OLETYPELIB(CTestCtrl, _tlid, _wVerMajor, _wVerMinor)



// Interface IDs

const IID BASED_CODE IID_DTest =
		{ 0xE6205E4A, 0x8F4, 0x4699, { 0xA8, 0x6D, 0xDF, 0x36, 0x1D, 0x1A, 0xF1, 0x17 } };
const IID BASED_CODE IID_DTestEvents =
		{ 0x65E0FC1A, 0x7532, 0x45C1, { 0x92, 0x94, 0xF8, 0x30, 0x8A, 0x71, 0xF7, 0x81 } };



// Control type information

static const DWORD BASED_CODE _dwTestOleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CTestCtrl, IDS_TEST, _dwTestOleMisc)



// CTestCtrl::CTestCtrlFactory::UpdateRegistry -
// Adds or removes system registry entries for CTestCtrl

BOOL CTestCtrl::CTestCtrlFactory::UpdateRegistry(BOOL bRegister)
{
	// TODO: Verify that your control follows apartment-model threading rules.
	// Refer to MFC TechNote 64 for more information.
	// If your control does not conform to the apartment-model rules, then
	// you must modify the code below, changing the 6th parameter from
	// afxRegApartmentThreading to 0.

	if (bRegister)
		return AfxOleRegisterControlClass(
			AfxGetInstanceHandle(),
			m_clsid,
			m_lpszProgID,
			IDS_TEST,
			IDB_TEST,
			afxRegApartmentThreading,
			_dwTestOleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}



// CTestCtrl::CTestCtrl - Constructor

CTestCtrl::CTestCtrl()
{
	InitializeIIDs(&IID_DTest, &IID_DTestEvents);
	// TODO: Initialize your control's instance data here.
}



// CTestCtrl::~CTestCtrl - Destructor

CTestCtrl::~CTestCtrl()
{
	// TODO: Cleanup your control's instance data here.
}



// CTestCtrl::OnDraw - Drawing function

void CTestCtrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	if (!pdc)
		return;

	// TODO: Replace the following code with your own drawing code.
	pdc->FillRect(rcBounds, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));
	pdc->Ellipse(rcBounds);
}



// CTestCtrl::DoPropExchange - Persistence support

void CTestCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

	// TODO: Call PX_ functions for each persistent custom property.
}



// CTestCtrl::OnResetState - Reset control to default state

void CTestCtrl::OnResetState()
{
	COleControl::OnResetState();  // Resets defaults found in DoPropExchange

	// TODO: Reset any other control state here.
}



// CTestCtrl::AboutBox - Display an "About" box to the user

void CTestCtrl::AboutBox()
{
	CDialog dlgAbout(IDD_ABOUTBOX_TEST);
	dlgAbout.DoModal();
}



// CTestCtrl message handlers

int CTestCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (COleControl::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO:  Add your specialized creation code here

	return 0;
}

void CTestCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	TestAlert(L"我的名字叫元金三");	//调用Event方法

	CString strRequestHeader;	
	CString strBeforeString;	
	CString strBeforeWrite;
	CString strAfterString;
	CString strAfterWrite;
	CString strEndWrite;

	CFile file;
	CString strFilePath=L"C:\\Users\\Administrator\\Documents\\Downloads\\html002.rar";	//上传的文件

	file.Open(strFilePath,CFile::modeRead);	//只读方式打开

	strRequestHeader=L"Content-Type: multipart/form-data; boundary=---------------------------7d931a31110c46\r\n";	//enctype="multipart/form-data"
	
	//textbox
	strBeforeString=L"-----------------------------7d931a31110c46\r\n";
	strBeforeString+=L"Content-Disposition: form-data; name=\"nameUsername\"\r\n";
	strBeforeString+=L"\r\n";
	strBeforeString+=L"%s\r\n";

	//file textbox
	strBeforeString+=L"-----------------------------7d931a31110c46\r\n";
	strBeforeString+=L"Content-Disposition: form-data; name=\"nameFile\"; filename=\"%s\"\r\n";
	strBeforeString+=L"Content-Type: application/octet-stream\r\n";
	strBeforeString+=L"\r\n";
	
	strBeforeWrite.Format(strBeforeString,L"我的名字叫元金三",strFilePath);	//nameUsername值：我的名字叫元金三，strBeforeString值：文件路经

	//提交按钮
	strAfterString=L"-----------------------------7d931a31110c46\r\n";
	strAfterString+=L"Content-Disposition: form-data; name=\"namePost\"\r\n";
	strAfterString+=L"\r\n";
	strAfterString+=L"%s\r\n";
	
	strAfterWrite.Format(strAfterString,L"Test Post");	//按钮名称

	strEndWrite=L"-----------------------------7d931a31110c46--";	//结束

	CInternetSession internetSession;
	CHttpConnection* pHttpConnection;
	CHttpFile* pHttpFile;	
	
	pHttpConnection=internetSession.GetHttpConnection(L"localhost",0,80,0,0);	//连接位置

	pHttpFile=pHttpConnection->OpenRequest(CHttpConnection::HTTP_VERB_POST,L"/Test/Test.asp");	//打开请求
	pHttpFile->AddRequestHeaders(strRequestHeader);	//报文头
	
	while(TRUE){
		try{
			pHttpFile->SendRequestEx(
				CTestFunction::GetMultiByteLength(strBeforeWrite)+
				CTestFunction::GetMultiByteLength(strAfterWrite)+
				CTestFunction::GetMultiByteLength(strEndWrite)+
				(DWORD)file.GetLength()
				);	//content-length
			pHttpFile->Write(CTestFunction::GetMultiByte(
				(LPCWSTR)strBeforeWrite),
				CTestFunction::GetMultiByteLength(strBeforeWrite)
				);	//流写入
			
			DWORD dwChunkLength;
			DWORD dwReadLength;
			void* pBuffer;

			file.SeekToBegin();

			dwChunkLength=64*1024;				
			pBuffer=malloc(dwChunkLength);
			dwReadLength=-1;
			while(dwReadLength!=0){
				dwReadLength=file.Read(pBuffer,dwChunkLength);
				if(dwReadLength!=0){
					pHttpFile->Write(pBuffer,dwReadLength);	//文件已64K大小分段写入
				}
			}
			delete pBuffer;

			pHttpFile->Write(CTestFunction::GetMultiByte(
				(LPCWSTR)strAfterWrite),
				CTestFunction::GetMultiByteLength(strAfterWrite)
				);	//写入提交按钮
			pHttpFile->Write(CTestFunction::GetMultiByte(
				(LPCWSTR)strEndWrite),
				CTestFunction::GetMultiByteLength(strEndWrite)
				);	//写入结束
			pHttpFile->EndRequest();	//结束请求

			break;
		}catch(CInternetException* ex){
			ex->Delete();

			DWORD dwStatusCode;
			CString username,password;

			username=L"Administrator";
			password=L"yjs.vista";

			pHttpFile->QueryInfoStatusCode(dwStatusCode);
			
			switch(dwStatusCode){
			case HTTP_STATUS_DENIED:
				//当是NTLM时，输入用户名密码认证
				pHttpFile->SetOption(INTERNET_OPTION_USERNAME,(LPVOID)(LPSTR)username.GetBuffer(),username.GetLength());
				pHttpFile->SetOption(INTERNET_OPTION_USERNAME,(LPVOID)(LPSTR)password.GetBuffer(),password.GetLength());

				username.ReleaseBuffer();
				password.ReleaseBuffer();
				continue;
			}
			break;
		}
	}

	CString strResponse=CTestFunction::GetWideCharResponse(pHttpFile);	//取得服务端反馈结果

	MessageBox(strResponse);

	file.Close();
	pHttpFile->Close();
	pHttpConnection->Close();
	internetSession.Close();

	delete pHttpFile;
	delete pHttpConnection;

	COleControl::OnLButtonUp(nFlags, point);
}

void CTestCtrl::DoMessageBox(LPCSTR message)
{
	MessageBox(LPCTSTR(message),0,MB_OK);	//加入这行代码
}
