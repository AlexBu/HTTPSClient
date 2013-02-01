// SelectUser.cpp : implementation file
//

#include "stdafx.h"
#include "HTTPSWebClient.h"
#include "SelectUser.h"


// CSelectUser dialog

IMPLEMENT_DYNAMIC(CSelectUser, CDialog)

CSelectUser::CSelectUser(CWnd* pParent /*=NULL*/)
	: CDialog(CSelectUser::IDD, pParent)
{

}

CSelectUser::~CSelectUser()
{
}

void CSelectUser::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_USER, listboxUser);
}


BEGIN_MESSAGE_MAP(CSelectUser, CDialog)
	//AFX_MSG_MAP{{(CSelectUser)
	//}}AFX_MSG_MAP
	ON_LBN_DBLCLK(IDC_LIST_USER, &CSelectUser::OnDblClkUserList) 
END_MESSAGE_MAP()


// CSelectUser message handlers

BOOL CSelectUser::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	// display user list except the user outside
	CArray<UserInfo>& userlist = CConfig::GetConfig().GetUser();
	for(int i = 0; i < userlist.GetCount(); i++)
	{
		if(userlist[i].name == userOutside)
			continue;
		listboxUser.InsertString(0, userlist[i].name);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CSelectUser::OnDblClkUserList()
{
	OnSelectUser();
	EndDialog(IDOK);
}

void CSelectUser::OnSelectUser()
{
	int index = listboxUser.GetCurSel();
	if(index == -1)
		return;

	CString str;
	listboxUser.GetText(index, str);
	if(str.IsEmpty())
		return;

	// search list
	CArray<UserInfo>& userlist = CConfig::GetConfig().GetUser();
	for(int i = 0; i < userlist.GetCount(); i++)
	{
		if(userlist[i].name == str)
		{
			selecteduser = userlist[i];
			break;
		}
	}
}

void CSelectUser::OnOK()
{
	OnSelectUser();
	CDialog::OnOK();
}
