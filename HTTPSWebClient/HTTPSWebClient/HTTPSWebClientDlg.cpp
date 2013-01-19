
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
	, nameP3(_T(""))
	, identityTypeP3(_T(""))
	, identityNoP3(_T(""))
	, mobileP3(_T(""))
	, seatTypeP3(_T(""))
	, nameP4(_T(""))
	, identityTypeP4(_T(""))
	, identityNoP4(_T(""))
	, mobileP4(_T(""))
	, seatTypeP4(_T(""))
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
	DDX_Text(pDX, IDC_EDIT_NAME_P3, nameP3);
	DDX_Text(pDX, IDC_EDIT_IDTYP_P3, identityTypeP3);
	DDX_Text(pDX, IDC_EDIT_IDNO_P3, identityNoP3);
	DDX_Text(pDX, IDC_EDIT_MOB_P3, mobileP3);
	DDX_Text(pDX, IDC_EDIT_SEATTYP_P3, seatTypeP3);
	DDX_Text(pDX, IDC_EDIT_NAME_P4, nameP4);
	DDX_Text(pDX, IDC_EDIT_IDTYP_P4, identityTypeP4);
	DDX_Text(pDX, IDC_EDIT_IDNO_P4, identityNoP4);
	DDX_Text(pDX, IDC_EDIT_MOB_P4, mobileP4);
	DDX_Text(pDX, IDC_EDIT_SEATTYP_P4, seatTypeP4);
	DDX_Text(pDX, IDC_EDIT_TRAIN, trainNo);
	DDX_Text(pDX, IDC_EDIT_STATION_FROM, stationFrom);
	DDX_Text(pDX, IDC_EDIT_STATION_TO, stationTo);
}

BEGIN_MESSAGE_MAP(CHTTPSWebClientDlg, CDialog)
	//AFX_MSG_MAP{{(CHTTPSWebClientDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_SETSTR, CHTTPSWebClientDlg::OnSetStr)
	ON_MESSAGE(WM_FINISH, CHTTPSWebClientDlg::OnFinish)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_LOGIN, &CHTTPSWebClientDlg::OnLogin)
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

void CHTTPSWebClientDlg::OnLogin()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	// fill up user info
	GetUserInfo();
	theApp.LoginToSite(RespondString);
	// set response
	UpdateData(FALSE);
}

void CHTTPSWebClientDlg::OnBook()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	// fill up passengers info
	GetPassengerInfo();
	theApp.BookTickets(RespondString);
	// set response
	UpdateData(FALSE);
}

void CHTTPSWebClientDlg::InitUserInfo()
{
	usernameStr = L"bkp84335";
	passwordStr = L"bsp2236";
}

void CHTTPSWebClientDlg::InitPassengerInfo()
{
	dateString= L"2013-01-20";
	trainNo = L"D305";
	stationFrom = L"南京";
	stationTo = L"上海";

	nameP1 = L"卜昆鹏";
	identityTypeP1 = L"1";	// 1 for identify card
	identityNoP1 = L"32032119831230701X";
	mobileP1 = L"18912952106";
	seatTypeP1 = L"3";	// big O for second class seat

	nameP2 = L"喻娟";
	identityTypeP2 = L"1";
	identityNoP2 = L"43012419860309496X";
	mobileP2 = L"18912952106";
	seatTypeP2 = L"3";

	nameP3 = L"黄圣君";
	identityTypeP3 = L"1";	// 1 for identify card
	identityNoP3 = L"430124198711255011";
	mobileP3 = L"18912952106";
	seatTypeP3 = L"3";	// big O for second class seat

	nameP4 = L"谢文兴";
	identityTypeP4 = L"1";
	identityNoP4 = L"430124198710013328";
	mobileP4 = L"18912952106";
	seatTypeP4 = L"3";
}

void CHTTPSWebClientDlg::GetPassengerInfo()
{
	//theApp.ticketInfo.randCode =  validateBookStr;

	theApp.ticketInfo.passengers[0].seat = seatTypeP1;
	theApp.ticketInfo.passengers[0].name = nameP1;
	theApp.ticketInfo.passengers[0].cardtype = identityTypeP1;
	theApp.ticketInfo.passengers[0].cardno = identityNoP1;
	theApp.ticketInfo.passengers[0].mobileno = mobileP1;
	theApp.ticketInfo.passengers[1].seat = seatTypeP2;
	theApp.ticketInfo.passengers[1].name = nameP2;
	theApp.ticketInfo.passengers[1].cardtype = identityTypeP2;
	theApp.ticketInfo.passengers[1].cardno = identityNoP2;
	theApp.ticketInfo.passengers[1].mobileno = mobileP2;

	theApp.ticketInfo.passengers[2].seat = seatTypeP3;
	theApp.ticketInfo.passengers[2].name = nameP3;
	theApp.ticketInfo.passengers[2].cardtype = identityTypeP3;
	theApp.ticketInfo.passengers[2].cardno = identityNoP3;
	theApp.ticketInfo.passengers[2].mobileno = mobileP3;
	theApp.ticketInfo.passengers[3].seat = seatTypeP4;
	theApp.ticketInfo.passengers[3].name = nameP4;
	theApp.ticketInfo.passengers[3].cardtype = identityTypeP4;
	theApp.ticketInfo.passengers[3].cardno = identityNoP4;
	theApp.ticketInfo.passengers[3].mobileno = mobileP4;

	theApp.queryInfo.departDate = dateString;
	theApp.queryInfo.fromStation = stationFrom;
	theApp.queryInfo.toStation = stationTo;
	theApp.trainInfo.stationFromTeName = stationFrom;
	theApp.trainInfo.stationToTeName = stationTo;
	theApp.trainInfo.trainCode = trainNo;
	theApp.trainInfo.trainDate = dateString;
	theApp.trainInfo.trainRoundDate = dateString;
}

void CHTTPSWebClientDlg::GetUserInfo()
{
	theApp.loginInfo.username = usernameStr;
	theApp.loginInfo.password = passwordStr;
}

LRESULT CHTTPSWebClientDlg::OnSetStr( WPARAM wParam, LPARAM lParam )
{
	// wParam:
	// high word: thread count
	// low word: stage
	// lParam: data
	return 1;
}

LRESULT CHTTPSWebClientDlg::OnFinish( WPARAM wParam, LPARAM lParam )
{
	// wParam:
	// high word: thread count
	// low word: stage
	// lParam: data
	return 1;
}
