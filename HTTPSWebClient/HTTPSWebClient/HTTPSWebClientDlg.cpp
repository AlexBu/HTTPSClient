
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
	, fromStation(_T(""))
	, toStation(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CHTTPSWebClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_USERNAME, usernameStr);
	DDX_Text(pDX, IDC_EDIT_PASSWORD, passwordStr);
	DDX_Text(pDX, IDC_EDIT_TRAIN, trainNo);
	DDX_Text(pDX, IDC_EDIT_STATION_FROM, fromStation);
	DDX_Text(pDX, IDC_EDIT_STATION_TO, toStation);
	DDX_Control(pDX, IDC_EDIT_GET, outputBox);
	DDX_Control(pDX, IDC_LIST_PASSENGER_BOARD, ctrlPassengerList);
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
	ON_BN_CLICKED(IDC_BUTTON_ADDPASS, &CHTTPSWebClientDlg::OnAddBoardPassenger)
	ON_BN_CLICKED(IDC_BUTTON_REMOVEPASS, &CHTTPSWebClientDlg::OnRemoveBoardPassenger)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_PASSENGER_BOARD, &CHTTPSWebClientDlg::OnNMDblclkListBoardPassenger)
	ON_WM_CLOSE()
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

	// collect input info
	CollectInputInfo();

	theApp.BookTickets();
	// set response
	UpdateData(FALSE);
}

void CHTTPSWebClientDlg::CollectInputInfo()
{
	theApp.loginInfo.username = usernameStr;
	theApp.loginInfo.password = passwordStr;

	for(int i = 0; i < dataPassengerList.GetCount(); i++)
	{
		theApp.ticketInfo.passengers.Add(dataPassengerList[i]);
	}

	CTime departDate;
	ctrlDate.GetTime(departDate);
	CString str = departDate.Format(L"%Y-%m-%d");
	theApp.queryInfo.departDate = str;
	theApp.queryInfo.fromStation = fromStation;
	theApp.queryInfo.toStation = toStation;
	theApp.trainInfo.stationFromTeName = fromStation;
	theApp.trainInfo.stationToTeName = toStation;
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
	UpdatePassengerListConfig();

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

	SetLastCloseInput();

	UpdateBoardPassengerListCtrl();
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
		UpdateBoardPassengerListCtrl();

		UpdateData(FALSE);
	}
}

void CHTTPSWebClientDlg::InitPassengerListCtrl()
{
	ctrlPassengerList.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	ctrlPassengerList.InsertColumn(0, L"Name");
	ctrlPassengerList.InsertColumn(1, L"SeatTyp");

	ctrlPassengerList.SetColumnWidth(0, 150);
	ctrlPassengerList.SetColumnWidth(1, LVSCW_AUTOSIZE_USEHEADER);
}

void CHTTPSWebClientDlg::UpdateBoardPassengerListCtrl()
{
	ctrlPassengerList.DeleteAllItems();
	for(int i = 0; i < dataPassengerList.GetCount(); i++)
	{
		PassengerInfo& passengerinfo = dataPassengerList[i];
		// load into list ctrl
		ctrlPassengerList.InsertItem(i, passengerinfo.name);
		ctrlPassengerList.SetItemText(i, 1, passengerinfo.seatTyp);
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

void CHTTPSWebClientDlg::UpdatePassengerListConfig()
{
	// TODO
	CArray<PassengerInfo>& passlist = CConfig::GetConfig().GetPassenger();
	BOOL isFound = FALSE;
	BOOL isUpdate = FALSE;
	for(int i = 0; i < dataPassengerList.GetCount(); i++)
	{
		isFound = FALSE;
		int needupdateindex = -1;
		for(int j = 0; j < passlist.GetCount(); j++)
		{
			if(passlist[j].name == dataPassengerList[i].name)
			{
				isFound = TRUE;
				if( (passlist[j].passTyp != dataPassengerList[i].passTyp) 
					|| (passlist[j].passNo != dataPassengerList[i].passNo) 
					|| (passlist[j].mobileNo != dataPassengerList[i].mobileNo) 
					|| (passlist[j].seatTyp != dataPassengerList[i].seatTyp) )
				{
					needupdateindex = j;
				}
				break;
			}
		}
		if(needupdateindex > -1)
		{
			passlist[needupdateindex] = dataPassengerList[i];
			isUpdate = TRUE;
		}
		if(!isFound)
		{
			passlist.Add(dataPassengerList[i]);
			isUpdate = TRUE;
		}
	}

	if(isUpdate)
		CConfig::GetConfig().SetUpdate();
}

void CHTTPSWebClientDlg::OnAddBoardPassenger()
{
	if(dataPassengerList.GetCount() >= 5)
		MessageBox(L"can not add more passengers!", L"add passenger", MB_OK);
	CEditPassenger dlg;
	if(dlg.DoModal() == IDOK)
	{
		UpdateOneBoardPassenger(dlg.passenger);
		UpdateBoardPassengerListCtrl();
	}
}

void CHTTPSWebClientDlg::OnRemoveBoardPassenger()
{
	POSITION pos = ctrlPassengerList.GetFirstSelectedItemPosition();

	while (pos)
	{
		int nItem = ctrlPassengerList.GetNextSelectedItem(pos);
		// remove this row
		int rowtodel = -1;
		for(int i = 0; i < dataPassengerList.GetCount(); i++)
		{
			if(dataPassengerList[i].name == ctrlPassengerList.GetItemText(nItem, 0))
			{
				rowtodel = i;
				break;
			}
		}
		if(rowtodel > -1)
		{
			dataPassengerList.RemoveAt(rowtodel);
		}
		ctrlPassengerList.DeleteItem(nItem);
	}

}

void CHTTPSWebClientDlg::OnNMDblclkListBoardPassenger(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	
	*pResult = 0;

	if(pNMItemActivate->iItem == -1)
		return;

	for(int i = 0; i < dataPassengerList.GetCount(); i++)
	{
		if(dataPassengerList[i].name == ctrlPassengerList.GetItemText(pNMItemActivate->iItem, 0))
		{
			CEditPassenger dlg;
			dlg.passenger = dataPassengerList[i];
			if(dlg.DoModal() == IDOK)
			{
				// update
				UpdateOneBoardPassenger(dlg.passenger);
				UpdateBoardPassengerListCtrl();
			}
			break;
		}
	}
}

void CHTTPSWebClientDlg::UpdateOneBoardPassenger( PassengerInfo& passinfo )
{
	BOOL isFound = FALSE;
	for(int j = 0; j < dataPassengerList.GetCount(); j++)
	{
		if(dataPassengerList[j].name == passinfo.name)
		{
			// overwrite
			dataPassengerList[j] = passinfo;
			isFound = TRUE;
			break;
		}
	}
	if(isFound == FALSE)
	{
		if(dataPassengerList.GetCount() >= 5)
		{
			// remove first
			dataPassengerList.RemoveAt(0);
		}
		// append
		dataPassengerList.Add(passinfo);
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
	CTimeSpan orderspan(20 - 1, 0, 0, 0);
	orderdate += orderspan;
	ctrlDate.SetTime(&orderdate);
}

void CHTTPSWebClientDlg::GetLastCloseInput()
{
	InputInfo& input = CConfig::GetConfig().GetInput();
	input.userinfo.name = usernameStr;
	input.userinfo.pass = passwordStr;
	input.passengerlist.pass.RemoveAll();
	for(int i = 0; i < dataPassengerList.GetCount(); i++)
	{
		input.passengerlist.pass.Add(dataPassengerList[i]);
	}
	input.passengerlist.count = input.passengerlist.pass.GetCount();
	input.fromStation = fromStation;
	input.toStation = toStation;
	input.trainNo = trainNo;

	CTime departDate;
	ctrlDate.GetTime(departDate);
	input.date = departDate.Format(L"%Y-%m-%d");
}

void CHTTPSWebClientDlg::OnClose()
{
	UpdateData(TRUE);
	GetLastCloseInput();
	CConfig::GetConfig().SetUpdate();
	CConfig::GetConfig().ReleaseConfig();
	CDialog::OnClose();
}

void CHTTPSWebClientDlg::SetLastCloseInput()
{
	InputInfo& input = CConfig::GetConfig().GetInput();
	usernameStr = input.userinfo.name;
	passwordStr = input.userinfo.pass;

	dataPassengerList.RemoveAll();
	for(int i = 0; i < input.passengerlist.pass.GetCount(); i++)
	{
		dataPassengerList.Add(input.passengerlist.pass[i]);
	}

	fromStation = input.fromStation;
	toStation = input.toStation;
	trainNo = input.trainNo;
}
