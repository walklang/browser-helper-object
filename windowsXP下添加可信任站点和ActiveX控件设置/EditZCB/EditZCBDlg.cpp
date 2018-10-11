// EditZCBDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "EditZCB.h"
#include "EditZCBDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CEditZCBDlg 对话框



CEditZCBDlg::CEditZCBDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEditZCBDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CEditZCBDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_ip);
}

BEGIN_MESSAGE_MAP(CEditZCBDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
END_MESSAGE_MAP()


// CEditZCBDlg 消息处理程序

BOOL CEditZCBDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将\“关于...\”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO：在此添加额外的初始化代码
	this->SetWindowText("添加可信任站点");
	
	return TRUE;  // 除非设置了控件的焦点，否则返回 TRUE
}

void CEditZCBDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CEditZCBDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
HCURSOR CEditZCBDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CEditZCBDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CString buf_ip;
	m_ip.GetWindowText(buf_ip);
	if(50 <  strlen(buf_ip)) 
	{
		AfxMessageBox("输入的网址格式不正确");
		return ;
	}
	char buf[100];
	memset(buf,0,100);
	memcpy(buf,buf_ip,strlen(buf_ip));
	/*if(0 != memcmp(buf,"http",4))
	{
		AfxMessageBox("输入的网址格式不正确");
		return ;
	}*/
	int i=0;
	int count=0;
	while(buf[i])//找出输入的网址中.的个数，已判断用何种方式修改注册表
	{
		char mem_c='.';
		if(memcmp(&buf[i],&mem_c,1)==0) count++;
		i++;
	}
	char buf_item1[100];//存放子项名称
	memset(buf_item1,0,100);
	char buf_item2[100];//存放子项名称
	memset(buf_item2,0,100);
	char buf_value[100];//值的名称
	memset(buf_value,0,100);
	if(2 == count)
	{
		if(3 != sscanf(buf,"%4s://%3s.%s",buf_value,buf_item1,buf_item2))
		{
			AfxMessageBox("输入的网址格式不正确");
			return ;
		}
		HKEY Hkey;
		char itemname[256];
		memset(itemname,0,256);
		sprintf(itemname,"%s\\%s","Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings\\ZoneMap\\Domains",buf_item2);
		if( ERROR_SUCCESS != RegCreateKey(HKEY_CURRENT_USER,itemname,&Hkey))
		{
			char errorbuf[256];
			memset(errorbuf,0,256);
			sprintf(errorbuf,"创建子项%s失败",itemname);
			AfxMessageBox(errorbuf);
			return ;
		}
		RegCloseKey(Hkey);
		memset(itemname,0,256);
		sprintf(itemname,"%s\\%s\\%s","Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings\\ZoneMap\\Domains",buf_item2,buf_item1);
		if( ERROR_SUCCESS != RegCreateKey(HKEY_CURRENT_USER,itemname,&Hkey))
		{
			char errorbuf[256];
			memset(errorbuf,0,256);
			sprintf(errorbuf,"创建子项%s失败",itemname);
			AfxMessageBox(errorbuf);
			return ;
		}
		//RegCloseKey(Hkey);
		DWORD valuedata=2;
		if( ERROR_SUCCESS != RegSetValueEx(Hkey,buf_value,0,REG_DWORD,(BYTE *)&valuedata,sizeof(DWORD)) )
		{
			char errorbuf[256];
			memset(errorbuf,0,256);
			sprintf(errorbuf,"创建值项%s失败",buf_value);
			AfxMessageBox(errorbuf);
			return ;
		}
		RegCloseKey(Hkey);
	}
	else if(3 == count)
	{
		if(2 != sscanf(buf,"%4s://%s",buf_value,buf_item1))
		{
			AfxMessageBox("输入的网址格式不正确");
			return ;
		}
		HKEY Hkey;
		if( ERROR_SUCCESS != RegCreateKey(HKEY_CURRENT_USER,"Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings\\ZoneMap\\Ranges\\Range100",&Hkey))
		{
			char errorbuf[256];
			memset(errorbuf,0,256);
			sprintf(errorbuf,"创建子项%s失败","Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings\\ZoneMap\\Ranges\\Range100");
			AfxMessageBox(errorbuf);
			return ;
		}
		DWORD valuedata=2;
		if( ERROR_SUCCESS != RegSetValueEx(Hkey,buf_value,0,REG_DWORD,(BYTE *)&valuedata,sizeof(DWORD)) )
		{
			char errorbuf[256];
			memset(errorbuf,0,256);
			sprintf(errorbuf,"创建值项%s失败",buf_value);
			AfxMessageBox(errorbuf);
			return ;
		}
		if( ERROR_SUCCESS != RegSetValueEx(Hkey,":Range",0,REG_SZ,(BYTE *)buf_item1,strlen(buf_item1)))
		{
			char errorbuf[256];
			memset(errorbuf,0,256);
			sprintf(errorbuf,"创建值项%s失败",buf_item1);
			AfxMessageBox(errorbuf);
			return ;
		}
		RegCloseKey(Hkey);
	}
	else
	{
		AfxMessageBox("输入的网址格式不正确");
		return ;
	}

	HKEY Hkey;
	if( ERROR_SUCCESS != RegOpenKey(HKEY_CURRENT_USER,"Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings\\Zones\\2",&Hkey))
	{
		char errorbuf[256];
		memset(errorbuf,0,256);
		sprintf(errorbuf,"打开子项%s失败","Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings\\Zones\\2");
		AfxMessageBox(errorbuf);
		return ;
	}
	DWORD valuedata=1;
	if( ERROR_SUCCESS != RegSetValueEx(Hkey,"1001",0,REG_DWORD,(BYTE *)&valuedata,sizeof(DWORD)) )
	{
		AfxMessageBox("设置ActiveX控件失败");
		return ;
	}
	if( ERROR_SUCCESS != RegSetValueEx(Hkey,"1004",0,REG_DWORD,(BYTE *)&valuedata,sizeof(DWORD)) )
	{
		AfxMessageBox("设置ActiveX控件失败");
		return ;
	}
	if( ERROR_SUCCESS != RegSetValueEx(Hkey,"1200",0,REG_DWORD,(BYTE *)&valuedata,sizeof(DWORD)) )
	{
		AfxMessageBox("设置ActiveX控件失败");
		return ;
	}
	if( ERROR_SUCCESS != RegSetValueEx(Hkey,"1201",0,REG_DWORD,(BYTE *)&valuedata,sizeof(DWORD)) )
	{
		AfxMessageBox("设置ActiveX控件失败");
		return ;
	}
	if( ERROR_SUCCESS != RegSetValueEx(Hkey,"1405",0,REG_DWORD,(BYTE *)&valuedata,sizeof(DWORD)) )
	{
		AfxMessageBox("设置ActiveX控件失败");
		return ;
	}
	if( ERROR_SUCCESS != RegSetValueEx(Hkey,"2201",0,REG_DWORD,(BYTE *)&valuedata,sizeof(DWORD)) )
	{
		AfxMessageBox("设置ActiveX控件失败");
		return ;
	}
	RegCloseKey(Hkey);
	AfxMessageBox("添加成功");
	return ;
}
