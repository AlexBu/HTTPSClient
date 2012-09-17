
// HTTPSWebClientDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "HTTPSWebClient.h"
#include "HTTPSWebClientDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CHTTPSWebClientDlg 对话框




CHTTPSWebClientDlg::CHTTPSWebClientDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHTTPSWebClientDlg::IDD, pParent)
	, URLString(_T(""))
	, POSTString(_T(""))
	, GETString(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CHTTPSWebClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_URL, URLString);
	DDX_Text(pDX, IDC_EDIT_POST, POSTString);
	DDX_Text(pDX, IDC_EDIT_GET, GETString);
}

BEGIN_MESSAGE_MAP(CHTTPSWebClientDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_CONNECT, &CHTTPSWebClientDlg::OnConnect)
END_MESSAGE_MAP()


// CHTTPSWebClientDlg 消息处理程序

BOOL CHTTPSWebClientDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CHTTPSWebClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
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

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CHTTPSWebClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CHTTPSWebClientDlg::OnConnect()
{
	Test_SetURL();

	// check URL and try to connect
	UpdateData(TRUE);

	CheckURL();

	theApp.ConnectToURL(URLString);
}

void CHTTPSWebClientDlg::CheckURL()
{
	return;
}

void CHTTPSWebClientDlg::Test_SetURL()
{
	//test: set URL to 12306
	//URLString = L"www.12306.cn/mormhweb/myfw/";
	URLString = L"www.12306.cn";
	UpdateData(FALSE);
}
