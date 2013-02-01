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

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CSelectPassenger::OnSelectPassenger()
{
	CArray<PassInfo>& passlist = CConfig::GetConfig().GetPassenger();
	for(int i = 0; i < passlist.GetCount(); i++)
	{
		for(int j = 0; j < passOutside.GetCount(); j++)
		{
			if(passlist[i].name == passOutside[j])
				;
		}
	}
}

void CSelectPassenger::OnOK()
{
	OnSelectPassenger();
	CDialog::OnOK();
}