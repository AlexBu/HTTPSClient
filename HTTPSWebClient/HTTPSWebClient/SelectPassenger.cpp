// SelectPassenger.cpp : implementation file
//

#include "stdafx.h"
#include "HTTPSWebClient.h"
#include "SelectPassenger.h"


// CSelectPassenger dialog

IMPLEMENT_DYNAMIC(CSelectPassenger, CDialog)

CSelectPassenger::CSelectPassenger(CWnd* pParent /*=NULL*/)
	: CDialog(CSelectPassenger::IDD, pParent)
{

}

CSelectPassenger::~CSelectPassenger()
{
}

void CSelectPassenger::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_PASSENGER, listboxPassenger);
}


BEGIN_MESSAGE_MAP(CSelectPassenger, CDialog)
END_MESSAGE_MAP()

// CSelectPassenger message handlers

BOOL CSelectPassenger::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	// display passenger list except the passengers outside
	CArray<PassInfo>& passlist = CConfig::GetConfig().GetPassenger();
	for(int i = 0; i < passlist.GetCount(); i++)
	{
		for(int j = 0; j < passOutside.GetCount(); j++)
		{
			if(passlist[i].name == passOutside[j])
				continue;
		}
		listboxPassenger.InsertString(0, passlist[i].name);
	}

	// test code
	listboxPassenger.InsertString(0, L"test1");
	listboxPassenger.InsertString(0, L"test1");
	listboxPassenger.InsertString(0, L"test1");
	listboxPassenger.InsertString(0, L"test1");
	listboxPassenger.InsertString(0, L"test1");
	listboxPassenger.InsertString(0, L"test1");
	listboxPassenger.InsertString(0, L"test1");
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CSelectPassenger::OnSelectPassenger()
{
	int selCount = listboxPassenger.GetSelCount();
	CArray<PassInfo>& passlist = CConfig::GetConfig().GetPassenger();
	CArray<int> selectedIndex;
	selectedIndex.SetSize(selCount);
	listboxPassenger.GetSelItems(selCount, selectedIndex.GetData());

	for(int i = 0; i < passlist.GetCount(); i++)
	{
		for(int j = 0; j < selectedIndex.GetCount(); j++)
		{
			CString str;
			listboxPassenger.GetText(j, str);
			if(passlist[i].name == str)
			{
				selectedpass.Add(passlist[i]);
			}
		}
	}
}

void CSelectPassenger::OnOK()
{
	if(listboxPassenger.GetSelCount() > 5)
	{
		MessageBox(L"cannot choose more than 5 passengers", L"select passenger", MB_OK);
		return;
	}
	OnSelectPassenger();
	CDialog::OnOK();
}