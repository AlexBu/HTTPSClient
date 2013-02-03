
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
	, usernameStr(_T(""))
	, passwordStr(_T(""))
	, trainNo(_T(""))
	, stationFrom(_T(""))
	, stationTo(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CHTTPSWebClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_USERNAME, usernameStr);
	DDX_Text(pDX, IDC_EDIT_PASSWORD, passwordStr);
	DDX_Text(pDX, IDC_EDIT_TRAIN, trainNo);
	DDX_Text(pDX, IDC_EDIT_STATION_FROM, stationFrom);
	DDX_Text(pDX, IDC_EDIT_STATION_TO, stationTo);
	DDX_Control(pDX, IDC_EDIT_GET, outputBox);
	DDX_Control(pDX, IDC_LIST_PASSENGER_BOARD, listctrlPassengers);
	DDX_Control(pDX, IDC_DATETIMEPICKER_DATE, ctrlDate);
}

BEGIN_MESSAGE_MAP(CHTTPSWebClientDlg, CDialog)
	//AFX_MSG_MAP{{(CHTTPSWebClientDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_SETSTR, CHTTPSWebClientDlg::OnSetStr)
	ON_MESSAGE(WM_LOGIN, CHTTPSWebClientDlg::OnLogin)
	ON_MESSAGE(WM_GETCODE, CHTTPSWebClientDlg::OnGetCode)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_BOOK, &CHTTPSWebClientDlg::OnBook)
	ON_BN_CLICKED(IDC_BUTTON_SELECT_USER, &CHTTPSWebClientDlg::OnSelectUser)
	ON_BN_CLICKED(IDC_BUTTON_SELECT_PASS, &CHTTPSWebClientDlg::OnSelectPassenger)
	ON_BN_CLICKED(IDC_BUTTON_ADDPASS, &CHTTPSWebClientDlg::OnAddPassenger)
	ON_BN_CLICKED(IDC_BUTTON_REMOVEPASS, &CHTTPSWebClientDlg::OnRemovePassenger)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_PASSENGER_BOARD, &CHTTPSWebClientDlg::OnNMDblclkListPassengerBoard)
END_MESSAGE_MAP()


// CHTTPSWebClientDlg 消息处理程序

BOOL CHTTPSWebClientDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// init date ctrl
	InitDate();


	LoadConfig();

	UpdateData(FALSE);

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

void CHTTPSWebClientDlg::OnBook()
{
	UpdateData(TRUE);

	// fill up user info
	GetUserInfo();
	// fill up passengers info
	GetPassengerInfo();

	theApp.BookTickets();
	// set response
	UpdateData(FALSE);
}

void CHTTPSWebClientDlg::GetUserInfo()
{
	theApp.loginInfo.username = usernameStr;
	theApp.loginInfo.password = passwordStr;
}

void CHTTPSWebClientDlg::GetPassengerInfo()
{
	//theApp.ticketInfo.randCode =  validateBookStr;

	for(int i = 0; i < listPassengers.GetCount(); i++)
	{
		theApp.ticketInfo.passengers.Add(listPassengers[i]);
	}

	CTime departDate;
	ctrlDate.GetTime(departDate);
	CString str = departDate.Format(L"%Y-%m-%d");
	theApp.queryInfo.departDate = str;
	theApp.queryInfo.fromStation = stationFrom;
	theApp.queryInfo.toStation = stationTo;
	theApp.trainInfo.stationFromTeName = stationFrom;
	theApp.trainInfo.stationToTeName = stationTo;
	theApp.trainInfo.trainCode = trainNo;
	theApp.trainInfo.trainDate = str;
	theApp.trainInfo.trainRoundDate = str;
}

LRESULT CHTTPSWebClientDlg::OnSetStr( WPARAM wParam, LPARAM lParam )
{
	// wParam:
	// high word: thread count
	// low word: stage
	// lParam: data

	CString output;
	outputBox.GetWindowText(output);
	output += (LPCTSTR)lParam;
	output += L"\r\n";
	outputBox.SetWindowText(output);
	outputBox.LineScroll(outputBox.GetLineCount());
	return 1;
}

LRESULT CHTTPSWebClientDlg::OnLogin( WPARAM wParam, LPARAM lParam )
{
	// wParam:
	// high word: thread count
	// low word: stage
	// lParam: data

	// update config

	// update account
	UpdateUserListConfig();
	// update passenger
	UpdatePassListConfig();

	return 1;
}

LRESULT CHTTPSWebClientDlg::OnGetCode( WPARAM wParam, LPARAM lParam )
{
	// wParam:
	// high word: thread count
	// low word: stage
	// lParam: data
	int valTyp = (int)lParam;

	// show validate picture
	if(valTyp == 0)
		theApp.GetLoginValCodeInput();
	else
		theApp.GetBookValCodeInput();

	return 1;
}

void CHTTPSWebClientDlg::LoadConfig()
{
	InitPassengerListCtrl();
}

void CHTTPSWebClientDlg::OnSelectUser()
{
	CSelectUser dlg;
	UpdateData(TRUE);
	if(dlg.DoModal() == IDOK)
	{
		// update current username / password
		UpdateUser(dlg.selecteduser);
	}
}

void CHTTPSWebClientDlg::OnSelectPassenger()
{
	CSelectPassenger dlg;
	UpdateData(TRUE);
	if(dlg.DoModal() == IDOK)
	{
		// update current passenger list
		// if exist, update
		// if not exist, append
		// if size exceed, remove first then append
		for(int i = 0; i < dlg.selectedpass.GetCount(); i++)
		{
			UpdateOneBoardPassenger(dlg.selectedpass[i]);
		}

		// update list ctrl
		UpdatePassengerListCtrl();

		UpdateData(FALSE);
	}
}

void CHTTPSWebClientDlg::InitPassengerListCtrl()
{
	listctrlPassengers.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	listctrlPassengers.InsertColumn(0, L"Name");
	listctrlPassengers.InsertColumn(1, L"SeatTyp");

	listctrlPassengers.SetColumnWidth(0, 150);
	listctrlPassengers.SetColumnWidth(1, LVSCW_AUTOSIZE_USEHEADER);
}

void CHTTPSWebClientDlg::UpdatePassengerListCtrl()
{
	listctrlPassengers.DeleteAllItems();
	for(int i = 0; i < listPassengers.GetCount(); i++)
	{
		PassInfo& passengerinfo = listPassengers[i];
		// load into list ctrl
		listctrlPassengers.InsertItem(i, passengerinfo.name);
		listctrlPassengers.SetItemText(i, 1, passengerinfo.seatTyp);
	}
}

void CHTTPSWebClientDlg::UpdateUserListConfig()
{
	CArray<UserInfo>& userlist = CConfig::GetConfig().GetUser();
	BOOL isFound = FALSE;
	BOOL needUpdate = FALSE;
	for(int i = 0; i < userlist.GetCount(); i++)
	{
		if(userlist[i].name == usernameStr)
		{
			isFound = TRUE;
			if(userlist[i].pass != passwordStr)
			{
				userlist[i].pass = passwordStr;
				needUpdate = TRUE;
			}
			break;
		}
	}
	if(!isFound)
	{
		UserInfo userinfo;
		userinfo.name = usernameStr;
		userinfo.pass = passwordStr;
		userlist.Add(userinfo);
		needUpdate = TRUE;
	}
	if(needUpdate)
		CConfig::GetConfig().SetUpdate();
}

void CHTTPSWebClientDlg::UpdatePassListConfig()
{
	// TODO
	CArray<PassInfo>& passlist = CConfig::GetConfig().GetPassenger();
	BOOL isFound = FALSE;
	BOOL isUpdate = FALSE;
	for(int i = 0; i < listPassengers.GetCount(); i++)
	{
		isFound = FALSE;
		int needupdateindex = -1;
		for(int j = 0; j < passlist.GetCount(); j++)
		{
			if(passlist[j].name == listPassengers[i].name)
			{
				isFound = TRUE;
				if( (passlist[j].passTyp != listPassengers[i].passTyp) 
					|| (passlist[j].passNo != listPassengers[i].passNo) 
					|| (passlist[j].mobileNo != listPassengers[i].mobileNo) 
					|| (passlist[j].seatTyp != listPassengers[i].seatTyp) )
				{
					needupdateindex = j;
				}
				break;
			}
		}
		if(needupdateindex > -1)
		{
			passlist[needupdateindex] = listPassengers[i];
			isUpdate = TRUE;
		}
		if(!isFound)
		{
			passlist.Add(listPassengers[i]);
			isUpdate = TRUE;
		}
	}

	if(isUpdate)
		CConfig::GetConfig().SetUpdate();
}

void CHTTPSWebClientDlg::OnAddPassenger()
{
	if(listPassengers.GetCount() >= 5)
		MessageBox(L"can not add more passengers!", L"add passenger", MB_OK);
	CEditPassenger dlg;
	if(dlg.DoModal() == IDOK)
	{
		UpdateOneBoardPassenger(dlg.passenger);
		UpdatePassengerListCtrl();
	}
}

void CHTTPSWebClientDlg::OnRemovePassenger()
{
	POSITION pos = listctrlPassengers.GetFirstSelectedItemPosition();

	while (pos)
	{
		int nItem = listctrlPassengers.GetNextSelectedItem(pos);
		// remove this row
		int rowtodel = -1;
		for(int i = 0; i < listPassengers.GetCount(); i++)
		{
			if(listPassengers[i].name == listctrlPassengers.GetItemText(nItem, 0))
			{
				rowtodel = i;
				break;
			}
		}
		if(rowtodel > -1)
		{
			listPassengers.RemoveAt(rowtodel);
		}
		listctrlPassengers.DeleteItem(nItem);
	}

}

void CHTTPSWebClientDlg::OnNMDblclkListPassengerBoard(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	
	*pResult = 0;

	if(pNMItemActivate->iItem == -1)
		return;

	for(int i = 0; i < listPassengers.GetCount(); i++)
	{
		if(listPassengers[i].name == listctrlPassengers.GetItemText(pNMItemActivate->iItem, 0))
		{
			CEditPassenger dlg;
			dlg.passenger = listPassengers[i];
			if(dlg.DoModal() == IDOK)
			{
				// update
				UpdateOneBoardPassenger(dlg.passenger);
				UpdatePassengerListCtrl();
			}
			break;
		}
	}
}

void CHTTPSWebClientDlg::UpdateOneBoardPassenger( PassInfo& passinfo )
{
	BOOL isFound = FALSE;
	for(int j = 0; j < listPassengers.GetCount(); j++)
	{
		if(listPassengers[j].name == passinfo.name)
		{
			// overwrite
			listPassengers[j] = passinfo;
			isFound = TRUE;
			break;
		}
	}
	if(isFound == FALSE)
	{
		if(listPassengers.GetCount() >= 5)
		{
			// remove first
			listPassengers.RemoveAt(0);
		}
		// append
		listPassengers.Add(passinfo);
	}
}

void CHTTPSWebClientDlg::UpdateUser( UserInfo &userinfo )
{
	usernameStr = userinfo.name;
	passwordStr = userinfo.pass;
	UpdateData(FALSE);
}

void CHTTPSWebClientDlg::InitDate()
{
	CTime orderdate = CTime::GetCurrentTime();
	CTimeSpan orderspan(20, 0, 0, 0);
	orderdate += orderspan;
	ctrlDate.SetTime(&orderdate);
}

