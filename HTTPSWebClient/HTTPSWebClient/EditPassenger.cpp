// EditPassenger.cpp : implementation file
//

#include "stdafx.h"
#include "HTTPSWebClient.h"
#include "EditPassenger.h"


// CEditPassenger dialog

IMPLEMENT_DYNAMIC(CEditPassenger, CDialog)

CEditPassenger::CEditPassenger(CWnd* pParent /*=NULL*/)
	: CDialog(CEditPassenger::IDD, pParent)
	, passname(_T(""))
	, passmobile(_T(""))
	, passidno(_T(""))
{

}

CEditPassenger::~CEditPassenger()
{
}

void CEditPassenger::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_PASSENGER_NAME, passname);
	DDX_Text(pDX, IDC_EDIT_PASSENGER_MOBILE, passmobile);
	DDX_Text(pDX, IDC_EDIT_PASSENGER_IDNO, passidno);
	DDX_Control(pDX, IDC_COMBO_PASSENGER_SEAT, cboxSeatType);
	DDX_Control(pDX, IDC_COMBO_PASSENGER_IDTYPE, cboxIdTyp);
}


BEGIN_MESSAGE_MAP(CEditPassenger, CDialog)
END_MESSAGE_MAP()


// CEditPassenger message handlers

BOOL CEditPassenger::OnInitDialog()
{
	CDialog::OnInitDialog();

	InitList();
	LoadPassenger();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CEditPassenger::OnOK()
{
	SavePassenger();

	CDialog::OnOK();
}

void CEditPassenger::InitList()
{
	cboxSeatType.AddString(L"二等座");
	cboxSeatType.AddString(L"一等座");
	cboxSeatType.AddString(L"商务座");
	cboxSeatType.AddString(L"硬卧");
	cboxSeatType.AddString(L"软卧");
	cboxSeatType.AddString(L"软座");
	cboxSeatType.AddString(L"硬座");
	cboxSeatType.AddString(L"特等座");
	cboxSeatType.AddString(L"高级软座");
	cboxSeatType.AddString(L"高级软卧");
	cboxSeatType.AddString(L"其他");
	
	cboxIdTyp.AddString(L"二代身份证");
	cboxIdTyp.AddString(L"一代身份证");
	cboxIdTyp.AddString(L"港澳通行证");
	cboxIdTyp.AddString(L"台湾通行证");
	cboxIdTyp.AddString(L"护照");
}

void CEditPassenger::LoadPassenger()
{
	passname = passenger.name;
	passidno = passenger.passNo;
	passmobile = passenger.mobileNo;

	if(passenger.seatTyp == L"O")
		cboxSeatType.SetCurSel(0);
	else if(passenger.seatTyp == L"M")
		cboxSeatType.SetCurSel(1);
	else if(passenger.seatTyp == L"9")
		cboxSeatType.SetCurSel(2);
	else if(passenger.seatTyp == L"3")
		cboxSeatType.SetCurSel(3);
	else if(passenger.seatTyp == L"4")
		cboxSeatType.SetCurSel(4);
	else if(passenger.seatTyp == L"2")
		cboxSeatType.SetCurSel(5);
	else if(passenger.seatTyp == L"1")
		cboxSeatType.SetCurSel(6);
	else if(passenger.seatTyp == L"P")
		cboxSeatType.SetCurSel(7);
	else if(passenger.seatTyp == L"E")
		cboxSeatType.SetCurSel(8);
	else if(passenger.seatTyp == L"6")
		cboxSeatType.SetCurSel(9);
	else if(passenger.seatTyp == L"Z")
		cboxSeatType.SetCurSel(10);
	else
		cboxSeatType.SetCurSel(0);

	if(passenger.passTyp == L"1")
		cboxIdTyp.SetCurSel(0);
	else if(passenger.passTyp == L"2")
		cboxIdTyp.SetCurSel(1);
	else if(passenger.passTyp == L"C")
		cboxIdTyp.SetCurSel(2);
	else if(passenger.passTyp == L"G")
		cboxIdTyp.SetCurSel(3);
	else if(passenger.passTyp == L"B")
		cboxIdTyp.SetCurSel(4);
	else
		cboxIdTyp.SetCurSel(0);

	UpdateData(FALSE);
}

void CEditPassenger::SavePassenger()
{
	UpdateData(TRUE);

	passenger.name = passname;
	passenger.passNo = passidno;
	passenger.mobileNo = passmobile;

	CString idtypeList[] = {
		L"1", L"2", L"C", L"G", L"B"
	};

	passenger.passTyp = idtypeList[cboxIdTyp.GetCurSel()];

	CString seattypeList[] = {
		L"O", L"M", L"9", 
		L"3", L"4", L"2",
		L"1", L"P", L"E",
		L"6", L"Z"
	};

	passenger.seatTyp = seattypeList[cboxSeatType.GetCurSel()];
}
