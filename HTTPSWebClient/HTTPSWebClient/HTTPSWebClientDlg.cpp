
// HTTPSWebClientDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "HTTPSWebClient.h"
#include "HTTPSWebClientDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CHTTPSWebClientDlg �Ի���

CHTTPSWebClientDlg::CHTTPSWebClientDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHTTPSWebClientDlg::IDD, pParent)
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
	DDX_Control(pDX, IDC_EDIT_GET, outputBox);
	DDX_Control(pDX, IDC_LIST_PASSENGER_BOARD, listctrlPassengers);
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


// CHTTPSWebClientDlg ��Ϣ�������

BOOL CHTTPSWebClientDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	
	InitUserInfo();
	InitPassengerInfo();

	LoadConfig();

	UpdateData(FALSE);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CHTTPSWebClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
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

void CHTTPSWebClientDlg::InitUserInfo()
{
	usernameStr = L"bkp84335";
	passwordStr = L"bsp2236";
}

void CHTTPSWebClientDlg::InitPassengerInfo()
{
	dateString= L"2013-02-03";
	trainNo = L"D305";
	stationFrom = L"�Ͼ�";
	stationTo = L"�Ϻ�";

	nameP1 = L"������";
	identityTypeP1 = L"1";	// 1 for identify card
	identityNoP1 = L"32032119831230701X";
	mobileP1 = L"18912952106";
	seatTypeP1 = L"3";	// big O for second class seat

	nameP2 = L"����";
	identityTypeP2 = L"1";
	identityNoP2 = L"43012419860309496X";
	mobileP2 = L"18912952106";
	seatTypeP2 = L"3";

	nameP3 = L"��ʥ��";
	identityTypeP3 = L"1";	// 1 for identify card
	identityNoP3 = L"430124198711255011";
	mobileP3 = L"18912952106";
	seatTypeP3 = L"3";	// big O for second class seat

	nameP4 = L"л����";
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
