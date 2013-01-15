
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
	, RespondString(_T(""))
	, AddrString(_T(""))
	, usernameStr(_T(""))
	, passwordStr(_T(""))
	, validateStr(_T(""))
	, dateString(_T(""))
	, nameP1(_T(""))
	, identityTypeP1(_T(""))
	, identityNoP1(_T(""))
	, mobileP1(_T(""))
	, seatTypeP1(_T(""))
	, nameP2(_T(""))
	, identityTypeP2(_T(""))
	, identityNoP2(_T(""))
	, mobileP2(_T(""))
	, seatTypeP2(_T(""))
	, trainNo(_T(""))
	, stationFrom(_T(""))
	, stationTo(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CHTTPSWebClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_GET, RespondString);
	DDX_Text(pDX, IDC_EDIT_VALIPIC, validateStr);
	DDX_Control(pDX, IDC_STATIC_VALIDATE_PIC_LOGIN, valPicLoginCtrl);
	DDX_Control(pDX, IDC_STATIC_VALIDATE_PIC_BOOK, valPicBookCtrl);
	DDX_Text(pDX, IDC_EDIT_DATE, dateString);
	DDX_Text(pDX, IDC_EDIT_USERNAME, usernameStr);
	DDX_Text(pDX, IDC_EDIT_PASSWORD, passwordStr);
	DDX_Text(pDX, IDC_EDIT_NAME_P1, nameP1);
	DDX_Text(pDX, IDC_EDIT_IDTYP_P1, identityTypeP1);
	DDX_Text(pDX, IDC_EDIT_IDNO_P1, identityNoP1);
	DDX_Text(pDX, IDC_EDIT_MOB_P1, mobileP1);
	DDX_Text(pDX, IDC_EDIT_SEATTYP_P1, seatTypeP1);
	DDX_Text(pDX, IDC_EDIT_NAME_P2, nameP2);
	DDX_Text(pDX, IDC_EDIT_IDTYP_P2, identityTypeP2);
	DDX_Text(pDX, IDC_EDIT_IDNO_P2, identityNoP2);
	DDX_Text(pDX, IDC_EDIT_MOB_P2, mobileP2);
	DDX_Text(pDX, IDC_EDIT_SEATTYP_P2, seatTypeP2);
	DDX_Text(pDX, IDC_EDIT_TRAIN, trainNo);
	DDX_Text(pDX, IDC_EDIT_STATION_FROM, stationFrom);
	DDX_Text(pDX, IDC_EDIT_STATION_TO, stationTo);
	DDX_Text(pDX, IDC_EDIT_VAL_PIC_BOOK, validateBookStr);
}

BEGIN_MESSAGE_MAP(CHTTPSWebClientDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_GET_VALPIC, &CHTTPSWebClientDlg::OnGetValLoginPic)
	ON_BN_CLICKED(IDC_BUTTON_LOGIN, &CHTTPSWebClientDlg::OnLogin)
	ON_BN_CLICKED(IDC_BUTTON_GET_VALPIC2, &CHTTPSWebClientDlg::OnGetValBookPic)
	ON_BN_CLICKED(IDC_BUTTON_BOOK, &CHTTPSWebClientDlg::OnBook)
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
	InitUserInfo();
	InitPassengerInfo();

	UpdateData(FALSE);

	// connect to site
	ConnectToSite();

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

void CHTTPSWebClientDlg::ConnectToSite()
{
	AddrString = _T("dynamic.12306.cn");
	
	theApp.ConnectToURL(AddrString);
}

void CHTTPSWebClientDlg::OnGetValLoginPic()
{
	CString ValPicAddr = L"/otsweb/passCodeAction.do?rand=sjrand";
	if(theApp.GetValidatePic(ValPicAddr, valPicLoginCtrl))
	{
		// redraw picture
		valPicLoginCtrl.InvalidateRect(NULL, TRUE);
	}
}

void CHTTPSWebClientDlg::OnLogin()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	// fill up passengers info
	GetPassengerInfo();
	theApp.LoginToSite(usernameStr, passwordStr, validateStr, RespondString);
	// set response
	UpdateData(FALSE);
}

void CHTTPSWebClientDlg::OnGetValBookPic()
{
	// TODO: Add your control notification handler code here
	CString ValPicAddr = L"/otsweb/passCodeAction.do?rand=randp";
	if(theApp.GetValidatePic(ValPicAddr, valPicBookCtrl))
	{
		// redraw picture
		valPicBookCtrl.InvalidateRect(NULL, TRUE);
	}
}

void CHTTPSWebClientDlg::OnBook()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	theApp.QueryTickets(dateString, trainNo, stationFrom, stationTo, RespondString, validateBookStr);
	// set response
	UpdateData(FALSE);
}

void CHTTPSWebClientDlg::InitUserInfo()
{
	usernameStr = L"bkp84335";
	passwordStr = L"bsp2236";

	dateString= L"2013-01-17";
	trainNo = L"D305";
}

void CHTTPSWebClientDlg::InitPassengerInfo()
{
	nameP1 = L"卜昆鹏";
	nameP2 = L"喻娟";
	identityTypeP1 = L"1";	// 1 for identify card
	identityTypeP2 = L"1";
	identityNoP1 = L"32032119831230701X";
	identityNoP2 = L"43012419860309496X";
	mobileP1 = L"18912952107";
	mobileP2 = L"18912952107";
	seatTypeP1 = L"O";	// big O for second class seat
	seatTypeP2 = L"O";
}

void CHTTPSWebClientDlg::GetPassengerInfo()
{
	theApp.passengerInfo[0].seat = seatTypeP1;
	theApp.passengerInfo[0].name = nameP1;
	theApp.passengerInfo[0].cardtype = identityTypeP1;
	theApp.passengerInfo[0].cardno = identityNoP1;
	theApp.passengerInfo[0].mobileno = mobileP1;

	theApp.passengerInfo[1].seat = seatTypeP2;
	theApp.passengerInfo[1].name = nameP2;
	theApp.passengerInfo[1].cardtype = identityTypeP2;
	theApp.passengerInfo[1].cardno = identityNoP2;
	theApp.passengerInfo[1].mobileno = mobileP2;
}