/*
* Copyright (c) 2003,GDC
* All rights reserved.
*
* 文件名称：ICCardCtl.C
* 文件标识：见配置管理计划书
* 摘要：读IC卡OCX控件的类实现文件
*
* 当前版本：0.1 beta
* 作者：王正平
* 完成日期：2003年06月10日
*/
#include "stdafx.h"
#include "ICCard.h"
#include "ICCardCtl.h"
#include "ICCardPpg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// 调用动态链接库用,放在共享内存段
#pragma data_seg("mydata") 
static HINSTANCE DLLInst = NULL; //动态库句柄
#pragma data_seg()
// 动态链接库引出函数定义
typedef  int (__stdcall READ_HOST_S)(int ComPort, char* buf);
READ_HOST_S* Read_Host_S;

IMPLEMENT_DYNCREATE(CICCardCtrl, COleControl)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CICCardCtrl, COleControl)
	//{{AFX_MSG_MAP(CICCardCtrl)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Dispatch map

BEGIN_DISPATCH_MAP(CICCardCtrl, COleControl)
	//{{AFX_DISPATCH_MAP(CICCardCtrl)
	DISP_PROPERTY_NOTIFY(CICCardCtrl, "ComPort", m_comPort, OnComPortChanged, VT_I2)
	DISP_FUNCTION(CICCardCtrl, "ReadICCard", ReadICCard, VT_BSTR, VTS_NONE)
	DISP_STOCKPROP_ENABLED()
	//}}AFX_DISPATCH_MAP
	DISP_FUNCTION_ID(CICCardCtrl, "AboutBox", DISPID_ABOUTBOX, AboutBox, VT_EMPTY, VTS_NONE)
END_DISPATCH_MAP()


/////////////////////////////////////////////////////////////////////////////
// Event map

BEGIN_EVENT_MAP(CICCardCtrl, COleControl)
	//{{AFX_EVENT_MAP(CICCardCtrl)
	//}}AFX_EVENT_MAP
END_EVENT_MAP()


/////////////////////////////////////////////////////////////////////////////
// Property pages

BEGIN_PROPPAGEIDS(CICCardCtrl, 1)
	PROPPAGEID(CICCardPropPage::guid)
END_PROPPAGEIDS(CICCardCtrl)


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CICCardCtrl, "ICCARD.ICCardCtrl.1",
	0x7ae7497b, 0xcad8, 0x4e66, 0xa5, 0x8b, 0xdd, 0xe9, 0xbc, 0xaf, 0x6b, 0x61)


/////////////////////////////////////////////////////////////////////////////
// Type library ID and version

IMPLEMENT_OLETYPELIB(CICCardCtrl, _tlid, _wVerMajor, _wVerMinor)


/////////////////////////////////////////////////////////////////////////////
// Interface IDs

const IID BASED_CODE IID_DICCard =
		{ 0x7ee8b159, 0x980f, 0x4380, { 0x84, 0x5a, 0x28, 0xd7, 0xd, 0x1b, 0xf7, 0x5c } };
const IID BASED_CODE IID_DICCardEvents =
		{ 0xcef07ba8, 0x3493, 0x47f6, { 0x8e, 0x24, 0x24, 0xda, 0x79, 0x14, 0xa0, 0x84 } };


/////////////////////////////////////////////////////////////////////////////
// Control type information

static const DWORD BASED_CODE _dwICCardOleMisc =
	OLEMISC_INVISIBLEATRUNTIME |
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CICCardCtrl, IDS_ICCARD, _dwICCardOleMisc)


/////////////////////////////////////////////////////////////////////////////
// CICCardCtrl::CICCardCtrlFactory::UpdateRegistry -
// Adds or removes system registry entries for CICCardCtrl

BOOL CICCardCtrl::CICCardCtrlFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterControlClass(
			AfxGetInstanceHandle(),
			m_clsid,
			m_lpszProgID,
			IDS_ICCARD,
			IDB_ICCARD,
			afxRegApartmentThreading,
			_dwICCardOleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}


/////////////////////////////////////////////////////////////////////////////
// CICCardCtrl::CICCardCtrl - Constructor

CICCardCtrl::CICCardCtrl()
{
	InitializeIIDs(&IID_DICCard, &IID_DICCardEvents);  
	// 设置控件尺寸
	SetInitialSize(24, 22);
	// 串口端口号，默认为0x01
	//m_comPort = DEFAULT_COM_PORT;
	// 加载动态链接库
	LoadDll();
}


/////////////////////////////////////////////////////////////////////////////
// CICCardCtrl::~CICCardCtrl - Destructor

CICCardCtrl::~CICCardCtrl()
{
    // 卸载DLL
	if (DLLInst!=NULL)
		FreeLibrary(DLLInst);
}


/////////////////////////////////////////////////////////////////////////////
// CICCardCtrl::OnDraw - Drawing function

void CICCardCtrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	pdc->FillRect(rcBounds, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));
	pdc->Ellipse(rcBounds);
	pdc->TextOut(rcBounds.left+6,rcBounds.top+4, "IC");
}


/////////////////////////////////////////////////////////////////////////////
// CICCardCtrl::DoPropExchange - Persistence support

void CICCardCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);
	// 持久属性
	//PX_Short(pPX, _T("ComPort"), (short)m_comPort, (short)DEFAULT_COM_PORT);
}


/////////////////////////////////////////////////////////////////////////////
// CICCardCtrl::OnResetState - Reset control to default state

void CICCardCtrl::OnResetState()
{
	COleControl::OnResetState();  // Resets defaults found in DoPropExchange
}


/*****************************************************************
* 函数介绍: 调用关于对话框函数
* 输入参数: 无
* 输出参数: 无
* 返 回 值: 无
*****************************************************************/
void CICCardCtrl::AboutBox()
{
	CDialog dlgAbout(IDD_ABOUTBOX_ICCARD);
	dlgAbout.DoModal();
}

/*****************************************************************
* 函数介绍: 当调用属性Enabled改变时触发
* 输入参数: 无
* 输出参数: 无
* 返 回 值: 无
*****************************************************************/
void CICCardCtrl::OnEnabledChanged() 
{
	COleControl::OnEnabledChanged();
}

/*****************************************************************
* 函数介绍: 当调用SetInitialSize()时触发
* 输入参数: 无
* 输出参数: 无
* 返 回 值: 无
*****************************************************************/
void CICCardCtrl::OnSetClientSite() 
{
	// 引起OnCreate的调用
	RecreateControlWindow();	
	COleControl::OnSetClientSite();
}

/*****************************************************************
* 函数介绍: 类创建函数
* 输入参数: LPCREATESTRUCT lpCreateStruct - 创建参数结构
* 输出参数: 无
* 返 回 值: int                           - 成功返回0,否则-1
*****************************************************************/
int CICCardCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (COleControl::OnCreate(lpCreateStruct) == -1)
		return -1;

	// 加载端口号
	CWinApp* pApp = AfxGetApp();
    m_comPort = pApp->GetProfileInt("ComPort", "Port", DEFAULT_COM_PORT);

	return 0;
}

/*****************************************************************
* 函数介绍: 类销毁函数
* 输入参数: 无
* 输出参数: 无
* 返 回 值: 无
*****************************************************************/
void CICCardCtrl::OnDestroy() 
{
	// 保存端口号
	CWinApp* pApp = AfxGetApp();
    pApp->WriteProfileInt("ComPort", "Port", m_comPort);
	
	COleControl::OnDestroy();
}

/*****************************************************************
* 函数介绍: 读IC卡方法, 他发送一个消息，调用消息函数触发事件
* 输入参数: 无
* 输出参数: 无
* 返 回 值: 读到的串
*****************************************************************/
BSTR CICCardCtrl::ReadICCard() 
{
	CString strResult;
	LPTSTR p;
	p = strResult.GetBuffer(1120);
    int nRet = Read_Host_S(m_comPort, p);
	strResult.ReleaseBuffer();
	// 如果成功
	if (nRet==0)
	{
		return strResult.AllocSysString();
	}
	else
	{
		return NULL;
	}
}

//===================自定义部分=================================//

/*****************************************************************
* 函数介绍: 加载DLL文件，引出函数
* 输入参数: 无
* 输出参数: 无
* 返 回 值: BOOL
*****************************************************************/
BOOL CICCardCtrl::LoadDll()
{
    HRSRC hResInfo;
	HGLOBAL hRes;
	HINSTANCE hInst;
    // 查找DLL资源 
	hInst = AfxGetInstanceHandle();
	hResInfo = FindResource(hInst, (LPCTSTR)IDR_DLL1,"DLL" );
    // 加载资源
	hRes = LoadResource(hInst,hResInfo );
	// 计算DLL文件大小
	DWORD dFileLength = SizeofResource(hInst, hResInfo );
	// 获取系统路径
	char pPath[MAX_PATH];
    GetSystemDirectory(pPath, MAX_PATH);
    // 生成文件名
	strcpy(g_pFile, pPath);
	strcat(g_pFile, "\\HGIC.Dll");
    // 创建DLL文件
	HANDLE hFile = CreateFile(g_pFile, GENERIC_WRITE | GENERIC_READ,
                            FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_NEW, 
                            FILE_ATTRIBUTE_NORMAL,NULL);
    DWORD dwNumberOfBytesWritten = 0;
	// 写DLL文件
    if(hFile > 0)
        WriteFile(hFile, (LPCVOID)hRes, dFileLength, &dwNumberOfBytesWritten,   NULL);
    else
	    MessageBox("加载Res不成功","提示信息",MB_OK);
	// 释放文件句柄
    CloseHandle(hFile);

    // 动态加载DLL
	DLLInst = LoadLibrary(g_pFile);
	// 如果加载成功，找出函数地址
	if (DLLInst!=NULL)
	{
	    Read_Host_S = (READ_HOST_S*)GetProcAddress(DLLInst, "Read_Host_S");
	}
	else
	{
		::MessageBox(NULL,"加载动态库失败！", "提示信息", MB_OK | MB_ICONINFORMATION);
        return false;
	}
    return true; 
}

void CICCardCtrl::OnComPortChanged() 
{
	// TODO: Add notification handler code

	SetModifiedFlag();
}
